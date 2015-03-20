/*
 * (c)COPYRIGHT
 * ALL RIGHT RESERVED
 *
 * FileName : w5500.c
  * -----------------------------------------------------------------
 */
#include <stdio.h>
#include <string.h>
#include "config.h"
#include "SPI2.h"
#include "w5500.h"
#include "socket.h"
#include "delay.h"
#include "spi.h"
#include "flash.h"

#ifdef __DEF_IINCHIP_PPP__
   #include "md5.h"
#endif



uint8_t TX_BUF[8][TX_RX_MAX_BUF_SIZE]; // TX Buffer for applications
uint8_t BUF_WRITE[8] = { 0 };
uint8_t BUF_READ[8] = { 0 };
//uint8_t TX_BUF_1[TX_RX_MAX_BUF_SIZE]; // TX Buffer for applications
//uint8_t TX_BUF_2[TX_RX_MAX_BUF_SIZE]; // TX Buffer for applications
//uint8_t TX_BUF_3[TX_RX_MAX_BUF_SIZE]; // TX Buffer for applications
//uint8_t TX_BUF_4[TX_RX_MAX_BUF_SIZE]; // TX Buffer for applications
//uint8_t TX_BUF_5[TX_RX_MAX_BUF_SIZE]; // TX Buffer for applications
//uint8_t TX_BUF_6[TX_RX_MAX_BUF_SIZE]; // TX Buffer for applications
//uint8_t TX_BUF_7[TX_RX_MAX_BUF_SIZE]; // TX Buffer for applications
//uint8_t TX_BUF_8[TX_RX_MAX_BUF_SIZE]; // TX Buffer for applications

static uint8_t I_STATUS[MAX_SOCK_NUM];
static uint16_t SSIZE[MAX_SOCK_NUM]; /**< Max Tx buffer size by each channel */
static uint16_t RSIZE[MAX_SOCK_NUM]; /**< Max Rx buffer size by each channel */

CONFIG_MSG Config_Msg = {
	{ 0x00, 0x08, 0xDC, 0x01, 0x12, 0x02 },//Mac[6]
	{ 192, 168, 88, 13 },//Lip[4]
	{ 255, 255, 255, 0 },//Sub[4]
	{ 192, 168, 88, 1 },//Gw[4]
	{ 202, 102, 152, 3 },//DNS_Server_IP[4]
	0,//DHCP
	8081 //port
};
CHCONFIG_TYPE_DEF Chconfig_Type_Def = {
	8081,//DST_IP port
	{ 192, 168, 88, 222 }//DST_IP Address 
};

// Configuration Network Information of W5500
//uint8_t MAC[6] = { 0x00, 0x08, 0xDC, 0x01, 0x02, 0x02 };//MAC Address
//uint8_t IP[4] = { 192, 168, 88, 12 };//IP Address
//uint8_t GateWay[4] = { 192, 168, 88, 1 };//Gateway Address
//uint8_t SubNet[4] = { 255, 255, 255, 0 };//SubnetMask Address
//
//TX MEM SIZE- SOCKET 0-7:2KB
//RX MEM SIZE- SOCKET 0-7:2KB
uint8_t txsize[MAX_SOCK_NUM] = { 2, 2, 2, 2, 2, 2, 2, 2 };
uint8_t rxsize[MAX_SOCK_NUM] = { 2, 2, 2, 2, 2, 2, 2, 2 };

//FOR TCP Client
//Configuration Network Information of TEST PC
//uint8_t Dest_IP[4] = { 192, 168, 88, 222 }; //DST_IP Address 
//uint16_t Dest_PORT = 8081; //DST_IP port

uint8_t ch_status[MAX_SOCK_NUM] = { 0, };	/** 0:close, 1:ready, 2:connected 3:init*/

void WIZ_CS(uint8_t val)
{
	if (val == LOW) {
		GPIO_ResetBits(GPIOC, NET_CS);
	}
	else if (val == HIGH){
		GPIO_SetBits(GPIOC, NET_CS);
	}
}


uint8_t SPI1_SendByte(uint8_t byte)
{
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

	SPI_I2S_SendData(SPI1, byte);

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

	return SPI_I2S_ReceiveData(SPI1);
}

