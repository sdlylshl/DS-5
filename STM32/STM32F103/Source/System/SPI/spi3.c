#include "spi3.h"

static void SPI3_GPIO_Config(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
	RCC_APB2PeriphClockCmd(SPI3_RCC_APB2_PORT|RCC_APB2Periph_GPIOA, ENABLE);
	
	#ifdef SPI3_NONREMAP
		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	#else
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	#endif

	
	GPIO_InitStructure.GPIO_Pin = SPI3_IRQ_PIN;
	GPIO_InitStructure.GPIO_Mode = SPI3_IRQ_MODE;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI3_IRQ_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = SPI3_RST_PIN;
	GPIO_InitStructure.GPIO_Mode = SPI3_RST_MODE;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI3_RST_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = SPI3_SCS_PIN;
	GPIO_InitStructure.GPIO_Mode = SPI3_SCS_MODE;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI3_SCS_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SPI3_SCLK_PIN;
	GPIO_InitStructure.GPIO_Mode = SPI3_SCLK_MODE;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI3_SCLK_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SPI3_MISO_PIN;
	GPIO_InitStructure.GPIO_Mode = SPI3_MISO_MODE;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI3_MISO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SPI3_MOSI_PIN;
	GPIO_InitStructure.GPIO_Mode = SPI3_MOSI_MODE;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI3_MOSI_PORT, &GPIO_InitStructure);
}

void SPI3_Config(void) {
	SPI_InitTypeDef SPI_InitStructure;

	SPI3_GPIO_Config();

	/* SPI Config -------------------------------------------------------------*/
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	//SPI_BaudRatePrescaler_2   2分频   (SPI 36M@sys 72M)
	//SPI_BaudRatePrescaler_4   4分频   (SPI 18M@sys 72M)
	//SPI_BaudRatePrescaler_8   8分频   (SPI 9M@sys 72M)
	//SPI_BaudRatePrescaler_16  16分频  (SPI 4.5M@sys 72M)
	//SPI_BaudRatePrescaler_256 256分频 (SPI 281.25K@sys 72M)
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;

	SPI_Init(SPI3, &SPI_InitStructure);

	/* Enable SPI */
#if !defined (SPI3_DMA)
	SPI_Cmd(SPI3, ENABLE);
#endif

	while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET)
		;
}

uint8_t SPI3_SendByte(uint8_t byte) {
	while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET)
		;

	SPI_I2S_SendData(SPI3, byte);

	while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET)
		;

	return SPI_I2S_ReceiveData(SPI3);
}

uint8_t SPI3_ReceiveByte(void) {
	while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET)
		;

	SPI_I2S_SendData(SPI3, 0);

	while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET)
		;

	return SPI_I2S_ReceiveData(SPI3);
}
