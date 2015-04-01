/*****************************************************************************************************
  Copyright (C), 2009-2009, YingHong, Ltd.
  File name:   	Communi.C
  Author:      	Xsky
  Version:     	1.0
  Date:		2011-03-16 15:56
  Description:   ���ֻ�ͨ����غ�����ȫ����������Ϸ���Ƹ߹�������Ѻ�ּ����Ƿ��ѹ
  Others:         
  Function List: 
  	1. ��
  	2. ��
  	3. ��
  	4. ��
  	5. ��

  History:        // �޸ļ�¼
    1. Date:		2011-03-16 15:56
       Author:		Xsky
       Modification:	������ģ��
**------------------------------------------------------------------------------------------------------
** ����޸�����: 2011-04-13 14:45
** �޸���: Xsky
** �汾: v1.0.00
**-------------------------------------------------------------------------------------------------------
** �޸���: Xsky
** ����:  2011-04-13 14:45
** �汾: v1.0.00
** �޸�: 
**			1. �޸ĺ���CAN_SendData()
**			2. 
*****************************************************************************************************/


#define COMMUNI_GLOBALS

#include "Config.h"


/*********************************************************************************************************
** ��������: CAN_SendBytes
** ��������: ����������Ϊ�����͵�CAN���������ʽT_CanFrame/CAN_TypeDef
** 			 ����ʱ��������п���ֱ�ӽ�����ѹ�����䣬��������ѹ�����
**			 �������ʧ�ܴ���Ϊ3��ȡ������, ԴMACΪ�̶�ֵ����MAC
**			 ��ʹ��ʱ�䴥��ģʽ
** �䡡��:  1. iPlayer: Ŀ�Ļ�̨������0~11, Ϊ��̨��-1
**			2. FrmType: ֡����(������)/������
**			3. *pData:  Ҫ���͵�����Դ������,Ϊ���Ч�ʶ���ΪT_CanDataֱ�ӽ�32λ��ȡ
**			4. nBytes: Ҫ���͵������ֽ���,0~8
** �䡡��: ��
** ������: �������
** ȫ�ֱ���: ��
** ����ģ��: 
** �桡��: v0.0.01
** ������: Xsky
** �ա���: 2010-03-16 16:00
** ����ע: 
**-------------------------------------------------------------------------------------------------------
** �桡��: v0.0.02
** �޸���: Xsky
** �ա���: 2011-04-13 14:45
** �衡��: FRM_ID_TYPE_BITnL ��Ϊ (FRM_ID_TYPE_BITnL + FRM_ID_LShitBITn)
********************************************************************************************************/
t_err CAN_SendData(register uint8 iPlayer, register T_FRM_Type FrmType, 
							T_CanData *pData, uint32 nDataBytes)
{
	T_CanFrame CanFrame = {0, };
	
	if(nDataBytes > FRM_MAX_BYTES)
	{
		// ���͵��ֽ�������
		return ERR_SendToLong;
	}
	_DbgPrintf_(( "CanID:%08X\r\n", CanFrame.IxR >> FRM_ID_LShitBITn ));
	CanFrame.IxR = FRM_CAN_ID_INIT(GET_LOAL_MAC(), FRM_BASE_MAC + (iPlayer+1))
				| (FrmType << (FRM_ID_TYPE_BITnL + FRM_ID_LShitBITn));
	//memcpy(&CanFrame.Data.u08[0], pSrc, nDataBytes);
	CanFrame.DTxR = nDataBytes;				/* ��CAN_TDTxR�Ĵ����ĳ�����λ������������Ϊ0, ����ֱ�Ӹ�ֵDLC */
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
		// ���͵��ֽ�������
		return ERR_SendToLong;
	}
	
	CanFrame.IxR = FRM_CAN_ID_INIT(SrcMac, DstMac)
				| (FrmType << (FRM_ID_TYPE_BITnL + FRM_ID_LShitBITn));
	_DbgPrintf_(( "cID:%08X\r\n", CanFrame.IxR >> FRM_ID_LShitBITn ));
	//memcpy(&CanFrame.Data.u08[0], pSrc, nDataBytes);
	CanFrame.DTxR = nDataBytes;				/* ��CAN_TDTxR�Ĵ����ĳ�����λ������������Ϊ0, ����ֱ�Ӹ�ֵDLC */
	CanFrame.Data.u32[0] = pData->u32[0];
	CanFrame.Data.u32[1] = pData->u32[1];
	
	return CAN_SendFrame(&CanFrame);
}
#endif

