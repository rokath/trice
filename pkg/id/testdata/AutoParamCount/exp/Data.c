/*! \file triceCheck.c
\brief trices for tool evaluation
\details The trices are dumped as 32bit values into a 32 bit wide fifo.
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/

//#define TRICE_OFF
#include "trice.h"

#define SYSTICKVAL16 SysTick->VAL //!< STM32 specific

// with count, with id
 TRICE8_1( Id(43255), "tst: %d\n", n );
 TRICE8_1( Id( 9070), "tst: %d\n", n ); TRICE8_1( Id(46064), "tst: %d\n", n );
 TRICE8_1( Id(50207), "tst: %d\n", n ); TRICE8_1( Id(18441), "tst: %d\n", n ); TRICE8_1( Id(55022), "tst: %d\n", n );
 TRICE8_1( Id(60593), "tst: %d\n", n )
 TRICE8_1( Id(16488), "tst: %d\n", n )  TRICE8_1( Id(59359), "tst: %d\n", n ) 
 TRICE8_1( Id(62524), "tst: %d\n", n )  TRICE8_1( Id(34009), "tst: %d\n", n )  TRICE8_1( Id(  453), "tst: %d\n", n ) 
 TRICE8_1( Id( 1650), "tst: %d\n", n ); TRICE8_1( Id(21904), "tst: %d\n", n ); TRICE8_1( Id(42223), "tst: %d\n", n ) 

// without count, without id
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


/*! write out all types of trices with fixed values for testing
\details One trice has one subtrace, if param size max 2 bytes. 
Traces with more bytes as parameter consist of several subtraces.
111 subtraces need 444 byte buffer space
*/
void triceCheckSet( void )
{
    TRICE8_4( Id(51789), "tst:TRICE8  %%07x ->  %03x  %03x  %03x  %03x\n", 1, 0x7f, 0x80, 0xff ); // 4
    TRICE8_4( Id(23804), "tst:TRICE8   %%4d -> %4d %4d %4d %4d\n", 1, 0x7f, 0x80, 0xff ); // 4
    TRICE8_5( Id(23826), "tst:TRICE8  %4o -> %4o %4o %4o %4o\n", 1, 0x7f, 0x80, 0xff ); // 4
    // 3 * 2 ss

    TRICE16_4( Id(63671), "tst:TRICE16  %%05x ->   %05x   %05x   %05x   %05x\n", 1, 0x7fff, 0x8000, 0xffff ); // 4
    TRICE16_4( Id(52200), "tst:TRICE16   %%6d ->  %6d  %6d  %6d  %6d\n", 1, 0x7fff, 0x8000, 0xffff ); // 4
    TRICE16_4( Id(21190), "tst:TRICE16   %%7o -> %7o %7o %7o %7o\n", 1, 0x7fff, 0x8000, 0xffff ); // 4
    // 3 * ss

    TRICE32_2( Id(37793), "tst:TRICE32   %%09x ->     %09x     %09x", 1, 0x7fffffff ); // 4
    TRICE32_2( Id(20305), "tst:     %09x     %09x\n", 0x80000000, 0xffffffff ); // 4
    TRICE32_2( Id(32857), "tst:TRICE32   %%11d ->   %11d   %11d", 1, 0x7fffffff ); // 4
    TRICE32_2( Id( 8334), "tst:   %11d   %11d\n", 0x80000000, 0xffffffff  ); // 4
    TRICE32_2( Id(11330), "tst:TRICE32   %%12o ->  %12o  %12o", 1, 0x7fffffff ); // 4
    TRICE32_2( Id(30653), "tst:  %12o  %12o\n", 0x80000000, 0xffffffff ); // 4
    // 6 * 4 ss

    TRICE8_1( Id(18784), "tst:TRICE8 %d\n", 1 ); // 1
    TRICE8_2( Id(19051), "tst:TRICE8 %d %d\n", 1, 2 ); // 1
    TRICE8_3( Id(52558), "tst:TRICE8 %d %d %d\n", 1, 2, 3 ); // 2
    TRICE8_4( Id( 9988), "tst:TRICE8 %d %d %d %d\n", 1, 2, 3, 4 ); // 2
    TRICE8_5( Id( 2451), "tst:TRICE8 %d %d %d %d %d\n", 1, 2, 3, 4, 5 ); // 3
    TRICE8_6( Id(23714), "tst:TRICE8 %d %d %d %d %d %d \n", 1, 2, 3, 4, 5, 6 ); // 3
    TRICE8_7( Id(26365), "tst:TRICE8 %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7 ); // 4
    TRICE8_8( Id(58043), "tst:TRICE8 %d %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7, 8 ); // 4
    // 18 subt
    
    TRICE16_1( Id(52760), "tst:TRICE16 %d\n", 1 ); // 1
    TRICE16_2( Id(52381), "tst:TRICE16 %d %d\n", 1, 2 ); // 2
    TRICE16_3( Id(20848), "tst:TRICE16 %d %d %d\n", 1, 2, 3 ); // 3
    TRICE16_4( Id(63360), "tst:TRICE16 %d %d %d %d\n", 1, 2, 3, 4 ); // 4
    // 10 subt

    TRICE32_1( Id(60417), "tst:TRICE32 %d\n", 1 ); // 2
    TRICE32_2( Id(22410), "tst:TRICE32 %d %d\n", 1, 2 ); // 4
    // 6 subtraces
    
    TRICE16_1( Id(15466), "ERR:error       message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(44598), "WRN:warning     message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(34858), "ATT:attension   message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(55029), "DIA:diagnostics message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(52610), "TIM:timing      message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(23814), "DBG:debug       message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(16700), "SIG:signal      message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(36066), "RD_:read        message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(34860), "WR_:write       message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(65297), "TST:test        message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(49609), "MSG:normal      message, SysTick is %d\n", SYSTICKVAL16 ); // 1

    TRICE16_1( Id(15607), "err:error       message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(35565), "wrn:warning     message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(15793), "att:attension   message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(20234), "dia:diagnostics message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(  909), "tim:timing      message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(  155), "dbg:debug       message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id( 7205), "sig:signal      message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(55054), "rd_:read        message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(27424), "wr_:write       message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(57100), "tst:test        message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(49174), "msg:normal      message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    // 22 subtraces
    
} // sum 111 sub traces (need 444 bytes Buffet space)

