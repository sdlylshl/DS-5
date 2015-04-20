/*----------------------------------------------------------------------------
 * Name:    Retarget.c
 * Purpose: 'Retarget' layer for target-dependent low level functions
 * Note(s):
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2011 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include "stm32f10x.h"
//#include "stm32f10x_usart.h"
#include "stm32f10x_conf.h"
#pragma import(__use_no_semihosting_swi)


const uint32_t MAX_UINT32 = (uint32_t) -1;

//voeride
int SER_GetChar(void) {
	int ch;
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET) {
	}
	ch = USART_ReceiveData(USART1);
	return ch;
}
int SER_PutChar(char ch) {
	// Write a character to the USART
	USART_SendData(USART1, (uint8_t) ch);

	//  Loop until the end of transmission
	//while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET){}
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
		;
	return ch;

}

struct __FILE {
	int handle; /* Add whatever you need here */
};
FILE __stdout;
FILE __stdin;

//MicroLIB重定向c库函数printf到USART
int fputc(int c, FILE *f) {
	return (SER_PutChar(c));
	//return 0;
}

int fgetc(FILE *f) {
	return (SER_GetChar());
	//return 0;
}

int ferror(FILE *f) {
	/* Your implementation of ferror */
	return EOF;
}

void _ttywrch(int c) {
	SER_PutChar(c);
}

void _sys_exit(int return_code) {
	label: goto label;
	/* endless loop */
}


void CallbackNull(void){ 
	; 
}

void disable_irq(){
__set_PRIMASK(1);
}
void enable_irq(){
__set_PRIMASK(0);
}

