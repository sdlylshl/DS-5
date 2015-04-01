/****************************************Copyright (c)**************************************************
**                                  vixa
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: ColorSensor.h
**��   ��   ��: Xsky
**�� �� ��  ��: 2011-04-15 09:10
**��        ��: ��ɫ������������������ض���, ��ɫ�������ͺ�:TCS3200D
** ��  ��: v1.0.00
**------------------------------------------------------------------------------------------------------
** ����ļ�: ColorSensor.c
**------------------------------------------------------------------------------------------------------
** ����޸�����: 
** �޸���: Xsky
** �汾: v1.0.00
**-------------------------------------------------------------------------------------------------------
** �޸���: Xsky
** ����: 
** �汾: v1.0.00
** �޸�: 
**			1. 
**			2. 
**			3. 
**			4. 
**			5. 
********************************************************************************************************/
#ifndef __COLORSENS_H__
#define __COLORSENS_H__

#include "IOInput.h"

#ifdef COLORSENS_GLOBALS
#define  COLORSENS_EXT
#else
#define  COLORSENS_EXT	extern
#endif

#define  COLOR_HARD_RGB				1		/* ������ɫRGB���뷽ʽ, ԭ������ɫ���������뷽ʽ */
#define  COLOR_HARD_FRQ				2		/* ��ɫ������Ƶ������ */
#define  COLOR_HARD					COLOR_HARD_RGB


/***************************************************************************************************/
// ��������
typedef uint8 t_color;			/* �Ѷ�����ɫ���� */
#define  COLOR_NUM		7		/* ��ɫ����1 */
#define  COLOR_1		1		
#define  COLOR_2		2
#define  COLOR_3		3
#define  COLOR_4		4
#define  COLOR_5		5
#define  COLOR_6		6
#define  COLOR_7		7
#define  COLOR_Undef	0x00	/* δ֪��ɫ */
#define  COLOR_Red		COLOR_1
#define  COLOR_Green	COLOR_2
#define  COLOR_Blue		COLOR_3
#define  COLOR_Yellow	COLOR_4
#define  COLOR_Purple	COLOR_5
#define  COLOR_Cyan		COLOR_6
#define  COLOR_White	COLOR_7
#define  COLOR_Black	COLOR_Undef

//COLORSENS_EXT __flash char ColorNames[][7];


//#define  XOut_ColorSel(c)			X_FPGA_IO_Out(0, COLORSENS_MASK, c)
#define  XOut_ColorLedOn()			X_IO_OutBits(X_IO_Index_OUT2, 0x40, 0xFF)
/* ����ɫ����������LED */
#define  XOut_ColorLedOff()			X_IO_OutBits(X_IO_Index_OUT2, 0x40, 0x00)	/* �ر���ɫ����������LED */


/***************************************************************************************************/
// ��������
//extern uint32 GetColor(uint8 ColorSelBits);

// ��ȡ��ǰ�����ɫ
COLORSENS_EXT t_color GetCurBallColor(void);

COLORSENS_EXT t_color RGB2ColorID(register uint8 r, register uint8 g, register uint8 b);


// ���Ժ���
COLORSENS_EXT char * ColorNames(t_color color);

/***************************************************************************************************/
// ȫ��������˵��
#ifndef COLORSENS_GLOBALS

#endif


#endif	// end: __COLORSENS_H__

