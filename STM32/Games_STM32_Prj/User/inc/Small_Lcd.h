#ifndef  _SMALL_LCD_H
#define  _SMALL_LCD_H
#include "Global.h"
#include "stm32f10x.h"
#include "delay.h"
#include "type.h"
#include "Target.h"

#define  LCD_MAX_RowNum		1		// 最大行号
#define  LCD_MAX_Rows		2		// 最多显示行数
#define  LCD_MAX_Cols		7		// 显示列数, 全角列
#define  LCD_POS_LastEnd	0xFF	// 接着上一次输出位置显示时的输入函数参数

#define  LCD_LED_On()		X_LCD_CTRL_OutBits( BIT(LCD_PIN_LEDA)|BIT(LCD_PIN_LEDK), BIT(LCD_PIN_LEDA) )
#define  LCD_LED_Shut()		X_LCD_CTRL_OutBits( BIT(LCD_PIN_LEDA)|BIT(LCD_PIN_LEDK), BIT(LCD_PIN_LEDK) )

extern void Init_Spi2(void);
extern void SPI2_Senddata(uint8 TxData);
extern void LCD_Init(void);
extern void LCD_SdString(const uint8 *temp);
extern void LCD_SdIntData(uint16 uiData, uint8 ucLen, uint8 ucRow, uint8 ucCol);
extern void LCD_SdStringPos(const uint8 *Str,uint8 col,uint8  row);
extern void LCD_Clear(void);
extern void  LCD_BlkWhite(void);
extern void  LCD_BlkWhite_Off(void);
extern void LCD_Shut(void);
extern void LCD_Disp_on(void);
extern void LCD_Send(uint8 senddata);
extern void LCD_SdCmd(uint8 scmd);
extern void LCD_SdData(uint8 DData);
extern void LCD_DispSetCursor(uint8 LineNum, uint8 ColumnNum);
extern void LCD_SetCurs_BlkWh(uint8 ucLine,uint8 ucColumn);


extern t_err DbgLcdPrintf(uint8 row, uint8 col, const char *fmt, ...);

#ifdef DEBUG
#define  _DbgLcdPrintf_(printf_exp)	DbgLcdPrintf printf_exp
#else
#define  _DbgLcdPrintf_
#endif

#endif
