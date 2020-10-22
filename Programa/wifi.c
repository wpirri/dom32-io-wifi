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
    CLEAN,
    INIT,
    CONNECT,
    LOOP,
    ERROR
} task_stm;

task_stm g_wifi_task_stm;
unsigned int g_wifi_task_stm_step;
unsigned int g_wifi_task_stm_delay;
unsigned int g_wifi_task_stm_timeout;

unsigned char g_wifi_read_line_buffer[WIFI_READ_LINE_BUFFER_LEN+1];
unsigned int g_wifi_read_line_buffer_index;

int WifiReadLine( char *line );
int WifiWriteLine( const char *line );
int WifiWaitLine(const char *line);
void WifiSetSTM(task_stm stm, unsigned int delay, unsigned int timeout);
void WifiNextStep(unsigned int delay, unsigned int timeout);
void WifiNextSTM(unsigned int delay, unsigned int timeout);
int WifiSTMTimeOut( void );

void WiFiInit( void )
{
    WifiSetSTM(RESET, 0, 0);
    g_wifi_task_stm_timeout = 0;
    g_wifi_read_line_buffer_index = 0;
}

void WiFiTask( void )
{
    if(g_wifi_task_stm_delay) return;
    
    switch(g_wifi_task_stm)
    {
        case RESET:
            WIFI_RESET_SetLow();
            WifiNextSTM(50, 0);
            break;
        case START:
            WIFI_RESET_SetHigh();
            WifiNextSTM(5, 50);
            break;
        case CLEAN:
            /* Me quedo recibiendo un rato la basura del principio */
            WifiWaitLine("OK");
            if(WifiSTMTimeOut())
            {
                WifiNextSTM(0, 0);
            }
            break;
        case INIT:
            switch(g_wifi_task_stm_step)
            {
                case 0:
                    WifiWriteLine("AT");
                    WifiNextStep(0, 25);
                    break;
                case 1:
                    if(WifiWaitLine("OK"))
                    {
                        WifiNextStep(0, 0);
                    }
                    else if(WifiSTMTimeOut())
                    {
                        WifiSetSTM(RESET, 0, 0);
                    }
                    break;
                case 2:
                    
                    WifiNextStep(0, 0);
                    break;
                case 3:
                    
                    WifiNextStep(0, 0);
                    break;
                case 4:
                    
                    WifiNextStep(0, 0);
                    break;
                case 5:
                    
                    WifiNextStep(0, 0);
                    break;
                case 6:
                    /* Inicialización finalizada */
                    WifiNextSTM(5, 0);
                    break;
                default:
                    WifiSetSTM(RESET, 0, 0);
                    break;
            }
            break;
        case CONNECT:
            WifiNextSTM(5, 0);
            break;
        case LOOP:
            break;
        default:    /* ERROR */
            break;
    }
}

/* Se llama 10 veces por egundo */
void WiFiTimer( void )
{
    if(g_wifi_task_stm_delay) g_wifi_task_stm_delay--;
    if(g_wifi_task_stm_timeout) g_wifi_task_stm_timeout--;
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

int WifiWaitLine(const char *line)
{
    unsigned int rc = 0;
    unsigned char c;
    
    while(UART1_IsRxReady())
    {
        c = UART1_Read();
        if(c == 0x0D || c ==  0x0A)
        {
            /* Se completa una línea */
            if(g_wifi_read_line_buffer_index)
            {
                if( !strcmp(line, g_wifi_read_line_buffer))
                {
                    rc = g_wifi_read_line_buffer_index;
                }
                g_wifi_read_line_buffer_index = 0;
            }
            break;
        }
        else if(g_wifi_read_line_buffer_index < WIFI_READ_LINE_BUFFER_LEN)
        {
            /* Se agrega el caracter a la línea */
            g_wifi_read_line_buffer[g_wifi_read_line_buffer_index] = c;
            g_wifi_read_line_buffer_index++;
            g_wifi_read_line_buffer[g_wifi_read_line_buffer_index] = 0;
        }
    }
    return rc;
}

void WifiSetSTM(task_stm stm, unsigned int delay, unsigned int timeout)
{
    g_wifi_task_stm = stm;
    g_wifi_task_stm_delay = delay;
    g_wifi_task_stm_timeout = timeout;
    g_wifi_task_stm_step = 0;
}

void WifiNextSTM(unsigned int delay, unsigned int timeout)
{
    g_wifi_task_stm++;
    g_wifi_task_stm_delay = delay;
    g_wifi_task_stm_timeout = timeout;
    g_wifi_task_stm_step = 0;
}

void WifiNextStep(unsigned int delay, unsigned int timeout)
{
    g_wifi_task_stm_delay = delay;
    g_wifi_task_stm_timeout = timeout;
    g_wifi_task_stm_step++;
}

int WifiSTMTimeOut( void )
{
    return ((g_wifi_task_stm_timeout)?0:1);
}
