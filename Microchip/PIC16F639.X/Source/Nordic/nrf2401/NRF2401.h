

#ifndef __SPI_NRF_H
#define __SPI_NRF_H

#include "../common/hal_nrf_hw.h"


#define MAX_RT      0x10 //达到最大重发次数中断标志位
#define TX_DS		0x20 //发送完成中断标志位	  //

#define RX_DR		0x40 //接收到数据中断标志位

//#define NRF_CSN_HIGH()      GPIO_SetBits(GPIOA, GPIO_Pin_1)
//#define NRF_CSN_LOW()      GPIO_ResetBits(GPIOA, GPIO_Pin_1)		 //csn置低
//#define NRF_CE_HIGH()	   GPIO_SetBits(GPIOA,GPIO_Pin_2)
//#define NRF_CE_LOW()	   GPIO_ResetBits(GPIOA,GPIO_Pin_2)			  //CE置低
//#define NRF_Read_IRQ()		GPIO_ReadInputDataBit ( GPIOA, GPIO_Pin_3) //中断引脚

//void SPI_NRF_Init(void);
////uint8_t SPI_NRF_RW(uint8_t dat);
//uint8_t SPI_NRF_ReadReg(uint8_t reg );
//
//
//uint8_t spi_nrf_readbuf(uint8_t reg,uint8_t *pBuf,uint8_t bytes);
//uint8_t nrf_spi_writebuf(uint8_t reg ,uint8_t *pBuf,uint8_t bytes);

void nrfchip_init0(void);
//void NRF_TX_Mode(void);
//void NRF_RX_Mode(void);
uint8_t nrf_rx_dat(uint8_t *rxbuf);
uint8_t NRF_Tx_Dat(uint8_t *txbuf);
uint8_t nrf_check(void);
//void nrf_msater_rx_mode(void);
//void nrf_msater_tx_mode(void);
void nrf_device_rx_mode(void);
void nrf_device_tx_mode(void);
#endif /* __SPI_NRF_H */





















