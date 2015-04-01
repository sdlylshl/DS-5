/***************************************************************************************************/
// 常数定义, 应用于其它UART时需要修改本段定义
/****************************************Copyright (c)**************************************************
**                                  vixa
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: Uart.C
**创   建   人: Xsky
**创 建 日  期: 2011-03-28 11:31
**描        述: UART驱动函数
**				UART接收超时定时器机制，设置定时计数器自动增长至顶值
**				一路串口使用一路输出比较中断，比较寄存器的值使用计数器当前值加上相应的串口超时计数值
**				当所有串口超时中断均完成时，关闭定时器，并复位计数器值
**				串口1到4使用定时器x通道1到4, 串口5到8使用定时器y通道1到4
** 版  本: v1.0.00
**------------------------------------------------------------------------------------------------------
** 相关文件: Uart.h
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

#define  UART_GLOBALS
#include "Global.h"
#include "Stm32f10x.h"
#include "Stm32f10x_rcc.h"
#include "Target.h"
#include "Uart.h"

#ifndef UART_EN
#define  UART_EN		0
#endif
#if UART_EN > 0

/***************************************************************************************************/
// 常数定义, 应用于其它UART时需要修改本段定义
#define  UART1T4_Rx_TIM_RCCCmd(s) 		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, s)/* 时钟 */
#define  UART1T4_Rx_TIM_NVICPrioGroup	NVIC_PriorityGroup_4	/* 定时器中断优先级组 */
#define  UART1T4_Rx_TIM_IRQn			TIM3_IRQn				/* 定时器中断向量号 */
#define  UART1T4_Rx_TIM_IRQPrio			0						/* 定时器中断优先级 */
#define  UART1T4_Rx_TIM_IRQPrioSub		1						/* 定时器中断子优先级 */
void 	 TIM3_IRQHandler(void);
#define  UART1T4_Rx_TIM_IRQHandler		TIM3_IRQHandler			/* 定时器中断处理函数 */

#define  UARTx_ENTER_CRITICALx(x)		UARTx_ENTER_CRITICALx_RX(x)	/* 关串口x中断 */
#define  UARTx_EXIT_CRITICALx(x)		UARTx_EXIT_CRITICALx_RX(x)	/* 开串口x中断 */
#define  UARTx_ENTER_CRITICAL()			UARTx_ENTER_CRITICALx_RX(1); UARTx_ENTER_CRITICALx_RX(2); UARTx_ENTER_CRITICALx_RX(3); UARTx_ENTER_CRITICALx_RX(4)	/* 关串口1到4中断 */
#define  UARTx_EXIT_CRITICAL()			UARTx_EXIT_CRITICALx_RX(1); UARTx_EXIT_CRITICALx_RX(2); UARTx_EXIT_CRITICALx_RX(3); UARTx_EXIT_CRITICALx_RX(4)	/* 开串口1到4中断 */

/***************************************************************************************************/
// 正文
#ifndef UART1T4_RX_TIM_EN
#define  UART1T4_RX_TIM_EN		0
#endif
#if UART1T4_RX_TIM_EN > 0

#if defined(UART1_EN) && (UART1_EN > 0)
uint8 Uart1_RxTimeOutFlag = 0;
#endif
#if defined(UART2_EN) && (UART2_EN > 0)
uint8 Uart2_RxTimeOutFlag = 0;
#endif
#if defined(UART3_EN) && (UART3_EN > 0)
uint8 Uart3_RxTimeOutFlag = 0;
#endif
#if defined(UART4_EN) && (UART4_EN > 0)
uint8 Uart4_RxTimeOutFlag = 0;
#endif


/*******************************************************************************
* 文件名	  	 : UART_Rx_TIM_Config
* 描述	         : 接收字符超时定时器初始化配置
* 					其机制为:接收中断产生时重置定时器计数并开启定时器，当定时器超时
*					说明在此定时时间内已无字符，则停止定时器，可直接读取数据
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void UART1T4_Rx_TIM_Config(void)
{
	/* Datasheet.CN
	** 高级控制定时器: p199, TIM1/TIM8
	** 基本定时器: p298, TIM6/TIM7
	** 通用定时器: p253, TIM2/3/4/5
	*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	TIM_DeInit(UART1T4_Rx_TIM);
	
	// 中断向量及优先级设置
	//NVIC_PriorityGroupConfig(UART1T4_Rx_TIM_NVICPrioGroup);
	NVIC_InitStructure.NVIC_IRQChannel = UART1T4_Rx_TIM_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = UART1T4_Rx_TIM_IRQPrio;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = UART1T4_Rx_TIM_IRQPrioSub;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	UART1T4_Rx_TIM_RCCCmd(ENABLE);
	//TIM_ITConfig(UART1T4_Rx_TIM, TIM_IT_Update, ENABLE);		// Del by Xsky 2011-07-26 11:30
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure); 
	TIM_TimeBaseInitStructure.TIM_Prescaler = CPU_HCLK/1000000 - 1;	// 预分频为1us
	TIM_TimeBaseInitStructure.TIM_Period = 0xFFFF;					// ARR
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(UART1T4_Rx_TIM,&TIM_TimeBaseInitStructure);
	
	// 输出比较模式设置
	TIM_OCStructInit(&TIM_OCInitStruct);
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_Timing;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OCInitStruct.TIM_Pulse = 0;
	#if defined(UART1_EN) && (UART1_EN > 0)
	TIM_OC1Init(UART1T4_Rx_TIM, &TIM_OCInitStruct);
	//TIM_ITConfig(UART1T4_Rx_TIM, TIM_IT_CC1);
	#endif
	#if defined(UART2_EN) && (UART2_EN > 0)
	TIM_OC2Init(UART1T4_Rx_TIM, &TIM_OCInitStruct);
	#endif
	#if defined(UART3_EN) && (UART3_EN > 0)
	TIM_OC3Init(UART1T4_Rx_TIM, &TIM_OCInitStruct);
	#endif
	#if defined(UART4_EN) && (UART4_EN > 0)
	TIM_OC4Init(UART1T4_Rx_TIM, &TIM_OCInitStruct);
	#endif
	/*
	TIM_ITConfig(UART1T4_Rx_TIM, 0 
			#if defined(UART1_EN) && (UART1_EN > 0)
				| TIM_IT_CC1
			#endif
			#if defined(UART2_EN) && (UART2_EN > 0)
				| TIM_IT_CC2
			#endif
			#if defined(UART3_EN) && (UART3_EN > 0)
				| TIM_IT_CC3
			#endif
			#if defined(UART4_EN) && (UART4_EN > 0)
				| TIM_IT_CC4
			#endif
	   	, ENABLE);
	*/
	//TIM_SelectInputTrigger(UART1T4_Rx_TIM,TIM_TS_ITR1);
	
	TIM_ITConfig(UART1T4_Rx_TIM, TIM_IT_Update, ENABLE);
	
	TIM_ARRPreloadConfig(UART1T4_Rx_TIM, ENABLE);
	
	TIM_ITConfig(UART1T4_Rx_TIM, TIM_IT_Update, DISABLE);
	
	//TIM_Cmd(UART1T4_Rx_TIM, ENABLE); 
}


