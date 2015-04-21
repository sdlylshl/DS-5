
#include "SST25_Flash.h"
#include "stm32f10x_bkp.h"
#include "stm32f10x_rcc.h"

//uint8_t * flashTestData = "1234567890";
uint8_t SST25_buffer[4096] = { 0 };
uint16_t logNr = 2; //当前记录编号 
//2048K/4=512 page
//Flash片选初始化
void SST25_Flash_Select_init(){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
	RCC_LSEConfig(RCC_LSE_OFF); //关闭外部低速外部时钟信号功能 后，PC13 PC14 PC15 才可以当普通IO用。
	BKP_TamperPinCmd(DISABLE); //关闭入侵检测功能，也就是 PC13，也可以当普通IO 使用	


	GPIO_InitStructure.GPIO_Pin = SST25_SCS_PIN;
	GPIO_InitStructure.GPIO_Mode = SST25_SCS_MODE;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(SST25_SCS_PORT, &GPIO_InitStructure);



	SST25_Select();
	//	SST25_DeSelect();

}

void SST25_Flash_init(void){

	SST25_Flash_Select_init();
	SST25_SPI_init();
	FlashReadID();
}
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
	SST25_Select();
	SST25_SPI_SendByte(SST25_ReadStatusReg);
	busy = SST25_SPI_ReadByte();
	SST25_DeSelect();
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
	SST25_Select();
	SST25_SPI_SendByte(SST25_WriteEnable);
	SST25_DeSelect();
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

	SST25_Select();
	SST25_SPI_SendByte(SST25_WriteDisable);
	SST25_DeSelect();
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
	SST25_Select();
	SST25_SPI_SendByte(SST25_ReadStatusReg);
	SST25_DeSelect();
	SST25_Select();
	SST25_SPI_SendByte(SST25_WriteStatusReg);
	SST25_SPI_SendByte(0);
	SST25_DeSelect();
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
	SST25_Select();
	SST25_SPI_SendByte(SST25_SectorErase_4K);
	SST25_SPI_SendByte((sector & 0xffffff) >> 16);          //addh
	SST25_SPI_SendByte((sector & 0xffff) >> 8);          //addl 
	SST25_SPI_SendByte(sector & 0xff);                 //wtt
	SST25_DeSelect();
	FlashWaitBusy();

}
/****************************************************************************
* 名    称：void FlashBlockErase(uint32_t block)
* 功    能：Block擦除
* 入口参数：uint32_t page--页64K
* 出口参数：无
* 说    明：
* 调用方法：无
****************************************************************************/
void FlashBlockErase(uint32_t block){
	FlashWriteStatus();
	FlashWriteEnable();
	SST25_Select();
	SST25_SPI_SendByte(SST25_BlockErase_64K);
	SST25_SPI_SendByte((block & 0xffffff) >> 16);          //addh
	SST25_SPI_SendByte((block & 0xffff) >> 8);          //addl 
	SST25_SPI_SendByte(block & 0xff);                 //wtt
	SST25_DeSelect();
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
	SST25_Select();
	SST25_SPI_SendByte(SST25_ChipErase);
	SST25_DeSelect();
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
	SST25_Select();
	SST25_SPI_SendByte(SST25_ManufactDeviceID);
	SST25_SPI_SendByte(0x00);
	SST25_SPI_SendByte(0x00);
	SST25_SPI_SendByte(0x00);
	fac_id = SST25_SPI_ReadByte();		          //BFH: 工程码SST
	dev_id = SST25_SPI_ReadByte();	              //41H: 器件型号SST25VF016B     
	SST25_DeSelect();
	return (fac_id << 8 | dev_id);
}

void SST25_BufferRead(uint8_t *readbuff, uint32_t addr, uint16_t NumByteToRead){
	unsigned int i = 0;
	SST25_Select();
	SST25_SPI_SendByte(SST25_ReadData);
	SST25_SPI_SendByte((addr & 0xffffff) >> 16);
	SST25_SPI_SendByte((addr & 0xffff) >> 8);
	SST25_SPI_SendByte(addr & 0xff);
	while (i < NumByteToRead){
		readbuff[i] = 0xFF^SST25_SPI_ReadByte();

		i++;
	}
	SST25_DeSelect();
}

