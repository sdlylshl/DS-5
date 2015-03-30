#ifndef __CAN_DUAL_H
#define __CAN_DUAL_H

void CAN_dual_Config(void);
void Init_dual_RxMes(CanRxMsg *RxMessage);
void NVIC_dual_Config(void);
int can_dual_test(void);

#endif

