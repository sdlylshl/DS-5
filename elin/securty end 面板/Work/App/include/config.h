#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "stm32f10x.h"
#include "Types.h"
//=================================================


//Port A

#define PA0_A_B_C		    		GPIO_Pin_0//A B C
#define RS485_EN			GPIO_Pin_1
#define WIZ_INT		    	GPIO_Pin_1
#define USART2_TX		    GPIO_Pin_2	// out
#define USART2_RX		    GPIO_Pin_3	// in 
#define WIZ_SCS			    GPIO_Pin_4	// out
#define WIZ_SCLK			GPIO_Pin_5	// out
#define WIZ_MISO			GPIO_Pin_6	// in
#define WIZ_MOSI			GPIO_Pin_7	// out
#define SIM900A_RST			GPIO_Pin_8	// 
#define USART1_TX		    GPIO_Pin_9	// out
#define USART1_RX		    GPIO_Pin_10	// in 
#define SIM900A_POW			GPIO_Pin_11	// 
#define PA12				GPIO_Pin_12	// 
#define PA13				GPIO_Pin_13	// 
#define PA14				GPIO_Pin_14	// out
#define PA15				GPIO_Pin_15	// out

//Port B

#define PB0_1_2_3_F1			        GPIO_Pin_0	// 1 2 3 f1
#define PB1_4_5_6_F2 					GPIO_Pin_1	// 4 5 6 f2
#define PB2								GPIO_Pin_2	// 
#define PB3								GPIO_Pin_3	// 
#define PB4								GPIO_Pin_4	// 
#define DS_SCLK							GPIO_Pin_5	// 
#define I2C_SCK	            			GPIO_Pin_6  //out
#define I2C_SDA	            			GPIO_Pin_7  //bidirection
#define DS_IO							GPIO_Pin_8
#define DS_RST							GPIO_Pin_9
#define PB10_7_8_9_F3			  		GPIO_Pin_10	// 7 8 9 f3
#define PB11_STAR_0_WELL_F4				GPIO_Pin_11	// * 0 # f4 
#define USART3_TX						GPIO_Pin_10
#define USART3_RX						GPIO_Pin_11
#define PB12_F1_F2_F3_F4_C			    GPIO_Pin_12	// F1 F2 F3 F4 C
#define PB13_3_6_9_WELL_B				GPIO_Pin_13	// 3 6 9 # B
#define PB14_2_5_8_0_A					GPIO_Pin_14 // 2 5 8 0 A
#define PB15_1_4_7_STAR					GPIO_Pin_15 // 1 4 7 *

#if 1
//Port C
#define LM3123_DB2				GPIO_Pin_0
#define LM3123_DB3				GPIO_Pin_1
#define LM3123_DB4				GPIO_Pin_2
#define LM3123_DB5				GPIO_Pin_3
#define LM3123_DB6				GPIO_Pin_4
#define LM3123_DB7				GPIO_Pin_5
#define LM3123_DB1				GPIO_Pin_6
#define LM3123_DB0				GPIO_Pin_7
#define LM3123_RW				GPIO_Pin_8
#define LM3123_A0				GPIO_Pin_9
#define LM3123_E1				GPIO_Pin_10
#define LM3123_E2				GPIO_Pin_11
#define LM3123_E3				GPIO_Pin_12
#define LM3123_RES				GPIO_Pin_13	//
#endif

//Port c
#define L_1602A_D7				GPIO_Pin_0
#define L_1602A_D6				GPIO_Pin_1
#define L_1602A_D5				GPIO_Pin_2
#define L_1602A_D4				GPIO_Pin_3
#define L_1602A_D3				GPIO_Pin_4
#define L_1602A_D2				GPIO_Pin_5
#define L_1602A_D1				GPIO_Pin_6
#define L_1602A_D0				GPIO_Pin_7
#define LCD_LED					GPIO_Pin_8
#define L_1602A_RS				GPIO_Pin_10
#define L_1602A_RW				GPIO_Pin_11
#define L_1602A_E				GPIO_Pin_12
#define BEEP					GPIO_Pin_14	//
#define KEY_LED					GPIO_Pin_15	// 

