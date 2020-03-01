/*! \file triceCheck.c
\brief trices for tool evaluation
\details The trices are dumped as 32bit values into a 32 bit wide fifo.
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/

//#define TRICE_OFF
#include "trice.h"

#define SYSTICKVAL16 SysTick->VAL //!< STM32 specific

/*! write out all types of trices with fixed values for testing
\details One trice has one subtrace, if param size max 2 bytes. 
Traces with more bytes as parameter consist of several subtraces.
111 subtraces need 444 byte buffer space
*/
void triceCheckSet( void )
{
    TRICE0 (Id(65261), "--------------------------------------------------------------------------------------------------------------------------------------------------\n" );
    // 1 subtrace

    TRICE8_4( Id(51789), "tst:TRICE8  %%03x ->  %03x  %03x  %03x  %03x\n", 1, 0x7f, 0x80, 0xff ); // 4
    TRICE8_4( Id(30364), "tst:TRICE8   %%4d -> %4d %4d %4d %4d\n", 1, 0x7f, 0x80, 0xff ); // 4
    TRICE8_4( Id( 4648), "tst:TRICE8   %%4o -> %4o %4o %4o %4o\n", 1, 0x7f, 0x80, 0xff ); // 4
    // 3 * 2 subtraces

    TRICE16_4( Id(36667), "tst:TRICE16  %%05x ->   %05x   %05x   %05x   %05x\n", 1, 0x7fff, 0x8000, 0xffff ); // 4
    TRICE16_4( Id(39908), "tst:TRICE16   %%6d ->  %6d  %6d  %6d  %6d\n", 1, 0x7fff, 0x8000, 0xffff ); // 4
    TRICE16_4( Id(60212), "tst:TRICE16   %%7o -> %7o %7o %7o %7o\n", 1, 0x7fff, 0x8000, 0xffff ); // 4
    // 3 * 4 subtraces

    TRICE32_2( Id(60059), "tst:TRICE32   %%09x ->     %09x     %09x", 1, 0x7fffffff ); // 4
    TRICE32_2( Id(50464), "tst:     %09x     %09x\n", 0x80000000, 0xffffffff ); // 4
    TRICE32_2( Id(30279), "tst:TRICE32   %%11d ->   %11d   %11d", 1, 0x7fffffff ); // 4
    TRICE32_2( Id(40670), "tst:   %11d   %11d\n", 0x80000000, 0xffffffff  ); // 4
    TRICE32_2( Id(40127), "tst:TRICE32   %%12o ->  %12o  %12o", 1, 0x7fffffff ); // 4
    TRICE32_2( Id( 4479), "tst:  %12o  %12o\n", 0x80000000, 0xffffffff ); // 4
    // 6 * 4 subtraces

    TRICE8_1( Id(37890), "tst:TRICE8 %d\n", 1 ); // 1
    TRICE8_2( Id(29394), "tst:TRICE8 %d %d\n", 1, 2 ); // 1
    TRICE8_3( Id(28923), "tst:TRICE8 %d %d %d\n", 1, 2, 3 ); // 2
    TRICE8_4( Id(38460), "tst:TRICE8 %d %d %d %d\n", 1, 2, 3, 4 ); // 2
    TRICE8_5( Id(52346), "tst:TRICE8 %d %d %d %d %d\n", 1, 2, 3, 4, 5 ); // 3
    TRICE8_6( Id( 8503), "tst:TRICE8 %d %d %d %d %d %d \n", 1, 2, 3, 4, 5, 6 ); // 3
    TRICE8_7( Id( 6807), "tst:TRICE8 %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7 ); // 4
    TRICE8_8( Id(51953), "tst:TRICE8 %d %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7, 8 ); // 4
    // 18 subtraces
    
    TRICE16_1( Id(18564), "tst:TRICE16 %d\n", 1 ); // 1
    TRICE16_2( Id(44341), "tst:TRICE16 %d %d\n", 1, 2 ); // 2
    TRICE16_3( Id(28620), "tst:TRICE16 %d %d %d\n", 1, 2, 3 ); // 3
    TRICE16_4( Id(43473), "tst:TRICE16 %d %d %d %d\n", 1, 2, 3, 4 ); // 4
    // 10 subtraces

    TRICE32_1( Id(23722), "tst:TRICE32 %d\n", 1 ); // 2
    TRICE32_2( Id(31973), "tst:TRICE32 %d %d\n", 1, 2 ); // 4
    TRICE32_3( Id(31973), "tst:TRICE32 %d %d %d\n", 1, 2, 3 ); // 6
    TRICE32_4( Id(31973), "tst:TRICE32 %d %d %d %d\n", 1, 2, 3, 4 ); // 8
    // 20 subtraces
    
    TRICE64_1( Id(15177), "tst:TRICE64 %d\n", 1 ); // 4
    TRICE64_2( Id(56737), "tst:TRICE64 %d %d\n", 1, 2 ); // 8
    // 12 subtraces
    
    TRICE16_1( Id(33281), "ERR:error       message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id( 3505), "WRN:warning     message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(32166), "ATT:attension   message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(63852), "DIA:diagnostics message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(55914), "TIM:timing      message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(58892), "DBG:debug       message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(15483), "SIG:signal      message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(12965), "RD_:read        message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id( 2082), "WR_:write       message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(19036), "TST:test        message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(60603), "MSG:normal      message, SysTick is %d\n", SYSTICKVAL16 ); // 1

    TRICE16_1( Id(27740), "err:error       message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(31710), "wrn:warning     message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id( 7075), "att:attension   message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(59999), "dia:diagnostics message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(54975), "tim:timing      message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(22076), "dbg:debug       message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(64486), "sig:signal      message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(35724), "rd_:read        message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(56849), "wr_:write       message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(24657), "tst:test        message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(52618), "msg:normal      message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    // 22 subtraces
    
    TRICE0 (Id( 9866), "wrn:A" );
    TRICE0 (Id(45246), "wr_:B" );
    TRICE0 (Id(61952), "tim:C\n" );
    TRICE0 (Id(43853), "--------------------------------------------------------------------------------------------------------------------------------------------------\n" );
    // 4 subtraces
} // sum 111 sub traces (need 444 bytes Buffet space)

