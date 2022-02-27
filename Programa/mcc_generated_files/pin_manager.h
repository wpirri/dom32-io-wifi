/**
  PIN MANAGER Generated Driver File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

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

#ifndef _PIN_MANAGER_H
#define _PIN_MANAGER_H
/**
    Section: Includes
*/
#include <xc.h>
#include <stdbool.h>
/**
    Section: Device Pin Macros
*/
/**
  @Summary
    Sets the GPIO pin, RA0, high using LATA0.

  @Description
    Sets the GPIO pin, RA0, high using LATA0.

  @Preconditions
    The RA0 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RA0 high (1)
    LED1_SetHigh();
    </code>

*/
#define LED1_SetHigh()          ( LATASET = (1 << 0) )
/**
  @Summary
    Sets the GPIO pin, RA0, low using LATA0.

  @Description
    Sets the GPIO pin, RA0, low using LATA0.

  @Preconditions
    The RA0 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RA0 low (0)
    LED1_SetLow();
    </code>

*/
#define LED1_SetLow()           ( LATACLR = (1 << 0) )

/**
  @Summary
    Sets a value to the GPIO pin.

  @Description
    Sets or Resets the GPIO pin, RA0, low or high using LATA0.

  @Preconditions
    The RA0 must be set to an output.

  @Returns
    None.

  @Param
    bool value; : value to be set to the GPIO pin.

  @Example
    <code>
    // Set RA0 to low.
    LED1_SetValue(false);
    </code>

*/
inline static void LED1_SetValue(bool value)
{
  if(value)
  {
    LED1_SetHigh();
  }
  else
  {
    LED1_SetLow();
  }
}

/**
  @Summary
    Toggles the GPIO pin, RA0, using LATA0.

  @Description
    Toggles the GPIO pin, RA0, using LATA0.

  @Preconditions
    The RA0 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RA0
    LED1_Toggle();
    </code>

*/
#define LED1_Toggle()           ( LATAINV = (1 << 0) )
/**
  @Summary
    Reads the value of the GPIO pin, RA0.

  @Description
    Reads the value of the GPIO pin, RA0.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RA0
    postValue = LED1_GetValue();
    </code>

*/
#define LED1_GetValue()         PORTAbits.RA0
/**
  @Summary
    Configures the GPIO pin, RA0, as an input.

  @Description
    Configures the GPIO pin, RA0, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RA0 as an input
    LED1_SetDigitalInput();
    </code>

*/
#define LED1_SetDigitalInput()   ( TRISASET = (1 << 0) )
/**
  @Summary
    Configures the GPIO pin, RA0, as an output.

  @Description
    Configures the GPIO pin, RA0, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RA0 as an output
    LED1_SetDigitalOutput();
    </code>

*/
#define LED1_SetDigitalOutput()   ( TRISACLR = (1 << 0) )
/**
  @Summary
    Sets the GPIO pin, RA1, high using LATA1.

  @Description
    Sets the GPIO pin, RA1, high using LATA1.

  @Preconditions
    The RA1 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RA1 high (1)
    LED2_SetHigh();
    </code>

*/
#define LED2_SetHigh()          ( LATASET = (1 << 1) )
/**
  @Summary
    Sets the GPIO pin, RA1, low using LATA1.

  @Description
    Sets the GPIO pin, RA1, low using LATA1.

  @Preconditions
    The RA1 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RA1 low (0)
    LED2_SetLow();
    </code>

*/
#define LED2_SetLow()           ( LATACLR = (1 << 1) )

/**
  @Summary
    Sets a value to the GPIO pin.

  @Description
    Sets or Resets the GPIO pin, RA1, low or high using LATA1.

  @Preconditions
    The RA1 must be set to an output.

  @Returns
    None.

  @Param
    bool value; : value to be set to the GPIO pin.

  @Example
    <code>
    // Set RA1 to low.
    LED2_SetValue(false);
    </code>

*/
inline static void LED2_SetValue(bool value)
{
  if(value)
  {
    LED2_SetHigh();
  }
  else
  {
    LED2_SetLow();
  }
}

