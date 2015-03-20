#include "usart2.h"

//中断接收缓冲区
volatile uint8_t USART2_read=0;
volatile uint8_t USART2_write=0;
uint8_t USART2_SendBuff[USART2_BUFF_SIZE];
uint8_t USART2_ReciveBuff[USART2_BUFF_SIZE];

void USART2_DMA_NVIC_Config(void) {
	//TX
	NVIC_SetPriority(DMA1_Channel6_IRQn, 8);
	NVIC_EnableIRQ(DMA1_Channel6_IRQn);	
	//RX
	NVIC_SetPriority(DMA1_Channel7_IRQn, 8);
	NVIC_EnableIRQ(DMA1_Channel7_IRQn);
}
void USART2_DMA_TX(uint8_t sendsize) {
	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //开启DMA时钟
	//USART2_DMA_NVIC_Config(); //配置DMA中断
	//开启 USART2_TX DMA
	NVIC_SetPriority(DMA1_Channel7_IRQn, 8);
	NVIC_EnableIRQ(DMA1_Channel7_IRQn);	
	
	/*设置DMA源：内存地址&串口数据寄存器地址*/
	DMA_InitStructure.DMA_PeripheralBaseAddr = USART2_DR_Base;
	/*内存地址(要传输的变量的指针)*/
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) USART2_SendBuff;
	/*方向：从内存到外设*/
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	/*传输大小DMA_BufferSize=SENDBUFF_SIZE*/
	DMA_InitStructure.DMA_BufferSize = sendsize;//USART2_DMA_SENDBUFF_SIZE;
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
	DMA_Init(DMA1_Channel7, &DMA_InitStructure);
	DMA_Cmd(DMA1_Channel7, ENABLE); //使能DMA
	DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, ENABLE); //配置DMA发送完成后产生中断

}
void DMA1_Channel7_IRQ_Handle(){
		//传输完成
}
void USART2_DMA_RX(uint8_t recvsize) {
	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //开启DMA时钟
	//USART2_DMA_NVIC_Config(); //配置DMA中断
	//开启 USART2_TX DMA
	NVIC_SetPriority(DMA1_Channel6_IRQn, 8);
	NVIC_EnableIRQ(DMA1_Channel6_IRQn);	
	
	/*设置DMA源：内存地址&串口数据寄存器地址*/
	DMA_InitStructure.DMA_PeripheralBaseAddr = USART2_DR_Base;
	/*内存地址(要传输的变量的指针)*/
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) USART2_ReciveBuff;
	/*方向：从外设到内存*/
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	/*传输大小DMA_BufferSize=SENDBUFF_SIZE*/
	DMA_InitStructure.DMA_BufferSize = recvsize;//USART2_DMA_SENDBUFF_SIZE;
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
	DMA_Init(DMA1_Channel6, &DMA_InitStructure);
	DMA_Cmd(DMA1_Channel6, ENABLE); //使能DMA
	DMA_ITConfig(DMA1_Channel6, DMA_IT_TC, ENABLE); //配置DMA发送完成后产生中断

}
void DMA1_Channel6_IRQ_Handle(){
		//传输完成
}
void USART2_NVIC_Config() {

	NVIC_SetPriority(USART2_IRQn, 4);
	NVIC_EnableIRQ(USART2_IRQn);
}

void USART2_GPIO_Config(void) {

	GPIO_InitTypeDef GPIO_InitStructure;
	/* config USART2 clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
#ifdef USART2_NONREMAP
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
#endif

#ifdef USART2_FULLREMAP
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);
#endif

	/* USART2 GPIO config */
	GPIO_InitStructure.GPIO_Pin = USART2_TX_PIN;
	GPIO_InitStructure.GPIO_Mode = USART2_TX_MODE; //  GPIO_Mode_AF_PP
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART2_TX_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = USART2_RX_PIN;
	GPIO_InitStructure.GPIO_Mode = USART2_RX_MODE; //GPIO_Mode_IN_FLOATING
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART2_RX_PORT, &GPIO_InitStructure);

}
void USART2_Init(void) {
	USART_InitTypeDef USART_InitStructure;
	USART2_GPIO_Config();
	/* USART2 mode config */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =
			USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);

#ifdef USART2_NVIC
	USART2_NVIC_Config();
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
#endif

#ifdef USART2_DMA_NVIC
	USART2_DMA_Config();
#endif
	USART_Cmd(USART2, ENABLE);
	USART_ClearFlag(USART2, USART_FLAG_TC);
}

//接收中断
void USART2_IRQHandler(void) {
	
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) {
		
		USART2_ReciveBuff[USART2_write++]=USART2->DR;
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);		
	}
}
