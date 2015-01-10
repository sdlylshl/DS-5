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


_nrfchip_t NRFCHIP;

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
void (*CSN_LOW)(void) = SPI2_ResetNSS;
void (*CSN_HIGH)(void) = SPI2_SetNSS;
void (*CE_LOW)(void) = SPI2_ResetRST;
void (*CE_HIGH)(void) = SPI2_SetRST;
uint8_t (*NRF_Read_IRQ)(void) = SPI2_readIRQ;
uint8_t (*hal_nrf_rw)(uint8_t) = SPI2_SendByte;


void nrfchip_spi1(void){
	CSN_LOW = SPI1_ResetNSS;
	CSN_HIGH =SPI1_SetNSS;
	CE_LOW =SPI1_ResetRST;
	CE_HIGH =SPI1_SetRST;
	NRF_Read_IRQ =SPI1_readIRQ;
	hal_nrf_rw =SPI1_SendByte;
}
void nrfchip_spi2(void){
	CSN_LOW = SPI2_ResetNSS;
	CSN_HIGH =SPI2_SetNSS;
	CE_LOW =SPI2_ResetRST;
	CE_HIGH =SPI2_SetRST;
	NRF_Read_IRQ =SPI2_readIRQ;
	hal_nrf_rw =SPI2_SendByte;
}
void nrfchip_choice(_nrfchip_t nrf){

	CSN_LOW =nrf.CS._low;
	CSN_HIGH =nrf.CS._high;
	CE_LOW =nrf.CE._low;
	CE_HIGH =nrf.CE._high;
	NRF_Read_IRQ =nrf.IRQ._readirq;
	hal_nrf_rw = nrf.IF.SPI._write_byte;

}
#endif