/*******************************************************************************
* 函数名	  	 : UART1T4_Rx_TIM_IRQHandler
* 描述	         : 接收超时定时器中断, 停止定时器并设置字符超时标志
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void UART1T4_Rx_TIM_IRQHandler(void)
{
	#if defined(UART1_EN) && (UART1_EN > 0)
		if CHKBITs(UART1T4_Rx_TIM->SR, TIM_FLAG_CC1)		/* 检查中断使用位以排除软件更中断 */
		{
			// 清除中断标志
			CLRBITs(UART1T4_Rx_TIM->SR, TIM_FLAG_CC1);
			
			UARTx_ENTER_CRITICALx(1);
			// 停止定时器
			UART1T4_Rx_TIM_CC_Stop(1);		// CC1关闭
			// 设置超时标志
			Uart1_RxTimeOutFlag = c_TimeOut;
			UARTx_EXIT_CRITICALx(1);
		}
	#endif
	#if defined(UART2_EN) && (UART2_EN > 0)
		if CHKBITs(UART1T4_Rx_TIM->SR, TIM_FLAG_CC2)		/* 检查中断使用位以排除软件更中断 */
		{
			// 清除中断标志
			CLRBITs(UART1T4_Rx_TIM->SR, TIM_FLAG_CC2);
			
			UARTx_ENTER_CRITICALx(2);
			// 停止定时器
			UART1T4_Rx_TIM_CC_Stop(2);		// CC1关闭
			// 设置超时标志
			Uart2_RxTimeOutFlag = c_TimeOut;
			UARTx_EXIT_CRITICALx(2);
		}
	#endif
	#if defined(UART3_EN) && (UART3_EN > 0)
		if CHKBITs(UART1T4_Rx_TIM->SR, TIM_FLAG_CC3)		/* 检查中断使用位以排除软件更中断 */
		{
			// 清除中断标志
			CLRBITs(UART1T4_Rx_TIM->SR, TIM_FLAG_CC3);
			
			UARTx_ENTER_CRITICALx(3);
			// 停止定时器
			UART1T4_Rx_TIM_CC_Stop(3);		// CC1关闭
			// 设置超时标志
			Uart3_RxTimeOutFlag = c_TimeOut;
			UARTx_EXIT_CRITICALx(3);
		}
	#endif
	#if defined(UART4_EN) && (UART4_EN > 0)
		if CHKBITs(UART1T4_Rx_TIM->SR, TIM_FLAG_CC4)		/* 检查中断使用位以排除软件更中断 */
		{
			// 清除中断标志
			CLRBITs(UART1T4_Rx_TIM->SR, TIM_FLAG_CC4);
			
			UARTx_ENTER_CRITICALx(4);
			// 停止定时器
			UART1T4_Rx_TIM_CC_Stop(4);		// CC1关闭
			// 设置超时标志
			Uart4_RxTimeOutFlag = c_TimeOut;
			UARTx_EXIT_CRITICALx(4);
		}
	#endif
	#if UART_EN > 0 && UART1T4_RX_TIM_EN > 0
		UARTx_ENTER_CRITICAL();
		// 检查是否有任意一个通道开启
		if CHKNBITs(UART1T4_Rx_TIM->CCER, TIM_CCER_CC1E | TIM_CCER_CC2E
					 					| TIM_CCER_CC3E | TIM_CCER_CC4E)
		{
			// 停止定时器
			UART1T4_Rx_TIM_Stop();
			// 清零定时计数器
			UART1T4_Rx_TIM_ClrCnt();
		}
		UARTx_EXIT_CRITICAL();
	#endif
}

/*********************************************************************************************************
** 函数名称: 
** 功能描述: 
** 输　入:  1. : 
**			2. : 
**			3. : 
** 输　出: 无
** 返　回: 无
** 全局变量: 无
** 调用模块: 
** 版　本: 
** 作　者: Xsky
** 日　期: 
** 备　注: 
**-------------------------------------------------------------------------------------------------------
** 版　本: 
** 修改人: 
** 日　期: 
** 描　述: 
********************************************************************************************************/


#endif	// end: #if UART1T4_RX_TIM_EN > 0 //

#endif	// end: #if UART_EN > 0 //

