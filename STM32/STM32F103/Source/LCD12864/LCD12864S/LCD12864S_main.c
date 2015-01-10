#include"LCD12864S.h"

/****************************************/

void LCD12864S_main(void) {
	LCD_Init();
	LCD_Clear();
	Display_String(0, 0, "");
	Display_String(0, 2, "STM32");
	while (1) {
	}
	//return 0;
}

/*********************************************/
