/***************************************************************************
    Copyright (C) 2024   Walter Pirri

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
#include "io.h"

#include <xc.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

#include "defines.h"
#include "wifi/wifi.h"
#include "http.h"


void IOInit( void )
{
    /****************************************************************************
     * Interrupt On Change: negative
     ***************************************************************************/
    CNEN1Bbits.CNIE1B7 = 1;    //Pin : RB7
    CNEN1Bbits.CNIE1B8 = 1;    //Pin : RB8
    /****************************************************************************
     * Interrupt On Change: flag
     ***************************************************************************/
    CNFBbits.CNFB7 = 0;    //Pin : RB7
    CNFBbits.CNFB8 = 0;    //Pin : RB8
    /****************************************************************************
     * Interrupt On Change: config
     ***************************************************************************/
    CNCONBbits.CNSTYLE = 1;    //Config for PORTB
    CNCONBbits.ON = 1;    //Config for PORTB

    /****************************************************************************
     * Interrupt On Change: Interrupt Enable
     ***************************************************************************/
    IFS0CLR= 1 << _IFS0_CNBIF_POSITION; //Clear CNBI interrupt flag
    IEC0bits.CNBIE = 1; //Enable CNBI interrupt
    
    LED_Azul_SetDigitalOutput();
    LED_Verde_SetDigitalOutput();
    LED_Ambar_SetDigitalOutput();
    
    IOConfig();
   
}

/* Interrupt service routine for the CNBI interrupt. */
void __attribute__ ((vector(_CHANGE_NOTICE_B_VECTOR), interrupt(IPL1SOFT))) _CHANGE_NOTICE_B( void )
{
#ifdef ___REVISAR___
    
    if(IFS0bits.CNBIF == 1)
    {
        WGND_IntData( CNFBbits.CNFB8, CNFBbits.CNFB7 );
        if(CNFBbits.CNFB8 == 1)
        {
            CNFBCLR = 0x100;  //Clear CNFBbits.CNFB8
        }
        
        if(CNFBbits.CNFB7 == 1)
        {
            CNFBCLR = 0x80;  //Clear CNFBbits.CNFB7
        }
        
        // Clear the flag
        IFS0CLR= 1 << _IFS0_CNBIF_POSITION; // Clear IFS0bits.CNBIF
    }
    
#endif /* ___REVISAR___ */
}

