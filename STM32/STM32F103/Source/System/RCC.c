//#include "System_config.h"

#include "stm32f10x.h"
void RCC_Config(void) {
	RCC_DeInit();

//  RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2   , ENABLE);
//  RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3   , ENABLE);
//  RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM4   , ENABLE);
//  RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM5   , ENABLE);
//  RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM6   , ENABLE);
//  RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM7   , ENABLE);
//  RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM12  , ENABLE);
//  RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM13  , ENABLE);
//  RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM14  , ENABLE);
//  RCC_APB1PeriphClockCmd( RCC_APB1Periph_WWDG   , ENABLE);
//  RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI2   , ENABLE);
//  RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI3   , ENABLE);
//  RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2 , ENABLE);
//  RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART3 , ENABLE);
//  RCC_APB1PeriphClockCmd( RCC_APB1Periph_UART4  , ENABLE);
//  RCC_APB1PeriphClockCmd( RCC_APB1Periph_UART5  , ENABLE);
//  RCC_APB1PeriphClockCmd( RCC_APB1Periph_I2C1   , ENABLE);
//  RCC_APB1PeriphClockCmd( RCC_APB1Periph_I2C2   , ENABLE);
//  RCC_APB1PeriphClockCmd( RCC_APB1Periph_USB    , ENABLE);
//  RCC_APB1PeriphClockCmd( RCC_APB1Periph_CAN1   , ENABLE);
//  RCC_APB1PeriphClockCmd( RCC_APB1Periph_CAN2   , ENABLE);
//  RCC_APB1PeriphClockCmd( RCC_APB1Periph_BKP    , ENABLE);
//  RCC_APB1PeriphClockCmd( RCC_APB1Periph_PWR    , ENABLE);
//  RCC_APB1PeriphClockCmd( RCC_APB1Periph_DAC    , ENABLE);
//  RCC_APB1PeriphClockCmd( RCC_APB1Periph_CEC    , ENABLE);
//	
//  RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO   , ENABLE);
//  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA  , ENABLE);
//  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB  , ENABLE);
//  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC  , ENABLE);
//  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD  , ENABLE);
//  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOE  , ENABLE);
//  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOF  , ENABLE);
//  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOG  , ENABLE);
//  RCC_APB2PeriphClockCmd( RCC_APB2Periph_ADC1   , ENABLE);
//  RCC_APB2PeriphClockCmd( RCC_APB2Periph_ADC2   , ENABLE);
//  RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM1   , ENABLE);
//  RCC_APB2PeriphClockCmd( RCC_APB2Periph_SPI1   , ENABLE);
//  RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM8   , ENABLE);
//  RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1 , ENABLE);
//  RCC_APB2PeriphClockCmd( RCC_APB2Periph_ADC3   , ENABLE);
//  RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM15  , ENABLE);
//  RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM16  , ENABLE);
//  RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM17  , ENABLE);
//  RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM9   , ENABLE);
//  RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM10  , ENABLE);
//  RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM11  , ENABLE);
//  
//  RCC_AHBPeriphClockCmd(  RCC_AHBPeriph_DMA1    , ENABLE);
//  RCC_AHBPeriphClockCmd(  RCC_AHBPeriph_DMA2    , ENABLE);
//  RCC_AHBPeriphClockCmd(  RCC_AHBPeriph_SRAM    , ENABLE);
//  RCC_AHBPeriphClockCmd(  RCC_AHBPeriph_FLITF   , ENABLE);
//  RCC_AHBPeriphClockCmd(  RCC_AHBPeriph_CRC     , ENABLE);  
#ifndef STM32F10X_CL  
//  RCC_AHBPeriphClockCmd( RCC_AHBPeriph_FSMC     , ENABLE);
//  RCC_AHBPeriphClockCmd( RCC_AHBPeriph_SDIO     , ENABLE);  
#else
//  RCC_AHBPeriphClockCmd( RCC_AHBPeriph_OTG_FS   , ENABLE);
//  RCC_AHBPeriphClockCmd( RCC_AHBPeriph_ETH_MAC  , ENABLE);
//  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ETH_MAC_Tx, ENABLE);
//  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_ETH_MAC_Rx, ENABLE);
#endif /* STM32F10X_CL */

}
