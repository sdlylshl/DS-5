/******************** (C) COPYRIGHT 2012 WildFire Team **************************
 * 文件名  ：SPI_NRF.c
 * 描述    ：SPI_NRF24L01+无线模块应用函数库         
 * 实验平台：野火STM32开发板
 * 硬件连接：-----------------------------|
 *          |                			  |
 *          |  	PA1            : NRF- CSN |
			|	PA5-SPI1-SCK   : NRF -SCK |
			|	PA6-SPI1-MISO  : NRF -MISO|
			|	PA7-SPI1-MOSI  : NRF -MOSI|
			|	PA3		   	   : NRF-IRQ  |
			|	PA2		   	   : NRF-CE   |
 *          |                			  |
 *           -----------------------------
 * 库版本  ：ST3.5.0
 *
 * 库版本  ：ST3.5.0
 *
 * 作者    ：wildfire team 
 * 论坛    ：http://www.amobbs.com/forum-1008-1.html
 * 淘宝    ：http://firestm32.taobao.com
**********************************************************************************/
#include "SPI_NRF.h"
#include <stdio.h>
// SPI2
//#include "./System/System_config.h"
#include "../../System/mcc.h"

uint8_t NRF__RX_BUF[RX_PLOAD_WIDTH] = { 0, 0, 0, 0 }; //接收数据缓存
uint8_t NRF__TX_BUF[TX_PLOAD_WIDTH] = { 0x18, 0x65, 2, 3 }; //发射数据缓存
//uint8_t NRF__DTX_BUF[TX_PLOAD_WIDTH] = { 0x11, 0x65, 2, 3 }; //发射数据缓存

//device ----------------master
//send 0xe7   --       0xe7            40
//recv 0x34  --- -       0x34           70
uint8_t NRF_MASTER_RECV_ADDRESS[TX_ADR_WIDTH] = { 0xe7, 0xe7, 0xe7, 0xe7, 0xe7 }; // 定义一个静态发送地址
uint8_t NRF_MASTER_SEND_ADDRESS[TX_ADR_WIDTH] = { 0x34, 0x43, 0x10, 0x10, 0x01 }; // 定义一个静态发送地址


// extern uint8_t NRF__RX_BUF[];		//接收数据缓存
// extern uint8_t NRF__TX_BUF[];		//发射数据缓存
 
// extern uint8_t NRF_MASTER_RECV_ADDRESS[];
// extern uint8_t NRF_MASTER_SEND_ADDRESS[];
 
#define NRF_DEVICE_RECV_ADDRESS NRF_MASTER_SEND_ADDRESS
#define NRF_DEVICE_SEND_ADDRESS NRF_MASTER_RECV_ADDRESS
#define MASTER_RX_CHANNEL 40
#define MASTER_TX_CHANNEL 70
#define DEVICE_RX_CHANNEL MASTER_TX_CHANNEL
#define DEVICE_TX_CHANNEL MASTER_RX_CHANNEL
#define NRF_ADDRESS_WIDTH 5
extern uint32_t (*GetCurrentTime)(void);
extern uint32_t (*GetDistanceTime)(uint32_t );
//#define GetCurrentTime TIM2_GetCurrentTime
//#define GetDistanceTime TIM2_GetDistanceTime
void Delay(volatile uint32_t nCount)
{
  for(; nCount != 0; nCount--);
} 
////注册外部接口函数
//SPI2
//void (*NRF_CSN_LOW)(void) = SPI2_ResetNSS;
//void (*NRF_CSN_HIGH)(void) = SPI2_SetNSS;
//void (*NRF_CE_LOW)(void) = SPI2_ResetRST;
//void (*NRF_CE_HIGH)(void) = SPI2_SetRST;
//uint8_t (*NRF_GET_IRQ)(void) = SPI2_readIRQ;
//uint8_t (*SPI_NRF_RW)(uint8_t) = SPI2_SendByte;

// 模拟SPI
void (*NRF_CSN_LOW)(void) = SPI_CSN_LOW;
void (*NRF_CSN_HIGH)(void) = SPI_CSN_HIGH;
void (*NRF_CE_LOW)(void) = SPI_CEN_LOW;
void (*NRF_CE_HIGH)(void) = SPI_CEN_HIGH;
uint8_t (*NRF_GET_IRQ)(void) = SPI_IRQ_READ;
uint8_t (*SPI_NRF_RW)(uint8_t) = SPI_ReadWrite;


//static void NRF_CSN_HIGH(){
//		SPI2_SCS_HIGH();
//}
//static void NRF_CSN_LOW(){
//		SPI2_SCS_LOW();
//}
//static void NRF_CE_HIGH(){
//		//NRF_CE(1);
//	SPI2_RST_HIGH();
//}
//static  void NRF_CE_LOW(){
//		//NRF_CE(0);
//	SPI2_RST_LOW();
//}

