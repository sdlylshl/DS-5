/*******************************************************************************
  Copyright (C), 2009-2011, YingHong, Ltd.
  File name:   	Global.H	
  Author:      	Xsky
  Version:     	1.0
  Date:		2011-02-24 09:53	
  
  Description:   ȫ�ֶ���
  Others:         
  Function List: 
  	1. ��
  	2. ��
  	3. ��
  	4. ��
  	5. ��

  History:        // �޸ļ�¼
    1. Date:		2009-04-08 16:25
       Author:		Xsky
       Modification:	������ģ��
*******************************************************************************/

#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#define HARD_VER		0x0022	// x.x.xx, Ӳ���汾
#define SOFT_VER		0x0001	// x.x.xx, �����汾


/****************************************************************************/
// ���뿪��

#define  RELEASE		0		// 1: ����, 0:�Ƿ���

#if RELEASE != 1
#define  DEBUG	
#endif

#define IS_RELEASE()		(RELEASE == 1)
#define IS_N_RELEASE()		(RELEASE != 1)

#ifdef DEBUG
//#define	DBG_PORT_INIT		// ��ȫ���˿ڳ�ʼ��Ϊ����
#define  DBG_UART1_LOOP	// UartӦ�ó���ػ�
#define  DBG_VKUART16_LOOP		// UartӦ�ó���ػ�

#define  DBG_CAN				// ����CAN
#define	 DBG_CAN_QUE_MAX		// CAN���ü�¼�������ֵ
#endif

#define  DBG_COLOR_STAT_F		// ��ɫͳ�����
#ifdef DBG_COLOR_STAT_F
#undef DEBUG
#endif

#ifdef DEBUG
#define  _DBG_(code)		code
#else
#define  _DBG_(code)		
#endif

#ifdef DEBUG
//extern uint8	DbgBuf[512];
#endif

#include "type.h"
#include "STM32_Type.h"
#include "Target.h"
#include "StateCtrl.h"

/***************************************************************************************************/
// ����ѡ��

/***************************************************************************************************/
// ������붨��
#define  ERR_NONE					0x0000
#define  ERR_BallNum				0x0100
#define  ERR_UndefColor				0x0101	/* ����ɫ���� */
#define  ERR_MainGameTimeOut		0x0102	/* ����Ϸ��ʱ */
#define  ERR_VacuumHousing			0x
#define  ERR_OutsideTrack			0x
#define  ERR_AccountLimit			0x
#define  ERR_Spin					0x
#define  ERR_PowerOnCheck			0x
#define  ERR_InsideTrack			0x

#define  ERR_InsideWaitBallTimeOut	0x0101	/* �ȴ���ʱ */
#define  ERR_InsideLoopSwitchErr	0x0102	/* ѭ��/ֹͣ���ش��� */
#define  ERR_OutsideLoop1Err		0x0103	/* ��һȦ��ʱ */
#define  ERR_OutsideLoop2Err		0x0104	/* �ڶ�Ȧ��ʱ */

#define  ERR_ParamCheck				0x0801	/* ���������� */

#define	 ERR_UsartTxQueueFull		0x0802	/* UART���Ͷ����� */
#define  ERR_UsartRxQueueFull		0x0803	/* UART���ն����� */

#define  ERR_GameStateUndef			0x0902	/* δ������Ϸ״̬���� */
#define  ERR_TimerUndef				0x0903	/* δ������Ϸ״̬���� */
#define  ERR_UndefPlayerData		0x0904	/* δ������Ϸ֡ */
#define  ERR_FrameError				0x0905	/* ֡���� */
#define  ERR_SendToLong				0x0906	/* �������͵��ֽ���̫�� */
#define  ERR_VSprintf				0x0907	/* vsprintf���ش��� */

#endif