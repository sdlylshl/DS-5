#include "stm32f10x.h"

#include "../../Source/FatFs/R0.11/diskio.h"
#include "../../Source/System/Flash/SST25_flash.h"
#include "../../Source/System/USART/usart1.h"
extern void filesysinit(void);
int main(){
	USART1_NVIC_Config(7);
	USART1_Init();	
	//SPI2	
	SST25_Flash_init();
//	filesysinit();
	testFlash();
}

void Delay_ms(uint32_t ms){

}
