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
#include "wifi.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "mcc_generated_files/uart1.h"

#define WIFI_BUFFER_LEN 4096

int wifi_count_rx_timer;
bool wifi_flag_rx_timer;
bool wifi_flag_rx_lf;

char wifi_buffer_txrx[WIFI_BUFFER_LEN+1];

/* *****************************************************************************
 * 
 * ************************************************************************** */
void WiFiInit( void )
{
    wifi_count_rx_timer = 0;
    wifi_flag_rx_timer = false;
    wifi_flag_rx_lf = true;


}

/* *****************************************************************************
 * 
 * ************************************************************************** */
void WiFiTask( void )
{
    int i = 0;
    
    if(wifi_flag_rx_timer || wifi_flag_rx_lf)
    {
        wifi_flag_rx_timer = false;
        wifi_flag_rx_lf = false;
        while( UART1_IsRxReady() && i < WIFI_BUFFER_LEN)
        {
            wifi_buffer_txrx[i] = UART1_Read();
            i++;
        }
        if(i > 0)
        {
            wifi_buffer_txrx[i] = 0;
            printf("Leidos %i bites\r\n", i);
            
            
            
        }
    }
}

/* *****************************************************************************
 *    Se llama 10 veces por egundo
 * ************************************************************************** */
void WiFiTimer( void )
{
    if(wifi_count_rx_timer)
    {
        wifi_count_rx_timer--;
        if(wifi_count_rx_timer==0)
        {
            wifi_flag_rx_timer = true;
            printf("Timer de rx vencido\r\n");
        }
    }

    
    
}

/* *****************************************************************************
 *    Se llama con caha char que entra por el serial
 * ************************************************************************** */
void WiFiNewChar(char ch)
{
    wifi_count_rx_timer = 5; /* 500 ms */
    if(ch == 0x0a)
    {
        wifi_flag_rx_lf = true;
        printf("Recibido un Line feed\r\n");
    }
    
}

/* ************************************************************************* */
