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
#include "..\Netif\RTL8019.h"
#include "..\TCPIP\IP.h"
#include "..\Netif\etherif.h"
#include "..\Netif\ARP.h"
#include "..\TCPIP\Netif.h"
#include "..\TCPIP\TCP.h"
#include "..\App\main.h"

#define DATA_SIZE 	0x500
BYTE DT_XDATA DataBlock[DATA_SIZE];
BYTE DT_XDATA str[]="hello";
socket		DT_XDATA * DT_XDATA ExConn;
socket		DT_XDATA * DT_XDATA	ExAccept;
socket		DT_XDATA * DT_XDATA	ExListen;


void OnReceive(void DT_XDATA * buf,WORD size)  REENTRANT_MUL
{
	/* send back data */
	TCPSend(ExConn,buf,size);
}
void OnAcceptRecv(void DT_XDATA *buf,WORD size) REENTRANT_MUL
{
	/* printf received data */
#ifdef DEBUG
	while(size--)
		printf("%c",*((BYTE DT_XDATA *)buf)++);
#endif
		
	/* send back data */
	TCPSend(ExAccept,buf,size);
}
void OnAccept(socket DT_XDATA *pNewSocket) REENTRANT_MUL
{
	ExAccept = pNewSocket;
	pNewSocket->recv = OnAcceptRecv;
	pNewSocket->close = OnClose;
}
void OnClose(socket DT_XDATA * pSocket) REENTRANT_MUL
{
	TCPClose(pSocket);	/* we close too */
}
void Timer() REENTRANT_MUL interrupt 1
{
	TH0 = 0x3C;	/*TIMER_24M_25MS_H;*/
	TL0 = 0xAA;	/*TIMER_24M_25MS_L;*/
	
	NetIfTimer();
	ARPTimer();
	TCPTimer();
}

void SerialInit()
{
	/* set TI to 1, set TR1 to 1 */
	SCON = 0x52;/* SM0 SM1 =1 SM2 REN TB8 RB8 TI RI */
	TMOD = 0x20;/* GATE=0 C/T-=0 M1 M0=2 GATE C/T- M1 M0 */
	TH1 = 0xE6;	/* TH1=E6 4800 when at 24MHz,TH1=F3,9600,24MHz */
	PCON = 0x80;
	TCON = 0x40;/* 01101001 TF1 TR1 TF0 TR0 IE1 IT1 IE0 IT */
}

void InterruptInit()
{
	TMOD |= 0x01;
	TH0 = 0x3C;	/*TIMER_24M_25MS_H;*/
	TL0 = 0xAA;	/*TIMER_24M_25MS_L;*/
	TR0 = 1;
	
	/* open interrupt for rtl and timer */
	EA = 1;
	/* EX0 = 1; */
	ET0 = 1;
}
main()
{
	struct SMemHead DT_XDATA *MemHead;
	struct SEtherDevice DT_XDATA DevRTL;
	BYTE	DT_XDATA EtherAddr[ETHER_ADDR_LEN] = {0x52,0x54,0x4c,0x30,0x2e,0x2f};
	IP_ADDR		IPAddr	= 0xc0a8020d;	/* 192.168.2.13	*//* ca71e590:202.113.229.144 *//* ca711075:202.113.16.117 */
	IP_ADDR		NetMask	= 0xffffff00;	/* 255.255.255.0 */
	IP_ADDR		GateWay	= 0xc0a80201;	/* 192.138.2.1  */
	
	/*
	 * init
	 */
	/* init. the order is not important */
	NetIfInit();
	ARPInit();
	TCPInit();
	MemInit();
	RTLInit(EtherAddr);	
	
	/* init Devcie struct and init this device */
	EtherDevInit(&DevRTL,EtherAddr,RTLSendPacket,RTLReceivePacket);

	/* add this device to NetIf */
	NetIfAdd(IPAddr,NetMask,GateWay,EtherInput,EtherOutput,&DevRTL);

	/*
	 * start use socket 
	 */
	SerialInit();
	InterruptInit();
	TRACE("start");
	
	/* Illustrate how to listen as a server */
	ExListen = TCPSocket(IPAddr);
	ExAccept = NULL;
	TCPListen(ExListen,TCP_DEFAULT_PORT,OnAccept);
	
	/* Illustrate how to connect to a server */
	ExConn = TCPSocket(IPAddr);
	if(TCPConnect(ExConn,0xc0a8020e,1001,OnReceive,OnClose) == TRUE)
	{
		/* Illustrate how to Use TCPSend() */
		TCPSend(ExConn,str,6);

		/* Illustrate how to Use TCPSendEx() */
		while(1)
		{
			if((MemHead = TCPAllocate(DATA_SIZE)) == NULL)
				break;
			MemCopy(MemHead->pStart,DataBlock,DATA_SIZE);
			if(TCPSendEx(ExConn,MemHead) == FALSE)
				break;
		}
	}
	
	TCPAbort(ExConn); 

	while(1);
}




