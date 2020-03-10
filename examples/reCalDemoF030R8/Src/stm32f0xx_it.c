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

#define WSIZ 1024 //!< must be power of 2!
static uint8_t rcWr[ WSIZ ]; 
#define RSIZ 4 //!< must be power of 2!
static uint8_t rcRd[ RSIZ ];


/*

typedef struct{
    size_t size; //!< fifo data size
    uint8_t* pBuff; //!< start address
    uint8_t* pLimit; //!< first address behind fifo data
    uint8_t* pWr; //!< internal write pointer
    uint8_t* pRd; //!< internal read pointer
} Fifo_t; //!< first in first out struct type

*/


//! fifo control struct, UART transmit date are provided here
Fifo_t wrFifo = {WSIZ, rcWr, rcWr+WSIZ, rcWr, rcWr }; 

//! fifo control struct, UART received data are arriving here
Fifo_t rdFifo = {RSIZ, rcRd, rcRd+RSIZ, rcRd, rcRd }; 

///////////////////////////////////////////////////////////////////////////////
// tx 
//

int txState = noTx;


#include "triceUtilities.h"


    static int pkgByteIdx = -1; // some interpretation is needed

static inline void resetPkgByteIndex( void ){
    pkgByteIdx = -1; 
}

static void Debug_validateReCal( uint8_t d ){
    //static uint8_t lo = 0;
    switch(pkgByteIdx){
        case 0: if( 0xc0 != d )       { TRICE8_2( Id(35279), "err: 0.d=0x%02x, pkgByteIdx=%d\n", d, pkgByteIdx );  } break;
        case 1: if( 0x60 != d )       { TRICE8_2( Id(24932), "err: 1.d=0x%02x, pkgByteIdx=%d\n", d, pkgByteIdx );  } break;
        case 2: if( 0x60 != d )       { TRICE8_2( Id(23748), "err: 2.d=0x%02x, pkgByteIdx=%d\n", d, pkgByteIdx );  } break;
        
        case 6: if( 0xc0  & d )       { TRICE8_2( Id(33873), "err: 6.d=0x%02x, pkgByteIdx=%d\n", d, pkgByteIdx );  } break;
        case 7: if( 0x01 != d )       { TRICE8_2( Id(25447), "err: 7.d=0x%02x, pkgByteIdx=%d\n", d, pkgByteIdx );  } break;
        //case 8: if(!(d<=11<d&&d <=13)){ TRICE8_2( Id(17145), "err: 8.d=0x%02x, pkgByteIdx=%d\n", d, pkgByteIdx );  } break;
        case 9: if( 0x00 != d )       { TRICE8_2( Id( 2517), "err: 9.d=0x%02x, pkgByteIdx=%d\n", d, pkgByteIdx );  } break;

        default: 
            if( 23 <= pkgByteIdx) {
                //TRICE8_3( Id(61436), "dbg: d=%d=0x%02x, pkgByteIdx=%d\n", d, d, pkgByteIdx );
            }
            if( 25 < pkgByteIdx ){
                TRICE8_1( Id( 6848), "ERR:OR! pkgByteIdx=%d\n", pkgByteIdx );
                //resetPkgByteIndex();
            }
        break;
    }
}


static uint8_t txNextByte( void ){
    uint8_t v;
    FifoPopUint8_Unprotected( &wrFifo, &v );
    Debug_validateReCal(v);
    triceTransmitData8( v );
    return v;
}

