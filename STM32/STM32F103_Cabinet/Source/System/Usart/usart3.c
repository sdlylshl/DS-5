#include "usart3.h"

//DMA Buff
uint8_t Usart3_SendBuff[SENDBUFF_SIZE];
//中断接收缓冲区
uint8_t USART3_write=0;
uint8_t USART3_REVC_BUF[USART3_REVC_BUF_SIZE];

void USART3_DMA_NVIC_Config(void) {
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure one bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	/* 配置P[A|B|C|D|E]0为中断源 */
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
void USART3_DMA_Config(void) {
	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //开启DMA时钟
	USART3_DMA_NVIC_Config(); //配置DMA中断

	/*设置DMA源：内存地址&串口数据寄存器地址*/
	//DMA_InitStructure.DMA_PeripheralBaseAddr = USART3_DR_Base;
	/*内存地址(要传输的变量的指针)*/
	//DMA_InitStructure.DMA_MemoryBaseAddr = (u32) SendBuff;
	/*方向：从内存到外设*/
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;

	/*传输大小DMA_BufferSize=SENDBUFF_SIZE*/
	DMA_InitStructure.DMA_BufferSize = SENDBUFF_SIZE;

	/*外设地址不增*/
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

	/*内存地址自增*/
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;

	/*外设数据单位*/
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;

	/*内存数据单位 8bit*/
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;

	/*DMA模式：一次传输，循环*/
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;

	/*优先级：中*/
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;

	/*禁止内存到内存的传输	*/
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

	/*配置DMA1的4通道*/
	DMA_Init(DMA1_Channel4, &DMA_InitStructure);

	DMA_Cmd(DMA1_Channel4, ENABLE); //使能DMA
	DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE); //配置DMA发送完成后产生中断

}

void USART3_NVIC_Config() {

	NVIC_SetPriority(USART3_IRQn, 4);
	NVIC_EnableIRQ(USART3_IRQn);
}

void USART3_GPIO_Config(void) {

	GPIO_InitTypeDef GPIO_InitStructure;
	/* config USART3 clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
#ifdef USART3_NONREMAP
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
#endif

#ifdef USART3_PARTIALREMAP
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_PartialRemap_USART3, ENABLE);
#endif

#ifdef USART3_FULLREMAP
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_FullRemap_USART3, ENABLE);
#endif

	/* USART3 GPIO config */
	GPIO_InitStructure.GPIO_Pin = USART3_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = USART3_TX_MODE; //  GPIO_Mode_AF_PP
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART3_TX_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = USART3_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = USART3_RX_MODE; //GPIO_Mode_IN_FLOATING
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART3_RX_PORT, &GPIO_InitStructure);

}
void USART3_Config(void) {
	USART_InitTypeDef USART_InitStructure;
	USART3_GPIO_Config();
	/* USART3 mode config */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =
			USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);

#ifdef USART3_NVIC
	USART3_NVIC_Config();
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
#endif

#ifdef USART3_DMA_NVIC
	USART3_DMA_Config();
#endif
	USART_Cmd(USART3, ENABLE);
	USART_ClearFlag(USART3, USART_FLAG_TC);
}
