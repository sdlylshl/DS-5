/******************** (C) COPYRIGHT 2012 WildFire Team ***************************
 * 文件名  ：Time_test.c
 * 描述    ：TIM2 1ms 定时应用函数库
 *
 * 实验平台：野火STM32开发板
 * 硬件连接：----------------------
 *          |                     |
 *          |           无           |
 *          |                     |
 *           ----------------------
 * 库版本  ：ST3.5.0
 * 作者    ：wildfire team
 * 论坛    ：http://www.amobbs.com/forum-1008-1.html
 * 淘宝    ：http://firestm32.taobao.com
**********************************************************************************/
#include "timer.h"
#include "Types.h"
#include "wigend.h"
#include "hardware.h"
#include "bar.h"

__IO uint32_t time; // ms 计时变量
__IO uint32_t Zigbee_time; // ms 计时变量
__IO uint32_t Net_time; // ms 计时变量
__IO uint32_t time_out;
__IO uint32_t timer_Device_update;
__IO uint32_t timer_Zigbee_getStatus;
__IO uint32_t timer_Zigbee_getBattery;
__IO uint32_t timer_Zigbee_sendBuff;
__IO uint32_t timer_Zigbee_ClearRequest;
__IO uint32_t timer_Zigbee_operate_default;
__IO uint32_t Timer3_Counter;
__IO uint32_t  ps2_time3;
uint32_t my_time3;
uint32_t wigend_time = 0;
const uint32_t MAX_UINT32 = (uint32_t)-1;
volatile uint32_t time4=0; // ms 计时变量
uint32_t key_time = 0;

void TIM2_test()
{

}


void TIM1_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/*
 * 函数名：TIM2_NVIC_Configuration
 * 描述  ：TIM2中断优先级配置
 * 输入  ：无
 * 输出  ：无
 */
void TIM2_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
void TIM3_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void TIM4_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*TIM_Period--1000   TIM_Prescaler--71 -->中断周期为1ms*/
void TIM4_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE);
    TIM_DeInit(TIM4);
    TIM4_NVIC_Configuration();
    TIM_TimeBaseStructure.TIM_Period = 1000;
    /* 累计 TIM_Period个频率后产生一个更新或者中断 */
    TIM_TimeBaseStructure.TIM_Prescaler = (SystemCoreClock/1000000-1); //(72 - 1);
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM4, TIM_FLAG_Update);
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM4, ENABLE);

//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , DISABLE);      /*先关闭等待使用*/
}

void TIM4_Start(void){
//		time4=0;
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE);
//		TIM_Cmd(TIM4, ENABLE);
}
	

void TIM4_Stop(void){
	TIM_Cmd(TIM4, DISABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , DISABLE); 
}
//获取当前参数--->当前时间 差值

uint32_t TIM4_GetDistanceTime(uint32_t pretime)
{			
			return (MAX_UINT32-pretime+time4+1);
}

uint32_t TIM4_GetCurrentTime(void){
			return (time4);
}
void TIM4_IRQHandler(void)
{
    if ( TIM_GetITStatus(TIM4 , TIM_IT_Update) != RESET )
    {
        TIM_ClearITPendingBit(TIM4 , TIM_FLAG_Update);
        time4++;
    }
}

