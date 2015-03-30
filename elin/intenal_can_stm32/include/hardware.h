#ifndef __HARDWARE_H
#define __HARDWARE_H
#include "Types.h"
#include "led.h"
#include "delay.h"
#include "flash.h"
#include "timer.h"
#include "usart.h"
#include "config.h"
#include "RS485.h"
#include "ds1302.h"
#include "can.h"

#define CAN_MSG_LEN 20
#define CAN_MSG_RX_LEN 20
extern Stu_Can_Msg  can_msg_lst[CAN_MSG_LEN];
extern Stu_Can_Msg  can_msg_rx_lst[CAN_MSG_RX_LEN];
extern uint8_t flag_get_Induction_Car[7];
extern uint8_t flag_get_key_Card ;
extern uint8_t flag_null_key_Card ;
extern uint8_t flag_test_get_key_Card ;
extern uint8_t flag_test_null_key_Card;
extern uint16_t GPIO_Pin_Input_Array[];
extern Boolean flag_check_card_been_got_away_state;
extern Boolean flag_send_heart_signal;
extern Boolean check_test_card_state ;
extern uint8_t times_heart_signal_lost;
extern __IO uint16_t ADC_ConvertedValue;

extern uint8_t sn_Induction_Car;
void hardware_init(void);
void scan_alarm_state(void);
void heart_signal_send(void);

Stu_Can_Msg * get_a_msg(void);
Stu_Can_Msg * get_a_rx_msg(void);
#endif

