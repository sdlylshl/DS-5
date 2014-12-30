#include "def.h"
#include "2440lib.h"
#include "LCD_Driver.h"


#include "stdio.h"

#include "Keyboard.h"
extern U16 WaitKey(void);

#include "LCD_Driver.h"
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
//#include "lcdtest.h"
extern void lcdtest(void);
//#include "canTest.h"
extern void CanTest(void);
//#include "EEPROM.h"
extern void EepromCheck(void);
extern U8 EepromErase(void);
//#include "DiagNose.h"
extern void DiagNose(void);

//#include "memtest.h"
extern void memtest(void);

#include "mmu.h"
extern void MMU_Disable(void);

//#include "keytest.h"
extern void Keytest(void);

//#include "u2440mon.h"
extern void USBDownload(void);
//下载方式设定
extern void DownLoad_COM(void);       //下载方式          0.USB 1.串口下载  ;其他 USB
extern void DownLoad_USB(void);
extern U8 download_run;   // 是否下载到NAND中 0:run 1:DownLoad

//#include "nand_lowlevel.h"
extern void NandWrite(void);
//extern void WrFileToNF_COM(void);
//#include "nand.h"
extern void NandLoadRun_App(void);

//******************************************************************************
void *fun(void); //

//******************************************************************************
#define MENUNUM 16

char name[MENUNUM + 2][30] =
{
    "[MENU]\n",
    " 1.USB DownLoad\n",
    " 2.COM DownLoad\n",
    " 3.Time Sync\n",
    " 4.Clear Data\n",
    " 5.LCD Dead Pixels\n",
    " 6.<LCD Bright Adjust>\n",
    " 7.Key Test\n",
    " 8.Key Light Test\n",
    " 9.Buzzer Test\n",
    "10.EEPROM Test\n",
    "11.MEM Test\n",
    "12.CAN Test\n",
    "13.<485 Test>\n",
    "14.<Params Config>\n",
    "15.ReBoot\n",
    "16.DiagNose\n",
    "[END]\n",
};

void (*reboot)(void) = (void ( *)(void))0x30000000;
extern void LcdMenu2(void)
{
    U8 i;
    U16 keys;
    U8 pos = 0;
    Lcd_Init();	
    Versions();
    //Printf("[MENU LIST]\n");
    Lcd_SetPosition(0, 4);
    for (i = 0; i < MENUNUM + 2 ; ++i)
    {
        Printf("%s", name[i]);

    }
    while (1)
    {
        Lcd_Update();  
		 //Delay(30000);
        keys = WaitKey();

        if (keys == KEY_UP)
        {
            if (pos > 1)
            {
                pos--;
            }
            else
            {
                pos = MENUNUM;
            }

        }
        if (keys == KEY_DOWN)
        {
            if (pos < MENUNUM)
            {
                pos++;
            }
            else
            {
                pos = 1;
            }

        }
        
       // Versions();
        Lcd_SetPosition(0, 3);
        Printf(" %d\n", pos);
        for (i = 0; i < MENUNUM + 2 ; i++)
        {
            //Lcd_SetPosition(0, 4);
            if ((i>0)&&(i == pos))
            {
                
                Lcd_SetColor(RED);
                Printf("%s", name[i]);
                Lcd_SetColor(BLUE);
            }
            else
            {
                Printf("%s", name[i]);
            }
        }
      	Lcd_Update();  

		
		if(keys == KEY_ENTER)	
    	  switch (pos)
            {
               
            case 0:
            	 
           // break;
            case 1:
            Versions();
            Printf("DownLoad By USB");
             DownLoad_USB();
                download_run = 0;
                NandWrite();
            break;
               
            case 2:
            	Versions();
            	Printf("DownLoad By COM");
            	DownLoad_COM();
                download_run = 0;
                NandWrite(); 
            break;
               
            case 3:
            	 Settime();
            	
            	 Versions();
            break;
               
            case 4:
            	 EepromErase();
            	 Versions();
            	
            break;
               
            case 5:
            	  lcdtest();
            	 
            	  Versions();
            break;
            case 6:
            	 
            break;
            case 7:
            	 Keytest();
            	
            	 Versions();
            break;
            case 8:
            	 KeyLight();
            	
            	 Versions();
            break;
            case 9:
            	  buzTest();
            	
            	  Versions();
            break;
            case 10:
            	 EepromCheck();
            	
            	 Versions();
            break;
            case 11:
            	  memtest();
            	 
            	  Versions();
            break;
            case 12:
            	 CanTest();
            	
            	 Versions();
            break;
            case 13:
            	 
            break;
            case 14:
            	 
            break;
            case 15:
            MMU_Disable();
            	 reboot();
            	 Versions();
            break;
            case 16:
            	 DiagNose();
            	 
            	 Versions();
            break;
           
            }
    //     	Lcd_Update();  
    }

}


