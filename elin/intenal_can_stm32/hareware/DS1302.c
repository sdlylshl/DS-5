#include "ds1302.h"


void Write1302(uint8_t addr, uint8_t dat);
uint8_t Read1302(uint8_t addr);
void ReadTime(void);

void Stop1302(void);
uint8_t ds1302_set_time(uint8_t* src);
uint8_t ds1302_get_time(uint8_t* dst);

STU_TIME utime;

/**************************************
函数功能：设置DS1302 对应IO为输入模式
**************************************/
static void SetDS1302IO_In(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  GPIO_InitStructure.GPIO_Pin = DS1302_IO_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(DS1302_IO_GPIO, &GPIO_InitStructure);
}

/**************************************
函数功能：设置DS1302 对应IO为输出模式
**************************************/
static void SetDS1302IO_Out(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  GPIO_InitStructure.GPIO_Pin = DS1302_IO_Pin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(DS1302_IO_GPIO, &GPIO_InitStructure);
}

/**************************************
函数功能：配置DS1302对应控制引脚
**************************************/
void DS1302_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd(DS1302_GPIO_CLOCK, ENABLE);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  //DS1302 IO引脚初始化为推挽输出
  GPIO_InitStructure.GPIO_Pin = DS1302_IO_Pin;
  GPIO_InitStructure.GPIO_Mode = DS1302_IO_GPIO_Mode;
  GPIO_Init(DS1302_IO_GPIO, &GPIO_InitStructure);
  //DS1302 RST引脚初始化为推挽输出
  GPIO_InitStructure.GPIO_Pin = DS1302_RST_Pin;
  GPIO_InitStructure.GPIO_Mode = DS1302_RST_GPIO_Mode;
  GPIO_Init(DS1302_RST_GPIO, &GPIO_InitStructure);
  //DS1302 SCLK引脚初始化为推挽输出
  GPIO_InitStructure.GPIO_Pin = DS1302_SCLK_Pin;
  GPIO_InitStructure.GPIO_Mode = DS1302_RST_GPIO_Mode;
  GPIO_Init(DS1302_SCLK_GPIO, &GPIO_InitStructure);
}
/**************************************
函数功能：向DS1302指定地址写入指定数据
**************************************/
void Write1302(uint8_t addr, uint8_t dat)
{
  uint8_t i;
  
  DS1302_RST_L();
  DS1302_SCLK_L();
  DS1302_RST_H();
  DS1302_IO_OUT();
  for(i = 8; i > 0; i--)
  {
    DS1302_SCLK_L();
    if(addr & 0x01)
      DS1302_IO_H();
    else
      DS1302_IO_L();
    addr >>= 1;
    DS1302_SCLK_H();
  }
  for(i = 8; i > 0; i--)
  {
    DS1302_SCLK_L();
    if(dat & 0x01)
      DS1302_IO_H();
    else
      DS1302_IO_L();
    dat >>= 1;
    DS1302_SCLK_H();
  }
  DS1302_RST_L();
}
/**************************************
函数功能：从DS1302指定地址读出数据
**************************************/
uint8_t Read1302(uint8_t addr)
{
  uint8_t i, RetVal = 0;
  
  DS1302_RST_L();
  DS1302_SCLK_L();
  DS1302_RST_H();
  DS1302_IO_OUT();
  for(i = 8; i > 0; i--)
  {
    DS1302_SCLK_L();
    if(addr & 0x01)
      DS1302_IO_H();
    else
      DS1302_IO_L();
    addr >>= 1;
    DS1302_SCLK_H();
  }
  DS1302_IO_IN();
  for(i = 8; i > 0; i--)
  {
    if(DS1302_IO_STATE())
      RetVal |= 0x80;
    else
      RetVal &= 0x7f;
    DS1302_SCLK_H();
    RetVal >>= 1;
    DS1302_SCLK_L();
  }
  DS1302_RST_L();
  
  return RetVal;
}
/**************************************
函数功能：从DS1302读取完整时间，并保存数据
**************************************/
void ReadTime(void)
{
  utime.sec = Read1302(DS1302_SECOND|DS1302_RD);
  utime.min = Read1302(DS1302_MINUTE|DS1302_RD);
  utime.hour = Read1302(DS1302_HOUR|DS1302_RD);
  utime.week = Read1302(DS1302_WEEK|DS1302_RD);
  utime.day = Read1302(DS1302_DAY|DS1302_RD);
  utime.month = Read1302(DS1302_MONTH|DS1302_RD);
  utime.year = Read1302(DS1302_YEAR|DS1302_RD);
}

/*************************************************************
*************************************************************/
void Write_1302_Time(unsigned char *time) 
{
	unsigned char add=0x80;      
	unsigned char i; 
	    
	
	Write1302(0x8e,0x00);       
	
	for(i=0;i<7;i++) 
	{
		Write1302(add,time[i]); 
		add+=2;                    
	}
	
	Write1302(0x8e,0x80);       
} 



/**************************************
函数功能：启动ＤＳ1302
**************************************/
void Start1302(void)
{
	uint8_t temp;
	temp = Read1302(DS1302_SECOND | DS1302_RD);
	Write1302(DS1302_SECOND | DS1302_WR,temp&0x7f);
}
/**************************************
函数功能：暂停ＤＳ1302
**************************************/
void Stop1302(void)
{
	uint8_t temp;
	temp = Read1302(DS1302_SECOND | DS1302_RD);
	Write1302(DS1302_SECOND | DS1302_WR,temp|0x80);
}



