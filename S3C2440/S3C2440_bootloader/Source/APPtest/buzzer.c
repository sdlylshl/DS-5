
#include "def.h"
#include "2440addr.h"
#include "2440lib.h"
#include "LCD_Driver.h"
#include "Keyboard.h"

extern void Versions(void);


extern void buzTest(void)
{
       	Lcd_ClearScr(0x0);   //黑色
		Versions();
		print(0,2,"Starting Buzzer Test",0xFF);
		
		print(0,4,"Press Enter key switch buzzer ",0xFF);
		print(0,6,"Press CNT Exit ",0xFF);
	while(1)
		{
			if(KEY_ENTER==KeyScan())
				rGPBDAT =((rGPBDAT&(~(1))) | ((~rGPBDAT)&(1)));
			if(KEY_BACK==KeyScan())
				break;
		Delay(5000);
	
		}
	rGPBDAT = 0x00001001;	
	while(!(KEY_BACK==KeyScan()));
	
}		