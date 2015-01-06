#include <stdint.h>
#include "stdio.h"
#include "./nrf24l01p/hal_nrf.h"
//工作模式的配置 
//Mode  	 PWR_UP       PRIM_RX(1) 		CE 
//发送模式 		 1 			0 				1 
//发送模式 		 1 			0 				≥10微秒的高电平 
//接收模式 		 1 			1 				1 
	
//待机模式Ⅱ	 1 			0 				1 
//待机模式Ⅰ	 1			- 				0 
//掉电模式  	 0 			- 				- 
//增强型 ShockBurst 模式的数据帧格式 
//帧头| 地址 |控制域| 数据| CRC 
//地址长度为3-5个字节，其内容为接收机的地址
// Global variables
uint8_t payload[RX_PLOAD_WIDTH];
uint8_t NRF__RX_BUF[RX_PLOAD_WIDTH] = {0,0,0,0};		//接收数据缓存
uint8_t NRF__TX_BUF[TX_PLOAD_WIDTH] = {0,0x65,2,3};		//发射数据缓存
//moren
uint8_t NRF_MASTER_ADDRESS[TX_ADR_WIDTH] = {0x34,0x43,0x10,0x10,0x01};  // 定义一个静态发送地址
uint8_t NRF_DEVICE_ADDRESS[RX_ADR_WIDTH] = {0x34,0x43,0x10,0x10,0x01};



uint8_t radio_busy;

static void nrf_init(){
	//端口&SPI初始化
	SPI2_Init();	
	//释放SPI总线
	CSN_HIGH(); 
}
uint8_t  nrf_test(){
	uint8_t reg;
    
    reg = hal_nrf_read_reg(CONFIG);
    reg = hal_nrf_read_reg(EN_AA      );
    reg = hal_nrf_read_reg(EN_RXADDR  );
    reg = hal_nrf_read_reg(SETUP_AW   );
    reg = hal_nrf_read_reg(SETUP_RETR );
    reg = hal_nrf_read_reg(RF_CH      );
    reg = hal_nrf_read_reg(RF_SETUP   );
    reg = hal_nrf_read_reg(STATUS     );
    reg = hal_nrf_read_reg(OBSERVE_TX );
    reg = hal_nrf_read_reg(CD         );
    reg = hal_nrf_read_reg(RX_ADDR_P0 );
    reg = hal_nrf_read_reg(RX_ADDR_P1 );
    reg = hal_nrf_read_reg(RX_ADDR_P2 );
    reg = hal_nrf_read_reg(RX_ADDR_P3 );
    reg = hal_nrf_read_reg(RX_ADDR_P4 );
    reg = hal_nrf_read_reg(RX_ADDR_P5 );
    reg = hal_nrf_read_reg(TX_ADDR    );
    reg = hal_nrf_read_reg(RX_PW_P0   );
    reg = hal_nrf_read_reg(RX_PW_P1   );
    reg = hal_nrf_read_reg(RX_PW_P2   );
    reg = hal_nrf_read_reg(RX_PW_P3   );
    reg = hal_nrf_read_reg(RX_PW_P4   );
    reg = hal_nrf_read_reg(RX_PW_P5   );
    reg = hal_nrf_read_reg(FIFO_STATUS);
    reg = hal_nrf_read_reg(DYNPD      );
    reg = hal_nrf_read_reg(FEATURE    );
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
	
	/*写入5个字节的地址.  */  
	//SPI_NRF_WriteBuf(NRF_WRITE_REG+TX_ADDR,buf,5);
    hal_nrf_set_address(HAL_NRF_TX,buf);
	/*读出写入的地址 */
	//SPI_NRF_ReadBuf(TX_ADDR,buf1,5); 
	  hal_nrf_get_address(HAL_NRF_TX,buf1);
	/*比较*/               
	for(i=0;i<5;i++)
	{
		if(buf1[i]!=buf[i])
		break;
	} 
	       
	if(i==5)
		return SUCCESS ;        //MCU与NRF成功连接 
	else
		return ERROR ;        //MCU与NRF不正常连接
}

static void nrf_config(){
		//参数配置必须在POWER DOWN 模式下
		//进入空闲模式
    CE_LOW();	
		//系统设置
	  //RF_SETUP设置传输功率,默认0db
    hal_nrf_set_output_power(HAL_NRF_0DBM);
    //RF_SETUP设置传输速率,默认2Mbps
    hal_nrf_set_datarate(HAL_NRF_2MBPS);

		//RF_CH设置传输通道 默认:2
		hal_nrf_set_rf_channel(40);
		//CONFIG设置中断IRQ 
		hal_nrf_set_irq_mode(HAL_NRF_MAX_RT,true);
		hal_nrf_set_irq_mode(HAL_NRF_TX_DS,true);
		hal_nrf_set_irq_mode(HAL_NRF_RX_DR,true);

		//SETUP_AW设置地址宽度，默认5
		//hal_nrf_set_address_width(HAL_NRF_AW_5BYTES);
		//RX_PW_P0设置接收数据宽度,默认0 nouse
		hal_nrf_set_rx_payload_width(HAL_NRF_PIPE0,RX_PLOAD_WIDTH);
		//RX_ADDR_P0设置发送节点地址,默认0xE7E7E7E7E7
		hal_nrf_set_address(HAL_NRF_TX,NRF_MASTER_ADDRESS);
		//RX_ADDR_P0设置接收节点0地址,默认0xE7E7E7E7E7
		hal_nrf_set_address(HAL_NRF_PIPE0,NRF_MASTER_ADDRESS);
		//RX_ADDR_P0设置接收节点1地址,默认0xC2C2C2C2C2
		//hal_nrf_set_address(HAL_NRF_PIPE1,NRF_DEVICE_ADDRESS);

		//SETUP_RETR设置重发次数,重发间隔,默认3次,250us
		hal_nrf_set_auto_retr(10,500);
		//EN_AA,EN_RXADDR开启通道0的自动应答,开启时强制打开CRC校验,默认开启
		hal_nrf_open_pipe(HAL_NRF_PIPE0,EN_AA);
			//CONFIG打开CRC16校验,默认8位
    hal_nrf_set_crc_mode(HAL_NRF_CRC_16BIT);
    //CONFIG默认POWER DOWN
    hal_nrf_set_power_mode(HAL_NRF_PWR_UP);
    
}


