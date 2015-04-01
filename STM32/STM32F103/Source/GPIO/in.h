#ifndef __IN_H
#define	__IN_H

#include "version.h"

/* the macro definition to trigger the led on or off
 * 1 - off
 - 0 - on
 */
#define ON  1
#define OFF 0
#define Flip 0
#define  SENSOR_RCC_PORT (RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE|RCC_APB2Periph_AFIO)

#define  SENSOR1_USE					 1
#define  SENSOR1_PORT     GPIOD  
#define  SENSOR1_PIN      GPIO_Pin_4  
#define  SENSOR1_MODE     GPIO_Mode_IPD  

#define  SENSOR2_USE					 1
#define  SENSOR2_PORT     GPIOD  
#define  SENSOR2_PIN      GPIO_Pin_0 
#define  SENSOR2_MODE     GPIO_Mode_IPD  

#define  SENSOR3_USE					 1
#define  SENSOR3_PORT     GPIOC
#define  SENSOR3_PIN      GPIO_Pin_10  
#define  SENSOR3_MODE     GPIO_Mode_IPD  

#define  SENSOR4_USE					 1
#define  SENSOR4_PORT     GPIOD
#define  SENSOR4_PIN      GPIO_Pin_3  
#define  SENSOR4_MODE     GPIO_Mode_IPD  

#define  SENSOR5_USE					 1
#define  SENSOR5_PORT     GPIOD  
#define  SENSOR5_PIN      GPIO_Pin_5   
#define  SENSOR5_MODE     GPIO_Mode_IPD  

#define  SENSOR6_USE					 1
#define  SENSOR6_PORT     GPIOC  			
#define  SENSOR6_PIN      GPIO_Pin_8  
#define  SENSOR6_MODE     GPIO_Mode_IPD  

#define  SENSOR7_USE					 1
#define  SENSOR7_PORT     GPIOC				
#define  SENSOR7_PIN      GPIO_Pin_2 
#define  SENSOR7_MODE     GPIO_Mode_IPD  

#define  SENSOR8_USE					 1
#define  SENSOR8_PORT     GPIOE				
#define  SENSOR8_PIN      GPIO_Pin_9  
#define  SENSOR8_MODE     GPIO_Mode_IPD  

#define  SENSOR9_USE					 1
#define  SENSOR9_PORT     GPIOE  
#define  SENSOR9_PIN      GPIO_Pin_13  
#define  SENSOR9_MODE     GPIO_Mode_IPD  

#define  SENSOR10_USE					 1
#define  SENSOR10_PORT     GPIOE  
#define  SENSOR10_PIN      GPIO_Pin_11
#define  SENSOR10_MODE     GPIO_Mode_IPD  

#define  SENSOR11_USE					 1
#define  SENSOR11_PORT     GPIOE
#define  SENSOR11_PIN      GPIO_Pin_10  
#define  SENSOR11_MODE     GPIO_Mode_IPD  

#define  SENSOR12_USE					 1
#define  SENSOR12_PORT     GPIOE
#define  SENSOR12_PIN      GPIO_Pin_12  
#define  SENSOR12_MODE     GPIO_Mode_IPD  

#define  SENSOR13_USE					 1
#define  SENSOR13_PORT     GPIOE  
#define  SENSOR13_PIN      GPIO_Pin_1   
#define  SENSOR13_MODE     GPIO_Mode_IPD  

#define  SENSOR14_USE					 1
#define  SENSOR14_PORT     GPIOE  			
#define  SENSOR14_PIN      GPIO_Pin_3  
#define  SENSOR14_MODE     GPIO_Mode_IPD  

#define  SENSOR15_USE					 1
#define  SENSOR15_PORT     GPIOC				
#define  SENSOR15_PIN      GPIO_Pin_14 
#define  SENSOR15_MODE     GPIO_Mode_IPD  

#define  SENSOR16_USE					 1
#define  SENSOR16_PORT     GPIOD				
#define  SENSOR16_PIN      GPIO_Pin_14  
#define  SENSOR16_MODE     GPIO_Mode_IPD  

#define  SENSOR17_USE					 1
#define  SENSOR17_PORT     GPIOC  
#define  SENSOR17_PIN      GPIO_Pin_4  
#define  SENSOR17_MODE     GPIO_Mode_IPD  

#define  SENSOR18_USE					 1
#define  SENSOR18_PORT     GPIOD  
#define  SENSOR18_PIN      GPIO_Pin_9
#define  SENSOR18_MODE     GPIO_Mode_IPD  

#define  SENSOR19_USE					 1
#define  SENSOR19_PORT     GPIOE
#define  SENSOR19_PIN      GPIO_Pin_5  
#define  SENSOR19_MODE     GPIO_Mode_IPD  

#define  SENSOR20_USE					 1
#define  SENSOR20_PORT     GPIOE
#define  SENSOR20_PIN      GPIO_Pin_6  
#define  SENSOR20_MODE     GPIO_Mode_IPD  

#define  SENSOR21_USE					 1
#define  SENSOR21_PORT     GPIOC  
#define  SENSOR21_PIN      GPIO_Pin_1  
#define  SENSOR21_MODE     GPIO_Mode_IPD  

#define  SENSOR22_USE					 1
#define  SENSOR22_PORT     GPIOD  			
#define  SENSOR22_PIN      GPIO_Pin_10  
#define  SENSOR22_MODE     GPIO_Mode_IPD  

#define  SENSOR23_USE					 1
#define  SENSOR23_PORT     GPIOC				
#define  SENSOR23_PIN      GPIO_Pin_7 
#define  SENSOR23_MODE     GPIO_Mode_IPD  

