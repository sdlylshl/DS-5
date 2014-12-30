#ifndef __PS2_H
#define __PS2_H

#include "version.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//PS2 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/13
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
#ifdef CABINET_DEVICE

#else
#define PS2_CLK_PORT		GPIOB
#define PS2_CLK_PIN			GPIO_Pin_1
#define PS2_CLK_MODE		GPIO_Mode_IPU

#define PS2_DAT_PORT		GPIOB
#define PS2_DAT_PIN			GPIO_Pin_0
#define PS2_DAT_MODE		GPIO_Mode_IPU
#endif
//物理接口定义
//PS2输入 		  				    
#define PS2_SCL PBin(1)			
#define PS2_SDA PBin(0)			
//PS2输出
#define PS2_SCL_OUT PBout(1)		
#define PS2_SDA_OUT PBout(0)		

//设置PS2_SCL输入输出状态.		  
#define PS2_SET_SCL_IN()  {GPIOB->CRL&=0XF0FFFFFF;GPIOB->CRL|=0X08000000;}
#define PS2_SET_SCL_OUT() {GPIOB->CRL&=0XF0FFFFFF;GPIOB->CRL|=0X03000000;}	  
//设置PS2_SDA输入输出状态.		  
#define PS2_SET_SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=0X80000000;}
#define PS2_SET_SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=0X30000000;} 

#define MOUSE    0X20 //鼠标模式
#define KEYBOARD 0X10 //键盘模式
#define CMDMODE  0X00 //发送命令
//PS2_Status当前状态标志
//[5:4]:当前工作的模式;[7]:接收到一次数据
//[6]:校验错误;[3:0]:收到的数据长度;	 
extern uint8_t PS2_Status;       //定义为命令模式
extern uint8_t PS2_DATA_BUF[16]; //ps2数据缓存区
//extern uint8_t MOUSE_ID;
extern uint8_t KeyBoard_ASCII;

void PS2_Init(void);
void EXTI1_IRQHandle(void);
uint8_t PS2_Send_Cmd(uint8_t cmd);
void PS2_Set_Int(uint8_t en);
uint8_t PS2_Get_Byte(void);
void PS2_En_Data_Report(void);  
void PS2_Dis_Data_Report(void);		  				    
#endif





















