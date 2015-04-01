/*****************************************************************************************************
  Copyright (C), 2009-2009, YingHong, Ltd.
  File name:   	Communi.C
  Author:      	Xsky
  Version:     	1.0
  Date:		2011-03-16 15:56
  Description:   主分机通信相关函数及全局量，与游戏机制高关联，如押分计算是否可压
  Others:         
  Function List: 
  	1. ：
  	2. ：
  	3. ：
  	4. ：
  	5. ：

  History:        // 修改记录
    1. Date:		2011-03-16 15:56
       Author:		Xsky
       Modification:	建立本模块
**------------------------------------------------------------------------------------------------------
** 最后修改日期: 2011-04-13 14:45
** 修改人: Xsky
** 版本: v1.0.00
**-------------------------------------------------------------------------------------------------------
** 修改人: Xsky
** 日期:  2011-04-13 14:45
** 版本: v1.0.00
** 修改: 
**			1. 修改函数CAN_SendData()
**			2. 
*****************************************************************************************************/


#define COMMUNI_GLOBALS

#include "Config.h"


/*********************************************************************************************************
** 函数名称: CAN_SendBytes
** 功能描述: 将数据设置为欲发送的CAN发送邮箱格式T_CanFrame/CAN_TypeDef
** 			 发送时如果邮箱有空则直接将数据压入邮箱，否则将数据压入队列
**			 如果发送失败次数为3则取消发送, 源MAC为固定值本机MAC
**			 不使用时间触发模式
** 输　入:  1. iPlayer: 目的机台号索引0~11, 为机台号-1
**			2. FrmType: 帧类型(功能码)/见定义
**			3. *pData:  要发送的数据源缓冲区,为提高效率定义为T_CanData直接接32位存取
**			4. nBytes: 要发送的数据字节数,0~8
** 输　出: 无
** 返　回: 错误代码
** 全局变量: 无
** 调用模块: 
** 版　本: v0.0.01
** 作　者: Xsky
** 日　期: 2010-03-16 16:00
** 备　注: 
**-------------------------------------------------------------------------------------------------------
** 版　本: v0.0.02
** 修改人: Xsky
** 日　期: 2011-04-13 14:45
** 描　述: FRM_ID_TYPE_BITnL 改为 (FRM_ID_TYPE_BITnL + FRM_ID_LShitBITn)
********************************************************************************************************/
t_err CAN_SendData(register uint8 iPlayer, register T_FRM_Type FrmType, 
							T_CanData *pData, uint32 nDataBytes)
{
	T_CanFrame CanFrame = {0, };
	
	if(nDataBytes > FRM_MAX_BYTES)
	{
		// 发送的字节数超长
		return ERR_SendToLong;
	}
	_DbgPrintf_(( "CanID:%08X\r\n", CanFrame.IxR >> FRM_ID_LShitBITn ));
	CanFrame.IxR = FRM_CAN_ID_INIT(GET_LOAL_MAC(), FRM_BASE_MAC + (iPlayer+1))
				| (FrmType << (FRM_ID_TYPE_BITnL + FRM_ID_LShitBITn));
	//memcpy(&CanFrame.Data.u08[0], pSrc, nDataBytes);
	CanFrame.DTxR = nDataBytes;				/* 因CAN_TDTxR寄存器的除低四位外其它可设置为0, 所以直接赋值DLC */
	CanFrame.Data.u32[0] = pData->u32[0];
	CanFrame.Data.u32[1] = pData->u32[1];
	
	return CAN_SendFrame(&CanFrame);
}
#ifdef DEBUG
t_err Dbg_CAN_SendData(uint8 SrcMac, register uint8 DstMac, register T_FRM_Type FrmType, 
							T_CanData *pData, uint32 nDataBytes)
{
	T_CanFrame CanFrame = {0, };
	
	if(nDataBytes > FRM_MAX_BYTES)
	{
		// 发送的字节数超长
		return ERR_SendToLong;
	}
	
	CanFrame.IxR = FRM_CAN_ID_INIT(SrcMac, DstMac)
				| (FrmType << (FRM_ID_TYPE_BITnL + FRM_ID_LShitBITn));
	_DbgPrintf_(( "cID:%08X\r\n", CanFrame.IxR >> FRM_ID_LShitBITn ));
	//memcpy(&CanFrame.Data.u08[0], pSrc, nDataBytes);
	CanFrame.DTxR = nDataBytes;				/* 因CAN_TDTxR寄存器的除低四位外其它可设置为0, 所以直接赋值DLC */
	CanFrame.Data.u32[0] = pData->u32[0];
	CanFrame.Data.u32[1] = pData->u32[1];
	
	return CAN_SendFrame(&CanFrame);
}
#endif

