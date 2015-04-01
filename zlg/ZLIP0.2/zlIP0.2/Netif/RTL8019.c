/*
 * Copyright (c) 2003 Electronic Engineering department of NAN KAI university
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
 * Author: zlin <wzzlin@eyou.com>
 *
 */
#include "..\GloblDef\GloblDef.h"
#include "..\TCPIP\TCPIPmem.h"
#include "..\Netif\RTL8019.h"

/* to prevent call RTLSendPacket() when RTLSendPackt() is already is called, InSending
is used. example when process run in RTLSendPacket() and a interrupt ocurr then call 
RTLSendPacket again, then the Register would have changed when interrupt return. */
static BYTE DT_XDATA InSending;

static BYTE DT_XDATA StartPageOfPacket;
/* receive head information */
/*struct RTLReceiveHeader
{
	BYTE ReceiveStatus;
	BYTE NextPacketStartPage;
	BYTE PacketSizeLow;
	BYTE PacketSizeHigh;
}Head;for some unknown resean Head must be a gloal value. */
static BYTE DT_XDATA Head[4]; 

/* last trasmit start page */
static BYTE DT_XDATA LastSendStartPage;

/* read rtl8019 register. port is reg address */
/*BYTE ReadReg(WORD port) 
{
	return *((BYTE xdata *)port);
}*/
#define ReadReg(port) (*((BYTE DT_XDATA *)port))

/* write value to register */
/*void WriteReg(WORD port,BYTE value) 
{
	*((BYTE xdata *)port) = value;
}*/
#define WriteReg(port,value) (*((BYTE DT_XDATA *)port) = value)

/* select which page of register to use*/
/* TO DO:set bit 7-6 in CR, CR_TXP must be 0(if 1 the packet is retrasmit) */
#define RTLPage(Index) WriteReg(CR,(ReadReg(CR) & 0x3B)|(BYTE)(Index << 6))

/* reset rtl8019 and init registers, LocalMacAddr is MAC address */
void RTLInit(BYTE LocalMACAddr[]) REENTRANT_MUL
{
	BYTE temp;
	int i;
	
	/* after hardware reset a longdelay is necessary for rtl to self-initial */
	for(i=0; i < RTL_DELAY_AFTER_HARDWARE_RESET; i++);	
	
	/* reset: write to reset prot */
	temp = ReadReg(RESET_PORT);
	WriteReg(RESET_PORT,temp);
	
	/* init RTL registers*/
	WriteReg(CR,(CR_PAGE0 | CR_ABORT_COMPLETE_DMA | CR_STOP_COMMAND)); /* set page0, stop command. command is stop after power up. */

	WriteReg(PSTART_WPAGE0,	RECEIVE_START_PAGE);	/* Pstart	*/
	WriteReg(PSTOP_WPAGE0,	RECEIVE_STOP_PAGE);		/* Pstop	*/
	WriteReg(BNRY_WPAGE0,		RECEIVE_START_PAGE);	/* BNRY		*/
	WriteReg(TPSR_WPAGE0,		SEND_START_PAGE0);	/* TPSR		*/

	WriteReg(RCR_WPAGE0,		0xCE);	/* RCR: refer to define of RCR in Rtl8019as.h */
	WriteReg(TCR_WPAGE0,		0xE0);	/* TCR: refer to define of TCR in Rtl8019as.h */
	WriteReg(DCR_WPAGE0,		0xC8);	/* DCR: refer to define of DCR in Rtl8019as.h */

	WriteReg(IMR_WPAGE0,0);				/* RTL recieve interrupt enabled */
	WriteReg(ISR_WPAGE0,		0xFF);	/* write FF to clear up all interrupt status */

	RTLPage(1);

	WriteReg(CURR_WPAGE1,RECEIVE_START_PAGE + 1);	
	
	/* MAR0 */
	/*WriteReg(0x08,0x00);	
	WriteReg(0x09,0x41);
	WriteReg(0x0a,0x00);
	WriteReg(0x0b,0x80);
	WriteReg(0x0c,0x00);
	WriteReg(0x0d,0x00);
	WriteReg(0x0e,0x00);
	WriteReg(0x0f,0x00);*/

	/* set phisical address */
	WriteReg(PRA0_WPAGE1,LocalMACAddr[0]);
	WriteReg(PRA1_WPAGE1,LocalMACAddr[1]);
	WriteReg(PRA2_WPAGE1,LocalMACAddr[2]);
	WriteReg(PRA3_WPAGE1,LocalMACAddr[3]);
	WriteReg(PRA4_WPAGE1,LocalMACAddr[4]);
	WriteReg(PRA5_WPAGE1,LocalMACAddr[5]);

	/* transimit start page */
	LastSendStartPage = SEND_START_PAGE0;
	StartPageOfPacket = RECEIVE_START_PAGE + 1;

	/* in the beginning, no packet is in sending */
	InSending = FALSE;

	/* initial over, start command and receive */
	WriteReg(CR,(CR_PAGE0 | CR_ABORT_COMPLETE_DMA | CR_START_COMMAND));
}

