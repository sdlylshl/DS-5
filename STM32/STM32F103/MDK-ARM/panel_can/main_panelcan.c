#include "stm32f10x.h"
#include "stdio.h"
#include "../../Source/System/USART/usart1.h"
#include "../../Source/System/Delay/systick.h"
#include "../../Source/System/CAN/can1.h"
#include "../../Source/panel/panel_can.h"
#include "../../Source/LCD1602/lcd1602a.h"
#include "../../Source/Menu/menu.h"
int main(){
	USART1_NVIC_Config(7);
	USART1_Init();
	printf("hello");
	LCD1602_Init();
	ShowMenu();
	//panel_can();
}


