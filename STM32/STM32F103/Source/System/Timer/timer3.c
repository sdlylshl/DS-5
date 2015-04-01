#include "timer3.h"

//LED3();
#include "config.h"
volatile uint32_t time3 = 0; // ms 计时变量

/*TIM_Period--1000   TIM_Prescaler--71 -->中断周期为1ms*/
void TIM3_Init(void) {
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_DeInit(TIM3);
	TIM_TimeBaseStructure.TIM_Period = 1000;
	/* 累计 TIM_Period个频率后产生一个更新或者中断 */
	TIM_TimeBaseStructure.TIM_Prescaler = (72 - 1);
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM3, ENABLE);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, DISABLE); /*先关闭等待使用*/
}

void TIM3_Start(void) {
	time3 = 0;
	TIM3_Init();
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
}

void TIM3_Stop(void) {
	TIM_Cmd(TIM3, DISABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, DISABLE);
}

void TIM3_IRQ(void) {
		time3++;

		if (time3 > 500) {
			LED3(1);
		}
		if (time3 > 1000) {

			LED3(0);
			time3 = 0;

		}

}
extern void TIM3_IRQ(void);
void TIM3_IRQHandler(void) {
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
		
		TIM3_IRQ();
		TIM_ClearITPendingBit(TIM3, TIM_FLAG_Update);
	}
}
