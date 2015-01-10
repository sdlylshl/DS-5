/* 
** INTEL CONFIDENTIAL
** Copyright 2000-2003 Intel Corporation All Rights Reserved.
**
** The source code contained or described herein and all documents
** related to the source code (Material) are owned by Intel Corporation
** or its suppliers or licensors.  Title to the Material remains with
** Intel Corporation or its suppliers and licensors. The Material contains
** trade secrets and proprietary and confidential information of Intel
** or its suppliers and licensors. The Material is protected by worldwide
** copyright and trade secret laws and treaty provisions. No part of the
** Material may be used, copied, reproduced, modified, published, uploaded,
** posted, transmitted, distributed, or disclosed in any way without Intel抯
** prior express written permission.

** No license under any patent, copyright, trade secret or other intellectual
** property right is granted to or conferred upon you by disclosure or
** delivery of the Materials, either expressly, by implication, inducement,
** estoppel or otherwise. Any license under such intellectual property rights
** must be express and approved by Intel in writing.
*/
#define X16M 1	//16M晶振

#include "MCP2515.h"
#include <stdio.h>

#include "def.h"
#include "2440lib.h"
#include "bootpara.h"
#include "LCD_Driver.h"
#include "Keyboard.h"
#include "Delay.h"
extern void Uart_Printf(char *fmt,...);

//模拟实现SPI
//***************************************************************************
//全局变量

//U8 ntxbuffer;		//the Number of Tx Buffer 
//
U32 SendNum;
U32 SendErrNum;
U32 ReceiveNum;
U32 RecErrNum;
//#define Crystal 16MHz

#define LED1_OUT            ( rGPBCON = rGPBCON & (~(3<<16)) | (1<<16) )        //GPB8   GPS删除
#define LED1_Off            ( rGPBDAT = rGPBDAT | (1<<8) )
#define LED1_On             ( rGPBDAT = rGPBDAT & (~(1<<8))  )

#define LED2_OUT            ( rGPBCON = rGPBCON & (~(3<<14)) | (1<<14) )        //GPB7    用于CAN读显示
#define LED2_Off            ( rGPBDAT = rGPBDAT | (1<<7) )
#define LED2_On             ( rGPBDAT = rGPBDAT & (~(1<<7))  )

#define LED3_OUT            ( rGPBCON = rGPBCON & (~(3<<12)) | (1<<12) )        //GPB6    用于CAN写显示
#define LED3_Off            ( rGPBDAT = rGPBDAT | (1<<6) )
#define LED3_On             ( rGPBDAT = rGPBDAT & (~(1<<6))  )

#define LED4_OUT            ( rGPBCON = rGPBCON & (~(3<<10)) | (1<<10) )        //GPB5 用于系统循环显示
#define LED4_Off            ( rGPBDAT = rGPBDAT | (1<<5) )
#define LED4_On             ( rGPBDAT = rGPBDAT & (~(1<<5))  )
/****************************************************************************
【功能说明】SPI接口IO片选初始化
****************************************************************************/
void MCP2510_IO_CS_Init( void ) 
{
   MCP2510_CS_OUT ;
   MCP2510_SI_OUT ;
   MCP2510_SCK_OUT ;
   MCP2510_SO_IN ;
   MCP2510_SO_PULLUP ;		//允许上拉
   //MCP2510_SO_DISPULLUP ;		//禁止上拉

   MCP2510_SI_L ;		//SI put 0
   MCP2510_SCK_L ;		//SCK put 0
   { U16 k=0; for( ; k <= DELAY_TIME; k++ ) ;  }  //延时至少300ns
   MCP2510_CS_H ;			// unselect the MCP2510
   { U16 k=0; for( ; k <= DELAY_TIME; k++ ) ;  }  //延时至少300ns
}

/****************************************************************************
【功能说明】SPI接口读写开始，片选有效
****************************************************************************/
void MCP2510_RW_Start( void ) 
{
   MCP2510_SI_L ;		//SI put 0
   MCP2510_SCK_L ;		//SCK put 0
   { U16 k=0; for( ; k <= DELAY_TIME; k++ ) ;  }  //延时至少300ns
   MCP2510_CS_L ;			// Select the MCP2510
   { U16 k=0; for( ; k <= DELAY_TIME; k++ ) ;  }  //延时至少300ns
}

/****************************************************************************
【功能说明】SPI接口写入数据
****************************************************************************/
void Spi_Write( U8 Data ) 
{
	U8 m ;

	for( m = 0; m < 8; m++ )
	{
		if( (Data&0x80)==0x80 )
			MCP2510_SI_H;		//SI put 1
		else
			MCP2510_SI_L;		//SI put 0

		{ U16 k=0; for( ; k <= DELAY_TIME; k++ ) ;  }  //延时至少300ns
		MCP2510_SCK_H ;		//SCK put 1
		Data = Data<<1 ;
		MCP2510_SCK_L ;		//SCK put 0
		{ U16 k=0; for( ; k <= DELAY_TIME; k++ ) ;  }  //延时至少300ns
	}
}

