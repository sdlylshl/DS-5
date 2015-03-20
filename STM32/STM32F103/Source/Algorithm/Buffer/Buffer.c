/*
 * Buffer.c
 *
 *  Created on: 2015年3月20日
 *      Author: souls
 */
#include "buffer.h"
//     高效循环缓冲区使用情况统计
//  已用空间           可用空间
//		0   read  write	 0xff
//00------------||-------------FF
//		1   read   write  0xff
//00------------|*|------------FF
//	  0xff write   read   1
//00************|-|************FF
//
Ebuffer_t Ebuffer;
void Ebuffer_Init(uint8_t* pread, uint8_t* pwrite, uint8_t * pbuf) {
	Ebuffer.buffer_read = pread;
	Ebuffer.buffer_write = pwrite;
	Ebuffer.buffer = pbuf;
	*Ebuffer.buffer_write = 0;
	*Ebuffer.buffer_read = 0;
}
//1. 获取使用空间大小
//注:如果write 写指针=read 读指针 则 缓冲区被重置.获取已接收数据大小为0
uint8_t Ebuffer_getused(void) {
	return (0xFF - *Ebuffer.buffer_read + *Ebuffer.buffer_write + 1);
}

//2.获取剩余空间大小
uint8_t Ebuffer_getspace(void) {
	if (*Ebuffer.buffer_write == *Ebuffer.buffer_read) {
		return 0xFF;
	} else {
		return (0xFF - *Ebuffer.buffer_write + *Ebuffer.buffer_read + 1);
	}
}
//读取缓冲区数据
uint8_t Ebuffer_reads(uint8_t * rbuf, uint8_t size) {
	uint8_t i;
	if (Ebuffer_getused() >= size) {
		for (i = 0; i < size; i++) {
			rbuf[i] = Ebuffer.buffer[(*Ebuffer.buffer_read)++];
		}
		return size;
	}
	return 0;
}

uint16_t Ebuffer_read(void) {
	if (*Ebuffer.buffer_read != *Ebuffer.buffer_write) {
		return Ebuffer.buffer[(*Ebuffer.buffer_read)++];
	}
	return 0xFFFF;
}
uint8_t Ebuffer_readback(void) {
	return (*Ebuffer.buffer_read)--;
}

uint8_t* Ebuffer_getpbufer(void) {

	return &Ebuffer.buffer[(*Ebuffer.buffer_read)-1];
}
