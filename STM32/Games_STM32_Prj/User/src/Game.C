/*******************************************************************************
  Copyright (C), 2009-2011, YingHong, Ltd.
  File name:   	Game.C
  Author:      	Xsky
  Version:     	1.0
  Date:		2011-02-25 09:07
  
  Description:   Game��ض��弰����
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
/* ��Ϸ���̳��� */
#define  TIME_ColorTestWait_ms			(3*60*1000) /* �ȴ���ɫ������ʶ��ʱ�� */

#define  TIME_InisdeLoopMs_ms			5000	/* Ѻ��֮ǰ��Ȧѭ��ʱ�� */
#define  TIME_OutsideLoop1TOut_ms		30000	/* ��Ȧ��һȦ��ʱʱ�� */
#define  TIME_OutSideLoop2TOut_ms		30000	/* ��Ȧ�ڶ�Ȧ��ʱʱ�� */
#define  TIME_Outside2InsideDly_ms		1000	/*  �л��ӳ� */

#define  TIME_InsideBallWait_ms			20000	/* ����Ȧѭ��ʱ�ȴ��� */
#define  TIME_InsideBallWaitNext_ms		80		/* �ȴ���һ����ĳ�ʱʱ�� */
//#define  TIME_InsideBallWaitPer_ms		2000

#define  TIME_Wait2Inside_ms			10000	/* �ȴ�������Ȧ������Ȧ */

#define  TIME_BroadcastOpenTOut_ms		300		/* ���ͽ����ʱʱ��/δ�յ����зֻ��ظ� */
#define  TIME_Game777_ms				10000	/* 777��Ϸʱ�� */

#define  TIME_MOTOR_RunDelayTick		(OS_TICKS_PER_SEC/20)		/* ����˶�֮�����ʱ */

/***************************************************************************************************/
// ��������

// Ѻ�ֱ��ʱ���ֵ
//const uint8 c_WagerRatesTbl[WAGER_RATE_NUM] = {WAGER_RATE_1, WAGER_RATE_2, WAGER_RATE_3};

// ����Ϊ����ʱ�������·�

// ���Ѻ��������ݶ���, ע���ȫ�ֱ���ֻ����main().Game()/Ѻ�ִ����ж���ʹ��
// ������Ҫ����Communi.h�е�WAGER_ENTER_CRITICAL()��������, �Է�ֹ������������
//T_PlayerWager g_PlayerWager[MAX_PLAYER] = {0, };
// ����ע����Ѻ��
uint32 	g_TotalBets[BET_ITEMS] = {0, };
// ����ע���Ӧ��Ӯ��
//uint32	g_TotalWagersWin[BET_ITEMS] = {0, };
// ����, ע�������Ϊ����Ĳ���ֵ����102.4, ����ֵΪʵ�ʱ���x10, ��ʼ��Ѻ��֮��Ϊʵ��ֵ��1024��
// ���㷨�µ��������ֵ:419.47=0xFFFFFFFF/9999(���Ѻ��)/1024(���ʳ�������)
uint32	g_Odds[BET_ITEMS] = {0, };
// ���Ѻ��
uint16  g_MaxBet[BET_ITEMS] = {0, };

typedef uint8 t_gameState;
// ȫ����Ϸ״̬
t_gameState	g_GameState = GAME_STATE_Init;
// ��Ϸ����ʱ
// static uint8	g_GameTimerSec = 0;
// ����״̬
//static uint16	g_ErrorFlag = 0;

static t_color FrontBallColors[BALL_WAGER_NUM];

static t_color *pGetColors = NULL;
static uint8   GetColorN = 0;

static uint8 InsideBallN = 0;

/***************************************************************************************************/
// ����
#ifdef DEBUG
//#define  DBG_COLOR
#endif


// ����⵽��ʱ�����ƿ���/�رմ����������
void ColorSensorEvent(T_IOEvent event)
{
}

