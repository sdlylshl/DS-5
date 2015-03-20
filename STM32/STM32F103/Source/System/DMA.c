//#include "System_config.h"
#include "version.h"
// 外设    通道1     		通道2     	通道3     	通道4       通道5       通道6        通道7 
// ADC     ADC1       
// SPI                 SPI1_RX     SPI1_TX     SPI2_RX     SPI2_TX   
// USART               USART3_TX   USART3_RX   USART1_TX   USART1_RX   USART2_RX   USART2_TX
// I2C                                         I2C2_TX     I2C2_RX     I2C1_TX     I2C1_RX 
// TIM1                TIM1_CH1    TIM1_CH2    TIM1_TX4    TIM1_UP     TIM1_CH3
//                                             TIM1_TRIG
//                                             TIM1_COM      
// TIM2    TIM2_CH3    TIM2_UP                             TIM2_CH1                TIM2_CH2 
//                                                                                 TIM2_CH4 
// TIM3                TIM3_CH3    TIM3_CH4                            TIM3_CH1
//                                 TIM3_UP                             TIM3_TRIG 
// TIM4    TIM4_CH1                            TIM4_CH2    TIM4_CH3                TIM4_UP

//DMA Mode
//#define USART1_DR_Base  0x40013804
#define SPI1_DR_Address    (SPI1_BASE + 0x0C)
#define SPI2_DR_Address    (SPI2_BASE + 0x0C)

#define SENDBUFF_SIZE 1000
//DMA Buff
//extern uint8_t SendBuff[SENDBUFF_SIZE];

//PeripheralBaseAddr:外设地址  &USART1->DR
//MemoryBaseAddr:存储器地址 
//BufferSize:数据传输量 
void DMA_Config(DMA_Channel_TypeDef* DMA_CHx, uint32_t PeripheralBaseAddr,
		uint32_t MemoryBaseAddr, uint32_t BufferSize) {
	DMA_InitTypeDef DMA_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //开启DMA时钟
	//NVIC_Config();	   			//配置DMA中断

	DMA_DeInit(DMA_CHx); //将DMA 的通道1 寄存器重设为缺省值 
	/*设置DMA源：内存地址&串口数据寄存器地址*/
	DMA_InitStructure.DMA_PeripheralBaseAddr = PeripheralBaseAddr;
	/*内存地址(要传输的变量的指针)*/
	DMA_InitStructure.DMA_MemoryBaseAddr = MemoryBaseAddr;

	/*传输方向：Memory --> Peripheral*/
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;

	/*传输大小DMA_BufferSize=SENDBUFF_SIZE*/
	DMA_InitStructure.DMA_BufferSize = BufferSize;

	//外设地址不增	内存地址自增
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	//数据单位--字节8bit
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	/*DMA模式：一次传输，循环*/
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	/*优先级：中*/
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	/*禁止内存到内存的传输	*/
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

	/*配置DMA1通道*/
	DMA_Init(DMA_CHx, &DMA_InitStructure);

}

//  USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE); //使能串口1的 DMA发送

//开启一次DMA传输 
void DMA_Enable(DMA_Channel_TypeDef*DMA_CHx) {
	DMA_Cmd(DMA_CHx, DISABLE);
	DMA_SetCurrDataCounter(DMA_CHx, SENDBUFF_SIZE); //设置  DMA缓存的大小 
	DMA_Cmd(DMA_CHx, ENABLE); //使能DMA 通道   
}

//开启DMA传输,传输完成后中断 
void DMA_NVIC_Enable(DMA_Channel_TypeDef*DMA_CHx) {
	DMA_Cmd(DMA_CHx, DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel4, SENDBUFF_SIZE); //设置  DMA缓存的大小 
	DMA_Cmd(DMA_CHx, ENABLE); //使能DMA 通道   	

	DMA_ITConfig(DMA_CHx, DMA_IT_TC, ENABLE); //配置DMA发送完成后产生中断
}

