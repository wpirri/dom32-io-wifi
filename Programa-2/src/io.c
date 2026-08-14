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
#include "log.h"
#include "config.h"
#include "control.h"
#include "wifi/wifi.h"
#include "http.h"
#include "wiegand.h"

int g_io_status[6];
int g_io_pulse_count[6];
int g_out_status[4];
int g_out_pulse_count[4];
char g_buffer_io_status[1024];

void IOInit( void )
{
    /* LED_Azul - A0 */
    TRISACLR = (1 << 0);
    /* LED_Verde - A1 */
    TRISACLR = (1 << 1);
    /* LED_Ambar - A4 */
    TRISACLR = (1 << 4);
    
    /* OUT1 - B9 */
    TRISBCLR = (1 << 9);
    /* OUT2 - C9 */
    TRISCCLR = (1 << 9);
    /* OUT3 - B10 */
    TRISBCLR = (1 << 10);
    /* OUT4 */
    TRISBCLR = (1 << 11);
    
    g_io_status[0] = (-1);
    g_io_status[1] = (-1);
    g_io_status[2] = (-1);
    g_io_status[3] = (-1);
    g_io_status[4] = (-1);
    g_io_status[5] = (-1);
    
    g_io_pulse_count[0] = 0;
    g_io_pulse_count[1] = 0;
    g_io_pulse_count[2] = 0;
    g_io_pulse_count[3] = 0;
    g_io_pulse_count[4] = 0;
    g_io_pulse_count[5] = 0;

    g_out_status[0] = (-1);
    g_out_status[1] = (-1);
    g_out_status[2] = (-1);
    g_out_status[3] = (-1);
    
    g_out_pulse_count[0] = 0;
    g_out_pulse_count[1] = 0;
    g_out_pulse_count[2] = 0;
    g_out_pulse_count[3] = 0;
    
}

