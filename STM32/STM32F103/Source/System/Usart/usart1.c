#include <stdio.h>
#include <stdarg.h>

#include "usart1.h"

//DMA Buff
volatile uint8_t USART1_write=0;
uint8_t USART1_SendBuff[USART1_BUFF_SIZE];
uint8_t USART1_ReciveBuff[USART1_BUFF_SIZE];

void USART1_NVIC_Config(uint8_t priority) {
	/* Configure the NVIC Preemption Priority Bits */
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	//NVIC_SetPriorityGrouping(0);
	/* Enable the USARTy Interrupt */

	//    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	//    NVIC_Init(&NVIC_InitStructure);
	NVIC_SetPriority(USART1_IRQn, priority);
	NVIC_EnableIRQ(USART1_IRQn);
}

static void USART1_GPIO_Config(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	//USART1_REMAP = 0 USART1_REMAP = 1 
	//USART1_TX PA9     PB6 
	//USART1_RX PA10     PB7   
#ifdef   USART1_NONREMAP
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
#else
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);
#endif  

	// Configure USART1 Tx (PA.9 PB.6)
	GPIO_InitStructure.GPIO_Pin = USART1_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = USART1_TX_MODE; //  GPIO_Mode_AF_PP      
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART1_TX_PORT, &GPIO_InitStructure);
	// Configure USART1 Rx (PA.10 PB.7)
	GPIO_InitStructure.GPIO_Pin = USART1_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = USART1_RX_MODE; //GPIO_Mode_IN_FLOATING      
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART1_RX_PORT, &GPIO_InitStructure);
}

void USART1_Init(void) {
	USART_InitTypeDef USART_InitStructure;

	USART1_GPIO_Config();
	/* USART1 mode config */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =
			USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	
#ifdef USART1_NVIC
	//使能USART1接收中断
	USART1_NVIC_Config(9);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
#endif

	USART_Cmd(USART1, ENABLE);
	USART_ClearFlag(USART1, USART_FLAG_TC);
}


void USART1_IRQHandler(void) {
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
		USART1_ReciveBuff[USART1_write++]=USART1->DR;
		//USART_SendData(USART1,USART1_ReciveBuff[USART1_write-1]);
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
	}
}
