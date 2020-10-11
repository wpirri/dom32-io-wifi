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
// PIC32MM0064GPL036 Configuration Bit Settings
// 'C' source line config statements
// FDEVOPT
#pragma config SOSCHP = OFF             // Secondary Oscillator High Power Enable bit (SOSC oprerates in normal power mode.)
#pragma config USERID = 0xFFFF          // User ID bits (User ID bits)
// FICD
#pragma config JTAGEN = OFF             // JTAG Enable bit (JTAG is disabled)
#pragma config ICS = PGx1               // ICE/ICD Communication Channel Selection bits (Communicate on PGEC3/PGED3)
// FPOR
#pragma config BOREN = BOR3             // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware; SBOREN bit disabled)
#pragma config RETVR = OFF              // Retention Voltage Regulator Enable bit (Retention regulator is disabled)
#pragma config LPBOREN = OFF            // Low Power Brown-out Enable bit (Low power BOR is disabled)
// FWDT
#pragma config SWDTPS = PS1048576       // Sleep Mode Watchdog Timer Postscale Selection bits (1:1048576)
#pragma config FWDTWINSZ = PS25_0       // Watchdog Timer Window Size bits (Watchdog timer window size is 25%)
#pragma config WINDIS = OFF             // Windowed Watchdog Timer Disable bit (Watchdog timer is in non-window mode)
#pragma config RWDTPS = PS1048576       // Run Mode Watchdog Timer Postscale Selection bits (1:1048576)
#pragma config RCLKSEL = LPRC           // Run Mode Watchdog Timer Clock Source Selection bits (Clock source is LPRC (same as for sleep mode))
#pragma config FWDTEN = OFF             // Watchdog Timer Enable bit (WDT is disabled)
// FOSCSEL
#pragma config FNOSC = FRCDIV           // Oscillator Selection bits (Fast RC oscillator (FRC) with divide-by-N)
#pragma config PLLSRC = FRC             // System PLL Input Clock Selection bit (FRC oscillator is selected as PLL reference input on device reset)
#pragma config SOSCEN = OFF             // Secondary Oscillator Enable bit (Secondary oscillator (SOSC) is disabled)
#pragma config IESO = OFF               // Two Speed Startup Enable bit (Two speed startup is disabled)
#pragma config POSCMOD = OFF            // Primary Oscillator Selection bit (Primary oscillator is disabled)
#pragma config OSCIOFNC = OFF           // System Clock on CLKO Pin Enable bit (OSCO pin operates as a normal I/O)
#pragma config SOSCSEL = ON             // Secondary Oscillator External Clock Enable bit (Crystal is used (RA4 and RB4 are controlled by SOSC))
#pragma config FCKSM = CSECMD           // Clock Switching and Fail-Safe Clock Monitor Enable bits (Clock switching is enabled; Fail-safe clock monitor is disabled)
// FSEC
#pragma config CP = OFF                 // Code Protection Enable bit (Code protection is disabled)
/* ************************************************************************** */
#include <xc.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hwconf.h"

/* ************************************************************************** */
typedef enum {
    MAIN_STM_RESET,
    MAIN_STM_WAIT_POST,
    MAIN_STM_POST,
    MAIN_STM_WAIT_PING,
    MAIN_STM_WAIT_DATA,
    MAIN_STM_INFO,
    MAIN_STM_MENU
} MAIN_STM;

/* ************************************************************************** */
volatile unsigned char g_main_stm_count;
volatile unsigned int g_timer10xs_count;
volatile unsigned char g_update_clock_flag;
volatile unsigned char g_timer10xs_flag;
unsigned char g_update_clima_count;

unsigned int g_resend_delay;
unsigned char g_retry_count;

