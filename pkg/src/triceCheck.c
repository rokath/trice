/*! \file triceCheck.c
\brief trices for tool evaluation
\details The trices are dumped as 32bit values into a 32 bit wide fifo.
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/

//#define TRICE_OFF // enable this line to disable trice code generation in this file object
#include "trice.h"

// nextRuntimeString returns a in length changing string from 0 to 250 bytes.
static char* nextRuntimeString( int length ){
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
static void triceRuntimeStrings( int from, int limit){
    for( int i = from; i < limit; i++ ){
        char* dynString =  nextRuntimeString(i);
        TRICE16_1( Id( 45910), "dbg:len=%d, ", strlen(dynString) );
        TRICE_S( Id( 47417), "MESSAGE:%s\n", dynString );   
    }
}

//! triceCheckSet writes out all types of trices with fixed values for testing
//! \details One trice has one subtrace, if param size max 2 bytes. 
//! Traces with more bytes as parameter consist of several subtraces.
void triceCheckSet(int index) {
    switch (index) {
        case 1:
            TRICE8_2( Id( 40707), "rd:TRICE8_2 %d, %d\n", 1, 2 );
            TRICE8_2( Id( 62688), "rd:TRICE8_2 %d, %d\n", 2, 3 );
            TRICE8_2( Id( 49739), "rd:TRICE8_2 %d, %d\n", 3, 4 );
            TRICE8_2( Id( 57430), "rd:TRICE8_2 %d, %d\n", 4, 5 );
            TRICE8_2( Id( 47602), "rd:TRICE8_2 %d, %d\n", 5, 6 );
            TRICE8_1( Id( 63474), "rd:TRICE8_1 line %d (%%d -1)\n", -1 );
            TRICE8_1( Id( 61213), "rd:TRICE8_1 line %u (%%u -1)\n", -1 );
            TRICE8_1( Id( 56571), "rd:TRICE8_1 line %b (%%b -1)\n", -1 );
            TRICE8_1( Id( 63602), "rd:TRICE8_1 line %x (%%x -1)\n", -1 );
            TRICE16_1( Id( 54350), "rd:TRICE16_1 line %d (%%d -1)\n", -1 );
            TRICE16_1( Id( 35169), "rd:TRICE16_1 line %u (%%u -1)\n", -1 );
            TRICE16_1( Id( 35133), "rd:TRICE16_1 line %b (%%b -1)\n", -1 );
            TRICE16_1( Id( 53632), "rd:TRICE16_1 line %x (%%x -1)\n", -1 );
            TRICE32_1( Id( 45750), "rd:TRICE32_1 line %d (%%d -1)\n", -1 );
            TRICE32_1( Id( 49915), "rd:TRICE32_1 line %u (%%u -1)\n", -1 );
            TRICE32_1( Id( 39033), "rd:TRICE32_1 line %b (%%b -1)\n", -1 );
            TRICE32_1( Id( 48655), "rd:TRICE32_1 line %x (%%x -1)\n", -1 );
            TRICE64_1( Id( 41928), "rd:TRICE64_1 line %d (%%d -1)\n", -1 );
            TRICE64_1( Id( 48160), "rd:TRICE64_1 line %u (%%u -1)\n", -1 );
            TRICE64_1( Id( 33464), "rd:TRICE64_1 line %b (%%b -1)\n", -1 );
            TRICE64_1( Id( 49762), "rd:TRICE64_1 line %x (%%x -1)\n", -1 );

            TRICE8_2( Id( 58835), "rd:TRICE8_2 line %d, %d\n", __LINE__, 2 );
        break;
        case 2:
            TRICE32_1( Id( 61654), "ATT:trice16_1 attention message, SysTick is %6d\n", SYSTICKVAL);
            TRICE32_1( Id( 63097), "ATT:trice16_1 attention message, SysTick is %6d\n", SYSTICKVAL);
            TRICE32_1( Id( 59357), "ATT:trice16_1 attention message, SysTick is %6d\n", SYSTICKVAL);
            TRICE32_1( Id( 55647), "ATT:trice16_1 attention message, SysTick is %6d\n", SYSTICKVAL);
            TRICE32_1( Id( 43058), "ATT:trice16_1 attention message, SysTick is %6d\n", SYSTICKVAL);
            TRICE32_1( Id( 40194), "ATT:trice16_1 attention message, SysTick is %6d\n", SYSTICKVAL);
            TRICE32_1( Id( 50626), "ATT:trice16_1 attention message, SysTick is %6d\n", SYSTICKVAL);
            TRICE32_1( Id( 43507), "ATT:trice16_1 attention message, SysTick is %6d\n", SYSTICKVAL);
            TRICE32_1( Id( 42521),  "rd:TRICE32_1 line %d\n", __LINE__ );
            TRICE64_1( Id( 51133),  "rd:TRICE64_1 %d\n", __LINE__ );
            TRICE32_2( Id( 55925),  "rd:TRICE32_2 line %d,%d\n", __LINE__, 2 );
            TRICE64_2( Id( 36492),  "rd:TRICE64_2 line %d,%d\n", __LINE__, 2 );
        break;
        case 3:
            TRICE8_1( Id( 46820), "rd:TRICE8_1 %d\n", 1 );
            TRICE8_2( Id( 56991), "rd:TRICE8_2 %d, %d\n", 1, 2 );
            TRICE8_3( Id( 40111), "rd:TRICE8_3 %d, %d, %d\n", 1, 2, 3 );
            TRICE8_4( Id( 42772), "rd:TRICE8_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
            TRICE8_5( Id( 34440), "rd:TRICE8_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
            TRICE8_6( Id( 60589), "rd:TRICE8_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
            TRICE8_7( Id( 43242), "rd:TRICE8_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
            TRICE8_8( Id( 49170), "rd:TRICE8_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );      
            TRICE16_1( Id( 39639), "rd:TRICE16_1 %d\n", 1 );
            TRICE16_2( Id( 54944), "rd:TRICE16_2 %d, %d\n", 1, 2 );
            TRICE16_3( Id( 50030), "rd:TRICE16_3 %d, %d, %d\n", 1, 2, 3 );
            TRICE16_4( Id( 40726), "rd:TRICE16_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
            TRICE32_1( Id( 42365), "rd:TRICE32_1 %d\n", 1 );
            TRICE32_2( Id( 60142), "rd:TRICE32_2 %d, %d\n", 1, 2 );
            TRICE32_3( Id( 62377), "rd:TRICE32_3 %d, %d, %d\n", 1, 2, 3 );
            TRICE32_4( Id( 62133), "rd:TRICE32_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
            TRICE64_1( Id( 58681), "rd:TRICE64_1 %d\n", 1 );
            TRICE64_2( Id( 41862), "rd:TRICE64_2 %d, %d\n", 1, 2 );
        break;
        case 4:
            TRICE0(Id( 55247), "--------------------------------------------------\n");
            TRICE0(Id( 51804), "--------------------------------------------------\n");
            TRICE16_1(Id( 56252), "dbg:12345 as 16bit is %#016b\n", 12345);
            TRICE0(Id( 45734), "--------------------------------------------------\n");
            TRICE0(Id( 48439), "sig:This ASSERT error is just a demo and no real error:\n");
            TRICE0(Id( 36621), "--------------------------------------------------\n");
            TRICE16_1(Id( 62486), "ERR:error       message, SysTick is %6u\n", SYSTICKVAL);
            TRICE16_1(Id( 50106), "WRN:warning     message, SysTick is %6u\n", SYSTICKVAL);
            TRICE16_1(Id( 55534), "ATT:attention   message, SysTick is %6u\n", SYSTICKVAL);
            TRICE16_1(Id( 39816), "DIA:diagnostics message, SysTick is %6u\n", SYSTICKVAL);
            TRICE16_1(Id( 60853), "TIM:timing      message, SysTick is %6u\n", SYSTICKVAL);
            TRICE16_1(Id( 64581), "DBG:debug       message, SysTick is %6u\n", SYSTICKVAL);
            TRICE16_1(Id( 46649), "SIG:signal      message, SysTick is %6u\n", SYSTICKVAL);
            TRICE16_1(Id( 45449), "RD:read         message, SysTick is %6u\n", SYSTICKVAL);
            TRICE16_1(Id( 38091), "WR:write        message, SysTick is %6u\n", SYSTICKVAL);
            TRICE16_1(Id( 56748), "ISR:interrupt   message, SysTick is %6u\n", SYSTICKVAL);
            TRICE16_1(Id( 51343), "MSG:normal      message, SysTick is %6u\n", SYSTICKVAL);
            TRICE16_1(Id( 59255), "INFO:informal   message, SysTick is %6u\n", SYSTICKVAL);
        break;
        case 5:
            TRICE32_1(Id( 38012), "time:TRICE32_1   message, SysTick is %6u\n", SYSTICKVAL);
            TRICE32_1(Id( 33478), "time:TRICE32_1   message, SysTick is %6u\n", SYSTICKVAL);
            TRICE32_1(Id( 57446), "time:TRICE32_1   message, SysTick is %6u\n", SYSTICKVAL);
            TRICE32_1(Id( 51946), "time:TRICE32_1   message, SysTick is %6u\n", SYSTICKVAL);
            TRICE32_1(Id( 34576), "time:TRICE32_1   message, SysTick is %6u\n", SYSTICKVAL);
            TRICE32_1(Id( 42368), "time:TRICE32_1   message, SysTick is %6u\n", SYSTICKVAL);
            TRICE32_1(Id( 63633), "time:TRICE32_1   message, SysTick is %6u\n", SYSTICKVAL);
            TRICE32_1(Id( 53889), "time:TRICE32_1   message, SysTick is %6u\n", SYSTICKVAL);
            TRICE32_1(Id( 42207), "time:TRICE32_1   message, SysTick is %6u\n", SYSTICKVAL);
            TRICE32_1(Id( 38326), "time:TRICE32_1   message, SysTick is %6u\n", SYSTICKVAL);
            TRICE32_1(Id( 61267), "time:TRICE32_1   message, SysTick is %6u\n", SYSTICKVAL);
        break;
        case 6:
            TRICE8_4( Id( 43420), "tst:TRICE8_4  %%03x ->  %03x  %03x  %03x  %03x\n", 1, 0x7f, 0x80, 0xff);
            TRICE8_4( Id( 46621), "tst:TRICE8_4   %%4d -> %4d %4d %4d %4d\n", 1, 0x7f, 0x80, 0xff);
            TRICE8_4( Id( 50996), "tst:TRICE8_4   %%4o -> %4o %4o %4o %4o\n", 1, 0x7f, 0x80, 0xff);
            TRICE16_4(Id( 37343), "tst:TRICE16_4  %%05x ->   %05x   %05x   %05x   %05x\n", 1, 0x7fff, 0x8000, 0xffff);
            TRICE16_4(Id( 58854), "tst:TRICE16_4   %%6d ->  %6d  %6d  %6d  %6d\n", 1, 0x7fff, 0x8000, 0xffff);
            TRICE16_4(Id( 61023), "tst:TRICE16_4   %%7o -> %7o %7o %7o %7o\n", 1, 0x7fff, 0x8000, 0xffff);
            TRICE32_4(Id( 47740), "tst:TRICE32_4 %%09x ->      %09x      %09x       %09x     %09x\n", 1, 0x7fffffff, 0x80000000, 0xffffffff); // 4
            TRICE32_4(Id( 46761), "tst:TRICE32_4 %%10d ->     %10d     %10d     %10d    %10x\n", 1, 0x7fffffff, 0x80000000, 0xffffffff); // 4
            TRICE64_1(Id( 49603), "att:TRICE64_1 %#b\n", 0x1122334455667788);
        break;
        case 7:
            TRICE8_1(Id( 52582), "tst:TRICE8_1 %02x\n", 0xA1);
            TRICE8_2(Id( 58020), "tst:TRICE8_2 %02x %02x\n", 0xA1, 0xA2);
            TRICE8_3(Id( 40122), "tst:TRICE8_3 %02x %02x %02x\n", 0xA1, 0xA2, 0xA3);
            TRICE8_4(Id( 50551), "tst:TRICE8_4 %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4);
            TRICE8_5(Id( 33857), "tst:TRICE8_5 %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5 );
            TRICE8_6(Id( 55004), "tst:TRICE8_6 %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6);
            TRICE8_7(Id( 49025), "tst:TRICE8_7 %02x %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7);
            TRICE8_8(Id( 53622), "tst:TRICE8_8 %02x %02x %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8);
        break;
        case 8:
            TRICE8_1(Id( 55503), "tst:TRICE8_1 %d\n", -111);
            TRICE8_2(Id( 64874), "tst:TRICE8_2 %d %d\n", -111, -222);
            TRICE8_3(Id( 49107), "tst:TRICE8_3 %d %d %d\n", -111, -222, -3);
            TRICE8_4(Id( 34673), "tst:TRICE8_4 %d %d %d %d\n", -111, -222, -3, -4);
            TRICE8_5(Id( 59637), "tst:TRICE8_5 %d %d %d %d %d\n", -111, -222, -3, -4, -5);
            TRICE8_6(Id( 60537), "tst:TRICE8_6 %d %d %d %d %d %d\n", -111, -222, -3, -4, -5, -6);
            TRICE8_7(Id( 45261), "tst:TRICE8_7 %d %d %d %d %d %d %d\n", -111, -222, -3, -4, -5, -6, -7);
            TRICE8_8(Id( 63789), "tst:TRICE8_8 %d %d %d %d %d %d %d %d\n", -111, -222, -3, -4, -5, -6, -7, -8);
            break;
        case 9:
            TRICE16_1(Id( 35206), "tst:TRICE16_1 %d\n", -111); // 1
            TRICE16_2(Id( 45725), "tst:TRICE16_2 %d %d\n", -111, -222); // 2
            TRICE16_3(Id( 63897), "tst:TRICE16_3 %d %d %d\n", -111, -222, -333); // 3
            TRICE16_4(Id( 55371), "tst:TRICE16_4 %d %d %d %d\n", -111, -222, -333, -444); // 4
            TRICE32_1(Id( 58781), "tst:TRICE32_1 %08x\n", 0x0123cafe); // 2
            TRICE32_1(Id( 54389), "tst:TRICE32_1 %d\n", -111); // 2
            TRICE32_2(Id( 59795), "tst:TRICE32_2 %x %x\n", -111, -222); // 4
            TRICE32_2(Id( 47183), "tst:TRICE32_2 %d %d\n", -111, -222); // 4
            TRICE32_3(Id( 39898), "tst:TRICE32_3 %x %x %x\n", -111, -222, -333); // 4
            TRICE32_3(Id( 40901), "tst:TRICE32_3 %d %d %d\n", -111, -222, -333); // 4
            TRICE32_4(Id( 58747), "tst:TRICE32_4 %x %x %x %x\n", -111, -222, -333, -444); // 4
            TRICE32_4(Id( 59716), "tst:TRICE32_4 %d %d %d %d\n", -111, -222, -333, -444); // 4
            TRICE64_1(Id( 59670), "tst:TRICE64_1 %d\n", -111); // 2
            TRICE64_2(Id( 46159), "tst:TRICE64_2 %d %d\n", -111, -222); // 4
            break;
        case 10:
            TRICE0(Id( 42984), "e:A");
            TRICE0(Id( 65475), "w:B");
            TRICE0(Id( 60278), "a:c");
            TRICE0(Id( 39056), "wr:d");
            TRICE0(Id( 57073), "rd:e\n");
            TRICE0(Id( 35315), "diag:f");
            TRICE0(Id( 53769), "d:G");
            TRICE0(Id( 38573), "t:H");
            TRICE0(Id( 37916), "time:i");
            TRICE0(Id( 52118), "message:J");
            TRICE0(Id( 49746), "dbg:k\n");
        break;
        case 11:
            TRICE0(Id( 40004), "1");
            TRICE0(Id( 61543), "2");
            TRICE0(Id( 51583), "3");
            TRICE0(Id( 35885), "4");
            TRICE0(Id( 40078), "e:7");
            TRICE0(Id( 44255), "m:12");
            TRICE0(Id( 51771), "m:123\n");
        break;
        case 12:
            TRICE8_1(Id( 54804), "tst:TRICE8_1 %d\n", 1);
            TRICE8_2(Id( 42494), "tst:TRICE8_2 %d %d\n", 1, 2);
            TRICE8_3(Id( 44072), "tst:TRICE8_3 %d %d %d\n", 1, 2, 3);
            TRICE8_4(Id( 51249), "tst:TRICE8_4 %d %d %d %d\n", 1, 2, 3, 4);
            TRICE8_5(Id( 56554), "tst:TRICE8_5 %d %d %d %d %d\n", 1, 2, 3, 4, 5);
            TRICE8_6(Id( 37866), "tst:TRICE8_6 %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6);
            TRICE8_7(Id( 35663), "tst:TRICE8_7 %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7);
            TRICE8_8(Id( 53316), "tst:TRICE8_8 %d %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7, 8);
            break;
        case 13:
            TRICE8_8(Id( 39887), "tst:TRICE8_1 %%d=%d, %%u=%u, 0x%%x=0x%x, 0x%%2x=0x%2x, 0x%%02x=0x%02x, 0x%%3x=0x%3x, 0x%%03x=0x%03x, %%b=%b\n", 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81);
            TRICE16_1(Id( 45609), "tst:TRICE16_1 0x%04x\n", 0xa0);
            TRICE8_1(Id( 34438), "tst:TRICE8_1 0x%02x\n", 0xa0);
            TRICE8_1(Id( 40928), "tst:TRICE8_1 0x%02x\n", 0xa0);
            TRICE8_1(Id( 59290), "tst:TRICE8_1 %u\n", 201);
            TRICE8_2(Id( 53955), "tst:TRICE8_2 %u %u\n", 201, 202);
            TRICE8_3(Id( 33982), "tst:TRICE8_3 %u %u %u\n", 201, 202, 203);
            TRICE8_4(Id( 64087), "tst:TRICE8_4 %u %u %u %u\n", 201, 202, 203, 204);
            TRICE8_5(Id( 56344), "tst:TRICE8_5 %u %u %u %u %u\n", 201, 202, 203, 204, 205);
            TRICE8_6(Id( 64958), "tst:TRICE8_6 %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206);
            TRICE8_7(Id( 37787), "tst:TRICE8_7 %u %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206, 207);
            TRICE8_8(Id( 44560), "tst:TRICE8_8 %u %u %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206, 207, 208);
            TRICE16_1(Id( 57408), "tst:TRICE16_1 %u\n", 60001);
            TRICE16_2(Id( 64554), "tst:TRICE16_2 %u %u\n", 60001, 60002);
            TRICE16_3(Id( 35559), "tst:TRICE16_3 %u %u %u\n", 60001, 60002, 60003);
            TRICE16_4(Id( 53522), "tst:TRICE16_4 %u %u %u %u\n", 60001, 60002, 60003, 60004);
            TRICE32_1(Id( 53612), "tst:TRICE32_1 %u\n", 4000000001);
            TRICE32_2(Id( 64117), "tst:TRICE32_2 %u %u\n", 4000000001, 4000000002);
            TRICE32_3(Id( 46431), "tst:TRICE32_3 %u %u %u\n", 4000000001, 4000000002, 4000000003);
            TRICE32_4(Id( 50279), "tst:TRICE32_4 %u %u %u %u\n", 4000000001, 4000000002, 4000000003, 4000000004);
            TRICE64_1(Id( 59485), "tst:TRICE64_1 %x\n", -1);
            TRICE64_2(Id( 63675), "tst:TRICE64_2 %x %x\n", -1, -2);
            TRICE64_1(Id( 49602), "tst:TRICE64_1 %u\n", -1);
            TRICE64_2(Id( 39532), "tst:TRICE64_2 %u %u\n", -1, -2);
            TRICE64_1(Id( 39510), "tst:TRICE64_1 %s\n", -1);
            TRICE64_2(Id( 50297), "tst:TRICE64_2 %s %u\n", -1, -2);
        break;
        case 14:
            TRICE8_8( Id( 48310), "dbg:%d %d %d %d %d %d %d %d\n", 8, 8, 8, 8, 8, 8, 8, 8 );
        {
            char* s = "AAAABBBBCCCC";
            TRICE_S( Id( 43140), "sig:%s\n", s ); 
            triceRuntimeStrings(0, 20);
        }
        break;
        case 15: 
            triceRuntimeStrings(30, 32 );
        break;
#if TRICE_MODE > 99 // direct modes have not enough buffer (or increase buffer size on stack ig possible=
        case 16: 
            triceRuntimeStrings(132, 136);
        break;
        case 17: 
            triceRuntimeStrings(250, 252); // Count byte is fb=251
            {
            static uint16_t cnt = 0;
            char *dataArray = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"; // The array with characters to send
            TRICE16_1( Id( 63480),"d:Sending uint16 counter as two byte hex: %x\n", cnt);
            TRICE_S( Id( 64963),"msg:Sending array: %s\n", dataArray);
            cnt++; 
        }
        break;
#endif // #if TRICE_MODE > 99 
        case 18: 
            //triceRuntimeStrings(252, 253); // 252 not possible
        {
            static uint16_t cnt = 0;
            char *dataArray = "DataArray[13]"; // The array with characters to send
            /* Transmit different data types through the UART */   
            TRICE0( Id( 40819),"\natt:Software Transmit UART Component demo\n");
            TRICE8_1( Id( 35344),"wrn:Sending 254 as single byte hex: %02x\n", 254);
            TRICE16_1( Id( 54723),"d:Sending uint16 counter as two byte hex: %x\n", cnt);
            TRICE_S( Id( 54661),"msg:Sending array: %s\n", dataArray);
            TRICE32_1( Id( 45696), "tim:SysTick=%d\n", SYSTICKVAL );
            TRICE32_1( Id( 51893), "tim:SysTick=%d\n", SYSTICKVAL );
            cnt++; 
        }
        break;
        case 19:
