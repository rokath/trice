/*! \file triceConfig.h
\author Thomas.Hoehenleitner [at] seerose.net
\details This file serves as example just for CONFIGURATION == 0. All other configurations are only alternatives.
*******************************************************************************/

#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

//! TRICE_CLEAN, if found inside triceConfig.h, is modified by the Trice tool to silent editor warnings in the cleaned state.
#define TRICE_CLEAN 1 // Do not define this at an other place! But you can delete this here.

#include <stdint.h>

extern volatile uint32_t* const DWT_CONTROL;
extern volatile uint32_t* const DWT_CYCCNT;
extern volatile uint32_t* const DEMCR;
extern volatile uint32_t* const LAR; // lock access register

#define TriceStamp16 ((uint16_t)(*DWT_CYCCNT)) // @64MHz wraps after a bit more than 1ms (MCU clocks)
#define TriceStamp32            (*DWT_CYCCNT)  // @64MHz -> 1 µs, wraps after 2^32 µs ~= 1.2 hours

void TriceHeadLine(char * name);
void LogTriceConfiguration(void);
void SomeExampleTrices(int burstCount);

#define TRICE_FULL_CHECK // enable all code inside triceCheck.c

#ifndef CONFIGURATION
#define CONFIGURATION 0 //!< The build script can define CONFIGURATION to select.
#endif

#define TRICE_DEFERRED_UARTA 1
#define TRICE_UARTA USART2

#define USE_SEGGER_RTT_LOCK_UNLOCK_MACROS 0
#define TRICE_ENTER_CRITICAL_SECTION { // SEGGER_RTT_LOCK() { 
#define TRICE_LEAVE_CRITICAL_SECTION } // SEGGER_RTT_UNLOCK() } 

#if CONFIGURATION == 0 //////////////////////////////////////////////////////////////
#define LogConfigInfo() trice8("dbg:CONFIGURATION == %d - An example configuration\n", CONFIGURATION );
#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1
#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_DEFERRED_BUFFER_SIZE 4096
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 0
#define TRICE_CYCLE_COUNTER 0
#define TRICE_FULL_CHECK // enable all code inside triceCheck.c

#elif CONFIGURATION == 1 //////////////////////////////////////////////////////////////
#define LogConfigInfo() trice8("dbg:CONFIGURATION == %d - An example configuration\n", CONFIGURATION );
#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_CONFIG_WARNINGS 0

#elif CONFIGURATION == 2 //////////////////////////////////////////////////////////////
#define LogConfigInfo() trice8("dbg:CONFIGURATION == %d - An example configuration\n", CONFIGURATION );
#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_CONFIG_WARNINGS 0

#elif CONFIGURATION == 3 //////////////////////////////////////////////////////////////
#define LogConfigInfo() trice8("dbg:CONFIGURATION == %d - An example configuration\n", CONFIGURATION );
#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_CONFIG_WARNINGS 0

#elif CONFIGURATION == 4 //////////////////////////////////////////////////////////////
#define LogConfigInfo() trice8("dbg:CONFIGURATION == %d - An example configuration\n", CONFIGURATION );
#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_CONFIG_WARNINGS 0

#elif CONFIGURATION == 5 //////////////////////////////////////////////////////////////
#define LogConfigInfo() trice8("dbg:CONFIGURATION == %d - An example configuration\n", CONFIGURATION );
#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_CONFIG_WARNINGS 0

#elif CONFIGURATION == 6 //////////////////////////////////////////////////////////////
#define LogConfigInfo() trice8("dbg:CONFIGURATION == %d - An example configuration\n", CONFIGURATION );
#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_CONFIG_WARNINGS 0

#elif CONFIGURATION == 7 //////////////////////////////////////////////////////////////
#define LogConfigInfo() trice8("dbg:CONFIGURATION == %d - An example configuration\n", CONFIGURATION );
#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_CONFIG_WARNINGS 0

#elif CONFIGURATION == 8 //////////////////////////////////////////////////////////////
#define LogConfigInfo() trice8("dbg:CONFIGURATION == %d - An example configuration\n", CONFIGURATION );
#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_CONFIG_WARNINGS 0

#elif CONFIGURATION == 8 + 1 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 8 + 2 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 8 + 3 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 8 + 4 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 8 + 5 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 8 + 6 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 8 + 7 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 8 + 8 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 16 + 1 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE

#elif CONFIGURATION == 16 + 2 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE

#elif CONFIGURATION == 16 + 3 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE

#elif CONFIGURATION == 16 + 4 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE

#elif CONFIGURATION == 16 + 5 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE

#elif CONFIGURATION == 16 + 6 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE

#elif CONFIGURATION == 16 + 7 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE

#elif CONFIGURATION == 16 + 8 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE

#elif CONFIGURATION == 24 + 1 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 24 + 2 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 24 + 3 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 24 + 4 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 24 + 5 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 24 + 6 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 24 + 7 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 24 + 8 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 32 + 1 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_CONFIG_WARNINGS 0

