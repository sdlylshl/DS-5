/****************************************Copyright (c)**************************************************
**                               广州周立功单片机发展有限公司
**                                     研    究    所
**                                        产品一部 
**
**                                 http://www.zlgmcu.com
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: queue.c
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
** 描　述: 改正注释错误和常量引用错误
**
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/

#define IN_QUEUE
//#include "Global.h"
#include "zlgQueue.h"
/*********************************************************************************************************
** 函数名称: QueueCreate
** 功能描述: 初始化数据队列
** 输　入: Buf      ：为队列分配的存储空间地址
**         SizeOfBuf：为队列分配的存储空间大小（字节）
**         ReadEmpty：为队列读空时处理程序
**         WriteFull：为队列写满时处理程序
** 输　出: NOT_OK:参数错误
**         QUEUE_OK:成功
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
uint8_t QueueCreate(void *Buf,
                          T_QUEUE_DATA_SIZE SizeOfBuf
#if (QUEUE_READ_EMPTY_EN > 0)
                          , uint8_t (* ReadEmpty)()
#endif
#if (QUEUE_WRITE_FULL_EN > 0)
                          ,uint8_t (* WriteFull)()
#endif
                          )
{
	T_DataQueue *Queue;
	
   	//uint16_t	i;
	//i =   	(uint16_t)(((T_DataQueue *)0)->Buf);
    	
	if (Buf != NULL && SizeOfBuf >= (sizeof(T_DataQueue)))        /* 判断参数是否有效 */
	{
		Queue = (T_DataQueue *)Buf;
		
		QUE_OS_ENTER_CRITICAL();
		                                                    /* 初始化结构体数据 */
		Queue->MaxData = (SizeOfBuf - (T_QUEUE_DATA_SIZE)(((T_DataQueue *)0)->Buf)) / 
		             sizeof(QUEUE_DATA_TYPE);               /* 计算队列可以存储的数据数目 */
		Queue->End = Queue->Buf + Queue->MaxData;               /* 计算数据缓冲的结束地址 */
		Queue->Out = Queue->Buf;
		Queue->In = Queue->Buf;
		Queue->NData = 0;
		
		#if (QUEUE_READ_EMPTY_EN > 0)
		Queue->ReadEmpty = ReadEmpty;
		#endif
		#if (QUEUE_WRITE_FULL_EN > 0)
		Queue->WriteFull = WriteFull;
		#endif
		
		QUE_OS_EXIT_CRITICAL();

		return QUEUE_OK;
	}
 	else
	{
		return NOT_OK;
	}
}


