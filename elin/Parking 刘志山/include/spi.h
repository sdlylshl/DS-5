#ifndef __SPI_H
#define __SPI_H
#include "stdint.h"

void WIZ_CS(uint8_t val); //设置SPI速度   
uint8_t SPI1_SendByte(uint8_t byte);//SPI总线读写一个字节
//void SPI1_Init(void);

#endif

