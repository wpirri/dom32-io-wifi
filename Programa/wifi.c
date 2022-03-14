/***************************************************************************
    Copyright (C) 2022   Walter Pirri

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
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>

#include "mcc_generated_files/uart1.h"
#include "mcc_generated_files/pin_manager.h"

#include "config.h"
#include "led.h"
#include "queue.h"
#include "http.h"
#include "strfunc.h"

typedef enum
{
    RESET = 0,
    START,
    WAIT_READY,
    INIT,
    INIT_STEP,
    INIT_OK,

    AP_MODE,
    AP_MODE_STEP,
    AP_MODE_OK,
            
    ST_MODE,
    ST_MODE_STEP,
    ST_MODE_OK,

    DATA_SEND,
    DATA_SEND_STEP,
    DATA_SEND_OK,
            
    MAIN,
    MAIN_LOOP,
            
    FATALERROR,
    ERROR
} wifi_task_stm;

#define WIFI_RX_BUFFER_LEN 4096
#define WIFI_TX_BUFFER_LEN 4096

#define WIFI_MAX_CONN 4

#define KEEP_ALIVE_TIMER 300

unsigned int wifi_queue_rx;
char wifi_buffer_rx[WIFI_RX_BUFFER_LEN];
unsigned int wifi_queue_tx;
char wifi_buffer_tx[WIFI_TX_BUFFER_LEN];

wifi_task_stm wifi_stm;
int wifi_count_stm_no_process;
volatile int wifi_count_rx_timer;
volatile bool wifi_flag_rx_parse_on_lf;
int wifi_count_command_seq;
char wifi_hw_mac[13];
int wifi_count_ap_mode_timer;
char wifi_buffer_msg_head[8];
int wifi_count_msg_head;

int wifi_count_keep_alive;
bool wifi_flag_keep_alive;

typedef struct _wifi_wait_for_st
{
    char wait[16];
    wifi_task_stm on_ok;
    wifi_task_stm on_error;
    int time_out;
} wifi_wait_for_st;

wifi_wait_for_st wwfst;

char wifi_data_send_ip_addr[16];
int wifi_data_send_ip_port;
int wifi_data_send_conn_id;
char wifi_data_send_buffer[WIFI_TX_BUFFER_LEN];


void WifiSendString(const char* msg, bool crlf);
void WiFiCheckUnsol(const char* msg);
void WiFiWaitFor(const char* wait, wifi_task_stm on_ok, wifi_task_stm on_error, int time_out);
void ProcessReceiveQueue( void );
void WiFiInitSeq( wifi_task_stm stm );
void WiFiAPModeSeq( wifi_task_stm stm );
void WiFiSTModeSeq( wifi_task_stm stm );
void WiFiDataSendSeq( wifi_task_stm stm );

/* *****************************************************************************
 * 
 * ************************************************************************** */
void WiFiInit( void )
{
    wifi_stm = RESET;
    wifi_count_stm_no_process = 0;
    wifi_count_rx_timer = 0;
    wifi_count_command_seq = 0;
    wifi_flag_rx_parse_on_lf = true;
    wifi_count_ap_mode_timer = 0;
    wifi_count_msg_head = 0;
    wifi_count_keep_alive = 0;
    wifi_flag_keep_alive = false;
    
    wifi_queue_rx = QueueOpen(wifi_buffer_rx, WIFI_RX_BUFFER_LEN);
    if(wifi_queue_rx == INVALID_QUEUE) printf("ERROR: Al crear cola de recepcion\r\n");

    wifi_queue_tx = QueueOpen(wifi_buffer_tx, WIFI_TX_BUFFER_LEN);
    if(wifi_queue_tx == INVALID_QUEUE) printf("ERROR: Al crear cola de transmision\r\n");

    WiFiWaitFor(0, 0, 0, 0);
}

/* *****************************************************************************
 * 
 * ************************************************************************** */
