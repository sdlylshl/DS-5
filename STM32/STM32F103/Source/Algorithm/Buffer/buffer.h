/*
 * buffer.h
 *
 *  Created on: 2015年3月20日
 *      Author: souls
 */

#ifndef BUFFER_H_
#define BUFFER_H_

#include "stdint.h"

typedef struct {

	uint8_t* buffer_write;
	uint8_t* buffer_read;
	uint8_t * buffer;
}Ebuffer_t;//Efficient Buffer

void Ebuffer_Init(uint8_t* pread,uint8_t* pwrite,uint8_t * pbuf);
uint8_t Ebuffer_getused(void);
//
uint8_t Ebuffer_getspace(void);
uint16_t Ebuffer_read(void);
uint8_t Ebuffer_reads(uint8_t * rbuf,uint8_t size);
uint8_t Ebuffer_readback(void);
uint8_t* Ebuffer_getpbufer(void);
#endif /* BUFFER_H_ */
