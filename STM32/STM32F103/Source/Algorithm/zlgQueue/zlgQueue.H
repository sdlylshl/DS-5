/****************************************Copyright (c)**************************************************
**                               ������������Ƭ����չ���޹�˾
**                                     ��    ��    ��
**                                        ��Ʒһ�� 
**
**                                 http://www.zlgmcu.com
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**��   ��   ��: queue.h
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
** �衡��: ����ע�ʹ���
**
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/

#include "stdint.h"
#include "stddef.h"
#ifndef NOT_OK
#define NOT_OK              0xff                        /* ��������                                     */
#endif

#define QUEUE_FULL          8                           /* ������                                       */
#define QUEUE_EMPTY         4                           /* ������                                       */
#define QUEUE_OK            0                           /* �����ɹ�                                     */

#define Q_WRITE_MODE        1                           /* �����ɹ�                                     */
#define Q_WRITE_FRONT_MODE  2                           /* �����ɹ�                                     */

#ifndef QUEUE_DATA_TYPE
#define QUEUE_DATA_TYPE     uint8_t
#endif

#ifndef QUEUE_READ_EMPTY_EN		
#define QUEUE_READ_EMPTY_EN		0
#endif
#ifndef QUEUE_WRITE_FULL_EN		
#define QUEUE_WRITE_FULL_EN		0
#endif

#define EN_QUEUE_READ_BYTES				1	// ��ֹ(0)����(1)FIFO���ն��ֽ�����
#define EN_QUEUE_WRITE            		0     /* ��ֹ(0)������(1)FIFO��������       */
#define EN_QUEUE_WRITE_BYTES         	1     /* ��ֹ(0)������(1)FIFO���Ͷ��ֽ�����       */
//#define EN_QUEUE_WRITE_FRONT      	0     /* ��ֹ(0)������(1)LIFO��������       */
#define EN_QUEUE_NDATA            		0     /* ��ֹ(0)������(1)ȡ�ö���������Ŀ   */
//#define EN_QUEUE_SIZE            		0     /* ��ֹ(0)������(1)ȡ�ö������������� */
#define EN_QUEUE_FLUSH           		1     /* ��ֹ(0)������(1)��ն���           */
#define EN_QUEUE_OUT_N					1	  /* ��ֹ(0)������(1)����ֱ�ӳ���N����Ԫ */

#define QUEUE_CRITICAL_EN		0	// ����д���Ƿ������ж�

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
#define T_QUEUE_DATA_SIZE	uint16_t
#endif
typedef struct {
	// �����Խ�In, Out, ����Ϊ������ʽ�ȶ���Ϊָ��ִ��Ч�ʵ�
	QUEUE_DATA_TYPE     *Out;                   /* ָ���������λ��         */
	QUEUE_DATA_TYPE     *In;                    /* ָ����������λ��         */
	QUEUE_DATA_TYPE     *End;                   /* ָ��Buf�Ľ���λ��        */
	uint16_t              NData;                  /* ���������ݸ���,��QUEUE_DATA_TYPEΪ��λ���� */
	uint16_t              MaxData;                /* �����������洢�����ݸ��� */
#if (QUEUE_READ_EMPTY_EN > 0)
	uint8_t               (* ReadEmpty)(void);        /* ���մ�������             */
#endif
#if (QUEUE_WRITE_FULL_EN > 0)
	uint8_t               (* WriteFull)(void);        /* д����������             */
#endif
	QUEUE_DATA_TYPE     Buf[1];                 /* �洢���ݵĿռ�           */
} T_DataQueue;


#ifndef IN_QUEUE
        uint8_t QueueCreate(void *Buf,
                          T_QUEUE_DATA_SIZE SizeOfBuf
#if (QUEUE_READ_EMPTY_EN > 0)
                          , uint8_t (* ReadEmpty)(void)
#endif
#if (QUEUE_WRITE_FULL_EN > 0)
                          ,uint8_t (* WriteFull)(void)
#endif
                          );
/*********************************************************************************************************
** ��������: QueueCreate
** ��������: ��ʼ�����ݶ���
** �䡡��: Buf      ��Ϊ���з���Ĵ洢�ռ��ַ
**         SizeOfBuf��Ϊ���з���Ĵ洢�ռ��С���ֽڣ�
**         ReadEmpty��Ϊ���ж���ʱ��������
**         WriteFull��Ϊ����д��ʱ��������
** �䡡��: NOT_OK  :��������
**         QUEUE_OK:�ɹ�
** ȫ�ֱ���: ��
** ����ģ��: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
********************************************************************************************************/
#define QUEUE_READ_ISR_QUICK(Data, Queue)	\
	do{\
		Data = *(Queue->Out);\
		Queue->Out++;                                       /* ��������ָ�� */\
		if (Queue->Out >= Queue->End)\
		{\
			Queue->Out = Queue->Buf;\
		}\
		Queue->NData--;                                     /* ���ݼ���      */\
	}while(0)