/*********************************************************************************************************
** 函数名称: QueueRead
** 功能描述: 获取队列中的数据
** 输　入: //Ret:存储返回的消息的地址, 已删除
**         Buf:指向队列的指针
** 输　出: 低位为数据
**		高位(错误码): NOT_OK     ：参数错误
**         					QUEUE_OK   ：收到消息
**         					QUEUE_EMPTY：无消息
** 全局变量: 无
** 调用模块: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
**
** 作　者: 陈明计
** 日　期: 2003年7月2日
**-------------------------------------------------------------------------------------------------------
** 修改人: Xsky
** 日　期: 2009-04-09 11:10
** 描　述: 修改返回值: uint8_t->uint16_t, 删除传入参数Ret
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
uint16_t QueueRead(/*QUEUE_DATA_TYPE *Ret,*/ void *Buf)
{
	register uint8_t err;
	register uint16_t uData;
	    T_DataQueue *Queue;
	
	err = NOT_OK;
	//uData = (NOT_OK << 8);
	if (Buf != NULL)                                            /* 队列是否有效 */
	{                                                           /* 有效 */
		Queue = (T_DataQueue *)Buf;
		
		QUE_OS_ENTER_CRITICAL();
		
		if (Queue->NData > 0)                                   /* 队列是否为空 */
		{                                                       /* 不空         */
			//*Ret = Queue->Out[0];                               /* 数据出队     */
			uData = *(Queue->Out);
			Queue->Out++;                                       /* 调整出队指针 */
			if (Queue->Out >= Queue->End)
			{
				Queue->Out = Queue->Buf;
			}
			Queue->NData--;                                     /* 数据减少      */
			err = QUEUE_OK;
		}
		else
		{                                                       /* 空              */
			err = QUEUE_EMPTY;
#if (QUEUE_READ_EMPTY_EN > 0)
			if (Queue->ReadEmpty != NULL)                       /* 调用用户处理函数 */
			{
				err = Queue->ReadEmpty(Ret, Queue);
			}
#endif
		}
		QUE_OS_EXIT_CRITICAL();
	}
	return (err<<8) | uData;
}

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
**-------------------------------------------------------------------------------------------------------
** 修改人: 
** 日　期: 
** 描　述: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#ifndef EN_QUEUE_READ_BYTES
#define EN_QUEUE_READ_BYTES		0
#endif
#if EN_QUEUE_READ_BYTES > 0
uint16_t QueueReadBytes(QUEUE_DATA_TYPE *DstBuf, uint16_t NData, void *Buf)
{
	register QUEUE_DATA_TYPE *pQueOut;
	register QUEUE_DATA_TYPE *pQueEnd;
	//register QUEUE_DATA_TYPE *pSrcData;
	register uint16_t nReadSize;
	
	register    T_DataQueue *Queue;
	//register uint8_t err;
	
	//err = NOT_OK;
	if (Buf != NULL)                                            /* 队列是否有效 */
	{                                                           		/* 有效 */
		Queue = (T_DataQueue *)Buf;
		
		QUE_OS_ENTER_CRITICAL();
		
		nReadSize = Queue->NData;
		if(nReadSize > 0)
		{                                                       /* 空              */
			pQueOut = Queue->Out;
			pQueEnd = Queue->End;
			//nReadSize = Queue->NData;
			if(NData > nReadSize)
			{
				NData = nReadSize;
			}
			nReadSize = NData;									/* 记录读取字节数 */
			while ((NData--)/* && (nReadSize--)*/)    			/* 队列是否为空 */
			{                                                       	/* 不空         */
				*DstBuf++ = *pQueOut++;                               /* 数据出队     */
				if (pQueOut >= pQueEnd)
				{
					pQueOut = Queue->Buf;
				}
				//nReadSize++;
				//Queue->NData--;                                     /* 数据减少      */
			}
			Queue->NData -= nReadSize;
			Queue->Out = pQueOut;
			//err = QUEUE_OK;
		}
		#if (QUEUE_READ_EMPTY_EN > 0)
		else
		{
			//err = QUEUE_EMPTY;
			nReadSize = 0;
			if (Queue->ReadEmpty != NULL)                       /* 调用用户处理函数 */
			{
				//err = Queue->ReadEmpty(DstBuf, nSrcSize, Queue);
				nReadSize = Queue->ReadEmpty(DstBuf, NData, Queue);
			}
		}
		#endif
		
		QUE_OS_EXIT_CRITICAL();
	}
	//return err;
	return nReadSize;
}
#endif	// end: #if EN_QUEUE_READ_BYTES > 0 //

