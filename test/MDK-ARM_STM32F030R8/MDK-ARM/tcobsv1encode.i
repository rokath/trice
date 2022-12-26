# 1 "../../../src/tcobsv1Encode.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 365 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "../../../src/tcobsv1Encode.c" 2





# 1 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\stdint.h" 1 3
# 56 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\stdint.h" 3
typedef signed char int8_t;
typedef signed short int int16_t;
typedef signed int int32_t;
typedef signed long long int int64_t;


typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long int uint64_t;





typedef signed char int_least8_t;
typedef signed short int int_least16_t;
typedef signed int int_least32_t;
typedef signed long long int int_least64_t;


typedef unsigned char uint_least8_t;
typedef unsigned short int uint_least16_t;
typedef unsigned int uint_least32_t;
typedef unsigned long long int uint_least64_t;




typedef signed int int_fast8_t;
typedef signed int int_fast16_t;
typedef signed int int_fast32_t;
typedef signed long long int int_fast64_t;


typedef unsigned int uint_fast8_t;
typedef unsigned int uint_fast16_t;
typedef unsigned int uint_fast32_t;
typedef unsigned long long int uint_fast64_t;






typedef signed int intptr_t;
typedef unsigned int uintptr_t;



typedef signed long long intmax_t;
typedef unsigned long long uintmax_t;
# 7 "../../../src/tcobsv1Encode.c" 2
# 1 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\stddef.h" 1 3
# 38 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\stddef.h" 3
  typedef signed int ptrdiff_t;







    typedef unsigned int size_t;
# 64 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\stddef.h" 3
      typedef unsigned short wchar_t;
# 95 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\stddef.h" 3
  typedef long double max_align_t;
