//! \file triceStackBuffer.c
//! \author Thomas.Hoehenleitner [at] seerose.net
//! //////////////////////////////////////////////////////////////////////////
#include "trice.h"

#if TRICE_BUFFER == TRICE_STACK_BUFFER

#if TRICE_DIAGNOSTICS == 1

//! triceSingleMaxWordCount is a diagnostics value usable to optimize buffer size.
unsigned triceSingleMaxWordCount = 0;

//! TriceLogDiagnosticValues shows the max used half buffer space. 
void TriceLogDiagnosticValues( void ){
    #ifdef SEGGER_RTT
    TriceLogSeggerDiagnostics();
    #endif

    unsigned triceSingleDepthMax = TRICE_DATA_OFFSET + (triceSingleMaxWordCount<<2);
    if( triceSingleDepthMax <= TRICE_BUFFER_SIZE ){
        TRice16( iD( 6536), "diag:triceSingleDepthMax =%4u of %d\n", triceSingleDepthMax, TRICE_BUFFER_SIZE );
    }else{
        TRice16( iD( 5233), "err:triceSingleDepthMax =%4u of %d (overflow!)\n", triceSingleDepthMax, TRICE_BUFFER_SIZE );
    }
}

#endif

#ifdef TRICE_CGO
void TriceTransfer( void ){} // todo 
#endif

#endif // #if TRICE_BUFFER == TRICE_STACK_BUFFER
