/*
 * timer8.c
 *
 *  Created on: 2015年3月2日
 *  Author: lishoulei
 */

#include "timer8.h"

extern const uint32_t MAX_UINT32;
volatile uint32_t timer8 = 0; // ms 计时变量

void TIM8_Start(void) {
	//1. TIM8功能启用
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
	//2. 开启中断
	TIM_ITConfig(TIM8, TIM_IT_Update, ENABLE);
	//3. 启动计时器
	TIM_Cmd(TIM8, ENABLE);
}

void TIM8_Stop(void) {
	TIM_Cmd(TIM8, DISABLE);
	TIM_ITConfig(TIM8, TIM_IT_Update, DISABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, DISABLE);	
}


void TIM8_Config(void){
	
	TIM_SetCounter(TIM8, 0);
	//TIM_SetAutoreload(TIM8, timeseting.Period);
	//TIM_PrescalerConfig(TIM8, timeseting.Prescaler, TIM_PSCReloadMode_Immediate);
	
	TIM_SetAutoreload(TIM8, 1000);//重载值最大为65536
	TIM_PrescalerConfig(TIM8, (SystemCoreClock / 1000000 - 1), TIM_PSCReloadMode_Immediate);

	TIM8_Start();
	//	//重新初始化定时器
//	TIM_SelectOutputTrigger(TIM8, TIM_TRGOSource_Update);
}

uint32_t TIM8_GetDistanceTime(uint32_t pretime) {
	return (MAX_UINT32 - pretime + timer8 + 1);
}

uint32_t TIM8_GetCurrentTime(void) {
	return (timer8);
}

void TIM8_BRK_IRQHandler(void) {
}
	
void TIM8_UP_IRQHandler(void) {
	if (TIM_GetITStatus(TIM8, TIM_IT_Update) != RESET) {
		timer8++;
		
		TIM_ClearITPendingBit(TIM8, TIM_FLAG_Update);
	}
}

void TIM8_TRG_COM_IRQHandler(void) {
}
void TIM8_CC_IRQHandler(void) {
}