/**************************************************************
The initial and control for 640��480 16Bpp TFT LCD----VGA
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
#include "Keyboard.h"
extern U16 KeyScan(void);
extern void Delay(int time);

extern void lcdtest(void)
{

		//Lcd_ClearScr(0x0);   //��ɫ
		Versions();
		//print(0,2,"Starting LCD Test",0xFF);
		//print(0,20,"Press any key is continue !",0xFF);
		//while(!KeyScan());
		Lcd_ClearScr(0xFFFFFF);   //��
		Delay(9000);
		Delay(9000);
		Delay(9000);
		Delay(9000);
	
		
		while(!KeyScan());
		Lcd_ClearScr(0xFF0000);   //��
		Delay(9000);
		Delay(9000);
		Delay(9000);
		Delay(9000);
	
		
		while(!KeyScan());
		Lcd_ClearScr(0x00FF00);   //��
		Delay(9000);
		Delay(9000);
		Delay(9000);
		Delay(9000);
	
	
		while(!KeyScan());
		Lcd_ClearScr(0x0000FF);   //��
		Delay(9000);
		Delay(9000);
		Delay(9000);
		Delay(9000);
		
		/*		
		while(!KeyScan());
		Lcd_ClearScr(0xFFFF00);   //��
		Delay(9000);
		Delay(9000);
		Delay(9000);
		Delay(9000);
		Delay(9000);
		Delay(9000);

		while(!KeyScan());
		Lcd_ClearScr(0xFF00FF);   //��
		Delay(9000);
		Delay(9000);
		Delay(9000);
		Delay(9000);
		Delay(9000);
		Delay(9000);
		Delay(9000);
		Delay(9000);
		
		while(!KeyScan());
		Lcd_ClearScr(0x00FFFF);   //��
		Delay(9000);
		Delay(9000);
		Delay(9000);
		Delay(9000);
		Delay(9000);
		Delay(9000);
		Delay(9000);
		Delay(9000);
		*/
		print(0,20,"Press any key is continue !",0);
		while(!(KEY_BACK==KeyScan()));
		
}