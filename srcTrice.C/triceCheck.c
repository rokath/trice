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



//! write out all types of trices with fixed values for testing
//! \details One trice has one subtrace, if param size max 2 bytes. 
//! Traces with more bytes as parameter consist of several subtraces.
void triceCheckSet(int index) {
    switch (index) {
        case 0:
            TRICE0(Id(24481), "--------------------------------------------------\n");
            //TRICE_P( "Hello %s, this is a %dst printf replacement.\n", "world", 1 ); // visible only with defined TRICE_PRINTF_ADAPTER
            TRICE0(Id(13428), "--------------------------------------------------\n");
            TRICE16_1(Id(18133), "dbg:12345 as 16bit is %#016b\n", 12345);
            TRICE0(Id(3791), "--------------------------------------------------\n");
            //TRICE_ASSERT(0==0);
            TRICE0(Id(17125), "sig:This ASSERT error is just a demo and no real error:\n");
            //TRICE_ASSERT(0==1);
            TRICE0(Id(26797), "--------------------------------------------------\n");
            break;
        case 1:
            TRICE16_1(Id(18113), "ERR:error       message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE16_1(Id(28289), "WRN:warning     message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE16_1(Id(53560), "ATT:attension   message, SysTick is %6d\n", SYSTICKVAL16);
                 // Disassembly for fast bare encoding
                 //     39:             TRICE16_1(Id(53560), "ATT:attension   message, SysTick is %6d\n", SYSTICKVAL16); 
                 // 	                                        ; r2 contains 0xE000E000
                 // 0x0800087C 6994      LDR      r4,[r2,#0x18] ; load systick into R4
                 // 0x0800087E 0DC0      LSRS     r0,r0,#23     ; right shift triceU32FifoWriteIndex by 23 (half mask operation)
                 // 0x08000880 4DE8      LDR      r5,[pc,#928]  ; @0x08000C24 = load ID into r5 (upper 16 bit because <<16 inside Id())
                 // 0x08000882 B2A4      UXTH     r4,r4         ; (uint16_t) mask
                 // 0x08000884 1964      ADDS     r4,r4,r5      ; combine ID and systic into r4
                 // 0x08000886 0085      LSLS     r5,r0,#2      ; compute from r0 byte index into r5
                 // 0x08000888 514C      STR      r4,[r1,r5]    ; store trice to memory location
                 // 0x0800088A 1C40      ADDS     r0,r0,#1      ; increment 32 bit index
                 // 0x0800088C 05C0      LSLS     r0,r0,#23     ; left shift triceU32FifoWriteIndex by 23 (half mask operation)
                 //     40:             TRICE16_1(Id(16672), "DIA:diagnostics message, SysTick is %6d\n", SYSTICKVAL16); 
            TRICE16_1(Id(16672), "DIA:diagnostics message, SysTick is %6d\n", SYSTICKVAL16);
        
        
        
            TRICE16_1(Id(42206), "TIM:timing      message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE16_1(Id(23973), "DBG:debug       message, SysTick is %6d\n", SYSTICKVAL16);
            break;
        case 2:
            TRICE16_1(Id(13730), "SIG:signal      message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE16_1(Id(24302), "RD:read         message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE16_1(Id(14138), "WR:write        message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE16_1(Id(55445), "ISR:interrupt   message, SysTick is %6d\n", SYSTICKVAL16);
                // Disassembly for pack encoding
                //     64:             TRICE16_1(Id(55445), "ISR:interrupt   message, SysTick is %6d\n", SYSTICKVAL16); 
                // 0x08000A42 6984      LDR      r4,[r0,#0x18]
                // 0x08000A44 0DD2      LSRS     r2,r2,#23
                // 0x08000A46 6984      LDR      r4,[r0,#0x18]
                // 0x08000A48 6984      LDR      r4,[r0,#0x18]
                // 0x08000A4A 6985      LDR      r5,[r0,#0x18]
                // 0x08000A4C 0624      LSLS     r4,r4,#24
                // 0x08000A4E 0C24      LSRS     r4,r4,#16
                // 0x08000A50 042D      LSLS     r5,r5,#16
                // 0x08000A52 0E2D      LSRS     r5,r5,#24
                // 0x08000A54 432C      ORRS     r4,r4,r5
                // 0x08000A56 0095      LSLS     r5,r2,#2
                // 0x08000A58 1C52      ADDS     r2,r2,#1
                // 0x08000A5A 514C      STR      r4,[r1,r5]
                // 0x08000A5C 05D2      LSLS     r2,r2,#23
                // 0x08000A5E 0DD2      LSRS     r2,r2,#23
                // 0x08000A60 0095      LSLS     r5,r2,#2
                // 0x08000A62 4C91      LDR      r4,[pc,#580]  ; @0x08000CA8
                // 0x08000A64 514C      STR      r4,[r1,r5]
                // 0x08000A66 1C52      ADDS     r2,r2,#1
                // 0x08000A68 05D2      LSLS     r2,r2,#23
                //     65:             TRICE16_1(Id(41495), "TST:test        message, SysTick is %6d\n", SYSTICKVAL16);         
            TRICE16_1(Id(41495), "TST:test        message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE16_1(Id(45697), "MSG:normal      message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE16_1(Id(24589), "INFO:informal   message, SysTick is %6d\n", SYSTICKVAL16);

            TRICE32_1(Id(55424), "TST:test        message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE32_1(Id(55424), "TST:test        message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE32_1(Id(55424), "TST:test        message, SysTick is %6d\n", SYSTICKVAL16);
                // Disassembly for pack encoding
                //     94:             TRICE32_1(Id(55424), "TST:test        message, SysTick is %6d\n", SYSTICKVAL16); 
                // 0x08000A58 698D      LDR      r5,[r1,#0x18]
                // 0x08000A5A 0DDB      LSRS     r3,r3,#23
                // 0x08000A5C 009E      LSLS     r6,r3,#2
                // 0x08000A5E 1C5B      ADDS     r3,r3,#1
                // 0x08000A60 05DB      LSLS     r3,r3,#23
                // 0x08000A62 5185      STR      r5,[r0,r6]
                // 0x08000A64 0DDB      LSRS     r3,r3,#23
                // 0x08000A66 009D      LSLS     r5,r3,#2
                // 0x08000A68 5142      STR      r2,[r0,r5]
                // 0x08000A6A 1C5B      ADDS     r3,r3,#1
                // 0x08000A6C 05DB      LSLS     r3,r3,#23
                //     95:             TRICE32_1(Id(55424), "TST:test        message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE32_1(Id(55424), "TST:test        message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE32_1(Id(55424), "TST:test        message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE32_1(Id(55424), "TST:test        message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE32_1(Id(55424), "TST:test        message, SysTick is %6d\n", SYSTICKVAL16);
            break;
        case 3:
            TRICE8_4(Id(2527), "tst:TRICE8  %%03x ->  %03x  %03x  %03x  %03x\n", 1, 0x7f, 0x80, 0xff);
            TRICE8_4(Id(63423), "tst:TRICE8   %%4d -> %4d %4d %4d %4d\n", 1, 0x7f, 0x80, 0xff);
            TRICE8_4(Id(40063), "tst:TRICE8   %%4o -> %4o %4o %4o %4o\n", 1, 0x7f, 0x80, 0xff);

            TRICE16_4(Id(8674), "tst:TRICE16  %%05x ->   %05x   %05x   %05x   %05x\n", 1, 0x7fff, 0x8000, 0xffff);
            TRICE16_4(Id(59869), "tst:TRICE16   %%6d ->  %6d  %6d  %6d  %6d\n", 1, 0x7fff, 0x8000, 0xffff);
            TRICE16_4(Id(24781), "tst:TRICE16   %%7o -> %7o %7o %7o %7o\n", 1, 0x7fff, 0x8000, 0xffff);
            break;
        case 4:
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
            TRICE_S( Id(47726), "%s", "" );
            TRICE_S( Id(51347), "%s", "\n" );
            TRICE_S( Id(62061), "%s", "1\n" );
            TRICE_S( Id(61919), "%s", "12\n" );
            TRICE_S( Id(40673), "%s", "123\n" );
            TRICE_S( Id(61171), "%s", "1234\n" );
            TRICE_S( Id(61464), "%s", "12345\n" );
            TRICE_S( Id(64530), "%s", "123456\n" );
            TRICE_S( Id(41027), "%s", "1234567\n" );
            TRICE_S( Id(16073), "%s", "12345678\n" );
            TRICE_S( Id(47177), "%s", "123456789\n" );
            TRICE_S( Id(29848), "%s", "123456789A\n" );
            TRICE_S( Id(56675), "%s", "123456789AB\n" );
            TRICE_S( Id(60173), "%s", "123456789ABC\n" );
            TRICE_S( Id(13422), "%s", "123456789ABCD\n" );
            TRICE_S( Id(39477), "%s", "123456789ABCDE\n" );
            TRICE_S( Id( 4201), "%s", "123456789ABCDEF\n" );
            TRICE_S( Id(31109), "%s", "123456789ABCDEFG\n" );
            break;
        case 15: 
            TRICE_S( Id( 9060), "%s", "123456789ABCDEFGH\n" );
            TRICE_S( Id(65468), "%s", "123456789ABCDEFGHI\n" );   
            TRICE_S( Id(35161), "%s", "123456789ABCDEFGHIJ\n" );
            TRICE_S( Id(41331), "%s", "123456789ABCDEFGHIJ0\n" );
            TRICE_S( Id(64839), "%s", "123456789ABCDEFGHIJ01\n" );
            TRICE_S( Id(53022), "%s", "123456789ABCDEFGHIJ012\n" );
            TRICE_S( Id(36582), "%s", "123456789ABCDEFGHIJ0123\n" );
            TRICE_S( Id(22400), "%s", "123456789ABCDEFGHIJ01234\n" );
            TRICE_S( Id(62017), "%s", "123456789ABCDEFGHIJ012345\n" );
            TRICE_S( Id(59495), "%s", "123456789ABCDEFGHIJ0123456\n" );
            TRICE_S( Id(36371), "%s", "123456789ABCDEFGHIJ01234567\n" );
            TRICE_S( Id( 8879), "%s", "123456789ABCDEFGHIJ012345678\n" );
            TRICE_S( Id( 6138), "%s", "123456789ABCDEFGHIJ0123456789\n" );
            TRICE_S( Id( 6394), "%s", "123456789ABCDEFGHIJ0123456789A\n" );
            TRICE_S( Id( 4796), "%s", "123456789ABCDEFGHIJ0123456789AB\n" );
            TRICE_S( Id(31729), "%s", "123456789ABCDEFGHIJ0123456789ABC\n" );
            TRICE_S( Id(17372), "%s", "123456789ABCDEFGHIJ0123456789ABCD\n" );
            TRICE_S( Id(63839), "%s", "123456789ABCDEFGHIJ0123456789ABCDE\n" );
            TRICE_S( Id(13090), "%s", "123456789ABCDEFGHIJ0123456789ABCDEF\n" );
            break;
        case 16: 
            TRICE_S( Id(50392), "%s", "123456789ABCDEFGHIJ0123456789ABCDEFG\n" );
            TRICE_S( Id(55102), "%s", "123456789ABCDEFGHIJ0123456789ABCDEFGH\n" );
            TRICE_S( Id(13473), "%s", "123456789ABCDEFGHIJ0123456789ABCDEFGHI\n" );
            TRICE_S( Id(51318), "%s", "123456789ABCDEFGHIJ0123456789ABCDEFGHIJ\n" );
            TRICE_S( Id(51003), "%s", "123456789ABCDEFGHIJ0123456789ABCDEFGHIJ0\n" );
            TRICE_S( Id(31736), "%s", "123456789ABCDEFGHIJ0123456789ABCDEFGHIJ01\n" );
            TRICE_S( Id( 7424), "%s", "123456789ABCDEFGHIJ0123456789ABCDEFGHIJ012\n" );
            break;
        case 17: 
            TRICE_S( Id(46462), "%s", "123456789ABCDEFGHIJ0123456789ABCDEFGHIJ0123\n" );
            TRICE_S( Id(65359), "%s", "123456789ABCDEFGHIJ0123456789ABCDEFGHIJ01234\n" );
            TRICE_S( Id( 6430), "%s", "123456789ABCDEFGHIJ0123456789ABCDEFGHIJ012345\n" );
            TRICE_S( Id(29017), "%s", "123456789ABCDEFGHIJ0123456789ABCDEFGHIJ0123456\n" );
            TRICE_S( Id(59317), "%s", "123456789ABCDEFGHIJ0123456789ABCDEFGHIJ01234567\n" );
            TRICE_S( Id(48729), "%s", "123456789ABCDEFGHIJ0123456789ABCDEFGHIJ012345678\n" );
            TRICE_S( Id(22511), "%s", "123456789ABCDEFGHIJ0123456789ABCDEFGHIJ0123456789\n" );
            TRICE_S( Id(62377), "%s", "123456789ABCDEFGHIJ0123456789ABCDEFGHIJ0123456789A\n" );
            break;
        case 18: 
            TRICE_S( Id(35340), "%s", "123456789ABCDEFGHIJ0123456789ABCDEFGHIJ0123456789AB\n" );
            TRICE_S( Id( 3354), "%s", "123456789ABCDEFGHIJ0123456789ABCDEFGHIJ0123456789ABC\n" );
            TRICE_S( Id(28177), "%s", "123456789ABCDEFGHIJ0123456789ABCDEFGHIJ0123456789ABCD\n" );
            break;
        case 19: 
            TRICE_S( Id(24863), "%s", "123456789ABCDEFGHIJKLMNOPQRSTUVW123456789ABCDEFGHIJKLMNOPQRSTUVW123456789ABCDEFGHIJKLMNOPQRSTUVW\n" );
            break;
        case 20: 
            TRICE_S( Id(11397), "%s", "123456789ABCDEFGHIJKLMNOPQRSTUVW123456789ABCDEFGHIJKLMNOPQRSTUVW123456789ABCDEFGHIJKLMNOPQRSTUVW123456789ABCDEFGHIJKLMNOPQRSTU\n" );
            break;   
        case 21: 
            TRICE_S( Id(30121), "%s", "123456789ABCDEFGHIJKLMNOPQRSTUVW123456789ABCDEFGHIJKLMNOPQRSTUVW123456789ABCDEFGHIJKLMNOPQRSTUVW123456789ABCDEFGHIJKLMNOPQRSTUV\n" );
            break;
        case 22: 
            // longest supported dynamically runtime string with 256 chars (pack format is ok with 65535 chars)
            TRICE_S( Id(41906), "sig:%s", "123456789ABCDEFGHIJKLMNOPQRSTUVW123456789ABCDEFGHIJKLMNOPQRSTUVW123456789ABCDEFGHIJKLMNOPQRSTUVW123456789ABCDEFGHIJKLMNOPQRSTUVW123456789ABCDEFGHIJKLMNOPQRSTUVW123456789ABCDEFGHIJKLMNOPQRSTUVW123456789ABCDEFGHIJKLMNOPQRSTUVW123456789ABCDEFGHIJKLMNOPQRABC\n" );
            #ifndef TRICE_ESC_LEGACY  // strlen > 255 forbidden for legacy esc encoding
            TRICE_S( Id(44619), "sig:%s", "123456789ABCDEFGHIJKLMNOPQRSTUVW123456789ABCDEFGHIJKLMNOPQRSTUVW123456789ABCDEFGHIJKLMNOPQRSTUVW123456789ABCDEFGHIJKLMNOPQRSTUVW123456789ABCDEFGHIJKLMNOPQRSTUVW123456789ABCDEFGHIJKLMNOPQRSTUVW123456789ABCDEFGHIJKLMNOPQRSTUVW123456789ABCDEFGHIJKLMNOPQRABCD\n" );
            #endif
        break;
        case 25:
           // TRICE_RS( "tata" ):
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
