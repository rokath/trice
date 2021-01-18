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
        c = c < 0x20 ? c + 0x20 : c;
        c = '`' == c ? 0x20 : c; // exclude back tick for easy testTable generation
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
void triceCheckSet(int index) {
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
            TRICE16_1(Id(41495), "TST:test        message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE16_1(Id(41495), "TST:test        message, SysTick is %6d\n", SYSTICKVAL16);
                // ARM Disassembly for bareL encoding (~27 clocks)
                // 0x08000A9A B672      CPSID    I
                // 0x08000A9C 6986      LDR      r6,[r0,#0x18]
                // 0x08000A9E 009F      LSLS     r7,r3,#2
                // 0x08000AA0 B2B6      UXTH     r6,r6
                // 0x08000AA2 1876      ADDS     r6,r6,r1
                // 0x08000AA4 1C5B      ADDS     r3,r3,#1
                // 0x08000AA6 05DB      LSLS     r3,r3,#23
                // 0x08000AA8 51D6      STR      r6,[r2,r7]
                // 0x08000AAA 0DDB      LSRS     r3,r3,#23
                // 0x08000AAC F3858810  MSR      PRIMASK,r5
                // 0x08000AB0 F3EF8510  MRS      r5,PRIMASK
            TRICE16_1(Id(41495), "TST:test        message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE16_1(Id(41495), "TST:test        message, SysTick is %6d\n", SYSTICKVAL16);
                // ARM Disassembly for packL encoding (~34 clocks)
                // 0x08000B30 B672      CPSID    I
                // 0x08000B32 009E      LSLS     r6,r3,#2
                // 0x08000B34 5181      STR      r1,[r0,r6]
                // 0x08000B36 1C5B      ADDS     r3,r3,#1
                // 0x08000B38 05DB      LSLS     r3,r3,#23 
                // 0x08000B3A 6996      LDR      r6,[r2,#0x18]
                // 0x08000B3C 0DDB      LSRS     r3,r3,#23
                // 0x08000B3E 009F      LSLS     r7,r3,#2
                // 0x08000B40 1C5B      ADDS     r3,r3,#1
                // 0x08000B42 B2B6      UXTH     r6,r6
                // 0x08000B44 05DB      LSLS     r3,r3,#23
                // 0x08000B46 51C6      STR      r6,[r0,r7]
                // 0x08000B48 0DDB      LSRS     r3,r3,#23
                // 0x08000B4A F3848810  MSR      PRIMASK,r4
                // 0x08000B4E F3EF8410  MRS      r4,PRIMASK
            TRICE16_1(Id(41495), "TST:test        message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE16_1(Id(41495), "TST:test        message, SysTick is %6d\n", SYSTICKVAL16);
                // ARM Disassembly for pack2L encoding (~39 clocks)
                // 0x08000C14 B672      CPSID    I
                // 0x08000C16 1866      ADDS     r6,r4,r1
                // 0x08000C18 009F      LSLS     r7,r3,#2
                // 0x08000C1A 1C64      ADDS     r4,r4,#1
                // 0x08000C1C 51C6      STR      r6,[r0,r7]
                // 0x08000C1E 1C5B      ADDS     r3,r3,#1
                // 0x08000C20 05DB      LSLS     r3,r3,#23
                // 0x08000C22 6996      LDR      r6,[r2,#0x18]
                // 0x08000C24 B2E4      UXTB     r4,r4
                // 0x08000C26 0DDB      LSRS     r3,r3,#23
                // 0x08000C28 009F      LSLS     r7,r3,#2
                // 0x08000C2A 1C5B      ADDS     r3,r3,#1
                // 0x08000C2C B2B6      UXTH     r6,r6
                // 0x08000C2E 05DB      LSLS     r3,r3,#23
                // 0x08000C30 51C6      STR      r6,[r0,r7]
                // 0x08000C32 0DDB      LSRS     r3,r3,#23
                // 0x08000C34 F3858810  MSR      PRIMASK,r5
                // 0x08000C38 F3EF8510  MRS      r5,PRIMASK
            TRICE16_1(Id(41495), "TST:test        message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE16_1(Id(41495), "TST:test        message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE16_1(Id(41495), "TST:test        message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE16_1(Id(41495), "TST:test        message, SysTick is %6d\n", SYSTICKVAL16);
            break;
        case 3:
            TRICE32_1(Id(55424), "TST:test        message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE32_1(Id(55424), "TST:test        message, SysTick is %6d\n", SYSTICKVAL16);
                // ARM Disassembly for bareL encoding (~40 clocks)
                // 0x08000BBC B672      CPSID    I
                // 0x08000BBE 6986      LDR      r6,[r0,#0x18]
                // 0x08000BC0 009F      LSLS     r7,r3,#2
                // 0x08000BC2 0C36      LSRS     r6,r6,#16
                // 0x08000BC4 51CE      STR      r6,[r1,r7]
                // 0x08000BC6 1C5B      ADDS     r3,r3,#1
                // 0x08000BC8 05DB      LSLS     r3,r3,#23
                // 0x08000BCA 6986      LDR      r6,[r0,#0x18]
                // 0x08000BCC 0DDB      LSRS     r3,r3,#23
                // 0x08000BCE B2B6      UXTH     r6,r6
                // 0x08000BD0 1AB6      SUBS     r6,r6,r2
                // 0x08000BD2 009F      LSLS     r7,r3,#2
                // 0x08000BD4 1C5B      ADDS     r3,r3,#1
                // 0x08000BD6 05DB      LSLS     r3,r3,#23
                // 0x08000BD8 51CE      STR      r6,[r1,r7]
                // 0x08000BDA 0DDB      LSRS     r3,r3,#23
                // 0x08000BDC F3848810  MSR      PRIMASK,r4
                // 0x08000BE0 F3EF8410  MRS      r4,PRIMASK
            TRICE32_1(Id(55424), "TST:test        message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE32_1(Id(55424), "TST:test        message, SysTick is %6d\n", SYSTICKVAL16);
                // ARM Disassembly for packL encoding (~33 clocks)
                // 0x08000CF0 B672      CPSID    I
                // 0x08000CF2 009E      LSLS     r6,r3,#2
                // 0x08000CF4 5181      STR      r1,[r0,r6]
                // 0x08000CF6 1C5B      ADDS     r3,r3,#1
                // 0x08000CF8 05DB      LSLS     r3,r3,#23
                // 0x08000CFA 6996      LDR      r6,[r2,#0x18]
                // 0x08000CFC 0DDB      LSRS     r3,r3,#23
                // 0x08000CFE 009F      LSLS     r7,r3,#2
                // 0x08000D00 1C5B      ADDS     r3,r3,#1
                // 0x08000D02 05DB      LSLS     r3,r3,#23
                // 0x08000D04 51C6      STR      r6,[r0,r7]
                // 0x08000D06 0DDB      LSRS     r3,r3,#23
                // 0x08000D08 F3858810  MSR      PRIMASK,r5
                // 0x08000D0C F3EF8510  MRS      r5,PRIMASK                
            TRICE32_1(Id(55424), "TST:test        message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE32_1(Id(55424), "TST:test        message, SysTick is %6d\n", SYSTICKVAL16);
                // ARM Disassembly for pack2L encoding (~37 clocks)
                // 0x08000E12 B672      CPSID    I
                // 0x08000E14 1866      ADDS     r6,r4,r1
                // 0x08000E16 009F      LSLS     r7,r3,#2
                // 0x08000E18 1C64      ADDS     r4,r4,#1
                // 0x08000E1A 51C6      STR      r6,[r0,r7]
                // 0x08000E1C 1C5B      ADDS     r3,r3,#1
                // 0x08000E1E 05DB      LSLS     r3,r3,#23
                // 0x08000E20 6996      LDR      r6,[r2,#0x18]
                // 0x08000E22 B2E4      UXTB     r4,r4
                // 0x08000E24 0DDB      LSRS     r3,r3,#23
                // 0x08000E26 009F      LSLS     r7,r3,#2
                // 0x08000E28 1C5B      ADDS     r3,r3,#1
                // 0x08000E2A 05DB      LSLS     r3,r3,#23
                // 0x08000E2C 51C6      STR      r6,[r0,r7]
                // 0x08000E2E 0DDB      LSRS     r3,r3,#23
                // 0x08000E30 F3858810  MSR      PRIMASK,r5
                // 0x08000E34 F3EF8510  MRS      r5,PRIMASK               
            TRICE32_1(Id(55424), "TST:test        message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE32_1(Id(55424), "TST:test        message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE32_1(Id(55424), "TST:test        message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE32_1(Id(55424), "TST:test        message, SysTick is %6d\n", SYSTICKVAL16);
            break;
        case 4:
            TRICE8_4(Id(2527), "tst:TRICE8  %%03x ->  %03x  %03x  %03x  %03x\n", 1, 0x7f, 0x80, 0xff);
            TRICE8_4(Id(63423), "tst:TRICE8   %%4d -> %4d %4d %4d %4d\n", 1, 0x7f, 0x80, 0xff);
            TRICE8_4(Id(40063), "tst:TRICE8   %%4o -> %4o %4o %4o %4o\n", 1, 0x7f, 0x80, 0xff);

            TRICE16_4(Id(8674), "tst:TRICE16  %%05x ->   %05x   %05x   %05x   %05x\n", 1, 0x7fff, 0x8000, 0xffff);
            TRICE16_4(Id(59869), "tst:TRICE16   %%6d ->  %6d  %6d  %6d  %6d\n", 1, 0x7fff, 0x8000, 0xffff);
            TRICE16_4(Id(24781), "tst:TRICE16   %%7o -> %7o %7o %7o %7o\n", 1, 0x7fff, 0x8000, 0xffff);

            TRICE32_4(Id(16811), "tst:TRICE32_4 %%09x ->      %09x      %09x       %09x     %09x\n", 1, 0x7fffffff, 0x80000000, 0xffffffff); // 4
            TRICE32_4(Id(28944), "tst:TRICE32_4 %%10d ->     %10d     %10d     %10d    %10x\n", 1, 0x7fffffff, 0x80000000, 0xffffffff); // 4
            TRICE64_1(Id(19049), "att:64bit %#b\n", 0x1122334455667788ull);
            break;
        case 5:
            TRICE8_1(Id(41267), "tst:TRICE8_1 %d\n", -1);
            TRICE8_2(Id(28873), "tst:TRICE8_2 %d %d\n", -1, -2);
            TRICE8_3(Id(18501), "tst:TRICE8_3 %d %d %d\n", -1, -2, -3);
            TRICE8_4(Id(34892), "tst:TRICE8_4 %d %d %d %d\n", -1, -2, -3, -4);
            TRICE8_5(Id( 2043), "tst:TRICE8_5 %d %d %d %d %d\n", -1, -2, -3, -4, -5);
            TRICE8_6(Id(36242), "tst:TRICE8_6 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6);
            TRICE8_7(Id(23187), "tst:TRICE8_7 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7);
            TRICE8_8(Id( 5279), "tst:TRICE8_8 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8);
            break;
        case 7:
            TRICE16_1(Id(50977), "tst:TRICE16_1 %d\n", -1); // 1
            TRICE16_2(Id(65136), "tst:TRICE16_2 %d %d\n", -1, -2); // 2
            TRICE16_3(Id(58915), "tst:TRICE16_3 %d %d %d\n", -1, -2, -3); // 3
            TRICE16_4(Id(45117), "tst:TRICE16_4 %d %d %d %d\n", -1, -2, -3, -4); // 4
            break;
        case 8:
            TRICE32_1(Id(6384), "tst:TRICE32_1 %d\n", -1); // 2
            TRICE32_1(Id( 4950), "tst:TRICE32_1 %08x\n", 0x0123cafe); // 2
            TRICE32_2(Id(11505), "tst:TRICE32_2 %d %d\n", -1, -2); // 4
            TRICE32_3(Id(56189), "tst:TRICE32_3 %d %d %d\n", -1, -2, -3); // 4
            TRICE32_4(Id(36573), "tst:TRICE32_4 %d %d %d %d\n", -1, -2, -3, -4); // 4
            break;
        case 9:
            TRICE64_1(Id(43171), "tst:TRICE64_1 %d\n", -1); // 2
            TRICE64_2(Id( 8402), "tst:TRICE64_2 %d %d\n", -1, -2); // 4
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
            TRICE8_1(Id(41267), "tst:TRICE8_1 %d\n", 1);
            TRICE8_2(Id(28873), "tst:TRICE8_2 %d %d\n", 1, 2);
            TRICE8_3(Id(18501), "tst:TRICE8_3 %d %d %d\n", 1, 2, 3);
            TRICE8_4(Id(34892), "tst:TRICE8_4 %d %d %d %d\n", 1, 2, 3, 4);
            TRICE8_5(Id( 2043), "tst:TRICE8_5 %d %d %d %d %d\n", 1, 2, 3, 4, 5);
            TRICE8_6(Id(36242), "tst:TRICE8_6 %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6);
            TRICE8_7(Id(23187), "tst:TRICE8_7 %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7);
            TRICE8_8(Id( 5279), "tst:TRICE8_8 %d %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7, 8);
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
        case 33:
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
