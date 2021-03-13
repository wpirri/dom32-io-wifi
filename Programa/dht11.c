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

unsigned char DHT11_temperatura;
unsigned char DHT11_humedad;
unsigned int DHT11_refresh_delay;
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
    /* Salida para alimentar el sensor */
    #ifdef DHT11_POWER_TRIS
    SetOutput(DHT11_POWER_TRIS, DHT11_POWER_BIT);
    /* Enciendo el sensor */
    SetBit(DHT11_POWER_OUT, DHT11_POWER_BIT);
    #endif
    DHT11_DATA_TRIS = 1;
    DHT11_DATA_PULLUP = 1;

  
    DHT11_temperatura = 0;
    DHT11_humedad = 0;

    DHT11_STM = DHT11_INIT;
    DHT11_refresh_delay = 0;
    DHT11_timeout_delay = 0;
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
unsigned char __attribute__((optimize("-O0"))) DHT11Task( void )
{
    switch(DHT11_STM)
    {
        case DHT11_INIT:
            //LED = 0;
            DHT11_bitCount = 0;

            DHT11_bitCount = 0;
            DHT11_highlen = 0;
            DHT11_lowlen = 0;

            dht11.buffer[0] = 0;
            dht11.buffer[1] = 0;
            dht11.buffer[2] = 0;
            dht11.buffer[3] = 0;
            dht11.buffer[4] = 0;
            DHT11_STM++;
            break;
        case DHT11_NONE:
            break;
        case DHT11_WAIT:
            /* Espera el start */
            if(DHT11_DATA_IN) DHT11_STM++;
            break;
        case DHT11_START:
            /* Salteo el header */
            while(!DHT11_DATA_IN && ++DHT11_lowlen < MAX_BIT_LEN);
            if(DHT11_lowlen > MAX_BIT_LEN)
            {
                DHT11_STM = DHT11_ERROR;
                break;
            }
            while(DHT11_DATA_IN && ++DHT11_highlen < MAX_BIT_LEN);
            if(DHT11_highlen > MAX_BIT_LEN)
            {
                DHT11_STM = DHT11_ERROR;
                break;
            }
            /* Ac� arrancan los datos */
            //LED = 1;
            DHT11_highlen = 0;
            DHT11_lowlen = 0;
            DHT11_bitCount = 0;
            while(DHT11_bitCount < 40)
            {
                /* Mientras est� abajo o error */
                while( !DHT11_DATA_IN && ++DHT11_lowlen < MAX_BIT_LEN);
                while(  DHT11_DATA_IN && ++DHT11_highlen < MAX_BIT_LEN);
                if(DHT11_highlen > MAX_BIT_LEN || DHT11_lowlen > MAX_BIT_LEN) break;
                
                if(DHT11_highlen > DHT11_lowlen)
                {
                    dht11.buffer[DHT11_bitCount/8] |= 1 << (7 - (DHT11_bitCount%8));
                }
                DHT11_highlen = 0;
                DHT11_lowlen = 0;
                DHT11_bitCount++;
            }
            //LED = 0;
            if(DHT11_highlen > MAX_BIT_LEN || DHT11_lowlen > MAX_BIT_LEN) DHT11_STM = DHT11_ERROR;
            else DHT11_STM++;
            break;
        case DHT11_CONTROL:
            /* Controlo el checksum */
            dht11.buffer[4] -= dht11.buffer[3];
            dht11.buffer[4] -= dht11.buffer[2];
            dht11.buffer[4] -= dht11.buffer[1];
            dht11.buffer[4] -= dht11.buffer[0];
            
            if(dht11.buffer[4] == 0)
            {
                DHT11_temperatura = dht11.data.temp_ent;
                DHT11_humedad = dht11.data.hum_ent;
            }
                
            DHT11_STM = DHT11_INIT;
            break;
        case DHT11_ERROR:
            DHT11_STM = DHT11_INIT;
            break;
    }
    return ((DHT11_STM<=DHT11_NONE)?0:1);
}

