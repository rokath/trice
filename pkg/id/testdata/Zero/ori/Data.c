/*! \file triceCheck.c
\brief trices for tool evaluation
\details The trices are dumped as 32bit values into a 32 bit wide fifo.
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/

//#define TRICE_OFF
#include "trice.h"

#define SYSTICKVAL16 SysTick->VAL //!< STM32 specific

/*! write out all types of logs with fixed values for testing
\details One trice has one subtrace, if param size max 2 bytes. 
Traces with more bytes as parameter consist of several subtraces.
111 subtraces need 444 byte buffer space
*/
void triceCheckSet( void )
{
    TL0 (Id(65261), "--------------------------------------------------------------------------------------------------------------------------------------------------\n" );
    // 1 subtrace

    TL8_4( Id(51789), "tst:TL8  %%03x ->  %03x  %03x  %03x  %03x\n", 1, 0x7f, 0x80, 0xff ); // 4
    TL8_4( Id(30364), "tst:TL8   %%4d -> %4d %4d %4d %4d\n", 1, 0x7f, 0x80, 0xff ); // 4
    TL8_4( Id( 4648), "tst:TL8   %%4o -> %4o %4o %4o %4o\n", 1, 0x7f, 0x80, 0xff ); // 4
    // 3 * 2 subtraces

    TL16_4( Id(36667), "tst:TL16  %%05x ->   %05x   %05x   %05x   %05x\n", 1, 0x7fff, 0x8000, 0xffff ); // 4
    TL16_4( Id(39908), "tst:TL16   %%6d ->  %6d  %6d  %6d  %6d\n", 1, 0x7fff, 0x8000, 0xffff ); // 4
    TL16_4( Id(60212), "tst:TL16   %%7o -> %7o %7o %7o %7o\n", 1, 0x7fff, 0x8000, 0xffff ); // 4
    // 3 * 4 subtraces

    TL32_2( Id(60059), "tst:TL32   %%09x ->     %09x     %09x", 1, 0x7fffffff ); // 4
    TL32_2( Id(50464), "tst:     %09x     %09x\n", 0x80000000, 0xffffffff ); // 4
    TL32_2( Id(30279), "tst:TL32   %%11d ->   %11d   %11d", 1, 0x7fffffff ); // 4
    TL32_2( Id(40670), "tst:   %11d   %11d\n", 0x80000000, 0xffffffff  ); // 4
    TL32_2( Id(40127), "tst:TL32   %%12o ->  %12o  %12o", 1, 0x7fffffff ); // 4
    TL32_2( Id( 4479), "tst:  %12o  %12o\n", 0x80000000, 0xffffffff ); // 4
    // 6 * 4 subtraces

    TL8_1( Id(37890), "tst:TL8 %d\n", 1 ); // 1
    TL8_2( Id(29394), "tst:TL8 %d %d\n", 1, 2 ); // 1
    TL8_3( Id(28923), "tst:TL8 %d %d %d\n", 1, 2, 3 ); // 2
    TL8_4( Id(38460), "tst:TL8 %d %d %d %d\n", 1, 2, 3, 4 ); // 2
    TL8_5( Id(52346), "tst:TL8 %d %d %d %d %d\n", 1, 2, 3, 4, 5 ); // 3
    TL8_6( Id( 8503), "tst:TL8 %d %d %d %d %d %d \n", 1, 2, 3, 4, 5, 6 ); // 3
    TL8_7( Id( 6807), "tst:TL8 %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7 ); // 4
    TL8_8( Id(51953), "tst:TL8 %d %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7, 8 ); // 4
    // 18 subtraces
    
    TL16_1( Id(18564), "tst:TL16 %d\n", 1 ); // 1
    TL16_2( Id(44341), "tst:TL16 %d %d\n", 1, 2 ); // 2
    TL16_3( Id(28620), "tst:TL16 %d %d %d\n", 1, 2, 3 ); // 3
    TL16_4( Id(43473), "tst:TL16 %d %d %d %d\n", 1, 2, 3, 4 ); // 4
    // 10 subtraces

    TL32_1( Id(23722), "tst:TL32 %d\n", 1 ); // 2
    TL32_2( Id(31973), "tst:TL32 %d %d\n", 1, 2 ); // 4
    // 6 subtraces
    
    TL16_1( Id(33281), "ERR:error       message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TL16_1( Id( 3505), "WRN:warning     message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TL16_1( Id(32166), "ATT:attension   message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TL16_1( Id(63852), "DIA:diagnostics message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TL16_1( Id(55914), "TIM:timing      message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TL16_1( Id(58892), "DBG:debug       message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TL16_1( Id(15483), "SIG:signal      message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TL16_1( Id(12965), "RD_:read        message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TL16_1( Id( 2082), "WR_:write       message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TL16_1( Id(19036), "TST:test        message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TL16_1( Id(60603), "MSG:normal      message, SysTick is %d\n", SYSTICKVAL16 ); // 1

    TL16_1( Id(27740), "err:error       message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TL16_1( Id(31710), "wrn:warning     message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TL16_1( Id( 7075), "att:attension   message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TL16_1( Id(59999), "dia:diagnostics message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TL16_1( Id(54975), "tim:timing      message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TL16_1( Id(22076), "dbg:debug       message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TL16_1( Id(64486), "sig:signal      message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TL16_1( Id(35724), "rd_:read        message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TL16_1( Id(56849), "wr_:write       message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TL16_1( Id(24657), "tst:test        message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TL16_1( Id(52618), "msg:normal      message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    // 22 subtraces
    
    TL0 (Id( 9866), "wrn:A" );
    TL0 (Id(45246), "wr_:B" );
    TL0 (Id(61952), "tim:C\n" );
    TL0 (Id(43853), "--------------------------------------------------------------------------------------------------------------------------------------------------\n" );
    // 4 subtraces
} // sum 111 sub traces (need 444 bytes Buffet space)

