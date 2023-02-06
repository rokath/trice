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
        break; case __LINE__: TRICE8( ID( 3115), "value=%u", 0x7e );          //exp: `value=126`
        break; case __LINE__: TRICE8( ID( 3516), "value=%u", 2 );             //exp: `value=2`
        break; case __LINE__: trice8( iD( 7805), "value=%x", 0x55 );                 //exp: `value=55`
        break; case __LINE__: TRICE8( id( 4666), "value=%d", 0x80 );          //exp: `value=-128`
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
