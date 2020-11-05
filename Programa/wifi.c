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

#include "main.h"

#define WIFI_READ_LINE_BUFFER_LEN 512
#define TIMER_AP_VALUE 600

const char http_200[] = "HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Type: text/html\r\nCache-Control: no-cache\r\n\r\n";
const char http_404[] = "HTTP/1.1 404 Not found\r\nConnection: close\r\n\r\n404: File not found\r\n";

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
    LISTAP,
    LOOP,
    ERROR
} task_stm;

task_stm g_wifi_task_stm;
unsigned int g_wifi_timer_ap;
unsigned int g_wifi_task_stm_step;
unsigned int g_wifi_task_stm_delay;
unsigned int g_wifi_task_stm_timeout;

unsigned char g_wifi_read_line_buffer[WIFI_READ_LINE_BUFFER_LEN+1];
unsigned int g_wifi_read_line_buffer_index;

int WifiReadLine( char *line );
void WifiWrite( const char *buffer );
void WifiWriteLine( const char *line );
int WifiWaitLine(const char *line);

int WifiTaskInit( void );
int WifiTaskAPMode( void );
int WifiTaskSTMode( void );
int WifiTaskUpgrade( void );
int WifiTaskLoop( void );
int WifiTaskListAP( void );

unsigned char g_loop_count;

void WifiTaskSTMSet(task_stm st, unsigned int delay, unsigned int to)
{
    g_wifi_task_stm = st;
    g_wifi_task_stm_delay = delay;
    g_wifi_task_stm_timeout = to;
    g_wifi_task_stm_step = 0;
}

/* ****************************************************************************

 **************************************************************************** */
void WiFiInit( void )
{
    g_wifi_task_stm = RESET;
    g_wifi_task_stm_delay = 0;
    g_wifi_task_stm_timeout = 0;
    g_wifi_task_stm_step = 0;
    g_wifi_read_line_buffer_index = 0;
    g_wifi_info.status.wifi_need_firmware_update = false;
    g_loop_count = 0;
    g_wifi_info.status.wifi_conectado = 0;
    g_wifi_info.status.wifi_ap_select = 1;
    g_wifi_info.status.wifi_start_ap = true;
    g_wifi_timer_ap = 0;
}

/* ****************************************************************************

 **************************************************************************** */
void WiFiTask( void )
{
    int rc;
    
    if(g_wifi_task_stm_delay) return;
    
    switch(g_wifi_task_stm)
    {
        case INVALID:
        case RESET:
			//printf("RESET\n");
            WIFI_RESET_SetLow();
            g_wifi_info.status.wifi_mode = 0;
            WifiTaskSTMSet(START, 50, 0);
            break;
        case START:
			//printf("START\n");
            WIFI_RESET_SetHigh();
            WifiTaskSTMSet(CLEAN, 5, 50);
            break;
        case CLEAN:
            /* Me quedo recibiendo un rato la basura de la POST del ESP-02 */
            /* No es basura, pero se ve como basura porque está a otra velocidad */
            WifiWaitLine(NULL);
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
        case LOOP:
            rc = WifiTaskLoop();
            if(rc > 0)      WifiTaskSTMSet(rc, 0, 0);
            else if(rc < 0) WifiTaskSTMSet(RESET, 0, 0);
            break;
        default:    /* ERROR */
			//printf("***** SOFTWARE ERROR ***** RESET *****\n");
            WifiTaskSTMSet(RESET, 0, 0);
            break;
    }
}

/* ****************************************************************************
   Se llama 10 veces por egundo

 **************************************************************************** */
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
}

