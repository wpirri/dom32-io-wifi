/***************************************************************************
    Copyright (C) 2022   Walter Pirri

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 ***************************************************************************/
#include "adc.h"

#include <xc.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "config.h"
#include "control.h"
#include "clog.h"

typedef enum
{
    ADC_STM_NONE,
    ADC_STM_WAIT,
    ADC_STM_START_SAMPLE,
    ADC_STM_WAIT_SAMPLE,
    ADC_STM_WAIT_CONV,
    ADC_STM_END_CONV,
} ADC_STM;

ADC_STM ADC_count_stm;
unsigned char ADC_count_channel;
unsigned char ADC_count_time;

void ADCInit(void)
{
    AD1CON2bits.VCFG = 0 ;
    AD1CON3bits.ADCS = 0xFF;
    AD1CON1bits.SSRC = 0;           // Set software trigger
    AD1CON3bits.SAMC = 0b10000;
    AD1CON1bits.FORM = 0b00;
    AD1CON2bits.SMPI = 0;
    
    ADC_count_stm = 0;
    ADC_count_channel = 0;
    ADC_count_time = 0;
}


void ADCConfig(void)
{
    if(ANSELBbits.ANSB3 ||
       ANSELBbits.ANSB2 ||
       ANSELAbits.ANSA2  )
    {
        AD1CON1bits.ON = 1;
    }
    else
    {
        AD1CON1bits.ON = 0;
    }
    ADC_count_time = 100;
}

void ADCTask(void)
{
    if(ANSELBbits.ANSB3 == 0 &&
       ANSELBbits.ANSB2 == 0 &&
       ANSELAbits.ANSA2  ) return;

    if(ADC_count_time) return;

    if(ADC_count_channel == 0)
        ADC_count_channel++;

    switch(ADC_count_stm)
    {
        case ADC_STM_NONE:
            switch(ADC_count_channel)
            {
                case 1:
                    if(ANSELBbits.ANSB3) ADC_count_stm++;
                    else ADC_count_channel++;
                    break;
                case 2:
                    if(ANSELBbits.ANSB2) ADC_count_stm++;
                    else ADC_count_channel++;
                    break;
                case 3:
                    if(ANSELAbits.ANSA2) ADC_count_stm++;
                    else ADC_count_channel++;
                    break;
                default:
                    ADC_count_channel = 0;
                    ADC_count_time = 100;
                    break;
            }
            break;
        case ADC_STM_WAIT:
            switch(ADC_count_channel)
            {
                case 1:
                    // Select RB3/AN11 input
                    AD1CHSbits.CH0SA = 11;
                    break;
                case 2:
                    // Select RB2/AN4 input
                    AD1CHSbits.CH0SA = 4;
                    break;
                case 3:
                    // Select RA2/AN5 input
                    AD1CHSbits.CH0SA = 5;
                    break;
            }
            ADC_count_stm++;
            break;
        case ADC_STM_START_SAMPLE:
            AD1CON1bits.SAMP = 1;
            ADC_count_time = 5;
            ADC_count_stm++;
            break;
        case ADC_STM_WAIT_SAMPLE:
            // Reset ADC interrupt flag
            IFS1CLR = _IFS1_AD1IF_MASK;
            AD1CON1bits.SAMP = 0;
            ADC_count_stm++;
            break;
        case ADC_STM_WAIT_CONV:
            if(IFS1bits.AD1IF == 0) break;
            ADC_count_stm++;
            break;
        case ADC_STM_END_CONV:
            switch(ADC_count_channel)
            {
                case 1:
                    g_control_io.stat3 = ADC1BUF0 * 3300l / 1023;
                    break;
                case 2:
                    g_control_io.stat4 = ADC1BUF0 * 3300l / 1023;
                    break;
                case 3:

                    break;
            }
            ADC_count_time = 10;
            ADC_count_channel++;
            ADC_count_stm = ADC_STM_NONE;
            break;
    }


    
}

void ADCTimer(void)
{
    if(ADC_count_time) ADC_count_time--;
        
}
