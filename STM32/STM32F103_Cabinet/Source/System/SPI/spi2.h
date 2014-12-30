#ifndef __SPI2_H
#define __SPI2_H

#include "version.h"
//#include "stm32f10x.h"
//#define SPI2_DMA
// SPI2 PB 12 13 14 15
#define SPI2_SCS_PORT				GPIOB
#define SPI2_SCS_PIN				GPIO_Pin_12
#define SPI2_SCS_MODE				GPIO_Mode_Out_PP

#define SPI2_SCLK_PORT			GPIOB
#define SPI2_SCLK_PIN				GPIO_Pin_13
#define SPI2_SCLK_MODE			GPIO_Mode_AF_PP

#define SPI2_MISO_PORT			GPIOB
#define SPI2_MISO_PIN				GPIO_Pin_14
#define SPI2_MISO_MODE			GPIO_Mode_AF_PP

#define SPI2_MOSI_PORT			GPIOB
#define SPI2_MOSI_PIN				GPIO_Pin_15
#define SPI2_MOSI_MODE			GPIO_Mode_AF_PP

void SPI2_Config(void);
uint8_t SPI2_SendByte(uint8_t byte);
uint8_t SPI2_ReceiveByte(void);

#endif