/**
  @Summary
    Toggles the GPIO pin, RA1, using LATA1.

  @Description
    Toggles the GPIO pin, RA1, using LATA1.

  @Preconditions
    The RA1 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RA1
    LED2_Toggle();
    </code>

*/
#define LED2_Toggle()           ( LATAINV = (1 << 1) )
/**
  @Summary
    Reads the value of the GPIO pin, RA1.

  @Description
    Reads the value of the GPIO pin, RA1.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RA1
    postValue = LED2_GetValue();
    </code>

*/
#define LED2_GetValue()         PORTAbits.RA1
/**
  @Summary
    Configures the GPIO pin, RA1, as an input.

  @Description
    Configures the GPIO pin, RA1, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RA1 as an input
    LED2_SetDigitalInput();
    </code>

*/
#define LED2_SetDigitalInput()   ( TRISASET = (1 << 1) )
/**
  @Summary
    Configures the GPIO pin, RA1, as an output.

  @Description
    Configures the GPIO pin, RA1, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RA1 as an output
    LED2_SetDigitalOutput();
    </code>

*/
#define LED2_SetDigitalOutput()   ( TRISACLR = (1 << 1) )
/**
  @Summary
    Sets the GPIO pin, RA3, high using LATA3.

  @Description
    Sets the GPIO pin, RA3, high using LATA3.

  @Preconditions
    The RA3 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RA3 high (1)
    U2TX_SetHigh();
    </code>

*/
#define U2TX_SetHigh()          ( LATASET = (1 << 3) )
/**
  @Summary
    Sets the GPIO pin, RA3, low using LATA3.

  @Description
    Sets the GPIO pin, RA3, low using LATA3.

  @Preconditions
    The RA3 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RA3 low (0)
    U2TX_SetLow();
    </code>

*/
#define U2TX_SetLow()           ( LATACLR = (1 << 3) )

/**
  @Summary
    Sets a value to the GPIO pin.

  @Description
    Sets or Resets the GPIO pin, RA3, low or high using LATA3.

  @Preconditions
    The RA3 must be set to an output.

  @Returns
    None.

  @Param
    bool value; : value to be set to the GPIO pin.

  @Example
    <code>
    // Set RA3 to low.
    U2TX_SetValue(false);
    </code>

*/
inline static void U2TX_SetValue(bool value)
{
  if(value)
  {
    U2TX_SetHigh();
  }
  else
  {
    U2TX_SetLow();
  }
}

/**
  @Summary
    Toggles the GPIO pin, RA3, using LATA3.

  @Description
    Toggles the GPIO pin, RA3, using LATA3.

  @Preconditions
    The RA3 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RA3
    U2TX_Toggle();
    </code>

*/
#define U2TX_Toggle()           ( LATAINV = (1 << 3) )
/**
  @Summary
    Reads the value of the GPIO pin, RA3.

  @Description
    Reads the value of the GPIO pin, RA3.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RA3
    postValue = U2TX_GetValue();
    </code>

*/
#define U2TX_GetValue()         PORTAbits.RA3
/**
  @Summary
    Configures the GPIO pin, RA3, as an input.

  @Description
    Configures the GPIO pin, RA3, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RA3 as an input
    U2TX_SetDigitalInput();
    </code>

*/
#define U2TX_SetDigitalInput()   ( TRISASET = (1 << 3) )
/**
  @Summary
    Configures the GPIO pin, RA3, as an output.

  @Description
    Configures the GPIO pin, RA3, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RA3 as an output
    U2TX_SetDigitalOutput();
    </code>

*/
#define U2TX_SetDigitalOutput()   ( TRISACLR = (1 << 3) )
/**
  @Summary
    Sets the GPIO pin, RA4, high using LATA4.

  @Description
    Sets the GPIO pin, RA4, high using LATA4.

  @Preconditions
    The RA4 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RA4 high (1)
    LED3_SetHigh();
    </code>

*/
#define LED3_SetHigh()          ( LATASET = (1 << 4) )
/**
  @Summary
    Sets the GPIO pin, RA4, low using LATA4.

  @Description
    Sets the GPIO pin, RA4, low using LATA4.

  @Preconditions
    The RA4 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RA4 low (0)
    LED3_SetLow();
    </code>

*/
#define LED3_SetLow()           ( LATACLR = (1 << 4) )

/**
  @Summary
    Sets a value to the GPIO pin.

  @Description
    Sets or Resets the GPIO pin, RA4, low or high using LATA4.

  @Preconditions
    The RA4 must be set to an output.

  @Returns
    None.

  @Param
    bool value; : value to be set to the GPIO pin.

  @Example
    <code>
    // Set RA4 to low.
    LED3_SetValue(false);
    </code>

*/
inline static void LED3_SetValue(bool value)
{
  if(value)
  {
    LED3_SetHigh();
  }
  else
  {
    LED3_SetLow();
  }
}

