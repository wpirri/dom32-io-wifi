/* 
 * File:   delay.h
 * Author: walter
 *
 * Created on 13 de agosto de 2018, 20:48
 */
#include <xc.h>

#include "delay.h"
#include "config.h"

void __attribute__((optimize("-O0"))) __delay_ms(unsigned long ms)
{
    unsigned long   count;

    while (ms--)
    {
        count = ((GetInstructionClock()/(unsigned long)8/(unsigned long)1000) - (unsigned long)5);
        while (count--);
    }
    _nop();
}

void __attribute__((optimize("-O0"))) __delay_us(unsigned long us)
{   
    /* Para clock de 8MHz */
    us += (us * 17 / 100);
    while (us--);
}