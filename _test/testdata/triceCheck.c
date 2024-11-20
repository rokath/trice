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
    TRice("\rTriceCheck %4d", n);
#else // #ifdef TRICE_CHECK_MIN
    char* sABCDE = "abcde 12345";
#if !TRICE_OFF
    uint32_t lenABCDE = strlen(sABCDE);
   float  x = (float)1089.6082763671875; // 0x44883377
    double y = 518.0547492508867; // 0x4080307020601050  
    #if TRICE_CGO == 1 || defined(TRICE_FULL_CHECK)
    char* A = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
    #endif
#endif // #if !TRICE_OFF
    char* s = "AAAAAAAAAAAA";
    char * five = "five";
    static int8_t b8[24] = { 0, -1, -2, 0x33, 4, 5, 6, 7, 8, 9, 10, 11, 0, -1, -2, 0x33, 4, 5, 6, 7, 8, 9, 10, 11 };
    static int16_t b16[] = { 0, -1, -2, 0x3344 };
    static int32_t b32[] = { 0, -1, -2, 0x33445555};
    static int64_t b64[4] = { 0, -1, -2, 0x3344555566666666 };
    unsigned anchorLine = __LINE__ + 20; // The offset value here must point to the first test line containing `anchorLine`.

    switch( n ){ 
        default:
        break; case __LINE__: trice16("att: line %u\n", __LINE__ );
        // Lines ending with a comment starting with `//exp:` are executed during trhe automatic Go tests.
        break; case __LINE__: TRice("Hello World!\n" );                                              //exp: time:feed3322default: Hello World!		
        // normal use cases
        break; case __LINE__: TRice("info:This is a message without values and a 32-bit stamp.\n" ); //exp: time:feed3322default: info:This is a message without values and a 32-bit stamp.
        break; case __LINE__: Trice("info:This is a message without values and a 16-bit stamp.\n" ); //exp: time:    be16default: info:This is a message without values and a 16-bit stamp.
        break; case __LINE__: trice("info:This is a message without values and without stamp.\n"  ); //exp: time:        default: info:This is a message without values and without stamp.
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

        break; case __LINE__: trice("sig:trice without stamp and with 0 to 12 values (most common use cases)\n" );    
        break; case __LINE__: trice("rd:trice\n" );                                                                                                   //exp: time:        default: rd:trice
        break; case __LINE__: trice("rd:trice %d\n", -1 );                                                                                            //exp: time:        default: rd:trice -1     
        break; case __LINE__: trice("rd:trice %d, %d\n", -1, -2 );                                                                                    //exp: time:        default: rd:trice -1, -2 
        break; case __LINE__: trice("rd:trice %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: rd:trice -1, -2, -3       
        break; case __LINE__: trice("rd:trice %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:        default: rd:trice -1, -2, -3, -4      
        break; case __LINE__: trice("rd:trice %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:        default: rd:trice -1, -2, -3, -4, -5        
        break; case __LINE__: trice("rd:trice %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: rd:trice -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice("rd:trice %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:        default: rd:trice -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice("rd:trice %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:        default: rd:trice -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice("rd:trice %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: rd:trice -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice("rd:trice %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:        default: rd:trice -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice("rd:trice %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:        default: rd:trice -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice("rd:trice %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: rd:trice -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: Trice("sig:Trice with 16-bit stamp and with 0 to 12 values (common use cases)\n" );     
        break; case __LINE__: Trice("rd:Trice\n" );                                                                                                   //exp: time:    be16default: rd:Trice
        break; case __LINE__: Trice("rd:Trice %d\n", -1 );                                                                                            //exp: time:    be16default: rd:Trice -1     
        break; case __LINE__: Trice("rd:Trice %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:Trice -1, -2 
        break; case __LINE__: Trice("rd:Trice %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:Trice -1, -2, -3       
        break; case __LINE__: Trice("rd:Trice %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:Trice -1, -2, -3, -4      
        break; case __LINE__: Trice("rd:Trice %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:Trice -1, -2, -3, -4, -5        
        break; case __LINE__: Trice("rd:Trice %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:Trice -1, -2, -3, -4, -5, -6        
        break; case __LINE__: Trice("rd:Trice %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:Trice -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: Trice("rd:Trice %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:Trice -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: Trice("rd:Trice %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:Trice -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: Trice("rd:Trice %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:Trice -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: Trice("rd:Trice %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:Trice -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: Trice("rd:Trice %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:Trice -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: TRice("sig:TRice with 32-bit stamp and with 0 to 12 values (common use cases)\n" );
        break; case __LINE__: TRice("rd:TRice\n" );                                                                                                   //exp: time:feed3322default: rd:TRice
        break; case __LINE__: TRice("rd:TRice %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRice -1     
        break; case __LINE__: TRice("rd:TRice %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRice -1, -2 
        break; case __LINE__: TRice("rd:TRice %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRice -1, -2, -3       
        break; case __LINE__: TRice("rd:TRice %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRice -1, -2, -3, -4      
        break; case __LINE__: TRice("rd:TRice %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRice -1, -2, -3, -4, -5        
        break; case __LINE__: TRice("rd:TRice %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRice -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRice("rd:TRice %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRice -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRice("rd:TRice %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRice -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRice("rd:TRice %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRice -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRice("rd:TRice %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRice -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRice("rd:TRice %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRice -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRice("rd:TRice %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRice -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: trice  ("sig:trice16 with 1 to 12 pointer\n" );
        break; case __LINE__: trice16("rd:trice16 %p\n", -1 );                                                                                            //exp: time:        default: rd:trice16 ffff      
        break; case __LINE__: trice16("rd:trice16 %p, %p\n", -1, -2 );                                                                                    //exp: time:        default: rd:trice16 ffff, fffe      
        break; case __LINE__: trice16("rd:trice16 %p, %p, %p\n", -1, -2, -3 );                                                                            //exp: time:        default: rd:trice16 ffff, fffe, fffd          
        break; case __LINE__: trice16("rd:trice16 %p, %p, %p, %p\n", -1, -2, -3, -4 );                                                                    //exp: time:        default: rd:trice16 ffff, fffe, fffd, fffc            
        break; case __LINE__: trice16("rd:trice16 %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5 );                                                            //exp: time:        default: rd:trice16 ffff, fffe, fffd, fffc, fffb       
        break; case __LINE__: trice16("rd:trice16 %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: rd:trice16 ffff, fffe, fffd, fffc, fffb, fffa              
        break; case __LINE__: trice16("rd:trice16 %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:        default: rd:trice16 ffff, fffe, fffd, fffc, fffb, fffa, fff9           
        break; case __LINE__: trice16("rd:trice16 %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:        default: rd:trice16 ffff, fffe, fffd, fffc, fffb, fffa, fff9, fff8               
        break; case __LINE__: trice16("rd:trice16 %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: rd:trice16 ffff, fffe, fffd, fffc, fffb, fffa, fff9, fff8, fff7              
        break; case __LINE__: trice16("rd:trice16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:        default: rd:trice16 ffff, fffe, fffd, fffc, fffb, fffa, fff9, fff8, fff7, fff6                
        break; case __LINE__: trice16("rd:trice16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:        default: rd:trice16 ffff, fffe, fffd, fffc, fffb, fffa, fff9, fff8, fff7, fff6, fff5              
        break; case __LINE__: trice16("rd:trice16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: rd:trice16 ffff, fffe, fffd, fffc, fffb, fffa, fff9, fff8, fff7, fff6, fff5, fff4

        break; case __LINE__: trice8 ("rd:trice8  %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );  //exp: time:        default: rd:trice8  ff, fe, fd, fc, fb, fa, f9, f8, f7, f6, f5, f4
        break; case __LINE__: trice32("rd:trice32 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );  //exp: time:        default: rd:trice32 ffffffff, fffffffe, fffffffd, fffffffc, fffffffb, fffffffa, fffffff9, fffffff8, fffffff7, fffffff6, fffffff5, fffffff4

        break; case __LINE__: trice  ("sig:trice16 with 1 to 12 hex\n" );

        break; case __LINE__: trice16("rd:trice16 %X\n", -1 );                                                                                             //exp: time:        default: rd:trice16 FFFF                 
        break; case __LINE__: trice16("rd:trice16 %X, %X\n", -1, -2 );                                                                                     //exp: time:        default: rd:trice16 FFFF, FFFE             
        break; case __LINE__: trice16("rd:trice16 %X, %X, %X\n", -1, -2, -3 );                                                                             //exp: time:        default: rd:trice16 FFFF, FFFE, FFFD            
        break; case __LINE__: trice16("rd:trice16 %X, %X, %X, %X\n", -1, -2, -3, -4 );                                                                     //exp: time:        default: rd:trice16 FFFF, FFFE, FFFD, FFFC                  
        break; case __LINE__: trice16("rd:trice16 %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5 );                                                             //exp: time:        default: rd:trice16 FFFF, FFFE, FFFD, FFFC, FFFB              
        break; case __LINE__: trice16("rd:trice16 %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6 );                                                     //exp: time:        default: rd:trice16 FFFF, FFFE, FFFD, FFFC, FFFB, FFFA                     
        break; case __LINE__: trice16("rd:trice16 %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7);                                              //exp: time:        default: rd:trice16 FFFF, FFFE, FFFD, FFFC, FFFB, FFFA, FFF9                
        break; case __LINE__: trice16("rd:trice16 %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                     //exp: time:        default: rd:trice16 FFFF, FFFE, FFFD, FFFC, FFFB, FFFA, FFF9, FFF8             
        break; case __LINE__: trice16("rd:trice16 %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                             //exp: time:        default: rd:trice16 FFFF, FFFE, FFFD, FFFC, FFFB, FFFA, FFF9, FFF8, FFF7          
        break; case __LINE__: trice16("rd:trice16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                     //exp: time:        default: rd:trice16 FFFF, FFFE, FFFD, FFFC, FFFB, FFFA, FFF9, FFF8, FFF7, FFF6          
        break; case __LINE__: trice16("rd:trice16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );           //exp: time:        default: rd:trice16 FFFF, FFFE, FFFD, FFFC, FFFB, FFFA, FFF9, FFF8, FFF7, FFF6, FFF5             
        break; case __LINE__: trice16("rd:trice16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );  //exp: time:        default: rd:trice16 FFFF, FFFE, FFFD, FFFC, FFFB, FFFA, FFF9, FFF8, FFF7, FFF6, FFF5, FFF4

        break; case __LINE__: trice8 ("rd:trice8  %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );  //exp: time:        default: rd:trice8  FF, FE, FD, FC, FB, FA, F9, F8, F7, F6, F5, F4
        break; case __LINE__: trice32("rd:trice32 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );  //exp: time:        default: rd:trice32 FFFFFFFF, FFFFFFFE, FFFFFFFD, FFFFFFFC, FFFFFFFB, FFFFFFFA, FFFFFFF9, FFFFFFF8, FFFFFFF7, FFFFFFF6, FFFFFFF5, FFFFFFF4

        break; case __LINE__: trice16("att: line %u\n", __LINE__ );
    
        break; case __LINE__: 
        #if TRICE_DEFERRED_XTEA_ENCRYPT == 1 || TRICE_DIRECT_XTEA_ENCRYPT == 1
                trice("--------------------------------------------------\n" );
                {
                    uint32_t by[8] = {1,2,3,4,5,6};
                    int count = (sizeof(by) + 7) & ~7; // only multiple of 8 encryptable
                    trice32("msg: message = %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
                    trice32("tim: pre TriceEncryption SysTick=%d\n", SYSTICKVAL );
                    XTEAEncrypt(by, count>>2);
                    trice32("tim: post TriceEncryption SysTick=%d\n", SYSTICKVAL );
                    trice32("att: TriceEncrypted =  %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] ); 
                    #if XTEA_DECRYPT == 1
                        trice16_1("tim: pre TriceDecryption SysTick=%d\n", SYSTICKVAL );
                        XTEADecrypt(by, count>>2);
                        trice16_1("tim: post TriceDecryption SysTick=%d\n", SYSTICKVAL );
                    #endif
                    trice32("msg: messge = %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
                }
                trice("--------------------------------------------------\n" );
        #endif

        break; case __LINE__: trice16("att: line %u\n", __LINE__ );

        break; case __LINE__: trice("dbg:Hi!\n" );                       //exp: time:        default: dbg:Hi!    
        break; case __LINE__: trice("dbg:	Hi!\n" );                    //exp: time:        default: dbg:	Hi!

        break; case __LINE__: trice("sig:Some time measurements\n" );
        break; case __LINE__: trice("isr:trice isr message, SysTick is %6d\n", SYSTICKVAL);                //exp: time:        default: isr:trice isr message, SysTick is      0 
        break; case __LINE__: trice("isr:trice isr message, SysTick is %6d\n", SYSTICKVAL);                //exp: time:        default: isr:trice isr message, SysTick is      0 
        break; case __LINE__: trice("isr:trice isr message, SysTick is %6d\n", SYSTICKVAL);                //exp: time:        default: isr:trice isr message, SysTick is      0 
        break; case __LINE__: trice("isr:trice isr message, SysTick is %6d\n", SYSTICKVAL);                //exp: time:        default: isr:trice isr message, SysTick is      0

        break; case __LINE__: trice_1("isr:trice_1 isr message, SysTick is %6d\n", SYSTICKVAL);            //exp: time:        default: isr:trice_1 isr message, SysTick is      0 
        break; case __LINE__: trice_1("isr:trice_1 isr message, SysTick is %6d\n", SYSTICKVAL);            //exp: time:        default: isr:trice_1 isr message, SysTick is      0 
        break; case __LINE__: trice_1("isr:trice_1 isr message, SysTick is %6d\n", SYSTICKVAL);            //exp: time:        default: isr:trice_1 isr message, SysTick is      0 
        break; case __LINE__: trice_1("isr:trice_1 isr message, SysTick is %6d\n", SYSTICKVAL);            //exp: time:        default: isr:trice_1 isr message, SysTick is      0

        break; case __LINE__: TRICE16_1(id(0), "isr:TRICE16_1 isr message, SysTick is %6d\n", SYSTICKVAL);        //exp: time:        default: isr:TRICE16_1 isr message, SysTick is      0 
        break; case __LINE__: TRICE16_1(id(0), "isr:TRICE16_1 isr message, SysTick is %6d\n", SYSTICKVAL);        //exp: time:        default: isr:TRICE16_1 isr message, SysTick is      0 
        break; case __LINE__: TRICE16_1(id(0), "isr:TRICE16_1 isr message, SysTick is %6d\n", SYSTICKVAL);        //exp: time:        default: isr:TRICE16_1 isr message, SysTick is      0 
        break; case __LINE__: TRICE16_1(id(0), "isr:TRICE16_1 isr message, SysTick is %6d\n", SYSTICKVAL);        //exp: time:        default: isr:TRICE16_1 isr message, SysTick is      0 

        break; case __LINE__: TRICE_1(id(0), "isr:TRICE_1 isr message, SysTick is %6d\n", SYSTICKVAL);            //exp: time:        default: isr:TRICE_1 isr message, SysTick is      0 
        break; case __LINE__: TRICE_1(id(0), "isr:TRICE_1 isr message, SysTick is %6d\n", SYSTICKVAL);            //exp: time:        default: isr:TRICE_1 isr message, SysTick is      0 
        break; case __LINE__: TRICE_1(id(0), "isr:TRICE_1 isr message, SysTick is %6d\n", SYSTICKVAL);            //exp: time:        default: isr:TRICE_1 isr message, SysTick is      0 
        break; case __LINE__: TRICE_1(id(0), "isr:TRICE_1 isr message, SysTick is %6d\n", SYSTICKVAL);            //exp: time:        default: isr:TRICE_1 isr message, SysTick is      0 

        break; case __LINE__: TRICE(id(0), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);                //exp: time:        default: isr:TRICE isr message, SysTick is      0 
        break; case __LINE__: TRICE(id(0), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);                //exp: time:        default: isr:TRICE isr message, SysTick is      0 
        break; case __LINE__: TRICE(id(0), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);                //exp: time:        default: isr:TRICE isr message, SysTick is      0 
        break; case __LINE__: TRICE(id(0), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);                //exp: time:        default: isr:TRICE isr message, SysTick is      0 

        break; case __LINE__: trice("tim:trice START time message\n");                                     //exp: time:        default: tim:trice START time message
        break; case __LINE__: trice("tim:trice STOP time message\n");                                      //exp: time:        default: tim:trice STOP time message
        break; case __LINE__: trice("tim:trice START time message\n");                                     //exp: time:        default: tim:trice START time message
        break; case __LINE__: trice("tim:trice STOP time message\n");                                      //exp: time:        default: tim:trice STOP time message 
        break; case __LINE__: trice("tim:trice START time message\n");                                     //exp: time:        default: tim:trice START time message
        break; case __LINE__: trice64("rd:trice64 %d, %d\n", 1, 2 );                                       //exp: time:        default: rd:trice64 1, 2   
        break; case __LINE__: trice("tim:trice STOP time message\n");                                      //exp: time:        default: tim:trice STOP time message
        
        break; case __LINE__: TRICE(id(0), "tim:TRICE START time message\n");                                     //exp: time:        default: tim:TRICE START time message
        break; case __LINE__: TRICE(id(0), "tim:TRICE STOP time message\n");                                      //exp: time:        default: tim:TRICE STOP time message 
        break; case __LINE__: TRICE(id(0), "tim:TRICE START time message\n");                                     //exp: time:        default: tim:TRICE START time message
        break; case __LINE__: TRICE(id(0), "tim:TRICE STOP time message\n");                                      //exp: time:        default: tim:TRICE STOP time message 
        break; case __LINE__: TRICE(id(0), "tim:TRICE START time message\n");                                     //exp: time:        default: tim:TRICE START time message
        break; case __LINE__: TRICE64(id(0), "rd:TRICE64 %d, %d\n", 1, 2 );                                       //exp: time:        default: rd:TRICE64 1, 2    
        break; case __LINE__: TRICE(id(0), "tim:TRICE STOP time message\n");                                      //exp: time:        default: tim:TRICE STOP time message 

        break; case __LINE__: trice  ("tim:trice --------------------------------------------------\n");   //exp: time:        default: tim:trice --------------------------------------------------
        break; case __LINE__: trice  ("tim:trice --------------------------------------------------\n");   //exp: time:        default: tim:trice --------------------------------------------------
        break; case __LINE__: trice64("rd:trice64 %d, %d\n", 1, 2 );                                       //exp: time:        default: rd:trice64 1, 2
        break; case __LINE__: trice  ("tim:trice --------------------------------------------------\n");   //exp: time:        default: tim:trice --------------------------------------------------
        break; case __LINE__: trice  ("tim:trice --------------------------------------------------\n");   //exp: time:        default: tim:trice --------------------------------------------------
        break; case __LINE__: trice64("rd:trice64 %d, %d\n", 1, 2 );                                       //exp: time:        default: rd:trice64 1, 2
        break; case __LINE__: trice32_1("rd:trice32_1 %d\n", -1 );                                         //exp: time:        default: rd:trice32_1 -1
        break; case __LINE__: trice64_1("rd:trice64_1 %d\n", -2 );                                         //exp: time:        default: rd:trice64_1 -2
        break; case __LINE__: trice32_2("rd:trice32_2 %d,%d\n", -1, 2 );                                   //exp: time:        default: rd:trice32_2 -1,2
        break; case __LINE__: trice64_2("rd:trice64_2 %d,%d\n", -1, 2 );                                   //exp: time:        default: rd:trice64_2 -1,2

        // special use cases
        break; case __LINE__: TRice64("info:12 64-bit values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 32-bit stamp.\n", -3, -4, -5, -6, 0, aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), -8 ); //exp: time:feed3322default: info:12 64-bit values -3, 18446744073709551612, fffffffffffffffb, FFFFFFFFFFFFFFFA, false, -7.123457e+00, -7.123457, -7.123456789, -7.123457E+00, -7.123457, -7.123456789, 0xb1111111111111111111111111111111111111111111111111111111111111000 and a 32-bit stamp.
        break; case __LINE__: Trice64("info:12 64-bit values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 16-bit stamp.\n", -3, -4, -5, -6, 0, aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), -8 ); //exp: time:    be16default: info:12 64-bit values -3, 18446744073709551612, fffffffffffffffb, FFFFFFFFFFFFFFFA, false, -7.123457e+00, -7.123457, -7.123456789, -7.123457E+00, -7.123457, -7.123456789, 0xb1111111111111111111111111111111111111111111111111111111111111000 and a 16-bit stamp.
        break; case __LINE__: trice64("info:12 64-bit values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and without stamp.\n" , -3, -4, -5, -6, 0, aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), -8 ); //exp: time:        default: info:12 64-bit values -3, 18446744073709551612, fffffffffffffffb, FFFFFFFFFFFFFFFA, false, -7.123457e+00, -7.123457, -7.123456789, -7.123457E+00, -7.123457, -7.123456789, 0xb1111111111111111111111111111111111111111111111111111111111111000 and without stamp.
        break; case __LINE__: TRice32("info:12 32-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 32-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:feed3322default: info:12 32-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 32-bit stamp.
        break; case __LINE__: Trice32("info:12 32-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 16-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:    be16default: info:12 32-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 16-bit stamp.
        break; case __LINE__: trice32("info:12 32-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and without stamp.\n" , -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:        default: info:12 32-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and without stamp.
        break; case __LINE__: TRice16("info:12 16-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 32-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:feed3322default: info:12 16-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 32-bit stamp.
        break; case __LINE__: Trice16("info:12 16-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 16-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:    be16default: info:12 16-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 16-bit stamp.
        break; case __LINE__: trice16("info:12 16-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and without stamp.\n" , -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:        default: info:12 16-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and without stamp.
        break; case __LINE__: TRice8 ("info:12  8-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 32-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:feed3322default: info:12  8-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 32-bit stamp.
        break; case __LINE__: Trice8 ("info:12  8-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 16-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:    be16default: info:12  8-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 16-bit stamp.
        break; case __LINE__: trice8 ("info:12  8-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and without stamp.\n" , -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:        default: info:12  8-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and without stamp.

        break; case __LINE__: trice16("att: line %u\n", __LINE__ );

        break; case __LINE__: TRice16("att: 0x8888 == %04xh\n", 0x8888 ); //exp: time:feed3322default: att: 0x8888 == 8888h
        break; case __LINE__: Trice16("att: 0x8888 == %04xh\n", 0x8888 ); //exp: time:    be16default: att: 0x8888 == 8888h
        break; case __LINE__: trice16("att: 0x8888 == %04xh\n", 0x8888 ); //exp: time:        default: att: 0x8888 == 8888h


        break; case __LINE__: trice16("att: line %u\n", __LINE__ );

        break; case __LINE__: trice8   ("value=%x\n", 0x55 );             //exp: time:        default: value=55
        break; case __LINE__: trice8   ("value=%x\n", 0x88 );             //exp: time:        default: value=88

        break; case __LINE__: trice("att:mixed int & float & double & bits\n" );
        break; case __LINE__: TRice32("rd:TRice32 int %d, float %f (%%f), %08x, %032b\n",       FloatToInt32(x),   aFloat(x),   aFloat(x),   aFloat(x) ); //exp: time:feed3322default: rd:TRice32 int 1089, float 1089.608276 (%f), 44883377, 01000100100010000011001101110111                                         
        break; case __LINE__: TRice32("rd:TRice32 int %d, float %f (%%f), %08x, %032b\n",          (int32_t)(x),   aFloat(x),   aFloat(x),   aFloat(x) ); //exp: time:feed3322default: rd:TRice32 int 1089, float 1089.608276 (%f), 44883377, 01000100100010000011001101110111                                         
        break; case __LINE__: TRice32("rd:TRice32 int %x, float %f (%%f), %08x, %032b\n",            0x44883377,   aFloat(x),   aFloat(x),   aFloat(x) ); //exp: time:feed3322default: rd:TRice32 int 44883377, float 1089.608276 (%f), 44883377, 01000100100010000011001101110111                                         
        break; case __LINE__: TRice32("rd:TRice32 int %d, float %f (%%f), %08x, %032b\n",                     x,   aFloat(x),   aFloat(x),   aFloat(x) ); //exp: time:feed3322default: rd:TRice32 int 1089, float 1089.608276 (%f), 44883377, 01000100100010000011001101110111                                         
        break; case __LINE__: TRice32("rd:TRice32 int %d, float %f (%%f), %08x, %032b\n",      FloatToInt32(-x),  aFloat(-x),  aFloat(-x),  aFloat(-x) ); //exp: time:feed3322default: rd:TRice32 int -1089, float -1089.608276 (%f), c4883377, 11000100100010000011001101110111                                       
        break; case __LINE__: TRice32("rd:TRice32 int %d, float %f (%%f), %08x, %032b\n",         (int32_t)(-x),  aFloat(-x),  aFloat(-x),  aFloat(-x) ); //exp: time:feed3322default: rd:TRice32 int -1089, float -1089.608276 (%f), c4883377, 11000100100010000011001101110111                                       
        break; case __LINE__: TRice32("rd:TRice32 int %X, float %f (%%f), %08x, %032b\n",           -0x44883377,  aFloat(-x),  aFloat(-x),  aFloat(-x) ); //exp: time:feed3322default: rd:TRice32 int BB77CC89, float -1089.608276 (%f), c4883377, 11000100100010000011001101110111                                        
        break; case __LINE__: TRice64("rd:TRice64 int %d, double %f (%%f), %016x, %064b\n",    DoubleToInt64(y),  aDouble(y),  aDouble(y),  aDouble(y) ); //exp: time:feed3322default: rd:TRice64 int 518, double 518.054749 (%f), 4080307020601050, 0100000010000000001100000111000000100000011000000001000001010000   
        break; case __LINE__: TRice64("rd:TRice64 int %d, double %f (%%f), %016x, %064b\n",        (int64_t)(y),  aDouble(y),  aDouble(y),  aDouble(y) ); //exp: time:feed3322default: rd:TRice64 int 518, double 518.054749 (%f), 4080307020601050, 0100000010000000001100000111000000100000011000000001000001010000   
        break; case __LINE__: TRice64("rd:TRice64 int %d, double %f (%%f), %016x, %064b\n",                   y,  aDouble(y),  aDouble(y),  aDouble(y) ); //exp: time:feed3322default: rd:TRice64 int 518, double 518.054749 (%f), 4080307020601050, 0100000010000000001100000111000000100000011000000001000001010000   
        break; case __LINE__: TRice64("rd:TRice64 int %d, double %f (%%f), %016x, %064b\n",   DoubleToInt64(-y), aDouble(-y), aDouble(-y), aDouble(-y) ); //exp: time:feed3322default: rd:TRice64 int -518, double -518.054749 (%f), c080307020601050, 1100000010000000001100000111000000100000011000000001000001010000
        break; case __LINE__: TRice64("rd:TRice64 int %d, double %f (%%f), %016x, %064b\n",       (int64_t)(-y), aDouble(-y), aDouble(-y), aDouble(-y) ); //exp: time:feed3322default: rd:TRice64 int -518, double -518.054749 (%f), c080307020601050, 1100000010000000001100000111000000100000011000000001000001010000
        break; case __LINE__: TRice64("rd:TRice64 int %X, double %f (%%f), %016x, %064b\n", -0x4080307020601050, aDouble(-y), aDouble(-y), aDouble(-y) ); //exp: time:feed3322default: rd:TRice64 int BF7FCF8FDF9FEFB0, double -518.054749 (%f), c080307020601050, 1100000010000000001100000111000000100000011000000001000001010000 

        break; case __LINE__: trice16("att: line %u\n", __LINE__ );

        break; case __LINE__: triceS("msg:With triceS:%s\n", sABCDE );               //exp: time:        default: msg:With triceS:abcde 12345
        break; case __LINE__: triceN("sig:With triceN:%s\n", sABCDE, lenABCDE );     //exp: time:        default: sig:With triceN:abcde 12345
        break; case __LINE__: TriceS("msg:With TriceS:%s\n", sABCDE );               //exp: time:    be16default: msg:With TriceS:abcde 12345
        break; case __LINE__: TriceN("sig:With TriceN:%s\n", sABCDE, lenABCDE );     //exp: time:    be16default: sig:With TriceN:abcde 12345
        break; case __LINE__: TRiceS("msg:With TRiceS:%s\n", sABCDE );               //exp: time:feed3322default: msg:With TRiceS:abcde 12345
        break; case __LINE__: TRiceN("sig:With TRiceN:%s\n", sABCDE, lenABCDE );     //exp: time:feed3322default: sig:With TRiceN:abcde 12345

        break; case __LINE__: TRICE_S(id(0), "msg:With TRICE_S:%s\n", sABCDE );             //exp: time:        default: msg:With TRICE_S:abcde 12345
        break; case __LINE__: TRICE_N(id(0), "sig:With TRICE_N:%s\n", sABCDE, lenABCDE );   //exp: time:        default: sig:With TRICE_N:abcde 12345
        break; case __LINE__: TRICE_S(Id(0), "msg:With TRICE_S:%s\n", sABCDE );             //exp: time:    be16default: msg:With TRICE_S:abcde 12345
        break; case __LINE__: TRICE_N(Id(0), "sig:With TRICE_N:%s\n", sABCDE, lenABCDE );   //exp: time:    be16default: sig:With TRICE_N:abcde 12345
        break; case __LINE__: TRICE_S(ID(0), "msg:With TRICE_S:%s\n", sABCDE );             //exp: time:feed3322default: msg:With TRICE_S:abcde 12345
        break; case __LINE__: TRICE_N(ID(0), "sig:With TRICE_N:%s\n", sABCDE, lenABCDE );   //exp: time:feed3322default: sig:With TRICE_N:abcde 12345

        break; case __LINE__: TRICE8_B(ID(0), "  %02x", sABCDE, lenABCDE );                 //exp: time:feed3322default:   61  62  63  64  65  20  31  32  33  34  35
        break; case __LINE__: trice("\n" );
        break; case __LINE__: TRICE8_B(ID(0), "BUF: %02x\n", sABCDE, lenABCDE );            //exp: time:feed3322default: BUF: 61 62 63 64 65 20 31 32 33 34 35
        break; case __LINE__: TRICE8_B(ID(0), "%4d", sABCDE, lenABCDE );                    //exp: time:feed3322default:   97  98  99 100 101  32  49  50  51  52  53
        break; case __LINE__: trice("\n" );
        break; case __LINE__: TRICE8_B(ID(0), "  %02x", b8, sizeof(b8)/sizeof(int8_t) );    //exp: time:feed3322default:   00  ff  fe  33  04  05  06  07  08  09  0a  0b  00  ff  fe  33  04  05  06  07  08  09  0a  0b
        break; case __LINE__: trice("\n" );

        break; case __LINE__: TRice8B(" %02x", b8,  sizeof(b8) /sizeof(int8_t) );    //exp: time:feed3322default:  00 ff fe 33 04 05 06 07 08 09 0a 0b 00 ff fe 33 04 05 06 07 08 09 0a 0b
        break; case __LINE__: trice("\n" );
        break; case __LINE__: Trice8B(" %02x", b8,  sizeof(b8) /sizeof(int8_t) );    //exp: time:    be16default:  00 ff fe 33 04 05 06 07 08 09 0a 0b 00 ff fe 33 04 05 06 07 08 09 0a 0b
        break; case __LINE__: trice("\n" );
        break; case __LINE__: trice8B(" %02x", b8,  sizeof(b8) /sizeof(int8_t) );    //exp: time:        default:  00 ff fe 33 04 05 06 07 08 09 0a 0b 00 ff fe 33 04 05 06 07 08 09 0a 0b
        break; case __LINE__: trice("\n" );

        break; case __LINE__: trice8B("att: %02x\n", b8,  sizeof(b8) /sizeof(int8_t) );  //exp: time:        default: att: 00 ff fe 33 04 05 06 07 08 09 0a 0b 00 ff fe 33 04 05 06 07 08 09 0a 0b
        break; case __LINE__: trice8B("rd: %02x", b8,  sizeof(b8) /sizeof(int8_t) );    //exp: time:        default: rd: 00 ff fe 33 04 05 06 07 08 09 0a 0b 00 ff fe 33 04 05 06 07 08 09 0a 0b
        break; case __LINE__: trice("\n" );
        break; case __LINE__: trice8B(" %02x\n", b8,  sizeof(b8) /sizeof(int8_t) );      //exp: time:        default:  00 ff fe 33 04 05 06 07 08 09 0a 0b 00 ff fe 33 04 05 06 07 08 09 0a 0b

        break; case __LINE__: TRICE16_B(ID(0), "  %04x", b16, sizeof(b16)/sizeof(int16_t) ); //exp: time:feed3322default:   0000  ffff  fffe  3344
        break; case __LINE__: trice("\n" );
        break; case __LINE__: TRice16B(" %04x", b16, sizeof(b16) /sizeof(int16_t) );  //exp: time:feed3322default:  0000 ffff fffe 3344
        break; case __LINE__: trice("\n" );
        break; case __LINE__: Trice16B(" %04x", b16, sizeof(b16) /sizeof(int16_t) );  //exp: time:    be16default:  0000 ffff fffe 3344
        break; case __LINE__: trice("\n" );
        break; case __LINE__: trice16B(" %04x", b16, sizeof(b16) /sizeof(int16_t) );  //exp: time:        default:  0000 ffff fffe 3344
        break; case __LINE__: trice("\n" );
        break; case __LINE__: trice16B("msg: %04x\n", b16, sizeof(b16) /sizeof(int16_t) );  //exp: time:        default: msg: 0000 ffff fffe 3344

        break; case __LINE__: TRICE32_B(ID(0), " %08x", b32, sizeof(b32)/sizeof(int32_t) );  //exp: time:feed3322default:  00000000 ffffffff fffffffe 33445555
        break; case __LINE__: trice("\n" );
        break; case __LINE__: TRice32B(" %08x", b32, sizeof(b32) /sizeof(int32_t)  ); //exp: time:feed3322default:  00000000 ffffffff fffffffe 33445555
        break; case __LINE__: trice("\n" );
        break; case __LINE__: Trice32B(" %08x", b32, sizeof(b32) /sizeof(int32_t)  ); //exp: time:    be16default:  00000000 ffffffff fffffffe 33445555
        break; case __LINE__: trice("\n" );
        break; case __LINE__: trice32B(" %08x", b32, sizeof(b32) /sizeof(int32_t)  ); //exp: time:        default:  00000000 ffffffff fffffffe 33445555
        break; case __LINE__: trice("\n" );
        break; case __LINE__: trice32B("att: %08x\n", b32, sizeof(b32) /sizeof(int32_t)  ); //exp: time:        default: att: 00000000 ffffffff fffffffe 33445555

        break; case __LINE__: TRICE64_B(ID(0), " %016x", b64, sizeof(b64)/sizeof(int64_t) );  //exp: time:feed3322default:  0000000000000000 ffffffffffffffff fffffffffffffffe 3344555566666666
        break; case __LINE__: trice("\n" );
        break; case __LINE__: TRice64B(" %016x", b64, sizeof(b64) /sizeof(int64_t)  ); //exp: time:feed3322default:  0000000000000000 ffffffffffffffff fffffffffffffffe 3344555566666666
        break; case __LINE__: trice("\n" );
        break; case __LINE__: Trice64B(" %016x", b64, sizeof(b64) /sizeof(int64_t)  ); //exp: time:    be16default:  0000000000000000 ffffffffffffffff fffffffffffffffe 3344555566666666
        break; case __LINE__: trice("\n" );
        break; case __LINE__: trice64B(" %016x", b64, sizeof(b64) /sizeof(int64_t)  ); //exp: time:        default:  0000000000000000 ffffffffffffffff fffffffffffffffe 3344555566666666
        break; case __LINE__: trice("\n" );
        break; case __LINE__: trice64B("SIG: %016x\n", b64, sizeof(b64) /sizeof(int64_t)  ); //exp: time:        default: SIG: 0000000000000000 ffffffffffffffff fffffffffffffffe 3344555566666666

        break; case __LINE__: TRICE8_F(ID(0), "info:FunctionNameW", b8,  sizeof(b8) /sizeof(int8_t) );   //exp: time:feed3322default: info:FunctionNameW(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)
        break; case __LINE__: TRice8F("call:FunctionNameW", b8,  sizeof(b8) /sizeof(int8_t) );    //exp: time:feed3322default: call:FunctionNameW(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)
        break; case __LINE__: Trice8F("call:FunctionNameW", b8,  sizeof(b8) /sizeof(int8_t) );    //exp: time:    be16default: call:FunctionNameW(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)
        break; case __LINE__: trice8F("call:FunctionNameW", b8,  sizeof(b8) /sizeof(int8_t) );    //exp: time:        default: call:FunctionNameW(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)
        break; case __LINE__: TRICE16_F(ID(0), "info:FunctionNameX", b16, sizeof(b16)/sizeof(int16_t) );  //exp: time:feed3322default: info:FunctionNameX(0000)(ffff)(fffe)(3344) 
        break; case __LINE__: TRice16F("call:FunctionNameX", b16, sizeof(b16)/sizeof(int16_t) );  //exp: time:feed3322default: call:FunctionNameX(0000)(ffff)(fffe)(3344) 
        break; case __LINE__: Trice16F("call:FunctionNameX", b16, sizeof(b16)/sizeof(int16_t) );  //exp: time:    be16default: call:FunctionNameX(0000)(ffff)(fffe)(3344) 
        break; case __LINE__: trice16F("call:FunctionNameX", b16, sizeof(b16)/sizeof(int16_t) );  //exp: time:        default: call:FunctionNameX(0000)(ffff)(fffe)(3344) 
        break; case __LINE__: TRICE32_F(ID(0), "info:FunctionNameY", b32, sizeof(b32)/sizeof(int32_t) );  //exp: time:feed3322default: info:FunctionNameY(00000000)(ffffffff)(fffffffe)(33445555)
        break; case __LINE__: TRice32F("call:FunctionNameY", b32, sizeof(b32)/sizeof(int32_t) );  //exp: time:feed3322default: call:FunctionNameY(00000000)(ffffffff)(fffffffe)(33445555)
        break; case __LINE__: Trice32F("call:FunctionNameY", b32, sizeof(b32)/sizeof(int32_t) );  //exp: time:    be16default: call:FunctionNameY(00000000)(ffffffff)(fffffffe)(33445555)
        break; case __LINE__: trice32F("call:FunctionNameY", b32, sizeof(b32)/sizeof(int32_t) );  //exp: time:        default: call:FunctionNameY(00000000)(ffffffff)(fffffffe)(33445555)
        break; case __LINE__: TRICE64_F(ID(0), "info:FunctionNameZ", b64, sizeof(b64)/sizeof(int64_t) );  //exp: time:feed3322default: info:FunctionNameZ(0000000000000000)(ffffffffffffffff)(fffffffffffffffe)(3344555566666666)
        break; case __LINE__: TRice64F("call:FunctionNameZ", b64, sizeof(b64)/sizeof(int64_t) );  //exp: time:feed3322default: call:FunctionNameZ(0000000000000000)(ffffffffffffffff)(fffffffffffffffe)(3344555566666666)
        break; case __LINE__: Trice64F("call:FunctionNameZ", b64, sizeof(b64)/sizeof(int64_t) );  //exp: time:    be16default: call:FunctionNameZ(0000000000000000)(ffffffffffffffff)(fffffffffffffffe)(3344555566666666)
        break; case __LINE__: trice64F("call:FunctionNameZ", b64, sizeof(b64)/sizeof(int64_t) );  //exp: time:        default: call:FunctionNameZ(0000000000000000)(ffffffffffffffff)(fffffffffffffffe)(3344555566666666)
        break; case __LINE__: TRiceS("msg:Hi %s!\n", five );                 //exp: time:feed3322default: msg:Hi five!
        break; case __LINE__: TriceS("msg:Hi %s!\n", five );                 //exp: time:    be16default: msg:Hi five!
        break; case __LINE__: triceS("msg:Hi %s!\n", five );                 //exp: time:        default: msg:Hi five!
        break; case __LINE__: TRiceN("msg:Hi %s!\n", five, strlen(five) );   //exp: time:feed3322default: msg:Hi five!
        break; case __LINE__: TriceN("msg:Hi %s!\n", five, strlen(five) );   //exp: time:    be16default: msg:Hi five!
        break; case __LINE__: triceN("msg:Hi %s!\n", five, strlen(five) );   //exp: time:        default: msg:Hi five!

        break; case __LINE__: TRICE_S(id(0), "sig:TRICE_S=%s\n", s );            //exp: time:        default: sig:TRICE_S=AAAAAAAAAAAA
        break; case __LINE__: TRICE_N(id(0), "sig:TRICE_N=%s\n", s, strlen(s) ); //exp: time:        default: sig:TRICE_N=AAAAAAAAAAAA
        break; case __LINE__: TRICE_S(Id(0), "sig:TRICE_S=%s\n", s );            //exp: time:    be16default: sig:TRICE_S=AAAAAAAAAAAA
        break; case __LINE__: TRICE_N(Id(0), "sig:TRICE_N=%s\n", s, strlen(s) ); //exp: time:    be16default: sig:TRICE_N=AAAAAAAAAAAA
        break; case __LINE__: TRICE_S(ID(0), "sig:TRICE_S=%s\n", s );            //exp: time:feed3322default: sig:TRICE_S=AAAAAAAAAAAA
        break; case __LINE__: TRICE_N(ID(0), "sig:TRICE_N=%s\n", s, strlen(s) ); //exp: time:feed3322default: sig:TRICE_N=AAAAAAAAAAAA
        break; case __LINE__: triceS("sig:triceS=%s\n", s );              //exp: time:        default: sig:triceS=AAAAAAAAAAAA
        break; case __LINE__: triceN("sig:triceN=%s\n", s, strlen(s) );   //exp: time:        default: sig:triceN=AAAAAAAAAAAA
        break; case __LINE__: TriceS("sig:TriceS=%s\n", s );              //exp: time:    be16default: sig:TriceS=AAAAAAAAAAAA
        break; case __LINE__: TriceN("sig:TriceN=%s\n", s, strlen(s) );   //exp: time:    be16default: sig:TriceN=AAAAAAAAAAAA
        break; case __LINE__: TRiceS("sig:TRiceS=%s\n", s );              //exp: time:feed3322default: sig:TRiceS=AAAAAAAAAAAA
        break; case __LINE__: TRiceN("sig:TRiceN=%s\n", s, strlen(s) );   //exp: time:feed3322default: sig:TRiceN=AAAAAAAAAAAA

        break; case __LINE__: trice16("att: line %u\n", __LINE__ );

        break; case __LINE__: exampleOfManualJSONencoding(); //exp: time:    be16default: att:MyStructEvaluationFunction(json:ExA{Apple:-1, Birn:2, Fisch:2.781000}
        break; case __LINE__: TRICE(Id(0), "MSG:1/11 = %g\n", aFloat( 1.0/11 ) ); //exp: time:    be16default: MSG:1/11 = 0.09090909
        break; case __LINE__: { //exp: time:feed3322default: msg:x = 5.934 = 5.934, 5.934
#if !TRICE_OFF
                              float a = (float)5.934;
                              float b = a + ((a > 0) ? 0.0005f : -0.0005f);
                              int c = (int)b;
                              int d = (int)(b * 1000) % 1000;
                              int e = (int)(1000 * (float)(a - c)); 
                              TRice("msg:x = %g = %d.%03d, %d.%03d\n", aFloat(a), c, d, c, e ); 
#endif                              
                              }

        break; case __LINE__: trice16("att: line %u\n", __LINE__ );

        break; case __LINE__: TRice("sig:Integer (indent, base, sign) - see https://yourbasic.org/golang/fmt-printf-reference-cheat-sheet/\n" );
        break; case __LINE__: TRice("rd: 15 	%d 	Base 10\n", 15 );                                        //exp: time:feed3322default: rd: 15 	15 	Base 10                                   
        break; case __LINE__: TRice("rd: +15 	%+d 	Always show sign\n", 15 );                           //exp: time:feed3322default: rd: +15 	+15 	Always show sign                          
        break; case __LINE__: TRice("rd:   15	%4d 	Pad with spaces (width 4, right justified)\n", 15 ); //exp: time:feed3322default: rd:   15	  15 	Pad with spaces (width 4, right justified)
        break; case __LINE__: TRice("rd:    15	%-4d	Pad with spaces (width 4, left justified)\n", 15 );  //exp: time:feed3322default: rd:    15	15  	Pad with spaces (width 4, left justified) 
        break; case __LINE__: TRice("rd: 0015	%04d	Pad with zeroes (width 4)\n", 15 );                  //exp: time:feed3322default: rd: 0015	0015	Pad with zeroes (width 4)                 
        break; case __LINE__: TRice("rd: 1111 	%b 	Base 2\n", 15 );                                       //exp: time:feed3322default: rd: 1111 	1111 	Base 2                                    
        break; case __LINE__: TRice("rd: 17 	%o 	Base 8\n", 15 );                                         //exp: time:feed3322default: rd: 17 	17 	Base 8                                    
        break; case __LINE__: TRice("rd: f 	%x 	Base 16, lowercase\n", 15 );                             //exp: time:feed3322default: rd: f 	f 	Base 16, lowercase                        
        break; case __LINE__: TRice("rd: F 	%X 	Base 16, uppercase\n", 15 );                             //exp: time:feed3322default: rd: F 	F 	Base 16, uppercase                        
        break; case __LINE__: TRice("rd: 0xf 	%#x 	Base 16, with leading 0x\n", 15 );                   //exp: time:feed3322default: rd: 0xf 	0xf 	Base 16, with leading 0x                  

        break; case __LINE__: trice16("att: line %u\n", __LINE__ );

        break; case __LINE__: trice("sig:Character (quoted, Unicode)\n" );
        break; case __LINE__: trice("rd: A 	%c 	Character\n", 'A' );                                     //exp: time:        default: rd: A 	A 	Character
        break; case __LINE__: trice("rd: 'A' 	%q 	Quoted character\n", 'A' );                            //exp: time:        default: rd: 'A' 	'A' 	Quoted character
    //  break; case __LINE__: trice("rd: U+0041 	%U 	Unicode\n", 'A' );                        // todo: detect %U to avoid message triceType = TRICE0 ParamSpace = 4 not matching with bitWidth  0 and paramCount 0 - ignoring package
    //  break; case __LINE__: trice("rd: U+0041 'A' 	%#U 	Unicode with character\n", 'A' ); // todo: detect %#U to avoid message triceType = TRICE0 ParamSpace = 4 not matching with bitWidth  0 and paramCount 0 - ignoring package

        break; case __LINE__: trice("sig:Boolean (true/false)\n" );
        break; case __LINE__: trice("rd:Use %%t to format a boolean as true (%t) or false (%t).\n", 1, 0 ); //exp: time:        default: rd:Use %t to format a boolean as true (true) or false (false).

        break; case __LINE__: trice("sig:Pointer (hex)\n" );
        break; case __LINE__: trice8 ("rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x84 );               //exp: time:        default: rd:Use %p to format a pointer in base 16 notation with leading 0x. (84)              
        break; case __LINE__: trice16("rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x1234 );             //exp: time:        default: rd:Use %p to format a pointer in base 16 notation with leading 0x. (1234)            
        break; case __LINE__: trice32("rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0xaabbccdd );         //exp: time:        default: rd:Use %p to format a pointer in base 16 notation with leading 0x. (aabbccdd)        
        break; case __LINE__: trice64("rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x1122334455667788 ); //exp: time:        default: rd:Use %p to format a pointer in base 16 notation with leading 0x. (1122334455667788)
        break; case __LINE__: trice  ("rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0xaabbccdd);          //exp: time:        default: rd:Use %p to format a pointer in base 16 notation with leading 0x. (aabbccdd)        

        break; case __LINE__: trice16("att: line %u\n", __LINE__ );

        break; case __LINE__: trice("sig:Float (indent, precision, scientific notation)\n" );
        break; case __LINE__: trice("rd: 1.234560e+02		%e 	%%e Scientific notation\n", aFloat(123.456) );                               //exp: time:        default: rd: 1.234560e+02		1.234560e+02 	%e Scientific notation
        break; case __LINE__: trice("rd: 123.456000		%f 	%%f Decimal point, no exponent\n", aFloat(123.456) );                          //exp: time:        default: rd: 123.456000		123.456001 	%f Decimal point, no exponent
        break; case __LINE__: trice("rd: 123.46			%.2f		%%.2f Default width, precision 2\n", aFloat(123.456) );                      //exp: time:        default: rd: 123.46			123.46		%.2f Default width, precision 2
        break; case __LINE__: trice("rd: ␣␣123.46		%8.2f	%%8.2f Width 8, precision 2\n", aFloat(123.456) );                              //exp: time:        default: rd: ␣␣123.46		  123.46	%8.2f Width 8, precision 2
        break; case __LINE__: trice("rd: 123.456		%g		%%g Exponent as needed, necessary digits only\n", aFloat(123.456) );           //exp: time:        default: rd: 123.456		123.456		%g Exponent as needed, necessary digits only

        break; case __LINE__: trice("sig:Double (indent, precision, scientific notation)\n" );
        break; case __LINE__: trice64("rd: 1.234560e+02		%e 	%%e Scientific notation\n", aDouble(123.456) );                            //exp: time:        default: rd: 1.234560e+02		1.234560e+02 	%e Scientific notation
        break; case __LINE__: trice64("rd: 123.456000		%f 	%%f Decimal point, no exponent\n", aDouble(123.456) );                       //exp: time:        default: rd: 123.456000		123.456000 	%f Decimal point, no exponent
        break; case __LINE__: trice64("rd: 123.46			%.2f    	%%.2f Default width, precision 2\n", aDouble(123.456) );                 //exp: time:        default: rd: 123.46			123.46    	%.2f Default width, precision 2
        break; case __LINE__: trice64("rd: ␣␣123.46		%8.2f 	%%8.2f Width 8, precision 2\n", aDouble(123.456) );                         //exp: time:        default: rd: ␣␣123.46		  123.46 	%8.2f Width 8, precision 2
        break; case __LINE__: trice64("rd: 123.456		%g		%%g Exponent as needed, necessary digits only\n", aDouble(123.456) );        //exp: time:        default: rd: 123.456		123.456		%g Exponent as needed, necessary digits only

        break; case __LINE__: trice("sig:String or byte slice (quote, indent, hex)\n" );
        break; case __LINE__: s = "café"; triceS("rd: café 			%s		Plain string\n", s );                //exp: time:        default: rd: café 			café		Plain string
        break; case __LINE__: s = "café"; triceS("rd: ␣␣café 		%6s 		Width 6, right justify\n", s );     //exp: time:        default: rd: ␣␣café 		  café 		Width 6, right justify
        break; case __LINE__: s = "café"; triceS("rd: café␣␣ 		%-6s 		Width 6, left justify\n", s );      //exp: time:        default: rd: café␣␣ 		café   		Width 6, left justify
        break; case __LINE__: s = "café"; triceS("rd: \"café\" 		%q		Quoted string\n", s );             //exp: time:        default: rd: \"café\" 		"café"		Quoted string
        break; case __LINE__: s = "café"; triceS("rd: 636166c3a9 		%x	Hex dump of byte values\n", s );   //exp: time:        default: rd: 636166c3a9 		636166c3a9	Hex dump of byte values 
        break; case __LINE__: s = "café"; triceS("rd: 63 61 66 c3 a9 	% x	Hex dump with spaces\n", s );    //exp: time:        default: rd: 63 61 66 c3 a9 	63 61 66 c3 a9	Hex dump with spaces

        break; case __LINE__: trice16("att: line %u\n", __LINE__ );
        break; case __LINE__: exampleOfManualSerialization(); // ATTENTION: This occupies ~1024 bytes in one half buffer when double buffer ist used!

        // This was a CGO compiler issue:
        break; case __LINE__: TRice("info:12 default bit width values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 32-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //exp: time:feed3322default: info:12 default bit width values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and a 32-bit stamp.
        break; case __LINE__: Trice("info:12 default bit width values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 16-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //exp: time:    be16default: info:12 default bit width values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and a 16-bit stamp.
        break; case __LINE__: trice("info:12 default bit width values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and without stamp.\n" , -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //exp: time:        default: info:12 default bit width values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and without stamp.

        break; case __LINE__: trice16("att: line %u\n", __LINE__ );

        break; case __LINE__: trice("sig:Runtime generated strings\n" );
        break; case __LINE__: 
        {
            #if !TRICE_OFF
            char* s0 = "AAAAAAAAAAAA";
            #endif
            TRICE32(ID(0), "dbg:len=%u:", strlen(s0) );
            TRICE_S(ID(0), "sig:%s\n", s0 );
        }
        break; case __LINE__: 
        {
            #if !TRICE_OFF
            char* s1 = "\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBCCCCCCCC\
";
#endif
                TRICE32(ID(0), "dbg:len=%u:", strlen(s1) );
                TRICE_S(ID(0), "sig:%s\n", s1 );
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
            TRICE32(ID(0), "dbg:len=%u: ", strlen(s2) );
            TRICE_S(ID(0), "sig:%s\n", s2 );
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
            TRICE32(ID(0), "dbg:len=%u: ", strlen(s3) );
            TRICE_S(ID(0), "sig:%s\n", s3 );
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
            TRICE32(ID(0), "dbg:len=%u: ", strlen(s4) );
            TRICE_S(ID(0), "sig:%s\n", s4 );
        }


#if TRICE_CGO == 1 || defined(TRICE_FULL_CHECK) 

        break; case __LINE__: trice  ("rd:%E (%%E)\n",  aFloat(-555555555.5555555555) ); //exp: time:        default: rd:-5.555556E+08 (%E)
        break; case __LINE__: trice  ("rd:%F (%%F)\n",  aFloat(-555555555.5555555555) ); //exp: time:        default: rd:-555555584.000000 (%F)
        break; case __LINE__: trice  ("rd:%G (%%G)\n",  aFloat(-555555555.5555555555) ); //exp: time:        default: rd:-5.555556E+08 (%G)
        break; case __LINE__: trice64("rd:%E (%%E)\n", aDouble(-555555555.5555555555) ); //exp: time:        default: rd:-5.555556E+08 (%E)
        break; case __LINE__: trice64("rd:%F (%%F)\n", aDouble(-555555555.5555555555) ); //exp: time:        default: rd:-555555555.555556 (%F)
        break; case __LINE__: trice64("rd:%G (%%G)\n", aDouble(-555555555.5555555555) ); //exp: time:        default: rd:-5.555555555555556E+08 (%G)
        
        break; case __LINE__: trice  ("rd:%e (%%e)\n",  aFloat(-555555555.5555555555) ); //exp: time:        default: rd:-5.555556e+08 (%e)
        break; case __LINE__: trice  ("rd:%f (%%f)\n",  aFloat(-555555555.5555555555) ); //exp: time:        default: rd:-555555584.000000 (%f)
        break; case __LINE__: trice  ("rd:%g (%%g)\n",  aFloat(-555555555.5555555555) ); //exp: time:        default: rd:-5.555556e+08 (%g)
        break; case __LINE__: trice64("rd:%e (%%e)\n", aDouble(-555555555.5555555555) ); //exp: time:        default: rd:-5.555556e+08 (%e)
        break; case __LINE__: trice64("rd:%f (%%f)\n", aDouble(-555555555.5555555555) ); //exp: time:        default: rd:-555555555.555556 (%f)
        break; case __LINE__: trice64("rd:%g (%%g)\n", aDouble(-555555555.5555555555) ); //exp: time:        default: rd:-5.555555555555556e+08 (%g)

        break; case __LINE__: trice32("msg:%u (%%u)\n", -1 ); //exp: time:        default: msg:4294967295 (%u)
        break; case __LINE__: trice32("msg:%b (%%b)\n", -1 ); //exp: time:        default: msg:11111111111111111111111111111111 (%b)
        break; case __LINE__: trice32("msg:%o (%%o)\n", -1 ); //exp: time:        default: msg:37777777777 (%o)
        break; case __LINE__: trice32("msg:%O (%%O)\n", -1 ); //exp: time:        default: msg:0o37777777777 (%O)
        break; case __LINE__: trice32("msg:%X (%%X)\n", -1 ); //exp: time:        default: msg:FFFFFFFF (%X)
        break; case __LINE__: trice32("msg:%x (%%x)\n", -1 ); //exp: time:        default: msg:ffffffff (%x)
        break; case __LINE__: trice32("msg:%d (%%d)\n", -1 ); //exp: time:        default: msg:-1 (%d)
        break; case __LINE__: trice64("msg:%u (%%u)\n", -1 ); //exp: time:        default: msg:18446744073709551615 (%u)
        break; case __LINE__: trice64("msg:%b (%%b)\n", -1 ); //exp: time:        default: msg:1111111111111111111111111111111111111111111111111111111111111111 (%b)
        break; case __LINE__: trice64("msg:%o (%%o)\n", -1 ); //exp: time:        default: msg:1777777777777777777777 (%o)
        break; case __LINE__: trice64("msg:%O (%%O)\n", -1 ); //exp: time:        default: msg:0o1777777777777777777777 (%O)
        break; case __LINE__: trice64("msg:%X (%%X)\n", -1 ); //exp: time:        default: msg:FFFFFFFFFFFFFFFF (%X)
        break; case __LINE__: trice64("msg:%x (%%x)\n", -1 ); //exp: time:        default: msg:ffffffffffffffff (%x)

    /**/break; case __LINE__: TRice        ("value=%d\n", -2  ); //exp: time:feed3322default: value=-2
    /**/break; case __LINE__: Trice        ("value=%d\n", -2  ); //exp: time:    be16default: value=-2
    /**/break; case __LINE__: trice        ("value=%d\n", -2  ); //exp: time:        default: value=-2
        break; case __LINE__: TRice8       ("value=%d\n", -2  ); //exp: time:feed3322default: value=-2
        break; case __LINE__: Trice8       ("value=%d\n", -2  ); //exp: time:    be16default: value=-2
        break; case __LINE__: trice8       ("value=%d\n", -2  ); //exp: time:        default: value=-2
        break; case __LINE__: TRice16      ("value=%d\n", -2  ); //exp: time:feed3322default: value=-2
        break; case __LINE__: Trice16      ("value=%d\n", -2  ); //exp: time:    be16default: value=-2
        break; case __LINE__: trice16      ("value=%d\n", -2  ); //exp: time:        default: value=-2
        break; case __LINE__: TRice32      ("value=%d\n", -2  ); //exp: time:feed3322default: value=-2
        break; case __LINE__: Trice32      ("value=%d\n", -2  ); //exp: time:    be16default: value=-2
        break; case __LINE__: trice32      ("value=%d\n", -2  ); //exp: time:        default: value=-2
        break; case __LINE__: TRice64      ("value=%d\n", -2  ); //exp: time:feed3322default: value=-2
        break; case __LINE__: Trice64      ("value=%d\n", -2  ); //exp: time:    be16default: value=-2
        break; case __LINE__: trice64      ("value=%d\n", -2  ); //exp: time:        default: value=-2
        
        break; case __LINE__: TRice_1      ("value=%d\n", -2  ); //exp: time:feed3322default: value=-2
        break; case __LINE__: Trice_1      ("value=%d\n", -2  ); //exp: time:    be16default: value=-2
        break; case __LINE__: trice_1      ("value=%d\n", -2  ); //exp: time:        default: value=-2
        break; case __LINE__: TRice8_1     ("value=%d\n", -2  ); //exp: time:feed3322default: value=-2
        break; case __LINE__: Trice8_1     ("value=%d\n", -2  ); //exp: time:    be16default: value=-2
        break; case __LINE__: trice8_1     ("value=%d\n", -2  ); //exp: time:        default: value=-2
        break; case __LINE__: TRice16_1    ("value=%d\n", -2  ); //exp: time:feed3322default: value=-2
        break; case __LINE__: Trice16_1    ("value=%d\n", -2  ); //exp: time:    be16default: value=-2
        break; case __LINE__: trice16_1    ("value=%d\n", -2  ); //exp: time:        default: value=-2
        break; case __LINE__: TRice32_1    ("value=%d\n", -2  ); //exp: time:feed3322default: value=-2
        break; case __LINE__: Trice32_1    ("value=%d\n", -2  ); //exp: time:    be16default: value=-2
        break; case __LINE__: trice32_1    ("value=%d\n", -2  ); //exp: time:        default: value=-2
        break; case __LINE__: TRice64_1    ("value=%d\n", -2  ); //exp: time:feed3322default: value=-2
        break; case __LINE__: Trice64_1    ("value=%d\n", -2  ); //exp: time:    be16default: value=-2
        break; case __LINE__: trice64_1    ("value=%d\n", -2  ); //exp: time:        default: value=-2
        
        break; case __LINE__: TRice        ("no value" ); //exp: time:feed3322default: no value
        break; case __LINE__: Trice        ("no value" ); //exp: time:    be16default: no value
        break; case __LINE__: trice        ("no value" ); //exp: time:        default: no value
        break; case __LINE__: TRice8       ("no value" ); //exp: time:feed3322default: no value
        break; case __LINE__: Trice8       ("no value" ); //exp: time:    be16default: no value
        break; case __LINE__: trice8       ("no value" ); //exp: time:        default: no value
        break; case __LINE__: TRice16      ("no value" ); //exp: time:feed3322default: no value
        break; case __LINE__: Trice16      ("no value" ); //exp: time:    be16default: no value
        break; case __LINE__: trice16      ("no value" ); //exp: time:        default: no value
        break; case __LINE__: TRice32      ("no value" ); //exp: time:feed3322default: no value
        break; case __LINE__: Trice32      ("no value" ); //exp: time:    be16default: no value
        break; case __LINE__: trice32      ("no value" ); //exp: time:        default: no value
        break; case __LINE__: TRice64      ("no value" ); //exp: time:feed3322default: no value
        break; case __LINE__: Trice64      ("no value" ); //exp: time:    be16default: no value
        break; case __LINE__: trice64      ("no value" ); //exp: time:        default: no value
    
        break; case __LINE__: TRice_0      ("no value" ); //exp: time:feed3322default: no value
        break; case __LINE__: Trice_0      ("no value" ); //exp: time:    be16default: no value
        break; case __LINE__: trice_0      ("no value" ); //exp: time:        default: no value
        break; case __LINE__: TRice8_0     ("no value" ); //exp: time:feed3322default: no value
        break; case __LINE__: Trice8_0     ("no value" ); //exp: time:    be16default: no value
        break; case __LINE__: trice8_0     ("no value" ); //exp: time:        default: no value
        break; case __LINE__: TRice16_0    ("no value" ); //exp: time:feed3322default: no value
        break; case __LINE__: Trice16_0    ("no value" ); //exp: time:    be16default: no value
        break; case __LINE__: trice16_0    ("no value" ); //exp: time:        default: no value
        break; case __LINE__: TRice32_0    ("no value" ); //exp: time:feed3322default: no value
        break; case __LINE__: Trice32_0    ("no value" ); //exp: time:    be16default: no value
        break; case __LINE__: trice32_0    ("no value" ); //exp: time:        default: no value
        break; case __LINE__: TRice64_0    ("no value" ); //exp: time:feed3322default: no value
        break; case __LINE__: Trice64_0    ("no value" ); //exp: time:    be16default: no value
        break; case __LINE__: trice64_0    ("no value" ); //exp: time:        default: no value
        break; case __LINE__: TRice0       ("no value" ); //exp: time:feed3322default: no value
        break; case __LINE__: Trice0       ("no value" ); //exp: time:    be16default: no value
        break; case __LINE__: trice0       ("no value" ); //exp: time:        default: no value
        break; case __LINE__: TRice        ("no value" ); //exp: time:feed3322default: no value
        break; case __LINE__: Trice        ("no value" ); //exp: time:    be16default: no value
        break; case __LINE__: trice        ("no value" ); //exp: time:        default: no value

        break; case __LINE__: TRice("info:This is a message with one value %d and a 32-bit stamp.\n", -2  ); //exp: time:feed3322default: info:This is a message with one value -2 and a 32-bit stamp.
        break; case __LINE__: Trice("info:This is a message with one value %d and a 16-bit stamp.\n", -2  ); //exp: time:    be16default: info:This is a message with one value -2 and a 16-bit stamp.
        break; case __LINE__: trice("info:This is a message with one value %d and without stamp.\n" , -2  ); //exp: time:        default: info:This is a message with one value -2 and without stamp.

        break; case __LINE__: TRICE(ID(0), "info:This is a message without values and a 32-bit stamp.\n" );        //exp: time:feed3322default: info:This is a message without values and a 32-bit stamp.
        break; case __LINE__: TRICE(Id(0), "info:This is a message without values and a 16-bit stamp.\n" );        //exp: time:    be16default: info:This is a message without values and a 16-bit stamp.
        break; case __LINE__: TRICE(id(0), "info:This is a message without values and without stamp.\n"  );        //exp: time:        default: info:This is a message without values and without stamp.

        break; case __LINE__: TRICE(ID(0), "info:This is a message with one value %d and a 32-bit stamp.\n", -2  ); //exp: time:feed3322default: info:This is a message with one value -2 and a 32-bit stamp.
        break; case __LINE__: TRICE(Id(0), "info:This is a message with one value %d and a 16-bit stamp.\n", -2  ); //exp: time:    be16default: info:This is a message with one value -2 and a 16-bit stamp.
        break; case __LINE__: TRICE(id(0), "info:This is a message with one value %d and without stamp.\n" , -2  ); //exp: time:        default: info:This is a message with one value -2 and without stamp.

        break; case __LINE__: TRICE(ID(0), "info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 32-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 );   //exp: time:feed3322default: info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and a 32-bit stamp.
        break; case __LINE__: TRICE(Id(0), "info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 16-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 );   //exp: time:    be16default: info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and a 16-bit stamp.
        break; case __LINE__: TRICE(id(0), "info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and without stamp.\n" , -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 );   //exp: time:        default: info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and without stamp.

        break; case __LINE__: TRICE32(ID(0), "info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 32-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //exp: time:feed3322default: info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and a 32-bit stamp.
        break; case __LINE__: TRICE32(Id(0), "info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 16-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //exp: time:    be16default: info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and a 16-bit stamp.
        break; case __LINE__: TRICE32(id(0), "info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and without stamp.\n" , -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //exp: time:        default: info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and without stamp.

        break; case __LINE__: TRICE8   (id(0), "msg:value=%u\n", 200 ); //exp: time:        default: msg:value=200
        break; case __LINE__: TRICE8_1 (id(0), "msg:value=%u\n", 200 ); //exp: time:        default: msg:value=200
        break; case __LINE__: TRICE8   (Id(0), "msg:value=%u\n", 200 ); //exp: time:    be16default: msg:value=200
        break; case __LINE__: TRICE8_1 (Id(0), "msg:value=%u\n", 200 ); //exp: time:    be16default: msg:value=200
        break; case __LINE__: TRICE8   (ID(0), "msg:value=%u\n", 200 ); //exp: time:feed3322default: msg:value=200
        break; case __LINE__: TRICE8_1 (ID(0), "msg:value=%u\n", 200 ); //exp: time:feed3322default: msg:value=200
        break; case __LINE__: trice8   ("msg:value=%u\n", 200 ); //exp: time:        default: msg:value=200
        break; case __LINE__: trice8_1 ("msg:value=%u\n", 200 ); //exp: time:        default: msg:value=200
        break; case __LINE__: Trice8   ("msg:value=%u\n", 200 ); //exp: time:    be16default: msg:value=200
        break; case __LINE__: Trice8_1 ("msg:value=%u\n", 200 ); //exp: time:    be16default: msg:value=200
        break; case __LINE__: TRice8   ("msg:value=%u\n", 200 ); //exp: time:feed3322default: msg:value=200
        break; case __LINE__: TRice8_1 ("msg:value=%u\n", 200 ); //exp: time:feed3322default: msg:value=200

        break; case __LINE__: TRICE16  (id(0), "msg:value=%u\n", 200 ); //exp: time:        default: msg:value=200
        break; case __LINE__: TRICE16_1(id(0), "msg:value=%u\n", 200 ); //exp: time:        default: msg:value=200
        break; case __LINE__: TRICE16  (Id(0), "msg:value=%u\n", 200 ); //exp: time:    be16default: msg:value=200
        break; case __LINE__: TRICE16_1(Id(0), "msg:value=%u\n", 200 ); //exp: time:    be16default: msg:value=200
        break; case __LINE__: TRICE16  (ID(0), "msg:value=%u\n", 200 ); //exp: time:feed3322default: msg:value=200
        break; case __LINE__: TRICE16_1(ID(0), "msg:value=%u\n", 200 ); //exp: time:feed3322default: msg:value=200
        break; case __LINE__: trice16  ("msg:value=%u\n", 200 ); //exp: time:        default: msg:value=200
        break; case __LINE__: trice16_1("msg:value=%u\n", 200 ); //exp: time:        default: msg:value=200
        break; case __LINE__: Trice16  ("msg:value=%u\n", 200 ); //exp: time:    be16default: msg:value=200
        break; case __LINE__: Trice16_1("msg:value=%u\n", 200 ); //exp: time:    be16default: msg:value=200
        break; case __LINE__: TRice16  ("msg:value=%u\n", 200 ); //exp: time:feed3322default: msg:value=200
        break; case __LINE__: TRice16_1("msg:value=%u\n", 200 ); //exp: time:feed3322default: msg:value=200

        break; case __LINE__: TRICE32  (id(0), "msg:value=%u\n", 200 ); //exp: time:        default: msg:value=200
        break; case __LINE__: TRICE32_1(id(0), "msg:value=%u\n", 200 ); //exp: time:        default: msg:value=200
        break; case __LINE__: TRICE32  (Id(0), "msg:value=%u\n", 200 ); //exp: time:    be16default: msg:value=200
        break; case __LINE__: TRICE32_1(Id(0), "msg:value=%u\n", 200 ); //exp: time:    be16default: msg:value=200
        break; case __LINE__: TRICE32  (ID(0), "msg:value=%u\n", 200 ); //exp: time:feed3322default: msg:value=200
        break; case __LINE__: TRICE32_1(ID(0), "msg:value=%u\n", 200 ); //exp: time:feed3322default: msg:value=200
        break; case __LINE__: trice32  ("msg:value=%u\n", 200 ); //exp: time:        default: msg:value=200
        break; case __LINE__: trice32_1("msg:value=%u\n", 200 ); //exp: time:        default: msg:value=200
        break; case __LINE__: Trice32  ("msg:value=%u\n", 200 ); //exp: time:    be16default: msg:value=200
        break; case __LINE__: Trice32_1("msg:value=%u\n", 200 ); //exp: time:    be16default: msg:value=200
        break; case __LINE__: TRice32  ("msg:value=%u\n", 200 ); //exp: time:feed3322default: msg:value=200
        break; case __LINE__: TRice32_1("msg:value=%u\n", 200 ); //exp: time:feed3322default: msg:value=200

        break; case __LINE__: TRICE64  (id(0), "msg:value=%u\n", 200 ); //exp: time:        default: msg:value=200
        break; case __LINE__: TRICE64_1(id(0), "msg:value=%u\n", 200 ); //exp: time:        default: msg:value=200
        break; case __LINE__: TRICE64  (Id(0), "msg:value=%u\n", 200 ); //exp: time:    be16default: msg:value=200
        break; case __LINE__: TRICE64_1(Id(0), "msg:value=%u\n", 200 ); //exp: time:    be16default: msg:value=200
        break; case __LINE__: TRICE64  (ID(0), "msg:value=%u\n", 200 ); //exp: time:feed3322default: msg:value=200
        break; case __LINE__: TRICE64_1(ID(0), "msg:value=%u\n", 200 ); //exp: time:feed3322default: msg:value=200
        break; case __LINE__: trice64  ("msg:value=%u\n", 200 ); //exp: time:        default: msg:value=200
        break; case __LINE__: trice64_1("msg:value=%u\n", 200 ); //exp: time:        default: msg:value=200
        break; case __LINE__: Trice64  ("msg:value=%u\n", 200 ); //exp: time:    be16default: msg:value=200
        break; case __LINE__: Trice64_1("msg:value=%u\n", 200 ); //exp: time:    be16default: msg:value=200
        break; case __LINE__: TRice64  ("msg:value=%u\n", 200 ); //exp: time:feed3322default: msg:value=200
        break; case __LINE__: TRice64_1("msg:value=%u\n", 200 ); //exp: time:feed3322default: msg:value=200

        // mixed 16-bit and 32-bit timestamp 

        break; case __LINE__: TRice8("msg:value=%d\n", -1 );                                                                                            //exp: time:feed3322default: msg:value=-1
        break; case __LINE__: Trice8("msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: msg:value=-1, -2
        break; case __LINE__: trice8("msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: msg:value=-1, -2, -3
        break; case __LINE__: TRice8("msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice8("msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice8("msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice8("msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:feed3322default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice8("msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice8("msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice8("msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:feed3322default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice8("msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice8("msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice16("msg:value=%d\n", -1 );                                                                                            //exp: time:feed3322default: msg:value=-1
        break; case __LINE__: Trice16("msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: msg:value=-1, -2
        break; case __LINE__: trice16("msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: msg:value=-1, -2, -3
        break; case __LINE__: TRice16("msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice16("msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice16("msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice16("msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:feed3322default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice16("msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice16("msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice16("msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:feed3322default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice16("msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice16("msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice32("msg:value=%d\n", -1 );                                                                                            //exp: time:feed3322default: msg:value=-1
        break; case __LINE__: Trice32("msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: msg:value=-1, -2
        break; case __LINE__: trice32("msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: msg:value=-1, -2, -3
        break; case __LINE__: TRice32("msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice32("msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice32("msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice32("msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:feed3322default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice32("msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice32("msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice32("msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:feed3322default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice32("msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice32("msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice64("msg:value=%d\n", -1 );                                                                                            //exp: time:feed3322default: msg:value=-1
        break; case __LINE__: Trice64("msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: msg:value=-1, -2
        break; case __LINE__: trice64("msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: msg:value=-1, -2, -3
        break; case __LINE__: TRice64("msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice64("msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice64("msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice64("msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:feed3322default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice64("msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice64("msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice64("msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:feed3322default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice64("msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice64("msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice8_1 ("msg:value=%d\n", -1 );                                                                                            //exp: time:feed3322default: msg:value=-1
        break; case __LINE__: Trice8_2 ("msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: msg:value=-1, -2
        break; case __LINE__: trice8_3 ("msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: msg:value=-1, -2, -3
        break; case __LINE__: TRice8_4 ("msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice8_5 ("msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice8_6 ("msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice8_7 ("msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:feed3322default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice8_8 ("msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice8_9 ("msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice8_10("msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:feed3322default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice8_11("msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice8_12("msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice16_1 ("msg:value=%d\n", -1 );                                                                                            //exp: time:feed3322default: msg:value=-1
        break; case __LINE__: Trice16_2 ("msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: msg:value=-1, -2
        break; case __LINE__: trice16_3 ("msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: msg:value=-1, -2, -3
        break; case __LINE__: TRice16_4 ("msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice16_5 ("msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice16_6 ("msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice16_7 ("msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:feed3322default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice16_8 ("msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice16_9 ("msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice16_10("msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:feed3322default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice16_11("msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice16_12("msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice32_1 ("msg:value=%d\n", -1 );                                                                                            //exp: time:feed3322default: msg:value=-1
        break; case __LINE__: Trice32_2 ("msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: msg:value=-1, -2
        break; case __LINE__: trice32_3 ("msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: msg:value=-1, -2, -3
        break; case __LINE__: TRice32_4 ("msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice32_5 ("msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice32_6 ("msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice32_7 ("msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:feed3322default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice32_8 ("msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice32_9 ("msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice32_10("msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:feed3322default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice32_11("msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice32_12("msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice64_1 ("msg:value=%d\n", -1 );                                                                                            //exp: time:feed3322default: msg:value=-1
        break; case __LINE__: Trice64_2 ("msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: msg:value=-1, -2
        break; case __LINE__: trice64_3 ("msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: msg:value=-1, -2, -3
        break; case __LINE__: TRice64_4 ("msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice64_5 ("msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice64_6 ("msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice64_7 ("msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:feed3322default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice64_8 ("msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice64_9 ("msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice64_10("msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:feed3322default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice64_11("msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice64_12("msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        // use cases for 8-bit data
        break; case __LINE__: trice8     ("value=%d\n", -2  ); //exp: time:        default: value=-2
        break; case __LINE__: Trice8     ("value=%d\n", -2  ); //exp: time:    be16default: value=-2
        break; case __LINE__: TRice8     ("value=%d\n", -2  ); //exp: time:feed3322default: value=-2
        break; case __LINE__: trice8_1   ("value=%d\n", -2  ); //exp: time:        default: value=-2
        break; case __LINE__: Trice8_1   ("value=%d\n", -2  ); //exp: time:    be16default: value=-2
        break; case __LINE__: TRice8_1   ("value=%d\n", -2  ); //exp: time:feed3322default: value=-2

        // use cases for 16-bit data
        break; case __LINE__: trice16    ("value=%d\n", -2  ); //exp: time:        default: value=-2
        break; case __LINE__: Trice16    ("value=%d\n", -2  ); //exp: time:    be16default: value=-2
        break; case __LINE__: TRice16    ("value=%d\n", -2  ); //exp: time:feed3322default: value=-2
        break; case __LINE__: trice16_1  ("value=%d\n", -2  ); //exp: time:        default: value=-2
        break; case __LINE__: Trice16_1  ("value=%d\n", -2  ); //exp: time:    be16default: value=-2
        break; case __LINE__: TRice16_1  ("value=%d\n", -2  ); //exp: time:feed3322default: value=-2

        // use cases for 32-bit data
        break; case __LINE__: trice32    ("value=%d\n", -2  ); //exp: time:        default: value=-2
        break; case __LINE__: Trice32    ("value=%d\n", -2  ); //exp: time:    be16default: value=-2
        break; case __LINE__: TRice32    ("value=%d\n", -2  ); //exp: time:feed3322default: value=-2
        break; case __LINE__: trice32_1  ("value=%d\n", -2  ); //exp: time:        default: value=-2
        break; case __LINE__: Trice32_1  ("value=%d\n", -2  ); //exp: time:    be16default: value=-2
        break; case __LINE__: TRice32_1  ("value=%d\n", -2  ); //exp: time:feed3322default: value=-2

        // use cases for 64-bit data
        break; case __LINE__: trice64    ("value=%d\n", -2  ); //exp: time:        default: value=-2
        break; case __LINE__: Trice64    ("value=%d\n", -2  ); //exp: time:    be16default: value=-2
        break; case __LINE__: TRice64    ("value=%d\n", -2  ); //exp: time:feed3322default: value=-2
        break; case __LINE__: trice64_1  ("value=%d\n", -2  ); //exp: time:        default: value=-2
        break; case __LINE__: Trice64_1  ("value=%d\n", -2  ); //exp: time:    be16default: value=-2
        break; case __LINE__: TRice64_1  ("value=%d\n", -2  ); //exp: time:feed3322default: value=-2

        // legacy use cases
        break; case __LINE__: TRICE      (ID(0), "value=%d\n", -2 ); //exp: time:feed3322default: value=-2
        break; case __LINE__: TRICE      (id(0), "value=%d\n", -2 ); //exp: time:        default: value=-2
        break; case __LINE__: TRICE      (Id(0), "value=%d\n", -2 ); //exp: time:    be16default: value=-2
        break; case __LINE__: TRICE      (ID(0), "value=%d\n", -2 ); //exp: time:feed3322default: value=-2

        // default stamp
        break; case __LINE__: TRICE8     (ID(0), "value=%d\n", -2 ); //exp: time:feed3322default: value=-2
        break; case __LINE__: TRICE8_1   (ID(0), "value=%d\n", -2 ); //exp: time:feed3322default: value=-2

        // no stamp
        break; case __LINE__: trice8     ("value=%d\n", -2 ); //exp: time:        default: value=-2
        break; case __LINE__: trice8_1   ("value=%d\n", -2 ); //exp: time:        default: value=-2
        break; case __LINE__: TRICE8     (id(0), "value=%d\n", -2 ); //exp: time:        default: value=-2
        break; case __LINE__: TRICE8_1   (id(0), "value=%d\n", -2 ); //exp: time:        default: value=-2

        // 16-bit stamp:
        break; case __LINE__: Trice8     ("value=%d\n", -2 ); //exp: time:    be16default: value=-2
        break; case __LINE__: Trice8_1   ("value=%d\n", -2 ); //exp: time:    be16default: value=-2
        break; case __LINE__: TRICE8     (Id(0), "value=%d\n", -2 ); //exp: time:    be16default: value=-2
        break; case __LINE__: TRICE8_1   (Id(0), "value=%d\n", -2 ); //exp: time:    be16default: value=-2

        // 32-bit stamp:
        break; case __LINE__: TRice8     ("value=%d\n", -2 ); //exp: time:feed3322default: value=-2
        break; case __LINE__: TRice8_1   ("value=%d\n", -2 ); //exp: time:feed3322default: value=-2
        break; case __LINE__: TRICE8     (ID(0), "value=%d\n", -2 ); //exp: time:feed3322default: value=-2
        break; case __LINE__: TRICE8_1   (ID(0), "value=%d\n", -2 ); //exp: time:feed3322default: value=-2

        // tags
      //break; case __LINE__: TRICE(id(0), "FATAL:magenta+b:red\n" );           //exp: time:        default: FATAL:magenta+b:red
      //break; case __LINE__: TRICE(id(0), "CRITICAL:red+i:default+h\n" );      //exp: time:        default: CRITICAL:red+i:default+h
      //break; case __LINE__: TRICE(id(0), "EMERGENCY:red+i:blue\n" );          //exp: time:        default: EMERGENCY:red+i:blue
      //break; case __LINE__: TRICE(id(0), "ERROR:11:red\n" );                  //exp: time:        default: ERROR:11:red
      //break; case __LINE__: TRICE(id(0), "WARNING:11+i:red\n" );              //exp: time:        default: WARNING:11+i:red
      //break; case __LINE__: TRICE(id(0), "ATTENTION:11:green\n" );            //exp: time:        default: ATTENTION:11:green
      //break; case __LINE__: TRICE(id(0), "INFO:cyan+b:default+h\n" );         //exp: time:        default: INFO:cyan+b:default+h
      //break; case __LINE__: TRICE(id(0), "DEBUG:130+i\n" );                   //exp: time:        default: DEBUG:130+i
      //break; case __LINE__: TRICE(id(0), "TRACE:default+i:default+h\n" );     //exp: time:        default: TRACE:default+i:default+h
      //break; case __LINE__: TRICE(id(0), "TIME:blue+i:blue+h\n" );            //exp: time:        default: TIME:blue+i:blue+h
      //break; case __LINE__: TRICE(id(0), "MESSAGE:green+h:black\n" );         //exp: time:        default: MESSAGE:green+h:black
      //break; case __LINE__: TRICE(id(0), "READ:black+i:yellow+h\n" );         //exp: time:        default: READ:black+i:yellow+h
      //break; case __LINE__: TRICE(id(0), "WRITE:black+u:yellow+h\n" );        //exp: time:        default: WRITE:black+u:yellow+h
      //break; case __LINE__: TRICE(id(0), "RECEIVE:black+h:black\n" );         //exp: time:        default: RECEIVE:black+h:black
      //break; case __LINE__: TRICE(id(0), "TRANSMIT:black:black+h\n" );        //exp: time:        default: TRANSMIT:black:black+h
      //break; case __LINE__: TRICE(id(0), "DIAG:yellow+i:default+h\n" );       //exp: time:        default: DIAG:yellow+i:default+h
      //break; case __LINE__: TRICE(id(0), "INTERRUPT:magenta+i:default+h\n" ); //exp: time:        default: INTERRUPT:magenta+i:default+h
      //break; case __LINE__: TRICE(id(0), "SIGNAL:118+i\n" );                  //exp: time:        default: SIGNAL:118+i
      //break; case __LINE__: TRICE(id(0), "TEST:yellow+h:black\n" );           //exp: time:        default: TEST:yellow+h:black
      //break; case __LINE__: TRICE(id(0), "DEFAULT:off\n" );                   //exp: time:        default: DEFAULT:off
      //break; case __LINE__: TRICE(id(0), "NOTICE:blue:white+h\n" );           //exp: time:        default: NOTICE:blue:white+h
      //break; case __LINE__: TRICE(id(0), "ALERT:magenta:magenta+h\n" );       //exp: time:        default: ALERT:magenta:magenta+h
      //break; case __LINE__: TRICE(id(0), "ASSERT:yellow+i:blue\n" );          //exp: time:        default: ASSERT:yellow+i:blue
      //break; case __LINE__: TRICE(id(0), "ALARM:red+i:white+h\n" );           //exp: time:        default: ALARM:red+i:white+h
      //break; case __LINE__: TRICE(id(0), "VERBOSE:blue:default\n" );          //exp: time:        default: VERBOSE:blue:default
      //break; case __LINE__: TRICE(id(0), "fatal:magenta+b:red\n" );           //exp: time:        default: fatal:magenta+b:red
      //break; case __LINE__: TRICE(id(0), "critical:red+i:default+h\n" );      //exp: time:        default: critical:red+i:default+h
      //break; case __LINE__: TRICE(id(0), "emergency:red+i:blue\n" );          //exp: time:        default: emergency:red+i:blue
      //break; case __LINE__: TRICE(id(0), "error:11:red\n" );                  //exp: time:        default: error:11:red
      //break; case __LINE__: TRICE(id(0), "warning:11+i:red\n" );              //exp: time:        default: warning:11+i:red
      //break; case __LINE__: TRICE(id(0), "attention:11:green\n" );            //exp: time:        default: attention:11:green
      //break; case __LINE__: TRICE(id(0), "info:cyan+b:default+h\n" );         //exp: time:        default: info:cyan+b:default+h
      //break; case __LINE__: TRICE(id(0), "debug:130+i\n" );                   //exp: time:        default: debug:130+i
      //break; case __LINE__: TRICE(id(0), "trace:default+i:default+h\n" );     //exp: time:        default: trace:default+i:default+h
      //break; case __LINE__: TRICE(id(0), "time:blue+i:blue+h\n" );            //exp: time:        default: time:blue+i:blue+h
      //break; case __LINE__: TRICE(id(0), "message:green+h:black\n" );         //exp: time:        default: message:green+h:black
      //break; case __LINE__: TRICE(id(0), "read:black+i:yellow+h\n" );         //exp: time:        default: read:black+i:yellow+h
      //break; case __LINE__: TRICE(id(0), "write:black+u:yellow+h\n" );        //exp: time:        default: write:black+u:yellow+h
      //break; case __LINE__: TRICE(id(0), "receive:black+h:black\n" );         //exp: time:        default: receive:black+h:black
      //break; case __LINE__: TRICE(id(0), "transmit:black:black+h\n" );        //exp: time:        default: transmit:black:black+h
      //break; case __LINE__: TRICE(id(0), "diag:yellow+i:default+h\n" );       //exp: time:        default: diag:yellow+i:default+h
      //break; case __LINE__: TRICE(id(0), "interrupt:magenta+i:default+h\n" ); //exp: time:        default: interrupt:magenta+i:default+h
      //break; case __LINE__: TRICE(id(0), "signal:118+i\n" );                  //exp: time:        default: signal:118+i
      //break; case __LINE__: TRICE(id(0), "test:yellow+h:black\n" );           //exp: time:        default: test:yellow+h:black
      //break; case __LINE__: TRICE(id(0), "default:off\n" );                   //exp: time:        default: default:off
      //break; case __LINE__: TRICE(id(0), "notice:blue:white+h\n" );           //exp: time:        default: notice:blue:white+h
      //break; case __LINE__: TRICE(id(0), "alert:magenta:magenta+h\n" );       //exp: time:        default: alert:magenta:magenta+h
      //break; case __LINE__: TRICE(id(0), "assert:yellow+i:blue\n" );          //exp: time:        default: assert:yellow+i:blue
      //break; case __LINE__: TRICE(id(0), "alarm:red+i:white+h\n" );           //exp: time:        default: alarm:red+i:white+h
      //break; case __LINE__: TRICE(id(0), "verbose:blue:default\n" );          //exp: time:        default: verbose:blue:default
        
        break; case __LINE__: trice("FATAL:magenta+b:red\n" );           //exp: time:        default: FATAL:magenta+b:red
        break; case __LINE__: trice("CRITICAL:red+i:default+h\n" );      //exp: time:        default: CRITICAL:red+i:default+h
        break; case __LINE__: trice("EMERGENCY:red+i:blue\n" );          //exp: time:        default: EMERGENCY:red+i:blue
        break; case __LINE__: trice("ERROR:11:red\n" );                  //exp: time:        default: ERROR:11:red
        break; case __LINE__: trice("WARNING:11+i:red\n" );              //exp: time:        default: WARNING:11+i:red
        break; case __LINE__: trice("ATTENTION:11:green\n" );            //exp: time:        default: ATTENTION:11:green
        break; case __LINE__: trice("INFO:cyan+b:default+h\n" );         //exp: time:        default: INFO:cyan+b:default+h
        break; case __LINE__: trice("DEBUG:130+i\n" );                   //exp: time:        default: DEBUG:130+i
        break; case __LINE__: trice("TRACE:default+i:default+h\n" );     //exp: time:        default: TRACE:default+i:default+h
        break; case __LINE__: trice("TIME:blue+i:blue+h\n" );            //exp: time:        default: TIME:blue+i:blue+h
        break; case __LINE__: trice("MESSAGE:green+h:black\n" );         //exp: time:        default: MESSAGE:green+h:black
        break; case __LINE__: trice("READ:black+i:yellow+h\n" );         //exp: time:        default: READ:black+i:yellow+h
        break; case __LINE__: trice("WRITE:black+u:yellow+h\n" );        //exp: time:        default: WRITE:black+u:yellow+h
        break; case __LINE__: trice("RECEIVE:black+h:black\n" );         //exp: time:        default: RECEIVE:black+h:black
        break; case __LINE__: trice("TRANSMIT:black:black+h\n" );        //exp: time:        default: TRANSMIT:black:black+h
        break; case __LINE__: trice("DIAG:yellow+i:default+h\n" );       //exp: time:        default: DIAG:yellow+i:default+h
        break; case __LINE__: trice("INTERRUPT:magenta+i:default+h\n" ); //exp: time:        default: INTERRUPT:magenta+i:default+h
        break; case __LINE__: trice("SIGNAL:118+i\n" );                  //exp: time:        default: SIGNAL:118+i
        break; case __LINE__: trice("TEST:yellow+h:black\n" );           //exp: time:        default: TEST:yellow+h:black
        break; case __LINE__: trice("DEFAULT:off\n" );                   //exp: time:        default: DEFAULT:off
        break; case __LINE__: trice("NOTICE:blue:white+h\n" );           //exp: time:        default: NOTICE:blue:white+h
        break; case __LINE__: trice("ALERT:magenta:magenta+h\n" );       //exp: time:        default: ALERT:magenta:magenta+h
        break; case __LINE__: trice("ASSERT:yellow+i:blue\n" );          //exp: time:        default: ASSERT:yellow+i:blue
        break; case __LINE__: trice("ALARM:red+i:white+h\n" );           //exp: time:        default: ALARM:red+i:white+h
        break; case __LINE__: trice("VERBOSE:blue:default\n" );          //exp: time:        default: VERBOSE:blue:default
        break; case __LINE__: trice("fatal:magenta+b:red\n" );           //exp: time:        default: fatal:magenta+b:red
        break; case __LINE__: trice("critical:red+i:default+h\n" );      //exp: time:        default: critical:red+i:default+h
        break; case __LINE__: trice("emergency:red+i:blue\n" );          //exp: time:        default: emergency:red+i:blue
        break; case __LINE__: trice("error:11:red\n" );                  //exp: time:        default: error:11:red
        break; case __LINE__: trice("warning:11+i:red\n" );              //exp: time:        default: warning:11+i:red
        break; case __LINE__: trice("attention:11:green\n" );            //exp: time:        default: attention:11:green
        break; case __LINE__: trice("info:cyan+b:default+h\n" );         //exp: time:        default: info:cyan+b:default+h
        break; case __LINE__: trice("debug:130+i\n" );                   //exp: time:        default: debug:130+i
        break; case __LINE__: trice("trace:default+i:default+h\n" );     //exp: time:        default: trace:default+i:default+h
        break; case __LINE__: trice("time:blue+i:blue+h\n" );            //exp: time:        default: time:blue+i:blue+h
        break; case __LINE__: trice("message:green+h:black\n" );         //exp: time:        default: message:green+h:black
        break; case __LINE__: trice("read:black+i:yellow+h\n" );         //exp: time:        default: read:black+i:yellow+h
        break; case __LINE__: trice("write:black+u:yellow+h\n" );        //exp: time:        default: write:black+u:yellow+h
        break; case __LINE__: trice("receive:black+h:black\n" );         //exp: time:        default: receive:black+h:black
        break; case __LINE__: trice("transmit:black:black+h\n" );        //exp: time:        default: transmit:black:black+h
        break; case __LINE__: trice("diag:yellow+i:default+h\n" );       //exp: time:        default: diag:yellow+i:default+h
        break; case __LINE__: trice("interrupt:magenta+i:default+h\n" ); //exp: time:        default: interrupt:magenta+i:default+h
        break; case __LINE__: trice("signal:118+i\n" );                  //exp: time:        default: signal:118+i
        break; case __LINE__: trice("test:yellow+h:black\n" );           //exp: time:        default: test:yellow+h:black
        break; case __LINE__: trice("default:off\n" );                   //exp: time:        default: default:off
        break; case __LINE__: trice("notice:blue:white+h\n" );           //exp: time:        default: notice:blue:white+h
        break; case __LINE__: trice("alert:magenta:magenta+h\n" );       //exp: time:        default: alert:magenta:magenta+h
        break; case __LINE__: trice("assert:yellow+i:blue\n" );          //exp: time:        default: assert:yellow+i:blue
        break; case __LINE__: trice("alarm:red+i:white+h\n" );           //exp: time:        default: alarm:red+i:white+h
        break; case __LINE__: trice("verbose:blue:default\n" );          //exp: time:        default: verbose:blue:default

      //break; case __LINE__: TRICE(Id(0), "FATAL:magenta+b:red\n" );           //exp: time:    be16default: FATAL:magenta+b:red
      //break; case __LINE__: TRICE(Id(0), "CRITICAL:red+i:default+h\n" );      //exp: time:    be16default: CRITICAL:red+i:default+h
      //break; case __LINE__: TRICE(Id(0), "EMERGENCY:red+i:blue\n" );          //exp: time:    be16default: EMERGENCY:red+i:blue
      //break; case __LINE__: TRICE(Id(0), "ERROR:11:red\n" );                  //exp: time:    be16default: ERROR:11:red
      //break; case __LINE__: TRICE(Id(0), "WARNING:11+i:red\n" );              //exp: time:    be16default: WARNING:11+i:red
      //break; case __LINE__: TRICE(Id(0), "ATTENTION:11:green\n" );            //exp: time:    be16default: ATTENTION:11:green
      //break; case __LINE__: TRICE(Id(0), "INFO:cyan+b:default+h\n" );         //exp: time:    be16default: INFO:cyan+b:default+h
      //break; case __LINE__: TRICE(Id(0), "DEBUG:130+i\n" );                   //exp: time:    be16default: DEBUG:130+i
      //break; case __LINE__: TRICE(Id(0), "TRACE:default+i:default+h\n" );     //exp: time:    be16default: TRACE:default+i:default+h
      //break; case __LINE__: TRICE(Id(0), "TIME:blue+i:blue+h\n" );            //exp: time:    be16default: TIME:blue+i:blue+h
      //break; case __LINE__: TRICE(Id(0), "MESSAGE:green+h:black\n" );         //exp: time:    be16default: MESSAGE:green+h:black
      //break; case __LINE__: TRICE(Id(0), "READ:black+i:yellow+h\n" );         //exp: time:    be16default: READ:black+i:yellow+h
      //break; case __LINE__: TRICE(Id(0), "WRITE:black+u:yellow+h\n" );        //exp: time:    be16default: WRITE:black+u:yellow+h
      //break; case __LINE__: TRICE(Id(0), "RECEIVE:black+h:black\n" );         //exp: time:    be16default: RECEIVE:black+h:black
      //break; case __LINE__: TRICE(Id(0), "TRANSMIT:black:black+h\n" );        //exp: time:    be16default: TRANSMIT:black:black+h
      //break; case __LINE__: TRICE(Id(0), "DIAG:yellow+i:default+h\n" );       //exp: time:    be16default: DIAG:yellow+i:default+h
      //break; case __LINE__: TRICE(Id(0), "INTERRUPT:magenta+i:default+h\n" ); //exp: time:    be16default: INTERRUPT:magenta+i:default+h
      //break; case __LINE__: TRICE(Id(0), "SIGNAL:118+i\n" );                  //exp: time:    be16default: SIGNAL:118+i
      //break; case __LINE__: TRICE(Id(0), "TEST:yellow+h:black\n" );           //exp: time:    be16default: TEST:yellow+h:black
      //break; case __LINE__: TRICE(Id(0), "DEFAULT:off\n" );                   //exp: time:    be16default: DEFAULT:off
      //break; case __LINE__: TRICE(Id(0), "NOTICE:blue:white+h\n" );           //exp: time:    be16default: NOTICE:blue:white+h
      //break; case __LINE__: TRICE(Id(0), "ALERT:magenta:magenta+h\n" );       //exp: time:    be16default: ALERT:magenta:magenta+h
      //break; case __LINE__: TRICE(Id(0), "ASSERT:yellow+i:blue\n" );          //exp: time:    be16default: ASSERT:yellow+i:blue
      //break; case __LINE__: TRICE(Id(0), "ALARM:red+i:white+h\n" );           //exp: time:    be16default: ALARM:red+i:white+h
      //break; case __LINE__: TRICE(Id(0), "VERBOSE:blue:default\n" );          //exp: time:    be16default: VERBOSE:blue:default
      //break; case __LINE__: TRICE(Id(0), "fatal:magenta+b:red\n" );           //exp: time:    be16default: fatal:magenta+b:red
      //break; case __LINE__: TRICE(Id(0), "critical:red+i:default+h\n" );      //exp: time:    be16default: critical:red+i:default+h
      //break; case __LINE__: TRICE(Id(0), "emergency:red+i:blue\n" );          //exp: time:    be16default: emergency:red+i:blue
      //break; case __LINE__: TRICE(Id(0), "error:11:red\n" );                  //exp: time:    be16default: error:11:red
      //break; case __LINE__: TRICE(Id(0), "warning:11+i:red\n" );              //exp: time:    be16default: warning:11+i:red
      //break; case __LINE__: TRICE(Id(0), "attention:11:green\n" );            //exp: time:    be16default: attention:11:green
      //break; case __LINE__: TRICE(Id(0), "info:cyan+b:default+h\n" );         //exp: time:    be16default: info:cyan+b:default+h
      //break; case __LINE__: TRICE(Id(0), "debug:130+i\n" );                   //exp: time:    be16default: debug:130+i
      //break; case __LINE__: TRICE(Id(0), "trace:default+i:default+h\n" );     //exp: time:    be16default: trace:default+i:default+h
      //break; case __LINE__: TRICE(Id(0), "time:blue+i:blue+h\n" );            //exp: time:    be16default: time:blue+i:blue+h
      //break; case __LINE__: TRICE(Id(0), "message:green+h:black\n" );         //exp: time:    be16default: message:green+h:black
      //break; case __LINE__: TRICE(Id(0), "read:black+i:yellow+h\n" );         //exp: time:    be16default: read:black+i:yellow+h
      //break; case __LINE__: TRICE(Id(0), "write:black+u:yellow+h\n" );        //exp: time:    be16default: write:black+u:yellow+h
      //break; case __LINE__: TRICE(Id(0), "receive:black+h:black\n" );         //exp: time:    be16default: receive:black+h:black
      //break; case __LINE__: TRICE(Id(0), "transmit:black:black+h\n" );        //exp: time:    be16default: transmit:black:black+h
      //break; case __LINE__: TRICE(Id(0), "diag:yellow+i:default+h\n" );       //exp: time:    be16default: diag:yellow+i:default+h
      //break; case __LINE__: TRICE(Id(0), "interrupt:magenta+i:default+h\n" ); //exp: time:    be16default: interrupt:magenta+i:default+h
      //break; case __LINE__: TRICE(Id(0), "signal:118+i\n" );                  //exp: time:    be16default: signal:118+i
      //break; case __LINE__: TRICE(Id(0), "test:yellow+h:black\n" );           //exp: time:    be16default: test:yellow+h:black
      //break; case __LINE__: TRICE(Id(0), "default:off\n" );                   //exp: time:    be16default: default:off
      //break; case __LINE__: TRICE(Id(0), "notice:blue:white+h\n" );           //exp: time:    be16default: notice:blue:white+h
      //break; case __LINE__: TRICE(Id(0), "alert:magenta:magenta+h\n" );       //exp: time:    be16default: alert:magenta:magenta+h
      //break; case __LINE__: TRICE(Id(0), "assert:yellow+i:blue\n" );          //exp: time:    be16default: assert:yellow+i:blue
      //break; case __LINE__: TRICE(Id(0), "alarm:red+i:white+h\n" );           //exp: time:    be16default: alarm:red+i:white+h
      //break; case __LINE__: TRICE(Id(0), "verbose:blue:default\n" );          //exp: time:    be16default: verbose:blue:default

      //break; case __LINE__: Trice("FATAL:magenta+b:red\n" );           //exp: time:    be16default: FATAL:magenta+b:red
      //break; case __LINE__: Trice("CRITICAL:red+i:default+h\n" );      //exp: time:    be16default: CRITICAL:red+i:default+h
      //break; case __LINE__: Trice("EMERGENCY:red+i:blue\n" );          //exp: time:    be16default: EMERGENCY:red+i:blue
      //break; case __LINE__: Trice("ERROR:11:red\n" );                  //exp: time:    be16default: ERROR:11:red
      //break; case __LINE__: Trice("WARNING:11+i:red\n" );              //exp: time:    be16default: WARNING:11+i:red
      //break; case __LINE__: Trice("ATTENTION:11:green\n" );            //exp: time:    be16default: ATTENTION:11:green
      //break; case __LINE__: Trice("INFO:cyan+b:default+h\n" );         //exp: time:    be16default: INFO:cyan+b:default+h
      //break; case __LINE__: Trice("DEBUG:130+i\n" );                   //exp: time:    be16default: DEBUG:130+i
      //break; case __LINE__: Trice("TRACE:default+i:default+h\n" );     //exp: time:    be16default: TRACE:default+i:default+h
      //break; case __LINE__: Trice("TIME:blue+i:blue+h\n" );            //exp: time:    be16default: TIME:blue+i:blue+h
      //break; case __LINE__: Trice("MESSAGE:green+h:black\n" );         //exp: time:    be16default: MESSAGE:green+h:black
      //break; case __LINE__: Trice("READ:black+i:yellow+h\n" );         //exp: time:    be16default: READ:black+i:yellow+h
      //break; case __LINE__: Trice("WRITE:black+u:yellow+h\n" );        //exp: time:    be16default: WRITE:black+u:yellow+h
      //break; case __LINE__: Trice("RECEIVE:black+h:black\n" );         //exp: time:    be16default: RECEIVE:black+h:black
      //break; case __LINE__: Trice("TRANSMIT:black:black+h\n" );        //exp: time:    be16default: TRANSMIT:black:black+h
      //break; case __LINE__: Trice("DIAG:yellow+i:default+h\n" );       //exp: time:    be16default: DIAG:yellow+i:default+h
      //break; case __LINE__: Trice("INTERRUPT:magenta+i:default+h\n" ); //exp: time:    be16default: INTERRUPT:magenta+i:default+h
      //break; case __LINE__: Trice("SIGNAL:118+i\n" );                  //exp: time:    be16default: SIGNAL:118+i
      //break; case __LINE__: Trice("TEST:yellow+h:black\n" );           //exp: time:    be16default: TEST:yellow+h:black
      //break; case __LINE__: Trice("DEFAULT:off\n" );                   //exp: time:    be16default: DEFAULT:off
      //break; case __LINE__: Trice("NOTICE:blue:white+h\n" );           //exp: time:    be16default: NOTICE:blue:white+h
      //break; case __LINE__: Trice("ALERT:magenta:magenta+h\n" );       //exp: time:    be16default: ALERT:magenta:magenta+h
      //break; case __LINE__: Trice("ASSERT:yellow+i:blue\n" );          //exp: time:    be16default: ASSERT:yellow+i:blue
      //break; case __LINE__: Trice("ALARM:red+i:white+h\n" );           //exp: time:    be16default: ALARM:red+i:white+h
      //break; case __LINE__: Trice("VERBOSE:blue:default\n" );          //exp: time:    be16default: VERBOSE:blue:default
      //break; case __LINE__: Trice("fatal:magenta+b:red\n" );           //exp: time:    be16default: fatal:magenta+b:red
      //break; case __LINE__: Trice("critical:red+i:default+h\n" );      //exp: time:    be16default: critical:red+i:default+h
      //break; case __LINE__: Trice("emergency:red+i:blue\n" );          //exp: time:    be16default: emergency:red+i:blue
      //break; case __LINE__: Trice("error:11:red\n" );                  //exp: time:    be16default: error:11:red
      //break; case __LINE__: Trice("warning:11+i:red\n" );              //exp: time:    be16default: warning:11+i:red
      //break; case __LINE__: Trice("attention:11:green\n" );            //exp: time:    be16default: attention:11:green
      //break; case __LINE__: Trice("info:cyan+b:default+h\n" );         //exp: time:    be16default: info:cyan+b:default+h
      //break; case __LINE__: Trice("debug:130+i\n" );                   //exp: time:    be16default: debug:130+i
      //break; case __LINE__: Trice("trace:default+i:default+h\n" );     //exp: time:    be16default: trace:default+i:default+h
      //break; case __LINE__: Trice("time:blue+i:blue+h\n" );            //exp: time:    be16default: time:blue+i:blue+h
      //break; case __LINE__: Trice("message:green+h:black\n" );         //exp: time:    be16default: message:green+h:black
      //break; case __LINE__: Trice("read:black+i:yellow+h\n" );         //exp: time:    be16default: read:black+i:yellow+h
      //break; case __LINE__: Trice("write:black+u:yellow+h\n" );        //exp: time:    be16default: write:black+u:yellow+h
      //break; case __LINE__: Trice("receive:black+h:black\n" );         //exp: time:    be16default: receive:black+h:black
      //break; case __LINE__: Trice("transmit:black:black+h\n" );        //exp: time:    be16default: transmit:black:black+h
      //break; case __LINE__: Trice("diag:yellow+i:default+h\n" );       //exp: time:    be16default: diag:yellow+i:default+h
      //break; case __LINE__: Trice("interrupt:magenta+i:default+h\n" ); //exp: time:    be16default: interrupt:magenta+i:default+h
      //break; case __LINE__: Trice("signal:118+i\n" );                  //exp: time:    be16default: signal:118+i
      //break; case __LINE__: Trice("test:yellow+h:black\n" );           //exp: time:    be16default: test:yellow+h:black
      //break; case __LINE__: Trice("default:off\n" );                   //exp: time:    be16default: default:off
      //break; case __LINE__: Trice("notice:blue:white+h\n" );           //exp: time:    be16default: notice:blue:white+h
      //break; case __LINE__: Trice("alert:magenta:magenta+h\n" );       //exp: time:    be16default: alert:magenta:magenta+h
      //break; case __LINE__: Trice("assert:yellow+i:blue\n" );          //exp: time:    be16default: assert:yellow+i:blue
      //break; case __LINE__: Trice("alarm:red+i:white+h\n" );           //exp: time:    be16default: alarm:red+i:white+h
      //break; case __LINE__: Trice("verbose:blue:default\n" );          //exp: time:    be16default: verbose:blue:default

      //break; case __LINE__: TRICE(ID(0), "FATAL:magenta+b:red\n" );           //exp: time:feed3322default: FATAL:magenta+b:red
      //break; case __LINE__: TRICE(ID(0), "CRITICAL:red+i:default+h\n" );      //exp: time:feed3322default: CRITICAL:red+i:default+h
      //break; case __LINE__: TRICE(ID(0), "EMERGENCY:red+i:blue\n" );          //exp: time:feed3322default: EMERGENCY:red+i:blue
      //break; case __LINE__: TRICE(ID(0), "ERROR:11:red\n" );                  //exp: time:feed3322default: ERROR:11:red
      //break; case __LINE__: TRICE(ID(0), "WARNING:11+i:red\n" );              //exp: time:feed3322default: WARNING:11+i:red
      //break; case __LINE__: TRICE(ID(0), "ATTENTION:11:green\n" );            //exp: time:feed3322default: ATTENTION:11:green
      //break; case __LINE__: TRICE(ID(0), "INFO:cyan+b:default+h\n" );         //exp: time:feed3322default: INFO:cyan+b:default+h
      //break; case __LINE__: TRICE(ID(0), "DEBUG:130+i\n" );                   //exp: time:feed3322default: DEBUG:130+i
      //break; case __LINE__: TRICE(ID(0), "TRACE:default+i:default+h\n" );     //exp: time:feed3322default: TRACE:default+i:default+h
      //break; case __LINE__: TRICE(ID(0), "TIME:blue+i:blue+h\n" );            //exp: time:feed3322default: TIME:blue+i:blue+h
      //break; case __LINE__: TRICE(ID(0), "MESSAGE:green+h:black\n" );         //exp: time:feed3322default: MESSAGE:green+h:black
      //break; case __LINE__: TRICE(ID(0), "READ:black+i:yellow+h\n" );         //exp: time:feed3322default: READ:black+i:yellow+h
      //break; case __LINE__: TRICE(ID(0), "WRITE:black+u:yellow+h\n" );        //exp: time:feed3322default: WRITE:black+u:yellow+h
      //break; case __LINE__: TRICE(ID(0), "RECEIVE:black+h:black\n" );         //exp: time:feed3322default: RECEIVE:black+h:black
      //break; case __LINE__: TRICE(ID(0), "TRANSMIT:black:black+h\n" );        //exp: time:feed3322default: TRANSMIT:black:black+h
      //break; case __LINE__: TRICE(ID(0), "DIAG:yellow+i:default+h\n" );       //exp: time:feed3322default: DIAG:yellow+i:default+h
      //break; case __LINE__: TRICE(ID(0), "INTERRUPT:magenta+i:default+h\n" ); //exp: time:feed3322default: INTERRUPT:magenta+i:default+h
      //break; case __LINE__: TRICE(ID(0), "SIGNAL:118+i\n" );                  //exp: time:feed3322default: SIGNAL:118+i
      //break; case __LINE__: TRICE(ID(0), "TEST:yellow+h:black\n" );           //exp: time:feed3322default: TEST:yellow+h:black
      //break; case __LINE__: TRICE(ID(0), "DEFAULT:off\n" );                   //exp: time:feed3322default: DEFAULT:off
      //break; case __LINE__: TRICE(ID(0), "NOTICE:blue:white+h\n" );           //exp: time:feed3322default: NOTICE:blue:white+h
      //break; case __LINE__: TRICE(ID(0), "ALERT:magenta:magenta+h\n" );       //exp: time:feed3322default: ALERT:magenta:magenta+h
      //break; case __LINE__: TRICE(ID(0), "ASSERT:yellow+i:blue\n" );          //exp: time:feed3322default: ASSERT:yellow+i:blue
      //break; case __LINE__: TRICE(ID(0), "ALARM:red+i:white+h\n" );           //exp: time:feed3322default: ALARM:red+i:white+h
      //break; case __LINE__: TRICE(ID(0), "VERBOSE:blue:default\n" );          //exp: time:feed3322default: VERBOSE:blue:default
      //break; case __LINE__: TRICE(ID(0), "fatal:magenta+b:red\n" );           //exp: time:feed3322default: fatal:magenta+b:red
      //break; case __LINE__: TRICE(ID(0), "critical:red+i:default+h\n" );      //exp: time:feed3322default: critical:red+i:default+h
      //break; case __LINE__: TRICE(ID(0), "emergency:red+i:blue\n" );          //exp: time:feed3322default: emergency:red+i:blue
      //break; case __LINE__: TRICE(ID(0), "error:11:red\n" );                  //exp: time:feed3322default: error:11:red
      //break; case __LINE__: TRICE(ID(0), "warning:11+i:red\n" );              //exp: time:feed3322default: warning:11+i:red
      //break; case __LINE__: TRICE(ID(0), "attention:11:green\n" );            //exp: time:feed3322default: attention:11:green
      //break; case __LINE__: TRICE(ID(0), "info:cyan+b:default+h\n" );         //exp: time:feed3322default: info:cyan+b:default+h
      //break; case __LINE__: TRICE(ID(0), "debug:130+i\n" );                   //exp: time:feed3322default: debug:130+i
      //break; case __LINE__: TRICE(ID(0), "trace:default+i:default+h\n" );     //exp: time:feed3322default: trace:default+i:default+h
      //break; case __LINE__: TRICE(ID(0), "time:blue+i:blue+h\n" );            //exp: time:feed3322default: time:blue+i:blue+h
      //break; case __LINE__: TRICE(ID(0), "message:green+h:black\n" );         //exp: time:feed3322default: message:green+h:black
      //break; case __LINE__: TRICE(ID(0), "read:black+i:yellow+h\n" );         //exp: time:feed3322default: read:black+i:yellow+h
      //break; case __LINE__: TRICE(ID(0), "write:black+u:yellow+h\n" );        //exp: time:feed3322default: write:black+u:yellow+h
      //break; case __LINE__: TRICE(ID(0), "receive:black+h:black\n" );         //exp: time:feed3322default: receive:black+h:black
      //break; case __LINE__: TRICE(ID(0), "transmit:black:black+h\n" );        //exp: time:feed3322default: transmit:black:black+h
      //break; case __LINE__: TRICE(ID(0), "diag:yellow+i:default+h\n" );       //exp: time:feed3322default: diag:yellow+i:default+h
      //break; case __LINE__: TRICE(ID(0), "interrupt:magenta+i:default+h\n" ); //exp: time:feed3322default: interrupt:magenta+i:default+h
      //break; case __LINE__: TRICE(ID(0), "signal:118+i\n" );                  //exp: time:feed3322default: signal:118+i
      //break; case __LINE__: TRICE(ID(0), "test:yellow+h:black\n" );           //exp: time:feed3322default: test:yellow+h:black
      //break; case __LINE__: TRICE(ID(0), "default:off\n" );                   //exp: time:feed3322default: default:off
      //break; case __LINE__: TRICE(ID(0), "notice:blue:white+h\n" );           //exp: time:feed3322default: notice:blue:white+h
      //break; case __LINE__: TRICE(ID(0), "alert:magenta:magenta+h\n" );       //exp: time:feed3322default: alert:magenta:magenta+h
      //break; case __LINE__: TRICE(ID(0), "assert:yellow+i:blue\n" );          //exp: time:feed3322default: assert:yellow+i:blue
      //break; case __LINE__: TRICE(ID(0), "alarm:red+i:white+h\n" );           //exp: time:feed3322default: alarm:red+i:white+h
      //break; case __LINE__: TRICE(ID(0), "verbose:blue:default\n" );          //exp: time:feed3322default: verbose:blue:default
        
      //break; case __LINE__: TRice("FATAL:magenta+b:red\n" );           //exp: time:feed3322default: FATAL:magenta+b:red
      //break; case __LINE__: TRice("CRITICAL:red+i:default+h\n" );      //exp: time:feed3322default: CRITICAL:red+i:default+h
      //break; case __LINE__: TRice("EMERGENCY:red+i:blue\n" );          //exp: time:feed3322default: EMERGENCY:red+i:blue
      //break; case __LINE__: TRice("ERROR:11:red\n" );                  //exp: time:feed3322default: ERROR:11:red
      //break; case __LINE__: TRice("WARNING:11+i:red\n" );              //exp: time:feed3322default: WARNING:11+i:red
      //break; case __LINE__: TRice("ATTENTION:11:green\n" );            //exp: time:feed3322default: ATTENTION:11:green
      //break; case __LINE__: TRice("INFO:cyan+b:default+h\n" );         //exp: time:feed3322default: INFO:cyan+b:default+h
      //break; case __LINE__: TRice("DEBUG:130+i\n" );                   //exp: time:feed3322default: DEBUG:130+i
      //break; case __LINE__: TRice("TRACE:default+i:default+h\n" );     //exp: time:feed3322default: TRACE:default+i:default+h
      //break; case __LINE__: TRice("TIME:blue+i:blue+h\n" );            //exp: time:feed3322default: TIME:blue+i:blue+h
      //break; case __LINE__: TRice("MESSAGE:green+h:black\n" );         //exp: time:feed3322default: MESSAGE:green+h:black
      //break; case __LINE__: TRice("READ:black+i:yellow+h\n" );         //exp: time:feed3322default: READ:black+i:yellow+h
      //break; case __LINE__: TRice("WRITE:black+u:yellow+h\n" );        //exp: time:feed3322default: WRITE:black+u:yellow+h
      //break; case __LINE__: TRice("RECEIVE:black+h:black\n" );         //exp: time:feed3322default: RECEIVE:black+h:black
      //break; case __LINE__: TRice("TRANSMIT:black:black+h\n" );        //exp: time:feed3322default: TRANSMIT:black:black+h
      //break; case __LINE__: TRice("DIAG:yellow+i:default+h\n" );       //exp: time:feed3322default: DIAG:yellow+i:default+h
      //break; case __LINE__: TRice("INTERRUPT:magenta+i:default+h\n" ); //exp: time:feed3322default: INTERRUPT:magenta+i:default+h
      //break; case __LINE__: TRice("SIGNAL:118+i\n" );                  //exp: time:feed3322default: SIGNAL:118+i
      //break; case __LINE__: TRice("TEST:yellow+h:black\n" );           //exp: time:feed3322default: TEST:yellow+h:black
      //break; case __LINE__: TRice("DEFAULT:off\n" );                   //exp: time:feed3322default: DEFAULT:off
      //break; case __LINE__: TRice("NOTICE:blue:white+h\n" );           //exp: time:feed3322default: NOTICE:blue:white+h
      //break; case __LINE__: TRice("ALERT:magenta:magenta+h\n" );       //exp: time:feed3322default: ALERT:magenta:magenta+h
      //break; case __LINE__: TRice("ASSERT:yellow+i:blue\n" );          //exp: time:feed3322default: ASSERT:yellow+i:blue
      //break; case __LINE__: TRice("ALARM:red+i:white+h\n" );           //exp: time:feed3322default: ALARM:red+i:white+h
      //break; case __LINE__: TRice("VERBOSE:blue:default\n" );          //exp: time:feed3322default: VERBOSE:blue:default
      //break; case __LINE__: TRice("fatal:magenta+b:red\n" );           //exp: time:feed3322default: fatal:magenta+b:red
      //break; case __LINE__: TRice("critical:red+i:default+h\n" );      //exp: time:feed3322default: critical:red+i:default+h
      //break; case __LINE__: TRice("emergency:red+i:blue\n" );          //exp: time:feed3322default: emergency:red+i:blue
      //break; case __LINE__: TRice("error:11:red\n" );                  //exp: time:feed3322default: error:11:red
      //break; case __LINE__: TRice("warning:11+i:red\n" );              //exp: time:feed3322default: warning:11+i:red
      //break; case __LINE__: TRice("attention:11:green\n" );            //exp: time:feed3322default: attention:11:green
      //break; case __LINE__: TRice("info:cyan+b:default+h\n" );         //exp: time:feed3322default: info:cyan+b:default+h
      //break; case __LINE__: TRice("debug:130+i\n" );                   //exp: time:feed3322default: debug:130+i
      //break; case __LINE__: TRice("trace:default+i:default+h\n" );     //exp: time:feed3322default: trace:default+i:default+h
      //break; case __LINE__: TRice("time:blue+i:blue+h\n" );            //exp: time:feed3322default: time:blue+i:blue+h
      //break; case __LINE__: TRice("message:green+h:black\n" );         //exp: time:feed3322default: message:green+h:black
      //break; case __LINE__: TRice("read:black+i:yellow+h\n" );         //exp: time:feed3322default: read:black+i:yellow+h
      //break; case __LINE__: TRice("write:black+u:yellow+h\n" );        //exp: time:feed3322default: write:black+u:yellow+h
      //break; case __LINE__: TRice("receive:black+h:black\n" );         //exp: time:feed3322default: receive:black+h:black
      //break; case __LINE__: TRice("transmit:black:black+h\n" );        //exp: time:feed3322default: transmit:black:black+h
      //break; case __LINE__: TRice("diag:yellow+i:default+h\n" );       //exp: time:feed3322default: diag:yellow+i:default+h
      //break; case __LINE__: TRice("interrupt:magenta+i:default+h\n" ); //exp: time:feed3322default: interrupt:magenta+i:default+h
      //break; case __LINE__: TRice("signal:118+i\n" );                  //exp: time:feed3322default: signal:118+i
      //break; case __LINE__: TRice("test:yellow+h:black\n" );           //exp: time:feed3322default: test:yellow+h:black
      //break; case __LINE__: TRice("default:off\n" );                   //exp: time:feed3322default: default:off
      //break; case __LINE__: TRice("notice:blue:white+h\n" );           //exp: time:feed3322default: notice:blue:white+h
      //break; case __LINE__: TRice("alert:magenta:magenta+h\n" );       //exp: time:feed3322default: alert:magenta:magenta+h
      //break; case __LINE__: TRice("assert:yellow+i:blue\n" );          //exp: time:feed3322default: assert:yellow+i:blue
      //break; case __LINE__: TRice("alarm:red+i:white+h\n" );           //exp: time:feed3322default: alarm:red+i:white+h
      //break; case __LINE__: TRice("verbose:blue:default\n" );          //exp: time:feed3322default: verbose:blue:default

   
   
        break; case __LINE__: TRICE64(id(0), "msg:%d (%%d)\n", -1 );            //exp: time:        default: msg:-1 (%d)

        break; case __LINE__: trice("sig:TRICE8 with 1 to 12 values\n" );

        break; case __LINE__: trice("rd:TRICE %d\n", -1 );                                                                                            //exp: time:        default: rd:TRICE -1     
        break; case __LINE__: trice("rd:TRICE %d, %d\n", -1, -2 );                                                                                    //exp: time:        default: rd:TRICE -1, -2 
        break; case __LINE__: trice("rd:TRICE %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: rd:TRICE -1, -2, -3       
        break; case __LINE__: trice("rd:TRICE %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:        default: rd:TRICE -1, -2, -3, -4      
        break; case __LINE__: trice("rd:TRICE %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5        
        break; case __LINE__: trice("rd:TRICE %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice("rd:TRICE %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice("rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice("rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice("rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice("rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice("rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: trice("rd:TRICE %d\n", -1 );                                                                                            //exp: time:        default: rd:TRICE -1     
        break; case __LINE__: trice("rd:TRICE %d, %d\n", -1, -2 );                                                                                    //exp: time:        default: rd:TRICE -1, -2 
        break; case __LINE__: trice("rd:TRICE %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: rd:TRICE -1, -2, -3       
        break; case __LINE__: trice("rd:TRICE %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:        default: rd:TRICE -1, -2, -3, -4      
        break; case __LINE__: trice("rd:TRICE %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5        
        break; case __LINE__: trice("rd:TRICE %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice("rd:TRICE %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice("rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice("rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice("rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice("rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice("rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: trice("rd:TRICE %d\n", -1 );                                                                                            //exp: time:        default: rd:TRICE -1     
        break; case __LINE__: trice("rd:TRICE %d, %d\n", -1, -2 );                                                                                    //exp: time:        default: rd:TRICE -1, -2 
        break; case __LINE__: trice("rd:TRICE %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: rd:TRICE -1, -2, -3       
        break; case __LINE__: trice("rd:TRICE %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:        default: rd:TRICE -1, -2, -3, -4      
        break; case __LINE__: trice("rd:TRICE %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5        
        break; case __LINE__: trice("rd:TRICE %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice("rd:TRICE %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice("rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice("rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice("rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice("rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice("rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

      //break; case __LINE__: trice("sig:TRICE_n with 1 to 12 values\n" );

      //break; case __LINE__: TRICE_1 (id(0), "rd:TRICE_1  %d\n", -1 );                                                                                            //exp: time:        default: rd:TRICE_1  -1     
      //break; case __LINE__: TRICE_2 (id(0), "rd:TRICE_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:        default: rd:TRICE_2  -1, -2 
      //break; case __LINE__: TRICE_3 (id(0), "rd:TRICE_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: rd:TRICE_3  -1, -2, -3       
      //break; case __LINE__: TRICE_4 (id(0), "rd:TRICE_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:        default: rd:TRICE_4  -1, -2, -3, -4      
      //break; case __LINE__: TRICE_5 (id(0), "rd:TRICE_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:        default: rd:TRICE_5  -1, -2, -3, -4, -5        
      //break; case __LINE__: TRICE_6 (id(0), "rd:TRICE_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: rd:TRICE_6  -1, -2, -3, -4, -5, -6        
      //break; case __LINE__: TRICE_7 (id(0), "rd:TRICE_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:        default: rd:TRICE_7  -1, -2, -3, -4, -5, -6, -7      
      //break; case __LINE__: TRICE_8 (id(0), "rd:TRICE_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:        default: rd:TRICE_8  -1, -2, -3, -4, -5, -6, -7, -8       
      //break; case __LINE__: TRICE_9 (id(0), "rd:TRICE_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: rd:TRICE_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
      //break; case __LINE__: TRICE_10(id(0), "rd:TRICE_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:        default: rd:TRICE_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
      //break; case __LINE__: TRICE_11(id(0), "rd:TRICE_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:        default: rd:TRICE_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
      //break; case __LINE__: TRICE_12(id(0), "rd:TRICE_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: rd:TRICE_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
      //break; case __LINE__: TRICE_1 (Id(0), "rd:TRICE_1  %d\n", -1 );                                                                                            //exp: time:    be16default: rd:TRICE_1  -1     
      //break; case __LINE__: TRICE_2 (Id(0), "rd:TRICE_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:TRICE_2  -1, -2 
      //break; case __LINE__: TRICE_3 (Id(0), "rd:TRICE_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:TRICE_3  -1, -2, -3       
      //break; case __LINE__: TRICE_4 (Id(0), "rd:TRICE_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:TRICE_4  -1, -2, -3, -4      
      //break; case __LINE__: TRICE_5 (Id(0), "rd:TRICE_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:TRICE_5  -1, -2, -3, -4, -5        
      //break; case __LINE__: TRICE_6 (Id(0), "rd:TRICE_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:TRICE_6  -1, -2, -3, -4, -5, -6        
      //break; case __LINE__: TRICE_7 (Id(0), "rd:TRICE_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:TRICE_7  -1, -2, -3, -4, -5, -6, -7      
      //break; case __LINE__: TRICE_8 (Id(0), "rd:TRICE_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:TRICE_8  -1, -2, -3, -4, -5, -6, -7, -8       
      //break; case __LINE__: TRICE_9 (Id(0), "rd:TRICE_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:TRICE_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
      //break; case __LINE__: TRICE_10(Id(0), "rd:TRICE_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:TRICE_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
      //break; case __LINE__: TRICE_11(Id(0), "rd:TRICE_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:TRICE_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
      //break; case __LINE__: TRICE_12(Id(0), "rd:TRICE_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:TRICE_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
      //break; case __LINE__: TRICE_1 (ID(0), "rd:TRICE_1  %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRICE_1  -1     
      //break; case __LINE__: TRICE_2 (ID(0), "rd:TRICE_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRICE_2  -1, -2 
      //break; case __LINE__: TRICE_3 (ID(0), "rd:TRICE_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRICE_3  -1, -2, -3       
      //break; case __LINE__: TRICE_4 (ID(0), "rd:TRICE_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRICE_4  -1, -2, -3, -4      
      //break; case __LINE__: TRICE_5 (ID(0), "rd:TRICE_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRICE_5  -1, -2, -3, -4, -5        
      //break; case __LINE__: TRICE_6 (ID(0), "rd:TRICE_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRICE_6  -1, -2, -3, -4, -5, -6        
      //break; case __LINE__: TRICE_7 (ID(0), "rd:TRICE_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRICE_7  -1, -2, -3, -4, -5, -6, -7      
      //break; case __LINE__: TRICE_8 (ID(0), "rd:TRICE_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRICE_8  -1, -2, -3, -4, -5, -6, -7, -8       
      //break; case __LINE__: TRICE_9 (ID(0), "rd:TRICE_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRICE_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
      //break; case __LINE__: TRICE_10(ID(0), "rd:TRICE_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRICE_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
      //break; case __LINE__: TRICE_11(ID(0), "rd:TRICE_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRICE_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
      //break; case __LINE__: TRICE_12(ID(0), "rd:TRICE_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRICE_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: TRICE(ID(0), "sig:trice_n with 1 to 12 values\n" );

        break; case __LINE__: trice_1 ("rd:trice_1  %d\n", -1 );                                                                                            //exp: time:        default: rd:trice_1  -1     
        break; case __LINE__: trice_2 ("rd:trice_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:        default: rd:trice_2  -1, -2 
        break; case __LINE__: trice_3 ("rd:trice_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: rd:trice_3  -1, -2, -3       
        break; case __LINE__: trice_4 ("rd:trice_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:        default: rd:trice_4  -1, -2, -3, -4      
        break; case __LINE__: trice_5 ("rd:trice_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:        default: rd:trice_5  -1, -2, -3, -4, -5        
        break; case __LINE__: trice_6 ("rd:trice_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: rd:trice_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice_7 ("rd:trice_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:        default: rd:trice_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice_8 ("rd:trice_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:        default: rd:trice_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice_9 ("rd:trice_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: rd:trice_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice_10("rd:trice_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:        default: rd:trice_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice_11("rd:trice_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:        default: rd:trice_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice_12("rd:trice_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: rd:trice_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: Trice_1 ("rd:Trice_1  %d\n", -1 );                                                                                            //exp: time:    be16default: rd:Trice_1  -1     
        break; case __LINE__: Trice_2 ("rd:Trice_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:Trice_2  -1, -2 
        break; case __LINE__: Trice_3 ("rd:Trice_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:Trice_3  -1, -2, -3       
        break; case __LINE__: Trice_4 ("rd:Trice_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:Trice_4  -1, -2, -3, -4      
        break; case __LINE__: Trice_5 ("rd:Trice_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:Trice_5  -1, -2, -3, -4, -5        
        break; case __LINE__: Trice_6 ("rd:Trice_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:Trice_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: Trice_7 ("rd:Trice_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:Trice_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: Trice_8 ("rd:Trice_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:Trice_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: Trice_9 ("rd:Trice_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:Trice_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: Trice_10("rd:Trice_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:Trice_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: Trice_11("rd:Trice_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:Trice_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: Trice_12("rd:Trice_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:Trice_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRice_1 ("rd:TRice_1  %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRice_1  -1     
        break; case __LINE__: TRice_2 ("rd:TRice_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRice_2  -1, -2 
        break; case __LINE__: TRice_3 ("rd:TRice_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRice_3  -1, -2, -3       
        break; case __LINE__: TRice_4 ("rd:TRice_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRice_4  -1, -2, -3, -4      
        break; case __LINE__: TRice_5 ("rd:TRice_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRice_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRice_6 ("rd:TRice_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRice_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRice_7 ("rd:TRice_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRice_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRice_8 ("rd:TRice_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRice_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRice_9 ("rd:TRice_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRice_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRice_10("rd:TRice_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRice_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRice_11("rd:TRice_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRice_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRice_12("rd:TRice_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRice_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

      //break; case __LINE__: trice("sig:TRICE8 with 1 to 12 values\n" );

      //break; case __LINE__: TRICE8(id(0), "rd:TRICE8 %d\n", -1 );                                                                                            //exp: time:        default: rd:TRICE8 -1     
      //break; case __LINE__: TRICE8(id(0), "rd:TRICE8 %d, %d\n", -1, -2 );                                                                                    //exp: time:        default: rd:TRICE8 -1, -2 
      //break; case __LINE__: TRICE8(id(0), "rd:TRICE8 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: rd:TRICE8 -1, -2, -3       
      //break; case __LINE__: TRICE8(id(0), "rd:TRICE8 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:        default: rd:TRICE8 -1, -2, -3, -4      
      //break; case __LINE__: TRICE8(id(0), "rd:TRICE8 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:        default: rd:TRICE8 -1, -2, -3, -4, -5        
      //break; case __LINE__: TRICE8(id(0), "rd:TRICE8 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: rd:TRICE8 -1, -2, -3, -4, -5, -6        
      //break; case __LINE__: TRICE8(id(0), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:        default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7      
      //break; case __LINE__: TRICE8(id(0), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:        default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8       
      //break; case __LINE__: TRICE8(id(0), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8, -9   
      //break; case __LINE__: TRICE8(id(0), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:        default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
      //break; case __LINE__: TRICE8(id(0), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:        default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
      //break; case __LINE__: TRICE8(id(0), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
      //break; case __LINE__: TRICE8(Id(0), "rd:TRICE8 %d\n", -1 );                                                                                            //exp: time:    be16default: rd:TRICE8 -1     
      //break; case __LINE__: TRICE8(Id(0), "rd:TRICE8 %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:TRICE8 -1, -2 
      //break; case __LINE__: TRICE8(Id(0), "rd:TRICE8 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:TRICE8 -1, -2, -3       
      //break; case __LINE__: TRICE8(Id(0), "rd:TRICE8 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:TRICE8 -1, -2, -3, -4      
      //break; case __LINE__: TRICE8(Id(0), "rd:TRICE8 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:TRICE8 -1, -2, -3, -4, -5        
      //break; case __LINE__: TRICE8(Id(0), "rd:TRICE8 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:TRICE8 -1, -2, -3, -4, -5, -6        
      //break; case __LINE__: TRICE8(Id(0), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7      
      //break; case __LINE__: TRICE8(Id(0), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8       
      //break; case __LINE__: TRICE8(Id(0), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8, -9   
      //break; case __LINE__: TRICE8(Id(0), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
      //break; case __LINE__: TRICE8(Id(0), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
      //break; case __LINE__: TRICE8(Id(0), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
      //break; case __LINE__: TRICE8(ID(0), "rd:TRICE8 %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRICE8 -1     
      //break; case __LINE__: TRICE8(ID(0), "rd:TRICE8 %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRICE8 -1, -2 
      //break; case __LINE__: TRICE8(ID(0), "rd:TRICE8 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRICE8 -1, -2, -3       
      //break; case __LINE__: TRICE8(ID(0), "rd:TRICE8 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRICE8 -1, -2, -3, -4      
      //break; case __LINE__: TRICE8(ID(0), "rd:TRICE8 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRICE8 -1, -2, -3, -4, -5        
      //break; case __LINE__: TRICE8(ID(0), "rd:TRICE8 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRICE8 -1, -2, -3, -4, -5, -6        
      //break; case __LINE__: TRICE8(ID(0), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7      
      //break; case __LINE__: TRICE8(ID(0), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8       
      //break; case __LINE__: TRICE8(ID(0), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8, -9   
      //break; case __LINE__: TRICE8(ID(0), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
      //break; case __LINE__: TRICE8(ID(0), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
      //break; case __LINE__: TRICE8(ID(0), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

      //break; case __LINE__: trice("sig:TRICE8_n with 1 to 12 values\n" );

      //break; case __LINE__: TRICE8_1 (id(0), "rd:TRICE8_1  %d\n", -1 );                                                                                            //exp: time:        default: rd:TRICE8_1  -1     
      //break; case __LINE__: TRICE8_2 (id(0), "rd:TRICE8_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:        default: rd:TRICE8_2  -1, -2 
      //break; case __LINE__: TRICE8_3 (id(0), "rd:TRICE8_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: rd:TRICE8_3  -1, -2, -3       
      //break; case __LINE__: TRICE8_4 (id(0), "rd:TRICE8_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:        default: rd:TRICE8_4  -1, -2, -3, -4      
      //break; case __LINE__: TRICE8_5 (id(0), "rd:TRICE8_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:        default: rd:TRICE8_5  -1, -2, -3, -4, -5        
      //break; case __LINE__: TRICE8_6 (id(0), "rd:TRICE8_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: rd:TRICE8_6  -1, -2, -3, -4, -5, -6        
      //break; case __LINE__: TRICE8_7 (id(0), "rd:TRICE8_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:        default: rd:TRICE8_7  -1, -2, -3, -4, -5, -6, -7      
      //break; case __LINE__: TRICE8_8 (id(0), "rd:TRICE8_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:        default: rd:TRICE8_8  -1, -2, -3, -4, -5, -6, -7, -8       
      //break; case __LINE__: TRICE8_9 (id(0), "rd:TRICE8_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: rd:TRICE8_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
      //break; case __LINE__: TRICE8_10(id(0), "rd:TRICE8_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:        default: rd:TRICE8_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
      //break; case __LINE__: TRICE8_11(id(0), "rd:TRICE8_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:        default: rd:TRICE8_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
      //break; case __LINE__: TRICE8_12(id(0), "rd:TRICE8_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: rd:TRICE8_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
      //break; case __LINE__: TRICE8_1 (Id(0), "rd:TRICE8_1  %d\n", -1 );                                                                                            //exp: time:    be16default: rd:TRICE8_1  -1     
      //break; case __LINE__: TRICE8_2 (Id(0), "rd:TRICE8_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:TRICE8_2  -1, -2 
      //break; case __LINE__: TRICE8_3 (Id(0), "rd:TRICE8_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:TRICE8_3  -1, -2, -3       
      //break; case __LINE__: TRICE8_4 (Id(0), "rd:TRICE8_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:TRICE8_4  -1, -2, -3, -4      
      //break; case __LINE__: TRICE8_5 (Id(0), "rd:TRICE8_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:TRICE8_5  -1, -2, -3, -4, -5        
      //break; case __LINE__: TRICE8_6 (Id(0), "rd:TRICE8_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:TRICE8_6  -1, -2, -3, -4, -5, -6        
      //break; case __LINE__: TRICE8_7 (Id(0), "rd:TRICE8_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:TRICE8_7  -1, -2, -3, -4, -5, -6, -7      
      //break; case __LINE__: TRICE8_8 (Id(0), "rd:TRICE8_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:TRICE8_8  -1, -2, -3, -4, -5, -6, -7, -8       
      //break; case __LINE__: TRICE8_9 (Id(0), "rd:TRICE8_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:TRICE8_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
      //break; case __LINE__: TRICE8_10(Id(0), "rd:TRICE8_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:TRICE8_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
      //break; case __LINE__: TRICE8_11(Id(0), "rd:TRICE8_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:TRICE8_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
      //break; case __LINE__: TRICE8_12(Id(0), "rd:TRICE8_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:TRICE8_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
      //break; case __LINE__: TRICE8_1 (ID(0), "rd:TRICE8_1  %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRICE8_1  -1     
      //break; case __LINE__: TRICE8_2 (ID(0), "rd:TRICE8_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRICE8_2  -1, -2 
      //break; case __LINE__: TRICE8_3 (ID(0), "rd:TRICE8_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRICE8_3  -1, -2, -3       
      //break; case __LINE__: TRICE8_4 (ID(0), "rd:TRICE8_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRICE8_4  -1, -2, -3, -4      
      //break; case __LINE__: TRICE8_5 (ID(0), "rd:TRICE8_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRICE8_5  -1, -2, -3, -4, -5        
      //break; case __LINE__: TRICE8_6 (ID(0), "rd:TRICE8_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRICE8_6  -1, -2, -3, -4, -5, -6        
      //break; case __LINE__: TRICE8_7 (ID(0), "rd:TRICE8_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRICE8_7  -1, -2, -3, -4, -5, -6, -7      
      //break; case __LINE__: TRICE8_8 (ID(0), "rd:TRICE8_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRICE8_8  -1, -2, -3, -4, -5, -6, -7, -8       
      //break; case __LINE__: TRICE8_9 (ID(0), "rd:TRICE8_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRICE8_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
      //break; case __LINE__: TRICE8_10(ID(0), "rd:TRICE8_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRICE8_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
      //break; case __LINE__: TRICE8_11(ID(0), "rd:TRICE8_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRICE8_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
      //break; case __LINE__: TRICE8_12(ID(0), "rd:TRICE8_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRICE8_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: trice("sig:trice8 with 1 to 12 values\n" );
        
        break; case __LINE__: trice8("rd:trice8 %d\n", -1 );                                                                                            //exp: time:        default: rd:trice8 -1     
        break; case __LINE__: trice8("rd:trice8 %d, %d\n", -1, -2 );                                                                                    //exp: time:        default: rd:trice8 -1, -2 
        break; case __LINE__: trice8("rd:trice8 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: rd:trice8 -1, -2, -3       
        break; case __LINE__: trice8("rd:trice8 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:        default: rd:trice8 -1, -2, -3, -4      
        break; case __LINE__: trice8("rd:trice8 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:        default: rd:trice8 -1, -2, -3, -4, -5        
        break; case __LINE__: trice8("rd:trice8 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: rd:trice8 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice8("rd:trice8 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:        default: rd:trice8 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice8("rd:trice8 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:        default: rd:trice8 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice8("rd:trice8 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: rd:trice8 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice8("rd:trice8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:        default: rd:trice8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice8("rd:trice8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:        default: rd:trice8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice8("rd:trice8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: rd:trice8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: Trice8("rd:Trice8 %d\n", -1 );                                                                                            //exp: time:    be16default: rd:Trice8 -1     
        break; case __LINE__: Trice8("rd:Trice8 %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:Trice8 -1, -2 
        break; case __LINE__: Trice8("rd:Trice8 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:Trice8 -1, -2, -3       
        break; case __LINE__: Trice8("rd:Trice8 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:Trice8 -1, -2, -3, -4      
        break; case __LINE__: Trice8("rd:Trice8 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:Trice8 -1, -2, -3, -4, -5        
        break; case __LINE__: Trice8("rd:Trice8 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:Trice8 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: Trice8("rd:Trice8 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:Trice8 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: Trice8("rd:Trice8 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:Trice8 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: Trice8("rd:Trice8 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:Trice8 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: Trice8("rd:Trice8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:Trice8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: Trice8("rd:Trice8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:Trice8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: Trice8("rd:Trice8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:Trice8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRice8("rd:TRice8 %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRice8 -1     
        break; case __LINE__: TRice8("rd:TRice8 %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRice8 -1, -2 
        break; case __LINE__: TRice8("rd:TRice8 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRice8 -1, -2, -3       
        break; case __LINE__: TRice8("rd:TRice8 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRice8 -1, -2, -3, -4      
        break; case __LINE__: TRice8("rd:TRice8 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRice8 -1, -2, -3, -4, -5        
        break; case __LINE__: TRice8("rd:TRice8 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRice8 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRice8("rd:TRice8 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRice8 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRice8("rd:TRice8 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRice8 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRice8("rd:TRice8 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRice8 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRice8("rd:TRice8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRice8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRice8("rd:TRice8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRice8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRice8("rd:TRice8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRice8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: TRICE(ID(0), "sig:trice8_n with 1 to 12 values\n" );

        break; case __LINE__: trice8_1 ("rd:trice8_1  %d\n", -1 );                                                                                            //exp: time:        default: rd:trice8_1  -1     
        break; case __LINE__: trice8_2 ("rd:trice8_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:        default: rd:trice8_2  -1, -2 
        break; case __LINE__: trice8_3 ("rd:trice8_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: rd:trice8_3  -1, -2, -3       
        break; case __LINE__: trice8_4 ("rd:trice8_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:        default: rd:trice8_4  -1, -2, -3, -4      
        break; case __LINE__: trice8_5 ("rd:trice8_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:        default: rd:trice8_5  -1, -2, -3, -4, -5        
        break; case __LINE__: trice8_6 ("rd:trice8_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: rd:trice8_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice8_7 ("rd:trice8_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:        default: rd:trice8_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice8_8 ("rd:trice8_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:        default: rd:trice8_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice8_9 ("rd:trice8_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: rd:trice8_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice8_10("rd:trice8_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:        default: rd:trice8_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice8_11("rd:trice8_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:        default: rd:trice8_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice8_12("rd:trice8_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: rd:trice8_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: Trice8_1 ("rd:Trice8_1  %d\n", -1 );                                                                                            //exp: time:    be16default: rd:Trice8_1  -1     
        break; case __LINE__: Trice8_2 ("rd:Trice8_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:Trice8_2  -1, -2 
        break; case __LINE__: Trice8_3 ("rd:Trice8_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:Trice8_3  -1, -2, -3       
        break; case __LINE__: Trice8_4 ("rd:Trice8_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:Trice8_4  -1, -2, -3, -4      
        break; case __LINE__: Trice8_5 ("rd:Trice8_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:Trice8_5  -1, -2, -3, -4, -5        
        break; case __LINE__: Trice8_6 ("rd:Trice8_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:Trice8_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: Trice8_7 ("rd:Trice8_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:Trice8_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: Trice8_8 ("rd:Trice8_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:Trice8_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: Trice8_9 ("rd:Trice8_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:Trice8_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: Trice8_10("rd:Trice8_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:Trice8_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: Trice8_11("rd:Trice8_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:Trice8_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: Trice8_12("rd:Trice8_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:Trice8_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRice8_1 ("rd:TRice8_1  %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRice8_1  -1     
        break; case __LINE__: TRice8_2 ("rd:TRice8_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRice8_2  -1, -2 
        break; case __LINE__: TRice8_3 ("rd:TRice8_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRice8_3  -1, -2, -3       
        break; case __LINE__: TRice8_4 ("rd:TRice8_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRice8_4  -1, -2, -3, -4      
        break; case __LINE__: TRice8_5 ("rd:TRice8_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRice8_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRice8_6 ("rd:TRice8_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRice8_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRice8_7 ("rd:TRice8_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRice8_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRice8_8 ("rd:TRice8_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRice8_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRice8_9 ("rd:TRice8_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRice8_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRice8_10("rd:TRice8_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRice8_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRice8_11("rd:TRice8_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRice8_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRice8_12("rd:TRice8_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRice8_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: trice("sig:TRICE16 with 1 to 12 values (line %d)\n", __LINE__ );

        break; case __LINE__: trice16("rd:TRICE16 %d\n", -1 );                                                                                            //exp: time:        default: rd:TRICE16 -1     
        break; case __LINE__: trice16("rd:TRICE16 %d, %d\n", -1, -2 );                                                                                    //exp: time:        default: rd:TRICE16 -1, -2 
        break; case __LINE__: trice16("rd:TRICE16 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: rd:TRICE16 -1, -2, -3       
        break; case __LINE__: trice16("rd:TRICE16 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:        default: rd:TRICE16 -1, -2, -3, -4      
        break; case __LINE__: trice16("rd:TRICE16 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:        default: rd:TRICE16 -1, -2, -3, -4, -5        
        break; case __LINE__: trice16("rd:TRICE16 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: rd:TRICE16 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice16("rd:TRICE16 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:        default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice16("rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:        default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice16("rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice16("rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:        default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice16("rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:        default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice16("rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: trice16("rd:TRICE16 %d\n", -1 );                                                                                            //exp: time:        default: rd:TRICE16 -1     
        break; case __LINE__: trice16("rd:TRICE16 %d, %d\n", -1, -2 );                                                                                    //exp: time:        default: rd:TRICE16 -1, -2 
        break; case __LINE__: trice16("rd:TRICE16 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: rd:TRICE16 -1, -2, -3       
        break; case __LINE__: trice16("rd:TRICE16 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:        default: rd:TRICE16 -1, -2, -3, -4      
        break; case __LINE__: trice16("rd:TRICE16 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:        default: rd:TRICE16 -1, -2, -3, -4, -5        
        break; case __LINE__: trice16("rd:TRICE16 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: rd:TRICE16 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice16("rd:TRICE16 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:        default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice16("rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:        default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice16("rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice16("rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:        default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice16("rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:        default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice16("rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: trice16("rd:TRICE16 %d\n", -1 );                                                                                            //exp: time:        default: rd:TRICE16 -1     
        break; case __LINE__: trice16("rd:TRICE16 %d, %d\n", -1, -2 );                                                                                    //exp: time:        default: rd:TRICE16 -1, -2 
        break; case __LINE__: trice16("rd:TRICE16 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: rd:TRICE16 -1, -2, -3       
        break; case __LINE__: trice16("rd:TRICE16 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:        default: rd:TRICE16 -1, -2, -3, -4      
        break; case __LINE__: trice16("rd:TRICE16 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:        default: rd:TRICE16 -1, -2, -3, -4, -5        
        break; case __LINE__: trice16("rd:TRICE16 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: rd:TRICE16 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice16("rd:TRICE16 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:        default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice16("rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:        default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice16("rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice16("rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:        default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice16("rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:        default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice16("rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

      //break; case __LINE__: trice     ("sig:TRICE16_n with 1 to 12 values (line %d)\n", __LINE__ );
        
      //break; case __LINE__: TRICE16_1 (id(0), "rd:TRICE16_1  %d\n", -1 );                                                                                            //exp: time:        default: rd:TRICE16_1  -1     
      //break; case __LINE__: TRICE16_2 (id(0), "rd:TRICE16_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:        default: rd:TRICE16_2  -1, -2 
      //break; case __LINE__: TRICE16_3 (id(0), "rd:TRICE16_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: rd:TRICE16_3  -1, -2, -3       
      //break; case __LINE__: TRICE16_4 (id(0), "rd:TRICE16_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:        default: rd:TRICE16_4  -1, -2, -3, -4      
      //break; case __LINE__: TRICE16_5 (id(0), "rd:TRICE16_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:        default: rd:TRICE16_5  -1, -2, -3, -4, -5        
      //break; case __LINE__: TRICE16_6 (id(0), "rd:TRICE16_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: rd:TRICE16_6  -1, -2, -3, -4, -5, -6        
      //break; case __LINE__: TRICE16_7 (id(0), "rd:TRICE16_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:        default: rd:TRICE16_7  -1, -2, -3, -4, -5, -6, -7      
      //break; case __LINE__: TRICE16_8 (id(0), "rd:TRICE16_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:        default: rd:TRICE16_8  -1, -2, -3, -4, -5, -6, -7, -8       
      //break; case __LINE__: TRICE16_9 (id(0), "rd:TRICE16_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: rd:TRICE16_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
      //break; case __LINE__: TRICE16_10(id(0), "rd:TRICE16_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:        default: rd:TRICE16_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
      //break; case __LINE__: TRICE16_11(id(0), "rd:TRICE16_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:        default: rd:TRICE16_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
      //break; case __LINE__: TRICE16_12(id(0), "rd:TRICE16_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: rd:TRICE16_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
      //break; case __LINE__: TRICE16_1 (Id(0), "rd:TRICE16_1  %d\n", -1 );                                                                                            //exp: time:    be16default: rd:TRICE16_1  -1     
      //break; case __LINE__: TRICE16_2 (Id(0), "rd:TRICE16_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:TRICE16_2  -1, -2 
      //break; case __LINE__: TRICE16_3 (Id(0), "rd:TRICE16_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:TRICE16_3  -1, -2, -3       
      //break; case __LINE__: TRICE16_4 (Id(0), "rd:TRICE16_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:TRICE16_4  -1, -2, -3, -4      
      //break; case __LINE__: TRICE16_5 (Id(0), "rd:TRICE16_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:TRICE16_5  -1, -2, -3, -4, -5        
      //break; case __LINE__: TRICE16_6 (Id(0), "rd:TRICE16_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:TRICE16_6  -1, -2, -3, -4, -5, -6        
      //break; case __LINE__: TRICE16_7 (Id(0), "rd:TRICE16_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:TRICE16_7  -1, -2, -3, -4, -5, -6, -7      
      //break; case __LINE__: TRICE16_8 (Id(0), "rd:TRICE16_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:TRICE16_8  -1, -2, -3, -4, -5, -6, -7, -8       
      //break; case __LINE__: TRICE16_9 (Id(0), "rd:TRICE16_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:TRICE16_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
      //break; case __LINE__: TRICE16_10(Id(0), "rd:TRICE16_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:TRICE16_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
      //break; case __LINE__: TRICE16_11(Id(0), "rd:TRICE16_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:TRICE16_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
      //break; case __LINE__: TRICE16_12(Id(0), "rd:TRICE16_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:TRICE16_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
      //break; case __LINE__: TRICE16_1 (ID(0), "rd:TRICE16_1  %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRICE16_1  -1     
      //break; case __LINE__: TRICE16_2 (ID(0), "rd:TRICE16_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRICE16_2  -1, -2 
      //break; case __LINE__: TRICE16_3 (ID(0), "rd:TRICE16_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRICE16_3  -1, -2, -3       
      //break; case __LINE__: TRICE16_4 (ID(0), "rd:TRICE16_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRICE16_4  -1, -2, -3, -4      
      //break; case __LINE__: TRICE16_5 (ID(0), "rd:TRICE16_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRICE16_5  -1, -2, -3, -4, -5        
      //break; case __LINE__: TRICE16_6 (ID(0), "rd:TRICE16_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRICE16_6  -1, -2, -3, -4, -5, -6        
      //break; case __LINE__: TRICE16_7 (ID(0), "rd:TRICE16_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRICE16_7  -1, -2, -3, -4, -5, -6, -7      
      //break; case __LINE__: TRICE16_8 (ID(0), "rd:TRICE16_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRICE16_8  -1, -2, -3, -4, -5, -6, -7, -8       
      //break; case __LINE__: TRICE16_9 (ID(0), "rd:TRICE16_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRICE16_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
      //break; case __LINE__: TRICE16_10(ID(0), "rd:TRICE16_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRICE16_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
      //break; case __LINE__: TRICE16_11(ID(0), "rd:TRICE16_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRICE16_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
      //break; case __LINE__: TRICE16_12(ID(0), "rd:TRICE16_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRICE16_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: trice("sig:trice16 with 1 to 12 values (line %d)\n", __LINE__ );

        break; case __LINE__: trice16("rd:trice16 %d\n", -1 );                                                                                            //exp: time:        default: rd:trice16 -1     
        break; case __LINE__: trice16("rd:trice16 %d, %d\n", -1, -2 );                                                                                    //exp: time:        default: rd:trice16 -1, -2 
        break; case __LINE__: trice16("rd:trice16 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: rd:trice16 -1, -2, -3       
        break; case __LINE__: trice16("rd:trice16 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:        default: rd:trice16 -1, -2, -3, -4      
        break; case __LINE__: trice16("rd:trice16 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:        default: rd:trice16 -1, -2, -3, -4, -5        
        break; case __LINE__: trice16("rd:trice16 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: rd:trice16 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice16("rd:trice16 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:        default: rd:trice16 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice16("rd:trice16 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:        default: rd:trice16 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice16("rd:trice16 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: rd:trice16 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice16("rd:trice16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:        default: rd:trice16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice16("rd:trice16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:        default: rd:trice16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice16("rd:trice16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: rd:trice16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: Trice16("rd:Trice16 %d\n", -1 );                                                                                            //exp: time:    be16default: rd:Trice16 -1     
        break; case __LINE__: Trice16("rd:Trice16 %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:Trice16 -1, -2 
        break; case __LINE__: Trice16("rd:Trice16 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:Trice16 -1, -2, -3       
        break; case __LINE__: Trice16("rd:Trice16 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:Trice16 -1, -2, -3, -4      
        break; case __LINE__: Trice16("rd:Trice16 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:Trice16 -1, -2, -3, -4, -5        
        break; case __LINE__: Trice16("rd:Trice16 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:Trice16 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: Trice16("rd:Trice16 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:Trice16 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: Trice16("rd:Trice16 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:Trice16 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: Trice16("rd:Trice16 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:Trice16 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: Trice16("rd:Trice16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:Trice16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: Trice16("rd:Trice16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:Trice16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: Trice16("rd:Trice16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:Trice16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRice16("rd:TRice16 %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRice16 -1     
        break; case __LINE__: TRice16("rd:TRice16 %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRice16 -1, -2 
        break; case __LINE__: TRice16("rd:TRice16 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRice16 -1, -2, -3       
        break; case __LINE__: TRice16("rd:TRice16 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRice16 -1, -2, -3, -4      
        break; case __LINE__: TRice16("rd:TRice16 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRice16 -1, -2, -3, -4, -5        
        break; case __LINE__: TRice16("rd:TRice16 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRice16 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRice16("rd:TRice16 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRice16 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRice16("rd:TRice16 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRice16 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRice16("rd:TRice16 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRice16 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRice16("rd:TRice16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRice16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRice16("rd:TRice16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRice16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRice16("rd:TRice16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRice16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: trice     ("sig:trice16_n with 1 to 12 values (line %d)\n", __LINE__ );
        
        break; case __LINE__: trice16_1 ("rd:trice16_1  %d\n", -1 );                                                                                            //exp: time:        default: rd:trice16_1  -1     
        break; case __LINE__: trice16_2 ("rd:trice16_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:        default: rd:trice16_2  -1, -2 
        break; case __LINE__: trice16_3 ("rd:trice16_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: rd:trice16_3  -1, -2, -3       
        break; case __LINE__: trice16_4 ("rd:trice16_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:        default: rd:trice16_4  -1, -2, -3, -4      
        break; case __LINE__: trice16_5 ("rd:trice16_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:        default: rd:trice16_5  -1, -2, -3, -4, -5        
        break; case __LINE__: trice16_6 ("rd:trice16_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: rd:trice16_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice16_7 ("rd:trice16_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:        default: rd:trice16_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice16_8 ("rd:trice16_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:        default: rd:trice16_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice16_9 ("rd:trice16_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: rd:trice16_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice16_10("rd:trice16_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:        default: rd:trice16_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice16_11("rd:trice16_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:        default: rd:trice16_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice16_12("rd:trice16_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: rd:trice16_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: Trice16_1 ("rd:Trice16_1  %d\n", -1 );                                                                                            //exp: time:    be16default: rd:Trice16_1  -1     
        break; case __LINE__: Trice16_2 ("rd:Trice16_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:Trice16_2  -1, -2 
        break; case __LINE__: Trice16_3 ("rd:Trice16_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:Trice16_3  -1, -2, -3       
        break; case __LINE__: Trice16_4 ("rd:Trice16_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:Trice16_4  -1, -2, -3, -4      
        break; case __LINE__: Trice16_5 ("rd:Trice16_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:Trice16_5  -1, -2, -3, -4, -5        
        break; case __LINE__: Trice16_6 ("rd:Trice16_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:Trice16_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: Trice16_7 ("rd:Trice16_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:Trice16_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: Trice16_8 ("rd:Trice16_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:Trice16_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: Trice16_9 ("rd:Trice16_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:Trice16_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: Trice16_10("rd:Trice16_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:Trice16_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: Trice16_11("rd:Trice16_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:Trice16_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: Trice16_12("rd:Trice16_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:Trice16_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRice16_1 ("rd:TRice16_1  %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRice16_1  -1     
        break; case __LINE__: TRice16_2 ("rd:TRice16_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRice16_2  -1, -2 
        break; case __LINE__: TRice16_3 ("rd:TRice16_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRice16_3  -1, -2, -3       
        break; case __LINE__: TRice16_4 ("rd:TRice16_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRice16_4  -1, -2, -3, -4      
        break; case __LINE__: TRice16_5 ("rd:TRice16_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRice16_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRice16_6 ("rd:TRice16_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRice16_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRice16_7 ("rd:TRice16_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRice16_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRice16_8 ("rd:TRice16_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRice16_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRice16_9 ("rd:TRice16_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRice16_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRice16_10("rd:TRice16_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRice16_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRice16_11("rd:TRice16_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRice16_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRice16_12("rd:TRice16_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRice16_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

      //break; case __LINE__: trice  ("sig:TRICE32 with 1 to 12 values (line %d)\n", __LINE__ );

      //break; case __LINE__: TRICE32(id(0), "rd:TRICE32 %d\n", -1 );                                                                                            //exp: time:        default: rd:TRICE32 -1     
      //break; case __LINE__: TRICE32(id(0), "rd:TRICE32 %d, %d\n", -1, -2 );                                                                                    //exp: time:        default: rd:TRICE32 -1, -2 
      //break; case __LINE__: TRICE32(id(0), "rd:TRICE32 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: rd:TRICE32 -1, -2, -3       
      //break; case __LINE__: TRICE32(id(0), "rd:TRICE32 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:        default: rd:TRICE32 -1, -2, -3, -4      
      //break; case __LINE__: TRICE32(id(0), "rd:TRICE32 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:        default: rd:TRICE32 -1, -2, -3, -4, -5        
      //break; case __LINE__: TRICE32(id(0), "rd:TRICE32 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: rd:TRICE32 -1, -2, -3, -4, -5, -6        
      //break; case __LINE__: TRICE32(id(0), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:        default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7      
      //break; case __LINE__: TRICE32(id(0), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:        default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8       
      //break; case __LINE__: TRICE32(id(0), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8, -9   
      //break; case __LINE__: TRICE32(id(0), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:        default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
      //break; case __LINE__: TRICE32(id(0), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:        default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
      //break; case __LINE__: TRICE32(id(0), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
      //break; case __LINE__: TRICE32(Id(0), "rd:TRICE32 %d\n", -1 );                                                                                            //exp: time:    be16default: rd:TRICE32 -1     
      //break; case __LINE__: TRICE32(Id(0), "rd:TRICE32 %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:TRICE32 -1, -2 
      //break; case __LINE__: TRICE32(Id(0), "rd:TRICE32 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:TRICE32 -1, -2, -3       
      //break; case __LINE__: TRICE32(Id(0), "rd:TRICE32 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:TRICE32 -1, -2, -3, -4      
      //break; case __LINE__: TRICE32(Id(0), "rd:TRICE32 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:TRICE32 -1, -2, -3, -4, -5        
      //break; case __LINE__: TRICE32(Id(0), "rd:TRICE32 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:TRICE32 -1, -2, -3, -4, -5, -6        
      //break; case __LINE__: TRICE32(Id(0), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7      
      //break; case __LINE__: TRICE32(Id(0), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8       
      //break; case __LINE__: TRICE32(Id(0), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8, -9   
      //break; case __LINE__: TRICE32(Id(0), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
      //break; case __LINE__: TRICE32(Id(0), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
      //break; case __LINE__: TRICE32(Id(0), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
      //break; case __LINE__: TRICE32(ID(0), "rd:TRICE32 %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRICE32 -1     
      //break; case __LINE__: TRICE32(ID(0), "rd:TRICE32 %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRICE32 -1, -2 
      //break; case __LINE__: TRICE32(ID(0), "rd:TRICE32 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRICE32 -1, -2, -3       
      //break; case __LINE__: TRICE32(ID(0), "rd:TRICE32 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRICE32 -1, -2, -3, -4      
      //break; case __LINE__: TRICE32(ID(0), "rd:TRICE32 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRICE32 -1, -2, -3, -4, -5        
      //break; case __LINE__: TRICE32(ID(0), "rd:TRICE32 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRICE32 -1, -2, -3, -4, -5, -6        
      //break; case __LINE__: TRICE32(ID(0), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7      
      //break; case __LINE__: TRICE32(ID(0), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8       
      //break; case __LINE__: TRICE32(ID(0), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8, -9   
      //break; case __LINE__: TRICE32(ID(0), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
      //break; case __LINE__: TRICE32(ID(0), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
      //break; case __LINE__: TRICE32(ID(0), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

      //break; case __LINE__: trice     ("signal:TRICE32_n with 1 to 12 values (line %d)\n", __LINE__ );

      //break; case __LINE__: TRICE32_1 (id(0), "rd:TRICE32_1  %d\n", -1 );                                                                                            //exp: time:        default: rd:TRICE32_1  -1     
      //break; case __LINE__: TRICE32_2 (id(0), "rd:TRICE32_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:        default: rd:TRICE32_2  -1, -2 
      //break; case __LINE__: TRICE32_3 (id(0), "rd:TRICE32_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: rd:TRICE32_3  -1, -2, -3       
      //break; case __LINE__: TRICE32_4 (id(0), "rd:TRICE32_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:        default: rd:TRICE32_4  -1, -2, -3, -4      
      //break; case __LINE__: TRICE32_5 (id(0), "rd:TRICE32_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:        default: rd:TRICE32_5  -1, -2, -3, -4, -5        
      //break; case __LINE__: TRICE32_6 (id(0), "rd:TRICE32_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: rd:TRICE32_6  -1, -2, -3, -4, -5, -6        
      //break; case __LINE__: TRICE32_7 (id(0), "rd:TRICE32_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:        default: rd:TRICE32_7  -1, -2, -3, -4, -5, -6, -7      
      //break; case __LINE__: TRICE32_8 (id(0), "rd:TRICE32_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:        default: rd:TRICE32_8  -1, -2, -3, -4, -5, -6, -7, -8       
      //break; case __LINE__: TRICE32_9 (id(0), "rd:TRICE32_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: rd:TRICE32_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
      //break; case __LINE__: TRICE32_10(id(0), "rd:TRICE32_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:        default: rd:TRICE32_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
      //break; case __LINE__: TRICE32_11(id(0), "rd:TRICE32_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:        default: rd:TRICE32_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
      //break; case __LINE__: TRICE32_12(id(0), "rd:TRICE32_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: rd:TRICE32_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
      //break; case __LINE__: TRICE32_1 (Id(0), "rd:TRICE32_1  %d\n", -1 );                                                                                            //exp: time:    be16default: rd:TRICE32_1  -1     
      //break; case __LINE__: TRICE32_2 (Id(0), "rd:TRICE32_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:TRICE32_2  -1, -2 
      //break; case __LINE__: TRICE32_3 (Id(0), "rd:TRICE32_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:TRICE32_3  -1, -2, -3       
      //break; case __LINE__: TRICE32_4 (Id(0), "rd:TRICE32_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:TRICE32_4  -1, -2, -3, -4      
      //break; case __LINE__: TRICE32_5 (Id(0), "rd:TRICE32_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:TRICE32_5  -1, -2, -3, -4, -5        
      //break; case __LINE__: TRICE32_6 (Id(0), "rd:TRICE32_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:TRICE32_6  -1, -2, -3, -4, -5, -6        
      //break; case __LINE__: TRICE32_7 (Id(0), "rd:TRICE32_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:TRICE32_7  -1, -2, -3, -4, -5, -6, -7      
      //break; case __LINE__: TRICE32_8 (Id(0), "rd:TRICE32_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:TRICE32_8  -1, -2, -3, -4, -5, -6, -7, -8       
      //break; case __LINE__: TRICE32_9 (Id(0), "rd:TRICE32_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:TRICE32_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
      //break; case __LINE__: TRICE32_10(Id(0), "rd:TRICE32_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:TRICE32_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
      //break; case __LINE__: TRICE32_11(Id(0), "rd:TRICE32_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:TRICE32_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
      //break; case __LINE__: TRICE32_12(Id(0), "rd:TRICE32_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:TRICE32_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
      //break; case __LINE__: TRICE32_1 (ID(0), "rd:TRICE32_1  %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRICE32_1  -1     
      //break; case __LINE__: TRICE32_2 (ID(0), "rd:TRICE32_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRICE32_2  -1, -2 
      //break; case __LINE__: TRICE32_3 (ID(0), "rd:TRICE32_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRICE32_3  -1, -2, -3       
      //break; case __LINE__: TRICE32_4 (ID(0), "rd:TRICE32_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRICE32_4  -1, -2, -3, -4      
      //break; case __LINE__: TRICE32_5 (ID(0), "rd:TRICE32_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRICE32_5  -1, -2, -3, -4, -5        
      //break; case __LINE__: TRICE32_6 (ID(0), "rd:TRICE32_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRICE32_6  -1, -2, -3, -4, -5, -6        
      //break; case __LINE__: TRICE32_7 (ID(0), "rd:TRICE32_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRICE32_7  -1, -2, -3, -4, -5, -6, -7      
      //break; case __LINE__: TRICE32_8 (ID(0), "rd:TRICE32_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRICE32_8  -1, -2, -3, -4, -5, -6, -7, -8       
      //break; case __LINE__: TRICE32_9 (ID(0), "rd:TRICE32_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRICE32_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
      //break; case __LINE__: TRICE32_10(ID(0), "rd:TRICE32_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRICE32_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
      //break; case __LINE__: TRICE32_11(ID(0), "rd:TRICE32_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRICE32_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
      //break; case __LINE__: TRICE32_12(ID(0), "rd:TRICE32_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRICE32_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: trice  ("sig:trice32 with 1 to 12 values (line %d)\n", __LINE__ );

        break; case __LINE__: trice32("rd:trice32 %d\n", -1 );                                                                                            //exp: time:        default: rd:trice32 -1     
        break; case __LINE__: trice32("rd:trice32 %d, %d\n", -1, -2 );                                                                                    //exp: time:        default: rd:trice32 -1, -2 
        break; case __LINE__: trice32("rd:trice32 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: rd:trice32 -1, -2, -3       
        break; case __LINE__: trice32("rd:trice32 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:        default: rd:trice32 -1, -2, -3, -4      
        break; case __LINE__: trice32("rd:trice32 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:        default: rd:trice32 -1, -2, -3, -4, -5        
        break; case __LINE__: trice32("rd:trice32 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: rd:trice32 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice32("rd:trice32 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:        default: rd:trice32 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice32("rd:trice32 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:        default: rd:trice32 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice32("rd:trice32 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: rd:trice32 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice32("rd:trice32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:        default: rd:trice32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice32("rd:trice32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:        default: rd:trice32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice32("rd:trice32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: rd:trice32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: Trice32("rd:Trice32 %d\n", -1 );                                                                                            //exp: time:    be16default: rd:Trice32 -1     
        break; case __LINE__: Trice32("rd:Trice32 %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:Trice32 -1, -2 
        break; case __LINE__: Trice32("rd:Trice32 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:Trice32 -1, -2, -3       
        break; case __LINE__: Trice32("rd:Trice32 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:Trice32 -1, -2, -3, -4      
        break; case __LINE__: Trice32("rd:Trice32 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:Trice32 -1, -2, -3, -4, -5        
        break; case __LINE__: Trice32("rd:Trice32 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:Trice32 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: Trice32("rd:Trice32 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:Trice32 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: Trice32("rd:Trice32 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:Trice32 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: Trice32("rd:Trice32 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:Trice32 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: Trice32("rd:Trice32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:Trice32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: Trice32("rd:Trice32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:Trice32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: Trice32("rd:Trice32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:Trice32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRice32("rd:TRice32 %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRice32 -1     
        break; case __LINE__: TRice32("rd:TRice32 %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRice32 -1, -2 
        break; case __LINE__: TRice32("rd:TRice32 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRice32 -1, -2, -3       
        break; case __LINE__: TRice32("rd:TRice32 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRice32 -1, -2, -3, -4      
        break; case __LINE__: TRice32("rd:TRice32 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRice32 -1, -2, -3, -4, -5        
        break; case __LINE__: TRice32("rd:TRice32 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRice32 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRice32("rd:TRice32 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRice32 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRice32("rd:TRice32 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRice32 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRice32("rd:TRice32 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRice32 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRice32("rd:TRice32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRice32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRice32("rd:TRice32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRice32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRice32("rd:TRice32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRice32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: trice     ("signal:trice32_n with 1 to 12 values (line %d)\n", __LINE__ );

        break; case __LINE__: trice32_1 ("rd:trice32_1  %d\n", -1 );                                                                                            //exp: time:        default: rd:trice32_1  -1     
        break; case __LINE__: trice32_2 ("rd:trice32_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:        default: rd:trice32_2  -1, -2 
        break; case __LINE__: trice32_3 ("rd:trice32_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: rd:trice32_3  -1, -2, -3       
        break; case __LINE__: trice32_4 ("rd:trice32_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:        default: rd:trice32_4  -1, -2, -3, -4      
        break; case __LINE__: trice32_5 ("rd:trice32_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:        default: rd:trice32_5  -1, -2, -3, -4, -5        
        break; case __LINE__: trice32_6 ("rd:trice32_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: rd:trice32_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice32_7 ("rd:trice32_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:        default: rd:trice32_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice32_8 ("rd:trice32_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:        default: rd:trice32_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice32_9 ("rd:trice32_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: rd:trice32_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice32_10("rd:trice32_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:        default: rd:trice32_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice32_11("rd:trice32_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:        default: rd:trice32_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice32_12("rd:trice32_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: rd:trice32_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: Trice32_1 ("rd:Trice32_1  %d\n", -1 );                                                                                            //exp: time:    be16default: rd:Trice32_1  -1     
        break; case __LINE__: Trice32_2 ("rd:Trice32_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:Trice32_2  -1, -2 
        break; case __LINE__: Trice32_3 ("rd:Trice32_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:Trice32_3  -1, -2, -3       
        break; case __LINE__: Trice32_4 ("rd:Trice32_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:Trice32_4  -1, -2, -3, -4      
        break; case __LINE__: Trice32_5 ("rd:Trice32_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:Trice32_5  -1, -2, -3, -4, -5        
        break; case __LINE__: Trice32_6 ("rd:Trice32_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:Trice32_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: Trice32_7 ("rd:Trice32_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:Trice32_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: Trice32_8 ("rd:Trice32_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:Trice32_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: Trice32_9 ("rd:Trice32_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:Trice32_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: Trice32_10("rd:Trice32_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:Trice32_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: Trice32_11("rd:Trice32_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:Trice32_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: Trice32_12("rd:Trice32_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:Trice32_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRice32_1 ("rd:TRice32_1  %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRice32_1  -1     
        break; case __LINE__: TRice32_2 ("rd:TRice32_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRice32_2  -1, -2 
        break; case __LINE__: TRice32_3 ("rd:TRice32_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRice32_3  -1, -2, -3       
        break; case __LINE__: TRice32_4 ("rd:TRice32_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRice32_4  -1, -2, -3, -4      
        break; case __LINE__: TRice32_5 ("rd:TRice32_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRice32_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRice32_6 ("rd:TRice32_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRice32_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRice32_7 ("rd:TRice32_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRice32_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRice32_8 ("rd:TRice32_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRice32_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRice32_9 ("rd:TRice32_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRice32_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRice32_10("rd:TRice32_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRice32_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRice32_11("rd:TRice32_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRice32_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRice32_12("rd:TRice32_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRice32_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

      //break; case __LINE__: trice  ("sig:TRICE64 with 1 to 12 values (line %d)\n", __LINE__ );

      //break; case __LINE__: TRICE64(id(0), "rd:TRICE64 %d\n", -1 );                                                                                            //exp: time:        default: rd:TRICE64 -1     
      //break; case __LINE__: TRICE64(id(0), "rd:TRICE64 %d, %d\n", -1, -2 );                                                                                    //exp: time:        default: rd:TRICE64 -1, -2 
      //break; case __LINE__: TRICE64(id(0), "rd:TRICE64 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: rd:TRICE64 -1, -2, -3       
      //break; case __LINE__: TRICE64(id(0), "rd:TRICE64 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:        default: rd:TRICE64 -1, -2, -3, -4      
      //break; case __LINE__: TRICE64(id(0), "rd:TRICE64 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:        default: rd:TRICE64 -1, -2, -3, -4, -5        
      //break; case __LINE__: TRICE64(id(0), "rd:TRICE64 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: rd:TRICE64 -1, -2, -3, -4, -5, -6        
      //break; case __LINE__: TRICE64(id(0), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:        default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7      
      //break; case __LINE__: TRICE64(id(0), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:        default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8       
      //break; case __LINE__: TRICE64(id(0), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8, -9   
      //break; case __LINE__: TRICE64(id(0), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:        default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
      //break; case __LINE__: TRICE64(id(0), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:        default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
      //break; case __LINE__: TRICE64(id(0), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
      //break; case __LINE__: TRICE64(Id(0), "rd:TRICE64 %d\n", -1 );                                                                                            //exp: time:    be16default: rd:TRICE64 -1     
      //break; case __LINE__: TRICE64(Id(0), "rd:TRICE64 %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:TRICE64 -1, -2 
      //break; case __LINE__: TRICE64(Id(0), "rd:TRICE64 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:TRICE64 -1, -2, -3       
      //break; case __LINE__: TRICE64(Id(0), "rd:TRICE64 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:TRICE64 -1, -2, -3, -4      
      //break; case __LINE__: TRICE64(Id(0), "rd:TRICE64 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:TRICE64 -1, -2, -3, -4, -5        
      //break; case __LINE__: TRICE64(Id(0), "rd:TRICE64 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:TRICE64 -1, -2, -3, -4, -5, -6        
      //break; case __LINE__: TRICE64(Id(0), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7      
      //break; case __LINE__: TRICE64(Id(0), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8       
      //break; case __LINE__: TRICE64(Id(0), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8, -9   
      //break; case __LINE__: TRICE64(Id(0), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
      //break; case __LINE__: TRICE64(Id(0), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
      //break; case __LINE__: TRICE64(Id(0), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
      //break; case __LINE__: TRICE64(ID(0), "rd:TRICE64 %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRICE64 -1     
      //break; case __LINE__: TRICE64(ID(0), "rd:TRICE64 %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRICE64 -1, -2 
      //break; case __LINE__: TRICE64(ID(0), "rd:TRICE64 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRICE64 -1, -2, -3       
      //break; case __LINE__: TRICE64(ID(0), "rd:TRICE64 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRICE64 -1, -2, -3, -4      
      //break; case __LINE__: TRICE64(ID(0), "rd:TRICE64 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRICE64 -1, -2, -3, -4, -5        
      //break; case __LINE__: TRICE64(ID(0), "rd:TRICE64 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRICE64 -1, -2, -3, -4, -5, -6        
      //break; case __LINE__: TRICE64(ID(0), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7      
      //break; case __LINE__: TRICE64(ID(0), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8       
      //break; case __LINE__: TRICE64(ID(0), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8, -9   
      //break; case __LINE__: TRICE64(ID(0), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
      //break; case __LINE__: TRICE64(ID(0), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
      //break; case __LINE__: TRICE64(ID(0), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

      //break; case __LINE__: trice     ("signal:TRICE64_n with 1 to 12 values (line %d)\n", __LINE__ );

      //break; case __LINE__: TRICE64_1 (id(0), "rd:TRICE64_1  %d\n", -1 );                                                                                            //exp: time:        default: rd:TRICE64_1  -1     
      //break; case __LINE__: TRICE64_2 (id(0), "rd:TRICE64_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:        default: rd:TRICE64_2  -1, -2 
      //break; case __LINE__: TRICE64_3 (id(0), "rd:TRICE64_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: rd:TRICE64_3  -1, -2, -3       
      //break; case __LINE__: TRICE64_4 (id(0), "rd:TRICE64_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:        default: rd:TRICE64_4  -1, -2, -3, -4      
      //break; case __LINE__: TRICE64_5 (id(0), "rd:TRICE64_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:        default: rd:TRICE64_5  -1, -2, -3, -4, -5        
      //break; case __LINE__: TRICE64_6 (id(0), "rd:TRICE64_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: rd:TRICE64_6  -1, -2, -3, -4, -5, -6        
      //break; case __LINE__: TRICE64_7 (id(0), "rd:TRICE64_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:        default: rd:TRICE64_7  -1, -2, -3, -4, -5, -6, -7      
      //break; case __LINE__: TRICE64_8 (id(0), "rd:TRICE64_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:        default: rd:TRICE64_8  -1, -2, -3, -4, -5, -6, -7, -8       
      //break; case __LINE__: TRICE64_9 (id(0), "rd:TRICE64_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: rd:TRICE64_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
      //break; case __LINE__: TRICE64_10(id(0), "rd:TRICE64_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:        default: rd:TRICE64_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
      //break; case __LINE__: TRICE64_11(id(0), "rd:TRICE64_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:        default: rd:TRICE64_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
      //break; case __LINE__: TRICE64_12(id(0), "rd:TRICE64_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: rd:TRICE64_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
      //break; case __LINE__: TRICE64_1 (Id(0), "rd:TRICE64_1  %d\n", -1 );                                                                                            //exp: time:    be16default: rd:TRICE64_1  -1     
      //break; case __LINE__: TRICE64_2 (Id(0), "rd:TRICE64_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:TRICE64_2  -1, -2 
      //break; case __LINE__: TRICE64_3 (Id(0), "rd:TRICE64_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:TRICE64_3  -1, -2, -3       
      //break; case __LINE__: TRICE64_4 (Id(0), "rd:TRICE64_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:TRICE64_4  -1, -2, -3, -4      
      //break; case __LINE__: TRICE64_5 (Id(0), "rd:TRICE64_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:TRICE64_5  -1, -2, -3, -4, -5        
      //break; case __LINE__: TRICE64_6 (Id(0), "rd:TRICE64_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:TRICE64_6  -1, -2, -3, -4, -5, -6        
      //break; case __LINE__: TRICE64_7 (Id(0), "rd:TRICE64_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:TRICE64_7  -1, -2, -3, -4, -5, -6, -7      
      //break; case __LINE__: TRICE64_8 (Id(0), "rd:TRICE64_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:TRICE64_8  -1, -2, -3, -4, -5, -6, -7, -8       
      //break; case __LINE__: TRICE64_9 (Id(0), "rd:TRICE64_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:TRICE64_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
      //break; case __LINE__: TRICE64_10(Id(0), "rd:TRICE64_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:TRICE64_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
      //break; case __LINE__: TRICE64_11(Id(0), "rd:TRICE64_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:TRICE64_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
      //break; case __LINE__: TRICE64_12(Id(0), "rd:TRICE64_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:TRICE64_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
      //break; case __LINE__: TRICE64_1 (ID(0), "rd:TRICE64_1  %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRICE64_1  -1     
      //break; case __LINE__: TRICE64_2 (ID(0), "rd:TRICE64_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRICE64_2  -1, -2 
      //break; case __LINE__: TRICE64_3 (ID(0), "rd:TRICE64_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRICE64_3  -1, -2, -3       
      //break; case __LINE__: TRICE64_4 (ID(0), "rd:TRICE64_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRICE64_4  -1, -2, -3, -4      
      //break; case __LINE__: TRICE64_5 (ID(0), "rd:TRICE64_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRICE64_5  -1, -2, -3, -4, -5        
      //break; case __LINE__: TRICE64_6 (ID(0), "rd:TRICE64_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRICE64_6  -1, -2, -3, -4, -5, -6        
      //break; case __LINE__: TRICE64_7 (ID(0), "rd:TRICE64_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRICE64_7  -1, -2, -3, -4, -5, -6, -7      
      //break; case __LINE__: TRICE64_8 (ID(0), "rd:TRICE64_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRICE64_8  -1, -2, -3, -4, -5, -6, -7, -8       
      //break; case __LINE__: TRICE64_9 (ID(0), "rd:TRICE64_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRICE64_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
      //break; case __LINE__: TRICE64_10(ID(0), "rd:TRICE64_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRICE64_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
      //break; case __LINE__: TRICE64_11(ID(0), "rd:TRICE64_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRICE64_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
      //break; case __LINE__: TRICE64_12(ID(0), "rd:TRICE64_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRICE64_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: trice  ("sig:trice64 with 1 to 12 values (line %d)\n", __LINE__ );

        break; case __LINE__: trice64("rd:trice64 %d\n", -1 );                                                                                            //exp: time:        default: rd:trice64 -1     
        break; case __LINE__: trice64("rd:trice64 %d, %d\n", -1, -2 );                                                                                    //exp: time:        default: rd:trice64 -1, -2 
        break; case __LINE__: trice64("rd:trice64 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: rd:trice64 -1, -2, -3       
        break; case __LINE__: trice64("rd:trice64 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:        default: rd:trice64 -1, -2, -3, -4      
        break; case __LINE__: trice64("rd:trice64 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:        default: rd:trice64 -1, -2, -3, -4, -5        
        break; case __LINE__: trice64("rd:trice64 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: rd:trice64 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice64("rd:trice64 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:        default: rd:trice64 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice64("rd:trice64 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:        default: rd:trice64 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice64("rd:trice64 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: rd:trice64 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice64("rd:trice64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:        default: rd:trice64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice64("rd:trice64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:        default: rd:trice64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice64("rd:trice64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: rd:trice64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: Trice64("rd:Trice64 %d\n", -1 );                                                                                            //exp: time:    be16default: rd:Trice64 -1     
        break; case __LINE__: Trice64("rd:Trice64 %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:Trice64 -1, -2 
        break; case __LINE__: Trice64("rd:Trice64 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:Trice64 -1, -2, -3       
        break; case __LINE__: Trice64("rd:Trice64 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:Trice64 -1, -2, -3, -4      
        break; case __LINE__: Trice64("rd:Trice64 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:Trice64 -1, -2, -3, -4, -5        
        break; case __LINE__: Trice64("rd:Trice64 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:Trice64 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: Trice64("rd:Trice64 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:Trice64 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: Trice64("rd:Trice64 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:Trice64 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: Trice64("rd:Trice64 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:Trice64 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: Trice64("rd:Trice64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:Trice64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: Trice64("rd:Trice64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:Trice64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: Trice64("rd:Trice64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:Trice64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRice64("rd:TRice64 %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRice64 -1     
        break; case __LINE__: TRice64("rd:TRice64 %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRice64 -1, -2 
        break; case __LINE__: TRice64("rd:TRice64 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRice64 -1, -2, -3       
        break; case __LINE__: TRice64("rd:TRice64 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRice64 -1, -2, -3, -4      
        break; case __LINE__: TRice64("rd:TRice64 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRice64 -1, -2, -3, -4, -5        
        break; case __LINE__: TRice64("rd:TRice64 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRice64 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRice64("rd:TRice64 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRice64 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRice64("rd:TRice64 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRice64 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRice64("rd:TRice64 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRice64 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRice64("rd:TRice64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRice64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRice64("rd:TRice64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRice64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRice64("rd:TRice64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRice64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: trice     ("signal:trice64_n with 1 to 12 values (line %d)\n", __LINE__ );

        break; case __LINE__: trice64_1 ("rd:trice64_1  %d\n", -1 );                                                                                            //exp: time:        default: rd:trice64_1  -1     
        break; case __LINE__: trice64_2 ("rd:trice64_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:        default: rd:trice64_2  -1, -2 
        break; case __LINE__: trice64_3 ("rd:trice64_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: rd:trice64_3  -1, -2, -3       
        break; case __LINE__: trice64_4 ("rd:trice64_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:        default: rd:trice64_4  -1, -2, -3, -4      
        break; case __LINE__: trice64_5 ("rd:trice64_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:        default: rd:trice64_5  -1, -2, -3, -4, -5        
        break; case __LINE__: trice64_6 ("rd:trice64_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: rd:trice64_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice64_7 ("rd:trice64_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:        default: rd:trice64_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice64_8 ("rd:trice64_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:        default: rd:trice64_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice64_9 ("rd:trice64_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: rd:trice64_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice64_10("rd:trice64_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:        default: rd:trice64_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice64_11("rd:trice64_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:        default: rd:trice64_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice64_12("rd:trice64_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: rd:trice64_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: Trice64_1 ("rd:Trice64_1  %d\n", -1 );                                                                                            //exp: time:    be16default: rd:Trice64_1  -1     
        break; case __LINE__: Trice64_2 ("rd:Trice64_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:Trice64_2  -1, -2 
        break; case __LINE__: Trice64_3 ("rd:Trice64_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:Trice64_3  -1, -2, -3       
        break; case __LINE__: Trice64_4 ("rd:Trice64_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:Trice64_4  -1, -2, -3, -4      
        break; case __LINE__: Trice64_5 ("rd:Trice64_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:Trice64_5  -1, -2, -3, -4, -5        
        break; case __LINE__: Trice64_6 ("rd:Trice64_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:Trice64_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: Trice64_7 ("rd:Trice64_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:Trice64_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: Trice64_8 ("rd:Trice64_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:Trice64_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: Trice64_9 ("rd:Trice64_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:Trice64_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: Trice64_10("rd:Trice64_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:Trice64_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: Trice64_11("rd:Trice64_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:Trice64_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: Trice64_12("rd:Trice64_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:Trice64_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRice64_1 ("rd:TRice64_1  %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRice64_1  -1     
        break; case __LINE__: TRice64_2 ("rd:TRice64_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRice64_2  -1, -2 
        break; case __LINE__: TRice64_3 ("rd:TRice64_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRice64_3  -1, -2, -3       
        break; case __LINE__: TRice64_4 ("rd:TRice64_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRice64_4  -1, -2, -3, -4      
        break; case __LINE__: TRice64_5 ("rd:TRice64_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRice64_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRice64_6 ("rd:TRice64_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRice64_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRice64_7 ("rd:TRice64_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRice64_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRice64_8 ("rd:TRice64_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRice64_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRice64_9 ("rd:TRice64_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRice64_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRice64_10("rd:TRice64_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRice64_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRice64_11("rd:TRice64_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRice64_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRice32_12("rd:TRice32_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRice32_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: TRICE(ID(0), "sig:Colors and ticks\n" );
        break; case __LINE__: TRICE(ID(0), "--------------------------------------------------\n");
        break; case __LINE__: TRICE(ID(0), "--------------------------------------------------\n");
        break; case __LINE__: TRICE(ID(0), "dbg:12345 as 16bit is %016b\n", 12345);
        break; case __LINE__: TRICE(ID(0), "--------------------------------------------------\n");
        break; case __LINE__: TRICE(ID(0), "sig:This ASSERT error is just a demo and no real error:\n");
        break; case __LINE__: TRICE(ID(0), "--------------------------------------------------\n");
      //break; case __LINE__: TRICE(ID(0), "ERR:error       message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE(ID(0), "WRN:warning     message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE(ID(0), "ATT:attention   message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE(ID(0), "DIA:diagnostics message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE(ID(0), "TIM:timing      message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE(ID(0), "DBG:debug       message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE(ID(0), "SIG:signal      message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE(ID(0), "RD:read         message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE(ID(0), "WR:write        message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE(ID(0), "ISR:interrupt   message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE(ID(0), "MSG:normal      message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE(ID(0), "INFO:informal   message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE(ID(0), "time:TRICE32_1  message, SysTick is %6u\n", SYSTICKVAL);

      //break; case __LINE__: TRICE(ID(0), "sig:Various ranges and formats\n" );
      //break; case __LINE__: TRICE8 (ID(0), "tst:TRICE8  %%03x -> %03x  %03x  %03x  %03x\n", 1, 0x7f, 0x80, 0xff);
      //break; case __LINE__: TRICE8 (ID(0), "tst:TRICE8   %%4d -> %4d %4d %4d %4d\n", 1, 0x7f, 0x80, 0xff);
      //break; case __LINE__: TRICE8 (ID(0), "tst:TRICE8   %%4u -> %4u %4u %4u %4u\n", 1, 0x7f, 0x80, 0xff);
      //break; case __LINE__: TRICE8 (ID(0), "tst:TRICE8   %%4o -> %4o %4o %4o %4o\n", 1, 0x7f, 0x80, 0xff);
      //break; case __LINE__: TRICE8 (ID(0), "tst:TRICE8   %%4O -> %4O %4O %4O %4O\n", 1, 0x7f, 0x80, 0xff);
      //break; case __LINE__: TRICE8 (ID(0), "tst:TRICE8   %%4X -> %4X %4X %4X %4X\n", 1, 0x7f, 0x80, 0xff);
      //break; case __LINE__: TRICE8 (ID(0), "tst:TRICE8   %%4x -> %4x %4x %4x %4x\n", 1, 0x7f, 0x80, 0xff);
      //break; case __LINE__: TRICE8 (ID(0), "tst:TRICE8   %%8b -> %8b %8b %8b %8b\n", 1, 0x7f, 0x80, 0xff);
      //break; case __LINE__: TRICE8 (ID(0), "tst:TRICE8  %%08b -> %08b %08b %08b %08b\n", 1, 0x7f, 0x80, 0xff);
      //break; case __LINE__: TRICE16(ID(0), "tst:TRICE16  %%05x ->   %05x   %05x   %05x   %05x\n", 1, 0x7fff, 0x8000, 0xffff);
      //break; case __LINE__: TRICE16(ID(0), "tst:TRICE16   %%6d ->  %6d  %6d  %6d  %6d\n",         1, 0x7fff, 0x8000, 0xffff);
      //break; case __LINE__: TRICE16(ID(0), "tst:TRICE16   %%7o -> %7o %7o %7o %7o\n",             1, 0x7fff, 0x8000, 0xffff);
      //break; case __LINE__: TRICE32(ID(0), "tst:TRICE32 %%09x ->      %09x      %09x       %09x     %09x\n", 1, 0x7fffffff, 0x80000000, 0xffffffff);
      //break; case __LINE__: TRICE32(ID(0), "tst:TRICE32 %%10d ->     %10d     %10d     %10d    %10x\n",      1, 0x7fffffff, 0x80000000, 0xffffffff);
      //break; case __LINE__: TRICE32(ID(0), "att:TRICE32 %40b\n", 0xAAAAAAAA);
      //break; case __LINE__: TRICE64(ID(0), "tst:TRICE64 %%09x ->      %09x      %09x       %09x     %09x\n", 1, 0x7fffffffffffffff, 0x8000000000000000, 0xffffffffffffffff);
      //break; case __LINE__: TRICE64(ID(0), "tst:TRICE64 %%10d ->     %10d     %10d     %10d    %10x\n",      1, 0x7fffffffffffffff, 0x8000000000000000, 0xffffffffffffffff);
      //break; case __LINE__: TRICE64(ID(0), "att:TRICE64 %70b\n", 0xAAAAAAAAAAAAAAAA);
      //break; case __LINE__: TRICE(ID(0), "sig:Legacy TRICE8\n" );
      //break; case __LINE__: TRICE8_1(ID(0), "tst:TRICE8_1 %02x\n", 0xA1);
      //break; case __LINE__: TRICE8_2(ID(0), "tst:TRICE8_2 %02x %02x\n", 0xA1, 0xA2);
      //break; case __LINE__: TRICE8_3(ID(0), "tst:TRICE8_3 %02x %02x %02x\n", 0xA1, 0xA2, 0xA3);
      //break; case __LINE__: TRICE8_4(ID(0), "tst:TRICE8_4 %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4);
      //break; case __LINE__: TRICE8_5(ID(0), "tst:TRICE8_5 %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5 );
      //break; case __LINE__: TRICE8_6(ID(0), "tst:TRICE8_6 %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6);
      //break; case __LINE__: TRICE8_7(ID(0), "tst:TRICE8_7 %02x %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7);
      //break; case __LINE__: TRICE8_8(ID(0), "tst:TRICE8_8 %02X %02X %02X %02X %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8); //exp: time:feed3322default: tst:TRICE8_8 A1 A2 A3 A4 a5 a6 a7 a8
      //break; case __LINE__: TRICE8_1(ID(0), "tst:TRICE8_1 %d\n",                      -1);
      //break; case __LINE__: TRICE8_2(ID(0), "tst:TRICE8_2 %d %d\n",                   -1, -2);
      //break; case __LINE__: TRICE8_3(ID(0), "tst:TRICE8_3 %d %d %d\n",                -1, -2, -3);
      //break; case __LINE__: TRICE8_4(ID(0), "tst:TRICE8_4 %d %d %d %d\n",             -1, -2, -3, -4);
      //break; case __LINE__: TRICE8_5(ID(0), "tst:TRICE8_5 %d %d %d %d %d\n",          -1, -2, -3, -4, -5);
      //break; case __LINE__: TRICE8_6(ID(0), "tst:TRICE8_6 %d %d %d %d %d %d\n",       -1, -2, -3, -4, -5, -6);
      //break; case __LINE__: TRICE8_7(ID(0), "tst:TRICE8_7 %d %d %d %d %d %d %d\n",    -1, -2, -3, -4, -5, -6, -7);
      //break; case __LINE__: TRICE8_8(ID(0), "tst:TRICE8_8 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8);
      //break; case __LINE__: TRICE8_1(ID(0), "tst:TRICE8_1 %d\n", 1);
      //break; case __LINE__: TRICE8_2(ID(0), "tst:TRICE8_2 %d %d\n", 1, 2);
      //break; case __LINE__: TRICE8_3(ID(0), "tst:TRICE8_3 %d %d %d\n", 1, 2, 3);
      //break; case __LINE__: TRICE8_4(ID(0), "tst:TRICE8_4 %d %d %d %d\n", 1, 2, 3, 4);
      //break; case __LINE__: TRICE8_5(ID(0), "tst:TRICE8_5 %d %d %d %d %d\n", 1, 2, 3, 4, 5);
      //break; case __LINE__: TRICE8_6(ID(0), "tst:TRICE8_6 %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6);
      //break; case __LINE__: TRICE8_7(ID(0), "tst:TRICE8_7 %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7);
      //break; case __LINE__: TRICE8_8(ID(0), "tst:TRICE8_8 %d %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7, 8);
      //break; case __LINE__: TRICE8_1(ID(0), "tst:TRICE8_1 %u\n", 201);
      //break; case __LINE__: TRICE8_2(ID(0), "tst:TRICE8_2 %u %u\n", 201, 202);
      //break; case __LINE__: TRICE8_3(ID(0), "tst:TRICE8_3 %u %u %u\n", 201, 202, 203);
      //break; case __LINE__: TRICE8_4(ID(0), "tst:TRICE8_4 %u %u %u %u\n", 201, 202, 203, 204);
      //break; case __LINE__: TRICE8_5(ID(0), "tst:TRICE8_5 %u %u %u %u %u\n", 201, 202, 203, 204, 205);
      //break; case __LINE__: TRICE8_6(ID(0), "tst:TRICE8_6 %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206);
      //break; case __LINE__: TRICE8_7(ID(0), "tst:TRICE8_7 %u %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206, 207);
      //break; case __LINE__: TRICE8_8(ID(0), "tst:TRICE8_8 %u %u %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206, 207, 208);
      //break; case __LINE__: TRICE8_8(ID(0), "tst:TRICE8_1 %%d=%d, %%u=%u, 0x%%x=0x%x, 0x%%2x=0x%2x, 0x%%02x=0x%02x, 0x%%3x=0x%3x, 0x%%03x=0x%03x, %%b=%b\n", 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81);

      //break; case __LINE__: TRICE    (ID(0), "sig:Legacy TRICE16-64\n" );
      //break; case __LINE__: TRICE16_1(ID(0), "tst:TRICE16_1 %d\n", -111);
      //break; case __LINE__: TRICE16_2(ID(0), "tst:TRICE16_2 %d %d\n", -111, -222);
      //break; case __LINE__: TRICE16_3(ID(0), "tst:TRICE16_3 %d %d %d\n", -111, -222, -333);
      //break; case __LINE__: TRICE16_4(ID(0), "tst:TRICE16_4 %d %d %d %d\n", -111, -222, -333, -444);
      //break; case __LINE__: TRICE32_1(ID(0), "tst:TRICE32_1 %08x\n", 0x0123cafe);
      //break; case __LINE__: TRICE32_1(ID(0), "tst:TRICE32_1 %d\n", -111);
      //break; case __LINE__: TRICE32_2(ID(0), "tst:TRICE32_2 %x %x\n", -111, -222);
      //break; case __LINE__: TRICE32_2(ID(0), "tst:TRICE32_2 %d %d\n", -111, -222);
      //break; case __LINE__: TRICE32_3(ID(0), "tst:TRICE32_3 %x %x %x\n", -111, -222, -333);
      //break; case __LINE__: TRICE32_3(ID(0), "tst:TRICE32_3 %d %d %d\n", -111, -222, -333);
      //break; case __LINE__: TRICE32_4(ID(0), "tst:TRICE32_4 %x %x %x %x\n", -111, -222, -333, -444);
      //break; case __LINE__: TRICE32_4(ID(0), "tst:TRICE32_4 %d %d %d %d\n", -111, -222, -333, -444);
      //break; case __LINE__: TRICE64_1(ID(0), "tst:TRICE64_1 %d\n", -111);
      //break; case __LINE__: TRICE64_2(ID(0), "tst:TRICE64_2 %d %d\n", -111, -222);
      //break; case __LINE__: TRICE16_1(ID(0), "tst:TRICE16_1 %u\n", 60001);
      //break; case __LINE__: TRICE16_2(ID(0), "tst:TRICE16_2 %u %u\n", 60001, 60002);
      //break; case __LINE__: TRICE16_3(ID(0), "tst:TRICE16_3 %u %u %u\n", 60001, 60002, 60003);
      //break; case __LINE__: TRICE16_4(ID(0), "tst:TRICE16_4 %u %u %u %u\n", 60001, 60002, 60003, 60004);
      //break; case __LINE__: TRICE32_1(ID(0), "tst:TRICE32_1 %u\n", 4000000001);
      //break; case __LINE__: TRICE32_2(ID(0), "tst:TRICE32_2 %u %u\n", 4000000001, 4000000002);
      //break; case __LINE__: TRICE32_3(ID(0), "tst:TRICE32_3 %u %u %u\n", 4000000001, 4000000002, 4000000003);
      //break; case __LINE__: TRICE32_4(ID(0), "tst:TRICE32_4 %u %u %u %u\n", 4000000001, 4000000002, 4000000003, 4000000004);
      //break; case __LINE__: TRICE64_1(ID(0), "tst:TRICE64_1 %x\n", -1);
      //break; case __LINE__: TRICE64_2(ID(0), "tst:TRICE64_2 %x %x\n", -1, -2);
      //break; case __LINE__: TRICE64_1(ID(0), "tst:TRICE64_1 %u\n", -1);
      //break; case __LINE__: TRICE64_2(ID(0), "tst:TRICE64_2 %u %u\n", -1, -2);

        break; case __LINE__: trice("sig:colored single letters and several TRICE macros in one line\n" );
        break; case __LINE__: trice("e:A");
        break; case __LINE__: trice("w:B");
        break; case __LINE__: trice("a:c");
        break; case __LINE__: trice("wr:d");
        break; case __LINE__: trice("rd:e\n");
        break; case __LINE__: trice("diag:f");
        break; case __LINE__: trice("d:G");
        break; case __LINE__: trice("t:H");
        break; case __LINE__: trice("time:i");
        break; case __LINE__: trice("message:J");
        break; case __LINE__: trice("dbg:k\n");
        break; case __LINE__: trice("1");
        break; case __LINE__: trice("2");
        break; case __LINE__: trice("3");
        break; case __LINE__: trice("4");
        break; case __LINE__: trice("e:7");
        break; case __LINE__: trice("m:12");
        break; case __LINE__: trice("m:123\n");
        break; case __LINE__: trice("e:A"); trice("w:B"); trice("a:c");
        break; case __LINE__: trice("wr:d"); trice("rd:e\n"); trice("diag:f");

      //reak; case __LINE__: trice("sig:TRICE8 with variable param count 1 to 12\n" );
      //reak; case __LINE__: TRICE8(ID(0), "tst:TRICE8 %d\n", -1 );
      //reak; case __LINE__: TRICE8(ID(0), "tst:TRICE8 %d %d\n", -1, -2 );
      //reak; case __LINE__: TRICE8(ID(0), "tst:TRICE8 %d %d %d\n", -1, -2, -3 );
      //reak; case __LINE__: TRICE8(ID(0), "tst:TRICE8 %d %d %d %d\n", -1, -2, -3, -4 );
      //reak; case __LINE__: TRICE8(ID(0), "tst:TRICE8 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
      //reak; case __LINE__: TRICE8(ID(0), "tst:TRICE8 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
      //reak; case __LINE__: TRICE8(ID(0), "tst:TRICE8 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
      //reak; case __LINE__: TRICE8(ID(0), "tst:TRICE8 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
      //reak; case __LINE__: TRICE8(ID(0), "tst:TRICE8 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
      //reak; case __LINE__: TRICE8(ID(0), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
      //reak; case __LINE__: TRICE8(ID(0), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
      //reak; case __LINE__: TRICE8(ID(0), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
      //reak; case __LINE__: TRICE8(ID(0), "tst:TRICE8 %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );
      //reak; case __LINE__: TRICE8_1 (ID(0), "tst:TRICE8_1  %d\n", -1 );
      //reak; case __LINE__: TRICE8_2 (ID(0), "tst:TRICE8_2  %d %d\n", -1, -2 );
      //reak; case __LINE__: TRICE8_3 (ID(0), "tst:TRICE8_3  %d %d %d\n", -1, -2, -3 );
      //reak; case __LINE__: TRICE8_4 (ID(0), "tst:TRICE8_4  %d %d %d %d\n", -1, -2, -3, -4 );
      //reak; case __LINE__: TRICE8_5 (ID(0), "tst:TRICE8_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
      //reak; case __LINE__: TRICE8_6 (ID(0), "tst:TRICE8_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
      //reak; case __LINE__: TRICE8_7 (ID(0), "tst:TRICE8_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
      //reak; case __LINE__: TRICE8_8 (ID(0), "tst:TRICE8_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
      //reak; case __LINE__: TRICE8_9 (ID(0), "tst:TRICE8_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
      //reak; case __LINE__: TRICE8_10(ID(0), "tst:TRICE8_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
      //reak; case __LINE__: TRICE8_11(ID(0), "tst:TRICE8_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
      //reak; case __LINE__: TRICE8_12(ID(0), "tst:TRICE8_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
      //reak; case __LINE__: TRICE8   (ID(0), "tst:TRICE8   %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );
      //reak; case __LINE__: TRICE8_9 (ID(0), "tst:TRICE8_9 %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );

      //reak; case __LINE__: trice("sig:TRICE16 with variable param count 1 to 12\n" );
      //reak; case __LINE__: TRICE16(ID(0), "tst:TRICE16 %d\n", -1 );
      //reak; case __LINE__: TRICE16(ID(0), "tst:TRICE16 %d %d\n", -1, -2 );
      //reak; case __LINE__: TRICE16(ID(0), "tst:TRICE16 %d %d %d\n", -1, -2, -3 );
      //reak; case __LINE__: TRICE16(ID(0), "tst:TRICE16 %d %d %d %d\n", -1, -2, -3, -4 );
      //reak; case __LINE__: TRICE16(ID(0), "tst:TRICE16 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
      //reak; case __LINE__: TRICE16(ID(0), "tst:TRICE16 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
      //reak; case __LINE__: TRICE16(ID(0), "tst:TRICE16 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
      //reak; case __LINE__: TRICE16(ID(0), "tst:TRICE16 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
      //reak; case __LINE__: TRICE16(ID(0), "tst:TRICE16 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
      //reak; case __LINE__: TRICE16(ID(0), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
      //reak; case __LINE__: TRICE16(ID(0), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
      //reak; case __LINE__: TRICE16(ID(0), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
      //reak; case __LINE__: TRICE16(ID(0), "tst:TRICE16 %t %b %x %X %d %u %o %O %p\n", 3, 3, 3, 3, 3, 3, 3, 3, 3 );
      //reak; case __LINE__: TRICE16_1 (ID(0), "tst:TRICE16_1  %d\n", -1 );
      //reak; case __LINE__: TRICE16_2 (ID(0), "tst:TRICE16_2  %d %d\n", -1, -2 );
      //reak; case __LINE__: TRICE16_3 (ID(0), "tst:TRICE16_3  %d %d %d\n", -1, -2, -3 );
      //reak; case __LINE__: TRICE16_4 (ID(0), "tst:TRICE16_4  %d %d %d %d\n", -1, -2, -3, -4 );
      //reak; case __LINE__: TRICE16_5 (ID(0), "tst:TRICE16_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
      //reak; case __LINE__: TRICE16_6 (ID(0), "tst:TRICE16_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
      //reak; case __LINE__: TRICE16_7 (ID(0), "tst:TRICE16_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
      //reak; case __LINE__: TRICE16_8 (ID(0), "tst:TRICE16_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
      //reak; case __LINE__: TRICE16_9 (ID(0), "tst:TRICE16_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
      //reak; case __LINE__: TRICE16_10(ID(0), "tst:TRICE16_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
      //reak; case __LINE__: TRICE16_11(ID(0), "tst:TRICE16_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
      //reak; case __LINE__: TRICE16_12(ID(0), "tst:TRICE16_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
      //reak; case __LINE__: TRICE16_9 (ID(0), "tst:TRICE16 %t %b %x %X %d %u %o %O %p\n", 3, 3, 3, 3, 3, 3, 3, 3, 3 );

      //break; case __LINE__: trice("sig:TRICE32 with variable param count 1 to 12\n" );
      //break; case __LINE__: TRICE32(ID(0), "tst:TRICE32 %d\n", -1 );
      //break; case __LINE__: TRICE32(ID(0), "tst:TRICE32 %d %d\n", -1, -2 );
      //break; case __LINE__: TRICE32(ID(0), "tst:TRICE32 %d %d %d\n", -1, -2, -3 );
      //break; case __LINE__: TRICE32(ID(0), "tst:TRICE32 %d %d %d %d\n", -1, -2, -3, -4 );
      //break; case __LINE__: TRICE32(ID(0), "tst:TRICE32 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
      //break; case __LINE__: TRICE32(ID(0), "tst:TRICE32 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
      //break; case __LINE__: TRICE32(ID(0), "tst:TRICE32 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
      //break; case __LINE__: TRICE32(ID(0), "tst:TRICE32 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
      //break; case __LINE__: TRICE32(ID(0), "tst:TRICE32 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
      //break; case __LINE__: TRICE32(ID(0), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
      //break; case __LINE__: TRICE32(ID(0), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
      //break; case __LINE__: TRICE32(ID(0), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
      //break; case __LINE__: TRICE32(ID(0), "tst:TRICE32 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));
      //break; case __LINE__: TRICE32_1 (ID(0), "tst:TRICE32_1  %d\n", -1 );
      //break; case __LINE__: TRICE32_2 (ID(0), "tst:TRICE32_2  %d %d\n", -1, -2 );
      //break; case __LINE__: TRICE32_3 (ID(0), "tst:TRICE32_3  %d %d %d\n", -1, -2, -3 );
      //break; case __LINE__: TRICE32_4 (ID(0), "tst:TRICE32_4  %d %d %d %d\n", -1, -2, -3, -4 );
      //break; case __LINE__: TRICE32_5 (ID(0), "tst:TRICE32_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
      //break; case __LINE__: TRICE32_6 (ID(0), "tst:TRICE32_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
      //break; case __LINE__: TRICE32_7 (ID(0), "tst:TRICE32_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
      //break; case __LINE__: TRICE32_8 (ID(0), "tst:TRICE32_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
      //break; case __LINE__: TRICE32_9 (ID(0), "tst:TRICE32_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
      //break; case __LINE__: TRICE32_10(ID(0), "tst:TRICE32_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
      //break; case __LINE__: TRICE32_11(ID(0), "tst:TRICE32_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
      //break; case __LINE__: TRICE32_12(ID(0), "tst:TRICE32_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
      //break; case __LINE__: TRICE32   (ID(0), "tst:TRICE32    %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));
      //break; case __LINE__: TRICE32_12(ID(0), "tst:TRICE32_12 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));

      //break; case __LINE__: trice("sig:TRICE64 with variable param count 1 to 12\n" );
      //break; case __LINE__: TRICE64(ID(0), "tst:TRICE64 %d\n", -1 );
      //break; case __LINE__: TRICE64(ID(0), "tst:TRICE64 %d %d\n", -1, -2 );
      //break; case __LINE__: TRICE64(ID(0), "tst:TRICE64 %d %d %d\n", -1, -2, -3 );
      //break; case __LINE__: TRICE64(ID(0), "tst:TRICE64 %d %d %d %d\n", -1, -2, -3, -4 );
      //break; case __LINE__: TRICE64(ID(0), "tst:TRICE64 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
      //break; case __LINE__: TRICE64(ID(0), "tst:TRICE64 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
      //break; case __LINE__: TRICE64(ID(0), "tst:TRICE64 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
      //break; case __LINE__: TRICE64(ID(0), "tst:TRICE64 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
      //break; case __LINE__: TRICE64(ID(0), "tst:TRICE64 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
      //break; case __LINE__: TRICE64(ID(0), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
      //break; case __LINE__: TRICE64(ID(0), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
      //break; case __LINE__: TRICE64(ID(0), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
      //break; case __LINE__: TRICE64(ID(0), "tst:TRICE64 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aDouble(3.14159), aDouble(3.14159), aDouble(3.14159));
      //break; case __LINE__: TRICE64_1 (ID(0), "tst:TRICE64_1  %d\n", -1 );
      //break; case __LINE__: TRICE64_2 (ID(0), "tst:TRICE64_2  %d %d\n", -1, -2 );
      //break; case __LINE__: TRICE64_3 (ID(0), "tst:TRICE64_3  %d %d %d\n", -1, -2, -3 );
      //break; case __LINE__: TRICE64_4 (ID(0), "tst:TRICE64_4  %d %d %d %d\n", -1, -2, -3, -4 );
      //break; case __LINE__: TRICE64_5 (ID(0), "tst:TRICE64_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
      //break; case __LINE__: TRICE64_6 (ID(0), "tst:TRICE64_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
      //break; case __LINE__: TRICE64_7 (ID(0), "tst:TRICE64_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
      //break; case __LINE__: TRICE64_8 (ID(0), "tst:TRICE64_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
      //break; case __LINE__: TRICE64_9 (ID(0), "tst:TRICE64_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
      //break; case __LINE__: TRICE64_10(ID(0), "tst:TRICE64_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
      //break; case __LINE__: TRICE64_11(ID(0), "tst:TRICE64_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
      //break; case __LINE__: TRICE64_12(ID(0), "tst:TRICE64_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
      //break; case __LINE__: TRICE64_12(ID(0), "tst:TRICE64_12 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aDouble(3.14159), aDouble(3.14159), aDouble(3.14159));

        break; case __LINE__: trice("att:positive and negative float in format variants\n");
        break; case __LINE__: trice("rd:TRICE float %f (%%f)\n", aFloat(x) );
        break; case __LINE__: trice("rd:TRICE float %9f (%%9f)\n", aFloat(x) );
        break; case __LINE__: trice("rd:TRICE float %.9f (%%.9f)\n", aFloat(x) );
        break; case __LINE__: trice("rd:TRICE float %9.f (%%9.f)\n", aFloat(x) );
        break; case __LINE__: trice("rd:TRICE float %9.6f (%%9.6f)\n", aFloat(x) );
        break; case __LINE__: trice("rd:TRICE float %f (%%f)\n", aFloat(-x) );
        break; case __LINE__: trice("rd:TRICE float %9f (%%9f)\n", aFloat(-x) );
        break; case __LINE__: trice("rd:TRICE float %.9f (%%.9f)\n", aFloat(-x) );
        break; case __LINE__: trice("rd:TRICE float %9.f (%%9.f)\n", aFloat(-x) );
        break; case __LINE__: trice("rd:TRICE float %9.6f (%%9.6f)\n", aFloat(-x) );
        break; case __LINE__: trice("rd:TRICE float %+f (%%f)\n", aFloat(x) );
        break; case __LINE__: trice("rd:TRICE float %+9f (%%9f)\n", aFloat(x) );
        break; case __LINE__: trice("rd:TRICE float %+.9f (%%.9f)\n", aFloat(x) );
        break; case __LINE__: trice("rd:TRICE float %+9.f (%%9.f)\n", aFloat(x) );
        break; case __LINE__: trice("rd:TRICE float %+9.6f (%%9.6f)\n", aFloat(x) );
        break; case __LINE__: trice("rd:TRICE float %+f (%%f)\n", aFloat(-x) );
        break; case __LINE__: trice("rd:TRICE float %+9f (%%9f)\n", aFloat(-x) );
        break; case __LINE__: trice("rd:TRICE float %+.9f (%%.9f)\n", aFloat(-x) );
        break; case __LINE__: trice("rd:TRICE float %+9.f (%%9.f)\n", aFloat(-x) );
        break; case __LINE__: trice("rd:TRICE float %+9.6f (%%9.6f)\n", aFloat(-x) );
        break; case __LINE__: trice("rd:TRICE float %-f (%%f)\n", aFloat(x) );
        break; case __LINE__: trice("rd:TRICE float %-9f (%%9f)\n", aFloat(x) );
        break; case __LINE__: trice("rd:TRICE float %-.9f (%%.9f)\n", aFloat(x) );
        break; case __LINE__: trice("rd:TRICE float %-9.f (%%9.f)\n", aFloat(x) );
        break; case __LINE__: trice("rd:TRICE float %-9.6f (%%9.6f)\n", aFloat(x) );
        break; case __LINE__: trice("rd:TRICE float %-f (%%f)\n", aFloat(-x) );
        break; case __LINE__: trice("rd:TRICE float %-9f (%%9f)\n", aFloat(-x) );
        break; case __LINE__: trice("rd:TRICE float %-.9f (%%.9f)\n", aFloat(-x) );
        break; case __LINE__: trice("rd:TRICE float %-9.f (%%9.f)\n", aFloat(-x) );
        break; case __LINE__: trice("rd:TRICE float %-9.6f (%%9.6f)\n", aFloat(-x) );
        break; case __LINE__: trice("att:positive float & double in variants\n");
      //break; case __LINE__: TRICE32_1(ID(0), "rd:TRICE32_1 float %e (%%e)\n", aFloat(x) );
      //break; case __LINE__: TRICE32_1(ID(0), "rd:TRICE32_1 float %f (%%f)\n", aFloat(x) );
      //break; case __LINE__: TRICE32_1(ID(0), "rd:TRICE32_1 float %g (%%g)\n", aFloat(x) );
      //break; case __LINE__: TRICE32_1(ID(0), "rd:TRICE32_1 float %E (%%E)\n", aFloat(x) );
      //break; case __LINE__: TRICE32_1(ID(0), "rd:TRICE32_1 float %F (%%F)\n", aFloat(x) );
      //break; case __LINE__: TRICE32_1(ID(0), "rd:TRICE32_1 float %G (%%G)\n", aFloat(x) );
      //break; case __LINE__: TRICE32  (ID(0), "rd:TRICE32 float %e (%%e)\n", aFloat(x) );
      //break; case __LINE__: TRICE32  (ID(0), "rd:TRICE32 float %f (%%f)\n", aFloat(x) );
      //break; case __LINE__: TRICE32  (ID(0), "rd:TRICE32 float %g (%%g)\n", aFloat(x) );
      //break; case __LINE__: TRICE32  (ID(0), "rd:TRICE32 float %E (%%E)\n", aFloat(x) );
      //break; case __LINE__: TRICE32  (ID(0), "rd:TRICE32 float %F (%%F)\n", aFloat(x) );
      //break; case __LINE__: TRICE32  (ID(0), "rd:TRICE32 float %G (%%G)\n", aFloat(x) );
      //break; case __LINE__: TRICE    (ID(0), "rd:TRICE float %e (%%e)\n", aFloat(x) );
      //break; case __LINE__: TRICE    (ID(0), "rd:TRICE float %f (%%f)\n", aFloat(x) );
      //break; case __LINE__: TRICE    (ID(0), "rd:TRICE float %g (%%g)\n", aFloat(x) );
      //break; case __LINE__: TRICE    (ID(0), "rd:TRICE float %E (%%E)\n", aFloat(x) );
      //break; case __LINE__: TRICE    (ID(0), "rd:TRICE float %F (%%F)\n", aFloat(x) );
      //break; case __LINE__: TRICE    (ID(0), "rd:TRICE float %G (%%G)\n", aFloat(x) );
      //break; case __LINE__: TRICE64  (ID(0), "rd:TRICE64 double %e (%%e), aDouble(y)\n", aDouble(y) );
      //break; case __LINE__: TRICE64  (ID(0), "rd:TRICE64 double %f (%%f), aDouble(y)\n", aDouble(y) );
      //break; case __LINE__: TRICE64  (ID(0), "rd:TRICE64 double %g (%%g), aDouble(y)\n", aDouble(y) );
      //break; case __LINE__: TRICE64  (ID(0), "rd:TRICE64 double %E (%%E), aDouble(y)\n", aDouble(y) );
      //break; case __LINE__: TRICE64  (ID(0), "rd:TRICE64 double %F (%%F), aDouble(y)\n", aDouble(y) );
      //break; case __LINE__: TRICE64  (ID(0), "rd:TRICE64 double %G (%%G), aDouble(y)\n", aDouble(y) );
      //break; case __LINE__: TRICE64_1(ID(0), "rd:TRICE64_1 double %e (%%e), aDouble(y)\n", aDouble(y) );
      //break; case __LINE__: TRICE64_1(ID(0), "rd:TRICE64_1 double %f (%%f), aDouble(y)\n", aDouble(y) );
      //break; case __LINE__: TRICE64_1(ID(0), "rd:TRICE64_1 double %g (%%g), aDouble(y)\n", aDouble(y) );
      //break; case __LINE__: TRICE64_1(ID(0), "rd:TRICE64_1 double %E (%%E), aDouble(y)\n", aDouble(y) );
      //break; case __LINE__: TRICE64_1(ID(0), "rd:TRICE64_1 double %F (%%F), aDouble(y)\n", aDouble(y) );
      //break; case __LINE__: TRICE64_1(ID(0), "rd:TRICE64_1 double %G (%%G), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: trice("att:negative float & double\n" );
        break; case __LINE__: trice("rd:TRICE float %e (%%e)\n", aFloat(-x) );
        break; case __LINE__: trice("rd:TRICE float %f (%%f)\n", aFloat(-x) );
        break; case __LINE__: trice("rd:TRICE float %g (%%g)\n", aFloat(-x) );
        break; case __LINE__: trice("rd:TRICE float %E (%%E)\n", aFloat(-x) );
        break; case __LINE__: trice("rd:TRICE float %F (%%F)\n", aFloat(-x) );
        break; case __LINE__: trice("rd:TRICE float %G (%%G)\n", aFloat(-x) );
      //break; case __LINE__: TRICE64(ID(0), "rd:TRICE64 double %e (%%e), aDouble(y)\n", aDouble(-y) );
      //break; case __LINE__: TRICE64(ID(0), "rd:TRICE64 double %f (%%f), aDouble(y)\n", aDouble(-y) );
      //break; case __LINE__: TRICE64(ID(0), "rd:TRICE64 double %g (%%g), aDouble(y)\n", aDouble(-y) );
      //break; case __LINE__: TRICE64(ID(0), "rd:TRICE64 double %E (%%E), aDouble(y)\n", aDouble(-y) );
      //break; case __LINE__: TRICE64(ID(0), "rd:TRICE64 double %F (%%F), aDouble(y)\n", aDouble(-y) );
      //break; case __LINE__: TRICE64(ID(0), "rd:TRICE64 double %G (%%G), aDouble(y)\n", aDouble(-y) );

      //break; case __LINE__: TRICE  (ID(0), "att:formatted float & double\n" );
      //break; case __LINE__: TRICE32(ID(0), "rd:TRICE32 float %12.6e (%%12.6e)\n", aFloat(x) );
      //break; case __LINE__: TRICE32(ID(0), "rd:TRICE32 float %12.6f (%%12.6f)\n", aFloat(x) );
      //break; case __LINE__: TRICE32(ID(0), "rd:TRICE32 float %12.6g (%%12.6g)\n", aFloat(x) );
      //break; case __LINE__: TRICE32(ID(0), "rd:TRICE32 float %12.6E (%%12.6E)\n", aFloat(x) );
      //break; case __LINE__: TRICE32(ID(0), "rd:TRICE32 float %12.6F (%%12.6F)\n", aFloat(x) );
      //break; case __LINE__: TRICE32(ID(0), "rd:TRICE32 float %12.6G (%%12.6G)\n", aFloat(x) );
      //break; case __LINE__: TRICE64(ID(0), "rd:TRICE64 double %12.6e (%%12.6e), aDouble(y)\n", aDouble(y) );
      //break; case __LINE__: TRICE64(ID(0), "rd:TRICE64 double %12.6f (%%12.6f), aDouble(y)\n", aDouble(y) );
      //break; case __LINE__: TRICE64(ID(0), "rd:TRICE64 double %12.6g (%%12.6g), aDouble(y)\n", aDouble(y) );
      //break; case __LINE__: TRICE64(ID(0), "rd:TRICE64 double %12.6E (%%12.6E), aDouble(y)\n", aDouble(y) );
      //break; case __LINE__: TRICE64(ID(0), "rd:TRICE64 double %12.6F (%%12.6F), aDouble(y)\n", aDouble(y) );
      //break; case __LINE__: TRICE64(ID(0), "rd:TRICE64 double %12.6G (%%12.6G), aDouble(y)\n", aDouble(y) );

      //break; case __LINE__: TRICE (ID(0), "att:Various single arguments\n" );
      //break; case __LINE__: TRICE8(ID(0), "rd:TRICE8 line %t (%%t ,0)\n", 0 );
      //break; case __LINE__: TRICE8(ID(0), "rd:TRICE8 line %t (%%t ,2)\n", 2 );
      //break; case __LINE__: TRICE8(ID(0), "rd:TRICE8 line %u (%%u ,-1)\n", -1 );
      //break; case __LINE__: TRICE8(ID(0), "rd:TRICE8 line %b (%%b ,-1)\n", -1 );
      //break; case __LINE__: TRICE8(ID(0), "rd:TRICE8 line %o (%%o ,-1)\n", -1 );
      //break; case __LINE__: TRICE8(ID(0), "rd:TRICE8 line %O (%%O ,-1)\n", -1 );
      //break; case __LINE__: TRICE8(ID(0), "rd:TRICE8 line %X (%%X ,-1)\n", -1 );
      //break; case __LINE__: TRICE8(ID(0), "rd:TRICE8 line %x (%%x ,-1)\n", -1 );
      //break; case __LINE__: TRICE8(ID(0), "rd:TRICE8 line %d (%%d ,-1)\n", -1 );
      //break; case __LINE__: TRICE16_1(ID(0), "rd:TRICE16_1 line %t (%%t ,0)\n", 0 );
      //break; case __LINE__: TRICE16_1(ID(0), "rd:TRICE16_1 line %t (%%t ,2)\n", 2 );
      //break; case __LINE__: TRICE16_1(ID(0), "rd:TRICE16_1 line %u (%%u -1)\n", -1 );
      //break; case __LINE__: TRICE16_1(ID(0), "rd:TRICE16_1 line %b (%%b -1)\n", -1 );
      //break; case __LINE__: TRICE16_1(ID(0), "rd:TRICE16_1 line %o (%%o -1)\n", -1 );
      //break; case __LINE__: TRICE16_1(ID(0), "rd:TRICE16_1 line %x (%%x -1)\n", -1 );
      //break; case __LINE__: TRICE16_1(ID(0), "rd:TRICE16_1 line %d (%%d -1)\n", -1 );
      //break; case __LINE__: trice("rd:TRICE line %t (%%t -1)\n", 0 );
      //break; case __LINE__: trice("rd:TRICE line %t (%%t -1)\n", 2 );
      //break; case __LINE__: trice("rd:TRICE line %u (%%u)\n", -1 );
      //break; case __LINE__: trice("rd:TRICE line %b (%%b)\n", -2 );
      //break; case __LINE__: trice("rd:TRICE line %o (%%o)\n", -3 );
      //break; case __LINE__: trice("rd:TRICE line %x (%%x)\n", -4 );
      //break; case __LINE__: trice("rd:TRICE line %d (%%d)\n", -5 );

      //break; case __LINE__: TRICE_N(ID(0), "msg:%s\n", A, 0 );
      //break; case __LINE__: TRICE_N(ID(0), "msg:%s\n", A, 1 );
      //break; case __LINE__: TRICE_N(ID(0), "msg:%s\n", A, 2 );
      //break; case __LINE__: TRICE_N(ID(0), "msg:%s\n", A, 3 );
      //break; case __LINE__: TRICE_N(ID(0), "msg:%s\n", A, 4 );
      //break; case __LINE__: TRICE_N(ID(0), "msg:%s\n", A, 5 );
      //break; case __LINE__: TRICE_N(ID(0), "msg:%s\n", A, 6 );
      //break; case __LINE__: TRICE_N(ID(0), "msg:%s\n", A, 7 );
      //break; case __LINE__: TRICE_N(ID(0), "msg:%s\n", A, 8 );
      //break; case __LINE__: TRICE_N(ID(0), "msg:%s\n", A, 9 );
      //break; case __LINE__: TRICE_N(ID(0), "msg:%s\n", A, 10 );
      //break; case __LINE__: TRICE_N(ID(0), "msg:%s\n", A, 11 );
      //break; case __LINE__: TRICE_N(ID(0), "msg:%s\n", A, 12 );
      //break; case __LINE__: TRICE_N(ID(0), "msg:%s\n", A, 13 );
      //break; case __LINE__: TRICE_N(ID(0), "msg:%s\n", A, 14 );
      //break; case __LINE__: TRICE_N(ID(0), "msg:%s\n", A, 15 );
      //break; case __LINE__: TRICE_N(ID(0), "msg:%s\n", A, 16 );
      //break; case __LINE__: TRICE_N(ID(0), "msg:%s\n", A, 17 );
      //break; case __LINE__: TRICE_N(ID(0), "msg:%s\n", A, 18 );
      //break; case __LINE__: TRICE_N(ID(0), "msg:%s\n", A, 19 );
      //break; case __LINE__: TRICE_N(ID(0), "msg:%s\n", A, 120 );
      //break; case __LINE__: TRICE_N(ID(0), "msg:%s\n", A, 121 );
      //break; case __LINE__: TRICE_N(ID(0), "msg:%s\n", A, 122 );
      //break; case __LINE__: TRICE_N(ID(0), "msg:%s\n", A, 123 );
      //break; case __LINE__: TRICE_N(ID(0), "msg:%s\n", A, 124 );
      //break; case __LINE__: TRICE_N(ID(0), "msg:%s\n", A, 125 );
      //break; case __LINE__: TRICE_N(ID(0), "msg:%s\n", A, 126 );
      //break; case __LINE__: TRICE_N(ID(0), "msg:%s\n", A, 127 );
      //break; case __LINE__: TRICE_N(ID(0), "msg:%s\n", A, 128 );
      //break; case __LINE__: TRICE_N(ID(0), "msg:%s\n", A, 129 );

        break; case __LINE__: triceN("msg:%s\n", A, 0 );
        break; case __LINE__: triceN("msg:%s\n", A, 1 );
        break; case __LINE__: triceN("msg:%s\n", A, 2 );
        break; case __LINE__: triceN("msg:%s\n", A, 3 );
        break; case __LINE__: triceN("msg:%s\n", A, 4 );
        break; case __LINE__: triceN("msg:%s\n", A, 5 );
        break; case __LINE__: triceN("msg:%s\n", A, 6 );
        break; case __LINE__: triceN("msg:%s\n", A, 7 );
        break; case __LINE__: triceN("msg:%s\n", A, 8 );
        break; case __LINE__: triceN("msg:%s\n", A, 9 );
        break; case __LINE__: triceN("msg:%s\n", A, 10 );
        break; case __LINE__: triceN("msg:%s\n", A, 11 );
        break; case __LINE__: triceN("msg:%s\n", A, 12 );
        break; case __LINE__: triceN("msg:%s\n", A, 13 );
        break; case __LINE__: triceN("msg:%s\n", A, 14 );
        break; case __LINE__: triceN("msg:%s\n", A, 15 );
        break; case __LINE__: triceN("msg:%s\n", A, 16 );
        break; case __LINE__: triceN("msg:%s\n", A, 17 );
        break; case __LINE__: triceN("msg:%s\n", A, 18 );
        break; case __LINE__: triceN("msg:%s\n", A, 19 );
        break; case __LINE__: triceN("msg:%s\n", A, 120 );
        break; case __LINE__: triceN("msg:%s\n", A, 121 );
        break; case __LINE__: triceN("msg:%s\n", A, 122 );
        break; case __LINE__: triceN("msg:%s\n", A, 123 );
        break; case __LINE__: triceN("msg:%s\n", A, 124 );
        break; case __LINE__: triceN("msg:%s\n", A, 125 );
        break; case __LINE__: triceN("msg:%s\n", A, 126 );
        break; case __LINE__: triceN("msg:%s\n", A, 127 );
        break; case __LINE__: triceN("msg:%s\n", A, 128 );
        break; case __LINE__: triceN("msg:%s\n", A, 129 );


        break; case __LINE__: trice("sig:TRICE with 1 to 12 values\n" );

        break; case __LINE__: trice("rd:TRICE %d\n", -1 );                                                                                              //exp: time:        default: rd:TRICE -1     
        break; case __LINE__: trice("rd:TRICE %d, %d\n", -1, -2 );                                                                                      //exp: time:        default: rd:TRICE -1, -2 
        break; case __LINE__: trice("rd:TRICE %d, %d, %d\n", -1, -2, -3 );                                                                              //exp: time:        default: rd:TRICE -1, -2, -3       
        break; case __LINE__: trice("rd:TRICE %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                      //exp: time:        default: rd:TRICE -1, -2, -3, -4      
        break; case __LINE__: trice("rd:TRICE %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                              //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5        
        break; case __LINE__: trice("rd:TRICE %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                      //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice("rd:TRICE %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                               //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice("rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                      //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice("rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                              //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice("rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                      //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice("rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );            //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice("rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );   //exp: time:        default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: trice("sig:TRICE_n with 1 to 12 values\n" );		
        break; case __LINE__: trice("rd:TRICE_1  %d\n", -1 );                                                                                            //exp: time:        default: rd:TRICE_1  -1     
        break; case __LINE__: trice("rd:TRICE_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:        default: rd:TRICE_2  -1, -2 
        break; case __LINE__: trice("rd:TRICE_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:        default: rd:TRICE_3  -1, -2, -3       
        break; case __LINE__: trice("rd:TRICE_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:        default: rd:TRICE_4  -1, -2, -3, -4      
        break; case __LINE__: trice("rd:TRICE_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:        default: rd:TRICE_5  -1, -2, -3, -4, -5        
        break; case __LINE__: trice("rd:TRICE_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:        default: rd:TRICE_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice("rd:TRICE_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:        default: rd:TRICE_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice("rd:TRICE_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:        default: rd:TRICE_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice("rd:TRICE_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:        default: rd:TRICE_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice("rd:TRICE_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:        default: rd:TRICE_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice("rd:TRICE_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:        default: rd:TRICE_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice("rd:TRICE_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:        default: rd:TRICE_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    


      //break; case __LINE__: TRICE  (ID(0), "sig:TRICE16 with 1 to 12 pointer\n" );
      //break; case __LINE__: TRICE16(ID(0), "rd:TRICE16 %p\n", -1 );
      //break; case __LINE__: TRICE16(ID(0), "rd:TRICE16 %p, %p\n", -1, -2 );
      //break; case __LINE__: TRICE16(ID(0), "rd:TRICE16 %p, %p, %p\n", -1, -2, -3 );
      //break; case __LINE__: TRICE16(ID(0), "rd:TRICE16 %p, %p, %p, %p\n", -1, -2, -3, -4 );
      //break; case __LINE__: TRICE16(ID(0), "rd:TRICE16 %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5 );
      //break; case __LINE__: TRICE16(ID(0), "rd:TRICE16 %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6 );
      //break; case __LINE__: TRICE16(ID(0), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7);
      //break; case __LINE__: TRICE16(ID(0), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8 );
      //break; case __LINE__: TRICE16(ID(0), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
      //break; case __LINE__: TRICE16(ID(0), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
      //break; case __LINE__: TRICE16(ID(0), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
      //break; case __LINE__: TRICE16(ID(0), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

      //break; case __LINE__: TRICE  (ID(0), "sig:TRICE16 with 1 to 12 hex\n" );
      //break; case __LINE__: TRICE16(ID(0), "rd:TRICE16 %X\n", -1 );
      //break; case __LINE__: TRICE16(ID(0), "rd:TRICE16 %X, %X\n", -1, -2 );
      //break; case __LINE__: TRICE16(ID(0), "rd:TRICE16 %X, %X, %X\n", -1, -2, -3 );
      //break; case __LINE__: TRICE16(ID(0), "rd:TRICE16 %X, %X, %X, %X\n", -1, -2, -3, -4 );
      //break; case __LINE__: TRICE16(ID(0), "rd:TRICE16 %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5 );
      //break; case __LINE__: TRICE16(ID(0), "rd:TRICE16 %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6 );
      //break; case __LINE__: TRICE16(ID(0), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7);
      //break; case __LINE__: TRICE16(ID(0), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8 );
      //break; case __LINE__: TRICE16(ID(0), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
      //break; case __LINE__: TRICE16(ID(0), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
      //break; case __LINE__: TRICE16(ID(0), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
      //break; case __LINE__: TRICE16(ID(0), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE(ID(0), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE(id(0), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE(Id(0), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE(ID(0), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: trice("w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: Trice("w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRice("w: Hello! 👋🙂 \a\n" ); // with sound!

        break; case __LINE__: TRICE_0(ID(0), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE_0(id(0), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE_0(Id(0), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE_0(ID(0), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: trice_0("w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: Trice_0("w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRice_0("w: Hello! 👋🙂 \a\n" ); // with sound!

        break; case __LINE__: TRICE0(ID(0), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE0(id(0), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE0(Id(0), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE0(ID(0), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: trice0("w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: Trice0("w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRice0("w: Hello! 👋🙂 \a\n" ); // with sound!

        break; case __LINE__: TRICE_0(ID(0), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE_0(id(0), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE_0(Id(0), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE_0(ID(0), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: trice_0("w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: Trice_0("w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRice_0("w: Hello! 👋🙂 \a\n" ); // with sound!

        break; case __LINE__: TRICE8(ID(0), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE8(id(0), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE8(Id(0), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE8(ID(0), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: trice8("w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: Trice8("w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRice8("w: Hello! 👋🙂 \a\n" ); // with sound!

        break; case __LINE__: TRICE8_0(ID(0), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE8_0(id(0), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE8_0(Id(0), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE8_0(ID(0), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: trice8_0("w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: Trice8_0("w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRice8_0("w: Hello! 👋🙂 \a\n" ); // with sound!

        break; case __LINE__: trice16("w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: trice16("w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: trice16("w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: trice16("w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: trice16("w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: Trice16("w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRice16("w: Hello! 👋🙂 \a\n" ); // with sound!

        break; case __LINE__: TRICE16_0(ID(0), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE16_0(id(0), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE16_0(Id(0), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE16_0(ID(0), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: trice16_0("w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: Trice16_0("w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRice16_0("w: Hello! 👋🙂 \a\n" ); // with sound!

        break; case __LINE__: TRICE32(ID(0), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE32(id(0), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE32(Id(0), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE32(ID(0), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: trice32("w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: Trice32("w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRice32("w: Hello! 👋🙂 \a\n" ); // with sound!

        break; case __LINE__: TRICE32_0(ID(0), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE32_0(id(0), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE32_0(Id(0), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE32_0(ID(0), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: trice32_0("w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: Trice32_0("w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRice32_0("w: Hello! 👋🙂 \a\n" ); // with sound!

        break; case __LINE__: TRICE64(ID(0), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE64(id(0), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE64(Id(0), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE64(ID(0), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: trice64("w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: Trice64("w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRice64("w: Hello! 👋🙂 \a\n" ); // with sound!

        break; case __LINE__: TRICE64_0(ID(0), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE64_0(id(0), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE64_0(Id(0), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE64_0(ID(0), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: trice64_0("w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: Trice64_0("w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRice64_0("w: Hello! 👋🙂 \a\n" ); // with sound!

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
#define SCOPY(element)                           \
	do {                                         \
		char* n = #element;                      \
		int size = sizeof(src->element);         \
		memcpy(p, &(src->element), size);        \
		p += size;                               \
		TRICE_S(ID(0), "rd:sizeof(%8s)", n); \
		TRICE(ID(0), " = %d\n", size);       \
	} while (0);

//! DCOPY is a helper macro for struct deserialization.
#define DCOPY(element)                           \
	do {                                         \
		char* n = #element;                      \
		int size = sizeof(dst->element);         \
		memcpy(&(dst->element), p, size);        \
		p += size;                               \
		TRICE_S(ID(0), "rd:sizeof(%8s)", n); \
		TRICE(ID(0), " = %d\n", size);       \
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
#endif
	TRICE(Id(0), "inf: Tryout tx struct:");
	TRICE8_B(Id(0), " %02x ", &tx, sizeof(tx));
	TRICE(Id(0), "\n");

	TRICE(Id(0), "inf: Tryout buffer:");
	TRICE8_B(Id(0), " %02x ", dst, len); // lint !e670
	TRICE(Id(0), "\n");

#if !TRICE_OFF
	char* src = dst; // "copy" - assume, data transferred now ("data transfer")
	len = deserializeTryout(&rx, src);
#endif
	TRICE(Id(0), "inf: Tryout rx struct:");
	TRICE8_B(Id(0), " %02x ", &rx, sizeof(rx));
	TRICE(Id(0), "\n");

	TRICE(Id(0), "inf:sizeOf(Trypout) = %d, buffer length = %d\n", sizeof(tx), len);
	TRICE8_F(Id(0), "info:TryoutStructFunction", &tx, sizeof(tx));
	TRICE8_F(Id(0), "info:TryoutBufferFunction", dst, len); // lint !e670
}

static void exampleOfManualJSONencoding(void) {
#if !TRICE_OFF
	typedef struct {
		int Apple, Birn;
		float Fish;
	} Ex_t;
	Ex_t Ex = {-1, 2, (float)2.781};
#endif
	Trice("att:MyStructEvaluationFunction(json:ExA{Apple:%d, Birn:%u, Fisch:%f}\n", Ex.Apple, Ex.Birn, aFloat(Ex.Fish));
}

static void dynString(int n) {
	char* s = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.,0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.,0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.,0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.,0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.,";
	const size_t l = strlen(s);
	n = n < l ? n : l;
	// trice("sig:%3d:", n ); - this gets overwritten in CGO_Test case, so we avoid it to keep testing simple.
	TRICE_N(id(0), "wr:%s\n", s, n);
}

#endif // #ifndef TRICE_CHECK_MIN

#endif // #if !TRICE_OFF
