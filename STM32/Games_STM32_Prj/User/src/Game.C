/*******************************************************************************
  Copyright (C), 2009-2011, YingHong, Ltd.
  File name:   	Game.C
  Author:      	Xsky
  Version:     	1.0
  Date:		2011-02-25 09:07
  
  Description:   Game相关定义及函数
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
#define GAME_GLOBALS

#include "Config.H"
//#include "core_cm3.h"
/*
#include "Global.h"
#include "Stm32f10x.h"
#include "GameParam.h"
#include "Game.h"
#include "Target.h"
#include "ColorSensor.h"
#include "IOInput.h"
#include "Delay.h"
#include "StateCtrl.h"
*/
#define  _DbgGameState_		_DbgPrintf_
#define  _DbgGameStateLcd_	_DbgLcdPrintf_

/***************************************************************************************************/
/* 游戏过程常数 */
#define  TIME_ColorTestWait_ms			(3*60*1000) /* 等待颜色传感器识别时间 */

#define  TIME_InisdeLoopMs_ms			5000	/* 押分之前内圈循环时间 */
#define  TIME_OutsideLoop1TOut_ms		30000	/* 外圈第一圈超时时间 */
#define  TIME_OutSideLoop2TOut_ms		30000	/* 外圈第二圈超时时间 */
#define  TIME_Outside2InsideDly_ms		1000	/*  切换延迟 */

#define  TIME_InsideBallWait_ms			20000	/* 在内圈循环时等待球 */
#define  TIME_InsideBallWaitNext_ms		80		/* 等待下一个球的超时时间 */
//#define  TIME_InsideBallWaitPer_ms		2000

#define  TIME_Wait2Inside_ms			10000	/* 等待球由外圈进入内圈 */

#define  TIME_BroadcastOpenTOut_ms		300		/* 发送结果超时时间/未收到所有分机回复 */
#define  TIME_Game777_ms				10000	/* 777游戏时间 */

#define  TIME_MOTOR_RunDelayTick		(OS_TICKS_PER_SEC/20)		/* 电机运动之后的延时 */

/***************************************************************************************************/
// 变量定义

// 押分倍率倍数值
//const uint8 c_WagerRatesTbl[WAGER_RATE_NUM] = {WAGER_RATE_1, WAGER_RATE_2, WAGER_RATE_3};

// 参数为启动时由主机下发

// 玩家押分相关数据定义, 注意该全局变量只能在main().Game()/押分处理中断中使用
// 否则需要更改Communi.h中的WAGER_ENTER_CRITICAL()函数定义, 以防止中重入性问题
//T_PlayerWager g_PlayerWager[MAX_PLAYER] = {0, };
// 各项注码总押分
uint32 	g_TotalBets[BET_ITEMS] = {0, };
// 各项注码对应的赢分
//uint32	g_TotalWagersWin[BET_ITEMS] = {0, };
// 赔率, 注意该赔率为保存的参数值乘以102.4, 参数值为实际倍率x10, 初始化押分之后为实际值的1024倍
// 该算法下的赔率最大值:419.47=0xFFFFFFFF/9999(最大押分)/1024(倍率乘数因子)
uint32	g_Odds[BET_ITEMS] = {0, };
// 最大押分
uint16  g_MaxBet[BET_ITEMS] = {0, };

typedef uint8 t_gameState;
// 全局游戏状态
t_gameState	g_GameState = GAME_STATE_Init;
// 游戏倒计时
// static uint8	g_GameTimerSec = 0;
// 错误状态
//static uint16	g_ErrorFlag = 0;

static t_color FrontBallColors[BALL_WAGER_NUM];

static t_color *pGetColors = NULL;
static uint8   GetColorN = 0;

static uint8 InsideBallN = 0;

/***************************************************************************************************/
// 正文
#ifdef DEBUG
//#define  DBG_COLOR
#endif


// 当检测到球时，控制开启/关闭传感器补光灯
void ColorSensorEvent(T_IOEvent event)
{
}

