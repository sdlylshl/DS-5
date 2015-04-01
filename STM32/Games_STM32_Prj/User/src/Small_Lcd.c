/*********************************************************************************
说明:主板小液晶操作
单片机:stm321f103vct6
液晶型号:HDW_12232E REV1.0
用串口来驱动
驱动:spi2,PB12 PB13 PB15 ,PB14作为普通i/o口控制语音芯片 
PB12在初始化作为普通i/o口来控制
20110517knimble 
1、用硬件spi测试，是否适合驱动
***********************************************************************************/

#include "Global.h"
#include "Small_lcd.h"
#include "Delay.h"
#include "target.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"

void Init_Spi2(void);
void SPI2_Senddata(uint8 TxData);
void LCD_Init(void);
void LCD_SdString(const uint8 *temp);
void LCD_SdIntData(uint16 uiData, uint8 ucLen, uint8 ucRow, uint8 ucCol);
void LCD_SdStringPos(const uint8 *Str,uint8 X,uint8  Y);
void LCD_Clear(void);
void  LCD_BlkWhite(void);
void  LCD_BlkWhite_Off(void);
void LCD_Shut(void);
void LCD_Disp_on(void);
void LCD_Send(uint8 senddata);
void LCD_SdCmd(uint8 scmd);
void LCD_SdData(uint8 DData);
void LCD_DispSetCursor(uint8 LineNum, uint8 ColumnNum);
void LCD_SetCurs_BlkWh(uint8 ucLine,uint8 ucColumn);


const uint8 CHNum[][2] = {"０","１", "２", "３", "４", "５", "６", "７", "８", "９"};

//对SPI2管脚的初始化
//在72M晶振下设置
void Init_Spi2(void)
{
	RCC->APB2ENR|=1<<3;   	//打开B口时钟
	RCC->APB1ENR|=1<<14;		//打开SPI2时钟

	GPIOB->CRH&=0X0000FFFF;
	GPIOB->CRH|=0xB3B30000;   //PB12 NSS,PB14 Miso作为普通I/O口，pb13sck,pb15mosi复用推挽输出 
	GPIOB->ODR&=0XEFFF;       //
	//
	SPI2->CR1|=1<<9; //软件nss管理
	SPI2->CR1|=1<<8;  

	SPI2->CR1|=1<<2; //SPI主机
//	SPI1->CR1|=0<<11;//8bit数据格式 
	SPI2->CR1|=1<<1; //空闲模式下SCK为1 CPOL=1
	SPI2->CR1|=1<<0; //数据采样从第二个时间边沿开始,CPHA=1	
	SPI2->CR1|=1<<5; //Fsck=Fcpu/16
	SPI2->CR1|=1<<3;  
	
	SPI2->CR1|=1<<6; //SPI设备使能	
}
//spi2数据的输出
void SPI2_Senddata(uint8         TxData)
{
	
	GPIOB->ODR|=GPIO_Pin_12;
	GPIOB->ODR&=~GPIO_Pin_13;
	GPIOB->ODR&=~GPIO_Pin_15;
	SPI2->DR=TxData;	 	  //发送一个byte   
	while((SPI2->SR&0x0002)==0)//等待发送区空				  
	{
		WdtFeed();                                   //喂狗
	}
	Delay_Nus(12);
	GPIOB->ODR&=~GPIO_Pin_12;
}
/**********************************************************
函数名：LCD_ScanKeyDown
功能：按键扫描函数
			只扫描菜单设置相关按键
输出：当键位有发生按下事件时对应键位为1，否则为0
******************************************/

/*uint8 LCD_ScanKeyDown(void)
{
	// 等待按键
	static uint8 ucSavedKeyState=0;	// 保存上次扫描时的按键状态
	uint8 ucKey=0;
	uint8 ucKey1=0;
	
	ucKey = (GET_KEY_MEUN_PIN);		// 取得按键输入引脚状态
	if(( ucKey & MSK_KEY_MENU))
	{
		Delay_Nms(1);
		ucKey = ucKey & GET_KEY_MEUN_PIN & MSK_KEY_MENU;
		ucKey1 = (ucKey ^ ucSavedKeyState) & ucKey;
		ucSavedKeyState = ucKey;
		
		return (ucKey1 & MSK_KEY_MENU);
	}
	ucSavedKeyState = 0;
	return 0;
} */

