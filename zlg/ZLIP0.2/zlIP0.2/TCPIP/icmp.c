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

#include "..\GloblDef\GloblDef.h"
#include "..\TCPIP\TCPIPmem.h"
#include "..\TCPIP\IP.h"
#include "..\TCPIP\icmp.h"

#if ICMP_EN
/* MemHead->pStart is point to ICMP head */
void ICMPInput(struct SMemHead DT_XDATA *MemHead)	REENTRANT_MUL
{
	IP_ADDR ipaddr;
	struct SIPHead DT_XDATA *pIPHead;

	/* which type of icmp */
	switch(((struct SICMPEchoHead DT_XDATA *)(MemHead->pStart))->type)
	{
	case ICMP_TYPE_QUERY:
		
		/* chage type */
		((struct SICMPEchoHead DT_XDATA *)(MemHead->pStart))->type = ICMP_TYPE_REPLY;

		/* adjust checksum. refer to lwip: if change type from 8 to 0,
		for checksum, that is increasing 0x8000 and add flowed hight bit 
		to bit 0.*/
		if (((struct SICMPEchoHead DT_XDATA *)(MemHead->pStart))->CheckSum >= htons(0xffff - (((WORD)ICMP_TYPE_QUERY) << 8))) 
			((struct SICMPEchoHead DT_XDATA *)(MemHead->pStart))->CheckSum += htons(((WORD)ICMP_TYPE_QUERY) << 8) + 1;
		else
			((struct SICMPEchoHead DT_XDATA *)(MemHead->pStart))->CheckSum += htons(((WORD)ICMP_TYPE_QUERY) << 8);

		/*
		 *send this packet back, first fill some of IPHead field 
		 */

		/* dec pStart and get ip head */
		pIPHead = (struct SIPHead DT_XDATA *)(MemHead->pStart -= IP_HEAD_MIN_LEN);

		/* exchange ipdest and ipscr */
		ipaddr = pIPHead->IPDest;
		pIPHead->IPDest = pIPHead->IPScr;
		pIPHead->IPScr  = ipaddr;

		/* ip total length not change */

		/* protocol */
		pIPHead->Protocol = IP_PROTOCOL_ICMP;

		IPOutput(MemHead);

		/* whether send success or not free this packet */
		MemFree(MemHead);
		
		break;
	default:
		MemFree(MemHead);
	}
}

#endif