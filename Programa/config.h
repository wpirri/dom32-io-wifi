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

// Clock frequency values
#define _XTAL_FREQ  24000000UL             // Hz
#define GetSystemClock()		(_XTAL_FREQ)
#define GetInstructionClock()	(GetSystemClock()/*/4*/)
#define GetPeripheralClock()	(GetSystemClock()/*/4*/)

struct _config_ /* 2176 bits, 272 bytes */
{
    char wifi_ap1[33];
    char wifi_ap1_pass[33];
    char wifi_ap2[33];
    char wifi_ap2_pass[33];
    char wifi_host1[33];
    char wifi_host2[33];
    char rqst_path[33];
    uint16_t wifi_host1_port;
    uint16_t wifi_host2_port;
    uint16_t report;
    unsigned char io_config;
    unsigned char an_config;
    unsigned char enable_https;
    unsigned char enable_wiegand;
    unsigned char io_config_default;
    unsigned long rtcc_clk_div_adj;
};

typedef struct
{
    union
    {
        struct _config_ config;
    } f;
    struct
    {
        unsigned char wifi_ap_select;
        unsigned char wifi_conectado;
        unsigned char host_conectado;
        unsigned char wifi_mode;
        unsigned char save_config;
        char hw_mac[13];
        char wifi_at[65];
        char wifi_sdk[65];
        char up_fw[65];
        bool wifi_ok;
        bool wifi_start_ap;
        bool wifi_ssl_capable;
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