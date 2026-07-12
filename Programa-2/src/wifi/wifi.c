/***************************************************************************
    Copyright (C) 2026   Walter Pirri

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

#include <xc.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "../defines.h"
#include "uart1.h"
#include "../log.h"
#include "../config.h"
#include "../misc/strfunc.h"
#include "../sd/fsio.h"
#include "../http.h"
#include "../io.h"

#define WIFI_RESET_SetHigh()    (LATBSET=(1<<10))
#define WIFI_RESET_SetLow()     (LATBCLR=(1<<10))
#define UART_WRITE              UART1_Write
#define UART_OVERFLOW           UART1_Overflow
#define UART_ISRXREADY          UART1_IsRxReady
#define UART_READ               UART1_Read

volatile unsigned int g_wifi_c_timer_count;
int g_wifi_c_mode;
int g_wifi_c_go_mode;
int g_wifi_c_ap_conectado;
int g_wifi_c_go_ap_conectado;
int g_wifi_c_sta_conectado;
char g_wifi_c_hw_mac[13];

void WifiWrite( const char *buffer );
void WifiWriteLine( const char *line );
int WifiReadAt( char *data, unsigned int max_data, char at, int to );
int WifiReadCount( char *data, unsigned int count, int to );
void WiFiWait(int t);
int WiFiCheckUnsolMsg(const char* msg);
int WiFiCommand(const char* cmd, const char* wait, int to);
void WiFiReadUnsolIPD(const char* msg);

/* ************************************************************************* */
void WiFiInit( void )
{
    g_wifi_c_ap_conectado = 0;
    g_wifi_c_go_ap_conectado = 0;
    g_wifi_c_mode = 0;
    g_wifi_c_go_mode = 0;
    g_wifi_c_sta_conectado = 0;

    UART1_Init();
}

void WiFiTimer( void )
{
    if(g_wifi_c_timer_count) g_wifi_c_timer_count--;
}

void WiFiTask( void )
{

    
}

int WiFiStart( void )
{
    LogMessage("[WIFI] Start");
    g_wifi_c_ap_conectado = 0;
    g_wifi_c_mode = 0;
    /* Reseteo el módulo. Bajo y subo el pin de reset */
    WIFI_RESET_SetLow();
    WiFiWait(50);
    WIFI_RESET_SetHigh();
    /* Leo y descarto lo que pueda tirar cuando levanta */
    WifiReadCount(NULL, 9999, 100);
    /* Trato de volverlo a fábrica y espero ready 10 seg */
    if(WiFiCommand("AT+RESTORE", "ready", 1000) < 0) return (-1);
    /* ECHO OFF */
    if(WiFiCommand("ATE0", "OK", 150) < 0) return (-1);
    /* Version */
    if(WiFiCommand("AT+GMR", "OK", 300) < 0) return (-1);
    /* Check soporte SSL */
    if(WiFiCommand("AT+CIPSSLCCONF?", "OK", 150) < 0) return (-1);
    /* Seteo el modo AP+ST para poder tomar las MAC */
    if(WiFiCommand("AT+CWMODE=3", "OK", 150) < 0) return (-1);
    /* Obtengo la MAC */
    if(WiFiCommand("AT+CIFSR", "OK", 150) < 0) return (-1);
    /* Me quedo en Modo 0 - Puede devolver error*/
    WiFiCommand("AT+CWMODE=0", "OK", 150);
    
    return 0;
}

int WiFiMode( void )
{
    return g_wifi_c_mode;
}

int WiFiConectado( void )
{
    return g_wifi_c_ap_conectado;
}

int WiFiStaConectado( void )
{
    return g_wifi_c_sta_conectado;
}

void WiFiSwitchRouter( void )
{
    if(g_wifi_c_go_ap_conectado == 1) g_wifi_c_go_ap_conectado = 2;
    else                              g_wifi_c_go_ap_conectado = 1;
}

