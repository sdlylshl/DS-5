/****************************************Copyright (c)**************************************************
**                                  vixa
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: ColorSensor.C
**��   ��   ��: Xsky
**�� �� ��  ��: 2011-04-15 09:10
**��        ��: ��ɫ��������������, ��ɫ�������ͺ�:TCS3200D
**				����: ����Ƶ�ʷ�Χ 20Hz~120KHz, S0:S1:HL
** ��  ��: v1.0.00
**------------------------------------------------------------------------------------------------------
** ����ļ�: ColorSensor.h
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

#define  COLORSENS_GLOBALS
#include "Global.h"
#include "Stm32f10x.h"
#include "Target.h"
#include "Timer5.h"
#include "ColorSensor.h"
#include "Delay.h"
#include "IOInput.h"
#include "function.h"


/***************************************************************************************************/
// ����
#ifndef COLOR_SENSOR_EN
#define	COLOR_SENSOR_EN		0
#endif
#if COLOR_SENSOR_EN > 0


#if IS_N_RELEASE()
static __flash char COLOR_Names[][8] = 
{
	"��err",			// 0
	"Red",				// COLOR_1
	"Green",			// 2
	"Blue",				// 3
	"Yellow",			// 4
	"Purple",			// 5
	"Cyan",				// 6
	"White"				// 7
};
char * ColorNames(t_color color)
{
	if(color > COLOR_NUM)
	{
		color = 0;
	}
	return (char *)COLOR_Names[color];
}
#endif

#if COLOR_HARD == COLOR_HARD_FRQ

/*********************************************************************************************************
** ��������: GetColor
** ��������: ѡ��TCS3200ָ������ɫ�����Ƶ��, �ȴ�ʱ��ΪTimer5�ж����TIMx_Update_Period_ms,100ms
** �䡡��:  1. Color: TCS3200, ���ļ�Lpc2220.h����
**				#define  COLORSENS_RED				0b000
**				#define  COLORSENS_BLUE				0b010
**				#define  COLORSENS_CLEAR			0b100
**				#define  COLORSENS_GREEN			0b110
** �䡡��: ��
** ������: R/G/Bͨ����Ӧ��TCS3200Ƶ��ֵ
** ȫ�ֱ���: ��
** ����ģ��: GetTim5Ch2Frequency(), XOut_ColorLedOn(), Delay_Nms(), XOut_ColorLedOff()
** �桡��: v0.0.01
** ������: Xsky
** �ա���: 
** ����ע: 
**-------------------------------------------------------------------------------------------------------
** �桡��: 
** �޸���: 
** �ա���: 
** �衡��: 
********************************************************************************************************/
static uint32 GetColorOne(uint8 ColorSelBits)
{
	register uint32 r;
	
	XOut_ColorSel(ColorSelBits);
	r = GetTim5Ch2Frequency();
	
	return r;
}

/*********************************************************************************************************
** ��������: GetColor
** ��������: ѡ��TCS3200ָ������ɫ�����Ƶ��, �ȴ�ʱ��ΪTimer5�ж����TIMx_Update_Period_ms,100ms
** �䡡��: ��
** �䡡��: ��
** ������: ����ֵ��ɫֵ
**		 #define  COLOR_1		1
**		 #define  COLOR_2		2
**		 #define  COLOR_3		3
**		 #define  COLOR_4		4
**		 #define  COLOR_5		5
** ȫ�ֱ���: ��
** ����ģ��: GetColorOne()
** �桡��: v0.0.01
** ������: Xsky
** �ա���: 2011-06-14 14:17
** ����ע: 
**-------------------------------------------------------------------------------------------------------
** �桡��: 
** �޸���: 
** �ա���: 
** �衡��: 
********************************************************************************************************/
t_color GetCurBallColor(void)
{
	uint32 R,G,B;	/* Red,Green,Blueͨ�� */
	uint32 L;		/* �׹��Ӧͨ�� */
	
	XOut_ColorLedOn();
	Delay_Nms(1);
	
	R = GetColorOne(COLORSENS_RED);
	G = GetColorOne(COLORSENS_GREEN);
	B = GetColorOne(COLORSENS_BLUE);
	L = GetColorOne(COLORSENS_CLEAR);
	
	XOut_ColorLedOff();
	
	if(R > 0) return COLOR_1;
	if(G > 0) return COLOR_2;
	if(B > 0) return COLOR_3;
	if(L > 0) return COLOR_4;
	
	return COLOR_Undef;
}

#else	// COLOR_HARD != COLOR_HARD_FRQ

t_color RGB2ColorID(register uint8 r, register uint8 g, register uint8 b)
{
	/* λ����: 0b: 0000 0RGB */
	static __flash t_color RGB2ColorTbl[8] = {
			/* 0:000 */ COLOR_Black, 	/* 1:00B */ COLOR_Blue, 
			/* 2:0G0 */ COLOR_Green, 	/* 3:0GB */ COLOR_Cyan, 
			/* 4:R00 */ COLOR_Red, 		/* 5:R0B */ COLOR_Purple, 
			/* 6:RG0 */ COLOR_Yellow, 	/* 7:RGB */ COLOR_White, 
			};
	
	uint8 rgbBits = 0;
	if(r > 0) rgbBits |= 0x04;
	if(g > 0) rgbBits |= 0x02;
	if(b > 0) rgbBits |= 0x01;
	
	return RGB2ColorTbl[rgbBits];
}

#endif


#endif	// end: #if COLOR_SENSOR_EN > 0 //