/**
  @Summary
    Toggles the GPIO pin, RA4, using LATA4.

  @Description
    Toggles the GPIO pin, RA4, using LATA4.

  @Preconditions
    The RA4 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RA4
    LED3_Toggle();
    </code>

*/
#define LED3_Toggle()           ( LATAINV = (1 << 4) )
/**
  @Summary
    Reads the value of the GPIO pin, RA4.

  @Description
    Reads the value of the GPIO pin, RA4.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RA4
    postValue = LED3_GetValue();
    </code>

*/
#define LED3_GetValue()         PORTAbits.RA4
/**
  @Summary
    Configures the GPIO pin, RA4, as an input.

  @Description
    Configures the GPIO pin, RA4, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RA4 as an input
    LED3_SetDigitalInput();
    </code>

*/
#define LED3_SetDigitalInput()   ( TRISASET = (1 << 4) )
/**
  @Summary
    Configures the GPIO pin, RA4, as an output.

  @Description
    Configures the GPIO pin, RA4, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RA4 as an output
    LED3_SetDigitalOutput();
    </code>

*/
#define LED3_SetDigitalOutput()   ( TRISACLR = (1 << 4) )
/**
  @Summary
    Sets the GPIO pin, RB10, high using LATB10.

  @Description
    Sets the GPIO pin, RB10, high using LATB10.

  @Preconditions
    The RB10 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RB10 high (1)
    O2_SetHigh();
    </code>

*/
#define O2_SetHigh()          ( LATBSET = (1 << 10) )
/**
  @Summary
    Sets the GPIO pin, RB10, low using LATB10.

  @Description
    Sets the GPIO pin, RB10, low using LATB10.

  @Preconditions
    The RB10 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RB10 low (0)
    O2_SetLow();
    </code>

*/
#define O2_SetLow()           ( LATBCLR = (1 << 10) )

/**
  @Summary
    Sets a value to the GPIO pin.

  @Description
    Sets or Resets the GPIO pin, RB10, low or high using LATB10.

  @Preconditions
    The RB10 must be set to an output.

  @Returns
    None.

  @Param
    bool value; : value to be set to the GPIO pin.

  @Example
    <code>
    // Set RB10 to low.
    O2_SetValue(false);
    </code>

*/
inline static void O2_SetValue(bool value)
{
  if(value)
  {
    O2_SetHigh();
  }
  else
  {
    O2_SetLow();
  }
}

/**
  @Summary
    Toggles the GPIO pin, RB10, using LATB10.

  @Description
    Toggles the GPIO pin, RB10, using LATB10.

  @Preconditions
    The RB10 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RB10
    O2_Toggle();
    </code>

*/
#define O2_Toggle()           ( LATBINV = (1 << 10) )
/**
  @Summary
    Reads the value of the GPIO pin, RB10.

  @Description
    Reads the value of the GPIO pin, RB10.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RB10
    postValue = O2_GetValue();
    </code>

*/
#define O2_GetValue()         PORTBbits.RB10
/**
  @Summary
    Configures the GPIO pin, RB10, as an input.

  @Description
    Configures the GPIO pin, RB10, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RB10 as an input
    O2_SetDigitalInput();
    </code>

*/
#define O2_SetDigitalInput()   ( TRISBSET = (1 << 10) )
/**
  @Summary
    Configures the GPIO pin, RB10, as an output.

  @Description
    Configures the GPIO pin, RB10, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RB10 as an output
    O2_SetDigitalOutput();
    </code>

*/
#define O2_SetDigitalOutput()   ( TRISBCLR = (1 << 10) )
/**
  @Summary
    Sets the GPIO pin, RB11, high using LATB11.

  @Description
    Sets the GPIO pin, RB11, high using LATB11.

  @Preconditions
    The RB11 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RB11 high (1)
    O1_SetHigh();
    </code>

*/
#define O1_SetHigh()          ( LATBSET = (1 << 11) )
/**
  @Summary
    Sets the GPIO pin, RB11, low using LATB11.

  @Description
    Sets the GPIO pin, RB11, low using LATB11.

  @Preconditions
    The RB11 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RB11 low (0)
    O1_SetLow();
    </code>

*/
#define O1_SetLow()           ( LATBCLR = (1 << 11) )

