/*********************************************
  NAME: memtest.c
  DESC: test SDRAM of SMDK2440 b/d
  HISTORY:
  03.27.2002:purnnamu: first release
 *********************************************/

#include "def.h"
#include "2440addr.h"
#include "2440lib.h"
#include "2440slib.h"
#include "mmu.h"


#include "LCD_Driver.h"
#include "stdio.h"
#include "Keyboard.h"

extern void memtest(void)
{
	//Lcd_ClearScr(0x0);   //ºÚÉ«
		Versions();
		MemoryTest();
	//	print(0,20,"Press any key is continue !",0xFF);
		{
		print(0,20,"Press any key is continue !",0xFF);
		while(!KeyScan());
		//Lcd_ClearScr(0x0);   //ºÚÉ«
		//Versions();
		}
while(!(KEY_BACK==KeyScan()));

}