/*********************************************************************************************************
** ��������: CAN_FilterConfig
** ��������: ��ʼ��CAN1 Filter
**			 ���ñ�ʶ��32λ����λģʽ����Э���ж����PrioID,SrcMAC,DstMAC
**			 �ֱ����ƥ��, ֻ��TypeID���ֿ�����ƥ��
**			 ֻ����Ŀ��MACΪ����MAC,ԴMACΪ1~10�ŷֻ���Ӧ��0x11~0x1C
**			 �ҹ�������ż��/�����ֱ���FIFO0/FIFO1��Ӧ
**		   ע: ���յ�DstMACΪ�̶�ֵ�ڱ�.h�ļ��ж���
** �䡡��: 
** �䡡��: ��
** ������: �������
** ȫ�ֱ���: ��
** ����ģ��: 
** �桡��: v0.00.1
** ������: Xsky
** �ա���: 2010-03-17 11:50
** ����ע: 
**-------------------------------------------------------------------------------------------------------
** �桡��: 
** �޸���: 
** �ա���: 
** �衡��: 
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

/* CAN1���Ƽ�״̬�仯�ж� */
void CAN1_SCE_IRQHandler (void)
{
	register t_ureg dw_r;
	register t_CanErrN *pErr;
	register __IO uint32_t *pESR;
	
	pESR = &CAN1->ESR;
	
	dw_r = (*pESR) & CAN_ESR_LEC;
	// LEC�ϴδ����
	if(dw_r)					/* �д����� */
	if(dw_r != CAN_ESR_LEC_USER)		/* ��������ϴ����ò�ͬ */
	{
		dw_r = RSHFT(dw_r, CAN_ESR_LEC_BITnL)-1;
		pErr = &g_History.SysErrors.CAN_LEC[dw_r];
		(*pErr)++;
		SETBITs(*pESR, CAN_ESR_LEC_USER);
	}
	
	// ���ߴ���
	if(CHKBITs(*pESR, CAN_ESR_BOFF)){
		pErr = &g_History.SysErrors.CAN_BOF;
		(*pErr)++;
		CLRBITs(*pESR, CAN_ESR_BOFF | CAN_ESR_TEC);
	}
	// ��������
	if(CHKBITs(*pESR, CAN_ESR_EPVF)){
		pErr = &g_History.SysErrors.CAN_EPV;
		(*pErr)++;
		CLRBITs(*pESR, CAN_ESR_EPVF | CAN_ESR_REC);
	}
	// ���󾯸�
	if(CHKBITs(*pESR, CAN_ESR_EWGF)){
		pErr = &g_History.SysErrors.CAN_EWG;
		(*pErr)++;
		CLRBITs(*pESR, CAN_ESR_EWGF);
	}
	
	dw_r = (*pESR);
	// ���ʧ�ܴ���̫����ȡ�����͵�ǰ֡
	if( (dw_r & CAN_ESR_TEC) > (10 << CAN_ESR_TEC_BITnL) ) //	Add by Xsky 2011-07-30 18:30
	{
		CLRBITs(*pESR, CAN_ESR_TEC);
		/* Clear CAN_TSR_RQCPx (rc_w1) */
		CAN1->TSR = CAN_TSR_RQCP0|CAN_TSR_RQCP1|CAN_TSR_RQCP2;
	}
	CAN1->MSR = CAN_MSR_ERRI;		// ��������ж�, Add by Xsky 2011-07-30 17:20

}

