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
#define _IN_CONTROL_C_
#include "control.h"
#undef _IN_CONTROL_C_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/rtcc.h"
#include "config.h"
#include "http.h"
#include "adc.h"
#include "clog.h"

unsigned int g_control_init_complete;
unsigned int g_control_update_firmware;
unsigned int g_ctrl_count_reset;

unsigned int g_control_count_in1;
unsigned int g_control_count_in2;
unsigned int g_control_count_in3;
unsigned int g_control_count_in4;
unsigned int g_control_count_in5;
unsigned int g_control_count_in6;

void CTRLNotificarStatus( int time )
{
    if(time == 0)
    {
        g_wifi_info.status.control_notificar = g_wifi_info.f.config.report;
    }
    else if(time < g_wifi_info.status.control_notificar || g_wifi_info.status.control_notificar == 0)
    {
        g_wifi_info.status.control_notificar = time;
    }
    LogMessage("Notificar en %i segundos", g_wifi_info.status.control_notificar/10);
}

void CTRLInit( void )
{
    g_wifi_info.status.control_notificar = 0;
    g_control_init_complete = 0;
    g_ctrl_count_reset = 0;
    g_control_update_firmware = 0;
    
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
    
    LogMessage("TRISB:   0x%04X", TRISB);
    LogMessage("CNPUB:   0x%04X", CNPUB);
    LogMessage("HTTPS:   %s", (g_wifi_info.f.config.enable_https)?"on":"off");
    LogMessage("WIEGAND: %s", (g_wifi_info.f.config.enable_wiegand)?"on":"off");
    
    g_control_count_in1 = 0;
    g_control_count_in2 = 0;
    g_control_count_in3 = 0;
    g_control_count_in4 = 0;
    g_control_count_in5 = 0;
    g_control_count_in6 = 0;

/* Primer notificacion */
    CTRLNotificarStatus(100);
}

