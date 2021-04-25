/***************************************************************************
    Copyright (C) 2018   Walter Pirri

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
#include "dht11.h"

#include <xc.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BIT_LEN 100
#define MAX_HEAD_LEN 150

int DHT11_temperatura;
int DHT11_humedad;
int DHT11_refresh_delay;
unsigned long DHT11_timeout_delay;
unsigned char DHT11_bitCount;
unsigned char DHT11_highlen;
unsigned char DHT11_lowlen;

typedef enum
{
    DHT11_INIT = 0u,
    DHT11_NONE,
    DHT11_WAIT,
    DHT11_START,
    DHT11_CONTROL,
    DHT11_ERROR
} DHT11STM;

volatile DHT11STM DHT11_STM;

typedef union _dht11
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
} dht_11;

dht_11 dht11;

void DHT11Init( void )
{

}

/*
    ____                        ____          ________       ___       _______
        |______________________|    |________|        |__+__| - |__-__|   +   |__
         <----    > 80ms    -->20/40us 80us     80us    50   30   50     70
         <-------    MCU    -------> <--- DHT ...
         <-- Solicitud de datos  --> <--- Header --->  <-  Datos
                                                       <-  0  -> <-    1    ->

    Datos: 8 bits   Parte entera de la temperatura
           8 bits   Parte decimal de la temperatura
           8 bits   Parte entera de la humedad
           8 bits   Parte decimalde la humedad
           8 bits   Checksum

*/
void DHT11Task( void )
{

}

/* Debe ser llamado 10 veces x seg */
void DHT11Timer( void )
{

}

int DHT11Temperatura( void )
{
    return DHT11_temperatura;
}

int DHT11Humedad( void )
{
    return DHT11_humedad;
}
