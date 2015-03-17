#include "timer2.h"

//LED();
#include "config.h"

uint32_t (*GetCurrentTime)(void) =TIM2_GetCurrentTime;
uint32_t (*GetDistanceTime)(uint32_t ) = TIM2_GetDistanceTime;

const uint32_t MAX_UINT32 = (uint32_t) -1;

volatile uint32_t time2 = 0; // ms 计时变量
volatile uint32_t Zigbee_time; // ms 计时变量
volatile uint32_t Net_time; // ms 计时变量
volatile uint32_t time_out;
volatile uint32_t timer_Device_update;
volatile uint32_t timer_Zigbee_getStatus;
volatile uint32_t timer_Zigbee_getBattery;
volatile uint32_t timer_Zigbee_sendBuff;
volatile uint32_t timer_Zigbee_ClearRequest;
volatile uint32_t timer_Zigbee_operate_default;
volatile uint32_t timer_curtain_stop;
volatile uint32_t timer_Zigbee_reset_default;

/*
 * 函数名：TIM2_NVIC_Configuration
 * 描述  ：TIM2中断优先级配置
 * 输入  ：无
 * 输出  ：无
 */
void TIM2_NVIC_Configuration(void) {
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/*TIM_Period--1000   TIM_Prescaler--71 -->中断周期为1ms*/
void TIM2_Init(void) {
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_DeInit(TIM2);
	//TIM2_NVIC_Configuration();
	TIM_TimeBaseStructure.TIM_Period = 1000;
	/* 累计 TIM_Period个频率后产生一个更新或者中断 */
	TIM_TimeBaseStructure.TIM_Prescaler = (72 - 1);
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, DISABLE); /*先关闭等待使用*/
}

void TIM2_Start(void) {
	time2 = 0;
	TIM2_Init();
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
}

void TIM2_Stop(void) {
	TIM_Cmd(TIM2, DISABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, DISABLE);
}

uint32_t TIM2_GetDistanceTime(uint32_t pretime) {
	return (MAX_UINT32 - pretime + time2 + 1);
}

uint32_t TIM2_GetCurrentTime(void) {
	return (time2);
}
void TIM2_ISR(void){

		time2++;
		/*	Zigbee_time++;
			Net_time++;
			time_out++;
			timer_Device_update++;
			timer_Zigbee_getStatus++;
			timer_Zigbee_getBattery++;
			timer_Zigbee_sendBuff++;
			timer_Zigbee_ClearRequest++;
			timer_Zigbee_operate_default++;
			timer_Zigbee_reset_default++;
			if (timer_curtain_stop) {
			timer_curtain_stop++;
			}*/

}