/****************************************************************************
【功能说明】SPI接口读出数据
****************************************************************************/
U8 Spi_Read( )
{
	U8 m ;
	U8 data = 0 ;

	for( m = 0; m < 8; m++ )
	{
		MCP2510_SCK_H ;		//SCK put 1
		{ U16 k=0; for( ; k <= DELAY_TIME; k++ ) ;  }  //延时至少300ns
		data = data<<1;
		if( MCP2510_SO_GET != 0 )
			data |= 0x01 ;
		else
			data &= 0xfe;

		{ U16 k=0; for( ; k <= DELAY_TIME; k++ ) ;  }  //延时至少300ns
		MCP2510_SCK_L ;		//SCK put 0
		{ U16 k=0; for( ; k <= DELAY_TIME; k++ ) ;  }  //延时至少300ns
	}

	return (data);
}

/****************************************************************************
【功能说明】 Send Command to MCP2510 via SPI 
****************************************************************************/
void SendCMDMCP2510( U8 CMD )
{
   MCP2510_RW_Start() ;		//Initial IO port and CS is select
   Spi_Write( CMD );
   MCP2510_CS_H ;			// Deselect the MCP2510
}

/****************************************************************************
【功能说明】软件复位MCP2510
****************************************************************************/
void MCP2510_Reset()
{
	MCP2510_RW_Start() ;
	Spi_Write( MCP2510INSTR_RESET );
	MCP2510_CS_H ;
}

/****************************************************************************
【功能说明】向MCP2510指定地址写入一个字节
****************************************************************************/
void MCP2510_Write( U8 address, U8 value)
{
	MCP2510_RW_Start() ;

	Spi_Write(MCP2510INSTR_WRITE);
	Spi_Write( address );
	Spi_Write( value );

	MCP2510_CS_H ;
}

/****************************************************************************
【功能说明】修改指定地址寄存器的某些位
****************************************************************************/
void MCP2510_WriteBits( U8 address, U8 data, U8 mask )
{
	// 1修改 0 不变
	MCP2510_RW_Start() ;

	Spi_Write( MCP2510INSTR_BITMDFY );
	Spi_Write( address);
	Spi_Write( mask);
	Spi_Write( data);

	MCP2510_CS_H ;
}

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
unsigned char MCP2510_ReadStatus()
{
	unsigned char result;

	MCP2510_RW_Start() ;

	Spi_Write(MCP2510INSTR_RDSTAT);
	result = Spi_Read() ;
	Spi_Write( 0 ) ;		//数据重复输出
	MCP2510_CS_H ;

//	if( MCP2510_DEBUG )		Uart_Printf( "StatusREG = 0x%x\n", result ) ;
	return result;
}

/****************************************************************************
【功能说明】从MCP2510指定地址中读出一个字节
****************************************************************************/
unsigned char MCP2510_Read( U8 address )
{
	unsigned char result;

	MCP2510_RW_Start() ;

	Spi_Write(MCP2510INSTR_READ) ;		//0x03
	Spi_Write( address ) ;
	result = Spi_Read() ;

	MCP2510_CS_H ;

	return result ;
}

/****************************************************************************
【功能说明】序列读取MCP2510数据	
****************************************************************************/
void MCP2510_SRead( U8 address, unsigned char* pdata, U8 nlength )
{
	int i;

	MCP2510_RW_Start() ;
	Spi_Write(MCP2510INSTR_READ);
	Spi_Write( address );

	for (i=0; i<nlength; i++)
	{
		*pdata=Spi_Read();
		//if( MCP2510_DEBUG )    Uart_Printf( "  0x%x\n", (unsigned char)*pdata ) ;
		pdata++;
	}
	MCP2510_CS_H ;
}


/****************************************************************************
【功能说明】序列写入MCP2510数据	
****************************************************************************/
void MCP2510_Swrite( U8 address, unsigned char* pdata, U8 nlength)
{
	int i;
	MCP2510_RW_Start() ;

	Spi_Write(MCP2510INSTR_WRITE);
	Spi_Write((unsigned char)address);

	for (i=0; i < nlength; i++) 
	{
		Spi_Write( (unsigned char)*pdata );
		//if( MCP2510_DEBUG )    Uart_Printf( "0x%x\n", (unsigned char)*pdata ) ;
		pdata++;
	}
	MCP2510_CS_H ;
}

