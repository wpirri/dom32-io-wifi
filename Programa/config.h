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
#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <stdbool.h>
#include <stdint.h>

/* Activa el logueo a la UART */
//#define DEBUG_ON
/* Incrementa mensaje de logueo del modulo wifi */
//#define DEBUG_WIFI
/* Vuelca el log a tarjeta SD */
//#define LOG_TO_FILE

// Clock frequency values
#define _XTAL_FREQ  24000000UL             // Hz
#define GetSystemClock()		(_XTAL_FREQ)
#define GetInstructionClock()	(GetSystemClock()/*/4*/)
#define GetPeripheralClock()	(GetSystemClock()/*/4*/)

#define DOWNLOAD_PATH           "/download"
#define FIRMWARE_FILENAME       "pgm.hex"
#define FLASH_OK 0xa5a5a5a5

typedef struct _config_in_file_ /*  */
{
    uint32_t dummy1;
    char wifi_ap1[33];                  /* ap1 */
    char wifi_ap1_pass[33];             /* ap1p */
    char wifi_ap2[33];                  /* ap2 */
    char wifi_ap2_pass[33];             /* ap2p */
    char wifi_host1[33];                /* ce1 */
    char wifi_host2[33];                /* ce2 */
    char rqst_path[33];                 /* path */
    uint16_t wifi_host1_port;           /* ce1p */
    uint16_t wifi_host2_port;           /* ce2p */
    uint16_t report;                    /* report */
    unsigned char io_config;
    unsigned char an_config;
    unsigned char enable_https;
    unsigned char enable_wiegand;
    unsigned char io_config_default;
    unsigned long rtcc_clk_div_adj;
    unsigned long start_config_mode;
    char wifi_sta_last_ip[16];
} config_in_file;

typedef struct
{
    union
    {
        char dummy[ ((sizeof(config_in_file)/sizeof(uint32_t))+1)*sizeof(uint32_t) ];
        config_in_file config;
    } f;
    struct
    {
        bool wifi_ok;
        bool wifi_start_ap;
        bool wifi_start_st;
        bool wifi_ssl_capable;
        bool flag_sd_ok;
        bool wifi_conectado_ok;
        unsigned char wifi_ap_select;
        unsigned char wifi_conectado_ap;
        unsigned char host_conectado;
        unsigned char wifi_mode;
        unsigned char save_config;
        unsigned int wifi_port_select;
        unsigned int count_inactividad;
        unsigned int control_notificar;
        char hw_mac[13];
        char wifi_at[65];
        char wifi_sdk[65];
        char up_fw[65];
    } status;
} wifi_info;

#ifdef _IN_CONFIG_C_
wifi_info g_wifi_info;
#else
extern wifi_info g_wifi_info;
#endif /* _IN_CONFIG_C_ */

void ConfigInit( void );
void ConfigLoad( void );
void ConfigSave(void);
void ConfigTimer(void);

#endif /* _CONFIG_H_ */