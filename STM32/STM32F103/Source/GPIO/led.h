#ifndef __LED_H
#define	__LED_H

#include "version.h"

/* the macro definition to trigger the led on or off
 * 1 - off
 - 0 - on
 */
#define ON  1
#define OFF 0
#define Flip 0 //翻转
#define  LED_RCC_PORT (RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE|RCC_APB2Periph_AFIO)
// 社会安防
#define  LED2_USE					 1
#define  LED2_PORT     GPIOD
#define  LED2_PIN      GPIO_Pin_1

//
#define  LED1_USE					 0
#define  LED1_PORT     GPIOD  
#define  LED1_PIN      GPIO_Pin_0  
//#define  LED2_USE					 1
//#define  LED2_PORT     GPIOD
//#define  LED2_PIN      GPIO_Pin_6
#define  LED3_USE					 1
#define  LED3_PORT     GPIOC
#define  LED3_PIN      GPIO_Pin_11
#define  LED4_USE					 0
#define  LED4_PORT     GPIOD
#define  LED4_PIN      GPIO_Pin_3  
#define  LED5_USE					 0
#define  LED5_PORT     GPIOD  
#define  LED5_PIN      GPIO_Pin_15  
#define  LED6_USE					 0
#define  LED6_PORT     GPIOB  			//C8
#define  LED6_PIN      GPIO_Pin_8  
#define  LED7_USE					 0
#define  LED7_PORT     GPIOA				
#define  LED7_PIN      GPIO_Pin_4  
#define  LED8_USE					 0
#define  LED8_PORT     GPIOC				//VE
#define  LED8_PIN      GPIO_Pin_6  				
					
					
#define    LED1(x)   x ? GPIO_SetBits(LED1_PORT,LED1_PIN):  GPIO_ResetBits(LED1_PORT,LED1_PIN)
#define    LED2(x)   x ? GPIO_SetBits(LED2_PORT,LED2_PIN):  GPIO_ResetBits(LED2_PORT,LED2_PIN)
#define    LED3(x)   x ? GPIO_SetBits(LED3_PORT,LED3_PIN):  GPIO_ResetBits(LED2_PORT,LED3_PIN)
#define    LED4(x)   x ? GPIO_SetBits(LED4_PORT,LED4_PIN):  GPIO_ResetBits(LED2_PORT,LED4_PIN)
#define    LED5(x)   x ? GPIO_SetBits(LED5_PORT,LED5_PIN):  GPIO_ResetBits(LED5_PORT,LED5_PIN)
#define    LED6(x)   x ? GPIO_SetBits(LED6_PORT,LED6_PIN):  GPIO_ResetBits(LED6_PORT,LED6_PIN)
#define    LED7(x)   x ? GPIO_SetBits(LED7_PORT,LED7_PIN):  GPIO_ResetBits(LED7_PORT,LED7_PIN)
#define    LED8(x)   x ? GPIO_SetBits(LED8_PORT,LED8_PIN):  GPIO_ResetBits(LED8_PORT,LED8_PIN)

void LED_GPIO_Config(void);
					
#endif /* __LED_H */
