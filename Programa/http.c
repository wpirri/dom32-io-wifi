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

#include "control.h"
#include "wifi.h"
#include "config.h"

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

char http_rqst[1024];
char http_rqst_data[256];
char http_rqst_uri[64];

typedef struct
{
    char type;
    char timeout;
    char rqst[16];
#ifdef INCLUDE_IO_FORMS    
    char data[2048];
#else
    char data[1024];
#endif
} http_data;

http_data htdat[WIFI_CIP_MAX_CONS+1];


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

void HttpDecodeFormData(char* out, const char* in, unsigned int max_out)
{
    if(!in || !out) return;
    
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
        max_out--;
        if(max_out == 0) break;
    }
    *out = 0;
}

int HttpGetRequest(const char *buffer, char *request)
{
	int rc = 0;
    
    LogMessage("HttpGetRequest\r\n");
    
	while(*buffer && *buffer != '/') buffer++;
	if( !(*buffer)) return 0;
	while(*buffer && *buffer != ' ')
	{
		*request = *buffer;
		buffer++;
		request++;
		rc++;
	}
	*request = 0;
	return rc;
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
void HttpReturnWiFiFormConfig(char *out)
{
    sprintf(out,
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

void HttpReturnWiFiConfig(char *out)
{
    sprintf(out,
            "%sID=%s&ap1=%s&ap1p=%s&ap2=%s&ap2p=%s&ce1=%s&ce1p=%ui&ce2=%s&ce2p=%ui&rqst=%s",
            http_200,
            g_wifi_info.status.hw_mac,
            g_wifi_info.f.config.wifi_ap1, g_wifi_info.f.config.wifi_ap1_pass,
            g_wifi_info.f.config.wifi_ap2, g_wifi_info.f.config.wifi_ap2_pass,
            g_wifi_info.f.config.wifi_host1, g_wifi_info.f.config.wifi_host1_port,
            g_wifi_info.f.config.wifi_host2, g_wifi_info.f.config.wifi_host2_port,
            g_wifi_info.f.config.rqst_path);
}

void HttpReturnStatus(char *out)
{
    sprintf(out, 
            "%sID=%s&IO1=%s&IO2=%s&IO3=%s&IO4=%s&IO5=%s&IO6=%s&"
            "OUT1=%s&OUT2=%s&OUT3=%s&OUT4=%s",
            http_200,
            g_wifi_info.status.hw_mac,
            (g_control_io.stat1)?"on":"off",
            (g_control_io.stat2)?"on":"off",
            (g_control_io.stat3)?"on":"off",
            (g_control_io.stat4)?"on":"off",
            (g_control_io.stat5)?"on":"off",
            (g_control_io.stat6)?"on":"off",
            (g_control_out.stat1)?"on":"off",
            (g_control_out.stat2)?"on":"off",
            (g_control_out.stat3)?"on":"off",
            (g_control_out.stat4)?"on":"off");
    if(g_wifi_info.f.config.io_config_default)
    {
        strcat(http_rqst_data, "&GETCONF=1");
    }
}

void HttpReturnStatusXML(char *out)
{
    sprintf(out,
            "%s"
            "<?xml version=\"1.0\"?>\r\n"
            "<response>\n"
            "<IO1>%s</IO1>\n"
            "<IO2>%s</IO2>\n"
            "<IO3>%s</IO3>\n"
            "<IO4>%s</IO4>\n"
            "<IO5>%s</IO5>\n"
            "<IO6>%s</IO6>\n"
            "</response>",
            xml_200,
            (g_control_io.stat1)?"on":"off",
            (g_control_io.stat2)?"on":"off",
            (g_control_io.stat3)?"on":"off",
            (g_control_io.stat4)?"on":"off",
            (g_control_io.stat5)?"on":"off",
            (g_control_io.stat6)?"on":"off");
}

#ifdef INCLUDE_IO_FORMS
void HttpReturnIOStatusForm(char *out)
{
    sprintf(out,
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

void HttpReturnIOConfigForm(char *out)
{
    sprintf(out,
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

void HttpReturnConfig(char *out)
{
    sprintf(out, 
            "%sID=%s&IO1=%s&IO2=%s&IO3=%s&IO4=%s&IO5=%s&IO6=%s",
            http_200,
            g_wifi_info.status.hw_mac,
            (g_control_io.dir1)?"in":"out",
            (g_control_io.dir2)?"in":"out",
            (g_control_io.dir3)?"in":"out",
            (g_control_io.dir4)?"in":"out",
            (g_control_io.dir5)?"in":"out",
            (g_control_io.dir6)?"in":"out");
    if(g_wifi_info.f.config.io_config_default)
    {
        strcat(http_rqst_data, "&GETCONF=1");
    }
}

void HttpReturnIOConfig(char *out)
{
    sprintf(out, 
            "%sID=%s&IO1=%s&IO2=%s&IO3=%s&IO4=%s&",
            http_200,
            g_wifi_info.status.hw_mac,
            (g_control_io.dir1)?"in":"out",
            (g_control_io.dir2)?"in":"out",
            (g_control_io.dir3)?"in":"out",
            (g_control_io.dir4)?"in":"out");
    if(g_wifi_info.f.config.io_config_default)
    {
        strcat(http_rqst_data, "&GETCONF=1");
    }
}

void HttpReturnIOConfigXML(char *out)
{
    sprintf(out,
            "%s"
            "<?xml version=\"1.0\"?>\r\n"
            "<response>\n"
            "<IO1>%s</IO1>\n"
            "<IO2>%s</IO2>\n"
            "<IO3>%s</IO3>\n"
            "<IO4>%s</IO4>\n"
            "</response>",
            xml_200,
            (g_control_io.dir1)?"in":"out",
            (g_control_io.dir2)?"in":"out",
            (g_control_io.dir3)?"in":"out",
            (g_control_io.dir4)?"in":"out");
}

void HttpProcessWiFiFormConfig(char* in)
{
    HTTPGetFormData(in, "ap1", g_wifi_info.f.config.wifi_ap1);
    HTTPGetFormData(in, "ap1p", g_wifi_info.f.config.wifi_ap1_pass);
    HTTPGetFormData(in, "ap2", g_wifi_info.f.config.wifi_ap2);
    HTTPGetFormData(in, "ap2p", g_wifi_info.f.config.wifi_ap2_pass);
    HTTPGetFormData(in, "ce1", g_wifi_info.f.config.wifi_host1);
    HTTPGetFormData(in, "ce2", g_wifi_info.f.config.wifi_host2);
    HTTPGetFormData(in, "path", g_wifi_info.f.config.rqst_path);
    g_wifi_info.status.save_config = 50;
}

/* *****************************************************************************
 * Inicializacion, llamar al inicio
 * ************************************************************************** */
void HTTPInit( void )
{
    memset(&htdat, 0, sizeof(htdat));
}

/* *****************************************************************************
 * Tareas de HTTP, llamar desde el loop de procesamiento
 * ************************************************************************** */
void HTTPTask( void )
{
    static int id = 0;
    unsigned int rc;
    
    if(htdat[id].type == 'G')
    {
        LogMessage("GET HTTP: %s %u bytes\r\n", htdat[id].rqst, strlen(htdat[id].data));
        if( !strcmp(htdat[id].rqst, "/"))
        {
            HttpReturnWiFiFormConfig(htdat[id].data);
        }
        else if( !strcmp(htdat[id].rqst, "/status.htm"))
        {
            HttpReturnStatus(htdat[id].data);
        }
        else if( !strcmp(htdat[id].rqst, "/iostatus.htm"))
        {
            HttpReturnStatus(htdat[id].data);
        }
        else if( !strcmp(htdat[id].rqst, "/iostatus.xml"))
        {
            HttpReturnStatusXML(htdat[id].data);
        }
        else if( !strcmp(htdat[id].rqst, "/config.htm"))
        {
            HttpReturnConfig(htdat[id].data);
        }
        else if( !strcmp(htdat[id].rqst, "/ioconfig.htm"))
        {
            HttpReturnIOConfig(htdat[id].data);
        }
        else if( !strcmp(htdat[id].rqst, "/ioconfig.xml"))
        {
            HttpReturnIOConfigXML(htdat[id].data);
        }
#ifdef INCLUDE_IO_FORMS
        else if( !strcmp(htdat[id].rqst, "/iostatus.cgi"))
        {
            HttpReturnIOStatusForm(htdat[id].data);
        }
        else if( !strcmp(htdat[id].rqst, "/ioconfig.cgi"))
        {
            HttpReturnIOConfigForm(htdat[id].data);
        }
#endif /* INCLUDE_IO_FORMS */
        else
        {
            strcpy(htdat[id].data, http_404);
        }
        htdat[id].type = 'R';
        LogMessage("RESP HTTP: %s %u bytes\r\n", htdat[id].rqst, strlen(htdat[id].data));
    }
    else if(htdat[id].type == 'P')
    {
        LogMessage("PUT HTTP: %s %u bytes\r\n", htdat[id].rqst, strlen(htdat[id].data));
        if( !strcmp(htdat[id].rqst, "/"))
        {
            HttpProcessWiFiFormConfig(htdat[id].data);
            HttpReturnWiFiFormConfig(htdat[id].data);
        }
        else if( !strcmp(htdat[id].rqst, "/wifi.cgi"))
        {
            HttpProcessWiFiFormConfig(htdat[id].data);
            HttpReturnWiFiConfig(htdat[id].data);
        }
        else if( !strcmp(htdat[id].rqst, "/iostatus.cgi"))
        {
            CTRLSetIO(htdat[id].data);
#ifdef INCLUDE_IO_FORMS
            HttpReturnIOStatusForm(htdat[id].data);
#else
            HttpReturnStatus(htdat[id].data);
#endif /* INCLUDE_IO_FORMS */
        }
        else if( !strcmp(htdat[id].rqst, "/ioswitch.cgi"))
        {
            CTRLSwitchIO(htdat[id].data);
            HttpReturnStatus(htdat[id].data);
        }
        else if( !strcmp(htdat[id].rqst, "/ioconfig.cgi"))
        {
            CTRLConfIO(htdat[id].data);
#ifdef INCLUDE_IO_FORMS
            HttpReturnIOConfigForm(htdat[id].data);
#else
            HttpReturnIOConfig(htdat[id].data);
#endif /* INCLUDE_IO_FORMS */
        }
        else
        {
            strcpy(htdat[id].data, http_404);
        }
        htdat[id].type = 'R';
        LogMessage("RESP HTTP: %s %u bytes\r\n", htdat[id].rqst, strlen(htdat[id].data));
    }
    else if(htdat[id].type == 'H')
    {
        LogMessage("HTTP RESPONSE: %u bytes\r\n", strlen(htdat[id].data));
        /* Se trata de una respuesta HTTP/1... */
        CTRLConfIO(htdat[id].data);
        CTRLSetIO(htdat[id].data);
        htdat[id].type = 0;
    }
    
    id++;
    if(id > (WIFI_CIP_MAX_CONS+1)) id = 0;
}

/* *****************************************************************************
 * 
 * ************************************************************************** */
void HTTPTimer( void )
{
    int i;
    
    for(i = 0; i < (WIFI_CIP_MAX_CONS+1); i++)
    {
        if(htdat[i].timeout && htdat[i].type)
        {
            htdat[i].timeout--;
            if(htdat[i].timeout == 0)
            {
                htdat[i].type = 0;
                LogMessage("HTTPTimer time-out htdat[%i]\r\n", i);
            }
        }
    }
}

/* *****************************************************************************
 * Agrega un mensaje HTTP a la cola para su tratamiento segun su tipo
 * P - POST recibido, hay que procesar
 * G - GET recibido, hay que procesar
 * ************************************************************************** */
int HTTPAdd(int id, const char* buffer)
{
    char *p;
    
    LogMessage("HTTPAdd(%i)\r\n", id);
    
    if(id >= 0 && id < WIFI_CIP_MAX_CONS)
    {
        LogMessage("  mensaje entrante.\r\n");
        /* Mensaje entrante */
        if(htdat[id].type == 0)
        {
            if(*buffer == 'G' || *buffer == 'P')
            {
                LogMessage("    requerimiento.\r\n");
                htdat[id].type = *buffer;
                htdat[id].timeout = 50;

                HttpGetRequest(buffer, htdat[id].rqst);

                if(htdat[id].type == 'P')
                {
                    p = strstr(buffer, "\r\n\r\n");
                    if(p)
                    {
                        p += 4;
                        //strncpy(htdat[id].data, p, sizeof(htdat[id].data));
                        //htdat[id].data[sizeof(htdat[id].data)-1] = 0;
                        HttpDecodeFormData(htdat[id].data, p, sizeof(htdat[id].data));
                    }
                }
                return 1;
            }
            else if(*buffer == 'H')
            {
                LogMessage("    respuesta.\r\n");
                htdat[id].type = *buffer;
                htdat[id].timeout = 50;
                p = strstr(buffer, "\r\n\r\n");
                if(p)
                {
                    p += 4;
                    //strncpy(htdat[id].data, p, sizeof(htdat[id].data));
                    //htdat[id].data[sizeof(htdat[id].data)-1] = 0;
                    HttpDecodeFormData(htdat[id].data, p, sizeof(htdat[id].data));
                }
                return 1;
            }
        }
        else
        {
            LogMessage("HTTPAdd Error. Conexion %i ocupada.\r\n", id);
        }
    }
    else if(id < 0)
    {
        LogMessage("  mensaje saliente.\r\n");
        /* Mensaje Saliente */
        for(id = 0; id < (WIFI_CIP_MAX_CONS+1); id++)
        {
            /* Busco un slot libre */
            if(htdat[id].type == 0)
            {
                htdat[id].timeout = 100;
                htdat[id].type = 'S';
                //strncpy(htdat[id].data, buffer, sizeof(htdat[id].data));
                //htdat[id].data[sizeof(htdat[id].data)-1] = 0;
                HttpDecodeFormData(htdat[id].data, buffer, sizeof(htdat[id].data));
                return 1;
            }
        }
    }
    return 0;
}

/* *****************************************************************************
 * Devuelve el indice de un mensaje pendiente de procesar
 * ************************************************************************** */
int HTTPPending(void)
{
    int i;
    for(i = 0; i < (WIFI_CIP_MAX_CONS+1); i++)
    {
        if(htdat[i].type == 'R' || htdat[i].type == 'S') break;
    }
    return ((i < (WIFI_CIP_MAX_CONS+1))?i:(-1));
}

/* *****************************************************************************
 * Devuelte el tipo de mensaje encolado en el indice
 * ************************************************************************** */
char HTTPGetType(int id)
{
    if(id >= 0 && id < (WIFI_CIP_MAX_CONS+1))
    {
        return htdat[id].type;
    }
    return 0;
}

/* *****************************************************************************
 * Saca un mensaje de la cola para procesar
 * ************************************************************************** */
const char* HTTPGet(int id, char* buffer)
{
    if(id >= 0 && id < (WIFI_CIP_MAX_CONS+1))
    {
        htdat[id].type = 0;
        if(buffer)
        {
            strcpy(buffer, htdat[id].data);
            return buffer;
        }
        return (const char*)&htdat[id].data[0];
    }
    return NULL;
}

/* *****************************************************************************
 * Genera un mensaje WEB con formato de formulario POST y lo encola para enviar
 * ************************************************************************** */
int HTTPNotificarStatus(void)
{
    LogMessage("HTTPNotificarStatus\r\n");

    sprintf(http_rqst_data, 
    "ID=%s&IO1=%s&IO2=%s&IO3=%s&IO4=%s&IO5=%s&IO6=%s&"
    "OUT1=%s&OUT2=%s&OUT3=%s&OUT4=%s&"
    "CHG=%c%c%c%c%c%c00%c%c%c%c0000",
    g_wifi_info.status.hw_mac,
    (g_control_io.stat1)?"on":"off",
    (g_control_io.stat2)?"on":"off",
    (g_control_io.stat3)?"on":"off",
    (g_control_io.stat4)?"on":"off",
    (g_control_io.stat5)?"on":"off",
    (g_control_io.stat6)?"on":"off",
    (g_control_out.stat1)?"on":"off",
    (g_control_out.stat2)?"on":"off",
    (g_control_out.stat3)?"on":"off",
    (g_control_out.stat4)?"on":"off",
    (g_control_io.change1)?'1':'0',
    (g_control_io.change2)?'1':'0',
    (g_control_io.change3)?'1':'0',
    (g_control_io.change4)?'1':'0',
    (g_control_io.change5)?'1':'0',
    (g_control_io.change6)?'1':'0',
    (g_control_out.change1)?'1':'0',
    (g_control_out.change2)?'1':'0',
    (g_control_out.change3)?'1':'0',
    (g_control_out.change4)?'1':'0');
    if(g_wifi_info.f.config.io_config_default)
    {
        strcat(http_rqst_data, "&GETCONF=1");
    }
    sprintf(http_rqst_uri, "%s/infoio.cgi", 
            g_wifi_info.f.config.rqst_path);
/* POST
 * 1.- %s: URI
 * 2.- %s: Host
 * 3.- %d: Content-Length
 * 4.- %s: datos
 */
    sprintf(http_rqst, http_post,
            http_rqst_uri,
            g_wifi_info.f.config.wifi_host1,
            strlen(http_rqst_data),
            http_rqst_data);
    if(HTTPAdd((-1), http_rqst) == 0)
    {
        /* No fue posible enviar */
        return 0;
    }
    
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

int HTTPNotificarTarjeta( const char *crd)
{
    LogMessage("HTTPNotificarTarjeta\r\n");

    sprintf(http_rqst_data, "ID=%s&CARD=%s", g_wifi_info.status.hw_mac, crd);
    sprintf(http_rqst_uri, "%s/infoio.cgi", g_wifi_info.f.config.rqst_path);
/* POST
 * 1.- %s: URI
 * 2.- %s: Host
 * 3.- %d: Content-Length
 * 4.- %s: datos
 */
    sprintf(http_rqst, http_post,
            http_rqst_uri,
            g_wifi_info.f.config.wifi_host1,
            strlen(http_rqst_data),
            http_rqst_data);
    if(HTTPAdd((-1), http_rqst) == 0)
    {
        /* No fue posible enviar */
        return 0;
    }
    return strlen(http_rqst);
}