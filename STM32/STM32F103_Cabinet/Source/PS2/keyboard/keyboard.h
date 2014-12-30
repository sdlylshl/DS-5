#ifndef __KEYBOARD_H
#define __KEYBOARD_H	 
#include "../ps2.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//鼠标 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/13
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//HOST->DEVICE的命令集				  				   
#define PS_RESET    		0XFF //复位命令 回应0XFA
#define RESEND	     		0XFE //再次发送
#define SET_DEFAULT	 		0XF6 //使用默认设置 回应0XFA
#define DIS_DATA_REPORT   	0XF5 //禁用数据报告 回应0XFA
#define EN_DATA_REPORT    	0XF4 //使能数据报告 回应0XFA
#define SET_SAMPLE_RATE		0XF3 //设置采样速率 回应0XFA
#define GET_DEVICE_ID       0XF2 //得到设备ID   回应0XFA+ID
#define SET_REMOTE_MODE     0XF0 //设置到REMOTE模式 回应OXFA
#define SET_WRAP_MODE       0XEE //设置到WRAP模式 回应0XFA
#define RST_WRAP_MODE       0XEC //回到WRAP之前的模式 回应0XFA
#define READ_DATA           0XEB //读取数据 回应0XFA+位移数据包
#define SET_STREAM_MODE     0XEA //设置到STREAM模式 回应0XFA
#define STATUS_REQUEST      0XE9 //请求得到状态 回应0XFA+3个字节
#define SET_RESOLUTION      0XE8 //设置分辨率 回应OXFA+读取1个字节+应带0XFA
#define SET_SCALING21       0XE7 //设置缩放比率为2:1 回应0XFA
#define SET_SCALING11       0XE6 //设置缩放比率为1:1 回应0XFA
//DEVICE->HOST的指令
#define ERROR	     		0XFC //错误
//#define RESEND	     	0XFE //再次发送
#define   Key_Code_0        

#define   Key_0							'0'
#define   Key_1							'1'
#define   Key_2							'2'
#define   Key_3							'3'
#define   Key_4							'4'
#define   Key_5							'5'
#define   Key_6							'6'
#define   Key_7							'7'
#define   Key_8							'8'
#define   Key_9							'9'
#define   Key_Dot						'.'
#define 	Key_SAVE_ASCII    's' 
#define 	Key_GET_ASCII     'g' 
#define 	Key_ENTER_ASCII   'e'    //回车
#define 	Key_BACK_ASCII    'b'   //退格
#define 	Key_DEL_ASCII    'd'   //退格

#define AREA_ADDRESS_MASTER   0x7A
#define MASTER_ADDRESS        0x01 
#define Reply_Call_Code       0xD1

//extern uint8_t Call_Buf[16];

void Init_Keyboard(void);
void Keyboard_Data_Pro(void);

#endif

