#ifndef __HARDWARE_H
#define __HARDWARE_H
#include "Types.h"
#include "wigend.h"
#include "led.h"
#include "loopback.h"
#include "w5500.h"
#include "delay.h"
#include "flash.h"
#include "timer.h"
#include "cardMachine.h"
#include "socket.h"
#include "usart.h"
#include "config.h"
#include "bar.h"
#include "spi_flash.h"
#include "i2c_ee.h"
#include "SST25_Flash.h"

extern uint8_t flag_get_Induction_Car[7];
extern uint8_t flag_get_key_Card ;
extern uint8_t flag_null_key_Card ;
extern uint8_t flag_test_get_key_Card ;
extern uint8_t flag_test_null_key_Card;
extern uint16_t GPIO_Pin_Input_Array[];
extern Boolean flag_check_card_been_got_away_state;

extern Boolean check_test_card_state ;

extern uint8_t sn_Induction_Car;
void hardware_init(void);

#endif

