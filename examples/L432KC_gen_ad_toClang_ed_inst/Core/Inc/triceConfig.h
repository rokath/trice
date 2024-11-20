/*! \file triceConfig.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#define TRICE_INLINE static inline

//! TRICE_CLEAN, if found inside triceConfig.h, is modified by the Trice tool to silent editor warnings in the cleaned state.
#define TRICE_CLEAN 1 // Do not define this at an other place! But you can delete this here.

#include <stdint.h>

extern volatile uint32_t * const DWT_CONTROL;
extern volatile uint32_t * const DWT_CYCCNT ;
extern volatile uint32_t * const DEMCR;
extern volatile uint32_t * const LAR; // lock access register

void TriceHeadLine(char* name);
void LogTriceConfiguration( void );
void SomeExampleTrices(int burstCount);

#ifndef CONFIGURATION
//! The build script can define CONFIGURATION
#define CONFIGURATION 0 
#endif

// #define TRICE_CHECK_MIN // TRICE_CHECK_MIN, if defined, reduces the compiled content of this file to a minimum. 

#define TriceStamp16 (*DWT_CYCCNT)      // @64MHz wraps after a bit more than 1ms (MCU clocks) 
#define TriceStamp32 ((*DWT_CYCCNT)>>6) // @64MHz -> 1 µs, wraps after 2^32 µs ~= 1.2 hours

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_DEFERRED_UARTA 1
#define TRICE_UARTA USART2

#define USE_SEGGER_RTT_LOCK_UNLOCK_MACROS 1
#define TRICE_ENTER_CRITICAL_SECTION { SEGGER_RTT_LOCK() { 
#define TRICE_LEAVE_CRITICAL_SECTION } SEGGER_RTT_UNLOCK() } 

#if CONFIGURATION == 0 //////////////////////////////////////////////////////////////
#define LogConfigInfo() trice8("dbg:CONFIGURATION == %d - An example configuration\n", CONFIGURATION );
#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 1
#define TRICE_CYCLE_COUNTER 1
#define TRICE_DEFERRED_OUTPUT 1
//#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE
#define TRICE_DEFERRED_BUFFER_SIZE 4096

//#define TRICE_CHECK_MIN

#elif CONFIGURATION == 1 //////////////////////////////////////////////////////////////
#define LogConfigInfo() trice8("dbg:CONFIGURATION == %d - An example configuration\n", CONFIGURATION );

#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_CONFIG_WARNINGS 0

#elif CONFIGURATION == 2 //////////////////////////////////////////////////////////////
#define LogConfigInfo() trice8("dbg:CONFIGURATION == %d - An example configuration\n", CONFIGURATION );

#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_CONFIG_WARNINGS 0

#elif CONFIGURATION == 3 //////////////////////////////////////////////////////////////
#define LogConfigInfo() trice8("dbg:CONFIGURATION == %d - An example configuration\n", CONFIGURATION );

#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_CONFIG_WARNINGS 0

#elif CONFIGURATION == 4 //////////////////////////////////////////////////////////////
#define LogConfigInfo() trice8("dbg:CONFIGURATION == %d - An example configuration\n", CONFIGURATION );

#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_CONFIG_WARNINGS 0

#elif CONFIGURATION == 5 //////////////////////////////////////////////////////////////
#define LogConfigInfo() trice8("dbg:CONFIGURATION == %d - An example configuration\n", CONFIGURATION );

#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_CONFIG_WARNINGS 0

#elif CONFIGURATION == 6 //////////////////////////////////////////////////////////////
#define LogConfigInfo() trice8("dbg:CONFIGURATION == %d - An example configuration\n", CONFIGURATION );

#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_CONFIG_WARNINGS 0

#elif CONFIGURATION == 7 //////////////////////////////////////////////////////////////
#define LogConfigInfo() trice8("dbg:CONFIGURATION == %d - An example configuration\n", CONFIGURATION );

#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_CONFIG_WARNINGS 0

#elif CONFIGURATION == 8 //////////////////////////////////////////////////////////////
#define LogConfigInfo() trice8("dbg:CONFIGURATION == %d - An example configuration\n", CONFIGURATION );

#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_CONFIG_WARNINGS 0

#elif CONFIGURATION == 8+1 //////////////////////////////////////////////////////////////
#define LogConfigInfo() trice8("dbg:CONFIGURATION == %d - An example configuration\n", CONFIGURATION );

#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 8+2 //////////////////////////////////////////////////////////////
#define LogConfigInfo() trice8("dbg:CONFIGURATION == %d - An example configuration\n", CONFIGURATION );

#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 8+3 //////////////////////////////////////////////////////////////
#define LogConfigInfo() trice8("dbg:CONFIGURATION == %d - An example configuration\n", CONFIGURATION );

#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 8+4 //////////////////////////////////////////////////////////////
#define LogConfigInfo() trice8("dbg:CONFIGURATION == %d - An example configuration\n", CONFIGURATION );

#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 8+5 //////////////////////////////////////////////////////////////
#define LogConfigInfo() trice8("dbg:CONFIGURATION == %d - An example configuration\n", CONFIGURATION );

#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 8+6 //////////////////////////////////////////////////////////////
#define LogConfigInfo() trice8("dbg:CONFIGURATION == %d - An example configuration\n", CONFIGURATION );

#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 8+7 //////////////////////////////////////////////////////////////
#define LogConfigInfo() trice8("dbg:CONFIGURATION == %d - An example configuration\n", CONFIGURATION );

#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 8+8 //////////////////////////////////////////////////////////////
#define LogConfigInfo() trice8("dbg:CONFIGURATION == %d - An example configuration\n", CONFIGURATION );

#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 16+1 //////////////////////////////////////////////////////////////
#define LogConfigInfo() trice8("dbg:CONFIGURATION == %d - An example configuration\n", CONFIGURATION );

#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE

#elif CONFIGURATION == 16+2 //////////////////////////////////////////////////////////////
#define LogConfigInfo() trice8("dbg:CONFIGURATION == %d - An example configuration\n", CONFIGURATION );

#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE

#elif CONFIGURATION == 16+3 //////////////////////////////////////////////////////////////
#define LogConfigInfo() trice8("dbg:CONFIGURATION == %d - An example configuration\n", CONFIGURATION );

#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE

#elif CONFIGURATION == 16+4 //////////////////////////////////////////////////////////////
#define LogConfigInfo() trice8("dbg:CONFIGURATION == %d - An example configuration\n", CONFIGURATION );

#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE

#elif CONFIGURATION == 16+5 //////////////////////////////////////////////////////////////
#define LogConfigInfo() trice8("dbg:CONFIGURATION == %d - An example configuration\n", CONFIGURATION );

#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE

#elif CONFIGURATION == 16+6 //////////////////////////////////////////////////////////////
#define LogConfigInfo() trice8("dbg:CONFIGURATION == %d - An example configuration\n", CONFIGURATION );

#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE

#elif CONFIGURATION == 16+7 //////////////////////////////////////////////////////////////
#define LogConfigInfo() trice8("dbg:CONFIGURATION == %d - An example configuration\n", CONFIGURATION );

#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE

#elif CONFIGURATION == 16+8 //////////////////////////////////////////////////////////////
#define LogConfigInfo() trice8("dbg:CONFIGURATION == %d - An example configuration\n", CONFIGURATION );

#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE


#elif CONFIGURATION == 24+1 //////////////////////////////////////////////////////////////
#define LogConfigInfo() trice8("dbg:CONFIGURATION == %d - An example configuration\n", CONFIGURATION );

#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 24+2 //////////////////////////////////////////////////////////////
#define LogConfigInfo() trice8("dbg:CONFIGURATION == %d - An example configuration\n", CONFIGURATION );

#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 24+3 //////////////////////////////////////////////////////////////
#define LogConfigInfo() trice8("dbg:CONFIGURATION == %d - An example configuration\n", CONFIGURATION );

#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 24+4 //////////////////////////////////////////////////////////////
#define LogConfigInfo() trice8("dbg:CONFIGURATION == %d - An example configuration\n", CONFIGURATION );

#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 24+5 //////////////////////////////////////////////////////////////
#define LogConfigInfo() trice8("dbg:CONFIGURATION == %d - An example configuration\n", CONFIGURATION );

#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 24+6 //////////////////////////////////////////////////////////////
#define LogConfigInfo() trice8("dbg:CONFIGURATION == %d - An example configuration\n", CONFIGURATION );

#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 24+7 //////////////////////////////////////////////////////////////
#define LogConfigInfo() trice8("dbg:CONFIGURATION == %d - An example configuration\n", CONFIGURATION );

#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 24+8 //////////////////////////////////////////////////////////////
#define LogConfigInfo() trice8("dbg:CONFIGURATION == %d - An example configuration\n", CONFIGURATION );

#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS













#elif CONFIGURATION == 33 /////////////////////////////////////////////////////////////
#define CONFIG_TEXT "An example configuration with direct RTT output only and optimized for speed"
// trice l -p JLINK -args="-Device STM32L432KC -if SWD -Speed 4000 -RTTChannel 0" -pf none  -d16 -ts32 us -ts16 "sig:clock: %5d"
#undef TRICE_DEFERRED_OUTPUT
#define TRICE_DEFERRED_OUTPUT 0
#define TRICE_BUFFER TRICE_STACK_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 0

// trice l -p JLINK -args="-Device STM32L432KC -if SWD -Speed 4000 -RTTChannel 0" -pf none -d16 -ts32 µs
#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1

#elif CONFIGURATION == 34 /////////////////////////////////////////////////////
#define CONFIG_TEXT "An example configuration with deferred UART output using the double buffer and\n\
without protection, diagnostics, critical sections to get max log speed."

#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 0
#define TRICE_CYCLE_COUNTER 0

#undef USE_SEGGER_RTT_LOCK_UNLOCK_MACROS
#undef TRICE_ENTER_CRITICAL_SECTION
#define TRICE_ENTER_CRITICAL_SECTION {
#undef TRICE_LEAVE_CRITICAL_SECTION
#define TRICE_LEAVE_CRITICAL_SECTION }

#elif CONFIGURATION == 35 /////////////////////////////////////////////////////
#define CONFIG_TEXT "A minimum configuration example."
//#undef TriceStamp16
//#undef TriceStamp32
//#define TriceStamp16 uwTick  // 1ms, wraps after 2^16 ms ~= a bit more than 1min 
//#define TriceStamp32 uwTick  // 1ms, wraps after 2^32 ms ~= 41 days 
#define TRICE_BUFFER TRICE_RING_BUFFER

#elif CONFIGURATION == 36 /////////////////////////////////////////////////////
#define CONFIG_TEXT "An example configuration with direct RTT output and parallel deferred UART output."

#define TRICE_BUFFER TRICE_RING_BUFFER

// trice l -p JLINK -args="-Device STM32L432KC -if SWD -Speed 4000 -RTTChannel 0" -pf none -d16 -ts32 µs
#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1

// trice log -p com7 -pw MySecret -pf COBS
#define TRICE_DEFERRED_XTEA_ENCRYPT 1 // See XTEA_ENCRYPT_KEY in triceDefault.Config.h for details.
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS
/////////////////////////////////////////////////////////////////////////////////////
#else
#error unknown CONFIGURATION value
#endif

#ifdef __cplusplus
}
#endif

#endif /* TRICE_CONFIG_H_ */
