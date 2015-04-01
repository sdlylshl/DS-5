/*******************************************************************************
  Copyright (C), 2009-2009, YingHong, Ltd.
  File name:   	LCD.C	
  Author:      	Xsky
  Version:     	1.0
  Date:		2009-04-25
  
  Description:   LCD驱动函数 RSMHV12232V10
  Others:         
  Function List: 
  	1. ：
  	2. ：
  	3. ：
  	4. ：
  	5. ：

  History:        // 修改记录
    1. Date:		2009-04-25
       Author:		Xsky
       Modification:	建立本模块
*******************************************************************************/
#define LCD_GLOBALS
#include "Config.H"

void LCD_Init(void)
{ 	
 	//端口初始化
	//LCD_DATA_DDR=0xFF;
	
	// 开背光灯
	//PORT_LCD_CTRL_RST&=~(BIT(PIN_LCD_RST));
	LCD_SET_RST();
	//PORT_LCD_CTRL &= ~(BIT(PIN_LCD_LIGHT)|BIT(PIN_LCD_EN)|BIT(PIN_LCD_RS));
	LCD_LightOn();
	LCD_CTRL_Opt( CLRBITs(LCD_CtrlReg, BIT(PNO_LCD_EN)|BIT(PNO_LCD_RS)) );
	Delay_Nms(3);		// 1
	//SETBIT(PORT_LCD_CTRL_RST, PIN_LCD_RST);		// 取消复位
	LCD_CLR_RST();
	
	Delay_Nms(5);			// 经测试此队若无延时则第一次启动送数据无显示
	
	LCD_SdCmd(0x30);		// 基本指令集
	LCD_SdCmd(0x06);		// 点设定:画面右移
	LCD_SdCmd(0x0C);		// lcd开，光标关
	//LCD_SdCmd(0x01);		//清屏
	LCD_Clear();				// 0x01, 清除显示屏幕，把DDRAM位址计数器调整为"00H"
	LCD_SdCmd(0x03);		// 把DDRAM位址计数器调整为"00H"，游标回原点，该功能不影响显示DDRAM
	LCD_SdCmd(0x02);		// 地址归位
	
	//Delay_Nms(25);			// 经测试此队若无延时则第一次启动送数据无显示
	// 切换到基本指令集
   	LCD_SdCmd(0x34);       	// 8bit I/F, basic command // 经测试若此命令缺少则第一次上电无显示
   	LCD_SdCmd(0x30);        	// 8bit I/F, basic command, graphic off
   	
	Delay_Nms(15);			// 经测试此队若无延时则第一次启动送数据无显示
   	LCD_SdCmd(0x34);       	// 8bit I/F, basic command // 经测试若此命令缺少则第一次上电无显示
   	LCD_SdCmd(0x30);        	// 8bit I/F, basic command, graphic off
	
}

////发送一串字符数据////// 
///set one string///
void LCD_SdString(const uint8 *temp)
{
   	// LCD_SdCmd(0x34);        // 8bit I/F, basic command
   	// LCD_SdCmd(0x30);        // 8bit I/F, basic command, graphic off
	while((*temp)!='\0' )
	{
		WDR();			// Add By Xsky 2008-07-25 17:00
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
		WDR();			// Add By Xsky 2008-07-25 17:00
		LCD_SdData(*temp);
		temp++;
	}
}
#endif

