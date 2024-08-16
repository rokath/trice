/*! \file triceCheck.c
\brief trices for tool evaluation
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/
#include <float.h>
#define TRICE_OFF 0 // Set this value to 1 to disable trice code generation in this file object.
#include "trice.h"

// The strings behind "//exp:" are the expected result for each line (-color=none)

static int32_t FloatToInt32( float f );
static int64_t DoubleToInt64( double f );
static void exampleOfManualSerialization( void );
static void exampleOfManualJSONencoding(void);
static void dynString( int n );

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
    char * five = "five";
    #if TRICE_CGO == 1
    char* A = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
    #endif // #if TRICE_CGO == 1

    unsigned anchorLine = __LINE__ + 20; // The offset value here must point to the first test line containing `anchorLine`.

    switch( n ){ 
        default:
        break; case __LINE__: trice16( iD(14000), "att: line %u\n", __LINE__ );
        // Lines ending with a comment starting with `//exp:` are executed during trhe automatic Go tests.
		break; case __LINE__: TRice( iD(14001), "Hello World!\n" );                                              //exp: time: 842,150_450default: Hello World!		
		// normal use cases
        break; case __LINE__: TRice( iD(14002), "info:This is a message without values and a 32-bit stamp.\n" ); //exp: time: 842,150_450default: info:This is a message without values and a 32-bit stamp.
        break; case __LINE__: Trice( iD(14003), "info:This is a message without values and a 16-bit stamp.\n" ); //exp: time:       5_654default: info:This is a message without values and a 16-bit stamp.
        break; case __LINE__: trice( iD(14004), "info:This is a message without values and without stamp.\n"  ); //exp: time:            default: info:This is a message without values and without stamp.
        break; case __LINE__: for( int i = 126; i < 130; i++ ){ dynString(i); }








        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:01
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:012
		break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123
		break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:01234
		break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:012345
		break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456
		break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:01234567
		break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:012345678
		break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789
		break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789a
		break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789ab
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789abc
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789abcd
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789abcde
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789abcdef
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789abcdefg
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789abcdefgh
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789abcdefghi
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789abcdefghij
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789abcdefghijk
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789abcdefghijkl
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789abcdefghijklm
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789abcdefghijklmn
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789abcdefghijklmno
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789abcdefghijklmnop
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789abcdefghijklmnopq
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789abcdefghijklmnopqr
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789abcdefghijklmnopqrs
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789abcdefghijklmnopqrst
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789abcdefghijklmnopqrstu
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789abcdefghijklmnopqrstuv
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789abcdefghijklmnopqrstuvw
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789abcdefghijklmnopqrstuvwx
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789abcdefghijklmnopqrstuvwxy
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789abcdefghijklmnopqrstuvwxyz
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789abcdefghijklmnopqrstuvwxyzA
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789abcdefghijklmnopqrstuvwxyzAB
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789abcdefghijklmnopqrstuvwxyzABC
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789abcdefghijklmnopqrstuvwxyzABCD
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789abcdefghijklmnopqrstuvwxyzABCDE
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789abcdefghijklmnopqrstuvwxyzABCDEF
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789abcdefghijklmnopqrstuvwxyzABCDEFG
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789abcdefghijklmnopqrstuvwxyzABCDEFGH
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHI
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJ
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJK
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKL
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLM
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMN
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNO
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOP
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQ
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQR
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRS
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRST
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTU
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUV
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVW
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWX
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXY
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:            default: wr:0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.,

        // special use cases
        break; case __LINE__: TRice64( iD(14005), "info:12 64-bit values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 32-bit stamp.\n", -3, -4, -5, -6, 0, aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), -8 ); //exp: time: 842,150_450default: info:12 64-bit values -3, 18446744073709551612, fffffffffffffffb, FFFFFFFFFFFFFFFA, false, -7.123457e+00, -7.123457, -7.123456789, -7.123457E+00, -7.123457, -7.123456789, 0xb1111111111111111111111111111111111111111111111111111111111111000 and a 32-bit stamp.
        break; case __LINE__: Trice64( iD(14006), "info:12 64-bit values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 16-bit stamp.\n", -3, -4, -5, -6, 0, aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), -8 ); //exp: time:       5_654default: info:12 64-bit values -3, 18446744073709551612, fffffffffffffffb, FFFFFFFFFFFFFFFA, false, -7.123457e+00, -7.123457, -7.123456789, -7.123457E+00, -7.123457, -7.123456789, 0xb1111111111111111111111111111111111111111111111111111111111111000 and a 16-bit stamp.
        break; case __LINE__: trice64( iD(14007), "info:12 64-bit values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and without stamp.\n" , -3, -4, -5, -6, 0, aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), -8 ); //exp: time:            default: info:12 64-bit values -3, 18446744073709551612, fffffffffffffffb, FFFFFFFFFFFFFFFA, false, -7.123457e+00, -7.123457, -7.123456789, -7.123457E+00, -7.123457, -7.123456789, 0xb1111111111111111111111111111111111111111111111111111111111111000 and without stamp.
        break; case __LINE__: TRice32( iD(14008), "info:12 32-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 32-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time: 842,150_450default: info:12 32-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 32-bit stamp.
        break; case __LINE__: Trice32( iD(14009), "info:12 32-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 16-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:       5_654default: info:12 32-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 16-bit stamp.
        break; case __LINE__: trice32( iD(14010), "info:12 32-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and without stamp.\n" , -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:            default: info:12 32-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and without stamp.
        break; case __LINE__: TRice16( iD(14011), "info:12 16-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 32-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time: 842,150_450default: info:12 16-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 32-bit stamp.
        break; case __LINE__: Trice16( iD(14012), "info:12 16-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 16-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:       5_654default: info:12 16-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 16-bit stamp.
        break; case __LINE__: trice16( iD(14013), "info:12 16-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and without stamp.\n" , -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:            default: info:12 16-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and without stamp.
        break; case __LINE__: TRice8 ( iD(14014), "info:12  8-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 32-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time: 842,150_450default: info:12  8-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 32-bit stamp.
        break; case __LINE__: Trice8 ( iD(14015), "info:12  8-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 16-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:       5_654default: info:12  8-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 16-bit stamp.
        break; case __LINE__: trice8 ( iD(14016), "info:12  8-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and without stamp.\n" , -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:            default: info:12  8-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and without stamp.

        break; case __LINE__: trice16( iD(14017), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRice16( iD(14018), "att: 0x8888 == %04xh\n", 0x8888 ); //exp: time: 842,150_450default: att: 0x8888 == 8888h
        break; case __LINE__: Trice16( iD(14019), "att: 0x8888 == %04xh\n", 0x8888 ); //exp: time:       5_654default: att: 0x8888 == 8888h
        break; case __LINE__: trice16( iD(14020), "att: 0x8888 == %04xh\n", 0x8888 ); //exp: time:            default: att: 0x8888 == 8888h


        break; case __LINE__: trice16( iD(14021), "att: line %u\n", __LINE__ );

        break; case __LINE__: trice8   ( iD(14022), "value=%x\n", 0x55 );             //exp: time:            default: value=55
        break; case __LINE__: trice8   ( iD(14023), "value=%x\n", 0x88 );             //exp: time:            default: value=88

        break; case __LINE__: TRICE( ID(14024), "att:mixed int & float & double & bits\n" );
        break; case __LINE__: TRICE32( ID(14025), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",       FloatToInt32(x),   aFloat(x),   aFloat(x),   aFloat(x) ); //exp: time: 842,150_450default: rd:TRICE32 int 1089, float 1089.608276 (%f), 44883377, 01000100100010000011001101110111                                         
    /**/break; case __LINE__: TRICE32( ID(14026), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",          (int32_t)(x),   aFloat(x),   aFloat(x),   aFloat(x) ); //exp: time: 842,150_450default: rd:TRICE32 int 1089, float 1089.608276 (%f), 44883377, 01000100100010000011001101110111                                         
        break; case __LINE__: TRICE32( ID(14027), "rd:TRICE32 int %x, float %f (%%f), %08x, %032b\n",            0x44883377,   aFloat(x),   aFloat(x),   aFloat(x) ); //exp: time: 842,150_450default: rd:TRICE32 int 44883377, float 1089.608276 (%f), 44883377, 01000100100010000011001101110111                                         
        break; case __LINE__: TRICE32( ID(14028), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",                     x,   aFloat(x),   aFloat(x),   aFloat(x) ); //exp: time: 842,150_450default: rd:TRICE32 int 1089, float 1089.608276 (%f), 44883377, 01000100100010000011001101110111                                         
        break; case __LINE__: TRICE32( ID(14029), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",      FloatToInt32(-x),  aFloat(-x),  aFloat(-x),  aFloat(-x) ); //exp: time: 842,150_450default: rd:TRICE32 int -1089, float -1089.608276 (%f), c4883377, 11000100100010000011001101110111                                       
        break; case __LINE__: TRICE32( ID(14030), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",         (int32_t)(-x),  aFloat(-x),  aFloat(-x),  aFloat(-x) ); //exp: time: 842,150_450default: rd:TRICE32 int -1089, float -1089.608276 (%f), c4883377, 11000100100010000011001101110111                                       
    ////break; case __LINE__: TRICE32( ID(14031), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",                    -x,  aFloat(-x),  aFloat(-x),  aFloat(-x) ); //exp: time: 842,150_450default: rd:TRICE32 int 1089, float -1089.608276 (%f), c4883377, 11000100100010000011001101110111                                        
        break; case __LINE__: TRICE32( ID(14032), "rd:TRICE32 int %X, float %f (%%f), %08x, %032b\n",           -0x44883377,  aFloat(-x),  aFloat(-x),  aFloat(-x) ); //exp: time: 842,150_450default: rd:TRICE32 int BB77CC89, float -1089.608276 (%f), c4883377, 11000100100010000011001101110111                                        
        break; case __LINE__: TRICE64( ID(14033), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",    DoubleToInt64(y),  aDouble(y),  aDouble(y),  aDouble(y) ); //exp: time: 842,150_450default: rd:TRICE64 int 518, double 518.054749 (%f), 4080307020601050, 0100000010000000001100000111000000100000011000000001000001010000   
        break; case __LINE__: TRICE64( ID(14034), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",        (int64_t)(y),  aDouble(y),  aDouble(y),  aDouble(y) ); //exp: time: 842,150_450default: rd:TRICE64 int 518, double 518.054749 (%f), 4080307020601050, 0100000010000000001100000111000000100000011000000001000001010000   
        break; case __LINE__: TRICE64( ID(14035), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",                   y,  aDouble(y),  aDouble(y),  aDouble(y) ); //exp: time: 842,150_450default: rd:TRICE64 int 518, double 518.054749 (%f), 4080307020601050, 0100000010000000001100000111000000100000011000000001000001010000   
        break; case __LINE__: TRICE64( ID(14036), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",   DoubleToInt64(-y), aDouble(-y), aDouble(-y), aDouble(-y) ); //exp: time: 842,150_450default: rd:TRICE64 int -518, double -518.054749 (%f), c080307020601050, 1100000010000000001100000111000000100000011000000001000001010000
        break; case __LINE__: TRICE64( ID(14037), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",       (int64_t)(-y), aDouble(-y), aDouble(-y), aDouble(-y) ); //exp: time: 842,150_450default: rd:TRICE64 int -518, double -518.054749 (%f), c080307020601050, 1100000010000000001100000111000000100000011000000001000001010000
    ////break; case __LINE__: TRICE64( ID(14038), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",                  -y, aDouble(-y), aDouble(-y), aDouble(-y) ); //exp: time: 842,150_450default: rd:TRICE64 int 518, double -518.054749 (%f), c080307020601050, 1100000010000000001100000111000000100000011000000001000001010000 
        break; case __LINE__: TRICE64( ID(14039), "rd:TRICE64 int %X, double %f (%%f), %016x, %064b\n", -0x4080307020601050, aDouble(-y), aDouble(-y), aDouble(-y) ); //exp: time: 842,150_450default: rd:TRICE64 int BF7FCF8FDF9FEFB0, double -518.054749 (%f), c080307020601050, 1100000010000000001100000111000000100000011000000001000001010000 

        break; case __LINE__: trice16( iD(14040), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRICE_S( ID(14041), "msg:With TRICE_S:%s\n", sABCDE );             //exp: time: 842,150_450default: msg:With TRICE_S:abcde 12345
        break; case __LINE__: TRICE_N( ID(14042), "sig:With TRICE_N:%s\n", sABCDE, lenABCDE );   //exp: time: 842,150_450default: sig:With TRICE_N:abcde 12345
        break; case __LINE__: TRICE8_B( ID(14043), "  %02x", sABCDE, lenABCDE );                 //exp: time: 842,150_450default:   61  62  63  64  65  20  31  32  33  34  35
        break; case __LINE__: trice( iD(14044), "\n" );
        break; case __LINE__: TRICE8_B( ID(14045), "BUF: %02x\n", sABCDE, lenABCDE );            //exp: time: 842,150_450default: BUF: 61 62 63 64 65 20 31 32 33 34 35
        break; case __LINE__: TRICE8_B( ID(14046), "%4d", sABCDE, lenABCDE );                    //exp: time: 842,150_450default:   97  98  99 100 101  32  49  50  51  52  53
        break; case __LINE__: trice( iD(14047), "\n" );
        break; case __LINE__: TRICE8_B( ID(14048), "  %02x", b8, sizeof(b8)/sizeof(int8_t) );    //exp: time: 842,150_450default:   00  ff  fe  33  04  05  06  07  08  09  0a  0b  00  ff  fe  33  04  05  06  07  08  09  0a  0b
        break; case __LINE__: trice( iD(14049), "\n" );

        break; case __LINE__: TRice8B( iD(14050), " %02x", b8,  sizeof(b8) /sizeof(int8_t) );    //exp: time: 842,150_450default:  00 ff fe 33 04 05 06 07 08 09 0a 0b 00 ff fe 33 04 05 06 07 08 09 0a 0b
        break; case __LINE__: trice( iD(14051), "\n" );
        break; case __LINE__: Trice8B( iD(14052), " %02x", b8,  sizeof(b8) /sizeof(int8_t) );    //exp: time:       5_654default:  00 ff fe 33 04 05 06 07 08 09 0a 0b 00 ff fe 33 04 05 06 07 08 09 0a 0b
        break; case __LINE__: trice( iD(14053), "\n" );
        break; case __LINE__: trice8B( iD(14054), " %02x", b8,  sizeof(b8) /sizeof(int8_t) );    //exp: time:            default:  00 ff fe 33 04 05 06 07 08 09 0a 0b 00 ff fe 33 04 05 06 07 08 09 0a 0b
        break; case __LINE__: trice( iD(14055), "\n" );

        break; case __LINE__: trice8B( iD(14056), "att: %02x\n", b8,  sizeof(b8) /sizeof(int8_t) );  //exp: time:            default: att: 00 ff fe 33 04 05 06 07 08 09 0a 0b 00 ff fe 33 04 05 06 07 08 09 0a 0b
        break; case __LINE__: trice8B( iD(14057), "rd: %02x", b8,  sizeof(b8) /sizeof(int8_t) );    //exp: time:            default: rd: 00 ff fe 33 04 05 06 07 08 09 0a 0b 00 ff fe 33 04 05 06 07 08 09 0a 0b
        break; case __LINE__: trice( iD(14058), "\n" );
        break; case __LINE__: trice8B( iD(14059), " %02x\n", b8,  sizeof(b8) /sizeof(int8_t) );      //exp: time:            default:  00 ff fe 33 04 05 06 07 08 09 0a 0b 00 ff fe 33 04 05 06 07 08 09 0a 0b

        break; case __LINE__: TRICE16_B( ID(14060), "  %04x", b16, sizeof(b16)/sizeof(int16_t) ); //exp: time: 842,150_450default:   0000  ffff  fffe  3344
        break; case __LINE__: trice( iD(14061), "\n" );
        break; case __LINE__: TRice16B( iD(14062), " %04x", b16, sizeof(b16) /sizeof(int16_t) );  //exp: time: 842,150_450default:  0000 ffff fffe 3344
        break; case __LINE__: trice( iD(14063), "\n" );
        break; case __LINE__: Trice16B( iD(14064), " %04x", b16, sizeof(b16) /sizeof(int16_t) );  //exp: time:       5_654default:  0000 ffff fffe 3344
        break; case __LINE__: trice( iD(14065), "\n" );
        break; case __LINE__: trice16B( iD(14066), " %04x", b16, sizeof(b16) /sizeof(int16_t) );  //exp: time:            default:  0000 ffff fffe 3344
        break; case __LINE__: trice( iD(14067), "\n" );
        break; case __LINE__: trice16B( iD(14068), "msg: %04x\n", b16, sizeof(b16) /sizeof(int16_t) );  //exp: time:            default: msg: 0000 ffff fffe 3344

        break; case __LINE__: TRICE32_B( ID(14069), " %08x", b32, sizeof(b32)/sizeof(int32_t) );  //exp: time: 842,150_450default:  00000000 ffffffff fffffffe 33445555
        break; case __LINE__: trice( iD(14070), "\n" );
        break; case __LINE__: TRice32B( iD(14071), " %08x", b32, sizeof(b32) /sizeof(int32_t)  ); //exp: time: 842,150_450default:  00000000 ffffffff fffffffe 33445555
        break; case __LINE__: trice( iD(14072), "\n" );
        break; case __LINE__: Trice32B( iD(14073), " %08x", b32, sizeof(b32) /sizeof(int32_t)  ); //exp: time:       5_654default:  00000000 ffffffff fffffffe 33445555
        break; case __LINE__: trice( iD(14074), "\n" );
        break; case __LINE__: trice32B( iD(14075), " %08x", b32, sizeof(b32) /sizeof(int32_t)  ); //exp: time:            default:  00000000 ffffffff fffffffe 33445555
        break; case __LINE__: trice( iD(14076), "\n" );
        break; case __LINE__: trice32B( iD(14077), "att: %08x\n", b32, sizeof(b32) /sizeof(int32_t)  ); //exp: time:            default: att: 00000000 ffffffff fffffffe 33445555

        break; case __LINE__: TRICE64_B( ID(14078), " %016x", b64, sizeof(b64)/sizeof(int64_t) );  //exp: time: 842,150_450default:  0000000000000000 ffffffffffffffff fffffffffffffffe 3344555566666666
        break; case __LINE__: trice( iD(14079), "\n" );
        break; case __LINE__: TRice64B( iD(14080), " %016x", b64, sizeof(b64) /sizeof(int64_t)  ); //exp: time: 842,150_450default:  0000000000000000 ffffffffffffffff fffffffffffffffe 3344555566666666
        break; case __LINE__: trice( iD(14081), "\n" );
        break; case __LINE__: Trice64B( iD(14082), " %016x", b64, sizeof(b64) /sizeof(int64_t)  ); //exp: time:       5_654default:  0000000000000000 ffffffffffffffff fffffffffffffffe 3344555566666666
        break; case __LINE__: trice( iD(14083), "\n" );
        break; case __LINE__: trice64B( iD(14084), " %016x", b64, sizeof(b64) /sizeof(int64_t)  ); //exp: time:            default:  0000000000000000 ffffffffffffffff fffffffffffffffe 3344555566666666
        break; case __LINE__: trice( iD(14085), "\n" );
        break; case __LINE__: trice64B( iD(14086), "SIG: %016x\n", b64, sizeof(b64) /sizeof(int64_t)  ); //exp: time:            default: SIG: 0000000000000000 ffffffffffffffff fffffffffffffffe 3344555566666666

        break; case __LINE__: TRICE8_F( ID(14087), "info:FunctionNameW", b8,  sizeof(b8) /sizeof(int8_t) );   //exp: time: 842,150_450default: info:FunctionNameW(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)
        break; case __LINE__: TRice8F( iD(14088), "call:FunctionNameW", b8,  sizeof(b8) /sizeof(int8_t) );    //exp: time: 842,150_450default: call:FunctionNameW(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)
        break; case __LINE__: Trice8F( iD(14089), "call:FunctionNameW", b8,  sizeof(b8) /sizeof(int8_t) );    //exp: time:       5_654default: call:FunctionNameW(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)
        break; case __LINE__: trice8F( iD(14090), "call:FunctionNameW", b8,  sizeof(b8) /sizeof(int8_t) );    //exp: time:            default: call:FunctionNameW(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)
        break; case __LINE__: TRICE16_F( ID(14091), "info:FunctionNameX", b16, sizeof(b16)/sizeof(int16_t) );  //exp: time: 842,150_450default: info:FunctionNameX(0000)(ffff)(fffe)(3344) 
        break; case __LINE__: TRice16F( iD(14092), "call:FunctionNameX", b16, sizeof(b16)/sizeof(int16_t) );  //exp: time: 842,150_450default: call:FunctionNameX(0000)(ffff)(fffe)(3344) 
        break; case __LINE__: Trice16F( iD(14093), "call:FunctionNameX", b16, sizeof(b16)/sizeof(int16_t) );  //exp: time:       5_654default: call:FunctionNameX(0000)(ffff)(fffe)(3344) 
        break; case __LINE__: trice16F( iD(14094), "call:FunctionNameX", b16, sizeof(b16)/sizeof(int16_t) );  //exp: time:            default: call:FunctionNameX(0000)(ffff)(fffe)(3344) 
        break; case __LINE__: TRICE32_F( ID(14095), "info:FunctionNameY", b32, sizeof(b32)/sizeof(int32_t) );  //exp: time: 842,150_450default: info:FunctionNameY(00000000)(ffffffff)(fffffffe)(33445555)
        break; case __LINE__: TRice32F( iD(14096), "call:FunctionNameY", b32, sizeof(b32)/sizeof(int32_t) );  //exp: time: 842,150_450default: call:FunctionNameY(00000000)(ffffffff)(fffffffe)(33445555)
        break; case __LINE__: Trice32F( iD(14097), "call:FunctionNameY", b32, sizeof(b32)/sizeof(int32_t) );  //exp: time:       5_654default: call:FunctionNameY(00000000)(ffffffff)(fffffffe)(33445555)
        break; case __LINE__: trice32F( iD(14098), "call:FunctionNameY", b32, sizeof(b32)/sizeof(int32_t) );  //exp: time:            default: call:FunctionNameY(00000000)(ffffffff)(fffffffe)(33445555)
        break; case __LINE__: TRICE64_F( ID(14099), "info:FunctionNameZ", b64, sizeof(b64)/sizeof(int64_t) );  //exp: time: 842,150_450default: info:FunctionNameZ(0000000000000000)(ffffffffffffffff)(fffffffffffffffe)(3344555566666666)
        break; case __LINE__: TRice64F( iD(14100), "call:FunctionNameZ", b64, sizeof(b64)/sizeof(int64_t) );  //exp: time: 842,150_450default: call:FunctionNameZ(0000000000000000)(ffffffffffffffff)(fffffffffffffffe)(3344555566666666)
        break; case __LINE__: Trice64F( iD(14101), "call:FunctionNameZ", b64, sizeof(b64)/sizeof(int64_t) );  //exp: time:       5_654default: call:FunctionNameZ(0000000000000000)(ffffffffffffffff)(fffffffffffffffe)(3344555566666666)
        break; case __LINE__: trice64F( iD(14102), "call:FunctionNameZ", b64, sizeof(b64)/sizeof(int64_t) );  //exp: time:            default: call:FunctionNameZ(0000000000000000)(ffffffffffffffff)(fffffffffffffffe)(3344555566666666)
        break; case __LINE__: TRiceS( iD(14103), "msg:Hi %s!\n", five );                 //exp: time: 842,150_450default: msg:Hi five!
        break; case __LINE__: TriceS( iD(14104), "msg:Hi %s!\n", five );                 //exp: time:       5_654default: msg:Hi five!
        break; case __LINE__: triceS( iD(14105), "msg:Hi %s!\n", five );                 //exp: time:            default: msg:Hi five!
        break; case __LINE__: TRiceN( iD(14106), "msg:Hi %s!\n", five, strlen(five) );   //exp: time: 842,150_450default: msg:Hi five!
        break; case __LINE__: TriceN( iD(14107), "msg:Hi %s!\n", five, strlen(five) );   //exp: time:       5_654default: msg:Hi five!
        break; case __LINE__: triceN( iD(14108), "msg:Hi %s!\n", five, strlen(five) );   //exp: time:            default: msg:Hi five!

        break; case __LINE__: TRICE_S( id(14109), "sig:TRICE_S=%s\n", s );            //exp: time:            default: sig:TRICE_S=AAAAAAAAAAAA
        break; case __LINE__: TRICE_N( id(14110), "sig:TRICE_N=%s\n", s, strlen(s) ); //exp: time:            default: sig:TRICE_N=AAAAAAAAAAAA
        break; case __LINE__: TRICE_S( Id(14111), "sig:TRICE_S=%s\n", s );            //exp: time:       5_654default: sig:TRICE_S=AAAAAAAAAAAA
        break; case __LINE__: TRICE_N( Id(14112), "sig:TRICE_N=%s\n", s, strlen(s) ); //exp: time:       5_654default: sig:TRICE_N=AAAAAAAAAAAA
        break; case __LINE__: TRICE_S( ID(14113), "sig:TRICE_S=%s\n", s );            //exp: time: 842,150_450default: sig:TRICE_S=AAAAAAAAAAAA
        break; case __LINE__: TRICE_N( ID(14114), "sig:TRICE_N=%s\n", s, strlen(s) ); //exp: time: 842,150_450default: sig:TRICE_N=AAAAAAAAAAAA
        break; case __LINE__: triceS( iD(14115), "sig:triceS=%s\n", s );              //exp: time:            default: sig:triceS=AAAAAAAAAAAA
        break; case __LINE__: triceN( iD(14116), "sig:triceN=%s\n", s, strlen(s) );   //exp: time:            default: sig:triceN=AAAAAAAAAAAA
        break; case __LINE__: TriceS( iD(14117), "sig:TriceS=%s\n", s );              //exp: time:       5_654default: sig:TriceS=AAAAAAAAAAAA
        break; case __LINE__: TriceN( iD(14118), "sig:TriceN=%s\n", s, strlen(s) );   //exp: time:       5_654default: sig:TriceN=AAAAAAAAAAAA
        break; case __LINE__: TRiceS( iD(14119), "sig:TRiceS=%s\n", s );              //exp: time: 842,150_450default: sig:TRiceS=AAAAAAAAAAAA
        break; case __LINE__: TRiceN( iD(14120), "sig:TRiceN=%s\n", s, strlen(s) );   //exp: time: 842,150_450default: sig:TRiceN=AAAAAAAAAAAA

        break; case __LINE__: trice16( iD(14121), "att: line %u\n", __LINE__ );

        break; case __LINE__: exampleOfManualJSONencoding(); //exp: time:       5_654default: att:MyStructEvaluationFunction(json:ExA{Apple:-1, Birn:2, Fisch:2.781000}
        break; case __LINE__: TRICE( Id(14122), "MSG:1/11 = %g\n", aFloat( 1.0/11 ) ); //exp: time:       5_654default: MSG:1/11 = 0.09090909
        break; case __LINE__: { //exp: time: 842,150_450default: msg:x = 5.934 = 5.934, 5.934
                              float a = (float)5.934;
                              float b = a + ((a > 0) ? 0.0005f : -0.0005f);
                              int c = (int)b;
                              int d = (int)(b * 1000) % 1000;
                              int e = (int)(1000 * (float)(a - c)); 
                              TRICE( ID(14123), "msg:x = %g = %d.%03d, %d.%03d\n", aFloat(a), c, d, c, e ); 
                              }

        break; case __LINE__: trice16( iD(14124), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRice( iD(14125), "sig:Integer (indent, base, sign) - see https://yourbasic.org/golang/fmt-printf-reference-cheat-sheet/\n" );
        break; case __LINE__: TRice( iD(14126), "rd: 15 	%d 	Base 10\n", 15 );                                        //exp: time: 842,150_450default: rd: 15 	15 	Base 10                                   
        break; case __LINE__: TRice( iD(14127), "rd: +15 	%+d 	Always show sign\n", 15 );                           //exp: time: 842,150_450default: rd: +15 	+15 	Always show sign                          
        break; case __LINE__: TRice( iD(14128), "rd:   15	%4d 	Pad with spaces (width 4, right justified)\n", 15 ); //exp: time: 842,150_450default: rd:   15	  15 	Pad with spaces (width 4, right justified)
        break; case __LINE__: TRice( iD(14129), "rd:    15	%-4d	Pad with spaces (width 4, left justified)\n", 15 );  //exp: time: 842,150_450default: rd:    15	15  	Pad with spaces (width 4, left justified) 
        break; case __LINE__: TRice( iD(14130), "rd: 0015	%04d	Pad with zeroes (width 4)\n", 15 );                  //exp: time: 842,150_450default: rd: 0015	0015	Pad with zeroes (width 4)                 
        break; case __LINE__: TRice( iD(14131), "rd: 1111 	%b 	Base 2\n", 15 );                                         //exp: time: 842,150_450default: rd: 1111 	1111 	Base 2                                    
        break; case __LINE__: TRice( iD(14132), "rd: 17 	%o 	Base 8\n", 15 );                                         //exp: time: 842,150_450default: rd: 17 	17 	Base 8                                    
        break; case __LINE__: TRice( iD(14133), "rd: f 	%x 	Base 16, lowercase\n", 15 );                                 //exp: time: 842,150_450default: rd: f 	f 	Base 16, lowercase                        
        break; case __LINE__: TRice( iD(14134), "rd: F 	%X 	Base 16, uppercase\n", 15 );                                 //exp: time: 842,150_450default: rd: F 	F 	Base 16, uppercase                        
        break; case __LINE__: TRice( iD(14135), "rd: 0xf 	%#x 	Base 16, with leading 0x\n", 15 );                   //exp: time: 842,150_450default: rd: 0xf 	0xf 	Base 16, with leading 0x                  

        break; case __LINE__: trice16( iD(14136), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRICE( ID(14137), "sig:Character (quoted, Unicode)\n" );
        break; case __LINE__: TRICE( ID(14138), "rd: A 	%c 	Character\n", 'A' );                                     //exp: time: 842,150_450default: rd: A 	A 	Character
        break; case __LINE__: TRICE( ID(14139), "rd: 'A' 	%q 	Quoted character\n", 'A' );                          //exp: time: 842,150_450default: rd: 'A' 	'A' 	Quoted character
    //  break; case __LINE__: TRICE( ID(14140), "rd: U+0041 	%U 	Unicode\n", 'A' );                        // todo: detect %U to avoid message triceType = TRICE0 ParamSpace = 4 not matching with bitWidth  0 and paramCount 0 - ignoring package
    //  break; case __LINE__: TRICE( ID(14141), "rd: U+0041 'A' 	%#U 	Unicode with character\n", 'A' ); // todo: detect %#U to avoid message triceType = TRICE0 ParamSpace = 4 not matching with bitWidth  0 and paramCount 0 - ignoring package

        break; case __LINE__: TRICE( ID(14142), "sig:Boolean (true/false)\n" );
        break; case __LINE__: TRICE( ID(14143), "rd:Use %%t to format a boolean as true (%t) or false (%t).\n", 1, 0 ); //exp: time: 842,150_450default: rd:Use %t to format a boolean as true (true) or false (false).

        break; case __LINE__: TRICE( ID(14144), "sig:Pointer (hex)\n" );
        break; case __LINE__: TRICE8 ( ID(14145), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x84 );               //exp: time: 842,150_450default: rd:Use %p to format a pointer in base 16 notation with leading 0x. (84)              
        break; case __LINE__: TRICE16( ID(14146), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x1234 );             //exp: time: 842,150_450default: rd:Use %p to format a pointer in base 16 notation with leading 0x. (1234)            
        break; case __LINE__: TRICE32( ID(14147), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0xaabbccdd );         //exp: time: 842,150_450default: rd:Use %p to format a pointer in base 16 notation with leading 0x. (aabbccdd)        
        break; case __LINE__: TRICE64( ID(14148), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x1122334455667788 ); //exp: time: 842,150_450default: rd:Use %p to format a pointer in base 16 notation with leading 0x. (1122334455667788)
        break; case __LINE__: TRICE  ( ID(14149), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0xaabbccdd);          //exp: time: 842,150_450default: rd:Use %p to format a pointer in base 16 notation with leading 0x. (aabbccdd)        

        break; case __LINE__: trice16( iD(14150), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRICE( ID(14151), "sig:Float (indent, precision, scientific notation)\n" );
        break; case __LINE__: TRICE( ID(14152), "rd: 1.234560e+02		%e 	%%e Scientific notation\n", aFloat(123.456) );                       //exp: time: 842,150_450default: rd: 1.234560e+02		1.234560e+02 	%e Scientific notation
        break; case __LINE__: TRICE( ID(14153), "rd: 123.456000		%f 	%%f Decimal point, no exponent\n", aFloat(123.456) );                    //exp: time: 842,150_450default: rd: 123.456000		123.456001 	%f Decimal point, no exponent
        break; case __LINE__: TRICE( ID(14154), "rd: 123.46			%.2f		%%.2f Default width, precision 2\n", aFloat(123.456) );          //exp: time: 842,150_450default: rd: 123.46			123.46		%.2f Default width, precision 2
        break; case __LINE__: TRICE( ID(14155), "rd: ␣␣123.46		%8.2f	%%8.2f Width 8, precision 2\n", aFloat(123.456) );                    //exp: time: 842,150_450default: rd: ␣␣123.46		  123.46	%8.2f Width 8, precision 2
        break; case __LINE__: TRICE( ID(14156), "rd: 123.456		%g		%%g Exponent as needed, necessary digits only\n", aFloat(123.456) ); //exp: time: 842,150_450default: rd: 123.456		123.456		%g Exponent as needed, necessary digits only

        break; case __LINE__: TRICE( ID(14157), "sig:Double (indent, precision, scientific notation)\n" );
        break; case __LINE__: TRICE64( ID(14158), "rd: 1.234560e+02		%e 	%%e Scientific notation\n", aDouble(123.456) );                       //exp: time: 842,150_450default: rd: 1.234560e+02		1.234560e+02 	%e Scientific notation
        break; case __LINE__: TRICE64( ID(14159), "rd: 123.456000		%f 	%%f Decimal point, no exponent\n", aDouble(123.456) );                //exp: time: 842,150_450default: rd: 123.456000		123.456000 	%f Decimal point, no exponent
        break; case __LINE__: TRICE64( ID(14160), "rd: 123.46			%.2f    	%%.2f Default width, precision 2\n", aDouble(123.456) );      //exp: time: 842,150_450default: rd: 123.46			123.46    	%.2f Default width, precision 2
        break; case __LINE__: TRICE64( ID(14161), "rd: ␣␣123.46		%8.2f 	%%8.2f Width 8, precision 2\n", aDouble(123.456) );                    //exp: time: 842,150_450default: rd: ␣␣123.46		  123.46 	%8.2f Width 8, precision 2
        break; case __LINE__: TRICE64( ID(14162), "rd: 123.456		%g		%%g Exponent as needed, necessary digits only\n", aDouble(123.456) ); //exp: time: 842,150_450default: rd: 123.456		123.456		%g Exponent as needed, necessary digits only

        break; case __LINE__: TRICE( ID(14163), "sig:String or byte slice (quote, indent, hex)\n" );
        break; case __LINE__: s = "café"; TRICE_S( ID(14164), "rd: café 			%s		Plain string\n", s );           //exp: time: 842,150_450default: rd: café 			café		Plain string
        break; case __LINE__: s = "café"; TRICE_S( ID(14165), "rd: ␣␣café 		%6s 		Width 6, right justify\n", s );  //exp: time: 842,150_450default: rd: ␣␣café 		  café 		Width 6, right justify
        break; case __LINE__: s = "café"; TRICE_S( ID(14166), "rd: café␣␣ 		%-6s 		Width 6, left justify\n", s );   //exp: time: 842,150_450default: rd: café␣␣ 		café   		Width 6, left justify
        break; case __LINE__: s = "café"; TRICE_S( ID(14167), "rd: \"café\" 		%q		Quoted string\n", s );          //exp: time: 842,150_450default: rd: \"café\" 		"café"		Quoted string
        break; case __LINE__: s = "café"; TRICE_S( ID(14168), "rd: 636166c3a9 		%x	Hex dump of byte values\n", s );    //exp: time: 842,150_450default: rd: 636166c3a9 		636166c3a9	Hex dump of byte values 
        break; case __LINE__: s = "café"; TRICE_S( ID(14169), "rd: 63 61 66 c3 a9 	% x	Hex dump with spaces\n", s );       //exp: time: 842,150_450default: rd: 63 61 66 c3 a9 	63 61 66 c3 a9	Hex dump with spaces

        break; case __LINE__: trice16( iD(14170), "att: line %u\n", __LINE__ );
        break; case __LINE__: exampleOfManualSerialization(); // ATTENTION: This occupies ~1024 bytes in one half buffer when double buffer ist used!

        // CGO compiler issue
        break; case __LINE__: TRice( iD(14171), "info:12 default bit width values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 32-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //exp: time: 842,150_450default: info:12 default bit width values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and a 32-bit stamp.
        break; case __LINE__: Trice( iD(14172), "info:12 default bit width values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 16-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //exp: time:       5_654default: info:12 default bit width values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and a 16-bit stamp.
        break; case __LINE__: trice( iD(14173), "info:12 default bit width values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and without stamp.\n" , -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //exp: time:            default: info:12 default bit width values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and without stamp.

        // todo: improve parsing of //exp string containing special characters
        //break; case __LINE__: TRice( iD(14174), "A TRice message containing a newline\nbefore the line end\n" );               //exp: time: 842,150_450default: A TRice message containing a newline\n                                         before the line end                                                                                                      
        //break; case __LINE__: TRice( iD(14175), "\nA TRice message starting with a a newline\n" );                             //exp: time: 842,150_450default: \n                                         A TRice message starting with a a newline                                                                                                                   
        //break; case __LINE__: TRice( iD(14176), "line0:A TRice message with several lines\nline1\nline2\nline3:last line\n" ); //exp: time: 842,150_450default: line0:A TRice message with several lines\\n                                         line1\n                                         line2\n                                         line3:last line

//          break; case __LINE__: trice16( iD(14177), "att: line %u\n", __LINE__ );
//  
//          break; case __LINE__: 
//      #if TRICE_DEFERRED_XTEA_ENCRYPT
//              trice( iD(14178), "--------------------------------------------------\n" );
//              {
//                  uint32_t by[8] = {1,2,3,4,5,6};
//                  int count = (sizeof(by) + 7) & ~7; // only multiple of 8 encryptable
//                  TRICE32( Id(14179), "msg: message = %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
//                  TRICE32( Id(14180), "tim: pre TriceEncryption SysTick=%d\n", SYSTICKVAL );
//                  XTEAEncrypt(by, count>>2);
//                  TRICE32( Id(14181), "tim: post TriceEncryption SysTick=%d\n", SYSTICKVAL );
//                  TRICE32( Id(14182), "att: TriceEncrypted =  %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
//                  #ifdef XTEA_DECRYPT
//                  TRICE16_1( Id(14183), "tim: pre TriceDecryption SysTick=%d\n", SYSTICKVAL );
//                  XTEADecrypt(by, count>>2);
//                  TRICE16_1( Id(14184), "tim: post TriceDecryption SysTick=%d\n", SYSTICKVAL );
//                  #endif
//                  TRICE32( Id(14185), "msg: messge = %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
//              }
//              trice( iD(14186), "--------------------------------------------------\n" );
//      #endif

        break; case __LINE__: trice( iD(14187), "dbg:Hi!\n" );                       //exp: time:            default: dbg:Hi!    
        break; case __LINE__: trice( iD(14188), "dbg:	Hi!\n" );                    //exp: time:            default: dbg:	Hi!
        break; case __LINE__: trice( iD(14189), "dbg:\t\tHi!\n" );                   //_ex: time:            default: dbg:\t\tHi!
        break; case __LINE__: trice( iD(14190), "dbg:\t\t\tHi!\n" );                 //_ex: time:            default: dbg:\t\t\tHi!
        break; case __LINE__: trice( iD(14191), "dbg:\\tHi!\n" );                    //_ex: time:            default: dbg:\\tHi!
      //break; case __LINE__: trice( iD(14192), "dbg:\aHi!\n" );    // with sound!
      //break; case __LINE__: trice( iD(14193), "dbg:\a\aHi!\n" );  // with sound!
      //break; case __LINE__: trice( iD(14194), "dbg:\a\a\aHi!\n" );// with sound!
      //break; case __LINE__: trice( iD(14195), "dbg:\\aHi!\n" );   // with sound!
      //break; case __LINE__: trice( iD(14196), "err:An ERROR messgage with sound!\a\n" );

        break; case __LINE__: trice16( iD(14197), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRICE( ID(14198), "sig:Some time measurements\n" );
        break; case __LINE__: TRICE( ID(14199), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID(14200), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID(14201), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID(14202), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
        break; case __LINE__: TRICE64( ID(14203), "rd:TRICE64 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE( ID(14204), "tim:TRICE START time message\n");
        break; case __LINE__: TRICE( ID(14205), "tim:TRICE STOP time message\n");
        break; case __LINE__: TRICE( ID(14206), "tim:TRICE START time message\n");
        break; case __LINE__: TRICE( ID(14207), "tim:TRICE STOP time message\n");
        break; case __LINE__: TRICE( ID(14208), "tim:TRICE START time message\n");
        break; case __LINE__: TRICE64( ID(14209), "rd:TRICE64 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE( ID(14210), "tim:TRICE STOP time message\n");
        break; case __LINE__: TRICE( ID(14211), "tim:TRICE --------------------------------------------------\n");
        break; case __LINE__: TRICE( ID(14212), "tim:TRICE --------------------------------------------------\n");
        break; case __LINE__: TRICE64( ID(14213), "rd:TRICE64 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE( ID(14214), "tim:TRICE --------------------------------------------------\n");
        break; case __LINE__: TRICE( ID(14215), "tim:TRICE --------------------------------------------------\n");
        break; case __LINE__: TRICE64( ID(14216), "rd:TRICE64 %d, %d\n", 1, 2 );
        break; case __LINE__: TRICE( ID(14217), "rd:TRICE32_1 line %d\n", __LINE__ );
        break; case __LINE__: TRICE64_1( ID(14218), "rd:TRICE64_1 %d\n", __LINE__ );
        break; case __LINE__: TRICE32_2( ID(14219), "rd:TRICE32_2 line %d,%d\n", __LINE__, 2 );
        break; case __LINE__: TRICE64_2( ID(14220), "rd:TRICE64_2 line %d,%d\n", __LINE__, 2 );

        break; case __LINE__: trice16( iD(14221), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRICE( ID(14222), "sig:Runtime generated strings\n" );
        break; case __LINE__: {
                                  s = "AAAAAAAAAAAA";
                                  TRICE32( ID(14223), "dbg:len=%u:", strlen(s) );
                                  TRICE_S( ID(14224), "sig:%s\n", s );
                              }
        break; case __LINE__: {
                s = "\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBCCCCCCCC\
";
                          }
        break; case __LINE__: TRICE32( ID(14225), "dbg:len=%u:", strlen(s) );
        break; case __LINE__: TRICE_S( ID(14226), "sig:%s\n", s );
        break; case __LINE__: TRICE( ID(14227), "sig:Runtime generated strings\n" );
        break; case __LINE__: {
            s = "\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\
DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\
EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\
";
                          }
        break; case __LINE__: TRICE32( ID(14228), "dbg:len=%u: ", strlen(s) );
        break; case __LINE__: TRICE_S( ID(14229), "sig:%s\n", s );
        break; case __LINE__: TRICE( ID(14230), "sig:Runtime generated strings\n" );
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
        break; case __LINE__: TRICE32( ID(14231), "dbg:len=%u: ", strlen(s) );
        break; case __LINE__: TRICE_S( ID(14232), "sig:%s\n", s );
        break; case __LINE__: TRICE( ID(14233), "sig:Runtime generated strings\n" );
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
        break; case __LINE__: TRICE32( ID(14234), "dbg:len=%u: ", strlen(s) );
        break; case __LINE__: TRICE_S( ID(14235), "sig:%s\n", s );


#if TRICE_CGO == 1

        break; case __LINE__: TRICE  ( ID(14236), "rd:%E (%%E)\n",  aFloat(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-5.555556E+08 (%E)
        break; case __LINE__: TRICE  ( ID(14237), "rd:%F (%%F)\n",  aFloat(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-555555584.000000 (%F)
        break; case __LINE__: TRICE  ( ID(14238), "rd:%G (%%G)\n",  aFloat(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-5.555556E+08 (%G)
        break; case __LINE__: TRICE64( ID(14239), "rd:%E (%%E)\n", aDouble(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-5.555556E+08 (%E)
        break; case __LINE__: TRICE64( ID(14240), "rd:%F (%%F)\n", aDouble(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-555555555.555556 (%F)
        break; case __LINE__: TRICE64( ID(14241), "rd:%G (%%G)\n", aDouble(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-5.555555555555556E+08 (%G)
        
        break; case __LINE__: TRICE  ( ID(14242), "rd:%e (%%e)\n",  aFloat(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-5.555556e+08 (%e)
        break; case __LINE__: TRICE  ( ID(14243), "rd:%f (%%f)\n",  aFloat(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-555555584.000000 (%f)
        break; case __LINE__: TRICE  ( ID(14244), "rd:%g (%%g)\n",  aFloat(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-5.555556e+08 (%g)
        break; case __LINE__: TRICE64( ID(14245), "rd:%e (%%e)\n", aDouble(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-5.555556e+08 (%e)
        break; case __LINE__: TRICE64( ID(14246), "rd:%f (%%f)\n", aDouble(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-555555555.555556 (%f)
        break; case __LINE__: TRICE64( ID(14247), "rd:%g (%%g)\n", aDouble(-555555555.5555555555) ); //exp: time: 842,150_450default: rd:-5.555555555555556e+08 (%g)

        break; case __LINE__: TRICE32( ID(14248), "msg:%u (%%u)\n", -1 ); //exp: time: 842,150_450default: msg:4294967295 (%u)
        break; case __LINE__: TRICE32( ID(14249), "msg:%b (%%b)\n", -1 ); //exp: time: 842,150_450default: msg:11111111111111111111111111111111 (%b)
        break; case __LINE__: TRICE32( ID(14250), "msg:%o (%%o)\n", -1 ); //exp: time: 842,150_450default: msg:37777777777 (%o)
        break; case __LINE__: TRICE32( ID(14251), "msg:%O (%%O)\n", -1 ); //exp: time: 842,150_450default: msg:0o37777777777 (%O)
        break; case __LINE__: TRICE32( ID(14252), "msg:%X (%%X)\n", -1 ); //exp: time: 842,150_450default: msg:FFFFFFFF (%X)
        break; case __LINE__: TRICE32( ID(14253), "msg:%x (%%x)\n", -1 ); //exp: time: 842,150_450default: msg:ffffffff (%x)
        break; case __LINE__: TRICE32( ID(14254), "msg:%d (%%d)\n", -1 ); //exp: time: 842,150_450default: msg:-1 (%d)
        break; case __LINE__: TRICE64( ID(14255), "msg:%u (%%u)\n", -1 ); //exp: time: 842,150_450default: msg:18446744073709551615 (%u)
        break; case __LINE__: TRICE64( ID(14256), "msg:%b (%%b)\n", -1 ); //exp: time: 842,150_450default: msg:1111111111111111111111111111111111111111111111111111111111111111 (%b)
        break; case __LINE__: TRICE64( ID(14257), "msg:%o (%%o)\n", -1 ); //exp: time: 842,150_450default: msg:1777777777777777777777 (%o)
        break; case __LINE__: TRICE64( ID(14258), "msg:%O (%%O)\n", -1 ); //exp: time: 842,150_450default: msg:0o1777777777777777777777 (%O)
        break; case __LINE__: TRICE64( ID(14259), "msg:%X (%%X)\n", -1 ); //exp: time: 842,150_450default: msg:FFFFFFFFFFFFFFFF (%X)
        break; case __LINE__: TRICE64( ID(14260), "msg:%x (%%x)\n", -1 ); //exp: time: 842,150_450default: msg:ffffffffffffffff (%x)

    /**/break; case __LINE__: TRice        ( iD(14261), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
    /**/break; case __LINE__: Trice        ( iD(14262), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
    /**/break; case __LINE__: trice        ( iD(14263), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: TRice8       ( iD(14264), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: Trice8       ( iD(14265), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: trice8       ( iD(14266), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: TRice16      ( iD(14267), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: Trice16      ( iD(14268), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: trice16      ( iD(14269), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: TRice32      ( iD(14270), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: Trice32      ( iD(14271), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: trice32      ( iD(14272), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: TRice64      ( iD(14273), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: Trice64      ( iD(14274), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: trice64      ( iD(14275), "value=%d\n", -2  ); //exp: time:            default: value=-2
        
        break; case __LINE__: TRice_1      ( iD(14276), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: Trice_1      ( iD(14277), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: trice_1      ( iD(14278), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: TRice8_1     ( iD(14279), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: Trice8_1     ( iD(14280), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: trice8_1     ( iD(14281), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: TRice16_1    ( iD(14282), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: Trice16_1    ( iD(14283), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: trice16_1    ( iD(14284), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: TRice32_1    ( iD(14285), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: Trice32_1    ( iD(14286), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: trice32_1    ( iD(14287), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: TRice64_1    ( iD(14288), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: Trice64_1    ( iD(14289), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: trice64_1    ( iD(14290), "value=%d\n", -2  ); //exp: time:            default: value=-2
        
        break; case __LINE__: TRice        ( iD(14291), "no value" ); //exp: time: 842,150_450default: no value
        break; case __LINE__: Trice        ( iD(14292), "no value" ); //exp: time:       5_654default: no value
        break; case __LINE__: trice        ( iD(14293), "no value" ); //exp: time:            default: no value
        break; case __LINE__: TRice8       ( iD(14294), "no value" ); //exp: time: 842,150_450default: no value
        break; case __LINE__: Trice8       ( iD(14295), "no value" ); //exp: time:       5_654default: no value
        break; case __LINE__: trice8       ( iD(14296), "no value" ); //exp: time:            default: no value
        break; case __LINE__: TRice16      ( iD(14297), "no value" ); //exp: time: 842,150_450default: no value
        break; case __LINE__: Trice16      ( iD(14298), "no value" ); //exp: time:       5_654default: no value
        break; case __LINE__: trice16      ( iD(14299), "no value" ); //exp: time:            default: no value
        break; case __LINE__: TRice32      ( iD(14300), "no value" ); //exp: time: 842,150_450default: no value
        break; case __LINE__: Trice32      ( iD(14301), "no value" ); //exp: time:       5_654default: no value
        break; case __LINE__: trice32      ( iD(14302), "no value" ); //exp: time:            default: no value
        break; case __LINE__: TRice64      ( iD(14303), "no value" ); //exp: time: 842,150_450default: no value
        break; case __LINE__: Trice64      ( iD(14304), "no value" ); //exp: time:       5_654default: no value
        break; case __LINE__: trice64      ( iD(14305), "no value" ); //exp: time:            default: no value
    
        break; case __LINE__: TRice_0      ( iD(14306), "no value" ); //exp: time: 842,150_450default: no value
        break; case __LINE__: Trice_0      ( iD(14307), "no value" ); //exp: time:       5_654default: no value
        break; case __LINE__: trice_0      ( iD(14308), "no value" ); //exp: time:            default: no value
        break; case __LINE__: TRice8_0     ( iD(14309), "no value" ); //exp: time: 842,150_450default: no value
        break; case __LINE__: Trice8_0     ( iD(14310), "no value" ); //exp: time:       5_654default: no value
        break; case __LINE__: trice8_0     ( iD(14311), "no value" ); //exp: time:            default: no value
        break; case __LINE__: TRice16_0    ( iD(14312), "no value" ); //exp: time: 842,150_450default: no value
        break; case __LINE__: Trice16_0    ( iD(14313), "no value" ); //exp: time:       5_654default: no value
        break; case __LINE__: trice16_0    ( iD(14314), "no value" ); //exp: time:            default: no value
        break; case __LINE__: TRice32_0    ( iD(14315), "no value" ); //exp: time: 842,150_450default: no value
        break; case __LINE__: Trice32_0    ( iD(14316), "no value" ); //exp: time:       5_654default: no value
        break; case __LINE__: trice32_0    ( iD(14317), "no value" ); //exp: time:            default: no value
        break; case __LINE__: TRice64_0    ( iD(14318), "no value" ); //exp: time: 842,150_450default: no value
        break; case __LINE__: Trice64_0    ( iD(14319), "no value" ); //exp: time:       5_654default: no value
        break; case __LINE__: trice64_0    ( iD(14320), "no value" ); //exp: time:            default: no value

        break; case __LINE__: TRice( iD(14321), "info:This is a message with one value %d and a 32-bit stamp.\n", -2  ); //exp: time: 842,150_450default: info:This is a message with one value -2 and a 32-bit stamp.
        break; case __LINE__: Trice( iD(14322), "info:This is a message with one value %d and a 16-bit stamp.\n", -2  ); //exp: time:       5_654default: info:This is a message with one value -2 and a 16-bit stamp.
        break; case __LINE__: trice( iD(14323), "info:This is a message with one value %d and without stamp.\n" , -2  ); //exp: time:            default: info:This is a message with one value -2 and without stamp.

        break; case __LINE__: TRICE( ID(14324), "info:This is a message without values and a 32-bit stamp.\n" );        //exp: time: 842,150_450default: info:This is a message without values and a 32-bit stamp.
        break; case __LINE__: TRICE( Id(14325), "info:This is a message without values and a 16-bit stamp.\n" );        //exp: time:       5_654default: info:This is a message without values and a 16-bit stamp.
        break; case __LINE__: TRICE( id(14326), "info:This is a message without values and without stamp.\n"  );        //exp: time:            default: info:This is a message without values and without stamp.

        break; case __LINE__: TRICE( ID(14327), "info:This is a message with one value %d and a 32-bit stamp.\n", -2  ); //exp: time: 842,150_450default: info:This is a message with one value -2 and a 32-bit stamp.
        break; case __LINE__: TRICE( Id(14328), "info:This is a message with one value %d and a 16-bit stamp.\n", -2  ); //exp: time:       5_654default: info:This is a message with one value -2 and a 16-bit stamp.
        break; case __LINE__: TRICE( id(14329), "info:This is a message with one value %d and without stamp.\n" , -2  ); //exp: time:            default: info:This is a message with one value -2 and without stamp.

        break; case __LINE__: TRICE( ID(14330), "info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 32-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 );   //exp: time: 842,150_450default: info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and a 32-bit stamp.
        break; case __LINE__: TRICE( Id(14331), "info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 16-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 );   //exp: time:       5_654default: info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and a 16-bit stamp.
        break; case __LINE__: TRICE( id(14332), "info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and without stamp.\n" , -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 );   //exp: time:            default: info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and without stamp.

        break; case __LINE__: TRICE32( ID(14333), "info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 32-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //exp: time: 842,150_450default: info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and a 32-bit stamp.
        break; case __LINE__: TRICE32( Id(14334), "info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 16-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //exp: time:       5_654default: info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and a 16-bit stamp.
        break; case __LINE__: TRICE32( id(14335), "info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and without stamp.\n" , -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //exp: time:            default: info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and without stamp.

        break; case __LINE__: TRICE8   ( id(14336), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: TRICE8_1 ( id(14337), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: TRICE8   ( Id(14338), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRICE8_1 ( Id(14339), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRICE8   ( ID(14340), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: TRICE8_1 ( ID(14341), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: trice8   ( iD(14342), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: trice8_1 ( iD(14343), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: Trice8   ( iD(14344), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: Trice8_1 ( iD(14345), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRice8   ( iD(14346), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: TRice8_1 ( iD(14347), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200

        break; case __LINE__: TRICE16  ( id(14348), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: TRICE16_1( id(14349), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: TRICE16  ( Id(14350), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRICE16_1( Id(14351), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRICE16  ( ID(14352), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: TRICE16_1( ID(14353), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: trice16  ( iD(14354), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: trice16_1( iD(14355), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: Trice16  ( iD(14356), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: Trice16_1( iD(14357), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRice16  ( iD(14358), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: TRice16_1( iD(14359), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200

        break; case __LINE__: TRICE32  ( id(14360), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: TRICE32_1( id(14361), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: TRICE32  ( Id(14362), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRICE32_1( Id(14363), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRICE32  ( ID(14364), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: TRICE32_1( ID(14365), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: trice32  ( iD(14366), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: trice32_1( iD(14367), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: Trice32  ( iD(14368), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: Trice32_1( iD(14369), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRice32  ( iD(14370), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: TRice32_1( iD(14371), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200

        break; case __LINE__: TRICE64  ( id(14372), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: TRICE64_1( id(14373), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: TRICE64  ( Id(14374), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRICE64_1( Id(14375), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRICE64  ( ID(14376), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: TRICE64_1( ID(14377), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: trice64  ( iD(14378), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: trice64_1( iD(14379), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: Trice64  ( iD(14380), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: Trice64_1( iD(14381), "msg:value=%u\n", 200 ); //exp: time:       5_654default: msg:value=200
        break; case __LINE__: TRice64  ( iD(14382), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200
        break; case __LINE__: TRice64_1( iD(14383), "msg:value=%u\n", 200 ); //exp: time: 842,150_450default: msg:value=200

        // mixed 16-bit and 32-bit timestamp 

        break; case __LINE__: TRice8( iD(14384), "msg:value=%d\n", -1 );                                                                                            //exp: time: 842,150_450default: msg:value=-1
        break; case __LINE__: Trice8( iD(14385), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: msg:value=-1, -2
        break; case __LINE__: trice8( iD(14386), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: msg:value=-1, -2, -3
        break; case __LINE__: TRice8( iD(14387), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice8( iD(14388), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice8( iD(14389), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice8( iD(14390), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice8( iD(14391), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice8( iD(14392), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice8( iD(14393), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice8( iD(14394), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice8( iD(14395), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice16( iD(14396), "msg:value=%d\n", -1 );                                                                                            //exp: time: 842,150_450default: msg:value=-1
        break; case __LINE__: Trice16( iD(14397), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: msg:value=-1, -2
        break; case __LINE__: trice16( iD(14398), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: msg:value=-1, -2, -3
        break; case __LINE__: TRice16( iD(14399), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice16( iD(14400), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice16( iD(14401), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice16( iD(14402), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice16( iD(14403), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice16( iD(14404), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice16( iD(14405), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice16( iD(14406), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice16( iD(14407), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice32( iD(14408), "msg:value=%d\n", -1 );                                                                                            //exp: time: 842,150_450default: msg:value=-1
        break; case __LINE__: Trice32( iD(14409), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: msg:value=-1, -2
        break; case __LINE__: trice32( iD(14410), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: msg:value=-1, -2, -3
        break; case __LINE__: TRice32( iD(14411), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice32( iD(14412), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice32( iD(14413), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice32( iD(14414), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice32( iD(14415), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice32( iD(14416), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice32( iD(14417), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice32( iD(14418), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice32( iD(14419), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice64( iD(14420), "msg:value=%d\n", -1 );                                                                                            //exp: time: 842,150_450default: msg:value=-1
        break; case __LINE__: Trice64( iD(14421), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: msg:value=-1, -2
        break; case __LINE__: trice64( iD(14422), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: msg:value=-1, -2, -3
        break; case __LINE__: TRice64( iD(14423), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice64( iD(14424), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice64( iD(14425), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice64( iD(14426), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice64( iD(14427), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice64( iD(14428), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice64( iD(14429), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice64( iD(14430), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice64( iD(14431), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice8_1 ( iD(14432), "msg:value=%d\n", -1 );                                                                                            //exp: time: 842,150_450default: msg:value=-1
        break; case __LINE__: Trice8_2 ( iD(14433), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: msg:value=-1, -2
        break; case __LINE__: trice8_3 ( iD(14434), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: msg:value=-1, -2, -3
        break; case __LINE__: TRice8_4 ( iD(14435), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice8_5 ( iD(14436), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice8_6 ( iD(14437), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice8_7 ( iD(14438), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice8_8 ( iD(14439), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice8_9 ( iD(14440), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice8_10( iD(14441), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice8_11( iD(14442), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice8_12( iD(14443), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice16_1 ( iD(14444), "msg:value=%d\n", -1 );                                                                                            //exp: time: 842,150_450default: msg:value=-1
        break; case __LINE__: Trice16_2 ( iD(14445), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: msg:value=-1, -2
        break; case __LINE__: trice16_3 ( iD(14446), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: msg:value=-1, -2, -3
        break; case __LINE__: TRice16_4 ( iD(14447), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice16_5 ( iD(14448), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice16_6 ( iD(14449), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice16_7 ( iD(14450), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice16_8 ( iD(14451), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice16_9 ( iD(14452), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice16_10( iD(14453), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice16_11( iD(14454), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice16_12( iD(14455), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice32_1 ( iD(14456), "msg:value=%d\n", -1 );                                                                                            //exp: time: 842,150_450default: msg:value=-1
        break; case __LINE__: Trice32_2 ( iD(14457), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: msg:value=-1, -2
        break; case __LINE__: trice32_3 ( iD(14458), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: msg:value=-1, -2, -3
        break; case __LINE__: TRice32_4 ( iD(14459), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice32_5 ( iD(14460), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice32_6 ( iD(14461), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice32_7 ( iD(14462), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice32_8 ( iD(14463), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice32_9 ( iD(14464), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice32_10( iD(14465), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice32_11( iD(14466), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice32_12( iD(14467), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice64_1 ( iD(14468), "msg:value=%d\n", -1 );                                                                                            //exp: time: 842,150_450default: msg:value=-1
        break; case __LINE__: Trice64_2 ( iD(14469), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: msg:value=-1, -2
        break; case __LINE__: trice64_3 ( iD(14470), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: msg:value=-1, -2, -3
        break; case __LINE__: TRice64_4 ( iD(14471), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice64_5 ( iD(14472), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice64_6 ( iD(14473), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice64_7 ( iD(14474), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice64_8 ( iD(14475), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice64_9 ( iD(14476), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice64_10( iD(14477), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time: 842,150_450default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice64_11( iD(14478), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice64_12( iD(14479), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        // use cases for 8-bit data
        break; case __LINE__: trice8     ( iD(14480), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: Trice8     ( iD(14481), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRice8     ( iD(14482), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: trice8_1   ( iD(14483), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: Trice8_1   ( iD(14484), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRice8_1   ( iD(14485), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2

        // use cases for 16-bit data
        break; case __LINE__: trice16    ( iD(14486), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: Trice16    ( iD(14487), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRice16    ( iD(14488), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: trice16_1  ( iD(14489), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: Trice16_1  ( iD(14490), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRice16_1  ( iD(14491), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2

        // use cases for 32-bit data
        break; case __LINE__: trice32    ( iD(14492), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: Trice32    ( iD(14493), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRice32    ( iD(14494), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: trice32_1  ( iD(14495), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: Trice32_1  ( iD(14496), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRice32_1  ( iD(14497), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2

        // use cases for 64-bit data
        break; case __LINE__: trice64    ( iD(14498), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: Trice64    ( iD(14499), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRice64    ( iD(14500), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: trice64_1  ( iD(14501), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: Trice64_1  ( iD(14502), "value=%d\n", -2  ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRice64_1  ( iD(14503), "value=%d\n", -2  ); //exp: time: 842,150_450default: value=-2

        // legacy use cases
        break; case __LINE__: TRICE      ( ID(14504), "value=%d\n", -2 ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: TRICE      ( id(14505), "value=%d\n", -2 ); //exp: time:            default: value=-2
        break; case __LINE__: TRICE      ( Id(14506), "value=%d\n", -2 ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRICE      ( ID(14507), "value=%d\n", -2 ); //exp: time: 842,150_450default: value=-2

        // default stamp
        break; case __LINE__: TRICE8     ( ID(14508), "value=%d\n", -2 ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: TRICE8_1   ( ID(14509), "value=%d\n", -2 ); //exp: time: 842,150_450default: value=-2

        // no stamp
        break; case __LINE__: trice8     ( iD(14510), "value=%d\n", -2 ); //exp: time:            default: value=-2
        break; case __LINE__: trice8_1   ( iD(14511), "value=%d\n", -2 ); //exp: time:            default: value=-2
        break; case __LINE__: TRICE8     ( id(14512), "value=%d\n", -2 ); //exp: time:            default: value=-2
        break; case __LINE__: TRICE8_1   ( id(14513), "value=%d\n", -2 ); //exp: time:            default: value=-2

        // 16-bit stamp:
        break; case __LINE__: Trice8     ( iD(14514), "value=%d\n", -2 ); //exp: time:       5_654default: value=-2
        break; case __LINE__: Trice8_1   ( iD(14515), "value=%d\n", -2 ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRICE8     ( Id(14516), "value=%d\n", -2 ); //exp: time:       5_654default: value=-2
        break; case __LINE__: TRICE8_1   ( Id(14517), "value=%d\n", -2 ); //exp: time:       5_654default: value=-2

        // 32-bit stamp:
        break; case __LINE__: TRice8     ( iD(14518), "value=%d\n", -2 ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: TRice8_1   ( iD(14519), "value=%d\n", -2 ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: TRICE8     ( ID(14520), "value=%d\n", -2 ); //exp: time: 842,150_450default: value=-2
        break; case __LINE__: TRICE8_1   ( ID(14521), "value=%d\n", -2 ); //exp: time: 842,150_450default: value=-2

        // channels
        break; case __LINE__: TRICE( id(14522), "FATAL:magenta+b:red\n" );           //exp: time:            default: FATAL:magenta+b:red
        break; case __LINE__: TRICE( id(14523), "CRITICAL:red+i:default+h\n" );      //exp: time:            default: CRITICAL:red+i:default+h
        break; case __LINE__: TRICE( id(14524), "EMERGENCY:red+i:blue\n" );          //exp: time:            default: EMERGENCY:red+i:blue
        break; case __LINE__: TRICE( id(14525), "ERROR:11:red\n" );                  //exp: time:            default: ERROR:11:red
        break; case __LINE__: TRICE( id(14526), "WARNING:11+i:red\n" );              //exp: time:            default: WARNING:11+i:red
        break; case __LINE__: TRICE( id(14527), "ATTENTION:11:green\n" );            //exp: time:            default: ATTENTION:11:green
        break; case __LINE__: TRICE( id(14528), "INFO:cyan+b:default+h\n" );         //exp: time:            default: INFO:cyan+b:default+h
        break; case __LINE__: TRICE( id(14529), "DEBUG:130+i\n" );                   //exp: time:            default: DEBUG:130+i
        break; case __LINE__: TRICE( id(14530), "TRACE:default+i:default+h\n" );     //exp: time:            default: TRACE:default+i:default+h
        break; case __LINE__: TRICE( id(14531), "TIME:blue+i:blue+h\n" );            //exp: time:            default: TIME:blue+i:blue+h
        break; case __LINE__: TRICE( id(14532), "MESSAGE:green+h:black\n" );         //exp: time:            default: MESSAGE:green+h:black
        break; case __LINE__: TRICE( id(14533), "READ:black+i:yellow+h\n" );         //exp: time:            default: READ:black+i:yellow+h
        break; case __LINE__: TRICE( id(14534), "WRITE:black+u:yellow+h\n" );        //exp: time:            default: WRITE:black+u:yellow+h
        break; case __LINE__: TRICE( id(14535), "RECEIVE:black+h:black\n" );         //exp: time:            default: RECEIVE:black+h:black
        break; case __LINE__: TRICE( id(14536), "TRANSMIT:black:black+h\n" );        //exp: time:            default: TRANSMIT:black:black+h
        break; case __LINE__: TRICE( id(14537), "DIAG:yellow+i:default+h\n" );       //exp: time:            default: DIAG:yellow+i:default+h
        break; case __LINE__: TRICE( id(14538), "INTERRUPT:magenta+i:default+h\n" ); //exp: time:            default: INTERRUPT:magenta+i:default+h
        break; case __LINE__: TRICE( id(14539), "SIGNAL:118+i\n" );                  //exp: time:            default: SIGNAL:118+i
        break; case __LINE__: TRICE( id(14540), "TEST:yellow+h:black\n" );           //exp: time:            default: TEST:yellow+h:black
        break; case __LINE__: TRICE( id(14541), "DEFAULT:off\n" );                   //exp: time:            default: DEFAULT:off
        break; case __LINE__: TRICE( id(14542), "NOTICE:blue:white+h\n" );           //exp: time:            default: NOTICE:blue:white+h
        break; case __LINE__: TRICE( id(14543), "ALERT:magenta:magenta+h\n" );       //exp: time:            default: ALERT:magenta:magenta+h
        break; case __LINE__: TRICE( id(14544), "ASSERT:yellow+i:blue\n" );          //exp: time:            default: ASSERT:yellow+i:blue
        break; case __LINE__: TRICE( id(14545), "ALARM:red+i:white+h\n" );           //exp: time:            default: ALARM:red+i:white+h
        break; case __LINE__: TRICE( id(14546), "CYCLE:blue+i:default+h\n" );        //exp: time:            default: CYCLE:blue+i:default+h
        break; case __LINE__: TRICE( id(14547), "VERBOSE:blue:default\n" );          //exp: time:            default: VERBOSE:blue:default
        break; case __LINE__: TRICE( id(14548), "fatal:magenta+b:red\n" );           //exp: time:            default: fatal:magenta+b:red
        break; case __LINE__: TRICE( id(14549), "critical:red+i:default+h\n" );      //exp: time:            default: critical:red+i:default+h
        break; case __LINE__: TRICE( id(14550), "emergency:red+i:blue\n" );          //exp: time:            default: emergency:red+i:blue
        break; case __LINE__: TRICE( id(14551), "error:11:red\n" );                  //exp: time:            default: error:11:red
        break; case __LINE__: TRICE( id(14552), "warning:11+i:red\n" );              //exp: time:            default: warning:11+i:red
        break; case __LINE__: TRICE( id(14553), "attention:11:green\n" );            //exp: time:            default: attention:11:green
        break; case __LINE__: TRICE( id(14554), "info:cyan+b:default+h\n" );         //exp: time:            default: info:cyan+b:default+h
        break; case __LINE__: TRICE( id(14555), "debug:130+i\n" );                   //exp: time:            default: debug:130+i
        break; case __LINE__: TRICE( id(14556), "trace:default+i:default+h\n" );     //exp: time:            default: trace:default+i:default+h
        break; case __LINE__: TRICE( id(14557), "time:blue+i:blue+h\n" );            //exp: time:            default: time:blue+i:blue+h
        break; case __LINE__: TRICE( id(14558), "message:green+h:black\n" );         //exp: time:            default: message:green+h:black
        break; case __LINE__: TRICE( id(14559), "read:black+i:yellow+h\n" );         //exp: time:            default: read:black+i:yellow+h
        break; case __LINE__: TRICE( id(14560), "write:black+u:yellow+h\n" );        //exp: time:            default: write:black+u:yellow+h
        break; case __LINE__: TRICE( id(14561), "receive:black+h:black\n" );         //exp: time:            default: receive:black+h:black
        break; case __LINE__: TRICE( id(14562), "transmit:black:black+h\n" );        //exp: time:            default: transmit:black:black+h
        break; case __LINE__: TRICE( id(14563), "diag:yellow+i:default+h\n" );       //exp: time:            default: diag:yellow+i:default+h
        break; case __LINE__: TRICE( id(14564), "interrupt:magenta+i:default+h\n" ); //exp: time:            default: interrupt:magenta+i:default+h
        break; case __LINE__: TRICE( id(14565), "signal:118+i\n" );                  //exp: time:            default: signal:118+i
        break; case __LINE__: TRICE( id(14566), "test:yellow+h:black\n" );           //exp: time:            default: test:yellow+h:black
        break; case __LINE__: TRICE( id(14567), "default:off\n" );                   //exp: time:            default: default:off
        break; case __LINE__: TRICE( id(14568), "notice:blue:white+h\n" );           //exp: time:            default: notice:blue:white+h
        break; case __LINE__: TRICE( id(14569), "alert:magenta:magenta+h\n" );       //exp: time:            default: alert:magenta:magenta+h
        break; case __LINE__: TRICE( id(14570), "assert:yellow+i:blue\n" );          //exp: time:            default: assert:yellow+i:blue
        break; case __LINE__: TRICE( id(14571), "alarm:red+i:white+h\n" );           //exp: time:            default: alarm:red+i:white+h
        break; case __LINE__: TRICE( id(14572), "cycle:blue+i:default+h\n" );        //exp: time:            default: cycle:blue+i:default+h
        break; case __LINE__: TRICE( id(14573), "verbose:blue:default\n" );          //exp: time:            default: verbose:blue:default
        
        break; case __LINE__: trice( iD(14574), "FATAL:magenta+b:red\n" );           //exp: time:            default: FATAL:magenta+b:red
        break; case __LINE__: trice( iD(14575), "CRITICAL:red+i:default+h\n" );      //exp: time:            default: CRITICAL:red+i:default+h
        break; case __LINE__: trice( iD(14576), "EMERGENCY:red+i:blue\n" );          //exp: time:            default: EMERGENCY:red+i:blue
        break; case __LINE__: trice( iD(14577), "ERROR:11:red\n" );                  //exp: time:            default: ERROR:11:red
        break; case __LINE__: trice( iD(14578), "WARNING:11+i:red\n" );              //exp: time:            default: WARNING:11+i:red
        break; case __LINE__: trice( iD(14579), "ATTENTION:11:green\n" );            //exp: time:            default: ATTENTION:11:green
        break; case __LINE__: trice( iD(14580), "INFO:cyan+b:default+h\n" );         //exp: time:            default: INFO:cyan+b:default+h
        break; case __LINE__: trice( iD(14581), "DEBUG:130+i\n" );                   //exp: time:            default: DEBUG:130+i
        break; case __LINE__: trice( iD(14582), "TRACE:default+i:default+h\n" );     //exp: time:            default: TRACE:default+i:default+h
        break; case __LINE__: trice( iD(14583), "TIME:blue+i:blue+h\n" );            //exp: time:            default: TIME:blue+i:blue+h
        break; case __LINE__: trice( iD(14584), "MESSAGE:green+h:black\n" );         //exp: time:            default: MESSAGE:green+h:black
        break; case __LINE__: trice( iD(14585), "READ:black+i:yellow+h\n" );         //exp: time:            default: READ:black+i:yellow+h
        break; case __LINE__: trice( iD(14586), "WRITE:black+u:yellow+h\n" );        //exp: time:            default: WRITE:black+u:yellow+h
        break; case __LINE__: trice( iD(14587), "RECEIVE:black+h:black\n" );         //exp: time:            default: RECEIVE:black+h:black
        break; case __LINE__: trice( iD(14588), "TRANSMIT:black:black+h\n" );        //exp: time:            default: TRANSMIT:black:black+h
        break; case __LINE__: trice( iD(14589), "DIAG:yellow+i:default+h\n" );       //exp: time:            default: DIAG:yellow+i:default+h
        break; case __LINE__: trice( iD(14590), "INTERRUPT:magenta+i:default+h\n" ); //exp: time:            default: INTERRUPT:magenta+i:default+h
        break; case __LINE__: trice( iD(14591), "SIGNAL:118+i\n" );                  //exp: time:            default: SIGNAL:118+i
        break; case __LINE__: trice( iD(14592), "TEST:yellow+h:black\n" );           //exp: time:            default: TEST:yellow+h:black
        break; case __LINE__: trice( iD(14593), "DEFAULT:off\n" );                   //exp: time:            default: DEFAULT:off
        break; case __LINE__: trice( iD(14594), "NOTICE:blue:white+h\n" );           //exp: time:            default: NOTICE:blue:white+h
        break; case __LINE__: trice( iD(14595), "ALERT:magenta:magenta+h\n" );       //exp: time:            default: ALERT:magenta:magenta+h
        break; case __LINE__: trice( iD(14596), "ASSERT:yellow+i:blue\n" );          //exp: time:            default: ASSERT:yellow+i:blue
        break; case __LINE__: trice( iD(14597), "ALARM:red+i:white+h\n" );           //exp: time:            default: ALARM:red+i:white+h
        break; case __LINE__: trice( iD(14598), "CYCLE:blue+i:default+h\n" );        //exp: time:            default: CYCLE:blue+i:default+h
        break; case __LINE__: trice( iD(14599), "VERBOSE:blue:default\n" );          //exp: time:            default: VERBOSE:blue:default
        break; case __LINE__: trice( iD(14600), "fatal:magenta+b:red\n" );           //exp: time:            default: fatal:magenta+b:red
        break; case __LINE__: trice( iD(14601), "critical:red+i:default+h\n" );      //exp: time:            default: critical:red+i:default+h
        break; case __LINE__: trice( iD(14602), "emergency:red+i:blue\n" );          //exp: time:            default: emergency:red+i:blue
        break; case __LINE__: trice( iD(14603), "error:11:red\n" );                  //exp: time:            default: error:11:red
        break; case __LINE__: trice( iD(14604), "warning:11+i:red\n" );              //exp: time:            default: warning:11+i:red
        break; case __LINE__: trice( iD(14605), "attention:11:green\n" );            //exp: time:            default: attention:11:green
        break; case __LINE__: trice( iD(14606), "info:cyan+b:default+h\n" );         //exp: time:            default: info:cyan+b:default+h
        break; case __LINE__: trice( iD(14607), "debug:130+i\n" );                   //exp: time:            default: debug:130+i
        break; case __LINE__: trice( iD(14608), "trace:default+i:default+h\n" );     //exp: time:            default: trace:default+i:default+h
        break; case __LINE__: trice( iD(14609), "time:blue+i:blue+h\n" );            //exp: time:            default: time:blue+i:blue+h
        break; case __LINE__: trice( iD(14610), "message:green+h:black\n" );         //exp: time:            default: message:green+h:black
        break; case __LINE__: trice( iD(14611), "read:black+i:yellow+h\n" );         //exp: time:            default: read:black+i:yellow+h
        break; case __LINE__: trice( iD(14612), "write:black+u:yellow+h\n" );        //exp: time:            default: write:black+u:yellow+h
        break; case __LINE__: trice( iD(14613), "receive:black+h:black\n" );         //exp: time:            default: receive:black+h:black
        break; case __LINE__: trice( iD(14614), "transmit:black:black+h\n" );        //exp: time:            default: transmit:black:black+h
        break; case __LINE__: trice( iD(14615), "diag:yellow+i:default+h\n" );       //exp: time:            default: diag:yellow+i:default+h
        break; case __LINE__: trice( iD(14616), "interrupt:magenta+i:default+h\n" ); //exp: time:            default: interrupt:magenta+i:default+h
        break; case __LINE__: trice( iD(14617), "signal:118+i\n" );                  //exp: time:            default: signal:118+i
        break; case __LINE__: trice( iD(14618), "test:yellow+h:black\n" );           //exp: time:            default: test:yellow+h:black
        break; case __LINE__: trice( iD(14619), "default:off\n" );                   //exp: time:            default: default:off
        break; case __LINE__: trice( iD(14620), "notice:blue:white+h\n" );           //exp: time:            default: notice:blue:white+h
        break; case __LINE__: trice( iD(14621), "alert:magenta:magenta+h\n" );       //exp: time:            default: alert:magenta:magenta+h
        break; case __LINE__: trice( iD(14622), "assert:yellow+i:blue\n" );          //exp: time:            default: assert:yellow+i:blue
        break; case __LINE__: trice( iD(14623), "alarm:red+i:white+h\n" );           //exp: time:            default: alarm:red+i:white+h
        break; case __LINE__: trice( iD(14624), "cycle:blue+i:default+h\n" );        //exp: time:            default: cycle:blue+i:default+h
        break; case __LINE__: trice( iD(14625), "verbose:blue:default\n" );          //exp: time:            default: verbose:blue:default

        break; case __LINE__: TRICE( Id(14626), "FATAL:magenta+b:red\n" );           //exp: time:       5_654default: FATAL:magenta+b:red
        break; case __LINE__: TRICE( Id(14627), "CRITICAL:red+i:default+h\n" );      //exp: time:       5_654default: CRITICAL:red+i:default+h
        break; case __LINE__: TRICE( Id(14628), "EMERGENCY:red+i:blue\n" );          //exp: time:       5_654default: EMERGENCY:red+i:blue
        break; case __LINE__: TRICE( Id(14629), "ERROR:11:red\n" );                  //exp: time:       5_654default: ERROR:11:red
        break; case __LINE__: TRICE( Id(14630), "WARNING:11+i:red\n" );              //exp: time:       5_654default: WARNING:11+i:red
        break; case __LINE__: TRICE( Id(14631), "ATTENTION:11:green\n" );            //exp: time:       5_654default: ATTENTION:11:green
        break; case __LINE__: TRICE( Id(14632), "INFO:cyan+b:default+h\n" );         //exp: time:       5_654default: INFO:cyan+b:default+h
        break; case __LINE__: TRICE( Id(14633), "DEBUG:130+i\n" );                   //exp: time:       5_654default: DEBUG:130+i
        break; case __LINE__: TRICE( Id(14634), "TRACE:default+i:default+h\n" );     //exp: time:       5_654default: TRACE:default+i:default+h
        break; case __LINE__: TRICE( Id(14635), "TIME:blue+i:blue+h\n" );            //exp: time:       5_654default: TIME:blue+i:blue+h
        break; case __LINE__: TRICE( Id(14636), "MESSAGE:green+h:black\n" );         //exp: time:       5_654default: MESSAGE:green+h:black
        break; case __LINE__: TRICE( Id(14637), "READ:black+i:yellow+h\n" );         //exp: time:       5_654default: READ:black+i:yellow+h
        break; case __LINE__: TRICE( Id(14638), "WRITE:black+u:yellow+h\n" );        //exp: time:       5_654default: WRITE:black+u:yellow+h
        break; case __LINE__: TRICE( Id(14639), "RECEIVE:black+h:black\n" );         //exp: time:       5_654default: RECEIVE:black+h:black
        break; case __LINE__: TRICE( Id(14640), "TRANSMIT:black:black+h\n" );        //exp: time:       5_654default: TRANSMIT:black:black+h
        break; case __LINE__: TRICE( Id(14641), "DIAG:yellow+i:default+h\n" );       //exp: time:       5_654default: DIAG:yellow+i:default+h
        break; case __LINE__: TRICE( Id(14642), "INTERRUPT:magenta+i:default+h\n" ); //exp: time:       5_654default: INTERRUPT:magenta+i:default+h
        break; case __LINE__: TRICE( Id(14643), "SIGNAL:118+i\n" );                  //exp: time:       5_654default: SIGNAL:118+i
        break; case __LINE__: TRICE( Id(14644), "TEST:yellow+h:black\n" );           //exp: time:       5_654default: TEST:yellow+h:black
        break; case __LINE__: TRICE( Id(14645), "DEFAULT:off\n" );                   //exp: time:       5_654default: DEFAULT:off
        break; case __LINE__: TRICE( Id(14646), "NOTICE:blue:white+h\n" );           //exp: time:       5_654default: NOTICE:blue:white+h
        break; case __LINE__: TRICE( Id(14647), "ALERT:magenta:magenta+h\n" );       //exp: time:       5_654default: ALERT:magenta:magenta+h
        break; case __LINE__: TRICE( Id(14648), "ASSERT:yellow+i:blue\n" );          //exp: time:       5_654default: ASSERT:yellow+i:blue
        break; case __LINE__: TRICE( Id(14649), "ALARM:red+i:white+h\n" );           //exp: time:       5_654default: ALARM:red+i:white+h
        break; case __LINE__: TRICE( Id(14650), "CYCLE:blue+i:default+h\n" );        //exp: time:       5_654default: CYCLE:blue+i:default+h
        break; case __LINE__: TRICE( Id(14651), "VERBOSE:blue:default\n" );          //exp: time:       5_654default: VERBOSE:blue:default
        break; case __LINE__: TRICE( Id(14652), "fatal:magenta+b:red\n" );           //exp: time:       5_654default: fatal:magenta+b:red
        break; case __LINE__: TRICE( Id(14653), "critical:red+i:default+h\n" );      //exp: time:       5_654default: critical:red+i:default+h
        break; case __LINE__: TRICE( Id(14654), "emergency:red+i:blue\n" );          //exp: time:       5_654default: emergency:red+i:blue
        break; case __LINE__: TRICE( Id(14655), "error:11:red\n" );                  //exp: time:       5_654default: error:11:red
        break; case __LINE__: TRICE( Id(14656), "warning:11+i:red\n" );              //exp: time:       5_654default: warning:11+i:red
        break; case __LINE__: TRICE( Id(14657), "attention:11:green\n" );            //exp: time:       5_654default: attention:11:green
        break; case __LINE__: TRICE( Id(14658), "info:cyan+b:default+h\n" );         //exp: time:       5_654default: info:cyan+b:default+h
        break; case __LINE__: TRICE( Id(14659), "debug:130+i\n" );                   //exp: time:       5_654default: debug:130+i
        break; case __LINE__: TRICE( Id(14660), "trace:default+i:default+h\n" );     //exp: time:       5_654default: trace:default+i:default+h
        break; case __LINE__: TRICE( Id(14661), "time:blue+i:blue+h\n" );            //exp: time:       5_654default: time:blue+i:blue+h
        break; case __LINE__: TRICE( Id(14662), "message:green+h:black\n" );         //exp: time:       5_654default: message:green+h:black
        break; case __LINE__: TRICE( Id(14663), "read:black+i:yellow+h\n" );         //exp: time:       5_654default: read:black+i:yellow+h
        break; case __LINE__: TRICE( Id(14664), "write:black+u:yellow+h\n" );        //exp: time:       5_654default: write:black+u:yellow+h
        break; case __LINE__: TRICE( Id(14665), "receive:black+h:black\n" );         //exp: time:       5_654default: receive:black+h:black
        break; case __LINE__: TRICE( Id(14666), "transmit:black:black+h\n" );        //exp: time:       5_654default: transmit:black:black+h
        break; case __LINE__: TRICE( Id(14667), "diag:yellow+i:default+h\n" );       //exp: time:       5_654default: diag:yellow+i:default+h
        break; case __LINE__: TRICE( Id(14668), "interrupt:magenta+i:default+h\n" ); //exp: time:       5_654default: interrupt:magenta+i:default+h
        break; case __LINE__: TRICE( Id(14669), "signal:118+i\n" );                  //exp: time:       5_654default: signal:118+i
        break; case __LINE__: TRICE( Id(14670), "test:yellow+h:black\n" );           //exp: time:       5_654default: test:yellow+h:black
        break; case __LINE__: TRICE( Id(14671), "default:off\n" );                   //exp: time:       5_654default: default:off
        break; case __LINE__: TRICE( Id(14672), "notice:blue:white+h\n" );           //exp: time:       5_654default: notice:blue:white+h
        break; case __LINE__: TRICE( Id(14673), "alert:magenta:magenta+h\n" );       //exp: time:       5_654default: alert:magenta:magenta+h
        break; case __LINE__: TRICE( Id(14674), "assert:yellow+i:blue\n" );          //exp: time:       5_654default: assert:yellow+i:blue
        break; case __LINE__: TRICE( Id(14675), "alarm:red+i:white+h\n" );           //exp: time:       5_654default: alarm:red+i:white+h
        break; case __LINE__: TRICE( Id(14676), "cycle:blue+i:default+h\n" );        //exp: time:       5_654default: cycle:blue+i:default+h
        break; case __LINE__: TRICE( Id(14677), "verbose:blue:default\n" );          //exp: time:       5_654default: verbose:blue:default

        break; case __LINE__: Trice( iD(14678), "FATAL:magenta+b:red\n" );           //exp: time:       5_654default: FATAL:magenta+b:red
        break; case __LINE__: Trice( iD(14679), "CRITICAL:red+i:default+h\n" );      //exp: time:       5_654default: CRITICAL:red+i:default+h
        break; case __LINE__: Trice( iD(14680), "EMERGENCY:red+i:blue\n" );          //exp: time:       5_654default: EMERGENCY:red+i:blue
        break; case __LINE__: Trice( iD(14681), "ERROR:11:red\n" );                  //exp: time:       5_654default: ERROR:11:red
        break; case __LINE__: Trice( iD(14682), "WARNING:11+i:red\n" );              //exp: time:       5_654default: WARNING:11+i:red
        break; case __LINE__: Trice( iD(14683), "ATTENTION:11:green\n" );            //exp: time:       5_654default: ATTENTION:11:green
        break; case __LINE__: Trice( iD(14684), "INFO:cyan+b:default+h\n" );         //exp: time:       5_654default: INFO:cyan+b:default+h
        break; case __LINE__: Trice( iD(14685), "DEBUG:130+i\n" );                   //exp: time:       5_654default: DEBUG:130+i
        break; case __LINE__: Trice( iD(14686), "TRACE:default+i:default+h\n" );     //exp: time:       5_654default: TRACE:default+i:default+h
        break; case __LINE__: Trice( iD(14687), "TIME:blue+i:blue+h\n" );            //exp: time:       5_654default: TIME:blue+i:blue+h
        break; case __LINE__: Trice( iD(14688), "MESSAGE:green+h:black\n" );         //exp: time:       5_654default: MESSAGE:green+h:black
        break; case __LINE__: Trice( iD(14689), "READ:black+i:yellow+h\n" );         //exp: time:       5_654default: READ:black+i:yellow+h
        break; case __LINE__: Trice( iD(14690), "WRITE:black+u:yellow+h\n" );        //exp: time:       5_654default: WRITE:black+u:yellow+h
        break; case __LINE__: Trice( iD(14691), "RECEIVE:black+h:black\n" );         //exp: time:       5_654default: RECEIVE:black+h:black
        break; case __LINE__: Trice( iD(14692), "TRANSMIT:black:black+h\n" );        //exp: time:       5_654default: TRANSMIT:black:black+h
        break; case __LINE__: Trice( iD(14693), "DIAG:yellow+i:default+h\n" );       //exp: time:       5_654default: DIAG:yellow+i:default+h
        break; case __LINE__: Trice( iD(14694), "INTERRUPT:magenta+i:default+h\n" ); //exp: time:       5_654default: INTERRUPT:magenta+i:default+h
        break; case __LINE__: Trice( iD(14695), "SIGNAL:118+i\n" );                  //exp: time:       5_654default: SIGNAL:118+i
        break; case __LINE__: Trice( iD(14696), "TEST:yellow+h:black\n" );           //exp: time:       5_654default: TEST:yellow+h:black
        break; case __LINE__: Trice( iD(14697), "DEFAULT:off\n" );                   //exp: time:       5_654default: DEFAULT:off
        break; case __LINE__: Trice( iD(14698), "NOTICE:blue:white+h\n" );           //exp: time:       5_654default: NOTICE:blue:white+h
        break; case __LINE__: Trice( iD(14699), "ALERT:magenta:magenta+h\n" );       //exp: time:       5_654default: ALERT:magenta:magenta+h
        break; case __LINE__: Trice( iD(14700), "ASSERT:yellow+i:blue\n" );          //exp: time:       5_654default: ASSERT:yellow+i:blue
        break; case __LINE__: Trice( iD(14701), "ALARM:red+i:white+h\n" );           //exp: time:       5_654default: ALARM:red+i:white+h
        break; case __LINE__: Trice( iD(14702), "CYCLE:blue+i:default+h\n" );        //exp: time:       5_654default: CYCLE:blue+i:default+h
        break; case __LINE__: Trice( iD(14703), "VERBOSE:blue:default\n" );          //exp: time:       5_654default: VERBOSE:blue:default
        break; case __LINE__: Trice( iD(14704), "fatal:magenta+b:red\n" );           //exp: time:       5_654default: fatal:magenta+b:red
        break; case __LINE__: Trice( iD(14705), "critical:red+i:default+h\n" );      //exp: time:       5_654default: critical:red+i:default+h
        break; case __LINE__: Trice( iD(14706), "emergency:red+i:blue\n" );          //exp: time:       5_654default: emergency:red+i:blue
        break; case __LINE__: Trice( iD(14707), "error:11:red\n" );                  //exp: time:       5_654default: error:11:red
        break; case __LINE__: Trice( iD(14708), "warning:11+i:red\n" );              //exp: time:       5_654default: warning:11+i:red
        break; case __LINE__: Trice( iD(14709), "attention:11:green\n" );            //exp: time:       5_654default: attention:11:green
        break; case __LINE__: Trice( iD(14710), "info:cyan+b:default+h\n" );         //exp: time:       5_654default: info:cyan+b:default+h
        break; case __LINE__: Trice( iD(14711), "debug:130+i\n" );                   //exp: time:       5_654default: debug:130+i
        break; case __LINE__: Trice( iD(14712), "trace:default+i:default+h\n" );     //exp: time:       5_654default: trace:default+i:default+h
        break; case __LINE__: Trice( iD(14713), "time:blue+i:blue+h\n" );            //exp: time:       5_654default: time:blue+i:blue+h
        break; case __LINE__: Trice( iD(14714), "message:green+h:black\n" );         //exp: time:       5_654default: message:green+h:black
        break; case __LINE__: Trice( iD(14715), "read:black+i:yellow+h\n" );         //exp: time:       5_654default: read:black+i:yellow+h
        break; case __LINE__: Trice( iD(14716), "write:black+u:yellow+h\n" );        //exp: time:       5_654default: write:black+u:yellow+h
        break; case __LINE__: Trice( iD(14717), "receive:black+h:black\n" );         //exp: time:       5_654default: receive:black+h:black
        break; case __LINE__: Trice( iD(14718), "transmit:black:black+h\n" );        //exp: time:       5_654default: transmit:black:black+h
        break; case __LINE__: Trice( iD(14719), "diag:yellow+i:default+h\n" );       //exp: time:       5_654default: diag:yellow+i:default+h
        break; case __LINE__: Trice( iD(14720), "interrupt:magenta+i:default+h\n" ); //exp: time:       5_654default: interrupt:magenta+i:default+h
        break; case __LINE__: Trice( iD(14721), "signal:118+i\n" );                  //exp: time:       5_654default: signal:118+i
        break; case __LINE__: Trice( iD(14722), "test:yellow+h:black\n" );           //exp: time:       5_654default: test:yellow+h:black
        break; case __LINE__: Trice( iD(14723), "default:off\n" );                   //exp: time:       5_654default: default:off
        break; case __LINE__: Trice( iD(14724), "notice:blue:white+h\n" );           //exp: time:       5_654default: notice:blue:white+h
        break; case __LINE__: Trice( iD(14725), "alert:magenta:magenta+h\n" );       //exp: time:       5_654default: alert:magenta:magenta+h
        break; case __LINE__: Trice( iD(14726), "assert:yellow+i:blue\n" );          //exp: time:       5_654default: assert:yellow+i:blue
        break; case __LINE__: Trice( iD(14727), "alarm:red+i:white+h\n" );           //exp: time:       5_654default: alarm:red+i:white+h
        break; case __LINE__: Trice( iD(14728), "cycle:blue+i:default+h\n" );        //exp: time:       5_654default: cycle:blue+i:default+h
        break; case __LINE__: Trice( iD(14729), "verbose:blue:default\n" );          //exp: time:       5_654default: verbose:blue:default

        break; case __LINE__: TRICE( ID(14730), "FATAL:magenta+b:red\n" );           //exp: time: 842,150_450default: FATAL:magenta+b:red
        break; case __LINE__: TRICE( ID(14731), "CRITICAL:red+i:default+h\n" );      //exp: time: 842,150_450default: CRITICAL:red+i:default+h
        break; case __LINE__: TRICE( ID(14732), "EMERGENCY:red+i:blue\n" );          //exp: time: 842,150_450default: EMERGENCY:red+i:blue
        break; case __LINE__: TRICE( ID(14733), "ERROR:11:red\n" );                  //exp: time: 842,150_450default: ERROR:11:red
        break; case __LINE__: TRICE( ID(14734), "WARNING:11+i:red\n" );              //exp: time: 842,150_450default: WARNING:11+i:red
        break; case __LINE__: TRICE( ID(14735), "ATTENTION:11:green\n" );            //exp: time: 842,150_450default: ATTENTION:11:green
        break; case __LINE__: TRICE( ID(14736), "INFO:cyan+b:default+h\n" );         //exp: time: 842,150_450default: INFO:cyan+b:default+h
        break; case __LINE__: TRICE( ID(14737), "DEBUG:130+i\n" );                   //exp: time: 842,150_450default: DEBUG:130+i
        break; case __LINE__: TRICE( ID(14738), "TRACE:default+i:default+h\n" );     //exp: time: 842,150_450default: TRACE:default+i:default+h
        break; case __LINE__: TRICE( ID(14739), "TIME:blue+i:blue+h\n" );            //exp: time: 842,150_450default: TIME:blue+i:blue+h
        break; case __LINE__: TRICE( ID(14740), "MESSAGE:green+h:black\n" );         //exp: time: 842,150_450default: MESSAGE:green+h:black
        break; case __LINE__: TRICE( ID(14741), "READ:black+i:yellow+h\n" );         //exp: time: 842,150_450default: READ:black+i:yellow+h
        break; case __LINE__: TRICE( ID(14742), "WRITE:black+u:yellow+h\n" );        //exp: time: 842,150_450default: WRITE:black+u:yellow+h
        break; case __LINE__: TRICE( ID(14743), "RECEIVE:black+h:black\n" );         //exp: time: 842,150_450default: RECEIVE:black+h:black
        break; case __LINE__: TRICE( ID(14744), "TRANSMIT:black:black+h\n" );        //exp: time: 842,150_450default: TRANSMIT:black:black+h
        break; case __LINE__: TRICE( ID(14745), "DIAG:yellow+i:default+h\n" );       //exp: time: 842,150_450default: DIAG:yellow+i:default+h
        break; case __LINE__: TRICE( ID(14746), "INTERRUPT:magenta+i:default+h\n" ); //exp: time: 842,150_450default: INTERRUPT:magenta+i:default+h
        break; case __LINE__: TRICE( ID(14747), "SIGNAL:118+i\n" );                  //exp: time: 842,150_450default: SIGNAL:118+i
        break; case __LINE__: TRICE( ID(14748), "TEST:yellow+h:black\n" );           //exp: time: 842,150_450default: TEST:yellow+h:black
        break; case __LINE__: TRICE( ID(14749), "DEFAULT:off\n" );                   //exp: time: 842,150_450default: DEFAULT:off
        break; case __LINE__: TRICE( ID(14750), "NOTICE:blue:white+h\n" );           //exp: time: 842,150_450default: NOTICE:blue:white+h
        break; case __LINE__: TRICE( ID(14751), "ALERT:magenta:magenta+h\n" );       //exp: time: 842,150_450default: ALERT:magenta:magenta+h
        break; case __LINE__: TRICE( ID(14752), "ASSERT:yellow+i:blue\n" );          //exp: time: 842,150_450default: ASSERT:yellow+i:blue
        break; case __LINE__: TRICE( ID(14753), "ALARM:red+i:white+h\n" );           //exp: time: 842,150_450default: ALARM:red+i:white+h
        break; case __LINE__: TRICE( ID(14754), "CYCLE:blue+i:default+h\n" );        //exp: time: 842,150_450default: CYCLE:blue+i:default+h
        break; case __LINE__: TRICE( ID(14755), "VERBOSE:blue:default\n" );          //exp: time: 842,150_450default: VERBOSE:blue:default
        break; case __LINE__: TRICE( ID(14756), "fatal:magenta+b:red\n" );           //exp: time: 842,150_450default: fatal:magenta+b:red
        break; case __LINE__: TRICE( ID(14757), "critical:red+i:default+h\n" );      //exp: time: 842,150_450default: critical:red+i:default+h
        break; case __LINE__: TRICE( ID(14758), "emergency:red+i:blue\n" );          //exp: time: 842,150_450default: emergency:red+i:blue
        break; case __LINE__: TRICE( ID(14759), "error:11:red\n" );                  //exp: time: 842,150_450default: error:11:red
        break; case __LINE__: TRICE( ID(14760), "warning:11+i:red\n" );              //exp: time: 842,150_450default: warning:11+i:red
        break; case __LINE__: TRICE( ID(14761), "attention:11:green\n" );            //exp: time: 842,150_450default: attention:11:green
        break; case __LINE__: TRICE( ID(14762), "info:cyan+b:default+h\n" );         //exp: time: 842,150_450default: info:cyan+b:default+h
        break; case __LINE__: TRICE( ID(14763), "debug:130+i\n" );                   //exp: time: 842,150_450default: debug:130+i
        break; case __LINE__: TRICE( ID(14764), "trace:default+i:default+h\n" );     //exp: time: 842,150_450default: trace:default+i:default+h
        break; case __LINE__: TRICE( ID(14765), "time:blue+i:blue+h\n" );            //exp: time: 842,150_450default: time:blue+i:blue+h
        break; case __LINE__: TRICE( ID(14766), "message:green+h:black\n" );         //exp: time: 842,150_450default: message:green+h:black
        break; case __LINE__: TRICE( ID(14767), "read:black+i:yellow+h\n" );         //exp: time: 842,150_450default: read:black+i:yellow+h
        break; case __LINE__: TRICE( ID(14768), "write:black+u:yellow+h\n" );        //exp: time: 842,150_450default: write:black+u:yellow+h
        break; case __LINE__: TRICE( ID(14769), "receive:black+h:black\n" );         //exp: time: 842,150_450default: receive:black+h:black
        break; case __LINE__: TRICE( ID(14770), "transmit:black:black+h\n" );        //exp: time: 842,150_450default: transmit:black:black+h
        break; case __LINE__: TRICE( ID(14771), "diag:yellow+i:default+h\n" );       //exp: time: 842,150_450default: diag:yellow+i:default+h
        break; case __LINE__: TRICE( ID(14772), "interrupt:magenta+i:default+h\n" ); //exp: time: 842,150_450default: interrupt:magenta+i:default+h
        break; case __LINE__: TRICE( ID(14773), "signal:118+i\n" );                  //exp: time: 842,150_450default: signal:118+i
        break; case __LINE__: TRICE( ID(14774), "test:yellow+h:black\n" );           //exp: time: 842,150_450default: test:yellow+h:black
        break; case __LINE__: TRICE( ID(14775), "default:off\n" );                   //exp: time: 842,150_450default: default:off
        break; case __LINE__: TRICE( ID(14776), "notice:blue:white+h\n" );           //exp: time: 842,150_450default: notice:blue:white+h
        break; case __LINE__: TRICE( ID(14777), "alert:magenta:magenta+h\n" );       //exp: time: 842,150_450default: alert:magenta:magenta+h
        break; case __LINE__: TRICE( ID(14778), "assert:yellow+i:blue\n" );          //exp: time: 842,150_450default: assert:yellow+i:blue
        break; case __LINE__: TRICE( ID(14779), "alarm:red+i:white+h\n" );           //exp: time: 842,150_450default: alarm:red+i:white+h
        break; case __LINE__: TRICE( ID(14780), "cycle:blue+i:default+h\n" );        //exp: time: 842,150_450default: cycle:blue+i:default+h
        break; case __LINE__: TRICE( ID(14781), "verbose:blue:default\n" );          //exp: time: 842,150_450default: verbose:blue:default
        
        break; case __LINE__: TRice( iD(14782), "FATAL:magenta+b:red\n" );           //exp: time: 842,150_450default: FATAL:magenta+b:red
        break; case __LINE__: TRice( iD(14783), "CRITICAL:red+i:default+h\n" );      //exp: time: 842,150_450default: CRITICAL:red+i:default+h
        break; case __LINE__: TRice( iD(14784), "EMERGENCY:red+i:blue\n" );          //exp: time: 842,150_450default: EMERGENCY:red+i:blue
        break; case __LINE__: TRice( iD(14785), "ERROR:11:red\n" );                  //exp: time: 842,150_450default: ERROR:11:red
        break; case __LINE__: TRice( iD(14786), "WARNING:11+i:red\n" );              //exp: time: 842,150_450default: WARNING:11+i:red
        break; case __LINE__: TRice( iD(14787), "ATTENTION:11:green\n" );            //exp: time: 842,150_450default: ATTENTION:11:green
        break; case __LINE__: TRice( iD(14788), "INFO:cyan+b:default+h\n" );         //exp: time: 842,150_450default: INFO:cyan+b:default+h
        break; case __LINE__: TRice( iD(14789), "DEBUG:130+i\n" );                   //exp: time: 842,150_450default: DEBUG:130+i
        break; case __LINE__: TRice( iD(14790), "TRACE:default+i:default+h\n" );     //exp: time: 842,150_450default: TRACE:default+i:default+h
        break; case __LINE__: TRice( iD(14791), "TIME:blue+i:blue+h\n" );            //exp: time: 842,150_450default: TIME:blue+i:blue+h
        break; case __LINE__: TRice( iD(14792), "MESSAGE:green+h:black\n" );         //exp: time: 842,150_450default: MESSAGE:green+h:black
        break; case __LINE__: TRice( iD(14793), "READ:black+i:yellow+h\n" );         //exp: time: 842,150_450default: READ:black+i:yellow+h
        break; case __LINE__: TRice( iD(14794), "WRITE:black+u:yellow+h\n" );        //exp: time: 842,150_450default: WRITE:black+u:yellow+h
        break; case __LINE__: TRice( iD(14795), "RECEIVE:black+h:black\n" );         //exp: time: 842,150_450default: RECEIVE:black+h:black
        break; case __LINE__: TRice( iD(14796), "TRANSMIT:black:black+h\n" );        //exp: time: 842,150_450default: TRANSMIT:black:black+h
        break; case __LINE__: TRice( iD(14797), "DIAG:yellow+i:default+h\n" );       //exp: time: 842,150_450default: DIAG:yellow+i:default+h
        break; case __LINE__: TRice( iD(14798), "INTERRUPT:magenta+i:default+h\n" ); //exp: time: 842,150_450default: INTERRUPT:magenta+i:default+h
        break; case __LINE__: TRice( iD(14799), "SIGNAL:118+i\n" );                  //exp: time: 842,150_450default: SIGNAL:118+i
        break; case __LINE__: TRice( iD(14800), "TEST:yellow+h:black\n" );           //exp: time: 842,150_450default: TEST:yellow+h:black
        break; case __LINE__: TRice( iD(14801), "DEFAULT:off\n" );                   //exp: time: 842,150_450default: DEFAULT:off
        break; case __LINE__: TRice( iD(14802), "NOTICE:blue:white+h\n" );           //exp: time: 842,150_450default: NOTICE:blue:white+h
        break; case __LINE__: TRice( iD(14803), "ALERT:magenta:magenta+h\n" );       //exp: time: 842,150_450default: ALERT:magenta:magenta+h
        break; case __LINE__: TRice( iD(14804), "ASSERT:yellow+i:blue\n" );          //exp: time: 842,150_450default: ASSERT:yellow+i:blue
        break; case __LINE__: TRice( iD(14805), "ALARM:red+i:white+h\n" );           //exp: time: 842,150_450default: ALARM:red+i:white+h
        break; case __LINE__: TRice( iD(14806), "CYCLE:blue+i:default+h\n" );        //exp: time: 842,150_450default: CYCLE:blue+i:default+h
        break; case __LINE__: TRice( iD(14807), "VERBOSE:blue:default\n" );          //exp: time: 842,150_450default: VERBOSE:blue:default
        break; case __LINE__: TRice( iD(14808), "fatal:magenta+b:red\n" );           //exp: time: 842,150_450default: fatal:magenta+b:red
        break; case __LINE__: TRice( iD(14809), "critical:red+i:default+h\n" );      //exp: time: 842,150_450default: critical:red+i:default+h
        break; case __LINE__: TRice( iD(14810), "emergency:red+i:blue\n" );          //exp: time: 842,150_450default: emergency:red+i:blue
        break; case __LINE__: TRice( iD(14811), "error:11:red\n" );                  //exp: time: 842,150_450default: error:11:red
        break; case __LINE__: TRice( iD(14812), "warning:11+i:red\n" );              //exp: time: 842,150_450default: warning:11+i:red
        break; case __LINE__: TRice( iD(14813), "attention:11:green\n" );            //exp: time: 842,150_450default: attention:11:green
        break; case __LINE__: TRice( iD(14814), "info:cyan+b:default+h\n" );         //exp: time: 842,150_450default: info:cyan+b:default+h
        break; case __LINE__: TRice( iD(14815), "debug:130+i\n" );                   //exp: time: 842,150_450default: debug:130+i
        break; case __LINE__: TRice( iD(14816), "trace:default+i:default+h\n" );     //exp: time: 842,150_450default: trace:default+i:default+h
        break; case __LINE__: TRice( iD(14817), "time:blue+i:blue+h\n" );            //exp: time: 842,150_450default: time:blue+i:blue+h
        break; case __LINE__: TRice( iD(14818), "message:green+h:black\n" );         //exp: time: 842,150_450default: message:green+h:black
        break; case __LINE__: TRice( iD(14819), "read:black+i:yellow+h\n" );         //exp: time: 842,150_450default: read:black+i:yellow+h
        break; case __LINE__: TRice( iD(14820), "write:black+u:yellow+h\n" );        //exp: time: 842,150_450default: write:black+u:yellow+h
        break; case __LINE__: TRice( iD(14821), "receive:black+h:black\n" );         //exp: time: 842,150_450default: receive:black+h:black
        break; case __LINE__: TRice( iD(14822), "transmit:black:black+h\n" );        //exp: time: 842,150_450default: transmit:black:black+h
        break; case __LINE__: TRice( iD(14823), "diag:yellow+i:default+h\n" );       //exp: time: 842,150_450default: diag:yellow+i:default+h
        break; case __LINE__: TRice( iD(14824), "interrupt:magenta+i:default+h\n" ); //exp: time: 842,150_450default: interrupt:magenta+i:default+h
        break; case __LINE__: TRice( iD(14825), "signal:118+i\n" );                  //exp: time: 842,150_450default: signal:118+i
        break; case __LINE__: TRice( iD(14826), "test:yellow+h:black\n" );           //exp: time: 842,150_450default: test:yellow+h:black
        break; case __LINE__: TRice( iD(14827), "default:off\n" );                   //exp: time: 842,150_450default: default:off
        break; case __LINE__: TRice( iD(14828), "notice:blue:white+h\n" );           //exp: time: 842,150_450default: notice:blue:white+h
        break; case __LINE__: TRice( iD(14829), "alert:magenta:magenta+h\n" );       //exp: time: 842,150_450default: alert:magenta:magenta+h
        break; case __LINE__: TRice( iD(14830), "assert:yellow+i:blue\n" );          //exp: time: 842,150_450default: assert:yellow+i:blue
        break; case __LINE__: TRice( iD(14831), "alarm:red+i:white+h\n" );           //exp: time: 842,150_450default: alarm:red+i:white+h
        break; case __LINE__: TRice( iD(14832), "cycle:blue+i:default+h\n" );        //exp: time: 842,150_450default: cycle:blue+i:default+h
        break; case __LINE__: TRice( iD(14833), "verbose:blue:default\n" );          //exp: time: 842,150_450default: verbose:blue:default

   
   
        break; case __LINE__: TRICE64( id(14834), "msg:%d (%%d)\n", -1 );            //exp: time:            default: msg:-1 (%d)

        break; case __LINE__: trice( iD(14835), "sig:TRICE8 with 1 to 12 values\n" );

        break; case __LINE__: TRICE( id(14836), "rd:TRICE %d\n", -1 );                                                                                            //exp: time:            default: rd:TRICE -1     
        break; case __LINE__: TRICE( id(14837), "rd:TRICE %d, %d\n", -1, -2 );                                                                                    //exp: time:            default: rd:TRICE -1, -2 
        break; case __LINE__: TRICE( id(14838), "rd:TRICE %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: rd:TRICE -1, -2, -3       
        break; case __LINE__: TRICE( id(14839), "rd:TRICE %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:            default: rd:TRICE -1, -2, -3, -4      
        break; case __LINE__: TRICE( id(14840), "rd:TRICE %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:            default: rd:TRICE -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE( id(14841), "rd:TRICE %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: rd:TRICE -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE( id(14842), "rd:TRICE %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:            default: rd:TRICE -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE( id(14843), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:            default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE( id(14844), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE( id(14845), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:            default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE( id(14846), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:            default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE( id(14847), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE( Id(14848), "rd:TRICE %d\n", -1 );                                                                                            //exp: time:       5_654default: rd:TRICE -1     
        break; case __LINE__: TRICE( Id(14849), "rd:TRICE %d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: rd:TRICE -1, -2 
        break; case __LINE__: TRICE( Id(14850), "rd:TRICE %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:       5_654default: rd:TRICE -1, -2, -3       
        break; case __LINE__: TRICE( Id(14851), "rd:TRICE %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:       5_654default: rd:TRICE -1, -2, -3, -4      
        break; case __LINE__: TRICE( Id(14852), "rd:TRICE %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: rd:TRICE -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE( Id(14853), "rd:TRICE %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:       5_654default: rd:TRICE -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE( Id(14854), "rd:TRICE %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:       5_654default: rd:TRICE -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE( Id(14855), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE( Id(14856), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:       5_654default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE( Id(14857), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:       5_654default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE( Id(14858), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE( Id(14859), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:       5_654default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE( ID(14860), "rd:TRICE %d\n", -1 );                                                                                            //exp: time: 842,150_450default: rd:TRICE -1     
        break; case __LINE__: TRICE( ID(14861), "rd:TRICE %d, %d\n", -1, -2 );                                                                                    //exp: time: 842,150_450default: rd:TRICE -1, -2 
        break; case __LINE__: TRICE( ID(14862), "rd:TRICE %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time: 842,150_450default: rd:TRICE -1, -2, -3       
        break; case __LINE__: TRICE( ID(14863), "rd:TRICE %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: rd:TRICE -1, -2, -3, -4      
        break; case __LINE__: TRICE( ID(14864), "rd:TRICE %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time: 842,150_450default: rd:TRICE -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE( ID(14865), "rd:TRICE %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time: 842,150_450default: rd:TRICE -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE( ID(14866), "rd:TRICE %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time: 842,150_450default: rd:TRICE -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE( ID(14867), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time: 842,150_450default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE( ID(14868), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time: 842,150_450default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE( ID(14869), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time: 842,150_450default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE( ID(14870), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time: 842,150_450default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE( ID(14871), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time: 842,150_450default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: trice( iD(14872), "sig:TRICE_n with 1 to 12 values\n" );

        break; case __LINE__: TRICE_1 ( id(14873), "rd:TRICE_1  %d\n", -1 );                                                                                            //exp: time:            default: rd:TRICE_1  -1     
        break; case __LINE__: TRICE_2 ( id(14874), "rd:TRICE_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:            default: rd:TRICE_2  -1, -2 
        break; case __LINE__: TRICE_3 ( id(14875), "rd:TRICE_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: rd:TRICE_3  -1, -2, -3       
        break; case __LINE__: TRICE_4 ( id(14876), "rd:TRICE_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:            default: rd:TRICE_4  -1, -2, -3, -4      
        break; case __LINE__: TRICE_5 ( id(14877), "rd:TRICE_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:            default: rd:TRICE_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE_6 ( id(14878), "rd:TRICE_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: rd:TRICE_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE_7 ( id(14879), "rd:TRICE_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:            default: rd:TRICE_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE_8 ( id(14880), "rd:TRICE_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:            default: rd:TRICE_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE_9 ( id(14881), "rd:TRICE_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: rd:TRICE_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE_10( id(14882), "rd:TRICE_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:            default: rd:TRICE_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE_11( id(14883), "rd:TRICE_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:            default: rd:TRICE_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE_12( id(14884), "rd:TRICE_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: rd:TRICE_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE_1 ( Id(14885), "rd:TRICE_1  %d\n", -1 );                                                                                            //exp: time:       5_654default: rd:TRICE_1  -1     
        break; case __LINE__: TRICE_2 ( Id(14886), "rd:TRICE_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: rd:TRICE_2  -1, -2 
        break; case __LINE__: TRICE_3 ( Id(14887), "rd:TRICE_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:       5_654default: rd:TRICE_3  -1, -2, -3       
        break; case __LINE__: TRICE_4 ( Id(14888), "rd:TRICE_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:       5_654default: rd:TRICE_4  -1, -2, -3, -4      
        break; case __LINE__: TRICE_5 ( Id(14889), "rd:TRICE_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: rd:TRICE_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE_6 ( Id(14890), "rd:TRICE_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:       5_654default: rd:TRICE_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE_7 ( Id(14891), "rd:TRICE_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:       5_654default: rd:TRICE_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE_8 ( Id(14892), "rd:TRICE_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: rd:TRICE_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE_9 ( Id(14893), "rd:TRICE_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:       5_654default: rd:TRICE_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE_10( Id(14894), "rd:TRICE_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:       5_654default: rd:TRICE_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE_11( Id(14895), "rd:TRICE_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: rd:TRICE_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE_12( Id(14896), "rd:TRICE_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:       5_654default: rd:TRICE_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE_1 ( ID(14897), "rd:TRICE_1  %d\n", -1 );                                                                                            //exp: time: 842,150_450default: rd:TRICE_1  -1     
        break; case __LINE__: TRICE_2 ( ID(14898), "rd:TRICE_2  %d, %d\n", -1, -2 );                                                                                    //exp: time: 842,150_450default: rd:TRICE_2  -1, -2 
        break; case __LINE__: TRICE_3 ( ID(14899), "rd:TRICE_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time: 842,150_450default: rd:TRICE_3  -1, -2, -3       
        break; case __LINE__: TRICE_4 ( ID(14900), "rd:TRICE_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: rd:TRICE_4  -1, -2, -3, -4      
        break; case __LINE__: TRICE_5 ( ID(14901), "rd:TRICE_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time: 842,150_450default: rd:TRICE_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE_6 ( ID(14902), "rd:TRICE_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time: 842,150_450default: rd:TRICE_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE_7 ( ID(14903), "rd:TRICE_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time: 842,150_450default: rd:TRICE_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE_8 ( ID(14904), "rd:TRICE_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time: 842,150_450default: rd:TRICE_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE_9 ( ID(14905), "rd:TRICE_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time: 842,150_450default: rd:TRICE_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE_10( ID(14906), "rd:TRICE_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time: 842,150_450default: rd:TRICE_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE_11( ID(14907), "rd:TRICE_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time: 842,150_450default: rd:TRICE_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE_12( ID(14908), "rd:TRICE_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time: 842,150_450default: rd:TRICE_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: trice( iD(14909), "sig:trice with 1 to 12 values\n" );
        
        break; case __LINE__: trice( iD(14910), "rd:trice %d\n", -1 );                                                                                            //exp: time:            default: rd:trice -1     
        break; case __LINE__: trice( iD(14911), "rd:trice %d, %d\n", -1, -2 );                                                                                    //exp: time:            default: rd:trice -1, -2 
        break; case __LINE__: trice( iD(14912), "rd:trice %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: rd:trice -1, -2, -3       
        break; case __LINE__: trice( iD(14913), "rd:trice %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:            default: rd:trice -1, -2, -3, -4      
        break; case __LINE__: trice( iD(14914), "rd:trice %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:            default: rd:trice -1, -2, -3, -4, -5        
        break; case __LINE__: trice( iD(14915), "rd:trice %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: rd:trice -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice( iD(14916), "rd:trice %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:            default: rd:trice -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice( iD(14917), "rd:trice %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:            default: rd:trice -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice( iD(14918), "rd:trice %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: rd:trice -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice( iD(14919), "rd:trice %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:            default: rd:trice -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice( iD(14920), "rd:trice %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:            default: rd:trice -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice( iD(14921), "rd:trice %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: rd:trice -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: Trice( iD(14922), "rd:Trice %d\n", -1 );                                                                                            //exp: time:       5_654default: rd:Trice -1     
        break; case __LINE__: Trice( iD(14923), "rd:Trice %d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: rd:Trice -1, -2 
        break; case __LINE__: Trice( iD(14924), "rd:Trice %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:       5_654default: rd:Trice -1, -2, -3       
        break; case __LINE__: Trice( iD(14925), "rd:Trice %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:       5_654default: rd:Trice -1, -2, -3, -4      
        break; case __LINE__: Trice( iD(14926), "rd:Trice %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: rd:Trice -1, -2, -3, -4, -5        
        break; case __LINE__: Trice( iD(14927), "rd:Trice %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:       5_654default: rd:Trice -1, -2, -3, -4, -5, -6        
        break; case __LINE__: Trice( iD(14928), "rd:Trice %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:       5_654default: rd:Trice -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: Trice( iD(14929), "rd:Trice %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: rd:Trice -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: Trice( iD(14930), "rd:Trice %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:       5_654default: rd:Trice -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: Trice( iD(14931), "rd:Trice %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:       5_654default: rd:Trice -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: Trice( iD(14932), "rd:Trice %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: rd:Trice -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: Trice( iD(14933), "rd:Trice %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:       5_654default: rd:Trice -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRice( iD(14934), "rd:TRice %d\n", -1 );                                                                                            //exp: time: 842,150_450default: rd:TRice -1     
        break; case __LINE__: TRice( iD(14935), "rd:TRice %d, %d\n", -1, -2 );                                                                                    //exp: time: 842,150_450default: rd:TRice -1, -2 
        break; case __LINE__: TRice( iD(14936), "rd:TRice %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time: 842,150_450default: rd:TRice -1, -2, -3       
        break; case __LINE__: TRice( iD(14937), "rd:TRice %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: rd:TRice -1, -2, -3, -4      
        break; case __LINE__: TRice( iD(14938), "rd:TRice %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time: 842,150_450default: rd:TRice -1, -2, -3, -4, -5        
        break; case __LINE__: TRice( iD(14939), "rd:TRice %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time: 842,150_450default: rd:TRice -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRice( iD(14940), "rd:TRice %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time: 842,150_450default: rd:TRice -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRice( iD(14941), "rd:TRice %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time: 842,150_450default: rd:TRice -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRice( iD(14942), "rd:TRice %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time: 842,150_450default: rd:TRice -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRice( iD(14943), "rd:TRice %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time: 842,150_450default: rd:TRice -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRice( iD(14944), "rd:TRice %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time: 842,150_450default: rd:TRice -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRice( iD(14945), "rd:TRice %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time: 842,150_450default: rd:TRice -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: TRICE( ID(14946), "sig:trice_n with 1 to 12 values\n" );

        break; case __LINE__: trice_1 ( iD(14947), "rd:trice_1  %d\n", -1 );                                                                                            //exp: time:            default: rd:trice_1  -1     
        break; case __LINE__: trice_2 ( iD(14948), "rd:trice_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:            default: rd:trice_2  -1, -2 
        break; case __LINE__: trice_3 ( iD(14949), "rd:trice_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: rd:trice_3  -1, -2, -3       
        break; case __LINE__: trice_4 ( iD(14950), "rd:trice_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:            default: rd:trice_4  -1, -2, -3, -4      
        break; case __LINE__: trice_5 ( iD(14951), "rd:trice_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:            default: rd:trice_5  -1, -2, -3, -4, -5        
        break; case __LINE__: trice_6 ( iD(14952), "rd:trice_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: rd:trice_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice_7 ( iD(14953), "rd:trice_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:            default: rd:trice_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice_8 ( iD(14954), "rd:trice_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:            default: rd:trice_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice_9 ( iD(14955), "rd:trice_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: rd:trice_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice_10( iD(14956), "rd:trice_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:            default: rd:trice_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice_11( iD(14957), "rd:trice_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:            default: rd:trice_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice_12( iD(14958), "rd:trice_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: rd:trice_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: Trice_1 ( iD(14959), "rd:Trice_1  %d\n", -1 );                                                                                            //exp: time:       5_654default: rd:Trice_1  -1     
        break; case __LINE__: Trice_2 ( iD(14960), "rd:Trice_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: rd:Trice_2  -1, -2 
        break; case __LINE__: Trice_3 ( iD(14961), "rd:Trice_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:       5_654default: rd:Trice_3  -1, -2, -3       
        break; case __LINE__: Trice_4 ( iD(14962), "rd:Trice_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:       5_654default: rd:Trice_4  -1, -2, -3, -4      
        break; case __LINE__: Trice_5 ( iD(14963), "rd:Trice_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: rd:Trice_5  -1, -2, -3, -4, -5        
        break; case __LINE__: Trice_6 ( iD(14964), "rd:Trice_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:       5_654default: rd:Trice_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: Trice_7 ( iD(14965), "rd:Trice_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:       5_654default: rd:Trice_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: Trice_8 ( iD(14966), "rd:Trice_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: rd:Trice_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: Trice_9 ( iD(14967), "rd:Trice_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:       5_654default: rd:Trice_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: Trice_10( iD(14968), "rd:Trice_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:       5_654default: rd:Trice_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: Trice_11( iD(14969), "rd:Trice_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: rd:Trice_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: Trice_12( iD(14970), "rd:Trice_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:       5_654default: rd:Trice_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRice_1 ( iD(14971), "rd:TRice_1  %d\n", -1 );                                                                                            //exp: time: 842,150_450default: rd:TRice_1  -1     
        break; case __LINE__: TRice_2 ( iD(14972), "rd:TRice_2  %d, %d\n", -1, -2 );                                                                                    //exp: time: 842,150_450default: rd:TRice_2  -1, -2 
        break; case __LINE__: TRice_3 ( iD(14973), "rd:TRice_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time: 842,150_450default: rd:TRice_3  -1, -2, -3       
        break; case __LINE__: TRice_4 ( iD(14974), "rd:TRice_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: rd:TRice_4  -1, -2, -3, -4      
        break; case __LINE__: TRice_5 ( iD(14975), "rd:TRice_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time: 842,150_450default: rd:TRice_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRice_6 ( iD(14976), "rd:TRice_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time: 842,150_450default: rd:TRice_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRice_7 ( iD(14977), "rd:TRice_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time: 842,150_450default: rd:TRice_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRice_8 ( iD(14978), "rd:TRice_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time: 842,150_450default: rd:TRice_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRice_9 ( iD(14979), "rd:TRice_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time: 842,150_450default: rd:TRice_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRice_10( iD(14980), "rd:TRice_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time: 842,150_450default: rd:TRice_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRice_11( iD(14981), "rd:TRice_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time: 842,150_450default: rd:TRice_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRice_12( iD(14982), "rd:TRice_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time: 842,150_450default: rd:TRice_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    










        break; case __LINE__: trice( iD(14983), "sig:TRICE8 with 1 to 12 values\n" );

        break; case __LINE__: TRICE8( id(14984), "rd:TRICE8 %d\n", -1 );                                                                                            //exp: time:            default: rd:TRICE8 -1     
        break; case __LINE__: TRICE8( id(14985), "rd:TRICE8 %d, %d\n", -1, -2 );                                                                                    //exp: time:            default: rd:TRICE8 -1, -2 
        break; case __LINE__: TRICE8( id(14986), "rd:TRICE8 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: rd:TRICE8 -1, -2, -3       
        break; case __LINE__: TRICE8( id(14987), "rd:TRICE8 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:            default: rd:TRICE8 -1, -2, -3, -4      
        break; case __LINE__: TRICE8( id(14988), "rd:TRICE8 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:            default: rd:TRICE8 -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE8( id(14989), "rd:TRICE8 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: rd:TRICE8 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE8( id(14990), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:            default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE8( id(14991), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:            default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE8( id(14992), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE8( id(14993), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:            default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE8( id(14994), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:            default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE8( id(14995), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE8( Id(14996), "rd:TRICE8 %d\n", -1 );                                                                                            //exp: time:       5_654default: rd:TRICE8 -1     
        break; case __LINE__: TRICE8( Id(14997), "rd:TRICE8 %d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: rd:TRICE8 -1, -2 
        break; case __LINE__: TRICE8( Id(14998), "rd:TRICE8 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:       5_654default: rd:TRICE8 -1, -2, -3       
        break; case __LINE__: TRICE8( Id(14999), "rd:TRICE8 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:       5_654default: rd:TRICE8 -1, -2, -3, -4      
        break; case __LINE__: TRICE8( Id(15000), "rd:TRICE8 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: rd:TRICE8 -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE8( Id(15001), "rd:TRICE8 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:       5_654default: rd:TRICE8 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE8( Id(15002), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:       5_654default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE8( Id(15003), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE8( Id(15004), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:       5_654default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE8( Id(15005), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:       5_654default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE8( Id(15006), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE8( Id(15007), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:       5_654default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE8( ID(15008), "rd:TRICE8 %d\n", -1 );                                                                                            //exp: time: 842,150_450default: rd:TRICE8 -1     
        break; case __LINE__: TRICE8( ID(15009), "rd:TRICE8 %d, %d\n", -1, -2 );                                                                                    //exp: time: 842,150_450default: rd:TRICE8 -1, -2 
        break; case __LINE__: TRICE8( ID(15010), "rd:TRICE8 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time: 842,150_450default: rd:TRICE8 -1, -2, -3       
        break; case __LINE__: TRICE8( ID(15011), "rd:TRICE8 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: rd:TRICE8 -1, -2, -3, -4      
        break; case __LINE__: TRICE8( ID(15012), "rd:TRICE8 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time: 842,150_450default: rd:TRICE8 -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE8( ID(15013), "rd:TRICE8 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time: 842,150_450default: rd:TRICE8 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE8( ID(15014), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time: 842,150_450default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE8( ID(15015), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time: 842,150_450default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE8( ID(15016), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time: 842,150_450default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE8( ID(15017), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time: 842,150_450default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE8( ID(15018), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time: 842,150_450default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE8( ID(15019), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time: 842,150_450default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: trice( iD(15020), "sig:TRICE8_n with 1 to 12 values\n" );

        break; case __LINE__: TRICE8_1 ( id(15021), "rd:TRICE8_1  %d\n", -1 );                                                                                            //exp: time:            default: rd:TRICE8_1  -1     
        break; case __LINE__: TRICE8_2 ( id(15022), "rd:TRICE8_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:            default: rd:TRICE8_2  -1, -2 
        break; case __LINE__: TRICE8_3 ( id(15023), "rd:TRICE8_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: rd:TRICE8_3  -1, -2, -3       
        break; case __LINE__: TRICE8_4 ( id(15024), "rd:TRICE8_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:            default: rd:TRICE8_4  -1, -2, -3, -4      
        break; case __LINE__: TRICE8_5 ( id(15025), "rd:TRICE8_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:            default: rd:TRICE8_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE8_6 ( id(15026), "rd:TRICE8_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: rd:TRICE8_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE8_7 ( id(15027), "rd:TRICE8_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:            default: rd:TRICE8_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE8_8 ( id(15028), "rd:TRICE8_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:            default: rd:TRICE8_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE8_9 ( id(15029), "rd:TRICE8_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: rd:TRICE8_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE8_10( id(15030), "rd:TRICE8_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:            default: rd:TRICE8_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE8_11( id(15031), "rd:TRICE8_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:            default: rd:TRICE8_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE8_12( id(15032), "rd:TRICE8_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: rd:TRICE8_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE8_1 ( Id(15033), "rd:TRICE8_1  %d\n", -1 );                                                                                            //exp: time:       5_654default: rd:TRICE8_1  -1     
        break; case __LINE__: TRICE8_2 ( Id(15034), "rd:TRICE8_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: rd:TRICE8_2  -1, -2 
        break; case __LINE__: TRICE8_3 ( Id(15035), "rd:TRICE8_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:       5_654default: rd:TRICE8_3  -1, -2, -3       
        break; case __LINE__: TRICE8_4 ( Id(15036), "rd:TRICE8_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:       5_654default: rd:TRICE8_4  -1, -2, -3, -4      
        break; case __LINE__: TRICE8_5 ( Id(15037), "rd:TRICE8_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: rd:TRICE8_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE8_6 ( Id(15038), "rd:TRICE8_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:       5_654default: rd:TRICE8_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE8_7 ( Id(15039), "rd:TRICE8_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:       5_654default: rd:TRICE8_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE8_8 ( Id(15040), "rd:TRICE8_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: rd:TRICE8_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE8_9 ( Id(15041), "rd:TRICE8_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:       5_654default: rd:TRICE8_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE8_10( Id(15042), "rd:TRICE8_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:       5_654default: rd:TRICE8_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE8_11( Id(15043), "rd:TRICE8_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: rd:TRICE8_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE8_12( Id(15044), "rd:TRICE8_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:       5_654default: rd:TRICE8_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE8_1 ( ID(15045), "rd:TRICE8_1  %d\n", -1 );                                                                                            //exp: time: 842,150_450default: rd:TRICE8_1  -1     
        break; case __LINE__: TRICE8_2 ( ID(15046), "rd:TRICE8_2  %d, %d\n", -1, -2 );                                                                                    //exp: time: 842,150_450default: rd:TRICE8_2  -1, -2 
        break; case __LINE__: TRICE8_3 ( ID(15047), "rd:TRICE8_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time: 842,150_450default: rd:TRICE8_3  -1, -2, -3       
        break; case __LINE__: TRICE8_4 ( ID(15048), "rd:TRICE8_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: rd:TRICE8_4  -1, -2, -3, -4      
        break; case __LINE__: TRICE8_5 ( ID(15049), "rd:TRICE8_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time: 842,150_450default: rd:TRICE8_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE8_6 ( ID(15050), "rd:TRICE8_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time: 842,150_450default: rd:TRICE8_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE8_7 ( ID(15051), "rd:TRICE8_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time: 842,150_450default: rd:TRICE8_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE8_8 ( ID(15052), "rd:TRICE8_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time: 842,150_450default: rd:TRICE8_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE8_9 ( ID(15053), "rd:TRICE8_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time: 842,150_450default: rd:TRICE8_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE8_10( ID(15054), "rd:TRICE8_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time: 842,150_450default: rd:TRICE8_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE8_11( ID(15055), "rd:TRICE8_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time: 842,150_450default: rd:TRICE8_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE8_12( ID(15056), "rd:TRICE8_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time: 842,150_450default: rd:TRICE8_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: trice( iD(15057), "sig:trice8 with 1 to 12 values\n" );
        
        break; case __LINE__: trice8( iD(15058), "rd:trice8 %d\n", -1 );                                                                                            //exp: time:            default: rd:trice8 -1     
        break; case __LINE__: trice8( iD(15059), "rd:trice8 %d, %d\n", -1, -2 );                                                                                    //exp: time:            default: rd:trice8 -1, -2 
        break; case __LINE__: trice8( iD(15060), "rd:trice8 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: rd:trice8 -1, -2, -3       
        break; case __LINE__: trice8( iD(15061), "rd:trice8 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:            default: rd:trice8 -1, -2, -3, -4      
        break; case __LINE__: trice8( iD(15062), "rd:trice8 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:            default: rd:trice8 -1, -2, -3, -4, -5        
        break; case __LINE__: trice8( iD(15063), "rd:trice8 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: rd:trice8 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice8( iD(15064), "rd:trice8 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:            default: rd:trice8 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice8( iD(15065), "rd:trice8 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:            default: rd:trice8 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice8( iD(15066), "rd:trice8 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: rd:trice8 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice8( iD(15067), "rd:trice8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:            default: rd:trice8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice8( iD(15068), "rd:trice8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:            default: rd:trice8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice8( iD(15069), "rd:trice8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: rd:trice8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: Trice8( iD(15070), "rd:Trice8 %d\n", -1 );                                                                                            //exp: time:       5_654default: rd:Trice8 -1     
        break; case __LINE__: Trice8( iD(15071), "rd:Trice8 %d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: rd:Trice8 -1, -2 
        break; case __LINE__: Trice8( iD(15072), "rd:Trice8 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:       5_654default: rd:Trice8 -1, -2, -3       
        break; case __LINE__: Trice8( iD(15073), "rd:Trice8 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:       5_654default: rd:Trice8 -1, -2, -3, -4      
        break; case __LINE__: Trice8( iD(15074), "rd:Trice8 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: rd:Trice8 -1, -2, -3, -4, -5        
        break; case __LINE__: Trice8( iD(15075), "rd:Trice8 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:       5_654default: rd:Trice8 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: Trice8( iD(15076), "rd:Trice8 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:       5_654default: rd:Trice8 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: Trice8( iD(15077), "rd:Trice8 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: rd:Trice8 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: Trice8( iD(15078), "rd:Trice8 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:       5_654default: rd:Trice8 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: Trice8( iD(15079), "rd:Trice8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:       5_654default: rd:Trice8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: Trice8( iD(15080), "rd:Trice8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: rd:Trice8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: Trice8( iD(15081), "rd:Trice8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:       5_654default: rd:Trice8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRice8( iD(15082), "rd:TRice8 %d\n", -1 );                                                                                            //exp: time: 842,150_450default: rd:TRice8 -1     
        break; case __LINE__: TRice8( iD(15083), "rd:TRice8 %d, %d\n", -1, -2 );                                                                                    //exp: time: 842,150_450default: rd:TRice8 -1, -2 
        break; case __LINE__: TRice8( iD(15084), "rd:TRice8 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time: 842,150_450default: rd:TRice8 -1, -2, -3       
        break; case __LINE__: TRice8( iD(15085), "rd:TRice8 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: rd:TRice8 -1, -2, -3, -4      
        break; case __LINE__: TRice8( iD(15086), "rd:TRice8 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time: 842,150_450default: rd:TRice8 -1, -2, -3, -4, -5        
        break; case __LINE__: TRice8( iD(15087), "rd:TRice8 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time: 842,150_450default: rd:TRice8 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRice8( iD(15088), "rd:TRice8 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time: 842,150_450default: rd:TRice8 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRice8( iD(15089), "rd:TRice8 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time: 842,150_450default: rd:TRice8 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRice8( iD(15090), "rd:TRice8 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time: 842,150_450default: rd:TRice8 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRice8( iD(15091), "rd:TRice8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time: 842,150_450default: rd:TRice8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRice8( iD(15092), "rd:TRice8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time: 842,150_450default: rd:TRice8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRice8( iD(15093), "rd:TRice8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time: 842,150_450default: rd:TRice8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: TRICE( ID(15094), "sig:trice8_n with 1 to 12 values\n" );

        break; case __LINE__: trice8_1 ( iD(15095), "rd:trice8_1  %d\n", -1 );                                                                                            //exp: time:            default: rd:trice8_1  -1     
        break; case __LINE__: trice8_2 ( iD(15096), "rd:trice8_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:            default: rd:trice8_2  -1, -2 
        break; case __LINE__: trice8_3 ( iD(15097), "rd:trice8_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: rd:trice8_3  -1, -2, -3       
        break; case __LINE__: trice8_4 ( iD(15098), "rd:trice8_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:            default: rd:trice8_4  -1, -2, -3, -4      
        break; case __LINE__: trice8_5 ( iD(15099), "rd:trice8_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:            default: rd:trice8_5  -1, -2, -3, -4, -5        
        break; case __LINE__: trice8_6 ( iD(15100), "rd:trice8_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: rd:trice8_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice8_7 ( iD(15101), "rd:trice8_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:            default: rd:trice8_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice8_8 ( iD(15102), "rd:trice8_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:            default: rd:trice8_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice8_9 ( iD(15103), "rd:trice8_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: rd:trice8_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice8_10( iD(15104), "rd:trice8_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:            default: rd:trice8_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice8_11( iD(15105), "rd:trice8_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:            default: rd:trice8_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice8_12( iD(15106), "rd:trice8_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: rd:trice8_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: Trice8_1 ( iD(15107), "rd:Trice8_1  %d\n", -1 );                                                                                            //exp: time:       5_654default: rd:Trice8_1  -1     
        break; case __LINE__: Trice8_2 ( iD(15108), "rd:Trice8_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: rd:Trice8_2  -1, -2 
        break; case __LINE__: Trice8_3 ( iD(15109), "rd:Trice8_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:       5_654default: rd:Trice8_3  -1, -2, -3       
        break; case __LINE__: Trice8_4 ( iD(15110), "rd:Trice8_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:       5_654default: rd:Trice8_4  -1, -2, -3, -4      
        break; case __LINE__: Trice8_5 ( iD(15111), "rd:Trice8_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: rd:Trice8_5  -1, -2, -3, -4, -5        
        break; case __LINE__: Trice8_6 ( iD(15112), "rd:Trice8_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:       5_654default: rd:Trice8_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: Trice8_7 ( iD(15113), "rd:Trice8_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:       5_654default: rd:Trice8_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: Trice8_8 ( iD(15114), "rd:Trice8_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: rd:Trice8_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: Trice8_9 ( iD(15115), "rd:Trice8_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:       5_654default: rd:Trice8_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: Trice8_10( iD(15116), "rd:Trice8_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:       5_654default: rd:Trice8_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: Trice8_11( iD(15117), "rd:Trice8_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: rd:Trice8_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: Trice8_12( iD(15118), "rd:Trice8_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:       5_654default: rd:Trice8_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRice8_1 ( iD(15119), "rd:TRice8_1  %d\n", -1 );                                                                                            //exp: time: 842,150_450default: rd:TRice8_1  -1     
        break; case __LINE__: TRice8_2 ( iD(15120), "rd:TRice8_2  %d, %d\n", -1, -2 );                                                                                    //exp: time: 842,150_450default: rd:TRice8_2  -1, -2 
        break; case __LINE__: TRice8_3 ( iD(15121), "rd:TRice8_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time: 842,150_450default: rd:TRice8_3  -1, -2, -3       
        break; case __LINE__: TRice8_4 ( iD(15122), "rd:TRice8_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: rd:TRice8_4  -1, -2, -3, -4      
        break; case __LINE__: TRice8_5 ( iD(15123), "rd:TRice8_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time: 842,150_450default: rd:TRice8_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRice8_6 ( iD(15124), "rd:TRice8_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time: 842,150_450default: rd:TRice8_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRice8_7 ( iD(15125), "rd:TRice8_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time: 842,150_450default: rd:TRice8_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRice8_8 ( iD(15126), "rd:TRice8_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time: 842,150_450default: rd:TRice8_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRice8_9 ( iD(15127), "rd:TRice8_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time: 842,150_450default: rd:TRice8_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRice8_10( iD(15128), "rd:TRice8_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time: 842,150_450default: rd:TRice8_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRice8_11( iD(15129), "rd:TRice8_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time: 842,150_450default: rd:TRice8_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRice8_12( iD(15130), "rd:TRice8_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time: 842,150_450default: rd:TRice8_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: trice( iD(15131), "sig:TRICE16 with 1 to 12 values (line %d)\n", __LINE__ );

        break; case __LINE__: TRICE16( id(15132), "rd:TRICE16 %d\n", -1 );                                                                                            //exp: time:            default: rd:TRICE16 -1     
        break; case __LINE__: TRICE16( id(15133), "rd:TRICE16 %d, %d\n", -1, -2 );                                                                                    //exp: time:            default: rd:TRICE16 -1, -2 
        break; case __LINE__: TRICE16( id(15134), "rd:TRICE16 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: rd:TRICE16 -1, -2, -3       
        break; case __LINE__: TRICE16( id(15135), "rd:TRICE16 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:            default: rd:TRICE16 -1, -2, -3, -4      
        break; case __LINE__: TRICE16( id(15136), "rd:TRICE16 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:            default: rd:TRICE16 -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE16( id(15137), "rd:TRICE16 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: rd:TRICE16 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE16( id(15138), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:            default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE16( id(15139), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:            default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE16( id(15140), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE16( id(15141), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:            default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE16( id(15142), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:            default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE16( id(15143), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE16( Id(15144), "rd:TRICE16 %d\n", -1 );                                                                                            //exp: time:       5_654default: rd:TRICE16 -1     
        break; case __LINE__: TRICE16( Id(15145), "rd:TRICE16 %d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: rd:TRICE16 -1, -2 
        break; case __LINE__: TRICE16( Id(15146), "rd:TRICE16 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:       5_654default: rd:TRICE16 -1, -2, -3       
        break; case __LINE__: TRICE16( Id(15147), "rd:TRICE16 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:       5_654default: rd:TRICE16 -1, -2, -3, -4      
        break; case __LINE__: TRICE16( Id(15148), "rd:TRICE16 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: rd:TRICE16 -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE16( Id(15149), "rd:TRICE16 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:       5_654default: rd:TRICE16 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE16( Id(15150), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:       5_654default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE16( Id(15151), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE16( Id(15152), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:       5_654default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE16( Id(15153), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:       5_654default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE16( Id(15154), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE16( Id(15155), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:       5_654default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE16( ID(15156), "rd:TRICE16 %d\n", -1 );                                                                                            //exp: time: 842,150_450default: rd:TRICE16 -1     
        break; case __LINE__: TRICE16( ID(15157), "rd:TRICE16 %d, %d\n", -1, -2 );                                                                                    //exp: time: 842,150_450default: rd:TRICE16 -1, -2 
        break; case __LINE__: TRICE16( ID(15158), "rd:TRICE16 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time: 842,150_450default: rd:TRICE16 -1, -2, -3       
        break; case __LINE__: TRICE16( ID(15159), "rd:TRICE16 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: rd:TRICE16 -1, -2, -3, -4      
        break; case __LINE__: TRICE16( ID(15160), "rd:TRICE16 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time: 842,150_450default: rd:TRICE16 -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE16( ID(15161), "rd:TRICE16 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time: 842,150_450default: rd:TRICE16 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE16( ID(15162), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time: 842,150_450default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE16( ID(15163), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time: 842,150_450default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE16( ID(15164), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time: 842,150_450default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE16( ID(15165), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time: 842,150_450default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE16( ID(15166), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time: 842,150_450default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE16( ID(15167), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time: 842,150_450default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: trice     ( iD(15168), "sig:TRICE16_n with 1 to 12 values (line %d)\n", __LINE__ );
        
        break; case __LINE__: TRICE16_1 ( id(15169), "rd:TRICE16_1  %d\n", -1 );                                                                                            //exp: time:            default: rd:TRICE16_1  -1     
        break; case __LINE__: TRICE16_2 ( id(15170), "rd:TRICE16_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:            default: rd:TRICE16_2  -1, -2 
        break; case __LINE__: TRICE16_3 ( id(15171), "rd:TRICE16_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: rd:TRICE16_3  -1, -2, -3       
        break; case __LINE__: TRICE16_4 ( id(15172), "rd:TRICE16_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:            default: rd:TRICE16_4  -1, -2, -3, -4      
        break; case __LINE__: TRICE16_5 ( id(15173), "rd:TRICE16_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:            default: rd:TRICE16_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE16_6 ( id(15174), "rd:TRICE16_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: rd:TRICE16_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE16_7 ( id(15175), "rd:TRICE16_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:            default: rd:TRICE16_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE16_8 ( id(15176), "rd:TRICE16_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:            default: rd:TRICE16_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE16_9 ( id(15177), "rd:TRICE16_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: rd:TRICE16_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE16_10( id(15178), "rd:TRICE16_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:            default: rd:TRICE16_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE16_11( id(15179), "rd:TRICE16_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:            default: rd:TRICE16_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE16_12( id(15180), "rd:TRICE16_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: rd:TRICE16_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE16_1 ( Id(15181), "rd:TRICE16_1  %d\n", -1 );                                                                                            //exp: time:       5_654default: rd:TRICE16_1  -1     
        break; case __LINE__: TRICE16_2 ( Id(15182), "rd:TRICE16_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: rd:TRICE16_2  -1, -2 
        break; case __LINE__: TRICE16_3 ( Id(15183), "rd:TRICE16_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:       5_654default: rd:TRICE16_3  -1, -2, -3       
        break; case __LINE__: TRICE16_4 ( Id(15184), "rd:TRICE16_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:       5_654default: rd:TRICE16_4  -1, -2, -3, -4      
        break; case __LINE__: TRICE16_5 ( Id(15185), "rd:TRICE16_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: rd:TRICE16_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE16_6 ( Id(15186), "rd:TRICE16_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:       5_654default: rd:TRICE16_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE16_7 ( Id(15187), "rd:TRICE16_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:       5_654default: rd:TRICE16_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE16_8 ( Id(15188), "rd:TRICE16_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: rd:TRICE16_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE16_9 ( Id(15189), "rd:TRICE16_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:       5_654default: rd:TRICE16_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE16_10( Id(15190), "rd:TRICE16_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:       5_654default: rd:TRICE16_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE16_11( Id(15191), "rd:TRICE16_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: rd:TRICE16_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE16_12( Id(15192), "rd:TRICE16_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:       5_654default: rd:TRICE16_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE16_1 ( ID(15193), "rd:TRICE16_1  %d\n", -1 );                                                                                            //exp: time: 842,150_450default: rd:TRICE16_1  -1     
        break; case __LINE__: TRICE16_2 ( ID(15194), "rd:TRICE16_2  %d, %d\n", -1, -2 );                                                                                    //exp: time: 842,150_450default: rd:TRICE16_2  -1, -2 
        break; case __LINE__: TRICE16_3 ( ID(15195), "rd:TRICE16_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time: 842,150_450default: rd:TRICE16_3  -1, -2, -3       
        break; case __LINE__: TRICE16_4 ( ID(15196), "rd:TRICE16_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: rd:TRICE16_4  -1, -2, -3, -4      
        break; case __LINE__: TRICE16_5 ( ID(15197), "rd:TRICE16_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time: 842,150_450default: rd:TRICE16_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE16_6 ( ID(15198), "rd:TRICE16_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time: 842,150_450default: rd:TRICE16_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE16_7 ( ID(15199), "rd:TRICE16_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time: 842,150_450default: rd:TRICE16_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE16_8 ( ID(15200), "rd:TRICE16_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time: 842,150_450default: rd:TRICE16_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE16_9 ( ID(15201), "rd:TRICE16_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time: 842,150_450default: rd:TRICE16_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE16_10( ID(15202), "rd:TRICE16_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time: 842,150_450default: rd:TRICE16_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE16_11( ID(15203), "rd:TRICE16_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time: 842,150_450default: rd:TRICE16_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE16_12( ID(15204), "rd:TRICE16_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time: 842,150_450default: rd:TRICE16_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: trice( iD(15205), "sig:trice16 with 1 to 12 values (line %d)\n", __LINE__ );

        break; case __LINE__: trice16( iD(15206), "rd:trice16 %d\n", -1 );                                                                                            //exp: time:            default: rd:trice16 -1     
        break; case __LINE__: trice16( iD(15207), "rd:trice16 %d, %d\n", -1, -2 );                                                                                    //exp: time:            default: rd:trice16 -1, -2 
        break; case __LINE__: trice16( iD(15208), "rd:trice16 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: rd:trice16 -1, -2, -3       
        break; case __LINE__: trice16( iD(15209), "rd:trice16 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:            default: rd:trice16 -1, -2, -3, -4      
        break; case __LINE__: trice16( iD(15210), "rd:trice16 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:            default: rd:trice16 -1, -2, -3, -4, -5        
        break; case __LINE__: trice16( iD(15211), "rd:trice16 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: rd:trice16 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice16( iD(15212), "rd:trice16 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:            default: rd:trice16 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice16( iD(15213), "rd:trice16 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:            default: rd:trice16 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice16( iD(15214), "rd:trice16 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: rd:trice16 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice16( iD(15215), "rd:trice16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:            default: rd:trice16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice16( iD(15216), "rd:trice16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:            default: rd:trice16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice16( iD(15217), "rd:trice16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: rd:trice16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: Trice16( iD(15218), "rd:Trice16 %d\n", -1 );                                                                                            //exp: time:       5_654default: rd:Trice16 -1     
        break; case __LINE__: Trice16( iD(15219), "rd:Trice16 %d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: rd:Trice16 -1, -2 
        break; case __LINE__: Trice16( iD(15220), "rd:Trice16 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:       5_654default: rd:Trice16 -1, -2, -3       
        break; case __LINE__: Trice16( iD(15221), "rd:Trice16 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:       5_654default: rd:Trice16 -1, -2, -3, -4      
        break; case __LINE__: Trice16( iD(15222), "rd:Trice16 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: rd:Trice16 -1, -2, -3, -4, -5        
        break; case __LINE__: Trice16( iD(15223), "rd:Trice16 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:       5_654default: rd:Trice16 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: Trice16( iD(15224), "rd:Trice16 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:       5_654default: rd:Trice16 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: Trice16( iD(15225), "rd:Trice16 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: rd:Trice16 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: Trice16( iD(15226), "rd:Trice16 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:       5_654default: rd:Trice16 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: Trice16( iD(15227), "rd:Trice16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:       5_654default: rd:Trice16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: Trice16( iD(15228), "rd:Trice16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: rd:Trice16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: Trice16( iD(15229), "rd:Trice16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:       5_654default: rd:Trice16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRice16( iD(15230), "rd:TRice16 %d\n", -1 );                                                                                            //exp: time: 842,150_450default: rd:TRice16 -1     
        break; case __LINE__: TRice16( iD(15231), "rd:TRice16 %d, %d\n", -1, -2 );                                                                                    //exp: time: 842,150_450default: rd:TRice16 -1, -2 
        break; case __LINE__: TRice16( iD(15232), "rd:TRice16 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time: 842,150_450default: rd:TRice16 -1, -2, -3       
        break; case __LINE__: TRice16( iD(15233), "rd:TRice16 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: rd:TRice16 -1, -2, -3, -4      
        break; case __LINE__: TRice16( iD(15234), "rd:TRice16 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time: 842,150_450default: rd:TRice16 -1, -2, -3, -4, -5        
        break; case __LINE__: TRice16( iD(15235), "rd:TRice16 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time: 842,150_450default: rd:TRice16 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRice16( iD(15236), "rd:TRice16 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time: 842,150_450default: rd:TRice16 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRice16( iD(15237), "rd:TRice16 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time: 842,150_450default: rd:TRice16 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRice16( iD(15238), "rd:TRice16 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time: 842,150_450default: rd:TRice16 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRice16( iD(15239), "rd:TRice16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time: 842,150_450default: rd:TRice16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRice16( iD(15240), "rd:TRice16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time: 842,150_450default: rd:TRice16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRice16( iD(15241), "rd:TRice16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time: 842,150_450default: rd:TRice16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: trice     ( iD(15242), "sig:trice16_n with 1 to 12 values (line %d)\n", __LINE__ );
        
        break; case __LINE__: trice16_1 ( iD(15243), "rd:trice16_1  %d\n", -1 );                                                                                            //exp: time:            default: rd:trice16_1  -1     
        break; case __LINE__: trice16_2 ( iD(15244), "rd:trice16_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:            default: rd:trice16_2  -1, -2 
        break; case __LINE__: trice16_3 ( iD(15245), "rd:trice16_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: rd:trice16_3  -1, -2, -3       
        break; case __LINE__: trice16_4 ( iD(15246), "rd:trice16_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:            default: rd:trice16_4  -1, -2, -3, -4      
        break; case __LINE__: trice16_5 ( iD(15247), "rd:trice16_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:            default: rd:trice16_5  -1, -2, -3, -4, -5        
        break; case __LINE__: trice16_6 ( iD(15248), "rd:trice16_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: rd:trice16_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice16_7 ( iD(15249), "rd:trice16_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:            default: rd:trice16_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice16_8 ( iD(15250), "rd:trice16_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:            default: rd:trice16_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice16_9 ( iD(15251), "rd:trice16_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: rd:trice16_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice16_10( iD(15252), "rd:trice16_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:            default: rd:trice16_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice16_11( iD(15253), "rd:trice16_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:            default: rd:trice16_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice16_12( iD(15254), "rd:trice16_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: rd:trice16_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: Trice16_1 ( iD(15255), "rd:Trice16_1  %d\n", -1 );                                                                                            //exp: time:       5_654default: rd:Trice16_1  -1     
        break; case __LINE__: Trice16_2 ( iD(15256), "rd:Trice16_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: rd:Trice16_2  -1, -2 
        break; case __LINE__: Trice16_3 ( iD(15257), "rd:Trice16_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:       5_654default: rd:Trice16_3  -1, -2, -3       
        break; case __LINE__: Trice16_4 ( iD(15258), "rd:Trice16_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:       5_654default: rd:Trice16_4  -1, -2, -3, -4      
        break; case __LINE__: Trice16_5 ( iD(15259), "rd:Trice16_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: rd:Trice16_5  -1, -2, -3, -4, -5        
        break; case __LINE__: Trice16_6 ( iD(15260), "rd:Trice16_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:       5_654default: rd:Trice16_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: Trice16_7 ( iD(15261), "rd:Trice16_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:       5_654default: rd:Trice16_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: Trice16_8 ( iD(15262), "rd:Trice16_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: rd:Trice16_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: Trice16_9 ( iD(15263), "rd:Trice16_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:       5_654default: rd:Trice16_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: Trice16_10( iD(15264), "rd:Trice16_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:       5_654default: rd:Trice16_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: Trice16_11( iD(15265), "rd:Trice16_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: rd:Trice16_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: Trice16_12( iD(15266), "rd:Trice16_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:       5_654default: rd:Trice16_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRice16_1 ( iD(15267), "rd:TRice16_1  %d\n", -1 );                                                                                            //exp: time: 842,150_450default: rd:TRice16_1  -1     
        break; case __LINE__: TRice16_2 ( iD(15268), "rd:TRice16_2  %d, %d\n", -1, -2 );                                                                                    //exp: time: 842,150_450default: rd:TRice16_2  -1, -2 
        break; case __LINE__: TRice16_3 ( iD(15269), "rd:TRice16_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time: 842,150_450default: rd:TRice16_3  -1, -2, -3       
        break; case __LINE__: TRice16_4 ( iD(15270), "rd:TRice16_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: rd:TRice16_4  -1, -2, -3, -4      
        break; case __LINE__: TRice16_5 ( iD(15271), "rd:TRice16_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time: 842,150_450default: rd:TRice16_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRice16_6 ( iD(15272), "rd:TRice16_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time: 842,150_450default: rd:TRice16_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRice16_7 ( iD(15273), "rd:TRice16_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time: 842,150_450default: rd:TRice16_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRice16_8 ( iD(15274), "rd:TRice16_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time: 842,150_450default: rd:TRice16_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRice16_9 ( iD(15275), "rd:TRice16_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time: 842,150_450default: rd:TRice16_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRice16_10( iD(15276), "rd:TRice16_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time: 842,150_450default: rd:TRice16_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRice16_11( iD(15277), "rd:TRice16_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time: 842,150_450default: rd:TRice16_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRice16_12( iD(15278), "rd:TRice16_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time: 842,150_450default: rd:TRice16_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: trice  ( iD(15279), "sig:TRICE32 with 1 to 12 values (line %d)\n", __LINE__ );

        break; case __LINE__: TRICE32( id(15280), "rd:TRICE32 %d\n", -1 );                                                                                            //exp: time:            default: rd:TRICE32 -1     
        break; case __LINE__: TRICE32( id(15281), "rd:TRICE32 %d, %d\n", -1, -2 );                                                                                    //exp: time:            default: rd:TRICE32 -1, -2 
        break; case __LINE__: TRICE32( id(15282), "rd:TRICE32 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: rd:TRICE32 -1, -2, -3       
        break; case __LINE__: TRICE32( id(15283), "rd:TRICE32 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:            default: rd:TRICE32 -1, -2, -3, -4      
        break; case __LINE__: TRICE32( id(15284), "rd:TRICE32 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:            default: rd:TRICE32 -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE32( id(15285), "rd:TRICE32 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: rd:TRICE32 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE32( id(15286), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:            default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE32( id(15287), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:            default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE32( id(15288), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE32( id(15289), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:            default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE32( id(15290), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:            default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE32( id(15291), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE32( Id(15292), "rd:TRICE32 %d\n", -1 );                                                                                            //exp: time:       5_654default: rd:TRICE32 -1     
        break; case __LINE__: TRICE32( Id(15293), "rd:TRICE32 %d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: rd:TRICE32 -1, -2 
        break; case __LINE__: TRICE32( Id(15294), "rd:TRICE32 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:       5_654default: rd:TRICE32 -1, -2, -3       
        break; case __LINE__: TRICE32( Id(15295), "rd:TRICE32 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:       5_654default: rd:TRICE32 -1, -2, -3, -4      
        break; case __LINE__: TRICE32( Id(15296), "rd:TRICE32 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: rd:TRICE32 -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE32( Id(15297), "rd:TRICE32 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:       5_654default: rd:TRICE32 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE32( Id(15298), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:       5_654default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE32( Id(15299), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE32( Id(15300), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:       5_654default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE32( Id(15301), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:       5_654default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE32( Id(15302), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE32( Id(15303), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:       5_654default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE32( ID(15304), "rd:TRICE32 %d\n", -1 );                                                                                            //exp: time: 842,150_450default: rd:TRICE32 -1     
        break; case __LINE__: TRICE32( ID(15305), "rd:TRICE32 %d, %d\n", -1, -2 );                                                                                    //exp: time: 842,150_450default: rd:TRICE32 -1, -2 
        break; case __LINE__: TRICE32( ID(15306), "rd:TRICE32 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time: 842,150_450default: rd:TRICE32 -1, -2, -3       
        break; case __LINE__: TRICE32( ID(15307), "rd:TRICE32 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: rd:TRICE32 -1, -2, -3, -4      
        break; case __LINE__: TRICE32( ID(15308), "rd:TRICE32 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time: 842,150_450default: rd:TRICE32 -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE32( ID(15309), "rd:TRICE32 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time: 842,150_450default: rd:TRICE32 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE32( ID(15310), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time: 842,150_450default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE32( ID(15311), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time: 842,150_450default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE32( ID(15312), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time: 842,150_450default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE32( ID(15313), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time: 842,150_450default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE32( ID(15314), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time: 842,150_450default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE32( ID(15315), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time: 842,150_450default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: trice     ( iD(15316), "signal:TRICE32_n with 1 to 12 values (line %d)\n", __LINE__ );

        break; case __LINE__: TRICE32_1 ( id(15317), "rd:TRICE32_1  %d\n", -1 );                                                                                            //exp: time:            default: rd:TRICE32_1  -1     
        break; case __LINE__: TRICE32_2 ( id(15318), "rd:TRICE32_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:            default: rd:TRICE32_2  -1, -2 
        break; case __LINE__: TRICE32_3 ( id(15319), "rd:TRICE32_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: rd:TRICE32_3  -1, -2, -3       
        break; case __LINE__: TRICE32_4 ( id(15320), "rd:TRICE32_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:            default: rd:TRICE32_4  -1, -2, -3, -4      
        break; case __LINE__: TRICE32_5 ( id(15321), "rd:TRICE32_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:            default: rd:TRICE32_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE32_6 ( id(15322), "rd:TRICE32_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: rd:TRICE32_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE32_7 ( id(15323), "rd:TRICE32_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:            default: rd:TRICE32_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE32_8 ( id(15324), "rd:TRICE32_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:            default: rd:TRICE32_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE32_9 ( id(15325), "rd:TRICE32_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: rd:TRICE32_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE32_10( id(15326), "rd:TRICE32_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:            default: rd:TRICE32_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE32_11( id(15327), "rd:TRICE32_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:            default: rd:TRICE32_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE32_12( id(15328), "rd:TRICE32_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: rd:TRICE32_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE32_1 ( Id(15329), "rd:TRICE32_1  %d\n", -1 );                                                                                            //exp: time:       5_654default: rd:TRICE32_1  -1     
        break; case __LINE__: TRICE32_2 ( Id(15330), "rd:TRICE32_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: rd:TRICE32_2  -1, -2 
        break; case __LINE__: TRICE32_3 ( Id(15331), "rd:TRICE32_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:       5_654default: rd:TRICE32_3  -1, -2, -3       
        break; case __LINE__: TRICE32_4 ( Id(15332), "rd:TRICE32_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:       5_654default: rd:TRICE32_4  -1, -2, -3, -4      
        break; case __LINE__: TRICE32_5 ( Id(15333), "rd:TRICE32_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: rd:TRICE32_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE32_6 ( Id(15334), "rd:TRICE32_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:       5_654default: rd:TRICE32_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE32_7 ( Id(15335), "rd:TRICE32_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:       5_654default: rd:TRICE32_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE32_8 ( Id(15336), "rd:TRICE32_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: rd:TRICE32_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE32_9 ( Id(15337), "rd:TRICE32_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:       5_654default: rd:TRICE32_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE32_10( Id(15338), "rd:TRICE32_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:       5_654default: rd:TRICE32_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE32_11( Id(15339), "rd:TRICE32_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: rd:TRICE32_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE32_12( Id(15340), "rd:TRICE32_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:       5_654default: rd:TRICE32_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE32_1 ( ID(15341), "rd:TRICE32_1  %d\n", -1 );                                                                                            //exp: time: 842,150_450default: rd:TRICE32_1  -1     
        break; case __LINE__: TRICE32_2 ( ID(15342), "rd:TRICE32_2  %d, %d\n", -1, -2 );                                                                                    //exp: time: 842,150_450default: rd:TRICE32_2  -1, -2 
        break; case __LINE__: TRICE32_3 ( ID(15343), "rd:TRICE32_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time: 842,150_450default: rd:TRICE32_3  -1, -2, -3       
        break; case __LINE__: TRICE32_4 ( ID(15344), "rd:TRICE32_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: rd:TRICE32_4  -1, -2, -3, -4      
        break; case __LINE__: TRICE32_5 ( ID(15345), "rd:TRICE32_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time: 842,150_450default: rd:TRICE32_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE32_6 ( ID(15346), "rd:TRICE32_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time: 842,150_450default: rd:TRICE32_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE32_7 ( ID(15347), "rd:TRICE32_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time: 842,150_450default: rd:TRICE32_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE32_8 ( ID(15348), "rd:TRICE32_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time: 842,150_450default: rd:TRICE32_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE32_9 ( ID(15349), "rd:TRICE32_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time: 842,150_450default: rd:TRICE32_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE32_10( ID(15350), "rd:TRICE32_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time: 842,150_450default: rd:TRICE32_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE32_11( ID(15351), "rd:TRICE32_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time: 842,150_450default: rd:TRICE32_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE32_12( ID(15352), "rd:TRICE32_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time: 842,150_450default: rd:TRICE32_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: trice  ( iD(15353), "sig:trice32 with 1 to 12 values (line %d)\n", __LINE__ );

        break; case __LINE__: trice32( iD(15354), "rd:trice32 %d\n", -1 );                                                                                            //exp: time:            default: rd:trice32 -1     
        break; case __LINE__: trice32( iD(15355), "rd:trice32 %d, %d\n", -1, -2 );                                                                                    //exp: time:            default: rd:trice32 -1, -2 
        break; case __LINE__: trice32( iD(15356), "rd:trice32 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: rd:trice32 -1, -2, -3       
        break; case __LINE__: trice32( iD(15357), "rd:trice32 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:            default: rd:trice32 -1, -2, -3, -4      
        break; case __LINE__: trice32( iD(15358), "rd:trice32 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:            default: rd:trice32 -1, -2, -3, -4, -5        
        break; case __LINE__: trice32( iD(15359), "rd:trice32 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: rd:trice32 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice32( iD(15360), "rd:trice32 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:            default: rd:trice32 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice32( iD(15361), "rd:trice32 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:            default: rd:trice32 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice32( iD(15362), "rd:trice32 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: rd:trice32 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice32( iD(15363), "rd:trice32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:            default: rd:trice32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice32( iD(15364), "rd:trice32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:            default: rd:trice32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice32( iD(15365), "rd:trice32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: rd:trice32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: Trice32( iD(15366), "rd:Trice32 %d\n", -1 );                                                                                            //exp: time:       5_654default: rd:Trice32 -1     
        break; case __LINE__: Trice32( iD(15367), "rd:Trice32 %d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: rd:Trice32 -1, -2 
        break; case __LINE__: Trice32( iD(15368), "rd:Trice32 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:       5_654default: rd:Trice32 -1, -2, -3       
        break; case __LINE__: Trice32( iD(15369), "rd:Trice32 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:       5_654default: rd:Trice32 -1, -2, -3, -4      
        break; case __LINE__: Trice32( iD(15370), "rd:Trice32 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: rd:Trice32 -1, -2, -3, -4, -5        
        break; case __LINE__: Trice32( iD(15371), "rd:Trice32 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:       5_654default: rd:Trice32 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: Trice32( iD(15372), "rd:Trice32 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:       5_654default: rd:Trice32 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: Trice32( iD(15373), "rd:Trice32 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: rd:Trice32 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: Trice32( iD(15374), "rd:Trice32 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:       5_654default: rd:Trice32 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: Trice32( iD(15375), "rd:Trice32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:       5_654default: rd:Trice32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: Trice32( iD(15376), "rd:Trice32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: rd:Trice32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: Trice32( iD(15377), "rd:Trice32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:       5_654default: rd:Trice32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRice32( iD(15378), "rd:TRice32 %d\n", -1 );                                                                                            //exp: time: 842,150_450default: rd:TRice32 -1     
        break; case __LINE__: TRice32( iD(15379), "rd:TRice32 %d, %d\n", -1, -2 );                                                                                    //exp: time: 842,150_450default: rd:TRice32 -1, -2 
        break; case __LINE__: TRice32( iD(15380), "rd:TRice32 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time: 842,150_450default: rd:TRice32 -1, -2, -3       
        break; case __LINE__: TRice32( iD(15381), "rd:TRice32 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: rd:TRice32 -1, -2, -3, -4      
        break; case __LINE__: TRice32( iD(15382), "rd:TRice32 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time: 842,150_450default: rd:TRice32 -1, -2, -3, -4, -5        
        break; case __LINE__: TRice32( iD(15383), "rd:TRice32 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time: 842,150_450default: rd:TRice32 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRice32( iD(15384), "rd:TRice32 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time: 842,150_450default: rd:TRice32 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRice32( iD(15385), "rd:TRice32 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time: 842,150_450default: rd:TRice32 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRice32( iD(15386), "rd:TRice32 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time: 842,150_450default: rd:TRice32 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRice32( iD(15387), "rd:TRice32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time: 842,150_450default: rd:TRice32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRice32( iD(15388), "rd:TRice32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time: 842,150_450default: rd:TRice32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRice32( iD(15389), "rd:TRice32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time: 842,150_450default: rd:TRice32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: trice     ( iD(15390), "signal:trice32_n with 1 to 12 values (line %d)\n", __LINE__ );

        break; case __LINE__: trice32_1 ( iD(15391), "rd:trice32_1  %d\n", -1 );                                                                                            //exp: time:            default: rd:trice32_1  -1     
        break; case __LINE__: trice32_2 ( iD(15392), "rd:trice32_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:            default: rd:trice32_2  -1, -2 
        break; case __LINE__: trice32_3 ( iD(15393), "rd:trice32_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: rd:trice32_3  -1, -2, -3       
        break; case __LINE__: trice32_4 ( iD(15394), "rd:trice32_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:            default: rd:trice32_4  -1, -2, -3, -4      
        break; case __LINE__: trice32_5 ( iD(15395), "rd:trice32_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:            default: rd:trice32_5  -1, -2, -3, -4, -5        
        break; case __LINE__: trice32_6 ( iD(15396), "rd:trice32_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: rd:trice32_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice32_7 ( iD(15397), "rd:trice32_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:            default: rd:trice32_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice32_8 ( iD(15398), "rd:trice32_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:            default: rd:trice32_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice32_9 ( iD(15399), "rd:trice32_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: rd:trice32_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice32_10( iD(15400), "rd:trice32_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:            default: rd:trice32_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice32_11( iD(15401), "rd:trice32_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:            default: rd:trice32_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice32_12( iD(15402), "rd:trice32_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: rd:trice32_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: Trice32_1 ( iD(15403), "rd:Trice32_1  %d\n", -1 );                                                                                            //exp: time:       5_654default: rd:Trice32_1  -1     
        break; case __LINE__: Trice32_2 ( iD(15404), "rd:Trice32_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: rd:Trice32_2  -1, -2 
        break; case __LINE__: Trice32_3 ( iD(15405), "rd:Trice32_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:       5_654default: rd:Trice32_3  -1, -2, -3       
        break; case __LINE__: Trice32_4 ( iD(15406), "rd:Trice32_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:       5_654default: rd:Trice32_4  -1, -2, -3, -4      
        break; case __LINE__: Trice32_5 ( iD(15407), "rd:Trice32_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: rd:Trice32_5  -1, -2, -3, -4, -5        
        break; case __LINE__: Trice32_6 ( iD(15408), "rd:Trice32_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:       5_654default: rd:Trice32_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: Trice32_7 ( iD(15409), "rd:Trice32_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:       5_654default: rd:Trice32_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: Trice32_8 ( iD(15410), "rd:Trice32_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: rd:Trice32_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: Trice32_9 ( iD(15411), "rd:Trice32_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:       5_654default: rd:Trice32_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: Trice32_10( iD(15412), "rd:Trice32_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:       5_654default: rd:Trice32_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: Trice32_11( iD(15413), "rd:Trice32_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: rd:Trice32_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: Trice32_12( iD(15414), "rd:Trice32_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:       5_654default: rd:Trice32_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRice32_1 ( iD(15415), "rd:TRice32_1  %d\n", -1 );                                                                                            //exp: time: 842,150_450default: rd:TRice32_1  -1     
        break; case __LINE__: TRice32_2 ( iD(15416), "rd:TRice32_2  %d, %d\n", -1, -2 );                                                                                    //exp: time: 842,150_450default: rd:TRice32_2  -1, -2 
        break; case __LINE__: TRice32_3 ( iD(15417), "rd:TRice32_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time: 842,150_450default: rd:TRice32_3  -1, -2, -3       
        break; case __LINE__: TRice32_4 ( iD(15418), "rd:TRice32_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: rd:TRice32_4  -1, -2, -3, -4      
        break; case __LINE__: TRice32_5 ( iD(15419), "rd:TRice32_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time: 842,150_450default: rd:TRice32_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRice32_6 ( iD(15420), "rd:TRice32_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time: 842,150_450default: rd:TRice32_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRice32_7 ( iD(15421), "rd:TRice32_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time: 842,150_450default: rd:TRice32_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRice32_8 ( iD(15422), "rd:TRice32_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time: 842,150_450default: rd:TRice32_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRice32_9 ( iD(15423), "rd:TRice32_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time: 842,150_450default: rd:TRice32_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRice32_10( iD(15424), "rd:TRice32_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time: 842,150_450default: rd:TRice32_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRice32_11( iD(15425), "rd:TRice32_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time: 842,150_450default: rd:TRice32_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRice32_12( iD(15426), "rd:TRice32_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time: 842,150_450default: rd:TRice32_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: trice  ( iD(15427), "sig:TRICE64 with 1 to 12 values (line %d)\n", __LINE__ );

        break; case __LINE__: TRICE64( id(15428), "rd:TRICE64 %d\n", -1 );                                                                                            //exp: time:            default: rd:TRICE64 -1     
        break; case __LINE__: TRICE64( id(15429), "rd:TRICE64 %d, %d\n", -1, -2 );                                                                                    //exp: time:            default: rd:TRICE64 -1, -2 
        break; case __LINE__: TRICE64( id(15430), "rd:TRICE64 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: rd:TRICE64 -1, -2, -3       
        break; case __LINE__: TRICE64( id(15431), "rd:TRICE64 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:            default: rd:TRICE64 -1, -2, -3, -4      
        break; case __LINE__: TRICE64( id(15432), "rd:TRICE64 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:            default: rd:TRICE64 -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE64( id(15433), "rd:TRICE64 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: rd:TRICE64 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE64( id(15434), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:            default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE64( id(15435), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:            default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE64( id(15436), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE64( id(15437), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:            default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE64( id(15438), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:            default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE64( id(15439), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE64( Id(15440), "rd:TRICE64 %d\n", -1 );                                                                                            //exp: time:       5_654default: rd:TRICE64 -1     
        break; case __LINE__: TRICE64( Id(15441), "rd:TRICE64 %d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: rd:TRICE64 -1, -2 
        break; case __LINE__: TRICE64( Id(15442), "rd:TRICE64 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:       5_654default: rd:TRICE64 -1, -2, -3       
        break; case __LINE__: TRICE64( Id(15443), "rd:TRICE64 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:       5_654default: rd:TRICE64 -1, -2, -3, -4      
        break; case __LINE__: TRICE64( Id(15444), "rd:TRICE64 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: rd:TRICE64 -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE64( Id(15445), "rd:TRICE64 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:       5_654default: rd:TRICE64 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE64( Id(15446), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:       5_654default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE64( Id(15447), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE64( Id(15448), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:       5_654default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE64( Id(15449), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:       5_654default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE64( Id(15450), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE64( Id(15451), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:       5_654default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE64( ID(15452), "rd:TRICE64 %d\n", -1 );                                                                                            //exp: time: 842,150_450default: rd:TRICE64 -1     
        break; case __LINE__: TRICE64( ID(15453), "rd:TRICE64 %d, %d\n", -1, -2 );                                                                                    //exp: time: 842,150_450default: rd:TRICE64 -1, -2 
        break; case __LINE__: TRICE64( ID(15454), "rd:TRICE64 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time: 842,150_450default: rd:TRICE64 -1, -2, -3       
        break; case __LINE__: TRICE64( ID(15455), "rd:TRICE64 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: rd:TRICE64 -1, -2, -3, -4      
        break; case __LINE__: TRICE64( ID(15456), "rd:TRICE64 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time: 842,150_450default: rd:TRICE64 -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE64( ID(15457), "rd:TRICE64 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time: 842,150_450default: rd:TRICE64 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE64( ID(15458), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time: 842,150_450default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE64( ID(15459), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time: 842,150_450default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE64( ID(15460), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time: 842,150_450default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE64( ID(15461), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time: 842,150_450default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE64( ID(15462), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time: 842,150_450default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE64( ID(15463), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time: 842,150_450default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: trice     ( iD(15464), "signal:TRICE64_n with 1 to 12 values (line %d)\n", __LINE__ );

        break; case __LINE__: TRICE64_1 ( id(15465), "rd:TRICE64_1  %d\n", -1 );                                                                                            //exp: time:            default: rd:TRICE64_1  -1     
        break; case __LINE__: TRICE64_2 ( id(15466), "rd:TRICE64_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:            default: rd:TRICE64_2  -1, -2 
        break; case __LINE__: TRICE64_3 ( id(15467), "rd:TRICE64_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: rd:TRICE64_3  -1, -2, -3       
        break; case __LINE__: TRICE64_4 ( id(15468), "rd:TRICE64_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:            default: rd:TRICE64_4  -1, -2, -3, -4      
        break; case __LINE__: TRICE64_5 ( id(15469), "rd:TRICE64_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:            default: rd:TRICE64_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE64_6 ( id(15470), "rd:TRICE64_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: rd:TRICE64_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE64_7 ( id(15471), "rd:TRICE64_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:            default: rd:TRICE64_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE64_8 ( id(15472), "rd:TRICE64_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:            default: rd:TRICE64_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE64_9 ( id(15473), "rd:TRICE64_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: rd:TRICE64_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE64_10( id(15474), "rd:TRICE64_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:            default: rd:TRICE64_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE64_11( id(15475), "rd:TRICE64_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:            default: rd:TRICE64_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE64_12( id(15476), "rd:TRICE64_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: rd:TRICE64_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE64_1 ( Id(15477), "rd:TRICE64_1  %d\n", -1 );                                                                                            //exp: time:       5_654default: rd:TRICE64_1  -1     
        break; case __LINE__: TRICE64_2 ( Id(15478), "rd:TRICE64_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: rd:TRICE64_2  -1, -2 
        break; case __LINE__: TRICE64_3 ( Id(15479), "rd:TRICE64_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:       5_654default: rd:TRICE64_3  -1, -2, -3       
        break; case __LINE__: TRICE64_4 ( Id(15480), "rd:TRICE64_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:       5_654default: rd:TRICE64_4  -1, -2, -3, -4      
        break; case __LINE__: TRICE64_5 ( Id(15481), "rd:TRICE64_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: rd:TRICE64_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE64_6 ( Id(15482), "rd:TRICE64_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:       5_654default: rd:TRICE64_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE64_7 ( Id(15483), "rd:TRICE64_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:       5_654default: rd:TRICE64_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE64_8 ( Id(15484), "rd:TRICE64_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: rd:TRICE64_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE64_9 ( Id(15485), "rd:TRICE64_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:       5_654default: rd:TRICE64_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE64_10( Id(15486), "rd:TRICE64_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:       5_654default: rd:TRICE64_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE64_11( Id(15487), "rd:TRICE64_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: rd:TRICE64_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE64_12( Id(15488), "rd:TRICE64_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:       5_654default: rd:TRICE64_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE64_1 ( ID(15489), "rd:TRICE64_1  %d\n", -1 );                                                                                            //exp: time: 842,150_450default: rd:TRICE64_1  -1     
        break; case __LINE__: TRICE64_2 ( ID(15490), "rd:TRICE64_2  %d, %d\n", -1, -2 );                                                                                    //exp: time: 842,150_450default: rd:TRICE64_2  -1, -2 
        break; case __LINE__: TRICE64_3 ( ID(15491), "rd:TRICE64_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time: 842,150_450default: rd:TRICE64_3  -1, -2, -3       
        break; case __LINE__: TRICE64_4 ( ID(15492), "rd:TRICE64_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: rd:TRICE64_4  -1, -2, -3, -4      
        break; case __LINE__: TRICE64_5 ( ID(15493), "rd:TRICE64_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time: 842,150_450default: rd:TRICE64_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE64_6 ( ID(15494), "rd:TRICE64_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time: 842,150_450default: rd:TRICE64_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE64_7 ( ID(15495), "rd:TRICE64_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time: 842,150_450default: rd:TRICE64_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE64_8 ( ID(15496), "rd:TRICE64_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time: 842,150_450default: rd:TRICE64_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE64_9 ( ID(15497), "rd:TRICE64_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time: 842,150_450default: rd:TRICE64_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE64_10( ID(15498), "rd:TRICE64_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time: 842,150_450default: rd:TRICE64_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE64_11( ID(15499), "rd:TRICE64_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time: 842,150_450default: rd:TRICE64_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE64_12( ID(15500), "rd:TRICE64_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time: 842,150_450default: rd:TRICE64_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: trice  ( iD(15501), "sig:trice64 with 1 to 12 values (line %d)\n", __LINE__ );

        break; case __LINE__: trice64( iD(15502), "rd:trice64 %d\n", -1 );                                                                                            //exp: time:            default: rd:trice64 -1     
        break; case __LINE__: trice64( iD(15503), "rd:trice64 %d, %d\n", -1, -2 );                                                                                    //exp: time:            default: rd:trice64 -1, -2 
        break; case __LINE__: trice64( iD(15504), "rd:trice64 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: rd:trice64 -1, -2, -3       
        break; case __LINE__: trice64( iD(15505), "rd:trice64 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:            default: rd:trice64 -1, -2, -3, -4      
        break; case __LINE__: trice64( iD(15506), "rd:trice64 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:            default: rd:trice64 -1, -2, -3, -4, -5        
        break; case __LINE__: trice64( iD(15507), "rd:trice64 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: rd:trice64 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice64( iD(15508), "rd:trice64 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:            default: rd:trice64 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice64( iD(15509), "rd:trice64 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:            default: rd:trice64 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice64( iD(15510), "rd:trice64 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: rd:trice64 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice64( iD(15511), "rd:trice64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:            default: rd:trice64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice64( iD(15512), "rd:trice64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:            default: rd:trice64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice64( iD(15513), "rd:trice64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: rd:trice64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: Trice64( iD(15514), "rd:Trice64 %d\n", -1 );                                                                                            //exp: time:       5_654default: rd:Trice64 -1     
        break; case __LINE__: Trice64( iD(15515), "rd:Trice64 %d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: rd:Trice64 -1, -2 
        break; case __LINE__: Trice64( iD(15516), "rd:Trice64 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:       5_654default: rd:Trice64 -1, -2, -3       
        break; case __LINE__: Trice64( iD(15517), "rd:Trice64 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:       5_654default: rd:Trice64 -1, -2, -3, -4      
        break; case __LINE__: Trice64( iD(15518), "rd:Trice64 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: rd:Trice64 -1, -2, -3, -4, -5        
        break; case __LINE__: Trice64( iD(15519), "rd:Trice64 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:       5_654default: rd:Trice64 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: Trice64( iD(15520), "rd:Trice64 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:       5_654default: rd:Trice64 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: Trice64( iD(15521), "rd:Trice64 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: rd:Trice64 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: Trice64( iD(15522), "rd:Trice64 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:       5_654default: rd:Trice64 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: Trice64( iD(15523), "rd:Trice64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:       5_654default: rd:Trice64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: Trice64( iD(15524), "rd:Trice64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: rd:Trice64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: Trice64( iD(15525), "rd:Trice64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:       5_654default: rd:Trice64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRice64( iD(15526), "rd:TRice64 %d\n", -1 );                                                                                            //exp: time: 842,150_450default: rd:TRice64 -1     
        break; case __LINE__: TRice64( iD(15527), "rd:TRice64 %d, %d\n", -1, -2 );                                                                                    //exp: time: 842,150_450default: rd:TRice64 -1, -2 
        break; case __LINE__: TRice64( iD(15528), "rd:TRice64 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time: 842,150_450default: rd:TRice64 -1, -2, -3       
        break; case __LINE__: TRice64( iD(15529), "rd:TRice64 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: rd:TRice64 -1, -2, -3, -4      
        break; case __LINE__: TRice64( iD(15530), "rd:TRice64 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time: 842,150_450default: rd:TRice64 -1, -2, -3, -4, -5        
        break; case __LINE__: TRice64( iD(15531), "rd:TRice64 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time: 842,150_450default: rd:TRice64 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRice64( iD(15532), "rd:TRice64 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time: 842,150_450default: rd:TRice64 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRice64( iD(15533), "rd:TRice64 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time: 842,150_450default: rd:TRice64 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRice64( iD(15534), "rd:TRice64 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time: 842,150_450default: rd:TRice64 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRice64( iD(15535), "rd:TRice64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time: 842,150_450default: rd:TRice64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRice64( iD(15536), "rd:TRice64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time: 842,150_450default: rd:TRice64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRice64( iD(15537), "rd:TRice64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time: 842,150_450default: rd:TRice64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: trice     ( iD(15538), "signal:trice64_n with 1 to 12 values (line %d)\n", __LINE__ );

        break; case __LINE__: trice64_1 ( iD(15539), "rd:trice64_1  %d\n", -1 );                                                                                            //exp: time:            default: rd:trice64_1  -1     
        break; case __LINE__: trice64_2 ( iD(15540), "rd:trice64_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:            default: rd:trice64_2  -1, -2 
        break; case __LINE__: trice64_3 ( iD(15541), "rd:trice64_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: rd:trice64_3  -1, -2, -3       
        break; case __LINE__: trice64_4 ( iD(15542), "rd:trice64_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:            default: rd:trice64_4  -1, -2, -3, -4      
        break; case __LINE__: trice64_5 ( iD(15543), "rd:trice64_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:            default: rd:trice64_5  -1, -2, -3, -4, -5        
        break; case __LINE__: trice64_6 ( iD(15544), "rd:trice64_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: rd:trice64_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice64_7 ( iD(15545), "rd:trice64_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:            default: rd:trice64_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice64_8 ( iD(15546), "rd:trice64_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:            default: rd:trice64_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice64_9 ( iD(15547), "rd:trice64_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: rd:trice64_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice64_10( iD(15548), "rd:trice64_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:            default: rd:trice64_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice64_11( iD(15549), "rd:trice64_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:            default: rd:trice64_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice64_12( iD(15550), "rd:trice64_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: rd:trice64_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: Trice64_1 ( iD(15551), "rd:Trice64_1  %d\n", -1 );                                                                                            //exp: time:       5_654default: rd:Trice64_1  -1     
        break; case __LINE__: Trice64_2 ( iD(15552), "rd:Trice64_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:       5_654default: rd:Trice64_2  -1, -2 
        break; case __LINE__: Trice64_3 ( iD(15553), "rd:Trice64_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:       5_654default: rd:Trice64_3  -1, -2, -3       
        break; case __LINE__: Trice64_4 ( iD(15554), "rd:Trice64_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:       5_654default: rd:Trice64_4  -1, -2, -3, -4      
        break; case __LINE__: Trice64_5 ( iD(15555), "rd:Trice64_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:       5_654default: rd:Trice64_5  -1, -2, -3, -4, -5        
        break; case __LINE__: Trice64_6 ( iD(15556), "rd:Trice64_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:       5_654default: rd:Trice64_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: Trice64_7 ( iD(15557), "rd:Trice64_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:       5_654default: rd:Trice64_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: Trice64_8 ( iD(15558), "rd:Trice64_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:       5_654default: rd:Trice64_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: Trice64_9 ( iD(15559), "rd:Trice64_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:       5_654default: rd:Trice64_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: Trice64_10( iD(15560), "rd:Trice64_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:       5_654default: rd:Trice64_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: Trice64_11( iD(15561), "rd:Trice64_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:       5_654default: rd:Trice64_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: Trice64_12( iD(15562), "rd:Trice64_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:       5_654default: rd:Trice64_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRice64_1 ( iD(15563), "rd:TRice64_1  %d\n", -1 );                                                                                            //exp: time: 842,150_450default: rd:TRice64_1  -1     
        break; case __LINE__: TRice64_2 ( iD(15564), "rd:TRice64_2  %d, %d\n", -1, -2 );                                                                                    //exp: time: 842,150_450default: rd:TRice64_2  -1, -2 
        break; case __LINE__: TRice64_3 ( iD(15565), "rd:TRice64_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time: 842,150_450default: rd:TRice64_3  -1, -2, -3       
        break; case __LINE__: TRice64_4 ( iD(15566), "rd:TRice64_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: rd:TRice64_4  -1, -2, -3, -4      
        break; case __LINE__: TRice64_5 ( iD(15567), "rd:TRice64_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time: 842,150_450default: rd:TRice64_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRice64_6 ( iD(15568), "rd:TRice64_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time: 842,150_450default: rd:TRice64_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRice64_7 ( iD(15569), "rd:TRice64_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time: 842,150_450default: rd:TRice64_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRice64_8 ( iD(15570), "rd:TRice64_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time: 842,150_450default: rd:TRice64_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRice64_9 ( iD(15571), "rd:TRice64_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time: 842,150_450default: rd:TRice64_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRice64_10( iD(15572), "rd:TRice64_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time: 842,150_450default: rd:TRice64_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRice64_11( iD(15573), "rd:TRice64_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time: 842,150_450default: rd:TRice64_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRice32_12( iD(15574), "rd:TRice32_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time: 842,150_450default: rd:TRice32_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    





        break; case __LINE__: TRICE  ( ID(15575), "sig:TRICE16 with 1 to 12 pointer\n" );
        break; case __LINE__: TRICE16( ID(15576), "rd:TRICE16 %p\n", -1 );
        break; case __LINE__: TRICE16( ID(15577), "rd:TRICE16 %p, %p\n", -1, -2 );
        break; case __LINE__: TRICE16( ID(15578), "rd:TRICE16 %p, %p, %p\n", -1, -2, -3 );
        break; case __LINE__: TRICE16( ID(15579), "rd:TRICE16 %p, %p, %p, %p\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE16( ID(15580), "rd:TRICE16 %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE16( ID(15581), "rd:TRICE16 %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE16( ID(15582), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE16( ID(15583), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE16( ID(15584), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE16( ID(15585), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE16( ID(15586), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE16( ID(15587), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE  ( ID(15588), "sig:TRICE16 with 1 to 12 hex\n" );
        break; case __LINE__: TRICE16( ID(15589), "rd:TRICE16 %X\n", -1 );
        break; case __LINE__: TRICE16( ID(15590), "rd:TRICE16 %X, %X\n", -1, -2 );
        break; case __LINE__: TRICE16( ID(15591), "rd:TRICE16 %X, %X, %X\n", -1, -2, -3 );
        break; case __LINE__: TRICE16( ID(15592), "rd:TRICE16 %X, %X, %X, %X\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE16( ID(15593), "rd:TRICE16 %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE16( ID(15594), "rd:TRICE16 %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE16( ID(15595), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE16( ID(15596), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE16( ID(15597), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE16( ID(15598), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE16( ID(15599), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE16( ID(15600), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );



        break; case __LINE__: TRICE( ID(15601), "sig:Colors and ticks\n" );
        break; case __LINE__: TRICE( ID(15602), "--------------------------------------------------\n");
        break; case __LINE__: TRICE( ID(15603), "--------------------------------------------------\n");
        break; case __LINE__: TRICE( ID(15604), "dbg:12345 as 16bit is %016b\n", 12345);
        break; case __LINE__: TRICE( ID(15605), "--------------------------------------------------\n");
        break; case __LINE__: TRICE( ID(15606), "sig:This ASSERT error is just a demo and no real error:\n");
        break; case __LINE__: TRICE( ID(15607), "--------------------------------------------------\n");
      //break; case __LINE__: TRICE( ID(15608), "ERR:error       message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID(15609), "WRN:warning     message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID(15610), "ATT:attention   message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID(15611), "DIA:diagnostics message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID(15612), "TIM:timing      message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID(15613), "DBG:debug       message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID(15614), "SIG:signal      message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID(15615), "RD:read         message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID(15616), "WR:write        message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID(15617), "ISR:interrupt   message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID(15618), "MSG:normal      message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID(15619), "INFO:informal   message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE( ID(15620), "time:TRICE32_1  message, SysTick is %6u\n", SYSTICKVAL);

        break; case __LINE__: TRICE( ID(15621), "sig:Various ranges and formats\n" );
        break; case __LINE__: TRICE8 ( ID(15622), "tst:TRICE8  %%03x -> %03x  %03x  %03x  %03x\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID(15623), "tst:TRICE8   %%4d -> %4d %4d %4d %4d\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID(15624), "tst:TRICE8   %%4u -> %4u %4u %4u %4u\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID(15625), "tst:TRICE8   %%4o -> %4o %4o %4o %4o\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID(15626), "tst:TRICE8   %%4O -> %4O %4O %4O %4O\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID(15627), "tst:TRICE8   %%4X -> %4X %4X %4X %4X\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID(15628), "tst:TRICE8   %%4x -> %4x %4x %4x %4x\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID(15629), "tst:TRICE8   %%8b -> %8b %8b %8b %8b\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 ( ID(15630), "tst:TRICE8  %%08b -> %08b %08b %08b %08b\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE16( ID(15631), "tst:TRICE16  %%05x ->   %05x   %05x   %05x   %05x\n", 1, 0x7fff, 0x8000, 0xffff);
        break; case __LINE__: TRICE16( ID(15632), "tst:TRICE16   %%6d ->  %6d  %6d  %6d  %6d\n",         1, 0x7fff, 0x8000, 0xffff);
        break; case __LINE__: TRICE16( ID(15633), "tst:TRICE16   %%7o -> %7o %7o %7o %7o\n",             1, 0x7fff, 0x8000, 0xffff);
        break; case __LINE__: TRICE32( ID(15634), "tst:TRICE32 %%09x ->      %09x      %09x       %09x     %09x\n", 1, 0x7fffffff, 0x80000000, 0xffffffff);
        break; case __LINE__: TRICE32( ID(15635), "tst:TRICE32 %%10d ->     %10d     %10d     %10d    %10x\n",      1, 0x7fffffff, 0x80000000, 0xffffffff);
        break; case __LINE__: TRICE32( ID(15636), "att:TRICE32 %40b\n", 0xAAAAAAAA);
        break; case __LINE__: TRICE64( ID(15637), "tst:TRICE64 %%09x ->      %09x      %09x       %09x     %09x\n", 1, 0x7fffffffffffffff, 0x8000000000000000, 0xffffffffffffffff);
        break; case __LINE__: TRICE64( ID(15638), "tst:TRICE64 %%10d ->     %10d     %10d     %10d    %10x\n",      1, 0x7fffffffffffffff, 0x8000000000000000, 0xffffffffffffffff);
        break; case __LINE__: TRICE64( ID(15639), "att:TRICE64 %70b\n", 0xAAAAAAAAAAAAAAAA);
        break; case __LINE__: TRICE( ID(15640), "sig:Legacy TRICE8\n" );
        break; case __LINE__: TRICE8_1( ID(15641), "tst:TRICE8_1 %02x\n", 0xA1);
        break; case __LINE__: TRICE8_2( ID(15642), "tst:TRICE8_2 %02x %02x\n", 0xA1, 0xA2);
        break; case __LINE__: TRICE8_3( ID(15643), "tst:TRICE8_3 %02x %02x %02x\n", 0xA1, 0xA2, 0xA3);
        break; case __LINE__: TRICE8_4( ID(15644), "tst:TRICE8_4 %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4);
        break; case __LINE__: TRICE8_5( ID(15645), "tst:TRICE8_5 %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5 );
        break; case __LINE__: TRICE8_6( ID(15646), "tst:TRICE8_6 %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6);
        break; case __LINE__: TRICE8_7( ID(15647), "tst:TRICE8_7 %02x %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7);
        break; case __LINE__: TRICE8_8( ID(15648), "tst:TRICE8_8 %02X %02X %02X %02X %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8); //exp: time: 842,150_450default: tst:TRICE8_8 A1 A2 A3 A4 a5 a6 a7 a8
        break; case __LINE__: TRICE8_1( ID(15649), "tst:TRICE8_1 %d\n",                      -1);
        break; case __LINE__: TRICE8_2( ID(15650), "tst:TRICE8_2 %d %d\n",                   -1, -2);
        break; case __LINE__: TRICE8_3( ID(15651), "tst:TRICE8_3 %d %d %d\n",                -1, -2, -3);
        break; case __LINE__: TRICE8_4( ID(15652), "tst:TRICE8_4 %d %d %d %d\n",             -1, -2, -3, -4);
        break; case __LINE__: TRICE8_5( ID(15653), "tst:TRICE8_5 %d %d %d %d %d\n",          -1, -2, -3, -4, -5);
        break; case __LINE__: TRICE8_6( ID(15654), "tst:TRICE8_6 %d %d %d %d %d %d\n",       -1, -2, -3, -4, -5, -6);
        break; case __LINE__: TRICE8_7( ID(15655), "tst:TRICE8_7 %d %d %d %d %d %d %d\n",    -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE8_8( ID(15656), "tst:TRICE8_8 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8);
        break; case __LINE__: TRICE8_1( ID(15657), "tst:TRICE8_1 %d\n", 1);
        break; case __LINE__: TRICE8_2( ID(15658), "tst:TRICE8_2 %d %d\n", 1, 2);
        break; case __LINE__: TRICE8_3( ID(15659), "tst:TRICE8_3 %d %d %d\n", 1, 2, 3);
        break; case __LINE__: TRICE8_4( ID(15660), "tst:TRICE8_4 %d %d %d %d\n", 1, 2, 3, 4);
        break; case __LINE__: TRICE8_5( ID(15661), "tst:TRICE8_5 %d %d %d %d %d\n", 1, 2, 3, 4, 5);
        break; case __LINE__: TRICE8_6( ID(15662), "tst:TRICE8_6 %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6);
        break; case __LINE__: TRICE8_7( ID(15663), "tst:TRICE8_7 %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7);
        break; case __LINE__: TRICE8_8( ID(15664), "tst:TRICE8_8 %d %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7, 8);
        break; case __LINE__: TRICE8_1( ID(15665), "tst:TRICE8_1 %u\n", 201);
        break; case __LINE__: TRICE8_2( ID(15666), "tst:TRICE8_2 %u %u\n", 201, 202);
        break; case __LINE__: TRICE8_3( ID(15667), "tst:TRICE8_3 %u %u %u\n", 201, 202, 203);
        break; case __LINE__: TRICE8_4( ID(15668), "tst:TRICE8_4 %u %u %u %u\n", 201, 202, 203, 204);
        break; case __LINE__: TRICE8_5( ID(15669), "tst:TRICE8_5 %u %u %u %u %u\n", 201, 202, 203, 204, 205);
        break; case __LINE__: TRICE8_6( ID(15670), "tst:TRICE8_6 %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206);
        break; case __LINE__: TRICE8_7( ID(15671), "tst:TRICE8_7 %u %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206, 207);
        break; case __LINE__: TRICE8_8( ID(15672), "tst:TRICE8_8 %u %u %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206, 207, 208);
        break; case __LINE__: TRICE8_8( ID(15673), "tst:TRICE8_1 %%d=%d, %%u=%u, 0x%%x=0x%x, 0x%%2x=0x%2x, 0x%%02x=0x%02x, 0x%%3x=0x%3x, 0x%%03x=0x%03x, %%b=%b\n", 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81);

        break; case __LINE__: TRICE    ( ID(15674), "sig:Legacy TRICE16-64\n" );
        break; case __LINE__: TRICE16_1( ID(15675), "tst:TRICE16_1 %d\n", -111);
        break; case __LINE__: TRICE16_2( ID(15676), "tst:TRICE16_2 %d %d\n", -111, -222);
        break; case __LINE__: TRICE16_3( ID(15677), "tst:TRICE16_3 %d %d %d\n", -111, -222, -333);
        break; case __LINE__: TRICE16_4( ID(15678), "tst:TRICE16_4 %d %d %d %d\n", -111, -222, -333, -444);
        break; case __LINE__: TRICE32_1( ID(15679), "tst:TRICE32_1 %08x\n", 0x0123cafe);
        break; case __LINE__: TRICE32_1( ID(15680), "tst:TRICE32_1 %d\n", -111);
        break; case __LINE__: TRICE32_2( ID(15681), "tst:TRICE32_2 %x %x\n", -111, -222);
        break; case __LINE__: TRICE32_2( ID(15682), "tst:TRICE32_2 %d %d\n", -111, -222);
        break; case __LINE__: TRICE32_3( ID(15683), "tst:TRICE32_3 %x %x %x\n", -111, -222, -333);
        break; case __LINE__: TRICE32_3( ID(15684), "tst:TRICE32_3 %d %d %d\n", -111, -222, -333);
        break; case __LINE__: TRICE32_4( ID(15685), "tst:TRICE32_4 %x %x %x %x\n", -111, -222, -333, -444);
        break; case __LINE__: TRICE32_4( ID(15686), "tst:TRICE32_4 %d %d %d %d\n", -111, -222, -333, -444);
        break; case __LINE__: TRICE64_1( ID(15687), "tst:TRICE64_1 %d\n", -111);
        break; case __LINE__: TRICE64_2( ID(15688), "tst:TRICE64_2 %d %d\n", -111, -222);
        break; case __LINE__: TRICE16_1( ID(15689), "tst:TRICE16_1 %u\n", 60001);
        break; case __LINE__: TRICE16_2( ID(15690), "tst:TRICE16_2 %u %u\n", 60001, 60002);
        break; case __LINE__: TRICE16_3( ID(15691), "tst:TRICE16_3 %u %u %u\n", 60001, 60002, 60003);
        break; case __LINE__: TRICE16_4( ID(15692), "tst:TRICE16_4 %u %u %u %u\n", 60001, 60002, 60003, 60004);
        break; case __LINE__: TRICE32_1( ID(15693), "tst:TRICE32_1 %u\n", 4000000001);
        break; case __LINE__: TRICE32_2( ID(15694), "tst:TRICE32_2 %u %u\n", 4000000001, 4000000002);
        break; case __LINE__: TRICE32_3( ID(15695), "tst:TRICE32_3 %u %u %u\n", 4000000001, 4000000002, 4000000003);
        break; case __LINE__: TRICE32_4( ID(15696), "tst:TRICE32_4 %u %u %u %u\n", 4000000001, 4000000002, 4000000003, 4000000004);
        break; case __LINE__: TRICE64_1( ID(15697), "tst:TRICE64_1 %x\n", -1);
        break; case __LINE__: TRICE64_2( ID(15698), "tst:TRICE64_2 %x %x\n", -1, -2);
        break; case __LINE__: TRICE64_1( ID(15699), "tst:TRICE64_1 %u\n", -1);
        break; case __LINE__: TRICE64_2( ID(15700), "tst:TRICE64_2 %u %u\n", -1, -2);

        break; case __LINE__: TRICE( ID(15701), "sig:colored single letters and several TRICE macros in one line\n" );
        break; case __LINE__: TRICE( ID(15702), "e:A");
        break; case __LINE__: TRICE( ID(15703), "w:B");
        break; case __LINE__: TRICE( ID(15704), "a:c");
        break; case __LINE__: TRICE( ID(15705), "wr:d");
        break; case __LINE__: TRICE( ID(15706), "rd:e\n");
        break; case __LINE__: TRICE( ID(15707), "diag:f");
        break; case __LINE__: TRICE( ID(15708), "d:G");
        break; case __LINE__: TRICE( ID(15709), "t:H");
        break; case __LINE__: TRICE( ID(15710), "time:i");
        break; case __LINE__: TRICE( ID(15711), "message:J");
        break; case __LINE__: TRICE( ID(15712), "dbg:k\n");
        break; case __LINE__: TRICE( ID(15713), "1");
        break; case __LINE__: TRICE( ID(15714), "2");
        break; case __LINE__: TRICE( ID(15715), "3");
        break; case __LINE__: TRICE( ID(15716), "4");
        break; case __LINE__: TRICE( ID(15717), "e:7");
        break; case __LINE__: TRICE( ID(15718), "m:12");
        break; case __LINE__: TRICE( ID(15719), "m:123\n");
        break; case __LINE__: TRICE( ID(15720), "e:A"); TRICE( ID(15721), "w:B"); TRICE( ID(15722), "a:c");
        break; case __LINE__: TRICE( ID(15723), "wr:d"); TRICE( ID(15724), "rd:e\n"); TRICE( ID(15725), "diag:f");

        break; case __LINE__: TRICE( ID(15726), "sig:TRICE8 with variable param count 1 to 12\n" );
        break; case __LINE__: TRICE8( ID(15727), "tst:TRICE8 %d\n", -1 );
        break; case __LINE__: TRICE8( ID(15728), "tst:TRICE8 %d %d\n", -1, -2 );
        break; case __LINE__: TRICE8( ID(15729), "tst:TRICE8 %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE8( ID(15730), "tst:TRICE8 %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE8( ID(15731), "tst:TRICE8 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE8( ID(15732), "tst:TRICE8 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE8( ID(15733), "tst:TRICE8 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE8( ID(15734), "tst:TRICE8 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE8( ID(15735), "tst:TRICE8 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE8( ID(15736), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE8( ID(15737), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE8( ID(15738), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE8( ID(15739), "tst:TRICE8 %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );
        break; case __LINE__: TRICE8_1 ( ID(15740), "tst:TRICE8_1  %d\n", -1 );
        break; case __LINE__: TRICE8_2 ( ID(15741), "tst:TRICE8_2  %d %d\n", -1, -2 );
        break; case __LINE__: TRICE8_3 ( ID(15742), "tst:TRICE8_3  %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE8_4 ( ID(15743), "tst:TRICE8_4  %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE8_5 ( ID(15744), "tst:TRICE8_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE8_6 ( ID(15745), "tst:TRICE8_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE8_7 ( ID(15746), "tst:TRICE8_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE8_8 ( ID(15747), "tst:TRICE8_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE8_9 ( ID(15748), "tst:TRICE8_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE8_10( ID(15749), "tst:TRICE8_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE8_11( ID(15750), "tst:TRICE8_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE8_12( ID(15751), "tst:TRICE8_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE8   ( ID(15752), "tst:TRICE8   %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );
        break; case __LINE__: TRICE8_9 ( ID(15753), "tst:TRICE8_9 %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );

        break; case __LINE__: TRICE( ID(15754), "sig:TRICE16 with variable param count 1 to 12\n" );
        break; case __LINE__: TRICE16( ID(15755), "tst:TRICE16 %d\n", -1 );
        break; case __LINE__: TRICE16( ID(15756), "tst:TRICE16 %d %d\n", -1, -2 );
        break; case __LINE__: TRICE16( ID(15757), "tst:TRICE16 %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE16( ID(15758), "tst:TRICE16 %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE16( ID(15759), "tst:TRICE16 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE16( ID(15760), "tst:TRICE16 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE16( ID(15761), "tst:TRICE16 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE16( ID(15762), "tst:TRICE16 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE16( ID(15763), "tst:TRICE16 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE16( ID(15764), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE16( ID(15765), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE16( ID(15766), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE16( ID(15767), "tst:TRICE16 %t %b %x %X %d %u %o %O %p\n", 3, 3, 3, 3, 3, 3, 3, 3, 3 );
        break; case __LINE__: TRICE16_1 ( ID(15768), "tst:TRICE16_1  %d\n", -1 );
        break; case __LINE__: TRICE16_2 ( ID(15769), "tst:TRICE16_2  %d %d\n", -1, -2 );
        break; case __LINE__: TRICE16_3 ( ID(15770), "tst:TRICE16_3  %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE16_4 ( ID(15771), "tst:TRICE16_4  %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE16_5 ( ID(15772), "tst:TRICE16_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE16_6 ( ID(15773), "tst:TRICE16_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE16_7 ( ID(15774), "tst:TRICE16_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE16_8 ( ID(15775), "tst:TRICE16_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE16_9 ( ID(15776), "tst:TRICE16_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE16_10( ID(15777), "tst:TRICE16_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE16_11( ID(15778), "tst:TRICE16_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE16_12( ID(15779), "tst:TRICE16_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE16_9 ( ID(15780), "tst:TRICE16 %t %b %x %X %d %u %o %O %p\n", 3, 3, 3, 3, 3, 3, 3, 3, 3 );

        break; case __LINE__: TRICE( ID(15781), "sig:TRICE32 with variable param count 1 to 12\n" );
        break; case __LINE__: TRICE32( ID(15782), "tst:TRICE32 %d\n", -1 );
        break; case __LINE__: TRICE32( ID(15783), "tst:TRICE32 %d %d\n", -1, -2 );
        break; case __LINE__: TRICE32( ID(15784), "tst:TRICE32 %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE32( ID(15785), "tst:TRICE32 %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE32( ID(15786), "tst:TRICE32 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE32( ID(15787), "tst:TRICE32 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE32( ID(15788), "tst:TRICE32 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE32( ID(15789), "tst:TRICE32 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE32( ID(15790), "tst:TRICE32 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE32( ID(15791), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE32( ID(15792), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE32( ID(15793), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE32( ID(15794), "tst:TRICE32 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));
        break; case __LINE__: TRICE32_1 ( ID(15795), "tst:TRICE32_1  %d\n", -1 );
        break; case __LINE__: TRICE32_2 ( ID(15796), "tst:TRICE32_2  %d %d\n", -1, -2 );
        break; case __LINE__: TRICE32_3 ( ID(15797), "tst:TRICE32_3  %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE32_4 ( ID(15798), "tst:TRICE32_4  %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE32_5 ( ID(15799), "tst:TRICE32_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE32_6 ( ID(15800), "tst:TRICE32_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE32_7 ( ID(15801), "tst:TRICE32_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE32_8 ( ID(15802), "tst:TRICE32_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE32_9 ( ID(15803), "tst:TRICE32_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE32_10( ID(15804), "tst:TRICE32_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE32_11( ID(15805), "tst:TRICE32_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE32_12( ID(15806), "tst:TRICE32_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE32   ( ID(15807), "tst:TRICE32    %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));
        break; case __LINE__: TRICE32_12( ID(15808), "tst:TRICE32_12 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));

        break; case __LINE__: TRICE( ID(15809), "sig:TRICE64 with variable param count 1 to 12\n" );
        break; case __LINE__: TRICE64( ID(15810), "tst:TRICE64 %d\n", -1 );
        break; case __LINE__: TRICE64( ID(15811), "tst:TRICE64 %d %d\n", -1, -2 );
        break; case __LINE__: TRICE64( ID(15812), "tst:TRICE64 %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE64( ID(15813), "tst:TRICE64 %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE64( ID(15814), "tst:TRICE64 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE64( ID(15815), "tst:TRICE64 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE64( ID(15816), "tst:TRICE64 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE64( ID(15817), "tst:TRICE64 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE64( ID(15818), "tst:TRICE64 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE64( ID(15819), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE64( ID(15820), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE64( ID(15821), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE64( ID(15822), "tst:TRICE64 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aDouble(3.14159), aDouble(3.14159), aDouble(3.14159));
        break; case __LINE__: TRICE64_1 ( ID(15823), "tst:TRICE64_1  %d\n", -1 );
        break; case __LINE__: TRICE64_2 ( ID(15824), "tst:TRICE64_2  %d %d\n", -1, -2 );
        break; case __LINE__: TRICE64_3 ( ID(15825), "tst:TRICE64_3  %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE64_4 ( ID(15826), "tst:TRICE64_4  %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE64_5 ( ID(15827), "tst:TRICE64_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE64_6 ( ID(15828), "tst:TRICE64_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE64_7 ( ID(15829), "tst:TRICE64_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE64_8 ( ID(15830), "tst:TRICE64_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE64_9 ( ID(15831), "tst:TRICE64_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE64_10( ID(15832), "tst:TRICE64_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE64_11( ID(15833), "tst:TRICE64_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE64_12( ID(15834), "tst:TRICE64_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE64_12( ID(15835), "tst:TRICE64_12 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aDouble(3.14159), aDouble(3.14159), aDouble(3.14159));

        break; case __LINE__: TRICE( ID(15836), "att:positive and negative float in format variants\n");
        break; case __LINE__: TRICE( ID(15837), "rd:TRICE float %f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID(15838), "rd:TRICE float %9f (%%9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID(15839), "rd:TRICE float %.9f (%%.9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID(15840), "rd:TRICE float %9.f (%%9.f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID(15841), "rd:TRICE float %9.6f (%%9.6f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID(15842), "rd:TRICE float %f (%%f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(15843), "rd:TRICE float %9f (%%9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(15844), "rd:TRICE float %.9f (%%.9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(15845), "rd:TRICE float %9.f (%%9.f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(15846), "rd:TRICE float %9.6f (%%9.6f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(15847), "rd:TRICE float %+f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID(15848), "rd:TRICE float %+9f (%%9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID(15849), "rd:TRICE float %+.9f (%%.9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID(15850), "rd:TRICE float %+9.f (%%9.f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID(15851), "rd:TRICE float %+9.6f (%%9.6f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID(15852), "rd:TRICE float %+f (%%f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(15853), "rd:TRICE float %+9f (%%9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(15854), "rd:TRICE float %+.9f (%%.9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(15855), "rd:TRICE float %+9.f (%%9.f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(15856), "rd:TRICE float %+9.6f (%%9.6f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(15857), "rd:TRICE float %-f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID(15858), "rd:TRICE float %-9f (%%9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID(15859), "rd:TRICE float %-.9f (%%.9f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID(15860), "rd:TRICE float %-9.f (%%9.f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID(15861), "rd:TRICE float %-9.6f (%%9.6f)\n", aFloat(x) );
        break; case __LINE__: TRICE( ID(15862), "rd:TRICE float %-f (%%f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(15863), "rd:TRICE float %-9f (%%9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(15864), "rd:TRICE float %-.9f (%%.9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(15865), "rd:TRICE float %-9.f (%%9.f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(15866), "rd:TRICE float %-9.6f (%%9.6f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(15867), "att:positive float & double in variants\n");
        break; case __LINE__: TRICE32_1( ID(15868), "rd:TRICE32_1 float %e (%%e)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1( ID(15869), "rd:TRICE32_1 float %f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1( ID(15870), "rd:TRICE32_1 float %g (%%g)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1( ID(15871), "rd:TRICE32_1 float %E (%%E)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1( ID(15872), "rd:TRICE32_1 float %F (%%F)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1( ID(15873), "rd:TRICE32_1 float %G (%%G)\n", aFloat(x) );
        break; case __LINE__: TRICE32  ( ID(15874), "rd:TRICE32 float %e (%%e)\n", aFloat(x) );
        break; case __LINE__: TRICE32  ( ID(15875), "rd:TRICE32 float %f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE32  ( ID(15876), "rd:TRICE32 float %g (%%g)\n", aFloat(x) );
        break; case __LINE__: TRICE32  ( ID(15877), "rd:TRICE32 float %E (%%E)\n", aFloat(x) );
        break; case __LINE__: TRICE32  ( ID(15878), "rd:TRICE32 float %F (%%F)\n", aFloat(x) );
        break; case __LINE__: TRICE32  ( ID(15879), "rd:TRICE32 float %G (%%G)\n", aFloat(x) );
        break; case __LINE__: TRICE    ( ID(15880), "rd:TRICE float %e (%%e)\n", aFloat(x) );
        break; case __LINE__: TRICE    ( ID(15881), "rd:TRICE float %f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE    ( ID(15882), "rd:TRICE float %g (%%g)\n", aFloat(x) );
        break; case __LINE__: TRICE    ( ID(15883), "rd:TRICE float %E (%%E)\n", aFloat(x) );
        break; case __LINE__: TRICE    ( ID(15884), "rd:TRICE float %F (%%F)\n", aFloat(x) );
        break; case __LINE__: TRICE    ( ID(15885), "rd:TRICE float %G (%%G)\n", aFloat(x) );
        break; case __LINE__: TRICE64  ( ID(15886), "rd:TRICE64 double %e (%%e), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64  ( ID(15887), "rd:TRICE64 double %f (%%f), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64  ( ID(15888), "rd:TRICE64 double %g (%%g), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64  ( ID(15889), "rd:TRICE64 double %E (%%E), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64  ( ID(15890), "rd:TRICE64 double %F (%%F), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64  ( ID(15891), "rd:TRICE64 double %G (%%G), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( ID(15892), "rd:TRICE64_1 double %e (%%e), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( ID(15893), "rd:TRICE64_1 double %f (%%f), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( ID(15894), "rd:TRICE64_1 double %g (%%g), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( ID(15895), "rd:TRICE64_1 double %E (%%E), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( ID(15896), "rd:TRICE64_1 double %F (%%F), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1( ID(15897), "rd:TRICE64_1 double %G (%%G), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE( ID(15898), "att:negative float & double\n" );
        break; case __LINE__: TRICE( ID(15899), "rd:TRICE float %e (%%e)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(15900), "rd:TRICE float %f (%%f)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(15901), "rd:TRICE float %g (%%g)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(15902), "rd:TRICE float %E (%%E)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(15903), "rd:TRICE float %F (%%F)\n", aFloat(-x) );
        break; case __LINE__: TRICE( ID(15904), "rd:TRICE float %G (%%G)\n", aFloat(-x) );
        break; case __LINE__: TRICE64( ID(15905), "rd:TRICE64 double %e (%%e), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64( ID(15906), "rd:TRICE64 double %f (%%f), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64( ID(15907), "rd:TRICE64 double %g (%%g), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64( ID(15908), "rd:TRICE64 double %E (%%E), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64( ID(15909), "rd:TRICE64 double %F (%%F), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64( ID(15910), "rd:TRICE64 double %G (%%G), aDouble(y)\n", aDouble(-y) );

        break; case __LINE__: TRICE  ( ID(15911), "att:formatted float & double\n" );
        break; case __LINE__: TRICE32( ID(15912), "rd:TRICE32 float %12.6e (%%12.6e)\n", aFloat(x) );
        break; case __LINE__: TRICE32( ID(15913), "rd:TRICE32 float %12.6f (%%12.6f)\n", aFloat(x) );
        break; case __LINE__: TRICE32( ID(15914), "rd:TRICE32 float %12.6g (%%12.6g)\n", aFloat(x) );
        break; case __LINE__: TRICE32( ID(15915), "rd:TRICE32 float %12.6E (%%12.6E)\n", aFloat(x) );
        break; case __LINE__: TRICE32( ID(15916), "rd:TRICE32 float %12.6F (%%12.6F)\n", aFloat(x) );
        break; case __LINE__: TRICE32( ID(15917), "rd:TRICE32 float %12.6G (%%12.6G)\n", aFloat(x) );
        break; case __LINE__: TRICE64( ID(15918), "rd:TRICE64 double %12.6e (%%12.6e), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64( ID(15919), "rd:TRICE64 double %12.6f (%%12.6f), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64( ID(15920), "rd:TRICE64 double %12.6g (%%12.6g), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64( ID(15921), "rd:TRICE64 double %12.6E (%%12.6E), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64( ID(15922), "rd:TRICE64 double %12.6F (%%12.6F), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64( ID(15923), "rd:TRICE64 double %12.6G (%%12.6G), aDouble(y)\n", aDouble(y) );

        break; case __LINE__: TRICE ( ID(15924), "att:Various single arguments\n" );
        break; case __LINE__: TRICE8( ID(15925), "rd:TRICE8 line %t (%%t ,0)\n", 0 );
        break; case __LINE__: TRICE8( ID(15926), "rd:TRICE8 line %t (%%t ,2)\n", 2 );
        break; case __LINE__: TRICE8( ID(15927), "rd:TRICE8 line %u (%%u ,-1)\n", -1 );
        break; case __LINE__: TRICE8( ID(15928), "rd:TRICE8 line %b (%%b ,-1)\n", -1 );
        break; case __LINE__: TRICE8( ID(15929), "rd:TRICE8 line %o (%%o ,-1)\n", -1 );
        break; case __LINE__: TRICE8( ID(15930), "rd:TRICE8 line %O (%%O ,-1)\n", -1 );
        break; case __LINE__: TRICE8( ID(15931), "rd:TRICE8 line %X (%%X ,-1)\n", -1 );
        break; case __LINE__: TRICE8( ID(15932), "rd:TRICE8 line %x (%%x ,-1)\n", -1 );
        break; case __LINE__: TRICE8( ID(15933), "rd:TRICE8 line %d (%%d ,-1)\n", -1 );
        break; case __LINE__: TRICE16_1( ID(15934), "rd:TRICE16_1 line %t (%%t ,0)\n", 0 );
        break; case __LINE__: TRICE16_1( ID(15935), "rd:TRICE16_1 line %t (%%t ,2)\n", 2 );
        break; case __LINE__: TRICE16_1( ID(15936), "rd:TRICE16_1 line %u (%%u -1)\n", -1 );
        break; case __LINE__: TRICE16_1( ID(15937), "rd:TRICE16_1 line %b (%%b -1)\n", -1 );
        break; case __LINE__: TRICE16_1( ID(15938), "rd:TRICE16_1 line %o (%%o -1)\n", -1 );
        break; case __LINE__: TRICE16_1( ID(15939), "rd:TRICE16_1 line %x (%%x -1)\n", -1 );
        break; case __LINE__: TRICE16_1( ID(15940), "rd:TRICE16_1 line %d (%%d -1)\n", -1 );
        break; case __LINE__: TRICE( ID(15941), "rd:TRICE line %t (%%t -1)\n", 0 );
        break; case __LINE__: TRICE( ID(15942), "rd:TRICE line %t (%%t -1)\n", 2 );
        break; case __LINE__: TRICE( ID(15943), "rd:TRICE line %u (%%u)\n", -1 );
        break; case __LINE__: TRICE( ID(15944), "rd:TRICE line %b (%%b)\n", -2 );
        break; case __LINE__: TRICE( ID(15945), "rd:TRICE line %o (%%o)\n", -3 );
        break; case __LINE__: TRICE( ID(15946), "rd:TRICE line %x (%%x)\n", -4 );
        break; case __LINE__: TRICE( ID(15947), "rd:TRICE line %d (%%d)\n", -5 );

        break; case __LINE__: TRICE_N( ID(15948), "msg:%s\n", A, 0 );
        break; case __LINE__: TRICE_N( ID(15949), "msg:%s\n", A, 1 );
        break; case __LINE__: TRICE_N( ID(15950), "msg:%s\n", A, 2 );
        break; case __LINE__: TRICE_N( ID(15951), "msg:%s\n", A, 3 );
        break; case __LINE__: TRICE_N( ID(15952), "msg:%s\n", A, 4 );
        break; case __LINE__: TRICE_N( ID(15953), "msg:%s\n", A, 5 );
        break; case __LINE__: TRICE_N( ID(15954), "msg:%s\n", A, 6 );
        break; case __LINE__: TRICE_N( ID(15955), "msg:%s\n", A, 7 );
        break; case __LINE__: TRICE_N( ID(15956), "msg:%s\n", A, 8 );
        break; case __LINE__: TRICE_N( ID(15957), "msg:%s\n", A, 9 );
        break; case __LINE__: TRICE_N( ID(15958), "msg:%s\n", A, 10 );
        break; case __LINE__: TRICE_N( ID(15959), "msg:%s\n", A, 11 );
        break; case __LINE__: TRICE_N( ID(15960), "msg:%s\n", A, 12 );
        break; case __LINE__: TRICE_N( ID(15961), "msg:%s\n", A, 13 );
        break; case __LINE__: TRICE_N( ID(15962), "msg:%s\n", A, 14 );
        break; case __LINE__: TRICE_N( ID(15963), "msg:%s\n", A, 15 );
        break; case __LINE__: TRICE_N( ID(15964), "msg:%s\n", A, 16 );
        break; case __LINE__: TRICE_N( ID(15965), "msg:%s\n", A, 17 );
        break; case __LINE__: TRICE_N( ID(15966), "msg:%s\n", A, 18 );
        break; case __LINE__: TRICE_N( ID(15967), "msg:%s\n", A, 19 );
        break; case __LINE__: TRICE_N( ID(15968), "msg:%s\n", A, 120 );
        break; case __LINE__: TRICE_N( ID(15969), "msg:%s\n", A, 121 );
        break; case __LINE__: TRICE_N( ID(15970), "msg:%s\n", A, 122 );
        break; case __LINE__: TRICE_N( ID(15971), "msg:%s\n", A, 123 );
        break; case __LINE__: TRICE_N( ID(15972), "msg:%s\n", A, 124 );
        break; case __LINE__: TRICE_N( ID(15973), "msg:%s\n", A, 125 );
        break; case __LINE__: TRICE_N( ID(15974), "msg:%s\n", A, 126 );
        break; case __LINE__: TRICE_N( ID(15975), "msg:%s\n", A, 127 );
        break; case __LINE__: TRICE_N( ID(15976), "msg:%s\n", A, 128 );
        break; case __LINE__: TRICE_N( ID(15977), "msg:%s\n", A, 129 );


        break; case __LINE__: TRICE( ID(15978), "sig:TRICE with 1 to 12 values\n" );

        break; case __LINE__: TRICE( ID(15979), "rd:TRICE %d\n", -1 );                                                                                            //exp: time: 842,150_450default: rd:TRICE -1     
        break; case __LINE__: TRICE( ID(15980), "rd:TRICE %d, %d\n", -1, -2 );                                                                                    //exp: time: 842,150_450default: rd:TRICE -1, -2 
        break; case __LINE__: TRICE( ID(15981), "rd:TRICE %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time: 842,150_450default: rd:TRICE -1, -2, -3       
        break; case __LINE__: TRICE( ID(15982), "rd:TRICE %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: rd:TRICE -1, -2, -3, -4      
        break; case __LINE__: TRICE( ID(15983), "rd:TRICE %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time: 842,150_450default: rd:TRICE -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE( ID(15984), "rd:TRICE %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time: 842,150_450default: rd:TRICE -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE( ID(15985), "rd:TRICE %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time: 842,150_450default: rd:TRICE -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE( ID(15986), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time: 842,150_450default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE( ID(15987), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time: 842,150_450default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE( ID(15988), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time: 842,150_450default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE( ID(15989), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time: 842,150_450default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE( ID(15990), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time: 842,150_450default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: TRICE( ID(15991), "sig:TRICE_n with 1 to 12 values\n" );		
        break; case __LINE__: TRICE( ID(15992), "rd:TRICE_1  %d\n", -1 );                                                                                            //exp: time: 842,150_450default: rd:TRICE_1  -1     
        break; case __LINE__: TRICE( ID(15993), "rd:TRICE_2  %d, %d\n", -1, -2 );                                                                                    //exp: time: 842,150_450default: rd:TRICE_2  -1, -2 
        break; case __LINE__: TRICE( ID(15994), "rd:TRICE_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time: 842,150_450default: rd:TRICE_3  -1, -2, -3       
        break; case __LINE__: TRICE( ID(15995), "rd:TRICE_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time: 842,150_450default: rd:TRICE_4  -1, -2, -3, -4      
        break; case __LINE__: TRICE( ID(15996), "rd:TRICE_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time: 842,150_450default: rd:TRICE_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE( ID(15997), "rd:TRICE_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time: 842,150_450default: rd:TRICE_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE( ID(15998), "rd:TRICE_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time: 842,150_450default: rd:TRICE_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE( ID(15999), "rd:TRICE_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time: 842,150_450default: rd:TRICE_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE( ID(16000), "rd:TRICE_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time: 842,150_450default: rd:TRICE_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE( ID(16001), "rd:TRICE_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time: 842,150_450default: rd:TRICE_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE( ID(16002), "rd:TRICE_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time: 842,150_450default: rd:TRICE_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE( ID(16003), "rd:TRICE_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time: 842,150_450default: rd:TRICE_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    


        break; case __LINE__: TRICE  ( ID(16004), "sig:TRICE16 with 1 to 12 pointer\n" );
        break; case __LINE__: TRICE16( ID(16005), "rd:TRICE16 %p\n", -1 );
        break; case __LINE__: TRICE16( ID(16006), "rd:TRICE16 %p, %p\n", -1, -2 );
        break; case __LINE__: TRICE16( ID(16007), "rd:TRICE16 %p, %p, %p\n", -1, -2, -3 );
        break; case __LINE__: TRICE16( ID(16008), "rd:TRICE16 %p, %p, %p, %p\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE16( ID(16009), "rd:TRICE16 %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE16( ID(16010), "rd:TRICE16 %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE16( ID(16011), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE16( ID(16012), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE16( ID(16013), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE16( ID(16014), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE16( ID(16015), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE16( ID(16016), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE  ( ID(16017), "sig:TRICE16 with 1 to 12 hex\n" );
        break; case __LINE__: TRICE16( ID(16018), "rd:TRICE16 %X\n", -1 );
        break; case __LINE__: TRICE16( ID(16019), "rd:TRICE16 %X, %X\n", -1, -2 );
        break; case __LINE__: TRICE16( ID(16020), "rd:TRICE16 %X, %X, %X\n", -1, -2, -3 );
        break; case __LINE__: TRICE16( ID(16021), "rd:TRICE16 %X, %X, %X, %X\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE16( ID(16022), "rd:TRICE16 %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE16( ID(16023), "rd:TRICE16 %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE16( ID(16024), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE16( ID(16025), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE16( ID(16026), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE16( ID(16027), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE16( ID(16028), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE16( ID(16029), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );


    #endif // #if TRICE_CGO == 1
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
#define SCOPY( element ) do{ char* n = #element; int size = sizeof( src->element ); memcpy( p, &(src->element), size ); p += size; TRICE_S( ID(16030), "rd:sizeof(%8s)", n ); TRICE( ID(16031), " = %d\n", size);}while(0);

    //! DCOPY is a helper macro for struct deserialization.
#define DCOPY( element ) do{ char* n = #element; int size = sizeof( dst->element ); memcpy( &(dst->element), p, size ); p += size; TRICE_S( ID(16032), "rd:sizeof(%8s)", n ); TRICE( ID(16033), " = %d\n", size);}while(0);


typedef struct{
    float x;
    float y;
    uint8_t rgb[3];
    //float z; // it seems, that the compiler does not align this with -o3 & time !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
} Point_t; //!< Point_t is small struct type.

static int serializePoint( char* dst, const Point_t * src ){
    char * p = dst;

    SCOPY( x )
    SCOPY( y )
    SCOPY( rgb )

    return p - dst;
}

static int deserializePoint( Point_t * const dst, const char * src ){
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

static int serializeTryout( char* dst, const Tryout_t * src ){
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

static int deserializeTryout( Tryout_t * const dst, const char * src ){
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
    TRICE ( Id(16034), "inf: Tryout tx struct:" );
    TRICE8_B( Id(16035), " %02x ", &tx, sizeof(tx) );
    TRICE( Id(16036), "\n" );

    TRICE ( Id(16037), "inf: Tryout buffer:" );
    TRICE8_B( Id(16038), " %02x ", dst, len ); //lint !e670
    TRICE( Id(16039), "\n" );

    src = dst; // "data transfer"

    len = deserializeTryout( &rx, src );
    TRICE ( Id(16040), "inf: Tryout rx struct:" );
    TRICE8_B( Id(16041), " %02x ", &rx, sizeof(rx) );
    TRICE( Id(16042), "\n" );

    TRICE( Id(16043), "inf:sizeOf(Trypout) = %d, buffer length = %d\n", sizeof(tx), len );
    TRICE8_F( Id(16044), "info:TryoutStructFunction", &tx, sizeof(tx) );
    TRICE8_F( Id(16045), "info:TryoutBufferFunction", dst, len ); //lint !e670
}


static void exampleOfManualJSONencoding(void){
    typedef  struct {
    int Apple, Birn;
    float Fish;
    } Ex_t;
    Ex_t Ex = { -1, 2, (float)2.781 };
    TRICE( Id(16046), "att:MyStructEvaluationFunction(json:ExA{Apple:%d, Birn:%u, Fisch:%f}\n", Ex.Apple, Ex.Birn, aFloat(Ex.Fish) );
}

static void dynString( int n ) {
    char * s = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.,0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.,0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.,0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.,0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.,";
    const size_t l = strlen(s);
    n = n < l ? n : l;
    // trice( iD(16047), "sig:%3d:", n ); - this gets overwritten in CGO_Test case, so we avoid it to keep testing simple.
    TRICE_N( id(16048), "wr:%s\n", s, n );
}        
