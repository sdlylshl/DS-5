/****************************************************************
 NAME: u2440mon.c
 4.0
    增加启动图片
    增加NOR启动支持
    支持新WINCE的内核烧写

 ****************************************************************/
#define GLOBAL_CLK      1

#include <stdlib.h>
#include <string.h>
#include "def.h"
#include "2440addr.h"
#include "2440lib.h"
#include "2440slib.h"
#include "mmu.h"
#include "nand_lowlevel.h"

#include "usbmain.h"
#include "usbout.h"
#include "usblib.h"
#include "2440usb.h"

//#include "ok2440_usb.a"
#include "LCD_Driver.h"
#include "stdio.h"

#include "Keyboard.h"

//#include "norflash.h"
#include "bootpara.h"


void Isr_Init(void);
void HaltUndef(void);
void HaltSwi(void);
void HaltPabort(void);
void HaltDabort(void);
void USBDownload(void);
void Menu(void);

//ADD
extern void DiagNose(void);
//extern void NandLoadRun_App();
extern void lcdMenu(void);


void Clk0_Enable(int clock_sel);
void Clk1_Enable(int clock_sel);
void Clk0_Disable(void);
void Clk1_Disable(void);
//*************************************
//#include "ver.h"
extern void Versions(void);

//***********TimerWatchdog*************
//#include "Timer.h"
extern void Timer_InitEx(void);
extern void Timer_StartEx(void);
extern U32 Timer_StopEx(void);

//*************************************
//#include "LcdMenu.h"
extern void lcdMenu(void);

//#include "EEPROM.h"
extern void IIC_init(void);
extern void IIC_Read(U32 addr, U8 *data);
extern void IIC_Write(U32 addr, U8 data);
extern char CheckApp(void);
//#include "nand.h"
extern void NandLoadRun_App(void);
extern void AutoNandLoadRun(void);
extern void Keytest(void);
//#include "Timer0.h"
//LCD 屏幕刷新
extern void Timer0Init(void);
/*************************************************************/

//#define DOWNLOAD_ADDRESS _RAM_STARTADDRESS

extern U32 tempDownloadAddress;
extern volatile unsigned char *downPt;
extern volatile U32 totalDmaCount;
extern volatile U16 checkSum;
//下载方式设定
extern U8  com_usb;       //下载方式          0.USB 1.串口下载  ;其他 USB
extern U8 download_run;   // 是否下载到NAND中 0:run 1:DownLoad 

/*************************************************************/
//与下载有关的全局变量
/*************************************************************/
volatile U8  *downPt;
volatile U32 downloadAddress;
U32 tempDownloadAddress;
volatile U32 downloadFileSize;
volatile U32 totalDmaCount;
volatile U16 checkSum;

volatile int isUsbdSetConfiguration = 0;


U8 menuUsed   = 0;	//  使用菜单模式 ，当进入下载菜单后不会跳出来
U8 consoleNum = 0;	//  串口输出选择
U8 autorun_ds = 0;	//  自动运行使能

//下载方式设定
U8  com_usb = 0;       //下载方式          0.USB 1.串口下载  ;其他 USB
U8 download_run = 0;   // 是否下载到NAND中 0:run 1:DownLoad 

/*************************************************************/
//
/*************************************************************/
#define RUNMODE     0       //运行模式
#define DIAGMODE    1       //诊断模式
#define DOWNMODE    2       //下载模式
#define HIDEMODE    4       //菜单模式

/*************************************************************/
//启动模式设定
U8  BOOTMODE = 0;       //0.运行 1.诊断 2.下载 3.菜单 其他 运行



/*************************************************************/
//启动模式标志
U8  NOAPP = 0;  //应用程序是否下载 没有下载
U8  BOOTISCONNECT   = 0; //Boot端是否连接
U8  HIDEMENU = 0;  //2按键是否同时按下
//硬件检测标志
U8  DIAGFLAG = 0;       //硬件诊断 标志


U8 EEPROMISOK = 1;      //EEPROM 无错误 硬件检测
U8 KEYISOK = 0;         //键盘测试
/*************************************************************/

void Led_Test(void);
void comdownload(void);

void NandErase(void);
void NandWrite(void);
void NandLoadRun(void);
void NandLoadRun_wince(void);

