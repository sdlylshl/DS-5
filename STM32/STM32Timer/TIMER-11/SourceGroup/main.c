#include "stm32f10x_lib.h"




void RCC_Configuration(void);
void GPIO_Configuration(void);
void AFIO_Configuration(void);
void NVIC_Configuration(void);
void SysTick_Configuration(void);
void Timer_Configuration(void);


void Delay(vu32 nCount);

u8 i=0;


int main(void)
{
  
  RCC_Configuration();  
  GPIO_Configuration(); 
  AFIO_Configuration();
  
  NVIC_Configuration();
  Timer_Configuration();

   
   
  while(1)
  {
   
  }

    
  
 
}

void RCC_Configuration(void)
{
  ErrorStatus HSEStartUpStatus;
  RCC_DeInit();
  RCC_HSEConfig(RCC_HSE_ON);
  HSEStartUpStatus = RCC_WaitForHSEStartUp();
  if(HSEStartUpStatus == SUCCESS)
  {
   RCC_HCLKConfig(RCC_SYSCLK_Div1); 
   RCC_PCLK2Config(RCC_HCLK_Div1); 
   RCC_PCLK1Config(RCC_HCLK_Div2);	
   RCC_ADCCLKConfig(RCC_PCLK2_Div6);
   FLASH_SetLatency(FLASH_Latency_2);
   FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
   RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);	 //PLL 72M
   RCC_PLLCmd(ENABLE);
   while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }
   RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    while(RCC_GetSYSCLKSource() != 0x08)
    {
    } 
  }

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ALL,ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_ALL, ENABLE);	 
}

void GPIO_Configuration(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  GPIO_DeInit(GPIOC);
  GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
  GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
  GPIO_Init(GPIOC,&GPIO_InitStruct);
}

void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  #ifdef  VECT_TAB_RAM  
  /* Set the Vector Table base location at 0x20000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
 #else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
 #endif
 
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

 
}

void AFIO_Configuration(void)
{
  GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE); //TIM3 完全重映射
  GPIO_PinRemapConfig(GPIO_Remap_TIM4,ENABLE); //TIM4 完全重映射
}



void Timer_Configuration(void)
{
 
 TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
 TIM_OCInitTypeDef   	   TIM_OCInitStructure;
 TIM_ICInitTypeDef		   TIM_ICInitStructure;
 
 //配置TIMER4为主模式，OC1作为触发输出
 TIM_DeInit(TIM4);
 
 TIM_TimeBaseStructure.TIM_Period=2000;		 
 TIM_TimeBaseStructure.TIM_Prescaler=0;
 TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV2; //采样分频
 TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
 TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
 TIM_PrescalerConfig(TIM4,0x8C9F,TIM_PSCReloadMode_Immediate);//时钟分频系数36000，所以定时器时钟为2K
 TIM_ARRPreloadConfig(TIM4, DISABLE);//禁止ARR预装载缓冲器
 //配置TIMER4的输入通道1的参数
 TIM_ICInitStructure.TIM_ICMode=TIM_ICMode_ICAP;
 TIM_ICInitStructure.TIM_Channel=TIM_Channel_1;
 TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;
 TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;
 TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;
 TIM_ICInitStructure.TIM_ICFilter=7; //Fsampling=Fdts/4 N=8  滤除32ms的抖动
 TIM_ICInit(TIM4, &TIM_ICInitStructure);

 
 TIM_TIxExternalClockConfig(TIM4,TIM_TS_TI1FP1,TIM_ICPolarity_Rising,7);//配置外部触发信号为TI1FP1
 TIM_SelectSlaveMode(TIM4,TIM_SlaveMode_Trigger);//配置TIMER4从模式为触发模式
 TIM_SelectOutputTrigger(TIM4,TIM_TRGOSource_Enable); //选择触发输出为使能
 TIM_SelectMasterSlaveMode(TIM4,TIM_MasterSlaveMode_Enable);//选择主/从模式(既是主模式也是从模式，触发信号延迟一段时间等待从定时器以达到同步的目的)



 
 
  
 
 //配置TIMER3为PWM1输出模式，输出占空比50%，周期为1S的PWM波，配置TIMER3从模式为触发模式，选择TIMER4为触发源
 TIM_DeInit(TIM3); 

 TIM_TimeBaseStructure.TIM_Period=2000;		 //ARR的值
 TIM_TimeBaseStructure.TIM_Prescaler=0;
 TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; //采样分频
 TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
 TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
 TIM_PrescalerConfig(TIM3,0x8C9F,TIM_PSCReloadMode_Immediate);//时钟分频系数36000，所以定时器时钟为2K
 TIM_ARRPreloadConfig(TIM3, DISABLE);//禁止ARR预装载缓冲器


 TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM2;
 TIM_OCInitStructure.TIM_Channel=TIM_Channel_1;
 TIM_OCInitStructure.TIM_Pulse=1000;				 //50%占空比
 TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
 TIM_OCInit(TIM3, &TIM_OCInitStructure);
 TIM_ITRxExternalClockConfig(TIM3, TIM_TS_ITR3);//外部时钟模式1，触发源为TIMER4
 TIM_SelectSlaveMode(TIM3,TIM_SlaveMode_Trigger);//配置为触发模式，时钟自动切换为内部时钟

 


 

 

// TIM_Cmd(TIM3, ENABLE);	//开启时钟
// TIM_Cmd(TIM4, ENABLE);	//开启时钟

}

void Delay(vu32 nCount)
{
  for(; nCount != 0; nCount--);
}








