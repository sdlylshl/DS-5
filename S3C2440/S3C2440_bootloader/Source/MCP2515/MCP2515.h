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
#define MCP2510_INT_IN		( rGPGCON = rGPGCON & (~(3<<2)) )		//GPG1 GPSһ���
#define MCP2510_INT_GET		( rGPGDAT & (0x01<<1 ) )	
#else
#define MCP2510_INT_IN		( rGPGCON = rGPGCON & (~(3<<6)) )		//GPG3 ��������
#define MCP2510_INT_GET		( rGPGDAT & (0x01<<3 ))	
#endif
/********************** MCP2510 Instruction *********************************/
#define MCP2510INSTR_RESET		0xc0		//��λΪȱʡ״̬�����趨Ϊ����ģʽ
#define MCP2510INSTR_READ		0x03		//�ӼĴ����ж�������
#define MCP2510INSTR_WRITE		0x02		//��Ĵ���д������
#define MCP2510INSTR_RTS		0x80		//����һ���������ͻ������ı��ķ���
#define MCP2510INSTR_RDSTAT		0xa0		//��ȡ״̬
#define MCP2510INSTR_BITMDFY	0x05		//λ�޸�
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
//BFPCTRL����RXnBF ���ſ��ƼĴ�����״̬�Ĵ�������ַ��0Ch��
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


//RXFnSIDH �����˲��Ĵ���n ��׼��ʶ����λ<10:3>
#define RXF0SIDH        0

//RXFnSIDL���������˲��Ĵ��� n ��׼��ʶ����λ
//RXFnSIDL[7..5]SID����׼��ʶ���˲�λ <2:0>
//RXFnSIDL[3]EXIDE����չ��ʶ��ʹ��λ
				//1 = �����˲���Ӧ������չ֡
				//0 = �����˲���Ӧ���ڱ�׼֡
//RXFnSIDL[1..0]EID����չ��ʶ���˲�λ <17:16>
//��Щλװ���������Խ��ձ�������չ��ʶ��λ<17:16>�����˲��жϵ��˲�λ				
#define RXF0SIDL        1

//RXFnEID8 �����˲��Ĵ���n ��չ��ʶ����λ<15:8>
#define RXF0EID8        2
//RXFnEID0 �����˲��Ĵ���n ��չ��ʶ����λ<7:0>
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


//RXMnSIDH �������μĴ��� n ��׼��ʶ����λ<10:3>
#define RXM0SIDH        0x20
#define RXM1SIDH        0x24

//RXMnSIDL ���������˲��� n ��׼��ʶ����λ<2:0>
//RXMnSIDL[7..5]=SID����׼��ʶ������λ <2:0>
//RXMnSIDL[1.0] =EID����չ��ʶ������λ <17:16>
//��Щλװ���������Խ��ձ�������չ��ʶ��λ<17:16>���������жϵ�����λ
#define RXM0SIDL        0x21
#define RXM1SIDL        0x25

//RXMnEID8�����������μĴ��� n ��չ��ʶ����λ<15:8>
#define RXM0EID8        0x22
#define RXM1EID8        0x26

//RXMnEID0�����������μĴ��� n ��չ��ʶ����λ<7:0>
#define RXM0EID0        0x23
#define RXM1EID0        0x27


#define CNF3            0x28
#define CNF2            0x29
#define CNF1            0x2A
#define CANINTE         0x2B
#define CANINTF         0x2C
#define EFLG            0x2D
#define TXRTSCTRL       0x0D

//EFLG �����־�Ĵ�������ַ��2Dh��
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



//CANINTE �ж�ʹ�ܼĴ�������ַ��2Bh��

	#define IVRIE           0x80	//���Ľ��ջ����ڼ䷢������ʱ�ж�
	#define WAKIE           0x40	//CAN �������лʱ�ж�
	#define ERRIE           0x20	//EFLG���������仯ʱ�ж�
	#define TX2IE           0x10	//���ͻ�����2 ���ж�ʹ��λ
	#define TX1IE           0x08
	#define TX0IE           0x04
	#define RX1IE           0x02	//RXB1װ�ر���ʱ�ж�
	#define RX0IE           0x01
	#define NO_IE           0x00
//CANINTF �жϱ�־�Ĵ�������ַ��2Ch��
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
	//RXB0CTRL[6.5]RXM�����ջ���������ģʽλ
	//11 = �ر����Σ��˲����ܣ��������б���
	//10 = ֻ���շ����˲��������Ĵ�����չ��ʶ������Ч����
	//01 = ֻ���շ����˲��������Ĵ��б�׼��ʶ������Ч����
	//00 = ���շ����˲������������д���չ��ʶ�����׼��ʶ������Ч����
	#define RXB_RX_ANY      0x60
	#define RXB_RX_EXT      0x40
	#define RXB_RX_STD      0x20
	#define RXB_RX_STDEXT   0x00

	#define RXB_RXMx_M      0x60
	// #define RXB_RXIE_M      0x10

	//RXB0CTRL[3]RXRTR�����յ�Զ�̴�������λ
	//1 = ���յ�Զ�̴�������
	//0 = û�н��յ�Զ�̴�������
	#define RXB_RXRTR       0x08    // In RXBnCTRL

	//RXB0CTRL[2]BUKT������ʹ��λ
	//1 = ���RXB0 ����RXB0 ���յ��ı��Ľ��������� RXB1
	//0 = �����ֹ
	#define RXB_BUKT        0x04
	#define RXB_BUKT_RO     0x02

	//FILHIT[0]���˲�������λָ��ʹ�ܱ��Ľ��յ������˲��Ĵ������
	//1 = �����˲��Ĵ���1 ��RXF1��
	//0 = �����˲��Ĵ���0 ��RXF0��
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

