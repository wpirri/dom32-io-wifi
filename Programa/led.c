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
#include "led.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "mcc_generated_files/pin_manager.h"

int led_count_led1;
int led_count_led2;
int led_count_led3;

int led_status_led1;
int led_status_led2;
int led_status_led3;

void LedInit( void )
{
    led_count_led1 = 0;
    led_count_led2 = 0;
    led_count_led3 = 0;
    led_status_led1 = 0;
    led_status_led2 = 0;
    led_status_led3 = 0;
}

void LedTask( void )
{
    
}

void LedTimer( void )
{
    if(++led_count_led1 >= 21) led_count_led1 = 0;
    if( !(led_count_led1&1) && led_count_led1 && led_status_led1 && ((led_count_led1/2)<=led_status_led1) )
        LED1_SetHigh();
    else
        LED1_SetLow();

    if(++led_count_led2 >= 21) led_count_led2 = 0;
    if(  (led_count_led2&1) && led_count_led2 && led_status_led2 && ((led_count_led2/2)<=led_status_led2) )
        LED2_SetHigh();
    else
        LED2_SetLow();

    if(++led_count_led3 >= 21) led_count_led3 = 0;
    if( !(led_count_led3&1) && led_count_led3 && led_status_led3 && ((led_count_led3/2)<=led_status_led3) )
        LED3_SetHigh();
    else
        LED3_SetLow();
}

void Led1(int status)
{
    led_status_led1 = status;
}

void Led2(int status)
{
    led_status_led2 = status;
}

void Led3(int status)
{
    led_status_led3 = status;
}

