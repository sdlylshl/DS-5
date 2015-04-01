/****************************************Copyright (c)**************************************************
**                               ������������Ƭ����չ���޹�˾
**                                     ��    ��    ��
**                                        ��Ʒһ�� 
**
**                                 http://www.zlgmcu.com
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: queue.c
**��   ��   ��: ������
**����޸�����: 2003��7��2��
**��        ��: ���ݶ��е��м��
**              
**--------------��ʷ�汾��Ϣ----------------------------------------------------------------------------
** ������: ������
** ��  ��: v1.0
** �ա���: 2003��7��2��
** �衡��: ԭʼ�汾
**
**--------------��ǰ�汾�޶�------------------------------------------------------------------------------
** �޸���: ������
** �ա���: 2004��5��19��
** �衡��: ����ע�ʹ���ͳ������ô���
**
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/

#define IN_QUEUE
//#include "Global.h"
#include "zlgQueue.h"
/*********************************************************************************************************
** ��������: QueueCreate
** ��������: ��ʼ�����ݶ���
** �䡡��: Buf      ��Ϊ���з���Ĵ洢�ռ��ַ
**         SizeOfBuf��Ϊ���з���Ĵ洢�ռ��С���ֽڣ�
**         ReadEmpty��Ϊ���ж���ʱ��������
**         WriteFull��Ϊ����д��ʱ��������
** �䡡��: NOT_OK:��������
**         QUEUE_OK:�ɹ�
** ȫ�ֱ���: ��
** ����ģ��: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
**
** ������: ������
** �ա���: 2003��7��2��
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
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
    	
	if (Buf != NULL && SizeOfBuf >= (sizeof(T_DataQueue)))        /* �жϲ����Ƿ���Ч */
	{
		Queue = (T_DataQueue *)Buf;
		
		QUE_OS_ENTER_CRITICAL();
		                                                    /* ��ʼ���ṹ������ */
		Queue->MaxData = (SizeOfBuf - (T_QUEUE_DATA_SIZE)(((T_DataQueue *)0)->Buf)) / 
		             sizeof(QUEUE_DATA_TYPE);               /* ������п��Դ洢��������Ŀ */
		Queue->End = Queue->Buf + Queue->MaxData;               /* �������ݻ���Ľ�����ַ */
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
** ��������: QueueRead
** ��������: ��ȡ�����е�����
** �䡡��: //Ret:�洢���ص���Ϣ�ĵ�ַ, ��ɾ��
**         Buf:ָ����е�ָ��
** �䡡��: ��λΪ����
**		��λ(������): NOT_OK     ����������
**         					QUEUE_OK   ���յ���Ϣ
**         					QUEUE_EMPTY������Ϣ
** ȫ�ֱ���: ��
** ����ģ��: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
**
** ������: ������
** �ա���: 2003��7��2��
**-------------------------------------------------------------------------------------------------------
** �޸���: Xsky
** �ա���: 2009-04-09 11:10
** �衡��: �޸ķ���ֵ: uint8_t->uint16_t, ɾ���������Ret
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
uint16_t QueueRead(/*QUEUE_DATA_TYPE *Ret,*/ void *Buf)
{
	register uint8_t err;
	register uint16_t uData;
	    T_DataQueue *Queue;
	
	err = NOT_OK;
	//uData = (NOT_OK << 8);
	if (Buf != NULL)                                            /* �����Ƿ���Ч */
	{                                                           /* ��Ч */
		Queue = (T_DataQueue *)Buf;
		
		QUE_OS_ENTER_CRITICAL();
		
		if (Queue->NData > 0)                                   /* �����Ƿ�Ϊ�� */
		{                                                       /* ����         */
			//*Ret = Queue->Out[0];                               /* ���ݳ���     */
			uData = *(Queue->Out);
			Queue->Out++;                                       /* ��������ָ�� */
			if (Queue->Out >= Queue->End)
			{
				Queue->Out = Queue->Buf;
			}
			Queue->NData--;                                     /* ���ݼ���      */
			err = QUEUE_OK;
		}
		else
		{                                                       /* ��              */
			err = QUEUE_EMPTY;
#if (QUEUE_READ_EMPTY_EN > 0)
			if (Queue->ReadEmpty != NULL)                       /* �����û��������� */
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
** ��������: QueueReadBytes
** ��������: ��ȡ�����еĶ��ֽ�����
** �䡡��: //Ret:�洢���ص���Ϣ�ĵ�ַ, ��ɾ��
**         Buf:ָ����е�ָ��
** �䡡��:  NOT_OK     ����������
**         		QUEUE_OK   ���յ���Ϣ
**         		QUEUE_EMPTY������Ϣ
** ȫ�ֱ���: ��
** ����ģ��: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
**
** ������: Xsky
** �ա���: 2009-04-09 11:10
**-------------------------------------------------------------------------------------------------------
** �޸���: 
** �ա���: 
** �衡��: 
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
	if (Buf != NULL)                                            /* �����Ƿ���Ч */
	{                                                           		/* ��Ч */
		Queue = (T_DataQueue *)Buf;
		
		QUE_OS_ENTER_CRITICAL();
		
		nReadSize = Queue->NData;
		if(nReadSize > 0)
		{                                                       /* ��              */
			pQueOut = Queue->Out;
			pQueEnd = Queue->End;
			//nReadSize = Queue->NData;
			if(NData > nReadSize)
			{
				NData = nReadSize;
			}
			nReadSize = NData;									/* ��¼��ȡ�ֽ��� */
			while ((NData--)/* && (nReadSize--)*/)    			/* �����Ƿ�Ϊ�� */
			{                                                       	/* ����         */
				*DstBuf++ = *pQueOut++;                               /* ���ݳ���     */
				if (pQueOut >= pQueEnd)
				{
					pQueOut = Queue->Buf;
				}
				//nReadSize++;
				//Queue->NData--;                                     /* ���ݼ���      */
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
			if (Queue->ReadEmpty != NULL)                       /* �����û��������� */
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
** ��������: QueueWrite
** ��������: FIFO��ʽ����һ���ֽ�����
** �䡡��: Buf :ָ����е�ָ��
**         Data:��Ϣ����
** �䡡��: NOT_OK   :��������
**         QUEUE_FULL:������
**         QUEUE_OK  :���ͳɹ�
** ȫ�ֱ���: ��
** ����ģ��: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
**
** ������: ������
** �ա���: 2003��7��2��
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
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
    if (Buf != NULL)                                                    /* �����Ƿ���Ч */
    {
        Queue = (T_DataQueue *)Buf;
        
        QUE_OS_ENTER_CRITICAL();
        
        if (Queue->NData < Queue->MaxData)                              /* �����Ƿ���  */
        {                                                               		/* ����        */
            Queue->In[0] = Data;                                        /* �������    */
            Queue->In++;                                                /* �������ָ��*/
            if (Queue->In >= Queue->End)
            {
                Queue->In = Queue->Buf;
            }
            Queue->NData++;                                             /* ��������    */
            err = QUEUE_OK;
        }
        else
        {                                                               /* ��           */
            err = QUEUE_FULL;
			#if (QUEUE_WRITE_FULL_EN > 0)
            if (Queue->WriteFull != NULL)                               /* �����û��������� */
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
** ��������: QueueWriteBytes
** ��������: FIFO��ʽ���Ͷ��ֽ�����
** �䡡��: Buf :ָ����е�ָ��
**         Data:��Ϣ����
** �䡡��: NOT_OK   :��������
**         QUEUE_FULL:������
**         QUEUE_OK  :���ͳɹ�
** ȫ�ֱ���: ��
** ����ģ��: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
**
** ������: ������
** �ա���: 2003��7��2��
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
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
	if (Buf != NULL)                                                    		// �����Ƿ���Ч 
	{
		Queue = (T_DataQueue *)Buf;
		
		QUE_OS_ENTER_CRITICAL();
		//nQueDataSize = Queue->NData;
		nDataSize = NData;
		
		if( (Queue->NData + nDataSize) > Queue->MaxData) 	// �����Ƿ�����  
		{											// �����
			err = QUEUE_FULL;							// �������������			
			
			#if (QUEUE_WRITE_FULL_EN > 0)
			if(Queue->WriteFull != NULL){				// �����û���������
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
** ��������: QueueWriteFront
** ��������: LIFO��ʽ��������
** �䡡��: Buf:ָ����е�ָ��
**         Data:��Ϣ����
** �䡡��: QUEUE_FULL:������
**         QUEUE_OK:���ͳɹ�
** ȫ�ֱ���: ��
** ����ģ��: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
**
** ������: ������
** �ա���: 2003��7��2��
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
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
    if (Buf != NULL)                                                    /* �����Ƿ���Ч */
    {
        Queue = (T_DataQueue *)Buf;
        
        QUE_OS_ENTER_CRITICAL();
        
        if (Queue->NData < Queue->MaxData)                              /* �����Ƿ���  */
        {                                                               /* ���� */
            Queue->Out--;                                               /* ��������ָ�� */
            if (Queue->Out < Queue->Buf)
            {
                Queue->Out = Queue->End - 1;
            }
            Queue->Out[0] = Data;                                       /* �������     */
            Queue->NData++;                                             /* ������Ŀ���� */
            err = QUEUE_OK;
        }
        else
        {                                                               /* ��           */
            err = QUEUE_FULL;
            
			#if (QUEUE_WRITE_FULL_EN > 0)
            if (Queue->WriteFull != NULL)                               /* �����û��������� */
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
** ��������: QueueNData
** ��������: ȡ�ö�����������
** �䡡��: Buf:ָ����е�ָ��
** �䡡��: ��Ϣ��
** ȫ�ֱ���: ��
** ����ģ��: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
**
** ������: ������
** �ա���: 2003��7��2��
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#ifndef EN_QUEUE_NDATA
#define EN_QUEUE_NDATA    0
#endif
#if EN_QUEUE_NDATA > 0
       uint16_t QueueNData(void *Buf)
{
    uint16_t temp;
    
    temp = 0;                                                   /* ������Ч����0 */
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
** ��������: QueueSize
** ��������: ȡ�ö���������
** �䡡��: Buf:ָ����е�ָ��
** �䡡��: ����������
** ȫ�ֱ���: ��
** ����ģ��: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
**
** ������: ������
** �ա���: 2003��7��2��
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#ifndef EN_QUEUE_SIZE
#define EN_QUEUE_SIZE    0
#endif

#if EN_QUEUE_SIZE > 0

        uint16_t QueueSize(void *Buf)
{
    uint16_t temp;
    
    temp = 0;                                                   /* ������Ч����0 */
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
** ��������: OSQFlush
** ��������: ��ն���
** �䡡��: Buf:ָ����е�ָ��
** �䡡��: ��
** ȫ�ֱ���: ��
** ����ģ��: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
**
** ������: ������
** �ա���: 2003��7��2��
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
#ifndef EN_QUEUE_FLUSH
#define EN_QUEUE_FLUSH    0
#endif

#if EN_QUEUE_FLUSH > 0

        void QueueFlush(void *Buf)
{
    T_DataQueue *Queue;
    
    if (Buf != NULL)                                                /* �����Ƿ���Ч */
    {                                                               /* ��Ч         */
        Queue = (T_DataQueue *)Buf;
        QUE_OS_ENTER_CRITICAL();
        Queue->Out = Queue->Buf;
        Queue->In = Queue->Buf;
        Queue->NData = 0;                                           /* ������ĿΪ0 */
        QUE_OS_EXIT_CRITICAL();
    }
}

#endif

/*********************************************************************************************************
** ��������: QueueOutN
** ��������: ���г���N����Ԫ, ֱ�ӵ���ָ��
** �䡡��: Buf:ָ����е�ָ��
** �䡡��: ��
** ȫ�ֱ���: ��
** ����ģ��: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
**
** ������: Xsky
** �ա���: 2011-04-02 15:16
**-------------------------------------------------------------------------------------------------------
** �޸���:
** �ա���:
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
    
    if (Buf != NULL)                                                /* �����Ƿ���Ч */
    {                                                               /* ��Ч         */
        Queue = (T_DataQueue *)Buf;
        QUE_OS_ENTER_CRITICAL();
        N0 = Queue->NData;
        if(N > N0) N = N0;
        Queue->NData = N0 - N;                                      /* ������ĿΪ0 */
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