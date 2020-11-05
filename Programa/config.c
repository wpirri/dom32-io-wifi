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
#define _IN_CONFIG_C_
#include "config.h"
#undef _IN_CONFIG_C_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void ConfigInit( void )
{
    memset(&g_wifi_info, 0, sizeof(wifi_info));
}

void ConfigLoad( void )
{
    strcpy(g_wifi_info.config.wifi_ap1, "872e-b");
    strcpy(g_wifi_info.config.wifi_ap1_pass, "00464bab872e");
    strcpy(g_wifi_info.config.wifi_ap2, "872e-a");
    strcpy(g_wifi_info.config.wifi_ap2_pass, "00464bab872e");
}

void ConfigSave(void)
{
    
}