/****************************************************************************
【功能说明】
****************************************************************************/
void MCP2510_SetBandRate(CanBandRate bandrate, int IsBackNormal)
{
	U8 value=0;
	U8 ReadBackCNT = 0;

	// Bit rate calculations.
	//
	//Input clock fre=16MHz
	// In this case, we'll use a speed of 125 kbit/s, 250 kbit/s, 500 kbit/s.
	// If we set the length of the propagation segment to 7 bit time quanta,
	// and we set both the phase segments to 4 quanta each,
	// one bit will be 1+7+4+4 = 16 quanta in length.
	//
	// setting the prescaler (BRP) to 0 => 500 kbit/s.
	// setting the prescaler (BRP) to 1 => 250 kbit/s.
	// setting the prescaler (BRP) to 3 => 125 kbit/s.
	//
	// If we set the length of the propagation segment to 3 bit time quanta,
	// and we set both the phase segments to 1 quanta each,
	// one bit will be 1+3+2+2 = 8 quanta in length.
	// setting the prescaler (BRP) to 0 => 1 Mbit/s.

	// Go into configuration mode
	MCP2510_Write(MCP2510REG_CANCTRL, MODE_CONFIG);

//	if( MCP2510_DEBUG )  RETAILMSG(1,(TEXT( "MCP2510REG_CANCTRL =  0x%x\n"), MCP2510_Read(MCP2510REG_CANCTRL) ));

	while( ReadBackCNT<8 )
	{
		value = ( MCP2510_Read( MCP2510REG_CANSTAT ) & 0xe0 );
		if(value == MODE_CONFIG ){
			//Uart_Printf( "ReadBackCNT = 0x%x\n", ReadBackCNT );
			break;
		}
		ReadBackCNT++ ;
	}
	
	if( ReadBackCNT == 8 ) 			//Set mcp2510's mode failed,redo it again
	{
		//Uart_Printf( "Set config mode is failed! CANCTRL = 0x%x\n", value );
		MCP2510_Reset();
		MCP2510_Write(MCP2510REG_CANCTRL, MODE_CONFIG);		//redo to set mcp2510 mode
		DelayMS( 15 );
		value = ( MCP2510_Read(MCP2510REG_CANCTRL) & 0xe0 );	//read back mode from CANSTAT Register
		//Uart_Printf( "Set is 0x%x , Read is 0x%x\n", MODE_CONFIG, value ) ;
	}

	switch(bandrate){
	case BandRate_10kbps:
	//16M
	#ifdef X16M
	MCP2510_Write(CNF1, 0x31);	//10k	16TQ
		MCP2510_Write(CNF2, 0xAA);  //PS1=7 TQ  PSeg=1 TQ
		MCP2510_Write(CNF3, 0x05);  //PS2=7 TQ SYNC=1 TQ	
	#else
		MCP2510_Write(CNF1, 0x18);	//10k	16TQ
		MCP2510_Write(CNF2, 0xAA);  //PS1=7 TQ  PSeg=1 TQ
		MCP2510_Write(CNF3, 0x05);  //PS2=7 TQ SYNC=1 TQ	
		//if( MCP2510_DEBUG )  Uart_Printf( "CNF1 =  0x%x\n", MCP2510_Read(CNF1) );
		//if( MCP2510_DEBUG )  Uart_Printf( "CNF2 =  0x%x\n", MCP2510_Read(CNF2) );
		//if( MCP2510_DEBUG )  Uart_Printf( "CNF3 =  0x%x\n", MCP2510_Read(CNF3) );
	#endif
		break;
	case BandRate_125kbps:
		MCP2510_Write(CNF1, SJW1|BRP4);	//Synchronization Jump Width Length =1 TQ
		MCP2510_Write(CNF2, BTLMODE_CNF3|(SEG4<<3)|SEG7); // Phase Seg 1 = 4, Prop Seg = 7
		MCP2510_Write(CNF3, SEG4);// Phase Seg 2 = 4
		break;
	case BandRate_250kbps:
		if(1)
		//if(boot_params.xosc.val==0)//16M
		{
			MCP2510_Write(CNF1, 0x1);	//10k	16TQ
			MCP2510_Write(CNF2, 0x9E);  //PS1=7 TQ  PSeg=1 TQ
			MCP2510_Write(CNF3, 0x03);  //PS2=7 TQ SYNC=1 TQ	
		}else{
			MCP2510_Write(CNF1, 0x0);	//10k	16TQ
			MCP2510_Write(CNF2, 0xAA);  //PS1=7 TQ  PSeg=1 TQ
			MCP2510_Write(CNF3, 0x05);  //PS2=7 TQ SYNC=1 TQ	
			//if( MCP2510_DEBUG )  Uart_Printf( "CNF1 =  0x%x\n", MCP2510_Read(CNF1) );
			//if( MCP2510_DEBUG )  Uart_Printf( "CNF2 =  0x%x\n", MCP2510_Read(CNF2) );
			//if( MCP2510_DEBUG )  Uart_Printf( "CNF3 =  0x%x\n", MCP2510_Read(CNF3) );
		}
	//	MCP2510_Write(CNF1, SJW1|BRP2);	//Synchronization Jump Width Length =1 TQ
	//	MCP2510_Write(CNF2, BTLMODE_CNF3|(SEG4<<3)|SEG7); // Phase Seg 1 = 4, Prop Seg = 7
	//	MCP2510_Write(CNF3, SEG4);// Phase Seg 2 = 4
		break;
	case BandRate_500kbps:
		MCP2510_Write(CNF1, SJW1|BRP1);	//Synchronization Jump Width Length =1 TQ
		MCP2510_Write(CNF2, BTLMODE_CNF3|(SEG4<<3)|SEG7); // Phase Seg 1 = 4, Prop Seg = 7
		MCP2510_Write(CNF3, SEG4);// Phase Seg 2 = 4
		break;
	case BandRate_1Mbps:
		MCP2510_Write(CNF1, SJW1|BRP1);	//Synchronization Jump Width Length =1 TQ
		MCP2510_Write(CNF2, BTLMODE_CNF3|(SEG3<<3)|SEG2); // Phase Seg 1 = 2, Prop Seg = 3
		MCP2510_Write(CNF3, SEG2);// Phase Seg 2 = 1
		break;
	}

	if( IsBackNormal == true  )
	{
		//Enable clock output
		MCP2510_Write(CLKCTRL, MODE_NORMAL | CLKEN | CLK8);
	}

}

