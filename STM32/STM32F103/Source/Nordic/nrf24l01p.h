

#ifndef __NRF2401_H
#define __NRF2401_H

#include "version.h"

uint8_t nrf_chip_test(_nrf_chip_t *nrf_chip);
void nrf_chip_rx_mode(_nrf_chip_t *nrf_chip, uint8_t *addr, uint8_t channel);
void nrf_chip_tx_mode(_nrf_chip_t *nrf_chip, uint8_t *addr, uint8_t channel);
uint8_t nrf_chip_tx_dat(_nrf_chip_t *nrf_chip, const uint8_t * txdat);
//void nrf_master(void);
//void nrf_device(void);
void nrf_main(void);

void nrf_isr(void);
#endif /* __NRF2401_H */





















