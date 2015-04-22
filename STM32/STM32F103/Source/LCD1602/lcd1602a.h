#ifndef __LCD_1602A_H
#define __LCD_1602A_H
#include "stm32f10x.h"
#define LCD1602_RCC_PORT		RCC_APB2Periph_GPIOC

#define LCD_1602A_PORT			GPIOC

#define LCD_1602A_D7				GPIO_Pin_0
#define LCD_1602A_D6				GPIO_Pin_1
#define LCD_1602A_D5				GPIO_Pin_2
#define LCD_1602A_D4				GPIO_Pin_3
#define LCD_1602A_D3				GPIO_Pin_4
#define LCD_1602A_D2				GPIO_Pin_5
#define LCD_1602A_D1				GPIO_Pin_6
#define LCD_1602A_D0				GPIO_Pin_7
//#define LCD_1602A_LCD				GPIO_Pin_8
#define LCD_1602A_RS				GPIO_Pin_10
#define LCD_1602A_RW				GPIO_Pin_11
#define LCD_1602A_E					GPIO_Pin_12

void LCD1602_Init(void);
void LCD1602_Write_String(uint8_t x,uint8_t y,char* string);

extern void LCD1602_Update(void);
extern void LCD1602_Flashing(uint32_t ms);
extern void(*LCD1602Callback_ISR)(void);
#endif
