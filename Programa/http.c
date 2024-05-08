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
#include "http.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/watchdog.h"
#include "control.h"
#include "wifi.h"
/* DHT #include "dhtxx.h" */
#include "config.h"
#include "clog.h"
#include "sd/fsio.h"

#define DOWNLOAD_TEMP_FILE_NAME "download.tmp"

const char xml_200[] =      "HTTP/1.1 200 OK\r\nConnection: close\r\n"
                            "Content-Type: text/xml\r\n\r\n";
const char http_200[] =     "HTTP/1.1 200 OK\r\n"
                            "Connection: close\r\n"
                            "Content-Type: text/html\r\n"
                            "Cache-Control: no-cache\r\n\r\n";
const char http_404[] =     "HTTP/1.1 404 Not found\r\n"
                            "Connection: close\r\n\r\n"
                            "404: File not found\r\n";
/* POST
 * 1.- %s: URI
 * 2.- %s: Host
 * 3.- %d: Content-Length
 * 4.- %s: datos
 */
const char http_post[] =    "POST %s HTTP/1.1\r\n"
                            "Host: %s\r\n"
                            "Connection: keep-alive\r\n"
                            "Content-Length: %d\r\n"
                            "User-Agent: Dom32io/1.00 (Microchip;PIC32)\r\n"
                            "Accept: text/html,text/xml\r\n"
                            "Content-Type: application/x-www-form-urlencoded\r\n\r\n%s";
/*
 * GET
 * 1.- %s: URI
 * 2.- %s: Host
 */
const char http_get[] =     "GET %s HTTP/1.1\r\n"
                            "Host: %s\r\n"
                            "Connection: close\r\n"
                            "User-Agent: Dom32io/1.00 (Microchip;PIC32)\r\n"
                            "Accept: text/html,text/xml\r\n\r\n";

#define MAX_HTTP_HEAD 256
#define MAX_HTTP_DATA 2048
#define MAX_HTTP_URI  64

#define HTTP_DATA_IN_QUEUE_LEN 2
#define HTTP_DATA_OUT_QUEUE_LEN 3

#define WIFI_HTTP_OUT_ID 0

char http_rqst[MAX_HTTP_DATA + MAX_HTTP_HEAD];
char http_rqst_data[MAX_HTTP_DATA];
char http_rqst_uri[MAX_HTTP_URI];

typedef struct
{
    char type;
    int conn_id;
    int http_rc;
    int form_rc;
    unsigned int timeout;
    char filename[FILE_NAME_SIZE+1];
    char rqst[MAX_HTTP_URI];
    char data[MAX_HTTP_DATA];
} http_data;

int http_data_in_queue_read_idx;
int http_data_in_queue_write_idx;
http_data http_data_in_queue[HTTP_DATA_IN_QUEUE_LEN];
int http_data_out_queue_read_idx;
int http_data_out_queue_write_idx;
http_data http_data_out_queue[HTTP_DATA_OUT_QUEUE_LEN];
unsigned int http_process_tmp_file;
char http_process_tmp_file_to_name[FILE_NAME_SIZE+1];

extern unsigned int main_count_inactividad;
extern int g_wifi_http_response;

int ReadLine(char *buffer, size_t max_len, FSFILE *fs);
int Convert2CharToHex(const char* h);

int HttpDecodeFormData(char* out, const char* in, unsigned int max_out)
{
    int len = 0;
    
    LogMessage("HttpDecodeFormData");

    if(!in || !out) return (-1);
    
    while(*in)
    {
        if(*in == 0x25) /* % */
        {
            in++;
            *out = (unsigned char)Convert2CharToHex(in);
            in += 2;
        }
        else
        {
            *out = *in;
            in++;
        }
        out++;
        len++;
        if(len >= max_out) break;
    }
    *out = 0;
    return (len < max_out)?len:(-1);
}

int HttpGetRequest(const char *buffer, char *request, int max_len)
{
	int rc = 0;
    
    LogMessage("HttpGetRequest");
    
	while(*buffer && *buffer != '/') buffer++;
	if( !(*buffer)) return 0;
	while(*buffer && *buffer != ' ' && rc < max_len)
	{
		*request = *buffer;
		buffer++;
		request++;
		rc++;
	}
	*request = 0;
	return (rc < max_len)?rc:(-1);
}

int HTTPGetFormData(char *buffer, const char *label, char *value)
{
    char *p;
    int len = 0;
    
    p = buffer;
    do
    {
        p = strstr(p, label);
        if(p)
        {
            p += strlen(label);
            if(*p == '=')
            {
                p++;
                while(*p && *p != '&')
                {
                    *value = *p;
                    p++;
                    value++;
                    len++;
                }
                *value = 0;
            }
        }
    } while(p);
    
    return len;
}

int HTTPHeadResponse(const char *http_head)
{
    char str[4];
    char *p1;
    
    LogMessage("[HTTPHeadResponse] %.15s", http_head); 

    if(http_head)
    {
        /* Busco el código de respuesta http en la cabecera
         * "HTTP/1.1 200 OK\r\n" */
        p1 = strchr(http_head, ' ');
        if(p1)
        {
            str[0] = *(++p1);
            str[1] = *(++p1);
            str[2] = *(++p1);
            str[3] = 0;
            return atoi(str);
        }
    }
    return 0;
}

int HTTPFormResponse(const char *http_data)
{
    char str[16];
    
    LogMessage("[HTTPFormResponse] %.15s", http_data); 

    if(http_data)
    {
        if( strchr(http_data, '{') && 
            strchr(http_data, ':') && 
            strchr(http_data, ',') && 
            strchr(http_data, '}')  )
        {
            LogMessage("Respuesta en formato JSON no esperada");
            return 99;
        }
        else if( strchr(http_data, '=') )
        {
            LogMessage("Respuesta en formato de formulario");
            if(HTTPGetFormData((char*)http_data, "error", str))
            {
                LogMessage("Respuesta con error: %s", str);
                return atoi(str);
            }
            else
            {
                /* Si no encuentra la leyenda error no hay error */
                return 0;
            }
        }
        else
        {
            /* Si no dice nada asumo no error */
            return 0;
        }
    }
    return (-1);
}

