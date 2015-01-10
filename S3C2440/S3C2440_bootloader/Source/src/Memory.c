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

//***********TimerWDG*************
//#include "Timer.h"
extern void Timer_InitEx(void);
extern void Timer_StartEx(void);
extern U32 Timer_StopEx(void);
//********************************
//定义跳过测试的内存地址
#define JMPDATA 0x01000000

void MemoryTest(void)
{
    U32 data;
    U8 memError = 0;
    U32 *pt;
    U32 time = 0;
  DisableIrq(BIT_TIMER0);
    //Uart_Printf("Memory Test(%xh-%xh):WR",_RAM_STARTADDRESS,(_ISR_STARTADDRESS&0xfff0000));
    //test sdram from _RAM_STARTADDRESS+2M
    Uart_Printf("Memory Test(%xh-%xh):\n", _RAM_STARTADDRESS + 0x00200000, (_ISR_STARTADDRESS & 0xffff0000));
    Printf("Memory Test(%xh-%xh):\n", _RAM_STARTADDRESS + 0x00200000, (_ISR_STARTADDRESS & 0xffff0000));

    Timer_InitEx();
    Timer_StartEx();
  
    pt = (U32 *)(_RAM_STARTADDRESS + JMPDATA);
    while ((U32)pt < (_ISR_STARTADDRESS & 0xffff0000))
    {
        *pt = (U32)pt;
        pt++;
    }
    time = Timer_StopEx();
    //Uart_Printf(" time %d s",time/1000);
     Uart_Printf("Write speed is %d MB/S,%d ms\n\n", (((_ISR_STARTADDRESS & 0xffff0000) - (_RAM_STARTADDRESS + JMPDATA)) / time), time / 1000);
    Printf("Write speed is %d MB/S,%d ms\n\n", (((_ISR_STARTADDRESS & 0xffff0000) - (_RAM_STARTADDRESS + JMPDATA)) / time), time / 1000);


    //Uart_Printf("\b\bRD");

    pt = (U32 *)(_RAM_STARTADDRESS + JMPDATA);

    Timer_InitEx();
    Timer_StartEx();

    while ((U32)pt < (_ISR_STARTADDRESS & 0xffff0000))
    {
        data = *pt;
        if (data != (U32)pt)
        {
            memError = 1;
            Uart_Printf("\b\bFAIL:0x%x=0x%x\n", pt, data);
            break;
        }
        pt++;
    }
    time = Timer_StopEx();
    //Uart_Printf(" SPEED is %5.1fMB/S,%3.1fS\n\n",(float)(((_ISR_STARTADDRESS&0xffff0000)-(_RAM_STARTADDRESS+0x00200000))/time/1000/1000.),time);
    Uart_Printf("Read speed is %d MB/S,%d ms\n\n", (((_ISR_STARTADDRESS & 0xffff0000) - (_RAM_STARTADDRESS + JMPDATA)) / time), time / 1000);
    Printf("Read speed is %d MB/S,%d ms\n\n", (((_ISR_STARTADDRESS & 0xffff0000) - (_RAM_STARTADDRESS + JMPDATA)) / time), time / 1000);



    if (memError == 0){
      Uart_Printf("\b\bMemory Test O.K.\n");
      Printf("\b\bMemory Test O.K.\n");
    }
  EnableIrq(BIT_TIMER0);
}

extern void ClearMemory(void)
{


    U8 memError = 0;
    U32 *pt;

    //
    // memory clear
    //
    Uart_Printf("Clear Memory (%xh-%xh):WR", _RAM_STARTADDRESS, HEAPEND);

    pt = (U32 *)_RAM_STARTADDRESS;
    while ((U32)pt < HEAPEND)
    {
        *pt = (U32)0x0;
        pt++;
    }

    if (memError == 0)Uart_Printf("\b\bO.K.\n");
}

