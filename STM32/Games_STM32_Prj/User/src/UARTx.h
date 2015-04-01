/****************************************Copyright (c)**************************************************
**                                  vixa
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: UARTx.h
**��   ��   ��: Xsky
**�� �� ��  ��: 2011-07-28 19:28
**��        ��: UART��ع�������, Ϊ����ʵ�壬���԰�����UARt1...n.c�ļ�����ʵ�ִ�������
** ��  ��: v1.0.00
**------------------------------------------------------------------------------------------------------
** ����ļ�: 
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

// �ֲ�����
#if UARTx_TX_EN > 0
static uint8	TxQueueBuf[UARTx_Tx_BufSize];
#define pTxBuf  ((T_DataQueue *)TxQueueBuf)
#endif
#if UARTx_RX_EN > 0
static uint8	RxQueueBuf[UARTx_Rx_BufSize];
#define pRxBuf  ((T_DataQueue *)RxQueueBuf)
	#if UARTx_DMA_EN > 0 && UARTx_DMA_RX_EN > 0
static uint8 	RxDMABuf[UARTx_Rx_DMABufSize];
	#endif
	#if (UARTx_RX_TIM_EN > 0) //&& (UARTx_RX_TIM_CC_EN == 0)				   
static t_TimeStatus RxTimeOutFlag = c_InTime;								   
	#endif
#endif



/***************************************************************************************************/
// ����, UART��������
#if UARTx_TX_EN > 0
#if UARTx_DMA_EN > 0
static uint16 DMA_Tx_N = 0;	// ��¼DMA�����ʼ����ʱ��Ŀ
/*********************************************************************************************************
** ��������: UARTx_DMA_Tx
** ��������: UARTx�����Ͷ����е����ݳ����е�ַ,�Լ�����ͷ�������ֽ���
**				д�뷢��DMA,������DMA����
**			 ע:(1). ʹ���˶��в�������,ֻ����DMA�жϼ��������е���
**				 	 �����ж��е��ÿ��ܻ�������������
**				(2). DMAֻ���ò��ֱ�Ҫ����������ģʽ���������ڳ�ʼ��ʱ����
**					
** �䡡��: ��
** �䡡��: ��
** ������: ������
** ȫ�ֱ���: ��
** ����ģ��: 
** �桡��: v1.0.00
** ������: Xsky
** �ա���: 2011-04-02 15:49
** ����ע: 
**-------------------------------------------------------------------------------------------------------
** �桡��: 
** �޸���: 
** �ա���: 
** �衡��: 
********************************************************************************************************/
static __INLINE void UARTx_DMA_Tx(void)
{
	/* USART.DMA.Datasheet.CN:p535 */
	/* DMA.Datashett.CN: p147 */
	register uint32_t N;
	register uint8 *pOut, *pEnd;
	
	/* Disable the selected DMAy Channelx */
	UARTx_DMA_Tx_Channel->CCR &= (uint16_t)(~DMA_CCR1_EN);
	
	/* ���ͻ����� */
	pOut = pTxBuf->Out;
	UARTx_DMA_Tx_Channel->CMAR = (uint32_t)(pOut);
	
	/* ���������͵��ֽ��� */
	pEnd = pTxBuf->End;
	N = pTxBuf->NData * sizeof(QUEUE_DATA_TYPE);
	if(N > 64) N = 64;					// ��С��ȴ�ʱ��, Add by Xsky 2011-07-29 15:52
	if ( (uint)pOut + N > (uint)pEnd )	// ������е�ͷ��ֻ����ǰ����
	{
		N = pEnd - pOut;
	}
	
	DMA_Tx_N = N;
	UARTx_DMA_Tx_Channel->CNDTR = N;
	
	/* ��������DMAͨ�� */
	UARTx_DMA_Tx_Channel->CCR |= DMA_CCR1_EN;
	
	/* ����UART����DMA���� */
	// UARTx->CR3 |= USART_DMAReq_Tx;	// ��ʼ��ʱ���ø�λ����, Del by Xsky 2011-05-24 16:08
}

