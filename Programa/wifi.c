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
#include <xc.h>
 /*
 	TODO: Seleccion de host de destino
    TODO: Agregar soporte SSL
 	TODO: Modo de conexiones solo salientes
 */

#define _IN_WIFI_C_
#include "wifi.h"
#undef _IN_WIFI_C_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "mcc_generated_files/uart1.h"
#include "mcc_generated_files/pin_manager.h"
#include "clog.h"
#include "strfunc.h"
#include "control.h"
#include "http.h"
#include "config.h"
#include "delay.h"
#include "sd/fsio.h"

#define TIMER_AP_VALUE 600

typedef enum
{
    INVALID = 0,
    RESET,
    START,
    CLEAN,
    INIT,
    UPGRADE,
    APMODE,
    STMODE,
    HTTP_RESP,
    HTTP_REQ,
    LISTAP,
    LOOP,
    ERROR
} task_stm;

const char *task_stm_label[] = {
    "INVALID",
    "RESET",
    "START",
    "CLEAN",
    "INIT",
    "UPGRADE",
    "APMODE",
    "STMODE",
    "HTTP_RESP",
    "HTTP_REQ",
    "LISTAP",
    "LOOP",
    "ERROR"};

task_stm g_wifi_task_stm;
task_stm g_wifi_task_stm_last;
unsigned int g_wifi_timer_ap;
unsigned int g_wifi_task_stm_step;
unsigned int g_wifi_task_stm_step_last;
unsigned int g_wifi_task_stm_delay;
unsigned int g_wifi_task_stm_timeout;
unsigned int g_wifi_task_retry;
unsigned char g_wifi_send_error_count;
unsigned char g_wifi_recv_error_count;
char g_wifi_read_line_buffer[WIFI_READ_LINE_BUFFER_LEN+1];
int g_wifi_read_line_buffer_index;
char g_wifi_http_filename[FILE_NAME_SIZE+1];
unsigned char g_wifi_read_line_buffer_timer;
bool g_wifi_read_line_wait_crlf;
bool g_wifi_wait_more_data;
char g_wifi_cmd[256];

bool g_wifi_need_upgrade;

void WifiTaskSTMSet(task_stm st, unsigned int delay, unsigned int to)
{
    g_wifi_task_stm = st;
    g_wifi_task_stm_delay = delay;
    g_wifi_task_stm_timeout = to;
    g_wifi_task_stm_step = 0;
    g_wifi_task_stm_step_last = (-1);
}

void WifiWrite( const char *buffer )
{
    if(buffer)
    {
        LogMessage("[WifiWrite] %-.100s", buffer);
        while(*buffer)
        {
            UART1_Write((uint8_t)*buffer);
            buffer++;
        }
    }
}

void WifiWriteLine( const char *line )
{
    if(line)
    {
        LogMessage("[WifiWriteLine] %-.100s", line);
        while(*line)
        {
            UART1_Write((uint8_t)*line);
            line++;
        }
    }
    UART1_Write('\r');
    UART1_Write('\n');
}

/*
    Return: = 0 - No se recibi� lo que se esperaba y ninguna acci�n no solicitada
            > 0 - Se recibi� algo no solicitado rc = nuevo estado
            < 0 - Se recibio lo que se esperaba
*/
int WifiWaitLine(const char *line1, const char *line2, const char *line3, const char *line4, const char *line5)
{
    int rc = 0;
    unsigned char c = 0;

    while(UART1_IsRxReady())
    {
        c = UART1_Read();
        g_wifi_read_line_buffer_timer = 3;

        /* Si estoy esperando fin de linea ... */
        if(g_wifi_read_line_wait_crlf)
        {
            /* salgo del loop cuando lo recibo */
            if(c == 0x0A || c == 0x0D) break;
            /* elimino todo lo recibido si recibo ascii extendido */
            if(c > 127)
            {
                g_wifi_read_line_buffer_timer = 0;
                g_wifi_read_line_buffer_index = 0;
                return 0;
            }
        }
        /*voy agregando los datos al buffer */
        if(g_wifi_read_line_buffer_index < WIFI_READ_LINE_BUFFER_LEN)
        {
            /* Se agrega el caracter a la linea */
            g_wifi_read_line_buffer[g_wifi_read_line_buffer_index] = c;
            g_wifi_read_line_buffer_index++;
            g_wifi_read_line_buffer[g_wifi_read_line_buffer_index] = 0;
            /* Detecto la llegada de datos por TCP y dejo de esperar el fin de linea */
            if(g_wifi_read_line_buffer_index == 4)
            {
                /*+IPD*/
                if(g_wifi_read_line_buffer[0] == '+' &&
                   g_wifi_read_line_buffer[1] == 'I' &&
                   g_wifi_read_line_buffer[2] == 'P' &&
                   g_wifi_read_line_buffer[3] == 'D'  )
                {
                    g_wifi_read_line_wait_crlf = false;
                }
            }
            /* Si el buffer está lleno, salgo */
            if(g_wifi_read_line_buffer_index == WIFI_READ_LINE_BUFFER_LEN) break;
        }
    }   
    
    if(g_wifi_wait_more_data && ((g_wifi_read_line_buffer_timer == 0) || (g_wifi_read_line_buffer_index == WIFI_READ_LINE_BUFFER_LEN)) )
    {
        if(g_wifi_read_line_buffer_timer == 0)
        {
            /* time-out esperando mas datos */
            HTTPEndTmpFile(http_buffer_output.filename, 
                    g_wifi_read_line_buffer, 
                    g_wifi_read_line_buffer_index);
            g_wifi_wait_more_data = false;
            g_wifi_read_line_buffer_index = 0;
            http_buffer_output.download = 0;
        }
        else if(g_wifi_read_line_buffer_index == WIFI_READ_LINE_BUFFER_LEN)
        {
            /* buffer lleno esperando mas datos */
            HTTPAddTmpFile(g_wifi_read_line_buffer, g_wifi_read_line_buffer_index);
            g_wifi_read_line_buffer_index = 0;
        }
    }
    /* Detecto el final de linea o time-out recibiendo datos */
    else if( (g_wifi_read_line_wait_crlf && (c == 0x0A || c == 0x0D)) || (g_wifi_read_line_buffer_timer == 0) )
    {
        g_wifi_read_line_wait_crlf = true;
        g_wifi_read_line_buffer_timer = 0;

        while(g_wifi_read_line_buffer_index)
        {
            if(line1)
            {
#ifdef DEBUG_WIFI
                LogMessage("WifiWaitLine: esperando:%.80s / recibido: %.80s", line1, g_wifi_read_line_buffer);
#endif
                if( !memcmp(line1, g_wifi_read_line_buffer, strlen(line1)))
                {
                    LogMessage("[WifiWaitLine] Encontrado: %s", line1);
                    rc = 1;  /* Se recibio lo que se esperaba */
                }
            }
            if(rc == 0 && line2)
            {
#ifdef DEBUG_WIFI
                LogMessage("WifiWaitLine: esperando:%.80s / recibido: %.80s", line2, g_wifi_read_line_buffer);
#endif
                if( !memcmp(line2, g_wifi_read_line_buffer, strlen(line2)))
                {
                    LogMessage("[WifiWaitLine] Encontrado: %s", line2);
                    rc = 2;  /* Se recibio lo que se esperaba */
                }
            }
            if(rc == 0 && line3)
            {
#ifdef DEBUG_WIFI
                LogMessage("WifiWaitLine: esperando:%.80s / recibido: %.80s", line3, g_wifi_read_line_buffer);
#endif
                if( !memcmp(line3, g_wifi_read_line_buffer, strlen(line3)))
                {
                    LogMessage("[WifiWaitLine] Encontrado: %s", line3);
                    rc = 3;  /* Se recibio lo que se esperaba */
                }
            }
            if(rc == 0 && line4)
            {
#ifdef DEBUG_WIFI
                LogMessage("WifiWaitLine: esperando:%.80s / recibido: %.80s", line4, g_wifi_read_line_buffer);
#endif
                if( !memcmp(line4, g_wifi_read_line_buffer, strlen(line4)))
                {
                    LogMessage("[WifiWaitLine] Encontrado: %s", line4);
                    rc = 4;  /* Se recibio lo que se esperaba */
                }
            }
            if(rc == 0 && line5)
            {
#ifdef DEBUG_WIFI
                LogMessage("WifiWaitLine: esperando:%.80s / recibido: %.80s", line5, g_wifi_read_line_buffer);
#endif
                if( !memcmp(line5, g_wifi_read_line_buffer, strlen(line5)))
                {
                    LogMessage("[WifiWaitLine] Encontrado: %s", line5);
                    rc = 5;  /* Se recibio lo que se esperaba */
                }
            }
            
            if(rc == 0)
            {
                rc = WifiCheckUnsol();
            }
            else
            {
                g_wifi_read_line_buffer_index = 0;
            }
        }
    }
    else if(g_wifi_read_line_buffer_index == WIFI_READ_LINE_BUFFER_LEN)
    {
        /*+IPD*/
        if(g_wifi_read_line_buffer[0] == '+' &&
           g_wifi_read_line_buffer[1] == 'I' &&
           g_wifi_read_line_buffer[2] == 'P' &&
           g_wifi_read_line_buffer[3] == 'D'  )
        {
            rc = WifiCheckUnsol();
            g_wifi_wait_more_data = true;
            g_wifi_read_line_buffer_index = 0;
        }
    }
    return rc;
}

