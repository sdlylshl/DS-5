/*
 * RS485.c
 *
 *  Created on: 2015年3月20日
 *      Author: souls
 */

#include "../System/Usart/usart2.h"

#include "RS485.h"
void RS485_GPIO_Config(void) {
	GPIO_InitTypeDef GPIO_InitStructure; //定义结构体

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
#if RS485_USE
	GPIO_InitStructure.GPIO_Pin = RS485_PIN;
	GPIO_InitStructure.GPIO_Mode = RS485_MODE; //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(RS485_PORT, &GPIO_InitStructure);
#endif
	RS485_RX();
}

void RS485_init() {
	RS485_GPIO_Config();
	USART2_Init();

}
