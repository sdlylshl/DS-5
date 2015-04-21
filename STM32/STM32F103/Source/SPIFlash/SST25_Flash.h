#ifndef __SST25_FLASH_H
#define __SST25_FLASH_H

#include "stdint.h"
#include "stm32f10x.h"
#include "../System/SPI/spi1.h"
#include "../System/SPI/spi2.h"

//***********SST25 SST25 区别*******************************************
//对于SST25 和SST25 最大的区别在页写上面
//1.SST25 0x02 字节写入 每次只能写入一个字节的数据
//2.SST25 0xAD SST25如果想页写需要使用单独的指令AAI 页大小没有限制
//3.SST25  0x02 页写   每次既可以写入一个自己亦可以写入一页，页大小为固定256
//*********************************************************************

#define SST25_16M_SIZE 	0x200000	/*(16/8)*1024*1024*/

#define SST25_PAGE_SIZE 	512
#define SST25_SECTOR_SIZE (4*1024)
#define SST25_BLOCK_SIZE (64*1024)
#define SST25_CLUSTOR_SIZE SST25_BLOCK_SIZE
#define SST25_SECTOR_COUNT 512 /*(SST25_16M_SIZE/SST25_SECTOR_SIZE)*/
#define SST25_BLOCK_COUNT 32 /*(SST25_16M_SIZE/SST25_BLOCK_SIZE)*/

#define SST25_ReadData			      0x03 /*读数据 24位地址自动增加*/
#define SST25_ReadData_Fast		    0x0B /*高速读*/
//#define SST25_FastReadDual		  0x3B /*高速读*/
#define SST25_ByteProgram		      0x02 /*编程一个数据字节*/
#define SST25_PageProgram		      0xAD /*编程AAI-Word-Program,*/
#define SST25_SectorErase_4K      0x20 /*擦除4K SECTOR块的存储器阵列*/
#define SST25_BlockErase_32K			0x52 /*擦除32K BLOCK块的存储器阵列*/
#define SST25_BlockErase_64K			0xD8 /*擦除64K BLOCK块的存储器阵列*/
#define SST25_ChipErase			      0xC7 /*或者0x60 Chip擦除全部存储器阵列*/
#define SST25_WriteEnable		      0x06 /*写使能*/
#define SST25_WriteDisable		    0x04 /*写禁止*/
#define SST25_ReadStatusReg		    0x05 /*读取状态寄存器*/
#define SST25_WriteStatusReg		  0x01 /*写状态寄存器*/
#define SST25_ReleasePowerDown	  0xAB /*读ID 或者0x90*/
#define SST25_DeviceID			      0xAB
#define SST25_ManufactDeviceID   	0x90 /*读ID*/
#define SST25_JedecDeviceID		    0x9F /*JEDEC ID 读*/
#define SST25_EnableWriteStatusReg		  0x50 /*写状态寄存器*/

#define SST25_EnableSOBY		  			0x70 /*AAI programming*/
#define SST25_DisableSOBY		  			0x80 /*AAI programming*/
//#define Dummy_Byte 0
//是否修改SPI默认 NSS



//*********************** 接口实现 1************************************
//	终端面板 SPI1 + GPC13           security terminal V1.3 
#define SST25_NSS_REMAP	

#define SST25_SPI_init 			SPI1_Init
#define SST25_SPI_SendByte 	SPI1_SendByte
#define SST25_SPI_ReadByte 	SPI1_ReceiveByte

//*********************** 接口实现 2************************************

////	停车场 SPI2                    ParkinV2.0

//#define SST25_SPI_init 			SPI2_Init
//#define SST25_SPI_SendByte 	SPI2_SendByte
//#define SST25_SPI_ReadByte 	SPI2_ReceiveByte

//*************************************************************************************
#ifdef  SST25_NSS_REMAP
#define SST25_SCS_PORT			GPIOC
#define SST25_SCS_PIN				GPIO_Pin_13
#define SST25_SCS_MODE			GPIO_Mode_Out_PP	//此处一定要PP输出否则程序不正常
#define SST25_SCS_HIGH()    GPIO_SetBits(SST25_SCS_PORT, SST25_SCS_PIN)
#define SST25_SCS_LOW()			GPIO_ResetBits(SST25_SCS_PORT, SST25_SCS_PIN)
#define SST25_Select 				SST25_SCS_LOW
#define SST25_DeSelect 			SST25_SCS_HIGH
#else
//#define SST25_Select 				SPI1_SCS_LOW
//#define SST25_DeSelect 			SPI1_SCS_HIGH
#define SST25_Select 				SPI2_SCS_LOW
#define SST25_DeSelect 			SPI2_SCS_HIGH
#endif

//*************************************************************************************
extern void SST25_Flash_init(void);
extern uint8_t FlashReadStatus(void); //读状态寄存器
extern void FlashWaitBusy(void);	//忙检测extern void FlashWriteEnable(void);	//写使能
extern void FlashWriteDisable(void);//写禁止
extern void FlashWriteStatus(void);	//写状态
extern void FlashSectorErase(uint32_t a1); //页擦除
extern void FlashChipErase(void);	//
extern uint16_t FlashReadID(void);		        //读取flashID四个字节

void SST25_BufferRead_HighSpeed(uint8_t *readbuff, uint32_t addr, uint16_t NumByteToRead);
void SST25_ByteWrite(uint32_t addr, uint8_t data);
void SST25_BufferWrite(uint8_t *pbuff, uint32_t addr, uint16_t NumByteToWrite);

void testFlash(void );

//FATFS interface
uint8_t		SST25_disk_status(void);
uint8_t		SST25_disk_initialize(void);
uint8_t		SST25_disk_read(uint8_t *buff,		/* Data buffer to store read data */
													uint16_t	sector,	/* Sector address in LBA */
													uint32_t	count		/* Number of sectors to read */);
uint8_t		SST25_disk_write(uint8_t *buff,		/* Data buffer to store read data */
													uint16_t	sector,	/* Sector address in LBA */
													uint32_t	count		/* Number of sectors to read */);


#endif
