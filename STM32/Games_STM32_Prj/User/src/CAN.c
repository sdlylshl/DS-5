/****************************************Copyright (c)**************************************************
**                              2009-2009, YingHong, Ltd.
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: CAN.h
**��   ��   ��: Xsky
**�� �� ��  ��: 2011-02-25 14:45
**��        ��: CAN������ͼ��������壬�����������жϷ�����
**			  ������CAN�����ʽ����/��������, FIFO���в����Ļ�����ԪΪT_CanFrame
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
#define  CAN_GLOBALS

#include "Config.h"

/*
#include "Global.h"
#include "stm32f10x.h"
#include "Stm32_Init.h"
#include "Target.h"
#include "CAN.h"
#include "Queue.h"
#include "GameParam.h"
#include "Communi.h"
*/

#ifndef CAN1_EN
#define CAN1_EN		0
#endif

#if CAN1_EN > 0

/***************************************************************************************************/
// ��������
#if CAN1_SWI_HANDLE_EN > 0								/* Add by Xsky 2011-06-18 14:55 */
#ifndef CAN1_SWI_IRQHandler_EXT_EN
#error	"ֱ�����ⲿʵ�ʴ��붨��SWI Handler��EXTI4_IRQHandler, ��������������ֱ��ʹ���������룡"
#define  CAN1_SWI_IRQHandler_EXT_EN 	0
#endif
#endif	/* end: #if CAN1_SWI_HANDLE_EN > 0 */

/***************************************************************************************************/
// ��������
// ���в���
/* �����жϣ�Ӧ�ò㷢�ͺ���ֱ�ӽ���������Ϊ�����͵�CAN���������ʽ
** T_CanFrame/CAN_TypeDef���
** ����ʱ��������п���ֱ�ӽ�����ѹ�����䣬��������ѹ�����
*/
T_CanQueue CanTxQueue;
T_CanQueue CanRxQueue;
// CAN���ͻ�����
static T_CanFrame CanTxBuf[CAN_QUE_TX_Nframe] = {0,};
// CAN���ջ�����
static T_CanFrame CanRxBuf[CAN_QUE_RX_Nframe] = {0, };

#ifdef DBG_CAN
// ���Ա���
//static CAN_TypeDef *pCAN1;
#endif

/***************************************************************************************************/
// ��������
static const uint8 TxMailBox_IdleNO_Tbl[] = {
				0xFF/*0x0:Busy*/,	0/*0x1:0*/, 	1/*010b:1*/, 
				0/*011b:0,1*/,		2/*100b:2*/,	0/*101b:0,2*/, 
				1/*110b:1,2*/,		0/*111b:0,1,2*/};

/*********************************************************************************************************
** ��������: CAN_QueueCreate
** ��������: ��ʼ�����ݶ���
** �䡡��: Buf     �� Ϊ���з���Ĵ洢�ռ��ַ
**         pDataBuf�� ���ݴ�Ż�����
**		   nData:	  ����Ԫ�ظ���,��*pDataBufΪ��Ԫ�ĸ���
**         ReadEmpty��Ϊ���ж���ʱ�������
**         WriteFull��Ϊ����д��ʱ�������
**         WriteFullParam: ����д����������ʱ�Ĳ���
** �䡡��: NOT_OK:��������
**         QUEUE_OK:�ɹ�
** ȫ�ֱ���: ��
** ����ģ��: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
**
** ������: Xsky
** �ա���: 2011-03-10 10:43
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
static t_err CAN_QueueCreate(register T_CanQueue *Queue, T_CanFrame *pDataBuf,
                          	t_size_opt nData /*, t_size_opt nSizeOfData*/
							#if (CAN_QUEUE_READ_EMPTY_EN > 0)
                          	,t_err (* ReadEmpty)()
							#endif
							#if (CAN_QUEUE_WRITE_FULL_EN > 0)
                          	,t_err (* WriteFull)(uint32)
							,uint32   WriteFullParam
							#endif
                          	)
{
	//T_DataQueue *Queue;
	
   	//uint16	i;
	//i =   	(uint16)(((T_DataQueue *)0)->Buf);
    	
	if (Queue != NULL && pDataBuf != NULL)						/* �жϲ����Ƿ���Ч */
	{
		//Queue = (T_DataQueue *)Buf;
		
		CAN_QUE_OS_ENTER_CRITICAL();
		                                                	/* ��ʼ���ṹ������ */
		Queue->Buf = pDataBuf;
		Queue->MaxData = nData;				               	/* ������п��Դ洢��������Ŀ */
		Queue->End = &Queue->Buf[Queue->MaxData];           /* �������ݻ���Ľ�����ַ,�ѳ��������� */
		Queue->Out = Queue->Buf;
		Queue->In = Queue->Buf;
		Queue->NData = 0;
		
		#if (CAN_QUEUE_READ_EMPTY_EN > 0)
		Queue->ReadEmpty = ReadEmpty;
		#endif
		#if (CAN_QUEUE_WRITE_FULL_EN > 0)
		Queue->WriteFull = WriteFull;
		Queue->WriteFullParam = WriteFullParam;
		#endif
		CAN_QUE_OS_EXIT_CRITICAL();

		return QUEUE_OK;
	}
 	else
	{
		return NOT_OK;
	}
}

