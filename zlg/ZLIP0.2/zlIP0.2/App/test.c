#include "..\GloblDef\GloblDef.h"
#include "..\TCPIP\TCPIPmem.h"
#include "..\Netif\RTL8019.h"
#include "..\TCPIP\IP.h"
#include "..\Netif\etherif.h"
#include "..\Netif\ARP.h"
#include "..\TCPIP\Netif.h"
#include "..\TCPIP\TCP.h"
#include "..\App\main.h"

BYTE DT_XDATA DataBlock[0x100];
struct SPacketQueue DT_XDATA * DT_XDATA qhead;
BYTE DT_XDATA str[]="hello";
socket		DT_XDATA * DT_XDATA ExConn;
socket		DT_XDATA * DT_XDATA	ExAccept;
void OnReceive(void DT_XDATA * buf,WORD size)  REENTRANT_MUL
{
	/* send back data */
	/*TCPSend(ExConn,buf,size);*/
}
void OnAcceptRecv(void DT_XDATA *buf,WORD size) REENTRANT_MUL
{
	/* printf received data */
	/*while(size--)
		printf("%c",*((BYTE DT_XDATA *)buf)++);*/
		
	/* send back data */
	/*TCPSend(ExAccept,buf,size);*/
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
	struct SMemHead DT_XDATA *MemHead,*MemHead2,*MemHead3,*MemHead4;
	struct SEtherDevice DT_XDATA DevRTL;
	BYTE	DT_XDATA EtherAddr[ETHER_ADDR_LEN] = {0x52,0x54,0x4c,0x30,0x2e,0x2f};
	IP_ADDR		IPAddr	= 0xc0a8020d;	/* 192.168.2.13	*/
	IP_ADDR		NetMask	= 0xffffff00;	/* 255.255.255.0 */
	IP_ADDR		GateWay	= 0xc0a80201;	/* 192.138.2.1  */
	DWORD i;
	
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

	/* ------------------- test start ---------------------- */
	MemHead = MemAllocate(10);
	qhead = NULL;
	for(i=0;;i++)
	{
		if(TCPGetTCB() == NULL)
			break;
	}	
	
	/* -------------------  test end  ---------------------- */
	
	ExConn = TCPSocket(IPAddr);
	
	/* listen */
	
	/*ExAccept = NULL;
	TCPListen(ExConn,TCP_DEFAULT_PORT,OnAccept);
	while(1);*/
	
	
	/* connect */
	TCPConnect(ExConn,0xc0a8020e,TCP_DEFAULT_PORT,OnReceive,OnClose);	
	while(1)
	{
		if((MemHead = TCPAllocate(0x100)) == NULL)
			break;
		MemCopy(MemHead->pStart,DataBlock,0x100);
		if(TCPSendEx(ExConn,MemHead) == FALSE)
			break;
	}
	while(1);
	TCPAbort(ExConn); 
	
}