/*********************************************************************************************************
** ��������: BallGetColors
** ��������: ѭ���ȴ���ʽʶ��ָ����Ŀ������ɫ
** �䡡��: 1. N: Ŀ��ʶ������
**		 2. Colors[]: ʶ��֮�����ɫֵ����
** �䡡��: ��
** ������: t_err: ȫ�ִ���״̬, ERR_xxx
**		ERR_NONE: �ɹ��ڳ�ʱʱ����ȡ��ָ����Ŀ����ɫ
**		ERR_InsideWaitBallTimeOut: �ȴ���ʱ
**		ERR_UndefColor: ʶ������һ��δ֪��ɫ
**		ERR_InsideLoopSwitchErr: ��Ȧֹͣ�������ش���
** ȫ�ֱ���: ��
** ����ģ��: 
**			SetTimerFlg_ms(), CHK_InputState(), isNTimeOut_ms()
**			CHK_IN_CenterSensor(), 
** �桡��: v0.0.01
** ������: Xsky
** �ա���: 2010-03-17 11:50
** ����ע: 
**-------------------------------------------------------------------------------------------------------
** �桡��: 
** �޸���: 
** �ա���: 
** �衡��: 
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
	// ������Ȧѭ��
	// 

	return ERR_NONE;
}

/*********************************************************************************************************
** ��������: s_BallCheck
** ��������: ��Ϸ�����̣�ѭ���ȴ���ʽ 
** �䡡��: ��
** �䡡��: ��
** ������: t_err: ȫ�ִ���״̬, ERR_xxx
**		ERR_NONE: �ɹ��ڳ�ʱʱ����ȡ��ָ����Ŀ����ɫ
**		ERR_InsideWaitBallTimeOut: �ȴ���ʱ
**		ERR_UndefColor: ʶ������һ��δ֪��ɫ
**		ERR_InsideLoopSwitchErr: ��Ȧֹͣ�������ش���
** ȫ�ֱ���: ��
** ����ģ��: BallGetColors()
** �桡��: v0.00.1
** ������: Xsky
** �ա���: 2011-06-15 16:50
** ����ע: 
**-------------------------------------------------------------------------------------------------------
** �桡��: 
** �޸���: 
** �ա���: 
** �衡��: 
********************************************************************************************************/
static t_err s_BallCheck(void *pdata)
{
	t_err err;
	
	return err;
}

/*********************************************************************************************************
** ��������: s_WagerInit
** ��������: ��ʼ��Ѻ�����ݣ���һ��״̬����������Ѻ�ֿ���Ѻ��״̬ 
** �䡡��: ��
** �䡡��: ��
** ������: t_err: ȫ�ִ���״̬, ERR_xxx
**		 ERR_NONE: �޴���
** ȫ�ֱ���: g_Param
** ����ģ��: 
** �桡��: v0.00.1
** ������: Xsky
** �ա���: 2011-06-15 08:41
** ����ע: 
**-------------------------------------------------------------------------------------------------------
** �桡��: 
** �޸���: 
** �ա���: 
** �衡��: 
********************************************************************************************************/
static t_err s_WagerInit(void *pdata)
{
	
	return ERR_NONE;
}

/*********************************************************************************************************
** ��������: s_Wager
** ��������: Ѻ�ֹ��̣�ѭ���ȴ���ʽ 
** �䡡��: ��
** �䡡��: ��
** ������: t_err: ȫ�ִ���״̬, ERR_xxx
**		 ERR_NONE: �޴���
** ȫ�ֱ���: g_Param
** ����ģ��: 
** �桡��: v0.00.1
** ������: Xsky
** �ա���: 2011-06-15 08:41
** ����ע: 
**-------------------------------------------------------------------------------------------------------
** �桡��: 
** �޸���: 
** �ա���: 
** �衡��: 
********************************************************************************************************/
static t_err s_Wager(void *pdata)
{	
	return ERR_NONE;
}

/*********************************************************************************************************
** ��������: s_Inside
** ��������: ��Ϸ�����̣�ѭ���ȴ���ʽ 
** �䡡��: ��
** �䡡��: ��
** ������: t_err: ȫ�ִ���״̬, ERR_xxx
**		 ERR_NONE: �޴���
** ȫ�ֱ���: 
** ����ģ��: 
** �桡��: v0.00.1
** ������: Xsky
** �ա���: 2011-06-15 08:41
** ����ע: 
**-------------------------------------------------------------------------------------------------------
** �桡��: 
** �޸���: 
** �ա���: 
** �衡��: 
********************************************************************************************************/
static t_err s_InsideLoop(void *pdata)
{

	return ERR_NONE;
}

