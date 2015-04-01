#include "stm32f10x_lib.h"




void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void Timer_Configuration(void);

void Delay(vu32 nCount);

u8 i=0;


int main(void)
{
  
  RCC_Configuration();  
  GPIO_Configuration();  
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
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC| RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	 
}

void GPIO_Configuration(void)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  GPIO_DeInit(GPIOC);
  GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
  GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC,&GPIO_InitStruct);

  GPIO_DeInit(GPIOA);
  GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0;
  GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA,&GPIO_InitStruct);
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

void Timer_Configuration(void)
{
 TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;

 TIM_DeInit(TIM2);

 TIM_TimeBaseStructure.TIM_Period=2000;		 //ARR的值
 TIM_TimeBaseStructure.TIM_Prescaler=0;
 TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; //采样分频
 TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
 TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
 TIM_PrescalerConfig(TIM2,0x8C9F,TIM_PSCReloadMode_Immediate);//时钟分频系数36000，所以定时器时钟为2K
 TIM_ARRPreloadConfig(TIM2, DISABLE);//禁止ARR预装载缓冲器
 TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);

 TIM_Cmd(TIM2, ENABLE);	//开启时钟

}

void Delay(vu32 nCount)
{
  for(; nCount != 0; nCount--);
}