int WifiWaitPrompt(const char prompt)
{
    while(UART1_IsRxReady())
    {
        if(UART1_Read() == prompt) return 1;
    }
    return 0;
}

int WifiCheckUnsol( void )
{
    char str[16];
    int id;
    int len;
    char *p;
    char tmp_buffer[WIFI_READ_LINE_BUFFER_LEN+1];

    if( !memcmp(g_wifi_read_line_buffer, "+IPD,", 5)) /* Datos socket */
    {
        /*
        +IPD,0,442:<datos>
        */
        LogMessage("[WifiCheckUnsol] %-.2000s", g_wifi_read_line_buffer);

        id = 0;
        len = 0;
        
        if(str_section(g_wifi_read_line_buffer, ',', 1, str))
        {
            id = atoi(str);
        }
        if(str_section(g_wifi_read_line_buffer, ',', 2, str))
        {
            len = atoi(str);
        }
        /* dejo en el buffer solo el HTTP */
        p = strchr(g_wifi_read_line_buffer, ':');
        if(p)
        {
            if( g_wifi_read_line_buffer_index == WIFI_READ_LINE_BUFFER_LEN ||
                ( http_buffer_output.download && 
                  (strstr(g_wifi_read_line_buffer, "Content-Type: text/csv") ||
                   strstr(g_wifi_read_line_buffer, "Content-Type: image/bmp"))) )
            {
                /* Si el buffer está lleno debe ser un download */
                if(HTTPInitTmpFile(g_wifi_read_line_buffer, g_wifi_read_line_buffer_index) < 0)
                {
                    /* No había download pendiente */
                    LogMessage("[WifiCheckUnsol] HTTP input overflow: %-.100s", g_wifi_read_line_buffer);
                }
                g_wifi_read_line_buffer_index = 0;
            }
            else
            {
	            p++;
	            tmp_buffer[0] = 0;
	            if(*(p+len))
	            {
	                strcpy(tmp_buffer, (p+len));
	            }
	            *(p+len) = 0;
	            HTTPAddInput(id, p);
	            if(tmp_buffer[0])
	            {
	                strcpy(g_wifi_read_line_buffer, tmp_buffer);
	                g_wifi_read_line_buffer_index = strlen(g_wifi_read_line_buffer);
	                LogMessage("[WifiCheckUnsol] Resto: %-.100s", g_wifi_read_line_buffer);
	            }
	            else
	            {
	            	g_wifi_read_line_buffer_index = 0;
	            }
			}            
        }
        else
        {
        	g_wifi_read_line_buffer_index = 0;
        }
    }
    else if( !memcmp(g_wifi_read_line_buffer, "WIFI CONNECTED", 14))
    {
        g_wifi_read_line_buffer_index = 0;
        LogMessage("[WIFI] Conectado al AP [%s].", 
                ((g_wifi_info.status.wifi_ap_select == 1)?g_wifi_info.f.config.wifi_ap1:g_wifi_info.f.config.wifi_ap2));
        g_wifi_info.status.wifi_conectado = g_wifi_info.status.wifi_ap_select;
    }
    else if( !memcmp(g_wifi_read_line_buffer, "WIFI DISCONNECTED", 17))
    {
        g_wifi_read_line_buffer_index = 0;
        LogMessage("[WiFi] Deconectado del AP.");
        if(g_wifi_info.status.wifi_conectado)
        {
            g_wifi_info.status.wifi_conectado = 0;
            g_wifi_info.status.wifi_ap_select = (g_wifi_info.status.wifi_ap_select == 1)?2:1;
            g_wifi_info.status.wifi_start_st = true;
        }
    }
    else if( !memcmp(g_wifi_read_line_buffer, "+STA_CONNECTED", 8))
    {
        g_wifi_read_line_buffer_index = 0;
        LogMessage("[WiFi] Cliente conectado.");
        /* Cliente conectado al AP */
        g_wifi_timer_ap = 0; // para que no salga de modo AP
    }
    else if( !memcmp(g_wifi_read_line_buffer, "+STA_DISCONNECTED", 8))
    {
        g_wifi_read_line_buffer_index = 0;
        LogMessage("[WiFi] Cliente desconectado.");
        /* Cliente desconectado al AP */
        g_wifi_timer_ap = 50; // para que salga en seguida del modo AP
    }
    else if( !memcmp(g_wifi_read_line_buffer, "+CIFSR:", 7))
    {
        g_wifi_read_line_buffer_index = 0;
        /* AT+CIFSR
         * +CIFSR:STAIP,"0.0.0.0"
         * +CIFSR:STAMAC,"80:7d:3a:62:03:60"
         */
        if( !memcmp(&g_wifi_read_line_buffer[7], "STAMAC", 6))
        {
            LogMessage("[WiFi] MAC %.17s", &g_wifi_read_line_buffer[15]);
            g_wifi_info.status.hw_mac[0]  = g_wifi_read_line_buffer[15];
            g_wifi_info.status.hw_mac[1]  = g_wifi_read_line_buffer[16];
            g_wifi_info.status.hw_mac[2]  = g_wifi_read_line_buffer[18];
            g_wifi_info.status.hw_mac[3]  = g_wifi_read_line_buffer[19];
            g_wifi_info.status.hw_mac[4]  = g_wifi_read_line_buffer[21];
            g_wifi_info.status.hw_mac[5]  = g_wifi_read_line_buffer[22];
            g_wifi_info.status.hw_mac[6]  = g_wifi_read_line_buffer[24];
            g_wifi_info.status.hw_mac[7]  = g_wifi_read_line_buffer[25];
            g_wifi_info.status.hw_mac[8]  = g_wifi_read_line_buffer[27];
            g_wifi_info.status.hw_mac[9]  = g_wifi_read_line_buffer[28];
            g_wifi_info.status.hw_mac[10] = g_wifi_read_line_buffer[30];
            g_wifi_info.status.hw_mac[11] = g_wifi_read_line_buffer[31];
            g_wifi_info.status.hw_mac[12] = 0;
        }
    }
    else if( !memcmp(g_wifi_read_line_buffer, "AT version:", 11))
    {
        g_wifi_read_line_buffer_index = 0;
        /*
         * AT version:1.6.2.0(Apr 13 2018 11:10:59)
         * AT version:1.7.4.0(May 11 2020 19:13:04)
         */
        LogMessage("[WiFi] %s.", g_wifi_read_line_buffer);
        strncpy(g_wifi_info.status.wifi_at, &g_wifi_read_line_buffer[11], sizeof(g_wifi_info.status.wifi_at)-1);
        //if(strstr(g_wifi_read_line_buffer, "1.6.2.0"))
        //{
        //    LogMessage("[WiFi] El firware necesita upgrade.");
        //    g_wifi_need_upgrade = true;
        //}
    }
    else if( !memcmp(g_wifi_read_line_buffer, "SDK version:", 12))
    {
        g_wifi_read_line_buffer_index = 0;
        LogMessage("[WiFi] %s.", g_wifi_read_line_buffer);
        strncpy(g_wifi_info.status.wifi_sdk, &g_wifi_read_line_buffer[12], sizeof(g_wifi_info.status.wifi_sdk)-1);
    }
    else if( !memcmp(g_wifi_read_line_buffer, "compile time:", 13))
    {
        g_wifi_read_line_buffer_index = 0;
        LogMessage("[WiFi] %s.", g_wifi_read_line_buffer);
    }
    else
    {
        g_wifi_read_line_buffer_index = 0;
    }
    return 0;
}