/*<html>
<head>
<title>Dom32IO</title>
</head>
<style>
body{background-color:grey;font-size:24pt;}
#it{height:75;font-size:24pt}
</style>
<body>
<form method="post">
<h1>WiFi</h1>
<hr />
<h2 >AP1:</h2>
&nbsp;<input id="it" type="text" name="ap1" value="%s">&nbsp;<input id="it" type="text" name="ap1p" value="%s"><br>
<h2 >AP2:</h2>
&nbsp;<input id="it" type="text" name="ap2" value="%s">&nbsp;<input id="it" type="text" name="ap2p" value="%s"><br>
<br>
<br>
&nbsp;&nbsp;<input id="it"  type="submit" value="Guardar">
</form>
</body>
</html>
*/
void HttpReturnWiFiFormConfig(char *out, int max_len)
{
    snprintf(out, max_len, 
            "%s"
            "<html>\n"
            "<head>\n"
            "<title>Dom32IO</title>\n"
            "</head>\n"
            "<style>\n"
            "body{background-color:grey;font-size:24pt;}\n"
            "#it{height:75;font-size:24pt}\n"
            "</style>\n"
            "<body>\n"
            "<form method=\"post\">\n"
            "<h1>WiFi</h1>\n"
            "<p>MAC: %s</p>\n"
            "<hr />\n"
            "<h2>AP1:</h2>\n"
            "&nbsp;<input id=\"it\" type=\"text\" name=\"ap1\" value=\"%s\">&nbsp;<input  id=\"it\" type=\"text\" name=\"ap1p\" value=\"%s\"><br>\n"
            "<h2>AP2:</h2>\n"
            "&nbsp;<input id=\"it\" type=\"text\" name=\"ap2\" value=\"%s\">&nbsp;<input  id=\"it\" type=\"text\" name=\"ap2p\" value=\"%s\"><br>\n"
            "<h2>Central:</h2>\n"
            "&nbsp;<input id=\"it\" type=\"text\" name=\"ce1\" value=\"%s\">&nbsp;<input  id=\"it\" type=\"text\" name=\"ce2\" value=\"%s\"><br>\n"
            "<h2>Path:</h2>\n"
            "&nbsp;<input id=\"it\" type=\"text\" name=\"path\" value=\"%s\"><br>\n"
            "<br>\n"
            "<br>\n"
            "&nbsp;&nbsp;<input  id=\"it\" type=\"submit\" value=\"Guardar\">"
            "</form>\n"
            "</body>\n"
            "</html>\n",
            http_200,
            g_wifi_info.status.hw_mac,
            g_wifi_info.f.config.wifi_ap1, g_wifi_info.f.config.wifi_ap1_pass,
            g_wifi_info.f.config.wifi_ap2, g_wifi_info.f.config.wifi_ap2_pass,
            g_wifi_info.f.config.wifi_host1, g_wifi_info.f.config.wifi_host2,
            g_wifi_info.f.config.rqst_path);
}

void HttpReturnResult(char *out, const char* result, int max_len)
{
    snprintf(out, max_len,
        "%sID=%s&TYP=IO&RESULT=%s",
        http_200,
        g_wifi_info.status.hw_mac,
        result);
    
    LogMessage("HttpReturn: %s - data-len: %d", result, strlen(out));
}

void HttpReturnWiFiConfig(char *out, int max_len)
{
    snprintf(out, max_len,
            "%sID=%s&TYP=IO&ap1=%s&ap1p=%s&ap2=%s&ap2p=%s&ce1=%s&ce1p=%u&ce2=%s&ce2p=%u&report=%u&rqst=%s",
            http_200,
            g_wifi_info.status.hw_mac,
            g_wifi_info.f.config.wifi_ap1, g_wifi_info.f.config.wifi_ap1_pass,
            g_wifi_info.f.config.wifi_ap2, g_wifi_info.f.config.wifi_ap2_pass,
            g_wifi_info.f.config.wifi_host1, g_wifi_info.f.config.wifi_host1_port,
            g_wifi_info.f.config.wifi_host2, g_wifi_info.f.config.wifi_host2_port,
            g_wifi_info.f.config.report/10, g_wifi_info.f.config.rqst_path);
}

/* *****************************************************************************
 * Genera un mensaje WEB con formato de formulario POST
 * y lo copia a un buffer (respuesta) o encola para enviar (notificación)
 * ************************************************************************** */
