#ifndef __FLASH_H
#define __FLASH_H
#include "Types.h"



extern UNI_CONF_DAT Conf_Dat;
extern UNI_CONF_DAT Default_Conf_Dat;
extern uint16_t conf_arr_len;
extern uint16_t  box_timeout_close_door;
extern uint16_t  box_timeout_get_express;
extern uint16_t  box_timeout_get_user;
extern uint16_t  box_timeout_get_temp;
extern uint16_t  box_timeout_express_mode;
extern uint16_t  box_timeout_input;
extern uint8_t   boxes_num_conf;

extern void  read_conf_from_flash(void);
void save_conf_to_flash(void);
extern void write_a_boxMgr_Msg_to_flash(uint8_t boxNr);
void write_a_boxMgr_Msg_to_flash(uint8_t boxNr);
extern void save_user_msg_to_flash(uint8_t nr);
	
void save_conf(void);
void read_conf(void);
void flash_init(void);

#endif
