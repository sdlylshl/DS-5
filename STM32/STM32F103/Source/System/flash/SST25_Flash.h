#ifndef __SST25_FLASH_H
#define __SST25_FLASH_H

#include "stdint.h"
#include "stm32f10x.h"
#include "../SPI/spi2.h"
#define Dummy_Byte 0

/* Select SPI FLASH: ChipSelect pin low  */
#define Select_Flash()    SPI2_SCS_LOW()
//GPIO_ResetBits(GPIOB, GPIO_Pin_12)
/* Deselect SPI FLASH: ChipSelect pin high */
#define DeSelect_Flash() SPI2_SCS_HIGH() 
//GPIO_SetBits(GPIOB, GPIO_Pin_12)
//void SPI_Flash_Init(void);	        //SPI初始化
//uint8_t SPI_Flash_ReadByte(void);		//flash操作基本函数，读一个字节
//uint8_t SPI_Flash_SendByte(uint8_t byte);		//	FLASH操作基本函数，发送一个字节

#define SPI_Flash_SendByte SPI2_SendByte
#define SPI_Flash_ReadByte SPI2_ReceiveByte

extern uint8_t FlashReadStatus(void); //读状态寄存器
extern void FlashWaitBusy(void);	//忙检测extern void FlashWriteEnable(void);	//写使能
extern void FlashWriteDisable(void);//写禁止
extern void FlashWriteStatus(void);	//写状态
extern void FlashSectorErase(uint32_t a1); //页擦除
extern void FlashChipErase(void);	//
extern uint16_t FlashReadID(void);		        //读取flashID四个字节

void SST25_ReadHighSpeed(uint32_t addr, uint8_t *readbuff, uint16_t BlockSize);
void SST25_WriteAutoAddrIncrease(uint32_t addr, uint8_t *readbuff, uint16_t BlockSize);

void testFlash(void );

//FATFS interface
uint8_t SST25_disk_status(void);
uint8_t SST25_disk_initialize(void);
uint8_t SST25_disk_read(uint8_t *buff,		/* Data buffer to store read data */
	uint16_t sector,	/* Sector address in LBA */
	uint32_t count		/* Number of sectors to read */);
uint8_t SST25_disk_write(uint8_t *buff,		/* Data buffer to store read data */
	uint16_t sector,	/* Sector address in LBA */
	uint32_t count		/* Number of sectors to read */);


#endif
