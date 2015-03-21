/*
 * RS485.h
 *
 *  Created on: 2015年3月20日
 *      Author: souls
 */

#ifndef RS485_H_
#define RS485_H_
#include "stm32f10x.h"
#include "../System/Usart/usart2.h"
#define  RS485_USE		 1
#define  RS485_PORT     GPIOA
#define  RS485_PIN      GPIO_Pin_1
#define  RS485_MODE     GPIO_Mode_Out_PP

#define RS485_RX() GPIO_ResetBits(RS485_PORT,RS485_PIN)
#define RS485_TX() GPIO_SetBits(RS485_PORT,RS485_PIN)
#define RS485_SendChar USART2_SendChar
extern void RS485_init(void);
#endif /* RS485_H_ */
