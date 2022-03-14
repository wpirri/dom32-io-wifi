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
//#include <stdlib.h>
//#include <limits.h>
#include <string.h>
//#include <unistd.h>

typedef struct _QUEUE_ELEMENT
{
    char* buffer;
    char* last_element;
    char* last_byte;
    unsigned int last_element_bytes;
    char* first_element;
    unsigned int element_count;
    unsigned int max_element_count;
    unsigned int buffer_used;
    unsigned int max_buffer_used;
    unsigned int total_buffer_len;
} QUEUE_ELEMENT;

volatile QUEUE_ELEMENT g_queue_list_buffer[MAX_QUEUE_COUNT];

unsigned int queue_count_opens;
unsigned int queue_count_max_opens;

char* QueueNextByte(char* buffer_root, unsigned int buffer_len, char* current_byte)
{
    current_byte++;
    if(current_byte >= (buffer_root + buffer_len))
    {
        //printf("QueueNextByte: Pegando la vuelta\n");
        //sleep(3);
        return buffer_root;
    }
    else
    {
        return current_byte;
    }
}


void QueueInit( void )
{
    unsigned int i;
    
    for(i = 0;i < MAX_QUEUE_COUNT; i++)
    {
        g_queue_list_buffer[i].buffer = NULL;
    }
    queue_count_opens = 0;
    queue_count_max_opens = 0;
}

unsigned int QueueOpen(char* buffer, unsigned int buffer_len)
{
    unsigned int i;
    
    for(i = 0;i < MAX_QUEUE_COUNT; i++)
    {
        if(g_queue_list_buffer[i].buffer == NULL)
        {
            g_queue_list_buffer[i].buffer = buffer;
            g_queue_list_buffer[i].total_buffer_len = buffer_len;
            g_queue_list_buffer[i].last_element = g_queue_list_buffer[i].buffer;
            g_queue_list_buffer[i].last_byte = g_queue_list_buffer[i].buffer;
            g_queue_list_buffer[i].last_element_bytes = 0;
            g_queue_list_buffer[i].first_element = g_queue_list_buffer[i].buffer;
            g_queue_list_buffer[i].element_count = 0;
            g_queue_list_buffer[i].max_element_count = 0;
            g_queue_list_buffer[i].buffer_used = 0;
            g_queue_list_buffer[i].max_buffer_used = 0;
            
            queue_count_opens++;
            if(queue_count_opens > queue_count_max_opens)
            {
                queue_count_max_opens = queue_count_opens;
            }
            return i;
        }
    }
    return INVALID_QUEUE;
}

void QueueClose(unsigned int id)
{
    if(id < INVALID_QUEUE && g_queue_list_buffer[id].buffer)
    {
        g_queue_list_buffer[id].buffer = NULL;
        queue_count_opens--;
    }
}

bool QueueAddBuffer(unsigned int id, char* data, unsigned int data_len)
{
    unsigned int i;
    union
    {
        char s[sizeof(unsigned int)];
        unsigned int i;
    } union_int_string;
    
    /* Adem�s de lo de siempre, que no haya un elemento empesado a cargar */
    if(id < INVALID_QUEUE && g_queue_list_buffer[id].buffer && (g_queue_list_buffer[id].last_byte == g_queue_list_buffer[id].last_element) )
    {
        if(g_queue_list_buffer[id].total_buffer_len > (g_queue_list_buffer[id].buffer_used + sizeof(unsigned int) + data_len + 1))
        {
            union_int_string.i = data_len + 1;
            for(i = 0; i < sizeof(unsigned int); i++)
            {
                g_queue_list_buffer[id].buffer_used++;
                *(g_queue_list_buffer[id].last_element) = union_int_string.s[i];
                g_queue_list_buffer[id].last_element = QueueNextByte(
                                g_queue_list_buffer[id].buffer,
                                g_queue_list_buffer[id].total_buffer_len,
                                g_queue_list_buffer[id].last_element);
            }
            while(data_len)
            {
                g_queue_list_buffer[id].buffer_used++;
                *(g_queue_list_buffer[id].last_element) = *data;
                g_queue_list_buffer[id].last_element = QueueNextByte(
                                g_queue_list_buffer[id].buffer,
                                g_queue_list_buffer[id].total_buffer_len,
                                g_queue_list_buffer[id].last_element);
                data_len--;
                data++;
            }
            /* Un byte mas al final con un nul */
            *(g_queue_list_buffer[id].last_element) = 0;
            g_queue_list_buffer[id].buffer_used++;
            g_queue_list_buffer[id].last_element = QueueNextByte(
                            g_queue_list_buffer[id].buffer,
                            g_queue_list_buffer[id].total_buffer_len,
                            g_queue_list_buffer[id].last_element);
            
            g_queue_list_buffer[id].last_element_bytes = 0;
            g_queue_list_buffer[id].last_byte = g_queue_list_buffer[id].last_element;
            
            g_queue_list_buffer[id].element_count++;
            
            if(g_queue_list_buffer[id].buffer_used > g_queue_list_buffer[id].max_buffer_used)
            {
                g_queue_list_buffer[id].max_buffer_used = g_queue_list_buffer[id].buffer_used;
            }
            return true;
        }
    }
    return false;
}