void PrtLCD()
{
    // Lcd_ClearScr(0x0);   //
    Versions();
    print(1, 3, "Press KEY_UP and KEY_DOWN to select!", 0xFF);
    print(1, 4, "Press Enter key to Enter the menu ", 0xFF);
    // print(0, 4, "Press KEY_BACK go back ", 0xFF);

    print(2, 6, "[Menu]", 0xFF);
    print(4, 7, "1.DownLoad ", 0xFF);
    print(4, 8, "2.Key Test", 0xFF);
    print(4, 9, "3.RTC Test", 0xFF);
    print(4, 10, "4.LCD Test", 0xFF);
    print(4, 11, "5.Buz Test", 0xFF);
    print(4, 12, "6.KeyLight Test", 0xFF);
    print(4, 13, "7.CAN Test", 0xFF);
    print(4, 14, "8.MEM Test", 0xFF);
    print(4, 15, "9.EEPROM CFG", 0xFF);
    print(4, 16, "10.COM DownLoad", 0xFF);
    print(4, 17, "11.USB DownLoad", 0xFF);
    print(4, 18, "12.BootParams", 0xFF);
    print(4, 19, "13.APP Test", 0xFF);
    //print(4,19,"13.Restart",0xFF);
    //print(4,11,"5.Buz Test",0xFF);
    print(2, 20, "[END]", 0xFF);




}
void lcdMenu(void)
{
    U8 num = 0;
    U16 key = 0;
    char flg = 1;
    // Lcd_Init();
    PrtLCD();
    //  Eepromtest2();
    do
    {
        //Delay(2000);
        key = WaitKey();

        if (KEY_UP == key)
        {
            if (num == 1)
            {
                num = 13;
            }
            else
            {
                num--;
            }
        }
        if (KEY_DOWN == key)
        {
            if (num > 12)
            {
                num = 1;
            }
            else
            {
                num++;
            }
        }
//        if (KEY_L == key)
//            num = 12;

        if ( KEY_ENTER == key)
        {

            if (13 == num)
            {
                DiagNose();
                //              boot_params.TestMode.val=0x5A;
                //              save_params();
                // while (!KeyScan());

                //MMU_Disable();
                // restart();
                // NandLoadRun_App();
                //MMU_Init();
                //PrtLCD();
            }
            if (12 == num)
            {

                MMU_Disable();
                MMU_Init();

                NandLoadRun_App();
                //MMU_Init();
            }
            if (11 == num)
            {

                //MMU_Disable();
                Lcd_ClearScr(0x0);   //
                print(4, 6, "Now is USB DownLoad", 0xFF);
                print(4, 17, "Press ESC to exit", 0xFF);
                print(4, 18, "Press SPACE download to APP", 0xFF);

                DownLoad_USB();
                download_run = 0;
                NandWrite();

                PrtLCD();
            }
            if (10 == num)
            {
                download_run = 0;
                Lcd_ClearScr(0x0);   //
                print(4, 6, "Now is COM DownLoad", 0xFF);
                print(4, 17, "Press ESC to exit", 0xFF);
                print(4, 18, "Press SPACE download to APP", 0xFF);
                //MMU_Disable();

                DownLoad_COM();
                download_run = 0;
                NandWrite();
                PrtLCD();
            }
            if (9 == num)
            {
                while (KeyScan()); //Wait Key Realese
                EepromErase();
                PrtLCD();
            }
            if (8 == num)
            {
                memtest();
                PrtLCD();
            }
            if (7 == num)
            {
                CanTest();
                PrtLCD();
            }
            if (6 == num)
            {
                KeyLight();
                PrtLCD();
            }
            if (5 == num)
            {
                buzTest();
                PrtLCD();
            }
            if (4 == num)
            {
                lcdtest();
                PrtLCD();
            }

            if (3 == num)
            {
                Settime();
                PrtLCD();
            }
            if (2 == num)
            {
                Keytest();
                PrtLCD();
            }
            if (1 == num)
            {
                download_run = 0;

                Lcd_ClearScr(0x0);   //

                if (boot_params.comdown.val == 0)
                {

                    print(4, 6, "Now is COM DownLoad", 0xFF);
                    print(4, 17, "Press ESC to exit", 0xFF);
                    print(4, 18, "Press SPACE download to APP", 0xFF);
                    //MMU_Disable();
                    DownLoad_COM();
                    download_run = 1;
                    NandWrite();

                }
                if (boot_params.comdown.val == 1)
                {
                    print(4, 6, "Now is USB DownLoad", 0xFF);
                    print(4, 18, "Press USB-->Transmit download to APP", 0xFF);
                    printf("press [USB Port-->transmit] to choose the file \n");
                    //MMU_Disable();
                    DownLoad_USB();
                    download_run = 1;
                    NandWrite();

                }
                PrtLCD();
            }
            if (num == 0)num++;

        }
        if (flg)
            switch (num)
            {
            case 0:

                break;
            case 1:
                //print(4,19,"13.Restart",0xFF);
                print(4, 19, "13.APP Test", 0xFF);
                print(4, 7, "1.DownLoad", 0xFF0000);
                print(4, 8, "2.Key Test", 0xFF);
                break;
            case 2:
                print(4, 7, "1.DownLoad", 0xFF);
                print(4, 8, "2.Key Test", 0xFF0000);
                print(4, 9, "3.RTC Test", 0xFF);
                break;
            case 3:
                print(4, 8, "2.Key Test", 0xFF);
                print(4, 9, "3.RTC Test", 0xFF0000);
                print(4, 10, "4.LCD Test", 0xFF);
                break;
            case 4:
                print(4, 9, "3.RTC Test", 0xFF);
                print(4, 10, "4.LCD Test", 0xFF0000);
                print(4, 11, "5.Buz Test", 0xFF);
                break;
            case 5:
                print(4, 10, "4.LCD Test", 0xFF);
                print(4, 11, "5.Buz Test", 0xFF0000);
                print(4, 12, "6.KeyLight Test", 0xFF);
                break;
            case 6:
                print(4, 11, "5.Buz Test", 0xFF);
                print(4, 12, "6.KeyLight Test", 0xFF0000);
                print(4, 13, "7.CAN Test", 0xFF);
                break;
            case 7:
                print(4, 12, "6.KeyLight Test", 0xFF);
                print(4, 13, "7.CAN Test", 0xFF0000);
                print(4, 14, "8.MEM Test", 0xFF);
                break;
            case 8:
                print(4, 13, "7.CAN Test", 0xFF);
                print(4, 14, "8.MEM Test", 0xFF0000);
                print(4, 15, "9.EEPROM CFG", 0xFF);
                break;
            case 9:
                print(4, 14, "8.MEM Test", 0xFF);
                print(4, 15, "9.EEPROM CFG", 0xFF0000);
                print(4, 16, "10.COM DownLoad", 0xFF);
                break;
            case 10:
                print(4, 15, "9.EEPROM CFG", 0xFF);
                print(4, 16, "10.COM DownLoad", 0xFF0000);
                print(4, 17, "11.USB DownLoad", 0xFF);
                break;
            case 11:
                print(4, 16, "10.COM DownLoad", 0xFF);
                print(4, 17, "11.USB DownLoad", 0xFF0000);
                print(4, 18, "12.BootParams", 0xFF);
                break;
            case 12:
                print(4, 17, "11.USB DownLoad", 0xFF);
                print(4, 18, "12.BootParams", 0xFF0000);
                print(4, 19, "13.APP Test", 0xFF);
                break;

            case 13:
                print(4, 18, "12.BootParams", 0xFF);
                print(4, 19, "13.APP Test", 0xFF0000);
                print(4, 7, "1.DownLoad", 0xFF);
                break;

            }
        //   Uart_Printf("\n num %x !\n",num);
        //  Uart_Printf("\n key %x !\n",key);
    }
    while (flg);

}