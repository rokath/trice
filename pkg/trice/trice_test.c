/*! \file trice_test.c
\brief wrapper for tests
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/
#include "trice.h"
#include "trice_test.h"
#include "cgoTrice.h"

// TriceCode performs trice code sequence n and returns the amount of into triceBuffer written bytes.
// This function is called from Go for tests. DO NOT CHANGE LINE NUMBER POSITIONS!!!
int TriceCode( int n ){
    char* s;
    switch( n ){
        case  0: TRICE32_1( id( 5595), "rd:TRICE32_1 line %d (%%d)\n", -1 ); return cgoTriceBufferDepth;
        case  1: TRICE32_1( Id( 5595), "rd:TRICE32_1 line %d (%%d)\n", -1 ); return cgoTriceBufferDepth;
        case  2: TRICE32_1( ID( 5595), "rd:TRICE32_1 line %d (%%d)\n", -1 ); return cgoTriceBufferDepth;
        case  3: TRICE32_1( iD( 5595), "rd:TRICE32_1 line %d (%%d)\n", -1 ); return cgoTriceBufferDepth;
        case  4: TRICE64( id( 5866), "rd:TRICE64 %d, %d\n", 1, 2 );          return cgoTriceBufferDepth;
        case  5: TRICE64( Id( 5866), "rd:TRICE64 %d, %d\n", 1, 2 );          return cgoTriceBufferDepth;
        case  6: TRICE64( ID( 5866), "rd:TRICE64 %d, %d\n", 1, 2 );          return cgoTriceBufferDepth;
        case  7: TRICE64( iD( 5866), "rd:TRICE64 %d, %d\n", 1, 2 );          return cgoTriceBufferDepth;
        case  8: s = "AAAAAAAAAAAA"; TRICE_S( id( 5928), "sig:%s\n", s );    return cgoTriceBufferDepth;
        case  9: s = "AAAAAAAAAAAA"; TRICE_S( Id( 5928), "sig:%s\n", s );    return cgoTriceBufferDepth;
        case 10: s = "AAAAAAAAAAAA"; TRICE_S( ID( 5928), "sig:%s\n", s );    return cgoTriceBufferDepth;
        case 11: s = "AAAAAAAAAAAA"; TRICE_S( id( 5928), "sig:%s\n", s );    return cgoTriceBufferDepth;
    }
    return 0;
}
