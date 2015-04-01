/****************************************Copyright (c)**************************************************
**                                  vixa
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: Uart.h
**创   建   人: Xsky
**创 建 日  期: 2011-03-28 11:31
**描        述: UART驱动相关定义、函数、声明
** 版  本: v1.0.00
**------------------------------------------------------------------------------------------------------
** 相关文件: Uart.c, 串口硬件相关配置需修改Uart.c文件
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
#ifndef __UART4_H__
#define __UART4_H__

#ifdef UART4_GLOBALS
#define  UART4_EXT
#else
#define  UART4_EXT	extern
#endif

/***************************************************************************************************/
// 常数定义
#define  UART4_BaudRate		115200	/* UART4波特率 */


/***************************************************************************************************/
// 函数声明
// 应用函数声明
// UART4发送
UART4_EXT t_err 		UART4_SendBytes(void *pSrc, t_size nBytes);
// UART4接收
UART4_EXT t_size 		UART4_RcvdSize(void);
UART4_EXT t_size_opt 	UART4_ReadBytes(void *pDst, t_size nBytes);


// 初始化配置函数
UART4_EXT t_err 		UART4_Config(void);


// 调试函数
UART4_EXT int 			UART4CanSendBytes(void);


/***************************************************************************************************/
// 全局量引用说明
#ifndef UART4_GLOBALS

#endif

#endif	// end: __UART4_H__