/*********************************************************************************************************
** 函数名称: QueueWrite
** 功能描述: FIFO方式发送一个字节数据
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
#ifndef EN_QUEUE_WRITE
#define EN_QUEUE_WRITE      0
#endif

#if EN_QUEUE_WRITE > 0

        uint8_t QueueWrite(void *Buf, QUEUE_DATA_TYPE Data)
{
    uint8_t err;
    T_DataQueue *Queue;
	
    err = NOT_OK;
    if (Buf != NULL)                                                    /* 队列是否有效 */
    {
        Queue = (T_DataQueue *)Buf;
        
        QUE_OS_ENTER_CRITICAL();
        
        if (Queue->NData < Queue->MaxData)                              /* 队列是否满  */
        {                                                               		/* 不满        */
            Queue->In[0] = Data;                                        /* 数据入队    */
            Queue->In++;                                                /* 调整入队指针*/
            if (Queue->In >= Queue->End)
            {
                Queue->In = Queue->Buf;
            }
            Queue->NData++;                                             /* 数据增加    */
            err = QUEUE_OK;
        }
        else
        {                                                               /* 满           */
            err = QUEUE_FULL;
			#if (QUEUE_WRITE_FULL_EN > 0)
            if (Queue->WriteFull != NULL)                               /* 调用用户处理函数 */
            {
                err = Queue->WriteFull(Queue, Data, Q_WRITE_MODE);
            }
			#endif
        }
        QUE_OS_EXIT_CRITICAL();
    }
    return err;
}
#endif

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
#ifndef EN_QUEUE_WRITE_BYTES
#define EN_QUEUE_WRITE_BYTES		0
#endif
#if EN_QUEUE_WRITE_BYTES > 0
uint8_t QueueWriteBytes(void *Buf, QUEUE_DATA_TYPE *pSrc, uint16_t NData)
{
	register QUEUE_DATA_TYPE *pQueIn;
	register QUEUE_DATA_TYPE *pQueEnd;
	register QUEUE_DATA_TYPE *pSrcData;
	register uint16_t nDataSize;
	//register uint16_t nQueDataSize;
	register T_DataQueue *Queue;
	register uint8_t err;
	
	err = NOT_OK;
	if (Buf != NULL)                                                    		// 队列是否有效 
	{
		Queue = (T_DataQueue *)Buf;
		
		QUE_OS_ENTER_CRITICAL();
		//nQueDataSize = Queue->NData;
		nDataSize = NData;
		
		if( (Queue->NData + nDataSize) > Queue->MaxData) 	// 队列是否会溢出  
		{											// 会溢出
			err = QUEUE_FULL;							// 返回溢出错误码			
			
			#if (QUEUE_WRITE_FULL_EN > 0)
			if(Queue->WriteFull != NULL){				// 调用用户处理函数
			err = Queue->WriteFull(Queue, pSrc, nDataSize);
			}
			#endif
			//QUE_OS_EXIT_CRITICAL();
			//return err;
		}
		else
		{
			pQueIn = Queue->In;
			pQueEnd = Queue->End;
			
			//Queue->In = &(Queue->Buf[((pQueIn - Queue->Buf) + nDataSize) % Queue->MaxData]);
			//Queue->NData += NData;
			//QUE_OS_EXIT_CRITICAL();
			
			pSrcData = pSrc;
			while(nDataSize--)
			{
				*pQueIn++ = *pSrcData++;
				if(pQueIn == pQueEnd){
					pQueIn = Queue->Buf;
				}
			}
			Queue->In = pQueIn;
			Queue->NData += NData;
			
			err = QUEUE_OK;
			
		}
		
 		QUE_OS_EXIT_CRITICAL();
 		
	}
	return err;
}
#endif		// end: #if EN_QUEUE_WRITE_BYTES > 0 //

/*********************************************************************************************************
** 函数名称: QueueWriteFront
** 功能描述: LIFO方式发送数据
** 输　入: Buf:指向队列的指针
**         Data:消息数据
** 输　出: QUEUE_FULL:队列满
**         QUEUE_OK:发送成功
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
#ifndef EN_QUEUE_WRITE_FRONT
#define EN_QUEUE_WRITE_FRONT    0
#endif

#if EN_QUEUE_WRITE_FRONT > 0

        uint8_t QueueWriteFront(void *Buf, QUEUE_DATA_TYPE Data)
{
    uint8_t err;
    T_DataQueue *Queue;

    err = NOT_OK;
    if (Buf != NULL)                                                    /* 队列是否有效 */
    {
        Queue = (T_DataQueue *)Buf;
        
        QUE_OS_ENTER_CRITICAL();
        
        if (Queue->NData < Queue->MaxData)                              /* 队列是否满  */
        {                                                               /* 不满 */
            Queue->Out--;                                               /* 调整出队指针 */
            if (Queue->Out < Queue->Buf)
            {
                Queue->Out = Queue->End - 1;
            }
            Queue->Out[0] = Data;                                       /* 数据入队     */
            Queue->NData++;                                             /* 数据数目增加 */
            err = QUEUE_OK;
        }
        else
        {                                                               /* 满           */
            err = QUEUE_FULL;
            
			#if (QUEUE_WRITE_FULL_EN > 0)
            if (Queue->WriteFull != NULL)                               /* 调用用户处理函数 */
            {
                err = Queue->WriteFull(Queue, Data, Q_WRITE_FRONT_MODE);
            }
			#endif
        }
        QUE_OS_EXIT_CRITICAL();
    }
    return err;
}

