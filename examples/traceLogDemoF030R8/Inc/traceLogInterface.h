/*! \file traceLogInterface.h
\brief hardware specific interface functions
\details tested on STM32F030
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#include "traceLogConfig.h" // TL_INLINE
#include "main.h" // hardware dependencies

/*! Check if a new byte can be written into tracelog transmit register.
\retval false == not empty
\retval true == epmty
User must provide this function.
*/
TL_INLINE uint32_t traceLogTxDataRegisterEmpty( void ){
    return LL_USART_IsActiveFlag_TXE( USART2 );
}

/*! Write value d into tracelog transmit register.
\param d byte to transmit
User must provide this function.
*/
TL_INLINE void traceLogTransmitData8( uint8_t d ){
    LL_USART_TransmitData8( USART2, d);
}

/*! Allow interrupt for empty tracelog data transmit register.
User must provide this function.
*/
TL_INLINE void traceLogEableTxEmptyInterrupt( void ){
    //LL_USART_EnableIT_TXE( USART2 ); // In STM32 done imlizit by writing data
}

/*! Disallow interrupt for empty tracelog data transmit register.
User must provide this function.
*/
TL_INLINE void traceLogDisableTxEmptyInterrupt( void ){
    LL_USART_DisableIT_TXE( USART2 );
}
