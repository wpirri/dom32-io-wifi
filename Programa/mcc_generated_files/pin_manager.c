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

#include "../wiegand.h"

/**
 Section: File specific functions
*/
//void (*IO6_InterruptHandler)(void) = NULL;
//void (*IO5_InterruptHandler)(void) = NULL;

/**
 Section: Driver Interface Function Definitions
*/
void PIN_MANAGER_Initialize (void)
{
    /****************************************************************************
     * Setting the Output Latch SFR(s)
     ***************************************************************************/
    LATA = 0x0000;
    LATB = 0x0000;
    LATC = 0x0000;

    /****************************************************************************
     * Setting the GPIO Direction SFR(s)
     ***************************************************************************/
    TRISA = 0x0004;
    TRISB = 0x81FF;
    TRISC = 0x0000;

    /****************************************************************************
     * Setting the Weak Pull Up and Weak Pull Down SFR(s)
     ***************************************************************************/
    CNPDA = 0x0000;
    CNPDB = 0x0000;
    CNPDC = 0x0000;
    CNPUA = 0x0000;
    CNPUB = 0x01BC;
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
    ANSELB = 0x0000;

    /****************************************************************************
     * Set the PPS
     ***************************************************************************/
    SYSTEM_RegUnlock(); // unlock PPS
    RPCONbits.IOLOCK = 0;
#ifdef __SE_HACE_EN_LogInit
    RPOR0bits.RP4R = 0x0004;    //RA3->UART2:U2TX
    RPINR9bits.U2RXR = 0x0000;    //RA2->NADA - ( 0x0003 RA2->UART2:U2RX )
#endif
    RPCONbits.IOLOCK = 1; // lock   PPS
    SYSTEM_RegLock(); 

    /****************************************************************************
     * Interrupt On Change: negative
     ***************************************************************************/
    CNEN1Bbits.CNIE1B7 = 1;    //Pin : RB7
    CNEN1Bbits.CNIE1B8 = 1;    //Pin : RB8
    /****************************************************************************
     * Interrupt On Change: flag
     ***************************************************************************/
    CNFBbits.CNFB7 = 0;    //Pin : RB7
    CNFBbits.CNFB8 = 0;    //Pin : RB8
    /****************************************************************************
     * Interrupt On Change: config
     ***************************************************************************/
    CNCONBbits.CNSTYLE = 1;    //Config for PORTB
    CNCONBbits.ON = 1;    //Config for PORTB
    
    /* Initialize IOC Interrupt Handler*/
//    IO6_SetInterruptHandler(&IO6_CallBack);
//    IO5_SetInterruptHandler(&IO5_CallBack);
    
#ifdef __SE_HACE_EN_CTRLInit__
    /****************************************************************************
     * Interrupt On Change: Interrupt Enable
     ***************************************************************************/
    IFS0CLR= 1 << _IFS0_CNBIF_POSITION; //Clear CNBI interrupt flag
    IEC0bits.CNBIE = 1; //Enable CNBI interrupt
#endif /* __SE_HACE_EN_CTRLInit__ */        
}

//void __attribute__ ((weak)) EXT_Data_CallBack(void)
//{
//
//}

//void __attribute__ ((weak)) IO6_CallBack(void)
//{
//
//}

//void __attribute__ ((weak)) IO5_CallBack(void)
//{
//
//}

//void IO6_SetInterruptHandler(void (* InterruptHandler)(void))
//{ 
//    IEC0bits.CNBIE = 0; //Disable CNBI interrupt
//    IO6_InterruptHandler = InterruptHandler; 
//    IEC0bits.CNBIE = 1; //Enable CNBI interrupt
//}

//void IO6_SetIOCInterruptHandler(void *handler)
//{ 
//    IO6_SetInterruptHandler(handler);
//}

//void IO5_SetInterruptHandler(void (* InterruptHandler)(void))
//{ 
//    IEC0bits.CNBIE = 0; //Disable CNBI interrupt
//    IO5_InterruptHandler = InterruptHandler; 
//    IEC0bits.CNBIE = 1; //Enable CNBI interrupt
//}

//void IO5_SetIOCInterruptHandler(void *handler)
//{ 
//    IO5_SetInterruptHandler(handler);
//}

//void EXT_Data_SetInterruptHandler(void (* InterruptHandler)(void))
//{ 
//    IEC0bits.CNAIE = 0; //Disable CNAI interrupt
//    EXT_Data_InterruptHandler = InterruptHandler; 
//    IEC0bits.CNAIE = 1; //Enable CNAI interrupt
//}

/* Interrupt service routine for the CNBI interrupt. */
void __attribute__ ((vector(_CHANGE_NOTICE_B_VECTOR), interrupt(IPL1SOFT))) _CHANGE_NOTICE_B( void )
{
    if(IFS0bits.CNBIF == 1)
    {
        WGND_IntData( CNFBbits.CNFB8, CNFBbits.CNFB7 );
        if(CNFBbits.CNFB8 == 1)
        {
//            if(IO6_InterruptHandler) 
//            { 
//                IO6_InterruptHandler(); 
//            }
            
            CNFBCLR = 0x100;  //Clear CNFBbits.CNFB8
        }
        
        if(CNFBbits.CNFB7 == 1)
        {
//            if(IO5_InterruptHandler) 
//            { 
//                IO5_InterruptHandler(); 
//            }
//            
            CNFBCLR = 0x80;  //Clear CNFBbits.CNFB7
        }
        
        // Clear the flag
        IFS0CLR= 1 << _IFS0_CNBIF_POSITION; // Clear IFS0bits.CNBIF
    }
}