/*********************************************************************************************************
** ��������: UARTx_DMA_TX_IRQHandler/DMAx_ChannelN_IRQHandler
** ��������: TX����DMA�жϴ�����
**				������������������DMA,������DMA����
**			 ע:(1). 
** �䡡��: ��
** �䡡��: ��
** ������: ������
** ȫ�ֱ���: ��
** ����ģ��: 
** �桡��: v1.0.00
** ������: Xsky
** �ա���: 2011-04-02 15:49
** ����ע: 
**-------------------------------------------------------------------------------------------------------
** �桡��: 
** �޸���: 
** �ա���: 
** �衡��: 
********************************************************************************************************/
void UARTx_DMA_Tx_IRQHandler(void)
{
	register t_ureg ISR;
	register T_DataQueue *Queue;
	
	ISR = UARTx_DMA->ISR;
	if(ISR & UARTx_DMA_Tx_IF_TC)	// DMA��������жϱ�־
	{
	    // ����жϱ�־
	    UARTx_DMA->IFCR = UARTx_DMA_Tx_IF_TC;
		
		UARTx_ENTER_CRITICAL_TX();	// ����������жϳ�����Ҳ���ô��ڷ��ͺ������������ж�
		// �����м�ȥDMA������ɵ�����
		Queue = pTxBuf;
	    QueueOutN(Queue, DMA_Tx_N);
	    DMA_Tx_N = 0;
		if(Queue->NData > 0)	// QueueNData(TxQueueBuf)
		{
			// ����δ��������, ���³�ʼ������
			UARTx_DMA_Tx();
		}
		else
		{
			// ����������ö���ͷβָ��, �Լ��ٵ�ͷ����
			//QueueFlush(TxQueueBuf);
			Queue->Out = Queue->Buf;
			Queue->In = Queue->Buf;
			Queue->NData = 0;											/* ������ĿΪ0 */
			// ������ɺ�DMA���������, Datasheet.CN:p536
			// �ɲ��ر�DMA
		}
		UARTx_EXIT_CRITICAL_TX();
	}
	// ����жϱ�־
	//UARTx_DMA->IFCR = UARTx_DMA_Tx_IF_GL; // �����������״̬��־����Send�����м��
	
	#if defined(DMA2_Channel4_IRQHook) && (UARTx_NO >= 4)
		#if (UARTx_DMA_Tx_Number == 0x0205) || (UARTx_DMA_Tx_Number == 0x0204)
		//ISR = DMA2->ISR;
		if(ISR & DMA2_FLAG_TC4)	// DMA��������жϱ�־
		{
			DMA2_Channel4_IRQHook();
		}
		#else
		#error	"UART1.C UARTx_DMA_TX_IRQHandler() Error! "
		#endif
	#endif	// #ifdef DMA2_Channel4_IRQHook
	
}
#endif		// end: #if UARTx_DMA_EN > 0 //

/*********************************************************************************************************
** ��������: UART1_SendBytes
** ��������: UART1���ͺ���, ��ָ�����ֽ�д�뷢�ͻ�����, �����������������ֱ�ӷ���, ��������������
**			������ѹ����еȴ�����
**			 ע: ʹ���˶��в�������,ֻ����DMA�жϼ��������е���
**				 �����ж��е��ÿ��ܻ�������������,�趨��_CRITICAL()����Ϊ����жϿ��غ���
** �䡡��:  1. *pSrc: ���ͻ�����
**			2. nBytes: �����������ֽ��� 
** �䡡��: ��
** ������: ������:  ERR_NONE
**			  	ERR_UsartTxQueueFull
** ȫ�ֱ���: ��
** ����ģ��: 
** �桡��: v1.0.00
** ������: Xsky
** �ա���: 2011-04-02 14:27
** ����ע: 
**-------------------------------------------------------------------------------------------------------
** �桡��: 
** �޸���: 
** �ա���: 
** �衡��: 
********************************************************************************************************/
t_err UARTx_SendBytes(void *pSrc, t_size nBytes)
{
	register t_err err;	
	//register T_DataQueue *Queue;
	
#if UARTx_DMA_EN > 0		// ����DMA
	// ֱ��д�����������Ͷ���
	if(nBytes == 0)											// Add by Xsky 2011-07-29 17:44
	{
		return ERR_NONE;
	}
	UARTx_ENTER_CRITICAL_TX();
	err = QueueWriteBytes(TxQueueBuf, pSrc, nBytes);
	if(err == QUEUE_OK)
	{
		if(DMA_Tx_N == 0)									// ����ϴδ�������� 
		//if CHKNBITs(UARTx_DMA->ISR, UARTx_DMA_Tx_IF_TC)	// 
		{
			UARTx_DMA_Tx();
		}
		
		err = ERR_NONE;
	}
	else	// ��������
	{
		if CHKBITs(UARTx_DMA->ISR, UARTx_DMA_Tx_IF_TE)
		{
			// ����������,��������־
			UARTx_DMA->IFCR = UARTx_DMA_Tx_IF_TE;
			// ���¿�������
			if((uint16)UARTx_DMA_Tx_Channel->CNDTR == 0)
			{
				UARTx_DMA_Tx();
			}
		}
		err = ERR_UsartTxQueueFull;
	}
	UARTx_EXIT_CRITICAL_TX();
#else					// δ����DMA
	register uint8_t Data;																	//rework by ���U��Ŀ 2011-11-23 08:53
	if( pTxBuf->NData + nBytes > pTxBuf->MaxData)											//rework by ���U��Ŀ 2011-11-14 10:32
	{
		// �������������ֱ���˳�
		return ERR_UsartTxQueueFull;
	}
	
	UARTx_ENTER_CRITICAL_TX();					// ���ж�									//rework by ���U��Ŀ 2011-11-23 08:53
	err = QueueWriteBytes(TxQueueBuf, pSrc, nBytes);										//
	UARTx_EXIT_CRITICAL_TX();					// ���ж�									//
	// �������ɷ�������																	//
	while(pTxBuf->NData > 0 && CHKBITs(UARTx->SR, USART_FLAG_TXE))							//
	{																						//
		QUEUE_READ_ISR_QUICK(Data, pTxBuf);													//
		UARTx->DR = Data;																	//
	}																						//rework by ���U��Ŀ 2011-11-23 08:53
	
	
	/*if(err == QUEUE_OK)
	{
		err = ERR_NONE;
	}
	else	// ��������
	{
		err = ERR_UsartTxQueueFull;
	}*/
	err = ERR_NONE;
#endif
	
	return err;
}
#endif		// end: #if UARTx_TX_EN > 0

