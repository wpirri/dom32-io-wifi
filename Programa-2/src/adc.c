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

#include "defines.h"
#include "config.h"
#include "control.h"
#include "log.h"

#define _DEBUG_ADC

typedef enum
{
    ADC_STM_NONE,
    ADC_STM_INIT,
    ADC_STM_SELECT,
    ADC_STM_START_SAMPLE,
    ADC_STM_WAIT_SAMPLE,
    ADC_STM_WAIT_CONV,
    ADC_STM_END_CONV,
} ADC_STM;

ADC_STM ADC_stm;
unsigned char ADC_channel;
unsigned char ADC_count_time;

int ADC_input1_value;
int ADC_input2_value;

void ADCInit(void)
{
    AD1CON2bits.VCFG = 0 ;
    AD1CON3bits.ADCS = 0xFF;
    AD1CON1bits.SSRC = 0;           // Set software trigger
    AD1CON3bits.SAMC = 0b10000;
    AD1CON1bits.FORM = 0b00;
    AD1CON2bits.SMPI = 0;
    
    ADC_stm = ADC_STM_NONE;
    ADC_count_time = 0;
    ADC_channel = 0;
}

void ADCTask(void)
{
    if(ADC_count_time) return;

    switch(ADC_stm)
    {
        case ADC_STM_NONE:
            if(ANSELBbits.ANSB3 || ANSELBbits.ANSB2 )
            {
                /* Necesito el módulo ADC */
                if(AD1CON1bits.ON) ADC_stm = ADC_STM_SELECT;
                else ADC_stm = ADC_STM_INIT;
            }
            else
            {
                /* No necesito el módulo ADC */
                if(AD1CON1bits.ON)
                {
#ifdef DEBUG_ADC
                    LogMessage("[ADCTask] ADC no necesario. Desactivando.");
#endif
                    AD1CON1bits.ON = 0;
                    ADC_count_time = 100;
                }
            }
            break;
        case ADC_STM_INIT:
#ifdef DEBUG_ADC            
            LogMessage("[ADCTask] ADC necesario. Activando.");
#endif
            AD1CON1bits.ON = 1;
            ADC_count_time = 100;
            ADC_stm++;
            break;
        case ADC_STM_SELECT:
            // Channel 1: RB3/AN11 (IO3)
            // Channel 2: RB2/AN4 (IO4)
            if(ADC_channel == 1)
            {
                if(ANSELBbits.ANSB2)    /* Channel 2 habilitado? */
                {
                    /* Si channel 2 está habilitado paso al channel 2, sino sigo en el 1 */
#ifdef DEBUG_ADC                    
                    LogMessage("[ADCTask] Channel 2 AN4 (IO4) Seleccionado");
#endif
                    AD1CHSbits.CH0SA = 4;
                    ADC_channel = 2;
                }
                else
                {
#ifdef DEBUG_ADC                    
                    LogMessage("[ADCTask] Channel 1 AN11 (IO3) Seleccionado");
#endif
                    AD1CHSbits.CH0SA = 11;
                }
            }
            else
            {
                if(ANSELBbits.ANSB3)    /* Channel 1 habilitado? */
                {
                    /* Si channel 1 está habilitado paso al channel 1, sino sigo en el 2 */
#ifdef DEBUG_ADC                    
                    LogMessage("[ADCTask] Channel 1 AN11 (IO3) Seleccionado");
#endif
                    AD1CHSbits.CH0SA = 11;
                    ADC_channel = 1;
                }
                else
                {
#ifdef DEBUG_ADC                    
                    LogMessage("[ADCTask] Channel 2 AN4 (IO4) Seleccionado");
#endif
                    AD1CHSbits.CH0SA = 4;
                }
            }             
            ADC_count_time = 10;
            ADC_stm++;
            break;
        case ADC_STM_START_SAMPLE:
#ifdef DEBUG_ADC            
            LogMessage("[ADCTask] Start SAMPLE");
#endif
            AD1CON1bits.SAMP = 1;
            ADC_count_time = 50;
            ADC_stm++;
            break;
        case ADC_STM_WAIT_SAMPLE:
            // Reset ADC interrupt flag
#ifdef DEBUG_ADC            
            LogMessage("[ADCTask] RESET INT flag");
#endif
            IFS1CLR = _IFS1_AD1IF_MASK;
            AD1CON1bits.SAMP = 0;
            ADC_stm++;
            break;
        case ADC_STM_WAIT_CONV:
            if(IFS1bits.AD1IF == 0) break;
#ifdef DEBUG_ADC            
            LogMessage("[ADCTask] End SAMPLE");
#endif
            ADC_stm++;
            break;
        case ADC_STM_END_CONV:
            switch(ADC_channel)
            {
                case 1:
                    ADC_input1_value = ADC1BUF0 * 3300l / 1023;
                    LogMessage("[ADCTask] AN11 (IO3) Val: %i", ADC_input1_value);
                    break;
                case 2:
                    ADC_input2_value = ADC1BUF0 * 3300l / 1023;
                    LogMessage("[ADCTask] AN4 (IO4) Val: %i", ADC_input2_value);
                    break;
            }
            ADC_count_time = 100;
            ADC_stm = ADC_STM_NONE;
            break;
    }
}

void ADCTimer(void)
{
    if(ADC_count_time) ADC_count_time--;
        
}

// Channel 1: RB3/AN11 (IO3)
// Channel 2: RB2/AN4 (IO4)
int ADCGet(int ch)
{
    switch(ch)
    {
        case 1:
            return ADC_input1_value;
        case 2:
            return ADC_input2_value;
        default:
            return (-1);
    }
}
