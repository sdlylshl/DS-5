#ifndef __SPI_H
#define __SPI_H
#include "../version.h"
#include <xc.h>
#include "./spi.h"
#include <stdint.h>
#define DELAY_TIME      10

#define SPI_DELAY()
//  void SPI_DELAY(){ uint16_t k=0; for( ; k <= DELAY_TIME; k++ );}
//#define SPI_CE (PORTAbits.RA0)
//#define SPI_CS (PORTAbits.RA1)
//#define SPI_IRQ() (PORTAbits.RA2)
//#define SPI_MISO() (PORTAbits.RA3)
//#define SPI_CLK (PORTAbits.RA4)
//#define SPI_MOSI (PORTAbits.RA5)
//最终方案：
// RA0/ICSPDAT     CE
// RA1/ICSPCLK     CSN
// RA4             IRQ
// RA5             LED

// RC0             SCK
// RC5             MISO
// RC4             MOSI

#define SPI_CE (PORTAbits.RA0)
#define SPI_CS (PORTAbits.RA1)
#define SPI_IRQ() (PORTAbits.RA4)

#define SPI_CLK (PORTCbits.RC0)
#define SPI_MOSI (PORTCbits.RC4)
#define SPI_MISO() (PORTCbits.RC5)

extern  void SPI_CEN_HIGH();
extern  void SPI_CEN_LOW();
extern  void SPI_CSN_HIGH();
extern  void SPI_CSN_LOW();

extern   void SPI_CLK_HIGH();
extern  void SPI_CLK_LOW();
extern  void SPI_MOSI_HIGH();
extern  void SPI_MOSI_LOW();

extern  uint8_t SPI_MISO_READ();

extern  uint8_t SPI_IRQ_READ();

extern void SPI_INIT( void );
extern uint8_t SPI_ReadWrite(uint8_t data);
#endif

