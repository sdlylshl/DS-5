#include "LCD12864P.h"

/*******************延时函数*******************/
void delay() {
	u16 a, b, c;
	for (c = 1600; c > 0; c--)
		for (b = 100; b > 0; b--)
			for (a = 16; a > 0; a--)
				;
}
/********************主函数********************/

int LCD12864P_main(void) {

	LCD12864P_Init();
	//LCD_Clear();
	LCD_ShowStringXY(1, 0, "2014-10-23 16:35");

	return 0;
}

/*********************************************/
