/*! \file triceCheck.c
\brief trices for tool evaluation
\details The trices are dumped as 32bit values into a 32 bit wide fifo.
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/

#define TRICE_LEVEL 100
#include "trice.h"
#include "xteaCrypto.h"

#define SYSTICKVAL16 SysTick->VAL //!< STM32 specific

/*! write out all types of trices with fixed values for testing
\details One trice has one subtrace, if param size max 2 bytes. 
Traces with more bytes as parameter consist of several subtraces.
111 subtraces need 444 byte buffer space
*/
void triceCheckSet( void )
{
    #if 0 //def TRICE_PRINTF_ADAPTER
    char* w ="world";
    tricePrintfAdapter( "Hello %s!\n", w );
    TRICE8_1( Id(51183), "%#b times\n", 5 );
    TRICE8_1( Id( 2863), "%b times\n", 5 );
    #endif
    TRICE16_1( Id(46097), "dbg:12345 as 16bit is %#016b\n", 12345 );
    TRICE0 (Id(30265), "--------------------------------------------------------------------------------------------------------------------------------------------------\n" );
    // 1 subtrace

    TRICE8_4( Id( 1750), "tst:TRICE8  %%03x ->  %03x  %03x  %03x  %03x\n", 1, 0x7f, 0x80, 0xff ); // 4
    TRICE8_4( Id(65426), "tst:TRICE8   %%4d -> %4d %4d %4d %4d\n", 1, 0x7f, 0x80, 0xff ); // 4
    TRICE8_4( Id(16582), "tst:TRICE8   %%4o -> %4o %4o %4o %4o\n", 1, 0x7f, 0x80, 0xff ); // 4
    // 3 * 2 subtraces

    TRICE16_4( Id(56005), "tst:TRICE16  %%05x ->   %05x   %05x   %05x   %05x\n", 1, 0x7fff, 0x8000, 0xffff ); // 4
    TRICE16_4( Id(32537), "tst:TRICE16   %%6d ->  %6d  %6d  %6d  %6d\n", 1, 0x7fff, 0x8000, 0xffff ); // 4
    TRICE16_4( Id(25198), "tst:TRICE16   %%7o -> %7o %7o %7o %7o\n", 1, 0x7fff, 0x8000, 0xffff ); // 4
    // 3 * 4 subtraces

    TRICE32_4( Id(13054), "tst:TRICE32_4 %%09x ->      %09x      %09x       %09x     %09x\n", 1, 0x7fffffff, 0x80000000, 0xffffffff ); // 4
    TRICE32_4( Id(  416), "tst:TRICE32_4 %%10d ->     %10d     %10d     %10d    %10x\n",      1, 0x7fffffff, 0x80000000, 0xffffffff ); // 4
    TRICE64_1( Id(51060), "att:64bit %#b\n", 0x1122334455667788ull );

    TRICE8_1( Id(38612), "tst:TRICE8 %d\n", -1 ); // 1
    TRICE8_2( Id(44766), "tst:TRICE8 %d %d\n", -1, -2 ); // 1
    TRICE8_3( Id(56466), "tst:TRICE8 %d %d %d\n", -1, -2, -3 ); // 2
    TRICE8_4( Id(25269), "tst:TRICE8 %d %d %d %d\n", -1, -2, -3, -4 ); // 2
    TRICE8_5( Id(29229), "tst:TRICE8 %d %d %d %d %d\n", -1, -2, -3, -4, -5 ); // 3
    TRICE8_6( Id(54673), "tst:TRICE8 %d %d %d %d %d %d \n", -1, -2, -3, -4, -5, -6 ); // 3
    TRICE8_7( Id(39003), "tst:TRICE8 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 ); // 4
    TRICE8_8( Id(17307), "tst:TRICE8 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 ); // 4
    // 18 subtraces
    
    TRICE16_1( Id(11441), "tst:TRICE16 %d\n", -1 ); // 1
    TRICE16_2( Id( 7354), "tst:TRICE16 %d %d\n", -1, -2 ); // 2
    TRICE16_3( Id(32565), "tst:TRICE16 %d %d %d\n", -1, -2, -3 ); // 3
    TRICE16_4( Id(18329), "tst:TRICE16 %d %d %d %d\n", -1, -2, -3, -4 ); // 4
    // 10 subtraces

    TRICE32_1( Id(21762), "tst:TRICE32 %d\n", -1 ); // 2
    TRICE32_2( Id(41764), "tst:TRICE32 %d %d\n", -1, -2 ); // 4
    TRICE32_3( Id( 6435), "tst:TRICE32 %d %d %d\n", -1, -2, -3 ); // 4
    TRICE32_4( Id( 4527), "tst:TRICE32 %d %d %d %d\n", -1, -2, -3, -4 ); // 4
    // 6 subtraces

    TRICE64_1( Id(39599), "tst:TRICE64 %d\n", -1 ); // 2
    TRICE64_2( Id( 5982), "tst:TRICE64 %d %d\n", -1, -2 ); // 4

    TRICE16_1( Id(47902), "ERR:error       message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(64631), "WRN:warning     message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(13824), "ATT:attension   message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(64166), "DIA:diagnostics message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(15655), "TIM:timing      message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(29332), "DBG:debug       message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(52913), "SIG:signal      message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(38264), "RD_:read        message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(33248), "WR_:write       message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(49476), "TST:test        message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(36947), "MSG:normal      message, SysTick is %d\n", SYSTICKVAL16 ); // 1

    TRICE16_1( Id(56814), "err:error       message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(50334), "wrn:warning     message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(17612), "att:attension   message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id( 6482), "dia:diagnostics message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(54963), "tim:timing      message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(29704), "dbg:debug       message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(42976), "sig:signal      message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(51711), "rd_:read        message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(57672), "wr_:write       message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(13378), "tst:test        message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    TRICE16_1( Id(48476), "msg:normal      message, SysTick is %d\n", SYSTICKVAL16 ); // 1
    // 22 subtraces

    TRICE0 (Id(12664), "wrn:A" );
    TRICE0 (Id(60989), "wr_:B" );
    TRICE0 (Id(61533), "tim:C\n" );
/*
    triceString( 18, "\n" );
    triceString( 18, "1\n" );
    triceString( 18, "12\n" );
    triceString( 18, "123\n" );
    triceString( 18, "1234\n" );
    triceString( 18, "12345\n" );
    triceString( 18, "123456\n" );
    triceString( 18, "1234567\n" );
    triceString( 18, "12345678\n" );
    triceString( 18, "123456789\n" );
    triceString( 18, "123456789a\n" );
    triceString( 18, "123456789ab\n" );
    triceString( 18, "123456789abc\n" );
    triceString( 18, "Berlin\n" );
    triceString( 18, "HonululuTown\n" );
    triceString( 18, "Leipzig\n" );
*/
#ifdef ENCRYPT
    {
        uint8_t b[8] = {1,2,3,4,5,6,7,8};
        TRICE8_8( Id(41421), "msg:1:%03x %03x %03x %03x %03x %03x %03x %03x\n", b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7] );
        TRICE16_1( Id(51627), "tim: pre encryption SysTick=%d\n", SYSTICKVAL16 );
        encrypt(b);
        TRICE16_1( Id(22987), "tim: post encryption SysTick=%d\n", SYSTICKVAL16 );
        TRICE8_8( Id(63901), "att:1:%03x %03x %03x %03x %03x %03x %03x %03x\n", b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7] );
        TRICE16_1( Id(11300), "tim: pre decryption SysTick=%d\n", SYSTICKVAL16 );
        decrypt(b);
        TRICE16_1( Id(16185), "tim: post decryption SysTick=%d\n", SYSTICKVAL16 );
        TRICE8_8( Id(43598), "msg:2:%03x %03x %03x %03x %03x %03x %03x %03x\n", b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7] );
    }
#endif
    TRICE0 (Id(58460), "--------------------------------------------------------------------------------------------------------------------------------------------------\n" );
    // 4 subtraces
} // sum 111 sub traces (need 444 bytes Buffet space)
