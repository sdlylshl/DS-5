#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "stm32f10x.h"

#define NET_RST                 GPIO_Pin_0 //PA
#define NET_INT                 GPIO_Pin_1 //PA

#define USART2_TX		        GPIO_Pin_2	// out PA
#define USART2_RX		        GPIO_Pin_3	// in  PA


#define SPI1_SCK				GPIO_Pin_5 //PA
#define SPI1_MISO   			GPIO_Pin_6 //PA
#define SPI1_MOSI				GPIO_Pin_7  //PA

#define USART1_TX		        GPIO_Pin_9	// out PA
#define USART1_RX		        GPIO_Pin_10	// in  PA

#define JTMS				    GPIO_Pin_13 //PA
#define JTCK				    GPIO_Pin_14 //PA
#define JTDI				    GPIO_Pin_15 //PA

//PB
#define DS_SCLK                 GPIO_Pin_0 //PB
#define DS_IO					GPIO_Pin_1 //PB

#define BOOT1					GPIO_Pin_2	// out PB
#define JTDO					GPIO_Pin_3	// in  PB

#define JTRST				    GPIO_Pin_4 //PB
//#define SPI1_SCK				GPIO_Pin_5 //PB
#define I2C1_SCL   				GPIO_Pin_6 //PB
#define I2C1_SDA				GPIO_Pin_7  //PB

#define USART3_TX		        GPIO_Pin_10	// out PB
#define USART3_RX		        GPIO_Pin_11	// in  PB

#define F_CS					GPIO_Pin_12 //PB
#define SPI2_SCK				GPIO_Pin_13 //PB
#define SPI2_MISO				GPIO_Pin_14 //PB
#define SPI2_MOSI				GPIO_Pin_15 //PB

//PC
#define D0						GPIO_Pin_0 //PC
#define D1						GPIO_Pin_1 //PC
#define D2						GPIO_Pin_2	// out PC
#define D3						GPIO_Pin_3	// in  PC

#define NET_CS				    GPIO_Pin_4 //PC
#define DS_RST					GPIO_Pin_5 //PC
//#define I2C1_SCL   			GPIO_Pin_6 //PC
#define RS485_RE1				GPIO_Pin_7  //PC
#define RS485_RE2		        GPIO_Pin_8	// out PC
#define RS485_RE3		        GPIO_Pin_9	// in  PC

#define UART4_TX		        GPIO_Pin_10	// out PC
#define UART4_RX		        GPIO_Pin_11	// in  PC

#define UART5_TX				GPIO_Pin_12 //PC
//#define SPI2_SCK				GPIO_Pin_13 //PC
#define OSC32_IN				GPIO_Pin_14 //PC
#define OSC32_OUT				GPIO_Pin_15 //PC
//PD
//#define D0					GPIO_Pin_0 //PD
//#define D1					GPIO_Pin_1 //PD
#define UART5_RX		        GPIO_Pin_2	// out PD
#define BEEP					GPIO_Pin_3	// in  PD

#define LED1_OUT				GPIO_Pin_4 //PD
#define LED2_OUT				GPIO_Pin_5 //PD
#define LED3_OUT				GPIO_Pin_6 //PD
#define LED4_OUT				GPIO_Pin_7  //PD

#define CPU1					GPIO_Pin_8	// out PD
#define CPU2					GPIO_Pin_9	// in  PD
#define CPU3					GPIO_Pin_10	// out PD
#define CPU4					GPIO_Pin_11	// in  PD
#define CPU5				    GPIO_Pin_12 //PD
#define CPU6				    GPIO_Pin_13 //PD
#define CPU7				    GPIO_Pin_14 //PD
#define CPU8				    GPIO_Pin_15 //PD
//PE
//#define D0					GPIO_Pin_0 //PE
//#define D1					GPIO_Pin_1 //PE
#define RELAY1					GPIO_Pin_2	// out PE
#define RELAY2					GPIO_Pin_3	// in  PE
#define RELAY3					GPIO_Pin_4 //PE
#define RELAY4					GPIO_Pin_5 //PE

//#define LED3_OUT				GPIO_Pin_6 //PE
//#define LED4_OUT				GPIO_Pin_7  //PE

//韦根
#define WIGEN_1_D0				GPIO_Pin_8   //PE 
#define WIGEN_1_D1				GPIO_Pin_9   //PE 
#define WIGEN_2_D0				GPIO_Pin_10  //PE 
#define WIGEN_2_D1				GPIO_Pin_11  //PE 
#define WIGEN_3_D0				GPIO_Pin_12  //PE 
#define WIGEN_3_D1				GPIO_Pin_13  //PE 
#define WIGEN_4_D0				GPIO_Pin_14  //PE 
#define WIGEN_4_D1				GPIO_Pin_15  //PE 

#define LED1 PDout(4)// PD13
#define LED2 PDout(5)// PD14	
#define LED3 PDout(6)// PD15
#define LED4 PDout(7) // PC6
//吐卡机取卡按键
#define INDUCTION_CAR     GPIO_Pin_12
#define Induction_Car 	  PDin(12)     // PD12

#define Key_D0            PCin(0)
#define Key_D1            PCin(1)
#define Key_D2            PCin(2)
#define Key_D3            PCin(3)

#define Line_RS485_RE1    PAin(3)
#define Line_RS485_RE2    PBin(11)
#define Line_RS485_RE3    PCin(11)

#define Beep              PDout(3) //PD

#define KEY_CARD          GPIO_Pin_8  // PB2
#define Key_Card    	  PDin(8) 

#define Line_CPU1          PDin(8)
#define Line_CPU2          PDin(9)
#define Line_CPU3          PDin(10)
#define Line_CPU4          PDin(11)
#define Line_CPU5          PDin(12)
#define Line_CPU6          PDin(13)
#define Line_CPU7          PDin(14)
#define Line_CPU8          PDin(15)

#define Line_Relay1            PEout(2)
#define Line_Relay2            PEout(3)
#define Line_Relay3            PEout(4)
#define Line_Relay4            PEout(5)

#define Line_wigen1_d0          PEin(8)
#define Line_wigen1_d1          PEin(9)
#define Line_wigen2_d0          PEin(10)
#define Line_wigen2_d1          PEin(11)
#define Line_wigen3_d0          PEin(12)
#define Line_wigen3_d1          PEin(13)
#define Line_wigen4_d0          PEin(14)
#define Line_wigen4_d1          PEin(15)

#define SOCK_CONFIG		2	// UDP
#define SOCK_DNS		2	// UDP
#define SOCK_DHCP		3	// UDP

#define MAX_BUF_SIZE		1460
#define KEEP_ALIVE_TIME	30	// 30sec

#define ON	1
#define OFF	0

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

#define IINCHIP_ISR_DISABLE()  __disable_fault_irq()
#define IINCHIP_ISR_ENABLE() __enable_fault_irq()

#define USE_GPIOA_AS_USART1

#define TX_485_E1  GPIO_SetBits(GPIOC,RS485_RE1);  
#define RX_485_E1  GPIO_ResetBits(GPIOC,RS485_RE1); 

#define TX_485_E2  GPIO_SetBits(GPIOC,RS485_RE2);  
#define RX_485_E2  GPIO_ResetBits(GPIOC,RS485_RE2); 

#define TX_485_E3  GPIO_SetBits(GPIOC,RS485_RE3);  
#define RX_485_E3  GPIO_ResetBits(GPIOC,RS485_RE3); 


#endif

