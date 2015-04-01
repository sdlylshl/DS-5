/****************************************Copyright (c)**************************************************
**                              2009-2009, YingHong, Ltd.
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: CAN.h
**��   ��   ��: Xsky
**�� �� ��  ��: 2011-02-25 14:45
**��        ��: CAN������ͼ��������壬�����������жϷ�����
** ��  ��: v1.0.00
**------------------------------------------------------------------------------------------------------
** ����ļ�: 
**			 
**			 
**-------------------------------------------------------------------------------------------------------
**
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
#ifndef __CAN_H__
#define __CAN_H__

#ifdef CAN_GLOBALS
#define  CAN_EXT
#else
#define  CAN_EXT	extern
#endif

#include "Global.h"

/***************************************************************************************************/
// ��������
#define  CAN_BPS		500000		/* CANͨ�Ų����ʶ��� */

#define  CAN_QUE_RX_Nframe	100		/* CAN���ն���֡��Ŀ,�Խṹ��T_CanFrameΪ��Ԫ������ */
#define  CAN_QUE_TX_Nframe	100		/* CAN���Ͷ���֡��Ŀ,�Խṹ��T_CanFrameΪ��Ԫ������ */

#define  CAN_QUEUE_WRITE_FULL_EN		1	/* 1: ��������д�����ú��� */

#define  CAN_ID_TYPE		CAN_ID_EXT		/* ����CANʹ�õ�IDΪ��չID */

#define  CAN1_SCE_IRQ_Handler_EXT_EN	1	/* �����ⲿ�ļ�����SCE�ж� */

// CAN���Ŷ���
#define  RCC_APB2Periph_GPIO_CAN    RCC_APB2Periph_GPIOB
#define  GPIO_Remap_CAN			   	GPIO_Remap1_CAN1
#define  GPIO_CAN				   	GPIOB
#define  GPIO_Pin_CAN_RX 		   	GPIO_Pin_8
#define  GPIO_Pin_CAN_TX 		   	GPIO_Pin_9

/* ����жϴ���:��CAN���յ����ݲ��洢��FIFO��,��������ж�,Ȼ���ڶ�Ӧ������жϴ������н������յ���CAN���� */
#define  CAN1_SWI_HANDLE_EN			1					/* 1:��������жϷ�ʽ������յ�CAN���� */
#if CAN1_SWI_HANDLE_EN > 0								/* Add by Xsky 2011-06-18 14:55 */
#define  CAN1_SWI_IRQn				EXTI4_IRQn			/* SWI�жϺ� */
#define  CAN1_SWI_IRQHandler		EXTI4_IRQHandler	/* ע��ö�����Ч���޸�ʵ�ʴ��� */
#define  CAN1_SWI_GPIO_PORT_SOURCE	GPIO_PortSourceGPIOA	/* ���ж�Դ�˿� */
#define  CAN1_SWI_GPIO_PIN_SOURCE	GPIO_PinSource4			/* ���ж�Դλ�� */
#define  CAN1_SWI_EXTI_LINE			EXTI_Line4				/* �ߺ� */
#define  CAN1_SWI_ClrInt()			EXTI->PR = CAN1_SWI_EXTI_LINE	/* ����ж� */
#define  CAN1_SWI_Disable()			CLRBITs(*((__IO uint32 *)(EXTI_BASE)), CAN1_SWI_EXTI_LINE)
#define  CAN1_SWI_Enable()			SETBITs(*((__IO uint32 *)(EXTI_BASE)), CAN1_SWI_EXTI_LINE)
CAN_EXT void EXTI4_IRQHandler(void);
#define CAN1_SWI_Handler 	EXTI4_IRQHandler
#endif	/* end: #if CAN1_SWI_HANDLE_EN > 0 */

