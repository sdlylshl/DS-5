

typedef struct
{
    　uchar 　keystateindex ;    　　// 当前状态索引号
    　uchar 　keydnstate ;    　　// 按下“向下”键时转向的状态索引号
    　uchar 　keyupstate ;    　　// 按下“向上”键时转向的状态索引号
    　uchar 　keycrstate ;    　　// 按下“回车”键时转向的状态索引号
    　uchar 　keybackstate ;    　　// 按下“退回”键时转向的状态索引号
    　void 　(*currentoperate)() ;   　　// 当前状态应该执行的功能*作
}keytab_t;
# define size - of keybd - menu 55 　　/ / 菜单总长度

keytab_t ruct  keytab[ size - of - keybd - menu ] =

{
    {0 ,0 ,0 ,1 ,0 , ( * mainjob1) } ,
    {1 ,7 ,2 ,8 ,0 , ( * dsp point) } , 　　/ / 第一层
    {2 ,1 ,3 ,8 ,0 , ( * dspcurve) )} , 　　/ / 第一层
    {3 ,2 ,4 ,36 ,0 , ( * dsp kout) } , 　　/ / 第一层
    {4 ,3 ,5 ,50 ,0 , ( * disclosedown) } , 　　/ / 第一层

}

typedef struct _TMenu{
    uint8_t Current;     
    uint8_t EventArg;    
    void(*OnOk)();
    void(*OnCancel)(); 
    void(*OnOtherKey)();

    
}TMenu;
TMenu MainMenu;

char * MainMenuTitle = "主菜单";

const char * MainMenuItems[] = {

    "1.室内温度调整",

    "2.室外温度调整",

    "3.运行时间调整",

    "4.ESC退出",

};
uint8_t tempin,tempout,temprun;

uint8_t Key_Value();

uint8_t Change_Value(uint8_t);

void Delay_1(unsigned int m);

void Run_Satus();



void OnSelectMainMenu(int index)
{

    if (index < 0 || index >= sizeof(MainMenuItems) / sizeof(char *)) return;

    ClearScreen();

    switch (index) {

    case 0:

        TextOut(0, 1, "室内调整开始 ");

        tempin=Change_Value(tempin);

        TextOut(0, 1, "室内调整完毕");

        Delay_1(300000);

        break;

    case 1:

        TextOut(0, 1, "室外调整开始 ");

        tempout=Change_Value(tempout);

        TextOut(0, 1, "室外调整完毕");

        Delay_1(300000);

        break;

    case 2:

        TextOut(0, 1, "运行时间调整开始");

        tempout=Change_Value(temprun);

        TextOut(0, 1, "运行时间完毕");

        Delay_1(300000);

        break;

    case 3:

        break;

    }

    ClearScreen();

}

uint8_t OnMainMenuOk(void * arg)
{

    TMenu * pMenu = (TMenu *)arg;

    if (pMenu) OnSelectMainMenu(pMenu->Current);

    return 0;

}

uint8_t OnMainMenuCancel(void * arg)
{

    TMenu * pMenu = (TMenu *)arg;

    if (pMenu) {

        CloseMenu(pMenu);

    }

    return 0;

}

uint8_t OnMainMenuOtherKey(void * arg)
{

    TMenu * pMenu = (TMenu *)arg;

    char key;

    if (pMenu) {

        key = pMenu->EventArg;

        OnSelectMainMenu(key - '1');

        DrawMenu(pMenu);

    }

    return 0;

}

void InitMenu()
{

    InitializeMenu(&MainMenu, MainMenuTitle, MainMenuItems, sizeof(MainMenuItems) / sizeof(char *));

    MainMenu.OnOk = OnMainMenuOk;

    MainMenu.OnCancel = OnMainMenuCancel;

    MainMenu.OnOtherKey = OnMainMenuOtherKey;

}


uint8_t Key_Value()
{

    uint8_t KeyTemp;

    KeyTemp=GetKey();

    switch(KeyTemp)

    {

    case(VK_ESCAPE):

        return(0);

        break;

    case(VK_ENTER):

        return(1);

        break;

    case(VK_UP):

        return(2);

        break;

    case(VK_DOWN):

        return(3);

        break;

    }

}

uint8_t Change_Value(uint8_t temp2)

{

    uint8_t temp3;

    temp3=temp2;

    while((Key_Value()!=0)&&(Key_Value()!=1))

    {

        if(Key_Value()==2)temp3++;

        else if(Key_Value()==3)temp3--;

        ClearLine(1);

        TextOut(0, 1, "调整状态%d", temp3);

    }

    if(Key_Value()==1)return(temp3);

    else if(Key_Value()==0) return(temp2);

}

void Delay_1(unsigned int m)

{

    while(m--);

}

void Run_Satus()

{

    ClearScreen();

    TextOut(0, 0, "室内温度%d",tempin);

    TextOut(0, 1, "室外温度%d",tempout);

    TextOut(0, 2, "运行时间%d",temprun);

    TextOut(0, 3, "ENTER 进入调整");

}

void tmain()

{

    tempin=30;

    tempout=30;

    temprun=30;

    uint8_t key;

    InitMenu();

    while(1)

    {

        Run_Satus();

        key=GetKey();

        if(key==VK_ENTER)ShowMenu(&MainMenu);

    }

}
}
}
