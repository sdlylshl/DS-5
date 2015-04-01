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

BYTE DT_XDATA EtherAddrAny[ETHER_ADDR_LEN] = {0xff,0xff,0xff,0xff,0xff,0xff};

/* entry table */
struct SARPEntry DT_XDATA ARPTable[ARP_ENTRY_MAX_NUM];

/* arp init */
void ARPInit() REENTRANT_MUL
{
	BYTE i;

	/* set every unit in arp tabel invalid */
	for(i = 0; i < ARP_ENTRY_MAX_NUM; i++)
		ARPTable[i].time = 0;
}

/* construct a arp query packet and return it */
struct SMemHead DT_XDATA * ARPQuery(struct SNetIf DT_XDATA *NetIf,IP_ADDR DestIP) REENTRANT_SIG
{
	struct SMemHead DT_XDATA *MemHead;
	struct SEtherHead DT_XDATA *EtherHead;
	struct SARPPacket DT_XDATA *ARPPacket;

	/* allocate a packet mem */
	if((MemHead = MemAllocate(sizeof(struct SARPPacket) + 
		sizeof(struct SEtherHead))) == NULL)
		return NULL;
	
	EtherHead = (struct SEtherHead DT_XDATA *)(MemHead->pStart);
	ARPPacket = (struct SARPPacket DT_XDATA *)(MemHead->pStart + 
		sizeof(struct SEtherHead));

	/* fill Ether head */
	MemCopy(EtherHead->DestAddr,EtherAddrAny,ETHER_ADDR_LEN);
	MemCopy(EtherHead->ScrAddr ,
		((struct SEtherDevice DT_XDATA *)(NetIf->Info))->Addr,ETHER_ADDR_LEN);
	EtherHead->type = htons(ETHER_TYPE_ARP);

	/* fill arp head */
	ARPPacket->HardWareAddrLen	= ARP_HARDWARE_ADDR_LEN_ETHER;
	ARPPacket->HardwareType		= htons(ARP_HARDWARE_TYPE_ETHER);
	ARPPacket->ProtocolAddrLen	= ARP_PROTOCOL_ADDR_LEN_IP;
	ARPPacket->ProtocolType		= htons(ARP_PROTOCOL_TYPE_IP);
	ARPPacket->type				= htons(ARP_TYPE_ARP_REQUEST);

	/* fill arp content */
	ARPPacket->IPDestAddr	= DestIP;
	ARPPacket->IPScrAddr	= NetIf->IPAddr;
	MemCopy(ARPPacket->EtherDestAddr,EtherAddrAny,ETHER_ADDR_LEN);
	MemCopy(ARPPacket->EtherScrAddr,
		((struct SEtherDevice DT_XDATA *)(NetIf->Info))->Addr,ETHER_ADDR_LEN);

	return MemHead;
}

/* deel with a input arp packet. if send a reply is needed return 
this replay packet, oterhwise return NULL  */
struct SMemHead DT_XDATA *ARPInput(struct SMemHead DT_XDATA *MemHead, struct SNetIf DT_XDATA *NetIf) REENTRANT_MUL
{
	struct SEtherHead DT_XDATA *EtherHead;
	struct SARPPacket DT_XDATA *ARPPacket;

	EtherHead = (struct SEtherHead DT_XDATA *)(MemHead->pStart);
	ARPPacket = (struct SARPPacket DT_XDATA *)(MemHead->pStart + 
		sizeof(struct SEtherHead));

	/* which type of arp */
	switch(ntohs(ARPPacket->type))
	{
	case ARP_TYPE_ARP_REQUEST:

		/* if arp request to local host */
		if(ARPPacket->IPDestAddr == NetIf->IPAddr)
		{
			/* send arp replay */

			/* fill Ether head */
			MemCopy(EtherHead->DestAddr,ARPPacket->EtherScrAddr,ETHER_ADDR_LEN);
			MemCopy(EtherHead->ScrAddr,
				((struct SEtherDevice DT_XDATA *)(NetIf->Info))->Addr,ETHER_ADDR_LEN);
			EtherHead->type = htons(ETHER_TYPE_ARP);

			/* copy source part to dest part. include Ether addr and 
			Ip addr */
			MemCopy(ARPPacket->EtherDestAddr,ARPPacket->EtherScrAddr,
				(sizeof(IP_ADDR) + ETHER_ADDR_LEN));

			/* fill source part. include Ether addr and Ip addr*/
			ARPPacket->IPScrAddr = NetIf->IPAddr;
			MemCopy(ARPPacket->EtherScrAddr,
				((struct SEtherDevice DT_XDATA *)(NetIf->Info))->Addr,ETHER_ADDR_LEN);

			/* arp type */
			ARPPacket->type = htons(ARP_TYPE_ARP_REPLY);

			return MemHead;
		}
		break;

	case ARP_TYPE_ARP_REPLY:
		/* add to arp table */
		ARPAddEntry(ARPPacket);
		break;
	}
				
	/* for any case except ARP_TYPE_ARP_REQUEST for this IP, 
	arp packet is released */
	MemFree(MemHead);

	/* no packet need send */
	return NULL;
}

/* add a entry to arp table */
void ARPAddEntry(struct SARPPacket DT_XDATA *ARPPacket) REENTRANT_MUL
{
	BYTE i;
	WORD MinTime;
	BYTE iToReplace;	/* index of entry going to be replace */

	/* find a free entry */
	for(i = 0; i<ARP_ENTRY_MAX_NUM; i++)
	{
		if(ARPTable[i].time == 0)
		{
			iToReplace = i;
			break;
		}
	}

	/* if no free entry, find the oldest entry */
	if(i == ARP_ENTRY_MAX_NUM)
	{	
		for(i = 0, MinTime = ARP_ENTRY_TIME_OUT, iToReplace = 0;
			i<ARP_ENTRY_MAX_NUM; i++)
		{
			if(MinTime > ARPTable[i].time)
			{
				MinTime = ARPTable[i].time;
				iToReplace = i;
			}
		}
	}

	/* replace the entry */
	MemCopy(ARPTable[iToReplace].EtherAddr,ARPPacket->EtherScrAddr,
		ETHER_ADDR_LEN);
	ARPTable[iToReplace].IPAddr = ARPPacket->IPScrAddr;

	/* start timer */
	ARPTable[iToReplace].time = ARP_ENTRY_TIME_OUT;
}

/* find IPAddr in arptable copy it to EtherAddr. if can't find return
false */
BOOL ARPFind(BYTE EtherAddr[],IP_ADDR IPAddr) REENTRANT_SIG
{
	BYTE i;
	for(i = 0; i<ARP_ENTRY_MAX_NUM; i++)
	{
		/* check only valid entry */
		if(ARPTable[i].time != 0)
		{
			if(ARPTable[i].IPAddr == IPAddr)
			{
				MemCopy(EtherAddr,ARPTable[i].EtherAddr,ETHER_ADDR_LEN);

				return TRUE;
			}
		}
	}
	return FALSE;
}

void ARPTimer() REENTRANT_MUL
{
	BYTE i;

	/* decrease every entry timer */
	for(i = 0; i<ARP_ENTRY_MAX_NUM; i++)
	{
		/* check only valid entry */
		if(ARPTable[i].time != 0)
		{
			ARPTable[i].time--;
		}
	}
}
	


		

