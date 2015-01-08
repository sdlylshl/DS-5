#include <stdint.h>
#include "stdio.h"
#include "./nrf24l01p/hal_nrf.h"
//����ģʽ������ 
//Mode		 PWR_UP			 PRIM_RX(1) 		CE 
//����ģʽ 		 1 			0 				1 
//����ģʽ 		 1 			0 				��10΢��ĸߵ�ƽ 
//����ģʽ 		 1 			1 				1 
	
//����ģʽ��	 1 			0 				1 
//����ģʽ��	 1			- 				0 
//����ģʽ		 0 			- 				- 
//��ǿ�� ShockBurst ģʽ������֡��ʽ 
//֡ͷ| ��ַ |������| ����| CRC 
//��ַ����Ϊ3-5���ֽڣ�������Ϊ���ջ��ĵ�ַ


// Global variables
uint8_t payload[RX_PLOAD_WIDTH];
uint8_t NRF__RX_BUF[RX_PLOAD_WIDTH] = {0,0,0,0};		//�������ݻ���
uint8_t NRF__TX_BUF[TX_PLOAD_WIDTH] = {0,0x65,2,3};		//�������ݻ���
//moren
uint8_t NRF_MASTER_ADDRESS[TX_ADR_WIDTH] = {0x34,0x43,0x10,0x10,0x01};	// ����һ����̬���͵�ַ
uint8_t NRF_DEVICE_ADDRESS[RX_ADR_WIDTH] = {0x34,0x43,0x10,0x10,0x01};

uint8_t radio_busy =0;

static void nrf_init(){
	//�˿�&SPI��ʼ��
	SPI2_Init();	
	//�ͷ�SPI����
	CSN_HIGH(); 
}
uint8_t	nrf_test(){
	uint8_t reg;
		
		reg = hal_nrf_read_reg(CONFIG);
		reg = hal_nrf_read_reg(EN_AA			);
		reg = hal_nrf_read_reg(EN_RXADDR	);
		reg = hal_nrf_read_reg(SETUP_AW	 );
		reg = hal_nrf_read_reg(SETUP_RETR );
		reg = hal_nrf_read_reg(RF_CH			);
		reg = hal_nrf_read_reg(RF_SETUP	 );
		reg = hal_nrf_read_reg(STATUS		 );
		reg = hal_nrf_read_reg(OBSERVE_TX );
		reg = hal_nrf_read_reg(CD				 );
		reg = hal_nrf_read_reg(RX_ADDR_P0 );
		reg = hal_nrf_read_reg(RX_ADDR_P1 );
		reg = hal_nrf_read_reg(RX_ADDR_P2 );
		reg = hal_nrf_read_reg(RX_ADDR_P3 );
		reg = hal_nrf_read_reg(RX_ADDR_P4 );
		reg = hal_nrf_read_reg(RX_ADDR_P5 );
		reg = hal_nrf_read_reg(TX_ADDR		);
		reg = hal_nrf_read_reg(RX_PW_P0	 );
		reg = hal_nrf_read_reg(RX_PW_P1	 );
		reg = hal_nrf_read_reg(RX_PW_P2	 );
		reg = hal_nrf_read_reg(RX_PW_P3	 );
		reg = hal_nrf_read_reg(RX_PW_P4	 );
		reg = hal_nrf_read_reg(RX_PW_P5	 );
		reg = hal_nrf_read_reg(FIFO_STATUS);
		reg = hal_nrf_read_reg(DYNPD			);
		reg = hal_nrf_read_reg(FEATURE		);
		return reg;
}
static uint8_t nrf_check(void)
{
	uint8_t buf[5]={0x12,0x22,0x32,0x42,0x52};
	uint8_t buf1[5];
	uint8_t i; 
		hal_nrf_get_address(HAL_NRF_TX,buf1);
		hal_nrf_get_address(HAL_NRF_PIPE0,buf1);
		hal_nrf_get_address(HAL_NRF_PIPE1,buf1);
		hal_nrf_get_address(HAL_NRF_PIPE2,buf1);
		hal_nrf_get_address(HAL_NRF_PIPE3,buf1);
		hal_nrf_get_address(HAL_NRF_PIPE4,buf1);
		hal_nrf_get_address(HAL_NRF_PIPE5,buf1);	 
	
	/*д��5���ֽڵĵ�ַ.	*/	
	//SPI_NRF_WriteBuf(NRF_WRITE_REG+TX_ADDR,buf,5);
		hal_nrf_set_address(HAL_NRF_TX,buf);
	/*����д��ĵ�ַ */
	//SPI_NRF_ReadBuf(TX_ADDR,buf1,5); 
		hal_nrf_get_address(HAL_NRF_TX,buf1);
	/*�Ƚ�*/							 
	for(i=0;i<5;i++)
	{
		if(buf1[i]!=buf[i])
		break;
	} 
				 
	if(i==5)
		return SUCCESS ;				//MCU��NRF�ɹ����� 
	else
		return ERROR ;				//MCU��NRF����������
}

