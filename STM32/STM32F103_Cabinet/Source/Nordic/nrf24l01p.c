#include <stdint.h>
#include "stdio.h"
#include "./nrf24l01p/hal_nrf.h"

// Global variables
uint8_t payload[3];
uint8_t NRF__RX_BUF[RX_PLOAD_WIDTH];		//接收数据缓存
uint8_t NRF__TX_BUF[TX_PLOAD_WIDTH];		//发射数据缓存
//moren
uint8_t TX_ADDRESS[TX_ADR_WIDTH] = {0x34,0x43,0x10,0x10,0x01};  // 定义一个静态发送地址
uint8_t RX_ADDRESS[RX_ADR_WIDTH] = {0x34,0x43,0x10,0x10,0x01};
static void nrf_init(){
	//端口&SPI初始化
	SPI2_Config();
	
	//释放SPI总线
	CSN_HIGH(); 
}

static uint8_t nrf_check(void)
{
	uint8_t buf[5]={0xC2,0xC2,0xC2,0xC2,0xC2};
	uint8_t buf1[5];
	uint8_t i; 
	 
	/*写入5个字节的地址.  */  
	//SPI_NRF_WriteBuf(NRF_WRITE_REG+TX_ADDR,buf,5);

    hal_nrf_set_address(HAL_NRF_TX,buf);
	/*读出写入的地址 */
	//SPI_NRF_ReadBuf(TX_ADDR,buf1,5); 
	  hal_nrf_get_address(HAL_NRF_TX,buf1);
	/*比较*/               
	for(i=0;i<5;i++)
	{
		if(buf1[i]!=0xC2)
		break;
	} 
	       
	if(i==5)
		return SUCCESS ;        //MCU与NRF成功连接 
	else
		return ERROR ;        //MCU与NRF不正常连接
}

static void nrf_config(){
	//进入空闲模式
    CE_LOW();	
		//1.设置地址宽度，默认5
		hal_nrf_set_address_width(HAL_NRF_AW_5BYTES);
    //设置接收节点0地址,默认0xE7E7E7E7E7
    hal_nrf_set_address(HAL_NRF_PIPE0,RX_ADDRESS);
		//设置接收节点1地址,默认0xC2C2C2C2C2
		hal_nrf_set_address(HAL_NRF_PIPE1,RX_ADDRESS);
    // 设置发送节点地址,默认0xE7E7E7E7E7
    hal_nrf_set_address(HAL_NRF_TX,TX_ADDRESS);
    //设置传输通道 默认:2
		hal_nrf_set_rf_channel(40);
    //设置接收数据宽度
    hal_nrf_set_rx_payload_width(HAL_NRF_PIPE0,RX_PLOAD_WIDTH);
    //设置传输功率,默认0db
    hal_nrf_set_output_power(HAL_NRF_0DBM);
    //设置传输速率,默认2Mbps
    hal_nrf_set_datarate(HAL_NRF_2MBPS);
    //打开CRC16校验,默认8位
    hal_nrf_set_crc_mode(HAL_NRF_CRC_16BIT);
    //开启通道0的自动应答,开启时强制打开CRC校验
    hal_nrf_open_pipe(HAL_NRF_PIPE1,true);    
    //设置重发次数,重发间隔,默认3次,250us
    hal_nrf_set_auto_retr(5,500);
    //默认POWER DOWN
    hal_nrf_set_power_mode(HAL_NRF_PWR_UP);
    //设置发送模式,默认TX
    hal_nrf_set_operation_mode(HAL_NRF_PTX);
    //进入接收模式
    CE_HIGH();
    
}

void nrf_main()
{
	uint8_t status;

	nrf_init();		 
	/*检测NRF模块与MCU的连接*/
  status =nrf_check();	


	/*判断连接状态*/  
   if(status == SUCCESS)	   
   		 printf("\r\n      NRF与MCU连接成功！\r\n");  
   else	  
   	     printf("\r\n  NRF与MCU连接失败，请重新检查接线。\r\n");

	nrf_config();
  for(;;){}
}

// Radio interrupt
void NRF_ISR()
{
  uint8_t irq_flags;

  // Read and clear IRQ flags from radio
  irq_flags = hal_nrf_get_clear_irq_flags();

  // If data received
  if((irq_flags & (1<<(uint8_t)HAL_NRF_RX_DR)) > 0)
  {
    // Read payload
    while(!hal_nrf_rx_fifo_empty())
    {
      hal_nrf_read_rx_payload(payload);
    }

    // Write received payload[0] to port 0
//    P0 = payload[0];
  }
}

/*
 * 函数名：NRF_RX_Mode
 * 描述  ：配置并进入接收模式
 * 输入  ：无	
 * 输出  ：无
 * 调用  ：外部调用
 */