/* Debe ser llamado 10 veces x seg */
void DHT11Timer( void )
{
    DHT11_refresh_delay++;

    if(DHT11_refresh_delay >= DHT11_REFRESH_DELAY)
    {
        DHT11_refresh_delay = 0;
        /* Genero el pulso para solicitar datos */
        DHT11_DATA_TRIS = 0;
        DHT11_DATA_OUT = 0;
        /* Cada DHT11_REFRESH_DELAY pongo en out en canal de datos 
           y mando un cero por 20ms (con 18ms alcanza) */
        __delay_ms(20);
        /* Fin del pulso, pongo el canal como input, el pull-up deberia mantenerlo arriba */
        DHT11_DATA_TRIS = 1;
        /*  */
        DHT11_STM = DHT11_WAIT;
        DHT11_timeout_delay = 5;
    }
    
    if(DHT11_timeout_delay)
    {
        DHT11_timeout_delay--;
        if(DHT11_timeout_delay == 0)
        {
            DHT11_STM = DHT11_INIT;
        }
    }
    
}

#ifdef __COMMENT__
void __attribute__((optimize("-O0"))) DHT11GetData( void )
{
    /* Genero el pulso para solicitar datos */
    DHT11_DATA_TRIS = 0;
    DHT11_DATA_OUT = 0;
    /* Cada DHT11_REFRESH_DELAY pongo en out en canal de datos 
       y mando un cero por 20ms (con 18ms alcanza) */
    __delay_ms(20);
    /* Fin del pulso, pongo el canal como input, el pull-up deberia mantenerlo arriba */
    DHT11_DATA_TRIS = 1;
    /* mientras espero inicializo las vaiables */
    DHT11_bitCount = 0;

    DHT11_bitCount = 0;
    DHT11_highlen = 0;
    DHT11_lowlen = 0;

    dht11.buffer[0] = 0;
    dht11.buffer[1] = 0;
    dht11.buffer[2] = 0;
    dht11.buffer[3] = 0;
    dht11.buffer[4] = 0;
    
    /* Espera el start */
    DHT11_timeout_delay = 100000;
    while( !DHT11_DATA_IN && --DHT11_timeout_delay > 0);
    if(!DHT11_DATA_IN)
    {
        return; /* timer-out esperando el START */
    }
    DHT11_timeout_delay = 100;
    while(DHT11_DATA_IN && --DHT11_timeout_delay > 0);
    if(DHT11_DATA_IN)
    {
        return; /* timer-out esperando el fin del START */
    }
    /* Fin del start */

    /* Salteo el header */
    while(!DHT11_DATA_IN && ++DHT11_lowlen < MAX_HEAD_LEN);
    if(DHT11_lowlen > MAX_BIT_LEN)
    {
        return; /* Time-out en el header */
    }
    while(DHT11_DATA_IN && ++DHT11_highlen < MAX_HEAD_LEN);
    if(DHT11_highlen > MAX_BIT_LEN)    {
        return; /* Time-out en el header */
    }
    /* Fin del header */
    
    /* Aca arrancan los datos */
    //LED = 1;
    DHT11_highlen = 0;
    DHT11_lowlen = 0;
    DHT11_bitCount = 0;
    while(DHT11_bitCount < 40)
    {
        /* Mientras est� abajo o error */
        while( !DHT11_DATA_IN && ++DHT11_lowlen < MAX_BIT_LEN);
        while(  DHT11_DATA_IN && ++DHT11_highlen < MAX_BIT_LEN);
        if(DHT11_highlen > MAX_BIT_LEN || DHT11_lowlen > MAX_BIT_LEN) break;

        if(DHT11_highlen > DHT11_lowlen)
        {
            dht11.buffer[DHT11_bitCount/8] |= 1 << (7 - (DHT11_bitCount%8));
        }
        DHT11_highlen = 0;
        DHT11_lowlen = 0;
        DHT11_bitCount++;
    }
    //LED = 0;
    if(DHT11_highlen > MAX_BIT_LEN || DHT11_lowlen > MAX_BIT_LEN)
    {
        return;
    }
    /* Fin de datos */
    
    /* Controlo el checksum */
    dht11.buffer[4] -= dht11.buffer[3];
    dht11.buffer[4] -= dht11.buffer[2];
    dht11.buffer[4] -= dht11.buffer[1];
    dht11.buffer[4] -= dht11.buffer[0];

    if(dht11.buffer[4] == 0)
    {
        /* Todo OK, lo paso a las variables correspondientes */
        DHT11_temperatura = dht11.data.temp_ent;
        DHT11_humedad = dht11.data.hum_ent;
    }
}

#endif /* __COMMENT__ */

unsigned char DHT11Temperatura( void )
{
    return DHT11_temperatura;
}

unsigned char DHT11Humedad( void )
{
    return DHT11_humedad;
}
