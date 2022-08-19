/*! \file triceCheck.c
\brief trices for tool evaluation
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/
#include <float.h>
//#define TRICE_OFF // enable this line to disable trice code generation in this file object
#include "trice.h"
#define TRICE_FILE Id(13388)

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
            TRICE( Id(14832), "FATAL:magenta+b:red\n" );
            TRICE( Id(12971), "CRITICAL:red+i:default+h\n" );
            TRICE( Id(13503), "EMERGENCY:red+i:blue\n" );
            TRICE( Id(15097), "ERROR:11:red\n" );
            TRICE( Id(10921), "WARNING:11+i:red\n" );
            TRICE( Id(14131), "ATTENTION:11:green\n" );
            TRICE( Id(10492), "INFO:cyan+b:default+h\n" );
            TRICE( Id(11578), "DEBUG:130+i\n" );
            TRICE( Id(12702), "TRACE:default+i:default+h\n" );
            TRICE( Id(14980), "TIME:blue+i:blue+h\n" );
            TRICE( Id(10324), "MESSAGE:green+h:black\n" );
            TRICE( Id(14972), "READ:black+i:yellow+h\n" );
            TRICE( Id(14310), "WRITE:black+u:yellow+h\n" );
            TRICE( Id(12104), "RECEIVE:black+h:black\n" );
            TRICE( Id(13522), "TRANSMIT:black:black+h\n" );
            TRICE( Id(11874), "DIAG:yellow+i:default+h\n" );
            TRICE( Id(15902), "INTERRUPT:magenta+i:default+h\n" );
            TRICE( Id(10810), "SIGNAL:118+i\n" );
            TRICE( Id(10154), "TEST:yellow+h:black\n" );
            TRICE( Id(15113), "DEFAULT:off\n" );
            TRICE( Id(13170), "NOTICE:blue:white+h\n" );
            TRICE( Id(11424), "ALERT:magenta:magenta+h\n" );
            TRICE( Id(15617), "ASSERT:yellow+i:blue\n" );
            TRICE( Id(15431), "ALARM:red+i:white+h\n" );
            TRICE( Id(11534), "CYCLE:blue+i:default+h\n" );
            TRICE( Id(14527), "VERBOSE:blue:default\n" );
        break;
        case 20:
            TRICE( Id(14068), "fatal:magenta+b:red\n" );
            TRICE( Id(12622), "critical:red+i:default+h\n" );
            TRICE( Id(10531), "emergency:red+i:blue\n" );
            TRICE( Id(15640), "error:11:red\n" );
            TRICE( Id(13209), "warning:11+i:red\n" );
            TRICE( Id(11370), "attention:11:green\n" );
            TRICE( Id(12222), "info:cyan+b:default+h\n" );
            TRICE( Id(11134), "debug:130+i\n" );
            TRICE( Id(13610), "trace:default+i:default+h\n" );
            TRICE( Id(12139), "tme:blue+i:blue+h\n" );
            TRICE( Id(14964), "message:green+h:black\n" );
            TRICE( Id(14805), "read:black+i:yellow+h\n" );
            TRICE( Id(11282), "write:black+u:yellow+h\n" );
            TRICE( Id(12821), "receive:black+h:black\n" );
            TRICE( Id(13598), "transmit:black:black+h\n" );
            TRICE( Id(15837), "diag:yellow+i:default+h\n" );
            TRICE( Id(12730), "interrupt:magenta+i:default+h\n" );
            TRICE( Id(15329), "signal:118+i\n" );
            TRICE( Id(12488), "test:yellow+h:black\n" );
            TRICE( Id(13696), "default:off\n" );
            TRICE( Id(13238), "notice:blue:white+h\n" );
            TRICE( Id(13859), "allert:magenta:magenta+h\n" );
            TRICE( Id(15540), "assert:yellow+i:blue\n" );
            TRICE( Id(11674), "alarm:red+i:white+h\n" );
            TRICE( Id(10086), "cycle:blue+i:default+h\n" );
            TRICE( Id(13529), "verbose:blue:default\n" );
        break;
        case 30:
            TRICE( Id(14048), "dbg:Hi!\n" );
            TRICE( Id(11199), "dbg:\tHi!\n" );
            TRICE( Id(14001), "dbg:\t\tHi!\n" );
            TRICE( Id(14807), "dbg:\t\t\tHi!\n" );
            TRICE( Id(10715), "dbg:\\tHi!\n" );
            TRICE( Id(13636), "dbg:\aHi!\n" );
            TRICE( Id(15285), "dbg:\a\aHi!\n" );
            TRICE( Id(12774), "dbg:\a\a\aHi!\n" );
            TRICE( Id(13747), "dbg:\\aHi!\n" );
            //TCOBSCheck();
        break;
        case 40:{
            int len = strlen(s);
            TRICE32( Id(12283), "dbg:len=%u:", len );
            TRICE_S( Id(12176), "sig:TRICE_S=%s\n", s );
            TRICE32( Id(14515), "dbg:len=%u:", len );
            TRICE_S( Id(14308), "sig:TRICE_S=%s\n", s );
            TRICE32( Id(15071), "dbg:len=%u:", len );
            TRICE_S( Id(15891), "sig:TRICE_S=%s\n", s );
        
            TRICE32( Id(15180), "dbg:len=%u:", len);
            TRICE_N( Id(12058), "sig:TRICE_N=%s\n", s, len );
            TRICE32( Id(11777), "dbg:len=%u:", len);
            TRICE_N( Id(14082), "sig:TRICE_N=%s\n", s, len );
            TRICE32( Id(12888), "dbg:len=%u:", len);
            TRICE_N( Id(10783), "sig:TRICE_N=%s\n", s, len );
        
          //TRICE_S( Id(13268), "sig:TRICE_S=%s\n", "Hello String" );     // not allowed
          //TRICE_N( Id(13084), "sig:TRICE_N=%s\n", "Hello Buffer", 13 ); // not allowed

            TRICE( Id(10125), "rd:TRICE line %E (%%E)\n", aFloat(-555555555.5555555555) );
            TRICE( Id(14736), "rd:TRICE line %F (%%F)\n", aFloat(-555555555.5555555555) );
            TRICE( Id(10727), "rd:TRICE line %G (%%G)\n", aFloat(-555555555.5555555555) );
        }
        break;
        case 50:
            TRICE32_1( Id(10573), "rd:TRICE32_1 line %u (%%u)\n", -1 );
            TRICE32_1( Id(11235), "rd:TRICE32_1 line %b (%%b)\n", -1 );
            TRICE32_1( Id(11726), "rd:TRICE32_1 line %o (%%o)\n", -1 );
            TRICE32_1( Id(12618), "rd:TRICE32_1 line %O (%%O)\n", -1 );
            TRICE32_1( Id(13929), "rd:TRICE32_1 line %X (%%X)\n", -1 );
            TRICE32_1( Id(11123), "rd:TRICE32_1 line %x (%%x)\n", -1 );
            TRICE32_1( Id(14052), "rd:TRICE32_1 line %d (%%d)\n", -1 );
        break;
        case 60:
            TRICE64_1( Id(13685), "rd:TRICE64_1 line %u (%%u)\n", -1 );
            TRICE64_1( Id(12328), "rd:TRICE64_1 line %b (%%b)\n", -1 );
            TRICE64_1( Id(13150), "rd:TRICE64_1 line %o (%%o)\n", -1 );
            TRICE64_1( Id(15200), "rd:TRICE64_1 line %O (%%O)\n", -1 );
            TRICE64_1( Id(11970), "rd:TRICE64_1 line %X (%%C)\n", -1 );
            TRICE64_1( Id(13267), "rd:TRICE64_1 line %x (%%x)\n", -1 );
            TRICE64_1( Id(14947), "rd:TRICE64_1 line %d (%%d)\n", -1 );
        break;
        case 70:
            TRICE64( Id(11984), "rd:TRICE64 line %E (%%E)\n", aDouble(-555555555.5555555555) );
            TRICE64( Id(11177), "rd:TRICE64 line %F (%%F)\n", aDouble(-555555555.5555555555) );
            TRICE64( Id(15832), "rd:TRICE64 line %G (%%G)\n", aDouble(-555555555.5555555555) );
        break;
        case 80:
            TRICE( Id(14710), "sig:Some time measurements\n" );
            TRICE( Id(15367), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
            TRICE( Id(14264), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
            TRICE( Id(15869), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
            TRICE( Id(13563), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
            TRICE64( Id(11550), "rd:TRICE64 %d, %d\n", 1, 2 );
        break;
        case 90:
            TRICE( Id(10893), "tim:TRICE START time message\n");
            TRICE( Id(15824), "tim:TRICE STOP time message\n");
            TRICE( Id(14098), "tim:TRICE START time message\n");
            TRICE( Id(11350), "tim:TRICE STOP time message\n");
            TRICE( Id(12433), "tim:TRICE START time message\n");
            TRICE64( Id(14429), "rd:TRICE64 %d, %d\n", 1, 2 );
            TRICE( Id(11851), "tim:TRICE STOP time message\n");
        break;
        case 100:
            TRICE( Id(15209), "tim:TRICE --------------------------------------------------\n");
            TRICE( Id(12973), "tim:TRICE --------------------------------------------------\n");
            TRICE( Id(12478), "tim:TRICE --------------------------------------------------\n");
            TRICE( Id(13151), "tim:TRICE --------------------------------------------------\n");
            TRICE( Id(11612), "tim:TRICE --------------------------------------------------\n");
            TRICE( Id(11862), "tim:TRICE --------------------------------------------------\n");
            TRICE64( Id(15087), "rd:TRICE64 %d, %d\n", 1, 2 );
        break;
        case 110:
            TRICE( Id(12625), "tim:TRICE --------------------------------------------------\n");
            TRICE( Id(12324), "tim:TRICE --------------------------------------------------\n");
            TRICE( Id(15054), "tim:TRICE --------------------------------------------------\n");
            TRICE( Id(12188), "tim:TRICE --------------------------------------------------\n");
            TRICE( Id(15472), "tim:TRICE --------------------------------------------------\n");
            TRICE( Id(14806), "tim:TRICE --------------------------------------------------\n");
            TRICE( Id(10211), "tim:TRICE --------------------------------------------------\n");
            TRICE64( Id(13827), "rd:TRICE64 %d, %d\n", 1, 2 );
        break;
        case 120:
            TRICE( Id(15743), "tim:TRICE --------------------------------------------------\n");
            TRICE( Id(13768), "tim:TRICE --------------------------------------------------\n");
            TRICE( Id(14053), "tim:TRICE --------------------------------------------------\n");
            TRICE( Id(10538),  "rd:TRICE32_1 line %d\n", __LINE__ );
            TRICE64_1( Id(10584),  "rd:TRICE64_1 %d\n", __LINE__ );
            TRICE32_2( Id(10696),  "rd:TRICE32_2 line %d,%d\n", __LINE__, 2 );
            TRICE64_2( Id(10107),  "rd:TRICE64_2 line %d,%d\n", __LINE__, 2 );
        break;
        case 130:
            TRICE( Id(10034), "sig:TRICE with 1 to 12 values\n" );
            TRICE( Id(15821),"rd:TRICE %d\n", -1 );
            TRICE( Id(10225),"rd:TRICE %d, %d\n", -1, -2 );
            TRICE( Id(13665),"rd:TRICE %d, %d, %d\n", -1, -2, -3 );
            TRICE( Id(12255),"rd:TRICE %d, %d, %d, %d\n", -1, -2, -3, -4 );
            TRICE( Id(12595),"rd:TRICE %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
            TRICE( Id(14826),"rd:TRICE %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE( Id(15972),"rd:TRICE %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
            TRICE( Id(11765),"rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 140:
            TRICE( Id(13041),"rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE( Id(15938),"rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
            TRICE( Id(15815),"rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE( Id(15933),"rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 150:
            TRICE( Id(13544), "sig:TRICE_n with 1 to 12 values\n" );
            TRICE_1( Id(14820),   "rd:TRICE_1 %d\n", 1 );
            TRICE_2( Id(12994),   "rd:TRICE_2 %d, %d\n", 1, 2 );
            TRICE_3( Id(11295),   "rd:TRICE_3 %d, %d, %d\n", 1, 2, 3 );
            TRICE_4( Id(14164),   "rd:TRICE_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
            TRICE_5( Id(14699),   "rd:TRICE_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
            TRICE_6( Id(12479),   "rd:TRICE_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
            TRICE_7( Id(10138),   "rd:TRICE_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
            TRICE_8( Id(15502),   "rd:TRICE_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
        break;
        case 160:
            TRICE_9( Id(10239),   "rd:TRICE_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
            TRICE_10( Id(12136),  "rd:TRICE_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
            TRICE_11( Id(15676),  "rd:TRICE_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
            TRICE_12( Id(10032),  "rd:TRICE_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );				
        break;
        case 170:
            TRICE( Id(11646), "sig:TRICE8 with 1 to 12 values\n" );
            TRICE8( Id(12003),"rd:TRICE8 %d\n", -1 );
            TRICE8( Id(15723),"rd:TRICE8 %d, %d\n", -1, -2 );
            TRICE8( Id(15452),"rd:TRICE8 %d, %d, %d\n", -1, -2, -3 );
            TRICE8( Id(13771),"rd:TRICE8 %d, %d, %d, %d\n", -1, -2, -3, -4 );
            TRICE8( Id(14101),"rd:TRICE8 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
            TRICE8( Id(12653),"rd:TRICE8 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE8( Id(15432),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
            TRICE8( Id(13058),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 180:
            TRICE8( Id(12470),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE8( Id(14660),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
            TRICE8( Id(15603),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE8( Id(12648),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 190:
            TRICE( Id(12856), "sig:TRICE8_n with 1 to 12 values\n" );
            TRICE8_1( Id(11041),   "rd:TRICE8_1 %d\n", 1 );
            TRICE8_2( Id(11389),   "rd:TRICE8_2 %d, %d\n", 1, 2 );
            TRICE8_3( Id(10824),   "rd:TRICE8_3 %d, %d, %d\n", 1, 2, 3 );
            TRICE8_4( Id(13720),   "rd:TRICE8_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
            TRICE8_5( Id(13481),   "rd:TRICE8_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
            TRICE8_6( Id(15506),   "rd:TRICE8_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
            TRICE8_7( Id(14330),   "rd:TRICE8_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
        break;
        case 200:
            TRICE8_8( Id(10683),   "rd:TRICE8_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
            TRICE8_9( Id(11192),   "rd:TRICE8_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
            TRICE8_10( Id(13371),  "rd:TRICE8_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
            TRICE8_11( Id(10183),  "rd:TRICE8_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
            TRICE8_12( Id(10672),  "rd:TRICE8_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );
        break;
        case 210:
            TRICE( Id(12842), "sig:TRICE16 with 1 to 12 values\n" );
            TRICE16( Id(14848),"rd:TRICE16 %d\n", -1 );
            TRICE16( Id(12944),"rd:TRICE16 %d, %d\n", -1, -2 );
            TRICE16( Id(15967),"rd:TRICE16 %d, %d, %d\n", -1, -2, -3 );
            TRICE16( Id(12163),"rd:TRICE16 %d, %d, %d, %d\n", -1, -2, -3, -4 );
            TRICE16( Id(11580),"rd:TRICE16 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
            TRICE16( Id(11502),"rd:TRICE16 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE16( Id(12819),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
            TRICE16( Id(15148),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 220:
            TRICE16( Id(13470),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE16( Id(13726),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
            TRICE16( Id(15116),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE16( Id(10800),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 230:
            TRICE( Id(11226), "sig:TRICE16_n with 1 to 12 values\n" );
            TRICE16_1( Id(14487),  "rd:TRICE16_1 %d\n", 1 );
            TRICE16_2( Id(13152),  "rd:TRICE16_2 %d, %d\n", 1, 2 );
            TRICE16_3( Id(14512),  "rd:TRICE16_3 %d, %d, %d\n", 1, 2, 3 );
            TRICE16_4( Id(12623),  "rd:TRICE16_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
            TRICE16_5( Id(14864),  "rd:TRICE16_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
            TRICE16_6( Id(15629),  "rd:TRICE16_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
            TRICE16_7( Id(15755),  "rd:TRICE16_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
            TRICE16_8( Id(14360),  "rd:TRICE16_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
        break;
        case 240:
            TRICE16_9( Id(14073),  "rd:TRICE16_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
            TRICE16_10( Id(10704), "rd:TRICE16_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
            TRICE16_11( Id(10811), "rd:TRICE16_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
            TRICE16_12( Id(13183), "rd:TRICE16_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );
        break;
        case 250:
            TRICE( Id(10289), "sig:TRICE16 with 1 to 12 pointer\n" );
            TRICE16( Id(10706),"rd:TRICE16 %p\n", -1 );
            TRICE16( Id(10649),"rd:TRICE16 %p, %p\n", -1, -2 );
            TRICE16( Id(14767),"rd:TRICE16 %p, %p, %p\n", -1, -2, -3 );
            TRICE16( Id(11732),"rd:TRICE16 %p, %p, %p, %p\n", -1, -2, -3, -4 );
            TRICE16( Id(12637),"rd:TRICE16 %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5 );
            TRICE16( Id(10679),"rd:TRICE16 %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6 );
            TRICE16( Id(13410),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7);
            TRICE16( Id(12332),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 260:
            TRICE16( Id(14037),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE16( Id(12330),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
            TRICE16( Id(14781),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE16( Id(10998),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 270:
            TRICE( Id(14607), "sig:TRICE16 with 1 to 12 hex\n" );
            TRICE16( Id(13452),"rd:TRICE16 %X\n", -1 );
            TRICE16( Id(12945),"rd:TRICE16 %X, %X\n", -1, -2 );
            TRICE16( Id(14006),"rd:TRICE16 %X, %X, %X\n", -1, -2, -3 );
            TRICE16( Id(15182),"rd:TRICE16 %X, %X, %X, %X\n", -1, -2, -3, -4 );
            TRICE16( Id(12446),"rd:TRICE16 %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5 );
            TRICE16( Id(13622),"rd:TRICE16 %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6 );
            TRICE16( Id(11420),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7);
            TRICE16( Id(12079),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 280:
            TRICE16( Id(15366),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE16( Id(15128),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
            TRICE16( Id(15152),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE16( Id(13866),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 290:
            TRICE( Id(12021), "sig:TRICE32 with 1 to 12 values\n" );
            TRICE32( Id(13820),"rd:TRICE32 %d\n", -1 );
            TRICE32( Id(10347),"rd:TRICE32 %d, %d\n", -1, -2 );
            TRICE32( Id(13940),"rd:TRICE32 %d, %d, %d\n", -1, -2, -3 );
            TRICE32( Id(13743),"rd:TRICE32 %d, %d, %d, %d\n", -1, -2, -3, -4 );
            TRICE32( Id(12669),"rd:TRICE32 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
            TRICE32( Id(14244),"rd:TRICE32 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE32( Id(14929),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
            TRICE32( Id(13572),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 300:
            TRICE32( Id(11034),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE32( Id(11887),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
            TRICE32( Id(15157),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE32( Id(11643),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 310:
            TRICE( Id(14697), "signal:TRICE32_n with 1 to 12 values\n" );
            TRICE32_1( Id(12970),  "rd:TRICE32_1 %d\n", 1 );
            TRICE32_2( Id(12203),  "rd:TRICE32_2 %d, %d\n", 1, 2 );
            TRICE32_3( Id(10402),  "rd:TRICE32_3 %d, %d, %d\n", 1, 2, 3 );
            TRICE32_4( Id(12509),  "rd:TRICE32_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
            TRICE32_5( Id(10697),  "rd:TRICE32_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
            TRICE32_6( Id(14995),  "rd:TRICE32_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
            TRICE32_7( Id(10961),  "rd:TRICE32_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
            TRICE32_8( Id(13931),  "rd:TRICE32_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
        break;
        case 320:
            TRICE32_9( Id(10353),  "rd:TRICE32_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
            TRICE32_10( Id(14377), "rd:TRICE32_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
            TRICE32_11( Id(15836), "rd:TRICE32_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
            TRICE32_12( Id(12428), "rd:TRICE32_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );
        break;
        case 330:
            TRICE( Id(12663), "sig:TRICE64 with 1 to 12 values\n" );
            TRICE64( Id(15513),"rd:TRICE64 %d\n", -1 );
            TRICE64( Id(13434),"rd:TRICE64 %d, %d\n", -1, -2 );
            TRICE64( Id(11848),"rd:TRICE64 %d, %d, %d\n", -1, -2, -3 );
            TRICE64( Id(10504),"rd:TRICE64 %d, %d, %d, %d\n", -1, -2, -3, -4 );
            TRICE64( Id(13596),"rd:TRICE64 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
            TRICE64( Id(15428),"rd:TRICE64 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE64( Id(15352),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
            TRICE64( Id(15504),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 340:
            TRICE64( Id(11911),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE64( Id(13998),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
            TRICE64( Id(14448),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE64( Id(10123),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 350:
            TRICE( Id(15187), "sig:TRICE64_n with 1 to 12 values\n" );
            TRICE64_1( Id(12767),  "rd:TRICE64_1 %d\n", 1 );
            TRICE64_2( Id(15289),  "rd:TRICE64_2 %d, %d\n", 1, 2 );
            TRICE64_3( Id(13498),  "rd:TRICE64_3 %d, %d, %d\n", 1, 2, 3 );
            TRICE64_4( Id(13536),  "rd:TRICE64_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
            TRICE64_5( Id(12306),  "rd:TRICE64_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
            TRICE64_6( Id(15204),  "rd:TRICE64_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
            TRICE64_7( Id(12586),  "rd:TRICE64_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
            TRICE64_8( Id(12872),  "rd:TRICE64_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
        break;
        case 360:
            TRICE64_9( Id(10023),  "rd:TRICE64_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
            TRICE64_10( Id(10393), "rd:TRICE64_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
            TRICE64_11( Id(10084), "rd:TRICE64_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
            TRICE64_12( Id(12389), "rd:TRICE64_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );
        break;
        case 370:
            TRICE( Id(11207), "sig:Colors and ticks\n" );
            TRICE(Id(14133), "--------------------------------------------------\n");
            TRICE(Id(14886), "--------------------------------------------------\n");
            TRICE(Id(15925), "dbg:12345 as 16bit is %016b\n", 12345);
            TRICE(Id(10077), "--------------------------------------------------\n");
            TRICE(Id(13558), "sig:This ASSERT error is just a demo and no real error:\n");
            TRICE(Id(11371), "--------------------------------------------------\n");
        break;
        case 380:
            TRICE(Id(15534), "ERR:error       message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id(13519), "WRN:warning     message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id(10654), "ATT:attention   message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id(15568), "DIA:diagnostics message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id(11956), "TIM:timing      message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id(10954), "DBG:debug       message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id(12461), "SIG:signal      message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id(14668), "RD:read         message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id(11026), "WR:write        message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id(12533), "ISR:interrupt   message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id(15276), "MSG:normal      message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id(10891), "INFO:informal   message, SysTick is %6u\n", SYSTICKVAL);
            TRICE(Id(14923), "time:TRICE32_1   message, SysTick is %6u\n", SYSTICKVAL);
        break;
        case 390:
            TRICE( Id(14785), "sig:Various ranges and formats\n" );
            TRICE8( Id(11431), "tst:TRICE8  %%03x -> %03x  %03x  %03x  %03x\n", 1, 0x7f, 0x80, 0xff);
            TRICE8( Id(15748), "tst:TRICE8   %%4d -> %4d %4d %4d %4d\n", 1, 0x7f, 0x80, 0xff);
            TRICE8( Id(13646), "tst:TRICE8   %%4u -> %4u %4u %4u %4u\n", 1, 0x7f, 0x80, 0xff);
            TRICE8( Id(13593), "tst:TRICE8   %%4o -> %4o %4o %4o %4o\n", 1, 0x7f, 0x80, 0xff);
            TRICE8( Id(14125), "tst:TRICE8   %%4O -> %4O %4O %4O %4O\n", 1, 0x7f, 0x80, 0xff);
            TRICE8( Id(12561), "tst:TRICE8   %%4X -> %4X %4X %4X %4X\n", 1, 0x7f, 0x80, 0xff);
            TRICE8( Id(14036), "tst:TRICE8   %%4x -> %4x %4x %4x %4x\n", 1, 0x7f, 0x80, 0xff);
            TRICE8( Id(15792), "tst:TRICE8   %%8b -> %8b %8b %8b %8b\n", 1, 0x7f, 0x80, 0xff);
            TRICE8( Id(10461), "tst:TRICE8  %%08b -> %08b %08b %08b %08b\n", 1, 0x7f, 0x80, 0xff);
        break;
        case 400:
            TRICE16(Id(14408), "tst:TRICE16  %%05x ->   %05x   %05x   %05x   %05x\n", 1, 0x7fff, 0x8000, 0xffff);
            TRICE16(Id(11497), "tst:TRICE16   %%6d ->  %6d  %6d  %6d  %6d\n",         1, 0x7fff, 0x8000, 0xffff);
            TRICE16(Id(14166), "tst:TRICE16   %%7o -> %7o %7o %7o %7o\n",             1, 0x7fff, 0x8000, 0xffff);

            TRICE32(Id(13282), "tst:TRICE32 %%09x ->      %09x      %09x       %09x     %09x\n", 1, 0x7fffffff, 0x80000000, 0xffffffff);
            TRICE32(Id(10231), "tst:TRICE32 %%10d ->     %10d     %10d     %10d    %10x\n",      1, 0x7fffffff, 0x80000000, 0xffffffff);
            TRICE32(Id(12628), "att:TRICE32 %40b\n", 0xAAAAAAAA);
        break;
        case 410:
            TRICE64(Id(15492), "tst:TRICE64 %%09x ->      %09x      %09x       %09x     %09x\n", 1, 0x7fffffffffffffff, 0x8000000000000000, 0xffffffffffffffff);
            TRICE64(Id(11986), "tst:TRICE64 %%10d ->     %10d     %10d     %10d    %10x\n",      1, 0x7fffffffffffffff, 0x8000000000000000, 0xffffffffffffffff);
            TRICE64(Id(11009), "att:TRICE64 %70b\n", 0xAAAAAAAAAAAAAAAA);
        break;
        case 420:
            TRICE( Id(15679), "sig:Legacy TRICE8\n" );
            TRICE8_1(Id(13605), "tst:TRICE8_1 %02x\n", 0xA1);
            TRICE8_2(Id(10949), "tst:TRICE8_2 %02x %02x\n", 0xA1, 0xA2);
            TRICE8_3(Id(11746), "tst:TRICE8_3 %02x %02x %02x\n", 0xA1, 0xA2, 0xA3);
            TRICE8_4(Id(14135), "tst:TRICE8_4 %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4);
            TRICE8_5(Id(10166), "tst:TRICE8_5 %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5 );
            TRICE8_6(Id(15450), "tst:TRICE8_6 %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6);
            TRICE8_7(Id(11074), "tst:TRICE8_7 %02x %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7);
            TRICE8_8(Id(11099), "tst:TRICE8_8 %02x %02x %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8);
        break;
        case 430:
            TRICE8_1(Id(15244), "tst:TRICE8_1 %d\n",                      -1);
            TRICE8_2(Id(12155), "tst:TRICE8_2 %d %d\n",                   -1, -2);
            TRICE8_3(Id(15840), "tst:TRICE8_3 %d %d %d\n",                -1, -2, -3);
            TRICE8_4(Id(14243), "tst:TRICE8_4 %d %d %d %d\n",             -1, -2, -3, -4);
            TRICE8_5(Id(14413), "tst:TRICE8_5 %d %d %d %d %d\n",          -1, -2, -3, -4, -5);
            TRICE8_6(Id(11901), "tst:TRICE8_6 %d %d %d %d %d %d\n",       -1, -2, -3, -4, -5, -6);
            TRICE8_7(Id(12257), "tst:TRICE8_7 %d %d %d %d %d %d %d\n",    -1, -2, -3, -4, -5, -6, -7);
            TRICE8_8(Id(14722), "tst:TRICE8_8 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8);
        break;
        case 440:
            TRICE8_1(Id(14654), "tst:TRICE8_1 %d\n", 1);
            TRICE8_2(Id(13789), "tst:TRICE8_2 %d %d\n", 1, 2);
            TRICE8_3(Id(14061), "tst:TRICE8_3 %d %d %d\n", 1, 2, 3);
            TRICE8_4(Id(14624), "tst:TRICE8_4 %d %d %d %d\n", 1, 2, 3, 4);
            TRICE8_5(Id(10594), "tst:TRICE8_5 %d %d %d %d %d\n", 1, 2, 3, 4, 5);
            TRICE8_6(Id(14943), "tst:TRICE8_6 %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6);
            TRICE8_7(Id(12234), "tst:TRICE8_7 %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7);
            TRICE8_8(Id(15063), "tst:TRICE8_8 %d %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7, 8);
        break;
        case 450:
            TRICE8_1(Id(13873), "tst:TRICE8_1 %u\n", 201);
            TRICE8_2(Id(10902), "tst:TRICE8_2 %u %u\n", 201, 202);
            TRICE8_3(Id(12749), "tst:TRICE8_3 %u %u %u\n", 201, 202, 203);
            TRICE8_4(Id(15184), "tst:TRICE8_4 %u %u %u %u\n", 201, 202, 203, 204);
            TRICE8_5(Id(15239), "tst:TRICE8_5 %u %u %u %u %u\n", 201, 202, 203, 204, 205);
            TRICE8_6(Id(12759), "tst:TRICE8_6 %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206);
            TRICE8_7(Id(12022), "tst:TRICE8_7 %u %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206, 207);
            TRICE8_8(Id(15510), "tst:TRICE8_8 %u %u %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206, 207, 208);
            TRICE8_8(Id(10219), "tst:TRICE8_1 %%d=%d, %%u=%u, 0x%%x=0x%x, 0x%%2x=0x%2x, 0x%%02x=0x%02x, 0x%%3x=0x%3x, 0x%%03x=0x%03x, %%b=%b\n", 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81);
        break;
        case 460:
            TRICE( Id(15172), "sig:Legacy TRICE16-64\n" );
            TRICE16_1(Id(15597), "tst:TRICE16_1 %d\n", -111);
            TRICE16_2(Id(13662), "tst:TRICE16_2 %d %d\n", -111, -222);
            TRICE16_3(Id(13324), "tst:TRICE16_3 %d %d %d\n", -111, -222, -333);
            TRICE16_4(Id(15554), "tst:TRICE16_4 %d %d %d %d\n", -111, -222, -333, -444);
            TRICE32_1(Id(14847), "tst:TRICE32_1 %08x\n", 0x0123cafe);
            TRICE32_1(Id(14719), "tst:TRICE32_1 %d\n", -111);
            TRICE32_2(Id(15190), "tst:TRICE32_2 %x %x\n", -111, -222);
            TRICE32_2(Id(10121), "tst:TRICE32_2 %d %d\n", -111, -222);
        break;
        case 470:
            TRICE32_3(Id(15598), "tst:TRICE32_3 %x %x %x\n", -111, -222, -333);
            TRICE32_3(Id(15400), "tst:TRICE32_3 %d %d %d\n", -111, -222, -333);
            TRICE32_4(Id(15887), "tst:TRICE32_4 %x %x %x %x\n", -111, -222, -333, -444);
            TRICE32_4(Id(13628), "tst:TRICE32_4 %d %d %d %d\n", -111, -222, -333, -444);
        break;
        case 480:
            TRICE64_1(Id(15576), "tst:TRICE64_1 %d\n", -111);
            TRICE64_2(Id(13557), "tst:TRICE64_2 %d %d\n", -111, -222);
            TRICE16_1(Id(14507), "tst:TRICE16_1 %u\n", 60001);
            TRICE16_2(Id(12844), "tst:TRICE16_2 %u %u\n", 60001, 60002);
            TRICE16_3(Id(10716), "tst:TRICE16_3 %u %u %u\n", 60001, 60002, 60003);
            TRICE16_4(Id(14368), "tst:TRICE16_4 %u %u %u %u\n", 60001, 60002, 60003, 60004);
            TRICE32_1(Id(11505), "tst:TRICE32_1 %u\n", 4000000001);
            TRICE32_2(Id(15509), "tst:TRICE32_2 %u %u\n", 4000000001, 4000000002);
            TRICE32_3(Id(15174), "tst:TRICE32_3 %u %u %u\n", 4000000001, 4000000002, 4000000003);
            TRICE32_4(Id(10848), "tst:TRICE32_4 %u %u %u %u\n", 4000000001, 4000000002, 4000000003, 4000000004);
        break;
        case 490:
            TRICE64_1(Id(10809), "tst:TRICE64_1 %x\n", -1);
            TRICE64_2(Id(13550), "tst:TRICE64_2 %x %x\n", -1, -2);
            TRICE64_1(Id(11390), "tst:TRICE64_1 %u\n", -1);
            TRICE64_2(Id(12152), "tst:TRICE64_2 %u %u\n", -1, -2);
        break;
        case 500:
            TRICE( Id(10979), "sig:colored single letters and several TRICE macros in one line\n" );
            TRICE(Id(15240), "e:A");
            TRICE(Id(13271), "w:B");
            TRICE(Id(14033), "a:c");
            TRICE(Id(13736), "wr:d");
            TRICE(Id(10558), "rd:e\n");
            TRICE(Id(12477), "diag:f");
            TRICE(Id(13803), "d:G");
            TRICE(Id(15399), "t:H");
            TRICE(Id(15651), "time:i");
            TRICE(Id(11582), "message:J");
            TRICE(Id(12884), "dbg:k\n");
        break;
        case 510:
            TRICE(Id(12923), "1");
            TRICE(Id(13883), "2");
            TRICE(Id(11301), "3");
            TRICE(Id(10566), "4");
            TRICE(Id(14994), "e:7");
            TRICE(Id(10896), "m:12");
            TRICE(Id(15734), "m:123\n");
            TRICE(Id(13495), "e:A");TRICE(Id(15740), "w:B");TRICE(Id(13389), "a:c");
            TRICE(Id(11752), "wr:d");TRICE(Id(10060), "rd:e\n");//TRICE(Id(10798), "diag:f");
        break;
        case 520:
            TRICE( Id(12594), "sig:Runtime generated strings\n" );
            s = "AAAAAAAAAAAA";
            TRICE32( Id(14450), "dbg:len=%u:", strlen(s) );
            TRICE_S( Id(12206), "sig:%s\n", s );
            s = "\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBCCCCCCCC\
";
            TRICE32( Id(14927), "dbg:len=%u:", strlen(s) );
            TRICE_S( Id(13279), "sig:%s\n", s );
        break;
        case 530:
        TRICE( Id(13296), "sig:Runtime generated strings\n" );
            s = "\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\
DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\
EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\
";
            TRICE32( Id(15531), "dbg:len=%u: ", strlen(s) );
            TRICE_S( Id(14979), "sig:%s\n", s );
        break;
        case 540:
            TRICE( Id(10609), "sig:Runtime generated strings\n" );
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
            TRICE32( Id(15775), "dbg:len=%u: ", strlen(s) );
            TRICE_S( Id(13396), "sig:%s\n", s );
        break;
        case 550:
            TRICE( Id(14548), "sig:Runtime generated strings\n" );
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
          TRICE32( Id(15741), "dbg:len=%u: ", strlen(s) );
          TRICE_S( Id(13104), "sig:%s\n", s );
        break;
        case 560:{ // https://yourbasic.org/golang/fmt-printf-reference-cheat-sheet/
            int i = 15;
            TRICE( Id(10734), "sig:Integer (indent, base, sign)\n" );
            TRICE( Id(15374), "rd: 15 	%d 	Base 10\n", i );
            TRICE( Id(13341), "rd: +15 	%+d 	Always show sign\n", i );
            TRICE( Id(10819), "rd:   15 	%4d 	Pad with spaces (width 4, right justified)\n", i );
            TRICE( Id(13064), "rd: 15   	%-4d 	Pad with spaces (width 4, left justified)\n", i );
            TRICE( Id(11211), "rd: 0015 	%04d 	Pad with zeroes (width 4)\n", i );
            TRICE( Id(12227), "rd: 1111 	%b 	Base 2\n", i );
            TRICE( Id(12484), "rd: 17 	%o 	Base 8\n", i );
            TRICE( Id(14853), "rd: f 	%x 	Base 16, lowercase\n", i );
            TRICE( Id(12183), "rd: F 	%X 	Base 16, uppercase\n", i );
            TRICE( Id(10314), "rd: 0xf 	%#x 	Base 16, with leading 0x\n", i );
        }
        break;
        case 570:{
            char a = 'A';
            TRICE( Id(15667), "sig:Character (quoted, Unicode)\n" );
            TRICE( Id(10603), "rd: A 	%c 	Character\n", a );
            TRICE( Id(15693), "rd: 'A' 	%q 	Quoted character\n", a );
          //TRICE( Id(14803), "rd: U+0041 	%U 	Unicode\n", a );// todo
          //TRICE( Id(11278), "rd: U+0041 'A' 	%#U 	Unicode with character\n", a );// todo
            TRICE( Id(12039), "sig:Boolean (true/false)\n" );
            TRICE( Id(11819), "rd:Use %%t to format a boolean as true (%t) or false (%t).\n", 1, 0 );
            TRICE( Id(14127), "sig:Pointer (hex)\n" );
        }
        break;
        case 580:
            TRICE8( Id(12667), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x84 );
            TRICE16( Id(12941), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x1234 );
            TRICE32( Id(12303), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0xaabbccdd );
            TRICE64( Id(12027), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x1122334455667788 );
            TRICE( Id(13286), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0xaabbccdd);
        break;
        case 590:{
            float f = 123.456;
            TRICE( Id(15843), "sig:Float (indent, precision, scientific notation)\n" );
            TRICE( Id(11170), "rd: 1.234560e+02		%e 	%%e Scientific notation\n", aFloat(f) );
            TRICE( Id(14961), "rd: 123.456000		%f 	%%f Decimal point, no exponent\n", aFloat(f) );
            TRICE( Id(14837), "rd: 123.46	 		%.2f		%%.2f Default width, precision 2\n", aFloat(f) );
            TRICE( Id(12558), "rd: ␣␣123.46		%8.2f	%%8.2f Width 8, precision 2\n", aFloat(f) );
            TRICE( Id(14656), "rd: 123.456		%g		%%g Exponent as needed, necessary digits only\n", aFloat(f) );
        }
        break;
        case 600:{
            double d = 123.456;
            TRICE( Id(15490), "sig:Double (indent, precision, scientific notation)\n" );
            TRICE64( Id(12441), "rd: 1.234560e+02		%e 	%%e Scientific notation\n", aDouble(d) );
            TRICE64( Id(10332), "rd: 123.456000		%f 	%%f Decimal point, no exponent\n", aDouble(d) );
            TRICE64( Id(14546), "rd: 123.46			%.2f    	%%.2f Default width, precision 2\n", aDouble(d) );
            TRICE64( Id(14316), "rd: ␣␣123.46		%8.2f 	%%8.2f Width 8, precision 2\n", aDouble(d) );
            TRICE64( Id(13922), "rd: 123.456		%g		%%g Exponent as needed, necessary digits only\n", aDouble(d) );
        }
        break;
        case 610:{
            char* s = "café";
            TRICE( Id(14132), "sig:String or byte slice (quote, indent, hex)\n" );
            TRICE_S( Id(13589), "rd: café 			%s		Plain string\n", s );
            TRICE_S( Id(11145), "rd: ␣␣café 		%6s 		Width 6, right justify\n", s );
            TRICE_S( Id(11054), "rd: café␣␣ 		%-6s 		Width 6, left justify\n", s );
            TRICE_S( Id(13928), "rd: \"café\" 		%q	 	Quoted string\n", s );
            TRICE_S( Id(12377), "rd: 636166c3a9 		%x	Hex dump of byte values\n", s );
            TRICE_S( Id(13799), "rd: 63 61 66 c3 a9 	% x	Hex dump with spaces\n", s );
        }
        break;
        case 620:
            TRICE( Id(15497), "sig:TRICE32 with variable param count 1 to 12\n" );
            TRICE8(Id(14974), "tst:TRICE8 %d\n", -1 );
            TRICE8(Id(15343), "tst:TRICE8 %d %d\n", -1, -2 );
            TRICE8(Id(11305), "tst:TRICE8 %d %d %d\n", -1, -2, -3 );
            TRICE8(Id(10483), "tst:TRICE8 %d %d %d %d\n", -1, -2, -3, -4 );
            TRICE8(Id(11753), "tst:TRICE8 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
            TRICE8(Id(15787), "tst:TRICE8 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE8(Id(14963), "tst:TRICE8 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
            TRICE8(Id(12845), "tst:TRICE8 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 630:
            TRICE8(Id(12726), "tst:TRICE8 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE8(Id(15935), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
            TRICE8(Id(11705), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE8(Id(14547), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 640:
            TRICE8(Id(12230), "tst:TRICE8 %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );
            TRICE8_1 (Id(13780), "tst:TRICE8_1  %d\n", -1 );
            TRICE8_2 (Id(15706), "tst:TRICE8_2  %d %d\n", -1, -2 );
            TRICE8_3 (Id(10213), "tst:TRICE8_3  %d %d %d\n", -1, -2, -3 );
            TRICE8_4 (Id(10656), "tst:TRICE8_4  %d %d %d %d\n", -1, -2, -3, -4 );
            TRICE8_5 (Id(10159), "tst:TRICE8_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
            TRICE8_6 (Id(15763), "tst:TRICE8_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE8_7 (Id(12690), "tst:TRICE8_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
            TRICE8_8 (Id(15685), "tst:TRICE8_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 650:
            TRICE8_9 (Id(14374), "tst:TRICE8_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE8_10(Id(12373), "tst:TRICE8_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
            TRICE8_11(Id(15361), "tst:TRICE8_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE8_12(Id(12494), "tst:TRICE8_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
            TRICE8  (Id(12146), "tst:TRICE8   %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );
            TRICE8_9(Id(14861), "tst:TRICE8_9 %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );
        break;
        case 660:
            TRICE( Id(15498), "sig:TRICE16 with variable param count 1 to 12\n" );
            TRICE16(Id(10542), "tst:TRICE16 %d\n", -1 );
            TRICE16(Id(12620), "tst:TRICE16 %d %d\n", -1, -2 );
            TRICE16(Id(13143), "tst:TRICE16 %d %d %d\n", -1, -2, -3 );
            TRICE16(Id(14189), "tst:TRICE16 %d %d %d %d\n", -1, -2, -3, -4 );
            TRICE16(Id(13263), "tst:TRICE16 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
            TRICE16(Id(11574), "tst:TRICE16 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE16(Id(13196), "tst:TRICE16 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
            TRICE16(Id(10315), "tst:TRICE16 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 670:
            TRICE16(Id(14216), "tst:TRICE16 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE16(Id(12210), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
            TRICE16(Id(14391), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE16(Id(13290), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 680:
            TRICE16(Id(14637), "tst:TRICE16 %t %b %x %X %d %u %o %O %p\n", 3, 3, 3, 3, 3, 3, 3, 3, 3 );
            TRICE16_1 (Id(10856), "tst:TRICE16_1  %d\n", -1 );
            TRICE16_2 (Id(14344), "tst:TRICE16_2  %d %d\n", -1, -2 );
            TRICE16_3 (Id(10185), "tst:TRICE16_3  %d %d %d\n", -1, -2, -3 );
            TRICE16_4 (Id(13937), "tst:TRICE16_4  %d %d %d %d\n", -1, -2, -3, -4 );
            TRICE16_5 (Id(12492), "tst:TRICE16_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
            TRICE16_6 (Id(14883), "tst:TRICE16_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE16_7 (Id(11959), "tst:TRICE16_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
            TRICE16_8 (Id(15592), "tst:TRICE16_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 690:
            TRICE16_9 (Id(15407), "tst:TRICE16_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE16_10(Id(10011), "tst:TRICE16_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
            TRICE16_11(Id(15402), "tst:TRICE16_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE16_12(Id(12493), "tst:TRICE16_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
            TRICE16_9(Id(13148), "tst:TRICE16 %t %b %x %X %d %u %o %O %p\n", 3, 3, 3, 3, 3, 3, 3, 3, 3 );
        break;
        case 700:
            TRICE( Id(11249), "sig:TRICE32 with variable param count 1 to 12\n" );
            TRICE32(Id(10053), "tst:TRICE32 %d\n", -1 );
            TRICE32(Id(14558), "tst:TRICE32 %d %d\n", -1, -2 );
            TRICE32(Id(14117), "tst:TRICE32 %d %d %d\n", -1, -2, -3 );
            TRICE32(Id(11827), "tst:TRICE32 %d %d %d %d\n", -1, -2, -3, -4 );
            TRICE32(Id(15762), "tst:TRICE32 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
            TRICE32(Id(11261), "tst:TRICE32 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE32(Id(11337), "tst:TRICE32 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
            TRICE32(Id(13987), "tst:TRICE32 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 710:
            TRICE32(Id(11430), "tst:TRICE32 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE32(Id(10178), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
            TRICE32(Id(13908), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE32(Id(12275), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 720:
            TRICE32(Id(12584), "tst:TRICE32 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));
            TRICE32_1 (Id(14881), "tst:TRICE32_1  %d\n", -1 );
            TRICE32_2 (Id(14945), "tst:TRICE32_2  %d %d\n", -1, -2 );
            TRICE32_3 (Id(12855), "tst:TRICE32_3  %d %d %d\n", -1, -2, -3 );
            TRICE32_4 (Id(14935), "tst:TRICE32_4  %d %d %d %d\n", -1, -2, -3, -4 );
            TRICE32_5 (Id(15371), "tst:TRICE32_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
            TRICE32_6 (Id(15921), "tst:TRICE32_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE32_7 (Id(13907), "tst:TRICE32_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
            TRICE32_8 (Id(11730), "tst:TRICE32_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 730:
            TRICE32_9 (Id(15274), "tst:TRICE32_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE32_10(Id(10870), "tst:TRICE32_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
            TRICE32_11(Id(15867), "tst:TRICE32_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE32_12(Id(15193), "tst:TRICE32_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
            TRICE32   (Id(14718), "tst:TRICE32    %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));
            TRICE32_12(Id(12983), "tst:TRICE32_12 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));
        break;
        case 740:
            TRICE( Id(12133), "sig:TRICE64 with variable param count 1 to 12\n" );
            TRICE64(Id(10955), "tst:TRICE64 %d\n", -1 );
            TRICE64(Id(10503), "tst:TRICE64 %d %d\n", -1, -2 );
            TRICE64(Id(14765), "tst:TRICE64 %d %d %d\n", -1, -2, -3 );
            TRICE64(Id(15541), "tst:TRICE64 %d %d %d %d\n", -1, -2, -3, -4 );
            TRICE64(Id(15127), "tst:TRICE64 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
            TRICE64(Id(12851), "tst:TRICE64 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE64(Id(15556), "tst:TRICE64 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
            TRICE64(Id(13011), "tst:TRICE64 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 750:
            TRICE64(Id(10027), "tst:TRICE64 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE64(Id(14630), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
            TRICE64(Id(13125), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE64(Id(12873), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break;
        case 760:
            TRICE64(Id(11202), "tst:TRICE64 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aDouble(3.14159), aDouble(3.14159), aDouble(3.14159));
            TRICE64_1 (Id(13511), "tst:TRICE64_1  %d\n", -1 );
            TRICE64_2 (Id(15960), "tst:TRICE64_2  %d %d\n", -1, -2 );
            TRICE64_3 (Id(15434), "tst:TRICE64_3  %d %d %d\n", -1, -2, -3 );
            TRICE64_4 (Id(11523), "tst:TRICE64_4  %d %d %d %d\n", -1, -2, -3, -4 );
            TRICE64_5 (Id(12879), "tst:TRICE64_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
            TRICE64_6 (Id(13981), "tst:TRICE64_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
            TRICE64_7 (Id(14116), "tst:TRICE64_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
            TRICE64_8 (Id(14345), "tst:TRICE64_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break;
        case 770:
            TRICE64_9 (Id(15461), "tst:TRICE64_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
            TRICE64_10(Id(13945), "tst:TRICE64_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
            TRICE64_11(Id(12687), "tst:TRICE64_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
            TRICE64_12(Id(10190), "tst:TRICE64_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
            TRICE64_12(Id(10650), "tst:TRICE64_12 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aDouble(3.14159), aDouble(3.14159), aDouble(3.14159));
        break;
        case 780:
            TRICE( Id(14012), "att:positive and negative float in format variants\n");
            TRICE( Id(10863), "rd:TRICE float %f (%%f)\n", aFloat(x) );
            TRICE( Id(15449), "rd:TRICE float %9f (%%9f)\n", aFloat(x) );
            TRICE( Id(14415), "rd:TRICE float %.9f (%%.9f)\n", aFloat(x) );
            TRICE( Id(11594), "rd:TRICE float %9.f (%%9.f)\n", aFloat(x) );
            TRICE( Id(14018), "rd:TRICE float %9.6f (%%9.6f)\n", aFloat(x) );
            TRICE( Id(13531), "rd:TRICE float %f (%%f)\n", aFloat(-x) );
        break;
        case 790:
            TRICE( Id(12325), "rd:TRICE float %9f (%%9f)\n", aFloat(-x) );
            TRICE( Id(15528), "rd:TRICE float %.9f (%%.9f)\n", aFloat(-x) );
            TRICE( Id(14359), "rd:TRICE float %9.f (%%9.f)\n", aFloat(-x) );
            TRICE( Id(14737), "rd:TRICE float %9.6f (%%9.6f)\n", aFloat(-x) );
            TRICE( Id(15997), "rd:TRICE float %+f (%%f)\n", aFloat(x) );
            TRICE( Id(13843), "rd:TRICE float %+9f (%%9f)\n", aFloat(x) );
            TRICE( Id(14583), "rd:TRICE float %+.9f (%%.9f)\n", aFloat(x) );
            TRICE( Id(11607), "rd:TRICE float %+9.f (%%9.f)\n", aFloat(x) );
        break;
        case 800:
            TRICE( Id(11840), "rd:TRICE float %+9.6f (%%9.6f)\n", aFloat(x) );
            TRICE( Id(13351), "rd:TRICE float %+f (%%f)\n", aFloat(-x) );
            TRICE( Id(12685), "rd:TRICE float %+9f (%%9f)\n", aFloat(-x) );
            TRICE( Id(10703), "rd:TRICE float %+.9f (%%.9f)\n", aFloat(-x) );
            TRICE( Id(15675), "rd:TRICE float %+9.f (%%9.f)\n", aFloat(-x) );
            TRICE( Id(13459), "rd:TRICE float %+9.6f (%%9.6f)\n", aFloat(-x) );
            TRICE( Id(10551), "rd:TRICE float %-f (%%f)\n", aFloat(x) );
            TRICE( Id(10286), "rd:TRICE float %-9f (%%9f)\n", aFloat(x) );
        break;
        case 810:
            TRICE( Id(14263), "rd:TRICE float %-.9f (%%.9f)\n", aFloat(x) );
            TRICE( Id(14797), "rd:TRICE float %-9.f (%%9.f)\n", aFloat(x) );
            TRICE( Id(11941), "rd:TRICE float %-9.6f (%%9.6f)\n", aFloat(x) );
            TRICE( Id(12694), "rd:TRICE float %-f (%%f)\n", aFloat(-x) );
            TRICE( Id(14764), "rd:TRICE float %-9f (%%9f)\n", aFloat(-x) );
            TRICE( Id(12705), "rd:TRICE float %-.9f (%%.9f)\n", aFloat(-x) );
            TRICE( Id(10851), "rd:TRICE float %-9.f (%%9.f)\n", aFloat(-x) );
            TRICE( Id(13233), "rd:TRICE float %-9.6f (%%9.6f)\n", aFloat(-x) );
        break;
        case 820:
            TRICE( Id(13417), "att:positive float & double in variants\n");
            TRICE32_1( Id(11747), "rd:TRICE32_1 float %e (%%e)\n", aFloat(x) );
            TRICE32_1( Id(13491), "rd:TRICE32_1 float %f (%%f)\n", aFloat(x) );
            TRICE32_1( Id(14643), "rd:TRICE32_1 float %g (%%g)\n", aFloat(x) );
            TRICE32_1( Id(11001), "rd:TRICE32_1 float %E (%%E)\n", aFloat(x) );
            TRICE32_1( Id(14593), "rd:TRICE32_1 float %F (%%F)\n", aFloat(x) );
            TRICE32_1( Id(15548), "rd:TRICE32_1 float %G (%%G)\n", aFloat(x) );
        break;
        case 830:
            TRICE32( Id(14338), "rd:TRICE32 float %e (%%e)\n", aFloat(x) );
            TRICE32( Id(11302), "rd:TRICE32 float %f (%%f)\n", aFloat(x) );
            TRICE32( Id(10581), "rd:TRICE32 float %g (%%g)\n", aFloat(x) );
            TRICE32( Id(14684), "rd:TRICE32 float %E (%%E)\n", aFloat(x) );
            TRICE32( Id(14325), "rd:TRICE32 float %F (%%F)\n", aFloat(x) );
            TRICE32( Id(10513), "rd:TRICE32 float %G (%%G)\n", aFloat(x) );
        break;
        case 840:
            TRICE( Id(10242), "rd:TRICE float %e (%%e)\n", aFloat(x) );
            TRICE( Id(14991), "rd:TRICE float %f (%%f)\n", aFloat(x) );
            TRICE( Id(15309), "rd:TRICE float %g (%%g)\n", aFloat(x) );
            TRICE( Id(12777), "rd:TRICE float %E (%%E)\n", aFloat(x) );
            TRICE( Id(13542), "rd:TRICE float %F (%%F)\n", aFloat(x) );
            TRICE( Id(13676), "rd:TRICE float %G (%%G)\n", aFloat(x) );
        break;
        case 850:
            TRICE64( Id(11324), "rd:TRICE64 double %e (%%e), aDouble(y)\n", aDouble(y) );
            TRICE64( Id(11939), "rd:TRICE64 double %f (%%f), aDouble(y)\n", aDouble(y) );
            TRICE64( Id(14698), "rd:TRICE64 double %g (%%g), aDouble(y)\n", aDouble(y) );
            TRICE64( Id(12806), "rd:TRICE64 double %E (%%E), aDouble(y)\n", aDouble(y) );
            TRICE64( Id(13904), "rd:TRICE64 double %F (%%F), aDouble(y)\n", aDouble(y) );
            TRICE64( Id(13078), "rd:TRICE64 double %G (%%G), aDouble(y)\n", aDouble(y) );
        break;
        case 860:
            TRICE64_1( Id(15233), "rd:TRICE64_1 double %e (%%e), aDouble(y)\n", aDouble(y) );
            TRICE64_1( Id(12111), "rd:TRICE64_1 double %f (%%f), aDouble(y)\n", aDouble(y) );
            TRICE64_1( Id(14093), "rd:TRICE64_1 double %g (%%g), aDouble(y)\n", aDouble(y) );
            TRICE64_1( Id(10257), "rd:TRICE64_1 double %E (%%E), aDouble(y)\n", aDouble(y) );
            TRICE64_1( Id(12087), "rd:TRICE64_1 double %F (%%F), aDouble(y)\n", aDouble(y) );
            TRICE64_1( Id(14247), "rd:TRICE64_1 double %G (%%G), aDouble(y)\n", aDouble(y) );
        break;
        case 870:
            TRICE( Id(15684), "att:negative float & double\n" );
            TRICE( Id(12608), "rd:TRICE float %e (%%e)\n", aFloat(-x) );
            TRICE( Id(15273), "rd:TRICE float %f (%%f)\n", aFloat(-x) );
            TRICE( Id(12716), "rd:TRICE float %g (%%g)\n", aFloat(-x) );
            TRICE( Id(13853), "rd:TRICE float %E (%%E)\n", aFloat(-x) );
            TRICE( Id(11642), "rd:TRICE float %F (%%F)\n", aFloat(-x) );
            TRICE( Id(13733), "rd:TRICE float %G (%%G)\n", aFloat(-x) );
        break;
        case 880:
            TRICE64( Id(12570), "rd:TRICE64 double %e (%%e), aDouble(y)\n", aDouble(-y) );
            TRICE64( Id(11564), "rd:TRICE64 double %f (%%f), aDouble(y)\n", aDouble(-y) );
            TRICE64( Id(11316), "rd:TRICE64 double %g (%%g), aDouble(y)\n", aDouble(-y) );
            TRICE64( Id(11858), "rd:TRICE64 double %E (%%E), aDouble(y)\n", aDouble(-y) );
            TRICE64( Id(15816), "rd:TRICE64 double %F (%%F), aDouble(y)\n", aDouble(-y) );
            TRICE64( Id(12015), "rd:TRICE64 double %G (%%G), aDouble(y)\n", aDouble(-y) );
        break;
        case 890:
            TRICE( Id(10555), "att:formatted float & double\n" );
            TRICE32( Id(15587), "rd:TRICE32 float %12.6e (%%12.6e)\n", aFloat(x) );
            TRICE32( Id(10399), "rd:TRICE32 float %12.6f (%%12.6f)\n", aFloat(x) );
            TRICE32( Id(15745), "rd:TRICE32 float %12.6g (%%12.6g)\n", aFloat(x) );
            TRICE32( Id(11888), "rd:TRICE32 float %12.6E (%%12.6E)\n", aFloat(x) );
            TRICE32( Id(12822), "rd:TRICE32 float %12.6F (%%12.6F)\n", aFloat(x) );
            TRICE32( Id(15261), "rd:TRICE32 float %12.6G (%%12.6G)\n", aFloat(x) );
        break;
        case 900:
            TRICE64( Id(11609), "rd:TRICE64 double %12.6e (%%12.6e), aDouble(y)\n", aDouble(y) );
            TRICE64( Id(10615), "rd:TRICE64 double %12.6f (%%12.6f), aDouble(y)\n", aDouble(y) );
            TRICE64( Id(11745), "rd:TRICE64 double %12.6g (%%12.6g), aDouble(y)\n", aDouble(y) );
            TRICE64( Id(15250), "rd:TRICE64 double %12.6E (%%12.6E), aDouble(y)\n", aDouble(y) );
            TRICE64( Id(12309), "rd:TRICE64 double %12.6F (%%12.6F), aDouble(y)\n", aDouble(y) );
            TRICE64( Id(11243), "rd:TRICE64 double %12.6G (%%12.6G), aDouble(y)\n", aDouble(y) );
        break;
        case 910:
            TRICE( Id(15868), "att:mixed int & float & double & bits\n" );
            TRICE32( Id(11656), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n", FloatToInt32(x), aFloat(x), aFloat(x), aFloat(x) );
            TRICE32( Id(10076), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",    (int32_t)(x), aFloat(x), aFloat(x), aFloat(x) );
            TRICE32( Id(11454), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",              x,  aFloat(x), aFloat(x), aFloat(x) );
            TRICE32( Id(14092), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n", FloatToInt32(-x), aFloat(-x), aFloat(-x), aFloat(-x) );
            TRICE32( Id(15536), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",    (int32_t)(-x), aFloat(-x), aFloat(-x), aFloat(-x) );
            TRICE32( Id(14813), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",              -x,  aFloat(-x), aFloat(-x), aFloat(-x) );
        break;
        case 920:
            TRICE64( Id(11308), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n", DoubleToInt64(y), aDouble(y), aDouble(y), aDouble(y) );
            TRICE64( Id(15029), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",     (int64_t)(y), aDouble(y), aDouble(y), aDouble(y) );
            TRICE64( Id(12858), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",               y,  aDouble(y), aDouble(y), aDouble(y) );
            TRICE64( Id(11902), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n", DoubleToInt64(-y), aDouble(-y), aDouble(-y), aDouble(-y) );
            TRICE64( Id(14387), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",     (int64_t)(-y), aDouble(-y), aDouble(-y), aDouble(-y) );
            TRICE64( Id(12009), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",               -y,  aDouble(-y), aDouble(-y), aDouble(-y) );
        break;
        case 930:
            TRICE( Id(12026), "att:Various single arguments\n" );
            TRICE8( Id(11527), "rd:TRICE8 line %t (%%t ,0)\n", 0 );
            TRICE8( Id(10546), "rd:TRICE8 line %t (%%t ,2)\n", 2 );
            TRICE8( Id(12885), "rd:TRICE8 line %u (%%u ,-1)\n", -1 );
            TRICE8( Id(12040), "rd:TRICE8 line %b (%%b ,-1)\n", -1 );
            TRICE8( Id(13520), "rd:TRICE8 line %o (%%o ,-1)\n", -1 );
            TRICE8( Id(14732), "rd:TRICE8 line %O (%%O ,-1)\n", -1 );
            TRICE8( Id(14202), "rd:TRICE8 line %X (%%X ,-1)\n", -1 );
            TRICE8( Id(14839), "rd:TRICE8 line %x (%%x ,-1)\n", -1 );
            TRICE8( Id(15268), "rd:TRICE8 line %d (%%d ,-1)\n", -1 );
        break;
        case 940:
            TRICE16_1( Id(13365), "rd:TRICE16_1 line %t (%%t ,0)\n", 0 );
            TRICE16_1( Id(13278), "rd:TRICE16_1 line %t (%%t ,2)\n", 2 );
            TRICE16_1( Id(12982), "rd:TRICE16_1 line %u (%%u -1)\n", -1 );
            TRICE16_1( Id(13549), "rd:TRICE16_1 line %b (%%b -1)\n", -1 );
            TRICE16_1( Id(14689), "rd:TRICE16_1 line %o (%%o -1)\n", -1 );
            TRICE16_1( Id(15043), "rd:TRICE16_1 line %x (%%x -1)\n", -1 );
        break;
        case 950:
            TRICE16_1( Id(10019), "rd:TRICE16_1 line %d (%%d -1)\n", -1 );
            TRICE( Id(15979), "rd:TRICE line %t (%%t -1)\n", 0 );
            TRICE( Id(13673), "rd:TRICE line %t (%%t -1)\n", 2 );
            TRICE( Id(12156), "rd:TRICE line %u (%%u)\n", -1 );
            TRICE( Id(12575), "rd:TRICE line %b (%%b)\n", -2 );
            TRICE( Id(11300), "rd:TRICE line %o (%%o)\n", -3 );
            TRICE( Id(13902), "rd:TRICE line %x (%%x)\n", -4 );
            TRICE( Id(13782), "rd:TRICE line %d (%%d)\n", -5 );
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
        TRICE0 (Id(11085), "--------------------------------------------------\n" );
        {
            uint32_t by[8] = {1,2,3,4,5,6};
            TRICE32(  Id(11998), "msg: message = %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
            TRICE32( Id(11907), "tim: pre TriceEncryption SysTick=%d\n", SYSTICKVAL );
            TriceEncrypt(by, 6);
            TRICE32( Id(14917), "tim: post TriceEncryption SysTick=%d\n", SYSTICKVAL );
            TRICE32(  Id(15615), "att: TriceEncrypted =  %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
            #ifdef TRICE_DECRYPT
            TRICE16_1( Id(11019), "tim: pre TriceDecryption SysTick=%d\n", SYSTICKVAL );
            TriceDecrypt(by, 6);
            TRICE16_1( Id(13211), "tim: post TriceDecryption SysTick=%d\n", SYSTICKVAL );
            #endif
            TRICE32(  Id(14240), "msg: messge = %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
        }
        TRICE0 (Id(14023), "--------------------------------------------------\n" );
#endif
        break;
        default:
        break;
    }
}



//! SCOPY is a helper macro for struct serialization.
#define SCOPY( element ) do{ char* n = #element; int size = sizeof( src->element ); memcpy( p, &(src->element), size ); p += size; TRICE_S( Id(10433), "rd:sizeof(%8s)", n ); TRICE( Id(14289), " = %d\n", size);}while(0);

    //! DCOPY is a helper macro for struct deserialization.
#define DCOPY( element ) do{ char* n = #element; int size = sizeof( dst->element ); memcpy( &(dst->element), p, size ); p += size; TRICE_S( Id(13474), "rd:sizeof(%8s)", n ); TRICE( Id(13800), " = %d\n", size);}while(0);


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
    TRICE ( Id(11442), "inf: Tryout tx struct:" );
    TRICE_B( Id(11284), " %02x ", &tx, sizeof(tx) );
    TRICE( Id(15779), "\n" );
    
    TRICE ( Id(14357), "inf: Tryout buffer:" );
    TRICE_B( Id(13606), " %02x ", dst, len );
    TRICE( Id(15046), "\n" );

    src = dst; // "data transfer"
    
    len = deserializeTryout( &rx, src );
    TRICE ( Id(12502), "inf: Tryout rx struct:" );
    TRICE_B( Id(13829), " %02x ", &rx, sizeof(rx) );
    TRICE( Id(14071), "\n" );

    TRICE( Id(14648), "inf:sizeOf(Trypout) = %d, buffer length = %d\n", sizeof(tx), len );
    TRICE8_F( Id(14705), "info:TryoutStructFunction", &tx, sizeof(tx) );
    TRICE8_F( Id(14395), "info:TryoutBufferFunction", dst, len ); 
}

static void exampleOfManualJSONencoding(void){
    typedef  struct {
    int Apple, Birn;
    float Fish;
    } Ex_t;
    Ex_t Ex = { -1, 2, 2.781 };
    TRICE( Id(13149), "att:MyStructEvaluationFunction(json:ExA{Apple:%d, Birn:%u, Fisch:%f}\n", Ex.Apple, Ex.Birn, aFloat(Ex.Fish) );
}

void exampleOfBuffersAndFunctions(void){
    static int8_t   b8[24] = { 0, -1, -2, 0x33, 4, 5, 6, 7, 8, 9, 10, 11, 0, -1, -2, 0x33, 4, 5, 6, 7, 8, 9, 10, 11 };
    static int16_t b16[] = { 0, -1, -2, 0x3344 };
    static int32_t b32[] = { 0, -1, -2, 0x33445555};
    static int64_t b64[4] = { 0, -1, -2, 0x3344555566666666 };
    char* s = "abcde 12345";
    uint32_t len;

    TRICE_S( Id(11119), "msg:With TRICE_S:%s\n", s );
    len = strlen(s);
    TRICE_N( Id(14748), "sig:With TRICE_N:%s\n", s, len );
    TRICE32( Id(15560), "att:len=%u:With TRICE_B:\n", len);
    TRICE_B( Id(15535), "  %02x", s, len );
    TRICE( Id(11168), "\n" );
    TRICE_B( Id(12385), "%4d", s, len );
    TRICE( Id(12939), "\n" );

    TRICE( Id(10771), "notice:TRICE_B example: " );
    TRICE_B( Id(13006), "  %02x", b8, sizeof(b8)/sizeof(int8_t) );
    TRICE( Id(11618), "\n" );
    TRICE( Id(13884), "notice:TRICE8_B example:" );
    TRICE8_B( Id(14956), "  %02x", b8, sizeof(b8)/sizeof(int8_t) );
    TRICE( Id(14378), "\n" );
    TRICE( Id(15376), "notice:TRICE16_B example:" );
    TRICE16_B( Id(13926), "  %04x", b16, sizeof(b16)/sizeof(int16_t) );
    TRICE( Id(12037), "\n" );
    TRICE( Id(15001), "notice:TRICE32_B example:" );
    TRICE32_B( Id(13642), "  %08x", b32, sizeof(b32)/sizeof(int32_t) );
    TRICE( Id(11092), "\n" );
    TRICE( Id(10846), "notice:TRICE64_B example:" );
    TRICE64_B( Id(10243), "  %016x", b64, sizeof(b64)/sizeof(int64_t) );
    TRICE( Id(10936), "\n" );    

    TRICE( Id(11916), "notice:TRICE_F example: " );
    TRICE_F( Id(14245), "info:FunctionNameW",   b8,  sizeof(b8) /sizeof(int8_t) );

    TRICE( Id(15171), "notice:TRICE8_F example:" );
    TRICE8_F(  Id(12053), "info:FunctionNameW",   b8,  sizeof(b8) /sizeof(int8_t) );

    TRICE( Id(10625), "notice:TRICE16_F example:" );
    TRICE16_F( Id(12132), "sig:FunctionNameX",    b16, sizeof(b16)/sizeof(int16_t) );

    TRICE( Id(12279), "notice:TRICE32_F example:" );
    TRICE32_F( Id(11436), "diag:FunctionNameY",   b32, sizeof(b32)/sizeof(int32_t) );

    TRICE( Id(14838), "notice:TRICE64_F example:" );
    TRICE64_F( Id(15889), "fatal:FunctionNameZ", b64, sizeof(b64)/sizeof(int64_t) );
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
        TRICE( Id(15037), "msg:exp=%02x act=%02x\n", expBuf[i], actBuf[i] );
        if( expBuf[i] != actBuf[i] ){
            return 0;
        }
    }
    return result;
}

void TCOBSCheck( void ){
    uint8_t result[80];
    for( int i = 0; i < TCOBS_TESTDATASET_COUNT; i++ ){
        TRICE( Id(12501), "dbg: Set %d\n", i );
        int rlen = TCOBSEncode( result, TCOBSTestData[i].in, TCOBSTestData[i].ilen );
        if( !equal( TCOBSTestData[i].out, TCOBSTestData[i].olen, result, rlen ) ){
            TRICE( Id(12319), "ERROR: TCOBS!\n" );
        }
    }
}
*/
