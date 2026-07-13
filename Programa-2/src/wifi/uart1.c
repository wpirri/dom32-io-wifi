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
#include "uart1.h"

#include <xc.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define WIFI_UART_RX_BYTEQ_LENGTH 4096

static uint8_t * volatile g_uart1_rxTail;
static uint8_t * g_uart1_rxHead;
static uint8_t * volatile g_uart1_txHead;
static bool volatile g_uart1_rxOverflowed;
static uint8_t g_uart1_rxQueue[WIFI_UART_RX_BYTEQ_LENGTH+1] ;

void UART1_Init(void)
{
    g_uart1_rxHead = g_uart1_rxQueue;
    g_uart1_rxTail = g_uart1_rxQueue;
    g_uart1_rxOverflowed = 0;

    /* ********************************************************************* */
    /* Configuracion de los pines */
    // Tx
    TRISBbits.TRISB14 = 0;
    // Rx
    TRISBbits.TRISB15 = 1;
    /* ********************************************************************* */
    /* Configuracion de interrupcion */
    //    UERI: UART 1 Error
    //    Priority: 1
        IPC13bits.U1EIP = 1;
    //    Sub Priority: 0
        IPC13bits.U1EIS = 0;
    //    UTXI: UART 1 Transmission
    //    Priority: 1
        IPC13bits.U1TXIP = 1;
    //    Sub Priority: 0
        IPC13bits.U1TXIS = 0;
    //    URXI: UART 1 Reception
    //    Priority: 1
        IPC13bits.U1RXIP = 1;
    //    Sub Priority: 0
        IPC13bits.U1RXIS = 0;
    /* Configuracion del UART */
    
    /* ********************************************************************* */
    /* Configuracion del la UART */
    /* ************************************************************************** */
    // System Reg Unlock
    SYSKEY = 0x00000000; 
    SYSKEY = 0xAA996655; //write Key1 to SYSKEY
    SYSKEY = 0x556699AA; //write Key2 to SYSKEY
    /* ************************************************************************** */

	IEC1bits.U1TXIE = 0;
	IEC1bits.U1RXIE = 0;

	// STSEL 1; PDSEL 8N; RTSMD disabled; OVFDIS disabled; ACTIVE disabled; RXINV disabled; WAKE disabled; BRGH enabled; IREN disabled; ON enabled; SLPEN disabled; SIDL disabled; ABAUD disabled; LPBACK disabled; UEN TX_RX; CLKSEL PBCLK; 
	U1MODE = (0x8008 & ~(1<<15));  // disabling UART
	// UTXISEL TX_ONE_CHAR; UTXINV disabled; ADDR 0; MASK 0; URXEN disabled; OERR disabled; URXISEL RX_ONE_CHAR; UTXBRK disabled; UTXEN disabled; ADDEN disabled; 
	U1STA = 0x0;
	// BaudRate = 115200; Frequency = 24000000 Hz; BRG 51; 
	U1BRG = 0x33;

	IEC1bits.U1RXIE = 1;

	//Make sure to set LAT bit corresponding to TxPin as high before UART initialization
	U1STASET = _U1STA_UTXEN_MASK;
	U1MODESET = _U1MODE_ON_MASK;  // enabling UART ON bit
	U1STASET = _U1STA_URXEN_MASK; 

    /* ************************************************************************** */
    // System Reg Lock
	SYSKEY = 0x00000000; 
}

bool UART1_IsRxReady(void)
{    
    return !(g_uart1_rxHead == g_uart1_rxTail);
}


void UART1_Write(unsigned int ch)
{
    while(U1STAbits.UTXBF);
    U1TXREG = ch;
}

unsigned char UART1_Read( void)
{
    unsigned char data = 0;

    while (g_uart1_rxHead == g_uart1_rxTail );
    
    data = *g_uart1_rxHead;

    g_uart1_rxHead++;

    if (g_uart1_rxHead == (g_uart1_rxQueue + WIFI_UART_RX_BYTEQ_LENGTH))
    {
        g_uart1_rxHead = g_uart1_rxQueue;
    }
    return data;
}

/* ************************************************************************* */

void __attribute__ ((vector(_UART1_RX_VECTOR), interrupt(IPL1SOFT))) _UART1_RX( void )
{
    while((U1STAbits.URXDA == 1))
    {
        *g_uart1_rxTail = U1RXREG;

        // Will the increment not result in a wrap and not result in a pure collision?
        // This is most often condition so check first
        if ( ( g_uart1_rxTail    != (g_uart1_rxQueue + WIFI_UART_RX_BYTEQ_LENGTH-1)) &&
             ((g_uart1_rxTail+1) != g_uart1_rxHead) )
        {
            g_uart1_rxTail++;
        } 
        else if ( (g_uart1_rxTail == (g_uart1_rxQueue + WIFI_UART_RX_BYTEQ_LENGTH-1)) &&
                  (g_uart1_rxHead !=  g_uart1_rxQueue) )
        {
            // Pure wrap no collision
            g_uart1_rxTail = g_uart1_rxQueue;
        } 
        else // must be collision
        {
            g_uart1_rxOverflowed = true;
        }

    }

    IFS1CLR= 1 << _IFS1_U1RXIF_POSITION;
}

bool UART1_Overflow()
{
    unsigned char c;
    
    if(g_uart1_rxOverflowed)
    {
        g_uart1_rxHead = g_uart1_rxQueue;
        g_uart1_rxTail = g_uart1_rxQueue;
        while((U1STAbits.URXDA == 1)) { c = U1RXREG; }
        c = c;
        g_uart1_rxOverflowed = false;
        return true;
    }
    return false;
}

void __attribute__ ((vector(_UART1_ERR_VECTOR), interrupt(IPL1SOFT))) _UART1_ERR ( void )
{
    unsigned char c;

    if ((U1STAbits.OERR == 1))
    {
        U1STACLR = _U1STA_OERR_MASK; 
    }
    
    g_uart1_rxHead = g_uart1_rxQueue;
    g_uart1_rxTail = g_uart1_rxQueue;
    while((U1STAbits.URXDA == 1)) { c = U1RXREG; }
    c = c;
    IFS1CLR= 1 << _IFS1_U1EIF_POSITION;
}