/*********************************************************************************************************
** ��������: CAN_QueueWriteFull
** ��������: CAN����д����������, ֱ�ӶԵ��ô����������洢�ڲ�����,��ͳ�ƴ���
** �䡡��: 
** �䡡��: ��
** ������: �������
** ȫ�ֱ���: ��
** ����ģ��: 
** �桡��: v0.00.1
** ������: Xsky
** �ա���: 2011-03-10 11:50
** ����ע: 
**-------------------------------------------------------------------------------------------------------
** �桡��: 
** �޸���: 
** �ա���: 
** �衡��: 
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
** ����ΪӦ�ò���ô���
********************************************************************************************************/
// ע��SendBuf����Ϊ˫�ֽ�, �ڲ��ִ�������н�uint24��Ŀ���ֽڵ���uint32����
static t_frame SendBuf;								/* ���ͻ����� */

/*********************************************************************************************************
** ��������: SendWagerBeginMsg
** ��������: ���Ϳ��ֹ㲥
** �䡡��: ��
** �䡡��: ��
** ������: �������
** ȫ�ֱ���: ��
** ����ģ��: SendToPlayer()
** �桡��: v0.00.1
** ������: Xsky
** �ա���: 2011-06-18 08:23
** ����ע: 
********************************************************************************************************/
t_err SendWagerBeginMsg(void)
{
	t_frame frame;
	t_err err;
	
	/* ���Ϳ�����Ϣ */
//	frame.u08[0] = FRM_CODE_WagerBeginMsg;			/* ���Ϳ�����Ϣ */
//	frame.u08[1] = g_Param.GambleParam.WagerSec;	/* ����ΪѺ��ʱ�� */
//	err = SendToPlayer(FRM_MAC_BROADCAST, FRM_T_GameCtrlMsg, &frame, sizeof(uint8)*2);
	
	Delay_Nus(1);									/* �ȴ�һ�������ݷ���ʱ�� */
	
	return err;
}

/*********************************************************************************************************
** ��������: SendWagerEndMsg
** ��������: ���ͽ���Ѻ�ֹ㲥
** �䡡��: ��
** �䡡��: ��
** ������: �������
** ȫ�ֱ���: ��
** ����ģ��: SendToPlayer()
** �桡��: v0.00.1
** ������: Xsky
** �ա���: 2011-06-21 16:56
** ����ע: 
********************************************************************************************************/
t_err SendWagerEndMsg(void)
{
	t_frame frame;
	t_err err;
	
	/* ���Ϳ�����Ϣ */
//	frame.u08[0] = FRM_CODE_WagerOverMsg;			/* ���ͽ���Ѻ����Ϣ */
//	err = SendToPlayer(FRM_MAC_BROADCAST, FRM_T_GameCtrlMsg, &frame, sizeof(uint8));
	
	Delay_Nus(1);									/* �ȴ�һ�������ݷ���ʱ�� */
	
	return err;
}

/*********************************************************************************************************
** ��������: SendWagerResultMsg
** ��������: ������Ϸ�����Ϣ
** �䡡��: ��
** �䡡��: ��
** ������: �������
** ȫ�ֱ���: ��
** ����ģ��: SendToPlayer()
** �桡��: v0.00.1
** ������: Xsky
** �ա���: 2011-06-21 17:27
** ����ע: 
********************************************************************************************************/
//t_err SendGameResultMsg(T_FrmGameRusult result)
//{
//	t_frame frame;
//	t_err err;
//	
//	/* ������Ϸ�����Ϣ */
//	frame.u32[0] = result;
//	err = SendToPlayer(FRM_MAC_BROADCAST, FRM_T_GameResult, &frame, sizeof(T_FrmGameRusult));
//	
//	//Delay_Nus(1);									/* �ȴ�һ�������ݷ���ʱ�� */
//	
//	return err;
//}

