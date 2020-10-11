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

#include "wifi_at.h"
#include "mcc_generated_files/uart1.h"
#include "queue.h"

#define MAX_WIFI_AT_LEN 255

typedef enum
{
    WSTM_INIT = 0,
    WSTM_RESET,
    WSTM_RESET_RELEASE_W,
    WSTM_RESET_RELEASE,
    WSTM_RESET_READY_W,
    WSTM_RESET_READY,

            
            
            
} WSTM;

typedef struct _wifi_at_cmd
{
    char cmd[MAX_WIFI_AT_LEN+1];
    char wait_for[17];
    int on_ok;
    int on_error;
    int time_out;
} wifi_at_cmd;

unsigned int g_wifi_at_cmd_queue;

void WiFiAtInit( void )
{
    g_wifi_at_cmd_queue = QueueOpen(10, sizeof(wifi_at_cmd));
    
}

void WiFiAtTasks( void )
{
    
}

void WiFiAtTimer( void )
{
    
}
