
//-----------------------------------------   
// Shenzhen TOPWAY Technology Co. Ltd.   
// LCD Module:        LM3123   
// Display size:      160x32   
// Driver/Controller: AX6120/AX6121   
// Date:              2005-02-26   
//-----------------------------------------   
#include "config.h"
#include "util.h"
#include <stdio.h>
#include <string.h>
#include "stm32f10x.h"

#include "lcd.h" 
#include "matrix_key.h"

extern u8 key_val[];
extern int old_arm_cmd;
extern int need_upload;
extern sys_para sys;

u8 key_passwd[5] = {0xff, 0xff, 0xff, 0xff, 0xff};


#if 1

/* char: e   code:0x0065 */
unsigned char acFontHZ_0065[32] = { 
  ________, ________,
  ________, ________, 
  ________, ________, 
  ________, ________, 
  ________, ________,
  ________, ________, 
  ________, __XXX___, 
  ________, _X___X__, 
  ________, _XXXXX__, 
  ________, _X______, 
  ________, _X___X__, 
  ________, __XXX___, 
  ________, ________,
  ________, ________,
  ________, ________,
  ________, ________,
};



unsigned char acFontHZ_C1DA[32] = { 
  ________,________,
  ________,________,
  ___X____,________,
  ___X___X,XXX_____,
  __X_X__X,__X_____,
  _X___X_X,__X_____,
  X__X__XX,_X______,
  ____X__X,_X______,
  _XXXXX_X,__X_____,
  _____X_X,__X_____,
  ____X__X,__X_____,
  __X_X__X,XX______,
  ___X___X,________,
  ____X__X,________,
  ________,________,
  ________,________
};

 
/* char: 网   code:0xCDF8 */
unsigned char acFontHZ_CDF8[32] = { 
  ________,________,
  ________,________,
  __XXXXXX,XXXX____,
  __X_____,________,
  __X_____,________,
  __XX___X,___X____,
  __X_X_X_,X_X_____,
  __X__X__,_X______,
  __X__X__,_X______,
  __X_X_X_,X_X_____,
  __XX___X,___X____,
  __X_____,________,
  __X_____,________,
  __X_____,__XX____,
  ________,________,
  ________,________
};


/* char: 络   code:0xC2E7 */
unsigned char acFontHZ_C2E7[32] = { 
  ________,________,
  ________,________,
  ____X___,X_______,
  ____X___,XXXX____,
  ___X___X,___X____,
  __X__XX_,X_X_____,
  __XXX___,_X______,
  ____X___,X_X_____,
  ___X__XX,___X____,
  __XXXX_X,XXXX____,
  _______X,___X____,
  ____XX_X,___X____,
  __XX___X,XXXX____,
  _______X,___X____,
  ________,________,
  ________,________
};





/* char: 科   code:0xBFC6 */
unsigned char acFontHZ_BFC6[32] = { 
  ________,________,
  ________,________,
  ___X____,X_______,
  XXX__X__,X_______,
  __X___X_,X_______,
  __X_____,X_______,
  XXXXXX__,X_______,
  __X___X_,X_______,
  _XXX____,X_______,
  _XX_X___,XXX_____,
  X_X__XXX,X_______,
  __X_____,X_______,
  __X_____,X_______,
  __X_____,X_______,
  ________,________,
  ________,________
};

/* char: 技   code:0xBCBC */
unsigned char acFontHZ_BCBC[32] = { 
  ________,________,
  ________,________,
  __X____X,________,
  __X____X,________,
  __X_XXXX,XXX_____,
  XXXX___X,________,
  __X____X,________,
  __X_XXXX,XX______,
  __XX_X__,_X______,
  XXX__X__,_X______,
  __X___X_,X_______,
  __X____X,________,
  __X___X_,X_______,
  _XX_XX__,_XX_____,
  ________,________,
  ________,________
};


