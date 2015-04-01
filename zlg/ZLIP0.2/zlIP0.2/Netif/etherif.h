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

/* net interface. fetch packet and send packet */
#define ETHER_TYPE_IP	0x0800	
#define ETHER_TYPE_ARP  0x0806

#define ETHER_ADDR_LEN	6	

	
/* the header of Ethernet packet */
struct SEtherHead
{
	BYTE	DestAddr[ETHER_ADDR_LEN];
	BYTE	ScrAddr[ETHER_ADDR_LEN];

	/* 16 bits.0800H IP, 0806H ARP, value less than 0x0600 used in 
	IEEE802 to indicate the length of the packet*/
	WORD type;	
	
};

/* struct for every ethernet device */
struct SEtherDevice
{
	BYTE Addr[ETHER_ADDR_LEN];

	/* send by this device */
	BOOL (DT_CODE * send)(void DT_XDATA *buf, WORD size)  REENTRANT_SIG;

	/* get a packet from device buffer. returned packet is sorted
	in buffer pointed by SMemHead. If no packet return NULL */
	struct SMemHead DT_XDATA * (DT_CODE * recv)() REENTRANT_SIG;
};

BOOL EtherOutput(struct SMemHead DT_XDATA *MemHead,struct SNetIf DT_XDATA* NetIf, 
		IP_ADDR DestIP) REENTRANT_SIG;
void EtherInput(struct SNetIf DT_XDATA * NetIf) REENTRANT_SIG;
void EtherDevInit(struct SEtherDevice DT_XDATA * pDevice, BYTE EtherAddr[],
				  BOOL (DT_CODE * send)(void DT_XDATA *buf, WORD size) REENTRANT_SIG,
				  struct SMemHead DT_XDATA *(DT_CODE * recv)() REENTRANT_SIG) REENTRANT_MUL;
