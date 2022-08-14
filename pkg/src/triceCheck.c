/*! \file triceCheck.c
\brief trices for tool evaluation
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/
#include <float.h>
//#define TRICE_OFF // enable this line to disable trice code generation in this file object
#include "trice.h"
#define TRICE_FILE Id(42698)

static int32_t FloatToInt32( float f );
static int64_t DoubleToInt64( double f );
static void exampleOfManualSerialization( void );
static void exampleOfManualJSONencoding(void);
static void exampleOfBuffersAndFunctions(void);

//! TriceCheckSet writes out all types of trices with fixed values for testing
//! \details One trice has one subtrace, if param size max 2 bytes. 
//! Traces with more bytes as parameter consist of several subtraces.
void TriceCheckSet(int index) {
    char* s = "AAAAAAAAAAAA";
    float  x = 1089.6082763671875; // 0x44883377
    double y = 518.0547492508867;  // 0x4080307020601050
     
    switch (index) {
        case 0:
        exampleOfManualSerialization();
        break;
        case 1:
        exampleOfManualJSONencoding();
        break;
        case 2:
        exampleOfBuffersAndFunctions();
        break;
        case 10:
            TRICE( Id(10753), "FATAL:magenta+b:red\n" );
            TRICE( Id(11320), "CRITICAL:red+i:default+h\n" );
            TRICE( Id(11066), "EMERGENCY:red+i:blue\n" );
            TRICE( Id(15633), "ERROR:11:red\n" );
            TRICE( Id(13026), "WARNING:11+i:red\n" );
            TRICE( Id(10242), "ATTENTION:11:green\n" );
            TRICE( Id(13116), "INFO:cyan+b:default+h\n" );
            TRICE( Id(15333), "DEBUG:130+i\n" );
            TRICE( Id(15711), "TRACE:default+i:default+h\n" );
            TRICE( Id(15172), "TIME:blue+i:blue+h\n" );
            TRICE( Id(15444), "MESSAGE:green+h:black\n" );
            TRICE( Id(12843), "READ:black+i:yellow+h\n" );
            TRICE( Id(14814), "WRITE:black+u:yellow+h\n" );
            TRICE( Id(11537), "RECEIVE:black+h:black\n" );
            TRICE( Id(13883), "TRANSMIT:black:black+h\n" );
            TRICE( Id(14103), "DIAG:yellow+i:default+h\n" );
            TRICE( Id(13662), "INTERRUPT:magenta+i:default+h\n" );
            TRICE( Id(13870), "SIGNAL:118+i\n" );
            TRICE( Id(11698), "TEST:yellow+h:black\n" );
            TRICE( Id(10303), "DEFAULT:off\n" );
            TRICE( Id(11240), "NOTICE:blue:white+h\n" );
            TRICE( Id(11307), "ALERT:magenta:magenta+h\n" );
            TRICE( Id(12949), "ASSERT:yellow+i:blue\n" );
            TRICE( Id(15875), "ALARM:red+i:white+h\n" );
            TRICE( Id(14339), "CYCLE:blue+i:default+h\n" );
            TRICE( Id(12329), "VERBOSE:blue:default\n" );
        break;
        case 20:
            TRICE( Id(14811), "fatal:magenta+b:red\n" );
            TRICE( Id(12957), "critical:red+i:default+h\n" );
            TRICE( Id(10100), "emergency:red+i:blue\n" );
            TRICE( Id(11498), "error:11:red\n" );
            TRICE( Id(14521), "warning:11+i:red\n" );
            TRICE( Id(15716), "attention:11:green\n" );
            TRICE( Id(11838), "info:cyan+b:default+h\n" );
            TRICE( Id(15584), "debug:130+i\n" );
            TRICE( Id(14155), "trace:default+i:default+h\n" );
            TRICE( Id(14191), "tme:blue+i:blue+h\n" );
            TRICE( Id(15957), "message:green+h:black\n" );
            TRICE( Id(10772), "read:black+i:yellow+h\n" );
            TRICE( Id(15997), "write:black+u:yellow+h\n" );
            TRICE( Id(12822), "receive:black+h:black\n" );
            TRICE( Id(15104), "transmit:black:black+h\n" );
            TRICE( Id(13190), "diag:yellow+i:default+h\n" );
            TRICE( Id(10389), "interrupt:magenta+i:default+h\n" );
            TRICE( Id(15415), "signal:118+i\n" );
            TRICE( Id(12020), "test:yellow+h:black\n" );
            TRICE( Id(15743), "default:off\n" );
            TRICE( Id(11419), "notice:blue:white+h\n" );
            TRICE( Id(14353), "allert:magenta:magenta+h\n" );
            TRICE( Id(11462), "assert:yellow+i:blue\n" );
            TRICE( Id(14775), "alarm:red+i:white+h\n" );
            TRICE( Id(11034), "cycle:blue+i:default+h\n" );
            TRICE( Id(10848), "verbose:blue:default\n" );
        break;
        case 30:
            TRICE( Id(13925), "dbg:Hi!\n" );
            TRICE( Id(12873), "dbg:\tHi!\n" );
            TRICE( Id(11937), "dbg:\t\tHi!\n" );
            TRICE( Id(14390), "dbg:\t\t\tHi!\n" );
            TRICE( Id(13706), "dbg:\\tHi!\n" );
            TRICE( Id(11473), "dbg:\aHi!\n" );
            TRICE( Id(13760), "dbg:\a\aHi!\n" );
            TRICE( Id(12252), "dbg:\a\a\aHi!\n" );
            TRICE( Id(13458), "dbg:\\aHi!\n" );
            //TCOBSCheck();
        break;
        case 40:{
            int len = strlen(s);
            TRICE32( Id(13167), "dbg:len=%u:", len );
            TRICE_S( Id(10411), "sig:TRICE_S=%s\n", s );
            TRICE32( Id(15242), "dbg:len=%u:", len );
            TRICE_S( Id(12413), "sig:TRICE_S=%s\n", s );
            TRICE32( Id(14884), "dbg:len=%u:", len );
            TRICE_S( Id(14097), "sig:TRICE_S=%s\n", s );
        
            TRICE32( Id(14156), "dbg:len=%u:", len);
            TRICE_N( Id(13478), "sig:TRICE_N=%s\n", s, len );
            TRICE32( Id(13663), "dbg:len=%u:", len);
            TRICE_N( Id(14439), "sig:TRICE_N=%s\n", s, len );
            TRICE32( Id(14234), "dbg:len=%u:", len);
            TRICE_N( Id(12485), "sig:TRICE_N=%s\n", s, len );
        
          //TRICE_S( Id(15177), "sig:TRICE_S=%s\n", "Hello String" );     // not allowed
          //TRICE_N( Id(12982), "sig:TRICE_N=%s\n", "Hello Buffer", 13 ); // not allowed

            TRICE( Id(10628), "rd:TRICE line %E (%%E)\n", aFloat(-555555555.5555555555) );
            TRICE( Id(14165), "rd:TRICE line %F (%%F)\n", aFloat(-555555555.5555555555) );
            TRICE( Id(11825), "rd:TRICE line %G (%%G)\n", aFloat(-555555555.5555555555) );
        }
        break;
        case 50:
            TRICE32_1( Id(14568), "rd:TRICE32_1 line %u (%%u)\n", -1 );
            TRICE32_1( Id(12079), "rd:TRICE32_1 line %b (%%b)\n", -1 );
            TRICE32_1( Id(10603), "rd:TRICE32_1 line %o (%%o)\n", -1 );
            TRICE32_1( Id(14783), "rd:TRICE32_1 line %O (%%O)\n", -1 );
            TRICE32_1( Id(12332), "rd:TRICE32_1 line %X (%%X)\n", -1 );
            TRICE32_1( Id(10305), "rd:TRICE32_1 line %x (%%x)\n", -1 );
            TRICE32_1( Id(15199), "rd:TRICE32_1 line %d (%%d)\n", -1 );
        break;
        case 60:
            TRICE64_1( Id(14832), "rd:TRICE64_1 line %u (%%u)\n", -1 );
            TRICE64_1( Id(13914), "rd:TRICE64_1 line %b (%%b)\n", -1 );
            TRICE64_1( Id(13373), "rd:TRICE64_1 line %o (%%o)\n", -1 );
            TRICE64_1( Id(13347), "rd:TRICE64_1 line %O (%%O)\n", -1 );
            TRICE64_1( Id(15607), "rd:TRICE64_1 line %X (%%C)\n", -1 );
            TRICE64_1( Id(11776), "rd:TRICE64_1 line %x (%%x)\n", -1 );
            TRICE64_1( Id(10025), "rd:TRICE64_1 line %d (%%d)\n", -1 );
        break;
        case 70:
            TRICE64( Id(10750), "rd:TRICE64 line %E (%%E)\n", aDouble(-555555555.5555555555) );
            TRICE64( Id(14461), "rd:TRICE64 line %F (%%F)\n", aDouble(-555555555.5555555555) );
            TRICE64( Id(11639), "rd:TRICE64 line %G (%%G)\n", aDouble(-555555555.5555555555) );
        break;
        case 80:
            TRICE( Id(15611), "sig:Some time measurements\n" );
            TRICE( Id(12971), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
            TRICE( Id(13608), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
            TRICE( Id(11813), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
            TRICE( Id(15325), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
            TRICE64( Id(14692), "rd:TRICE64 %d, %d\n", 1, 2 );
        break;
        case 90:
            TRICE( Id(14269), "tim:TRICE START time message\n");
            TRICE( Id(13937), "tim:TRICE STOP time message\n");
            TRICE( Id(13173), "tim:TRICE START time message\n");
            TRICE( Id(11405), "tim:TRICE STOP time message\n");
            TRICE( Id(15065), "tim:TRICE START time message\n");
            TRICE64( Id(13664), "rd:TRICE64 %d, %d\n", 1, 2 );
            TRICE( Id(12240), "tim:TRICE STOP time message\n");
        break;
        case 100:
            TRICE( Id(14886), "tim:TRICE --------------------------------------------------\n");
            TRICE( Id(10591), "tim:TRICE --------------------------------------------------\n");
            TRICE( Id(13992), "tim:TRICE --------------------------------------------------\n");
            TRICE( Id(12896), "tim:TRICE --------------------------------------------------\n");
            TRICE( Id(11894), "tim:TRICE --------------------------------------------------\n");
            TRICE( Id(12256), "tim:TRICE --------------------------------------------------\n");
            TRICE64( Id(11324), "rd:TRICE64 %d, %d\n", 1, 2 );
        break;
        case 110:
            TRICE( Id(14885), "tim:TRICE --------------------------------------------------\n");
            TRICE( Id(14520), "tim:TRICE --------------------------------------------------\n");
            TRICE( Id(14340), "tim:TRICE --------------------------------------------------\n");
            TRICE( Id(14118), "tim:TRICE --------------------------------------------------\n");
            TRICE( Id(14576), "tim:TRICE --------------------------------------------------\n");
            TRICE( Id(13648), "tim:TRICE --------------------------------------------------\n");
            TRICE( Id(11383), "tim:TRICE --------------------------------------------------\n");
            TRICE64( Id(10429), "rd:TRICE64 %d, %d\n", 1, 2 );
        break;
        case 120:
            TRICE( Id(12072), "tim:TRICE --------------------------------------------------\n");
            TRICE( Id(13868), "tim:TRICE --------------------------------------------------\n");
            TRICE( Id(15690), "tim:TRICE --------------------------------------------------\n");
            TRICE( Id(12960),  "rd:TRICE32_1 line %d\n", __LINE__ );
            TRICE64_1( Id(15480),  "rd:TRICE64_1 %d\n", __LINE__ );
            TRICE32_2( Id(15029),  "rd:TRICE32_2 line %d,%d\n", __LINE__, 2 );
            TRICE64_2( Id(11163),  "rd:TRICE64_2 line %d,%d\n", __LINE__, 2 );
        break;
        case 130:
            TRICE( Id(14624), "sig:TRICE with 1 to 12 values\n" );
            TRICE( Id(14647),"rd:TRICE %d\n", -1 );
            TRICE( Id(13276),"rd:TRICE %d, %d\n", -1, -2 );
            TRICE( Id(12430),"rd:TRICE %d, %d, %d\n", -1, -2, -3 );
            TRICE( Id(10959),"rd:TRICE %d, %d, %d, %d\n", -1, -2, -3, -4 );
            TRICE( Id(14593),"rd:TRICE %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
            TRICE( Id(12706),"rd:TRICE %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE( Id(12566),"rd:TRICE %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
            TRICE( Id(11997),"rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 140:
            TRICE( Id(15944),"rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE( Id(13285),"rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
            TRICE( Id(13424),"rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE( Id(15221),"rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 150:
            TRICE( Id(12704), "sig:TRICE_n with 1 to 12 values\n" );
            TRICE_1( Id(13462),   "rd:TRICE_1 %d\n", 1 );
            TRICE_2( Id(15315),   "rd:TRICE_2 %d, %d\n", 1, 2 );
            TRICE_3( Id(11251),   "rd:TRICE_3 %d, %d, %d\n", 1, 2, 3 );
            TRICE_4( Id(12655),   "rd:TRICE_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
            TRICE_5( Id(14012),   "rd:TRICE_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
            TRICE_6( Id(15409),   "rd:TRICE_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
            TRICE_7( Id(10611),   "rd:TRICE_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
            TRICE_8( Id(10544),   "rd:TRICE_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
        break;
        case 160:
            TRICE_9( Id(12912),   "rd:TRICE_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
            TRICE_10( Id(10420),  "rd:TRICE_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
            TRICE_11( Id(14014),  "rd:TRICE_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
            TRICE_12( Id(13174),  "rd:TRICE_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );				
        break;
        case 170:
            TRICE( Id(13586), "sig:TRICE8 with 1 to 12 values\n" );
            TRICE8( Id(13826),"rd:TRICE8 %d\n", -1 );
            TRICE8( Id(15630),"rd:TRICE8 %d, %d\n", -1, -2 );
            TRICE8( Id(11644),"rd:TRICE8 %d, %d, %d\n", -1, -2, -3 );
            TRICE8( Id(13230),"rd:TRICE8 %d, %d, %d, %d\n", -1, -2, -3, -4 );
            TRICE8( Id(11711),"rd:TRICE8 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
            TRICE8( Id(13239),"rd:TRICE8 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE8( Id(12264),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
            TRICE8( Id(13974),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 180:
            TRICE8( Id(12665),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE8( Id(13524),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
            TRICE8( Id(12517),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE8( Id(10577),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 190:
            TRICE( Id(14693), "sig:TRICE8_n with 1 to 12 values\n" );
            TRICE8_1( Id(13009),   "rd:TRICE8_1 %d\n", 1 );
            TRICE8_2( Id(15804),   "rd:TRICE8_2 %d, %d\n", 1, 2 );
            TRICE8_3( Id(14360),   "rd:TRICE8_3 %d, %d, %d\n", 1, 2, 3 );
            TRICE8_4( Id(11580),   "rd:TRICE8_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
            TRICE8_5( Id(12743),   "rd:TRICE8_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
            TRICE8_6( Id(14779),   "rd:TRICE8_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
            TRICE8_7( Id(13289),   "rd:TRICE8_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
        break;
        case 200:
            TRICE8_8( Id(15071),   "rd:TRICE8_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
            TRICE8_9( Id(14294),   "rd:TRICE8_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
            TRICE8_10( Id(15693),  "rd:TRICE8_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
            TRICE8_11( Id(13388),  "rd:TRICE8_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
            TRICE8_12( Id(12602),  "rd:TRICE8_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );
        break;
        case 210:
            TRICE( Id(11480), "sig:TRICE16 with 1 to 12 values\n" );
            TRICE16( Id(15529),"rd:TRICE16 %d\n", -1 );
            TRICE16( Id(10567),"rd:TRICE16 %d, %d\n", -1, -2 );
            TRICE16( Id(10543),"rd:TRICE16 %d, %d, %d\n", -1, -2, -3 );
            TRICE16( Id(15865),"rd:TRICE16 %d, %d, %d, %d\n", -1, -2, -3, -4 );
            TRICE16( Id(11847),"rd:TRICE16 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
            TRICE16( Id(13794),"rd:TRICE16 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE16( Id(12424),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
            TRICE16( Id(10749),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 220:
            TRICE16( Id(11742),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE16( Id(12001),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
            TRICE16( Id(14262),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE16( Id(12858),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 230:
            TRICE( Id(13073), "sig:TRICE16_n with 1 to 12 values\n" );
            TRICE16_1( Id(13270),  "rd:TRICE16_1 %d\n", 1 );
            TRICE16_2( Id(15565),  "rd:TRICE16_2 %d, %d\n", 1, 2 );
            TRICE16_3( Id(15387),  "rd:TRICE16_3 %d, %d, %d\n", 1, 2, 3 );
            TRICE16_4( Id(15985),  "rd:TRICE16_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
            TRICE16_5( Id(13796),  "rd:TRICE16_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
            TRICE16_6( Id(15591),  "rd:TRICE16_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
            TRICE16_7( Id(13699),  "rd:TRICE16_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
            TRICE16_8( Id(12296),  "rd:TRICE16_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
        break;
        case 240:
            TRICE16_9( Id(12642),  "rd:TRICE16_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
            TRICE16_10( Id(13769), "rd:TRICE16_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
            TRICE16_11( Id(14579), "rd:TRICE16_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
            TRICE16_12( Id(13860), "rd:TRICE16_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );
        break;
        case 250:
            TRICE( Id(14151), "sig:TRICE16 with 1 to 12 pointer\n" );
            TRICE16( Id(10977),"rd:TRICE16 %p\n", -1 );
            TRICE16( Id(13553),"rd:TRICE16 %p, %p\n", -1, -2 );
            TRICE16( Id(12374),"rd:TRICE16 %p, %p, %p\n", -1, -2, -3 );
            TRICE16( Id(11252),"rd:TRICE16 %p, %p, %p, %p\n", -1, -2, -3, -4 );
            TRICE16( Id(15671),"rd:TRICE16 %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5 );
            TRICE16( Id(10974),"rd:TRICE16 %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6 );
            TRICE16( Id(12905),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7);
            TRICE16( Id(14725),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 260:
            TRICE16( Id(15070),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE16( Id(15921),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
            TRICE16( Id(10660),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE16( Id(11089),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 270:
            TRICE( Id(13733), "sig:TRICE16 with 1 to 12 hex\n" );
            TRICE16( Id(15218),"rd:TRICE16 %X\n", -1 );
            TRICE16( Id(12389),"rd:TRICE16 %X, %X\n", -1, -2 );
            TRICE16( Id(13724),"rd:TRICE16 %X, %X, %X\n", -1, -2, -3 );
            TRICE16( Id(13295),"rd:TRICE16 %X, %X, %X, %X\n", -1, -2, -3, -4 );
            TRICE16( Id(11445),"rd:TRICE16 %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5 );
            TRICE16( Id(12006),"rd:TRICE16 %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6 );
            TRICE16( Id(15276),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7);
            TRICE16( Id(15762),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 280:
            TRICE16( Id(10315),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE16( Id(14756),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
            TRICE16( Id(12189),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE16( Id(12759),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 290:
            TRICE( Id(13743), "sig:TRICE32 with 1 to 12 values\n" );
            TRICE32( Id(13890),"rd:TRICE32 %d\n", -1 );
            TRICE32( Id(15223),"rd:TRICE32 %d, %d\n", -1, -2 );
            TRICE32( Id(10880),"rd:TRICE32 %d, %d, %d\n", -1, -2, -3 );
            TRICE32( Id(11945),"rd:TRICE32 %d, %d, %d, %d\n", -1, -2, -3, -4 );
            TRICE32( Id(14768),"rd:TRICE32 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
            TRICE32( Id(15585),"rd:TRICE32 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE32( Id(13414),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
            TRICE32( Id(14515),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 300:
            TRICE32( Id(15064),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE32( Id(13934),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
            TRICE32( Id(14727),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE32( Id(14615),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 310:
            TRICE( Id(13707), "signal:TRICE32_n with 1 to 12 values\n" );
            TRICE32_1( Id(10359),  "rd:TRICE32_1 %d\n", 1 );
            TRICE32_2( Id(10227),  "rd:TRICE32_2 %d, %d\n", 1, 2 );
            TRICE32_3( Id(12899),  "rd:TRICE32_3 %d, %d, %d\n", 1, 2, 3 );
            TRICE32_4( Id(14787),  "rd:TRICE32_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
            TRICE32_5( Id(14290),  "rd:TRICE32_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
            TRICE32_6( Id(10858),  "rd:TRICE32_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
            TRICE32_7( Id(11306),  "rd:TRICE32_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
            TRICE32_8( Id(13812),  "rd:TRICE32_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
        break;
        case 320:
            TRICE32_9( Id(10782),  "rd:TRICE32_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
            TRICE32_10( Id(15816), "rd:TRICE32_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
            TRICE32_11( Id(11155), "rd:TRICE32_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
            TRICE32_12( Id(12219), "rd:TRICE32_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );
        break;
        case 330:
            TRICE( Id(15094), "sig:TRICE64 with 1 to 12 values\n" );
            TRICE64( Id(15274),"rd:TRICE64 %d\n", -1 );
            TRICE64( Id(13622),"rd:TRICE64 %d, %d\n", -1, -2 );
            TRICE64( Id(11055),"rd:TRICE64 %d, %d, %d\n", -1, -2, -3 );
            TRICE64( Id(10493),"rd:TRICE64 %d, %d, %d, %d\n", -1, -2, -3, -4 );
            TRICE64( Id(13720),"rd:TRICE64 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
            TRICE64( Id(13813),"rd:TRICE64 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE64( Id(12102),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
            TRICE64( Id(12488),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 340:
            TRICE64( Id(13943),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE64( Id(10539),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
            TRICE64( Id(11208),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE64( Id(13563),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 350:
            TRICE( Id(13048), "sig:TRICE64_n with 1 to 12 values\n" );
            TRICE64_1( Id(10146),  "rd:TRICE64_1 %d\n", 1 );
            TRICE64_2( Id(15725),  "rd:TRICE64_2 %d, %d\n", 1, 2 );
            TRICE64_3( Id(14609),  "rd:TRICE64_3 %d, %d, %d\n", 1, 2, 3 );
            TRICE64_4( Id(12872),  "rd:TRICE64_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
            TRICE64_5( Id(13365),  "rd:TRICE64_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
            TRICE64_6( Id(10292),  "rd:TRICE64_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
            TRICE64_7( Id(14664),  "rd:TRICE64_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
            TRICE64_8( Id(13429),  "rd:TRICE64_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
        break;
        case 360:
            TRICE64_9( Id(15838),  "rd:TRICE64_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
            TRICE64_10( Id(12837), "rd:TRICE64_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
            TRICE64_11( Id(15279), "rd:TRICE64_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
            TRICE64_12( Id(12941), "rd:TRICE64_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );
        break;
        case 370:
            TRICE( Id(13533), "sig:Colors and ticks\n" );
            TRICE(Id(14701), "--------------------------------------------------\n");
            TRICE(Id(15344), "--------------------------------------------------\n");
            TRICE(Id(11226), "dbg:12345 as 16bit is %016b\n", 12345);
            TRICE(Id(10115), "--------------------------------------------------\n");
            TRICE(Id(11880), "sig:This ASSERT error is just a demo and no real error:\n");
            TRICE(Id(13229), "--------------------------------------------------\n");
        break;
        case 380:
            TRICE(Id(13119), "ERR:error       message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id(14826), "WRN:warning     message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id(12311), "ATT:attention   message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id(12786), "DIA:diagnostics message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id(12838), "TIM:timing      message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id(15098), "DBG:debug       message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id(13324), "SIG:signal      message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id(15425), "RD:read         message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id(15650), "WR:write        message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id(14797), "ISR:interrupt   message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id(11073), "MSG:normal      message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id(10478), "INFO:informal   message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id(13695), "time:TRICE32_1   message, SysTick is %6u\n", SYSTICKVAL);
        break;
        case 390:
            TRICE( Id(15702), "sig:Various ranges and formats\n" );
            TRICE8( Id(15316), "tst:TRICE8  %%03x -> %03x  %03x  %03x  %03x\n", 1, 0x7f, 0x80, 0xff);
            TRICE8( Id(15217), "tst:TRICE8   %%4d -> %4d %4d %4d %4d\n", 1, 0x7f, 0x80, 0xff);
            TRICE8( Id(10563), "tst:TRICE8   %%4u -> %4u %4u %4u %4u\n", 1, 0x7f, 0x80, 0xff);
            TRICE8( Id(11135), "tst:TRICE8   %%4o -> %4o %4o %4o %4o\n", 1, 0x7f, 0x80, 0xff);
            TRICE8( Id(10132), "tst:TRICE8   %%4O -> %4O %4O %4O %4O\n", 1, 0x7f, 0x80, 0xff);
            TRICE8( Id(14974), "tst:TRICE8   %%4X -> %4X %4X %4X %4X\n", 1, 0x7f, 0x80, 0xff);
            TRICE8( Id(14894), "tst:TRICE8   %%4x -> %4x %4x %4x %4x\n", 1, 0x7f, 0x80, 0xff);
            TRICE8( Id(14192), "tst:TRICE8   %%8b -> %8b %8b %8b %8b\n", 1, 0x7f, 0x80, 0xff);
            TRICE8( Id(12163), "tst:TRICE8  %%08b -> %08b %08b %08b %08b\n", 1, 0x7f, 0x80, 0xff);
        break;
        case 400:
            TRICE16(Id(14501), "tst:TRICE16  %%05x ->   %05x   %05x   %05x   %05x\n", 1, 0x7fff, 0x8000, 0xffff);
            TRICE16(Id(11279), "tst:TRICE16   %%6d ->  %6d  %6d  %6d  %6d\n",         1, 0x7fff, 0x8000, 0xffff);
            TRICE16(Id(11548), "tst:TRICE16   %%7o -> %7o %7o %7o %7o\n",             1, 0x7fff, 0x8000, 0xffff);

            TRICE32(Id(15114), "tst:TRICE32 %%09x ->      %09x      %09x       %09x     %09x\n", 1, 0x7fffffff, 0x80000000, 0xffffffff);
            TRICE32(Id(10067), "tst:TRICE32 %%10d ->     %10d     %10d     %10d    %10x\n",      1, 0x7fffffff, 0x80000000, 0xffffffff);
            TRICE32(Id(12894), "att:TRICE32 %40b\n", 0xAAAAAAAA);
        break;
        case 410:
            TRICE64(Id(11145), "tst:TRICE64 %%09x ->      %09x      %09x       %09x     %09x\n", 1, 0x7fffffffffffffff, 0x8000000000000000, 0xffffffffffffffff);
            TRICE64(Id(11263), "tst:TRICE64 %%10d ->     %10d     %10d     %10d    %10x\n",      1, 0x7fffffffffffffff, 0x8000000000000000, 0xffffffffffffffff);
            TRICE64(Id(10558), "att:TRICE64 %70b\n", 0xAAAAAAAAAAAAAAAA);
        break;
        case 420:
            TRICE( Id(12482), "sig:Legacy TRICE8\n" );
            TRICE8_1(Id(11043), "tst:TRICE8_1 %02x\n", 0xA1);
            TRICE8_2(Id(10013), "tst:TRICE8_2 %02x %02x\n", 0xA1, 0xA2);
            TRICE8_3(Id(11962), "tst:TRICE8_3 %02x %02x %02x\n", 0xA1, 0xA2, 0xA3);
            TRICE8_4(Id(14326), "tst:TRICE8_4 %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4);
            TRICE8_5(Id(14187), "tst:TRICE8_5 %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5 );
            TRICE8_6(Id(14967), "tst:TRICE8_6 %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6);
            TRICE8_7(Id(13200), "tst:TRICE8_7 %02x %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7);
            TRICE8_8(Id(10658), "tst:TRICE8_8 %02x %02x %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8);
        break;
        case 430:
            TRICE8_1(Id(15533), "tst:TRICE8_1 %d\n",                      -1);
            TRICE8_2(Id(13958), "tst:TRICE8_2 %d %d\n",                   -1, -2);
            TRICE8_3(Id(13618), "tst:TRICE8_3 %d %d %d\n",                -1, -2, -3);
            TRICE8_4(Id(10189), "tst:TRICE8_4 %d %d %d %d\n",             -1, -2, -3, -4);
            TRICE8_5(Id(11713), "tst:TRICE8_5 %d %d %d %d %d\n",          -1, -2, -3, -4, -5);
            TRICE8_6(Id(12346), "tst:TRICE8_6 %d %d %d %d %d %d\n",       -1, -2, -3, -4, -5, -6);
            TRICE8_7(Id(12190), "tst:TRICE8_7 %d %d %d %d %d %d %d\n",    -1, -2, -3, -4, -5, -6, -7);
            TRICE8_8(Id(11116), "tst:TRICE8_8 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8);
        break;
        case 440:
            TRICE8_1(Id(12506), "tst:TRICE8_1 %d\n", 1);
            TRICE8_2(Id(10205), "tst:TRICE8_2 %d %d\n", 1, 2);
            TRICE8_3(Id(11215), "tst:TRICE8_3 %d %d %d\n", 1, 2, 3);
            TRICE8_4(Id(15668), "tst:TRICE8_4 %d %d %d %d\n", 1, 2, 3, 4);
            TRICE8_5(Id(13677), "tst:TRICE8_5 %d %d %d %d %d\n", 1, 2, 3, 4, 5);
            TRICE8_6(Id(14852), "tst:TRICE8_6 %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6);
            TRICE8_7(Id(12134), "tst:TRICE8_7 %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7);
            TRICE8_8(Id(12489), "tst:TRICE8_8 %d %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7, 8);
        break;
        case 450:
            TRICE8_1(Id(13504), "tst:TRICE8_1 %u\n", 201);
            TRICE8_2(Id(13094), "tst:TRICE8_2 %u %u\n", 201, 202);
            TRICE8_3(Id(15760), "tst:TRICE8_3 %u %u %u\n", 201, 202, 203);
            TRICE8_4(Id(12146), "tst:TRICE8_4 %u %u %u %u\n", 201, 202, 203, 204);
            TRICE8_5(Id(15144), "tst:TRICE8_5 %u %u %u %u %u\n", 201, 202, 203, 204, 205);
            TRICE8_6(Id(14050), "tst:TRICE8_6 %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206);
            TRICE8_7(Id(12898), "tst:TRICE8_7 %u %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206, 207);
            TRICE8_8(Id(10595), "tst:TRICE8_8 %u %u %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206, 207, 208);
            TRICE8_8(Id(14696), "tst:TRICE8_1 %%d=%d, %%u=%u, 0x%%x=0x%x, 0x%%2x=0x%2x, 0x%%02x=0x%02x, 0x%%3x=0x%3x, 0x%%03x=0x%03x, %%b=%b\n", 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81);
        break;
        case 460:
            TRICE( Id(10416), "sig:Legacy TRICE16-64\n" );
            TRICE16_1(Id(13244), "tst:TRICE16_1 %d\n", -111);
            TRICE16_2(Id(13857), "tst:TRICE16_2 %d %d\n", -111, -222);
            TRICE16_3(Id(15932), "tst:TRICE16_3 %d %d %d\n", -111, -222, -333);
            TRICE16_4(Id(13184), "tst:TRICE16_4 %d %d %d %d\n", -111, -222, -333, -444);
            TRICE32_1(Id(10081), "tst:TRICE32_1 %08x\n", 0x0123cafe);
            TRICE32_1(Id(11775), "tst:TRICE32_1 %d\n", -111);
            TRICE32_2(Id(12029), "tst:TRICE32_2 %x %x\n", -111, -222);
            TRICE32_2(Id(15550), "tst:TRICE32_2 %d %d\n", -111, -222);
        break;
        case 470:
            TRICE32_3(Id(12468), "tst:TRICE32_3 %x %x %x\n", -111, -222, -333);
            TRICE32_3(Id(11365), "tst:TRICE32_3 %d %d %d\n", -111, -222, -333);
            TRICE32_4(Id(12433), "tst:TRICE32_4 %x %x %x %x\n", -111, -222, -333, -444);
            TRICE32_4(Id(12090), "tst:TRICE32_4 %d %d %d %d\n", -111, -222, -333, -444);
        break;
        case 480:
            TRICE64_1(Id(13865), "tst:TRICE64_1 %d\n", -111);
            TRICE64_2(Id(14085), "tst:TRICE64_2 %d %d\n", -111, -222);
            TRICE16_1(Id(12668), "tst:TRICE16_1 %u\n", 60001);
            TRICE16_2(Id(10620), "tst:TRICE16_2 %u %u\n", 60001, 60002);
            TRICE16_3(Id(14685), "tst:TRICE16_3 %u %u %u\n", 60001, 60002, 60003);
            TRICE16_4(Id(13314), "tst:TRICE16_4 %u %u %u %u\n", 60001, 60002, 60003, 60004);
            TRICE32_1(Id(12005), "tst:TRICE32_1 %u\n", 4000000001);
            TRICE32_2(Id(15787), "tst:TRICE32_2 %u %u\n", 4000000001, 4000000002);
            TRICE32_3(Id(15449), "tst:TRICE32_3 %u %u %u\n", 4000000001, 4000000002, 4000000003);
            TRICE32_4(Id(12418), "tst:TRICE32_4 %u %u %u %u\n", 4000000001, 4000000002, 4000000003, 4000000004);
        break;
        case 490:
            TRICE64_1(Id(12289), "tst:TRICE64_1 %x\n", -1);
            TRICE64_2(Id(12616), "tst:TRICE64_2 %x %x\n", -1, -2);
            TRICE64_1(Id(15895), "tst:TRICE64_1 %u\n", -1);
            TRICE64_2(Id(13419), "tst:TRICE64_2 %u %u\n", -1, -2);
        break;
        case 500:
            TRICE( Id(11230), "sig:colored single letters and several TRICE macros in one line\n" );
            TRICE(Id(11168), "e:A");
            TRICE(Id(10319), "w:B");
            TRICE(Id(14999), "a:c");
            TRICE(Id(13372), "wr:d");
            TRICE(Id(10875), "rd:e\n");
            TRICE(Id(12870), "diag:f");
            TRICE(Id(14897), "d:G");
            TRICE(Id(12508), "t:H");
            TRICE(Id(10460), "time:i");
            TRICE(Id(14964), "message:J");
            TRICE(Id(12749), "dbg:k\n");
        break;
        case 510:
            TRICE(Id(15986), "1");
            TRICE(Id(14558), "2");
            TRICE(Id(15031), "3");
            TRICE(Id(11600), "4");
            TRICE(Id(11031), "e:7");
            TRICE(Id(11882), "m:12");
            TRICE(Id(10829), "m:123\n");
            TRICE(Id(12552), "e:A");TRICE(Id(15369), "w:B");TRICE(Id(11053), "a:c");
            TRICE(Id(12330), "wr:d");TRICE(Id(15023), "rd:e\n");//TRICE(Id(10225), "diag:f");
        break;
        case 520:
            TRICE( Id(14678), "sig:Runtime generated strings\n" );
            s = "AAAAAAAAAAAA";
            TRICE32( Id(12767), "dbg:len=%u:", strlen(s) );
            TRICE_S( Id(14574), "sig:%s\n", s );
            s = "\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBCCCCCCCC\
";
            TRICE32( Id(12768), "dbg:len=%u:", strlen(s) );
            TRICE_S( Id(14525), "sig:%s\n", s );
        break;
        case 530:
        TRICE( Id(13421), "sig:Runtime generated strings\n" );
            s = "\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\
DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\
EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\
";
            TRICE32( Id(14286), "dbg:len=%u: ", strlen(s) );
            TRICE_S( Id(10783), "sig:%s\n", s );
        break;
        case 540:
            TRICE( Id(12201), "sig:Runtime generated strings\n" );
            s = "\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\
DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\
EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\
";
            TRICE32( Id(15834), "dbg:len=%u: ", strlen(s) );
            TRICE_S( Id(11696), "sig:%s\n", s );
        break;
        case 550:
            TRICE( Id(13310), "sig:Runtime generated strings\n" );
            s = "\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\
DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\
EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\
DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\
EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\
DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\
EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\
";
          TRICE32( Id(10296), "dbg:len=%u: ", strlen(s) );
          TRICE_S( Id(15017), "sig:%s\n", s );
        break;
        case 560:{ // https://yourbasic.org/golang/fmt-printf-reference-cheat-sheet/
            int i = 15;
            TRICE( Id(14969), "sig:Integer (indent, base, sign)\n" );
            TRICE( Id(13565), "rd: 15 	%d 	Base 10\n", i );
            TRICE( Id(13418), "rd: +15 	%+d 	Always show sign\n", i );
            TRICE( Id(10869), "rd:   15 	%4d 	Pad with spaces (width 4, right justified)\n", i );
            TRICE( Id(14009), "rd: 15   	%-4d 	Pad with spaces (width 4, left justified)\n", i );
            TRICE( Id(15109), "rd: 0015 	%04d 	Pad with zeroes (width 4)\n", i );
            TRICE( Id(13248), "rd: 1111 	%b 	Base 2\n", i );
            TRICE( Id(14436), "rd: 17 	%o 	Base 8\n", i );
            TRICE( Id(12669), "rd: f 	%x 	Base 16, lowercase\n", i );
            TRICE( Id(13115), "rd: F 	%X 	Base 16, uppercase\n", i );
            TRICE( Id(12726), "rd: 0xf 	%#x 	Base 16, with leading 0x\n", i );
        }
        break;
        case 570:{
            char a = 'A';
            TRICE( Id(13382), "sig:Character (quoted, Unicode)\n" );
            TRICE( Id(15263), "rd: A 	%c 	Character\n", a );
            TRICE( Id(10629), "rd: 'A' 	%q 	Quoted character\n", a );
          //TRICE( Id(14764), "rd: U+0041 	%U 	Unicode\n", a );// todo
          //TRICE( Id(11022), "rd: U+0041 'A' 	%#U 	Unicode with character\n", a );// todo
            TRICE( Id(12507), "sig:Boolean (true/false)\n" );
            TRICE( Id(12050), "rd:Use %%t to format a boolean as true (%t) or false (%t).\n", 1, 0 );
            TRICE( Id(13069), "sig:Pointer (hex)\n" );
        }
        break;
        case 580:
            TRICE8( Id(10771), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x84 );
            TRICE16( Id(12142), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x1234 );
            TRICE32( Id(14978), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0xaabbccdd );
            TRICE64( Id(10537), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x1122334455667788 );
            TRICE( Id(11509), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0xaabbccdd);
        break;
        case 590:{
            float f = 123.456;
            TRICE( Id(11967), "sig:Float (indent, precision, scientific notation)\n" );
            TRICE( Id(10265), "rd: 1.234560e+02		%e 	%%e Scientific notation\n", aFloat(f) );
            TRICE( Id(12195), "rd: 123.456000		%f 	%%f Decimal point, no exponent\n", aFloat(f) );
            TRICE( Id(11568), "rd: 123.46	 		%.2f		%%.2f Default width, precision 2\n", aFloat(f) );
            TRICE( Id(14519), "rd: ␣␣123.46		%8.2f	%%8.2f Width 8, precision 2\n", aFloat(f) );
            TRICE( Id(11950), "rd: 123.456		%g		%%g Exponent as needed, necessary digits only\n", aFloat(f) );
        }
        break;
        case 600:{
            double d = 123.456;
            TRICE( Id(13612), "sig:Double (indent, precision, scientific notation)\n" );
            TRICE64( Id(10669), "rd: 1.234560e+02		%e 	%%e Scientific notation\n", aDouble(d) );
            TRICE64( Id(14164), "rd: 123.456000		%f 	%%f Decimal point, no exponent\n", aDouble(d) );
            TRICE64( Id(15890), "rd: 123.46			%.2f    	%%.2f Default width, precision 2\n", aDouble(d) );
            TRICE64( Id(15350), "rd: ␣␣123.46		%8.2f 	%%8.2f Width 8, precision 2\n", aDouble(d) );
            TRICE64( Id(12907), "rd: 123.456		%g		%%g Exponent as needed, necessary digits only\n", aDouble(d) );
        }
        break;
        case 610:{
            char* s = "café";
            TRICE( Id(10223), "sig:String or byte slice (quote, indent, hex)\n" );
            TRICE_S( Id(15962), "rd: café 			%s		Plain string\n", s );
            TRICE_S( Id(15079), "rd: ␣␣café 		%6s 		Width 6, right justify\n", s );
            TRICE_S( Id(11891), "rd: café␣␣ 		%-6s 		Width 6, left justify\n", s );
            TRICE_S( Id(11409), "rd: \"café\" 		%q	 	Quoted string\n", s );
            TRICE_S( Id(14021), "rd: 636166c3a9 		%x	Hex dump of byte values\n", s );
            TRICE_S( Id(10112), "rd: 63 61 66 c3 a9 	% x	Hex dump with spaces\n", s );
        }
        break;
        case 620:
            TRICE( Id(11281), "sig:TRICE32 with variable param count 1 to 12\n" );
            TRICE8(Id(10718), "tst:TRICE8 %d\n", -1 );
            TRICE8(Id(14933), "tst:TRICE8 %d %d\n", -1, -2 );
            TRICE8(Id(14133), "tst:TRICE8 %d %d %d\n", -1, -2, -3 );
            TRICE8(Id(12384), "tst:TRICE8 %d %d %d %d\n", -1, -2, -3, -4 );
            TRICE8(Id(12172), "tst:TRICE8 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
            TRICE8(Id(15200), "tst:TRICE8 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE8(Id(13091), "tst:TRICE8 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
            TRICE8(Id(11722), "tst:TRICE8 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 630:
            TRICE8(Id(11401), "tst:TRICE8 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE8(Id(15182), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
            TRICE8(Id(11209), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE8(Id(12632), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 640:
            TRICE8(Id(14456), "tst:TRICE8 %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );
            TRICE8_1 (Id(13946), "tst:TRICE8_1  %d\n", -1 );
            TRICE8_2 (Id(15742), "tst:TRICE8_2  %d %d\n", -1, -2 );
            TRICE8_3 (Id(10475), "tst:TRICE8_3  %d %d %d\n", -1, -2, -3 );
            TRICE8_4 (Id(13339), "tst:TRICE8_4  %d %d %d %d\n", -1, -2, -3, -4 );
            TRICE8_5 (Id(15193), "tst:TRICE8_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
            TRICE8_6 (Id(15348), "tst:TRICE8_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE8_7 (Id(15785), "tst:TRICE8_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
            TRICE8_8 (Id(14400), "tst:TRICE8_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 650:
            TRICE8_9 (Id(12646), "tst:TRICE8_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE8_10(Id(15073), "tst:TRICE8_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
            TRICE8_11(Id(12107), "tst:TRICE8_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE8_12(Id(13150), "tst:TRICE8_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
            TRICE8  (Id(11361), "tst:TRICE8   %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );
            TRICE8_9(Id(13049), "tst:TRICE8_9 %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );
        break;
        case 660:
            TRICE( Id(14305), "sig:TRICE16 with variable param count 1 to 12\n" );
            TRICE16(Id(12461), "tst:TRICE16 %d\n", -1 );
            TRICE16(Id(15674), "tst:TRICE16 %d %d\n", -1, -2 );
            TRICE16(Id(12013), "tst:TRICE16 %d %d %d\n", -1, -2, -3 );
            TRICE16(Id(15020), "tst:TRICE16 %d %d %d %d\n", -1, -2, -3, -4 );
            TRICE16(Id(10653), "tst:TRICE16 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
            TRICE16(Id(15005), "tst:TRICE16 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE16(Id(15398), "tst:TRICE16 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
            TRICE16(Id(13557), "tst:TRICE16 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 670:
            TRICE16(Id(12342), "tst:TRICE16 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE16(Id(10024), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
            TRICE16(Id(13569), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE16(Id(15604), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 680:
            TRICE16(Id(10781), "tst:TRICE16 %t %b %x %X %d %u %o %O %p\n", 3, 3, 3, 3, 3, 3, 3, 3, 3 );
            TRICE16_1 (Id(12258), "tst:TRICE16_1  %d\n", -1 );
            TRICE16_2 (Id(14996), "tst:TRICE16_2  %d %d\n", -1, -2 );
            TRICE16_3 (Id(14388), "tst:TRICE16_3  %d %d %d\n", -1, -2, -3 );
            TRICE16_4 (Id(12218), "tst:TRICE16_4  %d %d %d %d\n", -1, -2, -3, -4 );
            TRICE16_5 (Id(14267), "tst:TRICE16_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
            TRICE16_6 (Id(12564), "tst:TRICE16_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE16_7 (Id(12348), "tst:TRICE16_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
            TRICE16_8 (Id(11562), "tst:TRICE16_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 690:
            TRICE16_9 (Id(14140), "tst:TRICE16_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE16_10(Id(11398), "tst:TRICE16_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
            TRICE16_11(Id(13736), "tst:TRICE16_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE16_12(Id(12012), "tst:TRICE16_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
            TRICE16_9(Id(13179), "tst:TRICE16 %t %b %x %X %d %u %o %O %p\n", 3, 3, 3, 3, 3, 3, 3, 3, 3 );
        break;
        case 700:
            TRICE( Id(14813), "sig:TRICE32 with variable param count 1 to 12\n" );
            TRICE32(Id(15044), "tst:TRICE32 %d\n", -1 );
            TRICE32(Id(11669), "tst:TRICE32 %d %d\n", -1, -2 );
            TRICE32(Id(10124), "tst:TRICE32 %d %d %d\n", -1, -2, -3 );
            TRICE32(Id(10285), "tst:TRICE32 %d %d %d %d\n", -1, -2, -3, -4 );
            TRICE32(Id(15115), "tst:TRICE32 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
            TRICE32(Id(15863), "tst:TRICE32 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE32(Id(14714), "tst:TRICE32 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
            TRICE32(Id(13772), "tst:TRICE32 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 710:
            TRICE32(Id(13337), "tst:TRICE32 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE32(Id(14960), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
            TRICE32(Id(15771), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE32(Id(13545), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 720:
            TRICE32(Id(10421), "tst:TRICE32 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));
            TRICE32_1 (Id(13060), "tst:TRICE32_1  %d\n", -1 );
            TRICE32_2 (Id(13666), "tst:TRICE32_2  %d %d\n", -1, -2 );
            TRICE32_3 (Id(14142), "tst:TRICE32_3  %d %d %d\n", -1, -2, -3 );
            TRICE32_4 (Id(15175), "tst:TRICE32_4  %d %d %d %d\n", -1, -2, -3, -4 );
            TRICE32_5 (Id(13931), "tst:TRICE32_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
            TRICE32_6 (Id(14430), "tst:TRICE32_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE32_7 (Id(10488), "tst:TRICE32_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
            TRICE32_8 (Id(10980), "tst:TRICE32_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 730:
            TRICE32_9 (Id(15515), "tst:TRICE32_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE32_10(Id(11291), "tst:TRICE32_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
            TRICE32_11(Id(10204), "tst:TRICE32_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE32_12(Id(10754), "tst:TRICE32_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
            TRICE32   (Id(10997), "tst:TRICE32    %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));
            TRICE32_12(Id(15443), "tst:TRICE32_12 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));
        break;
        case 740:
            TRICE( Id(11482), "sig:TRICE64 with variable param count 1 to 12\n" );
            TRICE64(Id(12696), "tst:TRICE64 %d\n", -1 );
            TRICE64(Id(10551), "tst:TRICE64 %d %d\n", -1, -2 );
            TRICE64(Id(14605), "tst:TRICE64 %d %d %d\n", -1, -2, -3 );
            TRICE64(Id(10168), "tst:TRICE64 %d %d %d %d\n", -1, -2, -3, -4 );
            TRICE64(Id(15994), "tst:TRICE64 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
            TRICE64(Id(13037), "tst:TRICE64 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE64(Id(12114), "tst:TRICE64 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
            TRICE64(Id(14800), "tst:TRICE64 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 750:
            TRICE64(Id(13023), "tst:TRICE64 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE64(Id(14995), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
            TRICE64(Id(14994), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE64(Id(13222), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 760:
            TRICE64(Id(10497), "tst:TRICE64 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aDouble(3.14159), aDouble(3.14159), aDouble(3.14159));
            TRICE64_1 (Id(11384), "tst:TRICE64_1  %d\n", -1 );
            TRICE64_2 (Id(10843), "tst:TRICE64_2  %d %d\n", -1, -2 );
            TRICE64_3 (Id(15757), "tst:TRICE64_3  %d %d %d\n", -1, -2, -3 );
            TRICE64_4 (Id(12420), "tst:TRICE64_4  %d %d %d %d\n", -1, -2, -3, -4 );
            TRICE64_5 (Id(13576), "tst:TRICE64_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
            TRICE64_6 (Id(12149), "tst:TRICE64_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE64_7 (Id(11399), "tst:TRICE64_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
            TRICE64_8 (Id(11533), "tst:TRICE64_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 770:
            TRICE64_9 (Id(12167), "tst:TRICE64_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE64_10(Id(11930), "tst:TRICE64_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
            TRICE64_11(Id(11675), "tst:TRICE64_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE64_12(Id(15610), "tst:TRICE64_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
            TRICE64_12(Id(15698), "tst:TRICE64_12 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aDouble(3.14159), aDouble(3.14159), aDouble(3.14159));
        break;
        case 780:
            TRICE( Id(12612), "att:positive and negative float in format variants\n");
            TRICE( Id(14625), "rd:TRICE float %f (%%f)\n", aFloat(x) );
            TRICE( Id(13784), "rd:TRICE float %9f (%%9f)\n", aFloat(x) );
            TRICE( Id(15660), "rd:TRICE float %.9f (%%.9f)\n", aFloat(x) );
            TRICE( Id(15410), "rd:TRICE float %9.f (%%9.f)\n", aFloat(x) );
            TRICE( Id(15009), "rd:TRICE float %9.6f (%%9.6f)\n", aFloat(x) );
            TRICE( Id(13656), "rd:TRICE float %f (%%f)\n", aFloat(-x) );
        break;
        case 790:
            TRICE( Id(12925), "rd:TRICE float %9f (%%9f)\n", aFloat(-x) );
            TRICE( Id(13264), "rd:TRICE float %.9f (%%.9f)\n", aFloat(-x) );
            TRICE( Id(13752), "rd:TRICE float %9.f (%%9.f)\n", aFloat(-x) );
            TRICE( Id(10158), "rd:TRICE float %9.6f (%%9.6f)\n", aFloat(-x) );
            TRICE( Id(14207), "rd:TRICE float %+f (%%f)\n", aFloat(x) );
            TRICE( Id(11258), "rd:TRICE float %+9f (%%9f)\n", aFloat(x) );
            TRICE( Id(14578), "rd:TRICE float %+.9f (%%.9f)\n", aFloat(x) );
            TRICE( Id(12335), "rd:TRICE float %+9.f (%%9.f)\n", aFloat(x) );
        break;
        case 800:
            TRICE( Id(14431), "rd:TRICE float %+9.6f (%%9.6f)\n", aFloat(x) );
            TRICE( Id(10687), "rd:TRICE float %+f (%%f)\n", aFloat(-x) );
            TRICE( Id(11834), "rd:TRICE float %+9f (%%9f)\n", aFloat(-x) );
            TRICE( Id(11379), "rd:TRICE float %+.9f (%%.9f)\n", aFloat(-x) );
            TRICE( Id(12761), "rd:TRICE float %+9.f (%%9.f)\n", aFloat(-x) );
            TRICE( Id(11831), "rd:TRICE float %+9.6f (%%9.6f)\n", aFloat(-x) );
            TRICE( Id(11422), "rd:TRICE float %-f (%%f)\n", aFloat(x) );
            TRICE( Id(11772), "rd:TRICE float %-9f (%%9f)\n", aFloat(x) );
        break;
        case 810:
            TRICE( Id(12787), "rd:TRICE float %-.9f (%%.9f)\n", aFloat(x) );
            TRICE( Id(14356), "rd:TRICE float %-9.f (%%9.f)\n", aFloat(x) );
            TRICE( Id(11756), "rd:TRICE float %-9.6f (%%9.6f)\n", aFloat(x) );
            TRICE( Id(11743), "rd:TRICE float %-f (%%f)\n", aFloat(-x) );
            TRICE( Id(10137), "rd:TRICE float %-9f (%%9f)\n", aFloat(-x) );
            TRICE( Id(10135), "rd:TRICE float %-.9f (%%.9f)\n", aFloat(-x) );
            TRICE( Id(15544), "rd:TRICE float %-9.f (%%9.f)\n", aFloat(-x) );
            TRICE( Id(11077), "rd:TRICE float %-9.6f (%%9.6f)\n", aFloat(-x) );
        break;
        case 820:
            TRICE( Id(14645), "att:positive float & double in variants\n");
            TRICE32_1( Id(15222), "rd:TRICE32_1 float %e (%%e)\n", aFloat(x) );
            TRICE32_1( Id(14694), "rd:TRICE32_1 float %f (%%f)\n", aFloat(x) );
            TRICE32_1( Id(12322), "rd:TRICE32_1 float %g (%%g)\n", aFloat(x) );
            TRICE32_1( Id(14721), "rd:TRICE32_1 float %E (%%E)\n", aFloat(x) );
            TRICE32_1( Id(10849), "rd:TRICE32_1 float %F (%%F)\n", aFloat(x) );
            TRICE32_1( Id(11695), "rd:TRICE32_1 float %G (%%G)\n", aFloat(x) );
        break;
        case 830:
            TRICE32( Id(10092), "rd:TRICE32 float %e (%%e)\n", aFloat(x) );
            TRICE32( Id(12427), "rd:TRICE32 float %f (%%f)\n", aFloat(x) );
            TRICE32( Id(10705), "rd:TRICE32 float %g (%%g)\n", aFloat(x) );
            TRICE32( Id(11935), "rd:TRICE32 float %E (%%E)\n", aFloat(x) );
            TRICE32( Id(10066), "rd:TRICE32 float %F (%%F)\n", aFloat(x) );
            TRICE32( Id(14949), "rd:TRICE32 float %G (%%G)\n", aFloat(x) );
        break;
        case 840:
            TRICE( Id(11185), "rd:TRICE float %e (%%e)\n", aFloat(x) );
            TRICE( Id(14126), "rd:TRICE float %f (%%f)\n", aFloat(x) );
            TRICE( Id(15395), "rd:TRICE float %g (%%g)\n", aFloat(x) );
            TRICE( Id(11262), "rd:TRICE float %E (%%E)\n", aFloat(x) );
            TRICE( Id(11280), "rd:TRICE float %F (%%F)\n", aFloat(x) );
            TRICE( Id(10365), "rd:TRICE float %G (%%G)\n", aFloat(x) );
        break;
        case 850:
            TRICE64( Id(13500), "rd:TRICE64 double %e (%%e), aDouble(y)\n", aDouble(y) );
            TRICE64( Id(13928), "rd:TRICE64 double %f (%%f), aDouble(y)\n", aDouble(y) );
            TRICE64( Id(14559), "rd:TRICE64 double %g (%%g), aDouble(y)\n", aDouble(y) );
            TRICE64( Id(12648), "rd:TRICE64 double %E (%%E), aDouble(y)\n", aDouble(y) );
            TRICE64( Id(15004), "rd:TRICE64 double %F (%%F), aDouble(y)\n", aDouble(y) );
            TRICE64( Id(15447), "rd:TRICE64 double %G (%%G), aDouble(y)\n", aDouble(y) );
        break;
        case 860:
            TRICE64_1( Id(12598), "rd:TRICE64_1 double %e (%%e), aDouble(y)\n", aDouble(y) );
            TRICE64_1( Id(10006), "rd:TRICE64_1 double %f (%%f), aDouble(y)\n", aDouble(y) );
            TRICE64_1( Id(11796), "rd:TRICE64_1 double %g (%%g), aDouble(y)\n", aDouble(y) );
            TRICE64_1( Id(13029), "rd:TRICE64_1 double %E (%%E), aDouble(y)\n", aDouble(y) );
            TRICE64_1( Id(12535), "rd:TRICE64_1 double %F (%%F), aDouble(y)\n", aDouble(y) );
            TRICE64_1( Id(15375), "rd:TRICE64_1 double %G (%%G), aDouble(y)\n", aDouble(y) );
        break;
        case 870:
            TRICE( Id(13547), "att:negative float & double\n" );
            TRICE( Id(13502), "rd:TRICE float %e (%%e)\n", aFloat(-x) );
            TRICE( Id(11552), "rd:TRICE float %f (%%f)\n", aFloat(-x) );
            TRICE( Id(12698), "rd:TRICE float %g (%%g)\n", aFloat(-x) );
            TRICE( Id(11159), "rd:TRICE float %E (%%E)\n", aFloat(-x) );
            TRICE( Id(10817), "rd:TRICE float %F (%%F)\n", aFloat(-x) );
            TRICE( Id(12499), "rd:TRICE float %G (%%G)\n", aFloat(-x) );
        break;
        case 880:
            TRICE64( Id(15389), "rd:TRICE64 double %e (%%e), aDouble(y)\n", aDouble(-y) );
            TRICE64( Id(10618), "rd:TRICE64 double %f (%%f), aDouble(y)\n", aDouble(-y) );
            TRICE64( Id(14712), "rd:TRICE64 double %g (%%g), aDouble(y)\n", aDouble(-y) );
            TRICE64( Id(15528), "rd:TRICE64 double %E (%%E), aDouble(y)\n", aDouble(-y) );
            TRICE64( Id(13203), "rd:TRICE64 double %F (%%F), aDouble(y)\n", aDouble(-y) );
            TRICE64( Id(15925), "rd:TRICE64 double %G (%%G), aDouble(y)\n", aDouble(-y) );
        break;
        case 890:
            TRICE( Id(13428), "att:formatted float & double\n" );
            TRICE32( Id(15203), "rd:TRICE32 float %12.6e (%%12.6e)\n", aFloat(x) );
            TRICE32( Id(11725), "rd:TRICE32 float %12.6f (%%12.6f)\n", aFloat(x) );
            TRICE32( Id(13219), "rd:TRICE32 float %12.6g (%%12.6g)\n", aFloat(x) );
            TRICE32( Id(11569), "rd:TRICE32 float %12.6E (%%12.6E)\n", aFloat(x) );
            TRICE32( Id(15738), "rd:TRICE32 float %12.6F (%%12.6F)\n", aFloat(x) );
            TRICE32( Id(12255), "rd:TRICE32 float %12.6G (%%12.6G)\n", aFloat(x) );
        break;
        case 900:
            TRICE64( Id(11186), "rd:TRICE64 double %12.6e (%%12.6e), aDouble(y)\n", aDouble(y) );
            TRICE64( Id(11887), "rd:TRICE64 double %12.6f (%%12.6f), aDouble(y)\n", aDouble(y) );
            TRICE64( Id(15095), "rd:TRICE64 double %12.6g (%%12.6g), aDouble(y)\n", aDouble(y) );
            TRICE64( Id(13973), "rd:TRICE64 double %12.6E (%%12.6E), aDouble(y)\n", aDouble(y) );
            TRICE64( Id(13413), "rd:TRICE64 double %12.6F (%%12.6F), aDouble(y)\n", aDouble(y) );
            TRICE64( Id(14834), "rd:TRICE64 double %12.6G (%%12.6G), aDouble(y)\n", aDouble(y) );
        break;
        case 910:
            TRICE( Id(12411), "att:mixed int & float & double & bits\n" );
            TRICE32( Id(12191), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n", FloatToInt32(x), aFloat(x), aFloat(x), aFloat(x) );
            TRICE32( Id(10641), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",    (int32_t)(x), aFloat(x), aFloat(x), aFloat(x) );
            TRICE32( Id(11670), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",              x,  aFloat(x), aFloat(x), aFloat(x) );
            TRICE32( Id(12544), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n", FloatToInt32(-x), aFloat(-x), aFloat(-x), aFloat(-x) );
            TRICE32( Id(13057), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",    (int32_t)(-x), aFloat(-x), aFloat(-x), aFloat(-x) );
            TRICE32( Id(12504), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",              -x,  aFloat(-x), aFloat(-x), aFloat(-x) );
        break;
        case 920:
            TRICE64( Id(15269), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n", DoubleToInt64(y), aDouble(y), aDouble(y), aDouble(y) );
            TRICE64( Id(15892), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",     (int64_t)(y), aDouble(y), aDouble(y), aDouble(y) );
            TRICE64( Id(14349), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",               y,  aDouble(y), aDouble(y), aDouble(y) );
            TRICE64( Id(14890), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n", DoubleToInt64(-y), aDouble(-y), aDouble(-y), aDouble(-y) );
            TRICE64( Id(11107), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",     (int64_t)(-y), aDouble(-y), aDouble(-y), aDouble(-y) );
            TRICE64( Id(14322), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",               -y,  aDouble(-y), aDouble(-y), aDouble(-y) );
        break;
        case 930:
            TRICE( Id(12611), "att:Various single arguments\n" );
            TRICE8( Id(10785), "rd:TRICE8 line %t (%%t ,0)\n", 0 );
            TRICE8( Id(13667), "rd:TRICE8 line %t (%%t ,2)\n", 2 );
            TRICE8( Id(12365), "rd:TRICE8 line %u (%%u ,-1)\n", -1 );
            TRICE8( Id(15918), "rd:TRICE8 line %b (%%b ,-1)\n", -1 );
            TRICE8( Id(13159), "rd:TRICE8 line %o (%%o ,-1)\n", -1 );
            TRICE8( Id(10121), "rd:TRICE8 line %O (%%O ,-1)\n", -1 );
            TRICE8( Id(14858), "rd:TRICE8 line %X (%%X ,-1)\n", -1 );
            TRICE8( Id(11356), "rd:TRICE8 line %x (%%x ,-1)\n", -1 );
            TRICE8( Id(11699), "rd:TRICE8 line %d (%%d ,-1)\n", -1 );
        break;
        case 940:
            TRICE16_1( Id(14177), "rd:TRICE16_1 line %t (%%t ,0)\n", 0 );
            TRICE16_1( Id(15888), "rd:TRICE16_1 line %t (%%t ,2)\n", 2 );
            TRICE16_1( Id(11319), "rd:TRICE16_1 line %u (%%u -1)\n", -1 );
            TRICE16_1( Id(11212), "rd:TRICE16_1 line %b (%%b -1)\n", -1 );
            TRICE16_1( Id(11142), "rd:TRICE16_1 line %o (%%o -1)\n", -1 );
            TRICE16_1( Id(11545), "rd:TRICE16_1 line %x (%%x -1)\n", -1 );
        break;
        case 950:
            TRICE16_1( Id(13252), "rd:TRICE16_1 line %d (%%d -1)\n", -1 );
            TRICE( Id(15432), "rd:TRICE line %t (%%t -1)\n", 0 );
            TRICE( Id(10696), "rd:TRICE line %t (%%t -1)\n", 2 );
            TRICE( Id(10873), "rd:TRICE line %u (%%u)\n", -1 );
            TRICE( Id(12721), "rd:TRICE line %b (%%b)\n", -2 );
            TRICE( Id(10985), "rd:TRICE line %o (%%o)\n", -3 );
            TRICE( Id(12299), "rd:TRICE line %x (%%x)\n", -4 );
            TRICE( Id(14372), "rd:TRICE line %d (%%d)\n", -5 );
        break;
        case 960:
        break;
        case 970:
        break;
        case 980:
        break;
        case 990:
        break;
        case 1000:
#ifdef TRICE_ENCRYPT
        TRICE0 (Id(13748), "--------------------------------------------------\n" );
        {
            uint32_t by[8] = {1,2,3,4,5,6};
            TRICE32(  Id(15555), "msg: message = %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
            TRICE32( Id(13785), "tim: pre TriceEncryption SysTick=%d\n", SYSTICKVAL );
            TriceEncrypt(by, 6);
            TRICE32( Id(12789), "tim: post TriceEncryption SysTick=%d\n", SYSTICKVAL );
            TRICE32(  Id(12963), "att: TriceEncrypted =  %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
            #ifdef TRICE_DECRYPT
            TRICE16_1( Id(15083), "tim: pre TriceDecryption SysTick=%d\n", SYSTICKVAL );
            TriceDecrypt(by, 6);
            TRICE16_1( Id(14616), "tim: post TriceDecryption SysTick=%d\n", SYSTICKVAL );
            #endif
            TRICE32(  Id(13485), "msg: messge = %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
        }
        TRICE0 (Id(10438), "--------------------------------------------------\n" );
#endif
        break;
        default:
        break;
    }
}



//! SCOPY is a helper macro for struct serialization.
#define SCOPY( element ) do{ char* n = #element; int size = sizeof( src->element ); memcpy( p, &(src->element), size ); p += size; TRICE_S( Id(15774), "rd:sizeof(%8s)", n ); TRICE( Id(14491), " = %d\n", size);}while(0);

    //! DCOPY is a helper macro for struct deserialization.
#define DCOPY( element ) do{ char* n = #element; int size = sizeof( dst->element ); memcpy( &(dst->element), p, size ); p += size; TRICE_S( Id(11529), "rd:sizeof(%8s)", n ); TRICE( Id(14311), " = %d\n", size);}while(0);


typedef struct{
    float x;
    uint8_t rgb[3];
    float y;
} Point_t; //!< Point_t is small struct type.

static int serializePoint( char* dst, Point_t const * src ){
    char * p = dst;

    SCOPY( x )
    SCOPY( rgb )
    SCOPY( y )

    return p - dst;
}

static int deserializePoint( Point_t * const dst, char const * src ){
    char const * p = src;

    DCOPY( x )
    DCOPY( rgb )
    DCOPY( y )
    
    return p - src;
}

typedef struct{
    float z;
    uint16_t u;
      int8_t s;
    char* addr;
    uint32_t x;
     int32_t y;
    char names[3][5];
    Point_t point[2];
    uint64_t bitmask;
} Tryout_t; //!<  Tryout_t is a struct example embedding an other struct.



static int serializeTryout( char* dst, Tryout_t const * src ){
    char * p = dst;

    SCOPY( z )
    SCOPY( u )
    SCOPY( s )
    SCOPY( addr )
    SCOPY( x )
    SCOPY( y )
    SCOPY( names )
    p += serializePoint( p, src->point );
    SCOPY( bitmask )  

    return p - dst;
}

static int deserializeTryout( Tryout_t * const dst, char const * src ){
    char const * p = src;

    DCOPY( z )
    DCOPY( u )
    DCOPY( s )
    DCOPY( addr )
    DCOPY( x )
    DCOPY( y )
    DCOPY( names )
    p += deserializePoint( dst->point, p );
    DCOPY( bitmask )  

    return p - src;
}

static void exampleOfManualSerialization( void ){
    Tryout_t tx; // struct to transfer 
    Tryout_t rx; // "received" struct
    static char dst[100]; // serialized data
    char* src = dst; // "copy" - assume, data transferred now
    int len; // serialized byte count
    
    /////////////////////////////////////////////////////////
    // fill tx with data
    tx.z = 123.456;
    tx.u = 44444;
    tx.addr="Haus";
    tx.s = -2;
    tx.x = 0xaa55bb77;
    tx.y = -1000000;

    memcpy( tx.names[0], "aaa", strlen( "aaa" ) ); 
    memcpy( tx.names[1], "bbbb", strlen( "bbbb" ) ); 
    memcpy( tx.names[2], "ccccc", strlen( "ccccc" ) ); 

    tx.point[0].x = 2.22;
    tx.point[0].y = -3.33;
    tx.point[0].rgb[0] = 0x44;
    tx.point[0].rgb[0] = 0x66;
    tx.point[0].rgb[0] = 0x88;

    tx.point[1].x = -66.66;
    tx.point[1].y = +5.5555;
    tx.point[1].rgb[0] = 0xee;
    tx.point[1].rgb[0] = 0xaa;
    tx.point[1].rgb[0] = 0xbb;
    
    tx.bitmask = 0xAAAA55550000FFFF;
    //
    ////////////////////////////////////////////////////////
    
    len = serializeTryout( dst, &tx );
    TRICE ( Id(11707), "inf: Tryout tx struct:" );
    TRICE_B( Id(12118), " %02x ", &tx, sizeof(tx) );
    TRICE( Id(13397), "\n" );
    
    TRICE ( Id(10824), "inf: Tryout buffer:" );
    TRICE_B( Id(12368), " %02x ", dst, len );
    TRICE( Id(12788), "\n" );

    src = dst; // "data transfer"
    
    len = deserializeTryout( &rx, src );
    TRICE ( Id(13572), "inf: Tryout rx struct:" );
    TRICE_B( Id(13828), " %02x ", &rx, sizeof(rx) );
    TRICE( Id(15874), "\n" );

    TRICE( Id(13016), "inf:sizeOf(Trypout) = %d, buffer length = %d\n", sizeof(tx), len );
    TRICE8_F( Id(13204), "info:TryoutStructFunction", &tx, sizeof(tx) );
    TRICE8_F( Id(11449), "info:TryoutBufferFunction", dst, len ); 
}

static void exampleOfManualJSONencoding(void){
    typedef  struct {
    int Apple, Birn;
    float Fish;
    } Ex_t;
    Ex_t Ex = { -1, 2, 2.781 };
    TRICE( Id(10080), "att:MyStructEvaluationFunction(json:ExA{Apple:%d, Birn:%u, Fisch:%f}\n", Ex.Apple, Ex.Birn, aFloat(Ex.Fish) );
}

void exampleOfBuffersAndFunctions(void){
    static int8_t   b8[24] = { 0, -1, -2, 0x33, 4, 5, 6, 7, 8, 9, 10, 11, 0, -1, -2, 0x33, 4, 5, 6, 7, 8, 9, 10, 11 };
    static int16_t b16[] = { 0, -1, -2, 0x3344 };
    static int32_t b32[] = { 0, -1, -2, 0x33445555};
    static int64_t b64[4] = { 0, -1, -2, 0x3344555566666666 };
    char* s = "abcde 12345";
    uint32_t len;

    TRICE_S( Id(12969), "msg:With TRICE_S:%s\n", s );
    len = strlen(s);
    TRICE_N( Id(11129), "sig:With TRICE_N:%s\n", s, len );
    TRICE32( Id(13833), "att:len=%u:With TRICE_B:\n", len);
    TRICE_B( Id(11117), "  %02x", s, len );
    TRICE( Id(11198), "\n" );
    TRICE_B( Id(11737), "%4d", s, len );
    TRICE( Id(12243), "\n" );

    TRICE( Id(13779), "notice:TRICE_B example: " );
    TRICE_B( Id(13007), "  %02x", b8, sizeof(b8)/sizeof(int8_t) );
    TRICE( Id(11563), "\n" );
    TRICE( Id(15833), "notice:TRICE8_B example:" );
    TRICE8_B( Id(10950), "  %02x", b8, sizeof(b8)/sizeof(int8_t) );
    TRICE( Id(11627), "\n" );
    TRICE( Id(12313), "notice:TRICE16_B example:" );
    TRICE16_B( Id(15858), "  %04x", b16, sizeof(b16)/sizeof(int16_t) );
    TRICE( Id(15732), "\n" );
    TRICE( Id(11000), "notice:TRICE32_B example:" );
    TRICE32_B( Id(14261), "  %08x", b32, sizeof(b32)/sizeof(int32_t) );
    TRICE( Id(10899), "\n" );
    TRICE( Id(15590), "notice:TRICE64_B example:" );
    TRICE64_B( Id(14200), "  %016x", b64, sizeof(b64)/sizeof(int64_t) );
    TRICE( Id(13054), "\n" );    

    TRICE( Id(15244), "notice:TRICE_F example: " );
    TRICE_F( Id(11128), "info:FunctionNameW",   b8,  sizeof(b8) /sizeof(int8_t) );

    TRICE( Id(14291), "notice:TRICE8_F example:" );
    TRICE8_F(  Id(14297), "info:FunctionNameW",   b8,  sizeof(b8) /sizeof(int8_t) );

    TRICE( Id(13430), "notice:TRICE16_F example:" );
    TRICE16_F( Id(13631), "sig:FunctionNameX",    b16, sizeof(b16)/sizeof(int16_t) );

    TRICE( Id(15343), "notice:TRICE32_F example:" );
    TRICE32_F( Id(15744), "diag:FunctionNameY",   b32, sizeof(b32)/sizeof(int32_t) );

    TRICE( Id(12356), "notice:TRICE64_F example:" );
    TRICE64_F( Id(13209), "fatal:FunctionNameZ", b64, sizeof(b64)/sizeof(int64_t) );
}

static int32_t FloatToInt32( float f ){
  if( f >= 0 ){
          return (int32_t)f;
    }
    return -(int32_t)-f;
}

static int64_t DoubleToInt64( double f ){
  if( f >= 0 ){
          return (int64_t)f;
    }
    return -(int64_t)-f;
}

/*
#include "tcobs.h"

typedef struct{
    int ilen;
    uint8_t in[80];
    int olen;
    uint8_t out[80];
} TCOBSTestDataSet;

TCOBSTestDataSet TCOBSTestData[] = {
    { 1, { 0    }, 
      1, { 0x20 } },
    
    { 1, { 0xFF       }, 
      2, { 0xFF, 0xA1 } },

    { 6, {    0,    0,    0,    0,    0, 0xFF, },
      4, { 0x60, 0x40, 0xFF, 0xA1,             } },
    
    { 5, { 0xAA, 0x02, 0x02, 0x00, 0xFF,       },
      6, { 0xAA, 0x02, 0x02, 0x23, 0xFF, 0xA1, } },
    
    { 5, { 0xFF, 0x02, 0x02, 0x00, 0xFF,       },
      6, { 0xFF, 0x02, 0x02, 0x23, 0xFF, 0xA1, } },
    
};

#define TCOBS_TESTDATASET_COUNT (sizeof(TCOBSTestData) / sizeof(TCOBSTestDataSet) )

int equal( uint8_t* expBuf, int expLen, uint8_t* actBuf, int actLen ){
    int result = 1;
    if( expLen != actLen){
        return 0;
    }
    for( int i = 0; i < expLen; i++ ){
        TRICE( Id(15372), "msg:exp=%02x act=%02x\n", expBuf[i], actBuf[i] );
        if( expBuf[i] != actBuf[i] ){
            return 0;
        }
    }
    return result;
}

void TCOBSCheck( void ){
    uint8_t result[80];
    for( int i = 0; i < TCOBS_TESTDATASET_COUNT; i++ ){
        TRICE( Id(11798), "dbg: Set %d\n", i );
        int rlen = TCOBSEncode( result, TCOBSTestData[i].in, TCOBSTestData[i].ilen );
        if( !equal( TCOBSTestData[i].out, TCOBSTestData[i].olen, result, rlen ) ){
            TRICE( Id(11450), "ERROR: TCOBS!\n" );
        }
    }
}
*/