void WifiSwitchHost( void )
{
    if(g_wifi_host == &g_wifi_info.f.config.wifi_host1[0] && g_wifi_info.f.config.wifi_host2[0] != 0)
    {
        g_wifi_host = &g_wifi_info.f.config.wifi_host2[0];
        g_wifi_port = (g_wifi_info.f.config.wifi_host2_port)?g_wifi_info.f.config.wifi_host2_port:((g_wifi_info.f.config.enable_https)?443:80);
    }
    else
    {
        g_wifi_host = &g_wifi_info.f.config.wifi_host1[0];
        g_wifi_port = (g_wifi_info.f.config.wifi_host1_port)?g_wifi_info.f.config.wifi_host1_port:((g_wifi_info.f.config.enable_https)?443:80);
    }
}

int WifiTaskInit( void )
{
    int rc = 0;

    if(g_wifi_task_stm_step != g_wifi_task_stm_step_last)
    {
        LogMessage("[WifiTaskInit] STM %i -> %i",
                g_wifi_task_stm_step_last,
                g_wifi_task_stm_step);
        g_wifi_task_stm_step_last = g_wifi_task_stm_step;
    }

    switch(g_wifi_task_stm_step)
    {
        case 0:
            LogMessage("[WifiTaskInit] RESTORE factory defaults.");
            /* Restore a fabrica y espero a que el modulo se reinicie */
            WifiWriteLine("AT+RESTORE");
            g_wifi_task_stm_step++;
            g_wifi_task_stm_delay = 0;
            g_wifi_task_stm_timeout = 100;
            break;
        case 1:
            if(WifiWaitLine("ready", 0, 0, 0, 0) || g_wifi_task_stm_timeout == 0)
            {
                g_wifi_task_stm_step++;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
            }
            break;
        case 2:
            /* ECHO OFF */
            WifiWriteLine("ATE0");
            g_wifi_task_stm_step++;
            g_wifi_task_stm_delay = 0;
            g_wifi_task_stm_timeout = 25;
            break;
        case 3:
            if(WifiWaitLine("OK", 0, 0, 0, 0))
            {
                g_wifi_task_stm_step++;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
            }
            else if(g_wifi_task_stm_timeout == 0)
            {
                rc = (-1);
            }
            break;
        case 4:
            /* Version */
            WifiWriteLine("AT+GMR");
            g_wifi_task_stm_step++;
            g_wifi_task_stm_delay = 0;
            g_wifi_task_stm_timeout = 25;
            break;
        case 5:
            if(WifiWaitLine("OK", 0, 0, 0, 0))
            {
                g_wifi_task_stm_step++;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
            }
            else if(g_wifi_task_stm_timeout == 0)
            {
                rc = (-1);
            }
            break;
        case 6:
            /* Check soporte SSL */
            WifiWriteLine("AT+CIPSSLCCONF?");
            g_wifi_task_stm_step++;
            g_wifi_task_stm_delay = 0;
            g_wifi_task_stm_timeout = 25;
            break;
        case 7:
            if(WifiWaitLine("OK", 0, 0, 0, 0))
            {
                g_wifi_task_stm_step++;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
                g_wifi_info.status.wifi_ssl_capable = true;
                LogMessage("[WifiTaskInit] Con soporte SSL");
                // Puede faltar
                // AT+CIPSSLSIZE=4096
            }
            else if(g_wifi_task_stm_timeout == 0)
            {
                g_wifi_task_stm_step++;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
                g_wifi_info.status.wifi_ssl_capable = false;
                LogMessage("[WifiTaskInit] Sin soporte SSL");
                //g_wifi_need_upgrade = true;
            }
            break;
        case 8:
            if( !g_wifi_info.status.wifi_start_ap &&
                ! g_wifi_info.status.wifi_start_st )
            {
                g_wifi_info.status.wifi_start_st = true;
            }
        default:
            rc = LOOP;
            break;
    }
    return rc;
}

