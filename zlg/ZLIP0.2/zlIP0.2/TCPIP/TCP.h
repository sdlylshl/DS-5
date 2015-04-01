/*
 * Copyright (c) 2003 Electric Application Laboratory of NAN KAI University
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * Author: Li Zhanglin <wzzlin@nankai.edu.cn>
 *
 */

/* tcp */

#define TCP_SEQ_COMPARE(seq1,seq2)	(SDWORD)(seq1 - seq2)	/* the compare will be less than 0, so set it as singed */
#define TCP_HEAD_LEN(pTCPHead)		(((pTCPHead->TCPHeadLen & 0xf0)>>4)*4)
#define TCP_DEFAULT_PORT			1001
#define TCP_CONNECTION_MAX_NUM		10
#define TCP_QUEUE_MAX_NUM			100
#define TCP_HEAD_MIN_LEN			20
#define TCP_RETRAN_MAX_TIME			6
#define TCP_RETRAN_TIME_OUT			20
#define TCP_DELAY_ACK_TIME_OUT		1
#define TCP_LASTACK_TIME_OUT		40

#define ALL_HEAD_SIZE	(TCP_HEAD_MIN_LEN + IP_HEAD_MIN_LEN + NETIF_HEAD_MAX_LEN)
/* tcp flag */
#define TCP_URG 0x20
#define TCP_ACK 0x10
#define TCP_PSH 0x08
#define TCP_RST 0x04
#define TCP_SYN 0x02
#define TCP_FIN 0x01

/* tcp state */
#define TCP_STATE_CLOSED	0
#define TCP_STATE_LASTACK	1
#define	TCP_STATE_TIMEWAIT	2
#define TCP_STATE_LISTEN	3
#define TCP_STATE_SYNRECVD	4
#define TCP_STATE_SYNSENT	5
#define TCP_STATE_ESTABLISHED	6
#define TCP_STATE_CLOSEWAIT	7
#define TCP_STATE_FINWAIT1	8
#define TCP_STATE_CLOSING	9
#define TCP_STATE_FINWAIT2	10


/* tcp header */
struct STCPHead
{
	WORD PortScr;	/* 16 bits */
	WORD PortDest;
	DWORD Seq;
	DWORD AckSeq;
	BYTE TCPHeadLen;	/* 8 bits, use high 4 bits */
	BYTE flag;			/* reserve 2 bits, urg, ack, psh, rst, syn, fin */
	WORD WndSize;		/* 16 bits */
	WORD CheckSum;		/* 16 bits */
	WORD UrgentPoint;	/* 16 bits */
}; 

/* tcb (transmit control block). used to keep tcp state 
information */
typedef struct STCB
{
	struct STCB DT_XDATA *pNext;   /* Next Node */

	BYTE	TCPState;

	/* conection idetifies */
	WORD	PortScr;
	WORD	PortDest;
	IP_ADDR	IPScr;
	IP_ADDR IPDest;

	DWORD	SeqMine;
	DWORD	SeqHis;		/* the sequence want to receive from
						also is AckSeq in out going packet */

	WORD	WndMine;
	WORD	WndHis;
	
	/* queues */
	struct SPacketQueue DT_XDATA *QUnacked;		/* unacked packet queue */
	struct SPacketQueue DT_XDATA *QUnSend;		/* packet need to be send */
	struct SPacketQueue DT_XDATA *QExceedSeq;	/* receive packet whoes 
										seq exceed the SeqHis */

	WORD	RetranTimer;			/* if it hit 0, indicate the 
									first unit in QUnacked is time 
									out */
	BYTE	RetranTimes;			/* retransmit time of a same 
									packet, if it exceed a sreshold
									. the connection go to colse */
	BOOL	bNeedAck;				/* we need send ack to him */
	WORD	DelayAckTimer;			/* if bNeedAck == TRUE, we delaye
									for DELAY_ACK_TIME_OUT to send ack
									*/
	WORD	LastAckTimer;			/* when in TCP_STATE_LASTACK a timer
									is started. when it out, transfer to 
									TCP_CLOSED state */
	/* call back functions */
	void (DT_CODE * accept)(struct STCB DT_XDATA *pNewTCB) REENTRANT_MUL;
	void (DT_CODE * recv)(void DT_XDATA * buf,WORD size) REENTRANT_MUL;
	void (DT_CODE * close)(struct STCB DT_XDATA * pSocket) REENTRANT_MUL;				/* when a opened connection closed */
}socket;

