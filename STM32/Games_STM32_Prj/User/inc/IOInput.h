/****************************************Copyright (c)**************************************************
**                                  vixa
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: IOInput.h
**创   建   人: Xsky
**创 建 日  期: 2011-04-23 14:28
**描        述: 输入量检测相关定义及函数声明
**				在本文件或者外部文件定义输入的消息事件处理句柄勾子
**				#define  IOIN_EVENT_DO_Key1(event)
**				#define  IOIN_EVENT_DO_Key2(event)
**				#define  IOIN_EVENT_DO_Key3(event)
**				#define  IOIN_EVENT_DO_Key4(event)
**				#define  IOIN_EVENT_DO_CenterSensor(event)
**				#define  IOIN_EVENT_DO_VacuumHousing(event)
**				#define  IOIN_EVENT_DO_BallCounter(event)
**				#define  IOIN_EVENT_DO_Spin(event)
** 版  本: v1.0.00
**------------------------------------------------------------------------------------------------------
** 相关文件: IOInput.C
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
#ifndef __IOINPUT_H__
#define __IOINPUT_H__
#include "type.h"

#ifdef IOINPUT_GLOBALS
#define  IOINPUT_EXT
#else
#define  IOINPUT_EXT	extern
#endif

/***************************************************************************************************/
// 常数定义
#define  IOIN_Key1				0
#define  IOIN_Key2				1
#define  IOIN_Key3				2
#define  IOIN_Key4				3
#define  IOIN_CenterSensor		4		/* 中央球颜色识别位置感应,庶断计数 */
#define  IOIN_VacuumHousing		5		/* 真空罩 */
#define  IOIN_BallCounter		6		/* 外圈数球光眼 */
#define  IOIN_Spin				7		/* 平衡器,机台摇晃 */
#define  IOIN_ColorRed			8		/* 颜色线:红 */
#define  IOIN_ColorGreen		9		/* 颜色线:绿 */
#define  IOIN_ColorBlue			10		/* 颜色线:蓝 */
#define  IOIN_NUMS				11		/* 输入量总数 */


typedef enum uint8{IN_Invalid=FALSE, IN_Valid=TRUE}T_IOInState;
typedef enum{VOL_Low=0, VOL_High=1}T_ValidVoltage;
typedef enum{IN_Event_Up=0, IN_Event_Down=1}T_IOEvent;

// 输入量检测函数, 有效状态检测, 即输入若为有效电平,则函数返回值为1, 且函数返回值为uint8型
#define  CHK_IN_Key1()				CHKNBIT(GPIOB->IDR, 0)	/* PB0, 低电平有效 */
#define  CHK_IN_Key2()				CHKNBIT(GPIOB->IDR, 1)	/* PB1, 低电平有效 */
#define  CHK_IN_Key3()				CHKNBIT(GPIOB->IDR, 6)	/* PB6, 低电平有效 */
#define  CHK_IN_Key4()				CHKNBIT(GPIOB->IDR, 7)	/* PB7, 低电平有效 */
#define  CHK_IN_VacuumHousing() 	CHKNBIT(GPIOE->IDR, 12) /* PE12,真空罩检测,同相光耦输入 */
#define  CHK_IN_CenterSensor()		CHKBIT(GPIOC->IDR,  7)	/* PC7,颜色传感器遮断计数,低电平有效 */
#define  CHK_IN_BallCounter()		CHKNBIT(GPIOE->IDR, 15) /* PE15,遮断计数,检测到球时为低电平 */
#define  CHK_IN_Spin()				CHKNBIT(GPIOE->IDR, 14) /* PE14,摇动检测,低电平有效 */
#define  CHK_IN_ColorRed()			CHKNBIT(GPIOD->IDR, 8)	/* PD8, IO_IN2, 颜色输入线Red */
#define  CHK_IN_ColorGreen()		CHKNBIT(GPIOD->IDR, 9)	/* PD8, IO_IN3, 颜色输入线Red */
#define  CHK_IN_ColorBlue() 		CHKNBIT(GPIOD->IDR, 10) 	/* PD8, IO_IN4, 颜色输入线Red */

/***************************************************************************************************/
// 函数声明
extern void TIM2_Config(void);
extern void InputInit(void);

/* 获取毫秒时钟嘀嗒, uint32最长计时49.7天 */
#define GetTimeTick_ms()			g_Timer_ms
#define SetTimerFlag_ms(timer)		(timer) = g_Timer_ms
#define isNTimeOut_ms(timer,diff) 	(GetTimeTick_ms()-(timer) < (diff))
#define isTimeOut_ms(timer,diff)	(GetTimeTick_ms()-(timer) >= (diff))
#define SetTimerFlag_ms_n()			nTimer = g_Timer_ms
#define isNTimeOut_ms_n(diff)	 	(GetTimeTick_ms()-nTimer < (diff))
#define isTimeOut_ms_n(diff)		(GetTimeTick_ms()-nTimer >= (diff))


/* 输入线状态检查,直接读取g_InputState变量值即为输入线状态, index为IOIN_xxx */
#define CHK_InputState(index)		g_InputState[index]
#define CHK_IOIN(name)				g_InputState[IOIN_##name]

/***************************************************************************************************/
// 全局量引用说明
#ifndef IOINPUT_GLOBALS
// 输入量状态
extern volatile uint8 g_InputState[IOIN_NUMS];
// 输入量事件记数
extern volatile uint8 g_InputDownEvents[IOIN_NUMS];	
extern volatile uint8 g_InputUpEvents[IOIN_NUMS];	
// 毫秒计时器
extern volatile uint32 g_Timer_ms;		// 最大值49.710269618055555555555555555556天
#endif



/***************************************************************************************************/
// 调试
#ifdef DEBUG
void KeyESC_Event(T_IOEvent event);
#else
#define 	 	 KeyESC_Event			NULL
#endif


#endif	// end: __IOINPUT_H__

