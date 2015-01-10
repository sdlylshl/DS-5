
#include "def.h"
#include "2440addr.h"
#include "2440lib.h"
#include "LCD_Driver.h"
#include "MCP2515.h"
#include "Keyboard.h"
#include "stdio.h"          // sprintf  
//extern char  read_buf[256];       //Read Can Buffer
//extern unsigned int ntxbuffer;        //the Number of Tx Buffer

//#include "ver.h"
extern void Versions(void);

extern void Uart_Printf(char *fmt, ...);

extern void Can_Write(U32 id, U8 *pdata, unsigned char dlc, int IsExt, int rxRTR);
extern void Init_MCP2510(CanBandRate bandrate);
extern void Can_Setup(void);
extern int Can_Poll(void);
extern int Can_Read(int n, U32 *id, U8 *pdata,  U8 *dlc, int *rxRTR, int *isExt);


#define CAN_SEND_ID 0x290

extern void CanTest0(void)
{
    int i, j;
    U32 id;
    unsigned char dlc;
    int rxRTR, isExt;
    int temp;
    char tmp[20];
    char flg;

    U8 data_write[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    U8 data_read[8] ;

    //      RETAILMSG(1,(TEXT( "\r\nCAN BUS Test[ MCP2510 ], press ESC key to exit !\r\n") ) ) ;
    Lcd_ClearScr(0x0);   //黑色
    Versions();
    print(0, 2, "Starting Can Test !", 0xFF);

    Init_MCP2510(BandRate_250kbps);
    Can_Setup();

    //   Uart_Init(0,115200);//初始化串口控制寄存器，设置正确的波特率
    //   Uart_Select(0);//选择串口，这里我们选串口0

    Uart_Printf( "\nCAN BUS TEST \n" );
    Uart_Printf( "Press 'ESC' key to Exit this program !\n\n" );
    i = 0;
    flg = 0;
    for (  j = 1; j < 21; j++)
        // while(1)
    {

        Can_Write( CAN_SEND_ID, data_write, 8, false, false);//发送报文  标准模式下必须有接收端才能发送成功
        //      Can_Write( 0x5b, data_write, 8, false, false);
        //#else
        while ( (i = Can_Poll()) == -1 ) ;

        Uart_Printf( "i=%d          " , i);
        for ( temp = 0; temp < 8; temp++)  data_read[temp] = 0 ;

        temp = Can_Read(i, &id, data_read, &dlc, &rxRTR, &isExt);
        ///temp=MCP2510_Read_Can(U8 nbuffer,&rxRTR,&id,data_read ,&dlc)
        Uart_Printf( "ID=0x%x\n", id );
        Uart_Printf( "numbers=%d\n" , j + 1);
        Uart_Printf( "Data=%x,%x,%x,%x,%x,%x,%x,%x\n\n", data_read[0], data_read[1], data_read[2], data_read[3], data_read[4], data_read[5], data_read[6], data_read[7] );
        //if ( Uart_GetKey() == 0x1b ) break;
        Lcd_ClearScr(0x0);   //黑色
        Versions();
        print(0, 2, "Starting Can Test !", 0xFF);
        print(0, 4, "ID =", 0xFF);
        sprintf(tmp, "%x", id);
        print(5, 4, tmp, 0xFF);

        print(0, 5, "Number =", 0xFF);
        sprintf(tmp, "%d", j);
        print(9, 5, tmp, 0xFF);
        for (temp = 0; temp < 8; temp++)
        {
            print(0, 6, "Data =", 0xFF);
            sprintf(tmp, "%X", data_read[temp]);
            print(7 + temp * 2, 6, tmp, 0xFF);
        }
        for ( temp = 0; temp < 8; temp++)
        {
            if (!(data_read[temp] == data_write[temp]))
                flg++;
        }

        Lcd_Update();
        Delay(5000);
    }
    if (flg)
    {
        print(0, 8, "CAN ERRO!!!", 0xFF0000);
    }
    else
        print(0, 10, "Can Test is OK!", 0xFF);
    print(0, 13, "Press anykey to continue!", 0xFF);

    Uart_Printf( "\nCAN BUS TEST done\n" );
    while (!(KEY_BACK == KeyScan()));
}
/**
 * 1.SendNum发送次数
 * 2.seNum发送失败次数
 * 3.rNum接收次数
 * 4.reNum接收错误次数
 *
 * 误码率
 */
/*
extern void CanTest2(void)
{

    U32 id;
    unsigned char dlc, err;
    int rxRTR, isExt;
    int temp;
    char tmp[20];
    char flg;

    int PollX; //接收缓冲器号 【0，1】
    U8 data_write[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    U8 data_read[8] ;
    U32 dat_send = 0; //待发送数据
    U32 i, j, k;
    U32 TimeOut;
    U32 SendNum = 0;
    U32 seNum = 0;
    U32 rNum = 0;
    U32 reNum = 0;

    //配置LCD显示
    Lcd_Init();



    //Uart_Init(0,115200);//初始化串口控制寄存器，设置正确的波特率
    //Uart_Select(0);//选择串口，这里我们选串口0
    Uart_Printf( "\nCAN BUS TEST \n" );
    Uart_Printf( "Press 'ESC' key to Exit this program !\n\n" );

    //CAN 初始化
    Init_MCP2510(BandRate_250kbps);
    //设置接收滤波器
    Can_Setup();
 

    flg = 0;
    while (1)
    {
        if (k++ > 0xFFFFFF00)
        {
            //初始化
            k = 0;
            SendNum = 0;
            seNum = 0;
            rNum = 0;
            reNum = 0;
            //暂停
        }


        // 串口输出
        Uart_Printf( "i=%d" , i);
        ///temp=MCP2510_Read_Can(U8 nbuffer,&rxRTR,&id,data_read ,&dlc)
        Uart_Printf( "ID=0x%x\n", id );
        Uart_Printf( "numbers=%d\n" , j + 1);
        Uart_Printf( "Data=%x,%x,%x,%x,%x,%x,%x,%x\n\n", data_read[0], data_read[1], data_read[2], data_read[3], data_read[4], data_read[5], data_read[6], data_read[7] );

        //LCD输出
        Versions();
        //Lcd_SetPosition(0, 4);
        Lcd_TextAlignMid();
        Printf("Starting Can Test !\n");

        Printf("Send   Number  is %d\n", SendNum);
        Printf("Send   Erro    is %d\n", seNum);
        Printf("Recive Number  is %d\n", rNum);
        Printf("Recive Erro    is %d\n", reNum);

        Printf("\nPress anykey Stop!\n");
        Printf("\nPress KEY_BACK Continue!\n");
        Lcd_Update();

        data_write[4] = SendNum & 0xFF;
        data_write[5] = (SendNum & 0xFF00) >> 8;
        data_write[6] = (SendNum & 0xFF0000) >> 16;
        data_write[7] = (SendNum & 0xFF000000) >> 24;

        //读缓冲器清零
        for ( j = 0; j < 8; j++)
            data_read[j] = 0 ;

        //CAN 发送
        SendNum++;
        Can_Write( CAN_SEND_ID, data_write, 8, false, false); //发送报文  标准模式下必须有接收端才能发送成功

       
        //等待CAN接收
        TimeOut = 10000; //设定超时时间ms

        //Can_Poll 查询缓冲器中是否有数据 如果有则返回缓冲器号 否则返回-1

        while ( ((PollX = Can_Poll()) == -1) && (TimeOut) )
        {
            while (TimeOut--)
            {
                Delay(1000);
            }

        }

        //收到应答报文
        if (TimeOut)
        {

            temp = Can_Read(PollX, &id, data_read, &dlc, &rxRTR, &isExt);
            rNum++;
        }

        //校验接收的报文是否正确
        for (i = 3; i < 8; ++i)
        {
            if (data_write[i] != data_read[i])
                break;
        }

        if (i < 9)
        {
            reNum++;
        }

     Delay(10000);

        //任意键暂停
        if (KeyScan())
        {
            Delay(5000);
            while (!(KEY_BACK == KeyScan()));
        }

    }

}
*/
U32         CanId;      //can_id为返回的ID值
U8          dlc;        //dlc表示data length code 0~8字节
//U32       rxRTR;      //rxRTR表示是否是RXRTR远程帧
//U32       isExt;
U8          data_read[8] ;
U8          data_write[8];
extern U32 SendNum;
extern U32 SendErrNum;
extern U32 ReceiveNum;
extern U32 RecErrNum;

void CanTest(void)
{

    unsigned char  err;
    int rxRTR, isExt;
    int temp;
    char tmp[20];
    char flg;

    int PollX; //接收缓冲器号 【0，1】
    U32 i, j, k;
    U32 TimeOut;
   


    //配置LCD显示
    Lcd_Init();

    //CAN 初始化
    Init_MCP2510(BandRate_250kbps);
    //设置接收滤波器
    Can_Setup();
    Init_CANReciveINT();
    Init_Timer1(); //开启CAN发送中断
    CanId = 0;    //can_id为返回的ID值
    dlc = 0;      //dlc表示data length code 0~8字节
    SendNum = 0;
    SendErrNum = 0;
    ReceiveNum = 0;
    RecErrNum = 0;

    flg = 0;
    //长按返回键退出
    while (!(KEY_BACK == KeyScan()))
    {
        /*
        if (k++ > 0xFFFFFF00)
        {
            //初始化
            k = 0;
            SendNum = 0;
            seNum = 0;
            rNum = 0;
            reNum = 0;
            //暂停
        }
        */
        //CanId = 0;    //can_id为返回的ID值

        //读缓冲器清零

        // SendNum++;
        // data_write[4] = SendNum & 0xFF;
        // data_write[5] = (SendNum & 0xFF00) >> 8;
        // data_write[6] = (SendNum & 0xFF0000) >> 16;
        // data_write[7] = (SendNum & 0xFF000000) >> 24;
        //CAN 发送
        
        // Can_Write( CAN_SEND_ID, data_write, 8, false, false); //发送报文  标准模式下必须有接收端才能发送成功
       // Can_Write( CAN_SEND_ID, data_write, 8, false, false); //发送报文  标准模式下必须有接收端才能发送成功
		
            //LCD输出
        //Lcd_Clear();
        //Versions();
        Lcd_SetPosition(0, 4);
        Lcd_TextAlignMid();
        Printf("Starting Can Test !\n\n\n");
		
        Printf("Send   Number  is %d\n", SendNum);
        Printf("Send   Erro    is %d\n", SendErrNum);
        Printf("Recive Number  is %d\n", ReceiveNum);
        Printf("Recive Erro    is %d\n\n", RecErrNum);
        Printf("CanId  is %x\n", CanId);
        Printf( "Send   Data=%x,%x,%x,%x,%x,%x,%x,%x\n\n", data_write[0], data_write[1], data_write[2], data_write[3], data_write[4], data_write[5], data_write[6], data_write[7] );
        Lcd_SetPosition(0, 14);
        Printf( "Recive Data=%x,%x,%x,%x,%x,%x,%x,%x\n\n", data_read[0], data_read[1], data_read[2], data_read[3], data_read[4], data_read[5], data_read[6], data_read[7] );

        Printf("\nPress anykey pause!\n");
        Printf("\nPress KEY_BACK Exit!\n");
        Lcd_Update();


        // 串口输出
       // Uart_Printf( "i=%d" , i);
        ///temp=MCP2510_Read_Can(U8 nbuffer,&rxRTR,&id,data_read ,&dlc)
        //Uart_Printf( "ID=0x%x\n", CanId );
      //  Uart_Printf( "numbers=%d\n" , j + 1);
       // Uart_Printf( "Data=%x,%x,%x,%x,%x,%x,%x,%x\n\n", data_read[0], data_read[1], data_read[2], data_read[3], data_read[4], data_read[5], data_read[6], data_read[7] );

    
        //任意键暂停
        if ((KeyScan())&&(KEY_BACK != KeyScan()))
        {
            Delay(5000);
            while (!(KEY_ENTER == KeyScan()));
        }

    }
    DisableIrq(BIT_TIMER1);

}