void SST25_BufferRead_HighSpeed(uint8_t *readbuff, uint32_t addr, uint16_t NumByteToRead)
{
	unsigned int i = 0;
	SST25_Select();
	SST25_SPI_SendByte(SST25_ReadData_Fast);
	SST25_SPI_SendByte((addr & 0xffffff) >> 16);
	SST25_SPI_SendByte((addr & 0xffff) >> 8);
	SST25_SPI_SendByte(addr & 0xff);
	SST25_SPI_SendByte(0);
	while (i < NumByteToRead){
		readbuff[i] = SST25_SPI_ReadByte() ^ 0xFF;
		i++;
	}
	SST25_DeSelect();
}

void SST25_ByteWrite(uint32_t addr, uint8_t data)
{


	FlashWriteEnable();	
	SST25_Select();
	SST25_SPI_SendByte(SST25_ByteProgram);
	SST25_SPI_SendByte((addr & 0xffffff) >> 16);
	SST25_SPI_SendByte((addr & 0xffff) >> 8);
	SST25_SPI_SendByte(addr & 0xff);
	SST25_SPI_SendByte(data ^ 0xFF);	
	SST25_DeSelect();
	FlashWriteDisable();
	//	SST25_Select();
	//	FlashWaitBusy();
}

void SST25_BufferWrite(uint8_t *pbuff, uint32_t addr, uint16_t NumByteToWrite)
{
	unsigned int i = 0, a2;
	//sect_clr(addr);   								  //删除页		  
	FlashWriteStatus();
	FlashWriteEnable();
	SST25_Select();
	SST25_SPI_SendByte(SST25_PageProgram);
	SST25_SPI_SendByte((addr & 0xffffff) >> 16);
	SST25_SPI_SendByte((addr & 0xffff) >> 8);
	SST25_SPI_SendByte(addr & 0xff);
	SST25_SPI_SendByte(pbuff[0] ^ 0xFF);
	SST25_SPI_SendByte(0xFF ^ pbuff[1] ^ 0xFF);
	SST25_DeSelect();
	i = 2;
	while (i < NumByteToWrite){
		for (a2 = 0; a2 < 120; a2++);
		SST25_Select();
		SST25_SPI_SendByte(SST25_PageProgram);
		SST25_SPI_SendByte(pbuff[i++] ^ 0xFF);
		SST25_SPI_SendByte(pbuff[i++] ^ 0xFF);
		SST25_DeSelect();
	}
	for (a2 = 0; a2 < 120; a2++);
	FlashWriteDisable();
	FlashWaitBusy();
	SST25_DeSelect();
}

uint8_t SST25_disk_status(void){
	return 0;
}
uint8_t SST25_disk_initialize(void){
	SPI2_Init();
	if (FlashReadID() == 0xBF41)
	{
		return 0;
	}
	else{
		return 1; //STA_NOINIT
	}
}
uint8_t SST25_disk_read(uint8_t *buff,		/* Data buffer to store read data */
	uint16_t sector,	/* Sector address in LBA */
	uint32_t count		/* Number of sectors to read */){
	SST25_BufferRead_HighSpeed(buff, sector << 12, SST25_SECTOR_SIZE);
	return 0;
}
uint8_t SST25_disk_write(uint8_t *pbuff,		/* Data buffer to store read data */
	uint16_t sector,	/* Sector address in LBA */
	uint32_t count		/* Number of sectors to read */){

	while (count--)
	{
		//FlashSectorErase(sector);
		SST25_BufferWrite(pbuff, sector << 12, SST25_SECTOR_SIZE);
	}
	return 0;
}



	#define BUFFSIZE 0x10
void testFlash(void)
{
	int i = 0;
	uint8_t flashTestData[BUFFSIZE] = { 0 };
	for (i = 0; i < BUFFSIZE; i++)flashTestData[i] = (uint8_t)i;
	FlashReadID();

	//FlashChipErase();
	SST25_BufferWrite(flashTestData,0,BUFFSIZE);
	
	SST25_ByteWrite(0x0, 0);
	for (i = 0; i < BUFFSIZE; i++)flashTestData[i] = 0;
	
	SST25_BufferRead(flashTestData, 0 , BUFFSIZE);
	SST25_BufferRead_HighSpeed(flashTestData, 0 , BUFFSIZE);
	//FlashSectorErase(0);
	
	for (i = 0; i < BUFFSIZE; i++)flashTestData[i] = 0;
	
	SST25_BufferRead_HighSpeed(flashTestData, 0 , BUFFSIZE);
	
	for (i = 0; i < 100; i++)
	{
		SST25_buffer[i] = 0;
	}
	SST25_ByteWrite(0x0, 0xFF);
	//SST25_WriteAutoAddrIncrease(0, flashTestData,64);
	SST25_BufferRead_HighSpeed(SST25_buffer, 0 * 64, 64);
	//printf("%d",flashTestData[0]);
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

