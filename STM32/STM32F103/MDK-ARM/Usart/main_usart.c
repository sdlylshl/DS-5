#include "stm32f10x.h"
#include "stdio.h"
#include "../../Source/System/USART/usart1.h"

int main(){
	USART1_NVIC_Config(7);
	USART1_Init();
	printf("hello word!\n");
}

void Delay_ms(uint32_t ms){

}