# 8 "../../../src/tcobsv1Encode.c" 2
# 1 "../../../src/tcobs.h" 1
# 23 "../../../src/tcobs.h"
int TCOBSEncode( void * restrict output, const void * restrict input, size_t length);
# 32 "../../../src/tcobs.h"
int TCOBSDecode( void * restrict output, size_t max, const void * restrict input, size_t length );
# 9 "../../../src/tcobsv1Encode.c" 2
# 1 "../../../src/tcobsv1Internal.h" 1
# 10 "../../../src/tcobsv1Encode.c" 2
# 67 "../../../src/tcobsv1Encode.c"
int TCOBSEncode( void * restrict output, const void * restrict input, size_t length){
    uint8_t* o = output;
    uint8_t* out = output;
    uint8_t const * i = input;
    uint8_t const * limit = (uint8_t*)input + length;
    uint8_t zeroCount = 0;
    uint8_t fullCount = 0;
    uint8_t reptCount = 0;
    uint8_t b_1 = 0;
    uint8_t b = 0;
    uint8_t offset = 0;
# 93 "../../../src/tcobsv1Encode.c"
    if( length >= 2 ){
        b = *i++;
        for(;;){
            b_1 = b;
            b = *i++;

            if( limit - i > 0 ){




                if( (b_1 | b) == 0){
                    zeroCount++;
                    if( zeroCount == 2 ){
                        zeroCount = 3;
                        { ; ; ; ; *o++ = (zeroCount << 5) | offset; offset = 0; zeroCount = 0; }
                        b = *i++;
                        if( limit - i == 0 ){
                            goto lastByte;
                        }

                    }
                    continue;
                }





                if( (b_1 & b) == 0xFF ){
                    fullCount++;
                    if( fullCount == 3 ){
                        fullCount = 4;
                        { ; ; ; ; *o++ = 0x80 | (fullCount << 5) | offset; offset = 0; fullCount = 0; }
                        b = *i++;
                        if( limit - i == 0 ){
                            goto lastByte;
                        }

                    }
                    continue;
                }






                if( b_1 == b ){
                                      ;
                                         ;
                    reptCount++;
                    if( reptCount == 4 ){
                        { *o++ = b; offset++; ; if( offset == 31 ){ *o++ = 0xA0 | 31; offset = 0; } }
                        { ; ; ; if( offset > 7 ){ *o++ = 0xA0 | offset; offset = 0; } *o++ = ((reptCount-1) << 3) | offset; offset = 0; reptCount = 0; }
                        b = *i++;
                        if( limit - i == 0 ){
                            goto lastByte;
                        }

                    }
                    continue;
                }




                if( zeroCount ) {



                    zeroCount++;
                    { ; ; ; ; *o++ = (zeroCount << 5) | offset; offset = 0; zeroCount = 0; }
                    continue;
                }
                if( fullCount ) {



                    fullCount++;
                    { ; ; ; ; *o++ = 0x80 | (fullCount << 5) | offset; offset = 0; fullCount = 0; }
                    continue;
                }
                if( reptCount ) {




                    if( reptCount == 1 ){
                        reptCount = 0;
                        { *o++ = b_1; offset++; ; if( offset == 31 ){ *o++ = 0xA0 | 31; offset = 0; } }
                        { *o++ = b_1; offset++; ; if( offset == 31 ){ *o++ = 0xA0 | 31; offset = 0; } }
                        continue;
                    }
                    *o++ = b_1;
                    offset++;
                    { ; ; ; if( offset > 7 ){ *o++ = 0xA0 | offset; offset = 0; } *o++ = ((reptCount-1) << 3) | offset; offset = 0; reptCount = 0; }
                    continue;
                }
# 200 "../../../src/tcobsv1Encode.c"
                if( b_1 == 0 ){

                    zeroCount++;
                    { ; ; ; ; *o++ = (zeroCount << 5) | offset; offset = 0; zeroCount = 0; }
                    continue;
                }
                if( b_1 == 0xFF ){

                    { *o++ = 0xFF; offset++; ; if( offset == 31 ){ *o++ = 0xA0 | 31; offset = 0; } };
                    continue;
                }



                { *o++ = b_1; offset++; ; if( offset == 31 ){ *o++ = 0xA0 | 31; offset = 0; } }
                continue;

            }else{

                if( (zeroCount | fullCount | reptCount) == 0){
                    if( b_1 == 0 && b == 0 ){
                        *o++ = 0x40 | offset;
                        return o - out;
                    }
                    if( b_1 == 0xFF && b == 0xFF ){
                        *o++ = 0xC0 | offset;
                        return o - out;
                    }
                    if( b_1 == 0 ){
                        zeroCount = 1;
                        { ; ; ; ; *o++ = (zeroCount << 5) | offset; offset = 0; zeroCount = 0; }
                        goto lastByte;
                    }


                    { *o++ = b_1; offset++; ; if( offset == 31 ){ *o++ = 0xA0 | 31; offset = 0; } }
                    goto lastByte;
                }




                if( zeroCount == 1 ) {



                    if( b != 0 ){
                        zeroCount++;
                        { ; ; ; ; *o++ = (zeroCount << 5) | offset; offset = 0; zeroCount = 0; }
                        goto lastByte;
                    }
                    if( b == 0 ){
                        *o++ = 0x60 | offset;
                        return o - out;
                    }

                }

                if( zeroCount == 2 ) {




                    zeroCount = 3;
                    { ; ; ; ; *o++ = (zeroCount << 5) | offset; offset = 0; zeroCount = 0; }
                    goto lastByte;
                }

                if( fullCount == 1 ) {



                    if( b == 0xFF ){
                                              ;
                        *o++ = 0xE0 | offset;
                        return o - out;
                    }
                    fullCount = 2;
                    { ; ; ; ; *o++ = 0x80 | (fullCount << 5) | offset; offset = 0; fullCount = 0; }
                    goto lastByte;
                }

                if( fullCount == 2 ) {



                    if( b == 0xFF ){
                                              ;
                        *o++ = 0x80 | offset;
                        return o - out;
                    }

                    fullCount++;
                    { ; ; ; ; *o++ = 0x80 | (fullCount << 5) | offset; offset = 0; fullCount = 0; }
                    goto lastByte;
                }

                if( fullCount == 3 ) {



                    if( b == 0xFF ){
                        { ; ; ; ; *o++ = 0x80 | (fullCount << 5) | offset; offset = 0; fullCount = 0; }
                                              ;
                        *o++ = 0xC0 | offset;
                        return o - out;
                    }

                    fullCount = 4;
                    { ; ; ; ; *o++ = 0x80 | (fullCount << 5) | offset; offset = 0; fullCount = 0; }
                    goto lastByte;
                }

                if( reptCount == 1 ) {




                    if( b_1 == b ){
                        { *o++ = b_1; offset++; ; if( offset == 31 ){ *o++ = 0xA0 | 31; offset = 0; } }
                                              ;
                        if( offset > 7 ){
                            *o++ = 0xA0 | offset;
                            offset = 0;
                        }
                        *o++ = 0x08 | offset;
                        return o - out;
                    }
                    { *o++ = b_1; offset++; ; if( offset == 31 ){ *o++ = 0xA0 | 31; offset = 0; } }
                    { *o++ = b_1; offset++; ; if( offset == 31 ){ *o++ = 0xA0 | 31; offset = 0; } }
                    goto lastByte;
                }
                if( reptCount == 2 ) {




                    if( b_1 == b ){
                        { *o++ = b_1; offset++; ; if( offset == 31 ){ *o++ = 0xA0 | 31; offset = 0; } }
                                              ;
                        if( offset > 7 ){
                            *o++ = 0xA0 | offset;
                            offset = 0;
                        }
                        *o++ = 0x10 | offset;
                        return o - out;
                    }
                    { *o++ = b_1; offset++; ; if( offset == 31 ){ *o++ = 0xA0 | 31; offset = 0; } }
                    { ; ; ; if( offset > 7 ){ *o++ = 0xA0 | offset; offset = 0; } *o++ = ((reptCount-1) << 3) | offset; offset = 0; reptCount = 0; }
                    goto lastByte;
                }
                if( reptCount == 3 ) {




                    { *o++ = b_1; offset++; ; if( offset == 31 ){ *o++ = 0xA0 | 31; offset = 0; } }
                    if( b_1 == b ){
                                              ;
                        if( offset > 7 ){
                            *o++ = 0xA0 | offset;
                            offset = 0;
                        }
                        *o++ = 0x18 | offset;
                        return o - out;
                    }

                    { ; ; ; if( offset > 7 ){ *o++ = 0xA0 | offset; offset = 0; } *o++ = ((reptCount-1) << 3) | offset; offset = 0; reptCount = 0; }
                    goto lastByte;
                }


            }
        }
    }
    if( length == 0 ){
        return 0;
    }
    if( length == 1 ){
        b = *i;
        goto lastByte;
    }

lastByte:
    if( b == 0 ){
                              ;
        *o++ = 0x20 | offset;
        return o - out;
    }else{
        *o++ = b;
        offset++;
                              ;
        *o++ = 0xA0 | offset;
        return o - out;
    }
}
