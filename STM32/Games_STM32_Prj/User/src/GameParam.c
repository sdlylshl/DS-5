/****************************************Copyright (c)**************************************************
**                                  2009-2011, YingHong, Ltd.
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: GameParam.c
**��   ��   ��: Xsky
**�� �� ��  ��: 2011-02-23 16:09
**��        ��: ��Ϸ������ض��弰������
** �汾: v1.0.00
**------------------------------------------------------------------------------------------------------
** ����ļ�: GameParam.h
**			 
**			 
**-------------------------------------------------------------------------------------------------------
**
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

#define  GAMEPARAM_GLOBALS
#include "Global.h"
#include "GameParam.h"
#include "string.h"
#include "Spi_FRAM.h"
#include "STM32_Function.h"

// ȫ�ֱ�����Ϸ����
T_Param 	g_Param = {0, };
// ȫ����Ŀ����,�ȼ�¼����
T_History	g_History = {0, };

/***************************************************************************************************/
//    ����

// ����Ĭ��ֵ����
static __flash T_Param ParamInitTbl = 
{
	//uint16			MemFlag1;			// ��־λ1
	PARAM_MemFlag1_Set,
	//uint16			ParamSize;			// ��������
	sizeof(T_Param),
	//uint16			HardVer;			// Ӳ���汾
	HARD_VER,
	//uint16			SoftVer;			// ����汾��
	SOFT_VER,
	//uint32			WriteTimes;			// ���洢����д����
	0,
	//uint16			Check;				// CRCУ��
	0x00,
	//uint16			MemFlag2;			// ��־λ2
	PARAM_MemFlag2_Set
};

// ��ȡ����
t_err ParamLoad(T_Param *pParam)
{
	FRAM_Read(pParam, FRAM_PARAM_Addr, sizeof(*pParam));
	
	return ERR_NONE;
}
// �������
t_err ParamSave(T_Param *pParam)
{
	pParam->WriteTimes++;
	pParam->MemFlag1 = PARAM_MemFlag1_Set;
	pParam->MemFlag2 = PARAM_MemFlag2_Set;
	pParam->Check = CRC_CalcBlock((uint8 *)pParam, STRUCT_OFFSET(T_History, Check));
	FRAM_Write(FRAM_PARAM_Addr, pParam, sizeof(*pParam));
	
	return ERR_NONE;
}
// ������, ���δ��ʼ�����ʼ������
t_err ParamCheck(T_Param *pParam)
{
	
	if (((pParam->MemFlag1 != PARAM_MemFlag1_Set)
	  && (pParam->MemFlag2 != PARAM_MemFlag2_Set))
	 ||  (pParam->HardVer != ParamInitTbl.HardVer)
	 ||	 (pParam->ParamSize != ParamInitTbl.ParamSize)
	   )
	{
		memcpy(pParam, &ParamInitTbl, sizeof(ParamInitTbl));
		ParamSave(pParam);
	}
	
	return ERR_NONE;
}
// ������ʼ�����ú���
t_err ParamInit(void)
{
	#ifdef DEBUG
	if(sizeof(T_DataStore) >= FRAM_TotalBytes)
	{
		dbg_SendString("FRAM Data is too large!\r\n");
	}
	#endif
	
	ParamLoad(&g_Param);
	ParamCheck(&g_Param);
	
	return ERR_NONE;
}

// ��ȡ��ʷ��¼
t_err HistoryLoad(T_History *pHistory)
{
	FRAM_Read(pHistory, FRAM_HISTORY_Addr, sizeof(*pHistory));
	
	return ERR_NONE;
}
/*************************************************************************************************** 
**	HistorySaveSub()
**	������ʷ��¼��ĳһ���ֻ���ȫ���ֽ�
**  ����: 1. nOffset: �������ʷ���ݽṹ��T_History��ƫ����,ʵ�ʶ���ı���ֻ��g_History
**		 2. nBytes: Ҫ������ֽ���
**	����: ������t_err
**************************************/
t_err HistorySaveSub(register T_History *pHistory, uint32 OffsetAtHistory, uint32 nBytes)
{
	//register T_History *pHistory;
	register uint32 r;
	register uint8 *p;
	register t_ureg_opt i;
	
	//pHistory = &g_History;
	pHistory->WriteTimes++;
	
	pHistory->Check = CRC_CalcBlock((uint8 *)pHistory, STRUCT_OFFSET(T_History, Check));
	
	p = ((uint8 *)pHistory + OffsetAtHistory);
	
	for(i = 0; i < HISTORY_SAVE_Areas; i++)
	{
		r = STRUCT_OFFSET(T_DataStore, History[i]);
		FRAM_Write(r + OffsetAtHistory, p, nBytes);			// ��������i
		if(nBytes != sizeof(T_History))
		{
			FRAM_Write(r + STRUCT_OFFSET(T_History, Check), &pHistory->Check, sizeof(pHistory->Check));			// ��������1
		}
	}
	/*
	r = STRUCT_OFFSET(T_DataStore, History[0]);
	FRAM_Write(r + OffsetAtHistory, p, nBytes);			// ��������1
	if(nBytes != sizeof(T_History))
	{
		FRAM_Write(r + STRUCT_OFFSET(T_History, Check), &pHistory->Check, nBytes);			// ��������1
	}
	
	r = STRUCT_OFFSET(T_DataStore, History[1]);			// ��������2
	FRAM_Write(r + OffsetAtHistory, p, nBytes);
	if(nBytes != sizeof(T_History))
	{
		FRAM_Write(r + STRUCT_OFFSET(T_History, Check), &pHistory->Check, nBytes);			// ��������1
	}
	*/
	
	/* ?????:ȱ�ټ���Ƿ�д�ɹ����ж� */
	
	return ERR_NONE;
}
// ������ʷ��¼
t_err HistorySave(T_History *pHistory)
{
	/*
	pHistory->WriteTimes++;
	pHistory->MemFlag1 = HISTORY_MemFlag1_Set;
	pHistory->MemFlag2 = HISTORY_MemFlag2_Set;
	pHistory->Check = CRC_CalcBlock((uint8 *)pHistory, STRUCT_OFFSET(T_History, Check));
	FRAM_Write(FRAM_HISTORY_Addr, pHistory, sizeof(*pHistory));
	FRAM_Write(FRAM_HISTORY_Bak_Addr, pHistory, sizeof(*pHistory));
	return ERR_NONE;
	*/
	/* ?????:ȱ�ټ���Ƿ�д�ɹ����ж� */
	
	pHistory->MemFlag1 = HISTORY_MemFlag1_Set;
	pHistory->MemFlag2 = HISTORY_MemFlag2_Set;
	return HistorySaveSub(pHistory, 0, sizeof(*pHistory));
	
}
// �����ʷ��¼�Ƿ��ʼ��,ֻ����־λ
t_err HistoryCheck(T_History *pHistory)
{
	if ( (pHistory->MemFlag1 != HISTORY_MemFlag1_Set)
	 &&  (pHistory->MemFlag2 != HISTORY_MemFlag2_Set)
	   )
	{
		//memcpy(pHistory, &HistoryInitTbl, sizeof(HistoryInitTbl));
		memset(pHistory, 0, sizeof(*pHistory));
		HistorySave(pHistory);
	}
	
	return ERR_NONE;
}
// ��ʼ����ʷ��¼
t_err HistoryInit(void)
{
	HistoryLoad(&g_History);
	HistoryCheck(&g_History);
	
	return ERR_NONE;
}


