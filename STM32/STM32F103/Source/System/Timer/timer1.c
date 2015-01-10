#include "timer1.h"

volatile uint32_t time1 = 0; // ms 计时变量

#if TIM1_NVIC
static void TIM1_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
#endif
void TIM1_Config(void) {
	TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;
	//Step1.时钟设置：启动TIM1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	//TIM1 使用内部时钟
	//TIM_InternalClockConfig(TIM1);
	TIM_DeInit(TIM1);

	//Step3.TIM模块设置
	//设置预分频器分频系数71，即APB2=72M, TIM1_CLK=72/72=1MHz
	//TIM_Period（TIM1_ARR）=1000，计数器向上计数到1000后产生更新事件，计数值归零
	//TIM_RepetitionCounter(TIM1_RCR)=0，每次向上溢出都产生更新事件
	TIM_BaseInitStructure.TIM_Period = 1000;
	TIM_BaseInitStructure.TIM_Prescaler = 71;
	TIM_BaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_BaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_BaseInitStructure);
#if TIM1_NVIC
	//清中断，以免一启用中断后立即产生中断
	TIM_ClearFlag(TIM1, TIM_FLAG_Update);
	//Step2.中断NVIC设置：允许中断，设置优先级
	TIM1_NVIC_Configuration();
	//使能TIM1中断源
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
#endif		
	//TIM1总开关：开启
	TIM_Cmd(TIM1, ENABLE);
	//Step4.注册中断服务子程序
	//将中断服务函数添加到stm32f10x_it.c
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, DISABLE); /*先关闭等待使用*/
}
void TIM1_Start(void) {
	time1 = 0;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	TIM_Cmd(TIM1, ENABLE);
}

void TIM1_Stop(void) {
	TIM_Cmd(TIM1, DISABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, DISABLE);
}

// 中断服务子程序

void TIM1_UP_IRQHandle(void) {
	GPIOC->ODR ^= (1 << 4); //闪灯
	TIM_ClearITPendingBit(TIM1, TIM_FLAG_Update); //清中断
}
void TIM1_CC_IRQHandle(void) {
	u16 capture;
	if (TIM_GetITStatus(TIM1, TIM_IT_CC1) == SET) {
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);
		capture = TIM_GetCapture1(TIM1);
		TIM_SetCompare1(TIM1, capture + 2000);
		//这里解释下:
		//将TIM1_CCR1的值增加2000，使得下一个TIM事件也需要2000个脉冲，
		//另一种方式是清零脉冲计数器
		//TIM_SetCounter(TIM2,0x0000);
	}
}