__flash uint8 CHNum[] = {"０１２３４５６７８９"};
// ucLen: 低七位表示长度，
// 最高位为1时表示输出全角字符,0时输出半角字符
void LCD_SdIntData(uint16 uiData, uint8 ucLen, uint8 ucRow, uint8 ucCol)
{
	uint16 iTmp;
	register uint8 i;
	uint8 ucBuf[5];
	register uint8 uBits;
	
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
		if( CHKBIT(ucLen, 7) )
		{
			LCD_SdData(CHNum[ucBuf[i-1] * 2]);		// 输出全角字符, 两个字节
			LCD_SdData(CHNum[ucBuf[i-1] * 2 + 1]);
			//LCD_SdData(' ');
		}
		else
		{
			LCD_SdData(ucBuf[i-1]+ 48);			// 输出半角字符单字节
		}
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
void   LCD_SdStringPos(const uint8 *Str,uint8  X,uint8  Y)
{
	if(X < LCD_MAX_Rows){	// Add By Xsky 2008-07-29 16:04
		LCD_DispSetCursor(X, Y);
	}
	//Delay_Nus(10);
	LCD_SdString(Str);
	//void SdString(uint8 *temp)
 }

//清屏函数
 void LCD_Clear(void)
 {
 	LCD_SdCmd(0x01);
	//Delay_Nms(2);
 }

//打开反白显示
void  LCD_BlkWhite(void)
{
      LCD_SdCmd(0x0d);
      //Delay_1ms();
}

// 关闭反白显示
void  LCD_BlkWhite_Off(void)
{
      LCD_SdCmd(0x0C);
      //Delay_1ms();
}

void LCD_Shut(void)
{
	LCD_SdCmd(0x01);
	//Delay_1ms();
	LCD_SdCmd(0x08);
	//Delay_1ms();
	LCD_LoghtOff();
	LCD_SdCmd(0x34); 
	LCD_SdCmd(0x0c);  
}

//开液晶显示
void LCD_Disp_on(void)
{
	LCD_LightOn();
	LCD_SdData(0x0d);
}

//enf of LCD

void LCD_SetCurs_BlkWh(uint8 ucLine,uint8 ucColumn)
{
	LCD_DispSetCursor(ucLine,ucColumn);
	LCD_BlkWhite();
}


__flash uint8 LINE_NUM_TBL[] = {0x80, 0x90, 0x88, 0x98, 0x00, 0x00, 0x00, 0x00};
void LCD_DispSetCursor(uint8 LineNum, uint8 ColumnNum)
{
    /*
    register uint8 i = 0x00;
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
    i = LINE_NUM_TBL[LineNum & 0x07];
    i = (ColumnNum&0x0f)|i; 			//确定列号
    */
   LCD_SdCmd( LINE_NUM_TBL[LineNum & 0x07] | (ColumnNum & 0x0f) );
}

/*
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
//  状态检查函数,判断是否处于忙状态
void  LCD_WaitIdle(void)
{
 	register unsigned char dat;
 	register uint8	iCount=0;
 	//CLRBIT(PORT_LCD_CTRL, PIN_LCD_RS);  //RS=0
 	//SETBIT(PORT_LCD_CTRL, );  //RW=1
 	register uint8 u8Reg;
 	register uint8 u8Reg2;
	
 	do
 	{
 		//CLI();
	 	/*
	 	LCD_CTRL_Opt( CLRBIT(LCD_CtrlReg, PNO_LCD_RS) ); //RS=0, 
	 	LCD_CTRL_Opt( SETBIT(LCD_CtrlReg, PNO_LCD_RnW) );//RW=1, Read
		*/
	 	u8Reg = LCD_CtrlReg;
	 	CLRBIT(u8Reg, PNO_LCD_RS);
	 	SETBITs(u8Reg, BIT(PNO_LCD_RnW));
	 	LCD_CtrlReg = u8Reg;
 	   	PORT_LCD_CTRL = u8Reg;					// 必须先输出RS/RW, 再输出EN
	 	
 	   	SETBIT(u8Reg, PNO_LCD_EN);
	 	LCD_CtrlReg = u8Reg;
	 	u8Reg2 = u8Reg;
	 	CLRBIT(u8Reg2, PNO_LCD_EN);
	 	LCD_CtrlReg = u8Reg2;
 	   	
		LCD_OS_ENTER_CRITICAL();
		LCD_DBUS_Out(0xFF);							// 使能上拉电阻
 		LCD_SET_BusIn();							// port as input
 		
 	   	//LCD_CTRL_Opt( SETBIT(LCD_CtrlReg, PNO_LCD_EN) );
 	   	PORT_LCD_CTRL = u8Reg;
 	   	
 	   	NOP();
 	   	//Delay_1us();
 	   	//Delay_Nus(1);
		dat = PIN_LCD_DATA;
		
		/*
		LCD_CTRL_Opt( CLRBIT(LCD_CtrlReg, PNO_LCD_EN) );
		*/
		//CLRBIT(u8Reg, PNO_LCD_EN);
 	   	PORT_LCD_CTRL = u8Reg2;
		//LCD_CtrlReg = u8Reg;
		
		LCD_OS_EXIT_CRITICAL();
		
		//SEI();
		iCount++;
 		WDR();										// Add By Xsky 2008-07-25 17:00
		Delay_1us();
		
		//break;										// 此Break已证实LCD Busy 为有效手段, 去掉后LCD不能正常工作// By Xsky 2009-05-08 14:20
    	} while ( (dat & 0x80) && (iCount<200));
    	//_DBG_( sprintf((char *)DbgBuf, "iCnt:%u; ", iCount) );	// By Xsky 2009-05-08 14:20
	//_DBG_( Uart1Send(DbgBuf, strlen(DbgBuf)) );			
	// iCnt:1; iCnt:3; iCnt:5; iCnt:6; iCnt:1; iCnt:4; iCnt:6; iCnt:5; iCnt:3; iCnt:6; iCnt:4; iCnt:5; iCnt:6; iCnt:1; iCnt:5; iCnt:3; iCnt:6; iCnt:5; iCnt:4; iCnt:5; iCnt:5; iCnt:4; iCnt:5; iCnt:5; 
	// 经测试LCD Busy 状态有效，可检测出液晶Busy
}