// s_WaitBallIn:�ȴ���ȫ��������Ȧ:���ڲ��Թ���
static t_err s_WaitBallIn(void *pdata)
{
	t_err err = ERR_InsideWaitBallTimeOut;
	
	
	return err;
}

// ���������л�����Ȧ,���ڲ��Թ���
static t_err s_Wait2Inside(void *pdata)
{
	t_err err = ERR_NONE;
	
	return err;
}


/*********************************************************************************************************
** ��������: s_Wager
** ��������: ��Ϸ�����̣�ѭ���ȴ���ʽ����һȦѭ��
** �䡡��: ��
** �䡡��: ��
** ������: t_err: ȫ�ִ���״̬, ERR_xxx
**		 ERR_NONE: �޴���
**		 ERR_OutsideLoop1Err: ��Ȧѭ����һȦ����
**		 ERR_TimerUndef: ��ʱ��δ�������
** ȫ�ֱ���: 
** ����ģ��: 
** �桡��: v0.00.1
** ������: Xsky
** �ա���: 2011-06-15 08:41
** ����ע: 
**-------------------------------------------------------------------------------------------------------
** �桡��: 
** �޸���: 
** �ա���: 
** �衡��: 
********************************************************************************************************/
static t_err s_OutsideLoop1(void *pdata)
{
	t_timeTick_ms	nTimer;
	t_err err = ERR_OutsideLoop1Err;
	
	return err;
}

/*********************************************************************************************************
** ��������: s_OutsideLoop2
** ��������: ��Ϸ�����̣�ѭ���ȴ���ʽ����һȦ���
** �䡡��: ��
** �䡡��: ��
** ������: t_err: ȫ�ִ���״̬, ERR_xxx
**		 ERR_NONE: �޴���
**		 ERR_OutsideLoop2Err
**		 ERR_TimerUndef: ��ʱ��δ�������
** ȫ�ֱ���: 
** ����ģ��: 
** �桡��: v0.00.1
** ������: Xsky
** �ա���: 2011-06-15 08:41
** ����ע: 
**-------------------------------------------------------------------------------------------------------
** �桡��: 
** �޸���: 
** �ա���: 
** �衡��: 
********************************************************************************************************/
static t_err s_OutsideLoop2(void *pdata)
{
	//t_timeTick_ms	nTimer;
	t_err err = ERR_NONE;

	_DbgGameState_(( "\r\n[s_OutsdLp2]" ));
		
	return err;
}

/*********************************************************************************************************
** ��������: s_BallRecognise
** ��������: ��Ϸ�����̣�ѭ���ȴ���ʽ����Ϸ������ʶ���� 
** �䡡��: ��
** �䡡��: ��
** ������: t_err: ȫ�ִ���״̬, ERR_xxx
**		ERR_NONE: �ɹ��ڳ�ʱʱ����ȡ��ָ����Ŀ����ɫ
**		ERR_InsideWaitBallTimeOut: �ȴ���ʱ
**		ERR_UndefColor: ʶ������һ��δ֪��ɫ
**		ERR_InsideLoopSwitchErr: ��Ȧֹͣ�������ش���
** ȫ�ֱ���: FrontBallColors
** ����ģ��: BallGetColors()
** �桡��: v0.00.1
** ������: Xsky
** �ա���: 2011-06-15 17:01
** ����ע: 
**-------------------------------------------------------------------------------------------------------
** �桡��: 
** �޸���: 
** �ա���: 
** �衡��: 
********************************************************************************************************/
static t_err s_BallRecognise(void *pdata)
{	
	t_err err = ERR_NONE;

	return err;
}

