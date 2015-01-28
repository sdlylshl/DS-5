/*
 * SPIsimulate.c
 *
 *  Created on: 2015-1-20
 *      Author: star
 */

#include "spi.h"

  void SPI_CEN_HIGH(){SPI_CE = 1;}
  void SPI_CEN_LOW() {SPI_CE = 0;}
  void SPI_CSN_HIGH(){SPI_CS = 1;}
  void SPI_CSN_LOW() {SPI_CS = 0;}

   void SPI_CLK_HIGH(){SPI_CLK = 1;}
  void SPI_CLK_LOW() {SPI_CLK = 0;}
  void SPI_MOSI_HIGH(){SPI_MOSI = 1;}
  void SPI_MOSI_LOW() {SPI_MOSI = 0;}

  uint8_t SPI_MISO_READ(){return SPI_MISO();}

  uint8_t SPI_IRQ_READ(){ return SPI_IRQ();}


//19 RA0(ICSPDAT)   --- CE
//18 RA1(ICSPCLK)  ---- CS
//17 RA2(INT)      ---- IRQ
//4 RA3(MCLR)      ---- MISO
//3 RA4(OSC_CLKOUT) --- CLK
//2 RA5(OSC_CLKIN)  ---MOSI

/****************************************************************************
【功能说明】SPI接口IO初始化
****************************************************************************/
void SPI_INIT( void )
{   //上拉
    WPUDAbits.WPUDA2 =1;
    WDAbits.WDA2 =1;
    //使能电平变换中断
    IOCAbits.IOCA2 = 1;

    TRISAbits.TRISA0 =0;
    TRISAbits.TRISA1 =0;
    TRISAbits.TRISA2 =1;
    TRISAbits.TRISA3 =1;
    TRISAbits.TRISA4 =0;
    TRISAbits.TRISA5 =0;

    PORTAbits.RA0 =0;

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

