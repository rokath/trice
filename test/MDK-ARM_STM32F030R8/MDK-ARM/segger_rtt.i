# 1 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 365 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c" 2
# 77 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c"
# 1 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.h" 1
# 61 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.h"
# 1 "../../../third_party/segger.com/SEGGER_RTT/RTT/../Config/SEGGER_RTT_Conf.h" 1
# 62 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.h" 2
# 268 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.h"
# 1 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\stdlib.h" 1 3
# 64 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\stdlib.h" 3
    typedef unsigned int size_t;
# 84 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\stdlib.h" 3
    typedef unsigned short wchar_t;
# 96 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\stdlib.h" 3
typedef struct div_t { int quot, rem; } div_t;

typedef struct ldiv_t { long int quot, rem; } ldiv_t;


typedef struct lldiv_t { long long quot, rem; } lldiv_t;
# 139 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\stdlib.h" 3
extern __attribute__((__nothrow__)) int __aeabi_MB_CUR_MAX(void);
# 158 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\stdlib.h" 3
extern __attribute__((__nothrow__)) double atof(const char * ) __attribute__((__nonnull__(1)));





extern __attribute__((__nothrow__)) int atoi(const char * ) __attribute__((__nonnull__(1)));





extern __attribute__((__nothrow__)) long int atol(const char * ) __attribute__((__nonnull__(1)));






extern __attribute__((__nothrow__)) long long atoll(const char * ) __attribute__((__nonnull__(1)));







extern __attribute__((__nothrow__)) double strtod(const char * __restrict , char ** __restrict ) __attribute__((__nonnull__(1)));
# 206 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\stdlib.h" 3
extern __attribute__((__nothrow__)) float strtof(const char * __restrict , char ** __restrict ) __attribute__((__nonnull__(1)));
extern __attribute__((__nothrow__)) long double strtold(const char * __restrict , char ** __restrict ) __attribute__((__nonnull__(1)));




extern __attribute__((__nothrow__)) long int strtol(const char * __restrict ,
                        char ** __restrict , int ) __attribute__((__nonnull__(1)));
# 243 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\stdlib.h" 3
extern __attribute__((__nothrow__)) unsigned long int strtoul(const char * __restrict ,
                                       char ** __restrict , int ) __attribute__((__nonnull__(1)));
# 275 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\stdlib.h" 3
extern __attribute__((__nothrow__)) long long strtoll(const char * __restrict ,
                                  char ** __restrict , int )
                          __attribute__((__nonnull__(1)));






extern __attribute__((__nothrow__)) unsigned long long strtoull(const char * __restrict ,
                                            char ** __restrict , int )
                                   __attribute__((__nonnull__(1)));






extern __attribute__((__nothrow__)) int rand(void);
# 303 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\stdlib.h" 3
extern __attribute__((__nothrow__)) void srand(unsigned int );
# 313 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\stdlib.h" 3
struct _rand_state { int __x[57]; };
extern __attribute__((__nothrow__)) int _rand_r(struct _rand_state *);
extern __attribute__((__nothrow__)) void _srand_r(struct _rand_state *, unsigned int);
struct _ANSI_rand_state { int __x[1]; };
extern __attribute__((__nothrow__)) int _ANSI_rand_r(struct _ANSI_rand_state *);
extern __attribute__((__nothrow__)) void _ANSI_srand_r(struct _ANSI_rand_state *, unsigned int);





extern __attribute__((__nothrow__)) void *calloc(size_t , size_t );





extern __attribute__((__nothrow__)) void free(void * );







extern __attribute__((__nothrow__)) void *malloc(size_t );





extern __attribute__((__nothrow__)) void *realloc(void * , size_t );
# 361 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\stdlib.h" 3
extern __attribute__((__nothrow__)) int posix_memalign(void ** , size_t , size_t );
# 374 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\stdlib.h" 3
typedef int (*__heapprt)(void *, char const *, ...);
extern __attribute__((__nothrow__)) void __heapstats(int (* )(void * ,
                                           char const * , ...),
                        void * ) __attribute__((__nonnull__(1)));
# 390 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\stdlib.h" 3
extern __attribute__((__nothrow__)) int __heapvalid(int (* )(void * ,
                                           char const * , ...),
                       void * , int ) __attribute__((__nonnull__(1)));
# 411 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\stdlib.h" 3
extern __attribute__((__nothrow__)) __attribute__((__noreturn__)) void abort(void);
# 422 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\stdlib.h" 3
extern __attribute__((__nothrow__)) int atexit(void (* )(void)) __attribute__((__nonnull__(1)));
# 444 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\stdlib.h" 3
extern __attribute__((__nothrow__)) __attribute__((__noreturn__)) void exit(int );
# 460 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\stdlib.h" 3
extern __attribute__((__nothrow__)) __attribute__((__noreturn__)) void _Exit(int );
# 471 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\stdlib.h" 3
extern __attribute__((__nothrow__)) char *getenv(const char * ) __attribute__((__nonnull__(1)));
# 484 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\stdlib.h" 3
extern __attribute__((__nothrow__)) int system(const char * );
# 497 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\stdlib.h" 3
extern void *bsearch(const void * , const void * ,
              size_t , size_t ,
              int (* )(const void *, const void *)) __attribute__((__nonnull__(1,2,5)));
# 532 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\stdlib.h" 3
extern void qsort(void * , size_t , size_t ,
           int (* )(const void *, const void *)) __attribute__((__nonnull__(1,4)));
# 560 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\stdlib.h" 3
extern __attribute__((__nothrow__)) __attribute__((__const__)) int abs(int );






extern __attribute__((__nothrow__)) __attribute__((__const__)) div_t div(int , int );
# 579 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\stdlib.h" 3
extern __attribute__((__nothrow__)) __attribute__((__const__)) long int labs(long int );
# 589 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\stdlib.h" 3
extern __attribute__((__nothrow__)) __attribute__((__const__)) ldiv_t ldiv(long int , long int );
# 610 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\stdlib.h" 3
extern __attribute__((__nothrow__)) __attribute__((__const__)) long long llabs(long long );
# 620 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\stdlib.h" 3
extern __attribute__((__nothrow__)) __attribute__((__const__)) lldiv_t lldiv(long long , long long );
# 644 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\stdlib.h" 3
typedef struct __sdiv32by16 { long quot, rem; } __sdiv32by16;
typedef struct __udiv32by16 { unsigned long quot, rem; } __udiv32by16;

typedef struct __sdiv64by32 { long rem, quot; } __sdiv64by32;

__attribute__((__value_in_regs__)) extern __attribute__((__nothrow__)) __attribute__((__const__)) __sdiv32by16 __rt_sdiv32by16(
     int ,
     short int );



__attribute__((__value_in_regs__)) extern __attribute__((__nothrow__)) __attribute__((__const__)) __udiv32by16 __rt_udiv32by16(
     unsigned int ,
     unsigned short );



__attribute__((__value_in_regs__)) extern __attribute__((__nothrow__)) __attribute__((__const__)) __sdiv64by32 __rt_sdiv64by32(
     int , unsigned int ,
     int );







extern __attribute__((__nothrow__)) unsigned int __fp_status(unsigned int , unsigned int );
# 705 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\stdlib.h" 3
extern __attribute__((__nothrow__)) int mblen(const char * , size_t );
# 720 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\stdlib.h" 3
extern __attribute__((__nothrow__)) int mbtowc(wchar_t * __restrict ,
                   const char * __restrict , size_t );
# 739 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\stdlib.h" 3
extern __attribute__((__nothrow__)) int wctomb(char * , wchar_t );
# 761 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\stdlib.h" 3
extern __attribute__((__nothrow__)) size_t mbstowcs(wchar_t * __restrict ,
                      const char * __restrict , size_t ) __attribute__((__nonnull__(2)));
