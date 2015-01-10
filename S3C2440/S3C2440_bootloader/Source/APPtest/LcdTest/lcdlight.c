/**************************************************************
The initial and control for 640×480 16Bpp TFT LCD----VGA
**************************************************************/


//#include <string.h> 
//#include <stddef.h>           /* needed for definition of NULL */
//#include "LCD_Private.H"      /* private modul definitions & config */
//#include "GUI_Private.H"
//#include "GUIDebug.h"
//#include "LCD_0.h"            /* Defines for first display */


#include "def.h"

#include "LCD_Driver.h"
#include "LCD_Config.h"
extern U16 KeyScan(void);
extern void Delay(int time);
extern void LcdBkLtSet(U32 Freq);
void lcdLightTest(){
		int i;
		Lcd_ClearScr(0x0);   //黑色
		Versions();
		print(0,2,"Starting LCD Test",0xFF);
		print(0,20,"Press any key is continue !",0xFF);
	//	while(!KeyScan());
	for(i=0;i<100;i++){
		//Delay(65535);
		//Delay(65535);
		LcdBkLtSet(i);
		Delay(2535);
		}
}