#ifndef __OUT_H
#define	__OUT_H

#include "stm32f10x.h"

/* the macro definition to trigger the led on or off
 * 1 - off
 - 0 - on
 */

//		boxMgr[0].subId = 14; 0x0E
//		boxMgr[1].subId = 13; 0x0D
//		boxMgr[2].subId = 17;
//		boxMgr[3].subId = 18;
//		boxMgr[4].subId = 15;
//		boxMgr[5].subId = 16;
//		boxMgr[6].subId = 7;
//		boxMgr[7].subId = 5;
//		boxMgr[8].subId = 24;
//		boxMgr[9].subId = 22;
//		boxMgr[10].subId = 21;
//		boxMgr[11].subId = 23;
#define ON  1
#define OFF 0
#define Flip 0 //
#define  LOCK_RCC_PORT (RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE|RCC_APB2Periph_AFIO)

#define  LOCK1_USE			1
#define  LOCK1_PORT     GPIOD  
#define  LOCK1_PIN      GPIO_Pin_2  
#define  LOCK1_MODE     GPIO_Mode_Out_PP  

#define  LOCK2_USE			1
#define  LOCK2_PORT     GPIOD  
#define  LOCK2_PIN      GPIO_Pin_6 
#define  LOCK2_MODE     GPIO_Mode_Out_PP  
  
#define  LOCK3_USE					 1
#define  LOCK3_PORT     GPIOC
#define  LOCK3_PIN      GPIO_Pin_11  
#define  LOCK3_MODE     GPIO_Mode_Out_PP  
 
#define  LOCK4_USE					 1
#define  LOCK4_PORT     GPIOC
#define  LOCK4_PIN      GPIO_Pin_12  
#define  LOCK4_MODE     GPIO_Mode_Out_PP  
 
#define  LOCK5_USE					 1
#define  LOCK5_PORT     GPIOD  
#define  LOCK5_PIN      GPIO_Pin_7   
#define  LOCK5_MODE     GPIO_Mode_Out_PP  

#define  LOCK6_USE					 1
#define  LOCK6_PORT     GPIOC  			
#define  LOCK6_PIN      GPIO_Pin_15  
#define  LOCK6_MODE     GPIO_Mode_Out_PP  
 
#define  LOCK7_USE					 1
#define  LOCK7_PORT     GPIOC				
#define  LOCK7_PIN      GPIO_Pin_0 
#define  LOCK7_MODE     GPIO_Mode_Out_PP  
  
#define  LOCK8_USE					 1
#define  LOCK8_PORT     GPIOE				
#define  LOCK8_PIN      GPIO_Pin_7  
#define  LOCK8_MODE     GPIO_Mode_Out_PP  

#define  LOCK9_USE					 1
#define  LOCK9_PORT     GPIOC  
#define  LOCK9_PIN      GPIO_Pin_5  
#define  LOCK9_MODE     GPIO_Mode_Out_PP  

#define  LOCK10_USE					 1
#define  LOCK10_PORT     GPIOE  
#define  LOCK10_PIN      GPIO_Pin_15 
#define  LOCK10_MODE     GPIO_Mode_Out_PP  
  
#define  LOCK11_USE					 1
#define  LOCK11_PORT     GPIOE
#define  LOCK11_PIN      GPIO_Pin_8  
#define  LOCK11_MODE     GPIO_Mode_Out_PP  
 
#define  LOCK12_USE					 1
#define  LOCK12_PORT     GPIOE
#define  LOCK12_PIN      GPIO_Pin_14  
#define  LOCK12_MODE     GPIO_Mode_Out_PP  
 
#define  LOCK13_USE					 1
#define  LOCK13_PORT     GPIOE  
#define  LOCK13_PIN      GPIO_Pin_0   
#define  LOCK13_MODE     GPIO_Mode_Out_PP  

#define  LOCK14_USE					 1
#define  LOCK14_PORT     GPIOE  			
#define  LOCK14_PIN      GPIO_Pin_2  
#define  LOCK14_MODE     GPIO_Mode_Out_PP  
 
#define  LOCK15_USE					 1
#define  LOCK15_PORT     GPIOC				
#define  LOCK15_PIN      GPIO_Pin_13 
#define  LOCK15_MODE     GPIO_Mode_Out_PP  
  
#define  LOCK16_USE					 1
#define  LOCK16_PORT     GPIOC				
#define  LOCK16_PIN      GPIO_Pin_3  
#define  LOCK16_MODE     GPIO_Mode_Out_PP  
 				 				
#define  LOCK17_USE					 1
#define  LOCK17_PORT     GPIOD  
#define  LOCK17_PIN      GPIO_Pin_8  
#define  LOCK17_MODE     GPIO_Mode_Out_PP  

#define  LOCK18_USE					 1
#define  LOCK18_PORT     GPIOD  
#define  LOCK18_PIN      GPIO_Pin_13
#define  LOCK18_MODE     GPIO_Mode_Out_PP  
  
#define  LOCK19_USE					 1
#define  LOCK19_PORT     GPIOD
#define  LOCK19_PIN      GPIO_Pin_1  
#define  LOCK19_MODE     GPIO_Mode_Out_PP  
 
#define  LOCK20_USE					 1
#define  LOCK20_PORT     GPIOE
#define  LOCK20_PIN      GPIO_Pin_4  
#define  LOCK20_MODE     GPIO_Mode_Out_PP  
 
#define  LOCK21_USE					 1
#define  LOCK21_PORT     GPIOC  
#define  LOCK21_PIN      GPIO_Pin_6   
#define  LOCK21_MODE     GPIO_Mode_Out_PP  