/* Permite armar un objeto de a un byte antes de ecolarlo */
bool QueueAddByte(unsigned int id, char ch)
{
    unsigned int i;

    //printf("QueueAddByte: Agregando:%c queue:%i buffer_used:%i last_element_bytes:%i - buffer: %lu\n",
    //        ch, id, g_queue_list_buffer[id].buffer_used, g_queue_list_buffer[id].last_element_bytes, (unsigned long)g_queue_list_buffer[id].buffer);

    if(id < INVALID_QUEUE && g_queue_list_buffer[id].buffer)
    {
        /* me fijo si va a entrar este byte */
        if(g_queue_list_buffer[id].total_buffer_len > (g_queue_list_buffer[id].buffer_used + sizeof(unsigned int) + 2))
        {
            /* Si es el primer elemento dejo es espacio para el unsigned int del tama�o */
            if(g_queue_list_buffer[id].last_element_bytes == 0)
            {
                for(i = 0; i < sizeof(unsigned int); i++)
                {
                    g_queue_list_buffer[id].buffer_used++;
                    g_queue_list_buffer[id].last_byte = QueueNextByte(
                                    g_queue_list_buffer[id].buffer,
                                    g_queue_list_buffer[id].total_buffer_len,
                                    g_queue_list_buffer[id].last_byte);
                }
            }
                
            *(g_queue_list_buffer[id].last_byte) = ch;
            g_queue_list_buffer[id].buffer_used++;
            g_queue_list_buffer[id].last_byte = QueueNextByte(
                            g_queue_list_buffer[id].buffer,
                            g_queue_list_buffer[id].total_buffer_len,
                            g_queue_list_buffer[id].last_byte);
            g_queue_list_buffer[id].last_element_bytes++;
            *(g_queue_list_buffer[id].last_byte) = 0;

            return true;
        }
    }
    return false;
}

bool QueueAddByteEnqueue(unsigned int id)
{
    unsigned int i;
    union
    {
        char s[sizeof(unsigned int)];
        unsigned int i;
    } union_int_string;
    
    //printf("QueueAddByteEnqueue: queue:%i buffer_used:%i last_element_bytes:%i - buffer: %lu\n",
    //        id, g_queue_list_buffer[id].buffer_used, g_queue_list_buffer[id].last_element_bytes, (unsigned long)g_queue_list_buffer[id].buffer);

    if(id < INVALID_QUEUE && g_queue_list_buffer[id].buffer)
    {
        /* Completo el tama�o del elemento al inicio del buffer */
        g_queue_list_buffer[id].last_element_bytes++;
        union_int_string.i = g_queue_list_buffer[id].last_element_bytes;
        for(i = 0; i < sizeof(unsigned int); i++)
        {
            *(g_queue_list_buffer[id].last_element) = union_int_string.s[i];
            g_queue_list_buffer[id].last_element = QueueNextByte(g_queue_list_buffer[id].buffer,
                        g_queue_list_buffer[id].total_buffer_len,
                        g_queue_list_buffer[id].last_element);
        }
        /* apunto el indice al elemento siguiente, un byte despu�s del �ltimo byte */
        g_queue_list_buffer[id].buffer_used++;
        g_queue_list_buffer[id].last_element = QueueNextByte(g_queue_list_buffer[id].buffer,
                    g_queue_list_buffer[id].total_buffer_len,
                    g_queue_list_buffer[id].last_byte);
        
        g_queue_list_buffer[id].last_element_bytes = 0;
        g_queue_list_buffer[id].last_byte = g_queue_list_buffer[id].last_element;
        
        g_queue_list_buffer[id].element_count++;
        
        if(g_queue_list_buffer[id].buffer_used > g_queue_list_buffer[id].max_buffer_used)
        {
            g_queue_list_buffer[id].max_buffer_used = g_queue_list_buffer[id].buffer_used;
        }

        return true;
    }
    return false;
}

void QueueAddByteCancell(unsigned int id)
{
    if(id < INVALID_QUEUE && g_queue_list_buffer[id].buffer)
    {
        g_queue_list_buffer[id].buffer_used -= g_queue_list_buffer[id].element_count;
        g_queue_list_buffer[id].buffer_used -= sizeof(unsigned int);
        
        g_queue_list_buffer[id].last_byte = g_queue_list_buffer[id].last_element;
        g_queue_list_buffer[id].last_element_bytes = 0;
    }
}