uint8_t getISR(uint8_t s)
{
  return I_STATUS[s];
}
void putISR(uint8_t s, uint8_t val)
{
   I_STATUS[s] = val;
}

uint16_t getIINCHIP_RxMAX(uint8_t s)
{
   return RSIZE[s];
}
uint16_t getIINCHIP_TxMAX(uint8_t s)
{
   return SSIZE[s];
}
void IINCHIP_CSoff(void)
{
  WIZ_CS(LOW);
}
void IINCHIP_CSon(void)
{
   WIZ_CS(HIGH);
}
u8  IINCHIP_SpiSendData(uint8_t dat)
{
   return(SPI1_SendByte(dat));
}

void IINCHIP_WRITE( uint32_t addrbsb,  uint8_t data)
{
   IINCHIP_ISR_DISABLE();                        // Interrupt Service Routine Disable
   IINCHIP_CSoff();                              // CS=0, SPI start
   IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);// Address byte 1
   IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);// Address byte 2
   IINCHIP_SpiSendData( (addrbsb & 0x000000F8) + 4);    // Data write command and Write data length 1
   IINCHIP_SpiSendData(data);                    // Data write (write 1byte data)
   IINCHIP_CSon();                               // CS=1,  SPI end
   IINCHIP_ISR_ENABLE();                         // Interrupt Service Routine Enable
}

uint8_t IINCHIP_READ(uint32_t addrbsb)
{
   uint8_t data = 0;
   IINCHIP_ISR_DISABLE();                        // Interrupt Service Routine Disable
   IINCHIP_CSoff();                              // CS=0, SPI start
   IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);// Address byte 1
   IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);// Address byte 2
   IINCHIP_SpiSendData( (addrbsb & 0x000000F8))    ;// Data read command and Read data length 1
   data = IINCHIP_SpiSendData(0x00);             // Data read (read 1byte data)
   IINCHIP_CSon();                               // CS=1,  SPI end
   IINCHIP_ISR_ENABLE();                         // Interrupt Service Routine Enable
   return data;    
}

uint16_t wiz_write_buf(uint32_t addrbsb,uint8_t* buf,uint16_t len)
{
   uint16_t idx = 0;
   if(len == 0) printf("Unexpected2 length 0\r\n");

//   IINCHIP_ISR_DISABLE();
   IINCHIP_CSoff();                              // CS=0, SPI start
   IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);// Address byte 1
   IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);// Address byte 2
   IINCHIP_SpiSendData( (addrbsb & 0x000000F8) + 4);    // Data write command and Write data length 1
   for(idx = 0; idx < len; idx++)                // Write data in loop
   {
     IINCHIP_SpiSendData(buf[idx]);
   }
   IINCHIP_CSon();                               // CS=1, SPI end
//   IINCHIP_ISR_ENABLE();                         // Interrupt Service Routine Enable    

   return len;  
}

uint16_t wiz_read_buf(uint32_t addrbsb, uint8_t* buf,uint16_t len)
{
  uint16_t idx = 0;
  if(len == 0)
  {
    printf("Unexpected2 length 0\r\n");
  }

  IINCHIP_ISR_DISABLE();
  //SPI MODE I/F
  IINCHIP_CSoff();                                  // CS=0, SPI start
  IINCHIP_SpiSendData( (addrbsb & 0x00FF0000)>>16);// Address byte 1
  IINCHIP_SpiSendData( (addrbsb & 0x0000FF00)>> 8);// Address byte 2
  IINCHIP_SpiSendData( (addrbsb & 0x000000F8));    // Data write command and Write data length 1
  for(idx = 0; idx < len; idx++)                    // Write data in loop
  {
    buf[idx] = IINCHIP_SpiSendData(0x00);
  }
  IINCHIP_CSon();                                   // CS=1, SPI end
  IINCHIP_ISR_ENABLE();                             // Interrupt Service Routine Enable
  
  return len;
}


/**
@brief  This function is for resetting of the iinchip. Initializes the iinchip to work in whether DIRECT or INDIRECT mode
*/
void iinchip_init(void)
{
  setMR( MR_RST );
#ifdef __DEF_IINCHIP_DBG__
  printf("MR value is %02x \r\n",IINCHIP_READ_COMMON(MR));
#endif
}

