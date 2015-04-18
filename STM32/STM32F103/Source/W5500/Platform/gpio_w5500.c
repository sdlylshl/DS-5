
#include "gpio_w5500.h"
//Delay_ms();
//#include "./System/System_config.h"

/*
 typedef enum
{ GPIO_Mode_AIN = 0x0,					//模拟输入
  GPIO_Mode_IN_FLOATING = 0x04, //浮空输入
  GPIO_Mode_IPD = 0x28,					//下拉输入
  GPIO_Mode_IPU = 0x48,					//上拉输入
  GPIO_Mode_Out_OD = 0x14,			//开漏输出
  GPIO_Mode_Out_PP = 0x10,			//推挽输出
  GPIO_Mode_AF_OD = 0x1C,				//开漏复用输出
  GPIO_Mode_AF_PP = 0x18				//推挽复用输出
}GPIOMode_TypeDef;
 
 typedef enum
{ 
  GPIO_Speed_10MHz = 1,
  GPIO_Speed_2MHz, 
  GPIO_Speed_50MHz
}GPIOSpeed_TypeDef;
 */


void WIZ_GPIO_Configuration(void)
{
			GPIO_InitTypeDef GPIO_InitStructure;

				/*开启GPIOC的外设时钟*/
				RCC_APB2PeriphClockCmd( W5500_RCC_APB2_PORT, ENABLE);	
	
        GPIO_InitStructure.GPIO_Pin = WIZ_RDY_PIN;
        GPIO_InitStructure.GPIO_Mode = WIZ_RDY_MODE;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
        GPIO_Init(WIZ_RDY_PORT, &GPIO_InitStructure);
	
        GPIO_InitStructure.GPIO_Pin = WIZ_nRST_PIN;
        GPIO_InitStructure.GPIO_Mode = WIZ_nRST_MODE;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
        GPIO_Init(WIZ_nRST_PORT, &GPIO_InitStructure);

}
