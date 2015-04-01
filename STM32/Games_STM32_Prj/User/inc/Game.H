/*******************************************************************************
  Copyright (C), 2009-2011, YingHong, Ltd.
  File name:   	Game.H	
  Author:      	Xsky
  Version:     	1.0
  Date:		2011-02-25 09:07
  
  Description:   Game��ض���
  Others:         
  Function List: 
  	1. ��
  	2. ��
  	3. ��
  	4. ��
  	5. ��

  History:        // �޸ļ�¼
    1. Date:		2011-02-25 09:07
       Author:		Xsky
       Modification:	������ģ��
*******************************************************************************/
#ifndef __GAME_H__
#define __GAME_H__

#ifdef GAME_GLOBALS
#define GAME_EXT
#else
#define GAME_EXT		extern
#endif

#include "GameParam.h"

/***************************************************************************************************/
// �ؼ���������
/* �ر��ж� */
#define  GAME_OS_ENTER_CRITICAL()		OS_ENTER_CRITICAL()
/* ���ж� */
#define  GAME_OS_EXIT_CRITICAL()		OS_EXIT_CRITICAL()

/***************************************************************************************************/
// 
#define GAME_STATE_Init				0		// ��Ϸ��ʼ��״̬
#define GAME_STATE_ReStart			1		// ���¿�ʼ��Ϸ
#define GAME_STATE_WaitNext			2		/* ��Ϸ�ȴ�������һ�� */
#define GAME_STATE_BallCheck		3		/*  */
#define GAME_STATE_WagerInit		4		/*  */
#define GAME_STATE_Wager			5		/*  */
#define GAME_STATE_InsideLoop		6		/*  */
#define GAME_STATE_OutsideLoop1		7		/* 1 */
#define GAME_STATE_OutsideLoop2		8		/* 2 */
#define GAME_STATE_BallRecognise	9		/*  */
#define GAME_STATE_Open				10		/*  */
#define GAME_STATE_GameLuck			11		/*  */
#define GAME_STATE_EnExitNext		5		/* ����ȴ�����״̬Ϊ��״̬,����˳���Ϸ���� */
#define GAME_STATE_Top				11		/* ��Ϸ���״ֵ̬ */
#define GAME_STATE_Num				(GAME_STATE_Top+1) /* ״̬���� */
#define GAME_STATE_Error			0xFF	/* �������󣬲����ټ�����Ϸ */

/***************************************************************************************************/
// ���Ͷ���

/***************************************************************************************************/
// ȫ��������˵��
#ifndef GAME_GLOBALS
//extern T_PlayerWager	g_PlayerWager[MAX_PLAYER];
extern uint32 	g_TotalBets[BET_ITEMS];
extern uint32	g_Odds[BET_ITEMS];
extern uint16	g_MaxBet[BET_ITEMS];

//extern T_WagerBet  g_WagerBets[BET_ITEMS];

extern uint8	g_GameState;
extern uint8	g_GameTimerSec;
extern uint16	g_ErrorFlag;

#endif		// end: #ifndef GAME_GLOBALS

/***************************************************************************************************/
// ��������
GAME_EXT t_err SysGameInit(void);

GAME_EXT t_err Game(void);

GAME_EXT t_err GameTest(void);

GAME_EXT void ColorSensorEvent(T_IOEvent event);


/* ���ý����µ���Ϸ״̬ */
#define GameStateSet(s) 	GAME_OS_ENTER_CRITICAL(); \
							g_GameState = s; \
							GAME_OS_EXIT_CRITICAL()
#define GameStateSetQ(s)	g_GameState = s
#define GameStateGet()		g_GameState
#define GameStateRead(r)	GAME_OS_ENTER_CRITICAL(); \
							r = g_GameState; \
							GAME_OS_EXIT_CRITICAL()





#endif	// end: #define __GAME_H__ //