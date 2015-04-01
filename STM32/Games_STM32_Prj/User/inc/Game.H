/*******************************************************************************
  Copyright (C), 2009-2011, YingHong, Ltd.
  File name:   	Game.H	
  Author:      	Xsky
  Version:     	1.0
  Date:		2011-02-25 09:07
  
  Description:   Game相关定义
  Others:         
  Function List: 
  	1. ：
  	2. ：
  	3. ：
  	4. ：
  	5. ：

  History:        // 修改记录
    1. Date:		2011-02-25 09:07
       Author:		Xsky
       Modification:	建立本模块
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
// 关键函数定义
/* 关闭中断 */
#define  GAME_OS_ENTER_CRITICAL()		OS_ENTER_CRITICAL()
/* 开中断 */
#define  GAME_OS_EXIT_CRITICAL()		OS_EXIT_CRITICAL()

/***************************************************************************************************/
// 
#define GAME_STATE_Init				0		// 游戏初始化状态
#define GAME_STATE_ReStart			1		// 重新开始游戏
#define GAME_STATE_WaitNext			2		/* 游戏等待进入下一局 */
#define GAME_STATE_BallCheck		3		/*  */
#define GAME_STATE_WagerInit		4		/*  */
#define GAME_STATE_Wager			5		/*  */
#define GAME_STATE_InsideLoop		6		/*  */
#define GAME_STATE_OutsideLoop1		7		/* 1 */
#define GAME_STATE_OutsideLoop2		8		/* 2 */
#define GAME_STATE_BallRecognise	9		/*  */
#define GAME_STATE_Open				10		/*  */
#define GAME_STATE_GameLuck			11		/*  */
#define GAME_STATE_EnExitNext		5		/* 如果等待进入状态为该状态,则可退出游戏过程 */
#define GAME_STATE_Top				11		/* 游戏最大状态值 */
#define GAME_STATE_Num				(GAME_STATE_Top+1) /* 状态总数 */
#define GAME_STATE_Error			0xFF	/* 致命错误，不能再继续游戏 */

/***************************************************************************************************/
// 类型定义

/***************************************************************************************************/
// 全局量引用说明
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
// 函数发布
GAME_EXT t_err SysGameInit(void);

GAME_EXT t_err Game(void);

GAME_EXT t_err GameTest(void);

GAME_EXT void ColorSensorEvent(T_IOEvent event);


/* 设置进入新的游戏状态 */
#define GameStateSet(s) 	GAME_OS_ENTER_CRITICAL(); \
							g_GameState = s; \
							GAME_OS_EXIT_CRITICAL()
#define GameStateSetQ(s)	g_GameState = s
#define GameStateGet()		g_GameState
#define GameStateRead(r)	GAME_OS_ENTER_CRITICAL(); \
							r = g_GameState; \
							GAME_OS_EXIT_CRITICAL()





#endif	// end: #define __GAME_H__ //