void WiFiTask( void )
{
    ProcessReceiveQueue();
    
    if(wifi_count_stm_no_process) return;
    switch(wifi_stm)
    {
        case RESET:
            WIFI_RESET_SetLow();
            printf("INFO: WiFi RESET\r\n");
            wifi_count_stm_no_process = 30;
            
            wifi_count_rx_timer = 0;
            wifi_count_command_seq = 0;
            wifi_flag_rx_parse_on_lf = true;
            wifi_count_ap_mode_timer = 0;
            wifi_count_msg_head = 0;
            wifi_count_keep_alive = 0;
            wifi_flag_keep_alive = false;
            
            QueueClean(wifi_queue_rx);
            QueueClean(wifi_queue_tx);
            wifi_stm++;
            break;
        case START:
            WIFI_RESET_SetHigh();
            printf("INFO: WiFi Release\r\n");
            WiFiWaitFor("ready", INIT, FATALERROR, 10);
            wifi_stm++;
            break;
        case WAIT_READY:
            wifi_count_command_seq = 0;
            break;
        case INIT:
            WiFiInitSeq(INIT_STEP);
            break;
        case  INIT_STEP:
            wifi_count_command_seq++;
            wifi_stm--;
            break;
        case  INIT_OK:
            printf("INFO: Init OK\r\n");
            wifi_count_command_seq = 0;
            wifi_stm++;
            break;
        case AP_MODE:
            WiFiAPModeSeq(AP_MODE_STEP);
            break;
        case AP_MODE_STEP:
            wifi_count_command_seq++;
            wifi_stm--;
            break;
        case AP_MODE_OK:
            printf("INFO: AP Mode\r\n");
            Led1(5);
            Led2(0);
            wifi_count_command_seq = 0;
            wifi_count_ap_mode_timer = 600;
            wifi_stm = MAIN;
            break;
        case ST_MODE:
            WiFiSTModeSeq(ST_MODE_STEP);
            break;
        case ST_MODE_STEP:
            wifi_count_command_seq++;
            wifi_stm--;
            break;
        case ST_MODE_OK:
            printf("INFO: ST Mode\r\n");
            wifi_stm = MAIN;
            break;
        case DATA_SEND:
            WiFiDataSendSeq(DATA_SEND_STEP);
            break;            
        case DATA_SEND_STEP:
            wifi_count_command_seq++;
            wifi_stm--;
            break;            
        case DATA_SEND_OK:
            printf("INFO: Datos Emitidos\r\n");
            wifi_stm = MAIN;
            break;            
        case MAIN:
            wifi_count_command_seq = 0;
            wifi_count_keep_alive = KEEP_ALIVE_TIMER;
            wifi_stm++;
            break;
        case MAIN_LOOP:
            if(QueueElements(wifi_queue_tx))
            {
                wifi_count_keep_alive = KEEP_ALIVE_TIMER;
                wifi_count_command_seq = 0;
                printf("INFO: Modo Data Send\r\n");
                wifi_stm = DATA_SEND;
            }
            else if(wifi_flag_keep_alive)
            {
                wifi_flag_keep_alive = false;
                wifi_count_keep_alive = KEEP_ALIVE_TIMER;
                WifiSendString("AT", true);
            }
            break;
        case FATALERROR:
            //WifiSendString("AT+RESTORE", true);
        case ERROR:
        default:
            printf("ERROR: Se resetea el modem WiFi\r\n");
            wifi_stm = RESET;
            break;
    }
}

/* *****************************************************************************
 *    Se llama 10 veces por egundo
 * ************************************************************************** */
void WiFiTimer( void )
{
    if(wifi_count_rx_timer)
    {
        wifi_count_rx_timer--;
        if(wifi_count_rx_timer==0)
        {
            QueueAddByteEnqueue(wifi_queue_rx);
            wifi_flag_rx_parse_on_lf = true;
        }
    }

    if(wifi_count_stm_no_process) wifi_count_stm_no_process--;

    /* Control de time-out en WaitFor */
    if(wwfst.time_out)
    {
        wwfst.time_out--;
        if(wwfst.time_out == 0)
        {
            /* time-out de wait-for */
            wifi_stm = wwfst.on_error;
            WiFiWaitFor(0, 0, 0, 0);
        }
    }
    
    if(wifi_count_ap_mode_timer)
    {
        wifi_count_ap_mode_timer--;
        if(wifi_count_ap_mode_timer == 0)
        {
            /* Salgo del modo AP */
            wifi_stm = ST_MODE;
        }
    }
    
    if(wifi_count_keep_alive)
    {
        wifi_count_keep_alive--;
        if(wifi_count_keep_alive == 0)
        {
            wifi_flag_keep_alive = true;
        }
    }
    
}

/* *****************************************************************************
 *    Se llama con cada char que entra por el serial
 * ************************************************************************** */
