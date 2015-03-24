/*
 * key.c
 *
 *  Created on: 2015年3月17日
 *      Author: souls
 */
#include "key.h"
#include "../GPIO/Beep.h"
typedef struct _KEY{
	uint8_t ispress;	//
	uint8_t iskey;		//按键来源
	uint16_t keyvalue;
	uint32_t timeout;
}KEY_t;

volatile uint16_t key;
void KeySwap(void){
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

void KeyInit(void){
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
uint8_t isKeyRelease(){

	return (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15) &&
		GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) &&
		GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) &&
		GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12));
}
uint8_t isKeyPress(){

	return (!isKeyRelease());
}
uint8_t isKey(uint16_t key){
	return (
	key == KEY_A||
	key == KEY_B||
	key == KEY_C||
	key == KEY_1||
	key == KEY_2||
	key == KEY_3||
	key == KEY_4||
	key == KEY_5||
	key == KEY_6||
	key == KEY_7||
	key == KEY_8||
	key == KEY_9||
	key == KEY_0||
	key == KEY_F1||
	key == KEY_F2||
	key == KEY_F3||
	key == KEY_F4||
	key == KEY_STAR||
	key == KEY_SHARP
	);

}
uint16_t KeySacn(){
	uint8_t i;
	key = 0;
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
			if(isKey(key)){
				//BeepStart(50);
				Beep(500);
			}else{
				key =0;
			}
		}
	}
	return key;
}
uint16_t GetKey(){
	KeyInit();
	while (isKeyPress());
	KeySacn();
	return key;
}

uint16_t KeyScan(){
	return 0;
}
