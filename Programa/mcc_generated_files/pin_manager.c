/**
  PIN MANAGER Generated Driver File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.c

  @Summary:
    This is the generated manager file for the PIC24 / dsPIC33 / PIC32MM MCUs device.  This manager
    configures the pins direction, initial state, analog setting.
    The peripheral pin select, PPS, configuration is also handled by this manager.

  @Description:
    This source file provides implementations for PIN MANAGER.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.170.0
        Device            :  PIC32MM0256GPM028
    The generated drivers are tested against the following:
        Compiler          :  XC32 v2.50
        MPLAB 	          :  MPLAB X v5.45
*/

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/


/**
    Section: Includes
*/

#include <xc.h>
#include "pin_manager.h"
#include "system.h"

/**
 Section: File specific functions
*/
void (*EXP05_D2_InterruptHandler)(void) = NULL;
void (*EXP08_D1_InterruptHandler)(void) = NULL;
void (*IN1_InterruptHandler)(void) = NULL;
void (*IN3_InterruptHandler)(void) = NULL;
void (*IN2_InterruptHandler)(void) = NULL;
void (*EXP04_D0_InterruptHandler)(void) = NULL;
void (*IN4_InterruptHandler)(void) = NULL;
void (*EXP03_InterruptHandler)(void) = NULL;
void (*EXP01_InterruptHandler)(void) = NULL;
void (*EXP07_InterruptHandler)(void) = NULL;