#if UARTx_RX_EN > 0
#if UARTx_DMA_EN > 0 && UARTx_DMA_RX_EN > 0
//static uint16 DMA_Rx_N = 0;		// ��¼����DMA�����ʼ����ʱ��Ŀ                  	 			 //del by ���U��Ŀ 2011-11-08 17:05
//static uint16 DMA_Rx_NAll = 0;	// ��¼δд�����ն��е�DMA�ܴ�����Ŀ							 //del by ���U��Ŀ 2011-11-08 17:05
/*********************************************************************************************************
** ��������: UARTx_DMA_Rx
** ��������: UARTx�����ն����е���������е�ַ,�Լ�����ͷ�Ŀ����ֽ���
**				д�����DMA,������DMA����,��ȡ���յ�������
**			 ע:(1). ʹ���˶��в�������,ֻ����DMA�жϼ��������е���
**				 �����ж��е��ÿ��ܻ�������������,�趨��_CRITICAL()����Ϊ����жϿ��غ���
**				(2). DMAֻ���ò��ֱ�Ҫ����������ģʽ���������ڳ�ʼ��ʱ����
**					
** �䡡��: ��
** �䡡��: ��
** ������: ������
** ȫ�ֱ���: ��
** ����ģ��: 
** �桡��: v1.0.00
** ������: Xsky
** �ա���: 2011-04-02 15:49
** ����ע: 
**-------------------------------------------------------------------------------------------------------
** �桡��: 
** �޸���: 
** �ա���: 
** �衡��: 
********************************************************************************************************/
static void UARTx_DMA_Rx(uint8 *pDst, uint16 nBytes)
{
	/* USART.DMA.Datasheet.CN:p535 */
	/* DMA.Datashett.CN: p147 */
	//register uint32_t N;																			  //del by ���U��Ŀ 2011-11-08 17:05
	//register uint8 *pOut, *pEnd;
	
	/* Disable the selected DMAy Channelx */
	UARTx_DMA_Rx_Channel->CCR &= (uint16_t)(~DMA_CCR1_EN);
	
	/* ���ͻ����� */
	UARTx_DMA_Rx_Channel->CMAR = (uint32_t)pDst;
	
	/* ���͵��ֽ��� */
	//DMA_Rx_N = nBytes;																			   //del by ���U��Ŀ 2011-11-08 17:05
	UARTx_DMA_Rx_Channel->CNDTR = nBytes;
	
	/* ��������DMAͨ�� */
	UARTx_DMA_Rx_Channel->CCR |= (DMA_CCR1_EN | DMA_CCR1_TCIE | DMA_CCR1_HTIE);						   //rework by ���U��Ŀ 2011-11-08 17:05
	
	/* ����UART����DMA���� */
	UARTx->CR3 |= USART_DMAReq_Rx;
}

/*********************************************************************************************************
** ��������: UARTx_DMA_RX_IRQHandler/DMAx_ChannelN_IRQHandler
** ��������: Rx����DMA�жϴ�����
**			 ע:(1). 
** �䡡��: ��
** �䡡��: ��
** ������: ������
** ȫ�ֱ���: ��
** ����ģ��: 
** �桡��: v1.0.00
** ������: Xsky
** �ա���: 2011-04-03 08:40
** ����ע: 
**-------------------------------------------------------------------------------------------------------
** �桡��: 
** �޸���: 
** �ա���: 
** �衡��: 
********************************************************************************************************/
void  UARTx_DMA_Rx_IRQHandler (void)	// UARTx_DMA_RX_IRQHandler
{
	//register t_ureg ISR;
	register t_ureg u32r;
	
	u32r = UARTx_DMA->ISR;
	UARTx_ENTER_CRITICAL_RX();																		//rework by ���U��Ŀ 2011-11-08 17:05
		if (u32r&UARTx_DMA_Rx_IF_TC )	// DMA��������жϱ�־										//	ʹ��CHKBITs(x,msk)�������Uart4�յ�����
	{	    																						//	����DMA�ж϶���������ת��������Ǿ�
			QueueWriteBytes(RxQueueBuf, &RxDMABuf[UARTx_Rx_DMABufSize/2], UARTx_Rx_DMABufSize/2);	//
	}																								//
		if (u32r&UARTx_DMA_Rx_IF_HT)	// DMA��������жϱ�־										//	ʹ��CHKBITs(x,msk)�������Uart4�յ�����
	{	    																						//	����DMA�ж϶���������ת��������Ǿ�
			QueueWriteBytes(RxQueueBuf, RxDMABuf, UARTx_Rx_DMABufSize/2);							//
	}																								//
	UARTx_EXIT_CRITICAL_RX();																		//
	// ����жϱ�־																					//
	UARTx_DMA->IFCR = UARTx_DMA_Rx_IF_TC | UARTx_DMA_Rx_IF_HT;										//rework by ���U��Ŀ 2011-11-08 17:05
}

