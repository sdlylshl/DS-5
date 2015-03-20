#ifndef __USART3_H
#define	__USART3_H

#include "version.h"


//中断接收缓冲区
extern volatile uint8_t USART3_write;

#define USART3_BUFF_SIZE 256
#define USART3_DMA_RECVBUFF_SIZE 256
extern uint8_t USART3_ReciveBuff[USART3_BUFF_SIZE];
#define USART3_DMA_SENDBUFF_SIZE 256
extern uint8_t USART3_SendBuff[USART3_BUFF_SIZE];

//DMA Mode
#define USART3_DR_Base  (USART3_BASE + 4)
#define SENDBUFF_SIZE 1000
//DMA Buff
extern uint8_t Usart3_SendBuff[SENDBUFF_SIZE];


//重映射
//#define USART3_NONREMAP
#define USART3_PARTIALREMAP
//#define USART3_FULLREMAP

//开启USART3中断
#define USART3_NVIC
//开启DMA传输
//#define USART3_DMA_NVIC

#ifdef USART3_NONREMAP
// USART3_REMAP = 00
#define USART3_TX_PORT			GPIOB
#define USART3_TX_PIN			GPIO_Pin_10
#define USART3_TX_MODE			GPIO_Mode_AF_PP

#define USART3_RX_PORT			GPIOB
#define USART3_RX_PIN			GPIO_Pin_11
#define USART3_RX_MODE			GPIO_Mode_IN_FLOATING
#endif

#ifdef USART3_PARTIALREMAP
//USART3_REMAP = 01
#define USART3_TX_PORT			GPIOC
#define USART3_TX_PIN			GPIO_Pin_10
#define USART3_TX_MODE			GPIO_Mode_AF_PP

#define USART3_RX_PORT			GPIOC
#define USART3_RX_PIN			GPIO_Pin_11
#define USART3_RX_MODE			GPIO_Mode_IN_FLOATING
#endif

#ifdef USART3_FULLREMAP
//USART3_REMAP = 11
#define USART3_TX_PORT			GPIOD
#define USART3_TX_PIN			GPIO_Pin_8
#define USART3_TX_MODE			GPIO_Mode_AF_PP

#define USART3_RX_PORT			GPIOD
#define USART3_RX_PIN			GPIO_Pin_9
#define USART3_RX_MODE			GPIO_Mode_IN_FLOATING
#endif


extern void USART3_Init(void);
extern void USART3_IRQHandler(void);

#endif /* __USART3_H */
