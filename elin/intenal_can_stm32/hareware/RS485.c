#include "RS485.h"

void RS485_init()
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB1Periph_USART2 | RCC_APB2Periph_GPIOA, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器USART1

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = RS485_RE_Pin;
	GPIO_InitStructure.GPIO_Mode = RS485_RE_GPIO_Mode;
	GPIO_Init(RS485_RE_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = RS485_TX_Pin;
	GPIO_InitStructure.GPIO_Mode = RS485_TX_GPIO_Mode;
	GPIO_Init(RS485_TX_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = RS485_RX_Pin;
	GPIO_InitStructure.GPIO_Mode = RS485_RX_GPIO_Mode;
	GPIO_Init(RS485_RX_GPIO, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 9600;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断1	 
	USART_Cmd(USART2, ENABLE);
}