#endif 	// end: #if UARTx_DMA_EN > 0  && UARTx_DMA_RX_EN > 0 // 

#if (UARTx_RX_TIM_EN > 0)

#if UARTx_RX_TIM_CC_EN > 0
#else

// ֹͣ��ʱ��
#define  UARTx_Rx_TIM_Stop() 	CLRBITs(UARTx_Rx_TIM->CR1, TIM_CR1_CEN)
/*void UARTx_Rx_TIM_Stop(void)
{
	TIM_Cmd(UARTx_Rx_TIM, DISABLE); 
}*/

// ������ʱ��
#define  UARTx_Rx_TIM_Start()	UARTx_Rx_TIM->CNT = 0x0000; \
								SETBITs(UARTx_Rx_TIM->CR1, TIM_CR1_CEN); \
								RxTimeOutFlag = c_InTime;
/*void UARTx_Rx_TIM_Start(void)
{
	TIM_SetCounter(UARTx_Rx_TIM,0x0000);
	TIM_Cmd(UARTx_Rx_TIM, ENABLE);
}*/

/*******************************************************************************
* �ļ���	  	 : UARTx_Rx_TIM_Config
* ����	         : �����ַ���ʱ��ʱ����ʼ������
* 					�����Ϊ:�����жϲ���ʱ���ö�ʱ��������������ʱ��������ʱ����ʱ
*					˵���ڴ˶�ʱʱ���������ַ�����ֹͣ��ʱ������ֱ�Ӷ�ȡ����
* ����           : ��
* ���           : ��
* ����           : ��
*******************************************************************************/
static void UARTx_Rx_TIM_Config(void)
{
	/* Datasheet.CN
	** �߼����ƶ�ʱ��: p199, TIM1/TIM8
	** ������ʱ��: p298, TIM6/TIM7
	** ͨ�ö�ʱ��: p253, TIM2/3/4/5
	*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	//TIM_OCInitTypeDef TIM_OCInitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	TIM_DeInit(UARTx_Rx_TIM);
	
	// �ж����������ȼ�����
	//NVIC_PriorityGroupConfig(UARTx_Rx_TIM_NVICPrioGroup);
	NVIC_InitStructure.NVIC_IRQChannel = UARTx_Rx_TIM_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = UARTx_Rx_TIM_IRQPrio;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = UARTx_Rx_TIM_IRQPrioSub;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	UARTx_Rx_TIM_RCCCmd(ENABLE);
	TIM_ITConfig(UARTx_Rx_TIM, TIM_IT_Update, ENABLE);
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure); 
	TIM_TimeBaseInitStructure.TIM_Prescaler = CPU_HCLK/1000000 - 1;	// Ԥ��ƵΪ1us
	TIM_TimeBaseInitStructure.TIM_Period = UARTx_RX_TIM_OutCharN * 11 * 1000000 
											/ UARTx_BaudRate;		// ARR
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(UARTx_Rx_TIM,&TIM_TimeBaseInitStructure);
	TIM_SelectOnePulseMode(UARTx_Rx_TIM, TIM_OPMode_Single);		// ѡ��Ϊ������ģʽ
	/*
	TIM_OCStructInit(&TIM_OCInitStruct);
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_Timing;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OCInitStruct.TIM_Pulse = 4999;
	TIM_OC1Init(UARTx_Rx_TIM, &TIM_OCInitStruct);
	
	TIM_SelectInputTrigger(UARTx_Rx_TIM,TIM_TS_ITR1);
	*/
	TIM_ARRPreloadConfig(UARTx_Rx_TIM, ENABLE);
	
	//TIM_Cmd(UARTx_Rx_TIM, ENABLE); 
}

