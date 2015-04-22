
//Last Modify Time:03/11/07 01:22   
//ReadMe   
//屏宽:112   
//屏高:64   
#include <reg51.h>   
#include <intrins.h>   
#include <absacc.h>   
//   
typedef struct   
    {   
    unsigned int KeyTab_MenuIndex;      //当前状态索引号   
    unsigned int KeyTab_MaxItems;       //本级菜单最大条目数   
    unsigned int KeyTab_PressOk;        //按下"回车"键时转向的状态索引号   
    unsigned int KeyTab_PressEsc;       //按下"返回"键时转向的状态索引号   
    unsigned int KeyTab_PressDown;      //按下"向下"键时转向的状态索引号   
    unsigned int KeyTab_PressUp;        //按下"向上"键时转向的状态索引号   
    void    (*CurrentOperate)();        //当前状态应该执行的功能操作   
    }KeyTabStruct;   
void    (*KeyFuncPtr)();    //按键功能指针   
//   
#define S_S         0x80//上      =1000,0000=0x80,   
#define S_X         0x40//下      =0100,0000=0x40,   
#define S_Z         0x20//左      =0010,0000=0x20,   
#define S_Y         0x10//右      =0001,0000=0x10,   
#define S_ZS        0xa0//左上    =1010,0000=0xa0,   
#define S_ZX        0x60//左下    =0110,0000=0x60,   
#define S_YS        0x90//右上    =1001,0000=0x90,   
#define S_YX        0x50//右下    =0101,0000=0x50,   
#define S_SXZY      0xf0//上下左右=1111,0000=0xf0   
           
//   
#define KEY_OK  0x11   
#define KEY_UP  0x12   
#define KEY_ESC 0x13   
//   
#define KEY_LEFT    0x21   
#define KEY_DOWN    0x22   
#define KEY_RIGHT   0x23   
//   
#define KEY_1       0x31   
#define KEY_2       0x32   
#define KEY_3       0x33   
//   
#define KEY_4       0x41   
#define KEY_5       0x42   
#define KEY_6       0x43   
//   
#define KEY_7       0x51   
#define KEY_8       0x52   
#define KEY_9       0x53   
//   
#define KEY_XING    0x61   
#define KEY_0       0x62   
#define KEY_JING    0x63   
//   
#define KEY_POWER   0x71   
#define KEY_NULL    0x00   
//   
//菜单定义  
#define MAX_KEYTABSTRUCT_NUM                        19   
#define MENU_FACE                                   0   
#define MENU_ROOT                                   1   
#define     MENU_VIEW                               2   
#define         MENU_VIEW_ABOUTRECORDE              3   
#define         MENU_VIEW_HANDSETCLOCK              4   
#define     MENU_OPERATION                          5   
#define         MENU_OPERATION_SETPOSITION          6   
#define         MENU_OPERATION_READZJTOFLASH        7   
#define         MENU_OPERATION_RDZJTORAM            8   
#define         MENU_OPERATION_DELGIVENZL           9   
#define         MENU_OPERATION_DELALLZJ             10   
#define     MENU_TEST                               11   
#define         MENU_TEST_RAMTEST                   12   
#define         MENU_TEST_FLASHTEST                 13   
#define     MENU_SYSTEMSET                          14   
#define         MENU_SYSTEMSET_DEBUG                15   
#define             MENU_SYSTEMSET_DEBUG_PASSWORD   16   
#define             MENU_SYSTEMSET_DEBUG_SEEPICTURE 17   
#define         MENU_SYSTEMSET_SETHANDCLK           18   
//   
#define Addr_Key            0xc000 //1100,0000,0000,0000   
#define Addr_XRamBegin1     0x0000 //0000,0000,0000,0000//共32K   
#define Addr_XRamEnd1       0x3fff //0011,1111,1111,1111   
#define Addr_XRamBegin2     0x4000 //0100,0000,0000,0000   
#define Addr_XRamEnd2       0x7fff //0111,1111,1111,1111   
#define Addr_XFlashBegin    0x9000//1000,0000,0000,0000 16K/每自然扇区   
#define Addr_XFlashEnd      0xbfff//1011,1111,1111,1111   
#define Addr_XFlashSector   0xc001//1100,0000,0000,0001 4个自然扇区(把FLASH的地址线移到这个口内容的低位)   
//Flash可用容量=4*16K=64K   
//自然扇区的内容字节范围(0->3)(0000,0000~0000,0011)共4块=64K,硬件上把A18,A17,A16锁住了，不能写)   
//能读不能写的扇区号为4-31为(0000,0100~0001,1111)共28块=448K,可以作为菜单位置   
//#define Addr_XFlashBegin      0x9000//1000,0000,0000,0000 16K/每自然扇区   
//#define Addr_XFlashEnd        0xbfff//1011,1111,1111,1111   
//能读能写的FLASH的连续地址如下:   
//#define Addr_XFlashBegin 0x0000//共64K   
//#define Addr_XFlashEnd   0xffff   
//   
//操作FLASH地址时，   
//1.算出页地址   
//2. 对扇区解锁(读扇区不用解锁):(0x5555)=0xaa,(0x2aaa)=0x55,(0x5555)=0xa0;   
//3。把高2位移入[Addr_XFlashSector]=0的低2位，   
//4。把原来的高2位置0，再或上10B   
//5. 然后整个扇区读/写256字节   
//片外数组0x0000~0x1fff;   
    unsigned char xdata Rev232Buffer[256]           _at_    0x0000; //->0x00ff   
    unsigned char xdata Send232Buffer[256]          _at_    0x0100; //->0x1ff   
    #define Addr_FlashWriteBuffer1                          0x0200  //->0x2ff   
    #define Addr_FlashWriteBuffer2                          0x0300  //->0x3ff   
    #define Addr_FlashReadBuffer1                           0x0400  //->0x4ff   
    #define Addr_FlashReadBuffer2                           0x0500  //->0x5ff   
    #define Addr_ZjWrRdBuffer                               0x0800  //->0xfff   
    unsigned char xdata clock[12]                   _at_    0x1000; //->0x100b   
    //   
    unsigned char xdata cGraphByte[8]               _at_    0x100e; //->0x1015   
    unsigned char xdata * xdata cpGraph             _at_    0x1016; //->0x1017   
    signed   char xdata cMenuHLight[0x10]           _at_    0x1018;   
    KeyTabStruct xdata KeyTab[MAX_KEYTABSTRUCT_NUM] _at_    0x1028;   
    unsigned char xdata CxTestByte1                 _at_    0x1145;   
//片外单变量0x2000~0x2fff;   
    //通讯专用   
    unsigned char xdata cUartTemp1      _at_ 0x2000;   
    unsigned char xdata cUartTemp2      _at_ 0x2001;   
    unsigned char xdata cUartTemp3      _at_ 0x2002;   
    unsigned char xdata cUartFunSn      _at_ 0x2003;   
    unsigned char xdata cUartDataLenght _at_ 0x2004;   
    unsigned char xdata cUartAddSum     _at_ 0x2005;   
    unsigned char xdata cUartXorSum     _at_ 0x2006;   
    unsigned char xdata iUartTemp1      _at_ 0x2007;   
    unsigned char xdata iUartTemp2      _at_ 0x2008;   
    unsigned char xdata iUartTemp3      _at_ 0x2009;   
    //Flash专用   
    unsigned char xdata cPageAddr       _at_ 0x2010;   
    unsigned int  xdata iSectorAddr     _at_ 0x2011;   
    unsigned int  xdata iFlashToRamAddr _at_ 0x2013;   
    unsigned int  xdata iRamToFlashAddr _at_ 0x2015;   
    unsigned char xdata iFlashAddrH     _at_ 0x2017;   
    unsigned char xdata iFlashAddrM     _at_ 0x2018;   
    unsigned char xdata iFlashAddrL     _at_ 0x2019;   
    unsigned char xdata iFlashRdWrNum   _at_ 0x201a;   
    //菜单专用   
    unsigned char xdata cMenuTemp1      _at_ 0x2020;   
    unsigned char xdata cMenuTemp2      _at_ 0x2021;   
    unsigned char xdata cMenuTemp3      _at_ 0x2022;   
    //   
    unsigned int  xdata iMenuID         _at_ 0x2023;   
    unsigned int  xdata iMenuNextID     _at_ 0x2025;   
    //   
    //画线专用   
    unsigned char xdata cLineTemp1      _at_ 0x2030;   
    unsigned char xdata cLineTemp2      _at_ 0x2031;   
    unsigned char xdata cLineTemp3      _at_ 0x2032;   
    unsigned char xdata cLineTemp4      _at_ 0x2033;   
    unsigned char xdata cLineTemp5      _at_ 0x2034;   
    unsigned char xdata cLineTemp6      _at_ 0x2035;   
    unsigned char xdata cLineTemp7      _at_ 0x2036;   
    unsigned char xdata cLineTemp8      _at_ 0x2037;   
    unsigned char xdata cLineTemp9      _at_ 0x2038;   
    unsigned char xdata cLineTemp10     _at_ 0x2039;   
    unsigned char xdata cLineTemp11     _at_ 0x203a;   
    unsigned char xdata cLineTemp12     _at_ 0x203b;   
    unsigned char xdata cLineTemp13     _at_ 0x203c;   
    //DispTextLine   
    unsigned char xdata cTextLineTemp1  _at_ 0x203d;   
    unsigned char xdata cTextLineTemp2  _at_ 0x203e;   
    unsigned char xdata cTextLineTemp3  _at_ 0x203f;   
    //图形字符专用   
    unsigned char xdata cGraphTemp1     _at_ 0x2040;   
    unsigned char xdata cGraphTemp2     _at_ 0x2041;   
    unsigned char xdata cGraphTemp3     _at_ 0x2042;   
    unsigned char xdata cGraphTemp4     _at_ 0x2043;   
    //主程序用   
    unsigned char xdata NowKey          _at_ 0x2044;   
    unsigned char xdata RxPos           _at_ 0x2045;   
    unsigned char xdata RxStep          _at_ 0x2046;   
    unsigned char xdata RxPrev          _at_ 0x2047;   
    unsigned char xdata RxNow           _at_ 0x2048;   
    unsigned char xdata RxDataLenght    _at_ 0x2049;   
    unsigned char xdata TxPos           _at_ 0x204a;   
    unsigned char xdata TxNum           _at_ 0x204b;   
    //中断0用   
    unsigned char xdata cTemp_ex0       _at_ 0x204c;   
    //通讯中断用   
    unsigned int  xdata i_uart          _at_ 0x204d;   
    //写Flash用   
    unsigned int  xdata i_FlashWr       _at_ 0x204f;   
    unsigned char xdata c1_FlashWr      _at_ 0x2051;   
    //读Flash用   
    unsigned int  xdata i_FlashRd       _at_ 0x2052;   
    unsigned char xdata c1_FlashRd      _at_ 0x2054;   
    //Make05Answe专用   
    unsigned int  xdata i_Make05Answer    _at_ 0x2055;   
    unsigned char xdata cAdd_Make05Answer _at_ 0x2057;   
    unsigned char xdata cXor_Make05Answer _at_ 0x2058;   
    //读时间用   
    unsigned char xdata i_RdTime            _at_ 0x2059;   
    //显示文本行用   
    unsigned char xdata col_DispTextLine    _at_ 0x205a;   
    unsigned char code * xdata p_DispTextLine _at_ 0x205b;   
    //显示开机画面   
    unsigned char xdata i_FaceBmp           _at_ 0x205d;   
    unsigned char xdata j_FaceBmp           _at_ 0x205e;   
    unsigned char xdata k_FaceBmp           _at_ 0x205f;   
    unsigned char xdata c1_FaceBmp          _at_ 0x2060;   
    //DisponeTextChar   
    unsigned char xdata i_DisponeTextChar   _at_ 0x2061;   
    unsigned char xdata j_DisponeTextChar   _at_ 0x2062;   
    unsigned char xdata k_DisponeTextChar   _at_ 0x2063;   
    unsigned char xdata cTemp_DisponeTextChar   _at_ 0x2064;   
    //键处理程序专用   
    unsigned char xdata cKeyTemp1   _at_ 0x2065;   
    //定时器0专用   
    unsigned int  xdata iT0_1S          _at_ 0x2066;   
    unsigned char xdata cClock_Year     _at_ 0x2068;   
    unsigned char xdata cClock_Month    _at_ 0x2069;   
    unsigned char xdata cClock_Day      _at_ 0x206a;   
    unsigned char xdata cClock_Hour     _at_ 0x206b;   
    unsigned char xdata cClock_Minute   _at_ 0x206c;   
    unsigned char xdata cClock_Second   _at_ 0x206d;   
   
    //cx   
   
   
//Timer   
#define TH0data 0xb8//20ms=0xb800,1ms=0xfc66   
#define TL0data 0x00//   
//   
sbit LCD_CS=P1^0;   
sbit LCD_RESET=P1^1;   
sbit LCD_A0=P1^5;   
sbit LCD_WR=P1^6;   
sbit KEYINT=P3^2;   
sbit I2C_SCK=P3^4;   
sbit I2C_SDA=P3^5;   
//   
sbit KEYROW1=P1^5;   
sbit KEYROW2=P1^6;   
sbit KEYROW3=P1^7;   
//   
unsigned char bdata BitByte0;   
    sbit bOldOnline=BitByte0^7;   
    sbit b_ReceiveOver=BitByte0^6;   
    sbit b_KeyInt=BitByte0^5;   
    sbit b_Arrive1S=BitByte0^4;   
unsigned char bdata BitByte1;   
unsigned char bdata BitByte2;   
unsigned char bdata BitValue;   
    sbit bOnline=BitValue^7;   
    sbit bPowerOff=BitValue^6;   
unsigned char bdata BitValue1;   
    sbit bBit7=BitValue1^7;   
    sbit bBit6=BitValue1^6;   
    sbit bBit5=BitValue1^5;   
    sbit bBit4=BitValue1^4;   
    sbit bBit3=BitValue1^3;   
    sbit bBit2=BitValue1^2;   
    sbit bBit1=BitValue1^1;   
    sbit bBit0=BitValue1^0;   
   
