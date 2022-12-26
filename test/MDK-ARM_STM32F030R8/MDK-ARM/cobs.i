# 1 "../../../src/cobs.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 365 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "../../../src/cobs.c" 2



# 1 "../../../src/cobs.h" 1







# 1 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\stddef.h" 1 3
# 38 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\stddef.h" 3
  typedef signed int ptrdiff_t;







    typedef unsigned int size_t;
# 64 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\stddef.h" 3
      typedef unsigned short wchar_t;
# 95 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\stddef.h" 3
  typedef long double max_align_t;
# 9 "../../../src/cobs.h" 2
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
# 10 "../../../src/cobs.h" 2

size_t COBSEncode(void * restrict out, const void * restrict in, size_t length);
size_t COBSDecode(void * restrict out, const void * restrict in, size_t length);
# 5 "../../../src/cobs.c" 2







size_t COBSEncode( void * restrict out, const void * restrict in, size_t length) {
 uint8_t * buffer = out;
 uint8_t *encode = buffer;
 uint8_t *codep = encode++;
 uint8_t code = 1;

 for (const uint8_t *byte = (const uint8_t *)in; length--; ++byte) {
  if (*byte) {
   *encode++ = *byte, ++code;
  }
  if (!*byte || code == 0xff) {
   *codep = code, code = 1, codep = encode;
   if (!*byte || length) {
    ++encode;
   }
  }
 }
 *codep = code;
 return (size_t)(encode - buffer);
}
# 40 "../../../src/cobs.c"
size_t COBSDecode(void * restrict out, const void * restrict in, size_t length ) {
 uint8_t* data = out;
 const uint8_t * buffer = in;
 const uint8_t *byte = buffer;
 uint8_t *decode = (uint8_t *)data;

 for (uint8_t code = 0xff, block = 0; byte < buffer + length; --block) {
  if (block) {
   *decode++ = *byte++;
  } else {
   if (code != 0xff) {
    *decode++ = 0;
   }
   block = code = *byte++;
   if (!code) {
    break;
   }
  }
 }
 return (size_t)(decode - data);
}
