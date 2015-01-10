#ifndef __USART_H
#define	__USART_H

#include "version.h"

extern void USARTx_Init(USART_TypeDef* USARTx);
extern void USARTx_printf(USART_TypeDef* USARTx, int8_t *Data,...);

#endif /* __USART_H */
