//! \file trice.c
//! \author Thomas.Hoehenleitner [at] seerose.net
//! ///////////////////////////////////////////////////////////////////////////

#include "trice.h"

#ifdef TRICE_AUXILIARY
#include "./box/triceAuxiolary.c"
#endif

#ifdef TRICE_RTT0
#include "./box/SEGGER_RTT.c"
#endif

#if TRICE_BUFFER == TRICE_STACK_BUFFER
#include "./box/triceStackBuffer.c"
#endif

#if TRICE_BUFFER == TRICE_STATIC_BUFFER
#include "./box/triceStaticBuffer.c"
#endif

#if TRICE_BUFFER == TRICE_DOUBLE_BUFFER
#include "./box/triceDoubleBuffer.c"
#endif

#if TRICE_BUFFER == TRICE_RING_BUFFER
#include "./box/triceRingBuffer.c"
#endif

#if (TRICE_DIRECT_OUT_FRAMING == TRICE_FRAMING_COBS) || (TRICE_DEFERRED_OUT_FRAMING == TRICE_FRAMING_COBS)
#include "./box/cobsEncode.c"
#endif

#if (TRICE_DIRECT_OUT_FRAMING == TRICE_FRAMING_TCOBS) || (TRICE_DEFERRED_OUT_FRAMING == TRICE_FRAMING_TCOBS)
#include "./box/tcobsv1Encode.c"
#endif

#if defined(TRICE_LOG_OVER_MODBUS_FUNC24_ALSO) || defined(TRICE_LOG_OVER_MODBUS_FUNC24_ONLY)
#include "./box/triceModbusBuffer.c"
#endif

#ifdef XTEA_ENCRYPT_KEY
#include "./box/xtea.c"
#endif

#include "./box/core.c"
#include "./box/trice8.c"
#include "./box/trice16.c"
#include "./box/trice32.c"
#include "./box/trice64.c"
#include "./box/triceDiagnostics.c"
