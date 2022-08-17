/*! \file trice_test.c
\brief wrapper for tests
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/
#include "trice.h"
#define TRICE_FILE Id(10145)
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
        case 0: TRICE32_1( Id(13245), "rd:TRICE32_1 line %d (%%d)\n", -1 ); return triceBufferDepth;
        case 1: TRICE64( Id(14227), "rd:TRICE64 %d, %d\n", 1, 2 );         return triceBufferDepth; 
        case 2: s = "AAAAAAAAAAAA"; TRICE_S( Id(13947), "sig:%s\n", s );   return triceBufferDepth; 
    }
    return 0;
}


//#ifdef TRICE_CGO_TEST


int triceBufferDepth = 0;
uint8_t* triceBuffer;

uint32_t ReadTime( void ){
    return 0x11111111;
}

//#endif // #ifdef TRICE_CGO_TEST
