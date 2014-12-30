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
** posted, transmitted, distributed, or disclosed in any way without Intel�s
** prior express written permission.

** No license under any patent, copyright, trade secret or other intellectual
** property right is granted to or conferred upon you by disclosure or
** delivery of the Materials, either expressly, by implication, inducement,
** estoppel or otherwise. Any license under such intellectual property rights
** must be express and approved by Intel in writing.
*/
#define X16M 1	//16M����

#include "MCP2515.h"
#include <stdio.h>

#include "def.h"
#include "2440lib.h"
#include "bootpara.h"
#include "LCD_Driver.h"
#include "Keyboard.h"
#include "Delay.h"
extern void Uart_Printf(char *fmt,...);

//ģ��ʵ��SPI
//***************************************************************************
//ȫ�ֱ���

//U8 ntxbuffer;		//the Number of Tx Buffer 
//
U32 SendNum;
U32 SendErrNum;
U32 ReceiveNum;
U32 RecErrNum;
//#define Crystal 16MHz

#define LED1_OUT            ( rGPBCON = rGPBCON & (~(3<<16)) | (1<<16) )        //GPB8   GPSɾ��
#define LED1_Off            ( rGPBDAT = rGPBDAT | (1<<8) )
#define LED1_On             ( rGPBDAT = rGPBDAT & (~(1<<8))  )

#define LED2_OUT            ( rGPBCON = rGPBCON & (~(3<<14)) | (1<<14) )        //GPB7    ����CAN����ʾ
#define LED2_Off            ( rGPBDAT = rGPBDAT | (1<<7) )
#define LED2_On             ( rGPBDAT = rGPBDAT & (~(1<<7))  )

#define LED3_OUT            ( rGPBCON = rGPBCON & (~(3<<12)) | (1<<12) )        //GPB6    ����CANд��ʾ
#define LED3_Off            ( rGPBDAT = rGPBDAT | (1<<6) )
#define LED3_On             ( rGPBDAT = rGPBDAT & (~(1<<6))  )

#define LED4_OUT            ( rGPBCON = rGPBCON & (~(3<<10)) | (1<<10) )        //GPB5 ����ϵͳѭ����ʾ
#define LED4_Off            ( rGPBDAT = rGPBDAT | (1<<5) )
#define LED4_On             ( rGPBDAT = rGPBDAT & (~(1<<5))  )
/****************************************************************************
������˵����SPI�ӿ�IOƬѡ��ʼ��
****************************************************************************/
void MCP2510_IO_CS_Init( void ) 
{
   MCP2510_CS_OUT ;
   MCP2510_SI_OUT ;
   MCP2510_SCK_OUT ;
   MCP2510_SO_IN ;
   MCP2510_SO_PULLUP ;		//��������
   //MCP2510_SO_DISPULLUP ;		//��ֹ����

   MCP2510_SI_L ;		//SI put 0
   MCP2510_SCK_L ;		//SCK put 0
   { U16 k=0; for( ; k <= DELAY_TIME; k++ ) ;  }  //��ʱ����300ns
   MCP2510_CS_H ;			// unselect the MCP2510
   { U16 k=0; for( ; k <= DELAY_TIME; k++ ) ;  }  //��ʱ����300ns
}

/****************************************************************************
������˵����SPI�ӿڶ�д��ʼ��Ƭѡ��Ч
****************************************************************************/
void MCP2510_RW_Start( void ) 
{
   MCP2510_SI_L ;		//SI put 0
   MCP2510_SCK_L ;		//SCK put 0
   { U16 k=0; for( ; k <= DELAY_TIME; k++ ) ;  }  //��ʱ����300ns
   MCP2510_CS_L ;			// Select the MCP2510
   { U16 k=0; for( ; k <= DELAY_TIME; k++ ) ;  }  //��ʱ����300ns
}