// Parallel
/*
void LCD_Send(uint8 senddata)
{
	register uint8 u8Reg;

	u8Reg = LCD_CtrlReg;
	SETBIT(u8Reg, PNO_LCD_EN);
	
	LCD_OS_ENTER_CRITICAL();
	LCD_SET_BusOut();
	
	PORT_LCD_DATA = senddata;				// 向数据口输出数据
	//SETBIT(PORT_LCD_CTRL, PIN_LCD_EN);		// 允许LCD
	//LCD_CTRL_Opt(SETBIT(LCD_CtrlReg, PNO_LCD_EN));
	PORT_LCD_CTRL = u8Reg;
	//Delay_Nus(5);
	//WDR();
	//CLRBIT(PORT_LCD_CTRL, PIN_LCD_EN);		// 禁用LCD_DATA
	//LCD_CTRL_Opt(CLRBIT(LCD_CtrlReg, PNO_LCD_EN));
	CLRBIT(u8Reg, PNO_LCD_EN);
	LCD_CtrlReg = u8Reg;
	PORT_LCD_CTRL = u8Reg;
	
	LCD_SET_BusIn();
	
	LCD_OS_EXIT_CRITICAL();
}
*/

void LCD_SdCmd(uint8 scmd)   //send command
{
	register uint8 u8Reg;
	register uint8 u8Reg2;
	
	LCD_WaitIdle();										// Add By Xsky 2008-5-24 18:25
	
	/*
	LCD_CTRL_Opt( CLRBITs(LCD_CtrlReg, BIT(PNO_LCD_RnW) | BIT(PNO_LCD_RS)) );
	*/
	u8Reg = LCD_CtrlReg;									// 定义寄存器以优化代码
	CLRBITs(u8Reg, BIT(PNO_LCD_RnW) | BIT(PNO_LCD_RS)); 	// LCD_RW = 0 写入状态// LCD_RS = 0 选择命令寄存器
	LCD_CtrlReg = u8Reg;
	PORT_LCD_CTRL = u8Reg;
	
	SETBIT(u8Reg, PNO_LCD_EN);
	//LCD_CtrlReg = u8Reg;
	u8Reg2 = u8Reg;
	CLRBIT(u8Reg2, PNO_LCD_EN);
	LCD_CtrlReg = u8Reg2;
	
	LCD_OS_ENTER_CRITICAL();
	LCD_SET_BusOut();
	
	PORT_LCD_DATA = scmd;								// 向数据口输出命令
	
	/*
	LCD_CTRL_Opt( SETBIT(LCD_CtrlReg, PNO_LCD_EN) );
	*/
	//LCD_CtrlReg = u8Reg;
	PORT_LCD_CTRL = u8Reg;							// 使LCD有效
	
	//NOP();
	//Delay_1us();
	//WDR();
	
	/*
	LCD_CTRL_Opt( CLRBIT(LCD_CtrlReg, PNO_LCD_EN) );
	*/
	//CLRBIT(u8Reg, PNO_LCD_EN);
	PORT_LCD_CTRL = u8Reg2;							// 使LCD锁存数据
	
	LCD_OS_EXIT_CRITICAL();
	
	//LCD_CTRL_Opt( CLRBITs(LCD_CtrlReg, BIT(PNO_LCD_RnW) ) );
	//LCD_CtrlReg = u8Reg;
	//PORT_LCD_CTRL = u8Reg;
}

void LCD_SdData(uint8 DData) 
{
	register uint8 u8Reg;
	register uint8 u8Reg2;
	
	LCD_WaitIdle();							// Add By Xsky 2008-5-24 18:25
	
	u8Reg = LCD_CtrlReg;									// 定义寄存器以优化代码
	CLRBITs(u8Reg, BIT(PNO_LCD_RnW) );					// LCD_RW = 0 写入状态
	SETBITs(u8Reg, BIT(PNO_LCD_RS) );		// LCD_RS = 1 选择数据寄存器
	LCD_CtrlReg = u8Reg;
	PORT_LCD_CTRL = u8Reg;								// 输出状态
	
	SETBIT(u8Reg, PNO_LCD_EN);
	//LCD_CtrlReg = u8Reg;
	u8Reg2 = u8Reg;
	CLRBIT(u8Reg2, PNO_LCD_EN);
	LCD_CtrlReg = u8Reg2;
	
#ifdef DEBUG
//lbLcdStrat:
	//WDR();
#endif
	
	//LCD_CTRL_Opt( SETBITs(LCD_CtrlReg, BIT(PNO_LCD_EN) | BIT(PNO_LCD_RS)) );
	
	LCD_OS_ENTER_CRITICAL();
	PORT_LCD_DATA = DData;						// 向数据口输出数据
	LCD_SET_BusOut();
	
	//LCD_CTRL_Opt( SETBIT(LCD_CtrlReg, PNO_LCD_EN) );
	PORT_LCD_CTRL = u8Reg;						// 使LCD有效
	
	//Delay_1us();
	//Delay_Nus(5);
	//WDR();
	
	/*
	LCD_CTRL_Opt( CLRBIT(LCD_CtrlReg, PNO_LCD_EN) );
	*/
	//CLRBIT(u8Reg, PNO_LCD_EN);
	PORT_LCD_CTRL = u8Reg2;						// 使LCD锁存数据
	
	LCD_OS_EXIT_CRITICAL();
	
#ifdef DEBUG
	//goto lbLcdStrat;
#endif
	
	//LCD_CTRL_Opt( CLRBITs(LCD_CtrlReg, BIT(PNO_LCD_RnW) ) );
	//PORT_LCD_CTRL = u8Reg;
	//LCD_CtrlReg = u8Reg;
}

