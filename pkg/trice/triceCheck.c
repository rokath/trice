/*! \file triceCheck.c
\brief trices for tool evaluation
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/
#include <float.h>
//#define TRICE_OFF // enable this line to disable trice code generation in this file object
#include "trice.h"
//#define TRICE_FILE Id(11004)

//lint -e666 -e826 -e831 -e665

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
    float  x = (float)1089.6082763671875; // 0x44883377
    double y = 518.0547492508867; // 0x4080307020601050
     
    switch (index) {
        case 10:
          //TRICE( Id( 6349), "FATAL:magenta+b:red\n" );
          //TRICE( Id( 1405), "CRITICAL:red+i:default+h\n" );
          //TRICE( Id( 3028), "EMERGENCY:red+i:blue\n" );
          //TRICE( Id( 1383), "ERROR:11:red\n" );
          //TRICE( Id( 3400), "WARNING:11+i:red\n" );
          //TRICE( Id( 1576), "ATTENTION:11:green\n" );
          //TRICE( Id( 7414), "INFO:cyan+b:default+h\n" );
          //TRICE( Id( 7058), "DEBUG:130+i\n" );
          //TRICE( Id( 6815), "TRACE:default+i:default+h\n" );
          //TRICE( Id( 3725), "TIME:blue+i:blue+h\n" );
          //TRICE( Id( 2295), "MESSAGE:green+h:black\n" );
          //TRICE( Id( 3836), "READ:black+i:yellow+h\n" );
          //TRICE( Id( 3301), "WRITE:black+u:yellow+h\n" );
          //TRICE( Id( 6412), "RECEIVE:black+h:black\n" );
          //TRICE( Id( 6459), "TRANSMIT:black:black+h\n" );
          //TRICE( Id( 2678), "DIAG:yellow+i:default+h\n" );
          //TRICE( Id( 7819), "INTERRUPT:magenta+i:default+h\n" );
          //TRICE( Id( 3388), "SIGNAL:118+i\n" );
          //TRICE( Id( 5984), "TEST:yellow+h:black\n" );
          //TRICE( Id( 6480), "DEFAULT:off\n" );
          //TRICE( Id( 6380), "NOTICE:blue:white+h\n" );
          //TRICE( Id( 6378), "ALERT:magenta:magenta+h\n" );
          //TRICE( Id( 6613), "ASSERT:yellow+i:blue\n" );
          //TRICE( Id( 5723), "ALARM:red+i:white+h\n" );
          //TRICE( Id( 1583), "CYCLE:blue+i:default+h\n" );
            TRICE( Id( 3108), "VERBOSE:blue:default\n" );
        break;
        case 20:
          //TRICE( Id( 2436), "fatal:magenta+b:red\n" );
          //TRICE( Id( 7900), "critical:red+i:default+h\n" );
          //TRICE( Id( 1696), "emergency:red+i:blue\n" );
          //TRICE( Id( 5617), "error:11:red\n" );
          //TRICE( Id( 2364), "warning:11+i:red\n" );
          //TRICE( Id( 2762), "attention:11:green\n" );
          //TRICE( Id( 1384), "info:cyan+b:default+h\n" );
          //TRICE( Id( 2356), "debug:130+i\n" );
          //TRICE( Id( 2427), "trace:default+i:default+h\n" );
          //TRICE( Id( 7007), "tme:blue+i:blue+h\n" );
          //TRICE( Id( 4925), "message:green+h:black\n" );
          //TRICE( Id( 7940), "read:black+i:yellow+h\n" );
          //TRICE( Id( 3408), "write:black+u:yellow+h\n" );
          //TRICE( Id( 7924), "receive:black+h:black\n" );
          //TRICE( Id( 1632), "transmit:black:black+h\n" );
          //TRICE( Id( 7153), "diag:yellow+i:default+h\n" );
          //TRICE( Id( 2439), "interrupt:magenta+i:default+h\n" );
          //TRICE( Id( 1041), "signal:118+i\n" );
          //TRICE( Id( 6127), "test:yellow+h:black\n" );
          //TRICE( Id( 2908), "default:off\n" );
          //TRICE( Id( 7643), "notice:blue:white+h\n" );
          //TRICE( Id( 5837), "allert:magenta:magenta+h\n" );
          //TRICE( Id( 4982), "assert:yellow+i:blue\n" );
          //TRICE( Id( 2778), "alarm:red+i:white+h\n" );
          //TRICE( Id( 4158), "cycle:blue+i:default+h\n" );
            TRICE( Id( 2977), "verbose:blue:default\n" );
        break;
        case 30:
            TRICE( Id( 5523), "dbg:Hi!\n" );
            TRICE( Id( 5187), "dbg:\tHi!\n" );
            TRICE( Id( 5729), "dbg:\t\tHi!\n" );
            TRICE( Id( 4140), "dbg:\t\t\tHi!\n" );
            TRICE( Id( 4572), "dbg:\\tHi!\n" );
            TRICE( Id( 2666), "dbg:\aHi!\n" );
            TRICE( Id( 7714), "dbg:\a\aHi!\n" );
            TRICE( Id( 3019), "dbg:\a\a\aHi!\n" );
            TRICE( Id( 2390), "dbg:\\aHi!\n" );
        break;
        case 40:{
            unsigned len = strlen(s);
            TRICE32( Id( 4751), "dbg:len=%u:", len );
            TRICE_S( Id( 4008), "sig:TRICE_S=%s\n", s );
            TRICE32( Id( 2452), "dbg:len=%u:", len);
            TRICE_N( Id( 3138), "sig:TRICE_N=%s\n", s, len );
            //TRICE_S( Id( 4077), "sig:TRICE_S=%s\n", "Hello String" );     // not allowed
            //TRICE_N( Id( 6578), "sig:TRICE_N=%s\n", "Hello Buffer", 13 ); // not allowed

            TRICE( Id( 6700), "rd:TRICE line %E (%%E)\n", aFloat(-555555555.5555555555) );
            TRICE( Id( 1556), "rd:TRICE line %F (%%F)\n", aFloat(-555555555.5555555555) );
            TRICE( Id( 6051), "rd:TRICE line %G (%%G)\n", aFloat(-555555555.5555555555) );
        }
        break;
        case 50:
            TRICE32_1( Id( 4858), "rd:TRICE32_1 line %u (%%u)\n", -1 );
            TRICE32_1( Id( 7625), "rd:TRICE32_1 line %b (%%b)\n", -1 );
            TRICE32_1( Id( 7462), "rd:TRICE32_1 line %o (%%o)\n", -1 );
            TRICE32_1( Id( 2111), "rd:TRICE32_1 line %O (%%O)\n", -1 );
            TRICE32_1( Id( 1891), "rd:TRICE32_1 line %X (%%X)\n", -1 );
            TRICE32_1( Id( 5275), "rd:TRICE32_1 line %x (%%x)\n", -1 );
            TRICE32_1( Id( 4556), "rd:TRICE32_1 line %d (%%d)\n", -1 );
        break;
        case 60:
            TRICE64_1( Id( 2327), "rd:TRICE64_1 line %u (%%u)\n", -1 );
            TRICE64_1( Id( 7678), "rd:TRICE64_1 line %b (%%b)\n", -1 );
            TRICE64_1( Id( 4952), "rd:TRICE64_1 line %o (%%o)\n", -1 );
            TRICE64_1( Id( 4729), "rd:TRICE64_1 line %O (%%O)\n", -1 );
            TRICE64_1( Id( 6229), "rd:TRICE64_1 line %X (%%C)\n", -1 );
            TRICE64_1( Id( 5865), "rd:TRICE64_1 line %x (%%x)\n", -1 );
            TRICE64_1( Id( 5716), "rd:TRICE64_1 line %d (%%d)\n", -1 );
        break;
        case 70:
            TRICE64( Id( 5514), "rd:TRICE64 line %E (%%E)\n", aDouble(-555555555.5555555555) );
            TRICE64( Id( 7914), "rd:TRICE64 line %F (%%F)\n", aDouble(-555555555.5555555555) );
            TRICE64( Id( 2704), "rd:TRICE64 line %G (%%G)\n", aDouble(-555555555.5555555555) );
        break;
        case 80:
            TRICE( Id( 7588), "sig:Some time measurements\n" );
            TRICE( Id( 5656), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
            TRICE( Id( 4553), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
            TRICE( Id( 7948), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
            TRICE( Id( 1195), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
            TRICE64( Id( 4862), "rd:TRICE64 %d, %d\n", 1, 2 );
        break;
        case 90:
            TRICE( Id( 6250), "tim:TRICE START time message\n");
            TRICE( Id( 7131), "tim:TRICE STOP time message\n");
            TRICE( Id( 5089), "tim:TRICE START time message\n");
            TRICE( Id( 3999), "tim:TRICE STOP time message\n");
            TRICE( Id( 4697), "tim:TRICE START time message\n");
            TRICE64( Id( 1687), "rd:TRICE64 %d, %d\n", 1, 2 );
            TRICE( Id( 5913), "tim:TRICE STOP time message\n");
        break;
        case 100:
          //TRICE( Id( 2719), "tim:TRICE --------------------------------------------------\n");
          //TRICE( Id( 5522), "tim:TRICE --------------------------------------------------\n");
          //TRICE( Id( 1376), "tim:TRICE --------------------------------------------------\n");
          //TRICE( Id( 4978), "tim:TRICE --------------------------------------------------\n");
            TRICE( Id( 2357), "tim:TRICE --------------------------------------------------\n");
            TRICE( Id( 1360), "tim:TRICE --------------------------------------------------\n");
            TRICE64( Id( 7652), "rd:TRICE64 %d, %d\n", 1, 2 );
        break;
        case 110:
          //TRICE( Id( 5426), "tim:TRICE --------------------------------------------------\n");
          //TRICE( Id( 6496), "tim:TRICE --------------------------------------------------\n");
          //TRICE( Id( 3769), "tim:TRICE --------------------------------------------------\n");
          //TRICE( Id( 7675), "tim:TRICE --------------------------------------------------\n");
          //TRICE( Id( 1590), "tim:TRICE --------------------------------------------------\n");
            TRICE( Id( 3333), "tim:TRICE --------------------------------------------------\n");
            TRICE( Id( 4993), "tim:TRICE --------------------------------------------------\n");
            TRICE64( Id( 5204), "rd:TRICE64 %d, %d\n", 1, 2 );
        break;
        case 120:
            TRICE( Id( 3849),  "rd:TRICE32_1 line %d\n", __LINE__ );
            TRICE64_1( Id( 6298),  "rd:TRICE64_1 %d\n", __LINE__ );
            TRICE32_2( Id( 5387),  "rd:TRICE32_2 line %d,%d\n", __LINE__, 2 );
            TRICE64_2( Id( 4989),  "rd:TRICE64_2 line %d,%d\n", __LINE__, 2 );
        break;
        case 130:
            TRICE( Id( 3878), "sig:TRICE with 1 to 12 values\n" );
            TRICE( Id( 4866),"rd:TRICE %d\n", -1 );
      //    TRICE( Id( 3556),"rd:TRICE %d, %d\n", -1, -2 );
      //    TRICE( Id( 4758),"rd:TRICE %d, %d, %d\n", -1, -2, -3 );
      //    TRICE( Id( 5471),"rd:TRICE %d, %d, %d, %d\n", -1, -2, -3, -4 );
      //    TRICE( Id( 3677),"rd:TRICE %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
      //    TRICE( Id( 2592),"rd:TRICE %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
      //    TRICE( Id( 4443),"rd:TRICE %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
      //    TRICE( Id( 1995),"rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
      //break;
      //case 140:
      //    TRICE( Id( 2650),"rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
      //    TRICE( Id( 3605),"rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
      //    TRICE( Id( 7910),"rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
      //
      //    TRICE( Id( 7850),"rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 150:
            TRICE( Id( 7134), "sig:TRICE_n with 1 to 12 values\n" );
      //    TRICE_1( Id( 4728),   "rd:TRICE_1 %d\n", 1 );
      //    TRICE_2( Id( 4790),   "rd:TRICE_2 %d, %d\n", 1, 2 );
      //    TRICE_3( Id( 4433),   "rd:TRICE_3 %d, %d, %d\n", 1, 2, 3 );
      //    TRICE_4( Id( 6486),   "rd:TRICE_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
      //    TRICE_5( Id( 4666),   "rd:TRICE_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
      //    TRICE_6( Id( 2146),   "rd:TRICE_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
      //    TRICE_7( Id( 1027),   "rd:TRICE_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
      //    TRICE_8( Id( 3727),   "rd:TRICE_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
      //break;
      //case 160:
      //    TRICE_9( Id( 3650),   "rd:TRICE_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
      //    TRICE_10( Id( 2890),  "rd:TRICE_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
      //    TRICE_11( Id( 7833),  "rd:TRICE_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
            TRICE_12( Id( 2679),  "rd:TRICE_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );				
        break;
        case 170:
            TRICE( Id( 2143), "sig:TRICE8 with 1 to 12 values\n" );
      //    TRICE8( Id( 3599),"rd:TRICE8 %d\n", -1 );
      //    TRICE8( Id( 4694),"rd:TRICE8 %d, %d\n", -1, -2 );
      //    TRICE8( Id( 5343),"rd:TRICE8 %d, %d, %d\n", -1, -2, -3 );
      //    TRICE8( Id( 3469),"rd:TRICE8 %d, %d, %d, %d\n", -1, -2, -3, -4 );
      //    TRICE8( Id( 5074),"rd:TRICE8 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
      //    TRICE8( Id( 4582),"rd:TRICE8 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
      //    TRICE8( Id( 1448),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
      //    TRICE8( Id( 1018),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
      //break;
      //case 180:
      //    TRICE8( Id( 6904),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
      //    TRICE8( Id( 6018),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
      //    TRICE8( Id( 5799),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE8( Id( 6621),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 190:
            TRICE( Id( 2098), "sig:TRICE8_n with 1 to 12 values\n" );
           TRICE8_1( Id( 2984),   "rd:TRICE8_1 %d\n", 1 );
     //    TRICE8_2( Id( 4347),   "rd:TRICE8_2 %d, %d\n", 1, 2 );
     //    TRICE8_3( Id( 3502),   "rd:TRICE8_3 %d, %d, %d\n", 1, 2, 3 );
     //    TRICE8_4( Id( 1030),   "rd:TRICE8_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
     //    TRICE8_5( Id( 1910),   "rd:TRICE8_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
     //    TRICE8_6( Id( 6507),   "rd:TRICE8_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
     //    TRICE8_7( Id( 6160),   "rd:TRICE8_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
     //break;
     //case 200:
     //    TRICE8_8( Id( 2168),   "rd:TRICE8_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
     //    TRICE8_9( Id( 3877),   "rd:TRICE8_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
     //    TRICE8_10( Id( 1907),  "rd:TRICE8_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
     //    TRICE8_11( Id( 6335),  "rd:TRICE8_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
            TRICE8_12( Id( 3782),  "rd:TRICE8_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );
        break;
        case 210:
            TRICE( Id( 5676), "sig:TRICE16 with 1 to 12 values\n" );
            TRICE16( Id( 7556),"rd:TRICE16 %d\n", -1 );
     //    TRICE16( Id( 6660),"rd:TRICE16 %d, %d\n", -1, -2 );
     //    TRICE16( Id( 5441),"rd:TRICE16 %d, %d, %d\n", -1, -2, -3 );
     //    TRICE16( Id( 7967),"rd:TRICE16 %d, %d, %d, %d\n", -1, -2, -3, -4 );
     //    TRICE16( Id( 1881),"rd:TRICE16 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
     //    TRICE16( Id( 6895),"rd:TRICE16 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
     //    TRICE16( Id( 7823),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
     //    TRICE16( Id( 1189),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
     //break;
     //case 220:
     //    TRICE16( Id( 7111),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
     //    TRICE16( Id( 1207),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
     //    TRICE16( Id( 5742),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE16( Id( 2052),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 230:
            TRICE( Id( 2234), "sig:TRICE16_n with 1 to 12 values\n" );
      //    TRICE16_1( Id( 7286),  "rd:TRICE16_1 %d\n", 1 );
      //    TRICE16_2( Id( 6109),  "rd:TRICE16_2 %d, %d\n", 1, 2 );
      //    TRICE16_3( Id( 4502),  "rd:TRICE16_3 %d, %d, %d\n", 1, 2, 3 );
      //    TRICE16_4( Id( 7546),  "rd:TRICE16_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
      //    TRICE16_5( Id( 2906),  "rd:TRICE16_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
      //    TRICE16_6( Id( 7229),  "rd:TRICE16_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
      //    TRICE16_7( Id( 7497),  "rd:TRICE16_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
      //    TRICE16_8( Id( 3323),  "rd:TRICE16_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
      //break;
      //case 240:
      //    TRICE16_9( Id( 3291),  "rd:TRICE16_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
      //    TRICE16_10( Id( 3729), "rd:TRICE16_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
      //    TRICE16_11( Id( 3947), "rd:TRICE16_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
            TRICE16_12( Id( 5693), "rd:TRICE16_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );
        break;
        case 250:
            TRICE( Id( 7366), "sig:TRICE16 with 1 to 12 pointer\n" );
            TRICE16( Id( 6189),"rd:TRICE16 %p\n", -1 );
      //    TRICE16( Id( 7603),"rd:TRICE16 %p, %p\n", -1, -2 );
      //    TRICE16( Id( 3491),"rd:TRICE16 %p, %p, %p\n", -1, -2, -3 );
      //    TRICE16( Id( 3430),"rd:TRICE16 %p, %p, %p, %p\n", -1, -2, -3, -4 );
      //    TRICE16( Id( 5028),"rd:TRICE16 %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5 );
      //    TRICE16( Id( 3256),"rd:TRICE16 %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6 );
      //    TRICE16( Id( 7927),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7);
      //    TRICE16( Id( 7406),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8 );
      //break;
      //case 260:
      //    TRICE16( Id( 2952),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
      //    TRICE16( Id( 5181),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
      //    TRICE16( Id( 4123),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE16( Id( 6389),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 270:
            TRICE( Id( 3475), "sig:TRICE16 with 1 to 12 hex\n" );
            TRICE16( Id( 5515),"rd:TRICE16 %X\n", -1 );
      //    TRICE16( Id( 6950),"rd:TRICE16 %X, %X\n", -1, -2 );
      //    TRICE16( Id( 5530),"rd:TRICE16 %X, %X, %X\n", -1, -2, -3 );
      //    TRICE16( Id( 3395),"rd:TRICE16 %X, %X, %X, %X\n", -1, -2, -3, -4 );
      //    TRICE16( Id( 2649),"rd:TRICE16 %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5 );
      //    TRICE16( Id( 4779),"rd:TRICE16 %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6 );
      //    TRICE16( Id( 2337),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7);
      //    TRICE16( Id( 3738),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8 );
      //break;
      //case 280:
      //    TRICE16( Id( 4700),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
      //    TRICE16( Id( 4462),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
      //    TRICE16( Id( 2632),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE16( Id( 4841),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 290:
            TRICE( Id( 6227), "sig:TRICE32 with 1 to 12 values\n" );
      //    TRICE32( Id( 7717),"rd:TRICE32 %d\n", -1 );
      //    TRICE32( Id( 5888),"rd:TRICE32 %d, %d\n", -1, -2 );
      //    TRICE32( Id( 5920),"rd:TRICE32 %d, %d, %d\n", -1, -2, -3 );
      //    TRICE32( Id( 6254),"rd:TRICE32 %d, %d, %d, %d\n", -1, -2, -3, -4 );
      //    TRICE32( Id( 6182),"rd:TRICE32 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
      //    TRICE32( Id( 3810),"rd:TRICE32 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
      //    TRICE32( Id( 7146),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
      //    TRICE32( Id( 3046),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
      //break;
      //case 300:
      //    TRICE32( Id( 4954),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
      //    TRICE32( Id( 2153),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
      //    TRICE32( Id( 2469),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE32( Id( 2463),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 310:
            TRICE( Id( 3950), "signal:TRICE32_n with 1 to 12 values\n" );
            TRICE32_1( Id( 6376),  "rd:TRICE32_1 %d\n", 1 );
      //    TRICE32_2( Id( 2145),  "rd:TRICE32_2 %d, %d\n", 1, 2 );
      //    TRICE32_3( Id( 1500),  "rd:TRICE32_3 %d, %d, %d\n", 1, 2, 3 );
      //    TRICE32_4( Id( 7886),  "rd:TRICE32_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
      //    TRICE32_5( Id( 4679),  "rd:TRICE32_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
      //    TRICE32_6( Id( 6630),  "rd:TRICE32_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
      //    TRICE32_7( Id( 1835),  "rd:TRICE32_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
      //    TRICE32_8( Id( 4591),  "rd:TRICE32_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
      //break;
      //case 320:
      //    TRICE32_9( Id( 4391),  "rd:TRICE32_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
      //    TRICE32_10( Id( 6036), "rd:TRICE32_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
      //    TRICE32_11( Id( 6209), "rd:TRICE32_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
            TRICE32_12( Id( 7348), "rd:TRICE32_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );
        break;
        case 330:
            TRICE( Id( 5653), "sig:TRICE64 with 1 to 12 values\n" );
            TRICE64( Id( 2048),"rd:TRICE64 %d\n", -1 );
      //    TRICE64( Id( 7185),"rd:TRICE64 %d, %d\n", -1, -2 );
      //    TRICE64( Id( 4484),"rd:TRICE64 %d, %d, %d\n", -1, -2, -3 );
      //    TRICE64( Id( 2833),"rd:TRICE64 %d, %d, %d, %d\n", -1, -2, -3, -4 );
      //    TRICE64( Id( 6569),"rd:TRICE64 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
      //    TRICE64( Id( 7315),"rd:TRICE64 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
      //    TRICE64( Id( 4552),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
      //    TRICE64( Id( 7912),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
      //break;
      //case 340:
      //    TRICE64( Id( 3158),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
      //    TRICE64( Id( 4798),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
      //    TRICE64( Id( 7373),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE64( Id( 3383),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 350:
            TRICE( Id( 6675), "sig:TRICE64_n with 1 to 12 values\n" );
            TRICE64_1( Id( 3786),  "rd:TRICE64_1 %d\n", 1 );
      //    TRICE64_2( Id( 7495),  "rd:TRICE64_2 %d, %d\n", 1, 2 );
      //    TRICE64_3( Id( 6439),  "rd:TRICE64_3 %d, %d, %d\n", 1, 2, 3 );
      //    TRICE64_4( Id( 1804),  "rd:TRICE64_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
      //    TRICE64_5( Id( 6286),  "rd:TRICE64_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
      //    TRICE64_6( Id( 5781),  "rd:TRICE64_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
      //    TRICE64_7( Id( 1759),  "rd:TRICE64_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
      //    TRICE64_8( Id( 4756),  "rd:TRICE64_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
      //break;
      //case 360:
      //    TRICE64_9( Id( 4028),  "rd:TRICE64_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
      //    TRICE64_10( Id( 1423), "rd:TRICE64_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
      //    TRICE64_11( Id( 5379), "rd:TRICE64_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
            TRICE64_12( Id( 6468), "rd:TRICE64_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );
        break;
        case 370:
            TRICE( Id( 1511), "sig:Colors and ticks\n" );
            TRICE(Id( 6330), "--------------------------------------------------\n");
            TRICE(Id( 6793), "--------------------------------------------------\n");
            TRICE(Id( 3295), "dbg:12345 as 16bit is %016b\n", 12345);
            TRICE(Id( 3555), "--------------------------------------------------\n");
            TRICE(Id( 3503), "sig:This ASSERT error is just a demo and no real error:\n");
            TRICE(Id( 6111), "--------------------------------------------------\n");
        break;
        case 380:
            TRICE(Id( 4703), "ERR:error       message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id( 2211), "WRN:warning     message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id( 1743), "ATT:attention   message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id( 6123), "DIA:diagnostics message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id( 7873), "TIM:timing      message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id( 2829), "DBG:debug       message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id( 1434), "SIG:signal      message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id( 3486), "RD:read         message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id( 2633), "WR:write        message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id( 5499), "ISR:interrupt   message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id( 1047), "MSG:normal      message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id( 1711), "INFO:informal   message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id( 2308), "time:TRICE32_1  message, SysTick is %6u\n", SYSTICKVAL);
        break;
        case 390:
            TRICE( Id( 2274), "sig:Various ranges and formats\n" );
            TRICE8( Id( 1697), "tst:TRICE8  %%03x -> %03x  %03x  %03x  %03x\n", 1, 0x7f, 0x80, 0xff);
            TRICE8( Id( 7854), "tst:TRICE8   %%4d -> %4d %4d %4d %4d\n", 1, 0x7f, 0x80, 0xff);
            TRICE8( Id( 4614), "tst:TRICE8   %%4u -> %4u %4u %4u %4u\n", 1, 0x7f, 0x80, 0xff);
            TRICE8( Id( 1825), "tst:TRICE8   %%4o -> %4o %4o %4o %4o\n", 1, 0x7f, 0x80, 0xff);
            TRICE8( Id( 5935), "tst:TRICE8   %%4O -> %4O %4O %4O %4O\n", 1, 0x7f, 0x80, 0xff);
            TRICE8( Id( 2279), "tst:TRICE8   %%4X -> %4X %4X %4X %4X\n", 1, 0x7f, 0x80, 0xff);
            TRICE8( Id( 2468), "tst:TRICE8   %%4x -> %4x %4x %4x %4x\n", 1, 0x7f, 0x80, 0xff);
            TRICE8( Id( 1818), "tst:TRICE8   %%8b -> %8b %8b %8b %8b\n", 1, 0x7f, 0x80, 0xff);
            TRICE8( Id( 2467), "tst:TRICE8  %%08b -> %08b %08b %08b %08b\n", 1, 0x7f, 0x80, 0xff);
        break;
        case 400:
            TRICE16(Id( 1540), "tst:TRICE16  %%05x ->   %05x   %05x   %05x   %05x\n", 1, 0x7fff, 0x8000, 0xffff);
            TRICE16(Id( 2336), "tst:TRICE16   %%6d ->  %6d  %6d  %6d  %6d\n",         1, 0x7fff, 0x8000, 0xffff);
            TRICE16(Id( 3364), "tst:TRICE16   %%7o -> %7o %7o %7o %7o\n",             1, 0x7fff, 0x8000, 0xffff);

            TRICE32(Id( 6915), "tst:TRICE32 %%09x ->      %09x      %09x       %09x     %09x\n", 1, 0x7fffffff, 0x80000000, 0xffffffff);
            TRICE32(Id( 4021), "tst:TRICE32 %%10d ->     %10d     %10d     %10d    %10x\n",      1, 0x7fffffff, 0x80000000, 0xffffffff);
            TRICE32(Id( 3273), "att:TRICE32 %40b\n", 0xAAAAAAAA);
        break;
        case 410:
            TRICE64(Id( 5184), "tst:TRICE64 %%09x ->      %09x      %09x       %09x     %09x\n", 1, 0x7fffffffffffffff, 0x8000000000000000, 0xffffffffffffffff);
            TRICE64(Id( 1481), "tst:TRICE64 %%10d ->     %10d     %10d     %10d    %10x\n",      1, 0x7fffffffffffffff, 0x8000000000000000, 0xffffffffffffffff);
            TRICE64(Id( 4823), "att:TRICE64 %70b\n", 0xAAAAAAAAAAAAAAAA);
        break;
        case 420:
            TRICE( Id( 7619), "sig:Legacy TRICE8\n" );
            TRICE8_1(Id( 3523), "tst:TRICE8_1 %02x\n", 0xA1);
            TRICE8_2(Id( 5834), "tst:TRICE8_2 %02x %02x\n", 0xA1, 0xA2);
            TRICE8_3(Id( 1793), "tst:TRICE8_3 %02x %02x %02x\n", 0xA1, 0xA2, 0xA3);
            TRICE8_4(Id( 5289), "tst:TRICE8_4 %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4);
            TRICE8_5(Id( 2500), "tst:TRICE8_5 %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5 );
            TRICE8_6(Id( 2223), "tst:TRICE8_6 %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6);
            TRICE8_7(Id( 7179), "tst:TRICE8_7 %02x %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7);
            TRICE8_8(Id( 6583), "tst:TRICE8_8 %02x %02x %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8);
        break;
        case 430:
            TRICE8_1(Id( 6296), "tst:TRICE8_1 %d\n",                      -1);
            TRICE8_2(Id( 3535), "tst:TRICE8_2 %d %d\n",                   -1, -2);
            TRICE8_3(Id( 4657), "tst:TRICE8_3 %d %d %d\n",                -1, -2, -3);
            TRICE8_4(Id( 4133), "tst:TRICE8_4 %d %d %d %d\n",             -1, -2, -3, -4);
            TRICE8_5(Id( 7755), "tst:TRICE8_5 %d %d %d %d %d\n",          -1, -2, -3, -4, -5);
            TRICE8_6(Id( 6923), "tst:TRICE8_6 %d %d %d %d %d %d\n",       -1, -2, -3, -4, -5, -6);
            TRICE8_7(Id( 4417), "tst:TRICE8_7 %d %d %d %d %d %d %d\n",    -1, -2, -3, -4, -5, -6, -7);
            TRICE8_8(Id( 5247), "tst:TRICE8_8 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8);
        break;
        case 440:
            TRICE8_1(Id( 2238), "tst:TRICE8_1 %d\n", 1);
            TRICE8_2(Id( 3075), "tst:TRICE8_2 %d %d\n", 1, 2);
            TRICE8_3(Id( 1880), "tst:TRICE8_3 %d %d %d\n", 1, 2, 3);
            TRICE8_4(Id( 1603), "tst:TRICE8_4 %d %d %d %d\n", 1, 2, 3, 4);
            TRICE8_5(Id( 5136), "tst:TRICE8_5 %d %d %d %d %d\n", 1, 2, 3, 4, 5);
            TRICE8_6(Id( 4332), "tst:TRICE8_6 %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6);
            TRICE8_7(Id( 5876), "tst:TRICE8_7 %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7);
            TRICE8_8(Id( 6102), "tst:TRICE8_8 %d %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7, 8);
        break;
        case 450:
            TRICE8_1(Id( 4789), "tst:TRICE8_1 %u\n", 201);
            TRICE8_2(Id( 6144), "tst:TRICE8_2 %u %u\n", 201, 202);
            TRICE8_3(Id( 3784), "tst:TRICE8_3 %u %u %u\n", 201, 202, 203);
            TRICE8_4(Id( 4881), "tst:TRICE8_4 %u %u %u %u\n", 201, 202, 203, 204);
            TRICE8_5(Id( 5279), "tst:TRICE8_5 %u %u %u %u %u\n", 201, 202, 203, 204, 205);
            TRICE8_6(Id( 3559), "tst:TRICE8_6 %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206);
            TRICE8_7(Id( 2511), "tst:TRICE8_7 %u %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206, 207);
            TRICE8_8(Id( 1722), "tst:TRICE8_8 %u %u %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206, 207, 208);
            TRICE8_8(Id( 1664), "tst:TRICE8_1 %%d=%d, %%u=%u, 0x%%x=0x%x, 0x%%2x=0x%2x, 0x%%02x=0x%02x, 0x%%3x=0x%3x, 0x%%03x=0x%03x, %%b=%b\n", 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81);
        break;
        case 460:
            TRICE( Id( 3756), "sig:Legacy TRICE16-64\n" );
            TRICE16_1(Id( 7383), "tst:TRICE16_1 %d\n", -111);
            TRICE16_2(Id( 7776), "tst:TRICE16_2 %d %d\n", -111, -222);
            TRICE16_3(Id( 6354), "tst:TRICE16_3 %d %d %d\n", -111, -222, -333);
            TRICE16_4(Id( 1326), "tst:TRICE16_4 %d %d %d %d\n", -111, -222, -333, -444);
            TRICE32_1(Id( 2324), "tst:TRICE32_1 %08x\n", 0x0123cafe);
            TRICE32_1(Id( 1125), "tst:TRICE32_1 %d\n", -111);
            TRICE32_2(Id( 5311), "tst:TRICE32_2 %x %x\n", -111, -222);
            TRICE32_2(Id( 4517), "tst:TRICE32_2 %d %d\n", -111, -222);
        break;
        case 470:
            TRICE32_3(Id( 5333), "tst:TRICE32_3 %x %x %x\n", -111, -222, -333);
            TRICE32_3(Id( 3719), "tst:TRICE32_3 %d %d %d\n", -111, -222, -333);
            TRICE32_4(Id( 3339), "tst:TRICE32_4 %x %x %x %x\n", -111, -222, -333, -444);
            TRICE32_4(Id( 2793), "tst:TRICE32_4 %d %d %d %d\n", -111, -222, -333, -444);
        break;
        case 480:
            TRICE64_1(Id( 5087), "tst:TRICE64_1 %d\n", -111);
            TRICE64_2(Id( 6804), "tst:TRICE64_2 %d %d\n", -111, -222);
            TRICE16_1(Id( 6238), "tst:TRICE16_1 %u\n", 60001);
            TRICE16_2(Id( 3525), "tst:TRICE16_2 %u %u\n", 60001, 60002);
            TRICE16_3(Id( 1149), "tst:TRICE16_3 %u %u %u\n", 60001, 60002, 60003);
            TRICE16_4(Id( 1615), "tst:TRICE16_4 %u %u %u %u\n", 60001, 60002, 60003, 60004);
            TRICE32_1(Id( 6399), "tst:TRICE32_1 %u\n", 4000000001);
            TRICE32_2(Id( 4355), "tst:TRICE32_2 %u %u\n", 4000000001, 4000000002);
            TRICE32_3(Id( 2508), "tst:TRICE32_3 %u %u %u\n", 4000000001, 4000000002, 4000000003);
            TRICE32_4(Id( 3017), "tst:TRICE32_4 %u %u %u %u\n", 4000000001, 4000000002, 4000000003, 4000000004);
        break;
        case 490:
            TRICE64_1(Id( 3178), "tst:TRICE64_1 %x\n", -1);
            TRICE64_2(Id( 1084), "tst:TRICE64_2 %x %x\n", -1, -2);
            TRICE64_1(Id( 2928), "tst:TRICE64_1 %u\n", -1);
            TRICE64_2(Id( 4708), "tst:TRICE64_2 %u %u\n", -1, -2);
        break;
        case 500:
            TRICE( Id( 5575), "sig:colored single letters and several TRICE macros in one line\n" );
            TRICE(Id( 5038), "e:A");
            TRICE(Id( 4422), "w:B");
            TRICE(Id( 7429), "a:c");
            TRICE(Id( 6397), "wr:d");
            TRICE(Id( 2174), "rd:e\n");
            TRICE(Id( 2644), "diag:f");
            TRICE(Id( 3865), "d:G");
            TRICE(Id( 1647), "t:H");
            TRICE(Id( 2712), "time:i");
            TRICE(Id( 2193), "message:J");
            TRICE(Id( 4920), "dbg:k\n");
        break;
        case 510:
            TRICE(Id( 2362), "1");
            TRICE(Id( 6946), "2");
            TRICE(Id( 5706), "3");
            TRICE(Id( 4416), "4");
            TRICE(Id( 5418), "e:7");
            TRICE(Id( 3713), "m:12");
            TRICE(Id( 7916), "m:123\n");
            TRICE(Id( 1267), "e:A");TRICE(Id( 1172), "w:B");TRICE(Id( 4340), "a:c");
            TRICE(Id( 5470), "wr:d");TRICE(Id( 3465), "rd:e\n");//TRICE(Id( 1549), "diag:f");
        break;
        case 520:
            TRICE( Id( 5349), "sig:Runtime generated strings\n" );
            s = "AAAAAAAAAAAA";
            TRICE32( Id( 1952), "dbg:len=%u:", strlen(s) );
            TRICE_S( Id( 4584), "sig:%s\n", s );
            s = "\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBCCCCCCCC\
";
            TRICE32( Id( 7152), "dbg:len=%u:", strlen(s) );
            TRICE_S( Id( 6629), "sig:%s\n", s );
        break;
        case 530:
        TRICE( Id( 3508), "sig:Runtime generated strings\n" );
            s = "\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\
DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\
EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\
";
            TRICE32( Id( 1116), "dbg:len=%u: ", strlen(s) );
            TRICE_S( Id( 4039), "sig:%s\n", s );
        break;
        case 540:
            TRICE( Id( 1646), "sig:Runtime generated strings\n" );
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
            TRICE32( Id( 4220), "dbg:len=%u: ", strlen(s) );
            TRICE_S( Id( 5043), "sig:%s\n", s );
        break;
        case 550:
            TRICE( Id( 2971), "sig:Runtime generated strings\n" );
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
          TRICE32( Id( 3161), "dbg:len=%u: ", strlen(s) );
          TRICE_S( Id( 6833), "sig:%s\n", s );
        break;
        case 560:{ // https://yourbasic.org/golang/fmt-printf-reference-cheat-sheet/
            int i = 15;
            TRICE( Id( 7949), "sig:Integer (indent, base, sign)\n" );
            TRICE( Id( 5466), "rd: 15 	%d 	Base 10\n", i );
            TRICE( Id( 4392), "rd: +15 	%+d 	Always show sign\n", i );
            TRICE( Id( 3350), "rd:   15 	%4d 	Pad with spaces (width 4, right justified)\n", i );
            TRICE( Id( 1169), "rd: 15   	%-4d 	Pad with spaces (width 4, left justified)\n", i );
            TRICE( Id( 2925), "rd: 0015 	%04d 	Pad with zeroes (width 4)\n", i );
            TRICE( Id( 2805), "rd: 1111 	%b 	Base 2\n", i );
            TRICE( Id( 4802), "rd: 17 	%o 	Base 8\n", i );
            TRICE( Id( 1155), "rd: f 	%x 	Base 16, lowercase\n", i );
            TRICE( Id( 4138), "rd: F 	%X 	Base 16, uppercase\n", i );
            TRICE( Id( 3224), "rd: 0xf 	%#x 	Base 16, with leading 0x\n", i );
        }
        break;
        case 570:{
            char a = 'A';
            TRICE( Id( 3187), "sig:Character (quoted, Unicode)\n" );
            TRICE( Id( 6983), "rd: A 	%c 	Character\n", a );
            TRICE( Id( 1023), "rd: 'A' 	%q 	Quoted character\n", a );
          //TRICE( Id( 1241), "rd: U+0041 	%U 	Unicode\n", a );// todo
          //TRICE( Id( 3603), "rd: U+0041 'A' 	%#U 	Unicode with character\n", a );// todo
            TRICE( Id( 1310), "sig:Boolean (true/false)\n" );
            TRICE( Id( 5947), "rd:Use %%t to format a boolean as true (%t) or false (%t).\n", 1, 0 );
            TRICE( Id( 4523), "sig:Pointer (hex)\n" );
        }
        break;
        case 580:
            TRICE8( Id( 2825), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x84 );
            TRICE16( Id( 5540), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x1234 );
            TRICE32( Id( 2951), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0xaabbccdd );
            TRICE64( Id( 1948), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x1122334455667788 );
            TRICE( Id( 3073), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0xaabbccdd);
        break;
        case 590:{
            float f = (float)123.456; 
            TRICE( Id( 4335), "sig:Float (indent, precision, scientific notation)\n" );
            TRICE( Id( 4117), "rd: 1.234560e+02		%e 	%%e Scientific notation\n", aFloat(f) );
            TRICE( Id( 7650), "rd: 123.456000		%f 	%%f Decimal point, no exponent\n", aFloat(f) );
            TRICE( Id( 1525), "rd: 123.46	 		%.2f		%%.2f Default width, precision 2\n", aFloat(f) );
            TRICE( Id( 7039), "rd: ␣␣123.46		%8.2f	%%8.2f Width 8, precision 2\n", aFloat(f) );
            TRICE( Id( 4016), "rd: 123.456		%g		%%g Exponent as needed, necessary digits only\n", aFloat(f) );
        }
        break;
        case 600:{
            double d = 123.456;
            TRICE( Id( 6863), "sig:Double (indent, precision, scientific notation)\n" );
            TRICE64( Id( 5906), "rd: 1.234560e+02		%e 	%%e Scientific notation\n", aDouble(d) );
            TRICE64( Id( 1460), "rd: 123.456000		%f 	%%f Decimal point, no exponent\n", aDouble(d) );
            TRICE64( Id( 3009), "rd: 123.46			%.2f    	%%.2f Default width, precision 2\n", aDouble(d) );
            TRICE64( Id( 2606), "rd: ␣␣123.46		%8.2f 	%%8.2f Width 8, precision 2\n", aDouble(d) );
            TRICE64( Id( 2119), "rd: 123.456		%g		%%g Exponent as needed, necessary digits only\n", aDouble(d) );
        }
        break;
        case 610:{
            s = "café";
            TRICE( Id( 4640), "sig:String or byte slice (quote, indent, hex)\n" );
            TRICE_S( Id( 1223), "rd: café 			%s		Plain string\n", s );
            TRICE_S( Id( 5956), "rd: ␣␣café 		%6s 		Width 6, right justify\n", s );
            TRICE_S( Id( 2872), "rd: café␣␣ 		%-6s 		Width 6, left justify\n", s );
            TRICE_S( Id( 3714), "rd: \"café\" 		%q	 	Quoted string\n", s );
            TRICE_S( Id( 3015), "rd: 636166c3a9 		%x	Hex dump of byte values\n", s );
            TRICE_S( Id( 4709), "rd: 63 61 66 c3 a9 	% x	Hex dump with spaces\n", s );
        }
        break;
        case 620:
            TRICE( Id( 6501), "sig:TRICE8 with variable param count 1 to 12\n" );
            TRICE8(Id( 5463), "tst:TRICE8 %d\n", -1 );
            TRICE8(Id( 2658), "tst:TRICE8 %d %d\n", -1, -2 );
            TRICE8(Id( 6492), "tst:TRICE8 %d %d %d\n", -1, -2, -3 );
            TRICE8(Id( 2568), "tst:TRICE8 %d %d %d %d\n", -1, -2, -3, -4 );
            TRICE8(Id( 7071), "tst:TRICE8 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
            TRICE8(Id( 1971), "tst:TRICE8 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE8(Id( 6977), "tst:TRICE8 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
            TRICE8(Id( 3980), "tst:TRICE8 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 630:
            TRICE8(Id( 1290), "tst:TRICE8 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE8(Id( 1025), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
            TRICE8(Id( 2061), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE8(Id( 7476), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 640:
            TRICE8(Id( 3570), "tst:TRICE8 %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );
            TRICE8_1 (Id( 2216), "tst:TRICE8_1  %d\n", -1 );
            TRICE8_2 (Id( 4176), "tst:TRICE8_2  %d %d\n", -1, -2 );
            TRICE8_3 (Id( 5177), "tst:TRICE8_3  %d %d %d\n", -1, -2, -3 );
            TRICE8_4 (Id( 6424), "tst:TRICE8_4  %d %d %d %d\n", -1, -2, -3, -4 );
            TRICE8_5 (Id( 3893), "tst:TRICE8_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
            TRICE8_6 (Id( 1843), "tst:TRICE8_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE8_7 (Id( 4948), "tst:TRICE8_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
            TRICE8_8 (Id( 5533), "tst:TRICE8_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 650:
            TRICE8_9 (Id( 3394), "tst:TRICE8_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE8_10(Id( 6777), "tst:TRICE8_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
            TRICE8_11(Id( 5070), "tst:TRICE8_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE8_12(Id( 4169), "tst:TRICE8_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
            TRICE8  (Id( 1369), "tst:TRICE8   %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );
            TRICE8_9(Id( 5430), "tst:TRICE8_9 %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );
        break;
        case 660:
            TRICE( Id( 2927), "sig:TRICE16 with variable param count 1 to 12\n" );
            TRICE16(Id( 3296), "tst:TRICE16 %d\n", -1 );
            TRICE16(Id( 1499), "tst:TRICE16 %d %d\n", -1, -2 );
            TRICE16(Id( 3068), "tst:TRICE16 %d %d %d\n", -1, -2, -3 );
            TRICE16(Id( 4675), "tst:TRICE16 %d %d %d %d\n", -1, -2, -3, -4 );
            TRICE16(Id( 3902), "tst:TRICE16 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
            TRICE16(Id( 2828), "tst:TRICE16 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE16(Id( 2524), "tst:TRICE16 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
            TRICE16(Id( 1173), "tst:TRICE16 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 670:
            TRICE16(Id( 1219), "tst:TRICE16 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE16(Id( 7666), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
            TRICE16(Id( 1066), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE16(Id( 4771), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 680:
            TRICE16(Id( 3546), "tst:TRICE16 %t %b %x %X %d %u %o %O %p\n", 3, 3, 3, 3, 3, 3, 3, 3, 3 );
            TRICE16_1 (Id( 4240), "tst:TRICE16_1  %d\n", -1 );
            TRICE16_2 (Id( 7174), "tst:TRICE16_2  %d %d\n", -1, -2 );
            TRICE16_3 (Id( 3468), "tst:TRICE16_3  %d %d %d\n", -1, -2, -3 );
            TRICE16_4 (Id( 6961), "tst:TRICE16_4  %d %d %d %d\n", -1, -2, -3, -4 );
            TRICE16_5 (Id( 2755), "tst:TRICE16_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
            TRICE16_6 (Id( 6547), "tst:TRICE16_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE16_7 (Id( 6044), "tst:TRICE16_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
            TRICE16_8 (Id( 5472), "tst:TRICE16_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 690:
            TRICE16_9 (Id( 2470), "tst:TRICE16_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE16_10(Id( 4319), "tst:TRICE16_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
            TRICE16_11(Id( 5049), "tst:TRICE16_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE16_12(Id( 4372), "tst:TRICE16_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
            TRICE16_9(Id( 1512), "tst:TRICE16 %t %b %x %X %d %u %o %O %p\n", 3, 3, 3, 3, 3, 3, 3, 3, 3 );
        break;
        case 700:
            TRICE( Id( 2068), "sig:TRICE32 with variable param count 1 to 12\n" );
            TRICE32(Id( 6607), "tst:TRICE32 %d\n", -1 );
            TRICE32(Id( 2861), "tst:TRICE32 %d %d\n", -1, -2 );
            TRICE32(Id( 2101), "tst:TRICE32 %d %d %d\n", -1, -2, -3 );
            TRICE32(Id( 1589), "tst:TRICE32 %d %d %d %d\n", -1, -2, -3, -4 );
            TRICE32(Id( 7063), "tst:TRICE32 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
            TRICE32(Id( 4295), "tst:TRICE32 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE32(Id( 6135), "tst:TRICE32 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
            TRICE32(Id( 4127), "tst:TRICE32 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 710:
            TRICE32(Id( 4662), "tst:TRICE32 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE32(Id( 3899), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
            TRICE32(Id( 6145), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE32(Id( 3825), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 720:
            TRICE32(Id( 2159), "tst:TRICE32 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));
            TRICE32_1 (Id( 1361), "tst:TRICE32_1  %d\n", -1 );
            TRICE32_2 (Id( 4898), "tst:TRICE32_2  %d %d\n", -1, -2 );
            TRICE32_3 (Id( 2377), "tst:TRICE32_3  %d %d %d\n", -1, -2, -3 );
            TRICE32_4 (Id( 5408), "tst:TRICE32_4  %d %d %d %d\n", -1, -2, -3, -4 );
            TRICE32_5 (Id( 4219), "tst:TRICE32_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
            TRICE32_6 (Id( 4475), "tst:TRICE32_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE32_7 (Id( 6341), "tst:TRICE32_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
            TRICE32_8 (Id( 1509), "tst:TRICE32_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 730:
            TRICE32_9 (Id( 2319), "tst:TRICE32_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE32_10(Id( 3349), "tst:TRICE32_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
            TRICE32_11(Id( 4370), "tst:TRICE32_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE32_12(Id( 5152), "tst:TRICE32_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
            TRICE32   (Id( 1491), "tst:TRICE32    %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));
            TRICE32_12(Id( 4046), "tst:TRICE32_12 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));
        break;
        case 740:
            TRICE( Id( 3243), "sig:TRICE64 with variable param count 1 to 12\n" );
            TRICE64(Id( 1703), "tst:TRICE64 %d\n", -1 );
            TRICE64(Id( 7212), "tst:TRICE64 %d %d\n", -1, -2 );
            TRICE64(Id( 4635), "tst:TRICE64 %d %d %d\n", -1, -2, -3 );
            TRICE64(Id( 4632), "tst:TRICE64 %d %d %d %d\n", -1, -2, -3, -4 );
            TRICE64(Id( 5282), "tst:TRICE64 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
            TRICE64(Id( 6310), "tst:TRICE64 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE64(Id( 7566), "tst:TRICE64 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
            TRICE64(Id( 2030), "tst:TRICE64 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 750:
            TRICE64(Id( 1893), "tst:TRICE64 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE64(Id( 2683), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
            TRICE64(Id( 1251), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE64(Id( 5597), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 760:
            TRICE64(Id( 4848), "tst:TRICE64 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aDouble(3.14159), aDouble(3.14159), aDouble(3.14159));
            TRICE64_1 (Id( 7196), "tst:TRICE64_1  %d\n", -1 );
            TRICE64_2 (Id( 7537), "tst:TRICE64_2  %d %d\n", -1, -2 );
            TRICE64_3 (Id( 6614), "tst:TRICE64_3  %d %d %d\n", -1, -2, -3 );
            TRICE64_4 (Id( 6228), "tst:TRICE64_4  %d %d %d %d\n", -1, -2, -3, -4 );
            TRICE64_5 (Id( 2724), "tst:TRICE64_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
            TRICE64_6 (Id( 4919), "tst:TRICE64_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE64_7 (Id( 3806), "tst:TRICE64_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
            TRICE64_8 (Id( 4490), "tst:TRICE64_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 770:
            TRICE64_9 (Id( 2455), "tst:TRICE64_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE64_10(Id( 2299), "tst:TRICE64_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
            TRICE64_11(Id( 4006), "tst:TRICE64_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE64_12(Id( 1479), "tst:TRICE64_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
            TRICE64_12(Id( 7164), "tst:TRICE64_12 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aDouble(3.14159), aDouble(3.14159), aDouble(3.14159));
        break;
        case 780:
            TRICE( Id( 3979), "att:positive and negative float in format variants\n");
            TRICE( Id( 3230), "rd:TRICE float %f (%%f)\n", aFloat(x) );
            TRICE( Id( 1787), "rd:TRICE float %9f (%%9f)\n", aFloat(x) );
            TRICE( Id( 7335), "rd:TRICE float %.9f (%%.9f)\n", aFloat(x) );
            TRICE( Id( 4394), "rd:TRICE float %9.f (%%9.f)\n", aFloat(x) );
            TRICE( Id( 6280), "rd:TRICE float %9.6f (%%9.6f)\n", aFloat(x) );
            TRICE( Id( 3188), "rd:TRICE float %f (%%f)\n", aFloat(-x) );
        break;
        case 790:
            TRICE( Id( 7222), "rd:TRICE float %9f (%%9f)\n", aFloat(-x) );
            TRICE( Id( 3264), "rd:TRICE float %.9f (%%.9f)\n", aFloat(-x) );
            TRICE( Id( 2682), "rd:TRICE float %9.f (%%9.f)\n", aFloat(-x) );
            TRICE( Id( 2546), "rd:TRICE float %9.6f (%%9.6f)\n", aFloat(-x) );
            TRICE( Id( 3498), "rd:TRICE float %+f (%%f)\n", aFloat(x) );
            TRICE( Id( 2497), "rd:TRICE float %+9f (%%9f)\n", aFloat(x) );
            TRICE( Id( 1443), "rd:TRICE float %+.9f (%%.9f)\n", aFloat(x) );
            TRICE( Id( 2694), "rd:TRICE float %+9.f (%%9.f)\n", aFloat(x) );
        break;
        case 800:
            TRICE( Id( 7875), "rd:TRICE float %+9.6f (%%9.6f)\n", aFloat(x) );
            TRICE( Id( 3173), "rd:TRICE float %+f (%%f)\n", aFloat(-x) );
            TRICE( Id( 3597), "rd:TRICE float %+9f (%%9f)\n", aFloat(-x) );
            TRICE( Id( 6995), "rd:TRICE float %+.9f (%%.9f)\n", aFloat(-x) );
            TRICE( Id( 5104), "rd:TRICE float %+9.f (%%9.f)\n", aFloat(-x) );
            TRICE( Id( 2263), "rd:TRICE float %+9.6f (%%9.6f)\n", aFloat(-x) );
            TRICE( Id( 1356), "rd:TRICE float %-f (%%f)\n", aFloat(x) );
            TRICE( Id( 3064), "rd:TRICE float %-9f (%%9f)\n", aFloat(x) );
        break;
        case 810:
            TRICE( Id( 1099), "rd:TRICE float %-.9f (%%.9f)\n", aFloat(x) );
            TRICE( Id( 4934), "rd:TRICE float %-9.f (%%9.f)\n", aFloat(x) );
            TRICE( Id( 4917), "rd:TRICE float %-9.6f (%%9.6f)\n", aFloat(x) );
            TRICE( Id( 7896), "rd:TRICE float %-f (%%f)\n", aFloat(-x) );
            TRICE( Id( 4446), "rd:TRICE float %-9f (%%9f)\n", aFloat(-x) );
            TRICE( Id( 7626), "rd:TRICE float %-.9f (%%.9f)\n", aFloat(-x) );
            TRICE( Id( 1446), "rd:TRICE float %-9.f (%%9.f)\n", aFloat(-x) );
            TRICE( Id( 2196), "rd:TRICE float %-9.6f (%%9.6f)\n", aFloat(-x) );
        break;
        case 820:
            TRICE( Id( 3341), "att:positive float & double in variants\n");
            TRICE32_1( Id( 6958), "rd:TRICE32_1 float %e (%%e)\n", aFloat(x) );
            TRICE32_1( Id( 7730), "rd:TRICE32_1 float %f (%%f)\n", aFloat(x) );
            TRICE32_1( Id( 5240), "rd:TRICE32_1 float %g (%%g)\n", aFloat(x) );
            TRICE32_1( Id( 7045), "rd:TRICE32_1 float %E (%%E)\n", aFloat(x) );
            TRICE32_1( Id( 5997), "rd:TRICE32_1 float %F (%%F)\n", aFloat(x) );
            TRICE32_1( Id( 2852), "rd:TRICE32_1 float %G (%%G)\n", aFloat(x) );
        break;
        case 830:
            TRICE32( Id( 4290), "rd:TRICE32 float %e (%%e)\n", aFloat(x) );
            TRICE32( Id( 7786), "rd:TRICE32 float %f (%%f)\n", aFloat(x) );
            TRICE32( Id( 3760), "rd:TRICE32 float %g (%%g)\n", aFloat(x) );
            TRICE32( Id( 5377), "rd:TRICE32 float %E (%%E)\n", aFloat(x) );
            TRICE32( Id( 3359), "rd:TRICE32 float %F (%%F)\n", aFloat(x) );
            TRICE32( Id( 2134), "rd:TRICE32 float %G (%%G)\n", aFloat(x) );
        break;
        case 840:
            TRICE( Id( 6698), "rd:TRICE float %e (%%e)\n", aFloat(x) );
            TRICE( Id( 2834), "rd:TRICE float %f (%%f)\n", aFloat(x) );
            TRICE( Id( 1076), "rd:TRICE float %g (%%g)\n", aFloat(x) );
            TRICE( Id( 1501), "rd:TRICE float %E (%%E)\n", aFloat(x) );
            TRICE( Id( 2071), "rd:TRICE float %F (%%F)\n", aFloat(x) );
            TRICE( Id( 4095), "rd:TRICE float %G (%%G)\n", aFloat(x) );
        break;
        case 850:
            TRICE64( Id( 6179), "rd:TRICE64 double %e (%%e), aDouble(y)\n", aDouble(y) );
            TRICE64( Id( 5934), "rd:TRICE64 double %f (%%f), aDouble(y)\n", aDouble(y) );
            TRICE64( Id( 5425), "rd:TRICE64 double %g (%%g), aDouble(y)\n", aDouble(y) );
            TRICE64( Id( 6599), "rd:TRICE64 double %E (%%E), aDouble(y)\n", aDouble(y) );
            TRICE64( Id( 2904), "rd:TRICE64 double %F (%%F), aDouble(y)\n", aDouble(y) );
            TRICE64( Id( 5794), "rd:TRICE64 double %G (%%G), aDouble(y)\n", aDouble(y) );
        break;
        case 860:
            TRICE64_1( Id( 6901), "rd:TRICE64_1 double %e (%%e), aDouble(y)\n", aDouble(y) );
            TRICE64_1( Id( 1145), "rd:TRICE64_1 double %f (%%f), aDouble(y)\n", aDouble(y) );
            TRICE64_1( Id( 5079), "rd:TRICE64_1 double %g (%%g), aDouble(y)\n", aDouble(y) );
            TRICE64_1( Id( 7765), "rd:TRICE64_1 double %E (%%E), aDouble(y)\n", aDouble(y) );
            TRICE64_1( Id( 6043), "rd:TRICE64_1 double %F (%%F), aDouble(y)\n", aDouble(y) );
            TRICE64_1( Id( 7354), "rd:TRICE64_1 double %G (%%G), aDouble(y)\n", aDouble(y) );
        break;
        case 870:
            TRICE( Id( 5548), "att:negative float & double\n" );
            TRICE( Id( 3751), "rd:TRICE float %e (%%e)\n", aFloat(-x) );
            TRICE( Id( 2305), "rd:TRICE float %f (%%f)\n", aFloat(-x) );
            TRICE( Id( 7936), "rd:TRICE float %g (%%g)\n", aFloat(-x) );
            TRICE( Id( 3136), "rd:TRICE float %E (%%E)\n", aFloat(-x) );
            TRICE( Id( 7437), "rd:TRICE float %F (%%F)\n", aFloat(-x) );
            TRICE( Id( 7665), "rd:TRICE float %G (%%G)\n", aFloat(-x) );
        break;
        case 880:
            TRICE64( Id( 1564), "rd:TRICE64 double %e (%%e), aDouble(y)\n", aDouble(-y) );
            TRICE64( Id( 3663), "rd:TRICE64 double %f (%%f), aDouble(y)\n", aDouble(-y) );
            TRICE64( Id( 6114), "rd:TRICE64 double %g (%%g), aDouble(y)\n", aDouble(-y) );
            TRICE64( Id( 5751), "rd:TRICE64 double %E (%%E), aDouble(y)\n", aDouble(-y) );
            TRICE64( Id( 5197), "rd:TRICE64 double %F (%%F), aDouble(y)\n", aDouble(-y) );
            TRICE64( Id( 4845), "rd:TRICE64 double %G (%%G), aDouble(y)\n", aDouble(-y) );
        break;
        case 890:
            TRICE( Id( 3996), "att:formatted float & double\n" );
            TRICE32( Id( 2413), "rd:TRICE32 float %12.6e (%%12.6e)\n", aFloat(x) );
            TRICE32( Id( 5825), "rd:TRICE32 float %12.6f (%%12.6f)\n", aFloat(x) );
            TRICE32( Id( 6319), "rd:TRICE32 float %12.6g (%%12.6g)\n", aFloat(x) );
            TRICE32( Id( 2383), "rd:TRICE32 float %12.6E (%%12.6E)\n", aFloat(x) );
            TRICE32( Id( 1317), "rd:TRICE32 float %12.6F (%%12.6F)\n", aFloat(x) );
            TRICE32( Id( 2285), "rd:TRICE32 float %12.6G (%%12.6G)\n", aFloat(x) );
        break;
        case 900:
            TRICE64( Id( 4356), "rd:TRICE64 double %12.6e (%%12.6e), aDouble(y)\n", aDouble(y) );
            TRICE64( Id( 5824), "rd:TRICE64 double %12.6f (%%12.6f), aDouble(y)\n", aDouble(y) );
            TRICE64( Id( 6257), "rd:TRICE64 double %12.6g (%%12.6g), aDouble(y)\n", aDouble(y) );
            TRICE64( Id( 3279), "rd:TRICE64 double %12.6E (%%12.6E), aDouble(y)\n", aDouble(y) );
            TRICE64( Id( 1457), "rd:TRICE64 double %12.6F (%%12.6F), aDouble(y)\n", aDouble(y) );
            TRICE64( Id( 6089), "rd:TRICE64 double %12.6G (%%12.6G), aDouble(y)\n", aDouble(y) );
        break;
        case 910:
            TRICE( Id( 6107), "att:mixed int & float & double & bits\n" );
            TRICE32( Id( 6161), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n", FloatToInt32(x), aFloat(x), aFloat(x), aFloat(x) );
            TRICE32( Id( 4134), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",    (int32_t)(x), aFloat(x), aFloat(x), aFloat(x) );
            TRICE32( Id( 4041), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",              x,  aFloat(x), aFloat(x), aFloat(x) );
            TRICE32( Id( 1984), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n", FloatToInt32(-x), aFloat(-x), aFloat(-x), aFloat(-x) );
            TRICE32( Id( 7627), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",    (int32_t)(-x), aFloat(-x), aFloat(-x), aFloat(-x) );
            TRICE32( Id( 4067), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",              -x,  aFloat(-x), aFloat(-x), aFloat(-x) );
        break;
        case 920:
            TRICE64( Id( 5497), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n", DoubleToInt64(y), aDouble(y), aDouble(y), aDouble(y) );
            TRICE64( Id( 6680), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",     (int64_t)(y), aDouble(y), aDouble(y), aDouble(y) );
            TRICE64( Id( 2481), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",               y,  aDouble(y), aDouble(y), aDouble(y) );
            TRICE64( Id( 2611), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n", DoubleToInt64(-y), aDouble(-y), aDouble(-y), aDouble(-y) );
            TRICE64( Id( 3277), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",     (int64_t)(-y), aDouble(-y), aDouble(-y), aDouble(-y) );
            TRICE64( Id( 3855), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",               -y,  aDouble(-y), aDouble(-y), aDouble(-y) );
        break;
        case 930:
            TRICE( Id( 5490), "att:Various single arguments\n" );
            TRICE8( Id( 3420), "rd:TRICE8 line %t (%%t ,0)\n", 0 );
            TRICE8( Id( 3209), "rd:TRICE8 line %t (%%t ,2)\n", 2 );
            TRICE8( Id( 7747), "rd:TRICE8 line %u (%%u ,-1)\n", -1 );
            TRICE8( Id( 1314), "rd:TRICE8 line %b (%%b ,-1)\n", -1 );
            TRICE8( Id( 1601), "rd:TRICE8 line %o (%%o ,-1)\n", -1 );
            TRICE8( Id( 7608), "rd:TRICE8 line %O (%%O ,-1)\n", -1 );
            TRICE8( Id( 5736), "rd:TRICE8 line %X (%%X ,-1)\n", -1 );
            TRICE8( Id( 4439), "rd:TRICE8 line %x (%%x ,-1)\n", -1 );
            TRICE8( Id( 1717), "rd:TRICE8 line %d (%%d ,-1)\n", -1 );
        break;
        case 940:
            TRICE16_1( Id( 1551), "rd:TRICE16_1 line %t (%%t ,0)\n", 0 );
            TRICE16_1( Id( 3415), "rd:TRICE16_1 line %t (%%t ,2)\n", 2 );
            TRICE16_1( Id( 7874), "rd:TRICE16_1 line %u (%%u -1)\n", -1 );
            TRICE16_1( Id( 4296), "rd:TRICE16_1 line %b (%%b -1)\n", -1 );
            TRICE16_1( Id( 3504), "rd:TRICE16_1 line %o (%%o -1)\n", -1 );
            TRICE16_1( Id( 5923), "rd:TRICE16_1 line %x (%%x -1)\n", -1 );
        break;
        case 950:
            TRICE16_1( Id( 7881), "rd:TRICE16_1 line %d (%%d -1)\n", -1 );
            TRICE( Id( 5459), "rd:TRICE line %t (%%t -1)\n", 0 );
            TRICE( Id( 6948), "rd:TRICE line %t (%%t -1)\n", 2 );
            TRICE( Id( 5126), "rd:TRICE line %u (%%u)\n", -1 );
            TRICE( Id( 4334), "rd:TRICE line %b (%%b)\n", -2 );
            TRICE( Id( 7405), "rd:TRICE line %o (%%o)\n", -3 );
            TRICE( Id( 6452), "rd:TRICE line %x (%%x)\n", -4 );
            TRICE( Id( 1609), "rd:TRICE line %d (%%d)\n", -5 );
        break;
        case 960:
        {
            s = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
            for( int i = 0; i < 17; i++ ){ //strlen(s) ){
                TRICE_N( Id( 2076), "msg:%s\n", s, i );
            }
            for( int i = 125; i < 130; i++ ){ //strlen(s) ){
                TRICE_N( Id( 4551), "msg:%s\n", s, i );
            }
        }
        break;
        case 970:
        exampleOfManualSerialization();
        break;
        case 980:
        exampleOfManualJSONencoding();
        break;
        case 990:
        exampleOfBuffersAndFunctions();
        break;
        case 1000:
#ifdef TRICE_ENCRYPT
        TRICE0 (Id( 1074), "--------------------------------------------------\n" );
        {
            uint32_t by[8] = {1,2,3,4,5,6};
            TRICE32(  Id( 6601), "msg: message = %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
            TRICE32( Id( 4856), "tim: pre TriceEncryption SysTick=%d\n", SYSTICKVAL );
            TriceEncrypt(by, 6);
            TRICE32( Id( 6448), "tim: post TriceEncryption SysTick=%d\n", SYSTICKVAL );
            TRICE32(  Id( 7547), "att: TriceEncrypted =  %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
            #ifdef TRICE_DECRYPT
            TRICE16_1( Id( 3168), "tim: pre TriceDecryption SysTick=%d\n", SYSTICKVAL );
            TriceDecrypt(by, 6);
            TRICE16_1( Id( 6610), "tim: post TriceDecryption SysTick=%d\n", SYSTICKVAL );
            #endif
            TRICE32(  Id( 2686), "msg: messge = %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
        }
        TRICE0 (Id( 6572), "--------------------------------------------------\n" );
#endif
        break;
        default:
        break;
    }
}



//! SCOPY is a helper macro for struct serialization.
#define SCOPY( element ) do{ char* n = #element; int size = sizeof( src->element ); memcpy( p, &(src->element), size ); p += size; TRICE_S( Id( 5219), "rd:sizeof(%8s)", n ); TRICE( Id( 6313), " = %d\n", size);}while(0);

    //! DCOPY is a helper macro for struct deserialization.
#define DCOPY( element ) do{ char* n = #element; int size = sizeof( dst->element ); memcpy( &(dst->element), p, size ); p += size; TRICE_S( Id( 1805), "rd:sizeof(%8s)", n ); TRICE( Id( 1656), " = %d\n", size);}while(0);


//  typedef struct{
//      float x;
//      float y;
//      uint8_t rgb[3];
//      //float z; // it seems, that the compiler does not align this with -o3 & time !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//  } Point_t; //!< Point_t is small struct type.
//  
//  static int serializePoint( char* dst, Point_t const * src ){
//      char * p = dst;
//  
//      SCOPY( x )
//      SCOPY( y )
//      SCOPY( rgb )
//  
//      return p - dst;
//  }
//  
//  static int deserializePoint( Point_t * const dst, char const * src ){
//      char const * p = src;
//  
//      DCOPY( x )
//      DCOPY( y )
//      DCOPY( rgb )
//      
//      return p - src;
//  }

typedef struct{
    float z;
    uint16_t u;
      int8_t s;
    char* addr;
    uint32_t x;
     int32_t y;
    char names[3][5];
    //Point_t point[2];
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
    //p += serializePoint( p, src->point );
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
    //p += deserializePoint( dst->point, p );
    DCOPY( bitmask )  

    return p - src;
}

static void exampleOfManualSerialization( void ){
    Tryout_t tx; // struct to transfer 
    Tryout_t rx; // "received" struct
    static char dst[100]; // serialized data
    char* src; // "copy" - assume, data transferred now
    int len; // serialized byte count
    
    /////////////////////////////////////////////////////////
    // fill tx with data
    tx.z = (float)123.456;
    tx.u = 44444;
    tx.addr="Haus";
    tx.s = -2;
    tx.x = 0xaa55bb77;
    tx.y = -1000000;

    memcpy( tx.names[0], "aaa", strlen( "aaa" ) ); 
    memcpy( tx.names[1], "bbbb", strlen( "bbbb" ) ); 
    memcpy( tx.names[2], "ccccc", strlen( "ccccc" ) ); 

    //  tx.point[0].x = 2.22;
    //  tx.point[0].y = -3.33;
    //  tx.point[0].rgb[0] = 0x44;
    //  tx.point[0].rgb[0] = 0x66;
    //  tx.point[0].rgb[0] = 0x88;
    //  
    //  tx.point[1].x = -66.66;
    //  tx.point[1].y = +5.5555;
    //  tx.point[1].rgb[0] = 0xee;
    //  tx.point[1].rgb[0] = 0xaa;
    //  tx.point[1].rgb[0] = 0xbb;
    
    tx.bitmask = 0xAAAA55550000FFFF;
    //
    ////////////////////////////////////////////////////////
    
    len = serializeTryout( dst, &tx );
    TRICE ( Id( 2475), "inf: Tryout tx struct:" );
    TRICE_B( Id( 1242), " %02x ", &tx, sizeof(tx) );
    TRICE( Id( 4720), "\n" );
    
    TRICE ( Id( 6405), "inf: Tryout buffer:" );
    TRICE_B( Id( 2946), " %02x ", dst, len ); //lint !e670
    TRICE( Id( 1286), "\n" );

    src = dst; // "data transfer"
    
    len = deserializeTryout( &rx, src );
    TRICE ( Id( 3643), "inf: Tryout rx struct:" );
    TRICE_B( Id( 5110), " %02x ", &rx, sizeof(rx) );
    TRICE( Id( 5626), "\n" );

    TRICE( Id( 3795), "inf:sizeOf(Trypout) = %d, buffer length = %d\n", sizeof(tx), len );
    TRICE8_F( Id( 7149), "info:TryoutStructFunction", &tx, sizeof(tx) );
    TRICE8_F( Id( 6861), "info:TryoutBufferFunction", dst, len ); //lint !e670
}

static void exampleOfManualJSONencoding(void){
    typedef  struct {
    int Apple, Birn;
    float Fish;
    } Ex_t;
    Ex_t Ex = { -1, 2, (float)2.781 };
    TRICE( Id( 7099), "att:MyStructEvaluationFunction(json:ExA{Apple:%d, Birn:%u, Fisch:%f}\n", Ex.Apple, Ex.Birn, aFloat(Ex.Fish) );
}

static void exampleOfBuffersAndFunctions(void){
    static int8_t   b8[24] = { 0, -1, -2, 0x33, 4, 5, 6, 7, 8, 9, 10, 11, 0, -1, -2, 0x33, 4, 5, 6, 7, 8, 9, 10, 11 };
    static int16_t b16[] = { 0, -1, -2, 0x3344 };
    static int32_t b32[] = { 0, -1, -2, 0x33445555};
    static int64_t b64[4] = { 0, -1, -2, 0x3344555566666666 };
    char* s = "abcde 12345";
    uint32_t len;

    TRICE_S( Id( 4155), "msg:With TRICE_S:%s\n", s );
    len = strlen(s);
    TRICE_N( Id( 4492), "sig:With TRICE_N:%s\n", s, len );
    TRICE32( Id( 1762), "att:len=%u:With TRICE_B:\n", len);
    TRICE_B( Id( 1417), "  %02x", s, len );
    TRICE( Id( 4024), "\n" );
    TRICE_B( Id( 5752), "%4d", s, len );
    TRICE( Id( 5809), "\n" );

    TRICE( Id( 2667), "notice:TRICE_B example: " );
    TRICE_B( Id( 5946), "  %02x", b8, sizeof(b8)/sizeof(int8_t) );
    TRICE( Id( 5583), "\n" );
    TRICE( Id( 5610), "notice:TRICE8_B example:" );
    TRICE8_B( Id( 7200), "  %02x", b8, sizeof(b8)/sizeof(int8_t) );
    TRICE( Id( 5082), "\n" );
    TRICE( Id( 6880), "notice:TRICE16_B example:" );
    TRICE16_B( Id( 5108), "  %04x", b16, sizeof(b16)/sizeof(int16_t) );
    TRICE( Id( 1033), "\n" );
    TRICE( Id( 3004), "notice:TRICE32_B example:" );
    TRICE32_B( Id( 6794), "  %08x", b32, sizeof(b32)/sizeof(int32_t) );
    TRICE( Id( 4063), "\n" );
    TRICE( Id( 4715), "notice:TRICE64_B example:" );
    TRICE64_B( Id( 2579), "  %016x", b64, sizeof(b64)/sizeof(int64_t) );
    TRICE( Id( 7169), "\n" );    

    TRICE( Id( 7498), "notice:TRICE_F example: " );
    TRICE_F( Id( 7599), "info:FunctionNameW",   b8,  sizeof(b8) /sizeof(int8_t) );
    //shown as: TRICE_F example: FunctionNameW(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)
    
    TRICE( Id( 7278), "notice:TRICE8_F example:" );
    TRICE8_F(  Id( 3582), "info:FunctionNameW",   b8,  sizeof(b8) /sizeof(int8_t) );
    //shown as: TRICE8_F example:FunctionNameW(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)
    
    TRICE( Id( 5972), "notice:TRICE16_F example:" );
    TRICE16_F( Id( 1217), "sig:FunctionNameX",    b16, sizeof(b16)/sizeof(int16_t) );
    //shown as: TRICE16_F example:FunctionNameX(0000)(ffff)(fffe)(3344) 

    TRICE( Id( 4307), "notice:TRICE32_F example:" );
    TRICE32_F( Id( 7953), "diag:FunctionNameY",   b32, sizeof(b32)/sizeof(int32_t) );
    //shown as: TRICE32_F example:FunctionNameY(00000000)(ffffffff)(fffffffe)(33445555)

    TRICE( Id( 5362), "notice:TRICE64_F example:" );
    TRICE64_F( Id( 1744), "fatal:FunctionNameZ", b64, sizeof(b64)/sizeof(int64_t) );
    //shown as: TRICE64_F example:FunctionNameZ(0000000000000000)(ffffffffffffffff)(fffffffffffffffe)(3344555566666666)
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