//#include "nand_lowlevel.h"
extern void NandWrite(void);
extern void WrFileToNF_COM(void);
//#include "nand.h"
extern void NandLoadRun_App(void);

//#define printf  Uart_Printf

extern char Image$$RO$$Limit[];
extern char Image$$RO$$Base[];
extern char Image$$RW$$Limit[];
extern char Image$$RW$$Base[];
extern char Image$$ZI$$Limit[];
extern char Image$$ZI$$Base[];

/*

#define DM9000_BASE         0x20000300
#define DM9000_DATA_OFFSET  4

static U8 dm9000_ior(int reg)
{
    *(volatile U8 *)DM9000_BASE = reg;
    return *(volatile U8 *)(DM9000_BASE + DM9000_DATA_OFFSET);
}

static void rd_dm9000_id(void)
{
    U16 id;

    id = dm9000_ior(0x28) | (dm9000_ior(0x29) << 8);
    printf("read dm9000 vid = 0x%x\n", id);

    id = dm9000_ior(0x2a) | (dm9000_ior(0x2b) << 8);
    printf("read dm9000 pid = 0x%x\n", id);

    id = dm9000_ior(0x8) | (dm9000_ior(0x9) << 8);
    printf("read dm9000 reg(0x09,0x08) = 0x%x\n", id);

    printf("dm9000 isr = 0x%x\n", dm9000_ior(0xfe));
}
*/
void DownLoad_USB(void)
{
com_usb = 0;

}
void DownLoad_COM(void)
{
com_usb = 1;
}



static U32 cpu_freq;
static U32 UPLL;
static void cal_cpu_bus_clk(void)
{
    U32 val;
    U8 m, p, s;

    val = rMPLLCON;
    m = (val >> 12) & 0xff;
    p = (val >> 4) & 0x3f;
    s = val & 3;

    //(m+8)*FIN*2 不要超出32位数!
    FCLK = ((m + 8) * (FIN / 100) * 2) / ((p + 2) * (1 << s)) * 100;

    val = rCLKDIVN;
    m = (val >> 1) & 3;
    p = val & 1;
    val = rCAMDIVN;
    s = val >> 8;

    switch (m) {
    case 0:
        HCLK = FCLK;
        break;
    case 1:
        HCLK = FCLK >> 1;
        break;
    case 2:
        if (s & 2)
            HCLK = FCLK >> 3;
        else
            HCLK = FCLK >> 2;
        break;
    case 3:
        if (s & 1)
            HCLK = FCLK / 6;
        else
            HCLK = FCLK / 3;
        break;
    }

    if (p)
        PCLK = HCLK >> 1;
    else
        PCLK = HCLK;

    if (s & 0x10)
        cpu_freq = HCLK;
    else
        cpu_freq = FCLK;

    val = rUPLLCON;
    m = (val >> 12) & 0xff;
    p = (val >> 4) & 0x3f;
    s = val & 3;
    UPLL = ((m + 8) * FIN) / ((p + 2) * (1 << s));
    if (UPLL == 96 * MEGA)
        rCLKDIVN |= 8;  //UCLK=UPLL/2
    UCLK = (rCLKDIVN & 8) ? (UPLL >> 1) : UPLL;
}