/****************************************************************************
【功能说明】读取MCP2510 CAN总线ID
参数: address为MCP2510寄存器地址
	can_id为返回的ID值
返回值
TRUE，表示是扩展ID(29位)
FALSE，表示非扩展ID(11位)
****************************************************************************/
int MCP2510_Read_Can_ID( U8 address, U32* can_id)
{
	U32 tbufdata;
	unsigned char* p=(unsigned char*)&tbufdata;

	MCP2510_SRead(address, p, 4);
	*can_id = (tbufdata<<3)|((tbufdata>>13)&0x7);
	*can_id &= 0x7ff;

	if ( (p[MCP2510LREG_SIDL] & TXB_EXIDE_M) ==  TXB_EXIDE_M ) {
		*can_id = (*can_id<<2) | (p[MCP2510LREG_SIDL] & 0x03);
		*can_id <<= 16;
		*can_id |= tbufdata>>16;
		return true;
	}
	return false;
}

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
int MCP2510_Read_Can(U8 nbuffer, int* rxRTR, U32* can_id, U8* data , U8* dlc)
{

	U8 mcp_addr = (nbuffer<<4) + 0x31, ctrl;
	int IsExt;

	IsExt=MCP2510_Read_Can_ID( mcp_addr, can_id);

	ctrl=MCP2510_Read(mcp_addr-1);
	*dlc=MCP2510_Read( mcp_addr+4);
	if ((ctrl & 0x08)) {
		*rxRTR = true;
	}
	else{
		*rxRTR = false;
	}
	*dlc &= DLC_MASK;
	//MCP2510_SRead(mcp_addr+5, data, *dlc);
	MCP2510_SRead(mcp_addr+5, data, 8);

	return IsExt;
}


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
void MCP2510_Write_Can( U8 nbuffer, int ext, U32 can_id, int rxRTR, U8* data,U8 dlc )
{
	U8 mcp_addr = (nbuffer<<4) + TXB0SIDH;
	MCP2510_Swrite(mcp_addr+5, data, dlc );  // write data bytes
	MCP2510_Write_Can_ID( mcp_addr, can_id,ext);  // write CAN id
	if (rxRTR)
		dlc |= RTR_MASK;  // if RTR set bit in byte
	MCP2510_Write((mcp_addr+4), dlc);            // write the RTR and DLC
}

/*******************************************\
*	设置MCP2510 CAN总线ID				*
*	参数: address为MCP2510寄存器地址*
*			can_id为设置的ID值			*
*			IsExt表示是否为扩展ID	*
\*******************************************/
void MCP2510_Write_Can_ID(U8 address, U32 can_id, int IsExt)
{
	U32 tbufdata;

	if (IsExt) {
		can_id&=0x1fffffff;	//29位
		tbufdata=can_id &0xffff;
		tbufdata<<=16;
		tbufdata|=(can_id>>(18-5)&(~0x1f));
		tbufdata |= TXB_EXIDE_M;
	}
	else{
		can_id&=0x7ff;	//11位
		tbufdata= (can_id>>3)|((can_id&0x7)<<13);
	}
	MCP2510_Swrite(address, (unsigned char*)&tbufdata, 4);
}