int WiFiSetMode(int mode)
{
    char cmd[128];

    LogMessage("[WIFI] SetMode %i", mode);    
    
    if(mode <  1 || mode > 3 ) return (-1);

    g_wifi_c_ap_conectado = 0;
    g_wifi_c_mode = 0;

    /* Apago cualquier LISTEN que pueda haber - Puede dar error */
    WiFiCommand("AT+CIPSERVER=0", "OK", 150);
    /* Si estaba en otro modo me muevo al 0 - Puede dar error*/
    WiFiCommand("AT+CWMODE=0", "OK", 150);
    /* Seteo el modo */
    sprintf(cmd, "AT+CWMODE=%i", mode);
    if(WiFiCommand(cmd, "OK", 150) < 0) return (-1);
    /* Multiconexion 0=no 1=si*/
    if(WiFiCommand("AT+CIPMUX=1", "OK", 150) < 0) return (-1);

    /* Solo para AP */
    if(mode == WIFI_MODE_AP || mode == WIFI_MODE_AP_ST)
    {
        sprintf(cmd, "AT+CWSAP=\"Dom32touch-%s\",\"\",1,0,4,0", &g_wifi_c_hw_mac[6]);
        if(WiFiCommand(cmd, "OK", 150) < 0) return (-1);
        /* Max conexiones */
        if(WiFiCommand("AT+CIPSERVERMAXCONN=1", "OK", 150) < 0) return (-1);
        /* LISTEN en port 80 */
        if(WiFiCommand("AT+CIPSERVER=1,80", "OK", 150) < 0) return (-1);
    }

    /* Solo para ST */
    if(mode == WIFI_MODE_ST || mode == WIFI_MODE_AP_ST)
    {
        /* Reconexión automática */
        if(WiFiCommand("AT+CWAUTOCONN=1", "OK", 150) < 0) return (-1);
        /* Me conecto con el AP */
        if(g_wifi_c_go_ap_conectado == 0) g_wifi_c_go_ap_conectado = 1;
        /* Si no hay AP Definido no puedo seguir */
        if(g_wifi_config.u.config.wifi_ap1[0] == 0 && g_wifi_config.u.config.wifi_ap2[0] == 0)
        {
            LogMessage("[WIFI] No hay AP definido");
            if(mode == WIFI_MODE_AP_ST)
            {
                /* Si es combinado AT+ST */
                g_wifi_c_mode = mode;
                return 0;
            }
            else
            {
                return (-1);
            }
        }
        /* Si el seleccionado no está configurado me paso al otro */
        if(g_wifi_c_go_ap_conectado == 1 && g_wifi_config.u.config.wifi_ap1[0] == 0)
        {
            g_wifi_c_go_ap_conectado = 2;
        }
        else if(g_wifi_c_go_ap_conectado == 2 && g_wifi_config.u.config.wifi_ap2[0] == 0)
        {
            g_wifi_c_go_ap_conectado = 1;
        }
        if(g_wifi_c_go_ap_conectado == 1)
        {
            /* Uso el AP1 */
            LogMessage("[WifiTaskSTMode] Conectando con AP1 %s (%s).",
                    g_wifi_config.u.config.wifi_ap1,
                    g_wifi_config.u.config.wifi_ap1_pass);
            if(g_wifi_config.u.config.wifi_ap1_pass[0])
            {
                /* Con clave */
                sprintf(cmd, "AT+CWJAP=\"%s\",\"%s\"",
                    g_wifi_config.u.config.wifi_ap1,
                    g_wifi_config.u.config.wifi_ap1_pass);
            }
            else
            {
                /* Sin clave */
                sprintf(cmd, "AT+CWJAP=\"%s\"",
                    g_wifi_config.u.config.wifi_ap1);
            }
        }
        else
        {
            /* Uso el AP2 */
            LogMessage("[WifiTaskSTMode] Conectando con AP2 %s (%s).",
                    g_wifi_config.u.config.wifi_ap2,
                    g_wifi_config.u.config.wifi_ap2_pass);
            if(g_wifi_config.u.config.wifi_ap2_pass[0])
            {
                /* Con clave */
                sprintf(cmd, "AT+CWJAP=\"%s\",\"%s\"",
                    g_wifi_config.u.config.wifi_ap2,
                    g_wifi_config.u.config.wifi_ap2_pass);
            }
            else
            {
                /* Sin clave */
                sprintf(cmd, "AT+CWJAP=\"%s\"",
                    g_wifi_config.u.config.wifi_ap2);
            }
        }
        if(WiFiCommand(cmd, "OK", 1000) < 0) return (-1);
        /* IP y MAC cuando conecte va a devolver la IP */
        if(WiFiCommand("AT+CIFSR", "OK", 300) < 0) return (-1);
    }
    g_wifi_c_mode = mode;
    return 0;
}