#ifdef ENCRYPT
        {
            uint8_t b[8] = {1,2,3,4,5,6,7,8};
            TRICE8_8(  Id( 48446), "msg: message = %03x %03x %03x %03x %03x %03x %03x %03x\n", b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7] );
            TRICE16_1( Id( 44144), "tim: pre encryption SysTick=%d\n", SYSTICKVAL );
            encrypt(b);
            TRICE16_1( Id( 53394), "tim: post encryption SysTick=%d\n", SYSTICKVAL );
            TRICE8_8(  Id( 64246), "att: encrypted = %03x %03x %03x %03x %03x %03x %03x %03x\n", b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7] );
            TRICE16_4( Id( 51043), "att: encrypted = %d,%d,%d,%d,", b[0], b[1], b[2], b[3] );
            TRICE16_4( Id( 48135), "att: %d,%d,%d,%d\n", b[4], b[5], b[6], b[7] );
            #ifdef DECRYPT
            TRICE16_1( Id( 61189), "tim: pre decryption SysTick=%d\n", SYSTICKVAL );
            decrypt(b);
            TRICE16_1( Id( 38266), "tim: post decryption SysTick=%d\n", SYSTICKVAL );
            #endif
            TRICE8_8(  Id( 51031), "msg: messge = %03x %03x %03x %03x %03x %03x %03x %03x\n", b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7] );
        }
        TRICE0 (Id( 51749), "--------------------------------------------------\n\n" );