/* write buffer to rlt ram */
void RTLWriteRam(WORD address, WORD size, BYTE DT_XDATA * buff) REENTRANT_SIG
{
	BYTE DT_XDATA *Endp;
	BYTE PrePage;	/* store page */
	PrePage = ReadReg(CR);
	RTLPage(0);
	WriteReg(RSARH_WPAGE0,(BYTE)((address>>8)&0x00ff));
	WriteReg(RSARL_WPAGE0,(BYTE)address);
	WriteReg(RBCRH_WPAGE0,(BYTE)((size>>8)&0x00ff));
	WriteReg(RBCRL_WPAGE0,(BYTE)size);
	WriteReg(CR,(0x00 | CR_REMOTE_WRITE | CR_START_COMMAND));
	for(Endp = buff + size; buff < Endp;)
	{
		WriteReg(REMOTE_DMA_PORT,*(buff++));
	}
	/* complete dma */
	WriteReg(RBCRH_WPAGE0,0);
	WriteReg(RBCRL_WPAGE0,0);
	WriteReg(CR,((PrePage&0xC0) | CR_ABORT_COMPLETE_DMA | CR_START_COMMAND));
}

/* read rlt ram data to buffer */
void RTLReadRam(WORD address,WORD size,BYTE DT_XDATA * buff) REENTRANT_MUL
{
	BYTE DT_XDATA * Endp;
	BYTE PrePage;	/* store page */
	
	PrePage = ReadReg(CR);
	RTLPage(0);
	WriteReg(RSARH_WPAGE0,(BYTE)((address>>8)&0x00ff));
	WriteReg(RSARL_WPAGE0,(BYTE)address);
	WriteReg(RBCRH_WPAGE0,(BYTE)((size>>8)&0x00ff));
	WriteReg(RBCRL_WPAGE0,(BYTE)size);
	WriteReg(CR,(0x00 | CR_REMOTE_READ | CR_START_COMMAND));
	for(Endp = buff + size; buff < Endp;)
	{
		*(buff++) = ReadReg(REMOTE_DMA_PORT);
	}
	/* complete dma */
	WriteReg(RBCRH_WPAGE0,0);
	WriteReg(RBCRL_WPAGE0,0);
	WriteReg(CR,((PrePage&0xC0) | CR_ABORT_COMPLETE_DMA | CR_START_COMMAND));
}
/* call this function to send a packet by RTL8019. packet store in ram
starts at 'buffer' and its size is 'size'. 'size' should not large than
MAX_PACKET_SIZE or the excess data will be discard. */
BOOL RTLSendPacket(BYTE DT_XDATA * buffer,WORD size) REENTRANT_SIG
{
	BYTE StartPage;
	BYTE PrePage;
	
	/* if send is already running */
	if(InSending == TRUE)
		return FALSE;
	else
		InSending = TRUE;
	/* store page */
	PrePage = ReadReg(CR);
	
	/* check pakcet size */
	if(size < MIN_PACKET_SIZE)
	{
		size = MIN_PACKET_SIZE;
	}
	else
	{
		if(size > MAX_PACKET_SIZE)
			size = MAX_PACKET_SIZE;
	}

	/* write packet to ram */
	if(LastSendStartPage == SEND_START_PAGE0)
	{
		StartPage = SEND_START_PAGE1;
		LastSendStartPage = SEND_START_PAGE1;
	}
	else
	{
		StartPage = SEND_START_PAGE0;
		LastSendStartPage = SEND_START_PAGE0;
	}
	RTLWriteRam((WORD)(((WORD)StartPage)<<8),size,buffer);
	
	/* wait for last time trasmition to complete */
	while((ReadReg(CR) & CR_TXP) == CR_TXP);

	/* write trasmit start page and size */
	RTLPage(0);
	WriteReg(TPSR_WPAGE0,StartPage);	/* TPSR */				
	WriteReg(TBCRL_WPAGE0,(BYTE)size);/*low */					
	WriteReg(TBCRH_WPAGE0,(BYTE)((size>>8)&0x00ff));	/*high*/
	WriteReg(CR,((PrePage&0xC0) | CR_ABORT_COMPLETE_DMA | CR_TXP | CR_START_COMMAND));		
	
	InSending = FALSE;
	return TRUE;
}

