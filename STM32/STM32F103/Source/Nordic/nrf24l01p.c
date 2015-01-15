#include <stdint.h>
#include "stdio.h"
#include "./nrf24l01p/hal_nrf.h"
//工作模式的配置
//Mode		 PWR_UP			 PRIM_RX(1) 		CE
//发送模式 		 1 			0 				1
//发送模式 		 1 			0 				≥10微秒的高电平
//接收模式 		 1 			1 				1

//待机模式Ⅱ	 1 			0 				1
//待机模式Ⅰ	 1			- 				0
//掉电模式		 0 			- 				-
//增强型 ShockBurst 模式的数据帧格式
//帧头| 地址 |控制域| 数据| CRC
//地址长度为3-5个字节，其内容为接收机的地址

// Global variables
uint8_t payload[RX_PLOAD_WIDTH];
uint8_t NRF__RX_BUF[RX_PLOAD_WIDTH] = { 0, 0, 0, 0 }; //接收数据缓存
uint8_t NRF__TX_BUF[TX_PLOAD_WIDTH] = { 0x18, 0x65, 2, 3 }; //发射数据缓存
//moren
uint8_t NRF_MASTER_ADDRESS[TX_ADR_WIDTH] = { 0x34, 0x43, 0x10, 0x10, 0x01 }; // 定义一个静态发送地址
uint8_t NRF_DEVICE_ADDRESS[RX_ADR_WIDTH] = { 0x34, 0x43, 0x10, 0x10, 0x01 };
_nrf_chip_t nrf_chip_master;
_nrf_chip_t nrf_chip_recv;
_nrf_chip_t nrf_chip_send;
_nrf_chip_t nrf_chip_device;
uint8_t radio_busy = 0;
uint8_t send_erro = 0;

#define MASTER