/*********************************************************************************************************
** 函数名称: CAN_FilterConfig
** 功能描述: 初始化CAN1 Filter
**			 设置标识符32位屏蔽位模式，对协议中定义的PrioID,SrcMAC,DstMAC
**			 分别进行匹配, 只对TypeID部分可任意匹配
**			 只接收目的MAC为本机MAC,源MAC为1~10号分机对应的0x11~0x1C
**			 且过滤器号偶数/奇数分别与FIFO0/FIFO1对应
**		   注: 接收的DstMAC为固定值在本.h文件中定义
** 输　入: 
** 输　出: 无
** 返　回: 错误代码
** 全局变量: 无
** 调用模块: 
** 版　本: v0.00.1
** 作　者: Xsky
** 日　期: 2010-03-17 11:50
** 备　注: 
**-------------------------------------------------------------------------------------------------------
** 版　本: 
** 修改人: 
** 日　期: 
** 描　述: 
********************************************************************************************************/
void CAN_FilterConfig(void)
{
	register t_ureg i;
	#if CAN_FILTER_N > 14
	#error	"Communi.C CAN_FilterConfig() CAN_FILTER_N is Too Large!\r\n"
	#endif
	
	for(i = 0; i < CAN_FILTER_N; i++)
	{
		CAN_wrFilterExt32MaskMode(i, FRM_CAN_ID_INIT(FRM_PLAYER_MAC_BASE + i, GET_LOAL_MAC()),
								  (uint32)FRM_CAN_ID_VARBITS, i & 0x0001);
		
	}
}

/* CAN1控制及状态变化中断 */
void CAN1_SCE_IRQHandler (void)
{
	register t_ureg dw_r;
	register t_CanErrN *pErr;
	register __IO uint32_t *pESR;
	
	pESR = &CAN1->ESR;
	
	dw_r = (*pESR) & CAN_ESR_LEC;
	// LEC上次错误号
	if(dw_r)					/* 有错误发生 */
	if(dw_r != CAN_ESR_LEC_USER)		/* 错误号与上次设置不同 */
	{
		dw_r = RSHFT(dw_r, CAN_ESR_LEC_BITnL)-1;
		pErr = &g_History.SysErrors.CAN_LEC[dw_r];
		(*pErr)++;
		SETBITs(*pESR, CAN_ESR_LEC_USER);
	}
	
	// 离线错误
	if(CHKBITs(*pESR, CAN_ESR_BOFF)){
		pErr = &g_History.SysErrors.CAN_BOF;
		(*pErr)++;
		CLRBITs(*pESR, CAN_ESR_BOFF | CAN_ESR_TEC);
	}
	// 被动错误
	if(CHKBITs(*pESR, CAN_ESR_EPVF)){
		pErr = &g_History.SysErrors.CAN_EPV;
		(*pErr)++;
		CLRBITs(*pESR, CAN_ESR_EPVF | CAN_ESR_REC);
	}
	// 错误警告
	if(CHKBITs(*pESR, CAN_ESR_EWGF)){
		pErr = &g_History.SysErrors.CAN_EWG;
		(*pErr)++;
		CLRBITs(*pESR, CAN_ESR_EWGF);
	}
	
	dw_r = (*pESR);
	// 如果失败次数太多则取消发送当前帧
	if( (dw_r & CAN_ESR_TEC) > (10 << CAN_ESR_TEC_BITnL) ) //	Add by Xsky 2011-07-30 18:30
	{
		CLRBITs(*pESR, CAN_ESR_TEC);
		/* Clear CAN_TSR_RQCPx (rc_w1) */
		CAN1->TSR = CAN_TSR_RQCP0|CAN_TSR_RQCP1|CAN_TSR_RQCP2;
	}
	CAN1->MSR = CAN_MSR_ERRI;		// 清除错误中断, Add by Xsky 2011-07-30 17:20

}

