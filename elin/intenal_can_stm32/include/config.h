#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "stm32f10x.h"

//µÁ—πºÏ≤‚
#define ADC9_Pin                GPIO_Pin_0 //PA 
#define ADC9_GPIO				GPIOA
#define ADC9_GPIO_Mode			GPIO_Mode_AIN
#define ADC9_RCC				RCC_APB2Periph_GPIOA

#define RS485_RE_Pin            GPIO_Pin_1 //PA
#define RS485_RE_GPIO			GPIOA
#define RS485_RE_GPIO_Mode		GPIO_Mode_Out_PP

#define RS485_TX_Pin		    GPIO_Pin_2	// out PA
#define RS485_TX_GPIO			GPIOA
#define RS485_TX_GPIO_Mode		GPIO_Mode_AF_PP

#define RS485_RX_Pin		    GPIO_Pin_3	// in  PA
#define RS485_RX_GPIO			GPIOA
#define RS485_RX_GPIO_Mode      GPIO_Mode_IN_FLOATING

#define ADC1_Pin				GPIO_Pin_4 //PA
#define ADC1_GPIO				GPIOA
#define ADC1_GPIO_Mode			GPIO_Mode_AIN
#define ADC1_RCC				RCC_APB2Periph_GPIOA


#define ADC2_Pin				GPIO_Pin_5 //PA
#define ADC2_GPIO				GPIOA
#define ADC2_GPIO_Mode			GPIO_Mode_AIN
#define ADC2_RCC				RCC_APB2Periph_GPIOA


#define ADC3_Pin   				GPIO_Pin_6 //PA
#define ADC3_GPIO				GPIOA
#define ADC3_GPIO_Mode			GPIO_Mode_AIN
#define ADC3_RCC				RCC_APB2Periph_GPIOA

#define ADC4_Pin				GPIO_Pin_7  //PA
#define ADC4_GPIO				GPIOA
#define ADC4_GPIO_Mode			GPIO_Mode_AIN
#define ADC4_RCC				RCC_APB2Periph_GPIOA

#define USART1_TX_Pin		    GPIO_Pin_9	// out PA
#define USART1_TX_GPIO			GPIOA
#define USART1_TX_GPIO_Mode		GPIO_Mode_AF_PP

#define USART1_RX_Pin		    GPIO_Pin_10	// in  PA
#define USART1_RX_GPIO			GPIOA
#define USART1_RX_GPIO_Mode		GPIO_Mode_IN_FLOATING	


#define LED4_Pin				GPIO_Pin_6 //PC
#define LED4_GPIO				GPIOC
#define LED4_GPIO_Mode          GPIO_Mode_Out_PP


#define LED3_Pin				GPIO_Pin_15 //PD
#define LED3_GPIO				GPIOD
#define LED3_GPIO_Mode			GPIO_Mode_Out_PP

#define JTMS				    GPIO_Pin_13 //PA
#define JTCK				    GPIO_Pin_14 //PA
#define JTDI				    GPIO_Pin_15 //PA

//PB
#define ADC7_Pin                GPIO_Pin_0 //PB
#define ADC7_GPIO				GPIOB
#define ADC7_GPIO_Mode			GPIO_Mode_AIN
#define ADC7_RCC				RCC_APB2Periph_GPIOB

#define ADC8_Pin				GPIO_Pin_1 //PB
#define ADC8_GPIO				GPIOB
#define ADC8_GPIO_Mode			GPIO_Mode_AIN
#define ADC8_RCC				RCC_APB2Periph_GPIOB

#define BOOT1					GPIO_Pin_2	// out PB
#define JTDO					GPIO_Pin_3	// in  PB

#define JTRST				    GPIO_Pin_4 //PB
#define DS1302_SCLK_Pin			GPIO_Pin_5 //PB
#define DS1302_SCLK_GPIO		GPIOB
#define DS1302_SCLK_GPIO_Mode	GPIO_Mode_Out_PP

#define I2C_SCL_Pin   			GPIO_Pin_6 //PB
#define I2C_SCL_GPIO			GPIOB
#define I2C_SCL_GPIO_Mode		GPIO_Mode_Out_PP

#define I2C_SDA_Pin			GPIO_Pin_7  //PB
#define I2C_SDA_GPIO			GPIOB
#define I2C_SDA_GPIO_Mode		GPIO_Mode_Out_PP

