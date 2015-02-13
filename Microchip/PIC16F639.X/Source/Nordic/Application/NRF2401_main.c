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
#include "stdio.h"
#include "SPI_NRF.h"
//usart1
//#include "./System/System_config.h"

uint8_t status; //用于判断接收/发送状态
//u8 NRF__TX_BUF[4] = { 0, 1, 2, 3 }; //发送缓冲
//u8 NRF__RX_BUF[4]; //接收缓冲

extern uint8_t NRF__RX_BUF[]; //接收数据缓存
extern uint8_t NRF__TX_BUF[]; //发射数据缓存
int i = 0;

extern uint32_t (*GetCurrentTime)(void);
extern uint32_t (*GetDistanceTime)(uint32_t );

void NRF_master_main(void){


}
void NRF_device_main(void) {
	uint32_t nrf_time;
        uint8_t recv;
	NRF_DEVICE_RX_MODE();
	while (1) {

		if ((GetDistanceTime(nrf_time) > 1000)||recv) {
                    recv =0;
			nrf_time = GetCurrentTime();

			NRF_DEVICE_TX_MODE();
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
			NRF_DEVICE_RX_MODE();
		}
		//NRF_DEVICE_RX_MODE();
		if(NRF_Rx_Dat(NRF__RX_BUF) == RX_DR){
			if (status == RX_DR) {
                            recv =1;
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
		NRF_DEVICE_RX_MODE();
		/*等待接收数据*/
		status = NRF_Rx_Dat(NRF__RX_BUF);

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

extern void NRF_COMN_CONFIG(void);

void nrf_main0() {
	/*检测NRF模块与MCU的连接*/
    PORTCbits.RC0 = 1;
    PORTCbits.RC4 = 1;
	status = NRF_Check();
	if (status == true){
		printf("\r\n       nrf2401 connect ok !	\r\n");
	//		 printf("\r\n      NRF与MCU连接成功\r\n");
               PORTCbits.RC0 = 0;//!PORTCbits.RC0;
        }
	else
		printf("\r\n       nrf2401 connect erro ! \r\n");
	//	     printf("\r\n   正在检测NRF与MCU是否正常连接。。。\r\n");

	SPI_NRF_Init();
	NRF_COMN_CONFIG();
	printf("nrf_main0 init\n");
        //PORTCbits.RC4 = !PORTCbits.RC4;
//NRF_master_main();
	NRF_device_main();
	while (1) {
		//nrf_recv();
		//nrf_send();
	}
}
/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/