static void nrf_config(){
		//�������ñ�����POWER DOWN ģʽ��
		//�������ģʽ
		CE_LOW();	
		//ϵͳ����
		//RF_SETUP���ô��书��,Ĭ��0db
		hal_nrf_set_output_power(HAL_NRF_0DBM);
		//RF_SETUP���ô�������,Ĭ��2Mbps
		hal_nrf_set_datarate(HAL_NRF_2MBPS);

		//RF_CH���ô���ͨ�� Ĭ��:2
		hal_nrf_set_rf_channel(40);
		//CONFIG�����ж�IRQ 
		hal_nrf_set_irq_mode(HAL_NRF_MASK_MAX_RT,true);
		hal_nrf_set_irq_mode(HAL_NRF_MASK_TX_DS,true);
		hal_nrf_set_irq_mode(HAL_NRF_MASK_RX_DR,true);

		//SETUP_AW���õ�ַ��ȣ�Ĭ��5
		//hal_nrf_set_address_width(HAL_NRF_AW_5BYTES);
		//RX_PW_P0���ý������ݿ��,Ĭ��0 nouse
		hal_nrf_set_rx_payload_width(HAL_NRF_PIPE0,RX_PLOAD_WIDTH);
		//RX_ADDR_P0���÷��ͽڵ��ַ,Ĭ��0xE7E7E7E7E7
		hal_nrf_set_address(HAL_NRF_TX,NRF_MASTER_ADDRESS);
		//RX_ADDR_P0���ý��սڵ�0��ַ,Ĭ��0xE7E7E7E7E7
		hal_nrf_set_address(HAL_NRF_PIPE0,NRF_MASTER_ADDRESS);
		//RX_ADDR_P0���ý��սڵ�1��ַ,Ĭ��0xC2C2C2C2C2
		//hal_nrf_set_address(HAL_NRF_PIPE1,NRF_DEVICE_ADDRESS);

		//SETUP_RETR�����ط�����,�ط����,Ĭ��3��,250us
		hal_nrf_set_auto_retr(10,500);
		//EN_AA,EN_RXADDR����ͨ��0���Զ�Ӧ��,����ʱǿ�ƴ�CRCУ��,Ĭ�Ͽ���
		hal_nrf_open_pipe(HAL_NRF_PIPE0,EN_AA);
			//CONFIG��CRC16У��,Ĭ��8λ
		hal_nrf_set_crc_mode(HAL_NRF_CRC_16BIT);
		//CONFIGĬ��POWER DOWN
		hal_nrf_set_power_mode(HAL_NRF_PWR_UP);
		
}

void nrf_rx_mode(){
		CE_LOW();			
		hal_nrf_flush_rx();
		hal_nrf_set_irq_mode(HAL_NRF_MASK_MAX_RT,false);
		hal_nrf_set_irq_mode(HAL_NRF_MASK_TX_DS,false);
		hal_nrf_set_irq_mode(HAL_NRF_MASK_RX_DR,true);	
		hal_nrf_set_operation_mode(HAL_NRF_PRX);
		CE_HIGH();
		
}
uint8_t nrf_tx_dat(){
	
		radio_busy = 0;
		//1. �л�����ģʽ
		CE_LOW();
	
		hal_nrf_set_irq_mode(HAL_NRF_MASK_MAX_RT,true);
		hal_nrf_set_irq_mode(HAL_NRF_MASK_TX_DS,true);
		hal_nrf_set_irq_mode(HAL_NRF_MASK_RX_DR,false);
		hal_nrf_set_operation_mode(HAL_NRF_PTX);
		hal_nrf_write_tx_payload(NRF__TX_BUF,TX_PLOAD_WIDTH);	
	
		CE_HIGH();
	NRF__TX_BUF[0]++;
	Delay_us(15);
		CE_PULSE();
	
		//2.�ȴ��������
#ifdef NVIC_SPI2_IRQ
		while(!radio_busy);

#else
		while(NRF_Read_IRQ());
	
		radio_busy = hal_nrf_get_clear_irq_flags();	

#endif 
		if(radio_busy & TX_DS){
			//printf("send data ok !\n");
			//printf("���ݷ��ͳɹ� !\n");
		}else
		if(radio_busy & MAX_RT){
			printf("��������ʹ���\n");
			hal_nrf_flush_tx();
		}
		
		//3.�л�������ģʽ
		nrf_rx_mode();
		
		return radio_busy;	
}
void nrf_recv_ack(){
	uint8_t i;
nrf_rx_mode();
	while(1){
		//
		//Delay_ms(100);
				//2.�յ�����
			 if(radio_busy & RX_DR){
				 radio_busy = 0;
				 
					//printf("\r\n ������ ���յ� �ӻ��� ���͵�����Ϊ");		 
					//for(i=0;i<4;i++)
					{					
						printf(" %d",NRF__RX_BUF[0]);
					} 

			 }
			 
		 }
}

