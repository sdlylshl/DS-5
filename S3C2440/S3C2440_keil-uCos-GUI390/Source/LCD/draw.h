
#include "def1.h"

// ������ɫ��������(���������ݽṹ)
#define  TCOLOR				U16
   
// ����LCM��������                 
#define  GUI_LCM_XMAX		320							/* ����Һ��x��ĵ��� */
#define  GUI_LCM_YMAX		240							/* ����Һ��y��ĵ��� */

// ������ɫ�궨��
#define   BLACK		0x0000      /* ��ɫ��    0,   0,   0 */
#define   NAVY		0x000F      /* ����ɫ��  0,   0, 128 */
#define   DGREEN	0x03E0		/* ����ɫ��  0, 128,   0 */
#define   DCYAN		0x03EF		/* ����ɫ��  0, 128, 128 */
#define   MAROON	0x7800		/* ���ɫ��128,   0,   0 */
#define   PURPLE	0x780F		/* ��ɫ��  128,   0, 128 */
#define   OLIVE		0x7BE0 	    /* ����̣�128, 128,   0 */
#define   LGRAY		0xC618	    /* �Ұ�ɫ��192, 192, 192 */
#define   DGRAY		0x7BEF		/* ���ɫ��128, 128, 128 */
#define   BLUE		0x001F		/* ��ɫ��    0,   0, 255 */
#define   GREEN		0x07E0		/* ��ɫ��    0, 255,   0 */
#define   CYAN	    0x07FF 		/* ��ɫ��    0, 255, 255 */
#define   RED		0xF800		/* ��ɫ��  255,   0,   0 */
#define   MAGENTA	0xF81F		/* Ʒ�죺  255,   0, 255 */
#define   YELLOW	0xFFE0		/* ��ɫ��  255, 255, 0   */
#define   WHITE		0xFFFF      /* ��ɫ��  255, 255, 255 */

#define  CENTER_X	160
#define  CENTER_Y	120




void  GUI_PutHZ(U32 x, U32 y, U8 *dat, U8 hno, U8 lno);
void  GUI_SetColor2(U16 color1, U16 color2);
void  CCircleDemo(void);

U16  random(U16 seed);

