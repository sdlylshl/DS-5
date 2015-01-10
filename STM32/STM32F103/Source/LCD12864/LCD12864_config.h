#ifndef __LCD_CONFIG_H
#define __LCD_CONFIG_H
//使用StdPeriph_Driver库
//#define USE_STDPERIPH_DRIVER 1

#include "..\config.h"
#include ".\LCD12864S\LCD12864S.h"
//#include ".\LCD12864P\LCD12864P.h"

//------------------------------------------------------------------------------
////      智能储物柜V1.1
//#define  LCD12864_RCC_PORT (RCC_APB2Periph_GPIOE)
////4.RS(CS)  H:data L:instruction    //
//#define  RS_PORT     GPIOE  
//#define  RS_PIN      GPIO_Pin_6  
////5.RW(SID) H:read L:write          //
//#define  RW_PORT     GPIOE  
//#define  RW_PIN      GPIO_Pin_5  
////6.EN(SCLK) Enable signal          //
//#define  EN_PORT     GPIOE
//#define  EN_PIN      GPIO_Pin_4  
////15.    H:Parallel L:Serial        //
//#define  PSB_PORT    GPIOE
//#define  PSB_PIN     GPIO_Pin_3  
////17.RST L:Reset                    // 
//#define  RST_PORT    GPIOE 
//#define  RST_PIN     GPIO_Pin_2
//  
//#define  BUSY_PORT   GPIOE 
//#define  BUSY_PIN    GPIO_Pin_15

//#define  LCD12864_DAT_PORT     GPIOE
//#define  LCD12864_DAT_PIN  GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10 |GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15
//---------------------------------------------------------------------------------
//       社会安防

#define  LCD12864_RCC_PORT (RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOE|RCC_APB2Periph_AFIO)

//4.RS(CS)  H:data L:instruction    //PC5
#define  RS_PORT     GPIOC  
#define  RS_PIN      GPIO_Pin_5  
//5.RW(SID) H:read L:write          //PB0
#define  RW_PORT     GPIOB  
#define  RW_PIN      GPIO_Pin_0  
//6.EN(SCLK) Enable signal          //PB1
#define  EN_PORT     GPIOB
#define  EN_PIN      GPIO_Pin_1  
//15.    H:Parallel L:Serial        //PA7
#define  PSB_PORT    GPIOA
#define  PSB_PIN     GPIO_Pin_7  
//17.RST L:Reset                    //PA6 
#define  RST_PORT    GPIOA 
#define  RST_PIN     GPIO_Pin_6

#define  BUSY_PORT   GPIOE 
#define  BUSY_PIN    GPIO_Pin_15

#define  LCD12864_DAT_PORT     GPIOE
#define  LCD12864_DAT_PIN  GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10 |GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15
//---------------------------------------------------------------------------------
//#define  LCD12864_CMD PORT   GPIOE
//#define  LCD_GPIO_CMD  GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6

#define    LCD_RS(x)   x ? GPIO_SetBits(RS_PORT,RS_PIN):  GPIO_ResetBits(RS_PORT,RS_PIN)
#define    LCD_RW(x)   x ? GPIO_SetBits(RW_PORT,RW_PIN):  GPIO_ResetBits(RW_PORT,RW_PIN)
#define    LCD_EN(x)   x ? GPIO_SetBits(EN_PORT,EN_PIN):  GPIO_ResetBits(EN_PORT,EN_PIN)
#define    LCD_PSB(x)  x ? GPIO_SetBits(PSB_PORT,PSB_PIN): GPIO_ResetBits(PSB_PORT,PSB_PIN)
#define    LCD_RST(x)  x ? GPIO_SetBits(RST_PORT,RST_PIN): GPIO_ResetBits(RST_PORT,RST_PIN)

//数据脚充当BUSY
#define  Text_Busy           GPIO_ReadInputDataBit(BUSY_PORT,BUSY_PIN)  //PCin(7)     
#define  LCD_WriteData(x)   {LCD12864_DAT_PORT->BSRR = x<<8 & 0xff00;LCD12864_DAT_PORT->BRR = ((~x)<<8) & 0xff00;}  //高8位的数据
//#define  LCD_WriteData(x)         GPIO_Write(LCD12864_GPIO_DAT ,(x<<8))
//#define  LCD_WriteData(x)   {LCD12864_GPIO_DAT->BSRR =    (((unsigned int)x<<8) | ((unsigned int)(~x)<<24)) ;GPIOE->BRR = ((~x)<<8) & 0xff00;}  //高8位的数据

void LCD12864P_Init(void);
void LCD12864S_Init(void);

void LCD12864P_InitPort(void);
void LCD12864S_InitPort(void);

int LCD12864P_main(void);
void LCD12864S_main(void);
#endif
