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
#include "http.h"

#include <xc.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

#include "defines.h"
#include "log.h"
#include "config.h"
#include "control.h"
#include "wifi/wifi.h"
#include "misc/strfunc.h"

#define _DEBUG_HTTP

int g_http_c_host_conectado;
int g_http_c_go_host_conectado;

const char http_c_xml_200[] = "HTTP/1.1 200 OK\r\n"
                              "Connection: close\r\n"
                              "Content-Type: text/xml\r\n\r\n";

const char http_c_http_200[] = "HTTP/1.1 200 OK\r\n"
                               "Connection: close\r\n"
                               "Content-Type: text/html\r\n"
                               "Cache-Control: no-cache\r\n\r\n";

const char http_c_http_404[] = "HTTP/1.1 404 Not found\r\n"
                               "Connection: close\r\n\r\n"
                               "404: File not found\r\n";
/* POST
 * 1.- %s: URI
 * 2.- %s: Host
 * 3.- %d: Content-Length
 * 4.- %s: datos
 */
const char http_c_http_post[] = "POST %s HTTP/1.1\r\n"
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
const char http_c_http_get[] = "GET %s HTTP/1.1\r\n"
                               "Host: %s\r\n"
                               "Connection: close\r\n"
                               "User-Agent: Dom32io/1.00 (Microchip;PIC32)\r\n"
                               "Accept: text/html,text/xml\r\n\r\n";

#ifndef strcasestr
char *strcasestr(const char *haystack, const char *needle) {
    if (!*needle) return (char *)haystack;

    for (; *haystack; haystack++) {
        const char *h = haystack;
        const char *n = needle;

        while (*h && *n && (tolower((unsigned char)*h) == tolower((unsigned char)*n))) {
            h++;
            n++;
        }

        if (!*n) return (char *)haystack;
    }

    return NULL;
}
#endif /* strcasestr */

int Convert2CharToHex(const char* h);
int HttpReturnWiFiFormConfig(char *out, int max_len);

void HttpInit(void)
{
    g_http_c_host_conectado = 0;
    g_http_c_go_host_conectado = 0;

}

void HttpTask(void)
{
    
}

void HttpTimerInt(void)
{
    
}

int HttpOnLine( void )
{
    if(WiFiConectado() == 0) g_http_c_host_conectado = 0;
    return (g_http_c_host_conectado > 0)?1:0;
}

char* HttpCurrentHost( void )
{
    if(g_http_c_host_conectado == 1)
    {
        return &g_wifi_config.u.config.wifi_host1[0];
    }
    else
    {
        return &g_wifi_config.u.config.wifi_host2[0];
    }
    return NULL;
}

int HttpCurrentPort( void )
{
    if(g_http_c_host_conectado == 1)
    {
        return g_wifi_config.u.config.wifi_host1_port;
    }
    else
    {
        return g_wifi_config.u.config.wifi_host2_port;
    }
    return 0;
}

void HttpConexionOk( void )
{
    g_http_c_host_conectado = g_http_c_go_host_conectado;
}

void HttpConexionError( void )
{
    g_http_c_host_conectado = 0;
}

void HttpSwitchHost( void )
{
    if(g_http_c_go_host_conectado == 1) g_http_c_go_host_conectado = 2;
    else                              g_http_c_go_host_conectado = 1;
}

