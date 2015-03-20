#include "config.h"
#include "util.h"
#include <stdio.h>
#include <string.h>
#include "stm32f10x.h"


#include "stm32f10x_it.h"

#include "LCD.h"



extern old_arm_cmd;

int rs485_usart2_send(u8* cmd, u8 len)
{
	int i;

	//printf("------send %s\n", cmd);
	for(i=0; i<len; i++)
		//printf("0x%x ", cmd[i]);
	//printf("\n");

	TX_485;
	Delay_ms(50);
	
	for(i=0; i<len; i++)
	{
		#if 0
		// Write a character to the USART
		 USART_SendData(USART2, cmd[i]);
		 
		 //  Loop until the end of transmission
		 while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
		 {
		 }
		 #endif

		 
		{		   
			while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);	  
				USART_SendData(USART2,cmd[i]);
		}	 

		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);	
		}
	//GPIO_ResetBits(GPIOA,RS485_EN);
	Delay_ms(50);
	RX_485;
 	return 0;
}

void rs485_init()
{
	GPIO_ResetBits(GPIOA,RS485_EN);
}


void send_void_msg()
{
	u8 cmd[5];

	cmd[0] = 0xfc;
	cmd[1] = 0;
	cmd[2] = POLL_RSP;
	cmd[3] = ADDR;
	cmd[cmd[1]+4] = calcfcs(cmd+1, cmd[1]+3);
	rs485_usart2_send(cmd, cmd[1]+5);
}


void send_arm_msg()
{
	u8 cmd[5];

	cmd[0] = 0xfc;
	cmd[1] = 0;
	cmd[2] = old_arm_cmd;
	cmd[3] = ADDR;
	cmd[cmd[1]+4] = calcfcs(cmd+1, cmd[1]+3);
	rs485_usart2_send(cmd, cmd[1]+5);
}

void send_disarm_msg()
{
	u8 cmd[5];

	cmd[0] = 0xfc;
	cmd[1] = 0;
	cmd[2] = DISARM_CMD;
	cmd[3] = ADDR;
	cmd[cmd[1]+4] = calcfcs(cmd+1, cmd[1]+3);
	rs485_usart2_send(cmd, cmd[1]+5);
}


void rs485_test()
{
	rs485_usart2_send("hello", 5);
}