/* char: 有   code:0xD3D0 */
unsigned char acFontHZ_D3D0[32] = { 
  ________,________,
  ________,________,
  ____X___,________,
  ____X___,________,
  XXXXXXXX,XXX_____,
  ___X____,________,
  __XXXXXX,XX______,
  _XX_____,_X______,
  X_XXXXXX,XX______,
  __X_____,_X______,
  __XXXXXX,XX______,
  __X_____,_X______,
  __X_____,_X______,
  __X_____,XX______,
  ________,________,
  ________,________
};



/* char: 限   code:0xCFDE */
unsigned char acFontHZ_CFDE[32] = { 
  ________,________,
  ________,________,
  XXXX_XXX,XX______,
  X__X_X__,_X______,
  X__X_X__,_X______,
  X_X__XXX,XX______,
  X_X__X__,_X______,
  X__X_X__,_X______,
  X__X_XXX,XX______,
  X__X_X_X,__X_____,
  X__X_X_X,_X______,
  XXX__X__,X_______,
  X____XX_,_X______,
  X____X__,__X_____,
  ________,________,
  ________,________
};



/* char: 公   code:0xB9AB */
unsigned char acFontHZ_B9AB[32] = { 
  ________,________,
  ________,________,
  ___X___X,________,
  ___X___X,________,
  ___X___X,________,
  __X_____,X_______,
  __X__X__,X_______,
  _X___X__,_X______,
  X___X___,__X_____,
  ____X___,________,
  ___X___X,________,
  __X_____,X_______,
  _XXXXXXX,XX______,
  ________,_X______,
  ________,________,
  ________,________
};

/* char: 司   code:0xCBBE */
unsigned char acFontHZ_CBBE[32] = { 
  ________,________,
  ________,________,
  ________,________,
  _XXXXXXX,XX______,
  ________,_X______,
  XXXXXXXX,_X______,
  ________,_X______,
  _XXXXXX_,_X______,

  
  _X____X_,_X______,
  _X____X_,_X______,
  _X____X_,_X______,
  _XXXXXX_,_X______,
  _X______,_X______,
  ______XX,X_______,
  ________,________,
  ________,________
};

/* char: 密   code:0xC3DC */
unsigned char acFontHZ_C3DC[32] = { 
  ________,________,
  _____X__,________,
  XXXXXXXX,XXX_____,
  X___X___,__X_____,
  _____X_X,________,
  _X_X__X_,_X______,
  _X_X_X__,X_X_____,
  X___XXXX,X_X_____,
  __XX____,________,
  _____X__,________,
  _X___X__,_X______,
  _X___X__,_X______,
  _XXXXXXX,XX______,
  ________,________,
  ________,________,
  ________,________
};
/* char: 码   code:0xC2EB */
unsigned char acFontHZ_C2EB[32] = { 
  ________,________,
  ____XXXX,XX______,
  XXXX____,_X______,
  __X__X__,_X______,
  __X__X__,_X______,
  _X___X__,_X______,
  _XXX_XXX,XXX_____,
  XX_X____,__X_____,
  _X_X____,__X_____,
  _X_XXXXX,X_X_____,
  _XXX____,__X_____,
  _X_X____,__X_____,
  ________,XX______,
  ________,________,
  ________,________,
  ________,________
};

/* char: 布   code:0xB2BC */
unsigned char acFontHZ_B2BC[32] = { 
  ________,________,
  ____X___,________,
  ____X___,________,
  XXXXXXXX,XXX_____,
  ___X__X_,________,
  __X___X_,________,
  __XXXXXX,XX______,
  _X_X__X_,_X______,
  _X_X__X_,_X______,
  X__X__X_,_X______,
  ___X__X_,_X______,
  ___X__X_,XX______,
  ______X_,________,
  ________,________,
  ________,________,
  ________,________
};

/* char: 防   code:0xB7C0 */
unsigned char acFontHZ_B7C0[32] = { 
  ________,________,
  ______X_,________,
  XXXX___X,________,
  X__XXXXX,XXX_____,
  X__X__X_,________,
  X_X___X_,________,
  X_X___XX,XXX_____,
  X__X__X_,__X_____,
  X__X__X_,__X_____,
  X__X__X_,__X_____,
  XXX__X__,__X_____,
  X____X__,__X_____,
  X___X___,XX______,
  ________,________,
  ________,________,
  ________,________
};

