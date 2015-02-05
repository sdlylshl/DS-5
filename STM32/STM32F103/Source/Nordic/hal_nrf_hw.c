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

#if 1
//函数指针定义
//void (*CSN_LOW)(void) = SPI2_ResetNSS;
//void (*CSN_HIGH)(void) = SPI2_SetNSS;
//void (*CE_LOW)(void) = SPI2_ResetRST;
//void (*CE_HIGH)(void) = SPI2_SetRST;
//uint8_t (*NRF_Read_IRQ)(void) = SPI2_readIRQ;
//uint8_t (*hal_nrf_rw)(uint8_t) = SPI2_SendByte;
const uint8_t chip_id[NRF_ADDRESS_WIDTH] ={0,0,0,0,1};
_nrf_chip_t nrf_chip;

void nrfchip_init0(void) {

	nrf_chip.CSN_LOW = SPI_CSN_LOW;
	nrf_chip.CSN_HIGH = SPI_CSN_HIGH;
	nrf_chip.CE_LOW = SPI_CEN_LOW;
	nrf_chip.CE_HIGH = SPI_CEN_HIGH;
	nrf_chip.NRF_Read_IRQ = SPI_IRQ_READ;
	nrf_chip.hal_nrf_rw = SPI_ReadWrite;

}

void nrfchipX_init(_nrf_chip_t *nrf_chip, SPIx_t SPIx) {

	assert_param(IS_SPI_ALL_PERIPH(SPIx));

	if (SPIx == SPI) {
		nrf_chip->CSN_LOW = SPI_CSN_LOW;
		nrf_chip->CSN_HIGH = SPI_CSN_HIGH;
		nrf_chip->CE_LOW = SPI_CEN_LOW;
		nrf_chip->CE_HIGH = SPI_CEN_HIGH;
		nrf_chip->NRF_Read_IRQ = SPI_IRQ_READ;
		nrf_chip->hal_nrf_rw = SPI_ReadWrite;
	} else if (SPIx == SPI_1) {
		nrf_chip->CSN_LOW = SPI1_ResetNSS;
		nrf_chip->CSN_HIGH = SPI1_SetNSS;
		nrf_chip->CE_LOW = SPI1_ResetRST;
		nrf_chip->CE_HIGH = SPI1_SetRST;
		nrf_chip->NRF_Read_IRQ = SPI1_readIRQ;
		nrf_chip->hal_nrf_rw = SPI1_SendByte;
	} else if (SPIx == SPI_2) {
		nrf_chip->CSN_LOW = SPI2_ResetNSS;
		nrf_chip->CSN_HIGH = SPI2_SetNSS;
		nrf_chip->CE_LOW = SPI2_ResetRST;
		nrf_chip->CE_HIGH = SPI2_SetRST;
		nrf_chip->NRF_Read_IRQ = SPI2_readIRQ;
		nrf_chip->hal_nrf_rw = SPI2_SendByte;

	}
}

#endif