void LCD_Init(void)
{
	Init_Spi2();				// 减少发布的初始化接口函数, Add by XSky 2011-07-26 11:34
	
	Delay_Nms(10);
	
	LCD_LED_On();				// 开背光灯
	
	LCD_SdCmd(0x30);
	LCD_SdCmd(0x06);//点设定:画面右移
	LCD_SdCmd(0x0C);//lcd开，光标关
	LCD_SdCmd(0x01);//清屏
	LCD_SdCmd(0x02);//地址归位
	
	//LCD_SdCmd(0x07);
   	LCD_SdCmd(0x34);        // 8bit I/F, basic command
	// 切换到基本指令集
   	LCD_SdCmd(0x30);        // 8bit I/F, basic command, graphic off
   	
	Delay_Nms(5);			// 如果不加延时则后续几个字节可能不能显示, 测试1ms时也可能不正常, Add by Xsky 2011-07-26 14:10

	#ifdef DEBUG
	LCD_LED_Shut();
	#endif
}

////发送一串字符数据////// 
///set one string///
void LCD_SdString(const uint8 *temp)
{
   	// LCD_SdCmd(0x34);        // 8bit I/F, basic command
   	// LCD_SdCmd(0x30);        // 8bit I/F, basic command, graphic off
	while((*temp)!='\0' )
	{
	//	WDR();			// Add By Xsky 2008-07-25 17:00
		LCD_SdData(*temp);
		temp++;
	}
}

#if (TESTOUTLCD == TRUE) || (TESTOUTLCD_EE==TRUE)
void LCD_SdStringT(uint8 *temp)
{ 
   	// LCD_SdCmd(0x34);        // 8bit I/F, basic command
   	// LCD_SdCmd(0x30);        // 8bit I/F, basic command, graphic off
	while((*temp)!='\0' )
	{
	//	WDR();			// Add By Xsky 2008-07-25 17:00
		LCD_SdData(*temp);
		temp++;
	}
}
#endif

// ucLen: 低七位表示长度，
// 最高位为1时表示输出全角字符,0时输出半角字符
//UCROW 表示开始的行，UCCOL表示开始的列
void LCD_SdIntData(uint16 uiData, uint8 ucLen, uint8 ucRow, uint8 ucCol)
{
	uint16 iTmp;
	uint8 i;
	uint8 ucBuf[5];
	uint8 uBits;
	
	iTmp=uiData;
	uBits = ucLen & 0x7F;
	for(i=0; i<5; i++)
	{
		if (i>uBits)
		{
			break;
		}
		ucBuf[i] = (iTmp % 10) ;
		iTmp = iTmp / 10;
	}
	//CLI();
	if(ucRow < LCD_MAX_Rows){		// 
		LCD_DispSetCursor(ucRow, ucCol);
	}
	for (i=uBits; i; i--)
	{
		//#if (TESTOUTLCD != TRUE)  && (TESTOUTLCD_EE != TRUE)
		if( CHKBIT(ucLen, 7) )
		{
			LCD_SdData(CHNum[ucBuf[i-1]][0]);		// 输出全角字符, 两个字节
			LCD_SdData(CHNum[ucBuf[i-1]][1]);
			//LCD_SdData(' ');
		}
		else
		{
		//#else
			LCD_SdData(ucBuf[i-1]+ 48);			// 输出半角字符单字节
		}
		//#endif
	}
	//SEI();
}
/*
// 发送一串字符数据
// 从指定光标位置开始显示指定字符串 
void LCD_SdStringPosT(const uint8 *temp,uint8  X,uint8  Y)
{ 
	LCD_DispSetCursor(X,Y);
   	// LCD_SdCmd(0x34);        // 8bit I/F, basic command
   	// LCD_SdCmd(0x30);        // 8bit I/F, basic command, graphic off
	while((*temp)!='\0' )
	{
		LCD_SdData(*temp);
		temp++;
	}
}
*/

