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
 * $LastChangedRevision: 2713 $
 */

/** @file
 * @brief Implementation of hal_nrf
*/
/*lint -t2 */
/*lint -esym( 534, hal_nrf_write_reg ) */
/*lint -esym( 534, hal_nrf_rw ) */
/*lint -e708 */
/*lint -e46 */

#include <stdint.h>
#include <stdbool.h>

#include "nordic_common.h"
#include "hal_nrf2401p.h"

#define SET_BIT_POS(pos) ((uint8_t) (1U<<( (uint8_t) (pos) )))
#define UINT8(t) ((uint8_t) (t))

/** Basis function read_reg.
 * Use this function to read the contents
 * of one radios register.
 *
 * @param reg Register to read
 * @return Register contents
*/
uint8_t hal_nrf_read_reg(_nrf_chip_t *nrf_chip, uint8_t reg);

/** Basis function write_reg.
 * Use this function to write a new value to
 * a radio register.
 *
 * @param reg Register to write
 * @param value New value to write
 * @return Status register
*/
uint8_t hal_nrf_write_reg(_nrf_chip_t *nrf_chip, uint8_t reg, uint8_t value);

/** Basis function, read_multibyte register .
 * Use this function to read multiple bytes from
 * a multibyte radio-register
 *
 * @param reg Multibyte register to read from
 * @param *pbuf Pointer to buffer in which to store read bytes to
 *
 * @return pipe# of received data (MSB), if operation used by a hal_nrf_read_rx_pload
 * @return length of read data (LSB), either for hal_nrf_read_rx_pload or
 * for hal_nrf_get_address.
*/
uint16_t hal_nrf_read_multibyte_reg(_nrf_chip_t *nrf_chip, uint8_t reg, uint8_t *pbuf);

/** Basis function, write_multibyte register.
 * Use this function to write multiple bytes to
 * a multiple radio register.
 *
 * @param reg Register to write
 * @param *pbuf pointer to buffer in which data to write is
 * @param length \# of bytes to write
*/
void hal_nrf_write_multibyte_reg(_nrf_chip_t *nrf_chip, uint8_t reg, const uint8_t *pbuf, uint8_t length);

/**
 * Typedef for the NRF_CONFIG register. Contains all the bitaddressable
 * settings in the bits struct and the value sent to the radio in the uint8_t
 */
typedef union {
    uint8_t value;
    struct {
        uint8_t prim_rx : 1;
        uint8_t pwr_up : 1;
        uint8_t crc0 : 1;
        uint8_t en_crc : 1;
        uint8_t mask_max_rt : 1;
        uint8_t mask_tx_ds : 1;
        uint8_t mask_rx_dr : 1;
        const uint8_t : 1;
    } bits;
} config_t;

/**
 * Typedef for the NRF_EN_AA, NRF_EN_RXADDR and NRF_DYNPD registers. Contains all the
 * bitaddressable settings in the bits struct and the value sent to the radio
 * in the uint8_t
 */
typedef union {
    uint8_t value;
    struct {
        uint8_t pipe_0 : 1;
        uint8_t pipe_1 : 1;
        uint8_t pipe_2 : 1;
        uint8_t pipe_3 : 1;
        uint8_t pipe_4 : 1;
        uint8_t pipe_5 : 1;
        const uint8_t : 2;
    } bits;
} en_pipes_t;

/**
 * Typedef for the NRF_SETUP_AW register. Contains all the bitaddressable
 * settings in the bits struct and the value sent to the radio in the uint8_t
 */
typedef union {
    uint8_t value;
    struct {
        uint8_t aw : 2;
        const uint8_t : 6;
    } bits;
} setup_aw_t;

/**
 * Typedef for the NRF_SETUP_RETR register. Contains all the bitaddressable
 * settings in the bits struct and the value sent to the radio in the uint8_t
 */
