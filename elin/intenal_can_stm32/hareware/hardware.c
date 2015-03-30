#include "Types.h"
#include "hardware.h"

uint8_t flag_get_Induction_Car[7] = { 0 };
uint8_t flag_get_key_Card = 0;
uint8_t flag_test_get_key_Card = 0;
uint8_t flag_null_key_Card = 2;
uint8_t flag_test_null_key_Card = 2;
uint8_t flag_down_state = 0;
Boolean flag_check_card_been_got_away_state = FALSE;

Boolean check_test_card_state = FALSE;
Boolean flag_send_heart_signal = FALSE;
uint8_t times_heart_signal_lost = 0;
uint16_t heart_signal_inteval = 20;
Stu_Can_Msg  can_msg_lst[CAN_MSG_LEN];
Stu_Can_Msg  can_msg_rx_lst[CAN_MSG_RX_LEN];
Stu_Can_Msg * get_a_msg()
{
	int i = 0;
	for(;i < CAN_MSG_LEN;i++)
	{
		if( can_msg_lst[i].times == 0)
		{
			can_msg_lst[i].times = 1;
			return &can_msg_lst[i];
		}
	}
	
	return NULL;
}
Stu_Can_Msg * get_a_rx_msg()
{
	int i = 0;
	for(;i < CAN_MSG_RX_LEN;i++)
	{
		if( can_msg_rx_lst[i].times == 0)
		{
			can_msg_rx_lst[i].times = 1;
			return &can_msg_rx_lst[i];
		}
	}
	
	return NULL;
}

uint16_t GPIO_Pin_Input_Array[] = {
	GPIO_Pin_8,
	GPIO_Pin_8,
	GPIO_Pin_9,
	GPIO_Pin_10,
	GPIO_Pin_11,
	GPIO_Pin_12,
	GPIO_Pin_13,
	GPIO_Pin_14,
	GPIO_Pin_15
};

uint8_t sn_Induction_Car = 0;

void hardware_init()
{
//	users_init();
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
	delay_init();	    	 //延时函数初始化	
//	DS1302_Init();
	flash_init();
	read_conf_from_flash();
	timer_init();

	//adc_multi_init();
	//ADC1_Init();
	//IIC_Init();
	usart_init();
	LED_init();
//	RS485_init();
	//beep_init();
	//jdq_init();
	//w5500_init();
	can_init();
//	ds1302_set_time_test();
//	Start1302();
}

uint32_t alarm_bit = 0;
void scan_alarm_state(void)
{
	
	if(alarm_bit > 0)
	{
		Stu_Can_Msg * msg = get_a_msg();
		if(msg != NULL)
		{
			msg->len = 7;
			msg->data[0] = NET_UDP_INPUT_ALARM_STAT;
			msg->data[1] = Default_Conf_Dat.Stu_Dat.filter_id>>8 & 0xff;
			msg->data[2] = Default_Conf_Dat.Stu_Dat.filter_id & 0xff;
			msg->data[3] = alarm_bit >> 24 & 0xff;
			msg->data[4] = alarm_bit >> 16 & 0xff;
			msg->data[5] = alarm_bit >> 8 & 0xff;
			msg->data[6] = alarm_bit & 0xff;
		}
		else
		{
			printf("Can Msg Lst 已填满！");
		}

		can_send_Msg(msg);
	}
}

void heart_signal_send(void)
{
	if(flag_send_heart_signal)//发送心跳
	{
			Stu_Can_Msg * msg = get_a_msg();
			if(msg != NULL)
			{
					msg->len = 3;
					msg->data[0] = NET_UDP_ALIVE;
					msg->data[1] = Default_Conf_Dat.Stu_Dat.filter_id>>8 & 0xff;
					msg->data[2] = Default_Conf_Dat.Stu_Dat.filter_id & 0xff;
			}
			else
			{
				printf("[heart_signal_send]Can Msg Lst is full !\r\n");
			}
			flag_send_heart_signal = FALSE;
			can_send_Msg(msg);
	}
}




//参数配置



//探测器故障报警
//电池电压过低报警
//断电报警
//获取布防状态
//配置联动
//设置心跳的间隔

//探测器故障报警
void alarm_detector_error()
{
			Stu_Can_Msg * msg = get_a_msg();
			if(msg != NULL)
			{
					msg->len = 3;
					msg->data[0] = NET_UDP_DETECTOR_LOSS;
					msg->data[1] = Default_Conf_Dat.Stu_Dat.filter_id>>8 & 0xff;
					msg->data[2] = Default_Conf_Dat.Stu_Dat.filter_id & 0xff;
			}
			else
			{
				printf("[heart_signal_send]Can Msg Lst 已填满！");
			}
			flag_send_heart_signal = FALSE;
			can_send_Msg(msg);
}

//电池电压过低报警
void alarm_battery_low_power()
{
			Stu_Can_Msg * msg = get_a_msg();
			if(msg != NULL)
			{
					msg->len = 3;
					msg->data[0] = NET_UDP_ALARM_BATTERY_LOW_POWER;
					msg->data[1] = Default_Conf_Dat.Stu_Dat.filter_id>>8 & 0xff;
					msg->data[2] = Default_Conf_Dat.Stu_Dat.filter_id & 0xff;
			}
			else
			{
				printf("[heart_signal_send]Can Msg Lst 已填满！");
			}
			flag_send_heart_signal = FALSE;
			can_send_Msg(msg);
}

//断电报警
void alarm_power_off()
{
			Stu_Can_Msg * msg = get_a_msg();
			if(msg != NULL)
			{
					msg->len = 3;
					msg->data[0] = NET_UDP_ALARM_POWEROFF;
					msg->data[1] = Default_Conf_Dat.Stu_Dat.filter_id>>8 & 0xff;
					msg->data[2] = Default_Conf_Dat.Stu_Dat.filter_id & 0xff;
			}
			else
			{
				printf("[heart_signal_send]Can Msg Lst 已填满！");
			}
			flag_send_heart_signal = FALSE;
			can_send_Msg(msg);
}

void alarm_is_active()
{
			Stu_Can_Msg * msg = get_a_msg();
			if(msg != NULL)
			{
					uint16_t alarm_id = 0x1314;//探测器的设备id
					msg->len = 3;
					msg->data[0] = NET_UDP_INPUT_ALARM_STAT;
					msg->data[1] = alarm_id>>8 & 0xff;
					msg->data[2] = alarm_id & 0xff;
			}
			else
			{
				printf("[heart_signal_send]Can Msg Lst 已填满！");
			}
			flag_send_heart_signal = FALSE;
			can_send_Msg(msg);
}
/*
心跳由内部通信板主动发送，外部通信板收到后，本地标记，并回复内部通信板
两板间通信的数据，收到后，需要回复发送方，收发的数据也可以作为心跳信号

*/
// id(4) + cmd(1) + data
//uint8_t get_uint8_data(uint32_t dat)
//{
//	uint8_t ret_dat = 0, i = 0;
//	for (; i < 8; i++)
//	{
//		ret_dat |= (dat % 10) << i;
//		dat /= 10;
//	}
//	return  ret_dat;
//}
