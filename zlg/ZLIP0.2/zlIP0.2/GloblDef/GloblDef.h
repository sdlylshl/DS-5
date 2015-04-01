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

/* data type define */
typedef unsigned char	BYTE;
typedef unsigned short	WORD;
typedef unsigned long	DWORD;
typedef unsigned char	BOOL;

typedef signed long		SDWORD;

#ifndef DEBUG 
//#	define DEBUG 
#endif

#ifdef DEBUG
#	include "stdio.h"	/* use printf */
#endif

#ifndef MCU_C51
#	define MCU_C51
#endif


#ifndef HOST_ORDER_AS_NET
#	define HOST_ORDER_AS_NET
#endif

#ifndef MULTI_THREAD	/* if this program is run in multithread */
//#	define MULTI_THREAD
#endif

#ifdef	MCU_C51
#	include "..\MCU\at89x52.h"
#	define	DT_CODE		code
#	define	DT_XDATA	xdata
#	define	REENTRANT_SIG	reentrant
#	ifdef	MULTI_THREAD
#		define	REENTRANT_MUL	reentrant
#	else
#		define	REENTRANT_MUL	
#	endif
#else
#	define	DT_CODE		
#	define	DT_XDATA
#	define	REENTRANT_SIG	
#	define	REENTRANT_MUL	
#endif

#ifndef TRUE
#	define TRUE 1
#endif

#ifndef FALSE
#	define FALSE 0
#endif

#ifndef NULL
#	define NULL 0
#endif

#ifdef DEBUG
#	define TRACE(str)	printf(str)
#	define VALUE(v)		printf(" %x ",v)
#else
#	define TRACE(str)
#	define VALUE(v)		
#endif

#ifndef HOST_ORDER_AS_NET
DWORD ntohl(DWORD in)  REENTRANT_SIG;
WORD ntohs(WORD in)  REENTRANT_SIG;
#else
#	define	ntohl(in)	in
#	define	ntohs(in)	in
#endif

#define htonl(in) ntohl(in)
#define htons(in) ntohs(in)

void MemCopy(void DT_XDATA *buf1,void DT_XDATA *buf2,WORD size)  REENTRANT_SIG;

/*
 * about problom of using function opointer in KeilC51 
 * refer to KeilC application note 129 -- Function Pointers in C51
 */
/*	
	1. use BL51 OVERLAY
	Delete:
	The fllowing function should be delete from call tree function 'main':
	RTLSendPacket RTLReceivePacket EtherInput EtherOutput

	Add:
	EtherOutput	to	IPOutput	and	IPInput
	EtherInput	to	NetIfTimer
	RTLSendPacket	to	EtherOutput
	RTLReceivePacket	to EtherInput

	2. define all these func as reentrant
*/