void CTRLTask( void )
{
    static unsigned char ctrl_stm = 0;
    
    ctrl_stm++;
    switch(ctrl_stm)
    {
        case 1:
            if(g_wifi_info.f.config.io_config & 0x01)  /* Input */
            {
                if(IO1_GetValue() && g_control_count_in1 < DELAY_INPUT) g_control_count_in1++;
                else if( !IO1_GetValue() && g_control_count_in1 > 0) g_control_count_in1--;
                    
                if( (g_control_count_in1 == 0 && g_control_io.stat1 == 1) ||
                        (g_control_count_in1 == DELAY_INPUT && g_control_io.stat1 == 0) )
                {
                    g_control_io.stat1 = (g_control_count_in1)?1:0;
                    if(g_control_init_complete)
                    {
                        g_control_io.change1 = true;
                        CTRLNotificarStatus(5);
                    }
                }
            }
            else                        /* Output */
            {
                if(IO1_GetValue() != g_control_io.stat1)
                {
                    IO1_SetValue(g_control_io.stat1);
                    if(g_control_init_complete && g_control_io.pulse1 == 0)
                    {
                        g_control_io.change1 = true;
                        CTRLNotificarStatus(1);
                    }
                }
            }
            break;
        case 2:
            if(g_wifi_info.f.config.io_config & 0x02)  /* Input */
            {
                if(IO2_GetValue() && g_control_count_in2 < DELAY_INPUT) g_control_count_in2++;
                else if( !IO2_GetValue() && g_control_count_in2 > 0) g_control_count_in2--;
                    
                if( (g_control_count_in2 == 0 && g_control_io.stat2 == 1) ||
                        (g_control_count_in2 == DELAY_INPUT && g_control_io.stat2 == 0) )
                {
                    g_control_io.stat2 = (g_control_count_in2)?1:0;
                    if(g_control_init_complete)
                    {
                        g_control_io.change2 = true;
                        CTRLNotificarStatus(5);
                    }
                }
            }
            else                        /* Output */
            {
                if(IO2_GetValue() != g_control_io.stat2)
                {
                    IO2_SetValue(g_control_io.stat2);
                    if(g_control_init_complete && g_control_io.pulse2 == 0)
                    {
                        g_control_io.change2 = true;
                        CTRLNotificarStatus(1);
                    }
                }
            }
            break;
        case 3:
            if(g_wifi_info.f.config.an_config & 0x04) break; /* Analog */
            if(g_wifi_info.f.config.io_config & 0x04)  /* Input */
            {
                if(IO3_GetValue() && g_control_count_in3 < DELAY_INPUT) g_control_count_in3++;
                else if( !IO3_GetValue() && g_control_count_in3 > 0) g_control_count_in3--;

                if( (g_control_count_in3 == 0 && g_control_io.stat3 == 1) ||
                        (g_control_count_in3 == DELAY_INPUT && g_control_io.stat3 == 0) )
                {
                    g_control_io.stat3 = (g_control_count_in3)?1:0;
                    if(g_control_init_complete)
                    {
                        g_control_io.change3 = true;
                        CTRLNotificarStatus(5);
                    }
                }
            }
            else                        /* Output */
            {
                if(IO3_GetValue() != g_control_io.stat3)
                {
                    IO3_SetValue(g_control_io.stat3);
                    if(g_control_init_complete && g_control_io.pulse3 == 0)
                    {
                        g_control_io.change3 = true;
                        CTRLNotificarStatus(1);
                    }
                }
            }
            break;
        case 4:
            if(g_wifi_info.f.config.an_config & 0x08) break; /* Analog */
            if(g_wifi_info.f.config.io_config & 0x08)  /* Input */
            {
                if(IO4_GetValue() && g_control_count_in4 < DELAY_INPUT) g_control_count_in4++;
                else if( !IO4_GetValue() && g_control_count_in4 > 0) g_control_count_in4--;

                if( (g_control_count_in4 == 0 && g_control_io.stat4 == 1) ||
                        (g_control_count_in4 == DELAY_INPUT && g_control_io.stat4 == 0) )
                {
                    g_control_io.stat4 = (g_control_count_in4)?1:0;
                    if(g_control_init_complete)
                    {
                        g_control_io.change4 = true;
                        CTRLNotificarStatus(5);
                    }
                }
            }
            else                        /* Output */
            {
                if(IO4_GetValue() != g_control_io.stat4)
                {
                    IO4_SetValue(g_control_io.stat4);
                    if(g_control_init_complete && g_control_io.pulse4 == 0)
                    {
                        g_control_io.change4 = true;
                        CTRLNotificarStatus(1);
                    }
                }
            }
            break;
        case 5:
            if(g_wifi_info.f.config.enable_wiegand) break;
            if(g_wifi_info.f.config.io_config & 0x10)  /* Input */
            {
                if(IO5_GetValue() && g_control_count_in5 < DELAY_INPUT) g_control_count_in5++;
                else if( !IO5_GetValue() && g_control_count_in5 > 0) g_control_count_in5--;
                    
                if( (g_control_count_in5 == 0 && g_control_io.stat5 == 1) ||
                        (g_control_count_in5 == DELAY_INPUT && g_control_io.stat5 == 0) )
                {
                    g_control_io.stat5 = (g_control_count_in5)?1:0;
                    if(g_control_init_complete)
                    {
                        g_control_io.change5 = true;
                        CTRLNotificarStatus(5);
                    }
                }
            }
            else                        /* Output */
            {
                if(IO5_GetValue() != g_control_io.stat5)
                {
                    IO5_SetValue(g_control_io.stat5);
                    if(g_control_init_complete && g_control_io.pulse5 == 0)
                    {
                        g_control_io.change5 = true;
                        CTRLNotificarStatus(1);
                    }
                }
            }
            break;
        case 6:
            if(g_wifi_info.f.config.enable_wiegand) break;
            if(g_wifi_info.f.config.io_config & 0x20)  /* Input */
            {
                if(IO6_GetValue() && g_control_count_in6 < DELAY_INPUT) g_control_count_in6++;
                else if( !IO6_GetValue() && g_control_count_in6 > 0) g_control_count_in6--;
                    
                if( (g_control_count_in6 == 0 && g_control_io.stat6 == 1) ||
                        (g_control_count_in6 == DELAY_INPUT && g_control_io.stat6 == 0) )
                {
                    g_control_io.stat6 = (g_control_count_in6)?1:0;
                    if(g_control_init_complete)
                    {
                        g_control_io.change6 = true;
                        CTRLNotificarStatus(5);
                    }
                }
            }
            else                        /* Output */
            {
                if(IO6_GetValue() != g_control_io.stat6)
                {
                    IO6_SetValue(g_control_io.stat6);
                    if(g_control_init_complete && g_control_io.pulse6 == 0)
                    {
                        g_control_io.change6 = true;
                        CTRLNotificarStatus(1);
                    }
                }
            }
            break;
        case 7:
            if(OUT1_GetValue() != g_control_out.stat1)
            {
                OUT1_SetValue(g_control_out.stat1);
                if(g_control_init_complete && g_control_out.pulse1 == 0)
                {
                    g_control_out.change1 = true;
                    CTRLNotificarStatus(1);
                }
            }
            break;
        case 8:
            if(OUT2_GetValue() != g_control_out.stat2)
            {
                OUT2_SetValue(g_control_out.stat2);
                if(g_control_init_complete && g_control_out.pulse2 == 0)
                {
                    g_control_out.change2 = true;
                    CTRLNotificarStatus(1);
                }
            }
            break;
        case 9:
            if(OUT3_GetValue() != g_control_out.stat3)
            {
                OUT3_SetValue(g_control_out.stat3);
                if(g_control_init_complete && g_control_out.pulse3 == 0)
                {
                    g_control_out.change3 = true;
                    CTRLNotificarStatus(1);
                }
            }
            break;
        case 10:
            if(OUT4_GetValue() != g_control_out.stat4)
            {
                OUT4_SetValue(g_control_out.stat4);
                if(g_control_init_complete && g_control_out.pulse4 == 0)
                {
                    g_control_out.change4 = true;
                    CTRLNotificarStatus(1);
                }
            }
            break;
        default:
            ctrl_stm = 0;
            break;
    }
    
}