/**
  @Summary
    Sets a value to the GPIO pin.

  @Description
    Sets or Resets the GPIO pin, RB11, low or high using LATB11.

  @Preconditions
    The RB11 must be set to an output.

  @Returns
    None.

  @Param
    bool value; : value to be set to the GPIO pin.

  @Example
    <code>
    // Set RB11 to low.
    O1_SetValue(false);
    </code>

*/
inline static void O1_SetValue(bool value)
{
  if(value)
  {
    O1_SetHigh();
  }
  else
  {
    O1_SetLow();
  }
}

/**
  @Summary
    Toggles the GPIO pin, RB11, using LATB11.

  @Description
    Toggles the GPIO pin, RB11, using LATB11.

  @Preconditions
    The RB11 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RB11
    O1_Toggle();
    </code>

*/
#define O1_Toggle()           ( LATBINV = (1 << 11) )
/**
  @Summary
    Reads the value of the GPIO pin, RB11.

  @Description
    Reads the value of the GPIO pin, RB11.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RB11
    postValue = O1_GetValue();
    </code>

*/
#define O1_GetValue()         PORTBbits.RB11
/**
  @Summary
    Configures the GPIO pin, RB11, as an input.

  @Description
    Configures the GPIO pin, RB11, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RB11 as an input
    O1_SetDigitalInput();
    </code>

*/
#define O1_SetDigitalInput()   ( TRISBSET = (1 << 11) )
/**
  @Summary
    Configures the GPIO pin, RB11, as an output.

  @Description
    Configures the GPIO pin, RB11, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RB11 as an output
    O1_SetDigitalOutput();
    </code>

*/
#define O1_SetDigitalOutput()   ( TRISBCLR = (1 << 11) )
/**
  @Summary
    Sets the GPIO pin, RB2, high using LATB2.

  @Description
    Sets the GPIO pin, RB2, high using LATB2.

  @Preconditions
    The RB2 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RB2 high (1)
    I1_SetHigh();
    </code>

*/
#define I1_SetHigh()          ( LATBSET = (1 << 2) )
/**
  @Summary
    Sets the GPIO pin, RB2, low using LATB2.

  @Description
    Sets the GPIO pin, RB2, low using LATB2.

  @Preconditions
    The RB2 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RB2 low (0)
    I1_SetLow();
    </code>

*/
#define I1_SetLow()           ( LATBCLR = (1 << 2) )

/**
  @Summary
    Sets a value to the GPIO pin.

  @Description
    Sets or Resets the GPIO pin, RB2, low or high using LATB2.

  @Preconditions
    The RB2 must be set to an output.

  @Returns
    None.

  @Param
    bool value; : value to be set to the GPIO pin.

  @Example
    <code>
    // Set RB2 to low.
    I1_SetValue(false);
    </code>

*/
inline static void I1_SetValue(bool value)
{
  if(value)
  {
    I1_SetHigh();
  }
  else
  {
    I1_SetLow();
  }
}

/**
  @Summary
    Toggles the GPIO pin, RB2, using LATB2.

  @Description
    Toggles the GPIO pin, RB2, using LATB2.

  @Preconditions
    The RB2 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RB2
    I1_Toggle();
    </code>

*/
#define I1_Toggle()           ( LATBINV = (1 << 2) )
/**
  @Summary
    Reads the value of the GPIO pin, RB2.

  @Description
    Reads the value of the GPIO pin, RB2.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RB2
    postValue = I1_GetValue();
    </code>

*/
#define I1_GetValue()         PORTBbits.RB2
/**
  @Summary
    Configures the GPIO pin, RB2, as an input.

  @Description
    Configures the GPIO pin, RB2, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RB2 as an input
    I1_SetDigitalInput();
    </code>

*/
#define I1_SetDigitalInput()   ( TRISBSET = (1 << 2) )
/**
  @Summary
    Configures the GPIO pin, RB2, as an output.

  @Description
    Configures the GPIO pin, RB2, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RB2 as an output
    I1_SetDigitalOutput();
    </code>

*/
#define I1_SetDigitalOutput()   ( TRISBCLR = (1 << 2) )
/**
  @Summary
    Sets the GPIO pin, RB3, high using LATB3.

  @Description
    Sets the GPIO pin, RB3, high using LATB3.

  @Preconditions
    The RB3 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RB3 high (1)
    I2_SetHigh();
    </code>

*/
#define I2_SetHigh()          ( LATBSET = (1 << 3) )
/**
  @Summary
    Sets the GPIO pin, RB3, low using LATB3.

  @Description
    Sets the GPIO pin, RB3, low using LATB3.

  @Preconditions
    The RB3 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RB3 low (0)
    I2_SetLow();
    </code>

*/
#define I2_SetLow()           ( LATBCLR = (1 << 3) )

