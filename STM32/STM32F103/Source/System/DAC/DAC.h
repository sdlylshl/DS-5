#ifndef DAC_H
#define DAC_H

#include "stm32f10x.h"
/*Private typedef --------------------------------------------------------------*/
/*Private define  --------------------------------------------------------------*/
#define mode_value 1  //mode_value=0,即使用DAC1 产生一个电压值，否则利用DAC2产生正弦波
#define DAC_DHR12R1_Address 0x40007408	//起始地址+偏置,通道1的12位右对齐数据保持寄存器
#define DAC_DHR12R2_Address 0x40007414 
  
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


extern void DAC_DC(uint32_t DCvalue);
extern void DAC_Config(void);
extern void DA_Wave(void);

#endif 