/***********************************************************************************\
								发送数据
	参数:
		data，发送数据

	Note: 使用三个缓冲区循环发送，没有做缓冲区有效检测
\***********************************************************************************/
	/*
void Can_Write(U32 id, U8 *pdata, unsigned char dlc, int IsExt, int rxRTR)
{
	unsigned char err ;
//	unsigned int ntxbuffer=0;
	MCP2510_Write_Can(ntxbuffer, IsExt, id, rxRTR, pdata, dlc);

	switch(ntxbuffer){
	case 0:
		//MCP2510_WriteBits(TXB0CTRL, (TXB_TXREQ_M|TXB_TXP10_M), 0xff) ;
		MCP2510_WriteBits(TXB0CTRL,(TXB_TXREQ_M|TXB_TXP10_M), 0xff) ;
//		do 
		{ 
			err = MCP2510_Read(TXB0CTRL) ;
//			Uart_Printf( "TXB0CTRL state: 0x%x\n",err );
		
		}
//		while(0);
//		while( (err &0x08)==0x08 )  ;
//		if( (err &0x70) != 0 )  RETAILMSG(1,(TEXT( " Can Send Err = 0x%x\n"), err ) );
		ntxbuffer=0;
		break;
	case 1:
		MCP2510_WriteBits(TXB1CTRL, (TXB_TXREQ_M|TXB_TXP10_M), 0xff) ;
	//	do 
		{ 
			err = MCP2510_Read(TXB1CTRL) ; 
//			Uart_Printf( "TXB1CTRL state: 0x%x\n",err );

		}
//		while( (err &0x08)==0x08 )  ;
		if( (err &0x70) != 0 )  Uart_Printf( "  Can Send Err = 0x%x\n", err ) ;
		ntxbuffer=1;
		break;
	case 2:
		MCP2510_WriteBits(TXB2CTRL, (TXB_TXREQ_M|TXB_TXP10_M), 0xff) ;
		do { err = MCP2510_Read(TXB2CTRL) ; }
		while( (err &0x08)==0x08 )  ;
//		if( (err &0x70) != 0 )  RETAILMSG(1,(TEXT( "  Can Send Err = 0x%x\n"), err ) );
		ntxbuffer=0;
		break;
	}

}*/

/***********************************************************************************\
                                发送数据
    参数:
        data，发送数据

    Note: 使用三个缓冲区循环发送，没有做缓冲区有效检测
\***********************************************************************************/
void Can_Write(U32 CAN_id, U8 *pdata, unsigned char dlc, int IsExt, int rxRTR)
{
    unsigned char err ;
    static int ntxbuffer = 0;    
    char errBUScount=0;
    
    //U32 rINTMSK_BAK;
    
    //备份中断
    //此处中断须禁用 否则接收总会丢失 也通过连续发送两次可解决
    //rINTMSK_BAK = rINTMSK;
    //DisableIrq(BIT_ALLMSK);
   
   //LED3_On;
    MCP2510_Write_Can(ntxbuffer, IsExt, CAN_id, rxRTR, pdata, dlc);    
   
    switch(ntxbuffer){
    case 0:
    //TXREQ 发送MCU将此位置 1 以请求报文发送－报文发送后该位自动清零
    MCP2510_WriteBits(TXB0CTRL, (TXB_TXREQ_M | TXB_TXP10_M), 0xff) ;

    do
    {
        MCP2510_WriteBits(CANINTF, (U8)(~(TX0INT)), TX0INT); // Clear interrupt
        // 检测TXREQ是否发送完成
        err = MCP2510_Read(TXB0CTRL) ;
        if ( (err & TXB_TXREQ_M)==0x08  )
        {
            errBUScount++;
            if (errBUScount == 10)
            {
                errBUScount = 0;
                Uart_Printf( "  CanBUS send0 Err = 0x%x\n", err  );
                break;
            }
        }
        else
        {
            errBUScount = 0;
        }

    }
    while (errBUScount != 0 )  ;
    ntxbuffer=1;
    break;

    case 1:
    //TXREQ 发送MCU将此位置 1 以请求报文发送－报文发送后该位自动清零
    MCP2510_WriteBits(TXB1CTRL, (TXB_TXREQ_M | TXB_TXP10_M), 0xff) ;

    do
    {
        MCP2510_WriteBits(CANINTF, (U8)(~(TX1INT)), TX1INT); // Clear interrupt
        // 检测TXREQ是否发送完成
        err = MCP2510_Read(TXB1CTRL) ;
        if ( (err & TXB_TXREQ_M)==0x08  )
        {
            errBUScount++;
            if (errBUScount == 10)
            {
                errBUScount = 0;
                Uart_Printf( "  CanBUS send1 Err = 0x%x\n", err  );
                break;
            }
        }
        else
        {
            errBUScount = 0;
        }

    }
    while (errBUScount != 0 )  ;
    ntxbuffer=2;
    break;    
    case 2:
    //TXREQ 发送MCU将此位置 1 以请求报文发送－报文发送后该位自动清零
    MCP2510_WriteBits(TXB2CTRL, (TXB_TXREQ_M | TXB_TXP10_M), 0xff) ;

    do
    {
        MCP2510_WriteBits(CANINTF, (U8)(~(TX2INT)), TX2INT); // Clear interrupt
        // 检测TXREQ是否发送完成
        err = MCP2510_Read(TXB2CTRL) ;
        if ( (err & TXB_TXREQ_M) ==0x08 )
        {
            errBUScount++;
            if (errBUScount == 10)
            {
                errBUScount = 0;
                Uart_Printf( "  CanBUS send2 Err = 0x%x\n", err  );
                break;
            }
        }
        else
        {
            errBUScount = 0;
        }

    }
    while (errBUScount != 0 )  ;
    ntxbuffer=0;
    break;
}
 
    if ( (err & 0x70) != 0 )SendErrNum++; 
 //EnableIrq(rINTMSK_BAK);

// LED3_Off;
}

