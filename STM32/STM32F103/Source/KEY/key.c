/*
 * key.c
 *
 *  Created on: 2015年3月17日
 *      Author: souls
 */
#include "key.h"
#include "../GPIO/Beep.h"

#include "../System/Timer/timer4.h"
#define KEY_GetCurrentTime TIM4_GetCurrentTime
#define KEY_GetDistanceTime TIM4_GetDistanceTime

//按键初始状态 高电平,当有按键按下的时候 低电平
//按键扫描就是检测有没有低电平触发

typedef struct _KEY{
	uint8_t ispress;	//
	uint8_t iskey;		//按键来源
	uint16_t curkey;
	uint16_t lastkey;
	uint16_t trigkey;
	uint16_t presskey;
	uint32_t timeout;
}KEY_t;

void KeySwap(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; //定义结构体

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	// ROW 行 PB0 1 10 11 输入上拉
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//ROW
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_ResetBits(GPIOB, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
}

void KeyInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; //定义结构体		
	//GPIO_ReadInputData(); 	
	//	COL 列  PB15 14 13 12 PA0 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	// ROW 行 PB0 1 10 11 输入上拉
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//ROW
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//输出为0 
	GPIO_ResetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_10 | GPIO_Pin_11);
	GPIO_ResetBits(GPIOA, GPIO_Pin_0);
}
uint8_t isKeyRelease()
{
	//有任意IO电平为低
	return (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15) &&
		GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) &&
		GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) &&
		GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12));
}
uint8_t isKeyPress()
{
	//所有的IO全为高电平
	return (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15) ||
		GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) ||
		GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) ||
		GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12));
	//return (!isKeyRelease());
}
uint8_t isKey(uint16_t key)
{
	return (
		key == KEY_A ||
		key == KEY_B ||
		key == KEY_C ||
		key == KEY_1 ||
		key == KEY_2 ||
		key == KEY_3 ||
		key == KEY_4 ||
		key == KEY_5 ||
		key == KEY_6 ||
		key == KEY_7 ||
		key == KEY_8 ||
		key == KEY_9 ||
		key == KEY_0 ||
		key == KEY_F1 ||
		key == KEY_F2 ||
		key == KEY_F3 ||
		key == KEY_F4 ||
		key == KEY_STAR ||
		key == KEY_SHARP
		);
}

uint16_t KeyScan()
{
	uint8_t i;
	uint16_t key = 0;
	if (isKeyPress()){
		for (i = 0xff; i > 0; i--){ ; }
		if (isKeyPress()){
			key = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15);
			key |= GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) << 1;
			key |= GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) << 2;
			key |= GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) << 3;
			KeySwap(); for (i = 100; i > 0; i--){ ; }
			key |= GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) << 4;
			key |= GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) << 5;
			key |= GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10) << 6;
			key |= GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) << 7;
			key |= GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) << 8;
			KeyInit();
			
			if (isKey(key)){
				//BeepStart(50);
				Beep(500);
			}
			else{
				key = 0;
			}
			
		}
	}
	return key;
}

//等待按键按下
uint16_t WaitKey(uint32_t timeout){
	//KeyInit();
	uint16_t key =0;
	uint32_t time =KEY_GetCurrentTime();
	while (1){	
		key=KeyScan();
		if(isKey(key))break;													//取得按键退出
		if(KEY_GetDistanceTime(time)>timeout)break;	//超时退出
	}
	return key;
}

volatile uint16_t key = 0;		//键值	
volatile uint8_t  trigkey = 0;	//连续触发标志
volatile uint32_t keytime = 0;	//按键按下持续时间
// 返回值0x00  没有按键按下
// 返回值0x01  相同按键连续按下 
// 返回值0xFF  不同按键按下
uint8_t  GetKey(){
	uint8_t i;

	static volatile uint16_t tmpkey = 0;
	//KeyInit();
	if (isKeyRelease()){
		//tmpkey = 0;
		trigkey = 0;
	}
	else
	{
		key = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15);
		key |= GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) << 1;
		key |= GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) << 2;
		key |= GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) << 3;
		KeySwap(); for (i = 100; i > 0; i--){ ; }
		key |= GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) << 4;
		key |= GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) << 5;
		key |= GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10) << 6;
		key |= GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) << 7;
		key |= GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) << 8;
		KeyInit();
		if (isKey(key)){

			if (tmpkey == key){
				trigkey = 1; //连续触发标志
		
				if(keytime == 0){	
					Beep(100);
				}		
				keytime++;
			}
			else
			{
				tmpkey = key;
				trigkey = 0xff; //非连续触发
				keytime = 0;
				Beep(100);
			}
		}
		else{
			key = 0;
		}
	}
	return trigkey;
}



void  keyEvent(void(*keyhandler)(),void (*timeouthandler)(),uint32_t timeout){
	if (timeout<10)
	{
		timeout = 10;
	}

	if (trigkey == 0xFF){
		//单次触发事件
		if (keyhandler)
		{
			keyhandler();
		}
	}else{
		//长按触发事件
		if (keytime>timeout)
		{
			keytime = 0;	
			if (timeouthandler)
			{
				timeouthandler();
			}
		}

	}
}