/*********************************************************************************************************
** 函数名称: CAN_QueueWriteFull
** 功能描述: CAN队列写满操作函数, 直接对调用次数计数并存储在参数区,以统计错误
** 输　入: 
** 输　出: 无
** 返　回: 错误代码
** 全局变量: 无
** 调用模块: 
** 版　本: v0.00.1
** 作　者: Xsky
** 日　期: 2011-03-10 11:50
** 备　注: 
**-------------------------------------------------------------------------------------------------------
** 版　本: 
** 修改人: 
** 日　期: 
** 描　述: 
********************************************************************************************************/
t_err CAN_QueueWriteFull(uint32 TxRx)
{
	if(TxRx == CAN_TxRx_Tx){
		g_History.SysErrors.CAN_SendQueOVF++;
	}
	else // CAN_TXRx_Rx
	{
		g_History.SysErrors.CAN_RcvQueOVF++;
	}
	
	return QUEUE_OK;
}


/*********************************************************************************************************
** 以下为应用层调用代码
********************************************************************************************************/
// 注意SendBuf必须为双字节, 在部分代码操作中将uint24的目标字节当做uint32处理
static t_frame SendBuf;								/* 发送缓冲区 */

/*********************************************************************************************************
** 函数名称: SendWagerBeginMsg
** 功能描述: 发送开局广播
** 输　入: 无
** 输　出: 无
** 返　回: 错误代码
** 全局变量: 无
** 调用模块: SendToPlayer()
** 版　本: v0.00.1
** 作　者: Xsky
** 日　期: 2011-06-18 08:23
** 备　注: 
********************************************************************************************************/
t_err SendWagerBeginMsg(void)
{
	t_frame frame;
	t_err err;
	
	/* 发送开局消息 */
//	frame.u08[0] = FRM_CODE_WagerBeginMsg;			/* 发送开局消息 */
//	frame.u08[1] = g_Param.GambleParam.WagerSec;	/* 参数为押分时长 */
//	err = SendToPlayer(FRM_MAC_BROADCAST, FRM_T_GameCtrlMsg, &frame, sizeof(uint8)*2);
	
	Delay_Nus(1);									/* 等待一定的数据发送时间 */
	
	return err;
}

/*********************************************************************************************************
** 函数名称: SendWagerEndMsg
** 功能描述: 发送结束押分广播
** 输　入: 无
** 输　出: 无
** 返　回: 错误代码
** 全局变量: 无
** 调用模块: SendToPlayer()
** 版　本: v0.00.1
** 作　者: Xsky
** 日　期: 2011-06-21 16:56
** 备　注: 
********************************************************************************************************/
t_err SendWagerEndMsg(void)
{
	t_frame frame;
	t_err err;
	
	/* 发送开局消息 */
//	frame.u08[0] = FRM_CODE_WagerOverMsg;			/* 发送结束押分消息 */
//	err = SendToPlayer(FRM_MAC_BROADCAST, FRM_T_GameCtrlMsg, &frame, sizeof(uint8));
	
	Delay_Nus(1);									/* 等待一定的数据发送时间 */
	
	return err;
}

/*********************************************************************************************************
** 函数名称: SendWagerResultMsg
** 功能描述: 发送游戏结果消息
** 输　入: 无
** 输　出: 无
** 返　回: 错误代码
** 全局变量: 无
** 调用模块: SendToPlayer()
** 版　本: v0.00.1
** 作　者: Xsky
** 日　期: 2011-06-21 17:27
** 备　注: 
********************************************************************************************************/
//t_err SendGameResultMsg(T_FrmGameRusult result)
//{
//	t_frame frame;
//	t_err err;
//	
//	/* 发送游戏结果消息 */
//	frame.u32[0] = result;
//	err = SendToPlayer(FRM_MAC_BROADCAST, FRM_T_GameResult, &frame, sizeof(T_FrmGameRusult));
//	
//	//Delay_Nus(1);									/* 等待一定的数据发送时间 */
//	
//	return err;
//}

