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

#include "main.h"
#include "control.h"

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
                            "Accept: text/html,text/xml\r\n\r\n%s";

char http_rqst[1024];
char http_rqst_data[256];
char http_rqst_uri[64];

typedef struct
{
    char type;
    char timeout;
    char rqst[16];
    char data[1024];
} http_data;

http_data htdat[WIFI_CIP_MAX_CONS];

int HttpGetRequest(const char *buffer, char *request)
{
	int rc = 0;
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
void HttpReturnFormConfig(char *out)
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
            "<h1>WiFi</h1>"
            "<hr />"
            "<h2>AP1:</h2>\n"
            "&nbsp;<input id=\"it\" type=\"text\" name=\"ap1\" value=\"%s\">&nbsp;<input  id=\"it\" type=\"text\" name=\"ap1p\" value=\"%s\"><br>\n"
            "<h2>AP2:</h2>\n"
            "&nbsp;<input id=\"it\" type=\"text\" name=\"ap2\" value=\"%s\">&nbsp;<input  id=\"it\" type=\"text\" name=\"ap2p\" value=\"%s\"><br>\n"
            "<h2>Central:</h2>\n"
            "&nbsp;<input id=\"it\" type=\"text\" name=\"ce1\" value=\"%s\">&nbsp;<input  id=\"it\" type=\"text\" name=\"ce2\" value=\"%s\"><br>\n"
            "<br>\n"
            "<br>\n"
            "&nbsp;&nbsp;<input  id=\"it\" type=\"submit\" value=\"Guardar\">"
            "</form>\n"
            "</body>\n"
            "</html>\n",
            http_200,
            g_wifi_info.f.config.wifi_ap1, g_wifi_info.f.config.wifi_ap1_pass,
            g_wifi_info.f.config.wifi_ap2, g_wifi_info.f.config.wifi_ap2_pass,
            g_wifi_info.f.config.wifi_host1, g_wifi_info.f.config.wifi_host2);
}

void HttpReturnIOStatus(char *out)
{
    sprintf(out,
            "%s"
            "<?xml version=\"1.0\"?>\r\n"
            "<IO1>%d</IO1>\n"
            "<IO2>%d</IO2>\n"
            "<IO3>%d</IO3>\n"
            "<IO4>%d</IO4>\n",
            xml_200,
            (g_control_io.stat1)?1:0,
            (g_control_io.stat2)?1:0,
            (g_control_io.stat3)?1:0,
            (g_control_io.stat4)?1:0);
}

void HttpReturnIOConfig(char *out)
{
    sprintf(out,
            "%s"
            "<?xml version=\"1.0\"?>\r\n"
            "<Dir1>%s</Dir1>\n"
            "<Dir2>%s</Dir2>\n"
            "<Dir3>%s</Dir3>\n"
            "<Dir4>%s</Dir4>\n",
            xml_200,
            (g_control_io.dir1)?"in":"out",
            (g_control_io.dir2)?"in":"out",
            (g_control_io.dir3)?"in":"out",
            (g_control_io.dir4)?"in":"out");
}

void HttpProcessFormConfig(char* in, char *out)
{
    HTTPGetFormData(in, "ap1", g_wifi_info.f.config.wifi_ap1);
    HTTPGetFormData(in, "ap1p", g_wifi_info.f.config.wifi_ap1_pass);
    HTTPGetFormData(in, "ap2", g_wifi_info.f.config.wifi_ap2);
    HTTPGetFormData(in, "ap2p", g_wifi_info.f.config.wifi_ap2_pass);
    HTTPGetFormData(in, "ce1", g_wifi_info.f.config.wifi_host1);
    HTTPGetFormData(in, "ce2", g_wifi_info.f.config.wifi_host2);
    
    HttpReturnFormConfig(out);
    
    g_wifi_info.status.save_config = 50;
}

/* *****************************************************************************
 * 
 * ************************************************************************** */
void HTTPInit( void )
{
    memset(&htdat, 0, sizeof(htdat));
}

/* *****************************************************************************
 * 
 * ************************************************************************** */
