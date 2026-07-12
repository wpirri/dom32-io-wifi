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
#ifndef _IO_H_
#define	_IO_H_

#include <xc.h>
#include <stdbool.h>

void IOInit( void );

void IOConfig( void );
void IOLedControl( void );

#define LED_Azul_SetHigh()          ( LATASET = (1 << 0) )
#define LED_Azul_SetLow()           ( LATACLR = (1 << 0) )
inline static void LED_Azul_SetValue(bool value)
{
  if(value)
  {
    LED_Azul_SetHigh();
  }
  else
  {
    LED_Azul_SetLow();
  }
}
#define LED_Azul_Toggle()           ( LATAINV = (1 << 0) )
#define LED_Azul_GetValue()         PORTAbits.RA0
#define LED_Azul_SetDigitalInput()   ( TRISASET = (1 << 0) )
#define LED_Azul_SetDigitalOutput()   ( TRISACLR = (1 << 0) )
#define LED_Verde_SetHigh()          ( LATASET = (1 << 1) )
#define LED_Verde_SetLow()           ( LATACLR = (1 << 1) )
inline static void LED_Verde_SetValue(bool value)
{
  if(value)
  {
    LED_Verde_SetHigh();
  }
  else
  {
    LED_Verde_SetLow();
  }
}
#define LED_Verde_Toggle()           ( LATAINV = (1 << 1) )
#define LED_Verde_GetValue()         PORTAbits.RA1
#define LED_Verde_SetDigitalInput()   ( TRISASET = (1 << 1) )
#define LED_Verde_SetDigitalOutput()   ( TRISACLR = (1 << 1) )
#define LED_Ambar_SetHigh()          ( LATASET = (1 << 4) )
#define LED_Ambar_SetLow()           ( LATACLR = (1 << 4) )
inline static void LED_Ambar_SetValue(bool value)
{
  if(value)
  {
    LED_Ambar_SetHigh();
  }
  else
  {
    LED_Ambar_SetLow();
  }
}
#define LED_Ambar_Toggle()           ( LATAINV = (1 << 4) )
#define LED_Ambar_GetValue()         PORTAbits.RA4
#define LED_Ambar_SetDigitalInput()   ( TRISASET = (1 << 4) )
#define LED_Ambar_SetDigitalOutput()   ( TRISACLR = (1 << 4) )

#define IO1_SetHigh()          ( LATBSET = (1 << 5) )
#define IO1_SetLow()           ( LATBCLR = (1 << 5) )
inline static void IO1_SetValue(bool value)
{
  if(value)
  {
    IO1_SetHigh();
  }
  else
  {
    IO1_SetLow();
  }
}
#define IO1_Toggle()           ( LATBINV = (1 << 5) )
#define IO1_GetValue()         PORTBbits.RB5
#define IO1_SetDigitalInput()   ( TRISBSET = (1 << 5) )
#define IO1_SetDigitalOutput()   ( TRISBCLR = (1 << 5) )
#define IO2_SetHigh()          ( LATBSET = (1 << 4) )
#define IO2_SetLow()           ( LATBCLR = (1 << 4) )
inline static void IO2_SetValue(bool value)
{
  if(value)
  {
    IO2_SetHigh();
  }
  else
  {
    IO2_SetLow();
  }
}
#define IO2_Toggle()           ( LATBINV = (1 << 4) )
#define IO2_GetValue()         PORTBbits.RB4
#define IO2_SetDigitalInput()   ( TRISBSET = (1 << 4) )
#define IO2_SetDigitalOutput()   ( TRISBCLR = (1 << 4) )
#define IO3_SetHigh()          ( LATBSET = (1 << 3) )
#define IO3_SetLow()           ( LATBCLR = (1 << 3) )
inline static void IO3_SetValue(bool value)
{
  if(value)
  {
    IO3_SetHigh();
  }
  else
  {
    IO3_SetLow();
  }
}
#define IO3_Toggle()           ( LATBINV = (1 << 3) )
#define IO3_GetValue()         PORTBbits.RB3
#define IO3_SetDigitalInput()   ( TRISBSET = (1 << 3) )
#define IO3_SetDigitalOutput()   ( TRISBCLR = (1 << 3) )
#define IO4_SetHigh()          ( LATBSET = (1 << 2) )
#define IO4_SetLow()           ( LATBCLR = (1 << 2) )
inline static void IO4_SetValue(bool value)
{
  if(value)
  {
    IO4_SetHigh();
  }
  else
  {
    IO4_SetLow();
  }
}
#define IO4_Toggle()           ( LATBINV = (1 << 2) )
#define IO4_GetValue()         PORTBbits.RB2
#define IO4_SetDigitalInput()   ( TRISBSET = (1 << 2) )
#define IO4_SetDigitalOutput()   ( TRISBCLR = (1 << 2) )
#define IO5_SetHigh()          ( LATBSET = (1 << 7) )
#define IO5_SetLow()           ( LATBCLR = (1 << 7) )
inline static void IO5_SetValue(bool value)
{
  if(value)
  {
    IO5_SetHigh();
  }
  else
  {
    IO5_SetLow();
  }
}
#define IO5_Toggle()           ( LATBINV = (1 << 7) )
#define IO5_GetValue()         PORTBbits.RB7
#define IO5_SetDigitalInput()   ( TRISBSET = (1 << 7) )
#define IO5_SetDigitalOutput()   ( TRISBCLR = (1 << 7) )
#define IO6_SetHigh()          ( LATBSET = (1 << 8) )
#define IO6_SetLow()           ( LATBCLR = (1 << 8) )
inline static void IO6_SetValue(bool value)
{
  if(value)
  {
    IO6_SetHigh();
  }
  else
  {
    IO6_SetLow();
  }
}
#define IO6_Toggle()           ( LATBINV = (1 << 8) )
#define IO6_GetValue()         PORTBbits.RB8
#define IO6_SetDigitalInput()   ( TRISBSET = (1 << 8) )
#define IO6_SetDigitalOutput()   ( TRISBCLR = (1 << 8) )

