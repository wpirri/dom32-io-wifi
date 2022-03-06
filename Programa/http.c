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
#include "http.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>

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

#ifdef __COMMENT__
void HttpReturnStatus(char *out)
{
    sprintf(out, 
            "%sID=%s&IO1=%s&IO2=%s&IO3=%s&IO4=%s&"
            "EXP1=%s&EXP3=%s&EXP4=%s&EXP5=%s&EXP7=%s&EXP8=%s&EXP9=%s",
            http_200,
            g_wifi_info.status.hw_mac,
            (g_control_io.stat1)?"on":"off",
            (g_control_io.stat2)?"on":"off",
            (g_control_io.stat3)?"on":"off",
            (g_control_io.stat4)?"on":"off",
            (g_control_exp.stat1)?"on":"off",
            (g_control_exp.stat3)?"on":"off",
            (g_control_exp.stat4)?"on":"off",
            (g_control_exp.stat5)?"on":"off",
            (g_control_exp.stat7)?"on":"off",
            (g_control_exp.stat8)?"on":"off",
            (g_control_exp.stat9)?"on":"off");
    if(g_wifi_info.f.config.io_config_default)
    {
        strcat(http_rqst_data, "&GETCONF=1");
    }
}

#endif /* __COMMENT__ */
#ifdef __COMMENT__

void HttpReturnIOStatusXML(char *out)
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
            (g_control_io.stat1)?"on":"off",
            (g_control_io.stat2)?"on":"off",
            (g_control_io.stat3)?"on":"off",
            (g_control_io.stat4)?"on":"off");
}

#endif /* __COMMENT__ */
#ifdef __COMMENT__

void HttpReturnExpStatusXML(char *out)
{
    sprintf(out,
            "%s"
            "<?xml version=\"1.0\"?>\r\n"
            "<response>\n"
            "<EXP1>%s</EXP1>\n"
            "<EXP3>%s</EXP3>\n"
            "<EXP4>%s</EXP4>\n"
            "<EXP5>%s</EXP5>\n"
            "<EXP7>%s</EXP7>\n"
            "<EXP8>%s</EXP8>\n"
            "<EXP9>%s</EXP9>\n"
            "</response>",
            xml_200,
            (g_control_exp.stat1)?"on":"off",
            (g_control_exp.stat3)?"on":"off",
            (g_control_exp.stat4)?"on":"off",
            (g_control_exp.stat5)?"on":"off",
            (g_control_exp.stat7)?"on":"off",
            (g_control_exp.stat8)?"on":"off",
            (g_control_exp.stat9)?"on":"off");
}

#endif /* __COMMENT__ */

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

