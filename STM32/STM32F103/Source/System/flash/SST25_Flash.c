
#include "SST25_Flash.h"
uint8_t * flashTestData = "1234567890";
unsigned char SST25_buffer[4096];
uint16_t logNr = 2; //当前记录编号 
void SPI_Flash_Init(void);
uint8_t SPI_Flash_ReadByte(void);
uint8_t SPI_Flash_SendByte(uint8_t byte);

void wip(void);
void wen(void);
void wdis(void);
void wsr(void);
unsigned char rdsr(void);
void sect_clr(unsigned long a1);  
void SST25_R_BLOCK(unsigned long addr, unsigned char *readbuff, unsigned int BlockSize);
void SST25_W_BLOCK(uint32_t addr, uint8_t *readbuff, uint16_t BlockSize);
void SPI_Flash_Init(void);

/****************************************************************************
* 名    称：void wen(void)
* 功    能：写使能
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void wen(void){
	Select_Flash();
	SPI_Flash_SendByte(0x06);
	NotSelect_Flash();
}

/****************************************************************************
* 名    称：void wdis(void)
* 功    能：写禁止
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void wdis(void){

	Select_Flash();
	SPI_Flash_SendByte(0x04); 
	NotSelect_Flash();
	wip();
	
}	
/****************************************************************************
* 名    称：void wsr(void)
* 功    能：写状态
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void wsr(void){	
	Select_Flash();
	SPI_Flash_SendByte(0x50);
	NotSelect_Flash(); 
	Select_Flash();
	SPI_Flash_SendByte(0x01);
	SPI_Flash_SendByte(0x00); 
	NotSelect_Flash();
    wip();
}

/****************************************************************************
* 名    称：void wip(void)
* 功    能：忙检测
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void wip(void){
	unsigned char a=1;
	while((a&0x01)==1) a=rdsr();	

}
/****************************************************************************
* 名    称：unsigned char rdsr(void)
* 功    能：读状态寄存器
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
unsigned char rdsr(void){
	unsigned char busy;
	Select_Flash();
	SPI_Flash_SendByte(0x05);
	busy = SPI_Flash_ReadByte();
	NotSelect_Flash();
	return(busy);
	
}

/****************************************************************************
* 名    称：void SST25_R_BLOCK(unsigned long addr, unsigned char *readbuff, unsigned int BlockSize)
* 功    能：页读
* 入口参数：unsigned long addr--页   unsigned char *readbuff--数组   unsigned int BlockSize--长度
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/  
void SST25_R_BLOCK(unsigned long addr, unsigned char *readbuff, unsigned int BlockSize){
	unsigned int i=0; 	
	Select_Flash();
	SPI_Flash_SendByte(0x0b);
	SPI_Flash_SendByte((addr&0xffffff)>>16);
	SPI_Flash_SendByte((addr&0xffff)>>8);
	SPI_Flash_SendByte(addr&0xff);
	SPI_Flash_SendByte(0);
	while(i<BlockSize){	
		readbuff[i]=SPI_Flash_ReadByte();
		
		i++;
	}
	NotSelect_Flash();	 	
}
/****************************************************************************
* 名    称：void SST25_W_BLOCK(uint32_t addr, uint8_t *readbuff, uint16_t BlockSize)
* 功    能：页写
* 入口参数：uint32_t addr--页   uint8_t *readbuff--数组   uint16_t BlockSize--长度
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/  
void SST25_W_BLOCK(uint32_t addr, uint8_t *readbuff, uint16_t BlockSize){
	unsigned int i=0,a2;
	//sect_clr(addr);   								  //删除页		  
	wsr();
  	wen();	
	Select_Flash();    
	SPI_Flash_SendByte(0xad);
	SPI_Flash_SendByte((addr&0xffffff)>>16);
	SPI_Flash_SendByte((addr&0xffff)>>8);
	SPI_Flash_SendByte(addr&0xff);
  	SPI_Flash_SendByte(readbuff[0]);
	SPI_Flash_SendByte(readbuff[1]);
	NotSelect_Flash();
	i=2;
	while(i<BlockSize){
		a2=120;
		while(a2>0) a2--;
		Select_Flash();
		SPI_Flash_SendByte(0xad);
		SPI_Flash_SendByte(readbuff[i++]);
		SPI_Flash_SendByte(readbuff[i++]);
		NotSelect_Flash();
	}
	
	a2=100;
	while(a2>0) a2--;
	wdis();	
	Select_Flash();	
	wip();
}
/****************************************************************************
* 名    称：void sect_clr(unsigned long a1)
* 功    能：页擦除
* 入口参数：unsigned long a1--页   
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void sect_clr(unsigned long a1){
	wsr();
	wen();     
	Select_Flash();	  
	SPI_Flash_SendByte(0x20);
	SPI_Flash_SendByte((a1&0xffffff)>>16);          //addh
	SPI_Flash_SendByte((a1&0xffff)>>8);          //addl 
	SPI_Flash_SendByte(a1&0xff);                 //wtt
	NotSelect_Flash();
	wip();
	
}

/****************************************************************************
* 名    称：void sect_clr(unsigned long a1)
* 功    能：页擦除
* 入口参数：unsigned long a1--页   
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void Bulk_clr(){
	wsr();
	wen();     
	Select_Flash();	  
	SPI_Flash_SendByte(0xc7);
	NotSelect_Flash();
	wip();
	
}

/****************************************************************************
* 名    称：void FlashReadID(void)
* 功    能：读工厂码及型号的函数
* 入口参数：  
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
uint16_t FlashReadID(void)
{
	uint8_t fac_id;
	uint8_t dev_id;
	Select_Flash();	
  	SPI_Flash_SendByte(0x90);
	SPI_Flash_SendByte(0x00);
	SPI_Flash_SendByte(0x00);
	SPI_Flash_SendByte(0x00);
  fac_id= SPI_Flash_ReadByte();		          //BFH: 工程码SST
	dev_id= SPI_Flash_ReadByte();	              //41H: 器件型号SST25VF016B     
  	NotSelect_Flash();
  	return (fac_id<<8|dev_id);
}

/*******************************************************************************
* Function Name  : SPI_FLASH_Init
* Description    : Initializes the peripherals used by the SPI FLASH driver.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_Flash_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
   
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 ,ENABLE);	
  /* 配置 SPI2 引脚: SCK, MISO 和 MOSI */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
 
  //V3---ENC28J60的SPI1 片选 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* SPI2配置 */ 
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI2, &SPI_InitStructure);
  
  SPI_Cmd(SPI2, ENABLE);   
  NotSelect_Flash();