/**
  @Summary
    Sets a value to the GPIO pin.

  @Description
    Sets or Resets the GPIO pin, RB3, low or high using LATB3.

  @Preconditions
    The RB3 must be set to an output.

  @Returns
    None.

  @Param
    bool value; : value to be set to the GPIO pin.

  @Example
    <code>
    // Set RB3 to low.
    I2_SetValue(false);
    </code>

*/
inline static void I2_SetValue(bool value)
{
  if(value)
  {
    I2_SetHigh();
  }
  else
  {
    I2_SetLow();
  }
}

/**
  @Summary
    Toggles the GPIO pin, RB3, using LATB3.

  @Description
    Toggles the GPIO pin, RB3, using LATB3.

  @Preconditions
    The RB3 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RB3
    I2_Toggle();
    </code>

*/
#define I2_Toggle()           ( LATBINV = (1 << 3) )
/**
  @Summary
    Reads the value of the GPIO pin, RB3.

  @Description
    Reads the value of the GPIO pin, RB3.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RB3
    postValue = I2_GetValue();
    </code>

*/
#define I2_GetValue()         PORTBbits.RB3
/**
  @Summary
    Configures the GPIO pin, RB3, as an input.

  @Description
    Configures the GPIO pin, RB3, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RB3 as an input
    I2_SetDigitalInput();
    </code>

*/
#define I2_SetDigitalInput()   ( TRISBSET = (1 << 3) )
/**
  @Summary
    Configures the GPIO pin, RB3, as an output.

  @Description
    Configures the GPIO pin, RB3, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RB3 as an output
    I2_SetDigitalOutput();
    </code>

*/
#define I2_SetDigitalOutput()   ( TRISBCLR = (1 << 3) )
/**
  @Summary
    Sets the GPIO pin, RB4, high using LATB4.

  @Description
    Sets the GPIO pin, RB4, high using LATB4.

  @Preconditions
    The RB4 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RB4 high (1)
    I3_SetHigh();
    </code>

*/
#define I3_SetHigh()          ( LATBSET = (1 << 4) )
/**
  @Summary
    Sets the GPIO pin, RB4, low using LATB4.

  @Description
    Sets the GPIO pin, RB4, low using LATB4.

  @Preconditions
    The RB4 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RB4 low (0)
    I3_SetLow();
    </code>

*/
#define I3_SetLow()           ( LATBCLR = (1 << 4) )

/**
  @Summary
    Sets a value to the GPIO pin.

  @Description
    Sets or Resets the GPIO pin, RB4, low or high using LATB4.

  @Preconditions
    The RB4 must be set to an output.

  @Returns
    None.

  @Param
    bool value; : value to be set to the GPIO pin.

  @Example
    <code>
    // Set RB4 to low.
    I3_SetValue(false);
    </code>

*/
inline static void I3_SetValue(bool value)
{
  if(value)
  {
    I3_SetHigh();
  }
  else
  {
    I3_SetLow();
  }
}

