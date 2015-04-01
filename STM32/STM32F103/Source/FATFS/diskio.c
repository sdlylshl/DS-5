/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stdio.h"
#include "../System/flash/SST25_flash.h"
#include "diskio.h"		/* FatFs lower layer API */
//#include "usbdisk.h"	/* Example: Header file of existing USB MSD control module */
//#include "atadrive.h"	/* Example: Header file of existing ATA harddisk control module */
//#include "sdcard.h"		/* Example: Header file of existing MMC/SDC contorl module */

/* Definitions of physical drive number for each drive */
#define ATA		0	/* Example: Map ATA harddisk to physical drive 0 */
#define MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
#define USB		2	/* Example: Map USB MSD to physical drive 2 */
#define SST25		3	/*SPI flash SST25*/		

BYTE filebuffer[4096];     // 

#define SST25_PAGE_SIZE 512
#define SST25_SECTOR_SIZE 4096
#define SST25_SECTOR_COUNT (0x200000/SST25_SECTOR_SIZE)
#define SST25_BLOCK_SIZE (4096/SST25_SECTOR_SIZE)
#define SST25_CLUSTOR_SIZE SST25_BLOCK_SIZE



#include "ff.h"
#include "diskio.h"
FATFS *fs;	
FATFS flash_fs[_VOLUMES];                      //   工作区
FIL GBK16_FIL,EN16,UNI_GBK_FIL,GBK_UNI_FIL;   // 文件标志 
UINT flash_br;   //文件读/写字节计数  
FRESULT flash_res;           // FatFs 功能函数返回结果变量

	/********************************************************************************
*函数原型：void FLASH_GBK_Init(void)
*函数功能：GB UNI 各种编码初始化
*********************************************************************************/
void FLASH_GBK_Init(void)//gbk初始化 
{     
	  //初始化卡中的字符文件
		//flash_res = f_open(&UNI_GBK_FIL, "/uni2oem.sys", FA_OPEN_EXISTING | FA_READ); //以读的方式打开选中的文件	
	  //if (flash_res )    while(1);   /* 出错 */ 
		//flash_res = f_open(&GBK_UNI_FIL, "/oem2uni.sys", FA_OPEN_EXISTING | FA_READ); //以读的方式打开选中的文件	
	  //if (flash_res )    while(1);   /* 出错 */ 
		flash_res = f_open(&GBK16_FIL, "/st16.sys", FA_OPEN_EXISTING | FA_READ); //以读的方式打开选中的文件	
	  if (flash_res )    while(1);   /* 出错 */ 
		flash_res = f_open(&EN16, "/en16.sys", FA_OPEN_EXISTING | FA_READ); //以读的方式打开选中的文件	
	  if (flash_res )    while(1);   /* 出错 */ 
}
void filesysinit(){
	int i;
	uint32_t bw,br;
	//FlashChipErase();	//1.
	//读取sector0
	SST25_ReadHighSpeed(0, filebuffer,SST25_SECTOR_SIZE);
	for(i=0;i<SST25_SECTOR_SIZE;i++){
		if(!(i%16)){
			printf("\n");
		}
		printf("%x ",filebuffer[i]);

	}
	//flash_fs.drv = SST25;
	flash_res=f_mount(&flash_fs[3],"3:",1);							 //1.将文件系统设置到0区 立即映射
	//f_mkfs("3:",0,1);					//2.格式化
			flash_res=f_getfree("3:",&i,&fs);
			flash_res = f_open(&EN16, "3:file.bin", FA_CREATE_ALWAYS | FA_WRITE);
	for(i=0;i<100;i++){
	filebuffer[i]=i;
	}		
			flash_res = f_write(&EN16, filebuffer, 100, &bw);   
			f_close(&EN16);
			flash_res = f_open(&EN16, "3:file.bin", FA_OPEN_EXISTING | FA_READ);
for(i=0;i<100;i++){
	filebuffer[i]=0;
	}			
			flash_res = f_read(&EN16, filebuffer, 100, &br); 

	SST25_ReadHighSpeed(0, filebuffer,SST25_SECTOR_SIZE);
	for(i=0;i<SST25_SECTOR_SIZE;i++){
		if(!(i%16)){
			printf("\n");
		}
		printf("%x ",filebuffer[i]);

	}
//	
//	FLASH_GBK_Init();
}

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) {
	case ATA :
		//result = ATA_disk_status();

		// translate the reslut code here

		return stat;

	case MMC :
		//result = MMC_disk_status();

		// translate the reslut code here

		return stat;

	case USB :
		//result = USB_disk_status();

		// translate the reslut code here

		return stat;
	case SST25:
		result = SST25_disk_status();

		// translate the reslut code here
		if (result)
		{
			stat = STA_NODISK;
		} 
		else
		{
			stat = STA_PROTECT;
		}
		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	DSTATUS stat;
	int result;

	switch (pdrv) {
	case ATA :
		//result = ATA_disk_initialize();

		// translate the reslut code here

		return stat;

	case MMC :
		//result = MMC_disk_initialize();

		// translate the reslut code here

		return stat;

	case USB :
		//result = USB_disk_initialize();

		// translate the reslut code here

		return stat;	
	case SST25:
		result = SST25_disk_initialize();

			// translate the reslut code here
			if (result)
			{
				stat = STA_NODISK;
			}
			else
			{
				//stat = STA_PROTECT;
				stat =0;
			}
			return stat;
	
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	case ATA :
		// translate the arguments here

		//result = ATA_disk_read(buff, sector, count);

		// translate the reslut code here

		return res;

	case MMC :
		// translate the arguments here

		//result = MMC_disk_read(buff, sector, count);

		// translate the reslut code here

		return res;

	case USB :
		// translate the arguments here

		//result = USB_disk_read(buff, sector, count);

		// translate the reslut code here

		return res;
	case SST25:
		result = SST25_disk_read(buff, sector, count);
		//RES_OK = 0,		/* 0: Successful */
		//RES_ERROR,		/* 1: R/W Error */
		//RES_WRPRT,		/* 2: Write Protected */
		//RES_NOTRDY,		/* 3: Not Ready */
		//RES_PARERR		/* 4: Invalid Parameter */
		// translate the reslut code here
		if (result)
		{

		}
		else
		{
			res = RES_OK;
		}
		return res;
	}

	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res;
	int result;

	switch (pdrv) {
	case ATA :
		// translate the arguments here

		//result = ATA_disk_write(buff, sector, count);

		// translate the reslut code here

		return res;

	case MMC :
		// translate the arguments here

		//result = MMC_disk_write(buff, sector, count);

		// translate the reslut code here

		return res;

	case USB :
		// translate the arguments here

		//result = USB_disk_write(buff, sector, count);

		// translate the reslut code here

		return res;
	case SST25:
		result = SST25_disk_write((uint8_t *)buff, sector, count);
		//RES_OK = 0,		/* 0: Successful */
		//RES_ERROR,		/* 1: R/W Error */
		//RES_WRPRT,		/* 2: Write Protected */
		//RES_NOTRDY,		/* 3: Not Ready */
		//RES_PARERR		/* 4: Invalid Parameter */
		// translate the reslut code here
		if (result)
		{

		}
		else
		{
			res = RES_OK;
		}
		return res;
	}

	return RES_PARERR;
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res = RES_OK;
	int result;

	switch (pdrv) {
	case ATA :

		// Process of the command for the ATA drive

		return res;

	case MMC :

		// Process of the command for the MMC/SD card

		return res;

	case USB :

		// Process of the command the USB drive

		return res;
	case SST25:
		switch (cmd)
		{
		case CTRL_SYNC:
			res = RES_OK;			
			break;
		case GET_BLOCK_SIZE:
			*(DWORD*)buff = 16;
			break;
		case GET_SECTOR_COUNT:	
			*(DWORD*)buff = SST25_SECTOR_COUNT;
			break;
		case GET_SECTOR_SIZE:/* Get internal block size in unit of sector */
			*(WORD*)buff = SST25_SECTOR_SIZE;
			break;
		default:
			res = RES_PARERR;
			break;
		}
		return res;
	}

	return RES_PARERR;
}
#endif

DWORD get_fattime(void){
	return 0;
}
