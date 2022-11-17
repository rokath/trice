/*! \file trice_test.c
\brief wrapper for tests
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/
#include "trice.h"
#include "trice_test.h"

// SetTriceBuffer sets the internal triceBuffer pointer to buf. 
// This function is called from Go for test setup.
void SetTriceBuffer( uint8_t* buf  ){
    triceBuffer = buf;
}

// TriceCode performs trice code sequence n and returns the amount of into triceBuffer written bytes.
// This function is called from Go for tests. DO NOT CHANGE LINE NUMBER POSITIONS!!!
int TriceCode( int n ){
    char* s;
    switch( n ){
        case  0: TRICE32_1( id( 5595), "rd:TRICE32_1 line %d (%%d)\n", -1 ); return triceBufferDepth;
        case  1: TRICE32_1( Id( 5595), "rd:TRICE32_1 line %d (%%d)\n", -1 ); return triceBufferDepth;
        case  2: TRICE32_1( ID( 5595), "rd:TRICE32_1 line %d (%%d)\n", -1 ); return triceBufferDepth;
        case  3: TRICE32_1( iD( 5595), "rd:TRICE32_1 line %d (%%d)\n", -1 ); return triceBufferDepth;
        case  4: TRICE64( id( 5866), "rd:TRICE64 %d, %d\n", 1, 2 );          return triceBufferDepth;
        case  5: TRICE64( Id( 5866), "rd:TRICE64 %d, %d\n", 1, 2 );          return triceBufferDepth;
        case  6: TRICE64( ID( 5866), "rd:TRICE64 %d, %d\n", 1, 2 );          return triceBufferDepth;
        case  7: TRICE64( iD( 5866), "rd:TRICE64 %d, %d\n", 1, 2 );          return triceBufferDepth;
        case  8: s = "AAAAAAAAAAAA"; TRICE_S( id( 5928), "sig:%s\n", s );    return triceBufferDepth;
        case  9: s = "AAAAAAAAAAAA"; TRICE_S( Id( 5928), "sig:%s\n", s );    return triceBufferDepth;
        case 10: s = "AAAAAAAAAAAA"; TRICE_S( ID( 5928), "sig:%s\n", s );    return triceBufferDepth;
        case 11: s = "AAAAAAAAAAAA"; TRICE_S( id( 5928), "sig:%s\n", s );    return triceBufferDepth;
    }
    return 0;
}

int triceBufferDepth = 0;
uint8_t* triceBuffer;

uint16_t TriceStamp16( void ){
    return 0x1616;
}

uint32_t TriceStamp32( void ){ 
    return 0x32323232; 
}

uint64_t TriceStamp64( void ){ 
    return 0x6464646464646464; 
}


//! triceNonBlockingWriteCgoBuffer copies buf with len into triceBuffer.
void triceNonBlockingWriteCgoBuffer( uint8_t const * buf, unsigned len ){
    //extern uint8_t* triceBuffer;
    //extern int triceBufferDepth;
    memcpy(triceBuffer, buf, len);
    triceBufferDepth = len;
}
