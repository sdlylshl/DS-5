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

/* IP */
//#define IP_ENABLE_FORWARD	/* enable packet come in an forward act as a router */
#define IP_VERSION_4	4

#define IP_HEAD_MIN_LEN	20	/* ip head len with no option */

#define IP_INITIAL_LIFE 20

#define IP_PROTOCOL_TCP		0x06	
#define IP_PROTOCOL_ICMP	0x01

/* caculate ip version of a packet */
#define IP_VERSION(pIPHead) (pIPHead->Ver_HeadLen>>4)

/* caculate ip head len of a packet.head len in Ver_HeadLen 
is mulitple of 4 byte */
#define IP_HEAD_LEN(pIPHead) ((pIPHead->Ver_HeadLen & 0x0f)*4)

typedef DWORD IP_ADDR;

struct SIPHead
{
	/* Version 4 bits, HeadLength 4 bits. typical value is 0x45 */
	BYTE Ver_HeadLen;	

	/* Precedence(priority) 3 bits, Delay, Throughput, Reliability
	, reserved 2 bits. typical value 0x00 */
	BYTE ServeType;	

	WORD TotalLen;				/* all size of IP packet inlcude IPHead. 16 bits */
	WORD FragmentID;			/* 16 bits */

	/* Reserved 1 bit, May be fragmented 1 bit, Last fragment 1 bit, 
	Fragment offset 13 bits. typical 0x00 */
#define	IP_FRAGMENT_OFFSET_MASK	0x1FFF
	WORD FragmentFlag_Offset;	

	BYTE LifeLength;			/* ttl */
	BYTE Protocol;				/* eg. IP_PROTOCAL_TCP*/
	WORD CheckSum;				/* 16 bits */
	IP_ADDR IPScr;				/* 32 bits */
	IP_ADDR IPDest;
};

WORD CheckSum(WORD DT_XDATA * buff,WORD size,DWORD InSum) REENTRANT_SIG;
void IPInput(struct SMemHead DT_XDATA *MemHead) REENTRANT_MUL;
BOOL IPOutput(struct SMemHead DT_XDATA * MemHead) REENTRANT_SIG;


