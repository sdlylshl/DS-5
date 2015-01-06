#include "timer4.h"

extern const uint32_t MAX_UINT32;
volatile uint32_t time4 = 0; // ms 计时变量
void TIM4_NVIC_Configuration(void) {
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/*TIM_Period--1000   TIM_Prescaler--71 -->中断周期为1ms*/
void TIM4_Config(void) {
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	TIM_DeInit(TIM4);
	TIM4_NVIC_Configuration();
	TIM_TimeBaseStructure.TIM_Period = 1000;
	/* 累计 TIM_Period个频率后产生一个更新或者中断 */
	TIM_TimeBaseStructure.TIM_Prescaler = (SystemCoreClock / 1000000 - 1); //(72 - 1);
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM4, ENABLE);

	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , DISABLE);      /*先关闭等待使用*/
}

void TIM4_Start(void) {
	time4 = 0;
	TIM4_Config();
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	TIM_Cmd(TIM4, ENABLE);
}

void TIM4_Stop(void) {
	TIM_Cmd(TIM4, DISABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, DISABLE);
}
//获取当前参数--->当前时间 差值

uint32_t TIM4_GetDistanceTime(uint32_t pretime) {
	return (MAX_UINT32 - pretime + time4 + 1);
}

uint32_t TIM4_GetCurrentTime(void) {
	return (time4);
}
void TIM4_IRQHandle(void) {
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) {
		TIM_ClearITPendingBit(TIM4, TIM_FLAG_Update);
		time4++;
	}
}

