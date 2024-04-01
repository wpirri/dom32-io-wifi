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
#define _IN_RTCC_C_
#include "rtcc.h"
#undef _IN_RTCC_C_

#include <xc.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mcc_generated_files/interrupt_manager.h"

#include "config.h"
#include "clog.h"

static uint8_t ConvertHexToBCD(uint8_t hexvalue);
static uint8_t ConvertBCDToHex(uint8_t bcdvalue);
int WeekDay(int year, int month, int day);

typedef enum
{
    RTCC_INIT,
    RTCC_SINC_1,
    RTCC_SINC_2,
    RTCC_SINC_3,
    RTCC_SINC_4,
    RTCC_SINC_5,
    RTCC_SINC_6,
    RTCC_SYNC_OK
} RTCC_STATUS;

RTCC_STATUS rtcc_status;

unsigned long g_rtcc_refresh_count;
unsigned int g_clk_div_adj;
        
void RTCCInit(void)
{
    rtcc_status = RTCC_INIT;
    g_rtcc_refresh_count = 10;
    g_clk_div_adj = 48000;
    g_get_time_flag = 0;
    
    
    /* Reg Unlock */
    SYSKEY = 0x00000000; //write invalid key to force lock
    SYSKEY = 0xAA996655; //write Key1 to SYSKEY
    SYSKEY = 0x556699AA; //write Key2 to SYSKEY
    
    RTCCON1CLR = (1 << _RTCCON1_WRLOCK_POSITION);
    RTCCON1CLR = (1 << _RTCCON1_ON_POSITION);
     
    // set 2018-01-01 00-00-00
    RTCDATE = 0x22010101; // Year/Month/Date/Wday
    RTCTIME = 0x0; //  hours/minutes/seconds

    // set 2018-01-01 00-01-57        
    ALMDATE = 0x10101; // Month/Date/Wday
    ALMTIME = 0x15700; // hours/minutes/seconds
    // ON enabled; OUTSEL Alarm Event; WRLOCK disabled; AMASK Every Half Second; ALMRPT 0; RTCOE disabled; CHIME enabled; ALRMEN enabled; 
    RTCCON1 = 0xC0008000;

    // DIV 15999; CLKSEL LPRC; FDIV 0; 
    //RTCCON2 = 0x3E7F0001;
    RTCCON2bits.CLKSEL = 1;     /* LPRC */
    RTCCON2bits.DIV = RTCC_DEFAULT_DIV;
    RTCCON2bits.FDIV = RTCC_DEFAULT_FDIV;
    
    // Enable RTCC 
    RTCCON1SET = (1 << _RTCCON1_ON_POSITION);
    RTCCON1SET = (1 << _RTCCON1_WRLOCK_POSITION);
    /* Reg Lock */
    SYSKEY = 0x00000000; //write invalid key to force lock

    IEC1bits.RTCCIE = 1;
    
}

bool RTCC_TimeGet(struct tm *currentTime)
{
    uint32_t register_value;
    if(RTCSTATbits.SYNC){
        return false;
    }
    
    register_value = RTCDATE;
    currentTime->tm_year = ConvertBCDToHex((register_value & 0xFF000000) >> 24);
    currentTime->tm_mon = ConvertBCDToHex((register_value & 0xFF0000) >> 16);
    currentTime->tm_mday = ConvertBCDToHex((register_value & 0xFF00) >> 8);
    currentTime->tm_wday = ConvertBCDToHex(register_value & 0xF);
    
    register_value = RTCTIME;
    currentTime->tm_hour = ConvertBCDToHex((register_value & 0xFF000000) >> 24);
    currentTime->tm_min = ConvertBCDToHex((register_value & 0xFF0000) >> 16);
    currentTime->tm_sec = ConvertBCDToHex((register_value & 0xFF00) >> 8);

    return true;
}

