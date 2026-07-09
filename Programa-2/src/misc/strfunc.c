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
#include "strfunc.h"

#include <string.h>

/*  Separa en campos segun el separador y devuelve el número de campo solicitado
    en count. El primer campo es el 1.
    Si count e 0 devuelve lo que hay antes del separador
    El dato devuelto se termina al encntrar un separador o cualquier caracter que no se número o letra
*/
int str_section(char *in, char sep, unsigned int count, char *out)
{
    char *p;
    int len = 0;
    
    p = in;
    while(count && *p)
    {
        if(*p == sep) count--;
        p++;
    }
    if(*p == 0 || count) return 0;
    /* p apunta al inicio del campo, copio hasta el final */
    while(*p && (*p >= 32 && *p <= 126) && *p != sep )
    {
        *out++ = *p++;
        len++;
    }
    *out = 0;
    return len;
}

int url_encode(char *in, char* out)
{
    char str[1024];
    char *p;
    
    if(in == NULL || out == NULL) return (-1);
    
    p = &str[0];
    while(*in)
    {
        if(*in == ' ')
        {
            *p++ = '%';
            *p++ = '2';
            *p++ = '0';
        }
        else
        {
            *p++ = *in;
        }
        in++;
    }
    *p = 0;
    
    p = &str[0];
    while(*p)
    {
        *out++ = *p++;
    }
    *out = 0;
    
    return (int)(p - &str[0]);
}
