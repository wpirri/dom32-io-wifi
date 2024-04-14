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

#include "control.h"
#include "clog.h"
#include "sd/fsio.h"

#define CONFIG_FLASK_OK 0x5A5A5A5A

#define CONFIG_FILENAME "D32WIFI.CFG"

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
    FSFILE* f;
    int rc;
    
    f = FSfopen(CONFIG_FILENAME, "r");
    if(f)
    {
        rc = FSfread(&g_wifi_info.f, sizeof(g_wifi_info.f), 1, f);
        FSfclose(f);
        if(rc == sizeof(g_wifi_info.f))
        {
            if(g_wifi_info.f.config.flash_ok != CONFIG_FLASK_OK)
            {
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

                LogMessage("[ConfigLoad] Load Ok.\n\n");
                return;
            }
        }
        LogMessage("[ConfigSave] Load Error.");
    }
    LogMessage("[ConfigLoad] Load Defaults.\n\n");
    ConfigLoadDefault();
    ConfigSave();
}

void ConfigSave(void)
{
    FSFILE* f;
    int rc;

    g_wifi_info.status.flash_error = false;
    g_wifi_info.status.save_config = 0;
    
    f = FSfopen(CONFIG_FILENAME, "rw");
    if(f)
    {
        rc = FSfwrite(&g_wifi_info.f, sizeof(g_wifi_info.f), 1, 1);
        FSfclose(f);
        if(rc == sizeof(g_wifi_info.f))
        {
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
            return;
        }
    }
    g_wifi_info.status.flash_error = true;
    LogMessage("[ConfigSave] Save Error.");
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
