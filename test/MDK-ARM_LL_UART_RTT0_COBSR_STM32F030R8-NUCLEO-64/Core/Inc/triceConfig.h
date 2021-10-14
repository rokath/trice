/*! \file triceConfig.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
// Enabling next line results in XTEA encryption  with the key.
//#define ENCRYPT XTEA_KEY( ea, bb, ec, 6f, 31, 80, 4e, b9, 68, e2, fa, ea, ae, f1, 50, 54 ); //!< -password MySecret
#ifdef ENCRYPT
// #define DECRYPT //!< usually not needed
void triceServeFifoEncryptedToBytesBuffer(void);
#endif

#define TRICE_STM32
#define TRICE_MODE 101


// #define TRICE_RTT_CHANNEL 0 //!< Uncomment and set channel number for SeggerRTT usage


//  #ifdef TRICE_NO_CODE_GENERATION
//  #define TRICE_ENCODING TRICE_NOCODE_ENCODING //!< Select target trice transfer encoding.
//  #else
//  #define TRICE_ENCODING TRICE_COBSR_ENCODING //!< Select target trice transfer encoding.
//  #endif


//  #define TRICE_U8PUSH(v)  do{ /*triceU8PushSeggerRTT(v);  */ triceU8Push(v); } while(0) //!< Set trice out channel(s) 
//  #define TRICE_U32PUSH(v) do{ /*triceU32PushSeggerRTT(v); */ triceU32Push(v); } while(0) //!< Set trice out channel(s) 

//#define UART_LL_STM32 //!< set UART hardware dependency
//#define TRICE_UART USART2 //!< set UART number if UART is used

void triceCheckSet( int index ); //!< tests





#ifdef __cplusplus
}
#endif

#endif /* TRICE_CONFIG_H_ */

