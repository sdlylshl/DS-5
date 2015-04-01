/***************************************************************************************************/
// ��������, Ӧ��������UARTʱ��Ҫ�޸ı��ζ���
/****************************************Copyright (c)**************************************************
**                                  vixa
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: Uart.C
**��   ��   ��: Xsky
**�� �� ��  ��: 2011-03-28 11:31
**��        ��: UART��������
**				UART���ճ�ʱ��ʱ�����ƣ����ö�ʱ�������Զ���������ֵ
**				һ·����ʹ��һ·����Ƚ��жϣ��ȽϼĴ�����ֵʹ�ü�������ǰֵ������Ӧ�Ĵ��ڳ�ʱ����ֵ
**				�����д��ڳ�ʱ�жϾ����ʱ���رն�ʱ��������λ������ֵ
**				����1��4ʹ�ö�ʱ��xͨ��1��4, ����5��8ʹ�ö�ʱ��yͨ��1��4
** ��  ��: v1.0.00
**------------------------------------------------------------------------------------------------------
** ����ļ�: Uart.h
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
// ��������, Ӧ��������UARTʱ��Ҫ�޸ı��ζ���
#define  UART1T4_Rx_TIM_RCCCmd(s) 		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, s)/* ʱ�� */
#define  UART1T4_Rx_TIM_NVICPrioGroup	NVIC_PriorityGroup_4	/* ��ʱ���ж����ȼ��� */
#define  UART1T4_Rx_TIM_IRQn			TIM3_IRQn				/* ��ʱ���ж������� */
#define  UART1T4_Rx_TIM_IRQPrio			0						/* ��ʱ���ж����ȼ� */
#define  UART1T4_Rx_TIM_IRQPrioSub		1						/* ��ʱ���ж������ȼ� */
void 	 TIM3_IRQHandler(void);
#define  UART1T4_Rx_TIM_IRQHandler		TIM3_IRQHandler			/* ��ʱ���жϴ����� */

#define  UARTx_ENTER_CRITICALx(x)		UARTx_ENTER_CRITICALx_RX(x)	/* �ش���x�ж� */
#define  UARTx_EXIT_CRITICALx(x)		UARTx_EXIT_CRITICALx_RX(x)	/* ������x�ж� */
#define  UARTx_ENTER_CRITICAL()			UARTx_ENTER_CRITICALx_RX(1); UARTx_ENTER_CRITICALx_RX(2); UARTx_ENTER_CRITICALx_RX(3); UARTx_ENTER_CRITICALx_RX(4)	/* �ش���1��4�ж� */
#define  UARTx_EXIT_CRITICAL()			UARTx_EXIT_CRITICALx_RX(1); UARTx_EXIT_CRITICALx_RX(2); UARTx_EXIT_CRITICALx_RX(3); UARTx_EXIT_CRITICALx_RX(4)	/* ������1��4�ж� */

