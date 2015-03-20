#ifndef __USART2_H
#define	__USART2_H

#include "version.h"



//中断接收缓冲区
extern volatile uint8_t USART2_read;
extern volatile uint8_t USART2_write;
#define USART2_BUFF_SIZE 256
#define USART2_DMA_RECVBUFF_SIZE 256
extern uint8_t USART2_ReciveBuff[USART2_BUFF_SIZE];
#define USART2_DMA_SENDBUFF_SIZE 256
extern uint8_t USART2_SendBuff[USART2_BUFF_SIZE];

#define USART2_DR_Base  (USART2_BASE + 4)

//重映射
#define USART2_NONREMAP
//#define USART2_FULLREMAP

//开启USART2中断
#define USART2_NVIC
//开启DMA传输
//#define USART2_DMA_NVIC

#ifdef USART2_NONREMAP
// USART2_REMAP = 00
#define USART2_CTS_PORT			GPIOA
#define USART2_CTS_PIN			GPIO_Pin_0
#define USART2_CTS_MODE			GPIO_Mode_AF_PP

#define USART2_RTS_PORT			GPIOA
#define USART2_RTS_PIN			GPIO_Pin_1
#define USART2_RTS_MODE			GPIO_Mode_AF_PP

#define USART2_TX_PORT			GPIOA
#define USART2_TX_PIN				GPIO_Pin_2
#define USART2_TX_MODE			GPIO_Mode_AF_PP

#define USART2_RX_PORT			GPIOA
#define USART2_RX_PIN				GPIO_Pin_3
#define USART2_RX_MODE			GPIO_Mode_IN_FLOATING

#define USART2_CK_PORT			GPIOA
#define USART2_CK_PIN				GPIO_Pin_4
#define USART2_CK_MODE			GPIO_Mode_AF_PP
#endif


#ifdef USART2_FULLREMAP
//USART2_REMAP = 11
#define USART2_CTS_PORT			GPIOD
#define USART2_CTS_PIN			GPIO_Pin_3
#define USART2_CTS_MODE			GPIO_Mode_AF_PP

#define USART2_RTS_PORT			GPIOD
#define USART2_RTS_PIN			GPIO_Pin_4
#define USART2_RTS_MODE			GPIO_Mode_AF_PP

#define USART2_TX_PORT			GPIOD
#define USART2_TX_PIN				GPIO_Pin_5
#define USART2_TX_MODE			GPIO_Mode_AF_PP

#define USART2_RX_PORT			GPIOD
#define USART2_RX_PIN				GPIO_Pin_6
#define USART2_RX_MODE			GPIO_Mode_IN_FLOATING

#define USART2_CK_PORT			GPIOD
#define USART2_CK_PIN				GPIO_Pin_7
#define USART2_CK_MODE			GPIO_Mode_AF_PP
#endif


extern void USART2_Init(void);
extern void USART2_IRQHandler(void);
#endif /* __USART2_H */