/*********************************************************************************************************
** 函数名称: CalProfit
** 功能描述: 计算指定押分项的红利
** 输　入: 1. Bets: 单机各项押分/整机各项押分, 在不加上当前Bet值之前的的押分值,
**				注意该数组的类型，传入参数时不能进行强制类型转换
**		 2. iBet: 押分项索引
**		 3. Bet: 当前押分值
** 输　出: 无
** 返　回: 红利绝对值
** 全局变量: g_Odds
** 调用模块: 
** 版　本: v0.00.1
** 作　者: Xsky
** 日　期: 2011-06-21 10:23
** 备　注: 
********************************************************************************************************/
__inline uint32 CalProfit(register uint32 Bets[], register t_ureg_opt iBet, uint32 Bet)
{
	register uint32 other;
	register uint32 cur;
	register t_ureg_opt j;
	
	other = 0;
	for(j = 0; j < BET_ITEMS; j++)				/* 计处所有押分分数和 */
	{
		other += Bets[iBet];
	}
	other -= Bets[iBet];						/* 其它项押分和 */
	
	cur = Bet + Bets[iBet];						/* 本项总押 */
	cur = (cur * g_Odds[iBet]) >> 10; 			/* 计算本项押分最大赔分值 */
	
	return cur > other? cur-other: other-cur; 	/* 计算红利差额 */
}

/*********************************************************************************************************
** 函数名称: 
** 功能描述: 以下分别对应接收到的消息
** 输　入: 1. iPlayer: 玩家统号索引值，为机台号-1，从0~N
**		 2. pFrame: 接收到的数据缓冲区
**		 3. nBytes: 接收到的数据长度
** 输　出: 无
** 返　回: 错误代码t_err
**		 ERR_NONE: 无错误
** 全局变量: 无
** 调用模块: 
** 版　本: v0.00.1
** 作　者: Xsky
** 日　期: 2011-06-18 08:23
** 备　注: 
********************************************************************************************************/
/*  发送返回消息 */
/* 0x01: a_GameCtrlMsg: 处理分机反回的游戏控制消息 */
static t_err  a_01_GameCtrlMsg(register uint8 iPlayer, register t_frame *pFrame, register t_ureg_opt nBytes)
{
	register t_ureg_opt r;
	register t_err err;
	if(nBytes == 0) {
		// 返回帧错误
		return ERR_FrameError;
	}
	err = ERR_NONE;
	r = pFrame->u08[0];
//	switch(r)
//	{
//		case FRM_CODE_WagerBeginMsg:
//			if(GameStateGet() == GAME_STATE_Wager || GameStateGet()==GAME_STATE_WagerInit)
//			{
//				SETBITs(g_PlayerWager[iPlayer].Flag, PLAYER_FLGMSK_WagerStart);
//			}
//			break;
//		case FRM_CODE_WagerOverMsg:
//			if(GameStateGet() == GAME_STATE_Wager || GameStateGet()==(GAME_STATE_Wager+1))
//			{
//				SETBITs(g_PlayerWager[iPlayer].Flag, PLAYER_FLGMSK_WagerEnd);
//			}
//			break;
//		default:
//			
//			break;
//	}
	
	
	return err;
}
/* 0x02: a_OddsMsg: 押分赢分倍率回复 */
static t_err  a_02_OddsMsg(register uint8 iPlayer, register t_frame *pFrame, register t_ureg_opt nBytes)
{
	return ERR_NONE;
}
/* 0x03: a_GameResult: 收到分机押分结束 */
static t_err  a_03_GameResult(register uint8 iPlayer, register t_frame *pFrame, register t_ureg_opt nBytes)
{
//	SETBITs(g_PlayerWager[iPlayer].Flag, PLAYER_FLGMSK_WagerEnd);
	return ERR_NONE;
}
/* 0x04: a_PlayerPoint: 收到分机回复本局赢利 */
static t_err  a_04_PlayerWinBet(register uint8 iPlayer, register t_frame *pFrame, register t_ureg_opt nBytes)
{
//	SETBITs(g_PlayerWager[iPlayer].Flag, PLAYER_FLGMSK_Over);
	return ERR_NONE;
}

