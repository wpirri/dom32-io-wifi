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
#ifndef _HTTP_H_
#define _HTTP_H_

#include <stdbool.h>

#define MAX_HTTP_DATA 2047
#define MAX_HTTP_URI  63

typedef struct
{
    char type;
    char timeout;
    int id;
    char uri[MAX_HTTP_URI + 1];
    char data[MAX_HTTP_DATA + 1];
} http_data;

#ifdef _IN_HTTP_C_
http_data http_buffer_input;
http_data http_buffer_output;
http_data http_buffer_response;
#else
extern http_data http_buffer_input;
extern http_data http_buffer_output;
extern http_data http_buffer_response;
#endif

void HTTPInit( void );

void HTTPTask( void );

void HTTPTimer( void );

int HTTPAddInput(int id, const char* buffer);
int HTTPAddOutput(const char* buffer);

int HTTPNotificarStatus();
int HTTPNotificarTarjeta( const char *crd );

int HTTPGetFormData(char *buffer, const char *label, char *value);

#endif /* _HTTP_H_ */