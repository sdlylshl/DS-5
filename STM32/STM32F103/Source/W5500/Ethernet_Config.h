#ifndef __ETHERNET_CONFIG_H
#define __ETHERNET_CONFIG_H
//ʹ��StdPeriph_Driver��
//#define USE_STDPERIPH_DRIVER 1

//#include "stdint.h"
#include "stm32f10x.h"
//#include "stm32f4xx.h"

#include "Ethernet\socket.h"
#include "Ethernet\W5500\w5500.h"
#include "Ethernet\wizchip_conf.h"
//extern void printf();

#include "Application\Application.h"
#include "Application\loopback_server.h"
#include "Application\loopback_client.h"

//#include "Platform\rcc_handler.h"
//#include "Platform\gpio_w5500.h"
//#include "Platform\timer_w5500.h"
//#include "Platformrt_w5500.h"
//#include "Platform\spi_w5500.h"
//#include "Platform\dma_w5500.h"
//extern void RCC_Configuration();
//extern void GPIO_Configuration();
//extern void Timer_Configuration();
//extern void DMA_Configuration();
//extern void SPI_Configuration();
//extern void USART_Configuration();

#define DATA_BUF_SIZE   1024

// SRAM address range is 0x2000 0000 ~ 0x2000 4FFF (20KB)
#define TX_RX_MAX_BUF_SIZE	266
//#define TX_BUF	0x20004000
//#define RX_BUF	(TX_BUF+TX_RX_MAX_BUF_SIZE)

#endif
