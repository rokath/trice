/*! \file triceCheck.c
\brief trices for tool evaluation
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/
#include <float.h>
//#define TRICE_OFF // enable this line to disable trice code generation in this file object
#include "trice.h"

// Do **NOT** edit triceCheck.c!!!
//
// Edit file triceCheck.txt instead and call updateTestData.sh then.
// The strings behind "//exp:" are the expected result for each line (-color=none)

static int32_t FloatToInt32( float f );
static int64_t DoubleToInt64( double f );
static void exampleOfManualSerialization( void );
static void exampleOfManualJSONencoding(void);
static void showTriceDepth( void );

//! TriceCheck performs trice code sequence n.
//! TriceCheck writes out all types of trices with fixed values for testing
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

        // normal use cases
        //break; case __LINE__: TRice( iD( 6968), "info:This is a message without values and a 32-bit stamp.\n" );        //exp: time:32323232info:This is a message without values and a 32-bit stamp.
        //break; case __LINE__: Trice( iD( 4086), "info:This is a message without values and a 16-bit stamp.\n" );        //exp: time:    1616info:This is a message without values and a 16-bit stamp.
        //break; case __LINE__: trice( iD( 6760), "info:This is a message without values and without stamp.\n"  );        //exp: time:        info:This is a message without values and without stamp.
        //break; case __LINE__: TRice( iD( 4475),"info:12 default bit width values %d, %u, %x, %X, &t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 32-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //exp: time:32323232info:12 default bit width values -3, 4294967292, fffffffb, FFFFFFFA, &t, 1.401298e-45, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11000000111000111111001101011100 and a 32-bit stamp.
        //break; case __LINE__: Trice( iD( 6633),"info:12 default bit width values %d, %u, %x, %X, &t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 16-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //exp: time:    1616info:12 default bit width values -3, 4294967292, fffffffb, FFFFFFFA, &t, 1.401298e-45, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11000000111000111111001101011100 and a 16-bit stamp.
        //break; case __LINE__: trice( iD( 2604),"info:12 default bit width values %d, %u, %x, %X, &t, %e, %f, %g, %E, %F, %G, 0xb%08b and without stamp.\n" , -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //exp: time:        info:12 default bit width values -3, 4294967292, fffffffb, FFFFFFFA, &t, 1.401298e-45, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11000000111000111111001101011100 and without stamp.

        // special use cases
        break; case __LINE__: TRice64( iD( 5629),"info:12 64-bit values %d, %u, %x, %X, &t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 32-bit stamp.\n", -3, -4, -5, -6, 1, aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), -8 ); //exp: time:32323232info:12 64-bit values -3, 18446744073709551612, fffffffffffffffb, FFFFFFFFFFFFFFFA, &t, 4.940656e-324, -7.123457, -7.123456789, -7.123457E+00, -7.123457, -7.123456789, 0xb1100000000011100011111100110101101110100110111001110010110001101 and a 32-bit stamp.
        break; case __LINE__: Trice64( iD( 6173),"info:12 64-bit values %d, %u, %x, %X, &t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 16-bit stamp.\n", -3, -4, -5, -6, 1, aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), -8 ); //exp: time:    1616info:12 64-bit values -3, 18446744073709551612, fffffffffffffffb, FFFFFFFFFFFFFFFA, &t, 4.940656e-324, -7.123457, -7.123456789, -7.123457E+00, -7.123457, -7.123456789, 0xb1100000000011100011111100110101101110100110111001110010110001101 and a 16-bit stamp.
        break; case __LINE__: trice64( iD( 3274),"info:12 64-bit values %d, %u, %x, %X, &t, %e, %f, %g, %E, %F, %G, 0xb%08b and without stamp.\n" , -3, -4, -5, -6, 1, aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), -8 ); //exp: time:        info:12 64-bit values -3, 18446744073709551612, fffffffffffffffb, FFFFFFFFFFFFFFFA, &t, 4.940656e-324, -7.123457, -7.123456789, -7.123457E+00, -7.123457, -7.123456789, 0xb1100000000011100011111100110101101110100110111001110010110001101 and without stamp.
        break; case __LINE__: TRice32( iD( 3848),"info:12 32-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 32-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:32323232info:12 32-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 32-bit stamp.
        break; case __LINE__: Trice32( iD( 1880),"info:12 32-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 16-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:    1616info:12 32-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 16-bit stamp.
        break; case __LINE__: trice32( iD( 2134),"info:12 32-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and without stamp.\n" , -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:        info:12 32-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and without stamp.
        break; case __LINE__: TRice16( iD( 2947),"info:12 16-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 32-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:32323232info:12 16-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 32-bit stamp.
        break; case __LINE__: Trice16( iD( 2780),"info:12 16-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 16-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:    1616info:12 16-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 16-bit stamp.
        break; case __LINE__: trice16( iD( 1595),"info:12 16-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and without stamp.\n" , -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:        info:12 16-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and without stamp.
        break; case __LINE__: TRice8 ( iD( 2266),"info:12  8-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 32-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:32323232info:12  8-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 32-bit stamp.
        break; case __LINE__: Trice8 ( iD( 5270),"info:12  8-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 16-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:    1616info:12  8-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 16-bit stamp.
        break; case __LINE__: trice8 ( iD( 7238),"info:12  8-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and without stamp.\n" , -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:        info:12  8-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and without stamp.

        break; case __LINE__: trice16( iD( 4252), "att: line %u\n", __LINE__ );
        break; case __LINE__: exampleOfManualSerialization();

        break; case __LINE__: trice8   ( iD( 2593),         "value=%x\n", 0x55 );                           //exp: time:        value=55
        break; case __LINE__: trice8   ( iD( 6899),  "value=%x\n", 0x88 );                           //exp: time:        value=88

        break; case __LINE__: TRICE( ID( 5893), "att:mixed int & float & double & bits\n" );
        break; case __LINE__: TRICE32( ID( 1348), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n", FloatToInt32(x), aFloat(x), aFloat(x), aFloat(x) );           //exp: time:32323232rd:TRICE32 int 1089, float 1089.608276 (%f), 44883377, 01000100100010000011001101110111                                         
        break; case __LINE__: TRICE32( ID( 1613), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",    (int32_t)(x), aFloat(x), aFloat(x), aFloat(x) );           //exp: time:32323232rd:TRICE32 int 1089, float 1089.608276 (%f), 44883377, 01000100100010000011001101110111                                         
        break; case __LINE__: TRICE32( ID( 6354), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",              x,  aFloat(x), aFloat(x), aFloat(x) );           //exp: time:32323232rd:TRICE32 int 1089, float 1089.608276 (%f), 44883377, 01000100100010000011001101110111                                         
        break; case __LINE__: TRICE32( ID( 2096), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n", FloatToInt32(-x), aFloat(-x), aFloat(-x), aFloat(-x) );       //exp: time:32323232rd:TRICE32 int -1089, float -1089.608276 (%f), c4883377, 11000100100010000011001101110111                                       
        break; case __LINE__: TRICE32( ID( 6078), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",    (int32_t)(-x), aFloat(-x), aFloat(-x), aFloat(-x) );       //exp: time:32323232rd:TRICE32 int -1089, float -1089.608276 (%f), c4883377, 11000100100010000011001101110111                                       
    //  break; case __LINE__: TRICE32( ID( 1142), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",              -x,  aFloat(-x), aFloat(-x), aFloat(-x) );       //exp: time:32323232rd:TRICE32 int 1089, float -1089.608276 (%f), c4883377, 11000100100010000011001101110111                                        
        break; case __LINE__: TRICE64( ID( 1702), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n", DoubleToInt64(y), aDouble(y), aDouble(y), aDouble(y) );     //exp: time:32323232rd:TRICE64 int 518, double 518.054749 (%f), 4080307020601050, 0100000010000000001100000111000000100000011000000001000001010000   
        break; case __LINE__: TRICE64( ID( 6061), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",     (int64_t)(y), aDouble(y), aDouble(y), aDouble(y) );     //exp: time:32323232rd:TRICE64 int 518, double 518.054749 (%f), 4080307020601050, 0100000010000000001100000111000000100000011000000001000001010000   
        break; case __LINE__: TRICE64( ID( 5363), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",               y,  aDouble(y), aDouble(y), aDouble(y) );     //exp: time:32323232rd:TRICE64 int 518, double 518.054749 (%f), 4080307020601050, 0100000010000000001100000111000000100000011000000001000001010000   
        break; case __LINE__: TRICE64( ID( 1813), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n", DoubleToInt64(-y), aDouble(-y), aDouble(-y), aDouble(-y) ); //exp: time:32323232rd:TRICE64 int -518, double -518.054749 (%f), c080307020601050, 1100000010000000001100000111000000100000011000000001000001010000
        break; case __LINE__: TRICE64( ID( 4865), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",     (int64_t)(-y), aDouble(-y), aDouble(-y), aDouble(-y) ); //exp: time:32323232rd:TRICE64 int -518, double -518.054749 (%f), c080307020601050, 1100000010000000001100000111000000100000011000000001000001010000
    //  break; case __LINE__: TRICE64( ID( 6376), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",               -y,  aDouble(-y), aDouble(-y), aDouble(-y) ); //exp: time:32323232rd:TRICE64 int 518, double -518.054749 (%f), c080307020601050, 1100000010000000001100000111000000100000011000000001000001010000 

        break; case __LINE__: trice16( iD( 2087), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRICE_S( ID( 7224), "msg:With TRICE_S:%s\n", sABCDE );                           //exp: time:32323232msg:With TRICE_S:abcde 12345
        break; case __LINE__: TRICE_N( ID( 5822), "sig:With TRICE_N:%s\n", sABCDE, lenABCDE );                 //exp: time:32323232sig:With TRICE_N:abcde 12345
        break; case __LINE__: TRICE8_B( ID( 3581), "  %02x", sABCDE, lenABCDE );                               //exp: time:32323232  61  62  63  64  65  20  31  32  33  34  35
        break; case __LINE__: TRICE( ID( 7888), "\n" );
        break; case __LINE__: TRICE8_B( ID( 7784), "%4d", sABCDE, lenABCDE );                                  //exp: time:32323232  97  98  99 100 101  32  49  50  51  52  53
        break; case __LINE__: TRICE( ID( 1771), "\n" );
        break; case __LINE__: TRICE8_B( ID( 7350), "  %02x", b8, sizeof(b8)/sizeof(int8_t) );                  //exp: time:32323232  00  ff  fe  33  04  05  06  07  08  09  0a  0b  00  ff  fe  33  04  05  06  07  08  09  0a  0b
        break; case __LINE__: TRICE( ID( 6249), "\n" );
        break; case __LINE__: TRICE16_B( ID( 2288), "  %04x", b16, sizeof(b16)/sizeof(int16_t) );              //exp: time:32323232  0000  ffff  fffe  3344
        break; case __LINE__: TRICE( ID( 7917), "\n" );
        break; case __LINE__: TRICE32_B( ID( 5463), "  %08x", b32, sizeof(b32)/sizeof(int32_t) );              //exp: time:32323232  00000000  ffffffff  fffffffe  33445555
        break; case __LINE__: TRICE( ID( 3424), "\n" );
        break; case __LINE__: TRICE64_B( ID( 5362), "  %016x", b64, sizeof(b64)/sizeof(int64_t) );             //exp: time:32323232  0000000000000000  ffffffffffffffff  fffffffffffffffe  3344555566666666
        break; case __LINE__: TRICE( ID( 1835), "\n" );
        break; case __LINE__: TRICE8_F( ID( 2440),  "info:FunctionNameW", b8,  sizeof(b8) /sizeof(int8_t) );   //exp: time:32323232info:FunctionNameW(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)
        break; case __LINE__: TRICE16_F( ID( 5858), "info:FunctionNameX", b16, sizeof(b16)/sizeof(int16_t) );  //exp: time:32323232info:FunctionNameX(0000)(ffff)(fffe)(3344) 
        break; case __LINE__: TRICE32_F( ID( 7212), "info:FunctionNameY", b32, sizeof(b32)/sizeof(int32_t) );  //exp: time:32323232info:FunctionNameY(00000000)(ffffffff)(fffffffe)(33445555)
        break; case __LINE__: TRICE64_F( ID( 4418), "info:FunctionNameZ", b64, sizeof(b64)/sizeof(int64_t) );  //exp: time:32323232info:FunctionNameZ(0000000000000000)(ffffffffffffffff)(fffffffffffffffe)(3344555566666666)
        break; case __LINE__: exampleOfManualJSONencoding(); // exp: time:    1616att:MyStructEvaluationFunction(json:ExA{Apple:-1, Birn:2, Fisch:2.781000}

        break; case __LINE__: trice16( iD( 6409), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRICE( Id( 4785), "MSG:1/11 = %g\n", aFloat( 1.0/11 ) ); //exp: time:    1616MSG:1/11 = 0.09090909
        break; case __LINE__: { //exp: time:32323232msg:x = 5.934 = 5.934, 5.934
                              float a = (float)5.934;
                              float b = a + ((a > 0) ? 0.0005f : -0.0005f);
                              int c = (int)b;
                              int d = (int)(b * 1000) % 1000;
                              int e = (int)(1000 * (float)(a - c)); 
                              TRICE( ID( 4178), "msg:x = %g = %d.%03d, %d.%03d\n", aFloat(a), c, d, c, e ); 
                              }

        //break; case __LINE__: TRICE( id( 4329), "dbg:Hi!\n" );                       // exp: time:        dbg:Hi!    
        //break; case __LINE__: TRICE( id( 2015), "dbg:\tHi!\n" );                     
        //break; case __LINE__: TRICE( id( 5492), "dbg:\t\tHi!\n" );                   
        //break; case __LINE__: TRICE( id( 4180), "dbg:\t\t\tHi!\n" );                 
        //break; case __LINE__: TRICE( id( 5596), "dbg:\\tHi!\n" );                    
        //break; case __LINE__: TRICE( Id( 4272), "dbg:\aHi!\n" ); // sound!
        //break; case __LINE__: TRICE( Id( 2949), "dbg:\a\aHi!\n" ); // sound!
        //break; case __LINE__: TRICE( Id( 1037), "dbg:\a\a\aHi!\n" ); // sound!
        //break; case __LINE__: TRICE( Id( 1629), "dbg:\\aHi!\n" );
        break; case __LINE__: TRICE( Id( 4642), "err:An ERROR messgage with sound!\a\n" ); // sound!

        break; case __LINE__: TRICE_S( ID( 3421), "sig:TRICE_S=%s\n", s );            // exp: time:32323232sig:TRICE_S=AAAAAAAAAAAA
        break; case __LINE__: TRICE_N( ID( 2031), "sig:TRICE_N=%s\n", s, strlen(s) ); // exp: time:32323232sig:TRICE_N=AAAAAAAAAAAA

        break; case __LINE__: trice16( iD( 2521), "att: line %u\n", __LINE__ );

//      break; case __LINE__: 
//  #ifdef XTEA_ENCRYPT_KEY
//          TRICE0 (Id( 4367), "--------------------------------------------------\n" );
//          {
//              uint32_t by[8] = {1,2,3,4,5,6};
//              int count = (sizeof(by) + 7) & ~7; // only multiple of 8 encryptable
//              TRICE32(  Id( 6050), "msg: message = %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
//              TRICE32( Id( 7530), "tim: pre TriceEncryption SysTick=%d\n", SYSTICKVAL );
//              XTEAEncrypt(by, count>>2);
//              TRICE32( Id( 1519), "tim: post TriceEncryption SysTick=%d\n", SYSTICKVAL );
//              TRICE32(  Id( 6095), "att: TriceEncrypted =  %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
//              #ifdef XTEA_DECRYPT
//              TRICE16_1( Id( 6906), "tim: pre TriceDecryption SysTick=%d\n", SYSTICKVAL );
//              XTEADecrypt(by, count>>2);
//              TRICE16_1( Id( 3562), "tim: post TriceDecryption SysTick=%d\n", SYSTICKVAL );
//              #endif
//              TRICE32(  Id( 4093), "msg: messge = %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
//          }
//          TRICE0 (Id( 2388), "--------------------------------------------------\n" );
//  #endif


        break; case __LINE__: showTriceDepth();

        break; case __LINE__: trice16( iD( 7471), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRICE( ID( 1072), "sig:Some time measurements\n" );
        break; case __LINE__: TRICE( ID( 7695), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 7611), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 4063), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 3916), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
        break; case __LINE__: TRICE64( ID( 2044), "rd:TRICE64 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE( ID( 7210), "tim:TRICE START time message\n");
        break; case __LINE__: TRICE( ID( 2526), "tim:TRICE STOP time message\n");
        break; case __LINE__: TRICE( ID( 6005), "tim:TRICE START time message\n");
        break; case __LINE__: TRICE( ID( 4902), "tim:TRICE STOP time message\n");
        break; case __LINE__: TRICE( ID( 5968), "tim:TRICE START time message\n");
        break; case __LINE__: TRICE64( ID( 6939), "rd:TRICE64 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE( ID( 5252), "tim:TRICE STOP time message\n");
        break; case __LINE__: TRICE( ID( 1248), "tim:TRICE --------------------------------------------------\n");
        break; case __LINE__: TRICE( ID( 7862), "tim:TRICE --------------------------------------------------\n");
        break; case __LINE__: TRICE64( ID( 2213), "rd:TRICE64 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE( ID( 5931), "tim:TRICE --------------------------------------------------\n");
        break; case __LINE__: TRICE( ID( 7537), "tim:TRICE --------------------------------------------------\n");
        break; case __LINE__: TRICE64( ID( 4527), "rd:TRICE64 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE( ID( 6802), "rd:TRICE32_1 line %d\n", __LINE__ );
        break; case __LINE__: TRICE64_1( ID( 3433), "rd:TRICE64_1 %d\n", __LINE__ );
        break; case __LINE__: TRICE32_2( ID( 7635), "rd:TRICE32_2 line %d,%d\n", __LINE__, 2 );
        break; case __LINE__: TRICE64_2( ID( 3502), "rd:TRICE64_2 line %d,%d\n", __LINE__, 2 );

        break; case __LINE__: trice16( iD( 4568), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRICE( ID( 5634), "sig:Runtime generated strings\n" );
        break; case __LINE__: {
                                  s = "AAAAAAAAAAAA";
                                  TRICE32( ID( 7500), "dbg:len=%u:", strlen(s) );
                                  TRICE_S( ID( 6436), "sig:%s\n", s );
                              }
        break; case __LINE__: {
                s = "\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBCCCCCCCC\
";
                          }
        break; case __LINE__: TRICE32( ID( 4021), "dbg:len=%u:", strlen(s) );
        break; case __LINE__: TRICE_S( ID( 6356), "sig:%s\n", s );
        break; case __LINE__: TRICE( ID( 5870), "sig:Runtime generated strings\n" );
        break; case __LINE__: {
            s = "\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\
DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\
EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\
";
                          }
        break; case __LINE__: TRICE32( ID( 6268), "dbg:len=%u: ", strlen(s) );
        break; case __LINE__: TRICE_S( ID( 4631), "sig:%s\n", s );
        break; case __LINE__: TRICE( ID( 7710), "sig:Runtime generated strings\n" );
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
        break; case __LINE__: TRICE32( ID( 6406), "dbg:len=%u: ", strlen(s) );
        break; case __LINE__: TRICE_S( ID( 4502), "sig:%s\n", s );
        break; case __LINE__: TRICE( ID( 3703), "sig:Runtime generated strings\n" );
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
        break; case __LINE__: TRICE32( ID( 7041), "dbg:len=%u: ", strlen(s) );
        break; case __LINE__: TRICE_S( ID( 3849), "sig:%s\n", s );

        break; case __LINE__: trice16( iD( 5746), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRICE( ID( 6003), "sig:Integer (indent, base, sign) - see https://yourbasic.org/golang/fmt-printf-reference-cheat-sheet/\n" );
        break; case __LINE__: TRICE( ID( 4205), "rd: 15 	%d 	Base 10\n", 15 );                                        //ex_: time:32323232rd: 15 	15 	Base 10                                   
        break; case __LINE__: TRICE( ID( 1947), "rd: +15 	%+d 	Always show sign\n", 15 );                           //ex_: time:32323232rd: +15 	+15 	Always show sign                          
        break; case __LINE__: TRICE( ID( 4759), "rd:   15 	%4d 	Pad with spaces (width 4, right justified)\n", 15 ); //ex_: time:32323232rd:   15	  15 	Pad with spaces (width 4, right justified)
        break; case __LINE__: TRICE( ID( 3672), "rd: 15   	%-4d 	Pad with spaces (width 4, left justified)\n", 15 );  //ex_: time:32323232rd: 15 	15   	Pad with spaces (width 4, left justified) 
        break; case __LINE__: TRICE( ID( 7221), "rd: 0015 	%04d 	Pad with zeroes (width 4)\n", 15 );                  //ex_: time:32323232rd: 0015 	0015 	Pad with zeroes (width 4)                 
        break; case __LINE__: TRICE( ID( 5541), "rd: 1111 	%b 	Base 2\n", 15 );                                         //ex_: time:32323232rd: 1111 	1111    Base 2                                    
        break; case __LINE__: TRICE( ID( 6575), "rd: 17 	%o 	Base 8\n", 15 );                                         //ex_: time:32323232rd: 17 	17      Base 8                                    
        break; case __LINE__: TRICE( ID( 4450), "rd: f 	%x 	Base 16, lowercase\n", 15 );                             //ex_: time:32323232rd: f 	f 	Base 16, lowercase                        
        break; case __LINE__: TRICE( ID( 4932), "rd: F 	%X 	Base 16, uppercase\n", 15 );                             //ex_: time:32323232rd: F 	F 	Base 16, uppercase                        
        break; case __LINE__: TRICE( ID( 1518), "rd: 0xf 	%#x 	Base 16, with leading 0x\n", 15 );                   //ex_: time:32323232rd: 0xf 	0xf 	Base 16, with leading 0x                  

        break; case __LINE__: trice16( iD( 6048), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRICE( ID( 6446), "sig:Character (quoted, Unicode)\n" );
        break; case __LINE__: TRICE( ID( 7294), "rd: A 	%c 	Character\n", 'A' );                                     //exp: time:32323232rd: A 	A 	Character
        break; case __LINE__: TRICE( ID( 2331), "rd: 'A' 	%q 	Quoted character\n", 'A' );                              //exp: time:32323232rd: 'A' 	'A' 	Quoted character
        break; case __LINE__: TRICE( ID( 1327), "rd: U+0041 	%U 	Unicode\n", 'A' );                        // todo
        break; case __LINE__: TRICE( ID( 7937), "rd: U+0041 'A' 	%#U 	Unicode with character\n", 'A' ); // todo

        break; case __LINE__: TRICE( ID( 1876), "sig:Boolean (true/false)\n" );
        break; case __LINE__: TRICE( ID( 3005), "rd:Use %%t to format a boolean as true (%t) or false (%t).\n", 1, 0 ); //exp: time:32323232rd:Use %t to format a boolean as true (true) or false (false).

        break; case __LINE__: TRICE( ID( 7038), "sig:Pointer (hex)\n" );
        break; case __LINE__: TRICE8 ( ID( 7702), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x84 );               //exp: time:32323232rd:Use %p to format a pointer in base 16 notation with leading 0x. (84)              
        break; case __LINE__: TRICE16( ID( 4882), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x1234 );             //exp: time:32323232rd:Use %p to format a pointer in base 16 notation with leading 0x. (1234)            
        break; case __LINE__: TRICE32( ID( 6076), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0xaabbccdd );         //exp: time:32323232rd:Use %p to format a pointer in base 16 notation with leading 0x. (aabbccdd)        
        break; case __LINE__: TRICE64( ID( 2965), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x1122334455667788 ); //exp: time:32323232rd:Use %p to format a pointer in base 16 notation with leading 0x. (1122334455667788)
        break; case __LINE__: TRICE  ( ID( 5663), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0xaabbccdd);          //exp: time:32323232rd:Use %p to format a pointer in base 16 notation with leading 0x. (aabbccdd)        

        break; case __LINE__: trice16( iD( 4988), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRICE( ID( 3547), "sig:Float (indent, precision, scientific notation)\n" );
        break; case __LINE__: TRICE( ID( 3903), "rd: 1.234560e+02		%e 	%%e Scientific notation\n", aFloat(123.456) );                       //exp: time:32323232rd: 1.234560e+02		1.234560e+02 	%e Scientific notation
        break; case __LINE__: TRICE( ID( 1699), "rd: 123.456000		%f 	%%f Decimal point, no exponent\n", aFloat(123.456) );                //exp: time:32323232rd: 123.456000		123.456001 	%f Decimal point, no exponent
        break; case __LINE__: TRICE( ID( 2974), "rd: 123.46			%.2f		%%.2f Default width, precision 2\n", aFloat(123.456) );      //exp: time:32323232rd: 123.46			123.46		%.2f Default width, precision 2
        break; case __LINE__: TRICE( ID( 7010), "rd: ␣␣123.46		%8.2f	%%8.2f Width 8, precision 2\n", aFloat(123.456) );                    //exp: time:32323232rd: ␣␣123.46		  123.46	%8.2f Width 8, precision 2
        break; case __LINE__: TRICE( ID( 1380), "rd: 123.456		%g		%%g Exponent as needed, necessary digits only\n", aFloat(123.456) ); //exp: time:32323232rd: 123.456		123.456		%g Exponent as needed, necessary digits only

        break; case __LINE__: TRICE( ID( 7180), "sig:Double (indent, precision, scientific notation)\n" );
        break; case __LINE__: TRICE64( ID( 3837), "rd: 1.234560e+02		%e 	%%e Scientific notation\n", aDouble(123.456) );                   //exp: time:32323232rd: 1.234560e+02		1.234560e+02 	%e Scientific notation
        break; case __LINE__: TRICE64( ID( 1529), "rd: 123.456000		%f 	%%f Decimal point, no exponent\n", aDouble(123.456) );                //exp: time:32323232rd: 123.456000		123.456000 	%f Decimal point, no exponent
        break; case __LINE__: TRICE64( ID( 5341), "rd: 123.46			%.2f    	%%.2f Default width, precision 2\n", aDouble(123.456) );      //exp: time:32323232rd: 123.46			123.46    	%.2f Default width, precision 2
        break; case __LINE__: TRICE64( ID( 3273), "rd: ␣␣123.46		%8.2f 	%%8.2f Width 8, precision 2\n", aDouble(123.456) );                //exp: time:32323232rd: ␣␣123.46		  123.46 	%8.2f Width 8, precision 2
        break; case __LINE__: TRICE64( ID( 4695), "rd: 123.456		%g		%%g Exponent as needed, necessary digits only\n", aDouble(123.456) ); //exp: time:32323232rd: 123.456		123.456		%g Exponent as needed, necessary digits only

        break; case __LINE__: TRICE( ID( 7491), "sig:String or byte slice (quote, indent, hex)\n" );
        break; case __LINE__: s = "café"; TRICE_S( ID( 7004), "rd: café 			%s		Plain string\n", s );           //exp: time:32323232rd: café 			café		Plain string
        break; case __LINE__: s = "café"; TRICE_S( ID( 2370), "rd: ␣␣café 		%6s 		Width 6, right justify\n", s );  //exp: time:32323232rd: ␣␣café 		  café 		Width 6, right justify
        break; case __LINE__: s = "café"; TRICE_S( ID( 6941), "rd: café␣␣ 		%-6s 		Width 6, left justify\n", s );   //exp: time:32323232rd: café␣␣ 		café   		Width 6, left justify
        break; case __LINE__: s = "café"; TRICE_S( ID( 7521), "rd: \"café\" 		%q		Quoted string\n", s );          //exp: time:32323232rd: \"café\" 		"café"		Quoted string
        break; case __LINE__: s = "café"; TRICE_S( ID( 5536), "rd: 636166c3a9 		%x	Hex dump of byte values\n", s );    //exp: time:32323232rd: 636166c3a9 		636166c3a9	Hex dump of byte values 
        break; case __LINE__: s = "café"; TRICE_S( ID( 3460), "rd: 63 61 66 c3 a9 	% x	Hex dump with spaces\n", s );       //exp: time:32323232rd: 63 61 66 c3 a9 	63 61 66 c3 a9	Hex dump with spaces

        break; case __LINE__: trice16( iD( 1002), "att: line %u\n", __LINE__ );

#ifdef TRICE_CGO

        break; case __LINE__: TRICE  ( ID( 7281), "rd:%E (%%E)\n",  aFloat(-555555555.5555555555) ); // exp: time:32323232rd:-5.555556E+08 (%E)
        break; case __LINE__: TRICE  ( ID( 3515), "rd:%F (%%F)\n",  aFloat(-555555555.5555555555) ); // exp: time:32323232rd:-555555584.000000 (%F)
        break; case __LINE__: TRICE  ( ID( 1944), "rd:%G (%%G)\n",  aFloat(-555555555.5555555555) ); // exp: time:32323232rd:-5.555556E+08 (%G)
        break; case __LINE__: TRICE64( ID( 4609), "rd:%E (%%E)\n", aDouble(-555555555.5555555555) ); // exp: time:32323232rd:-5.555556E+08 (%E)
        break; case __LINE__: TRICE64( ID( 5097), "rd:%F (%%F)\n", aDouble(-555555555.5555555555) ); // exp: time:32323232rd:-555555555.555556 (%F)
        break; case __LINE__: TRICE64( ID( 4853), "rd:%G (%%G)\n", aDouble(-555555555.5555555555) ); // exp: time:32323232rd:-5.555555555555556E+08 (%G)
        
        break; case __LINE__: TRICE  ( ID( 1187), "rd:%e (%%e)\n",  aFloat(-555555555.5555555555) ); // exp: time:32323232rd:-5.555556e+08 (%e)
        break; case __LINE__: TRICE  ( ID( 4360), "rd:%f (%%f)\n",  aFloat(-555555555.5555555555) ); // exp: time:32323232rd:-555555584.000000 (%f)
        break; case __LINE__: TRICE  ( ID( 5674), "rd:%g (%%g)\n",  aFloat(-555555555.5555555555) ); // exp: time:32323232rd:-5.555556e+08 (%g)
        break; case __LINE__: TRICE64( ID( 7481), "rd:%e (%%e)\n", aDouble(-555555555.5555555555) ); // exp: time:32323232rd:-5.555556e+08 (%e)
        break; case __LINE__: TRICE64( ID( 7149), "rd:%f (%%f)\n", aDouble(-555555555.5555555555) ); // exp: time:32323232rd:-555555555.555556 (%f)
        break; case __LINE__: TRICE64( ID( 3354), "rd:%g (%%g)\n", aDouble(-555555555.5555555555) ); // exp: time:32323232rd:-5.555555555555556e+08 (%g)

        break; case __LINE__: TRICE32( ID( 1005), "msg:%u (%%u)\n", -1 ); //exp: time:32323232msg:4294967295 (%u)
        break; case __LINE__: TRICE32( ID( 5165), "msg:%b (%%b)\n", -1 ); //exp: time:32323232msg:11111111111111111111111111111111 (%b)
        break; case __LINE__: TRICE32( ID( 5046), "msg:%o (%%o)\n", -1 ); //exp: time:32323232msg:37777777777 (%o)
        break; case __LINE__: TRICE32( ID( 4719), "msg:%O (%%O)\n", -1 ); //exp: time:32323232msg:0o37777777777 (%O)
        break; case __LINE__: TRICE32( ID( 3926), "msg:%X (%%X)\n", -1 ); //exp: time:32323232msg:FFFFFFFF (%X)
        break; case __LINE__: TRICE32( ID( 3944), "msg:%x (%%x)\n", -1 ); //exp: time:32323232msg:ffffffff (%x)
        break; case __LINE__: TRICE32( ID( 5699), "msg:%d (%%d)\n", -1 ); //exp: time:32323232msg:-1 (%d)
        break; case __LINE__: TRICE64( ID( 3022), "msg:%u (%%u)\n", -1 ); //exp: time:32323232msg:18446744073709551615 (%u)
        break; case __LINE__: TRICE64( ID( 3452), "msg:%b (%%b)\n", -1 ); //exp: time:32323232msg:1111111111111111111111111111111111111111111111111111111111111111 (%b)
        break; case __LINE__: TRICE64( ID( 4929), "msg:%o (%%o)\n", -1 ); //exp: time:32323232msg:1777777777777777777777 (%o)
        break; case __LINE__: TRICE64( ID( 7568), "msg:%O (%%O)\n", -1 ); //exp: time:32323232msg:0o1777777777777777777777 (%O)
        break; case __LINE__: TRICE64( ID( 5421), "msg:%X (%%X)\n", -1 ); //exp: time:32323232msg:FFFFFFFFFFFFFFFF (%X)
        break; case __LINE__: TRICE64( ID( 6296), "msg:%x (%%x)\n", -1 ); //exp: time:32323232msg:ffffffffffffffff (%x)

        //break; case __LINE__: TRice        ( iD( 2301), "value=%d\n", -2  ); //exp: time:32323232value=-2
        //break; case __LINE__: Trice        ( iD( 1173), "value=%d\n", -2  ); //exp: time:    1616value=-2
        //break; case __LINE__: trice        ( iD( 2349), "value=%d\n", -2  ); //exp: time:        value=-2
        break; case __LINE__: TRice8       ( iD( 5127), "value=%d\n", -2  ); //exp: time:32323232value=-2
        break; case __LINE__: Trice8       ( iD( 7499), "value=%d\n", -2  ); //exp: time:    1616value=-2
        break; case __LINE__: trice8       ( iD( 5079), "value=%d\n", -2  ); //exp: time:        value=-2
        break; case __LINE__: TRice16      ( iD( 5355), "value=%d\n", -2  ); //exp: time:32323232value=-2
        break; case __LINE__: Trice16      ( iD( 1455), "value=%d\n", -2  ); //exp: time:    1616value=-2
        break; case __LINE__: trice16      ( iD( 1644), "value=%d\n", -2  ); //exp: time:        value=-2
        break; case __LINE__: TRice32      ( iD( 6928), "value=%d\n", -2  ); //exp: time:32323232value=-2
        break; case __LINE__: Trice32      ( iD( 3377), "value=%d\n", -2  ); //exp: time:    1616value=-2
        break; case __LINE__: trice32      ( iD( 7458), "value=%d\n", -2  ); //exp: time:        value=-2
        break; case __LINE__: TRice64      ( iD( 4641), "value=%d\n", -2  ); //exp: time:32323232value=-2
        break; case __LINE__: Trice64      ( iD( 7740), "value=%d\n", -2  ); //exp: time:    1616value=-2
        break; case __LINE__: trice64      ( iD( 1925), "value=%d\n", -2  ); //exp: time:        value=-2
        
        //break; case __LINE__: TRice_1      ( iD( 6790), "value=%d\n", -2  ); //exp: time:32323232value=-2
        //break; case __LINE__: Trice_1      ( iD( 3042), "value=%d\n", -2  ); //exp: time:    1616value=-2
        //break; case __LINE__: trice_1      ( iD( 4842), "value=%d\n", -2  ); //exp: time:        value=-2
        //break; case __LINE__: TRice8_1     ( iD( 5102), "value=%d\n", -2  ); //exp: time:32323232value=-2
        //break; case __LINE__: Trice8_1     ( iD( 3670), "value=%d\n", -2  ); //exp: time:    1616value=-2
        //break; case __LINE__: trice8_1     ( iD( 6339), "value=%d\n", -2  ); //exp: time:        value=-2
        //break; case __LINE__: TRice16_1    ( iD( 5328), "value=%d\n", -2  ); //exp: time:32323232value=-2
        //break; case __LINE__: Trice16_1    ( iD( 2432), "value=%d\n", -2  ); //exp: time:    1616value=-2
        //break; case __LINE__: trice16_1    ( iD( 4596), "value=%d\n", -2  ); //exp: time:        value=-2
        //break; case __LINE__: TRice32_1    ( iD( 4905), "value=%d\n", -2  ); //exp: time:32323232value=-2
        //break; case __LINE__: Trice32_1    ( iD( 5128), "value=%d\n", -2  ); //exp: time:    1616value=-2
        //break; case __LINE__: trice32_1    ( iD( 4511), "value=%d\n", -2  ); //exp: time:        value=-2
        //break; case __LINE__: TRice64_1    ( iD( 3598), "value=%d\n", -2  ); //exp: time:32323232value=-2
        //break; case __LINE__: Trice64_1    ( iD( 7334), "value=%d\n", -2  ); //exp: time:    1616value=-2
        //break; case __LINE__: trice64_1    ( iD( 7788), "value=%d\n", -2  ); //exp: time:        value=-2
        
        //break; case __LINE__: TRice        ( iD( 7254), "no value" ); //exp: time:32323232no value
        //break; case __LINE__: Trice        ( iD( 1760), "no value" ); //exp: time:    1616no value
        //break; case __LINE__: trice        ( iD( 3212), "no value" ); //exp: time:        no value
        //break; case __LINE__: TRice8       ( iD( 1122), "no value" ); //exp: time:32323232no value
        //break; case __LINE__: Trice8       ( iD( 4599), "no value" ); //exp: time:    1616no value
        //break; case __LINE__: trice8       ( iD( 3620), "no value" ); //exp: time:        no value
        //break; case __LINE__: TRice16      ( iD( 3666), "no value" ); //exp: time:32323232no value
        //break; case __LINE__: Trice16      ( iD( 2841), "no value" ); //exp: time:    1616no value
        //break; case __LINE__: trice16      ( iD( 5917), "no value" ); //exp: time:        no value
        //break; case __LINE__: TRice32      ( iD( 2241), "no value" ); //exp: time:32323232no value
        //break; case __LINE__: Trice32      ( iD( 1726), "no value" ); //exp: time:    1616no value
        //break; case __LINE__: trice32      ( iD( 2407), "no value" ); //exp: time:        no value
        //break; case __LINE__: TRice64      ( iD( 4229), "no value" ); //exp: time:32323232no value
        //break; case __LINE__: Trice64      ( iD( 7674), "no value" ); //exp: time:    1616no value
        //break; case __LINE__: trice64      ( iD( 5289), "no value" ); //exp: time:        no value
        
        //break; case __LINE__: TRice_0      ( iD( 6629), "no value" ); //exp: time:32323232no value
        //break; case __LINE__: Trice_0      ( iD( 3135), "no value" ); //exp: time:    1616no value
        //break; case __LINE__: trice_0      ( iD( 3724), "no value" ); //exp: time:        no value
        //break; case __LINE__: TRice8_0     ( iD( 5124), "no value" ); //exp: time:32323232no value
        //break; case __LINE__: Trice8_0     ( iD( 4143), "no value" ); //exp: time:    1616no value
        //break; case __LINE__: trice8_0     ( iD( 2681), "no value" ); //exp: time:        no value
        //break; case __LINE__: TRice16_0    ( iD( 3329), "no value" ); //exp: time:32323232no value
        //break; case __LINE__: Trice16_0    ( iD( 1940), "no value" ); //exp: time:    1616no value
        //break; case __LINE__: trice16_0    ( iD( 4202), "no value" ); //exp: time:        no value
        //break; case __LINE__: TRice32_0    ( iD( 1830), "no value" ); //exp: time:32323232no value
        //break; case __LINE__: Trice32_0    ( iD( 7215), "no value" ); //exp: time:    1616no value
        //break; case __LINE__: trice32_0    ( iD( 5614), "no value" ); //exp: time:        no value
        //break; case __LINE__: TRice64_0    ( iD( 3264), "no value" ); //exp: time:32323232no value
        //break; case __LINE__: Trice64_0    ( iD( 4227), "no value" ); //exp: time:    1616no value
        //break; case __LINE__: trice64_0    ( iD( 7820), "no value" ); //exp: time:        no value

        //break; case __LINE__: TRice( iD( 1798),"info:This is a message with one value %d and a 32-bit stamp.\n", -2  ); //exp: time:32323232info:This is a message with one value -2 and a 32-bit stamp.
        //break; case __LINE__: Trice( iD( 2271),"info:This is a message with one value %d and a 16-bit stamp.\n", -2  ); //exp: time:    1616info:This is a message with one value -2 and a 16-bit stamp.
        //break; case __LINE__: trice( iD( 6013),"info:This is a message with one value %d and without stamp.\n" , -2  ); //exp: time:        info:This is a message with one value -2 and without stamp.

        break; case __LINE__: TRICE( ID( 7609), "info:This is a message without values and a 32-bit stamp.\n" );        //exp: time:32323232info:This is a message without values and a 32-bit stamp.
        break; case __LINE__: TRICE( Id( 6346), "info:This is a message without values and a 16-bit stamp.\n" );        //exp: time:    1616info:This is a message without values and a 16-bit stamp.
        break; case __LINE__: TRICE( id( 4346), "info:This is a message without values and without stamp.\n"  );        //exp: time:        info:This is a message without values and without stamp.

        break; case __LINE__: TRICE( ID( 3142),"info:This is a message with one value %d and a 32-bit stamp.\n", -2  ); //exp: time:32323232info:This is a message with one value -2 and a 32-bit stamp.
        break; case __LINE__: TRICE( Id( 1978),"info:This is a message with one value %d and a 16-bit stamp.\n", -2  ); //exp: time:    1616info:This is a message with one value -2 and a 16-bit stamp.
        break; case __LINE__: TRICE( id( 6431),"info:This is a message with one value %d and without stamp.\n" , -2  ); //exp: time:        info:This is a message with one value -2 and without stamp.

        break; case __LINE__: TRICE( ID( 2937),"info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 32-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 );   //exp: time:32323232info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and a 32-bit stamp.
        break; case __LINE__: TRICE( Id( 6237),"info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 16-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 );   //exp: time:    1616info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and a 16-bit stamp.
        break; case __LINE__: TRICE( id( 6283),"info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and without stamp.\n" , -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 );   //exp: time:        info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and without stamp.

        break; case __LINE__: TRICE32( ID( 7923),"info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 32-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //exp: time:32323232info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and a 32-bit stamp.
        break; case __LINE__: TRICE32( Id( 2822),"info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 16-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //exp: time:    1616info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and a 16-bit stamp.
        break; case __LINE__: TRICE32( id( 5263),"info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and without stamp.\n" , -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //exp: time:        info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and without stamp.

        break; case __LINE__: TRICE8   ( id( 4617), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: TRICE8_1 ( id( 2517), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: TRICE8   ( Id( 2322), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRICE8_1 ( Id( 2868), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRICE8   ( ID( 4082), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: TRICE8_1 ( ID( 5807), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: trice8   ( iD( 4097), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: trice8_1 ( iD( 4442), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: Trice8   ( iD( 2790), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: Trice8_1 ( iD( 7433), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRice8   ( iD( 2662), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: TRice8_1 ( iD( 4356), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200

        break; case __LINE__: TRICE16  ( id( 4098), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: TRICE16_1( id( 4358), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: TRICE16  ( Id( 7998), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRICE16_1( Id( 7005), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRICE16  ( ID( 3224), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: TRICE16_1( ID( 1322), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: trice16  ( iD( 7828), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: trice16_1( iD( 3602), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: Trice16  ( iD( 6300), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: Trice16_1( iD( 7489), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRice16  ( iD( 5167), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: TRice16_1( iD( 7252), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200

        break; case __LINE__: TRICE32  ( id( 7267), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: TRICE32_1( id( 4763), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: TRICE32  ( Id( 4611), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRICE32_1( Id( 2714), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRICE32  ( ID( 6859), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: TRICE32_1( ID( 6018), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: trice32  ( iD( 2046), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: trice32_1( iD( 1081), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: Trice32  ( iD( 4655), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: Trice32_1( iD( 7987), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRice32  ( iD( 7782), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: TRice32_1( iD( 6662), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200

        break; case __LINE__: TRICE64  ( id( 6819), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: TRICE64_1( id( 4993), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: TRICE64  ( Id( 2442), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRICE64_1( Id( 6211), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRICE64  ( ID( 6069), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: TRICE64_1( ID( 6522), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: trice64  ( iD( 1991), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: trice64_1( iD( 6717), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: Trice64  ( iD( 5705), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: Trice64_1( iD( 3390), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRice64  ( iD( 5955), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: TRice64_1( iD( 7866), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200

        // mixed 16-bit and 32-bit timestamp 
        break; case __LINE__: TRice8( iD( 4319), "msg:value=%d\n", -1 );                                                                                            //exp: time:32323232msg:value=-1
        break; case __LINE__: Trice8( iD( 3733), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    1616msg:value=-1, -2
        break; case __LINE__: trice8( iD( 5976), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        msg:value=-1, -2, -3
        break; case __LINE__: TRice8( iD( 5460), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:32323232msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice8( iD( 4002), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    1616msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice8( iD( 6684), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice8( iD( 3310), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice8( iD( 6427), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice8( iD( 3520), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice8( iD( 6541), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice8( iD( 2359), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice8( iD( 7486), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice16( iD( 4451), "msg:value=%d\n", -1 );                                                                                            //exp: time:32323232msg:value=-1
        break; case __LINE__: Trice16( iD( 3455), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    1616msg:value=-1, -2
        break; case __LINE__: trice16( iD( 4053), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        msg:value=-1, -2, -3
        break; case __LINE__: TRice16( iD( 3574), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:32323232msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice16( iD( 3989), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    1616msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice16( iD( 4060), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice16( iD( 7934), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice16( iD( 2268), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice16( iD( 7192), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice16( iD( 5320), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice16( iD( 4529), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice16( iD( 6842), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice32( iD( 6245), "msg:value=%d\n", -1 );                                                                                            //exp: time:32323232msg:value=-1
        break; case __LINE__: Trice32( iD( 5126), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    1616msg:value=-1, -2
        break; case __LINE__: trice32( iD( 5576), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        msg:value=-1, -2, -3
        break; case __LINE__: TRice32( iD( 6158), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:32323232msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice32( iD( 3821), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    1616msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice32( iD( 3213), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice32( iD( 5009), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice32( iD( 6817), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice32( iD( 3176), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice32( iD( 1469), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice32( iD( 5873), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice32( iD( 7910), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice64( iD( 3714), "msg:value=%d\n", -1 );                                                                                            //exp: time:32323232msg:value=-1
        break; case __LINE__: Trice64( iD( 6170), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    1616msg:value=-1, -2
        break; case __LINE__: trice64( iD( 3576), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        msg:value=-1, -2, -3
        break; case __LINE__: TRice64( iD( 4761), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:32323232msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice64( iD( 3879), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    1616msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice64( iD( 2014), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice64( iD( 1194), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice64( iD( 4085), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice64( iD( 2100), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice64( iD( 3945), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice64( iD( 4160), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice64( iD( 1393), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice8_1 ( iD( 1929), "msg:value=%d\n", -1 );                                                                                            //exp: time:32323232msg:value=-1
        break; case __LINE__: Trice8_2 ( iD( 1794), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    1616msg:value=-1, -2
        break; case __LINE__: trice8_3 ( iD( 3154), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        msg:value=-1, -2, -3
        break; case __LINE__: TRice8_4 ( iD( 1972), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:32323232msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice8_5 ( iD( 4492), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    1616msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice8_6 ( iD( 5284), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice8_7 ( iD( 5474), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice8_8 ( iD( 5846), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice8_9 ( iD( 1096), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice8_10( iD( 3524), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice8_11( iD( 6616), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice8_12( iD( 6370), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice16_1 ( iD( 5387), "msg:value=%d\n", -1 );                                                                                            //exp: time:32323232msg:value=-1
        break; case __LINE__: Trice16_2 ( iD( 1127), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    1616msg:value=-1, -2
        break; case __LINE__: trice16_3 ( iD( 2206), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        msg:value=-1, -2, -3
        break; case __LINE__: TRice16_4 ( iD( 5997), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:32323232msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice16_5 ( iD( 1255), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    1616msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice16_6 ( iD( 7063), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice16_7 ( iD( 5157), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice16_8 ( iD( 1656), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice16_9 ( iD( 3552), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice16_10( iD( 6623), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice16_11( iD( 1515), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice16_12( iD( 7641), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice32_1 ( iD( 6894), "msg:value=%d\n", -1 );                                                                                            //exp: time:32323232msg:value=-1
        break; case __LINE__: Trice32_2 ( iD( 5288), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    1616msg:value=-1, -2
        break; case __LINE__: trice32_3 ( iD( 5871), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        msg:value=-1, -2, -3
        break; case __LINE__: TRice32_4 ( iD( 7713), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:32323232msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice32_5 ( iD( 1104), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    1616msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice32_6 ( iD( 1921), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice32_7 ( iD( 1764), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice32_8 ( iD( 4697), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice32_9 ( iD( 4883), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice32_10( iD( 5816), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice32_11( iD( 5007), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice32_12( iD( 7815), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice64_1 ( iD( 7679), "msg:value=%d\n", -1 );                                                                                            //exp: time:32323232msg:value=-1
        break; case __LINE__: Trice64_2 ( iD( 7091), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    1616msg:value=-1, -2
        break; case __LINE__: trice64_3 ( iD( 7335), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        msg:value=-1, -2, -3
        break; case __LINE__: TRice64_4 ( iD( 5160), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:32323232msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice64_5 ( iD( 2801), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    1616msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice64_6 ( iD( 4244), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice64_7 ( iD( 4400), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice64_8 ( iD( 2582), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice64_9 ( iD( 1603), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice64_10( iD( 4645), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice64_11( iD( 5454), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice64_12( iD( 4714), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        // use cases for 8-bit data
        break; case __LINE__: trice8     ( iD( 7437), "value=%d\n", -2  ); //exp: time:        value=-2
        break; case __LINE__: Trice8     ( iD( 5228), "value=%d\n", -2  ); //exp: time:    1616value=-2
        break; case __LINE__: TRice8     ( iD( 5554), "value=%d\n", -2  ); //exp: time:32323232value=-2
        break; case __LINE__: trice8_1   ( iD( 3519), "value=%d\n", -2  ); //exp: time:        value=-2
        break; case __LINE__: Trice8_1   ( iD( 3850), "value=%d\n", -2  ); //exp: time:    1616value=-2
        break; case __LINE__: TRice8_1   ( iD( 7052), "value=%d\n", -2  ); //exp: time:32323232value=-2

        // use cases for 16-bit data
        break; case __LINE__: trice16    ( iD( 6493), "value=%d\n", -2  ); //exp: time:        value=-2
        break; case __LINE__: Trice16    ( iD( 4029), "value=%d\n", -2  ); //exp: time:    1616value=-2
        break; case __LINE__: TRice16    ( iD( 2395), "value=%d\n", -2  ); //exp: time:32323232value=-2
        break; case __LINE__: trice16_1  ( iD( 3765), "value=%d\n", -2  ); //exp: time:        value=-2
        break; case __LINE__: Trice16_1  ( iD( 3479), "value=%d\n", -2  ); //exp: time:    1616value=-2
        break; case __LINE__: TRice16_1  ( iD( 7228), "value=%d\n", -2  ); //exp: time:32323232value=-2

        // use cases for 32-bit data
        break; case __LINE__: trice32    ( iD( 5414), "value=%d\n", -2  ); //exp: time:        value=-2
        break; case __LINE__: Trice32    ( iD( 3530), "value=%d\n", -2  ); //exp: time:    1616value=-2
        break; case __LINE__: TRice32    ( iD( 6957), "value=%d\n", -2  ); //exp: time:32323232value=-2
        break; case __LINE__: trice32_1  ( iD( 1230), "value=%d\n", -2  ); //exp: time:        value=-2
        break; case __LINE__: Trice32_1  ( iD( 7553), "value=%d\n", -2  ); //exp: time:    1616value=-2
        break; case __LINE__: TRice32_1  ( iD( 1277), "value=%d\n", -2  ); //exp: time:32323232value=-2

        // use cases for 64-bit data
        break; case __LINE__: trice64    ( iD( 1069), "value=%d\n", -2  ); //exp: time:        value=-2
        break; case __LINE__: Trice64    ( iD( 2531), "value=%d\n", -2  ); //exp: time:    1616value=-2
        break; case __LINE__: TRice64    ( iD( 1406), "value=%d\n", -2  ); //exp: time:32323232value=-2
        break; case __LINE__: trice64_1  ( iD( 3962), "value=%d\n", -2  ); //exp: time:        value=-2
        break; case __LINE__: Trice64_1  ( iD( 3425), "value=%d\n", -2  ); //exp: time:    1616value=-2
        break; case __LINE__: TRice64_1  ( iD( 2507), "value=%d\n", -2  ); //exp: time:32323232value=-2

        // legacy use cases
        break; case __LINE__: TRICE      ( ID( 4586),        "value=%d\n", -2 ); //exp: time:32323232value=-2
        break; case __LINE__: TRICE      ( id( 3193), "value=%d\n", -2 ); //exp: time:        value=-2
        break; case __LINE__: TRICE      ( Id( 4495), "value=%d\n", -2 ); //exp: time:    1616value=-2
        break; case __LINE__: TRICE      ( ID( 7168), "value=%d\n", -2 ); //exp: time:32323232value=-2

        // default stamp
        break; case __LINE__: TRICE8     ( ID( 7817),        "value=%d\n", -2 ); //exp: time:32323232value=-2
        break; case __LINE__: TRICE8_1   ( ID( 5339),        "value=%d\n", -2 ); //exp: time:32323232value=-2

        // no stamp
        break; case __LINE__: trice8     ( iD( 4732),        "value=%d\n", -2 ); //exp: time:        value=-2
        break; case __LINE__: trice8_1   ( iD( 3729),        "value=%d\n", -2 ); //exp: time:        value=-2
        break; case __LINE__: TRICE8     ( id( 4051), "value=%d\n", -2 ); //exp: time:        value=-2
        break; case __LINE__: TRICE8_1   ( id( 1421), "value=%d\n", -2 ); //exp: time:        value=-2

        // 16-bit stamp:
        break; case __LINE__: Trice8     ( iD( 3484),        "value=%d\n", -2 ); //exp: time:    1616value=-2
        break; case __LINE__: Trice8_1   ( iD( 5678),        "value=%d\n", -2 ); //exp: time:    1616value=-2
        break; case __LINE__: TRICE8     ( Id( 1608), "value=%d\n", -2 ); //exp: time:    1616value=-2
        break; case __LINE__: TRICE8_1   ( Id( 6400), "value=%d\n", -2 ); //exp: time:    1616value=-2

        // 32-bit stamp:
        break; case __LINE__: TRice8     ( iD( 2769),        "value=%d\n", -2 ); //exp: time:32323232value=-2
        break; case __LINE__: TRice8_1   ( iD( 2651),        "value=%d\n", -2 ); //exp: time:32323232value=-2
        break; case __LINE__: TRICE8     ( ID( 4624), "value=%d\n", -2 ); //exp: time:32323232value=-2
        break; case __LINE__: TRICE8_1   ( ID( 2779), "value=%d\n", -2 ); //exp: time:32323232value=-2

        break; case __LINE__: TRICE( ID( 7566), "FATAL:magenta+b:red\n" );           // exp: time:32323232FATAL:magenta+b:red
        break; case __LINE__: TRICE( ID( 6963), "CRITICAL:red+i:default+h\n" );      // exp: time:32323232CRITICAL:red+i:default+h
        break; case __LINE__: TRICE( ID( 6402), "EMERGENCY:red+i:blue\n" );          // exp: time:32323232EMERGENCY:red+i:blue
        break; case __LINE__: TRICE( ID( 6971), "ERROR:11:red\n" );                  // exp: time:32323232ERROR:11:red
        break; case __LINE__: TRICE( ID( 5077), "WARNING:11+i:red\n" );              // exp: time:32323232WARNING:11+i:red
        break; case __LINE__: TRICE( ID( 1400), "ATTENTION:11:green\n" );            // exp: time:32323232ATTENTION:11:green
        break; case __LINE__: TRICE( ID( 6257), "INFO:cyan+b:default+h\n" );         // exp: time:32323232INFO:cyan+b:default+h
        break; case __LINE__: TRICE( ID( 4177), "DEBUG:130+i\n" );                   // exp: time:32323232DEBUG:130+i
        break; case __LINE__: TRICE( ID( 4154), "TRACE:default+i:default+h\n" );     // exp: time:32323232TRACE:default+i:default+h
        break; case __LINE__: TRICE( ID( 5645), "TIME:blue+i:blue+h\n" );            // exp: time:32323232TIME:blue+i:blue+h
        break; case __LINE__: TRICE( ID( 6291), "MESSAGE:green+h:black\n" );         // exp: time:32323232MESSAGE:green+h:black
        break; case __LINE__: TRICE( ID( 5770), "READ:black+i:yellow+h\n" );         // exp: time:32323232READ:black+i:yellow+h
        break; case __LINE__: TRICE( ID( 2425), "WRITE:black+u:yellow+h\n" );        // exp: time:32323232WRITE:black+u:yellow+h
        break; case __LINE__: TRICE( ID( 6088), "RECEIVE:black+h:black\n" );         // exp: time:32323232RECEIVE:black+h:black
        break; case __LINE__: TRICE( ID( 6668), "TRANSMIT:black:black+h\n" );        // exp: time:32323232TRANSMIT:black:black+h
        break; case __LINE__: TRICE( ID( 5574), "DIAG:yellow+i:default+h\n" );       // exp: time:32323232DIAG:yellow+i:default+h
        break; case __LINE__: TRICE( ID( 6074), "INTERRUPT:magenta+i:default+h\n" ); // exp: time:32323232INTERRUPT:magenta+i:default+h
        break; case __LINE__: TRICE( ID( 7490), "SIGNAL:118+i\n" );                  // exp: time:32323232SIGNAL:118+i
        break; case __LINE__: TRICE( ID( 2770), "TEST:yellow+h:black\n" );           // exp: time:32323232TEST:yellow+h:black
        break; case __LINE__: TRICE( ID( 1670), "DEFAULT:off\n" );                   // exp: time:32323232DEFAULT:off
        break; case __LINE__: TRICE( ID( 1846), "NOTICE:blue:white+h\n" );           // exp: time:32323232NOTICE:blue:white+h
        break; case __LINE__: TRICE( ID( 6590), "ALERT:magenta:magenta+h\n" );       // exp: time:32323232ALERT:magenta:magenta+h
        break; case __LINE__: TRICE( ID( 2673), "ASSERT:yellow+i:blue\n" );          // exp: time:32323232ASSERT:yellow+i:blue
        break; case __LINE__: TRICE( ID( 4701), "ALARM:red+i:white+h\n" );           // exp: time:32323232ALARM:red+i:white+h
        break; case __LINE__: TRICE( ID( 6665), "CYCLE:blue+i:default+h\n" );        // exp: time:32323232CYCLE:blue+i:default+h
        break; case __LINE__: TRICE( ID( 7688), "VERBOSE:blue:default\n" );          // exp: time:32323232VERBOSE:blue:default
        break; case __LINE__: TRICE( ID( 6267), "fatal:magenta+b:red\n" );           // exp: time:32323232fatal:magenta+b:red
        break; case __LINE__: TRICE( ID( 3905), "critical:red+i:default+h\n" );      // exp: time:32323232critical:red+i:default+h
        break; case __LINE__: TRICE( ID( 5836), "emergency:red+i:blue\n" );          // exp: time:32323232emergency:red+i:blue
        break; case __LINE__: TRICE( ID( 1581), "error:11:red\n" );                  // exp: time:32323232error:11:red
        break; case __LINE__: TRICE( ID( 4409), "warning:11+i:red\n" );              // exp: time:32323232warning:11+i:red
        break; case __LINE__: TRICE( ID( 1766), "attention:11:green\n" );            // exp: time:32323232attention:11:green
        break; case __LINE__: TRICE( ID( 5609), "info:cyan+b:default+h\n" );         // exp: time:32323232info:cyan+b:default+h
        break; case __LINE__: TRICE( ID( 2123), "debug:130+i\n" );                   // exp: time:32323232debug:130+i
        break; case __LINE__: TRICE( ID( 5069), "trace:default+i:default+h\n" );     // exp: time:32323232trace:default+i:default+h
        break; case __LINE__: TRICE( ID( 5135), "time:blue+i:blue+h\n" );            // exp: time:32323232time:blue+i:blue+h
        break; case __LINE__: TRICE( ID( 6374), "message:green+h:black\n" );         // exp: time:32323232message:green+h:black
        break; case __LINE__: TRICE( ID( 2627), "read:black+i:yellow+h\n" );         // exp: time:32323232read:black+i:yellow+h
        break; case __LINE__: TRICE( ID( 3373), "write:black+u:yellow+h\n" );        // exp: time:32323232write:black+u:yellow+h
        break; case __LINE__: TRICE( ID( 1259), "receive:black+h:black\n" );         // exp: time:32323232receive:black+h:black
        break; case __LINE__: TRICE( ID( 6041), "transmit:black:black+h\n" );        // exp: time:32323232transmit:black:black+h
        break; case __LINE__: TRICE( ID( 2176), "diag:yellow+i:default+h\n" );       // exp: time:32323232diag:yellow+i:default+h
        break; case __LINE__: TRICE( ID( 3128), "interrupt:magenta+i:default+h\n" ); // exp: time:32323232interrupt:magenta+i:default+h
        break; case __LINE__: TRICE( ID( 3980), "signal:118+i\n" );                  // exp: time:32323232signal:118+i
        break; case __LINE__: TRICE( ID( 5500), "test:yellow+h:black\n" );           // exp: time:32323232test:yellow+h:black
        break; case __LINE__: TRICE( ID( 6413), "default:off\n" );                   // exp: time:32323232default:off
        break; case __LINE__: TRICE( ID( 1213), "notice:blue:white+h\n" );           // exp: time:32323232notice:blue:white+h
        break; case __LINE__: TRICE( ID( 7787), "alert:magenta:magenta+h\n" );       // exp: time:32323232alert:magenta:magenta+h
        break; case __LINE__: TRICE( ID( 7308), "assert:yellow+i:blue\n" );          // exp: time:32323232assert:yellow+i:blue
        break; case __LINE__: TRICE( ID( 7638), "alarm:red+i:white+h\n" );           // exp: time:32323232alarm:red+i:white+h
        break; case __LINE__: TRICE( ID( 3761), "cycle:blue+i:default+h\n" );        // exp: time:32323232cycle:blue+i:default+h
        break; case __LINE__: TRICE( ID( 2580), "verbose:blue:default\n" );          // exp: time:32323232verbose:blue:default
        break; case __LINE__: TRICE64( id( 1907), "msg:%d (%%d)\n", -1 );     //exp: time:        msg:-1 (%d)

        break; case __LINE__: TRICE( ID( 3726), "sig:TRICE with 1 to 12 values\n" );
        break; case __LINE__: TRICE( ID( 1577), "rd:TRICE %d\n", -1 );
        break; case __LINE__: TRICE( ID( 1338), "rd:TRICE %d, %d\n", -1, -2 );
        break; case __LINE__: TRICE( ID( 3988), "rd:TRICE %d, %d, %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE( ID( 1280), "rd:TRICE %d, %d, %d, %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE( ID( 1668), "rd:TRICE %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE( ID( 2437), "rd:TRICE %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE( ID( 5993), "rd:TRICE %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE( ID( 5220), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE( ID( 3796), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE( ID( 3076), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE( ID( 5090), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE( ID( 7270), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE( ID( 2680),  "sig:TRICE_n with 1 to 12 values\n" );
        break; case __LINE__: TRICE_1( ID( 7302), "rd:TRICE_1 %d\n", 1 );
        break; case __LINE__: TRICE_2( ID( 4382), "rd:TRICE_2 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE_3( ID( 7209), "rd:TRICE_3 %d, %d, %d\n", 1, 2, 3 );
        break; case __LINE__: TRICE_4( ID( 5085), "rd:TRICE_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
        break; case __LINE__: TRICE_5( ID( 1566), "rd:TRICE_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
        break; case __LINE__: TRICE_6( ID( 1920), "rd:TRICE_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
        break; case __LINE__: TRICE_7( ID( 6466), "rd:TRICE_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
        break; case __LINE__: TRICE_8( ID( 3599), "rd:TRICE_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
        break; case __LINE__: TRICE_9( ID( 3728), "rd:TRICE_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
        break; case __LINE__: TRICE_10( ID( 7108), "rd:TRICE_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
        break; case __LINE__: TRICE_11( ID( 4867), "rd:TRICE_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
        break; case __LINE__: TRICE_12( ID( 1854), "rd:TRICE_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );				

        break; case __LINE__: TRICE( ID( 4104), "sig:TRICE8 with 1 to 12 values\n" );
        break; case __LINE__: TRICE8( ID( 3194), "rd:TRICE8 %d\n", -1 );
        break; case __LINE__: TRICE8( ID( 1216), "rd:TRICE8 %d, %d\n", -1, -2 );
        break; case __LINE__: TRICE8( ID( 7053), "rd:TRICE8 %d, %d, %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE8( ID( 5906), "rd:TRICE8 %d, %d, %d, %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE8( ID( 2005), "rd:TRICE8 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE8( ID( 4606), "rd:TRICE8 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE8( ID( 7137), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE8( ID( 6519), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE8( ID( 3077), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE8( ID( 3802), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE8( ID( 3183), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE8( ID( 3887), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE( ID( 3566), "sig:TRICE8_n with 1 to 12 values\n" );
        break; case __LINE__: TRICE8_1 ( ID( 5901), "rd:TRICE8_1 %d\n", 1 );
        break; case __LINE__: TRICE8_2 ( ID( 6067), "rd:TRICE8_2 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE8_3 ( ID( 4971), "rd:TRICE8_3 %d, %d, %d\n", 1, 2, 3 );
        break; case __LINE__: TRICE8_4 ( ID( 7718), "rd:TRICE8_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
        break; case __LINE__: TRICE8_5 ( ID( 6524), "rd:TRICE8_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
        break; case __LINE__: TRICE8_6 ( ID( 1394), "rd:TRICE8_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
        break; case __LINE__: TRICE8_7 ( ID( 4313), "rd:TRICE8_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
        break; case __LINE__: TRICE8_8 ( ID( 4861), "rd:TRICE8_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
        break; case __LINE__: TRICE8_9 ( ID( 6338), "rd:TRICE8_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
        break; case __LINE__: TRICE8_10( ID( 3026), "rd:TRICE8_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
        break; case __LINE__: TRICE8_11( ID( 2827), "rd:TRICE8_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
        break; case __LINE__: TRICE8_12( ID( 2154), "rd:TRICE8_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );

        break; case __LINE__: TRICE( ID( 1295), "sig:TRICE16 with 1 to 12 values\n" );
        break; case __LINE__: TRICE16( ID( 6865), "rd:TRICE16 %d\n", -1 );
        break; case __LINE__: TRICE16( ID( 2465), "rd:TRICE16 %d, %d\n", -1, -2 );
        break; case __LINE__: TRICE16( ID( 5236), "rd:TRICE16 %d, %d, %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE16( ID( 5800), "rd:TRICE16 %d, %d, %d, %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE16( ID( 5622), "rd:TRICE16 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE16( ID( 6898), "rd:TRICE16 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE16( ID( 5795), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE16( ID( 2577), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE16( ID( 2628), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE16( ID( 3029), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE16( ID( 5338), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE16( ID( 1186), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE     ( ID( 5066), "sig:TRICE16_n with 1 to 12 values\n" );
        break; case __LINE__: TRICE16_1 ( ID( 4806), "rd:TRICE16_1 %d\n", 1 );
        break; case __LINE__: TRICE16_2 ( ID( 2834), "rd:TRICE16_2 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE16_3 ( ID( 6196), "rd:TRICE16_3 %d, %d, %d\n", 1, 2, 3 );
        break; case __LINE__: TRICE16_4 ( ID( 1140), "rd:TRICE16_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
        break; case __LINE__: TRICE16_5 ( ID( 5717), "rd:TRICE16_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
        break; case __LINE__: TRICE16_6 ( ID( 7185), "rd:TRICE16_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
        break; case __LINE__: TRICE16_7 ( ID( 7018), "rd:TRICE16_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
        break; case __LINE__: TRICE16_8 ( ID( 5322), "rd:TRICE16_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
        break; case __LINE__: TRICE16_9 ( ID( 2450), "rd:TRICE16_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
        break; case __LINE__: TRICE16_10( ID( 2304), "rd:TRICE16_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
        break; case __LINE__: TRICE16_11( ID( 3528), "rd:TRICE16_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
        break; case __LINE__: TRICE16_12( ID( 7900), "rd:TRICE16_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );

        break; case __LINE__: TRICE  ( ID( 6975), "sig:TRICE16 with 1 to 12 pointer\n" );
        break; case __LINE__: TRICE16( ID( 2958), "rd:TRICE16 %p\n", -1 );
        break; case __LINE__: TRICE16( ID( 7147), "rd:TRICE16 %p, %p\n", -1, -2 );
        break; case __LINE__: TRICE16( ID( 4046), "rd:TRICE16 %p, %p, %p\n", -1, -2, -3 );
        break; case __LINE__: TRICE16( ID( 3095), "rd:TRICE16 %p, %p, %p, %p\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE16( ID( 3178), "rd:TRICE16 %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE16( ID( 6925), "rd:TRICE16 %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE16( ID( 2158), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE16( ID( 2368), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE16( ID( 5872), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE16( ID( 7974), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE16( ID( 6457), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE16( ID( 7293), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE  ( ID( 5768), "sig:TRICE16 with 1 to 12 hex\n" );
        break; case __LINE__: TRICE16( ID( 5604), "rd:TRICE16 %X\n", -1 );
        break; case __LINE__: TRICE16( ID( 5436), "rd:TRICE16 %X, %X\n", -1, -2 );
        break; case __LINE__: TRICE16( ID( 3932), "rd:TRICE16 %X, %X, %X\n", -1, -2, -3 );
        break; case __LINE__: TRICE16( ID( 2414), "rd:TRICE16 %X, %X, %X, %X\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE16( ID( 4722), "rd:TRICE16 %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE16( ID( 4819), "rd:TRICE16 %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE16( ID( 6810), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE16( ID( 4081), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE16( ID( 1415), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE16( ID( 4135), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE16( ID( 6282), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE16( ID( 5857), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE  ( ID( 1957), "sig:TRICE32 with 1 to 12 values\n" );
        break; case __LINE__: TRICE32( ID( 6509), "rd:TRICE32 %d\n", -1 );
        break; case __LINE__: TRICE32( ID( 1897), "rd:TRICE32 %d, %d\n", -1, -2 );
        break; case __LINE__: TRICE32( ID( 6498), "rd:TRICE32 %d, %d, %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE32( ID( 3816), "rd:TRICE32 %d, %d, %d, %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE32( ID( 4758), "rd:TRICE32 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE32( ID( 1026), "rd:TRICE32 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE32( ID( 6770), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE32( ID( 3017), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE32( ID( 1019), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE32( ID( 1905), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE32( ID( 1732), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE32( ID( 3597), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE     ( ID( 2549), "signal:TRICE32_n with 1 to 12 values\n" );
        break; case __LINE__: TRICE32_1 ( ID( 3697), "rd:TRICE32_1 %d\n", 1 );
        break; case __LINE__: TRICE32_2 ( ID( 4813), "rd:TRICE32_2 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE32_3 ( ID( 6534), "rd:TRICE32_3 %d, %d, %d\n", 1, 2, 3 );
        break; case __LINE__: TRICE32_4 ( ID( 3833), "rd:TRICE32_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
        break; case __LINE__: TRICE32_5 ( ID( 5163), "rd:TRICE32_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
        break; case __LINE__: TRICE32_6 ( ID( 4790), "rd:TRICE32_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
        break; case __LINE__: TRICE32_7 ( ID( 7574), "rd:TRICE32_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
        break; case __LINE__: TRICE32_8 ( ID( 7833), "rd:TRICE32_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
        break; case __LINE__: TRICE32_9 ( ID( 7918), "rd:TRICE32_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
        break; case __LINE__: TRICE32_10( ID( 5441), "rd:TRICE32_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
        break; case __LINE__: TRICE32_11( ID( 6730), "rd:TRICE32_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
        break; case __LINE__: TRICE32_12( ID( 1092), "rd:TRICE32_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );

        break; case __LINE__: TRICE  ( ID( 7639), "sig:TRICE64 with 1 to 12 values\n" );
        break; case __LINE__: TRICE64( ID( 3359), "rd:TRICE64 %d\n", -1 );
        break; case __LINE__: TRICE64( ID( 1174), "rd:TRICE64 %d, %d\n", -1, -2 );
        break; case __LINE__: TRICE64( ID( 1843), "rd:TRICE64 %d, %d, %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE64( ID( 1561), "rd:TRICE64 %d, %d, %d, %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE64( ID( 7821), "rd:TRICE64 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE64( ID( 7750), "rd:TRICE64 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE64( ID( 1734), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE64( ID( 2915), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE64( ID( 4185), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE64( ID( 3080), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE64( ID( 3756), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE64( ID( 3867), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE     ( ID( 4538), "sig:TRICE64_n with 1 to 12 values\n" );
        break; case __LINE__: TRICE64_1 ( ID( 1334), "rd:TRICE64_1 %d\n", 1 );
        break; case __LINE__: TRICE64_2 ( ID( 7086), "rd:TRICE64_2 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE64_2 ( ID( 6585), "rd:TRICE64_2 %x, %x\n", 0x1122334455667788, 0x0102030405060708 );
        break; case __LINE__: TRICE64_3 ( ID( 2654), "rd:TRICE64_3 %d, %d, %d\n", 1, 2, 3 );
        break; case __LINE__: TRICE64_4 ( ID( 6807), "rd:TRICE64_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
        break; case __LINE__: TRICE64_5 ( ID( 2584), "rd:TRICE64_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
        break; case __LINE__: TRICE64_6 ( ID( 6849), "rd:TRICE64_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
        break; case __LINE__: TRICE64_7 ( ID( 3855), "rd:TRICE64_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
        break; case __LINE__: TRICE64_8 ( ID( 4815), "rd:TRICE64_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
        break; case __LINE__: TRICE64_9 ( ID( 2850), "rd:TRICE64_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
        break; case __LINE__: TRICE64_10( ID( 7000), "rd:TRICE64_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
        break; case __LINE__: TRICE64_11( ID( 1208), "rd:TRICE64_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
        break; case __LINE__: TRICE64_12( ID( 4332), "rd:TRICE64_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );

        break; case __LINE__: TRICE( ID( 2719), "sig:Colors and ticks\n" );
        break; case __LINE__: TRICE( ID( 1692), "--------------------------------------------------\n");
        break; case __LINE__: TRICE( ID( 7549), "--------------------------------------------------\n");
        break; case __LINE__: TRICE( ID( 4493), "dbg:12345 as 16bit is %016b\n", 12345);
        break; case __LINE__: TRICE( ID( 4099), "--------------------------------------------------\n");
        break; case __LINE__: TRICE( ID( 4266), "sig:This ASSERT error is just a demo and no real error:\n");
        break; case __LINE__: TRICE( ID( 7706), "--------------------------------------------------\n");
        break; case __LINE__: TRICE( ID( 6312), "ERR:error       message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 6285), "WRN:warning     message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 2281), "ATT:attention   message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 7014), "DIA:diagnostics message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 2401), "TIM:timing      message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 3384), "DBG:debug       message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 7877), "SIG:signal      message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 3216), "RD:read         message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 4405), "WR:write        message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 2198), "ISR:interrupt   message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 1651), "MSG:normal      message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 6426), "INFO:informal   message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 7402), "time:TRICE32_1  message, SysTick is %6u\n", SYSTICKVAL);

        break; case __LINE__: TRICE( ID( 6675), "sig:Various ranges and formats\n" );
        break; case __LINE__: TRICE8 ( ID( 4946), "tst:TRICE8  %%03x -> %03x  %03x  %03x  %03x\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID( 7487), "tst:TRICE8   %%4d -> %4d %4d %4d %4d\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID( 5265), "tst:TRICE8   %%4u -> %4u %4u %4u %4u\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID( 5440), "tst:TRICE8   %%4o -> %4o %4o %4o %4o\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID( 3637), "tst:TRICE8   %%4O -> %4O %4O %4O %4O\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID( 6587), "tst:TRICE8   %%4X -> %4X %4X %4X %4X\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID( 5821), "tst:TRICE8   %%4x -> %4x %4x %4x %4x\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID( 5897), "tst:TRICE8   %%8b -> %8b %8b %8b %8b\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID( 5680), "tst:TRICE8  %%08b -> %08b %08b %08b %08b\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE16( ID( 7488), "tst:TRICE16  %%05x ->   %05x   %05x   %05x   %05x\n", 1, 0x7fff, 0x8000, 0xffff);
        break; case __LINE__: TRICE16( ID( 6191), "tst:TRICE16   %%6d ->  %6d  %6d  %6d  %6d\n",         1, 0x7fff, 0x8000, 0xffff);
        break; case __LINE__: TRICE16( ID( 7624), "tst:TRICE16   %%7o -> %7o %7o %7o %7o\n",             1, 0x7fff, 0x8000, 0xffff);
        break; case __LINE__: TRICE32( ID( 3315), "tst:TRICE32 %%09x ->      %09x      %09x       %09x     %09x\n", 1, 0x7fffffff, 0x80000000, 0xffffffff);
        break; case __LINE__: TRICE32( ID( 6411), "tst:TRICE32 %%10d ->     %10d     %10d     %10d    %10x\n",      1, 0x7fffffff, 0x80000000, 0xffffffff);
        break; case __LINE__: TRICE32( ID( 5497), "att:TRICE32 %40b\n", 0xAAAAAAAA);
        break; case __LINE__: TRICE64( ID( 2611), "tst:TRICE64 %%09x ->      %09x      %09x       %09x     %09x\n", 1, 0x7fffffffffffffff, 0x8000000000000000, 0xffffffffffffffff);
        break; case __LINE__: TRICE64( ID( 6478), "tst:TRICE64 %%10d ->     %10d     %10d     %10d    %10x\n",      1, 0x7fffffffffffffff, 0x8000000000000000, 0xffffffffffffffff);
        break; case __LINE__: TRICE64( ID( 1801), "att:TRICE64 %70b\n", 0xAAAAAAAAAAAAAAAA);
        break; case __LINE__: TRICE( ID( 5567), "sig:Legacy TRICE8\n" );
        break; case __LINE__: TRICE8_1( ID( 1946), "tst:TRICE8_1 %02x\n", 0xA1);
        break; case __LINE__: TRICE8_2( ID( 5956), "tst:TRICE8_2 %02x %02x\n", 0xA1, 0xA2);
        break; case __LINE__: TRICE8_3( ID( 5783), "tst:TRICE8_3 %02x %02x %02x\n", 0xA1, 0xA2, 0xA3);
        break; case __LINE__: TRICE8_4( ID( 2716), "tst:TRICE8_4 %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4);
        break; case __LINE__: TRICE8_5( ID( 1442), "tst:TRICE8_5 %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5 );
        break; case __LINE__: TRICE8_6( ID( 6320), "tst:TRICE8_6 %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6);
        break; case __LINE__: TRICE8_7( ID( 2792), "tst:TRICE8_7 %02x %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7);
        break; case __LINE__: TRICE8_8( ID( 6674), "tst:TRICE8_8 %02x %02x %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8);
        break; case __LINE__: TRICE8_1( ID( 6476), "tst:TRICE8_1 %d\n",                      -1);
        break; case __LINE__: TRICE8_2( ID( 1222), "tst:TRICE8_2 %d %d\n",                   -1, -2);
        break; case __LINE__: TRICE8_3( ID( 1841), "tst:TRICE8_3 %d %d %d\n",                -1, -2, -3);
        break; case __LINE__: TRICE8_4( ID( 5438), "tst:TRICE8_4 %d %d %d %d\n",             -1, -2, -3, -4);
        break; case __LINE__: TRICE8_5( ID( 3332), "tst:TRICE8_5 %d %d %d %d %d\n",          -1, -2, -3, -4, -5);
        break; case __LINE__: TRICE8_6( ID( 4410), "tst:TRICE8_6 %d %d %d %d %d %d\n",       -1, -2, -3, -4, -5, -6);
        break; case __LINE__: TRICE8_7( ID( 6344), "tst:TRICE8_7 %d %d %d %d %d %d %d\n",    -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE8_8( ID( 1266), "tst:TRICE8_8 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8);
        break; case __LINE__: TRICE8_1( ID( 2132), "tst:TRICE8_1 %d\n", 1);
        break; case __LINE__: TRICE8_2( ID( 3804), "tst:TRICE8_2 %d %d\n", 1, 2);
        break; case __LINE__: TRICE8_3( ID( 1605), "tst:TRICE8_3 %d %d %d\n", 1, 2, 3);
        break; case __LINE__: TRICE8_4( ID( 6277), "tst:TRICE8_4 %d %d %d %d\n", 1, 2, 3, 4);
        break; case __LINE__: TRICE8_5( ID( 4549), "tst:TRICE8_5 %d %d %d %d %d\n", 1, 2, 3, 4, 5);
        break; case __LINE__: TRICE8_6( ID( 5700), "tst:TRICE8_6 %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6);
        break; case __LINE__: TRICE8_7( ID( 4837), "tst:TRICE8_7 %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7);
        break; case __LINE__: TRICE8_8( ID( 6982), "tst:TRICE8_8 %d %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7, 8);
        break; case __LINE__: TRICE8_1( ID( 6127), "tst:TRICE8_1 %u\n", 201);
        break; case __LINE__: TRICE8_2( ID( 7392), "tst:TRICE8_2 %u %u\n", 201, 202);
        break; case __LINE__: TRICE8_3( ID( 5139), "tst:TRICE8_3 %u %u %u\n", 201, 202, 203);
        break; case __LINE__: TRICE8_4( ID( 6112), "tst:TRICE8_4 %u %u %u %u\n", 201, 202, 203, 204);
        break; case __LINE__: TRICE8_5( ID( 7162), "tst:TRICE8_5 %u %u %u %u %u\n", 201, 202, 203, 204, 205);
        break; case __LINE__: TRICE8_6( ID( 4019), "tst:TRICE8_6 %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206);
        break; case __LINE__: TRICE8_7( ID( 4321), "tst:TRICE8_7 %u %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206, 207);
        break; case __LINE__: TRICE8_8( ID( 1783), "tst:TRICE8_8 %u %u %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206, 207, 208);
        break; case __LINE__: TRICE8_8( ID( 2201), "tst:TRICE8_1 %%d=%d, %%u=%u, 0x%%x=0x%x, 0x%%2x=0x%2x, 0x%%02x=0x%02x, 0x%%3x=0x%3x, 0x%%03x=0x%03x, %%b=%b\n", 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81);

        break; case __LINE__: TRICE    ( ID( 2445), "sig:Legacy TRICE16-64\n" );
        break; case __LINE__: TRICE16_1( ID( 3031), "tst:TRICE16_1 %d\n", -111);
        break; case __LINE__: TRICE16_2( ID( 5425), "tst:TRICE16_2 %d %d\n", -111, -222);
        break; case __LINE__: TRICE16_3( ID( 2971), "tst:TRICE16_3 %d %d %d\n", -111, -222, -333);
        break; case __LINE__: TRICE16_4( ID( 3148), "tst:TRICE16_4 %d %d %d %d\n", -111, -222, -333, -444);
        break; case __LINE__: TRICE32_1( ID( 2836), "tst:TRICE32_1 %08x\n", 0x0123cafe);
        break; case __LINE__: TRICE32_1( ID( 2743), "tst:TRICE32_1 %d\n", -111);
        break; case __LINE__: TRICE32_2( ID( 1969), "tst:TRICE32_2 %x %x\n", -111, -222);
        break; case __LINE__: TRICE32_2( ID( 2876), "tst:TRICE32_2 %d %d\n", -111, -222);
        break; case __LINE__: TRICE32_3( ID( 4981), "tst:TRICE32_3 %x %x %x\n", -111, -222, -333);
        break; case __LINE__: TRICE32_3( ID( 7731), "tst:TRICE32_3 %d %d %d\n", -111, -222, -333);
        break; case __LINE__: TRICE32_4( ID( 5424), "tst:TRICE32_4 %x %x %x %x\n", -111, -222, -333, -444);
        break; case __LINE__: TRICE32_4( ID( 3087), "tst:TRICE32_4 %d %d %d %d\n", -111, -222, -333, -444);
        break; case __LINE__: TRICE64_1( ID( 5896), "tst:TRICE64_1 %d\n", -111);
        break; case __LINE__: TRICE64_2( ID( 4115), "tst:TRICE64_2 %d %d\n", -111, -222);
        break; case __LINE__: TRICE16_1( ID( 6032), "tst:TRICE16_1 %u\n", 60001);
        break; case __LINE__: TRICE16_2( ID( 7523), "tst:TRICE16_2 %u %u\n", 60001, 60002);
        break; case __LINE__: TRICE16_3( ID( 1865), "tst:TRICE16_3 %u %u %u\n", 60001, 60002, 60003);
        break; case __LINE__: TRICE16_4( ID( 2494), "tst:TRICE16_4 %u %u %u %u\n", 60001, 60002, 60003, 60004);
        break; case __LINE__: TRICE32_1( ID( 4708), "tst:TRICE32_1 %u\n", 4000000001);
        break; case __LINE__: TRICE32_2( ID( 4730), "tst:TRICE32_2 %u %u\n", 4000000001, 4000000002);
        break; case __LINE__: TRICE32_3( ID( 3286), "tst:TRICE32_3 %u %u %u\n", 4000000001, 4000000002, 4000000003);
        break; case __LINE__: TRICE32_4( ID( 7169), "tst:TRICE32_4 %u %u %u %u\n", 4000000001, 4000000002, 4000000003, 4000000004);
        break; case __LINE__: TRICE64_1( ID( 1286), "tst:TRICE64_1 %x\n", -1);
        break; case __LINE__: TRICE64_2( ID( 1282), "tst:TRICE64_2 %x %x\n", -1, -2);
        break; case __LINE__: TRICE64_1( ID( 6034), "tst:TRICE64_1 %u\n", -1);
        break; case __LINE__: TRICE64_2( ID( 3225), "tst:TRICE64_2 %u %u\n", -1, -2);

        break; case __LINE__: TRICE( ID( 5657), "sig:colored single letters and several TRICE macros in one line\n" );
        break; case __LINE__: TRICE( ID( 5694), "e:A");
        break; case __LINE__: TRICE( ID( 1796), "w:B");
        break; case __LINE__: TRICE( ID( 4948), "a:c");
        break; case __LINE__: TRICE( ID( 2167), "wr:d");
        break; case __LINE__: TRICE( ID( 5451), "rd:e\n");
        break; case __LINE__: TRICE( ID( 1862), "diag:f");
        break; case __LINE__: TRICE( ID( 4001), "d:G");
        break; case __LINE__: TRICE( ID( 2411), "t:H");
        break; case __LINE__: TRICE( ID( 4485), "time:i");
        break; case __LINE__: TRICE( ID( 1465), "message:J");
        break; case __LINE__: TRICE( ID( 2972), "dbg:k\n");
        break; case __LINE__: TRICE( ID( 7074), "1");
        break; case __LINE__: TRICE( ID( 1889), "2");
        break; case __LINE__: TRICE( ID( 3870), "3");
        break; case __LINE__: TRICE( ID( 5825), "4");
        break; case __LINE__: TRICE( ID( 7946), "e:7");
        break; case __LINE__: TRICE( ID( 7110), "m:12");
        break; case __LINE__: TRICE( ID( 3679), "m:123\n");
        break; case __LINE__: TRICE( ID( 6443), "e:A"); TRICE( ID( 4656), "w:B"); TRICE( ID( 1109), "a:c");
        break; case __LINE__: TRICE( ID( 5406), "wr:d"); TRICE( ID( 7304), "rd:e\n"); TRICE( ID( 4625), "diag:f");

        break; case __LINE__: TRICE( ID( 1979),  "sig:TRICE8 with variable param count 1 to 12\n" );
        break; case __LINE__: TRICE8( ID( 4437), "tst:TRICE8 %d\n", -1 );
        break; case __LINE__: TRICE8( ID( 1881), "tst:TRICE8 %d %d\n", -1, -2 );
        break; case __LINE__: TRICE8( ID( 4591), "tst:TRICE8 %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE8( ID( 7914), "tst:TRICE8 %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE8( ID( 5207), "tst:TRICE8 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE8( ID( 4530), "tst:TRICE8 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE8( ID( 1425), "tst:TRICE8 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE8( ID( 7106), "tst:TRICE8 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE8( ID( 3443), "tst:TRICE8 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE8( ID( 3648), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE8( ID( 4537), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE8( ID( 5994), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE8( ID( 4543), "tst:TRICE8 %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );
        break; case __LINE__: TRICE8_1 ( ID( 7278), "tst:TRICE8_1  %d\n", -1 );
        break; case __LINE__: TRICE8_2 ( ID( 6229), "tst:TRICE8_2  %d %d\n", -1, -2 );
        break; case __LINE__: TRICE8_3 ( ID( 1010), "tst:TRICE8_3  %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE8_4 ( ID( 5722), "tst:TRICE8_4  %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE8_5 ( ID( 5910), "tst:TRICE8_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE8_6 ( ID( 2130), "tst:TRICE8_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE8_7 ( ID( 3045), "tst:TRICE8_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE8_8 ( ID( 5056), "tst:TRICE8_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE8_9 ( ID( 3951), "tst:TRICE8_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE8_10( ID( 6893), "tst:TRICE8_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE8_11( ID( 2263), "tst:TRICE8_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE8_12( ID( 2757), "tst:TRICE8_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE8   ( ID( 5276), "tst:TRICE8   %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );
        break; case __LINE__: TRICE8_9 ( ID( 7122), "tst:TRICE8_9 %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );

        break; case __LINE__: TRICE( ID( 3727), "sig:TRICE16 with variable param count 1 to 12\n" );
        break; case __LINE__: TRICE16( ID( 4833), "tst:TRICE16 %d\n", -1 );
        break; case __LINE__: TRICE16( ID( 6517), "tst:TRICE16 %d %d\n", -1, -2 );
        break; case __LINE__: TRICE16( ID( 7850), "tst:TRICE16 %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE16( ID( 4096), "tst:TRICE16 %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE16( ID( 4479), "tst:TRICE16 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE16( ID( 7315), "tst:TRICE16 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE16( ID( 2981), "tst:TRICE16 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE16( ID( 1155), "tst:TRICE16 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE16( ID( 1536), "tst:TRICE16 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE16( ID( 6644), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE16( ID( 2738), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE16( ID( 1065), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE16( ID( 3059), "tst:TRICE16 %t %b %x %X %d %u %o %O %p\n", 3, 3, 3, 3, 3, 3, 3, 3, 3 );
        break; case __LINE__: TRICE16_1 ( ID( 7425), "tst:TRICE16_1  %d\n", -1 );
        break; case __LINE__: TRICE16_2 ( ID( 3752), "tst:TRICE16_2  %d %d\n", -1, -2 );
        break; case __LINE__: TRICE16_3 ( ID( 5330), "tst:TRICE16_3  %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE16_4 ( ID( 2052), "tst:TRICE16_4  %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE16_5 ( ID( 2238), "tst:TRICE16_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE16_6 ( ID( 1587), "tst:TRICE16_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE16_7 ( ID( 2866), "tst:TRICE16_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE16_8 ( ID( 5466), "tst:TRICE16_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE16_9 ( ID( 2230), "tst:TRICE16_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE16_10( ID( 5325), "tst:TRICE16_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE16_11( ID( 2814), "tst:TRICE16_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE16_12( ID( 2518), "tst:TRICE16_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE16_9 ( ID( 4821), "tst:TRICE16 %t %b %x %X %d %u %o %O %p\n", 3, 3, 3, 3, 3, 3, 3, 3, 3 );

        break; case __LINE__: TRICE( ID( 5559), "sig:TRICE32 with variable param count 1 to 12\n" );
        break; case __LINE__: TRICE32( ID( 3737), "tst:TRICE32 %d\n", -1 );
        break; case __LINE__: TRICE32( ID( 3422), "tst:TRICE32 %d %d\n", -1, -2 );
        break; case __LINE__: TRICE32( ID( 6581), "tst:TRICE32 %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE32( ID( 1326), "tst:TRICE32 %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE32( ID( 4087), "tst:TRICE32 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE32( ID( 7925), "tst:TRICE32 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE32( ID( 5044), "tst:TRICE32 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE32( ID( 3130), "tst:TRICE32 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE32( ID( 2995), "tst:TRICE32 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE32( ID( 4301), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE32( ID( 4256), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE32( ID( 6391), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE32( ID( 3164), "tst:TRICE32 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));
        break; case __LINE__: TRICE32_1 ( ID( 3633), "tst:TRICE32_1  %d\n", -1 );
        break; case __LINE__: TRICE32_2 ( ID( 2478), "tst:TRICE32_2  %d %d\n", -1, -2 );
        break; case __LINE__: TRICE32_3 ( ID( 3578), "tst:TRICE32_3  %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE32_4 ( ID( 5020), "tst:TRICE32_4  %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE32_5 ( ID( 5564), "tst:TRICE32_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE32_6 ( ID( 5580), "tst:TRICE32_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE32_7 ( ID( 3440), "tst:TRICE32_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE32_8 ( ID( 3851), "tst:TRICE32_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE32_9 ( ID( 7249), "tst:TRICE32_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE32_10( ID( 7450), "tst:TRICE32_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE32_11( ID( 7127), "tst:TRICE32_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE32_12( ID( 3339), "tst:TRICE32_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE32   ( ID( 4856), "tst:TRICE32    %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));
        break; case __LINE__: TRICE32_12( ID( 7916), "tst:TRICE32_12 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));

        break; case __LINE__: TRICE( ID( 1132), "sig:TRICE64 with variable param count 1 to 12\n" );
        break; case __LINE__: TRICE64( ID( 6619), "tst:TRICE64 %d\n", -1 );
        break; case __LINE__: TRICE64( ID( 6161), "tst:TRICE64 %d %d\n", -1, -2 );
        break; case __LINE__: TRICE64( ID( 6976), "tst:TRICE64 %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE64( ID( 3196), "tst:TRICE64 %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE64( ID( 7101), "tst:TRICE64 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE64( ID( 3353), "tst:TRICE64 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE64( ID( 7340), "tst:TRICE64 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE64( ID( 6375), "tst:TRICE64 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE64( ID( 2278), "tst:TRICE64 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE64( ID( 2875), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE64( ID( 6216), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE64( ID( 1896), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE64( ID( 7447), "tst:TRICE64 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aDouble(3.14159), aDouble(3.14159), aDouble(3.14159));
        break; case __LINE__: TRICE64_1 ( ID( 5812), "tst:TRICE64_1  %d\n", -1 );
        break; case __LINE__: TRICE64_2 ( ID( 4228), "tst:TRICE64_2  %d %d\n", -1, -2 );
        break; case __LINE__: TRICE64_3 ( ID( 1769), "tst:TRICE64_3  %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE64_4 ( ID( 4551), "tst:TRICE64_4  %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE64_5 ( ID( 1234), "tst:TRICE64_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE64_6 ( ID( 5036), "tst:TRICE64_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE64_7 ( ID( 7354), "tst:TRICE64_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE64_8 ( ID( 6460), "tst:TRICE64_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE64_9 ( ID( 1506), "tst:TRICE64_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE64_10( ID( 6609), "tst:TRICE64_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE64_11( ID( 2599), "tst:TRICE64_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE64_12( ID( 2113), "tst:TRICE64_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE64_12( ID( 1665), "tst:TRICE64_12 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aDouble(3.14159), aDouble(3.14159), aDouble(3.14159));

        break; case __LINE__: TRICE( ID( 5911), "att:positive and negative float in format variants\n");
        break; case __LINE__: TRICE( ID( 5245), "rd:TRICE float %f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 6759), "rd:TRICE float %9f (%%9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 1169), "rd:TRICE float %.9f (%%.9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 1402), "rd:TRICE float %9.f (%%9.f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 5511), "rd:TRICE float %9.6f (%%9.6f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 5285), "rd:TRICE float %f (%%f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 5076), "rd:TRICE float %9f (%%9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 6910), "rd:TRICE float %.9f (%%.9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 6396), "rd:TRICE float %9.f (%%9.f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 4399), "rd:TRICE float %9.6f (%%9.6f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 5242), "rd:TRICE float %+f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 4269), "rd:TRICE float %+9f (%%9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 4310), "rd:TRICE float %+.9f (%%.9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 7372), "rd:TRICE float %+9.f (%%9.f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 3491), "rd:TRICE float %+9.6f (%%9.6f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 4293), "rd:TRICE float %+f (%%f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 4704), "rd:TRICE float %+9f (%%9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 6610), "rd:TRICE float %+.9f (%%.9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 5039), "rd:TRICE float %+9.f (%%9.f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 7563), "rd:TRICE float %+9.6f (%%9.6f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 7808), "rd:TRICE float %-f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 1708), "rd:TRICE float %-9f (%%9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 4370), "rd:TRICE float %-.9f (%%.9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 6231), "rd:TRICE float %-9.f (%%9.f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 7044), "rd:TRICE float %-9.6f (%%9.6f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 1748), "rd:TRICE float %-f (%%f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 6302), "rd:TRICE float %-9f (%%9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 6019), "rd:TRICE float %-.9f (%%.9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 2397), "rd:TRICE float %-9.f (%%9.f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 6685), "rd:TRICE float %-9.6f (%%9.6f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 7615), "att:positive float & double in variants\n");
        break; case __LINE__: TRICE32_1( ID( 3824), "rd:TRICE32_1 float %e (%%e)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1( ID( 7790), "rd:TRICE32_1 float %f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1( ID( 5353), "rd:TRICE32_1 float %g (%%g)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1( ID( 3760), "rd:TRICE32_1 float %E (%%E)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1( ID( 6424), "rd:TRICE32_1 float %F (%%F)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1( ID( 5801), "rd:TRICE32_1 float %G (%%G)\n", aFloat(x) );
        break; case __LINE__: TRICE32  ( ID( 2444), "rd:TRICE32 float %e (%%e)\n", aFloat(x) );
        break; case __LINE__: TRICE32  ( ID( 4059), "rd:TRICE32 float %f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE32  ( ID( 2210), "rd:TRICE32 float %g (%%g)\n", aFloat(x) );
        break; case __LINE__: TRICE32  ( ID( 1052), "rd:TRICE32 float %E (%%E)\n", aFloat(x) );
        break; case __LINE__: TRICE32  ( ID( 7720), "rd:TRICE32 float %F (%%F)\n", aFloat(x) );
        break; case __LINE__: TRICE32  ( ID( 6676), "rd:TRICE32 float %G (%%G)\n", aFloat(x) );
        break; case __LINE__: TRICE    ( ID( 6582), "rd:TRICE float %e (%%e)\n", aFloat(x) );
        break; case __LINE__: TRICE    ( ID( 2722), "rd:TRICE float %f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE    ( ID( 3683), "rd:TRICE float %g (%%g)\n", aFloat(x) );
        break; case __LINE__: TRICE    ( ID( 7191), "rd:TRICE float %E (%%E)\n", aFloat(x) );
        break; case __LINE__: TRICE    ( ID( 3600), "rd:TRICE float %F (%%F)\n", aFloat(x) );
        break; case __LINE__: TRICE    ( ID( 6724), "rd:TRICE float %G (%%G)\n", aFloat(x) );
        break; case __LINE__: TRICE64  ( ID( 7312), "rd:TRICE64 double %e (%%e), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64  ( ID( 2472), "rd:TRICE64 double %f (%%f), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64  ( ID( 3118), "rd:TRICE64 double %g (%%g), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64  ( ID( 3000), "rd:TRICE64 double %E (%%E), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64  ( ID( 7989), "rd:TRICE64 double %F (%%F), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64  ( ID( 7376), "rd:TRICE64 double %G (%%G), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( ID( 1838), "rd:TRICE64_1 double %e (%%e), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( ID( 4915), "rd:TRICE64_1 double %f (%%f), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( ID( 2366), "rd:TRICE64_1 double %g (%%g), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( ID( 7342), "rd:TRICE64_1 double %E (%%E), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( ID( 3072), "rd:TRICE64_1 double %F (%%F), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( ID( 7594), "rd:TRICE64_1 double %G (%%G), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE( ID( 3186), "att:negative float & double\n" );
        break; case __LINE__: TRICE( ID( 3831), "rd:TRICE float %e (%%e)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 7069), "rd:TRICE float %f (%%f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 3117), "rd:TRICE float %g (%%g)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 4752), "rd:TRICE float %E (%%E)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 3330), "rd:TRICE float %F (%%F)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 7363), "rd:TRICE float %G (%%G)\n", aFloat(-x) );
        break; case __LINE__: TRICE64( ID( 3158), "rd:TRICE64 double %e (%%e), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64( ID( 7619), "rd:TRICE64 double %f (%%f), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64( ID( 2692), "rd:TRICE64 double %g (%%g), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64( ID( 1976), "rd:TRICE64 double %E (%%E), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64( ID( 7329), "rd:TRICE64 double %F (%%F), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64( ID( 4552), "rd:TRICE64 double %G (%%G), aDouble(y)\n", aDouble(-y) );

        break; case __LINE__: TRICE  ( ID( 1336), "att:formatted float & double\n" );
        break; case __LINE__: TRICE32( ID( 3100), "rd:TRICE32 float %12.6e (%%12.6e)\n", aFloat(x) );
        break; case __LINE__: TRICE32( ID( 4309), "rd:TRICE32 float %12.6f (%%12.6f)\n", aFloat(x) );
        break; case __LINE__: TRICE32( ID( 3378), "rd:TRICE32 float %12.6g (%%12.6g)\n", aFloat(x) );
        break; case __LINE__: TRICE32( ID( 6111), "rd:TRICE32 float %12.6E (%%12.6E)\n", aFloat(x) );
        break; case __LINE__: TRICE32( ID( 5261), "rd:TRICE32 float %12.6F (%%12.6F)\n", aFloat(x) );
        break; case __LINE__: TRICE32( ID( 1602), "rd:TRICE32 float %12.6G (%%12.6G)\n", aFloat(x) );
        break; case __LINE__: TRICE64( ID( 2907), "rd:TRICE64 double %12.6e (%%12.6e), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64( ID( 2810), "rd:TRICE64 double %12.6f (%%12.6f), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64( ID( 7175), "rd:TRICE64 double %12.6g (%%12.6g), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64( ID( 4226), "rd:TRICE64 double %12.6E (%%12.6E), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64( ID( 2905), "rd:TRICE64 double %12.6F (%%12.6F), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64( ID( 7587), "rd:TRICE64 double %12.6G (%%12.6G), aDouble(y)\n", aDouble(y) );

        break; case __LINE__: TRICE ( ID( 5200), "att:Various single arguments\n" );
        break; case __LINE__: TRICE8( ID( 6154), "rd:TRICE8 line %t (%%t ,0)\n", 0 );
        break; case __LINE__: TRICE8( ID( 1551), "rd:TRICE8 line %t (%%t ,2)\n", 2 );
        break; case __LINE__: TRICE8( ID( 7926), "rd:TRICE8 line %u (%%u ,-1)\n", -1 );
        break; case __LINE__: TRICE8( ID( 6639), "rd:TRICE8 line %b (%%b ,-1)\n", -1 );
        break; case __LINE__: TRICE8( ID( 3106), "rd:TRICE8 line %o (%%o ,-1)\n", -1 );
        break; case __LINE__: TRICE8( ID( 6761), "rd:TRICE8 line %O (%%O ,-1)\n", -1 );
        break; case __LINE__: TRICE8( ID( 3895), "rd:TRICE8 line %X (%%X ,-1)\n", -1 );
        break; case __LINE__: TRICE8( ID( 2921), "rd:TRICE8 line %x (%%x ,-1)\n", -1 );
        break; case __LINE__: TRICE8( ID( 7407), "rd:TRICE8 line %d (%%d ,-1)\n", -1 );
        break; case __LINE__: TRICE16_1( ID( 2934), "rd:TRICE16_1 line %t (%%t ,0)\n", 0 );
        break; case __LINE__: TRICE16_1( ID( 2659), "rd:TRICE16_1 line %t (%%t ,2)\n", 2 );
        break; case __LINE__: TRICE16_1( ID( 7682), "rd:TRICE16_1 line %u (%%u -1)\n", -1 );
        break; case __LINE__: TRICE16_1( ID( 7436), "rd:TRICE16_1 line %b (%%b -1)\n", -1 );
        break; case __LINE__: TRICE16_1( ID( 5260), "rd:TRICE16_1 line %o (%%o -1)\n", -1 );
        break; case __LINE__: TRICE16_1( ID( 5112), "rd:TRICE16_1 line %x (%%x -1)\n", -1 );
        break; case __LINE__: TRICE16_1( ID( 1271), "rd:TRICE16_1 line %d (%%d -1)\n", -1 );
        break; case __LINE__: TRICE( ID( 3596), "rd:TRICE line %t (%%t -1)\n", 0 );
        break; case __LINE__: TRICE( ID( 3203), "rd:TRICE line %t (%%t -1)\n", 2 );
        break; case __LINE__: TRICE( ID( 4472), "rd:TRICE line %u (%%u)\n", -1 );
        break; case __LINE__: TRICE( ID( 4066), "rd:TRICE line %b (%%b)\n", -2 );
        break; case __LINE__: TRICE( ID( 2196), "rd:TRICE line %o (%%o)\n", -3 );
        break; case __LINE__: TRICE( ID( 4992), "rd:TRICE line %x (%%x)\n", -4 );
        break; case __LINE__: TRICE( ID( 6110), "rd:TRICE line %d (%%d)\n", -5 );

        break; case __LINE__: TRICE_N( ID( 6778), "msg:%s\n", A, 0 );
        break; case __LINE__: TRICE_N( ID( 5094), "msg:%s\n", A, 1 );
        break; case __LINE__: TRICE_N( ID( 2498), "msg:%s\n", A, 2 );
        break; case __LINE__: TRICE_N( ID( 7666), "msg:%s\n", A, 3 );
        break; case __LINE__: TRICE_N( ID( 6310), "msg:%s\n", A, 4 );
        break; case __LINE__: TRICE_N( ID( 1183), "msg:%s\n", A, 5 );
        break; case __LINE__: TRICE_N( ID( 7751), "msg:%s\n", A, 6 );
        break; case __LINE__: TRICE_N( ID( 6021), "msg:%s\n", A, 7 );
        break; case __LINE__: TRICE_N( ID( 3170), "msg:%s\n", A, 8 );
        break; case __LINE__: TRICE_N( ID( 5566), "msg:%s\n", A, 9 );
        break; case __LINE__: TRICE_N( ID( 5326), "msg:%s\n", A, 10 );
        break; case __LINE__: TRICE_N( ID( 6308), "msg:%s\n", A, 11 );
        break; case __LINE__: TRICE_N( ID( 7807), "msg:%s\n", A, 12 );
        break; case __LINE__: TRICE_N( ID( 7080), "msg:%s\n", A, 13 );
        break; case __LINE__: TRICE_N( ID( 2392), "msg:%s\n", A, 14 );
        break; case __LINE__: TRICE_N( ID( 4022), "msg:%s\n", A, 15 );
        break; case __LINE__: TRICE_N( ID( 4344), "msg:%s\n", A, 16 );
        break; case __LINE__: TRICE_N( ID( 7245), "msg:%s\n", A, 17 );
        break; case __LINE__: TRICE_N( ID( 4168), "msg:%s\n", A, 18 );
        break; case __LINE__: TRICE_N( ID( 7728), "msg:%s\n", A, 19 );
        break; case __LINE__: TRICE_N( ID( 6552), "msg:%s\n", A, 120 );
        break; case __LINE__: TRICE_N( ID( 7198), "msg:%s\n", A, 121 );
        break; case __LINE__: TRICE_N( ID( 4983), "msg:%s\n", A, 122 );
        break; case __LINE__: TRICE_N( ID( 6695), "msg:%s\n", A, 123 );
        break; case __LINE__: TRICE_N( ID( 7257), "msg:%s\n", A, 124 );
        break; case __LINE__: TRICE_N( ID( 3052), "msg:%s\n", A, 125 );
        break; case __LINE__: TRICE_N( ID( 2479), "msg:%s\n", A, 126 );
        break; case __LINE__: TRICE_N( ID( 1054), "msg:%s\n", A, 127 );
        break; case __LINE__: TRICE_N( ID( 6042), "msg:%s\n", A, 128 );
        break; case __LINE__: TRICE_N( ID( 3066), "msg:%s\n", A, 129 );

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
#define SCOPY( element ) do{ char* n = #element; int size = sizeof( src->element ); memcpy( p, &(src->element), size ); p += size; TRICE_S( ID( 6184), "rd:sizeof(%8s)", n ); TRICE( ID( 6672), " = %d\n", size);}while(0);

    //! DCOPY is a helper macro for struct deserialization.
#define DCOPY( element ) do{ char* n = #element; int size = sizeof( dst->element ); memcpy( &(dst->element), p, size ); p += size; TRICE_S( ID( 3730), "rd:sizeof(%8s)", n ); TRICE( ID( 6423), " = %d\n", size);}while(0);


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
    TRICE ( Id( 3288), "inf: Tryout tx struct:" );
    TRICE8_B( Id( 3049), " %02x ", &tx, sizeof(tx) );
    TRICE( Id( 1438), "\n" );

    TRICE ( Id( 7035), "inf: Tryout buffer:" );
    TRICE8_B( Id( 5572), " %02x ", dst, len ); //lint !e670
    TRICE( Id( 4064), "\n" );

    src = dst; // "data transfer"

    len = deserializeTryout( &rx, src );
    TRICE ( Id( 5826), "inf: Tryout rx struct:" );
    TRICE8_B( Id( 3171), " %02x ", &rx, sizeof(rx) );
    TRICE( Id( 5484), "\n" );

    TRICE( Id( 5853), "inf:sizeOf(Trypout) = %d, buffer length = %d\n", sizeof(tx), len );
    TRICE8_F( Id( 7461), "info:TryoutStructFunction", &tx, sizeof(tx) );
    TRICE8_F( Id( 6716), "info:TryoutBufferFunction", dst, len ); //lint !e670
}

static void exampleOfManualJSONencoding(void){
    typedef  struct {
    int Apple, Birn;
    float Fish;
    } Ex_t;
    Ex_t Ex = { -1, 2, (float)2.781 };
    TRICE( Id(13149), "att:MyStructEvaluationFunction(json:ExA{Apple:%d, Birn:%u, Fisch:%f}\n", Ex.Apple, Ex.Birn, aFloat(Ex.Fish) );
}

static void showTriceDepth( void ){
    #if TRICE_MODE == TRICE_DOUBLE_BUFFER
    {
        uint16_t triceDepth = TriceDepth();
        uint16_t triceDepthMax = TriceDepthMax();
        TRice16( iD( 2089), "MSG: ?� STOP  TriceDepth = %d, TriceDepthMax = %d + %4u of %d\n", triceDepth, TRICE_DATA_OFFSET, triceDepthMax-TRICE_DATA_OFFSET, TRICE_HALF_BUFFER_SIZE );
    } 
    #endif
    #if TRICE_MODE == TRICE_STREAM_BUFFER
        TRice( iD( 2645), "MSG:triceFifoDepthMax = %d of max %d, triceStreamBufferDepthMax = %d of max %d\n", triceFifoDepthMax, TRICE_FIFO_ELEMENTS, triceStreamBufferDepthMax, TRICE_BUFFER_SIZE );
    #endif
}

void TriceLogDepthMax( void ){
    #if TRICE_MODE == TRICE_DOUBLE_BUFFER
        size_t tdm = TriceDepthMax();
        if( tdm <= TRICE_HALF_BUFFER_SIZE ){
            TRice16( iD( 4627), "diag:TriceDepthMax =%4u of %d\n", tdm, TRICE_HALF_BUFFER_SIZE );
        }else{
            TRice16( iD( 2936), "err:TriceDepthMax =%4u of %d (overflow!)\n", tdm, TRICE_HALF_BUFFER_SIZE );
        }
    #endif
}