/**
 Section: Driver Interface Function Definitions
*/
void PIN_MANAGER_Initialize (void)
{
    /****************************************************************************
     * Setting the Output Latch SFR(s)
     ***************************************************************************/
    LATA = 0x0000;
    LATB = 0x4000;
    LATC = 0x0000;

    /****************************************************************************
     * Setting the GPIO Direction SFR(s)
     ***************************************************************************/
    TRISA = 0x001C;
    TRISB = 0x8FFF;
    TRISC = 0x0200;

    /****************************************************************************
     * Setting the Weak Pull Up and Weak Pull Down SFR(s)
     ***************************************************************************/
    CNPDA = 0x0000;
    CNPDB = 0x0000;
    CNPDC = 0x0000;
    CNPUA = 0x000C;
    CNPUB = 0x01F0;
    CNPUC = 0x0000;

    /****************************************************************************
     * Setting the Open Drain SFR(s)
     ***************************************************************************/
    ODCA = 0x0000;
    ODCB = 0x2000;
    ODCC = 0x0000;

    /****************************************************************************
     * Setting the Analog/Digital Configuration SFR(s)
     ***************************************************************************/
    ANSELA = 0x0000;
    ANSELB = 0x000C;


    /****************************************************************************
     * Interrupt On Change: any
     ***************************************************************************/
    CNEN0Abits.CNIE0A2 = 1;    //Pin : RA2
    CNEN0Abits.CNIE0A3 = 1;    //Pin : RA3
    CNEN0Bbits.CNIE0B10 = 1;    //Pin : RB10
    CNEN0Bbits.CNIE0B11 = 1;    //Pin : RB11
    CNEN0Bbits.CNIE0B4 = 1;    //Pin : RB4
    CNEN0Bbits.CNIE0B5 = 1;    //Pin : RB5
    CNEN0Bbits.CNIE0B6 = 1;    //Pin : RB6
    CNEN0Bbits.CNIE0B7 = 1;    //Pin : RB7
    CNEN0Bbits.CNIE0B8 = 1;    //Pin : RB8
    CNEN0Bbits.CNIE0B9 = 1;    //Pin : RB9
    CNEN1Abits.CNIE1A2 = 1;    //Pin : RA2
    CNEN1Abits.CNIE1A3 = 1;    //Pin : RA3
    CNEN1Bbits.CNIE1B10 = 1;    //Pin : RB10
    CNEN1Bbits.CNIE1B11 = 1;    //Pin : RB11
    CNEN1Bbits.CNIE1B4 = 1;    //Pin : RB4
    CNEN1Bbits.CNIE1B5 = 1;    //Pin : RB5
    CNEN1Bbits.CNIE1B6 = 1;    //Pin : RB6
    CNEN1Bbits.CNIE1B7 = 1;    //Pin : RB7
    CNEN1Bbits.CNIE1B8 = 1;    //Pin : RB8
    CNEN1Bbits.CNIE1B9 = 1;    //Pin : RB9
    /****************************************************************************
     * Interrupt On Change: flag
     ***************************************************************************/
    CNFAbits.CNFA2 = 0;    //Pin : RA2
    CNFAbits.CNFA3 = 0;    //Pin : RA3
    CNFBbits.CNFB10 = 0;    //Pin : RB10
    CNFBbits.CNFB11 = 0;    //Pin : RB11
    CNFBbits.CNFB4 = 0;    //Pin : RB4
    CNFBbits.CNFB5 = 0;    //Pin : RB5
    CNFBbits.CNFB6 = 0;    //Pin : RB6
    CNFBbits.CNFB7 = 0;    //Pin : RB7
    CNFBbits.CNFB8 = 0;    //Pin : RB8
    CNFBbits.CNFB9 = 0;    //Pin : RB9
    /****************************************************************************
     * Interrupt On Change: config
     ***************************************************************************/
    CNCONAbits.CNSTYLE = 1;    //Config for PORTA
    CNCONAbits.ON = 1;    //Config for PORTA
    CNCONBbits.CNSTYLE = 1;    //Config for PORTB
    CNCONBbits.ON = 1;    //Config for PORTB
    
    /* Initialize IOC Interrupt Handler*/
    EXP05_D2_SetInterruptHandler(&EXP05_D2_CallBack);
    EXP08_D1_SetInterruptHandler(&EXP08_D1_CallBack);
    IN1_SetInterruptHandler(&IN1_CallBack);
    IN3_SetInterruptHandler(&IN3_CallBack);
    IN2_SetInterruptHandler(&IN2_CallBack);
    EXP04_D0_SetInterruptHandler(&EXP04_D0_CallBack);
    IN4_SetInterruptHandler(&IN4_CallBack);
    EXP03_SetInterruptHandler(&EXP03_CallBack);
    EXP01_SetInterruptHandler(&EXP01_CallBack);
    EXP07_SetInterruptHandler(&EXP07_CallBack);
    
    /****************************************************************************
     * Interrupt On Change: Interrupt Enable
     ***************************************************************************/
    IFS0CLR= 1 << _IFS0_CNAIF_POSITION; //Clear CNAI interrupt flag
    IEC0bits.CNAIE = 1; //Enable CNAI interrupt
    IFS0CLR= 1 << _IFS0_CNBIF_POSITION; //Clear CNBI interrupt flag
    IEC0bits.CNBIE = 1; //Enable CNBI interrupt
}

void __attribute__ ((weak)) EXP05_D2_CallBack(void)
{

}

void __attribute__ ((weak)) EXP08_D1_CallBack(void)
{

}

void __attribute__ ((weak)) IN1_CallBack(void)
{

}

void __attribute__ ((weak)) IN3_CallBack(void)
{

}

void __attribute__ ((weak)) IN2_CallBack(void)
{

}

void __attribute__ ((weak)) EXP04_D0_CallBack(void)
{

}

void __attribute__ ((weak)) IN4_CallBack(void)
{

}

void __attribute__ ((weak)) EXP03_CallBack(void)
{

}

void __attribute__ ((weak)) EXP01_CallBack(void)
{

}

void __attribute__ ((weak)) EXP07_CallBack(void)
{

}

void EXP05_D2_SetInterruptHandler(void (* InterruptHandler)(void))
{ 
    IEC0bits.CNAIE = 0; //Disable CNAI interrupt
    EXP05_D2_InterruptHandler = InterruptHandler; 
    IEC0bits.CNAIE = 1; //Enable CNAI interrupt
}

