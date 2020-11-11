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

#include "main.h"

unsigned char g_control_notificar;

void CTRLInit( void )
{
    g_control_io.ana1 = (g_wifi_info.f.config.an_config & 0x01);
    g_control_io.dir1 = (g_wifi_info.f.config.io_config & 0x01);
    g_control_io.stat1 = false;
    if(g_control_io.dir1)   IN1_SetDigitalInput();
    else                    IN1_SetDigitalOutput();
    g_control_io.ana2 = (g_wifi_info.f.config.an_config & 0x02);
    g_control_io.dir2 = (g_wifi_info.f.config.io_config & 0x02);
    g_control_io.stat2 = false;
    if(g_control_io.dir2)   IN2_SetDigitalInput();
    else                    IN2_SetDigitalOutput();
    g_control_io.ana3 = (g_wifi_info.f.config.an_config & 0x04);
    g_control_io.dir3 = (g_wifi_info.f.config.io_config & 0x04);
    g_control_io.stat3 = false;
    if(g_control_io.dir3)   IN3_SetDigitalInput();
    else                    IN3_SetDigitalOutput();
    g_control_io.ana4 = (g_wifi_info.f.config.an_config & 0x08);
    g_control_io.dir4 = (g_wifi_info.f.config.io_config & 0x08);
    g_control_io.stat4 = false;
    if(g_control_io.dir4)   IN4_SetDigitalInput();
    else                    IN4_SetDigitalOutput();
    
    g_control_notificar = 255;
}

void CTRLTask( void )
{
    static unsigned char ctrl_stm = 0;
    
    ctrl_stm++;
    switch(ctrl_stm)
    {
        case 1:
            if(g_control_io.ana1)       /* Analógico */
            {
                
            }
            else if(g_control_io.dir1)  /* Input */
            {
                if(IN1_GetValue() != g_control_io.stat1)
                {
                    g_control_io.stat1 = IN1_GetValue();
                    g_control_notificar = 5;
                }
            }
            else                        /* Output */
            {
                if(IN1_GetValue() != g_control_io.stat1)
                {
                    IN1_SetValue(g_control_io.stat1);
                    g_control_notificar = 25;
                }
            }
            break;
        case 2:
            if(g_control_io.ana2)       /* Analógico */
            {
                
            }
            else if(g_control_io.dir2)  /* Input */
            {
                if(IN2_GetValue() != g_control_io.stat2)
                {
                    g_control_io.stat2 = IN2_GetValue();
                    g_control_notificar = 5;
                }
            }
            else                        /* Output */
            {
                if(IN2_GetValue() != g_control_io.stat2)
                {
                    IN2_SetValue(g_control_io.stat2);
                    g_control_notificar = 25;
                }
            }
            break;
        case 3:
            if(g_control_io.ana3)       /* Analógico */
            {
                
            }
            else if(g_control_io.dir3)  /* Input */
            {
                if(IN3_GetValue() != g_control_io.stat3)
                {
                    g_control_io.stat3 = IN3_GetValue();
                    g_control_notificar = 5;
                }
            }
            else                        /* Output */
            {
                if(IN3_GetValue() != g_control_io.stat3)
                {
                    IN3_SetValue(g_control_io.stat3);
                    g_control_notificar = 25;
                }
            }
            break;
        case 4:
            if(g_control_io.ana4)       /* Analógico */
            {
                
            }
            else if(g_control_io.dir4)  /* Input */
            {
                if(IN4_GetValue() != g_control_io.stat4)
                {
                    g_control_io.stat4 = IN4_GetValue();
                    g_control_notificar = 5;
                }
            }
            else                        /* Output */
            {
                if(IN4_GetValue() != g_control_io.stat4)
                {
                    IN4_SetValue(g_control_io.stat4);
                    g_control_notificar = 25;
                }
            }
            break;
        default:
            ctrl_stm = 0;
            break;
    }
    
}

void CTRLTimer( void )
{
    if(g_control_notificar)
    {
        g_control_notificar--;
        if(g_control_notificar == 0)
        {
            HTTPNotificarStatus();
        }
    }
}

void CTRLSetIO(char* in)
{
    char tmp[10];
    
    if(g_control_io.dir1 == 0)
    {
        if(HTTPGetFormData(in, "IO1", tmp))
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
        if(HTTPGetFormData(in, "IO2", tmp))
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
        if(HTTPGetFormData(in, "IO3", tmp))
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
        if(HTTPGetFormData(in, "IO4", tmp))
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
}

void CTRLConfIO(char* in)
{
    char tmp[10];
    
    if(HTTPGetFormData(in, "Dir1", tmp))
    {
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
    
    if(HTTPGetFormData(in, "Dir2", tmp))
    {
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
    
    if(HTTPGetFormData(in, "Dir3", tmp))
    {
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
    
    if(HTTPGetFormData(in, "Dir4", tmp))
    {
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
}
