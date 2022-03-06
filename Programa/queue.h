/***************************************************************************
    Copyright (C) 2013   Walter Pirri

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

#ifndef _QUEUE_H_
#define	_QUEUE_H_

#include <stdbool.h>

#define MAX_QUEUE_COUNT 10
#define INVALID_QUEUE   MAX_QUEUE_COUNT

void QueueInit( void );
unsigned int QueueOpen(char* buffer, unsigned int buffer_len);
void QueueClose(unsigned int id);
bool QueueAddBuffer(unsigned int id, char* data, unsigned int data_len);
/* Permite armar un objeto de a un byte antes de ecolarlo */
bool QueueAddByte(unsigned int id, char ch);
bool QueueAddByteEnqueue(unsigned int id);
void QueueAddByteCancell(unsigned int id);

unsigned int QueueView(unsigned int id, char* data, unsigned int max_data_len);
unsigned int QueueGet(unsigned int id, char* data, unsigned int max_data_len);

bool QueueDel(unsigned int id);
bool QueueClean(unsigned int id);

/* estadisticas de uso de las colas */
unsigned int QueueOpens( void );
unsigned int QueueMaxOpens( void );
unsigned int QueueElements(unsigned int id);
unsigned int QueueBufferUsed(unsigned int id);
unsigned int QueueMaxBufferUsed(unsigned int id);

#endif	/* _QUEUE_H_ */