//static uint8_t NRF_GET_IRQ(){
//	//return NRF_IRQ_READ();
////return (GPIO_ReadInputDataBit(NRF_IRQ_PORT, NRF_IRQ_PIN)); //低电平产生中断
//	return SPI2_READ_IRQ();
//}

//static uint8_t SPI_NRF_RW(uint8_t dat){
//	return SPI2_SendByte(dat);
//}

/*
 * 函数名：SPI_NRF_Init
 * 描述  ：SPI的 I/O配置
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */

void SPI_NRF_Init(){
	//NRF_GPIO_Config();
	//SPI2_Init();
	//NRF_CSN_HIGH(); 
}


/*
 * 函数名：SPI_NRF_WriteReg
 * 描述  ：用于向NRF特定的寄存器写入数据
 * 输入  ：reg:NRF的命令+寄存器地址。
 		   dat:将要向寄存器写入的数据
 * 输出  ：NRF的status寄存器的状态
 * 调用  ：内部调用
 */
uint8_t SPI_NRF_WriteReg(uint8_t reg,uint8_t dat)
{
 	uint8_t status;
	 NRF_CE_LOW();
	/*置低CSN，使能SPI传输*/
    NRF_CSN_LOW();
				
	/*发送命令及寄存器号 */
	status = SPI_NRF_RW(reg);
		 
	 /*向寄存器写入数据*/
    SPI_NRF_RW(dat); 
	          
	/*CSN拉高，完成*/	   
  	NRF_CSN_HIGH();	
		
	/*返回状态寄存器的值*/
   	return(status);
}


/*
 * 函数名：SPI_NRF_ReadReg
 * 描述  ：用于从NRF特定的寄存器读出数据
 * 输入  ：reg:NRF的命令+寄存器地址。
 * 输出  ：寄存器中的数据
 * 调用  ：内部调用
 */
uint8_t SPI_NRF_ReadReg(uint8_t reg)
{
 	uint8_t reg_val;

	NRF_CE_LOW();
	/*置低CSN，使能SPI传输*/
 	NRF_CSN_LOW();
				
  	 /*发送寄存器号*/
	SPI_NRF_RW(reg); 

	 /*读取寄存器的值 */
	reg_val = SPI_NRF_RW(NRF_NOP);
	            
   	/*CSN拉高，完成*/
	NRF_CSN_HIGH();		
   	
	return reg_val;
}	


/*
 * 函数名：SPI_NRF_ReadBuf
 * 描述  ：用于从NRF的寄存器中读出一串数据
 * 输入  ：reg:NRF的命令+寄存器地址。
 		   pBuf：用于存储将被读出的寄存器数据的数组，外部定义
		   bytes: pBuf的数据长度	
 * 输出  ：NRF的status寄存器的状态
 * 调用  ：外部调用
 */
uint8_t SPI_NRF_ReadBuf(uint8_t reg,uint8_t *pBuf,uint8_t bytes)
{
 	uint8_t status, byte_cnt;

	  NRF_CE_LOW();
	/*置低CSN，使能SPI传输*/
	NRF_CSN_LOW();
		
	/*发送寄存器号*/		
	status = SPI_NRF_RW(reg); 

 	/*读取缓冲区数据*/
	 for(byte_cnt=0;byte_cnt<bytes;byte_cnt++)		  
	   pBuf[byte_cnt] = SPI_NRF_RW(NRF_NOP); //从NRF24L01读取数据  

	 /*CSN拉高，完成*/
	NRF_CSN_HIGH();	
		
 	return status;		//返回寄存器状态值
}



/*
 * 函数名：SPI_NRF_WriteBuf
 * 描述  ：用于向NRF的寄存器中写入一串数据
 * 输入  ：reg:NRF的命令+寄存器地址。
 		   pBuf：存储了将要写入写寄存器数据的数组，外部定义
		   bytes: pBuf的数据长度	
 * 输出  ：NRF的status寄存器的状态
 * 调用  ：外部调用
 */
uint8_t SPI_NRF_WriteBuf(uint8_t reg ,uint8_t *pBuf,uint8_t bytes)
{
	 uint8_t status,byte_cnt;
	 NRF_CE_LOW();
   	 /*置低CSN，使能SPI传输*/
	 NRF_CSN_LOW();			

	 /*发送寄存器号*/	
  	 status = SPI_NRF_RW(reg); 
 	
  	  /*向缓冲区写入数据*/
	 for(byte_cnt=0;byte_cnt<bytes;byte_cnt++)
		SPI_NRF_RW(*pBuf++);	//写数据到缓冲区 	 
	  	   
	/*CSN拉高，完成*/
	NRF_CSN_HIGH();			
  
  	return (status);	//返回NRF24L01的状态 		
}