void TIM1_Configuration(void)
{
    TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;
    //Step1.时钟设置：启动TIM1
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    //TIM1 使用内部时钟
    //TIM_InternalClockConfig(TIM1);
    TIM_DeInit(TIM1);
    //Step2.中断NVIC设置：允许中断，设置优先级
    TIM1_NVIC_Configuration();
    //Step3.TIM模块设置
    //设置预分频器分频系数71，即APB2=72M, TIM1_CLK=72/72=1MHz
    //TIM_Period（TIM1_ARR）=1000，计数器向上计数到1000后产生更新事件，计数值归零
    //TIM_RepetitionCounter(TIM1_RCR)=0，每次向上溢出都产生更新事件
    TIM_BaseInitStructure.TIM_Period = 1000;
    TIM_BaseInitStructure.TIM_Prescaler = 71;
    TIM_BaseInitStructure.TIM_ClockDivision = 0;
    TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_BaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_BaseInitStructure);
    //清中断，以免一启用中断后立即产生中断
    TIM_ClearFlag(TIM1, TIM_FLAG_Update);
    //使能TIM1中断源
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
    //TIM1总开关：开启
    TIM_Cmd(TIM1, ENABLE);
    //Step4.注册中断服务子程序
    //将中断服务函数添加到stm32f10x_it.c
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, DISABLE);   /*先关闭等待使用*/
}
/*TIM_Period--1000   TIM_Prescaler--71 -->中断周期为1ms*/
void TIM2_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
    TIM_DeInit(TIM2);
    TIM2_NVIC_Configuration();
    TIM_TimeBaseStructure.TIM_Period = 1000;
    /* 累计 TIM_Period个频率后产生一个更新或者中断 */
    TIM_TimeBaseStructure.TIM_Prescaler = (72 - 1);
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM2, ENABLE);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE);      /*先关闭等待使用*/
}
/*TIM_Period--1000   TIM_Prescaler--71 -->中断周期为1ms*/
void TIM3_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
    TIM_DeInit(TIM3);
    TIM3_NVIC_Configuration();
  /* Time base configuration */	
  TIM_TimeBaseStructure.TIM_Period = 1000;          
  TIM_TimeBaseStructure.TIM_Prescaler = 0;      
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;    
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
  
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  
  /* Prescaler configuration */
  TIM_PrescalerConfig(TIM3, 71, TIM_PSCReloadMode_Immediate);
  
  /* TIM enable counter */
  TIM_Cmd(TIM3, ENABLE);
  
  /* TIM IT enable */
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);  
}
/*TIM_Period--1000   TIM_Prescaler--71 -->中断周期为1ms*/
void TIM4_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE);
    TIM_DeInit(TIM4);
    TIM4_NVIC_Configuration();
    TIM_TimeBaseStructure.TIM_Period = 1000;
    /* 累计 TIM_Period个频率后产生一个更新或者中断 */
    TIM_TimeBaseStructure.TIM_Prescaler = (72 - 1);
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM4, TIM_FLAG_Update);
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM4, ENABLE);

//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , DISABLE);      /*先关闭等待使用*/
}





// 中断服务子程序
extern uint8_t BIT_Count;
void TIM1_UP_IRQHandle(void)
{
    GPIOC->ODR ^= (1 << 4);                        //闪灯
    TIM_ClearITPendingBit(TIM1, TIM_FLAG_Update); //清中断
}
void TIM1_CC_IRQHandle(void)
{
    u16 capture;
    if (TIM_GetITStatus(TIM1, TIM_IT_CC1) == SET)
    {
        TIM_ClearITPendingBit(TIM1, TIM_IT_CC1 );
        capture = TIM_GetCapture1(TIM1);
        TIM_SetCompare1(TIM1, capture + 2000);
        //这里解释下:
        //将TIM1_CCR1的值增加2000，使得下一个TIM事件也需要2000个脉冲，
        //另一种方式是清零脉冲计数器
        //TIM_SetCounter(TIM2,0x0000);
    }
}
void TIM2_IRQHandle(void)
{
    if ( TIM_GetITStatus(TIM2 , TIM_IT_Update) != RESET )
    {
        TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);
        time++;
        Zigbee_time++;
        Net_time++;
        time_out++;
        timer_Device_update++;
        timer_Zigbee_getStatus++;
        timer_Zigbee_getBattery++;
        timer_Zigbee_sendBuff++;
        timer_Zigbee_ClearRequest++;
        timer_Zigbee_operate_default++;

        if (time > 500)
        {
           // LED8( ON );
        }
        if (time > 1000)
        {

           // LED8( OFF );
            time = 0;

        }

    }
}



void Timer3_ISR(void)
{
  if (Timer3_Counter++ > 100) { // 1m x 1000 = 1sec
    Timer3_Counter = 0;
    my_time3++;
	check_wigend_input_flag();
		
	if(flag_get_key_Card )
	{
		key_time++;
		if(key_time >= 5)
		{
			flag_get_key_Card = 0;
			key_time = 0;
		}
		
	}
	else
	{
		if (flag_null_key_Card < 2)
			flag_null_key_Card++;
	}

	if (flag_test_get_key_Card)
	{
		key_time++;
		if (key_time >= 5)
		{
			flag_test_get_key_Card = 0;
			key_time = 0;
		}

	}
	else
	{
		if (flag_test_null_key_Card < 2)
			flag_test_null_key_Card++;
	}

	if (flag_Line_Relay_ON[0] > 0)
		flag_Line_Relay_ON[0]--;
	if (flag_Line_Relay_ON[1] > 0)
		flag_Line_Relay_ON[1]--;
	if (flag_Line_Relay_ON[2] > 0)
		flag_Line_Relay_ON[2]--;
	if (flag_Line_Relay_ON[3] > 0)
		flag_Line_Relay_ON[3]--;
  }

	
}

void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

		Timer3_ISR();
		
	}
}

uint32_t get_curTime()
{
	return my_time3;
}

uint32_t get_count()
{
	return Timer3_Counter;
}

uint32_t time_return(void)
{
	return my_time3;
}




