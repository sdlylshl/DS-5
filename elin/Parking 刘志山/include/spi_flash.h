#ifndef __SPI_FLASH_H
#define __SPI_FLASH_H

#include "stm32f10x.h"

#define SPI_FLASH_SPI                           SPI2
#define SPI_FLASH_SPI_CLK                       RCC_APB1Periph_SPI2
#define SPI_FLASH_SPI_SCK_PIN                   GPIO_Pin_13                  /* PA.05 */
#define SPI_FLASH_SPI_SCK_GPIO_PORT             GPIOB                       /* GPIOA */
#define SPI_FLASH_SPI_SCK_GPIO_CLK              RCC_APB2Periph_GPIOB
#define SPI_FLASH_SPI_MISO_PIN                  GPIO_Pin_14                  /* PA.06 */
#define SPI_FLASH_SPI_MISO_GPIO_PORT            GPIOB                       /* GPIOA */
#define SPI_FLASH_SPI_MISO_GPIO_CLK             RCC_APB2Periph_GPIOB
#define SPI_FLASH_SPI_MOSI_PIN                  GPIO_Pin_15                  /* PA.07 */
#define SPI_FLASH_SPI_MOSI_GPIO_PORT            GPIOB                       /* GPIOA */
#define SPI_FLASH_SPI_MOSI_GPIO_CLK             RCC_APB2Periph_GPIOB
#define SPI_FLASH_CS_PIN                        GPIO_Pin_12                  /* PC.04 */
#define SPI_FLASH_CS_GPIO_PORT                  GPIOB                       /* GPIOC */
#define SPI_FLASH_CS_GPIO_CLK                   RCC_APB2Periph_GPIOB


#define SPI_FLASH_CS_LOW()       GPIO_ResetBits(GPIOB, GPIO_Pin_12)
#define SPI_FLASH_CS_HIGH()      GPIO_SetBits(GPIOB, GPIO_Pin_12)



void SPI_FLASH_Init(void);
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


uint8_t SPI_FLASH_ReadByte(void);
uint8_t SPI_FLASH_SendByte(uint8_t byte);
uint16_t SPI_FLASH_SendHalfWord(uint16_t HalfWord);
void SPI_FLASH_WriteEnable(void);
void SPI_FLASH_WaitForWriteEnd(void);


#endif /* __SPI_FLASH_H */

