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
 * Asignaci�n de pines para PIC32MM0064GPL028 en DomTerm
 * 1    MCLR / Vpp                      Programaci�n/Debug/Reset
 * 2    RA0                 out         Led Status
 * 3    RA1                 out         Led Modo
 * 4    RB0/PGED1                       Programacion/Debug
 * 5    RB1/PGEC1                       Programacion/Debug
 * 6    RB2                 in          IO1
 * 7    RB3                 in          IO2
 * 8    Vss                             Vss
 * 9    RA2/RP3             UART2       Rx
 * 10   RA3/RP4             UART2       Tx
 * 11   RB4/RP5             in          IO3
 * 12   RA4/RP6             out         Led Aux
 * 13   Vdd                             +3v3
 * 14   RB5                 in          IO4
 * 15   RB6                 
 * 16   RB7                 in          IO5
 * 17   RB8/SCK1            in          IO6
 * 18   RB9/SDO1            out         OUT4
 * 19   RC9                 out         OUT3
 * 20   VCap                            Cap 100n + 10uF
 * 21   RB10/RP17           out         OUT2
 * 22   RB11/RP18           out         OUT1
 * 23   RB12/RP12           
 * 24   RB13/RP13           out/oc      RESET -> ESP-WROOM-02_Reset-pin15
 * 25   RB14/RP19           UART1       Tx    -> ESP-WROOM-02_Rx-pin11
 * 26   RB15/RP10           UART1       Rx    -> ESP-WROOM-02_Tx-pin12
 * 27   AVss                            Vss
 * 28   AVdd                            +3v3
 *
 * ****************************************************************************/

/**
  Section: Included Files
*/
#include <string.h>
#include <stdio.h>
#include <sys/time.h>

#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/watchdog.h"
#include "config.h"
#include "http.h"
#include "wifi.h"
#include "control.h"
#include "wiegand.h"
#include "dhtxx.h"

bool main_flag_core_timer;
int main_flag_core_timer_div_count;
int main_flag_dhtxx_timer_count;
char led_aux_count;


void LedControl( void )
{
    static char led_count = 0;
    
    if(led_aux_count)
    {
        LED_Aux_SetHigh();
        led_aux_count--;
        if(led_aux_count == 0)
        {
            LED_Aux_SetLow();
        }
    }
    if(++led_count >= 50)
    {
        LED_Status_SetLow();
        LED_Modo_SetLow();
        led_count = 0;
    }
    if(g_wifi_info.status.wifi_ok)
    {
        if(led_count > 10)
        {
            if(g_wifi_info.status.wifi_conectado)
            {
                if((led_count-10) <= (g_wifi_info.status.wifi_conectado*2))
                {
                    LED_Status_SetValue(led_count & 0x01);
                }
            }
            else
            {
                LED_Status_SetLow();
            }
        }
        else
        {
            LED_Status_SetLow();
        }
        if(g_wifi_info.status.wifi_mode)
        {
            if(led_count <= (g_wifi_info.status.wifi_mode*2))
            {
                LED_Modo_SetValue(led_count & 0x01);
            }
        }
        else
        {
            LED_Modo_SetLow();
        }
    }
    else
    {
        LED_Status_SetValue(led_count & 0x01);
        LED_Modo_SetValue(!(led_count & 0x01));
    }
    
    if(g_wifi_info.status.host_conectado)
    {
        LED_Aux_SetValue(led_count & 0x02);
    }
    else
    {
        LED_Aux_SetLow();
    }
    
    
}
/*
                         Main application
 */
int main(void)
{
    main_flag_core_timer = false;
    main_flag_core_timer_div_count = 0;
    main_flag_dhtxx_timer_count = 0;

    SYSTEM_Initialize();
    
    LogMessage("\r\n");
    LogMessage("MAIN: Compilacion " __DATE__ " " __TIME__ ".\r\n");
    LogMessage("MAIN: Compilador " __VERSION__ ".\r\n");
    LogMessage("MAIN: Procesador PIC32MM256GPM028.\r\n");
    
    ConfigInit();
    ConfigLoad();

    CTRLInit();
    WiFiInit();
    HTTPInit();
    WGND_Init();
    DHTxxInit();
    
    LogMessage("MAIN: Init Ok.\r\n");
    
    while(1)
    {
        WATCHDOG_TimerClear();

        WiFiTask();
        HTTPTask();
        CTRLTask();
        if(IS_WIEGAND_ENABLE) WGND_Task();
        
        if(main_flag_core_timer)
        {
            main_flag_core_timer = false;
            main_flag_core_timer_div_count++;
            if(main_flag_core_timer_div_count >= 10)
            {
                main_flag_core_timer_div_count = 0;
                /* Timer 10 veces por segundo - 100ms */
                LedControl();
                ConfigTimer();
                WiFiTimer();
                HTTPTimer();
                CTRLTimer();
                if(IS_WIEGAND_ENABLE) WGND_Timer();
                if(IS_DHT2x_ENABLE)
                {
                    main_flag_dhtxx_timer_count++;
                    if(main_flag_dhtxx_timer_count > 300)
                    {
                        main_flag_dhtxx_timer_count = 0;
                        DHTxxGetData();
                    }

                }
                
            }
            /* Timer 100 veces por segundo - 10ms */
            //DHT2xTimer();
        }
    }
    return 1; 
}
/**
 End of File
*/

