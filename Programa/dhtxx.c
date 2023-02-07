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

#include "mcc_generated_files/interrupt_manager.h"

#include "config.h"
#include "delay.h"
#include "clog.h"

#define DHTxx_MAX_BIT_COUNT 250

typedef struct _hl_values
{
    unsigned char h;
    unsigned char l;
    
} hl_values;

typedef union _dhtxx
{
    struct
    {
        unsigned char h1;
        unsigned char h2;
        unsigned char t1;
        unsigned char t2;
        unsigned char chksum;
    } data;
    unsigned char buffer[5];
} dht_xx;

dht_xx dhtxx;

void DHTxxInit( void )
{
    DHTxx_POWER_OUT = 1;
    dhtxx.buffer[0] = 0;
    dhtxx.buffer[1] = 0;
    dhtxx.buffer[2] = 0;
    dhtxx.buffer[3] = 0;
    dhtxx.buffer[4] = 0;
}

void DHTxxClose( void )
{
    DHTxx_POWER_OUT = 0;
    dhtxx.buffer[0] = 0;
    dhtxx.buffer[1] = 0;
    dhtxx.buffer[2] = 0;
    dhtxx.buffer[3] = 0;
    dhtxx.buffer[4] = 0;
}

int DHTxxTemperatura( int *t1, int *t2 )
{
    int t;
    /* De la libreria de Arduino para DHT22
      f = ((word)(data[2] & 0x7F)) << 8 | data[3];
      f *= 0.1;
      if (data[2] & 0x80) {
        f *= -1;
     */
    t = (dhtxx.data.t1 & 0x7F) << 8;
    t |= dhtxx.data.t2; 
    if(dhtxx.data.t1 & 0x08) t *= (-1);
    if(t1 && t2)
    {
        *t1 = t / 10;
        *t2 = t % 10;
    }
    
    return t;
}

int DHTxxHumedad( int *h1, int *h2 )
{
    int h;
    /*
     f = ((word)data[0]) << 8 | data[1];
     f *= 0.1;
    */
    h = dhtxx.data.h1 << 8;
    h |= dhtxx.data.h2; 
    if(h1 && h2)
    {
        *h1 = h / 10;
        *h2 = h % 10;
    }
    return h;
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
int /*__attribute__((optimize("-O0")))*/ DHTxxGetData( void )
{
    unsigned char b;
    unsigned char DHTxx_bitLen;
    hl_values v[40];
    
    //LogMessage("[DHT] GetData");
    memset(v, 0, sizeof(hl_values)*40);
    dhtxx.buffer[0] = 0;
    dhtxx.buffer[1] = 0;
    dhtxx.buffer[2] = 0;
    dhtxx.buffer[3] = 0;
    dhtxx.buffer[4] = 0;
    /* Genero el pulso para solicitar datos */
    DHTxx_DATA_TRIS = 0;
    DHTxx_DATA_OUT = 0;
    /* Cada DHT11_REFRESH_DELAY pongo en out en canal de datos 
       y mando un cero por 20ms (con 18ms alcanza) */
    __delay_ms(1);
    /* Fin del pulso, pongo el canal como input, el pull-up deberia mantenerlo arriba */
    DHTxx_DATA_TRIS = 1;
    /* Inicio parte crítica */
    INTERRUPT_GlobalDisable();    
    /* Espera el start */
    DHTxx_bitLen = 0;
    while( DHTxx_DATA_IN == 0 && ++DHTxx_bitLen < DHTxx_MAX_BIT_COUNT)
    {
        Nop();Nop();Nop();Nop();Nop();
        Nop();Nop();Nop();Nop();Nop();
    }
    if(DHTxx_bitLen == DHTxx_MAX_BIT_COUNT)
    {
        INTERRUPT_GlobalEnable();
        LogMessage("[DHT] start overflow 0");
        return 0;
    }
    DHTxx_bitLen = 0;
    while(DHTxx_DATA_IN == 1 && ++DHTxx_bitLen < DHTxx_MAX_BIT_COUNT)
    {
        Nop();Nop();Nop();Nop();Nop();
        Nop();Nop();Nop();Nop();Nop();
    }
    if(DHTxx_bitLen == DHTxx_MAX_BIT_COUNT)
    {
        INTERRUPT_GlobalEnable();
        LogMessage("[DHT] start overflow 1");
        return 0;
    }
    /* Fin del start */
    /* Salteo el header */
    DHTxx_bitLen = 0;
    while( DHTxx_DATA_IN == 0 && ++DHTxx_bitLen < DHTxx_MAX_BIT_COUNT)
    {
        Nop();Nop();Nop();Nop();Nop();
        Nop();Nop();Nop();Nop();Nop();
    }
    if(DHTxx_bitLen == DHTxx_MAX_BIT_COUNT)
    {
        INTERRUPT_GlobalEnable();
        LogMessage("[DHT] header overflow 0");
        return (-1);
    }
    DHTxx_bitLen = 0;
    while(DHTxx_DATA_IN == 1 && ++DHTxx_bitLen < DHTxx_MAX_BIT_COUNT)
    {
        Nop();Nop();Nop();Nop();Nop();
        Nop();Nop();Nop();Nop();Nop();
    }
    if(DHTxx_bitLen == DHTxx_MAX_BIT_COUNT)
    {
        INTERRUPT_GlobalEnable();
        LogMessage("[DHT] header overflow 1");
        return (-1);
    }
    /* Fin del header */
    /* Aca arrancan los datos */
    for(b = 0; b < 40; b++)
    {
        /* Mientras esta abajo o error */
        while( DHTxx_DATA_IN == 0 && ++v[b].l < DHTxx_MAX_BIT_COUNT);
        while( DHTxx_DATA_IN == 1 && ++v[b].h < DHTxx_MAX_BIT_COUNT);
        if(v[b].l >= DHTxx_MAX_BIT_COUNT || v[b].h >= DHTxx_MAX_BIT_COUNT) break;
    }
    INTERRUPT_GlobalEnable();
    /* Fin parte crítica */
    if(b < 40)
    {
        LogMessage("[DHT] overflow en datos. H: %d L: %d Bit: %d",
                v[b].h, v[b].l, b);
        return (-1);
    }
    for(b = 0; b < 40; b++)
    {
        if(v[b].h > v[b].l)
        {
            dhtxx.buffer[b/8] |= 1 << (7 - (b%8));
        }
    }    
    /* Fin de datos */
    /* Controlo el checksum */
    if(dhtxx.buffer[4] == (0xFF&(dhtxx.buffer[0]+dhtxx.buffer[1]+dhtxx.buffer[2]+dhtxx.buffer[3])))
    {
        /* Todo OK, lo paso a las variables correspondientes */
        LogMessage("[DHT] %02x %02x %02x %02x %02x",
                    dhtxx.buffer[0],dhtxx.buffer[1],dhtxx.buffer[2],dhtxx.buffer[3],dhtxx.buffer[4]);
        LogMessage("[DHT] HR: %d.%d %% - Temp: %d.%d °C",
                    DHTxxHumedad(0,0)/10, DHTxxHumedad(0,0)%10,
                    DHTxxTemperatura(0,0)/10, DHTxxTemperatura(0,0)%10);
        return 1;
    }
    else
    {
        LogMessage("[DHT] Error de CHKSum %02X %02X %02X %02X %02X",
                dhtxx.buffer[0], dhtxx.buffer[1], dhtxx.buffer[2],
                dhtxx.buffer[3], dhtxx.buffer[4]);
        return (-1);
    }
}