/*********************************************************************************************************
** ��������: s_Open
** ��������: ��Ϸ�����̣�ѭ���ȴ���ʽ����ʾ���
** �䡡��: ��
** �䡡��: ��
** ������: t_err: ȫ�ִ���״̬, ERR_xxx
**		 ERR_NONE: �޴���
** ȫ�ֱ���: 
** ����ģ��: 
** �桡��: v0.00.1
** ������: Xsky
** �ա���: 2011-06-15 08:41
** ����ע: 
**-------------------------------------------------------------------------------------------------------
** �桡��: 
** �޸���: 
** �ա���: 
** �衡��: 
********************************************************************************************************/
static t_err s_Open(void *pdata)
{
	t_timeTick_ms	nTimer;
	//t_timeTick_ms	nWaitMs;
	t_err err;	
	
	return err;
}

/*********************************************************************************************************
** ��������: s_GameLuck
** ��������: ��Ϸ�����̣�ѭ���ȴ���ʽ��777��Ϸ
** �䡡��: ��
** �䡡��: ��
** ������: t_err: ȫ�ִ���״̬, ERR_xxx
**		 ERR_NONE: �޴���
** ȫ�ֱ���: 
** ����ģ��: 
** �桡��: v0.00.1
** ������: Xsky
** �ա���: 2011-06-15 08:41
** ����ע: 
**-------------------------------------------------------------------------------------------------------
** �桡��: 
** �޸���: 
** �ա���: 
** �衡��: 
********************************************************************************************************/
static t_err s_GameLuck(void *pdata)
{
	t_timeTick_ms	nTimer;
	
	return ERR_NONE;
}

/*********************************************************************************************************
** ��������: IntoNextState
** ��������: ��Ϸѭ��������һ��״̬ 
** �䡡��: ��
** �䡡��: ��
** ������: ��
** ȫ�ֱ���: g_GameState, 
** ����ģ��: 
** �桡��: v0.00.1
** ������: Xsky
** �ա���: 2011-06-15 08:41
** ����ע: 
**-------------------------------------------------------------------------------------------------------
** �桡��: 
** �޸���: 
** �ա���: 
** �衡��: 
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
** ��������: IsNextStateEnableExit
** ��������: �����Ϸ״̬ȷ���Ƿ�����˳�, ���
** �䡡��: ��
** �䡡��: ��
** ������: ��
** ȫ�ֱ���: g_GameState, 
** ����ģ��: 
** �桡��: v0.00.1
** ������: Xsky
** �ա���: 2011-06-15 08:41
** ����ע: 
**-------------------------------------------------------------------------------------------------------
** �桡��: 
** �޸���: 
** �ա���: 
** �衡��: 
********************************************************************************************************/
static t_bool IsNextStateEnableExit(void)
{
	register t_gameState s;
	
	GameStateRead(s);
	if(s <= GAME_STATE_EnExitNext)		/* ��Ϸ״̬��Ѻ��֮ǰ�����Ա��жϣ�����Ѻ��״̬֮���ܱ��ж� */
	{
		return TRUE;
	}
	
	return FALSE;
}

/*********************************************************************************************************
 ��������״ִ̬�еĺ����� 
****************************************************************************************************/
typedef t_err t_pfGameActoin(void *pdata) ;

typedef struct t_gameRunCtrl_
{
	void *pStateActions;
	void *pStateActionsParam;
	t_ureg_opt nActions;
	
}t_gameRunCtrl;

