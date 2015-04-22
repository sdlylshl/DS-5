

//ReadMe   
//屏宽:112   
//屏高:64   
#include "stdint.h" 
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
void MenuFunctionNull(void);   
void MenuFunctionDemo1(void);   
void MenuFunctionDemo2(void){};   
void MenuFunctionDemo3(void){};   
void MenuFunctionDemo4(void);   
void MenuFunctionDemo5(void){};   
void MenuFunctionDemo6(void){};   
void MenuFunctionDemo7(void){};   
void MenuFunctionDemo8(void){};   
void MenuFunctionDemo9(void){};   
void MenuFunctionDemo10(void){};   
void MenuFunctionDemo11(void){};   
void MenuFunctionDemo12(void){};   
void MenuFunctionDemo13(void){};   
void MenuFunctionDemo14(void){};   
void MenuFunctionDemo15(void){};   
void MenuFunctionDemo16(void){};   
void MenuFunctionDemo17(void){};   
void MenuFunctionDemo18(void){};   
void MenuFunctionDemo19(void){};   
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
KeyTabStruct KeyTab[MAX_KEYTABSTRUCT_NUM]=   
    {   
        //CurMenuID,				MaxMenuItem,    OkMenuID,                       EscMenuID,              DownMenuID,                         UpMenuID,                           CurFunction   
        {MENU_FACE,								0,  MENU_ROOT,                      999,                    999,                                999,                                *MenuFunctionDemo1},   
        {MENU_ROOT,								4,  MENU_VIEW,                      MENU_FACE,              MENU_OPERATION,                     MENU_SYSTEMSET,                     *MenuFunctionDemo2},   
        
		{MENU_VIEW,								2,  MENU_VIEW_ABOUTRECORDE,         MENU_ROOT,              MENU_OPERATION,                     MENU_SYSTEMSET,                     *MenuFunctionDemo3},   
			{MENU_VIEW_ABOUTRECORDE,            0,  999,                            MENU_VIEW,              MENU_VIEW_HANDSETCLOCK,             MENU_VIEW_HANDSETCLOCK,             *MenuFunctionDemo4},   
			{MENU_VIEW_HANDSETCLOCK,            0,  999,                            MENU_VIEW,              MENU_VIEW_ABOUTRECORDE,             MENU_VIEW_ABOUTRECORDE,             *MenuFunctionDemo5},   
        
		{MENU_OPERATION,						5,  MENU_OPERATION_SETPOSITION,     MENU_ROOT,              MENU_TEST,                          MENU_VIEW,                          *MenuFunctionDemo6},   
			{MENU_OPERATION_SETPOSITION,        0,  999,                            MENU_OPERATION,         MENU_OPERATION_READZJTOFLASH,       MENU_OPERATION_DELALLZJ,            *MenuFunctionDemo7},   
			{MENU_OPERATION_READZJTOFLASH,      0,  999,                            MENU_OPERATION,         MENU_OPERATION_RDZJTORAM,           MENU_OPERATION_SETPOSITION,         *MenuFunctionDemo8},   
			{MENU_OPERATION_RDZJTORAM,          0,  999,                            MENU_OPERATION,         MENU_OPERATION_DELGIVENZL,          MENU_OPERATION_READZJTOFLASH,       *MenuFunctionDemo9},   
			{MENU_OPERATION_DELGIVENZL,         0,  999,                            MENU_OPERATION,         MENU_OPERATION_DELALLZJ,            MENU_OPERATION_RDZJTORAM,           *MenuFunctionDemo10},   
			{MENU_OPERATION_DELALLZJ,           0,  999,                            MENU_OPERATION,         MENU_OPERATION_SETPOSITION,         MENU_OPERATION_DELGIVENZL,          *MenuFunctionDemo11},   
        
		{MENU_TEST,								2,  MENU_TEST_RAMTEST,              MENU_ROOT,              MENU_SYSTEMSET,                     MENU_OPERATION,                     *MenuFunctionDemo12},   
			{MENU_TEST_RAMTEST,                 0,  999,                            MENU_TEST,              MENU_TEST_FLASHTEST,                MENU_TEST_FLASHTEST,                *MenuFunctionDemo13},   
			{MENU_TEST_FLASHTEST,               0,  999,                            MENU_TEST,              MENU_TEST_RAMTEST,                  MENU_TEST_RAMTEST,                  *MenuFunctionDemo14},   
        {MENU_SYSTEMSET,						2,  MENU_SYSTEMSET_DEBUG,           MENU_ROOT,              MENU_VIEW,                          MENU_TEST,                          *MenuFunctionDemo15},   
			{MENU_SYSTEMSET_DEBUG,              2,  MENU_SYSTEMSET_DEBUG_PASSWORD,  MENU_SYSTEMSET,         MENU_SYSTEMSET_SETHANDCLK,          MENU_SYSTEMSET_SETHANDCLK,          *MenuFunctionDemo16},   
			{MENU_SYSTEMSET_DEBUG_PASSWORD,     0,  999,                            MENU_SYSTEMSET_DEBUG,   MENU_SYSTEMSET_DEBUG_SEEPICTURE,    MENU_SYSTEMSET_DEBUG_SEEPICTURE,    *MenuFunctionDemo17},   
			{MENU_SYSTEMSET_DEBUG_SEEPICTURE,   0,  999,                            MENU_SYSTEMSET_DEBUG,   MENU_SYSTEMSET_DEBUG_PASSWORD,      MENU_SYSTEMSET_DEBUG_PASSWORD,      *MenuFunctionDemo18},   
			{MENU_SYSTEMSET_SETHANDCLK,         0,  999,                            MENU_SYSTEMSET,         MENU_SYSTEMSET_DEBUG,               MENU_SYSTEMSET_DEBUG,               *MenuFunctionDemo19}   
    };  
