#include "stm32f10x.h"
/***********************************************************************
 ʹ��stm32оƬ�����к�SerialNum
 ************************************************************************/
uint8_t SID[12];
uint8_t * GetSID() {
	uint16_t Device_Serial0, Device_Serial1;
	uint32_t Device_Serial2, Device_Serial3;

	Device_Serial0 = *(uint16_t*) (0x1FFFF7E8);
	Device_Serial1 = *(uint16_t*) (0x1FFFF7EA);
	Device_Serial2 = *(uint32_t*) (0x1FFFF7EC);
	Device_Serial3 = *(uint32_t*) (0x1FFFF7F0);

	SID[0] = (uint8_t) ((Device_Serial0 & 0x00FF));
	SID[1] = (uint8_t) ((Device_Serial0 & 0xFF00) >> 8);
	SID[2] = (uint8_t) ((Device_Serial1 & 0x00FF) >> 8);
	SID[3] = (uint8_t) (Device_Serial1 & 0xFF00);

	SID[4] = (uint8_t) ((Device_Serial2 & 0x000000FF));
	SID[5] = (uint8_t) ((Device_Serial2 & 0x0000FF00) >> 8);
	SID[6] = (uint8_t) ((Device_Serial2 & 0x00FF0000) >> 16);
	SID[7] = (uint8_t) ((Device_Serial2 & 0xFF000000) >> 16);

	SID[8] = (uint8_t) ((Device_Serial3 & 0x000000FF));
	SID[9] = (uint8_t) ((Device_Serial3 & 0x0000FF00) >> 8);
	SID[10] = (uint8_t) ((Device_Serial3 & 0x00FF0000) >> 16);
	SID[11] = (uint8_t) ((Device_Serial3 & 0xFF000000) >> 16);
	return SID;
}
