/****************************************Copyright (c)**************************************************
**                                  vixa
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: StateCtrl.c
**创   建   人: Xsky
**创 建 日  期: 2011-06-15 10:30
**描        述: 系统运行状态定义及相关操作函数
** 版  本: v1.0.00
**------------------------------------------------------------------------------------------------------
** 相关文件: StateCtrl.c
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
#ifndef __STATECTRL_H__
#define __STATECTRL_H__

#ifdef STATECTRL_GLOBALS
#define  STATECTRL_EXT
#else
#define  STATECTRL_EXT	extern
#endif


/***************************************************************************************************/
// 常数定义
// 状态定义
#define  SYS_STATE_Init				0		/* 系统初始化状态 */
#define  SYS_STATE_Game				1		/* 系统进入游戏状态 */
#define  SYS_STATE_Config			2		/* 系统进入菜单设置状态 */
#define  SYS_STATE_Top				2		/* 最大状态值 */
#define  SYS_STATE_Error			0xFF	/* 系统进入错误状态 */

/***************************************************************************************************/
// 类型定义
typedef t_ureg_opt t_sysState;

/***************************************************************************************************/
// 全局量引用说明
#ifndef STATECTRL_GLOBALS
extern t_sysState g_SysState;
extern t_sysState g_SysStateReq;

#endif


/***************************************************************************************************/
// 函数声明
/* 设置状态 */
#define  SYS_StateSet(s)	OS_ENTER_CRITICAL(); \
							g_SysState = s; \
							OS_EXIT_CRITICAL();
/* 直接获取全局状态量 */
#define  SYS_StateGet()		g_SysState
/* 关中断获取全局中断量 */
#define  SYS_StateRead(r)	OS_ENTER_CRITICAL(); \
							r = g_SysState; \
							OS_EXIT_CRITICAL();
/* 设置请求状态 */
#define  SYS_ReqStateSet(s)	OS_ENTER_CRITICAL(); \
							g_SysStateReq = s; \
							OS_EXIT_CRITICAL();
/* 直接获取全局状态量 */
#define  SYS_ReqStateGet()	g_SysStateReq
/* 关中断获取全局中断量 */
#define  SYS_ReqStateRead(r) OS_ENTER_CRITICAL(); \
							r = g_SysStateReq; \
							OS_EXIT_CRITICAL();

STATECTRL_EXT t_bool SysIsReqNextState(void);

#endif	// end: __STATECTRL_H__

