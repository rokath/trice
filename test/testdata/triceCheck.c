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

        break; case __LINE__: trice16( iD(5413), "att: line %u\n", __LINE__ );

        // normal use cases
        break; case __LINE__: TRICE( ID(1301), "info:This is a message without values and a 32-bit stamp.\n" ); //exp: time: 842,150_450default: info:This is a message without values and a 32-bit stamp.
        break; case __LINE__: Trice( iD(7783), "info:This is a message without values and a 16-bit stamp.\n" ); //exp: time:       5_654default: info:This is a message without values and a 16-bit stamp.
        break; case __LINE__: trice( iD(4188), "info:This is a message without values and without stamp.\n"  ); //exp: time:            default: info:This is a message without values and without stamp.

        // special use cases
        break; case __LINE__: TRice64( iD(5415), "info:12 64-bit values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 32-bit stamp.\n", -3, -4, -5, -6, 0, aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), -8 ); //exp: time: 842,150_450default: info:12 64-bit values -3, 18446744073709551612, fffffffffffffffb, FFFFFFFFFFFFFFFA, false, -7.123457e+00, -7.123457, -7.123456789, -7.123457E+00, -7.123457, -7.123456789, 0xb1111111111111111111111111111111111111111111111111111111111111000 and a 32-bit stamp.
        break; case __LINE__: Trice64( iD(6746), "info:12 64-bit values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 16-bit stamp.\n", -3, -4, -5, -6, 0, aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), -8 ); //exp: time:       5_654default: info:12 64-bit values -3, 18446744073709551612, fffffffffffffffb, FFFFFFFFFFFFFFFA, false, -7.123457e+00, -7.123457, -7.123456789, -7.123457E+00, -7.123457, -7.123456789, 0xb1111111111111111111111111111111111111111111111111111111111111000 and a 16-bit stamp.
        break; case __LINE__: trice64( iD(1741), "info:12 64-bit values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and without stamp.\n" , -3, -4, -5, -6, 0, aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), -8 ); //exp: time:            default: info:12 64-bit values -3, 18446744073709551612, fffffffffffffffb, FFFFFFFFFFFFFFFA, false, -7.123457e+00, -7.123457, -7.123456789, -7.123457E+00, -7.123457, -7.123456789, 0xb1111111111111111111111111111111111111111111111111111111111111000 and without stamp.
        break; case __LINE__: TRice32( iD(5099), "info:12 32-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 32-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time: 842,150_450default: info:12 32-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 32-bit stamp.
        break; case __LINE__: Trice32( iD(2760), "info:12 32-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 16-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:       5_654default: info:12 32-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 16-bit stamp.
        break; case __LINE__: trice32( iD(3881), "info:12 32-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and without stamp.\n" , -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:            default: info:12 32-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and without stamp.
        break; case __LINE__: TRice16( iD(6093), "info:12 16-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 32-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time: 842,150_450default: info:12 16-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 32-bit stamp.
        break; case __LINE__: Trice16( iD(3121), "info:12 16-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 16-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:       5_654default: info:12 16-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 16-bit stamp.
        break; case __LINE__: trice16( iD(6984), "info:12 16-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and without stamp.\n" , -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:            default: info:12 16-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and without stamp.
        break; case __LINE__: TRice8 ( iD(6861), "info:12  8-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 32-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time: 842,150_450default: info:12  8-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 32-bit stamp.
        break; case __LINE__: Trice8 ( iD(7306), "info:12  8-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 16-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:       5_654default: info:12  8-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 16-bit stamp.
        break; case __LINE__: trice8 ( iD(4171), "info:12  8-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and without stamp.\n" , -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:            default: info:12  8-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and without stamp.

        break; case __LINE__: trice16( iD(7893), "att: line %u\n", __LINE__ );

        // CGO compiler issue
    //  break; case __LINE__: TRice( iD(5224), "info:12 default bit width values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 32-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //_exp: time: 842,150_450default: info:12 default bit width values -3, 4294967292, fffffffb, FFFFFFFA, truet, 1.401298e-45, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11000000111000111111001101011100 and a 32-bit stamp.
    //  break; case __LINE__: Trice( iD(6329), "info:12 default bit width values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 16-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //_exp: time:       5_654default: info:12 default bit width values -3, 4294967292, fffffffb, FFFFFFFA, truet, 1.401298e-45, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11000000111000111111001101011100 and a 16-bit stamp.
    //  break; case __LINE__: trice( iD(1426), "info:12 default bit width values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and without stamp.\n" , -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //_exp: time:            default: info:12 default bit width values -3, 4294967292, fffffffb, FFFFFFFA, truet, 1.401298e-45, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11000000111000111111001101011100 and without stamp.

        // todo: improve parsing of //exp string containing special characters
        //break; case __LINE__: TRice( iD(5826), "A TRice message containing a newline\nbefore the line end\n" );               //exp: time: 842,150_450default: A TRice message containing a newline\n                                         before the line end                                                                                                      
        //break; case __LINE__: TRice( iD(3988), "\nA TRice message starting with a a newline\n" );                             //exp: time: 842,150_450default: \n                                         A TRice message starting with a a newline                                                                                                                   
        //break; case __LINE__: TRice( iD(7586), "line0:A TRice message with several lines\nline1\nline2\nline3:last line\n" ); //exp: time: 842,150_450default: line0:A TRice message with several lines\\n                                         line1\n                                         line2\n                                         line3:last line

        break; case __LINE__: TRice16( iD(5485), "att: 0x8888 == %04xh\n", 0x8888 ); //exp: time: 842,150_450default: att: 0x8888 == 8888h
        break; case __LINE__: Trice16( iD(6875), "att: 0x8888 == %04xh\n", 0x8888 ); //exp: time:       5_654default: att: 0x8888 == 8888h
        break; case __LINE__: trice16( iD(4549), "att: 0x8888 == %04xh\n", 0x8888 ); //exp: time:            default: att: 0x8888 == 8888h


        break; case __LINE__: trice16( iD(2748), "att: line %u\n", __LINE__ );

        break; case __LINE__: trice16( iD(7646), "att: line %u\n", __LINE__ );
      //break; case __LINE__: exampleOfManualSerialization(); // ATTENTION: This occupies ~1024 bytes in one half buffer when double buffer ist used!

        break; case __LINE__: trice8   ( iD(6538), "value=%x\n", 0x55 );                           //exp: time:            default: value=55
        break; case __LINE__: trice8   ( iD(7178), "value=%x\n", 0x88 );                           //exp: time:            default: value=88

        break; case __LINE__: TRICE( ID(6533), "att:mixed int & float & double & bits\n" );
        break; case __LINE__: TRICE32( ID(7137), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",       FloatToInt32(x),   aFloat(x),   aFloat(x),   aFloat(x) ); //exp: time: 842,150_450default: rd:TRICE32 int 1089, float 1089.608276 (%f), 44883377, 01000100100010000011001101110111                                         
    //  break; case __LINE__: TRICE32( ID(4703), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",          (int32_t)(x),   aFloat(x),   aFloat(x),   aFloat(x) ); //exp: time: 842,150_450default: rd:TRICE32 int 1089, float 1089.608276 (%f), 44883377, 01000100100010000011001101110111                                         
        break; case __LINE__: TRICE32( ID(6167), "rd:TRICE32 int %x, float %f (%%f), %08x, %032b\n",            0x44883377,   aFloat(x),   aFloat(x),   aFloat(x) ); //exp: time: 842,150_450default: rd:TRICE32 int 44883377, float 1089.608276 (%f), 44883377, 01000100100010000011001101110111                                         
        break; case __LINE__: TRICE32( ID(5053), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",                     x,   aFloat(x),   aFloat(x),   aFloat(x) ); //exp: time: 842,150_450default: rd:TRICE32 int 1089, float 1089.608276 (%f), 44883377, 01000100100010000011001101110111                                         
        break; case __LINE__: TRICE32( ID(4579), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",      FloatToInt32(-x),  aFloat(-x),  aFloat(-x),  aFloat(-x) ); //exp: time: 842,150_450default: rd:TRICE32 int -1089, float -1089.608276 (%f), c4883377, 11000100100010000011001101110111                                       
        break; case __LINE__: TRICE32( ID(3347), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",         (int32_t)(-x),  aFloat(-x),  aFloat(-x),  aFloat(-x) ); //exp: time: 842,150_450default: rd:TRICE32 int -1089, float -1089.608276 (%f), c4883377, 11000100100010000011001101110111                                       
    //  break; case __LINE__: TRICE32( ID(4551), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",                    -x,  aFloat(-x),  aFloat(-x),  aFloat(-x) ); //exp: time: 842,150_450default: rd:TRICE32 int 1089, float -1089.608276 (%f), c4883377, 11000100100010000011001101110111                                        
        break; case __LINE__: TRICE32( ID(1592), "rd:TRICE32 int %X, float %f (%%f), %08x, %032b\n",           -0x44883377,  aFloat(-x),  aFloat(-x),  aFloat(-x) ); //exp: time: 842,150_450default: rd:TRICE32 int BB77CC89, float -1089.608276 (%f), c4883377, 11000100100010000011001101110111                                        
        break; case __LINE__: TRICE64( ID(1121), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",    DoubleToInt64(y),  aDouble(y),  aDouble(y),  aDouble(y) ); //exp: time: 842,150_450default: rd:TRICE64 int 518, double 518.054749 (%f), 4080307020601050, 0100000010000000001100000111000000100000011000000001000001010000   
        break; case __LINE__: TRICE64( ID(6035), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",        (int64_t)(y),  aDouble(y),  aDouble(y),  aDouble(y) ); //exp: time: 842,150_450default: rd:TRICE64 int 518, double 518.054749 (%f), 4080307020601050, 0100000010000000001100000111000000100000011000000001000001010000   
        break; case __LINE__: TRICE64( ID(4010), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",                   y,  aDouble(y),  aDouble(y),  aDouble(y) ); //exp: time: 842,150_450default: rd:TRICE64 int 518, double 518.054749 (%f), 4080307020601050, 0100000010000000001100000111000000100000011000000001000001010000   
        break; case __LINE__: TRICE64( ID(3682), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",   DoubleToInt64(-y), aDouble(-y), aDouble(-y), aDouble(-y) ); //exp: time: 842,150_450default: rd:TRICE64 int -518, double -518.054749 (%f), c080307020601050, 1100000010000000001100000111000000100000011000000001000001010000
        break; case __LINE__: TRICE64( ID(2702), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",       (int64_t)(-y), aDouble(-y), aDouble(-y), aDouble(-y) ); //exp: time: 842,150_450default: rd:TRICE64 int -518, double -518.054749 (%f), c080307020601050, 1100000010000000001100000111000000100000011000000001000001010000
    //  break; case __LINE__: TRICE64( ID(2682), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",                  -y, aDouble(-y), aDouble(-y), aDouble(-y) ); //exp: time: 842,150_450default: rd:TRICE64 int 518, double -518.054749 (%f), c080307020601050, 1100000010000000001100000111000000100000011000000001000001010000 
        break; case __LINE__: TRICE64( ID(1531), "rd:TRICE64 int %X, double %f (%%f), %016x, %064b\n", -0x4080307020601050, aDouble(-y), aDouble(-y), aDouble(-y) ); //exp: time: 842,150_450default: rd:TRICE64 int BF7FCF8FDF9FEFB0, double -518.054749 (%f), c080307020601050, 1100000010000000001100000111000000100000011000000001000001010000 

        break; case __LINE__: trice16( iD(6971), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRICE_S( ID(4331), "msg:With TRICE_S:%s\n", sABCDE );                           //exp: time: 842,150_450default: msg:With TRICE_S:abcde 12345
        break; case __LINE__: TRICE_N( ID(2778), "sig:With TRICE_N:%s\n", sABCDE, lenABCDE );                 //exp: time: 842,150_450default: sig:With TRICE_N:abcde 12345
        break; case __LINE__: TRICE8_B( ID(2114), "  %02x", sABCDE, lenABCDE );                               //exp: time: 842,150_450default:   61  62  63  64  65  20  31  32  33  34  35
        break; case __LINE__: TRICE( ID(3246), "\n" );
        break; case __LINE__: TRICE8_B( ID(6802), "%4d", sABCDE, lenABCDE );                                  //exp: time: 842,150_450default:   97  98  99 100 101  32  49  50  51  52  53
        break; case __LINE__: TRICE( ID(5449), "\n" );
        break; case __LINE__: TRICE8_B( ID(1889), "  %02x", b8, sizeof(b8)/sizeof(int8_t) );                  //exp: time: 842,150_450default:   00  ff  fe  33  04  05  06  07  08  09  0a  0b  00  ff  fe  33  04  05  06  07  08  09  0a  0b
        break; case __LINE__: TRICE( ID(5108), "\n" );
        break; case __LINE__: TRICE16_B( ID(4553), "  %04x", b16, sizeof(b16)/sizeof(int16_t) );              //exp: time: 842,150_450default:   0000  ffff  fffe  3344
        break; case __LINE__: TRICE( ID(1060), "\n" );
        break; case __LINE__: TRICE32_B( ID(1382), "  %08x", b32, sizeof(b32)/sizeof(int32_t) );              //exp: time: 842,150_450default:   00000000  ffffffff  fffffffe  33445555
        break; case __LINE__: TRICE( ID(4700), "\n" );
        break; case __LINE__: TRICE64_B( ID(5471), "  %016x", b64, sizeof(b64)/sizeof(int64_t) );             //exp: time: 842,150_450default:   0000000000000000  ffffffffffffffff  fffffffffffffffe  3344555566666666
        break; case __LINE__: TRICE( ID(7814), "\n" );
        break; case __LINE__: TRICE8_F( ID(5376), "info:FunctionNameW", b8,  sizeof(b8) /sizeof(int8_t) );   //exp: time: 842,150_450default: info:FunctionNameW(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)
        break; case __LINE__: TRICE16_F( ID(6459), "info:FunctionNameX", b16, sizeof(b16)/sizeof(int16_t) );  //exp: time: 842,150_450default: info:FunctionNameX(0000)(ffff)(fffe)(3344) 
        break; case __LINE__: TRICE32_F( ID(6468), "info:FunctionNameY", b32, sizeof(b32)/sizeof(int32_t) );  //exp: time: 842,150_450default: info:FunctionNameY(00000000)(ffffffff)(fffffffe)(33445555)
        break; case __LINE__: TRICE64_F( ID(6045), "info:FunctionNameZ", b64, sizeof(b64)/sizeof(int64_t) );  //exp: time: 842,150_450default: info:FunctionNameZ(0000000000000000)(ffffffffffffffff)(fffffffffffffffe)(3344555566666666)
        break; case __LINE__: exampleOfManualJSONencoding(); //exp: time:       5_654default: att:MyStructEvaluationFunction(json:ExA{Apple:-1, Birn:2, Fisch:2.781000}

        break; case __LINE__: trice16( iD(3954), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRICE( Id(7806), "MSG:1/11 = %g\n", aFloat( 1.0/11 ) ); //exp: time:       5_654default: MSG:1/11 = 0.09090909
        break; case __LINE__: { //exp: time: 842,150_450default: msg:x = 5.934 = 5.934, 5.934
                              float a = (float)5.934;
                              float b = a + ((a > 0) ? 0.0005f : -0.0005f);
                              int c = (int)b;
                              int d = (int)(b * 1000) % 1000;
                              int e = (int)(1000 * (float)(a - c)); 
                              TRICE( ID(3479), "msg:x = %g = %d.%03d, %d.%03d\n", aFloat(a), c, d, c, e ); 
                              }

        //break; case __LINE__: TRICE( id(5922), "dbg:Hi!\n" );                       //exp: time:            default: dbg:Hi!    
        //break; case __LINE__: TRICE( id(4459), "dbg:\tHi!\n" );                     
        //break; case __LINE__: TRICE( id(4141), "dbg:\t\tHi!\n" );                   
        //break; case __LINE__: TRICE( id(6855), "dbg:\t\t\tHi!\n" );                 
        //break; case __LINE__: TRICE( id(7112), "dbg:\\tHi!\n" );                    
        //break; case __LINE__: TRICE( Id(5408), "dbg:\aHi!\n" ); // sound!
        //break; case __LINE__: TRICE( Id(7950), "dbg:\a\aHi!\n" ); // sound!
        //break; case __LINE__: TRICE( Id(2655), "dbg:\a\a\aHi!\n" ); // sound!
        //break; case __LINE__: TRICE( Id(6559), "dbg:\\aHi!\n" );
        break; case __LINE__: TRICE( Id(6234), "err:An ERROR messgage with sound!\a\n" ); // sound!

        break; case __LINE__: TRICE_S( ID(6403), "sig:TRICE_S=%s\n", s );            //exp: time: 842,150_450default: sig:TRICE_S=AAAAAAAAAAAA
        break; case __LINE__: TRICE_N( ID(3609), "sig:TRICE_N=%s\n", s, strlen(s) ); //exp: time: 842,150_450default: sig:TRICE_N=AAAAAAAAAAAA

        break; case __LINE__: trice16( iD(3167), "att: line %u\n", __LINE__ );

//      break; case __LINE__: 
//  #ifdef XTEA_ENCRYPT_KEY
//          TRICE0 ( Id(4696), "--------------------------------------------------\n" );
//          {
//              uint32_t by[8] = {1,2,3,4,5,6};
//              int count = (sizeof(by) + 7) & ~7; // only multiple of 8 encryptable
//              TRICE32( Id(1033), "msg: message = %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
//              TRICE32( Id(6956), "tim: pre TriceEncryption SysTick=%d\n", SYSTICKVAL );
//              XTEAEncrypt(by, count>>2);
//              TRICE32( Id(1465), "tim: post TriceEncryption SysTick=%d\n", SYSTICKVAL );
//              TRICE32( Id(4532), "att: TriceEncrypted =  %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
//              #ifdef XTEA_DECRYPT
//              TRICE16_1( Id(4986), "tim: pre TriceDecryption SysTick=%d\n", SYSTICKVAL );
//              XTEADecrypt(by, count>>2);
//              TRICE16_1( Id(1348), "tim: post TriceDecryption SysTick=%d\n", SYSTICKVAL );
//              #endif
//              TRICE32( Id(4355), "msg: messge = %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
//          }
//          TRICE0 ( Id(5250), "--------------------------------------------------\n" );
//  #endif

        break; case __LINE__: trice16( iD(4701), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRICE( ID(4439), "sig:Some time measurements\n" );
        break; case __LINE__: TRICE( ID(5822), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID(7878), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID(5755), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID(2861), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
        break; case __LINE__: TRICE64( ID(3424), "rd:TRICE64 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE( ID(3595), "tim:TRICE START time message\n");
        break; case __LINE__: TRICE( ID(2009), "tim:TRICE STOP time message\n");
        break; case __LINE__: TRICE( ID(7141), "tim:TRICE START time message\n");
        break; case __LINE__: TRICE( ID(4559), "tim:TRICE STOP time message\n");
        break; case __LINE__: TRICE( ID(6344), "tim:TRICE START time message\n");
        break; case __LINE__: TRICE64( ID(3503), "rd:TRICE64 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE( ID(7435), "tim:TRICE STOP time message\n");
        break; case __LINE__: TRICE( ID(6146), "tim:TRICE --------------------------------------------------\n");
        break; case __LINE__: TRICE( ID(2104), "tim:TRICE --------------------------------------------------\n");
        break; case __LINE__: TRICE64( ID(2690), "rd:TRICE64 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE( ID(4392), "tim:TRICE --------------------------------------------------\n");
        break; case __LINE__: TRICE( ID(2903), "tim:TRICE --------------------------------------------------\n");
        break; case __LINE__: TRICE64( ID(3701), "rd:TRICE64 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE( ID(2889), "rd:TRICE32_1 line %d\n", __LINE__ );
        break; case __LINE__: TRICE64_1( ID(6626), "rd:TRICE64_1 %d\n", __LINE__ );
        break; case __LINE__: TRICE32_2( ID(7557), "rd:TRICE32_2 line %d,%d\n", __LINE__, 2 );
        break; case __LINE__: TRICE64_2( ID(4149), "rd:TRICE64_2 line %d,%d\n", __LINE__, 2 );

        break; case __LINE__: trice16( iD(1130), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRICE( ID(4694), "sig:Runtime generated strings\n" );
        break; case __LINE__: {
                                  s = "AAAAAAAAAAAA";
                                  TRICE32( ID(1482), "dbg:len=%u:", strlen(s) );
                                  TRICE_S( ID(6170), "sig:%s\n", s );
                              }
        break; case __LINE__: {
                s = "\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBCCCCCCCC\
";
                          }
        break; case __LINE__: TRICE32( ID(5402), "dbg:len=%u:", strlen(s) );
        break; case __LINE__: TRICE_S( ID(6390), "sig:%s\n", s );
        break; case __LINE__: TRICE( ID(6989), "sig:Runtime generated strings\n" );
        break; case __LINE__: {
            s = "\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\
DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\
EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\
";
                          }
        break; case __LINE__: TRICE32( ID(1018), "dbg:len=%u: ", strlen(s) );
        break; case __LINE__: TRICE_S( ID(5723), "sig:%s\n", s );
        break; case __LINE__: TRICE( ID(3551), "sig:Runtime generated strings\n" );
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
        break; case __LINE__: TRICE32( ID(1933), "dbg:len=%u: ", strlen(s) );
        break; case __LINE__: TRICE_S( ID(2980), "sig:%s\n", s );
        break; case __LINE__: TRICE( ID(3245), "sig:Runtime generated strings\n" );
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
        break; case __LINE__: TRICE32( ID(1886), "dbg:len=%u: ", strlen(s) );
        break; case __LINE__: TRICE_S( ID(4441), "sig:%s\n", s );

        break; case __LINE__: trice16( iD(4655), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRICE( ID(2848), "sig:Integer (indent, base, sign) - see https://yourbasic.org/golang/fmt-printf-reference-cheat-sheet/\n" );
        break; case __LINE__: TRICE( ID(3011), "rd: 15 	%d 	Base 10\n", 15 );                                        //ex_: time: 842,150_450default: rd: 15 	15 	Base 10                                   
        break; case __LINE__: TRICE( ID(3838), "rd: +15 	%+d 	Always show sign\n", 15 );                           //ex_: time: 842,150_450default: rd: +15 	+15 	Always show sign                          
        break; case __LINE__: TRICE( ID(1092), "rd:   15 	%4d 	Pad with spaces (width 4, right justified)\n", 15 ); //ex_: time: 842,150_450default: rd:   15	  15 	Pad with spaces (width 4, right justified)
        break; case __LINE__: TRICE( ID(5431), "rd: 15   	%-4d 	Pad with spaces (width 4, left justified)\n", 15 );  //ex_: time: 842,150_450default: rd: 15 	15   	Pad with spaces (width 4, left justified) 
        break; case __LINE__: TRICE( ID(3368), "rd: 0015 	%04d 	Pad with zeroes (width 4)\n", 15 );                  //ex_: time: 842,150_450default: rd: 0015 	0015 	Pad with zeroes (width 4)                 
        break; case __LINE__: TRICE( ID(1022), "rd: 1111 	%b 	Base 2\n", 15 );                                         //ex_: time: 842,150_450default: rd: 1111 	1111    Base 2                                    
        break; case __LINE__: TRICE( ID(2052), "rd: 17 	%o 	Base 8\n", 15 );                                         //ex_: time: 842,150_450default: rd: 17 	17      Base 8                                    
        break; case __LINE__: TRICE( ID(1880), "rd: f 	%x 	Base 16, lowercase\n", 15 );                             //ex_: time: 842,150_450default: rd: f 	f 	Base 16, lowercase                        
        break; case __LINE__: TRICE( ID(4923), "rd: F 	%X 	Base 16, uppercase\n", 15 );                             //ex_: time: 842,150_450default: rd: F 	F 	Base 16, uppercase                        
        break; case __LINE__: TRICE( ID(3118), "rd: 0xf 	%#x 	Base 16, with leading 0x\n", 15 );                   //ex_: time: 842,150_450default: rd: 0xf 	0xf 	Base 16, with leading 0x                  

        break; case __LINE__: trice16( iD(2255), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRICE( ID(7273), "sig:Character (quoted, Unicode)\n" );
        break; case __LINE__: TRICE( ID(2612), "rd: A 	%c 	Character\n", 'A' );                                     //exp: time: 842,150_450default: rd: A 	A 	Character
        break; case __LINE__: TRICE( ID(1013), "rd: 'A' 	%q 	Quoted character\n", 'A' );                              //exp: time: 842,150_450default: rd: 'A' 	'A' 	Quoted character
    //  break; case __LINE__: TRICE( ID(6876), "rd: U+0041 	%U 	Unicode\n", 'A' );                        // todo: detect %U to avoid message triceType = TRICE0 ParamSpace = 4 not matching with bitWidth  0 and paramCount 0 - ignoring package
    //  break; case __LINE__: TRICE( ID(7841), "rd: U+0041 'A' 	%#U 	Unicode with character\n", 'A' ); // todo: detect %#U to avoid message triceType = TRICE0 ParamSpace = 4 not matching with bitWidth  0 and paramCount 0 - ignoring package

        break; case __LINE__: TRICE( ID(6730), "sig:Boolean (true/false)\n" );
        break; case __LINE__: TRICE( ID(1651), "rd:Use %%t to format a boolean as true (%t) or false (%t).\n", 1, 0 ); //exp: time: 842,150_450default: rd:Use %t to format a boolean as true (true) or false (false).

        break; case __LINE__: TRICE( ID(5375), "sig:Pointer (hex)\n" );
        break; case __LINE__: TRICE8 ( ID(7655), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x84 );               //exp: time: 842,150_450default: rd:Use %p to format a pointer in base 16 notation with leading 0x. (84)              
        break; case __LINE__: TRICE16( ID(2771), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x1234 );             //exp: time: 842,150_450default: rd:Use %p to format a pointer in base 16 notation with leading 0x. (1234)            
        break; case __LINE__: TRICE32( ID(6175), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0xaabbccdd );         //exp: time: 842,150_450default: rd:Use %p to format a pointer in base 16 notation with leading 0x. (aabbccdd)        
        break; case __LINE__: TRICE64( ID(4993), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x1122334455667788 ); //exp: time: 842,150_450default: rd:Use %p to format a pointer in base 16 notation with leading 0x. (1122334455667788)
        break; case __LINE__: TRICE  ( ID(4148), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0xaabbccdd);          //exp: time: 842,150_450default: rd:Use %p to format a pointer in base 16 notation with leading 0x. (aabbccdd)        

        break; case __LINE__: trice16( iD(7229), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRICE( ID(5058), "sig:Float (indent, precision, scientific notation)\n" );
        break; case __LINE__: TRICE( ID(3297), "rd: 1.234560e+02		%e 	%%e Scientific notation\n", aFloat(123.456) );                       //exp: time: 842,150_450default: rd: 1.234560e+02		1.234560e+02 	%e Scientific notation
        break; case __LINE__: TRICE( ID(6494), "rd: 123.456000		%f 	%%f Decimal point, no exponent\n", aFloat(123.456) );                //exp: time: 842,150_450default: rd: 123.456000		123.456001 	%f Decimal point, no exponent
        break; case __LINE__: TRICE( ID(7858), "rd: 123.46			%.2f		%%.2f Default width, precision 2\n", aFloat(123.456) );      //exp: time: 842,150_450default: rd: 123.46			123.46		%.2f Default width, precision 2
        break; case __LINE__: TRICE( ID(3210), "rd: ␣␣123.46		%8.2f	%%8.2f Width 8, precision 2\n", aFloat(123.456) );                    //exp: time: 842,150_450default: rd: ␣␣123.46		  123.46	%8.2f Width 8, precision 2
        break; case __LINE__: TRICE( ID(3127), "rd: 123.456		%g		%%g Exponent as needed, necessary digits only\n", aFloat(123.456) ); //exp: time: 842,150_450default: rd: 123.456		123.456		%g Exponent as needed, necessary digits only

        break; case __LINE__: TRICE( ID(6143), "sig:Double (indent, precision, scientific notation)\n" );
        break; case __LINE__: TRICE64( ID(2143), "rd: 1.234560e+02		%e 	%%e Scientific notation\n", aDouble(123.456) );                   //exp: time: 842,150_450default: rd: 1.234560e+02		1.234560e+02 	%e Scientific notation
        break; case __LINE__: TRICE64( ID(5170), "rd: 123.456000		%f 	%%f Decimal point, no exponent\n", aDouble(123.456) );                //exp: time: 842,150_450default: rd: 123.456000		123.456000 	%f Decimal point, no exponent
        break; case __LINE__: TRICE64( ID(4924), "rd: 123.46			%.2f    	%%.2f Default width, precision 2\n", aDouble(123.456) );      //exp: time: 842,150_450default: rd: 123.46			123.46    	%.2f Default width, precision 2
        break; case __LINE__: TRICE64( ID(1617), "rd: ␣␣123.46		%8.2f 	%%8.2f Width 8, precision 2\n", aDouble(123.456) );                //exp: time: 842,150_450default: rd: ␣␣123.46		  123.46 	%8.2f Width 8, precision 2
        break; case __LINE__: TRICE64( ID(4390), "rd: 123.456		%g		%%g Exponent as needed, necessary digits only\n", aDouble(123.456) ); //exp: time: 842,150_450default: rd: 123.456		123.456		%g Exponent as needed, necessary digits only

        break; case __LINE__: TRICE( ID(4520), "sig:String or byte slice (quote, indent, hex)\n" );
        break; case __LINE__: s = "café"; TRICE_S( ID(4939), "rd: café 			%s		Plain string\n", s );           //exp: time: 842,150_450default: rd: café 			café		Plain string
        break; case __LINE__: s = "café"; TRICE_S( ID(1899), "rd: ␣␣café 		%6s 		Width 6, right justify\n", s );  //exp: time: 842,150_450default: rd: ␣␣café 		  café 		Width 6, right justify
        break; case __LINE__: s = "café"; TRICE_S( ID(3392), "rd: café␣␣ 		%-6s 		Width 6, left justify\n", s );   //exp: time: 842,150_450default: rd: café␣␣ 		café   		Width 6, left justify
        break; case __LINE__: s = "café"; TRICE_S( ID(5578), "rd: \"café\" 		%q		Quoted string\n", s );          //exp: time: 842,150_450default: rd: \"café\" 		"café"		Quoted string
        break; case __LINE__: s = "café"; TRICE_S( ID(1885), "rd: 636166c3a9 		%x	Hex dump of byte values\n", s );    //exp: time: 842,150_450default: rd: 636166c3a9 		636166c3a9	Hex dump of byte values 
        break; case __LINE__: s = "café"; TRICE_S( ID(7168), "rd: 63 61 66 c3 a9 	% x	Hex dump with spaces\n", s );       //exp: time: 842,150_450default: rd: 63 61 66 c3 a9 	63 61 66 c3 a9	Hex dump with spaces

        break; case __LINE__: trice16( iD(5159), "att: line %u\n", __LINE__ );

#ifdef TRICE_CGO

        break; case __LINE__: TRICE  ( ID(2796), "rd:%E (%%E)\n",  aFloat(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-5.555556E+08 (%E)
        break; case __LINE__: TRICE  ( ID(6999), "rd:%F (%%F)\n",  aFloat(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-555555584.000000 (%F)
        break; case __LINE__: TRICE  ( ID(7492), "rd:%G (%%G)\n",  aFloat(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-5.555556E+08 (%G)
        break; case __LINE__: TRICE64( ID(5199), "rd:%E (%%E)\n", aDouble(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-5.555556E+08 (%E)
        break; case __LINE__: TRICE64( ID(2258), "rd:%F (%%F)\n", aDouble(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-555555555.555556 (%F)
        break; case __LINE__: TRICE64( ID(7712), "rd:%G (%%G)\n", aDouble(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-5.555555555555556E+08 (%G)
        
        break; case __LINE__: TRICE  ( ID(5044), "rd:%e (%%e)\n",  aFloat(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-5.555556e+08 (%e)
        break; case __LINE__: TRICE  ( ID(4996), "rd:%f (%%f)\n",  aFloat(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-555555584.000000 (%f)
        break; case __LINE__: TRICE  ( ID(3886), "rd:%g (%%g)\n",  aFloat(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-5.555556e+08 (%g)
        break; case __LINE__: TRICE64( ID(1811), "rd:%e (%%e)\n", aDouble(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-5.555556e+08 (%e)
        break; case __LINE__: TRICE64( ID(5130), "rd:%f (%%f)\n", aDouble(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-555555555.555556 (%f)
        break; case __LINE__: TRICE64( ID(2805), "rd:%g (%%g)\n", aDouble(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-5.555555555555556e+08 (%g)

        break; case __LINE__: TRICE32( ID(5855), "msg:%u (%%u)\n", -1 ); //exp: time: 842,150_450default: msg:4294967295 (%u)
        break; case __LINE__: TRICE32( ID(5150), "msg:%b (%%b)\n", -1 ); //exp: time: 842,150_450default: msg:11111111111111111111111111111111 (%b)
        break; case __LINE__: TRICE32( ID(2516), "msg:%o (%%o)\n", -1 ); //exp: time: 842,150_450default: msg:37777777777 (%o)
        break; case __LINE__: TRICE32( ID(4653), "msg:%O (%%O)\n", -1 ); //exp: time: 842,150_450default: msg:0o37777777777 (%O)
        break; case __LINE__: TRICE32( ID(6610), "msg:%X (%%X)\n", -1 ); //exp: time: 842,150_450default: msg:FFFFFFFF (%X)
        break; case __LINE__: TRICE32( ID(2318), "msg:%x (%%x)\n", -1 ); //exp: time: 842,150_450default: msg:ffffffff (%x)
        break; case __LINE__: TRICE32( ID(4178), "msg:%d (%%d)\n", -1 ); //exp: time: 842,150_450default: msg:-1 (%d)
        break; case __LINE__: TRICE64( ID(2526), "msg:%u (%%u)\n", -1 ); //exp: time: 842,150_450default: msg:18446744073709551615 (%u)
        break; case __LINE__: TRICE64( ID(7823), "msg:%b (%%b)\n", -1 ); //exp: time: 842,150_450default: msg:1111111111111111111111111111111111111111111111111111111111111111 (%b)
        break; case __LINE__: TRICE64( ID(7678), "msg:%o (%%o)\n", -1 ); //exp: time: 842,150_450default: msg:1777777777777777777777 (%o)
        break; case __LINE__: TRICE64( ID(2959), "msg:%O (%%O)\n", -1 ); //exp: time: 842,150_450default: msg:0o1777777777777777777777 (%O)
        break; case __LINE__: TRICE64( ID(3001), "msg:%X (%%X)\n", -1 ); //exp: time: 842,150_450default: msg:FFFFFFFFFFFFFFFF (%X)
        break; case __LINE__: TRICE64( ID(1357), "msg:%x (%%x)\n", -1 ); //exp: time: 842,150_450default: msg:ffffffffffffffff (%x)

        //break; case __LINE__: TRice        ( iD(2307), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        //break; case __LINE__: Trice        ( iD(4001), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        //break; case __LINE__: trice        ( iD(5546), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: TRice8       ( iD(1360), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: Trice8       ( iD(4690), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: trice8       ( iD(4975), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: TRice16      ( iD(3186), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: Trice16      ( iD(3197), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: trice16      ( iD(2918), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: TRice32      ( iD(5502), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: Trice32      ( iD(4344), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: trice32      ( iD(7713), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: TRice64      ( iD(6920), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: Trice64      ( iD(6945), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: trice64      ( iD(7774), "value=%d\n", -2  ); //exp: time:            default: value=-2
        
        //break; case __LINE__: TRice_1      ( iD(1057), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        //break; case __LINE__: Trice_1      ( iD(7429), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        //break; case __LINE__: trice_1      ( iD(5503), "value=%d\n", -2  ); //exp: time:            default: value=-2
        //break; case __LINE__: TRice8_1     ( iD(1278), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        //break; case __LINE__: Trice8_1     ( iD(2563), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        //break; case __LINE__: trice8_1     ( iD(1272), "value=%d\n", -2  ); //exp: time:            default: value=-2
        //break; case __LINE__: TRice16_1    ( iD(5534), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        //break; case __LINE__: Trice16_1    ( iD(6859), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        //break; case __LINE__: trice16_1    ( iD(7280), "value=%d\n", -2  ); //exp: time:            default: value=-2
        //break; case __LINE__: TRice32_1    ( iD(1076), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        //break; case __LINE__: Trice32_1    ( iD(7421), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        //break; case __LINE__: trice32_1    ( iD(2149), "value=%d\n", -2  ); //exp: time:            default: value=-2
        //break; case __LINE__: TRice64_1    ( iD(5720), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        //break; case __LINE__: Trice64_1    ( iD(6832), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        //break; case __LINE__: trice64_1    ( iD(6801), "value=%d\n", -2  ); //exp: time:            default: value=-2
        
        //break; case __LINE__: TRice        ( iD(4465), "no value" ); //exp: time: 842,150_450default: no value
        //break; case __LINE__: Trice        ( iD(6054), "no value" ); //exp: time:       5_654default: no value
        //break; case __LINE__: trice        ( iD(2395), "no value" ); //exp: time:            default: no value
        //break; case __LINE__: TRice8       ( iD(2376), "no value" ); //exp: time: 842,150_450default: no value
        //break; case __LINE__: Trice8       ( iD(5588), "no value" ); //exp: time:       5_654default: no value
        //break; case __LINE__: trice8       ( iD(6233), "no value" ); //exp: time:            default: no value
        //break; case __LINE__: TRice16      ( iD(5733), "no value" ); //exp: time: 842,150_450default: no value
        //break; case __LINE__: Trice16      ( iD(3396), "no value" ); //exp: time:       5_654default: no value
        //break; case __LINE__: trice16      ( iD(3129), "no value" ); //exp: time:            default: no value
        //break; case __LINE__: TRice32      ( iD(3290), "no value" ); //exp: time: 842,150_450default: no value
        //break; case __LINE__: Trice32      ( iD(4849), "no value" ); //exp: time:       5_654default: no value
        //break; case __LINE__: trice32      ( iD(3719), "no value" ); //exp: time:            default: no value
        //break; case __LINE__: TRice64      ( iD(5853), "no value" ); //exp: time: 842,150_450default: no value
        //break; case __LINE__: Trice64      ( iD(7236), "no value" ); //exp: time:       5_654default: no value
        //break; case __LINE__: trice64      ( iD(2313), "no value" ); //exp: time:            default: no value
        
        //break; case __LINE__: TRice_0      ( iD(3725), "no value" ); //exp: time: 842,150_450default: no value
        //break; case __LINE__: Trice_0      ( iD(6524), "no value" ); //exp: time:       5_654default: no value
        //break; case __LINE__: trice_0      ( iD(7086), "no value" ); //exp: time:            default: no value
        //break; case __LINE__: TRice8_0     ( iD(3413), "no value" ); //exp: time: 842,150_450default: no value
        //break; case __LINE__: Trice8_0     ( iD(2591), "no value" ); //exp: time:       5_654default: no value
        //break; case __LINE__: trice8_0     ( iD(1533), "no value" ); //exp: time:            default: no value
        //break; case __LINE__: TRice16_0    ( iD(3180), "no value" ); //exp: time: 842,150_450default: no value
        //break; case __LINE__: Trice16_0    ( iD(7852), "no value" ); //exp: time:       5_654default: no value
        //break; case __LINE__: trice16_0    ( iD(7490), "no value" ); //exp: time:            default: no value
        //break; case __LINE__: TRice32_0    ( iD(4203), "no value" ); //exp: time: 842,150_450default: no value
        //break; case __LINE__: Trice32_0    ( iD(3919), "no value" ); //exp: time:       5_654default: no value
        //break; case __LINE__: trice32_0    ( iD(2336), "no value" ); //exp: time:            default: no value
        //break; case __LINE__: TRice64_0    ( iD(3242), "no value" ); //exp: time: 842,150_450default: no value
        //break; case __LINE__: Trice64_0    ( iD(7427), "no value" ); //exp: time:       5_654default: no value
        //break; case __LINE__: trice64_0    ( iD(5347), "no value" ); //exp: time:            default: no value

    //  break; case __LINE__: TRice( iD(1313), "info:This is a message with one value %d and a 32-bit stamp.\n", -2  ); //exp: time: 842,150_450default: info:This is a message with one value -2 and a 32-bit stamp.
    //  break; case __LINE__: Trice( iD(3426), "info:This is a message with one value %d and a 16-bit stamp.\n", -2  ); //exp: time:       5_654default: info:This is a message with one value -2 and a 16-bit stamp.
    //  break; case __LINE__: trice( iD(6646), "info:This is a message with one value %d and without stamp.\n" , -2  ); //exp: time:            default: info:This is a message with one value -2 and without stamp.

        break; case __LINE__: TRICE( ID(6561), "info:This is a message without values and a 32-bit stamp.\n" );        //exp: time: 842,150_450default: info:This is a message without values and a 32-bit stamp.
        break; case __LINE__: TRICE( Id(4754), "info:This is a message without values and a 16-bit stamp.\n" );        //exp: time:       5_654default: info:This is a message without values and a 16-bit stamp.
        break; case __LINE__: TRICE( id(7403), "info:This is a message without values and without stamp.\n"  );        //exp: time:            default: info:This is a message without values and without stamp.

        break; case __LINE__: TRICE( ID(1487), "info:This is a message with one value %d and a 32-bit stamp.\n", -2  ); //exp: time: 842,150_450default: info:This is a message with one value -2 and a 32-bit stamp.
        break; case __LINE__: TRICE( Id(7222), "info:This is a message with one value %d and a 16-bit stamp.\n", -2  ); //exp: time:       5_654default: info:This is a message with one value -2 and a 16-bit stamp.
        break; case __LINE__: TRICE( id(4791), "info:This is a message with one value %d and without stamp.\n" , -2  ); //exp: time:            default: info:This is a message with one value -2 and without stamp.

        break; case __LINE__: TRICE( ID(5734), "info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 32-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 );   //exp: time: 842,150_450default: info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and a 32-bit stamp.
        break; case __LINE__: TRICE( Id(5357), "info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 16-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 );   //exp: time:       5_654default: info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and a 16-bit stamp.
        break; case __LINE__: TRICE( id(2374), "info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and without stamp.\n" , -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 );   //exp: time:            default: info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and without stamp.

        break; case __LINE__: TRICE32( ID(4732), "info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 32-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //exp: time: 842,150_450default: info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and a 32-bit stamp.
        break; case __LINE__: TRICE32( Id(4740), "info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 16-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //exp: time:       5_654default: info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and a 16-bit stamp.
        break; case __LINE__: TRICE32( id(5605), "info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and without stamp.\n" , -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //exp: time:            default: info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and without stamp.

        break; case __LINE__: TRICE8   ( id(1946), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: TRICE8_1 ( id(4511), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: TRICE8   ( Id(4954), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRICE8_1 ( Id(2393), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRICE8   ( ID(3344), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: TRICE8_1 ( ID(7094), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: trice8   ( iD(3822), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: trice8_1 ( iD(7235), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: Trice8   ( iD(6250), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: Trice8_1 ( iD(5834), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRice8   ( iD(4315), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: TRice8_1 ( iD(4245), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200

        break; case __LINE__: TRICE16  ( id(4394), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: TRICE16_1( id(4555), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: TRICE16  ( Id(6598), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRICE16_1( Id(6867), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRICE16  ( ID(5626), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: TRICE16_1( ID(1244), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: trice16  ( iD(1299), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: trice16_1( iD(3708), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: Trice16  ( iD(2404), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: Trice16_1( iD(1211), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRice16  ( iD(2432), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: TRice16_1( iD(5466), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200

        break; case __LINE__: TRICE32  ( id(2589), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: TRICE32_1( id(7380), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: TRICE32  ( Id(7719), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRICE32_1( Id(6594), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRICE32  ( ID(2356), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: TRICE32_1( ID(6440), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: trice32  ( iD(7630), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: trice32_1( iD(6224), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: Trice32  ( iD(3533), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: Trice32_1( iD(6983), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRice32  ( iD(4989), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: TRice32_1( iD(1701), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200

        break; case __LINE__: TRICE64  ( id(4209), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: TRICE64_1( id(7707), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: TRICE64  ( Id(7798), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRICE64_1( Id(7371), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRICE64  ( ID(2351), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: TRICE64_1( ID(4302), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: trice64  ( iD(3689), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: trice64_1( iD(1692), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: Trice64  ( iD(1788), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: Trice64_1( iD(3136), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRice64  ( iD(6491), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: TRice64_1( iD(6097), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200

        // mixed 16-bit and 32-bit timestamp 
        break; case __LINE__: TRice8( iD(7880), "msg:value=%d\n", -1 );                                                                                            //exp: time: 842,150_450default: msg:value=-1
        break; case __LINE__: Trice8( iD(5104), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: msg:value=-1, -2
        break; case __LINE__: trice8( iD(7903), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: msg:value=-1, -2, -3
        break; case __LINE__: TRice8( iD(4428), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice8( iD(6222), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice8( iD(7661), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice8( iD(1840), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice8( iD(2322), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice8( iD(5892), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice8( iD(7182), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice8( iD(3683), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice8( iD(7868), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice16( iD(2217), "msg:value=%d\n", -1 );                                                                                            //exp: time: 842,150_450default: msg:value=-1
        break; case __LINE__: Trice16( iD(4142), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: msg:value=-1, -2
        break; case __LINE__: trice16( iD(5116), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: msg:value=-1, -2, -3
        break; case __LINE__: TRice16( iD(1796), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice16( iD(4281), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice16( iD(2119), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice16( iD(1763), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice16( iD(4499), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice16( iD(2513), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice16( iD(6789), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice16( iD(7474), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice16( iD(2960), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice32( iD(1388), "msg:value=%d\n", -1 );                                                                                            //exp: time: 842,150_450default: msg:value=-1
        break; case __LINE__: Trice32( iD(6726), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: msg:value=-1, -2
        break; case __LINE__: trice32( iD(2358), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: msg:value=-1, -2, -3
        break; case __LINE__: TRice32( iD(6316), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice32( iD(6992), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice32( iD(5368), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice32( iD(6671), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice32( iD(2360), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice32( iD(5665), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice32( iD(5900), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice32( iD(4400), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice32( iD(5864), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice64( iD(5596), "msg:value=%d\n", -1 );                                                                                            //exp: time: 842,150_450default: msg:value=-1
        break; case __LINE__: Trice64( iD(6566), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: msg:value=-1, -2
        break; case __LINE__: trice64( iD(1662), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: msg:value=-1, -2, -3
        break; case __LINE__: TRice64( iD(4894), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice64( iD(5093), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice64( iD(1631), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice64( iD(3870), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice64( iD(7376), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice64( iD(6457), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice64( iD(5241), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice64( iD(7891), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice64( iD(1334), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice8_1 ( iD(5696), "msg:value=%d\n", -1 );                                                                                            //exp: time: 842,150_450default: msg:value=-1
        break; case __LINE__: Trice8_2 ( iD(3514), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: msg:value=-1, -2
        break; case __LINE__: trice8_3 ( iD(5256), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: msg:value=-1, -2, -3
        break; case __LINE__: TRice8_4 ( iD(6835), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice8_5 ( iD(1871), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice8_6 ( iD(3862), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice8_7 ( iD(5291), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice8_8 ( iD(2111), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice8_9 ( iD(3651), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice8_10( iD(6148), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice8_11( iD(4304), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice8_12( iD(6514), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice16_1 ( iD(5175), "msg:value=%d\n", -1 );                                                                                            //exp: time: 842,150_450default: msg:value=-1
        break; case __LINE__: Trice16_2 ( iD(7292), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: msg:value=-1, -2
        break; case __LINE__: trice16_3 ( iD(7860), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: msg:value=-1, -2, -3
        break; case __LINE__: TRice16_4 ( iD(4734), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice16_5 ( iD(2988), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice16_6 ( iD(2704), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice16_7 ( iD(2803), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice16_8 ( iD(2292), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice16_9 ( iD(2474), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice16_10( iD(4908), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice16_11( iD(3095), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice16_12( iD(5815), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice32_1 ( iD(6367), "msg:value=%d\n", -1 );                                                                                            //exp: time: 842,150_450default: msg:value=-1
        break; case __LINE__: Trice32_2 ( iD(4323), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: msg:value=-1, -2
        break; case __LINE__: trice32_3 ( iD(2311), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: msg:value=-1, -2, -3
        break; case __LINE__: TRice32_4 ( iD(4820), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice32_5 ( iD(1717), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice32_6 ( iD(2116), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice32_7 ( iD(7301), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice32_8 ( iD(3486), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice32_9 ( iD(7681), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice32_10( iD(4115), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice32_11( iD(3126), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice32_12( iD(4460), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice64_1 ( iD(5644), "msg:value=%d\n", -1 );                                                                                            //exp: time: 842,150_450default: msg:value=-1
        break; case __LINE__: Trice64_2 ( iD(5089), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: msg:value=-1, -2
        break; case __LINE__: trice64_3 ( iD(3943), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: msg:value=-1, -2, -3
        break; case __LINE__: TRice64_4 ( iD(5687), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice64_5 ( iD(6077), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice64_6 ( iD(4111), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice64_7 ( iD(4421), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice64_8 ( iD(2454), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice64_9 ( iD(6924), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice64_10( iD(4087), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice64_11( iD(1031), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice64_12( iD(6063), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        // use cases for 8-bit data
        break; case __LINE__: trice8     ( iD(2476), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: Trice8     ( iD(5572), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRice8     ( iD(1818), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: trice8_1   ( iD(6928), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: Trice8_1   ( iD(6978), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRice8_1   ( iD(7394), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2

        // use cases for 16-bit data
        break; case __LINE__: trice16    ( iD(5831), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: Trice16    ( iD(1931), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRice16    ( iD(2199), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: trice16_1  ( iD(2900), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: Trice16_1  ( iD(6885), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRice16_1  ( iD(4810), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2

        // use cases for 32-bit data
        break; case __LINE__: trice32    ( iD(5661), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: Trice32    ( iD(2099), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRice32    ( iD(3326), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: trice32_1  ( iD(7330), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: Trice32_1  ( iD(2410), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRice32_1  ( iD(3211), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2

        // use cases for 64-bit data
        break; case __LINE__: trice64    ( iD(2182), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: Trice64    ( iD(6230), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRice64    ( iD(7266), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: trice64_1  ( iD(6105), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: Trice64_1  ( iD(1100), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRice64_1  ( iD(7030), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2

        // legacy use cases
        break; case __LINE__: TRICE      ( ID(6028), "value=%d\n", -2 ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: TRICE      ( id(5804), "value=%d\n", -2 ); //exp: time:            default: value=-2
        break; case __LINE__: TRICE      ( Id(3920), "value=%d\n", -2 ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRICE      ( ID(3019), "value=%d\n", -2 ); //exp: time: 842,150_450default: value=-2

        // default stamp
        break; case __LINE__: TRICE8     ( ID(6622), "value=%d\n", -2 ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: TRICE8_1   ( ID(1416), "value=%d\n", -2 ); //exp: time: 842,150_450default: value=-2

        // no stamp
        break; case __LINE__: trice8     ( iD(4267), "value=%d\n", -2 ); //exp: time:            default: value=-2
        break; case __LINE__: trice8_1   ( iD(1938), "value=%d\n", -2 ); //exp: time:            default: value=-2
        break; case __LINE__: TRICE8     ( id(2979), "value=%d\n", -2 ); //exp: time:            default: value=-2
        break; case __LINE__: TRICE8_1   ( id(1354), "value=%d\n", -2 ); //exp: time:            default: value=-2

        // 16-bit stamp:
        break; case __LINE__: Trice8     ( iD(3057), "value=%d\n", -2 ); //exp: time:       5_654default: value=-2
        break; case __LINE__: Trice8_1   ( iD(2584), "value=%d\n", -2 ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRICE8     ( Id(4453), "value=%d\n", -2 ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRICE8_1   ( Id(5401), "value=%d\n", -2 ); //exp: time:       5_654default: value=-2

        // 32-bit stamp:
        break; case __LINE__: TRice8     ( iD(2400), "value=%d\n", -2 ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: TRice8_1   ( iD(7551), "value=%d\n", -2 ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: TRICE8     ( ID(2851), "value=%d\n", -2 ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: TRICE8_1   ( ID(7006), "value=%d\n", -2 ); //exp: time: 842,150_450default: value=-2

        break; case __LINE__: TRICE( ID(2109), "FATAL:magenta+b:red\n" );           //exp: time: 842,150_450default: FATAL:magenta+b:red
        break; case __LINE__: TRICE( ID(7597), "CRITICAL:red+i:default+h\n" );      //exp: time: 842,150_450default: CRITICAL:red+i:default+h
        break; case __LINE__: TRICE( ID(5262), "EMERGENCY:red+i:blue\n" );          //exp: time: 842,150_450default: EMERGENCY:red+i:blue
        break; case __LINE__: TRICE( ID(7297), "ERROR:11:red\n" );                  //exp: time: 842,150_450default: ERROR:11:red
        break; case __LINE__: TRICE( ID(1921), "WARNING:11+i:red\n" );              //exp: time: 842,150_450default: WARNING:11+i:red
        break; case __LINE__: TRICE( ID(3737), "ATTENTION:11:green\n" );            //exp: time: 842,150_450default: ATTENTION:11:green
        break; case __LINE__: TRICE( ID(7158), "INFO:cyan+b:default+h\n" );         //exp: time: 842,150_450default: INFO:cyan+b:default+h
        break; case __LINE__: TRICE( ID(7434), "DEBUG:130+i\n" );                   //exp: time: 842,150_450default: DEBUG:130+i
        break; case __LINE__: TRICE( ID(1353), "TRACE:default+i:default+h\n" );     //exp: time: 842,150_450default: TRACE:default+i:default+h
        break; case __LINE__: TRICE( ID(3918), "TIME:blue+i:blue+h\n" );            //exp: time: 842,150_450default: TIME:blue+i:blue+h
        break; case __LINE__: TRICE( ID(6192), "MESSAGE:green+h:black\n" );         //exp: time: 842,150_450default: MESSAGE:green+h:black
        break; case __LINE__: TRICE( ID(2479), "READ:black+i:yellow+h\n" );         //exp: time: 842,150_450default: READ:black+i:yellow+h
        break; case __LINE__: TRICE( ID(6029), "WRITE:black+u:yellow+h\n" );        //exp: time: 842,150_450default: WRITE:black+u:yellow+h
        break; case __LINE__: TRICE( ID(1141), "RECEIVE:black+h:black\n" );         //exp: time: 842,150_450default: RECEIVE:black+h:black
        break; case __LINE__: TRICE( ID(3572), "TRANSMIT:black:black+h\n" );        //exp: time: 842,150_450default: TRANSMIT:black:black+h
        break; case __LINE__: TRICE( ID(6513), "DIAG:yellow+i:default+h\n" );       //exp: time: 842,150_450default: DIAG:yellow+i:default+h
        break; case __LINE__: TRICE( ID(4210), "INTERRUPT:magenta+i:default+h\n" ); //exp: time: 842,150_450default: INTERRUPT:magenta+i:default+h
        break; case __LINE__: TRICE( ID(2921), "SIGNAL:118+i\n" );                  //exp: time: 842,150_450default: SIGNAL:118+i
        break; case __LINE__: TRICE( ID(6887), "TEST:yellow+h:black\n" );           //exp: time: 842,150_450default: TEST:yellow+h:black
        break; case __LINE__: TRICE( ID(2742), "DEFAULT:off\n" );                   //exp: time: 842,150_450default: DEFAULT:off
        break; case __LINE__: TRICE( ID(5694), "NOTICE:blue:white+h\n" );           //exp: time: 842,150_450default: NOTICE:blue:white+h
        break; case __LINE__: TRICE( ID(1627), "ALERT:magenta:magenta+h\n" );       //exp: time: 842,150_450default: ALERT:magenta:magenta+h
        break; case __LINE__: TRICE( ID(4621), "ASSERT:yellow+i:blue\n" );          //exp: time: 842,150_450default: ASSERT:yellow+i:blue
        break; case __LINE__: TRICE( ID(3908), "ALARM:red+i:white+h\n" );           //exp: time: 842,150_450default: ALARM:red+i:white+h
        break; case __LINE__: TRICE( ID(1671), "CYCLE:blue+i:default+h\n" );        //exp: time: 842,150_450default: CYCLE:blue+i:default+h
        break; case __LINE__: TRICE( ID(5182), "VERBOSE:blue:default\n" );          //exp: time: 842,150_450default: VERBOSE:blue:default
        break; case __LINE__: TRICE( ID(6586), "fatal:magenta+b:red\n" );           //exp: time: 842,150_450default: fatal:magenta+b:red
        break; case __LINE__: TRICE( ID(1395), "critical:red+i:default+h\n" );      //exp: time: 842,150_450default: critical:red+i:default+h
        break; case __LINE__: TRICE( ID(5740), "emergency:red+i:blue\n" );          //exp: time: 842,150_450default: emergency:red+i:blue
        break; case __LINE__: TRICE( ID(6970), "error:11:red\n" );                  //exp: time: 842,150_450default: error:11:red
        break; case __LINE__: TRICE( ID(1429), "warning:11+i:red\n" );              //exp: time: 842,150_450default: warning:11+i:red
        break; case __LINE__: TRICE( ID(4800), "attention:11:green\n" );            //exp: time: 842,150_450default: attention:11:green
        break; case __LINE__: TRICE( ID(7298), "info:cyan+b:default+h\n" );         //exp: time: 842,150_450default: info:cyan+b:default+h
        break; case __LINE__: TRICE( ID(4538), "debug:130+i\n" );                   //exp: time: 842,150_450default: debug:130+i
        break; case __LINE__: TRICE( ID(5971), "trace:default+i:default+h\n" );     //exp: time: 842,150_450default: trace:default+i:default+h
        break; case __LINE__: TRICE( ID(4358), "time:blue+i:blue+h\n" );            //exp: time: 842,150_450default: time:blue+i:blue+h
        break; case __LINE__: TRICE( ID(7413), "message:green+h:black\n" );         //exp: time: 842,150_450default: message:green+h:black
        break; case __LINE__: TRICE( ID(6006), "read:black+i:yellow+h\n" );         //exp: time: 842,150_450default: read:black+i:yellow+h
        break; case __LINE__: TRICE( ID(7444), "write:black+u:yellow+h\n" );        //exp: time: 842,150_450default: write:black+u:yellow+h
        break; case __LINE__: TRICE( ID(1719), "receive:black+h:black\n" );         //exp: time: 842,150_450default: receive:black+h:black
        break; case __LINE__: TRICE( ID(3527), "transmit:black:black+h\n" );        //exp: time: 842,150_450default: transmit:black:black+h
        break; case __LINE__: TRICE( ID(7918), "diag:yellow+i:default+h\n" );       //exp: time: 842,150_450default: diag:yellow+i:default+h
        break; case __LINE__: TRICE( ID(5086), "interrupt:magenta+i:default+h\n" ); //exp: time: 842,150_450default: interrupt:magenta+i:default+h
        break; case __LINE__: TRICE( ID(2495), "signal:118+i\n" );                  //exp: time: 842,150_450default: signal:118+i
        break; case __LINE__: TRICE( ID(7291), "test:yellow+h:black\n" );           //exp: time: 842,150_450default: test:yellow+h:black
        break; case __LINE__: TRICE( ID(7575), "default:off\n" );                   //exp: time: 842,150_450default: default:off
        break; case __LINE__: TRICE( ID(2871), "notice:blue:white+h\n" );           //exp: time: 842,150_450default: notice:blue:white+h
        break; case __LINE__: TRICE( ID(2995), "alert:magenta:magenta+h\n" );       //exp: time: 842,150_450default: alert:magenta:magenta+h
        break; case __LINE__: TRICE( ID(3284), "assert:yellow+i:blue\n" );          //exp: time: 842,150_450default: assert:yellow+i:blue
        break; case __LINE__: TRICE( ID(6516), "alarm:red+i:white+h\n" );           //exp: time: 842,150_450default: alarm:red+i:white+h
        break; case __LINE__: TRICE( ID(2181), "cycle:blue+i:default+h\n" );        //exp: time: 842,150_450default: cycle:blue+i:default+h
        break; case __LINE__: TRICE( ID(1458), "verbose:blue:default\n" );          //exp: time: 842,150_450default: verbose:blue:default
        break; case __LINE__: TRICE64( id(5706), "msg:%d (%%d)\n", -1 );     //exp: time:            default: msg:-1 (%d)

        break; case __LINE__: TRICE( ID(2837), "sig:TRICE with 1 to 12 values\n" );
        break; case __LINE__: TRICE( ID(3295), "rd:TRICE %d\n", -1 );
        break; case __LINE__: TRICE( ID(4240), "rd:TRICE %d, %d\n", -1, -2 );
        break; case __LINE__: TRICE( ID(7533), "rd:TRICE %d, %d, %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE( ID(1847), "rd:TRICE %d, %d, %d, %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE( ID(7560), "rd:TRICE %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE( ID(4488), "rd:TRICE %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE( ID(2958), "rd:TRICE %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE( ID(1725), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE( ID(5428), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE( ID(1116), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE( ID(7677), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE( ID(6272), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE( ID(7800), "sig:TRICE_n with 1 to 12 values\n" );
        break; case __LINE__: TRICE_1( ID(1831), "rd:TRICE_1 %d\n", 1 );
        break; case __LINE__: TRICE_2( ID(1110), "rd:TRICE_2 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE_3( ID(4988), "rd:TRICE_3 %d, %d, %d\n", 1, 2, 3 );
        break; case __LINE__: TRICE_4( ID(2611), "rd:TRICE_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
        break; case __LINE__: TRICE_5( ID(3085), "rd:TRICE_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
        break; case __LINE__: TRICE_6( ID(7420), "rd:TRICE_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
        break; case __LINE__: TRICE_7( ID(5419), "rd:TRICE_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
        break; case __LINE__: TRICE_8( ID(5164), "rd:TRICE_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
        break; case __LINE__: TRICE_9( ID(7725), "rd:TRICE_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
        break; case __LINE__: TRICE_10( ID(3004), "rd:TRICE_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
        break; case __LINE__: TRICE_11( ID(5274), "rd:TRICE_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
        break; case __LINE__: TRICE_12( ID(4607), "rd:TRICE_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );				

        break; case __LINE__: TRICE( ID(6929), "sig:TRICE8 with 1 to 12 values\n" );
        break; case __LINE__: TRICE8( ID(5009), "rd:TRICE8 %d\n", -1 );
        break; case __LINE__: TRICE8( ID(5664), "rd:TRICE8 %d, %d\n", -1, -2 );
        break; case __LINE__: TRICE8( ID(7754), "rd:TRICE8 %d, %d, %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE8( ID(5414), "rd:TRICE8 %d, %d, %d, %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE8( ID(1760), "rd:TRICE8 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE8( ID(6632), "rd:TRICE8 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE8( ID(7884), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE8( ID(1710), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE8( ID(4527), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE8( ID(7833), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE8( ID(3073), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE8( ID(4309), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE( ID(4463), "sig:TRICE8_n with 1 to 12 values\n" );
        break; case __LINE__: TRICE8_1 ( ID(7493), "rd:TRICE8_1 %d\n", 1 );
        break; case __LINE__: TRICE8_2 ( ID(7952), "rd:TRICE8_2 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE8_3 ( ID(7636), "rd:TRICE8_3 %d, %d, %d\n", 1, 2, 3 );
        break; case __LINE__: TRICE8_4 ( ID(4320), "rd:TRICE8_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
        break; case __LINE__: TRICE8_5 ( ID(7957), "rd:TRICE8_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
        break; case __LINE__: TRICE8_6 ( ID(2153), "rd:TRICE8_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
        break; case __LINE__: TRICE8_7 ( ID(2583), "rd:TRICE8_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
        break; case __LINE__: TRICE8_8 ( ID(2066), "rd:TRICE8_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
        break; case __LINE__: TRICE8_9 ( ID(6624), "rd:TRICE8_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
        break; case __LINE__: TRICE8_10( ID(5707), "rd:TRICE8_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
        break; case __LINE__: TRICE8_11( ID(1393), "rd:TRICE8_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
        break; case __LINE__: TRICE8_12( ID(7978), "rd:TRICE8_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );

        break; case __LINE__: TRICE( ID(1400), "sig:TRICE16 with 1 to 12 values\n" );
        break; case __LINE__: TRICE16( ID(3529), "rd:TRICE16 %d\n", -1 );
        break; case __LINE__: TRICE16( ID(7128), "rd:TRICE16 %d, %d\n", -1, -2 );
        break; case __LINE__: TRICE16( ID(5171), "rd:TRICE16 %d, %d, %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE16( ID(6892), "rd:TRICE16 %d, %d, %d, %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE16( ID(6581), "rd:TRICE16 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE16( ID(3760), "rd:TRICE16 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE16( ID(1365), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE16( ID(2060), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE16( ID(5509), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE16( ID(3716), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE16( ID(1540), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE16( ID(6743), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE     ( ID(7801), "sig:TRICE16_n with 1 to 12 values\n" );
        break; case __LINE__: TRICE16_1 ( ID(2897), "rd:TRICE16_1 %d\n", 1 );
        break; case __LINE__: TRICE16_2 ( ID(1097), "rd:TRICE16_2 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE16_3 ( ID(5982), "rd:TRICE16_3 %d, %d, %d\n", 1, 2, 3 );
        break; case __LINE__: TRICE16_4 ( ID(6562), "rd:TRICE16_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
        break; case __LINE__: TRICE16_5 ( ID(4452), "rd:TRICE16_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
        break; case __LINE__: TRICE16_6 ( ID(6037), "rd:TRICE16_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
        break; case __LINE__: TRICE16_7 ( ID(7600), "rd:TRICE16_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
        break; case __LINE__: TRICE16_8 ( ID(2854), "rd:TRICE16_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
        break; case __LINE__: TRICE16_9 ( ID(7761), "rd:TRICE16_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
        break; case __LINE__: TRICE16_10( ID(6486), "rd:TRICE16_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
        break; case __LINE__: TRICE16_11( ID(1562), "rd:TRICE16_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
        break; case __LINE__: TRICE16_12( ID(2131), "rd:TRICE16_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );

        break; case __LINE__: TRICE  ( ID(3291), "sig:TRICE16 with 1 to 12 pointer\n" );
        break; case __LINE__: TRICE16( ID(6371), "rd:TRICE16 %p\n", -1 );
        break; case __LINE__: TRICE16( ID(5469), "rd:TRICE16 %p, %p\n", -1, -2 );
        break; case __LINE__: TRICE16( ID(2938), "rd:TRICE16 %p, %p, %p\n", -1, -2, -3 );
        break; case __LINE__: TRICE16( ID(2524), "rd:TRICE16 %p, %p, %p, %p\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE16( ID(1910), "rd:TRICE16 %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE16( ID(2481), "rd:TRICE16 %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE16( ID(3485), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE16( ID(5156), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE16( ID(7631), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE16( ID(4540), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE16( ID(2970), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE16( ID(2463), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE  ( ID(7770), "sig:TRICE16 with 1 to 12 hex\n" );
        break; case __LINE__: TRICE16( ID(7512), "rd:TRICE16 %X\n", -1 );
        break; case __LINE__: TRICE16( ID(2057), "rd:TRICE16 %X, %X\n", -1, -2 );
        break; case __LINE__: TRICE16( ID(7649), "rd:TRICE16 %X, %X, %X\n", -1, -2, -3 );
        break; case __LINE__: TRICE16( ID(6382), "rd:TRICE16 %X, %X, %X, %X\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE16( ID(1955), "rd:TRICE16 %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE16( ID(6429), "rd:TRICE16 %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE16( ID(7752), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE16( ID(5356), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE16( ID(1620), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE16( ID(5761), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE16( ID(3872), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE16( ID(1084), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE  ( ID(1780), "sig:TRICE32 with 1 to 12 values\n" );
        break; case __LINE__: TRICE32( ID(2849), "rd:TRICE32 %d\n", -1 );
        break; case __LINE__: TRICE32( ID(7478), "rd:TRICE32 %d, %d\n", -1, -2 );
        break; case __LINE__: TRICE32( ID(7817), "rd:TRICE32 %d, %d, %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE32( ID(4604), "rd:TRICE32 %d, %d, %d, %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE32( ID(1004), "rd:TRICE32 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE32( ID(4661), "rd:TRICE32 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE32( ID(6758), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE32( ID(2166), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE32( ID(5699), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE32( ID(7430), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE32( ID(2059), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE32( ID(5193), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE     ( ID(6141), "signal:TRICE32_n with 1 to 12 values\n" );
        break; case __LINE__: TRICE32_1 ( ID(2972), "rd:TRICE32_1 %d\n", 1 );
        break; case __LINE__: TRICE32_2 ( ID(6786), "rd:TRICE32_2 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE32_3 ( ID(6144), "rd:TRICE32_3 %d, %d, %d\n", 1, 2, 3 );
        break; case __LINE__: TRICE32_4 ( ID(4982), "rd:TRICE32_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
        break; case __LINE__: TRICE32_5 ( ID(6014), "rd:TRICE32_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
        break; case __LINE__: TRICE32_6 ( ID(2308), "rd:TRICE32_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
        break; case __LINE__: TRICE32_7 ( ID(2544), "rd:TRICE32_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
        break; case __LINE__: TRICE32_8 ( ID(3444), "rd:TRICE32_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
        break; case __LINE__: TRICE32_9 ( ID(4327), "rd:TRICE32_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
        break; case __LINE__: TRICE32_10( ID(6811), "rd:TRICE32_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
        break; case __LINE__: TRICE32_11( ID(6245), "rd:TRICE32_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
        break; case __LINE__: TRICE32_12( ID(3929), "rd:TRICE32_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );

        break; case __LINE__: TRICE  ( ID(7523), "sig:TRICE64 with 1 to 12 values\n" );
        break; case __LINE__: TRICE64( ID(2797), "rd:TRICE64 %d\n", -1 );
        break; case __LINE__: TRICE64( ID(7525), "rd:TRICE64 %d, %d\n", -1, -2 );
        break; case __LINE__: TRICE64( ID(3577), "rd:TRICE64 %d, %d, %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE64( ID(1279), "rd:TRICE64 %d, %d, %d, %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE64( ID(7265), "rd:TRICE64 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE64( ID(4585), "rd:TRICE64 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE64( ID(2196), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE64( ID(2460), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE64( ID(6613), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE64( ID(2228), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE64( ID(5305), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE64( ID(1366), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE     ( ID(3978), "sig:TRICE64_n with 1 to 12 values\n" );
        break; case __LINE__: TRICE64_1 ( ID(1918), "rd:TRICE64_1 %d\n", 1 );
        break; case __LINE__: TRICE64_2 ( ID(2722), "rd:TRICE64_2 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE64_2 ( ID(3007), "rd:TRICE64_2 %x, %x\n", 0x1122334455667788, 0x0102030405060708 );
        break; case __LINE__: TRICE64_3 ( ID(1224), "rd:TRICE64_3 %d, %d, %d\n", 1, 2, 3 );
        break; case __LINE__: TRICE64_4 ( ID(3176), "rd:TRICE64_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
        break; case __LINE__: TRICE64_5 ( ID(6083), "rd:TRICE64_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
        break; case __LINE__: TRICE64_6 ( ID(6830), "rd:TRICE64_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
        break; case __LINE__: TRICE64_7 ( ID(4242), "rd:TRICE64_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
        break; case __LINE__: TRICE64_8 ( ID(3306), "rd:TRICE64_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
        break; case __LINE__: TRICE64_9 ( ID(1820), "rd:TRICE64_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
        break; case __LINE__: TRICE64_10( ID(5067), "rd:TRICE64_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
        break; case __LINE__: TRICE64_11( ID(7014), "rd:TRICE64_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
        break; case __LINE__: TRICE64_12( ID(4679), "rd:TRICE64_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );

        break; case __LINE__: TRICE( ID(5625), "sig:Colors and ticks\n" );
        break; case __LINE__: TRICE( ID(6044), "--------------------------------------------------\n");
        break; case __LINE__: TRICE( ID(2367), "--------------------------------------------------\n");
        break; case __LINE__: TRICE( ID(5035), "dbg:12345 as 16bit is %016b\n", 12345);
        break; case __LINE__: TRICE( ID(7788), "--------------------------------------------------\n");
        break; case __LINE__: TRICE( ID(6201), "sig:This ASSERT error is just a demo and no real error:\n");
        break; case __LINE__: TRICE( ID(6328), "--------------------------------------------------\n");
        break; case __LINE__: TRICE( ID(1829), "ERR:error       message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID(5658), "WRN:warning     message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID(4624), "ATT:attention   message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID(4566), "DIA:diagnostics message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID(7426), "TIM:timing      message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID(4332), "DBG:debug       message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID(6162), "SIG:signal      message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID(3898), "RD:read         message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID(7651), "WR:write        message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID(7934), "ISR:interrupt   message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID(7186), "MSG:normal      message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID(1297), "INFO:informal   message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID(1994), "time:TRICE32_1  message, SysTick is %6u\n", SYSTICKVAL);

        break; case __LINE__: TRICE( ID(1543), "sig:Various ranges and formats\n" );
        break; case __LINE__: TRICE8 ( ID(1973), "tst:TRICE8  %%03x -> %03x  %03x  %03x  %03x\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID(3461), "tst:TRICE8   %%4d -> %4d %4d %4d %4d\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID(7729), "tst:TRICE8   %%4u -> %4u %4u %4u %4u\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID(1024), "tst:TRICE8   %%4o -> %4o %4o %4o %4o\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID(3520), "tst:TRICE8   %%4O -> %4O %4O %4O %4O\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID(7406), "tst:TRICE8   %%4X -> %4X %4X %4X %4X\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID(3014), "tst:TRICE8   %%4x -> %4x %4x %4x %4x\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID(2595), "tst:TRICE8   %%8b -> %8b %8b %8b %8b\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID(4629), "tst:TRICE8  %%08b -> %08b %08b %08b %08b\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE16( ID(7945), "tst:TRICE16  %%05x ->   %05x   %05x   %05x   %05x\n", 1, 0x7fff, 0x8000, 0xffff);
        break; case __LINE__: TRICE16( ID(7685), "tst:TRICE16   %%6d ->  %6d  %6d  %6d  %6d\n",         1, 0x7fff, 0x8000, 0xffff);
        break; case __LINE__: TRICE16( ID(2687), "tst:TRICE16   %%7o -> %7o %7o %7o %7o\n",             1, 0x7fff, 0x8000, 0xffff);
        break; case __LINE__: TRICE32( ID(5785), "tst:TRICE32 %%09x ->      %09x      %09x       %09x     %09x\n", 1, 0x7fffffff, 0x80000000, 0xffffffff);
        break; case __LINE__: TRICE32( ID(4290), "tst:TRICE32 %%10d ->     %10d     %10d     %10d    %10x\n",      1, 0x7fffffff, 0x80000000, 0xffffffff);
        break; case __LINE__: TRICE32( ID(1124), "att:TRICE32 %40b\n", 0xAAAAAAAA);
        break; case __LINE__: TRICE64( ID(2457), "tst:TRICE64 %%09x ->      %09x      %09x       %09x     %09x\n", 1, 0x7fffffffffffffff, 0x8000000000000000, 0xffffffffffffffff);
        break; case __LINE__: TRICE64( ID(3661), "tst:TRICE64 %%10d ->     %10d     %10d     %10d    %10x\n",      1, 0x7fffffffffffffff, 0x8000000000000000, 0xffffffffffffffff);
        break; case __LINE__: TRICE64( ID(3518), "att:TRICE64 %70b\n", 0xAAAAAAAAAAAAAAAA);
        break; case __LINE__: TRICE( ID(2840), "sig:Legacy TRICE8\n" );
        break; case __LINE__: TRICE8_1( ID(6370), "tst:TRICE8_1 %02x\n", 0xA1);
        break; case __LINE__: TRICE8_2( ID(2137), "tst:TRICE8_2 %02x %02x\n", 0xA1, 0xA2);
        break; case __LINE__: TRICE8_3( ID(2564), "tst:TRICE8_3 %02x %02x %02x\n", 0xA1, 0xA2, 0xA3);
        break; case __LINE__: TRICE8_4( ID(7329), "tst:TRICE8_4 %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4);
        break; case __LINE__: TRICE8_5( ID(5599), "tst:TRICE8_5 %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5 );
        break; case __LINE__: TRICE8_6( ID(6398), "tst:TRICE8_6 %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6);
        break; case __LINE__: TRICE8_7( ID(2093), "tst:TRICE8_7 %02x %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7);
        break; case __LINE__: TRICE8_8( ID(7900), "tst:TRICE8_8 %02x %02x %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8);
        break; case __LINE__: TRICE8_1( ID(6693), "tst:TRICE8_1 %d\n",                      -1);
        break; case __LINE__: TRICE8_2( ID(3044), "tst:TRICE8_2 %d %d\n",                   -1, -2);
        break; case __LINE__: TRICE8_3( ID(3041), "tst:TRICE8_3 %d %d %d\n",                -1, -2, -3);
        break; case __LINE__: TRICE8_4( ID(6295), "tst:TRICE8_4 %d %d %d %d\n",             -1, -2, -3, -4);
        break; case __LINE__: TRICE8_5( ID(2677), "tst:TRICE8_5 %d %d %d %d %d\n",          -1, -2, -3, -4, -5);
        break; case __LINE__: TRICE8_6( ID(3027), "tst:TRICE8_6 %d %d %d %d %d %d\n",       -1, -2, -3, -4, -5, -6);
        break; case __LINE__: TRICE8_7( ID(2994), "tst:TRICE8_7 %d %d %d %d %d %d %d\n",    -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE8_8( ID(3899), "tst:TRICE8_8 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8);
        break; case __LINE__: TRICE8_1( ID(7315), "tst:TRICE8_1 %d\n", 1);
        break; case __LINE__: TRICE8_2( ID(5395), "tst:TRICE8_2 %d %d\n", 1, 2);
        break; case __LINE__: TRICE8_3( ID(7662), "tst:TRICE8_3 %d %d %d\n", 1, 2, 3);
        break; case __LINE__: TRICE8_4( ID(2630), "tst:TRICE8_4 %d %d %d %d\n", 1, 2, 3, 4);
        break; case __LINE__: TRICE8_5( ID(5562), "tst:TRICE8_5 %d %d %d %d %d\n", 1, 2, 3, 4, 5);
        break; case __LINE__: TRICE8_6( ID(2294), "tst:TRICE8_6 %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6);
        break; case __LINE__: TRICE8_7( ID(4437), "tst:TRICE8_7 %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7);
        break; case __LINE__: TRICE8_8( ID(5703), "tst:TRICE8_8 %d %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7, 8);
        break; case __LINE__: TRICE8_1( ID(2424), "tst:TRICE8_1 %u\n", 201);
        break; case __LINE__: TRICE8_2( ID(2098), "tst:TRICE8_2 %u %u\n", 201, 202);
        break; case __LINE__: TRICE8_3( ID(6312), "tst:TRICE8_3 %u %u %u\n", 201, 202, 203);
        break; case __LINE__: TRICE8_4( ID(2648), "tst:TRICE8_4 %u %u %u %u\n", 201, 202, 203, 204);
        break; case __LINE__: TRICE8_5( ID(1587), "tst:TRICE8_5 %u %u %u %u %u\n", 201, 202, 203, 204, 205);
        break; case __LINE__: TRICE8_6( ID(7571), "tst:TRICE8_6 %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206);
        break; case __LINE__: TRICE8_7( ID(6890), "tst:TRICE8_7 %u %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206, 207);
        break; case __LINE__: TRICE8_8( ID(2015), "tst:TRICE8_8 %u %u %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206, 207, 208);
        break; case __LINE__: TRICE8_8( ID(7063), "tst:TRICE8_1 %%d=%d, %%u=%u, 0x%%x=0x%x, 0x%%2x=0x%2x, 0x%%02x=0x%02x, 0x%%3x=0x%3x, 0x%%03x=0x%03x, %%b=%b\n", 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81);

        break; case __LINE__: TRICE    ( ID(4062), "sig:Legacy TRICE16-64\n" );
        break; case __LINE__: TRICE16_1( ID(6590), "tst:TRICE16_1 %d\n", -111);
        break; case __LINE__: TRICE16_2( ID(3299), "tst:TRICE16_2 %d %d\n", -111, -222);
        break; case __LINE__: TRICE16_3( ID(7220), "tst:TRICE16_3 %d %d %d\n", -111, -222, -333);
        break; case __LINE__: TRICE16_4( ID(5727), "tst:TRICE16_4 %d %d %d %d\n", -111, -222, -333, -444);
        break; case __LINE__: TRICE32_1( ID(6211), "tst:TRICE32_1 %08x\n", 0x0123cafe);
        break; case __LINE__: TRICE32_1( ID(1220), "tst:TRICE32_1 %d\n", -111);
        break; case __LINE__: TRICE32_2( ID(4169), "tst:TRICE32_2 %x %x\n", -111, -222);
        break; case __LINE__: TRICE32_2( ID(5101), "tst:TRICE32_2 %d %d\n", -111, -222);
        break; case __LINE__: TRICE32_3( ID(5458), "tst:TRICE32_3 %x %x %x\n", -111, -222, -333);
        break; case __LINE__: TRICE32_3( ID(6931), "tst:TRICE32_3 %d %d %d\n", -111, -222, -333);
        break; case __LINE__: TRICE32_4( ID(5296), "tst:TRICE32_4 %x %x %x %x\n", -111, -222, -333, -444);
        break; case __LINE__: TRICE32_4( ID(7341), "tst:TRICE32_4 %d %d %d %d\n", -111, -222, -333, -444);
        break; case __LINE__: TRICE64_1( ID(3790), "tst:TRICE64_1 %d\n", -111);
        break; case __LINE__: TRICE64_2( ID(4794), "tst:TRICE64_2 %d %d\n", -111, -222);
        break; case __LINE__: TRICE16_1( ID(5581), "tst:TRICE16_1 %u\n", 60001);
        break; case __LINE__: TRICE16_2( ID(5776), "tst:TRICE16_2 %u %u\n", 60001, 60002);
        break; case __LINE__: TRICE16_3( ID(2695), "tst:TRICE16_3 %u %u %u\n", 60001, 60002, 60003);
        break; case __LINE__: TRICE16_4( ID(2602), "tst:TRICE16_4 %u %u %u %u\n", 60001, 60002, 60003, 60004);
        break; case __LINE__: TRICE32_1( ID(1046), "tst:TRICE32_1 %u\n", 4000000001);
        break; case __LINE__: TRICE32_2( ID(2652), "tst:TRICE32_2 %u %u\n", 4000000001, 4000000002);
        break; case __LINE__: TRICE32_3( ID(4140), "tst:TRICE32_3 %u %u %u\n", 4000000001, 4000000002, 4000000003);
        break; case __LINE__: TRICE32_4( ID(4524), "tst:TRICE32_4 %u %u %u %u\n", 4000000001, 4000000002, 4000000003, 4000000004);
        break; case __LINE__: TRICE64_1( ID(3260), "tst:TRICE64_1 %x\n", -1);
        break; case __LINE__: TRICE64_2( ID(4710), "tst:TRICE64_2 %x %x\n", -1, -2);
        break; case __LINE__: TRICE64_1( ID(1528), "tst:TRICE64_1 %u\n", -1);
        break; case __LINE__: TRICE64_2( ID(6091), "tst:TRICE64_2 %u %u\n", -1, -2);

        break; case __LINE__: TRICE( ID(7699), "sig:colored single letters and several TRICE macros in one line\n" );
        break; case __LINE__: TRICE( ID(4716), "e:A");
        break; case __LINE__: TRICE( ID(3428), "w:B");
        break; case __LINE__: TRICE( ID(2016), "a:c");
        break; case __LINE__: TRICE( ID(3617), "wr:d");
        break; case __LINE__: TRICE( ID(2781), "rd:e\n");
        break; case __LINE__: TRICE( ID(5990), "diag:f");
        break; case __LINE__: TRICE( ID(4410), "d:G");
        break; case __LINE__: TRICE( ID(4622), "t:H");
        break; case __LINE__: TRICE( ID(5048), "time:i");
        break; case __LINE__: TRICE( ID(6767), "message:J");
        break; case __LINE__: TRICE( ID(2475), "dbg:k\n");
        break; case __LINE__: TRICE( ID(3391), "1");
        break; case __LINE__: TRICE( ID(1694), "2");
        break; case __LINE__: TRICE( ID(1971), "3");
        break; case __LINE__: TRICE( ID(6221), "4");
        break; case __LINE__: TRICE( ID(5023), "e:7");
        break; case __LINE__: TRICE( ID(6275), "m:12");
        break; case __LINE__: TRICE( ID(1029), "m:123\n");
        break; case __LINE__: TRICE( ID(6507), "e:A"); TRICE( ID(2878), "w:B"); TRICE( ID(1501), "a:c");
        break; case __LINE__: TRICE( ID(1063), "wr:d"); TRICE( ID(1861), "rd:e\n"); TRICE( ID(5328), "diag:f");

        break; case __LINE__: TRICE( ID(4768), "sig:TRICE8 with variable param count 1 to 12\n" );
        break; case __LINE__: TRICE8( ID(1156), "tst:TRICE8 %d\n", -1 );
        break; case __LINE__: TRICE8( ID(7383), "tst:TRICE8 %d %d\n", -1, -2 );
        break; case __LINE__: TRICE8( ID(6092), "tst:TRICE8 %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE8( ID(1566), "tst:TRICE8 %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE8( ID(1876), "tst:TRICE8 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE8( ID(7587), "tst:TRICE8 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE8( ID(2634), "tst:TRICE8 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE8( ID(7953), "tst:TRICE8 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE8( ID(3321), "tst:TRICE8 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE8( ID(6831), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE8( ID(4733), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE8( ID(5064), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE8( ID(5271), "tst:TRICE8 %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );
        break; case __LINE__: TRICE8_1 ( ID(5317), "tst:TRICE8_1  %d\n", -1 );
        break; case __LINE__: TRICE8_2 ( ID(6160), "tst:TRICE8_2  %d %d\n", -1, -2 );
        break; case __LINE__: TRICE8_3 ( ID(5681), "tst:TRICE8_3  %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE8_4 ( ID(2532), "tst:TRICE8_4  %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE8_5 ( ID(2044), "tst:TRICE8_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE8_6 ( ID(4735), "tst:TRICE8_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE8_7 ( ID(6285), "tst:TRICE8_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE8_8 ( ID(7744), "tst:TRICE8_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE8_9 ( ID(5123), "tst:TRICE8_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE8_10( ID(5685), "tst:TRICE8_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE8_11( ID(6824), "tst:TRICE8_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE8_12( ID(1504), "tst:TRICE8_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE8   ( ID(7368), "tst:TRICE8   %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );
        break; case __LINE__: TRICE8_9 ( ID(7610), "tst:TRICE8_9 %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );

        break; case __LINE__: TRICE( ID(4575), "sig:TRICE16 with variable param count 1 to 12\n" );
        break; case __LINE__: TRICE16( ID(3817), "tst:TRICE16 %d\n", -1 );
        break; case __LINE__: TRICE16( ID(4756), "tst:TRICE16 %d %d\n", -1, -2 );
        break; case __LINE__: TRICE16( ID(2731), "tst:TRICE16 %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE16( ID(4236), "tst:TRICE16 %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE16( ID(4904), "tst:TRICE16 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE16( ID(1658), "tst:TRICE16 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE16( ID(4878), "tst:TRICE16 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE16( ID(3202), "tst:TRICE16 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE16( ID(7256), "tst:TRICE16 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE16( ID(3690), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE16( ID(2256), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE16( ID(2825), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE16( ID(7844), "tst:TRICE16 %t %b %x %X %d %u %o %O %p\n", 3, 3, 3, 3, 3, 3, 3, 3, 3 );
        break; case __LINE__: TRICE16_1 ( ID(5911), "tst:TRICE16_1  %d\n", -1 );
        break; case __LINE__: TRICE16_2 ( ID(6323), "tst:TRICE16_2  %d %d\n", -1, -2 );
        break; case __LINE__: TRICE16_3 ( ID(1470), "tst:TRICE16_3  %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE16_4 ( ID(1103), "tst:TRICE16_4  %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE16_5 ( ID(7373), "tst:TRICE16_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE16_6 ( ID(4318), "tst:TRICE16_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE16_7 ( ID(6922), "tst:TRICE16_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE16_8 ( ID(7975), "tst:TRICE16_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE16_9 ( ID(3339), "tst:TRICE16_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE16_10( ID(7648), "tst:TRICE16_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE16_11( ID(6452), "tst:TRICE16_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE16_12( ID(4602), "tst:TRICE16_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE16_9 ( ID(7392), "tst:TRICE16 %t %b %x %X %d %u %o %O %p\n", 3, 3, 3, 3, 3, 3, 3, 3, 3 );

        break; case __LINE__: TRICE( ID(4422), "sig:TRICE32 with variable param count 1 to 12\n" );
        break; case __LINE__: TRICE32( ID(7461), "tst:TRICE32 %d\n", -1 );
        break; case __LINE__: TRICE32( ID(3075), "tst:TRICE32 %d %d\n", -1, -2 );
        break; case __LINE__: TRICE32( ID(1087), "tst:TRICE32 %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE32( ID(5339), "tst:TRICE32 %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE32( ID(3620), "tst:TRICE32 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE32( ID(4058), "tst:TRICE32 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE32( ID(6280), "tst:TRICE32 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE32( ID(4059), "tst:TRICE32 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE32( ID(3699), "tst:TRICE32 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE32( ID(3604), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE32( ID(7022), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE32( ID(6386), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE32( ID(3736), "tst:TRICE32 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));
        break; case __LINE__: TRICE32_1 ( ID(3031), "tst:TRICE32_1  %d\n", -1 );
        break; case __LINE__: TRICE32_2 ( ID(2823), "tst:TRICE32_2  %d %d\n", -1, -2 );
        break; case __LINE__: TRICE32_3 ( ID(4949), "tst:TRICE32_3  %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE32_4 ( ID(6745), "tst:TRICE32_4  %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE32_5 ( ID(4898), "tst:TRICE32_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE32_6 ( ID(4743), "tst:TRICE32_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE32_7 ( ID(7521), "tst:TRICE32_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE32_8 ( ID(6877), "tst:TRICE32_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE32_9 ( ID(3064), "tst:TRICE32_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE32_10( ID(6123), "tst:TRICE32_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE32_11( ID(1041), "tst:TRICE32_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE32_12( ID(4125), "tst:TRICE32_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE32   ( ID(6106), "tst:TRICE32    %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));
        break; case __LINE__: TRICE32_12( ID(4673), "tst:TRICE32_12 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));

        break; case __LINE__: TRICE( ID(6480), "sig:TRICE64 with variable param count 1 to 12\n" );
        break; case __LINE__: TRICE64( ID(1273), "tst:TRICE64 %d\n", -1 );
        break; case __LINE__: TRICE64( ID(3778), "tst:TRICE64 %d %d\n", -1, -2 );
        break; case __LINE__: TRICE64( ID(6232), "tst:TRICE64 %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE64( ID(5926), "tst:TRICE64 %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE64( ID(6348), "tst:TRICE64 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE64( ID(3548), "tst:TRICE64 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE64( ID(2509), "tst:TRICE64 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE64( ID(5619), "tst:TRICE64 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE64( ID(3670), "tst:TRICE64 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE64( ID(1661), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE64( ID(7734), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE64( ID(4338), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE64( ID(7996), "tst:TRICE64 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aDouble(3.14159), aDouble(3.14159), aDouble(3.14159));
        break; case __LINE__: TRICE64_1 ( ID(1869), "tst:TRICE64_1  %d\n", -1 );
        break; case __LINE__: TRICE64_2 ( ID(7634), "tst:TRICE64_2  %d %d\n", -1, -2 );
        break; case __LINE__: TRICE64_3 ( ID(4483), "tst:TRICE64_3  %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE64_4 ( ID(5442), "tst:TRICE64_4  %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE64_5 ( ID(4461), "tst:TRICE64_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE64_6 ( ID(2983), "tst:TRICE64_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE64_7 ( ID(1361), "tst:TRICE64_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE64_8 ( ID(5477), "tst:TRICE64_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE64_9 ( ID(4208), "tst:TRICE64_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE64_10( ID(5215), "tst:TRICE64_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE64_11( ID(3327), "tst:TRICE64_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE64_12( ID(6633), "tst:TRICE64_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE64_12( ID(1064), "tst:TRICE64_12 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aDouble(3.14159), aDouble(3.14159), aDouble(3.14159));

        break; case __LINE__: TRICE( ID(7929), "att:positive and negative float in format variants\n");
        break; case __LINE__: TRICE( ID(7540), "rd:TRICE float %f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID(4936), "rd:TRICE float %9f (%%9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID(3298), "rd:TRICE float %.9f (%%.9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID(3434), "rd:TRICE float %9.f (%%9.f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID(6927), "rd:TRICE float %9.6f (%%9.6f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID(3774), "rd:TRICE float %f (%%f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(5525), "rd:TRICE float %9f (%%9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(7924), "rd:TRICE float %.9f (%%.9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(4069), "rd:TRICE float %9.f (%%9.f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(1264), "rd:TRICE float %9.6f (%%9.6f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(7389), "rd:TRICE float %+f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID(3652), "rd:TRICE float %+9f (%%9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID(2529), "rd:TRICE float %+.9f (%%.9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID(6760), "rd:TRICE float %+9.f (%%9.f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID(6821), "rd:TRICE float %+9.6f (%%9.6f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID(4672), "rd:TRICE float %+f (%%f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(5950), "rd:TRICE float %+9f (%%9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(1488), "rd:TRICE float %+.9f (%%.9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(1463), "rd:TRICE float %+9.f (%%9.f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(2164), "rd:TRICE float %+9.6f (%%9.6f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(2141), "rd:TRICE float %-f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID(7294), "rd:TRICE float %-9f (%%9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID(2794), "rd:TRICE float %-.9f (%%.9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID(1772), "rd:TRICE float %-9.f (%%9.f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID(5245), "rd:TRICE float %-9.6f (%%9.6f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID(3564), "rd:TRICE float %-f (%%f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(2146), "rd:TRICE float %-9f (%%9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(6024), "rd:TRICE float %-.9f (%%.9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(1369), "rd:TRICE float %-9.f (%%9.f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(2928), "rd:TRICE float %-9.6f (%%9.6f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(4879), "att:positive float & double in variants\n");
        break; case __LINE__: TRICE32_1( ID(5905), "rd:TRICE32_1 float %e (%%e)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1( ID(3897), "rd:TRICE32_1 float %f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1( ID(7846), "rd:TRICE32_1 float %g (%%g)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1( ID(3103), "rd:TRICE32_1 float %E (%%E)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1( ID(2887), "rd:TRICE32_1 float %F (%%F)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1( ID(6595), "rd:TRICE32_1 float %G (%%G)\n", aFloat(x) );
        break; case __LINE__: TRICE32  ( ID(6850), "rd:TRICE32 float %e (%%e)\n", aFloat(x) );
        break; case __LINE__: TRICE32  ( ID(3263), "rd:TRICE32 float %f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE32  ( ID(3182), "rd:TRICE32 float %g (%%g)\n", aFloat(x) );
        break; case __LINE__: TRICE32  ( ID(4724), "rd:TRICE32 float %E (%%E)\n", aFloat(x) );
        break; case __LINE__: TRICE32  ( ID(2499), "rd:TRICE32 float %F (%%F)\n", aFloat(x) );
        break; case __LINE__: TRICE32  ( ID(3801), "rd:TRICE32 float %G (%%G)\n", aFloat(x) );
        break; case __LINE__: TRICE    ( ID(6870), "rd:TRICE float %e (%%e)\n", aFloat(x) );
        break; case __LINE__: TRICE    ( ID(4364), "rd:TRICE float %f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE    ( ID(2613), "rd:TRICE float %g (%%g)\n", aFloat(x) );
        break; case __LINE__: TRICE    ( ID(1943), "rd:TRICE float %E (%%E)\n", aFloat(x) );
        break; case __LINE__: TRICE    ( ID(1282), "rd:TRICE float %F (%%F)\n", aFloat(x) );
        break; case __LINE__: TRICE    ( ID(1636), "rd:TRICE float %G (%%G)\n", aFloat(x) );
        break; case __LINE__: TRICE64  ( ID(1088), "rd:TRICE64 double %e (%%e), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64  ( ID(4905), "rd:TRICE64 double %f (%%f), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64  ( ID(3552), "rd:TRICE64 double %g (%%g), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64  ( ID(2943), "rd:TRICE64 double %E (%%E), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64  ( ID(4919), "rd:TRICE64 double %F (%%F), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64  ( ID(4442), "rd:TRICE64 double %G (%%G), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( ID(3350), "rd:TRICE64_1 double %e (%%e), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( ID(6325), "rd:TRICE64_1 double %f (%%f), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( ID(7133), "rd:TRICE64_1 double %g (%%g), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( ID(1162), "rd:TRICE64_1 double %E (%%E), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( ID(5637), "rd:TRICE64_1 double %F (%%F), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( ID(5991), "rd:TRICE64_1 double %G (%%G), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE( ID(7682), "att:negative float & double\n" );
        break; case __LINE__: TRICE( ID(4944), "rd:TRICE float %e (%%e)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(6147), "rd:TRICE float %f (%%f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(4930), "rd:TRICE float %g (%%g)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(4045), "rd:TRICE float %E (%%E)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(5200), "rd:TRICE float %F (%%F)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(7821), "rd:TRICE float %G (%%G)\n", aFloat(-x) );
        break; case __LINE__: TRICE64( ID(6075), "rd:TRICE64 double %e (%%e), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64( ID(5863), "rd:TRICE64 double %f (%%f), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64( ID(4160), "rd:TRICE64 double %g (%%g), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64( ID(5212), "rd:TRICE64 double %E (%%E), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64( ID(3153), "rd:TRICE64 double %F (%%F), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64( ID(1243), "rd:TRICE64 double %G (%%G), aDouble(y)\n", aDouble(-y) );

        break; case __LINE__: TRICE  ( ID(6078), "att:formatted float & double\n" );
        break; case __LINE__: TRICE32( ID(3166), "rd:TRICE32 float %12.6e (%%12.6e)\n", aFloat(x) );
        break; case __LINE__: TRICE32( ID(5657), "rd:TRICE32 float %12.6f (%%12.6f)\n", aFloat(x) );
        break; case __LINE__: TRICE32( ID(1134), "rd:TRICE32 float %12.6g (%%12.6g)\n", aFloat(x) );
        break; case __LINE__: TRICE32( ID(7948), "rd:TRICE32 float %12.6E (%%12.6E)\n", aFloat(x) );
        break; case __LINE__: TRICE32( ID(7958), "rd:TRICE32 float %12.6F (%%12.6F)\n", aFloat(x) );
        break; case __LINE__: TRICE32( ID(1608), "rd:TRICE32 float %12.6G (%%12.6G)\n", aFloat(x) );
        break; case __LINE__: TRICE64( ID(3813), "rd:TRICE64 double %12.6e (%%12.6e), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64( ID(6129), "rd:TRICE64 double %12.6f (%%12.6f), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64( ID(6864), "rd:TRICE64 double %12.6g (%%12.6g), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64( ID(6623), "rd:TRICE64 double %12.6E (%%12.6E), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64( ID(4831), "rd:TRICE64 double %12.6F (%%12.6F), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64( ID(3198), "rd:TRICE64 double %12.6G (%%12.6G), aDouble(y)\n", aDouble(y) );

        break; case __LINE__: TRICE ( ID(5127), "att:Various single arguments\n" );
        break; case __LINE__: TRICE8( ID(1071), "rd:TRICE8 line %t (%%t ,0)\n", 0 );
        break; case __LINE__: TRICE8( ID(2259), "rd:TRICE8 line %t (%%t ,2)\n", 2 );
        break; case __LINE__: TRICE8( ID(3012), "rd:TRICE8 line %u (%%u ,-1)\n", -1 );
        break; case __LINE__: TRICE8( ID(5054), "rd:TRICE8 line %b (%%b ,-1)\n", -1 );
        break; case __LINE__: TRICE8( ID(7035), "rd:TRICE8 line %o (%%o ,-1)\n", -1 );
        break; case __LINE__: TRICE8( ID(5227), "rd:TRICE8 line %O (%%O ,-1)\n", -1 );
        break; case __LINE__: TRICE8( ID(5269), "rd:TRICE8 line %X (%%X ,-1)\n", -1 );
        break; case __LINE__: TRICE8( ID(1853), "rd:TRICE8 line %x (%%x ,-1)\n", -1 );
        break; case __LINE__: TRICE8( ID(4101), "rd:TRICE8 line %d (%%d ,-1)\n", -1 );
        break; case __LINE__: TRICE16_1( ID(3028), "rd:TRICE16_1 line %t (%%t ,0)\n", 0 );
        break; case __LINE__: TRICE16_1( ID(6761), "rd:TRICE16_1 line %t (%%t ,2)\n", 2 );
        break; case __LINE__: TRICE16_1( ID(1397), "rd:TRICE16_1 line %u (%%u -1)\n", -1 );
        break; case __LINE__: TRICE16_1( ID(1030), "rd:TRICE16_1 line %b (%%b -1)\n", -1 );
        break; case __LINE__: TRICE16_1( ID(7802), "rd:TRICE16_1 line %o (%%o -1)\n", -1 );
        break; case __LINE__: TRICE16_1( ID(2269), "rd:TRICE16_1 line %x (%%x -1)\n", -1 );
        break; case __LINE__: TRICE16_1( ID(4075), "rd:TRICE16_1 line %d (%%d -1)\n", -1 );
        break; case __LINE__: TRICE( ID(7970), "rd:TRICE line %t (%%t -1)\n", 0 );
        break; case __LINE__: TRICE( ID(3819), "rd:TRICE line %t (%%t -1)\n", 2 );
        break; case __LINE__: TRICE( ID(4468), "rd:TRICE line %u (%%u)\n", -1 );
        break; case __LINE__: TRICE( ID(7268), "rd:TRICE line %b (%%b)\n", -2 );
        break; case __LINE__: TRICE( ID(6705), "rd:TRICE line %o (%%o)\n", -3 );
        break; case __LINE__: TRICE( ID(5517), "rd:TRICE line %x (%%x)\n", -4 );
        break; case __LINE__: TRICE( ID(4121), "rd:TRICE line %d (%%d)\n", -5 );

        break; case __LINE__: TRICE_N( ID(6302), "msg:%s\n", A, 0 );
        break; case __LINE__: TRICE_N( ID(6435), "msg:%s\n", A, 1 );
        break; case __LINE__: TRICE_N( ID(4379), "msg:%s\n", A, 2 );
        break; case __LINE__: TRICE_N( ID(5019), "msg:%s\n", A, 3 );
        break; case __LINE__: TRICE_N( ID(5631), "msg:%s\n", A, 4 );
        break; case __LINE__: TRICE_N( ID(4994), "msg:%s\n", A, 5 );
        break; case __LINE__: TRICE_N( ID(7254), "msg:%s\n", A, 6 );
        break; case __LINE__: TRICE_N( ID(6719), "msg:%s\n", A, 7 );
        break; case __LINE__: TRICE_N( ID(1564), "msg:%s\n", A, 8 );
        break; case __LINE__: TRICE_N( ID(7151), "msg:%s\n", A, 9 );
        break; case __LINE__: TRICE_N( ID(6336), "msg:%s\n", A, 10 );
        break; case __LINE__: TRICE_N( ID(3523), "msg:%s\n", A, 11 );
        break; case __LINE__: TRICE_N( ID(2859), "msg:%s\n", A, 12 );
        break; case __LINE__: TRICE_N( ID(6444), "msg:%s\n", A, 13 );
        break; case __LINE__: TRICE_N( ID(2531), "msg:%s\n", A, 14 );
        break; case __LINE__: TRICE_N( ID(6753), "msg:%s\n", A, 15 );
        break; case __LINE__: TRICE_N( ID(6981), "msg:%s\n", A, 16 );
        break; case __LINE__: TRICE_N( ID(6677), "msg:%s\n", A, 17 );
        break; case __LINE__: TRICE_N( ID(4556), "msg:%s\n", A, 18 );
        break; case __LINE__: TRICE_N( ID(5254), "msg:%s\n", A, 19 );
        break; case __LINE__: TRICE_N( ID(1983), "msg:%s\n", A, 120 );
        break; case __LINE__: TRICE_N( ID(3936), "msg:%s\n", A, 121 );
        break; case __LINE__: TRICE_N( ID(6261), "msg:%s\n", A, 122 );
        break; case __LINE__: TRICE_N( ID(2291), "msg:%s\n", A, 123 );
        break; case __LINE__: TRICE_N( ID(7188), "msg:%s\n", A, 124 );
        break; case __LINE__: TRICE_N( ID(2827), "msg:%s\n", A, 125 );
        break; case __LINE__: TRICE_N( ID(2936), "msg:%s\n", A, 126 );
        break; case __LINE__: TRICE_N( ID(3443), "msg:%s\n", A, 127 );
        break; case __LINE__: TRICE_N( ID(3532), "msg:%s\n", A, 128 );
        break; case __LINE__: TRICE_N( ID(5908), "msg:%s\n", A, 129 );

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
#define SCOPY( element ) do{ char* n = #element; int size = sizeof( src->element ); memcpy( p, &(src->element), size ); p += size; TRICE_S( ID(5977), "rd:sizeof(%8s)", n ); TRICE( ID(4854), " = %d\n", size);}while(0);

    //! DCOPY is a helper macro for struct deserialization.
#define DCOPY( element ) do{ char* n = #element; int size = sizeof( dst->element ); memcpy( &(dst->element), p, size ); p += size; TRICE_S( ID(2904), "rd:sizeof(%8s)", n ); TRICE( ID(6391), " = %d\n", size);}while(0);


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
    TRICE ( Id(4517), "inf: Tryout tx struct:" );
    TRICE8_B( Id(5976), " %02x ", &tx, sizeof(tx) );
    TRICE( Id(6774), "\n" );

    TRICE ( Id(3515), "inf: Tryout buffer:" );
    TRICE8_B( Id(6933), " %02x ", dst, len ); //lint !e670
    TRICE( Id(2486), "\n" );

    src = dst; // "data transfer"

    len = deserializeTryout( &rx, src );
    TRICE ( Id(7972), "inf: Tryout rx struct:" );
    TRICE8_B( Id(6195), " %02x ", &rx, sizeof(rx) );
    TRICE( Id(2068), "\n" );

    TRICE( Id(7153), "inf:sizeOf(Trypout) = %d, buffer length = %d\n", sizeof(tx), len );
    TRICE8_F( Id(3419), "info:TryoutStructFunction", &tx, sizeof(tx) );
    TRICE8_F( Id(4764), "info:TryoutBufferFunction", dst, len ); //lint !e670
}
*/

static void exampleOfManualJSONencoding(void){
    typedef  struct {
    int Apple, Birn;
    float Fish;
    } Ex_t;
    Ex_t Ex = { -1, 2, (float)2.781 };
    TRICE( Id(3453), "att:MyStructEvaluationFunction(json:ExA{Apple:%d, Birn:%u, Fisch:%f}\n", Ex.Apple, Ex.Birn, aFloat(Ex.Fish) );
}
