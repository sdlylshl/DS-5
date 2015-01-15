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
#define MAX_NRFCHIP 3
#define NRF_ADR_WIDTH 5
#define NRF_ID_WIDTH 5
typedef enum{
	NRF_RECIVE,
	NRF_SEND
}NRF_NUM_t;
typedef struct __nrf_chip{
	volatile uint8_t radio_busy;
	uint8_t (*NRF_Read_IRQ)(void);
	void (*CSN_LOW)(void);
	void (*CSN_HIGH)(void);
	void (*CE_LOW)(void);
	void (*CE_HIGH)(void);
	uint8_t (*hal_nrf_rw)(uint8_t);

}_nrf_chip_t;
extern _nrf_chip_t nrf_chip;
typedef struct __nrfchip {
	uint8_t radio_busy;
	uint8_t chanal;
	uint8_t id[NRF_ID_WIDTH];
	uint8_t tx_addr[NRF_ADR_WIDTH];
	uint8_t rx_addr[NRF_ADR_WIDTH];
	uint8_t head;
	uint8_t end;
	uint8_t rx_buffer[256];

	struct _IRQ {
		uint8_t (*_readirq)(void);
	} IRQ;
	struct _CE {
		void (*_low)(void);
		void (*_high)(void);
	} CE;
	struct _CS {
		void (*_low)(void);
		void (*_high)(void);
	} CS;

	union _IF {

		struct {
			uint8_t (*_read_byte)(void);
			uint8_t (*_write_byte)(uint8_t wb);
		} SPI;
	} IF;
} _nrfchip_t;


typedef void (*vFUNv)(void);
typedef uint8_t (*uFUNu)(uint8_t);
typedef uint8_t (*uFUNv)(void);
extern vFUNv CSN_LOW,CSN_HIGH,CE_LOW,CE_HIGH;
extern uFUNv NRF_Read_IRQ;
extern uFUNu hal_nrf_rw;

//extern void (*CSN_LOW)(void);
//extern void (*CSN_HIGH)(void);
//extern void (*CE_LOW)(void);
//extern void (*CE_HIGH)(void);
//extern uint8_t (*hal_nrf_rw)(uint8_t);
//extern uint8_t (*NRF_Read_IRQ)(void);

//#define CSN_LOW()		do { SPI2_SCS_LOW(); } while(false)
//#define CSN_HIGH()	do { SPI2_SCS_HIGH(); } while(false)
//#define CE_LOW()		do { SPI2_RST_LOW(); } while(false)
//#define CE_HIGH()		do { SPI2_RST_HIGH(); } while(false)
//#define NRF_Read_IRQ() SPI2_READ_IRQ()
//#define hal_nrf_rw 	SPI2_SendByte

/**
 * Pulses the CE to nRF24L01 for at least 10 us
 */
#define CE_PULSE() do { \
  uint8_t count; \
  count = 50; \
  CE_HIGH();  \
  while(count--){} \
  CE_LOW();  \
  } while(false)

#endif // HAL_NRF_LU1_H__
typedef enum {
	SPI_1,
	SPI_2,
	SPI_3
}SPIx_t;
void nrfchip_init(_nrf_chip_t *nrf_chip, SPIx_t SPIx);
	
/** @} */