void nrf_tx_mode(){
		CE_LOW();
   //CONFIG设置发送模式,默认TX
    hal_nrf_set_operation_mode(HAL_NRF_PTX);
		CE_HIGH();
		Delay_us(100);
}
void nrf_rx_mode(){
		CE_LOW();
    //CONFIG设置发送模式,默认TX
    hal_nrf_set_operation_mode(HAL_NRF_PRX);
		CE_HIGH();
		
}
uint8_t nrf_tx_dat(){
	
	uint8_t status =0;
		radio_busy = true;
		CE_LOW();
    hal_nrf_write_tx_payload(NRF__TX_BUF,TX_PLOAD_WIDTH);
	
	  CE_HIGH();
		CE_PULSE();//Delay_us(15);
		//等待发送完成
		while(NRF_Read_IRQ()!=0); 
		//while(radio_busy);
		Delay_us(2000);
		status = hal_nrf_get_clear_irq_flags();
	  hal_nrf_flush_tx();
	 
    //If MAX_RT is asserted it must be cleared to enable further communication.
		//hal_nrf_clear_irq_flag(HAL_NRF_MAX_RT);
	
		return status;
}
#define POLL
uint8_t nrf_rx_dat(){
	uint8_t status =0;
	#ifdef POLL
		nrf_rx_mode();
    //CE_HIGH();
		while(NRF_Read_IRQ()!=0); 
  #endif
    
    CE_LOW();
    //pipe
    //hal_nrf_get_rx_data_source();
   
    status = hal_nrf_get_clear_irq_flags();
	
    if(status & (1<<RX_DR)){
			hal_nrf_read_rx_payload(NRF__RX_BUF);
			hal_nrf_flush_rx();
		}
	return status;
 
}

// Radio interrupt
void NRF_ISR()
{
  uint8_t irq_flags = 0;

  // Read and clear IRQ flags from radio
  irq_flags = hal_nrf_get_clear_irq_flags();

  switch(irq_flags)
  {
    // Transmission success
    case (1 << (uint8_t)HAL_NRF_TX_DS):
      radio_busy = false;
      // Data has been sent
      break;
    // Transmission failed (maximum re-transmits)
    case (1 << (uint8_t)HAL_NRF_MAX_RT):
      // When a MAX_RT interrupt occurs the TX payload will not be removed from the TX FIFO.
      // If the packet is to be discarded this must be done manually by flushing the TX FIFO.
      // Alternatively, CE_PULSE() can be called re-starting transmission of the payload.
      // (Will only be possible after the radio irq flags are cleared)
      hal_nrf_flush_tx();
      radio_busy = false;
      break;
		 // If data received
		case (1 << (uint8_t)HAL_NRF_RX_DR):

    // Read payload
    while(!hal_nrf_rx_fifo_empty())
    {
      hal_nrf_read_rx_payload(payload);
    }

		break;
    default:
      break;
  }
}


//*******************************************************
void nrf_main()
{
	uint8_t status=0;
	uint8_t i;
	nrf_init();		
	//nrf_EXIT_Config();	
	/*检测NRF模块与MCU的连接*/
	while(!status){
  status =nrf_check();	
	}
	//
	/*判断连接状态*/  
   if(status == SUCCESS)	   
   		 printf("\r\n      NRF与MCU连接成功！\r\n");  
   else	  
   	   printf("\r\n  NRF与MCU连接失败，请重新检查接线。\r\n");
	  nrf_test();
		nrf_config();
	  nrf_test();
			 nrf_rx_mode();
     while(1){   
     
     
	   	printf("\r\n 主机端 进入自应答发送模式\r\n"); 
				nrf_tx_mode();
    status = nrf_tx_dat();
        switch(status)
        {
        case (1<<MAX_RT):
        printf("\r\n 主机端 没接收到应答信号，发送次数超过限定值，发送失败。 \r\n");
        break;

        case (1<<TX_DS):
        printf("\r\n 主机端 接收到 从机端 的应答信号，发送成功！ \r\n");	 		
        break;  								
        }		
        	 	printf("\r\n 主机端 进入接收模式。 \r\n");	
			 	nrf_rx_mode();
		status = nrf_rx_dat();
        	switch(status)
			{
			 case (1<<RX_DR):
				 printf("\r\n 主机端 接收到 从机端 发送的数据为");
			 	for(i=0;i<4;i++)
				{					
					printf(" %d",NRF__RX_BUF[i]);
				}
            break;	
			}
        
         Delay_ms(1000);
     }
		 
}

