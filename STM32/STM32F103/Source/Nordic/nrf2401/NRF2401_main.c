/******************** (C) COPYRIGHT 2012 WildFire Team **************************
 * 文件名  ：main.c
 * 描述    ：NRF24L01无线w传输测试实验
 * 实验平台：野火STM32开发板
 * 库版本  ：ST3.5.0
 *
 * 作者    ：wildfire team
 * 论坛    ：http://www.amobbs.com/forum-1008-1.html
 * 淘宝    ：http://firestm32.taobao.com
 **********************************************************************************/
#include <stdio.h>
#include <stdbool.h>
#include "NRF2401.h"
//usart1
#include "./System/Usart/usart1.h"
#include "../common/hal_nrf_hw.h"
uint8_t status; //用于判断接收/发送状态

int i = 0;
void NRF_master_main(void){
	uint32_t nrf_time;
	while(true){

		if (GetDistanceTime(nrf_time) > 1000) {
			nrf_time = GetCurrentTime();

			nrf_device_tx_mode();
			printf("Send data: ");
			i = 100; //重发次数
			do {

				status = NRF_Tx_Dat(NRF__TX_BUF);
				//发送成功
				if (status & TX_DS) {
					//NRF_DEVICE_RX_MODE();
					break;
				}
				printf(" %x ", status);
			} while ((i--) && 1); //(status == MAX_RT);
			nrf_device_rx_mode();
		}
		status = nrf_rx_dat(NRF__RX_BUF);

		/*判断接收状态*/
		if (status == RX_DR) {
			printf("\r\n Device Data is : ");
			for (i = 0; i < 4; i++) {
				//printf("\r\n 从机端 接收到 主机端 发送的数据为：%d \r\n",NRF__RX_BUF[i]);

				printf(" %d ", NRF__RX_BUF[i]);
				/*把接收的数据+1后发送给主机*/
				//NRF__RX_BUF[i] += 1;
				NRF__TX_BUF[i] = NRF__RX_BUF[i];
			}
		}


	}

}
void NRF_device_main(void) {
	uint32_t nrf_time;
	nrf_device_rx_mode();
	while (true) {

		if (GetDistanceTime(nrf_time) > 1000) {
			nrf_time = GetCurrentTime();

			nrf_device_tx_mode();
			printf("Send data: ");
			i = 100; //重发次数
			do {

				status = NRF_Tx_Dat(NRF__TX_BUF);
				//发送成功
				if (status & TX_DS) {
					//NRF_DEVICE_RX_MODE();
					break;
				}
				printf(" %x ", status);
			} while ((i--) && 1); //(status == MAX_RT);
			nrf_device_rx_mode();
		}
		//NRF_DEVICE_RX_MODE();
		if(nrf_rx_dat(NRF__RX_BUF) == RX_DR){
			if (status == RX_DR) {
				printf("\r\n Device Data is : ");
				for (i = 0; i < 4; i++) {
					//printf("\r\n 从机端 接收到 主机端 发送的数据为：%d \r\n",NRF__RX_BUF[i]);

					printf(" %d ", NRF__RX_BUF[i]);
					/*把接收的数据+1后发送给主机*/
					NRF__RX_BUF[i] += 1;
					NRF__TX_BUF[i] = NRF__RX_BUF[i];
				}
			}

		}
		nrf_device_rx_mode();
		/*等待接收数据*/
		status = nrf_rx_dat(NRF__RX_BUF);

		/*判断接收状态*/
		if (status == RX_DR) {
			printf("\r\n Device Data is : ");
			for (i = 0; i < 4; i++) {
				//printf("\r\n 从机端 接收到 主机端 发送的数据为：%d \r\n",NRF__RX_BUF[i]);

				printf(" %d ", NRF__RX_BUF[i]);
				/*把接收的数据+1后发送给主机*/
				NRF__RX_BUF[i] += 1;
				NRF__TX_BUF[i] = NRF__RX_BUF[i];
			}
		}
	}
}

extern void nrf_common_config(void);

void nrf_main0() {

	 nrfchip_init0();
	/*检测NRF模块与MCU的连接*/
	status = nrf_check();
	if (status == SUCCESS)
		printf("\r\n       nrf2401 connect ok !	\r\n");
	//		 printf("\r\n      NRF与MCU连接成功\r\n");
	else
		printf("\r\n       nrf2401 connect erro ! \r\n");
	//	     printf("\r\n   正在检测NRF与MCU是否正常连接。。。\r\n");

	nrf_common_config();
	printf("nrf_main0 init\n");
NRF_master_main();
//	NRF_device_main();
	while (1) {
		//nrf_recv();
		//nrf_send();
	}
}
/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/