/**
  @Summary
    Toggles the GPIO pin, RB4, using LATB4.

  @Description
    Toggles the GPIO pin, RB4, using LATB4.

  @Preconditions
    The RB4 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RB4
    I3_Toggle();
    </code>

*/
#define I3_Toggle()           ( LATBINV = (1 << 4) )
/**
  @Summary
    Reads the value of the GPIO pin, RB4.

  @Description
    Reads the value of the GPIO pin, RB4.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RB4
    postValue = I3_GetValue();
    </code>

*/
#define I3_GetValue()         PORTBbits.RB4
/**
  @Summary
    Configures the GPIO pin, RB4, as an input.

  @Description
    Configures the GPIO pin, RB4, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RB4 as an input
    I3_SetDigitalInput();
    </code>

*/
#define I3_SetDigitalInput()   ( TRISBSET = (1 << 4) )
/**
  @Summary
    Configures the GPIO pin, RB4, as an output.

  @Description
    Configures the GPIO pin, RB4, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RB4 as an output
    I3_SetDigitalOutput();
    </code>

*/
#define I3_SetDigitalOutput()   ( TRISBCLR = (1 << 4) )
/**
  @Summary
    Sets the GPIO pin, RB5, high using LATB5.

  @Description
    Sets the GPIO pin, RB5, high using LATB5.

  @Preconditions
    The RB5 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RB5 high (1)
    I4_SetHigh();
    </code>

*/
#define I4_SetHigh()          ( LATBSET = (1 << 5) )
/**
  @Summary
    Sets the GPIO pin, RB5, low using LATB5.

  @Description
    Sets the GPIO pin, RB5, low using LATB5.

  @Preconditions
    The RB5 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RB5 low (0)
    I4_SetLow();
    </code>

*/
#define I4_SetLow()           ( LATBCLR = (1 << 5) )

/**
  @Summary
    Sets a value to the GPIO pin.

  @Description
    Sets or Resets the GPIO pin, RB5, low or high using LATB5.

  @Preconditions
    The RB5 must be set to an output.

  @Returns
    None.

  @Param
    bool value; : value to be set to the GPIO pin.

  @Example
    <code>
    // Set RB5 to low.
    I4_SetValue(false);
    </code>

*/
inline static void I4_SetValue(bool value)
{
  if(value)
  {
    I4_SetHigh();
  }
  else
  {
    I4_SetLow();
  }
}

/**
  @Summary
    Toggles the GPIO pin, RB5, using LATB5.

  @Description
    Toggles the GPIO pin, RB5, using LATB5.

  @Preconditions
    The RB5 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RB5
    I4_Toggle();
    </code>

*/
#define I4_Toggle()           ( LATBINV = (1 << 5) )
/**
  @Summary
    Reads the value of the GPIO pin, RB5.

  @Description
    Reads the value of the GPIO pin, RB5.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RB5
    postValue = I4_GetValue();
    </code>

*/
#define I4_GetValue()         PORTBbits.RB5
/**
  @Summary
    Configures the GPIO pin, RB5, as an input.

  @Description
    Configures the GPIO pin, RB5, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RB5 as an input
    I4_SetDigitalInput();
    </code>

*/
#define I4_SetDigitalInput()   ( TRISBSET = (1 << 5) )
/**
  @Summary
    Configures the GPIO pin, RB5, as an output.

  @Description
    Configures the GPIO pin, RB5, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RB5 as an output
    I4_SetDigitalOutput();
    </code>

*/
#define I4_SetDigitalOutput()   ( TRISBCLR = (1 << 5) )
/**
  @Summary
    Sets the GPIO pin, RB7, high using LATB7.

  @Description
    Sets the GPIO pin, RB7, high using LATB7.

  @Preconditions
    The RB7 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RB7 high (1)
    I5_SetHigh();
    </code>

*/
#define I5_SetHigh()          ( LATBSET = (1 << 7) )
/**
  @Summary
    Sets the GPIO pin, RB7, low using LATB7.

  @Description
    Sets the GPIO pin, RB7, low using LATB7.

  @Preconditions
    The RB7 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RB7 low (0)
    I5_SetLow();
    </code>

*/
#define I5_SetLow()           ( LATBCLR = (1 << 7) )

/**
  @Summary
    Sets a value to the GPIO pin.

  @Description
    Sets or Resets the GPIO pin, RB7, low or high using LATB7.

  @Preconditions
    The RB7 must be set to an output.

  @Returns
    None.

  @Param
    bool value; : value to be set to the GPIO pin.

  @Example
    <code>
    // Set RB7 to low.
    I5_SetValue(false);
    </code>

*/
inline static void I5_SetValue(bool value)
{
  if(value)
  {
    I5_SetHigh();
  }
  else
  {
    I5_SetLow();
  }
}

