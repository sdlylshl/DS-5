/****************************************Copyright (c)**************************************************
**                              2009-2009, YingHong, Ltd.
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: CAN.h
**创   建   人: Xsky
**创 建 日  期: 2011-02-25 14:45
**描        述: CAN相关类型及常数定义，驱动函数，中断服务函数
** 版  本: v1.0.00
**------------------------------------------------------------------------------------------------------
** 相关文件: 
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
#ifndef __CAN_H__
#define __CAN_H__

#ifdef CAN_GLOBALS
#define  CAN_EXT
#else
#define  CAN_EXT	extern
#endif

#include "Global.h"

/***************************************************************************************************/
// 常数定义
#define  CAN_BPS		500000		/* CAN通信波特率定义 */

#define  CAN_QUE_RX_Nframe	100		/* CAN接收队列帧数目,以结构体T_CanFrame为单元来计算 */
#define  CAN_QUE_TX_Nframe	100		/* CAN发送队列帧数目,以结构体T_CanFrame为单元来计算 */

#define  CAN_QUEUE_WRITE_FULL_EN		1	/* 1: 开启队列写满调用函数 */

#define  CAN_ID_TYPE		CAN_ID_EXT		/* 定义CAN使用的ID为扩展ID */

#define  CAN1_SCE_IRQ_Handler_EXT_EN	1	/* 定义外部文件处理SCE中断 */

// CAN引脚定义
#define  RCC_APB2Periph_GPIO_CAN    RCC_APB2Periph_GPIOB
#define  GPIO_Remap_CAN			   	GPIO_Remap1_CAN1
#define  GPIO_CAN				   	GPIOB
#define  GPIO_Pin_CAN_RX 		   	GPIO_Pin_8
#define  GPIO_Pin_CAN_TX 		   	GPIO_Pin_9

/* 软件中断处理:当CAN接收到数据并存储到FIFO后,触发软件中断,然后在对应的软件中断处理函数中解析接收到的CAN数据 */
#define  CAN1_SWI_HANDLE_EN			1					/* 1:开启软件中断方式处理接收的CAN数据 */
#if CAN1_SWI_HANDLE_EN > 0								/* Add by Xsky 2011-06-18 14:55 */
#define  CAN1_SWI_IRQn				EXTI4_IRQn			/* SWI中断号 */
#define  CAN1_SWI_IRQHandler		EXTI4_IRQHandler	/* 注意该定义无效需修改实际代码 */
#define  CAN1_SWI_GPIO_PORT_SOURCE	GPIO_PortSourceGPIOA	/* 线中断源端口 */
#define  CAN1_SWI_GPIO_PIN_SOURCE	GPIO_PinSource4			/* 线中断源位号 */
#define  CAN1_SWI_EXTI_LINE			EXTI_Line4				/* 线号 */
#define  CAN1_SWI_ClrInt()			EXTI->PR = CAN1_SWI_EXTI_LINE	/* 清除中断 */
#define  CAN1_SWI_Disable()			CLRBITs(*((__IO uint32 *)(EXTI_BASE)), CAN1_SWI_EXTI_LINE)
#define  CAN1_SWI_Enable()			SETBITs(*((__IO uint32 *)(EXTI_BASE)), CAN1_SWI_EXTI_LINE)
CAN_EXT void EXTI4_IRQHandler(void);
#define CAN1_SWI_Handler 	EXTI4_IRQHandler
#endif	/* end: #if CAN1_SWI_HANDLE_EN > 0 */

/***************************************************************************************************/
// STM32 CAN寄存器相关定义
#define CAN_TSR_TME_BITnL			26						/* CAN发送 */
#define CAN_TIxR_IDE				((uint32)BIT(2))		/* 0:标准标识符, 1:扩展标识符 */
#define CAN_TIxR_RTR				((uint32)BIT(1))		/* 0:数据帧, 1:远程帧 */
#define CAN_TIxR_TXRQ         		((uint32)0x00000001)	/* register CAN_TIxR */
#define CAN_TDTxR_DLC         		((uint32)0x0000000F)	/* register CAN_TDTxR  */
#define CAN_ESR_LEC_BITnL			4						/* CAN_ESC_LEC最低位号 */
#define CAN_ESR_LEC_USER			CAN_ESR_LEC				/* CAN_ESC_LEC位掩码 */
#define CAN_ESR_TEC_BITnL			16						/* CAN_ESR_TEC最低位号 */

#define CAN_INT_TX_DISABLE()		CLRBITs(CAN1->IER, CAN_IT_TME)	/* 关闭CAN发送中断 */
#define CAN_INT_TX_ENABLE()			SETBITs(CAN1->IER, CAN_IT_TME)	/* 开启CAN发送中断 */
#define CAN_INT_RX0_DISABLE()		CLRBITs(CAN1->IER, CAN_IT_FMP0) /* 关闭CAN接中断 */
#define CAN_INT_RX0_ENABLE()		SETBITs(CAN1->IER, CAN_IT_FMP0) /* 开启CAN接中断 */
#define CAN_INT_RX1_DISABLE()		CLRBITs(CAN1->IER, CAN_IT_FMP1) /* 关闭CAN接中断 */
#define CAN_INT_RX1_ENABLE()		SETBITs(CAN1->IER, CAN_IT_FMP1) /* 开启CAN接中断 */
#define CAN_INT_RX_DISABLE()		CLRBITs(CAN1->IER, CAN_IT_FMP0 | CAN_IT_FMP1) /* 关闭CAN接中断 */
#define CAN_INT_RX_ENABLE()			SETBITs(CAN1->IER, CAN_IT_FMP0 | CAN_IT_FMP1) /* 开启CAN接中断 */


