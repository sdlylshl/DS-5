/*
 * Beep.c
 *
 *  Created on: 2015年3月17日
 *      Author: souls
 */
#include "Beep.h"
#include "../System/Delay/SysTick.h"
volatile uint32_t beeptime = 0; // ms 计时变量
void(*BeepCallback_ISR)(void) = CallbackNull;
	
extern void Delay_ms(__IO uint32_t ms);
void BEEP_GPIO_Config(void) {
	GPIO_InitTypeDef GPIO_InitStructure; //定义结构体

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
#if BEEP_USE
	GPIO_InitStructure.GPIO_Pin = BEEP_PIN;
	GPIO_InitStructure.GPIO_Mode = BEEP_MODE; //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(BEEP_PORT, &GPIO_InitStructure);
#endif
}

void Beep(uint32_t ms){

	BEEP_GPIO_Config();
	BEEPON();
	Delay_ms(60);
	BEEPOFF();
}

void BeepCallback(void){

	if(!beeptime){
		BEEPOFF();
		BeepCallback_ISR = CallbackNull;
	}
		beeptime--;
}

void BeepStart(uint32_t ms){
	beeptime = ms;
	BEEP_GPIO_Config();
	BEEPON();
	BeepCallback_ISR=BeepCallback;
}
