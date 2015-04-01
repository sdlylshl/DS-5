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
#include "..\TCPIP\Netif.h"
#include "..\TCPIP\TCP.h"

/* Check sum calulation. data in buff, size, InSum is initial sum */
WORD CheckSum(WORD DT_XDATA * buff,WORD size,DWORD InSum) REENTRANT_SIG
{
	/* TO DO:in packet memory high part of short is in low memory. add all data in 
	form of 16 bits get a result of 32 bits, add high 16 bits to low 16 bits two 
	times.  get a 16 bits result then complement it. */

	DWORD cksum = InSum; 

	/* sum all word except the last odd byte(if size is a odd num) */
	WORD DT_XDATA * EndBuf = buff + size/2;
	while(buff < EndBuf)
	{ 
		/* net order is equeal as host order in mirochip, so no need to change */
		cksum += *(buff++); 
	} 
	
	/**((WORD xdata *)CheckSumInParam) = size;
	*((WORD xdata *)(CheckSumInParam+2)) = buff;
	asmAddCheckSum();
	cksum = CheckSumOutParm;
	*/
	
	/* if has last odd byte. use this byte as the high part of 16 bits, and add. */
	if((size & 0x0001) != 0) 
		cksum += (*buff) & 0xff00;

	cksum = (cksum >> 16) + (cksum & 0xffff); 
	cksum += (cksum >>16); 
	return (WORD)(~cksum); 
}

/* IP input process */
void IPInput(struct SMemHead DT_XDATA *MemHead) REENTRANT_MUL
{
	struct SIPHead DT_XDATA *pIPHead;
	struct SNetIf  DT_XDATA *pNetIf;		/* for search netif list */

	pIPHead = (struct SIPHead DT_XDATA *)(MemHead->pStart);

	/* check ip version */
	if(IP_VERSION(pIPHead) != IP_VERSION_4)
	{
		MemFree(MemHead);
		return;
	}

	/* if checksum is ok */
	if(CheckSum((WORD DT_XDATA *)pIPHead,(WORD)IP_HEAD_LEN(pIPHead),0) != 0)
	{
		MemFree(MemHead);
		return;
	}

	/* ip packet with options is not supported */
	if(IP_HEAD_LEN(pIPHead) != IP_HEAD_MIN_LEN)
	{
		MemFree(MemHead);
		return;
	}

	/* ip packet fragmented is not supported */
	if((pIPHead->FragmentFlag_Offset & IP_FRAGMENT_OFFSET_MASK)!= 0)
	{
		MemFree(MemHead);
		return;
	}

	
	/* if this packet for us. check all the netif. if a host
	has tow device(tow ip). This packet may come from one device
	but send for the IP of the other deviec. In this case we should
	not drop or forward this packet */
	
	/* if this packet is not for us. forward it */
	if((pNetIf = NetIfFindIP(pIPHead->IPDest)) == NULL)
	{
		#ifdef IP_ENABLE_FORWARD	/* if act as a router */
		/* We should decrease the IPHead->ttl */
		if(pIPHead->LifeLength != 0)
		{
			pIPHead->LifeLength--;
				
			/* recaculate IP head checksum. there is a easy method
			to recaculate, leave for later version improvment */
			CheckSum((WORD DT_XDATA *)pIPHead,(WORD)IP_HEAD_LEN(pIPHead),0);

			/* find a rout( a interface ) */
			if((pNetIf = NetIfFindRout(pIPHead->IPDest)) != NULL)
			{
				/* forward. send it through this interface. if return FALSE, we
				do not care, the soure of the packet will deel with it. */
				pNetIf->output(MemHead,pNetIf,pIPHead->IPDest);
			}
		}
		#endif
		
		MemFree(MemHead);
		return;
	}
	else
	{
		/* MemHead->pStart set to point uper layer */
		MemHead->pStart += sizeof(struct SIPHead);

		/* pass to the uper layer */
		switch(pIPHead->Protocol)
		{
		case IP_PROTOCOL_TCP:
			TCPInput(MemHead);
			break;
#if	ICMP_EN
		case IP_PROTOCOL_ICMP:
			ICMPInput(MemHead);
			break;
#endif
		default:
			MemFree(MemHead);
		}
	}
}

/* out put a ip packet,NOTE:MemHead->pStart point to IPHead.
IPScr IPDest Protocol TotalLen is already filled at uper layer.
To do so TCPCheckSum is easy to generate and pass augument to 
IPOutput is easyer. 
return :
	TURE: send the packt successful. */
BOOL IPOutput(struct SMemHead DT_XDATA * MemHead) REENTRANT_SIG
{
	struct SNetIf  DT_XDATA *pNetIf;
	struct SIPHead DT_XDATA *pIPHead;
	WORD tCheckSum;
	
	pIPHead = (struct SIPHead DT_XDATA *)(MemHead->pStart);

	/* found a rout */
	if((pNetIf = NetIfFindRout(pIPHead->IPDest)) != NULL)
	{
		/* fill IP head */
		pIPHead->CheckSum				= 0;
		pIPHead->FragmentFlag_Offset	= 0;
		pIPHead->FragmentID				= 0;
		pIPHead->LifeLength				= IP_INITIAL_LIFE;
		pIPHead->ServeType				= 0;
		pIPHead->Ver_HeadLen			= (IP_VERSION_4 << 4) + IP_HEAD_MIN_LEN/4;

		/* checksum */
		tCheckSum = CheckSum((WORD DT_XDATA *)pIPHead,(WORD)IP_HEAD_LEN(pIPHead),0);
		pIPHead->CheckSum = htons(tCheckSum);

		/* output it */
		return pNetIf->output(MemHead,pNetIf,pIPHead->IPDest);
	}
	else
		return FALSE;
	/* 'MemHead' freeing is at tcp model when it is acked */
}
