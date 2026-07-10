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

/* ****************************************************************************
 * Dom32Touch - Bastidor Tactil para sistema de dom�tica
 * 
 * PIC21MM00256GPM028
 * ESP-WROOM-02D
 * MicroSD
 * Display 2.4'' c/Driver ILI9341
 * Touch resistivo
 * 
 * Asignacion de pines del procesador
 * 
 *      Port        Funcionalidad       Conectado a
 * ----------------------------------------------------------------------------
 * 1    MCLR / Vpp                      Programaci�n/Debug/Reset
 * 2    RA0                 out         Led Status
 * 3    RA1                 out         Led Modo
 * 4    RB0/PGED1                       SPI_SDO -> MOSI / Programacion/Debug
 * 5    RB1/PGEC1                       SPI_SDI -> MISO / Programacion/Debug
 * 6    RB2/AN4             in          IO4
 * 7    RB3/AN11            in          IO3
 * 8    Vss                             Vss
 * 9    RA2/RP3/AN5         out         SPI_SCK -> CLK
 * 10   RA3/RP4             UART2       Tx
 * 11   RB4/RP5             in          IO2
 * 12   RA4/RP6             out         Led Aux
 * 13   Vdd                             +3v3
 * 14   RB5                 in          IO1
 * 15   RB6                 out         SPI_CS -> /CS
 * 16   RB7                 in          IO5
 * 17   RB8/SCK1            in          IO6
 * 18   RB9/SDO1            out         OUT1
 * 19   RC9                 out         OUT2
 * 20   VCap                            Cap 100n + 10uF
 * 21   RB10/RP17           out         OUT3
 * 22   RB11/RP18           out         OUT4
 * 23   RB12/RP12           
 * 24   RB13/RP13           out/oc      RESET -> ESP-WROOM-02_Reset-pin15
 * 25   RB14/RP19           UART1       Tx    -> ESP-WROOM-02_Rx-pin11
 * 26   RB15/RP10           UART1       Rx    -> ESP-WROOM-02_Tx-pin12
 * 27   AVss                            Vss
 * 28   AVdd                            +3v3
 * 
 * **************************************************************************/
#include <xc.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "defines.h"
#include "system.h"
#include "log.h"
#include "sd/fsio.h"
#include "config.h"
#include "rtcc.h"


#include "io.h"
#include "wifi/wifi.h"
#include "control.h"
#include "http.h"

int main(void)
{
    bool status_sd_ok = false;
   
    SystemInit();
    
    LogInit(true, false);
    
    if( FSInit() )
    {
        status_sd_ok = true;
        LogMessage("[MAIN] File System Ok");
    }
    else
    {
        status_sd_ok = false;
        LogMessage("[MAIN] File System Error");
    }
    LogMessage("[MAIN] Compilacion " __DATE__ " " __TIME__ ".");
    LogMessage("[MAIN] Compilador " __VERSION__ ".");
    
    FSremove(FIRMWARE_FILENAME);

    LogMessage("[MAIN] Configuracion");
    ConfigInit();
    if(status_sd_ok) ConfigLoad();
    DumpConfig();

    LogMessage("[MAIN] Init RTCC");
    RTCC_Initialize();

    LogMessage("[MAIN] I/O");
    IOInit();
    
    LogMessage("[MAIN] Init WiFi");
    WiFiInit();

    LogMessage("[MAIN] Init Http");
    HttpInit();
    
    LogMessage("[MAIN] Init STM");
    ControlInit(status_sd_ok);
    /* Reset POR y BOR 
       Se hace a lo último así las distintas inicializaciones pueden verificar
       si vengo de un Power On */
    if(RCONbits.POR || RCONbits.BOR)
    {
        /* Power-Up*/
    }
    RCONbits.POR = 0;
    RCONbits.BOR = 0;

    LogMessage("[MAIN] Init End");
    while(1)
    {
        CLRWDT();
        ControlTask();
        
        Control_WiFi_Task();
    }
    return 0;
}
