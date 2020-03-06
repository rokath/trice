/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f0xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f0xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "trice.h"
#include "Fifo.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
 
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#if 1
///////////////////////////////////////////////////////////////////////////////
// fifo instances
//

#define WSIZ 128 //!< must be power of 2!
static uint8_t rcWr[ WSIZ ]; 
#define RSIZ 512 //!< must be power of 2!
static uint8_t rcRd[ RSIZ ];

//! fifo control struct, UART transmit date are provided here
Fifo_t wrFifo = {0, rcWr, rcWr+WSIZ, rcWr, rcWr }; 

//! fifo control struct, UART received data are arriving here
Fifo_t rdFifo = {0, rcRd, rcRd+RSIZ, rcRd, rcRd }; 

///////////////////////////////////////////////////////////////////////////////
// tx 
//

int txState = noTx;

//! reCalTxHandler checks txe flag and wrFifo for depth and starts a byte transmission if possible
//! otherwise it disables the transmit empty interrupt
//! \param pTxState, set none when current packet done, set to reCalTx when reCal packet transfer active
//! \todo review packets to determine packet end
void reCalTxStart( int* pTxState ){
    if( triceTxDataRegisterEmpty() ){ // TX Data Register Empty 
        uint8_t value;
        size_t count = FifoReadableCount( &wrFifo );
        if( count >=8 ){ // start only if a full header already in wrFifo.
            // This is safe, because following data will be in fifo before the first 8 bytes are transmitted
            *pTxState = reCalTx;
            FifoPopUint8_InsideTxIsr( &wrFifo, &value );
            triceTransmitData8( value );
            triceEableTxEmptyInterrupt(); 
            //return 1;
        }else {
            //triceDisableTxEmptyInterrupt();
            //*pTxState = noTx;
            //return 0;
        }
    }
}


//! reCalTxHandler checks txe flag and wrFifo for depth and starts a byte transmission if possible
//! otherwise it disables the transmit empty interrupt
//! \param pTxState, set none when current packet done, set to reCalTx when reCal packet transfer active
//! \todo review packets to determine packet end
void reCalTxContinue( int* pTxState ){
    static int packageByteCounter = 0;
    if( triceTxDataRegisterEmpty() ){ // TX Data Register Empty 
        uint8_t value;
        size_t count = FifoReadableCount( &wrFifo );
        static uint8_t sLenL = 0;
        static uint16_t sLen = 0;
        packageByteCounter++; // first byte was already transmitted in reCalTxStart
        if( count ){
            if( packageByteCounter < 7 ){ //
                //*pTxState = reCalTx;
                FifoPopUint8_InsideTxIsr( &wrFifo, &value );
                triceTransmitData8( value );
                //triceEableTxEmptyInterrupt();
            } else if ( 7 == packageByteCounter ){
                FifoPopUint8_InsideTxIsr( &wrFifo, &value );
                triceTransmitData8( value );
                if( 1 != value ){
                    TRICE8_1( Id(57290), "dpc = %02x and not 1\n!", value );
                }
            } else if ( 8 == packageByteCounter ){
                FifoPopUint8_InsideTxIsr( &wrFifo, &value );
                triceTransmitData8( value );
                sLenL = value;
            } else if ( 9 == packageByteCounter ){
                FifoPopUint8_InsideTxIsr( &wrFifo, &value );
                triceTransmitData8( value );
                sLen = value;
                sLen = (sLen <<8) | sLenL;
            } else if ( packageByteCounter < sLen + 10 ){
                FifoPopUint8_InsideTxIsr( &wrFifo, &value );
                triceTransmitData8( value );
            } else {
                FifoPopUint8_InsideTxIsr( &wrFifo, &value );
                triceTransmitData8( value );
                triceDisableTxEmptyInterrupt();
                packageByteCounter = 0;
                *pTxState = noTx;
            }
        }
        if( 0 == count ){
            triceDisableTxEmptyInterrupt();
            packageByteCounter = 0;
            *pTxState = noTx;
        }
    }
}

//static size_t reCalCount

//! Check for data and start a transmission, if both channes have data give priority to reCal
//! \param PtrTxState do nothing if != noTx
void UartTxStartCheck( int* PtrTxState ){
    if( noTx == *PtrTxState ){
        reCalTxStart( PtrTxState );
    }
    if( noTx == *PtrTxState ){
        triceTxStart( PtrTxState );
    }
}

//! continue ongoing transmission, otherwise check for new data transmission
//! \param PtrTxState actual transmission state
void UartTxCheck( int* PtrTxState ){
    if( reCalTx == *PtrTxState ){
        reCalTxContinue( PtrTxState );
    } else if( triceTx == *PtrTxState ){
        triceTxContinue(PtrTxState);
    } else { // none == tsState
        UartTxStartCheck(PtrTxState);
    }
}
#endif
/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M0 Processor Interruption and Exception Handlers          */ 
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVC_IRQn 0 */

  /* USER CODE END SVC_IRQn 0 */
  /* USER CODE BEGIN SVC_IRQn 1 */

  /* USER CODE END SVC_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */
    static uint16_t msTick = 0;
    static uint32_t ms = 0; 
    msTick++;
    if( 1000 == msTick ){
        msTick = 0;
        ms +=1000;
        //TRICE32_1( Id(18577), "ISR:alive time %d milliseconds\n", ms );
    }

  /* USER CODE END SysTick_IRQn 0 */
  
  /* USER CODE BEGIN SysTick_IRQn 1 */
    //triceTxHandler(&txState); // start transmission if data
    UartTxStartCheck(&txState);
  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f0xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles USART2 global interrupt.
  */
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */
    //triceTxHandler(&txState);
    UartTxCheck(&txState);
  /* USER CODE END USART2_IRQn 0 */
  /* USER CODE BEGIN USART2_IRQn 1 */
  /* USER CODE END USART2_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