int WifiTaskAPMode( void )
{
    int rc = 0;

    if(g_wifi_task_stm_step != g_wifi_task_stm_step_last)
    {
        LogMessage("[WifiTaskAPMode] STM %i -> %i",
                g_wifi_task_stm_step_last,
                g_wifi_task_stm_step);
        g_wifi_task_stm_step_last = g_wifi_task_stm_step;
    }

    switch(g_wifi_task_stm_step)
    {
        case 0:
            /* Bajo los servers */
            WifiWriteLine("AT+CIPSERVER=0");
            g_wifi_task_stm_step++;
            g_wifi_task_stm_delay = 0;
            g_wifi_task_stm_timeout = 10;
            g_wifi_info.status.wifi_mode = 0;
            g_wifi_info.status.host_conectado = 0;
            break;
        case 1:
            /* No controlo error */
            if(WifiWaitLine("OK", 0, 0, 0, 0) || g_wifi_task_stm_timeout == 0)
            {
                g_wifi_task_stm_step++;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
            }
            break;
        case 2:
            /* Modo AP+Sta */
            WifiWriteLine("AT+CWMODE=3");
            g_wifi_task_stm_step++;
            g_wifi_task_stm_delay = 0;
            g_wifi_task_stm_timeout = 25;
            break;
        case 3:
            if(WifiWaitLine("OK", 0, 0, 0, 0))
            {
                g_wifi_task_stm_step++;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
                g_wifi_info.status.wifi_mode = 3;
                g_wifi_info.status.wifi_ok = true;
            }
            else if(g_wifi_task_stm_timeout == 0)
            {
                rc = (-1);
            }
            break;
        case 4:
            /* MAC e IP */
            WifiWriteLine("AT+CIFSR");
            g_wifi_task_stm_step++;
            g_wifi_task_stm_delay = 0;
            g_wifi_task_stm_timeout = 25;
            break;
        case 5:
            if(WifiWaitLine("OK", 0, 0, 0, 0) || g_wifi_task_stm_timeout == 0)
            {
                g_wifi_task_stm_step++;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
            }
            break;
        case 6:
            /* Nombre del AP */
            sprintf(g_wifi_cmd, "AT+CWSAP=\"Dom32io-%s\",\"\",1,0,4,0",
                    &g_wifi_info.status.hw_mac[6]);
            WifiWriteLine(g_wifi_cmd);
            g_wifi_task_stm_step++;
            g_wifi_task_stm_delay = 0;
            g_wifi_task_stm_timeout = 25;
            break;
        case 7:
            if(WifiWaitLine("OK", 0, 0, 0, 0))
            {
                g_wifi_task_stm_step++;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
            }
            else if(g_wifi_task_stm_timeout == 0)
            {
                rc = (-1);
            }
            break;
        case 8:
            /* Multiconexion */
            WifiWriteLine("AT+CIPMUX=1");
            g_wifi_task_stm_step++;
            g_wifi_task_stm_delay = 0;
            g_wifi_task_stm_timeout = 25;
            break;
        case 9:
            if(WifiWaitLine("OK", 0, 0, 0, 0))
            {
                g_wifi_task_stm_step++;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
            }
            else if(g_wifi_task_stm_timeout == 0)
            {
                rc = (-1);
            }
            break;
        case 10:
            /* Instancias del server */
            WifiWriteLine("AT+CIPSERVERMAXCONN=1");
            g_wifi_task_stm_step++;
            g_wifi_task_stm_delay = 0;
            g_wifi_task_stm_timeout = 25;
            break;
        case 11:
            if(WifiWaitLine("OK", 0, 0, 0, 0))
            {
                g_wifi_task_stm_step++;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
            }
            else if(g_wifi_task_stm_timeout == 0)
            {
                /* Los módulos con firmware viejo no soportan este comando */
                LogMessage("[WifiTaskAPMode] AT+CIPSERVERMAXCONN no soportado.");
                g_wifi_task_stm_step++;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
            }
            break;
        case 12:
            /* Listen */
            WifiWriteLine("AT+CIPSERVER=1,80");
            g_wifi_task_stm_step++;
            g_wifi_task_stm_delay = 0;
            g_wifi_task_stm_timeout = 25;
            break;
        case 13:
            if(WifiWaitLine("OK", 0, 0, 0, 0))
            {
                LogMessage("[WifiTaskAPMode] ********** Modo Configuracion **********");
                g_wifi_task_stm_step++;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
            }
            else if(g_wifi_task_stm_timeout == 0)
            {
                rc = (-1);
            }
            break;
        case 14:
        default:
            g_wifi_timer_ap = TIMER_AP_VALUE;
            rc = LOOP;
            break;
    }
    return rc;
}

