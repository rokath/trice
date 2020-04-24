/*! \file triceStm32WriteConfig.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_STM32_WRITE_CONFIG_H_
#define TRICE_STM32_WRITE_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "triceStm32CompilerConfig.h"
#include "main.h" // hardware specific stuff

///////////////////////////////////////////////////////////////////////////////
// hardware specific interface functions tested on NUCLEO-STM32F030
//

#define TRICE_USE_WRITE_FUNCTION // for a quick start you can enable this line and rely only putchar


TRICE_INLINE void triceWrite( char* c, int count ){
	if( count && triceTxDataRegisterEmpty() ){
		LL_USART_TransmitData8( TRICE_UART, c); // only one char 
	}
	return count - 1;
}

#define TRICE_ENTER_CRITICAL_SECTION {
#define TRICE_LEAVE_CRITICAL_SECTION }


//! Check if a new byte can be written into trice transmit register.
//! \retval 0 == not empty
//! \retval !0 == empty
//! User must provide this function.
TRICE_INLINE uint32_t triceTxDataRegisterEmpty( void ){
    return LL_USART_IsActiveFlag_TXE( TRICE_UART );
}


#ifdef __cplusplus
}
#endif

#endif /* TRICE_STM32_WRITE_CONFIG_H_ */
