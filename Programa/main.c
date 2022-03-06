/**
  Generated main.c file from MPLAB Code Configurator

  @Company
    Microchip Technology Inc.

  @File Name
    main.c

  @Summary
    This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.170.0
        Device            :  PIC32MM0256GPM028
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.61
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

/* *****************************************************************************
 * Asignación de pines para PIC32MM0064GPL028 en DomTerm
 * 1    MCLR / Vpp                      Programación/Debug/Reset
 * 2    RA0                 out         Led 1 - Status
 * 3    RA1                 out         Led 2 - Modo
 * 4    RB0/PGED1                       Programacion/Debug
 * 5    RB1/PGEC1                       Programacion/Debug
 * 6    RB2                 in          I1
 * 7    RB3                 in          I2
 * 8    Vss                             Vss
 * 9    RA2/RP3             in          U2RX
 * 10   RA3/RP4             out         U1TX
 * 11   RB4/RP5             in          I3
 * 12   RA4/RP6             out         Led 3
 * 13   Vdd                             +3v3
 * 14   RB5                 in          I4
 * 15   RB6
 * 16   RB7                 in          I5
 * 17   RB8/SCK1            in          I6
 * 18   RB9/SDO1            out         O4
 * 19   RC9                 out         O3
 * 20   VCap                            Cap 100n + 10uF
 * 21   RB10/RP17           out         O2
 * 22   RB11/RP18           out         O1
 * 23   Vusb 3v3
 * 24   RB13/RP13           out/oc      WIFI_RESET -> ESP-WROOM-02_Reset-pin15
 * 25   RB14/RP19           tx          U1TX -> ESP-WROOM-02_Rx-pin11
 * 26   RB15/RP10           rx          U1RX -> ESP-WROOM-02_Tx-pin12
 * 27   AVss                            Vss
 * 28   AVdd                            +3v3
 *
 * ****************************************************************************/

/**
  Section: Included Files
*/
#include "mcc_generated_files/system.h"

#include "mcc_generated_files/watchdog.h"

#include <stdio.h>

#include "led.h"
#include "wifi.h"
#include "queue.h"
#include "config.h"
#include "http.h"

/*
                         Main application
 */
int main(void)
{
    // initialize the device
    QueueInit();
    SYSTEM_Initialize();
    ConfigInit();
    LedInit();
    ConfigLoad();
    WiFiInit();
    HTTPInit();
    printf("Init complete\r\n");
    
    Led1(10);
    Led2(10);
    
    while (1)
    {
        // Add your application code
        WATCHDOG_TimerClear();
        
        LedTask();
        WiFiTask();
        ConfigTask();
        HTTPTask();
        
    }
    return 1; 
}
/**
 End of File
*/