int WifiTaskSTMode( void )
{
    int rc = 0;
    char *ap, *pass;

    if(g_wifi_task_stm_step != g_wifi_task_stm_step_last)
    {
        LogMessage("[WifiTaskSTMode] STM %i -> %i",
                g_wifi_task_stm_step_last,
                g_wifi_task_stm_step);
        g_wifi_task_stm_step_last = g_wifi_task_stm_step;
    }

    switch(g_wifi_task_stm_step)
    {
        case 0:
            //LogMessage("--> Modo STA.\n");
            /* Bajo los servers */
            WifiWriteLine("AT+CIPSERVER=0");
            g_wifi_task_stm_step++;
            g_wifi_task_stm_delay = 0;
            g_wifi_task_stm_timeout = 10;
            g_wifi_info.status.wifi_mode = 0;
            g_wifi_info.status.host_conectado = 0;
            break;
        case 1:
            if(WifiWaitLine("OK", 0, 0, 0, 0) || g_wifi_task_stm_timeout == 0)
            {
                g_wifi_task_stm_step++;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
            }
            break;
        case 2:
            /* Modo Sta */
            WifiWriteLine("AT+CWMODE=1");
            g_wifi_task_stm_step++;
            g_wifi_task_stm_delay = 0;
            g_wifi_task_stm_timeout = 25;
            break;
        case 3:
            if(WifiWaitLine("OK", 0, 0, 0, 0))
            {
                g_wifi_task_stm_step++;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
                g_wifi_info.status.wifi_ok = true;
            }
            else if(g_wifi_task_stm_timeout == 0)
            {
                //LogMessage("TIMEOUT esperando OK\n");
                rc = (-1);
            }
            break;
        case 4:
            /* Multiconexion */
            WifiWriteLine("AT+CIPMUX=1");
            g_wifi_task_stm_step++;
            g_wifi_task_stm_delay = 0;
            g_wifi_task_stm_timeout = 25;
            break;
        case 5:
            if(WifiWaitLine("OK", 0, 0, 0, 0))
            {
                g_wifi_task_stm_step++;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
            }
            else if(g_wifi_task_stm_timeout == 0)
            {
                //LogMessage("TIMEOUT esperando OK\n");
                rc = (-1);
            }
            break;
        case 6:
            /* Instancias del server */
            WifiWriteLine("AT+CIPSERVERMAXCONN=1");
            g_wifi_task_stm_step++;
            g_wifi_task_stm_delay = 0;
            g_wifi_task_stm_timeout = 25;
            break;
        case 7:
            if(WifiWaitLine("OK", 0, 0, 0, 0))
            {
                g_wifi_task_stm_step++;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
            }
            else if(g_wifi_task_stm_timeout == 0)
            {
                LogMessage("[WifiTaskSTMode] AT+CIPSERVERMAXCONN no soportado.");
                g_wifi_task_stm_step++;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
            }
            break;
        case 8:
            /* MAC e IP */
            WifiWriteLine("AT+CIFSR");
            g_wifi_task_stm_step++;
            g_wifi_task_stm_delay = 0;
            g_wifi_task_stm_timeout = 25;
            break;
        case 9:
            if(WifiWaitLine("OK", 0, 0, 0, 0) || g_wifi_task_stm_timeout == 0)
            {
                g_wifi_task_stm_step++;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
            }
            break;
        case 10/*8*/:
            /* Listen */
            WifiWriteLine("AT+CIPSERVER=1,80");
            g_wifi_task_stm_step++;
            g_wifi_task_stm_delay = 0;
            g_wifi_task_stm_timeout = 25;
            break;
        case 11/*9*/:
            if(WifiWaitLine("OK", 0, 0, 0, 0))
            {
                g_wifi_task_stm_step++;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
            }
            else if(g_wifi_task_stm_timeout == 0)
            {
                //LogMessage("TIMEOUT esperando OK\n");
                rc = (-1);
            }
            break;
        case 12/*10*/:
            /* Reconnect */
            WifiWriteLine("AT+CWAUTOCONN=1");
            g_wifi_task_stm_step++;
            g_wifi_task_stm_delay = 0;
            g_wifi_task_stm_timeout = 25;
            break;
        case 13/*11*/:
            if(WifiWaitLine("OK", 0, 0, 0, 0))
            {
                g_wifi_task_stm_step++;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
                g_wifi_info.status.wifi_mode = 1;
            }
            else if(g_wifi_task_stm_timeout == 0)
            {
                //LogMessage("TIMEOUT esperando OK\n");
                rc = (-1);
            }
            break;
        case 14/*12*/:
            /* Si el actual no existe cambio al que exista */
            if(g_wifi_info.status.wifi_ap_select == 1 &&
               g_wifi_info.f.config.wifi_ap1[0] == 0 &&
               g_wifi_info.f.config.wifi_ap2[0] != 0)
            {
                g_wifi_info.status.wifi_ap_select = 2;
            }
            
            if(g_wifi_info.status.wifi_ap_select == 2 &&
               g_wifi_info.f.config.wifi_ap2[0] == 0 &&
               g_wifi_info.f.config.wifi_ap1[0] != 0)
            {
                g_wifi_info.status.wifi_ap_select = 1;
            }
            
            ap = ((g_wifi_info.status.wifi_ap_select == 1)?g_wifi_info.f.config.wifi_ap1:g_wifi_info.f.config.wifi_ap2);
            pass = ((g_wifi_info.status.wifi_ap_select == 1)?g_wifi_info.f.config.wifi_ap1_pass:g_wifi_info.f.config.wifi_ap2_pass);
            if(*ap)
            {
                LogMessage("[WifiTaskSTMode] Conectando con AP %s (%s).", ap, pass);
                /* Connect to AP */
                if(*pass)
                {
                    /* Con clave */
                    sprintf(g_wifi_cmd, "AT+CWJAP=\"%s\",\"%s\"", ap, pass);
                }
                else
                {
                    /* Sin clave */
                    sprintf(g_wifi_cmd, "AT+CWJAP=\"%s\"", ap);
                }
                WifiWriteLine(g_wifi_cmd);
                g_wifi_task_stm_step++;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 300;
                g_wifi_info.status.wifi_conectado = 0;
                g_wifi_info.status.host_conectado = 0;
            }
            else
            {
                /* No hay AP definido */
                LogMessage("[WifiTaskSTMode] ERROR: No hay AP definido.");
                rc = LOOP;
            }
            break;
        case 15/*13*/:
            if(WifiWaitLine("OK", 0, 0, 0, 0))
            {
                LogMessage("[WifiTaskSTMode] ********** Modo normal **********");
                g_wifi_task_stm_step++;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
                g_wifi_info.status.control_notificar = 1;
            }
            else if(g_wifi_task_stm_timeout == 0)
            {
                LogMessage("[WifiTaskSTMode] Error de conexion con AP.");
                LogMessage("[WifiTaskSTMode] Reconectando...");
                g_wifi_info.status.wifi_ap_select = (g_wifi_info.status.wifi_ap_select == 1)?2:1;
                g_wifi_info.status.wifi_start_st = true;
                rc = LOOP;
            }
            break;
            // Puede faltar                
            // AT+CIPSNTPCFG=1,8	 // set SNTP timezone
            // AT+CIPSNTPTIME? // get SNTP time
        default:
            rc = LOOP;
            break;
    }
    return rc;
}

