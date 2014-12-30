
#include "def.h"
#include "2440addr.h"
#include "2440lib.h"
#include "Keyboard.h"
/*************************************************************/
//ȫ�ֱ���
U16 KeyNum = 0;        //��ֵ
extern U8  KeyIsUsed = 0;      //��ֵ�Ƿ�ʹ��

extern U8  KEYCOUNT = 0;               //�������´���
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

    rGPFUP  = rGPFUP | ((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3)) ; //��ֹGPF 0-7 ����
    rGPFUP  = rGPFUP | ((1 << 4) | (1 << 5) | (1 << 6) | (1 << 7)) ;
    rGPGUP  = rGPGUP | (1 << 0) ;                               //��ֹGPG0 ����


    rGPFCON = rGPFCON & (~((3 << 0) | (3 << 2) | (3 << 4) | (3 << 6)));     //GPF 0 1 2 3  Ϊ����

    rGPFCON = (rGPFCON & (~((3 << 8) | (3 << 10) | (3 << 12) | (3 << 14)))
               | ((1 << 8) | (1 << 10) | (1 << 12) | (1 << 14)));           //GPF 4 5 6 7  Ϊ���
    rGPGCON = (rGPGCON & (~(3 << 0)) | (1 << 0));                           //GPG0Ϊ���

    rGPFDAT = rGPFDAT & (~((1 << 4) | (1 << 5) | (1 << 6) | (1 << 7))); //�˿�GPF 4-7���0
    rGPGDAT = rGPGDAT & (~(1 << 0));                            //�˿�GPG0���0



    if (~rGPFDAT & 0xF)
    {
        //  Uart_Printf("\n rGPFDAT %2x !\n",rGPFDAT);//Ϊʲô�������ò�����ȷֵ������
        //Delay(1000);//�ӳٱ���Ҫ�� �������
        for (i = 0; i < 50000; i++);
        for (i = 0; i < 50000; i++);
        key = ((~rGPFDAT) & 0x000F);

        rGPFCON = (rGPFCON & (~((3 << 0) | (3 << 2) | (3 << 4) | (3 << 6))) //GPF 0 1 2 3  Ϊ���
                   | ((1 << 0) | (1 << 2) | (1 << 4) | (1 << 6)));

        rGPFCON = rGPFCON & (~((3 << 8) | (3 << 10) | (3 << 12) | (3 << 14))); //GPF 4 5 6 7  Ϊ����
        rGPGCON = rGPGCON & (~(3 << 0));                        //GPG0Ϊ����
        for (i = 0; i < 50000; i++); //�ӳٱ���Ҫ�� �������
        rGPFDAT = rGPFDAT & (~((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3))); //�˿�GPF 0-3���0

        for (i = 0; i < 50000; i++); //�ӳٱ���Ҫ�� �������
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

    // rGPFUP  = rGPFUP | ((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3)) ; //��ֹGPF 0-7 ����
    // rGPFUP  = rGPFUP | ((1 << 4) | (1 << 5) | (1 << 6) | (1 << 7)) ;
    rGPFUP  = 0;
    rGPHUP  = rGPHUP & ( ~(1 << 1)) ;                              //��ֹGPH1 ����


    rGPFCON = rGPFCON & (~((3 << 0) | (3 << 2) | (3 << 4) | (3 << 6)));     //GPF 0 1 2 3  Ϊ����

    rGPFCON = (rGPFCON & (~((3 << 8) | (3 << 10) | (3 << 12) | (3 << 14)))
               | ((1 << 8) | (1 << 10) | (1 << 12) | (1 << 14)));           //GPF 4 5 6 7  Ϊ���
    rGPHCON = (rGPHCON & (~(3 << 2)) | (1 << 2));                           //GPH1Ϊ���

    rGPFDAT = rGPFDAT & (~((1 << 4) | (1 << 5) | (1 << 6) | (1 << 7))); //�˿�GPF 4-7���0
    rGPHDAT = rGPHDAT & (~(1 << 1));                            //�˿�GPH1���0



    if (~rGPFDAT & 0xF)
    {
        //  Uart_Printf("\n rGPFDAT %2x !\n",rGPFDAT);//Ϊʲô�������ò�����ȷֵ������
        //Delay(1000);//�ӳٱ���Ҫ�� �������
       // for (i = 0; i < 50000; i++);
        for (i = 0; i < 5000; i++);
        key = ((~rGPFDAT) & 0x000F);

        rGPFCON = (rGPFCON & (~((3 << 0) | (3 << 2) | (3 << 4) | (3 << 6))) //GPF 0 1 2 3  Ϊ���
                   | ((1 << 0) | (1 << 2) | (1 << 4) | (1 << 6)));

        rGPFCON = rGPFCON & (~((3 << 8) | (3 << 10) | (3 << 12) | (3 << 14))); //GPF 4 5 6 7  Ϊ����
        rGPHCON = rGPHCON & (~(3 << 2));                        //GPG0Ϊ����
        for (i = 0; i < 50000; i++); //�ӳٱ���Ҫ�� �������
        rGPFDAT = rGPFDAT & (~((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3))); //�˿�GPF 0-3���0

        for (i = 0; i < 50000; i++); //�ӳٱ���Ҫ�� �������
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

    // rGPFUP  = rGPFUP | ((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3)) ; //��ֹGPF 0-7 ����
    // rGPFUP  = rGPFUP | ((1 << 4) | (1 << 5) | (1 << 6) | (1 << 7)) ;
    rGPFUP  = 0;            //0�����ڲ�����


    rGPFCON = rGPFCON & (~((3 << 0) | (3 << 2) | (3 << 4) | (3 << 6)));     //GPF 0 1 2 3  Ϊ����

    rGPFCON = (rGPFCON & (~((3 << 8) | (3 << 10) | (3 << 12) | (3 << 14)))
               | ((1 << 8) | (1 << 10) | (1 << 12) | (1 << 14)));           //GPF 4 5 6 7  Ϊ���

    rGPFDAT = rGPFDAT & (~((1 << 4) | (1 << 5) | (1 << 6) | (1 << 7))); //�˿�GPF 4-7���0


    if (~rGPFDAT & 0xF)
    {
        //  Uart_Printf("\n rGPFDAT %2x !\n",rGPFDAT);//Ϊʲô�������ò�����ȷֵ������
        //Delay(1000);//�ӳٱ���Ҫ�� �������
        for (i = 0; i < 50000; i++);
        for (i = 0; i < 50000; i++);
        key = ((~rGPFDAT) & 0x0F);

        rGPFCON = (rGPFCON & (~((3 << 0) | (3 << 2) | (3 << 4) | (3 << 6))) //GPF 0 1 2 3  Ϊ���
                   | ((1 << 0) | (1 << 2) | (1 << 4) | (1 << 6)));

        rGPFCON = rGPFCON & (~((3 << 8) | (3 << 10) | (3 << 12) | (3 << 14))); //GPF 4 5 6 7  Ϊ����

        for (i = 0; i < 50000; i++); //�ӳٱ���Ҫ�� �������
        rGPFDAT = rGPFDAT & (~((1 << 0) | (1 << 1) | (1 << 2) | (1 << 3))); //�˿�GPF 0-3���0

        for (i = 0; i < 50000; i++); //�ӳٱ���Ҫ�� �������
        key = key | (~rGPFDAT & 0xF0);

    }

#ifdef DEBUG
    if (key != 0)
        Uart_Printf("\n key %x !\n", key);
#endif

    KeyNum = key;

    return key;
}
//keypress �¼����� Click �¼�
extern U16 KeyPress(){

}
//��������
extern U16 KeyDown(){


}
//����̧��(release)
extern U16 KeyUp(){

}
extern U16 GetLastKey(){


}
//�ȴ����ⰴ��
extern U16 WaitKey()
{
    U16 Keys = 0;
    //�ȴ�һ����������
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

