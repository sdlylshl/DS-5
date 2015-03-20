#ifndef __WIGEN_H
#define __WIGEN_H
#include "Types.h"
#include "hardware.h"
extern Boolean flag_rs232_bind_wigen_card_nr[4];
extern uint8_t card_to_sending[4];


void check_wigen_buf(void);
void check_wigend_input_flag(void);

#endif