/* call this function to receive a ethernet packet from RTL8019. 
return value: 
	NULL: no packet can receive yet. 
	not NULL: 
		a address point to MemHead. This Head contain merory 
		Imformation(memory start address, memory end address ...) of 
		received packet. Memory is allocated by function 'MemAllocate(WORD size)'.
		a example of struct SMemHead is:

		struct SMemHead
		{
			BOOL used;						// if in using 
			BYTE DT_XDATA *pStart;			// the start address of memory 
			BYTE DT_XDATA *pEnd;
		};
		
		You can use your own struct SMemHead and MemAllocat function in your project.		
*/
struct SMemHead DT_XDATA * RTLReceivePacket() REENTRANT_SIG
{
	BYTE curr,bnry;
	WORD address;
	WORD PacketSize;
	struct SMemHead DT_XDATA *MemHead;

	/* if send is running don't crrupt RTL register*/
	if(InSending == TRUE)
		return NULL;

	MemHead = NULL;

	RTLPage(1);
	curr = ReadReg(CURR_RPAGE1);
	RTLPage(0);

	/* check if startpage exceed range becasue of unknow error */
	if(StartPageOfPacket >= RECEIVE_STOP_PAGE || StartPageOfPacket < RECEIVE_START_PAGE)
	{
		/* use curr as the StartPageOfPacket in this case */
		StartPageOfPacket = curr;
		return NULL;
	}
	
	/* check if there is packets to read */
	if(StartPageOfPacket == curr)
		return NULL;

	/* 
	 * read a packet 
	 */

	/* read packet head imformation */
	address = ((WORD)StartPageOfPacket)<<8;
	RTLReadRam(address,4,Head);

	/* check rsr, if isn't a good packet no read */
	if(Head[0] & RSR_RECEIVE_NO_ERROR)
	{
		/* this is a good packet */

		/* packet size, sub 4 bytes, this 4 byte is MAC checksum */
		PacketSize = ((WORD)Head[3])*256 + Head[2] - 4;	

		/* allocate buffer and read packet into buffer */
		if((MemHead = MemAllocate(PacketSize)) != NULL)
		{	
			/* if packet is put from bnry+1 to receive_stop_page and receive
			   start page to next packet startpage, that is if bnry+1 > next
			   packet start page and next start page != receive_start_page, 
			   we need read by two times. the first time from bnry+1 to receive
			   _stop_page, the second time from receive start page to next packet
			   startpage.
			*/
			address += 4;
			if(StartPageOfPacket > Head[1] && Head[1] != RECEIVE_START_PAGE)
			{
				RTLReadRam(address,(WORD)((((WORD)RECEIVE_STOP_PAGE)<<8) - address),MemHead->pStart);	/* read from rtl */
				RTLReadRam((WORD)(((WORD)RECEIVE_START_PAGE)<<8),(WORD)(PacketSize - ((((WORD)RECEIVE_STOP_PAGE)<<8) - address)),
					MemHead->pStart + ((((WORD)RECEIVE_STOP_PAGE)<<8) - address));	/* read from rtl */
			}
			else
			{
				RTLReadRam(address,PacketSize,MemHead->pStart);	/* read from rtl */
			}
			
		}
	}
	
	/* get next packet start page */
	StartPageOfPacket = Head[1];

	/* reset bnry */
	bnry = StartPageOfPacket - 1;
	if(bnry < RECEIVE_START_PAGE)
		bnry = RECEIVE_STOP_PAGE - 1;
	WriteReg(BNRY_WPAGE0,bnry);

	return MemHead;
}

/*void Start8019()
{
	WriteReg(CR,CR_ABORT_COMPLETE_DMA | CR_START_COMMAND);
}

void Stop8019()
{
	WriteReg(CR,CR_ABORT_COMPLETE_DMA | CR_STOP_COMMAND);
}*/
