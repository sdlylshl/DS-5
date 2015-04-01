#include "stm32f10x_lib.h"




void RCC_Configuration(void);
void GPIO_Configuration(void);
void AFIO_Configuration(void);
void NVIC_Configuration(void);
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
   //GPIO_WriteBit(GPIOC,GPIO_Pin_6,Bit_SET);
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
   RCC_PCLK1Config(RCC_HCLK_Div2);	//9M
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
  #ifdef  VECT_TAB_RAM  
  /* Set the Vector Table base location at 0x20000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
 #else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
 #endif
 
}

void AFIO_Configuration(void)
{
  GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE); //TIM3 完全重映射
}

void Timer_Configuration(void)
{
 TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
 TIM_OCInitTypeDef   	   TIM_OCInitStructure;

 TIM_DeInit(TIM3);

 TIM_TimeBaseStructure.TIM_Period=0XFFFF;		 //ARR的值
 TIM_TimeBaseStructure.TIM_Prescaler=0;
 TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; //采样分频
 TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
 TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
 TIM_PrescalerConfig(TIM3,0x8C9F,TIM_PSCReloadMode_Immediate);//时钟分频系数36000，所以定时器时钟为2K
 TIM_ARRPreloadConfig(TIM3, ENABLE);//使能ARR预装载缓冲器

 TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_Active;
 TIM_OCInitStructure.TIM_Channel=TIM_Channel_1;
 TIM_OCInitStructure.TIM_Pulse=1000;
 TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
 TIM_OCInit(TIM3, &TIM_OCInitStructure);
 TIM_ForcedOC1Config(TIM3, TIM_ForcedAction_Active);




 TIM_Cmd(TIM3, ENABLE);	//开启时钟

}

void Delay(vu32 nCount)
{
  for(; nCount != 0; nCount--);
}








