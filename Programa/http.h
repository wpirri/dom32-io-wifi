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

#include "sd/fsio.h"

#define MAX_HTTP_DATA 2048
#define MAX_HTTP_URI  256

typedef struct
{
    char type;
    char timeout;
    int id;
    int http_rc;
    int form_rc;
    char uri[MAX_HTTP_URI + 1];
    char data[MAX_HTTP_DATA + 1];
    char download;
    unsigned long download_length;
    char filename[FILE_NAME_SIZE+1];
} http_data;

#ifdef _IN_HTTP_C_
http_data http_buffer_input;
http_data http_buffer_output;
http_data http_buffer_response;
char g_download_file_name[256];
char g_download_file_params[256];
#else
extern http_data http_buffer_input;
extern http_data http_buffer_output;
extern http_data http_buffer_response;
extern char g_download_file_name[256];
extern char g_download_file_params[256];
#endif

void HTTPInit( void );

void HTTPTask( void );

void HTTPTimer( void );

int HTTPAddInput(int id, const char* buffer);
int HTTPAddOutput(const char* filename, bool download, const char* buffer, unsigned int to_cs);

int HTTPNotificarStatus();
int HTTPNotificarTarjeta( const char *crd );
int HTTPCommandSwitch( const char *obj_name );
int HTTPCommandPulse( const char *obj_name );

int HTTPGetFormData(const char *buffer, const char *label, char *value);
int HTTPGetHeadData(const char *buffer, const char *label, char *value);

int HTTPGetFile(const char* filename, const char* params);
int HTTPInitTmpFile(const char* buffer, int len);
int HTTPAddTmpFile(const char* buffer, int len);
int HTTPEndTmpFile(const char* filename, const char* buffer, int len);
int HTTPProcessTmpFile(const char* tmpname, const char* filename, unsigned long filelength);

#endif /* _HTTP_H_ */