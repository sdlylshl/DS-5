#include "config.h"
#include "util.h"
#include <stdio.h>
#include <string.h>
#include "stm32f10x.h"

#include "lcd.h"
#include "matrix_key.h"



u8 key_val[19] = {0};




void set_key_output(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOx, &GPIO_InitStructure);
}

void set_key_input(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
	GPIO_Init(GPIOx, &GPIO_InitStructure);
}


void key_in_test()
{
	u8 val;
	set_key_input(GPIOB, PB15_1_4_7_STAR);
	val = GPIO_ReadInputDataBit(GPIOB, PB15_1_4_7_STAR);
	printf("val: %d", val);
}


void init_matrix_key()
{
	GPIO_SetBits(GPIOB,PB0_1_2_3_F1);
	GPIO_SetBits(GPIOB,PB1_4_5_6_F2);
	GPIO_SetBits(GPIOB,PB10_7_8_9_F3);
	GPIO_SetBits(GPIOB,PB11_STAR_0_WELL_F4);
	GPIO_SetBits(GPIOA,PA0_A_B_C);
}


void press_hint_voice()
{
	beep_on();
	Delay_ms(100);
	beep_off();
}


void key_stat(u8 on)
{
	if(on)
		GPIO_ResetBits(GPIOD, LED5);
	else
		GPIO_SetBits(GPIOD, LED5);
}
void scan_matrix_key(u8* led_on)
{
	
	u8 i;

	for(i=0; i<19; i++)
		key_val[i] = 1;


	//ROW 0
	init_matrix_key();
	GPIO_ResetBits(GPIOB,PB0_1_2_3_F1);
	//key 1
	key_val[KEY_1] = GPIO_ReadInputDataBit(GPIOB, PB15_1_4_7_STAR);
	
	//key 2
	key_val[KEY_2] = GPIO_ReadInputDataBit(GPIOB, PB14_2_5_8_0_A);
	
	//key3
	key_val[KEY_3] = GPIO_ReadInputDataBit(GPIOB, PB13_3_6_9_WELL_B);
	
	//key f1
	key_val[KEY_F1] = GPIO_ReadInputDataBit(GPIOB, PB12_F1_F2_F3_F4_C);
	



	//ROW 1
	init_matrix_key();
	GPIO_ResetBits(GPIOB,PB1_4_5_6_F2);
	//key 4
	key_val[KEY_4] = GPIO_ReadInputDataBit(GPIOB, PB15_1_4_7_STAR);
	
	//key 5
	key_val[KEY_5] = GPIO_ReadInputDataBit(GPIOB, PB14_2_5_8_0_A);
	
	//key6
	key_val[KEY_6] = GPIO_ReadInputDataBit(GPIOB, PB13_3_6_9_WELL_B);
	
	//key f2
	key_val[KEY_F2] = GPIO_ReadInputDataBit(GPIOB, PB12_F1_F2_F3_F4_C);
	


	//ROW 2
	init_matrix_key();
	GPIO_ResetBits(GPIOB,PB10_7_8_9_F3);
	//key 7
	key_val[KEY_7] = GPIO_ReadInputDataBit(GPIOB, PB15_1_4_7_STAR);
	
	//key 8
	key_val[KEY_8] = GPIO_ReadInputDataBit(GPIOB, PB14_2_5_8_0_A);
	
	//key9
	key_val[KEY_9] = GPIO_ReadInputDataBit(GPIOB, PB13_3_6_9_WELL_B);
	
	//key f3
	key_val[KEY_F3] = GPIO_ReadInputDataBit(GPIOB, PB12_F1_F2_F3_F4_C);
	



	//ROW 3
	init_matrix_key();
	GPIO_ResetBits(GPIOB,PB11_STAR_0_WELL_F4);
	//key STAR
	key_val[KEY_STAR] = GPIO_ReadInputDataBit(GPIOB, PB15_1_4_7_STAR);
	//key 0
	key_val[KEY_0] = GPIO_ReadInputDataBit(GPIOB, PB14_2_5_8_0_A);
	//key WELL
	key_val[KEY_WELL] = GPIO_ReadInputDataBit(GPIOB, PB13_3_6_9_WELL_B);
	//key f4
	key_val[KEY_F4] = GPIO_ReadInputDataBit(GPIOB, PB12_F1_F2_F3_F4_C);


	//ROW 4
	init_matrix_key();
	GPIO_ResetBits(GPIOA,PA0_A_B_C);
	//key A
	key_val[KEY_A] = GPIO_ReadInputDataBit(GPIOB, PB14_2_5_8_0_A);
	//key B
	key_val[KEY_B] = GPIO_ReadInputDataBit(GPIOB, PB13_3_6_9_WELL_B);
	//key C
	key_val[KEY_C] = GPIO_ReadInputDataBit(GPIOB, PB12_F1_F2_F3_F4_C);

	//printf("key val: ");
	//for(i=0; i<19; i++)
	//	printf("%d ", key_val[i]);
	//printf("\n");
	for(i=0; i<19; i++)
	{
		if(key_val[i] == KEY_PRESSED)
		{
			*led_on = 1;
			press_hint_voice();
		}
		
	}
	
}




