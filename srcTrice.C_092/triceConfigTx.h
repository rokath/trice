/*! \file triceConfigTx.h
\brief This file is for trice specific project settings. It is included automatically.
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_CONFIG_TX_H_
#define TRICE_CONFIG_TX_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "main.h" // hardware specific stuff

extern uint32_t ms; 

#if TRICE_VARIANT == STM32_LL

///////////////////////////////////////////////////////////////////////////////
// hardware specific interface functions tested on NUCLEO-STM32F030
//

//! Check if a new byte can be written into trice transmit register.
//! \retval 0 == not empty
//! \retval !0 == empty
//! User must provide this function.
TRICE_INLINE uint32_t triceTxDataRegisterEmpty( void ){
    return LL_USART_IsActiveFlag_TXE( TRICE_UART );
}

//! Write value d into trice transmit register.
//! \param d byte to transmit
//! User must provide this function.
TRICE_INLINE void triceTransmitData8( uint8_t d ){
    LL_USART_TransmitData8( TRICE_UART, d);
}


#endif // #if TRICE_VARIANT == STM32_LL

#ifdef __cplusplus
}
#endif

#endif /* TRICE_CONFIG_TX_H_ */
