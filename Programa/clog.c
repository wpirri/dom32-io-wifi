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
#include "clog.h"

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

#include "rtcc.h"

void LogInit(void)
{
    LogMessage("LOG Init.");
}

void LogMessage(const char* msg, ...)
{
    va_list args;
    char str[81];
    struct tm logTime;
    
    RTCC_TimeGet(&logTime);
    printf("%02i:%02i:%02i-", logTime.tm_hour, logTime.tm_min, logTime.tm_sec);
    va_start (args, msg);
    vprintf(msg, args);
    va_end(args);
    printf("\r\n");
}
