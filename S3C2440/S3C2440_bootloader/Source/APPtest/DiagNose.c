/****************************************************************
 NAME: DiagNose.c


 ****************************************************************/
#include "def.h"
#include "2440addr.h"
#include "2440lib.h"
#include "2440slib.h"
#include "mmu.h"

#include "LCD_Driver.h"
#include "stdio.h"
#include "Delay.h"
#include "Keyboard.h"

#include "bootpara.h"
//#include "ver.h"
extern void Versions(void);

//#include "lcdDriver.h" 
//extern void Lcd_Init(void);
//#include "buzzer.h"
extern void buzTest(void);
//#include "rtc.h"
extern void Settime(void);
//#include "KeyLight.h"
extern void KeyLight(void);
//#include "canTest.h"
extern void CanTest(void);
//#include "EEPROM.h"
extern void EepromCheck(void);

extern void IIC_init(void);
extern void IIC_Read(U32 addr,U8 *data);
extern void IIC_Write(U32 addr, U8 data);

extern void memtest(void);
extern void Keytest(void);
extern void Eepromtest(void);
extern void lcdtest(void);
extern void Get_Rtc(void);
extern void Get_Rtc(void);
extern void Lcd_Init(void);
extern void (*reboot)(void);
void DiagNose(void)
{
 //   Lcd_Test();
 //   while(1);
    Lcd_Init();
	Versions();
    Printf("DiagNose MODE\n");
    

   // Uart_Printf("1.RTC Test  \n");
  //  Settime();

    Uart_Printf("2. Key  Test  \n");

    Keytest();

    Uart_Printf("3. LCD BackLight Test  \n");
    //lcdLightTest();

    lcdtest();

    Uart_Printf("4. Key Light Test  \n");


    Uart_Printf("5. LED Test  \n");

    Uart_Printf("6. Buzzer Test  \n");
    buzTest();

    Uart_Printf("7. Key Light Test  \n");
    KeyLight();

    Uart_Printf("9. RS485 Test  \n");

    Uart_Printf("10.CAN Test  \n");

    CanTest();

    Uart_Printf("1. MemoryTest  \n");

   // memtest();

    Uart_Printf("8. EEPROM CFG  \n");

    EepromClearAll();

    Lcd_ClearScr(0x0);   //黑色
   // print(20,20,"Test Finish !!!",0xFF0000);
    
    boot_params.diagnose.val=0x5A;
    
    save_params();
	search_params();
	if(boot_params.diagnose.val == 0x5A)
		//Printf()
    	print(20,25,"DiagNose is Finish !!!",0xFF);
   // print(20,26,"It will Exit in Five Minites !!!",0xFF);
    Delay(50000);
    //
     Lcd_Update();
    reboot();

}