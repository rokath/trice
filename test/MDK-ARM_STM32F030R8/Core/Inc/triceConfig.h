/*! \file triceConfig.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define TRICE_MODE 0

//! Direct output to UART with cycle counter. Trices inside interrupts allowed. Direct TRICE macro execution.
//! Command line similar to: "trice log -p COM1 -baud 115200"
#if TRICE_MODE == 0
#define TRICE_HEADLINE \
    TRICE0( Id( 57449), "s:                                          \n" ); \
    TRICE8( Id( 49675), "s:  TRICE_MODE %3d                          \ns:     ", TRICE_MODE ); \
    TRICE0( Id( 64478), "att:  Direct to UART, +cycle, +int  " ); \
    TRICE0( Id( 46377), "s:     \ns:                                          \n");
#define TRICE_STM32
#define TRICE_UART USART2 //!< set UART number if UART is used
#endif

//! Direct output to SEGGER RTT with cycle counter. Trices inside interrupts allowed. Direct TRICE macro execution.
//! Needs additional tools installed - see RTT documentation.
//! J-LINK Command line similar to: `trice log -args="-Device STM32G071RB -if SWD -Speed 4000 -RTTChannel 0 -RTTSearchRanges 0x20000000_0x1000"`
//! ST-LINK Command line similar to: `trice log -p ST-LINK -args="-Device STM32G071RB -if SWD -Speed 4000 -RTTChannel 0 -RTTSearchRanges 0x20000000_0x1000"`
#if TRICE_MODE == 10
#include "SEGGER_RTT.h"
#define RTT_WRITE( buf, len ) do{ SEGGER_RTT_Write(0 /*BufferIndex*/, buf, len ); }while(0)
#define TRICE_HEADLINE \
    TRICE0( Id( 57449), "s:                                          \n" ); \
    TRICE8( Id( 63820), "s:  TRICE_MODE %3d                          \n", TRICE_MODE ); \
    TRICE0( Id( 46700), "s:                                          \ns:     " ); \
    TRICE0( Id( 43538), "att:  Direct to RTT, +cycle, +int   " ); \
    TRICE0( Id( 46377), "s:     \ns:                                          \n");
#endif

//! Double Buffering output to UART with cycle counter. Trices inside interrupts allowed. Fast TRICE macro execution. 
//! Command line similar to: "trice log -p COM1 -baud 115200"
#if TRICE_MODE == 100
#define TRICE_HEADLINE \
    TRICE0( Id( 57449), "s:                                          \n" ); \
    TRICE8( Id( 37906), "s:  TRICE_MODE %3d                          \n", TRICE_MODE ); \
    TRICE0( Id( 46700), "s:                                          \ns:     " ); \
    TRICE0( Id( 57445), "att: Double buff UART, +cycle, +int " ); \
    TRICE0( Id( 46377), "s:     \ns:                                          \n");
#define TRICE_STM32
#define TRICE_UART USART2 //!< set UART number if UART is used
#endif

//! Double Buffering output to UART without cycle counter. No trices inside interrupts allowed. Super fast TRICE macro execution. 
//! Command line similar to: `trice log -p COM1 -baud 115200 -cc=false`
#if TRICE_MODE == 101
#define TRICE_HEADLINE \
    TRICE0( Id( 57449), "s:                                          \n" ); \
    TRICE8( Id( 51851), "s:  TRICE_MODE %3d                          \n", TRICE_MODE ); \
    TRICE0( Id( 46700), "s:                                          \ns:     " ); \
    TRICE0( Id( 60240), "att: Double buff UART, ~cycle, ~int " ); \
    TRICE0( Id( 46377), "s:     \ns:                                          \n");
#define TRICE_STM32
#define TRICE_UART USART2 //!< set UART number if UART is used
#endif

//! Double Buffering output to RTT with cycle counter. Trices inside interrupts allowed. Fast TRICE macro execution. 
//! Command line similar to: `trice l -args="-Device STM32F030R8 -if SWD -Speed 4000 -RTTChannel 0 -RTTSearchRanges 0x20000000_0x1000"`
#if TRICE_MODE == 110
#include "SEGGER_RTT.h"
#define RTT_WRITE( buf, len ) do{ SEGGER_RTT_Write(0 /*BufferIndex*/, buf, len ); }while(0)
//#define RTT_WRITE( buf, len )do{ unsigned x; do{ x=SEGGER_RTT_WriteSkipNoLock(0 /*BufferIndex*/, buf, len ); }while(x==0); }while(0)
#define TRICE_HEADLINE \
    TRICE0( Id( 57449), "s:                                          \n" ); \
    TRICE8( Id( 61754), "s:  TRICE_MODE %3d                          \n", TRICE_MODE ); \
    TRICE0( Id( 46700), "s:                                          \ns:     " ); \
    TRICE0( Id( 55144), "att: Double buff RTT, +cycle, +int  " ); \
    TRICE0( Id( 46377), "s:     \ns:                                          \n");
#endif

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



// Enabling next line results in XTEA encryption  with the key.
//#define ENCRYPT XTEA_KEY( ea, bb, ec, 6f, 31, 80, 4e, b9, 68, e2, fa, ea, ae, f1, 50, 54 ); //!< -password MySecret


#ifdef __cplusplus
}
#endif

#endif /* TRICE_CONFIG_H_ */

