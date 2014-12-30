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
//�������йص�ȫ�ֱ���
/*************************************************************/
extern volatile U8  *downPt;
extern volatile U32 downloadAddress;
extern U32 tempDownloadAddress;
extern volatile U32 downloadFileSize;
extern volatile U32 totalDmaCount;
extern volatile U16 checkSum;

extern volatile int isUsbdSetConfiguration;

extern U8 download_run; // �Ƿ����ص�NAND�� 1:���� 0:����
extern U8 menuUsed;	//  ʹ�ò˵�ģʽ �����������ز˵��󲻻�������
extern U8 consoleNum;	//  �������ѡ��
extern U8 autorun_ds;	//  �Զ�����ʹ��
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
            //2440��2410��DIDSTCx�Ĵ��������жϲ��������Ŀ���λ,USB��DMA����Ϊ�ֽڼ���
            //��ֹ��Ƶ��cache����ʱ���ش���0x80000�ֽ��ļ�ʱIsrDma2����!!!
            //while((rDSTAT2&0xfffff)==(0x80000-EP3_PKT_SIZE));
            while (!(rDSTAT2 & (1 << 20))); //��ֹDMA������δ��ʼ��д����һ����װֵ!!!
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
        register void(*run)(void);  //ʹ�üĴ��������Է�ֹ��ֹDCACHE�����ݲ�һ��!!!
        rINTMSK = BIT_ALLMSK;
        run = (void ( *)(void))downloadAddress; //ʹ��DCACHE��RW��Ҳ��CACHE����downloadAddress����cache��
        {
            MMU_DisableDCache();    //download program must be in
            MMU_DisableICache();    //non-cache area
            MMU_InvalidateDCache(); //ʹ����DCACHEʧЧ,�������MMU_Init�н���ˢ��DCACHE���洢��,
            //ΪʹӦ�ô�MMU_Init��ʽ�ĳ����ܱ���ȷ���б�����ʹDCACHEʧЧ!!!
            MMU_DisableMMU();
            //call_linux(0, 193, downloadAddress);  //��������3��������ֱ��ʹ��call_linux
        }
        run();
    }
}