/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
//#include "usbdisk.h"	/* Example: Header file of existing USB MSD control module */
//#include "atadrive.h"	/* Example: Header file of existing ATA harddisk control module */
//#include "sdcard.h"		/* Example: Header file of existing MMC/SDC contorl module */
#include "../../System/System_config.h"
/* Definitions of physical drive number for each drive */
#define ATA		0	/* Example: Map ATA harddisk to physical drive 0 */
#define MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
#define USB		2	/* Example: Map USB MSD to physical drive 2 */
#define SST25		3	/*SPI flash SST25*/		

BYTE filebuffer[4096];     // 
#define SST25_SECTOR_SIZE 4096
#define SST25_BLOCK_SIZE 512 



#include "ff.h"
#include "diskio.h"
FATFS flash_fs;                      // �߼��������ı�־
FIL GBK16_FIL,EN16,UNI_GBK_FIL,GBK_UNI_FIL;   // �ļ���־ 
UINT flash_br;   //�ļ���/д�ֽڼ���  
FRESULT flash_res;           // FatFs ���ܺ������ؽ������

	/********************************************************************************
*����ԭ�ͣ�void FLASH_GBK_Init(void)
*�������ܣ�GB UNI ���ֱ����ʼ��
*********************************************************************************/
void FLASH_GBK_Init(void)//gbk��ʼ�� 
{     
	  //��ʼ�����е��ַ��ļ�
		//flash_res = f_open(&UNI_GBK_FIL, "/uni2oem.sys", FA_OPEN_EXISTING | FA_READ); //�Զ��ķ�ʽ��ѡ�е��ļ�	
	  //if (flash_res )    while(1);   /* ���� */ 
		//flash_res = f_open(&GBK_UNI_FIL, "/oem2uni.sys", FA_OPEN_EXISTING | FA_READ); //�Զ��ķ�ʽ��ѡ�е��ļ�	
	  //if (flash_res )    while(1);   /* ���� */ 
		flash_res = f_open(&GBK16_FIL, "/st16.sys", FA_OPEN_EXISTING | FA_READ); //�Զ��ķ�ʽ��ѡ�е��ļ�	
	  if (flash_res )    while(1);   /* ���� */ 
		flash_res = f_open(&EN16, "/en16.sys", FA_OPEN_EXISTING | FA_READ); //�Զ��ķ�ʽ��ѡ�е��ļ�	
	  if (flash_res )    while(1);   /* ���� */ 
}
void filesysinit(){

	flash_res=f_mount(&flash_fs,"",1);							 //1.���ļ�ϵͳ���õ�0�� ����ӳ�� 
	FLASH_GBK_Init();
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
				stat = STA_PROTECT;
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

		if (pdrv){ return RES_PARERR; }

		switch (cmd)
		{
		case CTRL_SYNC:
			break;
		case GET_BLOCK_SIZE:
			*(DWORD*)buff = SST25_BLOCK_SIZE;
			break;
		case GET_SECTOR_COUNT:
			*(DWORD*)buff = SST25_BLOCK_SIZE;
			break;
		case GET_SECTOR_SIZE:
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