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

#include "cconfig.h"
#include "cctrl.h"
#include "minipow.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <plib.h>

typedef struct _wgnd_data
{
    union
    {
        struct
        {
            unsigned char d;
            unsigned char c;
            unsigned char b;
            unsigned char a;
        } bytes;
        unsigned long ldata;
    } udata;
} wgnd_data;

unsigned char gWGND_DataReady;
unsigned char gWGND_TimerCount;
unsigned char gWGND_BitCount;
unsigned char gWGND_ReadFlag;
wgnd_data gWGND_Data;

void WGND_Init( void )
{
    gWGND_DataReady = 0;
    gWGND_TimerCount = 0;
    gWGND_ReadFlag = 0;
    gWGND_BitCount = 0;
    gWGND_Data.udata.ldata = 0l;

//    TRISAbits.TRISA6 = 1;
//    TRISAbits.TRISA7 = 1;

}

/*  WGND_Timer
 *      Se lo llama 10 veces por segundo
 */
void WGND_Timer( void )
{
    if(gWGND_TimerCount)
    {
        gWGND_TimerCount--;
        if( !gWGND_TimerCount)
        {
            /* Reseteo los datos que pueda haber */
            gWGND_BitCount = 0;
            gWGND_Data.udata.ldata = 0l;
            gWGND_DataReady = 0;
        }
    }
}

void WGND_Ack( void )
{
    /* Reseteo todo para una nueva recepci�n */
    gWGND_BitCount = 0;
    gWGND_Data.udata.ldata = 0l;
    gWGND_DataReady = 0;
}

/*  WGND_IntData
 *      Se la llama desde la interrupcion por cambios en PORT
 */
void WGND_IntData( unsigned char d0, unsigned char d1 )
{
    if(gWGND_DataReady) return;
    if(d0 == d1) return;

    gWGND_Data.udata.ldata >>= 1;
    
    if(!d1) gWGND_Data.udata.ldata |= 0x80000000;
    else    gWGND_Data.udata.ldata &= 0x7FFFFFFF;

    gWGND_BitCount++;

    if(gWGND_BitCount == 26)
    {
        /* desplaso los bits para alinearlos a 32 */
        gWGND_Data.udata.ldata >>= 6;
        /* los 6 bits mas significativos no van */
        gWGND_Data.udata.ldata &= 0x3FFFFFFF;

        gWGND_TimerCount = 0;
        gWGND_DataReady = 1;
    }
    else
    {
        gWGND_TimerCount = 5;
    }
}

void WGND_Exec(hw_dev *dev, const char* crd)
{
    system_user user;
    unsigned int need_access;

    if( !dev || !crd) return;
    
    user.name[0] = '\0';
    memcpy(user.card, crd, sizeof(user.card));
    if(GetUser(&user))
    {
        need_access = 0xffffffff;
        if(dev->f.typ == DEV_NAME_GMHTOUCH315)
        {
            need_access = GetNeedAccess(dev->f.id, 0, 1);
        }
        else if(dev->f.typ == DEV_NAME_GMHREM315)
        {
            need_access = GetNeedAccess(dev->f.id, 4, 1);
        }
        else if(dev->f.typ == DEV_NAME_GMHWEB32)
        {
            need_access = GetNeedAccess(dev->f.id, 6, 1);
        }
        else if(dev->f.typ == DEV_NAME_MIO16X16AD)
        {
            /* TODO: Control de acceso para MIO-16x16-AD */
        }
        
        EventAlarma("Acceso con tarjeta", user.name);

        if(CheckAccess(user.name, NULL, need_access))
        {
            LogMessage("CARD", "Tarjeta %s de usuario %s en lectora de %s.",
                    crd, user.name, dev->f.name);
#ifdef ENABLE_GMHTOUCH315
            if(dev->f.typ == DEV_NAME_GMHTOUCH315)
            {
                dev->hw.gmhtouch315.change |= 0x01;     // -> (0)
            }
            else
#endif
#ifdef ENABLE_GMHREM315
            if(dev->f.typ == DEV_NAME_GMHREM315)
            {
                dev->hw.gmhkey2.change |= 0x10;         // -> (4)
                dev->hw.gmhkey2.status |= 0x10;
            }
            else
#endif
                if(dev->f.typ == DEV_NAME_GMHWEB32)
            {
                dev->hw.gmhweb32.io[0].change |= 0x40;  // -> (6)
                dev->hw.gmhweb32.io[0].status |= 0x40;
            }
            else if(dev->f.typ == DEV_NAME_MIO16X16AD)
            {
                /* TODO: Generar evento para MIO-16x16-AD */
            }
        }
        else
        {
            LogMessage("CARD", "Usuario %s acceso no permitido en lectora de %s.",
                    user.name, dev->f.name);
        }
    }
    else
    {
        LogMessage("CARD", "Targeta %s desconocida en lectora de %s.",
                crd, dev->f.name);
        AddAlert("Tarjeta nueva: ", crd);
    }

}

void WGND_Task( void )
{
    char cardHexa[9];

    if(gWGND_DataReady)
    {
        sprintf(cardHexa, "%02X%02X%02X%02X",
                gWGND_Data.udata.bytes.a,
                gWGND_Data.udata.bytes.b,
                gWGND_Data.udata.bytes.c,
                gWGND_Data.udata.bytes.d);
        WGND_Exec(GetHardwareDevicePtr(1), cardHexa);
        WGND_Ack();
    }
    else
    {
        if(PORTAbits.RA6 != PORTAbits.RA7)
        {
            if( !gWGND_ReadFlag)
            {
                /* Cada vez que cambian los ports y son diferentes */
                WGND_IntData( PORTAbits.RA6, PORTAbits.RA7 );
                gWGND_ReadFlag = 1;
            }
        }
        else
        {
            gWGND_ReadFlag = 0;
        }
    }    
}