/* Se llama 10 veces por segundo - 100ms */
void CTRLTimer( void )
{
    if(g_wifi_info.status.host_conectado == 0 && g_wifi_info.status.control_notificar > (g_wifi_info.f.config.report/10))
    {
        g_wifi_info.status.control_notificar = (g_wifi_info.f.config.report/10);
    }

    if( g_wifi_info.status.control_notificar > 0 &&
        g_wifi_info.status.wifi_mode == 1 &&
        g_wifi_info.status.wifi_conectado_ok )
    {
        g_wifi_info.status.control_notificar--;
        if(g_wifi_info.status.control_notificar == 0)
        {
            /* Levanto el flag para que
             * en adelante los cambios cuenten */
            g_control_init_complete = 1;
            HTTPNotificarStatus();
            /* programo un reintento, si hay respuesta Ok luego se extiende el tiempo */
            g_wifi_info.status.control_notificar = (g_wifi_info.f.config.report/10);
        }
    }
    
    /* Control de pulso */
    if(g_control_io.pulse1)
    {
        g_control_io.pulse1--;
        if(g_control_io.pulse1 == 0)
        {
            g_control_io.stat1 = 0;
            LogMessage("Fin pulso IO1");
            CTRLNotificarStatus(10);
        }
    }
    if(g_control_io.pulse2)
    {
        g_control_io.pulse2--;
        if(g_control_io.pulse2 == 0)
        {
            g_control_io.stat2 = 0;
            LogMessage("Fin pulso IO2");
            CTRLNotificarStatus(10);
        }
    }
    if(g_control_io.pulse3)
    {
        g_control_io.pulse3--;
        if(g_control_io.pulse3 == 0)
        {
            g_control_io.stat3 = 0;
            LogMessage("Fin pulso IO3");
            CTRLNotificarStatus(10);
        }
    }
    if(g_control_io.pulse4)
    {
        g_control_io.pulse4--;
        if(g_control_io.pulse4 == 0)
        {
            g_control_io.stat4 = 0;
            LogMessage("Fin pulso IO4");
            CTRLNotificarStatus(10);
        }
    }
    if(g_control_io.pulse5)
    {
        g_control_io.pulse5--;
        if(g_control_io.pulse5 == 0)
        {
            g_control_io.stat5 = 0;
            LogMessage("Fin pulso IO5");
            CTRLNotificarStatus(10);
        }
    }
    if(g_control_io.pulse6)
    {
        g_control_io.pulse6--;
        if(g_control_io.pulse6 == 0)
        {
            g_control_io.stat6 = 0;
            LogMessage("Fin pulso IO6");
            CTRLNotificarStatus(10);
        }
    }
    if(g_control_out.pulse1)
    {
        g_control_out.pulse1--;
        if(g_control_out.pulse1 == 0)
        {
            g_control_out.stat1 = 0;
            LogMessage("Fin pulso OUT1");
            CTRLNotificarStatus(10);
        }
    }
    if(g_control_out.pulse2)
    {
        g_control_out.pulse2--;
        if(g_control_out.pulse2 == 0)
        {
            g_control_out.stat2 = 0;
            LogMessage("Fin pulso OUT2");
            CTRLNotificarStatus(10);
        }
    }
    if(g_control_out.pulse3)
    {
        g_control_out.pulse3--;
        if(g_control_out.pulse3 == 0)
        {
            g_control_out.stat3 = 0;
            LogMessage("Fin pulso OUT3");
            CTRLNotificarStatus(10);
        }
    }
    if(g_control_out.pulse4)
    {
        g_control_out.pulse4--;
        if(g_control_out.pulse4 == 0)
        {
            g_control_out.stat4 = 0;
            LogMessage("Fin pulso OUT4");
            CTRLNotificarStatus(10);
        }
    }

    /* Control de reset */
    if(g_ctrl_count_reset)
    {
        g_ctrl_count_reset--;
        if(g_ctrl_count_reset == 0)
        {
            ConfigSave();
            LogMessage("[CTRL] RESET.");
            /* The following code illustrates a software Reset */
            // assume interrupts are disabled
            // assume the DMA controller is suspended
            // assume the device is locked
            /* perform a system unlock sequence */
            // starting critical sequence
            SYSKEY = 0x00000000; //write invalid key to force lock
            SYSKEY = 0xAA996655; //write key1 to SYSKEY
            SYSKEY = 0x556699AA; //write key2 to SYSKEY
            /* set SWRST bit to arm reset */
            RSWRSTSET = 1;
            /* read RSWRST register to trigger reset */
            unsigned int dummy; dummy = RSWRST;
            /* prevent any unwanted code execution until reset occurs*/
            while(dummy | 1);
        }
    }
    
    if(g_control_update_firmware && g_wifi_info.status.flag_sd_ok)
    {
        g_control_update_firmware--;
        if(g_control_update_firmware == 0)
        {
            if(HTTPGetFile(DOWNLOAD_PATH"/"FIRMWARE_FILENAME, NULL) <= 0 )
            {
                g_control_update_firmware = 10;
            }
        }
    }
    
}