void RTCC_TimeSet(struct tm *newTime)
{
    struct tm oldTime;
    int diferencia;
    
    RTCC_TimeGet(&oldTime);
    LogMessage("[RTCC_TimeSet] Cambio de hora [%d/%02d/%02d %02d:%02d:%02d] -> [%d/%02d/%02d %02d:%02d:%02d].",
            oldTime.tm_year, oldTime.tm_mon, oldTime.tm_mday, oldTime.tm_hour, oldTime.tm_min, oldTime.tm_sec,
            newTime->tm_year, newTime->tm_mon, newTime->tm_mday, newTime->tm_hour, newTime->tm_min, newTime->tm_sec);

    if(oldTime.tm_year == newTime->tm_year &&
       oldTime.tm_mon == newTime->tm_mon && 
       oldTime.tm_mday == newTime->tm_mday)
    {
        diferencia = (oldTime.tm_hour*3600 + oldTime.tm_min*60 + oldTime.tm_sec) - 
                (newTime->tm_hour*3600 + newTime->tm_min*60 + newTime->tm_sec);
        LogMessage("[RTCC_TimeSet] Diferencia: %i segundos", diferencia);
        
        if(diferencia == 0)
        {
            if(rtcc_status < RTCC_SYNC_OK) rtcc_status++;
        }
        else
        {
            RTCC_AdjustClk(diferencia);
        }
    }

    switch(rtcc_status)
    {
        case RTCC_INIT:
            LogMessage("[RTCC_TimeSet] Actualizar hora en 10 seg.");
            g_rtcc_refresh_count = 100; /* Actualizar cada 10 seg */
            break;
        case RTCC_SINC_1:
            LogMessage("[RTCC_TimeSet] Actualizar hora en 30 seg.");
            g_rtcc_refresh_count = 300; /* Actualizar cada 30 seg */
            break;
        case RTCC_SINC_2:
            LogMessage("[RTCC_TimeSet] Actualizar hora en 1 min.");
            g_rtcc_refresh_count = 600; /* Actualizar cada 1 min */
            break;
        case RTCC_SINC_3:
            LogMessage("[RTCC_TimeSet] Actualizar hora en 10 min.");
            g_rtcc_refresh_count = 6000; /* Actualizar cada 10 min */
            break;
        case RTCC_SINC_4:
            LogMessage("[RTCC_TimeSet] Actualizar hora en 1/2 hora.");
            g_rtcc_refresh_count = 18000; /* Actualizar cada 1/2 hora */
            break;
        case RTCC_SINC_5:
            LogMessage("[RTCC_TimeSet] Actualizar hora en 1 hora.");
            g_rtcc_refresh_count = 36000; /* Actualizar cada 1 hora */
            break;
        case RTCC_SINC_6:
            LogMessage("[RTCC_TimeSet] Actualizar hora en 12 horas.");
            g_rtcc_refresh_count = 432000; /* Actualizar cada 12 horas */
            break;
        case RTCC_SYNC_OK:
            LogMessage("[RTCC_TimeSet] Actualizar hora en 24 horas.");
            g_rtcc_refresh_count = 864000; /* Sincronizar cada 24 horas */
            break;
    }
    
    newTime->tm_wday = WeekDay(newTime->tm_year, newTime->tm_mon, newTime->tm_mday);
    
    /* Reg Unlock */
    __builtin_disable_interrupts();
    SYSKEY = 0x00000000; //write invalid key to force lock
    SYSKEY = 0xAA996655; //write Key1 to SYSKEY
    SYSKEY = 0x556699AA; //write Key2 to SYSKEY
    
    RTCCON1CLR = (1 << _RTCCON1_WRLOCK_POSITION);
    RTCCON1CLR = (1 << _RTCCON1_ON_POSITION);
  
    IFS1CLR= 1 << _IFS1_RTCCIF_POSITION;
    //IEC0bits.RTCCIE = 0;

    // set RTCC initial time
    RTCDATE = (ConvertHexToBCD(newTime->tm_year) << 24) | (ConvertHexToBCD(newTime->tm_mon) << 16 ) | (ConvertHexToBCD(newTime->tm_mday) << 8) | ConvertHexToBCD(newTime->tm_wday) ; // YEAR/MONTH-1/DAY-1/WEEKDAY
    RTCTIME = (ConvertHexToBCD(newTime->tm_hour) << 24) | (ConvertHexToBCD(newTime->tm_min) << 16 ) | (ConvertHexToBCD(newTime->tm_sec) << 8) ; // HOURS/MINUTES/SECOND

    RTCCON2bits.DIV = (g_clk_div_adj / 30);
    RTCCON2bits.FDIV = (g_clk_div_adj % 30);

    // Enable RTCC 
    RTCCON1SET = (1 << _RTCCON1_ON_POSITION);
    
    // SET WRLOCK
    RTCCON1SET = (1 << _RTCCON1_WRLOCK_POSITION);
    /* Reg Lock */
    SYSKEY = 0x00000000; //write invalid key to force lock
    __builtin_enable_interrupts();

    LogMessage("[RTCC_TimeSet] DIV: %d  FDIV: %d.", RTCCON2bits.DIV, RTCCON2bits.FDIV);
    
    //Enable RTCC interrupt
    //IEC0bits.RTCCIE = 1;
}

