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
 * $LastChangedRevision: 2519 $
 */

/** @file
@brief Register definitions for nRF24L01+

@details Header file defining register mapping with bit definitions.
This file is radio-chip dependent, and are included with the hal_nrf.h

@ingroup hal_nrf24l01p_hal_nrf
 */

#ifndef HAL_NRF_REG_H__
#define HAL_NRF_REG_H__

/** @name - Instruction Set - */
//@{
/* nRF24L01 Instruction Definitions */
#define NRF_W_REGISTER         0x20U  /**< Register write command */
#define NRF_R_RX_PAYLOAD       0x61U  /**< Read RX payload command */
#define NRF_W_TX_PAYLOAD       0xA0U  /**< Write TX payload command */
#define NRF_FLUSH_TX           0xE1U  /**< Flush TX register command */
#define NRF_FLUSH_RX           0xE2U  /**< Flush RX register command */
#define NRF_REUSE_TX_PL        0xE3U  /**< Reuse TX payload command */
#define NRF_ACTIVATE           0x50U  /**< Activate features */
#define NRF_R_RX_PL_WID        0x60U  /**< Read RX payload command */
#define NRF_W_ACK_PAYLOAD      0xA8U  /**< Write ACK payload command */
#define NRF_W_TX_PAYLOAD_NOACK 0xB0U  /**< Write ACK payload command */
#define NRF_NOP                0xFFU  /**< No Operation command, used for reading status register */
//@}

/** @name  - Register Memory Map - */
//@{
/* nRF24L01 * Register Definitions * */
#define NRF_CONFIG        0x00U  /**< nRF24L01 config register */
#define NRF_EN_AA         0x01U  /**< nRF24L01 enable Auto-Acknowledge register */
#define NRF_EN_RXADDR     0x02U  /**< nRF24L01 enable RX addresses register */
#define NRF_SETUP_AW      0x03U  /**< nRF24L01 setup of address width register */
#define NRF_SETUP_RETR    0x04U  /**< nRF24L01 setup of automatic retransmission register */
#define NRF_RF_CH         0x05U  /**< nRF24L01 RF channel register */
#define NRF_RF_SETUP      0x06U  /**< nRF24L01 RF setup register */
#define NRF_STATUS        0x07U  /**< nRF24L01 status register */
#define NRF_OBSERVE_TX    0x08U  /**< nRF24L01 transmit observe register */
#define NRF_CD            0x09U  /**< nRF24L01 carrier detect register */
#define NRF_RX_ADDR_P0    0x0AU  /**< nRF24L01 receive address data pipe0 */
#define NRF_RX_ADDR_P1    0x0BU  /**< nRF24L01 receive address data pipe1 */
#define NRF_RX_ADDR_P2    0x0CU  /**< nRF24L01 receive address data pipe2 */
#define NRF_RX_ADDR_P3    0x0DU  /**< nRF24L01 receive address data pipe3 */
#define NRF_RX_ADDR_P4    0x0EU  /**< nRF24L01 receive address data pipe4 */
#define NRF_RX_ADDR_P5    0x0FU  /**< nRF24L01 receive address data pipe5 */
#define NRF_TX_ADDR       0x10U  /**< nRF24L01 transmit address */
#define NRF_RX_PW_P0      0x11U  /**< nRF24L01 \# of bytes in rx payload for pipe0 */
#define NRF_RX_PW_P1      0x12U  /**< nRF24L01 \# of bytes in rx payload for pipe1 */
#define NRF_RX_PW_P2      0x13U  /**< nRF24L01 \# of bytes in rx payload for pipe2 */
#define NRF_RX_PW_P3      0x14U  /**< nRF24L01 \# of bytes in rx payload for pipe3 */
#define NRF_RX_PW_P4      0x15U  /**< nRF24L01 \# of bytes in rx payload for pipe4 */
#define NRF_RX_PW_P5      0x16U  /**< nRF24L01 \# of bytes in rx payload for pipe5 */
#define NRF_FIFO_STATUS   0x17U  /**< nRF24L01 FIFO status register */
#define NRF_DYNPD         0x1CU  /**< nRF24L01 Dynamic payload setup */
#define NRF_FEATURE       0x1DU  /**< nRF24L01 Exclusive feature setup */

//@}

/* nRF24L01 related definitions */
/* Interrupt definitions */
/* Operation mode definitions */