void WiFiNewChar(char ch)
{
    wifi_count_rx_timer = 5; /* 500 ms */

    if( wifi_flag_rx_parse_on_lf && (ch == 0x0a || ch == 0x0d) )
    {
        wifi_count_rx_timer = 0;
        if(wifi_count_msg_head)
        {
            wifi_count_msg_head = 0;
            QueueAddByteEnqueue(wifi_queue_rx);
        }
    }
    else
    {
        QueueAddByte(wifi_queue_rx, ch);
        
        /* Detecto mensajes que no terminan en LF */
        if(wifi_count_msg_head < 4)
        {
            wifi_buffer_msg_head[wifi_count_msg_head] = ch;
            wifi_count_msg_head++;
            if(wifi_count_msg_head == 4)
            {
                if(wifi_buffer_msg_head[0] == '+' &&
                   wifi_buffer_msg_head[1] == 'I' &&
                   wifi_buffer_msg_head[2] == 'P' &&
                   wifi_buffer_msg_head[3] == 'D'  )
                {
                    wifi_flag_rx_parse_on_lf = false;
                }
            }
        }
    }
}

/* ************************************************************************* */

void WifiSendString(const char* msg, bool crlf)
{
    printf("%.80s --> WiFi\r\n", msg);
    while(*msg)
    {
        UART1_Write(*msg);
        msg++;
    }
    if(crlf)
    {
        UART1_Write('\r');
        UART1_Write('\n');
    }
}

void WiFiCheckUnsol(const char* msg)
{
    char socket[8];
    char msglen[8];
    char method;
    char *http_head;
    char url[256];
    char *p, *q;
    int body_len;

    /* Se reciben datos por un socket */
    if( !memcmp(msg, "+IPD,", 5))
    {
        /*
        +IPD,0,442:<datos>
         * 1.- Id del socket
         * 2.- message len
        */
        str_section(msg, 1, socket);
        str_section(msg, 2, msglen);
        p = strchr(msg, ':');
        if(p)
        {
            p++;
            /* GET / HTTP/1.1 */
            http_head = p;
            /* Metodo P o G */
            method = *http_head;
            /* /sarasa/sasa/sarasasasa */
            while(*p && *p != ' ') p++;
            p++;
            q = &url[0];
            while(*p && *p != ' ')
            {
                *q = *p;
                p++;
                q++;
            }
            *q = 0;
            p++;
            q = strstr(p, "\r\n\r\n");
            if(q)
            {
                q += 4;
                body_len = atoi(msglen)-(q-http_head);
                HTTPMessage(atoi(socket), method, url, q, body_len);
            }
        }
    }
    else if( !memcmp(msg, "WIFI CONNECTED", 14))
    {

    }
    else if( !memcmp(msg, "WIFI DISCONNECTED", 17))
    {

    }
    else if( !memcmp(msg, "+STA_CONNECTED", 8))
    {
        /* Cliente conectado al AP propio */
        /* cancelo el timer de configuracion */
        wifi_count_ap_mode_timer = 0;   
        Led2(2);

    }
    else if( !memcmp(msg, "+STA_DISCONNECTED", 8))
    {
        /* Cliente desconectado al AP propio */
        /* reactivo el timer de configuracion (cortito) */
        wifi_count_ap_mode_timer = 50;   
        Led2(0);
    }
    else if( !memcmp(msg, "+CIFSR:", 7))
    {
        /* AT+CIFSR
         * +CIFSR:STAIP,"0.0.0.0"
         * +CIFSR:STAMAC,"80:7d:3a:62:03:60"
         */
        if( !memcmp(&msg[7], "STAMAC", 6))
        {
            wifi_hw_mac[0]  = msg[15];
            wifi_hw_mac[1]  = msg[16];
            wifi_hw_mac[2]  = msg[18];
            wifi_hw_mac[3]  = msg[19];
            wifi_hw_mac[4]  = msg[21];
            wifi_hw_mac[5]  = msg[22];
            wifi_hw_mac[6]  = msg[24];
            wifi_hw_mac[7]  = msg[25];
            wifi_hw_mac[8]  = msg[27];
            wifi_hw_mac[9]  = msg[28];
            wifi_hw_mac[10] = msg[30];
            wifi_hw_mac[11] = msg[31];
            wifi_hw_mac[12] = 0;
        }
    }
        
        
        
        
        
}