void CTRLSetIO(char* in)
{
    char tmp[64];
    
    LogMessage("[CTRLSetIO] %s", in);
    if( !(g_wifi_info.f.config.io_config & 0x01))
    {
        if(HTTPGetFormData(in, "PULSE-IO1", tmp))
        {
            g_control_io.stat1 = 1;
            g_control_io.pulse1 = atoi(tmp)*10;
            LogMessage("IO1: Pulso %s s", tmp);
        }
        else if(HTTPGetFormData(in, "IO1", tmp))
        {
            g_control_io.stat1 = atoi(tmp);
            LogMessage("IO1: %s", tmp);
        }
    }

    if( !(g_wifi_info.f.config.io_config & 0x02))
    {
        if(HTTPGetFormData(in, "PULSE-IO2", tmp))
        {
            g_control_io.stat2 = 1;
            g_control_io.pulse2 = atoi(tmp)*10;
            LogMessage("IO2: Pulso %s s", tmp);
        }
        else if(HTTPGetFormData(in, "IO2", tmp))
        {
            g_control_io.stat2 = atoi(tmp);
            LogMessage("IO2: %s", tmp);
        }
    }

    if( !(g_wifi_info.f.config.io_config & 0x04))
    {
        if(HTTPGetFormData(in, "PULSE-IO3", tmp))
        {
            g_control_io.stat3 = 1;
            g_control_io.pulse3 = atoi(tmp)*10;
            LogMessage("IO3: Pulso %s s", tmp);
        }
        else if(HTTPGetFormData(in, "IO3", tmp))
        {
            g_control_io.stat3 = atoi(tmp);
            LogMessage("IO3: %s", tmp);
        }
    }
    
    if( !(g_wifi_info.f.config.io_config & 0x08))
    {
        if(HTTPGetFormData(in, "PULSE-IO4", tmp))
        {
            g_control_io.stat4 = 1;
            g_control_io.pulse4 = atoi(tmp)*10;
            LogMessage("IO4: Pulso %s s", tmp);
        }
        else if(HTTPGetFormData(in, "IO4", tmp))
        {
            g_control_io.stat4 = atoi(tmp);
            LogMessage("IO4: %s", tmp);
        }
    }
    
    if( !(g_wifi_info.f.config.io_config & 0x10))
    {
        if(HTTPGetFormData(in, "PULSE-IO5", tmp))
        {
            g_control_io.stat5 = 1;
            g_control_io.pulse5 = atoi(tmp)*10;
            LogMessage("IO5: Pulso %s s", tmp);
        }
        else if(HTTPGetFormData(in, "IO5", tmp))
        {
            g_control_io.stat5 = atoi(tmp);
            LogMessage("IO5: %s", tmp);
        }
    }

    if( !(g_wifi_info.f.config.io_config & 0x20))
    {
        if(HTTPGetFormData(in, "PULSE-IO6", tmp))
        {
            g_control_io.stat6 = 1;
            g_control_io.pulse6 = atoi(tmp)*10;
            LogMessage("IO6: Pulso %s s", tmp);
        }
        else if(HTTPGetFormData(in, "IO6", tmp))
        {
            g_control_io.stat6 = atoi(tmp);
            LogMessage("IO6: %s", tmp);
        }
    }

    if(HTTPGetFormData(in, "PULSE-OUT1", tmp))
    {
        g_control_out.stat1 = 1;
        g_control_out.pulse1 = atoi(tmp)*10;
        LogMessage("OUT1: Pulso %s s", tmp);
    }
    else if(HTTPGetFormData(in, "OUT1", tmp))
    {
        g_control_out.stat1 = atoi(tmp);
        LogMessage("OUT1: %s", tmp);
    }
    
    if(HTTPGetFormData(in, "PULSE-OUT2", tmp))
    {
        g_control_out.stat2 = 1;
        g_control_out.pulse2 = atoi(tmp)*10;
        LogMessage("OUT2: Pulso %s s", tmp);
    }
    else if(HTTPGetFormData(in, "OUT2", tmp))
    {
        g_control_out.stat2 = atoi(tmp);
        LogMessage("OUT2: %s", tmp);
    }
    
    if(HTTPGetFormData(in, "PULSE-OUT3", tmp))
    {
        g_control_out.stat3 = 1;
        g_control_out.pulse3 = atoi(tmp)*10;
        LogMessage("OUT3: Pulso %s s", tmp);
    }
    else if(HTTPGetFormData(in, "OUT3", tmp))
    {
        g_control_out.stat3 = atoi(tmp);
        LogMessage("OUT3: %s", tmp);
    }
    
    if(HTTPGetFormData(in, "PULSE-OUT4", tmp))
    {
        g_control_out.stat4 = 1;
        g_control_out.pulse4 = atoi(tmp)*10;
        LogMessage("OUT4: Pulso %s s", tmp);
    }
    else if(HTTPGetFormData(in, "OUT4", tmp))
    {
        g_control_out.stat4 = atoi(tmp);
        LogMessage("OUT4: %s", tmp);
    }
}

