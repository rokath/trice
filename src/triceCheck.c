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
            TRICE( Id( 7420), "FATAL:magenta+b:red\n" );
            TRICE( Id( 2244), "CRITICAL:red+i:default+h\n" );
            TRICE( Id( 1977), "EMERGENCY:red+i:blue\n" );
            TRICE( Id( 7846), "ERROR:11:red\n" );
            TRICE( Id( 2831), "WARNING:11+i:red\n" );
            TRICE( Id( 3150), "ATTENTION:11:green\n" );
            TRICE( Id( 1327), "INFO:cyan+b:default+h\n" );
            TRICE( Id( 3111), "DEBUG:130+i\n" );
            TRICE( Id( 3159), "TRACE:default+i:default+h\n" );
            TRICE( Id( 2039), "TIME:blue+i:blue+h\n" );
            TRICE( Id( 5589), "MESSAGE:green+h:black\n" );
            TRICE( Id( 7322), "READ:black+i:yellow+h\n" );
            TRICE( Id( 7707), "WRITE:black+u:yellow+h\n" );
            TRICE( Id( 6334), "RECEIVE:black+h:black\n" );
            TRICE( Id( 1556), "TRANSMIT:black:black+h\n" );
            TRICE( Id( 6822), "DIAG:yellow+i:default+h\n" );
            TRICE( Id( 6078), "INTERRUPT:magenta+i:default+h\n" );
            TRICE( Id( 7224), "SIGNAL:118+i\n" );
            TRICE( Id( 1931), "TEST:yellow+h:black\n" );
            TRICE( Id( 7227), "DEFAULT:off\n" );
            TRICE( Id( 4609), "NOTICE:blue:white+h\n" );
            TRICE( Id( 1296), "ALERT:magenta:magenta+h\n" );
            TRICE( Id( 5292), "ASSERT:yellow+i:blue\n" );
            TRICE( Id( 7304), "ALARM:red+i:white+h\n" );
            TRICE( Id( 5651), "CYCLE:blue+i:default+h\n" );
            TRICE( Id( 1800), "VERBOSE:blue:default\n" );
        break;
        case 20:
            TRICE( Id( 1322), "fatal:magenta+b:red\n" );
            TRICE( Id( 6227), "critical:red+i:default+h\n" );
            TRICE( Id( 6257), "emergency:red+i:blue\n" );
            TRICE( Id( 7793), "error:11:red\n" );
            TRICE( Id( 1052), "warning:11+i:red\n" );
            TRICE( Id( 2635), "attention:11:green\n" );
            TRICE( Id( 6972), "info:cyan+b:default+h\n" );
            TRICE( Id( 3148), "debug:130+i\n" );
            TRICE( Id( 6639), "trace:default+i:default+h\n" );
            TRICE( Id( 2521), "tme:blue+i:blue+h\n" );
            TRICE( Id( 5572), "message:green+h:black\n" );
            TRICE( Id( 4573), "read:black+i:yellow+h\n" );
            TRICE( Id( 1606), "write:black+u:yellow+h\n" );
            TRICE( Id( 2807), "receive:black+h:black\n" );
            TRICE( Id( 5762), "transmit:black:black+h\n" );
            TRICE( Id( 1929), "diag:yellow+i:default+h\n" );
            TRICE( Id( 2022), "interrupt:magenta+i:default+h\n" );
            TRICE( Id( 6837), "signal:118+i\n" );
            TRICE( Id( 4171), "test:yellow+h:black\n" );
            TRICE( Id( 4554), "default:off\n" );
            TRICE( Id( 5163), "notice:blue:white+h\n" );
            TRICE( Id( 3309), "allert:magenta:magenta+h\n" );
            TRICE( Id( 3897), "assert:yellow+i:blue\n" );
            TRICE( Id( 5118), "alarm:red+i:white+h\n" );
            TRICE( Id( 4798), "cycle:blue+i:default+h\n" );
            TRICE( Id( 3641), "verbose:blue:default\n" );
        break;
        case 30:
            TRICE( Id( 6183), "dbg:Hi!\n" );
            TRICE( Id( 2818), "dbg:\tHi!\n" );
            TRICE( Id( 6245), "dbg:\t\tHi!\n" );
            TRICE( Id( 7445), "dbg:\t\t\tHi!\n" );
            TRICE( Id( 5722), "dbg:\\tHi!\n" );
            TRICE( Id( 7147), "dbg:\aHi!\n" );
            TRICE( Id( 3715), "dbg:\a\aHi!\n" );
            TRICE( Id( 2096), "dbg:\a\a\aHi!\n" );
            TRICE( Id( 6987), "dbg:\\aHi!\n" );
        break;
        case 40:{
            unsigned len = strlen(s);
            TRICE32( Id( 7354), "dbg:len=%u:", len );
            TRICE_S( Id( 5129), "sig:TRICE_S=%s\n", s );
            TRICE32( Id( 1439), "dbg:len=%u:", len);
            TRICE_N( Id( 5182), "sig:TRICE_N=%s\n", s, len );

            TRICE( Id( 7260), "rd:TRICE line %E (%%E)\n", aFloat(-555555555.5555555555) );
            TRICE( Id( 3291), "rd:TRICE line %F (%%F)\n", aFloat(-555555555.5555555555) );
            TRICE( Id( 5712), "rd:TRICE line %G (%%G)\n", aFloat(-555555555.5555555555) );
        }
        break;
        case 50:
            TRICE32_1( Id( 7679), "rd:TRICE32_1 line %u (%%u)\n", -1 );
            TRICE32_1( Id( 5550), "rd:TRICE32_1 line %b (%%b)\n", -1 );
            TRICE32_1( Id( 7867), "rd:TRICE32_1 line %o (%%o)\n", -1 );
            TRICE32_1( Id( 7875), "rd:TRICE32_1 line %O (%%O)\n", -1 );
            TRICE32_1( Id( 4680), "rd:TRICE32_1 line %X (%%X)\n", -1 );
            TRICE32_1( Id( 5219), "rd:TRICE32_1 line %x (%%x)\n", -1 );
            TRICE32_1( Id( 1169), "rd:TRICE32_1 line %d (%%d)\n", -1 );
        break;
        case 60:
            TRICE64_1( Id( 1058), "rd:TRICE64_1 line %u (%%u)\n", -1 );
            TRICE64_1( Id( 6341), "rd:TRICE64_1 line %b (%%b)\n", -1 );
            TRICE64_1( Id( 2590), "rd:TRICE64_1 line %o (%%o)\n", -1 );
            TRICE64_1( Id( 7612), "rd:TRICE64_1 line %O (%%O)\n", -1 );
            TRICE64_1( Id( 4929), "rd:TRICE64_1 line %X (%%C)\n", -1 );
            TRICE64_1( Id( 2778), "rd:TRICE64_1 line %x (%%x)\n", -1 );
            TRICE64_1( Id( 7508), "rd:TRICE64_1 line %d (%%d)\n", -1 );
        break;
        case 70:
            TRICE64( Id( 7618), "rd:TRICE64 line %E (%%E)\n", aDouble(-555555555.5555555555) );
            TRICE64( Id( 5474), "rd:TRICE64 line %F (%%F)\n", aDouble(-555555555.5555555555) );
            TRICE64( Id( 3164), "rd:TRICE64 line %G (%%G)\n", aDouble(-555555555.5555555555) );
        break;
        case 80:
            TRICE( Id( 5683), "sig:Some time measurements\n" );
            TRICE( Id( 2113), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
            TRICE( Id( 1076), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
            TRICE( Id( 2052), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
            TRICE( Id( 7509), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
            TRICE64( Id( 1524), "rd:TRICE64 %d, %d\n", 1, 2 );
        break;
        case 90:
            TRICE( Id( 7306), "tim:TRICE START time message\n");
            TRICE( Id( 2595), "tim:TRICE STOP time message\n");
            TRICE( Id( 4965), "tim:TRICE START time message\n");
            TRICE( Id( 7388), "tim:TRICE STOP time message\n");
            TRICE( Id( 4826), "tim:TRICE START time message\n");
            TRICE64( Id( 7624), "rd:TRICE64 %d, %d\n", 1, 2 );
            TRICE( Id( 1898), "tim:TRICE STOP time message\n");
        break;
        case 100:
          //TRICE( Id( 2990), "tim:TRICE --------------------------------------------------\n");
          //TRICE( Id( 6258), "tim:TRICE --------------------------------------------------\n");
          //TRICE( Id( 4854), "tim:TRICE --------------------------------------------------\n");
          //TRICE( Id( 5553), "tim:TRICE --------------------------------------------------\n");
            TRICE( Id( 6283), "tim:TRICE --------------------------------------------------\n");
            TRICE( Id( 5556), "tim:TRICE --------------------------------------------------\n");
            TRICE64( Id( 2746), "rd:TRICE64 %d, %d\n", 1, 2 );
        break;
        case 110:
          //TRICE( Id( 5966), "tim:TRICE --------------------------------------------------\n");
          //TRICE( Id( 7735), "tim:TRICE --------------------------------------------------\n");
          //TRICE( Id( 7832), "tim:TRICE --------------------------------------------------\n");
          //TRICE( Id( 7842), "tim:TRICE --------------------------------------------------\n");
          //TRICE( Id( 5976), "tim:TRICE --------------------------------------------------\n");
            TRICE( Id( 2869), "tim:TRICE --------------------------------------------------\n");
            TRICE( Id( 3506), "tim:TRICE --------------------------------------------------\n");
            TRICE64( Id( 4889), "rd:TRICE64 %d, %d\n", 1, 2 );
        break;
        case 120:
            TRICE( Id( 3885),  "rd:TRICE32_1 line %d\n", __LINE__ );
            TRICE64_1( Id( 4144),  "rd:TRICE64_1 %d\n", __LINE__ );
            TRICE32_2( Id( 1603),  "rd:TRICE32_2 line %d,%d\n", __LINE__, 2 );
            TRICE64_2( Id( 7163),  "rd:TRICE64_2 line %d,%d\n", __LINE__, 2 );
        break;
        case 130:
            TRICE( Id( 6182), "sig:TRICE with 1 to 12 values\n" );
            TRICE( Id( 4346),"rd:TRICE %d\n", -1 );
            TRICE( Id( 5585),"rd:TRICE %d, %d\n", -1, -2 );
            TRICE( Id( 5114),"rd:TRICE %d, %d, %d\n", -1, -2, -3 );
            TRICE( Id( 6099),"rd:TRICE %d, %d, %d, %d\n", -1, -2, -3, -4 );
            TRICE( Id( 2493),"rd:TRICE %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
            TRICE( Id( 3618),"rd:TRICE %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE( Id( 7990),"rd:TRICE %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
            TRICE( Id( 3339),"rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 140:
            TRICE( Id( 2232),"rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE( Id( 6850),"rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
            TRICE( Id( 6090),"rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE( Id( 5495),"rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 150:
            TRICE( Id( 2330), "sig:TRICE_n with 1 to 12 values\n" );
            TRICE_1( Id( 6881),   "rd:TRICE_1 %d\n", 1 );
            TRICE_2( Id( 6054),   "rd:TRICE_2 %d, %d\n", 1, 2 );
            TRICE_3( Id( 7515),   "rd:TRICE_3 %d, %d, %d\n", 1, 2, 3 );
            TRICE_4( Id( 4409),   "rd:TRICE_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
            TRICE_5( Id( 6543),   "rd:TRICE_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
            TRICE_6( Id( 5706),   "rd:TRICE_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
            TRICE_7( Id( 3749),   "rd:TRICE_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
            TRICE_8( Id( 1187),   "rd:TRICE_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
        break;
        case 160:
            TRICE_9( Id( 5157),   "rd:TRICE_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
            TRICE_10( Id( 4191),  "rd:TRICE_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
            TRICE_11( Id( 5030),  "rd:TRICE_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
            TRICE_12( Id( 1708),  "rd:TRICE_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );				
        break;
        case 170:
            TRICE( Id( 2228), "sig:TRICE8 with 1 to 12 values\n" );
            TRICE8( Id( 4777),"rd:TRICE8 %d\n", -1 );
            TRICE8( Id( 6821),"rd:TRICE8 %d, %d\n", -1, -2 );
            TRICE8( Id( 2030),"rd:TRICE8 %d, %d, %d\n", -1, -2, -3 );
            TRICE8( Id( 2955),"rd:TRICE8 %d, %d, %d, %d\n", -1, -2, -3, -4 );
            TRICE8( Id( 6626),"rd:TRICE8 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
            TRICE8( Id( 4356),"rd:TRICE8 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE8( Id( 4819),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
            TRICE8( Id( 3598),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 180:
            TRICE8( Id( 1273),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE8( Id( 5514),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
            TRICE8( Id( 6823),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE8( Id( 1093),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 190:
            TRICE( Id( 6889), "sig:TRICE8_n with 1 to 12 values\n" );
            TRICE8_1( Id( 4793),   "rd:TRICE8_1 %d\n", 1 );
            TRICE8_2( Id( 1120),   "rd:TRICE8_2 %d, %d\n", 1, 2 );
            TRICE8_3( Id( 2618),   "rd:TRICE8_3 %d, %d, %d\n", 1, 2, 3 );
            TRICE8_4( Id( 3465),   "rd:TRICE8_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
            TRICE8_5( Id( 5134),   "rd:TRICE8_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
            TRICE8_6( Id( 5101),   "rd:TRICE8_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
            TRICE8_7( Id( 3444),   "rd:TRICE8_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
        break;
        case 200:
            TRICE8_8( Id( 6625),   "rd:TRICE8_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
            TRICE8_9( Id( 4028),   "rd:TRICE8_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
            TRICE8_10( Id( 4608),  "rd:TRICE8_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
            TRICE8_11( Id( 3273),  "rd:TRICE8_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
            TRICE8_12( Id( 4859),  "rd:TRICE8_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );
        break;
        case 210:
            TRICE( Id( 7429), "sig:TRICE16 with 1 to 12 values\n" );
            TRICE16( Id( 3404),"rd:TRICE16 %d\n", -1 );
            TRICE16( Id( 6706),"rd:TRICE16 %d, %d\n", -1, -2 );
            TRICE16( Id( 3774),"rd:TRICE16 %d, %d, %d\n", -1, -2, -3 );
            TRICE16( Id( 4243),"rd:TRICE16 %d, %d, %d, %d\n", -1, -2, -3, -4 );
            TRICE16( Id( 6266),"rd:TRICE16 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
            TRICE16( Id( 6750),"rd:TRICE16 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE16( Id( 6634),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
            TRICE16( Id( 1036),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 220:
            TRICE16( Id( 2626),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE16( Id( 7537),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
            TRICE16( Id( 6097),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE16( Id( 6891),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 230:
            TRICE( Id( 6800), "sig:TRICE16_n with 1 to 12 values\n" );
            TRICE16_1( Id( 2697),  "rd:TRICE16_1 %d\n", 1 );
            TRICE16_2( Id( 4552),  "rd:TRICE16_2 %d, %d\n", 1, 2 );
            TRICE16_3( Id( 5086),  "rd:TRICE16_3 %d, %d, %d\n", 1, 2, 3 );
            TRICE16_4( Id( 7226),  "rd:TRICE16_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
            TRICE16_5( Id( 4651),  "rd:TRICE16_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
            TRICE16_6( Id( 2009),  "rd:TRICE16_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
            TRICE16_7( Id( 1805),  "rd:TRICE16_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
            TRICE16_8( Id( 4458),  "rd:TRICE16_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
        break;
        case 240:
            TRICE16_9( Id( 4396),  "rd:TRICE16_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
            TRICE16_10( Id( 3901), "rd:TRICE16_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
            TRICE16_11( Id( 1196), "rd:TRICE16_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
            TRICE16_12( Id( 5622), "rd:TRICE16_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );
        break;
        case 250:
            TRICE( Id( 3923), "sig:TRICE16 with 1 to 12 pointer\n" );
            TRICE16( Id( 2676),"rd:TRICE16 %p\n", -1 );
            TRICE16( Id( 5677),"rd:TRICE16 %p, %p\n", -1, -2 );
            TRICE16( Id( 1218),"rd:TRICE16 %p, %p, %p\n", -1, -2, -3 );
            TRICE16( Id( 1822),"rd:TRICE16 %p, %p, %p, %p\n", -1, -2, -3, -4 );
            TRICE16( Id( 7576),"rd:TRICE16 %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5 );
            TRICE16( Id( 3413),"rd:TRICE16 %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6 );
            TRICE16( Id( 1460),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7);
            TRICE16( Id( 3125),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 260:
            TRICE16( Id( 5870),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE16( Id( 7559),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
            TRICE16( Id( 2528),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE16( Id( 3313),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 270:
            TRICE( Id( 3303), "sig:TRICE16 with 1 to 12 hex\n" );
            TRICE16( Id( 2057),"rd:TRICE16 %X\n", -1 );
            TRICE16( Id( 2216),"rd:TRICE16 %X, %X\n", -1, -2 );
            TRICE16( Id( 3850),"rd:TRICE16 %X, %X, %X\n", -1, -2, -3 );
            TRICE16( Id( 2529),"rd:TRICE16 %X, %X, %X, %X\n", -1, -2, -3, -4 );
            TRICE16( Id( 7966),"rd:TRICE16 %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5 );
            TRICE16( Id( 2313),"rd:TRICE16 %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6 );
            TRICE16( Id( 2265),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7);
            TRICE16( Id( 7807),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 280:
            TRICE16( Id( 6977),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE16( Id( 6108),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
            TRICE16( Id( 1700),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE16( Id( 2700),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 290:
            TRICE( Id( 3067), "sig:TRICE32 with 1 to 12 values\n" );
            TRICE32( Id( 5489),"rd:TRICE32 %d\n", -1 );
            TRICE32( Id( 5560),"rd:TRICE32 %d, %d\n", -1, -2 );
            TRICE32( Id( 3397),"rd:TRICE32 %d, %d, %d\n", -1, -2, -3 );
            TRICE32( Id( 7650),"rd:TRICE32 %d, %d, %d, %d\n", -1, -2, -3, -4 );
            TRICE32( Id( 7629),"rd:TRICE32 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
            TRICE32( Id( 5320),"rd:TRICE32 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE32( Id( 7046),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
            TRICE32( Id( 6510),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 300:
            TRICE32( Id( 1247),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE32( Id( 6218),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
            TRICE32( Id( 6864),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE32( Id( 3247),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 310:
            TRICE( Id( 2394), "signal:TRICE32_n with 1 to 12 values\n" );
            TRICE32_1( Id( 6922),  "rd:TRICE32_1 %d\n", 1 );
            TRICE32_2( Id( 6409),  "rd:TRICE32_2 %d, %d\n", 1, 2 );
            TRICE32_3( Id( 4958),  "rd:TRICE32_3 %d, %d, %d\n", 1, 2, 3 );
            TRICE32_4( Id( 3782),  "rd:TRICE32_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
            TRICE32_5( Id( 6340),  "rd:TRICE32_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
            TRICE32_6( Id( 5869),  "rd:TRICE32_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
            TRICE32_7( Id( 3362),  "rd:TRICE32_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
            TRICE32_8( Id( 7378),  "rd:TRICE32_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
        break;
        case 320:
            TRICE32_9( Id( 3526),  "rd:TRICE32_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
            TRICE32_10( Id( 5859), "rd:TRICE32_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
            TRICE32_11( Id( 4194), "rd:TRICE32_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
            TRICE32_12( Id( 7438), "rd:TRICE32_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );
        break;
        case 330:
            TRICE( Id( 4843), "sig:TRICE64 with 1 to 12 values\n" );
            TRICE64( Id( 7873),"rd:TRICE64 %d\n", -1 );
            TRICE64( Id( 3768),"rd:TRICE64 %d, %d\n", -1, -2 );
            TRICE64( Id( 1632),"rd:TRICE64 %d, %d, %d\n", -1, -2, -3 );
            TRICE64( Id( 7047),"rd:TRICE64 %d, %d, %d, %d\n", -1, -2, -3, -4 );
            TRICE64( Id( 6310),"rd:TRICE64 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
            TRICE64( Id( 1777),"rd:TRICE64 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE64( Id( 2434),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
            TRICE64( Id( 1761),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 340:
            TRICE64( Id( 2565),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE64( Id( 3696),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
            TRICE64( Id( 4096),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE64( Id( 1306),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 350:
            TRICE( Id( 4493), "sig:TRICE64_n with 1 to 12 values\n" );
            TRICE64_1( Id( 2845),  "rd:TRICE64_1 %d\n", 1 );
            TRICE64_2( Id( 7544),  "rd:TRICE64_2 %d, %d\n", 1, 2 );
            TRICE64_2( Id( 4551),  "rd:TRICE64_2 %x, %x\n", 0x1122334455667788, 0x0102030405060708 );
            TRICE64_3( Id( 6955),  "rd:TRICE64_3 %d, %d, %d\n", 1, 2, 3 );
            TRICE64_4( Id( 2205),  "rd:TRICE64_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
            TRICE64_5( Id( 7969),  "rd:TRICE64_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
            TRICE64_6( Id( 6618),  "rd:TRICE64_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
            TRICE64_7( Id( 6851),  "rd:TRICE64_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
            TRICE64_8( Id( 5725),  "rd:TRICE64_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
        break;
        case 360:
            TRICE64_9( Id( 5735),  "rd:TRICE64_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
            TRICE64_10( Id( 1417), "rd:TRICE64_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
            TRICE64_11( Id( 3679), "rd:TRICE64_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
            TRICE64_12( Id( 4982), "rd:TRICE64_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );
        break;
        case 370:
            TRICE( Id( 3564), "sig:Colors and ticks\n" );
            TRICE(Id( 6565), "--------------------------------------------------\n");
            TRICE(Id( 7666), "--------------------------------------------------\n");
            TRICE(Id( 2906), "dbg:12345 as 16bit is %016b\n", 12345);
            TRICE(Id( 2121), "--------------------------------------------------\n");
            TRICE(Id( 5658), "sig:This ASSERT error is just a demo and no real error:\n");
            TRICE(Id( 3703), "--------------------------------------------------\n");
        break;
        case 380:
            TRICE(Id( 1516), "ERR:error       message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id( 6838), "WRN:warning     message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id( 3908), "ATT:attention   message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id( 7346), "DIA:diagnostics message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id( 1316), "TIM:timing      message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id( 5543), "DBG:debug       message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id( 4586), "SIG:signal      message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id( 3455), "RD:read         message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id( 7021), "WR:write        message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id( 5773), "ISR:interrupt   message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id( 3047), "MSG:normal      message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id( 2459), "INFO:informal   message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id( 4444), "time:TRICE32_1  message, SysTick is %6u\n", SYSTICKVAL);
        break;
        case 390:
            TRICE( Id( 3487), "sig:Various ranges and formats\n" );
            TRICE8( Id( 5316), "tst:TRICE8  %%03x -> %03x  %03x  %03x  %03x\n", 1, 0x7f, 0x80, 0xff);
            TRICE8( Id( 6631), "tst:TRICE8   %%4d -> %4d %4d %4d %4d\n", 1, 0x7f, 0x80, 0xff);
            TRICE8( Id( 4454), "tst:TRICE8   %%4u -> %4u %4u %4u %4u\n", 1, 0x7f, 0x80, 0xff);
            TRICE8( Id( 6179), "tst:TRICE8   %%4o -> %4o %4o %4o %4o\n", 1, 0x7f, 0x80, 0xff);
            TRICE8( Id( 7208), "tst:TRICE8   %%4O -> %4O %4O %4O %4O\n", 1, 0x7f, 0x80, 0xff);
            TRICE8( Id( 4288), "tst:TRICE8   %%4X -> %4X %4X %4X %4X\n", 1, 0x7f, 0x80, 0xff);
            TRICE8( Id( 1486), "tst:TRICE8   %%4x -> %4x %4x %4x %4x\n", 1, 0x7f, 0x80, 0xff);
            TRICE8( Id( 3425), "tst:TRICE8   %%8b -> %8b %8b %8b %8b\n", 1, 0x7f, 0x80, 0xff);
            TRICE8( Id( 4601), "tst:TRICE8  %%08b -> %08b %08b %08b %08b\n", 1, 0x7f, 0x80, 0xff);
        break;
        case 400:
            TRICE16(Id( 5900), "tst:TRICE16  %%05x ->   %05x   %05x   %05x   %05x\n", 1, 0x7fff, 0x8000, 0xffff);
            TRICE16(Id( 5248), "tst:TRICE16   %%6d ->  %6d  %6d  %6d  %6d\n",         1, 0x7fff, 0x8000, 0xffff);
            TRICE16(Id( 3281), "tst:TRICE16   %%7o -> %7o %7o %7o %7o\n",             1, 0x7fff, 0x8000, 0xffff);

            TRICE32(Id( 2836), "tst:TRICE32 %%09x ->      %09x      %09x       %09x     %09x\n", 1, 0x7fffffff, 0x80000000, 0xffffffff);
            TRICE32(Id( 2810), "tst:TRICE32 %%10d ->     %10d     %10d     %10d    %10x\n",      1, 0x7fffffff, 0x80000000, 0xffffffff);
            TRICE32(Id( 4886), "att:TRICE32 %40b\n", 0xAAAAAAAA);
        break;
        case 410:
            TRICE64(Id( 6724), "tst:TRICE64 %%09x ->      %09x      %09x       %09x     %09x\n", 1, 0x7fffffffffffffff, 0x8000000000000000, 0xffffffffffffffff);
            TRICE64(Id( 3611), "tst:TRICE64 %%10d ->     %10d     %10d     %10d    %10x\n",      1, 0x7fffffffffffffff, 0x8000000000000000, 0xffffffffffffffff);
            TRICE64(Id( 1221), "att:TRICE64 %70b\n", 0xAAAAAAAAAAAAAAAA);
        break;
        case 420:
            TRICE( Id( 1655), "sig:Legacy TRICE8\n" );
            TRICE8_1(Id( 7808), "tst:TRICE8_1 %02x\n", 0xA1);
            TRICE8_2(Id( 2805), "tst:TRICE8_2 %02x %02x\n", 0xA1, 0xA2);
            TRICE8_3(Id( 4623), "tst:TRICE8_3 %02x %02x %02x\n", 0xA1, 0xA2, 0xA3);
            TRICE8_4(Id( 3418), "tst:TRICE8_4 %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4);
            TRICE8_5(Id( 2358), "tst:TRICE8_5 %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5 );
            TRICE8_6(Id( 7880), "tst:TRICE8_6 %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6);
            TRICE8_7(Id( 3839), "tst:TRICE8_7 %02x %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7);
            TRICE8_8(Id( 2938), "tst:TRICE8_8 %02x %02x %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8);
        break;
        case 430:
            TRICE8_1(Id( 4327), "tst:TRICE8_1 %d\n",                      -1);
            TRICE8_2(Id( 2787), "tst:TRICE8_2 %d %d\n",                   -1, -2);
            TRICE8_3(Id( 3021), "tst:TRICE8_3 %d %d %d\n",                -1, -2, -3);
            TRICE8_4(Id( 3142), "tst:TRICE8_4 %d %d %d %d\n",             -1, -2, -3, -4);
            TRICE8_5(Id( 1509), "tst:TRICE8_5 %d %d %d %d %d\n",          -1, -2, -3, -4, -5);
            TRICE8_6(Id( 4666), "tst:TRICE8_6 %d %d %d %d %d %d\n",       -1, -2, -3, -4, -5, -6);
            TRICE8_7(Id( 2524), "tst:TRICE8_7 %d %d %d %d %d %d %d\n",    -1, -2, -3, -4, -5, -6, -7);
            TRICE8_8(Id( 3933), "tst:TRICE8_8 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8);
        break;
        case 440:
            TRICE8_1(Id( 2803), "tst:TRICE8_1 %d\n", 1);
            TRICE8_2(Id( 1765), "tst:TRICE8_2 %d %d\n", 1, 2);
            TRICE8_3(Id( 4080), "tst:TRICE8_3 %d %d %d\n", 1, 2, 3);
            TRICE8_4(Id( 3319), "tst:TRICE8_4 %d %d %d %d\n", 1, 2, 3, 4);
            TRICE8_5(Id( 2498), "tst:TRICE8_5 %d %d %d %d %d\n", 1, 2, 3, 4, 5);
            TRICE8_6(Id( 1792), "tst:TRICE8_6 %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6);
            TRICE8_7(Id( 4497), "tst:TRICE8_7 %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7);
            TRICE8_8(Id( 1030), "tst:TRICE8_8 %d %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7, 8);
        break;
        case 450:
            TRICE8_1(Id( 7491), "tst:TRICE8_1 %u\n", 201);
            TRICE8_2(Id( 2033), "tst:TRICE8_2 %u %u\n", 201, 202);
            TRICE8_3(Id( 4167), "tst:TRICE8_3 %u %u %u\n", 201, 202, 203);
            TRICE8_4(Id( 3027), "tst:TRICE8_4 %u %u %u %u\n", 201, 202, 203, 204);
            TRICE8_5(Id( 3556), "tst:TRICE8_5 %u %u %u %u %u\n", 201, 202, 203, 204, 205);
            TRICE8_6(Id( 1602), "tst:TRICE8_6 %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206);
            TRICE8_7(Id( 7037), "tst:TRICE8_7 %u %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206, 207);
            TRICE8_8(Id( 5745), "tst:TRICE8_8 %u %u %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206, 207, 208);
            TRICE8_8(Id( 5500), "tst:TRICE8_1 %%d=%d, %%u=%u, 0x%%x=0x%x, 0x%%2x=0x%2x, 0x%%02x=0x%02x, 0x%%3x=0x%3x, 0x%%03x=0x%03x, %%b=%b\n", 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81);
        break;
        case 460:
            TRICE( Id( 7394), "sig:Legacy TRICE16-64\n" );
            TRICE16_1(Id( 2902), "tst:TRICE16_1 %d\n", -111);
            TRICE16_2(Id( 3091), "tst:TRICE16_2 %d %d\n", -111, -222);
            TRICE16_3(Id( 7510), "tst:TRICE16_3 %d %d %d\n", -111, -222, -333);
            TRICE16_4(Id( 4728), "tst:TRICE16_4 %d %d %d %d\n", -111, -222, -333, -444);
            TRICE32_1(Id( 1335), "tst:TRICE32_1 %08x\n", 0x0123cafe);
            TRICE32_1(Id( 1727), "tst:TRICE32_1 %d\n", -111);
            TRICE32_2(Id( 4596), "tst:TRICE32_2 %x %x\n", -111, -222);
            TRICE32_2(Id( 4577), "tst:TRICE32_2 %d %d\n", -111, -222);
        break;
        case 470:
            TRICE32_3(Id( 3456), "tst:TRICE32_3 %x %x %x\n", -111, -222, -333);
            TRICE32_3(Id( 5312), "tst:TRICE32_3 %d %d %d\n", -111, -222, -333);
            TRICE32_4(Id( 6859), "tst:TRICE32_4 %x %x %x %x\n", -111, -222, -333, -444);
            TRICE32_4(Id( 5439), "tst:TRICE32_4 %d %d %d %d\n", -111, -222, -333, -444);
        break;
        case 480:
            TRICE64_1(Id( 1165), "tst:TRICE64_1 %d\n", -111);
            TRICE64_2(Id( 5308), "tst:TRICE64_2 %d %d\n", -111, -222);
            TRICE16_1(Id( 2982), "tst:TRICE16_1 %u\n", 60001);
            TRICE16_2(Id( 4308), "tst:TRICE16_2 %u %u\n", 60001, 60002);
            TRICE16_3(Id( 7606), "tst:TRICE16_3 %u %u %u\n", 60001, 60002, 60003);
            TRICE16_4(Id( 1786), "tst:TRICE16_4 %u %u %u %u\n", 60001, 60002, 60003, 60004);
            TRICE32_1(Id( 2495), "tst:TRICE32_1 %u\n", 4000000001);
            TRICE32_2(Id( 5082), "tst:TRICE32_2 %u %u\n", 4000000001, 4000000002);
            TRICE32_3(Id( 3841), "tst:TRICE32_3 %u %u %u\n", 4000000001, 4000000002, 4000000003);
            TRICE32_4(Id( 7625), "tst:TRICE32_4 %u %u %u %u\n", 4000000001, 4000000002, 4000000003, 4000000004);
        break;
        case 490:
            TRICE64_1(Id( 3903), "tst:TRICE64_1 %x\n", -1);
            TRICE64_2(Id( 4882), "tst:TRICE64_2 %x %x\n", -1, -2);
            TRICE64_1(Id( 3959), "tst:TRICE64_1 %u\n", -1);
            TRICE64_2(Id( 4034), "tst:TRICE64_2 %u %u\n", -1, -2);
        break;
        case 500:
            TRICE( Id( 4630), "sig:colored single letters and several TRICE macros in one line\n" );
            TRICE(Id( 4159), "e:A");
            TRICE(Id( 5090), "w:B");
            TRICE(Id( 4048), "a:c");
            TRICE(Id( 1790), "wr:d");
            TRICE(Id( 7275), "rd:e\n");
            TRICE(Id( 1264), "diag:f");
            TRICE(Id( 7829), "d:G");
            TRICE(Id( 1744), "t:H");
            TRICE(Id( 4116), "time:i");
            TRICE(Id( 1555), "message:J");
            TRICE(Id( 6014), "dbg:k\n");
        break;
        case 510:
            TRICE(Id( 6472), "1");
            TRICE(Id( 5605), "2");
            TRICE(Id( 5268), "3");
            TRICE(Id( 3837), "4");
            TRICE(Id( 4132), "e:7");
            TRICE(Id( 3029), "m:12");
            TRICE(Id( 1893), "m:123\n");
            TRICE(Id( 1375), "e:A");TRICE(Id( 2917), "w:B");TRICE(Id( 5877), "a:c");
            TRICE(Id( 4632), "wr:d");TRICE(Id( 7539), "rd:e\n");//TRICE(Id( 2008), "diag:f");
        break;
        case 520:
            TRICE( Id( 5420), "sig:Runtime generated strings\n" );
            s = "AAAAAAAAAAAA";
            TRICE32( Id( 7741), "dbg:len=%u:", strlen(s) );
            TRICE_S( Id( 5176), "sig:%s\n", s );
            s = "\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBCCCCCCCC\
";
            TRICE32( Id( 7063), "dbg:len=%u:", strlen(s) );
            TRICE_S( Id( 7637), "sig:%s\n", s );
        break;
        case 530:
        TRICE( Id( 3056), "sig:Runtime generated strings\n" );
            s = "\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\
DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\
EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\
";
            TRICE32( Id( 3009), "dbg:len=%u: ", strlen(s) );
            TRICE_S( Id( 4470), "sig:%s\n", s );
        break;
        case 540:
            TRICE( Id( 1014), "sig:Runtime generated strings\n" );
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
            TRICE32( Id( 3687), "dbg:len=%u: ", strlen(s) );
            TRICE_S( Id( 7719), "sig:%s\n", s );
        break;
        case 550:
            TRICE( Id( 4995), "sig:Runtime generated strings\n" );
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
          TRICE32( Id( 3619), "dbg:len=%u: ", strlen(s) );
          TRICE_S( Id( 1235), "sig:%s\n", s );
        break;
        case 560:{ // https://yourbasic.org/golang/fmt-printf-reference-cheat-sheet/
            int i = 15;
            TRICE( Id( 2639), "sig:Integer (indent, base, sign)\n" );
            TRICE( Id( 3430), "rd: 15 	%d 	Base 10\n", i );
            TRICE( Id( 7801), "rd: +15 	%+d 	Always show sign\n", i );
            TRICE( Id( 5696), "rd:   15 	%4d 	Pad with spaces (width 4, right justified)\n", i );
            TRICE( Id( 3775), "rd: 15   	%-4d 	Pad with spaces (width 4, left justified)\n", i );
            TRICE( Id( 3043), "rd: 0015 	%04d 	Pad with zeroes (width 4)\n", i );
            TRICE( Id( 6229), "rd: 1111 	%b 	Base 2\n", i );
            TRICE( Id( 3192), "rd: 17 	%o 	Base 8\n", i );
            TRICE( Id( 2570), "rd: f 	%x 	Base 16, lowercase\n", i );
            TRICE( Id( 1470), "rd: F 	%X 	Base 16, uppercase\n", i );
            TRICE( Id( 1873), "rd: 0xf 	%#x 	Base 16, with leading 0x\n", i );
        }
        break;
        case 570:{
            char a = 'A';
            TRICE( Id( 6720), "sig:Character (quoted, Unicode)\n" );
            TRICE( Id( 7669), "rd: A 	%c 	Character\n", a );
            TRICE( Id( 2970), "rd: 'A' 	%q 	Quoted character\n", a );
            TRICE( Id( 6044), "rd: U+0041 	%U 	Unicode\n", a );// todo
            TRICE( Id( 1964), "rd: U+0041 'A' 	%#U 	Unicode with character\n", a );// todo
            TRICE( Id( 2567), "sig:Boolean (true/false)\n" );
            TRICE( Id( 7736), "rd:Use %%t to format a boolean as true (%t) or false (%t).\n", 1, 0 );
            TRICE( Id( 7599), "sig:Pointer (hex)\n" );
        }
        break;
        case 580:
            TRICE8( Id( 7074), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x84 );
            TRICE16( Id( 4532), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x1234 );
            TRICE32( Id( 1536), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0xaabbccdd );
            TRICE64( Id( 1507), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x1122334455667788 );
            TRICE( Id( 1717), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0xaabbccdd);
        break;
        case 590:{
            float f = (float)123.456; 
            TRICE( Id( 4115), "sig:Float (indent, precision, scientific notation)\n" );
            TRICE( Id( 3554), "rd: 1.234560e+02		%e 	%%e Scientific notation\n", aFloat(f) );
            TRICE( Id( 2554), "rd: 123.456000		%f 	%%f Decimal point, no exponent\n", aFloat(f) );
            TRICE( Id( 7773), "rd: 123.46	 		%.2f		%%.2f Default width, precision 2\n", aFloat(f) );
            TRICE( Id( 3790), "rd: ␣␣123.46		%8.2f	%%8.2f Width 8, precision 2\n", aFloat(f) );
            TRICE( Id( 3340), "rd: 123.456		%g		%%g Exponent as needed, necessary digits only\n", aFloat(f) );
        }
        break;
        case 600:{
            double d = 123.456;
            TRICE( Id( 7188), "sig:Double (indent, precision, scientific notation)\n" );
            TRICE64( Id( 7792), "rd: 1.234560e+02		%e 	%%e Scientific notation\n", aDouble(d) );
            TRICE64( Id( 6452), "rd: 123.456000		%f 	%%f Decimal point, no exponent\n", aDouble(d) );
            TRICE64( Id( 3920), "rd: 123.46			%.2f    	%%.2f Default width, precision 2\n", aDouble(d) );
            TRICE64( Id( 4504), "rd: ␣␣123.46		%8.2f 	%%8.2f Width 8, precision 2\n", aDouble(d) );
            TRICE64( Id( 2968), "rd: 123.456		%g		%%g Exponent as needed, necessary digits only\n", aDouble(d) );
        }
        break;
        case 610:{
            s = "café";
            TRICE( Id( 1972), "sig:String or byte slice (quote, indent, hex)\n" );
            TRICE_S( Id( 4139), "rd: café 			%s		Plain string\n", s );
            TRICE_S( Id( 6473), "rd: ␣␣café 		%6s 		Width 6, right justify\n", s );
            TRICE_S( Id( 7906), "rd: café␣␣ 		%-6s 		Width 6, left justify\n", s );
            TRICE_S( Id( 3777), "rd: \"café\" 		%q	 	Quoted string\n", s );
            TRICE_S( Id( 1033), "rd: 636166c3a9 		%x	Hex dump of byte values\n", s );
            TRICE_S( Id( 4405), "rd: 63 61 66 c3 a9 	% x	Hex dump with spaces\n", s );
        }
        break;
        case 620:
            TRICE( Id( 1630), "sig:TRICE8 with variable param count 1 to 12\n" );
            TRICE8(Id( 4024), "tst:TRICE8 %d\n", -1 );
            TRICE8(Id( 6003), "tst:TRICE8 %d %d\n", -1, -2 );
            TRICE8(Id( 1360), "tst:TRICE8 %d %d %d\n", -1, -2, -3 );
            TRICE8(Id( 7038), "tst:TRICE8 %d %d %d %d\n", -1, -2, -3, -4 );
            TRICE8(Id( 7053), "tst:TRICE8 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
            TRICE8(Id( 3060), "tst:TRICE8 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE8(Id( 1308), "tst:TRICE8 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
            TRICE8(Id( 1363), "tst:TRICE8 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 630:
            TRICE8(Id( 5413), "tst:TRICE8 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE8(Id( 2100), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
            TRICE8(Id( 1424), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE8(Id( 6831), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 640:
            TRICE8(Id( 6042), "tst:TRICE8 %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );
            TRICE8_1 (Id( 1287), "tst:TRICE8_1  %d\n", -1 );
            TRICE8_2 (Id( 5600), "tst:TRICE8_2  %d %d\n", -1, -2 );
            TRICE8_3 (Id( 3422), "tst:TRICE8_3  %d %d %d\n", -1, -2, -3 );
            TRICE8_4 (Id( 3603), "tst:TRICE8_4  %d %d %d %d\n", -1, -2, -3, -4 );
            TRICE8_5 (Id( 3630), "tst:TRICE8_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
            TRICE8_6 (Id( 4221), "tst:TRICE8_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE8_7 (Id( 1293), "tst:TRICE8_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
            TRICE8_8 (Id( 7949), "tst:TRICE8_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 650:
            TRICE8_9 (Id( 5405), "tst:TRICE8_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE8_10(Id( 3609), "tst:TRICE8_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
            TRICE8_11(Id( 4328), "tst:TRICE8_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE8_12(Id( 6506), "tst:TRICE8_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
            TRICE8  (Id( 4744), "tst:TRICE8   %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );
            TRICE8_9(Id( 3353), "tst:TRICE8_9 %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );
        break;
        case 660:
            TRICE( Id( 6236), "sig:TRICE16 with variable param count 1 to 12\n" );
            TRICE16(Id( 3808), "tst:TRICE16 %d\n", -1 );
            TRICE16(Id( 5739), "tst:TRICE16 %d %d\n", -1, -2 );
            TRICE16(Id( 7710), "tst:TRICE16 %d %d %d\n", -1, -2, -3 );
            TRICE16(Id( 3864), "tst:TRICE16 %d %d %d %d\n", -1, -2, -3, -4 );
            TRICE16(Id( 6936), "tst:TRICE16 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
            TRICE16(Id( 4792), "tst:TRICE16 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE16(Id( 2019), "tst:TRICE16 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
            TRICE16(Id( 5896), "tst:TRICE16 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 670:
            TRICE16(Id( 6585), "tst:TRICE16 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE16(Id( 5295), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
            TRICE16(Id( 2157), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE16(Id( 6057), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 680:
            TRICE16(Id( 3417), "tst:TRICE16 %t %b %x %X %d %u %o %O %p\n", 3, 3, 3, 3, 3, 3, 3, 3, 3 );
            TRICE16_1 (Id( 7036), "tst:TRICE16_1  %d\n", -1 );
            TRICE16_2 (Id( 4296), "tst:TRICE16_2  %d %d\n", -1, -2 );
            TRICE16_3 (Id( 6391), "tst:TRICE16_3  %d %d %d\n", -1, -2, -3 );
            TRICE16_4 (Id( 1661), "tst:TRICE16_4  %d %d %d %d\n", -1, -2, -3, -4 );
            TRICE16_5 (Id( 1035), "tst:TRICE16_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
            TRICE16_6 (Id( 1517), "tst:TRICE16_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE16_7 (Id( 1963), "tst:TRICE16_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
            TRICE16_8 (Id( 7621), "tst:TRICE16_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 690:
            TRICE16_9 (Id( 7688), "tst:TRICE16_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE16_10(Id( 3950), "tst:TRICE16_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
            TRICE16_11(Id( 5743), "tst:TRICE16_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE16_12(Id( 1814), "tst:TRICE16_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
            TRICE16_9(Id( 1374), "tst:TRICE16 %t %b %x %X %d %u %o %O %p\n", 3, 3, 3, 3, 3, 3, 3, 3, 3 );
        break;
        case 700:
            TRICE( Id( 7422), "sig:TRICE32 with variable param count 1 to 12\n" );
            TRICE32(Id( 2468), "tst:TRICE32 %d\n", -1 );
            TRICE32(Id( 3673), "tst:TRICE32 %d %d\n", -1, -2 );
            TRICE32(Id( 6899), "tst:TRICE32 %d %d %d\n", -1, -2, -3 );
            TRICE32(Id( 7262), "tst:TRICE32 %d %d %d %d\n", -1, -2, -3, -4 );
            TRICE32(Id( 3178), "tst:TRICE32 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
            TRICE32(Id( 6903), "tst:TRICE32 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE32(Id( 5657), "tst:TRICE32 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
            TRICE32(Id( 3812), "tst:TRICE32 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 710:
            TRICE32(Id( 4378), "tst:TRICE32 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE32(Id( 2074), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
            TRICE32(Id( 1116), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE32(Id( 7701), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 720:
            TRICE32(Id( 2108), "tst:TRICE32 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));
            TRICE32_1 (Id( 7894), "tst:TRICE32_1  %d\n", -1 );
            TRICE32_2 (Id( 5612), "tst:TRICE32_2  %d %d\n", -1, -2 );
            TRICE32_3 (Id( 2072), "tst:TRICE32_3  %d %d %d\n", -1, -2, -3 );
            TRICE32_4 (Id( 1499), "tst:TRICE32_4  %d %d %d %d\n", -1, -2, -3, -4 );
            TRICE32_5 (Id( 4587), "tst:TRICE32_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
            TRICE32_6 (Id( 5830), "tst:TRICE32_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE32_7 (Id( 7732), "tst:TRICE32_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
            TRICE32_8 (Id( 4772), "tst:TRICE32_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 730:
            TRICE32_9 (Id( 7577), "tst:TRICE32_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE32_10(Id( 2414), "tst:TRICE32_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
            TRICE32_11(Id( 4996), "tst:TRICE32_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE32_12(Id( 7143), "tst:TRICE32_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
            TRICE32   (Id( 5141), "tst:TRICE32    %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));
            TRICE32_12(Id( 4629), "tst:TRICE32_12 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));
        break;
        case 740:
            TRICE( Id( 6289), "sig:TRICE64 with variable param count 1 to 12\n" );
            TRICE64(Id( 6544), "tst:TRICE64 %d\n", -1 );
            TRICE64(Id( 1969), "tst:TRICE64 %d %d\n", -1, -2 );
            TRICE64(Id( 7907), "tst:TRICE64 %d %d %d\n", -1, -2, -3 );
            TRICE64(Id( 7692), "tst:TRICE64 %d %d %d %d\n", -1, -2, -3, -4 );
            TRICE64(Id( 4822), "tst:TRICE64 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
            TRICE64(Id( 6175), "tst:TRICE64 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE64(Id( 7713), "tst:TRICE64 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
            TRICE64(Id( 7466), "tst:TRICE64 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 750:
            TRICE64(Id( 2004), "tst:TRICE64 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE64(Id( 7210), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
            TRICE64(Id( 1853), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE64(Id( 7598), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 760:
            TRICE64(Id( 3301), "tst:TRICE64 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aDouble(3.14159), aDouble(3.14159), aDouble(3.14159));
            TRICE64_1 (Id( 1177), "tst:TRICE64_1  %d\n", -1 );
            TRICE64_2 (Id( 1711), "tst:TRICE64_2  %d %d\n", -1, -2 );
            TRICE64_3 (Id( 5366), "tst:TRICE64_3  %d %d %d\n", -1, -2, -3 );
            TRICE64_4 (Id( 5684), "tst:TRICE64_4  %d %d %d %d\n", -1, -2, -3, -4 );
            TRICE64_5 (Id( 6946), "tst:TRICE64_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
            TRICE64_6 (Id( 3996), "tst:TRICE64_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE64_7 (Id( 3656), "tst:TRICE64_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
            TRICE64_8 (Id( 4247), "tst:TRICE64_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 770:
            TRICE64_9 (Id( 6178), "tst:TRICE64_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE64_10(Id( 6036), "tst:TRICE64_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
            TRICE64_11(Id( 7443), "tst:TRICE64_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE64_12(Id( 2992), "tst:TRICE64_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
            TRICE64_12(Id( 2967), "tst:TRICE64_12 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aDouble(3.14159), aDouble(3.14159), aDouble(3.14159));
        break;
        case 780:
            TRICE( Id( 7520), "att:positive and negative float in format variants\n");
            TRICE( Id( 4605), "rd:TRICE float %f (%%f)\n", aFloat(x) );
            TRICE( Id( 3606), "rd:TRICE float %9f (%%9f)\n", aFloat(x) );
            TRICE( Id( 7782), "rd:TRICE float %.9f (%%.9f)\n", aFloat(x) );
            TRICE( Id( 6833), "rd:TRICE float %9.f (%%9.f)\n", aFloat(x) );
            TRICE( Id( 6213), "rd:TRICE float %9.6f (%%9.6f)\n", aFloat(x) );
            TRICE( Id( 4432), "rd:TRICE float %f (%%f)\n", aFloat(-x) );
        break;
        case 790:
            TRICE( Id( 2812), "rd:TRICE float %9f (%%9f)\n", aFloat(-x) );
            TRICE( Id( 4295), "rd:TRICE float %.9f (%%.9f)\n", aFloat(-x) );
            TRICE( Id( 1538), "rd:TRICE float %9.f (%%9.f)\n", aFloat(-x) );
            TRICE( Id( 2220), "rd:TRICE float %9.6f (%%9.6f)\n", aFloat(-x) );
            TRICE( Id( 7886), "rd:TRICE float %+f (%%f)\n", aFloat(x) );
            TRICE( Id( 3683), "rd:TRICE float %+9f (%%9f)\n", aFloat(x) );
            TRICE( Id( 4633), "rd:TRICE float %+.9f (%%.9f)\n", aFloat(x) );
            TRICE( Id( 4261), "rd:TRICE float %+9.f (%%9.f)\n", aFloat(x) );
        break;
        case 800:
            TRICE( Id( 6915), "rd:TRICE float %+9.6f (%%9.6f)\n", aFloat(x) );
            TRICE( Id( 3701), "rd:TRICE float %+f (%%f)\n", aFloat(-x) );
            TRICE( Id( 2996), "rd:TRICE float %+9f (%%9f)\n", aFloat(-x) );
            TRICE( Id( 2370), "rd:TRICE float %+.9f (%%.9f)\n", aFloat(-x) );
            TRICE( Id( 5078), "rd:TRICE float %+9.f (%%9.f)\n", aFloat(-x) );
            TRICE( Id( 6084), "rd:TRICE float %+9.6f (%%9.6f)\n", aFloat(-x) );
            TRICE( Id( 7483), "rd:TRICE float %-f (%%f)\n", aFloat(x) );
            TRICE( Id( 4069), "rd:TRICE float %-9f (%%9f)\n", aFloat(x) );
        break;
        case 810:
            TRICE( Id( 3925), "rd:TRICE float %-.9f (%%.9f)\n", aFloat(x) );
            TRICE( Id( 5119), "rd:TRICE float %-9.f (%%9.f)\n", aFloat(x) );
            TRICE( Id( 5261), "rd:TRICE float %-9.6f (%%9.6f)\n", aFloat(x) );
            TRICE( Id( 6172), "rd:TRICE float %-f (%%f)\n", aFloat(-x) );
            TRICE( Id( 5851), "rd:TRICE float %-9f (%%9f)\n", aFloat(-x) );
            TRICE( Id( 1586), "rd:TRICE float %-.9f (%%.9f)\n", aFloat(-x) );
            TRICE( Id( 6313), "rd:TRICE float %-9.f (%%9.f)\n", aFloat(-x) );
            TRICE( Id( 7112), "rd:TRICE float %-9.6f (%%9.6f)\n", aFloat(-x) );
        break;
        case 820:
            TRICE( Id( 3392), "att:positive float & double in variants\n");
            TRICE32_1( Id( 1311), "rd:TRICE32_1 float %e (%%e)\n", aFloat(x) );
            TRICE32_1( Id( 2784), "rd:TRICE32_1 float %f (%%f)\n", aFloat(x) );
            TRICE32_1( Id( 5212), "rd:TRICE32_1 float %g (%%g)\n", aFloat(x) );
            TRICE32_1( Id( 4686), "rd:TRICE32_1 float %E (%%E)\n", aFloat(x) );
            TRICE32_1( Id( 7390), "rd:TRICE32_1 float %F (%%F)\n", aFloat(x) );
            TRICE32_1( Id( 2156), "rd:TRICE32_1 float %G (%%G)\n", aFloat(x) );
        break;
        case 830:
            TRICE32( Id( 2398), "rd:TRICE32 float %e (%%e)\n", aFloat(x) );
            TRICE32( Id( 5737), "rd:TRICE32 float %f (%%f)\n", aFloat(x) );
            TRICE32( Id( 7337), "rd:TRICE32 float %g (%%g)\n", aFloat(x) );
            TRICE32( Id( 1129), "rd:TRICE32 float %E (%%E)\n", aFloat(x) );
            TRICE32( Id( 6132), "rd:TRICE32 float %F (%%F)\n", aFloat(x) );
            TRICE32( Id( 3584), "rd:TRICE32 float %G (%%G)\n", aFloat(x) );
        break;
        case 840:
            TRICE( Id( 7902), "rd:TRICE float %e (%%e)\n", aFloat(x) );
            TRICE( Id( 6447), "rd:TRICE float %f (%%f)\n", aFloat(x) );
            TRICE( Id( 4186), "rd:TRICE float %g (%%g)\n", aFloat(x) );
            TRICE( Id( 1784), "rd:TRICE float %E (%%E)\n", aFloat(x) );
            TRICE( Id( 1151), "rd:TRICE float %F (%%F)\n", aFloat(x) );
            TRICE( Id( 7325), "rd:TRICE float %G (%%G)\n", aFloat(x) );
        break;
        case 850:
            TRICE64( Id( 7423), "rd:TRICE64 double %e (%%e), aDouble(y)\n", aDouble(y) );
            TRICE64( Id( 3911), "rd:TRICE64 double %f (%%f), aDouble(y)\n", aDouble(y) );
            TRICE64( Id( 2683), "rd:TRICE64 double %g (%%g), aDouble(y)\n", aDouble(y) );
            TRICE64( Id( 3795), "rd:TRICE64 double %E (%%E), aDouble(y)\n", aDouble(y) );
            TRICE64( Id( 2309), "rd:TRICE64 double %F (%%F), aDouble(y)\n", aDouble(y) );
            TRICE64( Id( 3402), "rd:TRICE64 double %G (%%G), aDouble(y)\n", aDouble(y) );
        break;
        case 860:
            TRICE64_1( Id( 2246), "rd:TRICE64_1 double %e (%%e), aDouble(y)\n", aDouble(y) );
            TRICE64_1( Id( 3502), "rd:TRICE64_1 double %f (%%f), aDouble(y)\n", aDouble(y) );
            TRICE64_1( Id( 3522), "rd:TRICE64_1 double %g (%%g), aDouble(y)\n", aDouble(y) );
            TRICE64_1( Id( 5373), "rd:TRICE64_1 double %E (%%E), aDouble(y)\n", aDouble(y) );
            TRICE64_1( Id( 6033), "rd:TRICE64_1 double %F (%%F), aDouble(y)\n", aDouble(y) );
            TRICE64_1( Id( 3952), "rd:TRICE64_1 double %G (%%G), aDouble(y)\n", aDouble(y) );
        break;
        case 870:
            TRICE( Id( 6569), "att:negative float & double\n" );
            TRICE( Id( 3801), "rd:TRICE float %e (%%e)\n", aFloat(-x) );
            TRICE( Id( 6756), "rd:TRICE float %f (%%f)\n", aFloat(-x) );
            TRICE( Id( 4536), "rd:TRICE float %g (%%g)\n", aFloat(-x) );
            TRICE( Id( 1770), "rd:TRICE float %E (%%E)\n", aFloat(-x) );
            TRICE( Id( 5290), "rd:TRICE float %F (%%F)\n", aFloat(-x) );
            TRICE( Id( 1890), "rd:TRICE float %G (%%G)\n", aFloat(-x) );
        break;
        case 880:
            TRICE64( Id( 5723), "rd:TRICE64 double %e (%%e), aDouble(y)\n", aDouble(-y) );
            TRICE64( Id( 7252), "rd:TRICE64 double %f (%%f), aDouble(y)\n", aDouble(-y) );
            TRICE64( Id( 4535), "rd:TRICE64 double %g (%%g), aDouble(y)\n", aDouble(-y) );
            TRICE64( Id( 5874), "rd:TRICE64 double %E (%%E), aDouble(y)\n", aDouble(-y) );
            TRICE64( Id( 1907), "rd:TRICE64 double %F (%%F), aDouble(y)\n", aDouble(-y) );
            TRICE64( Id( 5738), "rd:TRICE64 double %G (%%G), aDouble(y)\n", aDouble(-y) );
        break;
        case 890:
            TRICE( Id( 4366), "att:formatted float & double\n" );
            TRICE32( Id( 2658), "rd:TRICE32 float %12.6e (%%12.6e)\n", aFloat(x) );
            TRICE32( Id( 4865), "rd:TRICE32 float %12.6f (%%12.6f)\n", aFloat(x) );
            TRICE32( Id( 7560), "rd:TRICE32 float %12.6g (%%12.6g)\n", aFloat(x) );
            TRICE32( Id( 5902), "rd:TRICE32 float %12.6E (%%12.6E)\n", aFloat(x) );
            TRICE32( Id( 1164), "rd:TRICE32 float %12.6F (%%12.6F)\n", aFloat(x) );
            TRICE32( Id( 4932), "rd:TRICE32 float %12.6G (%%12.6G)\n", aFloat(x) );
        break;
        case 900:
            TRICE64( Id( 6113), "rd:TRICE64 double %12.6e (%%12.6e), aDouble(y)\n", aDouble(y) );
            TRICE64( Id( 5603), "rd:TRICE64 double %12.6f (%%12.6f), aDouble(y)\n", aDouble(y) );
            TRICE64( Id( 6598), "rd:TRICE64 double %12.6g (%%12.6g), aDouble(y)\n", aDouble(y) );
            TRICE64( Id( 3097), "rd:TRICE64 double %12.6E (%%12.6E), aDouble(y)\n", aDouble(y) );
            TRICE64( Id( 2154), "rd:TRICE64 double %12.6F (%%12.6F), aDouble(y)\n", aDouble(y) );
            TRICE64( Id( 4949), "rd:TRICE64 double %12.6G (%%12.6G), aDouble(y)\n", aDouble(y) );
        break;
        case 910:
            TRICE( Id( 7350), "att:mixed int & float & double & bits\n" );
            TRICE32( Id( 4362), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n", FloatToInt32(x), aFloat(x), aFloat(x), aFloat(x) );
            TRICE32( Id( 7353), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",    (int32_t)(x), aFloat(x), aFloat(x), aFloat(x) );
            TRICE32( Id( 2105), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",              x,  aFloat(x), aFloat(x), aFloat(x) );
            TRICE32( Id( 1841), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n", FloatToInt32(-x), aFloat(-x), aFloat(-x), aFloat(-x) );
            TRICE32( Id( 7146), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",    (int32_t)(-x), aFloat(-x), aFloat(-x), aFloat(-x) );
            TRICE32( Id( 7026), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",              -x,  aFloat(-x), aFloat(-x), aFloat(-x) );
        break;
        case 920:
            TRICE64( Id( 5593), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n", DoubleToInt64(y), aDouble(y), aDouble(y), aDouble(y) );
            TRICE64( Id( 4835), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",     (int64_t)(y), aDouble(y), aDouble(y), aDouble(y) );
            TRICE64( Id( 3987), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",               y,  aDouble(y), aDouble(y), aDouble(y) );
            TRICE64( Id( 5698), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n", DoubleToInt64(-y), aDouble(-y), aDouble(-y), aDouble(-y) );
            TRICE64( Id( 4801), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",     (int64_t)(-y), aDouble(-y), aDouble(-y), aDouble(-y) );
            TRICE64( Id( 3306), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",               -y,  aDouble(-y), aDouble(-y), aDouble(-y) );
        break;
        case 930:
            TRICE( Id( 6124), "att:Various single arguments\n" );
            TRICE8( Id( 1857), "rd:TRICE8 line %t (%%t ,0)\n", 0 );
            TRICE8( Id( 1495), "rd:TRICE8 line %t (%%t ,2)\n", 2 );
            TRICE8( Id( 4893), "rd:TRICE8 line %u (%%u ,-1)\n", -1 );
            TRICE8( Id( 4235), "rd:TRICE8 line %b (%%b ,-1)\n", -1 );
            TRICE8( Id( 7395), "rd:TRICE8 line %o (%%o ,-1)\n", -1 );
            TRICE8( Id( 7189), "rd:TRICE8 line %O (%%O ,-1)\n", -1 );
            TRICE8( Id( 6875), "rd:TRICE8 line %X (%%X ,-1)\n", -1 );
            TRICE8( Id( 7752), "rd:TRICE8 line %x (%%x ,-1)\n", -1 );
            TRICE8( Id( 1294), "rd:TRICE8 line %d (%%d ,-1)\n", -1 );
        break;
        case 940:
            TRICE16_1( Id( 1220), "rd:TRICE16_1 line %t (%%t ,0)\n", 0 );
            TRICE16_1( Id( 5802), "rd:TRICE16_1 line %t (%%t ,2)\n", 2 );
            TRICE16_1( Id( 6024), "rd:TRICE16_1 line %u (%%u -1)\n", -1 );
            TRICE16_1( Id( 4509), "rd:TRICE16_1 line %b (%%b -1)\n", -1 );
            TRICE16_1( Id( 4277), "rd:TRICE16_1 line %o (%%o -1)\n", -1 );
            TRICE16_1( Id( 2415), "rd:TRICE16_1 line %x (%%x -1)\n", -1 );
        break;
        case 950:
            TRICE16_1( Id( 5288), "rd:TRICE16_1 line %d (%%d -1)\n", -1 );
            TRICE( Id( 1057), "rd:TRICE line %t (%%t -1)\n", 0 );
            TRICE( Id( 5876), "rd:TRICE line %t (%%t -1)\n", 2 );
            TRICE( Id( 6159), "rd:TRICE line %u (%%u)\n", -1 );
            TRICE( Id( 4896), "rd:TRICE line %b (%%b)\n", -2 );
            TRICE( Id( 2606), "rd:TRICE line %o (%%o)\n", -3 );
            TRICE( Id( 7458), "rd:TRICE line %x (%%x)\n", -4 );
            TRICE( Id( 3963), "rd:TRICE line %d (%%d)\n", -5 );
        break;
        case 960:
        {
            s = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
            for( int i = 0; i < 17; i++ ){ //strlen(s) ){
                TRICE_N( Id( 4770), "msg:%s\n", s, i );
            }
            for( int i = 125; i < 130; i++ ){ //strlen(s) ){
                TRICE_N( Id( 2032), "msg:%s\n", s, i );
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
#ifdef XTEA_ENCRYPT_KEY
        TRICE0 (Id( 6463), "--------------------------------------------------\n" );
        {
            uint32_t by[8] = {1,2,3,4,5,6};
            int count = (sizeof(by) + 7) & ~7; // only multiple of 8 encryptable
            TRICE32(  Id( 7881), "msg: message = %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
            TRICE32( Id( 6649), "tim: pre TriceEncryption SysTick=%d\n", SYSTICKVAL );
            XTEAEncrypt(by, count>>2);
            TRICE32( Id( 6901), "tim: post TriceEncryption SysTick=%d\n", SYSTICKVAL );
            TRICE32(  Id( 7140), "att: TriceEncrypted =  %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
            #ifdef XTEA_DECRYPT
            TRICE16_1( Id( 4887), "tim: pre TriceDecryption SysTick=%d\n", SYSTICKVAL );
            XTEADecrypt(by, count>>2);
            TRICE16_1( Id( 6241), "tim: post TriceDecryption SysTick=%d\n", SYSTICKVAL );
            #endif
            TRICE32(  Id( 2186), "msg: messge = %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
        }
        TRICE0 (Id( 7158), "--------------------------------------------------\n" );
#endif
        break;
        default:
        break;
    }
}



//! SCOPY is a helper macro for struct serialization.
#define SCOPY( element ) do{ char* n = #element; int size = sizeof( src->element ); memcpy( p, &(src->element), size ); p += size; TRICE_S( Id( 5135), "rd:sizeof(%8s)", n ); TRICE( Id( 6144), " = %d\n", size);}while(0);

    //! DCOPY is a helper macro for struct deserialization.
#define DCOPY( element ) do{ char* n = #element; int size = sizeof( dst->element ); memcpy( &(dst->element), p, size ); p += size; TRICE_S( Id( 5152), "rd:sizeof(%8s)", n ); TRICE( Id( 3355), " = %d\n", size);}while(0);


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
    TRICE ( Id( 5410), "inf: Tryout tx struct:" );
    TRICE_B( Id( 6468), " %02x ", &tx, sizeof(tx) );
    TRICE( Id( 2822), "\n" );
    
    TRICE ( Id( 2706), "inf: Tryout buffer:" );
    TRICE_B( Id( 3396), " %02x ", dst, len ); //lint !e670
    TRICE( Id( 4774), "\n" );

    src = dst; // "data transfer"
    
    len = deserializeTryout( &rx, src );
    TRICE ( Id( 7187), "inf: Tryout rx struct:" );
    TRICE_B( Id( 6577), " %02x ", &rx, sizeof(rx) );
    TRICE( Id( 5383), "\n" );

    TRICE( Id( 5237), "inf:sizeOf(Tryout) = %d, buffer length = %d\n", sizeof(tx), len );
    TRICE8_F( Id( 6773), "info:TryoutStructFunction", &tx, sizeof(tx) );
    TRICE8_F( Id( 3543), "info:TryoutBufferFunction", dst, len ); //lint !e670
}

static void exampleOfManualJSONencoding(void){
    typedef  struct {
    int Apple, Birn;
    float Fish;
    } Ex_t;
    Ex_t Ex = { -1, 2, (float)2.781 };
    TRICE( Id( 7582), "att:MyStructEvaluationFunction(json:ExA{Apple:%d, Birn:%u, Fisch:%f}\n", Ex.Apple, Ex.Birn, aFloat(Ex.Fish) );
}

static void exampleOfBuffersAndFunctions(void){
    static int8_t   b8[24] = { 0, -1, -2, 0x33, 4, 5, 6, 7, 8, 9, 10, 11, 0, -1, -2, 0x33, 4, 5, 6, 7, 8, 9, 10, 11 };
    static int16_t b16[] = { 0, -1, -2, 0x3344 };
    static int32_t b32[] = { 0, -1, -2, 0x33445555};
    static int64_t b64[4] = { 0, -1, -2, 0x3344555566666666 };
    char* s = "abcde 12345";
    uint32_t len;

    TRICE_S( Id( 1436), "msg:With TRICE_S:%s\n", s );
    len = strlen(s);
    TRICE_N( Id( 3769), "sig:With TRICE_N:%s\n", s, len );
    TRICE32( Id( 6888), "att:len=%u:With TRICE_B:\n", len);
    TRICE_B( Id( 1587), "  %02x", s, len );
    TRICE( Id( 3130), "\n" );
    TRICE_B( Id( 6145), "%4d", s, len );
    TRICE( Id( 7051), "\n" );

    TRICE( Id( 7214), "notice:TRICE_B example: " );
    TRICE_B( Id( 1238), "  %02x", b8, sizeof(b8)/sizeof(int8_t) );
    TRICE( Id( 6623), "\n" );
    TRICE( Id( 3695), "notice:TRICE8_B example:" );
    TRICE8_B( Id( 5469), "  %02x", b8, sizeof(b8)/sizeof(int8_t) );
    TRICE( Id( 2454), "\n" );
    TRICE( Id( 4315), "notice:TRICE16_B example:" );
    TRICE16_B( Id( 7019), "  %04x", b16, sizeof(b16)/sizeof(int16_t) );
    TRICE( Id( 6717), "\n" );
    TRICE( Id( 5766), "notice:TRICE32_B example:" );
    TRICE32_B( Id( 7870), "  %08x", b32, sizeof(b32)/sizeof(int32_t) );
    TRICE( Id( 5095), "\n" );
    TRICE( Id( 5782), "notice:TRICE64_B example:" );
    TRICE64_B( Id( 1951), "  %016x", b64, sizeof(b64)/sizeof(int64_t) );
    TRICE( Id( 7721), "\n" );    

    TRICE( Id( 6381), "notice:TRICE_F example: " );
    TRICE_F( Id( 2844), "info:FunctionNameW",   b8,  sizeof(b8) /sizeof(int8_t) );
    //shown as: TRICE_F example: FunctionNameW(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)
    
    TRICE( Id( 3434), "notice:TRICE8_F example:" );
    TRICE8_F(  Id( 6950), "info:FunctionNameW",   b8,  sizeof(b8) /sizeof(int8_t) );
    //shown as: TRICE8_F example:FunctionNameW(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)
    
    TRICE( Id( 4491), "notice:TRICE16_F example:" );
    TRICE16_F( Id( 3620), "sig:FunctionNameX",    b16, sizeof(b16)/sizeof(int16_t) );
    //shown as: TRICE16_F example:FunctionNameX(0000)(ffff)(fffe)(3344) 

    TRICE( Id( 6999), "notice:TRICE32_F example:" );
    TRICE32_F( Id( 2711), "diag:FunctionNameY",   b32, sizeof(b32)/sizeof(int32_t) );
    //shown as: TRICE32_F example:FunctionNameY(00000000)(ffffffff)(fffffffe)(33445555)

    TRICE( Id( 7286), "notice:TRICE64_F example:" );
    TRICE64_F( Id( 4647), "fatal:FunctionNameZ", b64, sizeof(b64)/sizeof(int64_t) );
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
