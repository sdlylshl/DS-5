#ifndef __SST25_FLASH_H
#define __SST25_FLASH_H

#include "stdint.h"
#include "stm32f10x.h"
#include "../SPI/spi1.h"
#include "../SPI/spi2.h"
#define Dummy_Byte 0
//*********************** 接口实现 1************************************

////	停车场ParkinV2.0  SPI2
//#define SST25_SCS_PORT			GPIOB
//#define SST25_SCS_PIN				GPIO_Pin_12
//#define SST25_SCS_MODE			GPIO_Mode_Out_PP	//此处一定要PP输出否则程序不正常
//#define SST25_SCS_HIGH()    GPIO_SetBits(SST25_SCS_PORT, SST25_SCS_PIN)
//#define SST25_SCS_LOW()			GPIO_ResetBits(SST25_SCS_PORT, SST25_SCS_PIN)
////FLASH: ChipSelect  
//#define SST25_Select SST25_SCS_LOW
//#define SST25_DeSelect SST25_SCS_HIGH

//#define SST25_SPI_init SPI2_Init
//#define SST25_SendByte SPI2_SendByte
//#define SST25_ReadByte SPI2_ReceiveByte

//*********************** 接口实现 2************************************
//	security terminal V1.3 SPI1 + GPC13
//PARKING V2.0
#define SPI1_NONREMAP	

#define SST25_SCS_PORT			GPIOC
#define SST25_SCS_PIN				GPIO_Pin_13
#define SST25_SCS_MODE			GPIO_Mode_Out_PP	//此处一定要PP输出否则程序不正常
#define SST25_SCS_HIGH()    GPIO_SetBits(SST25_SCS_PORT, SST25_SCS_PIN)
#define SST25_SCS_LOW()			GPIO_ResetBits(SST25_SCS_PORT, SST25_SCS_PIN)
//FLASH: ChipSelect  
#define SST25_Select SST25_SCS_LOW
#define SST25_DeSelect SST25_SCS_HIGH

#define SST25_SPI_init SPI1_Init
#define SST25_SendByte SPI1_SendByte
#define SST25_ReadByte SPI1_ReceiveByte

//*************************************************************************************
extern void SST25_Flash_init(void);
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
uint8_t		SST25_disk_status(void);
uint8_t		SST25_disk_initialize(void);
uint8_t		SST25_disk_read(uint8_t *buff,		/* Data buffer to store read data */
													uint16_t	sector,	/* Sector address in LBA */
													uint32_t	count		/* Number of sectors to read */);
uint8_t		SST25_disk_write(uint8_t *buff,		/* Data buffer to store read data */
													uint16_t	sector,	/* Sector address in LBA */
													uint32_t	count		/* Number of sectors to read */);


#endif
