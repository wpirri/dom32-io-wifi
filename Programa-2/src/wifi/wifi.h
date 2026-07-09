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
#ifndef _WIFI_H_
#define _WIFI_H_

#define WIFI_MODE_NONE      0
#define WIFI_MODE_ST        1
#define WIFI_MODE_AP        2
#define WIFI_MODE_AP_ST     3

#define WIFI_STATUS_NO_CONECTADO    0
#define WIFI_STATUS_CONECTADO_AP1   1
#define WIFI_STATUS_CONECTADO_AP2   2

#define WIFI_HOST_OFFLINE           0
#define WIFI_HOST_ONLINE1           1
#define WIFI_HOST_ONLINE2           2

#define WIFI_BUFFER_LEN 4096

void WiFiInit( void );
int WiFiStart( void );
void WiFiTimer( void );
void WiFiTask( void );

int WiFiMode( void );
int WiFiConectado( void );
int WiFiStaConectado( void );
void WiFiSwitchRouter( void );
int WiFiSetMode(int mode);
void WiFiCheckUnsol( void );
const char* WiFiMAC( void );

int WiFiQueryHost(const char* host, int port, const char* send_msg, char* recv_msg);
int WiFiQueryHostToFile(const char* host, int port, const char* send_msg, const char* filename);

#endif /* _WIFI_H_ */