/**
@brief  This function set the transmit & receive buffer size as per the channels is used
Note for TMSR and RMSR bits are as follows\n
bit 1-0 : memory size of channel #0 \n
bit 3-2 : memory size of channel #1 \n
bit 5-4 : memory size of channel #2 \n
bit 7-6 : memory size of channel #3 \n
bit 9-8 : memory size of channel #4 \n
bit 11-10 : memory size of channel #5 \n
bit 12-12 : memory size of channel #6 \n
bit 15-14 : memory size of channel #7 \n
Maximum memory size for Tx, Rx in the W5500 is 16K Bytes,\n
In the range of 16KBytes, the memory size could be allocated dynamically by each channel.\n
Be attentive to sum of memory size shouldn't exceed 8Kbytes\n
and to data transmission and receiption from non-allocated channel may cause some problems.\n
If the 16KBytes memory is already  assigned to centain channel, \n
other 3 channels couldn't be used, for there's no available memory.\n
If two 4KBytes memory are assigned to two each channels, \n
other 2 channels couldn't be used, for there's no available memory.\n
*/
void sysinit( uint8_t * tx_size, uint8_t * rx_size  )
{
  uint16_t i;
  uint16_t ssum,rsum;
#ifdef __DEF_IINCHIP_DBG__
  printf("sysinit()\r\n");
#endif
  ssum = 0;
  rsum = 0;

  for (i = 0 ; i < MAX_SOCK_NUM; i++)       // Set the size, masking and base address of Tx & Rx memory by each channel
  {
          IINCHIP_WRITE( (Sn_TXMEM_SIZE(i)), tx_size[i]);
          IINCHIP_WRITE( (Sn_RXMEM_SIZE(i)), rx_size[i]);
          
#ifdef __DEF_IINCHIP_DBG__
         printf("tx_size[%d]: %d, Sn_TXMEM_SIZE = %d\r\n",i, tx_size[i], IINCHIP_READ(Sn_TXMEM_SIZE(i)));
         printf("rx_size[%d]: %d, Sn_RXMEM_SIZE = %d\r\n",i, rx_size[i], IINCHIP_READ(Sn_RXMEM_SIZE(i)));
#endif
		 SSIZE[i] = (uint16_t)(0);
		 RSIZE[i] = (uint16_t)(0);


    if (ssum <= 16384)
    {
         switch( tx_size[i] )
      {
      case 1:
		  SSIZE[i] = (uint16_t)(1024);
        break;
      case 2:
		  SSIZE[i] = (uint16_t)(2048);
        break;
      case 4:
		  SSIZE[i] = (uint16_t)(4096);
        break;
      case 8:
		  SSIZE[i] = (uint16_t)(8192);
        break;
      case 16:
		  SSIZE[i] = (uint16_t)(16384);
      break;
      default :
		  RSIZE[i] = (uint16_t)(2048);
        break;
      }
    }

   if (rsum <= 16384)
    {
         switch( rx_size[i] )
      {
      case 1:
		  RSIZE[i] = (uint16_t)(1024);
        break;
      case 2:
		  RSIZE[i] = (uint16_t)(2048);
        break;
      case 4:
		  RSIZE[i] = (uint16_t)(4096);
        break;
      case 8:
		  RSIZE[i] = (uint16_t)(8192);
        break;
      case 16:
		  RSIZE[i] = (uint16_t)(16384);
        break;
      default :
		  RSIZE[i] = (uint16_t)(2048);
        break;
      }
    }
    ssum += SSIZE[i];
    rsum += RSIZE[i];

  }
}

// added

/**
@brief  This function sets up gateway IP address.
*/
void setGAR(
  uint8_t * addr  /**< a pointer to a 4 -byte array responsible to set the Gateway IP address. */
  )
{
    wiz_write_buf(GAR0, addr, 4);
}
void getGWIP(uint8_t * addr)
{
    wiz_read_buf(GAR0, addr, 4);
}

/**
@brief  It sets up SubnetMask address
*/
void setSUBR(uint8_t * addr)
{   
    wiz_write_buf(SUBR0, addr, 4);
}
/**
@brief  This function sets up MAC address.
*/
void setSHAR(
  uint8_t * addr  /**< a pointer to a 6 -byte array responsible to set the MAC address. */
  )
{
  wiz_write_buf(SHAR0, addr, 6);  
}

