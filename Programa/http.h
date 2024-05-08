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

void HTTPInit( void );

void HTTPTask( void );

void HTTPTimer( void );

int HTTPOutPending(void);
int HTTPOutConnId(void);
char HTTPOutType(void);
const char* HTTPOutData(void);
unsigned int HTTPOutTimeOut(void);
const char* HTTPOutFilename(void);
void HTTPOutNext(void);

int HTTPNotificarStatus(char *out, int max_len);
int HTTPNotificarTarjeta( const char *crd);

int HTTPGetFormData(char *buffer, const char *label, char *value);

int HTTPAddOUTQueue(const char* filename, const char* buffer, unsigned int to_cs);
int HTTPAddRespQueue(int id, const char* buffer);
int HTTPAddINQueue(int id, const char* buffer);

int HTTPHeadResponse(const char *http_head);
int HTTPFormResponse(const char *http_data);

int HTTPGetFile(const char* filename, const char* params);
int HTTPInitTmpFile(const char* buffer, int len);
int HTTPAddTmpFile(const char* buffer, int len);
int HTTPEndTmpFile(const char* filename, const char* buffer, int len);
void HTTPProcessTmpFile(const char* tmpname, const char* filename);

#endif /* _HTTP_H_ */