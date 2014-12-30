
#include "def.h"
#include "2440addr.h"
#include "2440lib.h"
#include "Keyboard.h"
/*************************************************************/
//全局变量
U16 KeyNum = 0;        //键值
extern U8  KeyIsUsed = 0;      //键值是否使用

extern U8  KEYCOUNT = 0;               //按键按下次数
/*************************************************************/
#ifndef DEBUG
#define DEBUG 1
#endif

void Beep(int x, int y);



U16 key2num[10] = {0x104, 0x18, 0x28, 0x48, 0x88, 0x108, 0x14, 0x24, 0x44, 0x84};

U16 KeyScan0(void)
{

   volatile U16 key, i;
    key = 0;

    rGPFUP  = rGPFUP | ((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3)) ; //禁止GPF 0-7 上拉
    rGPFUP  = rGPFUP | ((1 << 4) | (1 << 5) | (1 << 6) | (1 << 7)) ;
    rGPGUP  = rGPGUP | (1 << 0) ;                               //禁止GPG0 上拉


    rGPFCON = rGPFCON & (~((3 << 0) | (3 << 2) | (3 << 4) | (3 << 6)));     //GPF 0 1 2 3  为输入

    rGPFCON = (rGPFCON & (~((3 << 8) | (3 << 10) | (3 << 12) | (3 << 14)))
               | ((1 << 8) | (1 << 10) | (1 << 12) | (1 << 14)));           //GPF 4 5 6 7  为输出
    rGPGCON = (rGPGCON & (~(3 << 0)) | (1 << 0));                           //GPG0为输出

    rGPFDAT = rGPFDAT & (~((1 << 4) | (1 << 5) | (1 << 6) | (1 << 7))); //端口GPF 4-7输出0
    rGPGDAT = rGPGDAT & (~(1 << 0));                            //端口GPG0输出0



    if (~rGPFDAT & 0xF)
    {
        //  Uart_Printf("\n rGPFDAT %2x !\n",rGPFDAT);//为什么不加这句得不到正确值？？？
        //Delay(1000);//延迟必须要加 否则出错。
        for (i = 0; i < 50000; i++);
        for (i = 0; i < 50000; i++);
        key = ((~rGPFDAT) & 0x000F);

        rGPFCON = (rGPFCON & (~((3 << 0) | (3 << 2) | (3 << 4) | (3 << 6))) //GPF 0 1 2 3  为输出
                   | ((1 << 0) | (1 << 2) | (1 << 4) | (1 << 6)));

        rGPFCON = rGPFCON & (~((3 << 8) | (3 << 10) | (3 << 12) | (3 << 14))); //GPF 4 5 6 7  为输入
        rGPGCON = rGPGCON & (~(3 << 0));                        //GPG0为输入
        for (i = 0; i < 50000; i++); //延迟必须要加 否则出错。
        rGPFDAT = rGPFDAT & (~((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3))); //端口GPF 0-3输出0

        for (i = 0; i < 50000; i++); //延迟必须要加 否则出错。
        key = key | (~rGPFDAT & 0x00F0) | ((~rGPGDAT & 0x01) << 8);

    }

#ifdef DEBUG
    if (key != 0)
        Uart_Printf("\n key %x !\n", key);
#endif

    KeyNum = key;

    return key;
}


U16 KeyScan1(void)
{

   volatile U16 key, i;
    key = 0;

    // rGPFUP  = rGPFUP | ((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3)) ; //禁止GPF 0-7 上拉
    // rGPFUP  = rGPFUP | ((1 << 4) | (1 << 5) | (1 << 6) | (1 << 7)) ;
    rGPFUP  = 0;
    rGPHUP  = rGPHUP & ( ~(1 << 1)) ;                              //禁止GPH1 上拉


    rGPFCON = rGPFCON & (~((3 << 0) | (3 << 2) | (3 << 4) | (3 << 6)));     //GPF 0 1 2 3  为输入

    rGPFCON = (rGPFCON & (~((3 << 8) | (3 << 10) | (3 << 12) | (3 << 14)))
               | ((1 << 8) | (1 << 10) | (1 << 12) | (1 << 14)));           //GPF 4 5 6 7  为输出
    rGPHCON = (rGPHCON & (~(3 << 2)) | (1 << 2));                           //GPH1为输出

    rGPFDAT = rGPFDAT & (~((1 << 4) | (1 << 5) | (1 << 6) | (1 << 7))); //端口GPF 4-7输出0
    rGPHDAT = rGPHDAT & (~(1 << 1));                            //端口GPH1输出0



    if (~rGPFDAT & 0xF)
    {
        //  Uart_Printf("\n rGPFDAT %2x !\n",rGPFDAT);//为什么不加这句得不到正确值？？？
        //Delay(1000);//延迟必须要加 否则出错。
       // for (i = 0; i < 50000; i++);
        for (i = 0; i < 5000; i++);
        key = ((~rGPFDAT) & 0x000F);

        rGPFCON = (rGPFCON & (~((3 << 0) | (3 << 2) | (3 << 4) | (3 << 6))) //GPF 0 1 2 3  为输出
                   | ((1 << 0) | (1 << 2) | (1 << 4) | (1 << 6)));

        rGPFCON = rGPFCON & (~((3 << 8) | (3 << 10) | (3 << 12) | (3 << 14))); //GPF 4 5 6 7  为输入
        rGPHCON = rGPHCON & (~(3 << 2));                        //GPG0为输入
        for (i = 0; i < 50000; i++); //延迟必须要加 否则出错。
        rGPFDAT = rGPFDAT & (~((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3))); //端口GPF 0-3输出0

        for (i = 0; i < 50000; i++); //延迟必须要加 否则出错。
        key = key | (~rGPFDAT & 0x00F0) | (((~rGPHDAT & 0x02) >> 1) << 8);

    }

#ifdef DEBUG
    if (key != 0)
        Uart_Printf("\n key %x !\n", key);
#endif

    KeyNum = key;

    return key;
}