int WifiTaskUpgrade( void )
{
    int rc = 0;

    if(g_wifi_task_stm_step != g_wifi_task_stm_step_last)
    {
        LogMessage("[WifiTaskUpgrade] STM %i -> %i",
                g_wifi_task_stm_step_last,
                g_wifi_task_stm_step);
        g_wifi_task_stm_step_last = g_wifi_task_stm_step;
    }

    switch(g_wifi_task_stm_step)
    {
        case 0:
            LogMessage("[WifiTaskUpgrade] Firmware Upgrade.\n");
            /* Bajo los servers */
            WifiWriteLine("AT+CIPUPDATE");
            g_wifi_task_stm_step = 1;
            g_wifi_task_stm_delay = 0;
            g_wifi_task_stm_timeout = 1200;
            break;
        case 1:
            if(WifiWaitLine("OK", 0, 0, 0, 0))
            {
                g_wifi_task_stm_step = 2;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
            }
            else if(g_wifi_task_stm_timeout == 0)
            {
                LogMessage("[WifiTaskUpgrade] Upgrade TIMEOUT.");
                g_wifi_task_stm_step = 0;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
            }
            break;
        default:
            rc = RESET;
            break;
    }
    return rc;
}

int WifiTaskHttpResp( void )
{
    int rc = 0;

    if(g_wifi_task_stm_step != g_wifi_task_stm_step_last)
    {
        LogMessage("[WifiTaskHttpResp] STM %i -> %i",
                g_wifi_task_stm_step_last,
                g_wifi_task_stm_step);
        g_wifi_task_stm_step_last = g_wifi_task_stm_step;
    }
    switch(g_wifi_task_stm_step)
    {
        case 0:
            g_wifi_task_retry = 0;
            if(http_buffer_response.type && 
               http_buffer_response.id >= 0) g_wifi_task_stm_step++;
            else rc = LOOP;
            break;
        case 1: /* Preparando envío de datos */
            LogMessage("[WifiTaskHttpResp] Enviando");
            sprintf(g_wifi_cmd, "AT+CIPSEND=%d,%d", 
                    http_buffer_response.id, 
                    strlen(http_buffer_response.data));
            WifiWriteLine(g_wifi_cmd);
            g_wifi_task_stm_step++;
            g_wifi_task_stm_delay = 0;
            g_wifi_task_stm_timeout = 15;
            break;
        case 2: /* Esperando prompt para envío de datos */
            if(WifiWaitPrompt('>'))
            {
                g_wifi_task_stm_step++;
                g_wifi_recv_error_count = 0;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
            }
            else if(g_wifi_task_stm_timeout == 0)
            {
                LogMessage("[WifiTaskHttpResp] TIME-OUT: Esperando PROMPT");
                g_wifi_task_stm_step = 7; /* -> Send ERROR */
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
            }
            break;
        case 3: /* Enviando datos */
            WifiWrite(http_buffer_response.data);
            g_wifi_task_stm_step++;
            g_wifi_task_stm_delay = 0;
            g_wifi_task_stm_timeout = 25;
            break;
        case 4: /* Esperando Ok de envío de datos */
            if(WifiWaitLine("SEND OK", 0, 0, 0, 0))
            {
                /* Si es una respuesta saliente sigue un close */
                LogMessage("[WifiTaskHttpResp] SEND Ok");
                g_wifi_task_stm_step ++;
                g_wifi_recv_error_count = 0;
                g_wifi_task_stm_delay = 5;
                g_wifi_task_stm_timeout = 0;
            }
            else if(g_wifi_task_stm_timeout == 0)
            {
                /* Si es una respuesta saliente sigue un close */
                LogMessage("[WifiTaskHttpResp] TIME-OUT: Esperando OK de SEND");
                g_wifi_task_stm_step++;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
            }
            break;
        case 5: /* CLOSE */
            LogMessage("[WifiTaskHttpResp] Cerrando");
            sprintf(g_wifi_cmd, "AT+CIPCLOSE=%d", http_buffer_response.id);
            WifiWriteLine(g_wifi_cmd);
            g_wifi_task_stm_step++;
            g_wifi_task_stm_delay = 0;
            g_wifi_task_stm_timeout = 25;
            break;
        case 6: /* Esperando el Ok del CLOSE */
            sprintf(g_wifi_cmd, "%d,CLOSED", http_buffer_response.id);
            if(WifiWaitLine(g_wifi_cmd, "UNLINK", "OK", 0, 0))
            {
                LogMessage("[WifiTaskHttpResp] CLOSE Ok");
                g_wifi_task_stm_step+=2;
                g_wifi_recv_error_count = 0;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
            }
            else if(g_wifi_task_stm_timeout == 0)
            {
                LogMessage("[WifiTaskHttpResp] TIME-OUT: Esperando Ok de CLOSE");
                g_wifi_task_stm_step++;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
            }
            break;
        case 7: /* Send Error o time-out */
            sprintf(g_wifi_cmd, "AT+CIPCLOSE=%d", http_buffer_response.id);
            WifiWriteLine(g_wifi_cmd);
            g_wifi_task_stm_step++;
            g_wifi_task_stm_delay = 15;
            g_wifi_task_stm_timeout = 0;
            break;
        case 8: /* Borro el mensaje de respuesta */
            http_buffer_response.type = 0;
            g_wifi_task_stm_step++;
            g_wifi_recv_error_count = 0;
            g_wifi_task_stm_delay = 0;
            g_wifi_task_stm_timeout = 0;
            break;
        default:
            rc = LOOP;
            break;
    }
    return rc;
}

