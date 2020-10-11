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
#ifndef _MAIN_H_    /* Guard against multiple inclusion */
#define _MAIN_H_

#include <stdbool.h>

typedef struct
{
	char wifi_ap1[33];
	char wifi_ap1_pass[65];
	char wifi_ap2[33];
	char wifi_ap2_pass[65];

	char wifi_host1[33];
	unsigned int wifi_host1_port;
	char wifi_host2[33];
	unsigned int wifi_host2_port;
} _config;


typedef struct 
{
    bool    flag_core_timer;
    
} _globals;



#ifdef _IN_MAIN_C_
_globals g;
#else /* _IN_MAIN_C_ */
extern _globals g;
#endif /* _IN_MAIN_C_ */

#endif /* _MAIN_H_ */
