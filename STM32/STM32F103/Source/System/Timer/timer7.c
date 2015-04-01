/*
 * timer7.c
 *
 *  Created on: 2015年3月2日
 *  Author: lishoulei
 */

#include "timer7.h"

extern const uint32_t MAX_UINT32;
volatile uint32_t timer7 = 0; // ms 计时变量

void TIM7_Start(void) {
	//1. TIM7功能启用
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
	//2. 开启中断
	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
	//3. 启动计时器
	TIM_Cmd(TIM7, ENABLE);
}

void TIM7_Stop(void) {
	TIM_Cmd(TIM7, DISABLE);
	TIM_ITConfig(TIM7, TIM_IT_Update, DISABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, DISABLE);	
}


void TIM7_Config(void){
	
	TIM_SetCounter(TIM7, 0);
	//TIM_SetAutoreload(TIM7, timeseting.Period);
	//TIM_PrescalerConfig(TIM7, timeseting.Prescaler, TIM_PSCReloadMode_Immediate);
	
	TIM_SetAutoreload(TIM7, 1000);//重载值最大为65536
	TIM_PrescalerConfig(TIM7, (SystemCoreClock / 1000000 - 1), TIM_PSCReloadMode_Immediate);

	TIM7_Start();
	//	//重新初始化定时器
//	TIM_SelectOutputTrigger(TIM7, TIM_TRGOSource_Update);
}

uint32_t TIM7_GetDistanceTime(uint32_t pretime) {
	return (MAX_UINT32 - pretime + timer7 + 1);
}

uint32_t TIM7_GetCurrentTime(void) {
	return (timer7);
}
void TIM7_IRQHandler(void) {
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET) {
		timer7++;
		
		TIM_ClearITPendingBit(TIM7, TIM_FLAG_Update);
	}
}