void CTRLCompareIO(char* in)
{
    char tmp[10];
    bool notificar = false;
    
    LogMessage("[CTRLCompareIO] %s", in);

    /* Para las entradas / salidas que vengan informadas */
    if(HTTPGetFormData(in, "IO1", tmp))
    {
        /* Si hay diferencia entre lo informado y el estado actual */
        if(atoi(tmp) != g_control_io.stat1)
        {
            g_control_io.change1 = 1;
            notificar = true;
            if( !(g_wifi_info.f.config.io_config & 0x01))
            {
                /* Si es una salida corrijo el estado */
                g_control_io.stat1 = atoi(tmp);
            }
        }
    }
    /* Para las entradas / salidas que vengan informadas */
    if(HTTPGetFormData(in, "IO2", tmp))
    {
        /* Si hay diferencia entre lo informado y el estado actual */
        if(atoi(tmp) != g_control_io.stat2)
        {
            g_control_io.change2 = 1;
            notificar = true;
            if( !(g_wifi_info.f.config.io_config & 0x02))
            {
                /* Si es una salida corrijo el estado */
                g_control_io.stat2 = atoi(tmp);
            }
        }
    }
    /* Para las entradas / salidas que vengan informadas */
    if(HTTPGetFormData(in, "IO3", tmp))
    {
        /* Si hay diferencia entre lo informado y el estado actual */
        if(atoi(tmp) != g_control_io.stat3)
        {
            g_control_io.change3 = 1;
            notificar = true;
            if( !(g_wifi_info.f.config.io_config & 0x04))
            {
                /* Si es una salida corrijo el estado */
                g_control_io.stat3 = atoi(tmp);
            }
        }
    }
    /* Para las entradas / salidas que vengan informadas */
    if(HTTPGetFormData(in, "IO4", tmp))
    {
        /* Si hay diferencia entre lo informado y el estado actual */
        if(atoi(tmp) != g_control_io.stat4)
        {
            g_control_io.change4 = 1;
            notificar = true;
            if( !(g_wifi_info.f.config.io_config & 0x08))
            {
                /* Si es una salida corrijo el estado */
                g_control_io.stat4 = atoi(tmp);
            }
        }
    }
    /* Para las entradas / salidas que vengan informadas */
    if(HTTPGetFormData(in, "IO5", tmp))
    {
        /* Si hay diferencia entre lo informado y el estado actual */
        if(atoi(tmp) != g_control_io.stat5)
        {
            g_control_io.change5 = 1;
            notificar = true;
            if( !(g_wifi_info.f.config.io_config & 0x10))
            {
                /* Si es una salida corrijo el estado */
                g_control_io.stat5 = atoi(tmp);
            }
        }
    }
    /* Para las entradas / salidas que vengan informadas */
    if(HTTPGetFormData(in, "IO6", tmp))
    {
        /* Si hay diferencia entre lo informado y el estado actual */
        if(atoi(tmp) != g_control_io.stat6)
        {
            g_control_io.change6 = 1;
            notificar = true;
            if( !(g_wifi_info.f.config.io_config & 0x20))
            {
                /* Si es una salida corrijo el estado */
                g_control_io.stat6 = atoi(tmp);
            }
        }
    }
    /* Para las salidas que vengan informadas */
    if(HTTPGetFormData(in, "OUT1", tmp))
    {
        /* Si hay diferencia entre lo informado y el estado actual */
        if(atoi(tmp) != g_control_out.stat1)
        {
            g_control_out.change1 = 1;
            notificar = true;
            g_control_out.stat1 = atoi(tmp);
        }
    }
    /* Para las salidas que vengan informadas */
    if(HTTPGetFormData(in, "OUT2", tmp))
    {
        /* Si hay diferencia entre lo informado y el estado actual */
        if(atoi(tmp) != g_control_out.stat2)
        {
            g_control_out.change2 = 1;
            notificar = true;
            g_control_out.stat2 = atoi(tmp);
        }
    }
    /* Para las salidas que vengan informadas */
    if(HTTPGetFormData(in, "OUT3", tmp))
    {
        /* Si hay diferencia entre lo informado y el estado actual */
        if(atoi(tmp) != g_control_out.stat3)
        {
            g_control_out.change3 = 1;
            notificar = true;
            g_control_out.stat3 = atoi(tmp);
        }
    }
    /* Para las salidas que vengan informadas */
    if(HTTPGetFormData(in, "OUT4", tmp))
    {
        /* Si hay diferencia entre lo informado y el estado actual */
        if(atoi(tmp) != g_control_out.stat4)
        {
            g_control_out.change4 = 1;
            notificar = true;
            g_control_out.stat4 = atoi(tmp);
        }
    }

    if(notificar) { CTRLNotificarStatus(5); }
}