//***************************************************************************************************************************************************************************************************		
uint8_t cMenuTemp1;
uint8_t cMenuTemp2;
uint8_t cMenuTemp3;
//   
uint16_t  iMenuID;			//当前菜单项 索引ID
uint16_t  iMenuNextID ;		//按键执行后 索引ID

void ShowNextMenu(uint16_t iMenuNextID)
{
	switch (iMenuNextID)
	{
	case    MENU_FACE:
		//DispFaceBmp();
		break;
	case    MENU_ROOT:
		//DispFaceBmp();
		break;
	case    MENU_VIEW:
		//DispMenu_Root();
		break;
	case    MENU_VIEW_ABOUTRECORDE:
	case    MENU_VIEW_HANDSETCLOCK:
		//DispMenu_View();
		break;
	case    MENU_OPERATION:
		//DispMenu_Root();
		break;
	case    MENU_OPERATION_SETPOSITION:
	case    MENU_OPERATION_READZJTOFLASH:
	case    MENU_OPERATION_RDZJTORAM:
	case    MENU_OPERATION_DELGIVENZL:
	case    MENU_OPERATION_DELALLZJ:
		//DispMenu_Operation();
		break;
	case    MENU_TEST:
		//DispMenu_Root();
		break;
	case    MENU_TEST_RAMTEST:
	case    MENU_TEST_FLASHTEST:
		//DispMenu_Test();
		break;
	case    MENU_SYSTEMSET:
		//DispMenu_Root();
		break;
	case    MENU_SYSTEMSET_DEBUG:
		//DispMenu_SystemSet();
		break;
	case    MENU_SYSTEMSET_DEBUG_PASSWORD:
	case    MENU_SYSTEMSET_DEBUG_SEEPICTURE:
		//DispMenu_Debug();
		break;
	case    MENU_SYSTEMSET_SETHANDCLK:
		//DispMenu_SystemSet();
		break;
	default:
		break;
	}

}
extern uint16_t NowKey;
extern uint8_t b_KeyInt; //中断触发标志 按键有效标志
void Change_Menu(uint16_t keyvalue){
	if (b_KeyInt)
	{
		switch (NowKey)
		{
		case    KEY_OK://KEY_OK,        KEY_UP,     KEY_ESC,   
			if (KeyTab[iMenuID].KeyTab_PressOk != 999)
			{
				iMenuID = KeyTab[iMenuID].KeyTab_PressOk;
				iMenuNextID = KeyTab[iMenuID].KeyTab_PressOk;
				ShowNextMenu(iMenuNextID);	//显示下级菜单
			}
			KeyFuncPtr = KeyTab[iMenuID].CurrentOperate;
			(*KeyFuncPtr)();    //执行当前按键的操作   
			break;
		case    KEY_ESC:
			//   
			if (KeyTab[iMenuID].KeyTab_PressEsc != 999)
			{
				iMenuID = KeyTab[iMenuID].KeyTab_PressEsc;
				iMenuNextID = KeyTab[iMenuID].KeyTab_PressOk;
				ShowNextMenu(iMenuNextID);	//显示下级菜单
				KeyFuncPtr = KeyTab[iMenuID].CurrentOperate;
				(*KeyFuncPtr)();    //执行当前按键的操作   
			}
			break;
		case    KEY_UP:
			if ((KeyTab[iMenuID].KeyTab_PressUp != 999) && (iMenuNextID != 999))
			{
				KeyTab[iMenuID].KeyTab_PressOk = KeyTab[iMenuNextID].KeyTab_PressUp;
				iMenuNextID = KeyTab[iMenuNextID].KeyTab_PressUp;
				ShowNextMenu(iMenuNextID);	//显示下级菜单
			}
			break;
			//   
		case    KEY_LEFT://KEY_LEFT,    KEY_DOWN,   KEY_RIGHT,   
			if ((KeyTab[iMenuID].KeyTab_PressUp != 999) && (iMenuNextID != 999))
			{
				KeyTab[iMenuID].KeyTab_PressOk = KeyTab[iMenuNextID].KeyTab_PressUp;
				iMenuNextID = KeyTab[iMenuNextID].KeyTab_PressUp;
				ShowNextMenu(iMenuNextID);	//显示下级菜单
			}
			break;
		case    KEY_DOWN:
			if ((KeyTab[iMenuID].KeyTab_PressDown != 999) && (iMenuNextID != 999))
			{
				KeyTab[iMenuID].KeyTab_PressOk = KeyTab[iMenuNextID].KeyTab_PressDown;
				iMenuNextID = KeyTab[iMenuNextID].KeyTab_PressDown;
				ShowNextMenu(iMenuNextID);	//显示下级菜单
			}
			break;
		case    KEY_RIGHT:
			if ((KeyTab[iMenuID].KeyTab_PressDown != 999) && (iMenuNextID != 999))
			{
				KeyTab[iMenuID].KeyTab_PressOk = KeyTab[iMenuNextID].KeyTab_PressDown;
				iMenuNextID = KeyTab[iMenuNextID].KeyTab_PressDown;
				ShowNextMenu(iMenuNextID);	//显示下级菜单
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
			
			break;
			//   
		default:
			break;
		}
		b_KeyInt = 0;
	}
}

//   
//void DispFaceBmp();   
//void DispMenu_Root();   
//void DispMenu_View();   
//void DispMenu_Operation();   
//void DispMenu_Test();   
//void DispMenu_SystemSet();   
//void DispMenu_Debug();   
 

 char *Menu_Root[4]=   
    {   
    " View         ",   
    " Operation    ",   
    " Test         ",   
    " System Set   "   
    };   
unsigned int Menu_RootID[4]=   
    {   
    MENU_VIEW,   
    MENU_OPERATION,   
    MENU_TEST,   
    MENU_SYSTEMSET   
    };   
//   
 char *Menu_View[2]=   
    {   
    " About ZL     ",   
    " HandSet CLK  "   
    };   
unsigned int Menu_ViewID[2]=   
    {   
    MENU_VIEW_ABOUTRECORDE,   
    MENU_VIEW_HANDSETCLOCK   
    };   
//   
 char *Menu_Operation[5]=   
    {   
    " Set Position ",   
    " Rd Zj To ROM ",   
    " Rd ZL To Ram ",   
    " Del Given ZL ",   
    " Del All ZL   "   
    };   
unsigned int Menu_OperationID[5]=   
    {   
    MENU_OPERATION_SETPOSITION,   
    MENU_OPERATION_READZJTOFLASH,   
    MENU_OPERATION_RDZJTORAM,   
    MENU_OPERATION_DELGIVENZL,   
    MENU_OPERATION_DELALLZJ   
    };   
//   
 char *Menu_Test[2]=   
    {   
    " Ram Test     ",   
    " Flash Test   "   
    };   
unsigned int Menu_TestID[2]=   
    {   
    MENU_TEST_RAMTEST,   
    MENU_TEST_FLASHTEST   
    };   
//   
 char *Menu_SystemSet[2]=   
    {   
    " Debug        ",   
    " Set Hand Clk "   
    };   
unsigned int Menu_SystemSetID[2]=   
    {   
    MENU_SYSTEMSET_DEBUG,   
    MENU_SYSTEMSET_SETHANDCLK   
    };   
//   
 char *Menu_Debug[2]=   
    {   
    " PassWord     ",   
    " See Picture  "   
    };   
unsigned int Menu_DebugID[2]=   
    {   
    MENU_SYSTEMSET_DEBUG_PASSWORD,   
    MENU_SYSTEMSET_DEBUG_SEEPICTURE   
    };   
//   
unsigned char MenuNULL[]={"              "};   
//   

void DispMenu_Debug()   
{
   
}   

void MenuFunctionNull()   
{     
	
}   
//   
void MenuFunctionDemo1(void)//   
{   
//DispOneBoxTextChar(8,1,'W',0);//   
//DispOneBoxTextChar(8,2,'e',0);   
//DispOneBoxTextChar(8,3,'l',0);   
//DispOneBoxTextChar(8,4,'c',0);   
//DispOneBoxTextChar(8,5,'o',0);   
//DispOneBoxTextChar(8,6,'m',0);   
//DispOneBoxTextChar(8,7,'e',0);   
//DispOneBoxTextChar(8,8,' ',0);//   
//DispOneBoxTextChar(8,9,'Y',0);   
//DispOneBoxTextChar(8,10,'o',0);   
//DispOneBoxTextChar(8,11,'u',0);   
//DispOneBoxTextChar(8,12,' ',0);   
//DispOneBoxTextChar(8,13,'!',0);   
//DispOneBoxTextChar(8,14,' ',0);   
}   
//   
void MenuFunctionDemo4(void)   
{   
//DispOneBoxTextChar(8,1,' ',1);   
//DispOneBoxTextChar(8,2,' ',1);   
//DispOneBoxTextChar(8,3,' ',1);   
//DispOneBoxTextChar(8,4,' ',1);   
//DispOneBoxTextChar(8,5,' ',1);   
//DispOneBoxTextChar(8,6,' ',1);   
//DispOneBoxTextChar(8,7,' ',1);   
//DispOneBoxTextChar(8,8,' ',1);   
//DispOneBoxTextChar(8,9,' ',1);   
//DispOneBoxTextChar(8,10,' ',1);   
//DispOneBoxTextChar(8,11,' ',1);   
//DispOneBoxTextChar(8,12,'<',0);   
//DispOneBoxTextChar(8,13,'<',0);   
//DispOneBoxTextChar(8,14,'<',0);   
}   

