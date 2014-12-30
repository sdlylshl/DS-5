#include "def.h"
#include "2440addr.h"
#include "2440lib.h"

#include "CH7026.h"
#include "bootpara.h"

/************************************************/
//some macros and inline function to support porting
//from linux driver
#define	CLKCON					rCLKCON
#define	CLKCON_CAMIF		(1<<19)
#define	UPLLCON					rUPLLCON
#define	CLKDIVN					rCLKDIVN
#define	DIVN_UPLL_EN			(1<<3)
#define	CAMDIVN					rCAMDIVN
#define	CAMCLK_SET_DIV			(1<<4)
#define	CAM_CTRL				rCIGCTRL	//??

#define	GPIO_E14				14
#define	GPIO_E15				15
#define	GPIO_PULLUP_EN			(0<<30)
#define	GPIO_PULLUP_DIS			(1<<30)
#define	GPIO_MODE_IN			(0<<31)
#define	GPIO_MODE_OUT			(1UL<<31)

#define	read_gpio_bit(x)		((rGPEDAT & (1<<(x))) ? 1 : 0)
#define	write_gpio_bit_set(x)	(rGPEDAT |= 1<<(x))
#define	write_gpio_bit_clear(x)	(rGPEDAT &= ~(1<<(x)))
//only support GPE, in and out
static __inline void set_gpio_ctrl(unsigned int gpio)
{
	rGPECON &= ~(3<<((gpio&0xf)*2));
	if(gpio&GPIO_MODE_OUT)
		rGPECON |= 1<<((gpio&0xf)*2);
	if(gpio&GPIO_PULLUP_DIS)
		rGPEUP |= 1<<(gpio&0xf);
	else
		rGPEUP &= ~(1<<(gpio&0xf));
}

#define	ENODEV			2
#define	udelay(x)		Delay(x/50)
#define	mdelay(x)		Delay((x)*8)
#define	printk			Uart_Printf

typedef unsigned long dma_addr_t;



/*********** start of CH7026 ********************/
/* refer to 25p of CH7026 datasheet */
#define CH7026_WRITE_ID 0XEC
#define CH7026_READ_ID 0XED

#define CH7026_MAX_CLK 24000000 // 24MHz
#define CH7026_PRODUCT_ID 0x54


#define CH7026_SCCB_DELAY    100 
#define CH7026_SCCB_DELAY2   100

#define SIO_C          (GPIO_E14)
#define SIO_D          (GPIO_E15)
#define MAKE_HIGH(_x)  write_gpio_bit_set(_x)
#define MAKE_LOW(_x)   write_gpio_bit_clear(_x)
#define BIT_READ(_x)   read_gpio_bit(_x)
#define CFG_READ(_x)   set_gpio_ctrl(_x | GPIO_PULLUP_DIS | GPIO_MODE_IN)
#define CFG_WRITE(_x)  set_gpio_ctrl(_x | GPIO_PULLUP_DIS | GPIO_MODE_OUT)


#if CH7026_SCCB_DELAY > 0
#define WAIT_CYL udelay(CH7026_SCCB_DELAY)
#else
#define WAIT_CYL (void)(0)
#endif


#if CH7026_SCCB_DELAY2 > 0
#define WAIT_STAB udelay(CH7026_SCCB_DELAY2)
#else
#define WAIT_STAB (void)(0)
#endif

static unsigned int get_camera_clk(void)
{
	return 24000000;//CH7026_MAX_CLK;
}

/* 2-wire SCCB */
void __inline CH7026_sccb_start(void)
{
	MAKE_HIGH(SIO_C);
	MAKE_HIGH(SIO_D);
	WAIT_STAB;
	MAKE_LOW(SIO_D);
	WAIT_STAB;
	MAKE_LOW(SIO_C);
	WAIT_STAB;
}

/* 2-wire SCCB */
void __inline CH7026_sccb_end(void)
{
	MAKE_LOW(SIO_D);
	WAIT_STAB;
	MAKE_HIGH(SIO_C);
	WAIT_STAB;
	MAKE_HIGH(SIO_D);
	WAIT_STAB;
}

void __inline CH7026_sccb_write_bit(unsigned char bit)
{
	if (bit)
		MAKE_HIGH(SIO_D);
	else
		MAKE_LOW(SIO_D);
	WAIT_STAB;
	MAKE_HIGH(SIO_C);
	WAIT_CYL;
	MAKE_LOW(SIO_C);
	WAIT_STAB;
}

int __inline CH7026_sccb_read_bit(void)
{
	int tmp = 0;

	MAKE_HIGH(SIO_C);
	WAIT_CYL;
	tmp = BIT_READ(SIO_D);
	MAKE_LOW(SIO_C);
	WAIT_STAB;

	return tmp;
}	

void __inline CH7026_sccb_writechar(unsigned char data)
{
	int i = 0;

	/* data */
	for (i = 0; i < 8; i++ ) {
		CH7026_sccb_write_bit(data & 0x80);
		data <<= 1;
	}

	/* 9th bit - Don't care */
	CH7026_sccb_write_bit(1);
}

