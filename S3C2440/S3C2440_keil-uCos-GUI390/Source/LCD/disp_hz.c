#include "draw.h"
#include "disp_hz.h"



void HzDisp_tst(void)
{
    int  x, y;
    
    
    GUI_SetColor2(GREEN, BLACK); 
    
     // ��1����ʾ"�ķ���"
    x=60; y=20;
    GUI_PutHZ(x, y, (U8 *)TST_HZ1, 40, 40);
    x += 40;
    GUI_PutHZ(x, y, (U8 *)TST_HZ2, 40, 40);
    x += 40;
    GUI_PutHZ(x, y, (U8 *)TST_HZ3, 40, 40);
    
    
    GUI_SetColor2(RED, BLACK); 
   
    // ��2����ʾ"���ڽ���"
    x=40; y += 50;
    GUI_PutHZ(x, y, (U8 *)TST_HZ4, 40, 40);
    x += 40;
    GUI_PutHZ(x, y, (U8 *)TST_HZ5, 40, 40);
    x += 40;
    GUI_PutHZ(x, y, (U8 *)TST_HZ6, 40, 40);
    x += 40;
    GUI_PutHZ(x, y, (U8 *)TST_HZ7, 40, 40);
    
    GUI_SetColor2(BLUE, BLACK); 
    
    // ��3����ʾ"��Ƭ��"
    x=60; y += 50;
    GUI_PutHZ(x, y, (U8 *)TST_HZ8, 40, 40);
    x += 40;
    GUI_PutHZ(x, y, (U8 *)TST_HZ9, 40, 40);
    x += 40;
    GUI_PutHZ(x, y, (U8 *)TST_HZ10, 40, 40);
    
    
     GUI_SetColor2(YELLOW, BLACK);
     // ��4����ʾ"�򵥲���"
     x=40; y += 50;
    GUI_PutHZ(x, y, (U8 *)TST_HZ11, 40, 40);
    x += 40;
    GUI_PutHZ(x, y, (U8 *)TST_HZ8, 40, 40);
    x += 40;
    GUI_PutHZ(x, y, (U8 *)TST_HZ12, 40, 40);
     x += 40;
    GUI_PutHZ(x, y, (U8 *)TST_HZ13, 40, 40);


}    