/****************************************************************************
������˵����SPI�ӿ�д������
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

		{ U16 k=0; for( ; k <= DELAY_TIME; k++ ) ;  }  //��ʱ����300ns
		MCP2510_SCK_H ;		//SCK put 1
		Data = Data<<1 ;
		MCP2510_SCK_L ;		//SCK put 0
		{ U16 k=0; for( ; k <= DELAY_TIME; k++ ) ;  }  //��ʱ����300ns
	}
}

/****************************************************************************
������˵����SPI�ӿڶ�������
****************************************************************************/
U8 Spi_Read( )
{
	U8 m ;
	U8 data = 0 ;

	for( m = 0; m < 8; m++ )
	{
		MCP2510_SCK_H ;		//SCK put 1
		{ U16 k=0; for( ; k <= DELAY_TIME; k++ ) ;  }  //��ʱ����300ns
		data = data<<1;
		if( MCP2510_SO_GET != 0 )
			data |= 0x01 ;
		else
			data &= 0xfe;

		{ U16 k=0; for( ; k <= DELAY_TIME; k++ ) ;  }  //��ʱ����300ns
		MCP2510_SCK_L ;		//SCK put 0
		{ U16 k=0; for( ; k <= DELAY_TIME; k++ ) ;  }  //��ʱ����300ns
	}

	return (data);
}

/****************************************************************************
������˵���� Send Command to MCP2510 via SPI 
****************************************************************************/
void SendCMDMCP2510( U8 CMD )
{
   MCP2510_RW_Start() ;		//Initial IO port and CS is select
   Spi_Write( CMD );
   MCP2510_CS_H ;			// Deselect the MCP2510
}

/****************************************************************************
������˵���������λMCP2510
****************************************************************************/
void MCP2510_Reset()
{
	MCP2510_RW_Start() ;
	Spi_Write( MCP2510INSTR_RESET );
	MCP2510_CS_H ;
}

/****************************************************************************
������˵������MCP2510ָ����ַд��һ���ֽ�
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
������˵�����޸�ָ����ַ�Ĵ�����ĳЩλ
****************************************************************************/
void MCP2510_WriteBits( U8 address, U8 data, U8 mask )
{
	// 1�޸� 0 ����
	MCP2510_RW_Start() ;

	Spi_Write( MCP2510INSTR_BITMDFY );
	Spi_Write( address);
	Spi_Write( mask);
	Spi_Write( data);

	MCP2510_CS_H ;
}

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
unsigned char MCP2510_ReadStatus()
{
	unsigned char result;

	MCP2510_RW_Start() ;

	Spi_Write(MCP2510INSTR_RDSTAT);
	result = Spi_Read() ;
	Spi_Write( 0 ) ;		//�����ظ����
	MCP2510_CS_H ;

//	if( MCP2510_DEBUG )		Uart_Printf( "StatusREG = 0x%x\n", result ) ;
	return result;
}

/****************************************************************************
������˵������MCP2510ָ����ַ�ж���һ���ֽ�
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
������˵�������ж�ȡMCP2510����	
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
������˵��������д��MCP2510����	
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
������˵����
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
������˵������ȡMCP2510 CAN����ID
����: addressΪMCP2510�Ĵ�����ַ
	can_idΪ���ص�IDֵ
����ֵ
TRUE����ʾ����չID(29λ)
FALSE����ʾ����չID(11λ)
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
*	д��MCP2510 ���͵�����							*
*	����: nbufferΪ�ڼ�������������Ϊ0��1��2	*
*			ext��ʾ�Ƿ�����չ����					*
*			can_idΪ���ص�IDֵ							*
*			rxRTR��ʾ�Ƿ���RXRTR						*
*			data��ʾ��ȡ������						*
*			dlc��ʾdata length code							*
*		FALSE����ʾ����չ����						*
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
*	����MCP2510 CAN����ID				*
*	����: addressΪMCP2510�Ĵ�����ַ*
*			can_idΪ���õ�IDֵ			*
*			IsExt��ʾ�Ƿ�Ϊ��չID	*
\*******************************************/
void MCP2510_Write_Can_ID(U8 address, U32 can_id, int IsExt)
{
	U32 tbufdata;

	if (IsExt) {
		can_id&=0x1fffffff;	//29λ
		tbufdata=can_id &0xffff;
		tbufdata<<=16;
		tbufdata|=(can_id>>(18-5)&(~0x1f));
		tbufdata |= TXB_EXIDE_M;
	}
	else{
		can_id&=0x7ff;	//11λ
		tbufdata= (can_id>>3)|((can_id&0x7)<<13);
	}
	MCP2510_Swrite(address, (unsigned char*)&tbufdata, 4);
}

