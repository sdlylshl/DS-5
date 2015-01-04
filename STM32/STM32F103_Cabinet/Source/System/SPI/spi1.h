#ifndef __SPI1_H
#define __SPI1_H

#include "version.h"

//#define SPI1_DMA
#ifdef CABINET_DEVICE
	#define SPI1_REMAP
#else
	#define SPI1_NONREMAP
#endif

#ifdef CABINET_DEVICE		
//Cabinet Device SPI1??|SPI3
#define  SPI1_RCC_APB2_PORT (RCC_APB2Periph_GPIOB)

#define SPI1_IRQ_PORT				GPIOB
#define SPI1_IRQ_PIN				GPIO_Pin_0
#define SPI1_IRQ_MODE				GPIO_Mode_IPU		
	
#define SPI1_RST_PORT				GPIOB
#define SPI1_RST_PIN				GPIO_Pin_1		
#define SPI1_RST_MODE				GPIO_Mode_Out_PP

#else		//Cabinet Host SPI1
#define SPI1_RCC_APB2_PORT (RCC_APB2Periph_GPIOA)

#define SPI1_IRQ_PORT				GPIOA
#define SPI1_IRQ_PIN				GPIO_Pin_1
#define SPI1_IRQ_MODE				GPIO_Mode_IPU		
	
#define SPI1_RST_PORT				GPIOA
#define SPI1_RST_PIN				GPIO_Pin_0		
#define SPI1_RST_MODE				GPIO_Mode_Out_PP
#endif
			
#if 0		//Societ 
#define  SPI1_RCC_APB2_PORT (RCC_APB2Periph_GPIOD)

#define SPI1_IRQ_PORT				GPIOD
#define SPI1_IRQ_PIN				GPIO_Pin_8
#define SPI1_IRQ_MODE				GPIO_Mode_IPU		
	
#define SPI1_RST_PORT				GPIOD
#define SPI1_RST_PIN				GPIO_Pin_9		
#define SPI1_RST_MODE				GPIO_Mode_Out_PP
#endif

#ifdef SPI1_NONREMAP
// SPI1 PA 4 5 6 7
#define SPI1_SCS_PORT				GPIOA
#define SPI1_SCS_PIN				GPIO_Pin_4
#define SPI1_SCS_MODE				GPIO_Mode_Out_OD

#define SPI1_SCLK_PORT			GPIOA
#define SPI1_SCLK_PIN				GPIO_Pin_5
#define SPI1_SCLK_MODE			GPIO_Mode_AF_PP

#define SPI1_MISO_PORT			GPIOA
#define SPI1_MISO_PIN				GPIO_Pin_6
#define SPI1_MISO_MODE			GPIO_Mode_AF_PP

#define SPI1_MOSI_PORT			GPIOA
#define SPI1_MOSI_PIN				GPIO_Pin_7
#define SPI1_MOSI_MODE			GPIO_Mode_AF_PP
#else
// SPI1 PA 15 PB 3 4 5
#define SPI1_SCS_PORT				GPIOA
#define SPI1_SCS_PIN				GPIO_Pin_15
#define SPI1_SCS_MODE				GPIO_Mode_Out_PP

#define SPI1_SCLK_PORT			GPIOB
#define SPI1_SCLK_PIN				GPIO_Pin_3
#define SPI1_SCLK_MODE			GPIO_Mode_AF_PP

#define SPI1_MISO_PORT			GPIOB
#define SPI1_MISO_PIN				GPIO_Pin_4
#define SPI1_MISO_MODE			GPIO_Mode_AF_PP

#define SPI1_MOSI_PORT			GPIOB
#define SPI1_MOSI_PIN				GPIO_Pin_5
#define SPI1_MOSI_MODE			GPIO_Mode_AF_PP
#endif

#define SPI1_SCS_HIGH()     GPIO_SetBits(SPI1_SCS_PORT, SPI1_SCS_PIN)
#define SPI1_SCS_LOW()			GPIO_ResetBits(SPI1_SCS_PORT, SPI1_SCS_PIN)
#define	SPI1_SCS(x)   			x ? GPIO_SetBits(SPI1_SCS_PORT,SPI1_SCS_PIN): GPIO_ResetBits(SPI1_SCS_PORT,SPI1_SCS_PIN)

#define SPI1_RST_HIGH()     GPIO_SetBits(SPI1_RST_PORT, SPI1_RST_PIN)
#define SPI1_RST_LOW()			GPIO_ResetBits(SPI1_RST_PORT, SPI1_RST_PIN)
#define	SPI1_RST(x)   			x ? GPIO_SetBits(SPI1_RST_PORT,SPI1_RST_PIN): GPIO_ResetBits(SPI1_RST_PORT,SPI1_RST_PIN)

#define	SPI1_READ_IRQ()   	return GPIO_ReadInputDataBit(SPI1_IRQ_PORT, SPI1_IRQ_PIN)

void SPI1_Config(void);
uint8_t SPI1_SendByte(uint8_t byte);
uint8_t SPI1_ReceiveByte(void);
void SPI1_deselect(void);
void SPI1_select(void);
#endif