unsigned int QueueView(unsigned int id, char* data, unsigned int max_data_len)
{
    unsigned int i;
    char *p;
    union
    {
        char s[sizeof(unsigned int)];
        unsigned int i;
    } union_int_string;
    
    if(id < INVALID_QUEUE && g_queue_list_buffer[id].buffer)
    {
        if(g_queue_list_buffer[id].element_count)
        {
            p = g_queue_list_buffer[id].first_element;
            for(i = 0; i < sizeof(unsigned int); i++)
            {
                union_int_string.s[i] = *p;
                p = QueueNextByte(g_queue_list_buffer[id].buffer,
                            g_queue_list_buffer[id].total_buffer_len,
                            p);
            }
            for(i = 0; i < union_int_string.i; i++)
            {
                if(max_data_len)
                {
                    *data = *p;
                    data++;
                    max_data_len--;
                }
                else
                {
                    break;
                }
                p = QueueNextByte(g_queue_list_buffer[id].buffer,
                            g_queue_list_buffer[id].total_buffer_len,
                            p);
            }
            return union_int_string.i;
        }
    }
    return 0;
}

unsigned int QueueGet(unsigned int id, char* data, unsigned int max_data_len)
{
    unsigned int i;
    union
    {
        char s[sizeof(unsigned int)];
        unsigned int i;
    } union_int_string;
    
    if(id < INVALID_QUEUE && g_queue_list_buffer[id].buffer)
    {
        if(g_queue_list_buffer[id].element_count)
        {
            for(i = 0; i < sizeof(unsigned int); i++)
            {
                union_int_string.s[i] = *(g_queue_list_buffer[id].first_element);
                g_queue_list_buffer[id].first_element = QueueNextByte(g_queue_list_buffer[id].buffer,
                            g_queue_list_buffer[id].total_buffer_len,
                            g_queue_list_buffer[id].first_element);
                g_queue_list_buffer[id].buffer_used--;
            }
            for(i = 0; i < union_int_string.i; i++)
            {
                if(max_data_len)
                {
                    *data = *(g_queue_list_buffer[id].first_element);
                    data++;
                    max_data_len--;
                }
                g_queue_list_buffer[id].first_element = QueueNextByte(g_queue_list_buffer[id].buffer,
                            g_queue_list_buffer[id].total_buffer_len,
                            g_queue_list_buffer[id].first_element);
                g_queue_list_buffer[id].buffer_used--;
            }
            g_queue_list_buffer[id].element_count--;
            return union_int_string.i;
        }
    }
    return 0;
}

bool QueueDel(unsigned int id)
{
    unsigned int i;
    unsigned int len;
    char element_len_into_chars[sizeof(unsigned int)];
    
    if(id < INVALID_QUEUE && g_queue_list_buffer[id].buffer)
    {
        /* Avanzo el puntero de salida al pr�ximo elemento */
        if(g_queue_list_buffer[id].element_count)
        {
            for(i = 0; i < sizeof(unsigned int); i++)
            {
                element_len_into_chars[i] = *(g_queue_list_buffer[id].first_element);
                g_queue_list_buffer[id].first_element = QueueNextByte(g_queue_list_buffer[id].buffer,
                            g_queue_list_buffer[id].total_buffer_len,
                            g_queue_list_buffer[id].first_element);
                g_queue_list_buffer[id].buffer_used--;
            }
            memcpy(&len, element_len_into_chars, sizeof(unsigned int));
            for(i = 0; i < len; i++)
            {
                g_queue_list_buffer[id].first_element = QueueNextByte(g_queue_list_buffer[id].buffer,
                            g_queue_list_buffer[id].total_buffer_len,
                            g_queue_list_buffer[id].first_element);
                g_queue_list_buffer[id].buffer_used--;
            }
            g_queue_list_buffer[id].element_count--;
        }
        return true;
    }
    return false;
}

bool QueueClean(unsigned int id)
{
    if(id < INVALID_QUEUE && g_queue_list_buffer[id].buffer)
    {
        g_queue_list_buffer[id].last_element = g_queue_list_buffer[id].buffer;
        g_queue_list_buffer[id].last_byte = g_queue_list_buffer[id].buffer;
        g_queue_list_buffer[id].last_element_bytes = 0;
        g_queue_list_buffer[id].first_element = g_queue_list_buffer[id].buffer;
        g_queue_list_buffer[id].element_count = 0;
        g_queue_list_buffer[id].buffer_used = 0;
        return true;
    }
    return false;
}


/* estadisticas de uso de las colas */
unsigned int QueueOpens( void )
{
    return queue_count_opens;
}

unsigned int QueueMaxOpens( void )
{
    return queue_count_max_opens;
}

unsigned int QueueElements(unsigned int id)
{
    if(id < INVALID_QUEUE && g_queue_list_buffer[id].buffer)
    {
        return g_queue_list_buffer[id].element_count;
    }
    return 0;
}

unsigned int QueueBufferUsed(unsigned int id)
{
    if(id < INVALID_QUEUE && g_queue_list_buffer[id].buffer)
    {
        return g_queue_list_buffer[id].buffer_used;
    }
    return 0;
}

unsigned int QueueMaxBufferUsed(unsigned int id)
{
    if(id < INVALID_QUEUE && g_queue_list_buffer[id].buffer)
    {
        return g_queue_list_buffer[id].max_buffer_used;
    }
    return 0;
}
