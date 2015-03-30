#ifndef __CAN_H
#define __CAN_H
#include "Types.h"
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
void can_init(void);
void CAN_SetMsg(void);
void can_send_Msg(Stu_Can_Msg * msg);
void decode_can_rx_msg(void);
#endif
