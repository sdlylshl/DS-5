/*******************************************************************************
  Copyright (C), 2009-2009, YingHong, Ltd.
  File name:   	Function.C	
  Author:      	Xsky
  Version:     	1.0
  Date:		2009-04-08 17:12	
  
  Description:   功能函数, 硬件平台无关的C算法函数
  Others:         
  Function List: 
  	1. ：
  	2. ：
  	3. ：
  	4. ：
  	5. ：

  History:        // 修改记录
    1. Date:		2009-04-08 17:12
       Author:		Xsky
       Modification:	建立本模块
*******************************************************************************/

#define FUNCTION_GLOBALS
#include "Global.H"
#include "Function.h"
//#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <String.H>
#include "Uart1.h"

__flash char HEX_CHAR_TBL[] = "0123456789ABCDEF";


/*******************************************************************************
** 函数名称: QueueInit
** 功能描述: 初始化索引队列
** 输	入:	1. pQue:	指向索引队列的指针
**			2. nBufSize:	缓冲区长度
** 输	出:	无
** 返	回:	无
** 全局变量: 无
** 函数调用: memset()
** 作	者: Xsky
** 版	本: V1.00.00
** 创建日期: 2009-04-08 17:35
void QueueInit(void *pBuf, uint16 nBufSize)
{
	memset(pBuf, 0, sizeof(T_Queue));
	
	((T_Queue *)pBuf)->BufSize = nBufSize - (uint16)(((T_Queue *)0)->Buf);
}
*******************************************************************************/

/*******************************************************************************
** 函数名称: QueueWriteBytes
** 功能描述: 向队列中写入数据
** 输	入:	1. pBuf:	缓冲区
**			2. pSrc:	源数据缓冲区
**			3. nSize:	源数据长度
** 输	出:	无
** 返	回:	无
** 全局变量: 无
** 函数调用: 
** 作	者: Xsky
** 版	本: V1.00.00
** 创建日期: 2009-04-08 18:13
*******************************************************************************/
#define EN_IQUEUE_WRITE_BYTES		0
#ifndef EN_IQUEUE_WRITE_BYTES
#define EN_IQUEUE_WRITE_BYTES		0
#endif
#if EN_IQUEUE_WRITE_BYTES > 0
uint8 IQueueWriteBytes(void *Buf, QUEUE_DATA_TYPE *pSrc, uint16 NData)
{
	register T_Queue *Queue;
	register QUEUE_DATA_TYPE *pQueBuf, *pSrcData;
	register uint16 iQueIn, iQueEnd;
	//register uint16 nQueDataSize;
	register uint16 nDataSize = NData;
	register uint8 err;
	
	err = NOT_OK;
	if (Buf != NULL)                                                    		// 队列是否有效 
	{
		Queue = (T_Queue *)Buf;
		
		QUE_OS_ENTER_CRITICAL();
		//nQueDataSize = Queue->NData;
		
		if( (Queue->DataSize + nDataSize) > Queue->BufSize) 	// 队列是否会溢出  
		{											// 会溢出
			err = QUEUE_FULL;							// 返回溢出错误码			
#if (QUEUE_WRITE_FULL_EN > 0)
			if(Queue->WriteFull != NULL){				// 调用用户处理函数
			err = Queue->WriteFull(Queue, pSrc, nDataSize);
			}
#endif
			QUE_OS_EXIT_CRITICAL();
			return err;
		}
		
		iQueIn = Queue->In;
		iQueEnd = Queue->End;
		pQueBuf = Queue->Buf;
		pSrcData = pSrc;
		while(nDataSize--)
		{
			//Queue->Buf[iQueIn++] = *pSrc++;
			pQueBuf[iQueIn] = *pSrcData++;
			iQueIn++;
			//*pQueIn++ = *pSrc++;
			if(iQueIn == iQueEnd){
				iQueIn = 0;
			}
			//nQueDataSize++;
			/*
			Queue->In[0] = *pSrc;					// 数据入队
			Queue->In++;							// 调整入队指针
			
			if(Queue->In >= Queue->End)				// 是否到队列尾部
			{
				Queue->In = Queue->Buf;				// 
			}
			Queue->NData++;							// 计数加
			*/
		}
		Queue->In = iQueIn;
		//Queue->NData = nQueDataSize;
		Queue->DataSize += NData;
		err = QUEUE_OK;
		
 		QUE_OS_EXIT_CRITICAL();
	}
	return err;
}
#endif		// end: #if EN_QUEUE_WRITE_BYTES > 0 //

#if IS_N_RELEASE()
t_err DbgPrintf(const char *fmt, ...)
{
	va_list ap;		// 定义一个 va_list 指针来访问参数表
	char sBuf[256];
	int32 i;
	t_err err = ERR_VSprintf;
	
	va_start(ap, fmt);                 	 	// 初始化 ap，让它指向第一个变参
	i = vsprintf(sBuf, fmt, ap);
	va_end(ap);                            	// 善后工作，关闭 ap
	
	if(i >= 0)
	{
		err = UART1_SendBytes(sBuf, i);
	}
	return err;
}
#endif

