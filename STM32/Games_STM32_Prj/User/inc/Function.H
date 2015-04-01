/*******************************************************************************
  Copyright (C), 2009-2011, YingHong, Ltd.
  File name:   	Function.H	
  Author:      	Xsky
  Version:     	1.0
  Date:		2011-02-25 11:50
  
  Description:   功能函数及定义, 与硬件完全无关的函数定义
  Others:         
  Function List: 
  	1. ：
  	2. ：
  	3. ：
  	4. ：
  	5. ：

  History:        // 修改记录
    1. Date:		2011-02-25 11:50
       Author:		Xsky
       Modification:	建立本模块
*******************************************************************************/

#include "Global.h"

#ifndef __FUNCTION_H__
#define __FUNCTION_H__

#ifdef FUNCTION_GLOBALS
#define FUNC_EXT
#else
#define FUNC_EXT	extern
#endif

#ifndef FUNCTION_GLOBALS
extern __flash char HEX_CHAR_TBL[16];
#endif
// 将半字节数值型换为十六进制字符
#define Bin2HexChar(val)	HEX_CHAR_TBL[(val)]	



/*
#define EN_QUEUE_WRITE			1

#define T_INDEX_QUE_DATA	uint8
typedef struct _T_Queue{		// 索引式队列，无数据区域
	// 经测试将In, Out, 定义为索引方式比定义为指针执行效率低
	uint16	Out;					// out索引
	uint16	In;					// in索引
	uint16	End;			
	uint16	DataSize;			// 数据有效个数
	uint16	BufSize;				// 缓冲区大小
#if (QUEUE_READ_EMPTY_EN > 0)
    uint8               (* ReadEmpty)(void);        // 读空处理函数             
#endif
#if (QUEUE_WRITE_FULL_EN > 0)
    uint8               (* WriteFull)(void);        //  写满处理函数             
#endif

	uint8	Buf[1];				// 队列数据区T_INDEX_QUE_DATA
}T_Queue;
*/

/*
typedef struct _T_QueBuf{
	T_Queue	Queue;			// 队列指针
	uint8	Data[1];			// 队列长度
}T_QueBuf;
*/

/**************************************************************************************/
// 函数声明

/*******************************************************************************
** 函数名称: IndexQueueInit
** 功能描述: 初始化索引队列
FUNC_EXT void QueueInit(void *pBuf, uint16 nBufSize);
*******************************************************************************/



FUNC_EXT t_err DbgPrintf(const char *fmt, ...);
#ifdef DEBUG
#define _DbgPrintf_(printf_exp)	DbgPrintf printf_exp
#else
#define _DbgPrintf_(x)	

#endif


#endif		// end: #ifndef __FUNCTION_H__ //
