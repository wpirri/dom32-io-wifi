/***************************************************************************
    Copyright (C) 2022   Walter Pirri

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
#include "dhtxx.h"

#include <xc.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "delay.h"

unsigned char DHTxx_bitCount;
unsigned char DHTxx_highlen;
unsigned char DHTxx_lowlen;

#define DHTxx_MAX_BIT_COUNT 200

typedef union _dhtxx
{
    struct
    {
        unsigned char hum_ent;
        unsigned char hum_dec;
        unsigned char temp_ent;
        unsigned char temp_dec;
        unsigned char chksum;
    } data;
    unsigned char buffer[5];
} dht_xx;

dht_xx dhtxx;

void DHTxxInit( void )
{
    DHTxx_POWER_OUT = 1;
    memset(&dhtxx, 0, sizeof(dht_xx));
}

int DHTxxTemperatura( void )
{
    return (dhtxx.data.temp_ent*100)+dhtxx.data.temp_dec;
}

int DHTxxHumedad( void )
{
    return (dhtxx.data.hum_ent*100)+dhtxx.data.hum_dec;
}

/*
    ____                        ____          ________       ___       _______
        |______________________|    |________|        |__+__| - |__-__|   +   |__
         <----    > 80ms    -->20/40us 80us     80us    50   30   50     70
         <-------    MCU    -------> <--- DHT ...
         <-- Solicitud de datos  --> <--- Header --->  <-  Datos
                                                       <-  0  -> <-    1    ->
 *                                 intCount  1        2     3   4     5       6
    Datos: 8 bits   Parte entera de la temperatura
           8 bits   Parte decimal de la temperatura
           8 bits   Parte entera de la humedad
           8 bits   Parte decimalde la humedad
           8 bits   Checksum

*/
void DHTxxGetData( void )
{
    LogMessage("[DHTxx] GetData\r\n");
    /* Genero el pulso para solicitar datos */
    DHTxx_DATA_TRIS = 0;
    DHTxx_DATA_OUT = 0;
    /* Cada DHT11_REFRESH_DELAY pongo en out en canal de datos 
       y mando un cero por 20ms (con 18ms alcanza) */
    __delay_ms(20);
    /* Fin del pulso, pongo el canal como input, el pull-up deberia mantenerlo arriba */
    DHTxx_DATA_TRIS = 1;
    /* mientras espero inicializo las vaiables */
    DHTxx_bitCount = 0;

    DHTxx_bitCount = 0;

    dhtxx.buffer[0] = 0;
    dhtxx.buffer[1] = 0;
    dhtxx.buffer[2] = 0;
    dhtxx.buffer[3] = 0;
    dhtxx.buffer[4] = 0;
    
    /* Espera el start */
    DHTxx_lowlen = 0;
    while( DHTxx_DATA_IN == 0 && ++DHTxx_lowlen < DHTxx_MAX_BIT_COUNT);
    if(DHTxx_lowlen == DHTxx_MAX_BIT_COUNT)
    {
        LogMessage("[DHTxx] Low overflow en START\r\n");
        return;
    }
    DHTxx_highlen = 0;
    while(DHTxx_DATA_IN == 1 && ++DHTxx_highlen < DHTxx_MAX_BIT_COUNT);
    if(DHTxx_highlen == DHTxx_MAX_BIT_COUNT)
    {
        LogMessage("[DHTxx] High overflow en START\r\n");
        return;
    }
    /* Fin del start */

    /* Salteo el header */
    while( DHTxx_DATA_IN == 0 && ++DHTxx_lowlen < DHTxx_MAX_BIT_COUNT);
    if(DHTxx_lowlen == DHTxx_MAX_BIT_COUNT)
    {
        LogMessage("[DHTxx] Low overflow en HEAD\r\n");
        return;
    }
    DHTxx_highlen = 0;
    while(DHTxx_DATA_IN == 1 && ++DHTxx_highlen < DHTxx_MAX_BIT_COUNT);
    if(DHTxx_highlen == DHTxx_MAX_BIT_COUNT)
    {
        LogMessage("[DHTxx] High overflow en HEAD\r\n");
        return;
    }
    /* Fin del header */
    
    /* Aca arrancan los datos */
    
    for(DHTxx_bitCount = 0; DHTxx_bitCount < 40; DHTxx_bitCount++)
    {
        /* Mientras esta abajo o error */
        DHTxx_lowlen = 0;
        while( DHTxx_DATA_IN == 0 && ++DHTxx_lowlen < DHTxx_MAX_BIT_COUNT);
        DHTxx_highlen = 0;
        while( DHTxx_DATA_IN == 1 && ++DHTxx_highlen < DHTxx_MAX_BIT_COUNT);
        
        if(DHTxx_highlen > DHTxx_MAX_BIT_COUNT || DHTxx_lowlen > DHTxx_MAX_BIT_COUNT) break;

        if(DHTxx_highlen > DHTxx_lowlen)
        {
            dhtxx.buffer[DHTxx_bitCount/8] |= 1 << (7 - (DHTxx_bitCount%8));
        }
    }

    if(DHTxx_bitCount < 40)
    {
        LogMessage("[DHTxx] overflow en datos. H: %d L: %d Bit: %d\r\n",
                DHTxx_highlen, DHTxx_lowlen, DHTxx_bitCount);
        return;
    }
    /* Fin de datos */
    
    /* Controlo el checksum */
    dhtxx.buffer[4] -= dhtxx.buffer[3];
    dhtxx.buffer[4] -= dhtxx.buffer[2];
    dhtxx.buffer[4] -= dhtxx.buffer[1];
    dhtxx.buffer[4] -= dhtxx.buffer[0];

    if(dhtxx.buffer[4] == 0)
    {
        /* Todo OK, lo paso a las variables correspondientes */
        LogMessage("[DHTxx] Temp: %d.%d °C - HR: %d.%d %%\r\n",
                    dhtxx.data.temp_ent, dhtxx.data.temp_dec,
                    dhtxx.data.hum_ent, dhtxx.data.hum_dec);
    }
    else
    {
        LogMessage("DHT11 - Error de CHKSum %02X %02X %02X %02X (%02X != 0)\r\n",
                dhtxx.buffer[0], dhtxx.buffer[1], dhtxx.buffer[2],
                dhtxx.buffer[3], dhtxx.buffer[4]);
    }
}