/***********************************************************************************\
								查询是否收到数据
	返回值:如果没有数据，则返回-1，
			否则，返回收到数据的缓冲区号
	Note: 如果两个缓冲区都收到数据，则返回第一个缓冲区
\***********************************************************************************/
int Can_Poll()
{

	if( MCP2510_ReadStatus()&RX0INT )
		return 0;
	
	if( MCP2510_ReadStatus()&RX1INT )
		return 1;

	return -1;
}

/****************************************************************************
【功能说明】
****************************************************************************/
int Can_Read(int n, U32* id, U8 *pdata,  U8*dlc, int* rxRTR, int *isExt)
{
	U8 bytes;
	bytes = MCP2510_Read(CANINTF);

	if(n==0)
	{
		if(bytes & RX0INT)
		{
			*isExt=MCP2510_Read_Can(n+3, rxRTR, id, pdata, dlc);
			MCP2510_WriteBits(CANINTF, (U8)(~(RX0INT)), RX0INT); // Clear interrupt
			return true ;
		}
		//RETAILMSG(1,(TEXT( "Error! 0 bytes is Read!!! CANINTF=0x%x\n"), byte) ) ;
		return false;
	}
	else if(n ==1 )
	{
		if(bytes & RX1INT)
		{
			*isExt=MCP2510_Read_Can(n+4, rxRTR, id, pdata, dlc);
			MCP2510_WriteBits(CANINTF, (U8)(~(RX1INT)), RX1INT); // Clear interrupt
			return true ;
		}
		//RETAILMSG(1,(TEXT( "0 bytes is Read!!! CANINTF=0x%x\n"), byte) ) ;
		return false;
	}

	//RETAILMSG(1,(TEXT( "Error! Receive channel=0x%x\n"), n )) ;
	return false;
}

/****************************************************************************
【功能说明】
****************************************************************************/
// Setup the CAN buffers used by the application.
// We currently use only one for reception and one for transmission.
// It is possible to use several to get a simple form of queue.
//
// We setup the unit to receive all CAN messages.
// As we only have at most 4 different messages to receive, we could use the
// filters to select them for us.
//
// Init_MCP2510() should already have been called.
extern void Can_Setup(void)
{
    // As no filters are active, all messages will be stored in RXB0 only if
    // no roll-over is active. We want to recieve all CAN messages (standard and extended)
    // (RXM<1:0> = 11).
    //SPI_mcp_write_bits(RXB0CTRL, RXB_RX_ANY, 0xFF);
    //SPI_mcp_write_bits(RXB1CTRL, RXB_RX_ANY, 0xFF);

    // But there is a bug in the chip, so we have to activate roll-over.
	//void MCP2510_WriteBits( U8 address, U8 data, U8 mask );
	MCP2510_WriteBits(RXB0CTRL, (RXB_RXM0|RXB_BUKT), 0xFF);		//关闭屏蔽滤波功能，接收所有报文，允许滚存 
	MCP2510_WriteBits(RXB1CTRL, RXB_RXM0, 0xFF);		//关闭屏蔽滤波功能，接收所有报文
//	RXB0CTRL---接收缓冲器0控制寄存器（0X60）
//  7:    未用 ，读0
//	6-5:  RXM:接收缓冲器工作模式位
//          11=关闭屏蔽、滤波功能；接收所有报文
//          10=只接收符合滤波条件的带扩展标识符的有效报文
//          01=只接收符合滤波条件的带标准标识符的有效报文
//          00=接收符合滤波条件的所有带扩展标识符或标准标识符的所有报文
//  4:  未用  ，读0
//  3：  RXRTR：接收到远程传送请求 ；1=接收到远程传送请求，0=未接收到远程传送请求
//  2：BUKT：滚存使能位   1=若RXB0满，RXB0接收到的报文将被滚存至RXB1，0=滚存禁止
//  1：  只读位
//  0：FILHIT：滤波器命中位----指明使能报文接收的验收滤波寄存器编号
//         1=验收滤波寄存器1（RXF1）
//         0=验收滤波寄存器0（RXF0）
//
//	RXB1CTRL---接收缓冲器1控制寄存器（0X70）
//  7:    未用 ，读0
//	6-5:  RXM:接收缓冲器工作模式位
//          11=关闭屏蔽、滤波功能；接收所有报文
//          10=只接收符合滤波条件的带扩展标识符的有效报文
//          01=只接收符合滤波条件的带标准标识符的有效报文
//          00=接收符合滤波条件的所有带扩展标识符或标准标识符的所有报文
//  4:  未用  ，读0
//  3：  RXRTR：接收到远程传送请求 ；1=接收到远程传送请求，0=未接收到远程传送请求
//  2-0: FILHIT 滤波器命中位----指明使能报文接收的验收滤波寄存器编号
//        101=验收滤波寄存器5(RXF5)
//        100=验收滤波寄存器4(RXF4)
//        011=验收滤波寄存器3(RXF3)
//        010=验收滤波寄存器2(RXF2)
//        001=验收滤波寄存器1(RXF1)(只有当RXB0CTRL中的BULK位置1时)
//        000=验收滤波寄存器0(RXF0)(只有当RXB0CTRL中的BULK位置1时)
	
}


