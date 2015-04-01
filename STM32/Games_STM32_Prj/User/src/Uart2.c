/****************************************Copyright (c)**************************************************
**                                  vixa
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: UART2.C
**创   建   人: 翻訳の目
**创 建 日  期: 2011-11-01 17:27
**描        述: UART驱动函数, 包含大部分硬件相关配置定义
**				文件内UARTx_开头的函数为公用函数移植到其它UART时一般不用修改直接使用
**				使用了DMA或者中断接收发送
**				使用了定时器中断检测字符超时
**			   注: 要求 CPU_HCLK >= 1MHz
** 版  本: v1.0.00
**------------------------------------------------------------------------------------------------------
** 相关文件: UART1.h, Uart.C, Uart.h
**------------------------------------------------------------------------------------------------------
** 最后修改日期: 
** 修改人: 
** 版本: v1.0.00
**-------------------------------------------------------------------------------------------------------
** 修改人: 
** 日期: 
** 版本: v1.0.00
** 修改: 
**			1. 
**			2. 
**			3. 
**			4. 
**			5. 
********************************************************************************************************/

#define  UART1_GLOBALS
#include "Global.h"
#include "Uart.h"
#include "UART2.h"
#include "Stm32f10x.h"
#include "Stm32f10x_rcc.h"
#include "Target.h"
#include "Queue.h"

#if UART2_EN > 0	

/***************************************************************************************************/
// 常数定义, 应用于其它UART时需要修改本段定义
// 中断服务函数名称来自于Startup_stm32f10x_hd.s的向量表定义

// 串口号
#define  UARTx_NO					2						/* 串口编号1~n */
// 编译开关
#define  UARTx_TX_EN				1						/* 允许发送 */
#define  UARTx_RX_EN				1						/* 允许接收 */