//   
//   
void lcd_write_d(unsigned char c1);   
void lcd_write_c(unsigned char c1);   
void LcdInit();   
void MyInit();   
//   
void DispOneBoxTextChar(unsigned char Row,unsigned char Col,unsigned char c1,unsigned char DispAttrib);   
void DispTwoBoxHexChar(unsigned char Row,unsigned char Col,unsigned char c1,unsigned char DispAttrib);   
void DispGraphChar(unsigned char Row,unsigned char Col,unsigned char xdata *cArray,unsigned char DispAttrib);   
void DispTextLine(unsigned char Row,unsigned char code *MenuText,unsigned char DispAttrib);   
//   
void ReadyForKey();   
void DelayMs(unsigned int iMs);   
void UartProcess();   
void IAmZj();   
void IAmOk();   
void IAmUnKnow();   
void FlashSectorWr(unsigned int SourceXRamAddr,unsigned int TargetFlashAddr);   
void FlashSectorRd(unsigned int SourceFlashAddr,unsigned int TargetXRamAddr);   
void Make05Answer();   
void Make06Answer();   
void I2Cstart(void);   
void I2Csend(unsigned char);   
unsigned char I2Csuback(void);   
void I2Cmastack(void);   
unsigned char I2Cread(void);   
void I2Cnoack(void);   
void I2Cstop(void);   
void Make07Answer();   
unsigned char set_time();   
void Make08Answer();   
void MakeBox(unsigned char row1,unsigned char col1,unsigned char row2,unsigned char col2);   
void MenuFunctionNull();   
void MenuFunctionDemo1();   
void MenuFunctionDemo2();   
void MenuFunctionDemo3();   
void MenuFunctionDemo4();   
void MenuFunctionDemo5();   
void MenuFunctionDemo6();   
void MenuFunctionDemo7();   
void MenuFunctionDemo8();   
void MenuFunctionDemo9();   
void MenuFunctionDemo10();   
void MenuFunctionDemo11();   
void MenuFunctionDemo12();   
void MenuFunctionDemo13();   
void MenuFunctionDemo14();   
void MenuFunctionDemo15();   
void MenuFunctionDemo16();   
void MenuFunctionDemo17();   
void MenuFunctionDemo18();   
void MenuFunctionDemo19();   
//   
void DispFaceBmp();   
void DispMenu_Root();   
void DispMenu_View();   
void DispMenu_Operation();   
void DispMenu_Test();   
void DispMenu_SystemSet();   
void DispMenu_Debug();   
//   
void JudgeHLight();   
//   
unsigned char ReadTime();   
void ShowDesktopTime();   
//   
unsigned char code Echar[96][8];   
unsigned char code FaceBmp[112][8];   
//   
unsigned char code *Menu_Root[4]=   
    {   
    " View         ",   
    " Operation    ",   
    " Test         ",   
    " System Set   "   
    };   
unsigned int code Menu_RootID[4]=   
    {   
    MENU_VIEW,   
    MENU_OPERATION,   
    MENU_TEST,   
    MENU_SYSTEMSET   
    };   
//   
unsigned char code *Menu_View[2]=   
    {   
    " About ZL     ",   
    " HandSet CLK  "   
    };   
unsigned int code Menu_ViewID[2]=   
    {   
    MENU_VIEW_ABOUTRECORDE,   
    MENU_VIEW_HANDSETCLOCK   
    };   
//   
unsigned char code *Menu_Operation[5]=   
    {   
    " Set Position ",   
    " Rd Zj To ROM ",   
    " Rd ZL To Ram ",   
    " Del Given ZL ",   
    " Del All ZL   "   
    };   
unsigned int code Menu_OperationID[5]=   
    {   
    MENU_OPERATION_SETPOSITION,   
    MENU_OPERATION_READZJTOFLASH,   
    MENU_OPERATION_RDZJTORAM,   
    MENU_OPERATION_DELGIVENZL,   
    MENU_OPERATION_DELALLZJ   
    };   
//   
unsigned char code *Menu_Test[2]=   
    {   
    " Ram Test     ",   
    " Flash Test   "   
    };   
unsigned int code Menu_TestID[2]=   
    {   
    MENU_TEST_RAMTEST,   
    MENU_TEST_FLASHTEST   
    };   
//   
unsigned char code *Menu_SystemSet[2]=   
    {   
    " Debug        ",   
    " Set Hand Clk "   
    };   
unsigned int code Menu_SystemSetID[2]=   
    {   
    MENU_SYSTEMSET_DEBUG,   
    MENU_SYSTEMSET_SETHANDCLK   
    };   
//   
unsigned char code *Menu_Debug[2]=   
    {   
    " PassWord     ",   
    " See Picture  "   
    };   
unsigned int code Menu_DebugID[2]=   
    {   
    MENU_SYSTEMSET_DEBUG_PASSWORD,   
    MENU_SYSTEMSET_DEBUG_SEEPICTURE   
    };   
