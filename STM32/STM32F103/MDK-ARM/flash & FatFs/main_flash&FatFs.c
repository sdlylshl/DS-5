#include "stm32f10x.h"

#include "../../Source/System/USART/usart1.h"
#include "../../Source/SPIFlash/SST25_flash.h"
#include "../../Source/SPIFlash/W25x_flash.h"
//#include "../../Source/FatFs/R0.11/diskio.h"
#include "../../Source/FatFs/R0.10b/diskio.h"
#include "../../Source/FatFs/R0.10b/ff.h"
#include "../../Source/FatFs/R0.10b/ff_gen_drv.h"
#include "../../Source/FatFs/R0.10b/drivers/spi_diskio.h"
extern void filesysinit(void);
char read[512];	
void FatFs_Demo(){
char * middle ="hello world";
FRESULT res;      
char SPIpath[10];	  //用于保存需要的得到的盘符  0 ...  

FATFS   SPIFatFs;  
FATFS   *Fs;  
FIL     SPIfile;	
uint32_t  byteswritten; //写入的字节数           
uint32_t  readbytes; //写入的字节数  
volatile int num = 0;
DWORD clast;	
res = FATFS_LinkDriver(&SPI_Driver, SPIpath);

//if(res != FR_OK)
	//Error_Handler();   

res = f_mount(&SPIFatFs, (TCHAR const*)SPIpath, 0);

//if(res != FR_OK) 
//	Error_Handler();

//res = f_mkfs((TCHAR const*)SPIpath, 0, 0);     

//if(res != FR_OK) 
//	Error_Handler();
res = f_getfree((TCHAR const*)SPIpath,&clast,&Fs);


res = f_open(&SPIfile, "0:STM000.c", FA_OPEN_ALWAYS | FA_WRITE);

//if(res != FR_OK)
//	Error_Handler();

       

  num++;

    res = f_write(&SPIfile, middle, 4096, (void *)&byteswritten);

  
		//f_sync(&SPIfile);
            
f_close(&SPIfile);
res = f_open(&SPIfile, "0:STM000.c", FA_OPEN_ALWAYS);
f_read (&SPIfile,read,sizeof(read),&readbytes); 
f_close(&SPIfile);
FATFS_UnLinkDriver(SPIpath);

/* Infinite loop */

while (1)

{

}

}
int main(){
	USART1_NVIC_Config(7);
	USART1_Init();	
	//SPI2	
	//SST25_Flash_init();
	W25X_FLASH_Init();
	//SPI_FLASH_BufferRead(read, 0, 512);
	//SPI_FLASH_BulkErase();
	//SPI_FLASH_BufferRead(read, 0, 512);
//	filesysinit();

	FatFs_Demo();
}

void Delay_ms(uint32_t ms){

}