int HTTPNotificarStatus(char *out, int max_len)
{
    bool first_change;
    
    /* Parte fija con la MAC y las entradas y salidas */
    snprintf(http_rqst_data, MAX_HTTP_DATA,
        "ID=%s&TYP=IO&IO1=%u&IO2=%u&IO3=%u&IO4=%u&IO5=%u&IO6=%u&OUT1=%u&OUT2=%u&OUT3=%u&OUT4=%u",
        g_wifi_info.status.hw_mac,
        g_control_io.stat1,
        g_control_io.stat2,
        g_control_io.stat3,
        g_control_io.stat4,
        g_control_io.stat5,
        g_control_io.stat6,
        (g_control_out.stat1)?1:0,
        (g_control_out.stat2)?1:0,
        (g_control_out.stat3)?1:0,
        (g_control_out.stat4)?1:0);
    
    if( g_control_io.change1 ||
        g_control_io.change2 ||
        g_control_io.change3 ||
        g_control_io.change4 ||
        g_control_io.change5 ||
        g_control_io.change6 ||
        g_control_out.change1 ||
        g_control_out.change2 ||
        g_control_out.change3 ||
        g_control_out.change4  )
    {
        strcat(http_rqst_data, "&CHG=");
        first_change = true;
        
        if(g_control_io.change1)
        {
            if(!first_change) strcat(http_rqst_data, ",");
            first_change = false;
            strcat(http_rqst_data, "IO1");
        }
        if(g_control_io.change2)
        {
            if(!first_change) strcat(http_rqst_data, ",");
            first_change = false;
            strcat(http_rqst_data, "IO2");
        }
        if(g_control_io.change3)
        {
            if(!first_change) strcat(http_rqst_data, ",");
            first_change = false;
            strcat(http_rqst_data, "IO3");
        }
        if(g_control_io.change4)
        {
            if(!first_change) strcat(http_rqst_data, ",");
            first_change = false;
            strcat(http_rqst_data, "IO4");
        }
        if(g_control_io.change5)
        {
            if(!first_change) strcat(http_rqst_data, ",");
            first_change = false;
            strcat(http_rqst_data, "IO5");
        }
        if(g_control_io.change6)
        {
            if(!first_change) strcat(http_rqst_data, ",");
            first_change = false;
            strcat(http_rqst_data, "IO6");
        }
        if(g_control_out.change1)
        {
            if(!first_change) strcat(http_rqst_data, ",");
            first_change = false;
            strcat(http_rqst_data, "OUT1");
        }
        if(g_control_out.change2)
        {
            if(!first_change) strcat(http_rqst_data, ",");
            first_change = false;
            strcat(http_rqst_data, "OUT2");
        }
        if(g_control_out.change3)
        {
            if(!first_change) strcat(http_rqst_data, ",");
            first_change = false;
            strcat(http_rqst_data, "OUT3");
        }
        if(g_control_out.change4)
        {
            if(!first_change) strcat(http_rqst_data, ",");
            first_change = false;
            strcat(http_rqst_data, "OUT4");
        }
    }
    
    /* Agrego info de Temperatura y humedad si está configurado */
    /* DHT
    if(h1 != 0)
    {
        DHTxxTemperatura(&t1,&t2);
        DHTxxHumedad(&h1,&h2); 
        snprintf(&http_rqst_data[strlen(http_rqst_data)], MAX_HTTP_DATA - strlen(http_rqst_data),
                "&TEMP=%i.%i&HUM=%i.%i", t1, t2, h1, h2);
    }
    */
    
    /* Version del stack AT del ESP-02 */
    if((MAX_HTTP_DATA - strlen(http_rqst_data) > sizeof(g_wifi_info.status.wifi_at)))
    {
        snprintf(&http_rqst_data[strlen(http_rqst_data)], MAX_HTTP_DATA - strlen(http_rqst_data), 
                "&AT=%s", g_wifi_info.status.wifi_at);
    }
    /* Version del SDK del ESP-02 */
    if((MAX_HTTP_DATA - strlen(http_rqst_data) > sizeof(g_wifi_info.status.wifi_sdk)))
    {
        snprintf(&http_rqst_data[strlen(http_rqst_data)], MAX_HTTP_DATA - strlen(http_rqst_data), 
                "&SDK=%s", g_wifi_info.status.wifi_sdk);
    }
    /* Compilacion del firmware */
    if((MAX_HTTP_DATA - strlen(http_rqst_data) > sizeof(g_wifi_info.status.up_fw)))
    {
        snprintf(&http_rqst_data[strlen(http_rqst_data)], MAX_HTTP_DATA - strlen(http_rqst_data), 
                "&FW=%s", g_wifi_info.status.up_fw);
    }
    /* Capacidad SSL del módulo WIFI */
    if((MAX_HTTP_DATA - strlen(http_rqst_data) > 10))
    {
        snprintf(&http_rqst_data[strlen(http_rqst_data)], MAX_HTTP_DATA - strlen(http_rqst_data), 
                "&SSL=%s", (g_wifi_info.status.wifi_ssl_capable)?"YES":"NO");
    }
    
    /* Agrego solicitud de configuración */
    if(g_wifi_info.f.config.io_config_default && (MAX_HTTP_DATA - strlen(http_rqst_data) > 10) )
    {
        strcat(http_rqst_data, "&GETCONF=1");
    }
    
    
    snprintf(http_rqst_uri, MAX_HTTP_URI, "%s/infoio.cgi", g_wifi_info.f.config.rqst_path);
/* POST
 * 1.- %s: URI
 * 2.- %s: Host
 * 3.- %d: Content-Length
 * 4.- %s: datos
 */
    snprintf(http_rqst, MAX_HTTP_DATA + MAX_HTTP_HEAD, http_post,
            http_rqst_uri,
            g_wifi_info.status.wifi_host_select,
            strlen(http_rqst_data),
            http_rqst_data);

    LogMessage("HTTPNotificarStatus - [%s] data-len: %d", http_rqst_uri, strlen(http_rqst));

    /* Debo poner en un buffer o enviar? */
    if(out)
    {
        /* Poner en un buffer */
        strncpy(out, http_rqst, max_len);
    }
    else if(HTTPAddOUTQueue(http_rqst_uri, http_rqst, 100) <= 0)
    {
        /* No fue posible enviar */
        return 0;
    }
    
    /* Reseteo los cambios */
    g_control_io.change1 = 0;
    g_control_io.change2 = 0;
    g_control_io.change3 = 0;
    g_control_io.change4 = 0;
    g_control_io.change5 = 0;
    g_control_io.change6 = 0;
    g_control_out.change1 = 0;
    g_control_out.change2 = 0;
    g_control_out.change3 = 0;
    g_control_out.change4 = 0;

    return strlen(http_rqst);
}

#ifdef INCLUDE_IO_FORMS
void HttpReturnIOStatusForm(char *out, int max_len)
{
    snprintf(out, max_len,
            "%s"
            "<html>\n"
            "<head>\n"
            "<title>Dom32IO - IO Status</title>\n"
            "</head>\n"
            "<style>\n"
            "body{background-color:grey;font-size:24pt;}\n"
            "#it{height:75;font-size:24pt}\n"
            "</style>\n"
            "<body>\n"
            "<form method=\"POST\">\n"
            "<h1>I/O</h1><hr />\n"
            "PORT 1:&nbsp;"
            "<select id=\"it\" name=\"IO1\" onchange=\"submit();\"><option value=\"on\"%s>on</option><option value=\"off\"%s>off</option></select>\n"
            "<select id=\"it\" name=\"IO2\" onchange=\"submit();\"><option value=\"on\"%s>on</option><option value=\"off\"%s>off</option></select>\n"
            "<select id=\"it\" name=\"IO3\" onchange=\"submit();\"><option value=\"on\"%s>on</option><option value=\"off\"%s>off</option></select>\n"
            "<select id=\"it\" name=\"IO4\" onchange=\"submit();\"><option value=\"on\"%s>on</option><option value=\"off\"%s>off</option></select>\n"
            "</form>\n"
            "</body>\n"
            "</html>\n",
            http_200,
            (g_control_io.stat1)?" selected":" ", (g_control_io.stat1)?" ":" selected",
            (g_control_io.stat2)?" selected":" ", (g_control_io.stat2)?" ":" selected",
            (g_control_io.stat3)?" selected":" ", (g_control_io.stat3)?" ":" selected",
            (g_control_io.stat4)?" selected":" ", (g_control_io.stat4)?" ":" selected");
}