void NRF_COMN_CONFIG(void){
	//	 NRF_EN_AA      :0x3f
	//	 NRF_EN_RXADDR  :0x03
	//	 NRF_SETUP_AW   :0x03
	//	 NRF_SETUP_RETR :0x1a
	//	 NRF_RF_SETUP   :0x0f
	//	 NRF_OBSERVE_TX :0x00
	//	 NRF_RX_PW_P0   :0x04
	NRF_CE_LOW();
	SPI_NRF_WriteReg(NRF_WRITE_REG + NRF_EN_AA,0x3f);	  //使能通道0的自动应答
	SPI_NRF_WriteReg(NRF_WRITE_REG + NRF_EN_RXADDR,0x03);//使能通道0的接收地址
	SPI_NRF_WriteReg(NRF_WRITE_REG + NRF_SETUP_AW,0x03);
	SPI_NRF_WriteReg(NRF_WRITE_REG + NRF_SETUP_RETR,0x00);
	SPI_NRF_WriteReg(NRF_WRITE_REG + NRF_RF_SETUP,0x0f); //设置TX发射参数,0db增益,2Mbps,低噪声增益开启
	SPI_NRF_WriteReg(NRF_WRITE_REG + NRF_OBSERVE_TX,0x00);
	SPI_NRF_WriteReg(NRF_WRITE_REG + NRF_RX_PW_P0,RX_PLOAD_WIDTH);//选择通道0的有效数据宽度
}

//主机专用寄存器配置
//    send
//    NRF_CONFIG     :0x0e
//    NRF_RF_CH      :0x46
//    NRF_RX_ADDR_P0 :0x34
//    NRF_TX_ADDR    :0x34
//    recv
//    NRF_CONFIG     :0x0f
//    NRF_RF_CH      :0x28
//    NRF_RX_ADDR_P0 :0xe7
//    NRF_TX_ADDR    :0xe7

void NRF_MSATER_TX_MODE(void){

	NRF_CE_LOW();
	SPI_NRF_WriteReg(NRF_WRITE_REG + NRF_CONFIG,0x0e);    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,发射模式,开启所有中断
	SPI_NRF_WriteReg(NRF_WRITE_REG + NRF_RF_CH,0x46);

	SPI_NRF_WriteBuf(NRF_WRITE_REG + NRF_RX_ADDR_P0,NRF_MASTER_SEND_ADDRESS,NRF_ADDRESS_WIDTH);
	SPI_NRF_WriteBuf(NRF_WRITE_REG + NRF_TX_ADDR,NRF_MASTER_SEND_ADDRESS,NRF_ADDRESS_WIDTH);
}
void NRF_MSATER_RX_MODE(void){
	NRF_CE_LOW();
	SPI_NRF_WriteReg(NRF_WRITE_REG + NRF_CONFIG,0x0f);    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,发射模式,开启所有中断
	SPI_NRF_WriteReg(NRF_WRITE_REG + NRF_RF_CH,0x28);

	SPI_NRF_WriteBuf(NRF_WRITE_REG + NRF_RX_ADDR_P0,NRF_MASTER_RECV_ADDRESS,NRF_ADDRESS_WIDTH);
	SPI_NRF_WriteBuf(NRF_WRITE_REG + NRF_TX_ADDR,NRF_MASTER_RECV_ADDRESS,NRF_ADDRESS_WIDTH);
	NRF_CE_HIGH();
}
//设备专用寄存器配置
//    recv
//    NRF_CONFIG     :0x0f
//    NRF_RF_CH      :0x46
//    NRF_RX_ADDR_P0 :0x34
//    NRF_TX_ADDR    :0x34
//    send
//    NRF_CONFIG     :0x0e
//    NRF_RF_CH      :0x28
//    NRF_RX_ADDR_P0 :0xe7
//    NRF_TX_ADDR    :0xe7
void NRF_DEVICE_RX_MODE(void){
uint8_t state;
	NRF_CE_LOW();
	SPI_NRF_WriteReg(NRF_WRITE_REG + NRF_CONFIG,0x0f);    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,发射模式,开启所有中断
	SPI_NRF_WriteReg(NRF_WRITE_REG + NRF_RF_CH,0x46);

	SPI_NRF_WriteBuf(NRF_WRITE_REG + NRF_RX_ADDR_P0,NRF_DEVICE_RECV_ADDRESS,NRF_ADDRESS_WIDTH);
	SPI_NRF_WriteBuf(NRF_WRITE_REG + NRF_TX_ADDR,NRF_DEVICE_RECV_ADDRESS,NRF_ADDRESS_WIDTH);

	NRF_CE_HIGH();
}
void NRF_DEVICE_TX_MODE(void){
	NRF_CE_LOW();
	SPI_NRF_WriteReg(NRF_WRITE_REG + NRF_CONFIG,0x0e);    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,发射模式,开启所有中断
	SPI_NRF_WriteReg(NRF_WRITE_REG + NRF_RF_CH,0x28);

	SPI_NRF_WriteBuf(NRF_WRITE_REG + NRF_RX_ADDR_P0,NRF_DEVICE_SEND_ADDRESS,NRF_ADDRESS_WIDTH);
	SPI_NRF_WriteBuf(NRF_WRITE_REG + NRF_TX_ADDR,NRF_DEVICE_SEND_ADDRESS,NRF_ADDRESS_WIDTH);
}