/*********************************************************************************************************
** 函数名称: BallGetColors
** 功能描述: 循环等待方式识别指定数目的球颜色
** 输　入: 1. N: 目标识别数量
**		 2. Colors[]: 识别之后的颜色值数组
** 输　出: 无
** 返　回: t_err: 全局错误状态, ERR_xxx
**		ERR_NONE: 成功在超时时间内取得指定数目的颜色
**		ERR_InsideWaitBallTimeOut: 等待球超时
**		ERR_UndefColor: 识别到至少一个未知颜色
**		ERR_InsideLoopSwitchErr: 内圈停止启动开关错误
** 全局变量: 无
** 调用模块: 
**			SetTimerFlg_ms(), CHK_InputState(), isNTimeOut_ms()
**			CHK_IN_CenterSensor(), 
** 版　本: v0.0.01
** 作　者: Xsky
** 日　期: 2010-03-17 11:50
** 备　注: 
**-------------------------------------------------------------------------------------------------------
** 版　本: 
** 修改人: 
** 日　期: 
** 描　述: 
********************************************************************************************************/
static t_err BallGetColors(t_color Colors[], t_ureg_opt N)
{
	register t_color 	color;

	return color;
}

static t_err s_ColorSensorTest(void *pdata)
{
	t_err err;
	
	return err;
}

static t_err s_GameStart(void *pdata)
{
	// 启动内圈循环
	// 

	return ERR_NONE;
}

/*********************************************************************************************************
** 函数名称: s_BallCheck
** 功能描述: 游戏主过程，循环等待方式 
** 输　入: 无
** 输　出: 无
** 返　回: t_err: 全局错误状态, ERR_xxx
**		ERR_NONE: 成功在超时时间内取得指定数目的颜色
**		ERR_InsideWaitBallTimeOut: 等待球超时
**		ERR_UndefColor: 识别到至少一个未知颜色
**		ERR_InsideLoopSwitchErr: 内圈停止启动开关错误
** 全局变量: 无
** 调用模块: BallGetColors()
** 版　本: v0.00.1
** 作　者: Xsky
** 日　期: 2011-06-15 16:50
** 备　注: 
**-------------------------------------------------------------------------------------------------------
** 版　本: 
** 修改人: 
** 日　期: 
** 描　述: 
********************************************************************************************************/
static t_err s_BallCheck(void *pdata)
{
	t_err err;
	
	return err;
}

/*********************************************************************************************************
** 函数名称: s_WagerInit
** 功能描述: 初始化押分数据，下一个状态即立即进入押分开局押分状态 
** 输　入: 无
** 输　出: 无
** 返　回: t_err: 全局错误状态, ERR_xxx
**		 ERR_NONE: 无错误
** 全局变量: g_Param
** 调用模块: 
** 版　本: v0.00.1
** 作　者: Xsky
** 日　期: 2011-06-15 08:41
** 备　注: 
**-------------------------------------------------------------------------------------------------------
** 版　本: 
** 修改人: 
** 日　期: 
** 描　述: 
********************************************************************************************************/
static t_err s_WagerInit(void *pdata)
{
	
	return ERR_NONE;
}

/*********************************************************************************************************
** 函数名称: s_Wager
** 功能描述: 押分过程，循环等待方式 
** 输　入: 无
** 输　出: 无
** 返　回: t_err: 全局错误状态, ERR_xxx
**		 ERR_NONE: 无错误
** 全局变量: g_Param
** 调用模块: 
** 版　本: v0.00.1
** 作　者: Xsky
** 日　期: 2011-06-15 08:41
** 备　注: 
**-------------------------------------------------------------------------------------------------------
** 版　本: 
** 修改人: 
** 日　期: 
** 描　述: 
********************************************************************************************************/
static t_err s_Wager(void *pdata)
{	
	return ERR_NONE;
}

/*********************************************************************************************************
** 函数名称: s_Inside
** 功能描述: 游戏主过程，循环等待方式 
** 输　入: 无
** 输　出: 无
** 返　回: t_err: 全局错误状态, ERR_xxx
**		 ERR_NONE: 无错误
** 全局变量: 
** 调用模块: 
** 版　本: v0.00.1
** 作　者: Xsky
** 日　期: 2011-06-15 08:41
** 备　注: 
**-------------------------------------------------------------------------------------------------------
** 版　本: 
** 修改人: 
** 日　期: 
** 描　述: 
********************************************************************************************************/
static t_err s_InsideLoop(void *pdata)
{

	return ERR_NONE;
}

// s_WaitBallIn:等待球全部进入内圈:用于测试过程
static t_err s_WaitBallIn(void *pdata)
{
	t_err err = ERR_InsideWaitBallTimeOut;
	
	
	return err;
}