/**
  @Summary
    Toggles the GPIO pin, RB7, using LATB7.

  @Description
    Toggles the GPIO pin, RB7, using LATB7.

  @Preconditions
    The RB7 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RB7
    I5_Toggle();
    </code>

*/
#define I5_Toggle()           ( LATBINV = (1 << 7) )
/**
  @Summary
    Reads the value of the GPIO pin, RB7.

  @Description
    Reads the value of the GPIO pin, RB7.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RB7
    postValue = I5_GetValue();
    </code>

*/
#define I5_GetValue()         PORTBbits.RB7
/**
  @Summary
    Configures the GPIO pin, RB7, as an input.

  @Description
    Configures the GPIO pin, RB7, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RB7 as an input
    I5_SetDigitalInput();
    </code>

*/
#define I5_SetDigitalInput()   ( TRISBSET = (1 << 7) )
/**
  @Summary
    Configures the GPIO pin, RB7, as an output.

  @Description
    Configures the GPIO pin, RB7, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RB7 as an output
    I5_SetDigitalOutput();
    </code>

*/
#define I5_SetDigitalOutput()   ( TRISBCLR = (1 << 7) )
/**
  @Summary
    Sets the GPIO pin, RB8, high using LATB8.

  @Description
    Sets the GPIO pin, RB8, high using LATB8.

  @Preconditions
    The RB8 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RB8 high (1)
    I6_SetHigh();
    </code>

*/
#define I6_SetHigh()          ( LATBSET = (1 << 8) )
/**
  @Summary
    Sets the GPIO pin, RB8, low using LATB8.

  @Description
    Sets the GPIO pin, RB8, low using LATB8.

  @Preconditions
    The RB8 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RB8 low (0)
    I6_SetLow();
    </code>

*/
#define I6_SetLow()           ( LATBCLR = (1 << 8) )

/**
  @Summary
    Sets a value to the GPIO pin.

  @Description
    Sets or Resets the GPIO pin, RB8, low or high using LATB8.

  @Preconditions
    The RB8 must be set to an output.

  @Returns
    None.

  @Param
    bool value; : value to be set to the GPIO pin.

  @Example
    <code>
    // Set RB8 to low.
    I6_SetValue(false);
    </code>

*/
inline static void I6_SetValue(bool value)
{
  if(value)
  {
    I6_SetHigh();
  }
  else
  {
    I6_SetLow();
  }
}

/**
  @Summary
    Toggles the GPIO pin, RB8, using LATB8.

  @Description
    Toggles the GPIO pin, RB8, using LATB8.

  @Preconditions
    The RB8 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RB8
    I6_Toggle();
    </code>

*/
#define I6_Toggle()           ( LATBINV = (1 << 8) )
/**
  @Summary
    Reads the value of the GPIO pin, RB8.

  @Description
    Reads the value of the GPIO pin, RB8.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RB8
    postValue = I6_GetValue();
    </code>

*/
#define I6_GetValue()         PORTBbits.RB8
/**
  @Summary
    Configures the GPIO pin, RB8, as an input.

  @Description
    Configures the GPIO pin, RB8, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RB8 as an input
    I6_SetDigitalInput();
    </code>

*/
#define I6_SetDigitalInput()   ( TRISBSET = (1 << 8) )
/**
  @Summary
    Configures the GPIO pin, RB8, as an output.

  @Description
    Configures the GPIO pin, RB8, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RB8 as an output
    I6_SetDigitalOutput();
    </code>

*/
#define I6_SetDigitalOutput()   ( TRISBCLR = (1 << 8) )
/**
  @Summary
    Sets the GPIO pin, RB9, high using LATB9.

  @Description
    Sets the GPIO pin, RB9, high using LATB9.

  @Preconditions
    The RB9 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RB9 high (1)
    O4_SetHigh();
    </code>

*/
#define O4_SetHigh()          ( LATBSET = (1 << 9) )
/**
  @Summary
    Sets the GPIO pin, RB9, low using LATB9.

  @Description
    Sets the GPIO pin, RB9, low using LATB9.

  @Preconditions
    The RB9 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RB9 low (0)
    O4_SetLow();
    </code>

*/
#define O4_SetLow()           ( LATBCLR = (1 << 9) )

/**
  @Summary
    Sets a value to the GPIO pin.

  @Description
    Sets or Resets the GPIO pin, RB9, low or high using LATB9.

  @Preconditions
    The RB9 must be set to an output.

  @Returns
    None.

  @Param
    bool value; : value to be set to the GPIO pin.

  @Example
    <code>
    // Set RB9 to low.
    O4_SetValue(false);
    </code>

*/
inline static void O4_SetValue(bool value)
{
  if(value)
  {
    O4_SetHigh();
  }
  else
  {
    O4_SetLow();
  }
}