/* char: 成   code:0xB3C9 */
unsigned char acFontHZ_B3C9[32] = { 
  ________,________,
  ______X_,X_______,
  ______X_,_X______,
  _XXXXXXX,XXX_____,
  _X____X_,________,
  _X____X_,________,
  _XXXX_X_,_X______,
  _X__X_X_,_X______,
  _X__X_X_,X_______,
  _X__X__X,__X_____,
  _X_XX_X_,X_X_____,
  _X___X__,_XX_____,
  X___X___,__X_____,
  ________,________,
  ________,________,
  ________,________
};

/* char: 功   code:0xB9A6 */
unsigned char acFontHZ_B9A6[32] = { 
  ________,________,
  ______X_,________,
  ______X_,________,
  XXXXX_X_,________,
  __X__XXX,XXX_____,
  __X___X_,__X_____,
  __X___X_,__X_____,
  __X___X_,__X_____,
  __X___X_,__X_____,
  __XXXX__,__X_____,
  XX___X__,__X_____,
  ____X___,__X_____,
  ___X____,XX______,
  ________,________,
  ________,________,
  ________,________
};

/* char: 撤   code:0xB3B7 */
unsigned char acFontHZ_B3B7[32] = { 
  ________,________,
  _X__X___,X_______,
  _XXXXXX_,X_______,
  _X_X_X_X,________,
  XXXXXX_X,XXX_____,
  _X____X_,_X______,
  _X_XXX_X,_X______,
  _XXX_X_X,_X______,
  XX_XXX_X,_X______,
  _X_X_X__,X_______,
  _X_XXX__,X_______,
  _X_X_X_X,_X______,
  XX_X_XX_,__X_____,
  ________,________,
  ________,________,
  ________,________
};




#endif

u8 data[640] = {0};

void change_one_chn_char(u8 index, u8* chn_char)
{
	int i=0, j;
	int k;

	printf("..............................index: %d\n", index);
	for(i=0; i<(ROW*COL/8); i++)
	{
		for(j=0; j<8; j++)
		{
			k = (index/10)*320+(index%10)*(COL)+(i/ROW)*MAX_COL+((i+16)%2)*8+7-j;
			
			if(chn_char[i] & (1<<j))
			{
				data[k] |= (1<<((i/2)%8));
				//printf("%02d %02d %02d %02d * ", k, (i/2), i, j);
			}
			else
			{
				data[k] &= (~(1<<((i/2)%8)));
				//printf("%02d %02d %02d %02d _ ", k, (i/2), i, j);
			}
		}
		printf("\n");
	}

		
}

u8 data0[160] = {

0x00, 0x80, 0x40, 0x40, 0x40, 0x80, 0x00, 0x00,     
0x40, 0x20, 0x01, 0x2c, 0x90, 0x02
} ;
u8 data1[160] = {

0x00, 0x07, 0x09, 0x09, 0x09, 0x05, 0x00, 0x00,     
0x00, 0x01, 0x09, 0x11, 0x2d, 0x03
};

u8 data2[160] = {
0x00,0x01,0x02, 0x02, 0x02, 0x01

} ;
u8 data3[160] = {
0x00,0xe0,0x90, 0x90, 0x90, 0x0a
};




void delay(int t)
{
	Delay_ms(1);
	return ;
	#if 0
	int i,j;
	for(j=0; j<t; j++)
		for(i=0; i<10; i++)
			;//nop();
	#endif
}

