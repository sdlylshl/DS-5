#ifndef __CARDMACHINE_H
#define __CARDMACHINE_H
#include "Types.h"
#include "usart.h"


void write_to_cardMachine(uint8_t * buf,uint8_t len);
void decode_usart1_dat(void);
void key_scan(void);
void send_card_nr_to_server(uint8_t nr, uint8_t dat[]);
void send_a_card(void);
void check_Induction_Car(void); //检查并发送地感信号
uint8_t get_Induction_Car(uint8_t nr);
void send_card_to_machine_mouth(void);
void send_check_card_machine_state_cmd(void);
Boolean search_card_in_list(uint8_t* dat);
void check_card_been_got_away_state(void);
void send_card_recycling_msg(void);

void add_new_card(uint8_t card_dat[]);
Boolean search_a_card(uint8_t human[], uint8_t cardId[], uint8_t tm[]);

Boolean check_card_is_exist(uint8_t cardId[]);
void parse_card_signal(void);
void get_card_Msg_by_lst_nr(uint16_t lst_nr, uint8_t card_dat[]);
void delete_all_cards(void);
Boolean check_card_exist(uint8_t cardId[]);
void get_All_card_Msg(uint16_t nr);
void delete_a_card_by_lst_nr(uint16_t lst_nr);
#endif
