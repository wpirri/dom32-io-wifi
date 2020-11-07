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
#include <stdint.h>

#include "main.h"

#define CONFIG_FLASK_OK 0xA5A55A5A

/* Have compiler allocate a page of flash from the NVM.  Aligned to a page. */
static const uint32_t __attribute((space(prog),aligned(FLASH_ERASE_PAGE_SIZE_IN_PC_UNITS))) flash_reserved_space[FLASH_ERASE_PAGE_SIZE_IN_PC_UNITS/4];

void ConfigLoadDefault( void)
{
    g_wifi_info.f.config.flash_ok = CONFIG_FLASK_OK;
    strcpy(g_wifi_info.f.config.wifi_ap1, "WiFiAp1");
    strcpy(g_wifi_info.f.config.wifi_ap1_pass, "Password1");
    strcpy(g_wifi_info.f.config.wifi_ap2,"WiFiAp2");
    strcpy(g_wifi_info.f.config.wifi_ap2_pass, "Password2");
}

void ConfigInit( void )
{
    memset(&g_wifi_info, 0, sizeof(wifi_info));
}

void ConfigLoad( void )
{
    int i;

    for(i = 0; i < FLASH_ARRAY; i++)
    {
        g_wifi_info.f.ui32[i] = FLASH_ReadWord( ((uint32_t)flash_reserved_space) + (i*4) );
    }
    
    if(g_wifi_info.f.config.flash_ok != CONFIG_FLASK_OK)
    {
        ConfigLoadDefault();
    }
}

void ConfigSave(void)
{
    int i;

    FLASH_Unlock(FLASH_UNLOCK_KEY);

    for(i = 0; i < FLASH_ARRAY; i+=2)
    {
        FLASH_WriteDoubleWord(((uint32_t)flash_reserved_space) + (i*4), 
                g_wifi_info.f.ui32[i],
                g_wifi_info.f.ui32[i+1]);
    }
    
    FLASH_Lock();
}

void ConfigTimer(void)
{
    if(g_wifi_info.status.save_config)
    {
        g_wifi_info.status.save_config--;
        if(g_wifi_info.status.save_config == 0)
        {
            ConfigSave();
        }
    }
    
}