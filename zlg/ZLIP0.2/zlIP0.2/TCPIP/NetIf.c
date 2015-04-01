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
#include "..\TCPIP\Netif.h"

/* You can get a new netif from NetIfPool */
static struct SNetIf DT_XDATA NetIfPool[NET_IF_MAX_NUM];

/* list hearder of free Netifs buffer */
static struct SNetIf DT_XDATA * DT_XDATA NetIfFreeList;

/* list header of Netifs */
static struct SNetIf DT_XDATA * DT_XDATA NetIfList;

void NetIfInit() REENTRANT_MUL
{
	WORD count;

	/* inti NetIfList */
	NetIfList = NULL;

	/* inti FreeList, chain up NetIfPool */
	for(count = 0, NetIfFreeList = NULL; count < NET_IF_MAX_NUM; count++)
	{
		NetIfPool[count].pNext = NetIfFreeList;
		NetIfFreeList = &NetIfPool[count];
	}
}

/* add a netif to list, return NULL if insufficient mem */
struct SNetIf DT_XDATA * NetIfAdd(DWORD IPAddr, DWORD NetMask,DWORD GateWay,
						 void (DT_CODE * input)(struct SNetIf DT_XDATA * NetIf) REENTRANT_SIG,
						 BOOL (DT_CODE * output)(struct SMemHead DT_XDATA *MemHead,struct SNetIf DT_XDATA* NetIf,DWORD DestIP) REENTRANT_SIG,
						 void DT_XDATA * Info) REENTRANT_MUL
{
	struct SNetIf DT_XDATA * NetIf;

	/* get netif from free list */
	NetIf = NetIfFreeList;
	NetIfFreeList = NetIfFreeList->pNext;

	/* if add more than NET_IF_MAX_NUM return NULL */
	if(NetIf == NULL)
		return NULL;
	else
	{
		NetIf->IPAddr	= htonl(IPAddr);
		NetIf->NetMask	= htonl(NetMask);
		NetIf->GateWay	= htonl(GateWay);
		NetIf->input	= input;
		NetIf->output	= output;
		NetIf->Info		= Info;

		/* add to the head of the list */
		NetIf->pNext = NetIfList;
		NetIfList = NetIf;

		return NetIf;
	}
}

/* find a netif which NetIf->NetMask & NetIf->NetAddr == 
NetIf->NetMask & IPAddr */
struct SNetIf DT_XDATA * NetIfFindRout(IP_ADDR IPAddr) REENTRANT_SIG
{
	struct SNetIf DT_XDATA *NetIf;
	for(NetIf = NetIfList; NetIf != NULL; NetIf = NetIf->pNext)
	{
		if((NetIf->NetMask & NetIf->IPAddr) == (NetIf->NetMask & IPAddr))
			return NetIf;
	}

	/* if can't find any suitable Netif, return NetIfList. That is the 
	first netif is the default Netif*/
	return NetIfList;	
}

struct SNetIf DT_XDATA * NetIfFindIP(IP_ADDR IPAddr) REENTRANT_MUL
{
	struct SNetIf DT_XDATA *pNetIf;
	for(pNetIf = NetIfList; pNetIf != NULL; pNetIf = pNetIf->pNext)
	{
		if(pNetIf->IPAddr == IPAddr)
			break;
	}
	return pNetIf;
}

/* net inteftimer. use to packup packet from every net interface */
void NetIfTimer() REENTRANT_MUL
{
	struct SNetIf DT_XDATA * NetIf;

	/* if netif has data to come in */
	for(NetIf = NetIfList; NetIf != NULL; NetIf = NetIf->pNext)
		NetIf->input(NetIf);
}