uint8_t nrf_test(_nrf_chip_t *nrf_chip) {
	uint8_t reg;
	if(nrf_chip == &nrf_chip_send)
	printf("nrf_chip_send");
	else
		printf("nrf_chip_recv");

	reg = hal_nrf_read_reg(nrf_chip, CONFIG); 	printf("\n CONFIG :0x%02x\n", reg);
	reg = hal_nrf_read_reg(nrf_chip, EN_AA);		printf(" EN_AA :0x%02x\n", reg);
	reg = hal_nrf_read_reg(nrf_chip, EN_RXADDR);printf(" EN_RXADDR :0x%02x\n", reg);
	reg = hal_nrf_read_reg(nrf_chip, SETUP_AW); printf(" SETUP_AW :0x%02x\n", reg);
	reg = hal_nrf_read_reg(nrf_chip, SETUP_RETR);printf(" SETUP_RETR :0x%02x\n", reg);
	reg = hal_nrf_read_reg(nrf_chip, RF_CH);printf(" RF_CH :0x%02x\n", reg);
	reg = hal_nrf_read_reg(nrf_chip, RF_SETUP);printf(" RF_SETUP :0x%02x\n", reg);
	reg = hal_nrf_read_reg(nrf_chip, STATUS);printf(" STATUS :0x%02x\n", reg);
	reg = hal_nrf_read_reg(nrf_chip, OBSERVE_TX);printf(" OBSERVE_TX :0x%02x\n", reg);
	reg = hal_nrf_read_reg(nrf_chip, CD);printf(" CD :0x%02x\n", reg);
	reg = hal_nrf_read_reg(nrf_chip, RX_ADDR_P0);printf(" RX_ADDR_P0 :0x%02x\n", reg);
	reg = hal_nrf_read_reg(nrf_chip, RX_ADDR_P1);printf(" RX_ADDR_P0 :0x%02x\n", reg);
	reg = hal_nrf_read_reg(nrf_chip, RX_ADDR_P2);printf(" RX_ADDR_P2 :0x%02x\n", reg);
	reg = hal_nrf_read_reg(nrf_chip, RX_ADDR_P3);printf(" RX_ADDR_P0 :0x%02x\n", reg);
	reg = hal_nrf_read_reg(nrf_chip, RX_ADDR_P4);printf(" RX_ADDR_P0 :0x%02x\n", reg);
	reg = hal_nrf_read_reg(nrf_chip, RX_ADDR_P5);printf(" RX_ADDR_P0 :0x%02x\n", reg);
	reg = hal_nrf_read_reg(nrf_chip, TX_ADDR);printf(" TX_ADDR :0x%02x\n", reg);
	reg = hal_nrf_read_reg(nrf_chip, RX_PW_P0);printf(" RX_PW_P0 :0x%02x\n", reg);
	reg = hal_nrf_read_reg(nrf_chip, RX_PW_P1);printf(" RX_PW_P0 :0x%02x\n", reg);
	reg = hal_nrf_read_reg(nrf_chip, RX_PW_P2);printf(" RX_PW_P0 :0x%02x\n", reg);
	reg = hal_nrf_read_reg(nrf_chip, RX_PW_P3);printf(" RX_PW_P0 :0x%02x\n", reg);
	reg = hal_nrf_read_reg(nrf_chip, RX_PW_P4);printf(" RX_PW_P0 :0x%02x\n", reg);
	reg = hal_nrf_read_reg(nrf_chip, RX_PW_P5);printf(" RX_PW_P0 :0x%02x\n", reg);
	reg = hal_nrf_read_reg(nrf_chip, FIFO_STATUS);printf(" FIFO_STATUS :0x%02x\n", reg);
	reg = hal_nrf_read_reg(nrf_chip, DYNPD);printf(" DYNPD :0x%02x\n", reg);
	reg = hal_nrf_read_reg(nrf_chip, FEATURE);printf(" FEATURE :0x%02x\n", reg);
	return reg;
}
static uint8_t nrf_check(_nrf_chip_t *nrf_chip) {
	uint8_t buf[5];
	uint8_t i;
	hal_nrf_get_address(nrf_chip, HAL_NRF_TX, buf);
	printf(" HAL_NRF_TX :");
	for (i = 0; i < 5; i++) {
	 printf(" 0x%02x",buf[i]);
	}
	printf("\n");
	hal_nrf_get_address(nrf_chip, HAL_NRF_PIPE0, buf);
	hal_nrf_get_address(nrf_chip, HAL_NRF_PIPE1, buf);
	hal_nrf_get_address(nrf_chip, HAL_NRF_PIPE2, buf);
	hal_nrf_get_address(nrf_chip, HAL_NRF_PIPE3, buf);
	hal_nrf_get_address(nrf_chip, HAL_NRF_PIPE4, buf);
	hal_nrf_get_address(nrf_chip, HAL_NRF_PIPE5, buf);

	/*写入5个字节的地址.	*/
	//SPI_NRF_WriteBuf(NRF_WRITE_REG+TX_ADDR,buf,5);
	hal_nrf_set_address(nrf_chip, HAL_NRF_TX, NRF_MASTER_ADDRESS);
	/*读出写入的地址 */
	//SPI_NRF_ReadBuf(TX_ADDR,buf1,5);
	hal_nrf_get_address(nrf_chip, HAL_NRF_TX, buf);	
	printf(" HAL_NRF_TX :");
	for (i = 0; i < 5; i++) {
	 printf(" 0x%02x",buf[i]);
	}
	printf("\n");
	/*比较*/
	for (i = 0; i < 5; i++) {
		if (buf[i] != NRF_MASTER_ADDRESS[i])
			break;
	}

	if (i == 5)
		return SUCCESS; //MCU与NRF成功连接
	else
		return ERROR; //MCU与NRF不正常连接
}