int WifiTaskHttpReq( void )
{
    int rc = 0;

    if(g_wifi_task_stm_step != g_wifi_task_stm_step_last)
    {
        LogMessage("[WifiTaskHttpReq] STM %i -> %i",
                g_wifi_task_stm_step_last,
                g_wifi_task_stm_step);
        g_wifi_task_stm_step_last = g_wifi_task_stm_step;
    }

    switch(g_wifi_task_stm_step)
    {
        case 0: /* Conectando */
            g_wifi_task_retry = 0;
            g_wifi_task_stm_step++;
        case 1: /* rtome para retry */
            LogMessage("[WifiTaskHttpReq] Conectando a: %s:%i",  g_wifi_host, g_wifi_port);
            sprintf(g_wifi_cmd, "AT+CIPSTART=%d,\"%s\",\"%s\",%d",
                    http_buffer_output.id,
                    (g_wifi_info.f.config.enable_https)?"SSL":"TCP",
                    g_wifi_host,
                    g_wifi_port);
            WifiWriteLine(g_wifi_cmd);
            g_wifi_task_stm_step++;
            g_wifi_task_stm_delay = 0;
            g_wifi_task_stm_timeout = 30;
            break;
        case 2: /* Esperando Ok de conexión */
            if(WifiWaitLine("OK", 0, 0, 0, 0))
            {
                g_wifi_task_stm_step++;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
            }
            else if(g_wifi_task_stm_timeout == 0)
            {
                LogMessage("[WifiTaskHttpReq] Open Error");
                g_wifi_task_stm_step = 11; /* -> Open ERROR */
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
            }
            break;
        case 3: /* Preparando envío de datos */
            LogMessage("[WifiTaskHttpReq] Enviando");
            sprintf(g_wifi_cmd, "AT+CIPSEND=%d,%d",
                    http_buffer_output.id,
                    strlen(http_buffer_output.data));
            WifiWriteLine(g_wifi_cmd);
            g_wifi_task_stm_step++;
            g_wifi_task_stm_delay = 0;
            g_wifi_task_stm_timeout = 15;
            break;
        case 4: /* Esperando prompt para envío de datos */
            if(WifiWaitPrompt('>'))
            {
                g_wifi_task_stm_step++;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
            }
            else if(g_wifi_task_stm_timeout == 0)
            {
                LogMessage("[WifiTaskHttpReq] TIME-OUT: Esperando PROMPT");
                g_wifi_task_stm_step = 10; /* -> Send ERROR */
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
            }
            break;
        case 5: /* Enviando datos */
            WifiWrite(http_buffer_output.data);
            g_wifi_task_stm_step++;
            g_wifi_task_stm_delay = 0;
            g_wifi_task_stm_timeout = 25;
            break;
        case 6: /* Esperando Ok de envío de datos */
            if(WifiWaitLine("SEND OK", 0, 0, 0, 0))
            {
                LogMessage("[WifiTaskHttpReq] SEND Ok");
                /* Si es un requerimiento saliente espero respuesta */
                g_wifi_send_error_count = 0;
                g_wifi_task_stm_step++;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = http_buffer_output.timeout;
                g_end_download = false;
            }
            else if(g_wifi_task_stm_timeout == 0)
            {
                LogMessage("[WifiTaskHttpReq] TIME-OUT: Esperando Ok de SEND");
                g_wifi_send_error_count = 0;
                g_wifi_task_stm_step = 10; /* Send Error */
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
            }
            break;
        case 7: /* Esperando CLOSE del server o datos */
            sprintf(g_wifi_cmd, "%d,CLOSED", http_buffer_output.id);
            if(WifiWaitLine(g_wifi_cmd, "UNLINK", "OK", 0, 0) || g_end_download)
            {
                /* Libero el buffer */
                http_buffer_output.type = 0;
                LogMessage("[WifiTaskHttpReq] CLOSE del Server");
                rc = LOOP;
            }
            else if(g_wifi_task_stm_timeout == 0)
            {
                /* Libero el buffer */
                http_buffer_output.type = 0;
                g_wifi_task_stm_step++;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
            }
            break;
        case 8:
            LogMessage("[WifiTaskHttpReq] Cerrando");
            sprintf(g_wifi_cmd, "AT+CIPCLOSE=%d", http_buffer_output.id);
            WifiWriteLine(g_wifi_cmd);
            g_wifi_task_stm_step++;
            g_wifi_task_stm_delay = 0;
            g_wifi_task_stm_timeout = 15;
            break;
        case 9: /* Listo */
            sprintf(g_wifi_cmd, "%d,CLOSED", http_buffer_output.id);
            if(WifiWaitLine(g_wifi_cmd, "UNLINK", "OK", 0, 0))
            {
                LogMessage("[WifiTaskHttpReq] Cerrado");
                rc = LOOP;
            }
            else if(g_wifi_task_stm_timeout == 0)
            {
                LogMessage("[WifiTaskHttpReq] TIME-OUT: Esperando Ok de CLOSE");
                rc = LOOP;
            }
            break;
        case 10:
            /* Send Error */
            sprintf(g_wifi_cmd, "AT+CIPCLOSE=%d", http_buffer_output.id);
            WifiWriteLine(g_wifi_cmd);
            g_wifi_task_stm_step++;
            g_wifi_task_stm_delay = 15;
            g_wifi_task_stm_timeout = 0;
            break;
        case 11: /* Open Error y Reintentos */
            g_wifi_send_error_count++;
            g_wifi_task_retry++;
            if(g_wifi_task_retry < 2)
            {
                WifiSwitchHost();
                g_wifi_task_stm_step = 1; /* -> Retry */
                g_wifi_task_stm_delay = 15;
                g_wifi_task_stm_timeout = 0;
            }
            else
            {
                g_wifi_task_stm_step++;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
                /* Libero el buffer */
                http_buffer_output.type = 0;
                http_buffer_output.download = false;
                g_wifi_info.status.host_conectado = 0;
            }
            break;
        default:
            rc = LOOP;
            break;
    }
    return rc;
}

int WifiTaskLoop( void )
{
    int rc = 0;

    if(g_wifi_send_error_count >= 5)
    {
        return (-1);
    }
    else if(g_wifi_recv_error_count >= 5)
    {
        return (-1);
    }
    /* Espero no solicitados */
    else if((rc = WifiWaitLine(0, 0, 0, 0, 0)) > 0)
    {
        return rc;
    }
    else if(g_wifi_info.status.wifi_start_ap)
    {
        g_wifi_info.status.wifi_start_ap = false;
        return APMODE;
    }
    else if(g_wifi_info.status.wifi_start_st)
    {
        g_wifi_info.status.wifi_start_st = false;
        return STMODE;
    }
    else if(http_buffer_response.type)
    {
        return HTTP_RESP;
    }
    else if(http_buffer_output.type)
    {
        return HTTP_REQ;
    }
    else if(g_wifi_need_upgrade && g_wifi_info.status.wifi_conectado)
    {
        return UPGRADE;
    }
    
    return 0;
}

