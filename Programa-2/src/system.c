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
#include "system.h"

/* ************************************************************************* */
// FDEVOPT
#pragma config SOSCHP = OFF    //Secondary Oscillator High Power Enable bit->SOSC oprerates in normal power mode.
#pragma config ALTI2C = OFF    //Alternate I2C1 Pins Location Enable bit->Primary I2C1 pins are used
#pragma config FUSBIDIO = OFF    //USBID pin control->USBID pin is controlled by the USB module
#pragma config FVBUSIO = OFF    //VBUS Pin Control->VBUS pin is controlled by the USB module

// FICD
#pragma config JTAGEN = OFF             // JTAG Enable bit (JTAG is disabled)
#pragma config ICS = PGx2               // ICE/ICD Communication Channel Selection bits (Communicate on PGEC3/PGED3)

// FPOR
#pragma config BOREN = BOR3             // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware; SBOREN bit disabled)
#pragma config RETVR = OFF              // Retention Voltage Regulator Enable bit (Retention regulator is disabled)
#pragma config LPBOREN = ON    //Downside Voltage Protection Enable bit->Low power BOR is enabled, when main BOR is disabled

// FWDT
#pragma config SWDTPS = PS1048576    //Sleep Mode Watchdog Timer Postscale Selection bits->1:1048576
#pragma config FWDTWINSZ = PS25_0    //Watchdog Timer Window Size bits->Watchdog timer window size is 25%
#pragma config WINDIS = OFF    //Windowed Watchdog Timer Disable bit->Watchdog timer is in non-window mode
#pragma config RWDTPS = PS1024    //Run Mode Watchdog Timer Postscale Selection bits->1:1024
#pragma config RCLKSEL = LPRC    //Run Mode Watchdog Timer Clock Source Selection bits->Clock source is LPRC (same as for sleep mode)
#pragma config FWDTEN = ON    //Watchdog Timer Enable bit->WDT is enabled

// FOSCSEL
#pragma config FNOSC = PLL    //Oscillator Selection bits->Primary or FRC oscillator with PLL
#pragma config PLLSRC = FRC    //System PLL Input Clock Selection bit->FRC oscillator is selected as PLL reference input on device reset
#pragma config SOSCEN = OFF    //Secondary Oscillator Enable bit->Secondary oscillator is disabled
#pragma config IESO = ON    //Two Speed Startup Enable bit->Two speed startup is enabled
#pragma config POSCMOD = OFF    //Primary Oscillator Selection bit->Primary oscillator is disabled
#pragma config OSCIOFNC = OFF    //OFF para RA4 como Digital I/O
#pragma config SOSCSEL = ON    //Secondary Oscillator External Clock Enable bit->SCLKI pin configured for Digital mode
#pragma config FCKSM = CSECME    //Clock Switching and Fail-Safe Clock Monitor Enable bits->Clock switching is enabled; Fail-safe clock monitor is enabled

// FSEC
#pragma config CP = OFF                 // Code Protection Enable bit (Code protection is disabled)
/* ************************************************************************* */

#include <xc.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "defines.h"
#include "log.h"
#include "wifi/wifi.h"
#include "control.h"
#include "io.h"

/* * Manejo de excepciones ************************************************** */
static enum {
    EXCEP_IRQ = 0,            // interrupt
    EXCEP_AdEL = 4,            // address error exception (load or ifetch)
    EXCEP_AdES,                // address error exception (store)
    EXCEP_IBE,                // bus error (ifetch)
    EXCEP_DBE,                // bus error (load/store)
    EXCEP_Sys,                // syscall
    EXCEP_Bp,                // breakpoint
    EXCEP_RI,                // reserved instruction
    EXCEP_CpU,                // coprocessor unusable
    EXCEP_Overflow,            // arithmetic overflow
    EXCEP_Trap,                // trap (possible divide by zero)
    EXCEP_IS1 = 16,            // implementation specfic 1
    EXCEP_CEU,                // CorExtend Unuseable
    EXCEP_C2E                // coprocessor 2
} _excep_code;