#define DS1302_RST_Pin			GPIO_Pin_8  //PB
#define DS1302_RST_GPIO			GPIOB
#define DS1302_RST_GPIO_Mode	GPIO_Mode_Out_PP

#define DS1302_IO_Pin			GPIO_Pin_9  //PB
#define DS1302_IO_GPIO			GPIOB
#define DS1302_IO_GPIO_Mode		GPIO_Mode_Out_PP
//#define 		        GPIO_Pin_10	// out PB
//#define USART3_RX		        GPIO_Pin_11	// in  PB

#define NET_CS_Pin				GPIO_Pin_12 //PB
#define NET_CS_GPIO				GPIOB
#define NET_CS_GPIO_Mode		GPIO_Mode_Out_PP

#define SPI2_SCK_Pin			GPIO_Pin_13 //PB
#define SPI2_SCK_GPIO			GPIOB
#define SPI2_SCK_GPIO_Mode		GPIO_Mode_AF_PP	


#define SPI2_MISO_Pin			GPIO_Pin_14 //PB
#define SPI2_MISO_GPIO			GPIOB
#define SPI2_MISO_GPIO_Mode		GPIO_Mode_AF_PP	

#define SPI2_MOSI_Pin			GPIO_Pin_15 //PB
#define SPI2_MOSI_GPIO			GPIOB
#define SPI2_MOSI_GPIO_Mode		GPIO_Mode_AF_PP	

//PC
#define JDQ5_Pin			GPIO_Pin_0 //PC
#define JDQ5_GPIO			GPIOC
#define JDQ5_GPIO_Mode		GPIO_Mode_Out_PP

#define JDQ6_Pin			GPIO_Pin_1 //PC
#define JDQ6_GPIO			GPIOC
#define JDQ6_GPIO_Mode		GPIO_Mode_Out_PP

#define JDQ7_Pin			GPIO_Pin_2	// out PC
#define JDQ7_GPIO			GPIOC
#define JDQ7_GPIO_Mode		GPIO_Mode_Out_PP

#define JDQ8_Pin			GPIO_Pin_3	// in  PC
#define JDQ8_GPIO			GPIOC
#define JDQ8_GPIO_Mode		GPIO_Mode_Out_PP

#define ADC5_Pin			GPIO_Pin_4 //PC
#define ADC5_GPIO			GPIOC
#define ADC5_GPIO_Mode		GPIO_Mode_Out_PP
#define ADC5_RCC				RCC_APB2Periph_GPIOC

#define ADC6_Pin			GPIO_Pin_5 //PC
#define ADC6_GPIO			GPIOC
#define ADC6_GPIO_Mode		GPIO_Mode_Out_PP
#define ADC6_RCC				RCC_APB2Periph_GPIOC

#define NET_INT_Pin   		GPIO_Pin_6 //PC
#define NET_INT_GPIO		GPIOC
#define NET_INT_GPIO_Mode	GPIO_Mode_IPU

#define NET_RST_Pin			GPIO_Pin_7  //PC
#define NET_RST_GPIO		GPIOC
#define NET_RST_GPIO_Mode	GPIO_Mode_Out_PP	

#define LED2_Pin		    GPIO_Pin_14	// out PC
#define LED2_GPIO				GPIOD
#define LED2_GPIO_Mode		GPIO_Mode_Out_PP

#define LED1_Pin		    GPIO_Pin_13	// in  PC
#define LED1_GPIO				GPIOD
#define LED1_GPIO_Mode		GPIO_Mode_Out_PP

#define JDQ1_Pin		    GPIO_Pin_14	// out PC
#define JDQ1_GPIO			GPIOC
#define JDQ1_GPIO_Mode		GPIO_Mode_Out_PP

#define JDQ2_Pin		    GPIO_Pin_11	// in  PC
#define JDQ2_GPIO			GPIOC
#define JDQ2_GPIO_Mode		GPIO_Mode_Out_PP

