#include <stdio.h>
#include "config.h"
char *str = "\r\n This is a USART1_printf demo \r\n";
extern void nrf_main0(void);
extern void nrf_main(void);
int NRF_master_main(void);
int main() {
//	uint8_t i;
	uint32_t Time = 0;
	//RCC_Config();
	//NVIC_Config();
	//DMA_Config();
	USART1_Init();
//	USART3_Init();
	TIM2_Start();	//通用定时器
	
//	TIM4_Start(); //用来做网络延时计时使用
	//nrf
	SPI2_Init();
	
	PS2_Init();
	LED_GPIO_Config();	

		//开总中断
	__set_PRIMASK(0);
	nrf_main0();
//		NRF_device_main();
//NRF_master_main();

#ifdef CABINET_DEVICE
	LOCK_GPIO_Config();
	SENSOR_GPIO_Config();
#endif

	//LCD12864P_main();

	//while(1)
		printf("USART1 start");
	//CAN_main();
	Ethernet_Init();

	
	//Ethernet_main();
	printf("%slist", str);
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

		Time = time4;
		printf("Time1 :%x \r\n", Time);
		//Delay_ms(1000);
		Time = time4;
		printf("Time2 :%x \r\n", Time);
		//LED8(ON);LED7(ON);LED6(ON);
		Time = time4;
		printf("Time2 :%x \r\n", Time);
		//Delay_ms(1000);
		//LED8(OFF);LED7(OFF);LED6(OFF);
		printf("%s", str);
		//
		//PfmBuffer_Next(PfmBuffer_Read());
	}
	return 0;
}

#ifdef  USE_FULL_ASSERT

/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while (1)
	{
	}
}
#endif