//CANCTRL����CAN���ƼĴ�������ַ��XFh��

	//CLKEN[2]��CLKOUT����ʹ��λ
	//1 =CLKOUT����ʹ��
	//0 =CLKOUT���Ž�ֹ�����Ŵ��ڸ���̬��
	#define CLKEN           0x04

	//CLKPRE<1:0>��CLKOUT����Ԥ��Ƶ��λ
	//00 =FCLKOUT = ϵͳʱ��Ƶ��/1
	//01 =FCLKOUT =ϵͳʱ��Ƶ��/2
	//10 =FCLKOUT = ϵͳʱ��Ƶ��/4
	//11 =FCLKOUT = ϵͳʱ��Ƶ��/8
	#define CLK1            0x00
	#define CLK2            0x01
	#define CLK4            0x02
	#define CLK8            0x03

	//REQOP<2:0>��������ģʽ��λ
	//000 = �趨Ϊ��������ģʽ
	//001 = �趨Ϊ����ģʽ
	//010 = �趨Ϊ����ģʽ
	//011 = �趨Ϊ������ģʽ
	//100 = �趨Ϊ����ģʽ
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
//ȫ�ֱ���
//extern U8 ntxbuffer;		//the Number of Tx Buffer 

/****************************************************************************
������˵����������λMCP2510
****************************************************************************/
void MCP2510_Reset(void);

/****************************************************************************
������˵������MCP2510ָ����ַ�ж���һ���ֽ�
****************************************************************************/
unsigned char MCP2510_Read(U8 address);

/****************************************************************************
������˵����              Read often used status
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
������˵������MCP2510ָ����ַд��һ���ֽ�
****************************************************************************/
void MCP2510_Write( U8 address, U8 value);

/****************************************************************************
������˵�����޸�ָ����ַ�Ĵ�����ĳЩλ Ҫ�޸ĵ�Ϊ��MASK��ָ��Ϊ1
****************************************************************************/

//// Mask 1�޸� 0 ����
void MCP2510_WriteBits( U8 address, U8 data, U8 mask );

/****************************************************************************
������˵�������ж�ȡMCP2510����	���ݳ���Ϊ nlength
****************************************************************************/
void MCP2510_SRead( U8 address, unsigned char* pdata, U8 nlength );

/****************************************************************************
������˵��������д��MCP2510����	���ݳ���Ϊ nlength
****************************************************************************/
void MCP2510_Swrite(U8 address, unsigned char* pdata, U8 nlength);


void MCP2510_SetBandRate(CanBandRate bandrate, int IsBackNormal);

/***********************************************************\
*	��ȡMCP2510 ���յ�����							*
*	����: nbufferΪ�ڼ�������������Ϊ3����4	*
*			can_idΪ���ص�IDֵ							*
*			rxRTR��ʾ�Ƿ���RXRTRԶ��֡						*
*			data��ʾ��ȡ������						*
*			dlc��ʾdata length code							*
*	����ֵ												*
*		TRUE����ʾ����չ����						*
*		FALSE����ʾ����չ����						*
\***********************************************************/
int MCP2510_Read_Can(U8 nbuffer, int* rxRTR, U32* can_id, U8* data , U8* dlc);

/***********************************************************\
*	д��MCP2510 ���͵�����							*
*	����: nbufferΪ�ڼ�������������Ϊ0��1��2	*
*			ext��ʾ�Ƿ�����չ����					*
*			can_idΪ���ص�IDֵ							*
*			rxRTR��ʾ�Ƿ���RXRTR						*
*			data��ʾ��ȡ������						*
*			dlc��ʾdata length code							*
*		FALSE����ʾ����չ����						*
\***********************************************************/
void MCP2510_Write_Can( U8 nbuffer, int ext, U32 can_id, int rxRTR, U8* data,U8 dlc );

/****************************************************************************
������˵������ȡMCP2510 CAN����ID
����: addressΪMCP2510�Ĵ�����ַ
	can_idΪ���ص�IDֵ
����ֵ
TRUE����ʾ����չID(29λ)
FALSE����ʾ����չID(11λ)
****************************************************************************/
int MCP2510_Read_Can_ID( U8 address, U32* can_id);

/*******************************************\
*	����MCP2510 CAN����ID				*
*	����: addressΪMCP2510�Ĵ�����ַ*
*			can_idΪ���õ�IDֵ			*
*			IsExt��ʾ�Ƿ�Ϊ��չID	*
\*******************************************/
void MCP2510_Write_Can_ID(U8 address, U32 can_id, int IsExt);
extern U32 		CanId;		//can_idΪ���ص�IDֵ
extern U8  		dlc; 		//dlc��ʾdata length code
//extern U32 		rxRTR;		//rxRTR��ʾ�Ƿ���RXRTRԶ��֡
//extern U32 		isExt;
extern U8  		data_read[8] ;
extern U8  		data_write[8];
extern void Init_CANReciveINT(void);
#endif