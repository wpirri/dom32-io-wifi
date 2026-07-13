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
#include "control.h"

#include <xc.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "defines.h"
#include "log.h"
#include "sd/fsio.h"
#include "config.h"
#include "rtcc.h"


#include "wifi/wifi.h"
#include "io.h"
#include "misc/strfunc.h"

#include "misc/beep.h"
#include "http.h"
#include "system.h"
#include "rtcc.h"

/* **** WiFi *************************************************************** */
typedef enum
{
    WIFI_INIT = 0,
    WIFI_MODO_AP,
    WIFI_MODO_AP_OK,
    WIFI_MODO_ST,
    WIFI_MODO_ST_OK,
    WIFI_CONECTAR_A_HOST,
    WIFI_CONECTADO_A_HOST,
    WIFI_DESCONECTADO_DE_HOST,
    WIFI_ERROR,
    WIFI_ERROR_FATAL,
    WIFI_ERROR_FATAL_LOOP
} enum_control_wifi_stm;

enum_control_wifi_stm control_wifi_stm;
unsigned int control_wifi_stm_delay;

/* **** Control ************************************************************ */
bool status_sd_ok;
bool assinc_timer_flag;
bool start_in_ap_mode;
unsigned int wifi_config_timer_count;
unsigned int keep_alive_timer_count;
unsigned int reset_timer_count;
unsigned int update_firmware_timer_count;
unsigned int save_config_timer_count;
unsigned int control_file_flags;
char filename_to_download[256];
unsigned int download_list_count;

void DownloadFile(const char* filename);
void ControlHostKeepAlive(void);
void ControlFileFlags( void );
void ProcesarDownloadList( void );

void ControlInit( bool sd_ok )
{
    status_sd_ok = sd_ok;
    filename_to_download[0] = 0;
    keep_alive_timer_count = 0;
    reset_timer_count = 0;
    update_firmware_timer_count = 0;
    save_config_timer_count = 0;
    assinc_timer_flag = false;
    control_file_flags = 6000;
    download_list_count = 0;
    
    /* **** WiFi *********************************************************** */
    control_wifi_stm = WIFI_INIT;
    control_wifi_stm_delay = 500;
    wifi_config_timer_count = 0;
    
    start_in_ap_mode = false;
    if(g_wifi_config.u.config.wifi_ap1[0] == 0 && g_wifi_config.u.config.wifi_ap2[0] == 0)
    {
        g_wifi_config.u.config.start_config_mode = 1;
    }
    if(g_wifi_config.u.config.start_config_mode)
    {
        start_in_ap_mode = true;
    }
    g_wifi_config.u.config.start_config_mode = 1;

    save_config_timer_count = 1;
}

void ControlTimer( void )
{
    /* Para las cosas que necesitan el timer pero no pueden estar dentro de una int */
    assinc_timer_flag = true;
    
    if(control_wifi_stm_delay) control_wifi_stm_delay--;
    if(wifi_config_timer_count) wifi_config_timer_count--;
    if(WiFiConectado() && keep_alive_timer_count) keep_alive_timer_count--;
    if(control_file_flags) control_file_flags--;
    if(reset_timer_count)
    {
        reset_timer_count--;
        if(reset_timer_count == 0)
        {
            SystemReset();
        }
    }
}

void ControlTask(void)
{
    if(WiFiConectado() && keep_alive_timer_count == 0)
    {
        ControlHostKeepAlive();
    }
    
    if(control_file_flags == 0)
    {
        control_file_flags = 6000;
        if(HttpOnLine())
        {
            ControlFileFlags();
        }
    }
    
    /* Para las cosas que dependen del timer pero no pueden estar en una interrupcion */
    if(assinc_timer_flag)
    {
        assinc_timer_flag = false;
        
        if(save_config_timer_count)
        {
            save_config_timer_count--;
            if(save_config_timer_count == 0)
            {
                ConfigSave();
            }

        }

        if(update_firmware_timer_count)
        {
            update_firmware_timer_count--;
            if(update_firmware_timer_count == 0)
            {
                DownloadFile("/download/"FIRMWARE_FILENAME);
            }
        }
        
    }
    
    if(filename_to_download[0])
    {
        LogMessage("[ControlTask] Download: [%s]", filename_to_download);

        DownloadFile(filename_to_download);

        filename_to_download[0] = 0;
    }
}

void DownloadFile(const char* filename)
{
    HttpDownload(filename);
}

