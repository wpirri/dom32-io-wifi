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
unsigned int QueueOpen(unsigned int elements, unsigned int len);
void QueueClose(unsigned int id);
bool QueueAdd(unsigned int id, void* data);
bool QueueView(unsigned int id, void** data);
bool QueueGet(unsigned int id, void* data);
bool QueueDel(unsigned int id);
bool QueueClean(unsigned int id);
unsigned int QueueOpens( void );
unsigned int QueueCount(unsigned int id);
unsigned int QueueMaxCount(unsigned int id);

#endif	/* _QUEUE_H_ */

