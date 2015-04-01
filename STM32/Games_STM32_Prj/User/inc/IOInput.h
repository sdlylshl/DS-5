/****************************************Copyright (c)**************************************************
**                                  vixa
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: IOInput.h
**��   ��   ��: Xsky
**�� �� ��  ��: 2011-04-23 14:28
**��        ��: �����������ض��弰��������
**				�ڱ��ļ������ⲿ�ļ������������Ϣ�¼�����������
**				#define  IOIN_EVENT_DO_Key1(event)
**				#define  IOIN_EVENT_DO_Key2(event)
**				#define  IOIN_EVENT_DO_Key3(event)
**				#define  IOIN_EVENT_DO_Key4(event)
**				#define  IOIN_EVENT_DO_CenterSensor(event)
**				#define  IOIN_EVENT_DO_VacuumHousing(event)
**				#define  IOIN_EVENT_DO_BallCounter(event)
**				#define  IOIN_EVENT_DO_Spin(event)
** ��  ��: v1.0.00
**------------------------------------------------------------------------------------------------------
** ����ļ�: IOInput.C
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
#ifndef __IOINPUT_H__
#define __IOINPUT_H__
#include "type.h"

#ifdef IOINPUT_GLOBALS
#define  IOINPUT_EXT
#else
#define  IOINPUT_EXT	extern
#endif

/***************************************************************************************************/
// ��������
#define  IOIN_Key1				0
#define  IOIN_Key2				1
#define  IOIN_Key3				2
#define  IOIN_Key4				3
#define  IOIN_CenterSensor		4		/* ��������ɫʶ��λ�ø�Ӧ,���ϼ��� */
#define  IOIN_VacuumHousing		5		/* ����� */
#define  IOIN_BallCounter		6		/* ��Ȧ������� */
#define  IOIN_Spin				7		/* ƽ����,��̨ҡ�� */
#define  IOIN_ColorRed			8		/* ��ɫ��:�� */
#define  IOIN_ColorGreen		9		/* ��ɫ��:�� */
#define  IOIN_ColorBlue			10		/* ��ɫ��:�� */
#define  IOIN_NUMS				11		/* ���������� */


typedef enum uint8{IN_Invalid=FALSE, IN_Valid=TRUE}T_IOInState;
typedef enum{VOL_Low=0, VOL_High=1}T_ValidVoltage;
typedef enum{IN_Event_Up=0, IN_Event_Down=1}T_IOEvent;

// ��������⺯��, ��Ч״̬���, ��������Ϊ��Ч��ƽ,��������ֵΪ1, �Һ�������ֵΪuint8��
#define  CHK_IN_Key1()				CHKNBIT(GPIOB->IDR, 0)	/* PB0, �͵�ƽ��Ч */
#define  CHK_IN_Key2()				CHKNBIT(GPIOB->IDR, 1)	/* PB1, �͵�ƽ��Ч */
#define  CHK_IN_Key3()				CHKNBIT(GPIOB->IDR, 6)	/* PB6, �͵�ƽ��Ч */
#define  CHK_IN_Key4()				CHKNBIT(GPIOB->IDR, 7)	/* PB7, �͵�ƽ��Ч */
#define  CHK_IN_VacuumHousing() 	CHKNBIT(GPIOE->IDR, 12) /* PE12,����ּ��,ͬ��������� */
#define  CHK_IN_CenterSensor()		CHKBIT(GPIOC->IDR,  7)	/* PC7,��ɫ�������ڶϼ���,�͵�ƽ��Ч */
#define  CHK_IN_BallCounter()		CHKNBIT(GPIOE->IDR, 15) /* PE15,�ڶϼ���,��⵽��ʱΪ�͵�ƽ */
#define  CHK_IN_Spin()				CHKNBIT(GPIOE->IDR, 14) /* PE14,ҡ�����,�͵�ƽ��Ч */
#define  CHK_IN_ColorRed()			CHKNBIT(GPIOD->IDR, 8)	/* PD8, IO_IN2, ��ɫ������Red */
#define  CHK_IN_ColorGreen()		CHKNBIT(GPIOD->IDR, 9)	/* PD8, IO_IN3, ��ɫ������Red */
#define  CHK_IN_ColorBlue() 		CHKNBIT(GPIOD->IDR, 10) 	/* PD8, IO_IN4, ��ɫ������Red */

/***************************************************************************************************/
// ��������
extern void TIM2_Config(void);
extern void InputInit(void);

/* ��ȡ����ʱ�����, uint32���ʱ49.7�� */
#define GetTimeTick_ms()			g_Timer_ms
#define SetTimerFlag_ms(timer)		(timer) = g_Timer_ms
#define isNTimeOut_ms(timer,diff) 	(GetTimeTick_ms()-(timer) < (diff))
#define isTimeOut_ms(timer,diff)	(GetTimeTick_ms()-(timer) >= (diff))
#define SetTimerFlag_ms_n()			nTimer = g_Timer_ms
#define isNTimeOut_ms_n(diff)	 	(GetTimeTick_ms()-nTimer < (diff))
#define isTimeOut_ms_n(diff)		(GetTimeTick_ms()-nTimer >= (diff))


/* ������״̬���,ֱ�Ӷ�ȡg_InputState����ֵ��Ϊ������״̬, indexΪIOIN_xxx */
#define CHK_InputState(index)		g_InputState[index]
#define CHK_IOIN(name)				g_InputState[IOIN_##name]

/***************************************************************************************************/
// ȫ��������˵��
#ifndef IOINPUT_GLOBALS
// ������״̬
extern volatile uint8 g_InputState[IOIN_NUMS];
// �������¼�����
extern volatile uint8 g_InputDownEvents[IOIN_NUMS];	
extern volatile uint8 g_InputUpEvents[IOIN_NUMS];	
// �����ʱ��
extern volatile uint32 g_Timer_ms;		// ���ֵ49.710269618055555555555555555556��
#endif



/***************************************************************************************************/
// ����
#ifdef DEBUG
void KeyESC_Event(T_IOEvent event);
#else
#define 	 	 KeyESC_Event			NULL
#endif


#endif	// end: __IOINPUT_H__

