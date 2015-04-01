/****************************************Copyright (c)**************************************************
**                                  vixa
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: ColorSensor.h
**创   建   人: Xsky
**创 建 日  期: 2011-04-15 09:10
**描        述: 颜色传感器驱动函数及相关定义, 颜色传感器型号:TCS3200D
** 版  本: v1.0.00
**------------------------------------------------------------------------------------------------------
** 相关文件: ColorSensor.c
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
#ifndef __COLORSENS_H__
#define __COLORSENS_H__

#include "IOInput.h"

#ifdef COLORSENS_GLOBALS
#define  COLORSENS_EXT
#else
#define  COLORSENS_EXT	extern
#endif

#define  COLOR_HARD_RGB				1		/* 三线颜色RGB输入方式, 原样机颜色传感器输入方式 */
#define  COLOR_HARD_FRQ				2		/* 颜色传感器频率输入 */
#define  COLOR_HARD					COLOR_HARD_RGB


/***************************************************************************************************/
// 常数定义
typedef uint8 t_color;			/* 已定义颜色种类 */
#define  COLOR_NUM		7		/* 颜色种类1 */
#define  COLOR_1		1		
#define  COLOR_2		2
#define  COLOR_3		3
#define  COLOR_4		4
#define  COLOR_5		5
#define  COLOR_6		6
#define  COLOR_7		7
#define  COLOR_Undef	0x00	/* 未知颜色 */
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
/* 打开颜色传感器照明LED */
#define  XOut_ColorLedOff()			X_IO_OutBits(X_IO_Index_OUT2, 0x40, 0x00)	/* 关闭颜色传感器照明LED */


/***************************************************************************************************/
// 函数声明
//extern uint32 GetColor(uint8 ColorSelBits);

// 读取当前球的颜色
COLORSENS_EXT t_color GetCurBallColor(void);

COLORSENS_EXT t_color RGB2ColorID(register uint8 r, register uint8 g, register uint8 b);


// 调试函数
COLORSENS_EXT char * ColorNames(t_color color);

/***************************************************************************************************/
// 全局量引用说明
#ifndef COLORSENS_GLOBALS

#endif


#endif	// end: __COLORSENS_H__