/*********************************************************************************************************
** ��������: CalProfit
** ��������: ����ָ��Ѻ����ĺ���
** �䡡��: 1. Bets: ��������Ѻ��/��������Ѻ��, �ڲ����ϵ�ǰBetֵ֮ǰ�ĵ�Ѻ��ֵ,
**				ע�����������ͣ��������ʱ���ܽ���ǿ������ת��
**		 2. iBet: Ѻ��������
**		 3. Bet: ��ǰѺ��ֵ
** �䡡��: ��
** ������: ��������ֵ
** ȫ�ֱ���: g_Odds
** ����ģ��: 
** �桡��: v0.00.1
** ������: Xsky
** �ա���: 2011-06-21 10:23
** ����ע: 
********************************************************************************************************/
__inline uint32 CalProfit(register uint32 Bets[], register t_ureg_opt iBet, uint32 Bet)
{
	register uint32 other;
	register uint32 cur;
	register t_ureg_opt j;
	
	other = 0;
	for(j = 0; j < BET_ITEMS; j++)				/* �ƴ�����Ѻ�ַ����� */
	{
		other += Bets[iBet];
	}
	other -= Bets[iBet];						/* ������Ѻ�ֺ� */
	
	cur = Bet + Bets[iBet];						/* ������Ѻ */
	cur = (cur * g_Odds[iBet]) >> 10; 			/* ���㱾��Ѻ��������ֵ */
	
	return cur > other? cur-other: other-cur; 	/* ���������� */
}