void HttpReturnExpStatusForm(char *out)
{
    sprintf(out,
            "%s"
            "<html>\n"
            "<head>\n"
            "<title>Dom32IO - Expansion Status</title>\n"
            "</head>\n"
            "<style>\n"
            "body{background-color:grey;font-size:24pt;}\n"
            "#it{height:75;font-size:24pt}\n"
            "</style>\n"
            "<body>\n"
            "<form method=\"POST\">\n"
            "<h1>I/O</h1><hr />\n"
            "EXP 1:&nbsp;"
            "<select id=\"it\" name=\"EXP1\" onchange=\"submit();\"><option value=\"on\"%s>on</option><option value=\"off\"%s>off</option></select>\n"
            "<select id=\"it\" name=\"EXP3\" onchange=\"submit();\"><option value=\"on\"%s>on</option><option value=\"off\"%s>off</option></select>\n"
            "<select id=\"it\" name=\"EXP4\" onchange=\"submit();\"><option value=\"on\"%s>on</option><option value=\"off\"%s>off</option></select>\n"
            "<select id=\"it\" name=\"EXP5\" onchange=\"submit();\"><option value=\"on\"%s>on</option><option value=\"off\"%s>off</option></select>\n"
            "<select id=\"it\" name=\"EXP7\" onchange=\"submit();\"><option value=\"on\"%s>on</option><option value=\"off\"%s>off</option></select>\n"
            "<select id=\"it\" name=\"EXP8\" onchange=\"submit();\"><option value=\"on\"%s>on</option><option value=\"off\"%s>off</option></select>\n"
            "<select id=\"it\" name=\"EXP9\" onchange=\"submit();\"><option value=\"on\"%s>on</option><option value=\"off\"%s>off</option></select>\n"
            "</form>\n"
            "</body>\n"
            "</html>\n",
            http_200,
            (g_control_exp.stat1)?" selected":" ", (g_control_exp.stat1)?" ":"selected",
            (g_control_exp.stat3)?" selected":" ", (g_control_exp.stat3)?" ":"selected",
            (g_control_exp.stat4)?" selected":" ", (g_control_exp.stat4)?" ":"selected",
            (g_control_exp.stat5)?" selected":" ", (g_control_exp.stat5)?" ":"selected",
            (g_control_exp.stat7)?" selected":" ", (g_control_exp.stat7)?" ":"selected",
            (g_control_exp.stat8)?" selected":" ", (g_control_exp.stat8)?" ":"selected",
            (g_control_exp.stat9)?" selected":" ", (g_control_exp.stat9)?" ":"selected");
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

void HttpReturnExpConfigForm(char *out)
{
    sprintf(out,
            "%s"
            "<html>\n"
            "<head>\n"
            "<title>Dom32IO - Expansion Config</title>\n"
            "</head>\n"
            "<style>\n"
            "body{background-color:grey;font-size:24pt;}\n"
            "#it{height:75;font-size:24pt}\n"
            "</style>\n"
            "<body>\n"
            "<form method=\"POST\">\n"
            "<h1>I/O</h1><hr />\n"
            "EXP 1:&nbsp;"
            "<select id=\"it\" name=\"EXP1\" onchange=\"submit();\"><option value=\"in\"%s>in</option><option value=\"out\"%s>out</option></select>\n"
            "<select id=\"it\" name=\"EXP3\" onchange=\"submit();\"><option value=\"in\"%s>in</option><option value=\"out\"%s>out</option></select>\n"
            "<select id=\"it\" name=\"EXP4\" onchange=\"submit();\"><option value=\"in\"%s>in</option><option value=\"out\"%s>out</option></select>\n"
            "<select id=\"it\" name=\"EXP5\" onchange=\"submit();\"><option value=\"in\"%s>in</option><option value=\"out\"%s>out</option></select>\n"
            "<select id=\"it\" name=\"EXP7\" onchange=\"submit();\"><option value=\"in\"%s>in</option><option value=\"out\"%s>out</option></select>\n"
            "<select id=\"it\" name=\"EXP8\" onchange=\"submit();\"><option value=\"in\"%s>in</option><option value=\"out\"%s>out</option></select>\n"
            "<select id=\"it\" name=\"EXP9\" onchange=\"submit();\"><option value=\"in\"%s>in</option><option value=\"out\"%s>out</option></select>\n"
            "</form>\n"
            "</body>\n"
            "</html>\n",
            http_200,
            (g_control_exp.dir1)?" selected":" ", (g_control_exp.dir1)?" ":"selected",
            (g_control_exp.dir3)?" selected":" ", (g_control_exp.dir3)?" ":"selected",
            (g_control_exp.dir4)?" selected":" ", (g_control_exp.dir4)?" ":"selected",
            (g_control_exp.dir5)?" selected":" ", (g_control_exp.dir5)?" ":"selected",
            (g_control_exp.dir7)?" selected":" ", (g_control_exp.dir7)?" ":"selected",
            (g_control_exp.dir8)?" selected":" ", (g_control_exp.dir8)?" ":"selected",
            (g_control_exp.dir9)?" selected":" ", (g_control_exp.dir9)?" ":"selected");
}
#endif /* INCLUDE_IO_FORMS */

#ifdef __COMMENT__
void HttpReturnConfig(char *out)
{
    sprintf(out, 
            "%sID=%s&IO1=%s&IO2=%s&IO3=%s&IO4=%s&"
            "EXP1=%s&EXP3=%s&EXP4=%s&EXP5=%s&EXP7=%s&EXP8=%s&EXP9=%s",
            http_200,
            g_wifi_info.status.hw_mac,
            (g_control_io.dir1)?"in":"out",
            (g_control_io.dir2)?"in":"out",
            (g_control_io.dir3)?"in":"out",
            (g_control_io.dir4)?"in":"out",
            (g_control_exp.dir1)?"in":"out",
            (g_control_exp.dir3)?"in":"out",
            (g_control_exp.dir4)?"in":"out",
            (g_control_exp.dir5)?"in":"out",
            (g_control_exp.dir7)?"in":"out",
            (g_control_exp.dir8)?"in":"out",
            (g_control_exp.dir9)?"in":"out");
    if(g_wifi_info.f.config.io_config_default)
    {
        strcat(http_rqst_data, "&GETCONF=1");
    }
}
#endif /* __COMMENT__ */
#ifdef __COMMENT__
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
#endif /* __COMMENT__ */
#ifdef __COMMENT__
void HttpReturnExpConfig(char *out)
{
    sprintf(out, 
            "%sID=%s&EXP1=%s&EXP3=%s&EXP4=%s&EXP5=%s&EXP7=%s&EXP8=%s&EXP9=%s",
            http_200,
            g_wifi_info.status.hw_mac,
            (g_control_exp.dir1)?"in":"out",
            (g_control_exp.dir3)?"in":"out",
            (g_control_exp.dir4)?"in":"out",
            (g_control_exp.dir5)?"in":"out",
            (g_control_exp.dir7)?"in":"out",
            (g_control_exp.dir8)?"in":"out",
            (g_control_exp.dir9)?"in":"out");
    if(g_wifi_info.f.config.io_config_default)
    {
        strcat(http_rqst_data, "&GETCONF=1");
    }
}
#endif /* __COMMENT__ */
#ifdef __COMMENT__
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
#endif /* __COMMENT__ */
#ifdef __COMMENT__
void HttpReturnExpConfigXML(char *out)
{
    sprintf(out,
            "%s"
            "<?xml version=\"1.0\"?>\r\n"
            "<response>\n"
            "<EXP1>%s</EXP1>\n"
            "<EXP3>%s</EXP3>\n"
            "<EXP4>%s</EXP4>\n"
            "<EXP5>%s</EXP5>\n"
            "<EXP7>%s</EXP7>\n"
            "<EXP8>%s</EXP8>\n"
            "<EXP9>%s</EXP9>\n"
            "</response>",
            xml_200,
            (g_control_exp.dir1)?"in":"out",
            (g_control_exp.dir3)?"in":"out",
            (g_control_exp.dir4)?"in":"out",
            (g_control_exp.dir5)?"in":"out",
            (g_control_exp.dir7)?"in":"out",
            (g_control_exp.dir8)?"in":"out",
            (g_control_exp.dir9)?"in":"out");
}
#endif /* __COMMENT__ */
#ifdef __COMMENT__
void HttpProcessWiFiFormConfig(char* in)
{
    HTTPGetFormData(in, "ap1", g_wifi_info.f.config.wifi_ap1);
    HTTPGetFormData(in, "ap1p", g_wifi_info.f.config.wifi_ap1_pass);
    HTTPGetFormData(in, "ap2", g_wifi_info.f.config.wifi_ap2);
    HTTPGetFormData(in, "ap2p", g_wifi_info.f.config.wifi_ap2_pass);
    HTTPGetFormData(in, "ce1", g_wifi_info.f.config.wifi_host1);
    HTTPGetFormData(in, "ce2", g_wifi_info.f.config.wifi_host2);
    g_wifi_info.status.save_config = 50;
}
#endif /* __COMMENT__ */


void HTTPInit( void )
{
    
}

void HTTPTask( void )
{
    
}

void HTTPTimer( void )
{
    
}

void HTTPMessage(int socket, char method, const char* url, const char* body, unsigned int body_len)
{
    printf("HTTP: socket: %d url: %s body: [%.80s]/%d", socket, url, body, body_len);

    if( !strcmp(url, "/") || !strcmp(url, "/index.htm"))
    {
        //HttpReturnWiFiFormConfig();
    }
    
}
