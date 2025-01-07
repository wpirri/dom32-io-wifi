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
#include <stdbool.h>

#include "mcc_generated_files/pin_manager.h"
#include "mcc_generated_files/system.h"
#include "mcc_generated_files/watchdog.h"

#include "control.h"
#include "clog.h"
#include "sd/fsio.h"
#include "flash/plib_nvm.h"

#define CONFIG_FILENAME "D32WIFI.CFG"

uint32_t config_flash_save_start_address;
uint32_t config_flash_end_address;

bool ConfigIsValidFlash( void );
void ConfigReadFromFlash( void );
void ConfigWriteToFlash( void );
void DumpConfig( void );


void ConfigLoadDefault( void)
{
    memset(&g_wifi_info.f.config, 0, sizeof(config_in_file));
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
    g_wifi_info.f.config.start_config_mode = true;

    g_wifi_info.status.save_config = 10;
}

void ConfigInit( void )
{
    memset(&g_wifi_info, 0, sizeof(wifi_info));
    config_flash_end_address = NVM_FLASH_START_ADDRESS + NVM_FLASH_SIZE;
    config_flash_save_start_address = config_flash_end_address - NVM_FLASH_PAGESIZE;
}

void ConfigLoad( void )
{
    FSFILE* f;
    int rc;

    if(g_wifi_info.status.flag_sd_ok)
    {
    	
    	
        LogMessage("[ConfigLoad] Leyendo configuracion de %s.", CONFIG_FILENAME);
        /* Lectura de configuración em tarjeta SD */
        f = FSfopen(CONFIG_FILENAME, FS_READ);
        if(f)
        {
            rc = FSfread(&g_wifi_info.f.config, 1, sizeof(config_in_file), f);
            FSfclose(f);
            LogMessage("[ConfigLoad] Leidos %i bytes de %s.", rc, CONFIG_FILENAME);
            if(rc >= 236)
            {
                LogMessage("[ConfigLoad] Lectura de %s Ok.", CONFIG_FILENAME);
                if(rc < sizeof(config_in_file))
                {
                    LogMessage("[ConfigLoad] Ajustando registro de configuracion de %d a %d bytes.",
                            rc, sizeof(config_in_file));
                    memset(&g_wifi_info.f.dummy[rc], 0, (sizeof(config_in_file)-rc));
                }
                
                DumpConfig();
                /* Me fijo si la copia en flash está Ok */
                if( !ConfigIsValidFlash())
                {
                    /* Hay que guardar la configuracion en flash */
                    ConfigWriteToFlash();
                }
                return;
            }
            LogMessage("[ConfigLoad] Error de lectura de %s.", CONFIG_FILENAME);

            /* Me fijo si la copia en flash está Ok */
            if(ConfigIsValidFlash())
            {
                /* Lectura alternativa de configuración en flash */
                ConfigReadFromFlash();
                DumpConfig();
                return;
            }
        }
        else
        {
            LogMessage("[ConfigLoad] No existe el archivo %s.", CONFIG_FILENAME);
            /* Me fijo si la copia en flash está Ok */
            if(ConfigIsValidFlash())
            {
                /* Lectura alternativa de configuración en flash */
                ConfigReadFromFlash();
                /* Guardo en SD */
                ConfigSave();
                return;
            }
            else
            {
                LogMessage("[ConfigLoad] Copia en flash no valida.");
            }
        }
    }
    else
    {
        LogMessage("[ConfigLoad] SD no presente. Utilizando respaldo en flash.", CONFIG_FILENAME);
        /* Me fijo si la copia en flash está Ok */
        if(ConfigIsValidFlash())
        {
            /* Lectura alternativa de configuración en flash */
            ConfigReadFromFlash();
            DumpConfig();
            return;
        }
        else
        {
            LogMessage("[ConfigLoad] Copia en frash no valida.");
        }
    }
    
    LogMessage("[ConfigLoad] Load Defaults.");
    ConfigLoadDefault();
    ConfigSave();
}