//   
unsigned char code MenuNULL[]={"              "};   
//   
main()   
{   
    MyInit();   
    LcdInit();   
    //   
    cMenuHLight[0x00]=0x00;   
    cMenuHLight[0x01]=0x00;   
    cMenuHLight[0x02]=0x00;   
    cMenuHLight[0x03]=0x00;   
    cMenuHLight[0x04]=0x00;   
    cMenuHLight[0x05]=0x00;   
    cMenuHLight[0x06]=0x00;   
    cMenuHLight[0x07]=0x00;   
    cMenuHLight[0x08]=0x00;   
    cMenuHLight[0x09]=0x00;   
    cMenuHLight[0x0a]=0x00;   
    cMenuHLight[0x0b]=0x00;   
    cMenuHLight[0x0c]=0x00;   
    cMenuHLight[0x0d]=0x00;   
    cMenuHLight[0x0e]=0x00;   
    cMenuHLight[0x0f]=0x00;   
    //   
    DelayMs(20);   
    //调试用   
    MenuFunctionNull();   
    //   
    BitValue=XBYTE[Addr_Key];   
    bOldOnline=(~bOnline);   
    b_KeyInt=0;   
    b_ReceiveOver=0;   
    b_Arrive1S=0;   
    //   
    DispFaceBmp();   
    //   
    iMenuID=MENU_FACE;   
    //   
    while(1)   
    {   
    ReadyForKey();   
    //   
    BitValue=XBYTE[Addr_Key];   
    if(bOldOnline!=bOnline)   
        {   
        bOldOnline=bOnline;   
        if(!bOldOnline)   
            {//Line=1   
            }   
        else   
            {//Line=0;   
            }   
        }   
    if(b_ReceiveOver)   
        {   
        //   
        UartProcess();   
        //   
        b_ReceiveOver=0;   
        RxStep=0;   
        RxPos=0;   
        REN=1;   
        }   
    //   
    if(b_KeyInt)   
        {   
        switch(NowKey)   
            {   
            case    KEY_OK://KEY_OK,        KEY_UP,     KEY_ESC,   
                    if(KeyTab[iMenuID].KeyTab_PressOk!=999)   
                        {   
                        iMenuID=KeyTab[iMenuID].KeyTab_PressOk;   
                        iMenuNextID=KeyTab[iMenuID].KeyTab_PressOk;   
                        JudgeHLight();   
                        }   
                    KeyFuncPtr=KeyTab[iMenuID].CurrentOperate;   
                    (*KeyFuncPtr)();    //执行当前按键的操作   
                    break;   
            case    KEY_UP:   
                    if((KeyTab[iMenuID].KeyTab_PressUp!=999)&&(iMenuNextID!=999))   
                        {   
                        KeyTab[iMenuID].KeyTab_PressOk=KeyTab[iMenuNextID].KeyTab_PressUp;   
                        iMenuNextID=KeyTab[iMenuNextID].KeyTab_PressUp;   
                        JudgeHLight();   
                        }   
                    break;   
            case    KEY_ESC:   
                    //   
                    if(KeyTab[iMenuID].KeyTab_PressEsc!=999)   
                        {   
                        iMenuID=KeyTab[iMenuID].KeyTab_PressEsc;   
                        iMenuNextID=KeyTab[iMenuID].KeyTab_PressOk;   
                        JudgeHLight();   
                        KeyFuncPtr=KeyTab[iMenuID].CurrentOperate;   
                        (*KeyFuncPtr)();    //执行当前按键的操作   
                        }   
                    break;   
                    //   
            case    KEY_LEFT://KEY_LEFT,    KEY_DOWN,   KEY_RIGHT,   
                    if((KeyTab[iMenuID].KeyTab_PressUp!=999)&&(iMenuNextID!=999))   
                        {   
                        KeyTab[iMenuID].KeyTab_PressOk=KeyTab[iMenuNextID].KeyTab_PressUp;   
                        iMenuNextID=KeyTab[iMenuNextID].KeyTab_PressUp;   
                        JudgeHLight();   
                        }   
                    break;   
            case    KEY_DOWN:   
                    if((KeyTab[iMenuID].KeyTab_PressDown!=999)&&(iMenuNextID!=999))   
                        {   
                        KeyTab[iMenuID].KeyTab_PressOk=KeyTab[iMenuNextID].KeyTab_PressDown;   
                        iMenuNextID=KeyTab[iMenuNextID].KeyTab_PressDown;   
                        JudgeHLight();   
                        }   
                    break;   
            case    KEY_RIGHT:   
                    if((KeyTab[iMenuID].KeyTab_PressDown!=999)&&(iMenuNextID!=999))   
                        {   
                        KeyTab[iMenuID].KeyTab_PressOk=KeyTab[iMenuNextID].KeyTab_PressDown;   
                        iMenuNextID=KeyTab[iMenuNextID].KeyTab_PressDown;   
                        JudgeHLight();   
                        }   
                    break;   
                    //   
            case    KEY_1://KEY_1,      KEY_2,      KEY_3,   
                    break;   
            case    KEY_2:   
                    break;   
            case    KEY_3:   
                    break;   
            case    KEY_4://KEY_4,      KEY_5,      KEY_6,   
                    break;   
            case    KEY_5:   
                    break;   
            case    KEY_6:   
                    break;   
            case    KEY_7://KEY_7,      KEY_8,      KEY_9,   
                    break;   
            case    KEY_8:   
                    break;   
            case    KEY_9:   
                    break;   
            case    KEY_XING://KEY_XING,    KEY_0,      KEY_JING,   
                    break;   
            case    KEY_0:   
                    break;   
            case    KEY_JING:   
                    break;   
            case    KEY_POWER://KEY_POWER,  KEY_NULL   
                    break;   
            case    KEY_NULL:   
                    MakeBox(3,3,5,12);   
                    break;   
                    //   
            default:   
                    break;   
            }   
        b_KeyInt=0;   
        }   
    if((b_Arrive1S==1)&&(iMenuID==MENU_FACE))   
        {   
        ReadTime();   
        //   
        ShowDesktopTime();   
        //   
        b_Arrive1S=0;   
        }   
    }   
}   
void isr_ex0(void)  interrupt 0 using 1   
{   
    b_KeyInt=1;   
    //   
    cTemp_ex0=0xff;   
    //   
    DelayMs(20);   
    ReadyForKey();   
    cTemp_ex0=XBYTE[Addr_Key];   
    //   
    cTemp_ex0=cTemp_ex0&0x3f;   
    switch(cTemp_ex0)   
        {   
        case        0x3e://第一行   
            NowKey=0x10;   
            break;   
        case        0x3d://第二行   
            NowKey=0x20;   
            break;   
        case        0x3b://第三行   
            NowKey=0x30;   
            break;   
        case        0x37://第四行   
            NowKey=0x40;   
            break;   
        case        0x2f://第五行   
            NowKey=0x50;   
            break;   
        case        0x1f://第六行   
            NowKey=0x60;   
            break;   
        default:   
            NowKey=0x00;   
            break;   
        }   
    //   
    KEYROW1=0;   
    KEYROW2=1;   
    KEYROW3=1;   
    cTemp_ex0=XBYTE[Addr_Key];   
    //   
    cTemp_ex0&=0x3f;   
    if(cTemp_ex0!=0x3f)   
        {   
        NowKey=NowKey|0x01;   
        }   
    else   
        {   
        KEYROW1=1;   
        KEYROW2=0;   
        KEYROW3=1;   
        cTemp_ex0=XBYTE[Addr_Key];   
        //   
            cTemp_ex0&=0x3f;   
            if(cTemp_ex0!=0x3f)   
                {   
                NowKey=NowKey|0x02;   
                }   
            else   
                {   
                KEYROW1=1;   
                KEYROW2=1;   
                KEYROW3=0;   
                cTemp_ex0=XBYTE[Addr_Key];   
                cTemp_ex0&=0x3f;   
                if(cTemp_ex0!=0x3f)   
                    {   
                    NowKey=NowKey|0x03;   
                    }   
                else   
                    {   
                    BitValue=XBYTE[Addr_Key];   
                    if(!bPowerOff)   
                        {   
                        NowKey=0x71;   
                        }   
                    else   
                        {   
                        NowKey=0x00;   
                        }   
                    }   
                }   
        }   
    //   
    do   
        {   
        DelayMs(100);   
        }while(!KEYINT);   
    //   
        IE0=0;//清外中断0请求   
}   
//   
void isr_t0(void)   interrupt 1 using 2   
{   
    TL0=TL0data;   
    TH0=TH0data;   
    iT0_1S+=20;//因为定时器的定时为20Ms一次   
    if(iT0_1S>=1000)   
        {   
        iT0_1S=0;   
        b_Arrive1S=1;   
        }   
}   
//   
void isr_ex1(void)  interrupt 2 using 1   
{   
    _nop_();   
}   
//   
void isr_t1(void)   interrupt 3 using 2   
{   
    _nop_();   
}   
//   
void isr_ser(void)  interrupt 4 using 3   
{   
if(TI)   
    {   
    if((TxNum--)>0)   
        {   
        SBUF=Send232Buffer[TxPos++];   
        }   
    else   
        {   
        TxNum=0;   
        TxPos=0;   
        }   
    //   
    TI=0;   
    }   
else   
    {   
    RI=0;   
    RxNow=SBUF;   
    Rev232Buffer[RxPos++]=RxNow;   
    if(RxPos>=255)   
        {   
        RxPos=0;   
        }   
    //   
    if(!b_ReceiveOver)   
        {   
        switch(RxStep)   
            {   
            case    0://帧头:55 55   
                if(RxNow==0x55)   
                    {   
                    if(RxPrev==0x55)   
                        {   
                        RxStep=1;   
                        }   
                    else   
                        {   
                        RxPrev=RxNow;   
                        RxPos=0;   
                        Rev232Buffer[RxPos++]=RxNow;   
                        RxStep=0;   
                        }   
                    }   
                else   
                    {   
                    RxStep=0;   
                    RxPos=0;   
                    REN=1;   
                    }   
                break;   
            case    1://命令帧和数据帧的判断   
                if((RxNow&0x80)==0)   
                    {   
                    RxStep=3;   
                    }   
                else   
                    {   
                    RxPrev=RxNow;   
                    RxStep=2;   
                    }   
                break;   
            case    2://命令帧，第二个命令字（和第一个命令字相同)   
                if(RxNow==RxPrev)   
                    {   
                    RxStep=7;   
                    }   
                else   
                    {   
                    RxStep=0;   
                    RxPos=0;   
                    REN=1;   
                    }   
                break;   
            case    3://数据帧,帧长度   
                RxDataLenght=RxNow;   
                RxStep=4;   
                break;   
            case    4://数据帧,数据段   
                RxDataLenght--;   
                if(RxDataLenght==0)   
                    {   
                    RxStep=5;   
                    }   
                break;   
            case    5://数据帧,累加和   
                RxStep=6;   
                break;   
            case    6://数据帧,异或和   
                RxStep=8;   
                break;   
            case    7://命令帧,累加和   
                RxStep=8;   
                break;   
            case    8://帧尾:aa aa   
                if(RxNow==0xaa)   
                    {   
                    if(RxPrev==0xaa)   
                        {   
                        b_ReceiveOver=1;   
                        REN=0;   
                        }   
                    else   
                        {   
                        RxPrev=RxNow;   
                        }   
                    }   
                else   
                    {   
                    RxStep=0;   
                    RxPos=0;   
                    REN=1;   
                    }   
                break;   
            default:   
                RxStep=0;   
                RxPos=0;   
                REN=1;   
                break;   
            }   
        }   
    }   
}   
//   
void UartProcess()   
{   
//   
cUartTemp1=0;   
cUartTemp2=0;   
cUartTemp3=0;   
cUartFunSn=0xff;   
//   
cUartTemp1=Rev232Buffer[0];   
cUartTemp2=Rev232Buffer[1];   
if((cUartTemp1==0x55)&(cUartTemp2==0x55))   
    {   
    cUartTemp1=Rev232Buffer[2];   
    cUartTemp1&=0x80;   
    if(cUartTemp1!=0)   
        {//命令帧   
        cUartTemp1=Rev232Buffer[2];   
        cUartTemp2=Rev232Buffer[3];   
        if(cUartTemp1==cUartTemp2)   
            {   
            cUartTemp3=cUartTemp1+cUartTemp2;   
            cUartTemp1=Rev232Buffer[4];   
            if(cUartTemp1==cUartTemp3)   
                {   
                cUartTemp1=Rev232Buffer[5];   
                cUartTemp2=Rev232Buffer[6];   
                if((cUartTemp1==0xaa)&&(cUartTemp2=0xaa))   
                    {   
                    cUartFunSn=Rev232Buffer[2];   
                    }   
                }   
            }   
        }   
    else   
        {//数据帧   
        cUartAddSum=Rev232Buffer[2]+Rev232Buffer[3];   
        cUartXorSum=Rev232Buffer[2]^Rev232Buffer[3];   
        cUartDataLenght=Rev232Buffer[3];   
        for(i_uart=4;i_uart<(cUartDataLenght+4);i_uart++)   
            {   
            cUartTemp1=Rev232Buffer[i_uart];   
            cUartAddSum+=cUartTemp1;   
            cUartXorSum^=cUartTemp1;   
            }   
        cUartTemp1=Rev232Buffer[cUartDataLenght+4];   
        cUartDataLenght++;   
        cUartTemp2=Rev232Buffer[cUartDataLenght+4];   
        cUartDataLenght++;   
        if((cUartTemp1==cUartAddSum)&&(cUartTemp2==cUartXorSum))   
            {   
            cUartTemp1=Rev232Buffer[cUartDataLenght+4];   
            cUartDataLenght++;   
            cUartTemp2=Rev232Buffer[cUartDataLenght+4];   
            cUartDataLenght++;   
            if((cUartTemp1==0xaa)&&(cUartTemp2==0xaa))   
                {   
                cUartFunSn=Rev232Buffer[6];   
                }   
            }   
        }   
    }   
switch(cUartFunSn)   
    {   
    case    0xa5:   
    case    0xa6://我要伪装成座机0xa7   
        IAmZj();   
        break;   
    case    0xac:   
    case    0xad:   
    case    0xae:   
        IAmOk();   
        break;   
    case    5:   
        iFlashAddrH=Rev232Buffer[7];   
        iFlashAddrM=Rev232Buffer[8];   
        iFlashAddrL=Rev232Buffer[9];   
        iFlashRdWrNum=Rev232Buffer[10];   
        //   
        i_uart=iFlashAddrM;   
        i_uart=(i_uart<<8)&0xff00;   
        FlashSectorRd(i_uart,Addr_FlashReadBuffer1);   
        //   
        i_uart=iFlashAddrM+1;   
        i_uart=(i_uart<<8)&0xff00;   
        FlashSectorRd(i_uart,Addr_FlashReadBuffer2);   
        //   
        Make05Answer();   
        //   
        break;   
    case    6:   
        iFlashAddrH=Rev232Buffer[7];   
        iFlashAddrM=Rev232Buffer[8];   
        iFlashAddrL=Rev232Buffer[9];   
        iFlashRdWrNum=Rev232Buffer[10];   
        //   
        i_uart=iFlashAddrM;   
        i_uart=(i_uart<<8)&0xff00;   
        FlashSectorRd(i_uart,Addr_FlashWriteBuffer1);   
        //   
        i_uart=iFlashAddrM+1;   
        i_uart=(i_uart<<8)&0xff00;   
        FlashSectorRd(i_uart,Addr_FlashWriteBuffer2);   
        //   
        cUartTemp1=iFlashRdWrNum;   
        iUartTemp1=iFlashAddrL;   
        cUartTemp2=0;   
        while(cUartTemp1>0)   
            {   
            XBYTE[Addr_FlashWriteBuffer1+iUartTemp1]=Rev232Buffer[11+cUartTemp2];   
            cUartTemp1--;   
            iUartTemp1++;   
            cUartTemp2++;   
            }   
        //   
        i_uart=iFlashAddrM;   
        i_uart=(i_uart<<8)&0xff00;   
        FlashSectorWr(Addr_FlashWriteBuffer1,i_uart);   
        //   
        i_uart=iFlashAddrM+1;   
        i_uart=(i_uart<<8)&0xff00;   
        FlashSectorWr(Addr_FlashWriteBuffer2,i_uart);   
        //   
        Make06Answer();   
        break;   
    case    7:   
        ReadTime();   
        Make07Answer();   
        break;   
    case    8:   
        //               5     7  8  9  10 11 12   
        //55 55 03 09 00 CF 08 03 11 11 19 39 15 6F FB AA AA   
        clock[0]=0xa2;//I2C address   
        clock[1]=0x02;//address   
        clock[8]=Rev232Buffer[7];//year   
        clock[7]=Rev232Buffer[8];//month   
        clock[5]=Rev232Buffer[9];//date   
        clock[4]=Rev232Buffer[10];//hours   
        clock[3]=Rev232Buffer[11];//minutes   
        clock[2]=Rev232Buffer[12];//seconds   
        set_time();   
        Make08Answer();   
        break;   
    default:   
        IAmUnKnow();   
        RxStep=0;   
        RxPos=0;   
        break;   
    }   
}   
//   
void IAmZj()   
{   
    Send232Buffer[0]=0x55;   
    Send232Buffer[1]=0x55;   
    Send232Buffer[2]=0xa7;   
    Send232Buffer[3]=0xa7;   
    Send232Buffer[4]=0x4e;   
    Send232Buffer[5]=0xaa;   
    Send232Buffer[6]=0xaa;   
    TxPos=0;   
    TxNum=7;   
    TI=1;   
}   
void IAmOk()   
{   
    Send232Buffer[0]=0x55;   
    Send232Buffer[1]=0x55;   
    Send232Buffer[2]=0x80;   
    Send232Buffer[3]=0x80;   
    Send232Buffer[4]=0x00;   
    Send232Buffer[5]=0xaa;   
    Send232Buffer[6]=0xaa;   
    TxPos=0;   
    TxNum=7;   
    TI=1;   
}   
void IAmUnKnow()   
{   
    Send232Buffer[0]=0x55;   
    Send232Buffer[1]=0x55;   
    Send232Buffer[2]=0x84;   
    Send232Buffer[3]=0x84;   
    Send232Buffer[4]=0x08;   
    Send232Buffer[5]=0xaa;   
    Send232Buffer[6]=0xaa;   
    TxPos=0;   
    TxNum=7;   
    TI=1;   
}   
//   
void FlashSectorWr(unsigned int SourceXRamAddr,unsigned int TargetFlashAddr)   
{   
//   
cPageAddr=(unsigned char)((TargetFlashAddr>>14)&0x0003);   
iSectorAddr=(TargetFlashAddr&0x3f00)|0x8000;   
iRamToFlashAddr=SourceXRamAddr&0xff00;   
//解锁   
XBYTE[Addr_XFlashSector]=0x01;   
XBYTE[0x9555]=0xaa;   
XBYTE[Addr_XFlashSector]=0x00;   
XBYTE[0xaaaa]=0x55;   
XBYTE[Addr_XFlashSector]=0x01;   
XBYTE[0x9555]=0xa0;   
//   
XBYTE[Addr_XFlashSector]=cPageAddr;   
for(i_FlashWr=iSectorAddr;i_FlashWr<=(iSectorAddr+0xff);i_FlashWr++)   
    {   
    c1_FlashWr=XBYTE[iRamToFlashAddr++];   
    XBYTE[i_FlashWr]=c1_FlashWr;   
    }   
DelayMs(10);   
}   
void FlashSectorRd(unsigned int SourceFlashAddr,unsigned int TargetXRamAddr)   
{   
//   
cPageAddr=(unsigned char)((SourceFlashAddr>>14)&0x0003);   
iSectorAddr=(SourceFlashAddr&0x3f00)|0x8000;   
iFlashToRamAddr=TargetXRamAddr&0xff00;   
//   
XBYTE[Addr_XFlashSector]=cPageAddr;   
for(i_FlashRd=iSectorAddr;i_FlashRd<=(iSectorAddr+0xff);i_FlashRd++)   
    {   
    c1_FlashRd=XBYTE[i_FlashRd];   
    XBYTE[iFlashToRamAddr++]=c1_FlashRd;   
    }   
}//   
//   
void Make05Answer()   
{   
    i_Make05Answer=iFlashAddrL;   
    cUartTemp2=0;   
    Send232Buffer[cUartTemp2++]=0x55;   
    Send232Buffer[cUartTemp2++]=0x55;   
    Send232Buffer[cUartTemp2++]=0x03;   
    Send232Buffer[cUartTemp2++]=4+iFlashRdWrNum;   
    Send232Buffer[cUartTemp2++]=0x00;   
    Send232Buffer[cUartTemp2++]=Rev232Buffer[5];//*********   
    Send232Buffer[cUartTemp2++]=0x05;   
    Send232Buffer[cUartTemp2++]=iFlashRdWrNum;   
    cUartTemp1=iFlashRdWrNum;   
    while((cUartTemp1--)!=0)   
        {   
        Send232Buffer[cUartTemp2++]=XBYTE[Addr_FlashReadBuffer1+i_Make05Answer];   
        i_Make05Answer++;   
        }   
    cAdd_Make05Answer=Send232Buffer[2];   
    cXor_Make05Answer=Send232Buffer[2];   
    for(i_Make05Answer=3;i_Make05Answer<cUartTemp2;i_Make05Answer++)   
        {   
        cAdd_Make05Answer=cAdd_Make05Answer+Send232Buffer[i_Make05Answer];   
        cXor_Make05Answer=cXor_Make05Answer^Send232Buffer[i_Make05Answer];   
        }   
    Send232Buffer[cUartTemp2++]=cAdd_Make05Answer;   
    Send232Buffer[cUartTemp2++]=cXor_Make05Answer;   
    Send232Buffer[cUartTemp2++]=0xaa;   
    Send232Buffer[cUartTemp2++]=0xaa;   
    TxPos=0;   
    TxNum=cUartTemp2;   
    TI=1;   
}   
void Make06Answer()   
{//55 55 03 04 00 CE 06 00 DB CF AA AA   
    cUartTemp2=0;   
    Send232Buffer[cUartTemp2++]=0x55;   
    Send232Buffer[cUartTemp2++]=0x55;   
    Send232Buffer[cUartTemp2++]=0x03;   
    Send232Buffer[cUartTemp2++]=0x04;   
    Send232Buffer[cUartTemp2++]=0x00;   
    Send232Buffer[cUartTemp2++]=Rev232Buffer[5];//*********   
    Send232Buffer[cUartTemp2++]=0x06;   
    Send232Buffer[cUartTemp2++]=0x00;   
    cAdd_Make05Answer=Send232Buffer[2];   
    cXor_Make05Answer=Send232Buffer[2];   
    for(i_Make05Answer=3;i_Make05Answer<=7;i_Make05Answer++)   
        {   
        cAdd_Make05Answer=cAdd_Make05Answer+Send232Buffer[i_Make05Answer];   
        cXor_Make05Answer=cXor_Make05Answer^Send232Buffer[i_Make05Answer];   
        }   
    Send232Buffer[cUartTemp2++]=cAdd_Make05Answer;   
    Send232Buffer[cUartTemp2++]=cXor_Make05Answer;   
    Send232Buffer[cUartTemp2++]=0xaa;   
    Send232Buffer[cUartTemp2++]=0xaa;   
    TxPos=0;   
    TxNum=cUartTemp2;   
    TI=1;   
}   
//   
unsigned char ReadTime()   
{   
    for(i_RdTime=0;i_RdTime<9;i_RdTime++)   
        clock[i_RdTime]=0;   
    I2Cstart();   
    I2Csend(0xA2);   
    if(I2Csuback()==0)return 0;   
    I2Csend(0x02);   
    if(I2Csuback()==0)return 0;   
    I2Cstart();   
    I2Csend(0xA3);   
    if(I2Csuback()==0)return 0;   
    clock[0]=I2Cread()&0x7f;//seconds   
    I2Cmastack();   
    clock[1]=I2Cread()&0x7f;//min   
    I2Cmastack();   
    clock[2]=I2Cread()&0x3f;//hours   
    I2Cmastack();   
    clock[3]=I2Cread()&0x3f;//day   
    I2Cmastack();   
    clock[7]=I2Cread()&0x07;//week   
    I2Cmastack();   
    clock[4]=I2Cread()&0x1f;//mouth   
    I2Cmastack();   
    clock[5]=I2Cread()&0x7f;//year   
    I2Cnoack();   
    I2Cstop();   
    clock[6]=0x20;   
    cClock_Year=clock[5];   
    cClock_Month=clock[4];   
    cClock_Day=clock[3];   
    cClock_Hour=clock[2];   
    cClock_Minute=clock[1];   
    cClock_Second=clock[0];   
    return 1;   
   //   
   
}//   
//   
void I2Cstart(void)   
{   
   I2C_SCK=0;_nop_();_nop_();   
   I2C_SDA=1;_nop_();_nop_();   
   I2C_SCK=1;_nop_();_nop_();   
   I2C_SDA=0;_nop_();_nop_();   
   I2C_SCK=0;_nop_();_nop_();   
   I2C_SDA=1;_nop_();_nop_();   
   I2C_SCK=1;   
}   
//   
void I2Csend(unsigned char d)   
{   
   unsigned char i;   
   BitValue1=d;   
   for(i=0;i<8;i++){   
      I2C_SDA=bBit7;   
      BitValue1<<=1;_nop_();_nop_();   
      I2C_SCK=1;_nop_();_nop_();   
      I2C_SCK=0;   
   }   
}   
//   
void I2Cmastack(void)   
{   
   I2C_SDA=0;   
   I2C_SCK=1;_nop_();_nop_();_nop_();   
   I2C_SCK=0;   
   I2C_SDA=1;   
}   
//   
unsigned char I2Csuback(void)   
{   
   unsigned char i;   
   I2C_SDA=1;//_nop_();_nop_();   
   I2C_SCK=1;_nop_();_nop_();   
   for(i=0;i<255;i++){   
      _nop_();_nop_();   
      if(I2C_SDA==0){   
         I2C_SCK=0;   
         return 1;   
      }   
   }   
   I2C_SCK=0;   
   return 0;   
}   
//   
unsigned char I2Cread(void)   
{   
   unsigned char i;   
   BitValue1=0;   
   for(i=0;i<8;i++){   
      BitValue1<<=1;   
      I2C_SDA=1;   
      I2C_SCK=1;_nop_();_nop_();_nop_();   
      bBit0=I2C_SDA;   
      I2C_SCK=0;   
   }   
   return BitValue1;   
}   
//   
void I2Cnoack(void)   
{   
   I2C_SDA=1;   
   I2C_SCK=1;_nop_();_nop_();_nop_();   
   I2C_SCK=0;   
}   
//   
void I2Cstop(void)   
{   
   I2C_SCK=0;_nop_();_nop_();   
   I2C_SDA=0;_nop_();_nop_();   
   I2C_SCK=1;_nop_();_nop_();   
   I2C_SDA=1;   
}   
//   
void Make07Answer()   
{//55 55 03 09 00 CF 07 04 11 11 16 09 08 2F D1 AA AA   
    cUartTemp2=0;   
    Send232Buffer[cUartTemp2++]=0x55;   
    Send232Buffer[cUartTemp2++]=0x55;   
    Send232Buffer[cUartTemp2++]=0x03;   
    Send232Buffer[cUartTemp2++]=0x09;   
    Send232Buffer[cUartTemp2++]=0x00;   
    Send232Buffer[cUartTemp2++]=Rev232Buffer[5];//*********   
    Send232Buffer[cUartTemp2++]=0x07;   
    Send232Buffer[cUartTemp2++]=clock[5];//Year   
    Send232Buffer[cUartTemp2++]=clock[4];//Month   
    Send232Buffer[cUartTemp2++]=clock[3];//Day   
    Send232Buffer[cUartTemp2++]=clock[2];//Hour   
    Send232Buffer[cUartTemp2++]=clock[1];//Minute   
    Send232Buffer[cUartTemp2++]=clock[0];//Second   
    cAdd_Make05Answer=Send232Buffer[2];   
    cXor_Make05Answer=Send232Buffer[2];   
    for(i_Make05Answer=3;i_Make05Answer<cUartTemp2;i_Make05Answer++)   
        {   
        cAdd_Make05Answer=cAdd_Make05Answer+Send232Buffer[i_Make05Answer];   
        cXor_Make05Answer=cXor_Make05Answer^Send232Buffer[i_Make05Answer];   
        }   
    Send232Buffer[cUartTemp2++]=cAdd_Make05Answer;   
    Send232Buffer[cUartTemp2++]=cXor_Make05Answer;   
    Send232Buffer[cUartTemp2++]=0xaa;   
    Send232Buffer[cUartTemp2++]=0xaa;   
    TxPos=0;   
    TxNum=cUartTemp2;   
    TI=1;   
}//   
//   
unsigned char set_time(void)   
{   
   I2Cstart();   
   for(i_RdTime=0;i_RdTime<11;i_RdTime++){   
      I2Csend(clock[i_RdTime]);   
      if(I2Csuback()==0)return 0;   
   }   
   I2Cstop();   
   return 1;   
}   
//   
void Make08Answer()   
{//55 55 03 04 00 CF 08 00 DE C0 AA AA   
    cUartTemp2=0;   
    Send232Buffer[cUartTemp2++]=0x55;   
    Send232Buffer[cUartTemp2++]=0x55;   
    Send232Buffer[cUartTemp2++]=0x03;   
    Send232Buffer[cUartTemp2++]=0x04;   
    Send232Buffer[cUartTemp2++]=0x00;   
    Send232Buffer[cUartTemp2++]=Rev232Buffer[5];//*********   
    Send232Buffer[cUartTemp2++]=0x08;   
    Send232Buffer[cUartTemp2++]=0x00;   
    cAdd_Make05Answer=Send232Buffer[2];   
    cXor_Make05Answer=Send232Buffer[2];   
    for(i_Make05Answer=3;i_Make05Answer<=7;i_Make05Answer++)   
        {   
        cAdd_Make05Answer=cAdd_Make05Answer+Send232Buffer[i_Make05Answer];   
        cXor_Make05Answer=cXor_Make05Answer^Send232Buffer[i_Make05Answer];   
        }   
    Send232Buffer[cUartTemp2++]=cAdd_Make05Answer;   
    Send232Buffer[cUartTemp2++]=cXor_Make05Answer;   
    Send232Buffer[cUartTemp2++]=0xaa;   
    Send232Buffer[cUartTemp2++]=0xaa;   
    TxPos=0;   
    TxNum=cUartTemp2;   
    TI=1;   
}   
//   
void DispTextLine(unsigned char Row,unsigned char code *MenuText,unsigned char DispAttrib)   
{   
//   
p_DispTextLine=MenuText;   
col_DispTextLine=1;   
do   
    {   
    if((Row==1)&&(col_DispTextLine==1))//第一行   
        {   
        cTextLineTemp1=DispAttrib|S_ZS;   
        }   
    else   
    if((Row==1)&&(col_DispTextLine==14))   
        {   
        cTextLineTemp1=DispAttrib|S_YS;   
        }   
    else   
    if((Row==1)&&(col_DispTextLine!=1)&&(col_DispTextLine!=14))   
        {   
        cTextLineTemp1=DispAttrib|S_S;   
        }   
    else   
    if((Row==7)&&(col_DispTextLine==1))//第7行   
        {   
        cTextLineTemp1=DispAttrib|S_ZX;   
        }   
    else   
    if((Row==7)&&(col_DispTextLine==14))   
        {   
        cTextLineTemp1=DispAttrib|S_YX;   
        }   
    else   
    if((Row==7)&&(col_DispTextLine!=1)&&(col_DispTextLine!=14))   
        {   
        cTextLineTemp1=DispAttrib|S_X;   
        }   
    else   
    if((Row!=1)&&(Row!=7)&&(col_DispTextLine==1))//中间行的左右两边   
        {   
        cTextLineTemp1=DispAttrib|S_Z;   
        }   
    else   
    if((Row!=1)&&(Row!=7)&&(col_DispTextLine==14))   
        {   
        cTextLineTemp1=DispAttrib|S_Y;   
        }   
    else//中间行的剩下的字符   
        {   
        cTextLineTemp1=DispAttrib;   
        }   
        //   
    DispOneBoxTextChar(Row,col_DispTextLine,*p_DispTextLine,cTextLineTemp1);   
    col_DispTextLine++;   
    p_DispTextLine++;   
    }while(*p_DispTextLine!=0x00);   
}   
//   
void DispMenu_Root()   
{   
for(cMenuTemp1=0;cMenuTemp1<7;cMenuTemp1++)   
    {   
    if(cMenuTemp1<4)   
        {   
        if(iMenuNextID==Menu_RootID[cMenuTemp1])   
            {   
            DispTextLine(cMenuTemp1+1,&Menu_Root[cMenuTemp1][0],0);   
            }   
        else   
            {   
            DispTextLine(cMenuTemp1+1,&Menu_Root[cMenuTemp1][0],1);   
            }   
        }   
    else   
        {   
        DispTextLine(cMenuTemp1+1,&MenuNULL[0],1);   
        }   
    }   
}   
//   
void DispMenu_View()   
{   
for(cMenuTemp1=0;cMenuTemp1<7;cMenuTemp1++)   
    {   
    if(cMenuTemp1<2)   
        {   
        if(iMenuNextID==Menu_ViewID[cMenuTemp1])   
            {   
            DispTextLine(cMenuTemp1+1,&Menu_View[cMenuTemp1][0],0);   
            }   
        else   
            {   
            DispTextLine(cMenuTemp1+1,&Menu_View[cMenuTemp1][0],1);   
            }   
        }   
    else   
        {   
        DispTextLine(cMenuTemp1+1,&MenuNULL[0],1);   
        }   
    }   
   
}   
void DispMenu_Operation()   
{   
for(cMenuTemp1=0;cMenuTemp1<7;cMenuTemp1++)   
    {   
    if(cMenuTemp1<5)   
        {   
        if(iMenuNextID==Menu_OperationID[cMenuTemp1])   
            {   
            DispTextLine(cMenuTemp1+1,&Menu_Operation[cMenuTemp1][0],0);   
            }   
        else   
            {   
            DispTextLine(cMenuTemp1+1,&Menu_Operation[cMenuTemp1][0],1);   
            }   
        }   
    else   
        {   
        DispTextLine(cMenuTemp1+1,&MenuNULL[0],1);   
        }   
    }   
   
   
}   
void DispMenu_Test()   
{   
for(cMenuTemp1=0;cMenuTemp1<7;cMenuTemp1++)   
    {   
    if(cMenuTemp1<2)   
        {   
        if(iMenuNextID==Menu_TestID[cMenuTemp1])   
            {   
            DispTextLine(cMenuTemp1+1,&Menu_Test[cMenuTemp1][0],0);   
            }   
        else   
            {   
            DispTextLine(cMenuTemp1+1,&Menu_Test[cMenuTemp1][0],1);   
            }   
        }   
    else   
        {   
        DispTextLine(cMenuTemp1+1,&MenuNULL[0],1);   
        }   
    }   
   
}   
void DispMenu_SystemSet()   
{   
for(cMenuTemp1=0;cMenuTemp1<7;cMenuTemp1++)   
    {   
    if(cMenuTemp1<2)   
        {   
        if(iMenuNextID==Menu_SystemSetID[cMenuTemp1])   
            {   
            DispTextLine(cMenuTemp1+1,&Menu_SystemSet[cMenuTemp1][0],0);   
            }   
        else   
            {   
            DispTextLine(cMenuTemp1+1,&Menu_SystemSet[cMenuTemp1][0],1);   
            }   
        }   
    else   
        {   
        DispTextLine(cMenuTemp1+1,&MenuNULL[0],1);   
        }   
    }   
   
}   
void DispMenu_Debug()   
{   
for(cMenuTemp1=0;cMenuTemp1<7;cMenuTemp1++)   
    {   
    if(cMenuTemp1<2)   
        {   
        if(iMenuNextID==Menu_DebugID[cMenuTemp1])   
            {   
            DispTextLine(cMenuTemp1+1,&Menu_Debug[cMenuTemp1][0],0);   
            }   
        else   
            {   
            DispTextLine(cMenuTemp1+1,&Menu_Debug[cMenuTemp1][0],1);   
            }   
        }   
    else   
        {   
        DispTextLine(cMenuTemp1+1,&MenuNULL[0],1);   
        }   
    }   
   
}   
//   
void MakeBox(unsigned char row1,unsigned char col1,unsigned char row2,unsigned char col2)   
{   
for(cLineTemp1=row1;cLineTemp1<=row2;cLineTemp1++)   
    {   
    for(cLineTemp2=col1;cLineTemp2<=col2;cLineTemp2++)   
        {   
        cGraphByte[0]=0x00;   
        cGraphByte[1]=0x00;   
        cGraphByte[2]=0x00;   
        cGraphByte[3]=0x00;   
        cGraphByte[4]=0x00;   
        cGraphByte[5]=0x00;   
        cGraphByte[6]=0x00;   
        cGraphByte[7]=0x00;   
        //   
        if((cLineTemp1==row1)&&(cLineTemp2==col1))//左上角   
            {   
            cGraphByte[0]=0xff;   
            cGraphByte[1]=0x01;   
            cGraphByte[2]=0x01;   
            cGraphByte[3]=0x01;   
            cGraphByte[4]=0x01;   
            cGraphByte[5]=0x01;   
            cGraphByte[6]=0x01;   
            cGraphByte[7]=0x01;   
            }   
        else if((cLineTemp1==row1)&&(cLineTemp2==col2))//右上角   
            {   
            cGraphByte[0]=0x01;   
            cGraphByte[1]=0x01;   
            cGraphByte[2]=0x01;   
            cGraphByte[3]=0x01;   
            cGraphByte[4]=0x01;   
            cGraphByte[5]=0x01;   
            cGraphByte[6]=0x01;   
            cGraphByte[7]=0xff;   
            }   
        else if((cLineTemp1==row2)&&(cLineTemp2==col1))//左下角   
            {   
            cGraphByte[0]=0xff;   
            cGraphByte[1]=0x80;   
            cGraphByte[2]=0x80;   
            cGraphByte[3]=0x80;   
            cGraphByte[4]=0x80;   
            cGraphByte[5]=0x80;   
            cGraphByte[6]=0x80;   
            cGraphByte[7]=0x80;   
            }   
        else if((cLineTemp1==row2)&&(cLineTemp2==col2))//右下角   
            {   
            cGraphByte[0]=0x80;   
            cGraphByte[1]=0x80;   
            cGraphByte[2]=0x80;   
            cGraphByte[3]=0x80;   
            cGraphByte[4]=0x80;   
            cGraphByte[5]=0x80;   
            cGraphByte[6]=0x80;   
            cGraphByte[7]=0xff;   
            }   
        else if(((cLineTemp1!=row1)||(cLineTemp1!=row2))&&(cLineTemp2==col1))//左边   
            {   
            cGraphByte[0]=0xff;   
            cGraphByte[1]=0x00;   
            cGraphByte[2]=0x00;   
            cGraphByte[3]=0x00;   
            cGraphByte[4]=0x00;   
            cGraphByte[5]=0x00;   
            cGraphByte[6]=0x00;   
            cGraphByte[7]=0x00;   
            }   
        else if(((cLineTemp1!=row1)||(cLineTemp1!=row2))&&(cLineTemp2==col2))//右边   
            {   
            cGraphByte[0]=0x00;   
            cGraphByte[1]=0x00;   
            cGraphByte[2]=0x00;   
            cGraphByte[3]=0x00;   
            cGraphByte[4]=0x00;   
            cGraphByte[5]=0x00;   
            cGraphByte[6]=0x00;   
            cGraphByte[7]=0xff;   
            }   
        else if((cLineTemp1==row1)&&((cLineTemp2!=col1)||(cLineTemp2!=col2)))//上边   
            {   
            cGraphByte[0]=0x01;   
            cGraphByte[1]=0x01;   
            cGraphByte[2]=0x01;   
            cGraphByte[3]=0x01;   
            cGraphByte[4]=0x01;   
            cGraphByte[5]=0x01;   
            cGraphByte[6]=0x01;   
            cGraphByte[7]=0x01;   
            }   
        else if((cLineTemp1==row2)&&((cLineTemp2!=col1)||(cLineTemp2!=col2)))//下边   
            {   
            cGraphByte[0]=0x80;   
            cGraphByte[1]=0x80;   
            cGraphByte[2]=0x80;   
            cGraphByte[3]=0x80;   
            cGraphByte[4]=0x80;   
            cGraphByte[5]=0x80;   
            cGraphByte[6]=0x80;   
            cGraphByte[7]=0x80;   
            }   
        //   
        DispGraphChar(cLineTemp1,cLineTemp2,&cGraphByte[0],1);   
        }   
    }   
}   
//   
void MenuFunctionNull()   
{   
    _nop_();   
}   
//   
void MenuFunctionDemo1()//   
{   
DispOneBoxTextChar(8,1,'W',0);//   
DispOneBoxTextChar(8,2,'e',0);   
DispOneBoxTextChar(8,3,'l',0);   
DispOneBoxTextChar(8,4,'c',0);   
DispOneBoxTextChar(8,5,'o',0);   
DispOneBoxTextChar(8,6,'m',0);   
DispOneBoxTextChar(8,7,'e',0);   
DispOneBoxTextChar(8,8,' ',0);//   
DispOneBoxTextChar(8,9,'Y',0);   
DispOneBoxTextChar(8,10,'o',0);   
DispOneBoxTextChar(8,11,'u',0);   
DispOneBoxTextChar(8,12,' ',0);   
DispOneBoxTextChar(8,13,'!',0);   
DispOneBoxTextChar(8,14,' ',0);   
}   
//   
void MenuFunctionDemo2()//   
{   
DispOneBoxTextChar(8,1,'>',0);   
DispOneBoxTextChar(8,2,'>',0);   
DispOneBoxTextChar(8,3,'>',0);   
DispOneBoxTextChar(8,4,' ',1);   
DispOneBoxTextChar(8,5,' ',1);   
DispOneBoxTextChar(8,6,' ',1);   
DispOneBoxTextChar(8,7,' ',1);   
DispOneBoxTextChar(8,8,' ',1);   
DispOneBoxTextChar(8,9,' ',1);   
DispOneBoxTextChar(8,10,' ',1);   
DispOneBoxTextChar(8,11,' ',1);   
DispOneBoxTextChar(8,12,' ',1);   
DispOneBoxTextChar(8,13,' ',1);   
DispOneBoxTextChar(8,14,' ',1);   
}   
//   
void MenuFunctionDemo3()//   
{   
DispOneBoxTextChar(8,1,'>',0);   
DispOneBoxTextChar(8,2,'>',0);   
DispOneBoxTextChar(8,3,'>',0);   
DispOneBoxTextChar(8,4,' ',1);   
DispOneBoxTextChar(8,5,' ',1);   
DispOneBoxTextChar(8,6,' ',1);   
DispOneBoxTextChar(8,7,' ',1);   
DispOneBoxTextChar(8,8,' ',1);   
DispOneBoxTextChar(8,9,' ',1);   
DispOneBoxTextChar(8,10,' ',1);   
DispOneBoxTextChar(8,11,' ',1);   
DispOneBoxTextChar(8,12,' ',1);   
DispOneBoxTextChar(8,13,' ',1);   
DispOneBoxTextChar(8,14,' ',1);   
}   
//   
void MenuFunctionDemo4()   
{   
DispOneBoxTextChar(8,1,' ',1);   
DispOneBoxTextChar(8,2,' ',1);   
DispOneBoxTextChar(8,3,' ',1);   
DispOneBoxTextChar(8,4,' ',1);   
DispOneBoxTextChar(8,5,' ',1);   
DispOneBoxTextChar(8,6,' ',1);   
DispOneBoxTextChar(8,7,' ',1);   
DispOneBoxTextChar(8,8,' ',1);   
DispOneBoxTextChar(8,9,' ',1);   
DispOneBoxTextChar(8,10,' ',1);   
DispOneBoxTextChar(8,11,' ',1);   
DispOneBoxTextChar(8,12,'<',0);   
DispOneBoxTextChar(8,13,'<',0);   
DispOneBoxTextChar(8,14,'<',0);   
}   
//   
void MenuFunctionDemo5()//   
{   
DispOneBoxTextChar(8,1,' ',1);   
DispOneBoxTextChar(8,2,' ',1);   
DispOneBoxTextChar(8,3,' ',1);   
DispOneBoxTextChar(8,4,' ',1);   
DispOneBoxTextChar(8,5,' ',1);   
DispOneBoxTextChar(8,6,' ',1);   
DispOneBoxTextChar(8,7,' ',1);   
DispOneBoxTextChar(8,8,' ',1);   
DispOneBoxTextChar(8,9,' ',1);   
DispOneBoxTextChar(8,10,' ',1);   
DispOneBoxTextChar(8,11,' ',1);   
DispOneBoxTextChar(8,12,'<',0);   
DispOneBoxTextChar(8,13,'<',0);   
DispOneBoxTextChar(8,14,'<',0);   
}   
//   
void MenuFunctionDemo6()//   
{   
DispOneBoxTextChar(8,1,'>',0);   
DispOneBoxTextChar(8,2,'>',0);   
DispOneBoxTextChar(8,3,'>',0);   
DispOneBoxTextChar(8,4,' ',1);   
DispOneBoxTextChar(8,5,' ',1);   
DispOneBoxTextChar(8,6,' ',1);   
DispOneBoxTextChar(8,7,' ',1);   
DispOneBoxTextChar(8,8,' ',1);   
DispOneBoxTextChar(8,9,' ',1);   
DispOneBoxTextChar(8,10,' ',1);   
DispOneBoxTextChar(8,11,' ',1);   
DispOneBoxTextChar(8,12,' ',1);   
DispOneBoxTextChar(8,13,' ',1);   
DispOneBoxTextChar(8,14,' ',1);   
}   
//   
void MenuFunctionDemo7()//   
{   
DispOneBoxTextChar(8,1,' ',1);   
DispOneBoxTextChar(8,2,' ',1);   
DispOneBoxTextChar(8,3,' ',1);   
DispOneBoxTextChar(8,4,' ',1);   
DispOneBoxTextChar(8,5,' ',1);   
DispOneBoxTextChar(8,6,' ',1);   
DispOneBoxTextChar(8,7,' ',1);   
DispOneBoxTextChar(8,8,' ',1);   
DispOneBoxTextChar(8,9,' ',1);   
DispOneBoxTextChar(8,10,' ',1);   
DispOneBoxTextChar(8,11,' ',1);   
DispOneBoxTextChar(8,12,'<',0);   
DispOneBoxTextChar(8,13,'<',0);   
DispOneBoxTextChar(8,14,'<',0);   
}   
//   
void MenuFunctionDemo8()//   
{   
DispOneBoxTextChar(8,1,' ',1);   
DispOneBoxTextChar(8,2,' ',1);   
DispOneBoxTextChar(8,3,' ',1);   
DispOneBoxTextChar(8,4,' ',1);   
DispOneBoxTextChar(8,5,' ',1);   
DispOneBoxTextChar(8,6,' ',1);   
DispOneBoxTextChar(8,7,' ',1);   
DispOneBoxTextChar(8,8,' ',1);   
DispOneBoxTextChar(8,9,' ',1);   
DispOneBoxTextChar(8,10,' ',1);   
DispOneBoxTextChar(8,11,' ',1);   
DispOneBoxTextChar(8,12,'<',0);   
DispOneBoxTextChar(8,13,'<',0);   
DispOneBoxTextChar(8,14,'<',0);   
}   
//   
void MenuFunctionDemo9()//   
{   
DispOneBoxTextChar(8,1,' ',1);   
DispOneBoxTextChar(8,2,' ',1);   
DispOneBoxTextChar(8,3,' ',1);   
DispOneBoxTextChar(8,4,' ',1);   
DispOneBoxTextChar(8,5,' ',1);   
DispOneBoxTextChar(8,6,' ',1);   
DispOneBoxTextChar(8,7,' ',1);   
DispOneBoxTextChar(8,8,' ',1);   
DispOneBoxTextChar(8,9,' ',1);   
DispOneBoxTextChar(8,10,' ',1);   
DispOneBoxTextChar(8,11,' ',1);   
DispOneBoxTextChar(8,12,'<',0);   
DispOneBoxTextChar(8,13,'<',0);   
DispOneBoxTextChar(8,14,'<',0);   
}   
//   
void MenuFunctionDemo10()//   
{   
DispOneBoxTextChar(8,1,' ',1);   
DispOneBoxTextChar(8,2,' ',1);   
DispOneBoxTextChar(8,3,' ',1);   
DispOneBoxTextChar(8,4,' ',1);   
DispOneBoxTextChar(8,5,' ',1);   
DispOneBoxTextChar(8,6,' ',1);   
DispOneBoxTextChar(8,7,' ',1);   
DispOneBoxTextChar(8,8,' ',1);   
DispOneBoxTextChar(8,9,' ',1);   
DispOneBoxTextChar(8,10,' ',1);   
DispOneBoxTextChar(8,11,' ',1);   
DispOneBoxTextChar(8,12,'<',0);   
DispOneBoxTextChar(8,13,'<',0);   
DispOneBoxTextChar(8,14,'<',0);   
}   
//   
void MenuFunctionDemo11()//   
{   
DispOneBoxTextChar(8,1,' ',1);   
DispOneBoxTextChar(8,2,' ',1);   
DispOneBoxTextChar(8,3,' ',1);   
DispOneBoxTextChar(8,4,' ',1);   
DispOneBoxTextChar(8,5,' ',1);   
DispOneBoxTextChar(8,6,' ',1);   
DispOneBoxTextChar(8,7,' ',1);   
DispOneBoxTextChar(8,8,' ',1);   
DispOneBoxTextChar(8,9,' ',1);   
DispOneBoxTextChar(8,10,' ',1);   
DispOneBoxTextChar(8,11,' ',1);   
DispOneBoxTextChar(8,12,'<',0);   
DispOneBoxTextChar(8,13,'<',0);   
DispOneBoxTextChar(8,14,'<',0);   
}   
//   
void MenuFunctionDemo12()//   
{   
DispOneBoxTextChar(8,1,'>',0);   
DispOneBoxTextChar(8,2,'>',0);   
DispOneBoxTextChar(8,3,'>',0);   
DispOneBoxTextChar(8,4,' ',1);   
DispOneBoxTextChar(8,5,' ',1);   
DispOneBoxTextChar(8,6,' ',1);   
DispOneBoxTextChar(8,7,' ',1);   
DispOneBoxTextChar(8,8,' ',1);   
DispOneBoxTextChar(8,9,' ',1);   
DispOneBoxTextChar(8,10,' ',1);   
DispOneBoxTextChar(8,11,' ',1);   
DispOneBoxTextChar(8,12,' ',1);   
DispOneBoxTextChar(8,13,' ',1);   
DispOneBoxTextChar(8,14,' ',1);   
}   
//   
void MenuFunctionDemo13()//   
{   
DispOneBoxTextChar(8,1,' ',1);   
DispOneBoxTextChar(8,2,' ',1);   
DispOneBoxTextChar(8,3,' ',1);   
DispOneBoxTextChar(8,4,' ',1);   
DispOneBoxTextChar(8,5,' ',1);   
DispOneBoxTextChar(8,6,' ',1);   
DispOneBoxTextChar(8,7,' ',1);   
DispOneBoxTextChar(8,8,' ',1);   
DispOneBoxTextChar(8,9,' ',1);   
DispOneBoxTextChar(8,10,' ',1);   
DispOneBoxTextChar(8,11,' ',1);   
DispOneBoxTextChar(8,12,'<',0);   
DispOneBoxTextChar(8,13,'<',0);   
DispOneBoxTextChar(8,14,'<',0);   
}   
//   
void MenuFunctionDemo14()//   
{   
DispOneBoxTextChar(8,1,' ',1);   
DispOneBoxTextChar(8,2,' ',1);   
DispOneBoxTextChar(8,3,' ',1);   
DispOneBoxTextChar(8,4,' ',1);   
DispOneBoxTextChar(8,5,' ',1);   
DispOneBoxTextChar(8,6,' ',1);   
DispOneBoxTextChar(8,7,' ',1);   
DispOneBoxTextChar(8,8,' ',1);   
DispOneBoxTextChar(8,9,' ',1);   
DispOneBoxTextChar(8,10,' ',1);   
DispOneBoxTextChar(8,11,' ',1);   
DispOneBoxTextChar(8,12,'<',0);   
DispOneBoxTextChar(8,13,'<',0);   
DispOneBoxTextChar(8,14,'<',0);   
}   
//   
void MenuFunctionDemo15()//   
{   
DispOneBoxTextChar(8,1,'>',0);   
DispOneBoxTextChar(8,2,'>',0);   
DispOneBoxTextChar(8,3,'>',0);   
DispOneBoxTextChar(8,4,' ',1);   
DispOneBoxTextChar(8,5,' ',1);   
DispOneBoxTextChar(8,6,' ',1);   
DispOneBoxTextChar(8,7,' ',1);   
DispOneBoxTextChar(8,8,' ',1);   
DispOneBoxTextChar(8,9,' ',1);   
DispOneBoxTextChar(8,10,' ',1);   
DispOneBoxTextChar(8,11,' ',1);   
DispOneBoxTextChar(8,12,' ',1);   
DispOneBoxTextChar(8,13,' ',1);   
DispOneBoxTextChar(8,14,' ',1);   
}   
//   
void MenuFunctionDemo16()//   
{   
DispOneBoxTextChar(8,1,'>',0);   
DispOneBoxTextChar(8,2,'>',0);   
DispOneBoxTextChar(8,3,'>',0);   
DispOneBoxTextChar(8,4,' ',1);   
DispOneBoxTextChar(8,5,' ',1);   
DispOneBoxTextChar(8,6,' ',1);   
DispOneBoxTextChar(8,7,' ',1);   
DispOneBoxTextChar(8,8,' ',1);   
DispOneBoxTextChar(8,9,' ',1);   
DispOneBoxTextChar(8,10,' ',1);   
DispOneBoxTextChar(8,11,' ',1);   
DispOneBoxTextChar(8,12,' ',1);   
DispOneBoxTextChar(8,13,' ',1);   
DispOneBoxTextChar(8,14,' ',1);   
}   
//   
void MenuFunctionDemo17()//   
{   
DispOneBoxTextChar(8,1,' ',1);   
DispOneBoxTextChar(8,2,' ',1);   
DispOneBoxTextChar(8,3,' ',1);   
DispOneBoxTextChar(8,4,' ',1);   
DispOneBoxTextChar(8,5,' ',1);   
DispOneBoxTextChar(8,6,' ',1);   
DispOneBoxTextChar(8,7,' ',1);   
DispOneBoxTextChar(8,8,' ',1);   
DispOneBoxTextChar(8,9,' ',1);   
DispOneBoxTextChar(8,10,' ',1);   
DispOneBoxTextChar(8,11,' ',1);   
DispOneBoxTextChar(8,12,'<',0);   
DispOneBoxTextChar(8,13,'<',0);   
DispOneBoxTextChar(8,14,'<',0);   
}   
//   
void MenuFunctionDemo18()//   
{   
DispOneBoxTextChar(8,1,' ',1);   
DispOneBoxTextChar(8,2,' ',1);   
DispOneBoxTextChar(8,3,' ',1);   
DispOneBoxTextChar(8,4,' ',1);   
DispOneBoxTextChar(8,5,' ',1);   
DispOneBoxTextChar(8,6,' ',1);   
DispOneBoxTextChar(8,7,' ',1);   
DispOneBoxTextChar(8,8,' ',1);   
DispOneBoxTextChar(8,9,' ',1);   
DispOneBoxTextChar(8,10,' ',1);   
DispOneBoxTextChar(8,11,' ',1);   
DispOneBoxTextChar(8,12,'<',0);   
DispOneBoxTextChar(8,13,'<',0);   
DispOneBoxTextChar(8,14,'<',0);   
}   
//   
void MenuFunctionDemo19()//   
{   
DispOneBoxTextChar(8,1,' ',1);   
DispOneBoxTextChar(8,2,' ',1);   
DispOneBoxTextChar(8,3,' ',1);   
DispOneBoxTextChar(8,4,' ',1);   
DispOneBoxTextChar(8,5,' ',1);   
DispOneBoxTextChar(8,6,' ',1);   
DispOneBoxTextChar(8,7,' ',1);   
DispOneBoxTextChar(8,8,' ',1);   
DispOneBoxTextChar(8,9,' ',1);   
DispOneBoxTextChar(8,10,' ',1);   
DispOneBoxTextChar(8,11,' ',1);   
DispOneBoxTextChar(8,12,'<',0);   
DispOneBoxTextChar(8,13,'<',0);   
DispOneBoxTextChar(8,14,'<',0);   
}   
//   
//   
void JudgeHLight()   
{   
switch(iMenuNextID)   
    {   
    case    MENU_FACE:   
            DispFaceBmp();   
            break;   
    case    MENU_ROOT:   
            DispFaceBmp();   
            break;   
    case    MENU_VIEW:   
            DispMenu_Root();   
            break;   
    case    MENU_VIEW_ABOUTRECORDE:   
    case    MENU_VIEW_HANDSETCLOCK:   
            DispMenu_View();   
            break;   
    case    MENU_OPERATION:   
            DispMenu_Root();   
            break;   
    case    MENU_OPERATION_SETPOSITION:   
    case    MENU_OPERATION_READZJTOFLASH:   
    case    MENU_OPERATION_RDZJTORAM:   
    case    MENU_OPERATION_DELGIVENZL:   
    case    MENU_OPERATION_DELALLZJ:   
            DispMenu_Operation();   
            break;   
    case    MENU_TEST:   
            DispMenu_Root();   
            break;   
    case    MENU_TEST_RAMTEST:   
    case    MENU_TEST_FLASHTEST:   
            DispMenu_Test();   
            break;   
    case    MENU_SYSTEMSET:   
            DispMenu_Root();   
            break;   
    case    MENU_SYSTEMSET_DEBUG:   
            DispMenu_SystemSet();   
            break;   
    case    MENU_SYSTEMSET_DEBUG_PASSWORD:   
    case    MENU_SYSTEMSET_DEBUG_SEEPICTURE:   
            DispMenu_Debug();   
            break;   
    case    MENU_SYSTEMSET_SETHANDCLK:   
            DispMenu_SystemSet();   
            break;   
    default:   
            break;   
    }   
   
}   
//   
void ShowDesktopTime()   
{   
        DispTwoBoxHexChar(1,7,cClock_Year,S_ZS|1);   
        DispOneBoxTextChar(1,9,'/',S_S+1);   
        DispTwoBoxHexChar(1,10,cClock_Month,S_S|1);   
        DispOneBoxTextChar(1,12,'/',S_S+1);   
        DispTwoBoxHexChar(1,13,cClock_Day,S_YS|1);   
        //   
        DispTwoBoxHexChar(2,7,cClock_Hour,S_ZX|1);   
        DispOneBoxTextChar(2,9,':',S_X+1);   
        DispTwoBoxHexChar(2,10,cClock_Minute,S_X|1);   
        DispOneBoxTextChar(2,12,':',S_X+1);   
        DispTwoBoxHexChar(2,13,cClock_Second,S_YX|1);   
}   
//   
void DispOneBoxTextChar(unsigned char Row,unsigned char Col,unsigned char c1,unsigned char DispAttrib)   
{   
    i_DisponeTextChar=0xb0+Row-1;   
    lcd_write_c(i_DisponeTextChar);//set page 0   
    //   
    i_DisponeTextChar=(0x00+Col)*8+2;   
    j_DisponeTextChar=i_DisponeTextChar&0x0f;   
    lcd_write_c(j_DisponeTextChar);//Set Lower Column Address   
    //   
    j_DisponeTextChar=((i_DisponeTextChar&0xf0)>>4)+0x10;   
    lcd_write_c(j_DisponeTextChar);//Set Higher Column Address   
    //   
    lcd_write_c(0x40);//Set Display Start Line   
    //   
    j_DisponeTextChar=c1-0x20;   
    //   
    for(i_DisponeTextChar=0;i_DisponeTextChar<8;i_DisponeTextChar++)   
    {   
        if((DispAttrib&0x01)==1)   
        {   
            k_DisponeTextChar=Echar[j_DisponeTextChar][i_DisponeTextChar];   
        }   
        else   
        {   
            k_DisponeTextChar=Echar[j_DisponeTextChar][i_DisponeTextChar]^0xff;   
        }   
        if((DispAttrib&0x80)>0)//上   
            {   
            k_DisponeTextChar|=0x01;   
            }   
        if((DispAttrib&0x40)>0)//下   
            {   
            k_DisponeTextChar|=0x80;   
            }   
        if((i_DisponeTextChar==0)&&((DispAttrib&0x20)>0))//左   
            {   
            k_DisponeTextChar|=0xff;   
            }   
        if( (i_DisponeTextChar==7) && ((DispAttrib&0x10)>0) )//右   
            {   
            k_DisponeTextChar|=0xff;   
            }   
        lcd_write_d(k_DisponeTextChar);   
    }   
}   
//   
void DispTwoBoxHexChar(unsigned char Row,unsigned char Col,unsigned char c1,unsigned char DispAttrib)   
{   
    i_DisponeTextChar=0xb0+Row-1;   
    lcd_write_c(i_DisponeTextChar);//set page 0   
    //   
    i_DisponeTextChar=(0x00+Col)*8+2;   
    j_DisponeTextChar=i_DisponeTextChar&0x0f;   
    lcd_write_c(j_DisponeTextChar);//Set Lower Column Address   
    //   
    j_DisponeTextChar=((i_DisponeTextChar&0xf0)>>4)+0x10;   
    lcd_write_c(j_DisponeTextChar);//Set Higher Column Address   
    //   
    lcd_write_c(0x40);//Set Display Start Line   
    //   
    cTemp_DisponeTextChar=(c1>>4)&0x0f;   
    if((cTemp_DisponeTextChar>=0)&&(cTemp_DisponeTextChar<=9))   
    {   
    cTemp_DisponeTextChar+=16;   
    }   
    else   
        if((cTemp_DisponeTextChar>=0x0a)&&(cTemp_DisponeTextChar<=0x0f))   
        {   
        cTemp_DisponeTextChar+=23;   
        }   
    //   
    for(i_DisponeTextChar=0;i_DisponeTextChar<8;i_DisponeTextChar++)   
    {   
        if((DispAttrib&0x01)==1)   
        {   
            k_DisponeTextChar=Echar[cTemp_DisponeTextChar][i_DisponeTextChar];   
        }   
        else   
        {   
            k_DisponeTextChar=Echar[cTemp_DisponeTextChar][i_DisponeTextChar]^0xff;   
        }   
        if((DispAttrib&0x80)>0)//上   
            {   
            k_DisponeTextChar|=0x01;   
            }   
        if((DispAttrib&0x40)>0)//下   
            {   
            k_DisponeTextChar|=0x80;   
            }   
        if((i_DisponeTextChar==0)&&((DispAttrib&0x20)>0))//左   
            {   
            k_DisponeTextChar|=0xff;   
            }   
        lcd_write_d(k_DisponeTextChar);   
    }   
    //   
    cTemp_DisponeTextChar=c1&0x0f;   
    if((cTemp_DisponeTextChar>=0)&&(cTemp_DisponeTextChar<=9))   
    {   
    cTemp_DisponeTextChar+=16;   
    }   
    else   
        if((cTemp_DisponeTextChar>=0x0a)&&(cTemp_DisponeTextChar<=0x0f))   
        {   
        cTemp_DisponeTextChar+=23;   
        }   
    //   
    for(i_DisponeTextChar=0;i_DisponeTextChar<8;i_DisponeTextChar++)   
    {   
        if((DispAttrib&0x01)==1)   
        {   
            k_DisponeTextChar=Echar[cTemp_DisponeTextChar][i_DisponeTextChar];   
        }   
        else   
        {   
            k_DisponeTextChar=Echar[cTemp_DisponeTextChar][i_DisponeTextChar]^0xff;   
        }   
        if((DispAttrib&0x80)>0)//上   
            {   
            k_DisponeTextChar|=0x01;   
            }   
        if((DispAttrib&0x40)>0)//下   
            {   
            k_DisponeTextChar|=0x80;   
            }   
        if( (i_DisponeTextChar==7) && ((DispAttrib&0x10)>0) )//右   
            {   
            k_DisponeTextChar|=0xff;   
            }   
        lcd_write_d(k_DisponeTextChar);   
    }   
}   
//   
//   
//   
//   
//   
//   
//   
//   
//   
//   
//   
//   
//   
//   
//   
//   
//   
//   
//   
//   
//   
//   
//   
//   
//   
//   
//   
//   
//   
//======================================================================   
//#                            lcd_write_c                             #   
//======================================================================   
void lcd_write_c(unsigned char c1)   
{   
    EA=0;   
    LCD_A0=0;//Command   
    LCD_WR=0;   
    LCD_CS=0;   
    //   
    P2=c1;P2=c1;   
    P2=c1;P2=c1;   
    P2=c1;P2=c1;   
    //   
    LCD_CS=1;   
    LCD_WR=1;   
    LCD_A0=1;//Data Byte   
    EA=1;   
}   
//======================================================================   
//#                            lcd_write_d                             #   
//======================================================================   
   
