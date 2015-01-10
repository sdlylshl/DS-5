/******************** (C) COPYRIGHT 2012 WildFire Team **************************
 * 文件名  ：main.c
 * 描述    ：NRF24L01无线w传输测试实验        
 * 实验平台：野火STM32开发板	
 * 库版本  ：ST3.5.0
 *
 * 作者    ：wildfire team 
 * 论坛    ：http://www.amobbs.com/forum-1008-1.html
 * 淘宝    ：http://firestm32.taobao.com
**********************************************************************************/
#include "stdio.h"
#include "SPI_NRF.h"
//usart1
#include "./System/System_config.h"

u8 status;	//用于判断接收/发送状态
u8 txbuf[4]={0,1,2,3};	 //发送缓冲
u8 rxbuf[4];			 //接收缓冲
int i=0;
uint8_t  nrf__test(void);
int NRF_master_main(void)
{
  
   SPI_NRF_Init();
   
nrf__test();

//	printf("\r\n 这是一个 NRF24L01 无线传输实验 \r\n");
//   	printf("\r\n 这是无线传输 主机端 的反馈信息\r\n");
//	printf("\r\n   正在检测NRF与MCU是否正常连接。。。\r\n");

	 /*检测NRF模块与MCU的连接*/
   	status = NRF_Check(); 

	/*判断连接状态*/  
   if(status == SUCCESS)	   
   		 printf("\r\n      NRF与MCU连接成功！\r\n");  
   else	  
   	     printf("\r\n  NRF与MCU连接失败，请重新检查接线。\r\n");


 while(1)
		{
//	   	printf("\r\n 主机端 进入自应答发送模式\r\n");
	
	 	NRF_TX_Mode();	
			/*开始发送数据*/	
		status = NRF_Tx_Dat(txbuf);	  
		  
		  /*判断发送状态*/
		  switch(status)
		  	{
		  	  case MAX_RT:
//			 	 	printf("\r\n 主机端 没接收到应答信号，发送次数超过限定值，发送失败。 \r\n");
				 break;
	
			  case ERROR:
	//		  	 	printf("\r\n 未知原因导致发送失败 %x\r\n",status);
				 break;
	
			  case TX_DS:
//			  		printf("\r\n 主机端 接收到 从机端 的应答信号，发送成功！ \r\n");	 		
				 break;  								
		  	}			   	
	
//	 	printf("\r\n 主机端 进入接收模式。 \r\n");	
		NRF_RX_Mode();
	 // nrf__test();
			/*等待接收数据*/
		status = NRF_Rx_Dat(rxbuf);
	
			/*判断接收状态*/
			switch(status)
			{
			 case RX_DR:
			 	for(i=0;i<4;i++)
				{					
	//				printf("\r\n 主机端 接收到 从机端 发送的数据为：%d \r\n",rxbuf[i]);
					txbuf[i] =rxbuf[i];
				}
				  break;
	
			 case ERROR:
//				  	printf("\r\n 主机端 接收出错。status%x   \r\n",status);
				  break;  		
			}
		} 
			
}


int NRF_device_main(void)
{      

/*SPI接口初始化*/   
   SPI_NRF_Init(); 

	//printf("\r\n 这是一个 NRF24L01 无线传输实验 \r\n");
  // 	printf("\r\n 这是无线传输 从机端 的反馈信息\r\n");
	//printf("\r\n   正在检测NRF与MCU是否正常连接。。。\r\n");
	 
	 /*检测NRF模块与MCU的连接*/
   	status = NRF_Check();   		
		 if(status == SUCCESS)
        printf("\r\n       nrf2401 connect ok !	\r\n");
		 	   //		 printf("\r\n      NRF与MCU连接成功\r\n");  
    else
        printf("\r\n       nrf2401 connect erro ! \r\n"); 
	   //	     printf("\r\n   正在检测NRF与MCU是否正常连接。。。\r\n");
			  
while(1)
	{  		 	
 	//printf("\r\n 从机端 进入接收模式\r\n"); 
	NRF_RX_Mode();
	 
	/*等待接收数据*/
	status = NRF_Rx_Dat(rxbuf);

	   /*判断接收状态*/
	if(status == RX_DR)
	{
	 for(i=0;i<4;i++)
	 {	
	 //	printf("\r\n 从机端 接收到 主机端 发送的数据为：%d \r\n",rxbuf[i]); 
	 	/*把接收的数据+1后发送给主机*/
	 	rxbuf[i]+=1;	  
		txbuf[i] = rxbuf[i]; 
		}
	}   
		//printf("\r\n 从机端 进入应答发送模式\r\n"); 
	  	NRF_TX_Mode();

		/*不断重发，直至发送成功*/	  
	 do
	   { 
				if(!i++)
					break;
				
		status = NRF_Tx_Dat(txbuf);
				if(status & TX_DS)
					break;
		printf(" %x ",status); 
		}while(status == MAX_RT);
	} 
} 

void nrf_recv(){
	NRF_RX_Mode();
	 
	/*等待接收数据*/
	status = NRF_Rx_Dat(rxbuf);

	   /*判断接收状态*/
	if(status == RX_DR)
	{
	 for(i=0;i<4;i++)
	 {	
	 	//printf("\r\n 从机端 接收到 主机端 发送的数据为：%d \r\n",rxbuf[i]); 
	 	/*把接收的数据+1后发送给主机*/
	 	rxbuf[i]+=1;	  
		txbuf[i] = rxbuf[i]; 
		}
	}   
}
void nrf_send(){

	//printf("\r\n 从机端 进入自应答发送模式\r\n"); 
	  	NRF_TX_Mode();

		/*不断重发，直至发送成功*/	  
	 do
	   { 	  
		status = NRF_Tx_Dat(txbuf);
		}while(status == MAX_RT);
	 
Delay_ms(500);
}

void nrf_main0(){
	  SPI_NRF_Init();	
//NRF_master_main();
	NRF_device_main();
	while(1){
	nrf_recv();
	//nrf_send();
	}
}
/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/

