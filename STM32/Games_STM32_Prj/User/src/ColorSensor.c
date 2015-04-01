/****************************************Copyright (c)**************************************************
**                                  vixa
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: ColorSensor.C
**创   建   人: Xsky
**创 建 日  期: 2011-04-15 09:10
**描        述: 颜色传感器驱动函数, 颜色传感器型号:TCS3200D
**				参数: 测量频率范围 20Hz~120KHz, S0:S1:HL
** 版  本: v1.0.00
**------------------------------------------------------------------------------------------------------
** 相关文件: ColorSensor.h
**------------------------------------------------------------------------------------------------------
** 最后修改日期: 
** 修改人: Xsky
** 版本: v1.0.00
**-------------------------------------------------------------------------------------------------------
** 修改人: Xsky
** 日期: 
** 版本: v1.0.00
** 修改: 
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
// 正文
#ifndef COLOR_SENSOR_EN
#define	COLOR_SENSOR_EN		0
#endif
#if COLOR_SENSOR_EN > 0


#if IS_N_RELEASE()
static __flash char COLOR_Names[][8] = 
{
	"▲err",			// 0
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
** 函数名称: GetColor
** 功能描述: 选择TCS3200指定的颜色检测其频率, 等待时间为Timer5中定义的TIMx_Update_Period_ms,100ms
** 输　入:  1. Color: TCS3200, 见文件Lpc2220.h定义
**				#define  COLORSENS_RED				0b000
**				#define  COLORSENS_BLUE				0b010
**				#define  COLORSENS_CLEAR			0b100
**				#define  COLORSENS_GREEN			0b110
** 输　出: 无
** 返　回: R/G/B通道对应的TCS3200频率值
** 全局变量: 无
** 调用模块: GetTim5Ch2Frequency(), XOut_ColorLedOn(), Delay_Nms(), XOut_ColorLedOff()
** 版　本: v0.0.01
** 作　者: Xsky
** 日　期: 
** 备　注: 
**-------------------------------------------------------------------------------------------------------
** 版　本: 
** 修改人: 
** 日　期: 
** 描　述: 
********************************************************************************************************/
static uint32 GetColorOne(uint8 ColorSelBits)
{
	register uint32 r;
	
	XOut_ColorSel(ColorSelBits);
	r = GetTim5Ch2Frequency();
	
	return r;
}

/*********************************************************************************************************
** 函数名称: GetColor
** 功能描述: 选择TCS3200指定的颜色检测其频率, 等待时间为Timer5中定义的TIMx_Update_Period_ms,100ms
** 输　入: 无
** 输　出: 无
** 返　回: 返回值颜色值
**		 #define  COLOR_1		1
**		 #define  COLOR_2		2
**		 #define  COLOR_3		3
**		 #define  COLOR_4		4
**		 #define  COLOR_5		5
** 全局变量: 无
** 调用模块: GetColorOne()
** 版　本: v0.0.01
** 作　者: Xsky
** 日　期: 2011-06-14 14:17
** 备　注: 
**-------------------------------------------------------------------------------------------------------
** 版　本: 
** 修改人: 
** 日　期: 
** 描　述: 
********************************************************************************************************/
t_color GetCurBallColor(void)
{
	uint32 R,G,B;	/* Red,Green,Blue通道 */
	uint32 L;		/* 白光对应通道 */
	
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
	/* 位构成: 0b: 0000 0RGB */
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