const char* IOGetStatus( void )
{
    char change[256];
    
    g_buffer_io_status[0] = 0;
    change[0] = 0;
    
    if(g_wifi_config.u.config.io_config_values[0] == 'i' || g_wifi_config.u.config.io_config_values[0] == 'o')
    {
        if(IO1_GetValue())
        {
            strcat(g_buffer_io_status, "&IO1=1");
            if( g_io_status[0] != 1 )
            {
                if(g_io_status[0] != (-1)) strcat(change, "IO1,");
                g_io_status[0] = 1;
            }
        }
        else
        {
            strcat(g_buffer_io_status, "&IO1=0");
            if( g_io_status[0] != 0 )
            {
                if(g_io_status[0] != (-1)) strcat(change, "IO1,");
                g_io_status[0] = 0;
            }
        }
    }

    if(g_wifi_config.u.config.io_config_values[1] == 'i' || g_wifi_config.u.config.io_config_values[1] == 'o')
    {
        if(IO2_GetValue())
        {
            strcat(g_buffer_io_status, "&IO2=1");
            if( g_io_status[1] != 1 )
            {
                if(g_io_status[1] != (-1)) strcat(change, "IO2,");
                g_io_status[1] = 1;
            }
        }
        else
        {
            strcat(g_buffer_io_status, "&IO2=0");
            if( g_io_status[1] != 0 )
            {
                if(g_io_status[1] != (-1)) strcat(change, "IO2,");
                g_io_status[1] = 0;
            }
        }
    }

    /*
     IO3 - Digital o Analogica
     */
    if(g_wifi_config.u.config.io_config_values[2] == 'i' || g_wifi_config.u.config.io_config_values[2] == 'o')
    {
        if(IO3_GetValue())
        {
            strcat(g_buffer_io_status, "&IO3=1");
            if( g_io_status[2] != 1 )
            {
                if(g_io_status[2] != (-1)) strcat(change, "IO3,");
                g_io_status[2] = 1;
            }
        }
        else
        {
            strcat(g_buffer_io_status, "&IO3=0");
            if( g_io_status[2] != 0 )
            {
                if(g_io_status[2] != (-1)) strcat(change, "IO3,");
                g_io_status[2] = 0;
            }
        }
    }
    else if(g_wifi_config.u.config.io_config_values[2] == 'a')
    {
        
    }
    
    /*
     IO4 - Digital o Analogica
     */
    if(g_wifi_config.u.config.io_config_values[3] == 'i' || g_wifi_config.u.config.io_config_values[3] == 'o')
    {
        if(IO4_GetValue())
        {
            strcat(g_buffer_io_status, "&IO4=1");
            if( g_io_status[3] != 1 )
            {
                if(g_io_status[3] != (-1)) strcat(change, "IO4,");
                g_io_status[3] = 1;
            }
        }
        else
        {
            strcat(g_buffer_io_status, "&IO4=0");
            if( g_io_status[3] != 0 )
            {
                if(g_io_status[3] != (-1)) strcat(change, "IO4,");
                g_io_status[3] = 0;
            }
        }
    }
    else if(g_wifi_config.u.config.io_config_values[3] == 'a')
    {
        
    }

    /*
     IO5 - IO6 - Los salté si tengo wiegand configurado
     */
    if(g_wifi_config.u.config.enable_wiegand == 0)
    {
        if(g_wifi_config.u.config.io_config_values[4] == 'i' || g_wifi_config.u.config.io_config_values[4] == 'o')
        {
            if(IO5_GetValue())
            {
                strcat(g_buffer_io_status, "&IO5=1");
                if( g_io_status[4] != 1 )
                {
                    if(g_io_status[4] != (-1)) strcat(change, "IO5,");
                    g_io_status[4] = 1;
                }
            }
            else
            {
                strcat(g_buffer_io_status, "&IO5=0");
                if( g_io_status[4] != 0 )
                {
                    if(g_io_status[5] != (-1)) strcat(change, "IO5,");
                    g_io_status[4] = 0;
                }
            }
        }

        if(g_wifi_config.u.config.io_config_values[5] == 'i' || g_wifi_config.u.config.io_config_values[5] == 'o')
        {
            if(IO6_GetValue())
            {
                strcat(g_buffer_io_status, "&IO6=1");
                if( g_io_status[5] != 1 )
                {
                    if(g_io_status[5] != (-1)) strcat(change, "IO6,");
                    g_io_status[5] = 1;
                }
            }
            else
            {
                strcat(g_buffer_io_status, "&IO6=0");
                if( g_io_status[5] != 0 )
                {
                    if(g_io_status[5] != (-1)) strcat(change, "IO6,");
                    g_io_status[5] = 0;
                }
            }
        }
    }
    
    if(OUT1_GetValue())
    {
        strcat(g_buffer_io_status, "&OUT1=1");
        if( g_out_status[0] != 1 )
        {
            if(g_out_status[0] != (-1)) strcat(change, "OUT1,");
            g_out_status[0] = 1;
        }
    }
    else
    {
        strcat(g_buffer_io_status, "&OUT1=0");
        if( g_out_status[0] != 0 )
        {
            if(g_out_status[0] != (-1)) strcat(change, "OUT1,");
            g_out_status[0] = 0;
        }
    }
    
    if(OUT2_GetValue())
    {
        strcat(g_buffer_io_status, "&OUT2=1");
        if( g_out_status[1] != 1 )
        {
            if(g_out_status[1] != (-1)) strcat(change, "OUT2,");
            g_out_status[1] = 1;
        }
    }
    else
    {
        strcat(g_buffer_io_status, "&OUT2=0");
        if( g_out_status[1] != 0 )
        {
            if(g_out_status[1] != (-1)) strcat(change, "OUT2,");
            g_out_status[1] = 0;
        }
    }
    
    if(OUT3_GetValue())
    {
        strcat(g_buffer_io_status, "&OUT3=1");
        if( g_out_status[2] != 1 )
        {
            if(g_out_status[2] != (-1)) strcat(change, "OUT3,");
            g_out_status[2] = 1;
        }
    }
    else
    {
        strcat(g_buffer_io_status, "&OUT3=0");
        if( g_out_status[2] != 0 )
        {
            if(g_out_status[2] != (-1)) strcat(change, "OUT3,");
            g_out_status[2] = 0;
        }
    }
    
    if(OUT4_GetValue())
    {
        strcat(g_buffer_io_status, "&OUT4=1");
        if( g_out_status[3] != 1 )
        {
            if(g_out_status[3] != (-1)) strcat(change, "OUT4,");
            g_out_status[3] = 1;
        }
    }
    else
    {
        strcat(g_buffer_io_status, "&OUT4=0");
        if( g_out_status[3] != 0 )
        {
            if(g_out_status[3] != (-1)) strcat(change, "OUT4,");
            g_out_status[3] = 0;
        }
    }
    
    /* Me fijo si hubo cambios y los agrego al buffer */
    if(strlen(change))
    {
        strcat(g_buffer_io_status, "&CHG=");
        strcat(g_buffer_io_status, change);
        /* saco la coma del final */
        g_buffer_io_status[strlen(g_buffer_io_status)-1] = 0;
    }

    return (const char*)(&g_buffer_io_status[0]);
}

void IOSetConfig( const char* msg )
{
    char s[256];
    int save_config = 0;
    int reset_system = 0;

    if(msg)
    {
        if(HTTPGetFormData(msg, "wiegand", s))
        {
            if(s[0] == '1')
            {
                if(g_wifi_config.u.config.enable_wiegand == 0)
                {
                    g_wifi_config.u.config.io_config_values[4] = 'i';
                    g_wifi_config.u.config.io_config_values[5] = 'i';
                    save_config = 1;
                    reset_system = 1;
                }
                g_wifi_config.u.config.enable_wiegand = 1;
            }
            else
            {
                if(g_wifi_config.u.config.enable_wiegand == 1)
                {
                    save_config = 1;
                    reset_system = 1;
                }
                g_wifi_config.u.config.enable_wiegand = 0;
            }
        }

        if(HTTPGetFormData(msg, "cfgIO1", s))
        {
            /* Entrada 1 - RB5 */
            if(s[0] == 'i')
            {
                g_wifi_config.u.config.io_config_values[0] = s[0];
                TRISBbits.TRISB5 = 1;
                CNPUBbits.CNPUB5 = 1;
            }
            else if(s[0] == 'o')
            {
                g_wifi_config.u.config.io_config_values[0] = s[0];
                TRISBbits.TRISB5 = 0;
                CNPUBbits.CNPUB5 = 0;
            }
            save_config = 1;
        }
        if(HTTPGetFormData(msg, "cfgIO2", s))
        {
            /* Entrada 2 - RB4 */
            if(s[0] == 'i')
            {
                g_wifi_config.u.config.io_config_values[1] = s[0];
                TRISBbits.TRISB4 = 1;
                CNPUBbits.CNPUB4 = 1;
            }
            else if(s[0] == 'o')
            {
                g_wifi_config.u.config.io_config_values[1] = s[0];
                TRISBbits.TRISB4 = 0;
                CNPUBbits.CNPUB4 = 0;
            }
            save_config = 1;
        }
        if(HTTPGetFormData(msg, "cfgIO3", s))
        {
            /* Entrada 3 - RB3 / AN11 */
            if(s[0] == 'i')
            {
                g_wifi_config.u.config.io_config_values[2] = s[0];
                TRISBbits.TRISB3 = 1;
                ANSELBbits.ANSB3 = 0;   // AN11
                CNPUBbits.CNPUB3 = 1;
            }
            else if(s[0] == 'o')
            {
                g_wifi_config.u.config.io_config_values[2] = s[0];
                TRISBbits.TRISB3 = 0;
                ANSELBbits.ANSB3 = 0;   // AN11
                CNPUBbits.CNPUB3 = 0;
            }
            else if(s[0] == 'a')
            {
                g_wifi_config.u.config.io_config_values[2] = s[0];
                TRISBbits.TRISB3 = 1;
                ANSELBbits.ANSB3 = 1;   // AN11
                CNPUBbits.CNPUB3 = 0;
            }
            save_config = 1;
        }
        if(HTTPGetFormData(msg, "cfgIO4", s))
        {
            /* Entrada 4 - RB2 / AN4*/
            if(s[0] == 'i')
            {
                g_wifi_config.u.config.io_config_values[3] = s[0];
                TRISBbits.TRISB2 = 1;
                ANSELBbits.ANSB2 = 0;   // AN4
                CNPUBbits.CNPUB2 = 1;
            }
            else if(s[0] == 'o')
            {
                g_wifi_config.u.config.io_config_values[3] = s[0];
                TRISBbits.TRISB2 = 0;
                ANSELBbits.ANSB2 = 0;   // AN4
                CNPUBbits.CNPUB2 = 0;
            }
            else if(s[0] == 'a')
            {
                g_wifi_config.u.config.io_config_values[3] = s[0];
                TRISBbits.TRISB2 = 1;
                ANSELBbits.ANSB2 = 1;   // AN4
                CNPUBbits.CNPUB2 = 0;
            }
            save_config = 1;
        }

        if(g_wifi_config.u.config.enable_wiegand == 0)
        {
            if(HTTPGetFormData(msg, "cfgIO5", s))
            {
                /* Entrada 5 - RB7 */
                if(s[0] == 'i')
                {
                    g_wifi_config.u.config.io_config_values[4] = s[0];
                    TRISBbits.TRISB7 = 1;
                    CNPUBbits.CNPUB7 = 1;
                }
                else if(s[0] == 'o')
                {
                    g_wifi_config.u.config.io_config_values[4] = s[0];
                    TRISBbits.TRISB7 = 0;
                    CNPUBbits.CNPUB7 = 0;
                }
                save_config = 1;
            }
            if(HTTPGetFormData(msg, "cfgIO6", s))
            {
                /* Entrada 6 - RB8 */
                if(s[0] == 'i')
                {
                    g_wifi_config.u.config.io_config_values[5] = s[0];
                    TRISBbits.TRISB8 = 1;
                    CNPUBbits.CNPUB8 = 1;
                }
                else if(s[0] == 'o')
                {
                    g_wifi_config.u.config.io_config_values[5] = s[0];
                    TRISBbits.TRISB8 = 0;
                    CNPUBbits.CNPUB8 = 0;
                }
                save_config = 1;
            }
        }
    }
    else
    {
        /* Entrada 1 - RB5 */
        if(g_wifi_config.u.config.io_config_values[0] == 'i')
        {

            TRISBbits.TRISB5 = 1;
            CNPUBbits.CNPUB5 = 1;
        }
        else if(g_wifi_config.u.config.io_config_values[0] == 'o')
        {
            TRISBbits.TRISB5 = 0;
            CNPUBbits.CNPUB5 = 0;
        }

        /* Entrada 2 - RB4 */
        if(g_wifi_config.u.config.io_config_values[1] == 'i')
        {
            TRISBbits.TRISB4 = 1;
            CNPUBbits.CNPUB4 = 1;
        }
        else if(g_wifi_config.u.config.io_config_values[1] == 'o')
        {
            TRISBbits.TRISB4 = 0;
            CNPUBbits.CNPUB4 = 0;
        }

        /* Entrada 3 - RB3 / AN11 */
        if(g_wifi_config.u.config.io_config_values[2] == 'i')
        {
            TRISBbits.TRISB3 = 1;
            ANSELBbits.ANSB3 = 0;   // AN11
            CNPUBbits.CNPUB3 = 1;
        }
        else if(g_wifi_config.u.config.io_config_values[2] == 'o')
        {
            TRISBbits.TRISB3 = 0;
            ANSELBbits.ANSB3 = 0;   // AN11
            CNPUBbits.CNPUB3 = 0;
        }
        else if(g_wifi_config.u.config.io_config_values[2] == 'a')
        {
            TRISBbits.TRISB3 = 1;
            ANSELBbits.ANSB3 = 1;   // AN11
            CNPUBbits.CNPUB3 = 0;
        }

        /* Entrada 4 - RB2 / AN4*/
        if(g_wifi_config.u.config.io_config_values[3] == 'i')
        {
            TRISBbits.TRISB2 = 1;
            ANSELBbits.ANSB2 = 0;   // AN4
            CNPUBbits.CNPUB2 = 1;
        }
        else if(g_wifi_config.u.config.io_config_values[3] == 'o')
        {
            TRISBbits.TRISB2 = 0;
            ANSELBbits.ANSB2 = 0;   // AN4
            CNPUBbits.CNPUB2 = 0;
        }
        else if(g_wifi_config.u.config.io_config_values[3] == 'a')
        {
            TRISBbits.TRISB2 = 1;
            ANSELBbits.ANSB2 = 1;   // AN4
            CNPUBbits.CNPUB2 = 0;
        }

        if(g_wifi_config.u.config.enable_wiegand == 0)
        {
            //Disable CNBI interrupt
            IEC0bits.CNBIE = 0;

            /* Entrada 5 - RB7 */
            if(g_wifi_config.u.config.io_config_values[4] == 'i')
            {
                TRISBbits.TRISB7 = 1;
                CNPUBbits.CNPUB7 = 1;
            }
            else if(g_wifi_config.u.config.io_config_values[4] == 'o')
            {
                TRISBbits.TRISB7 = 0;
                CNPUBbits.CNPUB7 = 0;
            }

            /* Entrada 6 - RB8 */
            if(g_wifi_config.u.config.io_config_values[5] == 'i')
            {
                TRISBbits.TRISB8 = 1;
                CNPUBbits.CNPUB8 = 1;
            }
            else if(g_wifi_config.u.config.io_config_values[5] == 'o')
            {
                TRISBbits.TRISB8 = 0;
                CNPUBbits.CNPUB8 = 0;
            }
        }
    }
    
    if(save_config)
    {
        g_wifi_config.u.config.io_config_default = 0;
        ControlSaveConfig(100);
    }
    
    if(reset_system)
    {
        ControlSystemReset(150);
    }
    
}

void IOSetStatus( const char* msg )
{
    char s[256];

    if(HTTPGetFormData(msg, "IO1", s) && 
            g_wifi_config.u.config.io_config_values[0] == 'o')
    {
        if(atoi(s) > 1)
        {
            g_io_pulse_count[0] = (atoi(s)-1)*100;
            IO1_SetHigh();
        } else if(atoi(s) == 1)
        {
            IO1_SetHigh();
        }
        else
        {
            IO1_SetLow();
        }
    }
    if(HTTPGetFormData(msg, "IO2", s) && 
            g_wifi_config.u.config.io_config_values[1] == 'o')
    {
        if(atoi(s) > 1)
        {
            g_io_pulse_count[1] = (atoi(s)-1)*100;
            IO2_SetHigh();
        } else if(atoi(s) == 1)
        {
            IO2_SetHigh();
        }
        else
        {
            IO2_SetLow();
        }
    }
    if(HTTPGetFormData(msg, "IO3", s) && 
            g_wifi_config.u.config.io_config_values[2] == 'o')
    {
        if(atoi(s) > 1)
        {
            g_io_pulse_count[2] = (atoi(s)-1)*100;
            IO3_SetHigh();
        } else if(atoi(s) == 1)
        {
            IO3_SetHigh();
        }
        else
        {
            IO3_SetLow();
        }
    }
    if(HTTPGetFormData(msg, "IO4", s) && 
            g_wifi_config.u.config.io_config_values[3] == 'o')
    {
        if(atoi(s) > 1)
        {
            g_io_pulse_count[3] = (atoi(s)-1)*100;
            IO4_SetHigh();
        } else if(atoi(s) == 1)
        {
            IO4_SetHigh();
        }
        else
        {
            IO4_SetLow();
        }
    }
    /*
     IO5 - IO6 - Excluidas si tengo configurado wiegand
     */
    if(g_wifi_config.u.config.enable_wiegand == 0)
    {
        if(HTTPGetFormData(msg, "IO5", s) && 
                g_wifi_config.u.config.io_config_values[4] == 'o')
        {
            if(atoi(s) > 1)
            {
                g_io_pulse_count[4] = (atoi(s)-1)*100;
                IO5_SetHigh();
            } else if(atoi(s) == 1)
            {
                IO5_SetHigh();
            }
            else
            {
                IO5_SetLow();
            }
        }
        if(HTTPGetFormData(msg, "IO6", s) && 
                g_wifi_config.u.config.io_config_values[5] == 'o')
        {
            if(atoi(s) > 1)
            {
                g_io_pulse_count[5] = (atoi(s)-1)*100;
                IO6_SetHigh();
            } else if(atoi(s) == 1)
            {
                IO6_SetHigh();
            }
            else
            {
                IO6_SetLow();
            }
        }
    }
    if(HTTPGetFormData(msg, "OUT1", s))
    {
        if(atoi(s) > 1)
        {
            g_out_pulse_count[0] = (atoi(s)-1)*100;
            OUT1_SetHigh();
        } else if(atoi(s) == 1)
        {
            OUT1_SetHigh();
        }
        else
        {
            OUT1_SetLow();
        }
    }
    if(HTTPGetFormData(msg, "OUT2", s))
    {
        if(atoi(s) > 1)
        {
            g_out_pulse_count[1] = (atoi(s)-1)*100;
            OUT2_SetHigh();
        } else if(atoi(s) == 1)
        {
            OUT2_SetHigh();
        }
        else
        {
            OUT2_SetLow();
        }
    }
    if(HTTPGetFormData(msg, "OUT3", s))
    {
        if(atoi(s) > 1)
        {
            g_out_pulse_count[2] = (atoi(s)-1)*100;
            OUT3_SetHigh();
        } else if(atoi(s) == 1)
        {
            OUT3_SetHigh();
        }
        else
        {
            OUT3_SetLow();
        }
    }
    if(HTTPGetFormData(msg, "OUT4", s))
    {
        if(atoi(s) > 1)
        {
            g_out_pulse_count[3] = (atoi(s)-1)*100;
            OUT4_SetHigh();
        } else if(atoi(s) == 1)
        {
            OUT4_SetHigh();
        }
        else
        {
            OUT4_SetLow();
        }
    }
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

void IOTimer( void )
{
    /* Manejo el apagado de los pulsos */
    if(g_io_pulse_count[0] > 0)
    {
        g_io_pulse_count[0]--;
        if(g_io_pulse_count[0] == 0)
        {
            IO1_SetLow();
        }
    }
    if(g_io_pulse_count[1] > 0)
    {
        g_io_pulse_count[1]--;
        if(g_io_pulse_count[1] == 0)
        {
            IO2_SetLow();
        }
    }
    if(g_io_pulse_count[2] > 0)
    {
        g_io_pulse_count[2]--;
        if(g_io_pulse_count[2] == 0)
        {
            IO3_SetLow();
        }
    }
    if(g_io_pulse_count[3] > 0)
    {
        g_io_pulse_count[3]--;
        if(g_io_pulse_count[3] == 0)
        {
            IO4_SetLow();
        }
    }
    if(g_io_pulse_count[4] > 0)
    {
        g_io_pulse_count[4]--;
        if(g_io_pulse_count[4] == 0)
        {
            IO5_SetLow();
        }
    }
    if(g_io_pulse_count[5] > 0)
    {
        g_io_pulse_count[5]--;
        if(g_io_pulse_count[5] == 0)
        {
            IO6_SetLow();
        }
    }
    if(g_out_pulse_count[0] > 0)
    {
        g_out_pulse_count[0]--;
        if(g_out_pulse_count[0] == 0)
        {
            OUT1_SetLow();
        }
    }
    if(g_out_pulse_count[1]  > 0)
    {
        g_out_pulse_count[1]--;
        if(g_out_pulse_count[1] == 0)
        {
            OUT2_SetLow();
        }
    }
    if(g_out_pulse_count[2] > 0)
    {
        g_out_pulse_count[2]--;
        if(g_out_pulse_count[2] == 0)
        {
            OUT3_SetLow();
        }
    }
    if(g_out_pulse_count[3] > 0)
    {
        g_out_pulse_count[3]--;
        if(g_out_pulse_count[3] == 0)
        {
            OUT4_SetLow();
        }
    }

}
