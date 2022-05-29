/* 
 * File:   delay.h
 * Author: walter
 *
 * Created on 13 de agosto de 2018, 20:48
 */
#include <xc.h>

#include "delay.h"
#include "config.h"

void __attribute__((optimize("-O0"))) __delay_ms(DWORD ms)
{
    DWORD   count;

    while (ms--)
    {
        count = ((GetInstructionClock()/(WORD)8/(WORD)1000) - (WORD)5);
        while (count--);
    }
    _nop();
}

void __attribute__((optimize("-O0"))) __delay_us(DWORD us)
{   
    /* Para clock de 8MHz */
    us += (us * 17 / 100);
    while (us--);
}