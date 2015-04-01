/*********************************************************************************
˵��:����СҺ������
��Ƭ��:stm321f103vct6
Һ���ͺ�:HDW_12232E REV1.0
�ô���������
����:spi2,PB12 PB13 PB15 ,PB14��Ϊ��ͨi/o�ڿ�������оƬ 
PB12�ڳ�ʼ����Ϊ��ͨi/o��������
20110517knimble 
1����Ӳ��spi���ԣ��Ƿ��ʺ�����
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


const uint8 CHNum[][2] = {"��","��", "��", "��", "��", "��", "��", "��", "��", "��"};

//��SPI2�ܽŵĳ�ʼ��
//��72M����������
void Init_Spi2(void)
{
	RCC->APB2ENR|=1<<3;   	//��B��ʱ��
	RCC->APB1ENR|=1<<14;		//��SPI2ʱ��

	GPIOB->CRH&=0X0000FFFF;
	GPIOB->CRH|=0xB3B30000;   //PB12 NSS,PB14 Miso��Ϊ��ͨI/O�ڣ�pb13sck,pb15mosi����������� 
	GPIOB->ODR&=0XEFFF;       //
	//
	SPI2->CR1|=1<<9; //���nss����
	SPI2->CR1|=1<<8;  

	SPI2->CR1|=1<<2; //SPI����
//	SPI1->CR1|=0<<11;//8bit���ݸ�ʽ 
	SPI2->CR1|=1<<1; //����ģʽ��SCKΪ1 CPOL=1
	SPI2->CR1|=1<<0; //���ݲ����ӵڶ���ʱ����ؿ�ʼ,CPHA=1	
	SPI2->CR1|=1<<5; //Fsck=Fcpu/16
	SPI2->CR1|=1<<3;  
	
	SPI2->CR1|=1<<6; //SPI�豸ʹ��	
}
//spi2���ݵ����
void SPI2_Senddata(uint8         TxData)
{
	
	GPIOB->ODR|=GPIO_Pin_12;
	GPIOB->ODR&=~GPIO_Pin_13;
	GPIOB->ODR&=~GPIO_Pin_15;
	SPI2->DR=TxData;	 	  //����һ��byte   
	while((SPI2->SR&0x0002)==0)//�ȴ���������				  
	{
		WdtFeed();                                   //ι��
	}
	Delay_Nus(12);
	GPIOB->ODR&=~GPIO_Pin_12;
}
/**********************************************************
��������LCD_ScanKeyDown
���ܣ�����ɨ�躯��
			ֻɨ��˵�������ذ���
���������λ�з��������¼�ʱ��Ӧ��λΪ1������Ϊ0
******************************************/