void IOConfig( void )
{

#ifdef ___REVISAR___
    
    /* Entrada 1 - RB5 */
    g_control_io.stat1 = false;
    g_control_io.change1 = false;
    if(g_wifi_info.f.config.io_config & 0x01)
    {
        TRISBbits.TRISB5 = 1;
        CNPUBbits.CNPUB5 = 1;
    }
    else
    {
        TRISBbits.TRISB5 = 0;
        CNPUBbits.CNPUB5 = 0;
    }
    /* Entrada 2 - RB4 */
    g_control_io.stat2 = false;
    g_control_io.change2 = false;
    if(g_wifi_info.f.config.io_config & 0x02)
    {
        TRISBbits.TRISB4 = 1;
        CNPUBbits.CNPUB4 = 1;
    }
    else
    {
        TRISBbits.TRISB4 = 0;
        CNPUBbits.CNPUB4 = 0;
    }
    /* Entrada 3 - RB3 / AN11 */
    g_control_io.stat3 = false;
    g_control_io.change3 = false;
    if(g_wifi_info.f.config.io_config & 0x04)
    {
        TRISBbits.TRISB3 = 1;
        if(g_wifi_info.f.config.an_config & 0x04)
        {
            ANSELBbits.ANSB3 = 1;   // AN11
            CNPUBbits.CNPUB3 = 0;
        }
        else
        {
            ANSELBbits.ANSB3 = 0;   // AN11
            CNPUBbits.CNPUB3 = 1;
        }
    }
    else
    {
        TRISBbits.TRISB3 = 0;
        CNPUBbits.CNPUB3 = 0;
    }
    /* Entrada 4 - RB2 */
    g_control_io.stat4 = false;
    g_control_io.change4 = false;
    if(g_wifi_info.f.config.io_config & 0x08)
    {
        TRISBbits.TRISB2 = 1;
        if(g_wifi_info.f.config.an_config & 0x08)
        {
            ANSELBbits.ANSB2 = 1;   // AN4
            CNPUBbits.CNPUB2 = 0;
        }
        else
        {
            ANSELBbits.ANSB2 = 0;   // AN4
            CNPUBbits.CNPUB2 = 1;
        }
    }
    else
    {
        TRISBbits.TRISB2 = 0;
        CNPUBbits.CNPUB2 = 0;
    }

    if(g_wifi_info.f.config.enable_wiegand)
    {
        /* Entrada 5 - RB7 */
        g_wifi_info.f.config.an_config &= (0x10^0xFF);
        g_wifi_info.f.config.io_config |= 0x10;
        g_control_io.stat5 = false;
        g_control_io.change5 = false;
        TRISBbits.TRISB7 = 1;
        CNPUBbits.CNPUB7 = 1;
        /* Entrada 6 - RB8 */
        g_wifi_info.f.config.an_config &= (0x20^0xFF);
        g_wifi_info.f.config.io_config |= 0x20;
        g_control_io.stat6 = false;
        g_control_io.change6 = false;
        TRISBbits.TRISB8 = 1;
        CNPUBbits.CNPUB8 = 1;

        /* Traido de interrupt_manager.c --> */
        //    CNBI: PORT B Change Notification
        //    Priority: 1
        IPC2bits.CNBIP = 1;
        //    Sub Priority: 0
        IPC2bits.CNBIS = 0;
        /* <-- Traido de interrupt_manager.c */
        /* Traido de pin_manager.c --> */
        /****************************************************************************
         * Interrupt On Change: Interrupt Enable
         ***************************************************************************/
        IFS0CLR= 1 << _IFS0_CNBIF_POSITION; //Clear CNBI interrupt flag
        IEC0bits.CNBIE = 1; //Enable CNBI interrupt
        /* <-- Traido de pin_manager.c */
    }
    else
    {
        /* Entrada 5 - RB7 */
        g_control_io.stat5 = false;
        g_control_io.change5 = false;
        if(g_wifi_info.f.config.io_config & 0x10)
        {
            TRISBbits.TRISB7 = 1;
            CNPUBbits.CNPUB7 = 1;
        }
        else
        {
            TRISBbits.TRISB7 = 0;
            CNPUBbits.CNPUB7 = 0;
        }
        /* Entrada 6 - RB8 */
        g_control_io.stat6 = false;
        g_control_io.change6 = false;
        if(g_wifi_info.f.config.io_config & 0x20)
        {
            TRISBbits.TRISB8 = 1;
            CNPUBbits.CNPUB8 = 1;
        }
        else
        {
            TRISBbits.TRISB8 = 0;
            CNPUBbits.CNPUB8 = 0;
        }
        
        //Disable CNBI interrupt
        IEC0bits.CNBIE = 0;
    }
    
    ADCConfig();
    
#endif /* ___REVISAR___ */
    
}

void IOLedControl( void )
{
    static int io_led_control_count = 0;
    
    if(--io_led_control_count > 0) return;
    io_led_control_count = 20;
    
    if(WiFiConectado() == 0)
    {
        LED_Verde_Toggle();
    }

    if(HttpOnLine())
    {
        LED_Ambar_SetHigh();
    }
    else if(WiFiConectado()) 
    {
        LED_Ambar_Toggle();
    }
    else
    {
        LED_Ambar_SetLow();
    }
    
    if(WiFiStaConectado())
    {
        LED_Azul_SetHigh();
    }
    else if(WiFiMode() == WIFI_MODE_AP)
    {
        LED_Azul_Toggle();
    }
    else
    {
        LED_Azul_SetLow();
    }
}