void set_data(u8 cmd_code)
{
	if(cmd_code & (1<<0))
		GPIO_SetBits(GPIOC, LM3123_DB0);
	else
		GPIO_ResetBits(GPIOC, LM3123_DB0);

	if(cmd_code & (1<<1))
		GPIO_SetBits(GPIOC, LM3123_DB1);
	else
		GPIO_ResetBits(GPIOC, LM3123_DB1);

	if(cmd_code & (1<<2))
		GPIO_SetBits(GPIOC, LM3123_DB2);
	else
		GPIO_ResetBits(GPIOC, LM3123_DB2);

	if(cmd_code & (1<<3))
		GPIO_SetBits(GPIOC, LM3123_DB3);
	else
		GPIO_ResetBits(GPIOC, LM3123_DB3);

	if(cmd_code & (1<<4))
		GPIO_SetBits(GPIOC, LM3123_DB4);
	else
		GPIO_ResetBits(GPIOC, LM3123_DB4);

	if(cmd_code & (1<<5))
		GPIO_SetBits(GPIOC, LM3123_DB5);
	else
		GPIO_ResetBits(GPIOC, LM3123_DB5);

	if(cmd_code & (1<<6))
		GPIO_SetBits(GPIOC, LM3123_DB6);
	else
		GPIO_ResetBits(GPIOC, LM3123_DB6);

	if(cmd_code & (1<<7))
		GPIO_SetBits(GPIOC, LM3123_DB7);
	else
		GPIO_ResetBits(GPIOC, LM3123_DB7);
}

void wrcmd1(u8 cmd_code)
{
	
	GPIO_ResetBits(GPIOC, LM3123_A0);
	GPIO_ResetBits(GPIOC, LM3123_RW);
	delay(5);
	GPIO_SetBits(GPIOC, LM3123_E1);
	set_data(cmd_code);
	delay(5);
	GPIO_ResetBits(GPIOC, LM3123_E1);
	GPIO_SetBits(GPIOC, LM3123_E1);
	
}

void wrdata1(u8 cmd_code)
{
	
	GPIO_SetBits(GPIOC, LM3123_A0);
	GPIO_ResetBits(GPIOC, LM3123_RW);
	delay(5);
	GPIO_SetBits(GPIOC, LM3123_E1);
	set_data(cmd_code);
	delay(5);
	GPIO_ResetBits(GPIOC, LM3123_E1);
	GPIO_SetBits(GPIOC, LM3123_E1);
	
}

void wrcmd2(u8 cmd_code)
{
	
	GPIO_ResetBits(GPIOC, LM3123_A0);
	GPIO_ResetBits(GPIOC, LM3123_RW);
	delay(5);
	GPIO_SetBits(GPIOC, LM3123_E2);
	set_data(cmd_code);
	delay(5);
	GPIO_ResetBits(GPIOC, LM3123_E2);
	GPIO_SetBits(GPIOC, LM3123_E2);
	
}

void wrdata2(u8 cmd_code)
{
	
	GPIO_SetBits(GPIOC, LM3123_A0);
	GPIO_ResetBits(GPIOC, LM3123_RW);
	delay(5);
	GPIO_SetBits(GPIOC, LM3123_E2);
	set_data(cmd_code);
	delay(5);
	GPIO_ResetBits(GPIOC, LM3123_E2);
	GPIO_SetBits(GPIOC, LM3123_E2);
	
}

void wrcmd3(u8 cmd_code)
{
	
	GPIO_ResetBits(GPIOC, LM3123_A0);
	GPIO_ResetBits(GPIOC, LM3123_RW);
	delay(5);
	GPIO_SetBits(GPIOC, LM3123_E3);
	set_data(cmd_code);
	delay(5);
	GPIO_ResetBits(GPIOC, LM3123_E3);
	GPIO_SetBits(GPIOC, LM3123_E3);
	
}

void wrdata3(u8 cmd_code)
{
	
	GPIO_SetBits(GPIOC, LM3123_A0);
	GPIO_ResetBits(GPIOC, LM3123_RW);
	delay(5);
	GPIO_SetBits(GPIOC, LM3123_E3);
	set_data(cmd_code);
	delay(5);
	GPIO_ResetBits(GPIOC, LM3123_E3);
	GPIO_SetBits(GPIOC, LM3123_E3);
	
}


void read_data()
{}
void init_lcd()
{
	wrcmd1(0xa4);
	wrcmd2(0xa4);
	wrcmd3(0xa4); //normal drive

	wrcmd1(0xa9);
	wrcmd2(0xa9);
	wrcmd3(0xa9);// 1/32 duty
	
	wrcmd1(0xa1); //ccw output adc=1
	wrcmd2(0xa1);
	wrcmd3(0xa1);

	wrcmd1(0xc0);//set start line 
	wrcmd2(0xc0);
	wrcmd3(0xc0);

	wrcmd1(0xaf);//display ram data not affected. display on
	wrcmd2(0xaf);
	wrcmd3(0xaf);

	//wrcmd1(0xEE);//read-modify-write=off
	//wrcmd2(0xEE);
	//wrcmd3(0xEE);
}

