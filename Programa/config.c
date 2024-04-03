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

#include "mcc_generated_files/memory/flash.h"
#include "control.h"
#include "clog.h"

#define CONFIG_FLASK_OK 0x5A5A5A5A

/* Have compiler allocate a page of flash from the NVM.  Aligned to a page. */
static const uint32_t __attribute((space(prog),aligned(FLASH_ERASE_PAGE_SIZE_IN_PC_UNITS))) flash_reserved_space[FLASH_ERASE_PAGE_SIZE_IN_PC_UNITS/4];

void ConfigLoadDefault( void)
{
    memset(&g_wifi_info.f.config, 0, sizeof(g_wifi_info.f.config));
    g_wifi_info.f.config.io_config = 0xFF;
    g_wifi_info.f.config.an_config = 0x00;
    g_wifi_info.f.config.wifi_host1_port = 80;
    g_wifi_info.f.config.wifi_host2_port = 80;
    strcpy(g_wifi_info.f.config.rqst_path, "/cgi-bin");
    g_wifi_info.f.config.report = 600;
    g_wifi_info.f.config.enable_https = 0;
    g_wifi_info.f.config.enable_wiegand = 0;
    g_wifi_info.f.config.io_config_default = 1;
    g_wifi_info.f.config.rtcc_clk_div_adj = 480000;
    g_wifi_info.f.config.flash_ok = CONFIG_FLASK_OK;

    g_wifi_info.status.save_config = 10;
}

void ConfigInit( void )
{
    memset(&g_wifi_info, 0, sizeof(wifi_info));
}

void ConfigLoad( void )
{
    int i;
    uint32_t f = (uint32_t)flash_reserved_space;
    
    for(i = 0; i < FLASH_ARRAY; i++)
    {
        g_wifi_info.f.ui32[i] = FLASH_ReadWord( f + (i*4) );
    }
    
    LogMessage("[ConfigLoad] %d de %d bytes usados (%d%%)", 
                sizeof(struct _config_),
                sizeof(flash_reserved_space),
                sizeof(struct _config_) * 100 / sizeof(flash_reserved_space));
    
    if(g_wifi_info.f.config.flash_ok != CONFIG_FLASK_OK)
    {
        ConfigLoadDefault();
        LogMessage("[ConfigLoad] Load Defaults.\n\n");
    }
    else
    {
        LogMessage("[ConfigLoad] Load Ok.\n\n");
    }
    LogMessage("[ConfigLoad] wifi_ap1:         %s", g_wifi_info.f.config.wifi_ap1);
    LogMessage("[ConfigLoad] wifi_ap1_pass:    %s", g_wifi_info.f.config.wifi_ap1_pass);
    LogMessage("[ConfigLoad] wifi_ap2:         %s", g_wifi_info.f.config.wifi_ap2);
    LogMessage("[ConfigLoad] wifi_ap2_pass:    %s", g_wifi_info.f.config.wifi_ap2_pass);
    LogMessage("[ConfigLoad] wifi_host1:       %s", g_wifi_info.f.config.wifi_host1);
    LogMessage("[ConfigLoad] wifi_host1_port:  %d", g_wifi_info.f.config.wifi_host1_port);
    LogMessage("[ConfigLoad] wifi_host2:       %s", g_wifi_info.f.config.wifi_host2);
    LogMessage("[ConfigLoad] wifi_host2_port:  %d", g_wifi_info.f.config.wifi_host2_port);
    LogMessage("[ConfigLoad] rqst_path:        %s", g_wifi_info.f.config.rqst_path);
    LogMessage("[ConfigLoad] io_config:        0x%02X", g_wifi_info.f.config.io_config);
    LogMessage("[ConfigLoad] an_config:        0x%02X", g_wifi_info.f.config.an_config);
    LogMessage("[ConfigLoad] report:           %d", g_wifi_info.f.config.report);
    LogMessage("[ConfigLoad] enable_https:     %s", (g_wifi_info.f.config.enable_https)?"si":"no");
    LogMessage("[ConfigLoad] enable_wiegand:   %s", (g_wifi_info.f.config.enable_wiegand)?"si":"no");
    LogMessage("[ConfigLoad] rtcc_clk_div_adj: %li", g_wifi_info.f.config.rtcc_clk_div_adj);
}

void __attribute__((optimize("-O0"))) ConfigSave(void)
{
    int i;
    uint32_t f = (uint32_t)flash_reserved_space;

    g_wifi_info.status.flash_error = false;
    g_wifi_info.status.save_config = 0;
    
    FLASH_Unlock(FLASH_UNLOCK_KEY);

    FLASH_ErasePage(f);
    
    for(i = 0; i < FLASH_ARRAY; i+=2)
    {
        if(FLASH_WriteDoubleWord( f + (i*4), 
                                  g_wifi_info.f.ui32[i],
                                  g_wifi_info.f.ui32[i+1]) == 0)
        {
            g_wifi_info.status.flash_error = true;
        }
    }
    
    FLASH_Lock();

    LogMessage("[ConfigSave] wifi_ap1:         %s", g_wifi_info.f.config.wifi_ap1);
    LogMessage("[ConfigSave] wifi_ap1_pass:    %s", g_wifi_info.f.config.wifi_ap1_pass);
    LogMessage("[ConfigSave] wifi_ap2:         %s", g_wifi_info.f.config.wifi_ap2);
    LogMessage("[ConfigSave] wifi_ap2_pass:    %s", g_wifi_info.f.config.wifi_ap2_pass);
    LogMessage("[ConfigSave] wifi_host1:       %s", g_wifi_info.f.config.wifi_host1);
    LogMessage("[ConfigSave] wifi_host1_port:  %d", g_wifi_info.f.config.wifi_host1_port);
    LogMessage("[ConfigSave] wifi_host2:       %s", g_wifi_info.f.config.wifi_host2);
    LogMessage("[ConfigSave] wifi_host2_port:  %d", g_wifi_info.f.config.wifi_host2_port);
    LogMessage("[ConfigSave] rqst_path:        %s", g_wifi_info.f.config.rqst_path);
    LogMessage("[ConfigSave] io_config:        0x%02X", g_wifi_info.f.config.io_config);
    LogMessage("[ConfigSave] an_config:        0x%02X", g_wifi_info.f.config.an_config);
    LogMessage("[ConfigSave] report:           %d", g_wifi_info.f.config.report);
    LogMessage("[ConfigSave] enable_https:     %s", (g_wifi_info.f.config.enable_https)?"si":"no");
    LogMessage("[ConfigSave] enable_wiegand:   %s", (g_wifi_info.f.config.enable_wiegand)?"si":"no");
    LogMessage("[ConfigSave] rtcc_clk_div_adj: %li", g_wifi_info.f.config.rtcc_clk_div_adj);
    
    LogMessage("[ConfigSave] Save Ok.");
}

void ConfigTimer(void)
{
    if(g_wifi_info.status.save_config)
    {
        g_wifi_info.status.save_config--;
        if(g_wifi_info.status.save_config == 0)
        {
            ConfigSave();
            CTRLInit();
        }
    }
    
}