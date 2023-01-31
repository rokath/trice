/*! \file triceCheck.c
\brief wrapper for tests
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/
#include "trice.h"

// TriceCheck performs trice code sequence n.
// This function is called from Go for tests.
// Each trice line needs to have a commented // "string" with its expected output.
// The ID values must be in this file, because it is compiled first and trice update runs later.
void TriceCheck( int n ){
    switch( n ){
        default:
        break; case 0: TRICE8( id(7), "value=%d", 0x7F ); //exp: `value=127`
        break; case 1: TRICE8( id(9), "value=%d", 0x80 ); //exp: `value=-128`
        break;
    }
}