typedef union {
    uint8_t value;
    struct {
        uint8_t arc : 4;
        uint8_t ard : 4;
    } bits;
} setup_retr_t;

/**
 * Typedef for the NRF_RF_CH register. Contains all the bitaddressable
 * settings in the bits struct and the value sent to the radio in the uint8_t
 */
typedef union {
    uint8_t value;
    struct {
        uint8_t rf_ch : 7;
        const uint8_t : 1;
    } bits;
} rf_ch_t;

/**
 * Typedef for the NRF_RF_SETUP register. Contains all the bitaddressable
 * settings in the bits struct and the value sent to the radio in the uint8_t
 */
typedef union {
    uint8_t value;
    struct {
        const uint8_t : 1;
        uint8_t rf_pwr : 2;
        uint8_t rf_dr_high : 1;
        uint8_t pll_lock : 1;
        uint8_t rf_dr_low : 1;
        const uint8_t : 1;
        uint8_t cont_wave : 1;
    } bits;
} rf_setup_t;

/**
 * Typedef for the RX_PW_Px registers. Contains all the bitaddressable
 * settings in the bits struct and the value sent to the radio in the uint8_t
 */
typedef union {
    uint8_t value;
    struct {
        uint8_t rx_pw : 6;
        const uint8_t : 2;
    } bits;
} rx_pw_t;

/**
 * Typedef for the NRF_FEATURE register. Contains all the bitaddressable
 * settings in the bits struct and the value sent to the radio in the uint8_t
 */
typedef union {
    uint8_t value;
    struct {
        uint8_t en_dyn_ack : 1;
        uint8_t en_ack_pay : 1;
        uint8_t en_dpl : 1;
        const uint8_t : 5;
    } bits;
} feature_t;

void hal_nrf_set_operation_mode(_nrf_chip_t *nrf_chip, hal_nrf_operation_mode_t op_mode)
{
    config_t config;
    config.value = hal_nrf_read_reg (nrf_chip, NRF_CONFIG);

    if(op_mode == HAL_NRF_PRX) {
        config.bits.prim_rx = 1U;
    } else {
        config.bits.prim_rx = 0U;
    }

    hal_nrf_write_reg (nrf_chip, NRF_CONFIG, config.value);
}

void hal_nrf_set_power_mode(_nrf_chip_t *nrf_chip, hal_nrf_pwr_mode_t pwr_mode)
{
    config_t config;
    config.value = hal_nrf_read_reg (nrf_chip, NRF_CONFIG);

    if(pwr_mode == HAL_NRF_PWR_UP) {
        config.bits.pwr_up = 1U;
    } else {
        config.bits.pwr_up = 0U;
    }

    hal_nrf_write_reg (nrf_chip, NRF_CONFIG, config.value);
}

void hal_nrf_set_crc_mode(_nrf_chip_t *nrf_chip, hal_nrf_crc_mode_t crc_mode)
{
    config_t config;
    config.value = hal_nrf_read_reg (nrf_chip, NRF_CONFIG);

    switch (crc_mode) {
    case HAL_NRF_CRC_OFF:
        config.bits.en_crc = 0U;
        break;
    case HAL_NRF_CRC_8BIT:
        config.bits.en_crc = 1U;
        config.bits.crc0 = 0U;
        break;
    case HAL_NRF_CRC_16BIT:
        config.bits.en_crc = 1U;
        config.bits.crc0 = 1U;
        break;
    default:
        break;
    }

    hal_nrf_write_reg (nrf_chip, NRF_CONFIG, config.value);
}

void hal_nrf_set_irq_mode(_nrf_chip_t *nrf_chip, hal_nrf_irq_source_t int_source, bool irq_state)
{
    config_t config;
    config.value = hal_nrf_read_reg (nrf_chip, NRF_CONFIG);

    switch (int_source) {
    case HAL_NRF_MAX_RT:
        config.bits.mask_max_rt = irq_state ? 0U : 1U;
        break;
    case HAL_NRF_TX_DS:
        config.bits.mask_tx_ds = irq_state ? 0U : 1U;
        break;
    case HAL_NRF_RX_DR:
        config.bits.mask_rx_dr = irq_state ? 0U : 1U;
        break;
    }

    hal_nrf_write_reg (nrf_chip, NRF_CONFIG, config.value);
}

