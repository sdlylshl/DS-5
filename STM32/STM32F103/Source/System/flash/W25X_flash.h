#ifndef __W25X_FLASH_H
#define __W25X_FLASH_H

#include "stm32f10x.h"
#include "../SPI/spi1.h"
#include "../SPI/spi2.h"

//**********************接口****************************************
//片选不是SPI_NSS 开启NSS映射 PC13
#define SPI_NSS_REMAP

#ifdef  SPI_NSS_REMAP
#define W25X_SCS_PORT			GPIOC
#define W25X_SCS_PIN			GPIO_Pin_13
#define W25X_SCS_MODE			GPIO_Mode_Out_PP	
#define W25X_SCS_HIGH()   GPIO_SetBits(W25X_SCS_PORT, W25X_SCS_PIN)
#define W25X_SCS_LOW()		GPIO_ResetBits(W25X_SCS_PORT, W25X_SCS_PIN)
#define SPI_FLASH_CS_LOW()	GPIO_ResetBits(W25X_SCS_PORT, W25X_SCS_PIN)
#define SPI_FLASH_CS_HIGH()	GPIO_SetBits(W25X_SCS_PORT, W25X_SCS_PIN)
#else
#define SPI_FLASH_CS_LOW()      SPI1_SCS_LOW()
#define SPI_FLASH_CS_HIGH()     SPI1_SCS_HIGH()
#endif

#define SPI_FLASH_Init					SPI1_Init
#define SPI_FLASH_SendByte 			SPI1_SendByte
#define SPI_FLASH_ReadByte 			SPI1_ReceiveByte


//***********************************************************************************
//uint8_t SPI_FLASH_ReadByte(void);
//uint8_t SPI_FLASH_SendByte(uint8_t byte);
//uint16_t SPI_FLASH_SendHalfWord(uint16_t HalfWord);

void W25X_FLASH_Init(void);

void SPI_FLASH_SectorErase(uint32_t SectorAddr);
void SPI_FLASH_BulkErase(void);
void SPI_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
uint32_t SPI_FLASH_ReadID(void);
uint32_t SPI_FLASH_ReadDeviceID(void);
void SPI_FLASH_StartReadSequence(uint32_t ReadAddr);
void SPI_Flash_PowerDown(void);
void SPI_Flash_WAKEUP(void);
void SPI_FLASH_WriteEnable(void);
void SPI_FLASH_WaitForWriteEnd(void);

#endif /* __SPI_FLASH_H */

