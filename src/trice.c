//! \file trice.c
//! \author Thomas.Hoehenleitner [at] seerose.net
//! ///////////////////////////////////////////////////////////////////////////

#include "trice.h"

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
