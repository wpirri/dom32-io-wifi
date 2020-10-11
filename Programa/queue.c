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
#include "queue.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

typedef struct _QUEUE_ELEMENT
{
    unsigned char first;
    unsigned char last;
    unsigned char count;
    unsigned char max_count;
    unsigned char elements;
    unsigned int len;
    void *buffer;
} QUEUE_ELEMENT;

QUEUE_ELEMENT g_queue_list_buffer[MAX_QUEUE_COUNT];

void QueueNextItem(unsigned int id, unsigned char *idx);

void QueueInit( void )
{
    unsigned int i;
    
    for(i = 0;i < MAX_QUEUE_COUNT; i++)
    {
        g_queue_list_buffer[i].buffer = NULL;
    }
}

unsigned int QueueOpen(unsigned int elements, unsigned int len)
{
    unsigned int i;
    
    for(i = 0;i < MAX_QUEUE_COUNT; i++)
    {
        if(g_queue_list_buffer[i].buffer == NULL) break;
    }
    if(i < MAX_QUEUE_COUNT)
    {
        g_queue_list_buffer[i].buffer = calloc(elements, len);
        if(g_queue_list_buffer[i].buffer)
        {
			g_queue_list_buffer[i].elements = elements;
			g_queue_list_buffer[i].len = len;
			g_queue_list_buffer[i].first = 0;
			g_queue_list_buffer[i].last = 0;
			g_queue_list_buffer[i].count = 0;
			g_queue_list_buffer[i].max_count = 0;
			return i;
        }
    }
    return (INVALID_QUEUE);
}

void QueueClose(unsigned int id)
{
    if(id < MAX_QUEUE_COUNT)
    {
        free(g_queue_list_buffer[id].buffer);
        g_queue_list_buffer[id].buffer = NULL;
    }    
}

bool QueueAdd(unsigned int id, void* data)
{
    if(id < MAX_QUEUE_COUNT)
    {
        if(g_queue_list_buffer[id].count < g_queue_list_buffer[id].elements)
        {
            memcpy((g_queue_list_buffer[id].buffer + (g_queue_list_buffer[id].last * g_queue_list_buffer[id].len)),
                    data,
                    g_queue_list_buffer[id].len);
            QueueNextItem(id, &g_queue_list_buffer[id].last);
            g_queue_list_buffer[id].count++;
            if(g_queue_list_buffer[id].count > g_queue_list_buffer[id].max_count)
            {
            	g_queue_list_buffer[id].max_count = g_queue_list_buffer[id].count;
            }

            return true;
        }
    }    
    return false;
}

bool QueueView(unsigned int id, void** data)
{
    if(id < MAX_QUEUE_COUNT)
    {
        if(g_queue_list_buffer[id].count)
        {
            *data =  (g_queue_list_buffer[id].buffer + (g_queue_list_buffer[id].first * g_queue_list_buffer[id].len));
            return true;
        }
    }    
    return false;
}

bool QueueGet(unsigned int id, void* data)
{
    if(id < MAX_QUEUE_COUNT)
    {
        if(g_queue_list_buffer[id].count)
        {
            memcpy( data,
                    (g_queue_list_buffer[id].buffer + (g_queue_list_buffer[id].first * g_queue_list_buffer[id].len)),
                    g_queue_list_buffer[id].len);
            QueueNextItem(id, &g_queue_list_buffer[id].first);
            g_queue_list_buffer[id].count--;
            return true;
        }
    }    
    return false;
}

bool QueueDel(unsigned int id)
{
    if(id < MAX_QUEUE_COUNT)
    {
        if(g_queue_list_buffer[id].count)
        {
            QueueNextItem(id, &g_queue_list_buffer[id].first);
            g_queue_list_buffer[id].count--;
            return true;
        }
    }    
    return false;
}

bool QueueClean(unsigned int id)
{
    if(id < MAX_QUEUE_COUNT)
    {
        g_queue_list_buffer[id].first = 0;
        g_queue_list_buffer[id].last = 0;
        g_queue_list_buffer[id].count = 0;
        return true;
    }
    return false;
}

unsigned int QueueOpens( void )
{
    unsigned int i;
    unsigned int count = 0;

    for(i = 0;i < MAX_QUEUE_COUNT; i++)
    {
        if(g_queue_list_buffer[i].buffer != NULL) count++;
    }
    return count;
}

unsigned int QueueCount(unsigned int id)
{
    return g_queue_list_buffer[id].count;
}

unsigned int QueueMaxCount(unsigned int id)
{
    return g_queue_list_buffer[id].max_count;
}

void QueueNextItem(unsigned int id, unsigned char *idx)
{
    (*idx)++;
    if((*idx) >= g_queue_list_buffer[id].elements) (*idx) = 0;
}
