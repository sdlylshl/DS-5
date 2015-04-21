/**
  ******************************************************************************
  * @file    sd_diskio.c
  * @author  MCD Application Team
  * @version V1.2.1
  * @date    20-November-2014
  * @brief   SPI Disk I/O driver
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "../ff_gen_drv.h"
#include "spi_diskio.h"
#include "../../../Source/SPIFlash/SST25_flash.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Block Size in Bytes */
#define PAGE_SIZE                512
#define SECTOR_SIZE              PAGE_SIZE

/* Private variables ---------------------------------------------------------*/
/* Disk status */
static volatile DSTATUS Stat = STA_NOINIT;

/* Private function prototypes -----------------------------------------------*/
DSTATUS SPI_initialize (void);
DSTATUS SPI_status (void);
DRESULT SPI_read (BYTE*, DWORD, UINT);
#if _USE_WRITE == 1
  DRESULT SPI_write (const BYTE*, DWORD, UINT);
#endif /* _USE_WRITE == 1 */
#if _USE_IOCTL == 1
  DRESULT SPI_ioctl (BYTE, void*);
#endif  /* _USE_IOCTL == 1 */
  
Diskio_drvTypeDef  SPI_Driver =
{
  SPI_initialize,
  SPI_status,
  SPI_read, 
#if  _USE_WRITE == 1
  SPI_write,
#endif /* _USE_WRITE == 1 */
  
#if  _USE_IOCTL == 1
  SPI_ioctl,
#endif /* _USE_IOCTL == 1 */
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes a Drive
  * @param  None
  * @retval DSTATUS: Operation status
  */
DSTATUS SPI_initialize(void)
{
  Stat = STA_NOINIT;
  
	//SST25_Flash_init();
  /* Configure the uSPI device */
  if(FlashReadID()== 0xBF41)
  {
    Stat &= ~STA_NOINIT;
  }
	
  return Stat;
}

/**
  * @brief  Gets Disk Status
  * @param  None
  * @retval DSTATUS: Operation status
  */
DSTATUS SPI_status(void)
{
  Stat = STA_NOINIT;

//  if(BSP_SPI_GetStatus() == MSPI_OK)
//  {
//    Stat &= ~STA_NOINIT;
//  }
  
  return Stat;
}

/**
  * @brief  Reads Sector(s)
  * @param  *buff: Data buffer to store read data
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to read (1..128)
  * @retval DRESULT: Operation result
  */
DRESULT SPI_read(BYTE *buff, DWORD sector, UINT count)
{
	uint8_t num=0;
  DRESULT res = RES_OK;
	while(count--){
		num++;
  SST25_BufferRead_HighSpeed((uint8_t*) buff,   
														 (uint32_t)(num*sector*SECTOR_SIZE),                   
																			 SECTOR_SIZE);
	}
//  if(BSP_SPI_ReadBlocks((uint32_t*)buff, 
//                       (uint64_t) (sector * BLOCK_SIZE), 
//                       BLOCK_SIZE, 
//                       count) != MSPI_OK)
//  {
//    res = RES_ERROR;
//  }
  
  return res;
}

/**
  * @brief  Writes Sector(s)
  * @param  *buff: Data to be written
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to write (1..128)
  * @retval DRESULT: Operation result
  */
#if _USE_WRITE == 1
DRESULT SPI_write(const BYTE *buff, DWORD sector, UINT count)
{
	uint8_t num=0;
  DRESULT res = RES_OK;
	while(count--){
		num++;
  SST25_BufferWrite( (uint8_t*)buff, 
												(uint64_t)(num*sector *SECTOR_SIZE ),
                        SECTOR_SIZE);
	}
//  if(0 )
//  {
//		
//    res = RES_ERROR;
//  }
//  
  return res;
}
#endif /* _USE_WRITE == 1 */

/**
  * @brief  I/O control operation
  * @param  cmd: Control code
  * @param  *buff: Buffer to send/receive control data
  * @retval DRESULT: Operation result
  */
#if _USE_IOCTL == 1
DRESULT SPI_ioctl(BYTE cmd, void *buff)
{
  DRESULT res = RES_ERROR;
  
  if (Stat & STA_NOINIT) return RES_NOTRDY;
  
  switch (cmd)
  {
  /* Make sure that no pending write process */
  case CTRL_SYNC :
    res = RES_OK;
    break;
  
  /* Get number of sectors on the disk (DWORD) */
  case GET_SECTOR_COUNT :
    //BSP_SPI_GetCardInfo(&CardInfo);
    *(DWORD*)buff = ((16/8)*1024*1024)/SECTOR_SIZE;//CardInfo.CardCapacity / BLOCK_SIZE;
    res = RES_OK;
    break;
  
  /* Get R/W sector size (WORD) */
  case GET_SECTOR_SIZE :
    *(WORD*)buff = SECTOR_SIZE;
    res = RES_OK;
    break;
  
  /* Get erase block size in unit of sector (DWORD) */
  case GET_BLOCK_SIZE :
    *(DWORD*)buff = (SECTOR_SIZE/16);
    break;
  
  default:
    res = RES_PARERR;
  }
  
  return res;
}
#endif /* _USE_IOCTL == 1 */
  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