//*******************************************************
void nrf_main()
{
	uint32_t nrf_time;
	uint8_t status=0;
	uint8_t i;
	nrf_init();		
	//nrf_EXIT_Config();	
	/*���NRFģ����MCU������*/
	while(!status){
	status =nrf_check();	
	}
	//
	/*�ж�����״̬*/	
	 if(status == SUCCESS)		 
	 		 printf("\r\n			NRF��MCU���ӳɹ���\r\n");	
	 else		
	 		 printf("\r\n	NRF��MCU����ʧ�ܣ������¼����ߡ�\r\n");
		nrf_test();
		nrf_config();
		nrf_test();
	 
	 nrf_recv_ack();
		 while(1){	 
			 
			 if(TIM4_GetDistanceTime(nrf_time)>1){
			 			nrf_time = TIM4_GetCurrentTime();
		 
				//printf("\r\n ������ ���뷢��ģʽ\r\n"); 				 
						status = nrf_tx_dat();
						switch(status)
						{
						case (1<<HAL_NRF_MAX_RT):
						printf("\r\n ������ û���յ�Ӧ���źţ����ʹ��������޶�ֵ������ʧ�ܡ� \r\n");
						break;

						case (1<<HAL_NRF_TX_DS):
						//printf("\r\n ������ ���յ� �ӻ��� ��Ӧ���źţ����ͳɹ��� \r\n");	 		
						break;									
						}			 
			 }
			 
			//2.�յ�����
			 if(radio_busy & RX_DR){
				 radio_busy = 0;
			// Read payload
				while(!hal_nrf_rx_fifo_empty())
				{

					hal_nrf_read_rx_payload(NRF__RX_BUF);

					printf("\r\n ������ ���յ� �ӻ��� ���͵�����Ϊ");		 
					for(i=0;i<4;i++)
					{					
						printf(" %d",NRF__RX_BUF[i]);
					} 
				}
				 
			}
		
		 }
		 
}

// Radio interrupt
void NRF_ISR()
{
	uint8_t irq_flags = 0;

	// Read and clear IRQ flags from radio
	irq_flags = hal_nrf_get_clear_irq_flags();

		 // radio_busy = false;
	switch(irq_flags)
	{
		// Transmission success
		case (1 << (uint8_t)HAL_NRF_TX_DS):
			radio_busy = TX_DS;
			// Data has been sent
			break;
		// Transmission failed (maximum re-transmits)
		case (1 << (uint8_t)HAL_NRF_MAX_RT):
			// When a MAX_RT interrupt occurs the TX payload will not be removed from the TX FIFO.
			// If the packet is to be discarded this must be done manually by flushing the TX FIFO.
			// Alternatively, CE_PULSE() can be called re-starting transmission of the payload.
			// (Will only be possible after the radio irq flags are cleared)
			//hal_nrf_flush_tx();
			radio_busy = MAX_RT;
			break;
		 // If data received
		case (1 << (uint8_t)HAL_NRF_RX_DR):
			radio_busy = RX_DR;
		// Read payload
		while(!hal_nrf_rx_fifo_empty())
		{
			//����reg<<8+lenth
			hal_nrf_read_rx_payload(NRF__RX_BUF);
		}
		//��Ҫ�˳�����ģʽ
		CE_HIGH();
		
		break;
		default:
			break;
	}
}