uint8_t ds1302_set_time_test()
{


STU_TIME tm;
tm.year = 14;
tm.week = 2;
tm.month = 12;
tm.day = 2;
tm.hour = 10;
tm.min = 39;
tm.sec = 33;

set_BSD_time( tm);
return 0;

}

uint8_t ds1302_set_time(uint8_t* src)
{
	uint8_t ret=ERROR;
	uint8_t dst[7];

	dst[0] = src[8]&0xf0+src[8]&0x0f;
	dst[1] = src[7]&0xf0+src[7]&0x0f;

	dst[2] = src[6]&0xf0+src[6]&0x0f;
	dst[3] = src[5]&0xf0+src[5]&0x0f;;

	dst[4] = src[4]&0xf0+src[4]&0x0f;
	dst[5] = src[3];

	dst[6] = src[2]&0xf0+src[2]&0x0f;
	Write_1302_Time(dst);


	return ret;
}



uint8_t ds1302_get_time(uint8_t* dst)
{
	uint8_t ret=ERROR;
	ReadTime();


	dst[0] = ((utime.year & 0xf0)>>4)*10 + (utime.year & 0x0f);
	dst[1] = utime.week;
	dst[2] = ((utime.month & 0xf0)>>4)*10 + (utime.month & 0x0f);
	dst[3] = ((utime.day & 0xf0)>>4)*10 + (utime.day & 0x0f);
	dst[4] = ((utime.hour & 0xf0)>>4)*10 + (utime.hour & 0x0f);
	dst[5] = ((utime.min& 0xf0)>>4)*10 + (utime.min & 0x0f);
	dst[6] = ((utime.sec& 0xf0)>>4)*10 + (utime.sec& 0x0f);

	
	return ret;
}



uint8_t get_time(uint8_t* dst)
{
	uint8_t ret=ERROR;
	ReadTime();

	dst[0] = utime.year;
	dst[1] = utime.week;
	dst[2] = utime.month;
	dst[3] = utime.day ;
	dst[4] = utime.hour ;
	dst[5] = utime.min;
	dst[6] = utime.sec;

	
	return ret;
}

void set_time(uint8_t * src)
{
	/*
	src[0] sec
	src[1] min
	src[2] hour
	src[3] day
	src[4] mon
	src[5] week
	src[6] year
	*/
	
//	uint8_t dst[7];
//	dst[0] = src[8]&0xf0+src[8]&0x0f;
//	dst[1] = src[7]&0xf0+src[7]&0x0f;

//	dst[2] = src[6]&0xf0+src[6]&0x0f;
//	dst[3] = src[5]&0xf0+src[5]&0x0f;;

//	dst[4] = src[4]&0xf0+src[4]&0x0f;
//	dst[5] = src[3];

//	dst[6] = src[2]&0xf0+src[2]&0x0f;
	Write_1302_Time(src);

}

void get_stu_time(STU_TIME *tm)
{
	tm->year = utime.year;
	tm->month = utime.month;
	tm->week = utime.week;
	tm->day = utime.day;
	tm->hour = utime.hour;
	tm->min  = utime.min;
	tm->sec = utime.sec;
}

STU_TIME get_BSD_time()
{
	ReadTime();
	return utime;
}

void set_BSD_time(STU_TIME  tm)
{
	uint8_t dst[7];
//	uint8_t src[10];
	dst[6] = (tm.year %100 / 10 << 4 & 0xf0)  | (tm.year%10 & 0x0f);
	dst[5] = tm.week & 0x0f;
	dst[4] = (tm.month / 10 << 4 &0xf0) | (tm.month % 10&0x0f);
	dst[3] = (tm.day / 10 << 4 & 0xf0) | (tm.day %10 & 0x0f);
	dst[2] = (tm.hour / 10 << 4 & 0xf0) | (tm.hour % 10 & 0x0f);
	dst[1] = (tm.min/ 10 << 4 & 0xf0) | (tm.min% 10 & 0x0f);
	dst[0] = (tm.sec/ 10 << 4 & 0xf0) | (tm.sec% 10 & 0x0f);

//	dst[0] = src[8]&0xf0+src[8]&0x0f;
//	dst[1] = src[7]&0xf0+src[7]&0x0f;

//	dst[2] = src[6]&0xf0+src[6]&0x0f;
//	dst[3] = src[5]&0xf0+src[5]&0x0f;;

//	dst[4] = src[4]&0xf0+src[4]&0x0f;
//	dst[5] = src[3];

//	dst[6] = src[2]&0xf0+src[2]&0x0f;
	Write_1302_Time(dst);

}


uint8_t ds1302_get_time_test()
{	
	uint8_t dst[7];
	ReadTime();


	dst[6] = ((utime.year & 0xf0)>>4)*10 + (utime.year & 0x0f);
	dst[5] = utime.week;
	dst[4] = ((utime.month & 0xf0)>>4)*10 + (utime.month & 0x0f);
	dst[3] = ((utime.day & 0xf0)>>4)*10 + (utime.day & 0x0f);
	dst[2] = ((utime.hour & 0xf0)>>4)*10 + (utime.hour & 0x0f);
	dst[1] = ((utime.min& 0xf0)>>4)*10 + (utime.min & 0x0f);
	dst[0] = ((utime.sec& 0xf0)>>4)*10 + (utime.sec& 0x0f);
	
	printf("year: %d, week: %d, mon: %d, day: %d, h: %d, m: %d, s: %d\n", 
		dst[6], dst[5], dst[4], dst[3], dst[2], dst[1], dst[0]);
	return 0;
}


