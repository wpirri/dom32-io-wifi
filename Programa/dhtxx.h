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
#ifndef _DHTxx_H_
#define	_DHTxx_H_

//#define DHTxx_POWER_TRIS    TRISBbits.TRISB6  
#define DHTxx_POWER_OUT     LATBbits.LATB6
//#define DHTxx_POWER_IN      PORTBbits.RB6

#define DHTxx_DATA_TRIS   TRISAbits.TRISA2
#define DHTxx_DATA_OUT    LATAbits.LATA2
#define DHTxx_DATA_IN     PORTAbits.RA2
#define DHTxx_DATA_PULLUP CNPUAbits.CNPUA2

#define DHTxx_REFRESH_DELAY 100

void DHTxxInit( void );
void DHTxxGetData();

/* Temp: 1000 = 10,00 °C */
int DHTxxTemperatura( void );
/* Hum: 1000 = 10,00 % */
int DHTxxHumedad( void );

#endif	/* _DHTxx_H_ */