void WiFiCheckUnsol( void )
{
    char buffer[256];
    
    if(WifiReadAt(buffer, 255, '\n', 2) > 0)
    {
        WiFiCheckUnsolMsg(buffer);
    }
}

const char* WiFiMAC( void )
{
    return ((const char*)(&g_wifi_c_hw_mac[0]));
}

void WifiWrite( const char *buffer )
{
    if(buffer)
    {
        LogMessage("[WIFI] WifiWrite [%-.100s]", buffer);
        while(*buffer)
        {
            UART_WRITE((uint8_t)*buffer);
            buffer++;
        }
    }
}

void WifiWriteLine( const char *line )
{
    if(line)
    {
        LogMessage("[WIFI] WifiWriteLine [%-.100s]", line);
        while(*line)
        {
            UART_WRITE((uint8_t)*line);
            line++;
        }
    }
    UART_WRITE('\r');
    UART_WRITE('\n');
}

int WifiReadAt( char *data, unsigned int max_data, char at, int to )
{
    unsigned int read_len = 0;
    char ch = at+1;
    
    if(UART_OVERFLOW()) return (-1);

    g_wifi_c_timer_count = to;
    do {
        CLRWDT();
        while( UART_ISRXREADY() && ch != at && read_len < max_data && g_wifi_c_timer_count > 0)
        {
            CLRWDT();
            ch = UART_READ();
            if(data/* && ch != 0x0d && ch != 0x0a*/)
            {
                /* Extiendo el tiempo cada vez que recibo algo válido */
                if(g_wifi_c_timer_count < to) g_wifi_c_timer_count++;
                *data = ch;
                data++;
            }
            read_len++;
        }
    } while (ch != at && read_len < max_data && g_wifi_c_timer_count > 0);

    if(g_wifi_c_timer_count == 0) return (-1);

    if(data) *data = 0;
    return read_len;
}

int WifiReadCount( char *data, unsigned int count, int to )
{
    unsigned int read_count = 0;
    char ch;
    
    if(UART_OVERFLOW()) return (-1);

    g_wifi_c_timer_count = to;
    do {
        CLRWDT();
        while( UART_ISRXREADY() && read_count < count && g_wifi_c_timer_count > 0)
        {
            CLRWDT();
            ch = UART_READ();
            if(data)
            {
                /* Extiendo el tiempo cada vez que recibo algo válido */
                if(g_wifi_c_timer_count < to) g_wifi_c_timer_count++;
                *data = ch;
                data++;
            }
            read_count++;
        }
    } while (read_count < count && g_wifi_c_timer_count > 0);
    
    if(g_wifi_c_timer_count == 0) return (read_count)?read_count:(-1);

    return read_count;
}

void WiFiWait(int t)
{
    g_wifi_c_timer_count = t;
    while(g_wifi_c_timer_count > 0)
    {
        CLRWDT();
    }
}

