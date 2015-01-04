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
 * $LastChangedRevision: 133 $
 */

/** @file
* @brief hal_nrf macros for nRF24LU1+
 *
 * @defgroup hal_nrf24lu1p_hal_nrf_hw hal_nrf_hw
 * @{
 * @ingroup hal_nrf24lu1p
 *
 */

#ifndef HAL_NRF_LU1_H__
#define HAL_NRF_LU1_H__
//#include "nrf24lu1p.h"
#include "version.h"
// SPI2
#include "../System/System_config.h"

#define TX_ADR_WIDTH 		5  	
#define TX_PLOAD_WIDTH  4   

#define RX_ADR_WIDTH    5
#define RX_PLOAD_WIDTH  4

#define CHANAL 40


/** Macro that set radio's CSN line LOW.
 *
 */
#define CSN_LOW()		do { SPI2_SCS_LOW(); } while(false)

/** Macro that set radio's CSN line HIGH.
 *
 */
#define CSN_HIGH()	do { SPI2_SCS_HIGH(); } while(false)

/** Macro that set radio's CE line LOW.
 *
 */
#define CE_LOW()		do { SPI2_RST_LOW(); } while(false)

/** Macro that set radio's CE line HIGH.
 *
 */
#define CE_HIGH()		do { SPI2_RST_HIGH(); } while(false)

#define hal_nrf_rw 	SPI2_SendByte
/**
 * Pulses the CE to nRF24L01 for at least 10 us
 */
#define CE_PULSE() do { \
  uint8_t count; \
  count = 20; \
  CE_HIGH();  \
  while(count--){} \
  CE_LOW();  \
  } while(false)

#endif // HAL_NRF_LU1_H__

/** @} */