#define  SENSOR24_USE					 1
#define  SENSOR24_PORT     GPIOD			
#define  SENSOR24_PIN      GPIO_Pin_15  
#define  SENSOR24_MODE     GPIO_Mode_IPD  

#define    SENSOR1(x)   x ? GPIO_SetBits(SENSOR1_PORT,SENSOR1_PIN):  GPIO_ResetBits(SENSOR1_PORT,SENSOR1_PIN)
#define    SENSOR2(x)   x ? GPIO_SetBits(SENSOR2_PORT,SENSOR2_PIN):  GPIO_ResetBits(SENSOR2_PORT,SENSOR2_PIN)
#define    SENSOR3(x)   x ? GPIO_SetBits(SENSOR3_PORT,SENSOR3_PIN):  GPIO_ResetBits(SENSOR3_PORT,SENSOR3_PIN)
#define    SENSOR4(x)   x ? GPIO_SetBits(SENSOR4_PORT,SENSOR4_PIN):  GPIO_ResetBits(SENSOR4_PORT,SENSOR4_PIN)
#define    SENSOR5(x)   x ? GPIO_SetBits(SENSOR5_PORT,SENSOR5_PIN):  GPIO_ResetBits(SENSOR5_PORT,SENSOR5_PIN)
#define    SENSOR6(x)   x ? GPIO_SetBits(SENSOR6_PORT,SENSOR6_PIN):  GPIO_ResetBits(SENSOR6_PORT,SENSOR6_PIN)
#define    SENSOR7(x)   x ? GPIO_SetBits(SENSOR7_PORT,SENSOR7_PIN):  GPIO_ResetBits(SENSOR7_PORT,SENSOR7_PIN)
#define    SENSOR8(x)   x ? GPIO_SetBits(SENSOR8_PORT,SENSOR8_PIN):  GPIO_ResetBits(SENSOR8_PORT,SENSOR8_PIN)
#define    SENSOR9(x)   x ? GPIO_SetBits(SENSOR9_PORT,SENSOR9_PIN):  GPIO_ResetBits(SENSOR9_PORT,SENSOR9_PIN)
#define    SENSOR10(x)   x ? GPIO_SetBits(SENSOR10_PORT,SENSOR10_PIN):  GPIO_ResetBits(SENSOR10_PORT,SENSOR10_PIN)
#define    SENSOR11(x)   x ? GPIO_SetBits(SENSOR11_PORT,SENSOR11_PIN):  GPIO_ResetBits(SENSOR11_PORT,SENSOR11_PIN)
#define    SENSOR12(x)   x ? GPIO_SetBits(SENSOR12_PORT,SENSOR12_PIN):  GPIO_ResetBits(SENSOR12_PORT,SENSOR12_PIN)
#define    SENSOR13(x)   x ? GPIO_SetBits(SENSOR13_PORT,SENSOR13_PIN):  GPIO_ResetBits(SENSOR13_PORT,SENSOR13_PIN)
#define    SENSOR14(x)   x ? GPIO_SetBits(SENSOR14_PORT,SENSOR14_PIN):  GPIO_ResetBits(SENSOR14_PORT,SENSOR14_PIN)
#define    SENSOR15(x)   x ? GPIO_SetBits(SENSOR15_PORT,SENSOR15_PIN):  GPIO_ResetBits(SENSOR15_PORT,SENSOR15_PIN)
#define    SENSOR16(x)   x ? GPIO_SetBits(SENSOR16_PORT,SENSOR16_PIN):  GPIO_ResetBits(SENSOR16_PORT,SENSOR16_PIN)
#define    SENSOR17(x)   x ? GPIO_SetBits(SENSOR17_PORT,SENSOR17_PIN):  GPIO_ResetBits(SENSOR17_PORT,SENSOR17_PIN)
#define    SENSOR18(x)   x ? GPIO_SetBits(SENSOR18_PORT,SENSOR18_PIN):  GPIO_ResetBits(SENSOR18_PORT,SENSOR18_PIN)
#define    SENSOR19(x)   x ? GPIO_SetBits(SENSOR19_PORT,SENSOR19_PIN):  GPIO_ResetBits(SENSOR19_PORT,SENSOR19_PIN)
#define    SENSOR20(x)   x ? GPIO_SetBits(SENSOR20_PORT,SENSOR20_PIN):  GPIO_ResetBits(SENSOR20_PORT,SENSOR20_PIN)
#define    SENSOR21(x)   x ? GPIO_SetBits(SENSOR21_PORT,SENSOR21_PIN):  GPIO_ResetBits(SENSOR21_PORT,SENSOR21_PIN)
#define    SENSOR22(x)   x ? GPIO_SetBits(SENSOR22_PORT,SENSOR22_PIN):  GPIO_ResetBits(SENSOR22_PORT,SENSOR22_PIN)
#define    SENSOR23(x)   x ? GPIO_SetBits(SENSOR23_PORT,SENSOR23_PIN):  GPIO_ResetBits(SENSOR23_PORT,SENSOR23_PIN)
#define    SENSOR24(x)   x ? GPIO_SetBits(SENSOR24_PORT,SENSOR24_PIN):  GPIO_ResetBits(SENSOR24_PORT,SENSOR24_PIN)
void SENSOR_GPIO_Config(void);
void SENSORn_ONOFF(uint8_t n, uint8_t onoff);

uint8_t GetSensorStatus(uint8_t ch);
#endif /* __SENSOR_H */
