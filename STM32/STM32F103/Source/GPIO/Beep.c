/*
 * Beep.c
 *
 *  Created on: 2015年3月17日
 *      Author: souls
 */
#include "Beep.h"
#include "../System/Delay/SysTick.h"
volatile uint32_t beeptime = 0; // ms 计时变量
extern void CallbackNull(void);
void(*BeepCallback_ISR)(void) = CallbackNull;
uint32_t TIME_BEEPOFF,TIME_BEEPON,TIME_BEEPTIME;	
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
void BeepStop(void){
		BeepCallback_ISR = CallbackNull;	
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
////didi
void BeepDiDiCallback(){
if (TIME_BEEPON == beeptime ){
		BEEPOFF();	//关
	}
	else if ((TIME_BEEPOFF+TIME_BEEPON) == beeptime){
		BEEPON();
		beeptime = 0;
	}
	beeptime++;
}
void BeepDiDiStart(uint32_t on,uint32_t off){
		TIME_BEEPON =on;
		TIME_BEEPOFF =off;
		BEEPON();
		BeepCallback_ISR=BeepDiDiCallback;
}
