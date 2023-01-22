/*! \file triceCheck.c
\brief wrapper for tests
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/
#include "trice.h"

// TriceCheck performs trice code sequence n.
// This function is called from Go for tests.
// Each trice line needs to have a commented // "string" with its expected output.
void TriceCheck( int n ){
    switch( n ){
        case  0: TRICE8_1( id(    2), "value=%d\n", -1 ); // "value=-1\n"
    }
}
