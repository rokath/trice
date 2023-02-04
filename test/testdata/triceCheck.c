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
        break; case __LINE__: TRICE8( id(   16), "value=%d", 0x7F ); //exp: `value=127`
        break; case __LINE__: TRICE8( id(   15), "value=%d", 0x80 ); //exp: `value=-128`
        break;
    }
}

//  void TriceLogDepthMax( void ){
//      size_t tdm = TriceDepthMax();
//      if( tdm <= TRICE_HALF_BUFFER_SIZE ){
//          TRICE16( Id( 1420),"diag:TriceDepthMax =%4u of %d\n", tdm, TRICE_HALF_BUFFER_SIZE );
//      }else{
//          TRICE16( Id( 2535),"err:TriceDepthMax =%4u of %d (overflow!)\n", tdm, TRICE_HALF_BUFFER_SIZE );
//      }
//  }
