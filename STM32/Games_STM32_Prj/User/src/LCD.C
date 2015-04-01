/*******************************************************************************
  Copyright (C), 2009-2009, YingHong, Ltd.
  File name:   	LCD.C	
  Author:      	Xsky
  Version:     	1.0
  Date:		2009-04-25
  
  Description:   LCD�������� RSMHV12232V10
  Others:         
  Function List: 
  	1. ��
  	2. ��
  	3. ��
  	4. ��
  	5. ��

  History:        // �޸ļ�¼
    1. Date:		2009-04-25
       Author:		Xsky
       Modification:	������ģ��
*******************************************************************************/
#define LCD_GLOBALS
#include "Config.H"

void LCD_Init(void)
{ 	
 	//�˿ڳ�ʼ��
	//LCD_DATA_DDR=0xFF;
	
	// �������
	//PORT_LCD_CTRL_RST&=~(BIT(PIN_LCD_RST));
	LCD_SET_RST();
	//PORT_LCD_CTRL &= ~(BIT(PIN_LCD_LIGHT)|BIT(PIN_LCD_EN)|BIT(PIN_LCD_RS));
	LCD_LightOn();
	LCD_CTRL_Opt( CLRBITs(LCD_CtrlReg, BIT(PNO_LCD_EN)|BIT(PNO_LCD_RS)) );
	Delay_Nms(3);		// 1
	//SETBIT(PORT_LCD_CTRL_RST, PIN_LCD_RST);		// ȡ����λ
	LCD_CLR_RST();
	
	Delay_Nms(5);			// �����Դ˶�������ʱ���һ����������������ʾ
	
	LCD_SdCmd(0x30);		// ����ָ�
	LCD_SdCmd(0x06);		// ���趨:��������
	LCD_SdCmd(0x0C);		// lcd��������
	//LCD_SdCmd(0x01);		//����
	LCD_Clear();				// 0x01, �����ʾ��Ļ����DDRAMλַ����������Ϊ"00H"
	LCD_SdCmd(0x03);		// ��DDRAMλַ����������Ϊ"00H"���α��ԭ�㣬�ù��ܲ�Ӱ����ʾDDRAM
	LCD_SdCmd(0x02);		// ��ַ��λ
	
	//Delay_Nms(25);			// �����Դ˶�������ʱ���һ����������������ʾ
	// �л�������ָ�
   	LCD_SdCmd(0x34);       	// 8bit I/F, basic command // ��������������ȱ�����һ���ϵ�����ʾ
   	LCD_SdCmd(0x30);        	// 8bit I/F, basic command, graphic off
   	
	Delay_Nms(15);			// �����Դ˶�������ʱ���һ����������������ʾ
   	LCD_SdCmd(0x34);       	// 8bit I/F, basic command // ��������������ȱ�����һ���ϵ�����ʾ
   	LCD_SdCmd(0x30);        	// 8bit I/F, basic command, graphic off
	
}

////����һ���ַ�����////// 
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

__flash uint8 CHNum[] = {"��������������������"};
// ucLen: ����λ��ʾ���ȣ�
// ���λΪ1ʱ��ʾ���ȫ���ַ�,0ʱ�������ַ�
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
			LCD_SdData(CHNum[ucBuf[i-1] * 2]);		// ���ȫ���ַ�, �����ֽ�
			LCD_SdData(CHNum[ucBuf[i-1] * 2 + 1]);
			//LCD_SdData(' ');
		}
		else
		{
			LCD_SdData(ucBuf[i-1]+ 48);			// �������ַ����ֽ�
		}
	}
	//SEI();
}
/*
// ����һ���ַ�����
// ��ָ�����λ�ÿ�ʼ��ʾָ���ַ��� 
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
// ��ָ�����λ�ÿ�ʼ��ʾָ���ַ��� 
void   LCD_Show(uint8  X,uint8  Y,const uint8 *Z)
{  
	LCD_DispSetCursor(X,Y);
	Delay_Nus(10);
	LCD_SdString(Z);
	//void SdString(uint8 *temp)
 }
*/
// ��ָ�����λ�ÿ�ʼ��ʾָ���ַ���
void   LCD_SdStringPos(const uint8 *Str,uint8  X,uint8  Y)
{
	if(X < LCD_MAX_Rows){	// Add By Xsky 2008-07-29 16:04
		LCD_DispSetCursor(X, Y);
	}
	//Delay_Nus(10);
	LCD_SdString(Str);
	//void SdString(uint8 *temp)
 }

//��������
 void LCD_Clear(void)
 {
 	LCD_SdCmd(0x01);
	//Delay_Nms(2);
 }

//�򿪷�����ʾ
void  LCD_BlkWhite(void)
{
      LCD_SdCmd(0x0d);
      //Delay_1ms();
}

// �رշ�����ʾ
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

