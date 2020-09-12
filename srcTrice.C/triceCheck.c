/*! \file triceCheck.c
\brief trices for tool evaluation
\details The trices are dumped as 32bit values into a 32 bit wide fifo.
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/
#define TRICE_FILENAME TRICE0( Id(59283), "rd_:triceCheck.c" );
//#define TRICE_CODE NO_CODE // enable this line to disable trice code generation in this file object
#include "bare.h"


#ifndef SYSTICKVAL16
#define SYSTICKVAL16 0 //!< sys clock state
#endif

//! write out all types of trices with fixed values for testing
//! \details One trice has one subtrace, if param size max 2 bytes. 
//! Traces with more bytes as parameter consist of several subtraces.
void triceCheckSet( int index )
{
    switch(index){
        case 0:
            TRICE0 (Id(24481), "--------------------------------------------------\r\n" );
            //TRICE_P( "Hello %s, this is a %dst printf replacement.\n", "world", 1 ); // visible only with defined TRICE_PRINTF_ADAPTER
            TRICE0 (Id(13428), "--------------------------------------------------\r\n" );
            TRICE16_1( Id(18133), "dbg:12345 as 16bit is %#016b\n", 12345 );
            TRICE0 (Id( 3791), "--------------------------------------------------\r\n" );
            //TRICE_ASSERT(0==0);
            TRICE0( Id(17125), "sig:This ASSERT error is just a demo and no real error:\n");
            //TRICE_ASSERT(0==1);
            TRICE0 (Id(26797), "--------------------------------------------------\r\n" );
        break;
        case 1:
            TRICE16_1( Id(18113), "ERR:error       message, SysTick is %6d\n", SYSTICKVAL16 );
            TRICE16_1( Id(28289), "WRN:warning     message, SysTick is %6d\n", SYSTICKVAL16 );
            TRICE16_1( Id(53560), "ATT:attension   message, SysTick is %6d\n", SYSTICKVAL16 );
            TRICE16_1( Id(16672), "DIA:diagnostics message, SysTick is %6d\n", SYSTICKVAL16 );
            TRICE16_1( Id(42206), "TIM:timing      message, SysTick is %6d\n", SYSTICKVAL16 );
            TRICE16_1( Id(23973), "DBG:debug       message, SysTick is %6d\n", SYSTICKVAL16 );
            TRICE16_1( Id(13730), "SIG:signal      message, SysTick is %6d\n", SYSTICKVAL16 );
            TRICE16_1( Id(24302), "RD:read         message, SysTick is %6d\n", SYSTICKVAL16 );
            TRICE16_1( Id(14138), "WR:write        message, SysTick is %6d\n", SYSTICKVAL16 );
            TRICE16_1( Id(55445), "ISR:interrupt   message, SysTick is %6d\n", SYSTICKVAL16 );
            TRICE16_1( Id(41495), "TST:test        message, SysTick is %6d\n", SYSTICKVAL16 );
            TRICE16_1( Id(45697), "MSG:normal      message, SysTick is %6d\n", SYSTICKVAL16 );
            TRICE16_1( Id(24589), "INFO:informal   message, SysTick is %6d\n", SYSTICKVAL16 );
        break;
        case 2:
            TRICE8_4( Id( 2527), "tst:TRICE8  %%03x ->  %03x  %03x  %03x  %03x\n", 1, 0x7f, 0x80, 0xff );
            TRICE8_4( Id(63423), "tst:TRICE8   %%4d -> %4d %4d %4d %4d\n", 1, 0x7f, 0x80, 0xff );
            TRICE8_4( Id(40063), "tst:TRICE8   %%4o -> %4o %4o %4o %4o\n", 1, 0x7f, 0x80, 0xff );

            TRICE16_4( Id( 8674), "tst:TRICE16  %%05x ->   %05x   %05x   %05x   %05x\n", 1, 0x7fff, 0x8000, 0xffff );
            TRICE16_4( Id(59869), "tst:TRICE16   %%6d ->  %6d  %6d  %6d  %6d\n", 1, 0x7fff, 0x8000, 0xffff );
            TRICE16_4( Id(24781), "tst:TRICE16   %%7o -> %7o %7o %7o %7o\n", 1, 0x7fff, 0x8000, 0xffff );

            TRICE32_4( Id(16811), "tst:TRICE32_4 %%09x ->      %09x      %09x       %09x     %09x\n", 1, 0x7fffffff, 0x80000000, 0xffffffff ); // 4
            TRICE32_4( Id(28944), "tst:TRICE32_4 %%10d ->     %10d     %10d     %10d    %10x\n",      1, 0x7fffffff, 0x80000000, 0xffffffff ); // 4
            TRICE64_1( Id(19049), "att:64bit %#b\n", 0x1122334455667788ull );
        break;
        case 3:
            TRICE8_1( Id(41267), "tst:TRICE8_1 %d\n", -1 );
            TRICE8_2( Id(28873), "tst:TRICE8_2 %d %d\n", -1, -2 );
            TRICE8_3( Id(18501), "tst:TRICE8_3 %d %d %d\n", -1, -2, -3 );
            TRICE8_4( Id(34892), "tst:TRICE8_4 %d %d %d %d\n", -1, -2, -3, -4 );
            TRICE8_5( Id( 2043), "tst:TRICE8_5 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
            TRICE8_6( Id(36242), "tst:TRICE8_6 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE8_7( Id(23187), "tst:TRICE8_7 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
            TRICE8_8( Id( 5279), "tst:TRICE8_8 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 4:
            TRICE16_1( Id(50977), "tst:TRICE16_1 %d\n", -1 ); // 1
            TRICE16_2( Id(65136), "tst:TRICE16_2 %d %d\n", -1, -2 ); // 2
            TRICE16_3( Id(58915), "tst:TRICE16_3 %d %d %d\n", -1, -2, -3 ); // 3
            TRICE16_4( Id(45117), "tst:TRICE16_4 %d %d %d %d\n", -1, -2, -3, -4 ); // 4
        break;
        case 5:
            TRICE32_1( Id( 6384), "tst:TRICE32_1 %d\n", -1 ); // 2
            TRICE32_2( Id(11505), "tst:TRICE32_2 %d %d\n", -1, -2 ); // 4
            TRICE32_3( Id(56189), "tst:TRICE32_3 %d %d %d\n", -1, -2, -3 ); // 4
            TRICE32_4( Id(36573), "tst:TRICE32_4 %d %d %d %d\n", -1, -2, -3, -4 ); // 4
        break;
        case 6:
            TRICE64_1( Id(43171), "tst:TRICE64_1 %d\n", -1 ); // 2
            TRICE64_2( Id( 8402), "tst:TRICE64_2 %d %d\n", -1, -2 ); // 4
        break;
        case 7:
            TRICE0(Id(44690), "e:A" );
            TRICE0(Id( 1471), "w:B" );
            TRICE0(Id(52563), "a:c" );
            TRICE0(Id(61604), "wr:d" );
            TRICE0(Id(39587), "rd:e\n" );
            TRICE0(Id(53436), "diag:f" );
            TRICE0(Id(32678), "d:G" );
            TRICE0(Id(13587), "t:H" );
            TRICE0(Id( 8059), "time:i" );
            TRICE0(Id(26357), "message:J" );
            TRICE0(Id( 2812), "dbg:k\n" );
        break;
        case 8:
            TRICE0( Id(43782), "1" );
            TRICE0( Id(31945), "2" );
            TRICE0( Id(57633), "3" );
            TRICE0( Id(32117), "4" );
            TRICE0( Id( 3996), "e:7" );
            TRICE0( Id( 6559), "m:12" );
            TRICE0( Id(30479), "m:123\n" );
        break;
        case 9:
            TRICE16_1( Id(41878), "err:error       message, SysTick is %6d\n", SYSTICKVAL16 );
            TRICE16_1( Id( 3242), "wrn:warning     message, SysTick is %6d\n", SYSTICKVAL16 );
            TRICE16_1( Id(10588), "att:attension   message, SysTick is %6d\n", SYSTICKVAL16 );
            TRICE16_1( Id(15045), "dia:diagnostics message, SysTick is %6d\n", SYSTICKVAL16 );
            TRICE16_1( Id(20195), "tim:timing      message, SysTick is %6d\n", SYSTICKVAL16 );
            TRICE16_1( Id(56298), "dbg:debug       message, SysTick is %6d\n", SYSTICKVAL16 );
            TRICE16_1( Id(62837), "sig:signal      message, SysTick is %6d\n", SYSTICKVAL16 );
            TRICE16_1( Id(35517), "rd_:read        message, SysTick is %6d\n", SYSTICKVAL16 );
            TRICE16_1( Id(47873), "wr:write        message, SysTick is %6d\n", SYSTICKVAL16 );
            TRICE16_1( Id(64995), "isr:interrupt   message, SysTick is %6d\n", SYSTICKVAL16 );
            TRICE16_1( Id(42962), "tst:test        message, SysTick is %6d\n", SYSTICKVAL16 );
            TRICE16_1( Id(59159), "msg:normal      message, SysTick is %6d\n", SYSTICKVAL16 );
            TRICE16_1( Id(48090), "info:informal   message, SysTick is %6d\n", SYSTICKVAL16 );
        break;
        case 10:
            TRICE0 (Id(55237), "e:A" );
            TRICE0 (Id(38195), "w:B" );
            TRICE0 (Id(43063), "a:c" );
            TRICE0 (Id( 6176), "wr:d" );
            TRICE0 (Id( 6665), "rd:e" );
            TRICE0 (Id(47959), "diag:y" );
            TRICE0 (Id(48390), "d:G" );
            TRICE0 (Id(64481), "t:H" );
            TRICE0 (Id(19868), "time:i" );
            TRICE0 (Id( 9207), "message:J" );
            TRICE0 (Id(29302), "inf:k\n" );
        break;
				/*
        case 11:
            TRICE0( Id(21100), "att:Next few lines have no color and visible only when TRICE_RUNTIME_GENERATED_STRINGS_SUPPORT is defined\n" );
            TRICE_S( 18, "\n" );
            TRICE_S( 18, "1\n" );
            TRICE_S( 18, "12\n" );
            TRICE_S( 18, "123\n" );
            TRICE_S( 18, "1234\n" );
            TRICE_S( 18, "12345\n" );
            TRICE_S( 18, "123456\n" );
            TRICE_S( 18, "1234567\n" );
            TRICE_S( 18, "12345678\n" );
            TRICE_S( 18, "123456789\n" );
        break;
        case 12:
            TRICE_S( 18, "123456789a\n" );
            TRICE_S( 18, "123456789ab\n" );
            TRICE_S( 18, "123456789abc\n" );
            TRICE_S( 18, "123456789abcd\n" );
            TRICE_S( 18, "123456789abcde\n" );
        break;
        case 13:
            TRICE_S( 18, "123456789abcdef\n" );
            TRICE_S( 18, "123456789abcdef7\n" );
            TRICE_S( 18, "123456789abcdef78\n" );
            TRICE_S( 18, "123456789abcdef789\n" );
            TRICE_S( 18, "123456789abcdef7890\n" );
        break;
				*/
        case 14:
        #ifdef ENCRYPT
            {
                uint8_t b[8] = {1,2,3,4,5,6,7,8};
                TRICE8_8( Id( 2502), "msg:1:%03x %03x %03x %03x %03x %03x %03x %03x\n", b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7] );
                TRICE16_1( Id(20625), "tim: pre encryption SysTick=%d\n", SYSTICKVAL16 );
                encrypt(b);
                TRICE16_1( Id(64368), "tim: post encryption SysTick=%d\n", SYSTICKVAL16 );
                TRICE8_8( Id(43335), "att:1:%03x %03x %03x %03x %03x %03x %03x %03x\n", b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7] );
                TRICE16_1( Id(56697), "tim: pre decryption SysTick=%d\n", SYSTICKVAL16 );
                decrypt(b);
                TRICE16_1( Id(50168), "tim: post decryption SysTick=%d\n", SYSTICKVAL16 );
                TRICE8_8( Id(  990), "msg:2:%03x %03x %03x %03x %03x %03x %03x %03x\n", b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7] );
            }
            TRICE0 (Id( 7275), "--------------------------------------------------\r\n\n" );
        #endif
        break;
        default:
        break;
    }
}
