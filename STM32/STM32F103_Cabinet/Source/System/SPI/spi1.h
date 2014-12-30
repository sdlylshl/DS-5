#ifndef __SPI1_H
#define __SPI1_H

#include "version.h"

//#define SPI1_DMA
#ifdef CABINET_DEVICE
	#define SPI1_REMAP
#else
	#define SPI1_NONREMAP
#endif

#ifdef SPI1_NONREMAP
// SPI1 PA 4 5 6 7
#define SPI1_SCS_PORT			GPIOA
#define SPI1_SCS_PIN			GPIO_Pin_4
#define SPI1_SCS_MODE			GPIO_Mode_Out_PP

#define SPI1_SCLK_PORT			GPIOA
#define SPI1_SCLK_PIN			GPIO_Pin_5
#define SPI1_SCLK_MODE			GPIO_Mode_AF_PP

#define SPI1_MISO_PORT			GPIOA
#define SPI1_MISO_PIN			GPIO_Pin_6
#define SPI1_MISO_MODE			GPIO_Mode_AF_PP

#define SPI1_MOSI_PORT			GPIOA
#define SPI1_MOSI_PIN			GPIO_Pin_7
#define SPI1_MOSI_MODE			GPIO_Mode_AF_PP
#else
// SPI1 PA 15 PB 3 4 5
#define SPI1_SCS_PORT			GPIOA
#define SPI1_SCS_PIN			GPIO_Pin_15
#define SPI1_SCS_MODE			GPIO_Mode_Out_PP

#define SPI1_SCLK_PORT			GPIOB
#define SPI1_SCLK_PIN			GPIO_Pin_3
#define SPI1_SCLK_MODE			GPIO_Mode_AF_PP

#define SPI1_MISO_PORT			GPIOB
#define SPI1_MISO_PIN			GPIO_Pin_4
#define SPI1_MISO_MODE			GPIO_Mode_AF_PP

#define SPI1_MOSI_PORT			GPIOB
#define SPI1_MOSI_PIN			GPIO_Pin_5
#define SPI1_MOSI_MODE			GPIO_Mode_AF_PP
#endif

#define	SPI1_SCS(x)   x ? GPIO_SetBits(SPI1_SCS_PORT,SPI1_SCS_PIN): GPIO_ResetBits(SPI1_SCS_PORT,SPI1_SCS_PIN)

void SPI1_Config(void);
uint8_t SPI1_SendByte(uint8_t byte);
uint8_t SPI1_ReceiveByte(void);
void SPI1_deselect(void);
void SPI1_select(void);
#endif

