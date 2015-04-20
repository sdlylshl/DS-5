#ifndef __SPI2_H
#define __SPI2_H

#include "stm32f10x.h"

//#define SPI2_DMA

//#if 1		//Societ 
//#define  SPI2_RCC_APB2_PORT (RCC_APB2Periph_GPIOD)

//#define SPI2_IRQ_PORT				GPIOD
//#define SPI2_IRQ_PIN				GPIO_Pin_8
//#define SPI2_IRQ_MODE				GPIO_Mode_IPU		
//    
//#define SPI2_RST_PORT				GPIOD
//#define SPI2_RST_PIN				GPIO_Pin_9		
//#define SPI2_RST_MODE				GPIO_Mode_Out_PP
//#endif

// SPI2 PB 12 13 14 15
#define SPI2_SCS_PORT				GPIOB
#define SPI2_SCS_PIN				GPIO_Pin_12
#define SPI2_SCS_MODE				GPIO_Mode_Out_PP	//此处一定要PP输出否则程序不正常

#define SPI2_SCLK_PORT			GPIOB
#define SPI2_SCLK_PIN				GPIO_Pin_13
#define SPI2_SCLK_MODE			GPIO_Mode_AF_PP

#define SPI2_MISO_PORT			GPIOB
#define SPI2_MISO_PIN				GPIO_Pin_14
#define SPI2_MISO_MODE			GPIO_Mode_AF_PP

#define SPI2_MOSI_PORT			GPIOB
#define SPI2_MOSI_PIN			  GPIO_Pin_15
#define SPI2_MOSI_MODE			GPIO_Mode_AF_PP


#define SPI2_SCS_HIGH()     GPIO_SetBits(SPI2_SCS_PORT, SPI2_SCS_PIN)
#define SPI2_SCS_LOW()			GPIO_ResetBits(SPI2_SCS_PORT, SPI2_SCS_PIN)
#define SPI2_SCS(x)   			x ? GPIO_SetBits(SPI2_SCS_PORT,SPI2_SCS_PIN): GPIO_ResetBits(SPI2_SCS_PORT,SPI2_SCS_PIN)

//#define SPI2_RST_HIGH()     GPIO_SetBits(SPI2_RST_PORT, SPI2_RST_PIN)
//#define SPI2_RST_LOW()			GPIO_ResetBits(SPI2_RST_PORT, SPI2_RST_PIN)
//#define SPI2_RST(x)   			x ? GPIO_SetBits(SPI2_RST_PORT,SPI2_RST_PIN): GPIO_ResetBits(SPI2_RST_PORT,SPI2_RST_PIN)

//#define	SPI2_READ_IRQ()   	GPIO_ReadInputDataBit(SPI2_IRQ_PORT, SPI2_IRQ_PIN)

void SPI2_Init(void);
//外部接口函数
uint8_t SPI2_SendByte(uint8_t byte);
uint8_t SPI2_ReceiveByte(void);
void SPI2_SetNSS(void);
void SPI2_ResetNSS(void);
//void SPI2_SetRST(void);
////void SPI2_ResetRST(void);
//uint8_t SPI2_readIRQ(void);

#endif