void ConfigSave(void)
{
    FSFILE* f;
    int rc;

    g_wifi_info.status.save_config = 0;
    
    if(g_wifi_info.status.flag_sd_ok)
    {
        LogMessage("[ConfigSave] Guardando configuracion en %s.", CONFIG_FILENAME);
        f = FSfopen(CONFIG_FILENAME, FS_WRITE);
        if(f)
        {
            rc = FSfwrite(&g_wifi_info.f.config, sizeof(config_in_file), 1, f);
            FSfclose(f);
            if(rc == 1)
            {
                LogMessage("[ConfigSave] Guardado en %s Ok.", CONFIG_FILENAME);
                DumpConfig();
            }
            else
            {
                LogMessage("[ConfigSave] Error de escritura en %s.", CONFIG_FILENAME);
            }
        }
        else
        {
            LogMessage("[ConfigSave] Error al abrir/crear el archivo "CONFIG_FILENAME" en SD.");
        }
    }
    /* Guardo una copia de la configuración en flash */
    ConfigWriteToFlash();    
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

bool ConfigIsValidFlash( void )
{
    uint32_t i32;
    
    NVM_Read( &i32, sizeof(uint32_t), config_flash_save_start_address);
    return ((i32 == FLASH_OK)?true:false);
}

void ConfigReadFromFlash( void )
{
    uint32_t *p_data;
    uint32_t addr;
    int len;
    
    LogMessage("[ConfigReadFromFlash] Leyendo configuracion de flash.");

    addr = config_flash_save_start_address;
    p_data = (uint32_t*)&g_wifi_info.f.config;
    len = sizeof(config_in_file);

    while(len > 0)
    {
        WATCHDOG_TimerClear();

        NVM_Read( p_data, sizeof(uint32_t), addr);
        
        addr += 4;
        p_data ++;
        len -= sizeof(uint32_t);
    }
}

void ConfigWriteToFlash( void )
{
    uint32_t *p_data;
    uint32_t addr;
    int len;
    
    LogMessage("[ConfigWriteToFlash] Guardando configuracion en flash.");

    for( addr = config_flash_save_start_address;
         addr < config_flash_end_address;
         addr += NVM_FLASH_PAGESIZE )
    {
        WATCHDOG_TimerClear();
        NVM_PageErase(addr);
        while(NVM_IsBusy()) {}
    }

    addr = config_flash_save_start_address;
    p_data = (uint32_t*)&g_wifi_info.f.config;
    len = sizeof(config_in_file);
    
    while(len > 0)
    {
        WATCHDOG_TimerClear();

        if( !NVM_DoubleWordWrite(p_data, addr))
        {
            LogMessage("[ConfigWriteToFlash] Error al escribir en flash.");
            break;
        }
        addr += 8;
        p_data += 2;
        len -= 2 * sizeof(uint32_t);

        while(NVM_IsBusy()) {}
    }
}

void DumpConfig( void )
{
    LogMessage("[DumpConfig] Usando %i de %i bytes (util %i bytes).",
            sizeof(g_wifi_info.f.dummy),
            (config_flash_end_address - config_flash_save_start_address),
            sizeof(config_in_file));  
    LogMessage("[DumpConfig] wifi_ap1:         %s", g_wifi_info.f.config.wifi_ap1);
    LogMessage("[DumpConfig] wifi_ap1_pass:    %s", g_wifi_info.f.config.wifi_ap1_pass);
    LogMessage("[DumpConfig] wifi_ap2:         %s", g_wifi_info.f.config.wifi_ap2);
    LogMessage("[DumpConfig] wifi_ap2_pass:    %s", g_wifi_info.f.config.wifi_ap2_pass);
    LogMessage("[DumpConfig] wifi_host1:       %s", g_wifi_info.f.config.wifi_host1);
    LogMessage("[DumpConfig] wifi_host1_port:  %d", g_wifi_info.f.config.wifi_host1_port);
    LogMessage("[DumpConfig] wifi_host2:       %s", g_wifi_info.f.config.wifi_host2);
    LogMessage("[DumpConfig] wifi_host2_port:  %d", g_wifi_info.f.config.wifi_host2_port);
    LogMessage("[DumpConfig] rqst_path:        %s", g_wifi_info.f.config.rqst_path);
    LogMessage("[DumpConfig] io_config:        0x%02X", g_wifi_info.f.config.io_config);
    LogMessage("[DumpConfig] an_config:        0x%02X", g_wifi_info.f.config.an_config);
    LogMessage("[DumpConfig] report:           %d", g_wifi_info.f.config.report);
    LogMessage("[DumpConfig] enable_https:     %s", (g_wifi_info.f.config.enable_https)?"si":"no");
    LogMessage("[DumpConfig] enable_wiegand:   %s", (g_wifi_info.f.config.enable_wiegand)?"si":"no");
    LogMessage("[DumpConfig] rtcc_clk_div_adj: %li", g_wifi_info.f.config.rtcc_clk_div_adj);
}
