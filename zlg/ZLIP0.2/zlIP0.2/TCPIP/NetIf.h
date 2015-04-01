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

/* Network interface Head file */

/* the max number of netif */
#define NET_IF_MAX_NUM	1	
#define NETIF_HEAD_MAX_LEN	14	/* the max  phsical layer head len. eg ethernet head len */


/* netif is associated with each netwok device */
struct SNetIf
{
	struct SNetIf DT_XDATA *pNext;	/* point to the next netif */
	IP_ADDR IPAddr;			/* Ip address for this device */
	IP_ADDR NetMask;			/* Net mask for this device */
	IP_ADDR GateWay;			/* Gate way IP address fort this device */

	/* call input to get a packet from device to IP layer. This 
	function maybe 'EtherInput()', and called periodically */
	void (DT_CODE * input)(struct SNetIf DT_XDATA * NetIf) REENTRANT_SIG;

	/* call output to put a packet from IP layer to device. After
	Ip layer selected a device, it use output to send this packet.
	MemHead contain a packet and Netif tell dirver which netif it 
	is. NOTE:MemHead->pStart point to pIPHead*/
	BOOL (DT_CODE * output)(struct SMemHead DT_XDATA *MemHead,struct SNetIf DT_XDATA* NetIf,DWORD DestIP) REENTRANT_SIG;	

	void DT_XDATA *Info;	/* information of this device. eg. MAC address. */
};
void NetIfInit() REENTRANT_MUL;
struct SNetIf DT_XDATA * NetIfAdd(DWORD IPAddr, DWORD NetMask,DWORD GateWay,
						 void (DT_CODE * input)(struct SNetIf DT_XDATA * NetIf) REENTRANT_SIG,
						 BOOL (DT_CODE * output)(struct SMemHead DT_XDATA *MemHead,struct SNetIf DT_XDATA* NetIf,DWORD DestIP) REENTRANT_SIG,
						 void DT_XDATA * Info) REENTRANT_MUL;
struct SNetIf DT_XDATA * NetIfFindRout(IP_ADDR IPAddr) REENTRANT_SIG;
struct SNetIf DT_XDATA * NetIfFindIP(IP_ADDR IPAddr) REENTRANT_MUL;
void NetIfTimer() REENTRANT_MUL;

