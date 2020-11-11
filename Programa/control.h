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
#ifndef _CONTROL_H_
#define _CONTROL_H_

#include <stdbool.h>

typedef struct
{
    bool dir1;      /* true = input, false = output */
    bool ana1;
    bool stat1;
    unsigned int pulse1;
    bool dir2;
    bool ana2;
    bool stat2;
    unsigned int pulse2;
    bool dir3;
    bool ana3;
    bool stat3;
    unsigned int pulse3;
    bool dir4;
    bool ana4;
    bool stat4;
    unsigned int pulse4;
}IO;

#ifdef _IN_CONTROL_C_
    IO g_control_io;
#else
    extern IO g_control_io;
#endif

void CTRLInit( void );
void CTRLTask( void );
void CTRLTimer( void );

void CTRLSetIO(char* in);
void CTRLConfIO(char* in);

#endif /* _CONTROL_H_ */