uint8_t hal_nrf_get_clear_irq_flags(_nrf_chip_t *nrf_chip)
{
    uint8_t retval;

    retval = hal_nrf_write_reg (nrf_chip, NRF_STATUS, (BIT_6|BIT_5|BIT_4));

    return (retval & (BIT_6|BIT_5|BIT_4));
}

uint8_t hal_nrf_clear_irq_flags_get_status(_nrf_chip_t *nrf_chip)
{
    uint8_t retval;

    // When RFIRQ is cleared (when calling write_reg), pipe information is unreliable (read again with read_reg)
    retval = hal_nrf_write_reg (nrf_chip, NRF_STATUS, (BIT_6|BIT_5|BIT_4)) & (BIT_6|BIT_5|BIT_4);
    retval |= hal_nrf_read_reg (nrf_chip, NRF_STATUS) & (BIT_3|BIT_2|BIT_1|BIT_0);

    return (retval);
}


void hal_nrf_clear_irq_flag(_nrf_chip_t *nrf_chip, hal_nrf_irq_source_t int_source)
{
    hal_nrf_write_reg (nrf_chip, NRF_STATUS, SET_BIT_POS(int_source));
}

uint8_t hal_nrf_get_irq_flags(_nrf_chip_t *nrf_chip)
{
    return hal_nrf_nop(nrf_chip) & (BIT_6|BIT_5|BIT_4);
}

void hal_nrf_open_pipe(_nrf_chip_t *nrf_chip, hal_nrf_address_t pipe_num, bool auto_ack)
{
    en_pipes_t en_rxaddr;
    en_pipes_t en_aa;
    en_rxaddr.value = hal_nrf_read_reg (nrf_chip, NRF_EN_RXADDR);
    en_aa.value = hal_nrf_read_reg (nrf_chip, NRF_EN_AA);

    switch(pipe_num) {
    case HAL_NRF_PIPE0:
    case HAL_NRF_PIPE1:
    case HAL_NRF_PIPE2:
    case HAL_NRF_PIPE3:
    case HAL_NRF_PIPE4:
    case HAL_NRF_PIPE5:
        en_rxaddr.value = en_rxaddr.value | SET_BIT_POS(pipe_num);

        if(auto_ack) {
            en_aa.value = en_aa.value | SET_BIT_POS(pipe_num);
        } else {
            en_aa.value = en_aa.value & (uint8_t)~SET_BIT_POS(pipe_num);
        }
        break;

    case HAL_NRF_ALL:
        en_rxaddr.value = (uint8_t)(~(BIT_6|BIT_7));

        if(auto_ack) {
            en_aa.value = (uint8_t)(~(BIT_6|BIT_7));
        } else {
            en_aa.value = 0U;
        }
        break;

    case HAL_NRF_TX:
    default:
        break;
    }

    hal_nrf_write_reg (nrf_chip, NRF_EN_RXADDR, en_rxaddr.value);
    hal_nrf_write_reg (nrf_chip, NRF_EN_AA, en_aa.value);
}

