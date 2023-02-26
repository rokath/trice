/*! \file triceCheck.c
\brief trices for tool evaluation
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/
#include <float.h>
//#define TRICE_OFF // enable this line to disable trice code generation in this file object
#include "trice.h"

// Do **NOT** edit triceCheck.c!!!
//
// Edit file triceCheck.c.txt instead and call updateTestData.sh then.
// The strings behind "//exp:" are the expected result for each line (-color=none)

static int32_t FloatToInt32( float f );
static int64_t DoubleToInt64( double f );
static void exampleOfManualSerialization( void );
static void exampleOfManualJSONencoding(void);
#define TRICE_SHORT_CHECK_START_LINE 50     // todo: automate somehow
#define TRICE_SHORT_CHECK_LIMIT_LINE (50+8) // todo: automate somehow

//! TriceCheck performs trice code sequence n. TriceCheck writes out all types of trices with fixed values for testing
//! \details One trice has one subtrace, if param size max 2 bytes. 
//! Traces with more bytes as parameter consist of several subtraces.
//! Each trice line needs to have a commented // "string" with its expected output.
//! The ID values must be in file triceCheck.c, because it is compiled first and trice update runs later.
// This function is also called from Go for tests.
void TriceCheck(int n) {
    char* s = "AAAAAAAAAAAA";
    float  x = (float)1089.6082763671875; // 0x44883377
    double y = 518.0547492508867; // 0x4080307020601050  
    static int8_t b8[24] = { 0, -1, -2, 0x33, 4, 5, 6, 7, 8, 9, 10, 11, 0, -1, -2, 0x33, 4, 5, 6, 7, 8, 9, 10, 11 };
    static int16_t b16[] = { 0, -1, -2, 0x3344 };
    static int32_t b32[] = { 0, -1, -2, 0x33445555};
    static int64_t b64[4] = { 0, -1, -2, 0x3344555566666666 };
    char* sABCDE = "abcde 12345";
    uint32_t lenABCDE = strlen(sABCDE);

    #ifdef TRICE_CGO
    char* A = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
    #endif // #ifdef TRICE_CGO

    switch( n ){
        default:

        break; case __LINE__: trice16( iD( 1198), "att: line %u\n", __LINE__ );

        #if __LINE__ != 50-3  // TRICE_SHORT_CHECK_START_LINE 
        #error // todo: automate somehow, see https://stackoverflow.com/questions/1597007/creating-c-macro-with-and-line-token-concatenation-with-positioning-macr
        #endif
        break; case __LINE__: TRice16( iD( 1906), "att: 0x8888 == %04xh\n", 0x8888 ); //exp: time: 842,150_450default: att: 0x8888 == 8888h
        break; case __LINE__: Trice16( iD( 4538), "att: 0x8888 == %04xh\n", 0x8888 ); //exp: time:       5_654default: att: 0x8888 == 8888h
        break; case __LINE__: trice16( iD( 7512), "att: 0x8888 == %04xh\n", 0x8888 ); //exp: time:            default: att: 0x8888 == 8888h

        // normal use cases
        break; case __LINE__: TRice( iD( 6619), "info:This is a message without values and a 32-bit stamp.\n" );        //exp: time: 842,150_450default: info:This is a message without values and a 32-bit stamp.
        break; case __LINE__: Trice( iD( 3819), "info:This is a message without values and a 16-bit stamp.\n" );        //exp: time:       5_654default: info:This is a message without values and a 16-bit stamp.
        break; case __LINE__: trice( iD( 4084), "info:This is a message without values and without stamp.\n"  );        //exp: time:            default: info:This is a message without values and without stamp.
        break; case __LINE__: trice16( iD( 4869), "att: line %u\n", __LINE__ );

        // todo: improve parsing of //exp string containing special characters
        //break; case __LINE__: TRice( iD( 3164), "A TRice message containing a newline\nbefore the line end\n" );               //exp: time: 842,150_450default: A TRice message containing a newline\n                                         before the line end                                                                                                      
        //break; case __LINE__: TRice( iD( 6554), "\nA TRice message starting with a a newline\n" );                             //exp: time: 842,150_450default: \n                                         A TRice message starting with a a newline                                                                                                                   
        //break; case __LINE__: TRice( iD( 3112), "line0:A TRice message with several lines\nline1\nline2\nline3:last line\n" ); //exp: time: 842,150_450default: line0:A TRice message with several lines\\n                                         line1\n                                         line2\n                                         line3:last line

        // normal use cases
        break; case __LINE__: TRice( iD( 3207), "info:This is a message without values and a 32-bit stamp.\n" );        //exp: time: 842,150_450default: info:This is a message without values and a 32-bit stamp.
        break; case __LINE__: Trice( iD( 5155), "info:This is a message without values and a 16-bit stamp.\n" );        //exp: time:       5_654default: info:This is a message without values and a 16-bit stamp.
        break; case __LINE__: trice( iD( 1060), "info:This is a message without values and without stamp.\n"  );        //exp: time:            default: info:This is a message without values and without stamp.

        // CGO compiler issue
    //  break; case __LINE__: TRice( iD( 6813),"info:12 default bit width values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 32-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //_exp: time: 842,150_450default: info:12 default bit width values -3, 4294967292, fffffffb, FFFFFFFA, truet, 1.401298e-45, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11000000111000111111001101011100 and a 32-bit stamp.
    //  break; case __LINE__: Trice( iD( 7351),"info:12 default bit width values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 16-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //_exp: time:       5_654default: info:12 default bit width values -3, 4294967292, fffffffb, FFFFFFFA, truet, 1.401298e-45, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11000000111000111111001101011100 and a 16-bit stamp.
    //  break; case __LINE__: trice( iD( 7688),"info:12 default bit width values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and without stamp.\n" , -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //_exp: time:            default: info:12 default bit width values -3, 4294967292, fffffffb, FFFFFFFA, truet, 1.401298e-45, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11000000111000111111001101011100 and without stamp.

        break; case __LINE__: trice16( iD( 7900), "att: line %u\n", __LINE__ );

        // special use cases
        break; case __LINE__: TRice64( iD( 1574),"info:12 64-bit values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 32-bit stamp.\n", -3, -4, -5, -6, 0, aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), -8 ); //exp: time: 842,150_450default: info:12 64-bit values -3, 18446744073709551612, fffffffffffffffb, FFFFFFFFFFFFFFFA, false, -7.123457e+00, -7.123457, -7.123456789, -7.123457E+00, -7.123457, -7.123456789, 0xb1111111111111111111111111111111111111111111111111111111111111000 and a 32-bit stamp.
        break; case __LINE__: Trice64( iD( 4506),"info:12 64-bit values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 16-bit stamp.\n", -3, -4, -5, -6, 0, aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), -8 ); //exp: time:       5_654default: info:12 64-bit values -3, 18446744073709551612, fffffffffffffffb, FFFFFFFFFFFFFFFA, false, -7.123457e+00, -7.123457, -7.123456789, -7.123457E+00, -7.123457, -7.123456789, 0xb1111111111111111111111111111111111111111111111111111111111111000 and a 16-bit stamp.
        break; case __LINE__: trice64( iD( 2557),"info:12 64-bit values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and without stamp.\n" , -3, -4, -5, -6, 0, aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), -8 ); //exp: time:            default: info:12 64-bit values -3, 18446744073709551612, fffffffffffffffb, FFFFFFFFFFFFFFFA, false, -7.123457e+00, -7.123457, -7.123456789, -7.123457E+00, -7.123457, -7.123456789, 0xb1111111111111111111111111111111111111111111111111111111111111000 and without stamp.
        break; case __LINE__: TRice32( iD( 2111),"info:12 32-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 32-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time: 842,150_450default: info:12 32-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 32-bit stamp.
        break; case __LINE__: Trice32( iD( 6723),"info:12 32-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 16-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:       5_654default: info:12 32-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 16-bit stamp.
        break; case __LINE__: trice32( iD( 4381),"info:12 32-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and without stamp.\n" , -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:            default: info:12 32-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and without stamp.
        break; case __LINE__: TRice16( iD( 1359),"info:12 16-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 32-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time: 842,150_450default: info:12 16-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 32-bit stamp.
        break; case __LINE__: Trice16( iD( 6142),"info:12 16-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 16-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:       5_654default: info:12 16-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 16-bit stamp.
        break; case __LINE__: trice16( iD( 3095),"info:12 16-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and without stamp.\n" , -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:            default: info:12 16-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and without stamp.
        break; case __LINE__: TRice8 ( iD( 2267),"info:12  8-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 32-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time: 842,150_450default: info:12  8-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 32-bit stamp.
        break; case __LINE__: Trice8 ( iD( 1463),"info:12  8-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 16-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:       5_654default: info:12  8-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 16-bit stamp.
        break; case __LINE__: trice8 ( iD( 2678),"info:12  8-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and without stamp.\n" , -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:            default: info:12  8-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and without stamp.

        break; case __LINE__: trice16( iD( 7573), "att: line %u\n", __LINE__ );
        break; case __LINE__: exampleOfManualSerialization();

        break; case __LINE__: trice8   ( iD( 2766),         "value=%x\n", 0x55 );                           //exp: time:            default: value=55
        break; case __LINE__: trice8   ( iD( 6810),  "value=%x\n", 0x88 );                           //exp: time:            default: value=88

        break; case __LINE__: TRICE( ID( 3396), "att:mixed int & float & double & bits\n" );
        break; case __LINE__: TRICE32( ID( 5151), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",       FloatToInt32(x),   aFloat(x),   aFloat(x),   aFloat(x) ); //exp: time: 842,150_450default: rd:TRICE32 int 1089, float 1089.608276 (%f), 44883377, 01000100100010000011001101110111                                         
    //  break; case __LINE__: TRICE32( ID( 6955), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",          (int32_t)(x),   aFloat(x),   aFloat(x),   aFloat(x) ); //exp: time: 842,150_450default: rd:TRICE32 int 1089, float 1089.608276 (%f), 44883377, 01000100100010000011001101110111                                         
        break; case __LINE__: TRICE32( ID( 4466), "rd:TRICE32 int %x, float %f (%%f), %08x, %032b\n",            0x44883377,   aFloat(x),   aFloat(x),   aFloat(x) ); //exp: time: 842,150_450default: rd:TRICE32 int 44883377, float 1089.608276 (%f), 44883377, 01000100100010000011001101110111                                         
        break; case __LINE__: TRICE32( ID( 6763), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",                     x,   aFloat(x),   aFloat(x),   aFloat(x) ); //exp: time: 842,150_450default: rd:TRICE32 int 1089, float 1089.608276 (%f), 44883377, 01000100100010000011001101110111                                         
        break; case __LINE__: TRICE32( ID( 6879), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",      FloatToInt32(-x),  aFloat(-x),  aFloat(-x),  aFloat(-x) ); //exp: time: 842,150_450default: rd:TRICE32 int -1089, float -1089.608276 (%f), c4883377, 11000100100010000011001101110111                                       
        break; case __LINE__: TRICE32( ID( 4269), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",         (int32_t)(-x),  aFloat(-x),  aFloat(-x),  aFloat(-x) ); //exp: time: 842,150_450default: rd:TRICE32 int -1089, float -1089.608276 (%f), c4883377, 11000100100010000011001101110111                                       
    //  break; case __LINE__: TRICE32( ID( 5436), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",                    -x,  aFloat(-x),  aFloat(-x),  aFloat(-x) ); //exp: time: 842,150_450default: rd:TRICE32 int 1089, float -1089.608276 (%f), c4883377, 11000100100010000011001101110111                                        
        break; case __LINE__: TRICE32( ID( 4664), "rd:TRICE32 int %X, float %f (%%f), %08x, %032b\n",           -0x44883377,  aFloat(-x),  aFloat(-x),  aFloat(-x) ); //exp: time: 842,150_450default: rd:TRICE32 int BB77CC89, float -1089.608276 (%f), c4883377, 11000100100010000011001101110111                                        
        break; case __LINE__: TRICE64( ID( 1889), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",    DoubleToInt64(y),  aDouble(y),  aDouble(y),  aDouble(y) ); //exp: time: 842,150_450default: rd:TRICE64 int 518, double 518.054749 (%f), 4080307020601050, 0100000010000000001100000111000000100000011000000001000001010000   
        break; case __LINE__: TRICE64( ID( 1902), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",        (int64_t)(y),  aDouble(y),  aDouble(y),  aDouble(y) ); //exp: time: 842,150_450default: rd:TRICE64 int 518, double 518.054749 (%f), 4080307020601050, 0100000010000000001100000111000000100000011000000001000001010000   
        break; case __LINE__: TRICE64( ID( 6644), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",                   y,  aDouble(y),  aDouble(y),  aDouble(y) ); //exp: time: 842,150_450default: rd:TRICE64 int 518, double 518.054749 (%f), 4080307020601050, 0100000010000000001100000111000000100000011000000001000001010000   
        break; case __LINE__: TRICE64( ID( 5057), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",   DoubleToInt64(-y), aDouble(-y), aDouble(-y), aDouble(-y) ); //exp: time: 842,150_450default: rd:TRICE64 int -518, double -518.054749 (%f), c080307020601050, 1100000010000000001100000111000000100000011000000001000001010000
        break; case __LINE__: TRICE64( ID( 2778), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",       (int64_t)(-y), aDouble(-y), aDouble(-y), aDouble(-y) ); //exp: time: 842,150_450default: rd:TRICE64 int -518, double -518.054749 (%f), c080307020601050, 1100000010000000001100000111000000100000011000000001000001010000
    //  break; case __LINE__: TRICE64( ID( 5193), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",                  -y, aDouble(-y), aDouble(-y), aDouble(-y) ); //exp: time: 842,150_450default: rd:TRICE64 int 518, double -518.054749 (%f), c080307020601050, 1100000010000000001100000111000000100000011000000001000001010000 
        break; case __LINE__: TRICE64( ID( 6157), "rd:TRICE64 int %X, double %f (%%f), %016x, %064b\n", -0x4080307020601050, aDouble(-y), aDouble(-y), aDouble(-y) ); //exp: time: 842,150_450default: rd:TRICE64 int BF7FCF8FDF9FEFB0, double -518.054749 (%f), c080307020601050, 1100000010000000001100000111000000100000011000000001000001010000 

        break; case __LINE__: trice16( iD( 2238), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRICE_S( ID( 5601), "msg:With TRICE_S:%s\n", sABCDE );                           //exp: time: 842,150_450default: msg:With TRICE_S:abcde 12345
        break; case __LINE__: TRICE_N( ID( 2080), "sig:With TRICE_N:%s\n", sABCDE, lenABCDE );                 //exp: time: 842,150_450default: sig:With TRICE_N:abcde 12345
        break; case __LINE__: TRICE8_B( ID( 6455), "  %02x", sABCDE, lenABCDE );                               //exp: time: 842,150_450default:   61  62  63  64  65  20  31  32  33  34  35
        break; case __LINE__: TRICE( ID( 4887), "\n" );
        break; case __LINE__: TRICE8_B( ID( 3949), "%4d", sABCDE, lenABCDE );                                  //exp: time: 842,150_450default:   97  98  99 100 101  32  49  50  51  52  53
        break; case __LINE__: TRICE( ID( 4888), "\n" );
        break; case __LINE__: TRICE8_B( ID( 7967), "  %02x", b8, sizeof(b8)/sizeof(int8_t) );                  //exp: time: 842,150_450default:   00  ff  fe  33  04  05  06  07  08  09  0a  0b  00  ff  fe  33  04  05  06  07  08  09  0a  0b
        break; case __LINE__: TRICE( ID( 2893), "\n" );
        break; case __LINE__: TRICE16_B( ID( 3840), "  %04x", b16, sizeof(b16)/sizeof(int16_t) );              //exp: time: 842,150_450default:   0000  ffff  fffe  3344
        break; case __LINE__: TRICE( ID( 6466), "\n" );
        break; case __LINE__: TRICE32_B( ID( 4556), "  %08x", b32, sizeof(b32)/sizeof(int32_t) );              //exp: time: 842,150_450default:   00000000  ffffffff  fffffffe  33445555
        break; case __LINE__: TRICE( ID( 4147), "\n" );
        break; case __LINE__: TRICE64_B( ID( 1794), "  %016x", b64, sizeof(b64)/sizeof(int64_t) );             //exp: time: 842,150_450default:   0000000000000000  ffffffffffffffff  fffffffffffffffe  3344555566666666
        break; case __LINE__: TRICE( ID( 7862), "\n" );
        break; case __LINE__: TRICE8_F( ID( 7587),  "info:FunctionNameW", b8,  sizeof(b8) /sizeof(int8_t) );   //exp: time: 842,150_450default: info:FunctionNameW(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)
        break; case __LINE__: TRICE16_F( ID( 7627), "info:FunctionNameX", b16, sizeof(b16)/sizeof(int16_t) );  //exp: time: 842,150_450default: info:FunctionNameX(0000)(ffff)(fffe)(3344) 
        break; case __LINE__: TRICE32_F( ID( 5914), "info:FunctionNameY", b32, sizeof(b32)/sizeof(int32_t) );  //exp: time: 842,150_450default: info:FunctionNameY(00000000)(ffffffff)(fffffffe)(33445555)
        break; case __LINE__: TRICE64_F( ID( 2173), "info:FunctionNameZ", b64, sizeof(b64)/sizeof(int64_t) );  //exp: time: 842,150_450default: info:FunctionNameZ(0000000000000000)(ffffffffffffffff)(fffffffffffffffe)(3344555566666666)
        break; case __LINE__: exampleOfManualJSONencoding(); //exp: time:       5_654default: att:MyStructEvaluationFunction(json:ExA{Apple:-1, Birn:2, Fisch:2.781000}

        break; case __LINE__: trice16( iD( 7821), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRICE( Id( 3261), "MSG:1/11 = %g\n", aFloat( 1.0/11 ) ); //exp: time:       5_654default: MSG:1/11 = 0.09090909
        break; case __LINE__: { //exp: time: 842,150_450default: msg:x = 5.934 = 5.934, 5.934
                              float a = (float)5.934;
                              float b = a + ((a > 0) ? 0.0005f : -0.0005f);
                              int c = (int)b;
                              int d = (int)(b * 1000) % 1000;
                              int e = (int)(1000 * (float)(a - c)); 
                              TRICE( ID( 1560), "msg:x = %g = %d.%03d, %d.%03d\n", aFloat(a), c, d, c, e ); 
                              }

        //break; case __LINE__: TRICE( id( 2903), "dbg:Hi!\n" );                       //exp: time:            default: dbg:Hi!    
        //break; case __LINE__: TRICE( id( 5166), "dbg:\tHi!\n" );                     
        //break; case __LINE__: TRICE( id( 3922), "dbg:\t\tHi!\n" );                   
        //break; case __LINE__: TRICE( id( 6582), "dbg:\t\t\tHi!\n" );                 
        //break; case __LINE__: TRICE( id( 3036), "dbg:\\tHi!\n" );                    
        //break; case __LINE__: TRICE( Id( 5564), "dbg:\aHi!\n" ); // sound!
        //break; case __LINE__: TRICE( Id( 6149), "dbg:\a\aHi!\n" ); // sound!
        //break; case __LINE__: TRICE( Id( 3141), "dbg:\a\a\aHi!\n" ); // sound!
        //break; case __LINE__: TRICE( Id( 2287), "dbg:\\aHi!\n" );
        break; case __LINE__: TRICE( Id( 2117), "err:An ERROR messgage with sound!\a\n" ); // sound!

        break; case __LINE__: TRICE_S( ID( 3552), "sig:TRICE_S=%s\n", s );            //exp: time: 842,150_450default: sig:TRICE_S=AAAAAAAAAAAA
        break; case __LINE__: TRICE_N( ID( 5769), "sig:TRICE_N=%s\n", s, strlen(s) ); //exp: time: 842,150_450default: sig:TRICE_N=AAAAAAAAAAAA

        break; case __LINE__: trice16( iD( 5861), "att: line %u\n", __LINE__ );

//      break; case __LINE__: 
//  #ifdef XTEA_ENCRYPT_KEY
//          TRICE0 (Id( 4367), "--------------------------------------------------\n" );
//          {
//              uint32_t by[8] = {1,2,3,4,5,6};
//              int count = (sizeof(by) + 7) & ~7; // only multiple of 8 encryptable
//              TRICE32(  Id( 3093), "msg: message = %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
//              TRICE32( Id( 7342), "tim: pre TriceEncryption SysTick=%d\n", SYSTICKVAL );
//              XTEAEncrypt(by, count>>2);
//              TRICE32( Id( 2291), "tim: post TriceEncryption SysTick=%d\n", SYSTICKVAL );
//              TRICE32(  Id( 5146), "att: TriceEncrypted =  %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
//              #ifdef XTEA_DECRYPT
//              TRICE16_1( Id( 7048), "tim: pre TriceDecryption SysTick=%d\n", SYSTICKVAL );
//              XTEADecrypt(by, count>>2);
//              TRICE16_1( Id( 3497), "tim: post TriceDecryption SysTick=%d\n", SYSTICKVAL );
//              #endif
//              TRICE32(  Id( 4093), "msg: messge = %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
//          }
//          TRICE0 (Id( 2388), "--------------------------------------------------\n" );
//  #endif

        break; case __LINE__: trice16( iD( 5131), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRICE( ID( 5886), "sig:Some time measurements\n" );
        break; case __LINE__: TRICE( ID( 2390), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 1956), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 6603), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 6393), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
        break; case __LINE__: TRICE64( ID( 7807), "rd:TRICE64 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE( ID( 6517), "tim:TRICE START time message\n");
        break; case __LINE__: TRICE( ID( 7043), "tim:TRICE STOP time message\n");
        break; case __LINE__: TRICE( ID( 3110), "tim:TRICE START time message\n");
        break; case __LINE__: TRICE( ID( 1024), "tim:TRICE STOP time message\n");
        break; case __LINE__: TRICE( ID( 3044), "tim:TRICE START time message\n");
        break; case __LINE__: TRICE64( ID( 7820), "rd:TRICE64 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE( ID( 7215), "tim:TRICE STOP time message\n");
        break; case __LINE__: TRICE( ID( 4899), "tim:TRICE --------------------------------------------------\n");
        break; case __LINE__: TRICE( ID( 5649), "tim:TRICE --------------------------------------------------\n");
        break; case __LINE__: TRICE64( ID( 3871), "rd:TRICE64 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE( ID( 3712), "tim:TRICE --------------------------------------------------\n");
        break; case __LINE__: TRICE( ID( 4323), "tim:TRICE --------------------------------------------------\n");
        break; case __LINE__: TRICE64( ID( 3661), "rd:TRICE64 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE( ID( 6635), "rd:TRICE32_1 line %d\n", __LINE__ );
        break; case __LINE__: TRICE64_1( ID( 4163), "rd:TRICE64_1 %d\n", __LINE__ );
        break; case __LINE__: TRICE32_2( ID( 1173), "rd:TRICE32_2 line %d,%d\n", __LINE__, 2 );
        break; case __LINE__: TRICE64_2( ID( 1275), "rd:TRICE64_2 line %d,%d\n", __LINE__, 2 );

        break; case __LINE__: trice16( iD( 7791), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRICE( ID( 7724), "sig:Runtime generated strings\n" );
        break; case __LINE__: {
                                  s = "AAAAAAAAAAAA";
                                  TRICE32( ID( 5483), "dbg:len=%u:", strlen(s) );
                                  TRICE_S( ID( 6483), "sig:%s\n", s );
                              }
        break; case __LINE__: {
                s = "\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBCCCCCCCC\
";
                          }
        break; case __LINE__: TRICE32( ID( 3737), "dbg:len=%u:", strlen(s) );
        break; case __LINE__: TRICE_S( ID( 3678), "sig:%s\n", s );
        break; case __LINE__: TRICE( ID( 5923), "sig:Runtime generated strings\n" );
        break; case __LINE__: {
            s = "\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\
DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\
EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\
";
                          }
        break; case __LINE__: TRICE32( ID( 6464), "dbg:len=%u: ", strlen(s) );
        break; case __LINE__: TRICE_S( ID( 1682), "sig:%s\n", s );
        break; case __LINE__: TRICE( ID( 2172), "sig:Runtime generated strings\n" );
        break; case __LINE__: {
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
                          }
        break; case __LINE__: TRICE32( ID( 3265), "dbg:len=%u: ", strlen(s) );
        break; case __LINE__: TRICE_S( ID( 5835), "sig:%s\n", s );
        break; case __LINE__: TRICE( ID( 4977), "sig:Runtime generated strings\n" );
        break; case __LINE__: {
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
                          }
        break; case __LINE__: TRICE32( ID( 2029), "dbg:len=%u: ", strlen(s) );
        break; case __LINE__: TRICE_S( ID( 2016), "sig:%s\n", s );

        break; case __LINE__: trice16( iD( 7307), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRICE( ID( 1228), "sig:Integer (indent, base, sign) - see https://yourbasic.org/golang/fmt-printf-reference-cheat-sheet/\n" );
        break; case __LINE__: TRICE( ID( 1837), "rd: 15 	%d 	Base 10\n", 15 );                                        //ex_: time: 842,150_450default: rd: 15 	15 	Base 10                                   
        break; case __LINE__: TRICE( ID( 3767), "rd: +15 	%+d 	Always show sign\n", 15 );                           //ex_: time: 842,150_450default: rd: +15 	+15 	Always show sign                          
        break; case __LINE__: TRICE( ID( 7840), "rd:   15 	%4d 	Pad with spaces (width 4, right justified)\n", 15 ); //ex_: time: 842,150_450default: rd:   15	  15 	Pad with spaces (width 4, right justified)
        break; case __LINE__: TRICE( ID( 2905), "rd: 15   	%-4d 	Pad with spaces (width 4, left justified)\n", 15 );  //ex_: time: 842,150_450default: rd: 15 	15   	Pad with spaces (width 4, left justified) 
        break; case __LINE__: TRICE( ID( 4734), "rd: 0015 	%04d 	Pad with zeroes (width 4)\n", 15 );                  //ex_: time: 842,150_450default: rd: 0015 	0015 	Pad with zeroes (width 4)                 
        break; case __LINE__: TRICE( ID( 5312), "rd: 1111 	%b 	Base 2\n", 15 );                                         //ex_: time: 842,150_450default: rd: 1111 	1111    Base 2                                    
        break; case __LINE__: TRICE( ID( 2052), "rd: 17 	%o 	Base 8\n", 15 );                                         //ex_: time: 842,150_450default: rd: 17 	17      Base 8                                    
        break; case __LINE__: TRICE( ID( 3323), "rd: f 	%x 	Base 16, lowercase\n", 15 );                             //ex_: time: 842,150_450default: rd: f 	f 	Base 16, lowercase                        
        break; case __LINE__: TRICE( ID( 3507), "rd: F 	%X 	Base 16, uppercase\n", 15 );                             //ex_: time: 842,150_450default: rd: F 	F 	Base 16, uppercase                        
        break; case __LINE__: TRICE( ID( 1410), "rd: 0xf 	%#x 	Base 16, with leading 0x\n", 15 );                   //ex_: time: 842,150_450default: rd: 0xf 	0xf 	Base 16, with leading 0x                  

        break; case __LINE__: trice16( iD( 4403), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRICE( ID( 5174), "sig:Character (quoted, Unicode)\n" );
        break; case __LINE__: TRICE( ID( 7937), "rd: A 	%c 	Character\n", 'A' );                                     //exp: time: 842,150_450default: rd: A 	A 	Character
        break; case __LINE__: TRICE( ID( 1941), "rd: 'A' 	%q 	Quoted character\n", 'A' );                              //exp: time: 842,150_450default: rd: 'A' 	'A' 	Quoted character
    //  break; case __LINE__: TRICE( ID( 3860), "rd: U+0041 	%U 	Unicode\n", 'A' );                        // todo: detect %U to avoid message triceType = TRICE0 ParamSpace = 4 not matching with bitWidth  0 and paramCount 0 - ignoring package
    //  break; case __LINE__: TRICE( ID( 3704), "rd: U+0041 'A' 	%#U 	Unicode with character\n", 'A' ); // todo: detect %#U to avoid message triceType = TRICE0 ParamSpace = 4 not matching with bitWidth  0 and paramCount 0 - ignoring package

        break; case __LINE__: TRICE( ID( 3155), "sig:Boolean (true/false)\n" );
        break; case __LINE__: TRICE( ID( 4930), "rd:Use %%t to format a boolean as true (%t) or false (%t).\n", 1, 0 ); //exp: time: 842,150_450default: rd:Use %t to format a boolean as true (true) or false (false).

        break; case __LINE__: TRICE( ID( 6251), "sig:Pointer (hex)\n" );
        break; case __LINE__: TRICE8 ( ID( 3218), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x84 );               //exp: time: 842,150_450default: rd:Use %p to format a pointer in base 16 notation with leading 0x. (84)              
        break; case __LINE__: TRICE16( ID( 3938), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x1234 );             //exp: time: 842,150_450default: rd:Use %p to format a pointer in base 16 notation with leading 0x. (1234)            
        break; case __LINE__: TRICE32( ID( 7470), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0xaabbccdd );         //exp: time: 842,150_450default: rd:Use %p to format a pointer in base 16 notation with leading 0x. (aabbccdd)        
        break; case __LINE__: TRICE64( ID( 5734), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x1122334455667788 ); //exp: time: 842,150_450default: rd:Use %p to format a pointer in base 16 notation with leading 0x. (1122334455667788)
        break; case __LINE__: TRICE  ( ID( 1529), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0xaabbccdd);          //exp: time: 842,150_450default: rd:Use %p to format a pointer in base 16 notation with leading 0x. (aabbccdd)        

        break; case __LINE__: trice16( iD( 1031), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRICE( ID( 7671), "sig:Float (indent, precision, scientific notation)\n" );
        break; case __LINE__: TRICE( ID( 6238), "rd: 1.234560e+02		%e 	%%e Scientific notation\n", aFloat(123.456) );                       //exp: time: 842,150_450default: rd: 1.234560e+02		1.234560e+02 	%e Scientific notation
        break; case __LINE__: TRICE( ID( 7789), "rd: 123.456000		%f 	%%f Decimal point, no exponent\n", aFloat(123.456) );                //exp: time: 842,150_450default: rd: 123.456000		123.456001 	%f Decimal point, no exponent
        break; case __LINE__: TRICE( ID( 2264), "rd: 123.46			%.2f		%%.2f Default width, precision 2\n", aFloat(123.456) );      //exp: time: 842,150_450default: rd: 123.46			123.46		%.2f Default width, precision 2
        break; case __LINE__: TRICE( ID( 2866), "rd: ␣␣123.46		%8.2f	%%8.2f Width 8, precision 2\n", aFloat(123.456) );                    //exp: time: 842,150_450default: rd: ␣␣123.46		  123.46	%8.2f Width 8, precision 2
        break; case __LINE__: TRICE( ID( 6846), "rd: 123.456		%g		%%g Exponent as needed, necessary digits only\n", aFloat(123.456) ); //exp: time: 842,150_450default: rd: 123.456		123.456		%g Exponent as needed, necessary digits only

        break; case __LINE__: TRICE( ID( 5585), "sig:Double (indent, precision, scientific notation)\n" );
        break; case __LINE__: TRICE64( ID( 4022), "rd: 1.234560e+02		%e 	%%e Scientific notation\n", aDouble(123.456) );                   //exp: time: 842,150_450default: rd: 1.234560e+02		1.234560e+02 	%e Scientific notation
        break; case __LINE__: TRICE64( ID( 4363), "rd: 123.456000		%f 	%%f Decimal point, no exponent\n", aDouble(123.456) );                //exp: time: 842,150_450default: rd: 123.456000		123.456000 	%f Decimal point, no exponent
        break; case __LINE__: TRICE64( ID( 1911), "rd: 123.46			%.2f    	%%.2f Default width, precision 2\n", aDouble(123.456) );      //exp: time: 842,150_450default: rd: 123.46			123.46    	%.2f Default width, precision 2
        break; case __LINE__: TRICE64( ID( 7109), "rd: ␣␣123.46		%8.2f 	%%8.2f Width 8, precision 2\n", aDouble(123.456) );                //exp: time: 842,150_450default: rd: ␣␣123.46		  123.46 	%8.2f Width 8, precision 2
        break; case __LINE__: TRICE64( ID( 3397), "rd: 123.456		%g		%%g Exponent as needed, necessary digits only\n", aDouble(123.456) ); //exp: time: 842,150_450default: rd: 123.456		123.456		%g Exponent as needed, necessary digits only

        break; case __LINE__: TRICE( ID( 5106), "sig:String or byte slice (quote, indent, hex)\n" );
        break; case __LINE__: s = "café"; TRICE_S( ID( 6235), "rd: café 			%s		Plain string\n", s );           //exp: time: 842,150_450default: rd: café 			café		Plain string
        break; case __LINE__: s = "café"; TRICE_S( ID( 3464), "rd: ␣␣café 		%6s 		Width 6, right justify\n", s );  //exp: time: 842,150_450default: rd: ␣␣café 		  café 		Width 6, right justify
        break; case __LINE__: s = "café"; TRICE_S( ID( 2710), "rd: café␣␣ 		%-6s 		Width 6, left justify\n", s );   //exp: time: 842,150_450default: rd: café␣␣ 		café   		Width 6, left justify
        break; case __LINE__: s = "café"; TRICE_S( ID( 3005), "rd: \"café\" 		%q		Quoted string\n", s );          //exp: time: 842,150_450default: rd: \"café\" 		"café"		Quoted string
        break; case __LINE__: s = "café"; TRICE_S( ID( 5761), "rd: 636166c3a9 		%x	Hex dump of byte values\n", s );    //exp: time: 842,150_450default: rd: 636166c3a9 		636166c3a9	Hex dump of byte values 
        break; case __LINE__: s = "café"; TRICE_S( ID( 1628), "rd: 63 61 66 c3 a9 	% x	Hex dump with spaces\n", s );       //exp: time: 842,150_450default: rd: 63 61 66 c3 a9 	63 61 66 c3 a9	Hex dump with spaces

        break; case __LINE__: trice16( iD( 4542), "att: line %u\n", __LINE__ );

#ifdef TRICE_CGO

        break; case __LINE__: TRICE  ( ID( 6909), "rd:%E (%%E)\n",  aFloat(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-5.555556E+08 (%E)
        break; case __LINE__: TRICE  ( ID( 5306), "rd:%F (%%F)\n",  aFloat(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-555555584.000000 (%F)
        break; case __LINE__: TRICE  ( ID( 4872), "rd:%G (%%G)\n",  aFloat(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-5.555556E+08 (%G)
        break; case __LINE__: TRICE64( ID( 1904), "rd:%E (%%E)\n", aDouble(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-5.555556E+08 (%E)
        break; case __LINE__: TRICE64( ID( 1076), "rd:%F (%%F)\n", aDouble(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-555555555.555556 (%F)
        break; case __LINE__: TRICE64( ID( 3723), "rd:%G (%%G)\n", aDouble(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-5.555555555555556E+08 (%G)
        
        break; case __LINE__: TRICE  ( ID( 7805), "rd:%e (%%e)\n",  aFloat(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-5.555556e+08 (%e)
        break; case __LINE__: TRICE  ( ID( 2435), "rd:%f (%%f)\n",  aFloat(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-555555584.000000 (%f)
        break; case __LINE__: TRICE  ( ID( 2318), "rd:%g (%%g)\n",  aFloat(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-5.555556e+08 (%g)
        break; case __LINE__: TRICE64( ID( 4571), "rd:%e (%%e)\n", aDouble(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-5.555556e+08 (%e)
        break; case __LINE__: TRICE64( ID( 2027), "rd:%f (%%f)\n", aDouble(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-555555555.555556 (%f)
        break; case __LINE__: TRICE64( ID( 7889), "rd:%g (%%g)\n", aDouble(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-5.555555555555556e+08 (%g)

        break; case __LINE__: TRICE32( ID( 2883), "msg:%u (%%u)\n", -1 ); //exp: time: 842,150_450default: msg:4294967295 (%u)
        break; case __LINE__: TRICE32( ID( 3505), "msg:%b (%%b)\n", -1 ); //exp: time: 842,150_450default: msg:11111111111111111111111111111111 (%b)
        break; case __LINE__: TRICE32( ID( 3133), "msg:%o (%%o)\n", -1 ); //exp: time: 842,150_450default: msg:37777777777 (%o)
        break; case __LINE__: TRICE32( ID( 3668), "msg:%O (%%O)\n", -1 ); //exp: time: 842,150_450default: msg:0o37777777777 (%O)
        break; case __LINE__: TRICE32( ID( 2684), "msg:%X (%%X)\n", -1 ); //exp: time: 842,150_450default: msg:FFFFFFFF (%X)
        break; case __LINE__: TRICE32( ID( 1206), "msg:%x (%%x)\n", -1 ); //exp: time: 842,150_450default: msg:ffffffff (%x)
        break; case __LINE__: TRICE32( ID( 4976), "msg:%d (%%d)\n", -1 ); //exp: time: 842,150_450default: msg:-1 (%d)
        break; case __LINE__: TRICE64( ID( 6760), "msg:%u (%%u)\n", -1 ); //exp: time: 842,150_450default: msg:18446744073709551615 (%u)
        break; case __LINE__: TRICE64( ID( 4547), "msg:%b (%%b)\n", -1 ); //exp: time: 842,150_450default: msg:1111111111111111111111111111111111111111111111111111111111111111 (%b)
        break; case __LINE__: TRICE64( ID( 1457), "msg:%o (%%o)\n", -1 ); //exp: time: 842,150_450default: msg:1777777777777777777777 (%o)
        break; case __LINE__: TRICE64( ID( 5984), "msg:%O (%%O)\n", -1 ); //exp: time: 842,150_450default: msg:0o1777777777777777777777 (%O)
        break; case __LINE__: TRICE64( ID( 6458), "msg:%X (%%X)\n", -1 ); //exp: time: 842,150_450default: msg:FFFFFFFFFFFFFFFF (%X)
        break; case __LINE__: TRICE64( ID( 3893), "msg:%x (%%x)\n", -1 ); //exp: time: 842,150_450default: msg:ffffffffffffffff (%x)

        //break; case __LINE__: TRice        ( iD( 5941), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        //break; case __LINE__: Trice        ( iD( 3980), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        //break; case __LINE__: trice        ( iD( 2167), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: TRice8       ( iD( 1320), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: Trice8       ( iD( 3162), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: trice8       ( iD( 6059), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: TRice16      ( iD( 7078), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: Trice16      ( iD( 4337), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: trice16      ( iD( 7346), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: TRice32      ( iD( 7925), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: Trice32      ( iD( 5504), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: trice32      ( iD( 7444), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: TRice64      ( iD( 3214), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: Trice64      ( iD( 5755), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: trice64      ( iD( 5416), "value=%d\n", -2  ); //exp: time:            default: value=-2
        
        //break; case __LINE__: TRice_1      ( iD( 6653), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        //break; case __LINE__: Trice_1      ( iD( 1051), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        //break; case __LINE__: trice_1      ( iD( 2603), "value=%d\n", -2  ); //exp: time:            default: value=-2
        //break; case __LINE__: TRice8_1     ( iD( 6948), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        //break; case __LINE__: Trice8_1     ( iD( 3954), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        //break; case __LINE__: trice8_1     ( iD( 2431), "value=%d\n", -2  ); //exp: time:            default: value=-2
        //break; case __LINE__: TRice16_1    ( iD( 1738), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        //break; case __LINE__: Trice16_1    ( iD( 7603), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        //break; case __LINE__: trice16_1    ( iD( 3631), "value=%d\n", -2  ); //exp: time:            default: value=-2
        //break; case __LINE__: TRice32_1    ( iD( 4396), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        //break; case __LINE__: Trice32_1    ( iD( 2637), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        //break; case __LINE__: trice32_1    ( iD( 5794), "value=%d\n", -2  ); //exp: time:            default: value=-2
        //break; case __LINE__: TRice64_1    ( iD( 5961), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        //break; case __LINE__: Trice64_1    ( iD( 4580), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        //break; case __LINE__: trice64_1    ( iD( 1481), "value=%d\n", -2  ); //exp: time:            default: value=-2
        
        //break; case __LINE__: TRice        ( iD( 4553), "no value" ); //exp: time: 842,150_450default: no value
        //break; case __LINE__: Trice        ( iD( 2455), "no value" ); //exp: time:       5_654default: no value
        //break; case __LINE__: trice        ( iD( 3372), "no value" ); //exp: time:            default: no value
        //break; case __LINE__: TRice8       ( iD( 7075), "no value" ); //exp: time: 842,150_450default: no value
        //break; case __LINE__: Trice8       ( iD( 7415), "no value" ); //exp: time:       5_654default: no value
        //break; case __LINE__: trice8       ( iD( 4958), "no value" ); //exp: time:            default: no value
        //break; case __LINE__: TRice16      ( iD( 1853), "no value" ); //exp: time: 842,150_450default: no value
        //break; case __LINE__: Trice16      ( iD( 5756), "no value" ); //exp: time:       5_654default: no value
        //break; case __LINE__: trice16      ( iD( 3111), "no value" ); //exp: time:            default: no value
        //break; case __LINE__: TRice32      ( iD( 2940), "no value" ); //exp: time: 842,150_450default: no value
        //break; case __LINE__: Trice32      ( iD( 1062), "no value" ); //exp: time:       5_654default: no value
        //break; case __LINE__: trice32      ( iD( 2549), "no value" ); //exp: time:            default: no value
        //break; case __LINE__: TRice64      ( iD( 2793), "no value" ); //exp: time: 842,150_450default: no value
        //break; case __LINE__: Trice64      ( iD( 2444), "no value" ); //exp: time:       5_654default: no value
        //break; case __LINE__: trice64      ( iD( 1271), "no value" ); //exp: time:            default: no value
        
        //break; case __LINE__: TRice_0      ( iD( 5797), "no value" ); //exp: time: 842,150_450default: no value
        //break; case __LINE__: Trice_0      ( iD( 4922), "no value" ); //exp: time:       5_654default: no value
        //break; case __LINE__: trice_0      ( iD( 1885), "no value" ); //exp: time:            default: no value
        //break; case __LINE__: TRice8_0     ( iD( 2457), "no value" ); //exp: time: 842,150_450default: no value
        //break; case __LINE__: Trice8_0     ( iD( 1877), "no value" ); //exp: time:       5_654default: no value
        //break; case __LINE__: trice8_0     ( iD( 6447), "no value" ); //exp: time:            default: no value
        //break; case __LINE__: TRice16_0    ( iD( 2958), "no value" ); //exp: time: 842,150_450default: no value
        //break; case __LINE__: Trice16_0    ( iD( 2371), "no value" ); //exp: time:       5_654default: no value
        //break; case __LINE__: trice16_0    ( iD( 2733), "no value" ); //exp: time:            default: no value
        //break; case __LINE__: TRice32_0    ( iD( 6303), "no value" ); //exp: time: 842,150_450default: no value
        //break; case __LINE__: Trice32_0    ( iD( 1286), "no value" ); //exp: time:       5_654default: no value
        //break; case __LINE__: trice32_0    ( iD( 1119), "no value" ); //exp: time:            default: no value
        //break; case __LINE__: TRice64_0    ( iD( 1573), "no value" ); //exp: time: 842,150_450default: no value
        //break; case __LINE__: Trice64_0    ( iD( 3342), "no value" ); //exp: time:       5_654default: no value
        //break; case __LINE__: trice64_0    ( iD( 1873), "no value" ); //exp: time:            default: no value

    //  break; case __LINE__: TRice( iD( 3471),"info:This is a message with one value %d and a 32-bit stamp.\n", -2  ); //exp: time: 842,150_450default: info:This is a message with one value -2 and a 32-bit stamp.
    //  break; case __LINE__: Trice( iD( 5070),"info:This is a message with one value %d and a 16-bit stamp.\n", -2  ); //exp: time:       5_654default: info:This is a message with one value -2 and a 16-bit stamp.
    //  break; case __LINE__: trice( iD( 6127),"info:This is a message with one value %d and without stamp.\n" , -2  ); //exp: time:            default: info:This is a message with one value -2 and without stamp.

        break; case __LINE__: TRICE( ID( 3868), "info:This is a message without values and a 32-bit stamp.\n" );        //exp: time: 842,150_450default: info:This is a message without values and a 32-bit stamp.
        break; case __LINE__: TRICE( Id( 6422), "info:This is a message without values and a 16-bit stamp.\n" );        //exp: time:       5_654default: info:This is a message without values and a 16-bit stamp.
        break; case __LINE__: TRICE( id( 6598), "info:This is a message without values and without stamp.\n"  );        //exp: time:            default: info:This is a message without values and without stamp.

        break; case __LINE__: TRICE( ID( 1836),"info:This is a message with one value %d and a 32-bit stamp.\n", -2  ); //exp: time: 842,150_450default: info:This is a message with one value -2 and a 32-bit stamp.
        break; case __LINE__: TRICE( Id( 4457),"info:This is a message with one value %d and a 16-bit stamp.\n", -2  ); //exp: time:       5_654default: info:This is a message with one value -2 and a 16-bit stamp.
        break; case __LINE__: TRICE( id( 1305),"info:This is a message with one value %d and without stamp.\n" , -2  ); //exp: time:            default: info:This is a message with one value -2 and without stamp.

        break; case __LINE__: TRICE( ID( 2042),"info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 32-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 );   //exp: time: 842,150_450default: info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and a 32-bit stamp.
        break; case __LINE__: TRICE( Id( 5606),"info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 16-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 );   //exp: time:       5_654default: info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and a 16-bit stamp.
        break; case __LINE__: TRICE( id( 2955),"info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and without stamp.\n" , -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 );   //exp: time:            default: info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and without stamp.

        break; case __LINE__: TRICE32( ID( 4920),"info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 32-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //exp: time: 842,150_450default: info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and a 32-bit stamp.
        break; case __LINE__: TRICE32( Id( 2535),"info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 16-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //exp: time:       5_654default: info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and a 16-bit stamp.
        break; case __LINE__: TRICE32( id( 7503),"info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and without stamp.\n" , -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //exp: time:            default: info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and without stamp.

        break; case __LINE__: TRICE8   ( id( 3808), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: TRICE8_1 ( id( 3557), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: TRICE8   ( Id( 1876), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRICE8_1 ( Id( 4720), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRICE8   ( ID( 6683), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: TRICE8_1 ( ID( 6509), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: trice8   ( iD( 1416), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: trice8_1 ( iD( 1409), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: Trice8   ( iD( 2361), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: Trice8_1 ( iD( 3019), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRice8   ( iD( 2508), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: TRice8_1 ( iD( 4740), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200

        break; case __LINE__: TRICE16  ( id( 3102), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: TRICE16_1( id( 4917), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: TRICE16  ( Id( 6010), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRICE16_1( Id( 5443), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRICE16  ( ID( 3210), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: TRICE16_1( ID( 6672), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: trice16  ( iD( 1503), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: trice16_1( iD( 2642), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: Trice16  ( iD( 3154), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: Trice16_1( iD( 3390), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRice16  ( iD( 2775), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: TRice16_1( iD( 6313), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200

        break; case __LINE__: TRICE32  ( id( 3846), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: TRICE32_1( id( 6410), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: TRICE32  ( Id( 3122), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRICE32_1( Id( 5175), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRICE32  ( ID( 3771), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: TRICE32_1( ID( 3910), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: trice32  ( iD( 7852), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: trice32_1( iD( 5361), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: Trice32  ( iD( 1644), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: Trice32_1( iD( 5742), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRice32  ( iD( 3078), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: TRice32_1( iD( 2578), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200

        break; case __LINE__: TRICE64  ( id( 1452), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: TRICE64_1( id( 5889), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: TRICE64  ( Id( 6930), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRICE64_1( Id( 2490), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRICE64  ( ID( 5669), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: TRICE64_1( ID( 1358), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: trice64  ( iD( 2746), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: trice64_1( iD( 7782), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: Trice64  ( iD( 5773), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: Trice64_1( iD( 5427), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRice64  ( iD( 1774), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: TRice64_1( iD( 3814), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200

        // mixed 16-bit and 32-bit timestamp 
        break; case __LINE__: TRice8( iD( 1430), "msg:value=%d\n", -1 );                                                                                            //exp: time: 842,150_450default: msg:value=-1
        break; case __LINE__: Trice8( iD( 4180), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: msg:value=-1, -2
        break; case __LINE__: trice8( iD( 4585), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: msg:value=-1, -2, -3
        break; case __LINE__: TRice8( iD( 4523), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice8( iD( 2590), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice8( iD( 7325), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice8( iD( 7399), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice8( iD( 6593), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice8( iD( 2174), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice8( iD( 7701), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice8( iD( 7061), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice8( iD( 6817), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice16( iD( 3883), "msg:value=%d\n", -1 );                                                                                            //exp: time: 842,150_450default: msg:value=-1
        break; case __LINE__: Trice16( iD( 4000), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: msg:value=-1, -2
        break; case __LINE__: trice16( iD( 4085), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: msg:value=-1, -2, -3
        break; case __LINE__: TRice16( iD( 5577), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice16( iD( 3196), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice16( iD( 2018), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice16( iD( 6499), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice16( iD( 6083), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice16( iD( 2151), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice16( iD( 1067), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice16( iD( 2219), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice16( iD( 6686), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice32( iD( 3957), "msg:value=%d\n", -1 );                                                                                            //exp: time: 842,150_450default: msg:value=-1
        break; case __LINE__: Trice32( iD( 1448), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: msg:value=-1, -2
        break; case __LINE__: trice32( iD( 2085), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: msg:value=-1, -2, -3
        break; case __LINE__: TRice32( iD( 4866), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice32( iD( 2616), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice32( iD( 4413), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice32( iD( 2888), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice32( iD( 7642), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice32( iD( 3366), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice32( iD( 2956), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice32( iD( 6004), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice32( iD( 7943), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice64( iD( 5333), "msg:value=%d\n", -1 );                                                                                            //exp: time: 842,150_450default: msg:value=-1
        break; case __LINE__: Trice64( iD( 4881), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: msg:value=-1, -2
        break; case __LINE__: trice64( iD( 4140), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: msg:value=-1, -2, -3
        break; case __LINE__: TRice64( iD( 3292), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice64( iD( 1829), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice64( iD( 6018), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice64( iD( 3263), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice64( iD( 1664), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice64( iD( 6250), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice64( iD( 7871), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice64( iD( 7552), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice64( iD( 1340), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice8_1 ( iD( 2878), "msg:value=%d\n", -1 );                                                                                            //exp: time: 842,150_450default: msg:value=-1
        break; case __LINE__: Trice8_2 ( iD( 7383), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: msg:value=-1, -2
        break; case __LINE__: trice8_3 ( iD( 3045), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: msg:value=-1, -2, -3
        break; case __LINE__: TRice8_4 ( iD( 5186), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice8_5 ( iD( 7203), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice8_6 ( iD( 6799), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice8_7 ( iD( 2788), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice8_8 ( iD( 7664), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice8_9 ( iD( 7991), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice8_10( iD( 4327), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice8_11( iD( 4226), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice8_12( iD( 2745), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice16_1 ( iD( 4767), "msg:value=%d\n", -1 );                                                                                            //exp: time: 842,150_450default: msg:value=-1
        break; case __LINE__: Trice16_2 ( iD( 6983), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: msg:value=-1, -2
        break; case __LINE__: trice16_3 ( iD( 3152), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: msg:value=-1, -2, -3
        break; case __LINE__: TRice16_4 ( iD( 6497), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice16_5 ( iD( 3567), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice16_6 ( iD( 3409), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice16_7 ( iD( 7564), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice16_8 ( iD( 1025), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice16_9 ( iD( 6735), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice16_10( iD( 4939), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice16_11( iD( 1859), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice16_12( iD( 3375), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice32_1 ( iD( 6691), "msg:value=%d\n", -1 );                                                                                            //exp: time: 842,150_450default: msg:value=-1
        break; case __LINE__: Trice32_2 ( iD( 3322), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: msg:value=-1, -2
        break; case __LINE__: trice32_3 ( iD( 4853), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: msg:value=-1, -2, -3
        break; case __LINE__: TRice32_4 ( iD( 4054), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice32_5 ( iD( 7438), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice32_6 ( iD( 7082), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice32_7 ( iD( 2061), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice32_8 ( iD( 7224), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice32_9 ( iD( 2588), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice32_10( iD( 5125), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice32_11( iD( 6943), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice32_12( iD( 7855), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice64_1 ( iD( 4389), "msg:value=%d\n", -1 );                                                                                            //exp: time: 842,150_450default: msg:value=-1
        break; case __LINE__: Trice64_2 ( iD( 2335), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: msg:value=-1, -2
        break; case __LINE__: trice64_3 ( iD( 3607), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: msg:value=-1, -2, -3
        break; case __LINE__: TRice64_4 ( iD( 1958), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice64_5 ( iD( 5582), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice64_6 ( iD( 6954), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice64_7 ( iD( 1150), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice64_8 ( iD( 7765), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice64_9 ( iD( 4275), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice64_10( iD( 6453), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice64_11( iD( 2820), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice64_12( iD( 2099), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        // use cases for 8-bit data
        break; case __LINE__: trice8     ( iD( 4168), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: Trice8     ( iD( 6232), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRice8     ( iD( 4286), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: trice8_1   ( iD( 7127), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: Trice8_1   ( iD( 4876), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRice8_1   ( iD( 1453), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2

        // use cases for 16-bit data
        break; case __LINE__: trice16    ( iD( 2707), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: Trice16    ( iD( 1781), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRice16    ( iD( 7500), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: trice16_1  ( iD( 3864), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: Trice16_1  ( iD( 3873), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRice16_1  ( iD( 6375), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2

        // use cases for 32-bit data
        break; case __LINE__: trice32    ( iD( 5365), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: Trice32    ( iD( 4261), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRice32    ( iD( 2360), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: trice32_1  ( iD( 6098), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: Trice32_1  ( iD( 7729), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRice32_1  ( iD( 3349), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2

        // use cases for 64-bit data
        break; case __LINE__: trice64    ( iD( 7566), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: Trice64    ( iD( 7174), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRice64    ( iD( 6175), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: trice64_1  ( iD( 2301), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: Trice64_1  ( iD( 6047), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRice64_1  ( iD( 5897), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2

        // legacy use cases
        break; case __LINE__: TRICE      ( ID( 2188),        "value=%d\n", -2 ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: TRICE      ( id( 6221), "value=%d\n", -2 ); //exp: time:            default: value=-2
        break; case __LINE__: TRICE      ( Id( 7483), "value=%d\n", -2 ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRICE      ( ID( 1071), "value=%d\n", -2 ); //exp: time: 842,150_450default: value=-2

        // default stamp
        break; case __LINE__: TRICE8     ( ID( 4834),        "value=%d\n", -2 ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: TRICE8_1   ( ID( 4766),        "value=%d\n", -2 ); //exp: time: 842,150_450default: value=-2

        // no stamp
        break; case __LINE__: trice8     ( iD( 2910),        "value=%d\n", -2 ); //exp: time:            default: value=-2
        break; case __LINE__: trice8_1   ( iD( 3436),        "value=%d\n", -2 ); //exp: time:            default: value=-2
        break; case __LINE__: TRICE8     ( id( 3811), "value=%d\n", -2 ); //exp: time:            default: value=-2
        break; case __LINE__: TRICE8_1   ( id( 1141), "value=%d\n", -2 ); //exp: time:            default: value=-2

        // 16-bit stamp:
        break; case __LINE__: Trice8     ( iD( 7019),        "value=%d\n", -2 ); //exp: time:       5_654default: value=-2
        break; case __LINE__: Trice8_1   ( iD( 1500),        "value=%d\n", -2 ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRICE8     ( Id( 1686), "value=%d\n", -2 ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRICE8_1   ( Id( 1624), "value=%d\n", -2 ); //exp: time:       5_654default: value=-2

        // 32-bit stamp:
        break; case __LINE__: TRice8     ( iD( 6755),        "value=%d\n", -2 ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: TRice8_1   ( iD( 6348),        "value=%d\n", -2 ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: TRICE8     ( ID( 2961), "value=%d\n", -2 ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: TRICE8_1   ( ID( 1021), "value=%d\n", -2 ); //exp: time: 842,150_450default: value=-2

        break; case __LINE__: TRICE( ID( 6207), "FATAL:magenta+b:red\n" );           //exp: time: 842,150_450default: FATAL:magenta+b:red
        break; case __LINE__: TRICE( ID( 6634), "CRITICAL:red+i:default+h\n" );      //exp: time: 842,150_450default: CRITICAL:red+i:default+h
        break; case __LINE__: TRICE( ID( 5064), "EMERGENCY:red+i:blue\n" );          //exp: time: 842,150_450default: EMERGENCY:red+i:blue
        break; case __LINE__: TRICE( ID( 1583), "ERROR:11:red\n" );                  //exp: time: 842,150_450default: ERROR:11:red
        break; case __LINE__: TRICE( ID( 5633), "WARNING:11+i:red\n" );              //exp: time: 842,150_450default: WARNING:11+i:red
        break; case __LINE__: TRICE( ID( 3135), "ATTENTION:11:green\n" );            //exp: time: 842,150_450default: ATTENTION:11:green
        break; case __LINE__: TRICE( ID( 4942), "INFO:cyan+b:default+h\n" );         //exp: time: 842,150_450default: INFO:cyan+b:default+h
        break; case __LINE__: TRICE( ID( 7917), "DEBUG:130+i\n" );                   //exp: time: 842,150_450default: DEBUG:130+i
        break; case __LINE__: TRICE( ID( 2611), "TRACE:default+i:default+h\n" );     //exp: time: 842,150_450default: TRACE:default+i:default+h
        break; case __LINE__: TRICE( ID( 2132), "TIME:blue+i:blue+h\n" );            //exp: time: 842,150_450default: TIME:blue+i:blue+h
        break; case __LINE__: TRICE( ID( 4738), "MESSAGE:green+h:black\n" );         //exp: time: 842,150_450default: MESSAGE:green+h:black
        break; case __LINE__: TRICE( ID( 7854), "READ:black+i:yellow+h\n" );         //exp: time: 842,150_450default: READ:black+i:yellow+h
        break; case __LINE__: TRICE( ID( 1772), "WRITE:black+u:yellow+h\n" );        //exp: time: 842,150_450default: WRITE:black+u:yellow+h
        break; case __LINE__: TRICE( ID( 1816), "RECEIVE:black+h:black\n" );         //exp: time: 842,150_450default: RECEIVE:black+h:black
        break; case __LINE__: TRICE( ID( 1315), "TRANSMIT:black:black+h\n" );        //exp: time: 842,150_450default: TRANSMIT:black:black+h
        break; case __LINE__: TRICE( ID( 3896), "DIAG:yellow+i:default+h\n" );       //exp: time: 842,150_450default: DIAG:yellow+i:default+h
        break; case __LINE__: TRICE( ID( 1948), "INTERRUPT:magenta+i:default+h\n" ); //exp: time: 842,150_450default: INTERRUPT:magenta+i:default+h
        break; case __LINE__: TRICE( ID( 4427), "SIGNAL:118+i\n" );                  //exp: time: 842,150_450default: SIGNAL:118+i
        break; case __LINE__: TRICE( ID( 5510), "TEST:yellow+h:black\n" );           //exp: time: 842,150_450default: TEST:yellow+h:black
        break; case __LINE__: TRICE( ID( 4136), "DEFAULT:off\n" );                   //exp: time: 842,150_450default: DEFAULT:off
        break; case __LINE__: TRICE( ID( 5132), "NOTICE:blue:white+h\n" );           //exp: time: 842,150_450default: NOTICE:blue:white+h
        break; case __LINE__: TRICE( ID( 7752), "ALERT:magenta:magenta+h\n" );       //exp: time: 842,150_450default: ALERT:magenta:magenta+h
        break; case __LINE__: TRICE( ID( 6269), "ASSERT:yellow+i:blue\n" );          //exp: time: 842,150_450default: ASSERT:yellow+i:blue
        break; case __LINE__: TRICE( ID( 3695), "ALARM:red+i:white+h\n" );           //exp: time: 842,150_450default: ALARM:red+i:white+h
        break; case __LINE__: TRICE( ID( 5627), "CYCLE:blue+i:default+h\n" );        //exp: time: 842,150_450default: CYCLE:blue+i:default+h
        break; case __LINE__: TRICE( ID( 1105), "VERBOSE:blue:default\n" );          //exp: time: 842,150_450default: VERBOSE:blue:default
        break; case __LINE__: TRICE( ID( 3788), "fatal:magenta+b:red\n" );           //exp: time: 842,150_450default: fatal:magenta+b:red
        break; case __LINE__: TRICE( ID( 3216), "critical:red+i:default+h\n" );      //exp: time: 842,150_450default: critical:red+i:default+h
        break; case __LINE__: TRICE( ID( 4875), "emergency:red+i:blue\n" );          //exp: time: 842,150_450default: emergency:red+i:blue
        break; case __LINE__: TRICE( ID( 6899), "error:11:red\n" );                  //exp: time: 842,150_450default: error:11:red
        break; case __LINE__: TRICE( ID( 5019), "warning:11+i:red\n" );              //exp: time: 842,150_450default: warning:11+i:red
        break; case __LINE__: TRICE( ID( 2376), "attention:11:green\n" );            //exp: time: 842,150_450default: attention:11:green
        break; case __LINE__: TRICE( ID( 5920), "info:cyan+b:default+h\n" );         //exp: time: 842,150_450default: info:cyan+b:default+h
        break; case __LINE__: TRICE( ID( 1039), "debug:130+i\n" );                   //exp: time: 842,150_450default: debug:130+i
        break; case __LINE__: TRICE( ID( 2771), "trace:default+i:default+h\n" );     //exp: time: 842,150_450default: trace:default+i:default+h
        break; case __LINE__: TRICE( ID( 2352), "time:blue+i:blue+h\n" );            //exp: time: 842,150_450default: time:blue+i:blue+h
        break; case __LINE__: TRICE( ID( 6271), "message:green+h:black\n" );         //exp: time: 842,150_450default: message:green+h:black
        break; case __LINE__: TRICE( ID( 1125), "read:black+i:yellow+h\n" );         //exp: time: 842,150_450default: read:black+i:yellow+h
        break; case __LINE__: TRICE( ID( 5195), "write:black+u:yellow+h\n" );        //exp: time: 842,150_450default: write:black+u:yellow+h
        break; case __LINE__: TRICE( ID( 6052), "receive:black+h:black\n" );         //exp: time: 842,150_450default: receive:black+h:black
        break; case __LINE__: TRICE( ID( 7813), "transmit:black:black+h\n" );        //exp: time: 842,150_450default: transmit:black:black+h
        break; case __LINE__: TRICE( ID( 2689), "diag:yellow+i:default+h\n" );       //exp: time: 842,150_450default: diag:yellow+i:default+h
        break; case __LINE__: TRICE( ID( 4206), "interrupt:magenta+i:default+h\n" ); //exp: time: 842,150_450default: interrupt:magenta+i:default+h
        break; case __LINE__: TRICE( ID( 4955), "signal:118+i\n" );                  //exp: time: 842,150_450default: signal:118+i
        break; case __LINE__: TRICE( ID( 6315), "test:yellow+h:black\n" );           //exp: time: 842,150_450default: test:yellow+h:black
        break; case __LINE__: TRICE( ID( 1849), "default:off\n" );                   //exp: time: 842,150_450default: default:off
        break; case __LINE__: TRICE( ID( 3274), "notice:blue:white+h\n" );           //exp: time: 842,150_450default: notice:blue:white+h
        break; case __LINE__: TRICE( ID( 4064), "alert:magenta:magenta+h\n" );       //exp: time: 842,150_450default: alert:magenta:magenta+h
        break; case __LINE__: TRICE( ID( 5383), "assert:yellow+i:blue\n" );          //exp: time: 842,150_450default: assert:yellow+i:blue
        break; case __LINE__: TRICE( ID( 6536), "alarm:red+i:white+h\n" );           //exp: time: 842,150_450default: alarm:red+i:white+h
        break; case __LINE__: TRICE( ID( 3447), "cycle:blue+i:default+h\n" );        //exp: time: 842,150_450default: cycle:blue+i:default+h
        break; case __LINE__: TRICE( ID( 7316), "verbose:blue:default\n" );          //exp: time: 842,150_450default: verbose:blue:default
        break; case __LINE__: TRICE64( id( 2445), "msg:%d (%%d)\n", -1 );     //exp: time:            default: msg:-1 (%d)

        break; case __LINE__: TRICE( ID( 5292), "sig:TRICE with 1 to 12 values\n" );
        break; case __LINE__: TRICE( ID( 2652), "rd:TRICE %d\n", -1 );
        break; case __LINE__: TRICE( ID( 1641), "rd:TRICE %d, %d\n", -1, -2 );
        break; case __LINE__: TRICE( ID( 1812), "rd:TRICE %d, %d, %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE( ID( 4158), "rd:TRICE %d, %d, %d, %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE( ID( 7448), "rd:TRICE %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE( ID( 1706), "rd:TRICE %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE( ID( 5388), "rd:TRICE %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE( ID( 7945), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE( ID( 4368), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE( ID( 7995), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE( ID( 5077), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE( ID( 6894), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE( ID( 7160),  "sig:TRICE_n with 1 to 12 values\n" );
        break; case __LINE__: TRICE_1( ID( 6026), "rd:TRICE_1 %d\n", 1 );
        break; case __LINE__: TRICE_2( ID( 7147), "rd:TRICE_2 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE_3( ID( 3205), "rd:TRICE_3 %d, %d, %d\n", 1, 2, 3 );
        break; case __LINE__: TRICE_4( ID( 2190), "rd:TRICE_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
        break; case __LINE__: TRICE_5( ID( 7251), "rd:TRICE_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
        break; case __LINE__: TRICE_6( ID( 6908), "rd:TRICE_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
        break; case __LINE__: TRICE_7( ID( 4283), "rd:TRICE_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
        break; case __LINE__: TRICE_8( ID( 4232), "rd:TRICE_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
        break; case __LINE__: TRICE_9( ID( 2664), "rd:TRICE_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
        break; case __LINE__: TRICE_10( ID( 6580), "rd:TRICE_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
        break; case __LINE__: TRICE_11( ID( 5484), "rd:TRICE_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
        break; case __LINE__: TRICE_12( ID( 4753), "rd:TRICE_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );				

        break; case __LINE__: TRICE( ID( 2077), "sig:TRICE8 with 1 to 12 values\n" );
        break; case __LINE__: TRICE8( ID( 3379), "rd:TRICE8 %d\n", -1 );
        break; case __LINE__: TRICE8( ID( 7402), "rd:TRICE8 %d, %d\n", -1, -2 );
        break; case __LINE__: TRICE8( ID( 6538), "rd:TRICE8 %d, %d, %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE8( ID( 1368), "rd:TRICE8 %d, %d, %d, %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE8( ID( 1277), "rd:TRICE8 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE8( ID( 2699), "rd:TRICE8 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE8( ID( 6978), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE8( ID( 6897), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE8( ID( 5323), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE8( ID( 1102), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE8( ID( 3209), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE8( ID( 5685), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE( ID( 4650), "sig:TRICE8_n with 1 to 12 values\n" );
        break; case __LINE__: TRICE8_1 ( ID( 4470), "rd:TRICE8_1 %d\n", 1 );
        break; case __LINE__: TRICE8_2 ( ID( 2724), "rd:TRICE8_2 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE8_3 ( ID( 7070), "rd:TRICE8_3 %d, %d, %d\n", 1, 2, 3 );
        break; case __LINE__: TRICE8_4 ( ID( 6420), "rd:TRICE8_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
        break; case __LINE__: TRICE8_5 ( ID( 5217), "rd:TRICE8_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
        break; case __LINE__: TRICE8_6 ( ID( 2474), "rd:TRICE8_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
        break; case __LINE__: TRICE8_7 ( ID( 7331), "rd:TRICE8_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
        break; case __LINE__: TRICE8_8 ( ID( 2786), "rd:TRICE8_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
        break; case __LINE__: TRICE8_9 ( ID( 4516), "rd:TRICE8_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
        break; case __LINE__: TRICE8_10( ID( 4015), "rd:TRICE8_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
        break; case __LINE__: TRICE8_11( ID( 6947), "rd:TRICE8_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
        break; case __LINE__: TRICE8_12( ID( 6448), "rd:TRICE8_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );

        break; case __LINE__: TRICE( ID( 6992), "sig:TRICE16 with 1 to 12 values\n" );
        break; case __LINE__: TRICE16( ID( 5934), "rd:TRICE16 %d\n", -1 );
        break; case __LINE__: TRICE16( ID( 7510), "rd:TRICE16 %d, %d\n", -1, -2 );
        break; case __LINE__: TRICE16( ID( 6590), "rd:TRICE16 %d, %d, %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE16( ID( 7958), "rd:TRICE16 %d, %d, %d, %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE16( ID( 6613), "rd:TRICE16 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE16( ID( 1373), "rd:TRICE16 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE16( ID( 3999), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE16( ID( 4841), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE16( ID( 1313), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE16( ID( 3284), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE16( ID( 5247), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE16( ID( 7254), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE     ( ID( 4722), "sig:TRICE16_n with 1 to 12 values\n" );
        break; case __LINE__: TRICE16_1 ( ID( 7848), "rd:TRICE16_1 %d\n", 1 );
        break; case __LINE__: TRICE16_2 ( ID( 3863), "rd:TRICE16_2 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE16_3 ( ID( 6911), "rd:TRICE16_3 %d, %d, %d\n", 1, 2, 3 );
        break; case __LINE__: TRICE16_4 ( ID( 5212), "rd:TRICE16_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
        break; case __LINE__: TRICE16_5 ( ID( 4993), "rd:TRICE16_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
        break; case __LINE__: TRICE16_6 ( ID( 1138), "rd:TRICE16_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
        break; case __LINE__: TRICE16_7 ( ID( 1282), "rd:TRICE16_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
        break; case __LINE__: TRICE16_8 ( ID( 2025), "rd:TRICE16_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
        break; case __LINE__: TRICE16_9 ( ID( 1413), "rd:TRICE16_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
        break; case __LINE__: TRICE16_10( ID( 2899), "rd:TRICE16_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
        break; case __LINE__: TRICE16_11( ID( 4929), "rd:TRICE16_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
        break; case __LINE__: TRICE16_12( ID( 7236), "rd:TRICE16_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );

        break; case __LINE__: TRICE  ( ID( 6270), "sig:TRICE16 with 1 to 12 pointer\n" );
        break; case __LINE__: TRICE16( ID( 4425), "rd:TRICE16 %p\n", -1 );
        break; case __LINE__: TRICE16( ID( 6941), "rd:TRICE16 %p, %p\n", -1, -2 );
        break; case __LINE__: TRICE16( ID( 1061), "rd:TRICE16 %p, %p, %p\n", -1, -2, -3 );
        break; case __LINE__: TRICE16( ID( 2179), "rd:TRICE16 %p, %p, %p, %p\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE16( ID( 7942), "rd:TRICE16 %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE16( ID( 7420), "rd:TRICE16 %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE16( ID( 1668), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE16( ID( 6591), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE16( ID( 1533), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE16( ID( 6134), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE16( ID( 3079), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE16( ID( 6091), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE  ( ID( 2408), "sig:TRICE16 with 1 to 12 hex\n" );
        break; case __LINE__: TRICE16( ID( 5720), "rd:TRICE16 %X\n", -1 );
        break; case __LINE__: TRICE16( ID( 5038), "rd:TRICE16 %X, %X\n", -1, -2 );
        break; case __LINE__: TRICE16( ID( 4870), "rd:TRICE16 %X, %X, %X\n", -1, -2, -3 );
        break; case __LINE__: TRICE16( ID( 6264), "rd:TRICE16 %X, %X, %X, %X\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE16( ID( 6822), "rd:TRICE16 %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE16( ID( 4058), "rd:TRICE16 %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE16( ID( 4213), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE16( ID( 5503), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE16( ID( 3714), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE16( ID( 2845), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE16( ID( 3492), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE16( ID( 6259), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE  ( ID( 5722), "sig:TRICE32 with 1 to 12 values\n" );
        break; case __LINE__: TRICE32( ID( 5998), "rd:TRICE32 %d\n", -1 );
        break; case __LINE__: TRICE32( ID( 2649), "rd:TRICE32 %d, %d\n", -1, -2 );
        break; case __LINE__: TRICE32( ID( 2098), "rd:TRICE32 %d, %d, %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE32( ID( 1469), "rd:TRICE32 %d, %d, %d, %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE32( ID( 4985), "rd:TRICE32 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE32( ID( 4126), "rd:TRICE32 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE32( ID( 7993), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE32( ID( 1442), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE32( ID( 2748), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE32( ID( 6677), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE32( ID( 7631), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE32( ID( 2885), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE     ( ID( 7168), "signal:TRICE32_n with 1 to 12 values\n" );
        break; case __LINE__: TRICE32_1 ( ID( 2692), "rd:TRICE32_1 %d\n", 1 );
        break; case __LINE__: TRICE32_2 ( ID( 6301), "rd:TRICE32_2 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE32_3 ( ID( 5578), "rd:TRICE32_3 %d, %d, %d\n", 1, 2, 3 );
        break; case __LINE__: TRICE32_4 ( ID( 2914), "rd:TRICE32_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
        break; case __LINE__: TRICE32_5 ( ID( 6845), "rd:TRICE32_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
        break; case __LINE__: TRICE32_6 ( ID( 7091), "rd:TRICE32_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
        break; case __LINE__: TRICE32_7 ( ID( 3476), "rd:TRICE32_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
        break; case __LINE__: TRICE32_8 ( ID( 5834), "rd:TRICE32_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
        break; case __LINE__: TRICE32_9 ( ID( 4582), "rd:TRICE32_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
        break; case __LINE__: TRICE32_10( ID( 7639), "rd:TRICE32_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
        break; case __LINE__: TRICE32_11( ID( 1639), "rd:TRICE32_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
        break; case __LINE__: TRICE32_12( ID( 6457), "rd:TRICE32_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );

        break; case __LINE__: TRICE  ( ID( 2134), "sig:TRICE64 with 1 to 12 values\n" );
        break; case __LINE__: TRICE64( ID( 6310), "rd:TRICE64 %d\n", -1 );
        break; case __LINE__: TRICE64( ID( 1727), "rd:TRICE64 %d, %d\n", -1, -2 );
        break; case __LINE__: TRICE64( ID( 5238), "rd:TRICE64 %d, %d, %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE64( ID( 5868), "rd:TRICE64 %d, %d, %d, %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE64( ID( 4346), "rd:TRICE64 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE64( ID( 5342), "rd:TRICE64 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE64( ID( 1556), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE64( ID( 2634), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE64( ID( 4183), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE64( ID( 2438), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE64( ID( 4682), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE64( ID( 1820), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE     ( ID( 1234), "sig:TRICE64_n with 1 to 12 values\n" );
        break; case __LINE__: TRICE64_1 ( ID( 2353), "rd:TRICE64_1 %d\n", 1 );
        break; case __LINE__: TRICE64_2 ( ID( 3437), "rd:TRICE64_2 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE64_2 ( ID( 4408), "rd:TRICE64_2 %x, %x\n", 0x1122334455667788, 0x0102030405060708 );
        break; case __LINE__: TRICE64_3 ( ID( 6467), "rd:TRICE64_3 %d, %d, %d\n", 1, 2, 3 );
        break; case __LINE__: TRICE64_4 ( ID( 5845), "rd:TRICE64_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
        break; case __LINE__: TRICE64_5 ( ID( 7700), "rd:TRICE64_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
        break; case __LINE__: TRICE64_6 ( ID( 6539), "rd:TRICE64_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
        break; case __LINE__: TRICE64_7 ( ID( 5558), "rd:TRICE64_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
        break; case __LINE__: TRICE64_8 ( ID( 7144), "rd:TRICE64_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
        break; case __LINE__: TRICE64_9 ( ID( 5497), "rd:TRICE64_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
        break; case __LINE__: TRICE64_10( ID( 2283), "rd:TRICE64_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
        break; case __LINE__: TRICE64_11( ID( 6475), "rd:TRICE64_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
        break; case __LINE__: TRICE64_12( ID( 6213), "rd:TRICE64_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );

        break; case __LINE__: TRICE( ID( 3404), "sig:Colors and ticks\n" );
        break; case __LINE__: TRICE( ID( 4026), "--------------------------------------------------\n");
        break; case __LINE__: TRICE( ID( 6878), "--------------------------------------------------\n");
        break; case __LINE__: TRICE( ID( 4325), "dbg:12345 as 16bit is %016b\n", 12345);
        break; case __LINE__: TRICE( ID( 5419), "--------------------------------------------------\n");
        break; case __LINE__: TRICE( ID( 5105), "sig:This ASSERT error is just a demo and no real error:\n");
        break; case __LINE__: TRICE( ID( 7514), "--------------------------------------------------\n");
        break; case __LINE__: TRICE( ID( 6851), "ERR:error       message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 7036), "WRN:warning     message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 6548), "ATT:attention   message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 2992), "DIA:diagnostics message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 1405), "TIM:timing      message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 7934), "DBG:debug       message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 3564), "SIG:signal      message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 3438), "RD:read         message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 3244), "WR:write        message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 5097), "ISR:interrupt   message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 1992), "MSG:normal      message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 1905), "INFO:informal   message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 7826), "time:TRICE32_1  message, SysTick is %6u\n", SYSTICKVAL);

        break; case __LINE__: TRICE( ID( 5664), "sig:Various ranges and formats\n" );
        break; case __LINE__: TRICE8 ( ID( 3733), "tst:TRICE8  %%03x -> %03x  %03x  %03x  %03x\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID( 6165), "tst:TRICE8   %%4d -> %4d %4d %4d %4d\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID( 4602), "tst:TRICE8   %%4u -> %4u %4u %4u %4u\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID( 6790), "tst:TRICE8   %%4o -> %4o %4o %4o %4o\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID( 5652), "tst:TRICE8   %%4O -> %4O %4O %4O %4O\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID( 5672), "tst:TRICE8   %%4X -> %4X %4X %4X %4X\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID( 7534), "tst:TRICE8   %%4x -> %4x %4x %4x %4x\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID( 2751), "tst:TRICE8   %%8b -> %8b %8b %8b %8b\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID( 4897), "tst:TRICE8  %%08b -> %08b %08b %08b %08b\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE16( ID( 7589), "tst:TRICE16  %%05x ->   %05x   %05x   %05x   %05x\n", 1, 0x7fff, 0x8000, 0xffff);
        break; case __LINE__: TRICE16( ID( 4331), "tst:TRICE16   %%6d ->  %6d  %6d  %6d  %6d\n",         1, 0x7fff, 0x8000, 0xffff);
        break; case __LINE__: TRICE16( ID( 2553), "tst:TRICE16   %%7o -> %7o %7o %7o %7o\n",             1, 0x7fff, 0x8000, 0xffff);
        break; case __LINE__: TRICE32( ID( 1361), "tst:TRICE32 %%09x ->      %09x      %09x       %09x     %09x\n", 1, 0x7fffffff, 0x80000000, 0xffffffff);
        break; case __LINE__: TRICE32( ID( 1483), "tst:TRICE32 %%10d ->     %10d     %10d     %10d    %10x\n",      1, 0x7fffffff, 0x80000000, 0xffffffff);
        break; case __LINE__: TRICE32( ID( 4518), "att:TRICE32 %40b\n", 0xAAAAAAAA);
        break; case __LINE__: TRICE64( ID( 7104), "tst:TRICE64 %%09x ->      %09x      %09x       %09x     %09x\n", 1, 0x7fffffffffffffff, 0x8000000000000000, 0xffffffffffffffff);
        break; case __LINE__: TRICE64( ID( 4487), "tst:TRICE64 %%10d ->     %10d     %10d     %10d    %10x\n",      1, 0x7fffffffffffffff, 0x8000000000000000, 0xffffffffffffffff);
        break; case __LINE__: TRICE64( ID( 5310), "att:TRICE64 %70b\n", 0xAAAAAAAAAAAAAAAA);
        break; case __LINE__: TRICE( ID( 4500), "sig:Legacy TRICE8\n" );
        break; case __LINE__: TRICE8_1( ID( 5120), "tst:TRICE8_1 %02x\n", 0xA1);
        break; case __LINE__: TRICE8_2( ID( 1090), "tst:TRICE8_2 %02x %02x\n", 0xA1, 0xA2);
        break; case __LINE__: TRICE8_3( ID( 6916), "tst:TRICE8_3 %02x %02x %02x\n", 0xA1, 0xA2, 0xA3);
        break; case __LINE__: TRICE8_4( ID( 1387), "tst:TRICE8_4 %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4);
        break; case __LINE__: TRICE8_5( ID( 7161), "tst:TRICE8_5 %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5 );
        break; case __LINE__: TRICE8_6( ID( 3855), "tst:TRICE8_6 %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6);
        break; case __LINE__: TRICE8_7( ID( 2082), "tst:TRICE8_7 %02x %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7);
        break; case __LINE__: TRICE8_8( ID( 6866), "tst:TRICE8_8 %02x %02x %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8);
        break; case __LINE__: TRICE8_1( ID( 2619), "tst:TRICE8_1 %d\n",                      -1);
        break; case __LINE__: TRICE8_2( ID( 5237), "tst:TRICE8_2 %d %d\n",                   -1, -2);
        break; case __LINE__: TRICE8_3( ID( 1443), "tst:TRICE8_3 %d %d %d\n",                -1, -2, -3);
        break; case __LINE__: TRICE8_4( ID( 3600), "tst:TRICE8_4 %d %d %d %d\n",             -1, -2, -3, -4);
        break; case __LINE__: TRICE8_5( ID( 4360), "tst:TRICE8_5 %d %d %d %d %d\n",          -1, -2, -3, -4, -5);
        break; case __LINE__: TRICE8_6( ID( 7433), "tst:TRICE8_6 %d %d %d %d %d %d\n",       -1, -2, -3, -4, -5, -6);
        break; case __LINE__: TRICE8_7( ID( 7533), "tst:TRICE8_7 %d %d %d %d %d %d %d\n",    -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE8_8( ID( 2742), "tst:TRICE8_8 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8);
        break; case __LINE__: TRICE8_1( ID( 3885), "tst:TRICE8_1 %d\n", 1);
        break; case __LINE__: TRICE8_2( ID( 4802), "tst:TRICE8_2 %d %d\n", 1, 2);
        break; case __LINE__: TRICE8_3( ID( 2815), "tst:TRICE8_3 %d %d %d\n", 1, 2, 3);
        break; case __LINE__: TRICE8_4( ID( 4566), "tst:TRICE8_4 %d %d %d %d\n", 1, 2, 3, 4);
        break; case __LINE__: TRICE8_5( ID( 1971), "tst:TRICE8_5 %d %d %d %d %d\n", 1, 2, 3, 4, 5);
        break; case __LINE__: TRICE8_6( ID( 7124), "tst:TRICE8_6 %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6);
        break; case __LINE__: TRICE8_7( ID( 4858), "tst:TRICE8_7 %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7);
        break; case __LINE__: TRICE8_8( ID( 4505), "tst:TRICE8_8 %d %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7, 8);
        break; case __LINE__: TRICE8_1( ID( 3512), "tst:TRICE8_1 %u\n", 201);
        break; case __LINE__: TRICE8_2( ID( 4690), "tst:TRICE8_2 %u %u\n", 201, 202);
        break; case __LINE__: TRICE8_3( ID( 7462), "tst:TRICE8_3 %u %u %u\n", 201, 202, 203);
        break; case __LINE__: TRICE8_4( ID( 1549), "tst:TRICE8_4 %u %u %u %u\n", 201, 202, 203, 204);
        break; case __LINE__: TRICE8_5( ID( 3845), "tst:TRICE8_5 %u %u %u %u %u\n", 201, 202, 203, 204, 205);
        break; case __LINE__: TRICE8_6( ID( 1417), "tst:TRICE8_6 %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206);
        break; case __LINE__: TRICE8_7( ID( 5808), "tst:TRICE8_7 %u %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206, 207);
        break; case __LINE__: TRICE8_8( ID( 1892), "tst:TRICE8_8 %u %u %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206, 207, 208);
        break; case __LINE__: TRICE8_8( ID( 1840), "tst:TRICE8_1 %%d=%d, %%u=%u, 0x%%x=0x%x, 0x%%2x=0x%2x, 0x%%02x=0x%02x, 0x%%3x=0x%3x, 0x%%03x=0x%03x, %%b=%b\n", 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81);

        break; case __LINE__: TRICE    ( ID( 4176), "sig:Legacy TRICE16-64\n" );
        break; case __LINE__: TRICE16_1( ID( 5468), "tst:TRICE16_1 %d\n", -111);
        break; case __LINE__: TRICE16_2( ID( 2847), "tst:TRICE16_2 %d %d\n", -111, -222);
        break; case __LINE__: TRICE16_3( ID( 2157), "tst:TRICE16_3 %d %d %d\n", -111, -222, -333);
        break; case __LINE__: TRICE16_4( ID( 7707), "tst:TRICE16_4 %d %d %d %d\n", -111, -222, -333, -444);
        break; case __LINE__: TRICE32_1( ID( 2443), "tst:TRICE32_1 %08x\n", 0x0123cafe);
        break; case __LINE__: TRICE32_1( ID( 6119), "tst:TRICE32_1 %d\n", -111);
        break; case __LINE__: TRICE32_2( ID( 7079), "tst:TRICE32_2 %x %x\n", -111, -222);
        break; case __LINE__: TRICE32_2( ID( 5054), "tst:TRICE32_2 %d %d\n", -111, -222);
        break; case __LINE__: TRICE32_3( ID( 1925), "tst:TRICE32_3 %x %x %x\n", -111, -222, -333);
        break; case __LINE__: TRICE32_3( ID( 6712), "tst:TRICE32_3 %d %d %d\n", -111, -222, -333);
        break; case __LINE__: TRICE32_4( ID( 7088), "tst:TRICE32_4 %x %x %x %x\n", -111, -222, -333, -444);
        break; case __LINE__: TRICE32_4( ID( 6336), "tst:TRICE32_4 %d %d %d %d\n", -111, -222, -333, -444);
        break; case __LINE__: TRICE64_1( ID( 3533), "tst:TRICE64_1 %d\n", -111);
        break; case __LINE__: TRICE64_2( ID( 7833), "tst:TRICE64_2 %d %d\n", -111, -222);
        break; case __LINE__: TRICE16_1( ID( 5393), "tst:TRICE16_1 %u\n", 60001);
        break; case __LINE__: TRICE16_2( ID( 1496), "tst:TRICE16_2 %u %u\n", 60001, 60002);
        break; case __LINE__: TRICE16_3( ID( 7436), "tst:TRICE16_3 %u %u %u\n", 60001, 60002, 60003);
        break; case __LINE__: TRICE16_4( ID( 3905), "tst:TRICE16_4 %u %u %u %u\n", 60001, 60002, 60003, 60004);
        break; case __LINE__: TRICE32_1( ID( 5300), "tst:TRICE32_1 %u\n", 4000000001);
        break; case __LINE__: TRICE32_2( ID( 7904), "tst:TRICE32_2 %u %u\n", 4000000001, 4000000002);
        break; case __LINE__: TRICE32_3( ID( 5537), "tst:TRICE32_3 %u %u %u\n", 4000000001, 4000000002, 4000000003);
        break; case __LINE__: TRICE32_4( ID( 7593), "tst:TRICE32_4 %u %u %u %u\n", 4000000001, 4000000002, 4000000003, 4000000004);
        break; case __LINE__: TRICE64_1( ID( 1973), "tst:TRICE64_1 %x\n", -1);
        break; case __LINE__: TRICE64_2( ID( 7332), "tst:TRICE64_2 %x %x\n", -1, -2);
        break; case __LINE__: TRICE64_1( ID( 3732), "tst:TRICE64_1 %u\n", -1);
        break; case __LINE__: TRICE64_2( ID( 7768), "tst:TRICE64_2 %u %u\n", -1, -2);

        break; case __LINE__: TRICE( ID( 1221), "sig:colored single letters and several TRICE macros in one line\n" );
        break; case __LINE__: TRICE( ID( 7800), "e:A");
        break; case __LINE__: TRICE( ID( 4376), "w:B");
        break; case __LINE__: TRICE( ID( 3710), "a:c");
        break; case __LINE__: TRICE( ID( 1685), "wr:d");
        break; case __LINE__: TRICE( ID( 1775), "rd:e\n");
        break; case __LINE__: TRICE( ID( 6820), "diag:f");
        break; case __LINE__: TRICE( ID( 2017), "d:G");
        break; case __LINE__: TRICE( ID( 2823), "t:H");
        break; case __LINE__: TRICE( ID( 6713), "time:i");
        break; case __LINE__: TRICE( ID( 5313), "message:J");
        break; case __LINE__: TRICE( ID( 6606), "dbg:k\n");
        break; case __LINE__: TRICE( ID( 5904), "1");
        break; case __LINE__: TRICE( ID( 6959), "2");
        break; case __LINE__: TRICE( ID( 3239), "3");
        break; case __LINE__: TRICE( ID( 3180), "4");
        break; case __LINE__: TRICE( ID( 2970), "e:7");
        break; case __LINE__: TRICE( ID( 5363), "m:12");
        break; case __LINE__: TRICE( ID( 3574), "m:123\n");
        break; case __LINE__: TRICE( ID( 1709), "e:A"); TRICE( ID( 4708), "w:B"); TRICE( ID( 4252), "a:c");
        break; case __LINE__: TRICE( ID( 2369), "wr:d"); TRICE( ID( 2548), "rd:e\n"); TRICE( ID( 4499), "diag:f");

        break; case __LINE__: TRICE( ID( 4948),  "sig:TRICE8 with variable param count 1 to 12\n" );
        break; case __LINE__: TRICE8( ID( 7578), "tst:TRICE8 %d\n", -1 );
        break; case __LINE__: TRICE8( ID( 2183), "tst:TRICE8 %d %d\n", -1, -2 );
        break; case __LINE__: TRICE8( ID( 4154), "tst:TRICE8 %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE8( ID( 2350), "tst:TRICE8 %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE8( ID( 3324), "tst:TRICE8 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE8( ID( 2091), "tst:TRICE8 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE8( ID( 5061), "tst:TRICE8 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE8( ID( 4062), "tst:TRICE8 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE8( ID( 3441), "tst:TRICE8 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE8( ID( 6761), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE8( ID( 3763), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE8( ID( 7138), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE8( ID( 1620), "tst:TRICE8 %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );
        break; case __LINE__: TRICE8_1 ( ID( 5566), "tst:TRICE8_1  %d\n", -1 );
        break; case __LINE__: TRICE8_2 ( ID( 4013), "tst:TRICE8_2  %d %d\n", -1, -2 );
        break; case __LINE__: TRICE8_3 ( ID( 7098), "tst:TRICE8_3  %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE8_4 ( ID( 2481), "tst:TRICE8_4  %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE8_5 ( ID( 2095), "tst:TRICE8_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE8_6 ( ID( 3326), "tst:TRICE8_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE8_7 ( ID( 1004), "tst:TRICE8_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE8_8 ( ID( 2463), "tst:TRICE8_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE8_9 ( ID( 2999), "tst:TRICE8_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE8_10( ID( 1181), "tst:TRICE8_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE8_11( ID( 5296), "tst:TRICE8_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE8_12( ID( 3572), "tst:TRICE8_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE8   ( ID( 4981), "tst:TRICE8   %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );
        break; case __LINE__: TRICE8_9 ( ID( 4515), "tst:TRICE8_9 %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );

        break; case __LINE__: TRICE( ID( 3300), "sig:TRICE16 with variable param count 1 to 12\n" );
        break; case __LINE__: TRICE16( ID( 7518), "tst:TRICE16 %d\n", -1 );
        break; case __LINE__: TRICE16( ID( 6471), "tst:TRICE16 %d %d\n", -1, -2 );
        break; case __LINE__: TRICE16( ID( 1912), "tst:TRICE16 %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE16( ID( 3793), "tst:TRICE16 %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE16( ID( 4397), "tst:TRICE16 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE16( ID( 1707), "tst:TRICE16 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE16( ID( 5442), "tst:TRICE16 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE16( ID( 5531), "tst:TRICE16 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE16( ID( 1154), "tst:TRICE16 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE16( ID( 2258), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE16( ID( 1869), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE16( ID( 4519), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE16( ID( 6266), "tst:TRICE16 %t %b %x %X %d %u %o %O %p\n", 3, 3, 3, 3, 3, 3, 3, 3, 3 );
        break; case __LINE__: TRICE16_1 ( ID( 7026), "tst:TRICE16_1  %d\n", -1 );
        break; case __LINE__: TRICE16_2 ( ID( 7785), "tst:TRICE16_2  %d %d\n", -1, -2 );
        break; case __LINE__: TRICE16_3 ( ID( 2186), "tst:TRICE16_3  %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE16_4 ( ID( 7949), "tst:TRICE16_4  %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE16_5 ( ID( 6642), "tst:TRICE16_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE16_6 ( ID( 3971), "tst:TRICE16_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE16_7 ( ID( 3613), "tst:TRICE16_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE16_8 ( ID( 3098), "tst:TRICE16_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE16_9 ( ID( 1823), "tst:TRICE16_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE16_10( ID( 4842), "tst:TRICE16_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE16_11( ID( 4703), "tst:TRICE16_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE16_12( ID( 2422), "tst:TRICE16_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE16_9 ( ID( 1550), "tst:TRICE16 %t %b %x %X %d %u %o %O %p\n", 3, 3, 3, 3, 3, 3, 3, 3, 3 );

        break; case __LINE__: TRICE( ID( 6803), "sig:TRICE32 with variable param count 1 to 12\n" );
        break; case __LINE__: TRICE32( ID( 1512), "tst:TRICE32 %d\n", -1 );
        break; case __LINE__: TRICE32( ID( 6254), "tst:TRICE32 %d %d\n", -1, -2 );
        break; case __LINE__: TRICE32( ID( 6045), "tst:TRICE32 %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE32( ID( 2803), "tst:TRICE32 %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE32( ID( 1497), "tst:TRICE32 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE32( ID( 4874), "tst:TRICE32 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE32( ID( 5654), "tst:TRICE32 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE32( ID( 2838), "tst:TRICE32 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE32( ID( 2912), "tst:TRICE32 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE32( ID( 4714), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE32( ID( 4545), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE32( ID( 7261), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE32( ID( 5391), "tst:TRICE32 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));
        break; case __LINE__: TRICE32_1 ( ID( 5135), "tst:TRICE32_1  %d\n", -1 );
        break; case __LINE__: TRICE32_2 ( ID( 6904), "tst:TRICE32_2  %d %d\n", -1, -2 );
        break; case __LINE__: TRICE32_3 ( ID( 2624), "tst:TRICE32_3  %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE32_4 ( ID( 7625), "tst:TRICE32_4  %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE32_5 ( ID( 3500), "tst:TRICE32_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE32_6 ( ID( 7853), "tst:TRICE32_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE32_7 ( ID( 4256), "tst:TRICE32_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE32_8 ( ID( 7817), "tst:TRICE32_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE32_9 ( ID( 5429), "tst:TRICE32_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE32_10( ID( 1579), "tst:TRICE32_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE32_11( ID( 6903), "tst:TRICE32_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE32_12( ID( 1739), "tst:TRICE32_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE32   ( ID( 3828), "tst:TRICE32    %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));
        break; case __LINE__: TRICE32_12( ID( 6148), "tst:TRICE32_12 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));

        break; case __LINE__: TRICE( ID( 1351), "sig:TRICE64 with variable param count 1 to 12\n" );
        break; case __LINE__: TRICE64( ID( 3189), "tst:TRICE64 %d\n", -1 );
        break; case __LINE__: TRICE64( ID( 6883), "tst:TRICE64 %d %d\n", -1, -2 );
        break; case __LINE__: TRICE64( ID( 4450), "tst:TRICE64 %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE64( ID( 5059), "tst:TRICE64 %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE64( ID( 1011), "tst:TRICE64 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE64( ID( 7334), "tst:TRICE64 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE64( ID( 7474), "tst:TRICE64 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE64( ID( 2482), "tst:TRICE64 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE64( ID( 6515), "tst:TRICE64 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE64( ID( 3790), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE64( ID( 5788), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE64( ID( 6417), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE64( ID( 2875), "tst:TRICE64 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aDouble(3.14159), aDouble(3.14159), aDouble(3.14159));
        break; case __LINE__: TRICE64_1 ( ID( 3108), "tst:TRICE64_1  %d\n", -1 );
        break; case __LINE__: TRICE64_2 ( ID( 1791), "tst:TRICE64_2  %d %d\n", -1, -2 );
        break; case __LINE__: TRICE64_3 ( ID( 4052), "tst:TRICE64_3  %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE64_4 ( ID( 5759), "tst:TRICE64_4  %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE64_5 ( ID( 5220), "tst:TRICE64_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE64_6 ( ID( 3887), "tst:TRICE64_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE64_7 ( ID( 1276), "tst:TRICE64_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE64_8 ( ID( 3608), "tst:TRICE64_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE64_9 ( ID( 1719), "tst:TRICE64_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE64_10( ID( 2430), "tst:TRICE64_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE64_11( ID( 6494), "tst:TRICE64_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE64_12( ID( 5975), "tst:TRICE64_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE64_12( ID( 1205), "tst:TRICE64_12 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aDouble(3.14159), aDouble(3.14159), aDouble(3.14159));

        break; case __LINE__: TRICE( ID( 2870), "att:positive and negative float in format variants\n");
        break; case __LINE__: TRICE( ID( 2747), "rd:TRICE float %f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 7258), "rd:TRICE float %9f (%%9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 5101), "rd:TRICE float %.9f (%%.9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 3969), "rd:TRICE float %9.f (%%9.f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 4202), "rd:TRICE float %9.6f (%%9.6f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 6297), "rd:TRICE float %f (%%f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 6109), "rd:TRICE float %9f (%%9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 7403), "rd:TRICE float %.9f (%%.9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 2343), "rd:TRICE float %9.f (%%9.f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 6258), "rd:TRICE float %9.6f (%%9.6f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 1900), "rd:TRICE float %+f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 7516), "rd:TRICE float %+9f (%%9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 1830), "rd:TRICE float %+.9f (%%.9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 6730), "rd:TRICE float %+9.f (%%9.f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 5424), "rd:TRICE float %+9.6f (%%9.6f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 1767), "rd:TRICE float %+f (%%f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 4395), "rd:TRICE float %+9f (%%9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 1423), "rd:TRICE float %+.9f (%%.9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 3641), "rd:TRICE float %+9.f (%%9.f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 7542), "rd:TRICE float %+9.6f (%%9.6f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 6927), "rd:TRICE float %-f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 2010), "rd:TRICE float %-9f (%%9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 1730), "rd:TRICE float %-.9f (%%.9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 3772), "rd:TRICE float %-9.f (%%9.f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 5946), "rd:TRICE float %-9.6f (%%9.6f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 7178), "rd:TRICE float %-f (%%f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 4960), "rd:TRICE float %-9f (%%9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 2722), "rd:TRICE float %-.9f (%%.9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 3229), "rd:TRICE float %-9.f (%%9.f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 3430), "rd:TRICE float %-9.6f (%%9.6f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 3924), "att:positive float & double in variants\n");
        break; case __LINE__: TRICE32_1( ID( 1324), "rd:TRICE32_1 float %e (%%e)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1( ID( 3336), "rd:TRICE32_1 float %f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1( ID( 1946), "rd:TRICE32_1 float %g (%%g)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1( ID( 1999), "rd:TRICE32_1 float %E (%%E)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1( ID( 3237), "rd:TRICE32_1 float %F (%%F)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1( ID( 2014), "rd:TRICE32_1 float %G (%%G)\n", aFloat(x) );
        break; case __LINE__: TRICE32  ( ID( 7170), "rd:TRICE32 float %e (%%e)\n", aFloat(x) );
        break; case __LINE__: TRICE32  ( ID( 4599), "rd:TRICE32 float %f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE32  ( ID( 3707), "rd:TRICE32 float %g (%%g)\n", aFloat(x) );
        break; case __LINE__: TRICE32  ( ID( 3029), "rd:TRICE32 float %E (%%E)\n", aFloat(x) );
        break; case __LINE__: TRICE32  ( ID( 6702), "rd:TRICE32 float %F (%%F)\n", aFloat(x) );
        break; case __LINE__: TRICE32  ( ID( 3622), "rd:TRICE32 float %G (%%G)\n", aFloat(x) );
        break; case __LINE__: TRICE    ( ID( 4043), "rd:TRICE float %e (%%e)\n", aFloat(x) );
        break; case __LINE__: TRICE    ( ID( 6201), "rd:TRICE float %f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE    ( ID( 4624), "rd:TRICE float %g (%%g)\n", aFloat(x) );
        break; case __LINE__: TRICE    ( ID( 6280), "rd:TRICE float %E (%%E)\n", aFloat(x) );
        break; case __LINE__: TRICE    ( ID( 2272), "rd:TRICE float %F (%%F)\n", aFloat(x) );
        break; case __LINE__: TRICE    ( ID( 5214), "rd:TRICE float %G (%%G)\n", aFloat(x) );
        break; case __LINE__: TRICE64  ( ID( 1309), "rd:TRICE64 double %e (%%e), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64  ( ID( 3538), "rd:TRICE64 double %f (%%f), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64  ( ID( 5549), "rd:TRICE64 double %g (%%g), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64  ( ID( 1887), "rd:TRICE64 double %E (%%E), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64  ( ID( 3972), "rd:TRICE64 double %F (%%F), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64  ( ID( 5298), "rd:TRICE64 double %G (%%G), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( ID( 3306), "rd:TRICE64_1 double %e (%%e), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( ID( 6650), "rd:TRICE64_1 double %f (%%f), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( ID( 5848), "rd:TRICE64_1 double %g (%%g), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( ID( 3815), "rd:TRICE64_1 double %E (%%E), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( ID( 3041), "rd:TRICE64_1 double %F (%%F), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( ID( 1801), "rd:TRICE64_1 double %G (%%G), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE( ID( 2740), "att:negative float & double\n" );
        break; case __LINE__: TRICE( ID( 5561), "rd:TRICE float %e (%%e)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 4228), "rd:TRICE float %f (%%f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 7166), "rd:TRICE float %g (%%g)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 2254), "rd:TRICE float %E (%%E)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 1632), "rd:TRICE float %F (%%F)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 3262), "rd:TRICE float %G (%%G)\n", aFloat(-x) );
        break; case __LINE__: TRICE64( ID( 6469), "rd:TRICE64 double %e (%%e), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64( ID( 5440), "rd:TRICE64 double %f (%%f), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64( ID( 1943), "rd:TRICE64 double %g (%%g), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64( ID( 6777), "rd:TRICE64 double %E (%%E), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64( ID( 1831), "rd:TRICE64 double %F (%%F), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64( ID( 3015), "rd:TRICE64 double %G (%%G), aDouble(y)\n", aDouble(-y) );

        break; case __LINE__: TRICE  ( ID( 4359), "att:formatted float & double\n" );
        break; case __LINE__: TRICE32( ID( 4721), "rd:TRICE32 float %12.6e (%%12.6e)\n", aFloat(x) );
        break; case __LINE__: TRICE32( ID( 6385), "rd:TRICE32 float %12.6f (%%12.6f)\n", aFloat(x) );
        break; case __LINE__: TRICE32( ID( 3065), "rd:TRICE32 float %12.6g (%%12.6g)\n", aFloat(x) );
        break; case __LINE__: TRICE32( ID( 4828), "rd:TRICE32 float %12.6E (%%12.6E)\n", aFloat(x) );
        break; case __LINE__: TRICE32( ID( 2163), "rd:TRICE32 float %12.6F (%%12.6F)\n", aFloat(x) );
        break; case __LINE__: TRICE32( ID( 2558), "rd:TRICE32 float %12.6G (%%12.6G)\n", aFloat(x) );
        break; case __LINE__: TRICE64( ID( 6516), "rd:TRICE64 double %12.6e (%%12.6e), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64( ID( 2890), "rd:TRICE64 double %12.6f (%%12.6f), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64( ID( 4709), "rd:TRICE64 double %12.6g (%%12.6g), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64( ID( 1458), "rd:TRICE64 double %12.6E (%%12.6E), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64( ID( 5367), "rd:TRICE64 double %12.6F (%%12.6F), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64( ID( 1339), "rd:TRICE64 double %12.6G (%%12.6G), aDouble(y)\n", aDouble(y) );

        break; case __LINE__: TRICE ( ID( 4557), "att:Various single arguments\n" );
        break; case __LINE__: TRICE8( ID( 2957), "rd:TRICE8 line %t (%%t ,0)\n", 0 );
        break; case __LINE__: TRICE8( ID( 7984), "rd:TRICE8 line %t (%%t ,2)\n", 2 );
        break; case __LINE__: TRICE8( ID( 1692), "rd:TRICE8 line %u (%%u ,-1)\n", -1 );
        break; case __LINE__: TRICE8( ID( 7253), "rd:TRICE8 line %b (%%b ,-1)\n", -1 );
        break; case __LINE__: TRICE8( ID( 1997), "rd:TRICE8 line %o (%%o ,-1)\n", -1 );
        break; case __LINE__: TRICE8( ID( 3547), "rd:TRICE8 line %O (%%O ,-1)\n", -1 );
        break; case __LINE__: TRICE8( ID( 1246), "rd:TRICE8 line %X (%%X ,-1)\n", -1 );
        break; case __LINE__: TRICE8( ID( 3644), "rd:TRICE8 line %x (%%x ,-1)\n", -1 );
        break; case __LINE__: TRICE8( ID( 2887), "rd:TRICE8 line %d (%%d ,-1)\n", -1 );
        break; case __LINE__: TRICE16_1( ID( 7515), "rd:TRICE16_1 line %t (%%t ,0)\n", 0 );
        break; case __LINE__: TRICE16_1( ID( 7936), "rd:TRICE16_1 line %t (%%t ,2)\n", 2 );
        break; case __LINE__: TRICE16_1( ID( 4591), "rd:TRICE16_1 line %u (%%u -1)\n", -1 );
        break; case __LINE__: TRICE16_1( ID( 3277), "rd:TRICE16_1 line %b (%%b -1)\n", -1 );
        break; case __LINE__: TRICE16_1( ID( 4012), "rd:TRICE16_1 line %o (%%o -1)\n", -1 );
        break; case __LINE__: TRICE16_1( ID( 1238), "rd:TRICE16_1 line %x (%%x -1)\n", -1 );
        break; case __LINE__: TRICE16_1( ID( 6907), "rd:TRICE16_1 line %d (%%d -1)\n", -1 );
        break; case __LINE__: TRICE( ID( 6347), "rd:TRICE line %t (%%t -1)\n", 0 );
        break; case __LINE__: TRICE( ID( 5540), "rd:TRICE line %t (%%t -1)\n", 2 );
        break; case __LINE__: TRICE( ID( 6718), "rd:TRICE line %u (%%u)\n", -1 );
        break; case __LINE__: TRICE( ID( 2184), "rd:TRICE line %b (%%b)\n", -2 );
        break; case __LINE__: TRICE( ID( 6156), "rd:TRICE line %o (%%o)\n", -3 );
        break; case __LINE__: TRICE( ID( 2785), "rd:TRICE line %x (%%x)\n", -4 );
        break; case __LINE__: TRICE( ID( 5005), "rd:TRICE line %d (%%d)\n", -5 );

        break; case __LINE__: TRICE_N( ID( 1602), "msg:%s\n", A, 0 );
        break; case __LINE__: TRICE_N( ID( 3387), "msg:%s\n", A, 1 );
        break; case __LINE__: TRICE_N( ID( 5304), "msg:%s\n", A, 2 );
        break; case __LINE__: TRICE_N( ID( 7029), "msg:%s\n", A, 3 );
        break; case __LINE__: TRICE_N( ID( 4837), "msg:%s\n", A, 4 );
        break; case __LINE__: TRICE_N( ID( 7529), "msg:%s\n", A, 5 );
        break; case __LINE__: TRICE_N( ID( 4073), "msg:%s\n", A, 6 );
        break; case __LINE__: TRICE_N( ID( 4627), "msg:%s\n", A, 7 );
        break; case __LINE__: TRICE_N( ID( 3487), "msg:%s\n", A, 8 );
        break; case __LINE__: TRICE_N( ID( 3750), "msg:%s\n", A, 9 );
        break; case __LINE__: TRICE_N( ID( 6463), "msg:%s\n", A, 10 );
        break; case __LINE__: TRICE_N( ID( 1878), "msg:%s\n", A, 11 );
        break; case __LINE__: TRICE_N( ID( 6219), "msg:%s\n", A, 12 );
        break; case __LINE__: TRICE_N( ID( 5197), "msg:%s\n", A, 13 );
        break; case __LINE__: TRICE_N( ID( 5736), "msg:%s\n", A, 14 );
        break; case __LINE__: TRICE_N( ID( 6473), "msg:%s\n", A, 15 );
        break; case __LINE__: TRICE_N( ID( 7818), "msg:%s\n", A, 16 );
        break; case __LINE__: TRICE_N( ID( 4546), "msg:%s\n", A, 17 );
        break; case __LINE__: TRICE_N( ID( 2576), "msg:%s\n", A, 18 );
        break; case __LINE__: TRICE_N( ID( 7684), "msg:%s\n", A, 19 );
        break; case __LINE__: TRICE_N( ID( 2323), "msg:%s\n", A, 120 );
        break; case __LINE__: TRICE_N( ID( 6371), "msg:%s\n", A, 121 );
        break; case __LINE__: TRICE_N( ID( 3578), "msg:%s\n", A, 122 );
        break; case __LINE__: TRICE_N( ID( 2224), "msg:%s\n", A, 123 );
        break; case __LINE__: TRICE_N( ID( 6145), "msg:%s\n", A, 124 );
        break; case __LINE__: TRICE_N( ID( 2790), "msg:%s\n", A, 125 );
        break; case __LINE__: TRICE_N( ID( 5714), "msg:%s\n", A, 126 );
        break; case __LINE__: TRICE_N( ID( 4153), "msg:%s\n", A, 127 );
        break; case __LINE__: TRICE_N( ID( 5535), "msg:%s\n", A, 128 );
        break; case __LINE__: TRICE_N( ID( 1491), "msg:%s\n", A, 129 );

    #endif // #ifdef TRICE_CGO
    }
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


//! SCOPY is a helper macro for struct serialization.
#define SCOPY( element ) do{ char* n = #element; int size = sizeof( src->element ); memcpy( p, &(src->element), size ); p += size; TRICE_S( ID( 4244), "rd:sizeof(%8s)", n ); TRICE( ID( 2294), " = %d\n", size);}while(0);

    //! DCOPY is a helper macro for struct deserialization.
#define DCOPY( element ) do{ char* n = #element; int size = sizeof( dst->element ); memcpy( &(dst->element), p, size ); p += size; TRICE_S( ID( 1136), "rd:sizeof(%8s)", n ); TRICE( ID( 4230), " = %d\n", size);}while(0);


typedef struct{
    float x;
    float y;
    uint8_t rgb[3];
    //float z; // it seems, that the compiler does not align this with -o3 & time !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
} Point_t; //!< Point_t is small struct type.

static int serializePoint( char* dst, Point_t const * src ){
    char * p = dst;

    SCOPY( x )
    SCOPY( y )
    SCOPY( rgb )

    return p - dst;
}

static int deserializePoint( Point_t * const dst, char const * src ){
    char const * p = src;

    DCOPY( x )
    DCOPY( y )
    DCOPY( rgb )
    
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
    TRICE ( Id( 4633), "inf: Tryout tx struct:" );
    TRICE8_B( Id( 6320), " %02x ", &tx, sizeof(tx) );
    TRICE( Id( 1532), "\n" );

    TRICE ( Id( 3416), "inf: Tryout buffer:" );
    TRICE8_B( Id( 5565), " %02x ", dst, len ); //lint !e670
    TRICE( Id( 1185), "\n" );

    src = dst; // "data transfer"

    len = deserializeTryout( &rx, src );
    TRICE ( Id( 6020), "inf: Tryout rx struct:" );
    TRICE8_B( Id( 4298), " %02x ", &rx, sizeof(rx) );
    TRICE( Id( 6522), "\n" );

    TRICE( Id( 7239), "inf:sizeOf(Trypout) = %d, buffer length = %d\n", sizeof(tx), len );
    TRICE8_F( Id( 4635), "info:TryoutStructFunction", &tx, sizeof(tx) );
    TRICE8_F( Id( 5071), "info:TryoutBufferFunction", dst, len ); //lint !e670
}

static void exampleOfManualJSONencoding(void){
    typedef  struct {
    int Apple, Birn;
    float Fish;
    } Ex_t;
    Ex_t Ex = { -1, 2, (float)2.781 };
    TRICE( Id(13149), "att:MyStructEvaluationFunction(json:ExA{Apple:%d, Birn:%u, Fisch:%f}\n", Ex.Apple, Ex.Birn, aFloat(Ex.Fish) );
}

int TriceShortCheckStartLine( void ){
    return TRICE_SHORT_CHECK_START_LINE;
}

int TriceShortCheckLimitLine( void ){
    return TRICE_SHORT_CHECK_LIMIT_LINE;
}

