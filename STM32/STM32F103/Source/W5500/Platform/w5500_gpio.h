#ifndef _W5500_GPIO_H_
#define _W5500_GPIO_H_

#include "stm32f10x.h"

#define CABINET_HOST

//#define  W5500_RCC_AHB_PORT	 (RCC_AHBPeriph_DMA1)
//#define  W5500_RCC_APB1_PORT (RCC_APB1Periph_SPI2)
//#define  W5500_RCC_APB2_PORT (RCC_APB2Periph_GPIOA)
															
//Cabinet Device SPI1复用|SPI3															
#ifdef CABINET_DEVICE		
//#define WIZ_SPI							SPI1
#define  W5500_RCC_APB2_PORT (RCC_APB2Periph_GPIOB)

#define WIZ_RDY_PORT				GPIOB
#define WIZ_RDY_PIN					GPIO_Pin_0
#define WIZ_RDY_MODE				GPIO_Mode_IPU		
	
#define WIZ_nRST_PORT				GPIOB
#define WIZ_nRST_PIN				GPIO_Pin_1		
#define WIZ_nRST_MODE				GPIO_Mode_Out_PP

#define WIZ_SCS_PORT				GPIOA
#define WIZ_SCS_PIN					GPIO_Pin_4
#define WIZ_SCS_MODE				GPIO_Mode_Out_PP	
#endif 
//Cabinet Host SPI1
#ifdef CABINET_HOST	
//#define WIZ_SPI							SPI1
#define W5500_RCC_APB2_PORT (RCC_APB2Periph_GPIOA)

#define WIZ_RDY_PORT				GPIOA
#define WIZ_RDY_PIN					GPIO_Pin_1
#define WIZ_RDY_MODE				GPIO_Mode_IPU		
	
#define WIZ_nRST_PORT				GPIOA
#define WIZ_nRST_PIN				GPIO_Pin_0		
#define WIZ_nRST_MODE				GPIO_Mode_Out_PP

#define WIZ_SCS_PORT				GPIOA
#define WIZ_SCS_PIN					GPIO_Pin_4
#define WIZ_SCS_MODE				GPIO_Mode_Out_PP	
#endif
			
#if 0		//社会安防接口
//#define WIZ_SPI							SPI1

#define  W5500_RCC_APB2_PORT (RCC_APB2Periph_GPIOD)

#define WIZ_RDY_PORT				GPIOD
#define WIZ_RDY_PIN					GPIO_Pin_8
#define WIZ_RDY_MODE				GPIO_Mode_IPU		
	
#define WIZ_nRST_PORT				GPIOD
#define WIZ_nRST_PIN				GPIO_Pin_9		
#define WIZ_nRST_MODE				GPIO_Mode_Out_PP

#define WIZ_SCS_PORT				GPIOA
#define WIZ_SCS_PIN					GPIO_Pin_4
#define WIZ_SCS_MODE				GPIO_Mode_Out_PP	
#endif

//#define WIZ_RDY_HIGH()         GPIO_SetBits(WIZ_RDY_PORT, WIZ_RDY_PIN)
//#define WIZ_RDY_LOW()          GPIO_ResetBits(WIZ_RDY_PORT, WIZ_RDY_PIN)
//#define WIZ_RDY(x)             x ? GPIO_SetBits(WIZ_RDY_PORT,WIZ_RDY_PIN): GPIO_ResetBits(WIZ_RDY_PORT,WIZ_RDY_PIN)
#define WIZ_RDY_READ()         GPIO_ReadInputDataBit(WIZ_RDY_PORT, WIZ_RDY_PIN)

#define WIZ_nRST_HIGH()        GPIO_SetBits(WIZ_nRST_PORT, WIZ_nRST_PIN)
#define WIZ_nRST_LOW()         GPIO_ResetBits(WIZ_nRST_PORT, WIZ_nRST_PIN)
#define WIZ_nRST(x)            x ? GPIO_SetBits(WIZ_nRST_PORT,WIZ_nRST_PIN): GPIO_ResetBits(WIZ_nRST_PORT,WIZ_nRST_PIN)

#define WIZ_SCS_HIGH()         GPIO_SetBits(WIZ_SCS_PORT, WIZ_SCS_PIN)
#define WIZ_SCS_LOW()          GPIO_ResetBits(WIZ_SCS_PORT, WIZ_SCS_PIN)
#define WIZ_SCS(x)             x ? GPIO_SetBits(WIZ_SCS_PORT,WIZ_SCS_PIN): GPIO_ResetBits(WIZ_SCS_PORT,WIZ_SCS_PIN)

//*************************接口函数*********************


//#define wizchip_select SPI1_SCS_LOW();
//#define wizchip_deselect SPI1_SCS_HIGH();

#define wizchip_spi_init SPI1_Init
#define wizchip_write_byte SPI1_SendByte
#define wizchip_read_byte SPI1_ReceiveByte


void wizchip_select(void);	
void wizchip_deselect(void);												
void WIZ_GPIO_Configuration(void);

#endif
