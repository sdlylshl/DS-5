#include "led.h"
	    
//LED IO≥ı ºªØ
void LED_init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;

 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD  | RCC_APB2Periph_GPIOC , ENABLE);

 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

 GPIO_InitStructure.GPIO_Pin = LED1_Pin;
 GPIO_InitStructure.GPIO_Mode = LED1_GPIO_Mode;	       
 GPIO_Init(LED1_GPIO, &GPIO_InitStructure);

 GPIO_InitStructure.GPIO_Pin = LED2_Pin;
 GPIO_InitStructure.GPIO_Mode = LED2_GPIO_Mode;	      
 GPIO_Init(LED2_GPIO, &GPIO_InitStructure);

 GPIO_InitStructure.GPIO_Pin = LED3_Pin;
 GPIO_InitStructure.GPIO_Mode = LED3_GPIO_Mode;	      
 GPIO_Init(LED3_GPIO, &GPIO_InitStructure);

 GPIO_InitStructure.GPIO_Pin = LED4_Pin;
 GPIO_InitStructure.GPIO_Mode = LED4_GPIO_Mode;	      
 GPIO_Init(LED4_GPIO, &GPIO_InitStructure);

}
 
