/**
  EUSART Generated Driver API Header File

  @Company
    Microchip Technology Inc.

  @File Name
    eusart.h

  @Summary
    This is the generated header file for the EUSART driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for EUSART.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.0
        Device            :  PIC18F45K20
        Driver Version    :  2.1.0
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.10 and above
        MPLAB 	          :  MPLAB X 5.35
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#ifndef EUSART_H
#define EUSART_H

/**
  Section: Included Files
*/

#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif


/**
  Section: Macro Declarations
*/

#define EUSART_DataReady  (EUSART_is_rx_ready())

/**
  Section: Data Type Definitions
*/

typedef union {
    struct {
        unsigned perr : 1;
        unsigned ferr : 1;
        unsigned oerr : 1;
        unsigned reserved : 5;
    };
    uint8_t status;
}eusart_status_t;


/**
  Section: EUSART APIs
*/

/**
  @Summary
    Initialization routine that takes inputs from the EUSART GUI.

  @Description
    This routine initializes the EUSART driver.
    This routine must be called before any other EUSART routine is called.

  @Preconditions
    None

  @Param
    None

  @Returns
    None

  @Comment
    
*/
void EUSART_Initialize(void);

/**
  @Summary
    Checks if the EUSART transmitter is ready to transmit data

  @Description
    This routine checks if EUSART transmitter is ready 
    to accept and transmit data byte

  @Preconditions
    EUSART_Initialize() function should have been called
    before calling this function.
    EUSART transmitter should be enabled before calling 
    this function

  @Param
    None

  @Returns
    Status of EUSART transmitter
    TRUE: EUSART transmitter is ready
    FALSE: EUSART transmitter is not ready
    
  @Example
    <code>
    void main(void)
    {
        volatile uint8_t rxData;
        
        // Initialize the device
        SYSTEM_Initialize();
        
        while(1)
        {
            // Logic to echo received data
            if(EUSART_is_rx_ready())
            {
                rxData = UART1_Read();
                if(EUSART_is_tx_ready())
                {
                    EUSARTWrite(rxData);
                }
            }
        }
    }
    </code>
*/
bool EUSART_is_tx_ready(void);

/**
  @Summary
    Checks if the EUSART receiver ready for reading

  @Description
    This routine checks if EUSART receiver has received data 
    and ready to be read

  @Preconditions
    EUSART_Initialize() function should be called
    before calling this function
    EUSART receiver should be enabled before calling this 
    function

  @Param
    None

  @Returns
    Status of EUSART receiver
    TRUE: EUSART receiver is ready for reading
    FALSE: EUSART receiver is not ready for reading
    
  @Example
    <code>
    void main(void)
    {
        volatile uint8_t rxData;
        
        // Initialize the device
        SYSTEM_Initialize();
        
        while(1)
        {
            // Logic to echo received data
            if(EUSART_is_rx_ready())
            {
                rxData = UART1_Read();
                if(EUSART_is_tx_ready())
                {
                    EUSART_Write(rxData);
                }
            }
        }
    }
    </code>
*/
bool EUSART_is_rx_ready(void);

/**
  @Summary
    Checks if EUSART data is transmitted

  @Description
    This function return the status of transmit shift register

  @Preconditions
    EUSART_Initialize() function should be called
    before calling this function
    EUSART transmitter should be enabled and EUSART_Write
    should be called before calling this function

  @Param
    None

  @Returns
    Status of EUSART receiver
    TRUE: Data completely shifted out if the USART shift register
    FALSE: Data is not completely shifted out of the shift register
    
  @Example
    <code>
    void main(void)
    {
        volatile uint8_t rxData;
        
        // Initialize the device
        SYSTEM_Initialize();
        
        while(1)
        {
            if(EUSART_is_tx_ready())
            {
				LED_0_SetHigh();
                EUSARTWrite(rxData);
            }
			if(EUSART_is_tx_done()
            {
                LED_0_SetLow();
            }
        }
    }
    </code>
*/
bool EUSART_is_tx_done(void);

/**
  @Summary
    Gets the error status of the last read byte.

  @Description
    This routine gets the error status of the last read byte.

  @Preconditions
    EUSART_Initialize() function should have been called
    before calling this function. The returned value is only
    updated after a read is called.

  @Param
    None

  @Returns
    the status of the last read byte

  @Example
	<code>
    void main(void)
    {
        volatile uint8_t rxData;
        volatile eusart_status_t rxStatus;
        
        // Initialize the device
        SYSTEM_Initialize();
        
        // Enable the Global Interrupts
        INTERRUPT_GlobalInterruptEnable();
        
        while(1)
        {
            // Logic to echo received data
            if(EUSART_is_rx_ready())
            {
                rxData = EUSART_Read();
                rxStatus = EUSART_get_last_status();
                if(rxStatus.ferr){
                    LED_0_SetHigh();
                }
            }
        }
    }
    </code>
 */
eusart_status_t EUSART_get_last_status(void);

/**
  @Summary
    Read a byte of data from the EUSART.

  @Description
    This routine reads a byte of data from the EUSART.

  @Preconditions
    EUSART_Initialize() function should have been called
    before calling this function. The transfer status should be checked to see
    if the receiver is not empty before calling this function.

  @Param
    None

  @Returns
    A data byte received by the driver.
*/
uint8_t EUSART_Read(void);

 /**
  @Summary
    Writes a byte of data to the EUSART.

  @Description
    This routine writes a byte of data to the EUSART.

  @Preconditions
    EUSART_Initialize() function should have been called
    before calling this function. The transfer status should be checked to see
    if transmitter is not busy before calling this function.

  @Param
    txData  - Data byte to write to the EUSART

  @Returns
    None
*/
void EUSART_Write(uint8_t txData);



/**
  @Summary
    Set EUSART Framing Error Handler

  @Description
    This API sets the function to be called upon EUSART framing error

  @Preconditions
    Initialize  the EUSART before calling this API

  @Param
    Address of function to be set as framing error handler

  @Returns
    None
*/
void EUSART_SetFramingErrorHandler(void (* interruptHandler)(void));

/**
  @Summary
    Set EUSART Overrun Error Handler

  @Description
    This API sets the function to be called upon EUSART overrun error

  @Preconditions
    Initialize  the EUSART module before calling this API

  @Param
    Address of function to be set as overrun error handler

  @Returns
    None
*/
void EUSART_SetOverrunErrorHandler(void (* interruptHandler)(void));

/**
  @Summary
    Set EUSART Error Handler

  @Description
    This API sets the function to be called upon EUSART error

  @Preconditions
    Initialize  the EUSART module before calling this API

  @Param
    Address of function to be set as error handler

  @Returns
    None
*/
void EUSART_SetErrorHandler(void (* interruptHandler)(void));



#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif

#endif  // EUSART_H
/**
 End of File
*/
