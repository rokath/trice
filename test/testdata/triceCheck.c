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

        break; case __LINE__: trice16( iD( 3206), "att: line %u\n", __LINE__ );

        // normal use cases
        break; case __LINE__: TRice( iD( 3166), "info:This is a message without values and a 32-bit stamp.\n" ); //exp: time: 842,150_450default: info:This is a message without values and a 32-bit stamp.
        break; case __LINE__: Trice( iD( 6198), "info:This is a message without values and a 16-bit stamp.\n" ); //exp: time:       5_654default: info:This is a message without values and a 16-bit stamp.
        break; case __LINE__: trice( iD( 6745), "info:This is a message without values and without stamp.\n"  ); //exp: time:            default: info:This is a message without values and without stamp.

        // special use cases
        break; case __LINE__: TRice64( iD( 2203),"info:12 64-bit values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 32-bit stamp.\n", -3, -4, -5, -6, 0, aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), -8 ); //exp: time: 842,150_450default: info:12 64-bit values -3, 18446744073709551612, fffffffffffffffb, FFFFFFFFFFFFFFFA, false, -7.123457e+00, -7.123457, -7.123456789, -7.123457E+00, -7.123457, -7.123456789, 0xb1111111111111111111111111111111111111111111111111111111111111000 and a 32-bit stamp.
        break; case __LINE__: Trice64( iD( 7925),"info:12 64-bit values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 16-bit stamp.\n", -3, -4, -5, -6, 0, aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), -8 ); //exp: time:       5_654default: info:12 64-bit values -3, 18446744073709551612, fffffffffffffffb, FFFFFFFFFFFFFFFA, false, -7.123457e+00, -7.123457, -7.123456789, -7.123457E+00, -7.123457, -7.123456789, 0xb1111111111111111111111111111111111111111111111111111111111111000 and a 16-bit stamp.
        break; case __LINE__: trice64( iD( 1716),"info:12 64-bit values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and without stamp.\n" , -3, -4, -5, -6, 0, aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), -8 ); //exp: time:            default: info:12 64-bit values -3, 18446744073709551612, fffffffffffffffb, FFFFFFFFFFFFFFFA, false, -7.123457e+00, -7.123457, -7.123456789, -7.123457E+00, -7.123457, -7.123456789, 0xb1111111111111111111111111111111111111111111111111111111111111000 and without stamp.
        break; case __LINE__: TRice32( iD( 2972),"info:12 32-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 32-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time: 842,150_450default: info:12 32-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 32-bit stamp.
        break; case __LINE__: Trice32( iD( 1295),"info:12 32-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 16-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:       5_654default: info:12 32-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 16-bit stamp.
        break; case __LINE__: trice32( iD( 2168),"info:12 32-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and without stamp.\n" , -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:            default: info:12 32-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and without stamp.
        break; case __LINE__: TRice16( iD( 1216),"info:12 16-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 32-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time: 842,150_450default: info:12 16-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 32-bit stamp.
        break; case __LINE__: Trice16( iD( 5877),"info:12 16-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 16-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:       5_654default: info:12 16-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 16-bit stamp.
        break; case __LINE__: trice16( iD( 1009),"info:12 16-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and without stamp.\n" , -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:            default: info:12 16-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and without stamp.
        break; case __LINE__: TRice8 ( iD( 2232),"info:12  8-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 32-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time: 842,150_450default: info:12  8-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 32-bit stamp.
        break; case __LINE__: Trice8 ( iD( 1079),"info:12  8-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 16-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:       5_654default: info:12  8-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 16-bit stamp.
        break; case __LINE__: trice8 ( iD( 5804),"info:12  8-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and without stamp.\n" , -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:            default: info:12  8-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and without stamp.

        break; case __LINE__: trice16( iD( 4547), "att: line %u\n", __LINE__ );

        // CGO compiler issue
    //  break; case __LINE__: TRice( iD( 2307),"info:12 default bit width values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 32-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //_exp: time: 842,150_450default: info:12 default bit width values -3, 4294967292, fffffffb, FFFFFFFA, truet, 1.401298e-45, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11000000111000111111001101011100 and a 32-bit stamp.
    //  break; case __LINE__: Trice( iD( 7067),"info:12 default bit width values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 16-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //_exp: time:       5_654default: info:12 default bit width values -3, 4294967292, fffffffb, FFFFFFFA, truet, 1.401298e-45, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11000000111000111111001101011100 and a 16-bit stamp.
    //  break; case __LINE__: trice( iD( 7611),"info:12 default bit width values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and without stamp.\n" , -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //_exp: time:            default: info:12 default bit width values -3, 4294967292, fffffffb, FFFFFFFA, truet, 1.401298e-45, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11000000111000111111001101011100 and without stamp.

        // todo: improve parsing of //exp string containing special characters
        //break; case __LINE__: TRice( iD( 3164), "A TRice message containing a newline\nbefore the line end\n" );               //exp: time: 842,150_450default: A TRice message containing a newline\n                                         before the line end                                                                                                      
        //break; case __LINE__: TRice( iD( 6554), "\nA TRice message starting with a a newline\n" );                             //exp: time: 842,150_450default: \n                                         A TRice message starting with a a newline                                                                                                                   
        //break; case __LINE__: TRice( iD( 3112), "line0:A TRice message with several lines\nline1\nline2\nline3:last line\n" ); //exp: time: 842,150_450default: line0:A TRice message with several lines\\n                                         line1\n                                         line2\n                                         line3:last line

        break; case __LINE__: TRice16( iD( 1721), "att: 0x8888 == %04xh\n", 0x8888 ); //exp: time: 842,150_450default: att: 0x8888 == 8888h
        break; case __LINE__: Trice16( iD( 2571), "att: 0x8888 == %04xh\n", 0x8888 ); //exp: time:       5_654default: att: 0x8888 == 8888h
        break; case __LINE__: trice16( iD( 6526), "att: 0x8888 == %04xh\n", 0x8888 ); //exp: time:            default: att: 0x8888 == 8888h


        break; case __LINE__: trice16( iD( 2339), "att: line %u\n", __LINE__ );

        break; case __LINE__: trice16( iD( 2080), "att: line %u\n", __LINE__ );
        break; case __LINE__: exampleOfManualSerialization();

        break; case __LINE__: trice8   ( iD( 6835),         "value=%x\n", 0x55 );                           //exp: time:            default: value=55
        break; case __LINE__: trice8   ( iD( 6858),  "value=%x\n", 0x88 );                           //exp: time:            default: value=88

        break; case __LINE__: TRICE( ID( 3341), "att:mixed int & float & double & bits\n" );
        break; case __LINE__: TRICE32( ID( 3168), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",       FloatToInt32(x),   aFloat(x),   aFloat(x),   aFloat(x) ); //exp: time: 842,150_450default: rd:TRICE32 int 1089, float 1089.608276 (%f), 44883377, 01000100100010000011001101110111                                         
    //  break; case __LINE__: TRICE32( ID( 6772), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",          (int32_t)(x),   aFloat(x),   aFloat(x),   aFloat(x) ); //exp: time: 842,150_450default: rd:TRICE32 int 1089, float 1089.608276 (%f), 44883377, 01000100100010000011001101110111                                         
        break; case __LINE__: TRICE32( ID( 2486), "rd:TRICE32 int %x, float %f (%%f), %08x, %032b\n",            0x44883377,   aFloat(x),   aFloat(x),   aFloat(x) ); //exp: time: 842,150_450default: rd:TRICE32 int 44883377, float 1089.608276 (%f), 44883377, 01000100100010000011001101110111                                         
        break; case __LINE__: TRICE32( ID( 4061), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",                     x,   aFloat(x),   aFloat(x),   aFloat(x) ); //exp: time: 842,150_450default: rd:TRICE32 int 1089, float 1089.608276 (%f), 44883377, 01000100100010000011001101110111                                         
        break; case __LINE__: TRICE32( ID( 1654), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",      FloatToInt32(-x),  aFloat(-x),  aFloat(-x),  aFloat(-x) ); //exp: time: 842,150_450default: rd:TRICE32 int -1089, float -1089.608276 (%f), c4883377, 11000100100010000011001101110111                                       
        break; case __LINE__: TRICE32( ID( 1362), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",         (int32_t)(-x),  aFloat(-x),  aFloat(-x),  aFloat(-x) ); //exp: time: 842,150_450default: rd:TRICE32 int -1089, float -1089.608276 (%f), c4883377, 11000100100010000011001101110111                                       
    //  break; case __LINE__: TRICE32( ID( 5884), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",                    -x,  aFloat(-x),  aFloat(-x),  aFloat(-x) ); //exp: time: 842,150_450default: rd:TRICE32 int 1089, float -1089.608276 (%f), c4883377, 11000100100010000011001101110111                                        
        break; case __LINE__: TRICE32( ID( 6117), "rd:TRICE32 int %X, float %f (%%f), %08x, %032b\n",           -0x44883377,  aFloat(-x),  aFloat(-x),  aFloat(-x) ); //exp: time: 842,150_450default: rd:TRICE32 int BB77CC89, float -1089.608276 (%f), c4883377, 11000100100010000011001101110111                                        
        break; case __LINE__: TRICE64( ID( 4251), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",    DoubleToInt64(y),  aDouble(y),  aDouble(y),  aDouble(y) ); //exp: time: 842,150_450default: rd:TRICE64 int 518, double 518.054749 (%f), 4080307020601050, 0100000010000000001100000111000000100000011000000001000001010000   
        break; case __LINE__: TRICE64( ID( 2250), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",        (int64_t)(y),  aDouble(y),  aDouble(y),  aDouble(y) ); //exp: time: 842,150_450default: rd:TRICE64 int 518, double 518.054749 (%f), 4080307020601050, 0100000010000000001100000111000000100000011000000001000001010000   
        break; case __LINE__: TRICE64( ID( 3487), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",                   y,  aDouble(y),  aDouble(y),  aDouble(y) ); //exp: time: 842,150_450default: rd:TRICE64 int 518, double 518.054749 (%f), 4080307020601050, 0100000010000000001100000111000000100000011000000001000001010000   
        break; case __LINE__: TRICE64( ID( 6402), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",   DoubleToInt64(-y), aDouble(-y), aDouble(-y), aDouble(-y) ); //exp: time: 842,150_450default: rd:TRICE64 int -518, double -518.054749 (%f), c080307020601050, 1100000010000000001100000111000000100000011000000001000001010000
        break; case __LINE__: TRICE64( ID( 2100), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",       (int64_t)(-y), aDouble(-y), aDouble(-y), aDouble(-y) ); //exp: time: 842,150_450default: rd:TRICE64 int -518, double -518.054749 (%f), c080307020601050, 1100000010000000001100000111000000100000011000000001000001010000
    //  break; case __LINE__: TRICE64( ID( 1974), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",                  -y, aDouble(-y), aDouble(-y), aDouble(-y) ); //exp: time: 842,150_450default: rd:TRICE64 int 518, double -518.054749 (%f), c080307020601050, 1100000010000000001100000111000000100000011000000001000001010000 
        break; case __LINE__: TRICE64( ID( 5692), "rd:TRICE64 int %X, double %f (%%f), %016x, %064b\n", -0x4080307020601050, aDouble(-y), aDouble(-y), aDouble(-y) ); //exp: time: 842,150_450default: rd:TRICE64 int BF7FCF8FDF9FEFB0, double -518.054749 (%f), c080307020601050, 1100000010000000001100000111000000100000011000000001000001010000 

        break; case __LINE__: trice16( iD( 6861), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRICE_S( ID( 3081), "msg:With TRICE_S:%s\n", sABCDE );                           //exp: time: 842,150_450default: msg:With TRICE_S:abcde 12345
        break; case __LINE__: TRICE_N( ID( 4791), "sig:With TRICE_N:%s\n", sABCDE, lenABCDE );                 //exp: time: 842,150_450default: sig:With TRICE_N:abcde 12345
        break; case __LINE__: TRICE8_B( ID( 2703), "  %02x", sABCDE, lenABCDE );                               //exp: time: 842,150_450default:   61  62  63  64  65  20  31  32  33  34  35
        break; case __LINE__: TRICE( ID( 4151), "\n" );
        break; case __LINE__: TRICE8_B( ID( 4407), "%4d", sABCDE, lenABCDE );                                  //exp: time: 842,150_450default:   97  98  99 100 101  32  49  50  51  52  53
        break; case __LINE__: TRICE( ID( 1757), "\n" );
        break; case __LINE__: TRICE8_B( ID( 4967), "  %02x", b8, sizeof(b8)/sizeof(int8_t) );                  //exp: time: 842,150_450default:   00  ff  fe  33  04  05  06  07  08  09  0a  0b  00  ff  fe  33  04  05  06  07  08  09  0a  0b
        break; case __LINE__: TRICE( ID( 7693), "\n" );
        break; case __LINE__: TRICE16_B( ID( 1695), "  %04x", b16, sizeof(b16)/sizeof(int16_t) );              //exp: time: 842,150_450default:   0000  ffff  fffe  3344
        break; case __LINE__: TRICE( ID( 4626), "\n" );
        break; case __LINE__: TRICE32_B( ID( 1678), "  %08x", b32, sizeof(b32)/sizeof(int32_t) );              //exp: time: 842,150_450default:   00000000  ffffffff  fffffffe  33445555
        break; case __LINE__: TRICE( ID( 5550), "\n" );
        break; case __LINE__: TRICE64_B( ID( 2266), "  %016x", b64, sizeof(b64)/sizeof(int64_t) );             //exp: time: 842,150_450default:   0000000000000000  ffffffffffffffff  fffffffffffffffe  3344555566666666
        break; case __LINE__: TRICE( ID( 5217), "\n" );
        break; case __LINE__: TRICE8_F( ID( 4858),  "info:FunctionNameW", b8,  sizeof(b8) /sizeof(int8_t) );   //exp: time: 842,150_450default: info:FunctionNameW(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)
        break; case __LINE__: TRICE16_F( ID( 3706), "info:FunctionNameX", b16, sizeof(b16)/sizeof(int16_t) );  //exp: time: 842,150_450default: info:FunctionNameX(0000)(ffff)(fffe)(3344) 
        break; case __LINE__: TRICE32_F( ID( 5592), "info:FunctionNameY", b32, sizeof(b32)/sizeof(int32_t) );  //exp: time: 842,150_450default: info:FunctionNameY(00000000)(ffffffff)(fffffffe)(33445555)
        break; case __LINE__: TRICE64_F( ID( 5495), "info:FunctionNameZ", b64, sizeof(b64)/sizeof(int64_t) );  //exp: time: 842,150_450default: info:FunctionNameZ(0000000000000000)(ffffffffffffffff)(fffffffffffffffe)(3344555566666666)
        break; case __LINE__: exampleOfManualJSONencoding(); //exp: time:       5_654default: att:MyStructEvaluationFunction(json:ExA{Apple:-1, Birn:2, Fisch:2.781000}

        break; case __LINE__: trice16( iD( 1171), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRICE( Id( 6320), "MSG:1/11 = %g\n", aFloat( 1.0/11 ) ); //exp: time:       5_654default: MSG:1/11 = 0.09090909
        break; case __LINE__: { //exp: time: 842,150_450default: msg:x = 5.934 = 5.934, 5.934
                              float a = (float)5.934;
                              float b = a + ((a > 0) ? 0.0005f : -0.0005f);
                              int c = (int)b;
                              int d = (int)(b * 1000) % 1000;
                              int e = (int)(1000 * (float)(a - c)); 
                              TRICE( ID( 6182), "msg:x = %g = %d.%03d, %d.%03d\n", aFloat(a), c, d, c, e ); 
                              }

        //break; case __LINE__: TRICE( id( 3661), "dbg:Hi!\n" );                       //exp: time:            default: dbg:Hi!    
        //break; case __LINE__: TRICE( id( 4457), "dbg:\tHi!\n" );                     
        //break; case __LINE__: TRICE( id( 3018), "dbg:\t\tHi!\n" );                   
        //break; case __LINE__: TRICE( id( 6714), "dbg:\t\t\tHi!\n" );                 
        //break; case __LINE__: TRICE( id( 7722), "dbg:\\tHi!\n" );                    
        //break; case __LINE__: TRICE( Id( 1449), "dbg:\aHi!\n" ); // sound!
        //break; case __LINE__: TRICE( Id( 1214), "dbg:\a\aHi!\n" ); // sound!
        //break; case __LINE__: TRICE( Id( 7391), "dbg:\a\a\aHi!\n" ); // sound!
        //break; case __LINE__: TRICE( Id( 5808), "dbg:\\aHi!\n" );
        break; case __LINE__: TRICE( Id( 4984), "err:An ERROR messgage with sound!\a\n" ); // sound!

        break; case __LINE__: TRICE_S( ID( 6390), "sig:TRICE_S=%s\n", s );            //exp: time: 842,150_450default: sig:TRICE_S=AAAAAAAAAAAA
        break; case __LINE__: TRICE_N( ID( 5333), "sig:TRICE_N=%s\n", s, strlen(s) ); //exp: time: 842,150_450default: sig:TRICE_N=AAAAAAAAAAAA

        break; case __LINE__: trice16( iD( 5028), "att: line %u\n", __LINE__ );

//      break; case __LINE__: 
//  #ifdef XTEA_ENCRYPT_KEY
//          TRICE0 (Id( 4367), "--------------------------------------------------\n" );
//          {
//              uint32_t by[8] = {1,2,3,4,5,6};
//              int count = (sizeof(by) + 7) & ~7; // only multiple of 8 encryptable
//              TRICE32(  Id( 1939), "msg: message = %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
//              TRICE32( Id( 7018), "tim: pre TriceEncryption SysTick=%d\n", SYSTICKVAL );
//              XTEAEncrypt(by, count>>2);
//              TRICE32( Id( 7361), "tim: post TriceEncryption SysTick=%d\n", SYSTICKVAL );
//              TRICE32(  Id( 3679), "att: TriceEncrypted =  %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
//              #ifdef XTEA_DECRYPT
//              TRICE16_1( Id( 4669), "tim: pre TriceDecryption SysTick=%d\n", SYSTICKVAL );
//              XTEADecrypt(by, count>>2);
//              TRICE16_1( Id( 5902), "tim: post TriceDecryption SysTick=%d\n", SYSTICKVAL );
//              #endif
//              TRICE32(  Id( 4093), "msg: messge = %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
//          }
//          TRICE0 (Id( 2388), "--------------------------------------------------\n" );
//  #endif

        break; case __LINE__: trice16( iD( 1023), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRICE( ID( 6865), "sig:Some time measurements\n" );
        break; case __LINE__: TRICE( ID( 6680), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 6834), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 6613), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 1405), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
        break; case __LINE__: TRICE64( ID( 5115), "rd:TRICE64 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE( ID( 6662), "tim:TRICE START time message\n");
        break; case __LINE__: TRICE( ID( 3064), "tim:TRICE STOP time message\n");
        break; case __LINE__: TRICE( ID( 6516), "tim:TRICE START time message\n");
        break; case __LINE__: TRICE( ID( 3853), "tim:TRICE STOP time message\n");
        break; case __LINE__: TRICE( ID( 2634), "tim:TRICE START time message\n");
        break; case __LINE__: TRICE64( ID( 3289), "rd:TRICE64 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE( ID( 4064), "tim:TRICE STOP time message\n");
        break; case __LINE__: TRICE( ID( 5622), "tim:TRICE --------------------------------------------------\n");
        break; case __LINE__: TRICE( ID( 3211), "tim:TRICE --------------------------------------------------\n");
        break; case __LINE__: TRICE64( ID( 3469), "rd:TRICE64 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE( ID( 5265), "tim:TRICE --------------------------------------------------\n");
        break; case __LINE__: TRICE( ID( 1719), "tim:TRICE --------------------------------------------------\n");
        break; case __LINE__: TRICE64( ID( 3938), "rd:TRICE64 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE( ID( 7667), "rd:TRICE32_1 line %d\n", __LINE__ );
        break; case __LINE__: TRICE64_1( ID( 7446), "rd:TRICE64_1 %d\n", __LINE__ );
        break; case __LINE__: TRICE32_2( ID( 7713), "rd:TRICE32_2 line %d,%d\n", __LINE__, 2 );
        break; case __LINE__: TRICE64_2( ID( 5439), "rd:TRICE64_2 line %d,%d\n", __LINE__, 2 );

        break; case __LINE__: trice16( iD( 6899), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRICE( ID( 3015), "sig:Runtime generated strings\n" );
        break; case __LINE__: {
                                  s = "AAAAAAAAAAAA";
                                  TRICE32( ID( 7268), "dbg:len=%u:", strlen(s) );
                                  TRICE_S( ID( 1556), "sig:%s\n", s );
                              }
        break; case __LINE__: {
                s = "\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBCCCCCCCC\
";
                          }
        break; case __LINE__: TRICE32( ID( 2272), "dbg:len=%u:", strlen(s) );
        break; case __LINE__: TRICE_S( ID( 5272), "sig:%s\n", s );
        break; case __LINE__: TRICE( ID( 3966), "sig:Runtime generated strings\n" );
        break; case __LINE__: {
            s = "\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\
DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\
EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\
";
                          }
        break; case __LINE__: TRICE32( ID( 7814), "dbg:len=%u: ", strlen(s) );
        break; case __LINE__: TRICE_S( ID( 5440), "sig:%s\n", s );
        break; case __LINE__: TRICE( ID( 6183), "sig:Runtime generated strings\n" );
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
        break; case __LINE__: TRICE32( ID( 7234), "dbg:len=%u: ", strlen(s) );
        break; case __LINE__: TRICE_S( ID( 7938), "sig:%s\n", s );
        break; case __LINE__: TRICE( ID( 1165), "sig:Runtime generated strings\n" );
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
        break; case __LINE__: TRICE32( ID( 3038), "dbg:len=%u: ", strlen(s) );
        break; case __LINE__: TRICE_S( ID( 2704), "sig:%s\n", s );

        break; case __LINE__: trice16( iD( 5897), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRICE( ID( 2601), "sig:Integer (indent, base, sign) - see https://yourbasic.org/golang/fmt-printf-reference-cheat-sheet/\n" );
        break; case __LINE__: TRICE( ID( 3371), "rd: 15 	%d 	Base 10\n", 15 );                                        //ex_: time: 842,150_450default: rd: 15 	15 	Base 10                                   
        break; case __LINE__: TRICE( ID( 6698), "rd: +15 	%+d 	Always show sign\n", 15 );                           //ex_: time: 842,150_450default: rd: +15 	+15 	Always show sign                          
        break; case __LINE__: TRICE( ID( 4438), "rd:   15 	%4d 	Pad with spaces (width 4, right justified)\n", 15 ); //ex_: time: 842,150_450default: rd:   15	  15 	Pad with spaces (width 4, right justified)
        break; case __LINE__: TRICE( ID( 5230), "rd: 15   	%-4d 	Pad with spaces (width 4, left justified)\n", 15 );  //ex_: time: 842,150_450default: rd: 15 	15   	Pad with spaces (width 4, left justified) 
        break; case __LINE__: TRICE( ID( 2592), "rd: 0015 	%04d 	Pad with zeroes (width 4)\n", 15 );                  //ex_: time: 842,150_450default: rd: 0015 	0015 	Pad with zeroes (width 4)                 
        break; case __LINE__: TRICE( ID( 5818), "rd: 1111 	%b 	Base 2\n", 15 );                                         //ex_: time: 842,150_450default: rd: 1111 	1111    Base 2                                    
        break; case __LINE__: TRICE( ID( 7407), "rd: 17 	%o 	Base 8\n", 15 );                                         //ex_: time: 842,150_450default: rd: 17 	17      Base 8                                    
        break; case __LINE__: TRICE( ID( 1059), "rd: f 	%x 	Base 16, lowercase\n", 15 );                             //ex_: time: 842,150_450default: rd: f 	f 	Base 16, lowercase                        
        break; case __LINE__: TRICE( ID( 2690), "rd: F 	%X 	Base 16, uppercase\n", 15 );                             //ex_: time: 842,150_450default: rd: F 	F 	Base 16, uppercase                        
        break; case __LINE__: TRICE( ID( 3233), "rd: 0xf 	%#x 	Base 16, with leading 0x\n", 15 );                   //ex_: time: 842,150_450default: rd: 0xf 	0xf 	Base 16, with leading 0x                  

        break; case __LINE__: trice16( iD( 7149), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRICE( ID( 3048), "sig:Character (quoted, Unicode)\n" );
        break; case __LINE__: TRICE( ID( 5062), "rd: A 	%c 	Character\n", 'A' );                                     //exp: time: 842,150_450default: rd: A 	A 	Character
        break; case __LINE__: TRICE( ID( 2434), "rd: 'A' 	%q 	Quoted character\n", 'A' );                              //exp: time: 842,150_450default: rd: 'A' 	'A' 	Quoted character
    //  break; case __LINE__: TRICE( ID( 6498), "rd: U+0041 	%U 	Unicode\n", 'A' );                        // todo: detect %U to avoid message triceType = TRICE0 ParamSpace = 4 not matching with bitWidth  0 and paramCount 0 - ignoring package
    //  break; case __LINE__: TRICE( ID( 1379), "rd: U+0041 'A' 	%#U 	Unicode with character\n", 'A' ); // todo: detect %#U to avoid message triceType = TRICE0 ParamSpace = 4 not matching with bitWidth  0 and paramCount 0 - ignoring package

        break; case __LINE__: TRICE( ID( 7531), "sig:Boolean (true/false)\n" );
        break; case __LINE__: TRICE( ID( 6375), "rd:Use %%t to format a boolean as true (%t) or false (%t).\n", 1, 0 ); //exp: time: 842,150_450default: rd:Use %t to format a boolean as true (true) or false (false).

        break; case __LINE__: TRICE( ID( 5559), "sig:Pointer (hex)\n" );
        break; case __LINE__: TRICE8 ( ID( 2894), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x84 );               //exp: time: 842,150_450default: rd:Use %p to format a pointer in base 16 notation with leading 0x. (84)              
        break; case __LINE__: TRICE16( ID( 6175), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x1234 );             //exp: time: 842,150_450default: rd:Use %p to format a pointer in base 16 notation with leading 0x. (1234)            
        break; case __LINE__: TRICE32( ID( 1022), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0xaabbccdd );         //exp: time: 842,150_450default: rd:Use %p to format a pointer in base 16 notation with leading 0x. (aabbccdd)        
        break; case __LINE__: TRICE64( ID( 4262), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x1122334455667788 ); //exp: time: 842,150_450default: rd:Use %p to format a pointer in base 16 notation with leading 0x. (1122334455667788)
        break; case __LINE__: TRICE  ( ID( 5699), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0xaabbccdd);          //exp: time: 842,150_450default: rd:Use %p to format a pointer in base 16 notation with leading 0x. (aabbccdd)        

        break; case __LINE__: trice16( iD( 5223), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRICE( ID( 3179), "sig:Float (indent, precision, scientific notation)\n" );
        break; case __LINE__: TRICE( ID( 3163), "rd: 1.234560e+02		%e 	%%e Scientific notation\n", aFloat(123.456) );                       //exp: time: 842,150_450default: rd: 1.234560e+02		1.234560e+02 	%e Scientific notation
        break; case __LINE__: TRICE( ID( 4034), "rd: 123.456000		%f 	%%f Decimal point, no exponent\n", aFloat(123.456) );                //exp: time: 842,150_450default: rd: 123.456000		123.456001 	%f Decimal point, no exponent
        break; case __LINE__: TRICE( ID( 6573), "rd: 123.46			%.2f		%%.2f Default width, precision 2\n", aFloat(123.456) );      //exp: time: 842,150_450default: rd: 123.46			123.46		%.2f Default width, precision 2
        break; case __LINE__: TRICE( ID( 4609), "rd: ␣␣123.46		%8.2f	%%8.2f Width 8, precision 2\n", aFloat(123.456) );                    //exp: time: 842,150_450default: rd: ␣␣123.46		  123.46	%8.2f Width 8, precision 2
        break; case __LINE__: TRICE( ID( 7423), "rd: 123.456		%g		%%g Exponent as needed, necessary digits only\n", aFloat(123.456) ); //exp: time: 842,150_450default: rd: 123.456		123.456		%g Exponent as needed, necessary digits only

        break; case __LINE__: TRICE( ID( 2847), "sig:Double (indent, precision, scientific notation)\n" );
        break; case __LINE__: TRICE64( ID( 2512), "rd: 1.234560e+02		%e 	%%e Scientific notation\n", aDouble(123.456) );                   //exp: time: 842,150_450default: rd: 1.234560e+02		1.234560e+02 	%e Scientific notation
        break; case __LINE__: TRICE64( ID( 5544), "rd: 123.456000		%f 	%%f Decimal point, no exponent\n", aDouble(123.456) );                //exp: time: 842,150_450default: rd: 123.456000		123.456000 	%f Decimal point, no exponent
        break; case __LINE__: TRICE64( ID( 7058), "rd: 123.46			%.2f    	%%.2f Default width, precision 2\n", aDouble(123.456) );      //exp: time: 842,150_450default: rd: 123.46			123.46    	%.2f Default width, precision 2
        break; case __LINE__: TRICE64( ID( 1667), "rd: ␣␣123.46		%8.2f 	%%8.2f Width 8, precision 2\n", aDouble(123.456) );                //exp: time: 842,150_450default: rd: ␣␣123.46		  123.46 	%8.2f Width 8, precision 2
        break; case __LINE__: TRICE64( ID( 7708), "rd: 123.456		%g		%%g Exponent as needed, necessary digits only\n", aDouble(123.456) ); //exp: time: 842,150_450default: rd: 123.456		123.456		%g Exponent as needed, necessary digits only

        break; case __LINE__: TRICE( ID( 3025), "sig:String or byte slice (quote, indent, hex)\n" );
        break; case __LINE__: s = "café"; TRICE_S( ID( 2200), "rd: café 			%s		Plain string\n", s );           //exp: time: 842,150_450default: rd: café 			café		Plain string
        break; case __LINE__: s = "café"; TRICE_S( ID( 3071), "rd: ␣␣café 		%6s 		Width 6, right justify\n", s );  //exp: time: 842,150_450default: rd: ␣␣café 		  café 		Width 6, right justify
        break; case __LINE__: s = "café"; TRICE_S( ID( 6640), "rd: café␣␣ 		%-6s 		Width 6, left justify\n", s );   //exp: time: 842,150_450default: rd: café␣␣ 		café   		Width 6, left justify
        break; case __LINE__: s = "café"; TRICE_S( ID( 2007), "rd: \"café\" 		%q		Quoted string\n", s );          //exp: time: 842,150_450default: rd: \"café\" 		"café"		Quoted string
        break; case __LINE__: s = "café"; TRICE_S( ID( 4364), "rd: 636166c3a9 		%x	Hex dump of byte values\n", s );    //exp: time: 842,150_450default: rd: 636166c3a9 		636166c3a9	Hex dump of byte values 
        break; case __LINE__: s = "café"; TRICE_S( ID( 5388), "rd: 63 61 66 c3 a9 	% x	Hex dump with spaces\n", s );       //exp: time: 842,150_450default: rd: 63 61 66 c3 a9 	63 61 66 c3 a9	Hex dump with spaces

        break; case __LINE__: trice16( iD( 3633), "att: line %u\n", __LINE__ );

#ifdef TRICE_CGO

        break; case __LINE__: TRICE  ( ID( 1710), "rd:%E (%%E)\n",  aFloat(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-5.555556E+08 (%E)
        break; case __LINE__: TRICE  ( ID( 1551), "rd:%F (%%F)\n",  aFloat(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-555555584.000000 (%F)
        break; case __LINE__: TRICE  ( ID( 6705), "rd:%G (%%G)\n",  aFloat(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-5.555556E+08 (%G)
        break; case __LINE__: TRICE64( ID( 4072), "rd:%E (%%E)\n", aDouble(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-5.555556E+08 (%E)
        break; case __LINE__: TRICE64( ID( 7550), "rd:%F (%%F)\n", aDouble(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-555555555.555556 (%F)
        break; case __LINE__: TRICE64( ID( 7184), "rd:%G (%%G)\n", aDouble(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-5.555555555555556E+08 (%G)
        
        break; case __LINE__: TRICE  ( ID( 2631), "rd:%e (%%e)\n",  aFloat(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-5.555556e+08 (%e)
        break; case __LINE__: TRICE  ( ID( 3016), "rd:%f (%%f)\n",  aFloat(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-555555584.000000 (%f)
        break; case __LINE__: TRICE  ( ID( 6592), "rd:%g (%%g)\n",  aFloat(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-5.555556e+08 (%g)
        break; case __LINE__: TRICE64( ID( 7631), "rd:%e (%%e)\n", aDouble(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-5.555556e+08 (%e)
        break; case __LINE__: TRICE64( ID( 7712), "rd:%f (%%f)\n", aDouble(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-555555555.555556 (%f)
        break; case __LINE__: TRICE64( ID( 4324), "rd:%g (%%g)\n", aDouble(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-5.555555555555556e+08 (%g)

        break; case __LINE__: TRICE32( ID( 1946), "msg:%u (%%u)\n", -1 ); //exp: time: 842,150_450default: msg:4294967295 (%u)
        break; case __LINE__: TRICE32( ID( 2090), "msg:%b (%%b)\n", -1 ); //exp: time: 842,150_450default: msg:11111111111111111111111111111111 (%b)
        break; case __LINE__: TRICE32( ID( 2119), "msg:%o (%%o)\n", -1 ); //exp: time: 842,150_450default: msg:37777777777 (%o)
        break; case __LINE__: TRICE32( ID( 1944), "msg:%O (%%O)\n", -1 ); //exp: time: 842,150_450default: msg:0o37777777777 (%O)
        break; case __LINE__: TRICE32( ID( 2333), "msg:%X (%%X)\n", -1 ); //exp: time: 842,150_450default: msg:FFFFFFFF (%X)
        break; case __LINE__: TRICE32( ID( 5343), "msg:%x (%%x)\n", -1 ); //exp: time: 842,150_450default: msg:ffffffff (%x)
        break; case __LINE__: TRICE32( ID( 5684), "msg:%d (%%d)\n", -1 ); //exp: time: 842,150_450default: msg:-1 (%d)
        break; case __LINE__: TRICE64( ID( 1262), "msg:%u (%%u)\n", -1 ); //exp: time: 842,150_450default: msg:18446744073709551615 (%u)
        break; case __LINE__: TRICE64( ID( 7654), "msg:%b (%%b)\n", -1 ); //exp: time: 842,150_450default: msg:1111111111111111111111111111111111111111111111111111111111111111 (%b)
        break; case __LINE__: TRICE64( ID( 4220), "msg:%o (%%o)\n", -1 ); //exp: time: 842,150_450default: msg:1777777777777777777777 (%o)
        break; case __LINE__: TRICE64( ID( 4475), "msg:%O (%%O)\n", -1 ); //exp: time: 842,150_450default: msg:0o1777777777777777777777 (%O)
        break; case __LINE__: TRICE64( ID( 4075), "msg:%X (%%X)\n", -1 ); //exp: time: 842,150_450default: msg:FFFFFFFFFFFFFFFF (%X)
        break; case __LINE__: TRICE64( ID( 6472), "msg:%x (%%x)\n", -1 ); //exp: time: 842,150_450default: msg:ffffffffffffffff (%x)

        //break; case __LINE__: TRice        ( iD( 7619), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        //break; case __LINE__: Trice        ( iD( 2215), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        //break; case __LINE__: trice        ( iD( 1086), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: TRice8       ( iD( 4631), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: Trice8       ( iD( 6089), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: trice8       ( iD( 4844), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: TRice16      ( iD( 1095), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: Trice16      ( iD( 6051), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: trice16      ( iD( 2394), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: TRice32      ( iD( 6876), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: Trice32      ( iD( 1809), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: trice32      ( iD( 7485), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: TRice64      ( iD( 1593), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: Trice64      ( iD( 5533), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: trice64      ( iD( 5206), "value=%d\n", -2  ); //exp: time:            default: value=-2
        
        //break; case __LINE__: TRice_1      ( iD( 7987), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        //break; case __LINE__: Trice_1      ( iD( 2291), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        //break; case __LINE__: trice_1      ( iD( 5433), "value=%d\n", -2  ); //exp: time:            default: value=-2
        //break; case __LINE__: TRice8_1     ( iD( 7299), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        //break; case __LINE__: Trice8_1     ( iD( 5881), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        //break; case __LINE__: trice8_1     ( iD( 4445), "value=%d\n", -2  ); //exp: time:            default: value=-2
        //break; case __LINE__: TRice16_1    ( iD( 6134), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        //break; case __LINE__: Trice16_1    ( iD( 2376), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        //break; case __LINE__: trice16_1    ( iD( 4796), "value=%d\n", -2  ); //exp: time:            default: value=-2
        //break; case __LINE__: TRice32_1    ( iD( 4900), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        //break; case __LINE__: Trice32_1    ( iD( 7014), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        //break; case __LINE__: trice32_1    ( iD( 2239), "value=%d\n", -2  ); //exp: time:            default: value=-2
        //break; case __LINE__: TRice64_1    ( iD( 2296), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        //break; case __LINE__: Trice64_1    ( iD( 5736), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        //break; case __LINE__: trice64_1    ( iD( 1424), "value=%d\n", -2  ); //exp: time:            default: value=-2
        
        //break; case __LINE__: TRice        ( iD( 1031), "no value" ); //exp: time: 842,150_450default: no value
        //break; case __LINE__: Trice        ( iD( 6208), "no value" ); //exp: time:       5_654default: no value
        //break; case __LINE__: trice        ( iD( 7544), "no value" ); //exp: time:            default: no value
        //break; case __LINE__: TRice8       ( iD( 5394), "no value" ); //exp: time: 842,150_450default: no value
        //break; case __LINE__: Trice8       ( iD( 6818), "no value" ); //exp: time:       5_654default: no value
        //break; case __LINE__: trice8       ( iD( 6939), "no value" ); //exp: time:            default: no value
        //break; case __LINE__: TRice16      ( iD( 2977), "no value" ); //exp: time: 842,150_450default: no value
        //break; case __LINE__: Trice16      ( iD( 3516), "no value" ); //exp: time:       5_654default: no value
        //break; case __LINE__: trice16      ( iD( 7417), "no value" ); //exp: time:            default: no value
        //break; case __LINE__: TRice32      ( iD( 3796), "no value" ); //exp: time: 842,150_450default: no value
        //break; case __LINE__: Trice32      ( iD( 4334), "no value" ); //exp: time:       5_654default: no value
        //break; case __LINE__: trice32      ( iD( 3748), "no value" ); //exp: time:            default: no value
        //break; case __LINE__: TRice64      ( iD( 3698), "no value" ); //exp: time: 842,150_450default: no value
        //break; case __LINE__: Trice64      ( iD( 6836), "no value" ); //exp: time:       5_654default: no value
        //break; case __LINE__: trice64      ( iD( 7756), "no value" ); //exp: time:            default: no value
        
        //break; case __LINE__: TRice_0      ( iD( 5940), "no value" ); //exp: time: 842,150_450default: no value
        //break; case __LINE__: Trice_0      ( iD( 4361), "no value" ); //exp: time:       5_654default: no value
        //break; case __LINE__: trice_0      ( iD( 5802), "no value" ); //exp: time:            default: no value
        //break; case __LINE__: TRice8_0     ( iD( 6938), "no value" ); //exp: time: 842,150_450default: no value
        //break; case __LINE__: Trice8_0     ( iD( 3581), "no value" ); //exp: time:       5_654default: no value
        //break; case __LINE__: trice8_0     ( iD( 6969), "no value" ); //exp: time:            default: no value
        //break; case __LINE__: TRice16_0    ( iD( 1592), "no value" ); //exp: time: 842,150_450default: no value
        //break; case __LINE__: Trice16_0    ( iD( 3459), "no value" ); //exp: time:       5_654default: no value
        //break; case __LINE__: trice16_0    ( iD( 3553), "no value" ); //exp: time:            default: no value
        //break; case __LINE__: TRice32_0    ( iD( 4063), "no value" ); //exp: time: 842,150_450default: no value
        //break; case __LINE__: Trice32_0    ( iD( 7460), "no value" ); //exp: time:       5_654default: no value
        //break; case __LINE__: trice32_0    ( iD( 7118), "no value" ); //exp: time:            default: no value
        //break; case __LINE__: TRice64_0    ( iD( 1033), "no value" ); //exp: time: 842,150_450default: no value
        //break; case __LINE__: Trice64_0    ( iD( 2213), "no value" ); //exp: time:       5_654default: no value
        //break; case __LINE__: trice64_0    ( iD( 1245), "no value" ); //exp: time:            default: no value

    //  break; case __LINE__: TRice( iD( 4173),"info:This is a message with one value %d and a 32-bit stamp.\n", -2  ); //exp: time: 842,150_450default: info:This is a message with one value -2 and a 32-bit stamp.
    //  break; case __LINE__: Trice( iD( 1813),"info:This is a message with one value %d and a 16-bit stamp.\n", -2  ); //exp: time:       5_654default: info:This is a message with one value -2 and a 16-bit stamp.
    //  break; case __LINE__: trice( iD( 1053),"info:This is a message with one value %d and without stamp.\n" , -2  ); //exp: time:            default: info:This is a message with one value -2 and without stamp.

        break; case __LINE__: TRICE( ID( 5185), "info:This is a message without values and a 32-bit stamp.\n" );        //exp: time: 842,150_450default: info:This is a message without values and a 32-bit stamp.
        break; case __LINE__: TRICE( Id( 7735), "info:This is a message without values and a 16-bit stamp.\n" );        //exp: time:       5_654default: info:This is a message without values and a 16-bit stamp.
        break; case __LINE__: TRICE( id( 5181), "info:This is a message without values and without stamp.\n"  );        //exp: time:            default: info:This is a message without values and without stamp.

        break; case __LINE__: TRICE( ID( 6451),"info:This is a message with one value %d and a 32-bit stamp.\n", -2  ); //exp: time: 842,150_450default: info:This is a message with one value -2 and a 32-bit stamp.
        break; case __LINE__: TRICE( Id( 6488),"info:This is a message with one value %d and a 16-bit stamp.\n", -2  ); //exp: time:       5_654default: info:This is a message with one value -2 and a 16-bit stamp.
        break; case __LINE__: TRICE( id( 7160),"info:This is a message with one value %d and without stamp.\n" , -2  ); //exp: time:            default: info:This is a message with one value -2 and without stamp.

        break; case __LINE__: TRICE( ID( 1181),"info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 32-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 );   //exp: time: 842,150_450default: info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and a 32-bit stamp.
        break; case __LINE__: TRICE( Id( 5850),"info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 16-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 );   //exp: time:       5_654default: info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and a 16-bit stamp.
        break; case __LINE__: TRICE( id( 4615),"info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and without stamp.\n" , -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 );   //exp: time:            default: info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and without stamp.

        break; case __LINE__: TRICE32( ID( 1205),"info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 32-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //exp: time: 842,150_450default: info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and a 32-bit stamp.
        break; case __LINE__: TRICE32( Id( 2410),"info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 16-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //exp: time:       5_654default: info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and a 16-bit stamp.
        break; case __LINE__: TRICE32( id( 3945),"info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and without stamp.\n" , -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //exp: time:            default: info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and without stamp.

        break; case __LINE__: TRICE8   ( id( 1087), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: TRICE8_1 ( id( 4533), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: TRICE8   ( Id( 5811), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRICE8_1 ( Id( 6417), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRICE8   ( ID( 3806), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: TRICE8_1 ( ID( 6634), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: trice8   ( iD( 6632), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: trice8_1 ( iD( 6071), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: Trice8   ( iD( 3835), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: Trice8_1 ( iD( 3182), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRice8   ( iD( 3616), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: TRice8_1 ( iD( 7143), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200

        break; case __LINE__: TRICE16  ( id( 2107), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: TRICE16_1( id( 6885), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: TRICE16  ( Id( 2424), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRICE16_1( Id( 1881), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRICE16  ( ID( 4138), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: TRICE16_1( ID( 7547), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: trice16  ( iD( 7074), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: trice16_1( iD( 7463), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: Trice16  ( iD( 7090), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: Trice16_1( iD( 7004), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRice16  ( iD( 3982), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: TRice16_1( iD( 2777), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200

        break; case __LINE__: TRICE32  ( id( 3534), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: TRICE32_1( id( 3989), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: TRICE32  ( Id( 2865), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRICE32_1( Id( 1217), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRICE32  ( ID( 4695), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: TRICE32_1( ID( 7900), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: trice32  ( iD( 2973), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: trice32_1( iD( 6837), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: Trice32  ( iD( 2747), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: Trice32_1( iD( 6083), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRice32  ( iD( 6153), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: TRice32_1( iD( 1440), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200

        break; case __LINE__: TRICE64  ( id( 2405), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: TRICE64_1( id( 7806), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: TRICE64  ( Id( 6297), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRICE64_1( Id( 2952), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRICE64  ( ID( 3594), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: TRICE64_1( ID( 2493), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: trice64  ( iD( 7872), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: trice64_1( iD( 7332), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: Trice64  ( iD( 4989), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: Trice64_1( iD( 2391), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRice64  ( iD( 6805), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: TRice64_1( iD( 6130), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200

        // mixed 16-bit and 32-bit timestamp 
        break; case __LINE__: TRice8( iD( 3354), "msg:value=%d\n", -1 );                                                                                            //exp: time: 842,150_450default: msg:value=-1
        break; case __LINE__: Trice8( iD( 5263), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: msg:value=-1, -2
        break; case __LINE__: trice8( iD( 4447), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: msg:value=-1, -2, -3
        break; case __LINE__: TRice8( iD( 2425), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice8( iD( 7307), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice8( iD( 2825), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice8( iD( 1207), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice8( iD( 6166), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice8( iD( 3737), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice8( iD( 7742), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice8( iD( 6750), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice8( iD( 7800), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice16( iD( 4373), "msg:value=%d\n", -1 );                                                                                            //exp: time: 842,150_450default: msg:value=-1
        break; case __LINE__: Trice16( iD( 3398), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: msg:value=-1, -2
        break; case __LINE__: trice16( iD( 3343), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: msg:value=-1, -2, -3
        break; case __LINE__: TRice16( iD( 3724), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice16( iD( 2833), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice16( iD( 4096), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice16( iD( 4487), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice16( iD( 1072), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice16( iD( 7835), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice16( iD( 5365), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice16( iD( 1910), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice16( iD( 4213), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice32( iD( 4193), "msg:value=%d\n", -1 );                                                                                            //exp: time: 842,150_450default: msg:value=-1
        break; case __LINE__: Trice32( iD( 1992), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: msg:value=-1, -2
        break; case __LINE__: trice32( iD( 6751), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: msg:value=-1, -2, -3
        break; case __LINE__: TRice32( iD( 5840), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice32( iD( 6474), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice32( iD( 4944), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice32( iD( 7115), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice32( iD( 1015), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice32( iD( 1249), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice32( iD( 1117), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice32( iD( 1878), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice32( iD( 1483), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice64( iD( 5944), "msg:value=%d\n", -1 );                                                                                            //exp: time: 842,150_450default: msg:value=-1
        break; case __LINE__: Trice64( iD( 5575), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: msg:value=-1, -2
        break; case __LINE__: trice64( iD( 3402), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: msg:value=-1, -2, -3
        break; case __LINE__: TRice64( iD( 5480), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice64( iD( 7380), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice64( iD( 7462), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice64( iD( 1557), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice64( iD( 6904), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice64( iD( 2251), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice64( iD( 7704), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice64( iD( 7488), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice64( iD( 6743), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice8_1 ( iD( 1270), "msg:value=%d\n", -1 );                                                                                            //exp: time: 842,150_450default: msg:value=-1
        break; case __LINE__: Trice8_2 ( iD( 6787), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: msg:value=-1, -2
        break; case __LINE__: trice8_3 ( iD( 7754), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: msg:value=-1, -2, -3
        break; case __LINE__: TRice8_4 ( iD( 6997), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice8_5 ( iD( 7374), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice8_6 ( iD( 6833), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice8_7 ( iD( 4564), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice8_8 ( iD( 6887), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice8_9 ( iD( 5249), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice8_10( iD( 4418), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice8_11( iD( 2858), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice8_12( iD( 7202), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice16_1 ( iD( 5858), "msg:value=%d\n", -1 );                                                                                            //exp: time: 842,150_450default: msg:value=-1
        break; case __LINE__: Trice16_2 ( iD( 7220), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: msg:value=-1, -2
        break; case __LINE__: trice16_3 ( iD( 7834), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: msg:value=-1, -2, -3
        break; case __LINE__: TRice16_4 ( iD( 5657), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice16_5 ( iD( 3632), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice16_6 ( iD( 7017), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice16_7 ( iD( 1920), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice16_8 ( iD( 4876), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice16_9 ( iD( 5015), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice16_10( iD( 1274), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice16_11( iD( 2014), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice16_12( iD( 7911), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice32_1 ( iD( 1107), "msg:value=%d\n", -1 );                                                                                            //exp: time: 842,150_450default: msg:value=-1
        break; case __LINE__: Trice32_2 ( iD( 1417), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: msg:value=-1, -2
        break; case __LINE__: trice32_3 ( iD( 4411), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: msg:value=-1, -2, -3
        break; case __LINE__: TRice32_4 ( iD( 4894), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice32_5 ( iD( 2420), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice32_6 ( iD( 6696), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice32_7 ( iD( 6306), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice32_8 ( iD( 7832), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice32_9 ( iD( 7617), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice32_10( iD( 5790), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice32_11( iD( 3023), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice32_12( iD( 1042), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice64_1 ( iD( 4508), "msg:value=%d\n", -1 );                                                                                            //exp: time: 842,150_450default: msg:value=-1
        break; case __LINE__: Trice64_2 ( iD( 4140), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: msg:value=-1, -2
        break; case __LINE__: trice64_3 ( iD( 5379), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: msg:value=-1, -2, -3
        break; case __LINE__: TRice64_4 ( iD( 3852), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice64_5 ( iD( 5945), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice64_6 ( iD( 1550), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice64_7 ( iD( 2765), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice64_8 ( iD( 6962), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice64_9 ( iD( 4674), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice64_10( iD( 4758), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice64_11( iD( 2492), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice64_12( iD( 4821), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        // use cases for 8-bit data
        break; case __LINE__: trice8     ( iD( 5751), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: Trice8     ( iD( 7665), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRice8     ( iD( 6481), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: trice8_1   ( iD( 7816), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: Trice8_1   ( iD( 2058), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRice8_1   ( iD( 4915), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2

        // use cases for 16-bit data
        break; case __LINE__: trice16    ( iD( 3296), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: Trice16    ( iD( 5256), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRice16    ( iD( 4914), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: trice16_1  ( iD( 3382), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: Trice16_1  ( iD( 3203), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRice16_1  ( iD( 7673), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2

        // use cases for 32-bit data
        break; case __LINE__: trice32    ( iD( 2671), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: Trice32    ( iD( 7327), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRice32    ( iD( 4127), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: trice32_1  ( iD( 5325), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: Trice32_1  ( iD( 1420), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRice32_1  ( iD( 1899), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2

        // use cases for 64-bit data
        break; case __LINE__: trice64    ( iD( 6477), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: Trice64    ( iD( 1658), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRice64    ( iD( 1834), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: trice64_1  ( iD( 4510), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: Trice64_1  ( iD( 3110), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRice64_1  ( iD( 2362), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2

        // legacy use cases
        break; case __LINE__: TRICE      ( ID( 2429),        "value=%d\n", -2 ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: TRICE      ( id( 2533), "value=%d\n", -2 ); //exp: time:            default: value=-2
        break; case __LINE__: TRICE      ( Id( 2843), "value=%d\n", -2 ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRICE      ( ID( 2895), "value=%d\n", -2 ); //exp: time: 842,150_450default: value=-2

        // default stamp
        break; case __LINE__: TRICE8     ( ID( 1324),        "value=%d\n", -2 ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: TRICE8_1   ( ID( 4710),        "value=%d\n", -2 ); //exp: time: 842,150_450default: value=-2

        // no stamp
        break; case __LINE__: trice8     ( iD( 5102),        "value=%d\n", -2 ); //exp: time:            default: value=-2
        break; case __LINE__: trice8_1   ( iD( 3861),        "value=%d\n", -2 ); //exp: time:            default: value=-2
        break; case __LINE__: TRICE8     ( id( 7627), "value=%d\n", -2 ); //exp: time:            default: value=-2
        break; case __LINE__: TRICE8_1   ( id( 6985), "value=%d\n", -2 ); //exp: time:            default: value=-2

        // 16-bit stamp:
        break; case __LINE__: Trice8     ( iD( 1964),        "value=%d\n", -2 ); //exp: time:       5_654default: value=-2
        break; case __LINE__: Trice8_1   ( iD( 3309),        "value=%d\n", -2 ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRICE8     ( Id( 6455), "value=%d\n", -2 ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRICE8_1   ( Id( 3583), "value=%d\n", -2 ); //exp: time:       5_654default: value=-2

        // 32-bit stamp:
        break; case __LINE__: TRice8     ( iD( 3538),        "value=%d\n", -2 ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: TRice8_1   ( iD( 4983),        "value=%d\n", -2 ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: TRICE8     ( ID( 2766), "value=%d\n", -2 ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: TRICE8_1   ( ID( 6793), "value=%d\n", -2 ); //exp: time: 842,150_450default: value=-2

        break; case __LINE__: TRICE( ID( 1051), "FATAL:magenta+b:red\n" );           //exp: time: 842,150_450default: FATAL:magenta+b:red
        break; case __LINE__: TRICE( ID( 6480), "CRITICAL:red+i:default+h\n" );      //exp: time: 842,150_450default: CRITICAL:red+i:default+h
        break; case __LINE__: TRICE( ID( 7574), "EMERGENCY:red+i:blue\n" );          //exp: time: 842,150_450default: EMERGENCY:red+i:blue
        break; case __LINE__: TRICE( ID( 6621), "ERROR:11:red\n" );                  //exp: time: 842,150_450default: ERROR:11:red
        break; case __LINE__: TRICE( ID( 1945), "WARNING:11+i:red\n" );              //exp: time: 842,150_450default: WARNING:11+i:red
        break; case __LINE__: TRICE( ID( 4165), "ATTENTION:11:green\n" );            //exp: time: 842,150_450default: ATTENTION:11:green
        break; case __LINE__: TRICE( ID( 1314), "INFO:cyan+b:default+h\n" );         //exp: time: 842,150_450default: INFO:cyan+b:default+h
        break; case __LINE__: TRICE( ID( 7148), "DEBUG:130+i\n" );                   //exp: time: 842,150_450default: DEBUG:130+i
        break; case __LINE__: TRICE( ID( 4141), "TRACE:default+i:default+h\n" );     //exp: time: 842,150_450default: TRACE:default+i:default+h
        break; case __LINE__: TRICE( ID( 4995), "TIME:blue+i:blue+h\n" );            //exp: time: 842,150_450default: TIME:blue+i:blue+h
        break; case __LINE__: TRICE( ID( 3101), "MESSAGE:green+h:black\n" );         //exp: time: 842,150_450default: MESSAGE:green+h:black
        break; case __LINE__: TRICE( ID( 2212), "READ:black+i:yellow+h\n" );         //exp: time: 842,150_450default: READ:black+i:yellow+h
        break; case __LINE__: TRICE( ID( 2454), "WRITE:black+u:yellow+h\n" );        //exp: time: 842,150_450default: WRITE:black+u:yellow+h
        break; case __LINE__: TRICE( ID( 1344), "RECEIVE:black+h:black\n" );         //exp: time: 842,150_450default: RECEIVE:black+h:black
        break; case __LINE__: TRICE( ID( 7924), "TRANSMIT:black:black+h\n" );        //exp: time: 842,150_450default: TRANSMIT:black:black+h
        break; case __LINE__: TRICE( ID( 5744), "DIAG:yellow+i:default+h\n" );       //exp: time: 842,150_450default: DIAG:yellow+i:default+h
        break; case __LINE__: TRICE( ID( 6902), "INTERRUPT:magenta+i:default+h\n" ); //exp: time: 842,150_450default: INTERRUPT:magenta+i:default+h
        break; case __LINE__: TRICE( ID( 7537), "SIGNAL:118+i\n" );                  //exp: time: 842,150_450default: SIGNAL:118+i
        break; case __LINE__: TRICE( ID( 5824), "TEST:yellow+h:black\n" );           //exp: time: 842,150_450default: TEST:yellow+h:black
        break; case __LINE__: TRICE( ID( 6769), "DEFAULT:off\n" );                   //exp: time: 842,150_450default: DEFAULT:off
        break; case __LINE__: TRICE( ID( 1081), "NOTICE:blue:white+h\n" );           //exp: time: 842,150_450default: NOTICE:blue:white+h
        break; case __LINE__: TRICE( ID( 1443), "ALERT:magenta:magenta+h\n" );       //exp: time: 842,150_450default: ALERT:magenta:magenta+h
        break; case __LINE__: TRICE( ID( 7811), "ASSERT:yellow+i:blue\n" );          //exp: time: 842,150_450default: ASSERT:yellow+i:blue
        break; case __LINE__: TRICE( ID( 2099), "ALARM:red+i:white+h\n" );           //exp: time: 842,150_450default: ALARM:red+i:white+h
        break; case __LINE__: TRICE( ID( 4187), "CYCLE:blue+i:default+h\n" );        //exp: time: 842,150_450default: CYCLE:blue+i:default+h
        break; case __LINE__: TRICE( ID( 7424), "VERBOSE:blue:default\n" );          //exp: time: 842,150_450default: VERBOSE:blue:default
        break; case __LINE__: TRICE( ID( 7254), "fatal:magenta+b:red\n" );           //exp: time: 842,150_450default: fatal:magenta+b:red
        break; case __LINE__: TRICE( ID( 5066), "critical:red+i:default+h\n" );      //exp: time: 842,150_450default: critical:red+i:default+h
        break; case __LINE__: TRICE( ID( 1682), "emergency:red+i:blue\n" );          //exp: time: 842,150_450default: emergency:red+i:blue
        break; case __LINE__: TRICE( ID( 7398), "error:11:red\n" );                  //exp: time: 842,150_450default: error:11:red
        break; case __LINE__: TRICE( ID( 4862), "warning:11+i:red\n" );              //exp: time: 842,150_450default: warning:11+i:red
        break; case __LINE__: TRICE( ID( 7027), "attention:11:green\n" );            //exp: time: 842,150_450default: attention:11:green
        break; case __LINE__: TRICE( ID( 1877), "info:cyan+b:default+h\n" );         //exp: time: 842,150_450default: info:cyan+b:default+h
        break; case __LINE__: TRICE( ID( 4706), "debug:130+i\n" );                   //exp: time: 842,150_450default: debug:130+i
        break; case __LINE__: TRICE( ID( 5093), "trace:default+i:default+h\n" );     //exp: time: 842,150_450default: trace:default+i:default+h
        break; case __LINE__: TRICE( ID( 6111), "time:blue+i:blue+h\n" );            //exp: time: 842,150_450default: time:blue+i:blue+h
        break; case __LINE__: TRICE( ID( 5113), "message:green+h:black\n" );         //exp: time: 842,150_450default: message:green+h:black
        break; case __LINE__: TRICE( ID( 4129), "read:black+i:yellow+h\n" );         //exp: time: 842,150_450default: read:black+i:yellow+h
        break; case __LINE__: TRICE( ID( 7670), "write:black+u:yellow+h\n" );        //exp: time: 842,150_450default: write:black+u:yellow+h
        break; case __LINE__: TRICE( ID( 4162), "receive:black+h:black\n" );         //exp: time: 842,150_450default: receive:black+h:black
        break; case __LINE__: TRICE( ID( 7908), "transmit:black:black+h\n" );        //exp: time: 842,150_450default: transmit:black:black+h
        break; case __LINE__: TRICE( ID( 5116), "diag:yellow+i:default+h\n" );       //exp: time: 842,150_450default: diag:yellow+i:default+h
        break; case __LINE__: TRICE( ID( 6905), "interrupt:magenta+i:default+h\n" ); //exp: time: 842,150_450default: interrupt:magenta+i:default+h
        break; case __LINE__: TRICE( ID( 3032), "signal:118+i\n" );                  //exp: time: 842,150_450default: signal:118+i
        break; case __LINE__: TRICE( ID( 4886), "test:yellow+h:black\n" );           //exp: time: 842,150_450default: test:yellow+h:black
        break; case __LINE__: TRICE( ID( 5235), "default:off\n" );                   //exp: time: 842,150_450default: default:off
        break; case __LINE__: TRICE( ID( 1164), "notice:blue:white+h\n" );           //exp: time: 842,150_450default: notice:blue:white+h
        break; case __LINE__: TRICE( ID( 3608), "alert:magenta:magenta+h\n" );       //exp: time: 842,150_450default: alert:magenta:magenta+h
        break; case __LINE__: TRICE( ID( 2529), "assert:yellow+i:blue\n" );          //exp: time: 842,150_450default: assert:yellow+i:blue
        break; case __LINE__: TRICE( ID( 1094), "alarm:red+i:white+h\n" );           //exp: time: 842,150_450default: alarm:red+i:white+h
        break; case __LINE__: TRICE( ID( 6987), "cycle:blue+i:default+h\n" );        //exp: time: 842,150_450default: cycle:blue+i:default+h
        break; case __LINE__: TRICE( ID( 3527), "verbose:blue:default\n" );          //exp: time: 842,150_450default: verbose:blue:default
        break; case __LINE__: TRICE64( id( 3539), "msg:%d (%%d)\n", -1 );     //exp: time:            default: msg:-1 (%d)

        break; case __LINE__: TRICE( ID( 2097), "sig:TRICE with 1 to 12 values\n" );
        break; case __LINE__: TRICE( ID( 1528), "rd:TRICE %d\n", -1 );
        break; case __LINE__: TRICE( ID( 3366), "rd:TRICE %d, %d\n", -1, -2 );
        break; case __LINE__: TRICE( ID( 1046), "rd:TRICE %d, %d, %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE( ID( 7448), "rd:TRICE %d, %d, %d, %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE( ID( 1701), "rd:TRICE %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE( ID( 5387), "rd:TRICE %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE( ID( 2964), "rd:TRICE %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE( ID( 2326), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE( ID( 3250), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE( ID( 5757), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE( ID( 4986), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE( ID( 3778), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE( ID( 7158),  "sig:TRICE_n with 1 to 12 values\n" );
        break; case __LINE__: TRICE_1( ID( 3658), "rd:TRICE_1 %d\n", 1 );
        break; case __LINE__: TRICE_2( ID( 4435), "rd:TRICE_2 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE_3( ID( 1233), "rd:TRICE_3 %d, %d, %d\n", 1, 2, 3 );
        break; case __LINE__: TRICE_4( ID( 3476), "rd:TRICE_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
        break; case __LINE__: TRICE_5( ID( 1766), "rd:TRICE_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
        break; case __LINE__: TRICE_6( ID( 5624), "rd:TRICE_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
        break; case __LINE__: TRICE_7( ID( 6003), "rd:TRICE_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
        break; case __LINE__: TRICE_8( ID( 7075), "rd:TRICE_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
        break; case __LINE__: TRICE_9( ID( 1039), "rd:TRICE_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
        break; case __LINE__: TRICE_10( ID( 3407), "rd:TRICE_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
        break; case __LINE__: TRICE_11( ID( 3880), "rd:TRICE_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
        break; case __LINE__: TRICE_12( ID( 2439), "rd:TRICE_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );				

        break; case __LINE__: TRICE( ID( 1197), "sig:TRICE8 with 1 to 12 values\n" );
        break; case __LINE__: TRICE8( ID( 1013), "rd:TRICE8 %d\n", -1 );
        break; case __LINE__: TRICE8( ID( 4424), "rd:TRICE8 %d, %d\n", -1, -2 );
        break; case __LINE__: TRICE8( ID( 7089), "rd:TRICE8 %d, %d, %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE8( ID( 3604), "rd:TRICE8 %d, %d, %d, %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE8( ID( 4636), "rd:TRICE8 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE8( ID( 6716), "rd:TRICE8 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE8( ID( 2597), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE8( ID( 7942), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE8( ID( 4472), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE8( ID( 1736), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE8( ID( 6200), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE8( ID( 6425), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE( ID( 3421), "sig:TRICE8_n with 1 to 12 values\n" );
        break; case __LINE__: TRICE8_1 ( ID( 2278), "rd:TRICE8_1 %d\n", 1 );
        break; case __LINE__: TRICE8_2 ( ID( 2593), "rd:TRICE8_2 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE8_3 ( ID( 1687), "rd:TRICE8_3 %d, %d, %d\n", 1, 2, 3 );
        break; case __LINE__: TRICE8_4 ( ID( 5124), "rd:TRICE8_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
        break; case __LINE__: TRICE8_5 ( ID( 6240), "rd:TRICE8_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
        break; case __LINE__: TRICE8_6 ( ID( 6363), "rd:TRICE8_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
        break; case __LINE__: TRICE8_7 ( ID( 5711), "rd:TRICE8_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
        break; case __LINE__: TRICE8_8 ( ID( 3462), "rd:TRICE8_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
        break; case __LINE__: TRICE8_9 ( ID( 2667), "rd:TRICE8_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
        break; case __LINE__: TRICE8_10( ID( 1374), "rd:TRICE8_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
        break; case __LINE__: TRICE8_11( ID( 3185), "rd:TRICE8_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
        break; case __LINE__: TRICE8_12( ID( 5980), "rd:TRICE8_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );

        break; case __LINE__: TRICE( ID( 7128), "sig:TRICE16 with 1 to 12 values\n" );
        break; case __LINE__: TRICE16( ID( 1604), "rd:TRICE16 %d\n", -1 );
        break; case __LINE__: TRICE16( ID( 3580), "rd:TRICE16 %d, %d\n", -1, -2 );
        break; case __LINE__: TRICE16( ID( 3736), "rd:TRICE16 %d, %d, %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE16( ID( 2724), "rd:TRICE16 %d, %d, %d, %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE16( ID( 7442), "rd:TRICE16 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE16( ID( 4432), "rd:TRICE16 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE16( ID( 4171), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE16( ID( 1102), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE16( ID( 3087), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE16( ID( 2336), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE16( ID( 2249), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE16( ID( 2224), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE     ( ID( 2507), "sig:TRICE16_n with 1 to 12 values\n" );
        break; case __LINE__: TRICE16_1 ( ID( 7303), "rd:TRICE16_1 %d\n", 1 );
        break; case __LINE__: TRICE16_2 ( ID( 3270), "rd:TRICE16_2 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE16_3 ( ID( 1244), "rd:TRICE16_3 %d, %d, %d\n", 1, 2, 3 );
        break; case __LINE__: TRICE16_4 ( ID( 3113), "rd:TRICE16_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
        break; case __LINE__: TRICE16_5 ( ID( 6533), "rd:TRICE16_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
        break; case __LINE__: TRICE16_6 ( ID( 4430), "rd:TRICE16_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
        break; case __LINE__: TRICE16_7 ( ID( 7223), "rd:TRICE16_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
        break; case __LINE__: TRICE16_8 ( ID( 6848), "rd:TRICE16_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
        break; case __LINE__: TRICE16_9 ( ID( 5338), "rd:TRICE16_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
        break; case __LINE__: TRICE16_10( ID( 4070), "rd:TRICE16_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
        break; case __LINE__: TRICE16_11( ID( 1169), "rd:TRICE16_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
        break; case __LINE__: TRICE16_12( ID( 3447), "rd:TRICE16_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );

        break; case __LINE__: TRICE  ( ID( 3295), "sig:TRICE16 with 1 to 12 pointer\n" );
        break; case __LINE__: TRICE16( ID( 7260), "rd:TRICE16 %p\n", -1 );
        break; case __LINE__: TRICE16( ID( 2300), "rd:TRICE16 %p, %p\n", -1, -2 );
        break; case __LINE__: TRICE16( ID( 6045), "rd:TRICE16 %p, %p, %p\n", -1, -2, -3 );
        break; case __LINE__: TRICE16( ID( 6356), "rd:TRICE16 %p, %p, %p, %p\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE16( ID( 1651), "rd:TRICE16 %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE16( ID( 1467), "rd:TRICE16 %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE16( ID( 5316), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE16( ID( 1372), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE16( ID( 1353), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE16( ID( 4312), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE16( ID( 6717), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE16( ID( 6419), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE  ( ID( 5444), "sig:TRICE16 with 1 to 12 hex\n" );
        break; case __LINE__: TRICE16( ID( 4066), "rd:TRICE16 %X\n", -1 );
        break; case __LINE__: TRICE16( ID( 5500), "rd:TRICE16 %X, %X\n", -1, -2 );
        break; case __LINE__: TRICE16( ID( 6362), "rd:TRICE16 %X, %X, %X\n", -1, -2, -3 );
        break; case __LINE__: TRICE16( ID( 6561), "rd:TRICE16 %X, %X, %X, %X\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE16( ID( 1045), "rd:TRICE16 %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE16( ID( 7822), "rd:TRICE16 %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE16( ID( 5807), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE16( ID( 6447), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE16( ID( 7085), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE16( ID( 6975), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE16( ID( 1571), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE16( ID( 3651), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE  ( ID( 4717), "sig:TRICE32 with 1 to 12 values\n" );
        break; case __LINE__: TRICE32( ID( 7061), "rd:TRICE32 %d\n", -1 );
        break; case __LINE__: TRICE32( ID( 3815), "rd:TRICE32 %d, %d\n", -1, -2 );
        break; case __LINE__: TRICE32( ID( 6135), "rd:TRICE32 %d, %d, %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE32( ID( 7743), "rd:TRICE32 %d, %d, %d, %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE32( ID( 2931), "rd:TRICE32 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE32( ID( 2161), "rd:TRICE32 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE32( ID( 1154), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE32( ID( 2091), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE32( ID( 6106), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE32( ID( 5941), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE32( ID( 7718), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE32( ID( 3315), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE     ( ID( 3000), "signal:TRICE32_n with 1 to 12 values\n" );
        break; case __LINE__: TRICE32_1 ( ID( 2835), "rd:TRICE32_1 %d\n", 1 );
        break; case __LINE__: TRICE32_2 ( ID( 3788), "rd:TRICE32_2 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE32_3 ( ID( 2077), "rd:TRICE32_3 %d, %d, %d\n", 1, 2, 3 );
        break; case __LINE__: TRICE32_4 ( ID( 2985), "rd:TRICE32_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
        break; case __LINE__: TRICE32_5 ( ID( 7972), "rd:TRICE32_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
        break; case __LINE__: TRICE32_6 ( ID( 7644), "rd:TRICE32_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
        break; case __LINE__: TRICE32_7 ( ID( 2217), "rd:TRICE32_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
        break; case __LINE__: TRICE32_8 ( ID( 1464), "rd:TRICE32_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
        break; case __LINE__: TRICE32_9 ( ID( 3828), "rd:TRICE32_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
        break; case __LINE__: TRICE32_10( ID( 6536), "rd:TRICE32_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
        break; case __LINE__: TRICE32_11( ID( 4770), "rd:TRICE32_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
        break; case __LINE__: TRICE32_12( ID( 6453), "rd:TRICE32_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );

        break; case __LINE__: TRICE  ( ID( 7226), "sig:TRICE64 with 1 to 12 values\n" );
        break; case __LINE__: TRICE64( ID( 7490), "rd:TRICE64 %d\n", -1 );
        break; case __LINE__: TRICE64( ID( 2723), "rd:TRICE64 %d, %d\n", -1, -2 );
        break; case __LINE__: TRICE64( ID( 6512), "rd:TRICE64 %d, %d, %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE64( ID( 6724), "rd:TRICE64 %d, %d, %d, %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE64( ID( 5187), "rd:TRICE64 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE64( ID( 3947), "rd:TRICE64 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE64( ID( 4549), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE64( ID( 3572), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE64( ID( 1916), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE64( ID( 3436), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE64( ID( 3126), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE64( ID( 4616), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE     ( ID( 7221), "sig:TRICE64_n with 1 to 12 values\n" );
        break; case __LINE__: TRICE64_1 ( ID( 4263), "rd:TRICE64_1 %d\n", 1 );
        break; case __LINE__: TRICE64_2 ( ID( 5697), "rd:TRICE64_2 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE64_2 ( ID( 2992), "rd:TRICE64_2 %x, %x\n", 0x1122334455667788, 0x0102030405060708 );
        break; case __LINE__: TRICE64_3 ( ID( 4601), "rd:TRICE64_3 %d, %d, %d\n", 1, 2, 3 );
        break; case __LINE__: TRICE64_4 ( ID( 1532), "rd:TRICE64_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
        break; case __LINE__: TRICE64_5 ( ID( 7356), "rd:TRICE64_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
        break; case __LINE__: TRICE64_6 ( ID( 2423), "rd:TRICE64_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
        break; case __LINE__: TRICE64_7 ( ID( 2800), "rd:TRICE64_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
        break; case __LINE__: TRICE64_8 ( ID( 7510), "rd:TRICE64_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
        break; case __LINE__: TRICE64_9 ( ID( 2093), "rd:TRICE64_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
        break; case __LINE__: TRICE64_10( ID( 3884), "rd:TRICE64_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
        break; case __LINE__: TRICE64_11( ID( 6966), "rd:TRICE64_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
        break; case __LINE__: TRICE64_12( ID( 7140), "rd:TRICE64_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );

        break; case __LINE__: TRICE( ID( 5827), "sig:Colors and ticks\n" );
        break; case __LINE__: TRICE( ID( 7707), "--------------------------------------------------\n");
        break; case __LINE__: TRICE( ID( 3592), "--------------------------------------------------\n");
        break; case __LINE__: TRICE( ID( 5458), "dbg:12345 as 16bit is %016b\n", 12345);
        break; case __LINE__: TRICE( ID( 2578), "--------------------------------------------------\n");
        break; case __LINE__: TRICE( ID( 2867), "sig:This ASSERT error is just a demo and no real error:\n");
        break; case __LINE__: TRICE( ID( 6327), "--------------------------------------------------\n");
        break; case __LINE__: TRICE( ID( 1546), "ERR:error       message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 3684), "WRN:warning     message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 4912), "ATT:attention   message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 6549), "DIA:diagnostics message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 5586), "TIM:timing      message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 5673), "DBG:debug       message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 3900), "SIG:signal      message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 5357), "RD:read         message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 5414), "WR:write        message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 3143), "ISR:interrupt   message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 6937), "MSG:normal      message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 5399), "INFO:informal   message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 7066), "time:TRICE32_1  message, SysTick is %6u\n", SYSTICKVAL);

        break; case __LINE__: TRICE( ID( 3133), "sig:Various ranges and formats\n" );
        break; case __LINE__: TRICE8 ( ID( 1088), "tst:TRICE8  %%03x -> %03x  %03x  %03x  %03x\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID( 5698), "tst:TRICE8   %%4d -> %4d %4d %4d %4d\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID( 6191), "tst:TRICE8   %%4u -> %4u %4u %4u %4u\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID( 2158), "tst:TRICE8   %%4o -> %4o %4o %4o %4o\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID( 3450), "tst:TRICE8   %%4O -> %4O %4O %4O %4O\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID( 7733), "tst:TRICE8   %%4X -> %4X %4X %4X %4X\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID( 3877), "tst:TRICE8   %%4x -> %4x %4x %4x %4x\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID( 6731), "tst:TRICE8   %%8b -> %8b %8b %8b %8b\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID( 7270), "tst:TRICE8  %%08b -> %08b %08b %08b %08b\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE16( ID( 6920), "tst:TRICE16  %%05x ->   %05x   %05x   %05x   %05x\n", 1, 0x7fff, 0x8000, 0xffff);
        break; case __LINE__: TRICE16( ID( 4054), "tst:TRICE16   %%6d ->  %6d  %6d  %6d  %6d\n",         1, 0x7fff, 0x8000, 0xffff);
        break; case __LINE__: TRICE16( ID( 2658), "tst:TRICE16   %%7o -> %7o %7o %7o %7o\n",             1, 0x7fff, 0x8000, 0xffff);
        break; case __LINE__: TRICE32( ID( 4688), "tst:TRICE32 %%09x ->      %09x      %09x       %09x     %09x\n", 1, 0x7fffffff, 0x80000000, 0xffffffff);
        break; case __LINE__: TRICE32( ID( 6507), "tst:TRICE32 %%10d ->     %10d     %10d     %10d    %10x\n",      1, 0x7fffffff, 0x80000000, 0xffffffff);
        break; case __LINE__: TRICE32( ID( 1296), "att:TRICE32 %40b\n", 0xAAAAAAAA);
        break; case __LINE__: TRICE64( ID( 5040), "tst:TRICE64 %%09x ->      %09x      %09x       %09x     %09x\n", 1, 0x7fffffffffffffff, 0x8000000000000000, 0xffffffffffffffff);
        break; case __LINE__: TRICE64( ID( 1689), "tst:TRICE64 %%10d ->     %10d     %10d     %10d    %10x\n",      1, 0x7fffffffffffffff, 0x8000000000000000, 0xffffffffffffffff);
        break; case __LINE__: TRICE64( ID( 4911), "att:TRICE64 %70b\n", 0xAAAAAAAAAAAAAAAA);
        break; case __LINE__: TRICE( ID( 1826), "sig:Legacy TRICE8\n" );
        break; case __LINE__: TRICE8_1( ID( 6977), "tst:TRICE8_1 %02x\n", 0xA1);
        break; case __LINE__: TRICE8_2( ID( 6565), "tst:TRICE8_2 %02x %02x\n", 0xA1, 0xA2);
        break; case __LINE__: TRICE8_3( ID( 4358), "tst:TRICE8_3 %02x %02x %02x\n", 0xA1, 0xA2, 0xA3);
        break; case __LINE__: TRICE8_4( ID( 4611), "tst:TRICE8_4 %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4);
        break; case __LINE__: TRICE8_5( ID( 7459), "tst:TRICE8_5 %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5 );
        break; case __LINE__: TRICE8_6( ID( 3891), "tst:TRICE8_6 %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6);
        break; case __LINE__: TRICE8_7( ID( 6616), "tst:TRICE8_7 %02x %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7);
        break; case __LINE__: TRICE8_8( ID( 6207), "tst:TRICE8_8 %02x %02x %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8);
        break; case __LINE__: TRICE8_1( ID( 5447), "tst:TRICE8_1 %d\n",                      -1);
        break; case __LINE__: TRICE8_2( ID( 6213), "tst:TRICE8_2 %d %d\n",                   -1, -2);
        break; case __LINE__: TRICE8_3( ID( 2728), "tst:TRICE8_3 %d %d %d\n",                -1, -2, -3);
        break; case __LINE__: TRICE8_4( ID( 1583), "tst:TRICE8_4 %d %d %d %d\n",             -1, -2, -3, -4);
        break; case __LINE__: TRICE8_5( ID( 2639), "tst:TRICE8_5 %d %d %d %d %d\n",          -1, -2, -3, -4, -5);
        break; case __LINE__: TRICE8_6( ID( 2726), "tst:TRICE8_6 %d %d %d %d %d %d\n",       -1, -2, -3, -4, -5, -6);
        break; case __LINE__: TRICE8_7( ID( 6984), "tst:TRICE8_7 %d %d %d %d %d %d %d\n",    -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE8_8( ID( 7587), "tst:TRICE8_8 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8);
        break; case __LINE__: TRICE8_1( ID( 7591), "tst:TRICE8_1 %d\n", 1);
        break; case __LINE__: TRICE8_2( ID( 3452), "tst:TRICE8_2 %d %d\n", 1, 2);
        break; case __LINE__: TRICE8_3( ID( 5766), "tst:TRICE8_3 %d %d %d\n", 1, 2, 3);
        break; case __LINE__: TRICE8_4( ID( 3165), "tst:TRICE8_4 %d %d %d %d\n", 1, 2, 3, 4);
        break; case __LINE__: TRICE8_5( ID( 5237), "tst:TRICE8_5 %d %d %d %d %d\n", 1, 2, 3, 4, 5);
        break; case __LINE__: TRICE8_6( ID( 7400), "tst:TRICE8_6 %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6);
        break; case __LINE__: TRICE8_7( ID( 6301), "tst:TRICE8_7 %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7);
        break; case __LINE__: TRICE8_8( ID( 3244), "tst:TRICE8_8 %d %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7, 8);
        break; case __LINE__: TRICE8_1( ID( 4477), "tst:TRICE8_1 %u\n", 201);
        break; case __LINE__: TRICE8_2( ID( 3061), "tst:TRICE8_2 %u %u\n", 201, 202);
        break; case __LINE__: TRICE8_3( ID( 7962), "tst:TRICE8_3 %u %u %u\n", 201, 202, 203);
        break; case __LINE__: TRICE8_4( ID( 6623), "tst:TRICE8_4 %u %u %u %u\n", 201, 202, 203, 204);
        break; case __LINE__: TRICE8_5( ID( 6119), "tst:TRICE8_5 %u %u %u %u %u\n", 201, 202, 203, 204, 205);
        break; case __LINE__: TRICE8_6( ID( 6963), "tst:TRICE8_6 %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206);
        break; case __LINE__: TRICE8_7( ID( 7740), "tst:TRICE8_7 %u %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206, 207);
        break; case __LINE__: TRICE8_8( ID( 5417), "tst:TRICE8_8 %u %u %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206, 207, 208);
        break; case __LINE__: TRICE8_8( ID( 4571), "tst:TRICE8_1 %%d=%d, %%u=%u, 0x%%x=0x%x, 0x%%2x=0x%2x, 0x%%02x=0x%02x, 0x%%3x=0x%3x, 0x%%03x=0x%03x, %%b=%b\n", 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81);

        break; case __LINE__: TRICE    ( ID( 2052), "sig:Legacy TRICE16-64\n" );
        break; case __LINE__: TRICE16_1( ID( 7430), "tst:TRICE16_1 %d\n", -111);
        break; case __LINE__: TRICE16_2( ID( 4130), "tst:TRICE16_2 %d %d\n", -111, -222);
        break; case __LINE__: TRICE16_3( ID( 1085), "tst:TRICE16_3 %d %d %d\n", -111, -222, -333);
        break; case __LINE__: TRICE16_4( ID( 1478), "tst:TRICE16_4 %d %d %d %d\n", -111, -222, -333, -444);
        break; case __LINE__: TRICE32_1( ID( 1517), "tst:TRICE32_1 %08x\n", 0x0123cafe);
        break; case __LINE__: TRICE32_1( ID( 4148), "tst:TRICE32_1 %d\n", -111);
        break; case __LINE__: TRICE32_2( ID( 1619), "tst:TRICE32_2 %x %x\n", -111, -222);
        break; case __LINE__: TRICE32_2( ID( 5667), "tst:TRICE32_2 %d %d\n", -111, -222);
        break; case __LINE__: TRICE32_3( ID( 5611), "tst:TRICE32_3 %x %x %x\n", -111, -222, -333);
        break; case __LINE__: TRICE32_3( ID( 3457), "tst:TRICE32_3 %d %d %d\n", -111, -222, -333);
        break; case __LINE__: TRICE32_4( ID( 4436), "tst:TRICE32_4 %x %x %x %x\n", -111, -222, -333, -444);
        break; case __LINE__: TRICE32_4( ID( 4420), "tst:TRICE32_4 %d %d %d %d\n", -111, -222, -333, -444);
        break; case __LINE__: TRICE64_1( ID( 2795), "tst:TRICE64_1 %d\n", -111);
        break; case __LINE__: TRICE64_2( ID( 2976), "tst:TRICE64_2 %d %d\n", -111, -222);
        break; case __LINE__: TRICE16_1( ID( 1122), "tst:TRICE16_1 %u\n", 60001);
        break; case __LINE__: TRICE16_2( ID( 5078), "tst:TRICE16_2 %u %u\n", 60001, 60002);
        break; case __LINE__: TRICE16_3( ID( 4051), "tst:TRICE16_3 %u %u %u\n", 60001, 60002, 60003);
        break; case __LINE__: TRICE16_4( ID( 4204), "tst:TRICE16_4 %u %u %u %u\n", 60001, 60002, 60003, 60004);
        break; case __LINE__: TRICE32_1( ID( 1838), "tst:TRICE32_1 %u\n", 4000000001);
        break; case __LINE__: TRICE32_2( ID( 3187), "tst:TRICE32_2 %u %u\n", 4000000001, 4000000002);
        break; case __LINE__: TRICE32_3( ID( 6999), "tst:TRICE32_3 %u %u %u\n", 4000000001, 4000000002, 4000000003);
        break; case __LINE__: TRICE32_4( ID( 1663), "tst:TRICE32_4 %u %u %u %u\n", 4000000001, 4000000002, 4000000003, 4000000004);
        break; case __LINE__: TRICE64_1( ID( 6732), "tst:TRICE64_1 %x\n", -1);
        break; case __LINE__: TRICE64_2( ID( 2143), "tst:TRICE64_2 %x %x\n", -1, -2);
        break; case __LINE__: TRICE64_1( ID( 6386), "tst:TRICE64_1 %u\n", -1);
        break; case __LINE__: TRICE64_2( ID( 3001), "tst:TRICE64_2 %u %u\n", -1, -2);

        break; case __LINE__: TRICE( ID( 2618), "sig:colored single letters and several TRICE macros in one line\n" );
        break; case __LINE__: TRICE( ID( 5546), "e:A");
        break; case __LINE__: TRICE( ID( 6726), "w:B");
        break; case __LINE__: TRICE( ID( 1833), "a:c");
        break; case __LINE__: TRICE( ID( 1096), "wr:d");
        break; case __LINE__: TRICE( ID( 3555), "rd:e\n");
        break; case __LINE__: TRICE( ID( 2524), "diag:f");
        break; case __LINE__: TRICE( ID( 7923), "d:G");
        break; case __LINE__: TRICE( ID( 4169), "t:H");
        break; case __LINE__: TRICE( ID( 2842), "time:i");
        break; case __LINE__: TRICE( ID( 2669), "message:J");
        break; case __LINE__: TRICE( ID( 4500), "dbg:k\n");
        break; case __LINE__: TRICE( ID( 4164), "1");
        break; case __LINE__: TRICE( ID( 5985), "2");
        break; case __LINE__: TRICE( ID( 5292), "3");
        break; case __LINE__: TRICE( ID( 6779), "4");
        break; case __LINE__: TRICE( ID( 2489), "e:7");
        break; case __LINE__: TRICE( ID( 5446), "m:12");
        break; case __LINE__: TRICE( ID( 1075), "m:123\n");
        break; case __LINE__: TRICE( ID( 2322), "e:A"); TRICE( ID( 4708), "w:B"); TRICE( ID( 1738), "a:c");
        break; case __LINE__: TRICE( ID( 3364), "wr:d"); TRICE( ID( 3512), "rd:e\n"); TRICE( ID( 6497), "diag:f");

        break; case __LINE__: TRICE( ID( 4308),  "sig:TRICE8 with variable param count 1 to 12\n" );
        break; case __LINE__: TRICE8( ID( 7384), "tst:TRICE8 %d\n", -1 );
        break; case __LINE__: TRICE8( ID( 1288), "tst:TRICE8 %d %d\n", -1, -2 );
        break; case __LINE__: TRICE8( ID( 6850), "tst:TRICE8 %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE8( ID( 7805), "tst:TRICE8 %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE8( ID( 7487), "tst:TRICE8 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE8( ID( 2137), "tst:TRICE8 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE8( ID( 6081), "tst:TRICE8 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE8( ID( 3942), "tst:TRICE8 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE8( ID( 5179), "tst:TRICE8 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE8( ID( 5693), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE8( ID( 1227), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE8( ID( 7231), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE8( ID( 5463), "tst:TRICE8 %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );
        break; case __LINE__: TRICE8_1 ( ID( 4709), "tst:TRICE8_1  %d\n", -1 );
        break; case __LINE__: TRICE8_2 ( ID( 7193), "tst:TRICE8_2  %d %d\n", -1, -2 );
        break; case __LINE__: TRICE8_3 ( ID( 3235), "tst:TRICE8_3  %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE8_4 ( ID( 4816), "tst:TRICE8_4  %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE8_5 ( ID( 4237), "tst:TRICE8_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE8_6 ( ID( 6545), "tst:TRICE8_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE8_7 ( ID( 6942), "tst:TRICE8_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE8_8 ( ID( 5891), "tst:TRICE8_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE8_9 ( ID( 7375), "tst:TRICE8_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE8_10( ID( 1871), "tst:TRICE8_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE8_11( ID( 7492), "tst:TRICE8_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE8_12( ID( 6047), "tst:TRICE8_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE8   ( ID( 3998), "tst:TRICE8   %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );
        break; case __LINE__: TRICE8_9 ( ID( 2993), "tst:TRICE8_9 %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );

        break; case __LINE__: TRICE( ID( 2752), "sig:TRICE16 with variable param count 1 to 12\n" );
        break; case __LINE__: TRICE16( ID( 1367), "tst:TRICE16 %d\n", -1 );
        break; case __LINE__: TRICE16( ID( 6609), "tst:TRICE16 %d %d\n", -1, -2 );
        break; case __LINE__: TRICE16( ID( 3721), "tst:TRICE16 %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE16( ID( 7548), "tst:TRICE16 %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE16( ID( 3795), "tst:TRICE16 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE16( ID( 7519), "tst:TRICE16 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE16( ID( 1070), "tst:TRICE16 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE16( ID( 5300), "tst:TRICE16 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE16( ID( 3826), "tst:TRICE16 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE16( ID( 2105), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE16( ID( 4252), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE16( ID( 4415), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE16( ID( 7853), "tst:TRICE16 %t %b %x %X %d %u %o %O %p\n", 3, 3, 3, 3, 3, 3, 3, 3, 3 );
        break; case __LINE__: TRICE16_1 ( ID( 1671), "tst:TRICE16_1  %d\n", -1 );
        break; case __LINE__: TRICE16_2 ( ID( 5246), "tst:TRICE16_2  %d %d\n", -1, -2 );
        break; case __LINE__: TRICE16_3 ( ID( 6133), "tst:TRICE16_3  %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE16_4 ( ID( 2890), "tst:TRICE16_4  %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE16_5 ( ID( 6462), "tst:TRICE16_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE16_6 ( ID( 4236), "tst:TRICE16_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE16_7 ( ID( 5303), "tst:TRICE16_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE16_8 ( ID( 1531), "tst:TRICE16_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE16_9 ( ID( 4339), "tst:TRICE16_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE16_10( ID( 7297), "tst:TRICE16_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE16_11( ID( 2140), "tst:TRICE16_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE16_12( ID( 3917), "tst:TRICE16_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE16_9 ( ID( 5954), "tst:TRICE16 %t %b %x %X %d %u %o %O %p\n", 3, 3, 3, 3, 3, 3, 3, 3, 3 );

        break; case __LINE__: TRICE( ID( 5413), "sig:TRICE32 with variable param count 1 to 12\n" );
        break; case __LINE__: TRICE32( ID( 3210), "tst:TRICE32 %d\n", -1 );
        break; case __LINE__: TRICE32( ID( 6220), "tst:TRICE32 %d %d\n", -1, -2 );
        break; case __LINE__: TRICE32( ID( 4385), "tst:TRICE32 %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE32( ID( 7243), "tst:TRICE32 %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE32( ID( 7052), "tst:TRICE32 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE32( ID( 4945), "tst:TRICE32 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE32( ID( 6897), "tst:TRICE32 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE32( ID( 5271), "tst:TRICE32 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE32( ID( 5538), "tst:TRICE32 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE32( ID( 3442), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE32( ID( 2958), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE32( ID( 3348), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE32( ID( 5058), "tst:TRICE32 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));
        break; case __LINE__: TRICE32_1 ( ID( 6279), "tst:TRICE32_1  %d\n", -1 );
        break; case __LINE__: TRICE32_2 ( ID( 1385), "tst:TRICE32_2  %d %d\n", -1, -2 );
        break; case __LINE__: TRICE32_3 ( ID( 7182), "tst:TRICE32_3  %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE32_4 ( ID( 4820), "tst:TRICE32_4  %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE32_5 ( ID( 1769), "tst:TRICE32_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE32_6 ( ID( 3908), "tst:TRICE32_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE32_7 ( ID( 6237), "tst:TRICE32_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE32_8 ( ID( 4596), "tst:TRICE32_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE32_9 ( ID( 6547), "tst:TRICE32_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE32_10( ID( 7347), "tst:TRICE32_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE32_11( ID( 1612), "tst:TRICE32_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE32_12( ID( 2305), "tst:TRICE32_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE32   ( ID( 2782), "tst:TRICE32    %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));
        break; case __LINE__: TRICE32_12( ID( 7695), "tst:TRICE32_12 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));

        break; case __LINE__: TRICE( ID( 7209), "sig:TRICE64 with variable param count 1 to 12\n" );
        break; case __LINE__: TRICE64( ID( 4657), "tst:TRICE64 %d\n", -1 );
        break; case __LINE__: TRICE64( ID( 1986), "tst:TRICE64 %d %d\n", -1, -2 );
        break; case __LINE__: TRICE64( ID( 6036), "tst:TRICE64 %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE64( ID( 3800), "tst:TRICE64 %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE64( ID( 1218), "tst:TRICE64 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE64( ID( 7878), "tst:TRICE64 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE64( ID( 3885), "tst:TRICE64 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE64( ID( 6668), "tst:TRICE64 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE64( ID( 7282), "tst:TRICE64 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE64( ID( 4679), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE64( ID( 6813), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE64( ID( 2281), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE64( ID( 7049), "tst:TRICE64 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aDouble(3.14159), aDouble(3.14159), aDouble(3.14159));
        break; case __LINE__: TRICE64_1 ( ID( 3849), "tst:TRICE64_1  %d\n", -1 );
        break; case __LINE__: TRICE64_2 ( ID( 7725), "tst:TRICE64_2  %d %d\n", -1, -2 );
        break; case __LINE__: TRICE64_3 ( ID( 5290), "tst:TRICE64_3  %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE64_4 ( ID( 2822), "tst:TRICE64_4  %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE64_5 ( ID( 4693), "tst:TRICE64_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE64_6 ( ID( 2054), "tst:TRICE64_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE64_7 ( ID( 7359), "tst:TRICE64_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE64_8 ( ID( 6537), "tst:TRICE64_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE64_9 ( ID( 3065), "tst:TRICE64_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE64_10( ID( 1789), "tst:TRICE64_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE64_11( ID( 5619), "tst:TRICE64_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE64_12( ID( 2989), "tst:TRICE64_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE64_12( ID( 5993), "tst:TRICE64_12 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aDouble(3.14159), aDouble(3.14159), aDouble(3.14159));

        break; case __LINE__: TRICE( ID( 1503), "att:positive and negative float in format variants\n");
        break; case __LINE__: TRICE( ID( 7844), "rd:TRICE float %f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 1724), "rd:TRICE float %9f (%%9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 5441), "rd:TRICE float %.9f (%%.9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 3397), "rd:TRICE float %9.f (%%9.f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 3194), "rd:TRICE float %9.6f (%%9.6f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 7956), "rd:TRICE float %f (%%f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 3709), "rd:TRICE float %9f (%%9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 6336), "rd:TRICE float %.9f (%%.9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 3062), "rd:TRICE float %9.f (%%9.f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 5164), "rd:TRICE float %9.6f (%%9.6f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 3903), "rd:TRICE float %+f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 1301), "rd:TRICE float %+9f (%%9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 4292), "rd:TRICE float %+.9f (%%.9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 5048), "rd:TRICE float %+9.f (%%9.f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 3582), "rd:TRICE float %+9.6f (%%9.6f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 2805), "rd:TRICE float %+f (%%f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 3961), "rd:TRICE float %+9f (%%9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 5118), "rd:TRICE float %+.9f (%%.9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 3033), "rd:TRICE float %+9.f (%%9.f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 5669), "rd:TRICE float %+9.6f (%%9.6f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 2887), "rd:TRICE float %-f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 5072), "rd:TRICE float %-9f (%%9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 5025), "rd:TRICE float %-.9f (%%.9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 5722), "rd:TRICE float %-9.f (%%9.f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 6690), "rd:TRICE float %-9.6f (%%9.6f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 2335), "rd:TRICE float %-f (%%f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 7296), "rd:TRICE float %-9f (%%9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 1589), "rd:TRICE float %-.9f (%%.9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 3824), "rd:TRICE float %-9.f (%%9.f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 4778), "rd:TRICE float %-9.6f (%%9.6f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 3274), "att:positive float & double in variants\n");
        break; case __LINE__: TRICE32_1( ID( 2686), "rd:TRICE32_1 float %e (%%e)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1( ID( 4667), "rd:TRICE32_1 float %f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1( ID( 7320), "rd:TRICE32_1 float %g (%%g)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1( ID( 5524), "rd:TRICE32_1 float %E (%%E)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1( ID( 6269), "rd:TRICE32_1 float %F (%%F)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1( ID( 5395), "rd:TRICE32_1 float %G (%%G)\n", aFloat(x) );
        break; case __LINE__: TRICE32  ( ID( 7292), "rd:TRICE32 float %e (%%e)\n", aFloat(x) );
        break; case __LINE__: TRICE32  ( ID( 7056), "rd:TRICE32 float %f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE32  ( ID( 4124), "rd:TRICE32 float %g (%%g)\n", aFloat(x) );
        break; case __LINE__: TRICE32  ( ID( 2370), "rd:TRICE32 float %E (%%E)\n", aFloat(x) );
        break; case __LINE__: TRICE32  ( ID( 6596), "rd:TRICE32 float %F (%%F)\n", aFloat(x) );
        break; case __LINE__: TRICE32  ( ID( 3045), "rd:TRICE32 float %G (%%G)\n", aFloat(x) );
        break; case __LINE__: TRICE    ( ID( 3871), "rd:TRICE float %e (%%e)\n", aFloat(x) );
        break; case __LINE__: TRICE    ( ID( 3055), "rd:TRICE float %f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE    ( ID( 7269), "rd:TRICE float %g (%%g)\n", aFloat(x) );
        break; case __LINE__: TRICE    ( ID( 4643), "rd:TRICE float %E (%%E)\n", aFloat(x) );
        break; case __LINE__: TRICE    ( ID( 3508), "rd:TRICE float %F (%%F)\n", aFloat(x) );
        break; case __LINE__: TRICE    ( ID( 4406), "rd:TRICE float %G (%%G)\n", aFloat(x) );
        break; case __LINE__: TRICE64  ( ID( 1231), "rd:TRICE64 double %e (%%e), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64  ( ID( 5489), "rd:TRICE64 double %f (%%f), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64  ( ID( 3220), "rd:TRICE64 double %g (%%g), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64  ( ID( 5962), "rd:TRICE64 double %E (%%E), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64  ( ID( 2844), "rd:TRICE64 double %F (%%F), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64  ( ID( 7672), "rd:TRICE64 double %G (%%G), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( ID( 7535), "rd:TRICE64_1 double %e (%%e), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( ID( 5976), "rd:TRICE64_1 double %f (%%f), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( ID( 4266), "rd:TRICE64_1 double %g (%%g), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( ID( 7210), "rd:TRICE64_1 double %E (%%E), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( ID( 4338), "rd:TRICE64_1 double %F (%%F), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( ID( 7340), "rd:TRICE64_1 double %G (%%G), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE( ID( 7065), "att:negative float & double\n" );
        break; case __LINE__: TRICE( ID( 2135), "rd:TRICE float %e (%%e)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 2888), "rd:TRICE float %f (%%f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 7652), "rd:TRICE float %g (%%g)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 5358), "rd:TRICE float %E (%%E)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 2570), "rd:TRICE float %F (%%F)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 5854), "rd:TRICE float %G (%%G)\n", aFloat(-x) );
        break; case __LINE__: TRICE64( ID( 2166), "rd:TRICE64 double %e (%%e), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64( ID( 4766), "rd:TRICE64 double %f (%%f), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64( ID( 1251), "rd:TRICE64 double %g (%%g), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64( ID( 2532), "rd:TRICE64 double %E (%%E), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64( ID( 4755), "rd:TRICE64 double %F (%%F), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64( ID( 4551), "rd:TRICE64 double %G (%%G), aDouble(y)\n", aDouble(-y) );

        break; case __LINE__: TRICE  ( ID( 6337), "att:formatted float & double\n" );
        break; case __LINE__: TRICE32( ID( 2948), "rd:TRICE32 float %12.6e (%%12.6e)\n", aFloat(x) );
        break; case __LINE__: TRICE32( ID( 6528), "rd:TRICE32 float %12.6f (%%12.6f)\n", aFloat(x) );
        break; case __LINE__: TRICE32( ID( 5671), "rd:TRICE32 float %12.6g (%%12.6g)\n", aFloat(x) );
        break; case __LINE__: TRICE32( ID( 7658), "rd:TRICE32 float %12.6E (%%12.6E)\n", aFloat(x) );
        break; case __LINE__: TRICE32( ID( 3730), "rd:TRICE32 float %12.6F (%%12.6F)\n", aFloat(x) );
        break; case __LINE__: TRICE32( ID( 4382), "rd:TRICE32 float %12.6G (%%12.6G)\n", aFloat(x) );
        break; case __LINE__: TRICE64( ID( 3259), "rd:TRICE64 double %12.6e (%%12.6e), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64( ID( 4887), "rd:TRICE64 double %12.6f (%%12.6f), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64( ID( 5970), "rd:TRICE64 double %12.6g (%%12.6g), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64( ID( 6264), "rd:TRICE64 double %12.6E (%%12.6E), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64( ID( 2043), "rd:TRICE64 double %12.6F (%%12.6F), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64( ID( 3301), "rd:TRICE64 double %12.6G (%%12.6G), aDouble(y)\n", aDouble(y) );

        break; case __LINE__: TRICE ( ID( 4832), "att:Various single arguments\n" );
        break; case __LINE__: TRICE8( ID( 6532), "rd:TRICE8 line %t (%%t ,0)\n", 0 );
        break; case __LINE__: TRICE8( ID( 4265), "rd:TRICE8 line %t (%%t ,2)\n", 2 );
        break; case __LINE__: TRICE8( ID( 3831), "rd:TRICE8 line %u (%%u ,-1)\n", -1 );
        break; case __LINE__: TRICE8( ID( 7824), "rd:TRICE8 line %b (%%b ,-1)\n", -1 );
        break; case __LINE__: TRICE8( ID( 6660), "rd:TRICE8 line %o (%%o ,-1)\n", -1 );
        break; case __LINE__: TRICE8( ID( 2315), "rd:TRICE8 line %O (%%O ,-1)\n", -1 );
        break; case __LINE__: TRICE8( ID( 4681), "rd:TRICE8 line %X (%%X ,-1)\n", -1 );
        break; case __LINE__: TRICE8( ID( 2120), "rd:TRICE8 line %x (%%x ,-1)\n", -1 );
        break; case __LINE__: TRICE8( ID( 5764), "rd:TRICE8 line %d (%%d ,-1)\n", -1 );
        break; case __LINE__: TRICE16_1( ID( 5598), "rd:TRICE16_1 line %t (%%t ,0)\n", 0 );
        break; case __LINE__: TRICE16_1( ID( 5759), "rd:TRICE16_1 line %t (%%t ,2)\n", 2 );
        break; case __LINE__: TRICE16_1( ID( 4344), "rd:TRICE16_1 line %u (%%u -1)\n", -1 );
        break; case __LINE__: TRICE16_1( ID( 4540), "rd:TRICE16_1 line %b (%%b -1)\n", -1 );
        break; case __LINE__: TRICE16_1( ID( 1410), "rd:TRICE16_1 line %o (%%o -1)\n", -1 );
        break; case __LINE__: TRICE16_1( ID( 5666), "rd:TRICE16_1 line %x (%%x -1)\n", -1 );
        break; case __LINE__: TRICE16_1( ID( 5382), "rd:TRICE16_1 line %d (%%d -1)\n", -1 );
        break; case __LINE__: TRICE( ID( 1649), "rd:TRICE line %t (%%t -1)\n", 0 );
        break; case __LINE__: TRICE( ID( 3013), "rd:TRICE line %t (%%t -1)\n", 2 );
        break; case __LINE__: TRICE( ID( 1777), "rd:TRICE line %u (%%u)\n", -1 );
        break; case __LINE__: TRICE( ID( 1909), "rd:TRICE line %b (%%b)\n", -2 );
        break; case __LINE__: TRICE( ID( 2816), "rd:TRICE line %o (%%o)\n", -3 );
        break; case __LINE__: TRICE( ID( 2875), "rd:TRICE line %x (%%x)\n", -4 );
        break; case __LINE__: TRICE( ID( 7469), "rd:TRICE line %d (%%d)\n", -5 );

        break; case __LINE__: TRICE_N( ID( 4209), "msg:%s\n", A, 0 );
        break; case __LINE__: TRICE_N( ID( 6860), "msg:%s\n", A, 1 );
        break; case __LINE__: TRICE_N( ID( 2932), "msg:%s\n", A, 2 );
        break; case __LINE__: TRICE_N( ID( 2609), "msg:%s\n", A, 3 );
        break; case __LINE__: TRICE_N( ID( 2174), "msg:%s\n", A, 4 );
        break; case __LINE__: TRICE_N( ID( 2862), "msg:%s\n", A, 5 );
        break; case __LINE__: TRICE_N( ID( 1484), "msg:%s\n", A, 6 );
        break; case __LINE__: TRICE_N( ID( 5345), "msg:%s\n", A, 7 );
        break; case __LINE__: TRICE_N( ID( 4783), "msg:%s\n", A, 8 );
        break; case __LINE__: TRICE_N( ID( 5895), "msg:%s\n", A, 9 );
        break; case __LINE__: TRICE_N( ID( 5614), "msg:%s\n", A, 10 );
        break; case __LINE__: TRICE_N( ID( 5916), "msg:%s\n", A, 11 );
        break; case __LINE__: TRICE_N( ID( 1953), "msg:%s\n", A, 12 );
        break; case __LINE__: TRICE_N( ID( 5267), "msg:%s\n", A, 13 );
        break; case __LINE__: TRICE_N( ID( 5752), "msg:%s\n", A, 14 );
        break; case __LINE__: TRICE_N( ID( 7104), "msg:%s\n", A, 15 );
        break; case __LINE__: TRICE_N( ID( 5088), "msg:%s\n", A, 16 );
        break; case __LINE__: TRICE_N( ID( 2809), "msg:%s\n", A, 17 );
        break; case __LINE__: TRICE_N( ID( 7133), "msg:%s\n", A, 18 );
        break; case __LINE__: TRICE_N( ID( 2264), "msg:%s\n", A, 19 );
        break; case __LINE__: TRICE_N( ID( 6701), "msg:%s\n", A, 120 );
        break; case __LINE__: TRICE_N( ID( 5844), "msg:%s\n", A, 121 );
        break; case __LINE__: TRICE_N( ID( 6346), "msg:%s\n", A, 122 );
        break; case __LINE__: TRICE_N( ID( 1683), "msg:%s\n", A, 123 );
        break; case __LINE__: TRICE_N( ID( 2884), "msg:%s\n", A, 124 );
        break; case __LINE__: TRICE_N( ID( 6821), "msg:%s\n", A, 125 );
        break; case __LINE__: TRICE_N( ID( 5782), "msg:%s\n", A, 126 );
        break; case __LINE__: TRICE_N( ID( 7112), "msg:%s\n", A, 127 );
        break; case __LINE__: TRICE_N( ID( 5255), "msg:%s\n", A, 128 );
        break; case __LINE__: TRICE_N( ID( 1590), "msg:%s\n", A, 129 );

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
#define SCOPY( element ) do{ char* n = #element; int size = sizeof( src->element ); memcpy( p, &(src->element), size ); p += size; TRICE_S( ID( 4891), "rd:sizeof(%8s)", n ); TRICE( ID( 4732), " = %d\n", size);}while(0);

    //! DCOPY is a helper macro for struct deserialization.
#define DCOPY( element ) do{ char* n = #element; int size = sizeof( dst->element ); memcpy( &(dst->element), p, size ); p += size; TRICE_S( ID( 2600), "rd:sizeof(%8s)", n ); TRICE( ID( 7970), " = %d\n", size);}while(0);


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
    TRICE ( Id( 5060), "inf: Tryout tx struct:" );
    TRICE8_B( Id( 7679), " %02x ", &tx, sizeof(tx) );
    TRICE( Id( 2954), "\n" );

    TRICE ( Id( 2069), "inf: Tryout buffer:" );
    TRICE8_B( Id( 4505), " %02x ", dst, len ); //lint !e670
    TRICE( Id( 1751), "\n" );

    src = dst; // "data transfer"

    len = deserializeTryout( &rx, src );
    TRICE ( Id( 1586), "inf: Tryout rx struct:" );
    TRICE8_B( Id( 3418), " %02x ", &rx, sizeof(rx) );
    TRICE( Id( 2379), "\n" );

    TRICE( Id( 6228), "inf:sizeOf(Trypout) = %d, buffer length = %d\n", sizeof(tx), len );
    TRICE8_F( Id( 1375), "info:TryoutStructFunction", &tx, sizeof(tx) );
    TRICE8_F( Id( 3490), "info:TryoutBufferFunction", dst, len ); //lint !e670
}

static void exampleOfManualJSONencoding(void){
    typedef  struct {
    int Apple, Birn;
    float Fish;
    } Ex_t;
    Ex_t Ex = { -1, 2, (float)2.781 };
    TRICE( Id(13149), "att:MyStructEvaluationFunction(json:ExA{Apple:%d, Birn:%u, Fisch:%f}\n", Ex.Apple, Ex.Birn, aFloat(Ex.Fish) );
}