#endif
        break;
        case 20:
            TRICE8_8(Id( 33702), "tst:TRICE8_1 %%d=%d, %%u=%u, 0x%%x=0x%x, 0x%%2x=0x%2x, 0x%%02x=0x%02x, 0x%%3x=0x%3x, 0x%%03x=0x%03x, %%b=%b\n", 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81);
            TRICE16_1(Id( 34004), "tst:TRICE16_1 0x%04x\n", 0xa0);
            TRICE8_1(Id( 62342), "tst:TRICE8_1 0x%02x\n", 0xa0);
            TRICE8_1(Id( 34484), "tst:TRICE8_1 0x%02x\n", 0xa0);
            TRICE8_1(Id( 56229), "tst:TRICE8_1 %u\n", 201);
            TRICE8_2(Id( 49010), "tst:TRICE8_2 %u %u\n", 201, 202);
            TRICE8_3(Id( 52509), "tst:TRICE8_3 %u %u %u\n", 201, 202, 203);
            TRICE8_4(Id( 47415), "tst:TRICE8_4 %u %u %u %u\n", 201, 202, 203, 204);
            TRICE8_5(Id( 56700), "tst:TRICE8_5 %u %u %u %u %u\n", 201, 202, 203, 204, 205);
            TRICE8_6(Id( 58847), "tst:TRICE8_6 %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206);
            TRICE8_7(Id( 51359), "tst:TRICE8_7 %u %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206, 207);
            TRICE8_8(Id( 52236), "tst:TRICE8_8 %u %u %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206, 207, 208);
            TRICE16_1(Id( 55311), "tst:TRICE16_1 %u\n", 60001);
            TRICE16_2(Id( 63358), "tst:TRICE16_2 %u %u\n", 60001, 60002);
            TRICE16_3(Id( 58776), "tst:TRICE16_3 %u %u %u\n", 60001, 60002, 60003);
            TRICE16_4(Id( 51895), "tst:TRICE16_4 %u %u %u %u\n", 60001, 60002, 60003, 60004);
            TRICE32_1(Id( 33535), "tst:TRICE32_1 %u\n", 4000000001);
            TRICE32_2(Id( 42047), "tst:TRICE32_2 %u %u\n", 4000000001, 4000000002);
            TRICE32_3(Id( 42536), "tst:TRICE32_3 %u %u %u\n", 4000000001, 4000000002, 4000000003);
            TRICE32_4(Id( 59395), "tst:TRICE32_4 %u %u %u %u\n", 4000000001, 4000000002, 4000000003, 4000000004);
            TRICE64_1(Id( 48635), "tst:TRICE64_1 %u\n", -1);
            TRICE64_2(Id( 64317), "tst:TRICE64_2 %u %u\n", -1, -2);
        break;
        default:
        break;
    }
}