void CTRLSwitchIO(char* in)
{
    char tmp[10];
    
    LogMessage("[CTRLSwitchIO] %s", in);
    if( !(g_wifi_info.f.config.io_config & 0x01))
    {
        if(HTTPGetFormData(in, "IO1", tmp))
        {
            if( tmp[0] == '1' )
            {
                g_control_io.stat1 = (g_control_io.stat1)?0:1;
                LogMessage("IO1: %d", g_control_io.stat1);
            }
        }
    }

    if( !(g_wifi_info.f.config.io_config & 0x02))
    {
        if(HTTPGetFormData(in, "IO2", tmp))
        {
            if( tmp[0] == '1' )
            {
                g_control_io.stat2 = (g_control_io.stat2)?0:1;
                LogMessage("IO2: %d", g_control_io.stat2);
            }
        }
    }

    if( !(g_wifi_info.f.config.io_config & 0x04))
    {
        if(HTTPGetFormData(in, "IO3", tmp))
        {
            if( tmp[0] == '1' )
            {
                g_control_io.stat3 = (g_control_io.stat3)?0:1;
                LogMessage("IO3: %d", g_control_io.stat3);
            }
        }
    }
    
    if( !(g_wifi_info.f.config.io_config & 0x08))
    {
        if(HTTPGetFormData(in, "IO4", tmp))
        {
            if( tmp[0] == '1' )
            {
                g_control_io.stat4 = (g_control_io.stat4)?0:1;
                LogMessage("IO4: %d", g_control_io.stat4);
            }
        }
    }
    
    if( !(g_wifi_info.f.config.io_config & 0x10))
    {
        if(HTTPGetFormData(in, "IO5", tmp))
        {
            if( tmp[0] == '1' )
            {
                g_control_io.stat5 = (g_control_io.stat5)?0:1;
                LogMessage("IO5: %d", g_control_io.stat5);
            }
        }
    }
    
    if( !(g_wifi_info.f.config.io_config & 0x20))
    {
        if(HTTPGetFormData(in, "IO6", tmp))
        {
            if( tmp[0] == '1' )
            {
                g_control_io.stat6 = (g_control_io.stat6)?0:1;
                LogMessage("IO6: %d", g_control_io.stat6);
            }
        }
    }
    
    if(HTTPGetFormData(in, "OUT1", tmp))
    {
        if( tmp[0] == '1' )
        {
            g_control_out.stat1 = (g_control_out.stat1)?0:1;
            LogMessage("OUT1: %d", g_control_out.stat1);
        }
    }
    
    if(HTTPGetFormData(in, "OUT2", tmp))
    {
        if( tmp[0] == '1' )
        {
            g_control_out.stat2 = (g_control_out.stat2)?0:1;
            LogMessage("OUT2: %d", g_control_out.stat2);
        }
    }
    
    if(HTTPGetFormData(in, "OUT3", tmp))
    {
        if( tmp[0] == '1' )
        {
            g_control_out.stat3 = (g_control_out.stat3)?0:1;
            LogMessage("OUT3: %d", g_control_out.stat3);
        }
    }
    
    if(HTTPGetFormData(in, "OUT4", tmp))
    {
        if( tmp[0] == '1' )
        {
            g_control_out.stat4 = (g_control_out.stat4)?0:1;
            LogMessage("OUT4: %d", g_control_out.stat4);
        }
    }
}

