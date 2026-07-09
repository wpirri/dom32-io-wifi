/***************************************************************************
    Copyright (C) 2026   Walter Pirri

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

void HttpInit(void);
void HttpTask(void);
void HttpTimerInt(void);
int HttpOnLine( void );
char* HttpCurrentHost( void );
int HttpCurrentPort( void );
void HttpConexionOk( void );
void HttpConexionError( void );
void HttpSwitchHost( void );

int HttpDecodeFormData(char* out, const char* in, unsigned int max_out);
int HttpGetRequest(const char *buffer, char *request, int max_len);
int HTTPGetFormData(const char *buffer, const char *label, char *value);
int HTTPGetHeadData(const char *buffer, const char *label, char *value);
int HTTPHeadResponse(const char *http_head);
int HTTPFormResponse(const char *http_data);


int HttpQueryHost(const char* url, const char* msg_send, char* resp_header, char* resp_data);
int HttpDownload(const char* filename);
void HTTPCommandSwitch( const char *obj_name );
void HTTPCommandPulse( const char *obj_name );

int HttpProcessRequest(const char* request, const char* uri, const char* host,
                       unsigned int content_length, char* data_io, unsigned int max_io);

#endif /* _HTTPL_H_ */