//  GPIO_SetBits(GPIOB, GPIO_Pin_4);			//禁止触摸电路的片选
//  GPIO_SetBits(GPIOB, GPIO_Pin_0);			//禁止ENC28J60的片选
}

/*******************************************************************************
* Function Name  : SPI_FLASH_ReadByte
* Description    : Reads a byte from the SPI Flash.
*                  This function must be used only if the Start_Read_Sequence
*                  function has been previously called.
* Input          : None
* Output         : None
* Return         : Byte Read from the SPI Flash.
*******************************************************************************/
uint8_t SPI_Flash_ReadByte(void)
{
  return (SPI_Flash_SendByte(Dummy_Byte));
}

/*******************************************************************************
* Function Name  : SPI_FLASH_SendByte
* Description    : Sends a byte through the SPI interface and return the byte 
*                  received from the SPI bus.
* Input          : byte : byte to send.
* Output         : None
* Return         : The value of the received byte.
*******************************************************************************/
uint8_t SPI_Flash_SendByte(uint8_t byte)
{
  /* Loop while DR register in not emplty */
  while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
  //NotSelect_Flash();  while(1);
  /* Send byte through the SPI2 peripheral */
  SPI_I2S_SendData(SPI2, byte);

  /* Wait to receive a byte */
  while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);

  /* Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(SPI2);
}



void testFlash(void )
{
	int i = 0;
//uint8_t flashTestData[2000] = {0};
FlashReadID();
	SST25_W_BLOCK(0, flashTestData,5);	
	SST25_R_BLOCK(0 * 64, SST25_buffer,64);
	for(;i< 10;i++)
	{
		SST25_R_BLOCK(i * 64, SST25_buffer,64);
		SST25_buffer[64] = 0;

//		UART1_Put_String(SST25_buffer);
	}
//sect_clr(0);
//Bulk_clr();
//return;
//	for(i = 0;i< 2000;i++)
//		flashTestData[i] = 1;
	
//	SST25_R_BLOCK(0, SST25_buffer,100);
//	UART1_Put_String(SST25_buffer);
//sect_clr(0);
//	SST25_R_BLOCK(1000, SST25_buffer,100);
//	UART1_Put_String(SST25_buffer);

//   SST25_W_BLOCK(0, SST25_buffer,4096);	        //将册数数据写入到SST25VF016B的0页里
//   delay_ms(100);
//   SST25_R_BLOCK(0, SST25_buffer,4096);	        //从SST25VF016B的0页里读出数据

//for(i = 0;i< 128;i++)
//	flashTestData[i] =4;
//   SST25_W_BLOCK(256 * 15, flashTestData,128);	
//for(i = 0;i< 128;i++)
//    	flashTestData[i] = 5;	 
//SST25_W_BLOCK(256 * 15 + 128, flashTestData,128);	
//for(i = 0;i< 128;i++)
//	flashTestData[i] = 6;	 
//SST25_W_BLOCK(256 * 15 + 256, flashTestData,128);
//   delay_ms(100);
//   SST25_R_BLOCK(256 * 15, SST25_buffer,512);	
//	 SST25_buffer[500] = 0;
//	 UART1_Put_String(SST25_buffer);
	
}

