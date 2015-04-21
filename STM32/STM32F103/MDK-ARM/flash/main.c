#include "stm32f10x.h"
#include "../../Source/System/USART/usart1.h"
#include "../../Source/SPIFlash/SST25_flash.h"
#include "../../Source/SPIFlash/W25X_flash.h"

int main(){
	USART1_NVIC_Config(7);
	USART1_Init();	
	//SPI2	
	//W25X_FLASH_Init();
	SST25_Flash_init();
	//FlashReadID();
	//W25X_Flash_Test();
	testFlash();
}

void Delay_ms(uint32_t ms){

}
