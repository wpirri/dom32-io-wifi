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
#include "wifi.h"

#include <string.h>

#include "main.h"

#define WIFI_READ_LINE_BUFFER_LEN 32

typedef enum
{
    RESET = 0,
    START,
    INIT,
    CONNECT,
    LOOP,
    ERROR
} task_stm;

task_stm g_wifi_task_stm;
unsigned int g_wifi_task_stm_step;
unsigned int g_wifi_task_stm_delay;
unsigned int g_wifi_task_waitline_delay;

char g_wifi_read_line_buffer[WIFI_READ_LINE_BUFFER_LEN+1];
unsigned char g_wifi_read_line_buffer_index;

int WifiReadLine( char *line );
int WifiWriteLine( const char *line );
int WifiWaitLine( const char *line, unsigned int cent );
void WifiNextSTM(unsigned int delay);
void WifiSetSTM(task_stm stm, unsigned int delay);
void WifiNextStep(unsigned int delay);
void WifiNextSTM(unsigned int delay);

void WiFiInit( void )
{
    WifiSetSTM(RESET, 0);
    g_wifi_task_waitline_delay = 0;
    g_wifi_read_line_buffer_index = 0;
}

void WiFiTask( void )
{
    if(g_wifi_task_stm_delay) return;
    
    switch(g_wifi_task_stm)
    {
        case RESET:
            WIFI_RESET_SetLow();
            WifiNextSTM(50);
            break;
        case START:
            WIFI_RESET_SetHigh();
            WifiNextSTM(5);
            break;
        case INIT:
            switch(g_wifi_task_stm_step)
            {
                case 0:
                    WifiWriteLine("AT");
                    
                    WifiSetSTM(RESET, 50);

                    //WifiNextStep(0);
                    break;
                case 1:
                    if(WifiWaitLine("OK", 30))
                    {
                        WifiNextStep(0);
                    }
                    else
                    {
                        WifiSetSTM(RESET, 0);
                    }
                    break;
                case 2:
                    WifiNextSTM(5);
                    break;
                default:
                    WifiSetSTM(RESET, 0);
                    break;
            }
            break;
        case CONNECT:
            WifiNextSTM(5);
            break;
        case LOOP:
            break;
        default:    /* ERROR */
            break;
    }
    
    /* Lectura */
    while(UART1_IsRxReady())
    {
        if(g_wifi_read_line_buffer_index < WIFI_READ_LINE_BUFFER_LEN)
        {
            g_wifi_read_line_buffer[g_wifi_read_line_buffer_index++] = UART1_Read();
            g_wifi_read_line_buffer[g_wifi_read_line_buffer_index] = 0;
        }
    }
}

/* Se llama 10 veces por egundo */
void WiFiTimer( void )
{
    if(g_wifi_task_stm_delay) g_wifi_task_stm_delay--;
    if(g_wifi_task_waitline_delay) g_wifi_task_waitline_delay--;
}

int WifiReadLine( char *line )
{
    int len = 0;
    char *p;
    if(strchr(g_wifi_read_line_buffer, '\n'))
    {
        p = &g_wifi_read_line_buffer[0];
        do
        {
            if(*p != 0x0D || *p == 0x0A)
            {
                *line = 0;
                return len;
            }
            else
            {
                *line = *p;
                line++;
                p++;
                len++;
            }
        }while(*p != 0x0D && *p != 0x0A);
    }
    return len;
}

int WifiWriteLine( const char *line )
{
    if(line)
    {
        while(*line)
        {
            UART1_Write((uint8_t)*line);
            line++;
        }
    }
    UART1_Write('\r');
    UART1_Write('\n');
    /* Reseteo el buffer de lectura */
    g_wifi_read_line_buffer_index = 0;
}

int WifiWaitLine( const char *line, unsigned int cent )
{
    int rc = 0;
    char tmp[WIFI_READ_LINE_BUFFER_LEN+1];
    
    g_wifi_task_waitline_delay = cent;
    do
    {
        if(WifiReadLine(tmp))
        {
            if( !strcmp(tmp, line))
            {
                rc = 1;
                break;
            }
        }
    }
    while(g_wifi_task_waitline_delay);
    g_wifi_read_line_buffer_index = 0;
    return rc;
}

void WifiSetSTM(task_stm stm, unsigned int delay)
{
    g_wifi_task_stm = stm;
    g_wifi_task_stm_delay = delay;
    g_wifi_task_stm_step = 0;
}

void WifiNextSTM(unsigned int delay)
{
    g_wifi_task_stm++;
    g_wifi_task_stm_delay = delay;
    g_wifi_task_stm_step = 0;
}

void WifiNextStep(unsigned int delay)
{
    g_wifi_task_stm_delay = delay;
    g_wifi_task_stm_step++;
}