/*********************************************************************************************************
** ��������: CAN_QueueWrite
** ��������: дָ����������������
** �䡡��: Queue:ָ����е�ָ��
**         pData:��Ϣ����
** ������: NOT_OK     ����������
**         QUEUE_OK   ���յ���Ϣ
**         QUEUE_EMPTY������Ϣ
** ȫ�ֱ���: ��
** ����ģ��: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
** ������: Xsky
** �ա���: 2011-03-10 10:43
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
** �衡��: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
__inline static t_err CAN_QueueWrite(register T_CanQueue *Queue, T_CanFrame *pData)
{
	register t_err err;
    //T_CanFrame *Queue;
	register T_CanFrame *pIn;

    err = NOT_OK;
    if (Queue != NULL)                             /* �����Ƿ���Ч */
    {
        //Queue = (T_DataQueue *)Buf;
        
        //CAN_QUE_OS_ENTER_CRITICAL();
		CAN_INT_TX_DISABLE();						/* �رշ����ж� */
        
        if (Queue->NData < Queue->MaxData)         	/* �����Ƿ���  */
        {                                  			/* ����        */
        	pIn = Queue->In;
            *pIn = *pData;                 			/* �������    */
            ++pIn;
            if (pIn >= Queue->End)
            {
                pIn = Queue->Buf;
            }
            Queue->In = pIn;                    	/* �������ָ��*/
            Queue->NData++;                         /* ��������    */
            err = QUEUE_OK;
        }
        else
        {                                           /* ��           */
            err = QUEUE_FULL;
			#if (CAN_QUEUE_WRITE_FULL_EN > 0)
            if (Queue->WriteFull != NULL)        	/* �����û������� */
            {
                err = Queue->WriteFull(Queue->WriteFullParam);
            }
			#endif
		}
		CAN_INT_TX_ENABLE();						/* ���������ж� */
    }
    return err;
}

// �����ж�
__inline static t_err CAN_QueueWriteQuick(register T_CanQueue *Queue, T_CanFrame *pData)
{
	register t_err err;
    //T_CanFrame *Queue;
	register T_CanFrame *pIn;
	
    err = NOT_OK;
    if (Queue != NULL)                             /* �����Ƿ���Ч */
    {
        //Queue = (T_DataQueue *)Buf;
        
        //CAN_QUE_OS_ENTER_CRITICAL();
        
        if (Queue->NData < Queue->MaxData)         	/* �����Ƿ���  */
        {                                  			/* ����        */
        	pIn = Queue->In;
            *pIn = *pData;                 			/* �������    */
            ++pIn;
            if (pIn >= Queue->End)
            {
                pIn = Queue->Buf;
            }
            Queue->In = pIn;                    	/* �������ָ��*/
            Queue->NData++;                         /* ��������    */
            err = QUEUE_OK;
        }
        else
        {                                           /* ��           */
            err = QUEUE_FULL;
			#if (CAN_QUEUE_WRITE_FULL_EN > 0)
            if (Queue->WriteFull != NULL)        	/* �����û������� */
            {
                err = Queue->WriteFull(Queue->WriteFullParam);
            }
			#endif
		}
		//CAN_QUE_OS_EXIT_CRITICAL();
    }
    return err;
}

