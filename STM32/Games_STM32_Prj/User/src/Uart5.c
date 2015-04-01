/****************************************Copyright (c)**************************************************
**                                  vixa
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: UART5.C
**��   ��   ��: ���U��Ŀ
**�� �� ��  ��: 2011-11-02 14:16
**��        ��: UART��������, �����󲿷�Ӳ��������ö���
**				�ļ���UARTx_��ͷ�ĺ���Ϊ���ú�����ֲ������UARTʱһ�㲻���޸�ֱ��ʹ��
**				ʹ����DMA�����жϽ��շ���
**				ʹ���˶�ʱ���жϼ���ַ���ʱ
**			   ע: Ҫ�� CPU_HCLK >= 1MHz
** ��  ��: v1.0.00
**------------------------------------------------------------------------------------------------------
** ����ļ�: UART5.h, Uart.C, Uart.h
**------------------------------------------------------------------------------------------------------
** ����޸�����: 
** �޸���: 
** �汾: v1.0.00
**-------------------------------------------------------------------------------------------------------
** �޸���: 
** ����: 
** �汾: v1.0.00
** �޸�: 
**			1. 
**			2. 
**			3. 
**			4. 
**			5. 
********************************************************************************************************/

#define  UART1_GLOBALS
#include "Global.h"
#include "Uart.h"
#include "UART5.h"
#include "Stm32f10x.h"
#include "Stm32f10x_rcc.h"
#include "Target.h"
#include "Queue.h"

#if UART1_EN > 0	

/***************************************************************************************************/
// ��������, Ӧ��������UARTʱ��Ҫ�޸ı��ζ���
// �жϷ���������������Startup_stm32f10x_hd.s����������

// ���ں�
#define  UARTx_NO					5						/* ���ڱ��1~n */
// ���뿪��
#define  UARTx_TX_EN				1						/* ������ */
#define  UARTx_RX_EN				1						/* ������� */

