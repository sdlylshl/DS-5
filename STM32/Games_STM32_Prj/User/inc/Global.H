/*******************************************************************************
  Copyright (C), 2009-2011, YingHong, Ltd.
  File name:   	Global.H	
  Author:      	Xsky
  Version:     	1.0
  Date:		2011-02-24 09:53	
  
  Description:   全局定义
  Others:         
  Function List: 
  	1. ：
  	2. ：
  	3. ：
  	4. ：
  	5. ：

  History:        // 修改记录
    1. Date:		2009-04-08 16:25
       Author:		Xsky
       Modification:	建立本模块
*******************************************************************************/

#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#define HARD_VER		0x0022	// x.x.xx, 硬件版本
#define SOFT_VER		0x0001	// x.x.xx, 软件版本


/****************************************************************************/
// 编译开关

#define  RELEASE		0		// 1: 发布, 0:非发布

#if RELEASE != 1
#define  DEBUG	
#endif

#define IS_RELEASE()		(RELEASE == 1)
#define IS_N_RELEASE()		(RELEASE != 1)

#ifdef DEBUG
//#define	DBG_PORT_INIT		// 将全部端口初始化为输入
#define  DBG_UART1_LOOP	// Uart应用程序回环
#define  DBG_VKUART16_LOOP		// Uart应用程序回环

#define  DBG_CAN				// 调试CAN
#define	 DBG_CAN_QUE_MAX		// CAN调用记录队列最大值
#endif

#define  DBG_COLOR_STAT_F		// 颜色统计输出
#ifdef DBG_COLOR_STAT_F
#undef DEBUG
#endif

#ifdef DEBUG
#define  _DBG_(code)		code
#else
#define  _DBG_(code)		
#endif

#ifdef DEBUG
//extern uint8	DbgBuf[512];
#endif

#include "type.h"
#include "STM32_Type.h"
#include "Target.h"
#include "StateCtrl.h"

/***************************************************************************************************/
// 参数选择

/***************************************************************************************************/
// 错误代码定义
#define  ERR_NONE					0x0000
#define  ERR_BallNum				0x0100
#define  ERR_UndefColor				0x0101	/* 球颜色错误 */
#define  ERR_MainGameTimeOut		0x0102	/* 主游戏超时 */
#define  ERR_VacuumHousing			0x
#define  ERR_OutsideTrack			0x
#define  ERR_AccountLimit			0x
#define  ERR_Spin					0x
#define  ERR_PowerOnCheck			0x
#define  ERR_InsideTrack			0x

#define  ERR_InsideWaitBallTimeOut	0x0101	/* 等待球超时 */
#define  ERR_InsideLoopSwitchErr	0x0102	/* 循环/停止开关错误 */
#define  ERR_OutsideLoop1Err		0x0103	/* 第一圈超时 */
#define  ERR_OutsideLoop2Err		0x0104	/* 第二圈超时 */

#define  ERR_ParamCheck				0x0801	/* 参数检查错误 */

#define	 ERR_UsartTxQueueFull		0x0802	/* UART发送队列满 */
#define  ERR_UsartRxQueueFull		0x0803	/* UART接收队列满 */

#define  ERR_GameStateUndef			0x0902	/* 未定义游戏状态错误 */
#define  ERR_TimerUndef				0x0903	/* 未定义游戏状态错误 */
#define  ERR_UndefPlayerData		0x0904	/* 未定义游戏帧 */
#define  ERR_FrameError				0x0905	/* 帧错误 */
#define  ERR_SendToLong				0x0906	/* 欲发发送的字节数太多 */
#define  ERR_VSprintf				0x0907	/* vsprintf返回错误 */

#endif
