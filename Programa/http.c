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
    char rqst[16];
    char data[64];
} http_data;

http_data htdat[WIFI_CIP_MAX_CONS];

void HTTPInit( void )
{
    memset(&htdat, 0, sizeof(htdat));
}

void HTTPTask( void )
{
    static int id = 0;
    
    if(htdat[id].type == 'G')
    {
        strcpy(htdat[id].data, http_404);
        htdat[id].type = 'R';
        
        
    }
    else if(htdat[id].type == 'P')
    {
        strcpy(htdat[id].data, http_404);
        htdat[id].type = 'R';
        
    }
    
    id++;
    if(id > WIFI_CIP_MAX_CONS) id = 0;
}

void HTTPTimer( void )
{
    
}

void HTTPAdd(int id, const char* buffer)
{
    if(htdat[id].type == 0)
    {
        htdat[id].type = *buffer;
        
        
        
    }
}

int HTTPPending(void)
{
    int i;
    for(i = 0; i < WIFI_CIP_MAX_CONS; i++)
    {
        if(htdat[i].type == 'R') break;
    }
    return ((i < WIFI_CIP_MAX_CONS)?i:(-1));
}

const char* HTTPGet(int id, char* buffer)
{
    if(id >= 0 && id < WIFI_CIP_MAX_CONS)
    {
        if(buffer)
        {
            strcpy(buffer, htdat[id].data);
        }
        htdat[id].type = 0;
        return (const char*)&htdat[id].data[0];
    }
}
