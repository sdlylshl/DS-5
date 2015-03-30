#ifndef __DS1302_H
#define __DS1302_H


#include "stm32f10x.h"
#include "Types.h"

#define DS1302_SCLK_H()   (GPIO_SetBits(DS1302_SCLK_GPIO, DS1302_SCLK_Pin))
#define DS1302_SCLK_L()   (GPIO_ResetBits(DS1302_SCLK_GPIO, DS1302_SCLK_Pin))
#define DS1302_RST_H()    (GPIO_SetBits(DS1302_RST_GPIO, DS1302_RST_Pin))
#define DS1302_RST_L()    (GPIO_ResetBits(DS1302_RST_GPIO, DS1302_RST_Pin))
#define DS1302_IO_H()     (GPIO_SetBits(DS1302_IO_GPIO, DS1302_IO_Pin))
#define DS1302_IO_L()     (GPIO_ResetBits(DS1302_IO_GPIO, DS1302_IO_Pin))
#define DS1302_IO_IN()    (SetDS1302IO_In())
#define DS1302_IO_OUT()   (SetDS1302IO_Out())
#define DS1302_IO_STATE() (GPIO_ReadInputDataBit(DS1302_IO_GPIO, DS1302_IO_Pin))

#define DS1302_SECOND     0x80
#define DS1302_MINUTE     0x82
#define DS1302_HOUR       0x84
#define DS1302_DAY        0x86
#define DS1302_MONTH      0x88
#define DS1302_WEEK       0x8A
#define DS1302_YEAR       0x8C
#define DS1302_PROTECT    0x8E

#define DS1302_RD       0x01
#define DS1302_WR       0x00




static void SetDS1302IO_In(void);
static void SetDS1302IO_Out(void);
void DS1302_Init(void);
void Start1302(void);
uint8_t ds1302_set_time_test(void);

STU_TIME get_BSD_time(void);
void set_BSD_time(STU_TIME  tm);
uint8_t get_time(uint8_t* dst);
void set_time(uint8_t * src);
void get_stu_time(STU_TIME *tm);


#endif