/*******************************************************************************
* ������	  	 : UARTx_Rx_TIM_IRQHandler
* ����	         : ���ճ�ʱ��ʱ���ж�, ֹͣ��ʱ���������ַ���ʱ��־
* ����           : ��
* ���           : ��
* ����           : ��
*******************************************************************************/
void UARTx_Rx_TIM_IRQHandler(void)	// UARTx_Rx_TIM_IRQHandler
{
	if CHKBITs(UARTx_Rx_TIM->SR, TIM_FLAG_Update)		/* �������жϱ�־λ */
	if CHKBITs(UARTx_Rx_TIM->DIER, TIM_FLAG_Update)		/* ����ж�ʹ��λ���ų�������ж� */
	{
		// ����жϱ�־
		//TIM_ClearFlag(UARTx_Rx_TIM,TIM_FLAG_Update);
		CLRBITs(UARTx_Rx_TIM->SR, TIM_FLAG_Update);
		
		// ֹͣ��ʱ��
		UARTx_Rx_TIM_Stop();
		// ���ó�ʱ��־
		RxTimeOutFlag = c_TimeOut;
	}
}
#endif	// end: #if UARTx_RX_TIM_EN > 0 //

/*******************************************************************************
* ������	  	 : UART1_isCharRcvTimeOut
* ����	         : ��ȡ�ַ���ʱ״̬,���һ�����յ����ַ�֮��
* ����           : ��
* ���           : ��
* ����           : 1:�ַ���ʱ, 0: δ��ʱ
t_bool UART1_isCharRcvTimeOut(void)
{
	// return RxTimeOutFlag = c_TimeOut;
	return RxTimeOutFlag;	// ע��˳����뾭��
}
*******************************************************************************/


#endif	// end: #if UARTx_RX_TIM_CC_EN > 0 //

/*******************************************************************************
* ������	  	 : UART1_RcvdSize / UARTx_RcvdSize
* ����	         : UART1���յ����ֽ���
*				  ע: ����ַ�δʱ�򷵻�0, �ַ���ʱ֮��ŷ���ʵ�ʽ��յ��ĳ���
* ����           : ��
* ���           : ��
* ����           : ��
*******************************************************************************/
t_size UARTx_RcvdSize(void)
{
	#if UARTx_RX_TIM_EN > 0
	#if UARTx_RX_TIM_CC_EN > 0
		return (UARTx_Rx_isTimeOut())? ((T_DataQueue *)RxQueueBuf)->NData: 0;
	#else
		return (RxTimeOutFlag==c_InTime)? ((T_DataQueue *)RxQueueBuf)->NData: 0;
	#endif
	#else
	return ((T_DataQueue *)RxQueueBuf)->NData;
	#endif
}

/*********************************************************************************************************
** ��������: UART1_ReadBytes
** ��������: ��ȡ���ջ������ڵ�����, 
**			ע: (1). ʹ���˶��в�������,��������������迼�Ƕ���ر��ж�
** �䡡��:  1. *Dst: ���ݷ�Ŀ�껺����
**			2. nBytes: �����������ֽ��� 
** �䡡��: ��
** ������: ʵ�ʶ�ȡ���ֽ��� 
** ȫ�ֱ���: ��
** ����ģ��: QueueReadBytes()
** �桡��: v1.0.00
** ������: Xsky
** �ա���: 2011-04-03 14:27
** ����ע: 
**-------------------------------------------------------------------------------------------------------
** �桡��: 
** �޸���: 
** �ա���: 
** �衡��: 
********************************************************************************************************/
t_size_opt UARTx_ReadBytes(void *pDst, t_size nBytes)
{
	return QueueReadBytes(pDst, nBytes, RxQueueBuf);
}

#endif	// end: #if UARTx_RX_EN > 0 //

