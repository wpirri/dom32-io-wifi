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
#ifndef _DEFINES_H_
#define	_DEFINES_H_

#include <xc.h>

#define SLEEP_TIMER 30

// Clock frequency values
#define _XTAL_FREQ  24000000UL             // Hz
#define GetSystemClock()		(_XTAL_FREQ)
#define GetInstructionClock()	(GetSystemClock()/*/4*/)
#define GetPeripheralClock()	(GetSystemClock()/*/4*/)

#define CLRWDT()  (WDTCONbits.WDTCLRKEY = 0x5743)

// 0x1D4C0 = 10ms
// 0x124F80 = 100ms
#define CORE_TIMER_COMPARE_VAL 0x1D4C0

#define FIRMWARE_FILENAME       "domtouch.hex"
#define TMP_FILENAME            "download.tmp"
#define CONFIG_FILENAME         "d32wifi.cfg"
#define DOWNLOAD_LIST_FILENAME  "download.lst"

#define TOUCH_DELAY 10
#define RETURN_TO_HOME 600
#define BOTON_BORDE 6
#define BOTON_REDONDO 15
#define BOTON_CUADRADO 0

#define HOME_SCREEN_CSV     "screen0.csv"
#define CONFIG_SCREEN_CSV   "config.csv"
#define OFFLINE_SCREEN_CSV  "offline.csv"
#define INIT_SCREEN_BMP     "power.bmp"
#define DOWNLOAD_SCREEN_BMP "download.bmp"
#define WIFI_SCREEN_BMP     "wifi.bmp"

#endif /* _DEFINES_H_ */