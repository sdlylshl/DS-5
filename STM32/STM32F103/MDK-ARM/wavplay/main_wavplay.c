
#include "stdio.h"
#include "stm32f10x.h"
#include "../../Source/Wavplay/wavplay.h"
#include "../../Source/System/DAC/dac.h"
#include "../../Source/System/Timer/timer2.h"
#include "../../Source/System/USART/usart1.h"
extern const uint8_t dear[];
int main(){		
	__set_PRIMASK(1);	//关中断
	NVIC_SetPriorityGrouping(4);
	USART1_NVIC_Config(7);
	TIM2_NVIC_Config(8);
	__set_PRIMASK(0);	//开中断
	TIM2_Init();
	USART1_Init();
	printf("sss");
	DAC_DC(4095);
	DAC_Config();
	wavplay((uint8_t *)dear);
	//DAC_DC(3095);
	while(1);
}

void Delay_ms(uint32_t ms){

}