# 779 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\stdlib.h" 3
extern __attribute__((__nothrow__)) size_t wcstombs(char * __restrict ,
                      const wchar_t * __restrict , size_t ) __attribute__((__nonnull__(2)));
# 798 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\stdlib.h" 3
extern __attribute__((__nothrow__)) void __use_realtime_heap(void);
extern __attribute__((__nothrow__)) void __use_realtime_division(void);
extern __attribute__((__nothrow__)) void __use_two_region_memory(void);
extern __attribute__((__nothrow__)) void __use_no_heap(void);
extern __attribute__((__nothrow__)) void __use_no_heap_region(void);

extern __attribute__((__nothrow__)) char const *__C_library_version_string(void);
extern __attribute__((__nothrow__)) int __C_library_version_number(void);
# 269 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.h" 2
# 1 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\stdarg.h" 1 3
# 40 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\stdarg.h" 3
  typedef __builtin_va_list va_list;
# 134 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\stdarg.h" 3
     typedef va_list __gnuc_va_list;
# 270 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.h" 2
# 304 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.h"
typedef struct {
  const char* sName;
            char* pBuffer;
            unsigned SizeOfBuffer;
            unsigned WrOff;
  volatile unsigned RdOff;
            unsigned Flags;
} SEGGER_RTT_BUFFER_UP;





typedef struct {
  const char* sName;
            char* pBuffer;
            unsigned SizeOfBuffer;
  volatile unsigned WrOff;
            unsigned RdOff;
            unsigned Flags;
} SEGGER_RTT_BUFFER_DOWN;






typedef struct {
  char acID[16];
  int MaxNumUpBuffers;
  int MaxNumDownBuffers;
  SEGGER_RTT_BUFFER_UP aUp[(3)];
  SEGGER_RTT_BUFFER_DOWN aDown[(3)];



} SEGGER_RTT_CB;







extern SEGGER_RTT_CB _SEGGER_RTT;
# 359 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.h"
int SEGGER_RTT_AllocDownBuffer (const char* sName, void* pBuffer, unsigned BufferSize, unsigned Flags);
int SEGGER_RTT_AllocUpBuffer (const char* sName, void* pBuffer, unsigned BufferSize, unsigned Flags);
int SEGGER_RTT_ConfigUpBuffer (unsigned BufferIndex, const char* sName, void* pBuffer, unsigned BufferSize, unsigned Flags);
int SEGGER_RTT_ConfigDownBuffer (unsigned BufferIndex, const char* sName, void* pBuffer, unsigned BufferSize, unsigned Flags);
int SEGGER_RTT_GetKey (void);
unsigned SEGGER_RTT_HasData (unsigned BufferIndex);
int SEGGER_RTT_HasKey (void);
unsigned SEGGER_RTT_HasDataUp (unsigned BufferIndex);
void SEGGER_RTT_Init (void);
unsigned SEGGER_RTT_Read (unsigned BufferIndex, void* pBuffer, unsigned BufferSize);
unsigned SEGGER_RTT_ReadNoLock (unsigned BufferIndex, void* pData, unsigned BufferSize);
int SEGGER_RTT_SetNameDownBuffer (unsigned BufferIndex, const char* sName);
int SEGGER_RTT_SetNameUpBuffer (unsigned BufferIndex, const char* sName);
int SEGGER_RTT_SetFlagsDownBuffer (unsigned BufferIndex, unsigned Flags);
int SEGGER_RTT_SetFlagsUpBuffer (unsigned BufferIndex, unsigned Flags);
int SEGGER_RTT_WaitKey (void);
unsigned SEGGER_RTT_Write (unsigned BufferIndex, const void* pBuffer, unsigned NumBytes);
unsigned SEGGER_RTT_WriteNoLock (unsigned BufferIndex, const void* pBuffer, unsigned NumBytes);
unsigned SEGGER_RTT_WriteSkipNoLock (unsigned BufferIndex, const void* pBuffer, unsigned NumBytes);
unsigned SEGGER_RTT_ASM_WriteSkipNoLock (unsigned BufferIndex, const void* pBuffer, unsigned NumBytes);
unsigned SEGGER_RTT_WriteString (unsigned BufferIndex, const char* s);
void SEGGER_RTT_WriteWithOverwriteNoLock(unsigned BufferIndex, const void* pBuffer, unsigned NumBytes);
unsigned SEGGER_RTT_PutChar (unsigned BufferIndex, char c);
unsigned SEGGER_RTT_PutCharSkip (unsigned BufferIndex, char c);
unsigned SEGGER_RTT_PutCharSkipNoLock (unsigned BufferIndex, char c);
unsigned SEGGER_RTT_GetAvailWriteSpace (unsigned BufferIndex);
unsigned SEGGER_RTT_GetBytesInBuffer (unsigned BufferIndex);
# 401 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.h"
unsigned SEGGER_RTT_ReadUpBuffer (unsigned BufferIndex, void* pBuffer, unsigned BufferSize);
unsigned SEGGER_RTT_ReadUpBufferNoLock (unsigned BufferIndex, void* pData, unsigned BufferSize);
unsigned SEGGER_RTT_WriteDownBuffer (unsigned BufferIndex, const void* pBuffer, unsigned NumBytes);
unsigned SEGGER_RTT_WriteDownBufferNoLock (unsigned BufferIndex, const void* pBuffer, unsigned NumBytes);
# 414 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.h"
int SEGGER_RTT_SetTerminal (unsigned char TerminalId);
int SEGGER_RTT_TerminalOut (unsigned char TerminalId, const char* s);







int SEGGER_RTT_printf(unsigned BufferIndex, const char * sFormat, ...);
int SEGGER_RTT_vprintf(unsigned BufferIndex, const char * sFormat, va_list * pParamList);
# 78 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c" 2

# 1 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\string.h" 1 3
# 44 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\string.h" 3
    typedef unsigned int size_t;
# 58 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\string.h" 3
extern __attribute__((__nothrow__)) void *memcpy(void * __restrict ,
                    const void * __restrict , size_t ) __attribute__((__nonnull__(1,2)));






extern __attribute__((__nothrow__)) void *memmove(void * ,
                    const void * , size_t ) __attribute__((__nonnull__(1,2)));
# 77 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\string.h" 3
extern __attribute__((__nothrow__)) char *strcpy(char * __restrict , const char * __restrict ) __attribute__((__nonnull__(1,2)));






extern __attribute__((__nothrow__)) char *strncpy(char * __restrict , const char * __restrict , size_t ) __attribute__((__nonnull__(1,2)));
# 93 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\string.h" 3
extern __attribute__((__nothrow__)) char *strcat(char * __restrict , const char * __restrict ) __attribute__((__nonnull__(1,2)));






extern __attribute__((__nothrow__)) char *strncat(char * __restrict , const char * __restrict , size_t ) __attribute__((__nonnull__(1,2)));
# 117 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\string.h" 3
extern __attribute__((__nothrow__)) int memcmp(const void * , const void * , size_t ) __attribute__((__nonnull__(1,2)));







extern __attribute__((__nothrow__)) int strcmp(const char * , const char * ) __attribute__((__nonnull__(1,2)));






extern __attribute__((__nothrow__)) int strncmp(const char * , const char * , size_t ) __attribute__((__nonnull__(1,2)));
# 141 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\string.h" 3
extern __attribute__((__nothrow__)) int strcasecmp(const char * , const char * ) __attribute__((__nonnull__(1,2)));







