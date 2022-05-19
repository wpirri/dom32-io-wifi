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
#include "config.h"
#include "http.h"

unsigned int g_control_notificar;

unsigned int g_control_count_in1;
unsigned int g_control_count_in2;
unsigned int g_control_count_in3;
unsigned int g_control_count_in4;
unsigned int g_control_count_in5;
unsigned int g_control_count_in6;

void CTRLNotificarStatus( int time )
{
    if(time < g_control_notificar || g_control_notificar == 0)
    {
        g_control_notificar = time;
    }
}

void CTRLInit( void )
{
    g_control_notificar = 0;
    /* Entrada 1 - RB5 */
    g_control_io.ana1 = (g_wifi_info.f.config.an_config & 0x01)?1:0;
    g_control_io.dir1 = (g_wifi_info.f.config.io_config & 0x01)?1:0;
    g_control_io.stat1 = false;
    if(g_control_io.dir1)   {TRISBSET = (1 << 5); CNPUBSET = (1 << 5);}
    else                    {TRISBCLR = (1 << 5); CNPUBCLR = (1 << 5);}
    /* Entrada 2 - RB4 */
    g_control_io.ana2 = (g_wifi_info.f.config.an_config & 0x02)?1:0;
    g_control_io.dir2 = (g_wifi_info.f.config.io_config & 0x02)?1:0;
    g_control_io.stat2 = false;
    if(g_control_io.dir2)   {TRISBSET = (1 << 4); CNPUBSET = (1 << 4);}
    else                    {TRISBCLR = (1 << 4); CNPUBCLR = (1 << 4);}
    /* Entrada 3 - RB3 */
    g_control_io.ana3 = (g_wifi_info.f.config.an_config & 0x04)?1:0;
    g_control_io.dir3 = (g_wifi_info.f.config.io_config & 0x04)?1:0;
    g_control_io.stat3 = false;
    if(g_control_io.dir3)   {TRISBSET = (1 << 3); CNPUBSET = (1 << 3);}
    else                    {TRISBCLR = (1 << 3); CNPUBCLR = (1 << 3);}
    /* Entrada 4 - RB2 */
    g_control_io.ana4 = (g_wifi_info.f.config.an_config & 0x08)?1:0;
    g_control_io.dir4 = (g_wifi_info.f.config.io_config & 0x08)?1:0;
    g_control_io.stat4 = false;
    if(g_control_io.dir4)   {TRISBSET = (1 << 2); CNPUBSET = (1 << 2);}
    else                    {TRISBCLR = (1 << 2); CNPUBCLR = (1 << 2);}
    /* Entrada 5 - RB7 */
    g_control_io.ana5 = (g_wifi_info.f.config.an_config & 0x10)?1:0;
    g_control_io.dir5 = (g_wifi_info.f.config.io_config & 0x10)?1:0;
    g_control_io.stat5 = false;
    if(g_control_io.dir5)   {TRISBSET = (1 << 7); CNPUBSET = (1 << 7);}
    else                    {TRISBCLR = (1 << 7); CNPUBCLR = (1 << 7);}
    /* Entrada 6 - RB8 */
    g_control_io.ana6 = (g_wifi_info.f.config.an_config & 0x20)?1:0;
    g_control_io.dir6 = (g_wifi_info.f.config.io_config & 0x20)?1:0;
    g_control_io.stat6 = false;
    if(g_control_io.dir6)   {TRISBSET = (1 << 8); CNPUBSET = (1 << 8);}
    else                    {TRISBCLR = (1 << 8); CNPUBCLR = (1 << 8);}

    LogMessage("TRISB: 0x%04X\r\n", TRISB);
    LogMessage("CNPUB: 0x%04X\r\n", CNPUB);
    
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
            if(g_control_io.ana1)       /* Anal�gico */
            {
                
            }
            else if(g_control_io.dir1)  /* Input */
            {
                if(IO1_GetValue() && g_control_count_in1 < DELAY_INPUT) g_control_count_in1++;
                else if( !IO1_GetValue() && g_control_count_in1 > 0) g_control_count_in1--;
                    
                if( (g_control_count_in1 == 0 && g_control_io.stat1 == 1) ||
                        (g_control_count_in1 == DELAY_INPUT && g_control_io.stat1 == 0) )
                {
                    g_control_io.stat1 = (g_control_count_in1)?1:0;
                    g_control_io.change1 = true;
                    CTRLNotificarStatus(5);
                }
            }
            else                        /* Output */
            {
                if(IO1_GetValue() != g_control_io.stat1)
                {
                    IO1_SetValue(g_control_io.stat1);
                    g_control_io.change1 = true;
                    //CTRLNotificarStatus(10);
                }
            }
            break;
        case 2:
            if(g_control_io.ana2)       /* Anal�gico */
            {
                
            }
            else if(g_control_io.dir2)  /* Input */
            {
                if(IO2_GetValue() && g_control_count_in2 < DELAY_INPUT) g_control_count_in2++;
                else if( !IO2_GetValue() && g_control_count_in2 > 0) g_control_count_in2--;
                    
                if( (g_control_count_in2 == 0 && g_control_io.stat2 == 1) ||
                        (g_control_count_in2 == DELAY_INPUT && g_control_io.stat2 == 0) )
                {
                    g_control_io.stat2 = (g_control_count_in2)?1:0;
                    g_control_io.change2 = true;
                    CTRLNotificarStatus(5);
                }
            }
            else                        /* Output */
            {
                if(IO2_GetValue() != g_control_io.stat2)
                {
                    IO2_SetValue(g_control_io.stat2);
                    g_control_io.change2 = true;
                    //CTRLNotificarStatus(10);
                }
            }
            break;
        case 3:
            if(g_control_io.ana3)       /* Anal�gico */
            {
                
            }
            else if(g_control_io.dir3)  /* Input */
            {
                if(IO3_GetValue() && g_control_count_in3 < DELAY_INPUT) g_control_count_in3++;
                else if( !IO3_GetValue() && g_control_count_in3 > 0) g_control_count_in3--;
                    
                if( (g_control_count_in3 == 0 && g_control_io.stat3 == 1) ||
                        (g_control_count_in3 == DELAY_INPUT && g_control_io.stat3 == 0) )
                {
                    g_control_io.stat3 = (g_control_count_in3)?1:0;
                    g_control_io.change3 = true;
                    CTRLNotificarStatus(5);
                }
            }
            else                        /* Output */
            {
                if(IO3_GetValue() != g_control_io.stat3)
                {
                    IO3_SetValue(g_control_io.stat3);
                    g_control_io.change3 = true;
                    //CTRLNotificarStatus(10);
                }
            }
            break;
        case 4:
            if(g_control_io.ana4)       /* Anal�gico */
            {
                
            }
            else if(g_control_io.dir4)  /* Input */
            {
                if(IO4_GetValue() && g_control_count_in4 < DELAY_INPUT) g_control_count_in4++;
                else if( !IO4_GetValue() && g_control_count_in4 > 0) g_control_count_in4--;
                    
                if( (g_control_count_in4 == 0 && g_control_io.stat4 == 1) ||
                        (g_control_count_in4 == DELAY_INPUT && g_control_io.stat4 == 0) )
                {
                    g_control_io.stat4 = (g_control_count_in4)?1:0;
                    g_control_io.change4 = true;
                    CTRLNotificarStatus(5);
                }
            }
            else                        /* Output */
            {
                if(IO4_GetValue() != g_control_io.stat4)
                {
                    IO4_SetValue(g_control_io.stat4);
                    g_control_io.change4 = true;
                    //CTRLNotificarStatus(10);
                }
            }
            break;
        case 5:
            if(g_control_io.ana5)       /* Anal�gico */
            {
                
            }
            else if(g_control_io.dir5)  /* Input */
            {
                if(IO5_GetValue() && g_control_count_in5 < DELAY_INPUT) g_control_count_in5++;
                else if( !IO5_GetValue() && g_control_count_in5 > 0) g_control_count_in5--;
                    
                if( (g_control_count_in5 == 0 && g_control_io.stat5 == 1) ||
                        (g_control_count_in5 == DELAY_INPUT && g_control_io.stat5 == 0) )
                {
                    g_control_io.stat5 = (g_control_count_in5)?1:0;
                    g_control_io.change5 = true;
                    CTRLNotificarStatus(5);
                }
            }
            else                        /* Output */
            {
                if(IO5_GetValue() != g_control_io.stat5)
                {
                    IO5_SetValue(g_control_io.stat5);
                    g_control_io.change5 = true;
                    //CTRLNotificarStatus(10);
                }
            }
            break;
        case 6:
            if(g_control_io.ana6)       /* Anal�gico */
            {
                
            }
            else if(g_control_io.dir6)  /* Input */
            {
                if(IO6_GetValue() && g_control_count_in6 < DELAY_INPUT) g_control_count_in6++;
                else if( !IO6_GetValue() && g_control_count_in6 > 0) g_control_count_in6--;
                    
                if( (g_control_count_in6 == 0 && g_control_io.stat6 == 1) ||
                        (g_control_count_in6 == DELAY_INPUT && g_control_io.stat6 == 0) )
                {
                    g_control_io.stat6 = (g_control_count_in6)?1:0;
                    g_control_io.change6 = true;
                    CTRLNotificarStatus(5);
                }
            }
            else                        /* Output */
            {
                if(IO6_GetValue() != g_control_io.stat6)
                {
                    IO6_SetValue(g_control_io.stat6);
                    g_control_io.change6 = true;
                    //CTRLNotificarStatus(10);
                }
            }
            break;
        case 7:
            if(OUT1_GetValue() != g_control_out.stat1)
            {
                OUT1_SetValue(g_control_out.stat1);
                g_control_out.change1 = true;
                //CTRLNotificarStatus(10);
            }
            break;
        case 8:
            if(OUT2_GetValue() != g_control_out.stat2)
            {
                OUT2_SetValue(g_control_out.stat2);
                g_control_out.change2 = true;
                //CTRLNotificarStatus(10);
            }
            break;
        case 9:
            if(OUT3_GetValue() != g_control_out.stat3)
            {
                OUT3_SetValue(g_control_out.stat3);
                g_control_out.change3 = true;
                //CTRLNotificarStatus(10);
            }
            break;
        case 10:
            if(OUT4_GetValue() != g_control_out.stat4)
            {
                OUT4_SetValue(g_control_out.stat4);
                g_control_out.change4 = true;
                //CTRLNotificarStatus(10);
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
    if( g_control_notificar > 0 &&
        g_wifi_info.status.wifi_mode == 1 &&
        g_wifi_info.status.wifi_conectado != 0 )
    {
        g_control_notificar--;
        if(g_control_notificar == 0)
        {
            if(HTTPNotificarStatus() == 0 )
            {
                /* programo un reintento */
                CTRLNotificarStatus(30);
            }
        }
    }
    else if(g_control_notificar == 0)
    {
        /* notifico cada minuto */
        CTRLNotificarStatus(600);
    }
    
    /* Control de pulso */
    if(g_control_io.pulse1)
    {
        g_control_io.pulse1--;
        if(g_control_io.pulse1 == 0)
        {
            g_control_io.stat1 = 0;
            CTRLNotificarStatus(10);
        }
    }
    if(g_control_io.pulse2)
    {
        g_control_io.pulse2--;
        if(g_control_io.pulse2 == 0)
        {
            g_control_io.stat2 = 0;
            CTRLNotificarStatus(10);
        }
    }
    if(g_control_io.pulse3)
    {
        g_control_io.pulse3--;
        if(g_control_io.pulse3 == 0)
        {
            g_control_io.stat3 = 0;
            CTRLNotificarStatus(10);
        }
    }
    if(g_control_io.pulse4)
    {
        g_control_io.pulse4--;
        if(g_control_io.pulse4 == 0)
        {
            g_control_io.stat4 = 0;
            CTRLNotificarStatus(10);
        }
    }
    if(g_control_io.pulse5)
    {
        g_control_io.pulse5--;
        if(g_control_io.pulse5 == 0)
        {
            g_control_io.stat5 = 0;
            CTRLNotificarStatus(10);
        }
    }
    if(g_control_io.pulse6)
    {
        g_control_io.pulse6--;
        if(g_control_io.pulse6 == 0)
        {
            g_control_io.stat6 = 0;
            CTRLNotificarStatus(10);
        }
    }
    if(g_control_out.pulse1)
    {
        g_control_out.pulse1--;
        if(g_control_out.pulse1 == 0)
        {
            g_control_out.stat1 = 0;
            CTRLNotificarStatus(10);
        }
    }
    if(g_control_out.pulse2)
    {
        g_control_out.pulse2--;
        if(g_control_out.pulse2 == 0)
        {
            g_control_out.stat2 = 0;
            CTRLNotificarStatus(10);
        }
    }
    if(g_control_out.pulse3)
    {
        g_control_out.pulse3--;
        if(g_control_out.pulse3 == 0)
        {
            g_control_out.stat3 = 0;
            CTRLNotificarStatus(10);
        }
    }
    if(g_control_out.pulse4)
    {
        g_control_out.pulse4--;
        if(g_control_out.pulse4 == 0)
        {
            g_control_out.stat4 = 0;
            CTRLNotificarStatus(10);
        }
    }
    
    
}

void CTRLSetIO(char* in)
{
    char tmp[10];
    
    LogMessage("CTRLSetIO: %s\r\n", in);
    if(g_control_io.dir1 == 0)
    {
        if(HTTPGetFormData(in, "PULSE-IO1", tmp))
        {
            g_control_io.stat1 = 1;
            g_control_io.pulse1 = atoi(tmp)*10;
        }
        else if(HTTPGetFormData(in, "IO1", tmp))
        {
            if( !strcmp(tmp, "on") )
            {
                g_control_io.stat1 = 1;
            }
            else if( !strcmp(tmp, "off") )
            {
                g_control_io.stat1 = 0;
            }
        }
    }

    if(g_control_io.dir2 == 0)
    {
        if(HTTPGetFormData(in, "PULSE-IO2", tmp))
        {
            g_control_io.stat2 = 1;
            g_control_io.pulse2 = atoi(tmp)*10;
        }
        else if(HTTPGetFormData(in, "IO2", tmp))
        {
            if( !strcmp(tmp, "on") )
            {
                g_control_io.stat2 = 1;
            }
            else if( !strcmp(tmp, "off") )
            {
                g_control_io.stat2 = 0;
            }
        }
    }

    if(g_control_io.dir3 == 0)
    {
        if(HTTPGetFormData(in, "PULSE-IO3", tmp))
        {
            g_control_io.stat3 = 1;
            g_control_io.pulse3 = atoi(tmp)*10;
        }
        else if(HTTPGetFormData(in, "IO3", tmp))
        {
            if( !strcmp(tmp, "on") )
            {
                g_control_io.stat3 = 1;
            }
            else if( !strcmp(tmp, "off") )
            {
                g_control_io.stat3 = 0;
            }
        }
    }
    
    if(g_control_io.dir4 == 0)
    {
        if(HTTPGetFormData(in, "PULSE-IO4", tmp))
        {
            g_control_io.stat4 = 1;
            g_control_io.pulse4 = atoi(tmp)*10;
        }
        else if(HTTPGetFormData(in, "IO4", tmp))
        {
            if( !strcmp(tmp, "on") )
            {
                g_control_io.stat4 = 1;
            }
            else if( !strcmp(tmp, "off") )
            {
                g_control_io.stat4 = 0;
            }
        }
    }
    
    if(g_control_io.dir5 == 0)
    {
        if(HTTPGetFormData(in, "PULSE-IO5", tmp))
        {
            g_control_io.stat5 = 1;
            g_control_io.pulse5 = atoi(tmp)*10;
        }
        else if(HTTPGetFormData(in, "IO5", tmp))
        {
            if( !strcmp(tmp, "on") )
            {
                g_control_io.stat5 = 1;
            }
            else if( !strcmp(tmp, "off") )
            {
                g_control_io.stat5 = 0;
            }
        }
    }

    if(g_control_io.dir6 == 0)
    {
        if(HTTPGetFormData(in, "PULSE-IO6", tmp))
        {
            g_control_io.stat6 = 1;
            g_control_io.pulse6 = atoi(tmp)*10;
        }
        else if(HTTPGetFormData(in, "IO6", tmp))
        {
            if( !strcmp(tmp, "on") )
            {
                g_control_io.stat6 = 1;
            }
            else if( !strcmp(tmp, "off") )
            {
                g_control_io.stat6 = 0;
            }
        }
    }

    if(HTTPGetFormData(in, "PULSE-OUT1", tmp))
    {
        g_control_out.stat1 = 1;
        g_control_out.pulse1 = atoi(tmp)*10;
    }
    else if(HTTPGetFormData(in, "OUT1", tmp))
    {
        if( !strcmp(tmp, "on") )
        {
            g_control_out.stat1 = 1;
        }
        else if( !strcmp(tmp, "off") )
        {
            g_control_out.stat1 = 0;
        }
    }
    
    if(HTTPGetFormData(in, "PULSE-OUT2", tmp))
    {
        g_control_out.stat2 = 1;
        g_control_out.pulse2 = atoi(tmp)*10;
    }
    else if(HTTPGetFormData(in, "OUT2", tmp))
    {
        if( !strcmp(tmp, "on") )
        {
            g_control_out.stat2 = 1;
        }
        else if( !strcmp(tmp, "off") )
        {
            g_control_out.stat2 = 0;
        }
    }
    
    if(HTTPGetFormData(in, "PULSE-OUT3", tmp))
    {
        g_control_out.stat3 = 1;
        g_control_out.pulse3 = atoi(tmp)*10;
    }
    else if(HTTPGetFormData(in, "OUT3", tmp))
    {
        if( !strcmp(tmp, "on") )
        {
            g_control_out.stat3 = 1;
        }
        else if( !strcmp(tmp, "off") )
        {
            g_control_out.stat3 = 0;
        }
    }
    
    if(HTTPGetFormData(in, "PULSE-OUT4", tmp))
    {
        g_control_out.stat4 = 1;
        g_control_out.pulse4 = atoi(tmp)*10;
    }
    else if(HTTPGetFormData(in, "OUT4", tmp))
    {
        if( !strcmp(tmp, "on") )
        {
            g_control_out.stat4 = 1;
        }
        else if( !strcmp(tmp, "off") )
        {
            g_control_out.stat4 = 0;
        }
    }
    
}

void CTRLSwitchIO(char* in)
{
    char tmp[10];
    
    LogMessage("CTRLSwitchIO: %s\r\n", in);
    if(g_control_io.dir1 == 0)
    {
        if(HTTPGetFormData(in, "IO1", tmp))
        {
            if( !strcmp(tmp, "on") )
            {
                g_control_io.stat1 = (g_control_io.stat1)?0:1;
            }
        }
    }

    if(g_control_io.dir2 == 0)
    {
        if(HTTPGetFormData(in, "IO2", tmp))
        {
            if( !strcmp(tmp, "on") )
            {
                g_control_io.stat2 = (g_control_io.stat2)?0:1;
            }
        }
    }

    if(g_control_io.dir3 == 0)
    {
        if(HTTPGetFormData(in, "IO3", tmp))
        {
            if( !strcmp(tmp, "on") )
            {
                g_control_io.stat3 = (g_control_io.stat3)?0:1;
            }
        }
    }
    
    if(g_control_io.dir4 == 0)
    {
        if(HTTPGetFormData(in, "IO4", tmp))
        {
            if( !strcmp(tmp, "on") )
            {
                g_control_io.stat4 = (g_control_io.stat4)?0:1;
            }
        }
    }
    
    if(g_control_io.dir5 == 0)
    {
        if(HTTPGetFormData(in, "IO5", tmp))
        {
            if( !strcmp(tmp, "on") )
            {
                g_control_io.stat5 = (g_control_io.stat5)?0:1;
            }
        }
    }
    
    if(g_control_io.dir6 == 0)
    {
        if(HTTPGetFormData(in, "IO6", tmp))
        {
            if( !strcmp(tmp, "on") )
            {
                g_control_io.stat6 = (g_control_io.stat6)?0:1;
            }
        }
    }
    
    if(HTTPGetFormData(in, "OUT1", tmp))
    {
        if( !strcmp(tmp, "on") )
        {
            g_control_out.stat1 = (g_control_out.stat1)?0:1;
        }
    }
    
    if(HTTPGetFormData(in, "OUT2", tmp))
    {
        if( !strcmp(tmp, "on") )
        {
            g_control_out.stat2 = (g_control_out.stat2)?0:1;
        }
    }
    
    if(HTTPGetFormData(in, "OUT3", tmp))
    {
        if( !strcmp(tmp, "on") )
        {
            g_control_out.stat3 = (g_control_out.stat3)?0:1;
        }
    }
    
    if(HTTPGetFormData(in, "OUT4", tmp))
    {
        if( !strcmp(tmp, "on") )
        {
            g_control_out.stat4 = (g_control_out.stat1)?0:1;
        }
    }
    
        
}

void CTRLConfIO(char* in)
{
    char tmp[10];
    bool change = false;
    
    LogMessage("CTRLConfIO: %s\r\n", in);
    if(HTTPGetFormData(in, "IO1", tmp))
    {
        change = true;
        if( !strcmp(tmp, "in") )
        {
            g_control_io.ana1 = 0;
            g_control_io.dir1 = 1;
        }
        else if( !strcmp(tmp, "out") )
        {
            g_control_io.ana1 = 0;
            g_control_io.dir1 = 0;
        }
        else if( !strcmp(tmp, "ana") )
        {
            g_control_io.ana1 = 1;
            g_control_io.dir1 = 1;
        }
    }
    
    if(HTTPGetFormData(in, "IO2", tmp))
    {
        change = true;
        if( !strcmp(tmp, "in") )
        {
            g_control_io.ana2 = 0;
            g_control_io.dir2 = 1;
        }
        else if( !strcmp(tmp, "out") )
        {
            g_control_io.ana2 = 0;
            g_control_io.dir2 = 0;
        }
        else if( !strcmp(tmp, "ana") )
        {
            g_control_io.ana2 = 1;
            g_control_io.dir2 = 1;
        }
    }
    
    if(HTTPGetFormData(in, "IO3", tmp))
    {
        change = true;
        if( !strcmp(tmp, "in") )
        {
            g_control_io.ana3 = 0;
            g_control_io.dir3 = 1;
        }
        else if( !strcmp(tmp, "out") )
        {
            g_control_io.ana3 = 0;
            g_control_io.dir3 = 0;
        }
        else if( !strcmp(tmp, "ana") )
        {
            g_control_io.ana3 = 1;
            g_control_io.dir3 = 1;
        }
    }
    
    if(HTTPGetFormData(in, "IO4", tmp))
    {
        change = true;
        if( !strcmp(tmp, "in") )
        {
            g_control_io.ana4 = 0;
            g_control_io.dir4 = 1;
        }
        else if( !strcmp(tmp, "out") )
        {
            g_control_io.ana4 = 0;
            g_control_io.dir4 = 0;
        }
        else if( !strcmp(tmp, "ana") )
        {
            g_control_io.ana4 = 1;
            g_control_io.dir4 = 1;
        }
    }
    
    if(HTTPGetFormData(in, "IO5", tmp))
    {
        change = true;
        if( !strcmp(tmp, "in") )
        {
            g_control_io.ana5 = 0;
            g_control_io.dir5 = 1;
        }
        else if( !strcmp(tmp, "out") )
        {
            g_control_io.ana5 = 0;
            g_control_io.dir5 = 0;
        }
        else if( !strcmp(tmp, "ana") )
        {
            g_control_io.ana5 = 1;
            g_control_io.dir5 = 1;
        }
    }
    
    if(HTTPGetFormData(in, "IO6", tmp))
    {
        change = true;
        if( !strcmp(tmp, "in") )
        {
            g_control_io.ana6 = 0;
            g_control_io.dir6 = 1;
        }
        else if( !strcmp(tmp, "out") )
        {
            g_control_io.ana6 = 0;
            g_control_io.dir6 = 0;
        }
        else if( !strcmp(tmp, "ana") )
        {
            g_control_io.ana6 = 1;
            g_control_io.dir6 = 1;
        }
    }

    if(change)
    {
        g_wifi_info.f.config.io_config_default = 0;
        g_wifi_info.status.save_config = 50;
    }
}
