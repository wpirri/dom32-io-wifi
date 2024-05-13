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
#define _IN_WIFI_C_
#include "wifi.h"
#undef _IN_WIFI_C_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "mcc_generated_files/uart1.h"
#include "mcc_generated_files/pin_manager.h"
#include "strfunc.h"
#include "control.h"
#include "http.h"
#include "config.h"
#include "clog.h"
#include "delay.h"
#include "sd/fsio.h"

//#define DEBUG_WIFI
#define WIFI_READ_LINE_BUFFER_LEN 1024
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
    HTTP,
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
    "HTTP",
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
int g_wifi_http_id;
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
bool g_wifi_fast_start;

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
#ifdef DEBUG_WIFI
	LogMessage("WifiWrite: %.80s", buffer);
#endif
    /* Reseteo el buffer de lectura */
    //g_wifi_read_line_buffer_index = 0;
    if(buffer)
    {
        while(*buffer)
        {
            UART1_Write((uint8_t)*buffer);
            buffer++;
        }
//        __delay_us(5);
    }
}

void WifiWriteLine( const char *line )
{
#ifdef DEBUG_WIFI
	LogMessage("WifiWriteLine: %.80s", line);
#endif
    /* Reseteo el buffer de lectura */
    //g_wifi_read_line_buffer_index = 0;
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
//    __delay_us(5);
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
            /* Se agrega el caracter a la l�nea */
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
        if((g_wifi_read_line_buffer_timer == 0))
        {
            /* time-out esperando mas datos */
            HTTPEndTmpFile(g_wifi_http_filename, 
                    g_wifi_read_line_buffer, 
                    g_wifi_read_line_buffer_index);
            g_wifi_wait_more_data = false;
            g_wifi_read_line_buffer_index = 0;
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

        if(g_wifi_read_line_buffer_index)
        {

            if(line1)
            {
#ifdef DEBUG_WIFI
                LogMessage("WifiWaitLine: esperando:%.80s / recibido: %.80s", line1, g_wifi_read_line_buffer);
#endif
                if( !memcmp(line1, g_wifi_read_line_buffer, strlen(line1)))
                {
                    rc = 1;  /* Se recibi� lo que se esperaba */
                }
            }
            if(rc == 0 && line2)
            {
#ifdef DEBUG_WIFI
                LogMessage("WifiWaitLine: esperando:%.80s / recibido: %.80s", line2, g_wifi_read_line_buffer);
#endif
                if( !memcmp(line2, g_wifi_read_line_buffer, strlen(line2)))
                {
                    rc = 2;  /* Se recibi� lo que se esperaba */
                }
            }
            if(rc == 0 && line3)
            {
#ifdef DEBUG_WIFI
                LogMessage("WifiWaitLine: esperando:%.80s / recibido: %.80s", line3, g_wifi_read_line_buffer);
#endif
                if( !memcmp(line3, g_wifi_read_line_buffer, strlen(line3)))
                {
                    rc = 3;  /* Se recibi� lo que se esperaba */
                }
            }
            if(rc == 0 && line4)
            {
#ifdef DEBUG_WIFI
                LogMessage("WifiWaitLine: esperando:%.80s / recibido: %.80s", line4, g_wifi_read_line_buffer);
#endif
                if( !memcmp(line4, g_wifi_read_line_buffer, strlen(line4)))
                {
                    rc = 4;  /* Se recibi� lo que se esperaba */
                }
            }
            if(rc == 0 && line5)
            {
#ifdef DEBUG_WIFI
                LogMessage("WifiWaitLine: esperando:%.80s / recibido: %.80s", line5, g_wifi_read_line_buffer);
#endif
                if( !memcmp(line5, g_wifi_read_line_buffer, strlen(line5)))
                {
                    rc = 5;  /* Se recibi� lo que se esperaba */
                }
            }
            
            if(rc == 0)
            {
                rc = WifiCheckUnsol(g_wifi_read_line_buffer, g_wifi_read_line_buffer_index);
            }
            
            g_wifi_read_line_buffer_index = 0;
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
            rc = WifiCheckUnsol(g_wifi_read_line_buffer, g_wifi_read_line_buffer_index);
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

int WifiCheckUnsol(const char* buffer, int buffer_len)
{
    char str[16];
    int conn_id;
    int data_len;
    char *p;

    if( !memcmp(buffer, "+IPD,", 5)) /* Datos socket */
    {
        /*
        +IPD,0,442:<datos>
        */
        conn_id = 0;
        data_len = 0;

        if(str_section((char*)buffer, ',', 1, str))
        {
            conn_id = atoi(str);
        }
        if(str_section((char*)buffer, ',', 2, str))
        {
            data_len = atoi(str);
        }
        /* dejo en el buffer solo el HTTP */
        p = strchr((char*)buffer, ':');
        if(p)
        {
            if( buffer_len == WIFI_READ_LINE_BUFFER_LEN)
            {
                HTTPInitTmpFile(buffer, WIFI_READ_LINE_BUFFER_LEN);
            }
            else
            {
                p++;
                *(p+data_len) = 0;
                HTTPAddINQueue(conn_id, p);
            }
        }
    }
    else if( !memcmp(buffer, "WIFI CONNECTED", 14))
    {
        LogMessage("[WIFI] Conectado al AP [%s].", 
                ((g_wifi_info.status.wifi_ap_select == 1)?g_wifi_info.f.config.wifi_ap1:g_wifi_info.f.config.wifi_ap2));
        g_wifi_info.status.wifi_conectado = g_wifi_info.status.wifi_ap_select;
    }
    else if( !memcmp(buffer, "WIFI DISCONNECTED", 17))
    {
        LogMessage("[WIFI] Desconectado del AP.");
        if(g_wifi_info.status.wifi_conectado)
        {
            LogMessage("[WIFI] Reconectando...");
            g_wifi_info.status.wifi_conectado = 0;
            g_wifi_info.status.wifi_ap_select = (g_wifi_info.status.wifi_ap_select == 1)?2:1;
            return STMODE;
        }
    }
    else if( !memcmp(buffer, "+STA_CONNECTED", 8))
    {
        LogMessage("[WIFI] Cliente conectado.");
        /* Cliente conectado al AP */
        g_wifi_timer_ap = 0; // para que no salga de modo AP
    }
    else if( !memcmp(buffer, "+STA_DISCONNECTED", 8))
    {
        LogMessage("[WIFI] Cliente desconectado.");
        /* Cliente desconectado al AP */
        g_wifi_timer_ap = 50; // para que salga en seguida del modo AP
    }
    else if( !memcmp(buffer, "+CIFSR:", 7))
    {
        /* AT+CIFSR
         * +CIFSR:STAIP,"0.0.0.0"
         * +CIFSR:STAMAC,"80:7d:3a:62:03:60"
         */
        if( !memcmp(&buffer[7], "STAMAC", 6))
        {
            LogMessage("[WIFI] MAC %.17s", &buffer[15]);
            g_wifi_info.status.hw_mac[0]  = buffer[15];
            g_wifi_info.status.hw_mac[1]  = buffer[16];
            g_wifi_info.status.hw_mac[2]  = buffer[18];
            g_wifi_info.status.hw_mac[3]  = buffer[19];
            g_wifi_info.status.hw_mac[4]  = buffer[21];
            g_wifi_info.status.hw_mac[5]  = buffer[22];
            g_wifi_info.status.hw_mac[6]  = buffer[24];
            g_wifi_info.status.hw_mac[7]  = buffer[25];
            g_wifi_info.status.hw_mac[8]  = buffer[27];
            g_wifi_info.status.hw_mac[9]  = buffer[28];
            g_wifi_info.status.hw_mac[10] = buffer[30];
            g_wifi_info.status.hw_mac[11] = buffer[31];
            g_wifi_info.status.hw_mac[12] = 0;
        }
    }
    else if( !memcmp(buffer, "AT version:", 11))
    {
        /*
         * AT version:1.6.2.0(Apr 13 2018 11:10:59)
         * AT version:1.7.4.0(May 11 2020 19:13:04)
         */
        LogMessage("[WIFI] %s.", buffer);
        strncpy((char*)g_wifi_info.status.wifi_at, (const char*)&buffer[11], sizeof(g_wifi_info.status.wifi_at)-1);
        //if(strstr(buffer, "1.6.2.0"))
        //{
        //    LogMessage("[WIFI] El firware necesita upgrade.");
        //    g_wifi_need_upgrade = true;
        //}
    }
    else if( !memcmp(buffer, "SDK version:", 12))
    {
        LogMessage("[WIFI] %s.", buffer);
        strncpy((char*)g_wifi_info.status.wifi_sdk, (const char*)&buffer[12], sizeof(g_wifi_info.status.wifi_sdk)-1);
    }
    else if( !memcmp(buffer, "compile time:", 13))
    {
        LogMessage("[WIFI] %s.", buffer);
    }
    return 0;
}

void WifiSwitchHost( void )
{
    if(g_wifi_info.status.wifi_host_select == &g_wifi_info.f.config.wifi_host1[0] && g_wifi_info.f.config.wifi_host2[0] != 0)
    {
        g_wifi_info.status.wifi_host_select = &g_wifi_info.f.config.wifi_host2[0];
        g_wifi_info.status.wifi_port_select = g_wifi_info.f.config.wifi_host2_port;
    }
    else
    {
        g_wifi_info.status.wifi_host_select = &g_wifi_info.f.config.wifi_host1[0];
        g_wifi_info.status.wifi_port_select = g_wifi_info.f.config.wifi_host1_port;
    }
}

int WifiTaskInit( void )
{
    int rc = 0;

    if(g_wifi_task_stm != g_wifi_task_stm_last)
    {
        LogMessage("[WifiTaskInit] STM %i -> %i",
                g_wifi_task_stm_step_last,
                g_wifi_task_stm_step);
        g_wifi_task_stm_step_last = g_wifi_task_stm_step;
    }

    switch(g_wifi_task_stm_step)
    {
        case 0:
            LogMessage("[WIFI] RESTORE factory defaults.");
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
                LogMessage("[WIFI] Con soporte SSL");
                // Puede faltar
                // AT+CIPSSLSIZE=4096
            }
            else if(g_wifi_task_stm_timeout == 0)
            {
                g_wifi_task_stm_step++;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
                g_wifi_info.status.wifi_ssl_capable = false;
                LogMessage("[WIFI] Sin soporte SSL");
                //g_wifi_need_upgrade = true;
            }
            break;
        case 8:
        default:
            rc = LOOP;
            break;
    }
    return rc;
}

int WifiTaskAPMode( void )
{
    int rc = 0;

    if(g_wifi_task_stm != g_wifi_task_stm_last)
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
            sprintf(g_wifi_cmd, "AT+CIPSERVERMAXCONN=%d", WIFI_CIP_MAX_CONS);
            WifiWriteLine(g_wifi_cmd);
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
                LogMessage("[WIFI] AT+CIPSERVERMAXCONN no soportado.");
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
                LogMessage("[WIFI] ********** Modo configuracion **********");
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

    if(g_wifi_task_stm != g_wifi_task_stm_last)
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
            g_wifi_task_stm_timeout = 100;
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
            g_wifi_task_stm_timeout = 100;
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
            sprintf(g_wifi_cmd, "AT+CIPSERVERMAXCONN=%d", WIFI_CIP_MAX_CONS);
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
                LogMessage("[WIFI] AT+CIPSERVERMAXCONN no soportado.");
                g_wifi_task_stm_step++;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
            }
            break;
        case 8:
            /* Listen */
            WifiWriteLine("AT+CIPSERVER=1,80");
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
                //LogMessage("TIMEOUT esperando OK\n");
                rc = (-1);
            }
            break;
        case 10:
            /* Reconnect */
            WifiWriteLine("AT+CWAUTOCONN=1");
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
                g_wifi_info.status.wifi_mode = 1;
            }
            else if(g_wifi_task_stm_timeout == 0)
            {
                //LogMessage("TIMEOUT esperando OK\n");
                rc = (-1);
            }
            break;
        case 12:
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
                LogMessage("[WIFI] Conectando con AP %s (%s).", ap, pass);
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
                LogMessage("[WIFI] No hay AP definido.");
                rc = LOOP;
            }
            break;
        case 13:
            if(WifiWaitLine("OK", 0, 0, 0, 0))
            {
                LogMessage("[WIFI] ********** Modo normal **********");
                g_wifi_task_stm_step++;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
            }
            else if(g_wifi_task_stm_timeout == 0)
            {
                LogMessage("[WIFI] Error de conexion con AP.");
                LogMessage("[WIFI] Reconectando...");
                g_wifi_info.status.wifi_ap_select = (g_wifi_info.status.wifi_ap_select == 1)?2:1;
                //rc = (-1);
                rc = STMODE;
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

    if(g_wifi_task_stm != g_wifi_task_stm_last)
    {
        LogMessage("[WifiTaskUpgrade] STM %i -> %i",
                g_wifi_task_stm_step_last,
                g_wifi_task_stm_step);
        g_wifi_task_stm_step_last = g_wifi_task_stm_step;
    }

    switch(g_wifi_task_stm_step)
    {
        case 0:
            LogMessage("[WIFI] Firmware Upgrade.\n");
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
                LogMessage("[WIFI] Upgrade TIMEOUT.");
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

int WifiTaskHttp( void )
{
    int rc = 0;

    if(g_wifi_task_stm != g_wifi_task_stm_last)
    {
        LogMessage("[WifiTaskHttp] STM %i -> %i",
                g_wifi_task_stm_step_last,
                g_wifi_task_stm_step);
        g_wifi_task_stm_step_last = g_wifi_task_stm_step;
    }

    switch(g_wifi_task_stm_step)
    {
        case 0:
            g_wifi_task_retry = 0;
            if(HTTPOutPending()) g_wifi_task_stm_step++;
            else rc = LOOP;
            break;
        case 1:
            g_wifi_http_id = HTTPOutConnId();
            /* Si es una respuesta salteo la conexion */
            if(HTTPOutType() == 'R')
            {
                LogMessage("WifiTaskHttp -> Respondiendo");
                g_wifi_task_stm_step += 2;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
                break;
            }
            if(g_wifi_info.status.wifi_port_select == 0)
            {
                if(g_wifi_info.f.config.enable_https)
                {
                    g_wifi_info.status.wifi_port_select = 443;
                }
                else
                {
                    g_wifi_info.status.wifi_port_select = 80;
                }
            }
            LogMessage("WifiTaskHttp -> Conectando %s:%i ...",  g_wifi_info.status.wifi_host_select, g_wifi_info.status.wifi_port_select);
            sprintf(g_wifi_cmd, "AT+CIPSTART=%d,\"%s\",\"%s\",%d",
                    g_wifi_http_id,
                    (g_wifi_info.f.config.enable_https)?"SSL":"TCP",
                    g_wifi_info.status.wifi_host_select,
                    g_wifi_info.status.wifi_port_select);
            LogMessage("[WiFi CMD] %s", g_wifi_cmd);
            WifiWriteLine(g_wifi_cmd);
            g_wifi_task_stm_step++;
            g_wifi_task_stm_delay = 0;
            g_wifi_task_stm_timeout = 30;
            break;
        case 2:
            if(WifiWaitLine("OK", 0, 0, 0, 0))
            {
                g_wifi_task_stm_step++;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
            }
            else if(g_wifi_task_stm_timeout == 0)
            {
                LogMessage("WifiTaskHttp -> Open Error");
                g_wifi_task_stm_step = 11; /* -> Open ERROR */
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
            }
            break;
        case 3:
            LogMessage("WifiTaskHttp -> Enviando");
            sprintf(g_wifi_cmd, "AT+CIPSEND=%d,%d", g_wifi_http_id, 
                    strlen(HTTPOutData()) );
            LogMessage("[WiFi CMD] %s", g_wifi_cmd);
            WifiWriteLine(g_wifi_cmd);
            g_wifi_task_stm_step++;
            g_wifi_task_stm_delay = 0;
            g_wifi_task_stm_timeout = 15;
            break;
        case 4:
            if(WifiWaitPrompt('>'))
            {
                g_wifi_task_stm_step++;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
            }
            else if(g_wifi_task_stm_timeout == 0)
            {
                LogMessage("WifiTaskHttp -> prompt time-out");
                g_wifi_task_stm_step = 10; /* -> Send ERROR */
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
            }
            break;
        case 5:
            LogMessage("[WiFi CMD] %s", HTTPOutData());
            WifiWrite(HTTPOutData());
            g_wifi_task_stm_step++;
            g_wifi_task_stm_delay = 0;
            g_wifi_task_stm_timeout = 25;
            break;
        case 6:
            if(WifiWaitLine("SEND OK", 0, 0, 0, 0) || g_wifi_task_stm_timeout == 0)
            {
                LogMessage("WifiTaskHttp -> Envio Ok");
                if(HTTPOutType() == 'S')
                {
                    /* Si es un requerimiento saliente espero respuesta */
                    g_wifi_info.status.wifi_http_response = 0;
                    g_wifi_send_error_count = 0;
                    g_wifi_fast_start = true;
                    g_wifi_task_stm_step++;
                    g_wifi_task_stm_delay = 0;
                    strncpy(g_wifi_http_filename, HTTPOutFilename(), FILE_NAME_SIZE);
                    g_wifi_task_stm_timeout = HTTPOutTimeOut();
                }
                else
                {
                    /* Si es una respuesta saliente sigue un close */
                    g_wifi_recv_error_count = 0;
                    g_wifi_task_stm_step += 2;
                    g_wifi_task_stm_delay = 5;
                    g_wifi_task_stm_timeout = 0;
                }
                /* Borro el mensaje de la cola */
                HTTPOutNext();
            }
            break;
        case 7:
            sprintf(g_wifi_cmd, "%d,CLOSED", g_wifi_http_id);
            if(WifiWaitLine(g_wifi_cmd, "UNLINK", "OK", 0, 0))
            {
                LogMessage("WifiTaskHttp -> Cerrado");
                rc = LOOP;
            }
            else if(g_wifi_task_stm_timeout == 0)
            {
                LogMessage("WifiTaskHttp -> Terminado");
                g_wifi_task_stm_step++;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
            }
            else if(g_wifi_info.status.wifi_http_response)
            {
                LogMessage("WifiTaskHttp -> Respuesta recibida");
                if(g_wifi_info.status.wifi_http_response > 0)
                {
                    g_wifi_info.status.host_conectado = 1;
                }
                else
                {
                    g_wifi_info.status.host_conectado = 0;
                    CTRLNotificarStatus(100);
                    WifiSwitchHost();
                }
                g_wifi_task_stm_step++;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
            }
            break;
        case 8:
            LogMessage("WifiTaskHttp -> Cerrando");
            sprintf(g_wifi_cmd, "AT+CIPCLOSE=%d", g_wifi_http_id);
            LogMessage("[WiFi CMD] %s", g_wifi_cmd);
            WifiWriteLine(g_wifi_cmd);
            g_wifi_task_stm_step++;
            g_wifi_task_stm_delay = 0;
            g_wifi_task_stm_timeout = 25;
            break;
        case 9:
            sprintf(g_wifi_cmd, "%d,CLOSED", g_wifi_http_id);
            if(WifiWaitLine(g_wifi_cmd, "UNLINK", "OK", 0, 0))
            {
                LogMessage("WifiTaskHttp -> Cerrado");
                rc = LOOP;
            }
            else if(g_wifi_task_stm_timeout == 0)
            {
                LogMessage("WifiTaskHttp -> time-out");
                rc = LOOP;
            }
            break;
        case 10:
            /* Send Error */
            sprintf(g_wifi_cmd, "AT+CIPCLOSE=%d", g_wifi_http_id);
            LogMessage("[WiFi CMD] %s", g_wifi_cmd);
            WifiWriteLine(g_wifi_cmd);
            g_wifi_task_stm_step++;
            g_wifi_task_stm_delay = 15;
            g_wifi_task_stm_timeout = 0;
            break;
        case 11:
            /* Open Error */
            if(HTTPOutType() == 'S')
            {
                /* Si es un  requerimiento saliente */
                g_wifi_info.status.host_conectado = 0;
                g_wifi_send_error_count++;
                /**/
                g_wifi_task_retry++;
                if(g_wifi_task_retry < 2)
                {
                    WifiSwitchHost();
                    g_wifi_task_stm_step = 1; /* -> Retry */
                    g_wifi_task_stm_delay = 15;
                    g_wifi_task_stm_timeout = 0;
                    break;
                }
            }
            else
            {
                /* Si es una respuesta saliente */
                g_wifi_recv_error_count++;
            }
            /* Borro el mensaje de la cola */
            //HTTPGet(g_wifi_http_id, NULL, true);
            rc = LOOP;
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
        if(g_wifi_fast_start)
        {
            LogMessage("[WIFI] Ir a modo ST.");
            return STMODE;
        }
        else
        {
            LogMessage("[WIFI] Ir a modo AP.");
            return APMODE;
        }
    }
    else if(HTTPOutPending())
    {
        //LogMessage("HTTP Pendiente");
        return HTTP;
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

    if(g_wifi_task_stm != g_wifi_task_stm_last)
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
    g_wifi_fast_start = false;
    
    g_wifi_info.status.wifi_host_select = &g_wifi_info.f.config.wifi_host1[0];
    g_wifi_info.status.wifi_port_select = g_wifi_info.f.config.wifi_host1_port;
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
            g_wifi_task_stm_timeout = 0;
            g_wifi_task_stm_step = 0;
            g_wifi_read_line_buffer_index = 0;
            g_wifi_read_line_buffer_timer = 0;
            g_wifi_read_line_wait_crlf = true;
            g_wifi_wait_more_data = false;
            g_wifi_task_stm_delay = 30;

            g_wifi_send_error_count = 0;
            g_wifi_recv_error_count = 0;

            g_wifi_info.status.wifi_conectado = 0;
            g_wifi_info.status.wifi_ap_select = 1;
            g_wifi_info.status.wifi_start_ap = true;
            g_wifi_timer_ap = 0;
            g_wifi_info.status.wifi_mode = 0;
            g_wifi_info.status.host_conectado = 0;
            
            WifiTaskSTMSet(START, 50, 0);
            break;
        case START:
            WIFI_RESET_SetHigh();
            g_wifi_task_stm_delay = 10;
            WifiTaskSTMSet(CLEAN, 5, 50);
            break;
        case CLEAN:
            /* Me quedo recibiendo un rato la basura de la POST del ESP-02 */
            /* No es basura, pero se ve como basura porque esta a otra velocidad */
            g_wifi_task_stm_delay = 0;
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
        case HTTP:
            rc = WifiTaskHttp();
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
            g_wifi_task_stm = STMODE;
        }
    }
    
    if(g_wifi_read_line_buffer_timer) g_wifi_read_line_buffer_timer--;
    
}

/* ************************************************************************* */
