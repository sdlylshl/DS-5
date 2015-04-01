/**********************(c)COPYRIGHT SNAIL 2013 *************************************
 ** file name   :	DAC.c
 ** author   	 :	snail
 ** version  	 :	v1.0  (using library 3.5 )
 ** date     	 :	2013/01/26
 ** description :  use the DAC to produce Sine Wave in working Dual model -DMA
 **********************************************************************************/
#include"DAC.h"

#include "stm32f10x_dac.h"
/*Initialize structure definition*/

const uint16_t SineData[32] = { 2047, 2447, 2831, 3185, 3498, 3750, 3939, 4056,
		4095, 4056, 3939, 3750, 3495, 3185, 2831, 2447, 2047, 1647, 1263, 909,
		599, 344, 155, 38, 0, 38, 155, 344, 599, 909, 1263, 1647 };
const uint16_t DAC2_SineWave[128] = { 2047, 2147, 2248, 2347, 2446, 2544, 2641,
		2737, 2830, 2922, 3012, 3099, 3184, 3266, 3346, 3422, 3494, 3564, 3629,
		3691, 3749, 3803, 3852, 3897, 3938, 3974, 4006, 4033, 4055, 4072, 4084,
		4092, 4094, 4092, 4084, 4072, 4055, 4033, 4006, 3974, 3938, 3897, 3852,
		3803, 3749, 3691, 3629, 3564, 3494, 3422, 3346, 3266, 3184, 3099, 3012,
		2922, 2830, 2737, 2641, 2544, 2446, 2347, 2248, 2147, 2047, 1947, 1846,
		1747, 1648, 1550, 1453, 1357, 1264, 1172, 1082, 995, 910, 828, 748, 672,
		600, 530, 465, 403, 345, 291, 242, 197, 156, 120, 88, 61, 39, 22, 10, 2,
		0, 2, 10, 22, 39, 61, 88, 120, 156, 197, 242, 291, 345, 403, 465, 530,
		600, 672, 748, 828, 910, 995, 1082, 1172, 1264, 1357, 1453, 1550, 1648,
		1747, 1846, 1947 };

uint16_t g_Wave[128];
uint16_t DAC1_Value[32];

/*******************************************************************************
 ** function name: RCC_Config
 ** description  : Initialize the DAC ,DMA ,TIM,GPIOA's clock
 ** input        : null
 ** output       : null
 ** return       : null
 *******************************************************************************/
void RCC_DAC_Config(void) {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
}

/*******************************************************************************
 ** function name: GPIO_DAC_Config
 ** description  : The PA.4(DAC_OUT1),PA.5(DAC_OUT2)	shoule be configured in
 analog (AIN).
 ** input        : null
 ** output       : null
 ** return       : null
 *******************************************************************************/
void GPIO_DAC_Config(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;// | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void DMA_DAC_Config(DAC_InitTypeDef * DAC_InitStructure ){

	DMA_InitTypeDef DMA_InitStructure;
#if mode_value
	DAC_Init(DAC_Channel_1, DAC_InitStructure);

	DMA_DeInit(DMA2_Channel3);
	DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR12R1_Address;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) &DAC1_Value;
	DMA_InitStructure.DMA_BufferSize = 32;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

	DMA_Init(DMA2_Channel3, &DMA_InitStructure);
	DMA_Cmd(DMA2_Channel3, ENABLE);

	DAC_Cmd(DAC_Channel_1, ENABLE);
	DAC_DMACmd(DAC_Channel_1, ENABLE);

#else 
	DAC_Init(DAC_Channel_2, DAC_InitStructure);

	DMA_DeInit(DMA2_Channel4);
	DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR12R2_Address;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&g_Wave;
	DMA_InitStructure.DMA_BufferSize = 32;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

	DMA_Init(DMA2_Channel4,&DMA_InitStructure);
	DMA_Cmd(DMA2_Channel4,ENABLE);

	DAC_Cmd(DAC_Channel_2,ENABLE);
	DAC_DMACmd(DAC_Channel_2,ENABLE);
#endif
}
/*******************************************************************************
 ** function name: DAC_Config
 ** description  : Init DAC
 ** input        : null
 ** output       : null
 ** return       : null
 *******************************************************************************/

void DAC_Config(void) {
	uint32_t i = 0;
	DAC_InitTypeDef DAC_InitStructure;

	DAC1_Value[0] = 4096 * 0.612;

	for (i = 0; i < 32; i++) {
		//g_Wave[i]=DAC2_SineWave[i] *0.606; //Vpp =2.0V
		g_Wave[i] = SineData[i] * 0.606; //Vpp =2.0V
		DAC1_Value[i] = SineData[i] * 0.606; //Vpp =2.0V
	}
	RCC_DAC_Config();
	GPIO_DAC_Config();

	TIM_PrescalerConfig(TIM2, 23, TIM_PSCReloadMode_Update);

	TIM_SetAutoreload(TIM2, 125);
	TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);

	DAC_InitStructure.DAC_Trigger = DAC_Trigger_T2_TRGO; //选择TIM2触发器作为DAC的触发事件
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable; //输出不用缓冲
	DAC_Init(DAC_Channel_1, &DAC_InitStructure);	
	
	DAC_SetChannel1Data(DAC_Align_12b_R,0);
	DAC_SoftwareTriggerCmd(DAC_Channel_1,ENABLE);
	DAC_Cmd(DAC_Channel_1, ENABLE);	
	

	//DMA_DAC_Config(&DAC_InitStructure );
	TIM_Cmd(TIM2, ENABLE);

}