static void set_cpu_bus_clk(void)
{
   
    int j;
    U8 key;
    U32 mpll_val;
   
    //  读取应用程序头文件,判断是应用程序是否已下载

// Mpll = ( 2 × (M+8) × Fin ) / ( (P+2) × 2^S ) 
// xPLLCON  位  描述  初始状态 MPLLCON/UPLLCON
// MDIV  [19:12]  主分频器控制  0x96 / 0x4d 
// PDIV  [9:4]  预分频器控制  0x03 / 0x03 
// SDIV  [1:0]  后分频器控制  0x0 / 0x0 

// 时钟分频控制（CLKDIVN）寄存器
// CLKDIVN  0x4C000014 
// DIVN_UPLL  [3]  HDIVN  [2:1]    PDIVN  [0] 
// 
// H  P    FCLK  HCLK      PCLK        分频比例 
// 
// 0  0    FCLK  FCLK      FCLK        1 : 1 : 1 （默认） 
// 0  1    FCLK  FCLK      FCLK / 2    1 : 1 : 2 
// 1  0    FCLK  FCLK / 2  FCLK / 2    1 : 2 : 2 
// 1  1    FCLK  FCLK / 2  FCLK / 4    1 : 2 : 4 
// 3  0    FCLK  FCLK / 3  FCLK / 3    1 : 3 : 3 
// 3  1    FCLK  FCLK / 3  FCLK / 6    1 : 3 : 6 
// 3  0    FCLK  FCLK / 6  FCLK / 6    1 : 6 : 6 
// 3  1    FCLK  FCLK / 6  FCLK / 12   1 : 6 : 12 
// 2  0    FCLK  FCLK / 4  FCLK / 4    1 : 4 : 4 
// 2  1    FCLK  FCLK / 4  FCLK / 8    1 : 4 : 8  (设定)
// 2  0    FCLK  FCLK / 8  FCLK / 8    1 : 8 : 8 
// 2  1    FCLK  FCLK / 8  FCLK / 16   1 : 8 : 16 
    //频率设定
    j = 2;
    //if(boot_params.display_mode.val==2)j=3;//TV mod

    switch (j) {
    case 0: //240
        key = 14;
        mpll_val = (112 << 12) | (4 << 4) | (1);
        break;
    case 1: //320
        key = 14;
        mpll_val = (72 << 12) | (1 << 4) | (1);
        break;  
    case 2: //400
        key = 14;
        mpll_val = (92 << 12) | (1 << 4) | (1);
        break;
    case 3: //420!!!
        key = 14;
        mpll_val = (97 << 12) | (1 << 4) | (1);
        break;
    default:
        key = 14;
        mpll_val = (92 << 12) | (1 << 4) | (1);
        break;
    }

#if (FIN == 12000000)
	//LED4_On();
	rGPBDAT = rGPBDAT & (~(1<<5));
    //init FCLK=400M, so change MPLL first
    ChangeMPllValue((mpll_val >> 12) & 0xff, (mpll_val >> 4) & 0x3f, mpll_val & 3);
    ChangeUPllValue(56,2,1);
    //1:4:8
    ChangeClockDivider(key, 12);
    cal_cpu_bus_clk();
    
    if (PCLK < (40 * MEGA)) {
        ChangeClockDivider(key, 11);
        cal_cpu_bus_clk();
    }
#else
//(FIN == 16000000)
    cal_cpu_bus_clk();
#endif

mpll_val = rMPLLCON;

}

void AppDownLoad(void)
{

    Lcd_Init();

    Versions();
    print(0, 2, "Use DNW DownLoad MyApp By USB", 0xFF);
    Uart_Printf( "Please Use DNW DownLoad MyApp By USB\n\n");

    download_run = 0; //下载模式
    NandWrite();
}
extern void CanTest2(void);

extern void Settime(void);
void (*restart)(void) = (void ( *)(void))0x0;
extern U32 T0_CNT,T1_CNT,T2_CNT,T3_CNT,T4_CNT;


