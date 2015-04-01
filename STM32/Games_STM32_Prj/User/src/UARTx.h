/****************************************Copyright (c)**************************************************
**                                  vixa
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: UARTx.h
**创   建   人: Xsky
**创 建 日  期: 2011-07-28 19:28
**描        述: UART相关公共函数, 为函数实体，用以包含在UARt1...n.c文件中以实现代码重用
** 版  本: v1.0.00
**------------------------------------------------------------------------------------------------------
** 相关文件: 
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

// 局部变量
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
// 正文, UART公共代码
#if UARTx_TX_EN > 0
#if UARTx_DMA_EN > 0
static uint16 DMA_Tx_N = 0;	// 记录DMA传输初始化的时数目
/*********************************************************************************************************
** 函数名称: UARTx_DMA_Tx
** 功能描述: UARTx将发送队列中的数据出队列地址,以及不调头的区域字节数
**				写入发送DMA,并启动DMA传输
**			 注:(1). 使用了队列操作函数,只可在DMA中断及主函数中调用
**				 	 其它中断中调用可能会引发重入问题
**				(2). DMA只配置部分必要参数，其它模式参数等已在初始化时配置
**					
** 输　入: 无
** 输　出: 无
** 返　回: 错误码
** 全局变量: 无
** 调用模块: 
** 版　本: v1.0.00
** 作　者: Xsky
** 日　期: 2011-04-02 15:49
** 备　注: 
**-------------------------------------------------------------------------------------------------------
** 版　本: 
** 修改人: 
** 日　期: 
** 描　述: 
********************************************************************************************************/
static __INLINE void UARTx_DMA_Tx(void)
{
	/* USART.DMA.Datasheet.CN:p535 */
	/* DMA.Datashett.CN: p147 */
	register uint32_t N;
	register uint8 *pOut, *pEnd;
	
	/* Disable the selected DMAy Channelx */
	UARTx_DMA_Tx_Channel->CCR &= (uint16_t)(~DMA_CCR1_EN);
	
	/* 发送缓冲区 */
	pOut = pTxBuf->Out;
	UARTx_DMA_Tx_Channel->CMAR = (uint32_t)(pOut);
	
	/* 计算欲发送的字节数 */
	pEnd = pTxBuf->End;
	N = pTxBuf->NData * sizeof(QUEUE_DATA_TYPE);
	if(N > 64) N = 64;					// 减小最长等待时间, Add by Xsky 2011-07-29 15:52
	if ( (uint)pOut + N > (uint)pEnd )	// 如果队列调头则只发送前部分
	{
		N = pEnd - pOut;
	}
	
	DMA_Tx_N = N;
	UARTx_DMA_Tx_Channel->CNDTR = N;
	
	/* 开启发送DMA通道 */
	UARTx_DMA_Tx_Channel->CCR |= DMA_CCR1_EN;
	
	/* 开启UART发送DMA请求 */
	// UARTx->CR3 |= USART_DMAReq_Tx;	// 初始化时配置该位即可, Del by Xsky 2011-05-24 16:08
}

