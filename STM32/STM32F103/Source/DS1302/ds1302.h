#ifndef __DS1302_H
#define __DS1302_H


#include "stm32f10x.h"

#if 0
#define   RdSec  0x81
#define   RdMin  0x83
#define   RdHour  0x85
#define   RdDate  0x87
#define   RdMonth  0x89
#define   RdWeek  0x8b
#define   RdYear  0x8d
#define   RdControl          0x8f
#define   RdTrickleCharge 0x91
#define   RdClockBurst  0xbf
#define   WrSec  0x80
#define   WrMin  0x82
#define   WrHour  0x84
#define   WrDate  0x86
#define   WrMonth  0x88
#define   WrWeek  0x8a
#define   WrYear  0x8c
#define   WrControl         0x8e
#define   WrTrickleCharge 0x90
#define   WrClockBurst  0xbe



#define DS1302_IORCC RCC_APB2Periph_GPIOB	

#define DS1302_PORT GPIOB

#define DS1302_SCK_PIN GPIO_Pin_10		
#define DS1302_IO_PIN GPIO_Pin_11
#define DS1302_CE_PIN GPIO_Pin_12

#define DS1302_CLRSCK() (DS1302_PORT->BRR = DS1302_SCK_PIN)		
#define DS1302_SETSCK() (DS1302_PORT->BSRR = DS1302_SCK_PIN)

#define DS1302_CLRIO() (DS1302_PORT->BRR = DS1302_IO_PIN)
#define DS1302_SETIO() (DS1302_PORT->BSRR = DS1302_IO_PIN)

#define DS1302_CLRCE() (DS1302_PORT->BRR = DS1302_CE_PIN)
#define DS1302_SETCE() (DS1302_PORT->BSRR = DS1302_CE_PIN)


#define DS1302_IO_IN()  {DS1302_PORT->CRH&=0XFFFFFFF0;DS1302_PORT->CRH|=8<<0;}  
#define DS1302_IO_OUT() {DS1302_PORT->CRH&=0XFFFFFFF0;DS1302_PORT->CRH|=3<<0;}

#define DS1302_READ_SDA()    (GPIO_ReadInputDataBit(DS1302_PORT, DS1302_IO_PIN))



typedef struct
{
	uint8_t year;
	uint8_t month;
	uint8_t date;
	uint8_t week;
	uint8_t hour;
	uint8_t min;
	uint8_t sec;
}TIME_TypeDef;	


void DS1302_Write8bit(uint8_t code);
uint8_t DS1302_Read8bit(void);

extern void RTC_init (void);
extern uint8_t DS1302_ReadByte(uint8_t con);
extern void DS1302_WriteByte(uint8_t con,uint8_t code);

extern void DS1302_WrintTime(TIME_TypeDef* time);
extern void DS1302_ReadTime(TIME_TypeDef* time);

#endif

#define DS1302_REG_READ       0x01
#define DS1302_REG_WRITE      0x00
#define DS1302_REG_SECOND     0x80
#define DS1302_REG_MINUTE     0x82
#define DS1302_REG_HOUR       0x84
#define DS1302_REG_DAY        0x86
#define DS1302_REG_MONTH      0x88
#define DS1302_REG_WEEK       0x8A
#define DS1302_REG_YEAR       0x8C
#define DS1302_REG_CONTROL    0x8E

#define DS1302_REG_TRICKLECHARGE 0x90
#define DS1302_REG_CLOCKBURST 	0xbe


#define DS1302_SCLK_GPIO  GPIOB
#define DS1302_SCLK_Pin   GPIO_Pin_5

#define DS1302_RST_GPIO   GPIOB
#define DS1302_RST_Pin    GPIO_Pin_9

#define DS1302_IO_GPIO    GPIOB
#define DS1302_IO_Pin     GPIO_Pin_8

#define DS1302_GPIO_CLOCK RCC_APB2Periph_GPIOB


#define DS1302_SCLK_H()   (GPIO_SetBits(DS1302_SCLK_GPIO, DS1302_SCLK_Pin))
#define DS1302_SCLK_L()   (GPIO_ResetBits(DS1302_SCLK_GPIO, DS1302_SCLK_Pin))

#define DS1302_RST_H()    (GPIO_SetBits(DS1302_RST_GPIO, DS1302_RST_Pin))
#define DS1302_RST_L()    (GPIO_ResetBits(DS1302_RST_GPIO, DS1302_RST_Pin))
#define DS1302_IO_H()     (GPIO_SetBits(DS1302_IO_GPIO, DS1302_IO_Pin))
#define DS1302_IO_L()     (GPIO_ResetBits(DS1302_IO_GPIO, DS1302_IO_Pin))
#define DS1302_IO_IN()    (SetDS1302IO_In())
#define DS1302_IO_OUT()   (SetDS1302IO_Out())
#define DS1302_IO_STATE() (GPIO_ReadInputDataBit(DS1302_IO_GPIO, DS1302_IO_Pin))

typedef struct
{
  uint8_t sec;
  uint8_t min;
  uint8_t hour;
  uint8_t week;
  uint8_t day;
  uint8_t month;
  uint8_t year;
}dstime_t;

void DS1302_Init(void);
void DS1302_Start(void);
uint8_t ds1302_set_dtime_test(void);

dstime_t get_BSD_time(void);
void set_BSD_time(dstime_t  tm);
uint8_t get_time(uint8_t* dst);
void set_time(uint8_t * src);


#endif