void HttpReturnIOConfigForm(char *out, int max_len)
{
    snprintf(out, max_len
            "%s"
            "<html>\n"
            "<head>\n"
            "<title>Dom32IO - IO Config</title>\n"
            "</head>\n"
            "<style>\n"
            "body{background-color:grey;font-size:24pt;}\n"
            "#it{height:75;font-size:24pt}\n"
            "</style>\n"
            "<body>\n"
            "<form method=\"POST\">\n"
            "<h1>I/O</h1><hr />\n"
            "PORT 1:&nbsp;"
            "<select id=\"it\" name=\"IO1\" onchange=\"submit();\"><option value=\"in\"%s>in</option><option value=\"out\"%s>out</option></select>\n"
            "<select id=\"it\" name=\"IO2\" onchange=\"submit();\"><option value=\"in\"%s>in</option><option value=\"out\"%s>out</option></select>\n"
            "<select id=\"it\" name=\"IO3\" onchange=\"submit();\"><option value=\"in\"%s>in</option><option value=\"out\"%s>out</option></select>\n"
            "<select id=\"it\" name=\"IO4\" onchange=\"submit();\"><option value=\"in\"%s>in</option><option value=\"out\"%s>out</option></select>\n"
            "</form>\n"
            "</body>\n"
            "</html>\n",
            http_200,
            (g_control_io.dir1)?" selected":" ", (g_control_io.dir1)?" ":" selected",
            (g_control_io.dir2)?" selected":" ", (g_control_io.dir2)?" ":" selected",
            (g_control_io.dir3)?" selected":" ", (g_control_io.dir3)?" ":" selected",
            (g_control_io.dir4)?" selected":" ", (g_control_io.dir4)?" ":" selected");
}

#endif /* INCLUDE_IO_FORMS */

void HttpReturnIOStatus(char *out, int max_len)
{
    snprintf(out, max_len,
        "%sID=%s&TYP=IO&IO1=%u&IO2=%u&IO3=%u&IO4=%u&IO5=%u&IO6=%u&OUT1=%u&OUT2=%u&OUT3=%u&OUT4=%u",
        http_200,
        g_wifi_info.status.hw_mac,
        g_control_io.stat1,
        g_control_io.stat2,
        g_control_io.stat3,
        g_control_io.stat4,
        g_control_io.stat5,
        g_control_io.stat6,
        (g_control_out.stat1)?1:0,
        (g_control_out.stat2)?1:0,
        (g_control_out.stat3)?1:0,
        (g_control_out.stat4)?1:0);
    
    /* Agrego info de Temperatura y humedad si está configurado */
    /* DHT
    if(h1 != 0)
    {
        DHTxxTemperatura(&t1,&t2);
        DHTxxHumedad(&h1,&h2); 
        snprintf(&out[strlen(out)], max_len - strlen(out),
                "&TEMP=%i.%i&HUM=%i.%i", t1, t2, h1, h2);
    }
    */
    /* Version del stack AT del ESP-02 */
    if((MAX_HTTP_DATA - strlen(out) > sizeof(g_wifi_info.status.wifi_at)))
    {
        snprintf(&out[strlen(out)], MAX_HTTP_DATA - strlen(out), 
                "&AT=%s", g_wifi_info.status.wifi_at);
    }
    /* Version del SDK del ESP-02 */
    if((MAX_HTTP_DATA - strlen(out) > sizeof(g_wifi_info.status.wifi_sdk)))
    {
        snprintf(&out[strlen(out)], MAX_HTTP_DATA - strlen(out), 
                "&SDK=%s", g_wifi_info.status.wifi_sdk);
    }
    /* Compilacion del firmware */
    if((MAX_HTTP_DATA - strlen(out) > sizeof(g_wifi_info.status.up_fw)))
    {
        snprintf(&out[strlen(out)], MAX_HTTP_DATA - strlen(out), 
                "&FW=%s", g_wifi_info.status.up_fw);
    }
    /* Capacidad SSL del módulo WIFI */
    if((MAX_HTTP_DATA - strlen(out) > 10))
    {
        snprintf(&out[strlen(out)], MAX_HTTP_DATA - strlen(out), 
                "&SSL=%s", (g_wifi_info.status.wifi_ssl_capable)?"YES":"NO");
    }
    
    LogMessage("HttpReturnIOStatus - data-len: %d", strlen(out));
}

void HttpReturnIOConfig(char *out, int max_len)
{
    snprintf(out, max_len,
            "%sID=%s&TYP=IO&IO1=%s&IO2=%s&IO3=%s&IO4=%s&IO5=%s&IO6=%s",
            http_200,
            g_wifi_info.status.hw_mac,
            (g_wifi_info.f.config.io_config & 0x01)?((g_wifi_info.f.config.an_config & 0x01)?"ana":"in"):"out",
            (g_wifi_info.f.config.io_config & 0x02)?((g_wifi_info.f.config.an_config & 0x02)?"ana":"in"):"out",
            (g_wifi_info.f.config.io_config & 0x04)?((g_wifi_info.f.config.an_config & 0x04)?"ana":"in"):"out",
            (g_wifi_info.f.config.io_config & 0x08)?((g_wifi_info.f.config.an_config & 0x08)?"ana":"in"):"out",
            (g_wifi_info.f.config.io_config & 0x10)?((g_wifi_info.f.config.an_config & 0x10)?"ana":"in"):"out",
            (g_wifi_info.f.config.io_config & 0x20)?((g_wifi_info.f.config.an_config & 0x20)?"ana":"in"):"out");
    if(g_wifi_info.f.config.io_config_default && (strlen(out)+10) < max_len)
    {
        strcat(out, "&GETCONF=1");
    }
}

void HttpProcessWiFiFormConfig(char* in)
{
    char str[8];
    
    HTTPGetFormData(in, "ap1", g_wifi_info.f.config.wifi_ap1);
    HTTPGetFormData(in, "ap1p", g_wifi_info.f.config.wifi_ap1_pass);
    HTTPGetFormData(in, "ap2", g_wifi_info.f.config.wifi_ap2);
    HTTPGetFormData(in, "ap2p", g_wifi_info.f.config.wifi_ap2_pass);
    HTTPGetFormData(in, "ce1", g_wifi_info.f.config.wifi_host1);
    if(HTTPGetFormData(in, "ce1p", str))
    {
        g_wifi_info.f.config.wifi_host1_port = atoi(str);
    }
    HTTPGetFormData(in, "ce2", g_wifi_info.f.config.wifi_host2);
    if(HTTPGetFormData(in, "ce2p", str))
    {
        g_wifi_info.f.config.wifi_host2_port = atoi(str);
    }
    HTTPGetFormData(in, "path", g_wifi_info.f.config.rqst_path);
    if(HTTPGetFormData(in, "report", str))
    {
        g_wifi_info.f.config.report = atoi(str) * 10;
        if(g_wifi_info.f.config.report == 0)
        {
            g_wifi_info.f.config.report = 600;
        }
    }

    g_wifi_info.status.save_config = 50;
}

