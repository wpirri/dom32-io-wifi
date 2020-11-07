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

const char http_200[] = "HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Type: text/html\r\nCache-Control: no-cache\r\n\r\n";
const char http_404[] = "HTTP/1.1 404 Not found\r\nConnection: close\r\n\r\n404: File not found\r\n";

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

int HttpGetFormData(char *buffer, const char *label, char *value)
{
    char *p;
    
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
                }
                *value = 0;
            }
        }
    } while(p);
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
            "<br>\n"
            "<br>\n"
            "&nbsp;&nbsp;<input  id=\"it\" type=\"submit\" value=\"Guardar\">"
            "</form>\n"
            "</body>\n"
            "</html>\n",
            http_200,
            g_wifi_info.config.wifi_ap1, g_wifi_info.config.wifi_ap1_pass,
            g_wifi_info.config.wifi_ap2, g_wifi_info.config.wifi_ap2_pass);
}

void HttpProcessFormConfig(char* in, char *out)
{
    HttpGetFormData(in, "ap1", g_wifi_info.config.wifi_ap1);
    HttpGetFormData(in, "ap1p", g_wifi_info.config.wifi_ap1_pass);
    HttpGetFormData(in, "ap2", g_wifi_info.config.wifi_ap2);
    HttpGetFormData(in, "ap2p", g_wifi_info.config.wifi_ap2_pass);
    
    HttpReturnFormConfig(out);
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
    
    if(htdat[id].type == 'G')
    {
        if( !strcmp(htdat[id].rqst, "/"))
        {
            HttpReturnFormConfig(htdat[id].data);
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

/* *****************************************************************************
 * 
 * ************************************************************************** */
int HTTPPending(void)
{
    int i;
    for(i = 0; i < WIFI_CIP_MAX_CONS; i++)
    {
        if(htdat[i].type == 'R') break;
    }
    return ((i < WIFI_CIP_MAX_CONS)?i:(-1));
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