int WifiTaskListAP( void )
{
    int rc = 0;

    if(g_wifi_task_stm_step != g_wifi_task_stm_step_last)
    {
        LogMessage("[WifiTaskListAP] STM %i -> %i",
                g_wifi_task_stm_step_last,
                g_wifi_task_stm_step);
        g_wifi_task_stm_step_last = g_wifi_task_stm_step;
    }

    switch(g_wifi_task_stm_step)
    {
        case 0:
            //LogMessage("--> List AP.\n");
            WifiWriteLine("AT+CWLAP");
            g_wifi_task_stm_step = 1;
            g_wifi_task_stm_delay = 0;
            g_wifi_task_stm_timeout = 50;
            break;
        case 1:
            if(WifiWaitLine("OK", 0, 0, 0, 0))
            {
                g_wifi_task_stm_step = 2;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
            }
            else if(g_wifi_task_stm_timeout == 0)
            {
                //LogMessage("TIMEOUT esperando Ok\n");
                rc = (-1);
            }
            break;
        default:
            g_wifi_task_stm_step = 0;
            g_wifi_task_stm_delay = 0;
            g_wifi_task_stm_timeout = 0;
            rc = LOOP;
            break;
    }
    return rc;
}

/* *****************************************************************************
 * 
 * ************************************************************************** */
void WiFiInit( void )
{
    g_wifi_task_stm = RESET;
    g_wifi_task_stm_last = INVALID;
    g_wifi_task_stm_delay = 0;
    g_wifi_need_upgrade = false;
    g_wifi_send_error_count = 0;
    g_wifi_recv_error_count = 0;
    
    g_wifi_host = &g_wifi_info.f.config.wifi_host1[0];
    g_wifi_port = (g_wifi_info.f.config.wifi_host1_port)?g_wifi_info.f.config.wifi_host1_port:((g_wifi_info.f.config.enable_https)?443:80);
    
    if(g_wifi_info.f.config.start_config_mode)
    {
        g_wifi_info.status.wifi_start_ap = true;
    }
    else
    {
        g_wifi_info.status.wifi_start_st = true;
        /* En el proximo arranque inicia en modo config */
        g_wifi_info.f.config.start_config_mode = true;
        ConfigSave();
    }
}

/* *****************************************************************************
 * 
 * ************************************************************************** */
void WiFiTask( void )
{
    int rc;
    
    if(g_wifi_task_stm_delay) return;
    
    if(g_wifi_task_stm != g_wifi_task_stm_last)
    {
        LogMessage("[WiFiTask] STM %s -> %s",
                task_stm_label[g_wifi_task_stm_last],
                task_stm_label[g_wifi_task_stm]);
        g_wifi_task_stm_last = g_wifi_task_stm;
    }

    switch(g_wifi_task_stm)
    {
        case INVALID:
        case RESET:
			LogMessage("[WiFiTask] RESET");
            WIFI_RESET_SetLow();
            g_wifi_read_line_buffer_index = 0;
            g_wifi_read_line_buffer_timer = 0;
            g_wifi_read_line_wait_crlf = true;
            g_wifi_wait_more_data = false;

            g_wifi_send_error_count = 0;
            g_wifi_recv_error_count = 0;

            g_wifi_info.status.wifi_conectado = 0;
            g_wifi_info.status.wifi_ap_select = 1;
            g_wifi_timer_ap = 0;
            g_wifi_info.status.wifi_mode = 0;
            g_wifi_info.status.host_conectado = 0;
            
            WifiTaskSTMSet(START, 20, 0);
            break;
        case START:
			//LogMessage("START\n");
            WIFI_RESET_SetHigh();
            WifiTaskSTMSet(CLEAN, 5, 20);
            break;
        case CLEAN:
            /* Me quedo recibiendo un rato la basura de la POST del ESP-02 */
            /* No es basura, pero se ve como basura porque esta a otra velocidad */
            WifiWaitLine(0, 0, 0, 0, 0);
            if(g_wifi_task_stm_timeout == 0)
            {
                WifiTaskSTMSet(INIT, 0, 0);
            }
            break;
        case INIT:
            rc = WifiTaskInit();
            if(rc > 0)      WifiTaskSTMSet(rc, 0, 0);
            else if(rc < 0) WifiTaskSTMSet(RESET, 0, 0);
            break;
        case UPGRADE:
            rc = WifiTaskUpgrade();
            if(rc > 0)      WifiTaskSTMSet(rc, 0, 0);
            else if(rc < 0) WifiTaskSTMSet(RESET, 0, 0);
            break;
        case APMODE:
            rc = WifiTaskAPMode();
            if(rc > 0)      WifiTaskSTMSet(rc, 0, 0);
            else if(rc < 0) WifiTaskSTMSet(RESET, 0, 0);
            break;
        case STMODE:
            rc = WifiTaskSTMode();
            if(rc > 0)      WifiTaskSTMSet(rc, 0, 0);
            else if(rc < 0) WifiTaskSTMSet(RESET, 0, 0);
            break;
        case LISTAP:
            rc = WifiTaskListAP();
            if(rc > 0)      WifiTaskSTMSet(rc, 0, 0);
            else if(rc < 0) WifiTaskSTMSet(RESET, 0, 0);
            break;
        case HTTP_RESP:
            rc = WifiTaskHttpResp();
            if(rc > 0)      WifiTaskSTMSet(rc, 0, 0);
            else if(rc < 0) WifiTaskSTMSet(RESET, 0, 0);
            break;
        case HTTP_REQ:
            rc = WifiTaskHttpReq();
            if(rc > 0)      WifiTaskSTMSet(rc, 0, 0);
            else if(rc < 0) WifiTaskSTMSet(RESET, 0, 0);
            break;
        case LOOP:
            rc = WifiTaskLoop();
            if(rc > 0)      WifiTaskSTMSet(rc, 0, 0);
            else if(rc < 0) WifiTaskSTMSet(RESET, 0, 0);
            break;
        default:    /* ERROR */
			LogMessage("***** SOFTWARE ERROR ***** RESET *****");
            WifiTaskSTMSet(RESET, 0, 0);
            break;
    }
}

/* *****************************************************************************
 *    Se llama 10 veces por egundo
 * ************************************************************************** */
void WiFiTimer( void )
{
    if(g_wifi_task_stm_delay) g_wifi_task_stm_delay--;
    if(g_wifi_task_stm_timeout) g_wifi_task_stm_timeout--;
    if(g_wifi_timer_ap)
    {
        g_wifi_timer_ap--;
        if(g_wifi_timer_ap == 0)
        {
            /* Vuelvo al modo normal */
            g_wifi_info.status.wifi_start_st = true;
        }
    }
    
    if(g_wifi_read_line_buffer_timer) g_wifi_read_line_buffer_timer--;
    
}

/* ************************************************************************* */