// 开机测试切换至内圈,用于测试过程
static t_err s_Wait2Inside(void *pdata)
{
	t_err err = ERR_NONE;
	
	return err;
}


/*********************************************************************************************************
** 函数名称: s_Wager
** 功能描述: 游戏主过程，循环等待方式，第一圈循环
** 输　入: 无
** 输　出: 无
** 返　回: t_err: 全局错误状态, ERR_xxx
**		 ERR_NONE: 无错误
**		 ERR_OutsideLoop1Err: 外圈循环第一圈错误
**		 ERR_TimerUndef: 定时器未定义错误
** 全局变量: 
** 调用模块: 
** 版　本: v0.00.1
** 作　者: Xsky
** 日　期: 2011-06-15 08:41
** 备　注: 
**-------------------------------------------------------------------------------------------------------
** 版　本: 
** 修改人: 
** 日　期: 
** 描　述: 
********************************************************************************************************/
static t_err s_OutsideLoop1(void *pdata)
{
	t_timeTick_ms	nTimer;
	t_err err = ERR_OutsideLoop1Err;
	
	return err;
}

/*********************************************************************************************************
** 函数名称: s_OutsideLoop2
** 功能描述: 游戏主过程，循环等待方式，第一圈检测
** 输　入: 无
** 输　出: 无
** 返　回: t_err: 全局错误状态, ERR_xxx
**		 ERR_NONE: 无错误
**		 ERR_OutsideLoop2Err
**		 ERR_TimerUndef: 定时器未定义错误
** 全局变量: 
** 调用模块: 
** 版　本: v0.00.1
** 作　者: Xsky
** 日　期: 2011-06-15 08:41
** 备　注: 
**-------------------------------------------------------------------------------------------------------
** 版　本: 
** 修改人: 
** 日　期: 
** 描　述: 
********************************************************************************************************/
static t_err s_OutsideLoop2(void *pdata)
{
	//t_timeTick_ms	nTimer;
	t_err err = ERR_NONE;

	_DbgGameState_(( "\r\n[s_OutsdLp2]" ));
		
	return err;
}

/*********************************************************************************************************
** 函数名称: s_BallRecognise
** 功能描述: 游戏主过程，循环等待方式，游戏结束，识别球 
** 输　入: 无
** 输　出: 无
** 返　回: t_err: 全局错误状态, ERR_xxx
**		ERR_NONE: 成功在超时时间内取得指定数目的颜色
**		ERR_InsideWaitBallTimeOut: 等待球超时
**		ERR_UndefColor: 识别到至少一个未知颜色
**		ERR_InsideLoopSwitchErr: 内圈停止启动开关错误
** 全局变量: FrontBallColors
** 调用模块: BallGetColors()
** 版　本: v0.00.1
** 作　者: Xsky
** 日　期: 2011-06-15 17:01
** 备　注: 
**-------------------------------------------------------------------------------------------------------
** 版　本: 
** 修改人: 
** 日　期: 
** 描　述: 
********************************************************************************************************/
static t_err s_BallRecognise(void *pdata)
{	
	t_err err = ERR_NONE;

	return err;
}

/*********************************************************************************************************
** 函数名称: s_Open
** 功能描述: 游戏主过程，循环等待方式，显示结果
** 输　入: 无
** 输　出: 无
** 返　回: t_err: 全局错误状态, ERR_xxx
**		 ERR_NONE: 无错误
** 全局变量: 
** 调用模块: 
** 版　本: v0.00.1
** 作　者: Xsky
** 日　期: 2011-06-15 08:41
** 备　注: 
**-------------------------------------------------------------------------------------------------------
** 版　本: 
** 修改人: 
** 日　期: 
** 描　述: 
********************************************************************************************************/
static t_err s_Open(void *pdata)
{
	t_timeTick_ms	nTimer;
	//t_timeTick_ms	nWaitMs;
	t_err err;	
	
	return err;
}