void hal_nrf_close_pipe(_nrf_chip_t *nrf_chip, hal_nrf_address_t pipe_num)
{
    en_pipes_t en_rxaddr;
    en_pipes_t en_aa;
    en_rxaddr.value = hal_nrf_read_reg (nrf_chip, NRF_EN_RXADDR);
    en_aa.value = hal_nrf_read_reg (nrf_chip, NRF_EN_AA);

    switch(pipe_num) {
    case HAL_NRF_PIPE0:
    case HAL_NRF_PIPE1:
    case HAL_NRF_PIPE2:
    case HAL_NRF_PIPE3:
    case HAL_NRF_PIPE4:
    case HAL_NRF_PIPE5:
        en_rxaddr.value = en_rxaddr.value & (uint8_t)~SET_BIT_POS(pipe_num);
        en_aa.value = en_aa.value & (uint8_t)~SET_BIT_POS(pipe_num);
        break;

    case HAL_NRF_ALL:
        en_rxaddr.value = 0U;
        en_aa.value = 0U;
        break;

    case HAL_NRF_TX:
    default:
        break;
    }

    hal_nrf_write_reg (nrf_chip, NRF_EN_RXADDR, en_rxaddr.value);
    hal_nrf_write_reg (nrf_chip, NRF_EN_AA, en_aa.value);
}

void hal_nrf_set_address(_nrf_chip_t *nrf_chip, const hal_nrf_address_t address, const uint8_t *addr)
{
    switch(address) {
    case HAL_NRF_TX:
    case HAL_NRF_PIPE0:
    case HAL_NRF_PIPE1:
        hal_nrf_write_multibyte_reg(nrf_chip, NRF_W_REGISTER + NRF_RX_ADDR_P0 + (uint8_t) address, addr, hal_nrf_get_address_width(nrf_chip));
        break;
    case HAL_NRF_PIPE2:
    case HAL_NRF_PIPE3:
    case HAL_NRF_PIPE4:
    case HAL_NRF_PIPE5:
        hal_nrf_write_reg (nrf_chip, NRF_RX_ADDR_P0 + (uint8_t) address, *addr);
        break;

    case HAL_NRF_ALL:
    default:
        break;
    }
}

uint8_t hal_nrf_get_address(_nrf_chip_t *nrf_chip, const hal_nrf_address_t address, uint8_t *addr)
{
    switch (address) {
    case HAL_NRF_PIPE0:
    case HAL_NRF_PIPE1:
    case HAL_NRF_TX:
        return (uint8_t)hal_nrf_read_multibyte_reg (nrf_chip, address, addr);
    default:
        *addr = hal_nrf_read_reg(nrf_chip, NRF_RX_ADDR_P0 + address);
        return 1U;
    }
}

void hal_nrf_set_auto_retr(_nrf_chip_t *nrf_chip, uint8_t retr, uint16_t delay)
{
    setup_retr_t setup_retr;
    setup_retr.bits.ard = (uint8_t)(delay >> 8);
    setup_retr.bits.arc = retr;

    hal_nrf_write_reg (nrf_chip, NRF_SETUP_RETR, setup_retr.value);
}

void hal_nrf_set_address_width(_nrf_chip_t *nrf_chip, hal_nrf_address_width_t address_width)
{
    setup_aw_t setup_aw;
    setup_aw.value = 0U;
    setup_aw.bits.aw = (uint8_t)address_width - 2U;

    hal_nrf_write_reg (nrf_chip, NRF_SETUP_AW, setup_aw.value);
}

uint8_t hal_nrf_get_address_width (_nrf_chip_t *nrf_chip)
{
    return hal_nrf_read_reg (nrf_chip, NRF_SETUP_AW) + 2U;
}

void hal_nrf_set_rx_payload_width(_nrf_chip_t *nrf_chip, hal_nrf_address_t pipe_num, uint8_t pload_width)
{
    hal_nrf_write_reg (nrf_chip, NRF_RX_PW_P0 + pipe_num, pload_width);
}