/* *****************************************************************************
 * Inicializacion, llamar al inicio
 * ************************************************************************** */
void HTTPInit( void )
{
    http_data_in_queue_read_idx = 0;
    http_data_in_queue_write_idx = 0;
    http_data_out_queue_read_idx = 0;
    http_data_out_queue_write_idx = 0;
    http_process_tmp_file = 0;
}

/* *****************************************************************************
 * Tareas de HTTP, llamar desde el loop de procesamiento
 * ************************************************************************** */
void HTTPTask( void )
{
    if(http_data_in_queue_read_idx == http_data_in_queue_write_idx) return;

    if(http_data_in_queue[http_data_in_queue_read_idx].type == 'G')
    {
        LogMessage("GET HTTP: %s %.15s",
                http_data_in_queue[http_data_in_queue_read_idx].rqst,
                http_data_in_queue[http_data_in_queue_read_idx].data);
        
        if( !strcmp(http_data_in_queue[http_data_in_queue_read_idx].rqst, "/"))
        {
            if(g_wifi_info.status.wifi_mode == 3)
            {
                HttpReturnWiFiFormConfig(http_data_in_queue[http_data_in_queue_read_idx].data, MAX_HTTP_DATA);
            }
            else
            {
                HttpReturnIOStatus(http_data_in_queue[http_data_in_queue_read_idx].data, MAX_HTTP_DATA);
            }
        }
        else if( !strcmp(http_data_in_queue[http_data_in_queue_read_idx].rqst, "/status.htm"))
        {
            HttpReturnIOStatus(http_data_in_queue[http_data_in_queue_read_idx].data, MAX_HTTP_DATA);
        }
        else if( !strcmp(http_data_in_queue[http_data_in_queue_read_idx].rqst, "/iostatus.htm"))
        {
            HttpReturnIOStatus(http_data_in_queue[http_data_in_queue_read_idx].data, MAX_HTTP_DATA);
        }
        else if( !strcmp(http_data_in_queue[http_data_in_queue_read_idx].rqst, "/config.htm"))
        {
            HttpReturnIOConfig(http_data_in_queue[http_data_in_queue_read_idx].data, MAX_HTTP_DATA);
        }
        else if( !strcmp(http_data_in_queue[http_data_in_queue_read_idx].rqst, "/ioconfig.htm"))
        {
            HttpReturnIOConfig(http_data_in_queue[http_data_in_queue_read_idx].data, MAX_HTTP_DATA);
        }
        else if( !strcmp(http_data_in_queue[http_data_in_queue_read_idx].rqst, "/reset.htm"))
        {
            CTRLReset();
            HttpReturnResult(http_data_in_queue[http_data_in_queue_read_idx].data, "Ok", MAX_HTTP_DATA);
        }
#ifdef INCLUDE_IO_FORMS
        else if( !strcmp(http_data_in_queue[http_data_in_queue_read_idx].rqst, "/iostatus.cgi"))
        {
            HttpReturnIOStatusForm(http_data_in_queue[http_data_in_queue_read_idx].data);
        }
        else if( !strcmp(http_data_in_queue[http_data_in_queue_read_idx].rqst, "/ioconfig.cgi"))
        {
            HttpReturnIOConfigForm(http_data_in_queue[http_data_in_queue_read_idx].data);
        }
#endif /* INCLUDE_IO_FORMS */
        else
        {
            strncpy(http_data_in_queue[http_data_in_queue_read_idx].data, http_404, MAX_HTTP_DATA);
        }
        /* Genero la respuesta */
        HTTPAddRespQueue(http_data_in_queue[http_data_in_queue_read_idx].conn_id, 
                http_data_in_queue[http_data_in_queue_read_idx].data);
    }
    else if(http_data_in_queue[http_data_in_queue_read_idx].type == 'P')
    {
        LogMessage("POST HTTP: %s %.15s",
                http_data_in_queue[http_data_in_queue_read_idx].rqst,
                http_data_in_queue[http_data_in_queue_read_idx].data);

        if( !strcmp(http_data_in_queue[http_data_in_queue_read_idx].rqst, "/"))
        {
            HttpProcessWiFiFormConfig(http_data_in_queue[http_data_in_queue_read_idx].data);
            HttpReturnWiFiFormConfig(http_data_in_queue[http_data_in_queue_read_idx].data, MAX_HTTP_DATA);
        }
        else if( !strcmp(http_data_in_queue[http_data_in_queue_read_idx].rqst, "/wifi.cgi"))
        {
            HttpProcessWiFiFormConfig(http_data_in_queue[http_data_in_queue_read_idx].data);
            HttpReturnWiFiConfig(http_data_in_queue[http_data_in_queue_read_idx].data, MAX_HTTP_DATA);
        }
        else if( !strcmp(http_data_in_queue[http_data_in_queue_read_idx].rqst, "/iostatus.cgi"))
        {
            CTRLSetIO(http_data_in_queue[http_data_in_queue_read_idx].data);
#ifdef INCLUDE_IO_FORMS
            HttpReturnIOStatusForm(http_data_in_queue[http_data_in_queue_read_idx].data);
#else
            HttpReturnIOStatus(http_data_in_queue[http_data_in_queue_read_idx].data, MAX_HTTP_DATA);
#endif /* INCLUDE_IO_FORMS */
        }
        else if( !strcmp(http_data_in_queue[http_data_in_queue_read_idx].rqst, "/ioswitch.cgi"))
        {
            CTRLSwitchIO(http_data_in_queue[http_data_in_queue_read_idx].data);
            HttpReturnIOStatus(http_data_in_queue[http_data_in_queue_read_idx].data, MAX_HTTP_DATA);
        }
        else if( !strcmp(http_data_in_queue[http_data_in_queue_read_idx].rqst, "/ioconfig.cgi"))
        {
            CTRLConfIO(http_data_in_queue[http_data_in_queue_read_idx].data);
#ifdef INCLUDE_IO_FORMS
            HttpReturnIOConfigForm(http_data_in_queue[http_data_in_queue_read_idx].data);
#else
            HttpReturnIOConfig(http_data_in_queue[http_data_in_queue_read_idx].data, MAX_HTTP_DATA);
#endif /* INCLUDE_IO_FORMS */
        }
        else
        {
            strncpy(http_data_in_queue[http_data_in_queue_read_idx].data, http_404, MAX_HTTP_DATA);
        }
        /* Genero la respuesta */
        HTTPAddRespQueue(http_data_in_queue[http_data_in_queue_read_idx].conn_id,
                http_data_in_queue[http_data_in_queue_read_idx].data);
    }
    else if(http_data_in_queue[http_data_in_queue_read_idx].type == 'H')
    {
        LogMessage("RESP HTTP: %i %.15s",
                http_data_in_queue[http_data_in_queue_read_idx].http_rc,
                http_data_in_queue[http_data_in_queue_read_idx].data);
        /* Se trata de una respuesta HTTP/1... */
        if(http_data_in_queue[http_data_in_queue_read_idx].http_rc == 200 &&
           http_data_in_queue[http_data_in_queue_read_idx].form_rc == 0 )
        {
            g_wifi_http_response = 1;
            /*CTRLCompareIO(http_data_in_queue[http_data_in_queue_read_idx].data);*/
            CTRLSetIO(http_data_in_queue[http_data_in_queue_read_idx].data);
        }
        else
        {
            g_wifi_http_response = (-1);
        }
        main_count_inactividad = 0;
    }
    else
    {
        LogMessage("HTTP: typ: %c DESCONOCIDO %.15s",
                http_data_in_queue[http_data_in_queue_read_idx].http_rc,
                http_data_in_queue[http_data_in_queue_read_idx].data);
    }
    
    /* Avanzo en la cola */
    http_data_in_queue_read_idx++;
    if(http_data_in_queue_read_idx == HTTP_DATA_IN_QUEUE_LEN) http_data_in_queue_read_idx = 0;
}