void clear_lcd()
{
	u8 i,j;

	for(i=0; i<4; i++)
	{
		wrcmd1(0xb8+i);
		wrcmd2(0xb8+i);
		wrcmd3(0xb8+i);
#if 0
		for(j=0;j<160;j++)   
        {   
        	if(j<60)
        	{
	        	wrcmd1(0x14+j);
				wrdata1(0x00);
        	}
			else if(j<140)
			{
				wrcmd2(0x00+j);
				wrdata2(0x00);
			}
			else
			{
				wrcmd3(0x13+j);
				wrdata3(0x00);
			}
	    }
#endif
#if 1
		wrcmd1(0x00);
		wrcmd2(0x00);
		wrcmd3(0x00);

		for(j=0;j<80;j++)   
        {   
	      wrdata1(0x00);
		  wrdata2(0x00);
		  wrdata3(0x00);
	    }  
#endif
	}
}



void Disp_Img(u8 *Img)
{
	u8 i,j,k;
	int n=0;
	for(k=0;k<4;k++)
	{
		wrcmd1(0xb8+k);
		for(i=0;i<60;i++)
		{
			wrcmd1(0x14+i);
			wrdata1(Img[n++]);
		}
		wrcmd2(0xb8+k);
		for(i=0;i<80;i++)
		{
			wrcmd2(i);
			wrdata2(Img[n++]);
		}
		wrcmd3(0xb8+k);
		for(i=0;i<20;i++)
		{
			wrcmd3(0x13+i);
			wrdata3(Img[n++]);
		}
	}
}


void Disp_word(u8 index, u8 *Img)
{
	u8 i,j,k;
	int n=0, start, sec_start;

	start = index*16;
	sec_start = index*16 + index/10*160;
	if(index < 10)
	{
		for(k=0; k<2; k++)
		{
			for(i=0; i<16; i++)
			{
				if(start < 60)
				{
					wrcmd1(0xb8+k);
					wrcmd1(0x14+index*16+i);
					wrdata1(Img[(k*160)+index*16+i]);
					printf("k: %d, 1: %d, 2: %d, 3: %d\n", k, 0xb8+k, 0x14+index*16+i, (k*160)+index*16+i);
				}
				else if(start < 140)
				{
					wrcmd2(0xb8+k);
					wrcmd2(start-60);
					wrdata2(Img[(k*160)+index*16+i]);
					printf("...k: %d, 1: %d, 2: %d, 3: %d\n", k, 0xb8+k, start-60, (k*160)+index*16+i);
				}
				else if(start < 160)
				{
					wrcmd3(0xb8+k);
					wrcmd3(0x13+start-140);
					wrdata3(Img[(k*160)+index*16+i]);
				}
				
				start++;
			}
			start = index*16;
		}
		
	}
	else
	{
		start = (index-10)*16;
		for(k=2; k<4; k++)
		{
			for(i=0; i<16; i++)
			{
				if(start < 60)
				{
					wrcmd1(0xb8+k);
					wrcmd1(0x14+(index-10)*16+i);
					wrdata1(Img[(k*160)+(index-10)*16+i]);
				}
				
				else if(start < 140)
				{
					wrcmd2(0xb8+k);
					wrcmd2(start-60);
					wrdata2(Img[(k*160)+(index-10)*16+i]);
				}
				else if(start < 160)
				{
					wrcmd3(0xb8+k);
					wrcmd3(0x13+start-140);
					wrdata3(Img[(k*160)+(index-10)*16+i]);
				}
				
				start++;
			}
			start = (index-10)*16;
		}
	}
		
}


