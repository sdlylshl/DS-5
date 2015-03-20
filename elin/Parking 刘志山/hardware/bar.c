#include "bar.h"

uint8_t flag_Line_Relay_ON[4] = {0};


void bar_scan()
{

	LED4 = !(Key_D0 | Key_D1 | Key_D2 | Key_D3);
	Beep = !LED4;
	if (Key_D0)
		Line_Relay1 = Key_D0;
	else
	{
		Line_Relay1 = flag_Line_Relay_ON[0];
	}
	if (Key_D1)
		Line_Relay2 = Key_D1;
	else
	{
		Line_Relay2 = flag_Line_Relay_ON[1];
	}
	if (Key_D2)
		Line_Relay3 = Key_D2;
	else
	{
		Line_Relay3 = flag_Line_Relay_ON[2];
	}
	if (Key_D3)
		Line_Relay4 = Key_D3;
	else
	{
		Line_Relay4 = flag_Line_Relay_ON[3];
	}
}