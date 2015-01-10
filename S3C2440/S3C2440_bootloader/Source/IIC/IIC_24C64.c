#include "def.h"
#include "2440addr.h"
//外部引用函数
//extern void Delay(int x) ;
extern void Uart_Printf(char *fmt, ...);


//采用硬件方式 实现IIC

//**********************************************************************/
//全局变量  
U8 addr16=1; //对于24C32以上的EEPROM 地址为16字节
U8 DevAddr=0xa0; //Device Address [1 0 1 0 A2 A1 A0]

extern void IIC_init(void);
extern void IIC_Read(U32 addr, U8 *data);
extern void IIC_Write(U32 addr, U8 data);
//**********************************************************************/

#define WRDATA      (1)
#define POLLACK     (2)
#define RDDATA      (3)
#define SETRDADDR   (4)

//#define PCLK 50000000
#define IICBUFSIZE 0x20
//**********************************************************************/
//本地静态变量
static U8 iicData[IICBUFSIZE];
static volatile int iicDataCount;
static volatile int iicStatus;
static volatile int iicMode;
static int iicPt;

static void Run_IicPoll(void);
//**********************************************************************/

extern void IIC_init(void)
{
    //设置GPE15->IICSDA 和 GPE14->IICSCL
    rGPEUP  |= (1 << 14) | (1 << 15);   //Pull-up disable GPE14 GPE15为开漏输出没有上拉选项
    rGPECON &= ~0xf0000000;
    rGPECON |= 0xa0000000;              //GPE15:IICSDA , GPE14:IICSCL

    //Enable ACK, Prescaler IICCLK=PCLK/16, Enable interrupt, Transmit clock value Tx clock=IICCLK/16
    rIICCON    = (1 << 7) | (0 << 6) | (1 << 5) | (0xf); //0xaf
    rIICADD  = 0x10;                    //2440 slave address = [7:1]
    rIICSTAT = 0x10;                    //IIC bus data output enable(Rx/Tx)
    Uart_Printf("IIC Init Finshed\n");
}

/*******************************Rd24C02*****************************************/
void IIC_Read(U32 addr, U8 *data)
{
    iicMode      = SETRDADDR;
    iicPt        = 0;

    if (addr16)
    {
        iicData[0]   = (U8)((addr >> 8) & 0XFF);
        iicData[1]   = (U8)(addr);
        iicDataCount = 2;
    }
    else
    {
        iicData[0]   = (U8)addr;
        iicDataCount = 1;
    }

    rIICDS   = DevAddr + 1;                 //RD
    rIICSTAT = 0xf0;                    //MasTx,Start

    //Clearing the pending bit isn't needed because the pending bit has been cleared.
    while (iicDataCount != -1)
        Run_IicPoll();

    iicMode      = RDDATA;
    iicPt        = 0;
    iicDataCount = 1;

    rIICDS   = DevAddr + 1;                  //WR 1010 0001
    rIICSTAT = 0xb0;                    //Master Rx,Start
    rIICCON  = 0xe0;//0xaf;              //Resumes IIC operation.
    while (iicDataCount != -1)
        Run_IicPoll();

    *data = iicData[1];
}

/*******************************Wr24C02*****************************************/
void IIC_Write(U32 addr, U8 data)
{
    unsigned int i;
    iicMode      = WRDATA;
    iicPt        = 0;

    if (addr16)
    {
        iicData[0]   = (U8)((addr >> 8) & 0XFF);
        iicData[1]   = (U8)(addr);
        iicData[2]   = data;
        iicDataCount = 3;
    }
    else
    {
        iicData[0]   = (U8)addr;
        //  Uart_Printf("%d\n",iicData[0]);
        iicData[1]   = data;
        iicDataCount = 2;
    }

    //8-bit data shift register for IIC-bus Tx/Rx operation.
    rIICDS        = DevAddr;           //RD 0xa0 1010 0000

    //Master Tx mode, Start(Write), IIC-bus data output enable
    //Bus arbitration sucessful, Address as slave status flag Cleared,
    //Address zero status flag cleared, Last received bit is 0
    rIICSTAT      = 0xf0;

    //Clearing the pending bit isn't needed because the pending bit has been cleared.
    while (iicDataCount != -1)
        Run_IicPoll();

    iicMode = POLLACK;
    while (1)
    {
        rIICDS     = DevAddr;
        iicStatus = 0x100;                 //To check if _iicStatus is changed
        rIICSTAT   = 0xf0;           //Master Tx, Start, Output Enable, Sucessful, Cleared, Cleared, 0
        rIICCON    = 0xe0;//0xaf;     //Resumes IIC operation. //hzh
        while (iicStatus == 0x100)
            Run_IicPoll();

        if (!(iicStatus & 0x1))
            break;                      //When ACK is received
    }
    rIICSTAT = 0xd0;                     //Master Tx condition, Stop(Write), Output Enable
    rIICCON  = 0xe0;//0xaf;              //Resumes IIC operation.  //hzh

    for (i = 0; i < 1000; i++);
    //  Delay(1);                           //Wait until stop condtion is in effect.
    //Write is completed.
}

//**********************************************************************/

static void IicPoll(void)
{
    U32 iicSt, i;
    iicSt = rIICSTAT;
    if (iicSt & 0x8) {};                 //When bus arbitration is failed. 总线仲裁失败
    if (iicSt & 0x4) {};                  //When a slave address is matched with IICADD
    if (iicSt & 0x2) {};                 //When a slave address is 0000000b
    if (iicSt & 0x1) {} ;                //When ACK isn't received

    switch (iicMode)
    {
    case POLLACK:
        iicStatus = iicSt;
        break;

    case RDDATA:
        if ((iicDataCount--) == 0)
        {
            iicData[iicPt++] = rIICDS;

            rIICSTAT = 0x90;           //Stop MasRx condition
            rIICCON  = 0xe0;//0xaf;     //Resumes IIC operation.
            for (i = 0; i < 1000; i++);
            //  Delay(1);                   //Wait until stop condtion is in effect.
            //Too long time...
            //The pending bit will not be set after issuing stop condition.
            break;
        }
        iicData[iicPt++] = rIICDS;
        //The last data has to be read with no ack.
        if ((iicDataCount) == 0)
            rIICCON = 0x60;//0x2f;       //Resumes IIC operation with NOACK.
        else
            rIICCON = 0xe0;//0xaf;      //Resumes IIC operation with ACK
        break;

    case WRDATA:
        if ((iicDataCount--) == 0)
        {
            rIICSTAT = 0xd0;           //stop MasTx condition
            rIICCON  = 0xe0;//0xaf;     //resumes IIC operation.
            for (i = 0; i < 1000; i++);
            //  Delay(1);                   //wait until stop condtion is in effect.
            //The pending bit will not be set after issuing stop condition.
            break;
        }
        rIICDS = iicData[iicPt++];        //_iicData[0] has dummy.
        for (i = 0; i < 10; i++);        //for setup time until rising edge of IICSCL
        rIICCON = 0xe0;//0xaf;          //resumes IIC operation.
        break;

    case SETRDADDR:
        if ((iicDataCount--) == 0)
        {
            break;                  //IIC operation is stopped because of IICCON[4]
        }
        rIICDS = iicData[iicPt++];
        for (i = 0; i < 10; i++);   //for setup time until rising edge of IICSCL
        rIICCON = 0xe0;//0xaf;             //resumes IIC operation.
        break;
    default:
        break;
    }
}


static void Run_IicPoll(void)
{
    if (rIICCON & 0x10)                 //Tx/Rx Interrupt Enable
        IicPoll();
}




