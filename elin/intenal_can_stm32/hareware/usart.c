#include "usart.h"
#include "hardware.h"

uint8_t rxBuffer[256];
uint8_t rx_rd = 0;
uint8_t rx_wr = 0;
Boolean flag_rs485_card_nr[3];
uint8_t rxBuffer5[256];
uint8_t rx_rd5 = 0;
uint8_t rx_wr5 = 0;

void usart_init()
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA , ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���USART1


	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = USART1_TX_Pin;
	GPIO_InitStructure.GPIO_Mode = USART1_TX_GPIO_Mode;
	GPIO_Init(USART1_TX_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = USART1_RX_Pin;
	GPIO_InitStructure.GPIO_Mode = USART1_RX_GPIO_Mode;
	GPIO_Init(USART1_RX_GPIO, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 9600;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�1	 
	USART_Cmd(USART1, ENABLE);
}

void USART1_IRQHandler(void)                	//����1�жϷ������
{
	uint8_t Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  
	{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
		rxBuffer[rx_wr++] = Res;
     }
} 
	
void USART2_IRQHandler(void)                	//����1�жϷ������
{
//	uint8_t Res;
//	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
//	{
//		Res = USART_ReceiveData(USART2);
//		
//	}	 

}

int fputc(int ch, FILE *f)
{
	while ((USART1->SR & 0X40) == 0);//ѭ������,ֱ���������   
	USART1->DR = (uint8_t)ch;
	return ch;
}

void UART_Put_Char(USART_TypeDef* uart,unsigned char DataToSend)
{
	//��Ҫ���͵��ֽ�д��UART�ķ��ͻ�����
	USART_SendData(uart, (unsigned char)DataToSend);
	//�ȴ��������
	while (!(uart->SR & USART_FLAG_TXE));
}

void UART1_Put_Char(unsigned char DataToSend)
{
	UART_Put_Char(USART1,DataToSend);
}

void UART2_Put_Char(unsigned char DataToSend)
{
	UART_Put_Char(USART2, DataToSend);
}

uint8_t last_buf[3];
Boolean flag_card_waiting = FALSE;
Boolean flag_card_recycling = FALSE;
void decode_rs232_buf()
{
	if ((rx_wr + 256 - rx_rd) % 256 > 5)
	{
		if ( rxBuffer[rx_rd] == 0xE0 && rxBuffer[( uint8_t ) ( rx_rd + 1 )] == 0x55 && rxBuffer[( uint8_t ) ( rx_rd + 2 )] == 0xAA )
		{
			uint8_t len = rxBuffer[( uint8_t ) ( rx_rd + 5 )];
			if ((rx_wr + 256 - rx_rd) % 256 >= len + 10)
			{
				uint8_t protocol = rxBuffer[( uint8_t ) ( rx_rd + 6 )];
				switch(protocol)
				{
					case 0x01://����can filter_id
					{
						uint16_t id = rxBuffer[( uint8_t ) ( rx_rd + 7 )]; 
						id = (id << 8) | rxBuffer[( uint8_t ) ( rx_rd + 8 )]; 
						Default_Conf_Dat.Stu_Dat.filter_id = id;
					}
					break;
					case 0x02://dest_std_id
					{
						uint16_t id = rxBuffer[( uint8_t ) ( rx_rd + 7 )]; 
						id = (id << 8) | rxBuffer[( uint8_t ) ( rx_rd + 8 )]; 
						Default_Conf_Dat.Stu_Dat.dest_std_id = id;
					}
					break;
					case 0x03://�����������
					{
						uint8_t tm = rxBuffer[( uint8_t ) ( rx_rd + 7 )]; 
						Default_Conf_Dat.Stu_Dat.tm_online_signal_inteval = tm * 20;
					}
					break;
					default:
					break;
				}
				save_conf_to_flash();
				rx_rd += 10 + len;
			}
		}

		else
		{
			rx_rd++;
		}

	}

}

/*  ���ð��ӵ�ַ�����ķ���
head + sn + len + data + crc
0xE0 0x55 0xAA + 0 0 + 0x03 + 0x01 0x13 0x14 + 0 0 0 0



*/

