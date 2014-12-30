#ifndef __USART1_H
#define	__USART1_H

//#include "..\System_config.h"
#include "version.h"

//DMA Mode
#define USART1_DR_Base  0x40013804
//DMA Buff
#define USART1_SENDBUFF_SIZE 1000
extern uint8_t USART1_SendBuff[USART1_SENDBUFF_SIZE];

//USART1
#define USART1_NVIC
//#define USART1_DMA

#define USART1_NONREMAP

#ifdef USART1_NONREMAP
//USART1_TX PA9
#define USART1_TX_PORT			GPIOA			
#define USART1_TX_PIN			GPIO_Pin_9
#define USART1_TX_MODE			GPIO_Mode_AF_PP	

//USART1_RX PA10
#define USART1_RX_PORT			GPIOA			
#define USART1_RX_PIN			GPIO_Pin_10
#define USART1_RX_MODE			GPIO_Mode_IN_FLOATING

#else

//USART1_TX PB6
#define USART1_TX_PORT			GPIOB
#define USART1_TX_PIN			GPIO_Pin_6
#define USART1_TX_MODE			GPIO_Mode_AF_PP
//USART1_RX PB7
#define USART1_RX_PORT			GPIOB
#define USART1_RX_PIN			GPIO_Pin_7
#define USART1_RX_MODE			GPIO_Mode_IN_FLOATING	

#endif


extern void USART1_Config(void);
extern void USART1_printf(char *Data, ...);

#endif /* __USART1_H */
