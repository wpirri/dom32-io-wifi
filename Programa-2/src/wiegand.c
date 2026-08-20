/***************************************************************************
    Copyright (C) 2015   Walter Pirri

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
#include "wiegand.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "control.h"
#include "config.h"
#include "log.h"

#define DEBUG_WIEGAND

typedef struct _wgnd_data
{
    union
    {
        struct
        {
            unsigned char h;
            unsigned char g;
            unsigned char f;
            unsigned char e;
            unsigned char d;
            unsigned char c;
            unsigned char b;
            unsigned char a;
        } bytes;
        int64_t i64data;
    } udata;
} wgnd_data;

volatile unsigned char gWGND_TimerCount;
volatile unsigned char gWGND_BitCount;
wgnd_data gWGND_Data;

void WGND_Ack( void )
{
    /* Reseteo todo para una nueva recepci�n */
    gWGND_BitCount = 0;
    gWGND_Data.udata.i64data = 0L;
}

void WGND_Init( void )
{
    gWGND_TimerCount = 0;
    gWGND_BitCount = 0;
    gWGND_Data.udata.i64data = 0L;

    if(g_wifi_config.u.config.enable_wiegand == 1)
    {
        /****************************************************************************
         * Interrupt On Change: negative
         ***************************************************************************/
        CNEN1Bbits.CNIE1B7 = 1;    //Pin : RB7
        CNEN1Bbits.CNIE1B8 = 1;    //Pin : RB8
        /****************************************************************************
         * Interrupt On Change: flag
         ***************************************************************************/
        CNFBbits.CNFB7 = 0;    //Pin : RB7
        CNFBbits.CNFB8 = 0;    //Pin : RB8
        /****************************************************************************
         * Interrupt On Change: config
         ***************************************************************************/
        CNCONBbits.CNSTYLE = 1;    //Config for PORTB
        CNCONBbits.ON = 1;    //Config for PORTB

        /****************************************************************************
         * Interrupt On Change: Interrupt Enable
         ***************************************************************************/
        IFS0CLR= 1 << _IFS0_CNBIF_POSITION; //Clear CNBI interrupt flag
        IEC0bits.CNBIE = 1; //Enable CNBI interrupt

        /* Entrada 5 - RB7 */
        TRISBbits.TRISB7 = 1;
        CNPUBbits.CNPUB7 = 1;
        /* Entrada 6 - RB8 */
        TRISBbits.TRISB8 = 1;
        CNPUBbits.CNPUB8 = 1;
        /* Control de interrupcion */
        //    CNBI: PORT B Change Notification
        //    Priority: 1
        IPC2bits.CNBIP = 1;
        //    Sub Priority: 0
        IPC2bits.CNBIS = 0;
        //Clear CNBI interrupt flag
        IFS0CLR= 1 << _IFS0_CNBIF_POSITION;
        //Enable CNBI interrupt
        IEC0bits.CNBIE = 1;
    }
    else
    {
        //Disable CNBI interrupt
        IEC0bits.CNBIE = 0;
    }
}

/*  WGND_Timer
 *      Se lo llama 100 veces por segundo
 */
void WGND_Timer( void )
{
    char cardHexa[17];
    
    if(g_wifi_config.u.config.enable_wiegand == 0) return;
    
    if(gWGND_TimerCount)
    {
        gWGND_TimerCount--;
        if( !gWGND_TimerCount)
        {
#ifdef DEBUG_WIEGAND            
            LogMessage("[WIEGAND] bit-count: %d", gWGND_BitCount);
#endif
            if(gWGND_BitCount == 26 || gWGND_BitCount == 34)
            {
                /* desplazo los bits para alinearlos a 64 */
                //gWGND_Data.udata.i64data >>= (64-gWGND_BitCount);
                /* los 6 bits mas significativos no van */
                //gWGND_Data.udata.i64data &= 0x3FFFFFFF;

                sprintf(cardHexa, "%02X%02X%02X%02X%02X%02X%02X%02X",
                        gWGND_Data.udata.bytes.a,
                        gWGND_Data.udata.bytes.b,
                        gWGND_Data.udata.bytes.c,
                        gWGND_Data.udata.bytes.d,
                        gWGND_Data.udata.bytes.e,
                        gWGND_Data.udata.bytes.f,
                        gWGND_Data.udata.bytes.g,
                        gWGND_Data.udata.bytes.h);
                LogMessage("[WIEGAND] CRD: %s", cardHexa);
                ControlNotificarTarjeta(cardHexa);
            }
            WGND_Ack();
        }
    }
}

/*  WGND_IntData
 *      Se la llama desde la interrupcion por cambios en PORT
 */
void WGND_IntData( unsigned char d0, unsigned char d1 )
{
    if(g_wifi_config.u.config.enable_wiegand == 0) return;
    if(d0 == d1) return;

    gWGND_Data.udata.i64data >>= 1;
    
    if(d1)  gWGND_Data.udata.i64data |= 0x80000000;
    else    gWGND_Data.udata.i64data &= 0x7FFFFFFF;

    gWGND_BitCount++;
    gWGND_TimerCount = 2;
}

/* Interrupt service routine for the CNBI interrupt. */
void __attribute__ ((vector(_CHANGE_NOTICE_B_VECTOR), interrupt(IPL1SOFT))) _CHANGE_NOTICE_B( void )
{
    if(IFS0bits.CNBIF == 1)
    {
        if(g_wifi_config.u.config.enable_wiegand == 1)
        {
            WGND_IntData( CNFBbits.CNFB8, CNFBbits.CNFB7 );
        }
        if(CNFBbits.CNFB8 == 1)
        {
            CNFBCLR = 0x100;  //Clear CNFBbits.CNFB8
        }
        if(CNFBbits.CNFB7 == 1)
        {
            CNFBCLR = 0x80;  //Clear CNFBbits.CNFB7
        }
        // Clear the flag
        IFS0CLR= 1 << _IFS0_CNBIF_POSITION; // Clear IFS0bits.CNBIF
    }
}
