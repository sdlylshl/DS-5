#ifndef __LED_H
#define __LED_H	 
#include "Types.h"
void LED_init(void);
	 			
#define LED1_OFF   GPIO_SetBits(LED1_GPIO,LED1_Pin);  
#define LED1_ON  GPIO_ResetBits(LED1_GPIO,LED1_Pin); 

#define LED2_OFF   GPIO_SetBits(LED2_GPIO,LED2_Pin);  
#define LED2_ON  GPIO_ResetBits(LED2_GPIO,LED2_Pin); 

#define LED3_OFF   GPIO_SetBits(LED3_GPIO,LED3_Pin);  
#define LED3_ON  GPIO_ResetBits(LED3_GPIO,LED3_Pin); 

#define LED4_OFF   GPIO_SetBits(LED4_GPIO,LED4_Pin);  
#define LED4_ON  GPIO_ResetBits(LED4_GPIO,LED4_Pin); 

#define LED1 PDout(13)
#define LED2 PDout(14)
#define LED3 PDout(15)
#define LED4 PCout(6)

#endif
