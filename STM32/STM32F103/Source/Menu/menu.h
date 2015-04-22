#ifndef _MENU_H_
#define _MENU_H_
typedef struct MenuItem
{
	uint8_t MenuCount;    				//菜单的数目
	char *DisplayString; 				//显示的字符串
	void (*pMenuFun)(); 				//菜单指向的功能函数
	struct MenuItem *ChildrenMenus;	    //子节点
	struct MenuItem *ParentMenus;		//父节点
}MenuItem_t;

void NullSubs(void);
void ShowMenu(void);
#endif 
