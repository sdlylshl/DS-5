#ifndef __LCD12864P_H_
#define __LCD12864P_H_	

#include "..\LCD12864_config.h"

#include "version.h"

void LCD12864_InitPort(void); //硬件端口初始化
void NOP(void); //延时函数
u8 LCD12864_busy(void); //检测忙状态
void LCD12684_Wcmd(u8 dat); //写指令
void LCD12684_Wdat(u8 dat); //写数据
void LCD12864_Init(void); //液晶初始化
void LCD12864_Clr(void); //	清屏
void LCD12864_Pos(u8 x, u8 y); //设置显示位置
void LCD12864_PhotoDis(u8 *bmp); //	显示图片
void LCD_ShowStringXY(u8 x, u8 y, char *p); //	显示字符串
void LCD_ShowString(u8 *p);
void LCD_ShowChar(u8 p);
//**********************************************************************************************************************
#endif	