#elif CONFIGURATION == 32 + 2 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_CONFIG_WARNINGS 0

#elif CONFIGURATION == 32 + 3 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_CONFIG_WARNINGS 0

#elif CONFIGURATION == 32 + 4 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_CONFIG_WARNINGS 0

#elif CONFIGURATION == 32 + 5 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_CONFIG_WARNINGS 0

#elif CONFIGURATION == 32 + 6 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_CONFIG_WARNINGS 0

#elif CONFIGURATION == 32 + 7 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_CONFIG_WARNINGS 0

#elif CONFIGURATION == 32 + 8 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_CONFIG_WARNINGS 0

#elif CONFIGURATION == 32 + 8 + 1 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 32 + 8 + 2 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 32 + 8 + 3 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 32 + 8 + 4 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 32 + 8 + 5 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 32 + 8 + 6 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 32 + 8 + 7 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 32 + 8 + 8 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 32 + 16 + 1 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE

#elif CONFIGURATION == 32 + 16 + 2 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE

#elif CONFIGURATION == 32 + 16 + 3 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE

#elif CONFIGURATION == 32 + 16 + 4 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE

#elif CONFIGURATION == 32 + 16 + 5 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE

#elif CONFIGURATION == 32 + 16 + 6 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE

#elif CONFIGURATION == 32 + 16 + 7 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE

#elif CONFIGURATION == 32 + 16 + 8 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE

#elif CONFIGURATION == 32 + 24 + 1 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 32 + 24 + 2 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 32 + 24 + 3 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 32 + 24 + 4 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 32 + 24 + 5 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 32 + 24 + 6 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 32 + 24 + 7 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_SINGLE_PACK_MODE
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 32 + 24 + 8 //////////////////////////////////////////////////////////////

#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_RING_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_TRANSFER_MODE TRICE_MULTI_PACK_MODE
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 64 + 1 //////////////////////////////////////////////////////////////

#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1
#define TRICE_BUFFER TRICE_STACK_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_CONFIG_WARNINGS 0

#elif CONFIGURATION == 64 + 2 //////////////////////////////////////////////////////////////

#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1
#define TRICE_BUFFER TRICE_STATIC_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_CONFIG_WARNINGS 0

#elif CONFIGURATION == 64 + 3 //////////////////////////////////////////////////////////////

#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1
#define TRICE_BUFFER TRICE_STACK_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_CONFIG_WARNINGS 0

#elif CONFIGURATION == 64 + 4 //////////////////////////////////////////////////////////////

#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1
#define TRICE_BUFFER TRICE_STATIC_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_CONFIG_WARNINGS 0

#elif CONFIGURATION == 64 + 5 //////////////////////////////////////////////////////////////

#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1
#define TRICE_BUFFER TRICE_STACK_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_CONFIG_WARNINGS 0

#elif CONFIGURATION == 64 + 6 //////////////////////////////////////////////////////////////

#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1
#define TRICE_BUFFER TRICE_STATIC_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_CONFIG_WARNINGS 0

#elif CONFIGURATION == 64 + 7 //////////////////////////////////////////////////////////////

#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1
#define TRICE_BUFFER TRICE_STACK_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_CONFIG_WARNINGS 0

#elif CONFIGURATION == 64 + 8 //////////////////////////////////////////////////////////////

#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1
#define TRICE_BUFFER TRICE_STATIC_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_CONFIG_WARNINGS 0

#elif CONFIGURATION == 64 + 8 + 1 //////////////////////////////////////////////////////////////

#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1
#define TRICE_BUFFER TRICE_STACK_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 64 + 8 + 2 //////////////////////////////////////////////////////////////

#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1
#define TRICE_BUFFER TRICE_STATIC_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 64 + 8 + 3 //////////////////////////////////////////////////////////////

#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1
#define TRICE_BUFFER TRICE_STACK_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 64 + 8 + 4 //////////////////////////////////////////////////////////////

#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1
#define TRICE_BUFFER TRICE_STATIC_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 64 + 8 + 5 //////////////////////////////////////////////////////////////

#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1
#define TRICE_BUFFER TRICE_STACK_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 64 + 8 + 6 //////////////////////////////////////////////////////////////

#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1
#define TRICE_BUFFER TRICE_STATIC_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 64 + 8 + 7 //////////////////////////////////////////////////////////////

#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1
#define TRICE_BUFFER TRICE_STACK_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 64 + 8 + 8 //////////////////////////////////////////////////////////////

#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1
#define TRICE_BUFFER TRICE_STATIC_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_XTEA_ENCRYPT 1
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 64 + 16 + 1 //////////////////////////////////////////////////////////////

#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1
#define TRICE_BUFFER TRICE_STACK_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 0

#elif CONFIGURATION == 64 + 16 + 2 //////////////////////////////////////////////////////////////

#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1
#define TRICE_BUFFER TRICE_STATIC_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 0

#elif CONFIGURATION == 64 + 16 + 3 //////////////////////////////////////////////////////////////

#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1
#define TRICE_BUFFER TRICE_STACK_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 0