// 缓冲区设置
#ifdef DEBUG
#define  UARTx_Tx_BufSize			512						/* 发送缓冲区字节数，包含队列指针结构 */
#else
#define  UARTx_Tx_BufSize			128						/* 发送缓冲区字节数，包含队列指针结构 */
#endif
#define  UARTx_Rx_BufSize			128						/* 接收缓冲区字节数，包含队列指针结构 */
#define  UARTx_Rx_DMABufSize		32						/* 接收DMA缓冲区长度,使用DMA中转接收数据 */
// 串口参数
#define  UARTx_BaudRate				UART2_BaudRate			/* 波特率 */
#define  UARTx_WordLength			USART_WordLength_9b		/* 数据位 */
#define  UARTx_StopBits				USART_StopBits_1		/* 停止位 */
#define  UARTx_Parity				USART_Parity_Odd		/* Odd:奇校验 */
#define  UARTx_HardwareFlowControl	USART_HardwareFlowControl_None	/* 硬件流控制 */
#define  UARTx_Mode					(USART_Mode_Rx | USART_Mode_Tx)	/* 收/发模式 */
#define  UARTx_Config				UART2_Config			/* 配置函数名 */
#define  UARTx_SendBytes			UART2_SendBytes			/* 发送函数名 */
#define  UARTx_RcvdSize				UART2_RcvdSize			/* 接收字节数 */
#define  UARTx_ReadBytes			UART2_ReadBytes			/* 接收函数名 */
// 引脚及内核相关定义
#define  UARTx						USART2			/* USART_TypeDef定义的UART基地址 */
//#define  UARTx_RCC_APB				2						/* UARTx所连接的APB号:1或者2 */
//#define  UARTx_RCC					RCC_APB2Periph_USART1	/* UARTx时钟 */
//#define  UARTx_IRQn					USART1_IRQn				/* UARTx中断向量号 */
#define  UARTx_NVIC_PriorityGroup	NVIC_PriorityGroup_USART2
#define  UARTx_NVIC_Priority		0
#define  UARTx_NVIC_PrioritySub		0
void	 USART2_IRQHandler(void);
#define  UARTx_IRQHandler			USART2_IRQHandler		/* UARTx中断处理函数名称, 注意为UART/USART */
#define  UARTx_GPIO					GPIOA					/* UARTx引脚所使用的端口 */
#define  UARTx_GPIO_RCC				RCC_APB2Periph_GPIOA	/* GPIOA RCC 名称定义 */
#define  UARTx_GPIO_Pin_Tx			GPIO_Pin_2				/* 根据硬件定义 */
#define  UARTx_GPIO_Pin_Rx			GPIO_Pin_3				/* 根据硬件定义 */
// DMA定义
#define  UARTx_DMA_EN				1						/* 1:启用DMA,0:不使用DMA,1:直接使用UART中断收发数据 */
#define  UARTx_DMA_RX_EN			1						/* 1:启用DMA接收中断,0:使用UART中断接收数据,一般不使用DMA接收数据 */
#define  UARTx_RCC_DMA				RCC_AHBPeriph_DMA1		/* 定义则开户DMA */
#define  UARTx_DMA					DMA1					/* 定义DMA寄存器 */
#define  UARTx_DMA_Rx_Channel		DMA1_Channel6			/* UART.RX对应的DMA通道 */
#define  UARTx_DMA_Rx_NVICPrioGroup	NVIC_PriorityGroup_3	/* 中断向量优先级组 */
#define  UARTx_DMA_Rx_IRQn			DMA1_Channel6_IRQn		/* 中断向量号 */
#define  UARTx_DMA_Rx_IRQPrio		0						/* 中断主优先级 */
#define  UARTx_DMA_Rx_IRQPrioSub	0						/* 中断次优先级 */
#define  UARTx_DMA_Rx_Priority		DMA_Priority_Medium		/* Rx优先级 */
#define  UARTx_DMA_Rx_IF_TC			DMA1_FLAG_TC6			/* 定义通道中断标志 */
#define  UARTx_DMA_Rx_IF_GL			DMA1_FLAG_GL6			/* 全局中断标志 */
#define  UARTx_DMA_Rx_IF_HT			DMA1_FLAG_HT6			/* 半传输完成中断标志 */
void 	 DMA1_Channel6_IRQHandler(void);
#define  UARTx_DMA_Rx_IRQHandler	DMA1_Channel6_IRQHandler	/* Rx中断处理函数名,注意此名称定义无效必须更改函数正式说明 */
// 接收超时定时器
#define  UARTx_RX_TIM_EN			0						/* 开启接收字符超时中断 */
#define  UARTx_RX_TIM_OutCharN		(4.5)					/* <57, 设置UART超时字符数,设置的超时时间为:CharNx11/Baudrate秒 */
#define  UARTx_RX_TIM_CC_EN			0						/* 设置为1则定时器使用公用文件Uart.c中使用输出比较模式,0:则本文件使用单独的定时器,在本文件中定义定时器 */
#if UARTx_RX_TIM_CC_EN > 0
#define  UARTx_Rx_TIM_Start()		UART1T4_Rx_TIM_CC_Start(2, 2)	/* 开启定时器通道 */
#define  UARTx_Rx_isTimeOut()		UART1T4_Rx_isTimeOut(2)			/* 关闭定时器通道 */
#else
//#define  UARTx_Rx_TIM				TIM6					/* 接收超时定时器 */
//#define  UARTx_Rx_TIM_RCCCmd(s) 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, s)/* 时钟 */
//#define  UARTx_Rx_TIM_NVICPrioGroup	NVIC_PriorityGroup_4	/* 定时器中断优先级组 */
//#define  UARTx_Rx_TIM_IRQn			TIM6_IRQn				/* 定时器中断向量号 */
//#define  UARTx_Rx_TIM_IRQPrio		0						/* 定时器中断优先级 */
//#define  UARTx_Rx_TIM_IRQPrioSub	0						/* 定时器中断子优先级 */
void 	 TIM6_IRQHandler(void);
#define  UARTx_Rx_TIM_IRQHandler	TIM6_IRQHandler			/* 定时器中断处理函数 */
#endif	 // end: #if UARTx_RX_TIM_CC_EN == 0 //
#define  UARTx_DMA_Tx_Channel		DMA1_Channel7			/* DMA通道 */
#define  UARTx_DMA_Tx_NVICPrioGroup	NVIC_PriorityGroup_3	/* 中断向量优先级组 */
#define  UARTx_DMA_Tx_IRQn			DMA1_Channel7_IRQn		/* 中断向量号 */
#define  UARTx_DMA_Tx_IRQPrio		0						/* 中断主优先级 */
#define  UARTx_DMA_Tx_IRQPrioSub	0						/* 中断次优先级 */
#define  UARTx_DMA_Tx_Priority		DMA_Priority_Low		/* Tx优先级 */
#define  UARTx_DMA_Tx_IF_TC			DMA1_FLAG_TC7			/* 定义通道中断标志 */
#define  UARTx_DMA_Tx_IF_TE			DMA1_FLAG_TE7			/* 传输错误中断标志 */
#define  UARTx_DMA_Tx_IF_GL			DMA1_FLAG_GL7			/* 全局中断标志 */
#define  UARTx_DMA_Tx_Number		0x0107					/* 高字节为DMA号1/2, 低字节为通道号 */
void 	 DMA1_Channel7_IRQHandler(void);
#define  UARTx_DMA_Tx_IRQHandler	DMA1_Channel7_IRQHandler	/* Tx中断处理函数名 */
// 中断管理设置
#if UARTx_DMA_EN > 0
#define  UARTx_ENTER_CRITICAL_TX()	OS_ENTER_CRITICAL()//CLRBITs(UARTx_DMA_Tx_Channel->CCR, DMA_IT_TC)	/* CLRBITs(UARTx->CR1, USART_FLAG_TC), 关中断 */
#define  UARTx_EXIT_CRITICAL_TX()	OS_EXIT_CRITICAL()//SETBITs(UARTx_DMA_Tx_Channel->CCR, DMA_IT_TC)	/* SETBITs(UARTx->CR1, USART_FLAG_TC), 开中断 */
#else
#define  UARTx_ENTER_CRITICAL_TX()	UARTx_ENTER_CRITICALx_TX(2)	/* CLRBITs(UARTx->CR1, USART_FLAG_TC)	, 关中断 */
#define  UARTx_EXIT_CRITICAL_TX()	UARTx_EXIT_CRITICALx_TX(2)	/* SETBITs(UARTx->CR1, USART_FLAG_TC)	 开中断 */
#endif
#define  UARTx_ENTER_CRITICAL_RX()	UARTx_ENTER_CRITICALx_RX(2)	/* CLRBITs(UARTx->CR1, USART_FLAG_RXNE), 关中断 */
#define  UARTx_EXIT_CRITICAL_RX()	UARTx_EXIT_CRITICALx_RX(2)	/* SETBITs(UARTx->CR1, USART_FLAG_RXNE), 开中断 */


/***************************************************************************************************/

#if 1==0
static const USART_InitTypeDef USART_InitTbl = {
	UARTx_BaudRate /* uint32_t USART_BaudRate;  */  , 
	UARTx_WordLength /* uint16_t USART_WordLength; */ ,
	UARTx_StopBits /* uint16_t USART_StopBits;   */ ,
	UARTx_Parity /* uint16_t USART_Parity; */	 ,
	UARTx_Mode /* uint16_t USART_Mode; */		 ,
	UARTx_HardwareFlowControl /* uint16_t USART_HardwareFlowControl; */
};
#endif

#include "UARTx.h"

#ifdef DEBUG
// 获取Uart1空闲缓冲区字节数
int Uart2CanSendBytes(void)
{
	int N;
	N = pTxBuf->MaxData- pTxBuf->NData;
	return N;
}

#endif

#endif		// end: #if UART2_EN > 0  //