/* ************************************************************************* */
int WifiCheckUnsol( void )
{
    char str[16];
    int id;
    int len;
    char *p, *q;
    char cmd[512];
    
    //printf("WifiCheckUnsol: %s\n", g_wifi_read_line_buffer);
    if( !memcmp(g_wifi_read_line_buffer, "WIFI CONNECTED", 14))
    {
        g_wifi_info.status.wifi_conectado = g_wifi_info.status.wifi_ap_select;
    }
    else if( !memcmp(g_wifi_read_line_buffer, "WIFI DISCONNECTED", 11))
    {
        g_wifi_info.status.wifi_conectado = 0;
        g_wifi_info.status.wifi_ap_select = (g_wifi_info.status.wifi_ap_select == 1)?2:1;
    }
    else if( !memcmp(g_wifi_read_line_buffer, "STA_CONNECTED", 14))
    {
        
    }
    else if( !memcmp(g_wifi_read_line_buffer, "STA_DISCONNECTED", 11))
    {
        
    }
    else if( !memcmp(g_wifi_read_line_buffer, "+CWJAP:", 7)) /* AT+CWJAP? */
    {
        
    }
    else if( !memcmp(g_wifi_read_line_buffer, "+IPD,", 5)) /* Datos socket */
    {
        /*
        +IPD,0,442:GET / HTTP/1.1
        Host: 192.168.4.1
        Connection: keep-alive
        Upgrade-Insecure-Requests: 1
        User-Agent: Mozilla/5.0 (Linux; Android 10; Redmi Note 8 Pro) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/86.0.4240.99 Mobile Safari/537.36
        Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,+/+;q=0.8,application/signed-exchange;v=b3;q=0.9
        Accept-Encoding: gzip, deflate
        Accept-Language: es-ES,es;q=0.9,en;q=0.8
        */
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
        p = &g_wifi_read_line_buffer[0];
        q = strchr(g_wifi_read_line_buffer, ':');
        if(q)
        {
            q++;
            while(*q)
            {
                *p = *q;
                p++;
                q++;
            }
            *p = 0;
            /*
            GET / HTTP/1.1
            Host: 192.168.4.1
            Connection: keep-alive
            Upgrade-Insecure-Requests: 1
            User-Agent: Mozilla/5.0 (Linux; Android 10; Redmi Note 8 Pro) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/86.0.4240.99 Mobile Safari/537.36
            Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,+/+;q=0.8,application/signed-exchange;v=b3;q=0.9
            Accept-Encoding: gzip, deflate
            Accept-Language: es-ES,es;q=0.9,en;q=0.8
            */
            sprintf(cmd, "AT+CIPSEND=%d,$d", id, strlen(http_200));
            WifiWriteLine(cmd);
            WifiWrite(http_200);
            sprintf(cmd, "AT+CIPCLOSE=%d", id);
            WifiWriteLine(cmd);
        }
    }
    return 0;
}