/*********************************************************************************************************
** ��������: 
** ��������: ���·ֱ��Ӧ���յ�����Ϣ
** �䡡��: 1. iPlayer: ���ͳ������ֵ��Ϊ��̨��-1����0~N
**		 2. pFrame: ���յ������ݻ�����
**		 3. nBytes: ���յ������ݳ���
** �䡡��: ��
** ������: �������t_err
**		 ERR_NONE: �޴���
** ȫ�ֱ���: ��
** ����ģ��: 
** �桡��: v0.00.1
** ������: Xsky
** �ա���: 2011-06-18 08:23
** ����ע: 
********************************************************************************************************/
/*  ���ͷ�����Ϣ */
/* 0x01: a_GameCtrlMsg: �����ֻ����ص���Ϸ������Ϣ */
static t_err  a_01_GameCtrlMsg(register uint8 iPlayer, register t_frame *pFrame, register t_ureg_opt nBytes)
{
	register t_ureg_opt r;
	register t_err err;
	if(nBytes == 0) {
		// ����֡����
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
/* 0x02: a_OddsMsg: Ѻ��Ӯ�ֱ��ʻظ� */
static t_err  a_02_OddsMsg(register uint8 iPlayer, register t_frame *pFrame, register t_ureg_opt nBytes)
{
	return ERR_NONE;
}
/* 0x03: a_GameResult: �յ��ֻ�Ѻ�ֽ��� */
static t_err  a_03_GameResult(register uint8 iPlayer, register t_frame *pFrame, register t_ureg_opt nBytes)
{
//	SETBITs(g_PlayerWager[iPlayer].Flag, PLAYER_FLGMSK_WagerEnd);
	return ERR_NONE;
}
/* 0x04: a_PlayerPoint: �յ��ֻ��ظ�����Ӯ�� */
static t_err  a_04_PlayerWinBet(register uint8 iPlayer, register t_frame *pFrame, register t_ureg_opt nBytes)
{
//	SETBITs(g_PlayerWager[iPlayer].Flag, PLAYER_FLGMSK_Over);
	return ERR_NONE;
}

/* ���շ�����Ϣ */
/* 0x21: c_InitDataRequest:�����������ʼ���� */
static t_err  c_21_InitDataRequest(register uint8 iPlayer, register t_frame *pFrame, register t_ureg_opt nBytes)
{
	// ��ʼ����
	register uint8 *pSend = (uint8 *)&SendBuf;
	register t_ureg_opt i, j;
//	register T_PlayerWager *pPlayerWager;
	t_ureg_opt N;
	t_err err;
	
//	pPlayerWager = &g_PlayerWager[iPlayer];
	// ���͵�0�γ�ʼ������
//	((T_FrmInitData0 *)pSend)->Seg = 0x00;								// ��0�α��
//	((T_FrmInitData0 *)pSend)->WagerSec = g_Param.GambleParam.WagerSec;	// Ѻ��ʱ��
//	((T_FrmInitData0 *)pSend)->WagerRate = pPlayerWager->WagerRateIndex;	// Ѻ�ֱ���
//	((T_FrmInitData0 *)pSend)->Stake = *((uint24 *)&pPlayerWager->Stake);				// �Ľ�
//	err = SendToPlayer(iPlayer, FRM_T_InitDataRequest, (t_frame *)pSend, sizeof(T_FrmInitData0));
	
	// ���ͺ�������Ѻ��ֵ
	i = 0;
	j = 0;
	while(i < BET_ITEMS)
	{
//		((T_FrmInitData0 *)pSend)->Seg = ((BET_ITEMS+1)/2 << 4) | ((i >> 1) + 1); // �����ܶ���(����λ),��ǰ�κ�(����λ)
//		N = sizeof(((T_FrmInitData1 *)pSend)->Seg);
		for(j = 0; j< 2; j++)
		{
//			((T_FrmInitData1 *)pSend)->Bet[j].index = i+1;				// ���������
//			((T_FrmInitData1 *)pSend)->Bet[j].Bet = pPlayerWager->Bet[i];// ��j��Ѻ����ֵ
//			N += sizeof( ((T_FrmInitData1 *)pSend)->Bet[0] );
			
			i++;
			if(i == BET_ITEMS){
				break;	/* break: for(j = 0; j< 2; j++) */
			}
		}
//		err = SendToPlayer(iPlayer, FRM_T_InitDataRequest, (t_frame *)pSend, nBytes);
	}
	// ���÷ֻ���ʼ����ɱ�־
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
	t_ureg_opt Seg = 0;			// ��¼����
	t_ureg_opt	N = 0;			// ��ЧѺ������
	t_err err = ERR_NONE;
	t_ureg_opt nSendBytes;
	register T_Param *pParam = &g_Param;
	//register T_WagerBet *pWagerBet;
	
	/* �����Ϸ״̬ */
	OS_ENTER_CRITICAL();
	r1 = GameStateGet();
	bet = SYS_StateGet();
	OS_EXIT_CRITICAL();
//	if( (r1 != GAME_STATE_Wager) || (bet != SYS_STATE_Game) )
	{
		// ����ϷѺ��״̬�����
		return ERR_NONE;
	}
	
//	pPlayerWager = &g_PlayerWager[iPlayer];
	for(i = 0; i < nBytes; i++)
	{
	}	/* end: for(i = 0; i < nBytes; i++) */
	
	return err;
}
/* 0x23: c_WagerCtrlMsg: �����巢��Ѻ�ֿ�����Ϣ */
static t_err  c_23_WagerCtrlMsg(register uint8 iPlayer, register t_frame *pFrame, register t_ureg_opt nBytes)
{
	register uint32 r;
	t_err err = ERR_NONE;
	register t_frame *pSend;
	register uint32 r1;
	
	SYS_StateRead(r1);
	if(r1 != SYS_STATE_Game)					/* ����Ϸ״̬������� */
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
//			// ������ҷ���
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
/* 0x24: c_BetMsg: �����巢�����·���Ϣ */
static t_err  c_24_BetMsg(register uint8 iPlayer, register t_frame *pFrame, register t_ureg_opt nBytes)
{
	register uint32 r;							// ��ʱ����
	t_err err = ERR_NONE;
	register t_frame *pSend;
//	register T_PlayerWager *pPlayerWager;
	register uint32 r1;							// ��ʱ����
	register uint8 flag;
	
	SYS_StateRead(r1);
	if(r1 != SYS_STATE_Game)					/* ����Ϸ״̬������� */
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
/* 0x25: c_FuncKeyMsg1: �����巢�Ͱ�����Ϣ1 */
static t_err  c_25_FuncKeyMsg1(register uint8 iPlayer, t_frame *pFrame, register t_ureg_opt nBytes)
{
	register uint32 r;
	t_err err = ERR_NONE;
	register t_frame *pSend;
	register uint32 r1;
	register uint8 *pData;
	register t_ureg_opt nSend;
	
	SYS_StateRead(r1);
	if(r1 != SYS_STATE_Game)					/* ����Ϸ״̬������� */
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
//			pSend->u08[0] = 0x00;		// ����
//			pSend->u08[1] = r;			// ����
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
		// �ظ�ԭʼ����
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
** ��������: EXTI4_IRQHandler
** ��������: �����жϴ��������ڴ���CAN��������
** �䡡��: ��
** �䡡��: ��
** ������: �������
** ȫ�ֱ���: ��
** ����ģ��: 
** �桡��: v0.00.1
** ������: Xsky
** �ա���: 2011-06-18 15:21
** ����ע: 
********************************************************************************************************/
void CAN1_SWI_Handler (void)
{
	#define _DbgCanMsg_		_DbgPrintf_
	#define _DbgCanMsgLcd_	_DbgLcdPrintf_
	
	// CAN���յ����ݺ���ѹ��FIFO��Ȼ�󴥷����жϷ����Ӧ�������ж�
	static T_CanFrame	CanFrame = {0, };
	t_err err;
	//uint8	DstMAC;
	register uint8	PlayerID;
	register T_FRM_Type	type;
	register uint32 r;
	register void *p;
	
	// ����ж�
	CAN1_SWI_ClrInt();
	
	// ��ȡCAN֡
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
		/* 		����CAN����Ӳ�����ˣ��������ж�
		DstMAC = (r >> FRM_DST_MAC_BITn) & FRM_DST_MAC_Msk;
		if(DstMAC != GET_LOAL_MAC())
		{
			break;
		}*/
		PlayerID = (r >> FRM_SRC_MAC_BITn) & FRM_SRC_MAC_Msk;	/* SrcMAC�ѽ��й���,�����ж� */
		PlayerID--;												/* ת��Ϊ���㿪ʼ��index */
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
//			// Ѻ����Ϣ���ȴ���
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
// ע��ú������ж��б�����, ��ʱ��������̫������Ӱ������ͨѶ
void KeyESC_Event(T_IOEvent event)
{
	t_frame frame;
	register t_ureg N;
	
	if(event == IN_Event_Down)
	{
		/* ���Ϳ�����Ϣ */
//		frame.u08[0] = FRM_CODE_WagerBeginMsg;			/* ���Ϳ�����Ϣ */
//		frame.u08[1] = g_Param.GambleParam.WagerSec;	/* ����ΪѺ��ʱ�� */
//		frame.u08[2] = 0x22;
//		frame.u08[3] = 0x33;
//		frame.u32[1] = 0x77665544;
//		for(N = 0; N < FRM_MAX_BYTES+1; N++)
//		{
//			// ģ�����������ֻ�
//			Dbg_CAN_SendData(FRM_HOST_MAC, FRM_PLAYER_MAC_BASE+N, FRM_T_GameCtrlMsg, &frame, N % (FRM_MAX_BYTES+1) );
//			Delay_Nus(180 + N * 20);									/* �ȴ�һ�������ݷ���ʱ�� */
//			//Delay_Nms(500);
//		}
//		//Delay_Nus(50 * 1000); 										/* �ȴ�һ�������ݷ���ʱ�� */
//		Delay_Nus(500);
//		for(N = 0; N < FRM_MAX_BYTES+1; N++)
//		{
//			// ģ��ֻ���������
//			Dbg_CAN_SendData(FRM_PLAYER_MAC_BASE+N, FRM_HOST_MAC, FRM_T_GameCtrlMsg, &frame, N % (FRM_MAX_BYTES+1) );
//			Delay_Nus(180 + N * 20);									/* �ȴ�һ�������ݷ���ʱ�� */
//		}
		//Dbg_CAN_SendData(FRM_HOST_MAC, FRM_T_GameCtrlMsg, &frame, sizeof(uint8)*2);
	}
	
	
}
#endif