/***************************************************************************************************/
// STM32 CAN�Ĵ�����ض���
#define CAN_TSR_TME_BITnL			26						/* CAN���� */
#define CAN_TIxR_IDE				((uint32)BIT(2))		/* 0:��׼��ʶ��, 1:��չ��ʶ�� */
#define CAN_TIxR_RTR				((uint32)BIT(1))		/* 0:����֡, 1:Զ��֡ */
#define CAN_TIxR_TXRQ         		((uint32)0x00000001)	/* register CAN_TIxR */
#define CAN_TDTxR_DLC         		((uint32)0x0000000F)	/* register CAN_TDTxR  */
#define CAN_ESR_LEC_BITnL			4						/* CAN_ESC_LEC���λ�� */
#define CAN_ESR_LEC_USER			CAN_ESR_LEC				/* CAN_ESC_LECλ���� */
#define CAN_ESR_TEC_BITnL			16						/* CAN_ESR_TEC���λ�� */

#define CAN_INT_TX_DISABLE()		CLRBITs(CAN1->IER, CAN_IT_TME)	/* �ر�CAN�����ж� */
#define CAN_INT_TX_ENABLE()			SETBITs(CAN1->IER, CAN_IT_TME)	/* ����CAN�����ж� */
#define CAN_INT_RX0_DISABLE()		CLRBITs(CAN1->IER, CAN_IT_FMP0) /* �ر�CAN���ж� */
#define CAN_INT_RX0_ENABLE()		SETBITs(CAN1->IER, CAN_IT_FMP0) /* ����CAN���ж� */
#define CAN_INT_RX1_DISABLE()		CLRBITs(CAN1->IER, CAN_IT_FMP1) /* �ر�CAN���ж� */
#define CAN_INT_RX1_ENABLE()		SETBITs(CAN1->IER, CAN_IT_FMP1) /* ����CAN���ж� */
#define CAN_INT_RX_DISABLE()		CLRBITs(CAN1->IER, CAN_IT_FMP0 | CAN_IT_FMP1) /* �ر�CAN���ж� */
#define CAN_INT_RX_ENABLE()			SETBITs(CAN1->IER, CAN_IT_FMP0 | CAN_IT_FMP1) /* ����CAN���ж� */


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
// ���Ͷ���

typedef enum {CAN_TxRx_Tx, CAN_TxRx_Rx} CanTxRx;

/* Local includes ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

typedef union
{
	uint8 	u08[8];		/* �����ֽ������������� */
	uint16	u16[4];		
	uint32	u32[2];		/* ����Ĵ�����������߶�ȡЧ�� */
}T_CanData;

// ��Ϊ��������������������ݽṹ������ͬ�����Զ������ݿ�ֱ��ʹ��
typedef struct
{
		uint32	IxR;	/* ��TIxR/RIxR��Ӧ */
		uint32 	DTxR;	/* ��TDTxR/RDTxR��Ӧ */
	T_CanData 	Data; 	/* TLxR:THxR/RLxR:RHxR, ���ֽ����� */
}T_CanFrame;

typedef struct
{
	// �����Խ�In, Out, ����Ϊ������ʽ�ȶ���Ϊָ��ִ��Ч�ʵ�
	T_CanFrame		*Out;					/* ָ���������λ�� 		*/
	T_CanFrame 		*In;					/* ָ����������λ�� 		*/
	T_CanFrame 		*End;					/* ָ��Buf�Ľ���λ��		*/
	uint16			NData;					/* ���������ݸ���			*/
	uint16			MaxData;				/* ����������洢�����ݸ��� */
#if (CAN_QUEUE_READ_EMPTY_EN > 0)
	t_err			(* ReadEmpty)(void);	/* ���մ����� 			*/
#endif
#if (CAN_QUEUE_WRITE_FULL_EN > 0)
	t_err			(* WriteFull)(uint32);	/* д�������� 			*/
	uint32			WriteFullParam;
#endif
	T_CanFrame 		*Buf; 					/* �洢���ݵĿռ�			*/
}T_CanQueue;

/***************************************************************************************************/
// �����ⲿ����

extern void CAN_FilterConfig(void);

#if (CAN_QUEUE_WRITE_FULL_EN > 0)
extern t_err CAN_QueueWriteFull(uint32 TxRx);	// �����ⲿ����
#endif

/***************************************************************************************************/
// ��������
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
// ȫ��������˵��
#ifndef CAN_GLOBALS
extern T_CanQueue CanTxQueue;
extern T_CanQueue CanRxQueue;
#endif

#endif	// end: __CAN_H__