U16 KeyScan2(void)
{

   volatile U16 key, i;
    key = 0;

    // rGPFUP  = rGPFUP | ((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3)) ; //禁止GPF 0-7 上拉
    // rGPFUP  = rGPFUP | ((1 << 4) | (1 << 5) | (1 << 6) | (1 << 7)) ;
    rGPFUP  = 0;            //0启用内部上拉


    rGPFCON = rGPFCON & (~((3 << 0) | (3 << 2) | (3 << 4) | (3 << 6)));     //GPF 0 1 2 3  为输入

    rGPFCON = (rGPFCON & (~((3 << 8) | (3 << 10) | (3 << 12) | (3 << 14)))
               | ((1 << 8) | (1 << 10) | (1 << 12) | (1 << 14)));           //GPF 4 5 6 7  为输出

    rGPFDAT = rGPFDAT & (~((1 << 4) | (1 << 5) | (1 << 6) | (1 << 7))); //端口GPF 4-7输出0


    if (~rGPFDAT & 0xF)
    {
        //  Uart_Printf("\n rGPFDAT %2x !\n",rGPFDAT);//为什么不加这句得不到正确值？？？
        //Delay(1000);//延迟必须要加 否则出错。
        for (i = 0; i < 50000; i++);
        for (i = 0; i < 50000; i++);
        key = ((~rGPFDAT) & 0x0F);

        rGPFCON = (rGPFCON & (~((3 << 0) | (3 << 2) | (3 << 4) | (3 << 6))) //GPF 0 1 2 3  为输出
                   | ((1 << 0) | (1 << 2) | (1 << 4) | (1 << 6)));

        rGPFCON = rGPFCON & (~((3 << 8) | (3 << 10) | (3 << 12) | (3 << 14))); //GPF 4 5 6 7  为输入

        for (i = 0; i < 50000; i++); //延迟必须要加 否则出错。
        rGPFDAT = rGPFDAT & (~((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3))); //端口GPF 0-3输出0

        for (i = 0; i < 50000; i++); //延迟必须要加 否则出错。
        key = key | (~rGPFDAT & 0xF0);

    }

#ifdef DEBUG
    if (key != 0)
        Uart_Printf("\n key %x !\n", key);
#endif

    KeyNum = key;

    return key;
}
//keypress 事件激发 Click 事件
extern U16 KeyPress(){

}
//按键按下
extern U16 KeyDown(){


}
//按键抬起(release)
extern U16 KeyUp(){

}
extern U16 GetLastKey(){


}
//等待任意按键
extern U16 WaitKey()
{
    U16 Keys = 0;
    //等待一个按键按下
    while (!KeyScan());
    Beep_ON();
   // rGPBDAT = ((rGPBDAT & (~(1))) | ((~rGPBDAT) & (1)));
    if (KeyScan())
    {
        Keys = KeyNum;

    }
    while (KeyScan()); //Wait Key Realese
//    rGPBDAT |= 1;
	Beep_OFF();
    return Keys;
}

void Beep(int x, int y)
{
}

//   1 GPF4 2GPF5 4 GPF6 8 GPF7 1GPH1
//       |     |     |     |     |
//8 GPF3 | 1   |2    |3    |4    |5
//  -----|-----|-----|-----|-----|
//4 GPF2 | 6   |7    |8    |9    |0
//  -----|-----|-----|-----|-----|
//2 GPF1 |UP   |Down |Left |Right|OK
//  -----|-----|-----|-----|-----|
//1 GPF0 |H    |M    |L    | F
//  -----|-----|-----|-----|