uint8_t hal_nrf_get_pipe_status(_nrf_chip_t *nrf_chip, hal_nrf_address_t pipe_num)
{
    en_pipes_t en_rxaddr;
    en_pipes_t en_aa;
    uint8_t en_rx_r, en_aa_r;

    en_rxaddr.value = hal_nrf_read_reg (nrf_chip, NRF_EN_RXADDR);
    en_aa.value = hal_nrf_read_reg (nrf_chip, NRF_EN_AA);

    switch (pipe_num) {
    case 0:
        en_rx_r = en_rxaddr.bits.pipe_0;
        en_aa_r = en_aa.bits.pipe_0;
        break;
    case 1:
        en_rx_r = en_rxaddr.bits.pipe_1;
        en_aa_r = en_aa.bits.pipe_1;
        break;
    case 2:
        en_rx_r = en_rxaddr.bits.pipe_2;
        en_aa_r = en_aa.bits.pipe_2;
        break;
    case 3:
        en_rx_r = en_rxaddr.bits.pipe_3;
        en_aa_r = en_aa.bits.pipe_3;
        break;
    case 4:
        en_rx_r = en_rxaddr.bits.pipe_4;
        en_aa_r = en_aa.bits.pipe_4;
        break;
    case 5:
        en_rx_r = en_rxaddr.bits.pipe_5;
        en_aa_r = en_aa.bits.pipe_5;
        break;
    default:
        en_rx_r = 0U;
        en_aa_r = 0U;
        break;
    }

    return (uint8_t)(en_aa_r << 1) + en_rx_r;
}

uint8_t hal_nrf_get_auto_retr_status(_nrf_chip_t *nrf_chip)
{
    return hal_nrf_read_reg(nrf_chip, NRF_OBSERVE_TX);
}

uint8_t hal_nrf_get_packet_lost_ctr(_nrf_chip_t *nrf_chip)
{
    return ((hal_nrf_read_reg(nrf_chip, NRF_OBSERVE_TX) & (BIT_7|BIT_6|BIT_5|BIT_4)) >> 4);
}

uint8_t hal_nrf_get_rx_payload_width(_nrf_chip_t *nrf_chip, hal_nrf_address_t pipe_num)
{
    uint8_t pw;

    switch (pipe_num) {
    case 0:
        pw = hal_nrf_read_reg (nrf_chip, NRF_RX_PW_P0);
        break;
    case 1:
        pw = hal_nrf_read_reg (nrf_chip, NRF_RX_PW_P1);
        break;
    case 2:
        pw = hal_nrf_read_reg (nrf_chip, NRF_RX_PW_P2);
        break;
    case 3:
        pw = hal_nrf_read_reg (nrf_chip, NRF_RX_PW_P3);
        break;
    case 4:
        pw = hal_nrf_read_reg (nrf_chip, NRF_RX_PW_P4);
        break;
    case 5:
        pw = hal_nrf_read_reg (nrf_chip, NRF_RX_PW_P5);
        break;
    default:
        pw = 0U;
        break;
    }

    return pw;
}

void hal_nrf_set_rf_channel(_nrf_chip_t *nrf_chip, uint8_t channel)
{
    rf_ch_t rf_ch;
    rf_ch.value = 0U;
    rf_ch.bits.rf_ch = channel;
    hal_nrf_write_reg (nrf_chip, NRF_RF_CH, rf_ch.value);
}

void hal_nrf_set_output_power(_nrf_chip_t *nrf_chip, hal_nrf_output_power_t power)
{
    rf_setup_t rf_setup;
    rf_setup.value = hal_nrf_read_reg (nrf_chip, NRF_RF_SETUP);

    rf_setup.bits.rf_pwr = (uint8_t)power;

    hal_nrf_write_reg (nrf_chip, NRF_RF_SETUP, rf_setup.value);
}

void hal_nrf_set_datarate(_nrf_chip_t *nrf_chip, hal_nrf_datarate_t datarate)
{
    rf_setup_t rf_setup;
    rf_setup.value = hal_nrf_read_reg (nrf_chip, NRF_RF_SETUP);

    switch (datarate) {
    case HAL_NRF_250KBPS:
        rf_setup.bits.rf_dr_low = 1U;
        rf_setup.bits.rf_dr_high = 0U;
        break;
    case HAL_NRF_1MBPS:
        rf_setup.bits.rf_dr_low = 0U;
        rf_setup.bits.rf_dr_high = 0U;
        break;
    case HAL_NRF_2MBPS:
    default:
        rf_setup.bits.rf_dr_low = 0U;
        rf_setup.bits.rf_dr_high = 1U;
        break;
    }

    hal_nrf_write_reg (nrf_chip, NRF_RF_SETUP, rf_setup.value);
}