/**
@brief  This function sets up Source IP address.
*/
void setSIPR(
  uint8_t * addr  /**< a pointer to a 4 -byte array responsible to set the Source IP address. */
  )
{
    wiz_write_buf(SIPR0, addr, 4);  
}

/**
@brief  This function sets up Source IP address.
*/
void getGAR(uint8_t * addr)
{
    wiz_read_buf(GAR0, addr, 4);
}
void getSUBR(uint8_t * addr)
{
    wiz_read_buf(SUBR0, addr, 4);
}
void getSHAR(uint8_t * addr)
{
    wiz_read_buf(SHAR0, addr, 6);
}
void getSIPR(uint8_t * addr)
{
    wiz_read_buf(SIPR0, addr, 4);
}

void setMR(uint8_t val)
{
  IINCHIP_WRITE(MR,val);
}

/**
@brief  This function gets Interrupt register in common register.
 */
uint8_t getIR( void )
{
   return IINCHIP_READ(IR);
}

/**
@brief  This function sets up Retransmission time.

If there is no response from the peer or delay in response then retransmission
will be there as per RTR (Retry Time-value Register)setting
*/
void setRTR(uint16_t timeout)
{
  IINCHIP_WRITE(RTR0,(uint8_t)((timeout & 0xff00) >> 8));
  IINCHIP_WRITE(RTR1,(uint8_t)(timeout & 0x00ff));
}

/**
@brief  This function set the number of Retransmission.

If there is no response from the peer or delay in response then recorded time
as per RTR & RCR register seeting then time out will occur.
*/
void setRCR(uint8_t retry)
{
  IINCHIP_WRITE(RCR,retry);
}

/**
@brief  This function set the interrupt mask Enable/Disable appropriate Interrupt. ('1' : interrupt enable)

If any bit in IMR is set as '0' then there is not interrupt signal though the bit is
set in IR register.
*/
void clearIR(uint8_t mask)
{
  IINCHIP_WRITE(IR, ~mask | getIR() ); // must be setted 0x10.
}

/**
@brief  This sets the maximum segment size of TCP in Active Mode), while in Passive Mode this is set by peer
*/
void setSn_MSS(SOCKET s, uint16_t Sn_MSSR)
{
  IINCHIP_WRITE( Sn_MSSR0(s), (uint8_t)((Sn_MSSR & 0xff00) >> 8));
  IINCHIP_WRITE( Sn_MSSR1(s), (uint8_t)(Sn_MSSR & 0x00ff));
}

void setSn_TTL(SOCKET s, uint8_t ttl)
{    
   IINCHIP_WRITE( Sn_TTL(s) , ttl);
}



/**
@brief  get socket interrupt status

These below functions are used to read the Interrupt & Soket Status register
*/
uint8_t getSn_IR(SOCKET s)
{
   return IINCHIP_READ(Sn_IR(s));
}


/**
@brief   get socket status
*/
uint8_t getSn_SR(SOCKET s)
{
   return IINCHIP_READ(Sn_SR(s));
}


/**
@brief  get socket TX free buf size

This gives free buffer size of transmit buffer. This is the data size that user can transmit.
User shuold check this value first and control the size of transmitting data
*/
uint16_t getSn_TX_FSR(SOCKET s)
{
  uint16_t val=0,val1=0;
  do
  {
    val1 = IINCHIP_READ(Sn_TX_FSR0(s));
    val1 = (val1 << 8) + IINCHIP_READ(Sn_TX_FSR1(s));
      if (val1 != 0)
    {
        val = IINCHIP_READ(Sn_TX_FSR0(s));
        val = (val << 8) + IINCHIP_READ(Sn_TX_FSR1(s));
    }
  } while (val != val1);
   return val;
}


/**
@brief   get socket RX recv buf size

This gives size of received data in receive buffer.
*/
uint16_t getSn_RX_RSR(SOCKET s)
{
  uint16_t val=0,val1=0;
  do
  {
    val1 = IINCHIP_READ(Sn_RX_RSR0(s));
    val1 = (val1 << 8) + IINCHIP_READ(Sn_RX_RSR1(s));
    if(val1 != 0)
    {
        val = IINCHIP_READ(Sn_RX_RSR0(s));
        val = (val << 8) + IINCHIP_READ(Sn_RX_RSR1(s));
    }
  } while (val != val1);
   return val;
}