#ifndef CAN_QUEUE_READ_EN
#define  CAN_QUEUE_READ_EN		0
#endif
#if CAN_QUEUE_READ_EN > 0
/*********************************************************************************************************
** ��������: CAN_QueueRead
** ��������: ��ȡ�����е�һ�����ݵ�Ԫ
** �䡡��: Queue:ָ����е�ָ��
**         pData:���ݴ�ŵ�
** ������: NOT_OK     ����������
**         QUEUE_OK   ���յ���Ϣ
**         QUEUE_EMPTY������Ϣ
** ȫ�ֱ���: ��
** ����ģ��: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
** ������: Xsky
** �ա���: 2011-03-11 10:00
**-------------------------------------------------------------------------------------------------------
** �޸���: Xsky
** �ա���: 
** �衡��: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
__inline t_err CAN_QueueRead(register T_CanQueue *Queue, T_CanFrame *pData)
{
	register t_err err;
	//register    T_DataQueue *Queue;
	register uint16 uData;
	register T_CanFrame *pOut, *pOutOld;
	
	err = NOT_OK;
	
	if (Queue != NULL)                              /* �����Ƿ���Ч */
	{                                               /* ��Ч */
		//Queue = (T_DataQueue *)Buf;
		
		//CAN_QUE_OS_ENTER_CRITICAL();
		CAN_INT_RX_DISABLE();						/* �ر����н����ж� */
		
		if (Queue->NData > 0)                       /* �����Ƿ�Ϊ�� */
		{                                    		/* ����      */
			//*Ret = Queue->Out[0];                 /* ���ݳ���  */
			pOut = Queue->Out;
			pOutOld = pOut;
			//*pData = *(pOut);
			++pOut;
			if (pOut >= Queue->End)
			{
				pOut = Queue->Buf;
			}
			Queue->Out = pOut;                  	/* ��������ָ�� */
			Queue->NData--;                         /* ���ݼ���      */
			
			CAN_INT_RX_ENABLE();					/* �������н����ж� */
			*pData = *pOutOld;						/* �����ж��ٸ��������Լ����ж�ʱ�� */
			err = QUEUE_OK;
		}
		else
		{                                           /* ��              */
			CAN_INT_RX_ENABLE();					/* �������н����ж� */
			
			err = QUEUE_EMPTY;
			#if (CAN_QUEUE_READ_EMPTY_EN > 0)
			if (Queue->ReadEmpty != NULL)           /* �����û������� */
			{
				err = Queue->ReadEmpty(Queue);
			}
			#endif
		}
		//CAN_QUE_OS_EXIT_CRITICAL();
	}
	return err;
}
#endif	// end: #if CAN_QUEUE_READ_EN > 0

// ���ж�
__inline static t_err CAN_QueueReadQuick(T_CanFrame *pData, register T_CanQueue *Queue)
{
	register t_err err;
	//register    T_DataQueue *Queue;
	register T_CanFrame *pOut;
	
	err = NOT_OK;
	
	if (Queue != NULL)                           	/* �����Ƿ���Ч */
	{                                               /* ��Ч */
		//Queue = (T_DataQueue *)Buf;
		
		//CAN_QUE_OS_ENTER_CRITICAL();
		
		if (Queue->NData > 0)                       /* �����Ƿ�Ϊ�� */
		{                                           /* ����         */
			//*Ret = Queue->Out[0];                 /* ���ݳ���     */
			pOut = Queue->Out;
			*pData = *pOut;
			++pOut;
			if (pOut >= Queue->End)
			{
				pOut = Queue->Buf;
			}
			Queue->Out = pOut;                  	/* ��������ָ�� */
			Queue->NData--;                         /* ���ݼ���      */
			err = QUEUE_OK;
		}
		else
		{                                           /* ��              */
			err = QUEUE_EMPTY;
			#if (CAN_QUEUE_READ_EMPTY_EN > 0)
			if (Queue->ReadEmpty != NULL)           /* �����û������� */
			{
				err = Queue->ReadEmpty(Queue);
			}
			#endif
		}
		//CAN_QUE_OS_EXIT_CRITICAL();
	}
	return err;
}