void __attribute__((weak, nomips16)) _general_exception_handler (void)
{
    static unsigned int _excep_addr;
    static char* _except_str;
    
    asm volatile("mfc0 %0,$13" : "=r" (_excep_code));
    asm volatile("mfc0 %0,$14" : "=r" (_excep_addr));

    _excep_code = (_excep_code & 0x0000007C) >> 2;

    switch(_excep_code){
        case EXCEP_IRQ: _except_str = "interrupt"; break;
        case EXCEP_AdEL: _except_str = "address error exception (load or ifetch)"; break;
        case EXCEP_AdES: _except_str = "address error exception (store)"; break;
        case EXCEP_IBE: _except_str = "bus error (ifetch)"; break;
        case EXCEP_DBE: _except_str = "bus error (load/store)"; break;
        case EXCEP_Sys: _except_str = "syscall"; break;
        case EXCEP_Bp: _except_str = "breakpoint"; break;
        case EXCEP_RI: _except_str = "reserved instruction"; break;
        case EXCEP_CpU: _except_str = "coprocessor unusable"; break;
        case EXCEP_Overflow: _except_str = "arithmetic overflow"; break;
        case EXCEP_Trap: _except_str = "trap (possible divide by zero)"; break;
        case EXCEP_IS1: _except_str = "implementation specfic 1"; break;
        case EXCEP_CEU: _except_str = "CorExtend Unuseable"; break;
        case EXCEP_C2E: _except_str = "coprocessor 2"; break;
    }

    LogMessage("[EXCEPTION] %s: 0x%X at 0x%X", _except_str, _excep_code, _excep_addr);
    
    while(1) { CLRWDT(); }
}

void __attribute__ ((vector(_CORE_TIMER_VECTOR), interrupt(IPL1SOFT))) _CORE_TIMER_ISR(void)
{
    uint32_t static compare = CORE_TIMER_COMPARE_VAL;

    // Update the compare value
    compare = compare + CORE_TIMER_COMPARE_VAL;
    _CP0_SET_COMPARE(compare);
    IFS0CLR= 1 << _IFS0_CTIF_POSITION;
    /* ********************************************************************* */
    /* 100 interrupciones por segundo                                        */
    /* ********************************************************************* */
    WiFiTimer();
    ControlTimer();
    IOLedControl();
}

void __attribute__ ( ( vector(_TIMER_1_VECTOR) , interrupt(IPL7SOFT) ) ) _TIMER1_IRS ( void )
{
    IFS0CLR = _IFS0_T1IF_MASK; //Clear T1IF
    /* ********************************************************************* */

}

