#include "stm32f10x.h"

#include "../../Source/FatFs/R0.11/diskio.h"
#include "../../Source/System/Flash/SST25_flash.h"
extern void filesysinit(void);
int main(){
	//SPI2
	//SPI_Flash_Init();
	filesysinit();
	testFlash();
}

void Delay_ms(uint32_t ms){

}