/**
@brief   This function is being called by send() and sendto() function also.

This function read the Tx write pointer register and after copy the data in buffer update the Tx write pointer
register. User should read upper byte first and lower byte later to get proper value.
*/
void send_data_processing(SOCKET s, uint8_t *data, uint16_t len)
{
  uint16_t ptr = 0;
  uint32_t addrbsb = 0;
  if(len == 0)
  {
    printf("CH: %d Unexpected1 length 0\r\n", s);
    return;
  }


  ptr = IINCHIP_READ( Sn_TX_WR0(s) );
  ptr = ((ptr & 0x00ff) << 8) + IINCHIP_READ(Sn_TX_WR1(s));

  addrbsb = (uint32_t)(ptr<<8) + (s<<5) + 0x10;
  wiz_write_buf(addrbsb, data, len);
  
  ptr += len;
  IINCHIP_WRITE( Sn_TX_WR0(s) ,(uint8_t)((ptr & 0xff00) >> 8));
  IINCHIP_WRITE( Sn_TX_WR1(s),(uint8_t)(ptr & 0x00ff));
}


/**
@brief  This function is being called by recv() also.

This function read the Rx read pointer register
and after copy the data from receive buffer update the Rx write pointer register.
User should read upper byte first and lower byte later to get proper value.
*/
void recv_data_processing(SOCKET s, uint8_t *data, uint16_t len)
{
  uint16_t ptr = 0;
  uint32_t addrbsb = 0;
  
  if(len == 0)
  {
    printf("CH: %d Unexpected2 length 0\r\n", s);
    return;
  }

  ptr = IINCHIP_READ( Sn_RX_RD0(s) );
  ptr = ((ptr & 0x00ff) << 8) + IINCHIP_READ( Sn_RX_RD1(s) );

  addrbsb = (uint32_t)(ptr<<8) + (s<<5) + 0x18;
  wiz_read_buf(addrbsb, data, len);
  ptr += len;

  IINCHIP_WRITE( Sn_RX_RD0(s), (uint8_t)((ptr & 0xff00) >> 8));
  IINCHIP_WRITE( Sn_RX_RD1(s), (uint8_t)(ptr & 0x00ff));
}

void Set_network(void)
{
	uint8_t tmp_array[6];

	setSHAR(Default_Conf_Dat.Stu_Dat.Config_Msg.Mac);
	setSUBR(Default_Conf_Dat.Stu_Dat.Config_Msg.Sub);
	setGAR(Default_Conf_Dat.Stu_Dat.Config_Msg.Gw);
	setSIPR(Default_Conf_Dat.Stu_Dat.Config_Msg.Lip);

	setRTR(6000);
	setRCR(3);

	//Init. TX & RX Memory size
	sysinit(txsize, rxsize);

	printf("\r\n----------------------------------------- \r\n");
	printf("W5500E01-M3                       \r\n");
	printf("Network Configuration Information \r\n");
	printf("----------------------------------------- \r\n");

	getSHAR(tmp_array);
	printf("\r\nMAC : %.2X.%.2X.%.2X.%.2X.%.2X.%.2X", tmp_array[0], tmp_array[1], tmp_array[2], tmp_array[3], tmp_array[4], tmp_array[5]);

	getSIPR(tmp_array);
	printf("\r\nIP : %d.%d.%d.%d", tmp_array[0], tmp_array[1], tmp_array[2], tmp_array[3]);

	getSUBR(tmp_array);
	printf("\r\nSN : %d.%d.%d.%d", tmp_array[0], tmp_array[1], tmp_array[2], tmp_array[3]);

	getGAR(tmp_array);
	printf("\r\nGW : %d.%d.%d.%d\r\n", tmp_array[0], tmp_array[1], tmp_array[2], tmp_array[3]);
	printf("----------------------------------------- \r\n\r\n");
}

void Reset_W5500(void)
{
	GPIO_ResetBits(GPIOA, NET_RST);
	delay_ms(1);
	GPIO_SetBits(GPIOA, NET_RST);
	delay_ms(10);
}





