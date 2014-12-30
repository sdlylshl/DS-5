
#include "def.h"
#include "2440addr.h"

void Beep(int x, int y);
/*************************************************************/
//ȫ�ֱ���
extern U16 KeyNum;         //��ֵ
//extern U8  KeyIsUsed;      //��ֵ�Ƿ�ʹ��
//extern U8  KEYDOWN;				//��������
//extern U8  KEYUP; 				//����̧���־
extern U16 key2num[];
/*************************************************************/
// 06 mode     41
// 25 Right		24
// 15 Down 		22
// 35 Left 		28
// 05 UP 		21
// 24 daisu 	14
// 34 queding 	18
// 36 shang 	48
// 07 zuo 		81
// 04 fengmingqi 11
// 16 xia 		42
// 26 you 		44
// 14 fanhui 	12
#if MC216
#define KEY_NUM0            
#define KEY_NUM1            
#define KEY_NUM2            
#define KEY_NUM3            
#define KEY_NUM4            
#define KEY_NUM5            
#define KEY_NUM6            
#define KEY_NUM7            
#define KEY_NUM8            
#define KEY_NUM9            

#define Key_RainClean              //���
#define KEY_F1                      //F1
#define KEY_LongLight               //Զ��
#define Key_ShortLight              //����
#define Key_Walk                    //����
#define Key_SpurtWater             //ϴ����
#define Key_Counter              	//������
#define Key_AirCondition            //�յ�
#define Key_IdleSpeed       0x18        //����
#define Key_Speaker         0x11     	//������

#define KEY_H             
#define KEY_M             
#define KEY_L             
#define KEY_F 
//@ add            
#define KEY_MODE			0x14
#define KEY_BACK            0x12

#define KEY_UP              0x21
#define KEY_DOWN            0x22
#define KEY_LEFT            0x28
#define KEY_RIGHT           0x24
#define KEY_NEXT            0x24
#define KEY_ENTER           0x41

#else

#define KEY_NUM0            0x104
#define KEY_NUM1            0x18
#define KEY_NUM2            0x28
#define KEY_NUM3            0x48
#define KEY_NUM4            0x88
#define KEY_NUM5            0x108
#define KEY_NUM6            0x14
#define KEY_NUM7            0x24
#define KEY_NUM8            0x44
#define KEY_NUM9            0x84

#define Key_RainClean       0x104       //���
#define KEY_F1              0x18        //F1
#define KEY_LongLight       0x28        //Զ��
#define Key_ShortLight      0x48        //����
#define Key_Walk            0x88        //����
#define Key_SpurtWater      0x108       //ϴ����
#define Key_Counter         0x14     	//������
#define Key_AirCondition    0x24        //�յ�
#define Key_IdleSpeed       0x44        //����
#define Key_Speaker         0x84     	//������



#define KEY_H               0x11
#define KEY_M               0x21
#define KEY_L               0x41
#define KEY_F               0x81

#define KEY_UP              0x12
#define KEY_DOWN            0x22
#define KEY_BACK            0x82
#define KEY_NEXT            0x42
#define KEY_ENTER           0x102
#endif
/*************************************************************/
// ���ϰ����Ķ���
// ���EEPROM������1 �� 646879 dky ANSI��Ϊ0x64 0x6B 0x79
/*************************************************************/
#if MC216
	//ǿ��ˢдMyAPP
	#ifndef KEY_DOWNAPP
	#define KEY_DOWNAPP			(KEY_BACK|Key_Speaker)
	#endif
	//�˵�ģʽ
	#ifndef KEY_MENU
	#define KEY_MENU			(KEY_ENTER|Key_IdleSpeed)
	#endif
	//ǿ��APP��ʼ��
	#ifndef KEY_KernalAPP
	#define KEY_KernalAPP			(KEY_BACK)
	#endif
#else
	#ifndef KEY_DOWNAPP
	#define KEY_DOWNAPP			(KEY_BACK|Key_Speaker)
	#endif
	#ifndef KEY_MENU
	#define KEY_MENU			(KEY_ENTER|Key_IdleSpeed)
	#endif
	#ifndef KEY_KernalAPP
	#define KEY_KernalAPP			(KEY_BACK)
	#endif
#endif
extern U16 KeyScan0(void);
extern U16 KeyScan1(void);
extern U16 KeyScan2(void);

#if GPS
//GPH1
#if MC216
#define KeyScan KeyScan2
#else
#define KeyScan KeyScan1
#endif

#define Beep_OFF()	(rGPBDAT &=~1)

#define Beep_ON()   (rGPBDAT |=1) 

#else
//GPG0
#define KeyScan KeyScan0

#define Beep_OFF()	(rGPBDAT|= 1)

#define Beep_ON()   (rGPBDAT&=~1)

#endif
//		10 		20 	 40 	80 	100
//      GPF4  GPF5  GPF6  GPF7  GPG0
//        |     |     |     |     |
//08 GPF3 | 1   |2    |3    |4    |5
//   -----|-----|-----|-----|-----|
//04 GPF2 | 6   |7    |8    |9    |0
//   -----|-----|-----|-----|-----|
//02 GPF1 |UP   |Down |Left |Right|OK
//   -----|-----|-----|-----|-----|
//01 GPF0 |H    |M    |L    | F
//   -----|-----|-----|-----|

//		10 		20 	 40 	80 	100
//      GPF4  GPF5  GPF6  GPF7  GPG0
//        |     |     |     |     |
//08 GPF3 | 1   |2    |3    |4    |5
//   -----|-----|-----|-----|-----|
//04 GPF2 | 6   |7    |8    |9    |0
//   -----|-----|-----|-----|-----|
//02 GPF1 |UP   |Down |Left |Right|OK
//   -----|-----|-----|-----|-----|
//01 GPF0 |H    |M    |L    | F
//   -----|-----|-----|-----|


// 06 mode     41
// 25 Right		24
// 15 Down 		22
// 35 Left 		28
// 05 UP 		21
// 24 daisu 	14
// 34 queding 	18
// 36 shang 	48
// 07 zuo 		81
// 04 fengmingqi 11
// 16 xia 		42
// 26 you 		44
// 14 fanhui 	12