/***************************************************************************************************/
// ����
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
* �ļ���	  	 : UART_Rx_TIM_Config
* ����	         : �����ַ���ʱ��ʱ����ʼ������
* 					�����Ϊ:�����жϲ���ʱ���ö�ʱ��������������ʱ��������ʱ����ʱ
*					˵���ڴ˶�ʱʱ���������ַ�����ֹͣ��ʱ������ֱ�Ӷ�ȡ����
* ����           : ��
* ���           : ��
* ����           : ��
*******************************************************************************/
void UART1T4_Rx_TIM_Config(void)
{
	/* Datasheet.CN
	** �߼����ƶ�ʱ��: p199, TIM1/TIM8
	** ������ʱ��: p298, TIM6/TIM7
	** ͨ�ö�ʱ��: p253, TIM2/3/4/5
	*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	TIM_DeInit(UART1T4_Rx_TIM);
	
	// �ж����������ȼ�����
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
	TIM_TimeBaseInitStructure.TIM_Prescaler = CPU_HCLK/1000000 - 1;	// Ԥ��ƵΪ1us
	TIM_TimeBaseInitStructure.TIM_Period = 0xFFFF;					// ARR
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(UART1T4_Rx_TIM,&TIM_TimeBaseInitStructure);
	
	// ����Ƚ�ģʽ����
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
* ������	  	 : UART1T4_Rx_TIM_IRQHandler
* ����	         : ���ճ�ʱ��ʱ���ж�, ֹͣ��ʱ���������ַ���ʱ��־
* ����           : ��
* ���           : ��
* ����           : ��
*******************************************************************************/
void UART1T4_Rx_TIM_IRQHandler(void)
{
	#if defined(UART1_EN) && (UART1_EN > 0)
		if CHKBITs(UART1T4_Rx_TIM->SR, TIM_FLAG_CC1)		/* ����ж�ʹ��λ���ų�������ж� */
		{
			// ����жϱ�־
			CLRBITs(UART1T4_Rx_TIM->SR, TIM_FLAG_CC1);
			
			UARTx_ENTER_CRITICALx(1);
			// ֹͣ��ʱ��
			UART1T4_Rx_TIM_CC_Stop(1);		// CC1�ر�
			// ���ó�ʱ��־
			Uart1_RxTimeOutFlag = c_TimeOut;
			UARTx_EXIT_CRITICALx(1);
		}
	#endif
	#if defined(UART2_EN) && (UART2_EN > 0)
		if CHKBITs(UART1T4_Rx_TIM->SR, TIM_FLAG_CC2)		/* ����ж�ʹ��λ���ų�������ж� */
		{
			// ����жϱ�־
			CLRBITs(UART1T4_Rx_TIM->SR, TIM_FLAG_CC2);
			
			UARTx_ENTER_CRITICALx(2);
			// ֹͣ��ʱ��
			UART1T4_Rx_TIM_CC_Stop(2);		// CC1�ر�
			// ���ó�ʱ��־
			Uart2_RxTimeOutFlag = c_TimeOut;
			UARTx_EXIT_CRITICALx(2);
		}
	#endif
	#if defined(UART3_EN) && (UART3_EN > 0)
		if CHKBITs(UART1T4_Rx_TIM->SR, TIM_FLAG_CC3)		/* ����ж�ʹ��λ���ų�������ж� */
		{
			// ����жϱ�־
			CLRBITs(UART1T4_Rx_TIM->SR, TIM_FLAG_CC3);
			
			UARTx_ENTER_CRITICALx(3);
			// ֹͣ��ʱ��
			UART1T4_Rx_TIM_CC_Stop(3);		// CC1�ر�
			// ���ó�ʱ��־
			Uart3_RxTimeOutFlag = c_TimeOut;
			UARTx_EXIT_CRITICALx(3);
		}
	#endif
	#if defined(UART4_EN) && (UART4_EN > 0)
		if CHKBITs(UART1T4_Rx_TIM->SR, TIM_FLAG_CC4)		/* ����ж�ʹ��λ���ų�������ж� */
		{
			// ����жϱ�־
			CLRBITs(UART1T4_Rx_TIM->SR, TIM_FLAG_CC4);
			
			UARTx_ENTER_CRITICALx(4);
			// ֹͣ��ʱ��
			UART1T4_Rx_TIM_CC_Stop(4);		// CC1�ر�
			// ���ó�ʱ��־
			Uart4_RxTimeOutFlag = c_TimeOut;
			UARTx_EXIT_CRITICALx(4);
		}
	#endif
	#if UART_EN > 0 && UART1T4_RX_TIM_EN > 0
		UARTx_ENTER_CRITICAL();
		// ����Ƿ�������һ��ͨ������
		if CHKNBITs(UART1T4_Rx_TIM->CCER, TIM_CCER_CC1E | TIM_CCER_CC2E
					 					| TIM_CCER_CC3E | TIM_CCER_CC4E)
		{
			// ֹͣ��ʱ��
			UART1T4_Rx_TIM_Stop();
			// ���㶨ʱ������
			UART1T4_Rx_TIM_ClrCnt();
		}
		UARTx_EXIT_CRITICAL();
	#endif
}

/*********************************************************************************************************
** ��������: 
** ��������: 
** �䡡��:  1. : 
**			2. : 
**			3. : 
** �䡡��: ��
** ������: ��
** ȫ�ֱ���: ��
** ����ģ��: 
** �桡��: 
** ������: Xsky
** �ա���: 
** ����ע: 
**-------------------------------------------------------------------------------------------------------
** �桡��: 
** �޸���: 
** �ա���: 
** �衡��: 
********************************************************************************************************/


#endif	// end: #if UART1T4_RX_TIM_EN > 0 //

#endif	// end: #if UART_EN > 0 //

