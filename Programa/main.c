/***************************************************************************
    Copyright (C) 2020   Walter Pirri

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

/* *****************************************************************************
 * Asignación de pines para PIC32MM0064GPL028 en DomTerm
 * 1    MCLR / Vpp                      Programación/Debug/Reset
 * 2    RA0                 out         Led Status
 * 3    RA1                 out         Led Modo
 * 4    RB0/PGED1                       Programacion/Debug
 * 5    RB1/PGEC1                       Programacion/Debug
 * 6    RB2                 
 * 7    RB3                 
 * 8    Vss                             Vss
 * 9    RA2/RP3             out         Exp-pin8
 * 10   RA3/RP4             out         Exp-pin5
 * 11   RB4/RP5             in          Entrada1
 * 12   RA4/RP6             
 * 13   Vdd                             +3v3
 * 14   RB5                 in          Entrada2
 * 15   RB6                 in          Entrada3
 * 16   RB7                 in          Entrada4
 * 17   RB8/SCK1            out         Exp-pin4
 * 18   RB9/SDO1            out         Exp-pin3
 * 19   RC9                 
 * 20   VCap                            Cap 100n + 10uF
 * 21   RB10/RP17           out         Exp-pin1
 * 22   RB11/RP18           out         Exp-pin7
 * 23   RB12/RP12           out         Exp-pin9
 * 24   RB13/RP13           out/oc      ESP-WROOM-02_Reset-pin15
 * 25   RB14/RP19           tx          U1TX -> ESP-WROOM-02_Rx-pin11
 * 26   RB15/RP10           rx          U1RX -> ESP-WROOM-02_Tx-pin12
 * 27   AVss                            Vss
 * 28   AVdd                            +3v3
 *
 * ****************************************************************************/

/**
  Section: Included Files
*/
#define _IN_MAIN_C_
#include "main.h"
#undef _IN_MAIN_C_

#include <string.h>
#include <stdio.h>
#include <sys/time.h>

bool main_flag_core_timer;

void LedControl( void )
{
    static char led_count = 0;
    
    if(++led_count >= 50) led_count = 0;
    if(led_count > 10)
    {
        if(g_wifi_info.status.wifi_conectado)
        {
            if((led_count-10) <= (g_wifi_info.status.wifi_conectado*2))
            {
                LED_STATUS_SetValue(led_count & 0x01);
            }
        }
        else
        {
            LED_STATUS_SetValue(0);
        }
    }
    if(g_wifi_info.status.wifi_mode)
    {
        if(led_count <= (g_wifi_info.status.wifi_mode*2))
        {
            LED_MODO_SetValue(led_count & 0x01);
        }
    }
    else
    {
        LED_MODO_SetValue(0);
    }
}
/*
                         Main application
 */
int main(void)
{
    main_flag_core_timer = false;

    SYSTEM_Initialize();
    ConfigInit();
    ConfigLoad();

    WiFiInit();
    HTTPInit();
    CTRLInit();
    
    while(1)
    {
        WATCHDOG_TimerClear();

        WiFiTask();
        HTTPTask();
        CTRLTask();
        
        if(main_flag_core_timer)
        {
            main_flag_core_timer = false;

            LedControl();
            ConfigTimer();
            WiFiTimer();
            HTTPTimer();
            CTRLTimer();
            
        }
    }
    return 1; 
}
/**
 End of File
*/