// Port D

#define LED5				GPIO_Pin_2

#define IO_INPUT_3			GPIO_Pin_0
#define IO_INPUT_4			GPIO_Pin_1
#define IO_INPUT_5			GPIO_Pin_2
#define IO_INPUT_6			GPIO_Pin_3
#define IO_INPUT_7			GPIO_Pin_4
#define IO_INPUT_8			GPIO_Pin_5
#define IO_INPUT_9			GPIO_Pin_6
#define IO_INPUT_10			GPIO_Pin_7


//Port E

#define IO_INPUT_14			GPIO_Pin_0
#define IO_INPUT_15			GPIO_Pin_1
#define KEY0				GPIO_Pin_2
#define KEY1				GPIO_Pin_3
#define KEY2				GPIO_Pin_4
#define KEY3				GPIO_Pin_5
//#define BEEP				GPIO_Pin_7
#define LED0_P				GPIO_Pin_8
#define LED0_S				GPIO_Pin_9
#define LED1_P				GPIO_Pin_10
#define LED1_S				GPIO_Pin_11
#define LED2_P				GPIO_Pin_12
#define LED2_S				GPIO_Pin_13
#define LED3_P				GPIO_Pin_14
#define LED3_S				GPIO_Pin_15





//=================================================
typedef struct _CONFIG_MSG
{
	uint8 Mac[6];
	uint8 Lip[4];
	uint8 Sub[4];
	uint8 Gw[4];
	uint8 DNS_Server_IP[4];	
	uint8  DHCP;
	uint16 any_port;
}CONFIG_MSG;


typedef struct _CONFIG_TYPE_DEF
{
	uint16 port;
	uint8 destip[4];
}CHCONFIG_TYPE_DEF;

typedef struct
{
	u8 link_group;
	u8 xor[16];
	u16 out[16];
	u32 in[16];
}link_para;


typedef struct
{	
	u8 cell_enable;
	u8 cell_group;
	u8 cell_xor;
	u8 cell[5][12];
}cell_para;


typedef struct
{
	u8 text_group;
	u8 text_enable;
	u8 text_length[5];
	u8 text[5][12];
	u8 text_content[5][140];
	u8 send_ok[5];
	u32 time[5];
	
}text_para;



typedef struct
{
	u32 arming;
	u32 machine_id;
} sys_para;
	
#define SOCK_CONFIG		2	// UDP
#define SOCK_DNS		2	// UDP
#define SOCK_DHCP		3	// UDP

#define MAX_BUF_SIZE		1460
#define KEEP_ALIVE_TIME	30	// 30sec

#define ON	1
#define OFF	0

#define HIGH		1
#define LOW		0


#define MAX_ALARM_IN 	16
#define MAX_LINK_GROUP  16
#define MAX_LINK_IN	 	16
#define MAX_LINK_OUT	8
#define MAX_ALARM_OUT	8
#define MAX_KEY_NUM		4
#define MAX_LIVE_TIMES	5
#define LIVE_INTERVAL	3


#define GSM_STAT_CLOSE 			0
#define GSM_STAT_INIT 			1
#define GSM_STAT_GPRS_INIT 		2
#define GSM_STAT_GPRS_CLOSE 	3

#define GSM_STAT_ERROR 			5

#define POWER_OFF_THR	20
#define BATTERY_LOWPOW_THR 2800

#define KEY_NORMAL 1
#define KEY_PRESSED 0
//#define __GNUC__

// SRAM address range is 0x2000 0000 ~ 0x2000 4FFF (20KB)
#define TX_RX_MAX_BUF_SIZE	2048
//#define TX_BUF	0x20004000
//#define RX_BUF	(TX_BUF+TX_RX_MAX_BUF_SIZE)

 
#define MAX_FLASH_PAGE_CAPICITY 2048
#define ApplicationAddress 	0x08004000


#endif