void lcd_write_d(unsigned char c1)   
{   
    EA=0;   
    LCD_A0=1;//Data Byte   
    LCD_WR=0;   
    LCD_CS=0;   
    //   
    P2=c1;P2=c1;   
    P2=c1;P2=c1;   
    P2=c1;P2=c1;   
    //   
    LCD_CS=1;   
    LCD_WR=1;   
    EA=1;   
}   
//   
void DispFaceBmp()   
{   
    //Disp The First Face Bmp   
    c1_FaceBmp=0;   
    i_FaceBmp=0;   
    j_FaceBmp=0;   
    k_FaceBmp=0;   
    //   
    lcd_write_c(0xb0+c1_FaceBmp);//set page 0   
    lcd_write_c(0x40);//Set Display Start Line   
    lcd_write_c(0x0a);//Set Lower Column Address   
    lcd_write_c(0x10);//Set Higher Column Address   
   
    //   
    for(i_FaceBmp=0;i_FaceBmp<112;i_FaceBmp++)   
    {   
        //   
        for(j_FaceBmp=0;j_FaceBmp<8;j_FaceBmp++)   
        {   
            //   
            lcd_write_d(FaceBmp[i_FaceBmp][j_FaceBmp]);//$>90   
            if(++k_FaceBmp>111)   
            {   
                k_FaceBmp=0;   
                c1_FaceBmp++;   
                lcd_write_c(0xb0+c1_FaceBmp);//set page 0   
                lcd_write_c(0x40);//Set Display Start Line   
                lcd_write_c(0x0a);//Set Lower Column Address   
                lcd_write_c(0x10);//Set Higher Column Address   
            }   
        }   
        //   
    }   
    DispOneBoxTextChar(4,7,'V',1);   
    DispOneBoxTextChar(4,8,'1',1);   
    DispOneBoxTextChar(4,9,'.',1);   
    DispOneBoxTextChar(4,10,'0',1);   
    DispOneBoxTextChar(4,11,'2',1);   
    DispOneBoxTextChar(4,12,'3',1);   
}   
//   
void LcdInit()   
{   
    LCD_RESET=0;//Hard Reset   
    DelayMs(10);//LCD_RESET=0;LCD_RESET=0;   
    LCD_RESET=1;   
    DelayMs(5);   
    lcd_write_c(0xe2);//15.Software Reset   
    lcd_write_c(0xa2);//8.Set LCD Bias   
    lcd_write_c(0xa9);//20.Set Bias Ratio,TC Value,Osc.Freq   
    lcd_write_c(0xca);//20-1.The Data Byte=8 Bits   
    lcd_write_c(0xa0);//7.Set Segment Re_map   
    lcd_write_c(0xc8);//13.Set Com OutputScan Direction   
    lcd_write_c(0x2f);//4.Set Power Control Register   
    lcd_write_c(0xa4);//9.Set Entire Display On/Off   
    lcd_write_c(0x24);//3.Set Internal Regulator Resistor Ratio   
    lcd_write_c(0x81);//6.Set Contrast Control Register   
    lcd_write_c(40);//6->1.value=0x00+value&0x3f,   
    lcd_write_c(0xa6);//10.Set Normal/Reverse Display   
    lcd_write_c(0xaf);//11.Set Display On/Off   
    lcd_write_c(0xb0);//12.Set Page Address   
}   
//   
void MyInit()   
{   
    //   
    TMOD=0x21;   
    //0010,0001B(GATE,C/T\,M1,M0,GATE,C/T\,M1,M0).   
    //T1,T0都不受/INT0和/INT1引脚控制,   
    //T1,T0都为定时器方式   
    //定时器1(方式2:自动重装的8位定时方式)用作波特率发生器.   
    //定时器0(方式1:作为16位定时器)   
    TF1=0;//清定时器1溢出位   
    TF0=0;//清定时器0溢出位   
    TL0=TL0data;   
    TH0=TH0data;   
    TL1=0xfd;   
    TH1=0xfd;   
    SM0=0;//8位UART,波特率可变.   
    SM1=1;//   
    SM2=0;//单机通讯   
    REN=1;//允许接收   
    TB8=1;//发送的第9位数据位=1   
    RB8=1;//接收到的仃止位为1**********   
    TI=0;//清串口发送请求   
    RI=0;//清串口接收请求   
    PCON=0;//SMOD(PCON.7)=0,波特率为振荡频率的1/64,且与T1有关   
    IP=0x10;//0001,000B 串行中断优先   
    //   
    IT0=1;//外中断0为下降沿触发   
    IE0=0;//清外中断0请求   
    //   
    IT1=1;//外中断1为下降沿触发   
    IE1=0;//清外中断1请求   
    //   
    PX0=0;//INT0中断优先级=低   
    PT0=0;//T0中断优先级=低   
    PX1=0;//INT1中断优先级=低   
    PT1=0;//T1中断优先级=低   
    PS=0;//串行中断优先级=低   
    //   
    EX0=1;//Enable Interrupt Ex0   
    ET0=1;//Enable Interrupt T0   
    EX1=0;//Disable Interrupt Ex1   
    ET1=0;//Disable Interrupt T1   
    ES=1;// Enable Interrupt Communication   
    EA=1;//Enable Interrupt All   
    TR1=1;//Run Time 1   
    TR0=1;//Run Time 0   
    //   
}   
//   
void DispGraphChar(unsigned char Row,unsigned char Col,unsigned char xdata *cArray,unsigned char DispAttrib)   
{   
    //unsigned char data * xdata p;   
    cGraphTemp1=0xb0+Row-1;   
    lcd_write_c(cGraphTemp1);//set page   
    //   
    cGraphTemp1=(0x00+Col)*8+2;   
    cGraphTemp2=cGraphTemp1&0x0f;   
    lcd_write_c(cGraphTemp2);//Set Lower Column Address   
    //   
    cGraphTemp2=((cGraphTemp1&0xf0)>>4)+0x10;   
    lcd_write_c(cGraphTemp2);//Set Higher Column Address   
    //   
    lcd_write_c(0x40);//Set Display Start Line   
    //   
    cpGraph=cArray;   
    //   
    for(cGraphTemp1=0;cGraphTemp1<8;cGraphTemp1++)   
        {   
        if(DispAttrib==1)   
            {   
            cGraphTemp2=*cpGraph;   
            }   
        else   
            {   
            cGraphTemp2=(*cpGraph)&0xff;   
            }   
        lcd_write_d(cGraphTemp2);   
        cpGraph++;   
        }   
}   
//   
void ReadyForKey()   
    {   
        KEYROW1=0;   
        KEYROW2=0;   
        KEYROW3=0;   
        //KEYINT=1;   
    }   
