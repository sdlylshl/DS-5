#include "Types.h"
#include "hardware.h"
#define CAN_BAUDRATE  1000
uint32_t presentTime;
uint32_t curTime = 0;
uint32_t lastTime = 0;
uint32_t keyTime = 0;
void Init_RxMes(CanRxMsg *RxMessage);
void NVIC_Config(void);
void CAN_Config(void);
CanTxMsg TxMessage;
CanRxMsg RxMessage;
volatile TestStatus TestRx;
int tmp = 0;

TestStatus CAN_Polling(void);
TestStatus CAN_Interrupt(void);
 int main()
 {		
	 hardware_init();

	LED1 = 0;
	LED2 = 1;
	LED3 = 0;
printf( "\r\n***** 这是一个双CAN通讯实验******** \r\n");
	 for(tmp = 0;tmp < 3;tmp++)
	 printf("%2x ",Default_Conf_Dat.arr_dat[tmp]);
	 printf("\r\n");
	Default_Conf_Dat.Stu_Dat.tm_online_signal_inteval =0x100; 
	while (1)
	{
		CAN_SetMsg();
		
		//decode_rs232_buf();
		//scan_alarm_state();
		//heart_signal_send();
		//decode_can_rx_msg();
		//delay_ms(500);
//		delay_ms(500);
		LED2 = !LED2;
		
//		CAN_SetMsg();			

////		printf("\r\n将要发送的报文内容为：\r\n");
//		printf("send ：0x%x 0x%x 0x%x 0x%x 0x%x \r\n",Default_Conf_Dat.Stu_Dat.filter_id,
//						Default_Conf_Dat.Stu_Dat.dest_ext_id,TxMessage.Data[0],TxMessage.Data[1],RxMessage.Data[7]);

//		/*发送消息 “CDAB”**/
//		CAN_Transmit(CAN1, &TxMessage);		
		delay_ms(1000);

	}  
}