void WriteScreen(u8 *DisplayData)    // DisplayData should be 160x32/8 = 640 bytes   
{   
    u8 TempData;   
    u8 i, j;   
    for(i=0;i<4;i++)   
    {   
        wrcmd1(0xb8|i);
		wrcmd2(0xb8|i);
		wrcmd3(0xb8|i);
		
        for(j=0;j<160;j++)   
        {   
	        TempData=(*(DisplayData+(i*160)+j));   
	        if (j<60)   
	        {   
	        	wrcmd1(0x14+j);
	            wrdata1(TempData);    // 0-59 column goto chip1   
	        }   
	        else if (j<140)   
	        {   
	        	wrcmd2(0x00+j);
	            wrdata2(TempData);    // 60-140 column goto chip2   
	        }   
	        else   
	        {   
	        	wrcmd3(0x13+j);
	            wrdata3(TempData);    // 141-160 column goto chip3   
	        }   
	    }   
    }   
           
} 


void show_icon()
{
	
}


void deal_485_cmd(int ret)
{
	if(ret == DISARM_CMD)
	{
		display_disarm();
		sys.arming = 0x0;
		//printf("deal_485_cmd recv DISARM_CMD\n");
		old_arm_cmd = DISARM_CMD;
		save_arming_to_eeprom(sys.arming);
	}
	else if(ret == ARM_CMD)
	{
		display_arm();
		sys.arming = 0xffffffff;
		old_arm_cmd = ARM_CMD;
		//printf("deal_485_cmd recv ARM_CMD\n");
		save_arming_to_eeprom(sys.arming);
	}
	else if(ret == POLL_CMD)
	{
		//if(need_upload)
		{
			send_arm_msg();
			//need_upload = 0;
		}
		//else
			//send_void_msg();
	}
}

int need_upload=0;
int old_arm_cmd=0xff;
static u8 key_passwd_index=0;


void key_passwd_backward()
{
	
	if(key_passwd_index == 0)
		key_passwd_index=4;
	else
		key_passwd_index--;
}

void key_passwd_forward()
{
	key_passwd_index++;
	if(key_passwd_index == 5)
		key_passwd_index=0;
}

u8 static delay_arm_flag=0;
static u32 delay_arm_time=0;
static u32 beep_time=0;

void delay_arm()
{
	static u8 flag=0;
	if(delay_arm_flag)
	{
		if(time_return()-delay_arm_time > 600)
		{
			old_arm_cmd = ARM_CMD;
			sys.arming = 0xffffffff;
			save_arming_to_eeprom(sys.arming);
			display_arm();
			need_upload = 1;
			delay_arm_flag = 0;
			beep_off();
		}
		else
		{
			if(time_return() - beep_time >= (10))
			{
				if(flag)
				{
					beep_on();
					flag=0;
				}
				else
				{
					beep_off();
					flag=1;
				}
				beep_time = time_return();
			}
		}
	}
	
		
}

