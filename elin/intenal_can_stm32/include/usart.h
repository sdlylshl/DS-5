#ifndef __USART_H
#define __USART_H
#include "Types.h"

extern uint8_t rxBuffer[256];
extern uint8_t rx_rd;
extern uint8_t rx_wr;
extern Boolean flag_rs485_card_nr[3];

void UART1_Put_Char(unsigned char DataToSend);
void UART2_Put_Char(unsigned char DataToSend);
void UART3_Put_Char(unsigned char DataToSend);
void UART4_Put_Char(unsigned char DataToSend);
void UART5_Put_Char(unsigned char DataToSend);
void decode_rs232_buf(void);
void usart_gpio_init(void);
void usart_init(void);
#endif
