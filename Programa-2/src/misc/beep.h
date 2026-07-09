/***************************************************************************
    Copyright (C) 2024   Walter Pirri

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
#ifndef _BEEP_H_
#define	_BEEP_H_

#include <stdbool.h>

#ifdef _ON_BEEP_C_
    volatile bool g_beep_flag;
#else
    extern volatile bool g_beep_flag;
#endif

void InitBeep( void );
void TimerBeep( void );

void Beep(int len);
void Beeps(int count);


#endif	/* _BEEP_H_ */

