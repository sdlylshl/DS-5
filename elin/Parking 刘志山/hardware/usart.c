#include "usart.h"
#include "hardware.h"

uint8_t rxBuffer[256];
uint8_t rx_rd = 0;
uint8_t rx_wr = 0;
Boolean flag_rs485_card_nr[3];
uint8_t rxBuffer5[256];
uint8_t rx_rd5 = 0;
uint8_t rx_wr5 = 0;

void USART1_IRQHandler(void)                	//串口1中断服务程序
	{
	uint8_t Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  
		{
			Res =USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
			rxBuffer[rx_wr++] = Res;
			UART5_Put_Char(Res);
     }
} 
	
void USART2_IRQHandler(void)                	//串口1中断服务程序
{
	uint8_t Res;
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		Res = USART_ReceiveData(USART2);
		
	}	 

}
void USART3_IRQHandler(void)                	
{
	uint8_t Res;
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断
	{
		Res = USART_ReceiveData(USART3);
	}
}
void UART4_IRQHandler(void)                	
{
	uint8_t Res;
	if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)  //接收中断
	{
		Res = USART_ReceiveData(UART4);
		UART1_Put_Char(Res);
	}
}
void UART5_IRQHandler(void)                	
{
	uint8_t Res;
	if (USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)  //接收中断
	{
		Res = USART_ReceiveData(UART5);
		if (Res == 0x06){
			UART5_Put_Char(0x05);
		 //UART1_Put_Char(Res);
		}
		else{
			rxBuffer5[rx_wr5++] = Res;
			
		}
	}
//		  if(USART_GetITStatus(UART5, USART_IT_TXE) != RESET)
//		  {   
//				/* Write one byte to the transmit data register */
//				USART_SendData(UART5, 0x05);                    
//			
//				/* Clear the USART1 transmit interrupt */
//				USART_ClearITPendingBit(UART5, USART_IT_TXE); 
//			
//		  }	 

}
void UART_Put_Char(USART_TypeDef* uart,unsigned char DataToSend)
{
	//将要发送的字节写到UART的发送缓冲区
	USART_SendData(uart, (unsigned char)DataToSend);
	//等待发送完成
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

void UART3_Put_Char(unsigned char DataToSend)
{
	UART_Put_Char(USART3, DataToSend);
}

void UART4_Put_Char(unsigned char DataToSend)
{
	UART_Put_Char(UART4, DataToSend);
}

void UART5_Put_Char(unsigned char DataToSend)
{
	UART_Put_Char(UART5, DataToSend);
}

uint8_t last_buf[3];
Boolean flag_card_waiting = FALSE;
Boolean flag_card_recycling = FALSE;
void decode_rs232_buf()
{
	if ((rx_wr5 + 256 - rx_rd5) % 256 > 0)
	{
		if (rxBuffer5[rx_rd5] == 0x02)
		{
			if ((rx_wr5 + 256 - rx_rd5) % 256 >= 8)
			{
				if (rxBuffer5[(uint8_t)(rx_rd5 + 1)] == 0x53 && rxBuffer5[(uint8_t)(rx_rd5 + 2)] == 0x46 && rxBuffer5[(uint8_t)(rx_rd5 + 6)] == 0x03)
				{
					uint8_t s1 = 0, s2 = 0, s3 = 0;
					s1 = rxBuffer5[(uint8_t)(rx_rd5 + 3)];
					s2 = rxBuffer5[(uint8_t)(rx_rd5 + 4)];
					s3 = rxBuffer5[(uint8_t)(rx_rd5 + 5)];
					if ((s3 & 0x04) == 0x04) //卡位于卡嘴位置
					{
						flag_card_waiting = TRUE;
					}
					if ((s3 & 0x04) == 0 && (s1 & 0x04) != 0x04 && flag_card_waiting == TRUE) //卡被取走
					{
						flag_check_card_been_got_away_state = FALSE;
						flag_card_waiting = FALSE;
						send_card_nr_to_server(Conf_Dat.Stu_Dat.rs232_bind_card_nr ,card_to_sending);
						card_to_sending[0] = 0;
						card_to_sending[1] = 0;
						card_to_sending[2] = 0;
						card_to_sending[3] = 0;
					}



					if ((s1 & 0x04) == 0x04)
					{
						flag_card_recycling = TRUE;
						
					}
					if ((s1 & 0x04) == 0 && flag_card_recycling == TRUE)
					{
						flag_card_recycling = FALSE;
						send_card_recycling_msg();
						UART1_Put_Char(s1);
						UART1_Put_Char(s2);
						UART1_Put_Char(s3);
					}

					

					if (check_test_card_state)
					{
						UART1_Put_Char(s1);
						UART1_Put_Char(s2);
						UART1_Put_Char(s3);
						check_test_card_state = FALSE;
					}

				}
				rx_rd5 += 8;
			}
		}

		else
		{
			rx_rd5++;
		}

	}

}