/* *****************************************************************************
 * 
 * ************************************************************************** */
void HTTPTimer( void )
{
    if(http_data_out_queue_read_idx != http_data_out_queue_write_idx)
    {
        if(http_data_out_queue[http_data_out_queue_read_idx].timeout)
        {
            http_data_out_queue[http_data_out_queue_read_idx].timeout--;
            if(http_data_out_queue[http_data_out_queue_read_idx].timeout == 0)
            {
                /* time-out */
                LogMessage("[HTTPTimer] time-out %.20s", http_data_out_queue[http_data_out_queue_read_idx].data);
                http_data_out_queue_read_idx++;
                if(http_data_out_queue_read_idx == HTTP_DATA_OUT_QUEUE_LEN) http_data_out_queue_read_idx = 0;
            }
        }
    }
    
    if(http_process_tmp_file)
    {
        http_process_tmp_file--;
        if(http_process_tmp_file == 0)
        {
            HTTPProcessTmpFile(DOWNLOAD_TEMP_FILE_NAME, http_process_tmp_file_to_name);
            FSremove(DOWNLOAD_TEMP_FILE_NAME);
            if( !strcmp(http_process_tmp_file_to_name, "pgm.hex"))
            {
                CTRLReset();
            }
        }
    }
}

int HTTPAddOUTQueue(const char* filename, const char* buffer, unsigned int to_cs)
{
    int next_i;
    int i;
    
    LogMessage("[HTTPAddOUTQueue] %.40s", buffer);
    
    /* Avanzo hasta la última '/' para dejar el nombre sin path */
    while(strchr(filename, '/'))
    {
        while(*filename != '/') filename++;
        filename++;
    }
    
    /* Punto de inserción */
    next_i = http_data_out_queue_write_idx + 1;
    /* Cola circular */
    if(next_i == HTTP_DATA_OUT_QUEUE_LEN) next_i = 0;
    /* Cola llena */
    if(next_i == http_data_out_queue_read_idx) return (-1);
    /* Cargo los datos en la cola */
    http_data_out_queue[http_data_out_queue_write_idx].type = 'S';
    http_data_out_queue[http_data_out_queue_write_idx].conn_id = WIFI_HTTP_OUT_ID;
    http_data_out_queue[http_data_out_queue_write_idx].timeout = to_cs;
    
    /* copio solo caracteres validos */
    for(i = 0; *filename; filename++)
    {
        if( (*filename >= '0' && *filename <= '9') ||
            (*filename >= 'a' && *filename <= 'z') ||
            (*filename >= 'A' && *filename <= 'Z') ||
             *filename == '.' || *filename == '_' )
        {
            http_data_out_queue[http_data_out_queue_write_idx].filename[i++] = *filename;
        }
    }
    http_data_out_queue[http_data_out_queue_write_idx].filename[i] = 0;
    
    http_data_out_queue[http_data_out_queue_write_idx].rqst[0] = 0;
    http_data_out_queue[http_data_out_queue_write_idx].data[0] = 0;
    if(HttpDecodeFormData(http_data_out_queue[http_data_out_queue_write_idx].data, buffer, MAX_HTTP_DATA) <= 0)
    {
        LogMessage("Data overflow");
        return (-1);
    }
    /* Adelanto el indice de la cola */
    http_data_out_queue_write_idx = next_i;
    return 1;
}

int HTTPAddRespQueue(int id, const char* buffer)
{
    int next_i;
    
    LogMessage("[HTTPAddRespQueue] %.40s", buffer);
    
    /* Punto de inserción */
    next_i = http_data_out_queue_write_idx + 1;
    /* Cola circular */
    if(next_i == HTTP_DATA_OUT_QUEUE_LEN) next_i = 0;
    /* Cola llena */
    if(next_i == http_data_out_queue_read_idx) return (-1);
    /* Cargo los datos en la cola */
    http_data_out_queue[http_data_out_queue_write_idx].type = 'R';
    http_data_out_queue[http_data_out_queue_write_idx].conn_id = id;
    http_data_out_queue[http_data_out_queue_write_idx].timeout = 0;
    http_data_out_queue[http_data_out_queue_write_idx].rqst[0] = 0;
    http_data_out_queue[http_data_out_queue_write_idx].data[0] = 0;
    if(HttpDecodeFormData(http_data_out_queue[http_data_out_queue_write_idx].data, buffer, MAX_HTTP_DATA) <= 0)
    {
        LogMessage("Data overflow");
        return (-1);
    }
    /* Adelanto el indice de la cola */
    http_data_out_queue_write_idx = next_i;
    return 1;
}

