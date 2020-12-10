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
    bool change1;
    unsigned int pulse1;
    bool dir2;
    bool ana2;
    bool stat2;
    bool change2;
    unsigned int pulse2;
    bool dir3;
    bool ana3;
    bool stat3;
    bool change3;
    unsigned int pulse3;
    bool dir4;
    bool ana4;
    bool stat4;
    bool change4;
    unsigned int pulse4;
}IO;

typedef struct
{
    bool dir1;      /* true = input, false = output */
    bool ana1;
    bool stat1;
    bool change1;
    unsigned int pulse1;
    bool dir3;
    bool ana3;
    bool stat3;
    bool change3;
    unsigned int pulse3;
    bool dir4;
    bool ana4;
    bool stat4;
    bool change4;
    unsigned int pulse4;
    bool dir5;
    bool ana5;
    bool stat5;
    bool change5;
    unsigned int pulse5;
    bool dir7;
    bool ana7;
    bool stat7;
    bool change7;
    unsigned int pulse7;
    bool dir8;
    bool ana8;
    bool stat8;
    bool change8;
    unsigned int pulse8;
    bool dir9;
    bool ana9;
    bool stat9;
    bool change9;
    unsigned int pulse9;
 }EXP;

#ifdef _IN_CONTROL_C_
    IO g_control_io;
    EXP g_control_exp;
#else
    extern IO g_control_io;
    extern EXP g_control_exp;
#endif

void CTRLInit( void );
void CTRLTask( void );
void CTRLTimer( void );

void CTRLSetIO(char* in);
void CTRLConfIO(char* in);

#endif /* _CONTROL_H_ */