//#if (UARTx_DMA_EN == 0) || (UARTx_DMA_RX_EN == 0)														//del by ���U��Ŀ 2011-11-08 17:05
/*******************************************************************************
* ������	  	 : USARTx_IRQHandler
* ����	         : UARTx�жϴ�����, ��ʹ���жϽ��ջ����жϷ���ʱ������/������
* ����           : ��
* ���           : ��
* ����           : ��
*******************************************************************************/
void UARTx_IRQHandler(void)	// UARTx_IRQHandler
{
	#if UARTx_DMA_RX_EN == 0 || UARTx_DMA_EN == 0													   //add by ���U��Ŀ 2011-11-08 17:05
	register T_DataQueue *RxQueue;
	#endif																							   //add by ���U��Ŀ 2011-11-08 17:05
	//#if UARTx_DMA_EN == 0 && UARTx_TX_EN > 0
	//register uint16 Data;
	//#endif
	#if UARTx_DMA_RX_EN > 0	&& UARTx_DMA_EN > 0														   //add by ���U��Ŀ 2011-11-08 17:05
	uint16_t n;																						   //
	if CHKBITs(UARTx->SR, USART_FLAG_IDLE)															   //
	{																								   //
		n = UARTx->DR;																				   //
		UARTx_ENTER_CRITICAL_RX();																	   //
		n = UARTx_DMA_Rx_Channel->CNDTR;															   //
		if(n>UARTx_Rx_DMABufSize/2)QueueWriteBytes(RxQueueBuf, RxDMABuf, UARTx_Rx_DMABufSize-n);	   //
		else QueueWriteBytes(RxQueueBuf, &RxDMABuf[UARTx_Rx_DMABufSize/2], UARTx_Rx_DMABufSize/2-n);   //
		UARTx_DMA_Rx(RxDMABuf,UARTx_Rx_DMABufSize);													   //
		UARTx_EXIT_CRITICAL_RX();												  					   //
	}																								   //
	#endif																							   //add by ���U��Ŀ 2011-11-08 17:05
	#if UARTx_DMA_RX_EN == 0 && UARTx_RX_EN > 0
	// �жϽ�������
	if CHKBITs(UARTx->SR, USART_FLAG_RXNE)			// ���ռķǿ�
	{
		// ������д�뻺����
		RxQueue = (T_DataQueue *)RxQueueBuf;
		UARTx_ENTER_CRITICAL_RX();
		QueueWriteQuickIsr(RxQueue, UARTx->DR);
		UARTx_EXIT_CRITICAL_RX();
		
		#if UARTx_RX_TIM_EN > 0
		if(RxQueue->NData > RxQueue->MaxData * 2 / 3)			// ��ֹ���ݲ��������ʱ, ���������ʱ�ж�, if(...), Add by Xsky 2011-07-29 15:35
		{
			// ���ó�ʱ��־
			RxTimeOutFlag = c_TimeOut;
		}
		else
		{
			// ������ʱ��ʱ��
			UARTx_Rx_TIM_Start();
		}
		#endif
	}
	#endif
	
/*	#if UARTx_DMA_EN == 0 && UARTx_TX_EN > 0												   //
	// ��������ж�, ��ʹ��DMA����ʱʹ��													   //
	if CHKBITs(UARTx->SR, USART_FLAG_TC | USART_FLAG_TXE)// ��������ж�/���ͼĴ������ж�	   //
	{																						   //
		UARTx_ENTER_CRITICAL_TX();					// ���ж�								   //
		if(pTxBuf->NData == 0)	// �������Ϊ��												   //del by ���U��Ŀ 2011-11-22 18:32
		{																					   //
			// ����жϱ�־																	   //
			CLRBITs(UARTx->SR, USART_FLAG_TC);												   //
		}																					   //
		else										// ���в�Ϊ��							   //
		{																					   //
			// �������ɷ�������															   //
			if CHKBITs(UARTx->SR, USART_FLAG_TXE)											   //
			{																				   //
				RxQueue = (T_DataQueue *)RxQueueBuf;										   //
				QUEUE_READ_ISR_QUICK(Data, RxQueue);										   //
				UARTx->DR = Data;															   //
			}																				   //
		}																					   //
		UARTx_EXIT_CRITICAL_TX();					// ���ж�								   //
	}																						   //
																							   //
	#endif 		 */																			   //
}
//#endif	// end: #if (UARTx_DMA_EN == 0) || (UARTx_DMA_RX_EN == 0) //		  			 	//del by ���U��Ŀ 2011-11-08 17:05

#if UARTx_DMA_EN > 0
static void UARTx_DMA_Config(void)
{
	/* USART.DMA.Datasheet.CN:p535 */
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHBPeriphClockCmd(UARTx_RCC_DMA, ENABLE);
	
#if UARTx_TX_EN > 0
	/* UARTx_DMA_Tx_Channel (triggered by UARTx Tx event) Config */
	DMA_DeInit(UARTx_DMA_Tx_Channel);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(UARTx->DR);
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)((T_DataQueue *)TxQueueBuf)->Out;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;	/* ���� */
	DMA_InitStructure.DMA_BufferSize = 0;	// ��ʼ����������Ϊ0
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = 0;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = UARTx_DMA_Tx_Priority;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(UARTx_DMA_Tx_Channel, &DMA_InitStructure);
	
	// ������������ж�
	DMA_ITConfig(UARTx_DMA_Tx_Channel, DMA_IT_TC, ENABLE);
	
	// �ж���������
	//NVIC_PriorityGroupConfig(UARTx_DMA_Tx_NVICPrioGroup);
	NVIC_InitStructure.NVIC_IRQChannel = UARTx_DMA_Tx_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = UARTx_DMA_Tx_IRQPrio;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = UARTx_DMA_Tx_IRQPrioSub;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	// ���ڿ���DMA����, �ɷ��ͺ������ÿ���
	/* Enable USARTy DMA TX request */
	//USART_DMACmd(UARTx, USART_DMAReq_Tx, ENABLE);
	/* Enable USARTy TX DMA1 Channel */
	//DMA_Cmd(UARTx_DMA_Tx_Channel, ENABLE);