int WifiTaskInit( void )
{
    int rc = 0;

    switch(g_wifi_task_stm_step)
    {
        case 0:
            //printf("--> Init.\n");
            /* Restore a fábrica y espero a que el modulo se reinicie */
            WifiWriteLine("AT+RESTORE");
            g_wifi_task_stm_step++;
            g_wifi_task_stm_delay = 0;
            g_wifi_task_stm_timeout = 100;
            break;
        case 1:
            if(WifiWaitLine("ready"))
            {
                g_wifi_task_stm_step++;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
            }
            else if(g_wifi_task_stm_timeout == 0)
            {
                //printf("TIMEOUT esperando ready\n");
                rc = (-1);
            }
            break;
        case 2:
            /* ECHO OFF */
            WifiWriteLine("AT");
            //WifiWriteLine("ATE0");
            g_wifi_task_stm_step++;
            g_wifi_task_stm_delay = 0;
            g_wifi_task_stm_timeout = 25;
            break;
        case 3:
            if(WifiWaitLine("OK"))
            {
                g_wifi_task_stm_step++;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
            }
            else if(g_wifi_task_stm_timeout == 0)
            {
                //printf("TIMEOUT esperando OK\n");
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
            if(WifiWaitLine("OK"))
            {
                g_wifi_task_stm_step++;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
            }
            else if(g_wifi_task_stm_timeout == 0)
            {
                //printf("TIMEOUT esperando OK\n");
                rc = (-1);
            }
            break;
        default:
            rc = LOOP;
            break;
    }
    return rc;
}

int WifiTaskAPMode( void )
{
    int rc = 0;

    switch(g_wifi_task_stm_step)
    {
        case 0:
            //printf("--> Modo AP.\n");
            /* Bajo los servers */
            WifiWriteLine("AT+CIPSERVER=0");
            g_wifi_task_stm_step++;
            g_wifi_task_stm_delay = 0;
            g_wifi_task_stm_timeout = 10;
            break;
        case 1:
            /* No controlo error */
            if(WifiWaitLine("OK") || g_wifi_task_stm_timeout == 0)
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
            g_wifi_info.status.wifi_mode = 3;
            break;
        case 3:
            if(WifiWaitLine("OK"))
            {
                g_wifi_task_stm_step++;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
            }
            else if(g_wifi_task_stm_timeout == 0)
            {
                //printf("TIMEOUT esperando OK\n");
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
            if(WifiWaitLine("OK"))
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
            /* Listen */
            WifiWriteLine("AT+CIPSERVER=1,80");
            g_wifi_task_stm_step++;
            g_wifi_task_stm_delay = 0;
            g_wifi_task_stm_timeout = 25;
            break;
        case 7:
            if(WifiWaitLine("OK"))
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
    char cmd[64];
    char *ap, *pass;

    switch(g_wifi_task_stm_step)
    {
        case 0:
            //printf("--> Modo STA.\n");
            /* Bajo los servers */
            WifiWriteLine("AT+CIPSERVER=0");
            g_wifi_task_stm_step++;
            g_wifi_task_stm_delay = 0;
            g_wifi_task_stm_timeout = 100;
            break;
        case 1:
            if(WifiWaitLine("OK") || g_wifi_task_stm_timeout == 0)
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
            if(WifiWaitLine("OK"))
            {
                g_wifi_task_stm_step++;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
            }
            else if(g_wifi_task_stm_timeout == 0)
            {
                //printf("TIMEOUT esperando OK\n");
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
            if(WifiWaitLine("OK"))
            {
                g_wifi_task_stm_step++;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
            }
            else if(g_wifi_task_stm_timeout == 0)
            {
                //printf("TIMEOUT esperando OK\n");
                rc = (-1);
            }
            break;
        case 6:
            /* Listen */
            WifiWriteLine("AT+CIPSERVER=1,3100");
            g_wifi_task_stm_step++;
            g_wifi_task_stm_delay = 0;
            g_wifi_task_stm_timeout = 25;
            break;
        case 7:
            if(WifiWaitLine("OK"))
            {
                g_wifi_task_stm_step++;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
            }
            else if(g_wifi_task_stm_timeout == 0)
            {
                //printf("TIMEOUT esperando OK\n");
                rc = (-1);
            }
            break;
        case 8:
            /* Reconnect */
            WifiWriteLine("AT+CWAUTOCONN=1");
            g_wifi_task_stm_step++;
            g_wifi_task_stm_delay = 0;
            g_wifi_task_stm_timeout = 25;
            g_wifi_info.status.wifi_mode = 1;
            break;
        case 9:
            if(WifiWaitLine("OK"))
            {
                g_wifi_task_stm_step++;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
            }
            else if(g_wifi_task_stm_timeout == 0)
            {
                //printf("TIMEOUT esperando OK\n");
                rc = (-1);
            }
            break;
        case 10:
            ap = ((g_wifi_info.status.wifi_ap_select == 1)?g_wifi_info.config.wifi_ap1:g_wifi_info.config.wifi_ap2);
            pass = ((g_wifi_info.status.wifi_ap_select == 1)?g_wifi_info.config.wifi_ap1_pass:g_wifi_info.config.wifi_ap2_pass);
            if(*ap)
            {
                /* Connect to AP */
                if(*pass)
                {
                    /* Con clave */
                    sprintf(cmd, "AT+CWJAP=\"%s\",\"%s\"", ap, pass);
                }
                else
                {
                    /* Sin clave */
                    sprintf(cmd, "AT+CWJAP=\"%s\"", ap);
                }
                WifiWriteLine(cmd);
                g_wifi_task_stm_step++;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 300;
            }
            else
            {
                /* No hay AP definido */
                rc = LOOP;
            }
            break;
        case 11:
            if(WifiWaitLine("OK"))
            {
                g_wifi_task_stm_step++;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
            }
            else if(g_wifi_task_stm_timeout == 0)
            {
                //printf("TIMEOUT esperando OK\n");
                g_wifi_info.status.wifi_ap_select = (g_wifi_info.status.wifi_ap_select == 1)?2:1;
                //rc = (-1);
                rc = LOOP;
            }
            break;
        default:
            rc = LOOP;
            break;
    }
    return rc;
}

int WifiTaskUpgrade( void )
{
    int rc = 0;

    switch(g_wifi_task_stm_step)
    {
        case 0:
            //printf("--> Upgrade.\n");
            /* Bajo los servers */
            WifiWriteLine("AT+CIPUPDATE");
            g_wifi_task_stm_step++;
            g_wifi_task_stm_delay = 0;
            g_wifi_task_stm_timeout = 900;
            break;
        case 1:
            if(WifiWaitLine("OK"))
            {
                g_wifi_task_stm_step++;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
            }
            else if(g_wifi_task_stm_timeout == 0)
            {
                //printf("TIMEOUT esperando OK\n");
                rc = (-1);
            }
            break;
        default:
            rc = RESET;
            break;
    }
    return rc;
}

int WifiTaskLoop( void )
{
    int rc;

    /* Espero no solicitados */
    rc = WifiWaitLine(NULL);
    if(rc > 0)
    {
        return rc;
    }
    else
    {
        if(g_wifi_info.status.wifi_start_ap)
        {
            g_wifi_info.status.wifi_start_ap = false;
            return APMODE;
        }
        
        
        
        
        
        
        return 0;
    }
}

int WifiTaskListAP( void )
{
    int rc = 0;

    switch(g_wifi_task_stm_step)
    {
        case 0:
            //printf("--> List AP.\n");
            WifiWriteLine("AT+CWLAP");
            g_wifi_task_stm_step++;
            g_wifi_task_stm_delay = 0;
            g_wifi_task_stm_timeout = 50;
            break;
        case 1:
            if(WifiWaitLine("OK"))
            {
                g_wifi_task_stm_step++;
                g_wifi_task_stm_delay = 0;
                g_wifi_task_stm_timeout = 0;
            }
            else if(g_wifi_task_stm_timeout == 0)
            {
                //printf("TIMEOUT esperando Ok\n");
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

void WifiWrite( const char *buffer )
{
	//printf("WifiWrite: %s\n", buffer);
    /* Reseteo el buffer de lectura */
    g_wifi_read_line_buffer_index = 0;
    if(buffer)
    {
        while(*buffer)
        {
            UART1_Write((uint8_t)*buffer);
            buffer++;
        }
    }
}

void WifiWriteLine( const char *line )
{
	//printf("WifiWriteLine: %s\n", line);
    /* Reseteo el buffer de lectura */
    g_wifi_read_line_buffer_index = 0;
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
}

/*
    Return: = 0 - No se recibió lo que se esperaba y ninguna acción no solicitada
            > 0 - Se recibió algo no solicitado rc = nuevo estado
            < 0 - Se recibio lo que se esperaba
*/
int WifiWaitLine(const char *line)
{
    int rc = 0;
    unsigned char c;

    while(UART1_IsRxReady())
    {
        c = UART1_Read();
        if(c == 0x0D)
        {
            /* Se completa una línea */
            if(g_wifi_read_line_buffer_index)
            {
                if(line)
                {
                    //printf("WifiWaitLine: esperando:%s\n", line);
                    if( !strcmp(line, g_wifi_read_line_buffer))
                    {
                        //printf("               recibido:%s\n", line);
                        rc = (-1);  /* Se recibió lo que se esperaba */
                    }
                    else
                    {
                        //printf("               recibido:%s\n", g_wifi_read_line_buffer);
                        rc = WifiCheckUnsol();
                    }
                }
                else
                {
                    //printf("               recibido:%s\n", g_wifi_read_line_buffer);
                    rc = WifiCheckUnsol();
                }
                g_wifi_read_line_buffer_index = 0;
            }
            break;
        }
        else if( (g_wifi_read_line_buffer_index < WIFI_READ_LINE_BUFFER_LEN) && c != 0x0A )
        {
            /* Se agrega el caracter a la línea */
            g_wifi_read_line_buffer[g_wifi_read_line_buffer_index] = c;
            g_wifi_read_line_buffer_index++;
            g_wifi_read_line_buffer[g_wifi_read_line_buffer_index] = 0;
        }
    }
    return rc;
}
