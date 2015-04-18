/**
 ******************************************************************************
 * @file    Project/STM32F4xx_StdPeriph_Templates/main.h 
 * @author  MCD Application Team
 * @version V1.1.0
 * @date    18-January-2013
 * @brief   Header for main.c module
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
 *
 * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *        http://www.st.com/software_license_agreement_liberty_v2
 *
 * Unless required by applicable law or agreed to in writing, software 
 * distributed under the License is distributed on an "AS IS" BASIS, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ETHERNET_H
#define __ETHERNET_H

/* Includes ------------------------------------------------------------------*/
//#include "stm32f4xx.h"
#include "stm32f10x.h"
//#include "Ethernet_Config.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
//void TimingDelay_Decrement(void);
//void Delay(__IO uint32_t nTime);
void wizchip_select(void);
void wizchip_deselect(void);
uint8_t wizchip_read(void);
void wizchip_write(uint8_t wb);
int Ethernet_Init(void);
int Ethernet_main(void);

void Application(void);

#endif /* __ETHERNET_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
