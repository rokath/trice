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
            TRICE( Id(0), "FATAL:magenta+b:red\n" );
            TRICE( Id(0), "CRITICAL:red+i:default+h\n" );
            TRICE( Id(0), "EMERGENCY:red+i:blue\n" );
            TRICE( Id(0), "ERROR:11:red\n" );
            TRICE( Id(0), "WARNING:11+i:red\n" );
            TRICE( Id(0), "ATTENTION:11:green\n" );
            TRICE( Id(0), "INFO:cyan+b:default+h\n" );
            TRICE( Id(0), "DEBUG:130+i\n" );
            TRICE( Id(0), "TRACE:default+i:default+h\n" );
            TRICE( Id(0), "TIME:blue+i:blue+h\n" );
            TRICE( Id(0), "MESSAGE:green+h:black\n" );
            TRICE( Id(0), "READ:black+i:yellow+h\n" );
            TRICE( Id(0), "WRITE:black+u:yellow+h\n" );
            TRICE( Id(0), "RECEIVE:black+h:black\n" );
            TRICE( Id(0), "TRANSMIT:black:black+h\n" );
            TRICE( Id(0), "DIAG:yellow+i:default+h\n" );
            TRICE( Id(0), "INTERRUPT:magenta+i:default+h\n" );
            TRICE( Id(0), "SIGNAL:118+i\n" );
            TRICE( Id(0), "TEST:yellow+h:black\n" );
            TRICE( Id(0), "DEFAULT:off\n" );
            TRICE( Id(0), "NOTICE:blue:white+h\n" );
            TRICE( Id(0), "ALERT:magenta:magenta+h\n" );
            TRICE( Id(0), "ASSERT:yellow+i:blue\n" );
            TRICE( Id(0), "ALARM:red+i:white+h\n" );
            TRICE( Id(0), "CYCLE:blue+i:default+h\n" );
            TRICE( Id(0), "VERBOSE:blue:default\n" );
        break;
        case 20:
            TRICE( Id(0), "fatal:magenta+b:red\n" );
            TRICE( Id(0), "critical:red+i:default+h\n" );
            TRICE( Id(0), "emergency:red+i:blue\n" );
            TRICE( Id(0), "error:11:red\n" );
            TRICE( Id(0), "warning:11+i:red\n" );
            TRICE( Id(0), "attention:11:green\n" );
            TRICE( Id(0), "info:cyan+b:default+h\n" );
            TRICE( Id(0), "debug:130+i\n" );
            TRICE( Id(0), "trace:default+i:default+h\n" );
            TRICE( Id(0), "tme:blue+i:blue+h\n" );
            TRICE( Id(0), "message:green+h:black\n" );
            TRICE( Id(0), "read:black+i:yellow+h\n" );
            TRICE( Id(0), "write:black+u:yellow+h\n" );
            TRICE( Id(0), "receive:black+h:black\n" );
            TRICE( Id(0), "transmit:black:black+h\n" );
            TRICE( Id(0), "diag:yellow+i:default+h\n" );
            TRICE( Id(0), "interrupt:magenta+i:default+h\n" );
            TRICE( Id(0), "signal:118+i\n" );
            TRICE( Id(0), "test:yellow+h:black\n" );
            TRICE( Id(0), "default:off\n" );
            TRICE( Id(0), "notice:blue:white+h\n" );
            TRICE( Id(0), "allert:magenta:magenta+h\n" );
            TRICE( Id(0), "assert:yellow+i:blue\n" );
            TRICE( Id(0), "alarm:red+i:white+h\n" );
            TRICE( Id(0), "cycle:blue+i:default+h\n" );
            TRICE( Id(0), "verbose:blue:default\n" );
        break;
        case 30:
            TRICE( Id(0), "dbg:Hi!\n" );
            TRICE( Id(0), "dbg:\tHi!\n" );
            TRICE( Id(0), "dbg:\t\tHi!\n" );
            TRICE( Id(0), "dbg:\t\t\tHi!\n" );
            TRICE( Id(0), "dbg:\\tHi!\n" );
            TRICE( Id(0), "dbg:\aHi!\n" );
            TRICE( Id(0), "dbg:\a\aHi!\n" );
            TRICE( Id(0), "dbg:\a\a\aHi!\n" );
            TRICE( Id(0), "dbg:\\aHi!\n" );
            //TCOBSCheck();
        break;
        case 40:{
            int len = strlen(s);
            TRICE32( Id(0), "dbg:len=%u:", len );
            TRICE_S( Id(0), "sig:TRICE_S=%s\n", s );
            TRICE32( Id(0), "dbg:len=%u:", len );
            TRICE_S( Id(0), "sig:TRICE_S=%s\n", s );
            TRICE32( Id(0), "dbg:len=%u:", len );
            TRICE_S( Id(0), "sig:TRICE_S=%s\n", s );
        
            TRICE32( Id(0), "dbg:len=%u:", len);
            TRICE_N( Id(0), "sig:TRICE_N=%s\n", s, len );
            TRICE32( Id(0), "dbg:len=%u:", len);
            TRICE_N( Id(0), "sig:TRICE_N=%s\n", s, len );
            TRICE32( Id(0), "dbg:len=%u:", len);
            TRICE_N( Id(0), "sig:TRICE_N=%s\n", s, len );
        
          //TRICE_S( Id(0), "sig:TRICE_S=%s\n", "Hello String" );     // not allowed
          //TRICE_N( Id(0), "sig:TRICE_N=%s\n", "Hello Buffer", 13 ); // not allowed

            TRICE( Id(0), "rd:TRICE line %E (%%E)\n", aFloat(-555555555.5555555555) );
            TRICE( Id(0), "rd:TRICE line %F (%%F)\n", aFloat(-555555555.5555555555) );
            TRICE( Id(0), "rd:TRICE line %G (%%G)\n", aFloat(-555555555.5555555555) );
        }
        break;
        case 50:
            TRICE32_1( Id(0), "rd:TRICE32_1 line %u (%%u)\n", -1 );
            TRICE32_1( Id(0), "rd:TRICE32_1 line %b (%%b)\n", -1 );
            TRICE32_1( Id(0), "rd:TRICE32_1 line %o (%%o)\n", -1 );
            TRICE32_1( Id(0), "rd:TRICE32_1 line %O (%%O)\n", -1 );
            TRICE32_1( Id(0), "rd:TRICE32_1 line %X (%%X)\n", -1 );
            TRICE32_1( Id(0), "rd:TRICE32_1 line %x (%%x)\n", -1 );
            TRICE32_1( Id(0), "rd:TRICE32_1 line %d (%%d)\n", -1 );
        break;
        case 60:
            TRICE64_1( Id(0), "rd:TRICE64_1 line %u (%%u)\n", -1 );
            TRICE64_1( Id(0), "rd:TRICE64_1 line %b (%%b)\n", -1 );
            TRICE64_1( Id(0), "rd:TRICE64_1 line %o (%%o)\n", -1 );
            TRICE64_1( Id(0), "rd:TRICE64_1 line %O (%%O)\n", -1 );
            TRICE64_1( Id(0), "rd:TRICE64_1 line %X (%%C)\n", -1 );
            TRICE64_1( Id(0), "rd:TRICE64_1 line %x (%%x)\n", -1 );
            TRICE64_1( Id(0), "rd:TRICE64_1 line %d (%%d)\n", -1 );
        break;
        case 70:
            TRICE64( Id(0), "rd:TRICE64 line %E (%%E)\n", aDouble(-555555555.5555555555) );
            TRICE64( Id(0), "rd:TRICE64 line %F (%%F)\n", aDouble(-555555555.5555555555) );
            TRICE64( Id(0), "rd:TRICE64 line %G (%%G)\n", aDouble(-555555555.5555555555) );
        break;
        case 80:
            TRICE( Id(0), "sig:Some time measurements\n" );
            TRICE( Id(0), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
            TRICE( Id(0), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
            TRICE( Id(0), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
            TRICE( Id(0), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
            TRICE64( Id(0), "rd:TRICE64 %d, %d\n", 1, 2 );
        break;
        case 90:
            TRICE( Id(0), "tim:TRICE START time message\n");
            TRICE( Id(0), "tim:TRICE STOP time message\n");
            TRICE( Id(0), "tim:TRICE START time message\n");
            TRICE( Id(0), "tim:TRICE STOP time message\n");
            TRICE( Id(0), "tim:TRICE START time message\n");
            TRICE64( Id(0), "rd:TRICE64 %d, %d\n", 1, 2 );
            TRICE( Id(0), "tim:TRICE STOP time message\n");
        break;
        case 100:
            TRICE( Id(0), "tim:TRICE --------------------------------------------------\n");
            TRICE( Id(0), "tim:TRICE --------------------------------------------------\n");
            TRICE( Id(0), "tim:TRICE --------------------------------------------------\n");
            TRICE( Id(0), "tim:TRICE --------------------------------------------------\n");
            TRICE( Id(0), "tim:TRICE --------------------------------------------------\n");
            TRICE( Id(0), "tim:TRICE --------------------------------------------------\n");
            TRICE64( Id(0), "rd:TRICE64 %d, %d\n", 1, 2 );
        break;
        case 110:
            TRICE( Id(0), "tim:TRICE --------------------------------------------------\n");
            TRICE( Id(0), "tim:TRICE --------------------------------------------------\n");
            TRICE( Id(0), "tim:TRICE --------------------------------------------------\n");
            TRICE( Id(0), "tim:TRICE --------------------------------------------------\n");
            TRICE( Id(0), "tim:TRICE --------------------------------------------------\n");
            TRICE( Id(0), "tim:TRICE --------------------------------------------------\n");
            TRICE( Id(0), "tim:TRICE --------------------------------------------------\n");
            TRICE64( Id(0), "rd:TRICE64 %d, %d\n", 1, 2 );
        break;
        case 120:
            TRICE( Id(0), "tim:TRICE --------------------------------------------------\n");
            TRICE( Id(0), "tim:TRICE --------------------------------------------------\n");
            TRICE( Id(0), "tim:TRICE --------------------------------------------------\n");
            TRICE( Id(0),  "rd:TRICE32_1 line %d\n", __LINE__ );
            TRICE64_1( Id(0),  "rd:TRICE64_1 %d\n", __LINE__ );
            TRICE32_2( Id(0),  "rd:TRICE32_2 line %d,%d\n", __LINE__, 2 );
            TRICE64_2( Id(0),  "rd:TRICE64_2 line %d,%d\n", __LINE__, 2 );
        break;
        case 130:
            TRICE( Id(0), "sig:TRICE with 1 to 12 values\n" );
            TRICE( Id(0),"rd:TRICE %d\n", -1 );
            TRICE( Id(0),"rd:TRICE %d, %d\n", -1, -2 );
            TRICE( Id(0),"rd:TRICE %d, %d, %d\n", -1, -2, -3 );
            TRICE( Id(0),"rd:TRICE %d, %d, %d, %d\n", -1, -2, -3, -4 );
            TRICE( Id(0),"rd:TRICE %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
            TRICE( Id(0),"rd:TRICE %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE( Id(0),"rd:TRICE %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
            TRICE( Id(0),"rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 140:
            TRICE( Id(0),"rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE( Id(0),"rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
            TRICE( Id(0),"rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE( Id(0),"rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 150:
            TRICE( Id(0), "sig:TRICE_n with 1 to 12 values\n" );
            TRICE_1( Id(0),   "rd:TRICE_1 %d\n", 1 );
            TRICE_2( Id(0),   "rd:TRICE_2 %d, %d\n", 1, 2 );
            TRICE_3( Id(0),   "rd:TRICE_3 %d, %d, %d\n", 1, 2, 3 );
            TRICE_4( Id(0),   "rd:TRICE_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
            TRICE_5( Id(0),   "rd:TRICE_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
            TRICE_6( Id(0),   "rd:TRICE_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
            TRICE_7( Id(0),   "rd:TRICE_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
            TRICE_8( Id(0),   "rd:TRICE_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
        break;
        case 160:
            TRICE_9( Id(0),   "rd:TRICE_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
            TRICE_10( Id(0),  "rd:TRICE_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
            TRICE_11( Id(0),  "rd:TRICE_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
            TRICE_12( Id(0),  "rd:TRICE_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );				
        break;
        case 170:
            TRICE( Id(0), "sig:TRICE8 with 1 to 12 values\n" );
            TRICE8( Id(0),"rd:TRICE8 %d\n", -1 );
            TRICE8( Id(0),"rd:TRICE8 %d, %d\n", -1, -2 );
            TRICE8( Id(0),"rd:TRICE8 %d, %d, %d\n", -1, -2, -3 );
            TRICE8( Id(0),"rd:TRICE8 %d, %d, %d, %d\n", -1, -2, -3, -4 );
            TRICE8( Id(0),"rd:TRICE8 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
            TRICE8( Id(0),"rd:TRICE8 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE8( Id(0),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
            TRICE8( Id(0),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 180:
            TRICE8( Id(0),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE8( Id(0),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
            TRICE8( Id(0),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE8( Id(0),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 190:
            TRICE( Id(0), "sig:TRICE8_n with 1 to 12 values\n" );
            TRICE8_1( Id(0),   "rd:TRICE8_1 %d\n", 1 );
            TRICE8_2( Id(0),   "rd:TRICE8_2 %d, %d\n", 1, 2 );
            TRICE8_3( Id(0),   "rd:TRICE8_3 %d, %d, %d\n", 1, 2, 3 );
            TRICE8_4( Id(0),   "rd:TRICE8_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
            TRICE8_5( Id(0),   "rd:TRICE8_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
            TRICE8_6( Id(0),   "rd:TRICE8_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
            TRICE8_7( Id(0),   "rd:TRICE8_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
        break;
        case 200:
            TRICE8_8( Id(0),   "rd:TRICE8_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
            TRICE8_9( Id(0),   "rd:TRICE8_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
            TRICE8_10( Id(0),  "rd:TRICE8_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
            TRICE8_11( Id(0),  "rd:TRICE8_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
            TRICE8_12( Id(0),  "rd:TRICE8_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );
        break;
        case 210:
            TRICE( Id(0), "sig:TRICE16 with 1 to 12 values\n" );
            TRICE16( Id(0),"rd:TRICE16 %d\n", -1 );
            TRICE16( Id(0),"rd:TRICE16 %d, %d\n", -1, -2 );
            TRICE16( Id(0),"rd:TRICE16 %d, %d, %d\n", -1, -2, -3 );
            TRICE16( Id(0),"rd:TRICE16 %d, %d, %d, %d\n", -1, -2, -3, -4 );
            TRICE16( Id(0),"rd:TRICE16 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
            TRICE16( Id(0),"rd:TRICE16 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE16( Id(0),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
            TRICE16( Id(0),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 220:
            TRICE16( Id(0),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE16( Id(0),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
            TRICE16( Id(0),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE16( Id(0),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 230:
            TRICE( Id(0), "sig:TRICE16_n with 1 to 12 values\n" );
            TRICE16_1( Id(0),  "rd:TRICE16_1 %d\n", 1 );
            TRICE16_2( Id(0),  "rd:TRICE16_2 %d, %d\n", 1, 2 );
            TRICE16_3( Id(0),  "rd:TRICE16_3 %d, %d, %d\n", 1, 2, 3 );
            TRICE16_4( Id(0),  "rd:TRICE16_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
            TRICE16_5( Id(0),  "rd:TRICE16_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
            TRICE16_6( Id(0),  "rd:TRICE16_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
            TRICE16_7( Id(0),  "rd:TRICE16_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
            TRICE16_8( Id(0),  "rd:TRICE16_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
        break;
        case 240:
            TRICE16_9( Id(0),  "rd:TRICE16_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
            TRICE16_10( Id(0), "rd:TRICE16_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
            TRICE16_11( Id(0), "rd:TRICE16_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
            TRICE16_12( Id(0), "rd:TRICE16_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );
        break;
        case 250:
            TRICE( Id(0), "sig:TRICE16 with 1 to 12 pointer\n" );
            TRICE16( Id(0),"rd:TRICE16 %p\n", -1 );
            TRICE16( Id(0),"rd:TRICE16 %p, %p\n", -1, -2 );
            TRICE16( Id(0),"rd:TRICE16 %p, %p, %p\n", -1, -2, -3 );
            TRICE16( Id(0),"rd:TRICE16 %p, %p, %p, %p\n", -1, -2, -3, -4 );
            TRICE16( Id(0),"rd:TRICE16 %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5 );
            TRICE16( Id(0),"rd:TRICE16 %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6 );
            TRICE16( Id(0),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7);
            TRICE16( Id(0),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 260:
            TRICE16( Id(0),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE16( Id(0),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
            TRICE16( Id(0),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE16( Id(0),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 270:
            TRICE( Id(0), "sig:TRICE16 with 1 to 12 hex\n" );
            TRICE16( Id(0),"rd:TRICE16 %X\n", -1 );
            TRICE16( Id(0),"rd:TRICE16 %X, %X\n", -1, -2 );
            TRICE16( Id(0),"rd:TRICE16 %X, %X, %X\n", -1, -2, -3 );
            TRICE16( Id(0),"rd:TRICE16 %X, %X, %X, %X\n", -1, -2, -3, -4 );
            TRICE16( Id(0),"rd:TRICE16 %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5 );
            TRICE16( Id(0),"rd:TRICE16 %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6 );
            TRICE16( Id(0),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7);
            TRICE16( Id(0),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 280:
            TRICE16( Id(0),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE16( Id(0),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
            TRICE16( Id(0),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE16( Id(0),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 290:
            TRICE( Id(0), "sig:TRICE32 with 1 to 12 values\n" );
            TRICE32( Id(0),"rd:TRICE32 %d\n", -1 );
            TRICE32( Id(0),"rd:TRICE32 %d, %d\n", -1, -2 );
            TRICE32( Id(0),"rd:TRICE32 %d, %d, %d\n", -1, -2, -3 );
            TRICE32( Id(0),"rd:TRICE32 %d, %d, %d, %d\n", -1, -2, -3, -4 );
            TRICE32( Id(0),"rd:TRICE32 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
            TRICE32( Id(0),"rd:TRICE32 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE32( Id(0),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
            TRICE32( Id(0),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 300:
            TRICE32( Id(0),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE32( Id(0),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
            TRICE32( Id(0),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE32( Id(0),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 310:
            TRICE( Id(0), "signal:TRICE32_n with 1 to 12 values\n" );
            TRICE32_1( Id(0),  "rd:TRICE32_1 %d\n", 1 );
            TRICE32_2( Id(0),  "rd:TRICE32_2 %d, %d\n", 1, 2 );
            TRICE32_3( Id(0),  "rd:TRICE32_3 %d, %d, %d\n", 1, 2, 3 );
            TRICE32_4( Id(0),  "rd:TRICE32_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
            TRICE32_5( Id(0),  "rd:TRICE32_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
            TRICE32_6( Id(0),  "rd:TRICE32_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
            TRICE32_7( Id(0),  "rd:TRICE32_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
            TRICE32_8( Id(0),  "rd:TRICE32_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
        break;
        case 320:
            TRICE32_9( Id(0),  "rd:TRICE32_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
            TRICE32_10( Id(0), "rd:TRICE32_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
            TRICE32_11( Id(0), "rd:TRICE32_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
            TRICE32_12( Id(0), "rd:TRICE32_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );
        break;
        case 330:
            TRICE( Id(0), "sig:TRICE64 with 1 to 12 values\n" );
            TRICE64( Id(0),"rd:TRICE64 %d\n", -1 );
            TRICE64( Id(0),"rd:TRICE64 %d, %d\n", -1, -2 );
            TRICE64( Id(0),"rd:TRICE64 %d, %d, %d\n", -1, -2, -3 );
            TRICE64( Id(0),"rd:TRICE64 %d, %d, %d, %d\n", -1, -2, -3, -4 );
            TRICE64( Id(0),"rd:TRICE64 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
            TRICE64( Id(0),"rd:TRICE64 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE64( Id(0),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
            TRICE64( Id(0),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 340:
            TRICE64( Id(0),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE64( Id(0),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
            TRICE64( Id(0),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE64( Id(0),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 350:
            TRICE( Id(0), "sig:TRICE64_n with 1 to 12 values\n" );
            TRICE64_1( Id(0),  "rd:TRICE64_1 %d\n", 1 );
            TRICE64_2( Id(0),  "rd:TRICE64_2 %d, %d\n", 1, 2 );
            TRICE64_3( Id(0),  "rd:TRICE64_3 %d, %d, %d\n", 1, 2, 3 );
            TRICE64_4( Id(0),  "rd:TRICE64_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
            TRICE64_5( Id(0),  "rd:TRICE64_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
            TRICE64_6( Id(0),  "rd:TRICE64_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
            TRICE64_7( Id(0),  "rd:TRICE64_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
            TRICE64_8( Id(0),  "rd:TRICE64_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
        break;
        case 360:
            TRICE64_9( Id(0),  "rd:TRICE64_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
            TRICE64_10( Id(0), "rd:TRICE64_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
            TRICE64_11( Id(0), "rd:TRICE64_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
            TRICE64_12( Id(0), "rd:TRICE64_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );
        break;
        case 370:
            TRICE( Id(0), "sig:Colors and ticks\n" );
            TRICE(Id(0), "--------------------------------------------------\n");
            TRICE(Id(0), "--------------------------------------------------\n");
            TRICE(Id(0), "dbg:12345 as 16bit is %016b\n", 12345);
            TRICE(Id(0), "--------------------------------------------------\n");
            TRICE(Id(0), "sig:This ASSERT error is just a demo and no real error:\n");
            TRICE(Id(0), "--------------------------------------------------\n");
        break;
        case 380:
            TRICE(Id(0), "ERR:error       message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id(0), "WRN:warning     message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id(0), "ATT:attention   message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id(0), "DIA:diagnostics message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id(0), "TIM:timing      message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id(0), "DBG:debug       message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id(0), "SIG:signal      message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id(0), "RD:read         message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id(0), "WR:write        message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id(0), "ISR:interrupt   message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id(0), "MSG:normal      message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id(0), "INFO:informal   message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id(0), "time:TRICE32_1   message, SysTick is %6u\n", SYSTICKVAL);
        break;
        case 390:
            TRICE( Id(0), "sig:Various ranges and formats\n" );
            TRICE8( Id(0), "tst:TRICE8  %%03x -> %03x  %03x  %03x  %03x\n", 1, 0x7f, 0x80, 0xff);
            TRICE8( Id(0), "tst:TRICE8   %%4d -> %4d %4d %4d %4d\n", 1, 0x7f, 0x80, 0xff);
            TRICE8( Id(0), "tst:TRICE8   %%4u -> %4u %4u %4u %4u\n", 1, 0x7f, 0x80, 0xff);
            TRICE8( Id(0), "tst:TRICE8   %%4o -> %4o %4o %4o %4o\n", 1, 0x7f, 0x80, 0xff);
            TRICE8( Id(0), "tst:TRICE8   %%4O -> %4O %4O %4O %4O\n", 1, 0x7f, 0x80, 0xff);
            TRICE8( Id(0), "tst:TRICE8   %%4X -> %4X %4X %4X %4X\n", 1, 0x7f, 0x80, 0xff);
            TRICE8( Id(0), "tst:TRICE8   %%4x -> %4x %4x %4x %4x\n", 1, 0x7f, 0x80, 0xff);
            TRICE8( Id(0), "tst:TRICE8   %%8b -> %8b %8b %8b %8b\n", 1, 0x7f, 0x80, 0xff);
            TRICE8( Id(0), "tst:TRICE8  %%08b -> %08b %08b %08b %08b\n", 1, 0x7f, 0x80, 0xff);
        break;
        case 400:
            TRICE16(Id(0), "tst:TRICE16  %%05x ->   %05x   %05x   %05x   %05x\n", 1, 0x7fff, 0x8000, 0xffff);
            TRICE16(Id(0), "tst:TRICE16   %%6d ->  %6d  %6d  %6d  %6d\n",         1, 0x7fff, 0x8000, 0xffff);
            TRICE16(Id(0), "tst:TRICE16   %%7o -> %7o %7o %7o %7o\n",             1, 0x7fff, 0x8000, 0xffff);

            TRICE32(Id(0), "tst:TRICE32 %%09x ->      %09x      %09x       %09x     %09x\n", 1, 0x7fffffff, 0x80000000, 0xffffffff);
            TRICE32(Id(0), "tst:TRICE32 %%10d ->     %10d     %10d     %10d    %10x\n",      1, 0x7fffffff, 0x80000000, 0xffffffff);
            TRICE32(Id(0), "att:TRICE32 %40b\n", 0xAAAAAAAA);
        break;
        case 410:
            TRICE64(Id(0), "tst:TRICE64 %%09x ->      %09x      %09x       %09x     %09x\n", 1, 0x7fffffffffffffff, 0x8000000000000000, 0xffffffffffffffff);
            TRICE64(Id(0), "tst:TRICE64 %%10d ->     %10d     %10d     %10d    %10x\n",      1, 0x7fffffffffffffff, 0x8000000000000000, 0xffffffffffffffff);
            TRICE64(Id(0), "att:TRICE64 %70b\n", 0xAAAAAAAAAAAAAAAA);
        break;
        case 420:
            TRICE( Id(0), "sig:Legacy TRICE8\n" );
            TRICE8_1(Id(0), "tst:TRICE8_1 %02x\n", 0xA1);
            TRICE8_2(Id(0), "tst:TRICE8_2 %02x %02x\n", 0xA1, 0xA2);
            TRICE8_3(Id(0), "tst:TRICE8_3 %02x %02x %02x\n", 0xA1, 0xA2, 0xA3);
            TRICE8_4(Id(0), "tst:TRICE8_4 %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4);
            TRICE8_5(Id(0), "tst:TRICE8_5 %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5 );
            TRICE8_6(Id(0), "tst:TRICE8_6 %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6);
            TRICE8_7(Id(0), "tst:TRICE8_7 %02x %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7);
            TRICE8_8(Id(0), "tst:TRICE8_8 %02x %02x %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8);
        break;
        case 430:
            TRICE8_1(Id(0), "tst:TRICE8_1 %d\n",                      -1);
            TRICE8_2(Id(0), "tst:TRICE8_2 %d %d\n",                   -1, -2);
            TRICE8_3(Id(0), "tst:TRICE8_3 %d %d %d\n",                -1, -2, -3);
            TRICE8_4(Id(0), "tst:TRICE8_4 %d %d %d %d\n",             -1, -2, -3, -4);
            TRICE8_5(Id(0), "tst:TRICE8_5 %d %d %d %d %d\n",          -1, -2, -3, -4, -5);
            TRICE8_6(Id(0), "tst:TRICE8_6 %d %d %d %d %d %d\n",       -1, -2, -3, -4, -5, -6);
            TRICE8_7(Id(0), "tst:TRICE8_7 %d %d %d %d %d %d %d\n",    -1, -2, -3, -4, -5, -6, -7);
            TRICE8_8(Id(0), "tst:TRICE8_8 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8);
        break;
        case 440:
            TRICE8_1(Id(0), "tst:TRICE8_1 %d\n", 1);
            TRICE8_2(Id(0), "tst:TRICE8_2 %d %d\n", 1, 2);
            TRICE8_3(Id(0), "tst:TRICE8_3 %d %d %d\n", 1, 2, 3);
            TRICE8_4(Id(0), "tst:TRICE8_4 %d %d %d %d\n", 1, 2, 3, 4);
            TRICE8_5(Id(0), "tst:TRICE8_5 %d %d %d %d %d\n", 1, 2, 3, 4, 5);
            TRICE8_6(Id(0), "tst:TRICE8_6 %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6);
            TRICE8_7(Id(0), "tst:TRICE8_7 %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7);
            TRICE8_8(Id(0), "tst:TRICE8_8 %d %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7, 8);
        break;
        case 450:
            TRICE8_1(Id(0), "tst:TRICE8_1 %u\n", 201);
            TRICE8_2(Id(0), "tst:TRICE8_2 %u %u\n", 201, 202);
            TRICE8_3(Id(0), "tst:TRICE8_3 %u %u %u\n", 201, 202, 203);
            TRICE8_4(Id(0), "tst:TRICE8_4 %u %u %u %u\n", 201, 202, 203, 204);
            TRICE8_5(Id(0), "tst:TRICE8_5 %u %u %u %u %u\n", 201, 202, 203, 204, 205);
            TRICE8_6(Id(0), "tst:TRICE8_6 %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206);
            TRICE8_7(Id(0), "tst:TRICE8_7 %u %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206, 207);
            TRICE8_8(Id(0), "tst:TRICE8_8 %u %u %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206, 207, 208);
            TRICE8_8(Id(0), "tst:TRICE8_1 %%d=%d, %%u=%u, 0x%%x=0x%x, 0x%%2x=0x%2x, 0x%%02x=0x%02x, 0x%%3x=0x%3x, 0x%%03x=0x%03x, %%b=%b\n", 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81);
        break;
        case 460:
            TRICE( Id(0), "sig:Legacy TRICE16-64\n" );
            TRICE16_1(Id(0), "tst:TRICE16_1 %d\n", -111);
            TRICE16_2(Id(0), "tst:TRICE16_2 %d %d\n", -111, -222);
            TRICE16_3(Id(0), "tst:TRICE16_3 %d %d %d\n", -111, -222, -333);
            TRICE16_4(Id(0), "tst:TRICE16_4 %d %d %d %d\n", -111, -222, -333, -444);
            TRICE32_1(Id(0), "tst:TRICE32_1 %08x\n", 0x0123cafe);
            TRICE32_1(Id(0), "tst:TRICE32_1 %d\n", -111);
            TRICE32_2(Id(0), "tst:TRICE32_2 %x %x\n", -111, -222);
            TRICE32_2(Id(0), "tst:TRICE32_2 %d %d\n", -111, -222);
        break;
        case 470:
            TRICE32_3(Id(0), "tst:TRICE32_3 %x %x %x\n", -111, -222, -333);
            TRICE32_3(Id(0), "tst:TRICE32_3 %d %d %d\n", -111, -222, -333);
            TRICE32_4(Id(0), "tst:TRICE32_4 %x %x %x %x\n", -111, -222, -333, -444);
            TRICE32_4(Id(0), "tst:TRICE32_4 %d %d %d %d\n", -111, -222, -333, -444);
        break;
        case 480:
            TRICE64_1(Id(0), "tst:TRICE64_1 %d\n", -111);
            TRICE64_2(Id(0), "tst:TRICE64_2 %d %d\n", -111, -222);
            TRICE16_1(Id(0), "tst:TRICE16_1 %u\n", 60001);
            TRICE16_2(Id(0), "tst:TRICE16_2 %u %u\n", 60001, 60002);
            TRICE16_3(Id(0), "tst:TRICE16_3 %u %u %u\n", 60001, 60002, 60003);
            TRICE16_4(Id(0), "tst:TRICE16_4 %u %u %u %u\n", 60001, 60002, 60003, 60004);
            TRICE32_1(Id(0), "tst:TRICE32_1 %u\n", 4000000001);
            TRICE32_2(Id(0), "tst:TRICE32_2 %u %u\n", 4000000001, 4000000002);
            TRICE32_3(Id(0), "tst:TRICE32_3 %u %u %u\n", 4000000001, 4000000002, 4000000003);
            TRICE32_4(Id(0), "tst:TRICE32_4 %u %u %u %u\n", 4000000001, 4000000002, 4000000003, 4000000004);
        break;
        case 490:
            TRICE64_1(Id(0), "tst:TRICE64_1 %x\n", -1);
            TRICE64_2(Id(0), "tst:TRICE64_2 %x %x\n", -1, -2);
            TRICE64_1(Id(0), "tst:TRICE64_1 %u\n", -1);
            TRICE64_2(Id(0), "tst:TRICE64_2 %u %u\n", -1, -2);
        break;
        case 500:
            TRICE( Id(0), "sig:colored single letters and several TRICE macros in one line\n" );
            TRICE(Id(0), "e:A");
            TRICE(Id(0), "w:B");
            TRICE(Id(0), "a:c");
            TRICE(Id(0), "wr:d");
            TRICE(Id(0), "rd:e\n");
            TRICE(Id(0), "diag:f");
            TRICE(Id(0), "d:G");
            TRICE(Id(0), "t:H");
            TRICE(Id(0), "time:i");
            TRICE(Id(0), "message:J");
            TRICE(Id(0), "dbg:k\n");
        break;
        case 510:
            TRICE(Id(0), "1");
            TRICE(Id(0), "2");
            TRICE(Id(0), "3");
            TRICE(Id(0), "4");
            TRICE(Id(0), "e:7");
            TRICE(Id(0), "m:12");
            TRICE(Id(0), "m:123\n");
            TRICE(Id(0), "e:A");TRICE(Id(0), "w:B");TRICE(Id(0), "a:c");
            TRICE(Id(0), "wr:d");TRICE(Id(0), "rd:e\n");//TRICE(Id(0), "diag:f");
        break;
        case 520:
            TRICE( Id(0), "sig:Runtime generated strings\n" );
            s = "AAAAAAAAAAAA";
            TRICE32( Id(0), "dbg:len=%u:", strlen(s) );
            TRICE_S( Id(0), "sig:%s\n", s );
            s = "\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBCCCCCCCC\
";
            TRICE32( Id(0), "dbg:len=%u:", strlen(s) );
            TRICE_S( Id(0), "sig:%s\n", s );
        break;
        case 530:
        TRICE( Id(0), "sig:Runtime generated strings\n" );
            s = "\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\
DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\
EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\
";
            TRICE32( Id(0), "dbg:len=%u: ", strlen(s) );
            TRICE_S( Id(0), "sig:%s\n", s );
        break;
        case 540:
            TRICE( Id(0), "sig:Runtime generated strings\n" );
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
            TRICE32( Id(0), "dbg:len=%u: ", strlen(s) );
            TRICE_S( Id(0), "sig:%s\n", s );
        break;
        case 550:
            TRICE( Id(0), "sig:Runtime generated strings\n" );
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
          TRICE32( Id(0), "dbg:len=%u: ", strlen(s) );
          TRICE_S( Id(0), "sig:%s\n", s );
        break;
        case 560:{ // https://yourbasic.org/golang/fmt-printf-reference-cheat-sheet/
            int i = 15;
            TRICE( Id(0), "sig:Integer (indent, base, sign)\n" );
            TRICE( Id(0), "rd: 15 	%d 	Base 10\n", i );
            TRICE( Id(0), "rd: +15 	%+d 	Always show sign\n", i );
            TRICE( Id(0), "rd:   15 	%4d 	Pad with spaces (width 4, right justified)\n", i );
            TRICE( Id(0), "rd: 15   	%-4d 	Pad with spaces (width 4, left justified)\n", i );
            TRICE( Id(0), "rd: 0015 	%04d 	Pad with zeroes (width 4)\n", i );
            TRICE( Id(0), "rd: 1111 	%b 	Base 2\n", i );
            TRICE( Id(0), "rd: 17 	%o 	Base 8\n", i );
            TRICE( Id(0), "rd: f 	%x 	Base 16, lowercase\n", i );
            TRICE( Id(0), "rd: F 	%X 	Base 16, uppercase\n", i );
            TRICE( Id(0), "rd: 0xf 	%#x 	Base 16, with leading 0x\n", i );
        }
        break;
        case 570:{
            char a = 'A';
            TRICE( Id(0), "sig:Character (quoted, Unicode)\n" );
            TRICE( Id(0), "rd: A 	%c 	Character\n", a );
            TRICE( Id(0), "rd: 'A' 	%q 	Quoted character\n", a );
          //TRICE( Id(0), "rd: U+0041 	%U 	Unicode\n", a );// todo
          //TRICE( Id(0), "rd: U+0041 'A' 	%#U 	Unicode with character\n", a );// todo
            TRICE( Id(0), "sig:Boolean (true/false)\n" );
            TRICE( Id(0), "rd:Use %%t to format a boolean as true (%t) or false (%t).\n", 1, 0 );
            TRICE( Id(0), "sig:Pointer (hex)\n" );
        }
        break;
        case 580:
            TRICE8( Id(0), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x84 );
            TRICE16( Id(0), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x1234 );
            TRICE32( Id(0), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0xaabbccdd );
            TRICE64( Id(0), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x1122334455667788 );
            TRICE( Id(0), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0xaabbccdd);
        break;
        case 590:{
            float f = 123.456;
            TRICE( Id(0), "sig:Float (indent, precision, scientific notation)\n" );
            TRICE( Id(0), "rd: 1.234560e+02		%e 	%%e Scientific notation\n", aFloat(f) );
            TRICE( Id(0), "rd: 123.456000		%f 	%%f Decimal point, no exponent\n", aFloat(f) );
            TRICE( Id(0), "rd: 123.46	 		%.2f		%%.2f Default width, precision 2\n", aFloat(f) );
            TRICE( Id(0), "rd: ␣␣123.46		%8.2f	%%8.2f Width 8, precision 2\n", aFloat(f) );
            TRICE( Id(0), "rd: 123.456		%g		%%g Exponent as needed, necessary digits only\n", aFloat(f) );
        }
        break;
        case 600:{
            double d = 123.456;
            TRICE( Id(0), "sig:Double (indent, precision, scientific notation)\n" );
            TRICE64( Id(0), "rd: 1.234560e+02		%e 	%%e Scientific notation\n", aDouble(d) );
            TRICE64( Id(0), "rd: 123.456000		%f 	%%f Decimal point, no exponent\n", aDouble(d) );
            TRICE64( Id(0), "rd: 123.46			%.2f    	%%.2f Default width, precision 2\n", aDouble(d) );
            TRICE64( Id(0), "rd: ␣␣123.46		%8.2f 	%%8.2f Width 8, precision 2\n", aDouble(d) );
            TRICE64( Id(0), "rd: 123.456		%g		%%g Exponent as needed, necessary digits only\n", aDouble(d) );
        }
        break;
        case 610:{
            char* s = "café";
            TRICE( Id(0), "sig:String or byte slice (quote, indent, hex)\n" );
            TRICE_S( Id(0), "rd: café 			%s		Plain string\n", s );
            TRICE_S( Id(0), "rd: ␣␣café 		%6s 		Width 6, right justify\n", s );
            TRICE_S( Id(0), "rd: café␣␣ 		%-6s 		Width 6, left justify\n", s );
            TRICE_S( Id(0), "rd: \"café\" 		%q	 	Quoted string\n", s );
            TRICE_S( Id(0), "rd: 636166c3a9 		%x	Hex dump of byte values\n", s );
            TRICE_S( Id(0), "rd: 63 61 66 c3 a9 	% x	Hex dump with spaces\n", s );
        }
        break;
        case 620:
            TRICE( Id(0), "sig:TRICE32 with variable param count 1 to 12\n" );
            TRICE8(Id(0), "tst:TRICE8 %d\n", -1 );
            TRICE8(Id(0), "tst:TRICE8 %d %d\n", -1, -2 );
            TRICE8(Id(0), "tst:TRICE8 %d %d %d\n", -1, -2, -3 );
            TRICE8(Id(0), "tst:TRICE8 %d %d %d %d\n", -1, -2, -3, -4 );
            TRICE8(Id(0), "tst:TRICE8 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
            TRICE8(Id(0), "tst:TRICE8 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE8(Id(0), "tst:TRICE8 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
            TRICE8(Id(0), "tst:TRICE8 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 630:
            TRICE8(Id(0), "tst:TRICE8 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE8(Id(0), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
            TRICE8(Id(0), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE8(Id(0), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 640:
            TRICE8(Id(0), "tst:TRICE8 %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );
            TRICE8_1 (Id(0), "tst:TRICE8_1  %d\n", -1 );
            TRICE8_2 (Id(0), "tst:TRICE8_2  %d %d\n", -1, -2 );
            TRICE8_3 (Id(0), "tst:TRICE8_3  %d %d %d\n", -1, -2, -3 );
            TRICE8_4 (Id(0), "tst:TRICE8_4  %d %d %d %d\n", -1, -2, -3, -4 );
            TRICE8_5 (Id(0), "tst:TRICE8_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
            TRICE8_6 (Id(0), "tst:TRICE8_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE8_7 (Id(0), "tst:TRICE8_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
            TRICE8_8 (Id(0), "tst:TRICE8_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 650:
            TRICE8_9 (Id(0), "tst:TRICE8_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE8_10(Id(0), "tst:TRICE8_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
            TRICE8_11(Id(0), "tst:TRICE8_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE8_12(Id(0), "tst:TRICE8_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
            TRICE8  (Id(0), "tst:TRICE8   %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );
            TRICE8_9(Id(0), "tst:TRICE8_9 %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );
        break;
        case 660:
            TRICE( Id(0), "sig:TRICE16 with variable param count 1 to 12\n" );
            TRICE16(Id(0), "tst:TRICE16 %d\n", -1 );
            TRICE16(Id(0), "tst:TRICE16 %d %d\n", -1, -2 );
            TRICE16(Id(0), "tst:TRICE16 %d %d %d\n", -1, -2, -3 );
            TRICE16(Id(0), "tst:TRICE16 %d %d %d %d\n", -1, -2, -3, -4 );
            TRICE16(Id(0), "tst:TRICE16 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
            TRICE16(Id(0), "tst:TRICE16 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE16(Id(0), "tst:TRICE16 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
            TRICE16(Id(0), "tst:TRICE16 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 670:
            TRICE16(Id(0), "tst:TRICE16 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE16(Id(0), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
            TRICE16(Id(0), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE16(Id(0), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 680:
            TRICE16(Id(0), "tst:TRICE16 %t %b %x %X %d %u %o %O %p\n", 3, 3, 3, 3, 3, 3, 3, 3, 3 );
            TRICE16_1 (Id(0), "tst:TRICE16_1  %d\n", -1 );
            TRICE16_2 (Id(0), "tst:TRICE16_2  %d %d\n", -1, -2 );
            TRICE16_3 (Id(0), "tst:TRICE16_3  %d %d %d\n", -1, -2, -3 );
            TRICE16_4 (Id(0), "tst:TRICE16_4  %d %d %d %d\n", -1, -2, -3, -4 );
            TRICE16_5 (Id(0), "tst:TRICE16_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
            TRICE16_6 (Id(0), "tst:TRICE16_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE16_7 (Id(0), "tst:TRICE16_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
            TRICE16_8 (Id(0), "tst:TRICE16_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 690:
            TRICE16_9 (Id(0), "tst:TRICE16_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE16_10(Id(0), "tst:TRICE16_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
            TRICE16_11(Id(0), "tst:TRICE16_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE16_12(Id(0), "tst:TRICE16_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
            TRICE16_9(Id(0), "tst:TRICE16 %t %b %x %X %d %u %o %O %p\n", 3, 3, 3, 3, 3, 3, 3, 3, 3 );
        break;
        case 700:
            TRICE( Id(0), "sig:TRICE32 with variable param count 1 to 12\n" );
            TRICE32(Id(0), "tst:TRICE32 %d\n", -1 );
            TRICE32(Id(0), "tst:TRICE32 %d %d\n", -1, -2 );
            TRICE32(Id(0), "tst:TRICE32 %d %d %d\n", -1, -2, -3 );
            TRICE32(Id(0), "tst:TRICE32 %d %d %d %d\n", -1, -2, -3, -4 );
            TRICE32(Id(0), "tst:TRICE32 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
            TRICE32(Id(0), "tst:TRICE32 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE32(Id(0), "tst:TRICE32 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
            TRICE32(Id(0), "tst:TRICE32 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 710:
            TRICE32(Id(0), "tst:TRICE32 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE32(Id(0), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
            TRICE32(Id(0), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE32(Id(0), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 720:
            TRICE32(Id(0), "tst:TRICE32 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));
            TRICE32_1 (Id(0), "tst:TRICE32_1  %d\n", -1 );
            TRICE32_2 (Id(0), "tst:TRICE32_2  %d %d\n", -1, -2 );
            TRICE32_3 (Id(0), "tst:TRICE32_3  %d %d %d\n", -1, -2, -3 );
            TRICE32_4 (Id(0), "tst:TRICE32_4  %d %d %d %d\n", -1, -2, -3, -4 );
            TRICE32_5 (Id(0), "tst:TRICE32_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
            TRICE32_6 (Id(0), "tst:TRICE32_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE32_7 (Id(0), "tst:TRICE32_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
            TRICE32_8 (Id(0), "tst:TRICE32_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 730:
            TRICE32_9 (Id(0), "tst:TRICE32_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE32_10(Id(0), "tst:TRICE32_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
            TRICE32_11(Id(0), "tst:TRICE32_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE32_12(Id(0), "tst:TRICE32_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
            TRICE32   (Id(0), "tst:TRICE32    %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));
            TRICE32_12(Id(0), "tst:TRICE32_12 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));
        break;
        case 740:
            TRICE( Id(0), "sig:TRICE64 with variable param count 1 to 12\n" );
            TRICE64(Id(0), "tst:TRICE64 %d\n", -1 );
            TRICE64(Id(0), "tst:TRICE64 %d %d\n", -1, -2 );
            TRICE64(Id(0), "tst:TRICE64 %d %d %d\n", -1, -2, -3 );
            TRICE64(Id(0), "tst:TRICE64 %d %d %d %d\n", -1, -2, -3, -4 );
            TRICE64(Id(0), "tst:TRICE64 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
            TRICE64(Id(0), "tst:TRICE64 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE64(Id(0), "tst:TRICE64 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
            TRICE64(Id(0), "tst:TRICE64 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 750:
            TRICE64(Id(0), "tst:TRICE64 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE64(Id(0), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
            TRICE64(Id(0), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE64(Id(0), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 760:
            TRICE64(Id(0), "tst:TRICE64 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aDouble(3.14159), aDouble(3.14159), aDouble(3.14159));
            TRICE64_1 (Id(0), "tst:TRICE64_1  %d\n", -1 );
            TRICE64_2 (Id(0), "tst:TRICE64_2  %d %d\n", -1, -2 );
            TRICE64_3 (Id(0), "tst:TRICE64_3  %d %d %d\n", -1, -2, -3 );
            TRICE64_4 (Id(0), "tst:TRICE64_4  %d %d %d %d\n", -1, -2, -3, -4 );
            TRICE64_5 (Id(0), "tst:TRICE64_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
            TRICE64_6 (Id(0), "tst:TRICE64_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE64_7 (Id(0), "tst:TRICE64_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
            TRICE64_8 (Id(0), "tst:TRICE64_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 770:
            TRICE64_9 (Id(0), "tst:TRICE64_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE64_10(Id(0), "tst:TRICE64_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
            TRICE64_11(Id(0), "tst:TRICE64_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE64_12(Id(0), "tst:TRICE64_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
            TRICE64_12(Id(0), "tst:TRICE64_12 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aDouble(3.14159), aDouble(3.14159), aDouble(3.14159));
        break;
        case 780:
            TRICE( Id(0), "att:positive and negative float in format variants\n");
            TRICE( Id(0), "rd:TRICE float %f (%%f)\n", aFloat(x) );
            TRICE( Id(0), "rd:TRICE float %9f (%%9f)\n", aFloat(x) );
            TRICE( Id(0), "rd:TRICE float %.9f (%%.9f)\n", aFloat(x) );
            TRICE( Id(0), "rd:TRICE float %9.f (%%9.f)\n", aFloat(x) );
            TRICE( Id(0), "rd:TRICE float %9.6f (%%9.6f)\n", aFloat(x) );
            TRICE( Id(0), "rd:TRICE float %f (%%f)\n", aFloat(-x) );
        break;
        case 790:
            TRICE( Id(0), "rd:TRICE float %9f (%%9f)\n", aFloat(-x) );
            TRICE( Id(0), "rd:TRICE float %.9f (%%.9f)\n", aFloat(-x) );
            TRICE( Id(0), "rd:TRICE float %9.f (%%9.f)\n", aFloat(-x) );
            TRICE( Id(0), "rd:TRICE float %9.6f (%%9.6f)\n", aFloat(-x) );
            TRICE( Id(0), "rd:TRICE float %+f (%%f)\n", aFloat(x) );
            TRICE( Id(0), "rd:TRICE float %+9f (%%9f)\n", aFloat(x) );
            TRICE( Id(0), "rd:TRICE float %+.9f (%%.9f)\n", aFloat(x) );
            TRICE( Id(0), "rd:TRICE float %+9.f (%%9.f)\n", aFloat(x) );
        break;
        case 800:
            TRICE( Id(0), "rd:TRICE float %+9.6f (%%9.6f)\n", aFloat(x) );
            TRICE( Id(0), "rd:TRICE float %+f (%%f)\n", aFloat(-x) );
            TRICE( Id(0), "rd:TRICE float %+9f (%%9f)\n", aFloat(-x) );
            TRICE( Id(0), "rd:TRICE float %+.9f (%%.9f)\n", aFloat(-x) );
            TRICE( Id(0), "rd:TRICE float %+9.f (%%9.f)\n", aFloat(-x) );
            TRICE( Id(0), "rd:TRICE float %+9.6f (%%9.6f)\n", aFloat(-x) );
            TRICE( Id(0), "rd:TRICE float %-f (%%f)\n", aFloat(x) );
            TRICE( Id(0), "rd:TRICE float %-9f (%%9f)\n", aFloat(x) );
        break;
        case 810:
            TRICE( Id(0), "rd:TRICE float %-.9f (%%.9f)\n", aFloat(x) );
            TRICE( Id(0), "rd:TRICE float %-9.f (%%9.f)\n", aFloat(x) );
            TRICE( Id(0), "rd:TRICE float %-9.6f (%%9.6f)\n", aFloat(x) );
            TRICE( Id(0), "rd:TRICE float %-f (%%f)\n", aFloat(-x) );
            TRICE( Id(0), "rd:TRICE float %-9f (%%9f)\n", aFloat(-x) );
            TRICE( Id(0), "rd:TRICE float %-.9f (%%.9f)\n", aFloat(-x) );
            TRICE( Id(0), "rd:TRICE float %-9.f (%%9.f)\n", aFloat(-x) );
            TRICE( Id(0), "rd:TRICE float %-9.6f (%%9.6f)\n", aFloat(-x) );
        break;
        case 820:
            TRICE( Id(0), "att:positive float & double in variants\n");
            TRICE32_1( Id(0), "rd:TRICE32_1 float %e (%%e)\n", aFloat(x) );
            TRICE32_1( Id(0), "rd:TRICE32_1 float %f (%%f)\n", aFloat(x) );
            TRICE32_1( Id(0), "rd:TRICE32_1 float %g (%%g)\n", aFloat(x) );
            TRICE32_1( Id(0), "rd:TRICE32_1 float %E (%%E)\n", aFloat(x) );
            TRICE32_1( Id(0), "rd:TRICE32_1 float %F (%%F)\n", aFloat(x) );
            TRICE32_1( Id(0), "rd:TRICE32_1 float %G (%%G)\n", aFloat(x) );
        break;
        case 830:
            TRICE32( Id(0), "rd:TRICE32 float %e (%%e)\n", aFloat(x) );
            TRICE32( Id(0), "rd:TRICE32 float %f (%%f)\n", aFloat(x) );
            TRICE32( Id(0), "rd:TRICE32 float %g (%%g)\n", aFloat(x) );
            TRICE32( Id(0), "rd:TRICE32 float %E (%%E)\n", aFloat(x) );
            TRICE32( Id(0), "rd:TRICE32 float %F (%%F)\n", aFloat(x) );
            TRICE32( Id(0), "rd:TRICE32 float %G (%%G)\n", aFloat(x) );
        break;
        case 840:
            TRICE( Id(0), "rd:TRICE float %e (%%e)\n", aFloat(x) );
            TRICE( Id(0), "rd:TRICE float %f (%%f)\n", aFloat(x) );
            TRICE( Id(0), "rd:TRICE float %g (%%g)\n", aFloat(x) );
            TRICE( Id(0), "rd:TRICE float %E (%%E)\n", aFloat(x) );
            TRICE( Id(0), "rd:TRICE float %F (%%F)\n", aFloat(x) );
            TRICE( Id(0), "rd:TRICE float %G (%%G)\n", aFloat(x) );
        break;
        case 850:
            TRICE64( Id(0), "rd:TRICE64 double %e (%%e), aDouble(y)\n", aDouble(y) );
            TRICE64( Id(0), "rd:TRICE64 double %f (%%f), aDouble(y)\n", aDouble(y) );
            TRICE64( Id(0), "rd:TRICE64 double %g (%%g), aDouble(y)\n", aDouble(y) );
            TRICE64( Id(0), "rd:TRICE64 double %E (%%E), aDouble(y)\n", aDouble(y) );
            TRICE64( Id(0), "rd:TRICE64 double %F (%%F), aDouble(y)\n", aDouble(y) );
            TRICE64( Id(0), "rd:TRICE64 double %G (%%G), aDouble(y)\n", aDouble(y) );
        break;
        case 860:
            TRICE64_1( Id(0), "rd:TRICE64_1 double %e (%%e), aDouble(y)\n", aDouble(y) );
            TRICE64_1( Id(0), "rd:TRICE64_1 double %f (%%f), aDouble(y)\n", aDouble(y) );
            TRICE64_1( Id(0), "rd:TRICE64_1 double %g (%%g), aDouble(y)\n", aDouble(y) );
            TRICE64_1( Id(0), "rd:TRICE64_1 double %E (%%E), aDouble(y)\n", aDouble(y) );
            TRICE64_1( Id(0), "rd:TRICE64_1 double %F (%%F), aDouble(y)\n", aDouble(y) );
            TRICE64_1( Id(0), "rd:TRICE64_1 double %G (%%G), aDouble(y)\n", aDouble(y) );
        break;
        case 870:
            TRICE( Id(0), "att:negative float & double\n" );
            TRICE( Id(0), "rd:TRICE float %e (%%e)\n", aFloat(-x) );
            TRICE( Id(0), "rd:TRICE float %f (%%f)\n", aFloat(-x) );
            TRICE( Id(0), "rd:TRICE float %g (%%g)\n", aFloat(-x) );
            TRICE( Id(0), "rd:TRICE float %E (%%E)\n", aFloat(-x) );
            TRICE( Id(0), "rd:TRICE float %F (%%F)\n", aFloat(-x) );
            TRICE( Id(0), "rd:TRICE float %G (%%G)\n", aFloat(-x) );
        break;
        case 880:
            TRICE64( Id(0), "rd:TRICE64 double %e (%%e), aDouble(y)\n", aDouble(-y) );
            TRICE64( Id(0), "rd:TRICE64 double %f (%%f), aDouble(y)\n", aDouble(-y) );
            TRICE64( Id(0), "rd:TRICE64 double %g (%%g), aDouble(y)\n", aDouble(-y) );
            TRICE64( Id(0), "rd:TRICE64 double %E (%%E), aDouble(y)\n", aDouble(-y) );
            TRICE64( Id(0), "rd:TRICE64 double %F (%%F), aDouble(y)\n", aDouble(-y) );
            TRICE64( Id(0), "rd:TRICE64 double %G (%%G), aDouble(y)\n", aDouble(-y) );
        break;
        case 890:
            TRICE( Id(0), "att:formatted float & double\n" );
            TRICE32( Id(0), "rd:TRICE32 float %12.6e (%%12.6e)\n", aFloat(x) );
            TRICE32( Id(0), "rd:TRICE32 float %12.6f (%%12.6f)\n", aFloat(x) );
            TRICE32( Id(0), "rd:TRICE32 float %12.6g (%%12.6g)\n", aFloat(x) );
            TRICE32( Id(0), "rd:TRICE32 float %12.6E (%%12.6E)\n", aFloat(x) );
            TRICE32( Id(0), "rd:TRICE32 float %12.6F (%%12.6F)\n", aFloat(x) );
            TRICE32( Id(0), "rd:TRICE32 float %12.6G (%%12.6G)\n", aFloat(x) );
        break;
        case 900:
            TRICE64( Id(0), "rd:TRICE64 double %12.6e (%%12.6e), aDouble(y)\n", aDouble(y) );
            TRICE64( Id(0), "rd:TRICE64 double %12.6f (%%12.6f), aDouble(y)\n", aDouble(y) );
            TRICE64( Id(0), "rd:TRICE64 double %12.6g (%%12.6g), aDouble(y)\n", aDouble(y) );
            TRICE64( Id(0), "rd:TRICE64 double %12.6E (%%12.6E), aDouble(y)\n", aDouble(y) );
            TRICE64( Id(0), "rd:TRICE64 double %12.6F (%%12.6F), aDouble(y)\n", aDouble(y) );
            TRICE64( Id(0), "rd:TRICE64 double %12.6G (%%12.6G), aDouble(y)\n", aDouble(y) );
        break;
        case 910:
            TRICE( Id(0), "att:mixed int & float & double & bits\n" );
            TRICE32( Id(0), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n", FloatToInt32(x), aFloat(x), aFloat(x), aFloat(x) );
            TRICE32( Id(0), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",    (int32_t)(x), aFloat(x), aFloat(x), aFloat(x) );
            TRICE32( Id(0), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",              x,  aFloat(x), aFloat(x), aFloat(x) );
            TRICE32( Id(0), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n", FloatToInt32(-x), aFloat(-x), aFloat(-x), aFloat(-x) );
            TRICE32( Id(0), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",    (int32_t)(-x), aFloat(-x), aFloat(-x), aFloat(-x) );
            TRICE32( Id(0), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",              -x,  aFloat(-x), aFloat(-x), aFloat(-x) );
        break;
        case 920:
            TRICE64( Id(0), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n", DoubleToInt64(y), aDouble(y), aDouble(y), aDouble(y) );
            TRICE64( Id(0), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",     (int64_t)(y), aDouble(y), aDouble(y), aDouble(y) );
            TRICE64( Id(0), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",               y,  aDouble(y), aDouble(y), aDouble(y) );
            TRICE64( Id(0), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n", DoubleToInt64(-y), aDouble(-y), aDouble(-y), aDouble(-y) );
            TRICE64( Id(0), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",     (int64_t)(-y), aDouble(-y), aDouble(-y), aDouble(-y) );
            TRICE64( Id(0), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",               -y,  aDouble(-y), aDouble(-y), aDouble(-y) );
        break;
        case 930:
            TRICE( Id(0), "att:Various single arguments\n" );
            TRICE8( Id(0), "rd:TRICE8 line %t (%%t ,0)\n", 0 );
            TRICE8( Id(0), "rd:TRICE8 line %t (%%t ,2)\n", 2 );
            TRICE8( Id(0), "rd:TRICE8 line %u (%%u ,-1)\n", -1 );
            TRICE8( Id(0), "rd:TRICE8 line %b (%%b ,-1)\n", -1 );
            TRICE8( Id(0), "rd:TRICE8 line %o (%%o ,-1)\n", -1 );
            TRICE8( Id(0), "rd:TRICE8 line %O (%%O ,-1)\n", -1 );
            TRICE8( Id(0), "rd:TRICE8 line %X (%%X ,-1)\n", -1 );
            TRICE8( Id(0), "rd:TRICE8 line %x (%%x ,-1)\n", -1 );
            TRICE8( Id(0), "rd:TRICE8 line %d (%%d ,-1)\n", -1 );
        break;
        case 940:
            TRICE16_1( Id(0), "rd:TRICE16_1 line %t (%%t ,0)\n", 0 );
            TRICE16_1( Id(0), "rd:TRICE16_1 line %t (%%t ,2)\n", 2 );
            TRICE16_1( Id(0), "rd:TRICE16_1 line %u (%%u -1)\n", -1 );
            TRICE16_1( Id(0), "rd:TRICE16_1 line %b (%%b -1)\n", -1 );
            TRICE16_1( Id(0), "rd:TRICE16_1 line %o (%%o -1)\n", -1 );
            TRICE16_1( Id(0), "rd:TRICE16_1 line %x (%%x -1)\n", -1 );
        break;
        case 950:
            TRICE16_1( Id(0), "rd:TRICE16_1 line %d (%%d -1)\n", -1 );
            TRICE( Id(0), "rd:TRICE line %t (%%t -1)\n", 0 );
            TRICE( Id(0), "rd:TRICE line %t (%%t -1)\n", 2 );
            TRICE( Id(0), "rd:TRICE line %u (%%u)\n", -1 );
            TRICE( Id(0), "rd:TRICE line %b (%%b)\n", -2 );
            TRICE( Id(0), "rd:TRICE line %o (%%o)\n", -3 );
            TRICE( Id(0), "rd:TRICE line %x (%%x)\n", -4 );
            TRICE( Id(0), "rd:TRICE line %d (%%d)\n", -5 );
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
        TRICE0 (Id(0), "--------------------------------------------------\n" );
        {
            uint32_t by[8] = {1,2,3,4,5,6};
            TRICE32(  Id(0), "msg: message = %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
            TRICE32( Id(0), "tim: pre TriceEncryption SysTick=%d\n", SYSTICKVAL );
            TriceEncrypt(by, 6);
            TRICE32( Id(0), "tim: post TriceEncryption SysTick=%d\n", SYSTICKVAL );
            TRICE32(  Id(0), "att: TriceEncrypted =  %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
            #ifdef TRICE_DECRYPT
            TRICE16_1( Id(0), "tim: pre TriceDecryption SysTick=%d\n", SYSTICKVAL );
            TriceDecrypt(by, 6);
            TRICE16_1( Id(0), "tim: post TriceDecryption SysTick=%d\n", SYSTICKVAL );
            #endif
            TRICE32(  Id(0), "msg: messge = %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
        }
        TRICE0 (Id(0), "--------------------------------------------------\n" );
#endif
        break;
        default:
        break;
    }
}



//! SCOPY is a helper macro for struct serialization.
#define SCOPY( element ) do{ char* n = #element; int size = sizeof( src->element ); memcpy( p, &(src->element), size ); p += size; TRICE_S( Id(0), "rd:sizeof(%8s)", n ); TRICE( Id(0), " = %d\n", size);}while(0);

    //! DCOPY is a helper macro for struct deserialization.
#define DCOPY( element ) do{ char* n = #element; int size = sizeof( dst->element ); memcpy( &(dst->element), p, size ); p += size; TRICE_S( Id(0), "rd:sizeof(%8s)", n ); TRICE( Id(0), " = %d\n", size);}while(0);


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
    TRICE ( Id(0), "inf: Tryout tx struct:" );
    TRICE_B( Id(0), " %02x ", &tx, sizeof(tx) );
    TRICE( Id(0), "\n" );
    
    TRICE ( Id(0), "inf: Tryout buffer:" );
    TRICE_B( Id(0), " %02x ", dst, len );
    TRICE( Id(0), "\n" );

    src = dst; // "data transfer"
    
    len = deserializeTryout( &rx, src );
    TRICE ( Id(0), "inf: Tryout rx struct:" );
    TRICE_B( Id(0), " %02x ", &rx, sizeof(rx) );
    TRICE( Id(0), "\n" );

    TRICE( Id(0), "inf:sizeOf(Trypout) = %d, buffer length = %d\n", sizeof(tx), len );
    TRICE8_F( Id(0), "info:TryoutStructFunction", &tx, sizeof(tx) );
    TRICE8_F( Id(0), "info:TryoutBufferFunction", dst, len ); 
}

static void exampleOfManualJSONencoding(void){
    typedef  struct {
    int Apple, Birn;
    float Fish;
    } Ex_t;
    Ex_t Ex = { -1, 2, 2.781 };
    TRICE( Id(0), "att:MyStructEvaluationFunction(json:ExA{Apple:%d, Birn:%u, Fisch:%f}\n", Ex.Apple, Ex.Birn, aFloat(Ex.Fish) );
}

void exampleOfBuffersAndFunctions(void){
    static int8_t   b8[24] = { 0, -1, -2, 0x33, 4, 5, 6, 7, 8, 9, 10, 11, 0, -1, -2, 0x33, 4, 5, 6, 7, 8, 9, 10, 11 };
    static int16_t b16[] = { 0, -1, -2, 0x3344 };
    static int32_t b32[] = { 0, -1, -2, 0x33445555};
    static int64_t b64[4] = { 0, -1, -2, 0x3344555566666666 };
    char* s = "abcde 12345";
    uint32_t len;

    TRICE_S( Id(0), "msg:With TRICE_S:%s\n", s );
    len = strlen(s);
    TRICE_N( Id(0), "sig:With TRICE_N:%s\n", s, len );
    TRICE32( Id(0), "att:len=%u:With TRICE_B:\n", len);
    TRICE_B( Id(0), "  %02x", s, len );
    TRICE( Id(0), "\n" );
    TRICE_B( Id(0), "%4d", s, len );
    TRICE( Id(0), "\n" );

    TRICE( Id(0), "notice:TRICE_B example: " );
    TRICE_B( Id(0), "  %02x", b8, sizeof(b8)/sizeof(int8_t) );
    TRICE( Id(0), "\n" );
    TRICE( Id(0), "notice:TRICE8_B example:" );
    TRICE8_B( Id(0), "  %02x", b8, sizeof(b8)/sizeof(int8_t) );
    TRICE( Id(0), "\n" );
    TRICE( Id(0), "notice:TRICE16_B example:" );
    TRICE16_B( Id(0), "  %04x", b16, sizeof(b16)/sizeof(int16_t) );
    TRICE( Id(0), "\n" );
    TRICE( Id(0), "notice:TRICE32_B example:" );
    TRICE32_B( Id(0), "  %08x", b32, sizeof(b32)/sizeof(int32_t) );
    TRICE( Id(0), "\n" );
    TRICE( Id(0), "notice:TRICE64_B example:" );
    TRICE64_B( Id(0), "  %016x", b64, sizeof(b64)/sizeof(int64_t) );
    TRICE( Id(0), "\n" );    

    TRICE( Id(0), "notice:TRICE_F example: " );
    TRICE_F( Id(0), "info:FunctionNameW",   b8,  sizeof(b8) /sizeof(int8_t) );

    TRICE( Id(0), "notice:TRICE8_F example:" );
    TRICE8_F(  Id(0), "info:FunctionNameW",   b8,  sizeof(b8) /sizeof(int8_t) );

    TRICE( Id(0), "notice:TRICE16_F example:" );
    TRICE16_F( Id(0), "sig:FunctionNameX",    b16, sizeof(b16)/sizeof(int16_t) );

    TRICE( Id(0), "notice:TRICE32_F example:" );
    TRICE32_F( Id(0), "diag:FunctionNameY",   b32, sizeof(b32)/sizeof(int32_t) );

    TRICE( Id(0), "notice:TRICE64_F example:" );
    TRICE64_F( Id(0), "fatal:FunctionNameZ", b64, sizeof(b64)/sizeof(int64_t) );
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
        TRICE( Id(0), "msg:exp=%02x act=%02x\n", expBuf[i], actBuf[i] );
        if( expBuf[i] != actBuf[i] ){
            return 0;
        }
    }
    return result;
}

void TCOBSCheck( void ){
    uint8_t result[80];
    for( int i = 0; i < TCOBS_TESTDATASET_COUNT; i++ ){
        TRICE( Id(0), "dbg: Set %d\n", i );
        int rlen = TCOBSEncode( result, TCOBSTestData[i].in, TCOBSTestData[i].ilen );
        if( !equal( TCOBSTestData[i].out, TCOBSTestData[i].olen, result, rlen ) ){
            TRICE( Id(0), "ERROR: TCOBS!\n" );
        }
    }
}
*/
