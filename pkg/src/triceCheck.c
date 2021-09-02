/*! \file triceCheck.c
\brief trices for tool evaluation
\details The trices are dumped as 32bit values into a 32 bit wide fifo.
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/

#include <string.h> // strlen

//#define TRICE_OFF // enable this line to disable trice code generation in this file object
#include "trice.h"

///////////////////////////////////////////////////////////////////////////////
// trice time measurement
//

//! triceRuntimeGeneratedStringUnbound can transfer runtime generated strings if TRICES_1 is not available.
TRICE_INLINE void triceRuntimeGeneratedStringUnbound( const char* s ){
    size_t len = strlen( s );
    char c1, c2, c3, c4, c5, c6, c7, c8;
    while( len ){
        switch( len ){
            case  0: return;
            case  1: c1=*s++;
                TRICE8_1( Id(65329), "%c", c1 ); return;
                //TRICE8_1( Id(65329), "%c", c1 ); return;
            case  2: c1=*s++; c2=*s++;
                TRICE8_2( Id(65279), "%c%c", c1, c2 ); return;
            case  3: c1=*s++; c2=*s++; c3=*s++;
                TRICE8_3( Id(65057), "%c%c%c", c1, c2, c3 ); return;
            case  4: c1=*s++; c2=*s++; c3=*s++; c4=*s++;
                TRICE8_4( Id(65052), "%c%c%c%c", c1, c2, c3, c4 ); return;
            case  5: c1=*s++; c2=*s++; c3=*s++; c4=*s++; c5=*s++;
                TRICE8_5( Id(65088), "%c%c%c%c%c", c1, c2, c3, c4, c5 ); return;
            case  6: c1=*s++; c2=*s++; c3=*s++; c4=*s++; c5=*s++; c6=*s++;
                TRICE8_6( Id(65473), "%c%c%c%c%c%c", c1, c2, c3, c4, c5, c6 ); return;
            case  7: c1=*s++; c2=*s++; c3=*s++; c4=*s++; c5=*s++; c6=*s++; c7=*s++;
                TRICE8_7( Id(65121), "%c%c%c%c%c%c%c", c1, c2, c3, c4, c5, c6, c7); return;
            case  8:
            default: c1 = *s++; c2 = *s++; c3 = *s++; c4 = *s++; c5 = *s++; c6 = *s++; c7 = *s++; c8 = *s++;
                TRICE8_8( Id(65468), "%c%c%c%c%c%c%c%c", c1, c2, c3, c4, c5, c6, c7, c8 );
                len -= 8;
        }
    }
    return;
}

//! trice runtime string
#define TRICE_RTS(dynString) do{ triceRuntimeGeneratedStringUnbound(dynString); }while(0)
#define trice_rts TRICE_RTS

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
        TRICE_S( Id(65213), "%s\n", rts );
    }
}

//! triceCheckSet writes out all types of trices with fixed values for testing
//! \details One trice has one subtrace, if param size max 2 bytes. 
//! Traces with more bytes as parameter consist of several subtraces.
void triceCheckSet(int index) {
    switch (index) {
        case 0:
        break;
        case 1:
        break;
        case 2:
            #if TRICE_ENCODING == TRICE_FLEX_ENCODING
            Trice8( id(25308), "rd:Trice8 line %d\n", __LINE__ );
            Trice8i( id(17181), "rd:Trice8i line %d\n", __LINE__ );
            #endif
            trice8( Id( 64238), "rd:trice8 line %d\n", 1 );
            TRICE8( Id( 33304), "rd:TRICE8 line %d\n", 1 );
            
            #if TRICE_ENCODING == TRICE_FLEX_ENCODING
            Trice8( id(24616), "rd:Trice8 line %d, %d\n", __LINE__, 2 );
            Trice8i( id(21324), "rd:Trice8i line %d, %d\n", __LINE__, 2 );
            #endif
            trice8( Id( 62651), "rd:trice8 line %d, %d\n", __LINE__, 2 );
            TRICE8( Id( 44324), "rd:TRICE8 line %d, %d\n", __LINE__, 2 );
            
            #if TRICE_ENCODING == TRICE_FLEX_ENCODING
            Trice16(  id(  337), "ATT:Trice16  attention   message, SysTick is %6u\n", SYSTICKVAL16);
            Trice16i( id( 7132), "ATT:Trice16i attention   message, SysTick is %6u\n", SYSTICKVAL16);
            #endif
            trice16( Id( 35055), "ATT:trice16 attention   message, SysTick is %6u\n", SYSTICKVAL16);
            TRICE16( Id( 35199), "ATT:TRICE16 attention   message, SysTick is %6u\n", SYSTICKVAL16);
            
            trice32( Id( 39197),  "rd:trice32 line %d\n", __LINE__ );
            TRICE32( Id( 58039),  "rd:TRICE32 line %d\n", __LINE__ );
            
            trice64( Id( 55382),  "rd:trice64 %d\n", __LINE__ );
            TRICE64( Id( 50571),  "rd:TRICE64 %d\n", __LINE__ );
        
            trice32( Id( 51112),  "rd:trice32 line %d,%d\n", __LINE__, 2 );
            TRICE32( Id( 54294),  "rd:TRICE32 line %d,%d\n", __LINE__, 2 );
            
            trice64( Id( 53022),  "rd:trice64 line %d,%d\n", __LINE__, 2 );
            TRICE64( Id( 32800),  "rd:TRICE64 line %d,%d\n", __LINE__, 2 );

        break;
        case 3:
            TRICE8( Id( 54733), "rd:%d\n", 1 );
            TRICE8( Id( 43353), "rd:%d, %d\n", 1, 2 );
            TRICE8( Id( 41366), "rd:%d, %d, %d\n", 1, 2, 3 );
            TRICE8( Id( 33659), "rd:%d, %d, %d, %d\n", 1, 2, 3, 4 );
            TRICE8( Id( 38669), "rd:%d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
            TRICE8( Id( 55218), "rd:%d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
            TRICE8( Id( 37880), "rd:%d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
            TRICE8( Id( 45086), "rd:%d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );      
            TRICE16( Id( 45795), "rd:%d\n", 1 );
            TRICE16( Id( 53291), "rd:%d, %d\n", 1, 2 );
            TRICE16( Id( 64042), "rd:%d, %d, %d\n", 1, 2, 3 );
            TRICE16( Id( 33682), "rd:%d, %d, %d, %d\n", 1, 2, 3, 4 );
            TRICE32( Id( 34956), "rd:%d\n", 1 );
            TRICE32( Id( 45744), "rd:%d, %d\n", 1, 2 );
            TRICE32( Id( 62537), "rd:%d, %d, %d\n", 1, 2, 3 );
            TRICE32( Id( 62360), "rd:%d, %d, %d, %d\n", 1, 2, 3, 4 );
            TRICE64( Id( 58278), "rd:%d\n", 1 );
            TRICE64( Id( 58787), "rd:%d, %d\n", 1, 2 );

            TRICE8i( Id( 55609), "rd:%d\n", 1 );
            TRICE8i( Id( 51264), "rd:%d, %d\n", 1, 2 );
            TRICE8i( Id( 40040), "rd:%d, %d, %d\n", 1, 2, 3 );
            TRICE8i( Id( 63518), "rd:%d, %d, %d, %d\n", 1, 2, 3, 4 );
            TRICE8i( Id( 42137), "rd:%d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
            TRICE8i( Id( 40236), "rd:%d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
            TRICE8i( Id( 55537), "rd:%d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
            TRICE8i( Id( 59913), "rd:%d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );      
            TRICE16i( Id( 33989), "rd:%d\n", 1 );
            TRICE16i( Id( 39935), "rd:%d, %d\n", 1, 2 );
            TRICE16i( Id( 35916), "rd:%d, %d, %d\n", 1, 2, 3 );
            TRICE16i( Id( 37353), "rd:%d, %d, %d, %d\n", 1, 2, 3, 4 );
            TRICE32i( Id( 50677), "rd:%d\n", 1 );
            TRICE32i( Id( 39907), "rd:%d, %d\n", 1, 2 );
            TRICE32i( Id( 62644), "rd:%d, %d, %d\n", 1, 2, 3 );
            TRICE32i( Id( 41613), "rd:%d, %d, %d, %d\n", 1, 2, 3, 4 );
            TRICE64i( Id( 38727), "rd:%d\n", 1 );
            TRICE64i( Id( 39062), "rd:%d, %d\n", 1, 2 );
        break;
        case 4:
            trice8( Id( 47796), "rd:%d\n", 1 );
            trice8( Id( 65107), "rd:%d, %d\n", 1, 2 );
            trice8( Id( 50437), "rd:%d, %d, %d\n", 1, 2, 3 );
            trice8( Id( 54544), "rd:%d, %d, %d, %d\n", 1, 2, 3, 4 );
            trice8( Id( 55372), "rd:%d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
            trice8( Id( 62334), "rd:%d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
            trice8( Id( 53639), "rd:%d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
            trice8( Id( 64054), "rd:%d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );      
            trice16( Id( 42009), "rd:%d\n", 1 );
            trice16( Id( 57162), "rd:%d, %d\n", 1, 2 );
            trice16( Id( 42903), "rd:%d, %d, %d\n", 1, 2, 3 );
            trice16( Id( 59976), "rd:%d, %d, %d, %d\n", 1, 2, 3, 4 );
            trice32( Id( 64745), "rd:%d\n", 1 );
            trice32( Id( 56533), "rd:%d, %d\n", 1, 2 );
            trice32( Id( 64908), "rd:%d, %d, %d\n", 1, 2, 3 );
            trice32( Id( 54250), "rd:%d, %d, %d, %d\n", 1, 2, 3, 4 );
            trice64( Id( 59745), "rd:%d\n", 1 );
            trice64( Id( 42481), "rd:%d, %d\n", 1, 2 );

            trice8i( Id( 50930), "rd:%d\n", 1 );
            trice8i( Id( 58541), "rd:%d, %d\n", 1, 2 );
            trice8i( Id( 48322), "rd:%d, %d, %d\n", 1, 2, 3 );
            trice8i( Id( 52309), "rd:%d, %d, %d, %d\n", 1, 2, 3, 4 );
            trice8i( Id( 55637), "rd:%d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
            trice8i( Id( 47069), "rd:%d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
            trice8i( Id( 61677), "rd:%d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
            trice8i( Id( 51484), "rd:%d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );      
            trice16i( Id( 53154), "rd:%d\n", 1 );
            trice16i( Id( 64268), "rd:%d, %d\n", 1, 2 );
            trice16i( Id( 34148), "rd:%d, %d, %d\n", 1, 2, 3 );
            trice16i( Id( 33899), "rd:%d, %d, %d, %d\n", 1, 2, 3, 4 );
            trice32i( Id( 51091), "rd:%d\n", 1 );
            trice32i( Id( 51156), "rd:%d, %d\n", 1, 2 );
            trice32i( Id( 33655), "rd:%d, %d, %d\n", 1, 2, 3 );
            trice32i( Id( 52233), "rd:%d, %d, %d, %d\n", 1, 2, 3, 4 );
            trice64i( Id( 52986), "rd:%d\n", 1 );
            trice64i( Id( 41911), "rd:%d, %d\n", 1, 2 );
        break;
        case 5:
            trice0(Id( 36646), "--------------------------------------------------\n");
            trice0(Id( 51718), "--------------------------------------------------\n");
            trice16_1(Id( 60271), "dbg:12345 as 16bit is %#016b\n", 12345);
            trice0(Id( 43956), "--------------------------------------------------\n");
            trice0(Id( 56427), "sig:This ASSERT error is just a demo and no real error:\n");
            trice0(Id( 64938), "--------------------------------------------------\n");

            TRICE0(Id(65304), "--------------------------------------------------\n");
            TRICE0(Id(65304), "--------------------------------------------------\n");
            TRICE16_1(Id(65168), "dbg:12345 as 16bit is %#016b\n", 12345);
            TRICE0(Id(65304), "--------------------------------------------------\n");
            TRICE0(Id(65010), "sig:This ASSERT error is just a demo and no real error:\n");
            TRICE0(Id(65304), "--------------------------------------------------\n");
            trice16_1(Id( 62064), "ERR:error       message, SysTick is %6u\n", SYSTICKVAL16);
            trice16_1(Id( 39944), "WRN:warning     message, SysTick is %6u\n", SYSTICKVAL16);
            trice16_1(Id( 39100), "ATT:attention   message, SysTick is %6u\n", SYSTICKVAL16);
            trice16_1(Id( 44117), "DIA:diagnostics message, SysTick is %6u\n", SYSTICKVAL16);
            trice16_1(Id( 49379), "TIM:timing      message, SysTick is %6u\n", SYSTICKVAL16);
            trice16_1(Id( 35238), "DBG:debug       message, SysTick is %6u\n", SYSTICKVAL16);
            trice16_1(Id( 44967), "SIG:signal      message, SysTick is %6u\n", SYSTICKVAL16);
            trice16_1(Id( 42601), "RD:read         message, SysTick is %6u\n", SYSTICKVAL16);
            trice16_1(Id( 55715), "WR:write        message, SysTick is %6u\n", SYSTICKVAL16);
            trice16_1(Id( 43733), "ISR:interrupt   message, SysTick is %6u\n", SYSTICKVAL16);
            trice16_1(Id( 36351), "MSG:normal      message, SysTick is %6u\n", SYSTICKVAL16);
            trice16_1(Id( 45671), "INFO:informal   message, SysTick is %6u\n", SYSTICKVAL16);

            TRICE16_1(Id(62535), "ERR:error       message, SysTick is %6u\n", SYSTICKVAL16);
            TRICE16_1(Id(10761), "WRN:warning     message, SysTick is %6u\n", SYSTICKVAL16);
            TRICE16_1(Id(17536), "ATT:attention   message, SysTick is %6u\n", SYSTICKVAL16);
            TRICE16_1(Id(45064), "DIA:diagnostics message, SysTick is %6u\n", SYSTICKVAL16);
            TRICE16_1(Id(59225), "TIM:timing      message, SysTick is %6u\n", SYSTICKVAL16);
            TRICE16_1(Id( 8101), "DBG:debug       message, SysTick is %6u\n", SYSTICKVAL16);
            TRICE16_1(Id(24194), "SIG:signal      message, SysTick is %6u\n", SYSTICKVAL16);
            TRICE16_1(Id(61024), "RD:read         message, SysTick is %6u\n", SYSTICKVAL16);
            TRICE16_1(Id(34882), "WR:write        message, SysTick is %6u\n", SYSTICKVAL16);
            TRICE16_1(Id(31820), "ISR:interrupt   message, SysTick is %6u\n", SYSTICKVAL16);
            TRICE16_1(Id(57007), "MSG:normal      message, SysTick is %6u\n", SYSTICKVAL16);
            TRICE16_1(Id(13390), "INFO:informal   message, SysTick is %6u\n", SYSTICKVAL16);
            break;
        case 6:
            trice16_1(Id(21430), "tst:trice16_1   message, SysTick is %6u\n", SYSTICKVAL16);
            trice16_1(Id(21430), "tst:trice16_1   message, SysTick is %6u\n", SYSTICKVAL16);
            trice16_1(Id( 40252), "tst:trice16_1   message, SysTick is %6u\n", SYSTICKVAL16);
            trice16_1(Id(21430), "tst:trice16_1   message, SysTick is %6u\n", SYSTICKVAL16);
            trice32_1(Id(65462), "tst:trice32_1   message, SysTick is %6d\n", SYSTICKVAL16);
            trice32_1(Id( 55290), "tst:trice32_1   message, SysTick is %6d\n", SYSTICKVAL16);
            trice32_1(Id( 33633),"tst:trice32_1   message, SysTick is %6d\n", SYSTICKVAL16);
            trice32_1(Id(65462), "tst:trice32_1   message, SysTick is %6d\n", SYSTICKVAL16);

            TRICE16_1(Id(10813), "time:TRICE16_1   message, SysTick is %6u\n", SYSTICKVAL16);
            TRICE16_1(Id(10813), "time:TRICE16_1   message, SysTick is %6u\n", SYSTICKVAL16);
            TRICE16_1(Id(10813), "time:TRICE16_1   message, SysTick is %6u\n", SYSTICKVAL16);
            TRICE16_1(Id(10813), "time:TRICE16_1   message, SysTick is %6u\n", SYSTICKVAL16);
            TRICE32_1(Id(28071), "time:TRICE32_1   message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE32_1(Id(28071), "time:TRICE32_1   message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE32_1(Id(28071), "time:TRICE32_1   message, SysTick is %6d\n", SYSTICKVAL16);
            TRICE32_1(Id(28071), "time:TRICE32_1   message, SysTick is %6d\n", SYSTICKVAL16);

            trice8_4( Id(65493), "tst:trice8_4  %%03x ->  %03x  %03x  %03x  %03x\n", 1, 0x7f, 0x80, 0xff);
            trice8_4( Id(65400), "tst:trice8_4   %%4d -> %4d %4d %4d %4d\n", 1, 0x7f, 0x80, 0xff);
            trice8_4( Id(65137), "tst:trice8_4   %%4o -> %4o %4o %4o %4o\n", 1, 0x7f, 0x80, 0xff);
            trice16_4(Id(65447), "tst:trice16_4  %%05x ->   %05x   %05x   %05x   %05x\n", 1, 0x7fff, 0x8000, 0xffff);
            trice16_4(Id(65498), "tst:trice16_4   %%6d ->  %6d  %6d  %6d  %6d\n", 1, 0x7fff, 0x8000, 0xffff);
            trice16_4(Id(65418), "tst:trice16_4   %%7o -> %7o %7o %7o %7o\n", 1, 0x7fff, 0x8000, 0xffff);
            trice32_4(Id(65507), "tst:trice32_4 %%09x ->      %09x      %09x       %09x     %09x\n", 1, 0x7fffffff, 0x80000000, 0xffffffff); // 4
            trice32_4(Id(65305), "tst:trice32_4 %%10d ->     %10d     %10d     %10d    %10x\n", 1, 0x7fffffff, 0x80000000, 0xffffffff); // 4
            trice64_1(Id(65251), "att:trice64_1 %#b\n", 0x1122334455667788);
           
            TRICE8_4( Id(65492), "tst:TRICE8_4  %%03x ->  %03x  %03x  %03x  %03x\n", 1, 0x7f, 0x80, 0xff);
            TRICE8_4( Id(65331), "tst:TRICE8_4   %%4d -> %4d %4d %4d %4d\n", 1, 0x7f, 0x80, 0xff);
            TRICE8_4( Id(65103), "tst:TRICE8_4   %%4o -> %4o %4o %4o %4o\n", 1, 0x7f, 0x80, 0xff);
            TRICE16_4(Id(65055), "tst:TRICE16_4  %%05x ->   %05x   %05x   %05x   %05x\n", 1, 0x7fff, 0x8000, 0xffff);
            TRICE16_4(Id(65077), "tst:TRICE16_4   %%6d ->  %6d  %6d  %6d  %6d\n", 1, 0x7fff, 0x8000, 0xffff);
            TRICE16_4(Id(65060), "tst:TRICE16_4   %%7o -> %7o %7o %7o %7o\n", 1, 0x7fff, 0x8000, 0xffff);
            TRICE32_4(Id(65510), "tst:TRICE32_4 %%09x ->      %09x      %09x       %09x     %09x\n", 1, 0x7fffffff, 0x80000000, 0xffffffff); // 4
            TRICE32_4(Id(65066), "tst:TRICE32_4 %%10d ->     %10d     %10d     %10d    %10x\n", 1, 0x7fffffff, 0x80000000, 0xffffffff); // 4
            TRICE64_1(Id(65396), "att:TRICE64_1 %#b\n", 0x1122334455667788);
        break;
        case 7:
            trice8_1(Id(10407), "tst:trice8_1 %02X\n", 0xA1);
            trice8_2(Id( 6214), "tst:trice8_2 %02X %02X\n", 0xA1, 0xA2);
            trice8_3(Id(28450), "tst:trice8_3 %02X %02X %02X\n", 0xA1, 0xA2, 0xA3);
            trice8_4(Id(10650), "tst:trice8_4 %02X %02X %02X %02X\n", 0xA1, 0xA2, 0xA3, 0xA4);
            trice8_5(Id(64819), "tst:trice8_5 %02X %02X %02X %02X %02X\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5 );
            trice8_6(Id(43916), "tst:trice8_6 %02X %02X %02X %02X %02X %02X\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6);
            trice8_7(Id(31759), "tst:trice8_7 %02X %02X %02X %02X %02X %02X %02X\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7);
            trice8_8(Id(62238), "tst:trice8_8 %02X %02X %02X %02X %02X %02X %02X %02X\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8);

            TRICE8_1(Id(52174), "tst:TRICE8_1 %02x\n", 0xA1);
            TRICE8_2(Id(20096), "tst:TRICE8_2 %02x %02x\n", 0xA1, 0xA2);
            TRICE8_3(Id(51119), "tst:TRICE8_3 %02x %02x %02x\n", 0xA1, 0xA2, 0xA3);
            TRICE8_4(Id(61488), "tst:TRICE8_4 %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4);
            TRICE8_5(Id(53200), "tst:TRICE8_5 %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5 );
            TRICE8_6(Id(21678), "tst:TRICE8_6 %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6);
            TRICE8_7(Id(22308), "tst:TRICE8_7 %02x %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7);
            TRICE8_8(Id(65460), "tst:TRICE8_8 %02x %02x %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8);
        break;
        case 8:
            trice8_1(Id(65112), "tst:trice8_1 %d\n", -111);
            trice8_2(Id(65442), "tst:trice8_2 %d %d\n", -111, -122);
            trice8_3(Id(65115), "tst:trice8_3 %d %d %d\n", -111, -122, -3);
            trice8_4(Id(65159), "tst:trice8_4 %d %d %d %d\n", -111, -122, -3, -4);
            trice8_5(Id(65503), "tst:trice8_5 %d %d %d %d %d\n", -111, -122, -3, -4, -5);
            trice8_6(Id(65495), "tst:trice8_6 %d %d %d %d %d %d\n", -111, -122, -3, -4, -5, -6);
            trice8_7(Id(65031), "tst:trice8_7 %d %d %d %d %d %d %d\n", -111, -122, -3, -4, -5, -6, -7);
            trice8_8(Id(65523), "tst:trice8_8 %d %d %d %d %d %d %d %d\n", -111, -122, -3, -4, -5, -6, -7, -8);
        
            TRICE8_1(Id(65409), "tst:TRICE8_1 %d\n", -111);
            TRICE8_2(Id(65308), "tst:TRICE8_2 %d %d\n", -111, -222);
            TRICE8_3(Id(65454), "tst:TRICE8_3 %d %d %d\n", -111, -222, -3);
            TRICE8_4(Id(65013), "tst:TRICE8_4 %d %d %d %d\n", -111, -222, -3, -4);
            TRICE8_5(Id(65239), "tst:TRICE8_5 %d %d %d %d %d\n", -111, -222, -3, -4, -5);
            TRICE8_6(Id(65372), "tst:TRICE8_6 %d %d %d %d %d %d\n", -111, -222, -3, -4, -5, -6);
            TRICE8_7(Id(65246), "tst:TRICE8_7 %d %d %d %d %d %d %d\n", -111, -222, -3, -4, -5, -6, -7);
            TRICE8_8(Id(65264), "tst:TRICE8_8 %d %d %d %d %d %d %d %d\n", -111, -222, -3, -4, -5, -6, -7, -8);
            break;
        case 9:
            trice16_1(Id(65424), "tst:trice16_1 %d\n", -111); // 1
            trice16_2(Id(65089), "tst:trice16_2 %d %d\n", -111, -222); // 2
            trice16_3(Id(65437), "tst:trice16_3 %d %d %d\n", -111, -222, -333); // 3
            trice16_4(Id(65144), "tst:trice16_4 %d %d %d %d\n", -111, -222, -333, -444); // 4

            TRICE16_1(Id(65416), "tst:TRICE16_1 %d\n", -111); // 1
            TRICE16_2(Id(65412), "tst:TRICE16_2 %d %d\n", -111, -222); // 2
            TRICE16_3(Id(65017), "tst:TRICE16_3 %d %d %d\n", -111, -222, -333); // 3
            TRICE16_4(Id(65117), "tst:TRICE16_4 %d %d %d %d\n", -111, -222, -333, -444); // 4

            trice32_1(Id(65300), "tst:trice32_1 %d\n", -111); // 2
            trice32_2(Id(65485), "tst:trice32_2 %d %d\n", -111, -222); // 4
            trice32_3(Id(65533), "tst:trice32_3 %d %d %d\n", -111, -222, -333); // 4
            trice32_4(Id(65369), "tst:trice32_4 %d %d %d %d\n", -111, -222, -333, -444); // 4
            trice32_1(Id(65094), "tst:trice32_1 %08x\n", 0x0123cafe); // 2
            trice32_2(Id(65525), "tst:trice32_2 %x %x\n", 0xfeedcafe, 0xcafe0123); // 4
            trice32_3(Id(65428), "tst:trice32_3 %x %x %x\n", 0xaa001122, 0xbb112233, 0xcc223344); // 4
            trice32_4(Id(65143), "tst:trice32_4 %x %x %x %x\n", 0x12345678, 0x9abcdef0, 0xfedcba98, 0x87654321); // 4
        
            TRICE32_1(Id(65048), "tst:TRICE32_1 %08x\n", 0x0123cafe); // 2
            TRICE32_1(Id(65529), "tst:TRICE32_1 %d\n", -111); // 2
            TRICE32_2(Id(65318), "tst:TRICE32_2 %x %x\n", -111, -222); // 4
            TRICE32_2(Id(65021), "tst:TRICE32_2 %d %d\n", -111, -222); // 4
            TRICE32_3(Id(65073), "tst:TRICE32_3 %x %x %x\n", -111, -222, -333); // 4
            TRICE32_3(Id(65132), "tst:TRICE32_3 %d %d %d\n", -111, -222, -333); // 4
            TRICE32_4(Id(65283), "tst:TRICE32_4 %x %x %x %x\n", -111, -222, -333, -444); // 4
            TRICE32_4(Id(65450), "tst:TRICE32_4 %d %d %d %d\n", -111, -222, -333, -444); // 4

            trice64_1(Id(65299), "tst:trice64_1 %d\n", -111); // 2
            trice64_2(Id(65406), "tst:trice64_2 %d %d\n", -111, -222); // 4
            trice64_1(Id(65003), "tst:trice64_1 %x\n", 0x8765432112345678); // 2
            trice64_2(Id(65262), "tst:trice64_2 %x %x\n", 0x13579bdffdb97531, 0xeca8642002468ace); // 4
            
            TRICE64_1(Id(65528), "tst:TRICE64_1 %d\n", -111); // 2
            TRICE64_2(Id(65391), "tst:TRICE64_2 %d %d\n", -111, -222); // 4
            break;
        case 10:
            trice0(Id( 36633), "e:A");
            trice0(Id( 35689), "w:B");
            trice0(Id( 34130), "a:c");
            trice0(Id( 46494), "wr:d");
            trice0(Id( 51208), "rd:e\n");
            trice0(Id( 52191), "diag:f");
            trice0(Id( 63466), "d:G");
            trice0(Id( 51003), "t:H");
            trice0(Id( 36379), "time:i");
            trice0(Id( 37773), "message:J");
            trice0(Id( 56998), "dbg:k\n");                  
        
            TRICE0(Id(65184), "e:A");
            TRICE0(Id(65042), "w:B");
            TRICE0(Id(65167), "a:c");
            TRICE0(Id(65443), "wr:d");
            TRICE0(Id(65367), "rd:e\n");
            TRICE0(Id(65090), "diag:f");
            TRICE0(Id(65201), "d:G");
            TRICE0(Id(65517), "t:H");
            TRICE0(Id(65041), "time:i");
            TRICE0(Id(65344), "message:J");
            TRICE0(Id(65314), "dbg:k\n");
            break;
        case 11:
            trice0(Id( 51375), "1");
            trice0(Id( 38569), "2");
            trice0(Id( 34274), "3");
            trice0(Id( 65459), "4");
            trice0(Id( 38013), "e:7");
            trice0(Id( 62317), "m:12");
            trice0(Id( 43675), "m:123\n");

            TRICE0(Id(65228), "1");
            TRICE0(Id(65161), "2");
            TRICE0(Id(65109), "3");
            TRICE0(Id(65219), "4");
            TRICE0(Id(65046), "e:7");
            TRICE0(Id(65005), "m:12");
            TRICE0(Id(65385), "m:123\n");
            break;
        case 12:
            trice8_1(Id(65112), "tst:trice8_1 %d\n", 1);
            trice8_2(Id(65442), "tst:trice8_2 %d %d\n", 1, 2);
            trice8_3(Id(65115), "tst:trice8_3 %d %d %d\n", 1, 2, 3);
            trice8_4(Id(65159), "tst:trice8_4 %d %d %d %d\n", 1, 2, 3, 4);
            trice8_5(Id(65503), "tst:trice8_5 %d %d %d %d %d\n", 1, 2, 3, 4, 5);
            trice8_6(Id(65495), "tst:trice8_6 %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6);
            trice8_7(Id(65031), "tst:trice8_7 %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7);
            trice8_8(Id(65523), "tst:trice8_8 %d %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7, 8);
        
            TRICE8_1(Id(65409), "tst:TRICE8_1 %d\n", 1);
            TRICE8_2(Id(65308), "tst:TRICE8_2 %d %d\n", 1, 2);
            TRICE8_3(Id(65454), "tst:TRICE8_3 %d %d %d\n", 1, 2, 3);
            TRICE8_4(Id(65013), "tst:TRICE8_4 %d %d %d %d\n", 1, 2, 3, 4);
            TRICE8_5(Id(65239), "tst:TRICE8_5 %d %d %d %d %d\n", 1, 2, 3, 4, 5);
            TRICE8_6(Id(65372), "tst:TRICE8_6 %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6);
            TRICE8_7(Id(65246), "tst:TRICE8_7 %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7);
            TRICE8_8(Id(65264), "tst:TRICE8_8 %d %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7, 8);
            break;
        case 13:
            trice_rts( "an_example_string\n" );
            trice_rts( "" );
            trice_rts( "\n" );
            trice_rts( "a\n" );
            trice_rts( "an\n" );
            trice_rts( "an_\n" );
            trice_rts( "an_e\n" );
            trice_rts( "an_ex\n" );
            trice_rts( "an_exa\n" );
            trice_rts( "an_exam\n" );
            trice_rts( "an_examp\n" );
            trice_rts( "an_exampl\n" );
            trice_rts( "an_example\n" );
            trice_rts( "an_example_\n" );
            trice_rts( "an_example_s\n" );
            trice_rts( "an_example_st\n" );
            trice_rts( "an_example_str\n" );
            trice_rts( "an_example_stri\n" );
            trice_rts( "an_example_strin\n" );
            trice_rts( "an_example_string\n" );
        
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
			triceRuntimeStrings(257, 259);
           break;
        case 19:
#ifdef ENCRYPT
        {
            uint8_t b[8] = {1,2,3,4,5,6,7,8};
            TRICE8_8(  Id(65083), "msg: message = %03x %03x %03x %03x %03x %03x %03x %03x\n", b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7] );
            TRICE16_1( Id(65054), "tim: pre encryption SysTick=%d\n", SYSTICKVAL16 );
            encrypt(b);
            TRICE16_1( Id(65330), "tim: post encryption SysTick=%d\n", SYSTICKVAL16 );
            TRICE8_8(  Id(65484), "att: encrypted = %03x %03x %03x %03x %03x %03x %03x %03x\n", b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7] );
            TRICE16_4( Id(65526), "att: encrypted = %d,%d,%d,%d,", b[0], b[1], b[2], b[3] );
            TRICE16_4( Id(65061), "att: %d,%d,%d,%d\n", b[4], b[5], b[6], b[7] );
            #ifdef DECRYPT
            TRICE16_1( Id(65405), "tim: pre decryption SysTick=%d\n", SYSTICKVAL16 );
            decrypt(b);
            TRICE16_1( Id(65051), "tim: post decryption SysTick=%d\n", SYSTICKVAL16 );
            #endif
            TRICE8_8(  Id(65281), "msg: messge = %03x %03x %03x %03x %03x %03x %03x %03x\n", b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7] );
        }
        TRICE0 (Id(65029), "--------------------------------------------------\n\n" );
#endif
        break;
        case 20:
            #if TRICE_ENCODING == TRICE_FLEX_ENCODING
            TRICE8_8(Id(  579), "tst:TRICE8_1 %%d=%d, %%u=%u, 0x%%x=0x%x, 0x%%2x=0x%2x, 0x%%02x=0x%02x, 0x%%3x=0x%3x, 0x%%03x=0x%03x, %%b=%b\n", 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81);
            TRICE16_1(Id(21394), "tst:TRICE16_1 0x%04x\n", 0xa0);
            TRICE8_1(Id(21201), "tst:TRICE8_1 0x%02x\n", 0xa0);
            TRICE8_1(Id(21201), "tst:TRICE8_1 0x%02x\n", 0xa0);
            TRICE8_1(Id(32742), "tst:TRICE8_1 %u\n", 201);
            TRICE8_2(Id( 7473), "tst:TRICE8_2 %u %u\n", 201, 202);
            TRICE8_3(Id(14014), "tst:TRICE8_3 %u %u %u\n", 201, 202, 203);
            TRICE8_4(Id(31376), "tst:TRICE8_4 %u %u %u %u\n", 201, 202, 203, 204);
            TRICE8_5(Id(41604), "tst:TRICE8_5 %u %u %u %u %u\n", 201, 202, 203, 204, 205);
            TRICE8_6(Id(24615), "tst:TRICE8_6 %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206);
            TRICE8_7(Id(55697), "tst:TRICE8_7 %u %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206, 207);
            TRICE8_8(Id(36547), "tst:TRICE8_8 %u %u %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206, 207, 208);
            TRICE16_1(Id(40865), "tst:TRICE16_1 %u\n", 60001);
            TRICE16_2(Id(30623), "tst:TRICE16_2 %u %u\n", 60001, 60002);
            TRICE16_3(Id(60735), "tst:TRICE16_3 %u %u %u\n", 60001, 60002, 60003);
            TRICE16_4(Id(12478), "tst:TRICE16_4 %u %u %u %u\n", 60001, 60002, 60003, 60004);
            TRICE32_1(Id(24267), "tst:TRICE32_1 %u\n", 4000000001);
            TRICE32_2(Id(38214), "tst:TRICE32_2 %u %u\n", 4000000001, 4000000002);
            TRICE32_3(Id(59229), "tst:TRICE32_3 %u %u %u\n", 4000000001, 4000000002, 4000000003);
            TRICE32_4(Id(38168), "tst:TRICE32_4 %u %u %u %u\n", 4000000001, 4000000002, 4000000003, 4000000004);
            TRICE64_1(Id(45250), "tst:TRICE64_1 %u\n", -1);
            TRICE64_2(Id(12051), "tst:TRICE64_2 %u %u\n", -1, -2);
        break;
        case 21:
            Trice0   ( id(26891), "wr: Trice0    short\n");
            Trice16_1( id(13520), "rd: Trice16_1 %u\n", 65000);
            Trice8_2 ( id(25873), "diag: Trice8_2  0x%02X, 0x%02x\n", 0xab, 0xcd);
            Trice16_1( id(30902), "time: Trice16_1 %6u\n", SYSTICKVAL16);
            Trice16_1( id(30902), "time: Trice16_1 %6u\n", SYSTICKVAL16);
            Trice16_1( id(30902), "time: Trice16_1 %6u\n", SYSTICKVAL16);
            Trice16_1( id(30902), "time: Trice16_1 %6u\n", SYSTICKVAL16);
            Trice16_1( id(13520), "rd: Trice16_1 %u\n", 65000);
            Trice16_1( id(30902), "time: Trice16_1 %6u\n", SYSTICKVAL16);
            Trice16_1( id(13520), "rd: Trice16_1 %u\n", 65000);
            Trice16_1( id(30902), "time: Trice16_1 %6u\n", SYSTICKVAL16);
        
            Trice0i   ( id(19001), "wr: Trice0i   short\n");
            Trice16_1i( id( 5678), "rd: Trice16_1i %d\n", 65000);
            Trice8_2i ( id( 4500), "diag: Trice8_2  0x%02X, 0x%02x\n", 0xab, 0xcd);
            Trice16_1i( id(22909), "time: Trice16_1i %6u\n", SYSTICKVAL16);
            Trice16_1i( id(22909), "time: Trice16_1i %6u\n", SYSTICKVAL16);
            Trice16_1i( id(22909), "time: Trice16_1i %6u\n", SYSTICKVAL16);
            Trice16_1i( id(22909), "time: Trice16_1i %6u\n", SYSTICKVAL16);
            Trice16_1i( id(13551), "rd: Trice16_1i %u\n", 65000);
            Trice16_1i( id(22909), "time: Trice16_1i %6u\n", SYSTICKVAL16);
            Trice16_1i( id(13551), "rd: Trice16_1i %u\n", 65000);
            Trice16_1i( id(22909), "time: Trice16_1i %6u\n", SYSTICKVAL16);
            break;
        case 22:
            Trice0i( id(15380),     "msg: Trice0i    -> short  trice macro    (no   cycle counter) for only inside critical section\n" );
            TRICE0i( Id( 39412),    "msg: TRICE0i    -> normal trice macro    (with cycle counter) for only inside critical section\n" );
            trice0i( Id( 51084),    "msg: trice0i    -> normal trice function (with cycle counter) for only inside critical section\n" );
            TRICE0 ( Id( 65336),    "msg: TRICE0     -> normal trice macro    (with cycle counter) for everywhere\n" );
            Trice0 ( id(23242),     "msg: Trice0     -> short  trice macro    (no   cycle counter) for everywhere\n" );
            trice0 ( Id( 50329),    "msg: trice0     -> normal trice function (with cycle counter) for everywhere\n" );
            break;
        case 23:
             Trice8_1i( id(26825),  "msg:  Trice8_1i -> short  trice macro    (no   cycle counter) for only inside critical section %d\n", 111 );
             trice8_1i( Id( 50448), "msg:  trice8_1i -> normal trice function (with cycle counter) for only inside critical section %d\n", 111 );
             TRICE8_1i( Id( 59253), "msg:  TRICE8_1i -> normal trice macro    (with cycle counter) for only inside critical section %d\n", 111 );
            Trice16_1i( id(32337),  "msg: Trice16_1i -> short  trice macro    (no   cycle counter) for only inside critical section %d\n", 111 );
            TRICE16_1i( Id( 33067), "msg: TRICE16_1i -> normal trice macro    (with cycle counter) for only inside critical section %d\n", 111 );
            trice16_1i( Id( 60551), "msg: trice16_1i -> normal trice function (with cycle counter) for only inside critical section %d\n", 111 );
            TRICE32_1i( Id( 51658), "msg: TRICE32_1i -> normal trice macro    (with cycle counter) for only inside critical section %d\n", 111 );
            trice32_1i( Id( 59595), "msg: trice32_1i -> normal trice function (with cycle counter) for only inside critical section %d\n", 111 );
            TRICE64_1i( Id( 63098), "msg: TRICE64_1i -> normal trice macro    (with cycle counter) for only inside critical section %d\n", 111 );
            trice64_1i( Id( 39419), "msg: trice64_1i -> normal trice function (with cycle counter) for only inside critical section %d\n", 111 );
             Trice8_1 ( id( 9484),  "msg:  Trice8_1  -> short  trice macro    (no   cycle counter) for everywhere                   %u\n", 222 );
             TRICE8_1 ( Id( 42720), "msg:  TRICE8_1  -> normal trice macro    (with cycle counter) for everywhere                   %u\n", 222 );
             trice8_1 ( Id( 35941), "msg:  trice8_1  -> normal trice function (with cycle counter) for everywhere                   %u\n", (int8_t)222 );
            Trice16_1 ( id( 2457),  "msg: Trice16_1  -> short  trice macro    (no   cycle counter) for everywhere                   %u\n", 222 );
            TRICE16_1 ( Id( 39892), "msg: TRICE16_1  -> normal trice macro    (with cycle counter) for everywhere                   %u\n", 222 );
            trice16_1 ( Id( 38837), "msg: trice16_1  -> normal trice function (with cycle counter) for everywhere                   %u\n", 222 );
            TRICE32_1 ( Id( 44960), "msg: TRICE32_1  -> normal trice macro    (with cycle counter) for everywhere                   %u\n", 222 );
            trice32_1 ( Id( 43451), "msg: trice32_1  -> normal trice function (with cycle counter) for everywhere                   %u\n", 222 );
            TRICE64_1 ( Id( 63361), "msg: TRICE64_1  -> normal trice macro    (with cycle counter) for everywhere                   %u\n", 222 );
            trice64_1 ( Id( 64447), "msg: trice64_1  -> normal trice function (with cycle counter) for everywhere                   %u\n", 222 );
            break;
        case 24:
            TRICE8_2i( Id( 54079),  "msg:  TRICE8_2i -> normal trice macro    (with cycle counter) for only inside critical section %x %X\n", 0x1a, 0x2a );
            trice8_2i( Id( 47820),  "msg:  trice8_2i -> normal trice function (with cycle counter) for only inside critical section %x %X\n", 0x1a, 0x2a );
            TRICE16_2i( Id( 62499), "msg: TRICE16_2i -> normal trice macro    (with cycle counter) for only inside critical section %x %X\n", 0x1a, 0x2a );
            trice16_2i( Id( 60229), "msg: trice16_2i -> normal trice function (with cycle counter) for only inside critical section %x %X\n", 0x1234, 0x1234 );
            TRICE32_2i( Id( 60145), "msg: TRICE32_2i -> normal trice macro    (with cycle counter) for only inside critical section %x %X\n", 0x1234, 0x1234 );
            trice32_2i( Id( 55780), "msg: trice32_2i -> normal trice function (with cycle counter) for only inside critical section %x %X\n", 0x11223344, 0x55667788 );
            TRICE64_2i( Id( 57939), "msg: TRICE64_2i -> normal trice macro    (with cycle counter) for only inside critical section %x %X\n", 0x1122334455667788, -0x1122334455667788 );
            trice64_2i( Id( 41845), "msg: trice64_2i -> normal trice function (with cycle counter) for only inside critical section %x %X\n", 0x1122334455667788, -0x1122334455667788 );
            TRICE8_2( Id( 35213),   "msg:  TRICE8_2  -> normal trice macro    (with cycle counter) for everywhere                   %x %X\n", 0x1a, 0x2a );
            trice8_2( Id( 45214),   "msg:  trice8_2  -> normal trice function (with cycle counter) for everywhere                   %x %X\n", 0x1a, 0x2a );
            TRICE16_2( Id( 50757),  "msg: TRICE16_2  -> normal trice macro    (with cycle counter) for everywhere                   %x %X\n", 0x1234, 0x1234 );
            trice16_2( Id( 45348),  "msg: trice16_2  -> normal trice function (with cycle counter) for everywhere                   %x %X\n", 0x1234, 0x1234 );
            TRICE32_2( Id( 47530),  "msg: TRICE32_2  -> normal trice macro    (with cycle counter) for everywhere                   %x %X\n", 0x11223344, 0x55667788 );
            trice32_2( Id( 64622),  "msg: trice32_2  -> normal trice function (with cycle counter) for everywhere                   %x %X\n", 0x11223344, 0x55667788 );
            TRICE64_2( Id( 38736),  "msg: TRICE64_2  -> normal trice macro    (with cycle counter) for everywhere                   %x %X\n", 0x1122334455667788, -0x1122334455667788 );
            trice64_2( Id( 35036),  "msg: trice64_2  -> normal trice function (with cycle counter) for everywhere                   %x %X\n", 0x1122334455667788, -0x1122334455667788 );
            break;
        case 25:
             TRICE8_3i( Id( 59167), "msg:  TRICE8_3i -> normal trice macro    (with cycle counter) for only inside critical section %x %X %u\n", 0x1a,               0x1a,               0x2a );
             trice8_3i( Id( 63167), "msg:  trice8_3i -> normal trice function (with cycle counter) for only inside critical section %x %X %u\n", 0x1a,               0x1a,               0x2a );
            TRICE16_3i( Id( 41697), "msg: TRICE16_3i -> normal trice macro    (with cycle counter) for only inside critical section %x %X %u\n", 0x1a,               0x1a,               0x2a );
            trice16_3i( Id( 47464), "msg: trice16_3i -> normal trice function (with cycle counter) for only inside critical section %x %X %u\n", 0x1234,             0x1234,             0x1234 );
            TRICE32_3i( Id( 58025), "msg: TRICE32_3i -> normal trice macro    (with cycle counter) for only inside critical section %x %X %u\n", 0x1234,             0x1234,             0x1234 );
            trice32_3i( Id( 52613), "msg: trice32_3i -> normal trice function (with cycle counter) for only inside critical section %x %X %u\n", 0x11223344,         0x11223344,         0x55667788 );
             TRICE8_3( Id( 51048),  "msg:  TRICE8_3  -> normal trice macro    (with cycle counter) for everywhere                   %x %X %u\n", 0x1a,               0x1a,               0x2a );
             trice8_3( Id( 62466),  "msg:  trice8_3  -> normal trice function (with cycle counter) for everywhere                   %x %X %u\n", 0x1a,               0x1a,               0x2a );
            TRICE16_3( Id( 48411),  "msg: TRICE16_3  -> normal trice macro    (with cycle counter) for everywhere                   %x %X %u\n", 0x1234,             0x1234,             0x1234 );
            trice16_3( Id( 34814),  "msg: trice16_3  -> normal trice function (with cycle counter) for everywhere                   %x %X %u\n", 0x1234,             0x1234,             0x1234 );
            TRICE32_3( Id( 37205),  "msg: TRICE32_3  -> normal trice macro    (with cycle counter) for everywhere                   %x %X %u\n", 0x11223344,         0x11223344,         0x55667788 );
            trice32_3( Id( 58845),  "msg: trice32_3  -> normal trice function (with cycle counter) for everywhere                   %x %X %u\n", 0x11223344,         0x11223344,         0x55667788 );
            break;
        case 26:
             TRICE8_4i( Id( 49458), "msg:  TRICE8_4i -> normal trice macro    (with cycle counter) for only inside critical section %x %X %u %d\n", 0x1a,               0x1a,               0x1a,               0x2a );
             trice8_4i( Id( 51750), "msg:  trice8_4i -> normal trice function (with cycle counter) for only inside critical section %x %X %u %d\n", 0x1a,               0x1a,               0x1a,               0x2a );
            TRICE16_4i( Id( 48203), "msg: TRICE16_4i -> normal trice macro    (with cycle counter) for only inside critical section %x %X %u %d\n", 0x1a,               0x1a,               0x1a,               0x2a );
            trice16_4i( Id( 47395), "msg: trice16_4i -> normal trice function (with cycle counter) for only inside critical section %x %X %u %d\n", 0x1234,             0x1234,             0x1234,             0x1234 );
            TRICE32_4i( Id( 41764), "msg: TRICE32_4i -> normal trice macro    (with cycle counter) for only inside critical section %x %X %u %d\n", 0x1234,             0x1234,             0x1234,             0x1234 );
            trice32_4i( Id( 45598), "msg: trice32_4i -> normal trice function (with cycle counter) for only inside critical section %x %X %u %d\n", 0x11223344,         0x11223344,         0x11223344,         0x55667788 );
             TRICE8_4( Id( 39086),  "msg:  TRICE8_4  -> normal trice macro    (with cycle counter) for everywhere                   %x %X %u %d\n", 0x1a,               0x1a,               0x1a,               0x2a );
             trice8_4( Id( 54569),  "msg:  trice8_4  -> normal trice function (with cycle counter) for everywhere                   %x %X %u %d\n", 0x1a,               0x1a,               0x1a,               0x2a );
            TRICE16_4( Id( 42270),  "msg: TRICE16_4  -> normal trice macro    (with cycle counter) for everywhere                   %x %X %u %d\n", 0x1234,             0x1234,             0x1234,             0x1234 );
            trice16_4( Id( 55892),  "msg: trice16_4  -> normal trice function (with cycle counter) for everywhere                   %x %X %u %d\n", 0x1234,             0x1234,             0x1234,             0x1234 );
            TRICE32_4( Id( 48416),  "msg: TRICE32_4  -> normal trice macro    (with cycle counter) for everywhere                   %x %X %u %d\n", 0x11223344,         0x11223344,         0x11223344,         0x55667788 );
            trice32_4( Id( 64296),  "msg: trice32_4  -> normal trice function (with cycle counter) for everywhere                   %x %X %u %d\n", 0x11223344,         0x11223344,         0x11223344,         0x55667788 );
             break;
        case 27:
             TRICE8_5i( Id( 56183), "msg:  TRICE8_5i -> normal trice macro    (with cycle counter) for only inside critical section %x %X %u %u %d\n", 0x1a, 0x2a, 0x3a, 0x4a, 0x5a );
             trice8_5i( Id( 45309), "msg:  trice8_5i -> normal trice function (with cycle counter) for only inside critical section %x %X %u %u %d\n", 0x1a, 0x2a, 0x3a, 0x4a, 0x5a );
             TRICE8_5( Id( 56218),  "msg:  TRICE8_5  -> normal trice macro    (with cycle counter) for everywhere                   %x %X %u %u %d\n", 0x1a, 0x2a, 0x3a, 0x4a, 0x5a );
             trice8_5( Id( 34176),  "msg:  trice8_5  -> normal trice function (with cycle counter) for everywhere                   %x %X %u %u %d\n", 0x1a, 0x2a, 0x3a, 0x4a, 0x5a );
             break;                
        case 28:                   
             TRICE8_6i( Id( 50770), "msg:  TRICE6_5i -> normal trice macro    (with cycle counter) for only inside critical section %x %X %u %d %x %X\n", 0x1a, 0x2a, 0x3a, 0x4a, 0x5a, 0x6a );
             trice8_6i( Id( 62387), "msg:  trice6_5i -> normal trice function (with cycle counter) for only inside critical section %x %X %u %d %x %X\n", 0x1a, 0x2a, 0x3a, 0x4a, 0x5a, 0x6a );
             TRICE8_6( Id( 42602),  "msg:  TRICE6_5  -> normal trice macro    (with cycle counter) for everywhere                   %x %X %u %d %x %X\n", 0x1a, 0x2a, 0x3a, 0x4a, 0x5a, 0x6a );
             trice8_6( Id( 55451),  "msg:  trice6_5  -> normal trice function (with cycle counter) for everywhere                   %x %X %u %d %x %X\n", 0x1a, 0x2a, 0x3a, 0x4a, 0x5a, 0x6a );
             break;                                                           
        case 29:                                                              
             TRICE8_7i( Id( 62924), "msg:  TRICE8_7i -> normal trice macro    (with cycle counter) for only inside critical section %x %X %u %d %x %X %u\n", 0x1a, 0x2a, 0x3a, 0x4a, 0x5a, 0x6a, 0x7a );
             trice8_7i( Id( 39375), "msg:  trice8_7i -> normal trice function (with cycle counter) for only inside critical section %x %X %u %d %x %X %u\n", 0x1a, 0x2a, 0x3a, 0x4a, 0x5a, 0x6a, 0x7a );
             TRICE8_7( Id( 45848),  "msg:  TRICE8_7  -> normal trice macro    (with cycle counter) for everywhere                   %x %X %u %d %x %X %u\n", 0x1a, 0x2a, 0x3a, 0x4a, 0x5a, 0x6a, 0x7a );
             trice8_7( Id( 56442),  "msg:  trice8_7  -> normal trice function (with cycle counter) for everywhere                   %x %X %u %d %x %X %u\n", 0x1a, 0x2a, 0x3a, 0x4a, 0x5a, 0x6a, 0x7a );
             break;                                                           
        case 30:                                                              
             TRICE8_8i( Id( 37386), "msg:  TRICE8_8i -> normal trice macro    (with cycle counter) for only inside critical section %x %X %u %d %x %X %u %d\n", 0x1a, 0x2a, 0x3a, 0x4a, 0x5a, 0x6a, 0x7a, 0x8a );
             trice8_8i( Id( 47472), "msg:  trice8_8i -> normal trice function (with cycle counter) for only inside critical section %x %X %u %d %x %X %u %d\n", 0x1a, 0x2a, 0x3a, 0x4a, 0x5a, 0x6a, 0x7a, 0x8a );
             TRICE8_8( Id( 64951),  "msg:  TRICE8_8  -> normal trice macro    (with cycle counter) for everywhere                   %x %X %u %d %x %X %u %d\n", 0x1a, 0x2a, 0x3a, 0x4a, 0x5a, 0x6a, 0x7a, 0x8a );
             trice8_8( Id( 48424),  "msg:  trice8_8  -> normal trice function (with cycle counter) for everywhere                   %x %X %u %d %x %X %u %d\n", 0x1a, 0x2a, 0x3a, 0x4a, 0x5a, 0x6a, 0x7a, 0x8a );
             break;
        case 31:
            Trice16i( id( 5112), "tim:@tick %5u ", SYSTICKVAL16 );
            Trice8i( id(10240), "sig:task %u -> %u\n", 7, 3 );
            Trice16i( id(20413), "tim:@tick %5u ", SYSTICKVAL16 );
            Trice8i( id(11934), "sig:task %u -> %u\n", 3, 6 );
            Trice16i( id( 5645), "tim:@tick %5u ", SYSTICKVAL16 );
            Trice8i( id(14039), "sig:task %u -> %u\n", 6, 7 );
            break;
            #endif // #if TRICE_ENCODING == TRICE_FLEX_ENCODING
        default:
            break;
    }
}