void HTTPTask( void )
{
    static int id = 0;
    unsigned int rc;
    
    if(htdat[id].type == 'G')
    {
        if( !strcmp(htdat[id].rqst, "/"))
        {
            HttpReturnFormConfig(htdat[id].data);
        }
        else if( !strcmp(htdat[id].rqst, "/getio.cgi"))
        {
            HttpReturnIOStatus(htdat[id].data);
        }
        else
        {
            strcpy(htdat[id].data, http_404);
        }
        htdat[id].type = 'R';
    }
    else if(htdat[id].type == 'P')
    {
        if( !strcmp(htdat[id].rqst, "/"))
        {
            HttpProcessFormConfig(htdat[id].data, htdat[id].data);
        }
        else if( !strcmp(htdat[id].rqst, "/setio.cgi"))
        {
            CTRLSetIO(htdat[id].data);
            HttpReturnIOStatus(htdat[id].data);
        }
        else if( !strcmp(htdat[id].rqst, "/confio.cgi"))
        {
            CTRLConfIO(htdat[id].data);
            HttpReturnIOConfig(htdat[id].data);
        }
        else
        {
            strcpy(htdat[id].data, http_404);
        }
        htdat[id].type = 'R';
    }
    
    id++;
    if(id > WIFI_CIP_MAX_CONS) id = 0;
}

/* *****************************************************************************
 * 
 * ************************************************************************** */
void HTTPTimer( void )
{
    int i;
    
    for(i = 0; i < WIFI_CIP_MAX_CONS; i++)
    {
        if(htdat[i].timeout)
        {
            htdat[i].timeout--;
            if(htdat[i].timeout == 0)
            {
                htdat[i].type = 0;
            }
        }
    }
}

/* *****************************************************************************
 * 
 * ************************************************************************** */
void HTTPAdd(int id, const char* buffer)
{
    char *p;
    if(id >= 0 && id < WIFI_CIP_MAX_CONS)
    {
        /* Mensaje entrante */
        if(htdat[id].type == 0 && (*buffer == 'G' || *buffer == 'P'))
        {
            htdat[id].type = *buffer;
            htdat[id].timeout = 50;

            HttpGetRequest(buffer, htdat[id].rqst);

            if(htdat[id].type == 'P')
            {
                p = strstr(buffer, "\r\n\r\n");
                if(p)
                {
                    p += 4;
                    strncpy(htdat[id].data, p, sizeof(htdat[id].data));
                    htdat[id].data[sizeof(htdat[id].data)-1] = 0;
                }
            }
        }
    }
    else if(id < 0)
    {
        /* Mensaje Saliente */
        for(id = 0; id < WIFI_CIP_MAX_CONS; id++)
        {
            /* Busco un slot libre */
            if(htdat[id].type == 0)
            {
                htdat[id].timeout = 100;
                htdat[id].type = 'S';
                strncpy(htdat[id].data, buffer, sizeof(htdat[id].data));
                htdat[id].data[sizeof(htdat[id].data)-1] = 0;
                return;
            }
        }
    }
}

/* *****************************************************************************
 * 
 * ************************************************************************** */
int HTTPPending(void)
{
    int i;
    for(i = 0; i < WIFI_CIP_MAX_CONS; i++)
    {
        if(htdat[i].type == 'R' || htdat[i].type == 'S') break;
    }
    return ((i < WIFI_CIP_MAX_CONS)?i:(-1));
}

/* *****************************************************************************
 * 
 * ************************************************************************** */
char HTTPGetType(int id)
{
    if(id >= 0 && id < WIFI_CIP_MAX_CONS)
    {
        return htdat[id].type;
    }
    return 0;
}

/* *****************************************************************************
 * 
 * ************************************************************************** */
const char* HTTPGet(int id, char* buffer)
{
    if(id >= 0 && id < WIFI_CIP_MAX_CONS)
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

void HTTPNotificarStatus(void)
{
    sprintf(http_rqst_data, 
            "IO1=%s&IO2=%s&IO3=%s&IO4=%s",
            (g_control_io.stat1)?"on":"off",
            (g_control_io.stat2)?"on":"off",
            (g_control_io.stat3)?"on":"off",
            (g_control_io.stat4)?"on":"off");
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
    HTTPAdd((-1), http_rqst);
}
