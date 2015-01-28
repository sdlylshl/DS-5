

#ifndef MCC_H
#define	MCC_H
#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include "interrupt_manager.h"

#include "pin_manager.h"
#include "tmr0.h"
//#include "pwm1.h"


//#include "eusart.h"
//#include "hlvd.h"
//#include "spi1.h"
//#include "memory.h"
//#include "adc.h"

#define _XTAL_FREQ  1000000

void SYSTEM_Initialize(void);

void OSCILLATOR_Initialize(void);


#endif
