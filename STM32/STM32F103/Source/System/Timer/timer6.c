/*
 * timer6.c
 *
 *  Created on: 2015年3月2日
 *      Author: star
 */

#include "version.h"

void TIM6_Config(void){
	TIM_SetCounter(TIM6,0);
//	//中断周期 为 Period*PrescalerCLK
//	TIM_SetAutoreload(TIM6, 125); //重载值最大为65536
//	//重新初始化定时器
//	TIM_PrescalerConfig(TIM6, 23, TIM_PSCReloadMode_Update); // (SystemCoreClock / 1000000 - 1); 1us
//
//	//72-22050 136/23
//	//72-128kbps/44100 136/11
//	//
//	//72-24000/192kbp-8-1-1 125/23
//
//	TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);

}
