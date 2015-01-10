/********************************************************************************
 *文件名：12864LCD底层驱动程序
 *库版本：V3.5
 *MCU：STM32F103RBT6(72Mhz)
 *硬件接口：CS->=>LCD12864_Pin_4;   SID=>LCD12864_Pin_5；  CLKLCD12864_Pin_6
 *说明：无      
 *作者：HHH_MCU.2011.8.19
 ********************************************************************************/
#ifndef __LCD12864S_H
#define __LED12864S_H

#include "..\LCD12864_config.h"

/*==============================12864管脚定义===========================*/
#define CS_H    GPIO_SetBits(RS_PORT, RS_PIN)     //=>LCD12864_Pin_4
#define CS_L    GPIO_ResetBits(RS_PORT, RS_PIN)

#define SID_H   GPIO_SetBits(RW_PORT, RW_PIN)        //=>LCD12864_Pin_5
#define SID_L   GPIO_ResetBits(RW_PORT, RW_PIN)

#define CLK_H   GPIO_SetBits(EN_PORT, EN_PIN)    //=>LCD12864_Pin_6
#define CLK_L   GPIO_ResetBits(EN_PORT, EN_PIN)

#define SID_Read    GPIO_ReadInputDataBit(BUSY_PORT,BUSY_PIN )     //读引脚
/*================================函数声明==============================*/
void LCD_CLK_Delay(void); //时钟线1/2周期延时
void Send_Byte(uint8_t Temp); //发送一个字节的数据
uint8_t Receive_Byte(void); //接收一个字节的数据
void Write_Data(uint8_t Dat); //写显示数据
void Write_Comm(uint8_t Comm); //写控制命令
void LCD_Init(void); //初始化LCD
void LCD_Clear(void); //清屏
void Check_Busy(void); //LCD12864测忙
void Display_String(uint8_t x, uint8_t y, char *s); //指定位置显示字符
void Display_Image(uint8_t *DData); //显示图形
void Show_Num(uint8_t x, uint8_t y, uint16_t Num); //显示变量的值0~9999
#endif
