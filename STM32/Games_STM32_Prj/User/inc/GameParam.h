/****************************************Copyright (c)**************************************************
**                                  vixa
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: GameParam.h
**��   ��   ��: Xsky
**�� �� ��  ��: 2011-02-23 16:09
**��        ��: ��Ϸ������ض��弰������
** ��  ��: v1.0.00
**------------------------------------------------------------------------------------------------------
** ����ļ�: GameParam.c
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
#ifndef __GAME_PARAM_H__
#define __GAME_PARAM_H__

#ifdef GAMEPARAM_GLOBALS
#define GAMEPARAM_EXT
#else
#define GAMEPARAM_EXT		extern
#endif

/***************************************************************************************************/
// �ؼ���������
/* �ر��ж� */
//#define  OS_ENTER_CRITICAL()		
/* ���ж� */
//#define  OS_EXIT_CRITICAL()			

/***************************************************************************************************/
// ��������
#define MAX_PLAYER				12		/* �ֻ����� */
#define PLAYER_MAX_NO			12		/* �ֻ����� */
#define BET_ITEMS				5		/* ��ע����,2:2:2:2:1 */
#define BALL_NUM				9		/* ������ */
#define BALL_WAGER_NUM			1		/* Ѻ���� */

/***************************************************************************/

// �����������
typedef struct T_HostErrors_{
	uint16	BallNum;					// ����������
	uint16	BallColor;					// ����ɫ����
	uint16	MainGameTimeOut;			// ����Ϸ��ʱ
	uint16	VacuumHousing;				// ����ÿ���
	uint16	OutsideTrack;				// ��Ȧ��Ӧ������
	uint16	AccountLimit;				// ��Ŀ��¼����
	uint16	Spin;						// ��̨ҡ�δ���
	uint16	PowerOnCheck;				// ������鳬ʱ
	uint16	InsideTrack;				// ��ͣ����Ȧ
}T_HostErrors;
// �ֻ�����
typedef struct T_PlayerErrorsName_{
	uint16	CoinInBlocked;				// Ͷ��������
	uint16	CoinOutBlocked;				// �˱�������
	uint16	CoinOutBoxEmpty;			// �˱�����
	uint16	TicketBlocked;				// ��Ʊ����ס
	uint16	TicketEmpty;				// ��Ʊ����
	uint16	Credit;						// Credit����
}T_PlayerErrorsName;
typedef union T_PlayerErrors_{
	uint16	Indexs[6];					// ����
T_PlayerErrorsName	Names;				// ����
}T_PlayerErrors;
// ���д���
typedef struct T_GameErrors_{
	T_HostErrors	Host;				// ��������
	T_PlayerErrors 	Player[BET_ITEMS];	// �ֻ�����
}T_GameErrors;

/***************************************************************************/
/***************************************************************************/
// ϵͳ����
//typedef struct T_SysParam_{
//}T_SysParam;

/***************************************************************************/

typedef uint32 t_CanErrN;
typedef struct T_SysErrors_{
	// CAN_����
	uint32	CAN_LEC[6];					// CAN���ִ���״̬����:λ���,��ʽ,ȷ��,����,����,CRC
	uint32	CAN_BOF;					// CAN���ߴ������
	uint32	CAN_EPV;					// CAN���󱻶�����
	uint32	CAN_EWG;					// CAN���󾯸��־����
	//uint32	CAN_TX;					// CAN���ʹ���
	//uint32	CAN_ALST;				// CAN�����ٲ�����Ĵ���
	//uint32	CAN_FOVR;				// CAN�������
	uint32	CAN_SendQueOVF;				// CAN���Ͷ������
	uint32	CAN_RcvQueOVF;				// CAN���ն������
	#if IS_N_RELEASE()
	uint8	CAN_SendQueMaxN;			// CAN���Ͷ������ֵ
	uint8	CAN_RcvQueMaxN;				// CAN���ն������Ԫ�ظ���
	#endif
}T_SysErrors;

/***************************************************************************/
#define	PARAM_MemFlag1_Set				0xA5E7
#define PARAM_MemFlag2_Set				0x5B5A
typedef struct T_Param_{
	uint16			MemFlag1;			// ��־λ1
	uint16			ParamSize;			// ��������
	uint16			HardVer;			// Ӳ���汾
	uint16			SoftVer;			// ����汾��
	uint32			WriteTimes;			// ���洢����д����
	
	uint16			MemFlag2;			// ��־λ2
	uint16			Check;				// CRCУ��
}T_Param;
//#define  PARAM_Ofst(x)		(uint32)(&(((T_Param *)0)->x))
//#define  PARAM_Size(x,y)	( PARAM_Ofst(y) - PARAM_Ofst(x) )

