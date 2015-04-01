/****************************************Copyright (c)**************************************************
**                                  vixa
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: StateCtrl.c
**��   ��   ��: Xsky
**�� �� ��  ��: 2011-06-15 10:30
**��        ��: ϵͳ����״̬���弰��ز�������
** ��  ��: v1.0.00
**------------------------------------------------------------------------------------------------------
** ����ļ�: StateCtrl.c
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
#ifndef __STATECTRL_H__
#define __STATECTRL_H__

#ifdef STATECTRL_GLOBALS
#define  STATECTRL_EXT
#else
#define  STATECTRL_EXT	extern
#endif


/***************************************************************************************************/
// ��������
// ״̬����
#define  SYS_STATE_Init				0		/* ϵͳ��ʼ��״̬ */
#define  SYS_STATE_Game				1		/* ϵͳ������Ϸ״̬ */
#define  SYS_STATE_Config			2		/* ϵͳ����˵�����״̬ */
#define  SYS_STATE_Top				2		/* ���״ֵ̬ */
#define  SYS_STATE_Error			0xFF	/* ϵͳ�������״̬ */

/***************************************************************************************************/
// ���Ͷ���
typedef t_ureg_opt t_sysState;

/***************************************************************************************************/
// ȫ��������˵��
#ifndef STATECTRL_GLOBALS
extern t_sysState g_SysState;
extern t_sysState g_SysStateReq;

#endif


/***************************************************************************************************/
// ��������
/* ����״̬ */
#define  SYS_StateSet(s)	OS_ENTER_CRITICAL(); \
							g_SysState = s; \
							OS_EXIT_CRITICAL();
/* ֱ�ӻ�ȡȫ��״̬�� */
#define  SYS_StateGet()		g_SysState
/* ���жϻ�ȡȫ���ж��� */
#define  SYS_StateRead(r)	OS_ENTER_CRITICAL(); \
							r = g_SysState; \
							OS_EXIT_CRITICAL();
/* ��������״̬ */
#define  SYS_ReqStateSet(s)	OS_ENTER_CRITICAL(); \
							g_SysStateReq = s; \
							OS_EXIT_CRITICAL();
/* ֱ�ӻ�ȡȫ��״̬�� */
#define  SYS_ReqStateGet()	g_SysStateReq
/* ���жϻ�ȡȫ���ж��� */
#define  SYS_ReqStateRead(r) OS_ENTER_CRITICAL(); \
							r = g_SysStateReq; \
							OS_EXIT_CRITICAL();

STATECTRL_EXT t_bool SysIsReqNextState(void);

#endif	// end: __STATECTRL_H__