int HTTPAddINQueue(int id, const char* buffer)
{
    char *p;
    int next_i;
    
    LogMessage("[HTTPAddINQueue] %.40s", buffer);
    
    /* Punto de inserción */
    next_i = http_data_in_queue_write_idx + 1;
    /* Cola circular */
    if(next_i == HTTP_DATA_IN_QUEUE_LEN) next_i = 0;
    /* Cola llena */
    if(next_i == http_data_in_queue_read_idx) return (-1);
    /* Cargo los datos en la cola */
    if(*buffer == 'G' || *buffer == 'P')
    {
        LogMessage("    requerimiento.");
        http_data_in_queue[http_data_in_queue_write_idx].type = *buffer;
        http_data_in_queue[http_data_in_queue_write_idx].conn_id = id;
        http_data_in_queue[http_data_in_queue_write_idx].timeout = 50;
        http_data_in_queue[http_data_in_queue_write_idx].http_rc = 0;
        http_data_in_queue[http_data_in_queue_write_idx].form_rc = 0;
        http_data_in_queue[http_data_in_queue_write_idx].rqst[0] = 0;
        http_data_in_queue[http_data_in_queue_write_idx].data[0] = 0;

        if(HttpGetRequest(buffer, http_data_in_queue[http_data_in_queue_write_idx].rqst, MAX_HTTP_URI) <= 0)
        {
            LogMessage("Request overflow");
            return (-1);
        }
        if(http_data_in_queue[http_data_in_queue_write_idx].type == 'P')
        {
            p = strstr(buffer, "\r\n\r\n");
            if(p)
            {
                p += 4;
                if(HttpDecodeFormData(http_data_in_queue[http_data_in_queue_write_idx].data, p, MAX_HTTP_DATA) <= 0)
                {
                    LogMessage("Data overflow");
                    return (-1);
                }
            }
        }
    }
    else if(*buffer == 'H')
    {
        LogMessage("    respuesta.");
        http_data_in_queue[http_data_in_queue_write_idx].type = *buffer;
        http_data_in_queue[http_data_in_queue_write_idx].http_rc = HTTPHeadResponse(buffer);
        if(http_data_in_queue[http_data_in_queue_write_idx].http_rc == 200)
        {
            LogMessage("      200 OK");
            http_data_in_queue[http_data_in_queue_write_idx].type = *buffer;
            http_data_in_queue[http_data_in_queue_write_idx].timeout = 50;
            p = strstr(buffer, "\r\n\r\n");
            if(p)
            {
                p += 4;
                if(HttpDecodeFormData(http_data_in_queue[http_data_in_queue_write_idx].data, p, MAX_HTTP_DATA) <= 0)
                {
                    LogMessage("Data overflow");
                    return (-1);
                }
                http_data_in_queue[http_data_in_queue_write_idx].form_rc = HTTPFormResponse(http_data_in_queue[http_data_in_queue_write_idx].data);
                if(http_data_in_queue[http_data_in_queue_write_idx].form_rc != 0)
                {
                    LogMessage("      FORM Error %i", http_data_in_queue[http_data_in_queue_write_idx].form_rc);
                }
            }
        }
        else
        {
            LogMessage("      HTTP Error %i", http_data_in_queue[http_data_in_queue_write_idx].http_rc);
            http_data_in_queue[http_data_in_queue_write_idx].form_rc = 0;
        }
    }

    /* Adelanto el indice de la cola */
    http_data_in_queue_write_idx = next_i;
    return 1;
}

/* *****************************************************************************
 * Devuelve si hay mensajes pendientes en cola de salida
 * ************************************************************************** */
int HTTPOutPending(void)
{
    if(http_data_out_queue_read_idx == http_data_out_queue_write_idx) return 0;
    else return 1;
}

int HTTPOutConnId(void)
{
    if(http_data_out_queue_read_idx == http_data_out_queue_write_idx) return 0;
    else return http_data_out_queue[http_data_out_queue_read_idx].conn_id;
}

/* *****************************************************************************
 * Devuelte el tipo de mensaje en cola de salida
 * ************************************************************************** */
char HTTPOutType(void)
{
    if(http_data_out_queue_read_idx == http_data_out_queue_write_idx) return 0;
    else return http_data_out_queue[http_data_out_queue_read_idx].type;
}

/* *****************************************************************************
 * Saca un mensaje de la cola de salida para procesar
 * ************************************************************************** */
const char* HTTPOutData(void)
{
    if(http_data_out_queue_read_idx == http_data_out_queue_write_idx) return (const char*) NULL;
    else return (const char*)(&http_data_out_queue[http_data_out_queue_read_idx].data[0]);
}

/* *****************************************************************************
 * 
 * ************************************************************************** */
unsigned int HTTPOutTimeOut(void)
{
    if(http_data_out_queue_read_idx == http_data_out_queue_write_idx) return 0;
    else return http_data_out_queue[http_data_out_queue_read_idx].timeout;
}

/* *****************************************************************************
 * 
 * ************************************************************************** */
const char* HTTPOutFilename(void)
{
    if(http_data_out_queue_read_idx == http_data_out_queue_write_idx) return (const char*) NULL;
    else return (const char*)(&http_data_out_queue[http_data_out_queue_read_idx].filename[0]);
}

/* *****************************************************************************
 * Avanza la cola de salida
 * ************************************************************************** */
void HTTPOutNext(void)
{
    if(http_data_out_queue_read_idx == http_data_out_queue_write_idx) return;
    /* Avanzo en la cola */
    http_data_out_queue_read_idx++;
    if(http_data_out_queue_read_idx == HTTP_DATA_OUT_QUEUE_LEN) http_data_out_queue_read_idx = 0;
}

int HTTPNotificarTarjeta( const char *crd)
{
    LogMessage("HTTPNotificarTarjeta");

    snprintf(http_rqst_data, MAX_HTTP_DATA, "ID=%s&TYP=IO&CARD=%s", g_wifi_info.status.hw_mac, crd);
    snprintf(http_rqst_uri, MAX_HTTP_URI, "%s/infoio.cgi", g_wifi_info.f.config.rqst_path);
/* POST
 * 1.- %s: URI
 * 2.- %s: Host
 * 3.- %d: Content-Length
 * 4.- %s: datos
 */
    snprintf(http_rqst, MAX_HTTP_DATA + MAX_HTTP_HEAD, http_post,
            http_rqst_uri,
            g_wifi_info.status.wifi_host_select,
            strlen(http_rqst_data),
            http_rqst_data);
    if(HTTPAddOUTQueue(http_rqst_uri, http_rqst, 100) <= 0)
    {
        /* No fue posible enviar */
        return 0;
    }
    return strlen(http_rqst);
}