void ControlHostKeepAlive(void)
{
    char url[256];
    char data[1024];
    int rc;
    char resp_err_code[16];
    char resp_err_msg[256];
    int i;
    
    sprintf(url, "%s/infoio.cgi", g_wifi_config.u.config.rqst_path);
    sprintf(data, "ID=%s&TYP=IO", WiFiMAC());
    rc = HttpQueryHost(url, data, NULL, data);
    if(rc == 200)
    {
        LogMessage("[ControlHostKeepAlive] HTTP Req Ok");

        /* Si la respuesta vino entre [""] le hago limpieza */
        if(data[0] == '[' && data[1] == '\"')
        {
            for(i = 2; data[i]; i++)
            {
                data[i-2] = data[i];
            }
            data[i-4] = 0;
        }

        if(HTTPGetFormData(data, "error", resp_err_code))
        {
            if(HTTPGetFormData(data, "message", resp_err_msg))
            {
                if(atoi(resp_err_code) == 0)
                {
                    LogMessage("[ControlHostKeepAlive] Respuesta del sistema Ok");
                    HttpConexionOk();
                    /* Busco configuraciones, comandos, etc */
                    ProcessGeneralConfig(data);
                    keep_alive_timer_count = g_wifi_config.u.config.report;
                    if(g_wifi_config.u.config.start_config_mode)
                    {
                        g_wifi_config.u.config.start_config_mode = 0;
                        save_config_timer_count = 1;
                    }
                    return;
                }
                else
                {
                    LogMessage("[ControlHostKeepAlive] Respuesta del sistema [error = %s message = %s]", resp_err_code, resp_err_msg );
                }
            }
        }
    }
    else
    {
        LogMessage("[ControlHostKeepAlive] HTTP Req No Ok [%i]", rc);
    }
    HttpConexionError();
    
    keep_alive_timer_count = (g_wifi_config.u.config.report > 500)?500:g_wifi_config.u.config.report;
}