void __inline CH7026_sccb_readchar(unsigned char *val)
{
	int i;
	int tmp = 0;

	CFG_READ(SIO_D);

	for (i = 7; i >= 0; i--)
		tmp |= CH7026_sccb_read_bit() << i;

	CFG_WRITE(SIO_D);

	/* 9th bit - N.A. */
	CH7026_sccb_write_bit(1);

	*val = tmp & 0xff;
}

/* 3-phase write */
static void CH7026_sccb_sendbyte(unsigned char subaddr, unsigned char data)
{

	CH7026_sccb_start();
	CH7026_sccb_writechar(CH7026_WRITE_ID);
	CH7026_sccb_writechar(subaddr);
	CH7026_sccb_writechar(data);
	CH7026_sccb_end();

	mdelay(7);
}

/* 2-phase read */
static unsigned char CH7026_sccb_receivebyte(unsigned char subaddr)
{
	unsigned char value;


	/* 2-phase write */
	CH7026_sccb_start();
	CH7026_sccb_writechar(CH7026_WRITE_ID);
	CH7026_sccb_writechar(subaddr);
	CH7026_sccb_end();

	/* 2-phase read */
	CH7026_sccb_start();
	CH7026_sccb_writechar(CH7026_READ_ID);
	CH7026_sccb_readchar(&value);
	CH7026_sccb_end();

	mdelay(7);


	return value;
}

void __inline IIC_IO_init(void)
{
	CFG_WRITE(SIO_C);
	CFG_WRITE(SIO_D);
	mdelay(10);
}

void __inline IIC_IO_deinit(void)
{
	CFG_READ(SIO_C);
	CFG_READ(SIO_D);
}

void __inline CH7026_config(void)
{
	int i;
	if(1==boot_params.display_mode.val)
	{
		if(1==boot_params.display_sel.val)
		{
			for (i = 0; i < CH7026_REG_NUM_VGA_640; i++) {
				CH7026_sccb_sendbyte(CH7026_REGS_VGA_640[i].subaddr & 0xff, CH7026_REGS_VGA_640[i].value & 0xff);
			}		
			printk(" |VGA init done (VGA MODE 640*480)                            |\n");
			
		} else if(2==boot_params.display_sel.val)
		{
			for (i = 0; i < CH7026_REG_NUM_VGA_800; i++) {
				CH7026_sccb_sendbyte(CH7026_REGS_VGA_800[i].subaddr & 0xff, CH7026_REGS_VGA_800[i].value & 0xff);
			}
			printk(" |VGA init done (VGA MODE 800*600)                            |\n");
			
		}
		
	}
	else
	{
		for (i = 0; i < CH7026_REG_NUM_TV; i++) {
			CH7026_sccb_sendbyte(CH7026_REGS_TV[i].subaddr & 0xff, CH7026_REGS_TV[i].value & 0xff);
		}
		printk(" |VGA init done (TV MODE)                                     |\n");
	}
	

}

int __inline check_CH7026(void)
{
	int ret = 0;
	int CH7026_id = 0;
	
	CH7026_id = CH7026_sccb_receivebyte(0x00);
	
	if (CH7026_id != CH7026_PRODUCT_ID) {
		ret = -ENODEV;
	} 

	CH7026_id = CH7026_sccb_receivebyte(0x00);
	//printk("Product ID is 0x%04x\n", CH7026_id);

	return ret;
}

int CH7026_MemInit(void)
{
	int retval = 0;
	int count = 200; // try times
	CH7026_sccb_sendbyte ( 0x06, CH7026_sccb_receivebyte ( 0x06 ) | 0x02 ); // Set MEMINT to 1
	CH7026_sccb_sendbyte ( 0x06, CH7026_sccb_receivebyte ( 0x06 ) & 0xFD); // Set MEMINT to 0

	while( count -- )
	{
		if ( CH7026_sccb_receivebyte ( 0x7E ) & 0x08 )
			return 1;//retval = 1;
		mdelay ( 1 ); // any delay function can be accepted here
	}
	return retval;
}


/***************************************************/
int VGA_init(void)
{
	int ret;
	IIC_IO_init();
	if(!boot_params.display_mode.val)return 0;//TV mode	

	ret = check_CH7026() ;
	if (ret) {
		printk(" |Can't find VGA chip !                                       |\n");
		return ret;
	}
	
	printk(" |Initial VGA now, Please wait...                             |\n");
	// Write CH7026_RegMap.rem to registers of CH7026:
	CH7026_config();

	// Power up CH7026:
	CH7026_sccb_sendbyte(0x04, CH7026_sccb_receivebyte(0x04)&0xfe);//// set bit 0 to '0'
/*
	// Memory initializaion:
	if ( !CH7026_MemInit() )
	{
		printk("CH7026 Memory init can not be completed.\n");
	}
	CH7026_sccb_sendbyte(0x09, CH7026_sccb_receivebyte(0x09)|0x09);
	// Start running:
	CH7026_sccb_sendbyte(0x06, CH7026_sccb_receivebyte(0x06) & 0xFE); // set bit 0 to ¡®0¡¯.
*/	
	//printk("{ 0x%02x, 0x%02x },\n", 0x06, CH7026_sccb_receivebyte(0x06));
	return 0;
}

