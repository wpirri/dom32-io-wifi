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
#ifndef _DHT11_H_
#define	_DHT11_H_

#define DHT11_DATA_TRIS
#define DHT11_DATA_IO  
#define DHT11_DATA_IN  

#define DHT11_REFRESH_DELAY 600

void DHT11Init( void );
void DHT11Task( void );
void DHT11Timer( void );

int DHT11Temperatura( void );
int DHT11Humedad( void );

#endif	/* _DHT11_H_ */

