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
#include "../System/spi.h"
//#include "./System/System_config.h"
#include "hal_nrf_hw.h"

#define MAX_NRFCHIP 1
#define NRF_ADR_WIDTH 5
//uint8_t nrf_txaddr[MAX_NRFCHIP][NRF_ADR_WIDTH];
//uint8_t nrf_rxaddr[MAX_NRFCHIP][NRF_ADR_WIDTH];
//#define NRF_ID_WIDTH 5
//uint8_t nrf_id[MAX_NRFCHIP][NRF_ID_WIDTH];

void nrfchip_init(_nrf_chip_t *nrf_chip,SPIx_t SPIx) {


	if (SPIx == SPI)
	{
		nrf_chip->CSN_LOW = SPI_CSN_LOW;
		nrf_chip->CSN_HIGH = SPI_CSN_HIGH;
		nrf_chip->CE_LOW = SPI_CEN_LOW;
		nrf_chip->CE_HIGH = SPI_CEN_HIGH;
		nrf_chip->NRF_Read_IRQ = SPI_IRQ_READ;
		nrf_chip->hal_nrf_rw = SPI_ReadWrite;
	}

}

