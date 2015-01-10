#ifndef _GPIO_HANDLER_H_
#define _GPIO_HANDLER_H_
#include "version.h"
//#include "../System/System_config.h"
//#include "../Ethernet_Config.h"
#define  W5500_RCC_AHB_PORT	 (RCC_AHBPeriph_DMA1)
//#define  W5500_RCC_APB1_PORT (RCC_APB1Periph_SPI2)
//#define  W5500_RCC_APB2_PORT (RCC_APB2Periph_GPIOA)
															
															
#ifdef CABINET_DEVICE		//Cabinet Device SPI1复用|SPI3
#define  W5500_RCC_APB2_PORT (RCC_APB2Periph_GPIOB)

#define WIZ_RDY_PORT				GPIOB
#define WIZ_RDY_PIN					GPIO_Pin_0
#define WIZ_RDY_MODE				GPIO_Mode_IPU		
	
#define WIZ_nRST_PORT				GPIOB
#define WIZ_nRST_PIN				GPIO_Pin_1		
#define WIZ_nRST_MODE				GPIO_Mode_Out_PP

#else		//Cabinet Host SPI1
#define W5500_RCC_APB2_PORT (RCC_APB2Periph_GPIOA)

#define WIZ_RDY_PORT				GPIOA
#define WIZ_RDY_PIN					GPIO_Pin_1
#define WIZ_RDY_MODE				GPIO_Mode_IPU		
	
#define WIZ_nRST_PORT				GPIOA
#define WIZ_nRST_PIN				GPIO_Pin_0		
#define WIZ_nRST_MODE				GPIO_Mode_Out_PP
#endif
			
#if 0		//社会安防接口
#define  W5500_RCC_APB2_PORT (RCC_APB2Periph_GPIOD)

#define WIZ_RDY_PORT				GPIOD
#define WIZ_RDY_PIN					GPIO_Pin_8
#define WIZ_RDY_MODE				GPIO_Mode_IPU		
	
#define WIZ_nRST_PORT				GPIOD
#define WIZ_nRST_PIN				GPIO_Pin_9		
#define WIZ_nRST_MODE				GPIO_Mode_Out_PP
#endif


#define WIZ_SPI							SPI1
//  // SPI2 PB 12 13 14 15	

//#define WIZ_SCS_PORT				SPI1_SCS_PORT
//#define WIZ_SCS_PIN					SPI1_SCS_PIN
//#define WIZ_SCS_MODE				SPI1_SCS_MODE

//#define WIZ_SCLK_PORT				GPIOB
//#define WIZ_SCLK_PIN				GPIO_Pin_13
//#define WIZ_SCLK_MODE				GPIO_Mode_AF_PP	

//#define WIZ_MISO_PORT				GPIOB
//#define WIZ_MISO_PIN				GPIO_Pin_14	
//#define WIZ_MISO_MODE				GPIO_Mode_AF_PP

//#define WIZ_MOSI_PORT				GPIOB		
//#define WIZ_MOSI_PIN				GPIO_Pin_15	
//#define WIZ_MOSI_MODE				GPIO_Mode_AF_PP


#define	WIZ_nRST(x)   x ? GPIO_SetBits(WIZ_nRST_PORT,WIZ_nRST_PIN): GPIO_ResetBits(WIZ_nRST_PORT,WIZ_nRST_PIN)
#define	WIZ_SCS(x)   x ? GPIO_SetBits(WIZ_SCS_PORT,WIZ_SCS_PIN): GPIO_ResetBits(WIZ_SCS_PORT,WIZ_SCS_PIN)
													//uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
void WIZ_GPIO_Configuration(void);

#endif
