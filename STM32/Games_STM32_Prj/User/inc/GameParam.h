/****************************************Copyright (c)**************************************************
**                                  vixa
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: GameParam.h
**创   建   人: Xsky
**创 建 日  期: 2011-02-23 16:09
**描        述: 游戏参数相关定义及函数等
** 版  本: v1.0.00
**------------------------------------------------------------------------------------------------------
** 相关文件: GameParam.c
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
#ifndef __GAME_PARAM_H__
#define __GAME_PARAM_H__

#ifdef GAMEPARAM_GLOBALS
#define GAMEPARAM_EXT
#else
#define GAMEPARAM_EXT		extern
#endif

/***************************************************************************************************/
// 关键函数定义
/* 关闭中断 */
//#define  OS_ENTER_CRITICAL()		
/* 开中断 */
//#define  OS_EXIT_CRITICAL()			

/***************************************************************************************************/
// 常数定义
#define MAX_PLAYER				12		/* 分机数量 */
#define PLAYER_MAX_NO			12		/* 分机数量 */
#define BET_ITEMS				5		/* 下注项数,2:2:2:2:1 */
#define BALL_NUM				9		/* 球数量 */
#define BALL_WAGER_NUM			1		/* 押球数 */

/***************************************************************************/

// 主机错误次数
typedef struct T_HostErrors_{
	uint16	BallNum;					// 球数量错误
	uint16	BallColor;					// 球颜色错误
	uint16	MainGameTimeOut;			// 主游戏超时
	uint16	VacuumHousing;				// 真空置开启
	uint16	OutsideTrack;				// 外圈感应器错误
	uint16	AccountLimit;				// 帐目记录限制
	uint16	Spin;						// 机台摇晃错误
	uint16	PowerOnCheck;				// 开机检查超时
	uint16	InsideTrack;				// 球停滞内圈
}T_HostErrors;
// 分机错误
typedef struct T_PlayerErrorsName_{
	uint16	CoinInBlocked;				// 投币器卡币
	uint16	CoinOutBlocked;				// 退币器卡币
	uint16	CoinOutBoxEmpty;			// 退币器空
	uint16	TicketBlocked;				// 彩票机卡住
	uint16	TicketEmpty;				// 彩票机空
	uint16	Credit;						// Credit错误
}T_PlayerErrorsName;
typedef union T_PlayerErrors_{
	uint16	Indexs[6];					// 索引
T_PlayerErrorsName	Names;				// 名称
}T_PlayerErrors;
// 所有错误
typedef struct T_GameErrors_{
	T_HostErrors	Host;				// 主机错误
	T_PlayerErrors 	Player[BET_ITEMS];	// 分机错误
}T_GameErrors;

/***************************************************************************/
/***************************************************************************/
// 系统参数
//typedef struct T_SysParam_{
//}T_SysParam;

/***************************************************************************/

typedef uint32 t_CanErrN;
typedef struct T_SysErrors_{
	// CAN_错误
	uint32	CAN_LEC[6];					// CAN各种错误状态次数:位填充,格式,确认,隐性,显性,CRC
	uint32	CAN_BOF;					// CAN离线错误次数
	uint32	CAN_EPV;					// CAN错误被动次数
	uint32	CAN_EWG;					// CAN错误警告标志次数
	//uint32	CAN_TX;					// CAN发送错误
	//uint32	CAN_ALST;				// CAN由于仲裁引起的错误
	//uint32	CAN_FOVR;				// CAN溢出错误
	uint32	CAN_SendQueOVF;				// CAN发送队列溢出
	uint32	CAN_RcvQueOVF;				// CAN接收队列溢出
	#if IS_N_RELEASE()
	uint8	CAN_SendQueMaxN;			// CAN发送队列最大值
	uint8	CAN_RcvQueMaxN;				// CAN接收队列最大元素个数
	#endif
}T_SysErrors;

/***************************************************************************/
#define	PARAM_MemFlag1_Set				0xA5E7
#define PARAM_MemFlag2_Set				0x5B5A
typedef struct T_Param_{
	uint16			MemFlag1;			// 标志位1
	uint16			ParamSize;			// 参数长度
	uint16			HardVer;			// 硬件版本
	uint16			SoftVer;			// 软件版本号
	uint32			WriteTimes;			// 本存储区域写次数
	
	uint16			MemFlag2;			// 标志位2
	uint16			Check;				// CRC校验
}T_Param;
//#define  PARAM_Ofst(x)		(uint32)(&(((T_Param *)0)->x))
//#define  PARAM_Size(x,y)	( PARAM_Ofst(y) - PARAM_Ofst(x) )