void ProcessGeneralConfig(char* in)
{
    char s[256];
    struct tm tm_time;
    char *p;
    
    LogMessage("[ProcessGeneralConfig] [%s]", in);
    
    if(HTTPGetFormData(in, "ap1", g_wifi_config.u.config.wifi_ap1))
    {
        save_config_timer_count = 500;
    }
    if(HTTPGetFormData(in, "ap1p", g_wifi_config.u.config.wifi_ap1_pass))
    {
        save_config_timer_count = 500;
    }
    if(HTTPGetFormData(in, "ap2", g_wifi_config.u.config.wifi_ap2))
    {
        save_config_timer_count = 500;
    }
    if(HTTPGetFormData(in, "ap2p", g_wifi_config.u.config.wifi_ap2_pass))
    {
        save_config_timer_count = 500;
    }
    if(HTTPGetFormData(in, "ce1", g_wifi_config.u.config.wifi_host1))
    {
        p = strchr(g_wifi_config.u.config.wifi_host1, ':');
        if(p)
        {
            *p = 0;
            p++;
            /* viene con el port */
            g_wifi_config.u.config.wifi_host1_port = atoi(p);
        }
        save_config_timer_count = 500;
    }
    if(HTTPGetFormData(in, "ce2", g_wifi_config.u.config.wifi_host2))
    {
        p = strchr(g_wifi_config.u.config.wifi_host2, ':');
        if(p)
        {
            *p = 0;
            p++;
            /* viene con el port */
            g_wifi_config.u.config.wifi_host2_port = atoi(p);
        }
        save_config_timer_count = 500;
    }
    if(HTTPGetFormData(in, "path", g_wifi_config.u.config.rqst_path))
    {
        save_config_timer_count = 500;
    }
    if(HTTPGetFormData(in, "ce1p", s))
    {
        g_wifi_config.u.config.wifi_host1_port = atoi(s);
        save_config_timer_count = 500;
    }
    if(HTTPGetFormData(in, "ce2p", s))
    {
        g_wifi_config.u.config.wifi_host2_port = atoi(s);
        save_config_timer_count = 500;
    }
    if(HTTPGetFormData(in, "rep", s))
    {
        g_wifi_config.u.config.report = atoi(s) * 100;
        if(g_wifi_config.u.config.report == 0)
        {
            g_wifi_config.u.config.report = 500;
        }
        save_config_timer_count = 500;
    }

    if(HTTPGetFormData(in, "reset", s))
    {
        reset_timer_count = 1000;
    }
    if(HTTPGetFormData(in, "update", s))
    {
        if( !strcmp(s, "firmware"))
        {
            update_firmware_timer_count = 300;
        }
        else if( !strcmp(s, "config"))
        {
            /* TODO: Actualizar la configuración de I/O */
            
        }
    }

    if(HTTPGetFormData(in, "time", s))
    {
        LogMessage("TIME: %s", s);
        /*             1111111111
         *   01234567890123456789
         tmp YYYY/MM/DD hh:mm:ss
         */
        if(s[4] == '/' && s[7] == '/' && s[10] == ' ' && 
                s[13] == ':' && s[16] == ':')
        {
            s[4] = 0;
            s[7] = 0;
            s[10] = 0;
            s[13] = 0;
            s[16] = 0;
            s[19] = 0;
            
            tm_time.tm_year = atoi(&s[0]) - 1900;
            tm_time.tm_mon = atoi(&s[5]) - 1;
            tm_time.tm_mday = atoi(&s[8]);
            tm_time.tm_hour = atoi(&s[11]);
            tm_time.tm_min = atoi(&s[14]);
            tm_time.tm_sec = atoi(&s[17]);

            RTCC_TimeSet(&tm_time);
        }
        else
        {
            LogMessage("[CTRLConfSys] Error de formato para TIME");
        }
    }
    if(HTTPGetFormData(in, "download", s))
    {
        strcpy(filename_to_download, s);
    }
    
    /* **** Control de I/O ************************************************* */
    /* **** Config **** */
    if(HTTPGetFormData(in, "wiegand", s))
    {
        if(s[0] == '1')
        {
            g_wifi_config.u.config.io_config_default = 0;
            save_config_timer_count = 500;
            g_wifi_config.u.config.enable_wiegand = 1;
            /* Entrada 5 - RB7 */
            TRISBbits.TRISB7 = 1;
            CNPUBbits.CNPUB7 = 1;
            /* Entrada 6 - RB8 */
            TRISBbits.TRISB8 = 1;
            CNPUBbits.CNPUB8 = 1;
            /* Control de interrupcion */
            //    CNBI: PORT B Change Notification
            //    Priority: 1
            IPC2bits.CNBIP = 1;
            //    Sub Priority: 0
            IPC2bits.CNBIS = 0;
            //Clear CNBI interrupt flag
            IFS0CLR= 1 << _IFS0_CNBIF_POSITION;
            //Enable CNBI interrupt
            IEC0bits.CNBIE = 1;
        }
        else
        {
            g_wifi_config.u.config.io_config_default = 0;
            save_config_timer_count = 500;
            g_wifi_config.u.config.enable_wiegand = 0;
            
            //Disable CNBI interrupt
            IEC0bits.CNBIE = 0;
        }
    }
    if(HTTPGetFormData(in, "Config_IO1", s))
    {
        /* Entrada 1 - RB5 */
        if(s[0] == 'i')
        {
            g_wifi_config.u.config.io_config_default = 0;
            g_wifi_config.u.config.io_config_values[0] = s[0];
            save_config_timer_count = 500;
            TRISBbits.TRISB5 = 1;
            CNPUBbits.CNPUB5 = 1;
        }
        else if(s[0] == 'o')
        {
            g_wifi_config.u.config.io_config_default = 0;
            g_wifi_config.u.config.io_config_values[0] = s[0];
            save_config_timer_count = 500;
            TRISBbits.TRISB5 = 0;
            CNPUBbits.CNPUB5 = 0;
        }
    }
    if(HTTPGetFormData(in, "Config_IO2", s))
    {
        /* Entrada 2 - RB4 */
        if(s[0] == 'i')
        {
            g_wifi_config.u.config.io_config_default = 0;
            g_wifi_config.u.config.io_config_values[1] = s[0];
            save_config_timer_count = 500;
            TRISBbits.TRISB4 = 1;
            CNPUBbits.CNPUB4 = 1;
        }
        else if(s[0] == 'o')
        {
            g_wifi_config.u.config.io_config_default = 0;
            g_wifi_config.u.config.io_config_values[1] = s[0];
            save_config_timer_count = 500;
            TRISBbits.TRISB4 = 0;
            CNPUBbits.CNPUB4 = 0;
        }
    }
    if(HTTPGetFormData(in, "Config_IO3", s))
    {
        /* Entrada 3 - RB3 / AN11 */
        if(s[0] == 'i')
        {
            g_wifi_config.u.config.io_config_default = 0;
            g_wifi_config.u.config.io_config_values[2] = s[0];
            save_config_timer_count = 500;
            TRISBbits.TRISB3 = 1;
            ANSELBbits.ANSB3 = 0;   // AN11
            CNPUBbits.CNPUB3 = 1;
        }
        else if(s[0] == 'o')
        {
            g_wifi_config.u.config.io_config_default = 0;
            g_wifi_config.u.config.io_config_values[2] = s[0];
            save_config_timer_count = 500;
            TRISBbits.TRISB3 = 0;
            ANSELBbits.ANSB3 = 0;   // AN11
            CNPUBbits.CNPUB3 = 0;
        }
        else if(s[0] == 'a')
        {
            g_wifi_config.u.config.io_config_default = 0;
            g_wifi_config.u.config.io_config_values[2] = s[0];
            save_config_timer_count = 500;
            TRISBbits.TRISB3 = 1;
            ANSELBbits.ANSB3 = 1;   // AN11
            CNPUBbits.CNPUB3 = 0;
        }
    }
    if(HTTPGetFormData(in, "Config_IO4", s))
    {
        /* Entrada 4 - RB2 / AN4*/
        if(s[0] == 'i')
        {
            g_wifi_config.u.config.io_config_default = 0;
            g_wifi_config.u.config.io_config_values[3] = s[0];
            save_config_timer_count = 500;
            TRISBbits.TRISB2 = 1;
            ANSELBbits.ANSB2 = 0;   // AN4
            CNPUBbits.CNPUB2 = 1;
        }
        else if(s[0] == 'o')
        {
            g_wifi_config.u.config.io_config_default = 0;
            g_wifi_config.u.config.io_config_values[3] = s[0];
            save_config_timer_count = 500;
            TRISBbits.TRISB2 = 0;
            ANSELBbits.ANSB2 = 0;   // AN4
            CNPUBbits.CNPUB2 = 0;
        }
        else if(s[0] == 'a')
        {
            g_wifi_config.u.config.io_config_default = 0;
            g_wifi_config.u.config.io_config_values[3] = s[0];
            save_config_timer_count = 500;
            TRISBbits.TRISB2 = 1;
            ANSELBbits.ANSB2 = 1;   // AN4
            CNPUBbits.CNPUB2 = 0;
        }
    }
    if(HTTPGetFormData(in, "Config_IO5", s))
    {
        /* Entrada 5 - RB7 */
        if(g_wifi_config.u.config.enable_wiegand == 0)
        {
            if(s[0] == 'i')
            {
                g_wifi_config.u.config.io_config_default = 0;
                g_wifi_config.u.config.io_config_values[4] = s[0];
                save_config_timer_count = 500;
                TRISBbits.TRISB7 = 1;
                CNPUBbits.CNPUB7 = 1;
            }
            else if(s[0] == 'o')
            {
                g_wifi_config.u.config.io_config_default = 0;
                g_wifi_config.u.config.io_config_values[4] = s[0];
                save_config_timer_count = 500;
                TRISBbits.TRISB7 = 0;
                CNPUBbits.CNPUB7 = 0;
            }
        }
    }
    if(HTTPGetFormData(in, "Config_IO6", s))
    {
        /* Entrada 6 - RB8 */
        if(g_wifi_config.u.config.enable_wiegand == 0)
        {
            if(s[0] == 'i')
            {
                g_wifi_config.u.config.io_config_default = 0;
                g_wifi_config.u.config.io_config_values[5] = s[0];
                save_config_timer_count = 500;
                TRISBbits.TRISB8 = 1;
                CNPUBbits.CNPUB8 = 1;
            }
            else if(s[0] == 'o')
            {
                g_wifi_config.u.config.io_config_default = 0;
                g_wifi_config.u.config.io_config_values[5] = s[0];
                save_config_timer_count = 500;
                TRISBbits.TRISB8 = 0;
                CNPUBbits.CNPUB8 = 0;
            }
        }
    }
    /* **** Set **** */
    if(HTTPGetFormData(in, "IO1", s))
    {
        if(s[0] == '1') IO1_SetHigh();
        else            IO1_SetLow();
    }
    if(HTTPGetFormData(in, "IO2", s))
    {
        if(s[0] == '1') IO2_SetHigh();
        else            IO2_SetLow();
    }
    if(HTTPGetFormData(in, "IO3", s))
    {
        if(s[0] == '1') IO3_SetHigh();
        else            IO3_SetLow();
    }
    if(HTTPGetFormData(in, "IO4", s))
    {
        if(s[0] == '1') IO4_SetHigh();
        else            IO4_SetLow();
    }
    if(HTTPGetFormData(in, "IO5", s))
    {
        if(s[0] == '1') IO5_SetHigh();
        else            IO5_SetLow();
    }
    if(HTTPGetFormData(in, "IO6", s))
    {
        if(s[0] == '1') IO6_SetHigh();
        else            IO6_SetLow();
    }
    if(HTTPGetFormData(in, "OUT1", s))
    {
        if(s[0] == '1') OUT1_SetHigh();
        else            OUT1_SetLow();
    }
    if(HTTPGetFormData(in, "OUT2", s))
    {
        if(s[0] == '1') OUT2_SetHigh();
        else            OUT2_SetLow();
    }
    if(HTTPGetFormData(in, "OUT3", s))
    {
        if(s[0] == '1') OUT3_SetHigh();
        else            OUT3_SetLow();
    }
    if(HTTPGetFormData(in, "OUT4", s))
    {
        if(s[0] == '1') OUT4_SetHigh();
        else            OUT4_SetLow();
    }
    /* **** Control de I/O ************************************************* */
}

