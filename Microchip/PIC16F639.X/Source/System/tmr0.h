

#ifndef _TMR0_H
#define _TMR0_H
#include <xc.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif

extern volatile uint32_t timer0;


void TMR0_Initialize(void);

uint8_t TMR0_Read8bitTimer(void);

void TMR0_Write8bitTimer(uint8_t timerVal);

void TMR0_Reload8bit(void);

uint32_t TMR0_GetDistanceTime(uint32_t pretime);

uint32_t TMR0_GetCurrentTime(void);

void TMR0_ISR(void);


#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif

#endif
    