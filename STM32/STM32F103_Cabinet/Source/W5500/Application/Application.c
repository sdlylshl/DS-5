/* Includes ------------------------------------------------------------------*/

#include <stdio.h>

#include "Application.h"

#include "../Ethernet_Config.h"
// SPI1
#include "./System/System_config.h"

//*************使用说明*****************
// 1.配置要使用的SPI
// 2.将SPI函数注册到此函数
//void wizchip_spi_init(void);
//void wizchip_select(void);
//void wizchip_deselect(void);
//void wizchip_write_byte(uint8_t wb);
//uint8_t wizchip_read_byte();
// 3.配置IO
//void wizchip_gpio_init();
//void wizchip_reset(void);
//uint8_t wizchip_isreday(void);
//*************************************

typedef struct _CONFIG_MSG {
	uint8_t Mac[6];
	uint8_t Lip[4];
	uint8_t Sub[4];
	uint8_t Gw[4];
	uint8_t DNS_Server_IP[4];
	uint8_t DHCP;
} CONFIG_MSG;

typedef struct _CONFIG_TYPE_DEF {
	uint16_t port;
	uint8_t destip[4];
} CHCONFIG_TYPE_DEF;

#define SOCK_CONFIG		2	// UDP
#define SOCK_DNS		2	// UDP
#define SOCK_DHCP		3	// UDP
#define MAX_BUF_SIZE		1460
#define KEEP_ALIVE_TIME	30	// 30sec
#define	MAX_SOCK_NUM		_WIZCHIP_SOCK_NUM_

//CONFIG_MSG Config_Msg;
//CHCONFIG_TYPE_DEF Chconfig_Type_Def;

//TX MEM SIZE- SOCKET 0-7:2KB
//RX MEM SIZE- SOCKET 0-7:2KB
uint8_t txsize[MAX_SOCK_NUM] = { 2, 2, 2, 2, 2, 2, 2, 2 };
uint8_t rxsize[MAX_SOCK_NUM] = { 2, 2, 2, 2, 2, 2, 2, 2 };

uint8_t ch_status[MAX_SOCK_NUM] = { 0, }; /** 0:close, 1:ready, 2:connected 3:init*/

// SRAM address range is 0x2000 0000 ~ 0x2000 4FFF (20KB)
//#define TX_RX_MAX_BUF_SIZE	2048
//#define TX_BUF	0x20004000
//#define RX_BUF	(TX_BUF+TX_RX_MAX_BUF_SIZE)
uint8_t TX_BUF[TX_RX_MAX_BUF_SIZE]; // TX Buffer for applications
uint8_t RX_BUF[TX_RX_MAX_BUF_SIZE] = { 10 }; // RX Buffer for applications
uint8_t RX_BUFF_SRV[8][256];

// Configuration Network Information of W5500
//uint8_t Enable_DHCP = OFF;
//uint8_t MAC[6] = {0x00, 0x08, 0xDC, 0x01, 0x02, 0x02};//MAC Address
//uint8_t IP[4] = {192, 168, 88, 34};//IP Address
//uint8_t GateWay[4] = {192, 168, 88, 1};//Gateway Address
//uint8_t SubNet[4] = {255, 255, 255, 0};//SubnetMask Address
//FOR TCP Client
//Configuration Network Information of TEST PC
uint8_t DEST_IP[4] = { 192, 168, 88, 12 }; //DST_IP Address
uint8_t DEST_IP1[4] = { 192, 168, 88, 91 }; //DST_IP Address
uint16_t DEST_PORT = 8081; //DST_IP port
uint16_t LISTION_PORT = 8081;

RCC_ClocksTypeDef RCC_ClockFreq;

uint8_t gDATABUF[DATA_BUF_SIZE];

wiz_PhyConf gWIZPHYCONF = { PHY_CONFBY_SW, PHY_MODE_AUTONEGO, PHY_SPEED_100,
		PHY_DUPLEX_FULL };

