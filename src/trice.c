//! \file trice.c
//! \author Thomas.Hoehenleitner [at] seerose.net
//! ///////////////////////////////////////////////////////////////////////////

#include "trice.h"

#ifdef TRICE_RTT0
#include "./box/SEGGER_RTT.c"
#endif

#if TRICE_INTERMEDIATE_BUFFER == TRICE_STACK_BUFFER
#include "./box/triceStackBuffer.c"
#endif

#if TRICE_INTERMEDIATE_BUFFER == TRICE_STATIC_BUFFER
#include "./box/triceStaticBuffer.c"
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



void showTriceDepth( void ){
    #if TRICE_MODE == TRICE_DOUBLE_BUFFER
    {
        uint16_t triceDepth = TriceDepth();
        uint16_t triceDepthMax = TriceDepthMax();
        TRice16( iD( 3281), "MSG: ?? STOP  TriceDepth = %d, TriceDepthMax = %d + %4u of %d\n", triceDepth, TRICE_DATA_OFFSET, triceDepthMax-TRICE_DATA_OFFSET, TRICE_HALF_BUFFER_SIZE );
    } 
    #endif
    #if TRICE_MODE == TRICE_STREAM_BUFFER
        TRice( iD( 5738), "MSG:triceFifoDepthMax = %d of max %d, triceStreamBufferDepthMax = %d of max %d\n", triceFifoDepthMax, TRICE_FIFO_ELEMENTS, triceStreamBufferDepthMax, TRICE_BUFFER_SIZE );
    #endif
}

void TriceLogDepthMax( void ){
    #if TRICE_MODE == TRICE_DOUBLE_BUFFER
        size_t tdm = TriceDepthMax();
        if( tdm <= TRICE_HALF_BUFFER_SIZE ){
            TRice16( iD( 6149), "diag:TriceDepthMax =%4u of %d\n", tdm, TRICE_HALF_BUFFER_SIZE );
        }else{
            TRice16( iD( 6885), "err:TriceDepthMax =%4u of %d (overflow!)\n", tdm, TRICE_HALF_BUFFER_SIZE );
        }
    #endif
}

//! TriceDiagnostics logs the trice buffer usage, when index is 0 or a nes max value was reached.
void TriceDiagnostics( int index ){
    if( index == 0){ // cyclic diagnostics
        TriceLogDepthMax();
        return;
    }
    // max diagnostics
    #if TRICE_MODE == TRICE_DOUBLE_BUFFER
        static uint16_t triceDepthMax_1 = 0;
        uint16_t triceDepthMax = TriceDepthMax();
        if( triceDepthMax_1 != triceDepthMax ){
            triceDepthMax_1 = triceDepthMax;
            TriceLogDepthMax();
        }
    #endif
    #if TRICE_MODE == TRICE_STREAM_BUFFER
        static uint16_t triceFifoDepthMax_1, triceStreamBufferDepthMax_1;
        if( triceFifoDepthMax_1 != triceFifoDepthMax || triceStreamBufferDepthMax_1 != triceStreamBufferDepthMax ){
            triceFifoDepthMax_1 = triceFifoDepthMax;
            triceStreamBufferDepthMax_1 = triceStreamBufferDepthMax;
            TRICE16( Id( 2192), "MSG:triceFifoDepthMax = %d of max %d, triceStreamBufferDepthMax = %d of max %d\n", triceFifoDepthMax, TRICE_FIFO_ELEMENTS, triceStreamBufferDepthMax, TRICE_BUFFER_SIZE );
        }
    #endif
}