int WiFiCheckUnsolMsg(const char* msg)
{
    LogMessage("[WIFI] Unsol [%s]", msg);

    if( !memcmp(msg, "WIFI CONNECTED", 14))
    {
        /* ST Conectada al Router */
    }
    else if( !memcmp(msg, "WIFI GOT IP", 11))
    {
        /* IP obtenida del Router */
        g_wifi_c_ap_conectado = g_wifi_c_go_ap_conectado;
    }
    else if( !memcmp(msg, "WIFI DISCONNECTED", 17))
    {
        /* Desconectado del Router */
        g_wifi_c_ap_conectado = 0;
    }
    else if( !memcmp(msg, "+STA_CONNECTED", 8))
    {
        /* Cliente conectado al AP */
        g_wifi_c_sta_conectado = 1;
    }
    else if( !memcmp(msg, "+STA_DISCONNECTED", 8))
    {
        /* Cliente desconectado del AP */
        g_wifi_c_sta_conectado = 0;
    }
    else if( !memcmp(msg, "+CIFSR:", 7))
    {
        if( !memcmp((const char*)(msg+7), "STAMAC", 6))
        {
            LogMessage("[WiFi] MAC %.17s", (const char*)(msg+15));
            g_wifi_c_hw_mac[0]  = *(msg+15);
            g_wifi_c_hw_mac[1]  = *(msg+16);
            g_wifi_c_hw_mac[2]  = *(msg+18);
            g_wifi_c_hw_mac[3]  = *(msg+19);
            g_wifi_c_hw_mac[4]  = *(msg+21);
            g_wifi_c_hw_mac[5]  = *(msg+22);
            g_wifi_c_hw_mac[6]  = *(msg+24);
            g_wifi_c_hw_mac[7]  = *(msg+25);
            g_wifi_c_hw_mac[8]  = *(msg+27);
            g_wifi_c_hw_mac[9]  = *(msg+28);
            g_wifi_c_hw_mac[10] = *(msg+30);
            g_wifi_c_hw_mac[11] = *(msg+31);
            g_wifi_c_hw_mac[12] = 0;
        }
        else if( !memcmp((const char*)(msg+7), "STAIP", 5))
        {
            if(strlen((const char*)(msg+14)))
            {
                LogMessage("[WiFi] IP %s", (const char*)(msg+14) );
            }
        }
    }
    else if( !memcmp(msg, "AT version:", 11))
    {
    }
    else if( !memcmp(msg, "SDK version:", 12))
    {
    }
    else if( !memcmp(msg, "compile time:", 13))
    {
    }
    else if( !memcmp(msg, "+IPD", 4))
    {
        WiFiReadUnsolIPD(msg);
    }
    
    return 0;
}

int WiFiCommand(const char* cmd, const char* wait, int to)
{
    char buffer[256];
    
    WifiWriteLine(cmd);
    do
    {
        if(WifiReadAt(buffer, 255, '\n', to) < 0)
        {
            LogMessage("[WiFiCommand] TIMEOUT: Esperando [%s]", wait);
            return (-1);
        }
        if( !strstr(buffer, wait))
        {
            LogMessage("[WiFiCommand] Recibido: [%s]", buffer);
            if( !memcmp(buffer, "ERROR", 5))
            {
                LogMessage("[WiFiCommand] ERROR: Esperando [%s]", wait);
                return (-1);
            }
            /*
            else if( !memcmp(buffer, "ALREADY CONNECTED", 17))
            {
                LogMessage("[WIFI] Deconectando y Reintentando [%s]", cmd);
                WiFiCommand("AT+CIPCLOSE=0", "OK", 100);
                WifiWriteLine(cmd);
                continue;
            }
            */
            WiFiCheckUnsolMsg(buffer);
        }
    } while( !strstr(buffer, wait));
    LogMessage("[WIFI] %s -> %s", cmd, wait);
    return 0;
}

