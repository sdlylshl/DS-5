/****************************************Copyright (c)**************************************************
**                              2009-2009, YingHong, Ltd.
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: CAN.h
**创   建   人: Xsky
**创 建 日  期: 2011-02-25 14:45
**描        述: CAN相关类型及常数定义，驱动函数，中断服务函数
**			  按完整CAN邮箱格式发送/接收数据, FIFO队列操作的基本单元为T_CanFrame
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
// 常数定义
#if CAN1_SWI_HANDLE_EN > 0								/* Add by Xsky 2011-06-18 14:55 */
#ifndef CAN1_SWI_IRQHandler_EXT_EN
#error	"直接在外部实际代码定义SWI Handler如EXTI4_IRQHandler, 以跳过函数调用直接使用驱动代码！"
#define  CAN1_SWI_IRQHandler_EXT_EN 	0
#endif
#endif	/* end: #if CAN1_SWI_HANDLE_EN > 0 */

/***************************************************************************************************/
// 变量定义
// 队列参数
/* 发送中断，应用层发送函数直接将数据设置为欲发送的CAN发送邮箱格式
** T_CanFrame/CAN_TypeDef打包
** 发送时如果邮箱有空则直接将数据压入邮箱，否则将数据压入队列
*/
T_CanQueue CanTxQueue;
T_CanQueue CanRxQueue;
// CAN发送缓冲区
static T_CanFrame CanTxBuf[CAN_QUE_TX_Nframe] = {0,};
// CAN接收缓冲区
static T_CanFrame CanRxBuf[CAN_QUE_RX_Nframe] = {0, };

#ifdef DBG_CAN
// 调试变量
//static CAN_TypeDef *pCAN1;
#endif

/***************************************************************************************************/
// 常数定义
static const uint8 TxMailBox_IdleNO_Tbl[] = {
				0xFF/*0x0:Busy*/,	0/*0x1:0*/, 	1/*010b:1*/, 
				0/*011b:0,1*/,		2/*100b:2*/,	0/*101b:0,2*/, 
				1/*110b:1,2*/,		0/*111b:0,1,2*/};

