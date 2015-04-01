/****************************************Copyright (c)**************************************************
**                                  vixa
**
**--------------文件信息--------------------------------------------------------------------------------
**文   件   名: Spi_FRAM.c
**创   建   人: Xsky
**创 建 日  期: 2011-03-21 16:36
**描        述: SPI FRAM,FM25L16, 驱动函数, STM32F10x
** 版  本: v1.0.00
**------------------------------------------------------------------------------------------------------
** 相关文件: Spi_FRAM.h
**			 
**			 
**-------------------------------------------------------------------------------------------------------
**
**------------------------------------------------------------------------------------------------------
** 最后修改日期: 
** 修改人: Xsky
** 版本: v1.0.00
**-------------------------------------------------------------------------------------------------------
** 修改人: Xsky
** 日期: 
** 版本: v1.0.00
** 修改: 
**			1. 
**			2. 
**			3. 
**			4. 
**			5. 
********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#define  SPI_FRAM_GLOBALS
#include "Global.h"
#include "stm32f10x.h"
#include "STM32_Init.h"
#include "Spi_FRAM.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Const define  -------------------------------------------------------------*/
#define RDSR  	0x05 /* 读状态寄存器指令 */
#define WRSR  	0x01 /* 写状态寄存器指令 */
#define WREN  	0x06 /* 设置写允许寄存器指令 */
#define WRDI  	0x04 /* 写禁止 */
#define READ  	0x03 /* 读数据 */
#define WRITE	0x02 /* 写数据 */

#define FLG_WPEN	0x80	/* controls the effect of the hardware /WP pin. */
#define FLG_WEL		0x02	/* The WEL flag indicates the state of the Write Enable Latch */
#define FLG_BP1		0x80	/* 写保护选择位 */
#define FLG_BP0		0x04	/* 写保护选择位 */

#define FLG_Protected_None	0x00	/* 不保护 */


//#define RXNE    0x01
//#define TXE     0x02
//#define BSY     0x80
//extern void Delay(u32 nTime);

/*
void FRAM_CS_LOW(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_5);
	GPIO_SetBits(GPIOB, GPIO_Pin_1);
}

void FRAM_CS_HIGH(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_5);
	GPIO_ResetBits(GPIOB, GPIO_Pin_1);
}
*/

/*******************************************************************************
* Function Name  : spi_read_one_byte
* Description    : Read a byte from the SPI.
* Input          : None.
* Output         : None
* Return         : The received byte.
*******************************************************************************/
static u8 spi_read_one_byte(void)
{
	u8 Data = 0;
	
	while (SPI_I2S_GetFlagStatus(SPI_FRAM, SPI_I2S_FLAG_TXE) == RESET);
	/* Send the byte */
	SPI_I2S_SendData(SPI_FRAM, 0xff);
	
	/* Wait until a data is received */
	while (SPI_I2S_GetFlagStatus(SPI_FRAM, SPI_I2S_FLAG_RXNE) == RESET);
	/* Get the received data */
	Data = SPI_FRAM->DR;		// SPI_I2S_ReceiveData(SPI_FRAM);
	
	//  while ((SPI1->SR & BSY) == 0x80);
	
	/* Return the shifted data */
	return Data;
}

/*******************************************************************************
* Function Name  : spi_write_one_byte
* Description    : Send one byte by SPI2
* Input          : outb--the byte to be sended 
* Output         : None
* Return         : None
*******************************************************************************/
static void spi_write_one_byte(unsigned char outb)
{  
	/* Wait until the transmit buffer is empty */
	while (SPI_I2S_GetFlagStatus(SPI_FRAM, SPI_I2S_FLAG_TXE) == RESET);
	/* Send the byte */
	SPI_I2S_SendData(SPI_FRAM, outb);
	
	/* Wait to receive a byte */
	while (SPI_I2S_GetFlagStatus(SPI_FRAM, SPI_I2S_FLAG_RXNE) == RESET);
	
 	//while ((SPI_FRAM->SR & BSY) == 0x80);
	//while ((SPI_FRAM->SR & BSY) == 0x80);
	//while ((SPI_FRAM->SR & BSY) == 0x80);
	
}

/*******************************************************************************	
* Function Name  : WrEnable
* Description    : 写命令使能
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void FRAM_WrEnable(void)
{
	FRAM_CS_LOW();
	spi_write_one_byte(WREN);//SPI_I2S_SendData(SPI1, WREN);
	FRAM_CS_HIGH();
}

/*******************************************************************************	
* Function Name  : WrDisnable
* Description    : 失能写命
* Input          : None
* Output         : None
* Return         : None
static void FRAM_WrDisnable(void)
{
	FRAM_CS_LOW();
	spi_write_one_byte(WRDI);//SPI_I2S_SendData(SPI1, WRDI);
	FRAM_CS_HIGH();
}
*******************************************************************************/