/*
// 从指定光标位置开始显示指定字符串 
void   LCD_Show(uint8  X,uint8  Y,const uint8 *Z)
{  
	LCD_DispSetCursor(X,Y);
	Delay_Nus(10);
	LCD_SdString(Z);
	//void SdString(uint8 *temp)
 }
*/
// 从指定光标位置开始显示指定字符串
void   LCD_SdStringPos(const uint8 *Str,uint8 row,uint8  col)
{
	if(row < LCD_MAX_Rows){	// Add By Xsky 2008-07-29 16:04
		LCD_DispSetCursor(row, col);
	}
	Delay_Nus(10);
	LCD_SdString(Str);
	//void SdString(uint8 *temp)
}

#ifdef DEBUG
t_err DbgLcdPrintf(uint8 row, uint8 col, const char *fmt, ...)	// Add by Xsky 2011-08-01 17:24
{
	va_list ap;		// 定义一个 va_list 指针来访问参数表
	char sBuf[32];
	int32 i;
	t_err err = ERR_VSprintf;
	
	va_start(ap, fmt);                 	 	// 初始化 ap，让它指向第一个变参
	i = vsprintf(sBuf, fmt, ap);
	va_end(ap);                            	// 善后工作，关闭 ap
	
	if(i >= 0)
	{
		memset(&sBuf[i], ' ', 16-i);
		LCD_SdStringPos((uint8 *)sBuf, row, col);
	}
	
	return err;
}
#endif

//清屏函数
 void LCD_Clear(void)
 {
 	LCD_SdCmd(0x01);
	Delay_Nms(2);
 }

//打开反白显示
void  LCD_BlkWhite(void)
{
      LCD_SdCmd(0x0d);
	  Delay_Nms(1);
}

// 关闭反白显示
void  LCD_BlkWhite_Off(void)
{
      LCD_SdCmd(0x0C);
	  Delay_Nms(1);
}

void LCD_Shut(void)
{
	LCD_SdCmd(0x01);
	Delay_Nms(1);
	LCD_SdCmd(0x08);
	Delay_Nms(1);
//	SETBIT(PORT_LCD_CTRL, PIN_LCD_LIGHT);		// 关背光灯
	LCD_LED_Shut();								// 关背光灯, Add by Xsky 2011-07-27 17:54
	
	LCD_SdCmd(0x34); 
	LCD_SdCmd(0x0c);  
}

//开液晶显示
void LCD_Disp_on(void)
{
//	CLRBIT(PORT_LCD_CTRL, PIN_LCD_LIGHT);		// 开背光灯
	LCD_LED_On();								// 开背光灯, Add by Xsky 2011-07-27 17:55
	LCD_SdData(0x0d);
}

// Parallel
void LCD_Send(uint8 senddata)
{
	//原来篮球机液晶并口发送
/*	PORT_LCD_DATA = senddata;				// 向数据口输出数据
	SETBIT(PORT_LCD_CTRL, PIN_LCD_EN);		// 允许LCD
	CLRBIT(PORT_LCD_CTRL, PIN_LCD_EN);		// 禁用LCD_DATA*/
	SPI2_Senddata(senddata);
}

void LCD_SdCmd(uint8 scmd)   //send command
{	
	uint8 T_data=0;
	/*
	LCD_WaitIdle();							// Add By Xsky 2008-5-24 18:25
	SETBIT(PORT_LCD_CTRL, PIN_LCD_RW);		// 调试
	PORT_LCD_DATA = scmd;				// 向数据口输出数据
	SETBIT(PORT_LCD_CTRL, PIN_LCD_EN );		// 允许LCD
	PORT_LCD_CTRL &= ~(BIT(PIN_LCD_RW) | BIT(PIN_LCD_RS));
	CLRBIT(PORT_LCD_CTRL, PIN_LCD_EN);		// 禁用LCD_DATA
	PORT_LCD_CTRL |= (BIT(PIN_LCD_RW) | BIT(PIN_LCD_RS));*/
	SPI2_Senddata(0xf8);
	T_data=scmd&0xf0;
	SPI2_Senddata(T_data);
	T_data=(scmd<<4)&0xf0;
	SPI2_Senddata(T_data);
}