static void nrf_config(_nrf_chip_t *nrf_chip) {
	//参数配置必须在POWER DOWN 模式下
	//进入空闲模式
	//系统设置
	//RF_SETUP设置传输功率,默认0db
	hal_nrf_set_output_power(nrf_chip, HAL_NRF_0DBM);
	//RF_SETUP设置传输速率,默认2Mbps
	hal_nrf_set_datarate(nrf_chip, HAL_NRF_2MBPS);

	//RF_CH设置传输通道 默认:2
	hal_nrf_set_rf_channel(nrf_chip, 40);
	//CONFIG设置中断IRQ
	hal_nrf_set_irq_mode(nrf_chip, HAL_NRF_MASK_MAX_RT, true);
	hal_nrf_set_irq_mode(nrf_chip, HAL_NRF_MASK_TX_DS, true);
	hal_nrf_set_irq_mode(nrf_chip, HAL_NRF_MASK_RX_DR, true);
	//hal_nrf_set_operation_mode(nrf_chip, HAL_NRF_PRX);

	hal_nrf_flush_rx(nrf_chip);
	hal_nrf_flush_tx(nrf_chip);
	//SETUP_AW设置地址宽度，默认5
	hal_nrf_set_address_width(nrf_chip, HAL_NRF_AW_5BYTES);
	//RX_PW_P0设置接收数据宽度,默认0 nouse
	hal_nrf_set_rx_payload_width(nrf_chip, HAL_NRF_PIPE0, RX_PLOAD_WIDTH);
	//RX_ADDR_P0设置发送节点地址,默认0xE7E7E7E7E7
	hal_nrf_set_address(nrf_chip, HAL_NRF_TX, NRF_MASTER_ADDRESS);
	//RX_ADDR_P0设置接收节点0地址,默认0xE7E7E7E7E7
	hal_nrf_set_address(nrf_chip, HAL_NRF_PIPE0, NRF_MASTER_ADDRESS);
	//RX_ADDR_P0设置接收节点1地址,默认0xC2C2C2C2C2
	//hal_nrf_set_address(nrf_chip, HAL_NRF_PIPE1,NRF_DEVICE_ADDRESS);

	//SETUP_RETR设置重发次数,重发间隔,默认3次,250us
	hal_nrf_set_auto_retr(nrf_chip, 10, 500);
	//EN_AA,EN_RXADDR开启通道0的自动应答,开启时强制打开CRC校验,默认开启
	hal_nrf_open_pipe(nrf_chip, HAL_NRF_PIPE0, EN_AA);
	//CONFIG打开CRC16校验,默认8位
	hal_nrf_set_crc_mode(nrf_chip, HAL_NRF_CRC_16BIT);
	//CONFIG默认POWER DOWN
	hal_nrf_set_power_mode(nrf_chip, HAL_NRF_PWR_UP);

	//CE_HIGH();
}

void nrf_rx_mode(_nrf_chip_t *nrf_chip) {
	//hal_nrf_flush_rx();
	//hal_nrf_set_irq_mode(HAL_NRF_MASK_MAX_RT,false);
	//hal_nrf_set_irq_mode(HAL_NRF_MASK_TX_DS,false);
	//hal_nrf_set_irq_mode(HAL_NRF_MASK_RX_DR,true);
	hal_nrf_set_operation_mode(nrf_chip, HAL_NRF_PRX);
	hal_nrf_enable_radio(nrf_chip);

}
void nrf_tx_mode(_nrf_chip_t *nrf_chip) {
	//hal_nrf_flush_rx();
	//hal_nrf_set_irq_mode(HAL_NRF_MASK_MAX_RT,false);
	//hal_nrf_set_irq_mode(HAL_NRF_MASK_TX_DS,false);
	//hal_nrf_set_irq_mode(HAL_NRF_MASK_RX_DR,true);
	hal_nrf_set_operation_mode(nrf_chip, HAL_NRF_PTX);
	//hal_nrf_enable_radio(nrf_chip);

}
uint8_t nrf_tx_dat(_nrf_chip_t *nrf_chip, const uint8_t * txdat) {
	uint32_t nrf_time=1000;
	nrf_chip->radio_busy = 0;
	printf("\nsend\n");
	//测试时必须要加，否则发送失败
	//Delay_us(100);
	nrf_chip->CE_LOW();
	//hal_nrf_set_irq_mode(HAL_NRF_MASK_MAX_RT,true);
	//hal_nrf_set_irq_mode(HAL_NRF_MASK_TX_DS,true);
	//hal_nrf_set_irq_mode(HAL_NRF_MASK_RX_DR,false);
	hal_nrf_set_operation_mode(nrf_chip, HAL_NRF_PTX);
	hal_nrf_write_tx_payload(nrf_chip, txdat, TX_PLOAD_WIDTH);
	hal_nrf_enable_radio(nrf_chip);

	//Delay_us(15);
	//CE_PULSE();

	//nrf_time = TIM4_GetCurrentTime();
	//2.等待发送完成
#ifdef NVIC_SPI2_IRQ
	while (!(nrf_chip->radio_busy)) {
		//if (TIM4_GetDistanceTime(nrf_time) > 10) {
			if(!(nrf_time--)){
				printf("busy%x",nrf_chip_send.radio_busy);
			//此处必须加延时不知为啥。。
			printf("send timeout !\n");
			send_erro = 1;
			hal_nrf_flush_tx(nrf_chip);
			//nrf_test(&nrf_chip_recv);				
			//nrf_test(&nrf_chip_send);
			break;
		}
	}

#else
	while(NRF_Read_IRQ());

	radio_busy = hal_nrf_get_clear_irq_flags(nrf_chip);

#endif

	if (nrf_chip->radio_busy & TX_DS) {
		//printf("send data ok !\n");
		printf("\n data send ok !");

	} else if (radio_busy & MAX_RT) {
		printf("\n send max times");
		hal_nrf_flush_tx(nrf_chip);
	}

	//3.切换到接收模式
	//nrf_rx_mode(nrf_chip);
	return nrf_chip->radio_busy;
}
//主机端
/**
 1.设备1接收模式 --信道64
 2.
 */
