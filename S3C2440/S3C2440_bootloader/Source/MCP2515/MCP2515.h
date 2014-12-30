#ifndef __MCP2515__
#define __MCP2515__

#include "def.h"
#define rGPEDAT  (*(volatile unsigned *)0x56000044)
#define rGPECON  (*(volatile unsigned *)0x56000040)
#define  rGPEUP   (*(volatile unsigned *)0x56000048)

#define rGPGCON (*(volatile unsigned *)0x56000060)
#define rGPGDAT (*(volatile unsigned *)0x56000064)

/****************************************************************************
MCP2510_CS		GPG6		output		( nSS0 )
MCP2510_SI		GPE12		output		( SPIMOSI0 )
MCP2510_SO		GPE11		input		( SPIMISO0 )
MCP2510_SCK		GPE13		output		( SPICLK0 )
MCP2510_INT		GPG3		input		( EINT8 )
****************************************************************************/
#define MCP2510_DEBUG    1
#define DELAY_TIME		500

//#define MCP2510_CS_OUT		( rGPGCON = rGPGCON & (~(3<<12)) | (1<<12) )		//GPG6
//#define MCP2510_CS_H		( rGPGDAT = rGPGDAT | (1<<6) )
//#define MCP2510_CS_L		( rGPGDAT = rGPGDAT & (~(1<<6))  )

#define MCP2510_CS_OUT		( rGPGCON = rGPGCON & (~(3<<4)) | (1<<4) )		//GPG2
#define MCP2510_CS_H		( rGPGDAT = rGPGDAT | (1<<2) )
#define MCP2510_CS_L		( rGPGDAT = rGPGDAT & (~(1<<2))  )

#define MCP2510_SI_OUT		( rGPECON = rGPECON & (~(3<<24)) | (1<<24) )		//GPE12
#define MCP2510_SI_H		( rGPEDAT = rGPEDAT | (1<<12) )
#define MCP2510_SI_L		( rGPEDAT = rGPEDAT & (~(1<<12)) )

#define MCP2510_SCK_OUT		( rGPECON = rGPECON & (~(3<<26)) | (1<<26) )		//GPE13
#define MCP2510_SCK_H		( rGPEDAT = rGPEDAT | (1<<13) )
#define MCP2510_SCK_L		( rGPEDAT = rGPEDAT & (~(1<<13)) )

#define MCP2510_SO_IN		( rGPECON = rGPECON & (~(3<<22)) | (0<<22) )		//GPE11
#define MCP2510_SO_GET		( rGPEDAT & (1<<11) )	
#define MCP2510_SO_PULLUP		( rGPEUP = rGPEUP & (~(1<<11)) )
#define MCP2510_SO_DISPULLUP		( rGPEUP = rGPEUP | (1<<11) )

#if GPS
#define MCP2510_INT_IN		( rGPGCON = rGPGCON & (~(3<<2)) )		//GPG1 GPS一体机
#define MCP2510_INT_GET		( rGPGDAT & (0x01<<1 ) )	
#else
#define MCP2510_INT_IN		( rGPGCON = rGPGCON & (~(3<<6)) )		//GPG3 第五代电控
#define MCP2510_INT_GET		( rGPGDAT & (0x01<<3 ))	
#endif
/********************** MCP2510 Instruction *********************************/
#define MCP2510INSTR_RESET		0xc0		//复位为缺省状态，并设定为配置模式
#define MCP2510INSTR_READ		0x03		//从寄存器中读出数据
#define MCP2510INSTR_WRITE		0x02		//向寄存器写入数据
#define MCP2510INSTR_RTS		0x80		//启动一个或多个发送缓冲器的报文发送
#define MCP2510INSTR_RDSTAT		0xa0		//读取状态
#define MCP2510INSTR_BITMDFY	0x05		//位修改
//***************************************************************************


/********************** MCP2510 Regrister *********************************/
	// Register offsets into the transmit buffers.
#define MCP2510REG_TXBnCTRL			0
#define MCP2510REG_TXBnSIDH			1
#define MCP2510REG_TXBnSIDL			2
#define MCP2510REG_TXBnEID8			3
#define MCP2510REG_TXBnEID0			4
#define MCP2510REG_TXBnDLC			5
#define MCP2510REG_TXBnD0			6
#define MCP2510REG_TXBnD1			7
#define MCP2510REG_TXBnD2			8
#define MCP2510REG_TXBnD3			9
#define MCP2510REG_TXBnD4			10
#define MCP2510REG_TXBnD5			11
#define MCP2510REG_TXBnD6			12
#define MCP2510REG_TXBnD7			13
#define MCP2510REG_CANSTAT			14
#define MCP2510REG_CANCTRL			15
	//
#define MCP2510LREG_SIDH			0
#define MCP2510LREG_SIDL			1
#define MCP2510LREG_EID8			2
#define MCP2510LREG_EID0			3