/* 接收返回消息 */
/* 0x21: c_InitDataRequest:向主板请求初始数据 */
static t_err  c_21_InitDataRequest(register uint8 iPlayer, register t_frame *pFrame, register t_ureg_opt nBytes)
{
	// 初始数据
	register uint8 *pSend = (uint8 *)&SendBuf;
	register t_ureg_opt i, j;
//	register T_PlayerWager *pPlayerWager;
	t_ureg_opt N;
	t_err err;
	
//	pPlayerWager = &g_PlayerWager[iPlayer];
	// 发送第0段初始化数据
//	((T_FrmInitData0 *)pSend)->Seg = 0x00;								// 第0段标记
//	((T_FrmInitData0 *)pSend)->WagerSec = g_Param.GambleParam.WagerSec;	// 押分时长
//	((T_FrmInitData0 *)pSend)->WagerRate = pPlayerWager->WagerRateIndex;	// 押分倍率
//	((T_FrmInitData0 *)pSend)->Stake = *((uint24 *)&pPlayerWager->Stake);				// 赌金
//	err = SendToPlayer(iPlayer, FRM_T_InitDataRequest, (t_frame *)pSend, sizeof(T_FrmInitData0));
	
	// 发送后续各项押分值
	i = 0;
	j = 0;
	while(i < BET_ITEMS)
	{
//		((T_FrmInitData0 *)pSend)->Seg = ((BET_ITEMS+1)/2 << 4) | ((i >> 1) + 1); // 计算总段数(高四位),当前段号(低四位)
//		N = sizeof(((T_FrmInitData1 *)pSend)->Seg);
		for(j = 0; j< 2; j++)
		{
//			((T_FrmInitData1 *)pSend)->Bet[j].index = i+1;				// 项分项索引
//			((T_FrmInitData1 *)pSend)->Bet[j].Bet = pPlayerWager->Bet[i];// 第j项押分项值
//			N += sizeof( ((T_FrmInitData1 *)pSend)->Bet[0] );
			
			i++;
			if(i == BET_ITEMS){
				break;	/* break: for(j = 0; j< 2; j++) */
			}
		}
//		err = SendToPlayer(iPlayer, FRM_T_InitDataRequest, (t_frame *)pSend, nBytes);
	}
	// 设置分机初始化完成标志
//	SETBITs(pPlayerWager->Flag, PLAYER_FLGMSK_InitOK);
	
	return err;
}
/* 0x22: c_InitDataRequest:  */
static t_err  c_22_WagerEvent(uint8 iPlayer, t_frame *pFrame, t_ureg_opt nBytes)
{
	register t_ureg_opt i;
	register t_ureg_opt iBet;
	register uint8 *pSend = (uint8 *)&SendBuf;
//	register T_PlayerWager *pPlayerWager;
	register uint32 bet,r1;
	t_ureg_opt Seg = 0;			// 记录段数
	t_ureg_opt	N = 0;			// 有效押分项数
	t_err err = ERR_NONE;
	t_ureg_opt nSendBytes;
	register T_Param *pParam = &g_Param;
	//register T_WagerBet *pWagerBet;
	
	/* 检查游戏状态 */
	OS_ENTER_CRITICAL();
	r1 = GameStateGet();
	bet = SYS_StateGet();
	OS_EXIT_CRITICAL();
//	if( (r1 != GAME_STATE_Wager) || (bet != SYS_STATE_Game) )
	{
		// 非游戏押分状态则输出
		return ERR_NONE;
	}
	
//	pPlayerWager = &g_PlayerWager[iPlayer];
	for(i = 0; i < nBytes; i++)
	{
	}	/* end: for(i = 0; i < nBytes; i++) */
	
	return err;
}
/* 0x23: c_WagerCtrlMsg: 向主板发送押分控制消息 */
static t_err  c_23_WagerCtrlMsg(register uint8 iPlayer, register t_frame *pFrame, register t_ureg_opt nBytes)
{
	register uint32 r;
	t_err err = ERR_NONE;
	register t_frame *pSend;
	register uint32 r1;
	
	SYS_StateRead(r1);
	if(r1 != SYS_STATE_Game)					/* 非游戏状态输出程序 */
	{
		return ERR_NONE;
	}
	
	r = pFrame->u08[0];
//	switch(r)
//	{
//		case FRM_CODE_WagerOverMsgUp:
//			SETBITs(g_PlayerWager[iPlayer].Flag, PLAYER_FLGMSK_WagerEnd);
//			break;
//		case FRM_CODE_LockOn: 
//			SETBITs(g_PlayerWager[iPlayer].Flag, PLAYER_FLGMSK_LockOn);
//			break;
//		case FRM_CODE_LockOff:
//			CLRBITs(g_PlayerWager[iPlayer].Flag, PLAYER_FLGMSK_LockOn);
//			// 保存玩家分数
//			FRAM_Write(STRUCT_OFFSET(T_DataStore, History[0]), &(g_History.PlayerStake[iPlayer]), sizeof(g_History.PlayerStake[iPlayer]));
//			HISTORY_SaveSub(PlayerStake[iPlayer]);
//			break;
//		default:
//			r = FRM_CODE_Undef;
//			break;
//	}
//	if(r != FRM_CODE_Undef)
//	{
//		pSend = &SendBuf;
//		pSend->u08[0] = r;
//		pSend->u08[1] = 0x00;
//		r = SendToPlayer(iPlayer, FRM_T_WagerCtrlMsg, pSend, sizeof(uint8)*2);
//		if(r != ERR_NONE){
//			err = r;
//		}
//	}
	
	return err;
}
/* 0x24: c_BetMsg: 向主板发送上下分消息 */
static t_err  c_24_BetMsg(register uint8 iPlayer, register t_frame *pFrame, register t_ureg_opt nBytes)
{
	register uint32 r;							// 临时变量
	t_err err = ERR_NONE;
	register t_frame *pSend;
//	register T_PlayerWager *pPlayerWager;
	register uint32 r1;							// 临时变量
	register uint8 flag;
	
	SYS_StateRead(r1);
	if(r1 != SYS_STATE_Game)					/* 非游戏状态输出程序 */
	{
		return ERR_NONE;
	}
	
//	pPlayerWager = &g_PlayerWager[iPlayer];
//	r = pFrame->u08[0];
//	flag = FRM_CODE_ErrUndef;
//	switch(r)
//	{
//		default:
//			r = FRM_CODE_Undef;
//			break;
//	}
//	if(r != FRM_CODE_Undef)
//	{
//		pSend = &SendBuf;
//		((T_FrmBetMsg *)pSend)->Code = r;
//		((T_FrmBetMsg *)pSend)->Flag = flag;
//		r1 = pPlayerWager->Stake;
//		((T_FrmBetMsg *)pSend)->Stake.u08[0] = (uint8)r1;
//		((T_FrmBetMsg *)pSend)->Stake.u08[1] = (uint8)(r1 >> 8);
//		((T_FrmBetMsg *)pSend)->Stake.u08[2] = (uint8)(r1 >> 16);
//		r = SendToPlayer(iPlayer, FRM_T_WagerCtrlMsg, pSend, sizeof(T_FrmBetMsg));
//		if(r != ERR_NONE){
//			err = r;
//		}
//	}
	
	return err;
}
/* 0x25: c_FuncKeyMsg1: 向主板发送按键消息1 */
static t_err  c_25_FuncKeyMsg1(register uint8 iPlayer, t_frame *pFrame, register t_ureg_opt nBytes)
{
	register uint32 r;
	t_err err = ERR_NONE;
	register t_frame *pSend;
	register uint32 r1;
	register uint8 *pData;
	register t_ureg_opt nSend;
	
	SYS_StateRead(r1);
	if(r1 != SYS_STATE_Game)					/* 非游戏状态输出程序 */
	{
		return ERR_NONE;
	}
	
	pData = &(pFrame->u08[1]);
	while(nBytes--)
	{
		r = *pData++;
		pSend = &SendBuf;
		nSend = 0;
//		switch(r)
//		{
//			case FRM_CODE_KeyChange:
//				nSend = g_PlayerWager[iPlayer].WagerRateIndex++;
//				nSend = nSend > WAGER_RATE_INDEX_MAX? WAGER_RATE_INDEX_1: r;
//				pSend->u08[2] = nSend;
//				nSend = sizeof(uint8) * 3;
//				break;
//			case FRM_CODE_KeyRecord:
//				
//				nSend = 0;
//				break;
//			default:
//				r = FRM_CODE_Undef;
//				break;
//		}
//		if(r != FRM_CODE_Undef)
//		{
//			pSend->u08[0] = 0x00;		// 保留
//			pSend->u08[1] = r;			// 按键
//			r = SendToPlayer(iPlayer, FRM_T_WagerCtrlMsg, pSend, sizeof(uint8)*2);
//			if(r != ERR_NONE){
//				err = r;
//			}
//		}
		
	}
	
	return err;
}
/* 0x2A: c_ErrorData:  */
static t_err  c_2A_ErrorData(register uint8 iPlayer, register t_frame *pFrame, register t_ureg_opt nBytes)
{
	register t_ureg_opt r;
	t_err err = ERR_NONE;
	static uint8 FrmID = 0xFF;
	
	r = ((T_FrmPlayerErr *)pFrame)->FrmID;
	if(r != FrmID)
	{
		r = ((T_FrmPlayerErr *)pFrame)->ErrIndex;
		g_History.GameErrors.Player[iPlayer].Indexs[r] += ((T_FrmPlayerErr *)pFrame)->ErrTimes;
		
		//pSend = pFrame;
		// 回复原始数据
//		err = SendToPlayer(iPlayer, FRM_T_ErrorData, pFrame, sizeof(T_FrmPlayerErr));
	}
	
	return err;
}