extern __attribute__((__nothrow__)) int strncasecmp(const char * , const char * , size_t ) __attribute__((__nonnull__(1,2)));
# 158 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\string.h" 3
extern __attribute__((__nothrow__)) int strcoll(const char * , const char * ) __attribute__((__nonnull__(1,2)));
# 169 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\string.h" 3
extern __attribute__((__nothrow__)) size_t strxfrm(char * __restrict , const char * __restrict , size_t ) __attribute__((__nonnull__(2)));
# 193 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\string.h" 3
extern __attribute__((__nothrow__)) void *memchr(const void * , int , size_t ) __attribute__((__nonnull__(1)));
# 209 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\string.h" 3
extern __attribute__((__nothrow__)) char *strchr(const char * , int ) __attribute__((__nonnull__(1)));
# 218 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\string.h" 3
extern __attribute__((__nothrow__)) size_t strcspn(const char * , const char * ) __attribute__((__nonnull__(1,2)));
# 232 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\string.h" 3
extern __attribute__((__nothrow__)) char *strpbrk(const char * , const char * ) __attribute__((__nonnull__(1,2)));
# 247 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\string.h" 3
extern __attribute__((__nothrow__)) char *strrchr(const char * , int ) __attribute__((__nonnull__(1)));
# 257 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\string.h" 3
extern __attribute__((__nothrow__)) size_t strspn(const char * , const char * ) __attribute__((__nonnull__(1,2)));
# 270 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\string.h" 3
extern __attribute__((__nothrow__)) char *strstr(const char * , const char * ) __attribute__((__nonnull__(1,2)));
# 280 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\string.h" 3
extern __attribute__((__nothrow__)) char *strtok(char * __restrict , const char * __restrict ) __attribute__((__nonnull__(2)));
extern __attribute__((__nothrow__)) char *_strtok_r(char * , const char * , char ** ) __attribute__((__nonnull__(2,3)));

extern __attribute__((__nothrow__)) char *strtok_r(char * , const char * , char ** ) __attribute__((__nonnull__(2,3)));
# 321 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\string.h" 3
extern __attribute__((__nothrow__)) void *memset(void * , int , size_t ) __attribute__((__nonnull__(1)));





extern __attribute__((__nothrow__)) char *strerror(int );







extern __attribute__((__nothrow__)) size_t strlen(const char * ) __attribute__((__nonnull__(1)));