void NRF_RX_Mode(void)

{
	
	//进入空闲模式
    CE_LOW();	
		//1.设置地址宽度，默认5
		hal_nrf_set_address_width(HAL_NRF_AW_5BYTES);
    //设置接收节点0地址,默认0xE7E7E7E7E7
    hal_nrf_set_address(HAL_NRF_PIPE0,RX_ADDRESS);
		//设置接收节点1地址,默认0xC2C2C2C2C2
		hal_nrf_set_address(HAL_NRF_PIPE1,RX_ADDRESS);
    // 设置发送节点地址,默认0xE7E7E7E7E7
    hal_nrf_set_address(HAL_NRF_TX,TX_ADDRESS);
    //设置传输通道 默认:2
		hal_nrf_set_rf_channel(40);
    //设置接收数据宽度
    hal_nrf_set_rx_payload_width(HAL_NRF_PIPE0,RX_PLOAD_WIDTH);
    //设置传输功率,默认0db
    hal_nrf_set_output_power(HAL_NRF_0DBM);
    //设置传输速率,默认2Mbps
    hal_nrf_set_datarate(HAL_NRF_2MBPS);
    //打开CRC16校验,默认8位
    hal_nrf_set_crc_mode(HAL_NRF_CRC_16BIT);
    //开启通道0的自动应答,开启时强制打开CRC校验
    hal_nrf_open_pipe(HAL_NRF_PIPE1,true);    
    //设置重发次数,重发间隔,默认3次,250us
    hal_nrf_set_auto_retr(5,500);
    //默认POWER DOWN
    hal_nrf_set_power_mode(HAL_NRF_PWR_UP);
    //设置发送模式,默认TX
    hal_nrf_set_operation_mode(HAL_NRF_PTX);
    //进入接收模式
    CE_HIGH();
    
//   SPI_NRF_WriteBuf(NRF_WRITE_REG+RX_ADDR_P0,RX_ADDRESS,RX_ADR_WIDTH);//写RX节点地址

//   SPI_NRF_WriteReg(NRF_WRITE_REG+EN_AA,0x01);    //使能通道0的自动应答    

//   SPI_NRF_WriteReg(NRF_WRITE_REG+EN_RXADDR,0x01);//使能通道0的接收地址    

//   SPI_NRF_WriteReg(NRF_WRITE_REG+RF_CH,CHANAL);      //设置RF通信频率    

//   SPI_NRF_WriteReg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//选择通道0的有效数据宽度      

//   SPI_NRF_WriteReg(NRF_WRITE_REG+RF_SETUP,0x0f); //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   

//   SPI_NRF_WriteReg(NRF_WRITE_REG+CONFIG, 0x0f);  //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式 

/*CE拉高，进入接收模式*/	
  //NRF_CE_HIGH();

}    

/*
 * 函数名：NRF_TX_Mode
 * 描述  ：配置发送模式
 * 输入  ：无	
 * 输出  ：无
 * 调用  ：外部调用
 */
void NRF_TX_Mode(void)
{  
	//NRF_CE_LOW();		

//   SPI_NRF_WriteBuf(NRF_WRITE_REG+TX_ADDR,TX_ADDRESS,TX_ADR_WIDTH);    //写TX节点地址 

//   SPI_NRF_WriteBuf(NRF_WRITE_REG+RX_ADDR_P0,RX_ADDRESS,RX_ADR_WIDTH); //设置TX节点地址,主要为了使能ACK   

//   SPI_NRF_WriteReg(NRF_WRITE_REG+EN_AA,0x01);     //使能通道0的自动应答    

//   SPI_NRF_WriteReg(NRF_WRITE_REG+EN_RXADDR,0x01); //使能通道0的接收地址  

//   SPI_NRF_WriteReg(NRF_WRITE_REG+SETUP_RETR,0x1a);//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次

//   SPI_NRF_WriteReg(NRF_WRITE_REG+RF_CH,CHANAL);       //设置RF通道为CHANAL

//   SPI_NRF_WriteReg(NRF_WRITE_REG+RF_SETUP,0x0f);  //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
//	
//   SPI_NRF_WriteReg(NRF_WRITE_REG+CONFIG,0x0e);    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,发射模式,开启所有中断

/*CE拉高，进入发送模式*/	
  //NRF_CE_HIGH();
	CE_PULSE();
    //Delay(0xffff); //CE要拉高一段时间才进入发送模式
}



/*
 * 函数名：NRF_Check
 * 描述  ：主要用于NRF与MCU是否正常连接
 * 输入  ：无	
 * 输出  ：SUCCESS/ERROR 连接正常/连接失败
 * 调用  ：外部调用
 */
