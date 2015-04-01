/*
 * timer5.c
 *
 *  Created on: 2015年3月2日
 *  Author: lishoulei
 */

#include "timer5.h"

extern const uint32_t MAX_UINT32;
volatile uint32_t timer5 = 0; // ms 计时变量

void TIM5_Start(void) {
	//1. TIM5功能启用
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	//2. 开启中断
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
	//3. 启动计时器
	TIM_Cmd(TIM5, ENABLE);
}

void TIM5_Stop(void) {
	TIM_Cmd(TIM5, DISABLE);
	TIM_ITConfig(TIM5, TIM_IT_Update, DISABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, DISABLE);	
}


void TIM5_Config(void){
	
	TIM_SetCounter(TIM5, 0);
	//TIM_SetAutoreload(TIM5, timeseting.Period);
	//TIM_PrescalerConfig(TIM5, timeseting.Prescaler, TIM_PSCReloadMode_Immediate);
	
	TIM_SetAutoreload(TIM5, 1000);//重载值最大为65536
	TIM_PrescalerConfig(TIM5, (SystemCoreClock / 1000000 - 1), TIM_PSCReloadMode_Immediate);

	TIM5_Start();
	//	//重新初始化定时器
//	TIM_SelectOutputTrigger(TIM5, TIM_TRGOSource_Update);
}

uint32_t TIM5_GetDistanceTime(uint32_t pretime) {
	return (MAX_UINT32 - pretime + timer5 + 1);
}

uint32_t TIM5_GetCurrentTime(void) {
	return (timer5);
}
void TIM5_IRQHandler(void) {
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET) {
		timer5++;
		
		TIM_ClearITPendingBit(TIM5, TIM_FLAG_Update);
	}
}