bool hal_nrf_rx_fifo_empty(_nrf_chip_t *nrf_chip)
{
    return (bool)((hal_nrf_read_reg(nrf_chip, NRF_FIFO_STATUS) >> HAL_NRF_RX_EMPTY) & 0x01U);
}

bool hal_nrf_rx_fifo_full(_nrf_chip_t *nrf_chip)
{
    return (bool)((hal_nrf_read_reg(nrf_chip, NRF_FIFO_STATUS) >> HAL_NRF_RX_FULL) & 0x01U);
}

bool hal_nrf_tx_fifo_empty(_nrf_chip_t *nrf_chip)
{
    return (bool)((hal_nrf_read_reg(nrf_chip, NRF_FIFO_STATUS) >> HAL_NRF_TX_EMPTY) & 0x01U);
}

bool hal_nrf_tx_fifo_full(_nrf_chip_t *nrf_chip)
{
    return (bool)((hal_nrf_read_reg(nrf_chip, NRF_FIFO_STATUS) >> HAL_NRF_TX_FIFO_FULL) & 0x01U);
}

uint8_t hal_nrf_get_tx_fifo_status(_nrf_chip_t *nrf_chip)
{
    return ((hal_nrf_read_reg(nrf_chip, NRF_FIFO_STATUS) & ((1U<<HAL_NRF_TX_FIFO_FULL)|(1U<<HAL_NRF_TX_EMPTY))) >> 4);
}

uint8_t hal_nrf_get_rx_fifo_status(_nrf_chip_t *nrf_chip)
{
    return (hal_nrf_read_reg(nrf_chip, NRF_FIFO_STATUS) & ((1U<<HAL_NRF_RX_FULL)|(1U<<HAL_NRF_RX_EMPTY)));
}

uint8_t hal_nrf_get_fifo_status(_nrf_chip_t *nrf_chip)
{
    return hal_nrf_read_reg(nrf_chip, NRF_FIFO_STATUS);
}

uint8_t hal_nrf_get_transmit_attempts(_nrf_chip_t *nrf_chip)
{
    return (hal_nrf_read_reg(nrf_chip, NRF_OBSERVE_TX) & (BIT_3|BIT_2|BIT_1|BIT_0));
}

bool hal_nrf_get_carrier_detect(_nrf_chip_t *nrf_chip)
{
    return (bool)(hal_nrf_read_reg(nrf_chip, NRF_CD) & 0x01U);
}

void hal_nrf_activate_features(_nrf_chip_t *nrf_chip)
{
    return;
}

void hal_nrf_setup_dynamic_payload (_nrf_chip_t *nrf_chip, uint8_t setup)
{
    en_pipes_t dynpd;
    dynpd.value = setup & (uint8_t)~0xC0U;

    hal_nrf_write_reg (nrf_chip, NRF_DYNPD, dynpd.value);
}

void hal_nrf_enable_dynamic_payload(_nrf_chip_t *nrf_chip, bool enable)
{
    feature_t feature;
    feature.value = hal_nrf_read_reg (nrf_chip, NRF_FEATURE);
    feature.bits.en_dpl = (enable) ? 1U : 0U;

    hal_nrf_write_reg (nrf_chip, NRF_FEATURE, feature.value);
}

void hal_nrf_enable_ack_payload(_nrf_chip_t *nrf_chip, bool enable)
{
    feature_t feature;
    feature.value = hal_nrf_read_reg (nrf_chip, NRF_FEATURE);
    feature.bits.en_ack_pay = (enable) ? 1U : 0U;

    hal_nrf_write_reg (nrf_chip, NRF_FEATURE, feature.value);
}