#define  LOCK22_USE					 1
#define  LOCK22_PORT     GPIOD  			
#define  LOCK22_PIN      GPIO_Pin_12  
#define  LOCK22_MODE     GPIO_Mode_Out_PP  
 
#define  LOCK23_USE					 1
#define  LOCK23_PORT     GPIOC				
#define  LOCK23_PIN      GPIO_Pin_9 
#define  LOCK23_MODE     GPIO_Mode_Out_PP  
  
#define  LOCK24_USE					 1
#define  LOCK24_PORT     GPIOD			
#define  LOCK24_PIN      GPIO_Pin_11  
#define  LOCK24_MODE     GPIO_Mode_Out_PP  
 									
					
#define    LOCK1(x)   x ? GPIO_SetBits(LOCK1_PORT,LOCK1_PIN):  GPIO_ResetBits(LOCK1_PORT,LOCK1_PIN)
#define    LOCK2(x)   x ? GPIO_SetBits(LOCK2_PORT,LOCK2_PIN):  GPIO_ResetBits(LOCK2_PORT,LOCK2_PIN)
#define    LOCK3(x)   x ? GPIO_SetBits(LOCK3_PORT,LOCK3_PIN):  GPIO_ResetBits(LOCK3_PORT,LOCK3_PIN)
#define    LOCK4(x)   x ? GPIO_SetBits(LOCK4_PORT,LOCK4_PIN):  GPIO_ResetBits(LOCK4_PORT,LOCK4_PIN)
#define    LOCK5(x)   x ? GPIO_SetBits(LOCK5_PORT,LOCK5_PIN):  GPIO_ResetBits(LOCK5_PORT,LOCK5_PIN)
#define    LOCK6(x)   x ? GPIO_SetBits(LOCK6_PORT,LOCK6_PIN):  GPIO_ResetBits(LOCK6_PORT,LOCK6_PIN)
#define    LOCK7(x)   x ? GPIO_SetBits(LOCK7_PORT,LOCK7_PIN):  GPIO_ResetBits(LOCK7_PORT,LOCK7_PIN)
#define    LOCK8(x)   x ? GPIO_SetBits(LOCK8_PORT,LOCK8_PIN):  GPIO_ResetBits(LOCK8_PORT,LOCK8_PIN)
#define    LOCK9(x)   x ? GPIO_SetBits(LOCK9_PORT,LOCK9_PIN):  GPIO_ResetBits(LOCK9_PORT,LOCK9_PIN)
#define    LOCK10(x)   x ? GPIO_SetBits(LOCK10_PORT,LOCK10_PIN):  GPIO_ResetBits(LOCK10_PORT,LOCK10_PIN)
#define    LOCK11(x)   x ? GPIO_SetBits(LOCK11_PORT,LOCK11_PIN):  GPIO_ResetBits(LOCK11_PORT,LOCK11_PIN)
#define    LOCK12(x)   x ? GPIO_SetBits(LOCK12_PORT,LOCK12_PIN):  GPIO_ResetBits(LOCK12_PORT,LOCK12_PIN)
#define    LOCK13(x)   x ? GPIO_SetBits(LOCK13_PORT,LOCK13_PIN):  GPIO_ResetBits(LOCK13_PORT,LOCK13_PIN)
#define    LOCK14(x)   x ? GPIO_SetBits(LOCK14_PORT,LOCK14_PIN):  GPIO_ResetBits(LOCK14_PORT,LOCK14_PIN)
#define    LOCK15(x)   x ? GPIO_SetBits(LOCK15_PORT,LOCK15_PIN):  GPIO_ResetBits(LOCK15_PORT,LOCK15_PIN)
#define    LOCK16(x)   x ? GPIO_SetBits(LOCK16_PORT,LOCK16_PIN):  GPIO_ResetBits(LOCK16_PORT,LOCK16_PIN)
#define    LOCK17(x)   x ? GPIO_SetBits(LOCK17_PORT,LOCK17_PIN):  GPIO_ResetBits(LOCK17_PORT,LOCK17_PIN)
#define    LOCK18(x)   x ? GPIO_SetBits(LOCK18_PORT,LOCK18_PIN):  GPIO_ResetBits(LOCK18_PORT,LOCK18_PIN)
#define    LOCK19(x)   x ? GPIO_SetBits(LOCK19_PORT,LOCK19_PIN):  GPIO_ResetBits(LOCK19_PORT,LOCK19_PIN)
#define    LOCK20(x)   x ? GPIO_SetBits(LOCK20_PORT,LOCK20_PIN):  GPIO_ResetBits(LOCK20_PORT,LOCK20_PIN)
#define    LOCK21(x)   x ? GPIO_SetBits(LOCK21_PORT,LOCK21_PIN):  GPIO_ResetBits(LOCK21_PORT,LOCK21_PIN)
#define    LOCK22(x)   x ? GPIO_SetBits(LOCK22_PORT,LOCK22_PIN):  GPIO_ResetBits(LOCK22_PORT,LOCK22_PIN)
#define    LOCK23(x)   x ? GPIO_SetBits(LOCK23_PORT,LOCK23_PIN):  GPIO_ResetBits(LOCK23_PORT,LOCK23_PIN)
#define    LOCK24(x)   x ? GPIO_SetBits(LOCK24_PORT,LOCK24_PIN):  GPIO_ResetBits(LOCK24_PORT,LOCK24_PIN)
void LOCK_GPIO_Config(void);
void LOCKn_ONOFF(uint8_t n,uint8_t onoff);	
int8_t Locker_open(uint8_t ch);

#endif /* __LOCK_H */
