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

#define DELAY_INPUT 1000

typedef struct
{
    unsigned int stat1;
    bool change1;
    unsigned int pulse1;
    unsigned int stat2;
    bool change2;
    unsigned int pulse2;
    unsigned int stat3;
    bool change3;
    unsigned int pulse3;
    unsigned int stat4;
    bool change4;
    unsigned int pulse4;
    unsigned int stat5;
    bool change5;
    unsigned int pulse5;
    unsigned int stat6;
    bool change6;
    unsigned int pulse6;
}IO;

typedef struct
{
    bool stat1;
    bool change1;
    unsigned int pulse1;
    bool stat2;
    bool change2;
    unsigned int pulse2;
    bool stat3;
    bool change3;
    unsigned int pulse3;
    bool stat4;
    bool change4;
    unsigned int pulse4;
 }OUT;

#ifdef _IN_CONTROL_C_
    IO g_control_io;
    OUT g_control_out;
#else
    extern IO g_control_io;
    extern OUT g_control_out;
#endif

void CTRLInit( void );
void CTRLTask( void );
void CTRLTimer( void );

void CTRLSetIO(char* in);
void CTRLSwitchIO(char* in);
void CTRLConfIO(char* in);

#endif /* _CONTROL_H_ */