/*********************************************************************************************************
** ��������: CAN_ReadFrame
** ��������: ��ȡ�������ݶ����е�һ�����ݵ�Ԫ
** �䡡��: 
**         pData:���ݴ�ŵ�
** ������: NOT_OK     ����������
**         QUEUE_OK   ���յ���Ϣ
**         QUEUE_EMPTY��������
** ȫ�ֱ���: ��
** ����ģ��: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
** ������: Xsky
** �ա���: 2011-03-11 10:00
**-------------------------------------------------------------------------------------------------------
** �޸���: Xsky
** �ա���: 
** �衡��: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
t_err CAN_ReadFrame(T_CanFrame *pData)
{
	register t_err err;
	//register    T_DataQueue *Queue;
	register T_CanQueue *Queue = &CanRxQueue;
	register T_CanFrame *pOut, *pOutOld;
	
	err = NOT_OK;
	
	if (Queue != NULL)                              /* �����Ƿ���Ч */
	{                                               /* ��Ч */
		//Queue = (T_DataQueue *)Buf;
		#ifdef DBG_CAN_QUE_MAX
			// ����,��¼���ն�����󻺳���
			if(CanRxQueue.NData < g_History.SysErrors.CAN_RcvQueMaxN){
				g_History.SysErrors.CAN_RcvQueMaxN = CanRxQueue.NData;
			}
		#endif
		
		CAN_INT_RX_DISABLE();						/* �ر����н����ж� */
		
		if (Queue->NData > 0)                       /* �����Ƿ�Ϊ�� */
		{                                    		/* ����         */
			//*Ret = Queue->Out[0];                 /* ���ݳ���     */
			pOutOld = Queue->Out;
			//*pData = *(pOut);
			pOut = pOutOld + 1;
			if (pOut >= Queue->End)
			{
				pOut = Queue->Buf;
			}
			Queue->Out = pOut;                  	/* ��������ָ�� */
			Queue->NData--;                         /* ���ݼ���      */
			
			CAN_INT_RX_ENABLE();					/* �������н����ж� */
			*pData = *pOutOld;						/* �����ж��ٸ��������Լ����ж�ʱ�� */
			err = QUEUE_OK;
		}
		else
		{                                           /* ��              */
			CAN_INT_RX_ENABLE();					/* �������н����ж� */
			
			err = QUEUE_EMPTY;
			#if (CAN_QUEUE_READ_EMPTY_EN > 0)
			if (Queue->ReadEmpty != NULL)           /* �����û������� */
			{
				err = Queue->ReadEmpty(Queue);
			}
			#endif
		}
		//CAN_QUE_OS_EXIT_CRITICAL();
	}
	
	return err;
}


/*********************************************************************************************************
** ��������: CAN_wrFilterExt32MaskMode
** ��������: ����CAN ID������:��ȷ��ģʽ: 32λ, ����λģʽ
** �䡡��:  1. filter_number: ��������,0~13
**			2. id: 	��ʶ���Ĵ���
**			3. mask: ��ʶ������(���ʶ���Ĵ�������λ�Ա�),1:����ƥ��, 0: ���ù���
**			4. fifo_number: ������FIFO, 0: ����FIFO0, 1:����FIFO1
** �䡡��: ��
** ������: �������
** ȫ�ֱ���: ��
** ����ģ��: 
** �桡��: v0.00.1
** ������: Xsky
** �ա���: 2010-03-17 11:50
** ����ע: 
**-------------------------------------------------------------------------------------------------------
** �桡��: 
** �޸���: 
** �ա���: 
** �衡��: 
********************************************************************************************************/
void CAN_wrFilterExt32MaskMode (register t_ureg filter_number, 
								uint32 id, uint32 mask, uint32 fifo_number)  
{
	register uint32 FilterNOBitPos;
  	
	if (filter_number > 13) {                       // check if Filter Memory is full
		return;
	}
	FilterNOBitPos = 1 << filter_number;
	//CAN_FilterInit(CAN_FilterInitTypeDef * CAN_FilterInitStruct)
	CAN1->FMR  |=  CAN_FMR_FINIT;       			// �������������ģʽ
	CAN1->FA1R &=  ~(uint32_t)(FilterNOBitPos); 	// ����ָ�����������Խ�������״̬
	
	CAN1->FS1R |= (uint32_t)(FilterNOBitPos);		// ���ù�����λ��Ϊ32λ
	CAN1->FM1R &= ~(uint32_t)(FilterNOBitPos);		// ����Ϊ����λģʽ
	
	CAN1->sFilterRegister[filter_number].FR1 = id; 	//  ID
	CAN1->sFilterRegister[filter_number].FR2 = mask;//  Mask
	
	if(fifo_number == 0){
		CAN1->FFA1R &= ~(uint32_t)(FilterNOBitPos);	// ������������FIFO 0
	}else{
		CAN1->FFA1R |= (uint32_t)(FilterNOBitPos);	// ������������FIFO 1
	}
	
	CAN1->FA1R  |=  (uint32_t)(FilterNOBitPos);  	// ����������
	
	CAN1->FMR &= ~CAN_FMR_FINIT;                    // ��������ģʽ

}