void WiFiWaitFor(const char* wait, wifi_task_stm on_ok, wifi_task_stm on_error, int time_out)
{
    if(wait)
    {
        strncpy(wwfst.wait, wait, sizeof(wwfst.wait)-1);
        wwfst.wait[sizeof(wwfst.wait)-1] = 0;
        wwfst.on_ok = on_ok;
        wwfst.on_error = on_error;
        wwfst.time_out = time_out * 10;
    }
    else
    {
        wwfst.wait[0] = 0;
        wwfst.on_ok = 0;
        wwfst.on_error = 0;
        wwfst.time_out = 0;
    }
}

void ProcessReceiveQueue( void )
{
    char msg[WIFI_RX_BUFFER_LEN];
    
    if(QueueGet(wifi_queue_rx, msg, WIFI_RX_BUFFER_LEN))
    {
        printf("WiFi --> %.80s\r\n", msg);

        /* Control de ocurrencia en WaitFor */
        if(wwfst.wait[0])
        {
            if(strstr(msg, wwfst.wait))
            {
                wifi_stm = wwfst.on_ok;
                WiFiWaitFor(0, 0, 0, 0);
                return;
            }
        }
        WiFiCheckUnsol(msg);
    }
}

void WiFiInitSeq( wifi_task_stm stm )
{
    switch(wifi_count_command_seq)
    {
        case 0:
            WifiSendString("ATE0", true);
            WiFiWaitFor("OK", stm, ERROR, 1);
            wifi_count_command_seq++;
            break;
        case 1: break;
        case 2:
            WifiSendString("AT+GMR", true);
            WiFiWaitFor("OK", stm, ERROR, 1);
            wifi_count_command_seq++;
            break;
        case 3: break;
        case 4:
            wifi_stm = stm +1;
            break;
    }
}

void WiFiAPModeSeq( wifi_task_stm stm )
{
    char str[64];

    switch(wifi_count_command_seq)
    {
        case 0:
            WifiSendString("AT+CIPSERVER=0", true);
            WiFiWaitFor("OK", stm, stm, 1);
            wifi_count_command_seq++;
            break;
        case 1: break;
        case 2:
            WifiSendString("AT+CWMODE=3", true);
            WiFiWaitFor("OK", stm, ERROR, 1);
            wifi_count_command_seq++;
            break;
        case 3: break;
        case 4:
            WifiSendString("AT+CIFSR", true);
            WiFiWaitFor("OK", stm, ERROR, 3);
            wifi_count_command_seq++;
            break;
        case 5: break;
        case 6:
            sprintf(str, "AT+CWSAP=\"Dom32io-%s\",\"\",1,0,4,0", &wifi_hw_mac[6]);
            WifiSendString(str, true);
            WiFiWaitFor("OK", stm, ERROR, 3);
            wifi_count_command_seq++;
            break;
        case 7: break;
        case 8:
            WifiSendString("AT+CIPMUX=1", true);
            WiFiWaitFor("OK", stm, ERROR, 1);
            wifi_count_command_seq++;
            break;
        case 9: break;
        case 10:
            sprintf(str, "AT+CIPSERVERMAXCONN=%d", WIFI_MAX_CONN);
            WifiSendString(str, true);
            WiFiWaitFor("OK", stm, ERROR, 1);
            wifi_count_command_seq++;
            break;
        case 11: break;
        case 12:
            WifiSendString("AT+CIPSERVER=1,80", true);
            WiFiWaitFor("OK", stm, ERROR, 3);
            wifi_count_command_seq++;
            break;
        case 13: break;
        case 14:
            wifi_stm = stm+1;
            break;
    }
}

