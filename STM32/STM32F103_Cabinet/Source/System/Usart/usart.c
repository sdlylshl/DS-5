
#include "usart.h"
#include <stdio.h>
#include <stdarg.h>

/*
 * 函数名：fputc
 * 描述  ：重定向c库函数printf到USART3
 * 输入  ：无
 * 输出  ：无
 * 调用  ：由printf调用
 */
//int fputc(int ch, FILE *f)
//{
//    /* 将Printf内容发往串口 */
//    USART_SendData(USART1, (unsigned char) ch);
//    while (!(USART1->SR & USART_FLAG_TXE));

//    return (ch);
//}

/*
 * 函数名：itoa
 * 描述  ：将整形数据转换成字符串
 * 输入  ：-radix =10 表示10进制，其他结果为0
 *         -value 要转换的整形数
 *         -buf 转换后的字符串
 *         -radix = 10
 * 输出  ：无
 * 返回  ：无
 * 调用  ：被USART3_printf()调用
 */
static char *itoa(int value, char *string, int radix)
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;
    }

    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;

    return string;

} /* NCL_Itoa */

/*
 * 函数名：USART3_printf
 * 描述  ：格式化输出，类似于C库中的printf，但这里没有用到C库
 * 输入  ：-USARTx 串口通道，这里只用到了串口2，即USART3
 *		     -Data   要发送到串口的内容的指针
 *			   -...    其他参数
 * 输出  ：无
 * 返回  ：无
 * 调用  ：外部调用
 *         典型应用USART3_printf( USART3, "\r\n this is a demo \r\n" );
 *            		 USART3_printf( USART3, "\r\n %d \r\n", i );
 *            		 USART3_printf( USART3, "\r\n %s \r\n", j );
 */
void USARTx_printf(USART_TypeDef* USARTx, int8_t *Data,...)
{
    const char *s;
    int d;
    char buf[36];

    va_list ap;
    va_start(ap, Data);

    while ( *Data != 0)     // 判断是否到达字符串结束符
    {
        if ( *Data == 0x5c )  //'\'
        {
            switch ( *++Data )
            {
            case 'r':							          //回车符
                USART_SendData(USARTx, 0x0d);
                Data ++;
                break;

            case 'n':							          //换行符
                USART_SendData(USARTx, 0x0a);
                Data ++;
                break;

            default:
                Data ++;
                break;
            }
        }
        else if ( *Data == '%')
        {
            //
            switch ( *++Data )
            {
            case 's':										  //字符串
                s = va_arg(ap, const char *);
                for ( ; *s; s++)
                {
                    USART_SendData(USARTx,*s);
                    while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
                }
                Data++;
                break;

            case 'd':										//十进制
                d = va_arg(ap, int);
                itoa(d, buf, 10);
                for (s = buf; *s; s++)
                {
                    USART_SendData(USARTx,*s);
                    while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
                }
                Data++;
                break;
            default:
                Data++;
                break;
            }
        } /* end of else if */
        else USART_SendData(USARTx, *Data++);
        while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );

    }
}
/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
