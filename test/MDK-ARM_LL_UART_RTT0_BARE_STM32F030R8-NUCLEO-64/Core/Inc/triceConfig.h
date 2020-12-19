/*! \file triceConfig.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

///////////////////////////////////////////////////////////////////////////////
//
// #define TRICE_ENTER_CRITICAL_SECTION { // Uncomment for more speed but only if TRICE macros cannot
// #define TRICE_LEAVE_CRITICAL_SECTION } // get interrupted by other TRICE macros (e.g. interrupts).
//
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// select target trice method
//#define TRICE_NO_CODE //!< no trice code generation
#define TRICE_WRITE_BARE_FIFO
//#define TRICE_WRITE_ESC_FIFO
//#define TRICE_WRITE_BARE_FIFO_WRITE_ESC_FIFO
//
///////////////////////////////////////////////////////////////////////////////



#define TRICE_UART USART2


///////////////////////////////////////////////////////////////////////////////
// adapt 
#ifdef TRICE_WRITE_ESC_FIFO

void tricePushByteEscFifo(uint8_t v);
void tricePushByteSeggerRTT(uint8_t v);

//! Set trice out channel(s) 
#define TRICE_PUSH_BYTE(v) do{ tricePushByteSeggerRTT(v); tricePushByteEscFifo(v); } while(0)

#endif // #ifdef TRICE_WRITE_ESC_FIFO
//
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// adapt
#ifdef TRICE_WRITE_BARE_FIFO

//! used as TRICE_CODE macro option for more flash occupation, but decreases execution time and needs smaller buffers
#define MORE_FLASH_AND_SPEED

//! used as TRICE_CODE macro option for less flash occupation, but increases execution time and needs bigger buffers
//#define LESS_FLASH_AND_SPEED

void tricePushSeggerRTT( uint32_t );
void tricePushBareFifo( uint32_t );

//! Set trice out channel(s) 
#define TRICE_PUSH(v) do{ tricePushSeggerRTT(v); tricePushBareFifo(v); } while(0)

#endif // #ifdef TRICE_WRITE_BARE_FIFO
//
///////////////////////////////////////////////////////////////////////////////




// See triceConfigCompiler.h for compiler adaption 

//!< tests
void triceCheckSet( int index ); 

#ifdef __cplusplus
}
#endif

#endif /* TRICE_CONFIG_H_ */
