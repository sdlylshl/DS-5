#include "spi2.h"

static void SPI2_GPIO_Config(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	RCC_APB2PeriphClockCmd(SPI2_RCC_APB2_PORT|RCC_APB2Periph_GPIOB, ENABLE);
#if 1
	GPIO_InitStructure.GPIO_Pin = SPI2_IRQ_PIN;
	GPIO_InitStructure.GPIO_Mode = SPI2_IRQ_MODE;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI2_IRQ_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SPI2_RST_PIN;
	GPIO_InitStructure.GPIO_Mode = SPI2_RST_MODE;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI2_RST_PORT, &GPIO_InitStructure);
#endif
	GPIO_InitStructure.GPIO_Pin = SPI2_SCS_PIN;
	GPIO_InitStructure.GPIO_Mode = SPI2_SCS_MODE;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI2_SCS_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SPI2_SCLK_PIN;
	GPIO_InitStructure.GPIO_Mode = SPI2_SCLK_MODE;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI2_SCLK_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SPI2_MISO_PIN;
	GPIO_InitStructure.GPIO_Mode = SPI2_MISO_MODE;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI2_MISO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SPI2_MOSI_PIN;
	GPIO_InitStructure.GPIO_Mode = SPI2_MOSI_MODE;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI2_MOSI_PORT, &GPIO_InitStructure);
}

void SPI2_Config(void) {
	SPI_InitTypeDef SPI_InitStructure;

	SPI2_GPIO_Config();

	/* SPI Config -------------------------------------------------------------*/
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;

	SPI_Init(SPI2, &SPI_InitStructure);

	/* Enable SPI */
#if !defined (SPI2_DMA)
	SPI_Cmd(SPI2, ENABLE);
#endif

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET)
		;
}

uint8_t SPI2_SendByte(uint8_t byte) {
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET)
		;

	SPI_I2S_SendData(SPI2, byte);

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)
		;

	return SPI_I2S_ReceiveData(SPI2);
}

uint8_t SPI2_ReceiveByte(void) {
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET)
		;

	SPI_I2S_SendData(SPI2, 0);

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)
		;

	return SPI_I2S_ReceiveData(SPI2);
}
