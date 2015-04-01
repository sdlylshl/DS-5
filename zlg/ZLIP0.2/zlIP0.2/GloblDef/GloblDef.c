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

#ifdef MCU_C51
/* assume a fun return a pointer to a address. If it return
NULL, we assume it return a error. but in 51 MCU a value at 
address 0 of xdata is possible. To a avoid other value use 
address 0 we define a value at address by KeilC keyword _at_ */
BYTE DT_XDATA OmitAddr0 _at_ 0;
#endif

#ifndef HOST_ORDER_AS_NET	
DWORD ntohl(DWORD in) REENTRANT_SIG
{
	DWORD out;
	out = (in<<24) | ((in<<8)&(0x00FF0000)) | ((in>>8)&(0x0000FF00)) | (in>>24) ;
	return out;
}

WORD ntohs(WORD in) REENTRANT_SIG
{
	WORD out;
	out = (in<<8) | (in>>8);
	return out;
}
#endif

/* MemCopy offered by normal C lib */
void MemCopy(void DT_XDATA *buf1,void DT_XDATA *buf2,WORD size) REENTRANT_SIG
{
	BYTE DT_XDATA * EndBuf;
	for(EndBuf = (BYTE DT_XDATA *)buf1 + size; EndBuf != (BYTE DT_XDATA *)buf1;)
	{
		*((BYTE DT_XDATA *)buf1)++ = *((BYTE DT_XDATA *)buf2)++;
	}
}