void Main(void)
{


    //危险 若block2存在坏块怎么办？？？
    
    int i;
    U8 AppText[2048];
   // U8 dig;
    U32 t;
    
    //menuUsed=0;
    U32  OSEndiannessTest    = 0x12345678;               /* Variable to test CPU endianness     */

    unsigned char *ptemp;
    ptemp = (unsigned char *)&OSEndiannessTest;
    if (*ptemp == 0x12) {
   }

#if ADS10
    // __rt_lib_init(); //for ADS 1.0使编译器支持除法、结构拷贝、FP运算
#endif


	
	set_cpu_bus_clk();
	Beep_OFF();
	//Lcd_Init();
    //  Port_Init();
    Clk0_Disable();
    Clk1_Disable();
    //中断函数注册
    Isr_Init();
    
    pISR_SWI = (_ISR_STARTADDRESS + 0xf0); //for pSOS

    //Led_Test();
    //读取默认配置
    rDSC0 = 0x2aa;
    rDSC1 = 0x2aaaaaaa;

    //读取NANDFLASH
    i = search_params();

// while(1);
    //autorun_trig=0;
    //默认串口0
    //consoleNum = 0;
	//配置串口
    consoleNum = boot_params.serial_sel.val & 3; // Uart 1 select for debug.
    if (consoleNum > 1)consoleNum = 0;

    Uart_Init(0, 115200/*boot_params.serial_baud.val*/);
    Uart_Select(consoleNum);//Uart_Select(consoleNum) 默认用串口0，如果用户要用别的串口的话请修改这里
  
	// 配置USB
    rMISCCR = rMISCCR&~(1 << 3); // USBD is selected instead of USBH1
    rMISCCR = rMISCCR&~(1 << 13); // USB port 1 is enabled.
	//        isUsbdSetConfiguration = 0;
    UsbdMain();
    rGPBDAT &=~(1<<6); //LED2()
    IIC_init();
	//rGPBDAT =0x00001146;
   
   // NF_ReadPageBP(12, 0, AppText);
   	

    //if (0x45 == AppText[0] && 0xea == AppText[3])
    if(1==CheckApp())
        NOAPP = 0;
    else
        NOAPP = 1;
    //Boot未连接
    /*
    if (rGPGDAT & (1 << 8))
        BOOTISCONNECT = 0;
    else
        BOOTISCONNECT = 1;
    
    */

    KeyScan();
    
    MMU_Init();
    
    	
    if(KEY_DOWNAPP == KeyNum){
        Printf("USB DOWNLOAD MODE");        
    	Lcd_Update();
    	
		DownLoad_USB();
        download_run = 0;
        NandWrite();
        //清升级标志
        IIC_Write(0x18, 0);
        Settime();
    }
    //强制引导原始APP
    if (KEY_KernalAPP == KeyNum){
    	IIC_Write(0x18,0);
    	BOOTISCONNECT = 0;
    	}
    //单次强制引导原始APP	
    if (KEY_BACK == KeyNum){
    	
    	BOOTISCONNECT = 0;
    	}
    //菜单模式
    if (KEY_MENU == KeyNum)
        HIDEMENU = 1;
    else
        HIDEMENU = 0;
	
   
    if(boot_params.diagnose.val==0x5A){
    	DIAGFLAG=1;
    	}else{
    	DIAGFLAG=0;
    	}
    	

Uart_Printf("HIDEMENU %d\n",HIDEMENU);	    	
Uart_Printf("BOOTISCONNECT %d\n",BOOTISCONNECT);	    	
Uart_Printf("NOAPP %d\n",NOAPP);
Uart_Printf("DIAGFLAG %d\n",DIAGFLAG);	
    	/*延迟测试*/
    	Timer_InitEx();
    	Timer_StartEx();
//    	Init_Timer0();
    	//Lcd_Update();
    	t=Timer_StopEx();
    	Uart_Printf("%d\n",t);
    	//Uart_Printf("%d\n",T0_CNT);
    	//while(1);
    	
    	//    lcdMenu();
//    while(1);
    	
    //IIC_Read(1, &BOOTMODE);
    
/*
    if ((0x5A != DIAGMODE) && (BOOTMODE))
        DiagNose();

    if (NOAPP && HIDEMODE)
        lcdMenu();


    //      未下载程序且从下载模式进入 进入USB下载

    if ((!NOAPP) && (!rGPGDAT & (1 << 8))) {

        IIC_Read(0, &com_usb); //串口1 其他USB方式
        AppDownLoad();
    }
 
    
 */
 //      Keytest();
 //LcdMenu2();
 // Settime();
 		//lcdMenu();
    // 启动策略8级实现
    // 隐藏按键具有最高的优先级
    //LED1_On();
	rGPBDAT = rGPBDAT & (~(1<<8));
	
    if(HIDEMENU) {
        // 隐藏按键按下进入菜单模式
       // Printf("MENU MODE");
       
        LcdMenu2();
    } else {
        //Boot
        if(BOOTISCONNECT) {
            if(NOAPP) {
                //USB下载模式
                Printf("USB DOWNLOAD MODE");
 				DownLoad_USB();
                download_run = 0;
                NandWrite();
                //清升级标志
                IIC_Write(0x18, 0);
                Settime();
            } else {
                //诊断
                if(DIAGFLAG) {
                    //LCDMENU   
                    Printf("MENU MODE");
                    LcdMenu2();
                } else {
                    //DIAGFLAG
                    Printf("DiagNose MODE\n");
					DiagNose();

                }

            }

        } else {

            if(NOAPP) {
                //USB下载模式
                Printf("USB DOWNLOAD MODE");
                DownLoad_USB();
                download_run = 0;
                NandWrite();
                //清升级标志
                IIC_Write(0x18, 0);
                Settime();

            } else {
                //诊断
                if(DIAGFLAG) {
               	
                	//LED2_On
               		rGPBDAT = rGPBDAT & (~(1<<2));
                    //RUNAPP
                    NandLoadRun_App();

                } else {
                    //DIAGFLAG
                    Printf("DiagNose MODE");
					DiagNose();

                }
            }

        }

    }




//DiagNose();
//rGPBDAT |=1; //0x00001003; //键盘灯关 蜂鸣器灭 背光灯灭 指示灯全亮

//未检测到应用程序
//GPG8 硬件控制
//三按键同时按下
/*
if (NOAPP && (rGPGDAT & (1 << 8)) && (!(5 == rGPFDAT & 0xf)))
{
    //  MMU_Disable();

    NandLoadRun_App();

}*/

    //***********************************************************************************
    // 配置完成后，取消蜂鸣器响，背光亮

    /*  {
        if(boot_params.version.val==2)  //PCB6V2
            {
                rGPBDAT = 0x00001007; //键盘灯关 蜂鸣器灭 背光灯灭 指示灯全亮
            }

        if(boot_params.version.val==3) //PCB6V3
            {
                    rGPBDAT = 0x00001003; //键盘灯关 蜂鸣器灭 背光灯灭 指示灯全亮

            }

        }
    */
    
    Uart_Printf(" +------------------------------------------------------------+\n");
    Uart_Printf("boot_params.AppRun_addr.val is %x\n\n", boot_params.AppRun_addr.val);
    Uart_Printf(" |   PCB6V3 Bootloader VER-3.5           www.stronggest.cn    |\n");
    Uart_Printf(" +------------------------------------------------------------+\n");
    Uart_Printf(" |FCLK=%dMHz,HCLK=%dMHz,PCLK=%dMHz, CPU is running at %dMHz|\n",
                FCLK / MEGA, HCLK / MEGA, PCLK / MEGA, cpu_freq / MEGA);
    Uart_Printf(" |UPLL=%dMHz, UCLK=%dMHz                                      |\n", UPLL / MEGA, UCLK / MEGA);
    Uart_Printf(" |Serial port %d, Baud rate is %d.                         |\n", boot_params.serial_sel.val, boot_params.serial_baud.val);
    Uart_Printf(" |OS image stored in %s Flash.Endian is %x                 |\n", "NAND", ptemp);
    if (boot_params.xosc.val == 1)
        Uart_Printf(" |CAN OSC is 8M1\n");
    else
        Uart_Printf(" |CAN OSC is 16M0\n");
    Uart_Printf("      Build time is: %s  %s\n", __DATE__ , __TIME__  );
    Uart_Printf( "          Image$$RO$$Base  = 0x%x\n", Image$$RO$$Base );
    Uart_Printf( "          Image$$RO$$Limit = 0x%x\n", Image$$RO$$Limit );
    Uart_Printf( "          Image$$RW$$Base  = 0x%x\n", Image$$RW$$Base );
    Uart_Printf( "          Image$$RW$$Limit = 0x%x\n", Image$$RW$$Limit );
    Uart_Printf( "          Image$$ZI$$Base  = 0x%x\n", Image$$ZI$$Base );
    Uart_Printf( "          Image$$ZI$$Limit = 0x%x\n", Image$$ZI$$Limit );
    Uart_Printf(" +------------------------------------------------------------+\n");

   restart();
    //  lcdLightTest();
    //      DiagNose();
    //      while(1);
    //插入串口 首次启动 执行测试模式


    //首次启动执行 硬件测试  EEPROM 第一个字节



    //DiagNose();

    //      boot_params.TestMode.val=0x5A;
    //      save_params();
    //      Uart_Printf("The First Start will Run TestMode");

/*
//1.插入串口 2.comdown配置为0 3.Boot 引脚未短接 进入下载模式
if((rGPHDAT&(1<<3))&&(!(boot_params.comdown.val==0x5A)) &&(rGPGDAT&(1<<8)) )
    {
    //  boot_params.comdown.val=0x5A;save_params();  //如果不想插入串口就下载，把屏蔽取消

        WrFileToNF_COM();
        Uart_Printf("Enter Com Download Mode\n\n");

    }

//未插入串口，运行应用程序
if(!(rGPHDAT&(1<<3))||(rGPGDAT&(1<<8)))
    {
    Uart_Printf("Loader User App\n\n");
    NandLoadRun_App();
    }
//Boot引脚短接到地 进入USB下载模式
*/
//*****************************************************************************************

/*
    Uart_SendByte('\n');
#if 1
    Uart_Printf(" +------------------------------------------------------------+\n");
    Uart_Printf(" |   TE/OK2440 Bootloader VER-5.1           www.witech.com.cn |\n");
    Uart_Printf(" |------------------------------------------------------------|\n");
    Uart_Printf(" |------------------------------------------------------------|\n");
    Uart_Printf(" |CPU ID is 0x%08x                                        |\n", rGSTATUS1);
    if(i==-1) {
        Uart_Printf(" |  Fail to found boot params!                                |\n");

        save_params();

    }
    Uart_Printf(" |FCLK=%dMHz,HCLK=%dMHz,PCLK=%dMHz, CPU is running at %dMHz|\n",
                    FCLK/MEGA, HCLK/MEGA, PCLK/MEGA, cpu_freq/MEGA);
    Uart_Printf(" |UPLL=%dMHz, UCLK=%dMHz                                      |\n", UPLL/MEGA, UCLK/MEGA);
    Uart_Printf(" |Serial port %d, Baud rate is %d.                         |\n", boot_params.serial_sel.val, boot_params.serial_baud.val);
    Uart_Printf(" |OS image stored in %s Flash.                              |\n","NAND");
    Uart_Printf(" |Autoboot delay is %d seconds.                                |\n", boot_params.boot_delay.val);
    //display_init();//added by pht
    VGA_init();

    if(boot_params.boot_delay.val)
        init_autorun_timer(boot_params.boot_delay.val);
    Uart_Printf(" +------------------------------------------------------------+\n");

#else
    Uart_Printf("FEILING 2440 Bootloader VER-5.0    www.witech.com.cn\n");
    if(i==-1) {
        Uart_Printf("Fail to found boot params!\n");

        save_params();

    }
    Uart_Printf("Autoboot delay is %d seconds.\n", boot_params.boot_delay.val);
    VGA_init();
    if(boot_params.boot_delay.val)
        init_autorun_timer(boot_params.boot_delay.val);

#endif
*/

//Uart_Printf("<*************************************************************>\n");

// Boot For USB

//  rDSC0 = 0x2aa;
//  rDSC1 = 0x2aaaaaaa;

if (0)
{
    int i;
    volatile U16 *p = (volatile U16 *)0x08000000;

    p[3] = 0xbf;
    p[2] = 0;
    p[3] = 0;
    p[2] = 1;
    printf("dr2=0x%04x\n", p[2]);

    for (i = 0; i < 8; i++)
        printf("0x%08x\n", p[i]);

}


//  Delay(0);   //calibrate Delay() first
//  Uart_Printf("Check SST39VF160 Flash ID is 0x%08x\n", GetFlashID());


//  pISR_SWI=(_ISR_STARTADDRESS+0xf0);  //for pSOS

Led_Display(0x6);

//MMU_Init();





//Delay(30000);
//Lcd_Tft_LTV350QV_F05_Init();//by pht.


while (1)
{
    if (menuUsed == 1)Menu();
    USBDownload();
}

}