/*uint8 LCD_ScanKeyDown(void)
{
	// �ȴ�����
	static uint8 ucSavedKeyState=0;	// �����ϴ�ɨ��ʱ�İ���״̬
	uint8 ucKey=0;
	uint8 ucKey1=0;
	
	ucKey = (GET_KEY_MEUN_PIN);		// ȡ�ð�����������״̬
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
	Init_Spi2();				// ���ٷ����ĳ�ʼ���ӿں���, Add by XSky 2011-07-26 11:34
	
	Delay_Nms(10);
	
	LCD_LED_On();				// �������
	
	LCD_SdCmd(0x30);
	LCD_SdCmd(0x06);//���趨:��������
	LCD_SdCmd(0x0C);//lcd��������
	LCD_SdCmd(0x01);//����
	LCD_SdCmd(0x02);//��ַ��λ
	
	//LCD_SdCmd(0x07);
   	LCD_SdCmd(0x34);        // 8bit I/F, basic command
	// �л�������ָ�
   	LCD_SdCmd(0x30);        // 8bit I/F, basic command, graphic off
   	
	Delay_Nms(5);			// ���������ʱ����������ֽڿ��ܲ�����ʾ, ����1msʱҲ���ܲ�����, Add by Xsky 2011-07-26 14:10

	#ifdef DEBUG
	LCD_LED_Shut();
	#endif
}

////����һ���ַ�����////// 
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

// ucLen: ����λ��ʾ���ȣ�
// ���λΪ1ʱ��ʾ���ȫ���ַ�,0ʱ�������ַ�
//UCROW ��ʾ��ʼ���У�UCCOL��ʾ��ʼ����
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
			LCD_SdData(CHNum[ucBuf[i-1]][0]);		// ���ȫ���ַ�, �����ֽ�
			LCD_SdData(CHNum[ucBuf[i-1]][1]);
			//LCD_SdData(' ');
		}
		else
		{
		//#else
			LCD_SdData(ucBuf[i-1]+ 48);			// �������ַ����ֽ�
		}
		//#endif
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
	va_list ap;		// ����һ�� va_list ָ�������ʲ�����
	char sBuf[32];
	int32 i;
	t_err err = ERR_VSprintf;
	
	va_start(ap, fmt);                 	 	// ��ʼ�� ap������ָ���һ�����
	i = vsprintf(sBuf, fmt, ap);
	va_end(ap);                            	// �ƺ������ر� ap
	
	if(i >= 0)
	{
		memset(&sBuf[i], ' ', 16-i);
		LCD_SdStringPos((uint8 *)sBuf, row, col);
	}
	
	return err;
}
#endif

//��������
 void LCD_Clear(void)
 {
 	LCD_SdCmd(0x01);
	Delay_Nms(2);
 }

//�򿪷�����ʾ
void  LCD_BlkWhite(void)
{
      LCD_SdCmd(0x0d);
	  Delay_Nms(1);
}

// �رշ�����ʾ
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
//	SETBIT(PORT_LCD_CTRL, PIN_LCD_LIGHT);		// �ر����
	LCD_LED_Shut();								// �ر����, Add by Xsky 2011-07-27 17:54
	
	LCD_SdCmd(0x34); 
	LCD_SdCmd(0x0c);  
}

//��Һ����ʾ
void LCD_Disp_on(void)
{
//	CLRBIT(PORT_LCD_CTRL, PIN_LCD_LIGHT);		// �������
	LCD_LED_On();								// �������, Add by Xsky 2011-07-27 17:55
	LCD_SdData(0x0d);
}

// Parallel
void LCD_Send(uint8 senddata)
{
	//ԭ�������Һ�����ڷ���
/*	PORT_LCD_DATA = senddata;				// �����ݿ��������
	SETBIT(PORT_LCD_CTRL, PIN_LCD_EN);		// ����LCD
	CLRBIT(PORT_LCD_CTRL, PIN_LCD_EN);		// ����LCD_DATA*/
	SPI2_Senddata(senddata);
}

void LCD_SdCmd(uint8 scmd)   //send command
{	
	uint8 T_data=0;
	/*
	LCD_WaitIdle();							// Add By Xsky 2008-5-24 18:25
	SETBIT(PORT_LCD_CTRL, PIN_LCD_RW);		// ����
	PORT_LCD_DATA = scmd;				// �����ݿ��������
	SETBIT(PORT_LCD_CTRL, PIN_LCD_EN );		// ����LCD
	PORT_LCD_CTRL &= ~(BIT(PIN_LCD_RW) | BIT(PIN_LCD_RS));
	CLRBIT(PORT_LCD_CTRL, PIN_LCD_EN);		// ����LCD_DATA
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

//  ״̬��麯��,�ж��Ƿ���æ״̬
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

void LCD_DispSetCursor(uint8 LineNum, uint8 ColumnNum)
{

    uint8 i=0x00;

	if( (LineNum >= LCD_MAX_Rows) || (ColumnNum >= LCD_MAX_Cols) )	// Add by Xsky 2011-08-01 17:25
	{
		// �����Ч�������һ��λ��д
		return;
	}
    
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
    i = (ColumnNum&0x0f)|i; //ȷ���к�
   LCD_SdCmd(i);
}

//enf of LCD


void LCD_SetCurs_BlkWh(uint8 ucLine,uint8 ucColumn)
{
	LCD_DispSetCursor(ucLine,ucColumn);
	LCD_BlkWhite();
}

/*uint16 ChgVal(uint16 ucMenuID, uint16 uiVal,uint8 BitSet,int8 Det)//��ĳһλ��������
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
       	Lenth=sizeof(USE_LCD);  //ȡҪ��ʾ������
       }
	else if(ucMenuID==MENU_HOW_CONNECT)
	{
		Lenth=sizeof(CONNECT);  //ȡҪ��ʾ������
	}
	else if(ucMenuID==MENU_ABOUT)
	{
		Lenth=sizeof(ABOUT);  //ȡҪ��ʾ������	
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