/******************* Bits in the TXBnCTRL registers.***************************/
#define TXB_TXBUFE_M    0x80
#define TXB_ABTF_M      0x40
#define TXB_MLOA_M      0x20
#define TXB_TXERR_M     0x10
#define TXB_TXREQ_M     0x08
#define TXB_TXIE_M      0x04
#define TXB_TXP10_M     0x03

#define DLC_MASK        0x0F
#define RTR_MASK        0x40


#define TXB0CTRL        0x30
#define TXB0SIDH        0x31

#define TXB1CTRL        0x40
#define TXB1SIDH        0x41

#define TXB2CTRL        0x50
#define TXB2SIDH        0x51

#define TXPRIOHIGH      0x03
#define TXPRIOHIGHLOW   0x02
#define TXPRIOLOWHIGH   0x01
#define TXPRIOLOW       0x00

#define TXB_EXIDE_M     0x08    // In TXBnSIDL
#define TXB_RTR_M       0x40    // In TXBnDLC

#define RXB_IDE_M       0x08    // In RXBnSIDL
#define RXB_RTR_M       0x40    // In RXBnDLC
//BFPCTRL——RXnBF 引脚控制寄存器和状态寄存器（地址：0Ch）
#define BFPCTRL         0x0C

#define B2RTS           0x20
#define B1RTS           0x10
#define B0RTS           0x08
#define B2RTSM          0x04
#define B1RTSM          0x02
#define B0RTSM          0x01

#define TEC             0x1C
#define REC             0x1D
#define CLKCTRL         MCP2510REG_CANCTRL


//RXFnSIDH 验收滤波寄存器n 标准标识符高位<10:3>
#define RXF0SIDH        0

//RXFnSIDL——验收滤波寄存器 n 标准标识符低位
//RXFnSIDL[7..5]SID：标准标识符滤波位 <2:0>
//RXFnSIDL[3]EXIDE：扩展标识符使能位
				//1 = 报文滤波仅应用于扩展帧
				//0 = 报文滤波仅应用于标准帧
//RXFnSIDL[1..0]EID：扩展标识符滤波位 <17:16>
//这些位装载了用来对接收报文中扩展标识符位<17:16>进行滤波判断的滤波位				
#define RXF0SIDL        1

//RXFnEID8 验收滤波寄存器n 扩展标识符高位<15:8>
#define RXF0EID8        2
//RXFnEID0 验收滤波寄存器n 扩展标识符低位<7:0>
#define RXF0EID0        3
#define RXF1SIDH        4
#define RXF1SIDL        5
#define RXF1EID8        6
#define RXF1EID0        7
#define RXF2SIDH        8
#define RXF2SIDL        9
#define RXF2EID8        10
#define RXF2EID0        11

#define RXF3SIDH        16
#define RXF3SIDL        17
#define RXF3EID8        18
#define RXF3EID0        19
#define RXF4SIDH        20
#define RXF4SIDL        21
#define RXF4EID8        22
#define RXF4EID0        23
#define RXF5SIDH        24
#define RXF5SIDL        25
#define RXF5EID8        26
#define RXF5EID0        27

#define RXF_EXIDE_M     0x08


//RXMnSIDH 验收屏蔽寄存器 n 标准标识符高位<10:3>
#define RXM0SIDH        0x20
#define RXM1SIDH        0x24

//RXMnSIDL 验收屏蔽滤波器 n 标准标识符低位<2:0>
//RXMnSIDL[7..5]=SID：标准标识符屏蔽位 <2:0>
//RXMnSIDL[1.0] =EID：扩展标识符屏蔽位 <17:16>
//这些位装载了用来对接收报文中扩展标识符位<17:16>进行屏蔽判断的屏蔽位
#define RXM0SIDL        0x21
#define RXM1SIDL        0x25

//RXMnEID8——验收屏蔽寄存器 n 扩展标识符高位<15:8>
#define RXM0EID8        0x22
#define RXM1EID8        0x26

//RXMnEID0——验收屏蔽寄存器 n 扩展标识符低位<7:0>
#define RXM0EID0        0x23
#define RXM1EID0        0x27


#define CNF3            0x28
#define CNF2            0x29
#define CNF1            0x2A
#define CANINTE         0x2B
#define CANINTF         0x2C
#define EFLG            0x2D
#define TXRTSCTRL       0x0D

//EFLG 错误标志寄存器（地址：2Dh）
	#define EFLG_RX1OVR     0x80
	#define EFLG_RX0OVR     0x40
	#define EFLG_TXBO       0x20
	#define EFLG_TXEP       0x10
	#define EFLG_RXEP       0x08
	#define EFLG_TXWAR      0x04
	#define EFLG_RXWAR      0x02
	#define EFLG_EWARN      0x01