#endif
	
#if (UARTx_RX_EN > 0) && (UARTx_DMA_RX_EN > 0)
	/* UARTx_DMA_Rx_Channel (triggered by UARTx Rx event) Config */
	DMA_DeInit(UARTx_DMA_Rx_Channel);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(UARTx->DR);
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32)RxDMABuf;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	/* ���� */
	DMA_InitStructure.DMA_BufferSize = UARTx_Rx_DMABufSize;									   //rework by ���U��Ŀ 2011-11-08 17:05
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;											   //rework by ���U��Ŀ 2011-11-08 17:05
	DMA_InitStructure.DMA_Priority = UARTx_DMA_Rx_Priority;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(UARTx_DMA_Rx_Channel, &DMA_InitStructure);
	
	// ������������ж�, �����봫������ж�
	DMA_ITConfig(UARTx_DMA_Rx_Channel, DMA_IT_TC | DMA_IT_HT, ENABLE);
	
	// �ж���������
	//NVIC_PriorityGroupConfig(UARTx_DMA_Rx_NVICPrioGroup);
	NVIC_InitStructure.NVIC_IRQChannel = UARTx_DMA_Rx_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = UARTx_DMA_Rx_IRQPrio;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = UARTx_DMA_Rx_IRQPrioSub;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* Enable UARTx DMA RX request */
	USART_DMACmd(UARTx, USART_DMAReq_Rx, ENABLE);										   //add by ���U��Ŀ 2011-11-08 17:05
	/* Enable UARTx TX DMA1 Channel */
	DMA_Cmd(UARTx_DMA_Rx_Channel, ENABLE);												   //add by ���U��Ŀ 2011-11-08 17:05
	
	//UARTx_DMA_Rx(RxDMABuf, 1);														   //del by ���U��Ŀ 2011-11-08 17:05
	/* ����DMA����:
	** ��ʼ��֮����DMA�ȴ�һ���ֽ�
	** �����յ�һ���ֽں�, DMA�ж�; Ȼ������DMA����RxDMABuf��һ�������ȴ�����
	** ��������ʱ��ʱ��
	** ��DMA���һ�봫����߶�ʱ����ʱ
	** ��RxDMABuf�е�����д�뵽���ն���QueueRxBuf��
	** ������DMA�ȴ�����һ���ֽ�,��������ռ�����,������һ�ν��չ���
	*/
	
#endif
	
}
#endif 	// end: UARTx_DMA_EN

