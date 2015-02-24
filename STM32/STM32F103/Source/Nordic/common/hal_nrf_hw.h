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

#ifndef __HAL_NRF_HW_H_
#define __HAL_NRF_HW_H_
//#include "nrf24lu1p.h"
#include "version.h"
#include "hal_nrf_reg.h"
// SPI2
#include "../../System/System_config.h"

#define NRF_ADDRESS_WIDTH 		5
#define NRF_PLOAD_WIDTH  		5
extern uint8_t NRF__RX_BUF[NRF_PLOAD_WIDTH];
extern uint8_t NRF__TX_BUF[NRF_PLOAD_WIDTH];
extern uint8_t NRF_CHIPID[NRF_ADDRESS_WIDTH];
extern uint8_t NRF_MASTER_RECV_ADDRESS[NRF_ADDRESS_WIDTH];
extern uint8_t NRF_MASTER_SEND_ADDRESS[NRF_ADDRESS_WIDTH];
//#define NRF_DEVICE_RECV_ADDRESS NRF_MASTER_SEND_ADDRESS
#define NRF_DEVICE_RECV_ADDRESS NRF_CHIPID
#define NRF_DEVICE_SEND_ADDRESS NRF_MASTER_RECV_ADDRESS
#define MASTER_RX_CHANNEL 40
#define MASTER_TX_CHANNEL 70
#define DEVICE_RX_CHANNEL MASTER_TX_CHANNEL
#define DEVICE_TX_CHANNEL MASTER_RX_CHANNEL

#define CHANAL 40

extern uint32_t (*GetCurrentTime)(void);
extern uint32_t (*GetDistanceTime)(uint32_t );

//#define MAX_NRFCHIP 1
//#define NRF_ADR_WIDTH 5
//#define NRF_ID_WIDTH 5
typedef enum {
	NRF_RECIVE, NRF_SEND
} NRF_NUM_t;

#if 0
////注册外部接口函数
//SPI2
//void (*NRF_CSN_LOW)(void) = SPI2_ResetNSS;
//void (*NRF_CSN_HIGH)(void) = SPI2_SetNSS;
//void (*NRF_CE_LOW)(void) = SPI2_ResetRST;
//void (*NRF_CE_HIGH)(void) = SPI2_SetRST;
//uint8_t (*NRF_GET_IRQ)(void) = SPI2_readIRQ;
//uint8_t (*SPI_NRF_RW)(uint8_t) = SPI2_SendByte;

// 模拟SPI
void (*NRF_CSN_LOW)(void) = SPI_CSN_LOW;
void (*NRF_CSN_HIGH)(void) = SPI_CSN_HIGH;
void (*NRF_CE_LOW)(void) = SPI_CEN_LOW;
void (*NRF_CE_HIGH)(void) = SPI_CEN_HIGH;
uint8_t (*NRF_GET_IRQ)(void) = SPI_IRQ_READ;
uint8_t (*SPI_NRF_RW)(uint8_t) = SPI_ReadWrite;
#else
typedef struct __nrf_chip {
	volatile uint8_t flag;		//接收发送标志位

//	volatile uint8_t chanel;	//信道
//	volatile uint8_t mode;		//工作模式
//	volatile uint8_t addr[NRF_ADDRESS_WIDTH];

	uint8_t (*NRF_Read_IRQ)(void);
	void (*CSN_LOW)(void);
	void (*CSN_HIGH)(void);
	void (*CE_LOW)(void);
	void (*CE_HIGH)(void);
	uint8_t (*hal_spi_rw)(uint8_t);

} _nrf_chip_t;
extern _nrf_chip_t nrf_chip;
#endif
//typedef struct __nrfchip {
//	uint8_t radio_busy;
//	uint8_t chanal;
//	uint8_t id[NRF_ID_WIDTH];
//	uint8_t tx_addr[NRF_ADR_WIDTH];
//	uint8_t rx_addr[NRF_ADR_WIDTH];
//	uint8_t head;
//	uint8_t end;
//	uint8_t rx_buffer[256];
//
//	struct _IRQ {
//		uint8_t (*_readirq)(void);
//	} IRQ;
//	struct _CE {
//		void (*_low)(void);
//		void (*_high)(void);
//	} CE;
//	struct _CS {
//		void (*_low)(void);
//		void (*_high)(void);
//	} CS;
//
//	union _IF {
//
//		struct {
//			uint8_t (*_read_byte)(void);
//			uint8_t (*_write_byte)(uint8_t wb);
//		} SPI;
//	} IF;
//} _nrfchip_t;

//typedef void (*vFUNv)(void);
//typedef uint8_t (*uFUNu)(uint8_t);
//typedef uint8_t (*uFUNv)(void);
//extern vFUNv CSN_LOW,CSN_HIGH,CE_LOW,CE_HIGH;
//extern uFUNv NRF_Read_IRQ;
//extern uFUNu hal_nrf_rw;

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


typedef enum {
	SPI, SPI_1, SPI_2, SPI_3
} SPIx_t;
#endif // __HAL_NRF_HW_H_