#define JDQ3_Pin			GPIO_Pin_12 //PC
#define JDQ3_GPIO			GPIOC
#define JDQ3_GPIO_Mode		GPIO_Mode_Out_PP
//#define SPI2_SCK				GPIO_Pin_13 //PC
#define BEEP_Pin			GPIO_Pin_14 //PC
#define BEEP_GPIO			GPIOC
#define BEEP_GPIO_Mode		GPIO_Mode_Out_PP
//#define OSC32_OUT				GPIO_Pin_15 //PC
//PD
//#define D0					GPIO_Pin_0 //PD
//#define D1					GPIO_Pin_1 //PD
#define JDQ4_Pin			GPIO_Pin_2	// out PD
#define JDQ4_GPIO			GPIOD
#define JDQ4_GPIO_Mode		GPIO_Mode_Out_PP
//#define BEEP					GPIO_Pin_3	// in  PD

#define DS1302_GPIO_CLOCK RCC_APB2Periph_GPIOB



#define Beep_ON	  GPIO_SetBits(BEEP_GPIO,BEEP_Pin);  
#define Beep_OFF  GPIO_ResetBits(BEEP_GPIO,BEEP_Pin); 

#define SOCK_CONFIG		2	// UDP
#define SOCK_DNS		2	// UDP
#define SOCK_DHCP		3	// UDP

#define MAX_BUF_SIZE		1460
#define KEEP_ALIVE_TIME	30	// 30sec

#define ON	0
#define OFF	1

#define HIGH		1
#define LOW		0

#define OK 0
#define ERROR 1

#define SER_SOCKET 2

#define ACK 0x06
#define NAK 0x15
#define ENQ 0x05

#define MAX_LINK_GROUP  16
#define MAX_LINK_IN	 	16
#define MAX_LINK_OUT	8
#define MAX_ALARM_OUT	8

#define ApplicationAddress 	0x08004000
#define CARD_MSG_START_ADDR       0x00

#define	MAX_SOCK_NUM		8	/**< Maxmium number of socket */
#define MAX_NUM_CARD_DOWNLOAD 20000
#define ERROR_CARD_NUM        0xFFFF

#define EEPROM_FLASH_SECTOR_LEN  0x1000 //(4 * 1024)
#define CARD_MSG_SAVED_LEN    32
#define CARDS_NUM_IN_A_SECTOR  (EEPROM_FLASH_SECTOR_LEN / CARD_MSG_SAVED_LEN)//128

#define EEPROM_TM_MODE_START_ADDR 			0x3c00 //15K

#define IINCHIP_ISR_DISABLE()  __disable_fault_irq()
#define IINCHIP_ISR_ENABLE() __enable_fault_irq()

#define USE_GPIOA_AS_USART1

#define TX_485_E1  GPIO_SetBits(GPIOC,RS485_RE1);  
#define RX_485_E1  GPIO_ResetBits(GPIOC,RS485_RE1); 

#define TX_485_E2  GPIO_SetBits(GPIOC,RS485_RE2);  
#define RX_485_E2  GPIO_ResetBits(GPIOC,RS485_RE2); 

#define TX_485_E3  GPIO_SetBits(GPIOC,RS485_RE3);  
#define RX_485_E3  GPIO_ResetBits(GPIOC,RS485_RE3); 

#define NET_UDP_ALIVE 							0x0
#define NET_UDP_ALIVE_RSP 						0x1
#define NET_UDP_RSP_OK 							0x2
#define NET_UDP_RSP_ERROR 						0x3