/*********************************************************************************************************
** 函数名称: s_GameLuck
** 功能描述: 游戏主过程，循环等待方式，777游戏
** 输　入: 无
** 输　出: 无
** 返　回: t_err: 全局错误状态, ERR_xxx
**		 ERR_NONE: 无错误
** 全局变量: 
** 调用模块: 
** 版　本: v0.00.1
** 作　者: Xsky
** 日　期: 2011-06-15 08:41
** 备　注: 
**-------------------------------------------------------------------------------------------------------
** 版　本: 
** 修改人: 
** 日　期: 
** 描　述: 
********************************************************************************************************/
static t_err s_GameLuck(void *pdata)
{
	t_timeTick_ms	nTimer;
	
	return ERR_NONE;
}

/*********************************************************************************************************
** 函数名称: IntoNextState
** 功能描述: 游戏循环进入下一个状态 
** 输　入: 无
** 输　出: 无
** 返　回: 无
** 全局变量: g_GameState, 
** 调用模块: 
** 版　本: v0.00.1
** 作　者: Xsky
** 日　期: 2011-06-15 08:41
** 备　注: 
**-------------------------------------------------------------------------------------------------------
** 版　本: 
** 修改人: 
** 日　期: 
** 描　述: 
********************************************************************************************************/
static t_err IntoNextState(void)
{
	register t_gameState s;
	
	GAME_OS_ENTER_CRITICAL();
	
	s = GameStateGet();
	if(s > GAME_STATE_Top)
	{
		s = GAME_STATE_Error;
	}
	if(s == GAME_STATE_Top)
	{
		s = GAME_STATE_ReStart;
	}else{
		s++;
	}
	
	GameStateSetQ(s);
	
	GAME_OS_EXIT_CRITICAL();
	
	return s;
}

/*********************************************************************************************************
** 函数名称: IsNextStateEnableExit
** 功能描述: 检查游戏状态确定是否可以退出, 检查
** 输　入: 无
** 输　出: 无
** 返　回: 无
** 全局变量: g_GameState, 
** 调用模块: 
** 版　本: v0.00.1
** 作　者: Xsky
** 日　期: 2011-06-15 08:41
** 备　注: 
**-------------------------------------------------------------------------------------------------------
** 版　本: 
** 修改人: 
** 日　期: 
** 描　述: 
********************************************************************************************************/
static t_bool IsNextStateEnableExit(void)
{
	register t_gameState s;
	
	GameStateRead(s);
	if(s <= GAME_STATE_EnExitNext)		/* 游戏状态在押分之前，可以被中断，进入押分状态之后不能被中断 */
	{
		return TRUE;
	}
	
	return FALSE;
}

/*********************************************************************************************************
 定义进入各状态执行的函数表 
****************************************************************************************************/
typedef t_err t_pfGameActoin(void *pdata) ;

typedef struct t_gameRunCtrl_
{
	void *pStateActions;
	void *pStateActionsParam;
	t_ureg_opt nActions;
	
}t_gameRunCtrl;

/*********************************************************************************************************
** 函数名称: GameRun
** 功能描述: 按照参数表指定的顺序运行指定过程
**		  注意: 如果执行的过程返回错误则退出执行过程
** 输　入: StateActions: 执行的过程表, 执行过程按照该参数内的函数顺序执行
**		 StateActionsParam: 执行的过程传入参数表
**		 nActions: 要执行的动作个数
** 输　出: 无
** 返　回: 无
** 全局变量: g_GameState, 
** 调用模块: 
** 版　本: v0.00.1
** 作　者: Xsky
** 日　期: 2011-06-15 08:41
** 备　注: 
**-------------------------------------------------------------------------------------------------------
** 版　本: 
** 修改人: 
** 日　期: 
** 描　述: 
********************************************************************************************************/
t_err GameRun(void *StateActions[], void *StateActionsParam[], t_ureg_opt nActions)
{
	t_err	err = ERR_NONE;
	register uint32 r;
	register void *p;
	
	err = ERR_NONE;
	
//	GameStateSet(GAME_STATE_Init);
	while(1)
	{
		//err= ERR_NONE;			 // 此状态不能添加以检测运行错误态
		GameStateRead(r);
		_DbgGameState_(( "\r\n[◆GmSt:%d]", r ));
		_DbgGameStateLcd_(( 1, 0, "GmSt:%d", r ));
		
		if(r < nActions)
		{
			err = ERR_NONE;
			p = (void *)StateActions[r];
			if(p != NULL)
			{
				// 执行各状态函数
				err = ((t_pfGameActoin *)p)(StateActionsParam[r]);
			}
		}
		else
		{
			// 错误则退出游戏
			if(err != ERR_NONE)
			{
				/* 非其它错误,设置为进入未知游戏状态错误 */
				err = ERR_GameStateUndef;
			}
			goto lbErrExit;
		}
		
		if(err == ERR_NONE)
		{
			IntoNextState();
			if(GameStateGet() == GAME_STATE_Error)
			{
				// 新的游戏状态错误
				err = ERR_GameStateUndef;
				goto lbErrExit;
			}
			
			if (SysIsReqNextState())			/* 检查是否有新状态请求 */
			if (IsNextStateEnableExit())		/* 检查当前状态是否可被打断 */
			{
				// 正常退出游戏过程
				goto lbNormalExit;
			}
		}
		else if(err != ERR_NONE)
		{
			// 错误则退出游戏
			goto lbErrExit;
		}
	}
	
lbErrExit:	/* 有错误退出本函数 */
	
	_DbgGameState_(( "[◆GmErr:%X]", err ));
	
	return err;
	
lbNormalExit:

	return ERR_NONE;
	
}