wiz_NetInfo gWIZNETINFO = { { 0x55, 0x08, 0xdc, 0x00, 0xab, 0xc5 }, ///< Source Mac Address
		{ 192, 168, 88, 16 }, ///< Source IP Address
		{ 255, 255, 255, 0 }, ///< Subnet Mask
		{ 192, 168, 88, 1 }, ///< Gateway IP Address
		{ 202, 102, 152, 3 }, ///< DNS server IP Address
		NETINFO_STATIC ///< 1 - Static, 2 - DHCP
		};

wiz_NetTimeout gWIZNETTIMEOUT = { 3, //< retry count
		6000 //< time unit 100us
		};

//外部接口函数
void wizchip_spi_init(void) {
	SPI1_Config();
}
void wizchip_select(void) {
	//WIZ_SCS(0);		//低电平有效
	SPI1_select();
}

void wizchip_deselect(void) {
	//WIZ_SCS(1);
	SPI1_deselect();
}

void wizchip_write_byte(uint8_t wb) {
	SPI1_SendByte(wb);
}

uint8_t wizchip_read_byte() {
	return SPI1_ReceiveByte();
}

void wizchip_gpio_init() {

	WIZ_GPIO_Configuration();
}
void wizchip_reset(void) {

	WIZ_nRST(0);
	Delay_ms(1); //大于500us 低电平有效
	WIZ_nRST(1);
	Delay_ms(10);
}

uint8_t wizchip_isreday(void) {

	return (!GPIO_ReadInputDataBit(WIZ_RDY_PORT, WIZ_RDY_PIN)); //低电平产生中断

}

void network_init(void) {
	uint8_t tmpstr[6];
	ctlnetwork(CN_SET_NETINFO, (void*) &gWIZNETINFO);
	ctlnetwork(CN_GET_NETINFO, (void*) &gWIZNETINFO);
	// Display Network Information
	ctlwizchip(CW_GET_ID, (void*) tmpstr);
	printf("\r\n=== %s NET CONF ===\r\n", (char*) tmpstr);
	printf("MAC: %02X:%02X:%02X:%02X:%02X:%02X\r\n", gWIZNETINFO.mac[0],
			gWIZNETINFO.mac[1], gWIZNETINFO.mac[2], gWIZNETINFO.mac[3],
			gWIZNETINFO.mac[4], gWIZNETINFO.mac[5]);
	printf("SIP: %d.%d.%d.%d\r\n", gWIZNETINFO.ip[0], gWIZNETINFO.ip[1],
			gWIZNETINFO.ip[2], gWIZNETINFO.ip[3]);
	printf("GAR: %d.%d.%d.%d\r\n", gWIZNETINFO.gw[0], gWIZNETINFO.gw[1],
			gWIZNETINFO.gw[2], gWIZNETINFO.gw[3]);
	printf("SUB: %d.%d.%d.%d\r\n", gWIZNETINFO.sn[0], gWIZNETINFO.sn[1],
			gWIZNETINFO.sn[2], gWIZNETINFO.sn[3]);
	printf("DNS: %d.%d.%d.%d\r\n", gWIZNETINFO.dns[0], gWIZNETINFO.dns[1],
			gWIZNETINFO.dns[2], gWIZNETINFO.dns[3]);
	printf("======================\r\n");

	//配置超时时间
	ctlnetwork(CN_SET_TIMEOUT, &gWIZNETTIMEOUT);
	//ctlnetwork (CN_SET_NETMODE

}

