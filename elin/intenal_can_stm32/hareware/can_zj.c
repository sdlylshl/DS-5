#include "hardware.h"

#include "can.h"
#include "led.h"
#include "delay.h"
#include "usart.h"

u8 CAN_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		CAN_InitTypeDef CAN_InitStructure;

		CAN_FilterInitTypeDef CAN_FilterInitStructure;

		 NVIC_InitTypeDef NVIC_InitStructure;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//?? PORTA ??
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//?? CAN1 ??
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //????
		GPIO_Init(GPIOB, &GPIO_InitStructure); //??? IO
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//????
		GPIO_Init(GPIOB, &GPIO_InitStructure);//??? IO

		CAN_InitStructure.CAN_TTCM=DISABLE; 
		CAN_InitStructure.CAN_ABOM=DISABLE; 
	  CAN_InitStructure.CAN_AWUM=DISABLE; 
	  CAN_InitStructure.CAN_NART=ENABLE; 
	  CAN_InitStructure.CAN_RFLM=DISABLE; 
	  CAN_InitStructure.CAN_TXFP=DISABLE; 
	  CAN_InitStructure.CAN_Mode= mode;  
		CAN_InitStructure.CAN_SJW=tsjw; 
		CAN_InitStructure.CAN_BS1=tbs1; 
		CAN_InitStructure.CAN_BS2=tbs2;
		CAN_InitStructure.CAN_Prescaler=brp; 
		CAN_Init(CAN1, &CAN_InitStructure); 
		CAN_FilterInitStructure.CAN_FilterNumber=0;  
		CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
		CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; 
		CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;
		CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
		CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;
		CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
		CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;
		CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; 
		CAN_FilterInit(&CAN_FilterInitStructure);

		CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);
		 NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
		 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 
		 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
		 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		 NVIC_Init(&NVIC_InitStructure);

		return 0;
}
#if CAN_RX0_INT_ENABLE 
void USB_LP_CAN1_RX0_IRQHandler(void)
{
		CanRxMsg RxMessage;
		int i=0;
		CAN_Receive(CAN1, 0, &RxMessage);
		for(i=0;i<8;i++)
			printf("rxbuf[%d]:%d\r\n",i,RxMessage.Data[i]);
}
#endif

u8 Can_Send_Msg(u8* msg,u8 len)
{
		 u8 mbox;
		 u16 i=0;
		 CanTxMsg TxMessage;
		 TxMessage.StdId=0x12; 
		 TxMessage.ExtId=0x12; 
		 TxMessage.IDE=0; 
		 TxMessage.RTR=0; 
		 TxMessage.DLC=len; 
		 for(i=0;i<8;i++)
				TxMessage.Data[i]=msg[i]; 
		 mbox= CAN_Transmit(CAN1, &TxMessage);
		 i=0;
		 while((CAN_TransmitStatus(CAN1, mbox)!=CAN_TxStatus_Failed)&&(i<0xfff))i++; 

		 if(i>=0xfff)return 1;
		 return 0;
}

u8 Can_Receive_Msg(u8 *buf)
{  
		u32 i;
		CanRxMsg RxMessage;
		if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)
			return 0;
		CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
		for(i=0;i<8;i++)
				buf[i]=RxMessage.Data[i];
		return RxMessage.DLC;
}

int can_jz_test(void)
{
		u8 canbuf[8];
		u8 mode=CAN_Mode_LoopBack;
		//NVIC_Configuration(); 
		CAN_Mode_Init(CAN_SJW_1tq,CAN_BS1_8tq,CAN_BS2_7tq,5,CAN_Mode_LoopBack);
		printf("CAN_Mode_Init");
		while(1)
		{
			Can_Send_Msg(canbuf,8);

			mode=!mode;
			CAN_Mode_Init(CAN_SJW_1tq,CAN_BS1_8tq,CAN_BS2_7tq,5,mode);


			Can_Receive_Msg(canbuf);

			delay_ms(500);
		}
}

void USB_LP_CAN1_RX0_IRQHandler(void)

{
  CanRxMsg RxMessage;

  RxMessage.StdId=0x00;
  RxMessage.ExtId=0x00;
  RxMessage.IDE=0;
  RxMessage.DLC=0;
  RxMessage.FMI=0;
  RxMessage.Data[0]=0x00;
  RxMessage.Data[1]=0x00;

  CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);

  if((RxMessage.ExtId==0x1234) && (RxMessage.IDE==CAN_ID_EXT)
     && (RxMessage.DLC==2) && ((RxMessage.Data[1]|RxMessage.Data[0]<<8)==0xDECA))
  {
    LED2 = 1; 
  }
  else
  {
    LED2 = 0; 
  }
}
void CAN1_RX0_IRQHandler(void)
{
  CanRxMsg RxMessage;

  RxMessage.StdId=0x00;
  RxMessage.ExtId=0x00;
  RxMessage.IDE=0;
  RxMessage.DLC=0;
  RxMessage.FMI=0;
  RxMessage.Data[0]=0x00;
  RxMessage.Data[1]=0x00;

  CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);

  if((RxMessage.ExtId==0x1234) && (RxMessage.IDE==CAN_ID_EXT)
     && (RxMessage.DLC==2) && ((RxMessage.Data[1]|RxMessage.Data[0]<<8)==0xDECA))
  {
    LED2 = 1; 
  }
  else
  {
    LED2 = 0; 
  }
}