/*********************************************************************************************************
** 函数名称: CAN_QueueCreate
** 功能描述: 初始化数据队列
** 输　入: Buf     ： 为队列分配的存储空间地址
**         pDataBuf： 数据存放缓冲区
**		   nData:	  数据元素个数,以*pDataBuf为单元的个数
**         ReadEmpty：为队列读空时处理程序
**         WriteFull：为队列写满时处理程序
**         WriteFullParam: 队列写满函数调用时的参数
** 输　出: NOT_OK:参数错误
**         QUEUE_OK:成功
** 全局变量: 无
** 调用模块: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
**
** 作　者: Xsky
** 日　期: 2011-03-10 10:43
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
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
    	
	if (Queue != NULL && pDataBuf != NULL)						/* 判断参数是否有效 */
	{
		//Queue = (T_DataQueue *)Buf;
		
		CAN_QUE_OS_ENTER_CRITICAL();
		                                                	/* 初始化结构体数据 */
		Queue->Buf = pDataBuf;
		Queue->MaxData = nData;				               	/* 计算队列可以存储的数据数目 */
		Queue->End = &Queue->Buf[Queue->MaxData];           /* 计算数据缓冲的结束地址,已超出缓冲区 */
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
** 函数名称: CAN_QueueWrite
** 功能描述: 写指定的数据量至队列
** 输　入: Queue:指向队列的指针
**         pData:消息数据
** 返　回: NOT_OK     ：参数错误
**         QUEUE_OK   ：收到消息
**         QUEUE_EMPTY：无消息
** 全局变量: 无
** 调用模块: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
** 作　者: Xsky
** 日　期: 2011-03-10 10:43
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
** 描　述: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
__inline static t_err CAN_QueueWrite(register T_CanQueue *Queue, T_CanFrame *pData)
{
	register t_err err;
    //T_CanFrame *Queue;
	register T_CanFrame *pIn;

    err = NOT_OK;
    if (Queue != NULL)                             /* 队列是否有效 */
    {
        //Queue = (T_DataQueue *)Buf;
        
        //CAN_QUE_OS_ENTER_CRITICAL();
		CAN_INT_TX_DISABLE();						/* 关闭发送中断 */
        
        if (Queue->NData < Queue->MaxData)         	/* 队列是否满  */
        {                                  			/* 不满        */
        	pIn = Queue->In;
            *pIn = *pData;                 			/* 数据入队    */
            ++pIn;
            if (pIn >= Queue->End)
            {
                pIn = Queue->Buf;
            }
            Queue->In = pIn;                    	/* 调整入队指针*/
            Queue->NData++;                         /* 数据增加    */
            err = QUEUE_OK;
        }
        else
        {                                           /* 满           */
            err = QUEUE_FULL;
			#if (CAN_QUEUE_WRITE_FULL_EN > 0)
            if (Queue->WriteFull != NULL)        	/* 调用用户处理函数 */
            {
                err = Queue->WriteFull(Queue->WriteFullParam);
            }
			#endif
		}
		CAN_INT_TX_ENABLE();						/* 开启发送中断 */
    }
    return err;
}

// 不关中断
__inline static t_err CAN_QueueWriteQuick(register T_CanQueue *Queue, T_CanFrame *pData)
{
	register t_err err;
    //T_CanFrame *Queue;
	register T_CanFrame *pIn;
	
    err = NOT_OK;
    if (Queue != NULL)                             /* 队列是否有效 */
    {
        //Queue = (T_DataQueue *)Buf;
        
        //CAN_QUE_OS_ENTER_CRITICAL();
        
        if (Queue->NData < Queue->MaxData)         	/* 队列是否满  */
        {                                  			/* 不满        */
        	pIn = Queue->In;
            *pIn = *pData;                 			/* 数据入队    */
            ++pIn;
            if (pIn >= Queue->End)
            {
                pIn = Queue->Buf;
            }
            Queue->In = pIn;                    	/* 调整入队指针*/
            Queue->NData++;                         /* 数据增加    */
            err = QUEUE_OK;
        }
        else
        {                                           /* 满           */
            err = QUEUE_FULL;
			#if (CAN_QUEUE_WRITE_FULL_EN > 0)
            if (Queue->WriteFull != NULL)        	/* 调用用户处理函数 */
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
** 函数名称: CAN_QueueRead
** 功能描述: 获取队列中的一条数据单元
** 输　入: Queue:指向队列的指针
**         pData:数据存放地
** 返　回: NOT_OK     ：参数错误
**         QUEUE_OK   ：收到消息
**         QUEUE_EMPTY：无消息
** 全局变量: 无
** 调用模块: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
** 作　者: Xsky
** 日　期: 2011-03-11 10:00
**-------------------------------------------------------------------------------------------------------
** 修改人: Xsky
** 日　期: 
** 描　述: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
__inline t_err CAN_QueueRead(register T_CanQueue *Queue, T_CanFrame *pData)
{
	register t_err err;
	//register    T_DataQueue *Queue;
	register uint16 uData;
	register T_CanFrame *pOut, *pOutOld;
	
	err = NOT_OK;
	
	if (Queue != NULL)                              /* 队列是否有效 */
	{                                               /* 有效 */
		//Queue = (T_DataQueue *)Buf;
		
		//CAN_QUE_OS_ENTER_CRITICAL();
		CAN_INT_RX_DISABLE();						/* 关闭所有接收中断 */
		
		if (Queue->NData > 0)                       /* 队列是否为空 */
		{                                    		/* 不空      */
			//*Ret = Queue->Out[0];                 /* 数据出队  */
			pOut = Queue->Out;
			pOutOld = pOut;
			//*pData = *(pOut);
			++pOut;
			if (pOut >= Queue->End)
			{
				pOut = Queue->Buf;
			}
			Queue->Out = pOut;                  	/* 调整出队指针 */
			Queue->NData--;                         /* 数据减少      */
			
			CAN_INT_RX_ENABLE();					/* 开启所有接收中断 */
			*pData = *pOutOld;						/* 开启中断再复制数据以减少中断时间 */
			err = QUEUE_OK;
		}
		else
		{                                           /* 空              */
			CAN_INT_RX_ENABLE();					/* 开启所有接收中断 */
			
			err = QUEUE_EMPTY;
			#if (CAN_QUEUE_READ_EMPTY_EN > 0)
			if (Queue->ReadEmpty != NULL)           /* 调用用户处理函数 */
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

// 不中断
__inline static t_err CAN_QueueReadQuick(T_CanFrame *pData, register T_CanQueue *Queue)
{
	register t_err err;
	//register    T_DataQueue *Queue;
	register T_CanFrame *pOut;
	
	err = NOT_OK;
	
	if (Queue != NULL)                           	/* 队列是否有效 */
	{                                               /* 有效 */
		//Queue = (T_DataQueue *)Buf;
		
		//CAN_QUE_OS_ENTER_CRITICAL();
		
		if (Queue->NData > 0)                       /* 队列是否为空 */
		{                                           /* 不空         */
			//*Ret = Queue->Out[0];                 /* 数据出队     */
			pOut = Queue->Out;
			*pData = *pOut;
			++pOut;
			if (pOut >= Queue->End)
			{
				pOut = Queue->Buf;
			}
			Queue->Out = pOut;                  	/* 调整出队指针 */
			Queue->NData--;                         /* 数据减少      */
			err = QUEUE_OK;
		}
		else
		{                                           /* 空              */
			err = QUEUE_EMPTY;
			#if (CAN_QUEUE_READ_EMPTY_EN > 0)
			if (Queue->ReadEmpty != NULL)           /* 调用用户处理函数 */
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
** 函数名称: CAN_ReadFrame
** 功能描述: 获取接收数据队列中的一条数据单元
** 输　入: 
**         pData:数据存放地
** 返　回: NOT_OK     ：参数错误
**         QUEUE_OK   ：收到消息
**         QUEUE_EMPTY：无数据
** 全局变量: 无
** 调用模块: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
** 作　者: Xsky
** 日　期: 2011-03-11 10:00
**-------------------------------------------------------------------------------------------------------
** 修改人: Xsky
** 日　期: 
** 描　述: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
t_err CAN_ReadFrame(T_CanFrame *pData)
{
	register t_err err;
	//register    T_DataQueue *Queue;
	register T_CanQueue *Queue = &CanRxQueue;
	register T_CanFrame *pOut, *pOutOld;
	
	err = NOT_OK;
	
	if (Queue != NULL)                              /* 队列是否有效 */
	{                                               /* 有效 */
		//Queue = (T_DataQueue *)Buf;
		#ifdef DBG_CAN_QUE_MAX
			// 调试,记录接收队列最大缓冲数
			if(CanRxQueue.NData < g_History.SysErrors.CAN_RcvQueMaxN){
				g_History.SysErrors.CAN_RcvQueMaxN = CanRxQueue.NData;
			}
		#endif
		
		CAN_INT_RX_DISABLE();						/* 关闭所有接收中断 */
		
		if (Queue->NData > 0)                       /* 队列是否为空 */
		{                                    		/* 不空         */
			//*Ret = Queue->Out[0];                 /* 数据出队     */
			pOutOld = Queue->Out;
			//*pData = *(pOut);
			pOut = pOutOld + 1;
			if (pOut >= Queue->End)
			{
				pOut = Queue->Buf;
			}
			Queue->Out = pOut;                  	/* 调整出队指针 */
			Queue->NData--;                         /* 数据减少      */
			
			CAN_INT_RX_ENABLE();					/* 开启所有接收中断 */
			*pData = *pOutOld;						/* 开启中断再复制数据以减少中断时间 */
			err = QUEUE_OK;
		}
		else
		{                                           /* 空              */
			CAN_INT_RX_ENABLE();					/* 开启所有接收中断 */
			
			err = QUEUE_EMPTY;
			#if (CAN_QUEUE_READ_EMPTY_EN > 0)
			if (Queue->ReadEmpty != NULL)           /* 调用用户处理函数 */
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
** 函数名称: CAN_wrFilterExt32MaskMode
** 功能描述: 配置CAN ID过滤器:已确定模式: 32位, 屏蔽位模式
** 输　入:  1. filter_number: 过滤器号,0~13
**			2. id: 	标识符寄存器
**			3. mask: 标识符掩码(与标识符寄存器进行位对比),1:必须匹配, 0: 不用关心
**			4. fifo_number: 关联的FIFO, 0: 关联FIFO0, 1:关联FIFO1
** 输　出: 无
** 返　回: 错误代码
** 全局变量: 无
** 调用模块: 
** 版　本: v0.00.1
** 作　者: Xsky
** 日　期: 2010-03-17 11:50
** 备　注: 
**-------------------------------------------------------------------------------------------------------
** 版　本: 
** 修改人: 
** 日　期: 
** 描　述: 
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
	CAN1->FMR  |=  CAN_FMR_FINIT;       			// 进入过滤器设置模式
	CAN1->FA1R &=  ~(uint32_t)(FilterNOBitPos); 	// 禁用指定过滤器，以进入设置状态
	
	CAN1->FS1R |= (uint32_t)(FilterNOBitPos);		// 设置过滤器位宽为32位
	CAN1->FM1R &= ~(uint32_t)(FilterNOBitPos);		// 设置为屏蔽位模式
	
	CAN1->sFilterRegister[filter_number].FR1 = id; 	//  ID
	CAN1->sFilterRegister[filter_number].FR2 = mask;//  Mask
	
	if(fifo_number == 0){
		CAN1->FFA1R &= ~(uint32_t)(FilterNOBitPos);	// 过滤器分配至FIFO 0
	}else{
		CAN1->FFA1R |= (uint32_t)(FilterNOBitPos);	// 过滤器分配至FIFO 1
	}
	
	CAN1->FA1R  |=  (uint32_t)(FilterNOBitPos);  	// 开启过滤器
	
	CAN1->FMR &= ~CAN_FMR_FINIT;                    // 进入正常模式

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
	
	// CAN_RX0接收中断
	#ifndef STM32F10X_CL
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	#else
	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
	#endif // STM32F10X_CL
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	// CAN1_RX1接收中断
	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	// CAN发送中断
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
	
	// CAN1状态改变中断
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_CAN_SCE);	// 分配在低优先级组
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
	CAN_InitStructure.CAN_TTCM = DISABLE;		// 时间触发通信模式:关闭
	CAN_InitStructure.CAN_ABOM = ENABLE;		// 自动离线管理
	CAN_InitStructure.CAN_AWUM = DISABLE;		// 自动唤醒:关闭
	#ifdef DEBUG
	CAN_InitStructure.CAN_NART = ENABLE;// 调试
	#else
	CAN_InitStructure.CAN_NART = DISABLE;		// 禁止自动重传模式: ENABLE,CAN报文只被发送1次，不管发送的结果如何（成功、出错或仲裁丢失）; DISABLE:按照CAN标准，CAN硬件在发送报文失败时会一直自动重传直到发送成功；
	#endif
	CAN_InitStructure.CAN_RFLM = DISABLE;		// 锁定接收FIFO: 在接收溢出时FIFO未被锁定，当接收FIFO的报文未被读出，下一个收到的报文会覆盖原有报文
	CAN_InitStructure.CAN_TXFP = ENABLE;		// FIFO优先级: 通过对CAN_MCR寄存器的TXFP位置’1’，可以把发送邮箱配置为发送FIFO。在该模式下，发送的优先级由发送请求次序决定。
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;// CAN操作模式
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;	// 重新同步跳跃宽度
	CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;	// 时间段1
	CAN_InitStructure.CAN_BS2 = CAN_BS2_5tq;	// 时间段2
	CAN_InitStructure.CAN_Prescaler = stm32_GetPCLK1()/(1+6+5)/CAN_BPS;// 分频设置:
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
	/* 调用外部过滤器初始化函数,因此处标识符定义与通信协议高相关 */
	CAN_FilterConfig();
	
	/* CAN FIFO0 message pending interrupt enable */ 
	/* 一旦往FIFO存入一个报文，硬件就会更新FMP[1:0]位，并且如果CAN_IER寄存器的FMPIE位为’1’，那么就会产生一个中断请求。 */
	/* 开启:FIFO0消息挂号中断CAN_IT_FMP0|CAN_IT_FMP1, 
	   		发送邮箱空中断CAN_IT_TME, Datasheet.CN:p445 */
	CAN_ITConfig(CAN1, CAN_IT_FMP0 | CAN_IT_FMP1 | CAN_IT_TME	/* 发送接收中断 */
					  |	CAN_IT_ERR | CAN_IT_LEC | CAN_IT_BOF | CAN_IT_EPV /* 控制及状态变化中断 */
					  | CAN_IT_EWG , 
				 ENABLE);						//使能指定的CAN中断
	
	/* 初始化其它寄存器 */
	CAN1->ESR |= CAN_ESR_LEC_USER;				// 将上次错误号设置为用户状态(程序不会设置该状态)以区分新的状态
	
	/* 应用程序队列初始化 */
	// TX, 发送队列
	CAN_QueueCreate(&CanTxQueue, CanTxBuf, lenof(CanTxBuf), 
					CAN_QueueWriteFull, CAN_TxRx_Tx);			// 发送队列
	// 接收队列RX
	CAN_QueueCreate(&CanRxQueue, CanRxBuf, lenof(CanRxBuf), 
					CAN_QueueWriteFull, CAN_TxRx_Rx);			// 接收队列
	
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
/* CAN1控制及状态变化中断 */
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
	FIFO从空状态开始，在接收到第一个有效的报文后，FIFO状态变为挂号_1(pending_1)，
	硬件相应地把CAN_RFR寄存器的FMP[1:0]设置为’01’(二进制01b)。
	软件可以读取FIFO输出邮箱来读出邮箱中的报文，然后通过对CAN_RFR寄存器的RFOM位
	设置’1’来释放邮箱，这样FIFO又变为空状态了。如果在释放邮箱的同时，
	又收到了一个有效的报文，那么FIFO仍然保留在挂号_1状态，软件可以读取FIFO
	输出邮箱来读出新收到的报文。
	如果应用程序不释放邮箱，在接收到下一个有效的报文后，FIFO状态变为
	挂号_2(pending_2)，硬件相应地把FMP[1:0]设置为’10’(二进制10b)。
	重复上面的过程，第三个有效的报文把FIFO变为挂号_3状态(FMP[1:0]＝11b)。
	此时，软件必须对RFOM位设置1来释放邮箱，以便FIFO可以有空间来存放下一个有效的
	报文；否则，下一个有效的报文到来时就会导致一个报文的丢失。
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
	
    /* 发送FIFO空中断 */
    if(CanTxQueue.NData > 0)
    {
    	// 如果发送队列不为空
		dw_r = (CAN1->TSR >> CAN_TSR_TME_BITnL) & 0x07;	// 查找空的发送邮箱
		if(dw_r)
		{
			// 有空闲的发送邮箱
			dw_r = TxMailBox_IdleNO_Tbl[dw_r];			// 获取一个空邮箱号
			pTxMailBox = &CAN1->sTxMailBox[dw_r];
			CAN_QueueReadQuick((T_CanFrame *)pTxMailBox, &CanTxQueue);
			// 请求发送
			SETBITs(pTxMailBox->TIR, CAN_TIxR_TXRQ);
		}
    }
    else
    {
		/* Clear CAN_TSR_RQCPx (rc_w1) */
		CAN1->TSR = CAN_TSR_RQCP0|CAN_TSR_RQCP1|CAN_TSR_RQCP2;	// 清除发送请求,以防止再次进入中断, Add by Xsky 2011-07-30 17:51
	}
}

/*********************************************************************************************************
** 函数名称: CAN_SendBytes
** 功能描述: 将数据设置为欲发送的CAN发送邮箱格式T_CanFrame/CAN_TypeDef
** 			 发送时如果邮箱有空则直接将数据压入邮箱，否则将数据压入队列
**			 如果发送失败次数为3则取消发送
** 输　入:  1. : *pSrc:  要发送的数据源缓冲区
**			2. : nBytes: 要发送的数据字节数
**			3. : 
** 输　出: 无
** 返　回: 错误代码
** 全局变量: 无
** 调用模块: 
** 版　本: v0.00.1
** 作　者: Xsky
** 日　期: 2010-03-11 10:14
** 备　注: 
**-------------------------------------------------------------------------------------------------------
** 版　本: 
** 修改人: 
** 日　期: 
** 描　述: 
********************************************************************************************************/
t_err CAN_SendFrame(T_CanFrame *pCanFrame)
{
	//T_CanFrame CanFrame = {0, };
	register CAN_TxMailBox_TypeDef *pTxMailBox;
	register t_ureg dw_r;
	t_err err = ERR_NONE;
	/* 发送报文的流程为：应用程序选择1个空置的发送邮箱；设置标识符，
	   数据长度和待发送数据；然后对CAN_TIxR寄存器的TXRQ位置’1’，来请求发送。 
	   发送状态CAN_TSR,Datasheet.CN:p441 */
	
	CAN_QUE_OS_ENTER_CRITICAL();				// Add by Xsky 2011-07-30 14:26
	
	if(CanTxQueue.NData > 0)
	{
		// 队列不为空则直接写入发送队列
		#ifdef DBG_CAN_QUE_MAX
			// 调试,记录发送队列最大存储数量
			if(CanTxQueue.NData < g_History.SysErrors.CAN_SendQueMaxN){
				g_History.SysErrors.CAN_SendQueMaxN = CanTxQueue.NData;
			}
		#endif
		
		err = CAN_QueueWrite(&CanTxQueue, pCanFrame);
	}
	else
	{
		// 如果队列为空
		
		dw_r = (CAN1->TSR >> CAN_TSR_TME_BITnL) & 0x07;
		if(dw_r)	// 有空闲的发送邮箱
		{
			//CAN_INT_TX_DISABLE();	
			// 如果不使用操作系统，且不在中断中调用此函数发送数据则可不关闭中断
			
			pTxMailBox = &CAN1->sTxMailBox[TxMailBox_IdleNO_Tbl[dw_r]];
			//pTxMailBox->TIR = pCanFrame->IxR;
			pTxMailBox->TDTR = pCanFrame->DTxR;
			pTxMailBox->TDLR = pCanFrame->Data.u32[0];
			pTxMailBox->TDHR = pCanFrame->Data.u32[1];
			//SETBITs(pTxMailBox->TIR, CAN_TIxR_TXRQ);
			pTxMailBox->TIR = pCanFrame->IxR | CAN_TIxR_TXRQ;
			
			//CAN_INT_TX_ENABLE();	// 开启发送中断
		}
		else		// 无空闲邮箱
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
	// CAN接收到数据后先压入FIFO，然后触发该中断服务对应的软件中断
}

#endif	/* end: #if CAN1_SWI_HANDLE_EN > 0 */


#endif	/* end: #if CAN1_EN > 0 */