/****************************************************************************
【功能说明】
****************************************************************************/
extern void Init_MCP2510(CanBandRate bandrate)
{
	unsigned char i,j,a;

	MCP2510_IO_CS_Init() ;
	MCP2510_Reset();

	MCP2510_SetBandRate(bandrate,false);		//设置波特率

	// Disable interrups.
	MCP2510_Write(CANINTE, NO_IE);  		//禁止所有中断
		
	// Mark all filter bits as don't care:
	MCP2510_Write_Can_ID(RXM0SIDH, 0,0);	//标准帧
	MCP2510_Write_Can_ID(RXM1SIDH, 0,0);
	//0.RXBnCTRL.FILHIT 决定了使用那个滤波器进行滤波
	//1. 屏蔽寄存器决定了对哪一位进行校验
	//2. 验收滤波器决定了对当前位匹配的值
	// Anyway, set all filters to 0:
	MCP2510_Write_Can_ID(RXF0SIDH, 0, 0);
	MCP2510_Write_Can_ID(RXF1SIDH, 0, 0);
	//
	MCP2510_Write_Can_ID(RXF2SIDH, 0, 0);
	MCP2510_Write_Can_ID(RXF3SIDH, 0, 0);
	MCP2510_Write_Can_ID(RXF4SIDH, 0, 0);
	MCP2510_Write_Can_ID(RXF5SIDH, 0, 0);

	//MCP2510_Write(CLKCTRL, MODE_LOOPBACK| CLKEN | CLK8);//回环模式
    //如果不能用两台设备联机实验的话，可以选择回环模式
	MCP2510_Write(CLKCTRL, MODE_NORMAL| CLKEN | CLK8);//标准模式
	
  	//MCP2510_Write(CLKCTRL, MODE_LISTENONLY| CLKEN | CLK8);//监听模式

	// Clear, deactivate the three transmit buffers
	a = TXB0CTRL;
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 14; j++) {
			MCP2510_Write(a, 0);
			a++;
	        }
       	a += 2; // We did not clear CANSTAT or CANCTRL
	}
	//接收符合滤波器条件的所有带扩展标识符或标准标识符的有效报文
	// and the two receive buffers.
	MCP2510_Write(RXB0CTRL, 0);
	MCP2510_Write(RXB1CTRL, 0);

	// The two pins RX0BF and RX1BF are used to control two LEDs; set them as outputs and set them as 00.
	MCP2510_Write(BFPCTRL, 0x3C);
	
	//Open Interrupt
	MCP2510_Write(CANINTE, RX0IE|RX1IE|IVRIE);
}

/****************************************************************************
【功能说明】MCP2510实验程序
****************************************************************************/
void Test_MCP2510(void)
{
	int i,j;
	U32 id;
	unsigned char dlc;
	int rxRTR, isExt;
	int temp;
	
	U8 data_write[8]={1,2,3,4,5,6,7,8};
	U8 data_read[8] ;
	
//   	RETAILMSG(1,(TEXT( "\r\nCAN BUS Test[ MCP2510 ], press ESC key to exit !\r\n") ) ) ;

	Init_MCP2510(BandRate_250kbps);
	Can_Setup();
	
    Uart_Init(0,115200);//初始化串口控制寄存器，设置正确的波特率
    Uart_Select(0);//选择串口，这里我们选串口0
	
	Uart_Printf( "\nCAN BUS TEST \n" );
	Uart_Printf( "Press 'ESC' key to Exit this program !\n\n" );
	i=0;
    for (  j=0; j<50;j++)
   // while(1)
    {
#if 1    
		Can_Write( 0x5a, data_write, 8, false, false);//发送报文  标准模式下必须有接收端才能发送成功
//		Can_Write( 0x5b, data_write, 8, false, false);
//#else		
		while( (i=Can_Poll())==-1 ) ;
		
		Uart_Printf( "i=%d          " ,i);
		for( temp=0; temp<8; temp++)  data_read[temp] = 0 ;
		
		temp = Can_Read(i, &id, data_read, &dlc, &rxRTR, &isExt);

	
		Uart_Printf( "ID=0x%x\n",id );
		Uart_Printf( "numbers=%d\n" ,j+1);
		Uart_Printf( "Data=%x,%x,%x,%x,%x,%x,%x,%x\n\n",data_read[0],data_read[1],data_read[2],data_read[3],data_read[4],data_read[5],data_read[6],data_read[7] );
		//if ( Uart_GetKey() == 0x1b ) break;
#endif		
		Delay(500);
	}
	Uart_Printf( "\nCAN BUS TEST done\n" );
	
}


