/*! \file triceCheck.c
\brief trices for tool evaluation
\details The trices are dumped as 32bit values into a 32 bit wide fifo.
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/

//#define TRICE_OFF
#include "trice.h"

#define SYSTICKVAL16 SysTick->VAL //!< STM32 specific

// with count, with id
 TRICE8_1( Id(0), "tst: %d\n", n );
 TRICE8_1( Id(0), "tst: %d\n", n ); TRICE8_1( Id(0), "tst: %d\n", n );
 TRICE8_1( Id(0), "tst: %d\n", n ); TRICE8_1( Id(0), "tst: %d\n", n ); TRICE8_1( Id(0), "tst: %d\n", n );
 TRICE8_1( Id(0), "tst: %d\n", n )
 TRICE8_1( Id(0), "tst: %d\n", n )  TRICE8_1( Id(0), "tst: %d\n", n ) 
 TRICE8_1( Id(0), "tst: %d\n", n )  TRICE8_1( Id(0), "tst: %d\n", n )  TRICE8_1( Id(0), "tst: %d\n", n ) 
 TRICE8_1( Id(0), "tst: %d\n", n ); TRICE8_1( Id(0), "tst: %d\n", n ); TRICE8_1( Id(0), "tst: %d\n", n ) 

// without count, without id
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


/*! write out all types of trices with fixed values for testing
\details One trice has one subtrace, if param size max 2 bytes. 
Traces with more bytes as parameter consist of several subtraces.
111 subtraces need 444 byte buffer space
*/
void triceCheckSet( void )
{
    TRICE8( "tst:TRICE8  %%07x ->  %03x  %03x  %03x  %03x\n", 1, 0x7f, 0x80, 0xff ); // 4
    TRICE8( "tst:TRICE8   %%4d -> %4d %4d %4d %4d\n", 1, 0x7f, 0x80, 0xff ); // 4
    TRICE8( "tst:TRICE8  %4o -> %4o %4o %4o %4o\n", 1, 0x7f, 0x80, 0xff ); // 4
    // 3 * 2 ss

    TRICE16( "tst:TRICE16  %%05x ->   %05x   %05x   %05x   %05x\n", 1, 0x7fff, 0x8000, 0xffff ); // 4
    TRICE16( "tst:TRICE16   %%6d ->  %6d  %6d  %6d  %6d\n", 1, 0x7fff, 0x8000, 0xffff ); // 4
    TRICE16( "tst:TRICE16   %%7o -> %7o %7o %7o %7o\n", 1, 0x7fff, 0x8000, 0xffff ); // 4
    // 3 * ss

    TRICE32( "tst:TRICE32   %%09x ->     %09x     %09x", 1, 0x7fffffff ); // 4
    TRICE32( "tst:     %09x     %09x\n", 0x80000000, 0xffffffff ); // 4
    TRICE32( "tst:TRICE32   %%11d ->   %11d   %11d", 1, 0x7fffffff ); // 4
    TRICE32( "tst:   %11d   %11d\n", 0x80000000, 0xffffffff  ); // 4
    TRICE32( "tst:TRICE32   %%12o ->  %12o  %12o", 1, 0x7fffffff ); // 4
    TRICE32( "tst:  %12o  %12o\n", 0x80000000, 0xffffffff ); // 4
    // 6 * 4 ss

    TRICE8( "tst:TRICE8 %d\n", 1 ); // 1
    TRICE8( "tst:TRICE8 %d %d\n", 1, 2 ); // 1
    TRICE8( "tst:TRICE8 %d %d %d\n", 1, 2, 3 ); // 2
    TRICE8( "tst:TRICE8 %d %d %d %d\n", 1, 2, 3, 4 ); // 2
    TRICE8( "tst:TRICE8 %d %d %d %d %d\n", 1, 2, 3, 4, 5 ); // 3
    TRICE8( "tst:TRICE8 %d %d %d %d %d %d \n", 1, 2, 3, 4, 5, 6 ); // 3
    TRICE8( "tst:TRICE8 %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7 ); // 4
    TRICE8( "tst:TRICE8 %d %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7, 8 ); // 4
    // 18 subt
    
    TRICE16( "tst:TRICE16 %d\n", 1 ); // 1
    TRICE16( "tst:TRICE16 %d %d\n", 1, 2 ); // 2
    TRICE16( "tst:TRICE16 %d %d %d\n", 1, 2, 3 ); // 3
    TRICE16( "tst:TRICE16 %d %d %d %d\n", 1, 2, 3, 4 ); // 4
    // 10 subt

    TRICE32( "tst:TRICE32 %d\n", 1 ); // 2
    TRICE32( "tst:TRICE32 %d %d\n", 1, 2 ); // 4
    // 6 subtraces
    
    TRICE16( "ERR:error       message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16( "WRN:warning     message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16( "ATT:attension   message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16( "DIA:diagnostics message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16( "TIM:timing      message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16( "DBG:debug       message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16( "SIG:signal      message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16( "RD_:read        message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16( "WR_:write       message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16( "TST:test        message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16( "MSG:normal      message, SysTick is %d\n", SYSTICKVAL16 ); // 1

    TRICE16( "err:error       message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16( "wrn:warning     message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16( "att:attension   message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16( "dia:diagnostics message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16( "tim:timing      message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16( "dbg:debug       message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16( "sig:signal      message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16( "rd_:read        message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16( "wr_:write       message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16( "tst:test        message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16( "msg:normal      message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    // 22 subtraces
    
} // sum 111 sub traces (need 444 bytes Buffet space)

