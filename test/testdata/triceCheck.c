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
        //break; case __LINE__: TRice( iD( 6254), "info:This is a message without values and a 32-bit stamp.\n" );        //exp: time:32323232info:This is a message without values and a 32-bit stamp.
        //break; case __LINE__: Trice( iD( 6431), "info:This is a message without values and a 16-bit stamp.\n" );        //exp: time:    1616info:This is a message without values and a 16-bit stamp.
        //break; case __LINE__: trice( iD( 4138), "info:This is a message without values and without stamp.\n"  );        //exp: time:        info:This is a message without values and without stamp.
        //break; case __LINE__: TRice( iD( 1667),"info:12 default bit width values %d, %u, %x, %X, &t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 32-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //exp: time:32323232info:12 default bit width values -3, 4294967292, fffffffb, FFFFFFFA, &t, 1.401298e-45, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11000000111000111111001101011100 and a 32-bit stamp.
        //break; case __LINE__: Trice( iD( 5394),"info:12 default bit width values %d, %u, %x, %X, &t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 16-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //exp: time:    1616info:12 default bit width values -3, 4294967292, fffffffb, FFFFFFFA, &t, 1.401298e-45, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11000000111000111111001101011100 and a 16-bit stamp.
        //break; case __LINE__: trice( iD( 6266),"info:12 default bit width values %d, %u, %x, %X, &t, %e, %f, %g, %E, %F, %G, 0xb%08b and without stamp.\n" , -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //exp: time:        info:12 default bit width values -3, 4294967292, fffffffb, FFFFFFFA, &t, 1.401298e-45, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11000000111000111111001101011100 and without stamp.

        // special use cases
        break; case __LINE__: TRice64( iD( 4376),"info:12 64-bit values %d, %u, %x, %X, &t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 32-bit stamp.\n", -3, -4, -5, -6, 1, aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), -8 ); //exp: time:32323232info:12 64-bit values -3, 18446744073709551612, fffffffffffffffb, FFFFFFFFFFFFFFFA, &t, 4.940656e-324, -7.123457, -7.123456789, -7.123457E+00, -7.123457, -7.123456789, 0xb1100000000011100011111100110101101110100110111001110010110001101 and a 32-bit stamp.
        break; case __LINE__: Trice64( iD( 7847),"info:12 64-bit values %d, %u, %x, %X, &t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 16-bit stamp.\n", -3, -4, -5, -6, 1, aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), -8 ); //exp: time:    1616info:12 64-bit values -3, 18446744073709551612, fffffffffffffffb, FFFFFFFFFFFFFFFA, &t, 4.940656e-324, -7.123457, -7.123456789, -7.123457E+00, -7.123457, -7.123456789, 0xb1100000000011100011111100110101101110100110111001110010110001101 and a 16-bit stamp.
        break; case __LINE__: trice64( iD( 1817),"info:12 64-bit values %d, %u, %x, %X, &t, %e, %f, %g, %E, %F, %G, 0xb%08b and without stamp.\n" , -3, -4, -5, -6, 1, aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), -8 ); //exp: time:        info:12 64-bit values -3, 18446744073709551612, fffffffffffffffb, FFFFFFFFFFFFFFFA, &t, 4.940656e-324, -7.123457, -7.123456789, -7.123457E+00, -7.123457, -7.123456789, 0xb1100000000011100011111100110101101110100110111001110010110001101 and without stamp.
        break; case __LINE__: TRice32( iD( 6150),"info:12 32-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 32-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:32323232info:12 32-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 32-bit stamp.
        break; case __LINE__: Trice32( iD( 7245),"info:12 32-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 16-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:    1616info:12 32-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 16-bit stamp.
        break; case __LINE__: trice32( iD( 4212),"info:12 32-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and without stamp.\n" , -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:        info:12 32-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and without stamp.
        break; case __LINE__: TRice16( iD( 4600),"info:12 16-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 32-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:32323232info:12 16-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 32-bit stamp.
        break; case __LINE__: Trice16( iD( 7704),"info:12 16-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 16-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:    1616info:12 16-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 16-bit stamp.
        break; case __LINE__: trice16( iD( 6764),"info:12 16-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and without stamp.\n" , -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:        info:12 16-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and without stamp.
        break; case __LINE__: TRice8 ( iD( 3810),"info:12  8-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 32-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:32323232info:12  8-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 32-bit stamp.
        break; case __LINE__: Trice8 ( iD( 6674),"info:12  8-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 16-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:    1616info:12  8-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 16-bit stamp.
        break; case __LINE__: trice8 ( iD( 1220),"info:12  8-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and without stamp.\n" , -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:        info:12  8-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and without stamp.

        break; case __LINE__: trice16( iD( 2811), "att: line %u\n", __LINE__ );
        break; case __LINE__: exampleOfManualSerialization();

        break; case __LINE__: trice8   ( iD( 3651),         "value=%x\n", 0x55 );                           //exp: time:        value=55
        break; case __LINE__: trice8   ( iD( 1887),  "value=%x\n", 0x88 );                           //exp: time:        value=88

        break; case __LINE__: TRICE( ID( 7270), "att:mixed int & float & double & bits\n" );
        break; case __LINE__: TRICE32( ID( 7767), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n", FloatToInt32(x), aFloat(x), aFloat(x), aFloat(x) );           //exp: time:32323232rd:TRICE32 int 1089, float 1089.608276 (%f), 44883377, 01000100100010000011001101110111                                         
        break; case __LINE__: TRICE32( ID( 4481), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",    (int32_t)(x), aFloat(x), aFloat(x), aFloat(x) );           //exp: time:32323232rd:TRICE32 int 1089, float 1089.608276 (%f), 44883377, 01000100100010000011001101110111                                         
        break; case __LINE__: TRICE32( ID( 6925), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",              x,  aFloat(x), aFloat(x), aFloat(x) );           //exp: time:32323232rd:TRICE32 int 1089, float 1089.608276 (%f), 44883377, 01000100100010000011001101110111                                         
        break; case __LINE__: TRICE32( ID( 5695), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n", FloatToInt32(-x), aFloat(-x), aFloat(-x), aFloat(-x) );       //exp: time:32323232rd:TRICE32 int -1089, float -1089.608276 (%f), c4883377, 11000100100010000011001101110111                                       
        break; case __LINE__: TRICE32( ID( 2238), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",    (int32_t)(-x), aFloat(-x), aFloat(-x), aFloat(-x) );       //exp: time:32323232rd:TRICE32 int -1089, float -1089.608276 (%f), c4883377, 11000100100010000011001101110111                                       
    //  break; case __LINE__: TRICE32( ID( 2528), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",              -x,  aFloat(-x), aFloat(-x), aFloat(-x) );       //exp: time:32323232rd:TRICE32 int 1089, float -1089.608276 (%f), c4883377, 11000100100010000011001101110111                                        
        break; case __LINE__: TRICE64( ID( 7864), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n", DoubleToInt64(y), aDouble(y), aDouble(y), aDouble(y) );     //exp: time:32323232rd:TRICE64 int 518, double 518.054749 (%f), 4080307020601050, 0100000010000000001100000111000000100000011000000001000001010000   
        break; case __LINE__: TRICE64( ID( 4706), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",     (int64_t)(y), aDouble(y), aDouble(y), aDouble(y) );     //exp: time:32323232rd:TRICE64 int 518, double 518.054749 (%f), 4080307020601050, 0100000010000000001100000111000000100000011000000001000001010000   
        break; case __LINE__: TRICE64( ID( 1948), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",               y,  aDouble(y), aDouble(y), aDouble(y) );     //exp: time:32323232rd:TRICE64 int 518, double 518.054749 (%f), 4080307020601050, 0100000010000000001100000111000000100000011000000001000001010000   
        break; case __LINE__: TRICE64( ID( 3889), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n", DoubleToInt64(-y), aDouble(-y), aDouble(-y), aDouble(-y) ); //exp: time:32323232rd:TRICE64 int -518, double -518.054749 (%f), c080307020601050, 1100000010000000001100000111000000100000011000000001000001010000
        break; case __LINE__: TRICE64( ID( 7488), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",     (int64_t)(-y), aDouble(-y), aDouble(-y), aDouble(-y) ); //exp: time:32323232rd:TRICE64 int -518, double -518.054749 (%f), c080307020601050, 1100000010000000001100000111000000100000011000000001000001010000
    //  break; case __LINE__: TRICE64( ID( 7321), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",               -y,  aDouble(-y), aDouble(-y), aDouble(-y) ); //exp: time:32323232rd:TRICE64 int 518, double -518.054749 (%f), c080307020601050, 1100000010000000001100000111000000100000011000000001000001010000 

        break; case __LINE__: trice16( iD( 2540), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRICE_S( ID( 1607), "msg:With TRICE_S:%s\n", sABCDE );                           //exp: time:32323232msg:With TRICE_S:abcde 12345
        break; case __LINE__: TRICE_N( ID( 2356), "sig:With TRICE_N:%s\n", sABCDE, lenABCDE );                 //exp: time:32323232sig:With TRICE_N:abcde 12345
        break; case __LINE__: TRICE8_B( ID( 1260), "  %02x", sABCDE, lenABCDE );                               //exp: time:32323232  61  62  63  64  65  20  31  32  33  34  35
        break; case __LINE__: TRICE( ID( 1477), "\n" );
        break; case __LINE__: TRICE8_B( ID( 2595), "%4d", sABCDE, lenABCDE );                                  //exp: time:32323232  97  98  99 100 101  32  49  50  51  52  53
        break; case __LINE__: TRICE( ID( 2712), "\n" );
        break; case __LINE__: TRICE8_B( ID( 7527), "  %02x", b8, sizeof(b8)/sizeof(int8_t) );                  //exp: time:32323232  00  ff  fe  33  04  05  06  07  08  09  0a  0b  00  ff  fe  33  04  05  06  07  08  09  0a  0b
        break; case __LINE__: TRICE( ID( 3898), "\n" );
        break; case __LINE__: TRICE16_B( ID( 1114), "  %04x", b16, sizeof(b16)/sizeof(int16_t) );              //exp: time:32323232  0000  ffff  fffe  3344
        break; case __LINE__: TRICE( ID( 3524), "\n" );
        break; case __LINE__: TRICE32_B( ID( 3102), "  %08x", b32, sizeof(b32)/sizeof(int32_t) );              //exp: time:32323232  00000000  ffffffff  fffffffe  33445555
        break; case __LINE__: TRICE( ID( 6268), "\n" );
        break; case __LINE__: TRICE64_B( ID( 7267), "  %016x", b64, sizeof(b64)/sizeof(int64_t) );             //exp: time:32323232  0000000000000000  ffffffffffffffff  fffffffffffffffe  3344555566666666
        break; case __LINE__: TRICE( ID( 2026), "\n" );
        break; case __LINE__: TRICE8_F( ID( 3555),  "info:FunctionNameW", b8,  sizeof(b8) /sizeof(int8_t) );   //exp: time:32323232info:FunctionNameW(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)
        break; case __LINE__: TRICE16_F( ID( 4469), "info:FunctionNameX", b16, sizeof(b16)/sizeof(int16_t) );  //exp: time:32323232info:FunctionNameX(0000)(ffff)(fffe)(3344) 
        break; case __LINE__: TRICE32_F( ID( 6074), "info:FunctionNameY", b32, sizeof(b32)/sizeof(int32_t) );  //exp: time:32323232info:FunctionNameY(00000000)(ffffffff)(fffffffe)(33445555)
        break; case __LINE__: TRICE64_F( ID( 2580), "info:FunctionNameZ", b64, sizeof(b64)/sizeof(int64_t) );  //exp: time:32323232info:FunctionNameZ(0000000000000000)(ffffffffffffffff)(fffffffffffffffe)(3344555566666666)
        break; case __LINE__: exampleOfManualJSONencoding(); // exp: time:    1616att:MyStructEvaluationFunction(json:ExA{Apple:-1, Birn:2, Fisch:2.781000}

        break; case __LINE__: trice16( iD( 5752), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRICE( Id( 5992), "MSG:1/11 = %g\n", aFloat( 1.0/11 ) ); //exp: time:    1616MSG:1/11 = 0.09090909
        break; case __LINE__: { //exp: time:32323232msg:x = 5.934 = 5.934, 5.934
                              float a = (float)5.934;
                              float b = a + ((a > 0) ? 0.0005f : -0.0005f);
                              int c = (int)b;
                              int d = (int)(b * 1000) % 1000;
                              int e = (int)(1000 * (float)(a - c)); 
                              TRICE( ID( 4641), "msg:x = %g = %d.%03d, %d.%03d\n", aFloat(a), c, d, c, e ); 
                              }

        //break; case __LINE__: TRICE( id( 1411), "dbg:Hi!\n" );                       // exp: time:        dbg:Hi!    
        //break; case __LINE__: TRICE( id( 5463), "dbg:\tHi!\n" );                     
        //break; case __LINE__: TRICE( id( 1888), "dbg:\t\tHi!\n" );                   
        //break; case __LINE__: TRICE( id( 6791), "dbg:\t\t\tHi!\n" );                 
        //break; case __LINE__: TRICE( id( 3345), "dbg:\\tHi!\n" );                    
        //break; case __LINE__: TRICE( Id( 4844), "dbg:\aHi!\n" ); // sound!
        //break; case __LINE__: TRICE( Id( 2728), "dbg:\a\aHi!\n" ); // sound!
        //break; case __LINE__: TRICE( Id( 3504), "dbg:\a\a\aHi!\n" ); // sound!
        //break; case __LINE__: TRICE( Id( 3434), "dbg:\\aHi!\n" );
        break; case __LINE__: TRICE( Id( 7419), "err:An ERROR messgage with sound!\a\n" ); // sound!

        break; case __LINE__: TRICE_S( ID( 2564), "sig:TRICE_S=%s\n", s );            // exp: time:32323232sig:TRICE_S=AAAAAAAAAAAA
        break; case __LINE__: TRICE_N( ID( 6007), "sig:TRICE_N=%s\n", s, strlen(s) ); // exp: time:32323232sig:TRICE_N=AAAAAAAAAAAA

        break; case __LINE__: trice16( iD( 7647), "att: line %u\n", __LINE__ );

//      break; case __LINE__: 
//  #ifdef XTEA_ENCRYPT_KEY
//          TRICE0 (Id( 4367), "--------------------------------------------------\n" );
//          {
//              uint32_t by[8] = {1,2,3,4,5,6};
//              int count = (sizeof(by) + 7) & ~7; // only multiple of 8 encryptable
//              TRICE32(  Id( 4710), "msg: message = %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
//              TRICE32( Id( 5308), "tim: pre TriceEncryption SysTick=%d\n", SYSTICKVAL );
//              XTEAEncrypt(by, count>>2);
//              TRICE32( Id( 3689), "tim: post TriceEncryption SysTick=%d\n", SYSTICKVAL );
//              TRICE32(  Id( 3064), "att: TriceEncrypted =  %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
//              #ifdef XTEA_DECRYPT
//              TRICE16_1( Id( 3585), "tim: pre TriceDecryption SysTick=%d\n", SYSTICKVAL );
//              XTEADecrypt(by, count>>2);
//              TRICE16_1( Id( 7301), "tim: post TriceDecryption SysTick=%d\n", SYSTICKVAL );
//              #endif
//              TRICE32(  Id( 4093), "msg: messge = %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
//          }
//          TRICE0 (Id( 2388), "--------------------------------------------------\n" );
//  #endif


        break; case __LINE__: showTriceDepth();

        break; case __LINE__: trice16( iD( 1872), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRICE( ID( 5364), "sig:Some time measurements\n" );
        break; case __LINE__: TRICE( ID( 3437), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 2040), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 2279), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 6065), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
        break; case __LINE__: TRICE64( ID( 2438), "rd:TRICE64 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE( ID( 7395), "tim:TRICE START time message\n");
        break; case __LINE__: TRICE( ID( 5844), "tim:TRICE STOP time message\n");
        break; case __LINE__: TRICE( ID( 1951), "tim:TRICE START time message\n");
        break; case __LINE__: TRICE( ID( 4976), "tim:TRICE STOP time message\n");
        break; case __LINE__: TRICE( ID( 1211), "tim:TRICE START time message\n");
        break; case __LINE__: TRICE64( ID( 5246), "rd:TRICE64 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE( ID( 4576), "tim:TRICE STOP time message\n");
        break; case __LINE__: TRICE( ID( 3586), "tim:TRICE --------------------------------------------------\n");
        break; case __LINE__: TRICE( ID( 3589), "tim:TRICE --------------------------------------------------\n");
        break; case __LINE__: TRICE64( ID( 4635), "rd:TRICE64 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE( ID( 5226), "tim:TRICE --------------------------------------------------\n");
        break; case __LINE__: TRICE( ID( 3790), "tim:TRICE --------------------------------------------------\n");
        break; case __LINE__: TRICE64( ID( 4180), "rd:TRICE64 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE( ID( 5629), "rd:TRICE32_1 line %d\n", __LINE__ );
        break; case __LINE__: TRICE64_1( ID( 2545), "rd:TRICE64_1 %d\n", __LINE__ );
        break; case __LINE__: TRICE32_2( ID( 7796), "rd:TRICE32_2 line %d,%d\n", __LINE__, 2 );
        break; case __LINE__: TRICE64_2( ID( 4559), "rd:TRICE64_2 line %d,%d\n", __LINE__, 2 );

        break; case __LINE__: trice16( iD( 3645), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRICE( ID( 3919), "sig:Runtime generated strings\n" );
        break; case __LINE__: {
                                  s = "AAAAAAAAAAAA";
                                  TRICE32( ID( 3082), "dbg:len=%u:", strlen(s) );
                                  TRICE_S( ID( 1904), "sig:%s\n", s );
                              }
        break; case __LINE__: {
                s = "\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBCCCCCCCC\
";
                          }
        break; case __LINE__: TRICE32( ID( 5592), "dbg:len=%u:", strlen(s) );
        break; case __LINE__: TRICE_S( ID( 7607), "sig:%s\n", s );
        break; case __LINE__: TRICE( ID( 6999), "sig:Runtime generated strings\n" );
        break; case __LINE__: {
            s = "\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\
DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\
EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\
";
                          }
        break; case __LINE__: TRICE32( ID( 7961), "dbg:len=%u: ", strlen(s) );
        break; case __LINE__: TRICE_S( ID( 6375), "sig:%s\n", s );
        break; case __LINE__: TRICE( ID( 1790), "sig:Runtime generated strings\n" );
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
        break; case __LINE__: TRICE32( ID( 5320), "dbg:len=%u: ", strlen(s) );
        break; case __LINE__: TRICE_S( ID( 1974), "sig:%s\n", s );
        break; case __LINE__: TRICE( ID( 5859), "sig:Runtime generated strings\n" );
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
        break; case __LINE__: TRICE32( ID( 4400), "dbg:len=%u: ", strlen(s) );
        break; case __LINE__: TRICE_S( ID( 7569), "sig:%s\n", s );

        break; case __LINE__: trice16( iD( 1277), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRICE( ID( 1788), "sig:Integer (indent, base, sign) - see https://yourbasic.org/golang/fmt-printf-reference-cheat-sheet/\n" );
        break; case __LINE__: TRICE( ID( 2070), "rd: 15 	%d 	Base 10\n", 15 );                                        //ex_: time:32323232rd: 15 	15 	Base 10                                   
        break; case __LINE__: TRICE( ID( 1925), "rd: +15 	%+d 	Always show sign\n", 15 );                           //ex_: time:32323232rd: +15 	+15 	Always show sign                          
        break; case __LINE__: TRICE( ID( 4421), "rd:   15 	%4d 	Pad with spaces (width 4, right justified)\n", 15 ); //ex_: time:32323232rd:   15	  15 	Pad with spaces (width 4, right justified)
        break; case __LINE__: TRICE( ID( 7271), "rd: 15   	%-4d 	Pad with spaces (width 4, left justified)\n", 15 );  //ex_: time:32323232rd: 15 	15   	Pad with spaces (width 4, left justified) 
        break; case __LINE__: TRICE( ID( 1558), "rd: 0015 	%04d 	Pad with zeroes (width 4)\n", 15 );                  //ex_: time:32323232rd: 0015 	0015 	Pad with zeroes (width 4)                 
        break; case __LINE__: TRICE( ID( 6523), "rd: 1111 	%b 	Base 2\n", 15 );                                         //ex_: time:32323232rd: 1111 	1111    Base 2                                    
        break; case __LINE__: TRICE( ID( 4013), "rd: 17 	%o 	Base 8\n", 15 );                                         //ex_: time:32323232rd: 17 	17      Base 8                                    
        break; case __LINE__: TRICE( ID( 2784), "rd: f 	%x 	Base 16, lowercase\n", 15 );                             //ex_: time:32323232rd: f 	f 	Base 16, lowercase                        
        break; case __LINE__: TRICE( ID( 1714), "rd: F 	%X 	Base 16, uppercase\n", 15 );                             //ex_: time:32323232rd: F 	F 	Base 16, uppercase                        
        break; case __LINE__: TRICE( ID( 4962), "rd: 0xf 	%#x 	Base 16, with leading 0x\n", 15 );                   //ex_: time:32323232rd: 0xf 	0xf 	Base 16, with leading 0x                  

        break; case __LINE__: trice16( iD( 1518), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRICE( ID( 2711), "sig:Character (quoted, Unicode)\n" );
        break; case __LINE__: TRICE( ID( 4663), "rd: A 	%c 	Character\n", 'A' );                                     //exp: time:32323232rd: A 	A 	Character
        break; case __LINE__: TRICE( ID( 1658), "rd: 'A' 	%q 	Quoted character\n", 'A' );                              //exp: time:32323232rd: 'A' 	'A' 	Quoted character
        break; case __LINE__: TRICE( ID( 2862), "rd: U+0041 	%U 	Unicode\n", 'A' );                        // todo
        break; case __LINE__: TRICE( ID( 1757), "rd: U+0041 'A' 	%#U 	Unicode with character\n", 'A' ); // todo

        break; case __LINE__: TRICE( ID( 4652), "sig:Boolean (true/false)\n" );
        break; case __LINE__: TRICE( ID( 5214), "rd:Use %%t to format a boolean as true (%t) or false (%t).\n", 1, 0 ); //exp: time:32323232rd:Use %t to format a boolean as true (true) or false (false).

        break; case __LINE__: TRICE( ID( 7934), "sig:Pointer (hex)\n" );
        break; case __LINE__: TRICE8 ( ID( 6298), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x84 );               //exp: time:32323232rd:Use %p to format a pointer in base 16 notation with leading 0x. (84)              
        break; case __LINE__: TRICE16( ID( 4640), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x1234 );             //exp: time:32323232rd:Use %p to format a pointer in base 16 notation with leading 0x. (1234)            
        break; case __LINE__: TRICE32( ID( 1572), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0xaabbccdd );         //exp: time:32323232rd:Use %p to format a pointer in base 16 notation with leading 0x. (aabbccdd)        
        break; case __LINE__: TRICE64( ID( 2039), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x1122334455667788 ); //exp: time:32323232rd:Use %p to format a pointer in base 16 notation with leading 0x. (1122334455667788)
        break; case __LINE__: TRICE  ( ID( 1796), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0xaabbccdd);          //exp: time:32323232rd:Use %p to format a pointer in base 16 notation with leading 0x. (aabbccdd)        

        break; case __LINE__: trice16( iD( 5868), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRICE( ID( 7870), "sig:Float (indent, precision, scientific notation)\n" );
        break; case __LINE__: TRICE( ID( 7083), "rd: 1.234560e+02		%e 	%%e Scientific notation\n", aFloat(123.456) );                       //exp: time:32323232rd: 1.234560e+02		1.234560e+02 	%e Scientific notation
        break; case __LINE__: TRICE( ID( 6408), "rd: 123.456000		%f 	%%f Decimal point, no exponent\n", aFloat(123.456) );                //exp: time:32323232rd: 123.456000		123.456001 	%f Decimal point, no exponent
        break; case __LINE__: TRICE( ID( 6502), "rd: 123.46			%.2f		%%.2f Default width, precision 2\n", aFloat(123.456) );      //exp: time:32323232rd: 123.46			123.46		%.2f Default width, precision 2
        break; case __LINE__: TRICE( ID( 4422), "rd: ␣␣123.46		%8.2f	%%8.2f Width 8, precision 2\n", aFloat(123.456) );                    //exp: time:32323232rd: ␣␣123.46		  123.46	%8.2f Width 8, precision 2
        break; case __LINE__: TRICE( ID( 4748), "rd: 123.456		%g		%%g Exponent as needed, necessary digits only\n", aFloat(123.456) ); //exp: time:32323232rd: 123.456		123.456		%g Exponent as needed, necessary digits only

        break; case __LINE__: TRICE( ID( 6505), "sig:Double (indent, precision, scientific notation)\n" );
        break; case __LINE__: TRICE64( ID( 7982), "rd: 1.234560e+02		%e 	%%e Scientific notation\n", aDouble(123.456) );                   //exp: time:32323232rd: 1.234560e+02		1.234560e+02 	%e Scientific notation
        break; case __LINE__: TRICE64( ID( 2583), "rd: 123.456000		%f 	%%f Decimal point, no exponent\n", aDouble(123.456) );                //exp: time:32323232rd: 123.456000		123.456000 	%f Decimal point, no exponent
        break; case __LINE__: TRICE64( ID( 3451), "rd: 123.46			%.2f    	%%.2f Default width, precision 2\n", aDouble(123.456) );      //exp: time:32323232rd: 123.46			123.46    	%.2f Default width, precision 2
        break; case __LINE__: TRICE64( ID( 4566), "rd: ␣␣123.46		%8.2f 	%%8.2f Width 8, precision 2\n", aDouble(123.456) );                //exp: time:32323232rd: ␣␣123.46		  123.46 	%8.2f Width 8, precision 2
        break; case __LINE__: TRICE64( ID( 1069), "rd: 123.456		%g		%%g Exponent as needed, necessary digits only\n", aDouble(123.456) ); //exp: time:32323232rd: 123.456		123.456		%g Exponent as needed, necessary digits only

        break; case __LINE__: TRICE( ID( 4380), "sig:String or byte slice (quote, indent, hex)\n" );
        break; case __LINE__: s = "café"; TRICE_S( ID( 5347), "rd: café 			%s		Plain string\n", s );           //exp: time:32323232rd: café 			café		Plain string
        break; case __LINE__: s = "café"; TRICE_S( ID( 2374), "rd: ␣␣café 		%6s 		Width 6, right justify\n", s );  //exp: time:32323232rd: ␣␣café 		  café 		Width 6, right justify
        break; case __LINE__: s = "café"; TRICE_S( ID( 1861), "rd: café␣␣ 		%-6s 		Width 6, left justify\n", s );   //exp: time:32323232rd: café␣␣ 		café   		Width 6, left justify
        break; case __LINE__: s = "café"; TRICE_S( ID( 3104), "rd: \"café\" 		%q		Quoted string\n", s );          //exp: time:32323232rd: \"café\" 		"café"		Quoted string
        break; case __LINE__: s = "café"; TRICE_S( ID( 5556), "rd: 636166c3a9 		%x	Hex dump of byte values\n", s );    //exp: time:32323232rd: 636166c3a9 		636166c3a9	Hex dump of byte values 
        break; case __LINE__: s = "café"; TRICE_S( ID( 2237), "rd: 63 61 66 c3 a9 	% x	Hex dump with spaces\n", s );       //exp: time:32323232rd: 63 61 66 c3 a9 	63 61 66 c3 a9	Hex dump with spaces

        break; case __LINE__: trice16( iD( 1015), "att: line %u\n", __LINE__ );

#ifdef TRICE_CGO

        break; case __LINE__: TRICE  ( ID( 6113), "rd:%E (%%E)\n",  aFloat(-555555555.5555555555) ); // exp: time:32323232rd:-5.555556E+08 (%E)
        break; case __LINE__: TRICE  ( ID( 6026), "rd:%F (%%F)\n",  aFloat(-555555555.5555555555) ); // exp: time:32323232rd:-555555584.000000 (%F)
        break; case __LINE__: TRICE  ( ID( 7664), "rd:%G (%%G)\n",  aFloat(-555555555.5555555555) ); // exp: time:32323232rd:-5.555556E+08 (%G)
        break; case __LINE__: TRICE64( ID( 5096), "rd:%E (%%E)\n", aDouble(-555555555.5555555555) ); // exp: time:32323232rd:-5.555556E+08 (%E)
        break; case __LINE__: TRICE64( ID( 3831), "rd:%F (%%F)\n", aDouble(-555555555.5555555555) ); // exp: time:32323232rd:-555555555.555556 (%F)
        break; case __LINE__: TRICE64( ID( 3701), "rd:%G (%%G)\n", aDouble(-555555555.5555555555) ); // exp: time:32323232rd:-5.555555555555556E+08 (%G)
        
        break; case __LINE__: TRICE  ( ID( 2616), "rd:%e (%%e)\n",  aFloat(-555555555.5555555555) ); // exp: time:32323232rd:-5.555556e+08 (%e)
        break; case __LINE__: TRICE  ( ID( 5963), "rd:%f (%%f)\n",  aFloat(-555555555.5555555555) ); // exp: time:32323232rd:-555555584.000000 (%f)
        break; case __LINE__: TRICE  ( ID( 6902), "rd:%g (%%g)\n",  aFloat(-555555555.5555555555) ); // exp: time:32323232rd:-5.555556e+08 (%g)
        break; case __LINE__: TRICE64( ID( 6749), "rd:%e (%%e)\n", aDouble(-555555555.5555555555) ); // exp: time:32323232rd:-5.555556e+08 (%e)
        break; case __LINE__: TRICE64( ID( 6185), "rd:%f (%%f)\n", aDouble(-555555555.5555555555) ); // exp: time:32323232rd:-555555555.555556 (%f)
        break; case __LINE__: TRICE64( ID( 2283), "rd:%g (%%g)\n", aDouble(-555555555.5555555555) ); // exp: time:32323232rd:-5.555555555555556e+08 (%g)

        break; case __LINE__: TRICE32( ID( 7960), "msg:%u (%%u)\n", -1 ); //exp: time:32323232msg:4294967295 (%u)
        break; case __LINE__: TRICE32( ID( 7593), "msg:%b (%%b)\n", -1 ); //exp: time:32323232msg:11111111111111111111111111111111 (%b)
        break; case __LINE__: TRICE32( ID( 6924), "msg:%o (%%o)\n", -1 ); //exp: time:32323232msg:37777777777 (%o)
        break; case __LINE__: TRICE32( ID( 2808), "msg:%O (%%O)\n", -1 ); //exp: time:32323232msg:0o37777777777 (%O)
        break; case __LINE__: TRICE32( ID( 1852), "msg:%X (%%X)\n", -1 ); //exp: time:32323232msg:FFFFFFFF (%X)
        break; case __LINE__: TRICE32( ID( 4896), "msg:%x (%%x)\n", -1 ); //exp: time:32323232msg:ffffffff (%x)
        break; case __LINE__: TRICE32( ID( 5091), "msg:%d (%%d)\n", -1 ); //exp: time:32323232msg:-1 (%d)
        break; case __LINE__: TRICE64( ID( 7469), "msg:%u (%%u)\n", -1 ); //exp: time:32323232msg:18446744073709551615 (%u)
        break; case __LINE__: TRICE64( ID( 7778), "msg:%b (%%b)\n", -1 ); //exp: time:32323232msg:1111111111111111111111111111111111111111111111111111111111111111 (%b)
        break; case __LINE__: TRICE64( ID( 7259), "msg:%o (%%o)\n", -1 ); //exp: time:32323232msg:1777777777777777777777 (%o)
        break; case __LINE__: TRICE64( ID( 6590), "msg:%O (%%O)\n", -1 ); //exp: time:32323232msg:0o1777777777777777777777 (%O)
        break; case __LINE__: TRICE64( ID( 3656), "msg:%X (%%X)\n", -1 ); //exp: time:32323232msg:FFFFFFFFFFFFFFFF (%X)
        break; case __LINE__: TRICE64( ID( 5833), "msg:%x (%%x)\n", -1 ); //exp: time:32323232msg:ffffffffffffffff (%x)

        //break; case __LINE__: TRice        ( iD( 2883), "value=%d\n", -2  ); //exp: time:32323232value=-2
        //break; case __LINE__: Trice        ( iD( 6529), "value=%d\n", -2  ); //exp: time:    1616value=-2
        //break; case __LINE__: trice        ( iD( 7685), "value=%d\n", -2  ); //exp: time:        value=-2
        break; case __LINE__: TRice8       ( iD( 5286), "value=%d\n", -2  ); //exp: time:32323232value=-2
        break; case __LINE__: Trice8       ( iD( 5664), "value=%d\n", -2  ); //exp: time:    1616value=-2
        break; case __LINE__: trice8       ( iD( 6690), "value=%d\n", -2  ); //exp: time:        value=-2
        break; case __LINE__: TRice16      ( iD( 2486), "value=%d\n", -2  ); //exp: time:32323232value=-2
        break; case __LINE__: Trice16      ( iD( 5564), "value=%d\n", -2  ); //exp: time:    1616value=-2
        break; case __LINE__: trice16      ( iD( 4614), "value=%d\n", -2  ); //exp: time:        value=-2
        break; case __LINE__: TRice32      ( iD( 3823), "value=%d\n", -2  ); //exp: time:32323232value=-2
        break; case __LINE__: Trice32      ( iD( 1916), "value=%d\n", -2  ); //exp: time:    1616value=-2
        break; case __LINE__: trice32      ( iD( 4354), "value=%d\n", -2  ); //exp: time:        value=-2
        break; case __LINE__: TRice64      ( iD( 4094), "value=%d\n", -2  ); //exp: time:32323232value=-2
        break; case __LINE__: Trice64      ( iD( 1537), "value=%d\n", -2  ); //exp: time:    1616value=-2
        break; case __LINE__: trice64      ( iD( 6318), "value=%d\n", -2  ); //exp: time:        value=-2
        
        //break; case __LINE__: TRice_1      ( iD( 2746), "value=%d\n", -2  ); //exp: time:32323232value=-2
        //break; case __LINE__: Trice_1      ( iD( 2197), "value=%d\n", -2  ); //exp: time:    1616value=-2
        //break; case __LINE__: trice_1      ( iD( 1942), "value=%d\n", -2  ); //exp: time:        value=-2
        //break; case __LINE__: TRice8_1     ( iD( 6628), "value=%d\n", -2  ); //exp: time:32323232value=-2
        //break; case __LINE__: Trice8_1     ( iD( 6546), "value=%d\n", -2  ); //exp: time:    1616value=-2
        //break; case __LINE__: trice8_1     ( iD( 5799), "value=%d\n", -2  ); //exp: time:        value=-2
        //break; case __LINE__: TRice16_1    ( iD( 3204), "value=%d\n", -2  ); //exp: time:32323232value=-2
        //break; case __LINE__: Trice16_1    ( iD( 5159), "value=%d\n", -2  ); //exp: time:    1616value=-2
        //break; case __LINE__: trice16_1    ( iD( 7266), "value=%d\n", -2  ); //exp: time:        value=-2
        //break; case __LINE__: TRice32_1    ( iD( 2989), "value=%d\n", -2  ); //exp: time:32323232value=-2
        //break; case __LINE__: Trice32_1    ( iD( 1527), "value=%d\n", -2  ); //exp: time:    1616value=-2
        //break; case __LINE__: trice32_1    ( iD( 3804), "value=%d\n", -2  ); //exp: time:        value=-2
        //break; case __LINE__: TRice64_1    ( iD( 4176), "value=%d\n", -2  ); //exp: time:32323232value=-2
        //break; case __LINE__: Trice64_1    ( iD( 1303), "value=%d\n", -2  ); //exp: time:    1616value=-2
        //break; case __LINE__: trice64_1    ( iD( 2861), "value=%d\n", -2  ); //exp: time:        value=-2
        
        //break; case __LINE__: TRice        ( iD( 1517), "no value" ); //exp: time:32323232no value
        //break; case __LINE__: Trice        ( iD( 2898), "no value" ); //exp: time:    1616no value
        //break; case __LINE__: trice        ( iD( 2462), "no value" ); //exp: time:        no value
        //break; case __LINE__: TRice8       ( iD( 1584), "no value" ); //exp: time:32323232no value
        //break; case __LINE__: Trice8       ( iD( 6288), "no value" ); //exp: time:    1616no value
        //break; case __LINE__: trice8       ( iD( 4835), "no value" ); //exp: time:        no value
        //break; case __LINE__: TRice16      ( iD( 2629), "no value" ); //exp: time:32323232no value
        //break; case __LINE__: Trice16      ( iD( 4471), "no value" ); //exp: time:    1616no value
        //break; case __LINE__: trice16      ( iD( 3364), "no value" ); //exp: time:        no value
        //break; case __LINE__: TRice32      ( iD( 3024), "no value" ); //exp: time:32323232no value
        //break; case __LINE__: Trice32      ( iD( 1434), "no value" ); //exp: time:    1616no value
        //break; case __LINE__: trice32      ( iD( 1859), "no value" ); //exp: time:        no value
        //break; case __LINE__: TRice64      ( iD( 7142), "no value" ); //exp: time:32323232no value
        //break; case __LINE__: Trice64      ( iD( 4552), "no value" ); //exp: time:    1616no value
        //break; case __LINE__: trice64      ( iD( 5171), "no value" ); //exp: time:        no value
        
        //break; case __LINE__: TRice_0      ( iD( 1378), "no value" ); //exp: time:32323232no value
        //break; case __LINE__: Trice_0      ( iD( 2418), "no value" ); //exp: time:    1616no value
        //break; case __LINE__: trice_0      ( iD( 1066), "no value" ); //exp: time:        no value
        //break; case __LINE__: TRice8_0     ( iD( 5532), "no value" ); //exp: time:32323232no value
        //break; case __LINE__: Trice8_0     ( iD( 3489), "no value" ); //exp: time:    1616no value
        //break; case __LINE__: trice8_0     ( iD( 2314), "no value" ); //exp: time:        no value
        //break; case __LINE__: TRice16_0    ( iD( 7070), "no value" ); //exp: time:32323232no value
        //break; case __LINE__: Trice16_0    ( iD( 5585), "no value" ); //exp: time:    1616no value
        //break; case __LINE__: trice16_0    ( iD( 7561), "no value" ); //exp: time:        no value
        //break; case __LINE__: TRice32_0    ( iD( 3501), "no value" ); //exp: time:32323232no value
        //break; case __LINE__: Trice32_0    ( iD( 2359), "no value" ); //exp: time:    1616no value
        //break; case __LINE__: trice32_0    ( iD( 7316), "no value" ); //exp: time:        no value
        //break; case __LINE__: TRice64_0    ( iD( 5529), "no value" ); //exp: time:32323232no value
        //break; case __LINE__: Trice64_0    ( iD( 6997), "no value" ); //exp: time:    1616no value
        //break; case __LINE__: trice64_0    ( iD( 6329), "no value" ); //exp: time:        no value

        //break; case __LINE__: TRice( iD( 1259),"info:This is a message with one value %d and a 32-bit stamp.\n", -2  ); //exp: time:32323232info:This is a message with one value -2 and a 32-bit stamp.
        //break; case __LINE__: Trice( iD( 6086),"info:This is a message with one value %d and a 16-bit stamp.\n", -2  ); //exp: time:    1616info:This is a message with one value -2 and a 16-bit stamp.
        //break; case __LINE__: trice( iD( 4522),"info:This is a message with one value %d and without stamp.\n" , -2  ); //exp: time:        info:This is a message with one value -2 and without stamp.

        break; case __LINE__: TRICE( ID( 3196), "info:This is a message without values and a 32-bit stamp.\n" );        //exp: time:32323232info:This is a message without values and a 32-bit stamp.
        break; case __LINE__: TRICE( Id( 1344), "info:This is a message without values and a 16-bit stamp.\n" );        //exp: time:    1616info:This is a message without values and a 16-bit stamp.
        break; case __LINE__: TRICE( id( 2149), "info:This is a message without values and without stamp.\n"  );        //exp: time:        info:This is a message without values and without stamp.

        break; case __LINE__: TRICE( ID( 3319),"info:This is a message with one value %d and a 32-bit stamp.\n", -2  ); //exp: time:32323232info:This is a message with one value -2 and a 32-bit stamp.
        break; case __LINE__: TRICE( Id( 5183),"info:This is a message with one value %d and a 16-bit stamp.\n", -2  ); //exp: time:    1616info:This is a message with one value -2 and a 16-bit stamp.
        break; case __LINE__: TRICE( id( 5846),"info:This is a message with one value %d and without stamp.\n" , -2  ); //exp: time:        info:This is a message with one value -2 and without stamp.

        break; case __LINE__: TRICE( ID( 4467),"info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 32-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 );   //exp: time:32323232info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and a 32-bit stamp.
        break; case __LINE__: TRICE( Id( 6643),"info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 16-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 );   //exp: time:    1616info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and a 16-bit stamp.
        break; case __LINE__: TRICE( id( 4393),"info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and without stamp.\n" , -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 );   //exp: time:        info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and without stamp.

        break; case __LINE__: TRICE32( ID( 6606),"info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 32-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //exp: time:32323232info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and a 32-bit stamp.
        break; case __LINE__: TRICE32( Id( 6399),"info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 16-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //exp: time:    1616info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and a 16-bit stamp.
        break; case __LINE__: TRICE32( id( 2999),"info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and without stamp.\n" , -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //exp: time:        info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and without stamp.

        break; case __LINE__: TRICE8   ( id( 6196), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: TRICE8_1 ( id( 5015), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: TRICE8   ( Id( 5538), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRICE8_1 ( Id( 4625), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRICE8   ( ID( 5461), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: TRICE8_1 ( ID( 7309), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: trice8   ( iD( 6407), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: trice8_1 ( iD( 6789), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: Trice8   ( iD( 4368), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: Trice8_1 ( iD( 7050), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRice8   ( iD( 1084), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: TRice8_1 ( iD( 5126), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200

        break; case __LINE__: TRICE16  ( id( 3058), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: TRICE16_1( id( 7486), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: TRICE16  ( Id( 2429), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRICE16_1( Id( 2450), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRICE16  ( ID( 6707), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: TRICE16_1( ID( 1924), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: trice16  ( iD( 7300), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: trice16_1( iD( 4259), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: Trice16  ( iD( 3991), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: Trice16_1( iD( 1321), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRice16  ( iD( 5078), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: TRice16_1( iD( 3677), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200

        break; case __LINE__: TRICE32  ( id( 4248), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: TRICE32_1( id( 6127), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: TRICE32  ( Id( 6571), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRICE32_1( Id( 1266), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRICE32  ( ID( 6730), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: TRICE32_1( ID( 5191), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: trice32  ( iD( 6656), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: trice32_1( iD( 2523), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: Trice32  ( iD( 1902), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: Trice32_1( iD( 3511), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRice32  ( iD( 7747), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: TRice32_1( iD( 2848), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200

        break; case __LINE__: TRICE64  ( id( 4369), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: TRICE64_1( id( 6544), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: TRICE64  ( Id( 5216), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRICE64_1( Id( 7372), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRICE64  ( ID( 2975), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: TRICE64_1( ID( 2810), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: trice64  ( iD( 3025), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: trice64_1( iD( 5509), "msg:value=%u\n", 200 ); //exp: time:        msg:value=200
        break; case __LINE__: Trice64  ( iD( 2770), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: Trice64_1( iD( 6207), "msg:value=%u\n", 200 ); //exp: time:    1616msg:value=200
        break; case __LINE__: TRice64  ( iD( 6702), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200
        break; case __LINE__: TRice64_1( iD( 2455), "msg:value=%u\n", 200 ); //exp: time:32323232msg:value=200

        // mixed 16-bit and 32-bit timestamp 
        break; case __LINE__: TRice8( iD( 2075), "msg:value=%d\n", -1 );                                                                                            //exp: time:32323232msg:value=-1
        break; case __LINE__: Trice8( iD( 7426), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    1616msg:value=-1, -2
        break; case __LINE__: trice8( iD( 4936), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        msg:value=-1, -2, -3
        break; case __LINE__: TRice8( iD( 5032), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:32323232msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice8( iD( 6218), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    1616msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice8( iD( 7452), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice8( iD( 4627), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice8( iD( 7800), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice8( iD( 1770), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice8( iD( 4065), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice8( iD( 3449), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice8( iD( 6763), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice16( iD( 5176), "msg:value=%d\n", -1 );                                                                                            //exp: time:32323232msg:value=-1
        break; case __LINE__: Trice16( iD( 6425), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    1616msg:value=-1, -2
        break; case __LINE__: trice16( iD( 4404), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        msg:value=-1, -2, -3
        break; case __LINE__: TRice16( iD( 5733), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:32323232msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice16( iD( 3150), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    1616msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice16( iD( 2434), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice16( iD( 2196), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice16( iD( 1656), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice16( iD( 3747), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice16( iD( 5616), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice16( iD( 5352), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice16( iD( 3051), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice32( iD( 7284), "msg:value=%d\n", -1 );                                                                                            //exp: time:32323232msg:value=-1
        break; case __LINE__: Trice32( iD( 7939), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    1616msg:value=-1, -2
        break; case __LINE__: trice32( iD( 5224), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        msg:value=-1, -2, -3
        break; case __LINE__: TRice32( iD( 4125), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:32323232msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice32( iD( 7669), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    1616msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice32( iD( 5382), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice32( iD( 7655), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice32( iD( 7753), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice32( iD( 3594), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice32( iD( 2181), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice32( iD( 1263), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice32( iD( 2535), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice64( iD( 4130), "msg:value=%d\n", -1 );                                                                                            //exp: time:32323232msg:value=-1
        break; case __LINE__: Trice64( iD( 4163), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    1616msg:value=-1, -2
        break; case __LINE__: trice64( iD( 4154), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        msg:value=-1, -2, -3
        break; case __LINE__: TRice64( iD( 7518), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:32323232msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice64( iD( 4297), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    1616msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice64( iD( 5762), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice64( iD( 2636), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice64( iD( 1376), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice64( iD( 5561), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice64( iD( 4461), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice64( iD( 3004), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice64( iD( 5686), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice8_1 ( iD( 7237), "msg:value=%d\n", -1 );                                                                                            //exp: time:32323232msg:value=-1
        break; case __LINE__: Trice8_2 ( iD( 7973), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    1616msg:value=-1, -2
        break; case __LINE__: trice8_3 ( iD( 3387), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        msg:value=-1, -2, -3
        break; case __LINE__: TRice8_4 ( iD( 7544), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:32323232msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice8_5 ( iD( 1415), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    1616msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice8_6 ( iD( 7375), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice8_7 ( iD( 3060), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice8_8 ( iD( 5906), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice8_9 ( iD( 1060), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice8_10( iD( 1132), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice8_11( iD( 6614), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice8_12( iD( 6437), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice16_1 ( iD( 1886), "msg:value=%d\n", -1 );                                                                                            //exp: time:32323232msg:value=-1
        break; case __LINE__: Trice16_2 ( iD( 1963), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    1616msg:value=-1, -2
        break; case __LINE__: trice16_3 ( iD( 6208), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        msg:value=-1, -2, -3
        break; case __LINE__: TRice16_4 ( iD( 7179), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:32323232msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice16_5 ( iD( 1586), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    1616msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice16_6 ( iD( 6856), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice16_7 ( iD( 6126), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice16_8 ( iD( 6223), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice16_9 ( iD( 4337), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice16_10( iD( 5473), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice16_11( iD( 3581), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice16_12( iD( 2553), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice32_1 ( iD( 7071), "msg:value=%d\n", -1 );                                                                                            //exp: time:32323232msg:value=-1
        break; case __LINE__: Trice32_2 ( iD( 7811), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    1616msg:value=-1, -2
        break; case __LINE__: trice32_3 ( iD( 7406), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        msg:value=-1, -2, -3
        break; case __LINE__: TRice32_4 ( iD( 3188), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:32323232msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice32_5 ( iD( 7262), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    1616msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice32_6 ( iD( 1795), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice32_7 ( iD( 2235), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice32_8 ( iD( 5328), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice32_9 ( iD( 5254), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice32_10( iD( 5885), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice32_11( iD( 1650), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice32_12( iD( 2392), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice64_1 ( iD( 6651), "msg:value=%d\n", -1 );                                                                                            //exp: time:32323232msg:value=-1
        break; case __LINE__: Trice64_2 ( iD( 3247), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    1616msg:value=-1, -2
        break; case __LINE__: trice64_3 ( iD( 6575), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        msg:value=-1, -2, -3
        break; case __LINE__: TRice64_4 ( iD( 5718), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:32323232msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice64_5 ( iD( 3475), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    1616msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice64_6 ( iD( 4027), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice64_7 ( iD( 5990), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice64_8 ( iD( 7643), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice64_9 ( iD( 1647), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice64_10( iD( 5495), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:32323232msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice64_11( iD( 1598), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    1616msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice64_12( iD( 1480), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        // use cases for 8-bit data
        break; case __LINE__: trice8     ( iD( 6178), "value=%d\n", -2  ); //exp: time:        value=-2
        break; case __LINE__: Trice8     ( iD( 1202), "value=%d\n", -2  ); //exp: time:    1616value=-2
        break; case __LINE__: TRice8     ( iD( 2592), "value=%d\n", -2  ); //exp: time:32323232value=-2
        break; case __LINE__: trice8_1   ( iD( 7666), "value=%d\n", -2  ); //exp: time:        value=-2
        break; case __LINE__: Trice8_1   ( iD( 2086), "value=%d\n", -2  ); //exp: time:    1616value=-2
        break; case __LINE__: TRice8_1   ( iD( 1249), "value=%d\n", -2  ); //exp: time:32323232value=-2

        // use cases for 16-bit data
        break; case __LINE__: trice16    ( iD( 6354), "value=%d\n", -2  ); //exp: time:        value=-2
        break; case __LINE__: Trice16    ( iD( 7091), "value=%d\n", -2  ); //exp: time:    1616value=-2
        break; case __LINE__: TRice16    ( iD( 6453), "value=%d\n", -2  ); //exp: time:32323232value=-2
        break; case __LINE__: trice16_1  ( iD( 4335), "value=%d\n", -2  ); //exp: time:        value=-2
        break; case __LINE__: Trice16_1  ( iD( 1197), "value=%d\n", -2  ); //exp: time:    1616value=-2
        break; case __LINE__: TRice16_1  ( iD( 2239), "value=%d\n", -2  ); //exp: time:32323232value=-2

        // use cases for 32-bit data
        break; case __LINE__: trice32    ( iD( 6366), "value=%d\n", -2  ); //exp: time:        value=-2
        break; case __LINE__: Trice32    ( iD( 5701), "value=%d\n", -2  ); //exp: time:    1616value=-2
        break; case __LINE__: TRice32    ( iD( 6192), "value=%d\n", -2  ); //exp: time:32323232value=-2
        break; case __LINE__: trice32_1  ( iD( 5211), "value=%d\n", -2  ); //exp: time:        value=-2
        break; case __LINE__: Trice32_1  ( iD( 5050), "value=%d\n", -2  ); //exp: time:    1616value=-2
        break; case __LINE__: TRice32_1  ( iD( 6770), "value=%d\n", -2  ); //exp: time:32323232value=-2

        // use cases for 64-bit data
        break; case __LINE__: trice64    ( iD( 6560), "value=%d\n", -2  ); //exp: time:        value=-2
        break; case __LINE__: Trice64    ( iD( 1936), "value=%d\n", -2  ); //exp: time:    1616value=-2
        break; case __LINE__: TRice64    ( iD( 3033), "value=%d\n", -2  ); //exp: time:32323232value=-2
        break; case __LINE__: trice64_1  ( iD( 6468), "value=%d\n", -2  ); //exp: time:        value=-2
        break; case __LINE__: Trice64_1  ( iD( 6303), "value=%d\n", -2  ); //exp: time:    1616value=-2
        break; case __LINE__: TRice64_1  ( iD( 4246), "value=%d\n", -2  ); //exp: time:32323232value=-2

        // legacy use cases
        break; case __LINE__: TRICE      ( ID( 1512),        "value=%d\n", -2 ); //exp: time:32323232value=-2
        break; case __LINE__: TRICE      ( id( 7758), "value=%d\n", -2 ); //exp: time:        value=-2
        break; case __LINE__: TRICE      ( Id( 2341), "value=%d\n", -2 ); //exp: time:    1616value=-2
        break; case __LINE__: TRICE      ( ID( 6316), "value=%d\n", -2 ); //exp: time:32323232value=-2

        // default stamp
        break; case __LINE__: TRICE8     ( ID( 2349),        "value=%d\n", -2 ); //exp: time:32323232value=-2
        break; case __LINE__: TRICE8_1   ( ID( 7157),        "value=%d\n", -2 ); //exp: time:32323232value=-2

        // no stamp
        break; case __LINE__: trice8     ( iD( 1214),        "value=%d\n", -2 ); //exp: time:        value=-2
        break; case __LINE__: trice8_1   ( iD( 1381),        "value=%d\n", -2 ); //exp: time:        value=-2
        break; case __LINE__: TRICE8     ( id( 1474), "value=%d\n", -2 ); //exp: time:        value=-2
        break; case __LINE__: TRICE8_1   ( id( 5443), "value=%d\n", -2 ); //exp: time:        value=-2

        // 16-bit stamp:
        break; case __LINE__: Trice8     ( iD( 3063),        "value=%d\n", -2 ); //exp: time:    1616value=-2
        break; case __LINE__: Trice8_1   ( iD( 6387),        "value=%d\n", -2 ); //exp: time:    1616value=-2
        break; case __LINE__: TRICE8     ( Id( 1106), "value=%d\n", -2 ); //exp: time:    1616value=-2
        break; case __LINE__: TRICE8_1   ( Id( 7804), "value=%d\n", -2 ); //exp: time:    1616value=-2

        // 32-bit stamp:
        break; case __LINE__: TRice8     ( iD( 6899),        "value=%d\n", -2 ); //exp: time:32323232value=-2
        break; case __LINE__: TRice8_1   ( iD( 7654),        "value=%d\n", -2 ); //exp: time:32323232value=-2
        break; case __LINE__: TRICE8     ( ID( 4863), "value=%d\n", -2 ); //exp: time:32323232value=-2
        break; case __LINE__: TRICE8_1   ( ID( 6308), "value=%d\n", -2 ); //exp: time:32323232value=-2

        break; case __LINE__: TRICE( ID( 4113), "FATAL:magenta+b:red\n" );           // exp: time:32323232FATAL:magenta+b:red
        break; case __LINE__: TRICE( ID( 6954), "CRITICAL:red+i:default+h\n" );      // exp: time:32323232CRITICAL:red+i:default+h
        break; case __LINE__: TRICE( ID( 4981), "EMERGENCY:red+i:blue\n" );          // exp: time:32323232EMERGENCY:red+i:blue
        break; case __LINE__: TRICE( ID( 2722), "ERROR:11:red\n" );                  // exp: time:32323232ERROR:11:red
        break; case __LINE__: TRICE( ID( 1138), "WARNING:11+i:red\n" );              // exp: time:32323232WARNING:11+i:red
        break; case __LINE__: TRICE( ID( 3860), "ATTENTION:11:green\n" );            // exp: time:32323232ATTENTION:11:green
        break; case __LINE__: TRICE( ID( 7130), "INFO:cyan+b:default+h\n" );         // exp: time:32323232INFO:cyan+b:default+h
        break; case __LINE__: TRICE( ID( 1570), "DEBUG:130+i\n" );                   // exp: time:32323232DEBUG:130+i
        break; case __LINE__: TRICE( ID( 6846), "TRACE:default+i:default+h\n" );     // exp: time:32323232TRACE:default+i:default+h
        break; case __LINE__: TRICE( ID( 4068), "TIME:blue+i:blue+h\n" );            // exp: time:32323232TIME:blue+i:blue+h
        break; case __LINE__: TRICE( ID( 2789), "MESSAGE:green+h:black\n" );         // exp: time:32323232MESSAGE:green+h:black
        break; case __LINE__: TRICE( ID( 3906), "READ:black+i:yellow+h\n" );         // exp: time:32323232READ:black+i:yellow+h
        break; case __LINE__: TRICE( ID( 2310), "WRITE:black+u:yellow+h\n" );        // exp: time:32323232WRITE:black+u:yellow+h
        break; case __LINE__: TRICE( ID( 1305), "RECEIVE:black+h:black\n" );         // exp: time:32323232RECEIVE:black+h:black
        break; case __LINE__: TRICE( ID( 7735), "TRANSMIT:black:black+h\n" );        // exp: time:32323232TRANSMIT:black:black+h
        break; case __LINE__: TRICE( ID( 1507), "DIAG:yellow+i:default+h\n" );       // exp: time:32323232DIAG:yellow+i:default+h
        break; case __LINE__: TRICE( ID( 6129), "INTERRUPT:magenta+i:default+h\n" ); // exp: time:32323232INTERRUPT:magenta+i:default+h
        break; case __LINE__: TRICE( ID( 1109), "SIGNAL:118+i\n" );                  // exp: time:32323232SIGNAL:118+i
        break; case __LINE__: TRICE( ID( 3062), "TEST:yellow+h:black\n" );           // exp: time:32323232TEST:yellow+h:black
        break; case __LINE__: TRICE( ID( 5272), "DEFAULT:off\n" );                   // exp: time:32323232DEFAULT:off
        break; case __LINE__: TRICE( ID( 5886), "NOTICE:blue:white+h\n" );           // exp: time:32323232NOTICE:blue:white+h
        break; case __LINE__: TRICE( ID( 5689), "ALERT:magenta:magenta+h\n" );       // exp: time:32323232ALERT:magenta:magenta+h
        break; case __LINE__: TRICE( ID( 2965), "ASSERT:yellow+i:blue\n" );          // exp: time:32323232ASSERT:yellow+i:blue
        break; case __LINE__: TRICE( ID( 2174), "ALARM:red+i:white+h\n" );           // exp: time:32323232ALARM:red+i:white+h
        break; case __LINE__: TRICE( ID( 6412), "CYCLE:blue+i:default+h\n" );        // exp: time:32323232CYCLE:blue+i:default+h
        break; case __LINE__: TRICE( ID( 3767), "VERBOSE:blue:default\n" );          // exp: time:32323232VERBOSE:blue:default
        break; case __LINE__: TRICE( ID( 6130), "fatal:magenta+b:red\n" );           // exp: time:32323232fatal:magenta+b:red
        break; case __LINE__: TRICE( ID( 6047), "critical:red+i:default+h\n" );      // exp: time:32323232critical:red+i:default+h
        break; case __LINE__: TRICE( ID( 3052), "emergency:red+i:blue\n" );          // exp: time:32323232emergency:red+i:blue
        break; case __LINE__: TRICE( ID( 1608), "error:11:red\n" );                  // exp: time:32323232error:11:red
        break; case __LINE__: TRICE( ID( 2778), "warning:11+i:red\n" );              // exp: time:32323232warning:11+i:red
        break; case __LINE__: TRICE( ID( 5065), "attention:11:green\n" );            // exp: time:32323232attention:11:green
        break; case __LINE__: TRICE( ID( 1143), "info:cyan+b:default+h\n" );         // exp: time:32323232info:cyan+b:default+h
        break; case __LINE__: TRICE( ID( 6641), "debug:130+i\n" );                   // exp: time:32323232debug:130+i
        break; case __LINE__: TRICE( ID( 7371), "trace:default+i:default+h\n" );     // exp: time:32323232trace:default+i:default+h
        break; case __LINE__: TRICE( ID( 3713), "time:blue+i:blue+h\n" );            // exp: time:32323232time:blue+i:blue+h
        break; case __LINE__: TRICE( ID( 6105), "message:green+h:black\n" );         // exp: time:32323232message:green+h:black
        break; case __LINE__: TRICE( ID( 7096), "read:black+i:yellow+h\n" );         // exp: time:32323232read:black+i:yellow+h
        break; case __LINE__: TRICE( ID( 4850), "write:black+u:yellow+h\n" );        // exp: time:32323232write:black+u:yellow+h
        break; case __LINE__: TRICE( ID( 2576), "receive:black+h:black\n" );         // exp: time:32323232receive:black+h:black
        break; case __LINE__: TRICE( ID( 6547), "transmit:black:black+h\n" );        // exp: time:32323232transmit:black:black+h
        break; case __LINE__: TRICE( ID( 2679), "diag:yellow+i:default+h\n" );       // exp: time:32323232diag:yellow+i:default+h
        break; case __LINE__: TRICE( ID( 6988), "interrupt:magenta+i:default+h\n" ); // exp: time:32323232interrupt:magenta+i:default+h
        break; case __LINE__: TRICE( ID( 3023), "signal:118+i\n" );                  // exp: time:32323232signal:118+i
        break; case __LINE__: TRICE( ID( 7490), "test:yellow+h:black\n" );           // exp: time:32323232test:yellow+h:black
        break; case __LINE__: TRICE( ID( 3362), "default:off\n" );                   // exp: time:32323232default:off
        break; case __LINE__: TRICE( ID( 4095), "notice:blue:white+h\n" );           // exp: time:32323232notice:blue:white+h
        break; case __LINE__: TRICE( ID( 3398), "alert:magenta:magenta+h\n" );       // exp: time:32323232alert:magenta:magenta+h
        break; case __LINE__: TRICE( ID( 7217), "assert:yellow+i:blue\n" );          // exp: time:32323232assert:yellow+i:blue
        break; case __LINE__: TRICE( ID( 4350), "alarm:red+i:white+h\n" );           // exp: time:32323232alarm:red+i:white+h
        break; case __LINE__: TRICE( ID( 7790), "cycle:blue+i:default+h\n" );        // exp: time:32323232cycle:blue+i:default+h
        break; case __LINE__: TRICE( ID( 2605), "verbose:blue:default\n" );          // exp: time:32323232verbose:blue:default
        break; case __LINE__: TRICE64( id( 5368), "msg:%d (%%d)\n", -1 );     //exp: time:        msg:-1 (%d)

        break; case __LINE__: TRICE( ID( 5734), "sig:TRICE with 1 to 12 values\n" );
        break; case __LINE__: TRICE( ID( 6677), "rd:TRICE %d\n", -1 );
        break; case __LINE__: TRICE( ID( 3095), "rd:TRICE %d, %d\n", -1, -2 );
        break; case __LINE__: TRICE( ID( 7420), "rd:TRICE %d, %d, %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE( ID( 2933), "rd:TRICE %d, %d, %d, %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE( ID( 4657), "rd:TRICE %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE( ID( 7505), "rd:TRICE %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE( ID( 4636), "rd:TRICE %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE( ID( 2993), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE( ID( 2765), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE( ID( 2912), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE( ID( 7210), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE( ID( 7661), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE( ID( 6655),  "sig:TRICE_n with 1 to 12 values\n" );
        break; case __LINE__: TRICE_1( ID( 5470), "rd:TRICE_1 %d\n", 1 );
        break; case __LINE__: TRICE_2( ID( 2362), "rd:TRICE_2 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE_3( ID( 6648), "rd:TRICE_3 %d, %d, %d\n", 1, 2, 3 );
        break; case __LINE__: TRICE_4( ID( 4819), "rd:TRICE_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
        break; case __LINE__: TRICE_5( ID( 5619), "rd:TRICE_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
        break; case __LINE__: TRICE_6( ID( 3803), "rd:TRICE_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
        break; case __LINE__: TRICE_7( ID( 4459), "rd:TRICE_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
        break; case __LINE__: TRICE_8( ID( 3396), "rd:TRICE_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
        break; case __LINE__: TRICE_9( ID( 5319), "rd:TRICE_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
        break; case __LINE__: TRICE_10( ID( 4894), "rd:TRICE_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
        break; case __LINE__: TRICE_11( ID( 1869), "rd:TRICE_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
        break; case __LINE__: TRICE_12( ID( 1324), "rd:TRICE_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );				

        break; case __LINE__: TRICE( ID( 1543), "sig:TRICE8 with 1 to 12 values\n" );
        break; case __LINE__: TRICE8( ID( 3463), "rd:TRICE8 %d\n", -1 );
        break; case __LINE__: TRICE8( ID( 3458), "rd:TRICE8 %d, %d\n", -1, -2 );
        break; case __LINE__: TRICE8( ID( 2688), "rd:TRICE8 %d, %d, %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE8( ID( 1210), "rd:TRICE8 %d, %d, %d, %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE8( ID( 7956), "rd:TRICE8 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE8( ID( 4837), "rd:TRICE8 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE8( ID( 6781), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE8( ID( 3872), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE8( ID( 4741), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE8( ID( 3652), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE8( ID( 2630), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE8( ID( 4694), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE( ID( 6243), "sig:TRICE8_n with 1 to 12 values\n" );
        break; case __LINE__: TRICE8_1 ( ID( 1777), "rd:TRICE8_1 %d\n", 1 );
        break; case __LINE__: TRICE8_2 ( ID( 6084), "rd:TRICE8_2 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE8_3 ( ID( 1679), "rd:TRICE8_3 %d, %d, %d\n", 1, 2, 3 );
        break; case __LINE__: TRICE8_4 ( ID( 4230), "rd:TRICE8_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
        break; case __LINE__: TRICE8_5 ( ID( 1370), "rd:TRICE8_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
        break; case __LINE__: TRICE8_6 ( ID( 3863), "rd:TRICE8_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
        break; case __LINE__: TRICE8_7 ( ID( 3180), "rd:TRICE8_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
        break; case __LINE__: TRICE8_8 ( ID( 5575), "rd:TRICE8_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
        break; case __LINE__: TRICE8_9 ( ID( 1070), "rd:TRICE8_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
        break; case __LINE__: TRICE8_10( ID( 4092), "rd:TRICE8_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
        break; case __LINE__: TRICE8_11( ID( 2350), "rd:TRICE8_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
        break; case __LINE__: TRICE8_12( ID( 5920), "rd:TRICE8_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );

        break; case __LINE__: TRICE( ID( 3788), "sig:TRICE16 with 1 to 12 values\n" );
        break; case __LINE__: TRICE16( ID( 1082), "rd:TRICE16 %d\n", -1 );
        break; case __LINE__: TRICE16( ID( 5215), "rd:TRICE16 %d, %d\n", -1, -2 );
        break; case __LINE__: TRICE16( ID( 3306), "rd:TRICE16 %d, %d, %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE16( ID( 4736), "rd:TRICE16 %d, %d, %d, %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE16( ID( 6756), "rd:TRICE16 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE16( ID( 5881), "rd:TRICE16 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE16( ID( 2258), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE16( ID( 2957), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE16( ID( 3780), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE16( ID( 4650), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE16( ID( 6600), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE16( ID( 2212), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE     ( ID( 7318), "sig:TRICE16_n with 1 to 12 values\n" );
        break; case __LINE__: TRICE16_1 ( ID( 3127), "rd:TRICE16_1 %d\n", 1 );
        break; case __LINE__: TRICE16_2 ( ID( 3763), "rd:TRICE16_2 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE16_3 ( ID( 6731), "rd:TRICE16_3 %d, %d, %d\n", 1, 2, 3 );
        break; case __LINE__: TRICE16_4 ( ID( 5409), "rd:TRICE16_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
        break; case __LINE__: TRICE16_5 ( ID( 7140), "rd:TRICE16_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
        break; case __LINE__: TRICE16_6 ( ID( 4152), "rd:TRICE16_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
        break; case __LINE__: TRICE16_7 ( ID( 1980), "rd:TRICE16_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
        break; case __LINE__: TRICE16_8 ( ID( 2624), "rd:TRICE16_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
        break; case __LINE__: TRICE16_9 ( ID( 1364), "rd:TRICE16_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
        break; case __LINE__: TRICE16_10( ID( 6070), "rd:TRICE16_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
        break; case __LINE__: TRICE16_11( ID( 7010), "rd:TRICE16_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
        break; case __LINE__: TRICE16_12( ID( 1516), "rd:TRICE16_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );

        break; case __LINE__: TRICE  ( ID( 6289), "sig:TRICE16 with 1 to 12 pointer\n" );
        break; case __LINE__: TRICE16( ID( 7453), "rd:TRICE16 %p\n", -1 );
        break; case __LINE__: TRICE16( ID( 3663), "rd:TRICE16 %p, %p\n", -1, -2 );
        break; case __LINE__: TRICE16( ID( 4814), "rd:TRICE16 %p, %p, %p\n", -1, -2, -3 );
        break; case __LINE__: TRICE16( ID( 2384), "rd:TRICE16 %p, %p, %p, %p\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE16( ID( 7947), "rd:TRICE16 %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE16( ID( 3786), "rd:TRICE16 %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE16( ID( 3503), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE16( ID( 2296), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE16( ID( 3497), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE16( ID( 1892), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE16( ID( 5818), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE16( ID( 1341), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE  ( ID( 4288), "sig:TRICE16 with 1 to 12 hex\n" );
        break; case __LINE__: TRICE16( ID( 6343), "rd:TRICE16 %X\n", -1 );
        break; case __LINE__: TRICE16( ID( 2954), "rd:TRICE16 %X, %X\n", -1, -2 );
        break; case __LINE__: TRICE16( ID( 6819), "rd:TRICE16 %X, %X, %X\n", -1, -2, -3 );
        break; case __LINE__: TRICE16( ID( 3405), "rd:TRICE16 %X, %X, %X, %X\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE16( ID( 1833), "rd:TRICE16 %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE16( ID( 5803), "rd:TRICE16 %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE16( ID( 6016), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE16( ID( 7407), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE16( ID( 6210), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE16( ID( 5393), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE16( ID( 6398), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE16( ID( 1004), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE  ( ID( 4062), "sig:TRICE32 with 1 to 12 values\n" );
        break; case __LINE__: TRICE32( ID( 2877), "rd:TRICE32 %d\n", -1 );
        break; case __LINE__: TRICE32( ID( 7865), "rd:TRICE32 %d, %d\n", -1, -2 );
        break; case __LINE__: TRICE32( ID( 4882), "rd:TRICE32 %d, %d, %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE32( ID( 6002), "rd:TRICE32 %d, %d, %d, %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE32( ID( 3612), "rd:TRICE32 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE32( ID( 7042), "rd:TRICE32 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE32( ID( 2334), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE32( ID( 6608), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE32( ID( 7510), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE32( ID( 1665), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE32( ID( 4841), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE32( ID( 2937), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE     ( ID( 1009), "signal:TRICE32_n with 1 to 12 values\n" );
        break; case __LINE__: TRICE32_1 ( ID( 6328), "rd:TRICE32_1 %d\n", 1 );
        break; case __LINE__: TRICE32_2 ( ID( 7399), "rd:TRICE32_2 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE32_3 ( ID( 7641), "rd:TRICE32_3 %d, %d, %d\n", 1, 2, 3 );
        break; case __LINE__: TRICE32_4 ( ID( 6344), "rd:TRICE32_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
        break; case __LINE__: TRICE32_5 ( ID( 5355), "rd:TRICE32_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
        break; case __LINE__: TRICE32_6 ( ID( 7038), "rd:TRICE32_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
        break; case __LINE__: TRICE32_7 ( ID( 3619), "rd:TRICE32_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
        break; case __LINE__: TRICE32_8 ( ID( 3961), "rd:TRICE32_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
        break; case __LINE__: TRICE32_9 ( ID( 2524), "rd:TRICE32_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
        break; case __LINE__: TRICE32_10( ID( 5228), "rd:TRICE32_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
        break; case __LINE__: TRICE32_11( ID( 4620), "rd:TRICE32_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
        break; case __LINE__: TRICE32_12( ID( 7941), "rd:TRICE32_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );

        break; case __LINE__: TRICE  ( ID( 4463), "sig:TRICE64 with 1 to 12 values\n" );
        break; case __LINE__: TRICE64( ID( 7481), "rd:TRICE64 %d\n", -1 );
        break; case __LINE__: TRICE64( ID( 4777), "rd:TRICE64 %d, %d\n", -1, -2 );
        break; case __LINE__: TRICE64( ID( 1633), "rd:TRICE64 %d, %d, %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE64( ID( 6927), "rd:TRICE64 %d, %d, %d, %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE64( ID( 7598), "rd:TRICE64 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE64( ID( 4178), "rd:TRICE64 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE64( ID( 4753), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE64( ID( 5269), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE64( ID( 1443), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE64( ID( 5946), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE64( ID( 2637), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE64( ID( 1173), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE     ( ID( 4671), "sig:TRICE64_n with 1 to 12 values\n" );
        break; case __LINE__: TRICE64_1 ( ID( 5822), "rd:TRICE64_1 %d\n", 1 );
        break; case __LINE__: TRICE64_2 ( ID( 7951), "rd:TRICE64_2 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE64_2 ( ID( 7616), "rd:TRICE64_2 %x, %x\n", 0x1122334455667788, 0x0102030405060708 );
        break; case __LINE__: TRICE64_3 ( ID( 4015), "rd:TRICE64_3 %d, %d, %d\n", 1, 2, 3 );
        break; case __LINE__: TRICE64_4 ( ID( 5685), "rd:TRICE64_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
        break; case __LINE__: TRICE64_5 ( ID( 5462), "rd:TRICE64_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
        break; case __LINE__: TRICE64_6 ( ID( 7855), "rd:TRICE64_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
        break; case __LINE__: TRICE64_7 ( ID( 1136), "rd:TRICE64_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
        break; case __LINE__: TRICE64_8 ( ID( 2266), "rd:TRICE64_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
        break; case __LINE__: TRICE64_9 ( ID( 7775), "rd:TRICE64_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
        break; case __LINE__: TRICE64_10( ID( 1186), "rd:TRICE64_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
        break; case __LINE__: TRICE64_11( ID( 1594), "rd:TRICE64_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
        break; case __LINE__: TRICE64_12( ID( 2976), "rd:TRICE64_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );

        break; case __LINE__: TRICE( ID( 3059), "sig:Colors and ticks\n" );
        break; case __LINE__: TRICE( ID( 6294), "--------------------------------------------------\n");
        break; case __LINE__: TRICE( ID( 5727), "--------------------------------------------------\n");
        break; case __LINE__: TRICE( ID( 7106), "dbg:12345 as 16bit is %016b\n", 12345);
        break; case __LINE__: TRICE( ID( 1141), "--------------------------------------------------\n");
        break; case __LINE__: TRICE( ID( 2533), "sig:This ASSERT error is just a demo and no real error:\n");
        break; case __LINE__: TRICE( ID( 1653), "--------------------------------------------------\n");
        break; case __LINE__: TRICE( ID( 3665), "ERR:error       message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 4040), "WRN:warning     message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 6887), "ATT:attention   message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 2073), "DIA:diagnostics message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 5350), "TIM:timing      message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 1484), "DBG:debug       message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 2109), "SIG:signal      message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 2236), "RD:read         message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 2286), "WR:write        message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 5136), "ISR:interrupt   message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 4448), "MSG:normal      message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 6164), "INFO:informal   message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID( 2643), "time:TRICE32_1  message, SysTick is %6u\n", SYSTICKVAL);

        break; case __LINE__: TRICE( ID( 3730), "sig:Various ranges and formats\n" );
        break; case __LINE__: TRICE8 ( ID( 5560), "tst:TRICE8  %%03x -> %03x  %03x  %03x  %03x\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID( 1898), "tst:TRICE8   %%4d -> %4d %4d %4d %4d\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID( 5761), "tst:TRICE8   %%4u -> %4u %4u %4u %4u\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID( 7258), "tst:TRICE8   %%4o -> %4o %4o %4o %4o\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID( 3623), "tst:TRICE8   %%4O -> %4O %4O %4O %4O\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID( 2572), "tst:TRICE8   %%4X -> %4X %4X %4X %4X\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID( 3723), "tst:TRICE8   %%4x -> %4x %4x %4x %4x\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID( 2087), "tst:TRICE8   %%8b -> %8b %8b %8b %8b\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID( 3094), "tst:TRICE8  %%08b -> %08b %08b %08b %08b\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE16( ID( 7795), "tst:TRICE16  %%05x ->   %05x   %05x   %05x   %05x\n", 1, 0x7fff, 0x8000, 0xffff);
        break; case __LINE__: TRICE16( ID( 2550), "tst:TRICE16   %%6d ->  %6d  %6d  %6d  %6d\n",         1, 0x7fff, 0x8000, 0xffff);
        break; case __LINE__: TRICE16( ID( 5435), "tst:TRICE16   %%7o -> %7o %7o %7o %7o\n",             1, 0x7fff, 0x8000, 0xffff);
        break; case __LINE__: TRICE32( ID( 1988), "tst:TRICE32 %%09x ->      %09x      %09x       %09x     %09x\n", 1, 0x7fffffff, 0x80000000, 0xffffffff);
        break; case __LINE__: TRICE32( ID( 7653), "tst:TRICE32 %%10d ->     %10d     %10d     %10d    %10x\n",      1, 0x7fffffff, 0x80000000, 0xffffffff);
        break; case __LINE__: TRICE32( ID( 2033), "att:TRICE32 %40b\n", 0xAAAAAAAA);
        break; case __LINE__: TRICE64( ID( 3967), "tst:TRICE64 %%09x ->      %09x      %09x       %09x     %09x\n", 1, 0x7fffffffffffffff, 0x8000000000000000, 0xffffffffffffffff);
        break; case __LINE__: TRICE64( ID( 6076), "tst:TRICE64 %%10d ->     %10d     %10d     %10d    %10x\n",      1, 0x7fffffffffffffff, 0x8000000000000000, 0xffffffffffffffff);
        break; case __LINE__: TRICE64( ID( 5523), "att:TRICE64 %70b\n", 0xAAAAAAAAAAAAAAAA);
        break; case __LINE__: TRICE( ID( 1821), "sig:Legacy TRICE8\n" );
        break; case __LINE__: TRICE8_1( ID( 4767), "tst:TRICE8_1 %02x\n", 0xA1);
        break; case __LINE__: TRICE8_2( ID( 4769), "tst:TRICE8_2 %02x %02x\n", 0xA1, 0xA2);
        break; case __LINE__: TRICE8_3( ID( 2365), "tst:TRICE8_3 %02x %02x %02x\n", 0xA1, 0xA2, 0xA3);
        break; case __LINE__: TRICE8_4( ID( 5879), "tst:TRICE8_4 %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4);
        break; case __LINE__: TRICE8_5( ID( 6966), "tst:TRICE8_5 %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5 );
        break; case __LINE__: TRICE8_6( ID( 3327), "tst:TRICE8_6 %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6);
        break; case __LINE__: TRICE8_7( ID( 7712), "tst:TRICE8_7 %02x %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7);
        break; case __LINE__: TRICE8_8( ID( 5985), "tst:TRICE8_8 %02x %02x %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8);
        break; case __LINE__: TRICE8_1( ID( 6936), "tst:TRICE8_1 %d\n",                      -1);
        break; case __LINE__: TRICE8_2( ID( 5424), "tst:TRICE8_2 %d %d\n",                   -1, -2);
        break; case __LINE__: TRICE8_3( ID( 6511), "tst:TRICE8_3 %d %d %d\n",                -1, -2, -3);
        break; case __LINE__: TRICE8_4( ID( 3771), "tst:TRICE8_4 %d %d %d %d\n",             -1, -2, -3, -4);
        break; case __LINE__: TRICE8_5( ID( 2143), "tst:TRICE8_5 %d %d %d %d %d\n",          -1, -2, -3, -4, -5);
        break; case __LINE__: TRICE8_6( ID( 7886), "tst:TRICE8_6 %d %d %d %d %d %d\n",       -1, -2, -3, -4, -5, -6);
        break; case __LINE__: TRICE8_7( ID( 4865), "tst:TRICE8_7 %d %d %d %d %d %d %d\n",    -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE8_8( ID( 7657), "tst:TRICE8_8 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8);
        break; case __LINE__: TRICE8_1( ID( 1128), "tst:TRICE8_1 %d\n", 1);
        break; case __LINE__: TRICE8_2( ID( 1394), "tst:TRICE8_2 %d %d\n", 1, 2);
        break; case __LINE__: TRICE8_3( ID( 4066), "tst:TRICE8_3 %d %d %d\n", 1, 2, 3);
        break; case __LINE__: TRICE8_4( ID( 6238), "tst:TRICE8_4 %d %d %d %d\n", 1, 2, 3, 4);
        break; case __LINE__: TRICE8_5( ID( 2282), "tst:TRICE8_5 %d %d %d %d %d\n", 1, 2, 3, 4, 5);
        break; case __LINE__: TRICE8_6( ID( 7229), "tst:TRICE8_6 %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6);
        break; case __LINE__: TRICE8_7( ID( 2505), "tst:TRICE8_7 %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7);
        break; case __LINE__: TRICE8_8( ID( 1998), "tst:TRICE8_8 %d %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7, 8);
        break; case __LINE__: TRICE8_1( ID( 7724), "tst:TRICE8_1 %u\n", 201);
        break; case __LINE__: TRICE8_2( ID( 4409), "tst:TRICE8_2 %u %u\n", 201, 202);
        break; case __LINE__: TRICE8_3( ID( 6607), "tst:TRICE8_3 %u %u %u\n", 201, 202, 203);
        break; case __LINE__: TRICE8_4( ID( 3035), "tst:TRICE8_4 %u %u %u %u\n", 201, 202, 203, 204);
        break; case __LINE__: TRICE8_5( ID( 2907), "tst:TRICE8_5 %u %u %u %u %u\n", 201, 202, 203, 204, 205);
        break; case __LINE__: TRICE8_6( ID( 1112), "tst:TRICE8_6 %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206);
        break; case __LINE__: TRICE8_7( ID( 2494), "tst:TRICE8_7 %u %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206, 207);
        break; case __LINE__: TRICE8_8( ID( 6991), "tst:TRICE8_8 %u %u %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206, 207, 208);
        break; case __LINE__: TRICE8_8( ID( 1152), "tst:TRICE8_1 %%d=%d, %%u=%u, 0x%%x=0x%x, 0x%%2x=0x%2x, 0x%%02x=0x%02x, 0x%%3x=0x%3x, 0x%%03x=0x%03x, %%b=%b\n", 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81);

        break; case __LINE__: TRICE    ( ID( 7609), "sig:Legacy TRICE16-64\n" );
        break; case __LINE__: TRICE16_1( ID( 7198), "tst:TRICE16_1 %d\n", -111);
        break; case __LINE__: TRICE16_2( ID( 7891), "tst:TRICE16_2 %d %d\n", -111, -222);
        break; case __LINE__: TRICE16_3( ID( 3091), "tst:TRICE16_3 %d %d %d\n", -111, -222, -333);
        break; case __LINE__: TRICE16_4( ID( 7807), "tst:TRICE16_4 %d %d %d %d\n", -111, -222, -333, -444);
        break; case __LINE__: TRICE32_1( ID( 1854), "tst:TRICE32_1 %08x\n", 0x0123cafe);
        break; case __LINE__: TRICE32_1( ID( 6825), "tst:TRICE32_1 %d\n", -111);
        break; case __LINE__: TRICE32_2( ID( 7425), "tst:TRICE32_2 %x %x\n", -111, -222);
        break; case __LINE__: TRICE32_2( ID( 2681), "tst:TRICE32_2 %d %d\n", -111, -222);
        break; case __LINE__: TRICE32_3( ID( 1420), "tst:TRICE32_3 %x %x %x\n", -111, -222, -333);
        break; case __LINE__: TRICE32_3( ID( 5998), "tst:TRICE32_3 %d %d %d\n", -111, -222, -333);
        break; case __LINE__: TRICE32_4( ID( 3600), "tst:TRICE32_4 %x %x %x %x\n", -111, -222, -333, -444);
        break; case __LINE__: TRICE32_4( ID( 5563), "tst:TRICE32_4 %d %d %d %d\n", -111, -222, -333, -444);
        break; case __LINE__: TRICE64_1( ID( 2272), "tst:TRICE64_1 %d\n", -111);
        break; case __LINE__: TRICE64_2( ID( 3264), "tst:TRICE64_2 %d %d\n", -111, -222);
        break; case __LINE__: TRICE16_1( ID( 4831), "tst:TRICE16_1 %u\n", 60001);
        break; case __LINE__: TRICE16_2( ID( 1343), "tst:TRICE16_2 %u %u\n", 60001, 60002);
        break; case __LINE__: TRICE16_3( ID( 5539), "tst:TRICE16_3 %u %u %u\n", 60001, 60002, 60003);
        break; case __LINE__: TRICE16_4( ID( 2868), "tst:TRICE16_4 %u %u %u %u\n", 60001, 60002, 60003, 60004);
        break; case __LINE__: TRICE32_1( ID( 4347), "tst:TRICE32_1 %u\n", 4000000001);
        break; case __LINE__: TRICE32_2( ID( 3610), "tst:TRICE32_2 %u %u\n", 4000000001, 4000000002);
        break; case __LINE__: TRICE32_3( ID( 6380), "tst:TRICE32_3 %u %u %u\n", 4000000001, 4000000002, 4000000003);
        break; case __LINE__: TRICE32_4( ID( 5262), "tst:TRICE32_4 %u %u %u %u\n", 4000000001, 4000000002, 4000000003, 4000000004);
        break; case __LINE__: TRICE64_1( ID( 7553), "tst:TRICE64_1 %x\n", -1);
        break; case __LINE__: TRICE64_2( ID( 4969), "tst:TRICE64_2 %x %x\n", -1, -2);
        break; case __LINE__: TRICE64_1( ID( 5207), "tst:TRICE64_1 %u\n", -1);
        break; case __LINE__: TRICE64_2( ID( 7133), "tst:TRICE64_2 %u %u\n", -1, -2);

        break; case __LINE__: TRICE( ID( 4707), "sig:colored single letters and several TRICE macros in one line\n" );
        break; case __LINE__: TRICE( ID( 7597), "e:A");
        break; case __LINE__: TRICE( ID( 3347), "w:B");
        break; case __LINE__: TRICE( ID( 1991), "a:c");
        break; case __LINE__: TRICE( ID( 6553), "wr:d");
        break; case __LINE__: TRICE( ID( 2919), "rd:e\n");
        break; case __LINE__: TRICE( ID( 3681), "diag:f");
        break; case __LINE__: TRICE( ID( 3618), "d:G");
        break; case __LINE__: TRICE( ID( 3305), "t:H");
        break; case __LINE__: TRICE( ID( 5550), "time:i");
        break; case __LINE__: TRICE( ID( 2325), "message:J");
        break; case __LINE__: TRICE( ID( 4569), "dbg:k\n");
        break; case __LINE__: TRICE( ID( 7912), "1");
        break; case __LINE__: TRICE( ID( 4943), "2");
        break; case __LINE__: TRICE( ID( 3448), "3");
        break; case __LINE__: TRICE( ID( 4165), "4");
        break; case __LINE__: TRICE( ID( 1602), "e:7");
        break; case __LINE__: TRICE( ID( 3978), "m:12");
        break; case __LINE__: TRICE( ID( 5970), "m:123\n");
        break; case __LINE__: TRICE( ID( 2663), "e:A"); TRICE( ID( 7996), "w:B"); TRICE( ID( 5847), "a:c");
        break; case __LINE__: TRICE( ID( 7698), "wr:d"); TRICE( ID( 2398), "rd:e\n"); TRICE( ID( 1194), "diag:f");

        break; case __LINE__: TRICE( ID( 1846),  "sig:TRICE8 with variable param count 1 to 12\n" );
        break; case __LINE__: TRICE8( ID( 2676), "tst:TRICE8 %d\n", -1 );
        break; case __LINE__: TRICE8( ID( 3975), "tst:TRICE8 %d %d\n", -1, -2 );
        break; case __LINE__: TRICE8( ID( 1938), "tst:TRICE8 %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE8( ID( 7024), "tst:TRICE8 %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE8( ID( 1540), "tst:TRICE8 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE8( ID( 7477), "tst:TRICE8 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE8( ID( 3617), "tst:TRICE8 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE8( ID( 2853), "tst:TRICE8 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE8( ID( 1213), "tst:TRICE8 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE8( ID( 6017), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE8( ID( 2007), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE8( ID( 5129), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE8( ID( 4455), "tst:TRICE8 %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );
        break; case __LINE__: TRICE8_1 ( ID( 5737), "tst:TRICE8_1  %d\n", -1 );
        break; case __LINE__: TRICE8_2 ( ID( 5227), "tst:TRICE8_2  %d %d\n", -1, -2 );
        break; case __LINE__: TRICE8_3 ( ID( 5231), "tst:TRICE8_3  %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE8_4 ( ID( 5753), "tst:TRICE8_4  %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE8_5 ( ID( 1749), "tst:TRICE8_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE8_6 ( ID( 5281), "tst:TRICE8_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE8_7 ( ID( 7599), "tst:TRICE8_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE8_8 ( ID( 3107), "tst:TRICE8_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE8_9 ( ID( 2759), "tst:TRICE8_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE8_10( ID( 1421), "tst:TRICE8_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE8_11( ID( 7858), "tst:TRICE8_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE8_12( ID( 3350), "tst:TRICE8_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE8   ( ID( 7936), "tst:TRICE8   %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );
        break; case __LINE__: TRICE8_9 ( ID( 1734), "tst:TRICE8_9 %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );

        break; case __LINE__: TRICE( ID( 5187), "sig:TRICE16 with variable param count 1 to 12\n" );
        break; case __LINE__: TRICE16( ID( 1085), "tst:TRICE16 %d\n", -1 );
        break; case __LINE__: TRICE16( ID( 4024), "tst:TRICE16 %d %d\n", -1, -2 );
        break; case __LINE__: TRICE16( ID( 7015), "tst:TRICE16 %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE16( ID( 3968), "tst:TRICE16 %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE16( ID( 3132), "tst:TRICE16 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE16( ID( 5153), "tst:TRICE16 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE16( ID( 3151), "tst:TRICE16 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE16( ID( 1135), "tst:TRICE16 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE16( ID( 7058), "tst:TRICE16 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE16( ID( 3785), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE16( ID( 6122), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE16( ID( 7156), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE16( ID( 2072), "tst:TRICE16 %t %b %x %X %d %u %o %O %p\n", 3, 3, 3, 3, 3, 3, 3, 3, 3 );
        break; case __LINE__: TRICE16_1 ( ID( 2482), "tst:TRICE16_1  %d\n", -1 );
        break; case __LINE__: TRICE16_2 ( ID( 7408), "tst:TRICE16_2  %d %d\n", -1, -2 );
        break; case __LINE__: TRICE16_3 ( ID( 5520), "tst:TRICE16_3  %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE16_4 ( ID( 6518), "tst:TRICE16_4  %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE16_5 ( ID( 3601), "tst:TRICE16_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE16_6 ( ID( 3178), "tst:TRICE16_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE16_7 ( ID( 5379), "tst:TRICE16_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE16_8 ( ID( 5417), "tst:TRICE16_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE16_9 ( ID( 7305), "tst:TRICE16_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE16_10( ID( 2657), "tst:TRICE16_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE16_11( ID( 6504), "tst:TRICE16_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE16_12( ID( 1099), "tst:TRICE16_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE16_9 ( ID( 1941), "tst:TRICE16 %t %b %x %X %d %u %o %O %p\n", 3, 3, 3, 3, 3, 3, 3, 3, 3 );

        break; case __LINE__: TRICE( ID( 4786), "sig:TRICE32 with variable param count 1 to 12\n" );
        break; case __LINE__: TRICE32( ID( 2306), "tst:TRICE32 %d\n", -1 );
        break; case __LINE__: TRICE32( ID( 5356), "tst:TRICE32 %d %d\n", -1, -2 );
        break; case __LINE__: TRICE32( ID( 1774), "tst:TRICE32 %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE32( ID( 5544), "tst:TRICE32 %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE32( ID( 6429), "tst:TRICE32 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE32( ID( 1684), "tst:TRICE32 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE32( ID( 4609), "tst:TRICE32 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE32( ID( 7774), "tst:TRICE32 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE32( ID( 2205), "tst:TRICE32 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE32( ID( 6848), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE32( ID( 6757), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE32( ID( 2433), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE32( ID( 3931), "tst:TRICE32 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));
        break; case __LINE__: TRICE32_1 ( ID( 3953), "tst:TRICE32_1  %d\n", -1 );
        break; case __LINE__: TRICE32_2 ( ID( 3634), "tst:TRICE32_2  %d %d\n", -1, -2 );
        break; case __LINE__: TRICE32_3 ( ID( 6138), "tst:TRICE32_3  %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE32_4 ( ID( 3402), "tst:TRICE32_4  %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE32_5 ( ID( 1401), "tst:TRICE32_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE32_6 ( ID( 5274), "tst:TRICE32_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE32_7 ( ID( 1563), "tst:TRICE32_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE32_8 ( ID( 7867), "tst:TRICE32_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE32_9 ( ID( 4726), "tst:TRICE32_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE32_10( ID( 4235), "tst:TRICE32_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE32_11( ID( 7779), "tst:TRICE32_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE32_12( ID( 7570), "tst:TRICE32_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE32   ( ID( 2984), "tst:TRICE32    %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));
        break; case __LINE__: TRICE32_12( ID( 2018), "tst:TRICE32_12 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));

        break; case __LINE__: TRICE( ID( 6660), "sig:TRICE64 with variable param count 1 to 12\n" );
        break; case __LINE__: TRICE64( ID( 7828), "tst:TRICE64 %d\n", -1 );
        break; case __LINE__: TRICE64( ID( 6844), "tst:TRICE64 %d %d\n", -1, -2 );
        break; case __LINE__: TRICE64( ID( 7361), "tst:TRICE64 %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE64( ID( 2281), "tst:TRICE64 %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE64( ID( 6873), "tst:TRICE64 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE64( ID( 3543), "tst:TRICE64 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE64( ID( 5754), "tst:TRICE64 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE64( ID( 1688), "tst:TRICE64 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE64( ID( 2025), "tst:TRICE64 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE64( ID( 1915), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE64( ID( 2160), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE64( ID( 1509), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE64( ID( 3761), "tst:TRICE64 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aDouble(3.14159), aDouble(3.14159), aDouble(3.14159));
        break; case __LINE__: TRICE64_1 ( ID( 4408), "tst:TRICE64_1  %d\n", -1 );
        break; case __LINE__: TRICE64_2 ( ID( 7686), "tst:TRICE64_2  %d %d\n", -1, -2 );
        break; case __LINE__: TRICE64_3 ( ID( 3391), "tst:TRICE64_3  %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE64_4 ( ID( 3226), "tst:TRICE64_4  %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE64_5 ( ID( 7949), "tst:TRICE64_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE64_6 ( ID( 5533), "tst:TRICE64_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE64_7 ( ID( 3719), "tst:TRICE64_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE64_8 ( ID( 2077), "tst:TRICE64_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE64_9 ( ID( 2032), "tst:TRICE64_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE64_10( ID( 2446), "tst:TRICE64_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE64_11( ID( 3301), "tst:TRICE64_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE64_12( ID( 3277), "tst:TRICE64_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE64_12( ID( 7915), "tst:TRICE64_12 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aDouble(3.14159), aDouble(3.14159), aDouble(3.14159));

        break; case __LINE__: TRICE( ID( 4342), "att:positive and negative float in format variants\n");
        break; case __LINE__: TRICE( ID( 3224), "rd:TRICE float %f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 3955), "rd:TRICE float %9f (%%9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 3113), "rd:TRICE float %.9f (%%.9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 3417), "rd:TRICE float %9.f (%%9.f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 2193), "rd:TRICE float %9.6f (%%9.6f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 4493), "rd:TRICE float %f (%%f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 5354), "rd:TRICE float %9f (%%9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 7197), "rd:TRICE float %.9f (%%.9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 7890), "rd:TRICE float %9.f (%%9.f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 2213), "rd:TRICE float %9.6f (%%9.6f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 1956), "rd:TRICE float %+f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 1450), "rd:TRICE float %+9f (%%9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 3459), "rd:TRICE float %+.9f (%%.9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 3574), "rd:TRICE float %+9.f (%%9.f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 6800), "rd:TRICE float %+9.6f (%%9.6f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 2597), "rd:TRICE float %+f (%%f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 6903), "rd:TRICE float %+9f (%%9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 4647), "rd:TRICE float %+.9f (%%.9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 3290), "rd:TRICE float %+9.f (%%9.f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 4041), "rd:TRICE float %+9.6f (%%9.6f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 1601), "rd:TRICE float %-f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 3348), "rd:TRICE float %-9f (%%9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 4077), "rd:TRICE float %-.9f (%%.9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 3036), "rd:TRICE float %-9.f (%%9.f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 7721), "rd:TRICE float %-9.6f (%%9.6f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID( 2082), "rd:TRICE float %-f (%%f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 1975), "rd:TRICE float %-9f (%%9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 6182), "rd:TRICE float %-.9f (%%.9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 5119), "rd:TRICE float %-9.f (%%9.f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 2216), "rd:TRICE float %-9.6f (%%9.6f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 2056), "att:positive float & double in variants\n");
        break; case __LINE__: TRICE32_1( ID( 1637), "rd:TRICE32_1 float %e (%%e)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1( ID( 5620), "rd:TRICE32_1 float %f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1( ID( 2030), "rd:TRICE32_1 float %g (%%g)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1( ID( 1862), "rd:TRICE32_1 float %E (%%E)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1( ID( 3536), "rd:TRICE32_1 float %F (%%F)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1( ID( 4762), "rd:TRICE32_1 float %G (%%G)\n", aFloat(x) );
        break; case __LINE__: TRICE32  ( ID( 7295), "rd:TRICE32 float %e (%%e)\n", aFloat(x) );
        break; case __LINE__: TRICE32  ( ID( 2274), "rd:TRICE32 float %f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE32  ( ID( 3824), "rd:TRICE32 float %g (%%g)\n", aFloat(x) );
        break; case __LINE__: TRICE32  ( ID( 4004), "rd:TRICE32 float %E (%%E)\n", aFloat(x) );
        break; case __LINE__: TRICE32  ( ID( 5125), "rd:TRICE32 float %F (%%F)\n", aFloat(x) );
        break; case __LINE__: TRICE32  ( ID( 6124), "rd:TRICE32 float %G (%%G)\n", aFloat(x) );
        break; case __LINE__: TRICE    ( ID( 3814), "rd:TRICE float %e (%%e)\n", aFloat(x) );
        break; case __LINE__: TRICE    ( ID( 7586), "rd:TRICE float %f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE    ( ID( 7938), "rd:TRICE float %g (%%g)\n", aFloat(x) );
        break; case __LINE__: TRICE    ( ID( 7390), "rd:TRICE float %E (%%E)\n", aFloat(x) );
        break; case __LINE__: TRICE    ( ID( 3240), "rd:TRICE float %F (%%F)\n", aFloat(x) );
        break; case __LINE__: TRICE    ( ID( 1361), "rd:TRICE float %G (%%G)\n", aFloat(x) );
        break; case __LINE__: TRICE64  ( ID( 5565), "rd:TRICE64 double %e (%%e), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64  ( ID( 3207), "rd:TRICE64 double %f (%%f), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64  ( ID( 4450), "rd:TRICE64 double %g (%%g), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64  ( ID( 7582), "rd:TRICE64 double %E (%%E), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64  ( ID( 2950), "rd:TRICE64 double %F (%%F), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64  ( ID( 6636), "rd:TRICE64 double %G (%%G), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( ID( 5681), "rd:TRICE64_1 double %e (%%e), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( ID( 4076), "rd:TRICE64_1 double %f (%%f), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( ID( 1758), "rd:TRICE64_1 double %g (%%g), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( ID( 7193), "rd:TRICE64_1 double %E (%%E), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( ID( 5070), "rd:TRICE64_1 double %F (%%F), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( ID( 2956), "rd:TRICE64_1 double %G (%%G), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE( ID( 4692), "att:negative float & double\n" );
        break; case __LINE__: TRICE( ID( 1037), "rd:TRICE float %e (%%e)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 6119), "rd:TRICE float %f (%%f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 4714), "rd:TRICE float %g (%%g)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 3733), "rd:TRICE float %E (%%E)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 2880), "rd:TRICE float %F (%%F)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID( 7710), "rd:TRICE float %G (%%G)\n", aFloat(-x) );
        break; case __LINE__: TRICE64( ID( 2144), "rd:TRICE64 double %e (%%e), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64( ID( 7060), "rd:TRICE64 double %f (%%f), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64( ID( 7942), "rd:TRICE64 double %g (%%g), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64( ID( 4742), "rd:TRICE64 double %E (%%E), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64( ID( 4209), "rd:TRICE64 double %F (%%F), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64( ID( 5742), "rd:TRICE64 double %G (%%G), aDouble(y)\n", aDouble(-y) );

        break; case __LINE__: TRICE  ( ID( 3917), "att:formatted float & double\n" );
        break; case __LINE__: TRICE32( ID( 3181), "rd:TRICE32 float %12.6e (%%12.6e)\n", aFloat(x) );
        break; case __LINE__: TRICE32( ID( 4540), "rd:TRICE32 float %12.6f (%%12.6f)\n", aFloat(x) );
        break; case __LINE__: TRICE32( ID( 6743), "rd:TRICE32 float %12.6g (%%12.6g)\n", aFloat(x) );
        break; case __LINE__: TRICE32( ID( 5976), "rd:TRICE32 float %12.6E (%%12.6E)\n", aFloat(x) );
        break; case __LINE__: TRICE32( ID( 4900), "rd:TRICE32 float %12.6F (%%12.6F)\n", aFloat(x) );
        break; case __LINE__: TRICE32( ID( 5371), "rd:TRICE32 float %12.6G (%%12.6G)\n", aFloat(x) );
        break; case __LINE__: TRICE64( ID( 4494), "rd:TRICE64 double %12.6e (%%12.6e), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64( ID( 3930), "rd:TRICE64 double %12.6f (%%12.6f), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64( ID( 7415), "rd:TRICE64 double %12.6g (%%12.6g), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64( ID( 7762), "rd:TRICE64 double %12.6E (%%12.6E), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64( ID( 6810), "rd:TRICE64 double %12.6F (%%12.6F), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64( ID( 1150), "rd:TRICE64 double %12.6G (%%12.6G), aDouble(y)\n", aDouble(y) );

        break; case __LINE__: TRICE ( ID( 6724), "att:Various single arguments\n" );
        break; case __LINE__: TRICE8( ID( 6596), "rd:TRICE8 line %t (%%t ,0)\n", 0 );
        break; case __LINE__: TRICE8( ID( 1464), "rd:TRICE8 line %t (%%t ,2)\n", 2 );
        break; case __LINE__: TRICE8( ID( 5322), "rd:TRICE8 line %u (%%u ,-1)\n", -1 );
        break; case __LINE__: TRICE8( ID( 2554), "rd:TRICE8 line %b (%%b ,-1)\n", -1 );
        break; case __LINE__: TRICE8( ID( 2177), "rd:TRICE8 line %o (%%o ,-1)\n", -1 );
        break; case __LINE__: TRICE8( ID( 4853), "rd:TRICE8 line %O (%%O ,-1)\n", -1 );
        break; case __LINE__: TRICE8( ID( 4825), "rd:TRICE8 line %X (%%X ,-1)\n", -1 );
        break; case __LINE__: TRICE8( ID( 3342), "rd:TRICE8 line %x (%%x ,-1)\n", -1 );
        break; case __LINE__: TRICE8( ID( 6143), "rd:TRICE8 line %d (%%d ,-1)\n", -1 );
        break; case __LINE__: TRICE16_1( ID( 2119), "rd:TRICE16_1 line %t (%%t ,0)\n", 0 );
        break; case __LINE__: TRICE16_1( ID( 2720), "rd:TRICE16_1 line %t (%%t ,2)\n", 2 );
        break; case __LINE__: TRICE16_1( ID( 7358), "rd:TRICE16_1 line %u (%%u -1)\n", -1 );
        break; case __LINE__: TRICE16_1( ID( 1348), "rd:TRICE16_1 line %b (%%b -1)\n", -1 );
        break; case __LINE__: TRICE16_1( ID( 3015), "rd:TRICE16_1 line %o (%%o -1)\n", -1 );
        break; case __LINE__: TRICE16_1( ID( 1062), "rd:TRICE16_1 line %x (%%x -1)\n", -1 );
        break; case __LINE__: TRICE16_1( ID( 2302), "rd:TRICE16_1 line %d (%%d -1)\n", -1 );
        break; case __LINE__: TRICE( ID( 3324), "rd:TRICE line %t (%%t -1)\n", 0 );
        break; case __LINE__: TRICE( ID( 1275), "rd:TRICE line %t (%%t -1)\n", 2 );
        break; case __LINE__: TRICE( ID( 6746), "rd:TRICE line %u (%%u)\n", -1 );
        break; case __LINE__: TRICE( ID( 5779), "rd:TRICE line %b (%%b)\n", -2 );
        break; case __LINE__: TRICE( ID( 7164), "rd:TRICE line %o (%%o)\n", -3 );
        break; case __LINE__: TRICE( ID( 4189), "rd:TRICE line %x (%%x)\n", -4 );
        break; case __LINE__: TRICE( ID( 2151), "rd:TRICE line %d (%%d)\n", -5 );

        break; case __LINE__: TRICE_N( ID( 1619), "msg:%s\n", A, 0 );
        break; case __LINE__: TRICE_N( ID( 4115), "msg:%s\n", A, 1 );
        break; case __LINE__: TRICE_N( ID( 7475), "msg:%s\n", A, 2 );
        break; case __LINE__: TRICE_N( ID( 2222), "msg:%s\n", A, 3 );
        break; case __LINE__: TRICE_N( ID( 2601), "msg:%s\n", A, 4 );
        break; case __LINE__: TRICE_N( ID( 3183), "msg:%s\n", A, 5 );
        break; case __LINE__: TRICE_N( ID( 4673), "msg:%s\n", A, 6 );
        break; case __LINE__: TRICE_N( ID( 4084), "msg:%s\n", A, 7 );
        break; case __LINE__: TRICE_N( ID( 4087), "msg:%s\n", A, 8 );
        break; case __LINE__: TRICE_N( ID( 7494), "msg:%s\n", A, 9 );
        break; case __LINE__: TRICE_N( ID( 2078), "msg:%s\n", A, 10 );
        break; case __LINE__: TRICE_N( ID( 2621), "msg:%s\n", A, 11 );
        break; case __LINE__: TRICE_N( ID( 2740), "msg:%s\n", A, 12 );
        break; case __LINE__: TRICE_N( ID( 1715), "msg:%s\n", A, 13 );
        break; case __LINE__: TRICE_N( ID( 6721), "msg:%s\n", A, 14 );
        break; case __LINE__: TRICE_N( ID( 4611), "msg:%s\n", A, 15 );
        break; case __LINE__: TRICE_N( ID( 2796), "msg:%s\n", A, 16 );
        break; case __LINE__: TRICE_N( ID( 6457), "msg:%s\n", A, 17 );
        break; case __LINE__: TRICE_N( ID( 6986), "msg:%s\n", A, 18 );
        break; case __LINE__: TRICE_N( ID( 6501), "msg:%s\n", A, 19 );
        break; case __LINE__: TRICE_N( ID( 2288), "msg:%s\n", A, 120 );
        break; case __LINE__: TRICE_N( ID( 2747), "msg:%s\n", A, 121 );
        break; case __LINE__: TRICE_N( ID( 4271), "msg:%s\n", A, 122 );
        break; case __LINE__: TRICE_N( ID( 1356), "msg:%s\n", A, 123 );
        break; case __LINE__: TRICE_N( ID( 2775), "msg:%s\n", A, 124 );
        break; case __LINE__: TRICE_N( ID( 7648), "msg:%s\n", A, 125 );
        break; case __LINE__: TRICE_N( ID( 4520), "msg:%s\n", A, 126 );
        break; case __LINE__: TRICE_N( ID( 1675), "msg:%s\n", A, 127 );
        break; case __LINE__: TRICE_N( ID( 6434), "msg:%s\n", A, 128 );
        break; case __LINE__: TRICE_N( ID( 5131), "msg:%s\n", A, 129 );

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
#define SCOPY( element ) do{ char* n = #element; int size = sizeof( src->element ); memcpy( p, &(src->element), size ); p += size; TRICE_S( ID( 7678), "rd:sizeof(%8s)", n ); TRICE( ID( 1181), " = %d\n", size);}while(0);

    //! DCOPY is a helper macro for struct deserialization.
#define DCOPY( element ) do{ char* n = #element; int size = sizeof( dst->element ); memcpy( &(dst->element), p, size ); p += size; TRICE_S( ID( 2278), "rd:sizeof(%8s)", n ); TRICE( ID( 6448), " = %d\n", size);}while(0);


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
    TRICE ( Id( 2935), "inf: Tryout tx struct:" );
    TRICE8_B( Id( 2250), " %02x ", &tx, sizeof(tx) );
    TRICE( Id( 3784), "\n" );

    TRICE ( Id( 5749), "inf: Tryout buffer:" );
    TRICE8_B( Id( 7153), " %02x ", dst, len ); //lint !e670
    TRICE( Id( 7278), "\n" );

    src = dst; // "data transfer"

    len = deserializeTryout( &rx, src );
    TRICE ( Id( 1524), "inf: Tryout rx struct:" );
    TRICE8_B( Id( 4145), " %02x ", &rx, sizeof(rx) );
    TRICE( Id( 5454), "\n" );

    TRICE( Id( 6110), "inf:sizeOf(Trypout) = %d, buffer length = %d\n", sizeof(tx), len );
    TRICE8_F( Id( 5693), "info:TryoutStructFunction", &tx, sizeof(tx) );
    TRICE8_F( Id( 6064), "info:TryoutBufferFunction", dst, len ); //lint !e670
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
        TRice16( iD( 4438), "MSG: ?� STOP  TriceDepth = %d, TriceDepthMax = %d + %4u of %d\n", triceDepth, TRICE_DATA_OFFSET, triceDepthMax-TRICE_DATA_OFFSET, TRICE_HALF_BUFFER_SIZE );
    } 
    #endif
    #if TRICE_MODE == TRICE_STREAM_BUFFER
        TRice( iD( 7783), "MSG:triceFifoDepthMax = %d of max %d, triceStreamBufferDepthMax = %d of max %d\n", triceFifoDepthMax, TRICE_FIFO_ELEMENTS, triceStreamBufferDepthMax, TRICE_BUFFER_SIZE );
    #endif
}

void TriceLogDepthMax( void ){
    #if TRICE_MODE == TRICE_DOUBLE_BUFFER
        size_t tdm = TriceDepthMax();
        if( tdm <= TRICE_HALF_BUFFER_SIZE ){
            TRice16( iD( 5180), "diag:TriceDepthMax =%4u of %d\n", tdm, TRICE_HALF_BUFFER_SIZE );
        }else{
            TRice16( iD( 3507), "err:TriceDepthMax =%4u of %d (overflow!)\n", tdm, TRICE_HALF_BUFFER_SIZE );
        }
    #endif
}

