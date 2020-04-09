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
     TRICE8_1( Id(43255), "tst: %d\n", n );
     TRICE8_1( Id( 9070), "tst: %d\n", n ); TRICE8_1( Id(46064), "tst: %d\n", n );
     TRICE8_1( Id(50207), "tst: %d\n", n ); TRICE8_1( Id(18441), "tst: %d\n", n ); TRICE8_1( Id(55022), "tst: %d\n", n );
     TRICE8_1( Id(60593), "tst: %d\n", n )
     TRICE8_1( Id(16488), "tst: %d\n", n )  TRICE8_1( Id(59359), "tst: %d\n", n ) 
     TRICE8_1( Id(62524), "tst: %d\n", n )  TRICE8_1( Id(34009), "tst: %d\n", n )  TRICE8_1( Id(  453), "tst: %d\n", n ) 
     TRICE8_1( Id( 1650), "tst: %d\n", n ); TRICE8_1( Id(21904), "tst: %d\n", n ); TRICE8_1( Id(42223), "tst: %d\n", n ) 

    // without count, without id, more in one line, optional semi-colon
     TRICE8_1( Id(60742), "tst: %d\n", n );
     TRICE8_1( Id(16872), "tst: %d\n", n ); TRICE8_1( Id(53631), "tst: %d\n", n );
     TRICE8_1( Id(15266), "tst: %d\n", n ); TRICE8_1( Id(61105), "tst: %d\n", n ); TRICE8_1( Id(63786), "tst: %d\n", n );
     TRICE8_1( Id(44194), "tst: %d\n", n ) 
     TRICE8_1( Id(57402), "tst: %d\n", n )  TRICE8_1( Id(38851), "tst: %d\n", n ) 
     TRICE8_1( Id(15177), "tst: %d\n", n )  TRICE8_1( Id(56737), "tst: %d\n", n )  TRICE8_1( Id(38738), "tst: %d\n", n ) 
     TRICE8_1( Id(16966), "tst: %d\n", n ); TRICE8_1( Id(57737), "tst: %d\n", n ); TRICE8_1( Id(28718), "tst: %d\n", n ) 

    // mixed
     TRICE8_1( Id(26878), "tst: %d\n", n );  TRICE8_1( Id(53334), "tst: %d\n", n )
     TRICE8_1( Id(43551), "tst: %d\n", n );  TRICE8_1( Id(64025), "tst: %d\n", n )

    // param count
    TRICE0( Id(51789), "tst: Hi" );
    
    TRICE8_1( Id(23804), "tst:TRICE8 %d\n", 1 );
    TRICE8_2( Id(23826), "tst:TRICE8 %d %d\n", 1, 2 );
    TRICE8_3( Id(63671), "tst:TRICE8 %d %d %d\n", 1, 2, 3 );
    TRICE8_4( Id(52200), "tst:TRICE8 %d %d %d %d\n", 1, 2, 3, 4 ); 
    TRICE8_5( Id(21190), "tst:TRICE8 %d %d %d %d %d\n", 1, 2, 3, 4, 5 );
    TRICE8_6( Id(37793), "tst:TRICE8 %d %d %d %d %d %d \n", 1, 2, 3, 4, 5, 6 );
    TRICE8_7( Id(20305), "tst:TRICE8 %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7 ); 
    TRICE8_8( Id(32857), "tst:TRICE8 %d %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7, 8 ); 

    TRICE16_1( Id( 8334), "tst:TRICE16 %d\n", 1 );
    TRICE16_2( Id(11330), "tst:TRICE16 %d %d\n", 1, 2 );
    TRICE16_3( Id(30653), "tst:TRICE16 %d %d %d\n", 1, 2, 3 );
    TRICE16_4( Id(18784), "tst:TRICE16 %d %d %d %d\n", 1, 2, 3, 4 );

    TRICE32_1( Id(19051), "tst:TRICE32 %d\n", 1 );
    TRICE32_2( Id(52558), "tst:TRICE32 %d %d\n", 1, 2 );
    TRICE32_3( Id( 9988), "tst:TRICE32 %d %d %d\n", 1, 2, 3 );
    TRICE32_4( Id( 2451), "tst:TRICE32 %d %d %d %d\n", 1, 2, 3, 4 );

    TRICE64_3( Id(23714), "tst:TRICE64 %d %d %d\n", 1, 2, 3 );
    TRICE64_4( Id(26365), "tst:TRICE64 %d %d %d %d\n", 1, 2, 3, 4 );
    
    // special chars
    TRICE0( Id(58043), "tst: Hi>" );
    TRICE0( Id(52760), "tst: Hi<" );
    TRICE0( Id(52381), "tst: Hi|" );
    TRICE0( Id(20848), "tst: Hi)" );
    TRICE0( Id(63360), "tst: Hi(" );
    TRICE0( Id(60417), "tst: Hi>>" );
    TRICE0( Id(22410), "tst: Hi<<" );
} 