/*******************************************************************************	
* Function Name  : WrStateReg
* Description    : 写状态寄存器
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void FRAM_WrStatReg(u8 stat)		 //写状态寄存器
{
	FRAM_WrEnable();
	FRAM_CS_LOW();
	spi_write_one_byte(WRSR);//SPI_I2S_SendData(SPI1, WRSR);
	spi_write_one_byte(stat);//SPI_I2S_SendData(SPI1, com);
	FRAM_CS_HIGH();
}

#if 1==0
static u8 FRAM_RdStatReg(void)			   //读状态寄存器
{
	u8 temp;
	for(temp=0;temp<100;temp++)
	;
	FRAM_CS_LOW();
	spi_write_one_byte(RDSR);//SPI_I2S_SendData(SPI1, RDSR);
	temp=spi_read_one_byte();//SPI_I2S_ReceiveData(SPI1);
	FRAM_CS_HIGH();
	return temp;
}				   
#endif

/*************************************************************
函数功能: 将数据写入FLASH
入口参数:	unsigned int add	地址（低13位有效）
				unsigned int len 数据长度
				unsigned int *p	写入数据首地址指针
*************************************************************/
void FRAM_Write(uint16 add,void *pSrc,uint32 len)
{
	unsigned int i;
	register uint8 *p = (uint8 *)pSrc;
	
	//CS_RESET;
	//spi_write_one_byte(0x06);	//WREN使能 允许操作码写操作
	//CS_SET;
	
	FRAM_CS_LOW();
	spi_write_one_byte(0x02);		//写存储器数据
	spi_write_one_byte(add>>8);		//写地址高八位
	spi_write_one_byte(add);		//写地址第八位	
	for (i=0;i<len;i++)
	{
		spi_write_one_byte(p[i]);	//连续写入长度为len的数据
	}
	FRAM_CS_HIGH();
	
}

/*************************************************************
函数功能: 清零flash
*************************************************************/
void FRAM_Zero(void)
{
	unsigned int i;
	
	//CS_RESET;
	//spi_write_one_byte(0x06);	//WREN使能 允许操作码写操作
	//CS_SET;
	
	FRAM_CS_LOW();
	spi_write_one_byte(0x02);	//写存储器数据
	spi_write_one_byte(0);		//写地址高八位
	spi_write_one_byte(0);		//写地址第八位	
	for (i=0;i<8192;i++)
	{
		spi_write_one_byte(0);	//连续写入长度为len的数据
	}
	FRAM_CS_HIGH();
	
}

/*************************************************************
函数功能: 将数据从FLASH读出
入口参数:	unsigned int add	地址（低13位有效）
				unsigned int len 数据长度
				unsigned int *p	读出数据首地址指针
*************************************************************/
void FRAM_Read(void *pDst, uint16 add,uint32 len)
{
	unsigned int i;
	register uint8 *p = (uint8 *)pDst;
	
	FRAM_CS_LOW();
	spi_write_one_byte(0x03);		//写存储器数据
	spi_write_one_byte(add>>8);	//写地址高八位
	spi_write_one_byte(add);		//写地址第八位	
	for (i=0;i<len;i++)
	{
		p[i] = spi_read_one_byte();	//连续读出长度为len的数据
	}
	FRAM_CS_HIGH();
}

/*******************************************************************************	
* Function Name  : FRAM_Config
* Description    : 初始化设置SPI引脚及SPI控制器参数
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void FRAM_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	SPI_InitTypeDef   SPI_InitStructure;

	/* GPIOA and GPIOC Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_FRAM_GPIO, ENABLE);
	/* SPI1 Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_FRAM_SPI, ENABLE);

	/* Configure SPI1 pins: SCK, MISO and MOSI */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_FRAM_SCK | 
								  GPIO_Pin_FRAM_SI  | GPIO_Pin_FRAM_SO;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_FRAM;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIO_FRAM, &GPIO_InitStructure);

	/* Configure PB5 pin: Fram CS pin */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_FRAM_CS;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_FRAM;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIO_FRAM_CS, &GPIO_InitStructure);
	FRAM_CS_HIGH();

	#ifdef GPIO_Remap_FRAM
	#errror	"Spi_FRAM.c Fram_Config(): Except SPI FRAM GPIO Remap Code!"
	#endif
	
	/* SPI1 Config */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_FRAM_DataSize;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;				// Datasheet.CN:p461
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_FRAM_BaudRatePrescaler;	// 36MHz / 9
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI_FRAM, &SPI_InitStructure);
	
	/* SPI1 enable */
	SPI_Cmd(SPI_FRAM, ENABLE);
	
	/* FRAM配置 */
	/* 允许写 */
	FRAM_WrEnable();
	/* 写状态寄存器,无保护区域 */
	FRAM_WrStatReg(FLG_WPEN | FLG_WEL);
	
}
