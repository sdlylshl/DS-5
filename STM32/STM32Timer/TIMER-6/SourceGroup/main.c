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
  GPIO_DeInit(GPIOD);
  GPIO_InitStruct.GPIO_Pin=GPIO_Pin_12;
  GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOD,&GPIO_InitStruct);

  GPIO_DeInit(GPIOC);
  GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
  GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
  GPIO_Init(GPIOC,&GPIO_InitStruct);
}

void NVIC_Configuration(void)
{
  NVIC_InitTypeDef  NVIC_InitStructure;
  #ifdef  VECT_TAB_RAM  
  /* Set the Vector Table base location at 0x20000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
 #else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
 #endif

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

 
}

void AFIO_Configuration(void)
{
  GPIO_PinRemapConfig(GPIO_Remap_TIM4,ENABLE); //TIM4 ��ȫ��ӳ��
  GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE); //TIM3 ��ȫ��ӳ��
}



void Timer_Configuration(void)
{
 TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
 TIM_ICInitTypeDef  	   TIM_ICInitStructure;
 TIM_OCInitTypeDef   	   TIM_OCInitStructure;
 TIM_DeInit(TIM3);

 TIM_TimeBaseStructure.TIM_Period=60000;		 //ARR��ֵ
 TIM_TimeBaseStructure.TIM_Prescaler=0;
 TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; //������Ƶ,Ƶ��1K
 TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
 TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
 TIM_PrescalerConfig(TIM4,0x8C9F,TIM_PSCReloadMode_Immediate);//ʱ�ӷ�Ƶϵ��36000�����Զ�ʱ��ʱ��Ϊ2K
 TIM_ARRPreloadConfig(TIM4, ENABLE);//ʹ��ARRԤװ�ػ�����

 TIM_ICInitStructure.TIM_ICMode=TIM_ICMode_PWMI;
 TIM_ICInitStructure.TIM_Channel=TIM_Channel_1;
 TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Falling;
 TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;
 TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;
 TIM_ICInitStructure.TIM_ICFilter=2;
 TIM_ICInit(TIM4,&TIM_ICInitStructure);

 TIM_SelectInputTrigger(TIM4, TIM_TS_TI1FP1);
 TIM_SelectSlaveMode(TIM4, TIM_SlaveMode_Reset);
 //����TIMER3
 TIM_TimeBaseStructure.TIM_Period=2000;		 //ARR��ֵ
 TIM_TimeBaseStructure.TIM_Prescaler=0;
 TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; //������Ƶ
 TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
 TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
 TIM_PrescalerConfig(TIM3,0x8C9F,TIM_PSCReloadMode_Immediate);//ʱ�ӷ�Ƶϵ��36000�����Զ�ʱ��ʱ��Ϊ2K
 TIM_ARRPreloadConfig(TIM3, ENABLE);//ʹ��ARRԤװ�ػ�����

 TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
 TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
 TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
 TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

 TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
 TIM_OCInitStructure.TIM_Channel=TIM_Channel_1;
 TIM_OCInitStructure.TIM_Pulse=500;				 //50%ռ�ձ�,����2��
 TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
 TIM_OCInit(TIM3, &TIM_OCInitStructure);
 

 TIM_ITConfig(TIM4, TIM_IT_Trigger, ENABLE);//ʹ�ܴ����ж�
 TIM_ITConfig(TIM4, TIM_IT_CC2, ENABLE);//ʹ�ܲ����ж�
 
 TIM_GenerateEvent(TIM4, TIM_EventSource_Update);
 

 TIM_Cmd(TIM3, ENABLE);	//����ʱ��
 TIM_Cmd(TIM4, ENABLE);	//����ʱ��

}

void Delay(vu32 nCount)
{
  for(; nCount != 0; nCount--);
}