/* ************************************************************************** */
/* Section: Exception Handling */
static unsigned int _excep_code;
static unsigned int _excep_addr;
static char *_cause_str;
static char *cause[] = 
{
    "Interrupt",
    "Undefined",
    "Undefined",
    "Undefined",
    "Load/fetch address error",
    "Store address error",
    "Instruction bus error",
    "Data bus error",
    "Syscall",
    "Breakpoint",
    "Reserved instruction",
    "Coprocessor unusable",
    "Arithmetic overflow",
    "Trap",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void _general_exception_handler ( void )
{
    _excep_code = (_CP0_GET_CAUSE() & 0x0000007C) >> 2;
    _excep_addr = _CP0_GET_EPC();
    _cause_str  = cause[_excep_code];

    while(1)
    {
        _nop();
    }
}

/* ************************************************************************** */
/* TIMER1 - Interrupcion a 12KHz (83us) */
void __attribute__ ( ( vector(_TIMER_1_VECTOR) , interrupt(IPL7SOFT) ) ) T1Interrupt ( void )
{
    IFS0CLR = _IFS0_T1IF_MASK; //Clear T1IF

    
}

/* ************************************************************************** */
/* RTCC - Interrupcion a 1/2 seg */
//void __attribute__ ((vector(_RTCC_VECTOR), interrupt(IPL1SOFT))) _ISR_RTCC(void) 
//{
//    IFS0CLR = _IFS0_RTCCIF_MASK;
//
//}

/* ************************************************************************** */
/* Interrupt service routine for the CNBI interrupt. */
//void __attribute__ ((vector(_CHANGE_NOTICE_B_VECTOR), interrupt(IPL1SOFT))) _CHANGE_NOTICE_B( void )
//{
//    if(IFS0bits.CNBIF == 1)
//    {
//        // Clear the flag
//        IFS0CLR= 1 << _IFS0_CNBIF_POSITION; //Clear IFS0bits.CNBIF
//        // interrupt on change for group CNFB
//        if(CNFBbits.CNFB12 == 1)
//        {
//            CNFBCLR = 0x1000;  //Clear CNFBbits.CNFB12
//
//        }
//    }
//}

/* ************************************************************************** */
int main(int argc, char** argv)
{
    INTCONbits.MVEC = 1;
    asm volatile("ei $0");

/* ************************************************************************** */
/*  Setting the Output Latch SFR(s) */
    LATA = 0x00000000;
    LATB = 0x00000000;
    LATC = 0x00000000;

/* ************************************************************************** */
/* Setting the GPIO Direction SFR(s) */
    TRISA = 0xFFFFFFFF;
    TRISB = 0xFFFFFFFF;
    TRISC = 0xFFFFFFFF;
    
/* ************************************************************************** */
/* Setting the Weak Pull Up and Weak Pull Down SFR(s) */
    CNPDA = 0x00000000;
    CNPDB = 0x00000000;
    CNPDC = 0x00000000;
    CNPUA = 0x00000000;
    CNPUB = 0x00000000;
    CNPUC = 0x00000000;

/* ************************************************************************** */
/* Setting the Open Drain SFR(s) */
    ODCA = 0x00000000;
    ODCB = 0x00000000;
    ODCC = 0x00000000;

/* ************************************************************************** */
/* Setting the Analog/Digital Configuration SFR(s) */
    ANSELA = 0x00000000;
    ANSELB = 0x00000000;

/* ************************************************************************** */
/* M�dulo CPP */
    CCP1CON1 = 0x00000000;    
    CCP2CON1 = 0x00000000;    
    CCP3CON1 = 0x00000000;    
    
/* ************************************************************************** */
/* Change notification */
    CNCONB = 0x00000000;

/* ************************************************************************** */
/* Set the PPS */
    // System Reg Unlock
    SYSKEY = 0x12345678; //write invalid key to force lock
    SYSKEY = 0xAA996655; //write Key1 to SYSKEY
    SYSKEY = 0x556699AA; //write Key2 to SYSKEY
    // unlock PPS
//    RPCONbits.IOLOCK = 0;
//    // Set PPS - Para usar Tx Rx Serial en caso de Wifi en lugar de RF315
//    RPINR9bits.U2RXR = 10;   // U2RX --> RP10 (RB15 - Pin 26)
//    // 1=TX, 2=U2RTS, 3=SDO...
//    RPOR2bits.RP9R = 1;      // RP9 (RB14 - Pin 25) --> U2TX
//    // lock   PPS
//    RPCONbits.IOLOCK = 1; 
    // System Reg Lock
    SYSKEY = 0x00000000; 
/* ************************************************************************** */
/* OSCILLATOR Init */
    // System Reg Unlock
    SYSKEY = 0x12345678; //write invalid key to force lock
    SYSKEY = 0xAA996655; //write Key1 to SYSKEY
    SYSKEY = 0x556699AA; //write Key2 to SYSKEY
    // CF No Clock Failure; FRCDIV FRC/1; SLPEN Device will enter Idle mode when a WAIT instruction is issued; NOSC SPLL; SOSCEN disabled; CLKLOCK Clock and PLL selections are not locked and may be modified; OSWEN Complete; 
    OSCCON = 0x100;
    // TUN Center frequency; 
    OSCTUN = 0x0;
     // PLLODIV 1:1; PLLMULT 4x; PLLICLK FRC; 
    SPLLCON = 0x20080;
    // SWRST disabled; 
    RSWRST = 0x0;
    // WDTO disabled; GNMI disabled; CF disabled; WDTS disabled; NMICNT 0; LVD disabled; SWNMI disabled; 
    RNMICON = 0x0;
    // SBOREN disabled; VREGS disabled; RETEN disabled; 
    PWRCON = 0x0;
    // System Reg Lock
    SYSKEY = 0x00000000; 
    // WDTO disabled; EXTR disabled; POR disabled; SLEEP disabled; BOR disabled; PORIO disabled; IDLE disabled; PORCORE disabled; BCFGERR disabled; CMR disabled; BCFGFAIL disabled; SWR disabled; 
    RCON = 0x0;
    // ON disabled; DIVSWEN disabled; RSLP disabled; ROSEL SYSCLK; OE disabled; SIDL disabled; RODIV 0; 
    REFO1CON = 0x0;
    // ROTRIM 0; 
    REFO1TRIM = 0x0;
    // SPDIVRDY disabled; 
    CLKSTAT = 0x0;
/* ************************************************************************** */
/* INTERRUPT Initialize */
    // Enable Multi Vector Configuration
    INTCONbits.MVEC = 1;
/* ************************************************************************** */
/* TIMER1 */
    IPC2bits.T1IP = 0x0001 ;
    IFS0CLR = _IFS0_T1IF_MASK; //Clear T1IF
    TMR1 = 0 ;
    T1CON = 0x00000000;
    T1CONbits.TCKPS0 = 0;
    T1CONbits.TCKPS1 = 0;
    T1CONbits.ON = 1;
    PR1 = 666;  /* 12KHz */
    // Enable TIMER1 Int
    IEC0bits.T1IE = 1 ;
/* ************************************************************************** */
/* RTCC: Real Time Clock Alarm */
    //    Priority: 1
//    IPC3bits.RTCCIP = 1;
    //    Sub Priority: 0
//    IPC3bits.RTCCIS = 0;
    // Dejo la habilitaci�n de la interrupcion
    // para cuando est� inicializado el RTCC
/* ************************************************************************** */
/* CNBI: PORT B Change Notification */
//    //    Priority: 1
//    IPC2bits.CNBIP = 1;
//    //    Sub Priority: 0
//    IPC2bits.CNBIS = 0;
//    /****************************************************************************
//     * Interrupt On Change for group CNCONB - config
//     ***************************************************************************/
//	CNCONBbits.ON = 1; 
//	CNCONBbits.CNSTYLE = 1; 
//    /****************************************************************************
//     * Interrupt On Change for group CNEN0B - positive
//     ***************************************************************************/
//	CNEN0Bbits.CNIE0B12 = 1; // Pin : RB12
//    /****************************************************************************
//     * Interrupt On Change for group CNEN1B - negative
//     ***************************************************************************/
//	CNEN1Bbits.CNIE1B12 = 1; // Pin : RB12
//    /****************************************************************************
//     * Interrupt On Change for group CNFB - flag
//     ***************************************************************************/
//	CNFBbits.CNFB12 = 0; // Pin : RB12
//
//    // Enable CNBI interrupt 
//    IEC0bits.CNBIE = 1;
/* ************************************************************************** */
    LED_STATUS_IO = 0;
    IN1_IO = 1;
    IN1_PU = 1;
    IN2_IO = 1;
    IN2_PU = 1;
    IN3_IO = 1;
    IN3_PU = 1;
    IN4_IO = 1;
    IN4_PU = 1;
   
    
/* ************************************************************************** */
    __builtin_enable_interrupts();
    while(1)
    {



    }
    return (EXIT_SUCCESS);
}
