/*! \file triceCheck.c
\brief trices for tool evaluation
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/
#if 1 // !TRICE_OFF - We do not exclude this file globally for TRICE_OFF == 1, to check if TRICE_OFF works well.
#include <float.h>
#include "trice.h"

// The strings behind "//exp:" are the expected result for each line (-color=none)

#ifndef TRICE_CHECK_MIN
#if !TRICE_OFF

static int32_t FloatToInt32(float f);
static int64_t DoubleToInt64(double f);

#endif

static void exampleOfManualSerialization(void);
static void exampleOfManualJSONencoding(void);
static void dynString(int n);
#endif // #ifndef TRICE_CHECK_MIN

// clang-format off

//! TriceCheck performs trice code sequence n. TriceCheck writes out all types of trices with fixed values for testing
//! \details One trice has one subtrace, if param size max 2 bytes. 
//! Traces with more bytes as parameter consist of several subtraces.
//! Each trice line needs to have a commented // "string" with its expected output.
//! The ID values must be in file triceCheck.c, because it is compiled first and trice update runs later.
// This function is also called from Go for tests.
void TriceCheck(int n) {
#ifdef TRICE_CHECK_MIN
    TRice(iD(13014), "\rTriceCheck %4d", n);
#else // #ifdef TRICE_CHECK_MIN
    char* sABCDE = "abcde 12345";
    uint32_t lenABCDE = strlen(sABCDE);
    float  x = (float)1089.6082763671875; // 0x44883377
    double y = 518.0547492508867; // 0x4080307020601050
#if TRICE_OFF
    TRICE_UNUSED(x)
    TRICE_UNUSED(y)
#endif
#if TRICE_CGO == 1 || defined(TRICE_FULL_CHECK)
    char* A = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
#endif
    char* s = "AAAAAAAAAAAA";
    char * five = "five";
    static int8_t b8[24] = { 0, -1, -2, 0x33, 4, 5, 6, 7, 8, 9, 10, 11, 0, -1, -2, 0x33, 4, 5, 6, 7, 8, 9, 10, 11 };
    static int16_t b16[] = { 0, -1, -2, 0x3344 };
    static int32_t b32[] = { 0, -1, -2, 0x33445555};
    static int64_t b64[4] = { 0, -1, -2, 0x3344555566666666 };
    unsigned anchorLine = __LINE__ + 20; // The offset value here must point to the first test line containing `anchorLine`.

    switch( n ){ 
        default:
        break; case __LINE__: trice16(iD(13015), "att: line %u\n", __LINE__ );
        // Lines ending with a comment starting with `//exp:` are executed during trhe automatic Go tests.
        break; case __LINE__: TRice(iD(13016), "Hello World!\n" );                                              //exp: time:feed3322default: Hello World!		
        // normal use cases
        break; case __LINE__: TRice(iD(13017), "info:This is a message without values and a 32-bit stamp.\n" ); //exp: time:feed3322default: info:This is a message without values and a 32-bit stamp.
        break; case __LINE__: Trice(iD(13018), "info:This is a message without values and a 16-bit stamp.\n" ); //exp: time:    be16default: info:This is a message without values and a 16-bit stamp.
        break; case __LINE__: trice(iD(13019), "info:This is a message without values and without stamp.\n"  ); //exp: time:        default: info:This is a message without values and without stamp.
        break; case __LINE__: for( int i = 126; i < 130; i++ ){ dynString(i); }








        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:01
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:012
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:01234
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:012345
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:01234567
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:012345678
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789a
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789ab
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789abc
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789abcd
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789abcde
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789abcdef
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789abcdefg
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789abcdefgh
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789abcdefghi
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789abcdefghij
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789abcdefghijk
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789abcdefghijkl
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789abcdefghijklm
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789abcdefghijklmn
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789abcdefghijklmno
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789abcdefghijklmnop
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789abcdefghijklmnopq
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789abcdefghijklmnopqr
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789abcdefghijklmnopqrs
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789abcdefghijklmnopqrst
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789abcdefghijklmnopqrstu
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789abcdefghijklmnopqrstuv
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789abcdefghijklmnopqrstuvw
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789abcdefghijklmnopqrstuvwx
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789abcdefghijklmnopqrstuvwxy
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789abcdefghijklmnopqrstuvwxyz
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789abcdefghijklmnopqrstuvwxyzA
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789abcdefghijklmnopqrstuvwxyzAB
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789abcdefghijklmnopqrstuvwxyzABC
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789abcdefghijklmnopqrstuvwxyzABCD
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789abcdefghijklmnopqrstuvwxyzABCDE
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789abcdefghijklmnopqrstuvwxyzABCDEF
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789abcdefghijklmnopqrstuvwxyzABCDEFG
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789abcdefghijklmnopqrstuvwxyzABCDEFGH
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHI
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJ
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJK
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKL
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLM
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMN
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNO
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOP
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQ
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQR
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRS
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRST
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTU
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUV
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVW
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWX
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXY
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.
        break; case __LINE__: dynString(__LINE__ - anchorLine); //exp: time:        default: wr:0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.,

        break; case __LINE__: trice(iD(13020), "sig:trice without stamp and with 0 to 12 values (most common use cases)\n" );    
        break; case __LINE__: trice(iD(13021), "rd:trice\n" );                                                                                                   //exp: time:        default: rd:trice
        break; case __LINE__: trice(iD(13022), "rd:trice %d\n", -1 );                                                                                            //exp: time:        default: rd:trice -1     
        break; case __LINE__: trice(iD(13023), "rd:trice %d, %d\n", -1, -2 );                                                                                    //exp: time:        default: rd:trice -1, -2 
        break; case __LINE__: trice(iD(13024), "rd:trice %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: rd:trice -1, -2, -3       
        break; case __LINE__: trice(iD(13025), "rd:trice %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:        default: rd:trice -1, -2, -3, -4      
        break; case __LINE__: trice(iD(13026), "rd:trice %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:        default: rd:trice -1, -2, -3, -4, -5        
        break; case __LINE__: trice(iD(13027), "rd:trice %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: rd:trice -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice(iD(13028), "rd:trice %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:        default: rd:trice -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice(iD(13029), "rd:trice %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:        default: rd:trice -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice(iD(13030), "rd:trice %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: rd:trice -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice(iD(13031), "rd:trice %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:        default: rd:trice -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice(iD(13032), "rd:trice %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:        default: rd:trice -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice(iD(13033), "rd:trice %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: rd:trice -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: Trice(iD(13034), "sig:Trice with 16-bit stamp and with 0 to 12 values (common use cases)\n" );     
        break; case __LINE__: Trice(iD(13035), "rd:Trice\n" );                                                                                                   //exp: time:    be16default: rd:Trice
        break; case __LINE__: Trice(iD(13036), "rd:Trice %d\n", -1 );                                                                                            //exp: time:    be16default: rd:Trice -1     
        break; case __LINE__: Trice(iD(13037), "rd:Trice %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:Trice -1, -2 
        break; case __LINE__: Trice(iD(13038), "rd:Trice %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:Trice -1, -2, -3       
        break; case __LINE__: Trice(iD(13039), "rd:Trice %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:Trice -1, -2, -3, -4      
        break; case __LINE__: Trice(iD(13040), "rd:Trice %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:Trice -1, -2, -3, -4, -5        
        break; case __LINE__: Trice(iD(13041), "rd:Trice %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:Trice -1, -2, -3, -4, -5, -6        
        break; case __LINE__: Trice(iD(13042), "rd:Trice %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:Trice -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: Trice(iD(13043), "rd:Trice %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:Trice -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: Trice(iD(13044), "rd:Trice %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:Trice -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: Trice(iD(13045), "rd:Trice %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:Trice -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: Trice(iD(13046), "rd:Trice %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:Trice -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: Trice(iD(13047), "rd:Trice %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:Trice -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: TRice(iD(13048), "sig:TRice with 32-bit stamp and with 0 to 12 values (common use cases)\n" );
        break; case __LINE__: TRice(iD(13049), "rd:TRice\n" );                                                                                                   //exp: time:feed3322default: rd:TRice
        break; case __LINE__: TRice(iD(13050), "rd:TRice %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRice -1     
        break; case __LINE__: TRice(iD(13051), "rd:TRice %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRice -1, -2 
        break; case __LINE__: TRice(iD(13052), "rd:TRice %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRice -1, -2, -3       
        break; case __LINE__: TRice(iD(13053), "rd:TRice %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRice -1, -2, -3, -4      
        break; case __LINE__: TRice(iD(13054), "rd:TRice %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRice -1, -2, -3, -4, -5        
        break; case __LINE__: TRice(iD(13055), "rd:TRice %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRice -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRice(iD(13056), "rd:TRice %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRice -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRice(iD(13057), "rd:TRice %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRice -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRice(iD(13058), "rd:TRice %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRice -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRice(iD(13059), "rd:TRice %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRice -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRice(iD(13060), "rd:TRice %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRice -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRice(iD(13061), "rd:TRice %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRice -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: trice  (iD(13062), "sig:trice16 with 1 to 12 pointer\n" );
        break; case __LINE__: trice16(iD(13063), "rd:trice16 %p\n", -1 );                                                                                             //exp: time:        default: rd:trice16 ffff      
        break; case __LINE__: trice16(iD(13064), "rd:trice16 %p, %p\n", -1, -2 );                                                                                     //exp: time:        default: rd:trice16 ffff, fffe      
        break; case __LINE__: trice16(iD(13065), "rd:trice16 %p, %p, %p\n", -1, -2, -3 );                                                                             //exp: time:        default: rd:trice16 ffff, fffe, fffd          
        break; case __LINE__: trice16(iD(13066), "rd:trice16 %p, %p, %p, %p\n", -1, -2, -3, -4 );                                                                     //exp: time:        default: rd:trice16 ffff, fffe, fffd, fffc            
        break; case __LINE__: trice16(iD(13067), "rd:trice16 %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5 );                                                             //exp: time:        default: rd:trice16 ffff, fffe, fffd, fffc, fffb       
        break; case __LINE__: trice16(iD(13068), "rd:trice16 %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6 );                                                     //exp: time:        default: rd:trice16 ffff, fffe, fffd, fffc, fffb, fffa              
        break; case __LINE__: trice16(iD(13069), "rd:trice16 %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7);                                              //exp: time:        default: rd:trice16 ffff, fffe, fffd, fffc, fffb, fffa, fff9           
        break; case __LINE__: trice16(iD(13070), "rd:trice16 %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                     //exp: time:        default: rd:trice16 ffff, fffe, fffd, fffc, fffb, fffa, fff9, fff8               
        break; case __LINE__: trice16(iD(13071), "rd:trice16 %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                             //exp: time:        default: rd:trice16 ffff, fffe, fffd, fffc, fffb, fffa, fff9, fff8, fff7              
        break; case __LINE__: trice16(iD(13072), "rd:trice16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                     //exp: time:        default: rd:trice16 ffff, fffe, fffd, fffc, fffb, fffa, fff9, fff8, fff7, fff6                
        break; case __LINE__: trice16(iD(13073), "rd:trice16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );           //exp: time:        default: rd:trice16 ffff, fffe, fffd, fffc, fffb, fffa, fff9, fff8, fff7, fff6, fff5              
        break; case __LINE__: trice16(iD(13074), "rd:trice16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );  //exp: time:        default: rd:trice16 ffff, fffe, fffd, fffc, fffb, fffa, fff9, fff8, fff7, fff6, fff5, fff4
        break; case __LINE__: trice8 (iD(13075), "rd:trice8  %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );  //exp: time:        default: rd:trice8  ff, fe, fd, fc, fb, fa, f9, f8, f7, f6, f5, f4
        break; case __LINE__: trice32(iD(13076), "rd:trice32 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );  //exp: time:        default: rd:trice32 ffffffff, fffffffe, fffffffd, fffffffc, fffffffb, fffffffa, fffffff9, fffffff8, fffffff7, fffffff6, fffffff5, fffffff4

        break; case __LINE__: trice  (iD(13077), "sig:trice16 with 1 to 12 hex\n" );
        break; case __LINE__: trice16(iD(13078), "rd:trice16 %X\n", -1 );                                                                                             //exp: time:        default: rd:trice16 FFFF                 
        break; case __LINE__: trice16(iD(13079), "rd:trice16 %X, %X\n", -1, -2 );                                                                                     //exp: time:        default: rd:trice16 FFFF, FFFE             
        break; case __LINE__: trice16(iD(13080), "rd:trice16 %X, %X, %X\n", -1, -2, -3 );                                                                             //exp: time:        default: rd:trice16 FFFF, FFFE, FFFD            
        break; case __LINE__: trice16(iD(13081), "rd:trice16 %X, %X, %X, %X\n", -1, -2, -3, -4 );                                                                     //exp: time:        default: rd:trice16 FFFF, FFFE, FFFD, FFFC                  
        break; case __LINE__: trice16(iD(13082), "rd:trice16 %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5 );                                                             //exp: time:        default: rd:trice16 FFFF, FFFE, FFFD, FFFC, FFFB              
        break; case __LINE__: trice16(iD(13083), "rd:trice16 %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6 );                                                     //exp: time:        default: rd:trice16 FFFF, FFFE, FFFD, FFFC, FFFB, FFFA                     
        break; case __LINE__: trice16(iD(13084), "rd:trice16 %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7);                                              //exp: time:        default: rd:trice16 FFFF, FFFE, FFFD, FFFC, FFFB, FFFA, FFF9                
        break; case __LINE__: trice16(iD(13085), "rd:trice16 %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                     //exp: time:        default: rd:trice16 FFFF, FFFE, FFFD, FFFC, FFFB, FFFA, FFF9, FFF8             
        break; case __LINE__: trice16(iD(13086), "rd:trice16 %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                             //exp: time:        default: rd:trice16 FFFF, FFFE, FFFD, FFFC, FFFB, FFFA, FFF9, FFF8, FFF7          
        break; case __LINE__: trice16(iD(13087), "rd:trice16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                     //exp: time:        default: rd:trice16 FFFF, FFFE, FFFD, FFFC, FFFB, FFFA, FFF9, FFF8, FFF7, FFF6          
        break; case __LINE__: trice16(iD(13088), "rd:trice16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );           //exp: time:        default: rd:trice16 FFFF, FFFE, FFFD, FFFC, FFFB, FFFA, FFF9, FFF8, FFF7, FFF6, FFF5             
        break; case __LINE__: trice16(iD(13089), "rd:trice16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );  //exp: time:        default: rd:trice16 FFFF, FFFE, FFFD, FFFC, FFFB, FFFA, FFF9, FFF8, FFF7, FFF6, FFF5, FFF4
        break; case __LINE__: trice8 (iD(13090), "rd:trice8  %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );  //exp: time:        default: rd:trice8  FF, FE, FD, FC, FB, FA, F9, F8, F7, F6, F5, F4
        break; case __LINE__: trice32(iD(13091), "rd:trice32 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );  //exp: time:        default: rd:trice32 FFFFFFFF, FFFFFFFE, FFFFFFFD, FFFFFFFC, FFFFFFFB, FFFFFFFA, FFFFFFF9, FFFFFFF8, FFFFFFF7, FFFFFFF6, FFFFFFF5, FFFFFFF4

        break; case __LINE__: trice16(iD(13092), "att: line %u\n", __LINE__ );
    
        break; case __LINE__: 
        #if TRICE_DEFERRED_XTEA_ENCRYPT == 1 || TRICE_DIRECT_XTEA_ENCRYPT == 1
                trice(iD(13093), "--------------------------------------------------\n" );
                {
                    uint32_t by[8] = {1,2,3,4,5,6};
                    int count = (sizeof(by) + 7) & ~7; // only multiple of 8 encryptable
                    trice32(iD(13094), "msg: message = %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
                    trice32(iD(13095), "tim: pre TriceEncryption SysTick=%d\n", SYSTICKVAL );
                    XTEAEncrypt(by, count>>2);
                    trice32(iD(13096), "tim: post TriceEncryption SysTick=%d\n", SYSTICKVAL );
                    trice32(iD(13097), "att: TriceEncrypted =  %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] ); 
                    #if XTEA_DECRYPT == 1
                        trice16_1(iD(13098), "tim: pre TriceDecryption SysTick=%d\n", SYSTICKVAL );
                        XTEADecrypt(by, count>>2);
                        trice16_1(iD(13099), "tim: post TriceDecryption SysTick=%d\n", SYSTICKVAL );
                    #endif
                    trice32(iD(13100), "msg: messge = %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
                }
                trice(iD(13101), "--------------------------------------------------\n" );
        #endif

        break; case __LINE__: trice16(iD(13102), "att: line %u\n", __LINE__ );
        break; case __LINE__: trice(iD(13103), "dbg:Hi!\n" );                                                         //exp: time:        default: dbg:Hi!    
        break; case __LINE__: trice(iD(13104), "dbg:	Hi!\n" );                                                        //exp: time:        default: dbg:	Hi!
        break; case __LINE__: trice(iD(13105), "sig:Some time measurements\n" );
        break; case __LINE__: trice(iD(13106), "isr:trice isr message, SysTick is %6d\n", SYSTICKVAL);                //exp: time:        default: isr:trice isr message, SysTick is      0 
        break; case __LINE__: trice(iD(13107), "isr:trice isr message, SysTick is %6d\n", SYSTICKVAL);                //exp: time:        default: isr:trice isr message, SysTick is      0 
        break; case __LINE__: trice(iD(13108), "isr:trice isr message, SysTick is %6d\n", SYSTICKVAL);                //exp: time:        default: isr:trice isr message, SysTick is      0 
        break; case __LINE__: trice(iD(13109), "isr:trice isr message, SysTick is %6d\n", SYSTICKVAL);                //exp: time:        default: isr:trice isr message, SysTick is      0
        break; case __LINE__: trice_1(iD(13110), "isr:trice_1 isr message, SysTick is %6d\n", SYSTICKVAL);            //exp: time:        default: isr:trice_1 isr message, SysTick is      0 
        break; case __LINE__: trice_1(iD(13111), "isr:trice_1 isr message, SysTick is %6d\n", SYSTICKVAL);            //exp: time:        default: isr:trice_1 isr message, SysTick is      0 
        break; case __LINE__: trice_1(iD(13112), "isr:trice_1 isr message, SysTick is %6d\n", SYSTICKVAL);            //exp: time:        default: isr:trice_1 isr message, SysTick is      0 
        break; case __LINE__: trice_1(iD(13113), "isr:trice_1 isr message, SysTick is %6d\n", SYSTICKVAL);            //exp: time:        default: isr:trice_1 isr message, SysTick is      0
        break; case __LINE__: TRICE16_1(id(13114), "isr:TRICE16_1 isr message, SysTick is %6d\n", SYSTICKVAL); //exp: time:        default: isr:TRICE16_1 isr message, SysTick is      0 
        break; case __LINE__: TRICE16_1(id(13115), "isr:TRICE16_1 isr message, SysTick is %6d\n", SYSTICKVAL); //exp: time:        default: isr:TRICE16_1 isr message, SysTick is      0 
        break; case __LINE__: TRICE16_1(id(13116), "isr:TRICE16_1 isr message, SysTick is %6d\n", SYSTICKVAL); //exp: time:        default: isr:TRICE16_1 isr message, SysTick is      0 
        break; case __LINE__: TRICE16_1(id(13117), "isr:TRICE16_1 isr message, SysTick is %6d\n", SYSTICKVAL); //exp: time:        default: isr:TRICE16_1 isr message, SysTick is      0 
        break; case __LINE__: TRICE_1(id(13118), "isr:TRICE_1 isr message, SysTick is %6d\n", SYSTICKVAL);     //exp: time:        default: isr:TRICE_1 isr message, SysTick is      0 
        break; case __LINE__: TRICE_1(id(13119), "isr:TRICE_1 isr message, SysTick is %6d\n", SYSTICKVAL);     //exp: time:        default: isr:TRICE_1 isr message, SysTick is      0 
        break; case __LINE__: TRICE_1(id(13120), "isr:TRICE_1 isr message, SysTick is %6d\n", SYSTICKVAL);     //exp: time:        default: isr:TRICE_1 isr message, SysTick is      0 
        break; case __LINE__: TRICE_1(id(13121), "isr:TRICE_1 isr message, SysTick is %6d\n", SYSTICKVAL);     //exp: time:        default: isr:TRICE_1 isr message, SysTick is      0 
        break; case __LINE__: TRICE(id(13122), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);         //exp: time:        default: isr:TRICE isr message, SysTick is      0 
        break; case __LINE__: TRICE(id(13123), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);         //exp: time:        default: isr:TRICE isr message, SysTick is      0 
        break; case __LINE__: TRICE(id(13124), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);         //exp: time:        default: isr:TRICE isr message, SysTick is      0 
        break; case __LINE__: TRICE(id(13125), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);         //exp: time:        default: isr:TRICE isr message, SysTick is      0 

        break; case __LINE__: trice(iD(13126), "tim:trice START time message\n");                                     //exp: time:        default: tim:trice START time message
        break; case __LINE__: trice(iD(13127), "tim:trice STOP time message\n");                                      //exp: time:        default: tim:trice STOP time message
        break; case __LINE__: trice(iD(13128), "tim:trice START time message\n");                                     //exp: time:        default: tim:trice START time message
        break; case __LINE__: trice(iD(13129), "tim:trice STOP time message\n");                                      //exp: time:        default: tim:trice STOP time message 
        break; case __LINE__: trice(iD(13130), "tim:trice START time message\n");                                     //exp: time:        default: tim:trice START time message
        break; case __LINE__: trice64(iD(13131), "rd:trice64 %d, %d\n", 1, 2 );                                       //exp: time:        default: rd:trice64 1, 2   
        break; case __LINE__: trice(iD(13132), "tim:trice STOP time message\n");                                      //exp: time:        default: tim:trice STOP time message
        break; case __LINE__: TRICE(id(13133), "tim:TRICE START time message\n");                              //exp: time:        default: tim:TRICE START time message
        break; case __LINE__: TRICE(id(13134), "tim:TRICE STOP time message\n");                               //exp: time:        default: tim:TRICE STOP time message 
        break; case __LINE__: TRICE(id(13135), "tim:TRICE START time message\n");                              //exp: time:        default: tim:TRICE START time message
        break; case __LINE__: TRICE(id(13136), "tim:TRICE STOP time message\n");                               //exp: time:        default: tim:TRICE STOP time message 
        break; case __LINE__: TRICE(id(13137), "tim:TRICE START time message\n");                              //exp: time:        default: tim:TRICE START time message
        break; case __LINE__: TRICE64(id(13138), "rd:TRICE64 %d, %d\n", 1, 2 );                                //exp: time:        default: rd:TRICE64 1, 2    
        break; case __LINE__: TRICE(id(13139), "tim:TRICE STOP time message\n");                               //exp: time:        default: tim:TRICE STOP time message 
        break; case __LINE__: trice  (iD(13140), "tim:trice --------------------------------------------------\n");   //exp: time:        default: tim:trice --------------------------------------------------
        break; case __LINE__: trice  (iD(13141), "tim:trice --------------------------------------------------\n");   //exp: time:        default: tim:trice --------------------------------------------------
        break; case __LINE__: trice64(iD(13142), "rd:trice64 %d, %d\n", 1, 2 );                                       //exp: time:        default: rd:trice64 1, 2
        break; case __LINE__: trice  (iD(13143), "tim:trice --------------------------------------------------\n");   //exp: time:        default: tim:trice --------------------------------------------------
        break; case __LINE__: trice  (iD(13144), "tim:trice --------------------------------------------------\n");   //exp: time:        default: tim:trice --------------------------------------------------
        break; case __LINE__: trice64(iD(13145), "rd:trice64 %d, %d\n", 1, 2 );                                       //exp: time:        default: rd:trice64 1, 2
        break; case __LINE__: trice32_1(iD(13146), "rd:trice32_1 %d\n", -1 );                                         //exp: time:        default: rd:trice32_1 -1
        break; case __LINE__: trice64_1(iD(13147), "rd:trice64_1 %d\n", -2 );                                         //exp: time:        default: rd:trice64_1 -2
        break; case __LINE__: trice32_2(iD(13148), "rd:trice32_2 %d,%d\n", -1, 2 );                                   //exp: time:        default: rd:trice32_2 -1,2
        break; case __LINE__: trice64_2(iD(13149), "rd:trice64_2 %d,%d\n", -1, 2 );                                   //exp: time:        default: rd:trice64_2 -1,2

        // special use cases
        break; case __LINE__: TRice64(iD(13150), "info:12 64-bit values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 32-bit stamp.\n", -3, -4, -5, -6, 0, aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), -8 ); //exp: time:feed3322default: info:12 64-bit values -3, 18446744073709551612, fffffffffffffffb, FFFFFFFFFFFFFFFA, false, -7.123457e+00, -7.123457, -7.123456789, -7.123457E+00, -7.123457, -7.123456789, 0xb1111111111111111111111111111111111111111111111111111111111111000 and a 32-bit stamp.
        break; case __LINE__: Trice64(iD(13151), "info:12 64-bit values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 16-bit stamp.\n", -3, -4, -5, -6, 0, aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), -8 ); //exp: time:    be16default: info:12 64-bit values -3, 18446744073709551612, fffffffffffffffb, FFFFFFFFFFFFFFFA, false, -7.123457e+00, -7.123457, -7.123456789, -7.123457E+00, -7.123457, -7.123456789, 0xb1111111111111111111111111111111111111111111111111111111111111000 and a 16-bit stamp.
        break; case __LINE__: trice64(iD(13152), "info:12 64-bit values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and without stamp.\n" , -3, -4, -5, -6, 0, aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), -8 ); //exp: time:        default: info:12 64-bit values -3, 18446744073709551612, fffffffffffffffb, FFFFFFFFFFFFFFFA, false, -7.123457e+00, -7.123457, -7.123456789, -7.123457E+00, -7.123457, -7.123456789, 0xb1111111111111111111111111111111111111111111111111111111111111000 and without stamp.

        break; case __LINE__: TRice32(iD(13153), "info:12 32-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 32-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: info:12 32-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 32-bit stamp.
        break; case __LINE__: Trice32(iD(13154), "info:12 32-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 16-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: info:12 32-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 16-bit stamp.
        break; case __LINE__: trice32(iD(13155), "info:12 32-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and without stamp.\n" , -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: info:12 32-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and without stamp.
        break; case __LINE__: TRice16(iD(13156), "info:12 16-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 32-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: info:12 16-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 32-bit stamp.
        break; case __LINE__: Trice16(iD(13157), "info:12 16-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 16-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: info:12 16-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 16-bit stamp.
        break; case __LINE__: trice16(iD(13158), "info:12 16-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and without stamp.\n" , -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: info:12 16-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and without stamp.
        break; case __LINE__: TRice8 (iD(13159), "info:12  8-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 32-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: info:12  8-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 32-bit stamp.
        break; case __LINE__: Trice8 (iD(13160), "info:12  8-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 16-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: info:12  8-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 16-bit stamp.
        break; case __LINE__: trice8 (iD(13161), "info:12  8-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and without stamp.\n" , -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: info:12  8-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and without stamp.

        break; case __LINE__: trice16(iD(13162), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRice16(iD(13163), "att: 0x8888 == %04xh\n", 0x8888 ); //exp: time:feed3322default: att: 0x8888 == 8888h
        break; case __LINE__: Trice16(iD(13164), "att: 0x8888 == %04xh\n", 0x8888 ); //exp: time:    be16default: att: 0x8888 == 8888h
        break; case __LINE__: trice16(iD(13165), "att: 0x8888 == %04xh\n", 0x8888 ); //exp: time:        default: att: 0x8888 == 8888h


        break; case __LINE__: trice16(iD(13166), "att: line %u\n", __LINE__ );
        break; case __LINE__: trice8   (iD(13167), "value=%x\n", 0x55 );             //exp: time:        default: value=55
        break; case __LINE__: trice8   (iD(13168), "value=%x\n", 0x88 );             //exp: time:        default: value=88

        break; case __LINE__: trice(iD(13169), "att:mixed int & float & double & bits\n" );
        break; case __LINE__: TRice32(iD(13170), "rd:TRice32 int %d, float %f (%%f), %08x, %032b\n",       FloatToInt32(x),   aFloat(x),   aFloat(x),   aFloat(x) ); //exp: time:feed3322default: rd:TRice32 int 1089, float 1089.608276 (%f), 44883377, 01000100100010000011001101110111                                         
        break; case __LINE__: TRice32(iD(13171), "rd:TRice32 int %d, float %f (%%f), %08x, %032b\n",          (int32_t)(x),   aFloat(x),   aFloat(x),   aFloat(x) ); //exp: time:feed3322default: rd:TRice32 int 1089, float 1089.608276 (%f), 44883377, 01000100100010000011001101110111                                         
        break; case __LINE__: TRice32(iD(13172), "rd:TRice32 int %x, float %f (%%f), %08x, %032b\n",            0x44883377,   aFloat(x),   aFloat(x),   aFloat(x) ); //exp: time:feed3322default: rd:TRice32 int 44883377, float 1089.608276 (%f), 44883377, 01000100100010000011001101110111                                         
        break; case __LINE__: TRice32(iD(13173), "rd:TRice32 int %d, float %f (%%f), %08x, %032b\n",                     x,   aFloat(x),   aFloat(x),   aFloat(x) ); //exp: time:feed3322default: rd:TRice32 int 1089, float 1089.608276 (%f), 44883377, 01000100100010000011001101110111                                         
        break; case __LINE__: TRice32(iD(13174), "rd:TRice32 int %d, float %f (%%f), %08x, %032b\n",      FloatToInt32(-x),  aFloat(-x),  aFloat(-x),  aFloat(-x) ); //exp: time:feed3322default: rd:TRice32 int -1089, float -1089.608276 (%f), c4883377, 11000100100010000011001101110111                                       
        break; case __LINE__: TRice32(iD(13175), "rd:TRice32 int %d, float %f (%%f), %08x, %032b\n",         (int32_t)(-x),  aFloat(-x),  aFloat(-x),  aFloat(-x) ); //exp: time:feed3322default: rd:TRice32 int -1089, float -1089.608276 (%f), c4883377, 11000100100010000011001101110111                                       
        break; case __LINE__: TRice32(iD(13176), "rd:TRice32 int %X, float %f (%%f), %08x, %032b\n",           -0x44883377,  aFloat(-x),  aFloat(-x),  aFloat(-x) ); //exp: time:feed3322default: rd:TRice32 int BB77CC89, float -1089.608276 (%f), c4883377, 11000100100010000011001101110111                                        
        break; case __LINE__: TRice64(iD(13177), "rd:TRice64 int %d, double %f (%%f), %016x, %064b\n",    DoubleToInt64(y),  aDouble(y),  aDouble(y),  aDouble(y) ); //exp: time:feed3322default: rd:TRice64 int 518, double 518.054749 (%f), 4080307020601050, 0100000010000000001100000111000000100000011000000001000001010000   
        break; case __LINE__: TRice64(iD(13178), "rd:TRice64 int %d, double %f (%%f), %016x, %064b\n",        (int64_t)(y),  aDouble(y),  aDouble(y),  aDouble(y) ); //exp: time:feed3322default: rd:TRice64 int 518, double 518.054749 (%f), 4080307020601050, 0100000010000000001100000111000000100000011000000001000001010000   
        break; case __LINE__: TRice64(iD(13179), "rd:TRice64 int %d, double %f (%%f), %016x, %064b\n",                   y,  aDouble(y),  aDouble(y),  aDouble(y) ); //exp: time:feed3322default: rd:TRice64 int 518, double 518.054749 (%f), 4080307020601050, 0100000010000000001100000111000000100000011000000001000001010000   
        break; case __LINE__: TRice64(iD(13180), "rd:TRice64 int %d, double %f (%%f), %016x, %064b\n",   DoubleToInt64(-y), aDouble(-y), aDouble(-y), aDouble(-y) ); //exp: time:feed3322default: rd:TRice64 int -518, double -518.054749 (%f), c080307020601050, 1100000010000000001100000111000000100000011000000001000001010000
        break; case __LINE__: TRice64(iD(13181), "rd:TRice64 int %d, double %f (%%f), %016x, %064b\n",       (int64_t)(-y), aDouble(-y), aDouble(-y), aDouble(-y) ); //exp: time:feed3322default: rd:TRice64 int -518, double -518.054749 (%f), c080307020601050, 1100000010000000001100000111000000100000011000000001000001010000
        break; case __LINE__: TRice64(iD(13182), "rd:TRice64 int %X, double %f (%%f), %016x, %064b\n", -0x4080307020601050, aDouble(-y), aDouble(-y), aDouble(-y) ); //exp: time:feed3322default: rd:TRice64 int BF7FCF8FDF9FEFB0, double -518.054749 (%f), c080307020601050, 1100000010000000001100000111000000100000011000000001000001010000 

        break; case __LINE__: trice16(iD(13183), "att: line %u\n", __LINE__ );
        break; case __LINE__: triceS(iD(13184), "msg:With triceS:%s\n", sABCDE );                      //exp: time:        default: msg:With triceS:abcde 12345
        break; case __LINE__: triceN(iD(13185), "sig:With triceN:%s\n", sABCDE, lenABCDE );            //exp: time:        default: sig:With triceN:abcde 12345
        break; case __LINE__: TriceS(iD(13186), "msg:With TriceS:%s\n", sABCDE );                      //exp: time:    be16default: msg:With TriceS:abcde 12345
        break; case __LINE__: TriceN(iD(13187), "sig:With TriceN:%s\n", sABCDE, lenABCDE );            //exp: time:    be16default: sig:With TriceN:abcde 12345
        break; case __LINE__: TRiceS(iD(13188), "msg:With TRiceS:%s\n", sABCDE );                      //exp: time:feed3322default: msg:With TRiceS:abcde 12345
        break; case __LINE__: TRiceN(iD(13189), "sig:With TRiceN:%s\n", sABCDE, lenABCDE );            //exp: time:feed3322default: sig:With TRiceN:abcde 12345
        break; case __LINE__: TRICE_S(id(13190), "msg:With TRICE_S:%s\n", sABCDE );             //exp: time:        default: msg:With TRICE_S:abcde 12345
        break; case __LINE__: TRICE_N(id(13191), "sig:With TRICE_N:%s\n", sABCDE, lenABCDE );   //exp: time:        default: sig:With TRICE_N:abcde 12345
        break; case __LINE__: TRICE_S(Id(13192), "msg:With TRICE_S:%s\n", sABCDE );             //exp: time:    be16default: msg:With TRICE_S:abcde 12345
        break; case __LINE__: TRICE_N(Id(13193), "sig:With TRICE_N:%s\n", sABCDE, lenABCDE );   //exp: time:    be16default: sig:With TRICE_N:abcde 12345
        break; case __LINE__: TRICE_S(ID(13194), "msg:With TRICE_S:%s\n", sABCDE );             //exp: time:feed3322default: msg:With TRICE_S:abcde 12345
        break; case __LINE__: TRICE_N(ID(13195), "sig:With TRICE_N:%s\n", sABCDE, lenABCDE );   //exp: time:feed3322default: sig:With TRICE_N:abcde 12345

        break; case __LINE__: TRICE8_B(ID(13196), "  %02x", sABCDE, lenABCDE );                 //exp: time:feed3322default:   61  62  63  64  65  20  31  32  33  34  35
        break; case __LINE__: trice(iD(13197), "\n" );
        break; case __LINE__: TRICE8_B(ID(13198), "BUF: %02x\n", sABCDE, lenABCDE );            //exp: time:feed3322default: BUF: 61 62 63 64 65 20 31 32 33 34 35
        break; case __LINE__: TRICE8_B(ID(13199), "%4d", sABCDE, lenABCDE );                    //exp: time:feed3322default:   97  98  99 100 101  32  49  50  51  52  53
        break; case __LINE__: trice(iD(13200), "\n" );
        break; case __LINE__: TRICE8_B(ID(13201), "  %02x", b8, sizeof(b8)/sizeof(int8_t) );    //exp: time:feed3322default:   00  ff  fe  33  04  05  06  07  08  09  0a  0b  00  ff  fe  33  04  05  06  07  08  09  0a  0b
        break; case __LINE__: trice(iD(13202), "\n" );

        break; case __LINE__: TRice8B(iD(13203), " %02x", b8,  sizeof(b8) /sizeof(int8_t) );    //exp: time:feed3322default:  00 ff fe 33 04 05 06 07 08 09 0a 0b 00 ff fe 33 04 05 06 07 08 09 0a 0b
        break; case __LINE__: trice(iD(13204), "\n" );
        break; case __LINE__: Trice8B(iD(13205), " %02x", b8,  sizeof(b8) /sizeof(int8_t) );    //exp: time:    be16default:  00 ff fe 33 04 05 06 07 08 09 0a 0b 00 ff fe 33 04 05 06 07 08 09 0a 0b
        break; case __LINE__: trice(iD(13206), "\n" );
        break; case __LINE__: trice8B(iD(13207), " %02x", b8,  sizeof(b8) /sizeof(int8_t) );    //exp: time:        default:  00 ff fe 33 04 05 06 07 08 09 0a 0b 00 ff fe 33 04 05 06 07 08 09 0a 0b
        break; case __LINE__: trice(iD(13208), "\n" );

        break; case __LINE__: trice8B(iD(13209), "att: %02x\n", b8,  sizeof(b8) /sizeof(int8_t) );  //exp: time:        default: att: 00 ff fe 33 04 05 06 07 08 09 0a 0b 00 ff fe 33 04 05 06 07 08 09 0a 0b
        break; case __LINE__: trice8B(iD(13210), "rd: %02x", b8,  sizeof(b8) /sizeof(int8_t) );    //exp: time:        default: rd: 00 ff fe 33 04 05 06 07 08 09 0a 0b 00 ff fe 33 04 05 06 07 08 09 0a 0b
        break; case __LINE__: trice(iD(13211), "\n" );
        break; case __LINE__: trice8B(iD(13212), " %02x\n", b8,  sizeof(b8) /sizeof(int8_t) );      //exp: time:        default:  00 ff fe 33 04 05 06 07 08 09 0a 0b 00 ff fe 33 04 05 06 07 08 09 0a 0b

        break; case __LINE__: TRICE16_B(ID(13213), "  %04x", b16, sizeof(b16)/sizeof(int16_t) ); //exp: time:feed3322default:   0000  ffff  fffe  3344
        break; case __LINE__: trice(iD(13214), "\n" );
        break; case __LINE__: TRice16B(iD(13215), " %04x", b16, sizeof(b16) /sizeof(int16_t) );  //exp: time:feed3322default:  0000 ffff fffe 3344
        break; case __LINE__: trice(iD(13216), "\n" );
        break; case __LINE__: Trice16B(iD(13217), " %04x", b16, sizeof(b16) /sizeof(int16_t) );  //exp: time:    be16default:  0000 ffff fffe 3344
        break; case __LINE__: trice(iD(13218), "\n" );
        break; case __LINE__: trice16B(iD(13219), " %04x", b16, sizeof(b16) /sizeof(int16_t) );  //exp: time:        default:  0000 ffff fffe 3344
        break; case __LINE__: trice(iD(13220), "\n" );
        break; case __LINE__: trice16B(iD(13221), "msg: %04x\n", b16, sizeof(b16) /sizeof(int16_t) );  //exp: time:        default: msg: 0000 ffff fffe 3344

        break; case __LINE__: TRICE32_B(ID(13222), " %08x", b32, sizeof(b32)/sizeof(int32_t) );  //exp: time:feed3322default:  00000000 ffffffff fffffffe 33445555
        break; case __LINE__: trice(iD(13223), "\n" );
        break; case __LINE__: TRice32B(iD(13224), " %08x", b32, sizeof(b32) /sizeof(int32_t)  ); //exp: time:feed3322default:  00000000 ffffffff fffffffe 33445555
        break; case __LINE__: trice(iD(13225), "\n" );
        break; case __LINE__: Trice32B(iD(13226), " %08x", b32, sizeof(b32) /sizeof(int32_t)  ); //exp: time:    be16default:  00000000 ffffffff fffffffe 33445555
        break; case __LINE__: trice(iD(13227), "\n" );
        break; case __LINE__: trice32B(iD(13228), " %08x", b32, sizeof(b32) /sizeof(int32_t)  ); //exp: time:        default:  00000000 ffffffff fffffffe 33445555
        break; case __LINE__: trice(iD(13229), "\n" );
        break; case __LINE__: trice32B(iD(13230), "att: %08x\n", b32, sizeof(b32) /sizeof(int32_t)  ); //exp: time:        default: att: 00000000 ffffffff fffffffe 33445555

        break; case __LINE__: TRICE64_B(ID(13231), " %016x", b64, sizeof(b64)/sizeof(int64_t) );  //exp: time:feed3322default:  0000000000000000 ffffffffffffffff fffffffffffffffe 3344555566666666
        break; case __LINE__: trice(iD(13232), "\n" );
        break; case __LINE__: TRice64B(iD(13233), " %016x", b64, sizeof(b64) /sizeof(int64_t)  ); //exp: time:feed3322default:  0000000000000000 ffffffffffffffff fffffffffffffffe 3344555566666666
        break; case __LINE__: trice(iD(13234), "\n" );
        break; case __LINE__: Trice64B(iD(13235), " %016x", b64, sizeof(b64) /sizeof(int64_t)  ); //exp: time:    be16default:  0000000000000000 ffffffffffffffff fffffffffffffffe 3344555566666666
        break; case __LINE__: trice(iD(13236), "\n" );
        break; case __LINE__: trice64B(iD(13237), " %016x", b64, sizeof(b64) /sizeof(int64_t)  ); //exp: time:        default:  0000000000000000 ffffffffffffffff fffffffffffffffe 3344555566666666
        break; case __LINE__: trice(iD(13238), "\n" );
        break; case __LINE__: trice64B(iD(13239), "SIG: %016x\n", b64, sizeof(b64) /sizeof(int64_t)  ); //exp: time:        default: SIG: 0000000000000000 ffffffffffffffff fffffffffffffffe 3344555566666666

        break; case __LINE__: TRICE8_F(ID(13240), "info:FunctionNameWa", b8,  sizeof(b8) /sizeof(int8_t) );    //exp: time:feed3322default: info:FunctionNameWa(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)
        break; case __LINE__: TRice8F(iD(13241), "call:FunctionNameWb", b8,  sizeof(b8) /sizeof(int8_t) );            //exp: time:feed3322default: call:FunctionNameWb(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)
        break; case __LINE__: Trice8F(iD(13242), "call:FunctionNameWc", b8,  sizeof(b8) /sizeof(int8_t) );            //exp: time:    be16default: call:FunctionNameWc(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)
        break; case __LINE__: trice8F(iD(13243), "call:FunctionNameWd", b8,  sizeof(b8) /sizeof(int8_t) );            //exp: time:        default: call:FunctionNameWd(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)
        break; case __LINE__: TRICE16_F(ID(13244), "info:FunctionNameXa", b16, sizeof(b16)/sizeof(int16_t) );  //exp: time:feed3322default: info:FunctionNameXa(0000)(ffff)(fffe)(3344) 
        break; case __LINE__: TRice16F(iD(13245), "call:FunctionNameXb", b16, sizeof(b16)/sizeof(int16_t) );          //exp: time:feed3322default: call:FunctionNameXb(0000)(ffff)(fffe)(3344) 
        break; case __LINE__: Trice16F(iD(13246), "call:FunctionNameXc", b16, sizeof(b16)/sizeof(int16_t) );          //exp: time:    be16default: call:FunctionNameXc(0000)(ffff)(fffe)(3344) 
        break; case __LINE__: trice16F(iD(13247), "call:FunctionNameXd", b16, sizeof(b16)/sizeof(int16_t) );          //exp: time:        default: call:FunctionNameXd(0000)(ffff)(fffe)(3344) 
        break; case __LINE__: TRICE32_F(ID(13248), "info:FunctionNameYa", b32, sizeof(b32)/sizeof(int32_t) );  //exp: time:feed3322default: info:FunctionNameYa(00000000)(ffffffff)(fffffffe)(33445555)
        break; case __LINE__: TRice32F(iD(13249), "rpc:FunctionNameYb", b32, sizeof(b32)/sizeof(int32_t) );           //exp: time:feed3322default: rpc:FunctionNameYb(00000000)(ffffffff)(fffffffe)(33445555)
        break; case __LINE__: Trice32F(iD(13250), "rpc:FunctionNameYc", b32, sizeof(b32)/sizeof(int32_t) );           //exp: time:    be16default: rpc:FunctionNameYc(00000000)(ffffffff)(fffffffe)(33445555)
        break; case __LINE__: trice32F(iD(13251), "call:FunctionNameYd", b32, sizeof(b32)/sizeof(int32_t) );          //exp: time:        default: call:FunctionNameYd(00000000)(ffffffff)(fffffffe)(33445555)
        break; case __LINE__: TRICE64_F(ID(13252), "info:FunctionNameZa", b64, sizeof(b64)/sizeof(int64_t) );  //exp: time:feed3322default: info:FunctionNameZa(0000000000000000)(ffffffffffffffff)(fffffffffffffffe)(3344555566666666)
        break; case __LINE__: TRice64F(iD(13253), "call:FunctionNameZb", b64, sizeof(b64)/sizeof(int64_t) );          //exp: time:feed3322default: call:FunctionNameZb(0000000000000000)(ffffffffffffffff)(fffffffffffffffe)(3344555566666666)
        break; case __LINE__: Trice64F(iD(13254), "RPC:FunctionNameZc", b64, sizeof(b64)/sizeof(int64_t) );           //exp: time:    be16default: RPC:FunctionNameZc(0000000000000000)(ffffffffffffffff)(fffffffffffffffe)(3344555566666666)
        break; case __LINE__: trice64F(iD(13255), "RPC:FunctionNameZd", b64, sizeof(b64)/sizeof(int64_t) );           //exp: time:        default: RPC:FunctionNameZd(0000000000000000)(ffffffffffffffff)(fffffffffffffffe)(3344555566666666)
        
        break; case __LINE__: TRiceS(iD(13256), "msg:Hi %s!\n", five );                     //exp: time:feed3322default: msg:Hi five!
        break; case __LINE__: TriceS(iD(13257), "msg:Hi %s!\n", five );                     //exp: time:    be16default: msg:Hi five!
        break; case __LINE__: triceS(iD(13258), "msg:Hi %s!\n", five );                     //exp: time:        default: msg:Hi five!
        break; case __LINE__: TRiceN(iD(13259), "msg:Hi %s!\n", five, strlen(five) );       //exp: time:feed3322default: msg:Hi five!
        break; case __LINE__: TriceN(iD(13260), "msg:Hi %s!\n", five, strlen(five) );       //exp: time:    be16default: msg:Hi five!
        break; case __LINE__: triceN(iD(13261), "msg:Hi %s!\n", five, strlen(five) );       //exp: time:        default: msg:Hi five!
        break; case __LINE__: TRICE_S(id(13262), "sig:TRICE_S=%s\n", s );            //exp: time:        default: sig:TRICE_S=AAAAAAAAAAAA
        break; case __LINE__: TRICE_N(id(13263), "sig:TRICE_N=%s\n", s, strlen(s) ); //exp: time:        default: sig:TRICE_N=AAAAAAAAAAAA
        break; case __LINE__: TRICE_S(Id(13264), "sig:TRICE_S=%s\n", s );            //exp: time:    be16default: sig:TRICE_S=AAAAAAAAAAAA
        break; case __LINE__: TRICE_N(Id(13265), "sig:TRICE_N=%s\n", s, strlen(s) ); //exp: time:    be16default: sig:TRICE_N=AAAAAAAAAAAA
        break; case __LINE__: TRICE_S(ID(13266), "sig:TRICE_S=%s\n", s );            //exp: time:feed3322default: sig:TRICE_S=AAAAAAAAAAAA
        break; case __LINE__: TRICE_N(ID(13267), "sig:TRICE_N=%s\n", s, strlen(s) ); //exp: time:feed3322default: sig:TRICE_N=AAAAAAAAAAAA
        break; case __LINE__: triceS(iD(13268), "sig:triceS=%s\n", s );                     //exp: time:        default: sig:triceS=AAAAAAAAAAAA
        break; case __LINE__: triceN(iD(13269), "sig:triceN=%s\n", s, strlen(s) );          //exp: time:        default: sig:triceN=AAAAAAAAAAAA
        break; case __LINE__: TriceS(iD(13270), "sig:TriceS=%s\n", s );                     //exp: time:    be16default: sig:TriceS=AAAAAAAAAAAA
        break; case __LINE__: TriceN(iD(13271), "sig:TriceN=%s\n", s, strlen(s) );          //exp: time:    be16default: sig:TriceN=AAAAAAAAAAAA
        break; case __LINE__: TRiceS(iD(13272), "sig:TRiceS=%s\n", s );                     //exp: time:feed3322default: sig:TRiceS=AAAAAAAAAAAA
        break; case __LINE__: TRiceN(iD(13273), "sig:TRiceN=%s\n", s, strlen(s) );          //exp: time:feed3322default: sig:TRiceN=AAAAAAAAAAAA

        break; case __LINE__: trice16(iD(13274), "att: line %u\n", __LINE__ );

        break; case __LINE__: exampleOfManualJSONencoding(); //exp: time:    be16default: att:MyStructEvaluationFunction(json:ExA{Apple:-1, Birn:2, Fisch:2.781000}
        break; case __LINE__: TRICE(Id(13275), "MSG:1/11 = %g\n", aFloat( 1.0/11 ) ); //exp: time:    be16default: MSG:1/11 = 0.09090909
        break; case __LINE__: { //exp: time:feed3322default: msg:x = 5.934 = 5.934, 5.934
#if !TRICE_OFF
                              float a = (float)5.934;
                              float b = a + ((a > 0) ? 0.0005f : -0.0005f);
                              int c = (int)b;
                              int d = (int)(b * 1000) % 1000;
                              int e = (int)(1000 * (float)(a - c)); 
                              TRice(iD(13276), "msg:x = %g = %d.%03d, %d.%03d\n", aFloat(a), c, d, c, e ); 
#endif                              
                              }

        break; case __LINE__: trice16(iD(13277), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRice(iD(13278), "sig:Integer (indent, base, sign) - see https://yourbasic.org/golang/fmt-printf-reference-cheat-sheet/\n" );
        break; case __LINE__: TRice(iD(13279), "rd: 15 	%d 	Base 10\n", 15 );                                        //exp: time:feed3322default: rd: 15 	15 	Base 10                                   
        break; case __LINE__: TRice(iD(13280), "rd: +15 	%+d 	Always show sign\n", 15 );                           //exp: time:feed3322default: rd: +15 	+15 	Always show sign                          
        break; case __LINE__: TRice(iD(13281), "rd:   15	%4d 	Pad with spaces (width 4, right justified)\n", 15 ); //exp: time:feed3322default: rd:   15	  15 	Pad with spaces (width 4, right justified)
        break; case __LINE__: TRice(iD(13282), "rd:    15	%-4d	Pad with spaces (width 4, left justified)\n", 15 );  //exp: time:feed3322default: rd:    15	15  	Pad with spaces (width 4, left justified) 
        break; case __LINE__: TRice(iD(13283), "rd: 0015	%04d	Pad with zeroes (width 4)\n", 15 );                  //exp: time:feed3322default: rd: 0015	0015	Pad with zeroes (width 4)                 
        break; case __LINE__: TRice(iD(13284), "rd: 1111 	%b 	Base 2\n", 15 );                                       //exp: time:feed3322default: rd: 1111 	1111 	Base 2                                    
        break; case __LINE__: TRice(iD(13285), "rd: 17 	%o 	Base 8\n", 15 );                                         //exp: time:feed3322default: rd: 17 	17 	Base 8                                    
        break; case __LINE__: TRice(iD(13286), "rd: f 	%x 	Base 16, lowercase\n", 15 );                             //exp: time:feed3322default: rd: f 	f 	Base 16, lowercase                        
        break; case __LINE__: TRice(iD(13287), "rd: F 	%X 	Base 16, uppercase\n", 15 );                             //exp: time:feed3322default: rd: F 	F 	Base 16, uppercase                        
        break; case __LINE__: TRice(iD(13288), "rd: 0xf 	%#x 	Base 16, with leading 0x\n", 15 );                   //exp: time:feed3322default: rd: 0xf 	0xf 	Base 16, with leading 0x                  

        break; case __LINE__: trice16(iD(13289), "att: line %u\n", __LINE__ );

        break; case __LINE__: trice(iD(13290), "sig:Character (quoted, Unicode)\n" );
        break; case __LINE__: trice(iD(13291), "rd: A 	%c 	Character\n", 'A' );                                     //exp: time:        default: rd: A 	A 	Character
        break; case __LINE__: trice(iD(13292), "rd: 'A' 	%q 	Quoted character\n", 'A' );                            //exp: time:        default: rd: 'A' 	'A' 	Quoted character
    //  break; case __LINE__: trice(iD(13293), "rd: U+0041 	%U 	Unicode\n", 'A' );                        // todo: detect %U  to avoid message triceType = TRICE0 ParamSpace = 4 not matching with bitWidth  0 and paramCount 0 - ignoring package
    //  break; case __LINE__: trice(iD(13294), "rd: U+0041 'A' 	%#U 	Unicode with character\n", 'A' );   // todo: detect %#U to avoid message triceType = TRICE0 ParamSpace = 4 not matching with bitWidth  0 and paramCount 0 - ignoring package

        break; case __LINE__: trice(iD(13295), "sig:Boolean (true/false)\n" );
        break; case __LINE__: trice(iD(13296), "rd:Use %%t to format a boolean as true (%t) or false (%t).\n", 1, 0 ); //exp: time:        default: rd:Use %t to format a boolean as true (true) or false (false).

        break; case __LINE__: trice(iD(13297), "sig:Pointer (hex)\n" );
        break; case __LINE__: trice8 (iD(13298), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x84 );               //exp: time:        default: rd:Use %p to format a pointer in base 16 notation with leading 0x. (84)              
        break; case __LINE__: trice16(iD(13299), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x1234 );             //exp: time:        default: rd:Use %p to format a pointer in base 16 notation with leading 0x. (1234)            
        break; case __LINE__: trice32(iD(13300), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0xaabbccdd );         //exp: time:        default: rd:Use %p to format a pointer in base 16 notation with leading 0x. (aabbccdd)        
        break; case __LINE__: trice64(iD(13301), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x1122334455667788 ); //exp: time:        default: rd:Use %p to format a pointer in base 16 notation with leading 0x. (1122334455667788)
        break; case __LINE__: trice  (iD(13302), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0xaabbccdd);          //exp: time:        default: rd:Use %p to format a pointer in base 16 notation with leading 0x. (aabbccdd)        

        break; case __LINE__: trice16(iD(13303), "att: line %u\n", __LINE__ );
        break; case __LINE__: trice(iD(13304), "sig:Float (indent, precision, scientific notation)\n" );
        break; case __LINE__: trice(iD(13305), "rd: 1.234560e+02		%e 	%%e Scientific notation\n", aFloat(123.456) );                               //exp: time:        default: rd: 1.234560e+02		1.234560e+02 	%e Scientific notation
        break; case __LINE__: trice(iD(13306), "rd: 123.456000		%f 	%%f Decimal point, no exponent\n", aFloat(123.456) );                          //exp: time:        default: rd: 123.456000		123.456001 	%f Decimal point, no exponent
        break; case __LINE__: trice(iD(13307), "rd: 123.46			%.2f		%%.2f Default width, precision 2\n", aFloat(123.456) );                      //exp: time:        default: rd: 123.46			123.46		%.2f Default width, precision 2
        break; case __LINE__: trice(iD(13308), "rd: ␣␣123.46		%8.2f	%%8.2f Width 8, precision 2\n", aFloat(123.456) );                             //exp: time:        default: rd: ␣␣123.46		  123.46	%8.2f Width 8, precision 2
        break; case __LINE__: trice(iD(13309), "rd: 123.456		%g		%%g Exponent as needed, necessary digits only\n", aFloat(123.456) );           //exp: time:        default: rd: 123.456		123.456		%g Exponent as needed, necessary digits only
        break; case __LINE__: trice(iD(13310), "sig:Double (indent, precision, scientific notation)\n" );
        break; case __LINE__: trice64(iD(13311), "rd: 1.234560e+02		%e 	%%e Scientific notation\n", aDouble(123.456) );                            //exp: time:        default: rd: 1.234560e+02		1.234560e+02 	%e Scientific notation
        break; case __LINE__: trice64(iD(13312), "rd: 123.456000		%f 	%%f Decimal point, no exponent\n", aDouble(123.456) );                       //exp: time:        default: rd: 123.456000		123.456000 	%f Decimal point, no exponent
        break; case __LINE__: trice64(iD(13313), "rd: 123.46			%.2f    	%%.2f Default width, precision 2\n", aDouble(123.456) );                 //exp: time:        default: rd: 123.46			123.46    	%.2f Default width, precision 2
        break; case __LINE__: trice64(iD(13314), "rd: ␣␣123.46		%8.2f 	%%8.2f Width 8, precision 2\n", aDouble(123.456) );                        //exp: time:        default: rd: ␣␣123.46		  123.46 	%8.2f Width 8, precision 2
        break; case __LINE__: trice64(iD(13315), "rd: 123.456		%g		%%g Exponent as needed, necessary digits only\n", aDouble(123.456) );        //exp: time:        default: rd: 123.456		123.456		%g Exponent as needed, necessary digits only

        break; case __LINE__: trice(iD(13316), "sig:String or byte slice (quote, indent, hex)\n" );
        break; case __LINE__: s = "café"; triceS(iD(13317), "rd: café 			%s		Plain string\n", s );                //exp: time:        default: rd: café 			café		Plain string
        break; case __LINE__: s = "café"; triceS(iD(13318), "rd: ␣␣café 		%6s 		Width 6, right justify\n", s );     //exp: time:        default: rd: ␣␣café 		  café 		Width 6, right justify
        break; case __LINE__: s = "café"; triceS(iD(13319), "rd: café␣␣ 		%-6s 		Width 6, left justify\n", s );      //exp: time:        default: rd: café␣␣ 		café   		Width 6, left justify
        break; case __LINE__: s = "café"; triceS(iD(13320), "rd: \"café\" 		%q		Quoted string\n", s );             //exp: time:        default: rd: \"café\" 		"café"		Quoted string
        break; case __LINE__: s = "café"; triceS(iD(13321), "rd: 636166c3a9 		%x	Hex dump of byte values\n", s );   //exp: time:        default: rd: 636166c3a9 		636166c3a9	Hex dump of byte values 
        break; case __LINE__: s = "café"; triceS(iD(13322), "rd: 63 61 66 c3 a9 	% x	Hex dump with spaces\n", s );    //exp: time:        default: rd: 63 61 66 c3 a9 	63 61 66 c3 a9	Hex dump with spaces

        break; case __LINE__: trice16(iD(13323), "att: line %u\n", __LINE__ );
        break; case __LINE__: exampleOfManualSerialization(); // ATTENTION: This occupies ~1024 bytes in one half buffer when double buffer ist used!

        // This was a CGO compiler issue:
        break; case __LINE__: TRice(iD(13324), "info:12 default bit width values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 32-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //exp: time:feed3322default: info:12 default bit width values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and a 32-bit stamp.
        break; case __LINE__: Trice(iD(13325), "info:12 default bit width values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 16-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //exp: time:    be16default: info:12 default bit width values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and a 16-bit stamp.
        break; case __LINE__: trice(iD(13326), "info:12 default bit width values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and without stamp.\n" , -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //exp: time:        default: info:12 default bit width values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and without stamp.

        break; case __LINE__: trice16(iD(13327), "att: line %u\n", __LINE__ );

        break; case __LINE__: trice(iD(13328), "sig:Runtime generated strings\n" );
        break; case __LINE__: 
        {
            #if !TRICE_OFF
            char* s0 = "AAAAAAAAAAAA";
            #endif
            TRICE32(ID(13329), "dbg:len=%u:", strlen(s0) );
            TRICE_S(ID(13330), "sig:%s\n", s0 );
        }
        break; case __LINE__: 
        {
            #if !TRICE_OFF
            char* s1 = "\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBCCCCCCCC\
";
#endif
                TRICE32(ID(13331), "dbg:len=%u:", strlen(s1) );
                TRICE_S(ID(13332), "sig:%s\n", s1 );
            }
        break; case __LINE__: 
        {
            #if !TRICE_OFF
            char* s2 = "\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\
DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\
EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\
";
#endif
            TRICE32(ID(13333), "dbg:len=%u: ", strlen(s2) );
            TRICE_S(ID(13334), "sig:%s\n", s2 );
        }
        break; case __LINE__: 
        {
            #if !TRICE_OFF
            char* s3 = "\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\
DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\
EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\
";
#endif
            TRICE32(ID(13335), "dbg:len=%u: ", strlen(s3) );
            TRICE_S(ID(13336), "sig:%s\n", s3 );
        }
        break; case __LINE__:
        {
                        #if !TRICE_OFF
            char* s4 = "\
0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789\
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
#endif
            TRICE32(ID(13337), "dbg:len=%u: ", strlen(s4) );
            TRICE_S(ID(13338), "sig:%s\n", s4 );
        }


#if TRICE_CGO == 1 || defined(TRICE_FULL_CHECK) 

        break; case __LINE__: trice  (iD(13339), "rd:%E (%%E)\n",  aFloat(-555555555.5555555555) ); //exp: time:        default: rd:-5.555556E+08 (%E)
        break; case __LINE__: trice  (iD(13340), "rd:%F (%%F)\n",  aFloat(-555555555.5555555555) ); //exp: time:        default: rd:-555555584.000000 (%F)
        break; case __LINE__: trice  (iD(13341), "rd:%G (%%G)\n",  aFloat(-555555555.5555555555) ); //exp: time:        default: rd:-5.555556E+08 (%G)
        break; case __LINE__: trice64(iD(13342), "rd:%E (%%E)\n", aDouble(-555555555.5555555555) ); //exp: time:        default: rd:-5.555556E+08 (%E)
        break; case __LINE__: trice64(iD(13343), "rd:%F (%%F)\n", aDouble(-555555555.5555555555) ); //exp: time:        default: rd:-555555555.555556 (%F)
        break; case __LINE__: trice64(iD(13344), "rd:%G (%%G)\n", aDouble(-555555555.5555555555) ); //exp: time:        default: rd:-5.555555555555556E+08 (%G)
        
        break; case __LINE__: trice  (iD(13345), "rd:%e (%%e)\n",  aFloat(-555555555.5555555555) ); //exp: time:        default: rd:-5.555556e+08 (%e)
        break; case __LINE__: trice  (iD(13346), "rd:%f (%%f)\n",  aFloat(-555555555.5555555555) ); //exp: time:        default: rd:-555555584.000000 (%f)
        break; case __LINE__: trice  (iD(13347), "rd:%g (%%g)\n",  aFloat(-555555555.5555555555) ); //exp: time:        default: rd:-5.555556e+08 (%g)
        break; case __LINE__: trice64(iD(13348), "rd:%e (%%e)\n", aDouble(-555555555.5555555555) ); //exp: time:        default: rd:-5.555556e+08 (%e)
        break; case __LINE__: trice64(iD(13349), "rd:%f (%%f)\n", aDouble(-555555555.5555555555) ); //exp: time:        default: rd:-555555555.555556 (%f)
        break; case __LINE__: trice64(iD(13350), "rd:%g (%%g)\n", aDouble(-555555555.5555555555) ); //exp: time:        default: rd:-5.555555555555556e+08 (%g)

        break; case __LINE__: trice32(iD(13351), "msg:%u (%%u)\n", -1 ); //exp: time:        default: msg:4294967295 (%u)
        break; case __LINE__: trice32(iD(13352), "msg:%b (%%b)\n", -1 ); //exp: time:        default: msg:11111111111111111111111111111111 (%b)
        break; case __LINE__: trice32(iD(13353), "msg:%o (%%o)\n", -1 ); //exp: time:        default: msg:37777777777 (%o)
        break; case __LINE__: trice32(iD(13354), "msg:%O (%%O)\n", -1 ); //exp: time:        default: msg:0o37777777777 (%O)
        break; case __LINE__: trice32(iD(13355), "msg:%X (%%X)\n", -1 ); //exp: time:        default: msg:FFFFFFFF (%X)
        break; case __LINE__: trice32(iD(13356), "msg:%x (%%x)\n", -1 ); //exp: time:        default: msg:ffffffff (%x)
        break; case __LINE__: trice32(iD(13357), "msg:%d (%%d)\n", -1 ); //exp: time:        default: msg:-1 (%d)
        break; case __LINE__: trice64(iD(13358), "msg:%u (%%u)\n", -1 ); //exp: time:        default: msg:18446744073709551615 (%u)
        break; case __LINE__: trice64(iD(13359), "msg:%b (%%b)\n", -1 ); //exp: time:        default: msg:1111111111111111111111111111111111111111111111111111111111111111 (%b)
        break; case __LINE__: trice64(iD(13360), "msg:%o (%%o)\n", -1 ); //exp: time:        default: msg:1777777777777777777777 (%o)
        break; case __LINE__: trice64(iD(13361), "msg:%O (%%O)\n", -1 ); //exp: time:        default: msg:0o1777777777777777777777 (%O)
        break; case __LINE__: trice64(iD(13362), "msg:%X (%%X)\n", -1 ); //exp: time:        default: msg:FFFFFFFFFFFFFFFF (%X)
        break; case __LINE__: trice64(iD(13363), "msg:%x (%%x)\n", -1 ); //exp: time:        default: msg:ffffffffffffffff (%x)

    /**/break; case __LINE__: TRice        (iD(13364), "value=%d\n", -2  ); //exp: time:feed3322default: value=-2
    /**/break; case __LINE__: Trice        (iD(13365), "value=%d\n", -2  ); //exp: time:    be16default: value=-2
    /**/break; case __LINE__: trice        (iD(13366), "value=%d\n", -2  ); //exp: time:        default: value=-2
        break; case __LINE__: TRice8       (iD(13367), "value=%d\n", -2  ); //exp: time:feed3322default: value=-2
        break; case __LINE__: Trice8       (iD(13368), "value=%d\n", -2  ); //exp: time:    be16default: value=-2
        break; case __LINE__: trice8       (iD(13369), "value=%d\n", -2  ); //exp: time:        default: value=-2
        break; case __LINE__: TRice16      (iD(13370), "value=%d\n", -2  ); //exp: time:feed3322default: value=-2
        break; case __LINE__: Trice16      (iD(13371), "value=%d\n", -2  ); //exp: time:    be16default: value=-2
        break; case __LINE__: trice16      (iD(13372), "value=%d\n", -2  ); //exp: time:        default: value=-2
        break; case __LINE__: TRice32      (iD(13373), "value=%d\n", -2  ); //exp: time:feed3322default: value=-2
        break; case __LINE__: Trice32      (iD(13374), "value=%d\n", -2  ); //exp: time:    be16default: value=-2
        break; case __LINE__: trice32      (iD(13375), "value=%d\n", -2  ); //exp: time:        default: value=-2
        break; case __LINE__: TRice64      (iD(13376), "value=%d\n", -2  ); //exp: time:feed3322default: value=-2
        break; case __LINE__: Trice64      (iD(13377), "value=%d\n", -2  ); //exp: time:    be16default: value=-2
        break; case __LINE__: trice64      (iD(13378), "value=%d\n", -2  ); //exp: time:        default: value=-2
        
        break; case __LINE__: TRice_1      (iD(13379), "value=%d\n", -2  ); //exp: time:feed3322default: value=-2
        break; case __LINE__: Trice_1      (iD(13380), "value=%d\n", -2  ); //exp: time:    be16default: value=-2
        break; case __LINE__: trice_1      (iD(13381), "value=%d\n", -2  ); //exp: time:        default: value=-2
        break; case __LINE__: TRice8_1     (iD(13382), "value=%d\n", -2  ); //exp: time:feed3322default: value=-2
        break; case __LINE__: Trice8_1     (iD(13383), "value=%d\n", -2  ); //exp: time:    be16default: value=-2
        break; case __LINE__: trice8_1     (iD(13384), "value=%d\n", -2  ); //exp: time:        default: value=-2
        break; case __LINE__: TRice16_1    (iD(13385), "value=%d\n", -2  ); //exp: time:feed3322default: value=-2
        break; case __LINE__: Trice16_1    (iD(13386), "value=%d\n", -2  ); //exp: time:    be16default: value=-2
        break; case __LINE__: trice16_1    (iD(13387), "value=%d\n", -2  ); //exp: time:        default: value=-2
        break; case __LINE__: TRice32_1    (iD(13388), "value=%d\n", -2  ); //exp: time:feed3322default: value=-2
        break; case __LINE__: Trice32_1    (iD(13389), "value=%d\n", -2  ); //exp: time:    be16default: value=-2
        break; case __LINE__: trice32_1    (iD(13390), "value=%d\n", -2  ); //exp: time:        default: value=-2
        break; case __LINE__: TRice64_1    (iD(13391), "value=%d\n", -2  ); //exp: time:feed3322default: value=-2
        break; case __LINE__: Trice64_1    (iD(13392), "value=%d\n", -2  ); //exp: time:    be16default: value=-2
        break; case __LINE__: trice64_1    (iD(13393), "value=%d\n", -2  ); //exp: time:        default: value=-2
        
        break; case __LINE__: TRice        (iD(13394), "no value" ); //exp: time:feed3322default: no value
        break; case __LINE__: Trice        (iD(13395), "no value" ); //exp: time:    be16default: no value
        break; case __LINE__: trice        (iD(13396), "no value" ); //exp: time:        default: no value
        break; case __LINE__: TRice8       (iD(13397), "no value" ); //exp: time:feed3322default: no value
        break; case __LINE__: Trice8       (iD(13398), "no value" ); //exp: time:    be16default: no value
        break; case __LINE__: trice8       (iD(13399), "no value" ); //exp: time:        default: no value
        break; case __LINE__: TRice16      (iD(13400), "no value" ); //exp: time:feed3322default: no value
        break; case __LINE__: Trice16      (iD(13401), "no value" ); //exp: time:    be16default: no value
        break; case __LINE__: trice16      (iD(13402), "no value" ); //exp: time:        default: no value
        break; case __LINE__: TRice32      (iD(13403), "no value" ); //exp: time:feed3322default: no value
        break; case __LINE__: Trice32      (iD(13404), "no value" ); //exp: time:    be16default: no value
        break; case __LINE__: trice32      (iD(13405), "no value" ); //exp: time:        default: no value
        break; case __LINE__: TRice64      (iD(13406), "no value" ); //exp: time:feed3322default: no value
        break; case __LINE__: Trice64      (iD(13407), "no value" ); //exp: time:    be16default: no value
        break; case __LINE__: trice64      (iD(13408), "no value" ); //exp: time:        default: no value
    
        break; case __LINE__: TRice_0      (iD(13409), "no value" ); //exp: time:feed3322default: no value
        break; case __LINE__: Trice_0      (iD(13410), "no value" ); //exp: time:    be16default: no value
        break; case __LINE__: trice_0      (iD(13411), "no value" ); //exp: time:        default: no value
        break; case __LINE__: TRice8_0     (iD(13412), "no value" ); //exp: time:feed3322default: no value
        break; case __LINE__: Trice8_0     (iD(13413), "no value" ); //exp: time:    be16default: no value
        break; case __LINE__: trice8_0     (iD(13414), "no value" ); //exp: time:        default: no value
        break; case __LINE__: TRice16_0    (iD(13415), "no value" ); //exp: time:feed3322default: no value
        break; case __LINE__: Trice16_0    (iD(13416), "no value" ); //exp: time:    be16default: no value
        break; case __LINE__: trice16_0    (iD(13417), "no value" ); //exp: time:        default: no value
        break; case __LINE__: TRice32_0    (iD(13418), "no value" ); //exp: time:feed3322default: no value
        break; case __LINE__: Trice32_0    (iD(13419), "no value" ); //exp: time:    be16default: no value
        break; case __LINE__: trice32_0    (iD(13420), "no value" ); //exp: time:        default: no value
        break; case __LINE__: TRice64_0    (iD(13421), "no value" ); //exp: time:feed3322default: no value
        break; case __LINE__: Trice64_0    (iD(13422), "no value" ); //exp: time:    be16default: no value
        break; case __LINE__: trice64_0    (iD(13423), "no value" ); //exp: time:        default: no value
        break; case __LINE__: TRice0       (iD(13424), "no value" ); //exp: time:feed3322default: no value
        break; case __LINE__: Trice0       (iD(13425), "no value" ); //exp: time:    be16default: no value
        break; case __LINE__: trice0       (iD(13426), "no value" ); //exp: time:        default: no value
        break; case __LINE__: TRice        (iD(13427), "no value" ); //exp: time:feed3322default: no value
        break; case __LINE__: Trice        (iD(13428), "no value" ); //exp: time:    be16default: no value
        break; case __LINE__: trice        (iD(13429), "no value" ); //exp: time:        default: no value

        break; case __LINE__: TRice(iD(13430), "info:This is a message with one value %d and a 32-bit stamp.\n", -2  );        //exp: time:feed3322default: info:This is a message with one value -2 and a 32-bit stamp.
        break; case __LINE__: Trice(iD(13431), "info:This is a message with one value %d and a 16-bit stamp.\n", -2  );        //exp: time:    be16default: info:This is a message with one value -2 and a 16-bit stamp.
        break; case __LINE__: trice(iD(13432), "info:This is a message with one value %d and without stamp.\n" , -2  );        //exp: time:        default: info:This is a message with one value -2 and without stamp.
        break; case __LINE__: TRICE(ID(13433), "info:This is a message without values and a 32-bit stamp.\n" );         //exp: time:feed3322default: info:This is a message without values and a 32-bit stamp.
        break; case __LINE__: TRICE(Id(13434), "info:This is a message without values and a 16-bit stamp.\n" );         //exp: time:    be16default: info:This is a message without values and a 16-bit stamp.
        break; case __LINE__: TRICE(id(13435), "info:This is a message without values and without stamp.\n"  );         //exp: time:        default: info:This is a message without values and without stamp.
        break; case __LINE__: TRICE(ID(13436), "info:This is a message with one value %d and a 32-bit stamp.\n", -2  ); //exp: time:feed3322default: info:This is a message with one value -2 and a 32-bit stamp.
        break; case __LINE__: TRICE(Id(13437), "info:This is a message with one value %d and a 16-bit stamp.\n", -2  ); //exp: time:    be16default: info:This is a message with one value -2 and a 16-bit stamp.
        break; case __LINE__: TRICE(id(13438), "info:This is a message with one value %d and without stamp.\n" , -2  ); //exp: time:        default: info:This is a message with one value -2 and without stamp.

        break; case __LINE__: TRICE(ID(13439), "info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 32-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 );   //exp: time:feed3322default: info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and a 32-bit stamp.
        break; case __LINE__: TRICE(Id(13440), "info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 16-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 );   //exp: time:    be16default: info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and a 16-bit stamp.
        break; case __LINE__: TRICE(id(13441), "info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and without stamp.\n" , -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 );   //exp: time:        default: info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and without stamp.
        break; case __LINE__: TRICE32(ID(13442), "info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 32-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //exp: time:feed3322default: info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and a 32-bit stamp.
        break; case __LINE__: TRICE32(Id(13443), "info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 16-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //exp: time:    be16default: info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and a 16-bit stamp.
        break; case __LINE__: TRICE32(id(13444), "info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and without stamp.\n" , -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //exp: time:        default: info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and without stamp.

        break; case __LINE__: TRICE8   (id(13445), "msg:value=%u\n", 200 ); //exp: time:        default: msg:value=200
        break; case __LINE__: TRICE8_1 (id(13446), "msg:value=%u\n", 200 ); //exp: time:        default: msg:value=200
        break; case __LINE__: TRICE8   (Id(13447), "msg:value=%u\n", 200 ); //exp: time:    be16default: msg:value=200
        break; case __LINE__: TRICE8_1 (Id(13448), "msg:value=%u\n", 200 ); //exp: time:    be16default: msg:value=200
        break; case __LINE__: TRICE8   (ID(13449), "msg:value=%u\n", 200 ); //exp: time:feed3322default: msg:value=200
        break; case __LINE__: TRICE8_1 (ID(13450), "msg:value=%u\n", 200 ); //exp: time:feed3322default: msg:value=200
        break; case __LINE__: trice8   (iD(13451), "msg:value=%u\n", 200 ); //exp: time:        default: msg:value=200
        break; case __LINE__: trice8_1 (iD(13452), "msg:value=%u\n", 200 ); //exp: time:        default: msg:value=200
        break; case __LINE__: Trice8   (iD(13453), "msg:value=%u\n", 200 ); //exp: time:    be16default: msg:value=200
        break; case __LINE__: Trice8_1 (iD(13454), "msg:value=%u\n", 200 ); //exp: time:    be16default: msg:value=200
        break; case __LINE__: TRice8   (iD(13455), "msg:value=%u\n", 200 ); //exp: time:feed3322default: msg:value=200
        break; case __LINE__: TRice8_1 (iD(13456), "msg:value=%u\n", 200 ); //exp: time:feed3322default: msg:value=200

        break; case __LINE__: TRICE16  (id(13457), "msg:value=%u\n", 200 ); //exp: time:        default: msg:value=200
        break; case __LINE__: TRICE16_1(id(13458), "msg:value=%u\n", 200 ); //exp: time:        default: msg:value=200
        break; case __LINE__: TRICE16  (Id(13459), "msg:value=%u\n", 200 ); //exp: time:    be16default: msg:value=200
        break; case __LINE__: TRICE16_1(Id(13460), "msg:value=%u\n", 200 ); //exp: time:    be16default: msg:value=200
        break; case __LINE__: TRICE16  (ID(13461), "msg:value=%u\n", 200 ); //exp: time:feed3322default: msg:value=200
        break; case __LINE__: TRICE16_1(ID(13462), "msg:value=%u\n", 200 ); //exp: time:feed3322default: msg:value=200
        break; case __LINE__: trice16  (iD(13463), "msg:value=%u\n", 200 ); //exp: time:        default: msg:value=200
        break; case __LINE__: trice16_1(iD(13464), "msg:value=%u\n", 200 ); //exp: time:        default: msg:value=200
        break; case __LINE__: Trice16  (iD(13465), "msg:value=%u\n", 200 ); //exp: time:    be16default: msg:value=200
        break; case __LINE__: Trice16_1(iD(13466), "msg:value=%u\n", 200 ); //exp: time:    be16default: msg:value=200
        break; case __LINE__: TRice16  (iD(13467), "msg:value=%u\n", 200 ); //exp: time:feed3322default: msg:value=200
        break; case __LINE__: TRice16_1(iD(13468), "msg:value=%u\n", 200 ); //exp: time:feed3322default: msg:value=200

        break; case __LINE__: TRICE32  (id(13469), "msg:value=%u\n", 200 ); //exp: time:        default: msg:value=200
        break; case __LINE__: TRICE32_1(id(13470), "msg:value=%u\n", 200 ); //exp: time:        default: msg:value=200
        break; case __LINE__: TRICE32  (Id(13471), "msg:value=%u\n", 200 ); //exp: time:    be16default: msg:value=200
        break; case __LINE__: TRICE32_1(Id(13472), "msg:value=%u\n", 200 ); //exp: time:    be16default: msg:value=200
        break; case __LINE__: TRICE32  (ID(13473), "msg:value=%u\n", 200 ); //exp: time:feed3322default: msg:value=200
        break; case __LINE__: TRICE32_1(ID(13474), "msg:value=%u\n", 200 ); //exp: time:feed3322default: msg:value=200
        break; case __LINE__: trice32  (iD(13475), "msg:value=%u\n", 200 ); //exp: time:        default: msg:value=200
        break; case __LINE__: trice32_1(iD(13476), "msg:value=%u\n", 200 ); //exp: time:        default: msg:value=200
        break; case __LINE__: Trice32  (iD(13477), "msg:value=%u\n", 200 ); //exp: time:    be16default: msg:value=200
        break; case __LINE__: Trice32_1(iD(13478), "msg:value=%u\n", 200 ); //exp: time:    be16default: msg:value=200
        break; case __LINE__: TRice32  (iD(13479), "msg:value=%u\n", 200 ); //exp: time:feed3322default: msg:value=200
        break; case __LINE__: TRice32_1(iD(13480), "msg:value=%u\n", 200 ); //exp: time:feed3322default: msg:value=200

        break; case __LINE__: TRICE64  (id(13481), "msg:value=%u\n", 200 ); //exp: time:        default: msg:value=200
        break; case __LINE__: TRICE64_1(id(13482), "msg:value=%u\n", 200 ); //exp: time:        default: msg:value=200
        break; case __LINE__: TRICE64  (Id(13483), "msg:value=%u\n", 200 ); //exp: time:    be16default: msg:value=200
        break; case __LINE__: TRICE64_1(Id(13484), "msg:value=%u\n", 200 ); //exp: time:    be16default: msg:value=200
        break; case __LINE__: TRICE64  (ID(13485), "msg:value=%u\n", 200 ); //exp: time:feed3322default: msg:value=200
        break; case __LINE__: TRICE64_1(ID(13486), "msg:value=%u\n", 200 ); //exp: time:feed3322default: msg:value=200
        break; case __LINE__: trice64  (iD(13487), "msg:value=%u\n", 200 ); //exp: time:        default: msg:value=200
        break; case __LINE__: trice64_1(iD(13488), "msg:value=%u\n", 200 ); //exp: time:        default: msg:value=200
        break; case __LINE__: Trice64  (iD(13489), "msg:value=%u\n", 200 ); //exp: time:    be16default: msg:value=200
        break; case __LINE__: Trice64_1(iD(13490), "msg:value=%u\n", 200 ); //exp: time:    be16default: msg:value=200
        break; case __LINE__: TRice64  (iD(13491), "msg:value=%u\n", 200 ); //exp: time:feed3322default: msg:value=200
        break; case __LINE__: TRice64_1(iD(13492), "msg:value=%u\n", 200 ); //exp: time:feed3322default: msg:value=200

        // mixed 16-bit and 32-bit timestamp 

        break; case __LINE__: TRice8(iD(13493), "msg:value=%d\n", -1 );                                                                                            //exp: time:feed3322default: msg:value=-1
        break; case __LINE__: Trice8(iD(13494), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: msg:value=-1, -2
        break; case __LINE__: trice8(iD(13495), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: msg:value=-1, -2, -3
        break; case __LINE__: TRice8(iD(13496), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice8(iD(13497), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice8(iD(13498), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice8(iD(13499), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:feed3322default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice8(iD(13500), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice8(iD(13501), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice8(iD(13502), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:feed3322default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice8(iD(13503), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice8(iD(13504), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice16(iD(13505), "msg:value=%d\n", -1 );                                                                                            //exp: time:feed3322default: msg:value=-1
        break; case __LINE__: Trice16(iD(13506), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: msg:value=-1, -2
        break; case __LINE__: trice16(iD(13507), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: msg:value=-1, -2, -3
        break; case __LINE__: TRice16(iD(13508), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice16(iD(13509), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice16(iD(13510), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice16(iD(13511), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:feed3322default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice16(iD(13512), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice16(iD(13513), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice16(iD(13514), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:feed3322default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice16(iD(13515), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice16(iD(13516), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice32(iD(13517), "msg:value=%d\n", -1 );                                                                                            //exp: time:feed3322default: msg:value=-1
        break; case __LINE__: Trice32(iD(13518), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: msg:value=-1, -2
        break; case __LINE__: trice32(iD(13519), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: msg:value=-1, -2, -3
        break; case __LINE__: TRice32(iD(13520), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice32(iD(13521), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice32(iD(13522), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice32(iD(13523), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:feed3322default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice32(iD(13524), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice32(iD(13525), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice32(iD(13526), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:feed3322default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice32(iD(13527), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice32(iD(13528), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice64(iD(13529), "msg:value=%d\n", -1 );                                                                                            //exp: time:feed3322default: msg:value=-1
        break; case __LINE__: Trice64(iD(13530), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: msg:value=-1, -2
        break; case __LINE__: trice64(iD(13531), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: msg:value=-1, -2, -3
        break; case __LINE__: TRice64(iD(13532), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice64(iD(13533), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice64(iD(13534), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice64(iD(13535), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:feed3322default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice64(iD(13536), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice64(iD(13537), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice64(iD(13538), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:feed3322default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice64(iD(13539), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice64(iD(13540), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice8_1 (iD(13541), "msg:value=%d\n", -1 );                                                                                            //exp: time:feed3322default: msg:value=-1
        break; case __LINE__: Trice8_2 (iD(13542), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: msg:value=-1, -2
        break; case __LINE__: trice8_3 (iD(13543), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: msg:value=-1, -2, -3
        break; case __LINE__: TRice8_4 (iD(13544), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice8_5 (iD(13545), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice8_6 (iD(13546), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice8_7 (iD(13547), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:feed3322default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice8_8 (iD(13548), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice8_9 (iD(13549), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice8_10(iD(13550), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:feed3322default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice8_11(iD(13551), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice8_12(iD(13552), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice16_1 (iD(13553), "msg:value=%d\n", -1 );                                                                                            //exp: time:feed3322default: msg:value=-1
        break; case __LINE__: Trice16_2 (iD(13554), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: msg:value=-1, -2
        break; case __LINE__: trice16_3 (iD(13555), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: msg:value=-1, -2, -3
        break; case __LINE__: TRice16_4 (iD(13556), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice16_5 (iD(13557), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice16_6 (iD(13558), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice16_7 (iD(13559), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:feed3322default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice16_8 (iD(13560), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice16_9 (iD(13561), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice16_10(iD(13562), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:feed3322default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice16_11(iD(13563), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice16_12(iD(13564), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice32_1 (iD(13565), "msg:value=%d\n", -1 );                                                                                            //exp: time:feed3322default: msg:value=-1
        break; case __LINE__: Trice32_2 (iD(13566), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: msg:value=-1, -2
        break; case __LINE__: trice32_3 (iD(13567), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: msg:value=-1, -2, -3
        break; case __LINE__: TRice32_4 (iD(13568), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice32_5 (iD(13569), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice32_6 (iD(13570), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice32_7 (iD(13571), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:feed3322default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice32_8 (iD(13572), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice32_9 (iD(13573), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice32_10(iD(13574), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:feed3322default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice32_11(iD(13575), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice32_12(iD(13576), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice64_1 (iD(13577), "msg:value=%d\n", -1 );                                                                                            //exp: time:feed3322default: msg:value=-1
        break; case __LINE__: Trice64_2 (iD(13578), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: msg:value=-1, -2
        break; case __LINE__: trice64_3 (iD(13579), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: msg:value=-1, -2, -3
        break; case __LINE__: TRice64_4 (iD(13580), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice64_5 (iD(13581), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice64_6 (iD(13582), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice64_7 (iD(13583), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:feed3322default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice64_8 (iD(13584), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice64_9 (iD(13585), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice64_10(iD(13586), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:feed3322default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice64_11(iD(13587), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice64_12(iD(13588), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        // use cases for 8-bit data
        break; case __LINE__: trice8     (iD(13589), "value=%d\n", -2  ); //exp: time:        default: value=-2
        break; case __LINE__: Trice8     (iD(13590), "value=%d\n", -2  ); //exp: time:    be16default: value=-2
        break; case __LINE__: TRice8     (iD(13591), "value=%d\n", -2  ); //exp: time:feed3322default: value=-2
        break; case __LINE__: trice8_1   (iD(13592), "value=%d\n", -2  ); //exp: time:        default: value=-2
        break; case __LINE__: Trice8_1   (iD(13593), "value=%d\n", -2  ); //exp: time:    be16default: value=-2
        break; case __LINE__: TRice8_1   (iD(13594), "value=%d\n", -2  ); //exp: time:feed3322default: value=-2

        // use cases for 16-bit data
        break; case __LINE__: trice16    (iD(13595), "value=%d\n", -2  ); //exp: time:        default: value=-2
        break; case __LINE__: Trice16    (iD(13596), "value=%d\n", -2  ); //exp: time:    be16default: value=-2
        break; case __LINE__: TRice16    (iD(13597), "value=%d\n", -2  ); //exp: time:feed3322default: value=-2
        break; case __LINE__: trice16_1  (iD(13598), "value=%d\n", -2  ); //exp: time:        default: value=-2
        break; case __LINE__: Trice16_1  (iD(13599), "value=%d\n", -2  ); //exp: time:    be16default: value=-2
        break; case __LINE__: TRice16_1  (iD(13600), "value=%d\n", -2  ); //exp: time:feed3322default: value=-2

        // use cases for 32-bit data
        break; case __LINE__: trice32    (iD(13601), "value=%d\n", -2  ); //exp: time:        default: value=-2
        break; case __LINE__: Trice32    (iD(13602), "value=%d\n", -2  ); //exp: time:    be16default: value=-2
        break; case __LINE__: TRice32    (iD(13603), "value=%d\n", -2  ); //exp: time:feed3322default: value=-2
        break; case __LINE__: trice32_1  (iD(13604), "value=%d\n", -2  ); //exp: time:        default: value=-2
        break; case __LINE__: Trice32_1  (iD(13605), "value=%d\n", -2  ); //exp: time:    be16default: value=-2
        break; case __LINE__: TRice32_1  (iD(13606), "value=%d\n", -2  ); //exp: time:feed3322default: value=-2

        // use cases for 64-bit data
        break; case __LINE__: trice64    (iD(13607), "value=%d\n", -2  ); //exp: time:        default: value=-2
        break; case __LINE__: Trice64    (iD(13608), "value=%d\n", -2  ); //exp: time:    be16default: value=-2
        break; case __LINE__: TRice64    (iD(13609), "value=%d\n", -2  ); //exp: time:feed3322default: value=-2
        break; case __LINE__: trice64_1  (iD(13610), "value=%d\n", -2  ); //exp: time:        default: value=-2
        break; case __LINE__: Trice64_1  (iD(13611), "value=%d\n", -2  ); //exp: time:    be16default: value=-2
        break; case __LINE__: TRice64_1  (iD(13612), "value=%d\n", -2  ); //exp: time:feed3322default: value=-2

        // legacy use cases
        break; case __LINE__: TRICE      (ID(13613), "value=%d\n", -2 ); //exp: time:feed3322default: value=-2
        break; case __LINE__: TRICE      (id(13614), "value=%d\n", -2 ); //exp: time:        default: value=-2
        break; case __LINE__: TRICE      (Id(13615), "value=%d\n", -2 ); //exp: time:    be16default: value=-2
        break; case __LINE__: TRICE      (ID(13616), "value=%d\n", -2 ); //exp: time:feed3322default: value=-2

        // default stamp
        break; case __LINE__: TRICE8     (ID(13617), "value=%d\n", -2 ); //exp: time:feed3322default: value=-2
        break; case __LINE__: TRICE8_1   (ID(13618), "value=%d\n", -2 ); //exp: time:feed3322default: value=-2

        // no stamp
        break; case __LINE__: trice8     (iD(13619), "value=%d\n", -2 ); //exp: time:        default: value=-2
        break; case __LINE__: trice8_1   (iD(13620), "value=%d\n", -2 ); //exp: time:        default: value=-2
        break; case __LINE__: TRICE8     (id(13621), "value=%d\n", -2 ); //exp: time:        default: value=-2
        break; case __LINE__: TRICE8_1   (id(13622), "value=%d\n", -2 ); //exp: time:        default: value=-2

        // 16-bit stamp:
        break; case __LINE__: Trice8     (iD(13623), "value=%d\n", -2 ); //exp: time:    be16default: value=-2
        break; case __LINE__: Trice8_1   (iD(13624), "value=%d\n", -2 ); //exp: time:    be16default: value=-2
        break; case __LINE__: TRICE8     (Id(13625), "value=%d\n", -2 ); //exp: time:    be16default: value=-2
        break; case __LINE__: TRICE8_1   (Id(13626), "value=%d\n", -2 ); //exp: time:    be16default: value=-2

        // 32-bit stamp:
        break; case __LINE__: TRice8     (iD(13627), "value=%d\n", -2 ); //exp: time:feed3322default: value=-2
        break; case __LINE__: TRice8_1   (iD(13628), "value=%d\n", -2 ); //exp: time:feed3322default: value=-2
        break; case __LINE__: TRICE8     (ID(13629), "value=%d\n", -2 ); //exp: time:feed3322default: value=-2
        break; case __LINE__: TRICE8_1   (ID(13630), "value=%d\n", -2 ); //exp: time:feed3322default: value=-2

#if TRICE_CGO == 1 || defined(TRICE_FULL_CHECK)
        // tags
        break; case __LINE__: TRICE(id(13631), "FATAL:magenta+b:red\n" );           //exp: time:        default: FATAL:magenta+b:red
        break; case __LINE__: TRICE(id(13632), "CRITICAL:red+i:default+h\n" );      //exp: time:        default: CRITICAL:red+i:default+h
        break; case __LINE__: TRICE(id(13633), "EMERGENCY:red+i:blue\n" );          //exp: time:        default: EMERGENCY:red+i:blue
        break; case __LINE__: TRICE(id(13634), "ERROR:11:red\n" );                  //exp: time:        default: ERROR:11:red
        break; case __LINE__: TRICE(id(13635), "WARNING:11+i:red\n" );              //exp: time:        default: WARNING:11+i:red
        break; case __LINE__: TRICE(id(13636), "ATTENTION:11:green\n" );            //exp: time:        default: ATTENTION:11:green
        break; case __LINE__: TRICE(id(13637), "INFO:cyan+b:default+h\n" );         //exp: time:        default: INFO:cyan+b:default+h
        break; case __LINE__: TRICE(id(13638), "DEBUG:130+i\n" );                   //exp: time:        default: DEBUG:130+i
        break; case __LINE__: TRICE(id(13639), "TRACE:default+i:default+h\n" );     //exp: time:        default: TRACE:default+i:default+h
        break; case __LINE__: TRICE(id(13640), "TIME:blue+i:blue+h\n" );            //exp: time:        default: TIME:blue+i:blue+h
        break; case __LINE__: TRICE(id(13641), "MESSAGE:green+h:black\n" );         //exp: time:        default: MESSAGE:green+h:black
        break; case __LINE__: TRICE(id(13642), "READ:black+i:yellow+h\n" );         //exp: time:        default: READ:black+i:yellow+h
        break; case __LINE__: TRICE(id(13643), "WRITE:black+u:yellow+h\n" );        //exp: time:        default: WRITE:black+u:yellow+h
        break; case __LINE__: TRICE(id(13644), "RECEIVE:black+h:black\n" );         //exp: time:        default: RECEIVE:black+h:black
        break; case __LINE__: TRICE(id(13645), "TRANSMIT:black:black+h\n" );        //exp: time:        default: TRANSMIT:black:black+h
        break; case __LINE__: TRICE(id(13646), "DIAG:yellow+i:default+h\n" );       //exp: time:        default: DIAG:yellow+i:default+h
        break; case __LINE__: TRICE(id(13647), "INTERRUPT:magenta+i:default+h\n" ); //exp: time:        default: INTERRUPT:magenta+i:default+h
        break; case __LINE__: TRICE(id(13648), "SIGNAL:118+i\n" );                  //exp: time:        default: SIGNAL:118+i
        break; case __LINE__: TRICE(id(13649), "TEST:yellow+h:black\n" );           //exp: time:        default: TEST:yellow+h:black
        break; case __LINE__: TRICE(id(13650), "DEFAULT:off\n" );                   //exp: time:        default: DEFAULT:off
        break; case __LINE__: TRICE(id(13651), "NOTICE:blue:white+h\n" );           //exp: time:        default: NOTICE:blue:white+h
        break; case __LINE__: TRICE(id(13652), "ALERT:magenta:magenta+h\n" );       //exp: time:        default: ALERT:magenta:magenta+h
        break; case __LINE__: TRICE(id(13653), "ASSERT:yellow+i:blue\n" );          //exp: time:        default: ASSERT:yellow+i:blue
        break; case __LINE__: TRICE(id(13654), "ALARM:red+i:white+h\n" );           //exp: time:        default: ALARM:red+i:white+h
        break; case __LINE__: TRICE(id(13655), "VERBOSE:blue:default\n" );          //exp: time:        default: VERBOSE:blue:default
        break; case __LINE__: TRICE(id(13656), "fatal:magenta+b:red\n" );           //exp: time:        default: fatal:magenta+b:red
        break; case __LINE__: TRICE(id(13657), "critical:red+i:default+h\n" );      //exp: time:        default: critical:red+i:default+h
        break; case __LINE__: TRICE(id(13658), "emergency:red+i:blue\n" );          //exp: time:        default: emergency:red+i:blue
        break; case __LINE__: TRICE(id(13659), "error:11:red\n" );                  //exp: time:        default: error:11:red
        break; case __LINE__: TRICE(id(13660), "warning:11+i:red\n" );              //exp: time:        default: warning:11+i:red
        break; case __LINE__: TRICE(id(13661), "attention:11:green\n" );            //exp: time:        default: attention:11:green
        break; case __LINE__: TRICE(id(13662), "info:cyan+b:default+h\n" );         //exp: time:        default: info:cyan+b:default+h
        break; case __LINE__: TRICE(id(13663), "debug:130+i\n" );                   //exp: time:        default: debug:130+i
        break; case __LINE__: TRICE(id(13664), "trace:default+i:default+h\n" );     //exp: time:        default: trace:default+i:default+h
        break; case __LINE__: TRICE(id(13665), "time:blue+i:blue+h\n" );            //exp: time:        default: time:blue+i:blue+h
        break; case __LINE__: TRICE(id(13666), "message:green+h:black\n" );         //exp: time:        default: message:green+h:black
        break; case __LINE__: TRICE(id(13667), "read:black+i:yellow+h\n" );         //exp: time:        default: read:black+i:yellow+h
        break; case __LINE__: TRICE(id(13668), "write:black+u:yellow+h\n" );        //exp: time:        default: write:black+u:yellow+h
        break; case __LINE__: TRICE(id(13669), "receive:black+h:black\n" );         //exp: time:        default: receive:black+h:black
        break; case __LINE__: TRICE(id(13670), "transmit:black:black+h\n" );        //exp: time:        default: transmit:black:black+h
        break; case __LINE__: TRICE(id(13671), "diag:yellow+i:default+h\n" );       //exp: time:        default: diag:yellow+i:default+h
        break; case __LINE__: TRICE(id(13672), "interrupt:magenta+i:default+h\n" ); //exp: time:        default: interrupt:magenta+i:default+h
        break; case __LINE__: TRICE(id(13673), "signal:118+i\n" );                  //exp: time:        default: signal:118+i
        break; case __LINE__: TRICE(id(13674), "test:yellow+h:black\n" );           //exp: time:        default: test:yellow+h:black
        break; case __LINE__: TRICE(id(13675), "default:off\n" );                   //exp: time:        default: default:off
        break; case __LINE__: TRICE(id(13676), "notice:blue:white+h\n" );           //exp: time:        default: notice:blue:white+h
        break; case __LINE__: TRICE(id(13677), "alert:magenta:magenta+h\n" );       //exp: time:        default: alert:magenta:magenta+h
        break; case __LINE__: TRICE(id(13678), "assert:yellow+i:blue\n" );          //exp: time:        default: assert:yellow+i:blue
        break; case __LINE__: TRICE(id(13679), "alarm:red+i:white+h\n" );           //exp: time:        default: alarm:red+i:white+h
        break; case __LINE__: TRICE(id(13680), "verbose:blue:default\n" );          //exp: time:        default: verbose:blue:default
#endif // #if TRICE_CGO == 1 || defined(TRICE_FULL_CHECK)
        break; case __LINE__: trice(iD(13681), "FATAL:magenta+b:red\n" );           //exp: time:        default: FATAL:magenta+b:red
        break; case __LINE__: trice(iD(13682), "CRITICAL:red+i:default+h\n" );      //exp: time:        default: CRITICAL:red+i:default+h
        break; case __LINE__: trice(iD(13683), "EMERGENCY:red+i:blue\n" );          //exp: time:        default: EMERGENCY:red+i:blue
        break; case __LINE__: trice(iD(13684), "ERROR:11:red\n" );                  //exp: time:        default: ERROR:11:red
        break; case __LINE__: trice(iD(13685), "WARNING:11+i:red\n" );              //exp: time:        default: WARNING:11+i:red
        break; case __LINE__: trice(iD(13686), "ATTENTION:11:green\n" );            //exp: time:        default: ATTENTION:11:green
        break; case __LINE__: trice(iD(13687), "INFO:cyan+b:default+h\n" );         //exp: time:        default: INFO:cyan+b:default+h
        break; case __LINE__: trice(iD(13688), "DEBUG:130+i\n" );                   //exp: time:        default: DEBUG:130+i
        break; case __LINE__: trice(iD(13689), "TRACE:default+i:default+h\n" );     //exp: time:        default: TRACE:default+i:default+h
        break; case __LINE__: trice(iD(13690), "TIME:blue+i:blue+h\n" );            //exp: time:        default: TIME:blue+i:blue+h
        break; case __LINE__: trice(iD(13691), "MESSAGE:green+h:black\n" );         //exp: time:        default: MESSAGE:green+h:black
        break; case __LINE__: trice(iD(13692), "READ:black+i:yellow+h\n" );         //exp: time:        default: READ:black+i:yellow+h
        break; case __LINE__: trice(iD(13693), "WRITE:black+u:yellow+h\n" );        //exp: time:        default: WRITE:black+u:yellow+h
        break; case __LINE__: trice(iD(13694), "RECEIVE:black+h:black\n" );         //exp: time:        default: RECEIVE:black+h:black
        break; case __LINE__: trice(iD(13695), "TRANSMIT:black:black+h\n" );        //exp: time:        default: TRANSMIT:black:black+h
        break; case __LINE__: trice(iD(13696), "DIAG:yellow+i:default+h\n" );       //exp: time:        default: DIAG:yellow+i:default+h
        break; case __LINE__: trice(iD(13697), "INTERRUPT:magenta+i:default+h\n" ); //exp: time:        default: INTERRUPT:magenta+i:default+h
        break; case __LINE__: trice(iD(13698), "SIGNAL:118+i\n" );                  //exp: time:        default: SIGNAL:118+i
        break; case __LINE__: trice(iD(13699), "TEST:yellow+h:black\n" );           //exp: time:        default: TEST:yellow+h:black
        break; case __LINE__: trice(iD(13700), "DEFAULT:off\n" );                   //exp: time:        default: DEFAULT:off
        break; case __LINE__: trice(iD(13701), "NOTICE:blue:white+h\n" );           //exp: time:        default: NOTICE:blue:white+h
        break; case __LINE__: trice(iD(13702), "ALERT:magenta:magenta+h\n" );       //exp: time:        default: ALERT:magenta:magenta+h
        break; case __LINE__: trice(iD(13703), "ASSERT:yellow+i:blue\n" );          //exp: time:        default: ASSERT:yellow+i:blue
        break; case __LINE__: trice(iD(13704), "ALARM:red+i:white+h\n" );           //exp: time:        default: ALARM:red+i:white+h
        break; case __LINE__: trice(iD(13705), "VERBOSE:blue:default\n" );          //exp: time:        default: VERBOSE:blue:default
        break; case __LINE__: trice(iD(13706), "fatal:magenta+b:red\n" );           //exp: time:        default: fatal:magenta+b:red
        break; case __LINE__: trice(iD(13707), "critical:red+i:default+h\n" );      //exp: time:        default: critical:red+i:default+h
        break; case __LINE__: trice(iD(13708), "emergency:red+i:blue\n" );          //exp: time:        default: emergency:red+i:blue
        break; case __LINE__: trice(iD(13709), "error:11:red\n" );                  //exp: time:        default: error:11:red
        break; case __LINE__: trice(iD(13710), "warning:11+i:red\n" );              //exp: time:        default: warning:11+i:red
        break; case __LINE__: trice(iD(13711), "attention:11:green\n" );            //exp: time:        default: attention:11:green
        break; case __LINE__: trice(iD(13712), "info:cyan+b:default+h\n" );         //exp: time:        default: info:cyan+b:default+h
        break; case __LINE__: trice(iD(13713), "debug:130+i\n" );                   //exp: time:        default: debug:130+i
        break; case __LINE__: trice(iD(13714), "trace:default+i:default+h\n" );     //exp: time:        default: trace:default+i:default+h
        break; case __LINE__: trice(iD(13715), "time:blue+i:blue+h\n" );            //exp: time:        default: time:blue+i:blue+h
        break; case __LINE__: trice(iD(13716), "message:green+h:black\n" );         //exp: time:        default: message:green+h:black
        break; case __LINE__: trice(iD(13717), "read:black+i:yellow+h\n" );         //exp: time:        default: read:black+i:yellow+h
        break; case __LINE__: trice(iD(13718), "write:black+u:yellow+h\n" );        //exp: time:        default: write:black+u:yellow+h
        break; case __LINE__: trice(iD(13719), "receive:black+h:black\n" );         //exp: time:        default: receive:black+h:black
        break; case __LINE__: trice(iD(13720), "transmit:black:black+h\n" );        //exp: time:        default: transmit:black:black+h
        break; case __LINE__: trice(iD(13721), "diag:yellow+i:default+h\n" );       //exp: time:        default: diag:yellow+i:default+h
        break; case __LINE__: trice(iD(13722), "interrupt:magenta+i:default+h\n" ); //exp: time:        default: interrupt:magenta+i:default+h
        break; case __LINE__: trice(iD(13723), "signal:118+i\n" );                  //exp: time:        default: signal:118+i
        break; case __LINE__: trice(iD(13724), "test:yellow+h:black\n" );           //exp: time:        default: test:yellow+h:black
        break; case __LINE__: trice(iD(13725), "default:off\n" );                   //exp: time:        default: default:off
        break; case __LINE__: trice(iD(13726), "notice:blue:white+h\n" );           //exp: time:        default: notice:blue:white+h
        break; case __LINE__: trice(iD(13727), "alert:magenta:magenta+h\n" );       //exp: time:        default: alert:magenta:magenta+h
        break; case __LINE__: trice(iD(13728), "assert:yellow+i:blue\n" );          //exp: time:        default: assert:yellow+i:blue
        break; case __LINE__: trice(iD(13729), "alarm:red+i:white+h\n" );           //exp: time:        default: alarm:red+i:white+h
        break; case __LINE__: trice(iD(13730), "verbose:blue:default\n" );          //exp: time:        default: verbose:blue:default
#if TRICE_CGO == 1 || defined(TRICE_FULL_CHECK)
        break; case __LINE__: TRICE(Id(13731), "FATAL:magenta+b:red\n" );           //exp: time:    be16default: FATAL:magenta+b:red
        break; case __LINE__: TRICE(Id(13732), "CRITICAL:red+i:default+h\n" );      //exp: time:    be16default: CRITICAL:red+i:default+h
        break; case __LINE__: TRICE(Id(13733), "EMERGENCY:red+i:blue\n" );          //exp: time:    be16default: EMERGENCY:red+i:blue
        break; case __LINE__: TRICE(Id(13734), "ERROR:11:red\n" );                  //exp: time:    be16default: ERROR:11:red
        break; case __LINE__: TRICE(Id(13735), "WARNING:11+i:red\n" );              //exp: time:    be16default: WARNING:11+i:red
        break; case __LINE__: TRICE(Id(13736), "ATTENTION:11:green\n" );            //exp: time:    be16default: ATTENTION:11:green
        break; case __LINE__: TRICE(Id(13737), "INFO:cyan+b:default+h\n" );         //exp: time:    be16default: INFO:cyan+b:default+h
        break; case __LINE__: TRICE(Id(13738), "DEBUG:130+i\n" );                   //exp: time:    be16default: DEBUG:130+i
        break; case __LINE__: TRICE(Id(13739), "TRACE:default+i:default+h\n" );     //exp: time:    be16default: TRACE:default+i:default+h
        break; case __LINE__: TRICE(Id(13740), "TIME:blue+i:blue+h\n" );            //exp: time:    be16default: TIME:blue+i:blue+h
        break; case __LINE__: TRICE(Id(13741), "MESSAGE:green+h:black\n" );         //exp: time:    be16default: MESSAGE:green+h:black
        break; case __LINE__: TRICE(Id(13742), "READ:black+i:yellow+h\n" );         //exp: time:    be16default: READ:black+i:yellow+h
        break; case __LINE__: TRICE(Id(13743), "WRITE:black+u:yellow+h\n" );        //exp: time:    be16default: WRITE:black+u:yellow+h
        break; case __LINE__: TRICE(Id(13744), "RECEIVE:black+h:black\n" );         //exp: time:    be16default: RECEIVE:black+h:black
        break; case __LINE__: TRICE(Id(13745), "TRANSMIT:black:black+h\n" );        //exp: time:    be16default: TRANSMIT:black:black+h
        break; case __LINE__: TRICE(Id(13746), "DIAG:yellow+i:default+h\n" );       //exp: time:    be16default: DIAG:yellow+i:default+h
        break; case __LINE__: TRICE(Id(13747), "INTERRUPT:magenta+i:default+h\n" ); //exp: time:    be16default: INTERRUPT:magenta+i:default+h
        break; case __LINE__: TRICE(Id(13748), "SIGNAL:118+i\n" );                  //exp: time:    be16default: SIGNAL:118+i
        break; case __LINE__: TRICE(Id(13749), "TEST:yellow+h:black\n" );           //exp: time:    be16default: TEST:yellow+h:black
        break; case __LINE__: TRICE(Id(13750), "DEFAULT:off\n" );                   //exp: time:    be16default: DEFAULT:off
        break; case __LINE__: TRICE(Id(13751), "NOTICE:blue:white+h\n" );           //exp: time:    be16default: NOTICE:blue:white+h
        break; case __LINE__: TRICE(Id(13752), "ALERT:magenta:magenta+h\n" );       //exp: time:    be16default: ALERT:magenta:magenta+h
        break; case __LINE__: TRICE(Id(13753), "ASSERT:yellow+i:blue\n" );          //exp: time:    be16default: ASSERT:yellow+i:blue
        break; case __LINE__: TRICE(Id(13754), "ALARM:red+i:white+h\n" );           //exp: time:    be16default: ALARM:red+i:white+h
        break; case __LINE__: TRICE(Id(13755), "VERBOSE:blue:default\n" );          //exp: time:    be16default: VERBOSE:blue:default
        break; case __LINE__: TRICE(Id(13756), "fatal:magenta+b:red\n" );           //exp: time:    be16default: fatal:magenta+b:red
        break; case __LINE__: TRICE(Id(13757), "critical:red+i:default+h\n" );      //exp: time:    be16default: critical:red+i:default+h
        break; case __LINE__: TRICE(Id(13758), "emergency:red+i:blue\n" );          //exp: time:    be16default: emergency:red+i:blue
        break; case __LINE__: TRICE(Id(13759), "error:11:red\n" );                  //exp: time:    be16default: error:11:red
        break; case __LINE__: TRICE(Id(13760), "warning:11+i:red\n" );              //exp: time:    be16default: warning:11+i:red
        break; case __LINE__: TRICE(Id(13761), "attention:11:green\n" );            //exp: time:    be16default: attention:11:green
        break; case __LINE__: TRICE(Id(13762), "info:cyan+b:default+h\n" );         //exp: time:    be16default: info:cyan+b:default+h
        break; case __LINE__: TRICE(Id(13763), "debug:130+i\n" );                   //exp: time:    be16default: debug:130+i
        break; case __LINE__: TRICE(Id(13764), "trace:default+i:default+h\n" );     //exp: time:    be16default: trace:default+i:default+h
        break; case __LINE__: TRICE(Id(13765), "time:blue+i:blue+h\n" );            //exp: time:    be16default: time:blue+i:blue+h
        break; case __LINE__: TRICE(Id(13766), "message:green+h:black\n" );         //exp: time:    be16default: message:green+h:black
        break; case __LINE__: TRICE(Id(13767), "read:black+i:yellow+h\n" );         //exp: time:    be16default: read:black+i:yellow+h
        break; case __LINE__: TRICE(Id(13768), "write:black+u:yellow+h\n" );        //exp: time:    be16default: write:black+u:yellow+h
        break; case __LINE__: TRICE(Id(13769), "receive:black+h:black\n" );         //exp: time:    be16default: receive:black+h:black
        break; case __LINE__: TRICE(Id(13770), "transmit:black:black+h\n" );        //exp: time:    be16default: transmit:black:black+h
        break; case __LINE__: TRICE(Id(13771), "diag:yellow+i:default+h\n" );       //exp: time:    be16default: diag:yellow+i:default+h
        break; case __LINE__: TRICE(Id(13772), "interrupt:magenta+i:default+h\n" ); //exp: time:    be16default: interrupt:magenta+i:default+h
        break; case __LINE__: TRICE(Id(13773), "signal:118+i\n" );                  //exp: time:    be16default: signal:118+i
        break; case __LINE__: TRICE(Id(13774), "test:yellow+h:black\n" );           //exp: time:    be16default: test:yellow+h:black
        break; case __LINE__: TRICE(Id(13775), "default:off\n" );                   //exp: time:    be16default: default:off
        break; case __LINE__: TRICE(Id(13776), "notice:blue:white+h\n" );           //exp: time:    be16default: notice:blue:white+h
        break; case __LINE__: TRICE(Id(13777), "alert:magenta:magenta+h\n" );       //exp: time:    be16default: alert:magenta:magenta+h
        break; case __LINE__: TRICE(Id(13778), "assert:yellow+i:blue\n" );          //exp: time:    be16default: assert:yellow+i:blue
        break; case __LINE__: TRICE(Id(13779), "alarm:red+i:white+h\n" );           //exp: time:    be16default: alarm:red+i:white+h
        break; case __LINE__: TRICE(Id(13780), "verbose:blue:default\n" );          //exp: time:    be16default: verbose:blue:default

        break; case __LINE__: Trice(iD(13781), "FATAL:magenta+b:red\n" );           //exp: time:    be16default: FATAL:magenta+b:red
        break; case __LINE__: Trice(iD(13782), "CRITICAL:red+i:default+h\n" );      //exp: time:    be16default: CRITICAL:red+i:default+h
        break; case __LINE__: Trice(iD(13783), "EMERGENCY:red+i:blue\n" );          //exp: time:    be16default: EMERGENCY:red+i:blue
        break; case __LINE__: Trice(iD(13784), "ERROR:11:red\n" );                  //exp: time:    be16default: ERROR:11:red
        break; case __LINE__: Trice(iD(13785), "WARNING:11+i:red\n" );              //exp: time:    be16default: WARNING:11+i:red
        break; case __LINE__: Trice(iD(13786), "ATTENTION:11:green\n" );            //exp: time:    be16default: ATTENTION:11:green
        break; case __LINE__: Trice(iD(13787), "INFO:cyan+b:default+h\n" );         //exp: time:    be16default: INFO:cyan+b:default+h
        break; case __LINE__: Trice(iD(13788), "DEBUG:130+i\n" );                   //exp: time:    be16default: DEBUG:130+i
        break; case __LINE__: Trice(iD(13789), "TRACE:default+i:default+h\n" );     //exp: time:    be16default: TRACE:default+i:default+h
        break; case __LINE__: Trice(iD(13790), "TIME:blue+i:blue+h\n" );            //exp: time:    be16default: TIME:blue+i:blue+h
        break; case __LINE__: Trice(iD(13791), "MESSAGE:green+h:black\n" );         //exp: time:    be16default: MESSAGE:green+h:black
        break; case __LINE__: Trice(iD(13792), "READ:black+i:yellow+h\n" );         //exp: time:    be16default: READ:black+i:yellow+h
        break; case __LINE__: Trice(iD(13793), "WRITE:black+u:yellow+h\n" );        //exp: time:    be16default: WRITE:black+u:yellow+h
        break; case __LINE__: Trice(iD(13794), "RECEIVE:black+h:black\n" );         //exp: time:    be16default: RECEIVE:black+h:black
        break; case __LINE__: Trice(iD(13795), "TRANSMIT:black:black+h\n" );        //exp: time:    be16default: TRANSMIT:black:black+h
        break; case __LINE__: Trice(iD(13796), "DIAG:yellow+i:default+h\n" );       //exp: time:    be16default: DIAG:yellow+i:default+h
        break; case __LINE__: Trice(iD(13797), "INTERRUPT:magenta+i:default+h\n" ); //exp: time:    be16default: INTERRUPT:magenta+i:default+h
        break; case __LINE__: Trice(iD(13798), "SIGNAL:118+i\n" );                  //exp: time:    be16default: SIGNAL:118+i
        break; case __LINE__: Trice(iD(13799), "TEST:yellow+h:black\n" );           //exp: time:    be16default: TEST:yellow+h:black
        break; case __LINE__: Trice(iD(13800), "DEFAULT:off\n" );                   //exp: time:    be16default: DEFAULT:off
        break; case __LINE__: Trice(iD(13801), "NOTICE:blue:white+h\n" );           //exp: time:    be16default: NOTICE:blue:white+h
        break; case __LINE__: Trice(iD(13802), "ALERT:magenta:magenta+h\n" );       //exp: time:    be16default: ALERT:magenta:magenta+h
        break; case __LINE__: Trice(iD(13803), "ASSERT:yellow+i:blue\n" );          //exp: time:    be16default: ASSERT:yellow+i:blue
        break; case __LINE__: Trice(iD(13804), "ALARM:red+i:white+h\n" );           //exp: time:    be16default: ALARM:red+i:white+h
        break; case __LINE__: Trice(iD(13805), "VERBOSE:blue:default\n" );          //exp: time:    be16default: VERBOSE:blue:default
        break; case __LINE__: Trice(iD(13806), "fatal:magenta+b:red\n" );           //exp: time:    be16default: fatal:magenta+b:red
        break; case __LINE__: Trice(iD(13807), "critical:red+i:default+h\n" );      //exp: time:    be16default: critical:red+i:default+h
        break; case __LINE__: Trice(iD(13808), "emergency:red+i:blue\n" );          //exp: time:    be16default: emergency:red+i:blue
        break; case __LINE__: Trice(iD(13809), "error:11:red\n" );                  //exp: time:    be16default: error:11:red
        break; case __LINE__: Trice(iD(13810), "warning:11+i:red\n" );              //exp: time:    be16default: warning:11+i:red
        break; case __LINE__: Trice(iD(13811), "attention:11:green\n" );            //exp: time:    be16default: attention:11:green
        break; case __LINE__: Trice(iD(13812), "info:cyan+b:default+h\n" );         //exp: time:    be16default: info:cyan+b:default+h
        break; case __LINE__: Trice(iD(13813), "debug:130+i\n" );                   //exp: time:    be16default: debug:130+i
        break; case __LINE__: Trice(iD(13814), "trace:default+i:default+h\n" );     //exp: time:    be16default: trace:default+i:default+h
        break; case __LINE__: Trice(iD(13815), "time:blue+i:blue+h\n" );            //exp: time:    be16default: time:blue+i:blue+h
        break; case __LINE__: Trice(iD(13816), "message:green+h:black\n" );         //exp: time:    be16default: message:green+h:black
        break; case __LINE__: Trice(iD(13817), "read:black+i:yellow+h\n" );         //exp: time:    be16default: read:black+i:yellow+h
        break; case __LINE__: Trice(iD(13818), "write:black+u:yellow+h\n" );        //exp: time:    be16default: write:black+u:yellow+h
        break; case __LINE__: Trice(iD(13819), "receive:black+h:black\n" );         //exp: time:    be16default: receive:black+h:black
        break; case __LINE__: Trice(iD(13820), "transmit:black:black+h\n" );        //exp: time:    be16default: transmit:black:black+h
        break; case __LINE__: Trice(iD(13821), "diag:yellow+i:default+h\n" );       //exp: time:    be16default: diag:yellow+i:default+h
        break; case __LINE__: Trice(iD(13822), "interrupt:magenta+i:default+h\n" ); //exp: time:    be16default: interrupt:magenta+i:default+h
        break; case __LINE__: Trice(iD(13823), "signal:118+i\n" );                  //exp: time:    be16default: signal:118+i
        break; case __LINE__: Trice(iD(13824), "test:yellow+h:black\n" );           //exp: time:    be16default: test:yellow+h:black
        break; case __LINE__: Trice(iD(13825), "default:off\n" );                   //exp: time:    be16default: default:off
        break; case __LINE__: Trice(iD(13826), "notice:blue:white+h\n" );           //exp: time:    be16default: notice:blue:white+h
        break; case __LINE__: Trice(iD(13827), "alert:magenta:magenta+h\n" );       //exp: time:    be16default: alert:magenta:magenta+h
        break; case __LINE__: Trice(iD(13828), "assert:yellow+i:blue\n" );          //exp: time:    be16default: assert:yellow+i:blue
        break; case __LINE__: Trice(iD(13829), "alarm:red+i:white+h\n" );           //exp: time:    be16default: alarm:red+i:white+h
        break; case __LINE__: Trice(iD(13830), "verbose:blue:default\n" );          //exp: time:    be16default: verbose:blue:default

        break; case __LINE__: TRICE(ID(13831), "FATAL:magenta+b:red\n" );           //exp: time:feed3322default: FATAL:magenta+b:red
        break; case __LINE__: TRICE(ID(13832), "CRITICAL:red+i:default+h\n" );      //exp: time:feed3322default: CRITICAL:red+i:default+h
        break; case __LINE__: TRICE(ID(13833), "EMERGENCY:red+i:blue\n" );          //exp: time:feed3322default: EMERGENCY:red+i:blue
        break; case __LINE__: TRICE(ID(13834), "ERROR:11:red\n" );                  //exp: time:feed3322default: ERROR:11:red
        break; case __LINE__: TRICE(ID(13835), "WARNING:11+i:red\n" );              //exp: time:feed3322default: WARNING:11+i:red
        break; case __LINE__: TRICE(ID(13836), "ATTENTION:11:green\n" );            //exp: time:feed3322default: ATTENTION:11:green
        break; case __LINE__: TRICE(ID(13837), "INFO:cyan+b:default+h\n" );         //exp: time:feed3322default: INFO:cyan+b:default+h
        break; case __LINE__: TRICE(ID(13838), "DEBUG:130+i\n" );                   //exp: time:feed3322default: DEBUG:130+i
        break; case __LINE__: TRICE(ID(13839), "TRACE:default+i:default+h\n" );     //exp: time:feed3322default: TRACE:default+i:default+h
        break; case __LINE__: TRICE(ID(13840), "TIME:blue+i:blue+h\n" );            //exp: time:feed3322default: TIME:blue+i:blue+h
        break; case __LINE__: TRICE(ID(13841), "MESSAGE:green+h:black\n" );         //exp: time:feed3322default: MESSAGE:green+h:black
        break; case __LINE__: TRICE(ID(13842), "READ:black+i:yellow+h\n" );         //exp: time:feed3322default: READ:black+i:yellow+h
        break; case __LINE__: TRICE(ID(13843), "WRITE:black+u:yellow+h\n" );        //exp: time:feed3322default: WRITE:black+u:yellow+h
        break; case __LINE__: TRICE(ID(13844), "RECEIVE:black+h:black\n" );         //exp: time:feed3322default: RECEIVE:black+h:black
        break; case __LINE__: TRICE(ID(13845), "TRANSMIT:black:black+h\n" );        //exp: time:feed3322default: TRANSMIT:black:black+h
        break; case __LINE__: TRICE(ID(13846), "DIAG:yellow+i:default+h\n" );       //exp: time:feed3322default: DIAG:yellow+i:default+h
        break; case __LINE__: TRICE(ID(13847), "INTERRUPT:magenta+i:default+h\n" ); //exp: time:feed3322default: INTERRUPT:magenta+i:default+h
        break; case __LINE__: TRICE(ID(13848), "SIGNAL:118+i\n" );                  //exp: time:feed3322default: SIGNAL:118+i
        break; case __LINE__: TRICE(ID(13849), "TEST:yellow+h:black\n" );           //exp: time:feed3322default: TEST:yellow+h:black
        break; case __LINE__: TRICE(ID(13850), "DEFAULT:off\n" );                   //exp: time:feed3322default: DEFAULT:off
        break; case __LINE__: TRICE(ID(13851), "NOTICE:blue:white+h\n" );           //exp: time:feed3322default: NOTICE:blue:white+h
        break; case __LINE__: TRICE(ID(13852), "ALERT:magenta:magenta+h\n" );       //exp: time:feed3322default: ALERT:magenta:magenta+h
        break; case __LINE__: TRICE(ID(13853), "ASSERT:yellow+i:blue\n" );          //exp: time:feed3322default: ASSERT:yellow+i:blue
        break; case __LINE__: TRICE(ID(13854), "ALARM:red+i:white+h\n" );           //exp: time:feed3322default: ALARM:red+i:white+h
        break; case __LINE__: TRICE(ID(13855), "VERBOSE:blue:default\n" );          //exp: time:feed3322default: VERBOSE:blue:default
        break; case __LINE__: TRICE(ID(13856), "fatal:magenta+b:red\n" );           //exp: time:feed3322default: fatal:magenta+b:red
        break; case __LINE__: TRICE(ID(13857), "critical:red+i:default+h\n" );      //exp: time:feed3322default: critical:red+i:default+h
        break; case __LINE__: TRICE(ID(13858), "emergency:red+i:blue\n" );          //exp: time:feed3322default: emergency:red+i:blue
        break; case __LINE__: TRICE(ID(13859), "error:11:red\n" );                  //exp: time:feed3322default: error:11:red
        break; case __LINE__: TRICE(ID(13860), "warning:11+i:red\n" );              //exp: time:feed3322default: warning:11+i:red
        break; case __LINE__: TRICE(ID(13861), "attention:11:green\n" );            //exp: time:feed3322default: attention:11:green
        break; case __LINE__: TRICE(ID(13862), "info:cyan+b:default+h\n" );         //exp: time:feed3322default: info:cyan+b:default+h
        break; case __LINE__: TRICE(ID(13863), "debug:130+i\n" );                   //exp: time:feed3322default: debug:130+i
        break; case __LINE__: TRICE(ID(13864), "trace:default+i:default+h\n" );     //exp: time:feed3322default: trace:default+i:default+h
        break; case __LINE__: TRICE(ID(13865), "time:blue+i:blue+h\n" );            //exp: time:feed3322default: time:blue+i:blue+h
        break; case __LINE__: TRICE(ID(13866), "message:green+h:black\n" );         //exp: time:feed3322default: message:green+h:black
        break; case __LINE__: TRICE(ID(13867), "read:black+i:yellow+h\n" );         //exp: time:feed3322default: read:black+i:yellow+h
        break; case __LINE__: TRICE(ID(13868), "write:black+u:yellow+h\n" );        //exp: time:feed3322default: write:black+u:yellow+h
        break; case __LINE__: TRICE(ID(13869), "receive:black+h:black\n" );         //exp: time:feed3322default: receive:black+h:black
        break; case __LINE__: TRICE(ID(13870), "transmit:black:black+h\n" );        //exp: time:feed3322default: transmit:black:black+h
        break; case __LINE__: TRICE(ID(13871), "diag:yellow+i:default+h\n" );       //exp: time:feed3322default: diag:yellow+i:default+h
        break; case __LINE__: TRICE(ID(13872), "interrupt:magenta+i:default+h\n" ); //exp: time:feed3322default: interrupt:magenta+i:default+h
        break; case __LINE__: TRICE(ID(13873), "signal:118+i\n" );                  //exp: time:feed3322default: signal:118+i
        break; case __LINE__: TRICE(ID(13874), "test:yellow+h:black\n" );           //exp: time:feed3322default: test:yellow+h:black
        break; case __LINE__: TRICE(ID(13875), "default:off\n" );                   //exp: time:feed3322default: default:off
        break; case __LINE__: TRICE(ID(13876), "notice:blue:white+h\n" );           //exp: time:feed3322default: notice:blue:white+h
        break; case __LINE__: TRICE(ID(13877), "alert:magenta:magenta+h\n" );       //exp: time:feed3322default: alert:magenta:magenta+h
        break; case __LINE__: TRICE(ID(13878), "assert:yellow+i:blue\n" );          //exp: time:feed3322default: assert:yellow+i:blue
        break; case __LINE__: TRICE(ID(13879), "alarm:red+i:white+h\n" );           //exp: time:feed3322default: alarm:red+i:white+h
        break; case __LINE__: TRICE(ID(13880), "verbose:blue:default\n" );          //exp: time:feed3322default: verbose:blue:default
        
        break; case __LINE__: TRice(iD(13881), "FATAL:magenta+b:red\n" );           //exp: time:feed3322default: FATAL:magenta+b:red
        break; case __LINE__: TRice(iD(13882), "CRITICAL:red+i:default+h\n" );      //exp: time:feed3322default: CRITICAL:red+i:default+h
        break; case __LINE__: TRice(iD(13883), "EMERGENCY:red+i:blue\n" );          //exp: time:feed3322default: EMERGENCY:red+i:blue
        break; case __LINE__: TRice(iD(13884), "ERROR:11:red\n" );                  //exp: time:feed3322default: ERROR:11:red
        break; case __LINE__: TRice(iD(13885), "WARNING:11+i:red\n" );              //exp: time:feed3322default: WARNING:11+i:red
        break; case __LINE__: TRice(iD(13886), "ATTENTION:11:green\n" );            //exp: time:feed3322default: ATTENTION:11:green
        break; case __LINE__: TRice(iD(13887), "INFO:cyan+b:default+h\n" );         //exp: time:feed3322default: INFO:cyan+b:default+h
        break; case __LINE__: TRice(iD(13888), "DEBUG:130+i\n" );                   //exp: time:feed3322default: DEBUG:130+i
        break; case __LINE__: TRice(iD(13889), "TRACE:default+i:default+h\n" );     //exp: time:feed3322default: TRACE:default+i:default+h
        break; case __LINE__: TRice(iD(13890), "TIME:blue+i:blue+h\n" );            //exp: time:feed3322default: TIME:blue+i:blue+h
        break; case __LINE__: TRice(iD(13891), "MESSAGE:green+h:black\n" );         //exp: time:feed3322default: MESSAGE:green+h:black
        break; case __LINE__: TRice(iD(13892), "READ:black+i:yellow+h\n" );         //exp: time:feed3322default: READ:black+i:yellow+h
        break; case __LINE__: TRice(iD(13893), "WRITE:black+u:yellow+h\n" );        //exp: time:feed3322default: WRITE:black+u:yellow+h
        break; case __LINE__: TRice(iD(13894), "RECEIVE:black+h:black\n" );         //exp: time:feed3322default: RECEIVE:black+h:black
        break; case __LINE__: TRice(iD(13895), "TRANSMIT:black:black+h\n" );        //exp: time:feed3322default: TRANSMIT:black:black+h
        break; case __LINE__: TRice(iD(13896), "DIAG:yellow+i:default+h\n" );       //exp: time:feed3322default: DIAG:yellow+i:default+h
        break; case __LINE__: TRice(iD(13897), "INTERRUPT:magenta+i:default+h\n" ); //exp: time:feed3322default: INTERRUPT:magenta+i:default+h
        break; case __LINE__: TRice(iD(13898), "SIGNAL:118+i\n" );                  //exp: time:feed3322default: SIGNAL:118+i
        break; case __LINE__: TRice(iD(13899), "TEST:yellow+h:black\n" );           //exp: time:feed3322default: TEST:yellow+h:black
        break; case __LINE__: TRice(iD(13900), "DEFAULT:off\n" );                   //exp: time:feed3322default: DEFAULT:off
        break; case __LINE__: TRice(iD(13901), "NOTICE:blue:white+h\n" );           //exp: time:feed3322default: NOTICE:blue:white+h
        break; case __LINE__: TRice(iD(13902), "ALERT:magenta:magenta+h\n" );       //exp: time:feed3322default: ALERT:magenta:magenta+h
        break; case __LINE__: TRice(iD(13903), "ASSERT:yellow+i:blue\n" );          //exp: time:feed3322default: ASSERT:yellow+i:blue
        break; case __LINE__: TRice(iD(13904), "ALARM:red+i:white+h\n" );           //exp: time:feed3322default: ALARM:red+i:white+h
        break; case __LINE__: TRice(iD(13905), "VERBOSE:blue:default\n" );          //exp: time:feed3322default: VERBOSE:blue:default
        break; case __LINE__: TRice(iD(13906), "fatal:magenta+b:red\n" );           //exp: time:feed3322default: fatal:magenta+b:red
        break; case __LINE__: TRice(iD(13907), "critical:red+i:default+h\n" );      //exp: time:feed3322default: critical:red+i:default+h
        break; case __LINE__: TRice(iD(13908), "emergency:red+i:blue\n" );          //exp: time:feed3322default: emergency:red+i:blue
        break; case __LINE__: TRice(iD(13909), "error:11:red\n" );                  //exp: time:feed3322default: error:11:red
        break; case __LINE__: TRice(iD(13910), "warning:11+i:red\n" );              //exp: time:feed3322default: warning:11+i:red
        break; case __LINE__: TRice(iD(13911), "attention:11:green\n" );            //exp: time:feed3322default: attention:11:green
        break; case __LINE__: TRice(iD(13912), "info:cyan+b:default+h\n" );         //exp: time:feed3322default: info:cyan+b:default+h
        break; case __LINE__: TRice(iD(13913), "debug:130+i\n" );                   //exp: time:feed3322default: debug:130+i
        break; case __LINE__: TRice(iD(13914), "trace:default+i:default+h\n" );     //exp: time:feed3322default: trace:default+i:default+h
        break; case __LINE__: TRice(iD(13915), "time:blue+i:blue+h\n" );            //exp: time:feed3322default: time:blue+i:blue+h
        break; case __LINE__: TRice(iD(13916), "message:green+h:black\n" );         //exp: time:feed3322default: message:green+h:black
        break; case __LINE__: TRice(iD(13917), "read:black+i:yellow+h\n" );         //exp: time:feed3322default: read:black+i:yellow+h
        break; case __LINE__: TRice(iD(13918), "write:black+u:yellow+h\n" );        //exp: time:feed3322default: write:black+u:yellow+h
        break; case __LINE__: TRice(iD(13919), "receive:black+h:black\n" );         //exp: time:feed3322default: receive:black+h:black
        break; case __LINE__: TRice(iD(13920), "transmit:black:black+h\n" );        //exp: time:feed3322default: transmit:black:black+h
        break; case __LINE__: TRice(iD(13921), "diag:yellow+i:default+h\n" );       //exp: time:feed3322default: diag:yellow+i:default+h
        break; case __LINE__: TRice(iD(13922), "interrupt:magenta+i:default+h\n" ); //exp: time:feed3322default: interrupt:magenta+i:default+h
        break; case __LINE__: TRice(iD(13923), "signal:118+i\n" );                  //exp: time:feed3322default: signal:118+i
        break; case __LINE__: TRice(iD(13924), "test:yellow+h:black\n" );           //exp: time:feed3322default: test:yellow+h:black
        break; case __LINE__: TRice(iD(13925), "default:off\n" );                   //exp: time:feed3322default: default:off
        break; case __LINE__: TRice(iD(13926), "notice:blue:white+h\n" );           //exp: time:feed3322default: notice:blue:white+h
        break; case __LINE__: TRice(iD(13927), "alert:magenta:magenta+h\n" );       //exp: time:feed3322default: alert:magenta:magenta+h
        break; case __LINE__: TRice(iD(13928), "assert:yellow+i:blue\n" );          //exp: time:feed3322default: assert:yellow+i:blue
        break; case __LINE__: TRice(iD(13929), "alarm:red+i:white+h\n" );           //exp: time:feed3322default: alarm:red+i:white+h
        break; case __LINE__: TRice(iD(13930), "verbose:blue:default\n" );          //exp: time:feed3322default: verbose:blue:default
#endif // #if TRICE_CGO == 1 || defined(TRICE_FULL_CHECK)
   
   
        break; case __LINE__: TRICE64(id(13931), "msg:%d (%%d)\n", -1 );            //exp: time:        default: msg:-1 (%d)

        break; case __LINE__: trice(iD(13932), "sig:TRICE8 with 1 to 12 values\n" );
        break; case __LINE__: trice(iD(13933), "rd:TRICE %d\n", -1 );                                                                                            //exp: time:        default: rd:TRICE -1     
        break; case __LINE__: trice(iD(13934), "rd:TRICE %d, %d\n", -1, -2 );                                                                                    //exp: time:        default: rd:TRICE -1, -2 
        break; case __LINE__: trice(iD(13935), "rd:TRICE %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: rd:TRICE -1, -2, -3       
        break; case __LINE__: trice(iD(13936), "rd:TRICE %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:        default: rd:TRICE -1, -2, -3, -4      
        break; case __LINE__: trice(iD(13937), "rd:TRICE %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5        
        break; case __LINE__: trice(iD(13938), "rd:TRICE %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice(iD(13939), "rd:TRICE %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice(iD(13940), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice(iD(13941), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice(iD(13942), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice(iD(13943), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice(iD(13944), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: trice(iD(13945), "rd:TRICE %d\n", -1 );                                                                                            //exp: time:        default: rd:TRICE -1     
        break; case __LINE__: trice(iD(13946), "rd:TRICE %d, %d\n", -1, -2 );                                                                                    //exp: time:        default: rd:TRICE -1, -2 
        break; case __LINE__: trice(iD(13947), "rd:TRICE %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: rd:TRICE -1, -2, -3       
        break; case __LINE__: trice(iD(13948), "rd:TRICE %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:        default: rd:TRICE -1, -2, -3, -4      
        break; case __LINE__: trice(iD(13949), "rd:TRICE %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5        
        break; case __LINE__: trice(iD(13950), "rd:TRICE %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice(iD(13951), "rd:TRICE %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice(iD(13952), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice(iD(13953), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice(iD(13954), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice(iD(13955), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice(iD(13956), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: trice(iD(13957), "rd:TRICE %d\n", -1 );                                                                                            //exp: time:        default: rd:TRICE -1     
        break; case __LINE__: trice(iD(13958), "rd:TRICE %d, %d\n", -1, -2 );                                                                                    //exp: time:        default: rd:TRICE -1, -2 
        break; case __LINE__: trice(iD(13959), "rd:TRICE %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: rd:TRICE -1, -2, -3       
        break; case __LINE__: trice(iD(13960), "rd:TRICE %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:        default: rd:TRICE -1, -2, -3, -4      
        break; case __LINE__: trice(iD(13961), "rd:TRICE %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5        
        break; case __LINE__: trice(iD(13962), "rd:TRICE %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice(iD(13963), "rd:TRICE %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice(iD(13964), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice(iD(13965), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice(iD(13966), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice(iD(13967), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice(iD(13968), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

#if TRICE_CGO == 1 || defined(TRICE_FULL_CHECK)
        break; case __LINE__: trice(iD(13969), "sig:TRICE_n with 1 to 12 values\n" );
        break; case __LINE__: TRICE_1 (id(13970), "rd:TRICE_1  %d\n", -1 );                                                                                            //exp: time:        default: rd:TRICE_1  -1     
        break; case __LINE__: TRICE_2 (id(13971), "rd:TRICE_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:        default: rd:TRICE_2  -1, -2 
        break; case __LINE__: TRICE_3 (id(13972), "rd:TRICE_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: rd:TRICE_3  -1, -2, -3       
        break; case __LINE__: TRICE_4 (id(13973), "rd:TRICE_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:        default: rd:TRICE_4  -1, -2, -3, -4      
        break; case __LINE__: TRICE_5 (id(13974), "rd:TRICE_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:        default: rd:TRICE_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE_6 (id(13975), "rd:TRICE_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: rd:TRICE_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE_7 (id(13976), "rd:TRICE_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:        default: rd:TRICE_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE_8 (id(13977), "rd:TRICE_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:        default: rd:TRICE_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE_9 (id(13978), "rd:TRICE_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: rd:TRICE_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE_10(id(13979), "rd:TRICE_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:        default: rd:TRICE_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE_11(id(13980), "rd:TRICE_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:        default: rd:TRICE_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE_12(id(13981), "rd:TRICE_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: rd:TRICE_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE_1 (Id(13982), "rd:TRICE_1  %d\n", -1 );                                                                                            //exp: time:    be16default: rd:TRICE_1  -1     
        break; case __LINE__: TRICE_2 (Id(13983), "rd:TRICE_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:TRICE_2  -1, -2 
        break; case __LINE__: TRICE_3 (Id(13984), "rd:TRICE_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:TRICE_3  -1, -2, -3       
        break; case __LINE__: TRICE_4 (Id(13985), "rd:TRICE_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:TRICE_4  -1, -2, -3, -4      
        break; case __LINE__: TRICE_5 (Id(13986), "rd:TRICE_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:TRICE_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE_6 (Id(13987), "rd:TRICE_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:TRICE_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE_7 (Id(13988), "rd:TRICE_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:TRICE_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE_8 (Id(13989), "rd:TRICE_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:TRICE_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE_9 (Id(13990), "rd:TRICE_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:TRICE_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE_10(Id(13991), "rd:TRICE_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:TRICE_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE_11(Id(13992), "rd:TRICE_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:TRICE_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE_12(Id(13993), "rd:TRICE_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:TRICE_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE_1 (ID(13994), "rd:TRICE_1  %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRICE_1  -1     
        break; case __LINE__: TRICE_2 (ID(13995), "rd:TRICE_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRICE_2  -1, -2 
        break; case __LINE__: TRICE_3 (ID(13996), "rd:TRICE_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRICE_3  -1, -2, -3       
        break; case __LINE__: TRICE_4 (ID(13997), "rd:TRICE_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRICE_4  -1, -2, -3, -4      
        break; case __LINE__: TRICE_5 (ID(13998), "rd:TRICE_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRICE_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE_6 (ID(13999), "rd:TRICE_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRICE_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE_7 (ID(14000), "rd:TRICE_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRICE_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE_8 (ID(14001), "rd:TRICE_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRICE_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE_9 (ID(14002), "rd:TRICE_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRICE_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE_10(ID(14003), "rd:TRICE_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRICE_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE_11(ID(14004), "rd:TRICE_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRICE_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE_12(ID(14005), "rd:TRICE_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRICE_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
#endif // #if TRICE_CGO == 1 || defined(TRICE_FULL_CHECK)

        break; case __LINE__: TRICE(ID(14006), "sig:trice_n with 1 to 12 values\n" );
        break; case __LINE__: trice_1 (iD(14007), "rd:trice_1  %d\n", -1 );                                                                                            //exp: time:        default: rd:trice_1  -1     
        break; case __LINE__: trice_2 (iD(14008), "rd:trice_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:        default: rd:trice_2  -1, -2 
        break; case __LINE__: trice_3 (iD(14009), "rd:trice_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: rd:trice_3  -1, -2, -3       
        break; case __LINE__: trice_4 (iD(14010), "rd:trice_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:        default: rd:trice_4  -1, -2, -3, -4      
        break; case __LINE__: trice_5 (iD(14011), "rd:trice_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:        default: rd:trice_5  -1, -2, -3, -4, -5        
        break; case __LINE__: trice_6 (iD(14012), "rd:trice_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: rd:trice_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice_7 (iD(14013), "rd:trice_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:        default: rd:trice_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice_8 (iD(14014), "rd:trice_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:        default: rd:trice_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice_9 (iD(14015), "rd:trice_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: rd:trice_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice_10(iD(14016), "rd:trice_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:        default: rd:trice_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice_11(iD(14017), "rd:trice_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:        default: rd:trice_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice_12(iD(14018), "rd:trice_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: rd:trice_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: Trice_1 (iD(14019), "rd:Trice_1  %d\n", -1 );                                                                                            //exp: time:    be16default: rd:Trice_1  -1     
        break; case __LINE__: Trice_2 (iD(14020), "rd:Trice_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:Trice_2  -1, -2 
        break; case __LINE__: Trice_3 (iD(14021), "rd:Trice_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:Trice_3  -1, -2, -3       
        break; case __LINE__: Trice_4 (iD(14022), "rd:Trice_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:Trice_4  -1, -2, -3, -4      
        break; case __LINE__: Trice_5 (iD(14023), "rd:Trice_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:Trice_5  -1, -2, -3, -4, -5        
        break; case __LINE__: Trice_6 (iD(14024), "rd:Trice_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:Trice_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: Trice_7 (iD(14025), "rd:Trice_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:Trice_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: Trice_8 (iD(14026), "rd:Trice_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:Trice_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: Trice_9 (iD(14027), "rd:Trice_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:Trice_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: Trice_10(iD(14028), "rd:Trice_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:Trice_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: Trice_11(iD(14029), "rd:Trice_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:Trice_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: Trice_12(iD(14030), "rd:Trice_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:Trice_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRice_1 (iD(14031), "rd:TRice_1  %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRice_1  -1     
        break; case __LINE__: TRice_2 (iD(14032), "rd:TRice_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRice_2  -1, -2 
        break; case __LINE__: TRice_3 (iD(14033), "rd:TRice_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRice_3  -1, -2, -3       
        break; case __LINE__: TRice_4 (iD(14034), "rd:TRice_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRice_4  -1, -2, -3, -4      
        break; case __LINE__: TRice_5 (iD(14035), "rd:TRice_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRice_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRice_6 (iD(14036), "rd:TRice_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRice_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRice_7 (iD(14037), "rd:TRice_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRice_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRice_8 (iD(14038), "rd:TRice_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRice_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRice_9 (iD(14039), "rd:TRice_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRice_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRice_10(iD(14040), "rd:TRice_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRice_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRice_11(iD(14041), "rd:TRice_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRice_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRice_12(iD(14042), "rd:TRice_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRice_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
#if TRICE_CGO == 1 || defined(TRICE_FULL_CHECK)
        break; case __LINE__: trice(iD(14043), "sig:TRICE8 with 1 to 12 values\n" );
        break; case __LINE__: TRICE8(id(14044), "rd:TRICE8 %d\n", -1 );                                                                                            //exp: time:        default: rd:TRICE8 -1     
        break; case __LINE__: TRICE8(id(14045), "rd:TRICE8 %d, %d\n", -1, -2 );                                                                                    //exp: time:        default: rd:TRICE8 -1, -2 
        break; case __LINE__: TRICE8(id(14046), "rd:TRICE8 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: rd:TRICE8 -1, -2, -3       
        break; case __LINE__: TRICE8(id(14047), "rd:TRICE8 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:        default: rd:TRICE8 -1, -2, -3, -4      
        break; case __LINE__: TRICE8(id(14048), "rd:TRICE8 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:        default: rd:TRICE8 -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE8(id(14049), "rd:TRICE8 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: rd:TRICE8 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE8(id(14050), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:        default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE8(id(14051), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:        default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE8(id(14052), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE8(id(14053), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:        default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE8(id(14054), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:        default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE8(id(14055), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE8(Id(14056), "rd:TRICE8 %d\n", -1 );                                                                                            //exp: time:    be16default: rd:TRICE8 -1     
        break; case __LINE__: TRICE8(Id(14057), "rd:TRICE8 %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:TRICE8 -1, -2 
        break; case __LINE__: TRICE8(Id(14058), "rd:TRICE8 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:TRICE8 -1, -2, -3       
        break; case __LINE__: TRICE8(Id(14059), "rd:TRICE8 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:TRICE8 -1, -2, -3, -4      
        break; case __LINE__: TRICE8(Id(14060), "rd:TRICE8 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:TRICE8 -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE8(Id(14061), "rd:TRICE8 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:TRICE8 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE8(Id(14062), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE8(Id(14063), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE8(Id(14064), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE8(Id(14065), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE8(Id(14066), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE8(Id(14067), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE8(ID(14068), "rd:TRICE8 %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRICE8 -1     
        break; case __LINE__: TRICE8(ID(14069), "rd:TRICE8 %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRICE8 -1, -2 
        break; case __LINE__: TRICE8(ID(14070), "rd:TRICE8 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRICE8 -1, -2, -3       
        break; case __LINE__: TRICE8(ID(14071), "rd:TRICE8 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRICE8 -1, -2, -3, -4      
        break; case __LINE__: TRICE8(ID(14072), "rd:TRICE8 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRICE8 -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE8(ID(14073), "rd:TRICE8 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRICE8 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE8(ID(14074), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE8(ID(14075), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE8(ID(14076), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE8(ID(14077), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE8(ID(14078), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE8(ID(14079), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: trice(iD(14080), "sig:TRICE8_n with 1 to 12 values\n" );
        break; case __LINE__: TRICE8_1 (id(14081), "rd:TRICE8_1  %d\n", -1 );                                                                                            //exp: time:        default: rd:TRICE8_1  -1     
        break; case __LINE__: TRICE8_2 (id(14082), "rd:TRICE8_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:        default: rd:TRICE8_2  -1, -2 
        break; case __LINE__: TRICE8_3 (id(14083), "rd:TRICE8_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: rd:TRICE8_3  -1, -2, -3       
        break; case __LINE__: TRICE8_4 (id(14084), "rd:TRICE8_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:        default: rd:TRICE8_4  -1, -2, -3, -4      
        break; case __LINE__: TRICE8_5 (id(14085), "rd:TRICE8_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:        default: rd:TRICE8_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE8_6 (id(14086), "rd:TRICE8_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: rd:TRICE8_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE8_7 (id(14087), "rd:TRICE8_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:        default: rd:TRICE8_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE8_8 (id(14088), "rd:TRICE8_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:        default: rd:TRICE8_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE8_9 (id(14089), "rd:TRICE8_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: rd:TRICE8_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE8_10(id(14090), "rd:TRICE8_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:        default: rd:TRICE8_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE8_11(id(14091), "rd:TRICE8_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:        default: rd:TRICE8_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE8_12(id(14092), "rd:TRICE8_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: rd:TRICE8_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE8_1 (Id(14093), "rd:TRICE8_1  %d\n", -1 );                                                                                            //exp: time:    be16default: rd:TRICE8_1  -1     
        break; case __LINE__: TRICE8_2 (Id(14094), "rd:TRICE8_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:TRICE8_2  -1, -2 
        break; case __LINE__: TRICE8_3 (Id(14095), "rd:TRICE8_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:TRICE8_3  -1, -2, -3       
        break; case __LINE__: TRICE8_4 (Id(14096), "rd:TRICE8_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:TRICE8_4  -1, -2, -3, -4      
        break; case __LINE__: TRICE8_5 (Id(14097), "rd:TRICE8_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:TRICE8_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE8_6 (Id(14098), "rd:TRICE8_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:TRICE8_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE8_7 (Id(14099), "rd:TRICE8_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:TRICE8_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE8_8 (Id(14100), "rd:TRICE8_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:TRICE8_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE8_9 (Id(14101), "rd:TRICE8_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:TRICE8_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE8_10(Id(14102), "rd:TRICE8_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:TRICE8_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE8_11(Id(14103), "rd:TRICE8_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:TRICE8_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE8_12(Id(14104), "rd:TRICE8_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:TRICE8_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE8_1 (ID(14105), "rd:TRICE8_1  %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRICE8_1  -1     
        break; case __LINE__: TRICE8_2 (ID(14106), "rd:TRICE8_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRICE8_2  -1, -2 
        break; case __LINE__: TRICE8_3 (ID(14107), "rd:TRICE8_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRICE8_3  -1, -2, -3       
        break; case __LINE__: TRICE8_4 (ID(14108), "rd:TRICE8_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRICE8_4  -1, -2, -3, -4      
        break; case __LINE__: TRICE8_5 (ID(14109), "rd:TRICE8_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRICE8_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE8_6 (ID(14110), "rd:TRICE8_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRICE8_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE8_7 (ID(14111), "rd:TRICE8_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRICE8_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE8_8 (ID(14112), "rd:TRICE8_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRICE8_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE8_9 (ID(14113), "rd:TRICE8_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRICE8_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE8_10(ID(14114), "rd:TRICE8_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRICE8_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE8_11(ID(14115), "rd:TRICE8_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRICE8_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE8_12(ID(14116), "rd:TRICE8_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRICE8_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
#endif // #if TRICE_CGO == 1 || defined(TRICE_FULL_CHECK)
        break; case __LINE__: trice(iD(14117), "sig:trice8 with 1 to 12 values\n" );
        break; case __LINE__: trice8(iD(14118), "rd:trice8 %d\n", -1 );                                                                                            //exp: time:        default: rd:trice8 -1     
        break; case __LINE__: trice8(iD(14119), "rd:trice8 %d, %d\n", -1, -2 );                                                                                    //exp: time:        default: rd:trice8 -1, -2 
        break; case __LINE__: trice8(iD(14120), "rd:trice8 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: rd:trice8 -1, -2, -3       
        break; case __LINE__: trice8(iD(14121), "rd:trice8 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:        default: rd:trice8 -1, -2, -3, -4      
        break; case __LINE__: trice8(iD(14122), "rd:trice8 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:        default: rd:trice8 -1, -2, -3, -4, -5        
        break; case __LINE__: trice8(iD(14123), "rd:trice8 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: rd:trice8 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice8(iD(14124), "rd:trice8 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:        default: rd:trice8 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice8(iD(14125), "rd:trice8 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:        default: rd:trice8 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice8(iD(14126), "rd:trice8 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: rd:trice8 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice8(iD(14127), "rd:trice8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:        default: rd:trice8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice8(iD(14128), "rd:trice8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:        default: rd:trice8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice8(iD(14129), "rd:trice8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: rd:trice8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: Trice8(iD(14130), "rd:Trice8 %d\n", -1 );                                                                                            //exp: time:    be16default: rd:Trice8 -1     
        break; case __LINE__: Trice8(iD(14131), "rd:Trice8 %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:Trice8 -1, -2 
        break; case __LINE__: Trice8(iD(14132), "rd:Trice8 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:Trice8 -1, -2, -3       
        break; case __LINE__: Trice8(iD(14133), "rd:Trice8 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:Trice8 -1, -2, -3, -4      
        break; case __LINE__: Trice8(iD(14134), "rd:Trice8 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:Trice8 -1, -2, -3, -4, -5        
        break; case __LINE__: Trice8(iD(14135), "rd:Trice8 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:Trice8 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: Trice8(iD(14136), "rd:Trice8 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:Trice8 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: Trice8(iD(14137), "rd:Trice8 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:Trice8 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: Trice8(iD(14138), "rd:Trice8 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:Trice8 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: Trice8(iD(14139), "rd:Trice8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:Trice8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: Trice8(iD(14140), "rd:Trice8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:Trice8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: Trice8(iD(14141), "rd:Trice8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:Trice8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRice8(iD(14142), "rd:TRice8 %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRice8 -1     
        break; case __LINE__: TRice8(iD(14143), "rd:TRice8 %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRice8 -1, -2 
        break; case __LINE__: TRice8(iD(14144), "rd:TRice8 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRice8 -1, -2, -3       
        break; case __LINE__: TRice8(iD(14145), "rd:TRice8 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRice8 -1, -2, -3, -4      
        break; case __LINE__: TRice8(iD(14146), "rd:TRice8 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRice8 -1, -2, -3, -4, -5        
        break; case __LINE__: TRice8(iD(14147), "rd:TRice8 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRice8 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRice8(iD(14148), "rd:TRice8 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRice8 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRice8(iD(14149), "rd:TRice8 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRice8 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRice8(iD(14150), "rd:TRice8 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRice8 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRice8(iD(14151), "rd:TRice8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRice8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRice8(iD(14152), "rd:TRice8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRice8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRice8(iD(14153), "rd:TRice8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRice8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: TRICE(ID(14154), "sig:trice8_n with 1 to 12 values\n" );
        break; case __LINE__: trice8_1 (iD(14155), "rd:trice8_1  %d\n", -1 );                                                                                            //exp: time:        default: rd:trice8_1  -1     
        break; case __LINE__: trice8_2 (iD(14156), "rd:trice8_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:        default: rd:trice8_2  -1, -2 
        break; case __LINE__: trice8_3 (iD(14157), "rd:trice8_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: rd:trice8_3  -1, -2, -3       
        break; case __LINE__: trice8_4 (iD(14158), "rd:trice8_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:        default: rd:trice8_4  -1, -2, -3, -4      
        break; case __LINE__: trice8_5 (iD(14159), "rd:trice8_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:        default: rd:trice8_5  -1, -2, -3, -4, -5        
        break; case __LINE__: trice8_6 (iD(14160), "rd:trice8_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: rd:trice8_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice8_7 (iD(14161), "rd:trice8_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:        default: rd:trice8_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice8_8 (iD(14162), "rd:trice8_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:        default: rd:trice8_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice8_9 (iD(14163), "rd:trice8_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: rd:trice8_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice8_10(iD(14164), "rd:trice8_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:        default: rd:trice8_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice8_11(iD(14165), "rd:trice8_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:        default: rd:trice8_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice8_12(iD(14166), "rd:trice8_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: rd:trice8_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: Trice8_1 (iD(14167), "rd:Trice8_1  %d\n", -1 );                                                                                            //exp: time:    be16default: rd:Trice8_1  -1     
        break; case __LINE__: Trice8_2 (iD(14168), "rd:Trice8_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:Trice8_2  -1, -2 
        break; case __LINE__: Trice8_3 (iD(14169), "rd:Trice8_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:Trice8_3  -1, -2, -3       
        break; case __LINE__: Trice8_4 (iD(14170), "rd:Trice8_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:Trice8_4  -1, -2, -3, -4      
        break; case __LINE__: Trice8_5 (iD(14171), "rd:Trice8_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:Trice8_5  -1, -2, -3, -4, -5        
        break; case __LINE__: Trice8_6 (iD(14172), "rd:Trice8_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:Trice8_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: Trice8_7 (iD(14173), "rd:Trice8_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:Trice8_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: Trice8_8 (iD(14174), "rd:Trice8_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:Trice8_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: Trice8_9 (iD(14175), "rd:Trice8_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:Trice8_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: Trice8_10(iD(14176), "rd:Trice8_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:Trice8_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: Trice8_11(iD(14177), "rd:Trice8_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:Trice8_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: Trice8_12(iD(14178), "rd:Trice8_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:Trice8_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRice8_1 (iD(14179), "rd:TRice8_1  %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRice8_1  -1     
        break; case __LINE__: TRice8_2 (iD(14180), "rd:TRice8_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRice8_2  -1, -2 
        break; case __LINE__: TRice8_3 (iD(14181), "rd:TRice8_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRice8_3  -1, -2, -3       
        break; case __LINE__: TRice8_4 (iD(14182), "rd:TRice8_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRice8_4  -1, -2, -3, -4      
        break; case __LINE__: TRice8_5 (iD(14183), "rd:TRice8_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRice8_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRice8_6 (iD(14184), "rd:TRice8_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRice8_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRice8_7 (iD(14185), "rd:TRice8_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRice8_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRice8_8 (iD(14186), "rd:TRice8_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRice8_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRice8_9 (iD(14187), "rd:TRice8_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRice8_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRice8_10(iD(14188), "rd:TRice8_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRice8_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRice8_11(iD(14189), "rd:TRice8_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRice8_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRice8_12(iD(14190), "rd:TRice8_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRice8_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: trice(iD(14191), "sig:TRICE16 with 1 to 12 values (line %d)\n", __LINE__ );
        break; case __LINE__: trice16(iD(14192), "rd:TRICE16 %d\n", -1 );                                                                                            //exp: time:        default: rd:TRICE16 -1     
        break; case __LINE__: trice16(iD(14193), "rd:TRICE16 %d, %d\n", -1, -2 );                                                                                    //exp: time:        default: rd:TRICE16 -1, -2 
        break; case __LINE__: trice16(iD(14194), "rd:TRICE16 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: rd:TRICE16 -1, -2, -3       
        break; case __LINE__: trice16(iD(14195), "rd:TRICE16 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:        default: rd:TRICE16 -1, -2, -3, -4      
        break; case __LINE__: trice16(iD(14196), "rd:TRICE16 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:        default: rd:TRICE16 -1, -2, -3, -4, -5        
        break; case __LINE__: trice16(iD(14197), "rd:TRICE16 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: rd:TRICE16 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice16(iD(14198), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:        default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice16(iD(14199), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:        default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice16(iD(14200), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice16(iD(14201), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:        default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice16(iD(14202), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:        default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice16(iD(14203), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: trice16(iD(14204), "rd:TRICE16 %d\n", -1 );                                                                                            //exp: time:        default: rd:TRICE16 -1     
        break; case __LINE__: trice16(iD(14205), "rd:TRICE16 %d, %d\n", -1, -2 );                                                                                    //exp: time:        default: rd:TRICE16 -1, -2 
        break; case __LINE__: trice16(iD(14206), "rd:TRICE16 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: rd:TRICE16 -1, -2, -3       
        break; case __LINE__: trice16(iD(14207), "rd:TRICE16 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:        default: rd:TRICE16 -1, -2, -3, -4      
        break; case __LINE__: trice16(iD(14208), "rd:TRICE16 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:        default: rd:TRICE16 -1, -2, -3, -4, -5        
        break; case __LINE__: trice16(iD(14209), "rd:TRICE16 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: rd:TRICE16 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice16(iD(14210), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:        default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice16(iD(14211), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:        default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice16(iD(14212), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice16(iD(14213), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:        default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice16(iD(14214), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:        default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice16(iD(14215), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: trice16(iD(14216), "rd:TRICE16 %d\n", -1 );                                                                                            //exp: time:        default: rd:TRICE16 -1     
        break; case __LINE__: trice16(iD(14217), "rd:TRICE16 %d, %d\n", -1, -2 );                                                                                    //exp: time:        default: rd:TRICE16 -1, -2 
        break; case __LINE__: trice16(iD(14218), "rd:TRICE16 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: rd:TRICE16 -1, -2, -3       
        break; case __LINE__: trice16(iD(14219), "rd:TRICE16 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:        default: rd:TRICE16 -1, -2, -3, -4      
        break; case __LINE__: trice16(iD(14220), "rd:TRICE16 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:        default: rd:TRICE16 -1, -2, -3, -4, -5        
        break; case __LINE__: trice16(iD(14221), "rd:TRICE16 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: rd:TRICE16 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice16(iD(14222), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:        default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice16(iD(14223), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:        default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice16(iD(14224), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice16(iD(14225), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:        default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice16(iD(14226), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:        default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice16(iD(14227), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
#if TRICE_CGO == 1 || defined(TRICE_FULL_CHECK)
        break; case __LINE__: trice     (iD(14228), "sig:TRICE16_n with 1 to 12 values (line %d)\n", __LINE__ );
        break; case __LINE__: TRICE16_1 (id(14229), "rd:TRICE16_1  %d\n", -1 );                                                                                            //exp: time:        default: rd:TRICE16_1  -1     
        break; case __LINE__: TRICE16_2 (id(14230), "rd:TRICE16_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:        default: rd:TRICE16_2  -1, -2 
        break; case __LINE__: TRICE16_3 (id(14231), "rd:TRICE16_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: rd:TRICE16_3  -1, -2, -3       
        break; case __LINE__: TRICE16_4 (id(14232), "rd:TRICE16_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:        default: rd:TRICE16_4  -1, -2, -3, -4      
        break; case __LINE__: TRICE16_5 (id(14233), "rd:TRICE16_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:        default: rd:TRICE16_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE16_6 (id(14234), "rd:TRICE16_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: rd:TRICE16_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE16_7 (id(14235), "rd:TRICE16_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:        default: rd:TRICE16_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE16_8 (id(14236), "rd:TRICE16_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:        default: rd:TRICE16_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE16_9 (id(14237), "rd:TRICE16_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: rd:TRICE16_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE16_10(id(14238), "rd:TRICE16_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:        default: rd:TRICE16_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE16_11(id(14239), "rd:TRICE16_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:        default: rd:TRICE16_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE16_12(id(14240), "rd:TRICE16_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: rd:TRICE16_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE16_1 (Id(14241), "rd:TRICE16_1  %d\n", -1 );                                                                                            //exp: time:    be16default: rd:TRICE16_1  -1     
        break; case __LINE__: TRICE16_2 (Id(14242), "rd:TRICE16_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:TRICE16_2  -1, -2 
        break; case __LINE__: TRICE16_3 (Id(14243), "rd:TRICE16_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:TRICE16_3  -1, -2, -3       
        break; case __LINE__: TRICE16_4 (Id(14244), "rd:TRICE16_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:TRICE16_4  -1, -2, -3, -4      
        break; case __LINE__: TRICE16_5 (Id(14245), "rd:TRICE16_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:TRICE16_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE16_6 (Id(14246), "rd:TRICE16_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:TRICE16_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE16_7 (Id(14247), "rd:TRICE16_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:TRICE16_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE16_8 (Id(14248), "rd:TRICE16_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:TRICE16_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE16_9 (Id(14249), "rd:TRICE16_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:TRICE16_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE16_10(Id(14250), "rd:TRICE16_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:TRICE16_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE16_11(Id(14251), "rd:TRICE16_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:TRICE16_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE16_12(Id(14252), "rd:TRICE16_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:TRICE16_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE16_1 (ID(14253), "rd:TRICE16_1  %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRICE16_1  -1     
        break; case __LINE__: TRICE16_2 (ID(14254), "rd:TRICE16_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRICE16_2  -1, -2 
        break; case __LINE__: TRICE16_3 (ID(14255), "rd:TRICE16_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRICE16_3  -1, -2, -3       
        break; case __LINE__: TRICE16_4 (ID(14256), "rd:TRICE16_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRICE16_4  -1, -2, -3, -4      
        break; case __LINE__: TRICE16_5 (ID(14257), "rd:TRICE16_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRICE16_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE16_6 (ID(14258), "rd:TRICE16_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRICE16_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE16_7 (ID(14259), "rd:TRICE16_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRICE16_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE16_8 (ID(14260), "rd:TRICE16_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRICE16_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE16_9 (ID(14261), "rd:TRICE16_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRICE16_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE16_10(ID(14262), "rd:TRICE16_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRICE16_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE16_11(ID(14263), "rd:TRICE16_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRICE16_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE16_12(ID(14264), "rd:TRICE16_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRICE16_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
#endif // #if TRICE_CGO == 1 || defined(TRICE_FULL_CHECK)
        break; case __LINE__: trice(iD(14265), "sig:trice16 with 1 to 12 values (line %d)\n", __LINE__ );
        break; case __LINE__: trice16(iD(14266), "rd:trice16 %d\n", -1 );                                                                                            //exp: time:        default: rd:trice16 -1     
        break; case __LINE__: trice16(iD(14267), "rd:trice16 %d, %d\n", -1, -2 );                                                                                    //exp: time:        default: rd:trice16 -1, -2 
        break; case __LINE__: trice16(iD(14268), "rd:trice16 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: rd:trice16 -1, -2, -3       
        break; case __LINE__: trice16(iD(14269), "rd:trice16 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:        default: rd:trice16 -1, -2, -3, -4      
        break; case __LINE__: trice16(iD(14270), "rd:trice16 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:        default: rd:trice16 -1, -2, -3, -4, -5        
        break; case __LINE__: trice16(iD(14271), "rd:trice16 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: rd:trice16 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice16(iD(14272), "rd:trice16 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:        default: rd:trice16 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice16(iD(14273), "rd:trice16 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:        default: rd:trice16 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice16(iD(14274), "rd:trice16 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: rd:trice16 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice16(iD(14275), "rd:trice16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:        default: rd:trice16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice16(iD(14276), "rd:trice16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:        default: rd:trice16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice16(iD(14277), "rd:trice16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: rd:trice16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: Trice16(iD(14278), "rd:Trice16 %d\n", -1 );                                                                                            //exp: time:    be16default: rd:Trice16 -1     
        break; case __LINE__: Trice16(iD(14279), "rd:Trice16 %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:Trice16 -1, -2 
        break; case __LINE__: Trice16(iD(14280), "rd:Trice16 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:Trice16 -1, -2, -3       
        break; case __LINE__: Trice16(iD(14281), "rd:Trice16 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:Trice16 -1, -2, -3, -4      
        break; case __LINE__: Trice16(iD(14282), "rd:Trice16 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:Trice16 -1, -2, -3, -4, -5        
        break; case __LINE__: Trice16(iD(14283), "rd:Trice16 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:Trice16 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: Trice16(iD(14284), "rd:Trice16 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:Trice16 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: Trice16(iD(14285), "rd:Trice16 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:Trice16 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: Trice16(iD(14286), "rd:Trice16 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:Trice16 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: Trice16(iD(14287), "rd:Trice16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:Trice16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: Trice16(iD(14288), "rd:Trice16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:Trice16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: Trice16(iD(14289), "rd:Trice16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:Trice16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRice16(iD(14290), "rd:TRice16 %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRice16 -1     
        break; case __LINE__: TRice16(iD(14291), "rd:TRice16 %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRice16 -1, -2 
        break; case __LINE__: TRice16(iD(14292), "rd:TRice16 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRice16 -1, -2, -3       
        break; case __LINE__: TRice16(iD(14293), "rd:TRice16 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRice16 -1, -2, -3, -4      
        break; case __LINE__: TRice16(iD(14294), "rd:TRice16 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRice16 -1, -2, -3, -4, -5        
        break; case __LINE__: TRice16(iD(14295), "rd:TRice16 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRice16 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRice16(iD(14296), "rd:TRice16 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRice16 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRice16(iD(14297), "rd:TRice16 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRice16 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRice16(iD(14298), "rd:TRice16 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRice16 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRice16(iD(14299), "rd:TRice16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRice16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRice16(iD(14300), "rd:TRice16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRice16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRice16(iD(14301), "rd:TRice16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRice16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: trice     (iD(14302), "sig:trice16_n with 1 to 12 values (line %d)\n", __LINE__ );
        break; case __LINE__: trice16_1 (iD(14303), "rd:trice16_1  %d\n", -1 );                                                                                            //exp: time:        default: rd:trice16_1  -1     
        break; case __LINE__: trice16_2 (iD(14304), "rd:trice16_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:        default: rd:trice16_2  -1, -2 
        break; case __LINE__: trice16_3 (iD(14305), "rd:trice16_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: rd:trice16_3  -1, -2, -3       
        break; case __LINE__: trice16_4 (iD(14306), "rd:trice16_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:        default: rd:trice16_4  -1, -2, -3, -4      
        break; case __LINE__: trice16_5 (iD(14307), "rd:trice16_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:        default: rd:trice16_5  -1, -2, -3, -4, -5        
        break; case __LINE__: trice16_6 (iD(14308), "rd:trice16_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: rd:trice16_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice16_7 (iD(14309), "rd:trice16_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:        default: rd:trice16_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice16_8 (iD(14310), "rd:trice16_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:        default: rd:trice16_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice16_9 (iD(14311), "rd:trice16_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: rd:trice16_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice16_10(iD(14312), "rd:trice16_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:        default: rd:trice16_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice16_11(iD(14313), "rd:trice16_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:        default: rd:trice16_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice16_12(iD(14314), "rd:trice16_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: rd:trice16_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: Trice16_1 (iD(14315), "rd:Trice16_1  %d\n", -1 );                                                                                            //exp: time:    be16default: rd:Trice16_1  -1     
        break; case __LINE__: Trice16_2 (iD(14316), "rd:Trice16_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:Trice16_2  -1, -2 
        break; case __LINE__: Trice16_3 (iD(14317), "rd:Trice16_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:Trice16_3  -1, -2, -3       
        break; case __LINE__: Trice16_4 (iD(14318), "rd:Trice16_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:Trice16_4  -1, -2, -3, -4      
        break; case __LINE__: Trice16_5 (iD(14319), "rd:Trice16_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:Trice16_5  -1, -2, -3, -4, -5        
        break; case __LINE__: Trice16_6 (iD(14320), "rd:Trice16_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:Trice16_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: Trice16_7 (iD(14321), "rd:Trice16_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:Trice16_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: Trice16_8 (iD(14322), "rd:Trice16_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:Trice16_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: Trice16_9 (iD(14323), "rd:Trice16_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:Trice16_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: Trice16_10(iD(14324), "rd:Trice16_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:Trice16_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: Trice16_11(iD(14325), "rd:Trice16_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:Trice16_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: Trice16_12(iD(14326), "rd:Trice16_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:Trice16_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRice16_1 (iD(14327), "rd:TRice16_1  %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRice16_1  -1     
        break; case __LINE__: TRice16_2 (iD(14328), "rd:TRice16_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRice16_2  -1, -2 
        break; case __LINE__: TRice16_3 (iD(14329), "rd:TRice16_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRice16_3  -1, -2, -3       
        break; case __LINE__: TRice16_4 (iD(14330), "rd:TRice16_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRice16_4  -1, -2, -3, -4      
        break; case __LINE__: TRice16_5 (iD(14331), "rd:TRice16_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRice16_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRice16_6 (iD(14332), "rd:TRice16_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRice16_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRice16_7 (iD(14333), "rd:TRice16_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRice16_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRice16_8 (iD(14334), "rd:TRice16_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRice16_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRice16_9 (iD(14335), "rd:TRice16_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRice16_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRice16_10(iD(14336), "rd:TRice16_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRice16_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRice16_11(iD(14337), "rd:TRice16_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRice16_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRice16_12(iD(14338), "rd:TRice16_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRice16_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
#if TRICE_CGO == 1 || defined(TRICE_FULL_CHECK)
        break; case __LINE__: trice  (iD(14339), "sig:TRICE32 with 1 to 12 values (line %d)\n", __LINE__ );
        break; case __LINE__: TRICE32(id(14340), "rd:TRICE32 %d\n", -1 );                                                                                            //exp: time:        default: rd:TRICE32 -1     
        break; case __LINE__: TRICE32(id(14341), "rd:TRICE32 %d, %d\n", -1, -2 );                                                                                    //exp: time:        default: rd:TRICE32 -1, -2 
        break; case __LINE__: TRICE32(id(14342), "rd:TRICE32 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: rd:TRICE32 -1, -2, -3       
        break; case __LINE__: TRICE32(id(14343), "rd:TRICE32 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:        default: rd:TRICE32 -1, -2, -3, -4      
        break; case __LINE__: TRICE32(id(14344), "rd:TRICE32 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:        default: rd:TRICE32 -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE32(id(14345), "rd:TRICE32 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: rd:TRICE32 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE32(id(14346), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:        default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE32(id(14347), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:        default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE32(id(14348), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE32(id(14349), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:        default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE32(id(14350), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:        default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE32(id(14351), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE32(Id(14352), "rd:TRICE32 %d\n", -1 );                                                                                            //exp: time:    be16default: rd:TRICE32 -1     
        break; case __LINE__: TRICE32(Id(14353), "rd:TRICE32 %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:TRICE32 -1, -2 
        break; case __LINE__: TRICE32(Id(14354), "rd:TRICE32 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:TRICE32 -1, -2, -3       
        break; case __LINE__: TRICE32(Id(14355), "rd:TRICE32 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:TRICE32 -1, -2, -3, -4      
        break; case __LINE__: TRICE32(Id(14356), "rd:TRICE32 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:TRICE32 -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE32(Id(14357), "rd:TRICE32 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:TRICE32 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE32(Id(14358), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE32(Id(14359), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE32(Id(14360), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE32(Id(14361), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE32(Id(14362), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE32(Id(14363), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE32(ID(14364), "rd:TRICE32 %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRICE32 -1     
        break; case __LINE__: TRICE32(ID(14365), "rd:TRICE32 %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRICE32 -1, -2 
        break; case __LINE__: TRICE32(ID(14366), "rd:TRICE32 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRICE32 -1, -2, -3       
        break; case __LINE__: TRICE32(ID(14367), "rd:TRICE32 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRICE32 -1, -2, -3, -4      
        break; case __LINE__: TRICE32(ID(14368), "rd:TRICE32 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRICE32 -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE32(ID(14369), "rd:TRICE32 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRICE32 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE32(ID(14370), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE32(ID(14371), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE32(ID(14372), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE32(ID(14373), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE32(ID(14374), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE32(ID(14375), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: trice     (iD(14376), "signal:TRICE32_n with 1 to 12 values (line %d)\n", __LINE__ );
        break; case __LINE__: TRICE32_1 (id(14377), "rd:TRICE32_1  %d\n", -1 );                                                                                            //exp: time:        default: rd:TRICE32_1  -1     
        break; case __LINE__: TRICE32_2 (id(14378), "rd:TRICE32_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:        default: rd:TRICE32_2  -1, -2 
        break; case __LINE__: TRICE32_3 (id(14379), "rd:TRICE32_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: rd:TRICE32_3  -1, -2, -3       
        break; case __LINE__: TRICE32_4 (id(14380), "rd:TRICE32_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:        default: rd:TRICE32_4  -1, -2, -3, -4      
        break; case __LINE__: TRICE32_5 (id(14381), "rd:TRICE32_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:        default: rd:TRICE32_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE32_6 (id(14382), "rd:TRICE32_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: rd:TRICE32_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE32_7 (id(14383), "rd:TRICE32_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:        default: rd:TRICE32_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE32_8 (id(14384), "rd:TRICE32_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:        default: rd:TRICE32_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE32_9 (id(14385), "rd:TRICE32_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: rd:TRICE32_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE32_10(id(14386), "rd:TRICE32_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:        default: rd:TRICE32_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE32_11(id(14387), "rd:TRICE32_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:        default: rd:TRICE32_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE32_12(id(14388), "rd:TRICE32_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: rd:TRICE32_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE32_1 (Id(14389), "rd:TRICE32_1  %d\n", -1 );                                                                                            //exp: time:    be16default: rd:TRICE32_1  -1     
        break; case __LINE__: TRICE32_2 (Id(14390), "rd:TRICE32_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:TRICE32_2  -1, -2 
        break; case __LINE__: TRICE32_3 (Id(14391), "rd:TRICE32_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:TRICE32_3  -1, -2, -3       
        break; case __LINE__: TRICE32_4 (Id(14392), "rd:TRICE32_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:TRICE32_4  -1, -2, -3, -4      
        break; case __LINE__: TRICE32_5 (Id(14393), "rd:TRICE32_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:TRICE32_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE32_6 (Id(14394), "rd:TRICE32_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:TRICE32_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE32_7 (Id(14395), "rd:TRICE32_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:TRICE32_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE32_8 (Id(14396), "rd:TRICE32_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:TRICE32_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE32_9 (Id(14397), "rd:TRICE32_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:TRICE32_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE32_10(Id(14398), "rd:TRICE32_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:TRICE32_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE32_11(Id(14399), "rd:TRICE32_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:TRICE32_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE32_12(Id(14400), "rd:TRICE32_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:TRICE32_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE32_1 (ID(14401), "rd:TRICE32_1  %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRICE32_1  -1     
        break; case __LINE__: TRICE32_2 (ID(14402), "rd:TRICE32_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRICE32_2  -1, -2 
        break; case __LINE__: TRICE32_3 (ID(14403), "rd:TRICE32_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRICE32_3  -1, -2, -3       
        break; case __LINE__: TRICE32_4 (ID(14404), "rd:TRICE32_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRICE32_4  -1, -2, -3, -4      
        break; case __LINE__: TRICE32_5 (ID(14405), "rd:TRICE32_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRICE32_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE32_6 (ID(14406), "rd:TRICE32_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRICE32_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE32_7 (ID(14407), "rd:TRICE32_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRICE32_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE32_8 (ID(14408), "rd:TRICE32_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRICE32_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE32_9 (ID(14409), "rd:TRICE32_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRICE32_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE32_10(ID(14410), "rd:TRICE32_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRICE32_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE32_11(ID(14411), "rd:TRICE32_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRICE32_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE32_12(ID(14412), "rd:TRICE32_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRICE32_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
#endif // #if TRICE_CGO == 1 || defined(TRICE_FULL_CHECK)
        break; case __LINE__: trice  (iD(14413), "sig:trice32 with 1 to 12 values (line %d)\n", __LINE__ );
        break; case __LINE__: trice32(iD(14414), "rd:trice32 %d\n", -1 );                                                                                            //exp: time:        default: rd:trice32 -1     
        break; case __LINE__: trice32(iD(14415), "rd:trice32 %d, %d\n", -1, -2 );                                                                                    //exp: time:        default: rd:trice32 -1, -2 
        break; case __LINE__: trice32(iD(14416), "rd:trice32 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: rd:trice32 -1, -2, -3       
        break; case __LINE__: trice32(iD(14417), "rd:trice32 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:        default: rd:trice32 -1, -2, -3, -4      
        break; case __LINE__: trice32(iD(14418), "rd:trice32 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:        default: rd:trice32 -1, -2, -3, -4, -5        
        break; case __LINE__: trice32(iD(14419), "rd:trice32 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: rd:trice32 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice32(iD(14420), "rd:trice32 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:        default: rd:trice32 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice32(iD(14421), "rd:trice32 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:        default: rd:trice32 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice32(iD(14422), "rd:trice32 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: rd:trice32 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice32(iD(14423), "rd:trice32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:        default: rd:trice32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice32(iD(14424), "rd:trice32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:        default: rd:trice32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice32(iD(14425), "rd:trice32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: rd:trice32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: Trice32(iD(14426), "rd:Trice32 %d\n", -1 );                                                                                            //exp: time:    be16default: rd:Trice32 -1     
        break; case __LINE__: Trice32(iD(14427), "rd:Trice32 %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:Trice32 -1, -2 
        break; case __LINE__: Trice32(iD(14428), "rd:Trice32 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:Trice32 -1, -2, -3       
        break; case __LINE__: Trice32(iD(14429), "rd:Trice32 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:Trice32 -1, -2, -3, -4      
        break; case __LINE__: Trice32(iD(14430), "rd:Trice32 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:Trice32 -1, -2, -3, -4, -5        
        break; case __LINE__: Trice32(iD(14431), "rd:Trice32 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:Trice32 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: Trice32(iD(14432), "rd:Trice32 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:Trice32 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: Trice32(iD(14433), "rd:Trice32 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:Trice32 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: Trice32(iD(14434), "rd:Trice32 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:Trice32 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: Trice32(iD(14435), "rd:Trice32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:Trice32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: Trice32(iD(14436), "rd:Trice32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:Trice32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: Trice32(iD(14437), "rd:Trice32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:Trice32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRice32(iD(14438), "rd:TRice32 %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRice32 -1     
        break; case __LINE__: TRice32(iD(14439), "rd:TRice32 %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRice32 -1, -2 
        break; case __LINE__: TRice32(iD(14440), "rd:TRice32 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRice32 -1, -2, -3       
        break; case __LINE__: TRice32(iD(14441), "rd:TRice32 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRice32 -1, -2, -3, -4      
        break; case __LINE__: TRice32(iD(14442), "rd:TRice32 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRice32 -1, -2, -3, -4, -5        
        break; case __LINE__: TRice32(iD(14443), "rd:TRice32 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRice32 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRice32(iD(14444), "rd:TRice32 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRice32 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRice32(iD(14445), "rd:TRice32 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRice32 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRice32(iD(14446), "rd:TRice32 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRice32 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRice32(iD(14447), "rd:TRice32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRice32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRice32(iD(14448), "rd:TRice32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRice32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRice32(iD(14449), "rd:TRice32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRice32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: trice     (iD(14450), "signal:trice32_n with 1 to 12 values (line %d)\n", __LINE__ );

        break; case __LINE__: trice32_1 (iD(14451), "rd:trice32_1  %d\n", -1 );                                                                                            //exp: time:        default: rd:trice32_1  -1     
        break; case __LINE__: trice32_2 (iD(14452), "rd:trice32_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:        default: rd:trice32_2  -1, -2 
        break; case __LINE__: trice32_3 (iD(14453), "rd:trice32_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: rd:trice32_3  -1, -2, -3       
        break; case __LINE__: trice32_4 (iD(14454), "rd:trice32_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:        default: rd:trice32_4  -1, -2, -3, -4      
        break; case __LINE__: trice32_5 (iD(14455), "rd:trice32_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:        default: rd:trice32_5  -1, -2, -3, -4, -5        
        break; case __LINE__: trice32_6 (iD(14456), "rd:trice32_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: rd:trice32_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice32_7 (iD(14457), "rd:trice32_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:        default: rd:trice32_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice32_8 (iD(14458), "rd:trice32_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:        default: rd:trice32_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice32_9 (iD(14459), "rd:trice32_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: rd:trice32_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice32_10(iD(14460), "rd:trice32_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:        default: rd:trice32_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice32_11(iD(14461), "rd:trice32_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:        default: rd:trice32_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice32_12(iD(14462), "rd:trice32_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: rd:trice32_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: Trice32_1 (iD(14463), "rd:Trice32_1  %d\n", -1 );                                                                                            //exp: time:    be16default: rd:Trice32_1  -1     
        break; case __LINE__: Trice32_2 (iD(14464), "rd:Trice32_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:Trice32_2  -1, -2 
        break; case __LINE__: Trice32_3 (iD(14465), "rd:Trice32_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:Trice32_3  -1, -2, -3       
        break; case __LINE__: Trice32_4 (iD(14466), "rd:Trice32_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:Trice32_4  -1, -2, -3, -4      
        break; case __LINE__: Trice32_5 (iD(14467), "rd:Trice32_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:Trice32_5  -1, -2, -3, -4, -5        
        break; case __LINE__: Trice32_6 (iD(14468), "rd:Trice32_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:Trice32_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: Trice32_7 (iD(14469), "rd:Trice32_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:Trice32_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: Trice32_8 (iD(14470), "rd:Trice32_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:Trice32_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: Trice32_9 (iD(14471), "rd:Trice32_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:Trice32_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: Trice32_10(iD(14472), "rd:Trice32_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:Trice32_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: Trice32_11(iD(14473), "rd:Trice32_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:Trice32_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: Trice32_12(iD(14474), "rd:Trice32_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:Trice32_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRice32_1 (iD(14475), "rd:TRice32_1  %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRice32_1  -1     
        break; case __LINE__: TRice32_2 (iD(14476), "rd:TRice32_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRice32_2  -1, -2 
        break; case __LINE__: TRice32_3 (iD(14477), "rd:TRice32_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRice32_3  -1, -2, -3       
        break; case __LINE__: TRice32_4 (iD(14478), "rd:TRice32_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRice32_4  -1, -2, -3, -4      
        break; case __LINE__: TRice32_5 (iD(14479), "rd:TRice32_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRice32_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRice32_6 (iD(14480), "rd:TRice32_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRice32_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRice32_7 (iD(14481), "rd:TRice32_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRice32_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRice32_8 (iD(14482), "rd:TRice32_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRice32_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRice32_9 (iD(14483), "rd:TRice32_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRice32_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRice32_10(iD(14484), "rd:TRice32_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRice32_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRice32_11(iD(14485), "rd:TRice32_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRice32_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRice32_12(iD(14486), "rd:TRice32_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRice32_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12
#if TRICE_CGO == 1 || defined(TRICE_FULL_CHECK)
        break; case __LINE__: trice  (iD(14487), "sig:TRICE64 with 1 to 12 values (line %d)\n", __LINE__ );
        break; case __LINE__: TRICE64(id(14488), "rd:TRICE64 %d\n", -1 );                                                                                            //exp: time:        default: rd:TRICE64 -1     
        break; case __LINE__: TRICE64(id(14489), "rd:TRICE64 %d, %d\n", -1, -2 );                                                                                    //exp: time:        default: rd:TRICE64 -1, -2 
        break; case __LINE__: TRICE64(id(14490), "rd:TRICE64 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: rd:TRICE64 -1, -2, -3       
        break; case __LINE__: TRICE64(id(14491), "rd:TRICE64 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:        default: rd:TRICE64 -1, -2, -3, -4      
        break; case __LINE__: TRICE64(id(14492), "rd:TRICE64 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:        default: rd:TRICE64 -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE64(id(14493), "rd:TRICE64 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: rd:TRICE64 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE64(id(14494), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:        default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE64(id(14495), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:        default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE64(id(14496), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE64(id(14497), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:        default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE64(id(14498), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:        default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE64(id(14499), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE64(Id(14500), "rd:TRICE64 %d\n", -1 );                                                                                            //exp: time:    be16default: rd:TRICE64 -1     
        break; case __LINE__: TRICE64(Id(14501), "rd:TRICE64 %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:TRICE64 -1, -2 
        break; case __LINE__: TRICE64(Id(14502), "rd:TRICE64 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:TRICE64 -1, -2, -3       
        break; case __LINE__: TRICE64(Id(14503), "rd:TRICE64 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:TRICE64 -1, -2, -3, -4      
        break; case __LINE__: TRICE64(Id(14504), "rd:TRICE64 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:TRICE64 -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE64(Id(14505), "rd:TRICE64 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:TRICE64 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE64(Id(14506), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE64(Id(14507), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE64(Id(14508), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE64(Id(14509), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE64(Id(14510), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE64(Id(14511), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE64(ID(14512), "rd:TRICE64 %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRICE64 -1     
        break; case __LINE__: TRICE64(ID(14513), "rd:TRICE64 %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRICE64 -1, -2 
        break; case __LINE__: TRICE64(ID(14514), "rd:TRICE64 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRICE64 -1, -2, -3       
        break; case __LINE__: TRICE64(ID(14515), "rd:TRICE64 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRICE64 -1, -2, -3, -4      
        break; case __LINE__: TRICE64(ID(14516), "rd:TRICE64 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRICE64 -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE64(ID(14517), "rd:TRICE64 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRICE64 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE64(ID(14518), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE64(ID(14519), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE64(ID(14520), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE64(ID(14521), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE64(ID(14522), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE64(ID(14523), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: trice     (iD(14524), "signal:TRICE64_n with 1 to 12 values (line %d)\n", __LINE__ );
        break; case __LINE__: TRICE64_1 (id(14525), "rd:TRICE64_1  %d\n", -1 );                                                                                            //exp: time:        default: rd:TRICE64_1  -1     
        break; case __LINE__: TRICE64_2 (id(14526), "rd:TRICE64_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:        default: rd:TRICE64_2  -1, -2 
        break; case __LINE__: TRICE64_3 (id(14527), "rd:TRICE64_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: rd:TRICE64_3  -1, -2, -3       
        break; case __LINE__: TRICE64_4 (id(14528), "rd:TRICE64_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:        default: rd:TRICE64_4  -1, -2, -3, -4      
        break; case __LINE__: TRICE64_5 (id(14529), "rd:TRICE64_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:        default: rd:TRICE64_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE64_6 (id(14530), "rd:TRICE64_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: rd:TRICE64_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE64_7 (id(14531), "rd:TRICE64_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:        default: rd:TRICE64_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE64_8 (id(14532), "rd:TRICE64_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:        default: rd:TRICE64_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE64_9 (id(14533), "rd:TRICE64_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: rd:TRICE64_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE64_10(id(14534), "rd:TRICE64_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:        default: rd:TRICE64_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE64_11(id(14535), "rd:TRICE64_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:        default: rd:TRICE64_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE64_12(id(14536), "rd:TRICE64_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: rd:TRICE64_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE64_1 (Id(14537), "rd:TRICE64_1  %d\n", -1 );                                                                                            //exp: time:    be16default: rd:TRICE64_1  -1     
        break; case __LINE__: TRICE64_2 (Id(14538), "rd:TRICE64_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:TRICE64_2  -1, -2 
        break; case __LINE__: TRICE64_3 (Id(14539), "rd:TRICE64_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:TRICE64_3  -1, -2, -3       
        break; case __LINE__: TRICE64_4 (Id(14540), "rd:TRICE64_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:TRICE64_4  -1, -2, -3, -4      
        break; case __LINE__: TRICE64_5 (Id(14541), "rd:TRICE64_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:TRICE64_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE64_6 (Id(14542), "rd:TRICE64_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:TRICE64_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE64_7 (Id(14543), "rd:TRICE64_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:TRICE64_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE64_8 (Id(14544), "rd:TRICE64_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:TRICE64_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE64_9 (Id(14545), "rd:TRICE64_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:TRICE64_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE64_10(Id(14546), "rd:TRICE64_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:TRICE64_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE64_11(Id(14547), "rd:TRICE64_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:TRICE64_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE64_12(Id(14548), "rd:TRICE64_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:TRICE64_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE64_1 (ID(14549), "rd:TRICE64_1  %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRICE64_1  -1     
        break; case __LINE__: TRICE64_2 (ID(14550), "rd:TRICE64_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRICE64_2  -1, -2 
        break; case __LINE__: TRICE64_3 (ID(14551), "rd:TRICE64_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRICE64_3  -1, -2, -3       
        break; case __LINE__: TRICE64_4 (ID(14552), "rd:TRICE64_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRICE64_4  -1, -2, -3, -4      
        break; case __LINE__: TRICE64_5 (ID(14553), "rd:TRICE64_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRICE64_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE64_6 (ID(14554), "rd:TRICE64_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRICE64_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE64_7 (ID(14555), "rd:TRICE64_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRICE64_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE64_8 (ID(14556), "rd:TRICE64_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRICE64_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE64_9 (ID(14557), "rd:TRICE64_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRICE64_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE64_10(ID(14558), "rd:TRICE64_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRICE64_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE64_11(ID(14559), "rd:TRICE64_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRICE64_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE64_12(ID(14560), "rd:TRICE64_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRICE64_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
#endif // #if TRICE_CGO == 1 || defined(TRICE_FULL_CHECK)
        break; case __LINE__: trice  (iD(14561), "sig:trice64 with 1 to 12 values (line %d)\n", __LINE__ );
        break; case __LINE__: trice64(iD(14562), "rd:trice64 %d\n", -1 );                                                                                            //exp: time:        default: rd:trice64 -1     
        break; case __LINE__: trice64(iD(14563), "rd:trice64 %d, %d\n", -1, -2 );                                                                                    //exp: time:        default: rd:trice64 -1, -2 
        break; case __LINE__: trice64(iD(14564), "rd:trice64 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: rd:trice64 -1, -2, -3       
        break; case __LINE__: trice64(iD(14565), "rd:trice64 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:        default: rd:trice64 -1, -2, -3, -4      
        break; case __LINE__: trice64(iD(14566), "rd:trice64 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:        default: rd:trice64 -1, -2, -3, -4, -5        
        break; case __LINE__: trice64(iD(14567), "rd:trice64 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: rd:trice64 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice64(iD(14568), "rd:trice64 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:        default: rd:trice64 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice64(iD(14569), "rd:trice64 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:        default: rd:trice64 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice64(iD(14570), "rd:trice64 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: rd:trice64 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice64(iD(14571), "rd:trice64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:        default: rd:trice64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice64(iD(14572), "rd:trice64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:        default: rd:trice64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice64(iD(14573), "rd:trice64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: rd:trice64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: Trice64(iD(14574), "rd:Trice64 %d\n", -1 );                                                                                            //exp: time:    be16default: rd:Trice64 -1     
        break; case __LINE__: Trice64(iD(14575), "rd:Trice64 %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:Trice64 -1, -2 
        break; case __LINE__: Trice64(iD(14576), "rd:Trice64 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:Trice64 -1, -2, -3       
        break; case __LINE__: Trice64(iD(14577), "rd:Trice64 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:Trice64 -1, -2, -3, -4      
        break; case __LINE__: Trice64(iD(14578), "rd:Trice64 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:Trice64 -1, -2, -3, -4, -5        
        break; case __LINE__: Trice64(iD(14579), "rd:Trice64 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:Trice64 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: Trice64(iD(14580), "rd:Trice64 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:Trice64 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: Trice64(iD(14581), "rd:Trice64 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:Trice64 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: Trice64(iD(14582), "rd:Trice64 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:Trice64 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: Trice64(iD(14583), "rd:Trice64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:Trice64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: Trice64(iD(14584), "rd:Trice64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:Trice64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: Trice64(iD(14585), "rd:Trice64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:Trice64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRice64(iD(14586), "rd:TRice64 %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRice64 -1     
        break; case __LINE__: TRice64(iD(14587), "rd:TRice64 %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRice64 -1, -2 
        break; case __LINE__: TRice64(iD(14588), "rd:TRice64 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRice64 -1, -2, -3       
        break; case __LINE__: TRice64(iD(14589), "rd:TRice64 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRice64 -1, -2, -3, -4      
        break; case __LINE__: TRice64(iD(14590), "rd:TRice64 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRice64 -1, -2, -3, -4, -5        
        break; case __LINE__: TRice64(iD(14591), "rd:TRice64 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRice64 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRice64(iD(14592), "rd:TRice64 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRice64 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRice64(iD(14593), "rd:TRice64 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRice64 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRice64(iD(14594), "rd:TRice64 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRice64 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRice64(iD(14595), "rd:TRice64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRice64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRice64(iD(14596), "rd:TRice64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRice64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRice64(iD(14597), "rd:TRice64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRice64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: trice     (iD(14598), "signal:trice64_n with 1 to 12 values (line %d)\n", __LINE__ );
        break; case __LINE__: trice64_1 (iD(14599), "rd:trice64_1  %d\n", -1 );                                                                                            //exp: time:        default: rd:trice64_1  -1     
        break; case __LINE__: trice64_2 (iD(14600), "rd:trice64_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:        default: rd:trice64_2  -1, -2 
        break; case __LINE__: trice64_3 (iD(14601), "rd:trice64_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: rd:trice64_3  -1, -2, -3       
        break; case __LINE__: trice64_4 (iD(14602), "rd:trice64_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:        default: rd:trice64_4  -1, -2, -3, -4      
        break; case __LINE__: trice64_5 (iD(14603), "rd:trice64_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:        default: rd:trice64_5  -1, -2, -3, -4, -5        
        break; case __LINE__: trice64_6 (iD(14604), "rd:trice64_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: rd:trice64_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice64_7 (iD(14605), "rd:trice64_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:        default: rd:trice64_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice64_8 (iD(14606), "rd:trice64_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:        default: rd:trice64_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice64_9 (iD(14607), "rd:trice64_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: rd:trice64_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice64_10(iD(14608), "rd:trice64_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:        default: rd:trice64_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice64_11(iD(14609), "rd:trice64_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:        default: rd:trice64_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice64_12(iD(14610), "rd:trice64_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: rd:trice64_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: Trice64_1 (iD(14611), "rd:Trice64_1  %d\n", -1 );                                                                                            //exp: time:    be16default: rd:Trice64_1  -1     
        break; case __LINE__: Trice64_2 (iD(14612), "rd:Trice64_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:Trice64_2  -1, -2 
        break; case __LINE__: Trice64_3 (iD(14613), "rd:Trice64_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:Trice64_3  -1, -2, -3       
        break; case __LINE__: Trice64_4 (iD(14614), "rd:Trice64_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:Trice64_4  -1, -2, -3, -4      
        break; case __LINE__: Trice64_5 (iD(14615), "rd:Trice64_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:Trice64_5  -1, -2, -3, -4, -5        
        break; case __LINE__: Trice64_6 (iD(14616), "rd:Trice64_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:Trice64_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: Trice64_7 (iD(14617), "rd:Trice64_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:Trice64_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: Trice64_8 (iD(14618), "rd:Trice64_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:Trice64_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: Trice64_9 (iD(14619), "rd:Trice64_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:Trice64_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: Trice64_10(iD(14620), "rd:Trice64_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:Trice64_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: Trice64_11(iD(14621), "rd:Trice64_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:Trice64_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: Trice64_12(iD(14622), "rd:Trice64_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:Trice64_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRice64_1 (iD(14623), "rd:TRice64_1  %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRice64_1  -1     
        break; case __LINE__: TRice64_2 (iD(14624), "rd:TRice64_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRice64_2  -1, -2 
        break; case __LINE__: TRice64_3 (iD(14625), "rd:TRice64_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRice64_3  -1, -2, -3       
        break; case __LINE__: TRice64_4 (iD(14626), "rd:TRice64_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRice64_4  -1, -2, -3, -4      
        break; case __LINE__: TRice64_5 (iD(14627), "rd:TRice64_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRice64_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRice64_6 (iD(14628), "rd:TRice64_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRice64_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRice64_7 (iD(14629), "rd:TRice64_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRice64_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRice64_8 (iD(14630), "rd:TRice64_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRice64_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRice64_9 (iD(14631), "rd:TRice64_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRice64_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRice64_10(iD(14632), "rd:TRice64_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRice64_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRice64_11(iD(14633), "rd:TRice64_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRice64_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRice32_12(iD(14634), "rd:TRice32_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRice32_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__:{
                                TRICE(Id(14635), "sig:Colors and ticks\n" );
                                TRICE(Id(14636), "--------------------------------------------------\n");
                                TRICE(Id(14637), "--------------------------------------------------\n");
                                TRICE(Id(14638), "dbg:12345 as 16bit is %016b\n", 12345);
                                TRICE(Id(14639), "--------------------------------------------------\n");
                                TRICE(Id(14640), "sig:This ASSERT error is just a demo and no real error:\n");
                                TRICE(Id(14641), "--------------------------------------------------\n");
                                TRICE(Id(14642), "ERR:error       message, SysTick is %6u\n", SYSTICKVAL);
                                TRICE(Id(14643), "WRN:warning     message, SysTick is %6u\n", SYSTICKVAL);
                                TRICE(Id(14644), "ATT:attention   message, SysTick is %6u\n", SYSTICKVAL);
                                TRICE(Id(14645), "DIA:diagnostics message, SysTick is %6u\n", SYSTICKVAL);
                                TRICE(Id(14646), "TIM:timing      message, SysTick is %6u\n", SYSTICKVAL);
                                TRICE(Id(14647), "DBG:debug       message, SysTick is %6u\n", SYSTICKVAL);
                                TRICE(Id(14648), "SIG:signal      message, SysTick is %6u\n", SYSTICKVAL);
                                TRICE(Id(14649), "RD:read         message, SysTick is %6u\n", SYSTICKVAL);
                                TRICE(Id(14650), "WR:write        message, SysTick is %6u\n", SYSTICKVAL);
                                TRICE(Id(14651), "ISR:interrupt   message, SysTick is %6u\n", SYSTICKVAL);
                                TRICE(Id(14652), "MSG:normal      message, SysTick is %6u\n", SYSTICKVAL);
                                TRICE(Id(14653), "INFO:informal   message, SysTick is %6u\n", SYSTICKVAL);
                                TRICE(Id(14654), "time:TRICE32_1  message, SysTick is %6u\n", SYSTICKVAL);
        }
#if TRICE_CGO == 1 || defined(TRICE_FULL_CHECK)
        break; case __LINE__: TRICE(ID(14655), "sig:Various ranges and formats\n" );
        break; case __LINE__: TRICE8 (ID(14656), "tst:TRICE8  %%03x -> %03x  %03x  %03x  %03x\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 (ID(14657), "tst:TRICE8   %%4d -> %4d %4d %4d %4d\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 (ID(14658), "tst:TRICE8   %%4u -> %4u %4u %4u %4u\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 (ID(14659), "tst:TRICE8   %%4o -> %4o %4o %4o %4o\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 (ID(14660), "tst:TRICE8   %%4O -> %4O %4O %4O %4O\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 (ID(14661), "tst:TRICE8   %%4X -> %4X %4X %4X %4X\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 (ID(14662), "tst:TRICE8   %%4x -> %4x %4x %4x %4x\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 (ID(14663), "tst:TRICE8   %%8b -> %8b %8b %8b %8b\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 (ID(14664), "tst:TRICE8  %%08b -> %08b %08b %08b %08b\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE16(ID(14665), "tst:TRICE16  %%05x ->   %05x   %05x   %05x   %05x\n", 1, 0x7fff, 0x8000, 0xffff);
        break; case __LINE__: TRICE16(ID(14666), "tst:TRICE16   %%6d ->  %6d  %6d  %6d  %6d\n",         1, 0x7fff, 0x8000, 0xffff);
        break; case __LINE__: TRICE16(ID(14667), "tst:TRICE16   %%7o -> %7o %7o %7o %7o\n",             1, 0x7fff, 0x8000, 0xffff);
        break; case __LINE__: TRICE32(ID(14668), "tst:TRICE32 %%09x ->      %09x      %09x       %09x     %09x\n", 1, 0x7fffffff, 0x80000000, 0xffffffff);
        break; case __LINE__: TRICE32(ID(14669), "tst:TRICE32 %%10d ->     %10d     %10d     %10d    %10x\n",      1, 0x7fffffff, 0x80000000, 0xffffffff);
        break; case __LINE__: TRICE32(ID(14670), "att:TRICE32 %40b\n", 0xAAAAAAAA);
        break; case __LINE__: TRICE64(ID(14671), "tst:TRICE64 %%09x ->      %09x      %09x       %09x     %09x\n", 1, 0x7fffffffffffffff, 0x8000000000000000, 0xffffffffffffffff);
        break; case __LINE__: TRICE64(ID(14672), "tst:TRICE64 %%10d ->     %10d     %10d     %10d    %10x\n",      1, 0x7fffffffffffffff, 0x8000000000000000, 0xffffffffffffffff);
        break; case __LINE__: TRICE64(ID(14673), "att:TRICE64 %70b\n", 0xAAAAAAAAAAAAAAAA);
        break; case __LINE__: TRICE(ID(14674), "sig:Legacy TRICE8\n" );
        break; case __LINE__: TRICE8_1(ID(14675), "tst:TRICE8_1 %02x\n", 0xA1);
        break; case __LINE__: TRICE8_2(ID(14676), "tst:TRICE8_2 %02x %02x\n", 0xA1, 0xA2);
        break; case __LINE__: TRICE8_3(ID(14677), "tst:TRICE8_3 %02x %02x %02x\n", 0xA1, 0xA2, 0xA3);
        break; case __LINE__: TRICE8_4(ID(14678), "tst:TRICE8_4 %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4);
        break; case __LINE__: TRICE8_5(ID(14679), "tst:TRICE8_5 %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5 );
        break; case __LINE__: TRICE8_6(ID(14680), "tst:TRICE8_6 %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6);
        break; case __LINE__: TRICE8_7(ID(14681), "tst:TRICE8_7 %02x %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7);
        break; case __LINE__: TRICE8_8(ID(14682), "tst:TRICE8_8 %02X %02X %02X %02X %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8); //exp: time:feed3322default: tst:TRICE8_8 A1 A2 A3 A4 a5 a6 a7 a8
        break; case __LINE__: {
                TRICE8_1(Id(14683), "tst:TRICE8_1 %d\n",                      -1);
                TRICE8_2(Id(14684), "tst:TRICE8_2 %d %d\n",                   -1, -2);
                TRICE8_3(Id(14685), "tst:TRICE8_3 %d %d %d\n",                -1, -2, -3);
                TRICE8_4(Id(14686), "tst:TRICE8_4 %d %d %d %d\n",             -1, -2, -3, -4);
                TRICE8_5(Id(14687), "tst:TRICE8_5 %d %d %d %d %d\n",          -1, -2, -3, -4, -5);
                TRICE8_6(Id(14688), "tst:TRICE8_6 %d %d %d %d %d %d\n",       -1, -2, -3, -4, -5, -6);
                TRICE8_7(Id(14689), "tst:TRICE8_7 %d %d %d %d %d %d %d\n",    -1, -2, -3, -4, -5, -6, -7);
                TRICE8_8(Id(14690), "tst:TRICE8_8 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8);
        }
        break; case __LINE__: TRICE8_1(ID(14691), "tst:TRICE8_1 %d\n", 1);
        break; case __LINE__: TRICE8_2(ID(14692), "tst:TRICE8_2 %d %d\n", 1, 2);
        break; case __LINE__: TRICE8_3(ID(14693), "tst:TRICE8_3 %d %d %d\n", 1, 2, 3);
        break; case __LINE__: TRICE8_4(ID(14694), "tst:TRICE8_4 %d %d %d %d\n", 1, 2, 3, 4);
        break; case __LINE__: TRICE8_5(ID(14695), "tst:TRICE8_5 %d %d %d %d %d\n", 1, 2, 3, 4, 5);
        break; case __LINE__: TRICE8_6(ID(14696), "tst:TRICE8_6 %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6);
        break; case __LINE__: TRICE8_7(ID(14697), "tst:TRICE8_7 %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7);
        break; case __LINE__: TRICE8_8(ID(14698), "tst:TRICE8_8 %d %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7, 8);
        break; case __LINE__: TRICE8_1(ID(14699), "tst:TRICE8_1 %u\n", 201);
        break; case __LINE__: TRICE8_2(ID(14700), "tst:TRICE8_2 %u %u\n", 201, 202);
        break; case __LINE__: TRICE8_3(ID(14701), "tst:TRICE8_3 %u %u %u\n", 201, 202, 203);
        break; case __LINE__: TRICE8_4(ID(14702), "tst:TRICE8_4 %u %u %u %u\n", 201, 202, 203, 204);
        break; case __LINE__: TRICE8_5(ID(14703), "tst:TRICE8_5 %u %u %u %u %u\n", 201, 202, 203, 204, 205);
        break; case __LINE__: TRICE8_6(ID(14704), "tst:TRICE8_6 %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206);
        break; case __LINE__: TRICE8_7(ID(14705), "tst:TRICE8_7 %u %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206, 207);
        break; case __LINE__: TRICE8_8(ID(14706), "tst:TRICE8_8 %u %u %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206, 207, 208);
        break; case __LINE__: TRICE8_8(ID(14707), "tst:TRICE8_1 %%d=%d, %%u=%u, 0x%%x=0x%x, 0x%%2x=0x%2x, 0x%%02x=0x%02x, 0x%%3x=0x%3x, 0x%%03x=0x%03x, %%b=%b\n", 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81);

        break; case __LINE__: TRICE    (ID(14708), "sig:Legacy TRICE16-64\n" );
        break; case __LINE__: {
                TRICE16_1(Id(14709), "tst:TRICE16_1 %d\n", -111);
                TRICE16_2(Id(14710), "tst:TRICE16_2 %d %d\n", -111, -222);
                TRICE16_3(Id(14711), "tst:TRICE16_3 %d %d %d\n", -111, -222, -333);
                TRICE16_4(Id(14712), "tst:TRICE16_4 %d %d %d %d\n", -111, -222, -333, -444);
        }
        break; case __LINE__: {
                TRICE32_1(Id(14713), "tst:TRICE32_1 %08x\n", 0x0123cafe);
                TRICE32_1(Id(14714), "tst:TRICE32_1 %d\n", -111);
                TRICE32_2(Id(14715), "tst:TRICE32_2 %x %x\n", -111, -222);
                TRICE32_2(Id(14716), "tst:TRICE32_2 %d %d\n", -111, -222);
                TRICE32_3(Id(14717), "tst:TRICE32_3 %x %x %x\n", -111, -222, -333);
                TRICE32_3(Id(14718), "tst:TRICE32_3 %d %d %d\n", -111, -222, -333);
                TRICE32_4(Id(14719), "tst:TRICE32_4 %x %x %x %x\n", -111, -222, -333, -444);
                TRICE32_4(Id(14720), "tst:TRICE32_4 %d %d %d %d\n", -111, -222, -333, -444);
        }
        break; case __LINE__: {
                TRICE64_1(Id(14721), "tst:TRICE64_1 %d\n", -111);
                TRICE64_2(Id(14722), "tst:TRICE64_2 %d %d\n", -111, -222);
                TRICE16_1(Id(14723), "tst:TRICE16_1 %u\n", 60001);
                TRICE16_2(Id(14724), "tst:TRICE16_2 %u %u\n", 60001, 60002);
                TRICE16_3(Id(14725), "tst:TRICE16_3 %u %u %u\n", 60001, 60002, 60003);
                TRICE16_4(Id(14726), "tst:TRICE16_4 %u %u %u %u\n", 60001, 60002, 60003, 60004);
        }
        break; case __LINE__: {
                TRICE32_1(Id(14727), "tst:TRICE32_1 %u\n", 4000000001);
                TRICE32_2(Id(14728), "tst:TRICE32_2 %u %u\n", 4000000001, 4000000002);
                TRICE32_3(Id(14729), "tst:TRICE32_3 %u %u %u\n", 4000000001, 4000000002, 4000000003);
                TRICE32_4(Id(14730), "tst:TRICE32_4 %u %u %u %u\n", 4000000001, 4000000002, 4000000003, 4000000004);
        }
        break; case __LINE__: {
                TRICE64_1(Id(14731), "tst:TRICE64_1 %x\n", -1);
                TRICE64_2(Id(14732), "tst:TRICE64_2 %x %x\n", -1, -2);
                TRICE64_1(Id(14733), "tst:TRICE64_1 %u\n", -1);
                TRICE64_2(Id(14734), "tst:TRICE64_2 %u %u\n", -1, -2);
        }
#endif // #if TRICE_CGO == 1 || defined(TRICE_FULL_CHECK)
        break; case __LINE__: trice(iD(14735), "sig:colored single letters and several TRICE macros in one line\n" );
        break; case __LINE__: {
                trice(iD(14736), "e:A");
                trice(iD(14737), "w:B");
                trice(iD(14738), "a:c");
                trice(iD(14739), "wr:d");
                trice(iD(14740), "rd:e\n");
                trice(iD(14741), "diag:f");
                trice(iD(14742), "d:G");
                trice(iD(14743), "t:H");
                trice(iD(14744), "time:i");
                trice(iD(14745), "message:J");
                trice(iD(14746), "dbg:k\n");
                trice(iD(14747), "1");
                trice(iD(14748), "2");
                trice(iD(14749), "3");
                trice(iD(14750), "4");
                trice(iD(14751), "e:7");
                trice(iD(14752), "m:12");
                trice(iD(14753), "m:123\n");
                trice(iD(14754), "e:A"); trice(iD(14755), "w:B"); trice(iD(14756), "a:c");
                trice(iD(14757), "wr:d"); trice(iD(14758), "rd:e\n"); trice(iD(14759), "diag:f");
        }
#if TRICE_CGO == 1 || defined(TRICE_FULL_CHECK)
        break; case __LINE__: trice(iD(14760), "sig:TRICE8 with variable param count 1 to 12\n" );
        break; case __LINE__: {
                TRICE8(Id(14761), "tst:TRICE8 %d\n", -1 );
                TRICE8(Id(14762), "tst:TRICE8 %d %d\n", -1, -2 );
                TRICE8(Id(14763), "tst:TRICE8 %d %d %d\n", -1, -2, -3 );
                TRICE8(Id(14764), "tst:TRICE8 %d %d %d %d\n", -1, -2, -3, -4 );
                TRICE8(Id(14765), "tst:TRICE8 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
                TRICE8(Id(14766), "tst:TRICE8 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
                TRICE8(Id(14767), "tst:TRICE8 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
                TRICE8(Id(14768), "tst:TRICE8 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
                TRICE8(Id(14769), "tst:TRICE8 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
                TRICE8(Id(14770), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
                TRICE8(Id(14771), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
                TRICE8(Id(14772), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
                TRICE8(Id(14773), "tst:TRICE8 %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );
        }
        break; case __LINE__: {
                TRICE8_1 (Id(14774), "tst:TRICE8_1  %d\n", -1 );
                TRICE8_2 (Id(14775), "tst:TRICE8_2  %d %d\n", -1, -2 );
                TRICE8_3 (Id(14776), "tst:TRICE8_3  %d %d %d\n", -1, -2, -3 );
                TRICE8_4 (Id(14777), "tst:TRICE8_4  %d %d %d %d\n", -1, -2, -3, -4 );
                TRICE8_5 (Id(14778), "tst:TRICE8_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
                TRICE8_6 (Id(14779), "tst:TRICE8_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
                TRICE8_7 (Id(14780), "tst:TRICE8_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
                TRICE8_8 (Id(14781), "tst:TRICE8_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
                TRICE8_9 (Id(14782), "tst:TRICE8_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
                TRICE8_10(Id(14783), "tst:TRICE8_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
                TRICE8_11(Id(14784), "tst:TRICE8_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
                TRICE8_12(Id(14785), "tst:TRICE8_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
                TRICE8   (Id(14786), "tst:TRICE8   %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );
                TRICE8_9 (Id(14787), "tst:TRICE8_9 %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );
        }
        break; case __LINE__: trice(iD(14788), "sig:TRICE16 with variable param count 1 to 12\n" );
        break; case __LINE__: {
                TRICE16(Id(14789), "tst:TRICE16 %d\n", -1 );
                TRICE16(Id(14790), "tst:TRICE16 %d %d\n", -1, -2 );
                TRICE16(Id(14791), "tst:TRICE16 %d %d %d\n", -1, -2, -3 );
                TRICE16(Id(14792), "tst:TRICE16 %d %d %d %d\n", -1, -2, -3, -4 );
                TRICE16(Id(14793), "tst:TRICE16 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
                TRICE16(Id(14794), "tst:TRICE16 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
                TRICE16(Id(14795), "tst:TRICE16 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
                TRICE16(Id(14796), "tst:TRICE16 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
                TRICE16(Id(14797), "tst:TRICE16 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
                TRICE16(Id(14798), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
                TRICE16(Id(14799), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
                TRICE16(Id(14800), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
                TRICE16(Id(14801), "tst:TRICE16 %t %b %x %X %d %u %o %O %p\n", 3, 3, 3, 3, 3, 3, 3, 3, 3 );
        }
        break; case __LINE__: {
                TRICE16_1 (ID(14802), "tst:TRICE16_1  %d\n", -1 );
                TRICE16_2 (ID(14803), "tst:TRICE16_2  %d %d\n", -1, -2 );
                TRICE16_3 (ID(14804), "tst:TRICE16_3  %d %d %d\n", -1, -2, -3 );
                TRICE16_4 (ID(14805), "tst:TRICE16_4  %d %d %d %d\n", -1, -2, -3, -4 );
                TRICE16_5 (ID(14806), "tst:TRICE16_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
                TRICE16_6 (ID(14807), "tst:TRICE16_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
                TRICE16_7 (ID(14808), "tst:TRICE16_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
                TRICE16_8 (ID(14809), "tst:TRICE16_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
                TRICE16_9 (ID(14810), "tst:TRICE16_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
                TRICE16_10(ID(14811), "tst:TRICE16_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
                TRICE16_11(ID(14812), "tst:TRICE16_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
                TRICE16_12(ID(14813), "tst:TRICE16_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
                TRICE16_9 (ID(14814), "tst:TRICE16 %t %b %x %X %d %u %o %O %p\n", 3, 3, 3, 3, 3, 3, 3, 3, 3 );
        }
        break; case __LINE__: trice(iD(14815), "sig:TRICE32 with variable param count 1 to 12\n" );
        break; case __LINE__: {
                TRICE32(Id(14816), "tst:TRICE32 %d\n", -1 );
                TRICE32(Id(14817), "tst:TRICE32 %d %d\n", -1, -2 );
                TRICE32(Id(14818), "tst:TRICE32 %d %d %d\n", -1, -2, -3 );
                TRICE32(Id(14819), "tst:TRICE32 %d %d %d %d\n", -1, -2, -3, -4 );
                TRICE32(Id(14820), "tst:TRICE32 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
                TRICE32(Id(14821), "tst:TRICE32 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
                TRICE32(Id(14822), "tst:TRICE32 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
                TRICE32(Id(14823), "tst:TRICE32 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
                TRICE32(Id(14824), "tst:TRICE32 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
                TRICE32(Id(14825), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
                TRICE32(Id(14826), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
                TRICE32(Id(14827), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
                TRICE32(Id(14828), "tst:TRICE32 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));
        }
        break; case __LINE__: {
                TRICE32_1 (Id(14829), "tst:TRICE32_1  %d\n", -1 );
                TRICE32_2 (Id(14830), "tst:TRICE32_2  %d %d\n", -1, -2 );
                TRICE32_3 (Id(14831), "tst:TRICE32_3  %d %d %d\n", -1, -2, -3 );
                TRICE32_4 (Id(14832), "tst:TRICE32_4  %d %d %d %d\n", -1, -2, -3, -4 );
                TRICE32_5 (Id(14833), "tst:TRICE32_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
                TRICE32_6 (Id(14834), "tst:TRICE32_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
                TRICE32_7 (Id(14835), "tst:TRICE32_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
                TRICE32_8 (Id(14836), "tst:TRICE32_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
                TRICE32_9 (Id(14837), "tst:TRICE32_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
                TRICE32_10(Id(14838), "tst:TRICE32_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
                TRICE32_11(Id(14839), "tst:TRICE32_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
                TRICE32_12(Id(14840), "tst:TRICE32_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
                TRICE32   (Id(14841), "tst:TRICE32    %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));
                TRICE32_12(Id(14842), "tst:TRICE32_12 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));
        }
        break; case __LINE__: trice(iD(14843), "sig:TRICE64 with variable param count 1 to 12\n" );
        break; case __LINE__: TRICE64(ID(14844), "tst:TRICE64 %d\n", -1 );
        break; case __LINE__: TRICE64(ID(14845), "tst:TRICE64 %d %d\n", -1, -2 );
        break; case __LINE__: TRICE64(ID(14846), "tst:TRICE64 %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE64(ID(14847), "tst:TRICE64 %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE64(ID(14848), "tst:TRICE64 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE64(ID(14849), "tst:TRICE64 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE64(ID(14850), "tst:TRICE64 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE64(ID(14851), "tst:TRICE64 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE64(ID(14852), "tst:TRICE64 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE64(ID(14853), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE64(ID(14854), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE64(ID(14855), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE64(ID(14856), "tst:TRICE64 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aDouble(3.14159), aDouble(3.14159), aDouble(3.14159));
        break; case __LINE__: TRICE64_1 (ID(14857), "tst:TRICE64_1  %d\n", -1 );
        break; case __LINE__: TRICE64_2 (ID(14858), "tst:TRICE64_2  %d %d\n", -1, -2 );
        break; case __LINE__: TRICE64_3 (ID(14859), "tst:TRICE64_3  %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE64_4 (ID(14860), "tst:TRICE64_4  %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE64_5 (ID(14861), "tst:TRICE64_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE64_6 (ID(14862), "tst:TRICE64_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE64_7 (ID(14863), "tst:TRICE64_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE64_8 (ID(14864), "tst:TRICE64_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE64_9 (ID(14865), "tst:TRICE64_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE64_10(ID(14866), "tst:TRICE64_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE64_11(ID(14867), "tst:TRICE64_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE64_12(ID(14868), "tst:TRICE64_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE64_12(ID(14869), "tst:TRICE64_12 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aDouble(3.14159), aDouble(3.14159), aDouble(3.14159));
#endif // #if TRICE_CGO == 1 || defined(TRICE_FULL_CHECK)
        break; case __LINE__: trice(iD(14870), "att:positive and negative float in format variants\n");
        break; case __LINE__: {
                trice(iD(14871), "rd:TRICE float %f (%%f)\n", aFloat(x) );
                trice(iD(14872), "rd:TRICE float %9f (%%9f)\n", aFloat(x) );
                trice(iD(14873), "rd:TRICE float %.9f (%%.9f)\n", aFloat(x) );
                trice(iD(14874), "rd:TRICE float %9.f (%%9.f)\n", aFloat(x) );
                trice(iD(14875), "rd:TRICE float %9.6f (%%9.6f)\n", aFloat(x) );
                trice(iD(14876), "rd:TRICE float %f (%%f)\n", aFloat(-x) );
                trice(iD(14877), "rd:TRICE float %9f (%%9f)\n", aFloat(-x) );
                trice(iD(14878), "rd:TRICE float %.9f (%%.9f)\n", aFloat(-x) );
                trice(iD(14879), "rd:TRICE float %9.f (%%9.f)\n", aFloat(-x) );
                trice(iD(14880), "rd:TRICE float %9.6f (%%9.6f)\n", aFloat(-x) );
                trice(iD(14881), "rd:TRICE float %+f (%%f)\n", aFloat(x) );
                trice(iD(14882), "rd:TRICE float %+9f (%%9f)\n", aFloat(x) );
                trice(iD(14883), "rd:TRICE float %+.9f (%%.9f)\n", aFloat(x) );
                trice(iD(14884), "rd:TRICE float %+9.f (%%9.f)\n", aFloat(x) );
                trice(iD(14885), "rd:TRICE float %+9.6f (%%9.6f)\n", aFloat(x) );
                trice(iD(14886), "rd:TRICE float %+f (%%f)\n", aFloat(-x) );
                trice(iD(14887), "rd:TRICE float %+9f (%%9f)\n", aFloat(-x) );
                trice(iD(14888), "rd:TRICE float %+.9f (%%.9f)\n", aFloat(-x) );
                trice(iD(14889), "rd:TRICE float %+9.f (%%9.f)\n", aFloat(-x) );
                trice(iD(14890), "rd:TRICE float %+9.6f (%%9.6f)\n", aFloat(-x) );
                trice(iD(14891), "rd:TRICE float %-f (%%f)\n", aFloat(x) );
                trice(iD(14892), "rd:TRICE float %-9f (%%9f)\n", aFloat(x) );
                trice(iD(14893), "rd:TRICE float %-.9f (%%.9f)\n", aFloat(x) );
                trice(iD(14894), "rd:TRICE float %-9.f (%%9.f)\n", aFloat(x) );
                trice(iD(14895), "rd:TRICE float %-9.6f (%%9.6f)\n", aFloat(x) );
                trice(iD(14896), "rd:TRICE float %-f (%%f)\n", aFloat(-x) );
                trice(iD(14897), "rd:TRICE float %-9f (%%9f)\n", aFloat(-x) );
                trice(iD(14898), "rd:TRICE float %-.9f (%%.9f)\n", aFloat(-x) );
                trice(iD(14899), "rd:TRICE float %-9.f (%%9.f)\n", aFloat(-x) );
                trice(iD(14900), "rd:TRICE float %-9.6f (%%9.6f)\n", aFloat(-x) );
        }
#if TRICE_CGO == 1 || defined(TRICE_FULL_CHECK)
        break; case __LINE__: trice(iD(14901), "att:positive float & double in variants\n");
        break; case __LINE__: TRICE32_1(ID(14902), "rd:TRICE32_1 float %e (%%e)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1(ID(14903), "rd:TRICE32_1 float %f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1(ID(14904), "rd:TRICE32_1 float %g (%%g)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1(ID(14905), "rd:TRICE32_1 float %E (%%E)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1(ID(14906), "rd:TRICE32_1 float %F (%%F)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1(ID(14907), "rd:TRICE32_1 float %G (%%G)\n", aFloat(x) );
        break; case __LINE__: TRICE32  (ID(14908), "rd:TRICE32 float %e (%%e)\n", aFloat(x) );
        break; case __LINE__: TRICE32  (ID(14909), "rd:TRICE32 float %f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE32  (ID(14910), "rd:TRICE32 float %g (%%g)\n", aFloat(x) );
        break; case __LINE__: TRICE32  (ID(14911), "rd:TRICE32 float %E (%%E)\n", aFloat(x) );
        break; case __LINE__: TRICE32  (ID(14912), "rd:TRICE32 float %F (%%F)\n", aFloat(x) );
        break; case __LINE__: TRICE32  (ID(14913), "rd:TRICE32 float %G (%%G)\n", aFloat(x) );
        break; case __LINE__: TRICE    (ID(14914), "rd:TRICE float %e (%%e)\n", aFloat(x) );
        break; case __LINE__: TRICE    (ID(14915), "rd:TRICE float %f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE    (ID(14916), "rd:TRICE float %g (%%g)\n", aFloat(x) );
        break; case __LINE__: TRICE    (ID(14917), "rd:TRICE float %E (%%E)\n", aFloat(x) );
        break; case __LINE__: TRICE    (ID(14918), "rd:TRICE float %F (%%F)\n", aFloat(x) );
        break; case __LINE__: TRICE    (ID(14919), "rd:TRICE float %G (%%G)\n", aFloat(x) );
        break; case __LINE__: TRICE64  (ID(14920), "rd:TRICE64 double %e (%%e), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64  (ID(14921), "rd:TRICE64 double %f (%%f), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64  (ID(14922), "rd:TRICE64 double %g (%%g), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64  (ID(14923), "rd:TRICE64 double %E (%%E), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64  (ID(14924), "rd:TRICE64 double %F (%%F), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64  (ID(14925), "rd:TRICE64 double %G (%%G), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1(ID(14926), "rd:TRICE64_1 double %e (%%e), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1(ID(14927), "rd:TRICE64_1 double %f (%%f), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1(ID(14928), "rd:TRICE64_1 double %g (%%g), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1(ID(14929), "rd:TRICE64_1 double %E (%%E), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1(ID(14930), "rd:TRICE64_1 double %F (%%F), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1(ID(14931), "rd:TRICE64_1 double %G (%%G), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: trice(iD(14932), "att:negative float & double\n" );
        break; case __LINE__: trice(iD(14933), "rd:TRICE float %e (%%e)\n", aFloat(-x) );
        break; case __LINE__: trice(iD(14934), "rd:TRICE float %f (%%f)\n", aFloat(-x) );
        break; case __LINE__: trice(iD(14935), "rd:TRICE float %g (%%g)\n", aFloat(-x) );
        break; case __LINE__: trice(iD(14936), "rd:TRICE float %E (%%E)\n", aFloat(-x) );
        break; case __LINE__: trice(iD(14937), "rd:TRICE float %F (%%F)\n", aFloat(-x) );
        break; case __LINE__: trice(iD(14938), "rd:TRICE float %G (%%G)\n", aFloat(-x) );
        break; case __LINE__: TRICE64(ID(14939), "rd:TRICE64 double %e (%%e), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64(ID(14940), "rd:TRICE64 double %f (%%f), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64(ID(14941), "rd:TRICE64 double %g (%%g), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64(ID(14942), "rd:TRICE64 double %E (%%E), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64(ID(14943), "rd:TRICE64 double %F (%%F), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64(ID(14944), "rd:TRICE64 double %G (%%G), aDouble(y)\n", aDouble(-y) );

        break; case __LINE__: TRICE  (ID(15511), "att:formatted float\n" );
        break; case __LINE__: {
                TRICE32(Id(14946), "rd:TRICE32 float %12.6e (%%12.6e)\n", aFloat(x) );
                TRICE32(Id(14947), "rd:TRICE32 float %12.6f (%%12.6f)\n", aFloat(x) );
                TRICE32(Id(14948), "rd:TRICE32 float %12.6g (%%12.6g)\n", aFloat(x) );
                TRICE32(Id(14949), "rd:TRICE32 float %12.6E (%%12.6E)\n", aFloat(x) );
                TRICE32(Id(14950), "rd:TRICE32 float %12.6F (%%12.6F)\n", aFloat(x) );
                TRICE32(Id(14951), "rd:TRICE32 float %12.6G (%%12.6G)\n", aFloat(x) );
        }
        break; case __LINE__: TRICE  (ID(16161), "att:formatted double\n" );
        break; case __LINE__: {
                TRICE64(Id(14952), "rd:TRICE64 double %12.6e (%%12.6e), aDouble(y)\n", aDouble(y) );
                TRICE64(Id(14953), "rd:TRICE64 double %12.6f (%%12.6f), aDouble(y)\n", aDouble(y) );
                TRICE64(Id(14954), "rd:TRICE64 double %12.6g (%%12.6g), aDouble(y)\n", aDouble(y) );
                TRICE64(Id(14955), "rd:TRICE64 double %12.6E (%%12.6E), aDouble(y)\n", aDouble(y) );
                TRICE64(Id(14956), "rd:TRICE64 double %12.6F (%%12.6F), aDouble(y)\n", aDouble(y) );
                TRICE64(Id(14957), "rd:TRICE64 double %12.6G (%%12.6G), aDouble(y)\n", aDouble(y) );
        }
        break; case __LINE__: TRICE (ID(14958), "att:Various single arguments\n" );
        break; case __LINE__: {
                TRICE8(Id(14959), "rd:TRICE8 line %t (%%t ,0)\n", 0 );
                TRICE8(Id(14960), "rd:TRICE8 line %t (%%t ,2)\n", 2 );
                TRICE8(Id(14961), "rd:TRICE8 line %u (%%u ,-1)\n", -1 );
                TRICE8(Id(14962), "rd:TRICE8 line %b (%%b ,-1)\n", -1 );
                TRICE8(Id(14963), "rd:TRICE8 line %o (%%o ,-1)\n", -1 );
                TRICE8(Id(14964), "rd:TRICE8 line %O (%%O ,-1)\n", -1 );
                TRICE8(Id(14965), "rd:TRICE8 line %X (%%X ,-1)\n", -1 );
                TRICE8(Id(14966), "rd:TRICE8 line %x (%%x ,-1)\n", -1 );
                TRICE8(Id(14967), "rd:TRICE8 line %d (%%d ,-1)\n", -1 );
        }
        break; case __LINE__: {
                TRICE16_1(Id(14968), "rd:TRICE16_1 line %t (%%t ,0)\n", 0 );
                TRICE16_1(Id(14969), "rd:TRICE16_1 line %t (%%t ,2)\n", 2 );
                TRICE16_1(Id(14970), "rd:TRICE16_1 line %u (%%u -1)\n", -1 );
                TRICE16_1(Id(14971), "rd:TRICE16_1 line %b (%%b -1)\n", -1 );
                TRICE16_1(Id(14972), "rd:TRICE16_1 line %o (%%o -1)\n", -1 );
                TRICE16_1(Id(14973), "rd:TRICE16_1 line %x (%%x -1)\n", -1 );
                TRICE16_1(Id(14974), "rd:TRICE16_1 line %d (%%d -1)\n", -1 );
                Trice(iD(15805), "rd:Trice line %t (%%t -1)\n", 0 );
                Trice(iD(15491), "rd:Trice line %t (%%t -1)\n", 2 );
                Trice(iD(15971), "rd:Trice line %u (%%u)\n", -1 );
                Trice(iD(16018), "rd:Trice line %b (%%b)\n", -2 );
                Trice(iD(15858), "rd:Trice line %o (%%o)\n", -3 );
                Trice(iD(15700), "rd:Trice line %x (%%x)\n", -4 );
                Trice(iD(16322), "rd:Trice line %d (%%d)\n", -5 );
        }
        break; case __LINE__: TRICE_N(id(14982), "msg:%s\n", A, 0 );
        break; case __LINE__: TRICE_N(id(14983), "msg:%s\n", A, 1 );
        break; case __LINE__: TRICE_N(id(14984), "msg:%s\n", A, 2 );
        break; case __LINE__: TRICE_N(id(14985), "msg:%s\n", A, 3 );
        break; case __LINE__: TRICE_N(id(14986), "msg:%s\n", A, 4 );
        break; case __LINE__: TRICE_N(id(14987), "msg:%s\n", A, 5 );
        break; case __LINE__: TRICE_N(id(14988), "msg:%s\n", A, 6 );
        break; case __LINE__: TRICE_N(id(14989), "msg:%s\n", A, 7 );
        break; case __LINE__: TRICE_N(id(14990), "msg:%s\n", A, 8 );
        break; case __LINE__: TRICE_N(id(14991), "msg:%s\n", A, 9 );
        break; case __LINE__: TRICE_N(id(14992), "msg:%s\n", A, 10 );
        break; case __LINE__: TRICE_N(id(14993), "msg:%s\n", A, 11 );
        break; case __LINE__: TRICE_N(id(14994), "msg:%s\n", A, 12 );
        break; case __LINE__: TRICE_N(id(14995), "msg:%s\n", A, 13 );
        break; case __LINE__: TRICE_N(id(14996), "msg:%s\n", A, 14 );
        break; case __LINE__: TRICE_N(id(14997), "msg:%s\n", A, 15 );
        break; case __LINE__: TRICE_N(id(14998), "msg:%s\n", A, 16 );
        break; case __LINE__: TRICE_N(id(14999), "msg:%s\n", A, 17 );
        break; case __LINE__: TRICE_N(id(15000), "msg:%s\n", A, 18 );
        break; case __LINE__: TRICE_N(id(15001), "msg:%s\n", A, 19 );
        break; case __LINE__: TRICE_N(id(15002), "msg:%s\n", A, 120 );
        break; case __LINE__: TRICE_N(id(15003), "msg:%s\n", A, 121 );
        break; case __LINE__: TRICE_N(id(15004), "msg:%s\n", A, 122 );
        break; case __LINE__: TRICE_N(id(15005), "msg:%s\n", A, 123 );
        break; case __LINE__: TRICE_N(id(15006), "msg:%s\n", A, 124 );
        break; case __LINE__: TRICE_N(id(15007), "msg:%s\n", A, 125 );
        break; case __LINE__: TRICE_N(id(15008), "msg:%s\n", A, 126 );
        break; case __LINE__: TRICE_N(id(15009), "msg:%s\n", A, 127 );
        break; case __LINE__: TRICE_N(id(15010), "msg:%s\n", A, 128 );
        break; case __LINE__: TRICE_N(id(15011), "msg:%s\n", A, 129 );
#endif // #if TRICE_CGO == 1 || defined(TRICE_FULL_CHECK)
        break; case __LINE__: triceN(iD(15012), "msg:%s\n", A, 0 );
        break; case __LINE__: triceN(iD(15013), "msg:%s\n", A, 1 );
        break; case __LINE__: triceN(iD(15014), "msg:%s\n", A, 2 );
        break; case __LINE__: triceN(iD(15015), "msg:%s\n", A, 3 );
        break; case __LINE__: triceN(iD(15016), "msg:%s\n", A, 4 );
        break; case __LINE__: triceN(iD(15017), "msg:%s\n", A, 5 );
        break; case __LINE__: triceN(iD(15018), "msg:%s\n", A, 6 );
        break; case __LINE__: triceN(iD(15019), "msg:%s\n", A, 7 );
        break; case __LINE__: triceN(iD(15020), "msg:%s\n", A, 8 );
        break; case __LINE__: triceN(iD(15021), "msg:%s\n", A, 9 );
        break; case __LINE__: triceN(iD(15022), "msg:%s\n", A, 10 );
        break; case __LINE__: triceN(iD(15023), "msg:%s\n", A, 11 );
        break; case __LINE__: triceN(iD(15024), "msg:%s\n", A, 12 );
        break; case __LINE__: triceN(iD(15025), "msg:%s\n", A, 13 );
        break; case __LINE__: triceN(iD(15026), "msg:%s\n", A, 14 );
        break; case __LINE__: triceN(iD(15027), "msg:%s\n", A, 15 );
        break; case __LINE__: triceN(iD(15028), "msg:%s\n", A, 16 );
        break; case __LINE__: triceN(iD(15029), "msg:%s\n", A, 17 );
        break; case __LINE__: triceN(iD(15030), "msg:%s\n", A, 18 );
        break; case __LINE__: triceN(iD(15031), "msg:%s\n", A, 19 );
        break; case __LINE__: triceN(iD(15032), "msg:%s\n", A, 120 );
        break; case __LINE__: triceN(iD(15033), "msg:%s\n", A, 121 );
        break; case __LINE__: triceN(iD(15034), "msg:%s\n", A, 122 );
        break; case __LINE__: triceN(iD(15035), "msg:%s\n", A, 123 );
        break; case __LINE__: triceN(iD(15036), "msg:%s\n", A, 124 );
        break; case __LINE__: triceN(iD(15037), "msg:%s\n", A, 125 );
        break; case __LINE__: triceN(iD(15038), "msg:%s\n", A, 126 );
        break; case __LINE__: triceN(iD(15039), "msg:%s\n", A, 127 );
        break; case __LINE__: triceN(iD(15040), "msg:%s\n", A, 128 );
        break; case __LINE__: triceN(iD(15041), "msg:%s\n", A, 129 );

        break; case __LINE__: trice(iD(15042), "sig:TRICE with 1 to 12 values\n" );
        break; case __LINE__: trice(iD(15043), "rd:TRICE %d\n", -1 );                                                                                              //exp: time:        default: rd:TRICE -1     
        break; case __LINE__: trice(iD(15044), "rd:TRICE %d, %d\n", -1, -2 );                                                                                      //exp: time:        default: rd:TRICE -1, -2 
        break; case __LINE__: trice(iD(15045), "rd:TRICE %d, %d, %d\n", -1, -2, -3 );                                                                              //exp: time:        default: rd:TRICE -1, -2, -3       
        break; case __LINE__: trice(iD(15046), "rd:TRICE %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                      //exp: time:        default: rd:TRICE -1, -2, -3, -4      
        break; case __LINE__: trice(iD(15047), "rd:TRICE %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                              //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5        
        break; case __LINE__: trice(iD(15048), "rd:TRICE %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                      //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice(iD(15049), "rd:TRICE %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                               //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice(iD(15050), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                      //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice(iD(15051), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                              //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice(iD(15052), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                      //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice(iD(15053), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );            //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice(iD(15054), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );   //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: trice(iD(15055), "sig:TRICE_n with 1 to 12 values\n" );		
        break; case __LINE__: trice(iD(15056), "rd:TRICE_1  %d\n", -1 );                                                                                            //exp: time:        default: rd:TRICE_1  -1     
        break; case __LINE__: trice(iD(15057), "rd:TRICE_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:        default: rd:TRICE_2  -1, -2 
        break; case __LINE__: trice(iD(15058), "rd:TRICE_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: rd:TRICE_3  -1, -2, -3       
        break; case __LINE__: trice(iD(15059), "rd:TRICE_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:        default: rd:TRICE_4  -1, -2, -3, -4      
        break; case __LINE__: trice(iD(15060), "rd:TRICE_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:        default: rd:TRICE_5  -1, -2, -3, -4, -5        
        break; case __LINE__: trice(iD(15061), "rd:TRICE_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: rd:TRICE_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice(iD(15062), "rd:TRICE_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:        default: rd:TRICE_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice(iD(15063), "rd:TRICE_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:        default: rd:TRICE_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice(iD(15064), "rd:TRICE_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: rd:TRICE_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice(iD(15065), "rd:TRICE_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:        default: rd:TRICE_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice(iD(15066), "rd:TRICE_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:        default: rd:TRICE_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice(iD(15067), "rd:TRICE_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: rd:TRICE_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
#if TRICE_CGO == 1 || defined(TRICE_FULL_CHECK)
        break; case __LINE__: TRice(iD(15522), "sig:Trice16 with 1 to 12 pointer\n" );
        break; case __LINE__: {
                Trice16(iD(16271), "rd:Trice16 %p\n", -1 );
                Trice16(iD(15724), "rd:Trice16 %p, %p\n", -1, -2 );
                Trice16(iD(16307), "rd:Trice16 %p, %p, %p\n", -1, -2, -3 );
                Trice16(iD(15475), "rd:Trice16 %p, %p, %p, %p\n", -1, -2, -3, -4 );
                Trice16(iD(15583), "rd:Trice16 %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5 );
                Trice16(iD(16204), "rd:Trice16 %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6 );
                Trice16(iD(16292), "rd:Trice16 %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7);
                Trice16(iD(16219), "rd:Trice16 %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8 );
                Trice16(iD(16004), "rd:Trice16 %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
                Trice16(iD(16135), "rd:Trice16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
                Trice16(iD(15360), "rd:Trice16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
                Trice16(iD(15608), "rd:Trice16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        }
        break; case __LINE__: TRice(iD(15260), "sig:TRICE16 with 1 to 12 hex\n" );
        break; case __LINE__: {
                Trice16(iD(15242), "rd:Trice16 %X\n", -1 );
                Trice16(iD(15340), "rd:Trice16 %X, %X\n", -1, -2 );
                Trice16(iD(16267), "rd:Trice16 %X, %X, %X\n", -1, -2, -3 );
                Trice16(iD(15545), "rd:Trice16 %X, %X, %X, %X\n", -1, -2, -3, -4 );
                Trice16(iD(15696), "rd:Trice16 %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5 );
                Trice16(iD(15525), "rd:Trice16 %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6 );
                Trice16(iD(15588), "rd:Trice16 %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7);
                Trice16(iD(15749), "rd:Trice16 %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8 );
                Trice16(iD(15632), "rd:Trice16 %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
                Trice16(iD(15881), "rd:Trice16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
                Trice16(iD(15785), "rd:Trice16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
                Trice16(iD(15329), "rd:Trice16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        }
#endif // #if TRICE_CGO == 1 || defined(TRICE_FULL_CHECK)
        break; case __LINE__: TRICE(ID(15094), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE(id(15095), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE(Id(15096), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE(ID(15097), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: trice(iD(15098), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: Trice(iD(15099), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRice(iD(15100), "w: Hello! 👋🙂 \a\n" ); // with sound!

        break; case __LINE__: TRICE_0(ID(15101), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE_0(id(15102), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE_0(Id(15103), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE_0(ID(15104), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: trice_0(iD(15105), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: Trice_0(iD(15106), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRice_0(iD(15107), "w: Hello! 👋🙂 \a\n" ); // with sound!

        break; case __LINE__: TRICE0(ID(15108), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE0(id(15109), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE0(Id(15110), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE0(ID(15111), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: trice0(iD(15112), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: Trice0(iD(15113), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRice0(iD(15114), "w: Hello! 👋🙂 \a\n" ); // with sound!

        break; case __LINE__: TRICE_0(ID(15115), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE_0(id(15116), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE_0(Id(15117), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE_0(ID(15118), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: trice_0(iD(15119), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: Trice_0(iD(15120), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRice_0(iD(15121), "w: Hello! 👋🙂 \a\n" ); // with sound!

        break; case __LINE__: TRICE8(ID(15122), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE8(id(15123), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE8(Id(15124), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE8(ID(15125), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: trice8(iD(15126), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: Trice8(iD(15127), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRice8(iD(15128), "w: Hello! 👋🙂 \a\n" ); // with sound!

        break; case __LINE__: TRICE8_0(ID(15129), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE8_0(id(15130), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE8_0(Id(15131), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE8_0(ID(15132), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: trice8_0(iD(15133), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: Trice8_0(iD(15134), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRice8_0(iD(15135), "w: Hello! 👋🙂 \a\n" ); // with sound!

        break; case __LINE__: trice16(iD(15136), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: trice16(iD(15137), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: trice16(iD(15138), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: trice16(iD(15139), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: trice16(iD(15140), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: Trice16(iD(15141), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRice16(iD(15142), "w: Hello! 👋🙂 \a\n" ); // with sound!

        break; case __LINE__: TRICE16_0(ID(15143), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE16_0(id(15144), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE16_0(Id(15145), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE16_0(ID(15146), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: trice16_0(iD(15147), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: Trice16_0(iD(15148), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRice16_0(iD(15149), "w: Hello! 👋🙂 \a\n" ); // with sound!

        break; case __LINE__: TRICE32(ID(15150), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE32(id(15151), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE32(Id(15152), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE32(ID(15153), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: trice32(iD(15154), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: Trice32(iD(15155), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRice32(iD(15156), "w: Hello! 👋🙂 \a\n" ); // with sound!

        break; case __LINE__: TRICE32_0(ID(15157), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE32_0(id(15158), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE32_0(Id(15159), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE32_0(ID(15160), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: trice32_0(iD(15161), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: Trice32_0(iD(15162), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRice32_0(iD(15163), "w: Hello! 👋🙂 \a\n" ); // with sound!

        break; case __LINE__: TRICE64(ID(15164), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE64(id(15165), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE64(Id(15166), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE64(ID(15167), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: trice64(iD(15168), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: Trice64(iD(15169), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRice64(iD(15170), "w: Hello! 👋🙂 \a\n" ); // with sound!

        break; case __LINE__: TRICE64_0(ID(15171), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE64_0(id(15172), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE64_0(Id(15173), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE64_0(ID(15174), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: trice64_0(iD(15175), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: Trice64_0(iD(15176), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRice64_0(iD(15177), "w: Hello! 👋🙂 \a\n" ); // with sound!

    #endif // #if TRICE_CGO == 1 || defined(TRICE_FULL_CHECK)
    }
#endif // #else // #ifdef TRICE_CHECK_MIN
}

// clang-format on

#ifndef TRICE_CHECK_MIN

#if !TRICE_OFF

static int32_t FloatToInt32(float f) {
	if (f >= 0) {
		return (int32_t)f;
	}
	return -(int32_t)-f;
}

static int64_t DoubleToInt64(double f) {
	if (f >= 0) {
		return (int64_t)f;
	}
	return -(int64_t)-f;
}

#endif

//! SCOPY is a helper macro for struct serialization.
#define SCOPY(element)                       \
	do {                                     \
		char* n = #element;                  \
		int size = sizeof(src->element);     \
		memcpy(p, &(src->element), size);    \
		p += size;                           \
		TRICE_S(ID(15178), "rd:sizeof(%8s)", n); \
		TRICE(ID(15179), " = %d\n", size);       \
	} while (0);

//! DCOPY is a helper macro for struct deserialization.
#define DCOPY(element)                       \
	do {                                     \
		char* n = #element;                  \
		int size = sizeof(dst->element);     \
		memcpy(&(dst->element), p, size);    \
		p += size;                           \
		TRICE_S(ID(15180), "rd:sizeof(%8s)", n); \
		TRICE(ID(15181), " = %d\n", size);       \
	} while (0);

typedef struct {
	float x;
	float y;
	uint8_t rgb[3];
	// float z; // it seems, that the compiler does not align this with -o3 & time !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
} Point_t; //!< Point_t is small struct type.

#if !TRICE_OFF

static int serializePoint(char* dst, const Point_t* src) {
	char* p = dst;

	SCOPY(x)
	SCOPY(y)
	SCOPY(rgb)

	return p - dst;
}

static int deserializePoint(Point_t* const dst, const char* src) {
	char const* p = src;

	DCOPY(x)
	DCOPY(y)
	DCOPY(rgb)

	return p - src;
}

#endif

typedef struct {
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

#if !TRICE_OFF

static int serializeTryout(char* dst, const Tryout_t* src) {
	char* p = dst;

	SCOPY(z)
	SCOPY(u)
	SCOPY(s)
	SCOPY(addr)
	SCOPY(x)
	SCOPY(y)
	SCOPY(names)
	p += serializePoint(p, src->point);
	SCOPY(bitmask)

	return p - dst;
}

static int deserializeTryout(Tryout_t* const dst, const char* src) {
	char const* p = src;

	DCOPY(z)
	DCOPY(u)
	DCOPY(s)
	DCOPY(addr)
	DCOPY(x)
	DCOPY(y)
	DCOPY(names)
	p += deserializePoint(dst->point, p);
	DCOPY(bitmask)

	return p - src;
}

#endif // #i !TRICE_OFF

static void exampleOfManualSerialization(void) {
	Tryout_t tx;          // struct to transfer
	Tryout_t rx;          // "received" struct
	static char dst[100]; // serialized data

	/////////////////////////////////////////////////////////
	// fill tx with data
	tx.z = (float)123.456;
	tx.u = 44444;
	tx.addr = "Haus";
	tx.s = -2;
	tx.x = 0xaa55bb77;
	tx.y = -1000000;

	memcpy(tx.names[0], "aaa", strlen("aaa"));
	memcpy(tx.names[1], "bbbb", strlen("bbbb"));
	memcpy(tx.names[2], "ccccc", strlen("ccccc"));

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
#if !TRICE_OFF
	int len = serializeTryout(dst, &tx); // serialized byte count
#else
  int len = 0;
#endif
	TRICE(Id(15182), "inf: Tryout tx struct:");
	TRICE8_B(Id(15183), " %02x ", &tx, sizeof(tx));
	TRICE(Id(15184), "\n");

	TRICE(Id(15185), "inf: Tryout buffer:");
	TRICE8_B(Id(15186), " %02x ", dst, len); // lint !e670
	TRICE(Id(15187), "\n");

#if !TRICE_OFF
	char* src = dst; // "copy" - assume, data transferred now ("data transfer")
	len = deserializeTryout(&rx, src);
#endif
	TRICE(Id(15188), "inf: Tryout rx struct:");
	TRICE8_B(Id(15189), " %02x ", &rx, sizeof(rx));
	TRICE(Id(15190), "\n");

	TRICE(Id(15191), "inf:sizeOf(Trypout) = %d, buffer length = %d\n", sizeof(tx), len);
	TRICE8_F(Id(15192), "info:TryoutStructFunction", &tx, sizeof(tx));
	TRICE8_F(Id(15193), "info:TryoutBufferFunction", dst, len); // lint !e670
}

static void exampleOfManualJSONencoding(void) {
#if !TRICE_OFF
	typedef struct {
		int Apple, Birn;
		float Fish;
	} Ex_t;
	Ex_t Ex = {-1, 2, (float)2.781};
#endif
	Trice(iD(15194), "att:MyStructEvaluationFunction(json:ExA{Apple:%d, Birn:%u, Fisch:%f}\n", Ex.Apple, Ex.Birn, aFloat(Ex.Fish));
}

static void dynString(int n) {
	char* s = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.,0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.,0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.,0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.,0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.,";
	const size_t l = strlen(s);
	n = n < l ? n : l;
	// trice(iD(15195), "sig:%3d:", n ); - this gets overwritten in CGO_Test case, so we avoid it to keep testing simple.
	TRICE_N(id(15196), "wr:%s\n", s, n);
}

#endif // #ifndef TRICE_CHECK_MIN

#endif // #if !TRICE_OFF