void LCD_SdData(uint8 DData) 
{
	uint8 T_data=0;
	SPI2_Senddata(0xfA);
	T_data=DData&0xf0;
	SPI2_Senddata(T_data);
	T_data=(DData<<4)&0xf0;
	SPI2_Senddata(T_data);
}

//  状态检查函数,判断是否处于忙状态
/*void  LCD_WaitIdle(void)
{
 	unsigned char dat;
 	//CLRBIT(PORT_LCD_CTRL, PIN_LCD_RS);  //RS=0
 	//SETBIT(PORT_LCD_CTRL, );  //RW=1
 	uint8	iCount=0;
 	
 	do
 	{
 		//CLI();
	 	CLRBIT(PORT_LCD_CTRL, PIN_LCD_RS);  	//RS=0, 
	 	SETBIT(PORT_LCD_CTRL, PIN_LCD_RW);  //RW=1, Read
 		DDR_LCD_DATA = 0x00;				// portA as input
 	   	SETBIT(PORT_LCD_CTRL, PIN_LCD_EN);
 	   	Delay_Nus(1);
		//dat = PORT_LCD_DATA;
		dat = PIN__LCD_DATA;
		CLRBIT(PORT_LCD_CTRL, PIN_LCD_EN);
		DDR_LCD_DATA = 0xFF;		// LCD Data portA as output
		//SEI();
		iCount++;
 		WDR();			// Add By Xsky 2008-07-25 17:00
    	} while ( (dat&0x80) && (iCount<100));
    	
	Delay_Nus(1);
}

void  LCD_WaitIdle(void)
{
 	unsigned char dat;
 	//CLRBIT(PORT_LCD_CTRL, PIN_LCD_RS);  //RS=0
 	//SETBIT(PORT_LCD_CTRL, );  //RW=1
 	uint8	iCount=0;
 	
 	do
 	{
 	
	SETBIT(PORT_LCD_CTRL, PIN_LCD_RS);  	//调试
	CLRBIT(PORT_LCD_CTRL, PIN_LCD_RW);  //调试
 		//CLI();
	//CLRBIT(PORT_LCD_CTRL, PIN_LCD_RS);  	//RS=0, 
		SETBIT(PORT_LCD_CTRL, PIN_LCD_RW);  //RW=1, Read 经测试必须先设置RW再设置EN
 		DDR_LCD_DATA = 0x00;				// portA as input
 	//SETBIT(PORT_LCD_CTRL, PIN_LCD_EN);
	CLRBIT(PORT_LCD_CTRL, PIN_LCD_RS);  	//调试经测试必须先设置RS再设置EN
 		PORT_LCD_CTRL |= (BIT(PIN_LCD_EN) | BIT(PIN_LCD_RW));
 	//Delay_Nus(1);
		dat = PORT_LCD_DATA;
		CLRBIT(PORT_LCD_CTRL, PIN_LCD_EN);
		DDR_LCD_DATA = 0xFF;		// LCD Data portA as output
		//SEI();
		iCount++;
 		WDR();			// Add By Xsky 2008-07-25 17:00
    	} while ( (dat&0x80) && (iCount<100));
    	
	Delay_Nus(1);
}

*/

void LCD_DispSetCursor(uint8 LineNum, uint8 ColumnNum)
{

    uint8 i=0x00;

	if( (LineNum >= LCD_MAX_Rows) || (ColumnNum >= LCD_MAX_Cols) )	// Add by Xsky 2011-08-01 17:25
	{
		// 如果无效则接着上一个位置写
		return;
	}
    
    switch(LineNum&0x0f)   //确定行号
    {
	case 0x00:
		i=0x80;
		break;
	case 0x01: 
		i=0x90; 
		break;
	case 0x02: 
		i=0x88;
		break;
	case 0x03:
		i=0x98;
		break;
	default : 
		break;
    }
    i = (ColumnNum&0x0f)|i; //确定列号
   LCD_SdCmd(i);
}