void nrf_master() {
	uint8_t status = 0;
	uint32_t nrf_time;
	uint8_t i;
//	printf("nrf_spi1\n");
	nrfchip_init(&nrf_chip_send,SPI_1);
	nrfchip_init(&nrf_chip_recv,SPI_2);

	nrf_test(&nrf_chip_send);
	nrf_test(&nrf_chip_recv);

	nrf_config(&nrf_chip_send);	
	nrf_config(&nrf_chip_recv);
	
	hal_nrf_set_operation_mode(&nrf_chip_send, HAL_NRF_PTX);
	hal_nrf_set_operation_mode(&nrf_chip_recv, HAL_NRF_PRX);
	
	nrf_test(&nrf_chip_send);
	nrf_test(&nrf_chip_recv);
	//nrf_EXIT_Config();
	/*检测NRF模块与MCU的连接*/

		status = nrf_check(&nrf_chip_recv);

	//
	/*判断连接状态*/
	if (status == SUCCESS)
		printf("\r\n   nrfchip_spi2    nrf2401 connect ok !	\r\n");
	else
		printf("\r\n   nrfchip_spi2   nrf2401 connect erro ! \r\n");
	printf(" master mode \n");

	//nrfchip_init(&nrf_chip_send,SPI_2);
	//nrf_config(&nrf_chip_send);

	//nrfchip_spi1();
	nrf_rx_mode(&nrf_chip_recv);
	//nrf_tx_mode(&nrf_chip_send);
	Delay_ms(100);
	printf(" master mode start \n");	
	
	nrf_tx_dat(&nrf_chip_send, NRF__TX_BUF);
	while (1) {
		//Delay_ms(100);
		//2.收到数据
		//发送失败|收到数据 重发
		if ((nrf_chip_recv.radio_busy) & RX_DR) {
				nrf_chip_recv.radio_busy = 0;
			
			for (i = 0; i < 4; i++) {
				printf(" %d", NRF__RX_BUF[i]);
			}
			//Delay_ms(10);
			//nrf_tx_dat(&nrf_chip_send, NRF__TX_BUF);
			//printf("\r\n master recv dat :");

			//hal_nrf_enable_radio(nrf_chip);
			//printf("\r\n 主机端 接收到 从机端 发送的数据为");

		}
			if (TIM4_GetDistanceTime(nrf_time) > 1000) {
			nrf_time = TIM4_GetCurrentTime();
				nrf_tx_dat(&nrf_chip_send, NRF__TX_BUF);
				}
		
		if(send_erro){
			while(hal_nrf_get_carrier_detect(&nrf_chip_send));
			if(!hal_nrf_get_carrier_detect(&nrf_chip_send)){
			nrf_tx_dat(&nrf_chip_send, NRF__TX_BUF);
			//重发一次
			send_erro =0;
			}
		}

	}
}
void nrf_device() {
		uint8_t status = 0;
	uint32_t nrf_time;
	uint8_t i;
//	uint8_t status = 0;
	nrfchip_init(&nrf_chip_device, SPI_2);
	nrf_config(&nrf_chip_device);

	printf(" device mode \n");
	//nrf_rx_mode(&nrf_chip_device);
	nrf_tx_mode(&nrf_chip_device);
	
		while (!status) {
		status = nrf_check(&nrf_chip_device);
	}
	//
	/*判断连接状态*/
	if (status == SUCCESS)
		printf("\r\n   nrfchip_spi2    nrf2401 connect ok !	\r\n");
	else
		printf("\r\n   nrfchip_spi2   nrf2401 connect erro ! \r\n");
	printf(" master mode \n");
	while (1) {
		//2.收到数据
		if (nrf_chip_device.radio_busy & RX_DR) {
				nrf_chip_device.radio_busy = 0;

			printf("\r\n device recv dat :");
			for (i = 0; i < 4; i++) {
				printf(" %d", NRF__RX_BUF[i]);
			}

		}

		//载波检测,接收模式下有效
		// if(hal_nrf_get_carrier_detect()) {
		// cdn++;
		//printf("检测到载波信号 %x\n",cdn);
		//}
		if (TIM4_GetDistanceTime(nrf_time) > 1000) {
			nrf_time = TIM4_GetCurrentTime();
			Delay_ms(10);
			//printf("\r\n 主机端 进入发送模式\r\n");
			 nrf_tx_dat(&nrf_chip_device, NRF__TX_BUF);
			 nrf_rx_mode(&nrf_chip_device);
			//printf("\r\n 主机端 进入发送模式 %x\r\n",status);
		}

	}
}
//*******************************************************
void nrf_main() {

//	//
#ifdef MASTER
	//nrf_test();
	nrf_master();
#else
	nrf_device();
#endif

}
void NRF_ISR_MASTER_RECV() {
	uint8_t irq_flags = 0;
	_nrf_chip_t *nrf_chip = &nrf_chip_recv;
	// Read and clear IRQ flags from radio
	irq_flags = hal_nrf_get_clear_irq_flags(nrf_chip);
	if (irq_flags & RX_DR) {
		while (!hal_nrf_rx_fifo_empty(nrf_chip)) {
			//返回reg<<8+lenth
			hal_nrf_read_rx_payload(nrf_chip, NRF__RX_BUF);
		}
		hal_nrf_enable_radio(nrf_chip); //不要退出接收模式
		//数据接收完再将标志位置位
		nrf_chip->radio_busy = RX_DR;
	}
}

