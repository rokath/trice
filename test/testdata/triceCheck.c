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
//static void exampleOfManualSerialization( void );
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

        break; case __LINE__: trice16( iD(7911), "att: line %u\n", __LINE__ );

        // normal use cases
        break; case __LINE__: TRice( iD(3814), "info:This is a message without values and a 32-bit stamp.\n" ); //exp: time: 842,150_450default: info:This is a message without values and a 32-bit stamp.
        break; case __LINE__: Trice( iD(4584), "info:This is a message without values and a 16-bit stamp.\n" ); //exp: time:       5_654default: info:This is a message without values and a 16-bit stamp.
        break; case __LINE__: trice( iD(6169), "info:This is a message without values and without stamp.\n"  ); //exp: time:            default: info:This is a message without values and without stamp.

        // special use cases
        break; case __LINE__: TRice64( iD(7310), "info:12 64-bit values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 32-bit stamp.\n", -3, -4, -5, -6, 0, aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), -8 ); //exp: time: 842,150_450default: info:12 64-bit values -3, 18446744073709551612, fffffffffffffffb, FFFFFFFFFFFFFFFA, false, -7.123457e+00, -7.123457, -7.123456789, -7.123457E+00, -7.123457, -7.123456789, 0xb1111111111111111111111111111111111111111111111111111111111111000 and a 32-bit stamp.
        break; case __LINE__: Trice64( iD(4438), "info:12 64-bit values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 16-bit stamp.\n", -3, -4, -5, -6, 0, aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), -8 ); //exp: time:       5_654default: info:12 64-bit values -3, 18446744073709551612, fffffffffffffffb, FFFFFFFFFFFFFFFA, false, -7.123457e+00, -7.123457, -7.123456789, -7.123457E+00, -7.123457, -7.123456789, 0xb1111111111111111111111111111111111111111111111111111111111111000 and a 16-bit stamp.
        break; case __LINE__: trice64( iD(4353), "info:12 64-bit values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and without stamp.\n" , -3, -4, -5, -6, 0, aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), -8 ); //exp: time:            default: info:12 64-bit values -3, 18446744073709551612, fffffffffffffffb, FFFFFFFFFFFFFFFA, false, -7.123457e+00, -7.123457, -7.123456789, -7.123457E+00, -7.123457, -7.123456789, 0xb1111111111111111111111111111111111111111111111111111111111111000 and without stamp.
        break; case __LINE__: TRice32( iD(4105), "info:12 32-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 32-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time: 842,150_450default: info:12 32-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 32-bit stamp.
        break; case __LINE__: Trice32( iD(1642), "info:12 32-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 16-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:       5_654default: info:12 32-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 16-bit stamp.
        break; case __LINE__: trice32( iD(7482), "info:12 32-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and without stamp.\n" , -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:            default: info:12 32-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and without stamp.
        break; case __LINE__: TRice16( iD(5997), "info:12 16-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 32-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time: 842,150_450default: info:12 16-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 32-bit stamp.
        break; case __LINE__: Trice16( iD(2885), "info:12 16-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 16-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:       5_654default: info:12 16-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 16-bit stamp.
        break; case __LINE__: trice16( iD(3964), "info:12 16-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and without stamp.\n" , -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:            default: info:12 16-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and without stamp.
        break; case __LINE__: TRice8 ( iD(3583), "info:12  8-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 32-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time: 842,150_450default: info:12  8-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 32-bit stamp.
        break; case __LINE__: Trice8 ( iD(5518), "info:12  8-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 16-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:       5_654default: info:12  8-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 16-bit stamp.
        break; case __LINE__: trice8 ( iD(1005), "info:12  8-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and without stamp.\n" , -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:            default: info:12  8-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and without stamp.

        break; case __LINE__: trice16( iD(4125), "att: line %u\n", __LINE__ );

        // CGO compiler issue
    //  break; case __LINE__: TRice( iD(4557), "info:12 default bit width values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 32-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //_exp: time: 842,150_450default: info:12 default bit width values -3, 4294967292, fffffffb, FFFFFFFA, truet, 1.401298e-45, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11000000111000111111001101011100 and a 32-bit stamp.
    //  break; case __LINE__: Trice( iD(6350), "info:12 default bit width values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 16-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //_exp: time:       5_654default: info:12 default bit width values -3, 4294967292, fffffffb, FFFFFFFA, truet, 1.401298e-45, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11000000111000111111001101011100 and a 16-bit stamp.
    //  break; case __LINE__: trice( iD(7692), "info:12 default bit width values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and without stamp.\n" , -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //_exp: time:            default: info:12 default bit width values -3, 4294967292, fffffffb, FFFFFFFA, truet, 1.401298e-45, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11000000111000111111001101011100 and without stamp.

        // todo: improve parsing of //exp string containing special characters
        //break; case __LINE__: TRice( iD(5114), "A TRice message containing a newline\nbefore the line end\n" );               //exp: time: 842,150_450default: A TRice message containing a newline\n                                         before the line end                                                                                                      
        //break; case __LINE__: TRice( iD(7366), "\nA TRice message starting with a a newline\n" );                             //exp: time: 842,150_450default: \n                                         A TRice message starting with a a newline                                                                                                                   
        //break; case __LINE__: TRice( iD(7808), "line0:A TRice message with several lines\nline1\nline2\nline3:last line\n" ); //exp: time: 842,150_450default: line0:A TRice message with several lines\\n                                         line1\n                                         line2\n                                         line3:last line

        break; case __LINE__: TRice16( iD(7084), "att: 0x8888 == %04xh\n", 0x8888 ); //exp: time: 842,150_450default: att: 0x8888 == 8888h
        break; case __LINE__: Trice16( iD(3586), "att: 0x8888 == %04xh\n", 0x8888 ); //exp: time:       5_654default: att: 0x8888 == 8888h
        break; case __LINE__: trice16( iD(5868), "att: 0x8888 == %04xh\n", 0x8888 ); //exp: time:            default: att: 0x8888 == 8888h


        break; case __LINE__: trice16( iD(4476), "att: line %u\n", __LINE__ );

        break; case __LINE__: trice16( iD(5970), "att: line %u\n", __LINE__ );
      //break; case __LINE__: exampleOfManualSerialization(); // ATTENTION: This occupies ~1024 bytes in one half buffer when double buffer ist used!

        break; case __LINE__: trice8   ( iD(1966), "value=%x\n", 0x55 );                           //exp: time:            default: value=55
        break; case __LINE__: trice8   ( iD(7420), "value=%x\n", 0x88 );                           //exp: time:            default: value=88

        break; case __LINE__: TRICE( ID(2092), "att:mixed int & float & double & bits\n" );
        break; case __LINE__: TRICE32( ID(2955), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",       FloatToInt32(x),   aFloat(x),   aFloat(x),   aFloat(x) ); //exp: time: 842,150_450default: rd:TRICE32 int 1089, float 1089.608276 (%f), 44883377, 01000100100010000011001101110111                                         
    //  break; case __LINE__: TRICE32( ID(2563), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",          (int32_t)(x),   aFloat(x),   aFloat(x),   aFloat(x) ); //exp: time: 842,150_450default: rd:TRICE32 int 1089, float 1089.608276 (%f), 44883377, 01000100100010000011001101110111                                         
        break; case __LINE__: TRICE32( ID(5143), "rd:TRICE32 int %x, float %f (%%f), %08x, %032b\n",            0x44883377,   aFloat(x),   aFloat(x),   aFloat(x) ); //exp: time: 842,150_450default: rd:TRICE32 int 44883377, float 1089.608276 (%f), 44883377, 01000100100010000011001101110111                                         
        break; case __LINE__: TRICE32( ID(3043), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",                     x,   aFloat(x),   aFloat(x),   aFloat(x) ); //exp: time: 842,150_450default: rd:TRICE32 int 1089, float 1089.608276 (%f), 44883377, 01000100100010000011001101110111                                         
        break; case __LINE__: TRICE32( ID(6421), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",      FloatToInt32(-x),  aFloat(-x),  aFloat(-x),  aFloat(-x) ); //exp: time: 842,150_450default: rd:TRICE32 int -1089, float -1089.608276 (%f), c4883377, 11000100100010000011001101110111                                       
        break; case __LINE__: TRICE32( ID(6821), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",         (int32_t)(-x),  aFloat(-x),  aFloat(-x),  aFloat(-x) ); //exp: time: 842,150_450default: rd:TRICE32 int -1089, float -1089.608276 (%f), c4883377, 11000100100010000011001101110111                                       
    //  break; case __LINE__: TRICE32( ID(1592), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",                    -x,  aFloat(-x),  aFloat(-x),  aFloat(-x) ); //exp: time: 842,150_450default: rd:TRICE32 int 1089, float -1089.608276 (%f), c4883377, 11000100100010000011001101110111                                        
        break; case __LINE__: TRICE32( ID(4222), "rd:TRICE32 int %X, float %f (%%f), %08x, %032b\n",           -0x44883377,  aFloat(-x),  aFloat(-x),  aFloat(-x) ); //exp: time: 842,150_450default: rd:TRICE32 int BB77CC89, float -1089.608276 (%f), c4883377, 11000100100010000011001101110111                                        
        break; case __LINE__: TRICE64( ID(3876), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",    DoubleToInt64(y),  aDouble(y),  aDouble(y),  aDouble(y) ); //exp: time: 842,150_450default: rd:TRICE64 int 518, double 518.054749 (%f), 4080307020601050, 0100000010000000001100000111000000100000011000000001000001010000   
        break; case __LINE__: TRICE64( ID(1475), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",        (int64_t)(y),  aDouble(y),  aDouble(y),  aDouble(y) ); //exp: time: 842,150_450default: rd:TRICE64 int 518, double 518.054749 (%f), 4080307020601050, 0100000010000000001100000111000000100000011000000001000001010000   
        break; case __LINE__: TRICE64( ID(1962), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",                   y,  aDouble(y),  aDouble(y),  aDouble(y) ); //exp: time: 842,150_450default: rd:TRICE64 int 518, double 518.054749 (%f), 4080307020601050, 0100000010000000001100000111000000100000011000000001000001010000   
        break; case __LINE__: TRICE64( ID(7414), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",   DoubleToInt64(-y), aDouble(-y), aDouble(-y), aDouble(-y) ); //exp: time: 842,150_450default: rd:TRICE64 int -518, double -518.054749 (%f), c080307020601050, 1100000010000000001100000111000000100000011000000001000001010000
        break; case __LINE__: TRICE64( ID(4391), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",       (int64_t)(-y), aDouble(-y), aDouble(-y), aDouble(-y) ); //exp: time: 842,150_450default: rd:TRICE64 int -518, double -518.054749 (%f), c080307020601050, 1100000010000000001100000111000000100000011000000001000001010000
    //  break; case __LINE__: TRICE64( ID(1064), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",                  -y, aDouble(-y), aDouble(-y), aDouble(-y) ); //exp: time: 842,150_450default: rd:TRICE64 int 518, double -518.054749 (%f), c080307020601050, 1100000010000000001100000111000000100000011000000001000001010000 
        break; case __LINE__: TRICE64( ID(2567), "rd:TRICE64 int %X, double %f (%%f), %016x, %064b\n", -0x4080307020601050, aDouble(-y), aDouble(-y), aDouble(-y) ); //exp: time: 842,150_450default: rd:TRICE64 int BF7FCF8FDF9FEFB0, double -518.054749 (%f), c080307020601050, 1100000010000000001100000111000000100000011000000001000001010000 

        break; case __LINE__: trice16( iD(4531), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRICE_S( ID(4166), "msg:With TRICE_S:%s\n", sABCDE );                           //exp: time: 842,150_450default: msg:With TRICE_S:abcde 12345
        break; case __LINE__: TRICE_N( ID(6010), "sig:With TRICE_N:%s\n", sABCDE, lenABCDE );                 //exp: time: 842,150_450default: sig:With TRICE_N:abcde 12345
        break; case __LINE__: TRICE8_B( ID(3203), "  %02x", sABCDE, lenABCDE );                               //exp: time: 842,150_450default:   61  62  63  64  65  20  31  32  33  34  35
        break; case __LINE__: TRICE( ID(4682), "\n" );
        break; case __LINE__: TRICE8_B( ID(7184), "%4d", sABCDE, lenABCDE );                                  //exp: time: 842,150_450default:   97  98  99 100 101  32  49  50  51  52  53
        break; case __LINE__: TRICE( ID(6095), "\n" );
        break; case __LINE__: TRICE8_B( ID(7731), "  %02x", b8, sizeof(b8)/sizeof(int8_t) );                  //exp: time: 842,150_450default:   00  ff  fe  33  04  05  06  07  08  09  0a  0b  00  ff  fe  33  04  05  06  07  08  09  0a  0b
        break; case __LINE__: TRICE( ID(5808), "\n" );
        break; case __LINE__: TRICE16_B( ID(3156), "  %04x", b16, sizeof(b16)/sizeof(int16_t) );              //exp: time: 842,150_450default:   0000  ffff  fffe  3344
        break; case __LINE__: TRICE( ID(7203), "\n" );
        break; case __LINE__: TRICE32_B( ID(1196), "  %08x", b32, sizeof(b32)/sizeof(int32_t) );              //exp: time: 842,150_450default:   00000000  ffffffff  fffffffe  33445555
        break; case __LINE__: TRICE( ID(3780), "\n" );
        break; case __LINE__: TRICE64_B( ID(7547), "  %016x", b64, sizeof(b64)/sizeof(int64_t) );             //exp: time: 842,150_450default:   0000000000000000  ffffffffffffffff  fffffffffffffffe  3344555566666666
        break; case __LINE__: TRICE( ID(4723), "\n" );
        break; case __LINE__: TRICE8_F( ID(3603), "info:FunctionNameW", b8,  sizeof(b8) /sizeof(int8_t) );   //exp: time: 842,150_450default: info:FunctionNameW(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)
        break; case __LINE__: TRICE16_F( ID(2784), "info:FunctionNameX", b16, sizeof(b16)/sizeof(int16_t) );  //exp: time: 842,150_450default: info:FunctionNameX(0000)(ffff)(fffe)(3344) 
        break; case __LINE__: TRICE32_F( ID(6470), "info:FunctionNameY", b32, sizeof(b32)/sizeof(int32_t) );  //exp: time: 842,150_450default: info:FunctionNameY(00000000)(ffffffff)(fffffffe)(33445555)
        break; case __LINE__: TRICE64_F( ID(2691), "info:FunctionNameZ", b64, sizeof(b64)/sizeof(int64_t) );  //exp: time: 842,150_450default: info:FunctionNameZ(0000000000000000)(ffffffffffffffff)(fffffffffffffffe)(3344555566666666)
        break; case __LINE__: exampleOfManualJSONencoding(); //exp: time:       5_654default: att:MyStructEvaluationFunction(json:ExA{Apple:-1, Birn:2, Fisch:2.781000}

        break; case __LINE__: trice16( iD(4651), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRICE( Id(4961), "MSG:1/11 = %g\n", aFloat( 1.0/11 ) ); //exp: time:       5_654default: MSG:1/11 = 0.09090909
        break; case __LINE__: { //exp: time: 842,150_450default: msg:x = 5.934 = 5.934, 5.934
                              float a = (float)5.934;
                              float b = a + ((a > 0) ? 0.0005f : -0.0005f);
                              int c = (int)b;
                              int d = (int)(b * 1000) % 1000;
                              int e = (int)(1000 * (float)(a - c)); 
                              TRICE( ID(6120), "msg:x = %g = %d.%03d, %d.%03d\n", aFloat(a), c, d, c, e ); 
                              }

        //break; case __LINE__: TRICE( id(5177), "dbg:Hi!\n" );                       //exp: time:            default: dbg:Hi!    
        //break; case __LINE__: TRICE( id(3133), "dbg:\tHi!\n" );                     
        //break; case __LINE__: TRICE( id(5271), "dbg:\t\tHi!\n" );                   
        //break; case __LINE__: TRICE( id(2256), "dbg:\t\t\tHi!\n" );                 
        //break; case __LINE__: TRICE( id(2071), "dbg:\\tHi!\n" );                    
        //break; case __LINE__: TRICE( Id(3122), "dbg:\aHi!\n" ); // sound!
        //break; case __LINE__: TRICE( Id(6379), "dbg:\a\aHi!\n" ); // sound!
        //break; case __LINE__: TRICE( Id(6329), "dbg:\a\a\aHi!\n" ); // sound!
        //break; case __LINE__: TRICE( Id(7238), "dbg:\\aHi!\n" );
        break; case __LINE__: TRICE( Id(4177), "err:An ERROR messgage with sound!\a\n" ); // sound!

        break; case __LINE__: TRICE_S( ID(7225), "sig:TRICE_S=%s\n", s );            //exp: time: 842,150_450default: sig:TRICE_S=AAAAAAAAAAAA
        break; case __LINE__: TRICE_N( ID(5029), "sig:TRICE_N=%s\n", s, strlen(s) ); //exp: time: 842,150_450default: sig:TRICE_N=AAAAAAAAAAAA

        break; case __LINE__: trice16( iD(3289), "att: line %u\n", __LINE__ );

//      break; case __LINE__: 
//  #ifdef XTEA_ENCRYPT_KEY
//          TRICE0 ( Id(5319), "--------------------------------------------------\n" );
//          {
//              uint32_t by[8] = {1,2,3,4,5,6};
//              int count = (sizeof(by) + 7) & ~7; // only multiple of 8 encryptable
//              TRICE32( Id(2550), "msg: message = %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
//              TRICE32( Id(7588), "tim: pre TriceEncryption SysTick=%d\n", SYSTICKVAL );
//              XTEAEncrypt(by, count>>2);
//              TRICE32( Id(7581), "tim: post TriceEncryption SysTick=%d\n", SYSTICKVAL );
//              TRICE32( Id(6746), "att: TriceEncrypted =  %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
//              #ifdef XTEA_DECRYPT
//              TRICE16_1( Id(5262), "tim: pre TriceDecryption SysTick=%d\n", SYSTICKVAL );
//              XTEADecrypt(by, count>>2);
//              TRICE16_1( Id(1599), "tim: post TriceDecryption SysTick=%d\n", SYSTICKVAL );
//              #endif
//              TRICE32( Id(7830), "msg: messge = %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
//          }
//          TRICE0 ( Id(7960), "--------------------------------------------------\n" );
//  #endif

        break; case __LINE__: trice16( iD(2082), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRICE( ID(6668), "sig:Some time measurements\n" );
        break; case __LINE__: TRICE( ID(4235), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID(7054), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID(5179), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID(7848), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
        break; case __LINE__: TRICE64( ID(2972), "rd:TRICE64 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE( ID(1513), "tim:TRICE START time message\n");
        break; case __LINE__: TRICE( ID(6873), "tim:TRICE STOP time message\n");
        break; case __LINE__: TRICE( ID(1494), "tim:TRICE START time message\n");
        break; case __LINE__: TRICE( ID(4516), "tim:TRICE STOP time message\n");
        break; case __LINE__: TRICE( ID(6909), "tim:TRICE START time message\n");
        break; case __LINE__: TRICE64( ID(6700), "rd:TRICE64 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE( ID(4045), "tim:TRICE STOP time message\n");
        break; case __LINE__: TRICE( ID(2435), "tim:TRICE --------------------------------------------------\n");
        break; case __LINE__: TRICE( ID(7529), "tim:TRICE --------------------------------------------------\n");
        break; case __LINE__: TRICE64( ID(4490), "rd:TRICE64 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE( ID(5775), "tim:TRICE --------------------------------------------------\n");
        break; case __LINE__: TRICE( ID(4024), "tim:TRICE --------------------------------------------------\n");
        break; case __LINE__: TRICE64( ID(2356), "rd:TRICE64 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE( ID(6456), "rd:TRICE32_1 line %d\n", __LINE__ );
        break; case __LINE__: TRICE64_1( ID(6006), "rd:TRICE64_1 %d\n", __LINE__ );
        break; case __LINE__: TRICE32_2( ID(1774), "rd:TRICE32_2 line %d,%d\n", __LINE__, 2 );
        break; case __LINE__: TRICE64_2( ID(6579), "rd:TRICE64_2 line %d,%d\n", __LINE__, 2 );

        break; case __LINE__: trice16( iD(7000), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRICE( ID(2844), "sig:Runtime generated strings\n" );
        break; case __LINE__: {
                                  s = "AAAAAAAAAAAA";
                                  TRICE32( ID(4688), "dbg:len=%u:", strlen(s) );
                                  TRICE_S( ID(6755), "sig:%s\n", s );
                              }
        break; case __LINE__: {
                s = "\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBCCCCCCCC\
";
                          }
        break; case __LINE__: TRICE32( ID(2728), "dbg:len=%u:", strlen(s) );
        break; case __LINE__: TRICE_S( ID(3994), "sig:%s\n", s );
        break; case __LINE__: TRICE( ID(5009), "sig:Runtime generated strings\n" );
        break; case __LINE__: {
            s = "\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\
DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\
EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\
";
                          }
        break; case __LINE__: TRICE32( ID(4741), "dbg:len=%u: ", strlen(s) );
        break; case __LINE__: TRICE_S( ID(1364), "sig:%s\n", s );
        break; case __LINE__: TRICE( ID(2178), "sig:Runtime generated strings\n" );
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
        break; case __LINE__: TRICE32( ID(5964), "dbg:len=%u: ", strlen(s) );
        break; case __LINE__: TRICE_S( ID(3315), "sig:%s\n", s );
        break; case __LINE__: TRICE( ID(1670), "sig:Runtime generated strings\n" );
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
        break; case __LINE__: TRICE32( ID(5095), "dbg:len=%u: ", strlen(s) );
        break; case __LINE__: TRICE_S( ID(4430), "sig:%s\n", s );

        break; case __LINE__: trice16( iD(1715), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRICE( ID(1530), "sig:Integer (indent, base, sign) - see https://yourbasic.org/golang/fmt-printf-reference-cheat-sheet/\n" );
        break; case __LINE__: TRICE( ID(5945), "rd: 15 	%d 	Base 10\n", 15 );                                        //ex_: time: 842,150_450default: rd: 15 	15 	Base 10                                   
        break; case __LINE__: TRICE( ID(5135), "rd: +15 	%+d 	Always show sign\n", 15 );                           //ex_: time: 842,150_450default: rd: +15 	+15 	Always show sign                          
        break; case __LINE__: TRICE( ID(1940), "rd:   15 	%4d 	Pad with spaces (width 4, right justified)\n", 15 ); //ex_: time: 842,150_450default: rd:   15	  15 	Pad with spaces (width 4, right justified)
        break; case __LINE__: TRICE( ID(3127), "rd: 15   	%-4d 	Pad with spaces (width 4, left justified)\n", 15 );  //ex_: time: 842,150_450default: rd: 15 	15   	Pad with spaces (width 4, left justified) 
        break; case __LINE__: TRICE( ID(2404), "rd: 0015 	%04d 	Pad with zeroes (width 4)\n", 15 );                  //ex_: time: 842,150_450default: rd: 0015 	0015 	Pad with zeroes (width 4)                 
        break; case __LINE__: TRICE( ID(7067), "rd: 1111 	%b 	Base 2\n", 15 );                                         //ex_: time: 842,150_450default: rd: 1111 	1111    Base 2                                    
        break; case __LINE__: TRICE( ID(1926), "rd: 17 	%o 	Base 8\n", 15 );                                         //ex_: time: 842,150_450default: rd: 17 	17      Base 8                                    
        break; case __LINE__: TRICE( ID(7243), "rd: f 	%x 	Base 16, lowercase\n", 15 );                             //ex_: time: 842,150_450default: rd: f 	f 	Base 16, lowercase                        
        break; case __LINE__: TRICE( ID(6237), "rd: F 	%X 	Base 16, uppercase\n", 15 );                             //ex_: time: 842,150_450default: rd: F 	F 	Base 16, uppercase                        
        break; case __LINE__: TRICE( ID(2112), "rd: 0xf 	%#x 	Base 16, with leading 0x\n", 15 );                   //ex_: time: 842,150_450default: rd: 0xf 	0xf 	Base 16, with leading 0x                  

        break; case __LINE__: trice16( iD(4654), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRICE( ID(3012), "sig:Character (quoted, Unicode)\n" );
        break; case __LINE__: TRICE( ID(3630), "rd: A 	%c 	Character\n", 'A' );                                     //exp: time: 842,150_450default: rd: A 	A 	Character
        break; case __LINE__: TRICE( ID(5721), "rd: 'A' 	%q 	Quoted character\n", 'A' );                              //exp: time: 842,150_450default: rd: 'A' 	'A' 	Quoted character
    //  break; case __LINE__: TRICE( ID(2503), "rd: U+0041 	%U 	Unicode\n", 'A' );                        // todo: detect %U to avoid message triceType = TRICE0 ParamSpace = 4 not matching with bitWidth  0 and paramCount 0 - ignoring package
    //  break; case __LINE__: TRICE( ID(1348), "rd: U+0041 'A' 	%#U 	Unicode with character\n", 'A' ); // todo: detect %#U to avoid message triceType = TRICE0 ParamSpace = 4 not matching with bitWidth  0 and paramCount 0 - ignoring package

        break; case __LINE__: TRICE( ID(4922), "sig:Boolean (true/false)\n" );
        break; case __LINE__: TRICE( ID(7044), "rd:Use %%t to format a boolean as true (%t) or false (%t).\n", 1, 0 ); //exp: time: 842,150_450default: rd:Use %t to format a boolean as true (true) or false (false).

        break; case __LINE__: TRICE( ID(6013), "sig:Pointer (hex)\n" );
        break; case __LINE__: TRICE8 ( ID(7968), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x84 );               //exp: time: 842,150_450default: rd:Use %p to format a pointer in base 16 notation with leading 0x. (84)              
        break; case __LINE__: TRICE16( ID(4200), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x1234 );             //exp: time: 842,150_450default: rd:Use %p to format a pointer in base 16 notation with leading 0x. (1234)            
        break; case __LINE__: TRICE32( ID(4426), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0xaabbccdd );         //exp: time: 842,150_450default: rd:Use %p to format a pointer in base 16 notation with leading 0x. (aabbccdd)        
        break; case __LINE__: TRICE64( ID(1016), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x1122334455667788 ); //exp: time: 842,150_450default: rd:Use %p to format a pointer in base 16 notation with leading 0x. (1122334455667788)
        break; case __LINE__: TRICE  ( ID(4792), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0xaabbccdd);          //exp: time: 842,150_450default: rd:Use %p to format a pointer in base 16 notation with leading 0x. (aabbccdd)        

        break; case __LINE__: trice16( iD(1029), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRICE( ID(1769), "sig:Float (indent, precision, scientific notation)\n" );
        break; case __LINE__: TRICE( ID(5180), "rd: 1.234560e+02		%e 	%%e Scientific notation\n", aFloat(123.456) );                       //exp: time: 842,150_450default: rd: 1.234560e+02		1.234560e+02 	%e Scientific notation
        break; case __LINE__: TRICE( ID(3724), "rd: 123.456000		%f 	%%f Decimal point, no exponent\n", aFloat(123.456) );                //exp: time: 842,150_450default: rd: 123.456000		123.456001 	%f Decimal point, no exponent
        break; case __LINE__: TRICE( ID(6601), "rd: 123.46			%.2f		%%.2f Default width, precision 2\n", aFloat(123.456) );      //exp: time: 842,150_450default: rd: 123.46			123.46		%.2f Default width, precision 2
        break; case __LINE__: TRICE( ID(7738), "rd: ␣␣123.46		%8.2f	%%8.2f Width 8, precision 2\n", aFloat(123.456) );                    //exp: time: 842,150_450default: rd: ␣␣123.46		  123.46	%8.2f Width 8, precision 2
        break; case __LINE__: TRICE( ID(6602), "rd: 123.456		%g		%%g Exponent as needed, necessary digits only\n", aFloat(123.456) ); //exp: time: 842,150_450default: rd: 123.456		123.456		%g Exponent as needed, necessary digits only

        break; case __LINE__: TRICE( ID(3236), "sig:Double (indent, precision, scientific notation)\n" );
        break; case __LINE__: TRICE64( ID(5459), "rd: 1.234560e+02		%e 	%%e Scientific notation\n", aDouble(123.456) );                   //exp: time: 842,150_450default: rd: 1.234560e+02		1.234560e+02 	%e Scientific notation
        break; case __LINE__: TRICE64( ID(7820), "rd: 123.456000		%f 	%%f Decimal point, no exponent\n", aDouble(123.456) );                //exp: time: 842,150_450default: rd: 123.456000		123.456000 	%f Decimal point, no exponent
        break; case __LINE__: TRICE64( ID(6685), "rd: 123.46			%.2f    	%%.2f Default width, precision 2\n", aDouble(123.456) );      //exp: time: 842,150_450default: rd: 123.46			123.46    	%.2f Default width, precision 2
        break; case __LINE__: TRICE64( ID(6201), "rd: ␣␣123.46		%8.2f 	%%8.2f Width 8, precision 2\n", aDouble(123.456) );                //exp: time: 842,150_450default: rd: ␣␣123.46		  123.46 	%8.2f Width 8, precision 2
        break; case __LINE__: TRICE64( ID(1048), "rd: 123.456		%g		%%g Exponent as needed, necessary digits only\n", aDouble(123.456) ); //exp: time: 842,150_450default: rd: 123.456		123.456		%g Exponent as needed, necessary digits only

        break; case __LINE__: TRICE( ID(6581), "sig:String or byte slice (quote, indent, hex)\n" );
        break; case __LINE__: s = "café"; TRICE_S( ID(2292), "rd: café 			%s		Plain string\n", s );           //exp: time: 842,150_450default: rd: café 			café		Plain string
        break; case __LINE__: s = "café"; TRICE_S( ID(5411), "rd: ␣␣café 		%6s 		Width 6, right justify\n", s );  //exp: time: 842,150_450default: rd: ␣␣café 		  café 		Width 6, right justify
        break; case __LINE__: s = "café"; TRICE_S( ID(2023), "rd: café␣␣ 		%-6s 		Width 6, left justify\n", s );   //exp: time: 842,150_450default: rd: café␣␣ 		café   		Width 6, left justify
        break; case __LINE__: s = "café"; TRICE_S( ID(6636), "rd: \"café\" 		%q		Quoted string\n", s );          //exp: time: 842,150_450default: rd: \"café\" 		"café"		Quoted string
        break; case __LINE__: s = "café"; TRICE_S( ID(1825), "rd: 636166c3a9 		%x	Hex dump of byte values\n", s );    //exp: time: 842,150_450default: rd: 636166c3a9 		636166c3a9	Hex dump of byte values 
        break; case __LINE__: s = "café"; TRICE_S( ID(7071), "rd: 63 61 66 c3 a9 	% x	Hex dump with spaces\n", s );       //exp: time: 842,150_450default: rd: 63 61 66 c3 a9 	63 61 66 c3 a9	Hex dump with spaces

        break; case __LINE__: trice16( iD(7052), "att: line %u\n", __LINE__ );

#ifdef TRICE_CGO

        break; case __LINE__: TRICE  ( ID(7314), "rd:%E (%%E)\n",  aFloat(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-5.555556E+08 (%E)
        break; case __LINE__: TRICE  ( ID(5124), "rd:%F (%%F)\n",  aFloat(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-555555584.000000 (%F)
        break; case __LINE__: TRICE  ( ID(1061), "rd:%G (%%G)\n",  aFloat(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-5.555556E+08 (%G)
        break; case __LINE__: TRICE64( ID(1346), "rd:%E (%%E)\n", aDouble(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-5.555556E+08 (%E)
        break; case __LINE__: TRICE64( ID(3679), "rd:%F (%%F)\n", aDouble(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-555555555.555556 (%F)
        break; case __LINE__: TRICE64( ID(5653), "rd:%G (%%G)\n", aDouble(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-5.555555555555556E+08 (%G)
        
        break; case __LINE__: TRICE  ( ID(5612), "rd:%e (%%e)\n",  aFloat(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-5.555556e+08 (%e)
        break; case __LINE__: TRICE  ( ID(2258), "rd:%f (%%f)\n",  aFloat(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-555555584.000000 (%f)
        break; case __LINE__: TRICE  ( ID(2354), "rd:%g (%%g)\n",  aFloat(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-5.555556e+08 (%g)
        break; case __LINE__: TRICE64( ID(4988), "rd:%e (%%e)\n", aDouble(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-5.555556e+08 (%e)
        break; case __LINE__: TRICE64( ID(3588), "rd:%f (%%f)\n", aDouble(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-555555555.555556 (%f)
        break; case __LINE__: TRICE64( ID(3238), "rd:%g (%%g)\n", aDouble(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-5.555555555555556e+08 (%g)

        break; case __LINE__: TRICE32( ID(5507), "msg:%u (%%u)\n", -1 ); //exp: time: 842,150_450default: msg:4294967295 (%u)
        break; case __LINE__: TRICE32( ID(4132), "msg:%b (%%b)\n", -1 ); //exp: time: 842,150_450default: msg:11111111111111111111111111111111 (%b)
        break; case __LINE__: TRICE32( ID(6585), "msg:%o (%%o)\n", -1 ); //exp: time: 842,150_450default: msg:37777777777 (%o)
        break; case __LINE__: TRICE32( ID(7305), "msg:%O (%%O)\n", -1 ); //exp: time: 842,150_450default: msg:0o37777777777 (%O)
        break; case __LINE__: TRICE32( ID(2916), "msg:%X (%%X)\n", -1 ); //exp: time: 842,150_450default: msg:FFFFFFFF (%X)
        break; case __LINE__: TRICE32( ID(2044), "msg:%x (%%x)\n", -1 ); //exp: time: 842,150_450default: msg:ffffffff (%x)
        break; case __LINE__: TRICE32( ID(1258), "msg:%d (%%d)\n", -1 ); //exp: time: 842,150_450default: msg:-1 (%d)
        break; case __LINE__: TRICE64( ID(1360), "msg:%u (%%u)\n", -1 ); //exp: time: 842,150_450default: msg:18446744073709551615 (%u)
        break; case __LINE__: TRICE64( ID(5875), "msg:%b (%%b)\n", -1 ); //exp: time: 842,150_450default: msg:1111111111111111111111111111111111111111111111111111111111111111 (%b)
        break; case __LINE__: TRICE64( ID(6935), "msg:%o (%%o)\n", -1 ); //exp: time: 842,150_450default: msg:1777777777777777777777 (%o)
        break; case __LINE__: TRICE64( ID(6752), "msg:%O (%%O)\n", -1 ); //exp: time: 842,150_450default: msg:0o1777777777777777777777 (%O)
        break; case __LINE__: TRICE64( ID(1157), "msg:%X (%%X)\n", -1 ); //exp: time: 842,150_450default: msg:FFFFFFFFFFFFFFFF (%X)
        break; case __LINE__: TRICE64( ID(2026), "msg:%x (%%x)\n", -1 ); //exp: time: 842,150_450default: msg:ffffffffffffffff (%x)

        //break; case __LINE__: TRice        ( iD(1981), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        //break; case __LINE__: Trice        ( iD(4264), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        //break; case __LINE__: trice        ( iD(1317), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: TRice8       ( iD(6132), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: Trice8       ( iD(2641), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: trice8       ( iD(7376), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: TRice16      ( iD(1018), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: Trice16      ( iD(2963), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: trice16      ( iD(6428), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: TRice32      ( iD(5336), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: Trice32      ( iD(5274), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: trice32      ( iD(6918), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: TRice64      ( iD(1034), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: Trice64      ( iD(7081), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: trice64      ( iD(2426), "value=%d\n", -2  ); //exp: time:            default: value=-2
        
        //break; case __LINE__: TRice_1      ( iD(6999), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        //break; case __LINE__: Trice_1      ( iD(1240), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        //break; case __LINE__: trice_1      ( iD(6415), "value=%d\n", -2  ); //exp: time:            default: value=-2
        //break; case __LINE__: TRice8_1     ( iD(1875), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        //break; case __LINE__: Trice8_1     ( iD(3999), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        //break; case __LINE__: trice8_1     ( iD(4422), "value=%d\n", -2  ); //exp: time:            default: value=-2
        //break; case __LINE__: TRice16_1    ( iD(3227), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        //break; case __LINE__: Trice16_1    ( iD(5850), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        //break; case __LINE__: trice16_1    ( iD(3966), "value=%d\n", -2  ); //exp: time:            default: value=-2
        //break; case __LINE__: TRice32_1    ( iD(3981), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        //break; case __LINE__: Trice32_1    ( iD(7566), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        //break; case __LINE__: trice32_1    ( iD(6784), "value=%d\n", -2  ); //exp: time:            default: value=-2
        //break; case __LINE__: TRice64_1    ( iD(6549), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        //break; case __LINE__: Trice64_1    ( iD(1690), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        //break; case __LINE__: trice64_1    ( iD(1447), "value=%d\n", -2  ); //exp: time:            default: value=-2
        
        //break; case __LINE__: TRice        ( iD(7199), "no value" ); //exp: time: 842,150_450default: no value
        //break; case __LINE__: Trice        ( iD(2453), "no value" ); //exp: time:       5_654default: no value
        //break; case __LINE__: trice        ( iD(6462), "no value" ); //exp: time:            default: no value
        //break; case __LINE__: TRice8       ( iD(7795), "no value" ); //exp: time: 842,150_450default: no value
        //break; case __LINE__: Trice8       ( iD(1976), "no value" ); //exp: time:       5_654default: no value
        //break; case __LINE__: trice8       ( iD(7272), "no value" ); //exp: time:            default: no value
        //break; case __LINE__: TRice16      ( iD(7999), "no value" ); //exp: time: 842,150_450default: no value
        //break; case __LINE__: Trice16      ( iD(7361), "no value" ); //exp: time:       5_654default: no value
        //break; case __LINE__: trice16      ( iD(2014), "no value" ); //exp: time:            default: no value
        //break; case __LINE__: TRice32      ( iD(4378), "no value" ); //exp: time: 842,150_450default: no value
        //break; case __LINE__: Trice32      ( iD(2201), "no value" ); //exp: time:       5_654default: no value
        //break; case __LINE__: trice32      ( iD(1216), "no value" ); //exp: time:            default: no value
        //break; case __LINE__: TRice64      ( iD(3387), "no value" ); //exp: time: 842,150_450default: no value
        //break; case __LINE__: Trice64      ( iD(5221), "no value" ); //exp: time:       5_654default: no value
        //break; case __LINE__: trice64      ( iD(2998), "no value" ); //exp: time:            default: no value
        
        //break; case __LINE__: TRice_0      ( iD(2525), "no value" ); //exp: time: 842,150_450default: no value
        //break; case __LINE__: Trice_0      ( iD(3750), "no value" ); //exp: time:       5_654default: no value
        //break; case __LINE__: trice_0      ( iD(5711), "no value" ); //exp: time:            default: no value
        //break; case __LINE__: TRice8_0     ( iD(4612), "no value" ); //exp: time: 842,150_450default: no value
        //break; case __LINE__: Trice8_0     ( iD(5594), "no value" ); //exp: time:       5_654default: no value
        //break; case __LINE__: trice8_0     ( iD(5200), "no value" ); //exp: time:            default: no value
        //break; case __LINE__: TRice16_0    ( iD(2230), "no value" ); //exp: time: 842,150_450default: no value
        //break; case __LINE__: Trice16_0    ( iD(6897), "no value" ); //exp: time:       5_654default: no value
        //break; case __LINE__: trice16_0    ( iD(5601), "no value" ); //exp: time:            default: no value
        //break; case __LINE__: TRice32_0    ( iD(6723), "no value" ); //exp: time: 842,150_450default: no value
        //break; case __LINE__: Trice32_0    ( iD(1972), "no value" ); //exp: time:       5_654default: no value
        //break; case __LINE__: trice32_0    ( iD(2766), "no value" ); //exp: time:            default: no value
        //break; case __LINE__: TRice64_0    ( iD(5870), "no value" ); //exp: time: 842,150_450default: no value
        //break; case __LINE__: Trice64_0    ( iD(2976), "no value" ); //exp: time:       5_654default: no value
        //break; case __LINE__: trice64_0    ( iD(5365), "no value" ); //exp: time:            default: no value

    //  break; case __LINE__: TRice( iD(2257), "info:This is a message with one value %d and a 32-bit stamp.\n", -2  ); //exp: time: 842,150_450default: info:This is a message with one value -2 and a 32-bit stamp.
    //  break; case __LINE__: Trice( iD(1853), "info:This is a message with one value %d and a 16-bit stamp.\n", -2  ); //exp: time:       5_654default: info:This is a message with one value -2 and a 16-bit stamp.
    //  break; case __LINE__: trice( iD(6464), "info:This is a message with one value %d and without stamp.\n" , -2  ); //exp: time:            default: info:This is a message with one value -2 and without stamp.

        break; case __LINE__: TRICE( ID(7577), "info:This is a message without values and a 32-bit stamp.\n" );        //exp: time: 842,150_450default: info:This is a message without values and a 32-bit stamp.
        break; case __LINE__: TRICE( Id(1147), "info:This is a message without values and a 16-bit stamp.\n" );        //exp: time:       5_654default: info:This is a message without values and a 16-bit stamp.
        break; case __LINE__: TRICE( id(4021), "info:This is a message without values and without stamp.\n"  );        //exp: time:            default: info:This is a message without values and without stamp.

        break; case __LINE__: TRICE( ID(3007), "info:This is a message with one value %d and a 32-bit stamp.\n", -2  ); //exp: time: 842,150_450default: info:This is a message with one value -2 and a 32-bit stamp.
        break; case __LINE__: TRICE( Id(7072), "info:This is a message with one value %d and a 16-bit stamp.\n", -2  ); //exp: time:       5_654default: info:This is a message with one value -2 and a 16-bit stamp.
        break; case __LINE__: TRICE( id(5861), "info:This is a message with one value %d and without stamp.\n" , -2  ); //exp: time:            default: info:This is a message with one value -2 and without stamp.

        break; case __LINE__: TRICE( ID(7009), "info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 32-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 );   //exp: time: 842,150_450default: info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and a 32-bit stamp.
        break; case __LINE__: TRICE( Id(4293), "info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 16-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 );   //exp: time:       5_654default: info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and a 16-bit stamp.
        break; case __LINE__: TRICE( id(2606), "info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and without stamp.\n" , -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 );   //exp: time:            default: info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and without stamp.

        break; case __LINE__: TRICE32( ID(6614), "info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 32-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //exp: time: 842,150_450default: info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and a 32-bit stamp.
        break; case __LINE__: TRICE32( Id(7969), "info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 16-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //exp: time:       5_654default: info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and a 16-bit stamp.
        break; case __LINE__: TRICE32( id(4694), "info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and without stamp.\n" , -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //exp: time:            default: info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and without stamp.

        break; case __LINE__: TRICE8   ( id(3629), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: TRICE8_1 ( id(4512), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: TRICE8   ( Id(1617), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRICE8_1 ( Id(2148), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRICE8   ( ID(7224), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: TRICE8_1 ( ID(6074), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: trice8   ( iD(2325), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: trice8_1 ( iD(1726), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: Trice8   ( iD(4929), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: Trice8_1 ( iD(4871), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRice8   ( iD(7962), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: TRice8_1 ( iD(5604), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200

        break; case __LINE__: TRICE16  ( id(4186), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: TRICE16_1( id(6670), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: TRICE16  ( Id(6678), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRICE16_1( Id(3154), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRICE16  ( ID(3132), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: TRICE16_1( ID(7634), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: trice16  ( iD(4129), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: trice16_1( iD(7313), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: Trice16  ( iD(1091), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: Trice16_1( iD(5823), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRice16  ( iD(3849), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: TRice16_1( iD(5035), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200

        break; case __LINE__: TRICE32  ( id(3426), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: TRICE32_1( id(6819), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: TRICE32  ( Id(7416), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRICE32_1( Id(2693), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRICE32  ( ID(6226), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: TRICE32_1( ID(1004), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: trice32  ( iD(1570), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: trice32_1( iD(4803), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: Trice32  ( iD(4368), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: Trice32_1( iD(5637), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRice32  ( iD(6113), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: TRice32_1( iD(7973), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200

        break; case __LINE__: TRICE64  ( id(1289), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: TRICE64_1( id(6082), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: TRICE64  ( Id(1776), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRICE64_1( Id(1178), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRICE64  ( ID(7356), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: TRICE64_1( ID(6093), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: trice64  ( iD(4428), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: trice64_1( iD(5150), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: Trice64  ( iD(5966), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: Trice64_1( iD(5219), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRice64  ( iD(4657), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: TRice64_1( iD(6486), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200

        // mixed 16-bit and 32-bit timestamp 
        break; case __LINE__: TRice8( iD(2469), "msg:value=%d\n", -1 );                                                                                            //exp: time: 842,150_450default: msg:value=-1
        break; case __LINE__: Trice8( iD(2067), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: msg:value=-1, -2
        break; case __LINE__: trice8( iD(6766), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: msg:value=-1, -2, -3
        break; case __LINE__: TRice8( iD(7648), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice8( iD(1698), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice8( iD(7716), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice8( iD(1524), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice8( iD(1487), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice8( iD(4414), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice8( iD(6277), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice8( iD(4223), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice8( iD(7099), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice16( iD(7601), "msg:value=%d\n", -1 );                                                                                            //exp: time: 842,150_450default: msg:value=-1
        break; case __LINE__: Trice16( iD(6919), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: msg:value=-1, -2
        break; case __LINE__: trice16( iD(1033), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: msg:value=-1, -2, -3
        break; case __LINE__: TRice16( iD(7462), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice16( iD(7478), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice16( iD(1153), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice16( iD(5842), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice16( iD(6776), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice16( iD(6157), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice16( iD(2120), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice16( iD(1874), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice16( iD(3608), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice32( iD(4313), "msg:value=%d\n", -1 );                                                                                            //exp: time: 842,150_450default: msg:value=-1
        break; case __LINE__: Trice32( iD(5345), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: msg:value=-1, -2
        break; case __LINE__: trice32( iD(7811), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: msg:value=-1, -2, -3
        break; case __LINE__: TRice32( iD(5619), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice32( iD(6749), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice32( iD(4921), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice32( iD(2098), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice32( iD(3755), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice32( iD(1822), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice32( iD(4529), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice32( iD(2037), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice32( iD(7042), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice64( iD(1673), "msg:value=%d\n", -1 );                                                                                            //exp: time: 842,150_450default: msg:value=-1
        break; case __LINE__: Trice64( iD(3065), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: msg:value=-1, -2
        break; case __LINE__: trice64( iD(2485), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: msg:value=-1, -2, -3
        break; case __LINE__: TRice64( iD(3783), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice64( iD(7701), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice64( iD(6633), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice64( iD(1460), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice64( iD(2347), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice64( iD(7672), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice64( iD(3864), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice64( iD(2316), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice64( iD(4044), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice8_1 ( iD(4395), "msg:value=%d\n", -1 );                                                                                            //exp: time: 842,150_450default: msg:value=-1
        break; case __LINE__: Trice8_2 ( iD(6423), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: msg:value=-1, -2
        break; case __LINE__: trice8_3 ( iD(1950), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: msg:value=-1, -2, -3
        break; case __LINE__: TRice8_4 ( iD(4609), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice8_5 ( iD(2889), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice8_6 ( iD(6794), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice8_7 ( iD(2745), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice8_8 ( iD(2675), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice8_9 ( iD(5493), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice8_10( iD(7494), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice8_11( iD(5773), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice8_12( iD(2968), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice16_1 ( iD(2872), "msg:value=%d\n", -1 );                                                                                            //exp: time: 842,150_450default: msg:value=-1
        break; case __LINE__: Trice16_2 ( iD(4510), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: msg:value=-1, -2
        break; case __LINE__: trice16_3 ( iD(7306), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: msg:value=-1, -2, -3
        break; case __LINE__: TRice16_4 ( iD(7853), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice16_5 ( iD(3328), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice16_6 ( iD(1548), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice16_7 ( iD(1586), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice16_8 ( iD(1753), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice16_9 ( iD(1819), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice16_10( iD(4149), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice16_11( iD(1488), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice16_12( iD(5282), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice32_1 ( iD(2561), "msg:value=%d\n", -1 );                                                                                            //exp: time: 842,150_450default: msg:value=-1
        break; case __LINE__: Trice32_2 ( iD(1366), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: msg:value=-1, -2
        break; case __LINE__: trice32_3 ( iD(3389), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: msg:value=-1, -2, -3
        break; case __LINE__: TRice32_4 ( iD(2487), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice32_5 ( iD(3493), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice32_6 ( iD(7809), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice32_7 ( iD(4780), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice32_8 ( iD(7133), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice32_9 ( iD(3934), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice32_10( iD(1684), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice32_11( iD(1944), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice32_12( iD(2653), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice64_1 ( iD(7793), "msg:value=%d\n", -1 );                                                                                            //exp: time: 842,150_450default: msg:value=-1
        break; case __LINE__: Trice64_2 ( iD(6934), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: msg:value=-1, -2
        break; case __LINE__: trice64_3 ( iD(1957), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: msg:value=-1, -2, -3
        break; case __LINE__: TRice64_4 ( iD(3098), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice64_5 ( iD(1967), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice64_6 ( iD(1297), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice64_7 ( iD(6869), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice64_8 ( iD(4542), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice64_9 ( iD(4939), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice64_10( iD(1296), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice64_11( iD(1085), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice64_12( iD(7200), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        // use cases for 8-bit data
        break; case __LINE__: trice8     ( iD(5460), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: Trice8     ( iD(1490), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRice8     ( iD(5292), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: trice8_1   ( iD(1003), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: Trice8_1   ( iD(5405), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRice8_1   ( iD(3182), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2

        // use cases for 16-bit data
        break; case __LINE__: trice16    ( iD(6655), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: Trice16    ( iD(3841), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRice16    ( iD(2540), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: trice16_1  ( iD(5210), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: Trice16_1  ( iD(2385), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRice16_1  ( iD(4067), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2

        // use cases for 32-bit data
        break; case __LINE__: trice32    ( iD(5138), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: Trice32    ( iD(2822), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRice32    ( iD(2039), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: trice32_1  ( iD(5933), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: Trice32_1  ( iD(2372), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRice32_1  ( iD(7697), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2

        // use cases for 64-bit data
        break; case __LINE__: trice64    ( iD(7266), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: Trice64    ( iD(5771), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRice64    ( iD(5166), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: trice64_1  ( iD(6479), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: Trice64_1  ( iD(6953), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRice64_1  ( iD(3207), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2

        // legacy use cases
        break; case __LINE__: TRICE      ( ID(5833), "value=%d\n", -2 ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: TRICE      ( id(3757), "value=%d\n", -2 ); //exp: time:            default: value=-2
        break; case __LINE__: TRICE      ( Id(1533), "value=%d\n", -2 ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRICE      ( ID(7101), "value=%d\n", -2 ); //exp: time: 842,150_450default: value=-2

        // default stamp
        break; case __LINE__: TRICE8     ( ID(7168), "value=%d\n", -2 ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: TRICE8_1   ( ID(3431), "value=%d\n", -2 ); //exp: time: 842,150_450default: value=-2

        // no stamp
        break; case __LINE__: trice8     ( iD(4649), "value=%d\n", -2 ); //exp: time:            default: value=-2
        break; case __LINE__: trice8_1   ( iD(6084), "value=%d\n", -2 ); //exp: time:            default: value=-2
        break; case __LINE__: TRICE8     ( id(7487), "value=%d\n", -2 ); //exp: time:            default: value=-2
        break; case __LINE__: TRICE8_1   ( id(1314), "value=%d\n", -2 ); //exp: time:            default: value=-2

        // 16-bit stamp:
        break; case __LINE__: Trice8     ( iD(1743), "value=%d\n", -2 ); //exp: time:       5_654default: value=-2
        break; case __LINE__: Trice8_1   ( iD(3804), "value=%d\n", -2 ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRICE8     ( Id(7726), "value=%d\n", -2 ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRICE8_1   ( Id(6534), "value=%d\n", -2 ); //exp: time:       5_654default: value=-2

        // 32-bit stamp:
        break; case __LINE__: TRice8     ( iD(4274), "value=%d\n", -2 ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: TRice8_1   ( iD(2365), "value=%d\n", -2 ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: TRICE8     ( ID(4060), "value=%d\n", -2 ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: TRICE8_1   ( ID(1898), "value=%d\n", -2 ); //exp: time: 842,150_450default: value=-2

        break; case __LINE__: TRICE( ID(2937), "FATAL:magenta+b:red\n" );           //exp: time: 842,150_450default: FATAL:magenta+b:red
        break; case __LINE__: TRICE( ID(4417), "CRITICAL:red+i:default+h\n" );      //exp: time: 842,150_450default: CRITICAL:red+i:default+h
        break; case __LINE__: TRICE( ID(5096), "EMERGENCY:red+i:blue\n" );          //exp: time: 842,150_450default: EMERGENCY:red+i:blue
        break; case __LINE__: TRICE( ID(2167), "ERROR:11:red\n" );                  //exp: time: 842,150_450default: ERROR:11:red
        break; case __LINE__: TRICE( ID(7991), "WARNING:11+i:red\n" );              //exp: time: 842,150_450default: WARNING:11+i:red
        break; case __LINE__: TRICE( ID(1539), "ATTENTION:11:green\n" );            //exp: time: 842,150_450default: ATTENTION:11:green
        break; case __LINE__: TRICE( ID(2598), "INFO:cyan+b:default+h\n" );         //exp: time: 842,150_450default: INFO:cyan+b:default+h
        break; case __LINE__: TRICE( ID(2724), "DEBUG:130+i\n" );                   //exp: time: 842,150_450default: DEBUG:130+i
        break; case __LINE__: TRICE( ID(6066), "TRACE:default+i:default+h\n" );     //exp: time: 842,150_450default: TRACE:default+i:default+h
        break; case __LINE__: TRICE( ID(3327), "TIME:blue+i:blue+h\n" );            //exp: time: 842,150_450default: TIME:blue+i:blue+h
        break; case __LINE__: TRICE( ID(3657), "MESSAGE:green+h:black\n" );         //exp: time: 842,150_450default: MESSAGE:green+h:black
        break; case __LINE__: TRICE( ID(1850), "READ:black+i:yellow+h\n" );         //exp: time: 842,150_450default: READ:black+i:yellow+h
        break; case __LINE__: TRICE( ID(5042), "WRITE:black+u:yellow+h\n" );        //exp: time: 842,150_450default: WRITE:black+u:yellow+h
        break; case __LINE__: TRICE( ID(1880), "RECEIVE:black+h:black\n" );         //exp: time: 842,150_450default: RECEIVE:black+h:black
        break; case __LINE__: TRICE( ID(7385), "TRANSMIT:black:black+h\n" );        //exp: time: 842,150_450default: TRANSMIT:black:black+h
        break; case __LINE__: TRICE( ID(3551), "DIAG:yellow+i:default+h\n" );       //exp: time: 842,150_450default: DIAG:yellow+i:default+h
        break; case __LINE__: TRICE( ID(6826), "INTERRUPT:magenta+i:default+h\n" ); //exp: time: 842,150_450default: INTERRUPT:magenta+i:default+h
        break; case __LINE__: TRICE( ID(3528), "SIGNAL:118+i\n" );                  //exp: time: 842,150_450default: SIGNAL:118+i
        break; case __LINE__: TRICE( ID(5769), "TEST:yellow+h:black\n" );           //exp: time: 842,150_450default: TEST:yellow+h:black
        break; case __LINE__: TRICE( ID(3739), "DEFAULT:off\n" );                   //exp: time: 842,150_450default: DEFAULT:off
        break; case __LINE__: TRICE( ID(6031), "NOTICE:blue:white+h\n" );           //exp: time: 842,150_450default: NOTICE:blue:white+h
        break; case __LINE__: TRICE( ID(5383), "ALERT:magenta:magenta+h\n" );       //exp: time: 842,150_450default: ALERT:magenta:magenta+h
        break; case __LINE__: TRICE( ID(6800), "ASSERT:yellow+i:blue\n" );          //exp: time: 842,150_450default: ASSERT:yellow+i:blue
        break; case __LINE__: TRICE( ID(6662), "ALARM:red+i:white+h\n" );           //exp: time: 842,150_450default: ALARM:red+i:white+h
        break; case __LINE__: TRICE( ID(2534), "CYCLE:blue+i:default+h\n" );        //exp: time: 842,150_450default: CYCLE:blue+i:default+h
        break; case __LINE__: TRICE( ID(7217), "VERBOSE:blue:default\n" );          //exp: time: 842,150_450default: VERBOSE:blue:default
        break; case __LINE__: TRICE( ID(4869), "fatal:magenta+b:red\n" );           //exp: time: 842,150_450default: fatal:magenta+b:red
        break; case __LINE__: TRICE( ID(6105), "critical:red+i:default+h\n" );      //exp: time: 842,150_450default: critical:red+i:default+h
        break; case __LINE__: TRICE( ID(6687), "emergency:red+i:blue\n" );          //exp: time: 842,150_450default: emergency:red+i:blue
        break; case __LINE__: TRICE( ID(6261), "error:11:red\n" );                  //exp: time: 842,150_450default: error:11:red
        break; case __LINE__: TRICE( ID(1896), "warning:11+i:red\n" );              //exp: time: 842,150_450default: warning:11+i:red
        break; case __LINE__: TRICE( ID(3351), "attention:11:green\n" );            //exp: time: 842,150_450default: attention:11:green
        break; case __LINE__: TRICE( ID(1002), "info:cyan+b:default+h\n" );         //exp: time: 842,150_450default: info:cyan+b:default+h
        break; case __LINE__: TRICE( ID(4082), "debug:130+i\n" );                   //exp: time: 842,150_450default: debug:130+i
        break; case __LINE__: TRICE( ID(5225), "trace:default+i:default+h\n" );     //exp: time: 842,150_450default: trace:default+i:default+h
        break; case __LINE__: TRICE( ID(6166), "time:blue+i:blue+h\n" );            //exp: time: 842,150_450default: time:blue+i:blue+h
        break; case __LINE__: TRICE( ID(5736), "message:green+h:black\n" );         //exp: time: 842,150_450default: message:green+h:black
        break; case __LINE__: TRICE( ID(6408), "read:black+i:yellow+h\n" );         //exp: time: 842,150_450default: read:black+i:yellow+h
        break; case __LINE__: TRICE( ID(3582), "write:black+u:yellow+h\n" );        //exp: time: 842,150_450default: write:black+u:yellow+h
        break; case __LINE__: TRICE( ID(4360), "receive:black+h:black\n" );         //exp: time: 842,150_450default: receive:black+h:black
        break; case __LINE__: TRICE( ID(5410), "transmit:black:black+h\n" );        //exp: time: 842,150_450default: transmit:black:black+h
        break; case __LINE__: TRICE( ID(2271), "diag:yellow+i:default+h\n" );       //exp: time: 842,150_450default: diag:yellow+i:default+h
        break; case __LINE__: TRICE( ID(4321), "interrupt:magenta+i:default+h\n" ); //exp: time: 842,150_450default: interrupt:magenta+i:default+h
        break; case __LINE__: TRICE( ID(7318), "signal:118+i\n" );                  //exp: time: 842,150_450default: signal:118+i
        break; case __LINE__: TRICE( ID(5951), "test:yellow+h:black\n" );           //exp: time: 842,150_450default: test:yellow+h:black
        break; case __LINE__: TRICE( ID(6069), "default:off\n" );                   //exp: time: 842,150_450default: default:off
        break; case __LINE__: TRICE( ID(5142), "notice:blue:white+h\n" );           //exp: time: 842,150_450default: notice:blue:white+h
        break; case __LINE__: TRICE( ID(6933), "alert:magenta:magenta+h\n" );       //exp: time: 842,150_450default: alert:magenta:magenta+h
        break; case __LINE__: TRICE( ID(2394), "assert:yellow+i:blue\n" );          //exp: time: 842,150_450default: assert:yellow+i:blue
        break; case __LINE__: TRICE( ID(7332), "alarm:red+i:white+h\n" );           //exp: time: 842,150_450default: alarm:red+i:white+h
        break; case __LINE__: TRICE( ID(1664), "cycle:blue+i:default+h\n" );        //exp: time: 842,150_450default: cycle:blue+i:default+h
        break; case __LINE__: TRICE( ID(6756), "verbose:blue:default\n" );          //exp: time: 842,150_450default: verbose:blue:default
        break; case __LINE__: TRICE64( id(2555), "msg:%d (%%d)\n", -1 );     //exp: time:            default: msg:-1 (%d)

        break; case __LINE__: TRICE( ID(5738), "sig:TRICE with 1 to 12 values\n" );
        break; case __LINE__: TRICE( ID(5239), "rd:TRICE %d\n", -1 );
        break; case __LINE__: TRICE( ID(2570), "rd:TRICE %d, %d\n", -1, -2 );
        break; case __LINE__: TRICE( ID(6403), "rd:TRICE %d, %d, %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE( ID(3828), "rd:TRICE %d, %d, %d, %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE( ID(6747), "rd:TRICE %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE( ID(6220), "rd:TRICE %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE( ID(5495), "rd:TRICE %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE( ID(6466), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE( ID(4667), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE( ID(4497), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE( ID(3285), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE( ID(5754), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE( ID(7815), "sig:TRICE_n with 1 to 12 values\n" );
        break; case __LINE__: TRICE_1( ID(6280), "rd:TRICE_1 %d\n", 1 );
        break; case __LINE__: TRICE_2( ID(1173), "rd:TRICE_2 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE_3( ID(4993), "rd:TRICE_3 %d, %d, %d\n", 1, 2, 3 );
        break; case __LINE__: TRICE_4( ID(6262), "rd:TRICE_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
        break; case __LINE__: TRICE_5( ID(1994), "rd:TRICE_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
        break; case __LINE__: TRICE_6( ID(1148), "rd:TRICE_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
        break; case __LINE__: TRICE_7( ID(7718), "rd:TRICE_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
        break; case __LINE__: TRICE_8( ID(5265), "rd:TRICE_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
        break; case __LINE__: TRICE_9( ID(4210), "rd:TRICE_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
        break; case __LINE__: TRICE_10( ID(2879), "rd:TRICE_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
        break; case __LINE__: TRICE_11( ID(3692), "rd:TRICE_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
        break; case __LINE__: TRICE_12( ID(6446), "rd:TRICE_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );				

        break; case __LINE__: TRICE( ID(6763), "sig:TRICE8 with 1 to 12 values\n" );
        break; case __LINE__: TRICE8( ID(6213), "rd:TRICE8 %d\n", -1 );
        break; case __LINE__: TRICE8( ID(7821), "rd:TRICE8 %d, %d\n", -1, -2 );
        break; case __LINE__: TRICE8( ID(5897), "rd:TRICE8 %d, %d, %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE8( ID(2038), "rd:TRICE8 %d, %d, %d, %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE8( ID(6187), "rd:TRICE8 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE8( ID(7642), "rd:TRICE8 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE8( ID(5862), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE8( ID(5118), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE8( ID(2819), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE8( ID(4686), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE8( ID(2226), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE8( ID(6621), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE( ID(6039), "sig:TRICE8_n with 1 to 12 values\n" );
        break; case __LINE__: TRICE8_1 ( ID(2457), "rd:TRICE8_1 %d\n", 1 );
        break; case __LINE__: TRICE8_2 ( ID(2305), "rd:TRICE8_2 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE8_3 ( ID(5420), "rd:TRICE8_3 %d, %d, %d\n", 1, 2, 3 );
        break; case __LINE__: TRICE8_4 ( ID(2419), "rd:TRICE8_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
        break; case __LINE__: TRICE8_5 ( ID(4157), "rd:TRICE8_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
        break; case __LINE__: TRICE8_6 ( ID(6233), "rd:TRICE8_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
        break; case __LINE__: TRICE8_7 ( ID(2933), "rd:TRICE8_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
        break; case __LINE__: TRICE8_8 ( ID(3034), "rd:TRICE8_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
        break; case __LINE__: TRICE8_9 ( ID(2060), "rd:TRICE8_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
        break; case __LINE__: TRICE8_10( ID(1501), "rd:TRICE8_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
        break; case __LINE__: TRICE8_11( ID(2695), "rd:TRICE8_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
        break; case __LINE__: TRICE8_12( ID(1708), "rd:TRICE8_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );

        break; case __LINE__: TRICE( ID(6471), "sig:TRICE16 with 1 to 12 values\n" );
        break; case __LINE__: TRICE16( ID(6538), "rd:TRICE16 %d\n", -1 );
        break; case __LINE__: TRICE16( ID(3696), "rd:TRICE16 %d, %d\n", -1, -2 );
        break; case __LINE__: TRICE16( ID(1251), "rd:TRICE16 %d, %d, %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE16( ID(5414), "rd:TRICE16 %d, %d, %d, %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE16( ID(3048), "rd:TRICE16 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE16( ID(4978), "rd:TRICE16 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE16( ID(3529), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE16( ID(3111), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE16( ID(1209), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE16( ID(7007), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE16( ID(4837), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE16( ID(2812), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE     ( ID(3442), "sig:TRICE16_n with 1 to 12 values\n" );
        break; case __LINE__: TRICE16_1 ( ID(1933), "rd:TRICE16_1 %d\n", 1 );
        break; case __LINE__: TRICE16_2 ( ID(7609), "rd:TRICE16_2 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE16_3 ( ID(6623), "rd:TRICE16_3 %d, %d, %d\n", 1, 2, 3 );
        break; case __LINE__: TRICE16_4 ( ID(2472), "rd:TRICE16_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
        break; case __LINE__: TRICE16_5 ( ID(1686), "rd:TRICE16_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
        break; case __LINE__: TRICE16_6 ( ID(3733), "rd:TRICE16_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
        break; case __LINE__: TRICE16_7 ( ID(1861), "rd:TRICE16_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
        break; case __LINE__: TRICE16_8 ( ID(5128), "rd:TRICE16_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
        break; case __LINE__: TRICE16_9 ( ID(3837), "rd:TRICE16_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
        break; case __LINE__: TRICE16_10( ID(5487), "rd:TRICE16_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
        break; case __LINE__: TRICE16_11( ID(4041), "rd:TRICE16_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
        break; case __LINE__: TRICE16_12( ID(1434), "rd:TRICE16_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );

        break; case __LINE__: TRICE  ( ID(5093), "sig:TRICE16 with 1 to 12 pointer\n" );
        break; case __LINE__: TRICE16( ID(3840), "rd:TRICE16 %p\n", -1 );
        break; case __LINE__: TRICE16( ID(2894), "rd:TRICE16 %p, %p\n", -1, -2 );
        break; case __LINE__: TRICE16( ID(3076), "rd:TRICE16 %p, %p, %p\n", -1, -2, -3 );
        break; case __LINE__: TRICE16( ID(4508), "rd:TRICE16 %p, %p, %p, %p\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE16( ID(7477), "rd:TRICE16 %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE16( ID(5314), "rd:TRICE16 %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE16( ID(4714), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE16( ID(6427), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE16( ID(6936), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE16( ID(2406), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE16( ID(1590), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE16( ID(2312), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE  ( ID(2629), "sig:TRICE16 with 1 to 12 hex\n" );
        break; case __LINE__: TRICE16( ID(7386), "rd:TRICE16 %X\n", -1 );
        break; case __LINE__: TRICE16( ID(3292), "rd:TRICE16 %X, %X\n", -1, -2 );
        break; case __LINE__: TRICE16( ID(6368), "rd:TRICE16 %X, %X, %X\n", -1, -2, -3 );
        break; case __LINE__: TRICE16( ID(5747), "rd:TRICE16 %X, %X, %X, %X\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE16( ID(2663), "rd:TRICE16 %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE16( ID(3574), "rd:TRICE16 %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE16( ID(1215), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE16( ID(1912), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE16( ID(5378), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE16( ID(3601), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE16( ID(3665), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE16( ID(4545), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE  ( ID(3309), "sig:TRICE32 with 1 to 12 values\n" );
        break; case __LINE__: TRICE32( ID(6505), "rd:TRICE32 %d\n", -1 );
        break; case __LINE__: TRICE32( ID(2671), "rd:TRICE32 %d, %d\n", -1, -2 );
        break; case __LINE__: TRICE32( ID(3165), "rd:TRICE32 %d, %d, %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE32( ID(6859), "rd:TRICE32 %d, %d, %d, %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE32( ID(5107), "rd:TRICE32 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE32( ID(5540), "rd:TRICE32 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE32( ID(1226), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE32( ID(3813), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE32( ID(7383), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE32( ID(3013), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE32( ID(4197), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE32( ID(3317), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE     ( ID(3717), "signal:TRICE32_n with 1 to 12 values\n" );
        break; case __LINE__: TRICE32_1 ( ID(6792), "rd:TRICE32_1 %d\n", 1 );
        break; case __LINE__: TRICE32_2 ( ID(6322), "rd:TRICE32_2 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE32_3 ( ID(1788), "rd:TRICE32_3 %d, %d, %d\n", 1, 2, 3 );
        break; case __LINE__: TRICE32_4 ( ID(5032), "rd:TRICE32_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
        break; case __LINE__: TRICE32_5 ( ID(1459), "rd:TRICE32_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
        break; case __LINE__: TRICE32_6 ( ID(7751), "rd:TRICE32_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
        break; case __LINE__: TRICE32_7 ( ID(7546), "rd:TRICE32_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
        break; case __LINE__: TRICE32_8 ( ID(2573), "rd:TRICE32_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
        break; case __LINE__: TRICE32_9 ( ID(2895), "rd:TRICE32_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
        break; case __LINE__: TRICE32_10( ID(4084), "rd:TRICE32_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
        break; case __LINE__: TRICE32_11( ID(3478), "rd:TRICE32_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
        break; case __LINE__: TRICE32_12( ID(1094), "rd:TRICE32_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );

        break; case __LINE__: TRICE  ( ID(7135), "sig:TRICE64 with 1 to 12 values\n" );
        break; case __LINE__: TRICE64( ID(1357), "rd:TRICE64 %d\n", -1 );
        break; case __LINE__: TRICE64( ID(1810), "rd:TRICE64 %d, %d\n", -1, -2 );
        break; case __LINE__: TRICE64( ID(7774), "rd:TRICE64 %d, %d, %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE64( ID(4839), "rd:TRICE64 %d, %d, %d, %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE64( ID(4528), "rd:TRICE64 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE64( ID(3987), "rd:TRICE64 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE64( ID(7381), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE64( ID(5400), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE64( ID(2470), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE64( ID(2138), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE64( ID(7930), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE64( ID(2376), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE     ( ID(3682), "sig:TRICE64_n with 1 to 12 values\n" );
        break; case __LINE__: TRICE64_1 ( ID(5457), "rd:TRICE64_1 %d\n", 1 );
        break; case __LINE__: TRICE64_2 ( ID(3208), "rd:TRICE64_2 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE64_2 ( ID(5906), "rd:TRICE64_2 %x, %x\n", 0x1122334455667788, 0x0102030405060708 );
        break; case __LINE__: TRICE64_3 ( ID(2349), "rd:TRICE64_3 %d, %d, %d\n", 1, 2, 3 );
        break; case __LINE__: TRICE64_4 ( ID(7667), "rd:TRICE64_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
        break; case __LINE__: TRICE64_5 ( ID(1901), "rd:TRICE64_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
        break; case __LINE__: TRICE64_6 ( ID(4386), "rd:TRICE64_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
        break; case __LINE__: TRICE64_7 ( ID(7832), "rd:TRICE64_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
        break; case __LINE__: TRICE64_8 ( ID(6397), "rd:TRICE64_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
        break; case __LINE__: TRICE64_9 ( ID(7259), "rd:TRICE64_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
        break; case __LINE__: TRICE64_10( ID(4261), "rd:TRICE64_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
        break; case __LINE__: TRICE64_11( ID(7822), "rd:TRICE64_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
        break; case __LINE__: TRICE64_12( ID(4017), "rd:TRICE64_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );

        break; case __LINE__: TRICE( ID(6325), "sig:Colors and ticks\n" );
        break; case __LINE__: TRICE( ID(1597), "--------------------------------------------------\n");
        break; case __LINE__: TRICE( ID(2358), "--------------------------------------------------\n");
        break; case __LINE__: TRICE( ID(2746), "dbg:12345 as 16bit is %016b\n", 12345);
        break; case __LINE__: TRICE( ID(3005), "--------------------------------------------------\n");
        break; case __LINE__: TRICE( ID(7797), "sig:This ASSERT error is just a demo and no real error:\n");
        break; case __LINE__: TRICE( ID(3659), "--------------------------------------------------\n");
        break; case __LINE__: TRICE( ID(4511), "ERR:error       message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID(5362), "WRN:warning     message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID(5030), "ATT:attention   message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID(3938), "DIA:diagnostics message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID(1483), "TIM:timing      message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID(1203), "DBG:debug       message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID(1724), "SIG:signal      message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID(2174), "RD:read         message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID(1583), "WR:write        message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID(6210), "ISR:interrupt   message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID(2621), "MSG:normal      message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID(7402), "INFO:informal   message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID(3892), "time:TRICE32_1  message, SysTick is %6u\n", SYSTICKVAL);

        break; case __LINE__: TRICE( ID(6758), "sig:Various ranges and formats\n" );
        break; case __LINE__: TRICE8 ( ID(6632), "tst:TRICE8  %%03x -> %03x  %03x  %03x  %03x\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID(6774), "tst:TRICE8   %%4d -> %4d %4d %4d %4d\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID(5935), "tst:TRICE8   %%4u -> %4u %4u %4u %4u\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID(7140), "tst:TRICE8   %%4o -> %4o %4o %4o %4o\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID(5249), "tst:TRICE8   %%4O -> %4O %4O %4O %4O\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID(5950), "tst:TRICE8   %%4X -> %4X %4X %4X %4X\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID(7788), "tst:TRICE8   %%4x -> %4x %4x %4x %4x\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID(1716), "tst:TRICE8   %%8b -> %8b %8b %8b %8b\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID(5385), "tst:TRICE8  %%08b -> %08b %08b %08b %08b\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE16( ID(3312), "tst:TRICE16  %%05x ->   %05x   %05x   %05x   %05x\n", 1, 0x7fff, 0x8000, 0xffff);
        break; case __LINE__: TRICE16( ID(7472), "tst:TRICE16   %%6d ->  %6d  %6d  %6d  %6d\n",         1, 0x7fff, 0x8000, 0xffff);
        break; case __LINE__: TRICE16( ID(5482), "tst:TRICE16   %%7o -> %7o %7o %7o %7o\n",             1, 0x7fff, 0x8000, 0xffff);
        break; case __LINE__: TRICE32( ID(1123), "tst:TRICE32 %%09x ->      %09x      %09x       %09x     %09x\n", 1, 0x7fffffff, 0x80000000, 0xffffffff);
        break; case __LINE__: TRICE32( ID(2328), "tst:TRICE32 %%10d ->     %10d     %10d     %10d    %10x\n",      1, 0x7fffffff, 0x80000000, 0xffffffff);
        break; case __LINE__: TRICE32( ID(3855), "att:TRICE32 %40b\n", 0xAAAAAAAA);
        break; case __LINE__: TRICE64( ID(7829), "tst:TRICE64 %%09x ->      %09x      %09x       %09x     %09x\n", 1, 0x7fffffffffffffff, 0x8000000000000000, 0xffffffffffffffff);
        break; case __LINE__: TRICE64( ID(7596), "tst:TRICE64 %%10d ->     %10d     %10d     %10d    %10x\n",      1, 0x7fffffffffffffff, 0x8000000000000000, 0xffffffffffffffff);
        break; case __LINE__: TRICE64( ID(7490), "att:TRICE64 %70b\n", 0xAAAAAAAAAAAAAAAA);
        break; case __LINE__: TRICE( ID(3852), "sig:Legacy TRICE8\n" );
        break; case __LINE__: TRICE8_1( ID(5791), "tst:TRICE8_1 %02x\n", 0xA1);
        break; case __LINE__: TRICE8_2( ID(1398), "tst:TRICE8_2 %02x %02x\n", 0xA1, 0xA2);
        break; case __LINE__: TRICE8_3( ID(5154), "tst:TRICE8_3 %02x %02x %02x\n", 0xA1, 0xA2, 0xA3);
        break; case __LINE__: TRICE8_4( ID(5693), "tst:TRICE8_4 %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4);
        break; case __LINE__: TRICE8_5( ID(3921), "tst:TRICE8_5 %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5 );
        break; case __LINE__: TRICE8_6( ID(1733), "tst:TRICE8_6 %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6);
        break; case __LINE__: TRICE8_7( ID(2884), "tst:TRICE8_7 %02x %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7);
        break; case __LINE__: TRICE8_8( ID(5416), "tst:TRICE8_8 %02x %02x %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8);
        break; case __LINE__: TRICE8_1( ID(4196), "tst:TRICE8_1 %d\n",                      -1);
        break; case __LINE__: TRICE8_2( ID(7328), "tst:TRICE8_2 %d %d\n",                   -1, -2);
        break; case __LINE__: TRICE8_3( ID(2883), "tst:TRICE8_3 %d %d %d\n",                -1, -2, -3);
        break; case __LINE__: TRICE8_4( ID(7928), "tst:TRICE8_4 %d %d %d %d\n",             -1, -2, -3, -4);
        break; case __LINE__: TRICE8_5( ID(3863), "tst:TRICE8_5 %d %d %d %d %d\n",          -1, -2, -3, -4, -5);
        break; case __LINE__: TRICE8_6( ID(7114), "tst:TRICE8_6 %d %d %d %d %d %d\n",       -1, -2, -3, -4, -5, -6);
        break; case __LINE__: TRICE8_7( ID(2367), "tst:TRICE8_7 %d %d %d %d %d %d %d\n",    -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE8_8( ID(7076), "tst:TRICE8_8 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8);
        break; case __LINE__: TRICE8_1( ID(5699), "tst:TRICE8_1 %d\n", 1);
        break; case __LINE__: TRICE8_2( ID(1661), "tst:TRICE8_2 %d %d\n", 1, 2);
        break; case __LINE__: TRICE8_3( ID(6281), "tst:TRICE8_3 %d %d %d\n", 1, 2, 3);
        break; case __LINE__: TRICE8_4( ID(4312), "tst:TRICE8_4 %d %d %d %d\n", 1, 2, 3, 4);
        break; case __LINE__: TRICE8_5( ID(4413), "tst:TRICE8_5 %d %d %d %d %d\n", 1, 2, 3, 4, 5);
        break; case __LINE__: TRICE8_6( ID(5054), "tst:TRICE8_6 %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6);
        break; case __LINE__: TRICE8_7( ID(3126), "tst:TRICE8_7 %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7);
        break; case __LINE__: TRICE8_8( ID(3904), "tst:TRICE8_8 %d %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7, 8);
        break; case __LINE__: TRICE8_1( ID(3803), "tst:TRICE8_1 %u\n", 201);
        break; case __LINE__: TRICE8_2( ID(7837), "tst:TRICE8_2 %u %u\n", 201, 202);
        break; case __LINE__: TRICE8_3( ID(2608), "tst:TRICE8_3 %u %u %u\n", 201, 202, 203);
        break; case __LINE__: TRICE8_4( ID(1463), "tst:TRICE8_4 %u %u %u %u\n", 201, 202, 203, 204);
        break; case __LINE__: TRICE8_5( ID(1979), "tst:TRICE8_5 %u %u %u %u %u\n", 201, 202, 203, 204, 205);
        break; case __LINE__: TRICE8_6( ID(1013), "tst:TRICE8_6 %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206);
        break; case __LINE__: TRICE8_7( ID(5937), "tst:TRICE8_7 %u %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206, 207);
        break; case __LINE__: TRICE8_8( ID(1270), "tst:TRICE8_8 %u %u %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206, 207, 208);
        break; case __LINE__: TRICE8_8( ID(5686), "tst:TRICE8_1 %%d=%d, %%u=%u, 0x%%x=0x%x, 0x%%2x=0x%2x, 0x%%02x=0x%02x, 0x%%3x=0x%3x, 0x%%03x=0x%03x, %%b=%b\n", 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81);

        break; case __LINE__: TRICE    ( ID(2840), "sig:Legacy TRICE16-64\n" );
        break; case __LINE__: TRICE16_1( ID(4503), "tst:TRICE16_1 %d\n", -111);
        break; case __LINE__: TRICE16_2( ID(3693), "tst:TRICE16_2 %d %d\n", -111, -222);
        break; case __LINE__: TRICE16_3( ID(2455), "tst:TRICE16_3 %d %d %d\n", -111, -222, -333);
        break; case __LINE__: TRICE16_4( ID(7433), "tst:TRICE16_4 %d %d %d %d\n", -111, -222, -333, -444);
        break; case __LINE__: TRICE32_1( ID(7899), "tst:TRICE32_1 %08x\n", 0x0123cafe);
        break; case __LINE__: TRICE32_1( ID(2827), "tst:TRICE32_1 %d\n", -111);
        break; case __LINE__: TRICE32_2( ID(4713), "tst:TRICE32_2 %x %x\n", -111, -222);
        break; case __LINE__: TRICE32_2( ID(7856), "tst:TRICE32_2 %d %d\n", -111, -222);
        break; case __LINE__: TRICE32_3( ID(4870), "tst:TRICE32_3 %x %x %x\n", -111, -222, -333);
        break; case __LINE__: TRICE32_3( ID(7175), "tst:TRICE32_3 %d %d %d\n", -111, -222, -333);
        break; case __LINE__: TRICE32_4( ID(7989), "tst:TRICE32_4 %x %x %x %x\n", -111, -222, -333, -444);
        break; case __LINE__: TRICE32_4( ID(6185), "tst:TRICE32_4 %d %d %d %d\n", -111, -222, -333, -444);
        break; case __LINE__: TRICE64_1( ID(1746), "tst:TRICE64_1 %d\n", -111);
        break; case __LINE__: TRICE64_2( ID(3875), "tst:TRICE64_2 %d %d\n", -111, -222);
        break; case __LINE__: TRICE16_1( ID(7450), "tst:TRICE16_1 %u\n", 60001);
        break; case __LINE__: TRICE16_2( ID(5625), "tst:TRICE16_2 %u %u\n", 60001, 60002);
        break; case __LINE__: TRICE16_3( ID(3423), "tst:TRICE16_3 %u %u %u\n", 60001, 60002, 60003);
        break; case __LINE__: TRICE16_4( ID(6860), "tst:TRICE16_4 %u %u %u %u\n", 60001, 60002, 60003, 60004);
        break; case __LINE__: TRICE32_1( ID(3957), "tst:TRICE32_1 %u\n", 4000000001);
        break; case __LINE__: TRICE32_2( ID(1823), "tst:TRICE32_2 %u %u\n", 4000000001, 4000000002);
        break; case __LINE__: TRICE32_3( ID(7393), "tst:TRICE32_3 %u %u %u\n", 4000000001, 4000000002, 4000000003);
        break; case __LINE__: TRICE32_4( ID(4450), "tst:TRICE32_4 %u %u %u %u\n", 4000000001, 4000000002, 4000000003, 4000000004);
        break; case __LINE__: TRICE64_1( ID(5760), "tst:TRICE64_1 %x\n", -1);
        break; case __LINE__: TRICE64_2( ID(4234), "tst:TRICE64_2 %x %x\n", -1, -2);
        break; case __LINE__: TRICE64_1( ID(6086), "tst:TRICE64_1 %u\n", -1);
        break; case __LINE__: TRICE64_2( ID(3587), "tst:TRICE64_2 %u %u\n", -1, -2);

        break; case __LINE__: TRICE( ID(4154), "sig:colored single letters and several TRICE macros in one line\n" );
        break; case __LINE__: TRICE( ID(2113), "e:A");
        break; case __LINE__: TRICE( ID(6217), "w:B");
        break; case __LINE__: TRICE( ID(3295), "a:c");
        break; case __LINE__: TRICE( ID(4168), "wr:d");
        break; case __LINE__: TRICE( ID(5280), "rd:e\n");
        break; case __LINE__: TRICE( ID(5886), "diag:f");
        break; case __LINE__: TRICE( ID(6699), "d:G");
        break; case __LINE__: TRICE( ID(7253), "t:H");
        break; case __LINE__: TRICE( ID(6574), "time:i");
        break; case __LINE__: TRICE( ID(3373), "message:J");
        break; case __LINE__: TRICE( ID(2571), "dbg:k\n");
        break; case __LINE__: TRICE( ID(5326), "1");
        break; case __LINE__: TRICE( ID(4753), "2");
        break; case __LINE__: TRICE( ID(5641), "3");
        break; case __LINE__: TRICE( ID(5524), "4");
        break; case __LINE__: TRICE( ID(6872), "e:7");
        break; case __LINE__: TRICE( ID(5049), "m:12");
        break; case __LINE__: TRICE( ID(5011), "m:123\n");
        break; case __LINE__: TRICE( ID(1672), "e:A"); TRICE( ID(6738), "w:B"); TRICE( ID(3912), "a:c");
        break; case __LINE__: TRICE( ID(7068), "wr:d"); TRICE( ID(3059), "rd:e\n"); TRICE( ID(1112), "diag:f");

        break; case __LINE__: TRICE( ID(4840), "sig:TRICE8 with variable param count 1 to 12\n" );
        break; case __LINE__: TRICE8( ID(7565), "tst:TRICE8 %d\n", -1 );
        break; case __LINE__: TRICE8( ID(7978), "tst:TRICE8 %d %d\n", -1, -2 );
        break; case __LINE__: TRICE8( ID(4432), "tst:TRICE8 %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE8( ID(2620), "tst:TRICE8 %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE8( ID(3697), "tst:TRICE8 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE8( ID(1335), "tst:TRICE8 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE8( ID(4156), "tst:TRICE8 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE8( ID(1465), "tst:TRICE8 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE8( ID(4707), "tst:TRICE8 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE8( ID(7583), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE8( ID(2729), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE8( ID(2116), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE8( ID(6196), "tst:TRICE8 %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );
        break; case __LINE__: TRICE8_1 ( ID(2206), "tst:TRICE8_1  %d\n", -1 );
        break; case __LINE__: TRICE8_2 ( ID(3888), "tst:TRICE8_2  %d %d\n", -1, -2 );
        break; case __LINE__: TRICE8_3 ( ID(2352), "tst:TRICE8_3  %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE8_4 ( ID(1443), "tst:TRICE8_4  %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE8_5 ( ID(2825), "tst:TRICE8_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE8_6 ( ID(4599), "tst:TRICE8_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE8_7 ( ID(7066), "tst:TRICE8_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE8_8 ( ID(4995), "tst:TRICE8_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE8_9 ( ID(7847), "tst:TRICE8_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE8_10( ID(4722), "tst:TRICE8_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE8_11( ID(3390), "tst:TRICE8_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE8_12( ID(5992), "tst:TRICE8_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE8   ( ID(4668), "tst:TRICE8   %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );
        break; case __LINE__: TRICE8_9 ( ID(1729), "tst:TRICE8_9 %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );

        break; case __LINE__: TRICE( ID(1553), "sig:TRICE16 with variable param count 1 to 12\n" );
        break; case __LINE__: TRICE16( ID(3291), "tst:TRICE16 %d\n", -1 );
        break; case __LINE__: TRICE16( ID(1416), "tst:TRICE16 %d %d\n", -1, -2 );
        break; case __LINE__: TRICE16( ID(3634), "tst:TRICE16 %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE16( ID(7120), "tst:TRICE16 %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE16( ID(1575), "tst:TRICE16 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE16( ID(6885), "tst:TRICE16 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE16( ID(5716), "tst:TRICE16 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE16( ID(5480), "tst:TRICE16 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE16( ID(5780), "tst:TRICE16 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE16( ID(5673), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE16( ID(3632), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE16( ID(7169), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE16( ID(2467), "tst:TRICE16 %t %b %x %X %d %u %o %O %p\n", 3, 3, 3, 3, 3, 3, 3, 3, 3 );
        break; case __LINE__: TRICE16_1 ( ID(4152), "tst:TRICE16_1  %d\n", -1 );
        break; case __LINE__: TRICE16_2 ( ID(4596), "tst:TRICE16_2  %d %d\n", -1, -2 );
        break; case __LINE__: TRICE16_3 ( ID(7715), "tst:TRICE16_3  %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE16_4 ( ID(4209), "tst:TRICE16_4  %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE16_5 ( ID(5609), "tst:TRICE16_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE16_6 ( ID(4760), "tst:TRICE16_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE16_7 ( ID(4987), "tst:TRICE16_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE16_8 ( ID(5462), "tst:TRICE16_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE16_9 ( ID(5456), "tst:TRICE16_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE16_10( ID(2018), "tst:TRICE16_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE16_11( ID(5717), "tst:TRICE16_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE16_12( ID(4167), "tst:TRICE16_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE16_9 ( ID(2832), "tst:TRICE16 %t %b %x %X %d %u %o %O %p\n", 3, 3, 3, 3, 3, 3, 3, 3, 3 );

        break; case __LINE__: TRICE( ID(2313), "sig:TRICE32 with variable param count 1 to 12\n" );
        break; case __LINE__: TRICE32( ID(4247), "tst:TRICE32 %d\n", -1 );
        break; case __LINE__: TRICE32( ID(2580), "tst:TRICE32 %d %d\n", -1, -2 );
        break; case __LINE__: TRICE32( ID(7921), "tst:TRICE32 %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE32( ID(2975), "tst:TRICE32 %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE32( ID(6383), "tst:TRICE32 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE32( ID(4171), "tst:TRICE32 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE32( ID(6506), "tst:TRICE32 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE32( ID(1796), "tst:TRICE32 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE32( ID(1662), "tst:TRICE32 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE32( ID(3136), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE32( ID(3923), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE32( ID(4560), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE32( ID(4323), "tst:TRICE32 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));
        break; case __LINE__: TRICE32_1 ( ID(4229), "tst:TRICE32_1  %d\n", -1 );
        break; case __LINE__: TRICE32_2 ( ID(3526), "tst:TRICE32_2  %d %d\n", -1, -2 );
        break; case __LINE__: TRICE32_3 ( ID(2176), "tst:TRICE32_3  %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE32_4 ( ID(1506), "tst:TRICE32_4  %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE32_5 ( ID(3020), "tst:TRICE32_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE32_6 ( ID(2816), "tst:TRICE32_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE32_7 ( ID(7531), "tst:TRICE32_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE32_8 ( ID(2596), "tst:TRICE32_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE32_9 ( ID(4025), "tst:TRICE32_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE32_10( ID(3231), "tst:TRICE32_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE32_11( ID(5259), "tst:TRICE32_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE32_12( ID(2285), "tst:TRICE32_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE32   ( ID(2460), "tst:TRICE32    %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));
        break; case __LINE__: TRICE32_12( ID(1877), "tst:TRICE32_12 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));

        break; case __LINE__: TRICE( ID(6995), "sig:TRICE64 with variable param count 1 to 12\n" );
        break; case __LINE__: TRICE64( ID(3967), "tst:TRICE64 %d\n", -1 );
        break; case __LINE__: TRICE64( ID(5770), "tst:TRICE64 %d %d\n", -1, -2 );
        break; case __LINE__: TRICE64( ID(3653), "tst:TRICE64 %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE64( ID(3725), "tst:TRICE64 %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE64( ID(1368), "tst:TRICE64 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE64( ID(6660), "tst:TRICE64 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE64( ID(4911), "tst:TRICE64 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE64( ID(1220), "tst:TRICE64 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE64( ID(7431), "tst:TRICE64 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE64( ID(6692), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE64( ID(6690), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE64( ID(5960), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE64( ID(4889), "tst:TRICE64 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aDouble(3.14159), aDouble(3.14159), aDouble(3.14159));
        break; case __LINE__: TRICE64_1 ( ID(4022), "tst:TRICE64_1  %d\n", -1 );
        break; case __LINE__: TRICE64_2 ( ID(1502), "tst:TRICE64_2  %d %d\n", -1, -2 );
        break; case __LINE__: TRICE64_3 ( ID(6504), "tst:TRICE64_3  %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE64_4 ( ID(3513), "tst:TRICE64_4  %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE64_5 ( ID(1081), "tst:TRICE64_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE64_6 ( ID(3971), "tst:TRICE64_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE64_7 ( ID(7681), "tst:TRICE64_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE64_8 ( ID(1768), "tst:TRICE64_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE64_9 ( ID(5697), "tst:TRICE64_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE64_10( ID(3611), "tst:TRICE64_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE64_11( ID(1507), "tst:TRICE64_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE64_12( ID(3761), "tst:TRICE64_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE64_12( ID(1798), "tst:TRICE64_12 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aDouble(3.14159), aDouble(3.14159), aDouble(3.14159));

        break; case __LINE__: TRICE( ID(5755), "att:positive and negative float in format variants\n");
        break; case __LINE__: TRICE( ID(2771), "rd:TRICE float %f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID(1889), "rd:TRICE float %9f (%%9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID(1344), "rd:TRICE float %.9f (%%.9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID(7722), "rd:TRICE float %9.f (%%9.f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID(3491), "rd:TRICE float %9.6f (%%9.6f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID(3052), "rd:TRICE float %f (%%f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(7208), "rd:TRICE float %9f (%%9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(2146), "rd:TRICE float %.9f (%%.9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(2551), "rd:TRICE float %9.f (%%9.f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(2666), "rd:TRICE float %9.6f (%%9.6f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(2539), "rd:TRICE float %+f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID(2158), "rd:TRICE float %+9f (%%9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID(5931), "rd:TRICE float %+.9f (%%.9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID(2759), "rd:TRICE float %+9.f (%%9.f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID(3655), "rd:TRICE float %+9.6f (%%9.6f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID(6713), "rd:TRICE float %+f (%%f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(6905), "rd:TRICE float %+9f (%%9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(1313), "rd:TRICE float %+.9f (%%.9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(5020), "rd:TRICE float %+9.f (%%9.f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(5481), "rd:TRICE float %+9.6f (%%9.6f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(1886), "rd:TRICE float %-f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID(7331), "rd:TRICE float %-9f (%%9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID(6027), "rd:TRICE float %-.9f (%%.9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID(6461), "rd:TRICE float %-9.f (%%9.f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID(6588), "rd:TRICE float %-9.6f (%%9.6f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID(7943), "rd:TRICE float %-f (%%f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(7817), "rd:TRICE float %-9f (%%9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(6151), "rd:TRICE float %-.9f (%%.9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(7704), "rd:TRICE float %-9.f (%%9.f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(2732), "rd:TRICE float %-9.6f (%%9.6f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(5867), "att:positive float & double in variants\n");
        break; case __LINE__: TRICE32_1( ID(5490), "rd:TRICE32_1 float %e (%%e)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1( ID(1856), "rd:TRICE32_1 float %f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1( ID(6782), "rd:TRICE32_1 float %g (%%g)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1( ID(6134), "rd:TRICE32_1 float %E (%%E)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1( ID(3890), "rd:TRICE32_1 float %F (%%F)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1( ID(2674), "rd:TRICE32_1 float %G (%%G)\n", aFloat(x) );
        break; case __LINE__: TRICE32  ( ID(4919), "rd:TRICE32 float %e (%%e)\n", aFloat(x) );
        break; case __LINE__: TRICE32  ( ID(4452), "rd:TRICE32 float %f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE32  ( ID(1964), "rd:TRICE32 float %g (%%g)\n", aFloat(x) );
        break; case __LINE__: TRICE32  ( ID(5909), "rd:TRICE32 float %E (%%E)\n", aFloat(x) );
        break; case __LINE__: TRICE32  ( ID(5168), "rd:TRICE32 float %F (%%F)\n", aFloat(x) );
        break; case __LINE__: TRICE32  ( ID(1742), "rd:TRICE32 float %G (%%G)\n", aFloat(x) );
        break; case __LINE__: TRICE    ( ID(4108), "rd:TRICE float %e (%%e)\n", aFloat(x) );
        break; case __LINE__: TRICE    ( ID(4942), "rd:TRICE float %f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE    ( ID(3931), "rd:TRICE float %g (%%g)\n", aFloat(x) );
        break; case __LINE__: TRICE    ( ID(6364), "rd:TRICE float %E (%%E)\n", aFloat(x) );
        break; case __LINE__: TRICE    ( ID(5127), "rd:TRICE float %F (%%F)\n", aFloat(x) );
        break; case __LINE__: TRICE    ( ID(4119), "rd:TRICE float %G (%%G)\n", aFloat(x) );
        break; case __LINE__: TRICE64  ( ID(6126), "rd:TRICE64 double %e (%%e), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64  ( ID(2266), "rd:TRICE64 double %f (%%f), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64  ( ID(4946), "rd:TRICE64 double %g (%%g), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64  ( ID(6087), "rd:TRICE64 double %E (%%E), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64  ( ID(3860), "rd:TRICE64 double %F (%%F), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64  ( ID(6315), "rd:TRICE64 double %G (%%G), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( ID(1633), "rd:TRICE64_1 double %e (%%e), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( ID(5975), "rd:TRICE64_1 double %f (%%f), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( ID(5924), "rd:TRICE64_1 double %g (%%g), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( ID(3413), "rd:TRICE64_1 double %E (%%E), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( ID(1417), "rd:TRICE64_1 double %F (%%F), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( ID(3002), "rd:TRICE64_1 double %G (%%G), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE( ID(1291), "att:negative float & double\n" );
        break; case __LINE__: TRICE( ID(6832), "rd:TRICE float %e (%%e)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(6593), "rd:TRICE float %f (%%f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(7961), "rd:TRICE float %g (%%g)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(4731), "rd:TRICE float %E (%%E)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(4608), "rd:TRICE float %F (%%F)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(6140), "rd:TRICE float %G (%%G)\n", aFloat(-x) );
        break; case __LINE__: TRICE64( ID(1923), "rd:TRICE64 double %e (%%e), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64( ID(1279), "rd:TRICE64 double %f (%%f), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64( ID(3965), "rd:TRICE64 double %g (%%g), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64( ID(3392), "rd:TRICE64 double %E (%%E), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64( ID(3080), "rd:TRICE64 double %F (%%F), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64( ID(3706), "rd:TRICE64 double %G (%%G), aDouble(y)\n", aDouble(-y) );

        break; case __LINE__: TRICE  ( ID(1120), "att:formatted float & double\n" );
        break; case __LINE__: TRICE32( ID(4513), "rd:TRICE32 float %12.6e (%%12.6e)\n", aFloat(x) );
        break; case __LINE__: TRICE32( ID(1567), "rd:TRICE32 float %12.6f (%%12.6f)\n", aFloat(x) );
        break; case __LINE__: TRICE32( ID(6978), "rd:TRICE32 float %12.6g (%%12.6g)\n", aFloat(x) );
        break; case __LINE__: TRICE32( ID(3110), "rd:TRICE32 float %12.6E (%%12.6E)\n", aFloat(x) );
        break; case __LINE__: TRICE32( ID(4356), "rd:TRICE32 float %12.6F (%%12.6F)\n", aFloat(x) );
        break; case __LINE__: TRICE32( ID(1154), "rd:TRICE32 float %12.6G (%%12.6G)\n", aFloat(x) );
        break; case __LINE__: TRICE64( ID(3325), "rd:TRICE64 double %12.6e (%%12.6e), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64( ID(4886), "rd:TRICE64 double %12.6f (%%12.6f), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64( ID(3492), "rd:TRICE64 double %12.6g (%%12.6g), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64( ID(5431), "rd:TRICE64 double %12.6E (%%12.6E), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64( ID(6420), "rd:TRICE64 double %12.6F (%%12.6F), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64( ID(6828), "rd:TRICE64 double %12.6G (%%12.6G), aDouble(y)\n", aDouble(y) );

        break; case __LINE__: TRICE ( ID(4600), "att:Various single arguments\n" );
        break; case __LINE__: TRICE8( ID(7940), "rd:TRICE8 line %t (%%t ,0)\n", 0 );
        break; case __LINE__: TRICE8( ID(4770), "rd:TRICE8 line %t (%%t ,2)\n", 2 );
        break; case __LINE__: TRICE8( ID(3255), "rd:TRICE8 line %u (%%u ,-1)\n", -1 );
        break; case __LINE__: TRICE8( ID(2572), "rd:TRICE8 line %b (%%b ,-1)\n", -1 );
        break; case __LINE__: TRICE8( ID(1312), "rd:TRICE8 line %o (%%o ,-1)\n", -1 );
        break; case __LINE__: TRICE8( ID(5451), "rd:TRICE8 line %O (%%O ,-1)\n", -1 );
        break; case __LINE__: TRICE8( ID(6519), "rd:TRICE8 line %X (%%X ,-1)\n", -1 );
        break; case __LINE__: TRICE8( ID(3209), "rd:TRICE8 line %x (%%x ,-1)\n", -1 );
        break; case __LINE__: TRICE8( ID(7662), "rd:TRICE8 line %d (%%d ,-1)\n", -1 );
        break; case __LINE__: TRICE16_1( ID(1523), "rd:TRICE16_1 line %t (%%t ,0)\n", 0 );
        break; case __LINE__: TRICE16_1( ID(7327), "rd:TRICE16_1 line %t (%%t ,2)\n", 2 );
        break; case __LINE__: TRICE16_1( ID(4683), "rd:TRICE16_1 line %u (%%u -1)\n", -1 );
        break; case __LINE__: TRICE16_1( ID(1378), "rd:TRICE16_1 line %b (%%b -1)\n", -1 );
        break; case __LINE__: TRICE16_1( ID(5302), "rd:TRICE16_1 line %o (%%o -1)\n", -1 );
        break; case __LINE__: TRICE16_1( ID(2888), "rd:TRICE16_1 line %x (%%x -1)\n", -1 );
        break; case __LINE__: TRICE16_1( ID(3838), "rd:TRICE16_1 line %d (%%d -1)\n", -1 );
        break; case __LINE__: TRICE( ID(1709), "rd:TRICE line %t (%%t -1)\n", 0 );
        break; case __LINE__: TRICE( ID(1697), "rd:TRICE line %t (%%t -1)\n", 2 );
        break; case __LINE__: TRICE( ID(7869), "rd:TRICE line %u (%%u)\n", -1 );
        break; case __LINE__: TRICE( ID(3424), "rd:TRICE line %b (%%b)\n", -2 );
        break; case __LINE__: TRICE( ID(6167), "rd:TRICE line %o (%%o)\n", -3 );
        break; case __LINE__: TRICE( ID(7244), "rd:TRICE line %x (%%x)\n", -4 );
        break; case __LINE__: TRICE( ID(4061), "rd:TRICE line %d (%%d)\n", -5 );

        break; case __LINE__: TRICE_N( ID(1657), "msg:%s\n", A, 0 );
        break; case __LINE__: TRICE_N( ID(4040), "msg:%s\n", A, 1 );
        break; case __LINE__: TRICE_N( ID(7859), "msg:%s\n", A, 2 );
        break; case __LINE__: TRICE_N( ID(2432), "msg:%s\n", A, 3 );
        break; case __LINE__: TRICE_N( ID(1311), "msg:%s\n", A, 4 );
        break; case __LINE__: TRICE_N( ID(7569), "msg:%s\n", A, 5 );
        break; case __LINE__: TRICE_N( ID(2021), "msg:%s\n", A, 6 );
        break; case __LINE__: TRICE_N( ID(5263), "msg:%s\n", A, 7 );
        break; case __LINE__: TRICE_N( ID(1653), "msg:%s\n", A, 8 );
        break; case __LINE__: TRICE_N( ID(3752), "msg:%s\n", A, 9 );
        break; case __LINE__: TRICE_N( ID(2882), "msg:%s\n", A, 10 );
        break; case __LINE__: TRICE_N( ID(3552), "msg:%s\n", A, 11 );
        break; case __LINE__: TRICE_N( ID(5597), "msg:%s\n", A, 12 );
        break; case __LINE__: TRICE_N( ID(4202), "msg:%s\n", A, 13 );
        break; case __LINE__: TRICE_N( ID(6630), "msg:%s\n", A, 14 );
        break; case __LINE__: TRICE_N( ID(2881), "msg:%s\n", A, 15 );
        break; case __LINE__: TRICE_N( ID(2556), "msg:%s\n", A, 16 );
        break; case __LINE__: TRICE_N( ID(3438), "msg:%s\n", A, 17 );
        break; case __LINE__: TRICE_N( ID(2670), "msg:%s\n", A, 18 );
        break; case __LINE__: TRICE_N( ID(7602), "msg:%s\n", A, 19 );
        break; case __LINE__: TRICE_N( ID(4727), "msg:%s\n", A, 120 );
        break; case __LINE__: TRICE_N( ID(7460), "msg:%s\n", A, 121 );
        break; case __LINE__: TRICE_N( ID(4150), "msg:%s\n", A, 122 );
        break; case __LINE__: TRICE_N( ID(1327), "msg:%s\n", A, 123 );
        break; case __LINE__: TRICE_N( ID(5593), "msg:%s\n", A, 124 );
        break; case __LINE__: TRICE_N( ID(7831), "msg:%s\n", A, 125 );
        break; case __LINE__: TRICE_N( ID(4854), "msg:%s\n", A, 126 );
        break; case __LINE__: TRICE_N( ID(7079), "msg:%s\n", A, 127 );
        break; case __LINE__: TRICE_N( ID(2069), "msg:%s\n", A, 128 );
        break; case __LINE__: TRICE_N( ID(1851), "msg:%s\n", A, 129 );

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

/*
//! SCOPY is a helper macro for struct serialization.
#define SCOPY( element ) do{ char* n = #element; int size = sizeof( src->element ); memcpy( p, &(src->element), size ); p += size; TRICE_S( ID(5559), "rd:sizeof(%8s)", n ); TRICE( ID(2163), " = %d\n", size);}while(0);

    //! DCOPY is a helper macro for struct deserialization.
#define DCOPY( element ) do{ char* n = #element; int size = sizeof( dst->element ); memcpy( &(dst->element), p, size ); p += size; TRICE_S( ID(2043), "rd:sizeof(%8s)", n ); TRICE( ID(6227), " = %d\n", size);}while(0);


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
    TRICE ( Id(7835), "inf: Tryout tx struct:" );
    TRICE8_B( Id(5303), " %02x ", &tx, sizeof(tx) );
    TRICE( Id(4818), "\n" );

    TRICE ( Id(7395), "inf: Tryout buffer:" );
    TRICE8_B( Id(6138), " %02x ", dst, len ); //lint !e670
    TRICE( Id(6795), "\n" );

    src = dst; // "data transfer"

    len = deserializeTryout( &rx, src );
    TRICE ( Id(4698), "inf: Tryout rx struct:" );
    TRICE8_B( Id(5255), " %02x ", &rx, sizeof(rx) );
    TRICE( Id(6200), "\n" );

    TRICE( Id(7990), "inf:sizeOf(Trypout) = %d, buffer length = %d\n", sizeof(tx), len );
    TRICE8_F( Id(5783), "info:TryoutStructFunction", &tx, sizeof(tx) );
    TRICE8_F( Id(4985), "info:TryoutBufferFunction", dst, len ); //lint !e670
}
*/

static void exampleOfManualJSONencoding(void){
    typedef  struct {
    int Apple, Birn;
    float Fish;
    } Ex_t;
    Ex_t Ex = { -1, 2, (float)2.781 };
    TRICE( Id(3685), "att:MyStructEvaluationFunction(json:ExA{Apple:%d, Birn:%u, Fisch:%f}\n", Ex.Apple, Ex.Birn, aFloat(Ex.Fish) );
}