#ifndef CAN1_SCE_IRQ_Handler_EXT_EN
#define CAN1_SCE_IRQ_Handler_EXT_EN	0
#endif

#ifndef CAN_QUEUE_CRITICAL_EN
#define  CAN_QUEUE_CRITICAL_EN		1
#endif
#if (CAN_QUEUE_CRITICAL_EN > 0)
#define CAN_QUE_OS_ENTER_CRITICAL()		OS_ENTER_CRITICAL()
#define CAN_QUE_OS_EXIT_CRITICAL()		OS_EXIT_CRITICAL()
#else
#define CAN_QUE_OS_ENTER_CRITICAL()		
#define CAN_QUE_OS_EXIT_CRITICAL()		
#endif

#ifndef CAN_QUEUE_READ_EMPTY_EN
#define CAN_QUEUE_READ_EMPTY_EN		0
#endif
#ifndef CAN_QUEUE_WRITE_FULL_EN
#define CAN_QUEUE_WRITE_FULL_EN		0
#endif


/***************************************************************************************************/
// 类型定义

typedef enum {CAN_TxRx_Tx, CAN_TxRx_Rx} CanTxRx;

/* Local includes ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

typedef union
{
	uint8 	u08[8];		/* 定义字节型以满足需求 */
	uint16	u16[4];		
	uint32	u32[2];		/* 定义寄存器类型以提高读取效率 */
}T_CanData;

// 因为发送邮箱与接收邮箱数据结构基本相同，所以队列数据可直接使用
typedef struct
{
		uint32	IxR;	/* 与TIxR/RIxR对应 */
		uint32 	DTxR;	/* 与TDTxR/RDTxR对应 */
	T_CanData 	Data; 	/* TLxR:THxR/RLxR:RHxR, 八字节数据 */
}T_CanFrame;

typedef struct
{
	// 经测试将In, Out, 定义为索引方式比定义为指针执行效率低
	T_CanFrame		*Out;					/* 指向数据输出位置 		*/
	T_CanFrame 		*In;					/* 指向数据输入位置 		*/
	T_CanFrame 		*End;					/* 指向Buf的结束位置		*/
	uint16			NData;					/* 队列中数据个数			*/
	uint16			MaxData;				/* 队列中允许存储的数据个数 */
#if (CAN_QUEUE_READ_EMPTY_EN > 0)
	t_err			(* ReadEmpty)(void);	/* 读空处理函数 			*/
#endif
#if (CAN_QUEUE_WRITE_FULL_EN > 0)
	t_err			(* WriteFull)(uint32);	/* 写满处理函数 			*/
	uint32			WriteFullParam;
#endif
	T_CanFrame 		*Buf; 					/* 存储数据的空间			*/
}T_CanQueue;

/***************************************************************************************************/
// 引用外部函数

extern void CAN_FilterConfig(void);

#if (CAN_QUEUE_WRITE_FULL_EN > 0)
extern t_err CAN_QueueWriteFull(uint32 TxRx);	// 引用外部函数
#endif

/***************************************************************************************************/
// 函数声明
//CAN_EXT TestStatus CAN_Polling(void);
//CAN_EXT TestStatus CAN_Interrupt(void);

CAN_EXT void CAN_Config(void);
//CAN_EXT void Init_RxMes(CanRxMsg *RxMessage);

CAN_EXT void CAN_wrFilterExt32MaskMode (t_ureg , uint32 , uint32 , uint32 );

#ifndef STM32F10X_CL
#define CAN1_TX_IRQHandler_Name		USB_HP_CAN1_TX_IRQHandler
//void USB_HP_CAN1_TX_IRQHandler (void) 
#define CAN1_RX0_IRQHandler_Name	USB_LP_CAN1_RX0_IRQHandler
//void USB_LP_CAN1_RX0_IRQHandler (void)
#else
#define CAN1_TX_IRQHandler_Name		CAN1_TX_IRQHandler
//void CAN1_TX_IRQHandler (void)
#define CAN1_RX0_IRQHandler_Name	CAN1_RX0_IRQHandler
//void CAN1_RX0_IRQHandler (void)
#endif
#ifndef STM32F10X_CL
CAN_EXT void USB_HP_CAN1_TX_IRQHandler (void);
CAN_EXT void USB_LP_CAN1_RX0_IRQHandler(void);
#else
CAN_EXT void CAN1_TX_IRQHandler(void);
CAN_EXT void CAN1_RX0_IRQHandler(void);
#endif
CAN_EXT void CAN1_RX1_IRQHandler (void);
#if CAN1_SCE_IRQ_Handler_EXT_EN == 0
CAN_EXT void CAN1_SCE_IRQHandler (void);
#endif

//CAN_EXT t_err CAN_QueueWrite(register T_CanQueue *Queue, T_CanFrame *pData);
//CAN_EXT t_err CAN_QueueRead(register T_CanQueue *Queue, T_CanFrame *pData);

CAN_EXT t_err CAN_ReadFrame(T_CanFrame *pData);
CAN_EXT t_err CAN_SendFrame(T_CanFrame *pCanFrame);

/***************************************************************************************************/
// 全局量引用说明
#ifndef CAN_GLOBALS
extern T_CanQueue CanTxQueue;
extern T_CanQueue CanRxQueue;
#endif

#endif	// end: __CAN_H__