void SystemInit( void )
{
    /* ********************************************************************* */
    /*  Setting the Output Latch SFR(s) */
    LATA = 0x00000000;
    LATB = 0x00000000;
    LATC = 0x00000000;

    /* ********************************************************************* */
    /* Setting the GPIO Direction SFR(s) */
    TRISA = 0xFFFFFFFF;
    TRISB = 0xFFFFFFFF;
    TRISC = 0xFFFFFFFF;
    
    /* ********************************************************************* */
    /* Setting the Weak Pull Up and Weak Pull Down SFR(s) */
    CNPDA = 0x00000000;
    CNPDB = 0x00000000;
    CNPDC = 0x00000000;
    CNPUA = 0x00000000;
    CNPUB = 0x00000000;
    CNPUC = 0x00000000;

    /* ********************************************************************* */
    /* Setting the Open Drain SFR(s) */
    ODCA = 0x00000000;
    ODCB = 0x00000000;
    ODCC = 0x00000000;

    /* ********************************************************************* */
    /* Setting the Analog/Digital Configuration SFR(s) */
    ANSELA = 0x00000000;
    ANSELB = 0x00000000;

    /* ********************************************************************* */
    /* Modulo CPP */
    CCP1CON1 = 0x00000000;    
    CCP2CON1 = 0x00000000;    
    CCP3CON1 = 0x00000000;    
    
    /* ********************************************************************* */
    /* Change notification */
    CNCONB = 0x00000000;
    /* ********************************************************************* */
    /* OSCILLATOR Init */
    // System Reg Unlock
    SYSKEY = 0x00000000; 
    SYSKEY = 0xAA996655; //write Key1 to SYSKEY
    SYSKEY = 0x556699AA; //write Key2 to SYSKEY
    // CF No Clock Failure; FRCDIV FRC/1; SLPEN Device will enter Idle mode when a WAIT instruction is issued; NOSC SPLL; SOSCEN disabled; CLKLOCK Clock and PLL selections are not locked and may be modified; OSWEN Complete; 
    OSCCON = 0x100;
    // TUN Center frequency; 
    OSCTUN = 0x0;
    // PLLODIV 1:1; PLLMULT 3x; PLLICLK FRC; 
    SPLLCON = 0x10080;
    // SWRST disabled; 
    RSWRST = 0x0;
    // WDTO disabled; GNMI disabled; CF disabled; WDTS disabled; NMICNT 0; LVD disabled; SWNMI disabled; 
    RNMICON = 0x0;
    // SBOREN disabled; VREGS disabled; RETEN disabled; 
    PWRCON = 0x0;
    // WDTO disabled; EXTR disabled; POR disabled; SLEEP disabled; BOR disabled; PORIO disabled; IDLE disabled; PORCORE disabled; BCFGERR disabled; CMR disabled; BCFGFAIL disabled; SWR disabled; 
    RCON = 0x0;
    // ON disabled; DIVSWEN disabled; RSLP disabled; ROSEL SYSCLK; OE disabled; SIDL disabled; RODIV 0; 
    REFO1CON = 0x0;
    // ROTRIM 0; 
    REFO1TRIM = 0x0;
    // SPDIVRDY disabled; 
    CLKSTAT = 0x0;
    // System Reg Lock
    SYSKEY = 0x00000000; 

    RCON = 0x00000000;
    /* ********************************************************************* */
    /* Core Timer */
    // Set the count value
    _CP0_SET_COUNT(0x0); 
    // Set the compare value
    _CP0_SET_COMPARE(CORE_TIMER_COMPARE_VAL); 
    // Interrupcion
    //    Priority: 1
    IPC0bits.CTIP = 1;
    //    Sub Priority: 0
    IPC0bits.CTIS = 0;
     // Enable the interrupt
    IEC0bits.CTIE = 1;
    /* ********************************************************************* */
    /* TIMER1 */
    IPC4bits.T1IP = 0x0001 ;
    IFS0CLR = _IFS0_T1IF_MASK; //Clear T1IF
    TMR1 = 0 ;
    T1CON = 0x00000000;
    T1CONbits.TCKPS0 = 0;
    T1CONbits.TCKPS1 = 0;
    T1CONbits.ON = 1;
    PR1 = 3427;
    // Enable TIMER1 Int
    IEC0bits.T1IE = 1 ;
    /* INTERRUPT Initialize */
    // Enable Multi Vector Configuration
    INTCONbits.MVEC = 1;
    __builtin_enable_interrupts();
}

void SystemReset( void )
{
    SYSKEY = 0x00000000; //write invalid key to force lock
    SYSKEY = 0xAA996655; //write key1 to SYSKEY
    SYSKEY = 0x556699AA; //write key2 to SYSKEY
    /* set SWRST bit to arm reset */
    RSWRSTSET = 1;
    /* read RSWRST register to trigger reset */
    unsigned int dummy; dummy = RSWRST;
    /* prevent any unwanted code execution until reset occurs*/
    while(dummy | 1);
}