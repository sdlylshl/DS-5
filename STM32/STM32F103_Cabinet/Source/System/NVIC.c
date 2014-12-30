//printf();
#include "stdio.h"

#include "version.h"
//#include "System_config.h"
/*
 * 函数名：NVIC_Config
 * 描述  ：DMA 中断配置
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
 NVIC_IRQChannel：
 
 NonMaskableInt_IRQn         = -14,  // 2 Non Maskable Interrupt                           
 MemoryManagement_IRQn       = -12,  // 4 Cortex-M3 Memory Management Interrupt            
 BusFault_IRQn               = -11,  // 5 Cortex-M3 Bus Fault Interrupt                    
 UsageFault_IRQn             = -10,  // 6 Cortex-M3 Usage Fault Interrupt                  
 SVCall_IRQn                 = -5,   // 11 Cortex-M3 SV Call Interrupt                     
 DebugMonitor_IRQn           = -4,   // 12 Cortex-M3 Debug Monitor Interrupt               
 PendSV_IRQn                 = -2,   // 14 Cortex-M3 Pend SV Interrupt                     
 SysTick_IRQn                = -1,   // 15 Cortex-M3 System Tick Interrupt                 
 //
 WWDG_IRQn                   = 0,    // Window WatchDog Interrupt                          
 PVD_IRQn                    = 1,    // PVD through EXTI Line detection Interrupt          
 TAMPER_IRQn                 = 2,    // Tamper Interrupt                                   
 RTC_IRQn                    = 3,    // RTC global Interrupt                               
 FLASH_IRQn                  = 4,    // FLASH global Interrupt                             
 RCC_IRQn                    = 5,    // RCC global Interrupt                               
 EXTI0_IRQn                  = 6,    // EXTI Line0 Interrupt                               
 EXTI1_IRQn                  = 7,    // EXTI Line1 Interrupt                               
 EXTI2_IRQn                  = 8,    // EXTI Line2 Interrupt                               
 EXTI3_IRQn                  = 9,    // EXTI Line3 Interrupt                               
 EXTI4_IRQn                  = 10,   // EXTI Line4 Interrupt                               
 DMA1_Channel1_IRQn          = 11,   // DMA1 Channel 1 global Interrupt                    
 DMA1_Channel2_IRQn          = 12,   // DMA1 Channel 2 global Interrupt                    
 DMA1_Channel3_IRQn          = 13,   // DMA1 Channel 3 global Interrupt                    
 DMA1_Channel4_IRQn          = 14,   // DMA1 Channel 4 global Interrupt                    
 DMA1_Channel5_IRQn          = 15,   // DMA1 Channel 5 global Interrupt                    
 DMA1_Channel6_IRQn          = 16,   // DMA1 Channel 6 global Interrupt                    
 DMA1_Channel7_IRQn          = 17,   // DMA1 Channel 7 global Interrupt                    

 ADC1_2_IRQn                 = 18,   // ADC1 and ADC2 global Interrupt                     
 USB_HP_CAN1_TX_IRQn         = 19,   // USB Device High Priority or CAN1 TX Interrupts     
 USB_LP_CAN1_RX0_IRQn        = 20,   // USB Device Low Priority or CAN1 RX0 Interrupts     
 CAN1_RX1_IRQn               = 21,   // CAN1 RX1 Interrupt                                 
 CAN1_SCE_IRQn               = 22,   // CAN1 SCE Interrupt                                 
 EXTI9_5_IRQn                = 23,   // External Line[9:5] Interrupts                      
 TIM1_BRK_IRQn               = 24,   // TIM1 Break Interrupt                               
 TIM1_UP_IRQn                = 25,   // TIM1 Update Interrupt                              
 TIM1_TRG_COM_IRQn           = 26,   // TIM1 Trigger and Commutation Interrupt             
 TIM1_CC_IRQn                = 27,   // TIM1 Capture Compare Interrupt                     
 TIM2_IRQn                   = 28,   // TIM2 global Interrupt                              
 TIM3_IRQn                   = 29,   // TIM3 global Interrupt                              
 I2C1_EV_IRQn                = 31,   // I2C1 Event Interrupt                               
 I2C1_ER_IRQn                = 32,   // I2C1 Error Interrupt                               
 SPI1_IRQn                   = 35,   // SPI1 global Interrupt                              
 USART1_IRQn                 = 37,   // USART1 global Interrupt                            
 USART2_IRQn                 = 38,   // USART2 global Interrupt                            
 EXTI15_10_IRQn              = 40,   // External Line[15:10] Interrupts                    
 RTCAlarm_IRQn               = 41,   // RTC Alarm through EXTI Line Interrupt              
 USBWakeUp_IRQn              = 42    // USB Device WakeUp from suspend through EXTI Line Interrupt  
 */

// 	PriorityGroup > PreemptionPriority抢占 > SubPriority
//	同一组优先级下的中断源间，没有中断嵌套
//	不同组优先级间，可以中断嵌套 (NVIC_PriorityGroup_0优先级最高)
//  ============================================================================================================================
//    NVIC_PriorityGroup   | NVIC_IRQChannelPreemptionPriority | NVIC_IRQChannelSubPriority  | Description
//  ============================================================================================================================
//   NVIC_PriorityGroup_0  |                0                  |            0-15             |   0 bits for pre-emption priority
//                         |                                   |                             |   4 bits for subpriority
//  ----------------------------------------------------------------------------------------------------------------------------
//   NVIC_PriorityGroup_1  |                0-1                |            0-7              |   1 bits for pre-emption priority
//                         |                                   |                             |   3 bits for subpriority
//  ----------------------------------------------------------------------------------------------------------------------------    
//   NVIC_PriorityGroup_2  |                0-3                |            0-3              |   2 bits for pre-emption priority
//                         |                                   |                             |   2 bits for subpriority
//  ----------------------------------------------------------------------------------------------------------------------------    
//   NVIC_PriorityGroup_3  |                0-7                |            0-1              |   3 bits for pre-emption priority
//                         |                                   |                             |   1 bits for subpriority
//  ----------------------------------------------------------------------------------------------------------------------------    
//   NVIC_PriorityGroup_4  |                0-15               |            0                |   4 bits for pre-emption priority
//                         |                                   |                             |   0 bits for subpriority                       
//  ============================================================================================================================
void NVIC_Config(void) {
//		NVIC_InitTypeDef NVIC_InitStructure;

//1.在有bootloader的时候，APP函数要重新定义这个地址，偏移量一般不是0了。
//2.调试模式下要加
#ifdef  VECT_TAB_RAM  
	/* Set the Vector Table base location at 0x20000000 */
	NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  /* VECT_TAB_FLASH  */
	/* Set the Vector Table base location at 0x08000000 */
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif

	/* Configure one bit for preemption priority */
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_SetPriorityGrouping(6);

}

void NVIC_Info(IRQn_Type IRQn) {
	uint32_t PriorityGrouping;
	uint32_t pPreemptPriority_IRQn;
	uint32_t pSubPriority_IRQn;
	PriorityGrouping = NVIC_GetPriorityGrouping();
	NVIC_DecodePriority(NVIC_GetPriority(IRQn), PriorityGrouping,
			&pPreemptPriority_IRQn, &pSubPriority_IRQn);
	printf("NVIC_GetPriorityGrouping:%d\n", PriorityGrouping);
	printf("pPreemptPriority_IRQn:%d\n", pPreemptPriority_IRQn);
	printf("pSubPriority_IRQn:%d\n", pSubPriority_IRQn);

}
