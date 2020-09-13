/*! \file bareConfigUART.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef BARE_CONFIG_UART_H_
#define BARE_CONFIG_UART_H_

#include <stdint.h>
#include "main.h" // hardware specific stuff
#include "bareConfigCompiler.h"

#ifdef __cplusplus
extern "C" {
#endif

//! TRICE_FIFO_BYTE_SIZE must be a power of 2, one trice needs 4 to 32 or one day more bytes.
//! The fifo has to hold trice bursts until they are transmitted.
//! It is transmitted with lower priority in the background for example with the UART tx interrupt.
#define TRICE_FIFO_BYTE_SIZE 2048

//! Set according to hardware
#define TRICE_UART USART2 

//! TRICE_BARE_SYNC_LEVEL is the max amount of trices bulk transmitted without a sync trice injected.
//! Assuming triceServeOut() is called every ms this is also the max ms abount without a sync trice.
//! Bigger numbers decrease the bandwidth needs but increase the possible loss of trices in case of
//! re-sync need. A number of 10 is recommended: every 10 ms or 10 trices a sync trice is transmitted
//! increasing the bandwidth need by 10% ans loosing 10 trices in the worst case.
#define TRICE_BARE_SYNC_LEVEL 10

//! Check if a new byte can be written into trice transmit register.
//! \retval 0 == not empty
//! \retval !0 == empty
//! User must provide this function.
TRICE_INLINE uint32_t triceTxDataRegisterEmpty( void ){
    return LL_USART_IsActiveFlag_TXE( TRICE_UART );
}

//! Write value v into trice transmit register.
//! \param v byte to transmit
//! User must provide this function.
TRICE_INLINE void triceTransmitData8( uint8_t v ){
    LL_USART_TransmitData8( TRICE_UART, v);
}

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

#ifdef __cplusplus
}
#endif

#endif /* BARE_CONFIG_UART_H_ */