void handle_key_event()
{
	u8 ret, i;
	static u8 last_key=0x20;
	

	for(i=KEY_A; i<KEY_WELL+1; i++)
	{
		if(key_val[i] == KEY_PRESSED)
		{
			if(i != last_key)
			{
				key_passwd[key_passwd_index] = i;
				
				key_passwd_forward();
				last_key = i;
				
				printf("key_passwd[0]: %d, key_passwd[1]: %d, key_passwd[2]: %d, key_passwd[3]: %d, key_passwd[4]: %d,\n", key_passwd[0], key_passwd[1], key_passwd[2], key_passwd[3], key_passwd[4]);
				
				break;
			}
		}
	}

	
	

	if((i == KEY_A)||(i == KEY_B))
	{
		printf("i :%d arm or disarm\n", i);
		key_passwd_backward();
		key_passwd_backward();
		if(key_passwd[key_passwd_index] == KEY_4)
		{
			printf("1 arm or disarm\n");
			key_passwd_backward();
			if(key_passwd[key_passwd_index] == KEY_3)
			{
				printf("2 arm or disarm\n");
				key_passwd_backward();
				if(key_passwd[key_passwd_index] == KEY_2)
				{
					printf("3 arm or disarm\n");
					key_passwd_backward();
					if(key_passwd[key_passwd_index] == KEY_1)
					{
						printf("4 arm or disarm\n");
						if(key_val[KEY_A] == KEY_PRESSED)
						{
							printf("ARM \n");
							if(old_arm_cmd != ARM_CMD)
							{	
								delay_arm_flag = 1;
								delay_arm_time=time_return();
								beep_time=time_return();
								
							}
							//send_arm_msg();
							//parse_rs485_cmd(&ret);
							//if(ret == 0)
							#if 0
							{
								change_one_chn_char(10, acFontHZ_B2BC);
								Disp_word(10, data);
								change_one_chn_char(11, acFontHZ_B7C0);
								Disp_word(11, data);
								change_one_chn_char(12, acFontHZ_B3C9);
								Disp_word(12, data);
								change_one_chn_char(13, acFontHZ_B9A6);
								Disp_word(13, data);
							}
							#endif
							
						}

						if(key_val[KEY_B] == KEY_PRESSED)
						{
							printf("DISARM \n");
							if(old_arm_cmd != DISARM_CMD)
							{
								old_arm_cmd = DISARM_CMD;
								need_upload = 1;
								sys.arming = 0x0;
								save_arming_to_eeprom(sys.arming);
							}
							//send_disarm_msg();
							//parse_rs485_cmd(&ret);
							//if(ret == 1)
							#if 0
							{
								change_one_chn_char(10, acFontHZ_B3B7);
								Disp_word(10, data);
								change_one_chn_char(11, acFontHZ_B7C0);
								Disp_word(11, data);
								change_one_chn_char(12, acFontHZ_B3C9);
								Disp_word(12, data);
								change_one_chn_char(13, acFontHZ_B9A6);
								Disp_word(13, data);
							}
							#endif
							display_disarm();
							}
					}
				}
			}
		}
	}
}
	


void reset_lcd()
{
	Delay_ms(50);

	GPIO_ResetBits(GPIOC, LM3123_RES);
	Delay_ms(5);
	GPIO_SetBits(GPIOC, LM3123_RES);
}

//
void lcd_test()
{
	int i;
	//reset_lcd();
	printf("lcd_test\n");
	clear_lcd();
	delay(10);
	
	init_lcd();
	delay(10);

	wrcmd1(0xb9);
	wrcmd1(0x13);
	wrdata1(0xf0);
	
	//while(0)
	{
		//WriteScreen(Logo1);
		//memcpy(data, data0, 160);
		//memcpy(data+160, data1, 160);
		//memcpy(data+320, data2, 160);
		//memcpy(data+480, data3, 160);
		change_one_chn_char(0, acFontHZ_0065);
		//for(i=0; i<20; i++)
		//	change_one_chn_char(i, acFontHZ_C1DA);
		change_one_chn_char(1, acFontHZ_C1DA);
		change_one_chn_char(2, acFontHZ_CDF8);
		change_one_chn_char(3, acFontHZ_C2E7);
		change_one_chn_char(4, acFontHZ_BFC6);
		change_one_chn_char(5, acFontHZ_BCBC);
		change_one_chn_char(6, acFontHZ_D3D0);
		change_one_chn_char(7, acFontHZ_CFDE);
		change_one_chn_char(8, acFontHZ_B9AB);
		change_one_chn_char(9, acFontHZ_CBBE);


		//change_one_chn_char(8, acFontHZ_0065);
		//change_one_chn_char(9, acFontHZ_C1DA);
		//change_one_chn_char(10, acFontHZ_BFC6);
		//change_one_chn_char(11, acFontHZ_BCBC);
		//change_one_chn_char(12, acFontHZ_D3D0);
		//change_one_chn_char(13, acFontHZ_CFDE);
		//change_one_chn_char(14, acFontHZ_B9AB);
		//change_one_chn_char(15, acFontHZ_CBBE);
		
		Disp_Img(data);
		//for(i=0; i<20; i++)
		//	Disp_word(i,data);
		delay(1000);
		printf("show start\n");
		//wrcmd1(0xb9);
		//wrcmd1(0x13);
		//wrdata1(0xf0);
	}
}

