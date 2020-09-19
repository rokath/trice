/*! \file triceConfig.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

//! Set to 
    // TO_TRICE_WRITE_ESC
    // MORE_FLASH_AND_SPEED
    // LESS_FLASH_AND_SPEED 
    // or NO_CODE
#define TRICE_CODE MORE_FLASH_AND_SPEED

#if TO_TRICE_WRITE_ESC == TRICE_CODE

void tricePushByteUART(uint8_t v);
void tricePushByteSeggerRTT(uint8_t v);

//! Set trice out channel(s) 
#define TRICE_PUSH_BYTE(v) do{ tricePushByteSeggerRTT(v); tricePushByteUART(v); } while(0)

#else // #if TO_TRICE_WRITE_ESC == TRICE_CODE

void tricePushSeggerRTT( uint32_t );
void tricePushFifoUART( uint32_t );

//! Set trice out channel(s) 
#define TRICE_PUSH(v) do{ tricePushSeggerRTT(v); tricePushFifoUART(v); } while(0)

#endif // #else // #if TO_TRICE_WRITE_ESC == TRICE_CODE

// See triceConfigCompiler.h for compiler adaption 

//!< tests
void triceCheckSet( int index ); 

#ifdef __cplusplus
}
#endif

#endif /* TRICE_CONFIG_H_ */
