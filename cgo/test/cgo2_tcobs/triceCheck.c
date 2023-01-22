/*! \file triceCheck.c
\brief wrapper for tests
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/
#include "trice.h"

// TriceCheck performs trice code sequence n.
// This function is called from Go for tests.
void TriceCheck( int n ){
    switch( n ){
        //case  0: TRICE32_1( id( 5595), "rd:TRICE32_1 line %d (%%d)\n", -1 );
        case  0: TRICE32_1( id(    2), "rd:TRICE32_1 line %d (%%d)\n", -1 );
    }
}