/* tcp queue */
struct SPacketQueue
{
	struct SPacketQueue DT_XDATA *pNext;
	struct SPacketQueue DT_XDATA *pPre;
	DWORD	Seq;				/* sequence of this packet */
	struct SMemHead DT_XDATA *MemHead;	/* packet store in MemHead' playload */
};

struct STCB DT_XDATA *TCPGetTCB() REENTRANT_SIG;
void TCPInsertTCB(struct STCB DT_XDATA *pTCB) REENTRANT_SIG;
void TCPAbort(struct STCB DT_XDATA *pTCB) REENTRANT_SIG;
struct SPacketQueue DT_XDATA * TCPGetQ() REENTRANT_SIG;
BOOL TCPInsertQ(struct SPacketQueue DT_XDATA * DT_XDATA *ppQ,struct SMemHead DT_XDATA *MemHead,
				DWORD Seq) REENTRANT_SIG;
struct SPacketQueue DT_XDATA * TCPOutQ(struct SPacketQueue DT_XDATA * DT_XDATA *ppQ) REENTRANT_SIG;

void TCPInit() REENTRANT_MUL;
WORD TCPCheckSum(struct SIPHead DT_XDATA * pIPHead,WORD TCPSize) REENTRANT_SIG;
void TCPTimer() REENTRANT_MUL;
void TCPRelease(struct STCB DT_XDATA *pTCB) REENTRANT_SIG;
BOOL TCPSendSeg(struct STCB DT_XDATA *pTCB,struct SMemHead DT_XDATA *MemHead,BYTE TCPFlag) REENTRANT_SIG;
BOOL TCPSendSegJudgeWnd(struct STCB DT_XDATA *pTCB,struct SMemHead DT_XDATA *MemHead) REENTRANT_MUL;
BOOL TCPSendUnsendQ(struct STCB DT_XDATA *pTCB) REENTRANT_MUL;
void TCPRecvSeg(struct STCB DT_XDATA *pTCB,struct SMemHead DT_XDATA *MemHead) REENTRANT_SIG;
void TCPInput(struct SMemHead DT_XDATA *MemHead) REENTRANT_SIG;

BOOL TCPSendEx(struct STCB DT_XDATA * pTCB,struct SMemHead DT_XDATA *MemHead) REENTRANT_MUL;
BOOL TCPSend(struct STCB DT_XDATA * pTCB,void DT_XDATA *buf,WORD DataSize) REENTRANT_MUL;
BOOL TCPConnect(struct STCB DT_XDATA * pTCB, DWORD DestIP, WORD DestPort,						
						void (DT_CODE * recv)(void DT_XDATA * buf,WORD size) REENTRANT_MUL,
						void (DT_CODE * close)(struct STCB DT_XDATA * pSocket) REENTRANT_MUL) REENTRANT_SIG;
void TCPClose(struct STCB DT_XDATA *pTCB) REENTRANT_MUL;
BOOL TCPListen(struct STCB DT_XDATA *pTCB,WORD ScrPort,void (DT_CODE * accept)(struct STCB DT_XDATA *pNewTCB) REENTRANT_MUL) REENTRANT_MUL;
struct STCB DT_XDATA * TCPSocket(IP_ADDR ScrIP) REENTRANT_SIG;
struct SMemHead DT_XDATA *TCPAllocate(WORD size) REENTRANT_SIG;