#define SPI_CS_SetHigh()          ( LATBSET = (1 << 6) )
#define SPI_CS_SetLow()           ( LATBCLR = (1 << 6) )
inline static void SPI_CS_SetValue(bool value)
{
  if(value)
  {
    SPI_CS_SetHigh();
  }
  else
  {
    SPI_CS_SetLow();
  }
}
#define SPI_CS_Toggle()           ( LATBINV = (1 << 6) )
#define SPI_CS_GetValue()         PORTBbits.RB6
#define SPI_CS_SetDigitalInput()   ( TRISBSET = (1 << 6) )
#define SPI_CS_SetDigitalOutput()   ( TRISBCLR = (1 << 6) )

#define OUT1_SetHigh()          ( LATBSET = (1 << 9) )
#define OUT1_SetLow()           ( LATBCLR = (1 << 9) )
inline static void OUT1_SetValue(bool value)
{
  if(value)
  {
    OUT1_SetHigh();
  }
  else
  {
    OUT1_SetLow();
  }
}
#define OUT1_Toggle()           ( LATBINV = (1 << 9) )
#define OUT1_GetValue()         PORTBbits.RB9
#define OUT1_SetDigitalInput()   ( TRISBSET = (1 << 9) )
#define OUT1_SetDigitalOutput()   ( TRISBCLR = (1 << 9) )

#define OUT2_SetHigh()          ( LATCSET = (1 << 9) )
#define OUT2_SetLow()           ( LATCCLR = (1 << 9) )
inline static void OUT2_SetValue(bool value)
{
  if(value)
  {
    OUT2_SetHigh();
  }
  else
  {
    OUT2_SetLow();
  }
}
#define OUT2_Toggle()           ( LATCINV = (1 << 9) )
#define OUT2_GetValue()         PORTCbits.RC9
#define OUT2_SetDigitalInput()   ( TRISCSET = (1 << 9) )
#define OUT2_SetDigitalOutput()   ( TRISCCLR = (1 << 9) )

#define OUT3_SetHigh()          ( LATBSET = (1 << 10) )
#define OUT3_SetLow()           ( LATBCLR = (1 << 10) )
inline static void OUT3_SetValue(bool value)
{
  if(value)
  {
    OUT3_SetHigh();
  }
  else
  {
    OUT3_SetLow();
  }
}
#define OUT3_Toggle()           ( LATBINV = (1 << 10) )
#define OUT3_GetValue()         PORTBbits.RB10
#define OUT3_SetDigitalInput()   ( TRISBSET = (1 << 10) )
#define OUT3_SetDigitalOutput()   ( TRISBCLR = (1 << 10) )

#define OUT4_SetHigh()          ( LATBSET = (1 << 11) )
#define OUT4_SetLow()           ( LATBCLR = (1 << 11) )
inline static void OUT4_SetValue(bool value)
{
  if(value)
  {
    OUT4_SetHigh();
  }
  else
  {
    OUT4_SetLow();
  }
}
#define OUT4_Toggle()           ( LATBINV = (1 << 11) )
#define OUT4_GetValue()         PORTBbits.RB11
#define OUT4_SetDigitalInput()   ( TRISBSET = (1 << 11) )
#define OUT4_SetDigitalOutput()   ( TRISBCLR = (1 << 11) )

#endif	/* _IO_H_ */