void hal_nrf_enable_dynamic_ack(_nrf_chip_t *nrf_chip, bool enable)
{
    feature_t feature;
    feature.value = hal_nrf_read_reg (nrf_chip, NRF_FEATURE);
    feature.bits.en_dyn_ack = (enable) ? 1U : 0U;

    hal_nrf_write_reg (nrf_chip, NRF_FEATURE, feature.value);
}

void hal_nrf_write_tx_payload(_nrf_chip_t *nrf_chip, const uint8_t *tx_pload, uint8_t length)
{
    hal_nrf_write_multibyte_reg(nrf_chip, NRF_WR_TX_PAYLOAD, tx_pload, length);
}

void hal_nrf_write_tx_payload_noack(_nrf_chip_t *nrf_chip, const uint8_t *tx_pload, uint8_t length)
{
    hal_nrf_write_multibyte_reg(nrf_chip, NRF_W_TX_PAYLOAD_NOACK, tx_pload, length);
}

void hal_nrf_write_ack_payload(_nrf_chip_t *nrf_chip, uint8_t pipe, const uint8_t *tx_pload, uint8_t length)
{
    hal_nrf_write_multibyte_reg(nrf_chip, NRF_W_ACK_PAYLOAD | pipe, tx_pload, length);
}

uint8_t hal_nrf_read_rx_payload_width(_nrf_chip_t *nrf_chip)
{
    return hal_nrf_read_reg(nrf_chip, NRF_R_RX_PL_WID);
}

uint16_t hal_nrf_read_rx_payload(_nrf_chip_t *nrf_chip, uint8_t *rx_pload)
{
    return hal_nrf_read_multibyte_reg(nrf_chip, UINT8(HAL_NRF_RX_PLOAD), rx_pload);
}

uint8_t hal_nrf_get_rx_data_source(_nrf_chip_t *nrf_chip)
{
    return ((hal_nrf_nop(nrf_chip) & (BIT_3|BIT_2|BIT_1)) >> 1);
}
//重发上一包数据
//若TX_REUSE=1则当CE位高电平状态时不断发送上一数据包
//TX_REUSE 通过 SPI  指令REUSE_TX_PL 设置 通过 W_TX_PALOAD或 FLUSH_TX复位
void hal_nrf_reuse_tx(_nrf_chip_t *nrf_chip)
{
	nrf_chip->CSN_LOW();
    nrf_chip->hal_spi_rw(NRF_REUSE_TX_PL);
    nrf_chip->CSN_HIGH();
}

bool hal_nrf_get_reuse_tx_status(_nrf_chip_t *nrf_chip)
{
    return (bool)((hal_nrf_get_fifo_status(nrf_chip) & (1U<<HAL_NRF_TX_REUSE)) >> HAL_NRF_TX_REUSE);
}

void hal_nrf_flush_rx(_nrf_chip_t *nrf_chip)
{
	nrf_chip->CSN_LOW();
	nrf_chip->hal_spi_rw(NRF_FLUSH_RX);
	nrf_chip->CSN_HIGH();
}

void hal_nrf_flush_tx(_nrf_chip_t *nrf_chip)
{
	nrf_chip->CSN_LOW();
	nrf_chip->hal_spi_rw(NRF_FLUSH_TX);
	nrf_chip->CSN_HIGH();
}

uint8_t hal_nrf_nop(_nrf_chip_t *nrf_chip)
{
    uint8_t retval;

    nrf_chip->CSN_LOW();
    retval = nrf_chip->hal_spi_rw(NRF_NOP);
    nrf_chip->CSN_HIGH();

    return retval;
}

void hal_nrf_set_pll_mode(_nrf_chip_t *nrf_chip, bool pll_lock)
{
    rf_setup_t rf_setup;
    rf_setup.value = hal_nrf_read_reg (nrf_chip, NRF_RF_SETUP);
    rf_setup.bits.pll_lock = (pll_lock) ? 1U : 0U;

    hal_nrf_write_reg(nrf_chip, NRF_RF_SETUP, rf_setup.value);
}