/*********************************************************************************************************
** 函数名称: GameTest
** 功能描述: 测试游戏过程，以检测传感器等
** 输　入: 无
** 输　出: 无
** 返　回: 无
** 全局变量: g_GameState, 
** 调用模块: 
** 版　本: v0.00.1
** 作　者: Xsky
** 日　期: 2011-06-15 08:41
** 备　注: 
**-------------------------------------------------------------------------------------------------------
** 版　本: 
** 修改人: 
** 日　期: 
** 描　述: 
********************************************************************************************************/
t_err GameTest(void)
{
	static const void *tst_GameStateActions[] = 
				{s_ColorSensorTest,  s_Wait2Inside,  s_OutsideLoop1,  s_OutsideLoop2, 
				/* 0			1				2					3	*/
				s_WaitBallIn, s_BallCheck, s_Wait2Inside
				/* 4			5				6				7	*/
				/* 8			9				10				11	*/
				};
	static void *tst_GameStateActionsParam[lenof(tst_GameStateActions)] = 
				{NULL, NULL/* ReStart */,	NULL/* OutsideLoop1 */, NULL/*OutsideLoop2*/, 
				/* 0			1				2					3	*/
				NULL/*InsideLoop*/, NULL/*BallCheck*/, NULL
				/* 4			5				6				7	*/
				/* 8			9				10				11	*/
				};
	
	_DbgPrintf_(( "GmTst:" ));
	_DBG_( LCD_SdStringPos("GmTst:", 1, 0) );
	
	return GameRun((void *)tst_GameStateActions, tst_GameStateActionsParam, lenof(tst_GameStateActions));
	
}

/*********************************************************************************************************
** 函数名称: SysGameInit
** 功能描述: 初始化玩家押分相关变量, 初始化电机开关状态, 并进行启动运行测试
** 输　入: 无
** 输　出: 无
** 返　回: 无
** 全局变量: g_PlayerWager
** 调用模块: 
** 版　本: v0.00.1
** 作　者: Xsky
** 日　期: 2011-06-13 16:00
** 备　注: 
**-------------------------------------------------------------------------------------------------------
** 版　本: 
** 修改人: 
** 日　期: 
** 描　述: 
********************************************************************************************************/
t_err SysGameInit(void)
{
	register t_ureg_opt i;
	
	#ifdef DEBUG_PIO
	
	volatile uint8 *pIO;
	volatile uint8 *pIO_C;
	
	/*
	pIO = (volatile uint8 *)0x60C00000;
	for(i = 0; i< 16; i++)
	{
		pIO = (volatile uint8 *)((uint32)0x60C00000 + (i << 16)) ;
		*pIO = 0xFF;
	}
	pIO = (volatile uint8 *)0x60C00000;
	for(i = 0; i< 16; i++)
	{
		pIO = (volatile uint8 *)((uint32)0x60C00000 + (i << 16)) ;
		*pIO = 0x00;
	}
	*/
	
	pIO = &X_IO_OUT3;
	pIO_C = &X_IO_LCDCtrl;
	
	*pIO = 0xFF;
	Delay_Nus(1);
	*pIO = 0x00;
	Delay_Nus(1);
	
	*pIO_C = 0xFF;
	Delay_Nus(1);
	*pIO_C = 0x00;
	Delay_Nus(2);
	
	*pIO = 0xFF;
	Delay_Nus(3);
	*pIO = 0x00;
	Delay_Nus(1);
	
	*pIO_C = 0xFF;
	Delay_Nus(1);
	*pIO_C = 0x00;
	Delay_Nus(4);
	
	*pIO = 0xFF;
	Delay_Nus(1);
	*pIO = 0x00;
	Delay_Nus(1);
	
	*pIO_C = 0xFF;
	Delay_Nus(1);
	*pIO_C = 0x00;
	Delay_Nus(2);
	
	*pIO = 0xFF;
	Delay_Nus(3);
	*pIO = 0x00;
	Delay_Nus(1);
	
	*pIO_C = 0xFF;
	Delay_Nus(1);
	*pIO_C = 0x00;
	Delay_Nus(1);
	
	OpenInsideTrack();
	Delay_Nms(1);
	CloseInsideTrack();
	Delay_Nms(1);
	
	#endif	// end: #ifdef DEBUG_XIO //
	
	/* 押分数据初始化 */
	for(i = 0; i < MAX_PLAYER; i++)
	{
		// 其它数据已初始化为0
		
	}
	#define _DBG_MTR_(exp)	//_DBG_(exp)

	
	
	// 测试,启动测试
	return GameTest();
}