void EXP05_D2_SetIOCInterruptHandler(void *handler)
{ 
    EXP05_D2_SetInterruptHandler(handler);
}

void EXP08_D1_SetInterruptHandler(void (* InterruptHandler)(void))
{ 
    IEC0bits.CNAIE = 0; //Disable CNAI interrupt
    EXP08_D1_InterruptHandler = InterruptHandler; 
    IEC0bits.CNAIE = 1; //Enable CNAI interrupt
}

void EXP08_D1_SetIOCInterruptHandler(void *handler)
{ 
    EXP08_D1_SetInterruptHandler(handler);
}

void IN1_SetInterruptHandler(void (* InterruptHandler)(void))
{ 
    IEC0bits.CNBIE = 0; //Disable CNBI interrupt
    IN1_InterruptHandler = InterruptHandler; 
    IEC0bits.CNBIE = 1; //Enable CNBI interrupt
}

void IN1_SetIOCInterruptHandler(void *handler)
{ 
    IN1_SetInterruptHandler(handler);
}

void IN3_SetInterruptHandler(void (* InterruptHandler)(void))
{ 
    IEC0bits.CNBIE = 0; //Disable CNBI interrupt
    IN3_InterruptHandler = InterruptHandler; 
    IEC0bits.CNBIE = 1; //Enable CNBI interrupt
}

void IN3_SetIOCInterruptHandler(void *handler)
{ 
    IN3_SetInterruptHandler(handler);
}

void IN2_SetInterruptHandler(void (* InterruptHandler)(void))
{ 
    IEC0bits.CNBIE = 0; //Disable CNBI interrupt
    IN2_InterruptHandler = InterruptHandler; 
    IEC0bits.CNBIE = 1; //Enable CNBI interrupt
}

void IN2_SetIOCInterruptHandler(void *handler)
{ 
    IN2_SetInterruptHandler(handler);
}

void EXP04_D0_SetInterruptHandler(void (* InterruptHandler)(void))
{ 
    IEC0bits.CNBIE = 0; //Disable CNBI interrupt
    EXP04_D0_InterruptHandler = InterruptHandler; 
    IEC0bits.CNBIE = 1; //Enable CNBI interrupt
}

void EXP04_D0_SetIOCInterruptHandler(void *handler)
{ 
    EXP04_D0_SetInterruptHandler(handler);
}

void IN4_SetInterruptHandler(void (* InterruptHandler)(void))
{ 
    IEC0bits.CNBIE = 0; //Disable CNBI interrupt
    IN4_InterruptHandler = InterruptHandler; 
    IEC0bits.CNBIE = 1; //Enable CNBI interrupt
}

void IN4_SetIOCInterruptHandler(void *handler)
{ 
    IN4_SetInterruptHandler(handler);
}

void EXP03_SetInterruptHandler(void (* InterruptHandler)(void))
{ 
    IEC0bits.CNBIE = 0; //Disable CNBI interrupt
    EXP03_InterruptHandler = InterruptHandler; 
    IEC0bits.CNBIE = 1; //Enable CNBI interrupt
}

void EXP03_SetIOCInterruptHandler(void *handler)
{ 
    EXP03_SetInterruptHandler(handler);
}

void EXP01_SetInterruptHandler(void (* InterruptHandler)(void))
{ 
    IEC0bits.CNBIE = 0; //Disable CNBI interrupt
    EXP01_InterruptHandler = InterruptHandler; 
    IEC0bits.CNBIE = 1; //Enable CNBI interrupt
}

void EXP01_SetIOCInterruptHandler(void *handler)
{ 
    EXP01_SetInterruptHandler(handler);
}

