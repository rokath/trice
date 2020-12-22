/*! \file triceConfig.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "trice.h"

///////////////////////////////////////////////////////////////////////////////
//
// #define TRICE_ENTER_CRITICAL_SECTION { // Uncomment for more speed but only if TRICE macros cannot
// #define TRICE_LEAVE_CRITICAL_SECTION } // get interrupted by other TRICE macros (e.g. interrupts).
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// select target trice method
#define TRICE_ENCODING TRICE_FAST_BARE_SYNC_ENCODING

#define TRICE_U8PUSH(v) do{ triceU8PushSeggerRTT(v); triceU8Push(v); } while(0) //!< Set trice out channel(s) 
#define TRICE_U32PUSH(v) do{ triceU32PushSeggerRTT(v); triceU32Push(v); } while(0) //!< Set trice out channel(s) 
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// uncomment for trice wrap transfer format
// #define TRICE_WRAP_START_BYTE 0xEB // 235
// #define TRICE_WRAP_LOCAL_ADDR 0x60 // to do: also trice tool parameter
// #define TRICE_WRAP_DEST_ADDR  0x60 // to do: also trice tool parameter
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// uncomment for trice UART transfer format
#define TRICE_UART USART2 // set UART number
///////////////////////////////////////////////////////////////////////////////

void triceCheckSet( int index ); //!< tests

#ifdef __cplusplus
}
#endif

#endif /* TRICE_CONFIG_H_ */
