/*
 * BipBuffer_main.c
 *
 *  Created on: 2014��11��17��
 *      Author: Administrator
 */
#include <stdio.h>
#include <stdlib.h>
//#include <malloc.h>
#include <string.h>

#include "bipbuffer.h"

#define BUFF_LEN 0xFF
BipBufferInfo_t bipBufferInfo;
char buffers[BUFF_LEN];
int Bipmain(void) {
	char buff[BUFF_LEN];
	unsigned long i = 0;
	unsigned long size = 0;

	if (init_bipbuffer(BUFF_LEN) != 0) {
		printf("\nallocate bip buffer err!");
		return -1;
	}

	memset(buff, 0x66, BUFF_LEN);
	write_data_to_bipbuffer(buff, BUFF_LEN);

	size = read_data_from_bipbuffer(buff, BUFF_LEN / 2);
	printf("\n have %ld read:", size);
	for (i = 0; i < size; i++)
		printf("%02X ", buff[i]);
	printf("\n====================================\n");

	memset(buff, 0xFF, BUFF_LEN);
	write_data_to_bipbuffer(buff, BUFF_LEN);
	write_data_to_bipbuffer(buff, BUFF_LEN / 2);

	size = read_data_from_bipbuffer(buff, BUFF_LEN);
	printf("\n have %ld read:", size);
	for (i = 0; i < size; i++)
		printf("%02X ", buff[i]);
	printf("\n====================================\n");

	printf("\n\n\n\n");
	printf_memory();

//fail:
	destroy_bipbuffer();
	return 0;
}

