#include <string.h>
#include <stdio.h>

#include "def.h"
#include "2440addr.h"
#include "2440lib.h"
#include "2440slib.h"
//#include "Nand.h"
#include "nand_lowlevel.h"
//suppport boot params
//#define   GLOBAL_PARAMS
#include "bootpara.h"
#include "2440usb.h"

//#include "Timer.h"
extern void Timer_InitEx(void);
extern void Timer_StartEx(void);
extern U32 Timer_StopEx(void);
//#include "nand.h"
extern void AutoNandLoadRun(void);

//#include "usbout.h"
void ClearEp3OutPktReady(void);
//#include "usblib.h"
void ConfigEp3DmaMode(U32 bufAddr,U32 count);

extern void Menu(void);
/*************************************************************/
//与下载有关的全局变量
/*************************************************************/
extern volatile U8  *downPt;
extern volatile U32 downloadAddress;
extern U32 tempDownloadAddress;
extern volatile U32 downloadFileSize;
extern volatile U32 totalDmaCount;
extern volatile U16 checkSum;

extern volatile int isUsbdSetConfiguration;

extern U8 download_run; // 是否下载到NAND中 1:下载 0:运行
extern U8 menuUsed;	//  使用菜单模式 ，当进入下载菜单后不会跳出来
extern U8 consoleNum;	//  串口输出选择
extern U8 autorun_ds;	//  自动运行使能
extern U8 autorun_trig;
/*************************************************************/