//��Һ����ʾ
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
    switch(LineNum&0x0f)   //ȷ���к�
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
    i = (ColumnNum&0x0f)|i; 			//ȷ���к�
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
 	
	SETBIT(PORT_LCD_CTRL, PIN_LCD_RS);  	//����
	CLRBIT(PORT_LCD_CTRL, PIN_LCD_RW);  //����
 		//CLI();
	//CLRBIT(PORT_LCD_CTRL, PIN_LCD_RS);  	//RS=0, 
		SETBIT(PORT_LCD_CTRL, PIN_LCD_RW);  //RW=1, Read �����Ա���������RW������EN
 		DDR_LCD_DATA = 0x00;				// portA as input
 	//SETBIT(PORT_LCD_CTRL, PIN_LCD_EN);
	CLRBIT(PORT_LCD_CTRL, PIN_LCD_RS);  	//���Ծ����Ա���������RS������EN
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
//  ״̬��麯��,�ж��Ƿ���æ״̬
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
 	   	PORT_LCD_CTRL = u8Reg;					// ���������RS/RW, �����EN
	 	
 	   	SETBIT(u8Reg, PNO_LCD_EN);
	 	LCD_CtrlReg = u8Reg;
	 	u8Reg2 = u8Reg;
	 	CLRBIT(u8Reg2, PNO_LCD_EN);
	 	LCD_CtrlReg = u8Reg2;
 	   	
		LCD_OS_ENTER_CRITICAL();
		LCD_DBUS_Out(0xFF);							// ʹ����������
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
		
		//break;										// ��Break��֤ʵLCD Busy Ϊ��Ч�ֶ�, ȥ����LCD������������// By Xsky 2009-05-08 14:20
    	} while ( (dat & 0x80) && (iCount<200));
    	//_DBG_( sprintf((char *)DbgBuf, "iCnt:%u; ", iCount) );	// By Xsky 2009-05-08 14:20
	//_DBG_( Uart1Send(DbgBuf, strlen(DbgBuf)) );			
	// iCnt:1; iCnt:3; iCnt:5; iCnt:6; iCnt:1; iCnt:4; iCnt:6; iCnt:5; iCnt:3; iCnt:6; iCnt:4; iCnt:5; iCnt:6; iCnt:1; iCnt:5; iCnt:3; iCnt:6; iCnt:5; iCnt:4; iCnt:5; iCnt:5; iCnt:4; iCnt:5; iCnt:5; 
	// ������LCD Busy ״̬��Ч���ɼ���Һ��Busy
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
	
	PORT_LCD_DATA = senddata;				// �����ݿ��������
	//SETBIT(PORT_LCD_CTRL, PIN_LCD_EN);		// ����LCD
	//LCD_CTRL_Opt(SETBIT(LCD_CtrlReg, PNO_LCD_EN));
	PORT_LCD_CTRL = u8Reg;
	//Delay_Nus(5);
	//WDR();
	//CLRBIT(PORT_LCD_CTRL, PIN_LCD_EN);		// ����LCD_DATA
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
	u8Reg = LCD_CtrlReg;									// ����Ĵ������Ż�����
	CLRBITs(u8Reg, BIT(PNO_LCD_RnW) | BIT(PNO_LCD_RS)); 	// LCD_RW = 0 д��״̬// LCD_RS = 0 ѡ������Ĵ���
	LCD_CtrlReg = u8Reg;
	PORT_LCD_CTRL = u8Reg;
	
	SETBIT(u8Reg, PNO_LCD_EN);
	//LCD_CtrlReg = u8Reg;
	u8Reg2 = u8Reg;
	CLRBIT(u8Reg2, PNO_LCD_EN);
	LCD_CtrlReg = u8Reg2;
	
	LCD_OS_ENTER_CRITICAL();
	LCD_SET_BusOut();
	
	PORT_LCD_DATA = scmd;								// �����ݿ��������
	
	/*
	LCD_CTRL_Opt( SETBIT(LCD_CtrlReg, PNO_LCD_EN) );
	*/
	//LCD_CtrlReg = u8Reg;
	PORT_LCD_CTRL = u8Reg;							// ʹLCD��Ч
	
	//NOP();
	//Delay_1us();
	//WDR();
	
	/*
	LCD_CTRL_Opt( CLRBIT(LCD_CtrlReg, PNO_LCD_EN) );
	*/
	//CLRBIT(u8Reg, PNO_LCD_EN);
	PORT_LCD_CTRL = u8Reg2;							// ʹLCD��������
	
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
	
	u8Reg = LCD_CtrlReg;									// ����Ĵ������Ż�����
	CLRBITs(u8Reg, BIT(PNO_LCD_RnW) );					// LCD_RW = 0 д��״̬
	SETBITs(u8Reg, BIT(PNO_LCD_RS) );		// LCD_RS = 1 ѡ�����ݼĴ���
	LCD_CtrlReg = u8Reg;
	PORT_LCD_CTRL = u8Reg;								// ���״̬
	
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
	PORT_LCD_DATA = DData;						// �����ݿ��������
	LCD_SET_BusOut();
	
	//LCD_CTRL_Opt( SETBIT(LCD_CtrlReg, PNO_LCD_EN) );
	PORT_LCD_CTRL = u8Reg;						// ʹLCD��Ч
	
	//Delay_1us();
	//Delay_Nus(5);
	//WDR();
	
	/*
	LCD_CTRL_Opt( CLRBIT(LCD_CtrlReg, PNO_LCD_EN) );
	*/
	//CLRBIT(u8Reg, PNO_LCD_EN);
	PORT_LCD_CTRL = u8Reg2;						// ʹLCD��������
	
	LCD_OS_EXIT_CRITICAL();
	
#ifdef DEBUG
	//goto lbLcdStrat;
#endif
	
	//LCD_CTRL_Opt( CLRBITs(LCD_CtrlReg, BIT(PNO_LCD_RnW) ) );
	//PORT_LCD_CTRL = u8Reg;
	//LCD_CtrlReg = u8Reg;
}