/**
  @Summary
    Toggles the GPIO pin, RB9, using LATB9.

  @Description
    Toggles the GPIO pin, RB9, using LATB9.

  @Preconditions
    The RB9 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RB9
    O4_Toggle();
    </code>

*/
#define O4_Toggle()           ( LATBINV = (1 << 9) )
/**
  @Summary
    Reads the value of the GPIO pin, RB9.

  @Description
    Reads the value of the GPIO pin, RB9.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RB9
    postValue = O4_GetValue();
    </code>

*/
#define O4_GetValue()         PORTBbits.RB9
/**
  @Summary
    Configures the GPIO pin, RB9, as an input.

  @Description
    Configures the GPIO pin, RB9, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RB9 as an input
    O4_SetDigitalInput();
    </code>

*/
#define O4_SetDigitalInput()   ( TRISBSET = (1 << 9) )
/**
  @Summary
    Configures the GPIO pin, RB9, as an output.

  @Description
    Configures the GPIO pin, RB9, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RB9 as an output
    O4_SetDigitalOutput();
    </code>

*/
#define O4_SetDigitalOutput()   ( TRISBCLR = (1 << 9) )
/**
  @Summary
    Sets the GPIO pin, RC9, high using LATC9.

  @Description
    Sets the GPIO pin, RC9, high using LATC9.

  @Preconditions
    The RC9 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RC9 high (1)
    O3_SetHigh();
    </code>

*/
#define O3_SetHigh()          ( LATCSET = (1 << 9) )
/**
  @Summary
    Sets the GPIO pin, RC9, low using LATC9.

  @Description
    Sets the GPIO pin, RC9, low using LATC9.

  @Preconditions
    The RC9 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Set RC9 low (0)
    O3_SetLow();
    </code>

*/
#define O3_SetLow()           ( LATCCLR = (1 << 9) )

/**
  @Summary
    Sets a value to the GPIO pin.

  @Description
    Sets or Resets the GPIO pin, RC9, low or high using LATC9.

  @Preconditions
    The RC9 must be set to an output.

  @Returns
    None.

  @Param
    bool value; : value to be set to the GPIO pin.

  @Example
    <code>
    // Set RC9 to low.
    O3_SetValue(false);
    </code>

*/
inline static void O3_SetValue(bool value)
{
  if(value)
  {
    O3_SetHigh();
  }
  else
  {
    O3_SetLow();
  }
}

/**
  @Summary
    Toggles the GPIO pin, RC9, using LATC9.

  @Description
    Toggles the GPIO pin, RC9, using LATC9.

  @Preconditions
    The RC9 must be set to an output.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Toggle RC9
    O3_Toggle();
    </code>

*/
#define O3_Toggle()           ( LATCINV = (1 << 9) )
/**
  @Summary
    Reads the value of the GPIO pin, RC9.

  @Description
    Reads the value of the GPIO pin, RC9.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint16_t portValue;

    // Read RC9
    postValue = O3_GetValue();
    </code>

*/
#define O3_GetValue()         PORTCbits.RC9
/**
  @Summary
    Configures the GPIO pin, RC9, as an input.

  @Description
    Configures the GPIO pin, RC9, as an input.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RC9 as an input
    O3_SetDigitalInput();
    </code>

*/
#define O3_SetDigitalInput()   ( TRISCSET = (1 << 9) )
/**
  @Summary
    Configures the GPIO pin, RC9, as an output.

  @Description
    Configures the GPIO pin, RC9, as an output.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    // Sets the RC9 as an output
    O3_SetDigitalOutput();
    </code>

*/
#define O3_SetDigitalOutput()   ( TRISCCLR = (1 << 9) )

/**
    Section: Function Prototypes
*/
/**
  @Summary
    Configures the pin settings of the PIC32MM0256GPM028
    The peripheral pin select, PPS, configuration is also handled by this manager.

  @Description
    This is the generated manager file for the PIC24 / dsPIC33 / PIC32MM MCUs device.  This manager
    configures the pins direction, initial state, analog setting.
    The peripheral pin select, PPS, configuration is also handled by this manager.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    void SYSTEM_Initialize(void)
    {
        // Other initializers are called from this function
        PIN_MANAGER_Initialize();
    }
    </code>

*/
void PIN_MANAGER_Initialize (void);



#endif