/**
* @brief	Configures the CAN.
* @param	None
* @retval None
*/
void CAN_Config(void)
{
	NVIC_InitTypeDef  NVIC_InitStructure;

	CAN_InitTypeDef 	   CAN_InitStructure;
	//CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	
	GPIO_InitTypeDef	GPIO_InitStructure;
	#if CAN1_SWI_HANDLE_EN > 0					/* Add by Xsky 2011-06-18 14:55 */
	EXTI_InitTypeDef EXTI_InitStructure;
	#endif
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_CAN);
	
	// CAN_RX0�����ж�
	#ifndef STM32F10X_CL
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	#else
	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
	#endif // STM32F10X_CL
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	// CAN1_RX1�����ж�
	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	// CAN�����ж�
	#ifndef STM32F10X_CL
	NVIC_InitStructure.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;
	#else
	NVIC_InitStructure.NVIC_IRQChannel = CAN1_TX_IRQn;
	#endif // STM32F10X_CL
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	#if CAN1_SWI_HANDLE_EN > 0					/* Add by Xsky 2011-06-18 14:55 */
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = CAN1_SWI_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* Connect Key Button EXTI Line to Key Button GPIO Pin */
	GPIO_EXTILineConfig(CAN1_SWI_GPIO_PORT_SOURCE, CAN1_SWI_GPIO_PIN_SOURCE);
	/* Configure Key Button EXTI Line to generate an interrupt on falling edge */  
	EXTI_InitStructure.EXTI_Line = CAN1_SWI_EXTI_LINE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	#endif	/* end: #if CAN1_SWI_HANDLE_EN > 0 */
	
	// CAN1״̬�ı��ж�
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_CAN_SCE);	// �����ڵ����ȼ���
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = CAN1_SCE_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* GPIO clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO |RCC_APB2Periph_GPIO_CAN, ENABLE);
	
	/* Configure CAN pin: RX */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_CAN_RX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_CAN, &GPIO_InitStructure);
	
	/* Configure CAN pin: TX */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_CAN_TX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_CAN, &GPIO_InitStructure);
	
	GPIO_PinRemapConfig(GPIO_Remap_CAN , ENABLE);
	
	/* CAN1 Periph clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	
	/* CAN register init */
	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);
	
	/* CAN cell init , Datasheet.CN:p451 */
	CAN_InitStructure.CAN_TTCM = DISABLE;		// ʱ�䴥��ͨ��ģʽ:�ر�
	CAN_InitStructure.CAN_ABOM = ENABLE;		// �Զ����߹���
	CAN_InitStructure.CAN_AWUM = DISABLE;		// �Զ�����:�ر�
	#ifdef DEBUG
	CAN_InitStructure.CAN_NART = ENABLE;// ����
	#else
	CAN_InitStructure.CAN_NART = DISABLE;		// ��ֹ�Զ��ش�ģʽ: ENABLE,CAN����ֻ������1�Σ����ܷ��͵Ľ����Σ��ɹ���������ٲö�ʧ��; DISABLE:����CAN��׼��CANӲ���ڷ��ͱ���ʧ��ʱ��һֱ�Զ��ش�ֱ�����ͳɹ���
	#endif
	CAN_InitStructure.CAN_RFLM = DISABLE;		// ��������FIFO: �ڽ������ʱFIFOδ��������������FIFO�ı���δ����������һ���յ��ı��ĻḲ��ԭ�б���
	CAN_InitStructure.CAN_TXFP = ENABLE;		// FIFO���ȼ�: ͨ����CAN_MCR�Ĵ�����TXFPλ�á�1�������԰ѷ�����������Ϊ����FIFO���ڸ�ģʽ�£����͵����ȼ��ɷ���������������
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;// CAN����ģʽ
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;	// ����ͬ����Ծ���
	CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;	// ʱ���1
	CAN_InitStructure.CAN_BS2 = CAN_BS2_5tq;	// ʱ���2
	CAN_InitStructure.CAN_Prescaler = stm32_GetPCLK1()/(1+6+5)/CAN_BPS;// ��Ƶ����:
	CAN_Init(CAN1, &CAN_InitStructure);
	
	//_DbgPrintf_(( "PCLK1:%u,2:%u\r\n", stm32_GetPCLK1(),stm32_GetPCLK2() ));
	
	/* CAN filter init */
	/* Datasheet.CN:22.7.4,P431 */
	/*
	CAN_FilterInitStructure.CAN_FilterNumber = 0;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);
	*/
	/* �����ⲿ��������ʼ������,��˴���ʶ��������ͨ��Э������ */
	CAN_FilterConfig();
	
	/* CAN FIFO0 message pending interrupt enable */ 
	/* һ����FIFO����һ�����ģ�Ӳ���ͻ����FMP[1:0]λ���������CAN_IER�Ĵ�����FMPIEλΪ��1������ô�ͻ����һ���ж����� */
	/* ����:FIFO0��Ϣ�Һ��ж�CAN_IT_FMP0|CAN_IT_FMP1, 
	   		����������ж�CAN_IT_TME, Datasheet.CN:p445 */
	CAN_ITConfig(CAN1, CAN_IT_FMP0 | CAN_IT_FMP1 | CAN_IT_TME	/* ���ͽ����ж� */
					  |	CAN_IT_ERR | CAN_IT_LEC | CAN_IT_BOF | CAN_IT_EPV /* ���Ƽ�״̬�仯�ж� */
					  | CAN_IT_EWG , 
				 ENABLE);						//ʹ��ָ����CAN�ж�
	
	/* ��ʼ�������Ĵ��� */
	CAN1->ESR |= CAN_ESR_LEC_USER;				// ���ϴδ��������Ϊ�û�״̬(���򲻻����ø�״̬)�������µ�״̬
	
	/* Ӧ�ó�����г�ʼ�� */
	// TX, ���Ͷ���
	CAN_QueueCreate(&CanTxQueue, CanTxBuf, lenof(CanTxBuf), 
					CAN_QueueWriteFull, CAN_TxRx_Tx);			// ���Ͷ���
	// ���ն���RX
	CAN_QueueCreate(&CanRxQueue, CanRxBuf, lenof(CanRxBuf), 
					CAN_QueueWriteFull, CAN_TxRx_Rx);			// ���ն���
	
}