void Enable_CANINT(void)
{
    //rEINTPEND = 0xffffff; //to clear the previous pending states
    rSRCPND |= (BIT_EINT8_23);
    rINTPND |= (BIT_EINT8_23);
    
    #if GPS
    rEINTMASK &= ~(1 << 9);
    #else
    //Open External interrupt mask
    rEINTMASK &= ~(1 << 11);
    #endif
    
    //Open Interrupt mask control    
    rINTMSK &= ~(BIT_EINT8_23);
}
void Disable_CANINT(void)
{
    //rEINTPEND = 0xffffff; //to clear the previous pending states
	#if GPS
    rEINTMASK |= (1 << 9);
    rEINTPEND |= (1 << 9);
    #else
    rEINTMASK |= (1 << 11);
    rEINTPEND |= (1 << 11);
    #endif
    rINTMSK |= (BIT_EINT8_23);
    rSRCPND |= (BIT_EINT8_23);
    rINTPND |= (BIT_EINT8_23);
}

static void __irq Eint8_23_ISR(void)
{
    U8 i, tempisr;
    //LED2_On;

    //rEINTMASK=0xFFFFFFFF;
    //Delay(10);
    Disable_CANINT();
    MCP2510_Write(CANINTE, NO_IE);          //禁止所有中断
    tempisr = Can_Poll();
    Can_Read(tempisr, &CanId, data_read, &dlc, 0, 0);

    //Uart_Printf("tempisr==.%x\n", tempisr);
    //Uart_Printf("ID==.%x\n",id);
    //Uart_Printf("CANCTRL==.%x\n", MCP2510_Read(MCP2510REG_CANCTRL));
    //Uart_Printf("CANINTF==.%x\n", MCP2510_Read(CANINTF));
    //Uart_Printf("eflg==.%x\n", MCP2510_Read(EFLG));
    //Uart_Printf("CANSTAT==.%x\n", MCP2510_Read(MCP2510REG_CANSTAT));
    //Uart_Printf("TXB0CTRL==.%x\n", MCP2510_Read(TXB0CTRL));

    if (((MCP2510_Read(CANINTF)) & 0xff) != 0)
    {
        //MCP2510_WriteBits(CANINTF, (U8)(~(RX0INT)), RX0INT); // Clear interrupt
        MCP2510_WriteBits(CANINTF, 0, 0xff); // Clear interrupt
    }
    if (((MCP2510_Read(EFLG)) & 0xff) != 0)
    {
        //MCP2510_WriteBits(CANINTF, (U8)(~(RX0INT)), RX0INT); // Clear interrupt
        MCP2510_WriteBits(EFLG, 0, 0xff); // Clear interrupt
    }
/*
    if (((MCP2510_Read(MCP2510REG_CANSTAT)) & 0xe0) != 0)
    {
        Init_MCP2510(BandRate_250kbps);             //MCP2515初始化
        Can_Setup();
    }
    */
   ReceiveNum++;
   //Lcd_SetPosition(0, 12);
   //Printf( "RECIVE Data=%x,%x,%x,%x,%x,%x,%x,%x\n\n", data_read[0], data_read[1], data_read[2], data_read[3], data_read[4], data_read[5], data_read[6], data_read[7] );
   //Lcd_Update();
        //校验接收的报文是否正确
        for (i = 3; i < 8; ++i)
        {
            if (data_write[i] != data_read[i])
                break;
        }

        if (i < 8)
        {
            RecErrNum++;
        }

	
    MCP2510_Write(CANINTE, RX0IE | RX1IE);  //开CAN中断
    Enable_CANINT();
    // LED2_Off;
}
void Init_CANReciveINT(void)
{

	#if GPS
	rGPGCON = rGPGCON & ~(3 << 2) | (2 << 2); //GPG1设置为EINT9
    rGPGDAT = rGPGDAT | (1 << 1);
    rGPGUP |= (1 << 1); //disable GPG3 pull up

    //rEXTINT1 = (rEXTINT1 & ~(7<<4));//|(2<<4);   //低电平触发  //EINT9 －>falling edge triggered
    rEXTINT1 = (rEXTINT1 & ~(7 << 4)) | (2 << 4); //下降沿触发  //EINT9 －>falling edge triggered

	#else
    rGPGCON = rGPGCON & ~(3 << 6) | (1 << 7); //GPG3设置为EINT11
    rGPGDAT = rGPGDAT | (1 << 3);
    rGPGUP |= (1 << 3); //disable GPG3 pull up

    //rEXTINT1 = (rEXTINT1 & ~(7<<12));//|(2<<12);   //低电平触发  //EINT11 －>falling edge triggered
    rEXTINT1 = (rEXTINT1 & ~(7 << 12)) | (2 << 12); //下降沿触发  //EINT11 －>falling edge triggered
    #endif

    pISR_EINT8_23 = (unsigned)Eint8_23_ISR;
    Enable_CANINT();
    
}

//源挂起寄存器（SRCPND）=0 中断未被请求；=1 中断源声明了中断请求
//中断挂起寄存器（INTPND） =0 未请求中断；=1 中断源已声明了中断请求

//中断屏蔽寄存器（INTMSK） =0 允许中断； =1 中断挂起
//外部中断屏蔽寄存器(EINTMASK)    =0 使能中断；=1 禁止中断
//外部中断挂起寄存器（EINTPEND）写1清零此位；=0 使能中断；=1 禁止中断
