//==================================================
//LCD1602�༶�˵���ʾ
//���νṹ+�ص�����
//==================================================
#include "stdint.h"
#include "menu.h"
#include "../LCD1602/lcd_1602a.h"
#define LCD_Write_String LCD1602_Write_String

#define Null 0

#define	ADD	  8	//
#define	SUB	  7	//
#define	ENTER	  5	//
#define	BACK	  9	//

#define	UP	  6	//
#define DOWN  1	//
#define ESC   3	
void NullSubs(void)
{
}
//==========================================================================
//Ŀ¼�ṹ�嶨��
//==========================================================================
struct MenuItem
{
	uint8_t MenuCount;    					//�˵�����Ŀ
	char *DisplayString; 						//��ʾ���ַ���
	void (*pMenuFun)(); 								//�˵�ָ��Ĺ��ܺ���
	struct MenuItem *ChildrenMenus;	//�ӽڵ�
	struct MenuItem *ParentMenus;		//���ڵ�
};

//�˵�Ŀ¼�ṹ������
 struct MenuItem MainMenu[7];
 struct MenuItem testMenu[6];

//�˵�ʹ�õ�ȫ�ֱ���
#define LcdMaxLine	2											//����LCD�����ʾ�˵���Ŀ
struct MenuItem (*MenuPoint) = MainMenu;	//�ṹ��˵�ָ��ָ�����˵�

uint8_t DisplayStart=0;										//��ʾ�˵��Ŀ�ʼλ��
uint8_t DisplayPoint=0;										//��ʾ�˵��ı��
uint8_t	ShowMenuFlag=0;										//�˵����б�־
uint8_t UserChoose=0;											//�û���ѡ��
uint8_t MaxItems=0;												//�˵�������


//==========================================================================
// �˵��ṹ������
//==========================================================================
//���˵�-��һ���˵�
struct MenuItem MainMenu[7] =		//�ṹ�����˵�
{
	{7,"Set Time      ",NullSubs,testMenu,Null},		//1.->����ʱ��
	{7,"Set Alarm Time",NullSubs,Null,Null},				//2.->���ñ���ʱ���
	{7,"Alarm Swtich  ",NullSubs,Null,Null},				//3.->�����ܿ���
	{7,"Check Alarm   ",NullSubs,Null,Null},				//4.->�鿴������Ϣ
	{7,"Alarm Init    ",NullSubs,Null,Null},				//5.->����̽ͷ��ʼ��
	{7,"Exit          ",NullSubs,MainMenu,MainMenu},//6.->�˳�(�������˵�)
	{7,"test          ",NullSubs,MainMenu,MainMenu}	//7.->�˳�
};

struct MenuItem testMenu[6] =		//�ṹ�����˵�
{
	{6,"www.jhmcu.com ",NullSubs,Null,MainMenu},
	{6,"22222222222222",NullSubs,Null,MainMenu},
	{6,"33333333333333",NullSubs,Null,MainMenu},
	{6,"44444444444444",NullSubs,Null,MainMenu},
	{6,"55555555555555",NullSubs,Null,MainMenu},
	{6,"www.jhmcu.com ",NullSubs,Null,MainMenu}
};

//==================================================
//LCD1602�˵���ʾͨ�ú���
//INPUT:�ṹ������,UserChoose
//OUTPUT:NONE
//FUNCTION:LCD1602Һ����ʾ������ʾ�˵�����
//==================================================
void ShowMenu(void)
{
	uint8_t i = 0;
	MaxItems = MenuPoint[0].MenuCount;	//��ȡ��ǰ�˵�����Ŀ��
	DisplayPoint = DisplayStart;		//ѡ�е�һ��˵�
	for(i=0;i<LcdMaxLine;i++)		//ѭ����ʾ��ǰ�˵�
	{
		//�����ǰ��ʾ�ڵ�С�ڲ˵���
		if(DisplayPoint < MaxItems){
			//����û�ѡ�иò˵������ڲ˵�ǰ��ӡ"->"
			if(DisplayPoint==UserChoose)
				LCD_Write_String(0,i,"->");
			else
				LCD_Write_String(0,i,"  ");
			LCD_Write_String(2,i,MenuPoint[DisplayPoint++].DisplayString);
		}else	//��ʾ���һҳ�˵�����ֻ��һ��˵�ʱ��������һ��
		{
			LCD_Write_String(2,LcdMaxLine-1,"                ");	//����һ����ʾ����
		}
	}
	//�˵���λ�߼�
	if(UserChoose%LcdMaxLine==0){
		DisplayStart = UserChoose;
	}else{
		//�߼�:����û�ѡ��Ĳ˵�������������ʾ��λ��ż��ҳ
		for(i=0;i<=MaxItems;i++){
			if(UserChoose!=(i%LcdMaxLine))
				DisplayStart = UserChoose-1;	//�˵�����
		}
	}
}

//==================================================
//�˵�ִ�к���
//INPUT:NONE
//OUTPUT:NONE
//==================================================
void ChangeMenu(uint8_t keynum)
{
	uint8_t update =0;
	switch(keynum){
		case ADD:
			UserChoose--;
			if (UserChoose==255)
			{
				UserChoose = 0;	//�Ϸ�ֹͣ��ֵΪ0,�ع���ֵΪMaxItems-1
			}
			update =1;
			break;
		case SUB:
			UserChoose++;
			if (UserChoose == MaxItems)
			{
				UserChoose = MaxItems-1;//�Ϸ�ֹͣ��ֵΪMaxItems-1,�ع���ֵΪ0
			}
			update =1;
			break;
		case ENTER:
			//����û�ѡ��Ĳ˵�ָ��ĺ������ǿպ���,��ִ�й��ܺ�����
			if (MenuPoint[UserChoose].pMenuFun != NullSubs)
			{
				(*MenuPoint[UserChoose].pMenuFun)();	//ִ�в˵����ܺ���
			}
			//����ǿպ����������Ӳ˵�,�Ҳ����Ӳ˵�,���˳�
			else if (MenuPoint[UserChoose].ChildrenMenus != Null)
			{
				MenuPoint = MenuPoint[UserChoose].ChildrenMenus;
				UserChoose = 0;
				DisplayStart = 0;
			}
			update =1;
			break;
		case BACK:
			if (MenuPoint[UserChoose].ParentMenus != Null)
			{
				MenuPoint = MenuPoint[UserChoose].ParentMenus;
				UserChoose = 0;
				DisplayStart = 0;
			}
			update =1;
			break;
		default:
			break;
	}
	if(update){
    ShowMenu();
	}
}
