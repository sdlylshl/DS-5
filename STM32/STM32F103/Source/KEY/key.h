/*
 * key.h
 *
 *  Created on: 2015年3月17日
 *      Author: souls
 */

#ifndef KEY_H_
#define KEY_H_
#include "stm32f10x.h"

#define KEY_A 0x0FD
#define KEY_B 0x0FB
#define KEY_C 0x0F7
#define KEY_1 0x1EE
#define KEY_2 0x1ED
#define KEY_3 0x1EB
#define KEY_4 0x1DE
#define KEY_5 0x1DD
#define KEY_6 0x1DB
#define KEY_7 0x1BE
#define KEY_8 0x1BD
#define KEY_9 0x1BB

#define KEY_0 0x17D

#define KEY_F1 0x1E7
#define KEY_F2 0x1D7
#define KEY_F3 0x1B7
#define KEY_F4 0x177

#define KEY_STAR 0x17E
#define KEY_SHARP 0x17B

#define KEY_PRESSED 0
extern uint8_t isKeyRelease(void);
extern uint8_t isKeyPress(void);
extern void KeyInit(void);
extern  uint16_t KeySacn(void);
extern uint16_t GetKey(void);
#endif /* KEY_H_ */
