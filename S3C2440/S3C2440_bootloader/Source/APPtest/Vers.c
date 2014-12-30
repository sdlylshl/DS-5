
#include "def.h"
#include "2440addr.h"
#include "LCD_Driver.h"
#include "Keyboard.h"
#include "stdlib.h"     //rand()
/**
//u2440mon.h
extern U8 EEPROMISOK;       //EEPROM ¥ÌŒÛ±Í÷æ
//2440lib.h
extern void Uart_Printf(char *fmt, ...);
//IIC_24C64.h
extern void IIC_init(void);
extern void IIC_Read(U32 addr, U8 *data);
extern void IIC_Write(U32 addr, U8 data);
*/

extern void Lcd_TextAlignMid(void);
extern void Printf(char *fmt, ...);
extern void Lcd_Clear(void);


extern void Versions(void)
{
	Lcd_Clear();
    //Lcd_TextAlignMid();
    Printf("BootLoader Menu Versions %d.%d\n",version/10,version%10);
    Printf("Biuld time: %s %s\n\n",__DATE__ , __TIME__);
}