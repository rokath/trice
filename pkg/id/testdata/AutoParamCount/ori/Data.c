/*! \file triceCheck.c
\brief trices for tool evaluation
\details The trices are dumped as 32bit values into a 32 bit wide fifo.
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/

#include "trice.h"

/*! write out all types of trices with fixed values for testing
\details One trice has one subtrace, if param size max 2 bytes. 
Traces with more bytes as parameter consist of several subtraces.
*/
void triceCheckSet( void )
{
    // with count, with id, more in one line, optional semi-colon
     TRICE8_1( Id(0), "tst: %d\n", n );
     TRICE8_1( Id(0), "tst: %d\n", n ); TRICE8_1( Id(0), "tst: %d\n", n );
     TRICE8_1( Id(0), "tst: %d\n", n ); TRICE8_1( Id(0), "tst: %d\n", n ); TRICE8_1( Id(0), "tst: %d\n", n );
     TRICE8_1( Id(0), "tst: %d\n", n )
     TRICE8_1( Id(0), "tst: %d\n", n )  TRICE8_1( Id(0), "tst: %d\n", n ) 
     TRICE8_1( Id(0), "tst: %d\n", n )  TRICE8_1( Id(0), "tst: %d\n", n )  TRICE8_1( Id(0), "tst: %d\n", n ) 
     TRICE8_1( Id(0), "tst: %d\n", n ); TRICE8_1( Id(0), "tst: %d\n", n ); TRICE8_1( Id(0), "tst: %d\n", n ) 

    // without count, without id, more in one line, optional semi-colon
     TRICE8( "tst: %d\n", n );
     TRICE8( "tst: %d\n", n ); TRICE8( "tst: %d\n", n );
     TRICE8( "tst: %d\n", n ); TRICE8( "tst: %d\n", n ); TRICE8( "tst: %d\n", n );
     TRICE8( "tst: %d\n", n ) 
     TRICE8( "tst: %d\n", n )  TRICE8( "tst: %d\n", n ) 
     TRICE8( "tst: %d\n", n )  TRICE8( "tst: %d\n", n )  TRICE8( "tst: %d\n", n ) 
     TRICE8( "tst: %d\n", n ); TRICE8( "tst: %d\n", n ); TRICE8( "tst: %d\n", n ) 

    // mixed
     TRICE8( "tst: %d\n", n );  TRICE8_1( Id(0), "tst: %d\n", n )
     TRICE8_1( Id(0), "tst: %d\n", n );  TRICE8( "tst: %d\n", n )

    // param count
    TRICE0( "tst: Hi" );
    
    TRICE8( "tst:TRICE8 %d\n", 1 );
    TRICE8( "tst:TRICE8 %d %d\n", 1, 2 );
    TRICE8( "tst:TRICE8 %d %d %d\n", 1, 2, 3 );
    TRICE8( "tst:TRICE8 %d %d %d %d\n", 1, 2, 3, 4 ); 
    TRICE8( "tst:TRICE8 %d %d %d %d %d\n", 1, 2, 3, 4, 5 );
    TRICE8( "tst:TRICE8 %d %d %d %d %d %d \n", 1, 2, 3, 4, 5, 6 );
    TRICE8( "tst:TRICE8 %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7 ); 
    TRICE8( "tst:TRICE8 %d %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7, 8 ); 

    TRICE16( "tst:TRICE16 %d\n", 1 );
    TRICE16( "tst:TRICE16 %d %d\n", 1, 2 );
    TRICE16( "tst:TRICE16 %d %d %d\n", 1, 2, 3 );
    TRICE16( "tst:TRICE16 %d %d %d %d\n", 1, 2, 3, 4 );

    TRICE32( "tst:TRICE32 %d\n", 1 );
    TRICE32( "tst:TRICE32 %d %d\n", 1, 2 );
    TRICE32( "tst:TRICE32 %d %d %d\n", 1, 2, 3 );
    TRICE32( "tst:TRICE32 %d %d %d %d\n", 1, 2, 3, 4 );

    TRICE64( "tst:TRICE64 %d %d %d\n", 1, 2, 3 );
    TRICE64( "tst:TRICE64 %d %d %d %d\n", 1, 2, 3, 4 );
    
    // special chars
    TRICE0( "tst: Hi>" );
    TRICE0( "tst: Hi<" );
    TRICE0( "tst: Hi|" );
    TRICE0( "tst: Hi)" );
    TRICE0( "tst: Hi(" );
    TRICE0( "tst: Hi>>" );
    TRICE0( "tst: Hi<<" );
} 

