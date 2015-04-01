#include "spi1.h"

static void SPI1_GPIO_Config(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(SPI1_RCC_APB2_PORT|RCC_APB2Periph_SPI1, ENABLE);

#ifdef   SPI1_NONREMAP
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
#else
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SPI1, ENABLE);

#endif
	
	GPIO_InitStructure.GPIO_Pin = SPI1_IRQ_PIN;
	GPIO_InitStructure.GPIO_Mode = SPI1_IRQ_MODE;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI1_IRQ_PORT, &GPIO_InitStructure);
		
	GPIO_InitStructure.GPIO_Pin = SPI1_RST_PIN;
	GPIO_InitStructure.GPIO_Mode = SPI1_RST_MODE;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI1_RST_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = SPI1_SCS_PIN;
	GPIO_InitStructure.GPIO_Mode = SPI1_SCS_MODE;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI1_SCS_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SPI1_SCLK_PIN;
	GPIO_InitStructure.GPIO_Mode = SPI1_SCLK_MODE;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI1_SCLK_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SPI1_MISO_PIN;
	GPIO_InitStructure.GPIO_Mode = SPI1_MISO_MODE;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI1_MISO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SPI1_MOSI_PIN;
	GPIO_InitStructure.GPIO_Mode = SPI1_MOSI_MODE;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI1_MOSI_PORT, &GPIO_InitStructure);
}

void SPI1_Init(void) {
	SPI_InitTypeDef SPI_InitStructure;

	SPI1_GPIO_Config();

	/* SPI Config -------------------------------------------------------------*/
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; //空闲为低
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; //第一个跳变沿采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; //高位在前
	SPI_InitStructure.SPI_CRCPolynomial = 7;

	SPI_Init(SPI1, &SPI_InitStructure);

	/* Enable SPI */
#if !defined (SPI1_DMA)
	SPI_Cmd(SPI1, ENABLE);
#endif

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
		;
}

//外部接口函数

uint8_t SPI1_SendByte(uint8_t byte) {
	uint8_t i = 0;
	//等待发送信号寄存器为非空，然后发送一个字节到spi总线上
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
		;

	SPI_I2S_SendData(SPI1, byte);
	//等待接收信号寄存器为非空，然后从spi总线上接收一个字节
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) {
		if (i++ > 0xF0)
			break;
	}

	return SPI_I2S_ReceiveData(SPI1);
}

uint8_t SPI1_ReceiveByte(void) {
	uint8_t i = 0;
	//时序同发生字节一样，只是不返回读取的字节
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
		;

	SPI_I2S_SendData(SPI1, 0);
	//等待接收信号寄存器为非空，然后从spi总线上接收一个字节
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) {
		if (i++ > 0xF0)
			break;
	}

	return SPI_I2S_ReceiveData(SPI1);
}

void SPI1_ResetNSS(void) {
	SPI1_SCS_LOW();
}
void SPI1_SetNSS(void) {
	SPI1_SCS_HIGH();
}

void SPI1_ResetRST(void) {
	SPI1_RST_LOW();
}
void SPI1_SetRST(void) {
	SPI1_RST_HIGH();
}
uint8_t SPI1_readIRQ(void){
	return SPI1_READ_IRQ();
}


void SPI1_IRQHandler() {
#ifdef DEBUG
	printf("SPI1_IRQHandler \r\n");
#endif
	while (1) {
		NVIC_SystemReset();
	}
}