/*********************************************************************************************************
** 函数名称: UARTx_DMA_TX_IRQHandler/DMAx_ChannelN_IRQHandler
** 功能描述: TX发送DMA中断处理函数
**				若有数据则重新配置DMA,并启动DMA传输
**			 注:(1). 
** 输　入: 无
** 输　出: 无
** 返　回: 错误码
** 全局变量: 无
** 调用模块: 
** 版　本: v1.0.00
** 作　者: Xsky
** 日　期: 2011-04-02 15:49
** 备　注: 
**-------------------------------------------------------------------------------------------------------
** 版　本: 
** 修改人: 
** 日　期: 
** 描　述: 
********************************************************************************************************/
void UARTx_DMA_Tx_IRQHandler(void)
{
	register t_ureg ISR;
	register T_DataQueue *Queue;
	
	ISR = UARTx_DMA->ISR;
	if(ISR & UARTx_DMA_Tx_IF_TC)	// DMA传输完成中断标志
	{
	    // 清除中断标志
	    UARTx_DMA->IFCR = UARTx_DMA_Tx_IF_TC;
		
		UARTx_ENTER_CRITICAL_TX();	// 如果在其它中断程序中也调用串口发送函数，则必须关中断
		// 队列中减去DMA传输完成的数量
		Queue = pTxBuf;
	    QueueOutN(Queue, DMA_Tx_N);
	    DMA_Tx_N = 0;
		if(Queue->NData > 0)	// QueueNData(TxQueueBuf)
		{
			// 还有未传输数据, 重新初始化传输
			UARTx_DMA_Tx();
		}
		else
		{
			// 传输完成重置队列头尾指针, 以减少调头次数
			//QueueFlush(TxQueueBuf);
			Queue->Out = Queue->Buf;
			Queue->In = Queue->Buf;
			Queue->NData = 0;											/* 数据数目为0 */
			// 传输完成后DMA不理会请求, Datasheet.CN:p536
			// 可不关闭DMA
		}
		UARTx_EXIT_CRITICAL_TX();
	}
	// 清除中断标志
	//UARTx_DMA->IFCR = UARTx_DMA_Tx_IF_GL; // 保留传输错误状态标志以在Send函数中检查
	
	#if defined(DMA2_Channel4_IRQHook) && (UARTx_NO >= 4)
		#if (UARTx_DMA_Tx_Number == 0x0205) || (UARTx_DMA_Tx_Number == 0x0204)
		//ISR = DMA2->ISR;
		if(ISR & DMA2_FLAG_TC4)	// DMA传输完成中断标志
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
** 函数名称: UART1_SendBytes
** 功能描述: UART1发送函数, 将指定的字节写入发送缓冲区, 如果缓冲区无数据则直接发送, 缓冲区有数据则
**			将数据压入队列等待发送
**			 注: 使用了队列操作函数,只可在DMA中断及主函数中调用
**				 其它中断中调用可能会引发重入问题,需定义_CRITICAL()函数为相关中断开关函数
** 输　入:  1. *pSrc: 发送缓冲区
**			2. nBytes: 待发送数据字节数 
** 输　出: 无
** 返　回: 错误码:  ERR_NONE
**			  	ERR_UsartTxQueueFull
** 全局变量: 无
** 调用模块: 
** 版　本: v1.0.00
** 作　者: Xsky
** 日　期: 2011-04-02 14:27
** 备　注: 
**-------------------------------------------------------------------------------------------------------
** 版　本: 
** 修改人: 
** 日　期: 
** 描　述: 
********************************************************************************************************/
t_err UARTx_SendBytes(void *pSrc, t_size nBytes)
{
	register t_err err;	
	//register T_DataQueue *Queue;
	
#if UARTx_DMA_EN > 0		// 启用DMA
	// 直接写入数据至发送队列
	if(nBytes == 0)											// Add by Xsky 2011-07-29 17:44
	{
		return ERR_NONE;
	}
	UARTx_ENTER_CRITICAL_TX();
	err = QueueWriteBytes(TxQueueBuf, pSrc, nBytes);
	if(err == QUEUE_OK)
	{
		if(DMA_Tx_N == 0)									// 如果上次传输已完成 
		//if CHKNBITs(UARTx_DMA->ISR, UARTx_DMA_Tx_IF_TC)	// 
		{
			UARTx_DMA_Tx();
		}
		
		err = ERR_NONE;
	}
	else	// 队列已满
	{
		if CHKBITs(UARTx_DMA->ISR, UARTx_DMA_Tx_IF_TE)
		{
			// 如果传输错误,清除错误标志
			UARTx_DMA->IFCR = UARTx_DMA_Tx_IF_TE;
			// 重新开启传输
			if((uint16)UARTx_DMA_Tx_Channel->CNDTR == 0)
			{
				UARTx_DMA_Tx();
			}
		}
		err = ERR_UsartTxQueueFull;
	}
	UARTx_EXIT_CRITICAL_TX();
#else					// 未启用DMA
	register uint8_t Data;																	//rework by 翻訳の目 2011-11-23 08:53
	if( pTxBuf->NData + nBytes > pTxBuf->MaxData)											//rework by 翻訳の目 2011-11-14 10:32
	{
		// 如果缓冲区满则直接退出
		return ERR_UsartTxQueueFull;
	}
	
	UARTx_ENTER_CRITICAL_TX();					// 关中断									//rework by 翻訳の目 2011-11-23 08:53
	err = QueueWriteBytes(TxQueueBuf, pSrc, nBytes);										//
	UARTx_EXIT_CRITICAL_TX();					// 开中断									//
	// 检测如果可发送数据																	//
	while(pTxBuf->NData > 0 && CHKBITs(UARTx->SR, USART_FLAG_TXE))							//
	{																						//
		QUEUE_READ_ISR_QUICK(Data, pTxBuf);													//
		UARTx->DR = Data;																	//
	}																						//rework by 翻訳の目 2011-11-23 08:53
	
	
	/*if(err == QUEUE_OK)
	{
		err = ERR_NONE;
	}
	else	// 队列已满
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
//static uint16 DMA_Rx_N = 0;		// 记录配置DMA传输初始化的时数目                  	 			 //del by 翻訳の目 2011-11-08 17:05
//static uint16 DMA_Rx_NAll = 0;	// 记录未写到接收队列的DMA总传输数目							 //del by 翻訳の目 2011-11-08 17:05
/*********************************************************************************************************
** 函数名称: UARTx_DMA_Rx
** 功能描述: UARTx将接收队列中的数据入队列地址,以及不调头的空闲字节数
**				写入接收DMA,并启动DMA传输,读取接收到的数据
**			 注:(1). 使用了队列操作函数,只可在DMA中断及主函数中调用
**				 其它中断中调用可能会引发重入问题,需定义_CRITICAL()函数为相关中断开关函数
**				(2). DMA只配置部分必要参数，其它模式参数等已在初始化时配置
**					
** 输　入: 无
** 输　出: 无
** 返　回: 错误码
** 全局变量: 无
** 调用模块: 
** 版　本: v1.0.00
** 作　者: Xsky
** 日　期: 2011-04-02 15:49
** 备　注: 
**-------------------------------------------------------------------------------------------------------
** 版　本: 
** 修改人: 
** 日　期: 
** 描　述: 
********************************************************************************************************/
static void UARTx_DMA_Rx(uint8 *pDst, uint16 nBytes)
{
	/* USART.DMA.Datasheet.CN:p535 */
	/* DMA.Datashett.CN: p147 */
	//register uint32_t N;																			  //del by 翻訳の目 2011-11-08 17:05
	//register uint8 *pOut, *pEnd;
	
	/* Disable the selected DMAy Channelx */
	UARTx_DMA_Rx_Channel->CCR &= (uint16_t)(~DMA_CCR1_EN);
	
	/* 发送缓冲区 */
	UARTx_DMA_Rx_Channel->CMAR = (uint32_t)pDst;
	
	/* 发送的字节数 */
	//DMA_Rx_N = nBytes;																			   //del by 翻訳の目 2011-11-08 17:05
	UARTx_DMA_Rx_Channel->CNDTR = nBytes;
	
	/* 开启接收DMA通道 */
	UARTx_DMA_Rx_Channel->CCR |= (DMA_CCR1_EN | DMA_CCR1_TCIE | DMA_CCR1_HTIE);						   //rework by 翻訳の目 2011-11-08 17:05
	
	/* 开启UART接收DMA请求 */
	UARTx->CR3 |= USART_DMAReq_Rx;
}

/*********************************************************************************************************
** 函数名称: UARTx_DMA_RX_IRQHandler/DMAx_ChannelN_IRQHandler
** 功能描述: Rx接收DMA中断处理函数
**			 注:(1). 
** 输　入: 无
** 输　出: 无
** 返　回: 错误码
** 全局变量: 无
** 调用模块: 
** 版　本: v1.0.00
** 作　者: Xsky
** 日　期: 2011-04-03 08:40
** 备　注: 
**-------------------------------------------------------------------------------------------------------
** 版　本: 
** 修改人: 
** 日　期: 
** 描　述: 
********************************************************************************************************/
void  UARTx_DMA_Rx_IRQHandler (void)	// UARTx_DMA_RX_IRQHandler
{
	//register t_ureg ISR;
	register t_ureg u32r;
	
	u32r = UARTx_DMA->ISR;
	UARTx_ENTER_CRITICAL_RX();																		//rework by 翻訳の目 2011-11-08 17:05
		if (u32r&UARTx_DMA_Rx_IF_TC )	// DMA传输完成中断标志										//	使用CHKBITs(x,msk)的情况下Uart4收到数据
	{	    																						//	发生DMA中断而不能跳到转到下面的那句
			QueueWriteBytes(RxQueueBuf, &RxDMABuf[UARTx_Rx_DMABufSize/2], UARTx_Rx_DMABufSize/2);	//
	}																								//
		if (u32r&UARTx_DMA_Rx_IF_HT)	// DMA传输完成中断标志										//	使用CHKBITs(x,msk)的情况下Uart4收到数据
	{	    																						//	发生DMA中断而不能跳到转到下面的那句
			QueueWriteBytes(RxQueueBuf, RxDMABuf, UARTx_Rx_DMABufSize/2);							//
	}																								//
	UARTx_EXIT_CRITICAL_RX();																		//
	// 清除中断标志																					//
	UARTx_DMA->IFCR = UARTx_DMA_Rx_IF_TC | UARTx_DMA_Rx_IF_HT;										//rework by 翻訳の目 2011-11-08 17:05
}

#endif 	// end: #if UARTx_DMA_EN > 0  && UARTx_DMA_RX_EN > 0 // 

#if (UARTx_RX_TIM_EN > 0)

#if UARTx_RX_TIM_CC_EN > 0
#else

// 停止定时器
#define  UARTx_Rx_TIM_Stop() 	CLRBITs(UARTx_Rx_TIM->CR1, TIM_CR1_CEN)
/*void UARTx_Rx_TIM_Stop(void)
{
	TIM_Cmd(UARTx_Rx_TIM, DISABLE); 
}*/

// 开启定时器
#define  UARTx_Rx_TIM_Start()	UARTx_Rx_TIM->CNT = 0x0000; \
								SETBITs(UARTx_Rx_TIM->CR1, TIM_CR1_CEN); \
								RxTimeOutFlag = c_InTime;
/*void UARTx_Rx_TIM_Start(void)
{
	TIM_SetCounter(UARTx_Rx_TIM,0x0000);
	TIM_Cmd(UARTx_Rx_TIM, ENABLE);
}*/

/*******************************************************************************
* 文件名	  	 : UARTx_Rx_TIM_Config
* 描述	         : 接收字符超时定时器初始化配置
* 					其机制为:接收中断产生时重置定时器计数并开启定时器，当定时器超时
*					说明在此定时时间内已无字符，则停止定时器，可直接读取数据
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
static void UARTx_Rx_TIM_Config(void)
{
	/* Datasheet.CN
	** 高级控制定时器: p199, TIM1/TIM8
	** 基本定时器: p298, TIM6/TIM7
	** 通用定时器: p253, TIM2/3/4/5
	*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	//TIM_OCInitTypeDef TIM_OCInitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	TIM_DeInit(UARTx_Rx_TIM);
	
	// 中断向量及优先级设置
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
	TIM_TimeBaseInitStructure.TIM_Prescaler = CPU_HCLK/1000000 - 1;	// 预分频为1us
	TIM_TimeBaseInitStructure.TIM_Period = UARTx_RX_TIM_OutCharN * 11 * 1000000 
											/ UARTx_BaudRate;		// ARR
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(UARTx_Rx_TIM,&TIM_TimeBaseInitStructure);
	TIM_SelectOnePulseMode(UARTx_Rx_TIM, TIM_OPMode_Single);		// 选择为单脉冲模式
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
* 函数名	  	 : UARTx_Rx_TIM_IRQHandler
* 描述	         : 接收超时定时器中断, 停止定时器并设置字符超时标志
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void UARTx_Rx_TIM_IRQHandler(void)	// UARTx_Rx_TIM_IRQHandler
{
	if CHKBITs(UARTx_Rx_TIM->SR, TIM_FLAG_Update)		/* 检查更新中断标志位 */
	if CHKBITs(UARTx_Rx_TIM->DIER, TIM_FLAG_Update)		/* 检查中断使用位以排除软件更中断 */
	{
		// 清除中断标志
		//TIM_ClearFlag(UARTx_Rx_TIM,TIM_FLAG_Update);
		CLRBITs(UARTx_Rx_TIM->SR, TIM_FLAG_Update);
		
		// 停止定时器
		UARTx_Rx_TIM_Stop();
		// 设置超时标志
		RxTimeOutFlag = c_TimeOut;
	}
}
#endif	// end: #if UARTx_RX_TIM_EN > 0 //

/*******************************************************************************
* 函数名	  	 : UART1_isCharRcvTimeOut
* 描述	         : 获取字符超时状态,最后一个接收到的字符之后
* 输入           : 无
* 输出           : 无
* 返回           : 1:字符超时, 0: 未超时
t_bool UART1_isCharRcvTimeOut(void)
{
	// return RxTimeOutFlag = c_TimeOut;
	return RxTimeOutFlag;	// 注意此除代码经简化
}
*******************************************************************************/


#endif	// end: #if UARTx_RX_TIM_CC_EN > 0 //

/*******************************************************************************
* 函数名	  	 : UART1_RcvdSize / UARTx_RcvdSize
* 描述	         : UART1接收到的字节数
*				  注: 如果字符未时则返回0, 字符超时之后才返回实际接收到的长度
* 输入           : 无
* 输出           : 无
* 返回           : 无
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
** 函数名称: UART1_ReadBytes
** 功能描述: 读取接收缓冲区内的数据, 
**			注: (1). 使用了队列操作函数,如果有重入问题需考虑定义关闭中断
** 输　入:  1. *Dst: 数据放目标缓冲区
**			2. nBytes: 待接收数据字节数 
** 输　出: 无
** 返　回: 实际读取的字节数 
** 全局变量: 无
** 调用模块: QueueReadBytes()
** 版　本: v1.0.00
** 作　者: Xsky
** 日　期: 2011-04-03 14:27
** 备　注: 
**-------------------------------------------------------------------------------------------------------
** 版　本: 
** 修改人: 
** 日　期: 
** 描　述: 
********************************************************************************************************/
t_size_opt UARTx_ReadBytes(void *pDst, t_size nBytes)
{
	return QueueReadBytes(pDst, nBytes, RxQueueBuf);
}

#endif	// end: #if UARTx_RX_EN > 0 //

//#if (UARTx_DMA_EN == 0) || (UARTx_DMA_RX_EN == 0)														//del by 翻訳の目 2011-11-08 17:05
/*******************************************************************************
* 函数名	  	 : USARTx_IRQHandler
* 描述	         : UARTx中断处理函数, 当使用中断接收或者中断发送时处理收/发队列
* 输入           : 无
* 输出           : 无
* 返回           : 无
*******************************************************************************/
void UARTx_IRQHandler(void)	// UARTx_IRQHandler
{
	#if UARTx_DMA_RX_EN == 0 || UARTx_DMA_EN == 0													   //add by 翻訳の目 2011-11-08 17:05
	register T_DataQueue *RxQueue;
	#endif																							   //add by 翻訳の目 2011-11-08 17:05
	//#if UARTx_DMA_EN == 0 && UARTx_TX_EN > 0
	//register uint16 Data;
	//#endif
	#if UARTx_DMA_RX_EN > 0	&& UARTx_DMA_EN > 0														   //add by 翻訳の目 2011-11-08 17:05
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
	#endif																							   //add by 翻訳の目 2011-11-08 17:05
	#if UARTx_DMA_RX_EN == 0 && UARTx_RX_EN > 0
	// 中断接收数据
	if CHKBITs(UARTx->SR, USART_FLAG_RXNE)			// 接收寄非空
	{
		// 将数据写入缓冲区
		RxQueue = (T_DataQueue *)RxQueueBuf;
		UARTx_ENTER_CRITICAL_RX();
		QueueWriteQuickIsr(RxQueue, UARTx->DR);
		UARTx_EXIT_CRITICAL_RX();
		
		#if UARTx_RX_TIM_EN > 0
		if(RxQueue->NData > RxQueue->MaxData * 2 / 3)			// 防止数据不间隔接收时, 不会产生超时中断, if(...), Add by Xsky 2011-07-29 15:35
		{
			// 设置超时标志
			RxTimeOutFlag = c_TimeOut;
		}
		else
		{
			// 启动超时定时器
			UARTx_Rx_TIM_Start();
		}
		#endif
	}
	#endif
	
/*	#if UARTx_DMA_EN == 0 && UARTx_TX_EN > 0												   //
	// 发送完成中断, 不使用DMA传输时使用													   //
	if CHKBITs(UARTx->SR, USART_FLAG_TC | USART_FLAG_TXE)// 发送完成中断/发送寄存器空中断	   //
	{																						   //
		UARTx_ENTER_CRITICAL_TX();					// 关中断								   //
		if(pTxBuf->NData == 0)	// 如果队列为空												   //del by 翻訳の目 2011-11-22 18:32
		{																					   //
			// 清除中断标志																	   //
			CLRBITs(UARTx->SR, USART_FLAG_TC);												   //
		}																					   //
		else										// 队列不为空							   //
		{																					   //
			// 检测如果可发送数据															   //
			if CHKBITs(UARTx->SR, USART_FLAG_TXE)											   //
			{																				   //
				RxQueue = (T_DataQueue *)RxQueueBuf;										   //
				QUEUE_READ_ISR_QUICK(Data, RxQueue);										   //
				UARTx->DR = Data;															   //
			}																				   //
		}																					   //
		UARTx_EXIT_CRITICAL_TX();					// 开中断								   //
	}																						   //
																							   //
	#endif 		 */																			   //
}
//#endif	// end: #if (UARTx_DMA_EN == 0) || (UARTx_DMA_RX_EN == 0) //		  			 	//del by 翻訳の目 2011-11-08 17:05

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
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;	/* 发送 */
	DMA_InitStructure.DMA_BufferSize = 0;	// 初始化必须配置为0
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = 0;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = UARTx_DMA_Tx_Priority;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(UARTx_DMA_Tx_Channel, &DMA_InitStructure);
	
	// 开启传输完成中断
	DMA_ITConfig(UARTx_DMA_Tx_Channel, DMA_IT_TC, ENABLE);
	
	// 中断向量配置
	//NVIC_PriorityGroupConfig(UARTx_DMA_Tx_NVICPrioGroup);
	NVIC_InitStructure.NVIC_IRQChannel = UARTx_DMA_Tx_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = UARTx_DMA_Tx_IRQPrio;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = UARTx_DMA_Tx_IRQPrioSub;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	// 现在开启DMA传输, 由发送函数调用开启
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
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	/* 接收 */
	DMA_InitStructure.DMA_BufferSize = UARTx_Rx_DMABufSize;									   //rework by 翻訳の目 2011-11-08 17:05
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;											   //rework by 翻訳の目 2011-11-08 17:05
	DMA_InitStructure.DMA_Priority = UARTx_DMA_Rx_Priority;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(UARTx_DMA_Rx_Channel, &DMA_InitStructure);
	
	// 开启传输完成中断, 开启半传输完成中断
	DMA_ITConfig(UARTx_DMA_Rx_Channel, DMA_IT_TC | DMA_IT_HT, ENABLE);
	
	// 中断向量配置
	//NVIC_PriorityGroupConfig(UARTx_DMA_Rx_NVICPrioGroup);
	NVIC_InitStructure.NVIC_IRQChannel = UARTx_DMA_Rx_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = UARTx_DMA_Rx_IRQPrio;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = UARTx_DMA_Rx_IRQPrioSub;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* Enable UARTx DMA RX request */
	USART_DMACmd(UARTx, USART_DMAReq_Rx, ENABLE);										   //add by 翻訳の目 2011-11-08 17:05
	/* Enable UARTx TX DMA1 Channel */
	DMA_Cmd(UARTx_DMA_Rx_Channel, ENABLE);												   //add by 翻訳の目 2011-11-08 17:05
	
	//UARTx_DMA_Rx(RxDMABuf, 1);														   //del by 翻訳の目 2011-11-08 17:05
	/* 接收DMA机制:
	** 初始化之后开启DMA等待一个字节
	** 当接收到一个字节后, DMA中断; 然后配置DMA按照RxDMABuf的一半容量等待接收
	** 并开启超时定时器
	** 若DMA完成一半传输或者定时器超时
	** 则将RxDMABuf中的数据写入到接收队列QueueRxBuf中
	** 并设置DMA等待接收一个字节,且清零接收计数器,进入下一次接收过程
	*/
	
#endif
	
}
#endif 	// end: UARTx_DMA_EN