static uint8_t ConvertHexToBCD(uint8_t hexvalue)
{
    uint8_t bcdvalue;
    bcdvalue = (hexvalue / 10) << 4;
    bcdvalue = bcdvalue | (hexvalue % 10);
    return (bcdvalue);
}

static uint8_t ConvertBCDToHex(uint8_t bcdvalue)
{
    uint8_t hexvalue;
    hexvalue = (((bcdvalue & 0xF0) >> 4)* 10) + (bcdvalue & 0x0F);
    return hexvalue;
}

int WeekDay(int year, int month, int day)
{
    static const int mnthTbl[]= {0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 12};
	int y=year%100;			// just the year, remove the century part
	int centItem=(3-((year/100)%4))*2;
	int yearItem=y/12+y%12+(y%12)/4;
	int wDay=centItem+yearItem+mnthTbl[month-1]+day;
	if(month==1 || month==2)
	{	// Jan Feb correction, if leap year
		if( (y!=0 && y%4==0) || year%400==0)
		{
			wDay--;
		}
	}
	return wDay%7;
}

/*
 * v = -1  -> Hace el reloj mas lento (lo hace atrasar)
 * v = 1 -> Hace el reloj mas rapido (lo hace adelantar)
 */
/*
Deviation from the desired input frequency is adjusted by changing the values of DIVx and FDIVx.
A faster than desired oscillator generally requires increases to the value of FDIVx for small changes,
or decreases to the value of DIVx for large changes.
On the other hand, a slow oscillator requires decreases to the value of FDIVx, or
increases to the value of DIVx.
*/
void RTCC_AdjustClk(int diferencia)
{
    if(diferencia < 0)
    {
        LogMessage("[RTCC_AdjustClk] Ajuste de clock (adelantar).");
        if(g_clk_div_adj > 45000) g_clk_div_adj += ((diferencia/5)-1);
        LogMessage("[RTCC_AdjustClk] Ajuste fino de reloj: %d.", g_clk_div_adj);
    }
    else if(diferencia > 0)
    {
        LogMessage("[RTCC_AdjustClk] Ajuste de clock (atrasar).");
        if(g_clk_div_adj < 51000) g_clk_div_adj += ((diferencia/5)+1);
        LogMessage("[RTCC_AdjustClk] Ajuste fino de reloj: %d.", g_clk_div_adj);
    }
    else
    {
        LogMessage("[RTCC_AdjustClk] Ajuste de clock Ok.");
    }
}

/* Se llama 2 veces por segundo */
void RTCC_Task( void )
{
    if(g_rtcc_refresh_count)
    {
        g_rtcc_refresh_count--;
        if(g_rtcc_refresh_count == 0)
        {
            g_get_time_flag = 1;
        }
    }
    
}
