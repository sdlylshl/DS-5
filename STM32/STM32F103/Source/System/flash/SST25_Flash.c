#include "SST25_Flash.h"
//uint8_t * flashTestData = "1234567890";
uint8_t SST25_buffer[4096] = {0};
uint16_t logNr = 2; //当前记录编号 
//2048K/4=512 page
#define SST25_PAGE_SIZE 512
#define SST25_SECTOR_SIZE 4096
#define SST25_SECTOR_COUNT (0x200000/SST25_SECTOR_SIZE)
#define SST25_BLOCK_SIZE (4096/SST25_SECTOR_SIZE)
#define SST25_CLUSTOR_SIZE SST25_BLOCK_SIZE

/****************************************************************************
* 名    称：unsigned char rdsr(void)
* 功    能：读状态寄存器
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无
****************************************************************************/
uint8_t FlashReadStatus(void){
	unsigned char busy;
	Select_Flash();
	SPI_Flash_SendByte(0x05);
	busy = SPI_Flash_ReadByte();
	DeSelect_Flash();
	return(busy);

}
/****************************************************************************
* 名    称：void wip(void)
* 功    能：忙检测
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无
****************************************************************************/
void FlashWaitBusy(void){
	unsigned char a = 1;
	while ((a & 0x01) == 1) a = FlashReadStatus();

}
/****************************************************************************
* 名    称：void wen(void)
* 功    能：写使能
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void FlashWriteEnable(void){
	Select_Flash();
	SPI_Flash_SendByte(0x06);
	DeSelect_Flash();
}
/****************************************************************************
* 名    称：void wdis(void)
* 功    能：写禁止
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void FlashWriteDisable(void){

	Select_Flash();
	SPI_Flash_SendByte(0x04); 
	DeSelect_Flash();
	FlashWaitBusy();
	
}	
/****************************************************************************
* 名    称：void wsr(void)
* 功    能：写状态
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/ 
void FlashWriteStatus(void){	
	Select_Flash();
	SPI_Flash_SendByte(0x50);
	DeSelect_Flash(); 
	Select_Flash();
	SPI_Flash_SendByte(0x01);
	SPI_Flash_SendByte(0x00); 
	DeSelect_Flash();
    FlashWaitBusy();
}
/****************************************************************************
* 名    称：void sect_clr(unsigned long a1)
* 功    能：页擦除
* 入口参数：uint32_t page--页4K
* 出口参数：无
* 说    明：
* 调用方法：无
****************************************************************************/
void FlashSectorErase(uint32_t sector){
	FlashWriteStatus();
	FlashWriteEnable();
	Select_Flash();
	SPI_Flash_SendByte(0x20);
	SPI_Flash_SendByte((sector & 0xffffff) >> 16);          //addh
	SPI_Flash_SendByte((sector & 0xffff) >> 8);          //addl 
	SPI_Flash_SendByte(sector & 0xff);                 //wtt
	DeSelect_Flash();
	FlashWaitBusy();

}
/****************************************************************************
* 名    称：void FlashBlockErase(uint32_t block)
* 功    能：Block擦除
* 入口参数：uint32_t page--页32K
* 出口参数：无
* 说    明：
* 调用方法：无
****************************************************************************/
void FlashBlockErase(uint32_t block){
	FlashWriteStatus();
	FlashWriteEnable();
	Select_Flash();
	SPI_Flash_SendByte(0x52);
	SPI_Flash_SendByte((block & 0xffffff) >> 16);          //addh
	SPI_Flash_SendByte((block & 0xffff) >> 8);          //addl 
	SPI_Flash_SendByte(block & 0xff);                 //wtt
	DeSelect_Flash();
	FlashWaitBusy();

}
/****************************************************************************
* 名    称：void sect_clr(unsigned long a1)
* 功    能：页擦除
* 入口参数：unsigned long a1--页
* 出口参数：无
* 说    明：
* 调用方法：无
****************************************************************************/
void FlashChipErase(){
	FlashWriteStatus();
	FlashWriteEnable();
	Select_Flash();
	SPI_Flash_SendByte(0xc7);
	DeSelect_Flash();
	FlashWaitBusy();

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
	fac_id = SPI_Flash_ReadByte();		          //BFH: 工程码SST
	dev_id = SPI_Flash_ReadByte();	              //41H: 器件型号SST25VF016B     
	DeSelect_Flash();
	return (fac_id << 8 | dev_id);
}
/****************************************************************************
* 名    称：void SST25_R_BLOCK(unsigned long addr, unsigned char *readbuff, unsigned int BlockSize)
* 功    能：页读
* 入口参数：unsigned long addr--页   unsigned char *readbuff--数组   unsigned int BlockSize--长度
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/  
void SST25_ReadHighSpeed(uint32_t addr, uint8_t *readbuff, uint16_t BlockSize){
	unsigned int i=0; 	
	Select_Flash();
	SPI_Flash_SendByte(0x0b);
	SPI_Flash_SendByte((addr&0xffffff)>>16);
	SPI_Flash_SendByte((addr&0xffff)>>8);
	SPI_Flash_SendByte(addr&0xff);
	SPI_Flash_SendByte(0);
	while(i<BlockSize){	
		readbuff[i]=0xFF^SPI_Flash_ReadByte();
		
		i++;
	}
	DeSelect_Flash();	 	
}

void SST25_WriteByte(uint32_t addr, uint8_t data){

	FlashWriteEnable();
	Select_Flash();
	SPI_Flash_SendByte(0x02);
	SPI_Flash_SendByte((addr & 0xffffff) >> 16);
	SPI_Flash_SendByte((addr & 0xffff) >> 8);
	SPI_Flash_SendByte(addr & 0xff);
	SPI_Flash_SendByte(0xFF^data);
	DeSelect_Flash();
	FlashWriteDisable();
//	Select_Flash();
//	FlashWaitBusy();
}
/****************************************************************************
* 名    称：void SST25_W_BLOCK(uint32_t addr, uint8_t *readbuff, uint16_t BlockSize)
* 功    能：页写
* 入口参数：uint32_t addr--页   uint8_t *readbuff--数组   uint16_t BlockSize--长度
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/  
void SST25_WriteAutoAddrIncrease(uint32_t addr, uint8_t *readbuff, uint16_t size){
	unsigned int i=0,a2;
	//sect_clr(addr);   								  //删除页		  
	FlashWriteStatus();
  	FlashWriteEnable();	
	Select_Flash();    
	SPI_Flash_SendByte(0xad);
	SPI_Flash_SendByte((addr&0xffffff)>>16);
	SPI_Flash_SendByte((addr&0xffff)>>8);
	SPI_Flash_SendByte(addr&0xff);
  	SPI_Flash_SendByte(0xFF^readbuff[0]);
	SPI_Flash_SendByte(0xFF^readbuff[1]);
	DeSelect_Flash();
	i=2;
	while(i<size){
		a2=120;
		while(a2>0) a2--;
		Select_Flash();
		SPI_Flash_SendByte(0xad);
		SPI_Flash_SendByte(0xFF^readbuff[i++]);
		SPI_Flash_SendByte(0xFF^readbuff[i++]);
		DeSelect_Flash();
	}
	
	a2=100;
	while(a2>0) a2--;
	FlashWriteDisable();	
	Select_Flash();	
	FlashWaitBusy();
}

uint8_t SST25_disk_status(void){
	return 0;
}
uint8_t SST25_disk_initialize(void){
	SPI2_Init();
	if (FlashReadID()== 0xBF41)
	{
		return 0;
	}else{
		return 1; //STA_NOINIT
	}
}
uint8_t SST25_disk_read(uint8_t *buff,		/* Data buffer to store read data */
	uint16_t sector,	/* Sector address in LBA */
	uint32_t count		/* Number of sectors to read */){

	 SST25_ReadHighSpeed(sector << 12, buff, SST25_SECTOR_SIZE);
		return 0;
}
uint8_t SST25_disk_write(uint8_t *pbuff,		/* Data buffer to store read data */
	uint16_t sector,	/* Sector address in LBA */
	uint32_t count		/* Number of sectors to read */){

		while (count --)
		{
			//FlashSectorErase(sector);
			SST25_WriteAutoAddrIncrease(sector << 12, pbuff, SST25_SECTOR_SIZE );
		}
		return 0;
}



void testFlash(void )
{
	int i = 0;
uint8_t flashTestData[512] = {0};
for ( i = 0; i < 100; i++)
{
	flashTestData[i] = i;
}
FlashReadID();

//FlashChipErase();
	
	SST25_ReadHighSpeed(0 * 64, SST25_buffer,64);
	for (i = 0; i < 100; i++)
	{
		SST25_buffer[i] = 0;
	}
	SST25_WriteByte(0x10,0xfe);
//SST25_WriteAutoAddrIncrease(0, flashTestData,64);
SST25_ReadHighSpeed(0 * 64, SST25_buffer, 64);

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

