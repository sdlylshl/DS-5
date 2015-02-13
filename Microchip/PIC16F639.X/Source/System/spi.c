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
  void SPI_CSN_LOW(void) {SPI_CS = 0;}

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
{
//  RA2 上拉
    OPTION_REGbits.nRAPU = 0;  //启用PORTA上下拉使能
    WDAbits.WDA2 =1;    //选择上拉
    WPUDAbits.WPUDA2 =1; // 1使能上下拉
//  RA2外部中断配置
    OPTION_REGbits.INTEDG =0;   //RA2下降沿触发中断
    INTCONbits.INTE = 1;    //启用RA2为中断

    TRISAbits.TRISA0 =0;    //CE
    TRISAbits.TRISA1 =0;    //CS
    TRISAbits.TRISA2 =1;    //IRQ
    TRISAbits.TRISA3 =1;    //MISO
    TRISAbits.TRISA4 =0;    //CLK
    TRISAbits.TRISA5 =0;    //MOSI
//测试
//SPI_MOSI_LOW();		//MOSI put 0
//SPI_CLK_LOW();		//CLK put 0
//SPI_CEN_LOW();
//SPI_CSN_LOW();
//SPI_MOSI_HIGH();		//MOSI put 0
//SPI_CLK_HIGH();		//CLK put 0
//SPI_CEN_HIGH();
//SPI_CSN_HIGH();
//   while(1);

    //PORTAbits.RA0 =0;
   SPI_CEN_HIGH();
   SPI_MOSI_LOW();		//MOSI put 0
   SPI_CLK_LOW();		//CLK put 0
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

