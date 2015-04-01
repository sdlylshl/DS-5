/****************************************Copyright (c)**************************************************
**                                  2009-2011, YingHong, Ltd.
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: GameParam.c
**创   建   人: Xsky
**创 建 日  期: 2011-02-23 16:09
**描        述: 游戏参数相关定义及函数等
** 版本: v1.0.00
**------------------------------------------------------------------------------------------------------
** 相关文件: GameParam.h
**			 
**			 
**-------------------------------------------------------------------------------------------------------
**
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

#define  GAMEPARAM_GLOBALS
#include "Global.h"
#include "GameParam.h"
#include "string.h"
#include "Spi_FRAM.h"
#include "STM32_Function.h"

// 全局变量游戏参数
T_Param 	g_Param = {0, };
// 全局帐目错误,等记录数据
T_History	g_History = {0, };

/***************************************************************************************************/
//    函数

// 参数默认值定义
static __flash T_Param ParamInitTbl = 
{
	//uint16			MemFlag1;			// 标志位1
	PARAM_MemFlag1_Set,
	//uint16			ParamSize;			// 参数长度
	sizeof(T_Param),
	//uint16			HardVer;			// 硬件版本
	HARD_VER,
	//uint16			SoftVer;			// 软件版本号
	SOFT_VER,
	//uint32			WriteTimes;			// 本存储区域写次数
	0,
	//uint16			Check;				// CRC校验
	0x00,
	//uint16			MemFlag2;			// 标志位2
	PARAM_MemFlag2_Set
};

// 读取参数
t_err ParamLoad(T_Param *pParam)
{
	FRAM_Read(pParam, FRAM_PARAM_Addr, sizeof(*pParam));
	
	return ERR_NONE;
}
// 保存参数
t_err ParamSave(T_Param *pParam)
{
	pParam->WriteTimes++;
	pParam->MemFlag1 = PARAM_MemFlag1_Set;
	pParam->MemFlag2 = PARAM_MemFlag2_Set;
	pParam->Check = CRC_CalcBlock((uint8 *)pParam, STRUCT_OFFSET(T_History, Check));
	FRAM_Write(FRAM_PARAM_Addr, pParam, sizeof(*pParam));
	
	return ERR_NONE;
}
// 检查参数, 如果未初始化则初始化参数
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
// 参数初始化调用函数
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

// 读取历史记录
t_err HistoryLoad(T_History *pHistory)
{
	FRAM_Read(pHistory, FRAM_HISTORY_Addr, sizeof(*pHistory));
	
	return ERR_NONE;
}
/*************************************************************************************************** 
**	HistorySaveSub()
**	保存历史记录的某一部分或者全部字节
**  参数: 1. nOffset: 相对于历史数据结构体T_History的偏移量,实际定义的变量只有g_History
**		 2. nBytes: 要保存的字节数
**	返回: 错误码t_err
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
		FRAM_Write(r + OffsetAtHistory, p, nBytes);			// 保存区域i
		if(nBytes != sizeof(T_History))
		{
			FRAM_Write(r + STRUCT_OFFSET(T_History, Check), &pHistory->Check, sizeof(pHistory->Check));			// 保存区域1
		}
	}
	/*
	r = STRUCT_OFFSET(T_DataStore, History[0]);
	FRAM_Write(r + OffsetAtHistory, p, nBytes);			// 保存区域1
	if(nBytes != sizeof(T_History))
	{
		FRAM_Write(r + STRUCT_OFFSET(T_History, Check), &pHistory->Check, nBytes);			// 保存区域1
	}
	
	r = STRUCT_OFFSET(T_DataStore, History[1]);			// 保存区域2
	FRAM_Write(r + OffsetAtHistory, p, nBytes);
	if(nBytes != sizeof(T_History))
	{
		FRAM_Write(r + STRUCT_OFFSET(T_History, Check), &pHistory->Check, nBytes);			// 保存区域1
	}
	*/
	
	/* ?????:缺少检测是否写成功的判断 */
	
	return ERR_NONE;
}
// 保存历史记录
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
	/* ?????:缺少检测是否写成功的判断 */
	
	pHistory->MemFlag1 = HISTORY_MemFlag1_Set;
	pHistory->MemFlag2 = HISTORY_MemFlag2_Set;
	return HistorySaveSub(pHistory, 0, sizeof(*pHistory));
	
}
// 检查历史记录是否初始化,只检查标志位
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
// 初始化历史记录
t_err HistoryInit(void)
{
	HistoryLoad(&g_History);
	HistoryCheck(&g_History);
	
	return ERR_NONE;
}


