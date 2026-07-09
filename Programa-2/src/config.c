/***************************************************************************
    Copyright (C) 2024   Walter Pirri

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

#include <xc.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "defines.h"
#include "log.h"
#include "sd/fsio.h"

void ConfigLoadDefault( void)
{
    memset(&g_wifi_config.u.config, 0, sizeof(config_in_file));
    g_wifi_config.u.config.wifi_host1_port = 80;
    g_wifi_config.u.config.wifi_host2_port = 80;
    strcpy(g_wifi_config.u.config.rqst_path, "/cgi-bin");
    strcpy(g_wifi_config.u.config.download_path, "/download");
    g_wifi_config.u.config.report = 600;
    g_wifi_config.u.config.enable_https = 0;
    g_wifi_config.u.config.io_config_default = 1;
    g_wifi_config.u.config.rtcc_clk_div_adj = 480000;
}

/*****************************************************************************/
void ConfigInit(void)
{
    memset(&g_wifi_config, 0, sizeof(wifi_config));
    ConfigLoadDefault();
}

void ConfigLoad( void )
{
    FSFILE* f;
    int rc;

    LogMessage("[CONFIG] ConfigLoad %s.", CONFIG_FILENAME);
    /* Lectura de configuraci�n em tarjeta SD */
    f = FSfopen(CONFIG_FILENAME, FS_READ);
    if(f)
    {
        rc = FSfread(&g_wifi_config.u.config, 1, sizeof(config_in_file), f);
        FSfclose(f);
        LogMessage("[CONFIG] Leidos %i bytes de %s.", rc, CONFIG_FILENAME);
        if(rc >= 236)
        {
            LogMessage("[CONFIG] Lectura de %s Ok.", CONFIG_FILENAME);
            if(rc < sizeof(config_in_file))
            {
                LogMessage("[CONFIG] Ajustando registro de configuracion de %d a %d bytes.",
                        rc, sizeof(config_in_file));
                memset(&g_wifi_config.u.dummy[rc], 0, (sizeof(config_in_file)-rc));
            }
            if(g_wifi_config.u.config.download_path[0] == 0)
            {
                strcpy(g_wifi_config.u.config.download_path, "/download");
            }
            return;
        }
        LogMessage("[CONFIG] Error de lectura de %s.", CONFIG_FILENAME);
    }
    
    LogMessage("[CONFIG] Load Defaults.");
    ConfigLoadDefault();
    ConfigSave();
}

void ConfigSave(void)
{
    FSFILE* f;
    int rc;

    LogMessage("[CONFIG] ConfigSave %s.", CONFIG_FILENAME);
    f = FSfopen(CONFIG_FILENAME, FS_WRITE);
    if(f)
    {
        rc = FSfwrite(&g_wifi_config.u.config, sizeof(config_in_file), 1, f);
        FSfclose(f);
        if(rc == 1)
        {
            LogMessage("[CONFIG] Guardado en %s Ok.", CONFIG_FILENAME);
            DumpConfig();
        }
        else
        {
            LogMessage("[CONFIG] Error de escritura en %s.", CONFIG_FILENAME);
        }
    }
    else
    {
        LogMessage("[CONFIG] Error al abrir/crear el archivo %s en SD.", CONFIG_FILENAME);
    }
}

void DumpConfig( void )
{
    LogMessage("[CONFIG] wifi_ap1:         %s", g_wifi_config.u.config.wifi_ap1);
    LogMessage("[CONFIG] wifi_ap1_pass:    %s", g_wifi_config.u.config.wifi_ap1_pass);
    LogMessage("[CONFIG] wifi_ap2:         %s", g_wifi_config.u.config.wifi_ap2);
    LogMessage("[CONFIG] wifi_ap2_pass:    %s", g_wifi_config.u.config.wifi_ap2_pass);
    LogMessage("[CONFIG] wifi_host1:       %s", g_wifi_config.u.config.wifi_host1);
    LogMessage("[CONFIG] wifi_host1_port:  %d", g_wifi_config.u.config.wifi_host1_port);
    LogMessage("[CONFIG] wifi_host2:       %s", g_wifi_config.u.config.wifi_host2);
    LogMessage("[CONFIG] wifi_host2_port:  %d", g_wifi_config.u.config.wifi_host2_port);
    LogMessage("[CONFIG] rqst_path:        %s", g_wifi_config.u.config.rqst_path);
    LogMessage("[CONFIG] report:           %d", g_wifi_config.u.config.report);
    LogMessage("[CONFIG] enable_https:     %s", (g_wifi_config.u.config.enable_https)?"si":"no");
    LogMessage("[CONFIG] rtcc_clk_div_adj: %li", g_wifi_config.u.config.rtcc_clk_div_adj);
}