//CNF1
	//[7..6]
	#define SJW1            0x00
	#define SJW2            0x40
	#define SJW3            0x80
	#define SJW4            0xC0
	//[5..0]	
	#define BRP1            0x00
	#define BRP2            0x01
	#define BRP3            0x02
	#define BRP4            0x03
	#define BRP5            0x04
	#define BRP6            0x05
	#define BRP7            0x06
	#define BRP8            0x07
//CNF2
	//[7]
	#define BTLMODE_CNF3    0x80
	//[6]
	#define SAMP1           0x00
	#define SAMP3           0x40
	//[5..3]
	#define SEG1            0x00
	#define SEG2            0x01
	#define SEG3            0x02
	#define SEG4            0x03
	#define SEG5            0x04
	#define SEG6            0x05
	#define SEG7            0x06
	#define SEG8            0x07



//CANINTE 中断使能寄存器（地址：2Bh）

	#define IVRIE           0x80	//报文接收或发送期间发生错误时中断
	#define WAKIE           0x40	//CAN 总线上有活动时中断
	#define ERRIE           0x20	//EFLG错误条件变化时中断
	#define TX2IE           0x10	//发送缓冲器2 空中断使能位
	#define TX1IE           0x08
	#define TX0IE           0x04
	#define RX1IE           0x02	//RXB1装载报文时中断
	#define RX0IE           0x01
	#define NO_IE           0x00
//CANINTF 中断标志寄存器（地址：2Ch）
	#define IVRINT          0x80
	#define WAKINT          0x40
	#define ERRINT          0x20
	#define TX2INT          0x10
	#define TX1INT          0x08
	#define TX0INT          0x04
	#define RX1INT          0x02
	#define RX0INT          0x01
	#define NO_INT          0x00

//RXB0CTRL
	#define RXB0CTRL        0x60
	#define RXB1CTRL        0x70

	#define RXB_RXRDY       0x80
	#define RXB_RXM1        0x40
	#define RXB_RXM0        0x20
	//RXB0CTRL[6.5]RXM：接收缓冲器工作模式位
	//11 = 关闭屏蔽／滤波功能；接收所有报文
	//10 = 只接收符合滤波器条件的带有扩展标识符的有效报文
	//01 = 只接收符合滤波器条件的带有标准标识符的有效报文
	//00 = 接收符合滤波器条件的所有带扩展标识符或标准标识符的有效报文
	#define RXB_RX_ANY      0x60
	#define RXB_RX_EXT      0x40
	#define RXB_RX_STD      0x20
	#define RXB_RX_STDEXT   0x00

	#define RXB_RXMx_M      0x60
	// #define RXB_RXIE_M      0x10

	//RXB0CTRL[3]RXRTR：接收到远程传送请求位
	//1 = 接收到远程传送请求
	//0 = 没有接收到远程传送请求
	#define RXB_RXRTR       0x08    // In RXBnCTRL

	//RXB0CTRL[2]BUKT：滚存使能位
	//1 = 如果RXB0 满，RXB0 接收到的报文将被滚存至 RXB1
	//0 = 滚存禁止
	#define RXB_BUKT        0x04
	#define RXB_BUKT_RO     0x02

	//FILHIT[0]：滤波器命中位指明使能报文接收的验收滤波寄存器编号
	//1 = 验收滤波寄存器1 （RXF1）
	//0 = 验收滤波寄存器0 （RXF0）
	#define RXB_FILHIT      0x01

	#define RXB_FILHIT2     0x04
	#define RXB_FILHIT1     0x02
	#define RXB_FILHIT_M    0x07
	#define RXB_RXF5        0x05
	#define RXB_RXF4        0x04
	#define RXB_RXF3        0x03
	#define RXB_RXF2        0x02
	#define RXB_RXF1        0x01
	#define RXB_RXF0        0x00

//CANCTRL——CAN控制寄存器（地址：XFh）

	//CLKEN[2]：CLKOUT引脚使能位
	//1 =CLKOUT引脚使能
	//0 =CLKOUT引脚禁止（引脚处于高阻态）
	#define CLKEN           0x04

	//CLKPRE<1:0>：CLKOUT引脚预分频比位
	//00 =FCLKOUT = 系统时钟频率/1
	//01 =FCLKOUT =系统时钟频率/2
	//10 =FCLKOUT = 系统时钟频率/4
	//11 =FCLKOUT = 系统时钟频率/8
	#define CLK1            0x00
	#define CLK2            0x01
	#define CLK4            0x02
	#define CLK8            0x03

	//REQOP<2:0>：请求工作模式的位
	//000 = 设定为正常工作模式
	//001 = 设定为休眠模式
	//010 = 设定为环回模式
	//011 = 设定为仅监听模式
	//100 = 设定为配置模式
	#define MODE_NORMAL     0x00
	#define MODE_SLEEP      0x20
	#define MODE_LOOPBACK   0x40
	#define MODE_LISTENONLY 0x60
	#define MODE_CONFIG     0x80
	#define ABORT           0x10

