#include "stm32f10x.h"

#include "../../Source/FatFs/R0.11/diskio.h"
#include "../../Source/System/Flash/SST25_flash.h"
#include "../../Source/System/USART/usart1.h"
#include "../../Source/System/Timer/timer4.h"
#include "../../Source/W5500/Application/Application.h"
extern void filesysinit(void);
int main(){
	uint32_t Time = 0;
	USART1_NVIC_Config(7);
	USART1_Init();
	TIM4_Config();
	//SPI2	
	Ethernet_Init();
		while (1) {

		//printf("AT+CPIN?\r");
		//Delay_ms(5000);
//		for (i = 0; i < 25; i++)
//		{
//			if (GetSensorStatus(i))
//			{
//				LOCKn_ONOFF(i, 1);
//			}
//			else
//			{
//				LOCKn_ONOFF(i, 0);
//			}
//		}
//		for (i = 0; i < 5; i++)
//		{
//			Locker_open(i);
//		}
		Application();

//		Time = time4;
//		printf("Time1 :%x \r\n", Time);
//		//Delay_ms(1000);
//		Time = time4;
//		printf("Time2 :%x \r\n", Time);
//		//LED8(ON);LED7(ON);LED6(ON);
//		Time = time4;
//		printf("Time2 :%x \r\n", Time);
//		//Delay_ms(1000);
//		//LED8(OFF);LED7(OFF);LED6(OFF);
//		printf("%s", str);
		//
		//PfmBuffer_Next(PfmBuffer_Read());
	}
}

//void Delay_ms(uint32_t ms){}
