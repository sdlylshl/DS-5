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
#ifndef __UART_H__
#define __UART_H__

#ifdef UART_GLOBALS
#define  UART_EXT
#else
#define  UART_EXT	extern
#endif

/***************************************************************************************************/
// 常数定义
#define  UART1T4_Rx_TIM					TIM3	/* 接收超时定时器 */
#define  UART1T4_RX_TIM_OutCharN		(4.5)	/* <57, 设置UART超时字符数,设置的超时时间为:CharNx11/Baudrate秒 */

#define  USART4		UART4
#define  USART5		UART5
// 关发送中断
#define  UARTx_ENTER_CRITICALx_TX(x)		OS_ENTER_CRITICAL()//CLRBITs(USART##x->CR1, USART_FLAG_TC)	/* 关中断 */
// 开发送中断
#define  UARTx_EXIT_CRITICALx_TX(x)			OS_EXIT_CRITICAL()//SETBITs(USART##x->CR1, USART_FLAG_TC)	/* 开中断 */
// 关闭接收中断
#define  UARTx_ENTER_CRITICALx_RX(x)		OS_ENTER_CRITICAL()//CLRBITs(USART##x->CR1, USART_FLAG_RXNE)	/* 关中断 */
// 开接收中断
#define  UARTx_EXIT_CRITICALx_RX(x)			OS_EXIT_CRITICAL()//SETBITs(USART##x->CR1, USART_FLAG_RXNE)	/* 开中断 */
/* 关定时器通道y中断 */
#define  UART1T4_TIM_ENTER_CRITICALx(y)		CLRBITs(UART1T4_Rx_TIM->DIER, TIM_IT_CC##y)	
/* 开定时器通道y中断 */
#define  UART1T4_TIM_EXIT_CRITICALx(y)		SETBITs(UART1T4_Rx_TIM->DIER, TIM_IT_CC##y)	

typedef enum{c_InTime=0, c_TimeOut=1}t_TimeStatus;


/***************************************************************************************************/
// 函数声明

// 停止定时器
#define  UART1T4_Rx_TIM_Stop() 				CLRBITs(UART1T4_Rx_TIM->CR1, TIM_CR1_CEN)
// 清零定时计数器
#define  UART1T4_Rx_TIM_ClrCnt()			UART1T4_Rx_TIM->CNT = 0x0000
// 开启定时器
#define  UART1T4_Rx_TIM_Start()				SETBITs(UART1T4_Rx_TIM->CR1, TIM_CR1_CEN);

// 开启CCx通道,串口1到4使用定时器x通道1到4, 串口5到8使用定时器y通道1到4
#define  UART1T4_Rx_TIM_CC_Start(Ux,CCy)	UART1T4_TIM_ENTER_CRITICALx(CCy); \
											UART1T4_Rx_TIM->CCR##CCy = (uint16)UART1T4_Rx_TIM->CNT + (uint16)(UARTx_RX_TIM_OutCharN * 11 * 1000000 / UARTx_BaudRate); \
											Uart##Ux##_RxTimeOutFlag = c_InTime; \
											SETBITs(UART1T4_Rx_TIM->CCER, TIM_CCER_CC##CCy##E); \
											UART1T4_Rx_TIM_Start(); \
											UART1T4_TIM_EXIT_CRITICALx(CCy)
// 关闭CCx通道,串口1到4使用定时器x通道1到4, 串口5到8使用定时器y通道1到4
#define  UART1T4_Rx_TIM_CC_Stop(CCy)		CLRBITs(UART1T4_Rx_TIM->CCER, TIM_CCER_CC##CCy##E)
// 检查是否超时,n串口号1~n
#define  UART1T4_Rx_isTimeOut(n)			(Uart##n##_RxTimeOutFlag == c_TimeOut)

// 定时器初始化函数
extern void UART1T4_Rx_TIM_Config(void);


/***************************************************************************************************/
// 全局量引用说明
#ifndef UART_GLOBALS
extern uint8 Uart1_RxTimeOutFlag;
extern uint8 Uart2_RxTimeOutFlag;
extern uint8 Uart3_RxTimeOutFlag;
extern uint8 Uart4_RxTimeOutFlag;

#endif


#endif	// end: __UART_H__