#if 1==0
/**
  * @brief  Configures the CAN, transmit and receive by polling
  * @param  None
  * @retval PASSED if the reception is well done, FAILED in other case
  */
TestStatus CAN_Polling(void)
{
	CAN_InitTypeDef        CAN_InitStructure;
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	CanTxMsg TxMessage;
	CanRxMsg RxMessage;
	uint32_t i = 0;
	uint8_t TransmitMailbox = 0;

	/* CAN register init */
	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);

	/* CAN cell init */
	CAN_InitStructure.CAN_TTCM=DISABLE;
	CAN_InitStructure.CAN_ABOM=DISABLE;
	CAN_InitStructure.CAN_AWUM=DISABLE;
	CAN_InitStructure.CAN_NART=DISABLE;
	CAN_InitStructure.CAN_RFLM=DISABLE;
	CAN_InitStructure.CAN_TXFP=DISABLE;
	CAN_InitStructure.CAN_Mode=CAN_Mode_LoopBack;
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1=CAN_BS1_8tq;
	CAN_InitStructure.CAN_BS2=CAN_BS2_7tq;
	CAN_InitStructure.CAN_Prescaler=5;
	CAN_Init(CAN1, &CAN_InitStructure);

	/* CAN filter init */
	CAN_FilterInitStructure.CAN_FilterNumber=0;
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;
	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=0;
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);

	/* transmit */
	TxMessage.StdId=0x11;
	TxMessage.RTR=CAN_RTR_DATA;
	TxMessage.IDE=CAN_ID_STD;
	TxMessage.DLC=2;
	TxMessage.Data[0]=0xCA;
	TxMessage.Data[1]=0xFE;

	TransmitMailbox=CAN_Transmit(CAN1, &TxMessage);
	i = 0;
	while((CAN_TransmitStatus(CAN1, TransmitMailbox) != CANTXOK) && (i != 0xFF))
	{
		i++;
	}

	i = 0;
	while((CAN_MessagePending(CAN1, CAN_FIFO0) < 1) && (i != 0xFF))
	{
		i++;
	}

	/* receive */
	RxMessage.StdId=0x00;
	RxMessage.IDE=CAN_ID_STD;
	RxMessage.DLC=0;
	RxMessage.Data[0]=0x00;
	RxMessage.Data[1]=0x00;
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);

	if (RxMessage.StdId!=0x11)
	{
		return FAILED;  
	}

	if (RxMessage.IDE!=CAN_ID_STD)
	{
		return FAILED;
	}

	if (RxMessage.DLC!=2)
	{
		return FAILED;  
	}
	
	if ((RxMessage.Data[0]<<8|RxMessage.Data[1])!=0xCAFE)
	{
		return FAILED;
	}
	
	return PASSED; /* Test Passed */
}
#endif	// #end: #if 1== 0