/*********************************************************************************************************
** ��������: GameRun
** ��������: ���ղ�����ָ����˳������ָ������
**		  ע��: ���ִ�еĹ��̷��ش������˳�ִ�й���
** �䡡��: StateActions: ִ�еĹ��̱�, ִ�й��̰��ոò����ڵĺ���˳��ִ��
**		 StateActionsParam: ִ�еĹ��̴��������
**		 nActions: Ҫִ�еĶ�������
** �䡡��: ��
** ������: ��
** ȫ�ֱ���: g_GameState, 
** ����ģ��: 
** �桡��: v0.00.1
** ������: Xsky
** �ա���: 2011-06-15 08:41
** ����ע: 
**-------------------------------------------------------------------------------------------------------
** �桡��: 
** �޸���: 
** �ա���: 
** �衡��: 
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
		//err= ERR_NONE;			 // ��״̬���������Լ�����д���̬
		GameStateRead(r);
		_DbgGameState_(( "\r\n[��GmSt:%d]", r ));
		_DbgGameStateLcd_(( 1, 0, "GmSt:%d", r ));
		
		if(r < nActions)
		{
			err = ERR_NONE;
			p = (void *)StateActions[r];
			if(p != NULL)
			{
				// ִ�и�״̬����
				err = ((t_pfGameActoin *)p)(StateActionsParam[r]);
			}
		}
		else
		{
			// �������˳���Ϸ
			if(err != ERR_NONE)
			{
				/* ����������,����Ϊ����δ֪��Ϸ״̬���� */
				err = ERR_GameStateUndef;
			}
			goto lbErrExit;
		}
		
		if(err == ERR_NONE)
		{
			IntoNextState();
			if(GameStateGet() == GAME_STATE_Error)
			{
				// �µ���Ϸ״̬����
				err = ERR_GameStateUndef;
				goto lbErrExit;
			}
			
			if (SysIsReqNextState())			/* ����Ƿ�����״̬���� */
			if (IsNextStateEnableExit())		/* ��鵱ǰ״̬�Ƿ�ɱ���� */
			{
				// �����˳���Ϸ����
				goto lbNormalExit;
			}
		}
		else if(err != ERR_NONE)
		{
			// �������˳���Ϸ
			goto lbErrExit;
		}
	}
	
lbErrExit:	/* �д����˳������� */
	
	_DbgGameState_(( "[��GmErr:%X]", err ));
	
	return err;
	
lbNormalExit:

	return ERR_NONE;
	
}

/*********************************************************************************************************
** ��������: GameTest
** ��������: ������Ϸ���̣��Լ�⴫������
** �䡡��: ��
** �䡡��: ��
** ������: ��
** ȫ�ֱ���: g_GameState, 
** ����ģ��: 
** �桡��: v0.00.1
** ������: Xsky
** �ա���: 2011-06-15 08:41
** ����ע: 
**-------------------------------------------------------------------------------------------------------
** �桡��: 
** �޸���: 
** �ա���: 
** �衡��: 
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
** ��������: SysGameInit
** ��������: ��ʼ�����Ѻ����ر���, ��ʼ���������״̬, �������������в���
** �䡡��: ��
** �䡡��: ��
** ������: ��
** ȫ�ֱ���: g_PlayerWager
** ����ģ��: 
** �桡��: v0.00.1
** ������: Xsky
** �ա���: 2011-06-13 16:00
** ����ע: 
**-------------------------------------------------------------------------------------------------------
** �桡��: 
** �޸���: 
** �ա���: 
** �衡��: 
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
	
	/* Ѻ�����ݳ�ʼ�� */
	for(i = 0; i < MAX_PLAYER; i++)
	{
		// ���������ѳ�ʼ��Ϊ0
		
	}
	#define _DBG_MTR_(exp)	//_DBG_(exp)

	
	
	// ����,��������
	return GameTest();
}



/*********************************************************************************************************
** ��������: Game
** ��������: ��Ϸ�����̣�ѭ���ȴ���ʽ 
** �䡡��: ��
** �䡡��: ��
** ������: ��
** ȫ�ֱ���: g_GameState, 
** ����ģ��: 
** �桡��: v0.00.1
** ������: Xsky
** �ա���: 2011-06-15 08:41
** ����ע: 
**-------------------------------------------------------------------------------------------------------
** �桡��: 
** �޸���: 
** �ա���: 
** �衡��: 
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
		// ���ý�����һ����Ϸ״̬
		//GameStateSet(GAME_STATE_BallRecognise);
		break;
	case GAME_STATE_BallCheck:
		err = s_BallGetColors(BallColors, BALL_NUM);
		/*
		if(err == ERR_NONE)
		{
			// �޴��������һ״̬
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
		// �������˳���Ϸ
		if(err != ERR_NONE)
		{
			err = ERR_GameStateUndef;
		}
		goto lbErrExit;
		break;
}/* end: switch (GameStateGet()) */
#endif

