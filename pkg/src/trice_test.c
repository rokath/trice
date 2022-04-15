/*! \file trice_test.c
\brief wrapper for tests
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/
#include "trice.h"
#include "trice_test.h"
#define TRICE_FILE Id(52024) 

void SetTriceBuffer( uint8_t* buf  ){
    triceBuffer = buf;
}

int T1( void ){ TRICE32_1( Id(58755), "rd:TRICE32_1 line %d (%%d)\n", -1 ); return triceBufferDepth; }
int T2( void ){ TRICE64( Id( 52183), "rd:TRICE64 %d, %d\n", 1, 2 );         return triceBufferDepth; }

int TN( int n ){
    switch( n ){
        case 1: TRICE32_1( Id(58755), "rd:TRICE32_1 line %d (%%d)\n", -1 ); return triceBufferDepth;
        case 2: TRICE64( Id( 52183), "rd:TRICE64 %d, %d\n", 1, 2 );         return triceBufferDepth; 
    }
}