#if CAN1_SCE_IRQ_Handler_EXT_EN == 0
/* CAN1���Ƽ�״̬�仯�ж� */
void CAN1_SCE_IRQHandler (void)
{
	
}
#endif		// end: #if CAN1_SCE_IRQ_Handler_EXT_EN == 0

/*----------------------------------------------------------------------------
* @brief	This function handles CAN1 Handler.
* @param	None
* @retval None
*----------------------------------------------------------------------------*/
void CAN1_RX0_IRQHandler_Name (void)	// CAN1_RX0_IRQHandler_Name
{
	/*
	FIFO�ӿ�״̬��ʼ���ڽ��յ���һ����Ч�ı��ĺ�FIFO״̬��Ϊ�Һ�_1(pending_1)��
	Ӳ����Ӧ�ذ�CAN_RFR�Ĵ�����FMP[1:0]����Ϊ��01��(������01b)��
	������Զ�ȡFIFO������������������еı��ģ�Ȼ��ͨ����CAN_RFR�Ĵ�����RFOMλ
	���á�1�����ͷ����䣬����FIFO�ֱ�Ϊ��״̬�ˡ�������ͷ������ͬʱ��
	���յ���һ����Ч�ı��ģ���ôFIFO��Ȼ�����ڹҺ�_1״̬��������Զ�ȡFIFO
	����������������յ��ı��ġ�
	���Ӧ�ó����ͷ����䣬�ڽ��յ���һ����Ч�ı��ĺ�FIFO״̬��Ϊ
	�Һ�_2(pending_2)��Ӳ����Ӧ�ذ�FMP[1:0]����Ϊ��10��(������10b)��
	�ظ�����Ĺ��̣���������Ч�ı��İ�FIFO��Ϊ�Һ�_3״̬(FMP[1:0]��11b)��
	��ʱ����������RFOMλ����1���ͷ����䣬�Ա�FIFO�����пռ��������һ����Ч��
	���ģ�������һ����Ч�ı��ĵ���ʱ�ͻᵼ��һ�����ĵĶ�ʧ��
	*/
	while (CAN1->RF0R & CAN_RF0R_FMP0) 			      	// message pending ?
	{
		CAN_QueueWriteQuick(&CanRxQueue, (T_CanFrame *)&CAN1->sFIFOMailBox[CAN_FIFO0]);
		CAN1->RF0R |= CAN_RF0R_RFOM0;					// Release FIFO 0 output mailbox
		
		#if CAN1_SWI_HANDLE_EN > 0						/* Add by Xsky 2011-06-18 15:48 */
		EXTI->SWIER |= CAN1_SWI_EXTI_LINE;				/* Add by Xsky 2011-06-18 15:47 */
		#endif
	}
}
void CAN1_RX1_IRQHandler (void) 
{
	while (CAN1->RF1R & CAN_RF1R_FMP1) 			      	// message pending ?
	{
		CAN_QueueWriteQuick(&CanRxQueue, (T_CanFrame *)&CAN1->sFIFOMailBox[CAN_FIFO1]);
		CAN1->RF1R |= CAN_RF1R_RFOM1;					// Release FIFO 1 output mailbox

		#if CAN1_SWI_HANDLE_EN > 0						/* Add by Xsky 2011-06-18 15:48 */
		EXTI->SWIER |= CAN1_SWI_EXTI_LINE;				/* Add by Xsky 2011-06-18 15:47 */
		#endif
	}
}

/*----------------------------------------------------------------------------
  CAN transmit interrupt handler
 *----------------------------------------------------------------------------*/
void CAN1_TX_IRQHandler_Name(void)	// CAN1_TX_IRQHandler_Name
{
	register t_ureg dw_r;
	register CAN_TxMailBox_TypeDef *pTxMailBox;
	// Datasheet.CN: p445
	
    /* ����FIFO���ж� */
    if(CanTxQueue.NData > 0)
    {
    	// ������Ͷ��в�Ϊ��
		dw_r = (CAN1->TSR >> CAN_TSR_TME_BITnL) & 0x07;	// ���ҿյķ�������
		if(dw_r)
		{
			// �п��еķ�������
			dw_r = TxMailBox_IdleNO_Tbl[dw_r];			// ��ȡһ���������
			pTxMailBox = &CAN1->sTxMailBox[dw_r];
			CAN_QueueReadQuick((T_CanFrame *)pTxMailBox, &CanTxQueue);
			// ������
			SETBITs(pTxMailBox->TIR, CAN_TIxR_TXRQ);
		}
    }
    else
    {
		/* Clear CAN_TSR_RQCPx (rc_w1) */
		CAN1->TSR = CAN_TSR_RQCP0|CAN_TSR_RQCP1|CAN_TSR_RQCP2;	// �����������,�Է�ֹ�ٴν����ж�, Add by Xsky 2011-07-30 17:51
	}
}