void Menu(void)
{

    U8 key;
    menuUsed = 1;
    while (1) {
#if 0
        Uart_Printf("\n +------------------------------------------------------------+\n");
        Uart_Printf(" |                    BOOTLOADER菜单                          |\n");
        Uart_Printf(" +------------------------------------------------------------+\n");
        Uart_Printf(" | [0] 下载并运行                                             |\n");
        Uart_Printf(" | [1] 下载到FLASH                                            |\n");

        Uart_Printf(" | [2] 用串口下载                                             |\n");
        Uart_Printf(" | [3] 启动应用程序                                           |\n");
        Uart_Printf(" | [4] 启动LINUX                                              |\n");

        Uart_Printf(" | [5] 启动WINCE                                              |\n");
        Uart_Printf(" | [6] 擦除FLASH                                              |\n");
        Uart_Printf(" | [7] 设置参数                                               |\n");
        Uart_Printf(" +------------------------------------------------------------+\n");
#else
        Uart_Printf("\n +------------------------------------------------------------+\n");
        Uart_Printf(" |                     Select Menu                            |\n");
        Uart_Printf(" +------------------------------------------------------------+\n");
        Uart_Printf(" | [0] Download & Run                                         |\n");
        Uart_Printf(" | [1] Download to flash                                      |\n");

        Uart_Printf(" | [2] Download From UART                                     |\n");
        Uart_Printf(" | [3] Boot My Application                                    |\n");
        Uart_Printf(" | [4] Boot Linux                                             |\n");

        Uart_Printf(" | [5] Boot Wince                                             |\n");
        Uart_Printf(" | [6] Erase Flash Partition                                  |\n");
        Uart_Printf(" | [7] Config parameters                                      |\n");
        Uart_Printf(" +------------------------------------------------------------+\n");

#endif
        key = Uart_Getch();

        switch (key) {
        case '0':
            Uart_Printf("\nDownload&Run is selected.\n\n");
            download_run = 1;
            return;
        case '1':
            download_run = 0;
            NandWrite();
            /*
                Uart_Printf("\nDownload Only is selected.\n");
                Uart_Printf("Enter a new temporary download address(0x3...):");
                tempDownloadAddress=Uart_GetIntNum();
                download_run=0;
                Uart_Printf("The temporary download address is 0x%x.\n\n",tempDownloadAddress);
                return;
            */
            break;
        case '2':
            comdownload();
            break;
        case '3':
            NandLoadRun_App();

        case '4':
            NandLoadRun();
            break;
        case '5':
            NandLoadRun_wince();
            break;
        case '6':
            NandErase();
            break;
        case '7':
            set_params();
            break;
            /*
                    case '8'
                        MemoryTest();
                        break;
            */
        default:
            break;
        }
    }

}