//! reCalTxHandler starts a reCal transmission if possible, otherwise it does nothing
//! It checks these conditions:
//! \li currently no transmission: TxState is noTx
//! \li tx register is empty 
//! \li wrFifo contains at least one reCal Header
//! In that case the TxState changes from noTx to reCalTx and the first byte goes out
//! and the transmit empty interrupt is enabled.
//! Otherwise reCalTxStart does nothing.
//! \param pTxState pointer to TxState
static void reCalTxStart( int* pTxState ){
    if( noTx == *pTxState && triceTxDataRegisterEmpty() && 8 <= FifoReadableCount_Unprotected( &wrFifo ) ){ 
        // start only if a full header already in wrFifo.
        // This is safe, because following data will be in fifo before the first 8 bytes are transmitted
        //txNextByte();
        *pTxState = reCalTx;
        triceEableTxEmptyInterrupt(); 
    }
}


    static uint32_t len = 0;   // 0, 1...65536 must fit
    static uint8_t lenL = 0;
    static uint8_t dpc = 0;    // we must transfer a complete block without trices inbetween



//! reCalTxHandler checks if the tx register is empty and that the TxState is reCalTx flag and does nothing otherwise

//! otherwise it disables the transmit empty interrupt
//! \param pTxState, set none when current packet done, set to reCalTx when reCal packet transfer active
//! \todo review packets to determine packet end
static void reCalTxContinue( int* pTxState ){

    if( reCalTx != *pTxState || !triceTxDataRegisterEmpty() ){
        return;
    }

    if( 0 == FifoReadableCount_Unprotected( &wrFifo ) ){
        TRICE16_2( Id(26366), "ERR:Bug! Unexpected state!pkgByteIdx=%d, len=%d\n", pkgByteIdx, len );
       resetPkgByteIndex(); // reload for next package
        triceDisableTxEmptyInterrupt();
        *pTxState = noTx; // 
        return;
    }
    
    pkgByteIdx++; // reset value is -1, so starts here with 0
    
    if( pkgByteIdx < 7 ){ // 0xc0 cad sad cr8 tid fid pix dpc
        txNextByte();     //   0   1   2   3   4   5   6   7
        return;
    }
    if ( 7 == pkgByteIdx ){ // dpc
        dpc = txNextByte(); // tx last byte of tx header 
        if( 1 != dpc ){
            TRICE8_1( Id( 1041), "err: dpc=%d\n", dpc );
           resetPkgByteIndex();
            return;
        }
        if( 0 == dpc ){ 
           resetPkgByteIndex(); // reload for next package
            triceDisableTxEmptyInterrupt();
            *pTxState = noTx; // reCal packet done
        }
        return;
    }

    // dpc is > 0, so read len first time
    if ( 8 == pkgByteIdx ){
        lenL = txNextByte();
        return;
    }
    if ( 9 == pkgByteIdx ){
        len = txNextByte() << 8; // little endian
        len |= lenL;
        len++; // len is len-1 value
        //TRICE16_2( Id(63041), "sig:lenL=%d, len=%d\n", lenL, len );
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
            resetPkgByteIndex();
            triceDisableTxEmptyInterrupt();
            *pTxState = noTx; // reCal packet done
        }
        return;
    } 
    
    TRICE0( Id(33480), "WRN:this is reached when dpc was bigger 1, read len of following data package\n" );
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
        if( !(12 <= len && len <= 14) ){
            TRICE16_2( Id(29983), "err:len = %d, pkgByteIdx = %d\n", len, pkgByteIdx );
        }
        return;
    }
}

//! Check for data and start a transmission, if both channes have data give priority to reCal
//! \param PtrTxState do nothing if != noTx
//! should be activated cyclically for example every 1 ms for small transmit delays
void UartTxStartCheck( int* PtrTxState ){
    reCalTxStart( PtrTxState ); // reCal transmission has priority over trices - iportand for 2 reasons: 
    // 1. runtims strings are earlier before their trigger trices. 
    // 2. reCal is more time critical than trice transmission
    triceTxStart( PtrTxState );
}

//! continue ongoing transmission, otherwise check for new data transmission
//! \param PtrTxState actual transmission state
//! should be activated cyclically for example every 1 ms for small transmit delays
void UartTxContinueCheck( int* PtrTxState ){
    reCalTxContinue( PtrTxState );
    triceTxContinue( PtrTxState );
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