extern void USBDownload(void)
{
    U32 i;
    U32 j;
    U16 cs;
    U32 temp;
    U16 dnCS;
    int first = 1;
    U32 time;
    U8 tempMem[16];
    U8 key;

    checkSum = 0;
    downloadAddress = (U32)tempMem; //_RAM_STARTADDRESS;
    downPt = (unsigned char *)downloadAddress;
    //This address is used for receiving first 8 byte.
    downloadFileSize = 0;

#if 0
    MMU_DisableICache();
    //For multi-ICE.
    //If ICache is not turned-off, debugging is started with ICache-on.
#endif

    /*******************************/
    /*  Test program download   */
    /*******************************/
    j = 0;

    if (isUsbdSetConfiguration == 0) {
        //Uart_Printf("  USB host is not connected yet.\n");
    }

    while (downloadFileSize == 0) {
        if (first == 1 && isUsbdSetConfiguration != 0) {
            //Uart_Printf("  USB host is connected. Waiting a download.\n");
            first = 0;
        }

        if (j % 0x100000 == 0)Led_Display(0x8);
        if (j % 0x100000 == 0x80000)Led_Display(0x0);
        j++;

        key = Uart_GetKey();
        if (autorun_trig)
            AutoNandLoadRun();  //run it in svc mode
        if (key != 0) {
            autorun_ds = 1;
            //printf("disable autorun\n");
            Menu();
            first = 1; //To display the message,"USB host ...."
        }
    }

    autorun_ds = 1;
    //printf("disable autorun\n");

    Timer_InitEx();
    Timer_StartEx();

#if USBDMA

    rINTMSK &= ~(BIT_DMA2);

    ClearEp3OutPktReady();
    // indicate the first packit is processed.
    // has been delayed for DMA2 cofiguration.

    if (downloadFileSize > EP3_PKT_SIZE) {
        if (downloadFileSize <= (0x80000)) {
            ConfigEp3DmaMode(downloadAddress + EP3_PKT_SIZE - 8, downloadFileSize - EP3_PKT_SIZE);

            //will not be used.
            /*     rDIDST2=(downloadAddress+downloadFileSize-EP3_PKT_SIZE);
                       rDIDSTC2=(0<<1)|(0<<0);
                    rDCON2=rDCON2&~(0xfffff)|(0);
            */
        } else {
            ConfigEp3DmaMode(downloadAddress + EP3_PKT_SIZE - 8, 0x80000 - EP3_PKT_SIZE);
            //2440比2410的DIDSTCx寄存器多了中断产生条件的控制位,USB的DMA传输为字节计数
            //防止高频开cache运行时下载大于0x80000字节文件时IsrDma2出错!!!
            //while((rDSTAT2&0xfffff)==(0x80000-EP3_PKT_SIZE));
            while (!(rDSTAT2 & (1 << 20))); //防止DMA传输尚未开始就写入下一次重装值!!!
            if (downloadFileSize > (0x80000 * 2)) { //for 1st autoreload
                rDIDST2 = (downloadAddress + 0x80000 - 8); //for 1st autoreload.
                rDIDSTC2 = (1 << 2) | (0 << 1) | (0 << 0);
                rDCON2 = rDCON2&~(0xfffff) | (0x80000);

                while (rEP3_DMA_TTC < 0xfffff) {
                    rEP3_DMA_TTC_L = 0xff;
                    rEP3_DMA_TTC_M = 0xff;
                    rEP3_DMA_TTC_H = 0xf;
                }
            } else {
                rDIDST2 = (downloadAddress + 0x80000 - 8); //for 1st autoreload.
                rDIDSTC2 = (1 << 2) | (0 << 1) | (0 << 0);
                rDCON2 = rDCON2&~(0xfffff) | (downloadFileSize - 0x80000);

                while (rEP3_DMA_TTC < 0xfffff) {
                    rEP3_DMA_TTC_L = 0xff;
                    rEP3_DMA_TTC_M = 0xff;
                    rEP3_DMA_TTC_H = 0xf;
                }
            }
        }
        totalDmaCount = 0;
    } else {
        totalDmaCount = downloadFileSize;
    }
#endif

    Uart_Printf("\nNow, Downloading [ADDRESS:%xh,TOTAL:%d]\n",
                downloadAddress, downloadFileSize);
    Uart_Printf("RECEIVED FILE SIZE:%8d", 0);

#if USBDMA
    j = 0x10000;

    while (1) {
        if ( (rDCDST2 - (U32)downloadAddress + 8) >= j) {
            Uart_Printf("\b\b\b\b\b\b\b\b%8d", j);
            j += 0x10000;
        }
        if (totalDmaCount >= downloadFileSize)break;
    }

#else
    j = 0x10000;

    while (((U32)downPt - downloadAddress) < (downloadFileSize - 8)) {
        if ( ((U32)downPt - downloadAddress) >= j) {
            Uart_Printf("\b\b\b\b\b\b\b\b%8d", j);
            j += 0x10000;
        }
    }
#endif

    time = Timer_StopEx();

    Uart_Printf("\b\b\b\b\b\b\b\b%8d", downloadFileSize);
    Uart_Printf("\n(%dKB/S,%dMS)\n", (downloadFileSize / time / 1000), time/1000);

#if USBDMA
    /*******************************/
    /*   Verify check sum       */
    /*******************************/

    Uart_Printf("Now, Checksum calculation\n");

    cs = 0;
    i = (downloadAddress);
    j = (downloadAddress + downloadFileSize - 10) & 0xfffffffc;
    while (i < j) {
        temp = *((U32 *)i);
        i += 4;
        cs += (U16)(temp & 0xff);
        cs += (U16)((temp & 0xff00) >> 8);
        cs += (U16)((temp & 0xff0000) >> 16);
        cs += (U16)((temp & 0xff000000) >> 24);
    }

    i = (downloadAddress + downloadFileSize - 10) & 0xfffffffc;
    j = (downloadAddress + downloadFileSize - 10);
    while (i < j) {
        cs += *((U8 *)i++);
    }

    checkSum = cs;
#else
    //checkSum was calculated including dnCS. So, dnCS should be subtracted.
    checkSum = checkSum - *((unsigned char *)(downloadAddress + downloadFileSize - 8 - 2))
               - *( (unsigned char *)(downloadAddress + downloadFileSize - 8 - 1) );
#endif

    dnCS = *((unsigned char *)(downloadAddress + downloadFileSize - 8 - 2)) +
           (*( (unsigned char *)(downloadAddress + downloadFileSize - 8 - 1) ) << 8);

    if (checkSum != dnCS) {
        Uart_Printf("Checksum Error!!! MEM:%x DN:%x\n", checkSum, dnCS);
        return;
    }

    Uart_Printf("Download O.K.\n\n");
    Uart_TxEmpty(consoleNum);

    if (download_run == 1) {
        register void(*run)(void);  //使用寄存器变量以防止禁止DCACHE后数据不一致!!!
        rINTMSK = BIT_ALLMSK;
        run = (void ( *)(void))downloadAddress; //使用DCACHE且RW区也在CACHE区间downloadAddress会在cache中
        {
            MMU_DisableDCache();    //download program must be in
            MMU_DisableICache();    //non-cache area
            MMU_InvalidateDCache(); //使所有DCACHE失效,本程序的MMU_Init中将会刷新DCACHE到存储器,
            //为使应用此MMU_Init方式的程序能被正确运行必须先使DCACHE失效!!!
            MMU_DisableMMU();
            //call_linux(0, 193, downloadAddress);  //或不用上面3个函数而直接使用call_linux
        }
        run();
    }
}