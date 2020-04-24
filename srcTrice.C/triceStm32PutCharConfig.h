/*! \file triceStm32PutcharConfig.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_STM32_PUTCHAR_CONFIG_H_
#define TRICE_STM32_PUTCHAR_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "triceStm32CompilerConfig.h"
#include "main.h" // hardware specific stuff

///////////////////////////////////////////////////////////////////////////////
// hardware specific interface functions tested on NUCLEO-STM32F030
//

#define TRICE_QUICK_AND_DIRTY_ONLY_PUTCHAR // for a quick start you can enable this line and rely only putchar


TRICE_INLINE void tricePutchar( char c ){
    LL_USART_TransmitData8( TRICE_UART, c); // put your putchar call here
}

#define TRICE_ENTER_CRITICAL_SECTION {
#define TRICE_LEAVE_CRITICAL_SECTION }

TRICE_INLINE uint32_t triceTxDataRegisterEmpty( void ){ return 1; } // would be good to implement it for better performance, otherwise you need to use a time interval for the Tx calls
TRICE_INLINE void triceTransmitData8( uint8_t d ){ tricePutchar( d ); }
TRICE_INLINE void triceEableTxEmptyInterrupt( void ){}
TRICE_INLINE void triceDisableTxEmptyInterrupt( void ){}

#ifdef __cplusplus
}
#endif

#endif /* TRICE_STM32_PUTCHAR_CONFIG_H_ */