// Radio interrupt
void NRF_ISR_MASTER_SEND() {
	uint8_t irq_flags = 0;
	_nrf_chip_t *nrf_chip = &nrf_chip_send;
	// Read and clear IRQ flags from radio
	irq_flags = hal_nrf_get_clear_irq_flags(nrf_chip);

	// radio_busy = false;
	switch (irq_flags) {

	// Transmission success
	case (1 << (uint8_t) HAL_NRF_TX_DS):
		
			nrf_chip->radio_busy = TX_DS;
		
		// Data has been sent
		break;
		// Transmission failed (maximum re-transmits)
	case (1 << (uint8_t) HAL_NRF_MAX_RT):
	
			// When a MAX_RT interrupt occurs the TX payload will not be removed from the TX FIFO.
			// If the packet is to be discarded this must be done manually by flushing the TX FIFO.
			// Alternatively, CE_PULSE() can be called re-starting transmission of the payload.
			// (Will only be possible after the radio irq flags are cleared)
			//hal_nrf_flush_tx();
			nrf_chip->radio_busy = MAX_RT;
	
		break;

	default:
		break;
	}
}

void NRF_ISR_DEVICE() {
	uint8_t irq_flags = 0;

	// Read and clear IRQ flags from radio
	irq_flags = hal_nrf_get_clear_irq_flags(&nrf_chip_device);

	// radio_busy = false;
	switch (irq_flags) {

	// Transmission success
	case (1 << (uint8_t) HAL_NRF_TX_DS):
			nrf_chip_device.radio_busy = TX_DS;
		// Data has been sent
		break;
		// Transmission failed (maximum re-transmits)
	case (1 << (uint8_t) HAL_NRF_MAX_RT):
			// When a MAX_RT interrupt occurs the TX payload will not be removed from the TX FIFO.
			// If the packet is to be discarded this must be done manually by flushing the TX FIFO.
			// Alternatively, CE_PULSE() can be called re-starting transmission of the payload.
			// (Will only be possible after the radio irq flags are cleared)
			//hal_nrf_flush_tx();
			nrf_chip_device.radio_busy = MAX_RT;
		break;
	case (1 << (uint8_t) HAL_NRF_RX_DR):
		while (!hal_nrf_rx_fifo_empty(&nrf_chip_device)) {
			//返回reg<<8+lenth
			hal_nrf_read_rx_payload(&nrf_chip_device, NRF__RX_BUF);
		}
		hal_nrf_enable_radio(&nrf_chip_device); //不要退出接收模式
		//数据接收完再将标志位置位
		nrf_chip_device.radio_busy = RX_DR;
		break;
	default:
		break;
	}
}