/*********************************************************************************************************
** ��������: CAN_SendBytes
** ��������: ����������Ϊ�����͵�CAN���������ʽT_CanFrame/CAN_TypeDef
** 			 ����ʱ��������п���ֱ�ӽ�����ѹ�����䣬��������ѹ�����
**			 �������ʧ�ܴ���Ϊ3��ȡ������
** �䡡��:  1. : *pSrc:  Ҫ���͵�����Դ������
**			2. : nBytes: Ҫ���͵������ֽ���
**			3. : 
** �䡡��: ��
** ������: �������
** ȫ�ֱ���: ��
** ����ģ��: 
** �桡��: v0.00.1
** ������: Xsky
** �ա���: 2010-03-11 10:14
** ����ע: 
**-------------------------------------------------------------------------------------------------------
** �桡��: 
** �޸���: 
** �ա���: 
** �衡��: 
********************************************************************************************************/
t_err CAN_SendFrame(T_CanFrame *pCanFrame)
{
	//T_CanFrame CanFrame = {0, };
	register CAN_TxMailBox_TypeDef *pTxMailBox;
	register t_ureg dw_r;
	t_err err = ERR_NONE;
	/* ���ͱ��ĵ�����Ϊ��Ӧ�ó���ѡ��1�����õķ������䣻���ñ�ʶ����
	   ���ݳ��Ⱥʹ��������ݣ�Ȼ���CAN_TIxR�Ĵ�����TXRQλ�á�1�����������͡� 
	   ����״̬CAN_TSR,Datasheet.CN:p441 */
	
	CAN_QUE_OS_ENTER_CRITICAL();				// Add by Xsky 2011-07-30 14:26
	
	if(CanTxQueue.NData > 0)
	{
		// ���в�Ϊ����ֱ��д�뷢�Ͷ���
		#ifdef DBG_CAN_QUE_MAX
			// ����,��¼���Ͷ������洢����
			if(CanTxQueue.NData < g_History.SysErrors.CAN_SendQueMaxN){
				g_History.SysErrors.CAN_SendQueMaxN = CanTxQueue.NData;
			}
		#endif
		
		err = CAN_QueueWrite(&CanTxQueue, pCanFrame);
	}
	else
	{
		// �������Ϊ��
		
		dw_r = (CAN1->TSR >> CAN_TSR_TME_BITnL) & 0x07;
		if(dw_r)	// �п��еķ�������
		{
			//CAN_INT_TX_DISABLE();	
			// �����ʹ�ò���ϵͳ���Ҳ����ж��е��ô˺�������������ɲ��ر��ж�
			
			pTxMailBox = &CAN1->sTxMailBox[TxMailBox_IdleNO_Tbl[dw_r]];
			//pTxMailBox->TIR = pCanFrame->IxR;
			pTxMailBox->TDTR = pCanFrame->DTxR;
			pTxMailBox->TDLR = pCanFrame->Data.u32[0];
			pTxMailBox->TDHR = pCanFrame->Data.u32[1];
			//SETBITs(pTxMailBox->TIR, CAN_TIxR_TXRQ);
			pTxMailBox->TIR = pCanFrame->IxR | CAN_TIxR_TXRQ;
			
			//CAN_INT_TX_ENABLE();	// ���������ж�
		}
		else		// �޿�������
		{
			err = CAN_QueueWrite(&CanTxQueue, pCanFrame);
		}
	}
	
	CAN_QUE_OS_EXIT_CRITICAL();					// Add by Xsky 2011-07-30 14:26
	
	return err;
}


#if (CAN1_SWI_HANDLE_EN > 0) && (CAN1_SWI_IRQHandler_EXT_EN==0)		/* Add by Xsky 2011-06-18 14:55 */
void EXTI4_IRQHandler(void)
{
	// CAN���յ����ݺ���ѹ��FIFO��Ȼ�󴥷����жϷ����Ӧ������ж�
}

#endif	/* end: #if CAN1_SWI_HANDLE_EN > 0 */


#endif	/* end: #if CAN1_EN > 0 */

