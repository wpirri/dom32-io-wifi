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
#ifndef _CONFIG_H_
#define	_CONFIG_H_

#include <xc.h>
#include <stdbool.h>

typedef struct _config_in_file_ /*  */
{
    char wifi_ap1[33];                  /* ap1 */
    char wifi_ap1_pass[33];             /* ap1p */
    char wifi_ap2[33];                  /* ap2 */
    char wifi_ap2_pass[33];             /* ap2p */
    char wifi_host1[33];                /* ce1 */
    char wifi_host2[33];                /* ce2 */
    char rqst_path[33];                 /* path */
    unsigned int wifi_host1_port;       /* ce1p */
    unsigned int wifi_host2_port;       /* ce2p */
    unsigned int report;                /* report */
    unsigned char enable_https;
    unsigned char io_config_default;
    unsigned long rtcc_clk_div_adj;
    unsigned char start_config_mode;
    unsigned char io_config_values[8];
    unsigned char enable_wiegand;
} config_in_file;

typedef struct
{
    union
    {
        char dummy[ ((sizeof(config_in_file)/sizeof(uint32_t))+1)*sizeof(uint32_t) ];
        config_in_file config;
    } u;
} wifi_config;

#ifdef _IN_CONFIG_C_
wifi_config g_wifi_config;
#else
extern wifi_config g_wifi_config;
#endif /* _IN_CONFIG_C_ */


void ConfigInit(void);

void ConfigLoad( void );
void ConfigSave(void);

void DumpConfig( void );

#endif /* _CONFIG_H_ */