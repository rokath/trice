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
#define TRICE_FILENAME TRICE0( Id(56535), "stm32f0xx_it.c" ); // macro __FILE__ does not work here, because of pre-compile evaluation
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

#define WSIZ 64 //!< must be power of 2!
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


#include "triceUtilities.h"



/*
ok:
2020/03/09 17:30:54     incoming: [109 115 103 58 48 49 50 51 52 53 54 55 56 57]                 0 nanoseconds
2020/03/09 17:30:54     incoming: [235 96 96 164 57 118 0 0]             0 nanoseconds
                                    ^---------------dieses Byte fehlt
--------------------------------------------------------------------------------------------------------------------------------------------------
2020/03/09 17:30:54   incoming: [235 96 96 204 31 33 25 0]                0 nanoseconds

2020/03/09 17:30:54     incoming: [235 96 96 83 92 228 0 0]              0 nanoseconds
0123456789--------------------------------------------------------------------------------------------------------------------------------------------------
2020/03/09 17:30:54         incoming: [235 96 96 231 209 221 192 192]           0 nanoseconds

BUG:


2020/03/09 17:30:55     incoming: [109 115 103 58 48 49 50 51 52 53 54 55 56 57]                 999400 nanoseconds
2020/03/09 17:30:55     incoming: [96 96 164 57 118 0 0 235]             1000600 nanoseconds
2020/03/09 17:30:55     incoming: [96]           0 nanoseconds
2020/03/09 17:30:55     incoming: [96]           0 nanoseconds
2020/03/09 17:30:55     incoming: [207]                  0 nanoseconds
2020/03/09 17:30:55     incoming: [31]           0 nanoseconds
2020/03/09 17:30:55     incoming: [33]           0 nanoseconds
2020/03/09 17:30:55     incoming: [26]           0 nanoseconds
2020/03/09 17:30:55     incoming: [0]            0 nanoseconds
                                  <235 96 96 207 31 33 26 0>
2020/03/09 17:30:55     incoming: [235 96 96 231 209 221 192 192]                0 nanoseconds
01234567892020/03/09 17:30:55   incoming: [235 96 96 231 209 221 96 96]                  0 nanoseconds
#-40(-64) 2020/03/09 17:30:55   incoming: [235 96 96 83 92 228 0 0]              0 nanoseconds
#60(96) --------------------------------------------------------------------------------------------------------------------------------------------------2020/03/09 17:30:55   incoming: [235 96 96 231 209 221 96 96]             0 nanoseconds

2020/03/09 17:30:55     incoming: [235 96 96 231 209 221 219 219]                0 nanoseconds


*/





/*! Write value d into trice transmit register.
\param d byte to transmit
User must provide this function.
*/
static void Dbg_triceTransmitData8( uint8_t d ){
    // TRICE8_2( Id(56785), "att:#%02x(%d) ", d, d );
    //FifoPushUint8_Protected( &rdFifo, d );
    
    LL_USART_TransmitData8( USART2, d);
}



static uint8_t txNextByte( void ){
    uint8_t value;
    FifoPopUint8_Unprotected( &wrFifo, &value );
    Dbg_triceTransmitData8( value );
    //triceEableTxEmptyInterrupt(); // probably not needed
    return value;
}

