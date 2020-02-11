/*! \file traceLogInterface.h
\brief hardware specific interface functions
\details user must provide these functions
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#include "traceLogConfig.h" // TL_INLINE
#include "main.h" // hardware dependencies

/*! Check if a new byte can be written into tracelog transmit register.
\retval 0 == not empty
\retval !0 == epmty
User must provide this function.
*/
TL_INLINE uint32_t traceLogTxDataRegisterEmpty( void ){
    return 1;
}

/*! Write value d into tracelog transmit register.
\param d byte to transmit
User must provide this function.
*/
TL_INLINE void traceLogTransmitData8( uint8_t d ){
    d = d;
}

/*! Allow interrupt for empty tracelog data transmit register.
User must provide this function.
*/
TL_INLINE void traceLogEableTxEmptyInterrupt( void ){
    
}

/*! Disallow interrupt for empty tracelog data transmit register.
User must provide this function.
*/
TL_INLINE void traceLogDisableTxEmptyInterrupt( void ){
    
}
