/******************** (C) COPYRIGHT 2012 WildFire Team **************************
 * �ļ���  ��main.c
 * ����    ��NRF24L01����w�������ʵ��        
 * ʵ��ƽ̨��Ұ��STM32������	
 * ��汾  ��ST3.5.0
 *
 * ����    ��wildfire team 
 * ��̳    ��http://www.amobbs.com/forum-1008-1.html
 * �Ա�    ��http://firestm32.taobao.com
**********************************************************************************/
#include "stdio.h"
#include "SPI_NRF.h"
//usart1
#include "./System/System_config.h"

u8 status;	//�����жϽ���/����״̬
u8 txbuf[4]={0,1,2,3};	 //���ͻ���
u8 rxbuf[4];			 //���ջ���
int i=0;
uint8_t  nrf__test(void);
int NRF_master_main(void)
{
  
   SPI_NRF_Init();
   
nrf__test();

	printf("\r\n ����һ�� NRF24L01 ���ߴ���ʵ�� \r\n");
   	printf("\r\n �������ߴ��� ������ �ķ�����Ϣ\r\n");
	printf("\r\n   ���ڼ��NRF��MCU�Ƿ��������ӡ�����\r\n");

	 /*���NRFģ����MCU������*/
   	status = NRF_Check(); 

	/*�ж�����״̬*/  
   if(status == SUCCESS)	   
   		 printf("\r\n      NRF��MCU���ӳɹ���\r\n");  
   else	  
   	     printf("\r\n  NRF��MCU����ʧ�ܣ������¼����ߡ�\r\n");


 while(1)
		{
	   	printf("\r\n ������ ������Ӧ����ģʽ\r\n");
	
	 	NRF_TX_Mode();	
			/*��ʼ��������*/	
		status = NRF_Tx_Dat(txbuf);	  
		  
		  /*�жϷ���״̬*/
		  switch(status)
		  	{
		  	  case MAX_RT:
			 	 	printf("\r\n ������ û���յ�Ӧ���źţ����ʹ��������޶�ֵ������ʧ�ܡ� \r\n");
				 break;
	
			  case ERROR:
			  	 	printf("\r\n δ֪ԭ���·���ʧ�� %x\r\n",status);
				 break;
	
			  case TX_DS:
			  		printf("\r\n ������ ���յ� �ӻ��� ��Ӧ���źţ����ͳɹ��� \r\n");	 		
				 break;  								
		  	}			   	
	
	 	printf("\r\n ������ �������ģʽ�� \r\n");	
		NRF_RX_Mode();
	 // nrf__test();
			/*�ȴ���������*/
		status = NRF_Rx_Dat(rxbuf);
	
			/*�жϽ���״̬*/
			switch(status)
			{
			 case RX_DR:
			 	for(i=0;i<4;i++)
				{					
					printf("\r\n ������ ���յ� �ӻ��� ���͵�����Ϊ��%d \r\n",rxbuf[i]);
					txbuf[i] =rxbuf[i];
				}
				  break;
	
			 case ERROR:
				  	printf("\r\n ������ ���ճ���status%x   \r\n",status);
				  break;  		
			}
		} 
			
}


int NRF_device_main(void)
{      

/*SPI�ӿڳ�ʼ��*/   
   SPI_NRF_Init(); 

	printf("\r\n ����һ�� NRF24L01 ���ߴ���ʵ�� \r\n");
   	printf("\r\n �������ߴ��� �ӻ��� �ķ�����Ϣ\r\n");
	printf("\r\n   ���ڼ��NRF��MCU�Ƿ��������ӡ�����\r\n");
	 
	 /*���NRFģ����MCU������*/
   	status = NRF_Check();   		
	   if(status == SUCCESS)	   
	   		 printf("\r\n      NRF��MCU���ӳɹ�\r\n");  
	   else	  
	   	     printf("\r\n   ���ڼ��NRF��MCU�Ƿ��������ӡ�����\r\n");
			  
while(1)
	{  		 	
 	printf("\r\n �ӻ��� �������ģʽ\r\n"); 
	NRF_RX_Mode();
	 
	/*�ȴ���������*/
	status = NRF_Rx_Dat(rxbuf);

	   /*�жϽ���״̬*/
	if(status == RX_DR)
	{
	 for(i=0;i<4;i++)
	 {	
	 	printf("\r\n �ӻ��� ���յ� ������ ���͵�����Ϊ��%d \r\n",rxbuf[i]); 
	 	/*�ѽ��յ�����+1���͸�����*/
	 	rxbuf[i]+=1;	  
		txbuf[i] = rxbuf[i]; 
		}
	}   
		printf("\r\n �ӻ��� ����Ӧ����ģʽ\r\n"); 
	  	NRF_TX_Mode();

		/*�����ط���ֱ�����ͳɹ�*/	  
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
	 
	/*�ȴ���������*/
	status = NRF_Rx_Dat(rxbuf);

	   /*�жϽ���״̬*/
	if(status == RX_DR)
	{
	 for(i=0;i<4;i++)
	 {	
	 	printf("\r\n �ӻ��� ���յ� ������ ���͵�����Ϊ��%d \r\n",rxbuf[i]); 
	 	/*�ѽ��յ�����+1���͸�����*/
	 	rxbuf[i]+=1;	  
		txbuf[i] = rxbuf[i]; 
		}
	}   
}
void nrf_send(){

	printf("\r\n �ӻ��� ������Ӧ����ģʽ\r\n"); 
	  	NRF_TX_Mode();

		/*�����ط���ֱ�����ͳɹ�*/	  
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