int Ethernet_Init(void) {
	uint8_t tmp = 0;
	//		uint32_t overtime = 0;
	uint8_t memsize[2][8] = { { 2, 2, 2, 2, 2, 2, 2, 2 }, { 2, 2, 2, 2, 2, 2, 2,
			2 } };

	//RCC_Config();

	wizchip_spi_init();
	WIZ_GPIO_Configuration();
	//WIZ_SPI_Configuration();
	//WIZ_DMA_Configuration();
	//Timer_Configuration();
	//WIZ_USART_Configuration(); /* SysTick end of count event each 10ms */

	//	RCC_GetClocksFreq(&RCC_Clocks);
	// SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);

	/* Add your application code here */

	/* Infinite loop */

	RCC_GetClocksFreq(&RCC_ClockFreq);

	printf("SYSCLK frequency: %d\r\n", RCC_ClockFreq.SYSCLK_Frequency);
	printf("PCLK1 frequency: %d\r\n", RCC_ClockFreq.PCLK1_Frequency);
	printf("PCLK2 frequency: %d\r\n", RCC_ClockFreq.PCLK2_Frequency);

	// reg_wizchip_cris_cbfunc(void(*cris_en)(void), void(*cris_ex)(void))
	//1.注册片选函数
	reg_wizchip_cs_cbfunc(wizchip_select, wizchip_deselect);

	//2.注册SPI读写函数
	reg_wizchip_spi_cbfunc(wizchip_read_byte, wizchip_write_byte);
	//2.
	//reg_wizchip_spiburst_cbfunc(wizchip_read_burst, wizchip_write_burst);
	//2.3 BUS读写
	//void reg_wizchip_bus_cbfunc(uint8_t(*bus_rb)(uint32_t addr), void (*bus_wb)(uint32_t addr, uint8_t wb));

	//3.复位W5500
	wizchip_reset();

	//4.等待就绪RDY
	while (wizchip_isreday()) {
		;
	}

	/* WIZCHIP SOCKET Buffer initialize */
	if (ctlwizchip(CW_INIT_WIZCHIP, (void*) memsize) == -1) {
		printf("WIZCHIP Initialized fail.\r\n");
		return (-1);
	}

	/* PHY link status check */
	//overtime =0;
	do {
		if (ctlwizchip(CW_GET_PHYLINK, (void*) &tmp) == -1)
			printf("Unknown PHY Link status.\r\n");
		if (TIM4_GetCurrentTime() > 0xFFFFFF)
			return -1;
	} while (tmp == PHY_LINK_OFF);

	printf("W5500 Demo on STM32F401RE Cortex M0\r\n");

	network_init();

	return 0;
}

int Ethernet_main(void) {

	int32_t ret = 0;
	uint8_t sn = 0;

	Ethernet_Init();

	while (1) {

#if 0
		if (GetIsTimeElapsed())
		{
			SetIsTimeElapsed(0);
			printf("Time Elapsed\r\n");
		}
#endif

#if 1
		if ((ret = loopback_tcps(sn, gDATABUF, 3000)) < 0) {
			printf("%d:loopback_tcps error:%d\r\n", sn, ret);
			break;
		}
		if ((ret = loopback_tcps(sn + 1, gDATABUF, 3000)) < 0) {
			printf("%d:loopback_tcps error:%d\r\n", sn, ret);
			break;
		}
#endif
#if 0
		if ((ret = rcvonly_tcps(sn, gDATABUF, 3000)) < 0)
		{
			printf("%d:loopback_tcps error:%d\r\n", sn, ret);
			break;
		}
#endif
	}
	return 0;
}

// 1.客户端主动连接服务器
// 2.服务器维护一个可用列表
// 3.可用列表下面维护一个可用设备表（由用户配置）
// 4.客户端只负责执行服务端的命令，无法检测设备好坏
// 5.客户端A.定时上报设备端口状态 B.设备执行后上报设备端口状态。
//


#ifdef CABINET_DEVICE
// 客户端动作
// 1.连接服务器
// 2.定时上报设备端口状态
// 3.接收服务器指令

void Application(void) {
	// 通过第1路socket创建连接
	//loopback_tcpc(0,DEST_IP,DEST_PORT);
	//loopback_tcpc(0, DEST_IP1, DEST_PORT);
	//loopback_tcpc(1, DEST_IP, DEST_PORT);
	loopback_tcps(2, RX_BUFF_SRV[2], LISTION_PORT);
	// 解析数据并执行相应命令
	ParseInstruction();

}
#else

// 服务端动作
//  1.监听传入连接
//  2.判断传入类型
//  3.创建连接表 ----总表对应
//  ID = CRC();
void Application(void) {
//	uint8_t i = 1;
	loopback_tcps(2, RX_BUFF_SRV[2], LISTION_PORT);
//	for (i = 2; i < 8; i++) {
//		loopback_tcps(i, RX_BUFF_SRV[i], LISTION_PORT);
//	}
//	loopback_tcpc(1, DEST_IP1, DEST_PORT);
	//loopback_tcpc(0, DEST_IP, DEST_PORT);
	ParseInstruction();
}

#endif // 