void CTRLConfIO(char* in)
{
    char tmp[64];
    bool change = false;
    bool reset = false;
    
    LogMessage("[CTRLConfIO] %s", in);

    if(HTTPGetFormData(in, "IO1", tmp))
    {
        if( !strcmp(tmp, "in") )
        {
            g_wifi_info.f.config.an_config &= (0x01^0xFF);
            g_wifi_info.f.config.io_config |= 0x01;
            change = true;
        }
        else if( !strcmp(tmp, "out") )
        {
            g_wifi_info.f.config.an_config &= (0x01^0xFF);
            g_wifi_info.f.config.io_config &= (0x01^0xFF);
            change = true;
        }
        LogMessage("IO1: %s", tmp);
    }
    
    if(HTTPGetFormData(in, "IO2", tmp))
    {
        if( !strcmp(tmp, "in") )
        {
            g_wifi_info.f.config.an_config &= (0x02^0xFF);
            g_wifi_info.f.config.io_config |= 0x02;
            change = true;
        }
        else if( !strcmp(tmp, "out") )
        {
            g_wifi_info.f.config.an_config &= (0x02^0xFF);
            g_wifi_info.f.config.io_config &= (0x02^0xFF);
            change = true;
        }
        LogMessage("IO2: %s", tmp);
    }
    
    if(HTTPGetFormData(in, "IO3", tmp))
    {
        if( !strcmp(tmp, "in") )
        {
            g_wifi_info.f.config.an_config &= (0x04^0xFF);
            g_wifi_info.f.config.io_config |= 0x04;
            change = true;
        }
        else if( !strcmp(tmp, "out") )
        {
            g_wifi_info.f.config.an_config &= (0x04^0xFF);
            g_wifi_info.f.config.io_config &= (0x04^0xFF);
            change = true;
        }
        else if( !strcmp(tmp, "ana") )
        {
            g_wifi_info.f.config.an_config |= 0x04;
            g_wifi_info.f.config.io_config |= 0x04;
            change = true;
        }
        LogMessage("IO3: %s", tmp);
    }
    
    if(HTTPGetFormData(in, "IO4", tmp))
    {
        if( !strcmp(tmp, "in") )
        {
            g_wifi_info.f.config.an_config &= (0x08^0xFF);
            g_wifi_info.f.config.io_config |= 0x08;
            change = true;
        }
        else if( !strcmp(tmp, "out") )
        {
            g_wifi_info.f.config.an_config &= (0x08^0xFF);
            g_wifi_info.f.config.io_config &= (0x08^0xFF);
            change = true;
        }
        else if( !strcmp(tmp, "ana") )
        {
            g_wifi_info.f.config.an_config |= 0x08;
            g_wifi_info.f.config.io_config |= 0x08;
            change = true;
        }
        LogMessage("IO4: %s", tmp);
    }
    
    if(HTTPGetFormData(in, "IO5", tmp))
    {
        if( !strcmp(tmp, "in") )
        {
            g_wifi_info.f.config.an_config &= (0x10^0xFF);
            g_wifi_info.f.config.io_config |= 0x10;
            change = true;
        }
        else if( !strcmp(tmp, "out") )
        {
            g_wifi_info.f.config.an_config &= (0x10^0xFF);
            g_wifi_info.f.config.io_config &= (0x10^0xFF);
            change = true;
        }
        LogMessage("IO5: %s", tmp);
    }
    
    if(HTTPGetFormData(in, "IO6", tmp))
    {
        if( !strcmp(tmp, "in") )
        {
            g_wifi_info.f.config.an_config &= (0x20^0xFF);
            g_wifi_info.f.config.io_config |= 0x20;
            change = true;
        }
        else if( !strcmp(tmp, "out") )
        {
            g_wifi_info.f.config.an_config &= (0x20^0xFF);
            g_wifi_info.f.config.io_config &= (0x20^0xFF);
            change = true;
        }
        LogMessage("IO6: %s", tmp);
    }

    if(HTTPGetFormData(in, "WIEGAND", tmp))
    {
        change = true;
        if( !strcmp(tmp, "yes") || !strcmp(tmp, "1") )
        {
            if(g_wifi_info.f.config.enable_wiegand == 0) reset = true;
            g_wifi_info.f.config.enable_wiegand = 1;
        }
        else
        {
            if(g_wifi_info.f.config.enable_wiegand == 1) reset = true;
            g_wifi_info.f.config.enable_wiegand = 0;
        }
        LogMessage("WIEGAND: %s", tmp);
    }

    if(HTTPGetFormData(in, "HTTPS", tmp))
    {
        if(g_wifi_info.status.wifi_ssl_capable)
        {
            change = true;
            
            if( !strcmp(tmp, "yes") || !strcmp(tmp, "1") )
            {
                if(g_wifi_info.f.config.enable_https == 0) reset = true;
                g_wifi_info.f.config.enable_https = 1;
            }
            else
            {
                if(g_wifi_info.f.config.enable_https == 1) reset = true;
                g_wifi_info.f.config.enable_https = 0;
            }
            LogMessage("HTTPS: %s", tmp);
        }
        else
        {
            LogMessage("HTTPS no posible");
        }
    }
    
    if(change)
    {
        g_wifi_info.f.config.io_config_default = 0;
        g_wifi_info.status.save_config = 150;
    }

    if(reset)
    {
        CTRLReset();
    }
}