/*
 * 函数名：NRF_Check
 * 描述  ：主要用于NRF与MCU是否正常连接
 * 输入  ：无	
 * 输出  ：SUCCESS/ERROR 连接正常/连接失败
 * 调用  ：外部调用
 */
uint8_t NRF_Check(void)
{
	uint8_t buf[5]={0xC2,0xC2,0xC2,0xC2,0xC2};
	uint8_t buf1[5];
	uint8_t i; 
	 
	/*写入5个字节的地址.  */  
	SPI_NRF_WriteBuf(NRF_WRITE_REG+NRF_TX_ADDR,buf,5);

	/*读出写入的地址 */
	SPI_NRF_ReadBuf(NRF_TX_ADDR,buf1,5); 
	 
	/*比较*/               
	for(i=0;i<5;i++)
	{
		if(buf1[i]!=0xC2)
		break;
	} 
	       
	if(i==5)
		return true ;        //MCU与NRF成功连接
	else
		return false ;        //MCU与NRF不正常连接
}


 /*
 * 函数名：NRF_Tx_Dat
 * 描述  ：用于向NRF的发送缓冲区中写入数据
 * 输入  ：txBuf：存储了将要发送的数据的数组，外部定义	
 * 输出  ：发送结果，成功返回TXDS,失败返回MAXRT或ERROR
 * 调用  ：外部调用
 */ 
uint8_t NRF_Tx_Dat(uint8_t *txbuf)
{
	uint8_t state;  
	uint32_t nrf_time=0x3000;
	
	NRF_CE_LOW();						
	SPI_NRF_WriteBuf(NRF_WR_TX_PAYLOAD,txbuf,TX_PLOAD_WIDTH);
	NRF_CE_HIGH();                           
	while(NRF_GET_IRQ()){

        if(!(nrf_time--)) 
				{
            printf("send timeout !\n");
            //send_erro = 1;	
					 //SPI_NRF_WriteReg(NRF_FLUSH_TX,NRF_NOP);    //1.超时清除TX FIFO寄存器 
            break;
        }
			} 	
	
	/*读取状态寄存器的值 */                              
	state = SPI_NRF_ReadReg(NRF_STATUS);

	 // 清除TX_DS或MAX_RT中断标志                 
	SPI_NRF_WriteReg(NRF_WRITE_REG+NRF_STATUS,state); 
	//  清除TX FIFO寄存器 	
	SPI_NRF_WriteReg(NRF_FLUSH_TX,NRF_NOP);   

	 /*判断中断类型*/    
	if(state&MAX_RT){                     //达到最大重发次数
		printf("send max !\n");
			 return MAX_RT; 
	}
	else if(state&TX_DS) {                 //发送完成
		printf("send ok \n");
	 	return TX_DS;
	}
	 else						  
			return false;                 //其他原因发送失败
} 

 /*
 * 函数名：NRF_Rx_Dat
 * 描述  ：用于从NRF的接收缓冲区中读出数据
 * 输入  ：rxBuf：用于接收该数据的数组，外部定义	
 * 输出  ：接收结果，
 * 调用  ：外部调用
 */ 
uint8_t NRF_Rx_Dat(uint8_t *rxbuf)
{
	uint32_t ct = GetCurrentTime();
	uint8_t state; 
	NRF_CE_HIGH();	 //进入接收状态
	//Delay(1000);
	printf("recv mode");
	 /*等待接收中断*/
	while(NRF_GET_IRQ()){
		if(GetDistanceTime(ct)>500)
			break;
	} 
	
	NRF_CE_LOW();  	 //进入待机状态
	/*读取status寄存器的值  */               
	state=SPI_NRF_ReadReg(NRF_STATUS);
	 
	/* 清除中断标志*/      
	SPI_NRF_WriteReg(NRF_WRITE_REG+NRF_STATUS,state);

	/*判断是否接收到数据*/
	if(state&RX_DR)                                 //接收到数据
	{
	  SPI_NRF_ReadBuf(NRF_RD_RX_PAYLOAD,rxbuf,RX_PLOAD_WIDTH);//读取数据
	     SPI_NRF_WriteReg(NRF_FLUSH_RX,NRF_NOP);          //清除RX FIFO寄存器
	  return RX_DR; 
	}
	else    
		return false;                    //没收到任何数据
}

/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
