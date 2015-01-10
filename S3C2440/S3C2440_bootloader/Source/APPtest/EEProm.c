
#include "def.h"
#include "2440addr.h"
#include "LCD_Driver.h"
#include "Keyboard.h"
#include "stdlib.h"     //rand()
#include "Delay.h"

//u2440mon.h
extern U8 EEPROMISOK;       //EEPROM 错误标志
//2440lib.h
extern void Uart_Printf(char *fmt, ...);
//IIC_24C64.h
extern void IIC_init(void);
extern void IIC_Read(U32 addr, U8 *data);
extern void IIC_Write(U32 addr, U8 data);

//24C64 has 8*1024 Byte Convert HEX is 0x2000

extern void Versions(void);

// 数据验证
extern void EepromCheck(void)
{

    U8 i;
    //  static U8 data[0x2000];
    U8 databak[16];
    U8 datared[16];
    U8 datawrt[16];
    U32 buffer[16];

    Versions();
    //print(0, 2, "EEPROM RANDOM TEST !", 0xFF);
    //Lcd_TextAlignMid();
    Printf("EEPROM random Check !\n");
    //  Uart_Printf("IIC Test(Polling)???ˉ using AT24C64\n");

    IIC_init();
    for (i = 0; i < 16; ++i) {
        //
        srand(i);
        buffer[i] = (U32)rand() % 0x2000;
        //Backup
        IIC_Read( buffer[i], &(databak[i]));
        //Write Data
        datawrt[i] = rand() % 0xFF;
        IIC_Write( buffer[i], datawrt[i]);
        IIC_Read( buffer[i], &(datared[i]));
        Printf("%2d. Write[0x%4x] = 0x%x | Read[0x%4x] = 0x%x\n", i, buffer[i], datawrt[i], buffer[i], datared[i]);
        if (datared[i] == datawrt[i]) {
            EEPROMISOK = 1;
        }
        //
        IIC_Write( buffer[i], databak[i]);
    }

    if (EEPROMISOK) {

        Printf("\nEEPROM Check Test is OK !\n");
    } else {

        Printf("\nEEPROM Check Test is ERRO !\n");

    }
    //print(0, 20, "Press any key is continue !", 0xFF);

    Printf("\nPress any key is continue ! !\n");
    while(!(KEY_BACK==KeyScan()));

}

extern U8 EepromErase(void)
{
    U32 pwd=978646;
    U16 passwd[20]= {0};
    unsigned int i;
    U8 pwderro=0;
    Versions();

    Printf("EEPROM Erase !\n\n");
    Printf("Press Password To Continue:");
     Lcd_Update();
    i=0;
    //Get Password
    while (1) {


        //
        //KEYDOWN KeyNum!=0;
        if (KeyScan())

        {
            //这里加入这么多的KeyScan 主要是防止在案件按下和抬起的时候会产生误码
            if (i>19||KeyScan() == KEY_ENTER) {
                break;
            }
            if(KeyScan() == KEY_BACK)
            	return 0;
            //Printf("%x  ", passwd[i]);
            passwd[i] = KeyScan();
            //Printf("%d  ", i);
            //Printf("%x  ", KeyNum);
            //Printf("%x\n", passwd[i]);
            Printf("*");
            i++;
            while (KeyScan()); //Wait Key Realese
        }

    }

    //password checking
    for(i=0; i<6; i++) {

        if (key2num[pwd%10]!= passwd[i]) {
            //Printf("%x ",i);
            //Printf("%x ", passwd[i]);
            //Printf("%x\n", key2num[pwd%10]);

            pwderro =1;
        }

        pwd/=10;
    }



    if(pwderro) {
        Printf("\nPassword erro!\n");
        Printf("Press any key is Retun !\n");
        while (!KeyScan());
        return 1;
    } else {

        IIC_init();

        Printf("\nPassword ok!\n");
        Printf("\nNow is Erasing EEPROM");
        Printf("\nPlease Wait a minite ......\n\n");
        //  数据清除
            /*
        //清理工作时间 设置为3小时
            IIC_Write(0x2b0,0x10);//H
            IIC_Write(0x2b1,0x00);//H
            IIC_Write(0x2b2,0x00);//H
            IIC_Write(0x2b3,0x10);//H
            IIC_Write(0x2b4,0x00);//M
            IIC_Write(0x2b5,0x00);//M
            IIC_Write(0x2b6,0x00);//M
            IIC_Write(0x2b7,0x10);//M
            IIC_Write(0x2b8,0x00);//L
            IIC_Write(0x2b9,0x00);//L
            IIC_Write(0x2ba,0x00);//L
            IIC_Write(0x2bb,0x00);//L
            IIC_Write(0x2bC,0x00);//F
            IIC_Write(0x2bD,0x00);//F
            IIC_Write(0x2bE,0x00);//F
            IIC_Write(0x2bF,0x20);//F

    */
        for (i = 0x10; i < 0x2000; i++)
            IIC_Write(i, 0x00);

        Printf("Eeprom Erase Finish!\n");
        Printf("Press any key is Retun !");
       // while (!KeyScan());
      while(!(KEY_BACK==KeyScan()));
        return 0;
    }

}
extern U8 EepromClearAll(void)
{
    U32 i;
    
    Versions();

    Printf("EEPROM Clear All!\n\n");
    Printf("Press Password To Continue:");
   
        IIC_init();

        Printf("\nNow is Erasing EEPROM");
        Printf("\nPlease Wait a minite ......\n\n");
        Lcd_Update();
        //  数据清除
           /*
        //清理工作时间 设置为3小时
            IIC_Write(0x2b0,0x10);//H
            IIC_Write(0x2b1,0x00);//H
            IIC_Write(0x2b2,0x00);//H
            IIC_Write(0x2b3,0x10);//H
            IIC_Write(0x2b4,0x00);//M
            IIC_Write(0x2b5,0x00);//M
            IIC_Write(0x2b6,0x00);//M
            IIC_Write(0x2b7,0x10);//M
            IIC_Write(0x2b8,0x00);//L
            IIC_Write(0x2b9,0x00);//L
            IIC_Write(0x2ba,0x00);//L
            IIC_Write(0x2bb,0x00);//L
            IIC_Write(0x2bC,0x00);//F
            IIC_Write(0x2bD,0x00);//F
            IIC_Write(0x2bE,0x00);//F
            IIC_Write(0x2bF,0x20);//F

            */
        for (i = 0; i < 0x2000; i++)
            IIC_Write(i, 0x00);

        Printf("Eeprom Erase Finish!\n");
        DelayMS(1000);
      //  Printf("Press any key is Retun !");
      //  while (!KeyScan());
        return 0;
   
}