/*********************************************************************************************************
** ��������: UART1_Config / UARTx_Config
** ��������: UART1��ʼ������UART����, ��ʼ������DMA
** �䡡��: ��
** �䡡��: ��
** ������: ��
** ȫ�ֱ���: ��
** ����ģ��: 
** �桡��: v1.0.00
** ������: Xsky
** �ա���: 2011-03-31 13:45
** ����ע: 
**-------------------------------------------------------------------------------------------------------
** �桡��: 
** �޸���: 
** �ա���: 
** �衡��: 
********************************************************************************************************/
t_err UARTx_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* ��ʼ�����Ͷ��� */
	#if UARTx_TX_EN > 0
	QueueCreate(TxQueueBuf, sizeof(TxQueueBuf));
	#endif
	/* ��ʼ�����ն��� */
	#if UARTx_RX_EN > 0
	QueueCreate(RxQueueBuf, sizeof(RxQueueBuf));
	#endif
	
	/* Enable UARTx pin Clock */
	//RCC_APB2PeriphClockCmd(UARTx_GPIO_RCC, ENABLE);												 //del by ���U��Ŀ 2011-11-10 19:43
	
	/* Enable USARTx Clock */
	#if 	(UARTx_NO == 1)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	#elif 	(UARTx_NO == 2)
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	#elif 	(UARTx_NO == 3)
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	#elif 	(UARTx_NO == 4)
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	#elif 	(UARTx_NO == 5)
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
	#else
	#error	"UARTx.h UARTx_Config() UARTx_NO #define Error!"
	#endif
	
	/* UARTx GPIO_Remap */
	#ifdef UARTx_GPIO_Remap
	GPIO_PinRemapConfig(UARTx_GPIO_Remap, ENABLE);
	#endif
	
	#if (UARTx_NO < 5)
	/* Configure USARTy Rx as input floating */
	GPIO_InitStructure.GPIO_Pin = UARTx_GPIO_Pin_Rx;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;											   //rework by ���U��Ŀ 2011-11-19 15:00
	GPIO_Init(UARTx_GPIO, &GPIO_InitStructure);
	
	/* Configure UARTx Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = UARTx_GPIO_Pin_Tx;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(UARTx_GPIO, &GPIO_InitStructure);
	#elif (UARTx_NO == 5)																			   //add by ���U��Ŀ 2011-11-22 09:40
	/* Configure USARTy Rx as input floating */														   //
	GPIO_InitStructure.GPIO_Pin = UARTx_GPIO_Pin_Rx;												   //
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;												   //
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;											   //
	GPIO_Init(GPIOD, &GPIO_InitStructure);															   //
																									   //
	/* Configure UARTx Tx as alternate function push-pull */										   //
	GPIO_InitStructure.GPIO_Pin = UARTx_GPIO_Pin_Tx;												   //
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;													   //
	GPIO_Init(GPIOC, &GPIO_InitStructure);															   //
	#endif																							   //add by ���U��Ŀ 2011-11-22 09:40
	/* UARTx configuration -------------------------------------------*/
	/* UARTx configured as follow:
		- BaudRate = 115200 baud  
		- Word Length = 8 Bits
		- One Stop Bit
		- Odd parity
		- Hardware flow control disabled (RTS and CTS signals)
		- Receive and transmit enabled
	*/
	
	/* Configure USART1 */
	USART_InitStructure.USART_BaudRate = UARTx_BaudRate;
	USART_InitStructure.USART_WordLength = UARTx_WordLength;
	USART_InitStructure.USART_StopBits = UARTx_StopBits;
	USART_InitStructure.USART_Parity = UARTx_Parity;
	USART_InitStructure.USART_HardwareFlowControl = UARTx_HardwareFlowControl;
	USART_InitStructure.USART_Mode = UARTx_Mode;
	USART_Init(UARTx, &USART_InitStructure);
	//USART_Init(UARTx, (USART_InitTypeDef *)&USART_InitTbl);
	
	// �����ж�������
	#if 	UARTx_NO == 1
	#define UARTx_IRQn		USART1_IRQn
	#elif 	UARTx_NO == 2
	#define UARTx_IRQn		USART2_IRQn
	#elif  	UARTx_NO == 3
	#define UARTx_IRQn		USART3_IRQn
	#elif  	UARTx_NO == 4
	#define UARTx_IRQn		UART4_IRQn
	#elif  	UARTx_NO == 5
	#define UARTx_IRQn		UART5_IRQn
	#else
	#error	"UARTx.h UARTx_Config() UARTx_NO #define Error!"
	#endif
	#if UARTx_DMA_EN > 0
		// ʹ��DMAʱ����DMAͨ��
		
		/* DMA Config */
		UARTx_DMA_Config();
		
		/* ����UART����DMA���� */
		UARTx->CR3 |= USART_DMAReq_Tx;	// Add by Xsky 2011-05-24 16:08
		
		#if UARTx_DMA_RX_EN == 0 && UARTx_RX_EN > 0
		/* Enable the UARTx Interrupt */
		//NVIC_PriorityGroupConfig(UARTx_NVIC_PriorityGroup);
		NVIC_InitStructure.NVIC_IRQChannel = UARTx_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = UARTx_NVIC_Priority;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = UARTx_NVIC_PrioritySub;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		USART_ITConfig(UARTx, USART_IT_RXNE, ENABLE);	// ���ջ������ǿ��ж�
		#endif
		#if UARTx_DMA_RX_EN > 0																	  //add by ���U��Ŀ 2011-11-08 17:05
		//NVIC_PriorityGroupConfig(UARTx_NVIC_PriorityGroup);										  //
		NVIC_InitStructure.NVIC_IRQChannel = UARTx_IRQn;										  //
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = UARTx_NVIC_Priority;				  //
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = UARTx_NVIC_PrioritySub;					  //
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;											  //
		NVIC_Init(&NVIC_InitStructure);															  //
		USART_ITConfig(UARTx, USART_IT_IDLE, ENABLE);											  //
		#endif																					  //add by ���U��Ŀ 2011-11-08 17:05
	#else
		// ��ʹ��DMAʱ��ʹ���жϷ���
		/* Enable the UARTx Interrupt */
		//NVIC_PriorityGroupConfig(UARTx_NVIC_PriorityGroup);
		NVIC_InitStructure.NVIC_IRQChannel = UARTx_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = UARTx_NVIC_Priority;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = UARTx_NVIC_PrioritySub;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		
		#if UARTx_TX_EN > 0
		//USART_ITConfig(UARTx, USART_IT_TXE, ENABLE);		// ��������ж�						 //del by ���U��Ŀ 2011-11-22 18:37
		#endif
		#if UARTx_RX_EN > 0
		USART_ITConfig(UARTx, USART_IT_RXNE, ENABLE);	// ���ջ������ǿ��ж�
		#endif
	#endif
	
	#if UARTx_RX_TIM_EN > 0 && UARTx_RX_EN > 0
	/* ���ճ�ʱ��ʱ�� */
	#if UARTx_RX_TIM_CC_EN == 0
	UARTx_Rx_TIM_Config();
	#endif
	#endif
	
	/* Enable USARTy */
	USART_Cmd(UARTx, ENABLE);
	
	return ERR_NONE;
}