int HTTPGetFile(const char* filename, const char* params)
{
    if( !filename ) return (-1);

    LogMessage("[HTTPGetFile] Solicitando archivo %s.", filename);
    
    if(params)
    {
        snprintf(http_rqst_uri, MAX_HTTP_URI, "%s?%s", filename, params);
    }
    else
    {
        strncpy(http_rqst_uri, filename, MAX_HTTP_URI);
    }
    /*
     * GET
     * 1.- %s: URI
     * 2.- %s: Host
     */
    snprintf(http_rqst, MAX_HTTP_DATA + MAX_HTTP_HEAD, http_get,
             http_rqst_uri,
             g_wifi_info.status.wifi_host_select);
    if(HTTPAddOUTQueue(http_rqst_uri, http_rqst, 3000) <= 0)
    {
        /* No fue posible enviar */
        return 0;
    }
    return strlen(http_rqst);
}

int HTTPInitTmpFile(const char* buffer, int len)
{
    FSFILE* f;
    int rc = (-1);

    
    //LogMessage("[HTTPInitTmpFile]");
    f = FSfopen(DOWNLOAD_TEMP_FILE_NAME, FS_WRITE);
    if(f)
    {
        rc = FSfwrite(buffer, 1, len, f);
        FSfclose(f);
    }
    return rc;
}

int HTTPAddTmpFile(const char* buffer, int len)
{
    FSFILE* f;
    int rc = (-1);

    //LogMessage("[HTTPAddTmpFile] %i bytes -> TMP", len);
    f = FSfopen(DOWNLOAD_TEMP_FILE_NAME, FS_APPEND);
    if(f)
    {
        rc = FSfwrite(buffer, 1, len, f);
        FSfclose(f);
    }
    return rc;
}

int HTTPEndTmpFile(const char* filename, const char* buffer, int len)
{
    int rc = 0;

    //LogMessage("[HTTPEndTmpFile] %i bytes -> TMP", len);
    if(len)
    {
        rc = HTTPAddTmpFile(buffer, len);
    }
    /* Procesar archivo temporal */
    g_wifi_http_response = 1;
    http_process_tmp_file = 50;
    strncpy(http_process_tmp_file_to_name, filename, FILE_NAME_SIZE);
    return rc;
}

/*
 Sirve solo para archivos de texto
 */
void HTTPProcessTmpFile(const char* tmpname, const char* filename)
{
    char buffer[80];
    char*p;
    char*p_ipd_len;
    int len;
    int ipd_len;
    FSFILE* fi;
    FSFILE* fo;
    bool descartar;

    LogMessage("[HTTPProcessTmpFile] %s -> %s", tmpname, filename);
    fi = FSfopen(tmpname, FS_READ);
    if(fi)
    {
        fo = FSfopen(filename, FS_WRITE);
        if(fo)
        {
            ipd_len = 0;
            descartar = TRUE;
            while( (len = ReadLine(buffer, 80, fi)) > 0 )
            {
                WATCHDOG_TimerClear();
                
                buffer[len] = 0;
                
                if( len == 2 && buffer[0] == 0x0d && buffer[1] == 0x0a )
                {
                    /* Descarto todo hasta el final de la cabecera HTTP */
                    descartar = FALSE;
                    /* Descarto todo lo que esté fuera del bloque +IPD */
                    if(ipd_len > len) ipd_len -= len;
                    else ipd_len = 0;
                }
                else if(ipd_len)
                {
                    if( !descartar)
                    {
                        p = &buffer[0];
                        FSfwrite(p, 1, (ipd_len > len)?len:ipd_len, fo);
                    }
                    /* Descarto todo lo que esté fuera del bloque +IPD */
                    if(ipd_len > len) ipd_len -= len;
                    else ipd_len = 0;
                }
                else if( buffer[0] == '+' &&
                         buffer[1] == 'I' &&
                         buffer[2] == 'P' &&
                         buffer[3] == 'D'  )
                {
                    /* Comienzo de in bloque +IPD,0,1460: */
                    p = &buffer[0];
                    /* Salteo la primer coma */
                    while(*p != ',') { p++; len--; }
                    p++; len--;
                    /* Salteo la segunda coma coma */
                    while(*p != ',') { p++; len--; }
                    p++; len--;
                    /*  */
                    p_ipd_len = p;
                    /* Voy hasta el fin de la cabecera +IPD */
                    while(*p != ':') { p++; len--; }
                    *p = 0;
                    ipd_len = atoi(p_ipd_len);
                    p++; len--;
                    /*   Estoy aca: v
                     *  +IPD,0,1460:___ */
                    if( !descartar)
                    {
                        /* Escribo el resto de la linea */
                        FSfwrite(p, 1, (ipd_len > len)?len:ipd_len, fo);
                    }
                    /* Descarto todo lo que esté fuera del bloque +IPD */
                    if(ipd_len > len) ipd_len -= len;
                    else ipd_len = 0;
                }
            }                
            FSfclose(fo);
        }
        FSfclose(fi);
    }        
}

int ReadLine(char *buffer, size_t max_len, FSFILE *fs)
{
    int len = 0;

    while(FSfread(buffer, 1, 1, fs) && len < max_len)
    {
        len++;
        if(*buffer == 0x0a) break;
        buffer++;
    }
    return len;
}

int Convert2CharToHex(const char* h)
{
    int h1, h2;
    
    if( (*h) >= '0' && (*h) <= '9' )
    {
        h1 = ((*h)-'0');
    }
    else if( (*h) >= 'A' && (*h) <= 'F' )
    {
        h1 = ((*h)-'A'+10);
    }
    else if( (*h) >= 'a' && (*h) <= 'f' )
    {
        h1 = ((*h)-'a'+10);
    }
    else
    {
        h1 = 0;
    }
    h++;
    if( (*h) >= '0' && (*h) <= '9' )
    {
        h2 = ((*h)-'0');
    }
    else if( (*h) >= 'A' && (*h) <= 'F' )
    {
        h2 = ((*h)-'A'+10);
    }
    else if( (*h) >= 'a' && (*h) <= 'f' )
    {
        h2 = ((*h)-'a'+10);
    }
    else
    {
        h2 = 0;
    }

    return ((h1*16)+h2);
}