/*********************************************************************************************************
** 函数名称: Game
** 功能描述: 游戏主过程，循环等待方式 
** 输　入: 无
** 输　出: 无
** 返　回: 无
** 全局变量: g_GameState, 
** 调用模块: 
** 版　本: v0.00.1
** 作　者: Xsky
** 日　期: 2011-06-15 08:41
** 备　注: 
**-------------------------------------------------------------------------------------------------------
** 版　本: 
** 修改人: 
** 日　期: 
** 描　述: 
********************************************************************************************************/
t_err Game(void)
{
	static const void *GameStateActions[GAME_STATE_Top+1] = 
				{NULL, NULL/* ReStart */,	s_GameStart, 	s_BallCheck, 
				/* 0			1				2					3	*/
				s_WagerInit, s_Wager,		s_InsideLoop,	s_OutsideLoop1, 
				/* 4			5				6				7	*/
				s_OutsideLoop2, s_BallRecognise, s_Open,	s_GameLuck
				/* 8			9				10				11	*/
				/* 12			13				14				15	*/
				};
	static void *GameStateActionsParam[GAME_STATE_Top+1] = 
				{NULL, NULL/* ReStart */,	NULL/* WaitNext */, NULL/*BallCheck*/, 
				/* 0			1				2					3	*/
				NULL/*WagerInit*/, NULL/*Wager*/, NULL/*InsideLoop*/, NULL/*OutsideLoop1*/, 
				/* 4			5				6				7	*/
				NULL/*OutsideLoop2*/, NULL/*BallRecognise*/, NULL/*Open*/,NULL/*Game777*/
				/* 8			9				10				11	*/
				/* 12			13				14				15	*/
				};
	
	return GameRun((void *)GameStateActions, GameStateActionsParam, GAME_STATE_Top+1);
	
	//return ERR_NONE;
}

#if 1 == 0
switch (GameStateGet())
{
	case GAME_STATE_Init:
		// 设置进入下一个游戏状态
		//GameStateSet(GAME_STATE_BallRecognise);
		break;
	case GAME_STATE_BallCheck:
		err = s_BallGetColors(BallColors, BALL_NUM);
		/*
		if(err == ERR_NONE)
		{
			// 无错误进入下一状态
			GameStateSet(GAME_STATE_Wager);
		}*/
		break;
		
	case GAME_STATE_Wager:
		
		break;
	case GAME_STATE_InsideLoop:
		
		break;
	case GAME_STATE_OutsideLoop1:
		
		break;
	case GAME_STATE_OutsideLoop2:
		
		break;
	case GAME_STATE_BallRecognise:
		
		break;
	case GAME_STATE_Open:
		
		break;
	case GAME_STATE_777Game:
		
		break;
	case GAME_STATE_WaitNext:
		
		break;
	default:
		// 错误则退出游戏
		if(err != ERR_NONE)
		{
			err = ERR_GameStateUndef;
		}
		goto lbErrExit;
		break;
}/* end: switch (GameStateGet()) */
#endif


