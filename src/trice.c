//! \file trice.c
//! \author Thomas.Hoehenleitner [at] seerose.net
//! ///////////////////////////////////////////////////////////////////////////

#include "trice.h"

///////////////////////////////////////////////////////////////////////////////
// (time) stamp funktions, rewrite these hardware specific functions in your project.
// 

// 16-bit us stamp, wraps after 10 milliseconds
__WEAK uint16_t TriceStamp16( void ){
    return 0x1616;
}

// 32-bit us stamp, wraps after 71,58 seconds
__WEAK uint32_t TriceStamp32( void ){
    return 0x32323232;
}

//
///////////////////////////////////////////////////////////////////////////////

#ifdef TRICE_RTT0
#include "./box/SEGGER_RTT.c"
#endif

#if TRICE_FRAMING==TRICE_FRAMING_COBS
#include "./box/cobsEncode.c"
#endif

#if TRICE_FRAMING==TRICE_FRAMING_TCOBS
#include "./box/tcobsv1Encode.c"
#endif

#if TRICE_MODE==TRICE_DOUBLE_BUFFER
#include "./box/triceDoubleBuffer.c"
#endif

#if defined(TRICE_LOG_OVER_MODBUS_FUNC24_ALSO) || defined(TRICE_LOG_OVER_MODBUS_FUNC24_ONLY)
#include "./box/triceModbusBuffer.c"
#endif

#if TRICE_MODE==TRICE_STACK_BUFFER
#include "./box/triceStackBuffer.c"
#endif

#if TRICE_MODE==TRICE_STREAM_BUFFER
#include "./box/triceStreamBuffer.c"
#endif

#ifdef XTEA_ENCRYPT_KEY
#include "./box/xtea.c"
#endif

#include "./box/core.c"
#include "./box/trice8.c"
#include "./box/trice16.c"
#include "./box/trice32.c"
#include "./box/trice64.c"