#define NET_UDP_INPUT_ALARM_STAT 				0x4
#define NET_UDP_DETECTOR_LOSS 				0x5
#define NET_UDP_ALARM_BATTERY_LOW_POWER 		0x6
#define NET_UDP_ALARM_NET_LOSS				0x7
#define NET_UDP_ALARM_POWEROFF				0x8
#define NET_UDP_LINK_SET						0x9
#define NET_UDP_ARMING      					0xA
#define NET_UDP_DISARMING					0xB
#define NET_UDP_SET_TIME						0xC
#define NET_UDP_GET_TIME					0xD
#define NET_UDP_STAT_GET					0xE
#define NET_UDP_SET_CELL_NUM					0xF
#define NET_UDP_SET_TEXT_NUM					0x10
#define NET_UDP_SET_DETECTOR_SENS				0x11
#define NET_UDP_GET_ARM_STAT					0x12
#define NET_UDP_GET_LINK_STAT					0x13
#define NET_UDP_GET_DETECTOR_SENS				0x14
#define NET_UDP_UPLOAD_ARM_STAT				0x15
#define NET_UDP_GET_BALANCE					0x16
#define NET_UDP_CANCEL_TEXT_ALARM				0x17
#define NET_UDP_CANCEL_CELL_ALARM				0x18
#define NET_UDP_SET_ARM_LINK					0x19
#define NET_UDP_SET_DISARM_LINK				0x1A
#define NET_UDP_GET_ARM_LINK					0x1b
#define NET_UDP_GET_DISARM_LINK				0x1c
#define NET_UDP_GET_CELL_NUM					0x1D
#define NET_UDP_GET_TEXT_NUM					0x1E
#define NET_UDP_GET_CH_ENABLE					0x1F
#define NET_UDP_SET_CH_ENABLE					0x20
#define NET_UDP_GET_INTERVAL					0x21
#define NET_UDP_SET_INTERVAL					0x22
#define NET_UDP_STAT_UPLOAD					0x23
#define NET_UDP_CANCEL_MISTAKE_ALARM			0x24
#define NET_UDP_GET_DISARM					0x25
#define NET_UDP_SET_ALARM_OUT				0x26
#define NET_UDP_GET_ALARM_OUT				0x27
#define NET_UDP_UPLOAD_DISARM_STAT				0x28
#define NET_UDP_SET_END_KEY					0x29
#define NET_UDP_GET_END_KEY					0x2A



#define NET_UDP_INPUT_ALARM_STAT_RSP				0x54
#define NET_UDP_DETECTOR_LOSS_RSP 					0x55
#define NET_UDP_ALARM_BATTERY_LOW_POWER_RSP 			0x56
#define NET_UDP_ALARM_NET_LOSS_RSP					0x57
#define NET_UDP_ALARM_POWEROFF_RSP					0x58
#define NET_UDP_LINK_SET_RSP						0x59
#define NET_UDP_ARMING_RSP      					0x5A
#define NET_UDP_DISARMING_RSP						0x5B
#define NET_UDP_SET_TIME_RSP						0x5C
#define NET_UDP_GET_TIME_RSP						0x5D
#define NET_UDP_STAT_GET_RSP						0x5E
#define NET_UDP_SET_CELL_NUM_RSP					0x5F
#define NET_UDP_SET_TEXT_NUM_RSP					0x60
#define NET_UDP_SET_DETECTOR_SENS_RSP				0x61
#define NET_UDP_GET_ARM_STAT_RSP					0x62
#define NET_UDP_GET_LINK_STAT_RSP					0x63
#define NET_UDP_GET_DETECTOR_SENS_RSP				0x64
#define NET_UDP_UPLOAD_ARM_STAT_RSP				0x65
#define NET_UDP_GET_BALANCE_RSP					0x66
#define NET_UDP_CANCEL_TEXT_ALARM_RSP				0x67
#define NET_UDP_CANCEL_CELL_ALARM_RSP				0x68
#define NET_UDP_SET_ARM_LINK_RSP					0x69
#define NET_UDP_SET_DISARM_LINK_RSP					0x6A
#define NET_UDP_GET_ARM_LINK_RSP					0x6B
#define NET_UDP_GET_DISARM_LINK_RSP					0x6C
#define NET_UDP_GET_CELL_NUM_RSP					0x6D
#define NET_UDP_GET_TEXT_NUM_RSP					0x6E
#define NET_UDP_GET_CH_ENABLE_RSP					0x6F
#define NET_UDP_SET_CH_ENABLE_RSP					0x70
#define NET_UDP_GET_INTERVAL_RSP					0x71
#define NET_UDP_SET_INTERVAL_RSP					0x72
#define NET_UDP_STAT_UPLOAD_RSP					0x73
#define NET_UDP_CANCEL_MISTAKE_ALARM_RSP				0x74
#define NET_UDP_GET_DISARM_RSP					0x75
#define NET_UDP_SET_ALARM_OUT_RSP					0x76
#define NET_UDP_GET_ALARM_OUT_RSP					0x77
#define NET_UDP_UPLOAD_DISARM_STAT_RSP				0x78
#define NET_UDP_SET_END_KEY_RSP					0x79
#define NET_UDP_GET_END_KEY_RSP					0x7A

#endif