void EXP07_SetInterruptHandler(void (* InterruptHandler)(void))
{ 
    IEC0bits.CNBIE = 0; //Disable CNBI interrupt
    EXP07_InterruptHandler = InterruptHandler; 
    IEC0bits.CNBIE = 1; //Enable CNBI interrupt
}

void EXP07_SetIOCInterruptHandler(void *handler)
{ 
    EXP07_SetInterruptHandler(handler);
}

/* Interrupt service routine for the CNAI interrupt. */
void __attribute__ ((vector(_CHANGE_NOTICE_A_VECTOR), interrupt(IPL1SOFT))) _CHANGE_NOTICE_A( void )
{
    if(IFS0bits.CNAIF == 1)
    {
        if(CNFAbits.CNFA3 == 1)
        {
            if(EXP05_D2_InterruptHandler) 
            { 
                EXP05_D2_InterruptHandler(); 
            }
            
            CNFACLR = 0x8;  //Clear CNFAbits.CNFA3
        }
        
        if(CNFAbits.CNFA2 == 1)
        {
            if(EXP08_D1_InterruptHandler) 
            { 
                EXP08_D1_InterruptHandler(); 
            }
            
            CNFACLR = 0x4;  //Clear CNFAbits.CNFA2
        }
        
        // Clear the flag
        IFS0CLR= 1 << _IFS0_CNAIF_POSITION; // Clear IFS0bits.CNAIF
    }
}

/* Interrupt service routine for the CNBI interrupt. */
void __attribute__ ((vector(_CHANGE_NOTICE_B_VECTOR), interrupt(IPL1SOFT))) _CHANGE_NOTICE_B( void )
{
    if(IFS0bits.CNBIF == 1)
    {
        if(CNFBbits.CNFB4 == 1)
        {
            if(IN1_InterruptHandler) 
            { 
                IN1_InterruptHandler(); 
            }
            
            CNFBCLR = 0x10;  //Clear CNFBbits.CNFB4
        }
        
        if(CNFBbits.CNFB6 == 1)
        {
            if(IN3_InterruptHandler) 
            { 
                IN3_InterruptHandler(); 
            }
            
            CNFBCLR = 0x40;  //Clear CNFBbits.CNFB6
        }
        
        if(CNFBbits.CNFB5 == 1)
        {
            if(IN2_InterruptHandler) 
            { 
                IN2_InterruptHandler(); 
            }
            
            CNFBCLR = 0x20;  //Clear CNFBbits.CNFB5
        }
        
        if(CNFBbits.CNFB8 == 1)
        {
            if(EXP04_D0_InterruptHandler) 
            { 
                EXP04_D0_InterruptHandler(); 
            }
            
            CNFBCLR = 0x100;  //Clear CNFBbits.CNFB8
        }
        
        if(CNFBbits.CNFB7 == 1)
        {
            if(IN4_InterruptHandler) 
            { 
                IN4_InterruptHandler(); 
            }
            
            CNFBCLR = 0x80;  //Clear CNFBbits.CNFB7
        }
        
        if(CNFBbits.CNFB9 == 1)
        {
            if(EXP03_InterruptHandler) 
            { 
                EXP03_InterruptHandler(); 
            }
            
            CNFBCLR = 0x200;  //Clear CNFBbits.CNFB9
        }
        
        if(CNFBbits.CNFB10 == 1)
        {
            if(EXP01_InterruptHandler) 
            { 
                EXP01_InterruptHandler(); 
            }
            
            CNFBCLR = 0x400;  //Clear CNFBbits.CNFB10
        }
        
        if(CNFBbits.CNFB11 == 1)
        {
            if(EXP07_InterruptHandler) 
            { 
                EXP07_InterruptHandler(); 
            }
            
            CNFBCLR = 0x800;  //Clear CNFBbits.CNFB11
        }
        
        // Clear the flag
        IFS0CLR= 1 << _IFS0_CNBIF_POSITION; // Clear IFS0bits.CNBIF
    }
}

