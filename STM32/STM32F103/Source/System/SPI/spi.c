/*
 * SPIsimulate.c
 *
 *  Created on: 2015-1-20
 *      Author: star
 */

#include "spi.h"


static void SPI_GPIO_Config(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOD, ENABLE);

	GPIO_InitStructure.GPIO_Pin = SPI_IRQ_PIN;
	GPIO_InitStructure.GPIO_Mode = SPI_IRQ_MODE;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_IRQ_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SPI_CEN_PIN;
	GPIO_InitStructure.GPIO_Mode = SPI_CEN_MODE;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_CEN_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SPI_CSN_PIN;
	GPIO_InitStructure.GPIO_Mode = SPI_CSN_MODE;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_CSN_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SPI_CLK_PIN;
	GPIO_InitStructure.GPIO_Mode = SPI_CLK_MODE;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_CLK_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SPI_MISO_PIN;
	GPIO_InitStructure.GPIO_Mode = SPI_MISO_MODE;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_MISO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SPI_MOSI_PIN;
	GPIO_InitStructure.GPIO_Mode = SPI_MOSI_MODE;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_MOSI_PORT, &GPIO_InitStructure);
}

/****************************************************************************
【功能说明】SPI接口IO初始化
****************************************************************************/
void SPI_INIT( void )
{
	 SPI_GPIO_Config();
   SPI_MOSI_LOW();		//SI put 0
   SPI_CLK_LOW();		//SCK put 0
   SPI_DELAY();
   SPI_CSN_HIGH() ;
   SPI_DELAY();
}

/****************************************************************************
【功能说明】SPI接口读出写入数据 MSB 上升沿读取
****************************************************************************/
uint8_t SPI_ReadWrite(uint8_t data)
{
	uint8_t m ;

	SPI_CLK_LOW();
//	SPI_MOSI_LOW();
	SPI_CSN_LOW();
	SPI_DELAY();
	for( m = 0; m < 8; m++ )
	{
		if( (data&0x80)==0x80 ){
			SPI_MOSI_HIGH();
		}else{
			SPI_MOSI_LOW();
		}
		SPI_DELAY();
		SPI_CLK_HIGH() ;
		
		data = data<<1;
		if( SPI_MISO_READ() != 0 ){
			data |= 0x01 ;
		}else{
			data &= 0xfe;
		}

		SPI_DELAY();
		SPI_CLK_LOW() ;
		
	}

	return (data);
}

