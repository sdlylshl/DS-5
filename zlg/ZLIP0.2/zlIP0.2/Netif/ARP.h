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

/* arp */

/* arppacket->HardwareType */
#define ARP_HARDWARE_TYPE_ETHER 0x0001

/* arppacket->ProtocolType */
#define ARP_PROTOCOL_TYPE_IP	0x0800

#define ARP_HARDWARE_ADDR_LEN_ETHER	ETHER_ADDR_LEN
#define ARP_PROTOCOL_ADDR_LEN_IP	sizeof(IP_ADDR)

/* arppacket->type */
#define ARP_TYPE_ARP_REQUEST	0x0001
#define ARP_TYPE_ARP_REPLY		0x0002
#define ARP_TYPE_RARP_REQUEST	0x0003
#define ARP_TYPE_RARP_REPLY		0x0004

#define ARP_ENTRY_MAX_NUM		4		/* must <= 255 */
#define ARP_ENTRY_TIME_OUT		0xFFFF	/* the time for refresh a entry */

/* arp entry */
struct SARPEntry
{
	IP_ADDR		IPAddr;			
	BYTE	EtherAddr[ETHER_ADDR_LEN];

	/* decrease every time trick. when it hit to 0, 
	remove it from arp entry tabel. if time = 0 indicate
	this entry is invalid*/
	WORD time;		
};

/* arp packet struct */
struct SARPPacket
{
	/* header */
	WORD HardwareType;
	WORD ProtocolType;
	BYTE HardWareAddrLen;
	BYTE ProtocolAddrLen;
	WORD type;				/* refer to arp type */

	/* arp content */
	BYTE	EtherScrAddr[ETHER_ADDR_LEN];
	IP_ADDR		IPScrAddr;
	BYTE	EtherDestAddr[ETHER_ADDR_LEN];
	IP_ADDR		IPDestAddr;
};


void ARPInit() REENTRANT_MUL;
struct SMemHead DT_XDATA * ARPQuery(struct SNetIf DT_XDATA *NetIf,IP_ADDR DestIP) REENTRANT_SIG;
struct SMemHead DT_XDATA *ARPInput(struct SMemHead DT_XDATA *MemHead, struct SNetIf DT_XDATA *NetIf) REENTRANT_MUL;
void ARPAddEntry(struct SARPPacket DT_XDATA *ARPPacket)  REENTRANT_MUL;
BOOL ARPFind(BYTE EtherAddr[],IP_ADDR IPAddr)  REENTRANT_SIG;
void ARPTimer()  REENTRANT_MUL;