void WiFiSTModeSeq( wifi_task_stm stm )
{
    char str[64];
    char *ap, *pass;

    switch(wifi_count_command_seq)
    {
        case 0:
            WifiSendString("AT+CIPSERVER=0", true);
            WiFiWaitFor("OK", stm, stm, 1);
            wifi_count_command_seq++;
            break;
        case 1: break;
        case 2:
            WifiSendString("AT+CWMODE=1", true);
            WiFiWaitFor("OK", stm, ERROR, 1);
            wifi_count_command_seq++;
            break;
        case 3: break;
        case 4:
            WifiSendString("AT+CIPMUX=1", true);
            WiFiWaitFor("OK", stm, ERROR, 1);
            wifi_count_command_seq++;
            break;
        case 5: break;
        case 6:
            sprintf(str, "AT+CIPSERVERMAXCONN=%d", WIFI_MAX_CONN);
            WifiSendString(str, true);
            WiFiWaitFor("OK", stm, ERROR, 1);
            wifi_count_command_seq++;
            break;
        case 7: break;
        case 8:
            WifiSendString("AT+CIPSERVER=1,80", true);
            WiFiWaitFor("OK", stm, ERROR, 3);
            wifi_count_command_seq++;
            break;
        case 9: break;
        case 10:
            WifiSendString("AT+CWAUTOCONN=1", true);
            WiFiWaitFor("OK", stm, ERROR, 1);
            wifi_count_command_seq++;
            break;
        case 11: break;
        case 12:
            ap = ((g_wifi_info.status.wifi_ap_select == 1)?g_wifi_info.f.config.wifi_ap1:g_wifi_info.f.config.wifi_ap2);
            pass = ((g_wifi_info.status.wifi_ap_select == 1)?g_wifi_info.f.config.wifi_ap1_pass:g_wifi_info.f.config.wifi_ap2_pass);
            if(*ap)
            {
                /* Connect to AP */
                if(*pass)
                {
                    /* Con clave */
                    sprintf(str, "AT+CWJAP=\"%s\",\"%s\"", ap, pass);
                }
                else
                {
                    /* Sin clave */
                    sprintf(str, "AT+CWJAP=\"%s\"", ap);
                }
                WifiSendString(str, true);
                WiFiWaitFor("OK", stm, ERROR, 5);
            }
            else
            {
                printf("WiFi no configurado.\r\n");
                wifi_stm = MAIN;
            }
            wifi_count_command_seq++;
            break;
        case 13: break;
        case 14:
            wifi_stm = MAIN;
            break;
    }
}

void WiFiDataSendSeq( wifi_task_stm stm )
{

    char *p;
    char str[64];
    
    switch(wifi_count_command_seq)
    {
        case 0:
            if(QueueGet(wifi_queue_tx, wifi_data_send_buffer, WIFI_TX_BUFFER_LEN))
            {
                if(wifi_data_send_buffer[0] == 'Q')
                {
                    /* Si es un requerimiento nuevo obtengo IP y port para hacer una conexion nnueva */
                    str_section(wifi_data_send_buffer, 1, wifi_data_send_ip_addr);
                    str_section(wifi_data_send_buffer, 2, str);
                    wifi_data_send_ip_port = atoi(str);
                    wifi_data_send_conn_id++;
                    sprintf(str, "AT+CIPSTART=%u,\"TCP\",\"%s\",%u", 
                            wifi_data_send_conn_id,
                            wifi_data_send_ip_addr,
                            wifi_data_send_ip_port);
                    WifiSendString(str, true);
                    WiFiWaitFor("OK", stm, stm, 10);
                    wifi_count_command_seq++;
                }
                else if(wifi_data_send_buffer[0] == 'R')
                {
                    /* Obtengo el id de la conexion */
                    str_section(wifi_data_send_buffer, 1, str);
                    wifi_data_send_conn_id = atoi(str);
                    wifi_stm = stm;
                    wifi_count_command_seq++;
                 }
                else
                {
                    /* Un mensaje que no corresponde */
                    printf("Un mensaje que no corresponde, Ni Q ni R.\r\n");
                    wifi_stm = MAIN;
                }
            }
            else
            {
                printf("No hay mensajes en la cola.\r\n");
                wifi_stm = MAIN;
            }
            break;
        case 1: break;
        case 2:
            p = strchr(wifi_data_send_buffer, ':');
            if(p)
            {
                p++;
                sprintf(str, "AT+CIPSEND=%u,%u",
                        wifi_data_send_conn_id,
                        strlen(p));
                WifiSendString(str, true);
                WiFiWaitFor(">", stm, stm, 30);
                wifi_count_command_seq++;
            }
            else
            {
                /* Un mensaje que no corresponde */
                printf("Un mensaje que no corresponde. Error de parseo.\r\n");
                wifi_stm = MAIN;
            }
            break;
        case 3: break;
        case 4:
            WifiSendString(p, false);
            WiFiWaitFor("SEND OK", stm, stm, 30);
            wifi_count_command_seq++;
            break;
        case 5: break;
        case 6:
            sprintf(str, "AT+CIPCLOSE=%u", wifi_data_send_conn_id);
            WifiSendString(str, true);
            WiFiWaitFor("OK", stm, stm, 3);
            wifi_count_command_seq++;
            break;
        case 7: break;
        case 8:
            wifi_stm = MAIN;
            break;
    }
}
