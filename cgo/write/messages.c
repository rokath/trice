//! \file messages.c
//! \author thomas.hoehenleitner [at] seerose.net

#include "trice.h"
#include "cgoTrice.h"
#include "messages.h"

int Message0( uint8_t b0 ){
    TRICE8_1( Id( 5277), "msg:Example %d\n", b0 );
    return cgoTriceBufferDepth;
}

int Message1( uint16_t stamp16, uint8_t b0 ){
    cgoTriceStamp16 = stamp16;
    TRICE8_1( Id( 7323), "msg:Example %d\n", b0 );
    return cgoTriceBufferDepth;
}

int Message2( uint32_t stamp, uint8_t b0 ){
    cgoTriceStamp32 = stamp;
    TRICE8_1( Id( 3820), "msg:Example %d\n", b0 );
    return cgoTriceBufferDepth;
}