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
#ifndef _WIFI_H_
#define _WIFI_H_

#include <stdbool.h>

#define WIFI_READ_LINE_BUFFER_LEN 2048

#ifdef _IN_WIFI_C_
    char *g_wifi_host;
    unsigned int g_wifi_port;
    bool g_end_download;
#else
    extern char *g_wifi_host;
    extern unsigned int g_wifi_port;
    extern bool g_end_download;
#endif

void WiFiInit( void );
void WiFiTask( void );
void WiFiTimer( void );

int WifiCheckUnsol( void );
void WifiSwitchHost( void );

#endif /* _WIFI_H_ */
