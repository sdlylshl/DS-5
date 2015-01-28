/* Copyright (c) 2009 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is confidential property of Nordic
 * Semiconductor ASA.Terms and conditions of usage are described in detail
 * in NORDIC SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRENTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 * $LastChangedRevision: 2513 $
 */

/** @file
 * @brief Implementation of #hal_nrf_rw.
 *
 * @details #hal_nrf_rw is an SPI function which is hardware dependent. This file
 * contains an implementation for nRF24LE1.
 */

#include <stdint.h>
#include "./System/System_config.h"
#include "hal_nrf_hw.h"

//#define MAX_NRFCHIP 1
//_nrfchip_t nrfchip;
//#define NRF_ADR_WIDTH 5
//uint8_t nrf_txaddr[MAX_NRFCHIP][NRF_ADR_WIDTH];
//uint8_t nrf_rxaddr[MAX_NRFCHIP][NRF_ADR_WIDTH];
//#define NRF_ID_WIDTH 5
//uint8_t nrf_id[MAX_NRFCHIP][NRF_ID_WIDTH];
//
//uint8_t nrf_rx_buffer[256];

//{
//	.id = {1,2,3,4},
//	.IRQ._readirq =SPI1_readIRQ,
//	.CE._select =SPI1_SetRST,
//	.CE._deselect =SPI1_ResetRST,
//	.CS._select =SPI1_ResetNSS,
//	.CS._deselect =SPI1_SetNSS,
//	.IF.SPI._read_byte =SPI1_ReceiveByte,
//	.IF.SPI._write_byte =SPI1_SendByte
//  .IF.SPI._read_byte   = nrfchip_spi_readbyte,
//  .IF.SPI._write_byte  = nrfchip_spi_writebyte
//};

//void nrfchip_cs_deselect(void) {}
//void nrfchip_cs_select(void) {}
//void nrfchip_ce_deselect(void) {}
//void nrfchip_ce_select(void) {}
//uint8_t nrfchip_readirq(void) {return SPI1_readIRQ();}
//uint8_t nrfchip_rw(void) {return SPI1_readIRQ();}
#if 1
//函数指针定义
//void (*CSN_LOW)(void) = SPI2_ResetNSS;
//void (*CSN_HIGH)(void) = SPI2_SetNSS;
//void (*CE_LOW)(void) = SPI2_ResetRST;
//void (*CE_HIGH)(void) = SPI2_SetRST;
//uint8_t (*NRF_Read_IRQ)(void) = SPI2_readIRQ;
//uint8_t (*hal_nrf_rw)(uint8_t) = SPI2_SendByte;

//void nrfchip_spi1(void) {
//	CSN_LOW = SPI1_ResetNSS;
//	CSN_HIGH = SPI1_SetNSS;
//	CE_LOW = SPI1_ResetRST;
//	CE_HIGH = SPI1_SetRST;
//	NRF_Read_IRQ = SPI1_readIRQ;
//	hal_nrf_rw = SPI1_SendByte;
//}
//void nrfchip_spi2(void) {
//	CSN_LOW = SPI2_ResetNSS;
//	CSN_HIGH = SPI2_SetNSS;
//	CE_LOW = SPI2_ResetRST;
//	CE_HIGH = SPI2_SetRST;
//	NRF_Read_IRQ = SPI2_readIRQ;
//	hal_nrf_rw = SPI2_SendByte;
//}

//void nrfchip_choice(_nrfchip_t nrf) {

//	CSN_LOW = nrf.CS._low;
//	CSN_HIGH = nrf.CS._high;
//	CE_LOW = nrf.CE._low;
//	CE_HIGH = nrf.CE._high;
//	NRF_Read_IRQ = nrf.IRQ._readirq;
//	hal_nrf_rw = nrf.IF.SPI._write_byte;

//}
//_nrf_chip_t nrf_chip;
void nrfchip_init(_nrf_chip_t *nrf_chip,SPIx_t SPIx) {


	assert_param(IS_SPI_ALL_PERIPH(SPIx));

	if (SPIx == SPI)
	{
		nrf_chip->CSN_LOW = SPI_CSN_LOW;
		nrf_chip->CSN_HIGH = SPI_CSN_HIGH;
		nrf_chip->CE_LOW = SPI_CEN_LOW;
		nrf_chip->CE_HIGH = SPI_CEN_HIGH;
		nrf_chip->NRF_Read_IRQ = SPI_IRQ_READ;
		nrf_chip->hal_nrf_rw = SPI_ReadWrite;
	}else
	if (SPIx == SPI_1)
	{
		nrf_chip->CSN_LOW = SPI1_ResetNSS;
		nrf_chip->CSN_HIGH = SPI1_SetNSS;
		nrf_chip->CE_LOW = SPI1_ResetRST;
		nrf_chip->CE_HIGH = SPI1_SetRST;
		nrf_chip->NRF_Read_IRQ = SPI1_readIRQ;
		nrf_chip->hal_nrf_rw = SPI1_SendByte;
	} else if (SPIx == SPI_2)
	{
		nrf_chip->CSN_LOW = SPI2_ResetNSS;
		nrf_chip->CSN_HIGH = SPI2_SetNSS;
		nrf_chip->CE_LOW = SPI2_ResetRST;
		nrf_chip->CE_HIGH = SPI2_SetRST;
		nrf_chip->NRF_Read_IRQ = SPI2_readIRQ;
		nrf_chip->hal_nrf_rw = SPI2_SendByte;

	}
	//return nrf_chip;
//	nrfchip.id = nrf_id[0];
//	nrfchip.radio_busy =0;
//	nrfchip.chanal =40;
//	nrfchip.head =0;
//	nrfchip.end =0;
//	nrfchip.rx_buffer = nrf_rx_buffer;
//	nrfchip.rx_addr = nrf_rxaddr[0];
//	nrfchip.tx_addr = nrf_txaddr[0] ;
//	nrfchip.IRQ._readirq =SPI1_readIRQ;
//	nrfchip.CE._low =SPI1_ResetRST;
//	nrfchip.CE._high =SPI1_SetRST;
//	nrfchip.CS._low =SPI1_ResetNSS;
//	nrfchip.CS._high =SPI1_SetNSS;
//	nrfchip.IF.SPI._write_byte =SPI1_SendByte;
}

#endif
