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
#include "..\Netif\etherif.h"
#include "..\Netif\ARP.h"
#include "..\TCPIP\Netif.h"


/* call output to put a packet from IP layer to device. After
	Ip layer selected a device, it use output to send this packet.
	MemHead contain a packet and Netif tell dirver which netif it 
	is. NOTE:MemHead->pStart point to pIPHead
return:
	TRUE: send successfuly.*/
BOOL EtherOutput(struct SMemHead DT_XDATA *MemHead,struct SNetIf DT_XDATA* NetIf, 
		IP_ADDR DestIP) REENTRANT_SIG
{
	DWORD NextIP;	/* next host to receive the packet in rout */
	struct SEtherHead DT_XDATA * pEtherHead;
	struct SMemHead DT_XDATA *p;
	
	pEtherHead = (struct SEtherHead DT_XDATA *)(MemHead->pStart - sizeof(struct SEtherHead));
		
	/* if DestIP in this subnet ... */
	if((NetIf->NetMask & NetIf->IPAddr) == (NetIf->NetMask & DestIP))
		NextIP = DestIP;
	else
		NextIP = NetIf->GateWay;

	/* find Ether addr of NextIP */
	if(ARPFind(pEtherHead->DestAddr,NextIP) == FALSE)
	{
		/* send a arp query */
		if((p = ARPQuery(NetIf,NextIP)) != NULL)
		{	
			((struct SEtherDevice DT_XDATA *)(NetIf->Info))->send(
				p->pStart,sizeof(struct SARPPacket) + 
				sizeof(struct SEtherHead));

			MemFree(p);
		}
	}
	else
	{
		/* fill ehter header, DestAddr already filled in ARPFind */
		MemCopy(pEtherHead->ScrAddr,
			((struct SEtherDevice DT_XDATA *)(NetIf->Info))->Addr,ETHER_ADDR_LEN);

		pEtherHead->type = htons(ETHER_TYPE_IP);	

		/* send the packet. packet lenth is less than MemHead size */
		return ((struct SEtherDevice DT_XDATA *)(NetIf->Info))->send(
			pEtherHead,(WORD)(MemHead->pEnd - (BYTE DT_XDATA *)pEtherHead));
	}
	return FALSE;
	/* free MemHead when it is acked in tcp model */
}

/* this function is called periodically.Get a packet from specific
device. If there is a packet, call NetIf->Input to do more */
void EtherInput(struct SNetIf DT_XDATA * NetIf) REENTRANT_SIG
{
	struct SMemHead DT_XDATA *MemHead;
	struct SEtherHead DT_XDATA *pEtherHead;
	struct SMemHead DT_XDATA *p;

	/* if there is a packet to deal with */
	while((MemHead = ((struct SEtherDevice DT_XDATA *)(NetIf->Info))->recv())
		!= NULL)
	{
		/* Note, pStart point to EtherHead */
		pEtherHead = (struct SEtherHead DT_XDATA *)(MemHead->pStart);
		
		/* which packet type */
		switch(ntohs(pEtherHead->type))
		{
		case ETHER_TYPE_IP:
			/* before pass to IP layer, let MemHead->pStart point
			to IP header */
			MemHead->pStart += sizeof(struct SEtherHead);

			/* pass to IP layer for more dealing */
			IPInput(MemHead);
			break;

		case ETHER_TYPE_ARP:
			if((p = ARPInput(MemHead,NetIf)) != NULL)
			{
				/* a arp reply need to be send */
				((struct SEtherDevice DT_XDATA *)(NetIf->Info))->send(
					p->pStart,sizeof(struct SARPPacket) 
					+ sizeof(struct SEtherHead));

				MemFree(p);
			}
			/* 'MemHead' is freed in ARPInput() */
			break;
		default:

			/* unknown packet type free */
			MemFree(MemHead);
		}
	}
}
/* ethernet device init */
void EtherDevInit(struct SEtherDevice DT_XDATA * pDevice, BYTE EtherAddr[],
				  BOOL (DT_CODE * send)(void DT_XDATA *buf, WORD size)  REENTRANT_SIG,
				  struct SMemHead DT_XDATA *(DT_CODE * recv)()  REENTRANT_SIG) REENTRANT_MUL
{
	MemCopy(pDevice->Addr,EtherAddr,ETHER_ADDR_LEN);
	pDevice->recv = recv;
	pDevice->send = send;
}



