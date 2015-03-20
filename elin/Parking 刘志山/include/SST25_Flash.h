#ifndef __SST25_FLASH_H
#define __SST25_FLASH_H

#include "stdint.h"
#include "config.h"
#include "TYPES.h"

#define Dummy_Byte 0

/* Select SPI FLASH: ChipSelect pin low  */
#define Select_Flash()     GPIO_ResetBits(GPIOB, GPIO_Pin_12)
/* Deselect SPI FLASH: ChipSelect pin high */
#define NotSelect_Flash()    GPIO_SetBits(GPIOB, GPIO_Pin_12)

void SPI_Flash_Init(void);	        //SPI初始化
uint8_t SPI_Flash_ReadByte(void);		//flash操作基本函数，读一个字节
uint8_t SPI_Flash_SendByte(uint8_t byte);		//	FLASH操作基本函数，发送一个字节

void SST25_R_BLOCK(uint32_t addr, uint8_t *readbuff, uint32_t BlockSize);
void SST25_W_BLOCK(uint32_t addr, uint8_t *readbuff, uint16_t BlockSize);

void FlashWaitBusy(void);			//Flash忙检测
void FlashReadID(void);		        //读取flashID四个字节

void Bulk_clr(void);
void sect_clr(uint32_t a1);
void testFlash(void );


#endif