#endif

/*********************************************************************************************************
** 函数名称: QueueNData
** 功能描述: 取得队列中数据数
** 输　入: Buf:指向队列的指针
** 输　出: 消息数
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
#ifndef EN_QUEUE_NDATA
#define EN_QUEUE_NDATA    0
#endif
#if EN_QUEUE_NDATA > 0
       uint16_t QueueNData(void *Buf)
{
    uint16_t temp;
    
    temp = 0;                                                   /* 队列无效返回0 */
    if (Buf != NULL)
    {
        QUE_OS_ENTER_CRITICAL();
        temp = ((T_DataQueue *)Buf)->NData;
        QUE_OS_EXIT_CRITICAL();
    }
    return temp;
}
#endif		// end: #if EN_QUEUE_NDATA > 0 //

/*********************************************************************************************************
** 函数名称: QueueSize
** 功能描述: 取得队列总容量
** 输　入: Buf:指向队列的指针
** 输　出: 队列总容量
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
#ifndef EN_QUEUE_SIZE
#define EN_QUEUE_SIZE    0
#endif

#if EN_QUEUE_SIZE > 0

        uint16_t QueueSize(void *Buf)
{
    uint16_t temp;
    
    temp = 0;                                                   /* 队列无效返回0 */
    if (Buf != NULL)
    {
        QUE_OS_ENTER_CRITICAL();
        temp = ((T_DataQueue *)Buf)->MaxData;
        QUE_OS_EXIT_CRITICAL();
    }
    return temp;
}

#endif

/*********************************************************************************************************
** 函数名称: OSQFlush
** 功能描述: 清空队列
** 输　入: Buf:指向队列的指针
** 输　出: 无
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
#ifndef EN_QUEUE_FLUSH
#define EN_QUEUE_FLUSH    0
#endif

#if EN_QUEUE_FLUSH > 0

        void QueueFlush(void *Buf)
{
    T_DataQueue *Queue;
    
    if (Buf != NULL)                                                /* 队列是否有效 */
    {                                                               /* 有效         */
        Queue = (T_DataQueue *)Buf;
        QUE_OS_ENTER_CRITICAL();
        Queue->Out = Queue->Buf;
        Queue->In = Queue->Buf;
        Queue->NData = 0;                                           /* 数据数目为0 */
        QUE_OS_EXIT_CRITICAL();
    }
}

#endif

/*********************************************************************************************************
** 函数名称: QueueOutN
** 功能描述: 队列出队N个单元, 直接调整指针
** 输　入: Buf:指向队列的指针
** 输　出: 无
** 全局变量: 无
** 调用模块: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
**
** 作　者: Xsky
** 日　期: 2011-04-02 15:16
**-------------------------------------------------------------------------------------------------------
** 修改人:
** 日　期:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#ifndef EN_QUEUE_OUT_N
#define EN_QUEUE_OUT_N    0
#endif

#if EN_QUEUE_OUT_N > 0

        void QueueOutN(void *Buf, uint32_t N)
{
    register T_DataQueue *Queue;
    register uint32_t N0;
    register QUEUE_DATA_TYPE *pBuf;
    register uint32_t dN;
    
    if (Buf != NULL)                                                /* 队列是否有效 */
    {                                                               /* 有效         */
        Queue = (T_DataQueue *)Buf;
        QUE_OS_ENTER_CRITICAL();
        N0 = Queue->NData;
        if(N > N0) N = N0;
        Queue->NData = N0 - N;                                      /* 数据数目为0 */
        //Queue->Out = (void *)((uint32_t)Queue->Buf + 				// Del by Xsky 2011-07-29 11:50
        //			  	(uint32_t)(Queue->Out + N)-(uint32_t)Queue->End);
        
        //pOut = Queue->Out;
        pBuf = Queue->Buf;											// Add by Xsky 2011-07-29 11:50
        dN = (Queue->Out - pBuf) + N;
        dN = dN % Queue->MaxData;
        Queue->Out = pBuf + dN;
        
        //Queue->In = Queue->Buf;
        QUE_OS_EXIT_CRITICAL();
    }
}

#endif


/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