int HttpDecodeFormData(char* out, const char* in, unsigned int max_out)
{
    int len = 0;
    
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

int HTTPGetFormData(const char *buffer, const char *label, char *value)
{
    const char *p;
    int len = 0;
    
    p = buffer;
    do
    {
        p = strstr(p, label);
        if(p)
        {
            len = 0;
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

int HTTPGetHeadData(const char *buffer, const char *label, char *value)
{
    const char *p;
    int len = 0;
    
    p = strcasestr(buffer, label);
    if(p)
    {
        p += strlen(label);
        if(*p == ':')
        {
            p++;
            while(*p == ' ') p++;
            while(*p && /**p != ' ' &&*/ *p != '\r' && *p != '\n')
            {
                *value = *p;
                p++;
                value++;
                len++;
            }
            *value = 0;
        }
    }
    return len;
}

int HTTPHeadResponse(const char *http_head)
{
    char str[4];
    char *p1;
    
    if(http_head)
    {
        /* Busco el c�digo de respuesta http en la cabecera
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
    
    if(http_data)
    {
        if( strchr(http_data, '{') && 
            strchr(http_data, ':') && 
            strchr(http_data, ',') && 
            strchr(http_data, '}')  )
        {
            LogMessage("[HTTPFormResponse] Respuesta en formato JSON no esperada");
            return 99;
        }
        else if( strchr(http_data, '=') )
        {
#ifdef DEBUG_HTTP
            LogMessage("[HTTPFormResponse] Respuesta en formato de formulario");
#endif
            if(HTTPGetFormData((char*)http_data, "error", str))
            {
#ifdef DEBUG_HTTP
                LogMessage("[HTTPFormResponse] Respuesta con error: %s", str);
#endif
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

int HttpQueryHost(const char* url, const char* msg_send, char* resp_header, char* resp_data)
{
    char* host;
    int port;
    char http_buffer[4096];
    int recv_len;
    char* head_end;
    int head_len;
    int http_rc = 0;

    if(WiFiConectado() == 0)
    {
        LogMessage("[HttpQueryHost] ERROR: WiFi no conectado");
        return (-1);
    }
    
    /* Selección de host y port */
    if(g_http_c_host_conectado == 0)
    {
        /* Si todavía no tuve una conexión Ok o tuve errores */
        
        if(g_wifi_config.u.config.wifi_host1[0] == 0 &&
           g_wifi_config.u.config.wifi_host2[0] == 0  )
        {
            /* Si no hay host definido no puedo seguir */
            return (-1);
        }
        
        if(g_http_c_go_host_conectado == 0)
        {
            /* Decido dónde probar primero */
            g_http_c_go_host_conectado = 1;
        }
        else
        {
            /* Invierto el host para probar */
            g_http_c_go_host_conectado = (g_http_c_go_host_conectado == 1)?2:1;
        }
        
        /* Si el que seleccioné no está configurado elijo el otro */
        if(g_http_c_go_host_conectado == 1 && g_wifi_config.u.config.wifi_host1[0] == 0)
        {
            g_http_c_go_host_conectado = 2;
        }
        else if(g_http_c_go_host_conectado == 2 && g_wifi_config.u.config.wifi_host2[0] == 0)
        {
            g_http_c_go_host_conectado = 1;
        }
    }
    else
    {
        /* Si ya estuve conectado voy por el mismo */
        g_http_c_go_host_conectado = g_http_c_host_conectado;
    }

    /* Tomo host y port de lo que elegí al final */
    if(g_http_c_go_host_conectado == 1)
    {
        host = &g_wifi_config.u.config.wifi_host1[0];
        port = g_wifi_config.u.config.wifi_host1_port;
    }
    else
    {
        host = &g_wifi_config.u.config.wifi_host2[0];
        port = g_wifi_config.u.config.wifi_host2_port;
    }

    /* Si el port está en cero elijo el default según protocolo */
    if(port == 0)
    {
        if(g_wifi_config.u.config.enable_https) port = 443;
        else port = 80;
    }

    /* POST
     * 1.- %s: URI
     * 2.- %s: Host
     * 3.- %d: Content-Length
     * 4.- %s: datos
     */
    sprintf(http_buffer, http_c_http_post, 
            url, host, strlen(msg_send), msg_send);
    
    LogMessage("[HttpQueryHost] Query: [%s] Host: [%s] [%s]", url, host, msg_send);
    
    recv_len = WiFiQueryHost(host, port, http_buffer, http_buffer);
    if(recv_len < 0)
    {
        LogMessage("[HttpQueryHost] ERROR en WiFiQueryHost");
        return (-1);
    }

    head_end = strstr(http_buffer, "\r\n\r\n");
    if(head_end)
    {
        /* Se reconoce una cabecera HTTP */
        head_len = (int)(head_end - (&http_buffer[0]));
        http_buffer[head_len] = 0;
        http_rc = HTTPHeadResponse(http_buffer);
        if(resp_header)
        {
            strcpy(resp_header, http_buffer);
        }
        if(resp_data)
        {
            strcpy(resp_data, &http_buffer[head_len+4]);
        }
        LogMessage("[HttpQueryHost] Resp: Head: [%s] Data: [%s]", http_buffer, &http_buffer[head_len+4]);
    }
    else
    {
        /* No se reconoce una cabecera HTTP */
        if(resp_header)
        {
            *resp_header = 0;
        }
        if(resp_data)
        {
            strcpy(resp_data, http_buffer);
        }
        LogMessage("[HttpQueryHost] Resp: [%s]", http_buffer);
    }
    return http_rc;
}

int HttpDownload(const char* filename)
{
    char url[256];
    char rqst[256];
    char* file;

    if( !HttpOnLine()) return (-1);

    /* obtengo el nombre sin path */
    file = (char*)filename;
    while(strchr(file, '/'))
    {
        file = strchr(file, '/') + 1;
    }
    /* Armo la solicitud del archivo */
    strcpy(url, filename);
    url_encode(url, url);
    /*
     * GET
     * 1.- %s: URI
     * 2.- %s: Host
     */
    snprintf(rqst, 250, http_c_http_get, url, HttpCurrentHost());
    LogMessage("[HttpDownload] Descargar archivo [%s]", filename);
    return WiFiQueryHostToFile(HttpCurrentHost(), HttpCurrentPort(), rqst, file);
}


void HTTPCommandSwitch( const char *obj_name )
{
    /*
     newAJAXCommand('/cgi-bin/abmassign.cgi?funcion=switch&Objeto=".$Nombre."'
     */
    char url[256];
    char rqst[1024];

    if( !HttpOnLine()) return;

    snprintf(url, 250, "%s/abmassign.cgi?funcion=switch&Objeto=%s", g_wifi_config.u.config.rqst_path, obj_name);
    url_encode(url, url);
    /*
     * GET
     * 1.- %s: URI
     * 2.- %s: Host
     */
    snprintf(rqst, 1000, http_c_http_get, url, HttpCurrentHost());
    WiFiQueryHost(HttpCurrentHost(), HttpCurrentPort(), rqst, NULL);
}

void HTTPCommandPulse( const char *obj_name )
{
    /*
     newAJAXCommand('/cgi-bin/abmassign.cgi?funcion=switch&Objeto=".$Nombre."'
     */
    char url[256];
    char rqst[1024];

    if( !HttpOnLine()) return;

    snprintf(url, 250, "%s/abmassign.cgi?funcion=pulse&Objeto=%s", g_wifi_config.u.config.rqst_path, obj_name);
    url_encode(url, url);
    /*
     * GET
     * 1.- %s: URI
     * 2.- %s: Host
     */
    snprintf(rqst, 1000, http_c_http_get, url, HttpCurrentHost());
    WiFiQueryHost(HttpCurrentHost(), HttpCurrentPort(), rqst, NULL);
}

int HttpProcessRequest(const char* request, const char* uri, const char* host,
                       unsigned int content_length, char* data_io, unsigned int max_io)
{
#ifdef DEBUG_HTTP
    LogMessage("[HttpProcessRequest] %s %s de %s data: %i bytes", request, uri, host, content_length);
#endif

    if( !strcmp(request, "GET"))
    {
        if( !strcmp(uri, "/") || !strcmp(uri, "/index.htm") )
        {
            return HttpReturnWiFiFormConfig(data_io, max_io);
        }
    }
    else if( !strcmp(request, "POST"))
    {
        HttpDecodeFormData(data_io, data_io, max_io);
#ifdef DEBUG_HTTP
        LogMessage("[HttpProcessRequest] POST: %i bytes [%s]", content_length, data_io);
#endif
        if( !strcmp(uri, "/") || !strcmp(uri, "/index.htm") )
        {
            /* Capturo datos del formulario */
            ProcessGeneralConfig(data_io);
            return HttpReturnWiFiFormConfig(data_io, 2048);
        }
    }
    strcpy(data_io, http_c_http_404);
    return (strlen(data_io));
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
int HttpReturnWiFiFormConfig(char *out, int max_len)
{
    char wifi_host1[64];
    char wifi_host2[64];
    
    if(g_wifi_config.u.config.wifi_host1[0] != 0)
    {
        if(g_wifi_config.u.config.wifi_host1_port > 0)
        {
            sprintf(wifi_host1, "%s:%i", g_wifi_config.u.config.wifi_host1, g_wifi_config.u.config.wifi_host1_port);
        }
        else
        {
            strcpy(wifi_host1, g_wifi_config.u.config.wifi_host1);
        }
    }
    else
    {
        wifi_host1[0] = 0;
    }

    if(g_wifi_config.u.config.wifi_host2[0] != 0)
    {
        if(g_wifi_config.u.config.wifi_host2_port > 0)
        {
            sprintf(wifi_host2, "%s:%i", g_wifi_config.u.config.wifi_host2, g_wifi_config.u.config.wifi_host2_port);
        }
        else
        {
            strcpy(wifi_host2, g_wifi_config.u.config.wifi_host2);
        }
    }
    else
    {
        wifi_host2[0] = 0;
    }
    
    snprintf(out, max_len, 
            "%s"
            "<html>\n"
            "<head>\n"
            "<title>Dom32IO</title>\n"
            "</head>\n"
            "<style>\n"
            "body{margin:0;padding:0;background:#e0e0e0;font-size:18px;}\n"
            "button{width:100%%;height:55px;font-size:20px;margin-top:20px;background:#4CAF50;color:white;border:none;border-radius:8px;}\n"
            "input{width:48%%;height:45px;font-size:18px;padding:5px;margin-top:5px;box-sizing:border-box;border-radius:6px;border:1px solid #888;}\n"
            "h1{margin:0 0 10px 0;font-size:22px;text-align:center;}\n"
            "label{display:block;margin-top:10px;font-weight:bold;}\n"
            "</style>\n"
            "<body>\n"
            "<form method=\"post\">\n"
            "<h1>Dom32IO</h1>\n"
            "<label>MAC</label>\n"
            "<p>%s</p>\n"
            "<hr />\n"
            "<label>AP1</label>\n"
            "<input type=\"text\" name=\"ap1\" value=\"%s\"> <input type=\"text\" name=\"ap1p\" value=\"%s\"><br>\n"
            "<label>AP2</label>\n"
            "<input type=\"text\" name=\"ap2\" value=\"%s\"> <input type=\"text\" name=\"ap2p\" value=\"%s\"><br>\n"
            "<label>Central</label>\n"
            "<input type=\"text\" name=\"ce1\" value=\"%s\"> <input type=\"text\" name=\"ce2\" value=\"%s\"><br>\n"
            "<label>Path</label>\n"
            "<input type=\"text\" name=\"path\" value=\"%s\">\n"


            "<label>Report</label>\n"
            "<input type=\"text\" name=\"rep\" value=\"%d\">\n"
            "<hr />\n"
            "<button type=\"submit\">Guardar</button>"
            "</form>\n"
            "</body>\n"
            "</html>\n",
            http_c_http_200,
            WiFiMAC(),
            g_wifi_config.u.config.wifi_ap1, g_wifi_config.u.config.wifi_ap1_pass,
            g_wifi_config.u.config.wifi_ap2, g_wifi_config.u.config.wifi_ap2_pass,
            wifi_host1, wifi_host2,
            g_wifi_config.u.config.rqst_path,

            g_wifi_config.u.config.report/100);
    
    return strlen(out);
}