void ControlFileFlags( void )
{
    FSFILE *f;
    
    f = FSfopen(FIRMWARE_FILENAME, FS_READ);
    if(f)
    {
        FSfclose(f);
        reset_timer_count = 300;
        return;
    }
    
    if(filename_to_download[0] == 0)
    {
        f = FSfopen(DOWNLOAD_LIST_FILENAME, FS_READ);
        if(f)
        {
            FSfclose(f);
            ProcesarDownloadList();
            control_file_flags = 100;
            return;
        }
    }    
    
}

void ProcesarDownloadList( void )
{
    FSFILE *f;
    int rc;
    char line[256];
    unsigned int line_len;
    unsigned int line_count = 0;
    
    f = FSfopen(DOWNLOAD_LIST_FILENAME, FS_READ);
    if(f)
    {
        do {
            /* Leo una linea */
            line_len = 0;
            do {
                rc = FSfread(&line[line_len++], 1, 1, f);
            } while(line[line_len-1] != 0x0d && line[line_len-1] != 0x0a && line_len < 256 && rc == 1);
            line[--line_len] = 0;
            if(line_len > 0 && line[0] != '#')
            {
                /* Analizo la linea */
                if(line_count == download_list_count)
                {
                    if(strchr(line, '\r')) *(strchr(line, '\r')) = 0;
                    if(strchr(line, '\n')) *(strchr(line, '\n')) = 0;
                    
                    strcpy(filename_to_download, line);
                    
                    download_list_count++;
                    break;
                }
                line_count++;
            }
        } while(rc == 1);
        FSfclose(f);
        if(rc == 0)
        {
            /* Fin de archivo */
            download_list_count = 0;
            FSremove(DOWNLOAD_LIST_FILENAME);
        }
    }
}

