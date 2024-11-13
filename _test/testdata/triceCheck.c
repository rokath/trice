/*! \file triceCheck.c
\brief trices for tool evaluation
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/
#if 1 // !TRICE_OFF - We do not exclude this file globally for TRICE_OFF == 1, to check if TRICE_OFF works well.
#include <float.h>
#include "trice.h"

// The strings behind "//exp:" are the expected result for each line (-color=none)

#if !TRICE_OFF

static int32_t FloatToInt32(float f);
static int64_t DoubleToInt64(double f);

#endif

static void exampleOfManualSerialization(void);
static void exampleOfManualJSONencoding(void);
static void dynString(int n);

// clang-format off

//! TriceCheck performs trice code sequence n. TriceCheck writes out all types of trices with fixed values for testing
//! \details One trice has one subtrace, if param size max 2 bytes. 
//! Traces with more bytes as parameter consist of several subtraces.
//! Each trice line needs to have a commented // "string" with its expected output.
//! The ID values must be in file triceCheck.c, because it is compiled first and trice update runs later.
// This function is also called from Go for tests.
void TriceCheck(int n) {
    char* sABCDE = "abcde 12345";
#if !TRICE_OFF
    uint32_t lenABCDE = strlen(sABCDE);
   float  x = (float)1089.6082763671875; // 0x44883377
    double y = 518.0547492508867; // 0x4080307020601050  
    #if TRICE_CGO == 1
    char* A = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
    #endif // #if TRICE_CGO == 1
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
        break; case __LINE__: trice16(iD(14000), "att: line %u\n", __LINE__ );
        // Lines ending with a comment starting with `//exp:` are executed during trhe automatic Go tests.
        break; case __LINE__: TRice(iD(14001), "Hello World!\n" );                                              //exp: time:feed3322default: Hello World!		
        // normal use cases
        break; case __LINE__: TRice(iD(14002), "info:This is a message without values and a 32-bit stamp.\n" ); //exp: time:feed3322default: info:This is a message without values and a 32-bit stamp.
        break; case __LINE__: Trice(iD(14003), "info:This is a message without values and a 16-bit stamp.\n" ); //exp: time:    be16default: info:This is a message without values and a 16-bit stamp.
        break; case __LINE__: trice(iD(14004), "info:This is a message without values and without stamp.\n"  ); //exp: time:            default: info:This is a message without values and without stamp.
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

        break; case __LINE__: trice(iD(14005), "sig:trice without stamp and with 0 to 12 values (most common use cases)\n" );    
        break; case __LINE__: trice(iD(14006), "rd:trice\n" );                                                                                                   //exp: time:            default: rd:trice
        break; case __LINE__: trice(iD(14007), "rd:trice %d\n", -1 );                                                                                            //exp: time:            default: rd:trice -1     
        break; case __LINE__: trice(iD(14008), "rd:trice %d, %d\n", -1, -2 );                                                                                    //exp: time:            default: rd:trice -1, -2 
        break; case __LINE__: trice(iD(14009), "rd:trice %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: rd:trice -1, -2, -3       
        break; case __LINE__: trice(iD(14010), "rd:trice %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:            default: rd:trice -1, -2, -3, -4      
        break; case __LINE__: trice(iD(14011), "rd:trice %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:            default: rd:trice -1, -2, -3, -4, -5        
        break; case __LINE__: trice(iD(14012), "rd:trice %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: rd:trice -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice(iD(14013), "rd:trice %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:            default: rd:trice -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice(iD(14014), "rd:trice %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:            default: rd:trice -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice(iD(14015), "rd:trice %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: rd:trice -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice(iD(14016), "rd:trice %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:            default: rd:trice -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice(iD(14017), "rd:trice %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:            default: rd:trice -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice(iD(14018), "rd:trice %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: rd:trice -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: Trice(iD(14019), "sig:Trice with 16-bit stamp and with 0 to 12 values (common use cases)\n" );     
        break; case __LINE__: Trice(iD(14020), "rd:Trice\n" );                                                                                                   //exp: time:    be16default: rd:Trice
        break; case __LINE__: Trice(iD(14021), "rd:Trice %d\n", -1 );                                                                                            //exp: time:    be16default: rd:Trice -1     
        break; case __LINE__: Trice(iD(14022), "rd:Trice %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:Trice -1, -2 
        break; case __LINE__: Trice(iD(14023), "rd:Trice %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:Trice -1, -2, -3       
        break; case __LINE__: Trice(iD(14024), "rd:Trice %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:Trice -1, -2, -3, -4      
        break; case __LINE__: Trice(iD(14025), "rd:Trice %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:Trice -1, -2, -3, -4, -5        
        break; case __LINE__: Trice(iD(14026), "rd:Trice %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:Trice -1, -2, -3, -4, -5, -6        
        break; case __LINE__: Trice(iD(14027), "rd:Trice %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:Trice -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: Trice(iD(14028), "rd:Trice %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:Trice -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: Trice(iD(14029), "rd:Trice %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:Trice -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: Trice(iD(14030), "rd:Trice %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:Trice -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: Trice(iD(14031), "rd:Trice %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:Trice -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: Trice(iD(14032), "rd:Trice %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:Trice -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: TRice(iD(14033), "sig:TRice with 32-bit stamp and with 0 to 12 values (common use cases)\n" );
        break; case __LINE__: TRice(iD(14034), "rd:TRice\n" );                                                                                                   //exp: time:feed3322default: rd:TRice
        break; case __LINE__: TRice(iD(14035), "rd:TRice %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRice -1     
        break; case __LINE__: TRice(iD(14036), "rd:TRice %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRice -1, -2 
        break; case __LINE__: TRice(iD(14037), "rd:TRice %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRice -1, -2, -3       
        break; case __LINE__: TRice(iD(14038), "rd:TRice %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRice -1, -2, -3, -4      
        break; case __LINE__: TRice(iD(14039), "rd:TRice %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRice -1, -2, -3, -4, -5        
        break; case __LINE__: TRice(iD(14040), "rd:TRice %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRice -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRice(iD(14041), "rd:TRice %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRice -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRice(iD(14042), "rd:TRice %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRice -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRice(iD(14043), "rd:TRice %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRice -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRice(iD(14044), "rd:TRice %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRice -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRice(iD(14045), "rd:TRice %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRice -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRice(iD(14046), "rd:TRice %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRice -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: trice  (iD(14047), "sig:trice16 with 1 to 12 pointer\n" );
        break; case __LINE__: trice16(iD(14048), "rd:trice16 %p\n", -1 );                                                                                            //exp: time:            default: rd:trice16 ffff      
        break; case __LINE__: trice16(iD(14049), "rd:trice16 %p, %p\n", -1, -2 );                                                                                    //exp: time:            default: rd:trice16 ffff, fffe      
        break; case __LINE__: trice16(iD(14050), "rd:trice16 %p, %p, %p\n", -1, -2, -3 );                                                                            //exp: time:            default: rd:trice16 ffff, fffe, fffd          
        break; case __LINE__: trice16(iD(14051), "rd:trice16 %p, %p, %p, %p\n", -1, -2, -3, -4 );                                                                    //exp: time:            default: rd:trice16 ffff, fffe, fffd, fffc            
        break; case __LINE__: trice16(iD(14052), "rd:trice16 %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5 );                                                            //exp: time:            default: rd:trice16 ffff, fffe, fffd, fffc, fffb       
        break; case __LINE__: trice16(iD(14053), "rd:trice16 %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: rd:trice16 ffff, fffe, fffd, fffc, fffb, fffa              
        break; case __LINE__: trice16(iD(14054), "rd:trice16 %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:            default: rd:trice16 ffff, fffe, fffd, fffc, fffb, fffa, fff9           
        break; case __LINE__: trice16(iD(14055), "rd:trice16 %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:            default: rd:trice16 ffff, fffe, fffd, fffc, fffb, fffa, fff9, fff8               
        break; case __LINE__: trice16(iD(14056), "rd:trice16 %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: rd:trice16 ffff, fffe, fffd, fffc, fffb, fffa, fff9, fff8, fff7              
        break; case __LINE__: trice16(iD(14057), "rd:trice16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:            default: rd:trice16 ffff, fffe, fffd, fffc, fffb, fffa, fff9, fff8, fff7, fff6                
        break; case __LINE__: trice16(iD(14058), "rd:trice16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:            default: rd:trice16 ffff, fffe, fffd, fffc, fffb, fffa, fff9, fff8, fff7, fff6, fff5              
        break; case __LINE__: trice16(iD(14059), "rd:trice16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: rd:trice16 ffff, fffe, fffd, fffc, fffb, fffa, fff9, fff8, fff7, fff6, fff5, fff4

        break; case __LINE__: trice8 (iD(14060), "rd:trice8  %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );  //exp: time:            default: rd:trice8  ff, fe, fd, fc, fb, fa, f9, f8, f7, f6, f5, f4
        break; case __LINE__: trice32(iD(14061), "rd:trice32 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );  //exp: time:            default: rd:trice32 ffffffff, fffffffe, fffffffd, fffffffc, fffffffb, fffffffa, fffffff9, fffffff8, fffffff7, fffffff6, fffffff5, fffffff4

        break; case __LINE__: trice  (iD(14062), "sig:trice16 with 1 to 12 hex\n" );

        break; case __LINE__: trice16(iD(14063), "rd:trice16 %X\n", -1 );                                                                                             //exp: time:            default: rd:trice16 FFFF                 
        break; case __LINE__: trice16(iD(14064), "rd:trice16 %X, %X\n", -1, -2 );                                                                                     //exp: time:            default: rd:trice16 FFFF, FFFE             
        break; case __LINE__: trice16(iD(14065), "rd:trice16 %X, %X, %X\n", -1, -2, -3 );                                                                             //exp: time:            default: rd:trice16 FFFF, FFFE, FFFD            
        break; case __LINE__: trice16(iD(14066), "rd:trice16 %X, %X, %X, %X\n", -1, -2, -3, -4 );                                                                     //exp: time:            default: rd:trice16 FFFF, FFFE, FFFD, FFFC                  
        break; case __LINE__: trice16(iD(14067), "rd:trice16 %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5 );                                                             //exp: time:            default: rd:trice16 FFFF, FFFE, FFFD, FFFC, FFFB              
        break; case __LINE__: trice16(iD(14068), "rd:trice16 %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6 );                                                     //exp: time:            default: rd:trice16 FFFF, FFFE, FFFD, FFFC, FFFB, FFFA                     
        break; case __LINE__: trice16(iD(14069), "rd:trice16 %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7);                                              //exp: time:            default: rd:trice16 FFFF, FFFE, FFFD, FFFC, FFFB, FFFA, FFF9                
        break; case __LINE__: trice16(iD(14070), "rd:trice16 %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                     //exp: time:            default: rd:trice16 FFFF, FFFE, FFFD, FFFC, FFFB, FFFA, FFF9, FFF8             
        break; case __LINE__: trice16(iD(14071), "rd:trice16 %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                             //exp: time:            default: rd:trice16 FFFF, FFFE, FFFD, FFFC, FFFB, FFFA, FFF9, FFF8, FFF7          
        break; case __LINE__: trice16(iD(14072), "rd:trice16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                     //exp: time:            default: rd:trice16 FFFF, FFFE, FFFD, FFFC, FFFB, FFFA, FFF9, FFF8, FFF7, FFF6          
        break; case __LINE__: trice16(iD(14073), "rd:trice16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );           //exp: time:            default: rd:trice16 FFFF, FFFE, FFFD, FFFC, FFFB, FFFA, FFF9, FFF8, FFF7, FFF6, FFF5             
        break; case __LINE__: trice16(iD(14074), "rd:trice16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );  //exp: time:            default: rd:trice16 FFFF, FFFE, FFFD, FFFC, FFFB, FFFA, FFF9, FFF8, FFF7, FFF6, FFF5, FFF4

        break; case __LINE__: trice8 (iD(14075), "rd:trice8  %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );  //exp: time:            default: rd:trice8  FF, FE, FD, FC, FB, FA, F9, F8, F7, F6, F5, F4
        break; case __LINE__: trice32(iD(14076), "rd:trice32 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );  //exp: time:            default: rd:trice32 FFFFFFFF, FFFFFFFE, FFFFFFFD, FFFFFFFC, FFFFFFFB, FFFFFFFA, FFFFFFF9, FFFFFFF8, FFFFFFF7, FFFFFFF6, FFFFFFF5, FFFFFFF4

        break; case __LINE__: trice16(iD(14077), "att: line %u\n", __LINE__ );
    
        break; case __LINE__: 
        #if TRICE_DEFERRED_XTEA_ENCRYPT == 1 || TRICE_DIRECT_XTEA_ENCRYPT == 1
                trice(iD(14078), "--------------------------------------------------\n" );
                {
                    uint32_t by[8] = {1,2,3,4,5,6};
                    int count = (sizeof(by) + 7) & ~7; // only multiple of 8 encryptable
                    trice32(iD(14079), "msg: message = %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
                    trice32(iD(14080), "tim: pre TriceEncryption SysTick=%d\n", SYSTICKVAL );
                    XTEAEncrypt(by, count>>2);
                    trice32(iD(14081), "tim: post TriceEncryption SysTick=%d\n", SYSTICKVAL );
                    trice32(iD(14082), "att: TriceEncrypted =  %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] ); 
                    #if XTEA_DECRYPT == 1
                        trice16_1(iD(14083), "tim: pre TriceDecryption SysTick=%d\n", SYSTICKVAL );
                        XTEADecrypt(by, count>>2);
                        trice16_1(iD(14084), "tim: post TriceDecryption SysTick=%d\n", SYSTICKVAL );
                    #endif
                    trice32(iD(14085), "msg: messge = %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
                }
                trice(iD(14086), "--------------------------------------------------\n" );
        #endif

        break; case __LINE__: trice16(iD(14087), "att: line %u\n", __LINE__ );

        break; case __LINE__: trice(iD(14088), "dbg:Hi!\n" );                       //exp: time:            default: dbg:Hi!    
        break; case __LINE__: trice(iD(14089), "dbg:	Hi!\n" );                    //exp: time:            default: dbg:	Hi!

        break; case __LINE__: trice(iD(14090), "sig:Some time measurements\n" );
        break; case __LINE__: trice(iD(14091), "isr:trice isr message, SysTick is %6d\n", SYSTICKVAL);                //exp: time:            default: isr:trice isr message, SysTick is      0 
        break; case __LINE__: trice(iD(14092), "isr:trice isr message, SysTick is %6d\n", SYSTICKVAL);                //exp: time:            default: isr:trice isr message, SysTick is      0 
        break; case __LINE__: trice(iD(14093), "isr:trice isr message, SysTick is %6d\n", SYSTICKVAL);                //exp: time:            default: isr:trice isr message, SysTick is      0 
        break; case __LINE__: trice(iD(14094), "isr:trice isr message, SysTick is %6d\n", SYSTICKVAL);                //exp: time:            default: isr:trice isr message, SysTick is      0

        break; case __LINE__: trice_1(iD(14095), "isr:trice_1 isr message, SysTick is %6d\n", SYSTICKVAL);            //exp: time:            default: isr:trice_1 isr message, SysTick is      0 
        break; case __LINE__: trice_1(iD(14096), "isr:trice_1 isr message, SysTick is %6d\n", SYSTICKVAL);            //exp: time:            default: isr:trice_1 isr message, SysTick is      0 
        break; case __LINE__: trice_1(iD(14097), "isr:trice_1 isr message, SysTick is %6d\n", SYSTICKVAL);            //exp: time:            default: isr:trice_1 isr message, SysTick is      0 
        break; case __LINE__: trice_1(iD(14098), "isr:trice_1 isr message, SysTick is %6d\n", SYSTICKVAL);            //exp: time:            default: isr:trice_1 isr message, SysTick is      0

        break; case __LINE__: TRICE16_1(id(14099), "isr:TRICE16_1 isr message, SysTick is %6d\n", SYSTICKVAL);        //exp: time:            default: isr:TRICE16_1 isr message, SysTick is      0 
        break; case __LINE__: TRICE16_1(id(14100), "isr:TRICE16_1 isr message, SysTick is %6d\n", SYSTICKVAL);        //exp: time:            default: isr:TRICE16_1 isr message, SysTick is      0 
        break; case __LINE__: TRICE16_1(id(14101), "isr:TRICE16_1 isr message, SysTick is %6d\n", SYSTICKVAL);        //exp: time:            default: isr:TRICE16_1 isr message, SysTick is      0 
        break; case __LINE__: TRICE16_1(id(14102), "isr:TRICE16_1 isr message, SysTick is %6d\n", SYSTICKVAL);        //exp: time:            default: isr:TRICE16_1 isr message, SysTick is      0 

        break; case __LINE__: TRICE_1(id(14103), "isr:TRICE_1 isr message, SysTick is %6d\n", SYSTICKVAL);            //exp: time:            default: isr:TRICE_1 isr message, SysTick is      0 
        break; case __LINE__: TRICE_1(id(14104), "isr:TRICE_1 isr message, SysTick is %6d\n", SYSTICKVAL);            //exp: time:            default: isr:TRICE_1 isr message, SysTick is      0 
        break; case __LINE__: TRICE_1(id(14105), "isr:TRICE_1 isr message, SysTick is %6d\n", SYSTICKVAL);            //exp: time:            default: isr:TRICE_1 isr message, SysTick is      0 
        break; case __LINE__: TRICE_1(id(14106), "isr:TRICE_1 isr message, SysTick is %6d\n", SYSTICKVAL);            //exp: time:            default: isr:TRICE_1 isr message, SysTick is      0 

        break; case __LINE__: TRICE(id(14107), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);                //exp: time:            default: isr:TRICE isr message, SysTick is      0 
        break; case __LINE__: TRICE(id(14108), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);                //exp: time:            default: isr:TRICE isr message, SysTick is      0 
        break; case __LINE__: TRICE(id(14109), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);                //exp: time:            default: isr:TRICE isr message, SysTick is      0 
        break; case __LINE__: TRICE(id(14110), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);                //exp: time:            default: isr:TRICE isr message, SysTick is      0 

        break; case __LINE__: trice(iD(14111), "tim:trice START time message\n");                                     //exp: time:            default: tim:trice START time message
        break; case __LINE__: trice(iD(14112), "tim:trice STOP time message\n");                                      //exp: time:            default: tim:trice STOP time message
        break; case __LINE__: trice(iD(14113), "tim:trice START time message\n");                                     //exp: time:            default: tim:trice START time message
        break; case __LINE__: trice(iD(14114), "tim:trice STOP time message\n");                                      //exp: time:            default: tim:trice STOP time message 
        break; case __LINE__: trice(iD(14115), "tim:trice START time message\n");                                     //exp: time:            default: tim:trice START time message
        break; case __LINE__: trice64(iD(14116), "rd:trice64 %d, %d\n", 1, 2 );                                       //exp: time:            default: rd:trice64 1, 2   
        break; case __LINE__: trice(iD(14117), "tim:trice STOP time message\n");                                      //exp: time:            default: tim:trice STOP time message
        
        break; case __LINE__: TRICE(id(14118), "tim:TRICE START time message\n");                                     //exp: time:            default: tim:TRICE START time message
        break; case __LINE__: TRICE(id(14119), "tim:TRICE STOP time message\n");                                      //exp: time:            default: tim:TRICE STOP time message 
        break; case __LINE__: TRICE(id(14120), "tim:TRICE START time message\n");                                     //exp: time:            default: tim:TRICE START time message
        break; case __LINE__: TRICE(id(14121), "tim:TRICE STOP time message\n");                                      //exp: time:            default: tim:TRICE STOP time message 
        break; case __LINE__: TRICE(id(14122), "tim:TRICE START time message\n");                                     //exp: time:            default: tim:TRICE START time message
        break; case __LINE__: TRICE64(id(14123), "rd:TRICE64 %d, %d\n", 1, 2 );                                       //exp: time:            default: rd:TRICE64 1, 2    
        break; case __LINE__: TRICE(id(14124), "tim:TRICE STOP time message\n");                                      //exp: time:            default: tim:TRICE STOP time message 

        break; case __LINE__: trice  (iD(14125), "tim:trice --------------------------------------------------\n");   //exp: time:            default: tim:trice --------------------------------------------------
        break; case __LINE__: trice  (iD(14126), "tim:trice --------------------------------------------------\n");   //exp: time:            default: tim:trice --------------------------------------------------
        break; case __LINE__: trice64(iD(14127), "rd:trice64 %d, %d\n", 1, 2 );                                       //exp: time:            default: rd:trice64 1, 2
        break; case __LINE__: trice  (iD(14128), "tim:trice --------------------------------------------------\n");   //exp: time:            default: tim:trice --------------------------------------------------
        break; case __LINE__: trice  (iD(14129), "tim:trice --------------------------------------------------\n");   //exp: time:            default: tim:trice --------------------------------------------------
        break; case __LINE__: trice64(iD(14130), "rd:trice64 %d, %d\n", 1, 2 );                                       //exp: time:            default: rd:trice64 1, 2
        break; case __LINE__: trice32_1(iD(14131), "rd:trice32_1 %d\n", -1 );                                         //exp: time:            default: rd:trice32_1 -1
        break; case __LINE__: trice64_1(iD(14132), "rd:trice64_1 %d\n", -2 );                                         //exp: time:            default: rd:trice64_1 -2
        break; case __LINE__: trice32_2(iD(14133), "rd:trice32_2 %d,%d\n", -1, 2 );                                   //exp: time:            default: rd:trice32_2 -1,2
        break; case __LINE__: trice64_2(iD(14134), "rd:trice64_2 %d,%d\n", -1, 2 );                                   //exp: time:            default: rd:trice64_2 -1,2

        // special use cases
        break; case __LINE__: TRice64(iD(14135), "info:12 64-bit values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 32-bit stamp.\n", -3, -4, -5, -6, 0, aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), -8 ); //exp: time:feed3322default: info:12 64-bit values -3, 18446744073709551612, fffffffffffffffb, FFFFFFFFFFFFFFFA, false, -7.123457e+00, -7.123457, -7.123456789, -7.123457E+00, -7.123457, -7.123456789, 0xb1111111111111111111111111111111111111111111111111111111111111000 and a 32-bit stamp.
        break; case __LINE__: Trice64(iD(14136), "info:12 64-bit values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 16-bit stamp.\n", -3, -4, -5, -6, 0, aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), -8 ); //exp: time:    be16default: info:12 64-bit values -3, 18446744073709551612, fffffffffffffffb, FFFFFFFFFFFFFFFA, false, -7.123457e+00, -7.123457, -7.123456789, -7.123457E+00, -7.123457, -7.123456789, 0xb1111111111111111111111111111111111111111111111111111111111111000 and a 16-bit stamp.
        break; case __LINE__: trice64(iD(14137), "info:12 64-bit values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and without stamp.\n" , -3, -4, -5, -6, 0, aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), aDouble(-7.123456789), -8 ); //exp: time:            default: info:12 64-bit values -3, 18446744073709551612, fffffffffffffffb, FFFFFFFFFFFFFFFA, false, -7.123457e+00, -7.123457, -7.123456789, -7.123457E+00, -7.123457, -7.123456789, 0xb1111111111111111111111111111111111111111111111111111111111111000 and without stamp.
        break; case __LINE__: TRice32(iD(14138), "info:12 32-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 32-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:feed3322default: info:12 32-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 32-bit stamp.
        break; case __LINE__: Trice32(iD(14139), "info:12 32-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 16-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:    be16default: info:12 32-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 16-bit stamp.
        break; case __LINE__: trice32(iD(14140), "info:12 32-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and without stamp.\n" , -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:            default: info:12 32-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and without stamp.
        break; case __LINE__: TRice16(iD(14141), "info:12 16-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 32-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:feed3322default: info:12 16-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 32-bit stamp.
        break; case __LINE__: Trice16(iD(14142), "info:12 16-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 16-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:    be16default: info:12 16-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 16-bit stamp.
        break; case __LINE__: trice16(iD(14143), "info:12 16-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and without stamp.\n" , -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:            default: info:12 16-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and without stamp.
        break; case __LINE__: TRice8 (iD(14144), "info:12  8-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 32-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:feed3322default: info:12  8-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 32-bit stamp.
        break; case __LINE__: Trice8 (iD(14145), "info:12  8-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and a 16-bit stamp.\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:    be16default: info:12  8-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and a 16-bit stamp.
        break; case __LINE__: trice8 (iD(14146), "info:12  8-bit values %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d and without stamp.\n" , -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );                //exp: time:            default: info:12  8-bit values -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 and without stamp.

        break; case __LINE__: trice16(iD(14147), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRice16(iD(14148), "att: 0x8888 == %04xh\n", 0x8888 ); //exp: time:feed3322default: att: 0x8888 == 8888h
        break; case __LINE__: Trice16(iD(14149), "att: 0x8888 == %04xh\n", 0x8888 ); //exp: time:    be16default: att: 0x8888 == 8888h
        break; case __LINE__: trice16(iD(14150), "att: 0x8888 == %04xh\n", 0x8888 ); //exp: time:            default: att: 0x8888 == 8888h


        break; case __LINE__: trice16(iD(14151), "att: line %u\n", __LINE__ );

        break; case __LINE__: trice8   (iD(14152), "value=%x\n", 0x55 );             //exp: time:            default: value=55
        break; case __LINE__: trice8   (iD(14153), "value=%x\n", 0x88 );             //exp: time:            default: value=88

        break; case __LINE__: trice(iD(14154), "att:mixed int & float & double & bits\n" );
        break; case __LINE__: TRice32(iD(14155), "rd:TRice32 int %d, float %f (%%f), %08x, %032b\n",       FloatToInt32(x),   aFloat(x),   aFloat(x),   aFloat(x) ); //exp: time:feed3322default: rd:TRice32 int 1089, float 1089.608276 (%f), 44883377, 01000100100010000011001101110111                                         
        break; case __LINE__: TRice32(iD(14156), "rd:TRice32 int %d, float %f (%%f), %08x, %032b\n",          (int32_t)(x),   aFloat(x),   aFloat(x),   aFloat(x) ); //exp: time:feed3322default: rd:TRice32 int 1089, float 1089.608276 (%f), 44883377, 01000100100010000011001101110111                                         
        break; case __LINE__: TRice32(iD(14157), "rd:TRice32 int %x, float %f (%%f), %08x, %032b\n",            0x44883377,   aFloat(x),   aFloat(x),   aFloat(x) ); //exp: time:feed3322default: rd:TRice32 int 44883377, float 1089.608276 (%f), 44883377, 01000100100010000011001101110111                                         
        break; case __LINE__: TRice32(iD(14158), "rd:TRice32 int %d, float %f (%%f), %08x, %032b\n",                     x,   aFloat(x),   aFloat(x),   aFloat(x) ); //exp: time:feed3322default: rd:TRice32 int 1089, float 1089.608276 (%f), 44883377, 01000100100010000011001101110111                                         
        break; case __LINE__: TRice32(iD(14159), "rd:TRice32 int %d, float %f (%%f), %08x, %032b\n",      FloatToInt32(-x),  aFloat(-x),  aFloat(-x),  aFloat(-x) ); //exp: time:feed3322default: rd:TRice32 int -1089, float -1089.608276 (%f), c4883377, 11000100100010000011001101110111                                       
        break; case __LINE__: TRice32(iD(14160), "rd:TRice32 int %d, float %f (%%f), %08x, %032b\n",         (int32_t)(-x),  aFloat(-x),  aFloat(-x),  aFloat(-x) ); //exp: time:feed3322default: rd:TRice32 int -1089, float -1089.608276 (%f), c4883377, 11000100100010000011001101110111                                       
        break; case __LINE__: TRice32(iD(14161), "rd:TRice32 int %X, float %f (%%f), %08x, %032b\n",           -0x44883377,  aFloat(-x),  aFloat(-x),  aFloat(-x) ); //exp: time:feed3322default: rd:TRice32 int BB77CC89, float -1089.608276 (%f), c4883377, 11000100100010000011001101110111                                        
        break; case __LINE__: TRice64(iD(14162), "rd:TRice64 int %d, double %f (%%f), %016x, %064b\n",    DoubleToInt64(y),  aDouble(y),  aDouble(y),  aDouble(y) ); //exp: time:feed3322default: rd:TRice64 int 518, double 518.054749 (%f), 4080307020601050, 0100000010000000001100000111000000100000011000000001000001010000   
        break; case __LINE__: TRice64(iD(14163), "rd:TRice64 int %d, double %f (%%f), %016x, %064b\n",        (int64_t)(y),  aDouble(y),  aDouble(y),  aDouble(y) ); //exp: time:feed3322default: rd:TRice64 int 518, double 518.054749 (%f), 4080307020601050, 0100000010000000001100000111000000100000011000000001000001010000   
        break; case __LINE__: TRice64(iD(14164), "rd:TRice64 int %d, double %f (%%f), %016x, %064b\n",                   y,  aDouble(y),  aDouble(y),  aDouble(y) ); //exp: time:feed3322default: rd:TRice64 int 518, double 518.054749 (%f), 4080307020601050, 0100000010000000001100000111000000100000011000000001000001010000   
        break; case __LINE__: TRice64(iD(14165), "rd:TRice64 int %d, double %f (%%f), %016x, %064b\n",   DoubleToInt64(-y), aDouble(-y), aDouble(-y), aDouble(-y) ); //exp: time:feed3322default: rd:TRice64 int -518, double -518.054749 (%f), c080307020601050, 1100000010000000001100000111000000100000011000000001000001010000
        break; case __LINE__: TRice64(iD(14166), "rd:TRice64 int %d, double %f (%%f), %016x, %064b\n",       (int64_t)(-y), aDouble(-y), aDouble(-y), aDouble(-y) ); //exp: time:feed3322default: rd:TRice64 int -518, double -518.054749 (%f), c080307020601050, 1100000010000000001100000111000000100000011000000001000001010000
        break; case __LINE__: TRice64(iD(14167), "rd:TRice64 int %X, double %f (%%f), %016x, %064b\n", -0x4080307020601050, aDouble(-y), aDouble(-y), aDouble(-y) ); //exp: time:feed3322default: rd:TRice64 int BF7FCF8FDF9FEFB0, double -518.054749 (%f), c080307020601050, 1100000010000000001100000111000000100000011000000001000001010000 

        break; case __LINE__: trice16(iD(14168), "att: line %u\n", __LINE__ );

        break; case __LINE__: triceS(iD(14169), "msg:With triceS:%s\n", sABCDE );               //exp: time:            default: msg:With triceS:abcde 12345
        break; case __LINE__: triceN(iD(14170), "sig:With triceN:%s\n", sABCDE, lenABCDE );     //exp: time:            default: sig:With triceN:abcde 12345
        break; case __LINE__: TriceS(iD(14171), "msg:With TriceS:%s\n", sABCDE );               //exp: time:    be16default: msg:With TriceS:abcde 12345
        break; case __LINE__: TriceN(iD(14172), "sig:With TriceN:%s\n", sABCDE, lenABCDE );     //exp: time:    be16default: sig:With TriceN:abcde 12345
        break; case __LINE__: TRiceS(iD(14173), "msg:With TRiceS:%s\n", sABCDE );               //exp: time:feed3322default: msg:With TRiceS:abcde 12345
        break; case __LINE__: TRiceN(iD(14174), "sig:With TRiceN:%s\n", sABCDE, lenABCDE );     //exp: time:feed3322default: sig:With TRiceN:abcde 12345

        break; case __LINE__: TRICE_S(id(14175), "msg:With TRICE_S:%s\n", sABCDE );             //exp: time:            default: msg:With TRICE_S:abcde 12345
        break; case __LINE__: TRICE_N(id(14176), "sig:With TRICE_N:%s\n", sABCDE, lenABCDE );   //exp: time:            default: sig:With TRICE_N:abcde 12345
        break; case __LINE__: TRICE_S(Id(14177), "msg:With TRICE_S:%s\n", sABCDE );             //exp: time:    be16default: msg:With TRICE_S:abcde 12345
        break; case __LINE__: TRICE_N(Id(14178), "sig:With TRICE_N:%s\n", sABCDE, lenABCDE );   //exp: time:    be16default: sig:With TRICE_N:abcde 12345
        break; case __LINE__: TRICE_S(ID(14179), "msg:With TRICE_S:%s\n", sABCDE );             //exp: time:feed3322default: msg:With TRICE_S:abcde 12345
        break; case __LINE__: TRICE_N(ID(14180), "sig:With TRICE_N:%s\n", sABCDE, lenABCDE );   //exp: time:feed3322default: sig:With TRICE_N:abcde 12345

        break; case __LINE__: TRICE8_B(ID(14181), "  %02x", sABCDE, lenABCDE );                 //exp: time:feed3322default:   61  62  63  64  65  20  31  32  33  34  35
        break; case __LINE__: trice(iD(14182), "\n" );
        break; case __LINE__: TRICE8_B(ID(14183), "BUF: %02x\n", sABCDE, lenABCDE );            //exp: time:feed3322default: BUF: 61 62 63 64 65 20 31 32 33 34 35
        break; case __LINE__: TRICE8_B(ID(14184), "%4d", sABCDE, lenABCDE );                    //exp: time:feed3322default:   97  98  99 100 101  32  49  50  51  52  53
        break; case __LINE__: trice(iD(14185), "\n" );
        break; case __LINE__: TRICE8_B(ID(14186), "  %02x", b8, sizeof(b8)/sizeof(int8_t) );    //exp: time:feed3322default:   00  ff  fe  33  04  05  06  07  08  09  0a  0b  00  ff  fe  33  04  05  06  07  08  09  0a  0b
        break; case __LINE__: trice(iD(14187), "\n" );

        break; case __LINE__: TRice8B(iD(14188), " %02x", b8,  sizeof(b8) /sizeof(int8_t) );    //exp: time:feed3322default:  00 ff fe 33 04 05 06 07 08 09 0a 0b 00 ff fe 33 04 05 06 07 08 09 0a 0b
        break; case __LINE__: trice(iD(14189), "\n" );
        break; case __LINE__: Trice8B(iD(14190), " %02x", b8,  sizeof(b8) /sizeof(int8_t) );    //exp: time:    be16default:  00 ff fe 33 04 05 06 07 08 09 0a 0b 00 ff fe 33 04 05 06 07 08 09 0a 0b
        break; case __LINE__: trice(iD(14191), "\n" );
        break; case __LINE__: trice8B(iD(14192), " %02x", b8,  sizeof(b8) /sizeof(int8_t) );    //exp: time:            default:  00 ff fe 33 04 05 06 07 08 09 0a 0b 00 ff fe 33 04 05 06 07 08 09 0a 0b
        break; case __LINE__: trice(iD(14193), "\n" );

        break; case __LINE__: trice8B(iD(14194), "att: %02x\n", b8,  sizeof(b8) /sizeof(int8_t) );  //exp: time:            default: att: 00 ff fe 33 04 05 06 07 08 09 0a 0b 00 ff fe 33 04 05 06 07 08 09 0a 0b
        break; case __LINE__: trice8B(iD(14195), "rd: %02x", b8,  sizeof(b8) /sizeof(int8_t) );    //exp: time:            default: rd: 00 ff fe 33 04 05 06 07 08 09 0a 0b 00 ff fe 33 04 05 06 07 08 09 0a 0b
        break; case __LINE__: trice(iD(14196), "\n" );
        break; case __LINE__: trice8B(iD(14197), " %02x\n", b8,  sizeof(b8) /sizeof(int8_t) );      //exp: time:            default:  00 ff fe 33 04 05 06 07 08 09 0a 0b 00 ff fe 33 04 05 06 07 08 09 0a 0b

        break; case __LINE__: TRICE16_B(ID(14198), "  %04x", b16, sizeof(b16)/sizeof(int16_t) ); //exp: time:feed3322default:   0000  ffff  fffe  3344
        break; case __LINE__: trice(iD(14199), "\n" );
        break; case __LINE__: TRice16B(iD(14200), " %04x", b16, sizeof(b16) /sizeof(int16_t) );  //exp: time:feed3322default:  0000 ffff fffe 3344
        break; case __LINE__: trice(iD(14201), "\n" );
        break; case __LINE__: Trice16B(iD(14202), " %04x", b16, sizeof(b16) /sizeof(int16_t) );  //exp: time:    be16default:  0000 ffff fffe 3344
        break; case __LINE__: trice(iD(14203), "\n" );
        break; case __LINE__: trice16B(iD(14204), " %04x", b16, sizeof(b16) /sizeof(int16_t) );  //exp: time:            default:  0000 ffff fffe 3344
        break; case __LINE__: trice(iD(14205), "\n" );
        break; case __LINE__: trice16B(iD(14206), "msg: %04x\n", b16, sizeof(b16) /sizeof(int16_t) );  //exp: time:            default: msg: 0000 ffff fffe 3344

        break; case __LINE__: TRICE32_B(ID(14207), " %08x", b32, sizeof(b32)/sizeof(int32_t) );  //exp: time:feed3322default:  00000000 ffffffff fffffffe 33445555
        break; case __LINE__: trice(iD(14208), "\n" );
        break; case __LINE__: TRice32B(iD(14209), " %08x", b32, sizeof(b32) /sizeof(int32_t)  ); //exp: time:feed3322default:  00000000 ffffffff fffffffe 33445555
        break; case __LINE__: trice(iD(14210), "\n" );
        break; case __LINE__: Trice32B(iD(14211), " %08x", b32, sizeof(b32) /sizeof(int32_t)  ); //exp: time:    be16default:  00000000 ffffffff fffffffe 33445555
        break; case __LINE__: trice(iD(14212), "\n" );
        break; case __LINE__: trice32B(iD(14213), " %08x", b32, sizeof(b32) /sizeof(int32_t)  ); //exp: time:            default:  00000000 ffffffff fffffffe 33445555
        break; case __LINE__: trice(iD(14214), "\n" );
        break; case __LINE__: trice32B(iD(14215), "att: %08x\n", b32, sizeof(b32) /sizeof(int32_t)  ); //exp: time:            default: att: 00000000 ffffffff fffffffe 33445555

        break; case __LINE__: TRICE64_B(ID(14216), " %016x", b64, sizeof(b64)/sizeof(int64_t) );  //exp: time:feed3322default:  0000000000000000 ffffffffffffffff fffffffffffffffe 3344555566666666
        break; case __LINE__: trice(iD(14217), "\n" );
        break; case __LINE__: TRice64B(iD(14218), " %016x", b64, sizeof(b64) /sizeof(int64_t)  ); //exp: time:feed3322default:  0000000000000000 ffffffffffffffff fffffffffffffffe 3344555566666666
        break; case __LINE__: trice(iD(14219), "\n" );
        break; case __LINE__: Trice64B(iD(14220), " %016x", b64, sizeof(b64) /sizeof(int64_t)  ); //exp: time:    be16default:  0000000000000000 ffffffffffffffff fffffffffffffffe 3344555566666666
        break; case __LINE__: trice(iD(14221), "\n" );
        break; case __LINE__: trice64B(iD(14222), " %016x", b64, sizeof(b64) /sizeof(int64_t)  ); //exp: time:            default:  0000000000000000 ffffffffffffffff fffffffffffffffe 3344555566666666
        break; case __LINE__: trice(iD(14223), "\n" );
        break; case __LINE__: trice64B(iD(14224), "SIG: %016x\n", b64, sizeof(b64) /sizeof(int64_t)  ); //exp: time:            default: SIG: 0000000000000000 ffffffffffffffff fffffffffffffffe 3344555566666666

        break; case __LINE__: TRICE8_F(ID(14225), "info:FunctionNameW", b8,  sizeof(b8) /sizeof(int8_t) );   //exp: time:feed3322default: info:FunctionNameW(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)
        break; case __LINE__: TRice8F(iD(14226), "call:FunctionNameW", b8,  sizeof(b8) /sizeof(int8_t) );    //exp: time:feed3322default: call:FunctionNameW(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)
        break; case __LINE__: Trice8F(iD(14227), "call:FunctionNameW", b8,  sizeof(b8) /sizeof(int8_t) );    //exp: time:    be16default: call:FunctionNameW(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)
        break; case __LINE__: trice8F(iD(14228), "call:FunctionNameW", b8,  sizeof(b8) /sizeof(int8_t) );    //exp: time:            default: call:FunctionNameW(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)
        break; case __LINE__: TRICE16_F(ID(14229), "info:FunctionNameX", b16, sizeof(b16)/sizeof(int16_t) );  //exp: time:feed3322default: info:FunctionNameX(0000)(ffff)(fffe)(3344) 
        break; case __LINE__: TRice16F(iD(14230), "call:FunctionNameX", b16, sizeof(b16)/sizeof(int16_t) );  //exp: time:feed3322default: call:FunctionNameX(0000)(ffff)(fffe)(3344) 
        break; case __LINE__: Trice16F(iD(14231), "call:FunctionNameX", b16, sizeof(b16)/sizeof(int16_t) );  //exp: time:    be16default: call:FunctionNameX(0000)(ffff)(fffe)(3344) 
        break; case __LINE__: trice16F(iD(14232), "call:FunctionNameX", b16, sizeof(b16)/sizeof(int16_t) );  //exp: time:            default: call:FunctionNameX(0000)(ffff)(fffe)(3344) 
        break; case __LINE__: TRICE32_F(ID(14233), "info:FunctionNameY", b32, sizeof(b32)/sizeof(int32_t) );  //exp: time:feed3322default: info:FunctionNameY(00000000)(ffffffff)(fffffffe)(33445555)
        break; case __LINE__: TRice32F(iD(14234), "call:FunctionNameY", b32, sizeof(b32)/sizeof(int32_t) );  //exp: time:feed3322default: call:FunctionNameY(00000000)(ffffffff)(fffffffe)(33445555)
        break; case __LINE__: Trice32F(iD(14235), "call:FunctionNameY", b32, sizeof(b32)/sizeof(int32_t) );  //exp: time:    be16default: call:FunctionNameY(00000000)(ffffffff)(fffffffe)(33445555)
        break; case __LINE__: trice32F(iD(14236), "call:FunctionNameY", b32, sizeof(b32)/sizeof(int32_t) );  //exp: time:            default: call:FunctionNameY(00000000)(ffffffff)(fffffffe)(33445555)
        break; case __LINE__: TRICE64_F(ID(14237), "info:FunctionNameZ", b64, sizeof(b64)/sizeof(int64_t) );  //exp: time:feed3322default: info:FunctionNameZ(0000000000000000)(ffffffffffffffff)(fffffffffffffffe)(3344555566666666)
        break; case __LINE__: TRice64F(iD(14238), "call:FunctionNameZ", b64, sizeof(b64)/sizeof(int64_t) );  //exp: time:feed3322default: call:FunctionNameZ(0000000000000000)(ffffffffffffffff)(fffffffffffffffe)(3344555566666666)
        break; case __LINE__: Trice64F(iD(14239), "call:FunctionNameZ", b64, sizeof(b64)/sizeof(int64_t) );  //exp: time:    be16default: call:FunctionNameZ(0000000000000000)(ffffffffffffffff)(fffffffffffffffe)(3344555566666666)
        break; case __LINE__: trice64F(iD(14240), "call:FunctionNameZ", b64, sizeof(b64)/sizeof(int64_t) );  //exp: time:            default: call:FunctionNameZ(0000000000000000)(ffffffffffffffff)(fffffffffffffffe)(3344555566666666)
        break; case __LINE__: TRiceS(iD(14241), "msg:Hi %s!\n", five );                 //exp: time:feed3322default: msg:Hi five!
        break; case __LINE__: TriceS(iD(14242), "msg:Hi %s!\n", five );                 //exp: time:    be16default: msg:Hi five!
        break; case __LINE__: triceS(iD(14243), "msg:Hi %s!\n", five );                 //exp: time:            default: msg:Hi five!
        break; case __LINE__: TRiceN(iD(14244), "msg:Hi %s!\n", five, strlen(five) );   //exp: time:feed3322default: msg:Hi five!
        break; case __LINE__: TriceN(iD(14245), "msg:Hi %s!\n", five, strlen(five) );   //exp: time:    be16default: msg:Hi five!
        break; case __LINE__: triceN(iD(14246), "msg:Hi %s!\n", five, strlen(five) );   //exp: time:            default: msg:Hi five!

        break; case __LINE__: TRICE_S(id(14247), "sig:TRICE_S=%s\n", s );            //exp: time:            default: sig:TRICE_S=AAAAAAAAAAAA
        break; case __LINE__: TRICE_N(id(14248), "sig:TRICE_N=%s\n", s, strlen(s) ); //exp: time:            default: sig:TRICE_N=AAAAAAAAAAAA
        break; case __LINE__: TRICE_S(Id(14249), "sig:TRICE_S=%s\n", s );            //exp: time:    be16default: sig:TRICE_S=AAAAAAAAAAAA
        break; case __LINE__: TRICE_N(Id(14250), "sig:TRICE_N=%s\n", s, strlen(s) ); //exp: time:    be16default: sig:TRICE_N=AAAAAAAAAAAA
        break; case __LINE__: TRICE_S(ID(14251), "sig:TRICE_S=%s\n", s );            //exp: time:feed3322default: sig:TRICE_S=AAAAAAAAAAAA
        break; case __LINE__: TRICE_N(ID(14252), "sig:TRICE_N=%s\n", s, strlen(s) ); //exp: time:feed3322default: sig:TRICE_N=AAAAAAAAAAAA
        break; case __LINE__: triceS(iD(14253), "sig:triceS=%s\n", s );              //exp: time:            default: sig:triceS=AAAAAAAAAAAA
        break; case __LINE__: triceN(iD(14254), "sig:triceN=%s\n", s, strlen(s) );   //exp: time:            default: sig:triceN=AAAAAAAAAAAA
        break; case __LINE__: TriceS(iD(14255), "sig:TriceS=%s\n", s );              //exp: time:    be16default: sig:TriceS=AAAAAAAAAAAA
        break; case __LINE__: TriceN(iD(14256), "sig:TriceN=%s\n", s, strlen(s) );   //exp: time:    be16default: sig:TriceN=AAAAAAAAAAAA
        break; case __LINE__: TRiceS(iD(14257), "sig:TRiceS=%s\n", s );              //exp: time:feed3322default: sig:TRiceS=AAAAAAAAAAAA
        break; case __LINE__: TRiceN(iD(14258), "sig:TRiceN=%s\n", s, strlen(s) );   //exp: time:feed3322default: sig:TRiceN=AAAAAAAAAAAA

        break; case __LINE__: trice16(iD(14259), "att: line %u\n", __LINE__ );

        break; case __LINE__: exampleOfManualJSONencoding(); //exp: time:    be16default: att:MyStructEvaluationFunction(json:ExA{Apple:-1, Birn:2, Fisch:2.781000}
        break; case __LINE__: TRICE(Id(14260), "MSG:1/11 = %g\n", aFloat( 1.0/11 ) ); //exp: time:    be16default: MSG:1/11 = 0.09090909
        break; case __LINE__: { //exp: time:feed3322default: msg:x = 5.934 = 5.934, 5.934
#if !TRICE_OFF
                              float a = (float)5.934;
                              float b = a + ((a > 0) ? 0.0005f : -0.0005f);
                              int c = (int)b;
                              int d = (int)(b * 1000) % 1000;
                              int e = (int)(1000 * (float)(a - c)); 
                              TRice(iD(14261), "msg:x = %g = %d.%03d, %d.%03d\n", aFloat(a), c, d, c, e ); 
#endif                              
                              }

        break; case __LINE__: trice16(iD(14262), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRice(iD(14263), "sig:Integer (indent, base, sign) - see https://yourbasic.org/golang/fmt-printf-reference-cheat-sheet/\n" );
        break; case __LINE__: TRice(iD(14264), "rd: 15 	%d 	Base 10\n", 15 );                                        //exp: time:feed3322default: rd: 15 	15 	Base 10                                   
        break; case __LINE__: TRice(iD(14265), "rd: +15 	%+d 	Always show sign\n", 15 );                           //exp: time:feed3322default: rd: +15 	+15 	Always show sign                          
        break; case __LINE__: TRice(iD(14266), "rd:   15	%4d 	Pad with spaces (width 4, right justified)\n", 15 ); //exp: time:feed3322default: rd:   15	  15 	Pad with spaces (width 4, right justified)
        break; case __LINE__: TRice(iD(14267), "rd:    15	%-4d	Pad with spaces (width 4, left justified)\n", 15 );  //exp: time:feed3322default: rd:    15	15  	Pad with spaces (width 4, left justified) 
        break; case __LINE__: TRice(iD(14268), "rd: 0015	%04d	Pad with zeroes (width 4)\n", 15 );                  //exp: time:feed3322default: rd: 0015	0015	Pad with zeroes (width 4)                 
        break; case __LINE__: TRice(iD(14269), "rd: 1111 	%b 	Base 2\n", 15 );                                         //exp: time:feed3322default: rd: 1111 	1111 	Base 2                                    
        break; case __LINE__: TRice(iD(14270), "rd: 17 	%o 	Base 8\n", 15 );                                         //exp: time:feed3322default: rd: 17 	17 	Base 8                                    
        break; case __LINE__: TRice(iD(14271), "rd: f 	%x 	Base 16, lowercase\n", 15 );                                 //exp: time:feed3322default: rd: f 	f 	Base 16, lowercase                        
        break; case __LINE__: TRice(iD(14272), "rd: F 	%X 	Base 16, uppercase\n", 15 );                                 //exp: time:feed3322default: rd: F 	F 	Base 16, uppercase                        
        break; case __LINE__: TRice(iD(14273), "rd: 0xf 	%#x 	Base 16, with leading 0x\n", 15 );                   //exp: time:feed3322default: rd: 0xf 	0xf 	Base 16, with leading 0x                  

        break; case __LINE__: trice16(iD(14274), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRICE(ID(14275), "sig:Character (quoted, Unicode)\n" );
        break; case __LINE__: TRICE(ID(14276), "rd: A 	%c 	Character\n", 'A' );                                     //exp: time:feed3322default: rd: A 	A 	Character
        break; case __LINE__: TRICE(ID(14277), "rd: 'A' 	%q 	Quoted character\n", 'A' );                          //exp: time:feed3322default: rd: 'A' 	'A' 	Quoted character
    //  break; case __LINE__: TRICE(ID(14278), "rd: U+0041 	%U 	Unicode\n", 'A' );                        // todo: detect %U to avoid message triceType = TRICE0 ParamSpace = 4 not matching with bitWidth  0 and paramCount 0 - ignoring package
    //  break; case __LINE__: TRICE(ID(14279), "rd: U+0041 'A' 	%#U 	Unicode with character\n", 'A' ); // todo: detect %#U to avoid message triceType = TRICE0 ParamSpace = 4 not matching with bitWidth  0 and paramCount 0 - ignoring package

        break; case __LINE__: TRICE(ID(14280), "sig:Boolean (true/false)\n" );
        break; case __LINE__: TRICE(ID(14281), "rd:Use %%t to format a boolean as true (%t) or false (%t).\n", 1, 0 ); //exp: time:feed3322default: rd:Use %t to format a boolean as true (true) or false (false).

        break; case __LINE__: TRICE(ID(14282), "sig:Pointer (hex)\n" );
        break; case __LINE__: TRICE8 (ID(14283), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x84 );               //exp: time:feed3322default: rd:Use %p to format a pointer in base 16 notation with leading 0x. (84)              
        break; case __LINE__: TRICE16(ID(14284), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x1234 );             //exp: time:feed3322default: rd:Use %p to format a pointer in base 16 notation with leading 0x. (1234)            
        break; case __LINE__: TRICE32(ID(14285), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0xaabbccdd );         //exp: time:feed3322default: rd:Use %p to format a pointer in base 16 notation with leading 0x. (aabbccdd)        
        break; case __LINE__: TRICE64(ID(14286), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x1122334455667788 ); //exp: time:feed3322default: rd:Use %p to format a pointer in base 16 notation with leading 0x. (1122334455667788)
        break; case __LINE__: TRICE  (ID(14287), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0xaabbccdd);          //exp: time:feed3322default: rd:Use %p to format a pointer in base 16 notation with leading 0x. (aabbccdd)        

        break; case __LINE__: trice16(iD(14288), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRICE(ID(14289), "sig:Float (indent, precision, scientific notation)\n" );
        break; case __LINE__: TRICE(ID(14290), "rd: 1.234560e+02		%e 	%%e Scientific notation\n", aFloat(123.456) );                       //exp: time:feed3322default: rd: 1.234560e+02		1.234560e+02 	%e Scientific notation
        break; case __LINE__: TRICE(ID(14291), "rd: 123.456000		%f 	%%f Decimal point, no exponent\n", aFloat(123.456) );                    //exp: time:feed3322default: rd: 123.456000		123.456001 	%f Decimal point, no exponent
        break; case __LINE__: TRICE(ID(14292), "rd: 123.46			%.2f		%%.2f Default width, precision 2\n", aFloat(123.456) );          //exp: time:feed3322default: rd: 123.46			123.46		%.2f Default width, precision 2
        break; case __LINE__: TRICE(ID(14293), "rd: ␣␣123.46		%8.2f	%%8.2f Width 8, precision 2\n", aFloat(123.456) );                    //exp: time:feed3322default: rd: ␣␣123.46		  123.46	%8.2f Width 8, precision 2
        break; case __LINE__: TRICE(ID(14294), "rd: 123.456		%g		%%g Exponent as needed, necessary digits only\n", aFloat(123.456) ); //exp: time:feed3322default: rd: 123.456		123.456		%g Exponent as needed, necessary digits only

        break; case __LINE__: TRICE(ID(14295), "sig:Double (indent, precision, scientific notation)\n" );
        break; case __LINE__: TRICE64(ID(14296), "rd: 1.234560e+02		%e 	%%e Scientific notation\n", aDouble(123.456) );                       //exp: time:feed3322default: rd: 1.234560e+02		1.234560e+02 	%e Scientific notation
        break; case __LINE__: TRICE64(ID(14297), "rd: 123.456000		%f 	%%f Decimal point, no exponent\n", aDouble(123.456) );                //exp: time:feed3322default: rd: 123.456000		123.456000 	%f Decimal point, no exponent
        break; case __LINE__: TRICE64(ID(14298), "rd: 123.46			%.2f    	%%.2f Default width, precision 2\n", aDouble(123.456) );      //exp: time:feed3322default: rd: 123.46			123.46    	%.2f Default width, precision 2
        break; case __LINE__: TRICE64(ID(14299), "rd: ␣␣123.46		%8.2f 	%%8.2f Width 8, precision 2\n", aDouble(123.456) );                    //exp: time:feed3322default: rd: ␣␣123.46		  123.46 	%8.2f Width 8, precision 2
        break; case __LINE__: TRICE64(ID(14300), "rd: 123.456		%g		%%g Exponent as needed, necessary digits only\n", aDouble(123.456) ); //exp: time:feed3322default: rd: 123.456		123.456		%g Exponent as needed, necessary digits only

        break; case __LINE__: TRICE(ID(14301), "sig:String or byte slice (quote, indent, hex)\n" );
        break; case __LINE__: s = "café"; TRICE_S(ID(14302), "rd: café 			%s		Plain string\n", s );           //exp: time:feed3322default: rd: café 			café		Plain string
        break; case __LINE__: s = "café"; TRICE_S(ID(14303), "rd: ␣␣café 		%6s 		Width 6, right justify\n", s );  //exp: time:feed3322default: rd: ␣␣café 		  café 		Width 6, right justify
        break; case __LINE__: s = "café"; TRICE_S(ID(14304), "rd: café␣␣ 		%-6s 		Width 6, left justify\n", s );   //exp: time:feed3322default: rd: café␣␣ 		café   		Width 6, left justify
        break; case __LINE__: s = "café"; TRICE_S(ID(14305), "rd: \"café\" 		%q		Quoted string\n", s );          //exp: time:feed3322default: rd: \"café\" 		"café"		Quoted string
        break; case __LINE__: s = "café"; TRICE_S(ID(14306), "rd: 636166c3a9 		%x	Hex dump of byte values\n", s );    //exp: time:feed3322default: rd: 636166c3a9 		636166c3a9	Hex dump of byte values 
        break; case __LINE__: s = "café"; TRICE_S(ID(14307), "rd: 63 61 66 c3 a9 	% x	Hex dump with spaces\n", s );       //exp: time:feed3322default: rd: 63 61 66 c3 a9 	63 61 66 c3 a9	Hex dump with spaces

        break; case __LINE__: trice16(iD(14308), "att: line %u\n", __LINE__ );
        break; case __LINE__: exampleOfManualSerialization(); // ATTENTION: This occupies ~1024 bytes in one half buffer when double buffer ist used!

        // This was a CGO compiler issue:
        break; case __LINE__: TRice(iD(14309), "info:12 default bit width values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 32-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //exp: time:feed3322default: info:12 default bit width values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and a 32-bit stamp.
        break; case __LINE__: Trice(iD(14310), "info:12 default bit width values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 16-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //exp: time:    be16default: info:12 default bit width values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and a 16-bit stamp.
        break; case __LINE__: trice(iD(14311), "info:12 default bit width values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and without stamp.\n" , -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //exp: time:            default: info:12 default bit width values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and without stamp.

        break; case __LINE__: trice16(iD(14312), "att: line %u\n", __LINE__ );

        break; case __LINE__: TRICE(ID(14313), "sig:Runtime generated strings\n" );
        break; case __LINE__: 
        {
            #if !TRICE_OFF
            char* s0 = "AAAAAAAAAAAA";
            #endif
            TRICE32(ID(14314), "dbg:len=%u:", strlen(s0) );
            TRICE_S(ID(14315), "sig:%s\n", s0 );
        }
        break; case __LINE__: 
        {
            #if !TRICE_OFF
            char* s1 = "\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBCCCCCCCC\
";
#endif
                TRICE32(ID(14316), "dbg:len=%u:", strlen(s1) );
                TRICE_S(ID(14317), "sig:%s\n", s1 );
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
            TRICE32(ID(14318), "dbg:len=%u: ", strlen(s2) );
            TRICE_S(ID(14319), "sig:%s\n", s2 );
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
            TRICE32(ID(14320), "dbg:len=%u: ", strlen(s3) );
            TRICE_S(ID(14321), "sig:%s\n", s3 );
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
            TRICE32(ID(14322), "dbg:len=%u: ", strlen(s4) );
            TRICE_S(ID(14323), "sig:%s\n", s4 );
        }


#if TRICE_CGO == 1

        break; case __LINE__: TRICE  (ID(14324), "rd:%E (%%E)\n",  aFloat(-555555555.5555555555) ); //exp: time:feed3322default: rd:-5.555556E+08 (%E)
        break; case __LINE__: TRICE  (ID(14325), "rd:%F (%%F)\n",  aFloat(-555555555.5555555555) ); //exp: time:feed3322default: rd:-555555584.000000 (%F)
        break; case __LINE__: TRICE  (ID(14326), "rd:%G (%%G)\n",  aFloat(-555555555.5555555555) ); //exp: time:feed3322default: rd:-5.555556E+08 (%G)
        break; case __LINE__: TRICE64(ID(14327), "rd:%E (%%E)\n", aDouble(-555555555.5555555555) ); //exp: time:feed3322default: rd:-5.555556E+08 (%E)
        break; case __LINE__: TRICE64(ID(14328), "rd:%F (%%F)\n", aDouble(-555555555.5555555555) ); //exp: time:feed3322default: rd:-555555555.555556 (%F)
        break; case __LINE__: TRICE64(ID(14329), "rd:%G (%%G)\n", aDouble(-555555555.5555555555) ); //exp: time:feed3322default: rd:-5.555555555555556E+08 (%G)
        
        break; case __LINE__: TRICE  (ID(14330), "rd:%e (%%e)\n",  aFloat(-555555555.5555555555) ); //exp: time:feed3322default: rd:-5.555556e+08 (%e)
        break; case __LINE__: TRICE  (ID(14331), "rd:%f (%%f)\n",  aFloat(-555555555.5555555555) ); //exp: time:feed3322default: rd:-555555584.000000 (%f)
        break; case __LINE__: TRICE  (ID(14332), "rd:%g (%%g)\n",  aFloat(-555555555.5555555555) ); //exp: time:feed3322default: rd:-5.555556e+08 (%g)
        break; case __LINE__: TRICE64(ID(14333), "rd:%e (%%e)\n", aDouble(-555555555.5555555555) ); //exp: time:feed3322default: rd:-5.555556e+08 (%e)
        break; case __LINE__: TRICE64(ID(14334), "rd:%f (%%f)\n", aDouble(-555555555.5555555555) ); //exp: time:feed3322default: rd:-555555555.555556 (%f)
        break; case __LINE__: TRICE64(ID(14335), "rd:%g (%%g)\n", aDouble(-555555555.5555555555) ); //exp: time:feed3322default: rd:-5.555555555555556e+08 (%g)

        break; case __LINE__: TRICE32(ID(14336), "msg:%u (%%u)\n", -1 ); //exp: time:feed3322default: msg:4294967295 (%u)
        break; case __LINE__: TRICE32(ID(14337), "msg:%b (%%b)\n", -1 ); //exp: time:feed3322default: msg:11111111111111111111111111111111 (%b)
        break; case __LINE__: TRICE32(ID(14338), "msg:%o (%%o)\n", -1 ); //exp: time:feed3322default: msg:37777777777 (%o)
        break; case __LINE__: TRICE32(ID(14339), "msg:%O (%%O)\n", -1 ); //exp: time:feed3322default: msg:0o37777777777 (%O)
        break; case __LINE__: TRICE32(ID(14340), "msg:%X (%%X)\n", -1 ); //exp: time:feed3322default: msg:FFFFFFFF (%X)
        break; case __LINE__: TRICE32(ID(14341), "msg:%x (%%x)\n", -1 ); //exp: time:feed3322default: msg:ffffffff (%x)
        break; case __LINE__: TRICE32(ID(14342), "msg:%d (%%d)\n", -1 ); //exp: time:feed3322default: msg:-1 (%d)
        break; case __LINE__: TRICE64(ID(14343), "msg:%u (%%u)\n", -1 ); //exp: time:feed3322default: msg:18446744073709551615 (%u)
        break; case __LINE__: TRICE64(ID(14344), "msg:%b (%%b)\n", -1 ); //exp: time:feed3322default: msg:1111111111111111111111111111111111111111111111111111111111111111 (%b)
        break; case __LINE__: TRICE64(ID(14345), "msg:%o (%%o)\n", -1 ); //exp: time:feed3322default: msg:1777777777777777777777 (%o)
        break; case __LINE__: TRICE64(ID(14346), "msg:%O (%%O)\n", -1 ); //exp: time:feed3322default: msg:0o1777777777777777777777 (%O)
        break; case __LINE__: TRICE64(ID(14347), "msg:%X (%%X)\n", -1 ); //exp: time:feed3322default: msg:FFFFFFFFFFFFFFFF (%X)
        break; case __LINE__: TRICE64(ID(14348), "msg:%x (%%x)\n", -1 ); //exp: time:feed3322default: msg:ffffffffffffffff (%x)

    /**/break; case __LINE__: TRice        (iD(14349), "value=%d\n", -2  ); //exp: time:feed3322default: value=-2
    /**/break; case __LINE__: Trice        (iD(14350), "value=%d\n", -2  ); //exp: time:    be16default: value=-2
    /**/break; case __LINE__: trice        (iD(14351), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: TRice8       (iD(14352), "value=%d\n", -2  ); //exp: time:feed3322default: value=-2
        break; case __LINE__: Trice8       (iD(14353), "value=%d\n", -2  ); //exp: time:    be16default: value=-2
        break; case __LINE__: trice8       (iD(14354), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: TRice16      (iD(14355), "value=%d\n", -2  ); //exp: time:feed3322default: value=-2
        break; case __LINE__: Trice16      (iD(14356), "value=%d\n", -2  ); //exp: time:    be16default: value=-2
        break; case __LINE__: trice16      (iD(14357), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: TRice32      (iD(14358), "value=%d\n", -2  ); //exp: time:feed3322default: value=-2
        break; case __LINE__: Trice32      (iD(14359), "value=%d\n", -2  ); //exp: time:    be16default: value=-2
        break; case __LINE__: trice32      (iD(14360), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: TRice64      (iD(14361), "value=%d\n", -2  ); //exp: time:feed3322default: value=-2
        break; case __LINE__: Trice64      (iD(14362), "value=%d\n", -2  ); //exp: time:    be16default: value=-2
        break; case __LINE__: trice64      (iD(14363), "value=%d\n", -2  ); //exp: time:            default: value=-2
        
        break; case __LINE__: TRice_1      (iD(14364), "value=%d\n", -2  ); //exp: time:feed3322default: value=-2
        break; case __LINE__: Trice_1      (iD(14365), "value=%d\n", -2  ); //exp: time:    be16default: value=-2
        break; case __LINE__: trice_1      (iD(14366), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: TRice8_1     (iD(14367), "value=%d\n", -2  ); //exp: time:feed3322default: value=-2
        break; case __LINE__: Trice8_1     (iD(14368), "value=%d\n", -2  ); //exp: time:    be16default: value=-2
        break; case __LINE__: trice8_1     (iD(14369), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: TRice16_1    (iD(14370), "value=%d\n", -2  ); //exp: time:feed3322default: value=-2
        break; case __LINE__: Trice16_1    (iD(14371), "value=%d\n", -2  ); //exp: time:    be16default: value=-2
        break; case __LINE__: trice16_1    (iD(14372), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: TRice32_1    (iD(14373), "value=%d\n", -2  ); //exp: time:feed3322default: value=-2
        break; case __LINE__: Trice32_1    (iD(14374), "value=%d\n", -2  ); //exp: time:    be16default: value=-2
        break; case __LINE__: trice32_1    (iD(14375), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: TRice64_1    (iD(14376), "value=%d\n", -2  ); //exp: time:feed3322default: value=-2
        break; case __LINE__: Trice64_1    (iD(14377), "value=%d\n", -2  ); //exp: time:    be16default: value=-2
        break; case __LINE__: trice64_1    (iD(14378), "value=%d\n", -2  ); //exp: time:            default: value=-2
        
        break; case __LINE__: TRice        (iD(14379), "no value" ); //exp: time:feed3322default: no value
        break; case __LINE__: Trice        (iD(14380), "no value" ); //exp: time:    be16default: no value
        break; case __LINE__: trice        (iD(14381), "no value" ); //exp: time:            default: no value
        break; case __LINE__: TRice8       (iD(14382), "no value" ); //exp: time:feed3322default: no value
        break; case __LINE__: Trice8       (iD(14383), "no value" ); //exp: time:    be16default: no value
        break; case __LINE__: trice8       (iD(14384), "no value" ); //exp: time:            default: no value
        break; case __LINE__: TRice16      (iD(14385), "no value" ); //exp: time:feed3322default: no value
        break; case __LINE__: Trice16      (iD(14386), "no value" ); //exp: time:    be16default: no value
        break; case __LINE__: trice16      (iD(14387), "no value" ); //exp: time:            default: no value
        break; case __LINE__: TRice32      (iD(14388), "no value" ); //exp: time:feed3322default: no value
        break; case __LINE__: Trice32      (iD(14389), "no value" ); //exp: time:    be16default: no value
        break; case __LINE__: trice32      (iD(14390), "no value" ); //exp: time:            default: no value
        break; case __LINE__: TRice64      (iD(14391), "no value" ); //exp: time:feed3322default: no value
        break; case __LINE__: Trice64      (iD(14392), "no value" ); //exp: time:    be16default: no value
        break; case __LINE__: trice64      (iD(14393), "no value" ); //exp: time:            default: no value
    
        break; case __LINE__: TRice_0      (iD(14394), "no value" ); //exp: time:feed3322default: no value
        break; case __LINE__: Trice_0      (iD(14395), "no value" ); //exp: time:    be16default: no value
        break; case __LINE__: trice_0      (iD(14396), "no value" ); //exp: time:            default: no value
        break; case __LINE__: TRice8_0     (iD(14397), "no value" ); //exp: time:feed3322default: no value
        break; case __LINE__: Trice8_0     (iD(14398), "no value" ); //exp: time:    be16default: no value
        break; case __LINE__: trice8_0     (iD(14399), "no value" ); //exp: time:            default: no value
        break; case __LINE__: TRice16_0    (iD(14400), "no value" ); //exp: time:feed3322default: no value
        break; case __LINE__: Trice16_0    (iD(14401), "no value" ); //exp: time:    be16default: no value
        break; case __LINE__: trice16_0    (iD(14402), "no value" ); //exp: time:            default: no value
        break; case __LINE__: TRice32_0    (iD(14403), "no value" ); //exp: time:feed3322default: no value
        break; case __LINE__: Trice32_0    (iD(14404), "no value" ); //exp: time:    be16default: no value
        break; case __LINE__: trice32_0    (iD(14405), "no value" ); //exp: time:            default: no value
        break; case __LINE__: TRice64_0    (iD(14406), "no value" ); //exp: time:feed3322default: no value
        break; case __LINE__: Trice64_0    (iD(14407), "no value" ); //exp: time:    be16default: no value
        break; case __LINE__: trice64_0    (iD(14408), "no value" ); //exp: time:            default: no value
        break; case __LINE__: TRice0       (iD(14409), "no value" ); //exp: time:feed3322default: no value
        break; case __LINE__: Trice0       (iD(14410), "no value" ); //exp: time:    be16default: no value
        break; case __LINE__: trice0       (iD(14411), "no value" ); //exp: time:            default: no value
        break; case __LINE__: TRice        (iD(14412), "no value" ); //exp: time:feed3322default: no value
        break; case __LINE__: Trice        (iD(14413), "no value" ); //exp: time:    be16default: no value
        break; case __LINE__: trice        (iD(14414), "no value" ); //exp: time:            default: no value

        break; case __LINE__: TRice(iD(14415), "info:This is a message with one value %d and a 32-bit stamp.\n", -2  ); //exp: time:feed3322default: info:This is a message with one value -2 and a 32-bit stamp.
        break; case __LINE__: Trice(iD(14416), "info:This is a message with one value %d and a 16-bit stamp.\n", -2  ); //exp: time:    be16default: info:This is a message with one value -2 and a 16-bit stamp.
        break; case __LINE__: trice(iD(14417), "info:This is a message with one value %d and without stamp.\n" , -2  ); //exp: time:            default: info:This is a message with one value -2 and without stamp.

        break; case __LINE__: TRICE(ID(14418), "info:This is a message without values and a 32-bit stamp.\n" );        //exp: time:feed3322default: info:This is a message without values and a 32-bit stamp.
        break; case __LINE__: TRICE(Id(14419), "info:This is a message without values and a 16-bit stamp.\n" );        //exp: time:    be16default: info:This is a message without values and a 16-bit stamp.
        break; case __LINE__: TRICE(id(14420), "info:This is a message without values and without stamp.\n"  );        //exp: time:            default: info:This is a message without values and without stamp.

        break; case __LINE__: TRICE(ID(14421), "info:This is a message with one value %d and a 32-bit stamp.\n", -2  ); //exp: time:feed3322default: info:This is a message with one value -2 and a 32-bit stamp.
        break; case __LINE__: TRICE(Id(14422), "info:This is a message with one value %d and a 16-bit stamp.\n", -2  ); //exp: time:    be16default: info:This is a message with one value -2 and a 16-bit stamp.
        break; case __LINE__: TRICE(id(14423), "info:This is a message with one value %d and without stamp.\n" , -2  ); //exp: time:            default: info:This is a message with one value -2 and without stamp.

        break; case __LINE__: TRICE(ID(14424), "info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 32-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 );   //exp: time:feed3322default: info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and a 32-bit stamp.
        break; case __LINE__: TRICE(Id(14425), "info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 16-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 );   //exp: time:    be16default: info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and a 16-bit stamp.
        break; case __LINE__: TRICE(id(14426), "info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and without stamp.\n" , -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 );   //exp: time:            default: info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and without stamp.

        break; case __LINE__: TRICE32(ID(14427), "info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 32-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //exp: time:feed3322default: info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and a 32-bit stamp.
        break; case __LINE__: TRICE32(Id(14428), "info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and a 16-bit stamp.\n", -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //exp: time:    be16default: info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and a 16-bit stamp.
        break; case __LINE__: TRICE32(id(14429), "info:12 values %d, %u, %x, %X, %t, %e, %f, %g, %E, %F, %G, 0xb%08b and without stamp.\n" , -3, -4, -5, -6, 1, aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), aFloat(-7.123456789), -8 ); //exp: time:            default: info:12 values -3, 4294967292, fffffffb, FFFFFFFA, true, -7.123457e+00, -7.123457, -7.123457, -7.123457E+00, -7.123457, -7.123457, 0xb11111111111111111111111111111000 and without stamp.

        break; case __LINE__: TRICE8   (id(14430), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: TRICE8_1 (id(14431), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: TRICE8   (Id(14432), "msg:value=%u\n", 200 ); //exp: time:    be16default: msg:value=200
        break; case __LINE__: TRICE8_1 (Id(14433), "msg:value=%u\n", 200 ); //exp: time:    be16default: msg:value=200
        break; case __LINE__: TRICE8   (ID(14434), "msg:value=%u\n", 200 ); //exp: time:feed3322default: msg:value=200
        break; case __LINE__: TRICE8_1 (ID(14435), "msg:value=%u\n", 200 ); //exp: time:feed3322default: msg:value=200
        break; case __LINE__: trice8   (iD(14436), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: trice8_1 (iD(14437), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: Trice8   (iD(14438), "msg:value=%u\n", 200 ); //exp: time:    be16default: msg:value=200
        break; case __LINE__: Trice8_1 (iD(14439), "msg:value=%u\n", 200 ); //exp: time:    be16default: msg:value=200
        break; case __LINE__: TRice8   (iD(14440), "msg:value=%u\n", 200 ); //exp: time:feed3322default: msg:value=200
        break; case __LINE__: TRice8_1 (iD(14441), "msg:value=%u\n", 200 ); //exp: time:feed3322default: msg:value=200

        break; case __LINE__: TRICE16  (id(14442), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: TRICE16_1(id(14443), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: TRICE16  (Id(14444), "msg:value=%u\n", 200 ); //exp: time:    be16default: msg:value=200
        break; case __LINE__: TRICE16_1(Id(14445), "msg:value=%u\n", 200 ); //exp: time:    be16default: msg:value=200
        break; case __LINE__: TRICE16  (ID(14446), "msg:value=%u\n", 200 ); //exp: time:feed3322default: msg:value=200
        break; case __LINE__: TRICE16_1(ID(14447), "msg:value=%u\n", 200 ); //exp: time:feed3322default: msg:value=200
        break; case __LINE__: trice16  (iD(14448), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: trice16_1(iD(14449), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: Trice16  (iD(14450), "msg:value=%u\n", 200 ); //exp: time:    be16default: msg:value=200
        break; case __LINE__: Trice16_1(iD(14451), "msg:value=%u\n", 200 ); //exp: time:    be16default: msg:value=200
        break; case __LINE__: TRice16  (iD(14452), "msg:value=%u\n", 200 ); //exp: time:feed3322default: msg:value=200
        break; case __LINE__: TRice16_1(iD(14453), "msg:value=%u\n", 200 ); //exp: time:feed3322default: msg:value=200

        break; case __LINE__: TRICE32  (id(14454), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: TRICE32_1(id(14455), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: TRICE32  (Id(14456), "msg:value=%u\n", 200 ); //exp: time:    be16default: msg:value=200
        break; case __LINE__: TRICE32_1(Id(14457), "msg:value=%u\n", 200 ); //exp: time:    be16default: msg:value=200
        break; case __LINE__: TRICE32  (ID(14458), "msg:value=%u\n", 200 ); //exp: time:feed3322default: msg:value=200
        break; case __LINE__: TRICE32_1(ID(14459), "msg:value=%u\n", 200 ); //exp: time:feed3322default: msg:value=200
        break; case __LINE__: trice32  (iD(14460), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: trice32_1(iD(14461), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: Trice32  (iD(14462), "msg:value=%u\n", 200 ); //exp: time:    be16default: msg:value=200
        break; case __LINE__: Trice32_1(iD(14463), "msg:value=%u\n", 200 ); //exp: time:    be16default: msg:value=200
        break; case __LINE__: TRice32  (iD(14464), "msg:value=%u\n", 200 ); //exp: time:feed3322default: msg:value=200
        break; case __LINE__: TRice32_1(iD(14465), "msg:value=%u\n", 200 ); //exp: time:feed3322default: msg:value=200

        break; case __LINE__: TRICE64  (id(14466), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: TRICE64_1(id(14467), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: TRICE64  (Id(14468), "msg:value=%u\n", 200 ); //exp: time:    be16default: msg:value=200
        break; case __LINE__: TRICE64_1(Id(14469), "msg:value=%u\n", 200 ); //exp: time:    be16default: msg:value=200
        break; case __LINE__: TRICE64  (ID(14470), "msg:value=%u\n", 200 ); //exp: time:feed3322default: msg:value=200
        break; case __LINE__: TRICE64_1(ID(14471), "msg:value=%u\n", 200 ); //exp: time:feed3322default: msg:value=200
        break; case __LINE__: trice64  (iD(14472), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: trice64_1(iD(14473), "msg:value=%u\n", 200 ); //exp: time:            default: msg:value=200
        break; case __LINE__: Trice64  (iD(14474), "msg:value=%u\n", 200 ); //exp: time:    be16default: msg:value=200
        break; case __LINE__: Trice64_1(iD(14475), "msg:value=%u\n", 200 ); //exp: time:    be16default: msg:value=200
        break; case __LINE__: TRice64  (iD(14476), "msg:value=%u\n", 200 ); //exp: time:feed3322default: msg:value=200
        break; case __LINE__: TRice64_1(iD(14477), "msg:value=%u\n", 200 ); //exp: time:feed3322default: msg:value=200

        // mixed 16-bit and 32-bit timestamp 

        break; case __LINE__: TRice8(iD(14478), "msg:value=%d\n", -1 );                                                                                            //exp: time:feed3322default: msg:value=-1
        break; case __LINE__: Trice8(iD(14479), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: msg:value=-1, -2
        break; case __LINE__: trice8(iD(14480), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: msg:value=-1, -2, -3
        break; case __LINE__: TRice8(iD(14481), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice8(iD(14482), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice8(iD(14483), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice8(iD(14484), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:feed3322default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice8(iD(14485), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice8(iD(14486), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice8(iD(14487), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:feed3322default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice8(iD(14488), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice8(iD(14489), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice16(iD(14490), "msg:value=%d\n", -1 );                                                                                            //exp: time:feed3322default: msg:value=-1
        break; case __LINE__: Trice16(iD(14491), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: msg:value=-1, -2
        break; case __LINE__: trice16(iD(14492), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: msg:value=-1, -2, -3
        break; case __LINE__: TRice16(iD(14493), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice16(iD(14494), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice16(iD(14495), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice16(iD(14496), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:feed3322default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice16(iD(14497), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice16(iD(14498), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice16(iD(14499), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:feed3322default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice16(iD(14500), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice16(iD(14501), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice32(iD(14502), "msg:value=%d\n", -1 );                                                                                            //exp: time:feed3322default: msg:value=-1
        break; case __LINE__: Trice32(iD(14503), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: msg:value=-1, -2
        break; case __LINE__: trice32(iD(14504), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: msg:value=-1, -2, -3
        break; case __LINE__: TRice32(iD(14505), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice32(iD(14506), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice32(iD(14507), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice32(iD(14508), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:feed3322default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice32(iD(14509), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice32(iD(14510), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice32(iD(14511), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:feed3322default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice32(iD(14512), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice32(iD(14513), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice64(iD(14514), "msg:value=%d\n", -1 );                                                                                            //exp: time:feed3322default: msg:value=-1
        break; case __LINE__: Trice64(iD(14515), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: msg:value=-1, -2
        break; case __LINE__: trice64(iD(14516), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: msg:value=-1, -2, -3
        break; case __LINE__: TRice64(iD(14517), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice64(iD(14518), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice64(iD(14519), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice64(iD(14520), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:feed3322default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice64(iD(14521), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice64(iD(14522), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice64(iD(14523), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:feed3322default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice64(iD(14524), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice64(iD(14525), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice8_1 (iD(14526), "msg:value=%d\n", -1 );                                                                                            //exp: time:feed3322default: msg:value=-1
        break; case __LINE__: Trice8_2 (iD(14527), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: msg:value=-1, -2
        break; case __LINE__: trice8_3 (iD(14528), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: msg:value=-1, -2, -3
        break; case __LINE__: TRice8_4 (iD(14529), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice8_5 (iD(14530), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice8_6 (iD(14531), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice8_7 (iD(14532), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:feed3322default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice8_8 (iD(14533), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice8_9 (iD(14534), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice8_10(iD(14535), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:feed3322default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice8_11(iD(14536), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice8_12(iD(14537), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice16_1 (iD(14538), "msg:value=%d\n", -1 );                                                                                            //exp: time:feed3322default: msg:value=-1
        break; case __LINE__: Trice16_2 (iD(14539), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: msg:value=-1, -2
        break; case __LINE__: trice16_3 (iD(14540), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: msg:value=-1, -2, -3
        break; case __LINE__: TRice16_4 (iD(14541), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice16_5 (iD(14542), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice16_6 (iD(14543), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice16_7 (iD(14544), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:feed3322default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice16_8 (iD(14545), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice16_9 (iD(14546), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice16_10(iD(14547), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:feed3322default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice16_11(iD(14548), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice16_12(iD(14549), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice32_1 (iD(14550), "msg:value=%d\n", -1 );                                                                                            //exp: time:feed3322default: msg:value=-1
        break; case __LINE__: Trice32_2 (iD(14551), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: msg:value=-1, -2
        break; case __LINE__: trice32_3 (iD(14552), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: msg:value=-1, -2, -3
        break; case __LINE__: TRice32_4 (iD(14553), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice32_5 (iD(14554), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice32_6 (iD(14555), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice32_7 (iD(14556), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:feed3322default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice32_8 (iD(14557), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice32_9 (iD(14558), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice32_10(iD(14559), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:feed3322default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice32_11(iD(14560), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice32_12(iD(14561), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: TRice64_1 (iD(14562), "msg:value=%d\n", -1 );                                                                                            //exp: time:feed3322default: msg:value=-1
        break; case __LINE__: Trice64_2 (iD(14563), "msg:value=%d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: msg:value=-1, -2
        break; case __LINE__: trice64_3 (iD(14564), "msg:value=%d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: msg:value=-1, -2, -3
        break; case __LINE__: TRice64_4 (iD(14565), "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: msg:value=-1, -2, -3, -4
        break; case __LINE__: Trice64_5 (iD(14566), "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5
        break; case __LINE__: trice64_6 (iD(14567), "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6
        break; case __LINE__: TRice64_7 (iD(14568), "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );                                            //exp: time:feed3322default: msg:value=-1, -2, -3, -4, -5, -6, -7
        break; case __LINE__: Trice64_8 (iD(14569), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8 
        break; case __LINE__: trice64_9 (iD(14570), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9
        break; case __LINE__: TRice64_10(iD(14571), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );                   //exp: time:feed3322default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10
        break; case __LINE__: Trice64_11(iD(14572), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11
        break; case __LINE__: trice64_12(iD(14573), "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: msg:value=-1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        // use cases for 8-bit data
        break; case __LINE__: trice8     (iD(14574), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: Trice8     (iD(14575), "value=%d\n", -2  ); //exp: time:    be16default: value=-2
        break; case __LINE__: TRice8     (iD(14576), "value=%d\n", -2  ); //exp: time:feed3322default: value=-2
        break; case __LINE__: trice8_1   (iD(14577), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: Trice8_1   (iD(14578), "value=%d\n", -2  ); //exp: time:    be16default: value=-2
        break; case __LINE__: TRice8_1   (iD(14579), "value=%d\n", -2  ); //exp: time:feed3322default: value=-2

        // use cases for 16-bit data
        break; case __LINE__: trice16    (iD(14580), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: Trice16    (iD(14581), "value=%d\n", -2  ); //exp: time:    be16default: value=-2
        break; case __LINE__: TRice16    (iD(14582), "value=%d\n", -2  ); //exp: time:feed3322default: value=-2
        break; case __LINE__: trice16_1  (iD(14583), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: Trice16_1  (iD(14584), "value=%d\n", -2  ); //exp: time:    be16default: value=-2
        break; case __LINE__: TRice16_1  (iD(14585), "value=%d\n", -2  ); //exp: time:feed3322default: value=-2

        // use cases for 32-bit data
        break; case __LINE__: trice32    (iD(14586), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: Trice32    (iD(14587), "value=%d\n", -2  ); //exp: time:    be16default: value=-2
        break; case __LINE__: TRice32    (iD(14588), "value=%d\n", -2  ); //exp: time:feed3322default: value=-2
        break; case __LINE__: trice32_1  (iD(14589), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: Trice32_1  (iD(14590), "value=%d\n", -2  ); //exp: time:    be16default: value=-2
        break; case __LINE__: TRice32_1  (iD(14591), "value=%d\n", -2  ); //exp: time:feed3322default: value=-2

        // use cases for 64-bit data
        break; case __LINE__: trice64    (iD(14592), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: Trice64    (iD(14593), "value=%d\n", -2  ); //exp: time:    be16default: value=-2
        break; case __LINE__: TRice64    (iD(14594), "value=%d\n", -2  ); //exp: time:feed3322default: value=-2
        break; case __LINE__: trice64_1  (iD(14595), "value=%d\n", -2  ); //exp: time:            default: value=-2
        break; case __LINE__: Trice64_1  (iD(14596), "value=%d\n", -2  ); //exp: time:    be16default: value=-2
        break; case __LINE__: TRice64_1  (iD(14597), "value=%d\n", -2  ); //exp: time:feed3322default: value=-2

        // legacy use cases
        break; case __LINE__: TRICE      (ID(14598), "value=%d\n", -2 ); //exp: time:feed3322default: value=-2
        break; case __LINE__: TRICE      (id(14599), "value=%d\n", -2 ); //exp: time:            default: value=-2
        break; case __LINE__: TRICE      (Id(14600), "value=%d\n", -2 ); //exp: time:    be16default: value=-2
        break; case __LINE__: TRICE      (ID(14601), "value=%d\n", -2 ); //exp: time:feed3322default: value=-2

        // default stamp
        break; case __LINE__: TRICE8     (ID(14602), "value=%d\n", -2 ); //exp: time:feed3322default: value=-2
        break; case __LINE__: TRICE8_1   (ID(14603), "value=%d\n", -2 ); //exp: time:feed3322default: value=-2

        // no stamp
        break; case __LINE__: trice8     (iD(14604), "value=%d\n", -2 ); //exp: time:            default: value=-2
        break; case __LINE__: trice8_1   (iD(14605), "value=%d\n", -2 ); //exp: time:            default: value=-2
        break; case __LINE__: TRICE8     (id(14606), "value=%d\n", -2 ); //exp: time:            default: value=-2
        break; case __LINE__: TRICE8_1   (id(14607), "value=%d\n", -2 ); //exp: time:            default: value=-2

        // 16-bit stamp:
        break; case __LINE__: Trice8     (iD(14608), "value=%d\n", -2 ); //exp: time:    be16default: value=-2
        break; case __LINE__: Trice8_1   (iD(14609), "value=%d\n", -2 ); //exp: time:    be16default: value=-2
        break; case __LINE__: TRICE8     (Id(14610), "value=%d\n", -2 ); //exp: time:    be16default: value=-2
        break; case __LINE__: TRICE8_1   (Id(14611), "value=%d\n", -2 ); //exp: time:    be16default: value=-2

        // 32-bit stamp:
        break; case __LINE__: TRice8     (iD(14612), "value=%d\n", -2 ); //exp: time:feed3322default: value=-2
        break; case __LINE__: TRice8_1   (iD(14613), "value=%d\n", -2 ); //exp: time:feed3322default: value=-2
        break; case __LINE__: TRICE8     (ID(14614), "value=%d\n", -2 ); //exp: time:feed3322default: value=-2
        break; case __LINE__: TRICE8_1   (ID(14615), "value=%d\n", -2 ); //exp: time:feed3322default: value=-2

        // channels
        break; case __LINE__: TRICE(id(14616), "FATAL:magenta+b:red\n" );           //exp: time:            default: FATAL:magenta+b:red
        break; case __LINE__: TRICE(id(14617), "CRITICAL:red+i:default+h\n" );      //exp: time:            default: CRITICAL:red+i:default+h
        break; case __LINE__: TRICE(id(14618), "EMERGENCY:red+i:blue\n" );          //exp: time:            default: EMERGENCY:red+i:blue
        break; case __LINE__: TRICE(id(14619), "ERROR:11:red\n" );                  //exp: time:            default: ERROR:11:red
        break; case __LINE__: TRICE(id(14620), "WARNING:11+i:red\n" );              //exp: time:            default: WARNING:11+i:red
        break; case __LINE__: TRICE(id(14621), "ATTENTION:11:green\n" );            //exp: time:            default: ATTENTION:11:green
        break; case __LINE__: TRICE(id(14622), "INFO:cyan+b:default+h\n" );         //exp: time:            default: INFO:cyan+b:default+h
        break; case __LINE__: TRICE(id(14623), "DEBUG:130+i\n" );                   //exp: time:            default: DEBUG:130+i
        break; case __LINE__: TRICE(id(14624), "TRACE:default+i:default+h\n" );     //exp: time:            default: TRACE:default+i:default+h
        break; case __LINE__: TRICE(id(14625), "TIME:blue+i:blue+h\n" );            //exp: time:            default: TIME:blue+i:blue+h
        break; case __LINE__: TRICE(id(14626), "MESSAGE:green+h:black\n" );         //exp: time:            default: MESSAGE:green+h:black
        break; case __LINE__: TRICE(id(14627), "READ:black+i:yellow+h\n" );         //exp: time:            default: READ:black+i:yellow+h
        break; case __LINE__: TRICE(id(14628), "WRITE:black+u:yellow+h\n" );        //exp: time:            default: WRITE:black+u:yellow+h
        break; case __LINE__: TRICE(id(14629), "RECEIVE:black+h:black\n" );         //exp: time:            default: RECEIVE:black+h:black
        break; case __LINE__: TRICE(id(14630), "TRANSMIT:black:black+h\n" );        //exp: time:            default: TRANSMIT:black:black+h
        break; case __LINE__: TRICE(id(14631), "DIAG:yellow+i:default+h\n" );       //exp: time:            default: DIAG:yellow+i:default+h
        break; case __LINE__: TRICE(id(14632), "INTERRUPT:magenta+i:default+h\n" ); //exp: time:            default: INTERRUPT:magenta+i:default+h
        break; case __LINE__: TRICE(id(14633), "SIGNAL:118+i\n" );                  //exp: time:            default: SIGNAL:118+i
        break; case __LINE__: TRICE(id(14634), "TEST:yellow+h:black\n" );           //exp: time:            default: TEST:yellow+h:black
        break; case __LINE__: TRICE(id(14635), "DEFAULT:off\n" );                   //exp: time:            default: DEFAULT:off
        break; case __LINE__: TRICE(id(14636), "NOTICE:blue:white+h\n" );           //exp: time:            default: NOTICE:blue:white+h
        break; case __LINE__: TRICE(id(14637), "ALERT:magenta:magenta+h\n" );       //exp: time:            default: ALERT:magenta:magenta+h
        break; case __LINE__: TRICE(id(14638), "ASSERT:yellow+i:blue\n" );          //exp: time:            default: ASSERT:yellow+i:blue
        break; case __LINE__: TRICE(id(14639), "ALARM:red+i:white+h\n" );           //exp: time:            default: ALARM:red+i:white+h
        break; case __LINE__: TRICE(id(14640), "CYCLE:blue+i:default+h\n" );        //exp: time:            default: CYCLE:blue+i:default+h
        break; case __LINE__: TRICE(id(14641), "VERBOSE:blue:default\n" );          //exp: time:            default: VERBOSE:blue:default
        break; case __LINE__: TRICE(id(14642), "fatal:magenta+b:red\n" );           //exp: time:            default: fatal:magenta+b:red
        break; case __LINE__: TRICE(id(14643), "critical:red+i:default+h\n" );      //exp: time:            default: critical:red+i:default+h
        break; case __LINE__: TRICE(id(14644), "emergency:red+i:blue\n" );          //exp: time:            default: emergency:red+i:blue
        break; case __LINE__: TRICE(id(14645), "error:11:red\n" );                  //exp: time:            default: error:11:red
        break; case __LINE__: TRICE(id(14646), "warning:11+i:red\n" );              //exp: time:            default: warning:11+i:red
        break; case __LINE__: TRICE(id(14647), "attention:11:green\n" );            //exp: time:            default: attention:11:green
        break; case __LINE__: TRICE(id(14648), "info:cyan+b:default+h\n" );         //exp: time:            default: info:cyan+b:default+h
        break; case __LINE__: TRICE(id(14649), "debug:130+i\n" );                   //exp: time:            default: debug:130+i
        break; case __LINE__: TRICE(id(14650), "trace:default+i:default+h\n" );     //exp: time:            default: trace:default+i:default+h
        break; case __LINE__: TRICE(id(14651), "time:blue+i:blue+h\n" );            //exp: time:            default: time:blue+i:blue+h
        break; case __LINE__: TRICE(id(14652), "message:green+h:black\n" );         //exp: time:            default: message:green+h:black
        break; case __LINE__: TRICE(id(14653), "read:black+i:yellow+h\n" );         //exp: time:            default: read:black+i:yellow+h
        break; case __LINE__: TRICE(id(14654), "write:black+u:yellow+h\n" );        //exp: time:            default: write:black+u:yellow+h
        break; case __LINE__: TRICE(id(14655), "receive:black+h:black\n" );         //exp: time:            default: receive:black+h:black
        break; case __LINE__: TRICE(id(14656), "transmit:black:black+h\n" );        //exp: time:            default: transmit:black:black+h
        break; case __LINE__: TRICE(id(14657), "diag:yellow+i:default+h\n" );       //exp: time:            default: diag:yellow+i:default+h
        break; case __LINE__: TRICE(id(14658), "interrupt:magenta+i:default+h\n" ); //exp: time:            default: interrupt:magenta+i:default+h
        break; case __LINE__: TRICE(id(14659), "signal:118+i\n" );                  //exp: time:            default: signal:118+i
        break; case __LINE__: TRICE(id(14660), "test:yellow+h:black\n" );           //exp: time:            default: test:yellow+h:black
        break; case __LINE__: TRICE(id(14661), "default:off\n" );                   //exp: time:            default: default:off
        break; case __LINE__: TRICE(id(14662), "notice:blue:white+h\n" );           //exp: time:            default: notice:blue:white+h
        break; case __LINE__: TRICE(id(14663), "alert:magenta:magenta+h\n" );       //exp: time:            default: alert:magenta:magenta+h
        break; case __LINE__: TRICE(id(14664), "assert:yellow+i:blue\n" );          //exp: time:            default: assert:yellow+i:blue
        break; case __LINE__: TRICE(id(14665), "alarm:red+i:white+h\n" );           //exp: time:            default: alarm:red+i:white+h
        break; case __LINE__: TRICE(id(14666), "cycle:blue+i:default+h\n" );        //exp: time:            default: cycle:blue+i:default+h
        break; case __LINE__: TRICE(id(14667), "verbose:blue:default\n" );          //exp: time:            default: verbose:blue:default
        
        break; case __LINE__: trice(iD(14668), "FATAL:magenta+b:red\n" );           //exp: time:            default: FATAL:magenta+b:red
        break; case __LINE__: trice(iD(14669), "CRITICAL:red+i:default+h\n" );      //exp: time:            default: CRITICAL:red+i:default+h
        break; case __LINE__: trice(iD(14670), "EMERGENCY:red+i:blue\n" );          //exp: time:            default: EMERGENCY:red+i:blue
        break; case __LINE__: trice(iD(14671), "ERROR:11:red\n" );                  //exp: time:            default: ERROR:11:red
        break; case __LINE__: trice(iD(14672), "WARNING:11+i:red\n" );              //exp: time:            default: WARNING:11+i:red
        break; case __LINE__: trice(iD(14673), "ATTENTION:11:green\n" );            //exp: time:            default: ATTENTION:11:green
        break; case __LINE__: trice(iD(14674), "INFO:cyan+b:default+h\n" );         //exp: time:            default: INFO:cyan+b:default+h
        break; case __LINE__: trice(iD(14675), "DEBUG:130+i\n" );                   //exp: time:            default: DEBUG:130+i
        break; case __LINE__: trice(iD(14676), "TRACE:default+i:default+h\n" );     //exp: time:            default: TRACE:default+i:default+h
        break; case __LINE__: trice(iD(14677), "TIME:blue+i:blue+h\n" );            //exp: time:            default: TIME:blue+i:blue+h
        break; case __LINE__: trice(iD(14678), "MESSAGE:green+h:black\n" );         //exp: time:            default: MESSAGE:green+h:black
        break; case __LINE__: trice(iD(14679), "READ:black+i:yellow+h\n" );         //exp: time:            default: READ:black+i:yellow+h
        break; case __LINE__: trice(iD(14680), "WRITE:black+u:yellow+h\n" );        //exp: time:            default: WRITE:black+u:yellow+h
        break; case __LINE__: trice(iD(14681), "RECEIVE:black+h:black\n" );         //exp: time:            default: RECEIVE:black+h:black
        break; case __LINE__: trice(iD(14682), "TRANSMIT:black:black+h\n" );        //exp: time:            default: TRANSMIT:black:black+h
        break; case __LINE__: trice(iD(14683), "DIAG:yellow+i:default+h\n" );       //exp: time:            default: DIAG:yellow+i:default+h
        break; case __LINE__: trice(iD(14684), "INTERRUPT:magenta+i:default+h\n" ); //exp: time:            default: INTERRUPT:magenta+i:default+h
        break; case __LINE__: trice(iD(14685), "SIGNAL:118+i\n" );                  //exp: time:            default: SIGNAL:118+i
        break; case __LINE__: trice(iD(14686), "TEST:yellow+h:black\n" );           //exp: time:            default: TEST:yellow+h:black
        break; case __LINE__: trice(iD(14687), "DEFAULT:off\n" );                   //exp: time:            default: DEFAULT:off
        break; case __LINE__: trice(iD(14688), "NOTICE:blue:white+h\n" );           //exp: time:            default: NOTICE:blue:white+h
        break; case __LINE__: trice(iD(14689), "ALERT:magenta:magenta+h\n" );       //exp: time:            default: ALERT:magenta:magenta+h
        break; case __LINE__: trice(iD(14690), "ASSERT:yellow+i:blue\n" );          //exp: time:            default: ASSERT:yellow+i:blue
        break; case __LINE__: trice(iD(14691), "ALARM:red+i:white+h\n" );           //exp: time:            default: ALARM:red+i:white+h
        break; case __LINE__: trice(iD(14692), "CYCLE:blue+i:default+h\n" );        //exp: time:            default: CYCLE:blue+i:default+h
        break; case __LINE__: trice(iD(14693), "VERBOSE:blue:default\n" );          //exp: time:            default: VERBOSE:blue:default
        break; case __LINE__: trice(iD(14694), "fatal:magenta+b:red\n" );           //exp: time:            default: fatal:magenta+b:red
        break; case __LINE__: trice(iD(14695), "critical:red+i:default+h\n" );      //exp: time:            default: critical:red+i:default+h
        break; case __LINE__: trice(iD(14696), "emergency:red+i:blue\n" );          //exp: time:            default: emergency:red+i:blue
        break; case __LINE__: trice(iD(14697), "error:11:red\n" );                  //exp: time:            default: error:11:red
        break; case __LINE__: trice(iD(14698), "warning:11+i:red\n" );              //exp: time:            default: warning:11+i:red
        break; case __LINE__: trice(iD(14699), "attention:11:green\n" );            //exp: time:            default: attention:11:green
        break; case __LINE__: trice(iD(14700), "info:cyan+b:default+h\n" );         //exp: time:            default: info:cyan+b:default+h
        break; case __LINE__: trice(iD(14701), "debug:130+i\n" );                   //exp: time:            default: debug:130+i
        break; case __LINE__: trice(iD(14702), "trace:default+i:default+h\n" );     //exp: time:            default: trace:default+i:default+h
        break; case __LINE__: trice(iD(14703), "time:blue+i:blue+h\n" );            //exp: time:            default: time:blue+i:blue+h
        break; case __LINE__: trice(iD(14704), "message:green+h:black\n" );         //exp: time:            default: message:green+h:black
        break; case __LINE__: trice(iD(14705), "read:black+i:yellow+h\n" );         //exp: time:            default: read:black+i:yellow+h
        break; case __LINE__: trice(iD(14706), "write:black+u:yellow+h\n" );        //exp: time:            default: write:black+u:yellow+h
        break; case __LINE__: trice(iD(14707), "receive:black+h:black\n" );         //exp: time:            default: receive:black+h:black
        break; case __LINE__: trice(iD(14708), "transmit:black:black+h\n" );        //exp: time:            default: transmit:black:black+h
        break; case __LINE__: trice(iD(14709), "diag:yellow+i:default+h\n" );       //exp: time:            default: diag:yellow+i:default+h
        break; case __LINE__: trice(iD(14710), "interrupt:magenta+i:default+h\n" ); //exp: time:            default: interrupt:magenta+i:default+h
        break; case __LINE__: trice(iD(14711), "signal:118+i\n" );                  //exp: time:            default: signal:118+i
        break; case __LINE__: trice(iD(14712), "test:yellow+h:black\n" );           //exp: time:            default: test:yellow+h:black
        break; case __LINE__: trice(iD(14713), "default:off\n" );                   //exp: time:            default: default:off
        break; case __LINE__: trice(iD(14714), "notice:blue:white+h\n" );           //exp: time:            default: notice:blue:white+h
        break; case __LINE__: trice(iD(14715), "alert:magenta:magenta+h\n" );       //exp: time:            default: alert:magenta:magenta+h
        break; case __LINE__: trice(iD(14716), "assert:yellow+i:blue\n" );          //exp: time:            default: assert:yellow+i:blue
        break; case __LINE__: trice(iD(14717), "alarm:red+i:white+h\n" );           //exp: time:            default: alarm:red+i:white+h
        break; case __LINE__: trice(iD(14718), "cycle:blue+i:default+h\n" );        //exp: time:            default: cycle:blue+i:default+h
        break; case __LINE__: trice(iD(14719), "verbose:blue:default\n" );          //exp: time:            default: verbose:blue:default

        break; case __LINE__: TRICE(Id(14720), "FATAL:magenta+b:red\n" );           //exp: time:    be16default: FATAL:magenta+b:red
        break; case __LINE__: TRICE(Id(14721), "CRITICAL:red+i:default+h\n" );      //exp: time:    be16default: CRITICAL:red+i:default+h
        break; case __LINE__: TRICE(Id(14722), "EMERGENCY:red+i:blue\n" );          //exp: time:    be16default: EMERGENCY:red+i:blue
        break; case __LINE__: TRICE(Id(14723), "ERROR:11:red\n" );                  //exp: time:    be16default: ERROR:11:red
        break; case __LINE__: TRICE(Id(14724), "WARNING:11+i:red\n" );              //exp: time:    be16default: WARNING:11+i:red
        break; case __LINE__: TRICE(Id(14725), "ATTENTION:11:green\n" );            //exp: time:    be16default: ATTENTION:11:green
        break; case __LINE__: TRICE(Id(14726), "INFO:cyan+b:default+h\n" );         //exp: time:    be16default: INFO:cyan+b:default+h
        break; case __LINE__: TRICE(Id(14727), "DEBUG:130+i\n" );                   //exp: time:    be16default: DEBUG:130+i
        break; case __LINE__: TRICE(Id(14728), "TRACE:default+i:default+h\n" );     //exp: time:    be16default: TRACE:default+i:default+h
        break; case __LINE__: TRICE(Id(14729), "TIME:blue+i:blue+h\n" );            //exp: time:    be16default: TIME:blue+i:blue+h
        break; case __LINE__: TRICE(Id(14730), "MESSAGE:green+h:black\n" );         //exp: time:    be16default: MESSAGE:green+h:black
        break; case __LINE__: TRICE(Id(14731), "READ:black+i:yellow+h\n" );         //exp: time:    be16default: READ:black+i:yellow+h
        break; case __LINE__: TRICE(Id(14732), "WRITE:black+u:yellow+h\n" );        //exp: time:    be16default: WRITE:black+u:yellow+h
        break; case __LINE__: TRICE(Id(14733), "RECEIVE:black+h:black\n" );         //exp: time:    be16default: RECEIVE:black+h:black
        break; case __LINE__: TRICE(Id(14734), "TRANSMIT:black:black+h\n" );        //exp: time:    be16default: TRANSMIT:black:black+h
        break; case __LINE__: TRICE(Id(14735), "DIAG:yellow+i:default+h\n" );       //exp: time:    be16default: DIAG:yellow+i:default+h
        break; case __LINE__: TRICE(Id(14736), "INTERRUPT:magenta+i:default+h\n" ); //exp: time:    be16default: INTERRUPT:magenta+i:default+h
        break; case __LINE__: TRICE(Id(14737), "SIGNAL:118+i\n" );                  //exp: time:    be16default: SIGNAL:118+i
        break; case __LINE__: TRICE(Id(14738), "TEST:yellow+h:black\n" );           //exp: time:    be16default: TEST:yellow+h:black
        break; case __LINE__: TRICE(Id(14739), "DEFAULT:off\n" );                   //exp: time:    be16default: DEFAULT:off
        break; case __LINE__: TRICE(Id(14740), "NOTICE:blue:white+h\n" );           //exp: time:    be16default: NOTICE:blue:white+h
        break; case __LINE__: TRICE(Id(14741), "ALERT:magenta:magenta+h\n" );       //exp: time:    be16default: ALERT:magenta:magenta+h
        break; case __LINE__: TRICE(Id(14742), "ASSERT:yellow+i:blue\n" );          //exp: time:    be16default: ASSERT:yellow+i:blue
        break; case __LINE__: TRICE(Id(14743), "ALARM:red+i:white+h\n" );           //exp: time:    be16default: ALARM:red+i:white+h
        break; case __LINE__: TRICE(Id(14744), "CYCLE:blue+i:default+h\n" );        //exp: time:    be16default: CYCLE:blue+i:default+h
        break; case __LINE__: TRICE(Id(14745), "VERBOSE:blue:default\n" );          //exp: time:    be16default: VERBOSE:blue:default
        break; case __LINE__: TRICE(Id(14746), "fatal:magenta+b:red\n" );           //exp: time:    be16default: fatal:magenta+b:red
        break; case __LINE__: TRICE(Id(14747), "critical:red+i:default+h\n" );      //exp: time:    be16default: critical:red+i:default+h
        break; case __LINE__: TRICE(Id(14748), "emergency:red+i:blue\n" );          //exp: time:    be16default: emergency:red+i:blue
        break; case __LINE__: TRICE(Id(14749), "error:11:red\n" );                  //exp: time:    be16default: error:11:red
        break; case __LINE__: TRICE(Id(14750), "warning:11+i:red\n" );              //exp: time:    be16default: warning:11+i:red
        break; case __LINE__: TRICE(Id(14751), "attention:11:green\n" );            //exp: time:    be16default: attention:11:green
        break; case __LINE__: TRICE(Id(14752), "info:cyan+b:default+h\n" );         //exp: time:    be16default: info:cyan+b:default+h
        break; case __LINE__: TRICE(Id(14753), "debug:130+i\n" );                   //exp: time:    be16default: debug:130+i
        break; case __LINE__: TRICE(Id(14754), "trace:default+i:default+h\n" );     //exp: time:    be16default: trace:default+i:default+h
        break; case __LINE__: TRICE(Id(14755), "time:blue+i:blue+h\n" );            //exp: time:    be16default: time:blue+i:blue+h
        break; case __LINE__: TRICE(Id(14756), "message:green+h:black\n" );         //exp: time:    be16default: message:green+h:black
        break; case __LINE__: TRICE(Id(14757), "read:black+i:yellow+h\n" );         //exp: time:    be16default: read:black+i:yellow+h
        break; case __LINE__: TRICE(Id(14758), "write:black+u:yellow+h\n" );        //exp: time:    be16default: write:black+u:yellow+h
        break; case __LINE__: TRICE(Id(14759), "receive:black+h:black\n" );         //exp: time:    be16default: receive:black+h:black
        break; case __LINE__: TRICE(Id(14760), "transmit:black:black+h\n" );        //exp: time:    be16default: transmit:black:black+h
        break; case __LINE__: TRICE(Id(14761), "diag:yellow+i:default+h\n" );       //exp: time:    be16default: diag:yellow+i:default+h
        break; case __LINE__: TRICE(Id(14762), "interrupt:magenta+i:default+h\n" ); //exp: time:    be16default: interrupt:magenta+i:default+h
        break; case __LINE__: TRICE(Id(14763), "signal:118+i\n" );                  //exp: time:    be16default: signal:118+i
        break; case __LINE__: TRICE(Id(14764), "test:yellow+h:black\n" );           //exp: time:    be16default: test:yellow+h:black
        break; case __LINE__: TRICE(Id(14765), "default:off\n" );                   //exp: time:    be16default: default:off
        break; case __LINE__: TRICE(Id(14766), "notice:blue:white+h\n" );           //exp: time:    be16default: notice:blue:white+h
        break; case __LINE__: TRICE(Id(14767), "alert:magenta:magenta+h\n" );       //exp: time:    be16default: alert:magenta:magenta+h
        break; case __LINE__: TRICE(Id(14768), "assert:yellow+i:blue\n" );          //exp: time:    be16default: assert:yellow+i:blue
        break; case __LINE__: TRICE(Id(14769), "alarm:red+i:white+h\n" );           //exp: time:    be16default: alarm:red+i:white+h
        break; case __LINE__: TRICE(Id(14770), "cycle:blue+i:default+h\n" );        //exp: time:    be16default: cycle:blue+i:default+h
        break; case __LINE__: TRICE(Id(14771), "verbose:blue:default\n" );          //exp: time:    be16default: verbose:blue:default

        break; case __LINE__: Trice(iD(14772), "FATAL:magenta+b:red\n" );           //exp: time:    be16default: FATAL:magenta+b:red
        break; case __LINE__: Trice(iD(14773), "CRITICAL:red+i:default+h\n" );      //exp: time:    be16default: CRITICAL:red+i:default+h
        break; case __LINE__: Trice(iD(14774), "EMERGENCY:red+i:blue\n" );          //exp: time:    be16default: EMERGENCY:red+i:blue
        break; case __LINE__: Trice(iD(14775), "ERROR:11:red\n" );                  //exp: time:    be16default: ERROR:11:red
        break; case __LINE__: Trice(iD(14776), "WARNING:11+i:red\n" );              //exp: time:    be16default: WARNING:11+i:red
        break; case __LINE__: Trice(iD(14777), "ATTENTION:11:green\n" );            //exp: time:    be16default: ATTENTION:11:green
        break; case __LINE__: Trice(iD(14778), "INFO:cyan+b:default+h\n" );         //exp: time:    be16default: INFO:cyan+b:default+h
        break; case __LINE__: Trice(iD(14779), "DEBUG:130+i\n" );                   //exp: time:    be16default: DEBUG:130+i
        break; case __LINE__: Trice(iD(14780), "TRACE:default+i:default+h\n" );     //exp: time:    be16default: TRACE:default+i:default+h
        break; case __LINE__: Trice(iD(14781), "TIME:blue+i:blue+h\n" );            //exp: time:    be16default: TIME:blue+i:blue+h
        break; case __LINE__: Trice(iD(14782), "MESSAGE:green+h:black\n" );         //exp: time:    be16default: MESSAGE:green+h:black
        break; case __LINE__: Trice(iD(14783), "READ:black+i:yellow+h\n" );         //exp: time:    be16default: READ:black+i:yellow+h
        break; case __LINE__: Trice(iD(14784), "WRITE:black+u:yellow+h\n" );        //exp: time:    be16default: WRITE:black+u:yellow+h
        break; case __LINE__: Trice(iD(14785), "RECEIVE:black+h:black\n" );         //exp: time:    be16default: RECEIVE:black+h:black
        break; case __LINE__: Trice(iD(14786), "TRANSMIT:black:black+h\n" );        //exp: time:    be16default: TRANSMIT:black:black+h
        break; case __LINE__: Trice(iD(14787), "DIAG:yellow+i:default+h\n" );       //exp: time:    be16default: DIAG:yellow+i:default+h
        break; case __LINE__: Trice(iD(14788), "INTERRUPT:magenta+i:default+h\n" ); //exp: time:    be16default: INTERRUPT:magenta+i:default+h
        break; case __LINE__: Trice(iD(14789), "SIGNAL:118+i\n" );                  //exp: time:    be16default: SIGNAL:118+i
        break; case __LINE__: Trice(iD(14790), "TEST:yellow+h:black\n" );           //exp: time:    be16default: TEST:yellow+h:black
        break; case __LINE__: Trice(iD(14791), "DEFAULT:off\n" );                   //exp: time:    be16default: DEFAULT:off
        break; case __LINE__: Trice(iD(14792), "NOTICE:blue:white+h\n" );           //exp: time:    be16default: NOTICE:blue:white+h
        break; case __LINE__: Trice(iD(14793), "ALERT:magenta:magenta+h\n" );       //exp: time:    be16default: ALERT:magenta:magenta+h
        break; case __LINE__: Trice(iD(14794), "ASSERT:yellow+i:blue\n" );          //exp: time:    be16default: ASSERT:yellow+i:blue
        break; case __LINE__: Trice(iD(14795), "ALARM:red+i:white+h\n" );           //exp: time:    be16default: ALARM:red+i:white+h
        break; case __LINE__: Trice(iD(14796), "CYCLE:blue+i:default+h\n" );        //exp: time:    be16default: CYCLE:blue+i:default+h
        break; case __LINE__: Trice(iD(14797), "VERBOSE:blue:default\n" );          //exp: time:    be16default: VERBOSE:blue:default
        break; case __LINE__: Trice(iD(14798), "fatal:magenta+b:red\n" );           //exp: time:    be16default: fatal:magenta+b:red
        break; case __LINE__: Trice(iD(14799), "critical:red+i:default+h\n" );      //exp: time:    be16default: critical:red+i:default+h
        break; case __LINE__: Trice(iD(14800), "emergency:red+i:blue\n" );          //exp: time:    be16default: emergency:red+i:blue
        break; case __LINE__: Trice(iD(14801), "error:11:red\n" );                  //exp: time:    be16default: error:11:red
        break; case __LINE__: Trice(iD(14802), "warning:11+i:red\n" );              //exp: time:    be16default: warning:11+i:red
        break; case __LINE__: Trice(iD(14803), "attention:11:green\n" );            //exp: time:    be16default: attention:11:green
        break; case __LINE__: Trice(iD(14804), "info:cyan+b:default+h\n" );         //exp: time:    be16default: info:cyan+b:default+h
        break; case __LINE__: Trice(iD(14805), "debug:130+i\n" );                   //exp: time:    be16default: debug:130+i
        break; case __LINE__: Trice(iD(14806), "trace:default+i:default+h\n" );     //exp: time:    be16default: trace:default+i:default+h
        break; case __LINE__: Trice(iD(14807), "time:blue+i:blue+h\n" );            //exp: time:    be16default: time:blue+i:blue+h
        break; case __LINE__: Trice(iD(14808), "message:green+h:black\n" );         //exp: time:    be16default: message:green+h:black
        break; case __LINE__: Trice(iD(14809), "read:black+i:yellow+h\n" );         //exp: time:    be16default: read:black+i:yellow+h
        break; case __LINE__: Trice(iD(14810), "write:black+u:yellow+h\n" );        //exp: time:    be16default: write:black+u:yellow+h
        break; case __LINE__: Trice(iD(14811), "receive:black+h:black\n" );         //exp: time:    be16default: receive:black+h:black
        break; case __LINE__: Trice(iD(14812), "transmit:black:black+h\n" );        //exp: time:    be16default: transmit:black:black+h
        break; case __LINE__: Trice(iD(14813), "diag:yellow+i:default+h\n" );       //exp: time:    be16default: diag:yellow+i:default+h
        break; case __LINE__: Trice(iD(14814), "interrupt:magenta+i:default+h\n" ); //exp: time:    be16default: interrupt:magenta+i:default+h
        break; case __LINE__: Trice(iD(14815), "signal:118+i\n" );                  //exp: time:    be16default: signal:118+i
        break; case __LINE__: Trice(iD(14816), "test:yellow+h:black\n" );           //exp: time:    be16default: test:yellow+h:black
        break; case __LINE__: Trice(iD(14817), "default:off\n" );                   //exp: time:    be16default: default:off
        break; case __LINE__: Trice(iD(14818), "notice:blue:white+h\n" );           //exp: time:    be16default: notice:blue:white+h
        break; case __LINE__: Trice(iD(14819), "alert:magenta:magenta+h\n" );       //exp: time:    be16default: alert:magenta:magenta+h
        break; case __LINE__: Trice(iD(14820), "assert:yellow+i:blue\n" );          //exp: time:    be16default: assert:yellow+i:blue
        break; case __LINE__: Trice(iD(14821), "alarm:red+i:white+h\n" );           //exp: time:    be16default: alarm:red+i:white+h
        break; case __LINE__: Trice(iD(14822), "cycle:blue+i:default+h\n" );        //exp: time:    be16default: cycle:blue+i:default+h
        break; case __LINE__: Trice(iD(14823), "verbose:blue:default\n" );          //exp: time:    be16default: verbose:blue:default

        break; case __LINE__: TRICE(ID(14824), "FATAL:magenta+b:red\n" );           //exp: time:feed3322default: FATAL:magenta+b:red
        break; case __LINE__: TRICE(ID(14825), "CRITICAL:red+i:default+h\n" );      //exp: time:feed3322default: CRITICAL:red+i:default+h
        break; case __LINE__: TRICE(ID(14826), "EMERGENCY:red+i:blue\n" );          //exp: time:feed3322default: EMERGENCY:red+i:blue
        break; case __LINE__: TRICE(ID(14827), "ERROR:11:red\n" );                  //exp: time:feed3322default: ERROR:11:red
        break; case __LINE__: TRICE(ID(14828), "WARNING:11+i:red\n" );              //exp: time:feed3322default: WARNING:11+i:red
        break; case __LINE__: TRICE(ID(14829), "ATTENTION:11:green\n" );            //exp: time:feed3322default: ATTENTION:11:green
        break; case __LINE__: TRICE(ID(14830), "INFO:cyan+b:default+h\n" );         //exp: time:feed3322default: INFO:cyan+b:default+h
        break; case __LINE__: TRICE(ID(14831), "DEBUG:130+i\n" );                   //exp: time:feed3322default: DEBUG:130+i
        break; case __LINE__: TRICE(ID(14832), "TRACE:default+i:default+h\n" );     //exp: time:feed3322default: TRACE:default+i:default+h
        break; case __LINE__: TRICE(ID(14833), "TIME:blue+i:blue+h\n" );            //exp: time:feed3322default: TIME:blue+i:blue+h
        break; case __LINE__: TRICE(ID(14834), "MESSAGE:green+h:black\n" );         //exp: time:feed3322default: MESSAGE:green+h:black
        break; case __LINE__: TRICE(ID(14835), "READ:black+i:yellow+h\n" );         //exp: time:feed3322default: READ:black+i:yellow+h
        break; case __LINE__: TRICE(ID(14836), "WRITE:black+u:yellow+h\n" );        //exp: time:feed3322default: WRITE:black+u:yellow+h
        break; case __LINE__: TRICE(ID(14837), "RECEIVE:black+h:black\n" );         //exp: time:feed3322default: RECEIVE:black+h:black
        break; case __LINE__: TRICE(ID(14838), "TRANSMIT:black:black+h\n" );        //exp: time:feed3322default: TRANSMIT:black:black+h
        break; case __LINE__: TRICE(ID(14839), "DIAG:yellow+i:default+h\n" );       //exp: time:feed3322default: DIAG:yellow+i:default+h
        break; case __LINE__: TRICE(ID(14840), "INTERRUPT:magenta+i:default+h\n" ); //exp: time:feed3322default: INTERRUPT:magenta+i:default+h
        break; case __LINE__: TRICE(ID(14841), "SIGNAL:118+i\n" );                  //exp: time:feed3322default: SIGNAL:118+i
        break; case __LINE__: TRICE(ID(14842), "TEST:yellow+h:black\n" );           //exp: time:feed3322default: TEST:yellow+h:black
        break; case __LINE__: TRICE(ID(14843), "DEFAULT:off\n" );                   //exp: time:feed3322default: DEFAULT:off
        break; case __LINE__: TRICE(ID(14844), "NOTICE:blue:white+h\n" );           //exp: time:feed3322default: NOTICE:blue:white+h
        break; case __LINE__: TRICE(ID(14845), "ALERT:magenta:magenta+h\n" );       //exp: time:feed3322default: ALERT:magenta:magenta+h
        break; case __LINE__: TRICE(ID(14846), "ASSERT:yellow+i:blue\n" );          //exp: time:feed3322default: ASSERT:yellow+i:blue
        break; case __LINE__: TRICE(ID(14847), "ALARM:red+i:white+h\n" );           //exp: time:feed3322default: ALARM:red+i:white+h
        break; case __LINE__: TRICE(ID(14848), "CYCLE:blue+i:default+h\n" );        //exp: time:feed3322default: CYCLE:blue+i:default+h
        break; case __LINE__: TRICE(ID(14849), "VERBOSE:blue:default\n" );          //exp: time:feed3322default: VERBOSE:blue:default
        break; case __LINE__: TRICE(ID(14850), "fatal:magenta+b:red\n" );           //exp: time:feed3322default: fatal:magenta+b:red
        break; case __LINE__: TRICE(ID(14851), "critical:red+i:default+h\n" );      //exp: time:feed3322default: critical:red+i:default+h
        break; case __LINE__: TRICE(ID(14852), "emergency:red+i:blue\n" );          //exp: time:feed3322default: emergency:red+i:blue
        break; case __LINE__: TRICE(ID(14853), "error:11:red\n" );                  //exp: time:feed3322default: error:11:red
        break; case __LINE__: TRICE(ID(14854), "warning:11+i:red\n" );              //exp: time:feed3322default: warning:11+i:red
        break; case __LINE__: TRICE(ID(14855), "attention:11:green\n" );            //exp: time:feed3322default: attention:11:green
        break; case __LINE__: TRICE(ID(14856), "info:cyan+b:default+h\n" );         //exp: time:feed3322default: info:cyan+b:default+h
        break; case __LINE__: TRICE(ID(14857), "debug:130+i\n" );                   //exp: time:feed3322default: debug:130+i
        break; case __LINE__: TRICE(ID(14858), "trace:default+i:default+h\n" );     //exp: time:feed3322default: trace:default+i:default+h
        break; case __LINE__: TRICE(ID(14859), "time:blue+i:blue+h\n" );            //exp: time:feed3322default: time:blue+i:blue+h
        break; case __LINE__: TRICE(ID(14860), "message:green+h:black\n" );         //exp: time:feed3322default: message:green+h:black
        break; case __LINE__: TRICE(ID(14861), "read:black+i:yellow+h\n" );         //exp: time:feed3322default: read:black+i:yellow+h
        break; case __LINE__: TRICE(ID(14862), "write:black+u:yellow+h\n" );        //exp: time:feed3322default: write:black+u:yellow+h
        break; case __LINE__: TRICE(ID(14863), "receive:black+h:black\n" );         //exp: time:feed3322default: receive:black+h:black
        break; case __LINE__: TRICE(ID(14864), "transmit:black:black+h\n" );        //exp: time:feed3322default: transmit:black:black+h
        break; case __LINE__: TRICE(ID(14865), "diag:yellow+i:default+h\n" );       //exp: time:feed3322default: diag:yellow+i:default+h
        break; case __LINE__: TRICE(ID(14866), "interrupt:magenta+i:default+h\n" ); //exp: time:feed3322default: interrupt:magenta+i:default+h
        break; case __LINE__: TRICE(ID(14867), "signal:118+i\n" );                  //exp: time:feed3322default: signal:118+i
        break; case __LINE__: TRICE(ID(14868), "test:yellow+h:black\n" );           //exp: time:feed3322default: test:yellow+h:black
        break; case __LINE__: TRICE(ID(14869), "default:off\n" );                   //exp: time:feed3322default: default:off
        break; case __LINE__: TRICE(ID(14870), "notice:blue:white+h\n" );           //exp: time:feed3322default: notice:blue:white+h
        break; case __LINE__: TRICE(ID(14871), "alert:magenta:magenta+h\n" );       //exp: time:feed3322default: alert:magenta:magenta+h
        break; case __LINE__: TRICE(ID(14872), "assert:yellow+i:blue\n" );          //exp: time:feed3322default: assert:yellow+i:blue
        break; case __LINE__: TRICE(ID(14873), "alarm:red+i:white+h\n" );           //exp: time:feed3322default: alarm:red+i:white+h
        break; case __LINE__: TRICE(ID(14874), "cycle:blue+i:default+h\n" );        //exp: time:feed3322default: cycle:blue+i:default+h
        break; case __LINE__: TRICE(ID(14875), "verbose:blue:default\n" );          //exp: time:feed3322default: verbose:blue:default
        
        break; case __LINE__: TRice(iD(14876), "FATAL:magenta+b:red\n" );           //exp: time:feed3322default: FATAL:magenta+b:red
        break; case __LINE__: TRice(iD(14877), "CRITICAL:red+i:default+h\n" );      //exp: time:feed3322default: CRITICAL:red+i:default+h
        break; case __LINE__: TRice(iD(14878), "EMERGENCY:red+i:blue\n" );          //exp: time:feed3322default: EMERGENCY:red+i:blue
        break; case __LINE__: TRice(iD(14879), "ERROR:11:red\n" );                  //exp: time:feed3322default: ERROR:11:red
        break; case __LINE__: TRice(iD(14880), "WARNING:11+i:red\n" );              //exp: time:feed3322default: WARNING:11+i:red
        break; case __LINE__: TRice(iD(14881), "ATTENTION:11:green\n" );            //exp: time:feed3322default: ATTENTION:11:green
        break; case __LINE__: TRice(iD(14882), "INFO:cyan+b:default+h\n" );         //exp: time:feed3322default: INFO:cyan+b:default+h
        break; case __LINE__: TRice(iD(14883), "DEBUG:130+i\n" );                   //exp: time:feed3322default: DEBUG:130+i
        break; case __LINE__: TRice(iD(14884), "TRACE:default+i:default+h\n" );     //exp: time:feed3322default: TRACE:default+i:default+h
        break; case __LINE__: TRice(iD(14885), "TIME:blue+i:blue+h\n" );            //exp: time:feed3322default: TIME:blue+i:blue+h
        break; case __LINE__: TRice(iD(14886), "MESSAGE:green+h:black\n" );         //exp: time:feed3322default: MESSAGE:green+h:black
        break; case __LINE__: TRice(iD(14887), "READ:black+i:yellow+h\n" );         //exp: time:feed3322default: READ:black+i:yellow+h
        break; case __LINE__: TRice(iD(14888), "WRITE:black+u:yellow+h\n" );        //exp: time:feed3322default: WRITE:black+u:yellow+h
        break; case __LINE__: TRice(iD(14889), "RECEIVE:black+h:black\n" );         //exp: time:feed3322default: RECEIVE:black+h:black
        break; case __LINE__: TRice(iD(14890), "TRANSMIT:black:black+h\n" );        //exp: time:feed3322default: TRANSMIT:black:black+h
        break; case __LINE__: TRice(iD(14891), "DIAG:yellow+i:default+h\n" );       //exp: time:feed3322default: DIAG:yellow+i:default+h
        break; case __LINE__: TRice(iD(14892), "INTERRUPT:magenta+i:default+h\n" ); //exp: time:feed3322default: INTERRUPT:magenta+i:default+h
        break; case __LINE__: TRice(iD(14893), "SIGNAL:118+i\n" );                  //exp: time:feed3322default: SIGNAL:118+i
        break; case __LINE__: TRice(iD(14894), "TEST:yellow+h:black\n" );           //exp: time:feed3322default: TEST:yellow+h:black
        break; case __LINE__: TRice(iD(14895), "DEFAULT:off\n" );                   //exp: time:feed3322default: DEFAULT:off
        break; case __LINE__: TRice(iD(14896), "NOTICE:blue:white+h\n" );           //exp: time:feed3322default: NOTICE:blue:white+h
        break; case __LINE__: TRice(iD(14897), "ALERT:magenta:magenta+h\n" );       //exp: time:feed3322default: ALERT:magenta:magenta+h
        break; case __LINE__: TRice(iD(14898), "ASSERT:yellow+i:blue\n" );          //exp: time:feed3322default: ASSERT:yellow+i:blue
        break; case __LINE__: TRice(iD(14899), "ALARM:red+i:white+h\n" );           //exp: time:feed3322default: ALARM:red+i:white+h
        break; case __LINE__: TRice(iD(14900), "CYCLE:blue+i:default+h\n" );        //exp: time:feed3322default: CYCLE:blue+i:default+h
        break; case __LINE__: TRice(iD(14901), "VERBOSE:blue:default\n" );          //exp: time:feed3322default: VERBOSE:blue:default
        break; case __LINE__: TRice(iD(14902), "fatal:magenta+b:red\n" );           //exp: time:feed3322default: fatal:magenta+b:red
        break; case __LINE__: TRice(iD(14903), "critical:red+i:default+h\n" );      //exp: time:feed3322default: critical:red+i:default+h
        break; case __LINE__: TRice(iD(14904), "emergency:red+i:blue\n" );          //exp: time:feed3322default: emergency:red+i:blue
        break; case __LINE__: TRice(iD(14905), "error:11:red\n" );                  //exp: time:feed3322default: error:11:red
        break; case __LINE__: TRice(iD(14906), "warning:11+i:red\n" );              //exp: time:feed3322default: warning:11+i:red
        break; case __LINE__: TRice(iD(14907), "attention:11:green\n" );            //exp: time:feed3322default: attention:11:green
        break; case __LINE__: TRice(iD(14908), "info:cyan+b:default+h\n" );         //exp: time:feed3322default: info:cyan+b:default+h
        break; case __LINE__: TRice(iD(14909), "debug:130+i\n" );                   //exp: time:feed3322default: debug:130+i
        break; case __LINE__: TRice(iD(14910), "trace:default+i:default+h\n" );     //exp: time:feed3322default: trace:default+i:default+h
        break; case __LINE__: TRice(iD(14911), "time:blue+i:blue+h\n" );            //exp: time:feed3322default: time:blue+i:blue+h
        break; case __LINE__: TRice(iD(14912), "message:green+h:black\n" );         //exp: time:feed3322default: message:green+h:black
        break; case __LINE__: TRice(iD(14913), "read:black+i:yellow+h\n" );         //exp: time:feed3322default: read:black+i:yellow+h
        break; case __LINE__: TRice(iD(14914), "write:black+u:yellow+h\n" );        //exp: time:feed3322default: write:black+u:yellow+h
        break; case __LINE__: TRice(iD(14915), "receive:black+h:black\n" );         //exp: time:feed3322default: receive:black+h:black
        break; case __LINE__: TRice(iD(14916), "transmit:black:black+h\n" );        //exp: time:feed3322default: transmit:black:black+h
        break; case __LINE__: TRice(iD(14917), "diag:yellow+i:default+h\n" );       //exp: time:feed3322default: diag:yellow+i:default+h
        break; case __LINE__: TRice(iD(14918), "interrupt:magenta+i:default+h\n" ); //exp: time:feed3322default: interrupt:magenta+i:default+h
        break; case __LINE__: TRice(iD(14919), "signal:118+i\n" );                  //exp: time:feed3322default: signal:118+i
        break; case __LINE__: TRice(iD(14920), "test:yellow+h:black\n" );           //exp: time:feed3322default: test:yellow+h:black
        break; case __LINE__: TRice(iD(14921), "default:off\n" );                   //exp: time:feed3322default: default:off
        break; case __LINE__: TRice(iD(14922), "notice:blue:white+h\n" );           //exp: time:feed3322default: notice:blue:white+h
        break; case __LINE__: TRice(iD(14923), "alert:magenta:magenta+h\n" );       //exp: time:feed3322default: alert:magenta:magenta+h
        break; case __LINE__: TRice(iD(14924), "assert:yellow+i:blue\n" );          //exp: time:feed3322default: assert:yellow+i:blue
        break; case __LINE__: TRice(iD(14925), "alarm:red+i:white+h\n" );           //exp: time:feed3322default: alarm:red+i:white+h
        break; case __LINE__: TRice(iD(14926), "cycle:blue+i:default+h\n" );        //exp: time:feed3322default: cycle:blue+i:default+h
        break; case __LINE__: TRice(iD(14927), "verbose:blue:default\n" );          //exp: time:feed3322default: verbose:blue:default

   
   
        break; case __LINE__: TRICE64(id(14928), "msg:%d (%%d)\n", -1 );            //exp: time:            default: msg:-1 (%d)

        break; case __LINE__: trice(iD(14929), "sig:TRICE8 with 1 to 12 values\n" );

        break; case __LINE__: TRICE(id(14930), "rd:TRICE %d\n", -1 );                                                                                            //exp: time:            default: rd:TRICE -1     
        break; case __LINE__: TRICE(id(14931), "rd:TRICE %d, %d\n", -1, -2 );                                                                                    //exp: time:            default: rd:TRICE -1, -2 
        break; case __LINE__: TRICE(id(14932), "rd:TRICE %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: rd:TRICE -1, -2, -3       
        break; case __LINE__: TRICE(id(14933), "rd:TRICE %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:            default: rd:TRICE -1, -2, -3, -4      
        break; case __LINE__: TRICE(id(14934), "rd:TRICE %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:            default: rd:TRICE -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE(id(14935), "rd:TRICE %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: rd:TRICE -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE(id(14936), "rd:TRICE %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:            default: rd:TRICE -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE(id(14937), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:            default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE(id(14938), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE(id(14939), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:            default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE(id(14940), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:            default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE(id(14941), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE(Id(14942), "rd:TRICE %d\n", -1 );                                                                                            //exp: time:    be16default: rd:TRICE -1     
        break; case __LINE__: TRICE(Id(14943), "rd:TRICE %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:TRICE -1, -2 
        break; case __LINE__: TRICE(Id(14944), "rd:TRICE %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:TRICE -1, -2, -3       
        break; case __LINE__: TRICE(Id(14945), "rd:TRICE %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:TRICE -1, -2, -3, -4      
        break; case __LINE__: TRICE(Id(14946), "rd:TRICE %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:TRICE -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE(Id(14947), "rd:TRICE %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:TRICE -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE(Id(14948), "rd:TRICE %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:TRICE -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE(Id(14949), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE(Id(14950), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE(Id(14951), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE(Id(14952), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE(Id(14953), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE(ID(14954), "rd:TRICE %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRICE -1     
        break; case __LINE__: TRICE(ID(14955), "rd:TRICE %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRICE -1, -2 
        break; case __LINE__: TRICE(ID(14956), "rd:TRICE %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRICE -1, -2, -3       
        break; case __LINE__: TRICE(ID(14957), "rd:TRICE %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRICE -1, -2, -3, -4      
        break; case __LINE__: TRICE(ID(14958), "rd:TRICE %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRICE -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE(ID(14959), "rd:TRICE %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRICE -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE(ID(14960), "rd:TRICE %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRICE -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE(ID(14961), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE(ID(14962), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE(ID(14963), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE(ID(14964), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE(ID(14965), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: trice(iD(14966), "sig:TRICE_n with 1 to 12 values\n" );

        break; case __LINE__: TRICE_1 (id(14967), "rd:TRICE_1  %d\n", -1 );                                                                                            //exp: time:            default: rd:TRICE_1  -1     
        break; case __LINE__: TRICE_2 (id(14968), "rd:TRICE_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:            default: rd:TRICE_2  -1, -2 
        break; case __LINE__: TRICE_3 (id(14969), "rd:TRICE_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: rd:TRICE_3  -1, -2, -3       
        break; case __LINE__: TRICE_4 (id(14970), "rd:TRICE_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:            default: rd:TRICE_4  -1, -2, -3, -4      
        break; case __LINE__: TRICE_5 (id(14971), "rd:TRICE_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:            default: rd:TRICE_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE_6 (id(14972), "rd:TRICE_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: rd:TRICE_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE_7 (id(14973), "rd:TRICE_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:            default: rd:TRICE_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE_8 (id(14974), "rd:TRICE_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:            default: rd:TRICE_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE_9 (id(14975), "rd:TRICE_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: rd:TRICE_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE_10(id(14976), "rd:TRICE_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:            default: rd:TRICE_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE_11(id(14977), "rd:TRICE_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:            default: rd:TRICE_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE_12(id(14978), "rd:TRICE_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: rd:TRICE_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE_1 (Id(14979), "rd:TRICE_1  %d\n", -1 );                                                                                            //exp: time:    be16default: rd:TRICE_1  -1     
        break; case __LINE__: TRICE_2 (Id(14980), "rd:TRICE_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:TRICE_2  -1, -2 
        break; case __LINE__: TRICE_3 (Id(14981), "rd:TRICE_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:TRICE_3  -1, -2, -3       
        break; case __LINE__: TRICE_4 (Id(14982), "rd:TRICE_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:TRICE_4  -1, -2, -3, -4      
        break; case __LINE__: TRICE_5 (Id(14983), "rd:TRICE_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:TRICE_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE_6 (Id(14984), "rd:TRICE_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:TRICE_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE_7 (Id(14985), "rd:TRICE_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:TRICE_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE_8 (Id(14986), "rd:TRICE_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:TRICE_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE_9 (Id(14987), "rd:TRICE_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:TRICE_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE_10(Id(14988), "rd:TRICE_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:TRICE_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE_11(Id(14989), "rd:TRICE_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:TRICE_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE_12(Id(14990), "rd:TRICE_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:TRICE_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE_1 (ID(14991), "rd:TRICE_1  %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRICE_1  -1     
        break; case __LINE__: TRICE_2 (ID(14992), "rd:TRICE_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRICE_2  -1, -2 
        break; case __LINE__: TRICE_3 (ID(14993), "rd:TRICE_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRICE_3  -1, -2, -3       
        break; case __LINE__: TRICE_4 (ID(14994), "rd:TRICE_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRICE_4  -1, -2, -3, -4      
        break; case __LINE__: TRICE_5 (ID(14995), "rd:TRICE_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRICE_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE_6 (ID(14996), "rd:TRICE_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRICE_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE_7 (ID(14997), "rd:TRICE_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRICE_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE_8 (ID(14998), "rd:TRICE_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRICE_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE_9 (ID(14999), "rd:TRICE_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRICE_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE_10(ID(15000), "rd:TRICE_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRICE_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE_11(ID(15001), "rd:TRICE_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRICE_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE_12(ID(15002), "rd:TRICE_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRICE_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: TRICE(ID(15003), "sig:trice_n with 1 to 12 values\n" );

        break; case __LINE__: trice_1 (iD(15004), "rd:trice_1  %d\n", -1 );                                                                                            //exp: time:            default: rd:trice_1  -1     
        break; case __LINE__: trice_2 (iD(15005), "rd:trice_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:            default: rd:trice_2  -1, -2 
        break; case __LINE__: trice_3 (iD(15006), "rd:trice_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: rd:trice_3  -1, -2, -3       
        break; case __LINE__: trice_4 (iD(15007), "rd:trice_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:            default: rd:trice_4  -1, -2, -3, -4      
        break; case __LINE__: trice_5 (iD(15008), "rd:trice_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:            default: rd:trice_5  -1, -2, -3, -4, -5        
        break; case __LINE__: trice_6 (iD(15009), "rd:trice_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: rd:trice_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice_7 (iD(15010), "rd:trice_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:            default: rd:trice_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice_8 (iD(15011), "rd:trice_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:            default: rd:trice_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice_9 (iD(15012), "rd:trice_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: rd:trice_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice_10(iD(15013), "rd:trice_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:            default: rd:trice_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice_11(iD(15014), "rd:trice_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:            default: rd:trice_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice_12(iD(15015), "rd:trice_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: rd:trice_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: Trice_1 (iD(15016), "rd:Trice_1  %d\n", -1 );                                                                                            //exp: time:    be16default: rd:Trice_1  -1     
        break; case __LINE__: Trice_2 (iD(15017), "rd:Trice_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:Trice_2  -1, -2 
        break; case __LINE__: Trice_3 (iD(15018), "rd:Trice_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:Trice_3  -1, -2, -3       
        break; case __LINE__: Trice_4 (iD(15019), "rd:Trice_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:Trice_4  -1, -2, -3, -4      
        break; case __LINE__: Trice_5 (iD(15020), "rd:Trice_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:Trice_5  -1, -2, -3, -4, -5        
        break; case __LINE__: Trice_6 (iD(15021), "rd:Trice_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:Trice_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: Trice_7 (iD(15022), "rd:Trice_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:Trice_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: Trice_8 (iD(15023), "rd:Trice_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:Trice_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: Trice_9 (iD(15024), "rd:Trice_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:Trice_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: Trice_10(iD(15025), "rd:Trice_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:Trice_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: Trice_11(iD(15026), "rd:Trice_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:Trice_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: Trice_12(iD(15027), "rd:Trice_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:Trice_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRice_1 (iD(15028), "rd:TRice_1  %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRice_1  -1     
        break; case __LINE__: TRice_2 (iD(15029), "rd:TRice_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRice_2  -1, -2 
        break; case __LINE__: TRice_3 (iD(15030), "rd:TRice_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRice_3  -1, -2, -3       
        break; case __LINE__: TRice_4 (iD(15031), "rd:TRice_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRice_4  -1, -2, -3, -4      
        break; case __LINE__: TRice_5 (iD(15032), "rd:TRice_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRice_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRice_6 (iD(15033), "rd:TRice_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRice_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRice_7 (iD(15034), "rd:TRice_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRice_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRice_8 (iD(15035), "rd:TRice_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRice_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRice_9 (iD(15036), "rd:TRice_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRice_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRice_10(iD(15037), "rd:TRice_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRice_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRice_11(iD(15038), "rd:TRice_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRice_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRice_12(iD(15039), "rd:TRice_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRice_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: trice(iD(15040), "sig:TRICE8 with 1 to 12 values\n" );

        break; case __LINE__: TRICE8(id(15041), "rd:TRICE8 %d\n", -1 );                                                                                            //exp: time:            default: rd:TRICE8 -1     
        break; case __LINE__: TRICE8(id(15042), "rd:TRICE8 %d, %d\n", -1, -2 );                                                                                    //exp: time:            default: rd:TRICE8 -1, -2 
        break; case __LINE__: TRICE8(id(15043), "rd:TRICE8 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: rd:TRICE8 -1, -2, -3       
        break; case __LINE__: TRICE8(id(15044), "rd:TRICE8 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:            default: rd:TRICE8 -1, -2, -3, -4      
        break; case __LINE__: TRICE8(id(15045), "rd:TRICE8 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:            default: rd:TRICE8 -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE8(id(15046), "rd:TRICE8 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: rd:TRICE8 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE8(id(15047), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:            default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE8(id(15048), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:            default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE8(id(15049), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE8(id(15050), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:            default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE8(id(15051), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:            default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE8(id(15052), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE8(Id(15053), "rd:TRICE8 %d\n", -1 );                                                                                            //exp: time:    be16default: rd:TRICE8 -1     
        break; case __LINE__: TRICE8(Id(15054), "rd:TRICE8 %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:TRICE8 -1, -2 
        break; case __LINE__: TRICE8(Id(15055), "rd:TRICE8 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:TRICE8 -1, -2, -3       
        break; case __LINE__: TRICE8(Id(15056), "rd:TRICE8 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:TRICE8 -1, -2, -3, -4      
        break; case __LINE__: TRICE8(Id(15057), "rd:TRICE8 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:TRICE8 -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE8(Id(15058), "rd:TRICE8 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:TRICE8 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE8(Id(15059), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE8(Id(15060), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE8(Id(15061), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE8(Id(15062), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE8(Id(15063), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE8(Id(15064), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE8(ID(15065), "rd:TRICE8 %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRICE8 -1     
        break; case __LINE__: TRICE8(ID(15066), "rd:TRICE8 %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRICE8 -1, -2 
        break; case __LINE__: TRICE8(ID(15067), "rd:TRICE8 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRICE8 -1, -2, -3       
        break; case __LINE__: TRICE8(ID(15068), "rd:TRICE8 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRICE8 -1, -2, -3, -4      
        break; case __LINE__: TRICE8(ID(15069), "rd:TRICE8 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRICE8 -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE8(ID(15070), "rd:TRICE8 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRICE8 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE8(ID(15071), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE8(ID(15072), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE8(ID(15073), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE8(ID(15074), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE8(ID(15075), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE8(ID(15076), "rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRICE8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: trice(iD(15077), "sig:TRICE8_n with 1 to 12 values\n" );

        break; case __LINE__: TRICE8_1 (id(15078), "rd:TRICE8_1  %d\n", -1 );                                                                                            //exp: time:            default: rd:TRICE8_1  -1     
        break; case __LINE__: TRICE8_2 (id(15079), "rd:TRICE8_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:            default: rd:TRICE8_2  -1, -2 
        break; case __LINE__: TRICE8_3 (id(15080), "rd:TRICE8_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: rd:TRICE8_3  -1, -2, -3       
        break; case __LINE__: TRICE8_4 (id(15081), "rd:TRICE8_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:            default: rd:TRICE8_4  -1, -2, -3, -4      
        break; case __LINE__: TRICE8_5 (id(15082), "rd:TRICE8_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:            default: rd:TRICE8_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE8_6 (id(15083), "rd:TRICE8_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: rd:TRICE8_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE8_7 (id(15084), "rd:TRICE8_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:            default: rd:TRICE8_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE8_8 (id(15085), "rd:TRICE8_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:            default: rd:TRICE8_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE8_9 (id(15086), "rd:TRICE8_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: rd:TRICE8_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE8_10(id(15087), "rd:TRICE8_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:            default: rd:TRICE8_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE8_11(id(15088), "rd:TRICE8_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:            default: rd:TRICE8_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE8_12(id(15089), "rd:TRICE8_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: rd:TRICE8_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE8_1 (Id(15090), "rd:TRICE8_1  %d\n", -1 );                                                                                            //exp: time:    be16default: rd:TRICE8_1  -1     
        break; case __LINE__: TRICE8_2 (Id(15091), "rd:TRICE8_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:TRICE8_2  -1, -2 
        break; case __LINE__: TRICE8_3 (Id(15092), "rd:TRICE8_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:TRICE8_3  -1, -2, -3       
        break; case __LINE__: TRICE8_4 (Id(15093), "rd:TRICE8_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:TRICE8_4  -1, -2, -3, -4      
        break; case __LINE__: TRICE8_5 (Id(15094), "rd:TRICE8_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:TRICE8_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE8_6 (Id(15095), "rd:TRICE8_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:TRICE8_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE8_7 (Id(15096), "rd:TRICE8_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:TRICE8_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE8_8 (Id(15097), "rd:TRICE8_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:TRICE8_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE8_9 (Id(15098), "rd:TRICE8_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:TRICE8_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE8_10(Id(15099), "rd:TRICE8_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:TRICE8_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE8_11(Id(15100), "rd:TRICE8_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:TRICE8_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE8_12(Id(15101), "rd:TRICE8_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:TRICE8_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE8_1 (ID(15102), "rd:TRICE8_1  %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRICE8_1  -1     
        break; case __LINE__: TRICE8_2 (ID(15103), "rd:TRICE8_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRICE8_2  -1, -2 
        break; case __LINE__: TRICE8_3 (ID(15104), "rd:TRICE8_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRICE8_3  -1, -2, -3       
        break; case __LINE__: TRICE8_4 (ID(15105), "rd:TRICE8_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRICE8_4  -1, -2, -3, -4      
        break; case __LINE__: TRICE8_5 (ID(15106), "rd:TRICE8_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRICE8_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE8_6 (ID(15107), "rd:TRICE8_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRICE8_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE8_7 (ID(15108), "rd:TRICE8_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRICE8_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE8_8 (ID(15109), "rd:TRICE8_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRICE8_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE8_9 (ID(15110), "rd:TRICE8_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRICE8_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE8_10(ID(15111), "rd:TRICE8_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRICE8_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE8_11(ID(15112), "rd:TRICE8_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRICE8_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE8_12(ID(15113), "rd:TRICE8_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRICE8_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: trice(iD(15114), "sig:trice8 with 1 to 12 values\n" );
        
        break; case __LINE__: trice8(iD(15115), "rd:trice8 %d\n", -1 );                                                                                            //exp: time:            default: rd:trice8 -1     
        break; case __LINE__: trice8(iD(15116), "rd:trice8 %d, %d\n", -1, -2 );                                                                                    //exp: time:            default: rd:trice8 -1, -2 
        break; case __LINE__: trice8(iD(15117), "rd:trice8 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: rd:trice8 -1, -2, -3       
        break; case __LINE__: trice8(iD(15118), "rd:trice8 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:            default: rd:trice8 -1, -2, -3, -4      
        break; case __LINE__: trice8(iD(15119), "rd:trice8 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:            default: rd:trice8 -1, -2, -3, -4, -5        
        break; case __LINE__: trice8(iD(15120), "rd:trice8 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: rd:trice8 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice8(iD(15121), "rd:trice8 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:            default: rd:trice8 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice8(iD(15122), "rd:trice8 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:            default: rd:trice8 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice8(iD(15123), "rd:trice8 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: rd:trice8 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice8(iD(15124), "rd:trice8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:            default: rd:trice8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice8(iD(15125), "rd:trice8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:            default: rd:trice8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice8(iD(15126), "rd:trice8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: rd:trice8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: Trice8(iD(15127), "rd:Trice8 %d\n", -1 );                                                                                            //exp: time:    be16default: rd:Trice8 -1     
        break; case __LINE__: Trice8(iD(15128), "rd:Trice8 %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:Trice8 -1, -2 
        break; case __LINE__: Trice8(iD(15129), "rd:Trice8 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:Trice8 -1, -2, -3       
        break; case __LINE__: Trice8(iD(15130), "rd:Trice8 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:Trice8 -1, -2, -3, -4      
        break; case __LINE__: Trice8(iD(15131), "rd:Trice8 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:Trice8 -1, -2, -3, -4, -5        
        break; case __LINE__: Trice8(iD(15132), "rd:Trice8 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:Trice8 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: Trice8(iD(15133), "rd:Trice8 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:Trice8 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: Trice8(iD(15134), "rd:Trice8 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:Trice8 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: Trice8(iD(15135), "rd:Trice8 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:Trice8 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: Trice8(iD(15136), "rd:Trice8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:Trice8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: Trice8(iD(15137), "rd:Trice8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:Trice8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: Trice8(iD(15138), "rd:Trice8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:Trice8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRice8(iD(15139), "rd:TRice8 %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRice8 -1     
        break; case __LINE__: TRice8(iD(15140), "rd:TRice8 %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRice8 -1, -2 
        break; case __LINE__: TRice8(iD(15141), "rd:TRice8 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRice8 -1, -2, -3       
        break; case __LINE__: TRice8(iD(15142), "rd:TRice8 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRice8 -1, -2, -3, -4      
        break; case __LINE__: TRice8(iD(15143), "rd:TRice8 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRice8 -1, -2, -3, -4, -5        
        break; case __LINE__: TRice8(iD(15144), "rd:TRice8 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRice8 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRice8(iD(15145), "rd:TRice8 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRice8 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRice8(iD(15146), "rd:TRice8 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRice8 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRice8(iD(15147), "rd:TRice8 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRice8 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRice8(iD(15148), "rd:TRice8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRice8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRice8(iD(15149), "rd:TRice8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRice8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRice8(iD(15150), "rd:TRice8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRice8 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: TRICE(ID(15151), "sig:trice8_n with 1 to 12 values\n" );

        break; case __LINE__: trice8_1 (iD(15152), "rd:trice8_1  %d\n", -1 );                                                                                            //exp: time:            default: rd:trice8_1  -1     
        break; case __LINE__: trice8_2 (iD(15153), "rd:trice8_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:            default: rd:trice8_2  -1, -2 
        break; case __LINE__: trice8_3 (iD(15154), "rd:trice8_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: rd:trice8_3  -1, -2, -3       
        break; case __LINE__: trice8_4 (iD(15155), "rd:trice8_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:            default: rd:trice8_4  -1, -2, -3, -4      
        break; case __LINE__: trice8_5 (iD(15156), "rd:trice8_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:            default: rd:trice8_5  -1, -2, -3, -4, -5        
        break; case __LINE__: trice8_6 (iD(15157), "rd:trice8_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: rd:trice8_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice8_7 (iD(15158), "rd:trice8_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:            default: rd:trice8_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice8_8 (iD(15159), "rd:trice8_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:            default: rd:trice8_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice8_9 (iD(15160), "rd:trice8_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: rd:trice8_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice8_10(iD(15161), "rd:trice8_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:            default: rd:trice8_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice8_11(iD(15162), "rd:trice8_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:            default: rd:trice8_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice8_12(iD(15163), "rd:trice8_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: rd:trice8_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: Trice8_1 (iD(15164), "rd:Trice8_1  %d\n", -1 );                                                                                            //exp: time:    be16default: rd:Trice8_1  -1     
        break; case __LINE__: Trice8_2 (iD(15165), "rd:Trice8_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:Trice8_2  -1, -2 
        break; case __LINE__: Trice8_3 (iD(15166), "rd:Trice8_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:Trice8_3  -1, -2, -3       
        break; case __LINE__: Trice8_4 (iD(15167), "rd:Trice8_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:Trice8_4  -1, -2, -3, -4      
        break; case __LINE__: Trice8_5 (iD(15168), "rd:Trice8_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:Trice8_5  -1, -2, -3, -4, -5        
        break; case __LINE__: Trice8_6 (iD(15169), "rd:Trice8_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:Trice8_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: Trice8_7 (iD(15170), "rd:Trice8_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:Trice8_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: Trice8_8 (iD(15171), "rd:Trice8_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:Trice8_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: Trice8_9 (iD(15172), "rd:Trice8_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:Trice8_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: Trice8_10(iD(15173), "rd:Trice8_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:Trice8_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: Trice8_11(iD(15174), "rd:Trice8_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:Trice8_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: Trice8_12(iD(15175), "rd:Trice8_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:Trice8_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRice8_1 (iD(15176), "rd:TRice8_1  %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRice8_1  -1     
        break; case __LINE__: TRice8_2 (iD(15177), "rd:TRice8_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRice8_2  -1, -2 
        break; case __LINE__: TRice8_3 (iD(15178), "rd:TRice8_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRice8_3  -1, -2, -3       
        break; case __LINE__: TRice8_4 (iD(15179), "rd:TRice8_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRice8_4  -1, -2, -3, -4      
        break; case __LINE__: TRice8_5 (iD(15180), "rd:TRice8_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRice8_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRice8_6 (iD(15181), "rd:TRice8_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRice8_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRice8_7 (iD(15182), "rd:TRice8_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRice8_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRice8_8 (iD(15183), "rd:TRice8_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRice8_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRice8_9 (iD(15184), "rd:TRice8_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRice8_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRice8_10(iD(15185), "rd:TRice8_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRice8_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRice8_11(iD(15186), "rd:TRice8_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRice8_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRice8_12(iD(15187), "rd:TRice8_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRice8_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: trice(iD(15188), "sig:TRICE16 with 1 to 12 values (line %d)\n", __LINE__ );

        break; case __LINE__: TRICE16(id(15189), "rd:TRICE16 %d\n", -1 );                                                                                            //exp: time:            default: rd:TRICE16 -1     
        break; case __LINE__: TRICE16(id(15190), "rd:TRICE16 %d, %d\n", -1, -2 );                                                                                    //exp: time:            default: rd:TRICE16 -1, -2 
        break; case __LINE__: TRICE16(id(15191), "rd:TRICE16 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: rd:TRICE16 -1, -2, -3       
        break; case __LINE__: TRICE16(id(15192), "rd:TRICE16 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:            default: rd:TRICE16 -1, -2, -3, -4      
        break; case __LINE__: TRICE16(id(15193), "rd:TRICE16 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:            default: rd:TRICE16 -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE16(id(15194), "rd:TRICE16 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: rd:TRICE16 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE16(id(15195), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:            default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE16(id(15196), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:            default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE16(id(15197), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE16(id(15198), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:            default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE16(id(15199), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:            default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE16(id(15200), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE16(Id(15201), "rd:TRICE16 %d\n", -1 );                                                                                            //exp: time:    be16default: rd:TRICE16 -1     
        break; case __LINE__: TRICE16(Id(15202), "rd:TRICE16 %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:TRICE16 -1, -2 
        break; case __LINE__: TRICE16(Id(15203), "rd:TRICE16 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:TRICE16 -1, -2, -3       
        break; case __LINE__: TRICE16(Id(15204), "rd:TRICE16 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:TRICE16 -1, -2, -3, -4      
        break; case __LINE__: TRICE16(Id(15205), "rd:TRICE16 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:TRICE16 -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE16(Id(15206), "rd:TRICE16 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:TRICE16 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE16(Id(15207), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE16(Id(15208), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE16(Id(15209), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE16(Id(15210), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE16(Id(15211), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE16(Id(15212), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE16(ID(15213), "rd:TRICE16 %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRICE16 -1     
        break; case __LINE__: TRICE16(ID(15214), "rd:TRICE16 %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRICE16 -1, -2 
        break; case __LINE__: TRICE16(ID(15215), "rd:TRICE16 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRICE16 -1, -2, -3       
        break; case __LINE__: TRICE16(ID(15216), "rd:TRICE16 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRICE16 -1, -2, -3, -4      
        break; case __LINE__: TRICE16(ID(15217), "rd:TRICE16 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRICE16 -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE16(ID(15218), "rd:TRICE16 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRICE16 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE16(ID(15219), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE16(ID(15220), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE16(ID(15221), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE16(ID(15222), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE16(ID(15223), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE16(ID(15224), "rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRICE16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: trice     (iD(15225), "sig:TRICE16_n with 1 to 12 values (line %d)\n", __LINE__ );
        
        break; case __LINE__: TRICE16_1 (id(15226), "rd:TRICE16_1  %d\n", -1 );                                                                                            //exp: time:            default: rd:TRICE16_1  -1     
        break; case __LINE__: TRICE16_2 (id(15227), "rd:TRICE16_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:            default: rd:TRICE16_2  -1, -2 
        break; case __LINE__: TRICE16_3 (id(15228), "rd:TRICE16_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: rd:TRICE16_3  -1, -2, -3       
        break; case __LINE__: TRICE16_4 (id(15229), "rd:TRICE16_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:            default: rd:TRICE16_4  -1, -2, -3, -4      
        break; case __LINE__: TRICE16_5 (id(15230), "rd:TRICE16_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:            default: rd:TRICE16_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE16_6 (id(15231), "rd:TRICE16_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: rd:TRICE16_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE16_7 (id(15232), "rd:TRICE16_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:            default: rd:TRICE16_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE16_8 (id(15233), "rd:TRICE16_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:            default: rd:TRICE16_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE16_9 (id(15234), "rd:TRICE16_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: rd:TRICE16_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE16_10(id(15235), "rd:TRICE16_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:            default: rd:TRICE16_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE16_11(id(15236), "rd:TRICE16_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:            default: rd:TRICE16_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE16_12(id(15237), "rd:TRICE16_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: rd:TRICE16_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE16_1 (Id(15238), "rd:TRICE16_1  %d\n", -1 );                                                                                            //exp: time:    be16default: rd:TRICE16_1  -1     
        break; case __LINE__: TRICE16_2 (Id(15239), "rd:TRICE16_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:TRICE16_2  -1, -2 
        break; case __LINE__: TRICE16_3 (Id(15240), "rd:TRICE16_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:TRICE16_3  -1, -2, -3       
        break; case __LINE__: TRICE16_4 (Id(15241), "rd:TRICE16_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:TRICE16_4  -1, -2, -3, -4      
        break; case __LINE__: TRICE16_5 (Id(15242), "rd:TRICE16_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:TRICE16_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE16_6 (Id(15243), "rd:TRICE16_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:TRICE16_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE16_7 (Id(15244), "rd:TRICE16_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:TRICE16_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE16_8 (Id(15245), "rd:TRICE16_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:TRICE16_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE16_9 (Id(15246), "rd:TRICE16_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:TRICE16_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE16_10(Id(15247), "rd:TRICE16_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:TRICE16_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE16_11(Id(15248), "rd:TRICE16_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:TRICE16_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE16_12(Id(15249), "rd:TRICE16_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:TRICE16_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE16_1 (ID(15250), "rd:TRICE16_1  %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRICE16_1  -1     
        break; case __LINE__: TRICE16_2 (ID(15251), "rd:TRICE16_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRICE16_2  -1, -2 
        break; case __LINE__: TRICE16_3 (ID(15252), "rd:TRICE16_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRICE16_3  -1, -2, -3       
        break; case __LINE__: TRICE16_4 (ID(15253), "rd:TRICE16_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRICE16_4  -1, -2, -3, -4      
        break; case __LINE__: TRICE16_5 (ID(15254), "rd:TRICE16_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRICE16_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE16_6 (ID(15255), "rd:TRICE16_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRICE16_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE16_7 (ID(15256), "rd:TRICE16_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRICE16_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE16_8 (ID(15257), "rd:TRICE16_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRICE16_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE16_9 (ID(15258), "rd:TRICE16_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRICE16_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE16_10(ID(15259), "rd:TRICE16_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRICE16_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE16_11(ID(15260), "rd:TRICE16_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRICE16_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE16_12(ID(15261), "rd:TRICE16_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRICE16_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: trice(iD(15262), "sig:trice16 with 1 to 12 values (line %d)\n", __LINE__ );

        break; case __LINE__: trice16(iD(15263), "rd:trice16 %d\n", -1 );                                                                                            //exp: time:            default: rd:trice16 -1     
        break; case __LINE__: trice16(iD(15264), "rd:trice16 %d, %d\n", -1, -2 );                                                                                    //exp: time:            default: rd:trice16 -1, -2 
        break; case __LINE__: trice16(iD(15265), "rd:trice16 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: rd:trice16 -1, -2, -3       
        break; case __LINE__: trice16(iD(15266), "rd:trice16 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:            default: rd:trice16 -1, -2, -3, -4      
        break; case __LINE__: trice16(iD(15267), "rd:trice16 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:            default: rd:trice16 -1, -2, -3, -4, -5        
        break; case __LINE__: trice16(iD(15268), "rd:trice16 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: rd:trice16 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice16(iD(15269), "rd:trice16 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:            default: rd:trice16 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice16(iD(15270), "rd:trice16 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:            default: rd:trice16 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice16(iD(15271), "rd:trice16 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: rd:trice16 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice16(iD(15272), "rd:trice16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:            default: rd:trice16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice16(iD(15273), "rd:trice16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:            default: rd:trice16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice16(iD(15274), "rd:trice16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: rd:trice16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: Trice16(iD(15275), "rd:Trice16 %d\n", -1 );                                                                                            //exp: time:    be16default: rd:Trice16 -1     
        break; case __LINE__: Trice16(iD(15276), "rd:Trice16 %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:Trice16 -1, -2 
        break; case __LINE__: Trice16(iD(15277), "rd:Trice16 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:Trice16 -1, -2, -3       
        break; case __LINE__: Trice16(iD(15278), "rd:Trice16 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:Trice16 -1, -2, -3, -4      
        break; case __LINE__: Trice16(iD(15279), "rd:Trice16 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:Trice16 -1, -2, -3, -4, -5        
        break; case __LINE__: Trice16(iD(15280), "rd:Trice16 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:Trice16 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: Trice16(iD(15281), "rd:Trice16 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:Trice16 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: Trice16(iD(15282), "rd:Trice16 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:Trice16 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: Trice16(iD(15283), "rd:Trice16 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:Trice16 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: Trice16(iD(15284), "rd:Trice16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:Trice16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: Trice16(iD(15285), "rd:Trice16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:Trice16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: Trice16(iD(15286), "rd:Trice16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:Trice16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRice16(iD(15287), "rd:TRice16 %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRice16 -1     
        break; case __LINE__: TRice16(iD(15288), "rd:TRice16 %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRice16 -1, -2 
        break; case __LINE__: TRice16(iD(15289), "rd:TRice16 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRice16 -1, -2, -3       
        break; case __LINE__: TRice16(iD(15290), "rd:TRice16 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRice16 -1, -2, -3, -4      
        break; case __LINE__: TRice16(iD(15291), "rd:TRice16 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRice16 -1, -2, -3, -4, -5        
        break; case __LINE__: TRice16(iD(15292), "rd:TRice16 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRice16 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRice16(iD(15293), "rd:TRice16 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRice16 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRice16(iD(15294), "rd:TRice16 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRice16 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRice16(iD(15295), "rd:TRice16 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRice16 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRice16(iD(15296), "rd:TRice16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRice16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRice16(iD(15297), "rd:TRice16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRice16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRice16(iD(15298), "rd:TRice16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRice16 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: trice     (iD(15299), "sig:trice16_n with 1 to 12 values (line %d)\n", __LINE__ );
        
        break; case __LINE__: trice16_1 (iD(15300), "rd:trice16_1  %d\n", -1 );                                                                                            //exp: time:            default: rd:trice16_1  -1     
        break; case __LINE__: trice16_2 (iD(15301), "rd:trice16_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:            default: rd:trice16_2  -1, -2 
        break; case __LINE__: trice16_3 (iD(15302), "rd:trice16_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: rd:trice16_3  -1, -2, -3       
        break; case __LINE__: trice16_4 (iD(15303), "rd:trice16_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:            default: rd:trice16_4  -1, -2, -3, -4      
        break; case __LINE__: trice16_5 (iD(15304), "rd:trice16_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:            default: rd:trice16_5  -1, -2, -3, -4, -5        
        break; case __LINE__: trice16_6 (iD(15305), "rd:trice16_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: rd:trice16_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice16_7 (iD(15306), "rd:trice16_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:            default: rd:trice16_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice16_8 (iD(15307), "rd:trice16_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:            default: rd:trice16_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice16_9 (iD(15308), "rd:trice16_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: rd:trice16_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice16_10(iD(15309), "rd:trice16_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:            default: rd:trice16_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice16_11(iD(15310), "rd:trice16_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:            default: rd:trice16_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice16_12(iD(15311), "rd:trice16_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: rd:trice16_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: Trice16_1 (iD(15312), "rd:Trice16_1  %d\n", -1 );                                                                                            //exp: time:    be16default: rd:Trice16_1  -1     
        break; case __LINE__: Trice16_2 (iD(15313), "rd:Trice16_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:Trice16_2  -1, -2 
        break; case __LINE__: Trice16_3 (iD(15314), "rd:Trice16_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:Trice16_3  -1, -2, -3       
        break; case __LINE__: Trice16_4 (iD(15315), "rd:Trice16_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:Trice16_4  -1, -2, -3, -4      
        break; case __LINE__: Trice16_5 (iD(15316), "rd:Trice16_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:Trice16_5  -1, -2, -3, -4, -5        
        break; case __LINE__: Trice16_6 (iD(15317), "rd:Trice16_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:Trice16_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: Trice16_7 (iD(15318), "rd:Trice16_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:Trice16_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: Trice16_8 (iD(15319), "rd:Trice16_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:Trice16_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: Trice16_9 (iD(15320), "rd:Trice16_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:Trice16_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: Trice16_10(iD(15321), "rd:Trice16_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:Trice16_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: Trice16_11(iD(15322), "rd:Trice16_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:Trice16_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: Trice16_12(iD(15323), "rd:Trice16_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:Trice16_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRice16_1 (iD(15324), "rd:TRice16_1  %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRice16_1  -1     
        break; case __LINE__: TRice16_2 (iD(15325), "rd:TRice16_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRice16_2  -1, -2 
        break; case __LINE__: TRice16_3 (iD(15326), "rd:TRice16_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRice16_3  -1, -2, -3       
        break; case __LINE__: TRice16_4 (iD(15327), "rd:TRice16_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRice16_4  -1, -2, -3, -4      
        break; case __LINE__: TRice16_5 (iD(15328), "rd:TRice16_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRice16_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRice16_6 (iD(15329), "rd:TRice16_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRice16_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRice16_7 (iD(15330), "rd:TRice16_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRice16_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRice16_8 (iD(15331), "rd:TRice16_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRice16_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRice16_9 (iD(15332), "rd:TRice16_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRice16_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRice16_10(iD(15333), "rd:TRice16_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRice16_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRice16_11(iD(15334), "rd:TRice16_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRice16_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRice16_12(iD(15335), "rd:TRice16_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRice16_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: trice  (iD(15336), "sig:TRICE32 with 1 to 12 values (line %d)\n", __LINE__ );

        break; case __LINE__: TRICE32(id(15337), "rd:TRICE32 %d\n", -1 );                                                                                            //exp: time:            default: rd:TRICE32 -1     
        break; case __LINE__: TRICE32(id(15338), "rd:TRICE32 %d, %d\n", -1, -2 );                                                                                    //exp: time:            default: rd:TRICE32 -1, -2 
        break; case __LINE__: TRICE32(id(15339), "rd:TRICE32 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: rd:TRICE32 -1, -2, -3       
        break; case __LINE__: TRICE32(id(15340), "rd:TRICE32 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:            default: rd:TRICE32 -1, -2, -3, -4      
        break; case __LINE__: TRICE32(id(15341), "rd:TRICE32 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:            default: rd:TRICE32 -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE32(id(15342), "rd:TRICE32 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: rd:TRICE32 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE32(id(15343), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:            default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE32(id(15344), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:            default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE32(id(15345), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE32(id(15346), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:            default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE32(id(15347), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:            default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE32(id(15348), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE32(Id(15349), "rd:TRICE32 %d\n", -1 );                                                                                            //exp: time:    be16default: rd:TRICE32 -1     
        break; case __LINE__: TRICE32(Id(15350), "rd:TRICE32 %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:TRICE32 -1, -2 
        break; case __LINE__: TRICE32(Id(15351), "rd:TRICE32 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:TRICE32 -1, -2, -3       
        break; case __LINE__: TRICE32(Id(15352), "rd:TRICE32 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:TRICE32 -1, -2, -3, -4      
        break; case __LINE__: TRICE32(Id(15353), "rd:TRICE32 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:TRICE32 -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE32(Id(15354), "rd:TRICE32 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:TRICE32 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE32(Id(15355), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE32(Id(15356), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE32(Id(15357), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE32(Id(15358), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE32(Id(15359), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE32(Id(15360), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE32(ID(15361), "rd:TRICE32 %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRICE32 -1     
        break; case __LINE__: TRICE32(ID(15362), "rd:TRICE32 %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRICE32 -1, -2 
        break; case __LINE__: TRICE32(ID(15363), "rd:TRICE32 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRICE32 -1, -2, -3       
        break; case __LINE__: TRICE32(ID(15364), "rd:TRICE32 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRICE32 -1, -2, -3, -4      
        break; case __LINE__: TRICE32(ID(15365), "rd:TRICE32 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRICE32 -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE32(ID(15366), "rd:TRICE32 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRICE32 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE32(ID(15367), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE32(ID(15368), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE32(ID(15369), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE32(ID(15370), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE32(ID(15371), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE32(ID(15372), "rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRICE32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: trice     (iD(15373), "signal:TRICE32_n with 1 to 12 values (line %d)\n", __LINE__ );

        break; case __LINE__: TRICE32_1 (id(15374), "rd:TRICE32_1  %d\n", -1 );                                                                                            //exp: time:            default: rd:TRICE32_1  -1     
        break; case __LINE__: TRICE32_2 (id(15375), "rd:TRICE32_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:            default: rd:TRICE32_2  -1, -2 
        break; case __LINE__: TRICE32_3 (id(15376), "rd:TRICE32_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: rd:TRICE32_3  -1, -2, -3       
        break; case __LINE__: TRICE32_4 (id(15377), "rd:TRICE32_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:            default: rd:TRICE32_4  -1, -2, -3, -4      
        break; case __LINE__: TRICE32_5 (id(15378), "rd:TRICE32_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:            default: rd:TRICE32_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE32_6 (id(15379), "rd:TRICE32_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: rd:TRICE32_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE32_7 (id(15380), "rd:TRICE32_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:            default: rd:TRICE32_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE32_8 (id(15381), "rd:TRICE32_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:            default: rd:TRICE32_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE32_9 (id(15382), "rd:TRICE32_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: rd:TRICE32_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE32_10(id(15383), "rd:TRICE32_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:            default: rd:TRICE32_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE32_11(id(15384), "rd:TRICE32_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:            default: rd:TRICE32_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE32_12(id(15385), "rd:TRICE32_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: rd:TRICE32_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE32_1 (Id(15386), "rd:TRICE32_1  %d\n", -1 );                                                                                            //exp: time:    be16default: rd:TRICE32_1  -1     
        break; case __LINE__: TRICE32_2 (Id(15387), "rd:TRICE32_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:TRICE32_2  -1, -2 
        break; case __LINE__: TRICE32_3 (Id(15388), "rd:TRICE32_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:TRICE32_3  -1, -2, -3       
        break; case __LINE__: TRICE32_4 (Id(15389), "rd:TRICE32_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:TRICE32_4  -1, -2, -3, -4      
        break; case __LINE__: TRICE32_5 (Id(15390), "rd:TRICE32_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:TRICE32_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE32_6 (Id(15391), "rd:TRICE32_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:TRICE32_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE32_7 (Id(15392), "rd:TRICE32_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:TRICE32_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE32_8 (Id(15393), "rd:TRICE32_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:TRICE32_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE32_9 (Id(15394), "rd:TRICE32_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:TRICE32_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE32_10(Id(15395), "rd:TRICE32_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:TRICE32_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE32_11(Id(15396), "rd:TRICE32_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:TRICE32_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE32_12(Id(15397), "rd:TRICE32_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:TRICE32_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE32_1 (ID(15398), "rd:TRICE32_1  %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRICE32_1  -1     
        break; case __LINE__: TRICE32_2 (ID(15399), "rd:TRICE32_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRICE32_2  -1, -2 
        break; case __LINE__: TRICE32_3 (ID(15400), "rd:TRICE32_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRICE32_3  -1, -2, -3       
        break; case __LINE__: TRICE32_4 (ID(15401), "rd:TRICE32_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRICE32_4  -1, -2, -3, -4      
        break; case __LINE__: TRICE32_5 (ID(15402), "rd:TRICE32_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRICE32_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE32_6 (ID(15403), "rd:TRICE32_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRICE32_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE32_7 (ID(15404), "rd:TRICE32_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRICE32_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE32_8 (ID(15405), "rd:TRICE32_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRICE32_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE32_9 (ID(15406), "rd:TRICE32_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRICE32_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE32_10(ID(15407), "rd:TRICE32_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRICE32_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE32_11(ID(15408), "rd:TRICE32_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRICE32_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE32_12(ID(15409), "rd:TRICE32_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRICE32_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: trice  (iD(15410), "sig:trice32 with 1 to 12 values (line %d)\n", __LINE__ );

        break; case __LINE__: trice32(iD(15411), "rd:trice32 %d\n", -1 );                                                                                            //exp: time:            default: rd:trice32 -1     
        break; case __LINE__: trice32(iD(15412), "rd:trice32 %d, %d\n", -1, -2 );                                                                                    //exp: time:            default: rd:trice32 -1, -2 
        break; case __LINE__: trice32(iD(15413), "rd:trice32 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: rd:trice32 -1, -2, -3       
        break; case __LINE__: trice32(iD(15414), "rd:trice32 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:            default: rd:trice32 -1, -2, -3, -4      
        break; case __LINE__: trice32(iD(15415), "rd:trice32 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:            default: rd:trice32 -1, -2, -3, -4, -5        
        break; case __LINE__: trice32(iD(15416), "rd:trice32 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: rd:trice32 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice32(iD(15417), "rd:trice32 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:            default: rd:trice32 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice32(iD(15418), "rd:trice32 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:            default: rd:trice32 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice32(iD(15419), "rd:trice32 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: rd:trice32 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice32(iD(15420), "rd:trice32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:            default: rd:trice32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice32(iD(15421), "rd:trice32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:            default: rd:trice32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice32(iD(15422), "rd:trice32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: rd:trice32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: Trice32(iD(15423), "rd:Trice32 %d\n", -1 );                                                                                            //exp: time:    be16default: rd:Trice32 -1     
        break; case __LINE__: Trice32(iD(15424), "rd:Trice32 %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:Trice32 -1, -2 
        break; case __LINE__: Trice32(iD(15425), "rd:Trice32 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:Trice32 -1, -2, -3       
        break; case __LINE__: Trice32(iD(15426), "rd:Trice32 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:Trice32 -1, -2, -3, -4      
        break; case __LINE__: Trice32(iD(15427), "rd:Trice32 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:Trice32 -1, -2, -3, -4, -5        
        break; case __LINE__: Trice32(iD(15428), "rd:Trice32 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:Trice32 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: Trice32(iD(15429), "rd:Trice32 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:Trice32 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: Trice32(iD(15430), "rd:Trice32 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:Trice32 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: Trice32(iD(15431), "rd:Trice32 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:Trice32 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: Trice32(iD(15432), "rd:Trice32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:Trice32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: Trice32(iD(15433), "rd:Trice32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:Trice32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: Trice32(iD(15434), "rd:Trice32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:Trice32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRice32(iD(15435), "rd:TRice32 %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRice32 -1     
        break; case __LINE__: TRice32(iD(15436), "rd:TRice32 %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRice32 -1, -2 
        break; case __LINE__: TRice32(iD(15437), "rd:TRice32 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRice32 -1, -2, -3       
        break; case __LINE__: TRice32(iD(15438), "rd:TRice32 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRice32 -1, -2, -3, -4      
        break; case __LINE__: TRice32(iD(15439), "rd:TRice32 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRice32 -1, -2, -3, -4, -5        
        break; case __LINE__: TRice32(iD(15440), "rd:TRice32 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRice32 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRice32(iD(15441), "rd:TRice32 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRice32 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRice32(iD(15442), "rd:TRice32 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRice32 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRice32(iD(15443), "rd:TRice32 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRice32 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRice32(iD(15444), "rd:TRice32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRice32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRice32(iD(15445), "rd:TRice32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRice32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRice32(iD(15446), "rd:TRice32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRice32 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: trice     (iD(15447), "signal:trice32_n with 1 to 12 values (line %d)\n", __LINE__ );

        break; case __LINE__: trice32_1 (iD(15448), "rd:trice32_1  %d\n", -1 );                                                                                            //exp: time:            default: rd:trice32_1  -1     
        break; case __LINE__: trice32_2 (iD(15449), "rd:trice32_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:            default: rd:trice32_2  -1, -2 
        break; case __LINE__: trice32_3 (iD(15450), "rd:trice32_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: rd:trice32_3  -1, -2, -3       
        break; case __LINE__: trice32_4 (iD(15451), "rd:trice32_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:            default: rd:trice32_4  -1, -2, -3, -4      
        break; case __LINE__: trice32_5 (iD(15452), "rd:trice32_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:            default: rd:trice32_5  -1, -2, -3, -4, -5        
        break; case __LINE__: trice32_6 (iD(15453), "rd:trice32_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: rd:trice32_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice32_7 (iD(15454), "rd:trice32_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:            default: rd:trice32_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice32_8 (iD(15455), "rd:trice32_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:            default: rd:trice32_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice32_9 (iD(15456), "rd:trice32_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: rd:trice32_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice32_10(iD(15457), "rd:trice32_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:            default: rd:trice32_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice32_11(iD(15458), "rd:trice32_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:            default: rd:trice32_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice32_12(iD(15459), "rd:trice32_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: rd:trice32_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: Trice32_1 (iD(15460), "rd:Trice32_1  %d\n", -1 );                                                                                            //exp: time:    be16default: rd:Trice32_1  -1     
        break; case __LINE__: Trice32_2 (iD(15461), "rd:Trice32_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:Trice32_2  -1, -2 
        break; case __LINE__: Trice32_3 (iD(15462), "rd:Trice32_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:Trice32_3  -1, -2, -3       
        break; case __LINE__: Trice32_4 (iD(15463), "rd:Trice32_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:Trice32_4  -1, -2, -3, -4      
        break; case __LINE__: Trice32_5 (iD(15464), "rd:Trice32_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:Trice32_5  -1, -2, -3, -4, -5        
        break; case __LINE__: Trice32_6 (iD(15465), "rd:Trice32_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:Trice32_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: Trice32_7 (iD(15466), "rd:Trice32_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:Trice32_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: Trice32_8 (iD(15467), "rd:Trice32_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:Trice32_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: Trice32_9 (iD(15468), "rd:Trice32_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:Trice32_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: Trice32_10(iD(15469), "rd:Trice32_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:Trice32_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: Trice32_11(iD(15470), "rd:Trice32_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:Trice32_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: Trice32_12(iD(15471), "rd:Trice32_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:Trice32_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRice32_1 (iD(15472), "rd:TRice32_1  %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRice32_1  -1     
        break; case __LINE__: TRice32_2 (iD(15473), "rd:TRice32_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRice32_2  -1, -2 
        break; case __LINE__: TRice32_3 (iD(15474), "rd:TRice32_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRice32_3  -1, -2, -3       
        break; case __LINE__: TRice32_4 (iD(15475), "rd:TRice32_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRice32_4  -1, -2, -3, -4      
        break; case __LINE__: TRice32_5 (iD(15476), "rd:TRice32_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRice32_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRice32_6 (iD(15477), "rd:TRice32_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRice32_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRice32_7 (iD(15478), "rd:TRice32_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRice32_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRice32_8 (iD(15479), "rd:TRice32_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRice32_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRice32_9 (iD(15480), "rd:TRice32_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRice32_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRice32_10(iD(15481), "rd:TRice32_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRice32_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRice32_11(iD(15482), "rd:TRice32_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRice32_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRice32_12(iD(15483), "rd:TRice32_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRice32_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12

        break; case __LINE__: trice  (iD(15484), "sig:TRICE64 with 1 to 12 values (line %d)\n", __LINE__ );

        break; case __LINE__: TRICE64(id(15485), "rd:TRICE64 %d\n", -1 );                                                                                            //exp: time:            default: rd:TRICE64 -1     
        break; case __LINE__: TRICE64(id(15486), "rd:TRICE64 %d, %d\n", -1, -2 );                                                                                    //exp: time:            default: rd:TRICE64 -1, -2 
        break; case __LINE__: TRICE64(id(15487), "rd:TRICE64 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: rd:TRICE64 -1, -2, -3       
        break; case __LINE__: TRICE64(id(15488), "rd:TRICE64 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:            default: rd:TRICE64 -1, -2, -3, -4      
        break; case __LINE__: TRICE64(id(15489), "rd:TRICE64 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:            default: rd:TRICE64 -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE64(id(15490), "rd:TRICE64 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: rd:TRICE64 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE64(id(15491), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:            default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE64(id(15492), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:            default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE64(id(15493), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE64(id(15494), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:            default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE64(id(15495), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:            default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE64(id(15496), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE64(Id(15497), "rd:TRICE64 %d\n", -1 );                                                                                            //exp: time:    be16default: rd:TRICE64 -1     
        break; case __LINE__: TRICE64(Id(15498), "rd:TRICE64 %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:TRICE64 -1, -2 
        break; case __LINE__: TRICE64(Id(15499), "rd:TRICE64 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:TRICE64 -1, -2, -3       
        break; case __LINE__: TRICE64(Id(15500), "rd:TRICE64 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:TRICE64 -1, -2, -3, -4      
        break; case __LINE__: TRICE64(Id(15501), "rd:TRICE64 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:TRICE64 -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE64(Id(15502), "rd:TRICE64 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:TRICE64 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE64(Id(15503), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE64(Id(15504), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE64(Id(15505), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE64(Id(15506), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE64(Id(15507), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE64(Id(15508), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE64(ID(15509), "rd:TRICE64 %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRICE64 -1     
        break; case __LINE__: TRICE64(ID(15510), "rd:TRICE64 %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRICE64 -1, -2 
        break; case __LINE__: TRICE64(ID(15511), "rd:TRICE64 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRICE64 -1, -2, -3       
        break; case __LINE__: TRICE64(ID(15512), "rd:TRICE64 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRICE64 -1, -2, -3, -4      
        break; case __LINE__: TRICE64(ID(15513), "rd:TRICE64 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRICE64 -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE64(ID(15514), "rd:TRICE64 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRICE64 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE64(ID(15515), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE64(ID(15516), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE64(ID(15517), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE64(ID(15518), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE64(ID(15519), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE64(ID(15520), "rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRICE64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: trice     (iD(15521), "signal:TRICE64_n with 1 to 12 values (line %d)\n", __LINE__ );

        break; case __LINE__: TRICE64_1 (id(15522), "rd:TRICE64_1  %d\n", -1 );                                                                                            //exp: time:            default: rd:TRICE64_1  -1     
        break; case __LINE__: TRICE64_2 (id(15523), "rd:TRICE64_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:            default: rd:TRICE64_2  -1, -2 
        break; case __LINE__: TRICE64_3 (id(15524), "rd:TRICE64_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: rd:TRICE64_3  -1, -2, -3       
        break; case __LINE__: TRICE64_4 (id(15525), "rd:TRICE64_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:            default: rd:TRICE64_4  -1, -2, -3, -4      
        break; case __LINE__: TRICE64_5 (id(15526), "rd:TRICE64_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:            default: rd:TRICE64_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE64_6 (id(15527), "rd:TRICE64_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: rd:TRICE64_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE64_7 (id(15528), "rd:TRICE64_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:            default: rd:TRICE64_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE64_8 (id(15529), "rd:TRICE64_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:            default: rd:TRICE64_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE64_9 (id(15530), "rd:TRICE64_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: rd:TRICE64_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE64_10(id(15531), "rd:TRICE64_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:            default: rd:TRICE64_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE64_11(id(15532), "rd:TRICE64_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:            default: rd:TRICE64_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE64_12(id(15533), "rd:TRICE64_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: rd:TRICE64_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE64_1 (Id(15534), "rd:TRICE64_1  %d\n", -1 );                                                                                            //exp: time:    be16default: rd:TRICE64_1  -1     
        break; case __LINE__: TRICE64_2 (Id(15535), "rd:TRICE64_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:TRICE64_2  -1, -2 
        break; case __LINE__: TRICE64_3 (Id(15536), "rd:TRICE64_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:TRICE64_3  -1, -2, -3       
        break; case __LINE__: TRICE64_4 (Id(15537), "rd:TRICE64_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:TRICE64_4  -1, -2, -3, -4      
        break; case __LINE__: TRICE64_5 (Id(15538), "rd:TRICE64_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:TRICE64_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE64_6 (Id(15539), "rd:TRICE64_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:TRICE64_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE64_7 (Id(15540), "rd:TRICE64_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:TRICE64_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE64_8 (Id(15541), "rd:TRICE64_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:TRICE64_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE64_9 (Id(15542), "rd:TRICE64_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:TRICE64_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE64_10(Id(15543), "rd:TRICE64_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:TRICE64_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE64_11(Id(15544), "rd:TRICE64_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:TRICE64_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE64_12(Id(15545), "rd:TRICE64_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:TRICE64_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRICE64_1 (ID(15546), "rd:TRICE64_1  %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRICE64_1  -1     
        break; case __LINE__: TRICE64_2 (ID(15547), "rd:TRICE64_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRICE64_2  -1, -2 
        break; case __LINE__: TRICE64_3 (ID(15548), "rd:TRICE64_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRICE64_3  -1, -2, -3       
        break; case __LINE__: TRICE64_4 (ID(15549), "rd:TRICE64_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRICE64_4  -1, -2, -3, -4      
        break; case __LINE__: TRICE64_5 (ID(15550), "rd:TRICE64_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRICE64_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE64_6 (ID(15551), "rd:TRICE64_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRICE64_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE64_7 (ID(15552), "rd:TRICE64_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRICE64_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE64_8 (ID(15553), "rd:TRICE64_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRICE64_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE64_9 (ID(15554), "rd:TRICE64_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRICE64_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE64_10(ID(15555), "rd:TRICE64_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRICE64_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE64_11(ID(15556), "rd:TRICE64_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRICE64_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE64_12(ID(15557), "rd:TRICE64_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRICE64_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: trice  (iD(15558), "sig:trice64 with 1 to 12 values (line %d)\n", __LINE__ );

        break; case __LINE__: trice64(iD(15559), "rd:trice64 %d\n", -1 );                                                                                            //exp: time:            default: rd:trice64 -1     
        break; case __LINE__: trice64(iD(15560), "rd:trice64 %d, %d\n", -1, -2 );                                                                                    //exp: time:            default: rd:trice64 -1, -2 
        break; case __LINE__: trice64(iD(15561), "rd:trice64 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: rd:trice64 -1, -2, -3       
        break; case __LINE__: trice64(iD(15562), "rd:trice64 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:            default: rd:trice64 -1, -2, -3, -4      
        break; case __LINE__: trice64(iD(15563), "rd:trice64 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:            default: rd:trice64 -1, -2, -3, -4, -5        
        break; case __LINE__: trice64(iD(15564), "rd:trice64 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: rd:trice64 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice64(iD(15565), "rd:trice64 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:            default: rd:trice64 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice64(iD(15566), "rd:trice64 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:            default: rd:trice64 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice64(iD(15567), "rd:trice64 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: rd:trice64 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice64(iD(15568), "rd:trice64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:            default: rd:trice64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice64(iD(15569), "rd:trice64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:            default: rd:trice64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice64(iD(15570), "rd:trice64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: rd:trice64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: Trice64(iD(15571), "rd:Trice64 %d\n", -1 );                                                                                            //exp: time:    be16default: rd:Trice64 -1     
        break; case __LINE__: Trice64(iD(15572), "rd:Trice64 %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:Trice64 -1, -2 
        break; case __LINE__: Trice64(iD(15573), "rd:Trice64 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:Trice64 -1, -2, -3       
        break; case __LINE__: Trice64(iD(15574), "rd:Trice64 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:Trice64 -1, -2, -3, -4      
        break; case __LINE__: Trice64(iD(15575), "rd:Trice64 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:Trice64 -1, -2, -3, -4, -5        
        break; case __LINE__: Trice64(iD(15576), "rd:Trice64 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:Trice64 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: Trice64(iD(15577), "rd:Trice64 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:Trice64 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: Trice64(iD(15578), "rd:Trice64 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:Trice64 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: Trice64(iD(15579), "rd:Trice64 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:Trice64 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: Trice64(iD(15580), "rd:Trice64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:Trice64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: Trice64(iD(15581), "rd:Trice64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:Trice64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: Trice64(iD(15582), "rd:Trice64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:Trice64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRice64(iD(15583), "rd:TRice64 %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRice64 -1     
        break; case __LINE__: TRice64(iD(15584), "rd:TRice64 %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRice64 -1, -2 
        break; case __LINE__: TRice64(iD(15585), "rd:TRice64 %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRice64 -1, -2, -3       
        break; case __LINE__: TRice64(iD(15586), "rd:TRice64 %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRice64 -1, -2, -3, -4      
        break; case __LINE__: TRice64(iD(15587), "rd:TRice64 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRice64 -1, -2, -3, -4, -5        
        break; case __LINE__: TRice64(iD(15588), "rd:TRice64 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRice64 -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRice64(iD(15589), "rd:TRice64 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRice64 -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRice64(iD(15590), "rd:TRice64 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRice64 -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRice64(iD(15591), "rd:TRice64 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRice64 -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRice64(iD(15592), "rd:TRice64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRice64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRice64(iD(15593), "rd:TRice64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRice64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRice64(iD(15594), "rd:TRice64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRice64 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: trice     (iD(15595), "signal:trice64_n with 1 to 12 values (line %d)\n", __LINE__ );

        break; case __LINE__: trice64_1 (iD(15596), "rd:trice64_1  %d\n", -1 );                                                                                            //exp: time:            default: rd:trice64_1  -1     
        break; case __LINE__: trice64_2 (iD(15597), "rd:trice64_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:            default: rd:trice64_2  -1, -2 
        break; case __LINE__: trice64_3 (iD(15598), "rd:trice64_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:            default: rd:trice64_3  -1, -2, -3       
        break; case __LINE__: trice64_4 (iD(15599), "rd:trice64_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:            default: rd:trice64_4  -1, -2, -3, -4      
        break; case __LINE__: trice64_5 (iD(15600), "rd:trice64_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:            default: rd:trice64_5  -1, -2, -3, -4, -5        
        break; case __LINE__: trice64_6 (iD(15601), "rd:trice64_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:            default: rd:trice64_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: trice64_7 (iD(15602), "rd:trice64_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:            default: rd:trice64_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: trice64_8 (iD(15603), "rd:trice64_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:            default: rd:trice64_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: trice64_9 (iD(15604), "rd:trice64_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:            default: rd:trice64_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: trice64_10(iD(15605), "rd:trice64_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:            default: rd:trice64_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: trice64_11(iD(15606), "rd:trice64_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:            default: rd:trice64_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: trice64_12(iD(15607), "rd:trice64_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:            default: rd:trice64_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: Trice64_1 (iD(15608), "rd:Trice64_1  %d\n", -1 );                                                                                            //exp: time:    be16default: rd:Trice64_1  -1     
        break; case __LINE__: Trice64_2 (iD(15609), "rd:Trice64_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:    be16default: rd:Trice64_2  -1, -2 
        break; case __LINE__: Trice64_3 (iD(15610), "rd:Trice64_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:    be16default: rd:Trice64_3  -1, -2, -3       
        break; case __LINE__: Trice64_4 (iD(15611), "rd:Trice64_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:    be16default: rd:Trice64_4  -1, -2, -3, -4      
        break; case __LINE__: Trice64_5 (iD(15612), "rd:Trice64_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:    be16default: rd:Trice64_5  -1, -2, -3, -4, -5        
        break; case __LINE__: Trice64_6 (iD(15613), "rd:Trice64_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:    be16default: rd:Trice64_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: Trice64_7 (iD(15614), "rd:Trice64_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:    be16default: rd:Trice64_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: Trice64_8 (iD(15615), "rd:Trice64_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:    be16default: rd:Trice64_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: Trice64_9 (iD(15616), "rd:Trice64_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:    be16default: rd:Trice64_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: Trice64_10(iD(15617), "rd:Trice64_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:    be16default: rd:Trice64_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: Trice64_11(iD(15618), "rd:Trice64_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:    be16default: rd:Trice64_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: Trice64_12(iD(15619), "rd:Trice64_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:    be16default: rd:Trice64_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    
        break; case __LINE__: TRice64_1 (iD(15620), "rd:TRice64_1  %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRice64_1  -1     
        break; case __LINE__: TRice64_2 (iD(15621), "rd:TRice64_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRice64_2  -1, -2 
        break; case __LINE__: TRice64_3 (iD(15622), "rd:TRice64_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRice64_3  -1, -2, -3       
        break; case __LINE__: TRice64_4 (iD(15623), "rd:TRice64_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRice64_4  -1, -2, -3, -4      
        break; case __LINE__: TRice64_5 (iD(15624), "rd:TRice64_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRice64_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRice64_6 (iD(15625), "rd:TRice64_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRice64_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRice64_7 (iD(15626), "rd:TRice64_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRice64_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRice64_8 (iD(15627), "rd:TRice64_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRice64_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRice64_9 (iD(15628), "rd:TRice64_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRice64_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRice64_10(iD(15629), "rd:TRice64_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRice64_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRice64_11(iD(15630), "rd:TRice64_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRice64_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRice32_12(iD(15631), "rd:TRice32_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRice32_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: TRICE(ID(15632), "sig:Colors and ticks\n" );
        break; case __LINE__: TRICE(ID(15633), "--------------------------------------------------\n");
        break; case __LINE__: TRICE(ID(15634), "--------------------------------------------------\n");
        break; case __LINE__: TRICE(ID(15635), "dbg:12345 as 16bit is %016b\n", 12345);
        break; case __LINE__: TRICE(ID(15636), "--------------------------------------------------\n");
        break; case __LINE__: TRICE(ID(15637), "sig:This ASSERT error is just a demo and no real error:\n");
        break; case __LINE__: TRICE(ID(15638), "--------------------------------------------------\n");
      //break; case __LINE__: TRICE(ID(15639), "ERR:error       message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE(ID(15640), "WRN:warning     message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE(ID(15641), "ATT:attention   message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE(ID(15642), "DIA:diagnostics message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE(ID(15643), "TIM:timing      message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE(ID(15644), "DBG:debug       message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE(ID(15645), "SIG:signal      message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE(ID(15646), "RD:read         message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE(ID(15647), "WR:write        message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE(ID(15648), "ISR:interrupt   message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE(ID(15649), "MSG:normal      message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE(ID(15650), "INFO:informal   message, SysTick is %6u\n", SYSTICKVAL);
        break; case __LINE__: TRICE(ID(15651), "time:TRICE32_1  message, SysTick is %6u\n", SYSTICKVAL);

        break; case __LINE__: TRICE(ID(15652), "sig:Various ranges and formats\n" );
        break; case __LINE__: TRICE8 (ID(15653), "tst:TRICE8  %%03x -> %03x  %03x  %03x  %03x\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 (ID(15654), "tst:TRICE8   %%4d -> %4d %4d %4d %4d\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 (ID(15655), "tst:TRICE8   %%4u -> %4u %4u %4u %4u\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 (ID(15656), "tst:TRICE8   %%4o -> %4o %4o %4o %4o\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 (ID(15657), "tst:TRICE8   %%4O -> %4O %4O %4O %4O\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 (ID(15658), "tst:TRICE8   %%4X -> %4X %4X %4X %4X\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 (ID(15659), "tst:TRICE8   %%4x -> %4x %4x %4x %4x\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 (ID(15660), "tst:TRICE8   %%8b -> %8b %8b %8b %8b\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE8 (ID(15661), "tst:TRICE8  %%08b -> %08b %08b %08b %08b\n", 1, 0x7f, 0x80, 0xff);
        break; case __LINE__: TRICE16(ID(15662), "tst:TRICE16  %%05x ->   %05x   %05x   %05x   %05x\n", 1, 0x7fff, 0x8000, 0xffff);
        break; case __LINE__: TRICE16(ID(15663), "tst:TRICE16   %%6d ->  %6d  %6d  %6d  %6d\n",         1, 0x7fff, 0x8000, 0xffff);
        break; case __LINE__: TRICE16(ID(15664), "tst:TRICE16   %%7o -> %7o %7o %7o %7o\n",             1, 0x7fff, 0x8000, 0xffff);
        break; case __LINE__: TRICE32(ID(15665), "tst:TRICE32 %%09x ->      %09x      %09x       %09x     %09x\n", 1, 0x7fffffff, 0x80000000, 0xffffffff);
        break; case __LINE__: TRICE32(ID(15666), "tst:TRICE32 %%10d ->     %10d     %10d     %10d    %10x\n",      1, 0x7fffffff, 0x80000000, 0xffffffff);
        break; case __LINE__: TRICE32(ID(15667), "att:TRICE32 %40b\n", 0xAAAAAAAA);
        break; case __LINE__: TRICE64(ID(15668), "tst:TRICE64 %%09x ->      %09x      %09x       %09x     %09x\n", 1, 0x7fffffffffffffff, 0x8000000000000000, 0xffffffffffffffff);
        break; case __LINE__: TRICE64(ID(15669), "tst:TRICE64 %%10d ->     %10d     %10d     %10d    %10x\n",      1, 0x7fffffffffffffff, 0x8000000000000000, 0xffffffffffffffff);
        break; case __LINE__: TRICE64(ID(15670), "att:TRICE64 %70b\n", 0xAAAAAAAAAAAAAAAA);
        break; case __LINE__: TRICE(ID(15671), "sig:Legacy TRICE8\n" );
        break; case __LINE__: TRICE8_1(ID(15672), "tst:TRICE8_1 %02x\n", 0xA1);
        break; case __LINE__: TRICE8_2(ID(15673), "tst:TRICE8_2 %02x %02x\n", 0xA1, 0xA2);
        break; case __LINE__: TRICE8_3(ID(15674), "tst:TRICE8_3 %02x %02x %02x\n", 0xA1, 0xA2, 0xA3);
        break; case __LINE__: TRICE8_4(ID(15675), "tst:TRICE8_4 %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4);
        break; case __LINE__: TRICE8_5(ID(15676), "tst:TRICE8_5 %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5 );
        break; case __LINE__: TRICE8_6(ID(15677), "tst:TRICE8_6 %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6);
        break; case __LINE__: TRICE8_7(ID(15678), "tst:TRICE8_7 %02x %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7);
        break; case __LINE__: TRICE8_8(ID(15679), "tst:TRICE8_8 %02X %02X %02X %02X %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8); //exp: time:feed3322default: tst:TRICE8_8 A1 A2 A3 A4 a5 a6 a7 a8
        break; case __LINE__: TRICE8_1(ID(15680), "tst:TRICE8_1 %d\n",                      -1);
        break; case __LINE__: TRICE8_2(ID(15681), "tst:TRICE8_2 %d %d\n",                   -1, -2);
        break; case __LINE__: TRICE8_3(ID(15682), "tst:TRICE8_3 %d %d %d\n",                -1, -2, -3);
        break; case __LINE__: TRICE8_4(ID(15683), "tst:TRICE8_4 %d %d %d %d\n",             -1, -2, -3, -4);
        break; case __LINE__: TRICE8_5(ID(15684), "tst:TRICE8_5 %d %d %d %d %d\n",          -1, -2, -3, -4, -5);
        break; case __LINE__: TRICE8_6(ID(15685), "tst:TRICE8_6 %d %d %d %d %d %d\n",       -1, -2, -3, -4, -5, -6);
        break; case __LINE__: TRICE8_7(ID(15686), "tst:TRICE8_7 %d %d %d %d %d %d %d\n",    -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE8_8(ID(15687), "tst:TRICE8_8 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8);
        break; case __LINE__: TRICE8_1(ID(15688), "tst:TRICE8_1 %d\n", 1);
        break; case __LINE__: TRICE8_2(ID(15689), "tst:TRICE8_2 %d %d\n", 1, 2);
        break; case __LINE__: TRICE8_3(ID(15690), "tst:TRICE8_3 %d %d %d\n", 1, 2, 3);
        break; case __LINE__: TRICE8_4(ID(15691), "tst:TRICE8_4 %d %d %d %d\n", 1, 2, 3, 4);
        break; case __LINE__: TRICE8_5(ID(15692), "tst:TRICE8_5 %d %d %d %d %d\n", 1, 2, 3, 4, 5);
        break; case __LINE__: TRICE8_6(ID(15693), "tst:TRICE8_6 %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6);
        break; case __LINE__: TRICE8_7(ID(15694), "tst:TRICE8_7 %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7);
        break; case __LINE__: TRICE8_8(ID(15695), "tst:TRICE8_8 %d %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7, 8);
        break; case __LINE__: TRICE8_1(ID(15696), "tst:TRICE8_1 %u\n", 201);
        break; case __LINE__: TRICE8_2(ID(15697), "tst:TRICE8_2 %u %u\n", 201, 202);
        break; case __LINE__: TRICE8_3(ID(15698), "tst:TRICE8_3 %u %u %u\n", 201, 202, 203);
        break; case __LINE__: TRICE8_4(ID(15699), "tst:TRICE8_4 %u %u %u %u\n", 201, 202, 203, 204);
        break; case __LINE__: TRICE8_5(ID(15700), "tst:TRICE8_5 %u %u %u %u %u\n", 201, 202, 203, 204, 205);
        break; case __LINE__: TRICE8_6(ID(15701), "tst:TRICE8_6 %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206);
        break; case __LINE__: TRICE8_7(ID(15702), "tst:TRICE8_7 %u %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206, 207);
        break; case __LINE__: TRICE8_8(ID(15703), "tst:TRICE8_8 %u %u %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206, 207, 208);
        break; case __LINE__: TRICE8_8(ID(15704), "tst:TRICE8_1 %%d=%d, %%u=%u, 0x%%x=0x%x, 0x%%2x=0x%2x, 0x%%02x=0x%02x, 0x%%3x=0x%3x, 0x%%03x=0x%03x, %%b=%b\n", 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81);

        break; case __LINE__: TRICE    (ID(15705), "sig:Legacy TRICE16-64\n" );
        break; case __LINE__: TRICE16_1(ID(15706), "tst:TRICE16_1 %d\n", -111);
        break; case __LINE__: TRICE16_2(ID(15707), "tst:TRICE16_2 %d %d\n", -111, -222);
        break; case __LINE__: TRICE16_3(ID(15708), "tst:TRICE16_3 %d %d %d\n", -111, -222, -333);
        break; case __LINE__: TRICE16_4(ID(15709), "tst:TRICE16_4 %d %d %d %d\n", -111, -222, -333, -444);
        break; case __LINE__: TRICE32_1(ID(15710), "tst:TRICE32_1 %08x\n", 0x0123cafe);
        break; case __LINE__: TRICE32_1(ID(15711), "tst:TRICE32_1 %d\n", -111);
        break; case __LINE__: TRICE32_2(ID(15712), "tst:TRICE32_2 %x %x\n", -111, -222);
        break; case __LINE__: TRICE32_2(ID(15713), "tst:TRICE32_2 %d %d\n", -111, -222);
        break; case __LINE__: TRICE32_3(ID(15714), "tst:TRICE32_3 %x %x %x\n", -111, -222, -333);
        break; case __LINE__: TRICE32_3(ID(15715), "tst:TRICE32_3 %d %d %d\n", -111, -222, -333);
        break; case __LINE__: TRICE32_4(ID(15716), "tst:TRICE32_4 %x %x %x %x\n", -111, -222, -333, -444);
        break; case __LINE__: TRICE32_4(ID(15717), "tst:TRICE32_4 %d %d %d %d\n", -111, -222, -333, -444);
        break; case __LINE__: TRICE64_1(ID(15718), "tst:TRICE64_1 %d\n", -111);
        break; case __LINE__: TRICE64_2(ID(15719), "tst:TRICE64_2 %d %d\n", -111, -222);
        break; case __LINE__: TRICE16_1(ID(15720), "tst:TRICE16_1 %u\n", 60001);
        break; case __LINE__: TRICE16_2(ID(15721), "tst:TRICE16_2 %u %u\n", 60001, 60002);
        break; case __LINE__: TRICE16_3(ID(15722), "tst:TRICE16_3 %u %u %u\n", 60001, 60002, 60003);
        break; case __LINE__: TRICE16_4(ID(15723), "tst:TRICE16_4 %u %u %u %u\n", 60001, 60002, 60003, 60004);
        break; case __LINE__: TRICE32_1(ID(15724), "tst:TRICE32_1 %u\n", 4000000001);
        break; case __LINE__: TRICE32_2(ID(15725), "tst:TRICE32_2 %u %u\n", 4000000001, 4000000002);
        break; case __LINE__: TRICE32_3(ID(15726), "tst:TRICE32_3 %u %u %u\n", 4000000001, 4000000002, 4000000003);
        break; case __LINE__: TRICE32_4(ID(15727), "tst:TRICE32_4 %u %u %u %u\n", 4000000001, 4000000002, 4000000003, 4000000004);
        break; case __LINE__: TRICE64_1(ID(15728), "tst:TRICE64_1 %x\n", -1);
        break; case __LINE__: TRICE64_2(ID(15729), "tst:TRICE64_2 %x %x\n", -1, -2);
        break; case __LINE__: TRICE64_1(ID(15730), "tst:TRICE64_1 %u\n", -1);
        break; case __LINE__: TRICE64_2(ID(15731), "tst:TRICE64_2 %u %u\n", -1, -2);

        break; case __LINE__: TRICE(ID(15732), "sig:colored single letters and several TRICE macros in one line\n" );
        break; case __LINE__: TRICE(ID(15733), "e:A");
        break; case __LINE__: TRICE(ID(15734), "w:B");
        break; case __LINE__: TRICE(ID(15735), "a:c");
        break; case __LINE__: TRICE(ID(15736), "wr:d");
        break; case __LINE__: TRICE(ID(15737), "rd:e\n");
        break; case __LINE__: TRICE(ID(15738), "diag:f");
        break; case __LINE__: TRICE(ID(15739), "d:G");
        break; case __LINE__: TRICE(ID(15740), "t:H");
        break; case __LINE__: TRICE(ID(15741), "time:i");
        break; case __LINE__: TRICE(ID(15742), "message:J");
        break; case __LINE__: TRICE(ID(15743), "dbg:k\n");
        break; case __LINE__: TRICE(ID(15744), "1");
        break; case __LINE__: TRICE(ID(15745), "2");
        break; case __LINE__: TRICE(ID(15746), "3");
        break; case __LINE__: TRICE(ID(15747), "4");
        break; case __LINE__: TRICE(ID(15748), "e:7");
        break; case __LINE__: TRICE(ID(15749), "m:12");
        break; case __LINE__: TRICE(ID(15750), "m:123\n");
        break; case __LINE__: TRICE(ID(15751), "e:A"); TRICE(ID(15752), "w:B"); TRICE(ID(15753), "a:c");
        break; case __LINE__: TRICE(ID(15754), "wr:d"); TRICE(ID(15755), "rd:e\n"); TRICE(ID(15756), "diag:f");

        break; case __LINE__: TRICE(ID(15757), "sig:TRICE8 with variable param count 1 to 12\n" );
        break; case __LINE__: TRICE8(ID(15758), "tst:TRICE8 %d\n", -1 );
        break; case __LINE__: TRICE8(ID(15759), "tst:TRICE8 %d %d\n", -1, -2 );
        break; case __LINE__: TRICE8(ID(15760), "tst:TRICE8 %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE8(ID(15761), "tst:TRICE8 %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE8(ID(15762), "tst:TRICE8 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE8(ID(15763), "tst:TRICE8 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE8(ID(15764), "tst:TRICE8 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE8(ID(15765), "tst:TRICE8 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE8(ID(15766), "tst:TRICE8 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE8(ID(15767), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE8(ID(15768), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE8(ID(15769), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE8(ID(15770), "tst:TRICE8 %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );
        break; case __LINE__: TRICE8_1 (ID(15771), "tst:TRICE8_1  %d\n", -1 );
        break; case __LINE__: TRICE8_2 (ID(15772), "tst:TRICE8_2  %d %d\n", -1, -2 );
        break; case __LINE__: TRICE8_3 (ID(15773), "tst:TRICE8_3  %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE8_4 (ID(15774), "tst:TRICE8_4  %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE8_5 (ID(15775), "tst:TRICE8_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE8_6 (ID(15776), "tst:TRICE8_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE8_7 (ID(15777), "tst:TRICE8_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE8_8 (ID(15778), "tst:TRICE8_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE8_9 (ID(15779), "tst:TRICE8_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE8_10(ID(15780), "tst:TRICE8_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE8_11(ID(15781), "tst:TRICE8_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE8_12(ID(15782), "tst:TRICE8_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE8   (ID(15783), "tst:TRICE8   %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );
        break; case __LINE__: TRICE8_9 (ID(15784), "tst:TRICE8_9 %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );

        break; case __LINE__: TRICE(ID(15785), "sig:TRICE16 with variable param count 1 to 12\n" );
        break; case __LINE__: TRICE16(ID(15786), "tst:TRICE16 %d\n", -1 );
        break; case __LINE__: TRICE16(ID(15787), "tst:TRICE16 %d %d\n", -1, -2 );
        break; case __LINE__: TRICE16(ID(15788), "tst:TRICE16 %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE16(ID(15789), "tst:TRICE16 %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE16(ID(15790), "tst:TRICE16 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE16(ID(15791), "tst:TRICE16 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE16(ID(15792), "tst:TRICE16 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE16(ID(15793), "tst:TRICE16 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE16(ID(15794), "tst:TRICE16 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE16(ID(15795), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE16(ID(15796), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE16(ID(15797), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE16(ID(15798), "tst:TRICE16 %t %b %x %X %d %u %o %O %p\n", 3, 3, 3, 3, 3, 3, 3, 3, 3 );
        break; case __LINE__: TRICE16_1 (ID(15799), "tst:TRICE16_1  %d\n", -1 );
        break; case __LINE__: TRICE16_2 (ID(15800), "tst:TRICE16_2  %d %d\n", -1, -2 );
        break; case __LINE__: TRICE16_3 (ID(15801), "tst:TRICE16_3  %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE16_4 (ID(15802), "tst:TRICE16_4  %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE16_5 (ID(15803), "tst:TRICE16_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE16_6 (ID(15804), "tst:TRICE16_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE16_7 (ID(15805), "tst:TRICE16_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE16_8 (ID(15806), "tst:TRICE16_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE16_9 (ID(15807), "tst:TRICE16_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE16_10(ID(15808), "tst:TRICE16_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE16_11(ID(15809), "tst:TRICE16_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE16_12(ID(15810), "tst:TRICE16_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE16_9 (ID(15811), "tst:TRICE16 %t %b %x %X %d %u %o %O %p\n", 3, 3, 3, 3, 3, 3, 3, 3, 3 );

        break; case __LINE__: TRICE(ID(15812), "sig:TRICE32 with variable param count 1 to 12\n" );
        break; case __LINE__: TRICE32(ID(15813), "tst:TRICE32 %d\n", -1 );
        break; case __LINE__: TRICE32(ID(15814), "tst:TRICE32 %d %d\n", -1, -2 );
        break; case __LINE__: TRICE32(ID(15815), "tst:TRICE32 %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE32(ID(15816), "tst:TRICE32 %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE32(ID(15817), "tst:TRICE32 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE32(ID(15818), "tst:TRICE32 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE32(ID(15819), "tst:TRICE32 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE32(ID(15820), "tst:TRICE32 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE32(ID(15821), "tst:TRICE32 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE32(ID(15822), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE32(ID(15823), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE32(ID(15824), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE32(ID(15825), "tst:TRICE32 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));
        break; case __LINE__: TRICE32_1 (ID(15826), "tst:TRICE32_1  %d\n", -1 );
        break; case __LINE__: TRICE32_2 (ID(15827), "tst:TRICE32_2  %d %d\n", -1, -2 );
        break; case __LINE__: TRICE32_3 (ID(15828), "tst:TRICE32_3  %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE32_4 (ID(15829), "tst:TRICE32_4  %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE32_5 (ID(15830), "tst:TRICE32_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE32_6 (ID(15831), "tst:TRICE32_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE32_7 (ID(15832), "tst:TRICE32_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE32_8 (ID(15833), "tst:TRICE32_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE32_9 (ID(15834), "tst:TRICE32_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE32_10(ID(15835), "tst:TRICE32_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE32_11(ID(15836), "tst:TRICE32_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE32_12(ID(15837), "tst:TRICE32_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE32   (ID(15838), "tst:TRICE32    %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));
        break; case __LINE__: TRICE32_12(ID(15839), "tst:TRICE32_12 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));

        break; case __LINE__: TRICE(ID(15840), "sig:TRICE64 with variable param count 1 to 12\n" );
        break; case __LINE__: TRICE64(ID(15841), "tst:TRICE64 %d\n", -1 );
        break; case __LINE__: TRICE64(ID(15842), "tst:TRICE64 %d %d\n", -1, -2 );
        break; case __LINE__: TRICE64(ID(15843), "tst:TRICE64 %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE64(ID(15844), "tst:TRICE64 %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE64(ID(15845), "tst:TRICE64 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE64(ID(15846), "tst:TRICE64 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE64(ID(15847), "tst:TRICE64 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE64(ID(15848), "tst:TRICE64 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE64(ID(15849), "tst:TRICE64 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE64(ID(15850), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE64(ID(15851), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE64(ID(15852), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE64(ID(15853), "tst:TRICE64 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aDouble(3.14159), aDouble(3.14159), aDouble(3.14159));
        break; case __LINE__: TRICE64_1 (ID(15854), "tst:TRICE64_1  %d\n", -1 );
        break; case __LINE__: TRICE64_2 (ID(15855), "tst:TRICE64_2  %d %d\n", -1, -2 );
        break; case __LINE__: TRICE64_3 (ID(15856), "tst:TRICE64_3  %d %d %d\n", -1, -2, -3 );
        break; case __LINE__: TRICE64_4 (ID(15857), "tst:TRICE64_4  %d %d %d %d\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE64_5 (ID(15858), "tst:TRICE64_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE64_6 (ID(15859), "tst:TRICE64_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE64_7 (ID(15860), "tst:TRICE64_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
        break; case __LINE__: TRICE64_8 (ID(15861), "tst:TRICE64_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE64_9 (ID(15862), "tst:TRICE64_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE64_10(ID(15863), "tst:TRICE64_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
        break; case __LINE__: TRICE64_11(ID(15864), "tst:TRICE64_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE64_12(ID(15865), "tst:TRICE64_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
        break; case __LINE__: TRICE64_12(ID(15866), "tst:TRICE64_12 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aDouble(3.14159), aDouble(3.14159), aDouble(3.14159));

        break; case __LINE__: TRICE(ID(15867), "att:positive and negative float in format variants\n");
        break; case __LINE__: TRICE(ID(15868), "rd:TRICE float %f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE(ID(15869), "rd:TRICE float %9f (%%9f)\n", aFloat(x) );
        break; case __LINE__: TRICE(ID(15870), "rd:TRICE float %.9f (%%.9f)\n", aFloat(x) );
        break; case __LINE__: TRICE(ID(15871), "rd:TRICE float %9.f (%%9.f)\n", aFloat(x) );
        break; case __LINE__: TRICE(ID(15872), "rd:TRICE float %9.6f (%%9.6f)\n", aFloat(x) );
        break; case __LINE__: TRICE(ID(15873), "rd:TRICE float %f (%%f)\n", aFloat(-x) );
        break; case __LINE__: TRICE(ID(15874), "rd:TRICE float %9f (%%9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE(ID(15875), "rd:TRICE float %.9f (%%.9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE(ID(15876), "rd:TRICE float %9.f (%%9.f)\n", aFloat(-x) );
        break; case __LINE__: TRICE(ID(15877), "rd:TRICE float %9.6f (%%9.6f)\n", aFloat(-x) );
        break; case __LINE__: TRICE(ID(15878), "rd:TRICE float %+f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE(ID(15879), "rd:TRICE float %+9f (%%9f)\n", aFloat(x) );
        break; case __LINE__: TRICE(ID(15880), "rd:TRICE float %+.9f (%%.9f)\n", aFloat(x) );
        break; case __LINE__: TRICE(ID(15881), "rd:TRICE float %+9.f (%%9.f)\n", aFloat(x) );
        break; case __LINE__: TRICE(ID(15882), "rd:TRICE float %+9.6f (%%9.6f)\n", aFloat(x) );
        break; case __LINE__: TRICE(ID(15883), "rd:TRICE float %+f (%%f)\n", aFloat(-x) );
        break; case __LINE__: TRICE(ID(15884), "rd:TRICE float %+9f (%%9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE(ID(15885), "rd:TRICE float %+.9f (%%.9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE(ID(15886), "rd:TRICE float %+9.f (%%9.f)\n", aFloat(-x) );
        break; case __LINE__: TRICE(ID(15887), "rd:TRICE float %+9.6f (%%9.6f)\n", aFloat(-x) );
        break; case __LINE__: TRICE(ID(15888), "rd:TRICE float %-f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE(ID(15889), "rd:TRICE float %-9f (%%9f)\n", aFloat(x) );
        break; case __LINE__: TRICE(ID(15890), "rd:TRICE float %-.9f (%%.9f)\n", aFloat(x) );
        break; case __LINE__: TRICE(ID(15891), "rd:TRICE float %-9.f (%%9.f)\n", aFloat(x) );
        break; case __LINE__: TRICE(ID(15892), "rd:TRICE float %-9.6f (%%9.6f)\n", aFloat(x) );
        break; case __LINE__: TRICE(ID(15893), "rd:TRICE float %-f (%%f)\n", aFloat(-x) );
        break; case __LINE__: TRICE(ID(15894), "rd:TRICE float %-9f (%%9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE(ID(15895), "rd:TRICE float %-.9f (%%.9f)\n", aFloat(-x) );
        break; case __LINE__: TRICE(ID(15896), "rd:TRICE float %-9.f (%%9.f)\n", aFloat(-x) );
        break; case __LINE__: TRICE(ID(15897), "rd:TRICE float %-9.6f (%%9.6f)\n", aFloat(-x) );
        break; case __LINE__: TRICE(ID(15898), "att:positive float & double in variants\n");
        break; case __LINE__: TRICE32_1(ID(15899), "rd:TRICE32_1 float %e (%%e)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1(ID(15900), "rd:TRICE32_1 float %f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1(ID(15901), "rd:TRICE32_1 float %g (%%g)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1(ID(15902), "rd:TRICE32_1 float %E (%%E)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1(ID(15903), "rd:TRICE32_1 float %F (%%F)\n", aFloat(x) );
        break; case __LINE__: TRICE32_1(ID(15904), "rd:TRICE32_1 float %G (%%G)\n", aFloat(x) );
        break; case __LINE__: TRICE32  (ID(15905), "rd:TRICE32 float %e (%%e)\n", aFloat(x) );
        break; case __LINE__: TRICE32  (ID(15906), "rd:TRICE32 float %f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE32  (ID(15907), "rd:TRICE32 float %g (%%g)\n", aFloat(x) );
        break; case __LINE__: TRICE32  (ID(15908), "rd:TRICE32 float %E (%%E)\n", aFloat(x) );
        break; case __LINE__: TRICE32  (ID(15909), "rd:TRICE32 float %F (%%F)\n", aFloat(x) );
        break; case __LINE__: TRICE32  (ID(15910), "rd:TRICE32 float %G (%%G)\n", aFloat(x) );
        break; case __LINE__: TRICE    (ID(15911), "rd:TRICE float %e (%%e)\n", aFloat(x) );
        break; case __LINE__: TRICE    (ID(15912), "rd:TRICE float %f (%%f)\n", aFloat(x) );
        break; case __LINE__: TRICE    (ID(15913), "rd:TRICE float %g (%%g)\n", aFloat(x) );
        break; case __LINE__: TRICE    (ID(15914), "rd:TRICE float %E (%%E)\n", aFloat(x) );
        break; case __LINE__: TRICE    (ID(15915), "rd:TRICE float %F (%%F)\n", aFloat(x) );
        break; case __LINE__: TRICE    (ID(15916), "rd:TRICE float %G (%%G)\n", aFloat(x) );
        break; case __LINE__: TRICE64  (ID(15917), "rd:TRICE64 double %e (%%e), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64  (ID(15918), "rd:TRICE64 double %f (%%f), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64  (ID(15919), "rd:TRICE64 double %g (%%g), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64  (ID(15920), "rd:TRICE64 double %E (%%E), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64  (ID(15921), "rd:TRICE64 double %F (%%F), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64  (ID(15922), "rd:TRICE64 double %G (%%G), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1(ID(15923), "rd:TRICE64_1 double %e (%%e), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1(ID(15924), "rd:TRICE64_1 double %f (%%f), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1(ID(15925), "rd:TRICE64_1 double %g (%%g), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1(ID(15926), "rd:TRICE64_1 double %E (%%E), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1(ID(15927), "rd:TRICE64_1 double %F (%%F), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64_1(ID(15928), "rd:TRICE64_1 double %G (%%G), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE(ID(15929), "att:negative float & double\n" );
        break; case __LINE__: TRICE(ID(15930), "rd:TRICE float %e (%%e)\n", aFloat(-x) );
        break; case __LINE__: TRICE(ID(15931), "rd:TRICE float %f (%%f)\n", aFloat(-x) );
        break; case __LINE__: TRICE(ID(15932), "rd:TRICE float %g (%%g)\n", aFloat(-x) );
        break; case __LINE__: TRICE(ID(15933), "rd:TRICE float %E (%%E)\n", aFloat(-x) );
        break; case __LINE__: TRICE(ID(15934), "rd:TRICE float %F (%%F)\n", aFloat(-x) );
        break; case __LINE__: TRICE(ID(15935), "rd:TRICE float %G (%%G)\n", aFloat(-x) );
        break; case __LINE__: TRICE64(ID(15936), "rd:TRICE64 double %e (%%e), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64(ID(15937), "rd:TRICE64 double %f (%%f), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64(ID(15938), "rd:TRICE64 double %g (%%g), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64(ID(15939), "rd:TRICE64 double %E (%%E), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64(ID(15940), "rd:TRICE64 double %F (%%F), aDouble(y)\n", aDouble(-y) );
        break; case __LINE__: TRICE64(ID(15941), "rd:TRICE64 double %G (%%G), aDouble(y)\n", aDouble(-y) );

        break; case __LINE__: TRICE  (ID(15942), "att:formatted float & double\n" );
        break; case __LINE__: TRICE32(ID(15943), "rd:TRICE32 float %12.6e (%%12.6e)\n", aFloat(x) );
        break; case __LINE__: TRICE32(ID(15944), "rd:TRICE32 float %12.6f (%%12.6f)\n", aFloat(x) );
        break; case __LINE__: TRICE32(ID(15945), "rd:TRICE32 float %12.6g (%%12.6g)\n", aFloat(x) );
        break; case __LINE__: TRICE32(ID(15946), "rd:TRICE32 float %12.6E (%%12.6E)\n", aFloat(x) );
        break; case __LINE__: TRICE32(ID(15947), "rd:TRICE32 float %12.6F (%%12.6F)\n", aFloat(x) );
        break; case __LINE__: TRICE32(ID(15948), "rd:TRICE32 float %12.6G (%%12.6G)\n", aFloat(x) );
        break; case __LINE__: TRICE64(ID(15949), "rd:TRICE64 double %12.6e (%%12.6e), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64(ID(15950), "rd:TRICE64 double %12.6f (%%12.6f), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64(ID(15951), "rd:TRICE64 double %12.6g (%%12.6g), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64(ID(15952), "rd:TRICE64 double %12.6E (%%12.6E), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64(ID(15953), "rd:TRICE64 double %12.6F (%%12.6F), aDouble(y)\n", aDouble(y) );
        break; case __LINE__: TRICE64(ID(15954), "rd:TRICE64 double %12.6G (%%12.6G), aDouble(y)\n", aDouble(y) );

        break; case __LINE__: TRICE (ID(15955), "att:Various single arguments\n" );
        break; case __LINE__: TRICE8(ID(15956), "rd:TRICE8 line %t (%%t ,0)\n", 0 );
        break; case __LINE__: TRICE8(ID(15957), "rd:TRICE8 line %t (%%t ,2)\n", 2 );
        break; case __LINE__: TRICE8(ID(15958), "rd:TRICE8 line %u (%%u ,-1)\n", -1 );
        break; case __LINE__: TRICE8(ID(15959), "rd:TRICE8 line %b (%%b ,-1)\n", -1 );
        break; case __LINE__: TRICE8(ID(15960), "rd:TRICE8 line %o (%%o ,-1)\n", -1 );
        break; case __LINE__: TRICE8(ID(15961), "rd:TRICE8 line %O (%%O ,-1)\n", -1 );
        break; case __LINE__: TRICE8(ID(15962), "rd:TRICE8 line %X (%%X ,-1)\n", -1 );
        break; case __LINE__: TRICE8(ID(15963), "rd:TRICE8 line %x (%%x ,-1)\n", -1 );
        break; case __LINE__: TRICE8(ID(15964), "rd:TRICE8 line %d (%%d ,-1)\n", -1 );
        break; case __LINE__: TRICE16_1(ID(15965), "rd:TRICE16_1 line %t (%%t ,0)\n", 0 );
        break; case __LINE__: TRICE16_1(ID(15966), "rd:TRICE16_1 line %t (%%t ,2)\n", 2 );
        break; case __LINE__: TRICE16_1(ID(15967), "rd:TRICE16_1 line %u (%%u -1)\n", -1 );
        break; case __LINE__: TRICE16_1(ID(15968), "rd:TRICE16_1 line %b (%%b -1)\n", -1 );
        break; case __LINE__: TRICE16_1(ID(15969), "rd:TRICE16_1 line %o (%%o -1)\n", -1 );
        break; case __LINE__: TRICE16_1(ID(15970), "rd:TRICE16_1 line %x (%%x -1)\n", -1 );
        break; case __LINE__: TRICE16_1(ID(15971), "rd:TRICE16_1 line %d (%%d -1)\n", -1 );
        break; case __LINE__: TRICE(ID(15972), "rd:TRICE line %t (%%t -1)\n", 0 );
        break; case __LINE__: TRICE(ID(15973), "rd:TRICE line %t (%%t -1)\n", 2 );
        break; case __LINE__: TRICE(ID(15974), "rd:TRICE line %u (%%u)\n", -1 );
        break; case __LINE__: TRICE(ID(15975), "rd:TRICE line %b (%%b)\n", -2 );
        break; case __LINE__: TRICE(ID(15976), "rd:TRICE line %o (%%o)\n", -3 );
        break; case __LINE__: TRICE(ID(15977), "rd:TRICE line %x (%%x)\n", -4 );
        break; case __LINE__: TRICE(ID(15978), "rd:TRICE line %d (%%d)\n", -5 );

        break; case __LINE__: TRICE_N(ID(15979), "msg:%s\n", A, 0 );
        break; case __LINE__: TRICE_N(ID(15980), "msg:%s\n", A, 1 );
        break; case __LINE__: TRICE_N(ID(15981), "msg:%s\n", A, 2 );
        break; case __LINE__: TRICE_N(ID(15982), "msg:%s\n", A, 3 );
        break; case __LINE__: TRICE_N(ID(15983), "msg:%s\n", A, 4 );
        break; case __LINE__: TRICE_N(ID(15984), "msg:%s\n", A, 5 );
        break; case __LINE__: TRICE_N(ID(15985), "msg:%s\n", A, 6 );
        break; case __LINE__: TRICE_N(ID(15986), "msg:%s\n", A, 7 );
        break; case __LINE__: TRICE_N(ID(15987), "msg:%s\n", A, 8 );
        break; case __LINE__: TRICE_N(ID(15988), "msg:%s\n", A, 9 );
        break; case __LINE__: TRICE_N(ID(15989), "msg:%s\n", A, 10 );
        break; case __LINE__: TRICE_N(ID(15990), "msg:%s\n", A, 11 );
        break; case __LINE__: TRICE_N(ID(15991), "msg:%s\n", A, 12 );
        break; case __LINE__: TRICE_N(ID(15992), "msg:%s\n", A, 13 );
        break; case __LINE__: TRICE_N(ID(15993), "msg:%s\n", A, 14 );
        break; case __LINE__: TRICE_N(ID(15994), "msg:%s\n", A, 15 );
        break; case __LINE__: TRICE_N(ID(15995), "msg:%s\n", A, 16 );
        break; case __LINE__: TRICE_N(ID(15996), "msg:%s\n", A, 17 );
        break; case __LINE__: TRICE_N(ID(15997), "msg:%s\n", A, 18 );
        break; case __LINE__: TRICE_N(ID(15998), "msg:%s\n", A, 19 );
        break; case __LINE__: TRICE_N(ID(15999), "msg:%s\n", A, 120 );
        break; case __LINE__: TRICE_N(ID(16000), "msg:%s\n", A, 121 );
        break; case __LINE__: TRICE_N(ID(16001), "msg:%s\n", A, 122 );
        break; case __LINE__: TRICE_N(ID(16002), "msg:%s\n", A, 123 );
        break; case __LINE__: TRICE_N(ID(16003), "msg:%s\n", A, 124 );
        break; case __LINE__: TRICE_N(ID(16004), "msg:%s\n", A, 125 );
        break; case __LINE__: TRICE_N(ID(16005), "msg:%s\n", A, 126 );
        break; case __LINE__: TRICE_N(ID(16006), "msg:%s\n", A, 127 );
        break; case __LINE__: TRICE_N(ID(16007), "msg:%s\n", A, 128 );
        break; case __LINE__: TRICE_N(ID(16008), "msg:%s\n", A, 129 );


        break; case __LINE__: TRICE(ID(16009), "sig:TRICE with 1 to 12 values\n" );

        break; case __LINE__: TRICE(ID(16010), "rd:TRICE %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRICE -1     
        break; case __LINE__: TRICE(ID(16011), "rd:TRICE %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRICE -1, -2 
        break; case __LINE__: TRICE(ID(16012), "rd:TRICE %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRICE -1, -2, -3       
        break; case __LINE__: TRICE(ID(16013), "rd:TRICE %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRICE -1, -2, -3, -4      
        break; case __LINE__: TRICE(ID(16014), "rd:TRICE %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRICE -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE(ID(16015), "rd:TRICE %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRICE -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE(ID(16016), "rd:TRICE %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRICE -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE(ID(16017), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE(ID(16018), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE(ID(16019), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE(ID(16020), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE(ID(16021), "rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRICE -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    

        break; case __LINE__: TRICE(ID(16022), "sig:TRICE_n with 1 to 12 values\n" );		
        break; case __LINE__: TRICE(ID(16023), "rd:TRICE_1  %d\n", -1 );                                                                                            //exp: time:feed3322default: rd:TRICE_1  -1     
        break; case __LINE__: TRICE(ID(16024), "rd:TRICE_2  %d, %d\n", -1, -2 );                                                                                    //exp: time:feed3322default: rd:TRICE_2  -1, -2 
        break; case __LINE__: TRICE(ID(16025), "rd:TRICE_3  %d, %d, %d\n", -1, -2, -3 );                                                                            //exp: time:feed3322default: rd:TRICE_3  -1, -2, -3       
        break; case __LINE__: TRICE(ID(16026), "rd:TRICE_4  %d, %d, %d, %d\n", -1, -2, -3, -4 );                                                                    //exp: time:feed3322default: rd:TRICE_4  -1, -2, -3, -4      
        break; case __LINE__: TRICE(ID(16027), "rd:TRICE_5  %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );                                                            //exp: time:feed3322default: rd:TRICE_5  -1, -2, -3, -4, -5        
        break; case __LINE__: TRICE(ID(16028), "rd:TRICE_6  %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );                                                    //exp: time:feed3322default: rd:TRICE_6  -1, -2, -3, -4, -5, -6        
        break; case __LINE__: TRICE(ID(16029), "rd:TRICE_7  %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);                                             //exp: time:feed3322default: rd:TRICE_7  -1, -2, -3, -4, -5, -6, -7      
        break; case __LINE__: TRICE(ID(16030), "rd:TRICE_8  %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );                                    //exp: time:feed3322default: rd:TRICE_8  -1, -2, -3, -4, -5, -6, -7, -8       
        break; case __LINE__: TRICE(ID(16031), "rd:TRICE_9  %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );                            //exp: time:feed3322default: rd:TRICE_9  -1, -2, -3, -4, -5, -6, -7, -8, -9   
        break; case __LINE__: TRICE(ID(16032), "rd:TRICE_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);                    //exp: time:feed3322default: rd:TRICE_10 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10    
        break; case __LINE__: TRICE(ID(16033), "rd:TRICE_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );          //exp: time:feed3322default: rd:TRICE_11 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11     
        break; case __LINE__: TRICE(ID(16034), "rd:TRICE_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 ); //exp: time:feed3322default: rd:TRICE_12 -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12    


        break; case __LINE__: TRICE  (ID(16035), "sig:TRICE16 with 1 to 12 pointer\n" );
        break; case __LINE__: TRICE16(ID(16036), "rd:TRICE16 %p\n", -1 );
        break; case __LINE__: TRICE16(ID(16037), "rd:TRICE16 %p, %p\n", -1, -2 );
        break; case __LINE__: TRICE16(ID(16038), "rd:TRICE16 %p, %p, %p\n", -1, -2, -3 );
        break; case __LINE__: TRICE16(ID(16039), "rd:TRICE16 %p, %p, %p, %p\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE16(ID(16040), "rd:TRICE16 %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE16(ID(16041), "rd:TRICE16 %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE16(ID(16042), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE16(ID(16043), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE16(ID(16044), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE16(ID(16045), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE16(ID(16046), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE16(ID(16047), "rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE  (ID(16048), "sig:TRICE16 with 1 to 12 hex\n" );
        break; case __LINE__: TRICE16(ID(16049), "rd:TRICE16 %X\n", -1 );
        break; case __LINE__: TRICE16(ID(16050), "rd:TRICE16 %X, %X\n", -1, -2 );
        break; case __LINE__: TRICE16(ID(16051), "rd:TRICE16 %X, %X, %X\n", -1, -2, -3 );
        break; case __LINE__: TRICE16(ID(16052), "rd:TRICE16 %X, %X, %X, %X\n", -1, -2, -3, -4 );
        break; case __LINE__: TRICE16(ID(16053), "rd:TRICE16 %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5 );
        break; case __LINE__: TRICE16(ID(16054), "rd:TRICE16 %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6 );
        break; case __LINE__: TRICE16(ID(16055), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7);
        break; case __LINE__: TRICE16(ID(16056), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8 );
        break; case __LINE__: TRICE16(ID(16057), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
        break; case __LINE__: TRICE16(ID(16058), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
        break; case __LINE__: TRICE16(ID(16059), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
        break; case __LINE__: TRICE16(ID(16060), "rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );

        break; case __LINE__: TRICE(ID(16061), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE(id(16062), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE(Id(16063), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE(ID(16064), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: trice(iD(16065), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: Trice(iD(16066), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRice(iD(16067), "w: Hello! 👋🙂 \a\n" ); // with sound!

        break; case __LINE__: TRICE_0(ID(16068), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE_0(id(16069), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE_0(Id(16070), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE_0(ID(16071), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: trice_0(iD(16072), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: Trice_0(iD(16073), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRice_0(iD(16074), "w: Hello! 👋🙂 \a\n" ); // with sound!

        break; case __LINE__: TRICE0(ID(16075), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE0(id(16076), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE0(Id(16077), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE0(ID(16078), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: trice0(iD(16079), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: Trice0(iD(16080), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRice0(iD(16081), "w: Hello! 👋🙂 \a\n" ); // with sound!

        break; case __LINE__: TRICE_0(ID(16082), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE_0(id(16083), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE_0(Id(16084), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE_0(ID(16085), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: trice_0(iD(16086), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: Trice_0(iD(16087), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRice_0(iD(16088), "w: Hello! 👋🙂 \a\n" ); // with sound!

        break; case __LINE__: TRICE8(ID(16089), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE8(id(16090), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE8(Id(16091), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE8(ID(16092), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: trice8(iD(16093), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: Trice8(iD(16094), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRice8(iD(16095), "w: Hello! 👋🙂 \a\n" ); // with sound!

        break; case __LINE__: TRICE8_0(ID(16096), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE8_0(id(16097), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE8_0(Id(16098), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE8_0(ID(16099), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: trice8_0(iD(16100), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: Trice8_0(iD(16101), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRice8_0(iD(16102), "w: Hello! 👋🙂 \a\n" ); // with sound!

        break; case __LINE__: TRICE16(ID(16103), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE16(id(16104), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE16(Id(16105), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE16(ID(16106), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: trice16(iD(16107), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: Trice16(iD(16108), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRice16(iD(16109), "w: Hello! 👋🙂 \a\n" ); // with sound!

        break; case __LINE__: TRICE16_0(ID(16110), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE16_0(id(16111), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE16_0(Id(16112), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE16_0(ID(16113), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: trice16_0(iD(16114), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: Trice16_0(iD(16115), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRice16_0(iD(16116), "w: Hello! 👋🙂 \a\n" ); // with sound!

        break; case __LINE__: TRICE32(ID(16117), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE32(id(16118), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE32(Id(16119), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE32(ID(16120), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: trice32(iD(16121), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: Trice32(iD(16122), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRice32(iD(16123), "w: Hello! 👋🙂 \a\n" ); // with sound!

        break; case __LINE__: TRICE32_0(ID(16124), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE32_0(id(16125), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE32_0(Id(16126), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE32_0(ID(16127), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: trice32_0(iD(16128), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: Trice32_0(iD(16129), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRice32_0(iD(16130), "w: Hello! 👋🙂 \a\n" ); // with sound!

        break; case __LINE__: TRICE64(ID(16131), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE64(id(16132), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE64(Id(16133), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE64(ID(16134), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: trice64(iD(16135), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: Trice64(iD(16136), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRice64(iD(16137), "w: Hello! 👋🙂 \a\n" ); // with sound!

        break; case __LINE__: TRICE64_0(ID(16138), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE64_0(id(16139), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE64_0(Id(16140), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRICE64_0(ID(16141), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: trice64_0(iD(16142), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: Trice64_0(iD(16143), "w: Hello! 👋🙂 \a\n" ); // with sound!
        break; case __LINE__: TRice64_0(iD(16144), "w: Hello! 👋🙂 \a\n" ); // with sound!

    #endif // #if TRICE_CGO == 1
    }
}

// clang-format on

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
		TRICE_S(ID(16145), "rd:sizeof(%8s)", n); \
		TRICE(ID(16146), " = %d\n", size);       \
	} while (0);

//! DCOPY is a helper macro for struct deserialization.
#define DCOPY(element)                           \
	do {                                         \
		char* n = #element;                      \
		int size = sizeof(dst->element);         \
		memcpy(&(dst->element), p, size);        \
		p += size;                               \
		TRICE_S(ID(16147), "rd:sizeof(%8s)", n); \
		TRICE(ID(16148), " = %d\n", size);       \
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
	TRICE(Id(16149), "inf: Tryout tx struct:");
	TRICE8_B(Id(16150), " %02x ", &tx, sizeof(tx));
	TRICE(Id(16151), "\n");

	TRICE(Id(16152), "inf: Tryout buffer:");
	TRICE8_B(Id(16153), " %02x ", dst, len); // lint !e670
	TRICE(Id(16154), "\n");

#if !TRICE_OFF
	char* src = dst; // "copy" - assume, data transferred now ("data transfer")
	len = deserializeTryout(&rx, src);
#endif
	TRICE(Id(16155), "inf: Tryout rx struct:");
	TRICE8_B(Id(16156), " %02x ", &rx, sizeof(rx));
	TRICE(Id(16157), "\n");

	TRICE(Id(16158), "inf:sizeOf(Trypout) = %d, buffer length = %d\n", sizeof(tx), len);
	TRICE8_F(Id(16159), "info:TryoutStructFunction", &tx, sizeof(tx));
	TRICE8_F(Id(16160), "info:TryoutBufferFunction", dst, len); // lint !e670
}

static void exampleOfManualJSONencoding(void) {
#if !TRICE_OFF
	typedef struct {
		int Apple, Birn;
		float Fish;
	} Ex_t;
	Ex_t Ex = {-1, 2, (float)2.781};
#endif
	Trice(iD(16161), "att:MyStructEvaluationFunction(json:ExA{Apple:%d, Birn:%u, Fisch:%f}\n", Ex.Apple, Ex.Birn, aFloat(Ex.Fish));
}

static void dynString(int n) {
	char* s = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.,0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.,0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.,0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.,0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.,";
	const size_t l = strlen(s);
	n = n < l ? n : l;
	// trice(iD(16162), "sig:%3d:", n ); - this gets overwritten in CGO_Test case, so we avoid it to keep testing simple.
	TRICE_N(id(16163), "wr:%s\n", s, n);
}

#endif // #if !TRICE_OFF
