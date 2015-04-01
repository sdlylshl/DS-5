/****************************************Copyright (c)**************************************************
**                               广州周立功单片机发展有限公司
**                                     研    究    所
**                                        产品一部 
**
**                                 http://www.zlgmcu.com
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: queue.h
**创   建   人: 陈明计
**最后修改日期: 2003年7月2日
**描        述: 数据队列的中间件
**              
**--------------历史版本信息----------------------------------------------------------------------------
** 创建人: 陈明计
** 版  本: v1.0
** 日　期: 2003年7月2日
** 描　述: 原始版本
**
**--------------当前版本修订------------------------------------------------------------------------------
** 修改人: 陈明计
** 日　期: 2004年5月19日
** 描　述: 改正注释错误
**
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/


#ifndef NOT_OK
#define NOT_OK              0xff                        /* 参数错误                                     */
#endif

#define QUEUE_FULL          8                           /* 队列满                                       */
#define QUEUE_EMPTY         4                           /* 无数据                                       */
#define QUEUE_OK            0                           /* 操作成功                                     */

#define Q_WRITE_MODE        1                           /* 操作成功                                     */
#define Q_WRITE_FRONT_MODE  2                           /* 操作成功                                     */

#ifndef QUEUE_DATA_TYPE
#define QUEUE_DATA_TYPE     uint8
#endif

#ifndef QUEUE_READ_EMPTY_EN		
#define QUEUE_READ_EMPTY_EN		0
#endif
#ifndef QUEUE_WRITE_FULL_EN		
#define QUEUE_WRITE_FULL_EN		0
#endif

#define EN_QUEUE_READ_BYTES				1	// 禁止(0)允许(1)FIFO接收多字节数据
#define EN_QUEUE_WRITE            		0     /* 禁止(0)或允许(1)FIFO发送数据       */
#define EN_QUEUE_WRITE_BYTES         	1     /* 禁止(0)或允许(1)FIFO发送多字节数据       */
//#define EN_QUEUE_WRITE_FRONT      	0     /* 禁止(0)或允许(1)LIFO发送数据       */
#define EN_QUEUE_NDATA            		0     /* 禁止(0)或允许(1)取得队列数据数目   */
//#define EN_QUEUE_SIZE            		0     /* 禁止(0)或允许(1)取得队列数据总容量 */
#define EN_QUEUE_FLUSH           		1     /* 禁止(0)或允许(1)清空队列           */
#define EN_QUEUE_OUT_N					1	  /* 禁止(0)或允许(1)队列直接出队N个单元 */

//#define QUEUE_CRITICAL_EN		0	// 队列写入是否允许中断

#ifndef QUEUE_CRITICAL_EN	
#define  QUEUE_CRITICAL_EN		1
#endif
#if (QUEUE_CRITICAL_EN > 0)
#define QUE_OS_ENTER_CRITICAL()		OS_ENTER_CRITICAL()
#define QUE_OS_EXIT_CRITICAL()		OS_EXIT_CRITICAL()
#else
#define QUE_OS_ENTER_CRITICAL()	
#define QUE_OS_EXIT_CRITICAL()		
#endif


#ifndef T_QUEUE_DATA_SIZE
#define T_QUEUE_DATA_SIZE	uint16
#endif
typedef struct {
	// 经测试将In, Out, 定义为索引方式比定义为指针执行效率低
	QUEUE_DATA_TYPE     *Out;                   /* 指向数据输出位置         */
	QUEUE_DATA_TYPE     *In;                    /* 指向数据输入位置         */
	QUEUE_DATA_TYPE     *End;                   /* 指向Buf的结束位置        */
	uint16              NData;                  /* 队列中数据个数,以QUEUE_DATA_TYPE为单位计数 */
	uint16              MaxData;                /* 队列中允许存储的数据个数 */
#if (QUEUE_READ_EMPTY_EN > 0)
	uint8               (* ReadEmpty)(void);        /* 读空处理函数             */
#endif
#if (QUEUE_WRITE_FULL_EN > 0)
	uint8               (* WriteFull)(void);        /* 写满处理函数             */
#endif
	QUEUE_DATA_TYPE     Buf[1];                 /* 存储数据的空间           */
} T_DataQueue;


