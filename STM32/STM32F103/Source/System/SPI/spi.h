#ifndef __SPI_H
#define __SPI_H

#include "version.h"

#define DELAY_TIME      500


#define SPI_IRQ_PORT            GPIOD
#define SPI_IRQ_PIN             GPIO_Pin_8
#define SPI_IRQ_MODE            GPIO_Mode_IPU

#define SPI_CEN_PORT            GPIOD
#define SPI_CEN_PIN             GPIO_Pin_9
#define SPI_CEN_MODE            GPIO_Mode_Out_PP

// SPI2 PB 12 13 14 15
#define SPI_CSN_PORT        GPIOB
#define SPI_CSN_PIN         GPIO_Pin_12
#define SPI_CSN_MODE        GPIO_Mode_Out_PP    //此处一定要PP输出否则程序不正常

#define SPI_CLK_PORT        GPIOB
#define SPI_CLK_PIN         GPIO_Pin_13
#define SPI_CLK_MODE        GPIO_Mode_Out_PP

#define SPI_MISO_PORT       GPIOB
#define SPI_MISO_PIN        GPIO_Pin_14
#define SPI_MISO_MODE       GPIO_Mode_IPU

#define SPI_MOSI_PORT       GPIOB
#define SPI_MOSI_PIN        GPIO_Pin_15
#define SPI_MOSI_MODE       GPIO_Mode_Out_PP

//#define SPI_CEN_HIGH()      GPIO_SetBits(SPI_CEN_PORT, SPI_CEN_PIN)
//#define SPI_CEN_LOW()       GPIO_ResetBits(SPI_CEN_PORT, SPI_CEN_PIN)
//#define SPI_CEN(x)          x ? GPIO_SetBits(SPI_CEN_PORT,SPI_CEN_PIN): GPIO_ResetBits(SPI_CEN_PORT,SPI_CEN_PIN)

//#define SPI_CSN_HIGH()      GPIO_SetBits(SPI_CSN_PORT, SPI_CSN_PIN)
//#define SPI_CSN_LOW()       GPIO_ResetBits(SPI_CSN_PORT, SPI_CSN_PIN)
//#define SPI_CSN(x)          x ? GPIO_SetBits(SPI_CSN_PORT,SPI_CSN_PIN): GPIO_ResetBits(SPI_CSN_PORT,SPI_CSN_PIN)

#define SPI_CLK_HIGH()      GPIO_SetBits(SPI_CLK_PORT, SPI_CLK_PIN)
#define SPI_CLK_LOW()       GPIO_ResetBits(SPI_CLK_PORT, SPI_CLK_PIN)
#define SPI_CLK(x)          x ? GPIO_SetBits(SPI_CLK_PORT,SPI_CLK_PIN): GPIO_ResetBits(SPI_CLK_PORT,SPI_CLK_PIN)

#define SPI_MOSI_HIGH()     GPIO_SetBits(SPI_MOSI_PORT, SPI_MOSI_PIN)
#define SPI_MOSI_LOW()      GPIO_ResetBits(SPI_MOSI_PORT, SPI_MOSI_PIN)
#define SPI_MOSI(x)         x ? GPIO_SetBits(SPI_MOSI_PORT,SPI_MOSI_PIN): GPIO_ResetBits(SPI_MOSI_PORT,SPI_MOSI_PIN)

#define SPI_MISO_READ()     GPIO_ReadInputDataBit(SPI_MISO_PORT, SPI_MISO_PIN)

//#define SPI_IRQ_READ()      GPIO_ReadInputDataBit(SPI_IRQ_PORT, SPI_IRQ_PIN)

__STATIC_INLINE void SPI_CEN_HIGH(){GPIO_SetBits(SPI_CEN_PORT, SPI_CEN_PIN);}
__STATIC_INLINE void SPI_CEN_LOW() {GPIO_ResetBits(SPI_CEN_PORT, SPI_CEN_PIN);}
__STATIC_INLINE void SPI_CSN_HIGH(){GPIO_SetBits(SPI_CSN_PORT, SPI_CSN_PIN);}
__STATIC_INLINE void SPI_CSN_LOW() {
GPIO_ResetBits(SPI_CSN_PORT, SPI_CSN_PIN);}

//__STATIC_INLINE void SPI_CLK_HIGH(){GPIO_SetBits(SPI_CLK_PORT, SPI_CLK_PIN);}
//__STATIC_INLINE void SPI_CLK_LOW() {GPIO_ResetBits(SPI_CLK_PORT, SPI_CLK_PIN);}
//__STATIC_INLINE void SPI_MOSI_HIGH(){GPIO_SetBits(SPI_MOSI_PORT, SPI_MOSI_PIN);}
//__STATIC_INLINE void SPI_MOSI_LOW() {GPIO_ResetBits(SPI_MOSI_PORT, SPI_MOSI_PIN);}

//__STATIC_INLINE uint8_t SPI_MISO_READ(){return GPIO_ReadInputDataBit(SPI_MISO_PORT, SPI_MISO_PIN);}

__STATIC_INLINE uint8_t SPI_IRQ_READ(){ return GPIO_ReadInputDataBit(SPI_IRQ_PORT, SPI_IRQ_PIN);}

extern uint8_t SPI_ReadWrite(uint8_t data);
#endif

