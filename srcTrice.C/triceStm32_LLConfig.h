/*! \file triceStm32_LLConfig.h
\brief This file is for trice specific project settings
\details adapt needed fifo size, compiler settings and 4 hardware access functions
- triceTxDataRegisterEmpty()
- triceTransmitData8()
- triceEnableTxEmptyInterrupt()
- triceDisableTxEmptyInterrupt()
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_STM32_LL_CONFIG_H_
#define TRICE_STM32_LL_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "main.h" // hardware specific stuff

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

#ifdef NO_INTERRUPTS

//! Allow interrupt for empty trice data transmit register.
//! User must provide this function.
TRICE_INLINE void triceEnableTxEmptyInterrupt( void ){
}

//! Disallow interrupt for empty trice data transmit register.
//! User must provide this function.
TRICE_INLINE void triceDisableTxEmptyInterrupt( void ){
}

#endif // #ifdef NO_INTERRUPTS

#ifdef WITH_INTERRUPTS

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

//! Save interrupt state and disable Interrupts
//! \details Workaround for ARM Cortex M0 and M0+
//! \li __get_PRIMASK() is 0 when interrupts are enabled globally
//! \li __get_PRIMASK() is 1 when interrupts are disabled globally
//! If trices are used only outside critical sections or interrupts
//! you can leave this macro pair empty for more speed.
#define TRICE_ENTER_CRITICAL_SECTION { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); {

//! Restore interrupt state
//! \details Workaround for ARM Cortex M0 and M0+
//! \li __get_PRIMASK() is 0 when interrupts are enabled globally
//! \li __get_PRIMASK() is 1 when interrupts are disabled globally
//! If trices are used only outside critical sections or interrupts
//! you can leave this macro pair empty for more speed.
#define TRICE_LEAVE_CRITICAL_SECTION } __set_PRIMASK(primaskstate); }

#endif // #ifdef USE_INTERRUPTS

#ifdef __cplusplus
}
#endif

#endif /* TRICE_STM32_LL_CONFIG_H_ */
