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
    TL0 (Id(0), "--------------------------------------------------------------------------------------------------------------------------------------------------\n" );
    // 1 subtrace

    TL8_4( Id(0), "tst:TL8  %%03x ->  %03x  %03x  %03x  %03x\n", 1, 0x7f, 0x80, 0xff ); // 4
    TL8_4( Id(0), "tst:TL8   %%4d -> %4d %4d %4d %4d\n", 1, 0x7f, 0x80, 0xff ); // 4
    TL8_4( Id(0), "tst:TL8   %%4o -> %4o %4o %4o %4o\n", 1, 0x7f, 0x80, 0xff ); // 4
    // 3 * 2 subtraces

    TL16_4( Id(0), "tst:TL16  %%05x ->   %05x   %05x   %05x   %05x\n", 1, 0x7fff, 0x8000, 0xffff ); // 4
    TL16_4( Id(0), "tst:TL16   %%6d ->  %6d  %6d  %6d  %6d\n", 1, 0x7fff, 0x8000, 0xffff ); // 4
    TL16_4( Id(0), "tst:TL16   %%7o -> %7o %7o %7o %7o\n", 1, 0x7fff, 0x8000, 0xffff ); // 4
    // 3 * 4 subtraces

    TL32_2( Id(0), "tst:TL32   %%09x ->     %09x     %09x", 1, 0x7fffffff ); // 4
    TL32_2( Id(0), "tst:     %09x     %09x\n", 0x80000000, 0xffffffff ); // 4
    TL32_2( Id(0), "tst:TL32   %%11d ->   %11d   %11d", 1, 0x7fffffff ); // 4
    TL32_2( Id(0), "tst:   %11d   %11d\n", 0x80000000, 0xffffffff  ); // 4
    TL32_2( Id(0), "tst:TL32   %%12o ->  %12o  %12o", 1, 0x7fffffff ); // 4
    TL32_2( Id(0), "tst:  %12o  %12o\n", 0x80000000, 0xffffffff ); // 4
    // 6 * 4 subtraces

    TL8_1( Id(0), "tst:TL8 %d\n", 1 ); // 1
    TL8_2( Id(0), "tst:TL8 %d %d\n", 1, 2 ); // 1
    TL8_3( Id(0), "tst:TL8 %d %d %d\n", 1, 2, 3 ); // 2
    TL8_4( Id(0), "tst:TL8 %d %d %d %d\n", 1, 2, 3, 4 ); // 2
    TL8_5( Id(0), "tst:TL8 %d %d %d %d %d\n", 1, 2, 3, 4, 5 ); // 3
    TL8_6( Id(0), "tst:TL8 %d %d %d %d %d %d \n", 1, 2, 3, 4, 5, 6 ); // 3
    TL8_7( Id(0), "tst:TL8 %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7 ); // 4
    TL8_8( Id(0), "tst:TL8 %d %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7, 8 ); // 4
    // 18 subtraces
    
    TL16_1( Id(0), "tst:TL16 %d\n", 1 ); // 1
    TL16_2( Id(0), "tst:TL16 %d %d\n", 1, 2 ); // 2
    TL16_3( Id(0), "tst:TL16 %d %d %d\n", 1, 2, 3 ); // 3
    TL16_4( Id(0), "tst:TL16 %d %d %d %d\n", 1, 2, 3, 4 ); // 4
    // 10 subtraces

    TL32_1( Id(0), "tst:TL32 %d\n", 1 ); // 2
    TL32_2( Id(0), "tst:TL32 %d %d\n", 1, 2 ); // 4
    // 6 subtraces
    
    TL16_1( Id(0), "ERR:error       message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TL16_1( Id(0), "WRN:warning     message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TL16_1( Id(0), "ATT:attension   message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TL16_1( Id(0), "DIA:diagnostics message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TL16_1( Id(0), "TIM:timing      message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TL16_1( Id(0), "DBG:debug       message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TL16_1( Id(0), "SIG:signal      message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TL16_1( Id(0), "RD_:read        message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TL16_1( Id(0), "WR_:write       message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TL16_1( Id(0), "TST:test        message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TL16_1( Id(0), "MSG:normal      message, SysTick is %d\n", SYSTICKVAL16 ); // 1

    TL16_1( Id(0), "err:error       message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TL16_1( Id(0), "wrn:warning     message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TL16_1( Id(0), "att:attension   message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TL16_1( Id(0), "dia:diagnostics message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TL16_1( Id(0), "tim:timing      message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TL16_1( Id(0), "dbg:debug       message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TL16_1( Id(0), "sig:signal      message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TL16_1( Id(0), "rd_:read        message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TL16_1( Id(0), "wr_:write       message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TL16_1( Id(0), "tst:test        message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TL16_1( Id(0), "msg:normal      message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    // 22 subtraces
    
    TL0 (Id(0), "wrn:A" );
    TL0 (Id(0), "wr_:B" );
    TL0 (Id(0), "tim:C\n" );
    TL0 (Id(0), "--------------------------------------------------------------------------------------------------------------------------------------------------\n" );
    // 4 subtraces
} // sum 111 sub traces (need 444 bytes Buffet space)

