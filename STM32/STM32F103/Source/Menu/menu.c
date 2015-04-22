//==================================================
//LCD1602多级菜单显示
//树形结构+回调函数
//==================================================
#include "stdint.h"
#include "menu.h"
#include "../LCD1602/lcd1602a.h"
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
//目录结构体定义
//==========================================================================
#include "menu.h"

//菜单目录结构体声明
 struct MenuItem MainMenu[7];
 struct MenuItem testMenu[6];

//菜单使用的全局变量
#define LcdMaxLine	2						//定义LCD最多显示菜单数目
struct MenuItem (*MenuPoint) = MainMenu;	//结构体菜单指针指向主菜单

uint8_t DisplayStart=0;						//显示菜单的开始位置
uint8_t DisplayPoint=0;						//显示菜单的编号
uint8_t	ShowMenuFlag=0;						//菜单运行标志
uint8_t UserChoose=0;						//用户的选择
uint8_t MaxItems=0;							//菜单的数量


//==========================================================================
// 菜单结构体链表
//==========================================================================
//主菜单-第一级菜单
struct MenuItem MainMenu[7] =		//结构体主菜单
{
	{7,"Set Time      ",NullSubs,testMenu,Null},		//1.->设置时间
	{7,"Set Alarm Time",NullSubs,Null,Null},				//2.->设置报警时间段
	{7,"Alarm Swtich  ",NullSubs,Null,Null},				//3.->报警总开关
	{7,"Check Alarm   ",NullSubs,Null,Null},				//4.->查看报警信息
	{7,"Alarm Init    ",NullSubs,Null,Null},				//5.->红外探头初始化
	{7,"Exit          ",NullSubs,MainMenu,MainMenu},//6.->退出(返回主菜单)
	{7,"test          ",NullSubs,MainMenu,MainMenu}	//7.->退出
};

struct MenuItem testMenu[6] =		//结构体主菜单
{
	{6,"www.jhmcu.com ",NullSubs,Null,MainMenu},
	{6,"22222222222222",NullSubs,Null,MainMenu},
	{6,"33333333333333",NullSubs,Null,MainMenu},
	{6,"44444444444444",NullSubs,Null,MainMenu},
	{6,"55555555555555",NullSubs,Null,MainMenu},
	{6,"www.jhmcu.com ",NullSubs,Null,MainMenu}
};

//==================================================
//LCD1602菜单显示通用函数
//INPUT:结构体链表,UserChoose
//OUTPUT:NONE
//FUNCTION:LCD1602液晶显示屏上显示菜单功能
//==================================================
void ShowMenu(void)
{
	uint8_t i = 0;
	MaxItems = MenuPoint[0].MenuCount;	//获取当前菜单的条目数
	DisplayPoint = DisplayStart;		//选中第一项菜单
	for(i=0;i<LcdMaxLine;i++)		//循环显示当前菜单
	{
		//如果当前显示节点小于菜单数
		if(DisplayPoint < MaxItems){
			//如果用户选中该菜单，则在菜单前打印"->"
			if(DisplayPoint==UserChoose)
				LCD_Write_String(0,i,"->");
			else
				LCD_Write_String(0,i,"  ");
			LCD_Write_String(2,i,MenuPoint[DisplayPoint++].DisplayString);
		}else	//显示最后一页菜单，并只有一项菜单时，清除最后一行
		{
			LCD_Write_String(2,LcdMaxLine-1,"                ");	//将上一次显示清屏
		}
	}
	//菜单定位逻辑
	if(UserChoose%LcdMaxLine==0){
		DisplayStart = UserChoose;
	}else{
		//逻辑:如果用户选择的菜单是奇数，则将显示定位在偶尔页
		for(i=0;i<=MaxItems;i++){
			if(UserChoose!=(i%LcdMaxLine))
				DisplayStart = UserChoose-1;	//菜单滚屏
		}
	}
}

//==================================================
//菜单执行函数
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
				UserChoose = 0;	//上翻停止赋值为0,回滚赋值为MaxItems-1
			}
			update =1;
			break;
		case SUB:
			UserChoose++;
			if (UserChoose == MaxItems)
			{
				UserChoose = MaxItems-1;//上翻停止赋值为MaxItems-1,回滚赋值为0
			}
			update =1;
			break;
		case ENTER:
			//如果用户选择的菜单指向的函数不是空函数,则执行功能函数。
			if (MenuPoint[UserChoose].pMenuFun != NullSubs)
			{
				(*MenuPoint[UserChoose].pMenuFun)();	//执行菜单功能函数
			}
			//如果是空函数，则找子菜单,找不到子菜单,就退出
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