#define QUEUE_READ_QUICK(Data, Queue)	\
	do{\
		QUE_OS_ENTER_CRITICAL();\
		if (Queue->NData > 0)                                  	/* �����Ƿ�Ϊ�� */ \
		{                                                      			/* ����         */\
			Data = *(Queue->Out);\
			Queue->Out++;                                       /* ��������ָ�� */\
			if (Queue->Out >= Queue->End)\
			{\
				Queue->Out = Queue->Buf;\
			}\
			Queue->NData--;                                     /* ���ݼ���      */\
		}\
		QUE_OS_EXIT_CRITICAL();\
	}while(0)
#define QUEUE_READ_QUICK_CRITICAL(Data, Queue, Enter, Exit)	\
	do{\
		Enter;\
		if (Queue->NData > 0)									/* �����Ƿ�Ϊ�� */ \
		{																/* ���� 		*/\
			Data = *(Queue->Out);\
			Queue->Out++;										/* ��������ָ�� */\
			if (Queue->Out >= Queue->End)\
			{\
				Queue->Out = Queue->Buf;\
			}\
			Queue->NData--; 									/* ���ݼ��� 	 */\
		}\
		Exit;\
	}while(0)
#define QUEUE_READ(Data, Queue, err)	\
	do{\
		QUE_OS_ENTER_CRITICAL();\
		if (Queue->NData > 0)                                  	/* �����Ƿ�Ϊ�� */ \
		{                                                      			/* ����         */\
			Data = *(Queue->Out);\
			Queue->Out++;                                       /* ��������ָ�� */\
			if (Queue->Out >= Queue->End)\
			{\
				Queue->Out = Queue->Buf;\
			}\
			Queue->NData--;                                     /* ���ݼ���      */\
			err = QUEUE_OK;\
		}\
		else\
		{                                                       /* ��              */\
			err = QUEUE_EMPTY;\
		}\
		QUE_OS_EXIT_CRITICAL();\
	}while(0)
#define QUEUE_READ_CRITECAL(Data, Queue, err)	\
	do{\
		OS_ENTER_CRITICAL();\
		if (Queue->NData > 0)                                  	/* �����Ƿ�Ϊ�� */ \
		{                                                      			/* ����         */\
			Data = *(Queue->Out);\
			Queue->Out++;                                       /* ��������ָ�� */\
			if (Queue->Out >= Queue->End)\
			{\
				Queue->Out = Queue->Buf;\
			}\
			Queue->NData--;                                     /* ���ݼ���      */\
			err = QUEUE_OK;\
		}\
		else\
		{                                                       /* ��              */\
			err = QUEUE_EMPTY;\
		}\
		OS_EXIT_CRITICAL();\
	}while(0)
extern uint16_t QueueRead(/*QUEUE_DATA_TYPE *Ret, */void *Buf);
/*********************************************************************************************************
** ��������: QueueRead
** ��������: ��ȡ�����е�����
** �䡡��: Ret:�洢���ص���Ϣ�ĵ�ַ
**         Buf:ָ����е�ָ��
** �䡡��: NOT_OK     ����������
**         QUEUE_OK   ���յ���Ϣ
**         QUEUE_EMPTY�����п�
** ȫ�ֱ���: ��
** ����ģ��: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
********************************************************************************************************/

extern uint16_t QueueReadBytes(QUEUE_DATA_TYPE *DstBuf, uint16_t NData, void *Buf);
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
********************************************************************************************************/
#define QUEUE_WRITE_ISR_QUICK(Queue, Data)	\
	do{\
		if (Queue->NData < Queue->MaxData)			/* �����Ƿ���  */ \
		{                                                               		/* ����        */ \
			Queue->In[0] = Data;                                  	/* �������    */ \
			Queue->In++;                                			/* �������ָ��*/ \
			if (Queue->In >= Queue->End) \
			{ \
				Queue->In = Queue->Buf; \
			}\
			Queue->NData++;                               		/* ��������    */ \
		}\
	}while(0)