#elif CONFIGURATION == 64 + 16 + 4 //////////////////////////////////////////////////////////////

#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1
#define TRICE_BUFFER TRICE_STATIC_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 0

#elif CONFIGURATION == 64 + 16 + 5 //////////////////////////////////////////////////////////////

#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1
#define TRICE_BUFFER TRICE_STACK_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 1

#elif CONFIGURATION == 64 + 16 + 6 //////////////////////////////////////////////////////////////

#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1
#define TRICE_BUFFER TRICE_STATIC_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 1

#elif CONFIGURATION == 64 + 16 + 7 //////////////////////////////////////////////////////////////

#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1
#define TRICE_BUFFER TRICE_STACK_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 1

#elif CONFIGURATION == 64 + 16 + 8 //////////////////////////////////////////////////////////////

#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1
#define TRICE_BUFFER TRICE_STATIC_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 1

#elif CONFIGURATION == 64 + 24 + 1 //////////////////////////////////////////////////////////////

#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1
#define TRICE_BUFFER TRICE_STACK_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 64 + 24 + 2 //////////////////////////////////////////////////////////////

#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1
#define TRICE_BUFFER TRICE_STATIC_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 64 + 24 + 3 //////////////////////////////////////////////////////////////

#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1
#define TRICE_BUFFER TRICE_STACK_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 64 + 24 + 4 //////////////////////////////////////////////////////////////

#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1
#define TRICE_BUFFER TRICE_STATIC_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 0
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 64 + 24 + 5 //////////////////////////////////////////////////////////////

#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1
#define TRICE_BUFFER TRICE_STACK_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 64 + 24 + 6 //////////////////////////////////////////////////////////////

#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1
#define TRICE_BUFFER TRICE_STATIC_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 64 + 24 + 7 //////////////////////////////////////////////////////////////

#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1
#define TRICE_BUFFER TRICE_STACK_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 64 + 24 + 8 //////////////////////////////////////////////////////////////

#define TRICE_DIRECT_OUTPUT 1
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1
#define TRICE_BUFFER TRICE_STATIC_BUFFER
#define TRICE_PROTECT 1
#define TRICE_DIAGNOSTICS 1
#define TRICE_DEFERRED_OUT_FRAMING TRICE_FRAMING_COBS

#elif CONFIGURATION == 97 /////////////////////////////////////////////////////////////
#define LogConfigInfo() trice8("dbg:CONFIGURATION == %d - An example configuration with direct RTT output only and optimized for speed\n", CONFIGURATION);
#define TRICE_DIRECT_OUTPUT 1
#define TRICE_BUFFER TRICE_STACK_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 0

// trice l -p JLINK -args="-Device STM32L432KC -if SWD -Speed 4000 -RTTChannel 0" -pf none -d16
#define TRICE_DIRECT_SEGGER_RTT_32BIT_WRITE 1

#elif CONFIGURATION == 98 /////////////////////////////////////////////////////
#define LogConfigInfo() trice8("dbg:CONFIGURATION == %d - UART, no cycle counter, no critical sections.\n", CONFIGURATION);
#define TRICE_DEFERRED_OUTPUT 1
#define TRICE_BUFFER TRICE_DOUBLE_BUFFER
#define TRICE_PROTECT 0
#define TRICE_DIAGNOSTICS 0
#define TRICE_CYCLE_COUNTER 0

#define TRICE_DEFERRED_BUFFER_SIZE 4096
#define TRICE_SINGLE_MAX_SIZE 512

#undef USE_SEGGER_RTT_LOCK_UNLOCK_MACROS
#undef TRICE_ENTER_CRITICAL_SECTION
#define TRICE_ENTER_CRITICAL_SECTION {
#undef TRICE_LEAVE_CRITICAL_SECTION
#define TRICE_LEAVE_CRITICAL_SECTION }

#elif CONFIGURATION == 99 /////////////////////////////////////////////////////
#define LogConfigInfo() trice8("dbg:CONFIGURATION == %d - An minimum configuration example.\n", CONFIGURATION);
#define TRICE_DEFERRED_OUTPUT 1
//#undef TriceStamp16
//#undef TriceStamp32
//#define TriceStamp16 uwTick  // 1ms, wraps after 2^16 ms ~= a bit more than 1min 
//#define TriceStamp32 uwTick  // 1ms, wraps after 2^32 ms ~= 41 days 
#define TRICE_BUFFER TRICE_RING_BUFFER

#elif CONFIGURATION == 100 /////////////////////////////////////////////////////
#define LogConfigInfo() trice8("dbg:CONFIGURATION == %d - An example configuration with direct RTT output and parallel deferred UART output.\n", CONFIGURATION);
#define TRICE_DEFERRED_OUTPUT 1
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

#ifndef LogConfigInfo
#define LogConfigInfo() trice8("dbg:CONFIGURATION == %d - An example configuration\n", CONFIGURATION);
#endif

#ifdef __cplusplus
}
#endif

#endif /* TRICE_CONFIG_H_ */
