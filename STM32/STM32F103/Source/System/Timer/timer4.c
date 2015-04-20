

#include "timer4.h"
	// 高级控制定时器: p199, TIM1/TIM8
	// 基本定时器: p298, TIM6/TIM7
	// 通用定时器: p253, TIM2/3/4/5
extern const uint32_t MAX_UINT32;
volatile uint32_t time4 = 0; // ms 计时变量

void TIM4_Start(void) {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM4, ENABLE);
}

void TIM4_Stop(void) {
	TIM_Cmd(TIM4, DISABLE);
	TIM_ITConfig(TIM4, TIM_IT_Update, DISABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, DISABLE);	
}


void TIM4_Config(void){
	
	TIM_SetCounter(TIM4, 0);	
	TIM_SetAutoreload(TIM4, 1000);//重载值最大为65536
	TIM_PrescalerConfig(TIM4, (SystemCoreClock / 1000000 - 1), TIM_PSCReloadMode_Immediate);
	TIM4_Start();
}

//获取当前参数--->当前时间 差值

uint32_t TIM4_GetDistanceTime(uint32_t pretime) {
	return (MAX_UINT32 - pretime + time4 + 1);
}

uint32_t TIM4_GetCurrentTime(void) {
	return (time4);
}

extern void (*BeepCallback_ISR)(void);
extern void(*LEDCallback_ISR)(void);

void TIM4_IRQHandler(void) {
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) {
		time4++;
		//BeepCallback_ISR();
		//LEDCallback_ISR();
		TIM_ClearITPendingBit(TIM4, TIM_FLAG_Update);
	}
}