//enf of LCD


void LCD_SetCurs_BlkWh(uint8 ucLine,uint8 ucColumn)
{
	LCD_DispSetCursor(ucLine,ucColumn);
	LCD_BlkWhite();
}

/*uint16 ChgVal(uint16 ucMenuID, uint16 uiVal,uint8 BitSet,int8 Det)//对某一位进行设置
{
	uint16 iTmp=1;
	uint8 Num[6];
	uint8  i;
	uint16 result=0;
	uint16 uiInitVal ;
	uiInitVal = uiVal;
	for(i=1;i<=5;i++)
	{
		Num[i]=uiInitVal%10;
		uiInitVal=uiInitVal/10;
	}
	if(Det==1)
	{
		Num[BitSet]++;
		if(Num[BitSet]==10)
		{
			Num[BitSet]=0;
		}
	}
	if(Det==(-1))
	{
		if (Num[BitSet]==0)
		{
			Num[BitSet]=9;
		}
		else
		{
		//if((Num[BitSet]+1)==0)
			Num[BitSet]--;
		}
	}
	for(i=1;i<=5;i++)
	{
		
		result=result+ Num[i]*iTmp;
		iTmp=iTmp*10;
	}
	if ( NOTIN(result, KeyTab[ucMenuID].Store.ucMinVal, KeyTab[ucMenuID].Store.uiMaxVal) )
	{
		return uiVal;
	}
	return result;
	

}



void LCD_HELP_SHOW(int8  ucMenuID,uint8 Bit)
{
	uint8 Lenth;
	
	 if(ucMenuID==MENU_TO_USE_LCD)
       {
		Lenth=sizeof(USE_LCD);
		LCD_SdStringPos(USE_LCD[Bit], 0,0);
		if((((Bit+1)*15)<Lenth))
		{
			LCD_SdStringPos(USE_LCD[Bit+1], 1,0);
		}
       }
	else if(ucMenuID==MENU_HOW_CONNECT)
	{
		Lenth=sizeof(CONNECT);
		LCD_SdStringPos(CONNECT[Bit], 0,0);
		if((((Bit+1)*15)<Lenth))
		{
			LCD_SdStringPos(CONNECT[Bit+1], 1,0);
		}
	}
	else if(ucMenuID==MENU_ABOUT)
	{
		Lenth=sizeof(ABOUT);
		LCD_SdStringPos(ABOUT[Bit],0,0);
		if((((Bit+1)*15)<Lenth))
		{
			LCD_SdStringPos(ABOUT[Bit+1],1,0 );
		}	
	}
}



uint16 LCD_HELP_Slg(int8 ucMenuID,uint16 Bit,uint8 Det)
{
	uint8 Lenth=0;

       
 	if(ucMenuID==MENU_TO_USE_LCD)
       {
       	Lenth=sizeof(USE_LCD);  //取要显示的行数
       }
	else if(ucMenuID==MENU_HOW_CONNECT)
	{
		Lenth=sizeof(CONNECT);  //取要显示的行数
	}
	else if(ucMenuID==MENU_ABOUT)
	{
		Lenth=sizeof(ABOUT);  //取要显示的行数	
	}
	Lenth /= 15;
	if(Det==0)
	{
		if(Bit==0)
		{
			Bit=0;
		}
		else
		{
			Bit=Bit-2;
		}
	}
	else if(Det==1)
	{
		if(((Bit==(Lenth-1)) && (Lenth%2==1))||((Bit==(Lenth-2)) && (Lenth%2==0)))
		{
			Bit=0;
		}
		else
		{
			Bit=Bit+2;
		}
	}
	return Bit;
}
*/