extern void __irq IsrUsbd(void);
extern void __irq IsrDma2(void);
void Isr_Init(void)
{
    pISR_UNDEF = (unsigned)HaltUndef;
    pISR_SWI  = (unsigned)HaltSwi;
    pISR_PABORT = (unsigned)HaltPabort;
    pISR_DABORT = (unsigned)HaltDabort;
    rINTMOD = 0x0;    // All=IRQ mode
    rINTMSK = BIT_ALLMSK; // All interrupt is masked.

    //pISR_URXD0=(unsigned)Uart0_RxInt;
    //rINTMSK=~(BIT_URXD0);   //enable UART0 RX Default value=0xffffffff

#if 1
    pISR_USBD = (unsigned)IsrUsbd;
    pISR_DMA2 = (unsigned)IsrDma2;
#else
    pISR_IRQ = (unsigned)IsrUsbd;
    //Why doesn't it receive the big file if use this. (???)
    //It always stops when 327680 bytes are received.
#endif
    ClearPending(BIT_DMA2);
    ClearPending(BIT_USBD);
    //rINTMSK&=~(BIT_USBD);

    //pISR_FIQ,pISR_IRQ must be initialized
}


void HaltUndef(void)
{
    Uart_Printf("Undefined instruction exception!!!\n");
    while (1);
}

void HaltSwi(void)
{
    Uart_Printf("SWI exception!!!\n");
    while (1);
}

void HaltPabort(void)
{
    Uart_Printf("Pabort exception!!!\n");
    while (1);
}

void HaltDabort(void)
{
    Uart_Printf("Dabort exception!!!\n");
    while (1);
}

void Clk0_Enable(int clock_sel)
{
    // 0:MPLLin, 1:UPLL, 2:FCLK, 3:HCLK, 4:PCLK, 5:DCLK0
    rMISCCR = rMISCCR&~(7 << 4) | (clock_sel << 4);
    rGPHCON = rGPHCON&~(3 << 18) | (2 << 18);
}
void Clk1_Enable(int clock_sel)
{
    // 0:MPLLout, 1:UPLL, 2:RTC, 3:HCLK, 4:PCLK, 5:DCLK1
    rMISCCR = rMISCCR&~(7 << 8) | (clock_sel << 8);
    rGPHCON = rGPHCON&~(3 << 20) | (2 << 20);
}
void Clk0_Disable(void)
{
    rGPHCON = rGPHCON&~(3 << 18); // GPH9 Input
}
void Clk1_Disable(void)
{
    rGPHCON = rGPHCON&~(3 << 20); // GPH10 Input
}