/*********************************************************************************************************
** 函数名称: UART1_Config / UARTx_Config
** 功能描述: UART1初始化配置UART特性, 初始化配置DMA
** 输　入: 无
** 输　出: 无
** 返　回: 无
** 全局变量: 无
** 调用模块: 
** 版　本: v1.0.00
** 作　者: Xsky
** 日　期: 2011-03-31 13:45
** 备　注: 
**-------------------------------------------------------------------------------------------------------
** 版　本: 
** 修改人: 
** 日　期: 
** 描　述: 
********************************************************************************************************/
t_err UARTx_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* 初始化发送队列 */
	#if UARTx_TX_EN > 0
	QueueCreate(TxQueueBuf, sizeof(TxQueueBuf));
	#endif
	/* 初始化接收队列 */
	#if UARTx_RX_EN > 0
	QueueCreate(RxQueueBuf, sizeof(RxQueueBuf));
	#endif
	
	/* Enable UARTx pin Clock */
	//RCC_APB2PeriphClockCmd(UARTx_GPIO_RCC, ENABLE);												 //del by 翻訳の目 2011-11-10 19:43
	
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
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;											   //rework by 翻訳の目 2011-11-19 15:00
	GPIO_Init(UARTx_GPIO, &GPIO_InitStructure);
	
	/* Configure UARTx Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = UARTx_GPIO_Pin_Tx;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(UARTx_GPIO, &GPIO_InitStructure);
	#elif (UARTx_NO == 5)																			   //add by 翻訳の目 2011-11-22 09:40
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
	#endif																							   //add by 翻訳の目 2011-11-22 09:40
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
	
	// 定义中断向量号
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
		// 使用DMA时配置DMA通道
		
		/* DMA Config */
		UARTx_DMA_Config();
		
		/* 开启UART发送DMA请求 */
		UARTx->CR3 |= USART_DMAReq_Tx;	// Add by Xsky 2011-05-24 16:08
		
		#if UARTx_DMA_RX_EN == 0 && UARTx_RX_EN > 0
		/* Enable the UARTx Interrupt */
		//NVIC_PriorityGroupConfig(UARTx_NVIC_PriorityGroup);
		NVIC_InitStructure.NVIC_IRQChannel = UARTx_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = UARTx_NVIC_Priority;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = UARTx_NVIC_PrioritySub;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		USART_ITConfig(UARTx, USART_IT_RXNE, ENABLE);	// 接收缓冲器非空中断
		#endif
		#if UARTx_DMA_RX_EN > 0																	  //add by 翻訳の目 2011-11-08 17:05
		//NVIC_PriorityGroupConfig(UARTx_NVIC_PriorityGroup);										  //
		NVIC_InitStructure.NVIC_IRQChannel = UARTx_IRQn;										  //
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = UARTx_NVIC_Priority;				  //
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = UARTx_NVIC_PrioritySub;					  //
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;											  //
		NVIC_Init(&NVIC_InitStructure);															  //
		USART_ITConfig(UARTx, USART_IT_IDLE, ENABLE);											  //
		#endif																					  //add by 翻訳の目 2011-11-08 17:05
	#else
		// 不使用DMA时，使用中断发送
		/* Enable the UARTx Interrupt */
		//NVIC_PriorityGroupConfig(UARTx_NVIC_PriorityGroup);
		NVIC_InitStructure.NVIC_IRQChannel = UARTx_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = UARTx_NVIC_Priority;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = UARTx_NVIC_PrioritySub;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		
		#if UARTx_TX_EN > 0
		//USART_ITConfig(UARTx, USART_IT_TXE, ENABLE);		// 发送完成中断						 //del by 翻訳の目 2011-11-22 18:37
		#endif
		#if UARTx_RX_EN > 0
		USART_ITConfig(UARTx, USART_IT_RXNE, ENABLE);	// 接收缓冲器非空中断
		#endif
	#endif
	
	#if UARTx_RX_TIM_EN > 0 && UARTx_RX_EN > 0
	/* 接收超时定时器 */
	#if UARTx_RX_TIM_CC_EN == 0
	UARTx_Rx_TIM_Config();
	#endif
	#endif
	
	/* Enable USARTy */
	USART_Cmd(UARTx, ENABLE);
	
	return ERR_NONE;
}