int WiFiQueryHost(const char* host, int port, const char* send_msg, char* recv_msg)
{
    char cmd[WIFI_BUFFER_LEN+1];
    char str_tmp[256];
    int data_len = 0;
    int recv_len = 0;
    int rc;
    char* p;

    LogMessage("[WIFI] WiFiQueryHost: Enviando %i bytes a %s:%i", strlen(send_msg), host, port);
    
    if(WiFiConectado() == 0)
    {
        LogMessage("[WIFI] WiFiQueryHost: WiFi no conectado");
        return (-1);
    }
    LED_Verde_SetHigh();
    /* Conexión */
    sprintf(cmd, "AT+CIPSTART=0,\"%s\",\"%s\",%i",
            (g_wifi_config.u.config.enable_https)?"SSL":"TCP", host, port);
    if(WiFiCommand(cmd, "OK", 1000) < 0)
    {
        LED_Verde_SetLow();
        LogMessage("[WIFI] WiFiQueryHost: time-Out intentando conectar con %s:%i", host, port);
        return (-1);
    }
    /* Send */
    sprintf(cmd, "AT+CIPSEND=0,%d", strlen(send_msg));
    WifiWriteLine(cmd);
    /* Espero el prompt para enviar los datos */
    if(WifiReadAt(NULL, 9999, '>', 1000) < 0)
    {
        LED_Verde_SetLow();
        LogMessage("[WIFI] WiFiQueryHost: Time-Out esperando enviar datos");
        WiFiCommand("AT+CIPCLOSE=0", "OK", 100);
        return (-1);
    }
    if(WiFiCommand(send_msg, "SEND OK", 1000) < 0)
    {
        LED_Verde_SetLow();
        LogMessage("[WIFI] WiFiQueryHost: Time-Out esperando SEND OK");
        WiFiCommand("AT+CIPCLOSE=0", "OK", 100);
        return (-1);
    }

    /*
        [+IPD,0,147:HTTP/1.1 200 OK]
        [date: Tue, 02 Jun 2026 21:15:09 GMT]
        [server: uvicorn]
        [content-length: 22]
        [content-type: application/json]     

        +IPD,0,324:HTTP/1.1 200 OK\r\n
        date: Sun, 07 Jun 2026 21:13:20 GMT\r\n
        server: uvicorn\r\n
        content-type: application/octet-stream\r\n
        accept-ranges: bytes\r\n
        content-disposition: attachment; filename="domtouch.hex"\r\n
        content-length: 234753\r\n
        last-modified: Fri, 05 Jun 2026 16:08:43 GMT\r\n
        etag: "143788c1e6107729cd29afe54f569547"\r\n
        connection: close\r\n
        \r\n
    */
    
    LogMessage("[WIFI] WiFiQueryHost: Esperando respuesta");
    /* Recibo la cabecera IPD o el close */
    while( (rc = WifiReadAt(cmd, WIFI_BUFFER_LEN, '\n', 500)) >= 0)
    {
        cmd[rc] = 0;
        if( !memcmp(cmd, "+IPD", 4))
        {
            if(str_section(cmd, ',', 2, str_tmp))
            {
                data_len = atoi(str_tmp);
            }
            if(strchr(cmd, ':'))
            {
                p = ((char*)(strchr(cmd, ':')+1));
                recv_len = strlen(p);
                if(recv_msg)
                {
                    strcpy(recv_msg, p);
                    recv_msg += recv_len;
                }
            }
        }
        else if( cmd[0] == '\r' && cmd[1] == '\n' && data_len )
        {
            recv_len += rc;
            if(recv_msg)
            {
                strcpy(recv_msg, cmd);
                recv_msg += rc;
            }
            break;
        }
        else
        {
            recv_len += rc;
            if(recv_msg)
            {
                strcpy(recv_msg, cmd);
                recv_msg += rc;
            }
        }
    }

    /* Recibo los datos */
    while(recv_len < data_len)
    {
        rc = WifiReadCount(cmd, ((data_len-recv_len)<WIFI_BUFFER_LEN)?(data_len-recv_len):WIFI_BUFFER_LEN, 500 );
        if(rc < 0)
        {
            LogMessage("[WIFI] WiFiQueryHost Time-Out faltando %i bytes", (recv_len < data_len));
            break;
        }
        cmd[rc] = 0;
        if(recv_msg)
        {
            memcpy(recv_msg, cmd, rc);
            recv_msg += rc;
            *recv_msg = 0;
        }
        recv_len += rc;
    }

    LogMessage("[WIFI] WiFiQueryHost: Esperando Cierre de conexion");
    /* Espero el cierre de conexión */
    while(WifiReadAt(cmd, 256, '\n', 10) >= 0)
    {
        if( !memcmp(cmd, "0,CLOSED", 8))
        {
            break;
        }
    }
    LED_Verde_SetLow();    
    /* Me aseguro el Close */
    WiFiCommand("AT+CIPCLOSE=0", "OK", 100);

    return recv_len;
}