#ifndef IN_QUEUE
        uint8 QueueCreate(void *Buf,
                          T_QUEUE_DATA_SIZE SizeOfBuf
#if (QUEUE_READ_EMPTY_EN > 0)
                          , uint8 (* ReadEmpty)(void)
#endif
#if (QUEUE_WRITE_FULL_EN > 0)
                          ,uint8 (* WriteFull)(void)
#endif
                          );
/*********************************************************************************************************
** 函数名称: QueueCreate
** 功能描述: 初始化数据队列
** 输　入: Buf      ：为队列分配的存储空间地址
**         SizeOfBuf：为队列分配的存储空间大小（字节）
**         ReadEmpty：为队列读空时处理程序
**         WriteFull：为队列写满时处理程序
** 输　出: NOT_OK  :参数错误
**         QUEUE_OK:成功
** 全局变量: 无
** 调用模块: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
********************************************************************************************************/
#define QUEUE_READ_ISR_QUICK(Data, Queue)	\
	do{\
		Data = *(Queue->Out);\
		Queue->Out++;                                       /* 调整出队指针 */\
		if (Queue->Out >= Queue->End)\
		{\
			Queue->Out = Queue->Buf;\
		}\
		Queue->NData--;                                     /* 数据减少      */\
	}while(0)
#define QUEUE_READ_QUICK(Data, Queue)	\
	do{\
		QUE_OS_ENTER_CRITICAL();\
		if (Queue->NData > 0)                                  	/* 队列是否为空 */ \
		{                                                      			/* 不空         */\
			Data = *(Queue->Out);\
			Queue->Out++;                                       /* 调整出队指针 */\
			if (Queue->Out >= Queue->End)\
			{\
				Queue->Out = Queue->Buf;\
			}\
			Queue->NData--;                                     /* 数据减少      */\
		}\
		QUE_OS_EXIT_CRITICAL();\
	}while(0)
#define QUEUE_READ_QUICK_CRITICAL(Data, Queue, Enter, Exit)	\
	do{\
		Enter;\
		if (Queue->NData > 0)									/* 队列是否为空 */ \
		{																/* 不空 		*/\
			Data = *(Queue->Out);\
			Queue->Out++;										/* 调整出队指针 */\
			if (Queue->Out >= Queue->End)\
			{\
				Queue->Out = Queue->Buf;\
			}\
			Queue->NData--; 									/* 数据减少 	 */\
		}\
		Exit;\
	}while(0)
#define QUEUE_READ(Data, Queue, err)	\
	do{\
		QUE_OS_ENTER_CRITICAL();\
		if (Queue->NData > 0)                                  	/* 队列是否为空 */ \
		{                                                      			/* 不空         */\
			Data = *(Queue->Out);\
			Queue->Out++;                                       /* 调整出队指针 */\
			if (Queue->Out >= Queue->End)\
			{\
				Queue->Out = Queue->Buf;\
			}\
			Queue->NData--;                                     /* 数据减少      */\
			err = QUEUE_OK;\
		}\
		else\
		{                                                       /* 空              */\
			err = QUEUE_EMPTY;\
		}\
		QUE_OS_EXIT_CRITICAL();\
	}while(0)
#define QUEUE_READ_CRITECAL(Data, Queue, err)	\
	do{\
		OS_ENTER_CRITICAL();\
		if (Queue->NData > 0)                                  	/* 队列是否为空 */ \
		{                                                      			/* 不空         */\
			Data = *(Queue->Out);\
			Queue->Out++;                                       /* 调整出队指针 */\
			if (Queue->Out >= Queue->End)\
			{\
				Queue->Out = Queue->Buf;\
			}\
			Queue->NData--;                                     /* 数据减少      */\
			err = QUEUE_OK;\
		}\
		else\
		{                                                       /* 空              */\
			err = QUEUE_EMPTY;\
		}\
		OS_EXIT_CRITICAL();\
	}while(0)
