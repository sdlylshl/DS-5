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
}



void Timer_Configuration(void)
{
 TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
 TIM_OCInitTypeDef   	   TIM_OCInitStructure;
 
 //配置TIMER2为主模式，OC1作为触发输出
 TIM_DeInit(TIM2);
 
 
 TIM_TimeBaseStructure.TIM_Period=60000;		 //PWM 周期30s
 TIM_TimeBaseStructure.TIM_Prescaler=0;
 TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; //采样分频
 TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
 TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
 TIM_PrescalerConfig(TIM2,0x8C9F,TIM_PSCReloadMode_Immediate);//时钟分频系数36000，所以定时器时钟为2K
 TIM_ARRPreloadConfig(TIM2, DISABLE);//禁止ARR预装载缓冲器

 TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
 TIM_OCInitStructure.TIM_Channel=TIM_Channel_1;
 TIM_OCInitStructure.TIM_Pulse=30000;				 //50%占空比
 TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
 TIM_OCInit(TIM2, &TIM_OCInitStructure);

 TIM_SelectOutputTrigger(TIM2,TIM_TRGOSource_OC1Ref); //选择触发输出为OC1


 
 
 
 
 
 
 
 //配置TIMER3为PWM1输出模式，输出占空比50%，周期为1S的PWM波，配置TIMER3从模式为门控模式，选择TIMER2为触发源
 TIM_DeInit(TIM3);
 
 

 TIM_TimeBaseStructure.TIM_Period=2000;		 //ARR的值
 TIM_TimeBaseStructure.TIM_Prescaler=0;
 TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; //采样分频
 TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
 TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
 TIM_PrescalerConfig(TIM3,0x8C9F,TIM_PSCReloadMode_Immediate);//时钟分频系数36000，所以定时器时钟为2K
 TIM_ARRPreloadConfig(TIM3, DISABLE);//禁止ARR预装载缓冲器


 TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
 TIM_OCInitStructure.TIM_Channel=TIM_Channel_1;
 TIM_OCInitStructure.TIM_Pulse=1000;				 //50%占空比
 TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
 TIM_OCInit(TIM3, &TIM_OCInitStructure);
 TIM_ITRxExternalClockConfig(TIM3, TIM_TS_ITR1);//外部时钟模式1，触发源为TIMER2
 TIM_SelectSlaveMode(TIM3,TIM_SlaveMode_Gated);//配置为门控模式，时钟自动切换为内部时钟

 


 

 

 TIM_Cmd(TIM3, ENABLE);	//开启时钟
 TIM_Cmd(TIM2, ENABLE);	//开启时钟

}

void Delay(vu32 nCount)
{
  for(; nCount != 0; nCount--);
}