typedef t_err t_frameHandler(register uint8, t_frame *, t_ureg_opt);

static const void *r_0x_HandlerTbl[] = 
	{NULL, 	a_01_GameCtrlMsg,	a_02_OddsMsg, a_03_GameResult, 
	/* 0			1				2					3	*/
	a_04_PlayerWinBet
	/* 4			5				6				7	*/
	/* 8			9				10				11	*/
	/* 12			13				14				15	*/
	};
static const void *r_2x_HandlerTbl[] = 
	{NULL, 	c_21_InitDataRequest, c_22_WagerEvent, c_23_WagerCtrlMsg, 
	/* 0			1				2				3	*/
	c_24_BetMsg, c_25_FuncKeyMsg1, NULL,			NULL,
	/* 4			5				6				7	*/
	NULL,			NULL,		  c_2A_ErrorData 
	/* 8			9				A				11	*/
	/* 12			13				14				15	*/
	};

/*********************************************************************************************************
** 函数名称: EXTI4_IRQHandler
** 功能描述: 软件中断触发，用于处理CAN接收数据
** 输　入: 无
** 输　出: 无
** 返　回: 错误代码
** 全局变量: 无
** 调用模块: 
** 版　本: v0.00.1
** 作　者: Xsky
** 日　期: 2011-06-18 15:21
** 备　注: 
********************************************************************************************************/
void CAN1_SWI_Handler (void)
{
	#define _DbgCanMsg_		_DbgPrintf_
	#define _DbgCanMsgLcd_	_DbgLcdPrintf_
	
	// CAN接收到数据后先压入FIFO，然后触发该中断服务对应的软件中断
	static T_CanFrame	CanFrame = {0, };
	t_err err;
	//uint8	DstMAC;
	register uint8	PlayerID;
	register T_FRM_Type	type;
	register uint32 r;
	register void *p;
	
	// 清除中断
	CAN1_SWI_ClrInt();
	
	// 读取CAN帧
	while(1)
	{
		_DbgCanMsg_(( "CAN.SWI:" ));
		
		CanFrame.Data.u32[0] = 0x00000000;
		CanFrame.Data.u32[1] = 0x00000000;
		err = CAN_ReadFrame(&CanFrame);
		if(err != QUEUE_OK)
		{
			break;		/* break: while(1) */
		}
		
		r = CanFrame.IxR >> FRM_ID_LShitBITn;
		/* 		已由CAN进行硬件过滤，不用再判断
		DstMAC = (r >> FRM_DST_MAC_BITn) & FRM_DST_MAC_Msk;
		if(DstMAC != GET_LOAL_MAC())
		{
			break;
		}*/
		PlayerID = (r >> FRM_SRC_MAC_BITn) & FRM_SRC_MAC_Msk;	/* SrcMAC已进行过滤,不用判断 */
		PlayerID--;												/* 转换为以零开始的index */
		type = (r >> FRM_ID_TYPE_BITnL) & FRM_ID_TYPE_Msk;
		_DbgCanMsg_(( "ID:%08X,[%u]:%02X,%02X %02X\r\n", r, PlayerID, type, CanFrame.Data.u08[0], CanFrame.Data.u08[1] ));
		_DbgCanMsgLcd_(( 0,0, "ID[%u]:%02X ", PlayerID, type ));
		_DbgCanMsgLcd_(( 1,0, "%02X %02X ", CanFrame.Data.u08[0], CanFrame.Data.u08[1] ));
		if(PlayerID < FRM_PLAYER_MAC_BASE)
		{
			_DbgCanMsg_(( "CIdErr0!\r\n" ));
			return;
		}
		PlayerID -= FRM_PLAYER_MAC_BASE;
		if(PlayerID >= MAX_PLAYER)
		{
			_DbgCanMsg_(( "CIdErr1!\r\n" ));
			
			return;
		}
		
//		if(type == FRM_T_WagerEvent)
//		{
//			// 押分消息优先处理
//			err = c_22_WagerEvent(PlayerID, (t_frame *)&CanFrame.Data, 
//								CanFrame.DTxR & 0xFF);
//		}
//		else
		{
			p = NULL;
			r = type & 0xF0;
			type &= 0x0F;
			if((r == 0x20) && (type < lenof(r_2x_HandlerTbl)))
			{
				p = r_2x_HandlerTbl;
			}
			else if((r == 0x20) && (type < lenof(r_0x_HandlerTbl)))
			{
				p = r_0x_HandlerTbl;
			}
			if(p != NULL)
			{
				err = ((t_frameHandler *)p)(PlayerID, (t_frame *)&CanFrame.Data, 
											CanFrame.DTxR & 0xFF);
			}else{
				err = ERR_UndefPlayerData;
			}
		}
	}
	
}


