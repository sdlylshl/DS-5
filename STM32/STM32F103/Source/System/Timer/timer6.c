/*
 * timer6.c
 *
 *  Created on: 2015年3月2日
 *  Author: lishoulei
 */

#include "timer6.h"

extern const uint32_t MAX_UINT32;
volatile uint32_t time6 = 0; // ms 计时变量

void TIM6_Start(void) {
	//1. TIM6功能启用
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	//2. 开启中断
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
	//3. 启动计时器
	TIM_Cmd(TIM6, ENABLE);
}

void TIM6_Stop(void) {
	TIM_Cmd(TIM6, DISABLE);
	TIM_ITConfig(TIM6, TIM_IT_Update, DISABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, DISABLE);	
}


void TIM6_Config(void){
	
	TIM_SetCounter(TIM6, 0);
	//TIM_SetAutoreload(TIM6, timeseting.Period);
	//TIM_PrescalerConfig(TIM6, timeseting.Prescaler, TIM_PSCReloadMode_Immediate);
	
	TIM_SetAutoreload(TIM6, 1000);//重载值最大为65536
	TIM_PrescalerConfig(TIM6, (SystemCoreClock / 1000000 - 1), TIM_PSCReloadMode_Immediate);

	TIM6_Start();
	//	//重新初始化定时器
//	TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);
}

uint32_t TIM6_GetDistanceTime(uint32_t pretime) {
	return (MAX_UINT32 - pretime + time6 + 1);
}

uint32_t TIM6_GetCurrentTime(void) {
	return (time6);
}
void TIM6_IRQHandler(void) {
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET) {
		time6++;
		
		TIM_ClearITPendingBit(TIM6, TIM_FLAG_Update);
	}
}