//! reCalTxHandler checks txe flag and wrFifo for depth and starts a byte transmission if possible
//! otherwise it disables the transmit empty interrupt
//! \param pTxState, set none when current packet done, set to reCalTx when reCal packet transfer active
//! \todo review packets to determine packet end
static void reCalTxStart( int* pTxState ){
    if( triceTxDataRegisterEmpty() ){ 
        size_t count = FifoReadableCount_Unprotected( &wrFifo );
        if( count >= 8 ){ // start only if a full header already in wrFifo.
            // This is safe, because following data will be in fifo before the first 8 bytes are transmitted
            txNextByte();
            *pTxState = reCalTx;
            triceEableTxEmptyInterrupt(); 
            //return 1;
        //}else {
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
static void reCalTxContinue( int* pTxState ){
        static int pkgByteIdx = 0; // so some interpretation is needed
        static uint32_t len = 0;   // 1...65536 must fit
        static uint8_t lenL = 0;
    //ASSERT_OR_RETURN( FifoReadableCount_Unprotected( &wrFifo ) );
    size_t x = FifoReadableCount_Unprotected( &wrFifo );
    if( 0 == x ){
        TRICE16_2( Id(20598), "ERR:unexpected state!pkgByteIdx=%d, len=%d\n", pkgByteIdx, len );
        pkgByteIdx = 0; // reload for next package
        triceDisableTxEmptyInterrupt();
        *pTxState = noTx; // BUG
        return;
    }
    if( 1 == x ){
        TRICE0( Id(26118), "wrn:last byte!" );
    }
    if( triceTxDataRegisterEmpty() ){ // TX Data Register Empty 
        static uint8_t dpc = 0;    // we must transfer a complete block without trices inbetween
        pkgByteIdx++; // first byte was already transmitted in reCalTxStart
        if( pkgByteIdx < 7 ){ // 0xc0 cad sad cr8 tid fid pix dpc
            txNextByte();     //   0   1   2   3   4   5   6   7
            return;
        }
        if ( 7 == pkgByteIdx ){ // dpc
            dpc = txNextByte(); // tx last byte of tx header 
            if( 0 == dpc ){ 
                pkgByteIdx = 0; // reload for next package
                triceDisableTxEmptyInterrupt();
                *pTxState = noTx; // reCal packet done
            }
            //len = 0;
            return;
        }

        // dpc > 0, so read len first time
        if ( 8 == pkgByteIdx ){
            lenL = txNextByte();
            return;
        }
        if ( 9 == pkgByteIdx ){
            len = txNextByte() << 8;
            len |= lenL;
            len++; // len is len-1 value
            return;
        }

        // "loop" starts here
        if ( pkgByteIdx < len + 9 ){ // len=1...65536
            txNextByte(); // tx data
            return;
        } else if ( pkgByteIdx == len + 9 ){ // len=1...65536
            txNextByte(); // tx last date in data package
            dpc--;
            if( 0 == dpc ){ 
                pkgByteIdx = 0;
                triceDisableTxEmptyInterrupt();
                *pTxState = noTx; // reCal packet done
            }
            return;
        } 
        
        // this is reached when dpc was > 1, read len of following data package
        if ( len + 10 == pkgByteIdx ){ // 10 = header + sizeof(uint16_t)
            lenL = txNextByte();
            return;
        } 
        if ( len + 11 == pkgByteIdx ){
            int len_1 = len; // keep old value
            len = txNextByte() << 8;
            len |= lenL;
            len ++; // len is len-1 value
            len = len_1 + sizeof(uint16_t) + len; // accumulate
            return;
        }

        //} else {
        //    FifoPopUint8_InsideTxIsr( &wrFifo, &value );
        //    triceTransmitData8( value );
        //    triceDisableTxEmptyInterrupt();
        //    pkgByteIdx = 0;
        //    *pTxState = noTx;
        //}
        //}else{
        //    TRICE0( "ERR:Unexpected execution path 
        //if( 0 == count ){
        //    
        //    pkgByteIdx = 0;
        //    
        //}
    }
}

//static size_t reCalCount

//! Check for data and start a transmission, if both channes have data give priority to reCal
//! \param PtrTxState do nothing if != noTx
//! should be activated cyclically for example every 1 ms for small transmit delays
void UartTxStartCheck( int* PtrTxState ){
    if( noTx == *PtrTxState ){
        reCalTxStart( PtrTxState ); // reCal transmission has priority over trices - iportand for 2 reasons: 
        } // 1. runtims strings are earlier befor their trigger trices. 2. reCal is more time critical than trice transmission
    if( noTx == *PtrTxState ){
        triceTxStart( PtrTxState );
    }
}

//! continue ongoing transmission, otherwise check for new data transmission
//! \param PtrTxState actual transmission state
//! should be activated cyclically for example every 1 ms for small transmit delays
void UartTxContinueCheck( int* PtrTxState ){
    if( reCalTx == *PtrTxState ){
        reCalTxContinue( PtrTxState );
    } else if( triceTx == *PtrTxState ){
        triceTxContinue(PtrTxState);
    } else { // none == tsState
        //UartTxStartCheck(PtrTxState); // start immediately next transmission if a transmission finished
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
    UartTxContinueCheck(&txState);
  /* USER CODE END USART2_IRQn 0 */
  /* USER CODE BEGIN USART2_IRQn 1 */
  /* USER CODE END USART2_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
