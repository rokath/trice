//! \file triceDiagnostics.c
//! \author Thomas.Hoehenleitner [at] seerose.net
//! //////////////////////////////////////////////////////////////////////////
#include "trice.h"

//! triceDepthMax is a diagnostics value usable to optimize buffer size.
unsigned triceDepthMax = 0; 

void showTriceDepth( void ){
    #if TRICE_BUFFER == TRICE_DOUBLE_BUFFER
    {
        uint32_t triceDepth = TriceDepth();
        triceDepthMax = TriceDepthMax();
        TRice( iD( 3668), "MSG: ?? STOP  TriceDepth = %d, TriceDepthMax = %d + %4u of %d\n", triceDepth, TRICE_DATA_OFFSET, triceDepthMax-TRICE_DATA_OFFSET, TRICE_HALF_BUFFER_SIZE ); //lint !e585
    } 
    #endif
}

void TriceLogDepthMax( void ){
    #if TRICE_BUFFER == TRICE_DOUBLE_BUFFER
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
    #if TRICE_BUFFER == TRICE_DOUBLE_BUFFER
    static unsigned triceDepthMax_1 = 0;
    triceDepthMax = TriceDepthMax();
    if( triceDepthMax_1 != triceDepthMax ){
        triceDepthMax_1 = triceDepthMax;
        TriceLogDepthMax();
    }
    #endif
}
