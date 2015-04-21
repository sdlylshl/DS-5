#ifndef __W25X_FLASH_H
#define __W25X_FLASH_H

#include "stm32f10x.h"
#include "../System/SPI/spi1.h"
#include "../System/SPI/spi2.h"
//#define SPI_FLASH_PageSize      4096
#define SPI_FLASH_PageSize      256
#define SPI_FLASH_PerWritePageSize      256

#define W25X_16_SIZE 		(16/8)*1024*1024
//#define W25X_32_SIZE 		(32/8)*1024*1024
//#define W25X_64_SIZE 		(64/8)*1024*1024

#define W25X_PAGE_SIZE 		256
#define W25X_SECTOR_SIZE (4*1024)
#define W25X_BLOCK_SIZE (64*1024)
#define W25X_CLUSTOR_SIZE W25X_BLOCK_SIZE
//W25X_16 SECTOR区数量 512
#define W25X_SECTOR_COUNT (W25X_16_SIZE/W25X_SECTOR_SIZE)
//W25X_16 BLOCK块数量 32
#define W25X_BLOCK_COUNT (W25X_16_SIZE/W25X_BLOCK_SIZE)

//***********W25X W25X 区别*******************************************
//对于W25X 和W25X 最大的区别在页写上面
//1.W25X 0x02 字节写入 每次只能写入一个字节的数据
//2.W25X 0xAD W25X如果想页写需要使用单独的指令AAI 页大小没有限制
//3.W25X  0x02 页写   每次既可以写入一个自己亦可以写入一页，页大小为固定256
//*********************************************************************

/* Private define ------------------------------------------------------------*/
#define W25X_WriteEnable		      0x06 //写使能
#define W25X_WriteDisable		      0x04 //写禁止
#define W25X_ReadStatusReg		    0x05 //读取状态寄存器 
#define W25X_WriteStatusReg		    0x01 //写状态寄存器
#define W25X_ReadData			        0x03 //读数据 24位地址自动增加
#define W25X_FastReadData		      0x0B //高速读
#define W25X_FastReadDual		      0x3B //高速读
#define W25X_ByteProgram		      0x02 //编程一个数据字节
#define W25X_PageProgram_256      0x02 //编程一页数据,最大256
#define W25X_SectorErase_4K	      0x20 //擦除4K SECTOR块的存储器阵列
#define W25X_BlockErase_64K	      0xD8 //擦除64K BLOCK块的存储器阵列
#define W25X_ChipErase			      0xC7 //或者0x60 Chip擦除全部存储器阵列
#define W25X_PowerDown			      0xB9
#define W25X_ReleasePowerDown	    0xAB //读ID 或者0x90
#define W25X_DeviceID			        0xAB
#define W25X_ManufactDeviceID   	0x90 //读ID
#define W25X_JedecDeviceID		    0x9F //JEDEC ID 读

#define WIP_Flag                  0x01  /* Write In Progress (WIP) flag */

#define Dummy_Byte                0x00

//*********************** 接口实现 1************************************
//	终端面板 SPI1 + GPC13 security terminal V1.3 
#define W25X_NSS_REMAP	

#define SPI_FLASH_Init					SPI1_Init
#define SPI_FLASH_SendByte 			SPI1_SendByte
#define SPI_FLASH_ReadByte 			SPI1_ReceiveByte

//*********************** 接口实现 2************************************
////	停车场 SPI2         ParkinV2.0

//#define SPI_FLASH_Init					SPI2_Init
//#define SPI_FLASH_SendByte 			SPI2_SendByte
//#define SPI_FLASH_ReadByte 			SPI2_ReceiveByte

//*************************************************************************************
#ifdef  W25X_NSS_REMAP
#define W25X_SCS_PORT				GPIOC
#define W25X_SCS_PIN				GPIO_Pin_13
#define W25X_SCS_MODE				GPIO_Mode_Out_PP	//此处一定要PP输出否则程序不正常
#define W25X_SCS_HIGH()    	GPIO_SetBits(W25X_SCS_PORT, W25X_SCS_PIN)
#define W25X_SCS_LOW()			GPIO_ResetBits(W25X_SCS_PORT, W25X_SCS_PIN)
#define SPI_FLASH_CS_LOW 		W25X_SCS_LOW
#define SPI_FLASH_CS_HIGH 	W25X_SCS_HIGH
#else
//#define SPI_FLASH_CS_LOW 		SPI1_SCS_LOW
//#define SPI_FLASH_CS_HIGH 	SPI1_SCS_HIGH
#define SPI_FLASH_CS_LOW 		SPI2_SCS_LOW
#define SPI_FLASH_CS_HIGH 	SPI2_SCS_HIGH
#endif

//***********************************************************************************
//uint8_t SPI_FLASH_ReadByte(void);
//uint8_t SPI_FLASH_SendByte(uint8_t byte);
//uint16_t SPI_FLASH_SendHalfWord(uint16_t HalfWord);

void W25X_FLASH_Init(void);
void W25X_Flash_Test(void);
void SPI_FLASH_WriteEnable(void);
void SPI_FLASH_WriteDisable(void);
void SPI_FLASH_StartReadSequence(uint32_t ReadAddr);
void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
void SPI_FLASH_BufferRead_Fast(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
void SPI_FLASH_ByteWrite(uint32_t WriteAddr ,uint8_t data);
void SPI_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void SPI_FLASH_SectorErase(uint32_t SectorAddr);
void SPI_FLASH_BlockErase(uint32_t BlockAddr);
void SPI_FLASH_BulkErase(void);
void SPI_Flash_PowerDown(void);
uint32_t SPI_FLASH_ReadID(void);
uint32_t SPI_FLASH_ReadDeviceID(void);
void SPI_Flash_WAKEUP(void);
void SPI_FLASH_WaitForWriteEnd(void);

#endif /* __SPI_FLASH_H */