/** An enum describing the radio's irq sources.
 *
 */
typedef enum {

	HAL_NRF_MASK_MAX_RT = 4,     /**<0> 1:MASK Max retries interrupt */
	HAL_NRF_MASK_TX_DS,          /**<0> 1:MASK TX data sent interrupt */
	HAL_NRF_MASK_RX_DR           /**<0> 1:MASK RX data received interrupt */
} hal_nrf_irq_source_t;

/* Operation mode definitions */
/** An enum describing the radio's power mode.
 *
 */
typedef enum {
		HAL_NRF_PTX,            /**<0> 0:Primary TX operation default*/
		HAL_NRF_PRX             /**<0> 1:Primary RX operation */
} hal_nrf_operation_mode_t;

/** An enum describing the radio's CRC mode.
 *
 */
typedef enum {
    HAL_NRF_CRC_OFF,    /**< CRC check disabled */
    HAL_NRF_CRC_8BIT,   /**< CRC check set to 8-bit default*/
    HAL_NRF_CRC_16BIT   /**< CRC check set to 16-bit */
} hal_nrf_crc_mode_t;

/** An enum describing the radio's power mode.
 *
 */
typedef enum {
    HAL_NRF_PWR_DOWN,       /**<0> 0: Device power-down default*/
    HAL_NRF_PWR_UP          /**<0> 1: Device power-up */
} hal_nrf_pwr_mode_t;

/** An enum describing the radio's output power mode's.
 *
 */
typedef enum {
    HAL_NRF_18DBM,          /**< Output power set to -18dBm */
    HAL_NRF_12DBM,          /**< Output power set to -12dBm */
    HAL_NRF_6DBM,           /**< Output power set to -6dBm  */
    HAL_NRF_0DBM            /**< Output power set to 0dBm  default */
} hal_nrf_output_power_t;

/** An enum describing the radio's on-air datarate.
 *
 */
typedef enum {
    HAL_NRF_1MBPS,          /**< Datarate set to 1 Mbps  */
    HAL_NRF_2MBPS,          /**< Datarate set to 2 Mbps  default */
    HAL_NRF_250KBPS         /**< Datarate set to 250 kbps*/
} hal_nrf_datarate_t;


/** An enum describing the read/write payload command.
 *
 */
typedef enum {
    HAL_NRF_TX_PLOAD = 7,   /**< TX payload definition */
    HAL_NRF_RX_PLOAD,        /**< RX payload definition */
    HAL_NRF_ACK_PLOAD
} hal_nrf_pload_command_t;

/** Structure containing the radio's address map.
 * Pipe0 contains 5 unique address bytes,
 * while pipe[1..5] share the 4 MSB bytes, set in pipe1.
 * <p><b> - Remember that the LSB byte for all pipes have to be unique! -</b>
 */
// nRF24L01 Address struct


//typedef struct {
//   uint8_t p0[5];            /**< Pipe0 address, 5 bytes */
//    uint8_t p1[5];            /**< Pipe1 address, 5 bytes, 4 MSB bytes shared for pipe1 to pipe5 */
//    uint8_t p2[1];            /**< Pipe2 address, 1 byte */
//    uint8_t p3[1];            /**< Pipe3 address, 1 byte */
//   uint8_t p4[1];            /**< Pipe3 address, 1 byte */
//   uint8_t p5[1];            /**< Pipe3 address, 1 byte */
//   uint8_t tx[5];            /**< TX address, 5 byte */
//} hal_nrf_l01_addr_map;


/** An enum describing the nRF24L01 pipe addresses and TX address.
 *
 */
typedef enum {
    HAL_NRF_PIPE0 = 0,          /**< Select pipe0 0xE7E7E7E7E7 */ //34 43 10 10 01
    HAL_NRF_PIPE1,              /**< Select pipe1 0xC2C2C2C2C2*/	//34 43 10 10 01
    HAL_NRF_PIPE2,              /**< Select pipe2 0xC3*/
    HAL_NRF_PIPE3,              /**< Select pipe3 0xC4*/
    HAL_NRF_PIPE4,              /**< Select pipe4 0xC5*/
    HAL_NRF_PIPE5,              /**< Select pipe5 0xC6*/
    HAL_NRF_TX,                 /**< Refer to TX address 0xE7E7E7E7E7*///34 43 10 10 01
    HAL_NRF_ALL = 0xFF          /**< Close or open all pipes*/
                                /**< @see hal_nrf_set_address @see hal_nrf_get_address
                                 @see hal_nrf_open_pipe  @see hal_nrf_close_pipe */
} hal_nrf_address_t;