#ifdef DEBUG
// 注意该函数在中断中被调用, 延时函数不能太长否则影响其它通讯
void KeyESC_Event(T_IOEvent event)
{
	t_frame frame;
	register t_ureg N;
	
	if(event == IN_Event_Down)
	{
		/* 发送开局消息 */
//		frame.u08[0] = FRM_CODE_WagerBeginMsg;			/* 发送开局消息 */
//		frame.u08[1] = g_Param.GambleParam.WagerSec;	/* 参数为押分时长 */
//		frame.u08[2] = 0x22;
//		frame.u08[3] = 0x33;
//		frame.u32[1] = 0x77665544;
//		for(N = 0; N < FRM_MAX_BYTES+1; N++)
//		{
//			// 模拟主机发至分机
//			Dbg_CAN_SendData(FRM_HOST_MAC, FRM_PLAYER_MAC_BASE+N, FRM_T_GameCtrlMsg, &frame, N % (FRM_MAX_BYTES+1) );
//			Delay_Nus(180 + N * 20);									/* 等待一定的数据发送时间 */
//			//Delay_Nms(500);
//		}
//		//Delay_Nus(50 * 1000); 										/* 等待一定的数据发送时间 */
//		Delay_Nus(500);
//		for(N = 0; N < FRM_MAX_BYTES+1; N++)
//		{
//			// 模拟分机发至主机
//			Dbg_CAN_SendData(FRM_PLAYER_MAC_BASE+N, FRM_HOST_MAC, FRM_T_GameCtrlMsg, &frame, N % (FRM_MAX_BYTES+1) );
//			Delay_Nus(180 + N * 20);									/* 等待一定的数据发送时间 */
//		}
		//Dbg_CAN_SendData(FRM_HOST_MAC, FRM_T_GameCtrlMsg, &frame, sizeof(uint8)*2);
	}
	
	
}
#endif

