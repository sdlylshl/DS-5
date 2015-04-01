/*******************************************************************************
  Copyright (C), 2009-2009, YingHong, Ltd.
  File name:   	LCD.H	
  Author:      	Xsky
  Version:     	1.0
  Date:		2009-04-25
  
  Description:   LCD��������
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
#ifndef __LCD_H__
#define __LCD_H__

#ifdef LCD_GLOBALS
#define LCD_EXT
#else
#define LCD_EXT		extern
#endif

/*****************************************************************************/
// LCDӦ�ó���ѡ��
#ifndef LCD_CRITICAL_EN	
#define  LCD_CRITICAL_EN		0
#endif
#if (LCD_CRITICAL_EN > 0)
#define LCD_OS_ENTER_CRITICAL()	ENTER_EXTBUS_CRITICAL()
#define LCD_OS_EXIT_CRITICAL()		EXIT_EXTBUS_CRITICAL()
#else
#define LCD_OS_ENTER_CRITICAL()	
#define LCD_OS_EXIT_CRITICAL()		
#endif

/*****************************************************************************/
// LCDӲ���ӿڶ���
// 
#define PORT_LCD_CTRL		EXT_PORT1			// LCD���ƼĴ�����ַ
#define LCD_CtrlReg			g_ExtPort1
#define LCD_OUT_CTRL()		EXT_PORT1 = g_ExtPort1

#define LCD_CTRL_Opt(code)	code; 	LCD_OUT_CTRL()
#define LCD_SET_BusOut()		DDR_LCD_DATA = 0xFF
#define LCD_SET_BusIn()		DDR_LCD_DATA = 0x00
#define LCD_DBUS_Out(data)	PORT_LCD_DATA = data

// LCD��λ
#define LCD_SET_RST()		/*CLRBIT(LCD_CtrlReg, PNO_LCD_nRST); LCD_OUT_CTRL()*/
// LCD�����λ
#define LCD_CLR_RST()		/*SETBIT(LCD_CtrlReg, PNO_LCD_nRST); LCD_OUT_CTRL()*/
// ѡ�����ݼĴ���
#define LCD_SET_RS_Data()	/*LCD_CTRL_Opt( SETBIT(LCD_CtrlReg, PNO_LCD_RS) )*/
// ѡ������Ĵ���
#define LCD_SET_RS_Cmd()	/*LCD_CTRL_Opt( CLRBIT(LCD_CtrlReg, PNO_LCD_RS) )*/
// ����LCD�����
#define LCD_LightOn()		/*CLRBIT(PORT_LCD_LEDK, PNO_LCD_LEDK)*/
// �ر�LCD�����
#define LCD_LoghtOff()		/*SETBIT(PORT_LCD_LEDK, PNO_LCD_LEDK)*/


/*****************************************************************************/
// LCD��������
#define  LCD_MAX_RowNum	1		// ����к�
#define  LCD_MAX_Rows		2		// �����ʾ����
#define  LCD_MAX_Cols		7		// ��ʾ����, ȫ����

/*****************************************************************************/
// LCD��������
//LCD_EXT uint8   LCD_ReadData(void);
LCD_EXT void   LCD_WaitIdle(void);
LCD_EXT void   LCD_Init(void);
//LCD_EXT void   LCD_Send(uint8 senddata);
LCD_EXT void   LCD_SdCmd(uint8 scmd);
LCD_EXT void   LCD_SdData(uint8 DData); 
LCD_EXT void   LCD_SdIntData(uint16 uiData, uint8 ucLen,  uint8 ucRow, uint8 ucCol);
LCD_EXT void   LCD_SdString(const uint8 *temp);
LCD_EXT void   LCD_SdStringPos(const uint8 *Str,uint8  X,uint8  Y);
#if (TESTOUTLCD == TRUE) || (TESTOUTLCD_EE==TRUE)
LCD_EXT void   LCD_SdStringT(uint8 *temp);
#endif
//��������
LCD_EXT void   LCD_Clear(void);
//��Һ����ʾ
LCD_EXT void   LCD_Disp_on(void);
//void    LCD_DispSetCursor(uint8 LineNum, uint8 ColumnNum);
/*
//void LCD_WriteTextScreen(uint8 *TxtData);
//void LCD_WriteTextScreen2(uint8 *pstr);
//void LCD_WriteGraphicScreen(uint8 *GDData);
void LCD_Show(uint8  X,uint8  Y,const uint8 *Z);
void LCD_SdStringPosT(const uint8 *temp,uint8  X,uint8  Y);
*/
LCD_EXT void   LCD_BlkWhite(void);
LCD_EXT void   LCD_Shut(void);
LCD_EXT void   LCD_BlkWhite_Off(void);
LCD_EXT void   LCD_DispSetCursor(uint8 LineNum, uint8 ColumnNum);
LCD_EXT void   LCD_SetCurs_BlkWh(uint8 ucLine,uint8 ucColumn);


#endif	// end: #define __LCD_H__ //