/** An enum describing the radio's address width.
 *
 */
typedef enum {
    HAL_NRF_AW_3BYTES = 3,      /**< Set address width to 3 bytes */
    HAL_NRF_AW_4BYTES,          /**< Set address width to 4 bytes */
    HAL_NRF_AW_5BYTES           /**< Set address width to 5 bytes default*/
} hal_nrf_address_width_t;
typedef enum {
		HAL_NRF_TX_FULL,				/**< TX FIFO full flag.*/ 
	  HAL_NRF_RX_P_NO,				/**< Data pipe number for the payload available for reading from RX_FIFO */
	  HAL_NRF_MAX_RT = 4,     /**< Max retries interrupt */
    HAL_NRF_TX_DS,          /**< TX data sent interrupt */
    HAL_NRF_RX_DR           /**< RX data received interrupt */
} hal_nrf_status_t;
//若TX_REUSE=1则当CE位高电平状态时不断发送上一数据包
typedef enum {
	HAL_NRF_RX_EMPTY = 0,				  /**<1> 1: RX FIFO empty */
	HAL_NRF_RX_FULL,							/**<0> 1: RX FIFO full. */
	HAL_NRF_TX_EMPTY =4,					/**<1> 1: TX FIFO empty */
	HAL_NRF_TX_FIFO_FULL,					/**<0> 1: TX FIFO full. */
	HAL_NRF_TX_REUSE							/**<0>    TX payload reuse is active until NRF_W_TX_PAYLOAD or FLUSH TX is executed.
																			    TX_REUSE=1	Pulse the CE high for at least 10us to Reuse last transmitted payload.
																			    TX_REUSE is set by the SPI command NRF_REUSE_TX_PL, 
																			    and is reset by the SPI commands NRF_W_TX_PAYLOAD or FLUSH TX*/
} hal_nrf_fifo_status_t;

/** @name NRF_CONFIG register bit definitions */
//@{

#define MASK_RX_DR    0x40     /**< NRF_CONFIG register bit 6 */
#define MASK_TX_DS    0x20     /**< NRF_CONFIG register bit 5 */
#define MASK_MAX_RT   0x10     /**< NRF_CONFIG register bit 4 */
#define EN_CRC        0x08     /**< NRF_CONFIG register bit 3 */
#define CRCO          0x04     /**< NRF_CONFIG register bit 2 */
#define PWR_UP        0x02     /**< NRF_CONFIG register bit 1 */
#define PRIM_RX       0x01     /**< NRF_CONFIG register bit 0 */
//@}

/** @name NRF_RF_SETUP register bit definitions */
//@{
#define PLL_LOCK      0x10     /**< NRF_RF_SETUP register bit 4 */
#define RF_DR         0x08     /**< NRF_RF_SETUP register bit 3 */
#define RF_PWR1       0x04     /**< NRF_RF_SETUP register bit 2 */
#define RF_PWR0       0x02     /**< NRF_RF_SETUP register bit 1 */
#define LNA_HCURR     0x01     /**< NRF_RF_SETUP register bit 0 */
//@}

/* NRF_STATUS 0x07 */
/** @name NRF_STATUS register bit definitions */
//@{
#define RX_DR         0x40     /**< NRF_STATUS register bit 6 */
#define TX_DS         0x20     /**< NRF_STATUS register bit 5 */
#define MAX_RT        0x10     /**< NRF_STATUS register bit 4 */
#define TX_FULL       0x01     /**< NRF_STATUS register bit 0 */
//@}

/* NRF_FIFO_STATUS 0x17 */
/** @name NRF_FIFO_STATUS register bit definitions */
//@{
#define TX_REUSE      0x40     /**< NRF_FIFO_STATUS register bit 6 */
#define TX_FIFO_FULL  0x20     /**< NRF_FIFO_STATUS register bit 5 */
#define TX_EMPTY      0x10     /**< NRF_FIFO_STATUS register bit 4 */
#define RX_FULL       0x02     /**< NRF_FIFO_STATUS register bit 1 */
#define RX_EMPTY      0x01     /**< NRF_FIFO_STATUS register bit 0 */
//@}

#endif // HAL_NRF_REG_H__
/** @} */
