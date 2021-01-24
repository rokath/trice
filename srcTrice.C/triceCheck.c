/*! \file triceCheck.c
\brief trices for tool evaluation
\details The trices are dumped as 32bit values into a 32 bit wide fifo.
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/
#define TRICE_FILENAME TRICE0( Id(59283), "rd_:triceCheck.c" );
//#define TRICE_CODE NO_CODE // enable this line to disable trice code generation in this file object
#include "trice.h"

#include "main.h" // __get_PRIMASK

///////////////////////////////////////////////////////////////////////////////
// trice time measurement
//

#ifndef TRICE_S
#define TRICE_S( i, p, s )
#endif


//! triceRuntimeGeneratedStringUnbound can transfer runtime generated strings if TRICES_1 is not available.
TRICE_INLINE void triceRuntimeGeneratedStringUnbound( const char* s ){
    size_t len = strlen( s );
    char c1, c2, c3, c4, c5, c6, c7, c8;
    while( len ){
        switch( len ){
            case  0: return;
            case  1: c1=*s++;
                TRICE8_1( Id(36152), "%c", c1 ); return;
            case  2: c1=*s++; c2=*s++;
                TRICE8_2( Id(49862), "%c%c", c1, c2 ); return;
            case  3: c1=*s++; c2=*s++; c3=*s++;
                TRICE8_3( Id(60898), "%c%c%c", c1, c2, c3 ); return;
            case  4: c1=*s++; c2=*s++; c3=*s++; c4=*s++;
                TRICE8_4( Id(57970), "%c%c%c%c", c1, c2, c3, c4 ); return;
            case  5: c1=*s++; c2=*s++; c3=*s++; c4=*s++; c5=*s++;
                TRICE8_5( Id(49813), "%c%c%c%c%c", c1, c2, c3, c4, c5 ); return;
            case  6: c1=*s++; c2=*s++; c3=*s++; c4=*s++; c5=*s++; c6=*s++;
                TRICE8_6( Id(10201), "%c%c%c%c%c%c", c1, c2, c3, c4, c5, c6 ); return;
            case  7: c1=*s++; c2=*s++; c3=*s++; c4=*s++; c5=*s++; c6=*s++; c7=*s++;
                TRICE8_7( Id(57439), "%c%c%c%c%c%c%c", c1, c2, c3, c4, c5, c6, c7); return;
            case  8:
            default: c1 = *s++; c2 = *s++; c3 = *s++; c4 = *s++; c5 = *s++; c6 = *s++; c7 = *s++; c8 = *s++;
                TRICE8_8( Id(53018), "%c%c%c%c%c%c%c%c", c1, c2, c3, c4, c5, c6, c7, c8 );
                len -= 8;
        }
    }
    return;
}

//! trice runtime string
#define TRICE_RTS(dynString) do{ triceRuntimeGeneratedStringUnbound(dynString); }while(0)

// nextRuntimeString returns a in length changing string from 0 to 250 bytes.
char* nextRuntimeString( int length ){
    static char rts[300] = {0};
    for( int i = 0; i < length; i++ ){
       char c = 0x7f & (0x20 + i);
        c = c < 0x20 ? c + 0x20 : c; // exclude control characters
        c = '`' == c ? 0x20 : c; // exclude back tick for easy testTable generation
        c = 127 == c ? 0x20 : c; // exclude DEL control character
        rts[i] = c;
    }
    rts[length] = 0;
    return rts;
}

// triceRuntimeStrings sends n stings to the trice port.
void triceRuntimeStrings( int from, int limit){
    for( int i = from; i < limit; i++ ){
        char* rts =  nextRuntimeString(i);
        TRICE_S( Id(   99), "%s\n", rts );
    }
}

//! write out all types of trices with fixed values for testing
//! \details One trice has one subtrace, if param size max 2 bytes. 
//! Traces with more bytes as parameter consist of several subtraces.
void triceCheckSetTime(int index) {
    switch (index) {
        case 0:
            TRICE0(Id(24481), "--------------------------------------------------\n");
            TRICE0(Id(24481), "--------------------------------------------------\n");
            TRICE16_1(Id(18133), "dbg:12345 as 16bit is %#016b\n", 12345);
            TRICE0(Id(24481), "--------------------------------------------------\n");
            TRICE0(Id(17125), "sig:This ASSERT error is just a demo and no real error:\n");
            TRICE0(Id(24481), "--------------------------------------------------\n");
            break;
        case 1:
            TRICE16_1(Id(18113), "ERR:error       message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE16_1(Id(28289), "WRN:warning     message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE16_1(Id(53560), "ATT:attension   message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE16_1(Id(16672), "DIA:diagnostics message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE16_1(Id(42206), "TIM:timing      message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE16_1(Id(23973), "DBG:debug       message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE16_1(Id(13730), "SIG:signal      message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE16_1(Id(24302), "RD:read         message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE16_1(Id(14138), "WR:write        message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE16_1(Id(55445), "ISR:interrupt   message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE16_1(Id(45697), "MSG:normal      message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE16_1(Id(24589), "INFO:informal   message, SysTick is %6d\n", SYSTICKVAL16);
            break;
        case 2:
            TRICE16_1(Id( 9508), "tst:TRICE16_1   message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE16_1(Id(17458), "tst:TRICE16_1   message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE16_1(Id(60292), "tst:TRICE16_1   message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE16_1(Id(18686), "tst:TRICE16_1   message, SysTick is %6d\n", SYSTICKVAL16);
            break;
        case 3:
            TRICE32_1(Id(31275), "tst:TRICE32_1   message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE32_1(Id(55224), "tst:TRICE32_1   message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE32_1(Id(22099), "tst:TRICE32_1   message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE32_1(Id(58610), "tst:TRICE32_1   message, SysTick is %6d\n", SYSTICKVAL16);
            break;
        case 4:
            TRICE8_4( Id( 3127), "tst:TRICE8_4  %%03x ->  %03x  %03x  %03x  %03x\n", 1, 0x7f, 0x80, 0xff);
            TRICE8_4( Id( 9122), "tst:TRICE8_4   %%4d -> %4d %4d %4d %4d\n", 1, 0x7f, 0x80, 0xff);
            TRICE8_4( Id(13112), "tst:TRICE8_4   %%4o -> %4o %4o %4o %4o\n", 1, 0x7f, 0x80, 0xff);
            TRICE16_4(Id(52092), "tst:TRICE16_4  %%05x ->   %05x   %05x   %05x   %05x\n", 1, 0x7fff, 0x8000, 0xffff);
            TRICE16_4(Id(44164), "tst:TRICE16_4   %%6d ->  %6d  %6d  %6d  %6d\n", 1, 0x7fff, 0x8000, 0xffff);
            TRICE16_4(Id(18812), "tst:TRICE16_4   %%7o -> %7o %7o %7o %7o\n", 1, 0x7fff, 0x8000, 0xffff);
            TRICE32_4(Id(64034), "tst:TRICE32_4 %%09x ->      %09x      %09x       %09x     %09x\n", 1, 0x7fffffff, 0x80000000, 0xffffffff); // 4
            TRICE32_4(Id(57309), "tst:TRICE32_4 %%10d ->     %10d     %10d     %10d    %10x\n", 1, 0x7fffffff, 0x80000000, 0xffffffff); // 4
            TRICE64_1(Id(11840), "att:TRICE64_1 %#b\n", 0x1122334455667788);
            break;
        case 5:
            TRICE8_1(Id(65514), "tst:TRICE8_1 %d\n", -111);
            TRICE8_2(Id(33867), "tst:TRICE8_2 %d %d\n", -111, -222);
            TRICE8_3(Id(46200), "tst:TRICE8_3 %d %d %d\n", -111, -222, -3);
            TRICE8_4(Id( 8620), "tst:TRICE8_4 %d %d %d %d\n", -111, -222, -3, -4);
            TRICE8_5(Id(52633), "tst:TRICE8_5 %d %d %d %d %d\n", -111, -222, -3, -4, -5);
            TRICE8_6(Id(50340), "tst:TRICE8_6 %d %d %d %d %d %d\n", -111, -222, -3, -4, -5, -6);
            TRICE8_7(Id(46478), "tst:TRICE8_7 %d %d %d %d %d %d %d\n", -111, -222, -3, -4, -5, -6, -7);
            TRICE8_8(Id(26742), "tst:TRICE8_8 %d %d %d %d %d %d %d %d\n", -111, -222, -3, -4, -5, -6, -7, -8);
            break;
        case 7:
            TRICE16_1(Id(26440), "tst:TRICE16_1 %d\n", -111); // 1
            TRICE16_2(Id( 2548), "tst:TRICE16_2 %d %d\n", -111, -222); // 2
            TRICE16_3(Id(32883), "tst:TRICE16_3 %d %d %d\n", -111, -222, -333); // 3
            TRICE16_4(Id(26843), "tst:TRICE16_4 %d %d %d %d\n", -111, -222, -333, -444); // 4
            break;
        case 8:
            TRICE32_1(Id( 9906), "tst:TRICE32_1 %08x\n", 0x0123cafe); // 2
            TRICE32_1(Id( 3159), "tst:TRICE32_1 %d\n", -111); // 2
            TRICE32_2(Id(31104), "tst:TRICE32_2 %x %x\n", -111, -222); // 4
            TRICE32_2(Id(39839), "tst:TRICE32_2 %d %d\n", -111, -222); // 4
            TRICE32_3(Id(19056), "tst:TRICE32_3 %x %x %x\n", -111, -222, -333); // 4
            TRICE32_3(Id(14942), "tst:TRICE32_3 %d %d %d\n", -111, -222, -333); // 4
            TRICE32_4(Id(37841), "tst:TRICE32_4 %x %x %x %x\n", -111, -222, -333, -444); // 4
            TRICE32_4(Id(39140), "tst:TRICE32_4 %d %d %d %d\n", -111, -222, -333, -444); // 4
            break;
        case 9:
            TRICE64_1(Id(29022), "tst:TRICE64_1 %d\n", -111); // 2
            TRICE64_2(Id(50677), "tst:TRICE64_2 %d %d\n", -111, -222); // 4
            break;
        case 10:
            TRICE0(Id(44690), "e:A");
            TRICE0(Id(1471), "w:B");
            TRICE0(Id(52563), "a:c");
            TRICE0(Id(61604), "wr:d");
            TRICE0(Id(39587), "rd:e\n");
            TRICE0(Id(53436), "diag:f");
            TRICE0(Id(32678), "d:G");
            TRICE0(Id(13587), "t:H");
            TRICE0(Id(8059), "time:i");
            TRICE0(Id(26357), "message:J");
            TRICE0(Id(2812), "dbg:k\n");
            break;
        case 11:
            TRICE0(Id(43782), "1");
            TRICE0(Id(31945), "2");
            TRICE0(Id(57633), "3");
            TRICE0(Id(32117), "4");
            TRICE0(Id(3996), "e:7");
            TRICE0(Id(6559), "m:12");
            TRICE0(Id(30479), "m:123\n");
            break;
        case 12:
            TRICE8_1(Id(43482), "tst:TRICE8_1 %d\n", 1);
            TRICE8_2(Id(44387), "tst:TRICE8_2 %d %d\n", 1, 2);
            TRICE8_3(Id(54384), "tst:TRICE8_3 %d %d %d\n", 1, 2, 3);
            TRICE8_4(Id(52683), "tst:TRICE8_4 %d %d %d %d\n", 1, 2, 3, 4);
            TRICE8_5(Id(15890), "tst:TRICE8_5 %d %d %d %d %d\n", 1, 2, 3, 4, 5);
            TRICE8_6(Id(12759), "tst:TRICE8_6 %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6);
            TRICE8_7(Id(29701), "tst:TRICE8_7 %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7);
            TRICE8_8(Id(52412), "tst:TRICE8_8 %d %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7, 8);
            break;
        case 13:
            TRICE_RTS( "an_example_string\n" );
            TRICE_RTS( "" );
            TRICE_RTS( "\n" );
            TRICE_RTS( "a\n" );
            TRICE_RTS( "an\n" );
            TRICE_RTS( "an_\n" );
            TRICE_RTS( "an_e\n" );
            TRICE_RTS( "an_ex\n" );
            TRICE_RTS( "an_exa\n" );
            TRICE_RTS( "an_exam\n" );
            TRICE_RTS( "an_examp\n" );
            TRICE_RTS( "an_exampl\n" );
            TRICE_RTS( "an_example\n" );
            TRICE_RTS( "an_example_\n" );
            TRICE_RTS( "an_example_s\n" );
            TRICE_RTS( "an_example_st\n" );
            TRICE_RTS( "an_example_str\n" );
            TRICE_RTS( "an_example_stri\n" );
            TRICE_RTS( "an_example_strin\n" );
            TRICE_RTS( "an_example_string\n" );
            break;
        case 14:
            triceRuntimeStrings(0, 20);
            break;
       case 15: 
			triceRuntimeStrings(30, 35 );
           break;
       case 16: 
			triceRuntimeStrings(126, 130);
           break;
       case 17: 
			triceRuntimeStrings(254, 256);
           break;
        case 18:
#ifdef ENCRYPT
        {
            uint8_t b[8] = {1,2,3,4,5,6,7,8};
            TRICE8_8(  Id(32509), "msg: message = %03x %03x %03x %03x %03x %03x %03x %03x\n", b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7] );
            TRICE16_1( Id(20625), "tim: pre encryption SysTick=%d\n", SYSTICKVAL16 );
            encrypt(b);
            TRICE16_1( Id(64368), "tim: post encryption SysTick=%d\n", SYSTICKVAL16 );
            TRICE8_8(  Id(44594), "att: encrypted = %03x %03x %03x %03x %03x %03x %03x %03x\n", b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7] );
            TRICE16_4( Id(36992), "att: encrypted = %d,%d,%d,%d,", b[0], b[1], b[2], b[3] );
            TRICE16_4( Id(30981), "att: %d,%d,%d,%d\n", b[4], b[5], b[6], b[7] );
            TRICE16_1( Id(56697), "tim: pre decryption SysTick=%d\n", SYSTICKVAL16 );
            decrypt(b);
            TRICE16_1( Id(50168), "tim: post decryption SysTick=%d\n", SYSTICKVAL16 );
            TRICE8_8(  Id(36226), "msg: messge = %03x %03x %03x %03x %03x %03x %03x %03x\n", b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7] );
        }
        TRICE0 (Id( 7275), "--------------------------------------------------\n\n" );
#endif
        break;
    default:
        break;
    }
}

#if TRICE_PACK2_ENCODING == TRICE_ENCODING || TRICE_BARE_ENCODING == TRICE_ENCODING
//! write out all types of trices with fixed values for testing
//! \details One trice has one subtrace, if param size max 2 bytes. 
//! Traces with more bytes as parameter consist of several subtraces.
void triceCheckSetSpace(int index) {
    switch (index) {
        case 0:
            trice0(Id( 7428), "--------------------------------------------------\n");
            trice0(Id(34936), "--------------------------------------------------\n");
            trice16_1(Id(53581), "dbg:12345 as 16bit is %#016b\n", 12345);
            trice0(Id(44974), "--------------------------------------------------\n");
            trice0(Id(44673), "sig:This ASSERT error is just a demo and no real error:\n");
            trice0(Id(64750), "--------------------------------------------------\n");
            break;
        case 1:
            trice16_1(Id(53601), "ERR:error       message, SysTick is %6d\n", SYSTICKVAL16);
            trice16_1(Id(63356), "WRN:warning     message, SysTick is %6d\n", SYSTICKVAL16);
            trice16_1(Id(13115), "ATT:attension   message, SysTick is %6d\n", SYSTICKVAL16);
            trice16_1(Id(34880), "DIA:diagnostics message, SysTick is %6d\n", SYSTICKVAL16);
            trice16_1(Id(28691), "TIM:timing      message, SysTick is %6d\n", SYSTICKVAL16);
            trice16_1(Id(21299), "DBG:debug       message, SysTick is %6d\n", SYSTICKVAL16);
            trice16_1(Id(21756), "SIG:signal      message, SysTick is %6d\n", SYSTICKVAL16);
            trice16_1(Id(55641), "RD:read         message, SysTick is %6d\n", SYSTICKVAL16);
            trice16_1(Id(21649), "WR:write        message, SysTick is %6d\n", SYSTICKVAL16);
            trice16_1(Id(38305), "ISR:interrupt   message, SysTick is %6d\n", SYSTICKVAL16);
            trice16_1(Id(37470), "MSG:normal      message, SysTick is %6d\n", SYSTICKVAL16);
            trice16_1(Id(63899), "INFO:informal   message, SysTick is %6d\n", SYSTICKVAL16);
            break;
        case 2:
            trice16_1(Id(35005), "tst:trice16_1   message, SysTick is %6d\n", SYSTICKVAL16);
            trice16_1(Id(11613), "tst:trice16_1   message, SysTick is %6d\n", SYSTICKVAL16);
            trice16_1(Id(40655), "tst:trice16_1   message, SysTick is %6d\n", SYSTICKVAL16);
            trice16_1(Id(59000), "tst:trice16_1   message, SysTick is %6d\n", SYSTICKVAL16);
            break;
        case 3:
            trice32_1(Id(62153), "tst:trice32_1   message, SysTick is %6d\n", SYSTICKVAL16);
            trice32_1(Id(43785), "tst:trice32_1   message, SysTick is %6d\n", SYSTICKVAL16);
            trice32_1(Id(15016), "tst:trice32_1   message, SysTick is %6d\n", SYSTICKVAL16);
            trice32_1(Id(31714), "tst:trice32_1   message, SysTick is %6d\n", SYSTICKVAL16);
            break;
        case 4:
            trice8_4( Id(  527), "tst:trice8_4  %%03x ->  %03x  %03x  %03x  %03x\n", 1, 0x7f, 0x80, 0xff);
            trice8_4( Id(40262), "tst:trice8_4   %%4d -> %4d %4d %4d %4d\n", 1, 0x7f, 0x80, 0xff);
            trice8_4( Id(35860), "tst:trice8_4   %%4o -> %4o %4o %4o %4o\n", 1, 0x7f, 0x80, 0xff);
            trice16_4(Id( 7247), "tst:trice16_4  %%05x ->   %05x   %05x   %05x   %05x\n", 1, 0x7fff, 0x8000, 0xffff);
            trice16_4(Id(34359), "tst:trice16_4   %%6d ->  %6d  %6d  %6d  %6d\n", 1, 0x7fff, 0x8000, 0xffff);
            trice16_4(Id(15533), "tst:trice16_4   %%7o -> %7o %7o %7o %7o\n", 1, 0x7fff, 0x8000, 0xffff);
            trice32_4(Id(48518), "tst:trice32_4 %%09x ->      %09x      %09x       %09x     %09x\n", 1, 0x7fffffff, 0x80000000, 0xffffffff); // 4
            trice32_4(Id(40516), "tst:trice32_4 %%10d ->     %10d     %10d     %10d    %10x\n", 1, 0x7fffffff, 0x80000000, 0xffffffff); // 4
            trice64_1(Id(49301), "att:trice64_1 %#b\n", 0x1122334455667788);
            break;
        case 5:
            trice8_1(Id( 5320), "tst:trice8_1 %d\n", -111);
            trice8_2(Id(25279), "tst:trice8_2 %d %d\n", -111, -222);
            trice8_3(Id(65490), "tst:trice8_3 %d %d %d\n", -111, -222, -3);
            trice8_4(Id(41198), "tst:trice8_4 %d %d %d %d\n", -111, -222, -3, -4);
            trice8_5(Id(43603), "tst:trice8_5 %d %d %d %d %d\n", -111, -222, -3, -4, -5);
            trice8_6(Id(39057), "tst:trice8_6 %d %d %d %d %d %d\n", -111, -222, -3, -4, -5, -6);
            trice8_7(Id(60429), "tst:trice8_7 %d %d %d %d %d %d %d\n", -111, -222, -3, -4, -5, -6, -7);
            trice8_8(Id(36581), "tst:trice8_8 %d %d %d %d %d %d %d %d\n", -111, -222, -3, -4, -5, -6, -7, -8);
            break;
        case 7:
            trice16_1(Id( 3423), "tst:trice16_1 %d\n", -111); // 1
            trice16_2(Id(19534), "tst:trice16_2 %d %d\n", -111, -222); // 2
            trice16_3(Id(64187), "tst:trice16_3 %d %d %d\n", -111, -222, -333); // 3
            trice16_4(Id( 1904), "tst:trice16_4 %d %d %d %d\n", -111, -222, -333, -444); // 4
            break;
        case 8:
            trice32_1(Id(32938), "tst:trice32_1 %d\n", -111); // 2
            trice32_2(Id(49109), "tst:trice32_2 %d %d\n", -111, -222); // 4
            trice32_3(Id(27374), "tst:trice32_3 %d %d %d\n", -111, -222, -333); // 4
            trice32_4(Id( 8882), "tst:trice32_4 %d %d %d %d\n", -111, -222, -333, -444); // 4
            trice32_1(Id(24745), "tst:trice32_1 %08x\n", 0x0123cafe); // 2
            trice32_2(Id(55058), "tst:trice32_2 %x %x\n", 0xfeedcafe, 0xcafe0123); // 4
            trice32_3(Id(29755), "tst:trice32_3 %x %x %x\n", 0xaa001122, 0xbb112233, 0xcc223344); // 4
            trice32_4(Id(53110), "tst:trice32_4 %x %x %x %x\n", 0x12345678, 0x9abcdef0, 0xfedcba98, 0x87654321); // 4
            break;
        case 9:
            trice64_1(Id(59010), "tst:trice64_1 %d\n", -111); // 2
            trice64_2(Id(59011), "tst:trice64_2 %d %d\n", -111, -222); // 4
            trice64_1(Id(60000), "tst:trice64_1 %x\n", 0x8765432112345678); // 2
            trice64_2(Id(58010), "tst:trice64_2 %x %x\n", 0x13579bdffdb97531, 0xeca8642002468ace); // 4
            break;
        case 10:
            trice0(Id( 4361), "e:A");
            trice0(Id(61148), "w:B");
            trice0(Id(51175), "a:c");
            trice0(Id(38433), "wr:d");
            trice0(Id(19819), "rd:e\n");
            trice0(Id(53400), "diag:f");
            trice0(Id(25505), "d:G");
            trice0(Id(53207), "t:H");
            trice0(Id(34163), "time:i");
            trice0(Id(35099), "message:J");
            trice0(Id(19680), "dbg:k\n");
            break;
        case 11:
            trice0(Id(44026), "1");
            trice0(Id(27771), "2");
            trice0(Id(14471), "3");
            trice0(Id( 1869), "4");
            trice0(Id(58735), "e:7");
            trice0(Id(48828), "m:12");
            trice0(Id(41422), "m:123\n");
            break;
        case 12:
            trice8_1(Id(26082), "tst:trice8_1 %d\n", 1);
            trice8_2(Id(62944), "tst:trice8_2 %d %d\n", 1, 2);
            trice8_3(Id(35237), "tst:trice8_3 %d %d %d\n", 1, 2, 3);
            trice8_4(Id( 3173), "tst:trice8_4 %d %d %d %d\n", 1, 2, 3, 4);
            trice8_5(Id(12942), "tst:trice8_5 %d %d %d %d %d\n", 1, 2, 3, 4, 5);
            trice8_6(Id(18973), "tst:trice8_6 %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6);
            trice8_7(Id(56762), "tst:trice8_7 %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7);
            trice8_8(Id(12736), "tst:trice8_8 %d %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7, 8);
            break;
        case 13:
            TRICE_RTS( "an_example_string\n" );
            TRICE_RTS( "" );
            TRICE_RTS( "\n" );
            TRICE_RTS( "a\n" );
            TRICE_RTS( "an\n" );
            TRICE_RTS( "an_\n" );
            TRICE_RTS( "an_e\n" );
            TRICE_RTS( "an_ex\n" );
            TRICE_RTS( "an_exa\n" );
            TRICE_RTS( "an_exam\n" );
            TRICE_RTS( "an_examp\n" );
            TRICE_RTS( "an_exampl\n" );
            TRICE_RTS( "an_example\n" );
            TRICE_RTS( "an_example_\n" );
            TRICE_RTS( "an_example_s\n" );
            TRICE_RTS( "an_example_st\n" );
            TRICE_RTS( "an_example_str\n" );
            TRICE_RTS( "an_example_stri\n" );
            TRICE_RTS( "an_example_strin\n" );
            TRICE_RTS( "an_example_string\n" );
            break;
        case 14:
            triceRuntimeStrings(0, 20);
            break;
       case 15: 
			triceRuntimeStrings(30, 35 );
           break;
       case 16: 
			triceRuntimeStrings(126, 130);
           break;
       case 17: 
			triceRuntimeStrings(254, 256);
           break;
        case 18:
#ifdef ENCRYPT
        {
            uint8_t b[8] = {1,2,3,4,5,6,7,8};
            TRICE8_8(  Id(32509), "msg: message = %03x %03x %03x %03x %03x %03x %03x %03x\n", b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7] );
            TRICE16_1( Id(20625), "tim: pre encryption SysTick=%d\n", SYSTICKVAL16 );
            encrypt(b);
            TRICE16_1( Id(64368), "tim: post encryption SysTick=%d\n", SYSTICKVAL16 );
            TRICE8_8(  Id(44594), "att: encrypted = %03x %03x %03x %03x %03x %03x %03x %03x\n", b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7] );
            TRICE16_4( Id(36992), "att: encrypted = %d,%d,%d,%d,", b[0], b[1], b[2], b[3] );
            TRICE16_4( Id(30981), "att: %d,%d,%d,%d\n", b[4], b[5], b[6], b[7] );
            TRICE16_1( Id(56697), "tim: pre decryption SysTick=%d\n", SYSTICKVAL16 );
            decrypt(b);
            TRICE16_1( Id(50168), "tim: post decryption SysTick=%d\n", SYSTICKVAL16 );
            TRICE8_8(  Id(36226), "msg: messge = %03x %03x %03x %03x %03x %03x %03x %03x\n", b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7] );
        }
        TRICE0 (Id( 7275), "--------------------------------------------------\n\n" );
#endif
        break;
    default:
        break;
    }
}
#endif // #if TRICE_PACK2_ENCODING == TRICE_ENCODING

