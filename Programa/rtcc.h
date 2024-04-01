/***************************************************************************
    Copyright (C) 2017   Walter Pirri

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
#ifndef _RTCC_H_
#define _RTCC_H_

#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#define RTCC_DEFAULT_DIV    15998       /* 2Hz con Xtal de 8MHz */
#define RTCC_DEFAULT_FDIV   15          /* Mitad del rango - 0..31 */

#ifdef _IN_RTCC_C_
unsigned char g_get_time_flag;
#else
extern unsigned char g_get_time_flag;
#endif /* _IN_RTCC_C_ */

void RTCCInit(void);

bool RTCC_TimeGet(struct tm *currentTime);
void RTCC_TimeSet(struct tm *initialTime);
void RTCC_TimeReset(bool reset);
void RTCC_AdjustClk(int diferencia);

void RTCC_SetAdjust(int div, int fdiv);

/* Se llama dos veces por segundo */
void RTCC_Task( void );

#endif // _RTCC_H_