extern uint16 QueueRead(/*QUEUE_DATA_TYPE *Ret, */void *Buf);
/*********************************************************************************************************
** 函数名称: QueueRead
** 功能描述: 获取队列中的数据
** 输　入: Ret:存储返回的消息的地址
**         Buf:指向队列的指针
** 输　出: NOT_OK     ：参数错误
**         QUEUE_OK   ：收到消息
**         QUEUE_EMPTY：队列空
** 全局变量: 无
** 调用模块: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
********************************************************************************************************/

extern uint16 QueueReadBytes(QUEUE_DATA_TYPE *DstBuf, uint16 NData, void *Buf);
/*********************************************************************************************************
** 函数名称: QueueReadBytes
** 功能描述: 获取队列中的多字节数据
** 输　入: //Ret:存储返回的消息的地址, 已删除
**         Buf:指向队列的指针
** 输　出:  NOT_OK     ：参数错误
**         		QUEUE_OK   ：收到消息
**         		QUEUE_EMPTY：无消息
** 全局变量: 无
** 调用模块: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
**
** 作　者: Xsky
** 日　期: 2009-04-09 11:10
********************************************************************************************************/
#define QUEUE_WRITE_ISR_QUICK(Queue, Data)	\
	do{\
		if (Queue->NData < Queue->MaxData)			/* 队列是否满  */ \
		{                                                               		/* 不满        */ \
			Queue->In[0] = Data;                                  	/* 数据入队    */ \
			Queue->In++;                                			/* 调整入队指针*/ \
			if (Queue->In >= Queue->End) \
			{ \
				Queue->In = Queue->Buf; \
			}\
			Queue->NData++;                               		/* 数据增加    */ \
		}\
	}while(0)

static __inline void QueueWriteQuickIsr(register T_DataQueue *Queue, QUEUE_DATA_TYPE Data)	
{
	if (Queue->NData < Queue->MaxData)							/* 队列是否满  	*/ 
	{															/* 不满 	   	*/ 
		Queue->In[0] = Data;									/* 数据入队    	*/ 
		Queue->In++;											/* 调整入队指针	*/ 
		if (Queue->In >= Queue->End) 
		{ 
			Queue->In = Queue->Buf; 
		}
		Queue->NData++; 										/* 数据增加    	*/ 
	}
	//else
	{
		//err = Queue->ReadEmpty(DstBuf, nSrcSize, Queue);
	//	NOP();
	}
}

#define QUEUE_WRITE_QUICK(Queue, Data)	\
	do{\
		QUE_OS_ENTER_CRITICAL(); \
		if (Queue->NData < Queue->MaxData)							/* 队列是否满  */ \
		{                                                               		/* 不满        */ \
			Queue->In[0] = (Data);                                	/* 数据入队    */ \
			Queue->In++;                                			/* 调整入队指针*/ \
			if (Queue->In >= Queue->End) \
			{ \
				Queue->In = Queue->Buf; \
			}\
			Queue->NData++;                               		/* 数据增加    */ \
		}\
		QUE_OS_EXIT_CRITICAL(); \
	}while(0)
#define QUEUE_WRITE_QUICK_CRITICAL(Queue, Data, Enter, Exit)	/* 使用自定义开关中断函数 */ \
	do{\
		Enter; \
		if (Queue->NData < Queue->MaxData)						/* 队列是否满  */ \
		{																		/* 不满 	   */ \
			Queue->In[0] = Data;									/* 数据入队    */ \
			Queue->In++;											/* 调整入队指针*/ \
			if (Queue->In >= Queue->End) \
			{ \
				Queue->In = Queue->Buf; \
			}\
			Queue->NData++; 									/* 数据增加    */ \
		}\
		Exit; \
	}while(0)
