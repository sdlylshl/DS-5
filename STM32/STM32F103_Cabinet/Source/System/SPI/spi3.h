#ifndef __SPI3_H
#define __SPI3_H

#include "version.h"

#define SPI3_NONREMAP

//#define SPI3_DMA 

#define SPI3_RCC_APB2_PORT  (RCC_APB2Periph_GPIOB)

#define SPI3_IRQ_PORT				GPIOB
#define SPI3_IRQ_PIN				GPIO_Pin_12
#define SPI3_IRQ_MODE				GPIO_Mode_IPU
           
#define SPI3_RST_PORT			  GPIOB
#define SPI3_RST_PIN				GPIO_Pin_13
#define SPI3_RST_MODE			  GPIO_Mode_Out_PP

#ifdef SPI3_NONREMAP
// SPI3 PB 3 4 5	
#define SPI3_SCS_PORT				GPIOA
#define SPI3_SCS_PIN				GPIO_Pin_15
#define SPI3_SCS_MODE				GPIO_Mode_Out_PP

#define SPI3_SCLK_PORT			GPIOB
#define SPI3_SCLK_PIN				GPIO_Pin_3
#define SPI3_SCLK_MODE			GPIO_Mode_AF_PP	

#define SPI3_MISO_PORT			GPIOB
#define SPI3_MISO_PIN				GPIO_Pin_4
#define SPI3_MISO_MODE			GPIO_Mode_AF_PP

#define SPI3_MOSI_PORT			GPIOB		
#define SPI3_MOSI_PIN				GPIO_Pin_5
#define SPI3_MOSI_MODE			GPIO_Mode_AF_PP
#else
// SPI3 PA4 PC 3 4 5
#define SPI3_SCS_PORT				GPIOA
#define SPI3_SCS_PIN				GPIO_Pin_4
#define SPI3_SCS_MODE				GPIO_Mode_Out_PP

#define SPI3_SCLK_PORT			GPIOC
#define SPI3_SCLK_PIN				GPIO_Pin_10
#define SPI3_SCLK_MODE			GPIO_Mode_AF_PP

#define SPI3_MISO_PORT			GPIOC
#define SPI3_MISO_PIN				GPIO_Pin_11
#define SPI3_MISO_MODE			GPIO_Mode_AF_PP

#define SPI3_MOSI_PORT			GPIOC
#define SPI3_MOSI_PIN				GPIO_Pin_12
#define SPI3_MOSI_MODE			GPIO_Mode_AF_PP
#endif

#define SPI3_SCS_HIGH()     GPIO_SetBits(SPI3_SCS_PORT, SPI3_SCS_PIN)
#define SPI3_SCS_LOW()			GPIO_ResetBits(SPI3_SCS_PORT, SPI3_SCS_PIN)
#define	SPI3_SCS(x)   			x ? GPIO_SetBits(SPI3_SCS_PORT,SPI3_SCS_PIN): GPIO_ResetBits(SPI3_SCS_PORT,SPI3_SCS_PIN)

#define SPI3_RST_HIGH()     GPIO_SetBits(SPI3_RST_PORT, SPI3_RST_PIN)
#define SPI3_RST_LOW()			GPIO_ResetBits(SPI3_RST_PORT, SPI3_RST_PIN)
#define	SPI3_RST(x)   			x ? GPIO_SetBits(SPI3_RST_PORT,SPI3_RST_PIN): GPIO_ResetBits(SPI3_RST_PORT,SPI3_RST_PIN)

#define	SPI3_READ_IRQ()   	return GPIO_ReadInputDataBit(SPI3_IRQ_PORT, SPI3_IRQ_PIN)
	
extern void SPI3_Config(void);
extern uint8_t SPI3_SendByte(uint8_t byte);
extern uint8_t SPI3_ReceiveByte(void);

#endif

