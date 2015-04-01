/*******************************************************************************
  Copyright (C), 2009-2009, YingHong, Ltd.
  File name:   	Function.C	
  Author:      	Xsky
  Version:     	1.0
  Date:		2009-04-08 17:12	
  
  Description:   ���ܺ���, Ӳ��ƽ̨�޹ص�C�㷨����
  Others:         
  Function List: 
  	1. ��
  	2. ��
  	3. ��
  	4. ��
  	5. ��

  History:        // �޸ļ�¼
    1. Date:		2009-04-08 17:12
       Author:		Xsky
       Modification:	������ģ��
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
** ��������: QueueInit
** ��������: ��ʼ����������
** ��	��:	1. pQue:	ָ���������е�ָ��
**			2. nBufSize:	����������
** ��	��:	��
** ��	��:	��
** ȫ�ֱ���: ��
** ��������: memset()
** ��	��: Xsky
** ��	��: V1.00.00
** ��������: 2009-04-08 17:35
void QueueInit(void *pBuf, uint16 nBufSize)
{
	memset(pBuf, 0, sizeof(T_Queue));
	
	((T_Queue *)pBuf)->BufSize = nBufSize - (uint16)(((T_Queue *)0)->Buf);
}
*******************************************************************************/

/*******************************************************************************
** ��������: QueueWriteBytes
** ��������: �������д������
** ��	��:	1. pBuf:	������
**			2. pSrc:	Դ���ݻ�����
**			3. nSize:	Դ���ݳ���
** ��	��:	��
** ��	��:	��
** ȫ�ֱ���: ��
** ��������: 
** ��	��: Xsky
** ��	��: V1.00.00
** ��������: 2009-04-08 18:13
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
	if (Buf != NULL)                                                    		// �����Ƿ���Ч 
	{
		Queue = (T_Queue *)Buf;
		
		QUE_OS_ENTER_CRITICAL();
		//nQueDataSize = Queue->NData;
		
		if( (Queue->DataSize + nDataSize) > Queue->BufSize) 	// �����Ƿ�����  
		{											// �����
			err = QUEUE_FULL;							// �������������			
#if (QUEUE_WRITE_FULL_EN > 0)
			if(Queue->WriteFull != NULL){				// �����û���������
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
			Queue->In[0] = *pSrc;					// �������
			Queue->In++;							// �������ָ��
			
			if(Queue->In >= Queue->End)				// �Ƿ񵽶���β��
			{
				Queue->In = Queue->Buf;				// 
			}
			Queue->NData++;							// ������
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
	va_list ap;		// ����һ�� va_list ָ�������ʲ�����
	char sBuf[256];
	int32 i;
	t_err err = ERR_VSprintf;
	
	va_start(ap, fmt);                 	 	// ��ʼ�� ap������ָ���һ�����
	i = vsprintf(sBuf, fmt, ap);
	va_end(ap);                            	// �ƺ������ر� ap
	
	if(i >= 0)
	{
		err = UART1_SendBytes(sBuf, i);
	}
	return err;
}
#endif