typedef enum{
	BandRate_10kbps,
	BandRate_125kbps,
	BandRate_250kbps,
	BandRate_500kbps,
	BandRate_1Mbps
}CanBandRate;


// Start the transmission from one of the tx buffers.
#define MCP2510_transmit(address)		do{MCP2510_WriteBits(address, TXB_TXREQ_M, TXB_TXREQ_M);}while(0)
//****************************************************************************
//全局变量
//extern U8 ntxbuffer;		//the Number of Tx Buffer 

/****************************************************************************
【功能说明】软件复位MCP2510
****************************************************************************/
void MCP2510_Reset(void);

/****************************************************************************
【功能说明】从MCP2510指定地址中读出一个字节
****************************************************************************/
unsigned char MCP2510_Read(U8 address);

/****************************************************************************
【功能说明】              Read often used status
//Status 	 7    	6    	5    	4    	3    	2  	1	0
//		|	|	|	|	|	|	|	|									
//		|	|	|	|	|	|	|	|___CANINTF.RX0IF
//		|	|	|	|	|	|	|_______CANINTF.RX1IF
//		|	|	|	|	|	|___________TXB0CTRL.TXREQ
//		|	|	|	|	|_______________CANINTF.TX0IF
//		|	|	|	|___________________TXB1CTRL.TXREQ
//		|	|	|_______________________CANINTF.TX1IF
//		|	|___________________________TXB2CTRL.TXREQ
//		|_______________________________CANINTF.TX2IF
****************************************************************************/
unsigned char MCP2510_ReadStatus(void);

/****************************************************************************
【功能说明】向MCP2510指定地址写入一个字节
****************************************************************************/
void MCP2510_Write( U8 address, U8 value);

/****************************************************************************
【功能说明】修改指定地址寄存器的某些位 要修改的为在MASK中指定为1
****************************************************************************/

//// Mask 1修改 0 不变
void MCP2510_WriteBits( U8 address, U8 data, U8 mask );

/****************************************************************************
【功能说明】序列读取MCP2510数据	数据长度为 nlength
****************************************************************************/
void MCP2510_SRead( U8 address, unsigned char* pdata, U8 nlength );

/****************************************************************************
【功能说明】序列写入MCP2510数据	数据长度为 nlength
****************************************************************************/
void MCP2510_Swrite(U8 address, unsigned char* pdata, U8 nlength);


void MCP2510_SetBandRate(CanBandRate bandrate, int IsBackNormal);

/***********************************************************\
*	读取MCP2510 接收的数据							*
*	参数: nbuffer为第几个缓冲区可以为3或者4	*
*			can_id为返回的ID值							*
*			rxRTR表示是否是RXRTR远程帧						*
*			data表示读取的数据						*
*			dlc表示data length code							*
*	返回值												*
*		TRUE，表示是扩展总线						*
*		FALSE，表示非扩展总线						*
\***********************************************************/
int MCP2510_Read_Can(U8 nbuffer, int* rxRTR, U32* can_id, U8* data , U8* dlc);

/***********************************************************\
*	写入MCP2510 发送的数据							*
*	参数: nbuffer为第几个缓冲区可以为0、1、2	*
*			ext表示是否是扩展总线					*
*			can_id为返回的ID值							*
*			rxRTR表示是否是RXRTR						*
*			data表示读取的数据						*
*			dlc表示data length code							*
*		FALSE，表示非扩展总线						*
\***********************************************************/
void MCP2510_Write_Can( U8 nbuffer, int ext, U32 can_id, int rxRTR, U8* data,U8 dlc );

/****************************************************************************
【功能说明】读取MCP2510 CAN总线ID
参数: address为MCP2510寄存器地址
	can_id为返回的ID值
返回值
TRUE，表示是扩展ID(29位)
FALSE，表示非扩展ID(11位)
****************************************************************************/
int MCP2510_Read_Can_ID( U8 address, U32* can_id);

/*******************************************\
*	设置MCP2510 CAN总线ID				*
*	参数: address为MCP2510寄存器地址*
*			can_id为设置的ID值			*
*			IsExt表示是否为扩展ID	*
\*******************************************/
void MCP2510_Write_Can_ID(U8 address, U32 can_id, int IsExt);
extern U32 		CanId;		//can_id为返回的ID值
extern U8  		dlc; 		//dlc表示data length code
//extern U32 		rxRTR;		//rxRTR表示是否是RXRTR远程帧
//extern U32 		isExt;
extern U8  		data_read[8] ;
extern U8  		data_write[8];
extern void Init_CANReciveINT(void);
#endif