int WiFiQueryHostToFile(const char* host, int port, const char* send_msg, const char* filename)
{
    char cmd[WIFI_BUFFER_LEN+1];
    char str_tmp[256];
    char real_filename[256];
    int data_len = 0;
    int recv_len = 0;
    int rc;
    char *p, *q;
    char* p_ipd;
    unsigned long content_length = 0;
    unsigned long total_escrito = 0;
    bool http_200_ok = false;
    FSFILE* f;

    real_filename[0] = 0;
    
    //LogMessage("[WiFiQueryHostToFile] Enviando %i bytes a %s:%i", strlen(send_msg), host, port);
    
    if(WiFiConectado() == 0)
    {
        LogMessage("[WiFiQueryHostToFile] WiFi no conectado");
        return (-1);
    }
    
    /* Conexión */
    sprintf(cmd, "AT+CIPSTART=0,\"%s\",\"%s\",%i",
            (g_wifi_config.u.config.enable_https)?"SSL":"TCP", host, port);
    if(WiFiCommand(cmd, "OK", 1000) < 0)
    {
        LogMessage("[WiFiQueryHostToFile] Time-Out intentando conectar con %s:%i", host, port);
        return (-1);
    }
    /* Send */
    sprintf(cmd, "AT+CIPSEND=0,%d", strlen(send_msg));
    WifiWriteLine(cmd);
    /* Espero el prompt para enviar los datos */
    if(WifiReadAt(NULL, 9999, '>', 1000) < 0)
    {
        LogMessage("[WiFiQueryHostToFile] Time-Out esperando enviar datos");
        WiFiCommand("AT+CIPCLOSE=0", "OK", 100);
        return (-1);
    }
    if(WiFiCommand(send_msg, "SEND OK", 1000) < 0)
    {
        LogMessage("[WiFiQueryHostToFile] Time-Out esperando SEND OK");
        WiFiCommand("AT+CIPCLOSE=0", "OK", 100);
        return (-1);
    }

    /*
        [+IPD,0,147:HTTP/1.1 200 OK]
        [date: Tue, 02 Jun 2026 21:15:09 GMT]
        [server: uvicorn]
        [content-length: 22]
        [content-type: application/json]     

        +IPD,0,324:HTTP/1.1 200 OK\r\n
        date: Sun, 07 Jun 2026 21:13:20 GMT\r\n
        server: uvicorn\r\n
        content-type: application/octet-stream\r\n
        accept-ranges: bytes\r\n
        content-disposition: attachment; filename="domtouch.hex"\r\n
        content-length: 234753\r\n
        last-modified: Fri, 05 Jun 2026 16:08:43 GMT\r\n
        etag: "143788c1e6107729cd29afe54f569547"\r\n
        connection: close\r\n
        \r\n
    */
    
    LogMessage("[WiFiQueryHostToFile] Requerimiento enviado, esperando respuesta");
    /* Recibo la cabecera IPD o el close */
    while( (rc = WifiReadAt(cmd, WIFI_BUFFER_LEN, '\n', 500)) >= 0)
    {
        cmd[rc] = 0;
        if( !memcmp(cmd, "+IPD", 4))
        {
            if(str_section(cmd, ',', 2, str_tmp))
            {
                data_len = atoi(str_tmp);
            }
            if(strchr(cmd, ':'))
            {
                p = ((char*)(strchr(cmd, ':')+1));
                recv_len = strlen(p);
                /* La cabecera HTTP no va al archivo*/
                if(strstr(p, "200 OK"))
                {
                    http_200_ok = true;
                }
            }
        }
        else if( cmd[0] == '\r' && cmd[1] == '\n' && data_len )
        {
            recv_len += rc;
            /* La cabecera HTTP no va al archivo*/
            break;
        }
        else
        {
            recv_len += rc;
            /* La cabecera HTTP no va al archivo*/
            /* Tomo datos de la cabecera HTTP */
            /* Parseo la cabacara http */
            if(HTTPGetHeadData(cmd, "Content-Length", str_tmp))
            {
                LogMessage("[WiFiQueryHostToFile] Content-Length [%s]", str_tmp);
                content_length = atol(str_tmp);
            }

            if(HTTPGetHeadData(cmd, "Content-Disposition", str_tmp))
            {
                LogMessage("[WiFiQueryHostToFile] Content-Disposition [%s]", str_tmp);
                if( !memcmp(str_tmp, "attachment;", 11) && strstr(str_tmp, "filename=") )
                {
                    p = (char*)(strstr(str_tmp, "filename=")+9);
                    q = &real_filename[0];
                    while(*p && ( (*p >= 0x30 && *p <= 0x39)||
                                  (*p >= 0x41 && *p <= 0x5A)||
                                  (*p >= 0x61 && *p <= 0x7A)||
                                  *p == 0x22 || *p == 0x2E||
                                  *p == 0x2D || *p == 0x5F||
                                  *p == 0x7E) )
                    {
                        if(*p != 0x22)
                        {
                            *q = *p;
                            q++;
                        }
                        p++;
                    }
                    *q = 0;
                    LogMessage("[WiFiQueryHostToFile] Archivo [%s]", real_filename);
                }
            }
        }
    }

    /* Se terminó de recibir la cabecera HTTP */
    LogMessage("[WiFiQueryHostToFile] Recibiendo archivo de %i bytes", content_length);
    f = FSfopen(TMP_FILENAME, FS_WRITE);
    if( !f )
    {
        LogMessage("[WiFiQueryHostToFile] ERROR intentando abrir el archivo temporal [%s]", TMP_FILENAME);
        WiFiCommand("AT+CIPCLOSE=0", "OK", 100);
        return (-1);
    }

    do
    {
        /* Recibo los datos */
        while(recv_len < data_len)
        {
            rc = WifiReadCount(cmd, ((data_len-recv_len)<WIFI_BUFFER_LEN)?(data_len-recv_len):WIFI_BUFFER_LEN, 100 );
            if(rc < 0)
            {
                LogMessage("[WiFiQueryHostToFile] Time-Out faltando %i bytes", (data_len - recv_len));
                break;
            }
            cmd[rc] = 0;
            recv_len += rc;
            
            /* Escribir cmd x rc bytes al archivo */
            FSfwrite(cmd, rc, 1, f);
            total_escrito += rc;
        }
        rc = WifiReadCount(cmd, 16, 500);
        if(rc > 0)
        {
            cmd[rc] = 0;
            p_ipd = strstr(cmd, "+IPD");
            if(p_ipd)
            {
                if(str_section(p_ipd, ',', 2, str_tmp))
                {
                    data_len = atoi(str_tmp);
                }
                if(strchr(p_ipd, ':'))
                {
                    p = ((char*)(strchr(p_ipd, ':')+1));
                    recv_len = rc - ((int)(p - &cmd[0]));

                    /* Escribir p x rc - ((int)(p - &cmd[0])) bytes al archivo */
                    FSfwrite(p, recv_len, 1, f);
                    total_escrito += recv_len;
                }                
            }
            else if(content_length == total_escrito)
            {
                break;
            }
            else
            {
                LogMessage("[WiFiQueryHostToFile] ERROR esperando cabecera +IPD");
                break;
            }
        }
    } while (rc >= 0);

    if(real_filename[0] == 0)
    {
        strcpy(real_filename, filename);
    }
    
    LogMessage("[WiFiQueryHostToFile] Archivo [%s] %lu/%lu bytes",
                real_filename, content_length, total_escrito);
    
    if(http_200_ok && content_length == total_escrito)
    {
        FSremove(real_filename);
        FSrename(real_filename, f);
        FSfclose(f);
    }
    
    LogMessage("[WiFiQueryHostToFile] Esperando Cierre de conexion");
    /* Espero el cierre de conexión */
    while(WifiReadAt(cmd, 256, '\n', 10) >= 0)
    {
        if( !memcmp(cmd, "0,CLOSED", 8))
        {
            break;
        }
    }
    
    /* Me aseguro el Close */
    WiFiCommand("AT+CIPCLOSE=0", "OK", 100);

    return recv_len;
}