extern __attribute__((__nothrow__)) size_t strlcpy(char * , const char * , size_t ) __attribute__((__nonnull__(1,2)));
# 362 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\string.h" 3
extern __attribute__((__nothrow__)) size_t strlcat(char * , const char * , size_t ) __attribute__((__nonnull__(1,2)));
# 388 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\string.h" 3
extern __attribute__((__nothrow__)) void _membitcpybl(void * , const void * , int , int , size_t ) __attribute__((__nonnull__(1,2)));
extern __attribute__((__nothrow__)) void _membitcpybb(void * , const void * , int , int , size_t ) __attribute__((__nonnull__(1,2)));
extern __attribute__((__nothrow__)) void _membitcpyhl(void * , const void * , int , int , size_t ) __attribute__((__nonnull__(1,2)));
extern __attribute__((__nothrow__)) void _membitcpyhb(void * , const void * , int , int , size_t ) __attribute__((__nonnull__(1,2)));
extern __attribute__((__nothrow__)) void _membitcpywl(void * , const void * , int , int , size_t ) __attribute__((__nonnull__(1,2)));
extern __attribute__((__nothrow__)) void _membitcpywb(void * , const void * , int , int , size_t ) __attribute__((__nonnull__(1,2)));
extern __attribute__((__nothrow__)) void _membitmovebl(void * , const void * , int , int , size_t ) __attribute__((__nonnull__(1,2)));
extern __attribute__((__nothrow__)) void _membitmovebb(void * , const void * , int , int , size_t ) __attribute__((__nonnull__(1,2)));
extern __attribute__((__nothrow__)) void _membitmovehl(void * , const void * , int , int , size_t ) __attribute__((__nonnull__(1,2)));
extern __attribute__((__nothrow__)) void _membitmovehb(void * , const void * , int , int , size_t ) __attribute__((__nonnull__(1,2)));
extern __attribute__((__nothrow__)) void _membitmovewl(void * , const void * , int , int , size_t ) __attribute__((__nonnull__(1,2)));
extern __attribute__((__nothrow__)) void _membitmovewb(void * , const void * , int , int , size_t ) __attribute__((__nonnull__(1,2)));
# 80 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c" 2
# 258 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c"
static unsigned char _aTerminalId[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
# 279 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c"
  SEGGER_RTT_CB _SEGGER_RTT;
  static char _acUpBuffer [(3*1024)];
  static char _acDownBuffer[(16)];


static unsigned char _ActiveTerminal;
# 312 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c"
static void _DoInit(void) {
  volatile SEGGER_RTT_CB* p;
  static const char _aInitStr[] = "\0\0\0\0\0\0TTR REGGES";
  unsigned i;



  p = (volatile SEGGER_RTT_CB*)((char*)&_SEGGER_RTT + (0));
  memset((SEGGER_RTT_CB*)p, 0, sizeof(_SEGGER_RTT));
  p->MaxNumUpBuffers = (3);
  p->MaxNumDownBuffers = (3);



  p->aUp[0].sName = "Terminal";
  p->aUp[0].pBuffer = _acUpBuffer;
  p->aUp[0].SizeOfBuffer = (3*1024);
  p->aUp[0].RdOff = 0u;
  p->aUp[0].WrOff = 0u;
  p->aUp[0].Flags = (0);



  p->aDown[0].sName = "Terminal";
  p->aDown[0].pBuffer = _acDownBuffer;
  p->aDown[0].SizeOfBuffer = (16);
  p->aDown[0].RdOff = 0u;
  p->aDown[0].WrOff = 0u;
  p->aDown[0].Flags = (0);





            ;
  for (i = 0; i < sizeof(_aInitStr) - 1; ++i) {
    p->acID[i] = _aInitStr[sizeof(_aInitStr) - 2 - i];
  }
            ;
}
# 372 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c"
static unsigned _WriteBlocking(SEGGER_RTT_BUFFER_UP* pRing, const char* pBuffer, unsigned NumBytes) {
  unsigned NumBytesToWrite;
  unsigned NumBytesWritten;
  unsigned RdOff;
  unsigned WrOff;
  volatile char* pDst;



  NumBytesWritten = 0u;
  WrOff = pRing->WrOff;
  do {
    RdOff = pRing->RdOff;
    if (RdOff > WrOff) {
      NumBytesToWrite = RdOff - WrOff - 1u;
    } else {
      NumBytesToWrite = pRing->SizeOfBuffer - (WrOff - RdOff + 1u);
    }
    NumBytesToWrite = (((NumBytesToWrite) < ((pRing->SizeOfBuffer - WrOff))) ? (NumBytesToWrite) : ((pRing->SizeOfBuffer - WrOff)));
    NumBytesToWrite = (((NumBytesToWrite) < (NumBytes)) ? (NumBytesToWrite) : (NumBytes));
    pDst = (pRing->pBuffer + WrOff) + (0);
# 401 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c"
    memcpy(((void*)pDst), (pBuffer), (NumBytesToWrite));
    NumBytesWritten += NumBytesToWrite;
    pBuffer += NumBytesToWrite;
    NumBytes -= NumBytesToWrite;
    WrOff += NumBytesToWrite;

    if (WrOff == pRing->SizeOfBuffer) {
      WrOff = 0u;
    }
              ;
    pRing->WrOff = WrOff;
  } while (NumBytes);
  return NumBytesWritten;
}
# 434 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c"
static void _WriteNoCheck(SEGGER_RTT_BUFFER_UP* pRing, const char* pData, unsigned NumBytes) {
  unsigned NumBytesAtOnce;
  unsigned WrOff;
  unsigned Rem;
  volatile char* pDst;

  WrOff = pRing->WrOff;
  Rem = pRing->SizeOfBuffer - WrOff;
  if (Rem > NumBytes) {



    pDst = (pRing->pBuffer + WrOff) + (0);
# 455 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c"
    memcpy(((void*)pDst), (pData), (NumBytes));
              ;
    pRing->WrOff = WrOff + NumBytes;

  } else {
# 477 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c"
    NumBytesAtOnce = Rem;
    pDst = (pRing->pBuffer + WrOff) + (0);
    memcpy(((void*)pDst), (pData), (NumBytesAtOnce));
    NumBytesAtOnce = NumBytes - Rem;
    pDst = pRing->pBuffer + (0);
    memcpy(((void*)pDst), (pData + Rem), (NumBytesAtOnce));
              ;
    pRing->WrOff = NumBytesAtOnce;

  }
}
# 502 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c"
static void _PostTerminalSwitch(SEGGER_RTT_BUFFER_UP* pRing, unsigned char TerminalId) {
  unsigned char ac[2];

  ac[0] = 0xFFu;
  ac[1] = _aTerminalId[TerminalId];
  _WriteBlocking(pRing, (const char*)ac, 2u);
}
# 524 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c"
static unsigned _GetAvailWriteSpace(SEGGER_RTT_BUFFER_UP* pRing) {
  unsigned RdOff;
  unsigned WrOff;
  unsigned r;




  RdOff = pRing->RdOff;
  WrOff = pRing->WrOff;
  if (RdOff <= WrOff) {
    r = pRing->SizeOfBuffer - 1u - WrOff + RdOff;
  } else {
    r = RdOff - WrOff - 1u;
  }
  return r;
}
# 571 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c"
unsigned SEGGER_RTT_ReadUpBufferNoLock(unsigned BufferIndex, void* pData, unsigned BufferSize) {
  unsigned NumBytesRem;
  unsigned NumBytesRead;
  unsigned RdOff;
  unsigned WrOff;
  unsigned char* pBuffer;
  SEGGER_RTT_BUFFER_UP* pRing;
  volatile char* pSrc;

  { volatile SEGGER_RTT_CB* pRTTCBInit; pRTTCBInit = (volatile SEGGER_RTT_CB*)((char*)&_SEGGER_RTT + (0)); do { if (pRTTCBInit->acID[0] == '\0') { _DoInit(); } } while (0); };
  pRing = (SEGGER_RTT_BUFFER_UP*)((char*)&_SEGGER_RTT.aUp[BufferIndex] + (0));
  pBuffer = (unsigned char*)pData;
  RdOff = pRing->RdOff;
  WrOff = pRing->WrOff;
  NumBytesRead = 0u;



  if (RdOff > WrOff) {
    NumBytesRem = pRing->SizeOfBuffer - RdOff;
    NumBytesRem = (((NumBytesRem) < (BufferSize)) ? (NumBytesRem) : (BufferSize));
    pSrc = (pRing->pBuffer + RdOff) + (0);
# 601 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c"
    memcpy((pBuffer), ((void*)pSrc), (NumBytesRem));
    NumBytesRead += NumBytesRem;
    pBuffer += NumBytesRem;
    BufferSize -= NumBytesRem;
    RdOff += NumBytesRem;




    if (RdOff == pRing->SizeOfBuffer) {
      RdOff = 0u;
    }
  }



  NumBytesRem = WrOff - RdOff;
  NumBytesRem = (((NumBytesRem) < (BufferSize)) ? (NumBytesRem) : (BufferSize));
  if (NumBytesRem > 0u) {
    pSrc = (pRing->pBuffer + RdOff) + (0);
# 629 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c"
    memcpy((pBuffer), ((void*)pSrc), (NumBytesRem));
    NumBytesRead += NumBytesRem;
    pBuffer += NumBytesRem;
    BufferSize -= NumBytesRem;
    RdOff += NumBytesRem;

  }



  if (NumBytesRead) {
    pRing->RdOff = RdOff;
  }

  return NumBytesRead;
}
# 663 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c"
unsigned SEGGER_RTT_ReadNoLock(unsigned BufferIndex, void* pData, unsigned BufferSize) {
  unsigned NumBytesRem;
  unsigned NumBytesRead;
  unsigned RdOff;
  unsigned WrOff;
  unsigned char* pBuffer;
  SEGGER_RTT_BUFFER_DOWN* pRing;
  volatile char* pSrc;

  { volatile SEGGER_RTT_CB* pRTTCBInit; pRTTCBInit = (volatile SEGGER_RTT_CB*)((char*)&_SEGGER_RTT + (0)); do { if (pRTTCBInit->acID[0] == '\0') { _DoInit(); } } while (0); };
  pRing = (SEGGER_RTT_BUFFER_DOWN*)((char*)&_SEGGER_RTT.aDown[BufferIndex] + (0));
  pBuffer = (unsigned char*)pData;
  RdOff = pRing->RdOff;
  WrOff = pRing->WrOff;
  NumBytesRead = 0u;



  if (RdOff > WrOff) {
    NumBytesRem = pRing->SizeOfBuffer - RdOff;
    NumBytesRem = (((NumBytesRem) < (BufferSize)) ? (NumBytesRem) : (BufferSize));
    pSrc = (pRing->pBuffer + RdOff) + (0);
# 693 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c"
    memcpy((pBuffer), ((void*)pSrc), (NumBytesRem));
    NumBytesRead += NumBytesRem;
    pBuffer += NumBytesRem;
    BufferSize -= NumBytesRem;
    RdOff += NumBytesRem;




    if (RdOff == pRing->SizeOfBuffer) {
      RdOff = 0u;
    }
  }



  NumBytesRem = WrOff - RdOff;
  NumBytesRem = (((NumBytesRem) < (BufferSize)) ? (NumBytesRem) : (BufferSize));
  if (NumBytesRem > 0u) {
    pSrc = (pRing->pBuffer + RdOff) + (0);
# 721 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c"
    memcpy((pBuffer), ((void*)pSrc), (NumBytesRem));
    NumBytesRead += NumBytesRem;
    pBuffer += NumBytesRem;
    BufferSize -= NumBytesRem;
    RdOff += NumBytesRem;

  }
  if (NumBytesRead) {
    pRing->RdOff = RdOff;
  }

  return NumBytesRead;
}
# 760 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c"
unsigned SEGGER_RTT_ReadUpBuffer(unsigned BufferIndex, void* pBuffer, unsigned BufferSize) {
  unsigned NumBytesRead;

  { unsigned int _SEGGER_RTT__LockState; __asm volatile ("mrs   %0, primask  \n\t" "movs  r1, #1       \n\t" "msr   primask, r1  \n\t" : "=r" (_SEGGER_RTT__LockState) : : "r1", "cc" );;



  NumBytesRead = SEGGER_RTT_ReadUpBufferNoLock(BufferIndex, pBuffer, BufferSize);



  __asm volatile ("msr   primask, %0  \n\t" : : "r" (_SEGGER_RTT__LockState) : ); };

  return NumBytesRead;
}
# 792 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c"
unsigned SEGGER_RTT_Read(unsigned BufferIndex, void* pBuffer, unsigned BufferSize) {
  unsigned NumBytesRead;

  { unsigned int _SEGGER_RTT__LockState; __asm volatile ("mrs   %0, primask  \n\t" "movs  r1, #1       \n\t" "msr   primask, r1  \n\t" : "=r" (_SEGGER_RTT__LockState) : : "r1", "cc" );;



  NumBytesRead = SEGGER_RTT_ReadNoLock(BufferIndex, pBuffer, BufferSize);



  __asm volatile ("msr   primask, %0  \n\t" : : "r" (_SEGGER_RTT__LockState) : ); };

  return NumBytesRead;
}
# 831 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c"
void SEGGER_RTT_WriteWithOverwriteNoLock(unsigned BufferIndex, const void* pBuffer, unsigned NumBytes) {
  const char* pData;
  SEGGER_RTT_BUFFER_UP* pRing;
  unsigned Avail;
  volatile char* pDst;



  pData = (const char *)pBuffer;
  pRing = (SEGGER_RTT_BUFFER_UP*)((char*)&_SEGGER_RTT.aUp[BufferIndex] + (0));



  if (pRing->WrOff == pRing->RdOff) {
    Avail = pRing->SizeOfBuffer - 1u;
  } else if ( pRing->WrOff < pRing->RdOff) {
    Avail = pRing->RdOff - pRing->WrOff - 1u;
  } else {
    Avail = pRing->RdOff - pRing->WrOff - 1u + pRing->SizeOfBuffer;
  }
  if (NumBytes > Avail) {
    pRing->RdOff += (NumBytes - Avail);
    while (pRing->RdOff >= pRing->SizeOfBuffer) {
      pRing->RdOff -= pRing->SizeOfBuffer;
    }
  }



  Avail = pRing->SizeOfBuffer - pRing->WrOff;
  do {
    if (Avail > NumBytes) {



      pDst = (pRing->pBuffer + pRing->WrOff) + (0);
# 875 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c"
      memcpy(((void*)pDst), (pData), (NumBytes));
                ;
      pRing->WrOff += NumBytes;

      break;
    } else {



      pDst = (pRing->pBuffer + pRing->WrOff) + (0);
# 893 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c"
      memcpy(((void*)pDst), (pData), (Avail));
      pData += Avail;
                ;
      pRing->WrOff = 0;
      NumBytes -= Avail;

      Avail = (pRing->SizeOfBuffer - 1);
    }
  } while (NumBytes);
}
# 932 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c"
unsigned SEGGER_RTT_WriteSkipNoLock(unsigned BufferIndex, const void* pBuffer, unsigned NumBytes) {
  const char* pData;
  SEGGER_RTT_BUFFER_UP* pRing;
  unsigned Avail;
  unsigned RdOff;
  unsigned WrOff;
  unsigned Rem;
  volatile char* pDst;
# 950 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c"
  pData = (const char *)pBuffer;
  pRing = (SEGGER_RTT_BUFFER_UP*)((char*)&_SEGGER_RTT.aUp[BufferIndex] + (0));
  RdOff = pRing->RdOff;
  WrOff = pRing->WrOff;
  if (RdOff <= WrOff) {
    Avail = pRing->SizeOfBuffer - WrOff - 1u;
    if (Avail >= NumBytes) {
CopyStraight:
      pDst = (pRing->pBuffer + WrOff) + (0);
      memcpy((void*)pDst, pData, NumBytes);
                ;
      pRing->WrOff = WrOff + NumBytes;
      return 1;
    }
    Avail += RdOff;
    if (Avail >= NumBytes) {
      Rem = pRing->SizeOfBuffer - WrOff;
      pDst = (pRing->pBuffer + WrOff) + (0);
      memcpy((void*)pDst, pData, Rem);
      NumBytes -= Rem;






      if (NumBytes) {
        pDst = pRing->pBuffer + (0);
        memcpy((void*)pDst, pData + Rem, NumBytes);
      }
                ;
      pRing->WrOff = NumBytes;
      return 1;
    }
  } else {
    Avail = RdOff - WrOff - 1u;
    if (Avail >= NumBytes) {
      goto CopyStraight;
    }
  }
  return 0;
}
# 1022 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c"
unsigned SEGGER_RTT_WriteDownBufferNoLock(unsigned BufferIndex, const void* pBuffer, unsigned NumBytes) {
  unsigned Status;
  unsigned Avail;
  const char* pData;
  SEGGER_RTT_BUFFER_UP* pRing;




  pData = (const char *)pBuffer;
  pRing = (SEGGER_RTT_BUFFER_UP*)((char*)&_SEGGER_RTT.aDown[BufferIndex] + (0));



  switch (pRing->Flags) {
  case (0):




    Avail = _GetAvailWriteSpace(pRing);
    if (Avail < NumBytes) {
      Status = 0u;
    } else {
      Status = NumBytes;
      _WriteNoCheck(pRing, pData, NumBytes);
    }
    break;
  case (1):



    Avail = _GetAvailWriteSpace(pRing);
    Status = Avail < NumBytes ? Avail : NumBytes;
    _WriteNoCheck(pRing, pData, Status);
    break;
  case (2):



    Status = _WriteBlocking(pRing, pData, NumBytes);
    break;
  default:
    Status = 0u;
    break;
  }



  return Status;
}
# 1097 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c"
unsigned SEGGER_RTT_WriteNoLock(unsigned BufferIndex, const void* pBuffer, unsigned NumBytes) {
  unsigned Status;
  unsigned Avail;
  const char* pData;
  SEGGER_RTT_BUFFER_UP* pRing;



  pData = (const char *)pBuffer;
  pRing = (SEGGER_RTT_BUFFER_UP*)((char*)&_SEGGER_RTT.aUp[BufferIndex] + (0));



  switch (pRing->Flags) {
  case (0):




    Avail = _GetAvailWriteSpace(pRing);
    if (Avail < NumBytes) {
      Status = 0u;
    } else {
      Status = NumBytes;
      _WriteNoCheck(pRing, pData, NumBytes);
    }
    break;
  case (1):



    Avail = _GetAvailWriteSpace(pRing);
    Status = Avail < NumBytes ? Avail : NumBytes;
    _WriteNoCheck(pRing, pData, Status);
    break;
  case (2):



    Status = _WriteBlocking(pRing, pData, NumBytes);
    break;
  default:
    Status = 0u;
    break;
  }



  return Status;
}
# 1173 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c"
unsigned SEGGER_RTT_WriteDownBuffer(unsigned BufferIndex, const void* pBuffer, unsigned NumBytes) {
  unsigned Status;

  { volatile SEGGER_RTT_CB* pRTTCBInit; pRTTCBInit = (volatile SEGGER_RTT_CB*)((char*)&_SEGGER_RTT + (0)); do { if (pRTTCBInit->acID[0] == '\0') { _DoInit(); } } while (0); };
  { unsigned int _SEGGER_RTT__LockState; __asm volatile ("mrs   %0, primask  \n\t" "movs  r1, #1       \n\t" "msr   primask, r1  \n\t" : "=r" (_SEGGER_RTT__LockState) : : "r1", "cc" );;
  Status = SEGGER_RTT_WriteDownBufferNoLock(BufferIndex, pBuffer, NumBytes);
  __asm volatile ("msr   primask, %0  \n\t" : : "r" (_SEGGER_RTT__LockState) : ); };
  return Status;
}
# 1202 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c"
unsigned SEGGER_RTT_Write(unsigned BufferIndex, const void* pBuffer, unsigned NumBytes) {
  unsigned Status;

  { volatile SEGGER_RTT_CB* pRTTCBInit; pRTTCBInit = (volatile SEGGER_RTT_CB*)((char*)&_SEGGER_RTT + (0)); do { if (pRTTCBInit->acID[0] == '\0') { _DoInit(); } } while (0); };
  { unsigned int _SEGGER_RTT__LockState; __asm volatile ("mrs   %0, primask  \n\t" "movs  r1, #1       \n\t" "msr   primask, r1  \n\t" : "=r" (_SEGGER_RTT__LockState) : : "r1", "cc" );;
  Status = SEGGER_RTT_WriteNoLock(BufferIndex, pBuffer, NumBytes);
  __asm volatile ("msr   primask, %0  \n\t" : : "r" (_SEGGER_RTT__LockState) : ); };
  return Status;
}
# 1232 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c"
unsigned SEGGER_RTT_WriteString(unsigned BufferIndex, const char* s) {
  unsigned Len;

  Len = strlen((s));
  return SEGGER_RTT_Write(BufferIndex, s, Len);
}
# 1262 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c"
unsigned SEGGER_RTT_PutCharSkipNoLock(unsigned BufferIndex, char c) {
  SEGGER_RTT_BUFFER_UP* pRing;
  unsigned WrOff;
  unsigned Status;
  volatile char* pDst;



  pRing = (SEGGER_RTT_BUFFER_UP*)((char*)&_SEGGER_RTT.aUp[BufferIndex] + (0));



  WrOff = pRing->WrOff + 1;
  if (WrOff == pRing->SizeOfBuffer) {
    WrOff = 0;
  }



  if (WrOff != pRing->RdOff) {
    pDst = (pRing->pBuffer + pRing->WrOff) + (0);
    *pDst = c;
              ;
    pRing->WrOff = WrOff;
    Status = 1;
  } else {
    Status = 0;
  }

  return Status;
}
# 1312 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c"
unsigned SEGGER_RTT_PutCharSkip(unsigned BufferIndex, char c) {
  SEGGER_RTT_BUFFER_UP* pRing;
  unsigned WrOff;
  unsigned Status;
  volatile char* pDst;



  { volatile SEGGER_RTT_CB* pRTTCBInit; pRTTCBInit = (volatile SEGGER_RTT_CB*)((char*)&_SEGGER_RTT + (0)); do { if (pRTTCBInit->acID[0] == '\0') { _DoInit(); } } while (0); };
  { unsigned int _SEGGER_RTT__LockState; __asm volatile ("mrs   %0, primask  \n\t" "movs  r1, #1       \n\t" "msr   primask, r1  \n\t" : "=r" (_SEGGER_RTT__LockState) : : "r1", "cc" );;



  pRing = (SEGGER_RTT_BUFFER_UP*)((char*)&_SEGGER_RTT.aUp[BufferIndex] + (0));



  WrOff = pRing->WrOff + 1;
  if (WrOff == pRing->SizeOfBuffer) {
    WrOff = 0;
  }



  if (WrOff != pRing->RdOff) {
    pDst = (pRing->pBuffer + pRing->WrOff) + (0);
    *pDst = c;
              ;
    pRing->WrOff = WrOff;
    Status = 1;
  } else {
    Status = 0;
  }



  __asm volatile ("msr   primask, %0  \n\t" : : "r" (_SEGGER_RTT__LockState) : ); };

  return Status;
}
# 1371 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c"
unsigned SEGGER_RTT_PutChar(unsigned BufferIndex, char c) {
  SEGGER_RTT_BUFFER_UP* pRing;
  unsigned WrOff;
  unsigned Status;
  volatile char* pDst;



  { volatile SEGGER_RTT_CB* pRTTCBInit; pRTTCBInit = (volatile SEGGER_RTT_CB*)((char*)&_SEGGER_RTT + (0)); do { if (pRTTCBInit->acID[0] == '\0') { _DoInit(); } } while (0); };
  { unsigned int _SEGGER_RTT__LockState; __asm volatile ("mrs   %0, primask  \n\t" "movs  r1, #1       \n\t" "msr   primask, r1  \n\t" : "=r" (_SEGGER_RTT__LockState) : : "r1", "cc" );;



  pRing = (SEGGER_RTT_BUFFER_UP*)((char*)&_SEGGER_RTT.aUp[BufferIndex] + (0));



  WrOff = pRing->WrOff + 1;
  if (WrOff == pRing->SizeOfBuffer) {
    WrOff = 0;
  }



  if (pRing->Flags == (2)) {
    while (WrOff == pRing->RdOff) {
      ;
    }
  }



  if (WrOff != pRing->RdOff) {
    pDst = (pRing->pBuffer + pRing->WrOff) + (0);
    *pDst = c;
              ;
    pRing->WrOff = WrOff;
    Status = 1;
  } else {
    Status = 0;
  }



  __asm volatile ("msr   primask, %0  \n\t" : : "r" (_SEGGER_RTT__LockState) : ); };
  return Status;
}
# 1434 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c"
int SEGGER_RTT_GetKey(void) {
  char c;
  int r;

  r = (int)SEGGER_RTT_Read(0u, &c, 1u);
  if (r == 1) {
    r = (int)(unsigned char)c;
  } else {
    r = -1;
  }
  return r;
}
# 1462 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c"
int SEGGER_RTT_WaitKey(void) {
  int r;

  do {
    r = SEGGER_RTT_GetKey();
  } while (r < 0);
  return r;
}
# 1485 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c"
int SEGGER_RTT_HasKey(void) {
  SEGGER_RTT_BUFFER_DOWN* pRing;
  unsigned RdOff;
  int r;

  { volatile SEGGER_RTT_CB* pRTTCBInit; pRTTCBInit = (volatile SEGGER_RTT_CB*)((char*)&_SEGGER_RTT + (0)); do { if (pRTTCBInit->acID[0] == '\0') { _DoInit(); } } while (0); };
  pRing = (SEGGER_RTT_BUFFER_DOWN*)((char*)&_SEGGER_RTT.aDown[0] + (0));
  RdOff = pRing->RdOff;
  if (RdOff != pRing->WrOff) {
    r = 1;
  } else {
    r = 0;
  }
  return r;
}
# 1513 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c"
unsigned SEGGER_RTT_HasData(unsigned BufferIndex) {
  SEGGER_RTT_BUFFER_DOWN* pRing;
  unsigned v;

  pRing = (SEGGER_RTT_BUFFER_DOWN*)((char*)&_SEGGER_RTT.aDown[BufferIndex] + (0));
  v = pRing->WrOff;
  return v - pRing->RdOff;
}
# 1534 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c"
unsigned SEGGER_RTT_HasDataUp(unsigned BufferIndex) {
  SEGGER_RTT_BUFFER_UP* pRing;
  unsigned v;

  pRing = (SEGGER_RTT_BUFFER_UP*)((char*)&_SEGGER_RTT.aUp[BufferIndex] + (0));
  v = pRing->RdOff;
  return pRing->WrOff - v;
}
# 1562 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c"
int SEGGER_RTT_AllocDownBuffer(const char* sName, void* pBuffer, unsigned BufferSize, unsigned Flags) {
  int BufferIndex;
  volatile SEGGER_RTT_CB* pRTTCB;

  { volatile SEGGER_RTT_CB* pRTTCBInit; pRTTCBInit = (volatile SEGGER_RTT_CB*)((char*)&_SEGGER_RTT + (0)); do { if (pRTTCBInit->acID[0] == '\0') { _DoInit(); } } while (0); };
  { unsigned int _SEGGER_RTT__LockState; __asm volatile ("mrs   %0, primask  \n\t" "movs  r1, #1       \n\t" "msr   primask, r1  \n\t" : "=r" (_SEGGER_RTT__LockState) : : "r1", "cc" );;
  pRTTCB = (volatile SEGGER_RTT_CB*)((unsigned char*)&_SEGGER_RTT + (0));
  BufferIndex = 0;
  do {
    if (pRTTCB->aDown[BufferIndex].pBuffer == 0) {
      break;
    }
    BufferIndex++;
  } while (BufferIndex < pRTTCB->MaxNumDownBuffers);
  if (BufferIndex < pRTTCB->MaxNumDownBuffers) {
    pRTTCB->aDown[BufferIndex].sName = sName;
    pRTTCB->aDown[BufferIndex].pBuffer = (char*)pBuffer;
    pRTTCB->aDown[BufferIndex].SizeOfBuffer = BufferSize;
    pRTTCB->aDown[BufferIndex].RdOff = 0u;
    pRTTCB->aDown[BufferIndex].WrOff = 0u;
    pRTTCB->aDown[BufferIndex].Flags = Flags;
              ;
  } else {
    BufferIndex = -1;
  }
  __asm volatile ("msr   primask, %0  \n\t" : : "r" (_SEGGER_RTT__LockState) : ); };
  return BufferIndex;
}
# 1610 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c"
int SEGGER_RTT_AllocUpBuffer(const char* sName, void* pBuffer, unsigned BufferSize, unsigned Flags) {
  int BufferIndex;
  volatile SEGGER_RTT_CB* pRTTCB;

  { volatile SEGGER_RTT_CB* pRTTCBInit; pRTTCBInit = (volatile SEGGER_RTT_CB*)((char*)&_SEGGER_RTT + (0)); do { if (pRTTCBInit->acID[0] == '\0') { _DoInit(); } } while (0); };
  { unsigned int _SEGGER_RTT__LockState; __asm volatile ("mrs   %0, primask  \n\t" "movs  r1, #1       \n\t" "msr   primask, r1  \n\t" : "=r" (_SEGGER_RTT__LockState) : : "r1", "cc" );;
  pRTTCB = (volatile SEGGER_RTT_CB*)((unsigned char*)&_SEGGER_RTT + (0));
  BufferIndex = 0;
  do {
    if (pRTTCB->aUp[BufferIndex].pBuffer == 0) {
      break;
    }
    BufferIndex++;
  } while (BufferIndex < pRTTCB->MaxNumUpBuffers);
  if (BufferIndex < pRTTCB->MaxNumUpBuffers) {
    pRTTCB->aUp[BufferIndex].sName = sName;
    pRTTCB->aUp[BufferIndex].pBuffer = (char*)pBuffer;
    pRTTCB->aUp[BufferIndex].SizeOfBuffer = BufferSize;
    pRTTCB->aUp[BufferIndex].RdOff = 0u;
    pRTTCB->aUp[BufferIndex].WrOff = 0u;
    pRTTCB->aUp[BufferIndex].Flags = Flags;
              ;
  } else {
    BufferIndex = -1;
  }
  __asm volatile ("msr   primask, %0  \n\t" : : "r" (_SEGGER_RTT__LockState) : ); };
  return BufferIndex;
}
# 1664 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c"
int SEGGER_RTT_ConfigUpBuffer(unsigned BufferIndex, const char* sName, void* pBuffer, unsigned BufferSize, unsigned Flags) {
  int r;
  volatile SEGGER_RTT_CB* pRTTCB;
  volatile SEGGER_RTT_BUFFER_UP* pUp;

  { volatile SEGGER_RTT_CB* pRTTCBInit; pRTTCBInit = (volatile SEGGER_RTT_CB*)((char*)&_SEGGER_RTT + (0)); do { if (pRTTCBInit->acID[0] == '\0') { _DoInit(); } } while (0); };
  pRTTCB = (volatile SEGGER_RTT_CB*)((unsigned char*)&_SEGGER_RTT + (0));
  if (BufferIndex < (3)) {
    { unsigned int _SEGGER_RTT__LockState; __asm volatile ("mrs   %0, primask  \n\t" "movs  r1, #1       \n\t" "msr   primask, r1  \n\t" : "=r" (_SEGGER_RTT__LockState) : : "r1", "cc" );;
    pUp = &pRTTCB->aUp[BufferIndex];
    if (BufferIndex) {
      pUp->sName = sName;
      pUp->pBuffer = (char*)pBuffer;
      pUp->SizeOfBuffer = BufferSize;
      pUp->RdOff = 0u;
      pUp->WrOff = 0u;
    }
    pUp->Flags = Flags;
    __asm volatile ("msr   primask, %0  \n\t" : : "r" (_SEGGER_RTT__LockState) : ); };
    r = 0;
  } else {
    r = -1;
  }
  return r;
}
# 1715 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c"
int SEGGER_RTT_ConfigDownBuffer(unsigned BufferIndex, const char* sName, void* pBuffer, unsigned BufferSize, unsigned Flags) {
  int r;
  volatile SEGGER_RTT_CB* pRTTCB;
  volatile SEGGER_RTT_BUFFER_DOWN* pDown;

  { volatile SEGGER_RTT_CB* pRTTCBInit; pRTTCBInit = (volatile SEGGER_RTT_CB*)((char*)&_SEGGER_RTT + (0)); do { if (pRTTCBInit->acID[0] == '\0') { _DoInit(); } } while (0); };
  pRTTCB = (volatile SEGGER_RTT_CB*)((unsigned char*)&_SEGGER_RTT + (0));
  if (BufferIndex < (3)) {
    { unsigned int _SEGGER_RTT__LockState; __asm volatile ("mrs   %0, primask  \n\t" "movs  r1, #1       \n\t" "msr   primask, r1  \n\t" : "=r" (_SEGGER_RTT__LockState) : : "r1", "cc" );;
    pDown = &pRTTCB->aDown[BufferIndex];
    if (BufferIndex) {
      pDown->sName = sName;
      pDown->pBuffer = (char*)pBuffer;
      pDown->SizeOfBuffer = BufferSize;
      pDown->RdOff = 0u;
      pDown->WrOff = 0u;
    }
    pDown->Flags = Flags;
              ;
    __asm volatile ("msr   primask, %0  \n\t" : : "r" (_SEGGER_RTT__LockState) : ); };
    r = 0;
  } else {
    r = -1;
  }
  return r;
}
# 1758 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c"
int SEGGER_RTT_SetNameUpBuffer(unsigned BufferIndex, const char* sName) {
  int r;
  volatile SEGGER_RTT_CB* pRTTCB;
  volatile SEGGER_RTT_BUFFER_UP* pUp;

  { volatile SEGGER_RTT_CB* pRTTCBInit; pRTTCBInit = (volatile SEGGER_RTT_CB*)((char*)&_SEGGER_RTT + (0)); do { if (pRTTCBInit->acID[0] == '\0') { _DoInit(); } } while (0); };
  pRTTCB = (volatile SEGGER_RTT_CB*)((unsigned char*)&_SEGGER_RTT + (0));
  if (BufferIndex < (3)) {
    { unsigned int _SEGGER_RTT__LockState; __asm volatile ("mrs   %0, primask  \n\t" "movs  r1, #1       \n\t" "msr   primask, r1  \n\t" : "=r" (_SEGGER_RTT__LockState) : : "r1", "cc" );;
    pUp = &pRTTCB->aUp[BufferIndex];
    pUp->sName = sName;
    __asm volatile ("msr   primask, %0  \n\t" : : "r" (_SEGGER_RTT__LockState) : ); };
    r = 0;
  } else {
    r = -1;
  }
  return r;
}
# 1793 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c"
int SEGGER_RTT_SetNameDownBuffer(unsigned BufferIndex, const char* sName) {
  int r;
  volatile SEGGER_RTT_CB* pRTTCB;
  volatile SEGGER_RTT_BUFFER_DOWN* pDown;

  { volatile SEGGER_RTT_CB* pRTTCBInit; pRTTCBInit = (volatile SEGGER_RTT_CB*)((char*)&_SEGGER_RTT + (0)); do { if (pRTTCBInit->acID[0] == '\0') { _DoInit(); } } while (0); };
  pRTTCB = (volatile SEGGER_RTT_CB*)((unsigned char*)&_SEGGER_RTT + (0));
  if (BufferIndex < (3)) {
    { unsigned int _SEGGER_RTT__LockState; __asm volatile ("mrs   %0, primask  \n\t" "movs  r1, #1       \n\t" "msr   primask, r1  \n\t" : "=r" (_SEGGER_RTT__LockState) : : "r1", "cc" );;
    pDown = &pRTTCB->aDown[BufferIndex];
    pDown->sName = sName;
    __asm volatile ("msr   primask, %0  \n\t" : : "r" (_SEGGER_RTT__LockState) : ); };
    r = 0;
  } else {
    r = -1;
  }
  return r;
}
# 1828 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c"
int SEGGER_RTT_SetFlagsUpBuffer(unsigned BufferIndex, unsigned Flags) {
  int r;
  volatile SEGGER_RTT_CB* pRTTCB;
  volatile SEGGER_RTT_BUFFER_UP* pUp;

  { volatile SEGGER_RTT_CB* pRTTCBInit; pRTTCBInit = (volatile SEGGER_RTT_CB*)((char*)&_SEGGER_RTT + (0)); do { if (pRTTCBInit->acID[0] == '\0') { _DoInit(); } } while (0); };
  pRTTCB = (volatile SEGGER_RTT_CB*)((unsigned char*)&_SEGGER_RTT + (0));
  if (BufferIndex < (3)) {
    { unsigned int _SEGGER_RTT__LockState; __asm volatile ("mrs   %0, primask  \n\t" "movs  r1, #1       \n\t" "msr   primask, r1  \n\t" : "=r" (_SEGGER_RTT__LockState) : : "r1", "cc" );;
    pUp = &pRTTCB->aUp[BufferIndex];
    pUp->Flags = Flags;
    __asm volatile ("msr   primask, %0  \n\t" : : "r" (_SEGGER_RTT__LockState) : ); };
    r = 0;
  } else {
    r = -1;
  }
  return r;
}
# 1863 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c"
int SEGGER_RTT_SetFlagsDownBuffer(unsigned BufferIndex, unsigned Flags) {
  int r;
  volatile SEGGER_RTT_CB* pRTTCB;
  volatile SEGGER_RTT_BUFFER_DOWN* pDown;

  { volatile SEGGER_RTT_CB* pRTTCBInit; pRTTCBInit = (volatile SEGGER_RTT_CB*)((char*)&_SEGGER_RTT + (0)); do { if (pRTTCBInit->acID[0] == '\0') { _DoInit(); } } while (0); };
  pRTTCB = (volatile SEGGER_RTT_CB*)((unsigned char*)&_SEGGER_RTT + (0));
  if (BufferIndex < (3)) {
    { unsigned int _SEGGER_RTT__LockState; __asm volatile ("mrs   %0, primask  \n\t" "movs  r1, #1       \n\t" "msr   primask, r1  \n\t" : "=r" (_SEGGER_RTT__LockState) : : "r1", "cc" );;
    pDown = &pRTTCB->aDown[BufferIndex];
    pDown->Flags = Flags;
    __asm volatile ("msr   primask, %0  \n\t" : : "r" (_SEGGER_RTT__LockState) : ); };
    r = 0;
  } else {
    r = -1;
  }
  return r;
}
# 1891 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c"
void SEGGER_RTT_Init (void) {
  _DoInit();
}
# 1912 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c"
int SEGGER_RTT_SetTerminal (unsigned char TerminalId) {
  unsigned char ac[2];
  SEGGER_RTT_BUFFER_UP* pRing;
  unsigned Avail;
  int r;

  { volatile SEGGER_RTT_CB* pRTTCBInit; pRTTCBInit = (volatile SEGGER_RTT_CB*)((char*)&_SEGGER_RTT + (0)); do { if (pRTTCBInit->acID[0] == '\0') { _DoInit(); } } while (0); };
  r = 0;
  ac[0] = 0xFFu;
  if (TerminalId < sizeof(_aTerminalId)) {
    ac[1] = _aTerminalId[TerminalId];
    pRing = (SEGGER_RTT_BUFFER_UP*)((char*)&_SEGGER_RTT.aUp[0] + (0));
    { unsigned int _SEGGER_RTT__LockState; __asm volatile ("mrs   %0, primask  \n\t" "movs  r1, #1       \n\t" "msr   primask, r1  \n\t" : "=r" (_SEGGER_RTT__LockState) : : "r1", "cc" );;
    if ((pRing->Flags & (3)) == (2)) {
      _ActiveTerminal = TerminalId;
      _WriteBlocking(pRing, (const char*)ac, 2u);
    } else {
      Avail = _GetAvailWriteSpace(pRing);
      if (Avail >= 2) {
        _ActiveTerminal = TerminalId;
        _WriteNoCheck(pRing, (const char*)ac, 2u);
      } else {
        r = -1;
      }
    }
    __asm volatile ("msr   primask, %0  \n\t" : : "r" (_SEGGER_RTT__LockState) : ); };
  } else {
    r = -1;
  }
  return r;
}
# 1961 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c"
int SEGGER_RTT_TerminalOut (unsigned char TerminalId, const char* s) {
  int Status;
  unsigned FragLen;
  unsigned Avail;
  SEGGER_RTT_BUFFER_UP* pRing;

  { volatile SEGGER_RTT_CB* pRTTCBInit; pRTTCBInit = (volatile SEGGER_RTT_CB*)((char*)&_SEGGER_RTT + (0)); do { if (pRTTCBInit->acID[0] == '\0') { _DoInit(); } } while (0); };



  if (TerminalId < (char)sizeof(_aTerminalId)) {



    pRing = (SEGGER_RTT_BUFFER_UP*)((char*)&_SEGGER_RTT.aUp[0] + (0));




    FragLen = strlen((s));



    { unsigned int _SEGGER_RTT__LockState; __asm volatile ("mrs   %0, primask  \n\t" "movs  r1, #1       \n\t" "msr   primask, r1  \n\t" : "=r" (_SEGGER_RTT__LockState) : : "r1", "cc" );;
    Avail = _GetAvailWriteSpace(pRing);
    switch (pRing->Flags & (3)) {
    case (0):




      if (Avail < (FragLen + 4u)) {
        Status = 0;
      } else {
        _PostTerminalSwitch(pRing, TerminalId);
        Status = (int)_WriteBlocking(pRing, s, FragLen);
        _PostTerminalSwitch(pRing, _ActiveTerminal);
      }
      break;
    case (1):





      if (Avail < 4u) {
        Status = -1;
      } else {
        _PostTerminalSwitch(pRing, TerminalId);
        Status = (int)_WriteBlocking(pRing, s, (FragLen < (Avail - 4u)) ? FragLen : (Avail - 4u));
        _PostTerminalSwitch(pRing, _ActiveTerminal);
      }
      break;
    case (2):



      _PostTerminalSwitch(pRing, TerminalId);
      Status = (int)_WriteBlocking(pRing, s, FragLen);
      _PostTerminalSwitch(pRing, _ActiveTerminal);
      break;
    default:
      Status = -1;
      break;
    }



    __asm volatile ("msr   primask, %0  \n\t" : : "r" (_SEGGER_RTT__LockState) : ); };
  } else {
    Status = -1;
  }
  return Status;
}
# 2049 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c"
unsigned SEGGER_RTT_GetAvailWriteSpace (unsigned BufferIndex) {
  SEGGER_RTT_BUFFER_UP* pRing;

  pRing = (SEGGER_RTT_BUFFER_UP*)((char*)&_SEGGER_RTT.aUp[BufferIndex] + (0));
  return _GetAvailWriteSpace(pRing);
}
# 2070 "../../../third_party/segger.com/SEGGER_RTT/RTT/SEGGER_RTT.c"
unsigned SEGGER_RTT_GetBytesInBuffer(unsigned BufferIndex) {
  unsigned RdOff;
  unsigned WrOff;
  unsigned r;
  volatile SEGGER_RTT_CB* pRTTCB;




  pRTTCB = (volatile SEGGER_RTT_CB*)((unsigned char*)&_SEGGER_RTT + (0));
  RdOff = pRTTCB->aUp[BufferIndex].RdOff;
  WrOff = pRTTCB->aUp[BufferIndex].WrOff;
  if (RdOff <= WrOff) {
    r = WrOff - RdOff;
  } else {
    r = pRTTCB->aUp[BufferIndex].SizeOfBuffer - (WrOff - RdOff);
  }
  return r;
}