void hal_nrf_enable_continious_wave (_nrf_chip_t *nrf_chip, bool enable)
{
    rf_setup_t rf_setup;
    rf_setup.value = hal_nrf_read_reg (nrf_chip, NRF_RF_SETUP);
    rf_setup.bits.cont_wave = (enable ? 1U : 0U);

    hal_nrf_write_reg(nrf_chip, NRF_RF_SETUP, rf_setup.value);
}

uint8_t hal_nrf_read_reg(_nrf_chip_t *nrf_chip, uint8_t reg)
{
    uint8_t temp;

    nrf_chip->CE_LOW();

    nrf_chip->CSN_LOW();

    nrf_chip->hal_spi_rw(reg);

    temp = nrf_chip->hal_spi_rw(NRF_NOP);

    nrf_chip->CSN_HIGH();

    return temp;
}

uint8_t hal_nrf_write_reg(_nrf_chip_t *nrf_chip, uint8_t reg, uint8_t value)
{
    uint8_t retval;
    /*lint -esym(550,dummy) symbol not accessed*/
    /*lint -esym(438,dummy) last assigned value not used*/
    /*lint -esym(838,dummy) previously assigned value not used*/
    //uint8_t volatile dummy;

    nrf_chip->CE_LOW();

    nrf_chip->CSN_LOW();

    /*发送命令及寄存器号 */
    retval = nrf_chip->hal_spi_rw(NRF_W_REGISTER+reg);

    /*向寄存器写入数据*/
    nrf_chip->hal_spi_rw(value);

    nrf_chip->CSN_HIGH();

    return retval;
}

uint16_t hal_nrf_read_multibyte_reg(_nrf_chip_t *nrf_chip, uint8_t reg, uint8_t *pbuf)
{
    uint8_t length,byte_cnt;

    nrf_chip->CE_LOW();
    switch(reg) {
    case HAL_NRF_PIPE0:
    case HAL_NRF_PIPE1:
    case HAL_NRF_TX:
        length = hal_nrf_get_address_width(nrf_chip);
        nrf_chip->CSN_LOW();
        nrf_chip->hal_spi_rw(NRF_RX_ADDR_P0 + reg);
        break;

    case HAL_NRF_RX_PLOAD:
        reg = hal_nrf_get_rx_data_source(nrf_chip);
        if (reg < 7U) {
            length = hal_nrf_read_rx_payload_width(nrf_chip);
            nrf_chip->CSN_LOW();
            nrf_chip->hal_spi_rw(NRF_RD_RX_PAYLOAD);
        } else {
            length = 0U;
        }
        break;

    default:
        length = 0U;
        break;
    }

    for(byte_cnt=0; byte_cnt<length; byte_cnt++)
        pbuf[byte_cnt] = nrf_chip->hal_spi_rw(NRF_NOP); //从NRF24L01读取数据
    nrf_chip->CSN_HIGH();

    return (((uint16_t) reg << 8) | length);
}

void hal_nrf_write_multibyte_reg(_nrf_chip_t *nrf_chip, uint8_t reg, const uint8_t *pbuf, uint8_t length)
{
    uint8_t byte_cnt;
    nrf_chip->CE_LOW();
    nrf_chip->CSN_LOW();

    /*发送寄存器号*/
    nrf_chip->hal_spi_rw(reg);

    /*向缓冲区写入数据*/
    for(byte_cnt=0; byte_cnt<length; byte_cnt++)
    	nrf_chip->hal_spi_rw(*pbuf++);	//写数据到缓冲区

    nrf_chip->CSN_HIGH();
}
void hal_nrf_enable_radio(_nrf_chip_t *nrf_chip){
	nrf_chip->CE_HIGH();
}
