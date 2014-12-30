
#include "def.h"
#include "2440addr.h"
#include "2440lib.h"
#include "LCD_Driver.h"
#include "stdio.h"

#include "Keyboard.h"
//#include "ver.h"
extern void Versions(void);

extern void KeyLight(void)
{
		
		Lcd_ClearScr(0x0);   //ºÚÉ«
		Versions();
		print(0,2,"Starting key Light Test",0xFF);
		
		print(0,4,"Press Enter key switch Light ",0xFF);
		print(0,6,"Press BACK Exit ",0xFF);
	while(1)
		{
			if(KEY_ENTER==KeyScan())
				rGPBDAT =((rGPBDAT&(~(1<<2))) | ((~rGPBDAT)&(1<<2)));
			if(KEY_BACK==KeyScan())
				break;
		Delay(50000);
	
		}
	rGPBDAT = 0x00001001;
	Lcd_ClearScr(0x0);   //ºÚÉ«
	Versions();
	
}		