/* **** WiFi *************************************************************** */
void Control_WiFi_Task(void)
{
    WiFiCheckUnsol();
    
    if(control_wifi_stm_delay)return;

    switch(control_wifi_stm)
    {
        case WIFI_INIT:
            if(WiFiStart() == 0)
            {
                /* Modo inicial del dispositivo */
                if(start_in_ap_mode)
                {
                    control_wifi_stm = WIFI_MODO_AP;
                }
                else
                {
                    control_wifi_stm = WIFI_MODO_ST;
                }
            }
            else
            {
                control_wifi_stm_delay = 500;
            }
            break;
        case WIFI_MODO_AP:
            if(WiFiSetMode(WIFI_MODE_AP) == 0)
            {
                control_wifi_stm++;
                wifi_config_timer_count = 6000;
            }
            else
            {
                control_wifi_stm = WIFI_ERROR;
            }
            break;
        case WIFI_MODO_AP_OK:
            if(wifi_config_timer_count == 0)
            {
                control_wifi_stm = WIFI_MODO_ST;
            }
            if(WiFiStaConectado())
            {
                wifi_config_timer_count = 250;
            }
            break;
        case WIFI_MODO_ST:
            if(WiFiSetMode(WIFI_MODE_ST) == 0)
            {
                control_wifi_stm++;
            }
            else
            {
                control_wifi_stm = WIFI_ERROR;
            }
            break;
        case WIFI_MODO_ST_OK:
            if(WiFiConectado() > 0)
            {
                control_wifi_stm++;
            }
            else
            {
                WiFiSwitchRouter();
                control_wifi_stm_delay = 500;
                control_wifi_stm = WIFI_MODO_ST;
            }
            break;
        case WIFI_CONECTAR_A_HOST:
            break;
        case WIFI_CONECTADO_A_HOST:
            break;
        case WIFI_DESCONECTADO_DE_HOST:
            break;
        case WIFI_ERROR:
            break;
        case WIFI_ERROR_FATAL:
            break;
        case WIFI_ERROR_FATAL_LOOP:
            break;
        default:
            break;
    }    
}
