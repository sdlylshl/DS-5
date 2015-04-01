/****************************************Copyright (c)**************************************************
**                                  vixa
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: Uart.h
**��   ��   ��: Xsky
**�� �� ��  ��: 2011-03-28 11:31
**��        ��: UART������ض��塢����������
** ��  ��: v1.0.00
**------------------------------------------------------------------------------------------------------
** ����ļ�: Uart.c, ����Ӳ������������޸�Uart.c�ļ�
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
#ifndef __UART_H__
#define __UART_H__

#ifdef UART_GLOBALS
#define  UART_EXT
#else
#define  UART_EXT	extern
#endif

/***************************************************************************************************/
// ��������
#define  UART1T4_Rx_TIM					TIM3	/* ���ճ�ʱ��ʱ�� */
#define  UART1T4_RX_TIM_OutCharN		(4.5)	/* <57, ����UART��ʱ�ַ���,���õĳ�ʱʱ��Ϊ:CharNx11/Baudrate�� */

#define  USART4		UART4
#define  USART5		UART5
// �ط����ж�
#define  UARTx_ENTER_CRITICALx_TX(x)		OS_ENTER_CRITICAL()//CLRBITs(USART##x->CR1, USART_FLAG_TC)	/* ���ж� */
// �������ж�
#define  UARTx_EXIT_CRITICALx_TX(x)			OS_EXIT_CRITICAL()//SETBITs(USART##x->CR1, USART_FLAG_TC)	/* ���ж� */
// �رս����ж�
#define  UARTx_ENTER_CRITICALx_RX(x)		OS_ENTER_CRITICAL()//CLRBITs(USART##x->CR1, USART_FLAG_RXNE)	/* ���ж� */
// �������ж�
#define  UARTx_EXIT_CRITICALx_RX(x)			OS_EXIT_CRITICAL()//SETBITs(USART##x->CR1, USART_FLAG_RXNE)	/* ���ж� */
/* �ض�ʱ��ͨ��y�ж� */
#define  UART1T4_TIM_ENTER_CRITICALx(y)		CLRBITs(UART1T4_Rx_TIM->DIER, TIM_IT_CC##y)	
/* ����ʱ��ͨ��y�ж� */
#define  UART1T4_TIM_EXIT_CRITICALx(y)		SETBITs(UART1T4_Rx_TIM->DIER, TIM_IT_CC##y)	

typedef enum{c_InTime=0, c_TimeOut=1}t_TimeStatus;


/***************************************************************************************************/
// ��������

// ֹͣ��ʱ��
#define  UART1T4_Rx_TIM_Stop() 				CLRBITs(UART1T4_Rx_TIM->CR1, TIM_CR1_CEN)
// ���㶨ʱ������
#define  UART1T4_Rx_TIM_ClrCnt()			UART1T4_Rx_TIM->CNT = 0x0000
// ������ʱ��
#define  UART1T4_Rx_TIM_Start()				SETBITs(UART1T4_Rx_TIM->CR1, TIM_CR1_CEN);

// ����CCxͨ��,����1��4ʹ�ö�ʱ��xͨ��1��4, ����5��8ʹ�ö�ʱ��yͨ��1��4
#define  UART1T4_Rx_TIM_CC_Start(Ux,CCy)	UART1T4_TIM_ENTER_CRITICALx(CCy); \
											UART1T4_Rx_TIM->CCR##CCy = (uint16)UART1T4_Rx_TIM->CNT + (uint16)(UARTx_RX_TIM_OutCharN * 11 * 1000000 / UARTx_BaudRate); \
											Uart##Ux##_RxTimeOutFlag = c_InTime; \
											SETBITs(UART1T4_Rx_TIM->CCER, TIM_CCER_CC##CCy##E); \
											UART1T4_Rx_TIM_Start(); \
											UART1T4_TIM_EXIT_CRITICALx(CCy)
// �ر�CCxͨ��,����1��4ʹ�ö�ʱ��xͨ��1��4, ����5��8ʹ�ö�ʱ��yͨ��1��4
#define  UART1T4_Rx_TIM_CC_Stop(CCy)		CLRBITs(UART1T4_Rx_TIM->CCER, TIM_CCER_CC##CCy##E)
// ����Ƿ�ʱ,n���ں�1~n
#define  UART1T4_Rx_isTimeOut(n)			(Uart##n##_RxTimeOutFlag == c_TimeOut)

// ��ʱ����ʼ������
extern void UART1T4_Rx_TIM_Config(void);


/***************************************************************************************************/
// ȫ��������˵��
#ifndef UART_GLOBALS
extern uint8 Uart1_RxTimeOutFlag;
extern uint8 Uart2_RxTimeOutFlag;
extern uint8 Uart3_RxTimeOutFlag;
extern uint8 Uart4_RxTimeOutFlag;

#endif


#endif	// end: __UART_H__