void CTRLConfSys(char* in)
{
    char tmp[64];
    bool change = false;
    struct tm tm_time;

    LogMessage("[CTRLConfSys] %s", in);

    if(HTTPGetFormData(in, "UPDATE-FW", tmp))
    {
        LogMessage("********** Actualizar Firmware **********");
        /* Actualizar firmware en 5 seg */
        g_control_update_firmware = 50;
    }
    
    if(HTTPGetFormData(in, "report", tmp))
    {
        change = true;
        g_wifi_info.f.config.report = atoi(tmp) * 10;
        if(g_wifi_info.f.config.report == 0)
        {
            g_wifi_info.f.config.report = 600;
        }
        LogMessage("Keep Alive: %is", g_wifi_info.f.config.report/10);
    }
    
    if(HTTPGetFormData(in, "REPORT", tmp))
    {
        change = true;
        g_wifi_info.f.config.report = atoi(tmp) * 10;
        if(g_wifi_info.f.config.report == 0)
        {
            g_wifi_info.f.config.report = 600;
        }
        LogMessage("Keep Alive: %is", g_wifi_info.f.config.report/10);
    }
    
    if(HTTPGetFormData(in, "TIME", tmp))
    {
        LogMessage("TIME: %s", tmp);
        /*             1111111111
         *   01234567890123456789
         tmp YYYY/MM/DD hh:mm:ss
         */
        if(tmp[4] == '/' && tmp[7] == '/' && tmp[10] == ' ' && 
                tmp[13] == ':' && tmp[16] == ':')
        {
            tmp[4] = 0;
            tmp[7] = 0;
            tmp[10] = 0;
            tmp[13] = 0;
            tmp[16] = 0;
            tmp[19] = 0;
            
            tm_time.tm_year = atoi(&tmp[0]) - 1900;
            tm_time.tm_mon = atoi(&tmp[5]);
            tm_time.tm_mday = atoi(&tmp[8]);
            tm_time.tm_hour = atoi(&tmp[11]);
            tm_time.tm_min = atoi(&tmp[14]);
            tm_time.tm_sec = atoi(&tmp[17]);

            RTCC_TimeSet(&tm_time);
        }
        else
        {
            LogMessage("[CTRLConfSys] Error de formato para TIME");
        }
    }
    
    if(change)
    {
        g_wifi_info.f.config.io_config_default = 0;
        g_wifi_info.status.save_config = 150;
    }
}

void CTRLReset( void )
{
    /* reset en 10 seg */
    LogMessage("[CTRL] RESET solicitado.");
    g_ctrl_count_reset = 30;
}