#define QUEUE_WRITE(Queue, Data, err)	\
	do{\
		QUE_OS_ENTER_CRITICAL(); \
		if (Queue->NData < Queue->MaxData)			/* 队列是否满  */ \
		{                                                               		/* 不满        */ \
			Queue->In[0] = Data;                                  	/* 数据入队    */ \
			Queue->In++;                                			/* 调整入队指针*/ \
			if (Queue->In >= Queue->End) \
			{ \
				Queue->In = Queue->Buf; \
			}\
			Queue->NData++;                               		/* 数据增加    */ \
			err = QUEUE_OK;\
		}\
		else{ err = QUEUE_FULL; } \
		QUE_OS_EXIT_CRITICAL(); \
	}while(0)
#define QUEUE_WRITE_CRITICAL(Queue, Data, err)	\
	do{\
		if (Queue->NData < Queue->MaxData)			/* 队列是否满  */ \
		{                                                               		/* 不满        */ \
			OS_ENTER_CRITICAL(); \
			Queue->In[0] = Data;                                  	/* 数据入队    */ \
			Queue->In++;                                			/* 调整入队指针*/ \
			if (Queue->In >= Queue->End) \
			{ \
				Queue->In = Queue->Buf; \
			}\
			Queue->NData++;                               		/* 数据增加    */ \
			OS_EXIT_CRITICAL(); \
			err = QUEUE_OK;\
		}\
		else{ err = QUEUE_FULL; } \
	}while(0)
extern uint8 QueueWrite(void *Buf, QUEUE_DATA_TYPE Data);
/*********************************************************************************************************
** 函数名称: QueueWrite
** 功能描述: FIFO方式发送数据
** 输　入: Buf :指向队列的指针
**         Data:发送的数据
** 输　出: NOT_OK   ：参数错误
**         QUEUE_FULL:队列满
**         QUEUE_OK  :发送成功
** 全局变量: 无
** 调用模块: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
********************************************************************************************************/

extern uint8 QueueWriteBytes(void *Buf, QUEUE_DATA_TYPE *pSrc, uint16 NData);
/*********************************************************************************************************
** 函数名称: QueueWriteBytes
** 功能描述: FIFO方式发送多字节数据
** 输　入: Buf :指向队列的指针
**         Data:消息数据
** 输　出: NOT_OK   :参数错误
**         QUEUE_FULL:队列满
**         QUEUE_OK  :发送成功
** 全局变量: 无
** 调用模块: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
**
** 作　者: 陈明计
** 日　期: 2003年7月2日
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/


extern uint8 QueueWriteFront(void *Buf, QUEUE_DATA_TYPE Data);
/*********************************************************************************************************
** 函数名称: QueueWrite
** 功能描述: LIFO方式发送数据
** 输　入: Buf:指向队列的指针
**         Data:消息数据
** 输　出: QUEUE_FULL:队列满
**         QUEUE_OK:发送成功
** 全局变量: 无
** 调用模块: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
********************************************************************************************************/

#define QUEUE_GET_N_DATA(Buf)	(((T_DataQueue *)Buf)->NData)

#if (EN_QUEUE_NDATA > 0)
extern uint16 QueueNData(void *Buf);
#endif
/*********************************************************************************************************
** 函数名称: QueueNData
** 功能描述: 取得队列中数据数
** 输　入: Buf:指向队列的指针
** 输　出: 消息数
** 全局变量: 无
** 调用模块: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
********************************************************************************************************/

extern uint16 QueueSize(void *Buf);
/*********************************************************************************************************
** 函数名称: QueueSize
** 功能描述: 取得队列总容量
** 输　入: Buf:指向队列的指针
** 输　出: 队列总容量
** 全局变量: 无
** 调用模块: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
********************************************************************************************************/

extern void QueueFlush(void *Buf);
/*********************************************************************************************************
** 函数名称: OSQFlush
** 功能描述: 清空队列
** 输　入: Buf:指向队列的指针
** 输　出: 无
** 全局变量: 无
** 调用模块: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
********************************************************************************************************/


extern void QueueOutN(void *Buf, uint32 N);
/*********************************************************************************************************
** 函数名称: QueueOutN
** 功能描述: 队列出队N个单元, 直接调整指针
** 输　入: Buf:指向队列的指针
** 输　出: 无
** 全局变量: 无
** 调用模块: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
********************************************************************************************************/

#endif		// end: #ifndef IN_QUEUE //

/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
