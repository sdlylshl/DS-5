/* 
 * File:   main.h
 * Author: zwf
 *
 * Created on 2015年1月10日, 下午1:28
 */

#ifndef MAIN_H
#define	MAIN_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <xc.h>
#include "pic18f2680.h"

#include  "timer.h"




#define LED       LATBbits.LATB5
#define TEST_PIN  LATBbits.LATB3

#define  PWM_ON   CCPR1L = PR2/2
#define  PWM_OFF  CCPR1L = 0



typedef struct __AFE
{
    uint8_t  AFE_Buf[256];
//    uint8_t  AFE_Len;
    uint8_t  AFE_Send_Flag;
    uint32_t  Count;
    uint32_t  S_Count;


}_AFE;




#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_H */