// ����������
#ifdef DEBUG
#define  UARTx_Tx_BufSize			512						/* ���ͻ������ֽ�������������ָ��ṹ */
#else
#define  UARTx_Tx_BufSize			128						/* ���ͻ������ֽ�������������ָ��ṹ */
#endif
#define  UARTx_Rx_BufSize			64						/* ���ջ������ֽ�������������ָ��ṹ */
#define  UARTx_Rx_DMABufSize		32						/* ����DMA����������,ʹ��DMA��ת�������� */
// ���ڲ���
#define  UARTx_BaudRate				UART5_BaudRate			/* ������ */
#define  UARTx_WordLength			USART_WordLength_9b		/* ����λ */
#define  UARTx_StopBits				USART_StopBits_1		/* ֹͣλ */
#define  UARTx_Parity				USART_Parity_Odd		/* Odd:��У�� */
#define  UARTx_HardwareFlowControl	USART_HardwareFlowControl_None	/* Ӳ�������� */
#define  UARTx_Mode					(USART_Mode_Rx | USART_Mode_Tx)	/* ��/��ģʽ */
#define  UARTx_Config				UART5_Config			/* ���ú����� */
#define  UARTx_SendBytes			UART5_SendBytes			/* ���ͺ����� */
#define  UARTx_RcvdSize				UART5_RcvdSize			/* �����ֽ��� */
#define  UARTx_ReadBytes			UART5_ReadBytes			/* ���պ����� */
// ���ż��ں���ض���
#define  UARTx						UART5			/* USART_TypeDef�����UART����ַ */
//#define  UARTx_RCC_APB				2						/* UARTx�����ӵ�APB��:1����2 */
//#define  UARTx_RCC					RCC_APB2Periph_USART1	/* UARTxʱ�� */
//#define  UARTx_IRQn					USART1_IRQn				/* UARTx�ж������� */
#define  UARTx_NVIC_PriorityGroup	NVIC_PriorityGroup_USART5
#define  UARTx_NVIC_Priority		0
#define  UARTx_NVIC_PrioritySub		0
void	 USART5_IRQHandler(void);
#define  UARTx_IRQHandler			UART5_IRQHandler		/* UARTx�жϴ���������, ע��ΪUART/USART */
#define  UARTx_GPIO					GPIOC					/* UARTx������ʹ�õĶ˿� */
#define  UARTx_GPIO_RCC				RCC_APB2Periph_GPIOC	/* GPIOA RCC ���ƶ��� */
#define  UARTx_GPIO_Pin_Tx			GPIO_Pin_12				/* ����Ӳ������ */
#define  UARTx_GPIO_Pin_Rx			GPIO_Pin_2				/* ����Ӳ������ */
// DMA����
#define  UARTx_DMA_EN				0						/* 1:����DMA,0:��ʹ��DMA,1:ֱ��ʹ��UART�ж��շ����� */
#define  UARTx_DMA_RX_EN			0						/* 1:����DMA�����ж�,0:ʹ��UART�жϽ�������,һ�㲻ʹ��DMA�������� */
#define  UARTx_RCC_DMA				RCC_AHBPeriph_DMA2		/* �����򿪻�DMA */
#define  UARTx_DMA					DMA2					/* ����DMA�Ĵ��� */
#define  UARTx_DMA_Rx_Channel		DMA2_Channel3			/* UART.RX��Ӧ��DMAͨ�� */
#define  UARTx_DMA_Rx_NVICPrioGroup	NVIC_PriorityGroup_3	/* �ж��������ȼ��� */
#define  UARTx_DMA_Rx_IRQn			DMA2_Channel3_IRQn		/* �ж������� */
#define  UARTx_DMA_Rx_IRQPrio		0						/* �ж������ȼ� */
#define  UARTx_DMA_Rx_IRQPrioSub	0						/* �жϴ����ȼ� */
#define  UARTx_DMA_Rx_Priority		DMA_Priority_Medium		/* Rx���ȼ� */
#define  UARTx_DMA_Rx_IF_TC			DMA2_FLAG_TC3			/* ����ͨ���жϱ�־ */
#define  UARTx_DMA_Rx_IF_GL			DMA2_FLAG_GL3			/* ȫ���жϱ�־ */
#define  UARTx_DMA_Rx_IF_HT			DMA2_FLAG_HT3			/* �봫������жϱ�־ */
void 	 DMA1_Channel5_IRQHandler(void);
#define  UARTx_DMA_Rx_IRQHandler	DMA2_Channel3_IRQHandler	/* Rx�жϴ�������,ע������ƶ�����Ч������ĺ�����ʽ˵�� */
// ���ճ�ʱ��ʱ��
#define  UARTx_RX_TIM_EN			0						/* ���������ַ���ʱ�ж� */
#define  UARTx_RX_TIM_OutCharN		(4.5)					/* <57, ����UART��ʱ�ַ���,���õĳ�ʱʱ��Ϊ:CharNx11/Baudrate�� */
#define  UARTx_RX_TIM_CC_EN			0						/* ����Ϊ1��ʱ��ʹ�ù����ļ�Uart.c��ʹ������Ƚ�ģʽ,0:���ļ�ʹ�õ����Ķ�ʱ��,�ڱ��ļ��ж��嶨ʱ�� */
#if UARTx_RX_TIM_CC_EN > 0
#define  UARTx_Rx_TIM_Start()		UART1T4_Rx_TIM_CC_Start(5, 5)	/* ������ʱ��ͨ�� */
#define  UARTx_Rx_isTimeOut()		UART1T4_Rx_isTimeOut(5)			/* �رն�ʱ��ͨ�� */
#else
//#define  UARTx_Rx_TIM				TIM6					/* ���ճ�ʱ��ʱ�� */
//#define  UARTx_Rx_TIM_RCCCmd(s) 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, s)/* ʱ�� */
//#define  UARTx_Rx_TIM_NVICPrioGroup	NVIC_PriorityGroup_4	/* ��ʱ���ж����ȼ��� */
//#define  UARTx_Rx_TIM_IRQn			TIM6_IRQn				/* ��ʱ���ж������� */
//#define  UARTx_Rx_TIM_IRQPrio		0						/* ��ʱ���ж����ȼ� */
//#define  UARTx_Rx_TIM_IRQPrioSub	0						/* ��ʱ���ж������ȼ� */
void 	 TIM6_IRQHandler(void);
#define  UARTx_Rx_TIM_IRQHandler	TIM6_IRQHandler			/* ��ʱ���жϴ����� */
#endif	 // end: #if UARTx_RX_TIM_CC_EN == 0 //
#define  UARTx_DMA_Tx_Channel		DMA2_Channel5			/* DMAͨ�� */
#define  UARTx_DMA_Tx_NVICPrioGroup	NVIC_PriorityGroup_3	/* �ж��������ȼ��� */
#define  UARTx_DMA_Tx_IRQn			DMA2_Channel4_5_IRQn	/* �ж������� */
#define  UARTx_DMA_Tx_IRQPrio		0						/* �ж������ȼ� */
#define  UARTx_DMA_Tx_IRQPrioSub	0						/* �жϴ����ȼ� */
#define  UARTx_DMA_Tx_Priority		DMA_Priority_Low		/* Tx���ȼ� */
#define  UARTx_DMA_Tx_IF_TC			DMA2_FLAG_TC5			/* ����ͨ���жϱ�־ */
#define  UARTx_DMA_Tx_IF_TE			DMA2_FLAG_TE5			/* ��������жϱ�־ */
#define  UARTx_DMA_Tx_IF_GL			DMA2_FLAG_GL5			/* ȫ���жϱ�־ */
#define  UARTx_DMA_Tx_Number		0x0104					/* ���ֽ�ΪDMA��1/2, ���ֽ�Ϊͨ���� */
void 	 DMA1_Channel4_IRQHandler(void);
#define  UARTx_DMA_Tx_IRQHandler	DMA2_Channel4_5_IRQHandler	/* Tx�жϴ������� */
// �жϹ�������
#if UARTx_DMA_EN > 0
#define  UARTx_ENTER_CRITICAL_TX()	OS_ENTER_CRITICAL()//CLRBITs(UARTx_DMA_Tx_Channel->CCR, DMA_IT_TC)	/* CLRBITs(UARTx->CR1, USART_FLAG_TC), ���ж� */
#define  UARTx_EXIT_CRITICAL_TX()	OS_EXIT_CRITICAL()//SETBITs(UARTx_DMA_Tx_Channel->CCR, DMA_IT_TC)	/* SETBITs(UARTx->CR1, USART_FLAG_TC), ���ж� */
#else
#define  UARTx_ENTER_CRITICAL_TX()	UARTx_ENTER_CRITICALx_TX(5)	/* CLRBITs(UARTx->CR1, USART_FLAG_TC)	, ���ж� */
#define  UARTx_EXIT_CRITICAL_TX()	UARTx_EXIT_CRITICALx_TX(5)	/* SETBITs(UARTx->CR1, USART_FLAG_TC)	 ���ж� */
#endif
#define  UARTx_ENTER_CRITICAL_RX()	UARTx_ENTER_CRITICALx_RX(5)	/* CLRBITs(UARTx->CR1, USART_FLAG_RXNE), ���ж� */
#define  UARTx_EXIT_CRITICAL_RX()	UARTx_EXIT_CRITICALx_RX(5)	/* SETBITs(UARTx->CR1, USART_FLAG_RXNE), ���ж� */


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
// ��ȡUart5���л������ֽ���
int Uart5CanSendBytes(void)
{
	int N;
	N = pTxBuf->MaxData- pTxBuf->NData;
	return N;
}

#endif

#endif		// end: #if UART5_EN > 0  //