#define	HISTORY_MemFlag1_Set				0x5AC7
#define HISTORY_MemFlag2_Set				0xACA5
typedef struct T_History_{
	uint16			MemFlag1;			// ��־λ1
	
	uint32			WriteTimes;			// ���洢����д����,���:4,294,967,295��(42��)
	
	T_GameErrors	GameErrors;			// ��Ϸ����
	T_SysErrors		SysErrors;			// ϵͳ����,���������
	
	uint16			Check;				// CRCУ��
	uint16			MemFlag2;			// ��־λ2
}T_History;

typedef struct T_RunTimer_{
	uint32			TotalRunSec;		// ��̨�ܹ�����ʱ��
	uint32			ReportRunSec;		// ��̨���ϴα��������ڵ�����ʱ��
}T_RunTimer;

//#define  PARAM_Ofst(x)		(uint32)(&(((T_Param *)0)->x))
//#define  PARAM_Size(x,y)	( PARAM_Ofst(y) - PARAM_Ofst(x) )
#define HISTORY_SAVE_Areas		2
typedef struct T_DataStore_{			// ����������FRAM�ڵĴ洢�ṹ
	uint32			Blank0[4];			// �����ռ�
	T_Param			Param;				// �����洢��
	uint32			Blank1[1];			// �����ռ�
	uint32			*pHistory;			// 
	T_History		History[HISTORY_SAVE_Areas];// ��Ŀ������
}T_DataStore;
// FRAM�����洢����ַ
#define  FRAM_PARAM_Addr		STRUCT_OFFSET(T_DataStore, Param)
// FRAM��ʷ��¼�洢����ַ
#define  FRAM_HISTORY_Addr		STRUCT_OFFSET(T_DataStore, History[0])
#define  FRAM_HISTORY_Bak_Addr	STRUCT_OFFSET(T_DataStore, History[1])

/***************************************************************************************************/
// ��������

// ������ʼ�����ú���
GAMEPARAM_EXT t_err ParamInit(void);
// ��ʼ����ʷ��¼
GAMEPARAM_EXT t_err HistoryInit(void);

/* ������� */
GAMEPARAM_EXT t_err ParamSave(T_Param *pParam);
/* ������ʷ��¼���� */
GAMEPARAM_EXT t_err HistorySave(T_History *pHistory);

/********************************************************************************************
** ?????: ���ݱ�����Ҫ�޸�Ϊ���Ի���
** ��SPI�ǲ��ɹ���Ӳ����Դ,�п�����main()�����л��߽��մ����ж��о����б������ݵĶ���
** ��main�����е���Game()������Ѻ�ֽ��������ɺ���Ҫ����History����
** �����մ����ж��лᴦ�����·�Ͷ�˱ҵ�����Ҳ��Ҫ����History����
** ����Ŀǰֱ�ӵ��õĻ��ƴ��ڿ�����������
** ���ڿ���Ч�ʼ����Ψһ�����Բ�ȡ���µĻ���
** (1). ����һ��FIFO���У�����Ԫ��Ϊ:����������Ҫ��������ݻ�����ָ�룬���ݳ��ȣ���ֹ/����д��ǣ�
**	   ���ȼ���������ʽ, ������ʾ��Ϣָ��ͷ(��ΪNULL������ʾ��ʾ����ʾ��TFT����LCD�������
**	   �Թؼ������籣����ҷ���ʧ�ܵ���Ҫ��Ϣ����ͣ���Ȳ���������������Ϸ��¼��ɲ�������ʾ)��
**	   ��������ǰ��Ҫ���в�����ɫ�Ӻ���
** (2). ĳ��������ҪдFRAMʱ�����������Ƿ����ظ���ָ��������ϵ��������Ҫд������ָ��ѹ��FIFO
** (3). ����һ������жϷ�����,����Ϊֱ�Ӳ���FRAM�Ĵ���
** (4). ����FRAM������״̬��, ��������дFRAM��Ҫ�Ĺ��̣���CS�ͣ����������뿪ʼ/������ݷ�����ɵȵ�
** (5). ����һ��g_History״̬��ֹ���ݱ��������дg_History���У��ֵ��׼ȷ/���������У����Բ���ע
** (6). ʹ��DMA������������
** ��дFRAM��Ҫ�ȴ�ʱ
** 
*********************************/
/* ������ʷ������ָ��ƫ���������� */
#define  HISTORY_SaveSub(subName)	HistorySaveSub(&g_History, STRUCT_OFFSET(T_History,subName), STRUCT_SIZEOF(T_History, subName))
#define  HISTORY_SaveAll()			HistorySave(&g_History)
GAMEPARAM_EXT t_err HistorySaveSub(T_History *pHistory, uint32 OffsetAtHistory, uint32 nBytes);

/***************************************************************************************************/
// ȫ��������˵��
#ifndef GAMEPARAM_GLOBALS
extern const T_Param ParamInitTbl;				// ������ʼ����
extern T_Param 	g_Param;						// ȫ�ֲ���
extern T_History	g_History;					// ȫ����Ŀ����,�ȼ�¼����
#endif		// end: #ifndef GAME_GLOBALS


#endif		// end: __GAME_PARAM_H__ 

