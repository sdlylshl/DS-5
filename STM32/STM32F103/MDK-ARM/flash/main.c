#include "stm32f10x.h"
#include "../../Source/System/Flash/SST25_flash.h"
#include "../../Source/System/USART/usart1.h"
int main(){
	USART1_NVIC_Config(7);
	USART1_Init();	
	//SPI2	
	SST25_Flash_init();
	FlashReadID();
	//testFlash();
}

void Delay_ms(uint32_t ms){

}