static __inline void QueueWriteQuickIsr(register T_DataQueue *Queue, QUEUE_DATA_TYPE Data)	
{
	if (Queue->NData < Queue->MaxData)							/* �����Ƿ���  	*/ 
	{															/* ���� 	   	*/ 
		Queue->In[0] = Data;									/* �������    	*/ 
		Queue->In++;											/* �������ָ��	*/ 
		if (Queue->In >= Queue->End) 
		{ 
			Queue->In = Queue->Buf; 
		}
		Queue->NData++; 										/* ��������    	*/ 
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
		if (Queue->NData < Queue->MaxData)							/* �����Ƿ���  */ \
		{                                                               		/* ����        */ \
			Queue->In[0] = (Data);                                	/* �������    */ \
			Queue->In++;                                			/* �������ָ��*/ \
			if (Queue->In >= Queue->End) \
			{ \
				Queue->In = Queue->Buf; \
			}\
			Queue->NData++;                               		/* ��������    */ \
		}\
		QUE_OS_EXIT_CRITICAL(); \
	}while(0)
#define QUEUE_WRITE_QUICK_CRITICAL(Queue, Data, Enter, Exit)	/* ʹ���Զ��忪���жϺ��� */ \
	do{\
		Enter; \
		if (Queue->NData < Queue->MaxData)						/* �����Ƿ���  */ \
		{																		/* ���� 	   */ \
			Queue->In[0] = Data;									/* �������    */ \
			Queue->In++;											/* �������ָ��*/ \
			if (Queue->In >= Queue->End) \
			{ \
				Queue->In = Queue->Buf; \
			}\
			Queue->NData++; 									/* ��������    */ \
		}\
		Exit; \
	}while(0)
#define QUEUE_WRITE(Queue, Data, err)	\
	do{\
		QUE_OS_ENTER_CRITICAL(); \
		if (Queue->NData < Queue->MaxData)			/* �����Ƿ���  */ \
		{                                                               		/* ����        */ \
			Queue->In[0] = Data;                                  	/* �������    */ \
			Queue->In++;                                			/* �������ָ��*/ \
			if (Queue->In >= Queue->End) \
			{ \
				Queue->In = Queue->Buf; \
			}\
			Queue->NData++;                               		/* ��������    */ \
			err = QUEUE_OK;\
		}\
		else{ err = QUEUE_FULL; } \
		QUE_OS_EXIT_CRITICAL(); \
	}while(0)
#define QUEUE_WRITE_CRITICAL(Queue, Data, err)	\
	do{\
		if (Queue->NData < Queue->MaxData)			/* �����Ƿ���  */ \
		{                                                               		/* ����        */ \
			OS_ENTER_CRITICAL(); \
			Queue->In[0] = Data;                                  	/* �������    */ \
			Queue->In++;                                			/* �������ָ��*/ \
			if (Queue->In >= Queue->End) \
			{ \
				Queue->In = Queue->Buf; \
			}\
			Queue->NData++;                               		/* ��������    */ \
			OS_EXIT_CRITICAL(); \
			err = QUEUE_OK;\
		}\
		else{ err = QUEUE_FULL; } \
	}while(0)
extern uint8_t QueueWrite(void *Buf, QUEUE_DATA_TYPE Data);
/*********************************************************************************************************
** ��������: QueueWrite
** ��������: FIFO��ʽ��������
** �䡡��: Buf :ָ����е�ָ��
**         Data:���͵�����
** �䡡��: NOT_OK   ����������
**         QUEUE_FULL:������
**         QUEUE_OK  :���ͳɹ�
** ȫ�ֱ���: ��
** ����ģ��: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
********************************************************************************************************/

extern uint8_t QueueWriteBytes(void *Buf, QUEUE_DATA_TYPE *pSrc, uint16_t NData);
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


extern uint8_t QueueWriteFront(void *Buf, QUEUE_DATA_TYPE Data);
/*********************************************************************************************************
** ��������: QueueWrite
** ��������: LIFO��ʽ��������
** �䡡��: Buf:ָ����е�ָ��
**         Data:��Ϣ����
** �䡡��: QUEUE_FULL:������
**         QUEUE_OK:���ͳɹ�
** ȫ�ֱ���: ��
** ����ģ��: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
********************************************************************************************************/

#define QUEUE_GET_N_DATA(Buf)	(((T_DataQueue *)Buf)->NData)

#if (EN_QUEUE_NDATA > 0)
extern uint16_t QueueNData(void *Buf);
#endif
/*********************************************************************************************************
** ��������: QueueNData
** ��������: ȡ�ö�����������
** �䡡��: Buf:ָ����е�ָ��
** �䡡��: ��Ϣ��
** ȫ�ֱ���: ��
** ����ģ��: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
********************************************************************************************************/

extern uint16_t QueueSize(void *Buf);
/*********************************************************************************************************
** ��������: QueueSize
** ��������: ȡ�ö���������
** �䡡��: Buf:ָ����е�ָ��
** �䡡��: ����������
** ȫ�ֱ���: ��
** ����ģ��: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
********************************************************************************************************/

extern void QueueFlush(void *Buf);
/*********************************************************************************************************
** ��������: OSQFlush
** ��������: ��ն���
** �䡡��: Buf:ָ����е�ָ��
** �䡡��: ��
** ȫ�ֱ���: ��
** ����ģ��: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
********************************************************************************************************/


extern void QueueOutN(void *Buf, uint32_t N);
/*********************************************************************************************************
** ��������: QueueOutN
** ��������: ���г���N����Ԫ, ֱ�ӵ���ָ��
** �䡡��: Buf:ָ����е�ָ��
** �䡡��: ��
** ȫ�ֱ���: ��
** ����ģ��: OS_ENTER_CRITICAL,OS_EXIT_CRITICAL
********************************************************************************************************/

#endif		// end: #ifndef IN_QUEUE //

/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/