#define	HISTORY_MemFlag1_Set				0x5AC7
#define HISTORY_MemFlag2_Set				0xACA5
typedef struct T_History_{
	uint16			MemFlag1;			// 标志位1
	
	uint32			WriteTimes;			// 本存储区域写次数,最大:4,294,967,295次(42亿)
	
	T_GameErrors	GameErrors;			// 游戏错误
	T_SysErrors		SysErrors;			// 系统错误,驱动错误等
	
	uint16			Check;				// CRC校验
	uint16			MemFlag2;			// 标志位2
}T_History;

typedef struct T_RunTimer_{
	uint32			TotalRunSec;		// 机台总共运行时间
	uint32			ReportRunSec;		// 机台由上次报码至现在的运行时间
}T_RunTimer;

//#define  PARAM_Ofst(x)		(uint32)(&(((T_Param *)0)->x))
//#define  PARAM_Size(x,y)	( PARAM_Ofst(y) - PARAM_Ofst(x) )
#define HISTORY_SAVE_Areas		2
typedef struct T_DataStore_{			// 定义数据在FRAM内的存储结构
	uint32			Blank0[4];			// 保留空间
	T_Param			Param;				// 参数存储区
	uint32			Blank1[1];			// 保留空间
	uint32			*pHistory;			// 
	T_History		History[HISTORY_SAVE_Areas];// 帐目等数据
}T_DataStore;
// FRAM参数存储基地址
#define  FRAM_PARAM_Addr		STRUCT_OFFSET(T_DataStore, Param)
// FRAM历史记录存储基地址
#define  FRAM_HISTORY_Addr		STRUCT_OFFSET(T_DataStore, History[0])
#define  FRAM_HISTORY_Bak_Addr	STRUCT_OFFSET(T_DataStore, History[1])

/***************************************************************************************************/
// 函数声明

// 参数初始化调用函数
GAMEPARAM_EXT t_err ParamInit(void);
// 初始化历史记录
GAMEPARAM_EXT t_err HistoryInit(void);

/* 保存参数 */
GAMEPARAM_EXT t_err ParamSave(T_Param *pParam);
/* 保存历史记录数据 */
GAMEPARAM_EXT t_err HistorySave(T_History *pHistory);

/********************************************************************************************
** ?????: 数据保存需要修改为下以机制
** 因SPI是不可共享硬件资源,有可能在main()程序中或者接收处理中断中均会有保存数据的动作
** 如main函数中调用Game()函数对押分结果处理完成后需要保存History数据
** 而接收处理中断中会处理上下分投退币等数据也需要保存History数据
** 所以目前直接调用的机制存在可重入性问题
** 基于考虑效率及入口唯一，可以采取如下的机制
** (1). 定义一个FIFO队列，队列元素为:各个进程需要保存的数据缓冲区指针，数据长度，禁止/允许写标记，
**	   优先级，错误处理方式, 错误提示信息指针头(可为NULL，不显示提示，提示与TFT或者LCD相关联，
**	   对关键数据如保存玩家分数失败等重要信息进行停机等操作，而其它如游戏记录则可不进行提示)，
**	   保存数据前需要进行操作的色子函数
** (2). 某个进程需要写FRAM时，检查队列中是否有重复的指针或包含关系，若无则将要写的数据指针压入FIFO
** (3). 增加一个软件中断服务函数,函数为直接操作FRAM的代码
** (4). 定义FRAM操作的状态机, 包含整个写FRAM需要的过程，如CS低，发送命令码开始/完成数据发送完成等等
** (5). 增加一个g_History状态防止数据被多个进程写g_History造成校验值不准确/如果不考虑校验可以不关注
** (6). 使用DMA发送连续数据
** 当写FRAM需要等待时
** 
*********************************/
/* 保存历史数据中指定偏移量的数据 */
#define  HISTORY_SaveSub(subName)	HistorySaveSub(&g_History, STRUCT_OFFSET(T_History,subName), STRUCT_SIZEOF(T_History, subName))
#define  HISTORY_SaveAll()			HistorySave(&g_History)
GAMEPARAM_EXT t_err HistorySaveSub(T_History *pHistory, uint32 OffsetAtHistory, uint32 nBytes);

/***************************************************************************************************/
// 全局量引用说明
#ifndef GAMEPARAM_GLOBALS
extern const T_Param ParamInitTbl;				// 参数初始化表
extern T_Param 	g_Param;						// 全局参数
extern T_History	g_History;					// 全局帐目错误,等记录数据
#endif		// end: #ifndef GAME_GLOBALS


#endif		// end: __GAME_PARAM_H__ 