//   
void DelayMs(unsigned int iMs)   
{   
unsigned int  data i_DelayMs;   
unsigned char data j_DelayMs;   
   
    for(i_DelayMs=0;i_DelayMs<iMs;i_DelayMs++)   
    {   
        for(j_DelayMs=0;j_DelayMs<224;j_DelayMs++)   
        {   
            _nop_();   
        }   
    }   
}//   
   
//Next Function   
unsigned char code Echar[96][8]={   
    //  (0) !(1) "(2) #(3) $(4) %(5) &(6) '(7)   
    // ((8) )(9) *(10) +(11) ,(12) -(13) .(14) /(15)   
    // 0(16) 1(17) 2(18) 3(19) 4(20) 5(21) 6(22) 7(23)   
    // 8(24) 9(25) :(26) ;(27) <(28) =(29) >(30) ?(31)   
    // @(32) A(33) B(34) C(35) D(36) E(37) F(38) G(39)   
    // H(40) I(41) J(42) K(43) L(44) M(45) N(46) O(47)   
    // P(48) Q(49) R(50) S(51) T(52) U(53) V(54) W(55)   
    // X(56) Y(57) Z(58) [(59) \(60) ](61) ^(62) _(63)   
    // `(64) a(65) b(66) c(67) d(68) e(69) f(70) g(71)   
    // h(72) i(73) j(74) k(75) l(76) m(77) n(78) o(79)   
    // p(80) q(81) r(82) s(83) t(84) u(85) v(86) w(87)   
    // x(88) y(89) z(90) {(91) |(92) }(93) ~(94) (95)   
   
   
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*" ",0*/   
   
    {0x00,0x00,0x06,0x5F,0x5F,0x06,0x00,0x00},/*"!",1*/   
   
    {0x00,0x00,0x07,0x07,0x00,0x07,0x07,0x00},/*""",2*/   
   
    {0x00,0x14,0x7F,0x7F,0x14,0x7F,0x7F,0x14},/*"#",3*/   
   
    {0x00,0x24,0x2E,0x6B,0x6B,0x3A,0x12,0x00},/*"$",4*/   
   
    {0x00,0x46,0x66,0x30,0x18,0x0C,0x66,0x62},/*"%",5*/   
   
    {0x00,0x30,0x7A,0x4F,0x5D,0x37,0x7A,0x48},/*"&",6*/   
   
    {0x00,0x04,0x07,0x03,0x00,0x00,0x00,0x00},/*"'",7*/   
   
    {0x00,0x00,0x1C,0x3E,0x63,0x41,0x00,0x00},/*"(",8*/   
   
    {0x00,0x00,0x41,0x63,0x3E,0x1C,0x00,0x00},/*")",9*/   
   
    {0x00,0x08,0x2A,0x3E,0x1C,0x1C,0x3E,0x2A},/*"*",10*/   
   
    {0x00,0x08,0x08,0x3E,0x3E,0x08,0x08,0x00},/*"+",11*/   
   
    {0x00,0x00,0xA0,0xE0,0x60,0x00,0x00,0x00},/*",",12*/   
   
    {0x00,0x08,0x08,0x08,0x08,0x08,0x08,0x00},/*"-",13*/   
   
    {0x00,0x00,0x00,0x60,0x60,0x00,0x00,0x00},/*".",14*/   
   
    {0x00,0x60,0x30,0x18,0x0C,0x06,0x03,0x01},/*"/",15*/   
   
    {0x00,0x3E,0x7F,0x59,0x4D,0x7F,0x3E,0x00},/*"0",16*/   
   
    {0x00,0x42,0x42,0x7F,0x7F,0x40,0x40,0x00},/*"1",17*/   
   
    {0x00,0x62,0x73,0x59,0x49,0x6F,0x66,0x00},/*"2",18*/   
   
    {0x00,0x22,0x63,0x49,0x49,0x7F,0x36,0x00},/*"3",19*/   
   
    {0x00,0x18,0x1C,0x16,0x13,0x7F,0x7F,0x10},/*"4",20*/   
   
    {0x00,0x27,0x67,0x45,0x45,0x7D,0x39,0x00},/*"5",21*/   
   
    {0x00,0x3C,0x7E,0x4B,0x49,0x79,0x30,0x00},/*"6",22*/   
   
    {0x00,0x03,0x63,0x71,0x19,0x0F,0x07,0x00},/*"7",23*/   
   
    {0x00,0x36,0x7F,0x49,0x49,0x7F,0x36,0x00},/*"8",24*/   
   
    {0x00,0x06,0x4F,0x49,0x69,0x3F,0x1E,0x00},/*"9",25*/   
   
    {0x00,0x00,0x00,0x6C,0x6C,0x00,0x00,0x00},/*":",26*/   
   
    {0x00,0x00,0xA0,0xEC,0x6C,0x00,0x00,0x00},/*";",27*/   
   
    {0x00,0x08,0x1C,0x36,0x63,0x41,0x00,0x00},/*"<",28*/   
   
    {0x00,0x14,0x14,0x14,0x14,0x14,0x14,0x00},/*"=",29*/   
   
    {0x00,0x00,0x41,0x63,0x36,0x1C,0x08,0x00},/*">",30*/   
   
    {0x00,0x02,0x03,0x51,0x59,0x0F,0x06,0x00},/*"?",31*/   
   
    {0x00,0x3E,0x7F,0x41,0x5D,0x5D,0x1F,0x1E},/*"@",32*/   
   
    {0x00,0x7C,0x7E,0x13,0x13,0x7E,0x7C,0x00},/*"A",33*/   
   
    {0x00,0x41,0x7F,0x7F,0x49,0x49,0x7F,0x36},/*"B",34*/   
   
    {0x00,0x1C,0x3E,0x63,0x41,0x41,0x63,0x22},/*"C",35*/   
   
    {0x00,0x41,0x7F,0x7F,0x41,0x63,0x7F,0x1C},/*"D",36*/   
   
    {0x00,0x41,0x7F,0x7F,0x49,0x5D,0x41,0x63},/*"E",37*/   
   
    {0x00,0x41,0x7F,0x7F,0x49,0x1D,0x01,0x03},/*"F",38*/   
   
    {0x00,0x1C,0x3E,0x63,0x41,0x51,0x73,0x72},/*"G",39*/   
   
    {0x00,0x7F,0x7F,0x08,0x08,0x7F,0x7F,0x00},/*"H",40*/   
   
    {0x00,0x00,0x41,0x7F,0x7F,0x41,0x00,0x00},/*"I",41*/   
   
    {0x00,0x30,0x70,0x40,0x41,0x7F,0x3F,0x01},/*"J",42*/   
   
    {0x00,0x41,0x7F,0x7F,0x08,0x1C,0x77,0x63},/*"K",43*/   
   
    {0x00,0x41,0x7F,0x7F,0x41,0x40,0x60,0x70},/*"L",44*/   
   
    {0x00,0x7F,0x7F,0x06,0x0C,0x06,0x7F,0x7F},/*"M",45*/   
   
    {0x00,0x7F,0x7F,0x06,0x0C,0x18,0x7F,0x7F},/*"N",46*/   
   
    {0x00,0x1C,0x3E,0x63,0x41,0x63,0x3E,0x1C},/*"O",47*/   
   
    {0x00,0x41,0x7F,0x7F,0x49,0x09,0x0F,0x06},/*"P",48*/   
   
    {0x00,0x1E,0x3F,0x21,0x71,0x7F,0x5E,0x00},/*"Q",49*/   
   
    {0x00,0x41,0x7F,0x7F,0x19,0x39,0x6F,0x46},/*"R",50*/   
   
    {0x00,0x26,0x67,0x4D,0x59,0x7B,0x32,0x00},/*"S",51*/   
   
    {0x00,0x03,0x41,0x7F,0x7F,0x41,0x03,0x00},/*"T",52*/   
   
    {0x00,0x7F,0x7F,0x40,0x40,0x7F,0x7F,0x00},/*"U",53*/   
   
    {0x00,0x1F,0x3F,0x60,0x60,0x3F,0x1F,0x00},/*"V",54*/   
   
    {0x00,0x7F,0x7F,0x30,0x18,0x30,0x7F,0x7F},/*"W",55*/   
   
    {0x00,0x63,0x77,0x1C,0x08,0x1C,0x77,0x63},/*"X",56*/   
   
    {0x00,0x07,0x4F,0x78,0x78,0x4F,0x07,0x00},/*"Y",57*/   
   
    {0x00,0x67,0x73,0x59,0x4D,0x47,0x63,0x71},/*"Z",58*/   
   
    {0x00,0x00,0x7F,0x7F,0x41,0x41,0x00,0x00},/*"[",59*/   
   
    {0x00,0x01,0x03,0x06,0x0C,0x18,0x30,0x60},/*"\",60*/   
   
    {0x00,0x00,0x41,0x41,0x7F,0x7F,0x00,0x00},/*"]",61*/   
   
    {0x00,0x08,0x0C,0x06,0x03,0x06,0x0C,0x08},/*"^",62*/   
   
    {0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80},/*"_",63*/   
   
    {0x00,0x00,0x00,0x03,0x07,0x04,0x00,0x00},/*"`",64*/   
   
    {0x00,0x20,0x74,0x54,0x54,0x3C,0x78,0x40},/*"a",65*/   
   
    {0x00,0x41,0x3F,0x7F,0x44,0x44,0x7C,0x38},/*"b",66*/   
   
    {0x00,0x38,0x7C,0x44,0x44,0x6C,0x28,0x00},/*"c",67*/   
   
    {0x00,0x30,0x78,0x48,0x49,0x3F,0x7F,0x40},/*"d",68*/   
   
    {0x00,0x38,0x7C,0x54,0x54,0x5C,0x18,0x00},/*"e",69*/   
   
    {0x00,0x48,0x7E,0x7F,0x49,0x03,0x02,0x00},/*"f",70*/   
   
    {0x00,0x98,0xBC,0xA4,0xA4,0xF8,0x7C,0x04},/*"g",71*/   
   
    {0x00,0x41,0x7F,0x7F,0x08,0x04,0x7C,0x78},/*"h",72*/   
   
    {0x00,0x00,0x44,0x7D,0x7D,0x40,0x00,0x00},/*"i",73*/   
   
    {0x00,0x40,0xC4,0x84,0xFD,0x7D,0x00,0x00},/*"j",74*/   
   
    {0x00,0x41,0x7F,0x7F,0x10,0x38,0x6C,0x44},/*"k",75*/   
   
    {0x00,0x00,0x41,0x7F,0x7F,0x40,0x00,0x00},/*"l",76*/   
   
    {0x00,0x7C,0x7C,0x0C,0x18,0x0C,0x7C,0x78},/*"m",77*/   
   
    {0x00,0x7C,0x7C,0x04,0x04,0x7C,0x78,0x00},/*"n",78*/   
   
    {0x00,0x38,0x7C,0x44,0x44,0x7C,0x38,0x00},/*"o",79*/   
   
    {0x00,0x84,0xFC,0xF8,0xA4,0x24,0x3C,0x18},/*"p",80*/   
   
    {0x00,0x18,0x3C,0x24,0xA4,0xF8,0xFC,0x84},/*"q",81*/   
   
    {0x00,0x44,0x7C,0x78,0x44,0x1C,0x18,0x00},/*"r",82*/   
   
    {0x00,0x48,0x5C,0x54,0x54,0x74,0x24,0x00},/*"s",83*/   
   
    {0x00,0x00,0x04,0x3E,0x7F,0x44,0x24,0x00},/*"t",84*/   
   
    {0x00,0x3C,0x7C,0x40,0x40,0x3C,0x7C,0x40},/*"u",85*/   
   
    {0x00,0x1C,0x3C,0x60,0x60,0x3C,0x1C,0x00},/*"v",86*/   
   
    {0x00,0x3C,0x7C,0x60,0x30,0x60,0x7C,0x3C},/*"w",87*/   
   
    {0x00,0x44,0x6C,0x38,0x10,0x38,0x6C,0x44},/*"x",88*/   
   
    {0x00,0x9C,0xBC,0xA0,0xA0,0xFC,0x7C,0x00},/*"y",89*/   
   
    {0x00,0x4C,0x64,0x74,0x5C,0x4C,0x64,0x00},/*"z",90*/   
   
    {0x00,0x08,0x08,0x3E,0x77,0x41,0x41,0x00},/*"{",91*/   
   
    {0x00,0x00,0x00,0x00,0x77,0x77,0x00,0x00},/*"|",92*/   
   
    {0x00,0x41,0x41,0x77,0x3E,0x08,0x08,0x00},/*"}",93*/   
   
    {0x00,0x02,0x03,0x01,0x03,0x02,0x03,0x01},/*"~",94*/   
   
    {0x00,0x78,0x7C,0x46,0x43,0x46,0x7C,0x78}/*"",95*/   
   
};   
unsigned char code FaceBmp[112][8]={   
//     E:\CHENXUAN\Cx2004\Project\1MyHandSet\Picture\测试工装.bmp(0)   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x80,0xC0,0xE0,0xF0,0xF8,0xF8,0xF8,0xF0},   
{0x60,0xC0,0x80,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x80,0x80,0xE0,0xF0,0xF8,0xFC,0xFE,0xFF},   
{0xFF,0x3F,0x1F,0x0F,0xF7,0xF3,0xF3,0xE7},   
{0x0E,0x1C,0x39,0xF3,0xE6,0x0C,0x18,0xB8},   
{0xFC,0xFE,0x73,0x3F,0x1E,0x0C,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x0F},   
{0x1B,0x3B,0x1F,0x0F,0x07,0x07,0x0F,0x1F},   
{0x3F,0x7F,0xFE,0xFC,0xF9,0xF1,0xF1,0xF9},   
{0xCC,0xE6,0x73,0x39,0x1C,0x0E,0x07,0x03},   
{0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x01,0x03,0x07,0x07,0x03},   
{0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0xC0,0xC0,0x80,0x00,0x80,0x80,0x80,0x80},   
{0x80,0x80,0x00,0x00,0x00,0xC0,0xC0,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0xC0,0xC0,0x80,0x80,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80},   
{0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x80,0x80,0x00,0xC0,0xC0,0x00,0x00},   
{0x00,0x00,0xC0,0xC0,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C},   
{0x3C,0xBB,0xF3,0xF0,0xFF,0xFF,0xFD,0xFD},   
{0xFF,0xFF,0x00,0xFE,0xFE,0xFF,0xFF,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x60,0x61,0x63},   
{0xE7,0xE7,0x0C,0x6C,0x6C,0xEC,0xEC,0x6C},   
{0xFF,0xFF,0x0D,0x0F,0x0F,0x0C,0x00,0x00},   
{0x00,0x00,0x00,0x03,0x03,0x03,0x03,0x03},   
{0x03,0xFF,0xFF,0x03,0x03,0x03,0x03,0x03},   
{0x03,0x03,0x00,0x00,0x00,0x00,0x00,0x00},   
{0xF0,0xF1,0xBF,0x9F,0xFF,0xFF,0x86,0xE6},   
{0xF6,0xF6,0xBF,0xBF,0xB6,0xB6,0xF7,0xF7},   
{0x86,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06},   
{0x06,0x3F,0x7F,0x70,0x3B,0x1F,0x0F,0x0F},   
{0x3F,0x3B,0x00,0x37,0x77,0x7F,0x3F,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x3F,0x3F,0x1C,0x1C,0x38,0x3F,0x1F,0x0C},   
{0x0D,0x0F,0x1F,0x38,0x7C,0x7C,0x00,0x00},   
{0x00,0x00,0x30,0x30,0x30,0x30,0x30,0x30},   
{0x30,0x3F,0x3F,0x30,0x30,0x30,0x30,0x30},   
{0x38,0x38,0x30,0x00,0x00,0x00,0x00,0x00},   
{0x19,0x19,0x1D,0x0D,0x0F,0x7F,0x7F,0x33},   
{0x3B,0x1F,0x1F,0x1D,0x1F,0x3F,0x73,0x71},   
{0x31,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},   
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*"E:\CHENXUAN\Cx2004\Project\1MyHandSet\Picture\测试工装.bmp",0*/   
/* (112 X 64 )*/   
};   
//   
//************************************************************************   
//* Menu Struct:                                                         *   
//************************************************************************   
//* First Bmp--------------------->(MenuID=0)                            *   
//*  ---+                                                                *   
//*     |View--------------------->(MenuID=1)                            *   
//*     +---+                                                            *   
//*     |   |About Recorde-------->(MenuID=2)                            *   
//*     |   +--------------                                              *   
//*     |   |HandSet Clock-------->(MenuID=3)                            *   
//*     |   +--------------                                              *   
//*     |Operation---------------->(MenuID=4)                            *   
//*     +---+                                                            *   
//*     |   |Set Position--------->(MenuID=5)                            *   
//*     |   +--------------                                              *   
//*     |   |Rd Zj To Flash------->(MenuID=6)                            *   
//*     |   +--------------                                              *   
//*     |   |Rd Zl To Ram--------->(MenuID=7)                            *   
//*     |   +--------------                                              *   
//*     |   |Del Given Zl--------->(MenuID=8)                            *   
//*     |   +--------------                                              *   
//*     |   |Del All Zl----------->(MenuID=9)                            *   
//*     |   +--------------                                              *   
//*     |Test--------------------->(MenuID=10)                           *   
//*     +---+                                                            *   
//*     |   |Ram Test------------->(MenuID=11)                           *   
//*     |   +--------------                                              *   
//*     |   |Flash Test----------->(MenuID=12)                           *   
//*     |   +--------------                                              *   
//*     |System Set--------------->(MenuID=13)                           *   
//*     +---+                                                            *   
//*         |Debug---------------->(MenuID=14)                           *   
//*         +---+                                                        *   
//*         |   |PassWord--------->(MenuID=15)                           *   
//*         |   +--------------                                          *   
//*         |   |See Picture------>(MenuID=16)                           *   
//*         |   +--------------                                          *   
//*         |Set Hand Clk--------->(MenuID=17)                           *   
//*         +--------------                                              *   
//************************************************************************   
//CurMenuID=本菜单ID   
//MaxMenuItem=同级菜单最大项数   
//OkMenuID=子菜单层所对应的菜单ID,ID=999为菜单已经到底了   
//EscMenuID=父菜单层所对应的菜单ID,ID=999为菜单已经到顶了   
//DownMenuID=弟菜单层所对应的菜单ID,ID=999为菜单是独生子   
//UpMenuID=兄菜单层所对应的菜单ID,ID=999为菜单是独生子   
//CurFunction=本菜单所对应的菜单函数指针   
KeyTabStruct xdata KeyTab[MAX_KEYTABSTRUCT_NUM]=   
    {   
        //CurMenuID,              MaxMenuItem,  OkMenuID,                       EscMenuID,              DownMenuID,                         UpMenuID,                           CurFunction   
        {MENU_FACE,                         0,  MENU_ROOT,                      999,                    999,                                999,                                *MenuFunctionDemo1},   
        {MENU_ROOT,                         4,  MENU_VIEW,                      MENU_FACE,              MENU_OPERATION,                     MENU_SYSTEMSET,                     *MenuFunctionDemo2},   
        {MENU_VIEW,                         2,  MENU_VIEW_ABOUTRECORDE,         MENU_ROOT,              MENU_OPERATION,                     MENU_SYSTEMSET,                     *MenuFunctionDemo3},   
        {MENU_VIEW_ABOUTRECORDE,            0,  999,                            MENU_VIEW,              MENU_VIEW_HANDSETCLOCK,             MENU_VIEW_HANDSETCLOCK,             *MenuFunctionDemo4},   
        {MENU_VIEW_HANDSETCLOCK,            0,  999,                            MENU_VIEW,              MENU_VIEW_ABOUTRECORDE,             MENU_VIEW_ABOUTRECORDE,             *MenuFunctionDemo5},   
        {MENU_OPERATION,                    5,  MENU_OPERATION_SETPOSITION,     MENU_ROOT,              MENU_TEST,                          MENU_VIEW,                          *MenuFunctionDemo6},   
        {MENU_OPERATION_SETPOSITION,        0,  999,                            MENU_OPERATION,         MENU_OPERATION_READZJTOFLASH,       MENU_OPERATION_DELALLZJ,            *MenuFunctionDemo7},   
        {MENU_OPERATION_READZJTOFLASH,      0,  999,                            MENU_OPERATION,         MENU_OPERATION_RDZJTORAM,           MENU_OPERATION_SETPOSITION,         *MenuFunctionDemo8},   
        {MENU_OPERATION_RDZJTORAM,          0,  999,                            MENU_OPERATION,         MENU_OPERATION_DELGIVENZL,          MENU_OPERATION_READZJTOFLASH,       *MenuFunctionDemo9},   
        {MENU_OPERATION_DELGIVENZL,         0,  999,                            MENU_OPERATION,         MENU_OPERATION_DELALLZJ,            MENU_OPERATION_RDZJTORAM,           *MenuFunctionDemo10},   
        {MENU_OPERATION_DELALLZJ,           0,  999,                            MENU_OPERATION,         MENU_OPERATION_SETPOSITION,         MENU_OPERATION_DELGIVENZL,          *MenuFunctionDemo11},   
        {MENU_TEST,                         2,  MENU_TEST_RAMTEST,              MENU_ROOT,              MENU_SYSTEMSET,                     MENU_OPERATION,                     *MenuFunctionDemo12},   
        {MENU_TEST_RAMTEST,                 0,  999,                            MENU_TEST,              MENU_TEST_FLASHTEST,                MENU_TEST_FLASHTEST,                *MenuFunctionDemo13},   
        {MENU_TEST_FLASHTEST,               0,  999,                            MENU_TEST,              MENU_TEST_RAMTEST,                  MENU_TEST_RAMTEST,                  *MenuFunctionDemo14},   
        {MENU_SYSTEMSET,                    2,  MENU_SYSTEMSET_DEBUG,           MENU_ROOT,              MENU_VIEW,                          MENU_TEST,                          *MenuFunctionDemo15},   
        {MENU_SYSTEMSET_DEBUG,              2,  MENU_SYSTEMSET_DEBUG_PASSWORD,  MENU_SYSTEMSET,         MENU_SYSTEMSET_SETHANDCLK,          MENU_SYSTEMSET_SETHANDCLK,          *MenuFunctionDemo16},   
        {MENU_SYSTEMSET_DEBUG_PASSWORD,     0,  999,                            MENU_SYSTEMSET_DEBUG,   MENU_SYSTEMSET_DEBUG_SEEPICTURE,    MENU_SYSTEMSET_DEBUG_SEEPICTURE,    *MenuFunctionDemo17},   
        {MENU_SYSTEMSET_DEBUG_SEEPICTURE,   0,  999,                            MENU_SYSTEMSET_DEBUG,   MENU_SYSTEMSET_DEBUG_PASSWORD,      MENU_SYSTEMSET_DEBUG_PASSWORD,      *MenuFunctionDemo18},   
        {MENU_SYSTEMSET_SETHANDCLK,         0,  999,                            MENU_SYSTEMSET,         MENU_SYSTEMSET_DEBUG,               MENU_SYSTEMSET_DEBUG,               *MenuFunctionDemo19}   
    };   