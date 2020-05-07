/*! \file triceConfigTxInterrupt.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_CONFIG_TX_INTERRUPT_H_
#define TRICE_CONFIG_TX_INTERRUPT_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "main.h" // hardware specific stuff

#if TRICE_VARIANT == STM32_LL

//! Allow interrupt for empty trice data transmit register.
//! User must provide this function.
TRICE_INLINE void triceEnableTxEmptyInterrupt( void ){
    LL_USART_EnableIT_TXE( TRICE_UART );
}

//! Disallow interrupt for empty trice data transmit register.
//! User must provide this function.
TRICE_INLINE void triceDisableTxEmptyInterrupt( void ){
    LL_USART_DisableIT_TXE( TRICE_UART );
}

#endif // #if TRICE_VARIANT == STM32_LL

#ifdef __cplusplus
}
#endif

#endif /* TRICE_CONFIG_TX_INTERRUPT_H_ */
