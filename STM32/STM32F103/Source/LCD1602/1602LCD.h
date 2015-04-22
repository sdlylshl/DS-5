//定义MCU与LCD的接口
/*
#define LCD_EN_PORT    PORTC
#define LCD_RW_PORT    PORTD
#define LCD_RS_PORT    PORTC
#define LCD_DATA_PORT  PORTD
#define LCD_DATA_DDR   DDRD
#define LCD_DATA_PIN   PIND

#define LCD_EN         0x20   //portc5         out
#define LCD_RW         0x08   //portd3         out/in
#define LCD_RS         0x10   //portc4         out
#define LCD_DATA       0xf0   //portd4/5/6/7   out
*/
#define SET_EN()	GPIO_SetBits(GPIOB, GPIO_Pin_13)
#define CLR_EN()	GPIO_ResetBits(GPIOB, GPIO_Pin_13)

#define SET_RW()	GPIO_SetBits(GPIOB, GPIO_Pin_15)
#define CLR_RW()	GPIO_ResetBits(GPIOB, GPIO_Pin_15)

#define SET_RS()	GPIO_SetBits(GPIOB, GPIO_Pin_14)
#define CLR_RS()	GPIO_ResetBits(GPIOB, GPIO_Pin_14)   

#define LCD_DATA_PORT  	GPIOC,GPIO_ReadOutputData(GPIOC)
/*--------------------------------------------------------------------------------------------------
Public function prototypes
--------------------------------------------------------------------------------------------------*/
void LCD_init         (void);
void LCD_en_write     (void);
void LCD_write_char   (unsigned command,unsigned data);
void LCD_wait_Ready   (void);
void LCD_set_xy       (unsigned char x, unsigned char y);
void LCD_write_string (unsigned char X,unsigned char Y,unsigned char *s);
void delay_nus        (unsigned int n);
void delay_nms        (unsigned int n);