void WiFiReadUnsolIPD(const char* msg)
{
    int rc;
    char str[256];
    char read_buffer[2048];
    unsigned int data_len = 0;
    char *p;

    char uri[256];
    char host[256];
    char request[8];
    unsigned int content_length = 0;
    
    LogMessage("[WiFiReadUnsolIPD] msg: [%s]", msg);

    strcpy(read_buffer, msg);
    rc = strlen(msg);
    
    do
    {
        read_buffer[rc] = 0;
        
        if( !memcmp(read_buffer, "+IPD", 4))
        {
            if(str_section(read_buffer, ',', 2, str))
            {
                data_len = atoi(str);
            }
            if(strchr(read_buffer, ':'))
            {
                p = ((char*)(strchr(read_buffer, ':')+1));
            }
            /* De esta primera parte saco algunos datos */
            if( !memcmp(p, "GET ", 4))
            {
                strcpy(request, "GET");
            }
            else if( !memcmp(p, "POST ", 5))
            {
                strcpy(request, "POST");
            }
            HttpGetRequest(p, uri, 255);
            
            
        }
        else if( read_buffer[0] == '\r' && read_buffer[1] == '\n' && data_len )
        {
            /* Fin de la cabecera */
            break;
        }
        else
        {
            /* La cabecera HTTP no va al archivo*/
            /* Tomo datos de la cabecera HTTP que necesito */
            if(HTTPGetHeadData(read_buffer, "Content-Length", str))
            {
                content_length = atol(str);
            }
            HTTPGetHeadData(read_buffer, "Host", host);

        }
    } while( (rc = WifiReadAt(read_buffer, 2047, '\n', 500)) > 0);
    /* se terminó de recibir la cabecera, falta recibir [content_length] bytes */
    /* Ya tengo request, uri, request, host y content_length */
    
    if(content_length > 2048)
    {
        WiFiCommand("AT+CIPCLOSE=0", "OK", 100);
        LogMessage("[WiFiReadUnsolIPD] Por leer %u bytes, no hay espacio");
        return;
    }
    
    if(content_length > 0)
    {
        if( (rc = WifiReadCount(read_buffer, content_length, 5000 )) <= 0)
        {
            WiFiCommand("AT+CIPCLOSE=0", "OK", 100);
            LogMessage("[WiFiReadUnsolIPD] No se pudieron recibir los %u bytes restantes", content_length);
            return;
        }
        read_buffer[rc] = 0;
        LogMessage("[WiFiReadUnsolIPD] Recibido: %u Eperado: %i bytes", content_length, rc);
    }

    
    if(HttpProcessRequest(request, uri, host, content_length, read_buffer, 2048) > 0)
    {
        /* Send */
        sprintf(str, "AT+CIPSEND=0,%d", strlen(read_buffer));
        WifiWriteLine(str);
        /* Espero el prompt para enviar los datos */
        if(WifiReadAt(NULL, 9999, '>', 1000) < 0)
        {
            LogMessage("[WiFiReadUnsolIPD] Time-Out esperando enviar datos");
            WiFiCommand("AT+CIPCLOSE=0", "OK", 100);
            return;
        }
        if(WiFiCommand(read_buffer, "SEND OK", 1000) < 0)
        {
            LogMessage("[WiFiReadUnsolIPD] Time-Out esperando SEND OK");
            WiFiCommand("AT+CIPCLOSE=0", "OK", 100);
            return;
        }
    }
    WiFiCommand("AT+CIPCLOSE=0", "OK", 100);
}