/***********************************************************************************\
								��������
	����:
		data����������

	Note: ʹ������������ѭ�����ͣ�û������������Ч���
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
                                ��������
    ����:
        data����������

    Note: ʹ������������ѭ�����ͣ�û������������Ч���
\***********************************************************************************/
void Can_Write(U32 CAN_id, U8 *pdata, unsigned char dlc, int IsExt, int rxRTR)
{
    unsigned char err ;
    static int ntxbuffer = 0;    
    char errBUScount=0;
    
    //U32 rINTMSK_BAK;
    
    //�����ж�
    //�˴��ж������ ��������ܻᶪʧ Ҳͨ�������������οɽ��
    //rINTMSK_BAK = rINTMSK;
    //DisableIrq(BIT_ALLMSK);
   
   //LED3_On;
    MCP2510_Write_Can(ntxbuffer, IsExt, CAN_id, rxRTR, pdata, dlc);    
   
    switch(ntxbuffer){
    case 0:
    //TXREQ ����MCU����λ�� 1 �������ķ��ͣ����ķ��ͺ��λ�Զ�����
    MCP2510_WriteBits(TXB0CTRL, (TXB_TXREQ_M | TXB_TXP10_M), 0xff) ;

    do
    {
        MCP2510_WriteBits(CANINTF, (U8)(~(TX0INT)), TX0INT); // Clear interrupt
        // ���TXREQ�Ƿ������
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
    //TXREQ ����MCU����λ�� 1 �������ķ��ͣ����ķ��ͺ��λ�Զ�����
    MCP2510_WriteBits(TXB1CTRL, (TXB_TXREQ_M | TXB_TXP10_M), 0xff) ;

    do
    {
        MCP2510_WriteBits(CANINTF, (U8)(~(TX1INT)), TX1INT); // Clear interrupt
        // ���TXREQ�Ƿ������
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
    //TXREQ ����MCU����λ�� 1 �������ķ��ͣ����ķ��ͺ��λ�Զ�����
    MCP2510_WriteBits(TXB2CTRL, (TXB_TXREQ_M | TXB_TXP10_M), 0xff) ;

    do
    {
        MCP2510_WriteBits(CANINTF, (U8)(~(TX2INT)), TX2INT); // Clear interrupt
        // ���TXREQ�Ƿ������
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
								��ѯ�Ƿ��յ�����
	����ֵ:���û�����ݣ��򷵻�-1��
			���򣬷����յ����ݵĻ�������
	Note: ����������������յ����ݣ��򷵻ص�һ��������
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
������˵����
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
������˵����
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
	MCP2510_WriteBits(RXB0CTRL, (RXB_RXM0|RXB_BUKT), 0xFF);		//�ر������˲����ܣ��������б��ģ�������� 
	MCP2510_WriteBits(RXB1CTRL, RXB_RXM0, 0xFF);		//�ر������˲����ܣ��������б���
//	RXB0CTRL---���ջ�����0���ƼĴ�����0X60��
//  7:    δ�� ����0
//	6-5:  RXM:���ջ���������ģʽλ
//          11=�ر����Ρ��˲����ܣ��������б���
//          10=ֻ���շ����˲������Ĵ���չ��ʶ������Ч����
//          01=ֻ���շ����˲������Ĵ���׼��ʶ������Ч����
//          00=���շ����˲����������д���չ��ʶ�����׼��ʶ�������б���
//  4:  δ��  ����0
//  3��  RXRTR�����յ�Զ�̴������� ��1=���յ�Զ�̴�������0=δ���յ�Զ�̴�������
//  2��BUKT������ʹ��λ   1=��RXB0����RXB0���յ��ı��Ľ���������RXB1��0=�����ֹ
//  1��  ֻ��λ
//  0��FILHIT���˲�������λ----ָ��ʹ�ܱ��Ľ��յ������˲��Ĵ������
//         1=�����˲��Ĵ���1��RXF1��
//         0=�����˲��Ĵ���0��RXF0��
//
//	RXB1CTRL---���ջ�����1���ƼĴ�����0X70��
//  7:    δ�� ����0
//	6-5:  RXM:���ջ���������ģʽλ
//          11=�ر����Ρ��˲����ܣ��������б���
//          10=ֻ���շ����˲������Ĵ���չ��ʶ������Ч����
//          01=ֻ���շ����˲������Ĵ���׼��ʶ������Ч����
//          00=���շ����˲����������д���չ��ʶ�����׼��ʶ�������б���
//  4:  δ��  ����0
//  3��  RXRTR�����յ�Զ�̴������� ��1=���յ�Զ�̴�������0=δ���յ�Զ�̴�������
//  2-0: FILHIT �˲�������λ----ָ��ʹ�ܱ��Ľ��յ������˲��Ĵ������
//        101=�����˲��Ĵ���5(RXF5)
//        100=�����˲��Ĵ���4(RXF4)
//        011=�����˲��Ĵ���3(RXF3)
//        010=�����˲��Ĵ���2(RXF2)
//        001=�����˲��Ĵ���1(RXF1)(ֻ�е�RXB0CTRL�е�BULKλ��1ʱ)
//        000=�����˲��Ĵ���0(RXF0)(ֻ�е�RXB0CTRL�е�BULKλ��1ʱ)
	
}


/****************************************************************************
������˵����
****************************************************************************/
extern void Init_MCP2510(CanBandRate bandrate)
{
	unsigned char i,j,a;

	MCP2510_IO_CS_Init() ;
	MCP2510_Reset();

	MCP2510_SetBandRate(bandrate,false);		//���ò�����

	// Disable interrups.
	MCP2510_Write(CANINTE, NO_IE);  		//��ֹ�����ж�
		
	// Mark all filter bits as don't care:
	MCP2510_Write_Can_ID(RXM0SIDH, 0,0);	//��׼֡
	MCP2510_Write_Can_ID(RXM1SIDH, 0,0);
	//0.RXBnCTRL.FILHIT ������ʹ���Ǹ��˲��������˲�
	//1. ���μĴ��������˶���һλ����У��
	//2. �����˲��������˶Ե�ǰλƥ���ֵ
	// Anyway, set all filters to 0:
	MCP2510_Write_Can_ID(RXF0SIDH, 0, 0);
	MCP2510_Write_Can_ID(RXF1SIDH, 0, 0);
	//
	MCP2510_Write_Can_ID(RXF2SIDH, 0, 0);
	MCP2510_Write_Can_ID(RXF3SIDH, 0, 0);
	MCP2510_Write_Can_ID(RXF4SIDH, 0, 0);
	MCP2510_Write_Can_ID(RXF5SIDH, 0, 0);

	//MCP2510_Write(CLKCTRL, MODE_LOOPBACK| CLKEN | CLK8);//�ػ�ģʽ
    //�����������̨�豸����ʵ��Ļ�������ѡ��ػ�ģʽ
	MCP2510_Write(CLKCTRL, MODE_NORMAL| CLKEN | CLK8);//��׼ģʽ
	
  	//MCP2510_Write(CLKCTRL, MODE_LISTENONLY| CLKEN | CLK8);//����ģʽ

	// Clear, deactivate the three transmit buffers
	a = TXB0CTRL;
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 14; j++) {
			MCP2510_Write(a, 0);
			a++;
	        }
       	a += 2; // We did not clear CANSTAT or CANCTRL
	}
	//���շ����˲������������д���չ��ʶ�����׼��ʶ������Ч����
	// and the two receive buffers.
	MCP2510_Write(RXB0CTRL, 0);
	MCP2510_Write(RXB1CTRL, 0);

	// The two pins RX0BF and RX1BF are used to control two LEDs; set them as outputs and set them as 00.
	MCP2510_Write(BFPCTRL, 0x3C);
	
	//Open Interrupt
	MCP2510_Write(CANINTE, RX0IE|RX1IE|IVRIE);
}

/****************************************************************************
������˵����MCP2510ʵ�����
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
	
    Uart_Init(0,115200);//��ʼ�����ڿ��ƼĴ�����������ȷ�Ĳ�����
    Uart_Select(0);//ѡ�񴮿ڣ���������ѡ����0
	
	Uart_Printf( "\nCAN BUS TEST \n" );
	Uart_Printf( "Press 'ESC' key to Exit this program !\n\n" );
	i=0;
    for (  j=0; j<50;j++)
   // while(1)
    {
#if 1    
		Can_Write( 0x5a, data_write, 8, false, false);//���ͱ���  ��׼ģʽ�±����н��ն˲��ܷ��ͳɹ�
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
    MCP2510_Write(CANINTE, NO_IE);          //��ֹ�����ж�
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
        Init_MCP2510(BandRate_250kbps);             //MCP2515��ʼ��
        Can_Setup();
    }
    */
   ReceiveNum++;
   //Lcd_SetPosition(0, 12);
   //Printf( "RECIVE Data=%x,%x,%x,%x,%x,%x,%x,%x\n\n", data_read[0], data_read[1], data_read[2], data_read[3], data_read[4], data_read[5], data_read[6], data_read[7] );
   //Lcd_Update();
        //У����յı����Ƿ���ȷ
        for (i = 3; i < 8; ++i)
        {
            if (data_write[i] != data_read[i])
                break;
        }

        if (i < 8)
        {
            RecErrNum++;
        }

	
    MCP2510_Write(CANINTE, RX0IE | RX1IE);  //��CAN�ж�
    Enable_CANINT();
    // LED2_Off;
}
void Init_CANReciveINT(void)
{

	#if GPS
	rGPGCON = rGPGCON & ~(3 << 2) | (2 << 2); //GPG1����ΪEINT9
    rGPGDAT = rGPGDAT | (1 << 1);
    rGPGUP |= (1 << 1); //disable GPG3 pull up

    //rEXTINT1 = (rEXTINT1 & ~(7<<4));//|(2<<4);   //�͵�ƽ����  //EINT9 ��>falling edge triggered
    rEXTINT1 = (rEXTINT1 & ~(7 << 4)) | (2 << 4); //�½��ش���  //EINT9 ��>falling edge triggered

	#else
    rGPGCON = rGPGCON & ~(3 << 6) | (1 << 7); //GPG3����ΪEINT11
    rGPGDAT = rGPGDAT | (1 << 3);
    rGPGUP |= (1 << 3); //disable GPG3 pull up

    //rEXTINT1 = (rEXTINT1 & ~(7<<12));//|(2<<12);   //�͵�ƽ����  //EINT11 ��>falling edge triggered
    rEXTINT1 = (rEXTINT1 & ~(7 << 12)) | (2 << 12); //�½��ش���  //EINT11 ��>falling edge triggered
    #endif

    pISR_EINT8_23 = (unsigned)Eint8_23_ISR;
    Enable_CANINT();
    
}

//Դ����Ĵ�����SRCPND��=0 �ж�δ������=1 �ж�Դ�������ж�����
//�жϹ���Ĵ�����INTPND�� =0 δ�����жϣ�=1 �ж�Դ���������ж�����

//�ж����μĴ�����INTMSK�� =0 �����жϣ� =1 �жϹ���
//�ⲿ�ж����μĴ���(EINTMASK)    =0 ʹ���жϣ�=1 ��ֹ�ж�
//�ⲿ�жϹ���Ĵ�����EINTPEND��д1�����λ��=0 ʹ���жϣ�=1 ��ֹ�ж�
