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

unsigned int g_control_notificar;

unsigned int g_control_count_in1;
unsigned int g_control_count_in2;
unsigned int g_control_count_in3;
unsigned int g_control_count_in4;

unsigned int g_control_count_ex1;
unsigned int g_control_count_ex2;
unsigned int g_control_count_ex3;
unsigned int g_control_count_ex4;
unsigned int g_control_count_ex5;
unsigned int g_control_count_ex6;
unsigned int g_control_count_ex7;
unsigned int g_control_count_ex8;
unsigned int g_control_count_ex9;

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
    /* Extradas */
    g_control_io.ana1 = (g_wifi_info.f.config.an_config & 0x01)?1:0;
    g_control_io.dir1 = (g_wifi_info.f.config.io_config & 0x01)?1:0;
    g_control_io.stat1 = false;
    if(g_control_io.dir1)   {TRISBSET = (1 << 4); CNPUBSET = (1 << 4);}
    else                    {TRISBCLR = (1 << 4);}
    g_control_io.ana2 = (g_wifi_info.f.config.an_config & 0x02)?1:0;
    g_control_io.dir2 = (g_wifi_info.f.config.io_config & 0x02)?1:0;
    g_control_io.stat2 = false;
    if(g_control_io.dir2)   {TRISBSET = (1 << 5); CNPUBSET = (1 << 5);}
    else                    {TRISBCLR = (1 << 5);}
    g_control_io.ana3 = (g_wifi_info.f.config.an_config & 0x04)?1:0;
    g_control_io.dir3 = (g_wifi_info.f.config.io_config & 0x04)?1:0;
    g_control_io.stat3 = false;
    if(g_control_io.dir3)   {TRISBSET = (1 << 6); CNPUBSET = (1 << 6);}
    else                    {TRISBCLR = (1 << 6);}
    g_control_io.ana4 = (g_wifi_info.f.config.an_config & 0x08)?1:0;
    g_control_io.dir4 = (g_wifi_info.f.config.io_config & 0x08)?1:0;
    g_control_io.stat4 = false;
    if(g_control_io.dir4)   {TRISBSET = (1 << 7); CNPUBSET = (1 << 7);}
    else                    {TRISBCLR = (1 << 7);}

    /* Expansion */
    g_control_exp.ana1 = (g_wifi_info.f.config.exp_an_config & 0x01)?1:0;
    g_control_exp.dir1 = (g_wifi_info.f.config.exp_io_config & 0x01)?1:0;
    g_control_exp.stat1 = false;
    if(g_control_exp.dir1)  {TRISBSET = (1 << 10); CNPUBSET = (1 << 10);}
    else                    {TRISBCLR = (1 << 10);}
    g_control_exp.ana3 = (g_wifi_info.f.config.exp_an_config & 0x02)?1:0;
    g_control_exp.dir3 = (g_wifi_info.f.config.exp_io_config & 0x02)?1:0;
    g_control_exp.stat3 = false;
    if(g_control_exp.dir3)  {TRISBSET = (1 << 9); CNPUBSET = (1 << 9);}
    else                    {TRISBCLR = (1 << 9);}
    g_control_exp.ana4 = (g_wifi_info.f.config.exp_an_config & 0x04)?1:0;
    g_control_exp.dir4 = (g_wifi_info.f.config.exp_io_config & 0x04)?1:0;
    g_control_exp.stat4 = false;
    if(g_control_exp.dir4)  {TRISBSET = (1 << 8); CNPUBSET = (1 << 8);}
    else                    {TRISBCLR = (1 << 8);}
    g_control_exp.ana5 = (g_wifi_info.f.config.exp_an_config & 0x08)?1:0;
    g_control_exp.dir5 = (g_wifi_info.f.config.exp_io_config & 0x08)?1:0;
    g_control_exp.stat5 = false;
    if(g_control_exp.dir5)  {TRISASET = (1 << 3); CNPUASET = (1 << 3);}
    else                    {TRISACLR = (1 << 3);}
    g_control_exp.ana7 = (g_wifi_info.f.config.exp_an_config & 0x10)?1:0;
    g_control_exp.dir7 = (g_wifi_info.f.config.exp_io_config & 0x10)?1:0;
    g_control_exp.stat7 = false;
    if(g_control_exp.dir7)  {TRISBSET = (1 << 11); CNPUBSET = (1 << 11);}
    else                    {TRISBCLR = (1 << 11);}
    g_control_exp.ana8 = (g_wifi_info.f.config.exp_an_config & 0x20)?1:0;
    g_control_exp.dir8 = (g_wifi_info.f.config.exp_io_config & 0x20)?1:0;
    g_control_exp.stat8 = false;
    if(g_control_exp.dir8)  {TRISASET = (1 << 2); CNPUASET = (1 << 2);}
    else                    {TRISACLR = (1 << 2);}
    g_control_exp.ana9 = (g_wifi_info.f.config.exp_an_config & 0x40)?1:0;
    g_control_exp.dir9 = (g_wifi_info.f.config.exp_io_config & 0x40)?1:0;
    g_control_exp.stat9 = false;
    if(g_control_exp.dir9)  {TRISBSET = (1 << 12); CNPUBSET = (1 << 12);}
    else                    {TRISBCLR = (1 << 12);}
    
    g_control_count_in1 = 0;
    g_control_count_in2 = 0;
    g_control_count_in3 = 0;
    g_control_count_in4 = 0;
    g_control_count_ex1 = 0;
    g_control_count_ex2 = 0;
    g_control_count_ex3 = 0;
    g_control_count_ex4 = 0;
    g_control_count_ex5 = 0;
    g_control_count_ex6 = 0;
    g_control_count_ex7 = 0;
    g_control_count_ex8 = 0;
    g_control_count_ex9 = 0;

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
            if(g_control_io.ana1)       /* Analógico */
            {
                
            }
            else if(g_control_io.dir1)  /* Input */
            {
                if(IN1_GetValue() && g_control_count_in1 < DELAY_INPUT) g_control_count_in1++;
                else if( !IN1_GetValue() && g_control_count_in1 > 0) g_control_count_in1--;
                    
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
                if(IN1_GetValue() != g_control_io.stat1)
                {
                    IN1_SetValue(g_control_io.stat1);
                    g_control_io.change1 = true;
                    CTRLNotificarStatus(25);
                }
            }
            break;
        case 2:
            if(g_control_io.ana2)       /* Analógico */
            {
                
            }
            else if(g_control_io.dir2)  /* Input */
            {
                if(IN2_GetValue() && g_control_count_in2 < DELAY_INPUT) g_control_count_in2++;
                else if( !IN2_GetValue() && g_control_count_in2 > 0) g_control_count_in2--;
                    
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
                if(IN2_GetValue() != g_control_io.stat2)
                {
                    IN2_SetValue(g_control_io.stat2);
                    g_control_io.change2 = true;
                    CTRLNotificarStatus(25);
                }
            }
            break;
        case 3:
            if(g_control_io.ana3)       /* Analógico */
            {
                
            }
            else if(g_control_io.dir3)  /* Input */
            {
                if(IN3_GetValue() && g_control_count_in3 < DELAY_INPUT) g_control_count_in3++;
                else if( !IN3_GetValue() && g_control_count_in3 > 0) g_control_count_in3--;
                    
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
                if(IN3_GetValue() != g_control_io.stat3)
                {
                    IN3_SetValue(g_control_io.stat3);
                    g_control_io.change3 = true;
                    CTRLNotificarStatus(25);
                }
            }
            break;
        case 4:
            if(g_control_io.ana4)       /* Analógico */
            {
                
            }
            else if(g_control_io.dir4)  /* Input */
            {
                if(IN4_GetValue() && g_control_count_in4 < DELAY_INPUT) g_control_count_in4++;
                else if( !IN4_GetValue() && g_control_count_in4 > 0) g_control_count_in4--;
                    
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
                if(IN4_GetValue() != g_control_io.stat4)
                {
                    IN4_SetValue(g_control_io.stat4);
                    g_control_io.change4 = true;
                    CTRLNotificarStatus(25);
                }
            }
            break;
        case 5:
            if(g_control_exp.ana1)       /* Analógico */
            {
                
            }
            else if(g_control_exp.dir1)  /* Input */
            {
                if(EXP01_GetValue() && g_control_count_ex1 < DELAY_INPUT) g_control_count_ex1++;
                else if( !EXP01_GetValue() && g_control_count_ex1 > 0) g_control_count_ex1--;
                    
                if( (g_control_count_ex1 == 0 && g_control_exp.stat1 == 1) ||
                        (g_control_count_ex1 == DELAY_INPUT && g_control_exp.stat1 == 0) )
                {
                    g_control_exp.stat1 = (g_control_count_ex1)?1:0;
                    g_control_exp.change1 = true;
                    CTRLNotificarStatus(5);
                }
            }
            else                        /* Output */
            {
                if(EXP01_GetValue() != g_control_exp.stat1)
                {
                    EXP01_SetValue(g_control_exp.stat1);
                    g_control_exp.change1 = true;
                    CTRLNotificarStatus(25);
                }
            }
            break;
        case 6:
            if(g_control_exp.ana3)       /* Analógico */
            {
                
            }
            else if(g_control_exp.dir3)  /* Input */
            {
                if(EXP03_GetValue() && g_control_count_ex3 < DELAY_INPUT) g_control_count_ex3++;
                else if( !EXP03_GetValue() && g_control_count_ex3 > 0) g_control_count_ex3--;
                    
                if( (g_control_count_ex3 == 0 && g_control_exp.stat3 == 1) ||
                        (g_control_count_ex3 == DELAY_INPUT && g_control_exp.stat3 == 0) )
                {
                    g_control_exp.stat3 = (g_control_count_ex3)?1:0;
                    g_control_exp.change3 = true;
                    CTRLNotificarStatus(25);
                }
            }
            else                        /* Output */
            {
                if(EXP03_GetValue() != g_control_exp.stat3)
                {
                    EXP03_SetValue(g_control_exp.stat3);
                    g_control_exp.change3 = true;
                    CTRLNotificarStatus(25);
                }
            }
            break;
        case 7:
            if(g_control_exp.ana4)       /* Analógico */
            {
                
            }
            else if(g_control_exp.dir4)  /* Input */
            {
                if(EXP04_D0_GetValue() && g_control_count_ex4 < DELAY_INPUT) g_control_count_ex4++;
                else if( !EXP04_D0_GetValue() && g_control_count_ex4 > 0) g_control_count_ex4--;
                    
                if( (g_control_count_ex4 == 0 && g_control_exp.stat4 == 1) ||
                        (g_control_count_ex4 == DELAY_INPUT && g_control_exp.stat4 == 0) )
                {
                    g_control_exp.stat4 = (g_control_count_ex4)?1:0;
                    g_control_exp.change4 = true;
                    CTRLNotificarStatus(5);
                }
            }
            else                        /* Output */
            {
                if(EXP04_D0_GetValue() != g_control_exp.stat4)
                {
                    EXP04_D0_SetValue(g_control_exp.stat4);
                    g_control_exp.change4 = true;
                    CTRLNotificarStatus(25);
                }
            }
            break;
        case 8:
            if(g_control_exp.ana5)       /* Analógico */
            {
                
            }
            else if(g_control_exp.dir5)  /* Input */
            {
                if(EXP05_GetValue() && g_control_count_ex5 < DELAY_INPUT) g_control_count_ex5++;
                else if( !EXP05_GetValue() && g_control_count_ex5 > 0) g_control_count_ex5--;
                    
                if( (g_control_count_ex5 == 0 && g_control_exp.stat5 == 1) ||
                        (g_control_count_ex5 == DELAY_INPUT && g_control_exp.stat5 == 0) )
                {
                    g_control_exp.stat5 = (g_control_count_ex5)?1:0;
                    g_control_exp.change5 = true;
                    CTRLNotificarStatus(5);
                }
            }
            else                        /* Output */
            {
                if(EXP05_GetValue() != g_control_exp.stat5)
                {
                    EXP05_SetValue(g_control_exp.stat5);
                    g_control_exp.change5 = true;
                    CTRLNotificarStatus(25);
                }
            }
            break;
        case 9:
            if(g_control_exp.ana7)       /* Analógico */
            {
                
            }
            else if(g_control_exp.dir7)  /* Input */
            {
                if(EXP07_GetValue() && g_control_count_ex7 < DELAY_INPUT) g_control_count_ex7++;
                else if( !EXP07_GetValue() && g_control_count_ex7 > 0) g_control_count_ex7--;
                    
                if( (g_control_count_ex7 == 0 && g_control_exp.stat7 == 1) ||
                        (g_control_count_ex7 == DELAY_INPUT && g_control_exp.stat7 == 0) )
                {
                    g_control_exp.stat7 = (g_control_count_ex7)?1:0;
                    g_control_exp.change7 = true;
                    CTRLNotificarStatus(5);
                }
            }
            else                        /* Output */
            {
                if(EXP07_GetValue() != g_control_exp.stat7)
                {
                    EXP07_SetValue(g_control_exp.stat7);
                    g_control_exp.change7 = true;
                    CTRLNotificarStatus(25);
                }
            }
            break;
        case 10:
            if(g_control_exp.ana8)       /* Analógico */
            {
                
            }
            else if(g_control_exp.dir8)  /* Input */
            {
                if(EXP08_D1_GetValue() && g_control_count_ex8 < DELAY_INPUT) g_control_count_ex8++;
                else if( !EXP08_D1_GetValue() && g_control_count_ex8 > 0) g_control_count_ex8--;
                    
                if( (g_control_count_ex8 == 0 && g_control_exp.stat8 == 1) ||
                        (g_control_count_ex8 == DELAY_INPUT && g_control_exp.stat8 == 0) )
                {
                    g_control_exp.stat8 = (g_control_count_ex8)?1:0;
                    g_control_exp.change8 = true;
                    CTRLNotificarStatus(5);
                }
            }
            else                        /* Output */
            {
                if(EXP08_D1_GetValue() != g_control_exp.stat8)
                {
                    EXP08_D1_SetValue(g_control_exp.stat8);
                    g_control_exp.change8 = true;
                    CTRLNotificarStatus(25);
                }
            }
            break;
        case 11:
            if(g_control_exp.ana9)       /* Analógico */
            {
                
            }
            else if(g_control_exp.dir9)  /* Input */
            {
                if(EXP09_GetValue() && g_control_count_ex9 < DELAY_INPUT) g_control_count_ex9++;
                else if( !EXP09_GetValue() && g_control_count_ex9 > 0) g_control_count_ex9--;
                    
                if( (g_control_count_ex9 == 0 && g_control_exp.stat9 == 1) ||
                        (g_control_count_ex9 == DELAY_INPUT && g_control_exp.stat9 == 0) )
                {
                    g_control_exp.stat9 = (g_control_count_ex9)?1:0;
                    g_control_exp.change9 = true;
                    CTRLNotificarStatus(5);
                }
            }
            else                        /* Output */
            {
                if(EXP09_GetValue() != g_control_exp.stat9)
                {
                    EXP09_SetValue(g_control_exp.stat9);
                    g_control_exp.change9 = true;
                    CTRLNotificarStatus(25);
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
    if(g_control_notificar && g_wifi_info.status.wifi_conectado)
    {
        g_control_notificar--;
        if(g_control_notificar == 0)
        {
            if(HTTPNotificarStatus() == 0 )
            {
                /* programo un reintento */
                CTRLNotificarStatus(10);
            }
        }
    }
    else if(g_control_notificar == 0)
    {
        /* notifico cada minuto */
        CTRLNotificarStatus(600);
    }
}

void CTRLSetIO(char* in)
{
    char tmp[10];
    
//    CTRLNotificarStatus(5);

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
    
    if(g_control_exp.dir1 == 0)
    {
        if(HTTPGetFormData(in, "EXP1", tmp))
        {
            if( !strcmp(tmp, "on") )
            {
                g_control_exp.stat1 = 1;
            }
            else if( !strcmp(tmp, "off") )
            {
                g_control_exp.stat1 = 0;
            }
        }
    }
    
    if(g_control_exp.dir3 == 0)
    {
        if(HTTPGetFormData(in, "EXP3", tmp))
        {
            if( !strcmp(tmp, "on") )
            {
                g_control_exp.stat3 = 1;
            }
            else if( !strcmp(tmp, "off") )
            {
                g_control_exp.stat3 = 0;
            }
        }
    }
    
    if(g_control_exp.dir4 == 0)
    {
        if(HTTPGetFormData(in, "EXP4", tmp))
        {
            if( !strcmp(tmp, "on") )
            {
                g_control_exp.stat4 = 1;
            }
            else if( !strcmp(tmp, "off") )
            {
                g_control_exp.stat4 = 0;
            }
        }
    }
    
    if(g_control_exp.dir5 == 0)
    {
        if(HTTPGetFormData(in, "EXP5", tmp))
        {
            if( !strcmp(tmp, "on") )
            {
                g_control_exp.stat5 = 1;
            }
            else if( !strcmp(tmp, "off") )
            {
                g_control_exp.stat5 = 0;
            }
        }
    }

    if(g_control_exp.dir7 == 0)
    {
        if(HTTPGetFormData(in, "EXP7", tmp))
        {
            if( !strcmp(tmp, "on") )
            {
                g_control_exp.stat7 = 1;
            }
            else if( !strcmp(tmp, "off") )
            {
                g_control_exp.stat7 = 0;
            }
        }
    }
    
    if(g_control_exp.dir8 == 0)
    {
        if(HTTPGetFormData(in, "EXP8", tmp))
        {
            if( !strcmp(tmp, "on") )
            {
                g_control_exp.stat8 = 1;
            }
            else if( !strcmp(tmp, "off") )
            {
                g_control_exp.stat8 = 0;
            }
        }
    }
    
    if(g_control_exp.dir9 == 0)
    {
        if(HTTPGetFormData(in, "EXP9", tmp))
        {
            if( !strcmp(tmp, "on") )
            {
                g_control_exp.stat9 = 1;
            }
            else if( !strcmp(tmp, "off") )
            {
                g_control_exp.stat9 = 0;
            }
        }
    }
        
}

void CTRLSwitchIO(char* in)
{
    char tmp[10];
    
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
    
    if(g_control_exp.dir1 == 0)
    {
        if(HTTPGetFormData(in, "EXP1", tmp))
        {
            if( !strcmp(tmp, "on") )
            {
                g_control_exp.stat1 = (g_control_exp.stat1)?0:1;
            }
        }
    }
    
    if(g_control_exp.dir3 == 0)
    {
        if(HTTPGetFormData(in, "EXP3", tmp))
        {
            if( !strcmp(tmp, "on") )
            {
                g_control_exp.stat3 = (g_control_exp.stat3)?0:1;
            }
        }
    }
    
    if(g_control_exp.dir4 == 0)
    {
        if(HTTPGetFormData(in, "EXP4", tmp))
        {
            if( !strcmp(tmp, "on") )
            {
                g_control_exp.stat4 = (g_control_exp.stat4)?0:1;
            }
        }
    }
    
    if(g_control_exp.dir5 == 0)
    {
        if(HTTPGetFormData(in, "EXP5", tmp))
        {
            if( !strcmp(tmp, "on") )
            {
                g_control_exp.stat5 = (g_control_exp.stat5)?0:1;
            }
        }
    }

    if(g_control_exp.dir7 == 0)
    {
        if(HTTPGetFormData(in, "EXP7", tmp))
        {
            if( !strcmp(tmp, "on") )
            {
                g_control_exp.stat7 = (g_control_exp.stat7)?0:1;
            }
        }
    }
    
    if(g_control_exp.dir8 == 0)
    {
        if(HTTPGetFormData(in, "EXP8", tmp))
        {
            if( !strcmp(tmp, "on") )
            {
                g_control_exp.stat8 = (g_control_exp.stat8)?0:1;
            }
        }
    }
    
    if(g_control_exp.dir9 == 0)
    {
        if(HTTPGetFormData(in, "EXP9", tmp))
        {
            if( !strcmp(tmp, "on") )
            {
                g_control_exp.stat9 = (g_control_exp.stat9)?0:1;
            }
        }
    }
        
}

void CTRLConfIO(char* in)
{
    char tmp[10];
    
//    CTRLNotificarStatus(5);
    
    if(HTTPGetFormData(in, "IO1", tmp))
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
    
    if(HTTPGetFormData(in, "IO2", tmp))
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
    
    if(HTTPGetFormData(in, "IO3", tmp))
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
    
    if(HTTPGetFormData(in, "IO4", tmp))
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
    
    if(HTTPGetFormData(in, "EXP1", tmp))
    {
        if( !strcmp(tmp, "in") )
        {
            g_control_exp.ana1 = 0;
            g_control_exp.dir1 = 1;
        }
        else if( !strcmp(tmp, "out") )
        {
            g_control_exp.ana1 = 0;
            g_control_exp.dir1 = 0;
        }
        else if( !strcmp(tmp, "ana") )
        {
            g_control_exp.ana1 = 1;
            g_control_exp.dir1 = 1;
        }
    }
    
    if(HTTPGetFormData(in, "EXP3", tmp))
    {
        if( !strcmp(tmp, "in") )
        {
            g_control_exp.ana3 = 0;
            g_control_exp.dir3 = 1;
        }
        else if( !strcmp(tmp, "out") )
        {
            g_control_exp.ana3 = 0;
            g_control_exp.dir3 = 0;
        }
        else if( !strcmp(tmp, "ana") )
        {
            g_control_exp.ana3 = 1;
            g_control_exp.dir3 = 1;
        }
    }
    
    if(HTTPGetFormData(in, "EXP4", tmp))
    {
        if( !strcmp(tmp, "in") )
        {
            g_control_exp.ana4 = 0;
            g_control_exp.dir4 = 1;
        }
        else if( !strcmp(tmp, "out") )
        {
            g_control_exp.ana4 = 0;
            g_control_exp.dir4 = 0;
        }
        else if( !strcmp(tmp, "ana") )
        {
            g_control_exp.ana4 = 1;
            g_control_exp.dir4 = 1;
        }
    }
    
    if(HTTPGetFormData(in, "EXP5", tmp))
    {
        if( !strcmp(tmp, "in") )
        {
            g_control_exp.ana5 = 0;
            g_control_exp.dir5 = 1;
        }
        else if( !strcmp(tmp, "out") )
        {
            g_control_exp.ana5 = 0;
            g_control_exp.dir5 = 0;
        }
        else if( !strcmp(tmp, "ana") )
        {
            g_control_exp.ana5 = 1;
            g_control_exp.dir5 = 1;
        }
    }
    
    if(HTTPGetFormData(in, "EXP7", tmp))
    {
        if( !strcmp(tmp, "in") )
        {
            g_control_exp.ana7 = 0;
            g_control_exp.dir7 = 1;
        }
        else if( !strcmp(tmp, "out") )
        {
            g_control_exp.ana7 = 0;
            g_control_exp.dir7 = 0;
        }
        else if( !strcmp(tmp, "ana") )
        {
            g_control_exp.ana7 = 1;
            g_control_exp.dir7 = 1;
        }
    }
    
    if(HTTPGetFormData(in, "EXP8", tmp))
    {
        if( !strcmp(tmp, "in") )
        {
            g_control_exp.ana8 = 0;
            g_control_exp.dir8 = 1;
        }
        else if( !strcmp(tmp, "out") )
        {
            g_control_exp.ana8 = 0;
            g_control_exp.dir8 = 0;
        }
        else if( !strcmp(tmp, "ana") )
        {
            g_control_exp.ana8 = 1;
            g_control_exp.dir8 = 1;
        }
    }
    
    if(HTTPGetFormData(in, "EXP9", tmp))
    {
        if( !strcmp(tmp, "in") )
        {
            g_control_exp.ana9 = 0;
            g_control_exp.dir9 = 1;
        }
        else if( !strcmp(tmp, "out") )
        {
            g_control_exp.ana9 = 0;
            g_control_exp.dir9 = 0;
        }
        else if( !strcmp(tmp, "ana") )
        {
            g_control_exp.ana9 = 1;
            g_control_exp.dir9 = 1;
        }
    }
    g_wifi_info.f.config.io_config_default = 0;
    g_wifi_info.status.save_config = 50;
}
