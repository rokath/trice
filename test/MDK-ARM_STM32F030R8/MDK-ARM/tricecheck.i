# 1 "../../../src/triceCheck.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 365 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "../../../src/triceCheck.c" 2




# 1 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\float.h" 1 3
# 197 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\float.h" 3
unsigned _controlfp(unsigned, unsigned);
unsigned _clearfp(void);
unsigned _statusfp(void);
# 6 "../../../src/triceCheck.c" 2

# 1 "../../../src/trice.h" 1
# 88 "../../../src/trice.h"
typedef enum{
    UartA,
    UartB,
    Rtt0,
    Cgo
} TriceWriteDevice_t;

# 1 "../Core/Inc\\triceConfig.h" 1
# 12 "../Core/Inc\\triceConfig.h"
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
# 13 "../Core/Inc\\triceConfig.h" 2
# 158 "../Core/Inc\\triceConfig.h"
# 1 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include\\cmsis_armclang.h" 1
# 31 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include\\cmsis_armclang.h" 3


# 1 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\arm_compat.h" 1 3






# 1 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\arm_acle.h" 1 3
# 39 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\arm_acle.h" 3
static __inline__ void __attribute__((__always_inline__, __nodebug__)) __wfi(void) {
  __builtin_arm_wfi();
}



static __inline__ void __attribute__((__always_inline__, __nodebug__)) __wfe(void) {
  __builtin_arm_wfe();
}



static __inline__ void __attribute__((__always_inline__, __nodebug__)) __sev(void) {
  __builtin_arm_sev();
}



static __inline__ void __attribute__((__always_inline__, __nodebug__)) __sevl(void) {
  __builtin_arm_sevl();
}



static __inline__ void __attribute__((__always_inline__, __nodebug__)) __yield(void) {
  __builtin_arm_yield();
}







static __inline__ uint32_t __attribute__((__always_inline__, __nodebug__))
__swp(uint32_t __x, volatile uint32_t *__p) {
  uint32_t v;
  do
    v = __builtin_arm_ldrex(__p);
  while (__builtin_arm_strex(__x, __p));
  return v;
}
# 107 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\arm_acle.h" 3
static __inline__ void __attribute__((__always_inline__, __nodebug__)) __nop(void) {
  __builtin_arm_nop();
}





static __inline__ uint32_t __attribute__((__always_inline__, __nodebug__))
__ror(uint32_t __x, uint32_t __y) {
  __y %= 32;
  if (__y == 0)
    return __x;
  return (__x >> __y) | (__x << (32 - __y));
}

static __inline__ uint64_t __attribute__((__always_inline__, __nodebug__))
__rorll(uint64_t __x, uint32_t __y) {
  __y %= 64;
  if (__y == 0)
    return __x;
  return (__x >> __y) | (__x << (64 - __y));
}

static __inline__ unsigned long __attribute__((__always_inline__, __nodebug__))
__rorl(unsigned long __x, uint32_t __y) {

  return __ror(__x, __y);



}



static __inline__ uint32_t __attribute__((__always_inline__, __nodebug__))
__clz(uint32_t __t) {
  return __builtin_clz(__t);
}

static __inline__ unsigned long __attribute__((__always_inline__, __nodebug__))
__clzl(unsigned long __t) {
  return __builtin_clzl(__t);
}

static __inline__ uint64_t __attribute__((__always_inline__, __nodebug__))
__clzll(uint64_t __t) {
  return __builtin_clzll(__t);
}


static __inline__ uint32_t __attribute__((__always_inline__, __nodebug__))
__cls(uint32_t __t) {
  return __builtin_arm_cls(__t);
}

static __inline__ uint32_t __attribute__((__always_inline__, __nodebug__))
__clsl(unsigned long __t) {

  return __builtin_arm_cls(__t);



}

static __inline__ uint32_t __attribute__((__always_inline__, __nodebug__))
__clsll(uint64_t __t) {
  return __builtin_arm_cls64(__t);
}


static __inline__ uint32_t __attribute__((__always_inline__, __nodebug__))
__rev(uint32_t __t) {
  return __builtin_bswap32(__t);
}

static __inline__ unsigned long __attribute__((__always_inline__, __nodebug__))
__revl(unsigned long __t) {

  return __builtin_bswap32(__t);



}

static __inline__ uint64_t __attribute__((__always_inline__, __nodebug__))
__revll(uint64_t __t) {
  return __builtin_bswap64(__t);
}


static __inline__ uint32_t __attribute__((__always_inline__, __nodebug__))
__rev16(uint32_t __t) {
  return __ror(__rev(__t), 16);
}

static __inline__ uint64_t __attribute__((__always_inline__, __nodebug__))
__rev16ll(uint64_t __t) {
  return (((uint64_t)__rev16(__t >> 32)) << 32) | __rev16(__t);
}

static __inline__ unsigned long __attribute__((__always_inline__, __nodebug__))
__rev16l(unsigned long __t) {

    return __rev16(__t);



}


static __inline__ int16_t __attribute__((__always_inline__, __nodebug__))
__revsh(int16_t __t) {
  return __builtin_bswap16(__t);
}


static __inline__ uint32_t __attribute__((__always_inline__, __nodebug__))
__rbit(uint32_t __t) {
  return __builtin_arm_rbit(__t);
}

static __inline__ uint64_t __attribute__((__always_inline__, __nodebug__))
__rbitll(uint64_t __t) {

  return (((uint64_t)__builtin_arm_rbit(__t)) << 32) |
         __builtin_arm_rbit(__t >> 32);



}

static __inline__ unsigned long __attribute__((__always_inline__, __nodebug__))
__rbitl(unsigned long __t) {

  return __rbit(__t);



}
# 8 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\arm_compat.h" 2 3
# 36 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\arm_compat.h" 3
static __inline__ unsigned int __attribute__((unavailable(
    "intrinsic not supported for this architecture"))) __disable_fiq(void);
# 65 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\arm_compat.h" 3
static __inline__ unsigned int __attribute__((__always_inline__, __nodebug__))
__disable_irq(void) {
  unsigned int cpsr;


  __asm__ __volatile__("mrs %[cpsr], primask\n"
                       "cpsid i\n"
                       : [cpsr] "=r"(cpsr));
  return cpsr & 0x1;
# 89 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\arm_compat.h" 3
}



static __inline__ void __attribute__((unavailable(
    "intrinsic not supported for this architecture"))) __enable_fiq(void);
# 112 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\arm_compat.h" 3
static __inline__ void __attribute__((__always_inline__, __nodebug__))
__enable_irq(void) {

  __asm__ __volatile__("cpsie i");
# 124 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\arm_compat.h" 3
}

static __inline__ void __attribute__((__always_inline__, __nodebug__)) __force_stores(void) {
    __asm__ __volatile__ ("" : : : "memory", "cc");
}

static void __attribute__((__always_inline__, __nodebug__)) __memory_changed(void) {
    __asm__ __volatile__ ("" : : : "memory", "cc");
}

static void __attribute__((__always_inline__, __nodebug__)) __schedule_barrier(void) {
    __asm__ __volatile__ ("" : : : "memory", "cc");
}

static __inline__ int __attribute__((__always_inline__, __nodebug__))
__semihost(int val, const void *ptr) {
  register int v __asm__("r0") = val;
  register const void *p __asm__("r1") = ptr;
  __asm__ __volatile__(


      "bkpt 0xab"
# 160 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\arm_compat.h" 3
      : "+r"(v), "+r"(p)
      :
      : "memory", "cc");
  return v;
}
# 181 "C:\\Keil_v5\\ARM\\ARMCLANG\\Bin\\..\\include\\arm_compat.h" 3
static __inline__ unsigned int __attribute__((
    unavailable("intrinsic not supported for targets without floating point")))
__vfp_status(unsigned int mask, unsigned int flags);
# 34 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include\\cmsis_armclang.h" 2 3
# 68 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include\\cmsis_armclang.h" 3
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpacked"

  struct __attribute__((packed)) T_UINT32 { uint32_t v; };
#pragma clang diagnostic pop



#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpacked"

  struct __attribute__((packed, aligned(1))) T_UINT16_WRITE { uint16_t v; };
#pragma clang diagnostic pop



#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpacked"

  struct __attribute__((packed, aligned(1))) T_UINT16_READ { uint16_t v; };
#pragma clang diagnostic pop



#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpacked"

  struct __attribute__((packed, aligned(1))) T_UINT32_WRITE { uint32_t v; };
#pragma clang diagnostic pop



#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpacked"

  struct __attribute__((packed, aligned(1))) T_UINT32_READ { uint32_t v; };
#pragma clang diagnostic pop
# 142 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include\\cmsis_armclang.h" 3
__attribute__((always_inline)) static __inline uint32_t __get_CONTROL(void)
{
  uint32_t result;

  __asm volatile ("MRS %0, control" : "=r" (result) );
  return(result);
}
# 172 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include\\cmsis_armclang.h" 3
__attribute__((always_inline)) static __inline void __set_CONTROL(uint32_t control)
{
  __asm volatile ("MSR control, %0" : : "r" (control) : "memory");
}
# 196 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include\\cmsis_armclang.h" 3
__attribute__((always_inline)) static __inline uint32_t __get_IPSR(void)
{
  uint32_t result;

  __asm volatile ("MRS %0, ipsr" : "=r" (result) );
  return(result);
}







__attribute__((always_inline)) static __inline uint32_t __get_APSR(void)
{
  uint32_t result;

  __asm volatile ("MRS %0, apsr" : "=r" (result) );
  return(result);
}







__attribute__((always_inline)) static __inline uint32_t __get_xPSR(void)
{
  uint32_t result;

  __asm volatile ("MRS %0, xpsr" : "=r" (result) );
  return(result);
}







__attribute__((always_inline)) static __inline uint32_t __get_PSP(void)
{
  uint32_t result;

  __asm volatile ("MRS %0, psp" : "=r" (result) );
  return(result);
}
# 268 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include\\cmsis_armclang.h" 3
__attribute__((always_inline)) static __inline void __set_PSP(uint32_t topOfProcStack)
{
  __asm volatile ("MSR psp, %0" : : "r" (topOfProcStack) : );
}
# 292 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include\\cmsis_armclang.h" 3
__attribute__((always_inline)) static __inline uint32_t __get_MSP(void)
{
  uint32_t result;

  __asm volatile ("MRS %0, msp" : "=r" (result) );
  return(result);
}
# 322 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include\\cmsis_armclang.h" 3
__attribute__((always_inline)) static __inline void __set_MSP(uint32_t topOfMainStack)
{
  __asm volatile ("MSR msp, %0" : : "r" (topOfMainStack) : );
}
# 373 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include\\cmsis_armclang.h" 3
__attribute__((always_inline)) static __inline uint32_t __get_PRIMASK(void)
{
  uint32_t result;

  __asm volatile ("MRS %0, primask" : "=r" (result) );
  return(result);
}
# 403 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include\\cmsis_armclang.h" 3
__attribute__((always_inline)) static __inline void __set_PRIMASK(uint32_t priMask)
{
  __asm volatile ("MSR primask, %0" : : "r" (priMask) : "memory");
}
# 876 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include\\cmsis_armclang.h" 3
__attribute__((always_inline)) static __inline uint32_t __ROR(uint32_t op1, uint32_t op2)
{
  op2 %= 32U;
  if (op2 == 0U)
  {
    return op1;
  }
  return (op1 >> op2) | (op1 << (32U - op2));
}
# 1121 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include\\cmsis_armclang.h" 3
__attribute__((always_inline)) static __inline int32_t __SSAT(int32_t val, uint32_t sat)
{
  if ((sat >= 1U) && (sat <= 32U))
  {
    const int32_t max = (int32_t)((1U << (sat - 1U)) - 1U);
    const int32_t min = -1 - max ;
    if (val > max)
    {
      return max;
    }
    else if (val < min)
    {
      return min;
    }
  }
  return val;
}
# 1146 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include\\cmsis_armclang.h" 3
__attribute__((always_inline)) static __inline uint32_t __USAT(int32_t val, uint32_t sat)
{
  if (sat <= 31U)
  {
    const uint32_t max = ((1U << sat) - 1U);
    if (val > (int32_t)max)
    {
      return max;
    }
    else if (val < 0)
    {
      return 0U;
    }
  }
  return (uint32_t)val;
}
# 159 "../Core/Inc\\triceConfig.h" 2
# 202 "../Core/Inc\\triceConfig.h"
# 1 "../Core/Inc/main.h" 1
# 31 "../Core/Inc/main.h"
# 1 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_crs.h" 1
# 29 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_crs.h"
# 1 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Device/ST/STM32F0xx/Include\\stm32f0xx.h" 1
# 129 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Device/ST/STM32F0xx/Include\\stm32f0xx.h"
# 1 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Device/ST/STM32F0xx/Include/stm32f030x8.h" 1
# 68 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Device/ST/STM32F0xx/Include/stm32f030x8.h"
typedef enum
{

  NonMaskableInt_IRQn = -14,
  HardFault_IRQn = -13,
  SVC_IRQn = -5,
  PendSV_IRQn = -2,
  SysTick_IRQn = -1,


  WWDG_IRQn = 0,
  RTC_IRQn = 2,
  FLASH_IRQn = 3,
  RCC_IRQn = 4,
  EXTI0_1_IRQn = 5,
  EXTI2_3_IRQn = 6,
  EXTI4_15_IRQn = 7,
  DMA1_Channel1_IRQn = 9,
  DMA1_Channel2_3_IRQn = 10,
  DMA1_Channel4_5_IRQn = 11,
  ADC1_IRQn = 12,
  TIM1_BRK_UP_TRG_COM_IRQn = 13,
  TIM1_CC_IRQn = 14,
  TIM3_IRQn = 16,
  TIM6_IRQn = 17,
  TIM14_IRQn = 19,
  TIM15_IRQn = 20,
  TIM16_IRQn = 21,
  TIM17_IRQn = 22,
  I2C1_IRQn = 23,
  I2C2_IRQn = 24,
  SPI1_IRQn = 25,
  SPI2_IRQn = 26,
  USART1_IRQn = 27,
  USART2_IRQn = 28
} IRQn_Type;





# 1 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include\\core_cm0.h" 1
# 29 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include\\core_cm0.h" 3
# 63 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include\\core_cm0.h" 3
# 1 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include/cmsis_version.h" 1 3
# 29 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include/cmsis_version.h" 3
# 64 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include\\core_cm0.h" 2 3
# 115 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include\\core_cm0.h" 3
# 1 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include/cmsis_compiler.h" 1 3
# 116 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include\\core_cm0.h" 2 3
# 199 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include\\core_cm0.h" 3
typedef union
{
  struct
  {
    uint32_t _reserved0:28;
    uint32_t V:1;
    uint32_t C:1;
    uint32_t Z:1;
    uint32_t N:1;
  } b;
  uint32_t w;
} APSR_Type;
# 229 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include\\core_cm0.h" 3
typedef union
{
  struct
  {
    uint32_t ISR:9;
    uint32_t _reserved0:23;
  } b;
  uint32_t w;
} IPSR_Type;
# 247 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include\\core_cm0.h" 3
typedef union
{
  struct
  {
    uint32_t ISR:9;
    uint32_t _reserved0:15;
    uint32_t T:1;
    uint32_t _reserved1:3;
    uint32_t V:1;
    uint32_t C:1;
    uint32_t Z:1;
    uint32_t N:1;
  } b;
  uint32_t w;
} xPSR_Type;
# 286 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include\\core_cm0.h" 3
typedef union
{
  struct
  {
    uint32_t _reserved0:1;
    uint32_t SPSEL:1;
    uint32_t _reserved1:30;
  } b;
  uint32_t w;
} CONTROL_Type;
# 314 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include\\core_cm0.h" 3
typedef struct
{
  volatile uint32_t ISER[1U];
        uint32_t RESERVED0[31U];
  volatile uint32_t ICER[1U];
        uint32_t RSERVED1[31U];
  volatile uint32_t ISPR[1U];
        uint32_t RESERVED2[31U];
  volatile uint32_t ICPR[1U];
        uint32_t RESERVED3[31U];
        uint32_t RESERVED4[64U];
  volatile uint32_t IP[8U];
} NVIC_Type;
# 341 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include\\core_cm0.h" 3
typedef struct
{
  volatile const uint32_t CPUID;
  volatile uint32_t ICSR;
        uint32_t RESERVED0;
  volatile uint32_t AIRCR;
  volatile uint32_t SCR;
  volatile uint32_t CCR;
        uint32_t RESERVED1;
  volatile uint32_t SHP[2U];
  volatile uint32_t SHCSR;
} SCB_Type;
# 448 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include\\core_cm0.h" 3
typedef struct
{
  volatile uint32_t CTRL;
  volatile uint32_t LOAD;
  volatile uint32_t VAL;
  volatile const uint32_t CALIB;
} SysTick_Type;
# 623 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include\\core_cm0.h" 3
static __inline void __NVIC_EnableIRQ(IRQn_Type IRQn)
{
  if ((int32_t)(IRQn) >= 0)
  {
    ((NVIC_Type *) ((0xE000E000UL) + 0x0100UL) )->ISER[0U] = (uint32_t)(1UL << (((uint32_t)IRQn) & 0x1FUL));
  }
}
# 640 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include\\core_cm0.h" 3
static __inline uint32_t __NVIC_GetEnableIRQ(IRQn_Type IRQn)
{
  if ((int32_t)(IRQn) >= 0)
  {
    return((uint32_t)(((((NVIC_Type *) ((0xE000E000UL) + 0x0100UL) )->ISER[0U] & (1UL << (((uint32_t)IRQn) & 0x1FUL))) != 0UL) ? 1UL : 0UL));
  }
  else
  {
    return(0U);
  }
}
# 659 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include\\core_cm0.h" 3
static __inline void __NVIC_DisableIRQ(IRQn_Type IRQn)
{
  if ((int32_t)(IRQn) >= 0)
  {
    ((NVIC_Type *) ((0xE000E000UL) + 0x0100UL) )->ICER[0U] = (uint32_t)(1UL << (((uint32_t)IRQn) & 0x1FUL));
    __builtin_arm_dsb(0xF);;
    __builtin_arm_isb(0xF);;
  }
}
# 678 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include\\core_cm0.h" 3
static __inline uint32_t __NVIC_GetPendingIRQ(IRQn_Type IRQn)
{
  if ((int32_t)(IRQn) >= 0)
  {
    return((uint32_t)(((((NVIC_Type *) ((0xE000E000UL) + 0x0100UL) )->ISPR[0U] & (1UL << (((uint32_t)IRQn) & 0x1FUL))) != 0UL) ? 1UL : 0UL));
  }
  else
  {
    return(0U);
  }
}
# 697 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include\\core_cm0.h" 3
static __inline void __NVIC_SetPendingIRQ(IRQn_Type IRQn)
{
  if ((int32_t)(IRQn) >= 0)
  {
    ((NVIC_Type *) ((0xE000E000UL) + 0x0100UL) )->ISPR[0U] = (uint32_t)(1UL << (((uint32_t)IRQn) & 0x1FUL));
  }
}
# 712 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include\\core_cm0.h" 3
static __inline void __NVIC_ClearPendingIRQ(IRQn_Type IRQn)
{
  if ((int32_t)(IRQn) >= 0)
  {
    ((NVIC_Type *) ((0xE000E000UL) + 0x0100UL) )->ICPR[0U] = (uint32_t)(1UL << (((uint32_t)IRQn) & 0x1FUL));
  }
}
# 730 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include\\core_cm0.h" 3
static __inline void __NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority)
{
  if ((int32_t)(IRQn) >= 0)
  {
    ((NVIC_Type *) ((0xE000E000UL) + 0x0100UL) )->IP[( (((uint32_t)(int32_t)(IRQn)) >> 2UL) )] = ((uint32_t)(((NVIC_Type *) ((0xE000E000UL) + 0x0100UL) )->IP[( (((uint32_t)(int32_t)(IRQn)) >> 2UL) )] & ~(0xFFUL << ( ((((uint32_t)(int32_t)(IRQn)) ) & 0x03UL) * 8UL))) |
       (((priority << (8U - 2)) & (uint32_t)0xFFUL) << ( ((((uint32_t)(int32_t)(IRQn)) ) & 0x03UL) * 8UL)));
  }
  else
  {
    ((SCB_Type *) ((0xE000E000UL) + 0x0D00UL) )->SHP[( (((((uint32_t)(int32_t)(IRQn)) & 0x0FUL)-8UL) >> 2UL) )] = ((uint32_t)(((SCB_Type *) ((0xE000E000UL) + 0x0D00UL) )->SHP[( (((((uint32_t)(int32_t)(IRQn)) & 0x0FUL)-8UL) >> 2UL) )] & ~(0xFFUL << ( ((((uint32_t)(int32_t)(IRQn)) ) & 0x03UL) * 8UL))) |
       (((priority << (8U - 2)) & (uint32_t)0xFFUL) << ( ((((uint32_t)(int32_t)(IRQn)) ) & 0x03UL) * 8UL)));
  }
}
# 754 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include\\core_cm0.h" 3
static __inline uint32_t __NVIC_GetPriority(IRQn_Type IRQn)
{

  if ((int32_t)(IRQn) >= 0)
  {
    return((uint32_t)(((((NVIC_Type *) ((0xE000E000UL) + 0x0100UL) )->IP[ ( (((uint32_t)(int32_t)(IRQn)) >> 2UL) )] >> ( ((((uint32_t)(int32_t)(IRQn)) ) & 0x03UL) * 8UL) ) & (uint32_t)0xFFUL) >> (8U - 2)));
  }
  else
  {
    return((uint32_t)(((((SCB_Type *) ((0xE000E000UL) + 0x0D00UL) )->SHP[( (((((uint32_t)(int32_t)(IRQn)) & 0x0FUL)-8UL) >> 2UL) )] >> ( ((((uint32_t)(int32_t)(IRQn)) ) & 0x03UL) * 8UL) ) & (uint32_t)0xFFUL) >> (8U - 2)));
  }
}
# 779 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include\\core_cm0.h" 3
static __inline uint32_t NVIC_EncodePriority (uint32_t PriorityGroup, uint32_t PreemptPriority, uint32_t SubPriority)
{
  uint32_t PriorityGroupTmp = (PriorityGroup & (uint32_t)0x07UL);
  uint32_t PreemptPriorityBits;
  uint32_t SubPriorityBits;

  PreemptPriorityBits = ((7UL - PriorityGroupTmp) > (uint32_t)(2)) ? (uint32_t)(2) : (uint32_t)(7UL - PriorityGroupTmp);
  SubPriorityBits = ((PriorityGroupTmp + (uint32_t)(2)) < (uint32_t)7UL) ? (uint32_t)0UL : (uint32_t)((PriorityGroupTmp - 7UL) + (uint32_t)(2));

  return (
           ((PreemptPriority & (uint32_t)((1UL << (PreemptPriorityBits)) - 1UL)) << SubPriorityBits) |
           ((SubPriority & (uint32_t)((1UL << (SubPriorityBits )) - 1UL)))
         );
}
# 806 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include\\core_cm0.h" 3
static __inline void NVIC_DecodePriority (uint32_t Priority, uint32_t PriorityGroup, uint32_t* const pPreemptPriority, uint32_t* const pSubPriority)
{
  uint32_t PriorityGroupTmp = (PriorityGroup & (uint32_t)0x07UL);
  uint32_t PreemptPriorityBits;
  uint32_t SubPriorityBits;

  PreemptPriorityBits = ((7UL - PriorityGroupTmp) > (uint32_t)(2)) ? (uint32_t)(2) : (uint32_t)(7UL - PriorityGroupTmp);
  SubPriorityBits = ((PriorityGroupTmp + (uint32_t)(2)) < (uint32_t)7UL) ? (uint32_t)0UL : (uint32_t)((PriorityGroupTmp - 7UL) + (uint32_t)(2));

  *pPreemptPriority = (Priority >> SubPriorityBits) & (uint32_t)((1UL << (PreemptPriorityBits)) - 1UL);
  *pSubPriority = (Priority ) & (uint32_t)((1UL << (SubPriorityBits )) - 1UL);
}
# 830 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include\\core_cm0.h" 3
static __inline void __NVIC_SetVector(IRQn_Type IRQn, uint32_t vector)
{
  uint32_t *vectors = (uint32_t *)0x0U;
  vectors[(int32_t)IRQn + 16] = vector;
}
# 845 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include\\core_cm0.h" 3
static __inline uint32_t __NVIC_GetVector(IRQn_Type IRQn)
{
  uint32_t *vectors = (uint32_t *)0x0U;
  return vectors[(int32_t)IRQn + 16];
}






__attribute__((__noreturn__)) static __inline void __NVIC_SystemReset(void)
{
  __builtin_arm_dsb(0xF);;

  ((SCB_Type *) ((0xE000E000UL) + 0x0D00UL) )->AIRCR = ((0x5FAUL << 16U) |
                 (1UL << 2U));
  __builtin_arm_dsb(0xF);;

  for(;;)
  {
    __builtin_arm_nop();
  }
}
# 889 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include\\core_cm0.h" 3
static __inline uint32_t SCB_GetFPUType(void)
{
    return 0U;
}
# 920 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include\\core_cm0.h" 3
static __inline uint32_t SysTick_Config(uint32_t ticks)
{
  if ((ticks - 1UL) > (0xFFFFFFUL ))
  {
    return (1UL);
  }

  ((SysTick_Type *) ((0xE000E000UL) + 0x0010UL) )->LOAD = (uint32_t)(ticks - 1UL);
  __NVIC_SetPriority (SysTick_IRQn, (1UL << 2) - 1UL);
  ((SysTick_Type *) ((0xE000E000UL) + 0x0010UL) )->VAL = 0UL;
  ((SysTick_Type *) ((0xE000E000UL) + 0x0010UL) )->CTRL = (1UL << 2U) |
                   (1UL << 1U) |
                   (1UL );
  return (0UL);
}
# 110 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Device/ST/STM32F0xx/Include/stm32f030x8.h" 2
# 1 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Device/ST/STM32F0xx/Include/system_stm32f0xx.h" 1
# 58 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Device/ST/STM32F0xx/Include/system_stm32f0xx.h"
extern uint32_t SystemCoreClock;
extern const uint8_t AHBPrescTable[16];
extern const uint8_t APBPrescTable[8];
# 86 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Device/ST/STM32F0xx/Include/system_stm32f0xx.h"
extern void SystemInit(void);
extern void SystemCoreClockUpdate(void);
# 111 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Device/ST/STM32F0xx/Include/stm32f030x8.h" 2
# 121 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Device/ST/STM32F0xx/Include/stm32f030x8.h"
typedef struct
{
  volatile uint32_t ISR;
  volatile uint32_t IER;
  volatile uint32_t CR;
  volatile uint32_t CFGR1;
  volatile uint32_t CFGR2;
  volatile uint32_t SMPR;
       uint32_t RESERVED1;
       uint32_t RESERVED2;
  volatile uint32_t TR;
       uint32_t RESERVED3;
  volatile uint32_t CHSELR;
       uint32_t RESERVED4[5];
  volatile uint32_t DR;
} ADC_TypeDef;

typedef struct
{
  volatile uint32_t CCR;
} ADC_Common_TypeDef;





typedef struct
{
  volatile uint32_t DR;
  volatile uint8_t IDR;
  uint8_t RESERVED0;
  uint16_t RESERVED1;
  volatile uint32_t CR;
  uint32_t RESERVED2;
  volatile uint32_t INIT;
  volatile uint32_t RESERVED3;
} CRC_TypeDef;





typedef struct
{
  volatile uint32_t IDCODE;
  volatile uint32_t CR;
  volatile uint32_t APB1FZ;
  volatile uint32_t APB2FZ;
}DBGMCU_TypeDef;





typedef struct
{
  volatile uint32_t CCR;
  volatile uint32_t CNDTR;
  volatile uint32_t CPAR;
  volatile uint32_t CMAR;
} DMA_Channel_TypeDef;

typedef struct
{
  volatile uint32_t ISR;
  volatile uint32_t IFCR;
} DMA_TypeDef;





typedef struct
{
  volatile uint32_t IMR;
  volatile uint32_t EMR;
  volatile uint32_t RTSR;
  volatile uint32_t FTSR;
  volatile uint32_t SWIER;
  volatile uint32_t PR;
} EXTI_TypeDef;




typedef struct
{
  volatile uint32_t ACR;
  volatile uint32_t KEYR;
  volatile uint32_t OPTKEYR;
  volatile uint32_t SR;
  volatile uint32_t CR;
  volatile uint32_t AR;
  volatile uint32_t RESERVED;
  volatile uint32_t OBR;
  volatile uint32_t WRPR;
} FLASH_TypeDef;




typedef struct
{
  volatile uint16_t RDP;
  volatile uint16_t USER;
  volatile uint16_t DATA0;
  volatile uint16_t DATA1;
  volatile uint16_t WRP0;
  volatile uint16_t WRP1;
} OB_TypeDef;





typedef struct
{
  volatile uint32_t MODER;
  volatile uint32_t OTYPER;
  volatile uint32_t OSPEEDR;
  volatile uint32_t PUPDR;
  volatile uint32_t IDR;
  volatile uint32_t ODR;
  volatile uint32_t BSRR;
  volatile uint32_t LCKR;
  volatile uint32_t AFR[2];
  volatile uint32_t BRR;
} GPIO_TypeDef;





typedef struct
{
  volatile uint32_t CFGR1;
       uint32_t RESERVED;
  volatile uint32_t EXTICR[4];
  volatile uint32_t CFGR2;
} SYSCFG_TypeDef;





typedef struct
{
  volatile uint32_t CR1;
  volatile uint32_t CR2;
  volatile uint32_t OAR1;
  volatile uint32_t OAR2;
  volatile uint32_t TIMINGR;
  volatile uint32_t TIMEOUTR;
  volatile uint32_t ISR;
  volatile uint32_t ICR;
  volatile uint32_t PECR;
  volatile uint32_t RXDR;
  volatile uint32_t TXDR;
} I2C_TypeDef;





typedef struct
{
  volatile uint32_t KR;
  volatile uint32_t PR;
  volatile uint32_t RLR;
  volatile uint32_t SR;
  volatile uint32_t WINR;
} IWDG_TypeDef;





typedef struct
{
  volatile uint32_t CR;
  volatile uint32_t CSR;
} PWR_TypeDef;





typedef struct
{
  volatile uint32_t CR;
  volatile uint32_t CFGR;
  volatile uint32_t CIR;
  volatile uint32_t APB2RSTR;
  volatile uint32_t APB1RSTR;
  volatile uint32_t AHBENR;
  volatile uint32_t APB2ENR;
  volatile uint32_t APB1ENR;
  volatile uint32_t BDCR;
  volatile uint32_t CSR;
  volatile uint32_t AHBRSTR;
  volatile uint32_t CFGR2;
  volatile uint32_t CFGR3;
  volatile uint32_t CR2;
} RCC_TypeDef;




typedef struct
{
  volatile uint32_t TR;
  volatile uint32_t DR;
  volatile uint32_t CR;
  volatile uint32_t ISR;
  volatile uint32_t PRER;
       uint32_t RESERVED1;
       uint32_t RESERVED2;
  volatile uint32_t ALRMAR;
       uint32_t RESERVED3;
  volatile uint32_t WPR;
  volatile uint32_t SSR;
  volatile uint32_t SHIFTR;
  volatile uint32_t TSTR;
  volatile uint32_t TSDR;
  volatile uint32_t TSSSR;
  volatile uint32_t CALR;
  volatile uint32_t TAFCR;
  volatile uint32_t ALRMASSR;
} RTC_TypeDef;





typedef struct
{
  volatile uint32_t CR1;
  volatile uint32_t CR2;
  volatile uint32_t SR;
  volatile uint32_t DR;
  volatile uint32_t CRCPR;
  volatile uint32_t RXCRCR;
  volatile uint32_t TXCRCR;
  volatile uint32_t I2SCFGR;
} SPI_TypeDef;




typedef struct
{
  volatile uint32_t CR1;
  volatile uint32_t CR2;
  volatile uint32_t SMCR;
  volatile uint32_t DIER;
  volatile uint32_t SR;
  volatile uint32_t EGR;
  volatile uint32_t CCMR1;
  volatile uint32_t CCMR2;
  volatile uint32_t CCER;
  volatile uint32_t CNT;
  volatile uint32_t PSC;
  volatile uint32_t ARR;
  volatile uint32_t RCR;
  volatile uint32_t CCR1;
  volatile uint32_t CCR2;
  volatile uint32_t CCR3;
  volatile uint32_t CCR4;
  volatile uint32_t BDTR;
  volatile uint32_t DCR;
  volatile uint32_t DMAR;
  volatile uint32_t OR;
} TIM_TypeDef;





typedef struct
{
  volatile uint32_t CR1;
  volatile uint32_t CR2;
  volatile uint32_t CR3;
  volatile uint32_t BRR;
  volatile uint32_t GTPR;
  volatile uint32_t RTOR;
  volatile uint32_t RQR;
  volatile uint32_t ISR;
  volatile uint32_t ICR;
  volatile uint16_t RDR;
  uint16_t RESERVED1;
  volatile uint16_t TDR;
  uint16_t RESERVED2;
} USART_TypeDef;




typedef struct
{
  volatile uint32_t CR;
  volatile uint32_t CFR;
  volatile uint32_t SR;
} WWDG_TypeDef;
# 130 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Device/ST/STM32F0xx/Include\\stm32f0xx.h" 2
# 169 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Device/ST/STM32F0xx/Include\\stm32f0xx.h"
typedef enum
{
  RESET = 0U,
  SET = !RESET
} FlagStatus, ITStatus;

typedef enum
{
  DISABLE = 0U,
  ENABLE = !DISABLE
} FunctionalState;


typedef enum
{
  SUCCESS = 0U,
  ERROR = !SUCCESS
} ErrorStatus;
# 30 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_crs.h" 2
# 32 "../Core/Inc/main.h" 2
# 1 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_rcc.h" 1
# 88 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_rcc.h"
typedef struct
{
  uint32_t SYSCLK_Frequency;
  uint32_t HCLK_Frequency;
  uint32_t PCLK1_Frequency;
} LL_RCC_ClocksTypeDef;
# 669 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_rcc.h"
static __inline void LL_RCC_HSE_EnableCSS(void)
{
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CR) |= ((0x1UL << (19U))));
}







static __inline void LL_RCC_HSE_DisableCSS(void)
{
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CR) &= ~((0x1UL << (19U))));
}






static __inline void LL_RCC_HSE_EnableBypass(void)
{
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CR) |= ((0x1UL << (18U))));
}






static __inline void LL_RCC_HSE_DisableBypass(void)
{
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CR) &= ~((0x1UL << (18U))));
}






static __inline void LL_RCC_HSE_Enable(void)
{
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CR) |= ((0x1UL << (16U))));
}






static __inline void LL_RCC_HSE_Disable(void)
{
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CR) &= ~((0x1UL << (16U))));
}






static __inline uint32_t LL_RCC_HSE_IsReady(void)
{
  return (((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CR) & ((0x1UL << (17U)))) == ((0x1UL << (17U))));
}
# 748 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_rcc.h"
static __inline void LL_RCC_HSI_Enable(void)
{
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CR) |= ((0x1UL << (0U))));
}






static __inline void LL_RCC_HSI_Disable(void)
{
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CR) &= ~((0x1UL << (0U))));
}






static __inline uint32_t LL_RCC_HSI_IsReady(void)
{
  return (((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CR) & ((0x1UL << (1U)))) == ((0x1UL << (1U))));
}
# 780 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_rcc.h"
static __inline uint32_t LL_RCC_HSI_GetCalibration(void)
{
  return (uint32_t)(((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CR) & ((0xFFUL << (8U)))) >> (8U));
}
# 794 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_rcc.h"
static __inline void LL_RCC_HSI_SetCalibTrimming(uint32_t Value)
{
  (((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CR)) = ((((((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CR))) & (~((0x1FUL << (3U))))) | (Value << (3U)))));
}






static __inline uint32_t LL_RCC_HSI_GetCalibTrimming(void)
{
  return (uint32_t)(((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CR) & ((0x1FUL << (3U)))) >> (3U));
}
# 873 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_rcc.h"
static __inline void LL_RCC_HSI14_Enable(void)
{
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CR2) |= ((0x1UL << (0U))));
}






static __inline void LL_RCC_HSI14_Disable(void)
{
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CR2) &= ~((0x1UL << (0U))));
}






static __inline uint32_t LL_RCC_HSI14_IsReady(void)
{
  return (((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CR2) & ((0x1UL << (1U)))) == ((0x1UL << (1U))));
}






static __inline void LL_RCC_HSI14_EnableADCControl(void)
{
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CR2) &= ~((0x1UL << (2U))));
}






static __inline void LL_RCC_HSI14_DisableADCControl(void)
{
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CR2) |= ((0x1UL << (2U))));
}
# 927 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_rcc.h"
static __inline void LL_RCC_HSI14_SetCalibTrimming(uint32_t Value)
{
  (((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CR2)) = ((((((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CR2))) & (~((0x1FUL << (3U))))) | (Value << (uint32_t)3U))));
}
# 939 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_rcc.h"
static __inline uint32_t LL_RCC_HSI14_GetCalibTrimming(void)
{
  return (uint32_t)(((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CR2) & ((0x1FUL << (3U)))) >> (uint32_t)3U);
}






static __inline uint32_t LL_RCC_HSI14_GetCalibration(void)
{
  return (uint32_t)(((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CR2) & ((0xFFUL << (8U)))) >> (uint32_t)8U);
}
# 967 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_rcc.h"
static __inline void LL_RCC_LSE_Enable(void)
{
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->BDCR) |= ((0x1UL << (0U))));
}






static __inline void LL_RCC_LSE_Disable(void)
{
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->BDCR) &= ~((0x1UL << (0U))));
}






static __inline void LL_RCC_LSE_EnableBypass(void)
{
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->BDCR) |= ((0x1UL << (2U))));
}






static __inline void LL_RCC_LSE_DisableBypass(void)
{
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->BDCR) &= ~((0x1UL << (2U))));
}
# 1013 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_rcc.h"
static __inline void LL_RCC_LSE_SetDriveCapability(uint32_t LSEDrive)
{
  (((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->BDCR)) = ((((((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->BDCR))) & (~((0x3UL << (3U))))) | (LSEDrive))));
}
# 1027 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_rcc.h"
static __inline uint32_t LL_RCC_LSE_GetDriveCapability(void)
{
  return (uint32_t)(((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->BDCR) & ((0x3UL << (3U)))));
}






static __inline uint32_t LL_RCC_LSE_IsReady(void)
{
  return (((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->BDCR) & ((0x1UL << (1U)))) == ((0x1UL << (1U))));
}
# 1055 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_rcc.h"
static __inline void LL_RCC_LSI_Enable(void)
{
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CSR) |= ((0x1UL << (0U))));
}






static __inline void LL_RCC_LSI_Disable(void)
{
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CSR) &= ~((0x1UL << (0U))));
}






static __inline uint32_t LL_RCC_LSI_IsReady(void)
{
  return (((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CSR) & ((0x1UL << (1U)))) == ((0x1UL << (1U))));
}
# 1100 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_rcc.h"
static __inline void LL_RCC_SetSysClkSource(uint32_t Source)
{
  (((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CFGR)) = ((((((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CFGR))) & (~((0x3UL << (0U))))) | (Source))));
}
# 1116 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_rcc.h"
static __inline uint32_t LL_RCC_GetSysClkSource(void)
{
  return (uint32_t)(((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CFGR) & ((0x3UL << (2U)))));
}
# 1136 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_rcc.h"
static __inline void LL_RCC_SetAHBPrescaler(uint32_t Prescaler)
{
  (((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CFGR)) = ((((((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CFGR))) & (~((0xFUL << (4U))))) | (Prescaler))));
}
# 1152 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_rcc.h"
static __inline void LL_RCC_SetAPB1Prescaler(uint32_t Prescaler)
{
  (((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CFGR)) = ((((((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CFGR))) & (~((0x7UL << (8U))))) | (Prescaler))));
}
# 1171 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_rcc.h"
static __inline uint32_t LL_RCC_GetAHBPrescaler(void)
{
  return (uint32_t)(((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CFGR) & ((0xFUL << (4U)))));
}
# 1186 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_rcc.h"
static __inline uint32_t LL_RCC_GetAPB1Prescaler(void)
{
  return (uint32_t)(((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CFGR) & ((0x7UL << (8U)))));
}
# 1230 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_rcc.h"
static __inline void LL_RCC_ConfigMCO(uint32_t MCOxSource, uint32_t MCOxPrescaler)
{







  (((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CFGR)) = ((((((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CFGR))) & (~((0xFUL << (24U))))) | (MCOxSource))));

}
# 1273 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_rcc.h"
static __inline void LL_RCC_SetUSARTClockSource(uint32_t USARTxSource)
{
  (((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CFGR3)) = ((((((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CFGR3))) & (~(((0x3UL << (0U)) << ((USARTxSource & 0xFF000000U) >> 24U))))) | ((USARTxSource & 0x00FFFFFFU)))));
}
# 1286 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_rcc.h"
static __inline void LL_RCC_SetI2CClockSource(uint32_t I2CxSource)
{
  (((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CFGR3)) = ((((((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CFGR3))) & (~((0x1UL << (4U))))) | (I2CxSource))));
}
# 1351 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_rcc.h"
static __inline uint32_t LL_RCC_GetUSARTClockSource(uint32_t USARTx)
{
  return (uint32_t)(((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CFGR3) & (((0x3UL << (0U)) << USARTx))) | (USARTx << 24U));
}
# 1365 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_rcc.h"
static __inline uint32_t LL_RCC_GetI2CClockSource(uint32_t I2Cx)
{
  return (uint32_t)(((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CFGR3) & (I2Cx)));
}
# 1425 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_rcc.h"
static __inline void LL_RCC_SetRTCClockSource(uint32_t Source)
{
  (((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->BDCR)) = ((((((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->BDCR))) & (~((0x3UL << (8U))))) | (Source))));
}
# 1439 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_rcc.h"
static __inline uint32_t LL_RCC_GetRTCClockSource(void)
{
  return (uint32_t)(((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->BDCR) & ((0x3UL << (8U)))));
}






static __inline void LL_RCC_EnableRTC(void)
{
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->BDCR) |= ((0x1UL << (15U))));
}






static __inline void LL_RCC_DisableRTC(void)
{
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->BDCR) &= ~((0x1UL << (15U))));
}






static __inline uint32_t LL_RCC_IsEnabledRTC(void)
{
  return (((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->BDCR) & ((0x1UL << (15U)))) == ((0x1UL << (15U))));
}






static __inline void LL_RCC_ForceBackupDomainReset(void)
{
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->BDCR) |= ((0x1UL << (16U))));
}






static __inline void LL_RCC_ReleaseBackupDomainReset(void)
{
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->BDCR) &= ~((0x1UL << (16U))));
}
# 1507 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_rcc.h"
static __inline void LL_RCC_PLL_Enable(void)
{
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CR) |= ((0x1UL << (24U))));
}







static __inline void LL_RCC_PLL_Disable(void)
{
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CR) &= ~((0x1UL << (24U))));
}






static __inline uint32_t LL_RCC_PLL_IsReady(void)
{
  return (((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CR) & ((0x1UL << (25U)))) == ((0x1UL << (25U))));
}
# 1629 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_rcc.h"
static __inline void LL_RCC_PLL_ConfigDomain_SYS(uint32_t Source, uint32_t PLLMul)
{
  (((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CFGR)) = ((((((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CFGR))) & (~((0x1UL << (16U)) | (0xFUL << (18U))))) | ((Source & (0x1UL << (16U))) | PLLMul))));
  (((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CFGR2)) = ((((((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CFGR2))) & (~((0xFUL << (0U))))) | ((Source & (0xFUL << (0U)))))));
}
# 1649 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_rcc.h"
static __inline void LL_RCC_PLL_SetMainSource(uint32_t PLLSource)
{
  (((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CFGR)) = ((((((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CFGR))) & (~((0x1UL << (16U))))) | (PLLSource))));
}
# 1666 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_rcc.h"
static __inline uint32_t LL_RCC_PLL_GetMainSource(void)
{
  return (uint32_t)(((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CFGR) & ((0x1UL << (16U)))));
}
# 1691 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_rcc.h"
static __inline uint32_t LL_RCC_PLL_GetMultiplicator(void)
{
  return (uint32_t)(((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CFGR) & ((0xFUL << (18U)))));
}
# 1718 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_rcc.h"
static __inline uint32_t LL_RCC_PLL_GetPrediv(void)
{
  return (uint32_t)(((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CFGR2) & ((0xFUL << (0U)))));
}
# 1736 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_rcc.h"
static __inline void LL_RCC_ClearFlag_LSIRDY(void)
{
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CIR) |= ((0x1UL << (16U))));
}






static __inline void LL_RCC_ClearFlag_LSERDY(void)
{
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CIR) |= ((0x1UL << (17U))));
}






static __inline void LL_RCC_ClearFlag_HSIRDY(void)
{
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CIR) |= ((0x1UL << (18U))));
}






static __inline void LL_RCC_ClearFlag_HSERDY(void)
{
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CIR) |= ((0x1UL << (19U))));
}






static __inline void LL_RCC_ClearFlag_PLLRDY(void)
{
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CIR) |= ((0x1UL << (20U))));
}






static __inline void LL_RCC_ClearFlag_HSI14RDY(void)
{
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CIR) |= ((0x1UL << (21U))));
}
# 1808 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_rcc.h"
static __inline void LL_RCC_ClearFlag_HSECSS(void)
{
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CIR) |= ((0x1UL << (23U))));
}






static __inline uint32_t LL_RCC_IsActiveFlag_LSIRDY(void)
{
  return (((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CIR) & ((0x1UL << (0U)))) == ((0x1UL << (0U))));
}






static __inline uint32_t LL_RCC_IsActiveFlag_LSERDY(void)
{
  return (((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CIR) & ((0x1UL << (1U)))) == ((0x1UL << (1U))));
}






static __inline uint32_t LL_RCC_IsActiveFlag_HSIRDY(void)
{
  return (((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CIR) & ((0x1UL << (2U)))) == ((0x1UL << (2U))));
}






static __inline uint32_t LL_RCC_IsActiveFlag_HSERDY(void)
{
  return (((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CIR) & ((0x1UL << (3U)))) == ((0x1UL << (3U))));
}






static __inline uint32_t LL_RCC_IsActiveFlag_PLLRDY(void)
{
  return (((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CIR) & ((0x1UL << (4U)))) == ((0x1UL << (4U))));
}






static __inline uint32_t LL_RCC_IsActiveFlag_HSI14RDY(void)
{
  return (((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CIR) & ((0x1UL << (5U)))) == ((0x1UL << (5U))));
}
# 1890 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_rcc.h"
static __inline uint32_t LL_RCC_IsActiveFlag_HSECSS(void)
{
  return (((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CIR) & ((0x1UL << (7U)))) == ((0x1UL << (7U))));
}






static __inline uint32_t LL_RCC_IsActiveFlag_IWDGRST(void)
{
  return (((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CSR) & ((0x1UL << (29U)))) == ((0x1UL << (29U))));
}






static __inline uint32_t LL_RCC_IsActiveFlag_LPWRRST(void)
{
  return (((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CSR) & ((0x1UL << (31U)))) == ((0x1UL << (31U))));
}






static __inline uint32_t LL_RCC_IsActiveFlag_OBLRST(void)
{
  return (((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CSR) & ((0x1UL << (25U)))) == ((0x1UL << (25U))));
}






static __inline uint32_t LL_RCC_IsActiveFlag_PINRST(void)
{
  return (((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CSR) & ((0x1UL << (26U)))) == ((0x1UL << (26U))));
}






static __inline uint32_t LL_RCC_IsActiveFlag_PORRST(void)
{
  return (((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CSR) & ((0x1UL << (27U)))) == ((0x1UL << (27U))));
}






static __inline uint32_t LL_RCC_IsActiveFlag_SFTRST(void)
{
  return (((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CSR) & ((0x1UL << (28U)))) == ((0x1UL << (28U))));
}






static __inline uint32_t LL_RCC_IsActiveFlag_WWDGRST(void)
{
  return (((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CSR) & ((0x1UL << (30U)))) == ((0x1UL << (30U))));
}







static __inline uint32_t LL_RCC_IsActiveFlag_V18PWRRST(void)
{
  return (((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CSR) & ((0x1UL << (23U)))) == ((0x1UL << (23U))));
}







static __inline void LL_RCC_ClearResetFlags(void)
{
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CSR) |= ((0x1UL << (24U))));
}
# 2000 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_rcc.h"
static __inline void LL_RCC_EnableIT_LSIRDY(void)
{
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CIR) |= ((0x1UL << (8U))));
}






static __inline void LL_RCC_EnableIT_LSERDY(void)
{
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CIR) |= ((0x1UL << (9U))));
}






static __inline void LL_RCC_EnableIT_HSIRDY(void)
{
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CIR) |= ((0x1UL << (10U))));
}






static __inline void LL_RCC_EnableIT_HSERDY(void)
{
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CIR) |= ((0x1UL << (11U))));
}






static __inline void LL_RCC_EnableIT_PLLRDY(void)
{
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CIR) |= ((0x1UL << (12U))));
}






static __inline void LL_RCC_EnableIT_HSI14RDY(void)
{
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CIR) |= ((0x1UL << (13U))));
}
# 2072 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_rcc.h"
static __inline void LL_RCC_DisableIT_LSIRDY(void)
{
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CIR) &= ~((0x1UL << (8U))));
}






static __inline void LL_RCC_DisableIT_LSERDY(void)
{
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CIR) &= ~((0x1UL << (9U))));
}






static __inline void LL_RCC_DisableIT_HSIRDY(void)
{
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CIR) &= ~((0x1UL << (10U))));
}






static __inline void LL_RCC_DisableIT_HSERDY(void)
{
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CIR) &= ~((0x1UL << (11U))));
}






static __inline void LL_RCC_DisableIT_PLLRDY(void)
{
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CIR) &= ~((0x1UL << (12U))));
}






static __inline void LL_RCC_DisableIT_HSI14RDY(void)
{
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CIR) &= ~((0x1UL << (13U))));
}
# 2144 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_rcc.h"
static __inline uint32_t LL_RCC_IsEnabledIT_LSIRDY(void)
{
  return (((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CIR) & ((0x1UL << (8U)))) == ((0x1UL << (8U))));
}






static __inline uint32_t LL_RCC_IsEnabledIT_LSERDY(void)
{
  return (((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CIR) & ((0x1UL << (9U)))) == ((0x1UL << (9U))));
}






static __inline uint32_t LL_RCC_IsEnabledIT_HSIRDY(void)
{
  return (((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CIR) & ((0x1UL << (10U)))) == ((0x1UL << (10U))));
}






static __inline uint32_t LL_RCC_IsEnabledIT_HSERDY(void)
{
  return (((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CIR) & ((0x1UL << (11U)))) == ((0x1UL << (11U))));
}






static __inline uint32_t LL_RCC_IsEnabledIT_PLLRDY(void)
{
  return (((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CIR) & ((0x1UL << (12U)))) == ((0x1UL << (12U))));
}






static __inline uint32_t LL_RCC_IsEnabledIT_HSI14RDY(void)
{
  return (((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CIR) & ((0x1UL << (13U)))) == ((0x1UL << (13U))));
}
# 2219 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_rcc.h"
ErrorStatus LL_RCC_DeInit(void);







void LL_RCC_GetSystemClocksFreq(LL_RCC_ClocksTypeDef *RCC_Clocks);
uint32_t LL_RCC_GetUSARTClockFreq(uint32_t USARTxSource);
uint32_t LL_RCC_GetI2CClockFreq(uint32_t I2CxSource);
# 33 "../Core/Inc/main.h" 2
# 1 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_bus.h" 1
# 227 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_bus.h"
static __inline void LL_AHB1_GRP1_EnableClock(uint32_t Periphs)
{
  volatile uint32_t tmpreg;
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->AHBENR) |= (Periphs));

  tmpreg = ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->AHBENR) & (Periphs));
  (void)tmpreg;
}
# 267 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_bus.h"
static __inline uint32_t LL_AHB1_GRP1_IsEnabledClock(uint32_t Periphs)
{
  return (((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->AHBENR) & (Periphs)) == Periphs);
}
# 303 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_bus.h"
static __inline void LL_AHB1_GRP1_DisableClock(uint32_t Periphs)
{
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->AHBENR) &= ~(Periphs));
}
# 330 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_bus.h"
static __inline void LL_AHB1_GRP1_ForceReset(uint32_t Periphs)
{
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->AHBRSTR) |= (Periphs));
}
# 357 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_bus.h"
static __inline void LL_AHB1_GRP1_ReleaseReset(uint32_t Periphs)
{
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->AHBRSTR) &= ~(Periphs));
}
# 415 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_bus.h"
static __inline void LL_APB1_GRP1_EnableClock(uint32_t Periphs)
{
  volatile uint32_t tmpreg;
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->APB1ENR) |= (Periphs));

  tmpreg = ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->APB1ENR) & (Periphs));
  (void)tmpreg;
}
# 469 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_bus.h"
static __inline uint32_t LL_APB1_GRP1_IsEnabledClock(uint32_t Periphs)
{
  return (((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->APB1ENR) & (Periphs)) == Periphs);
}
# 519 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_bus.h"
static __inline void LL_APB1_GRP1_DisableClock(uint32_t Periphs)
{
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->APB1ENR) &= ~(Periphs));
}
# 570 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_bus.h"
static __inline void LL_APB1_GRP1_ForceReset(uint32_t Periphs)
{
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->APB1RSTR) |= (Periphs));
}
# 621 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_bus.h"
static __inline void LL_APB1_GRP1_ReleaseReset(uint32_t Periphs)
{
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->APB1RSTR) &= ~(Periphs));
}
# 665 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_bus.h"
static __inline void LL_APB1_GRP2_EnableClock(uint32_t Periphs)
{
  volatile uint32_t tmpreg;
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->APB2ENR) |= (Periphs));

  tmpreg = ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->APB2ENR) & (Periphs));
  (void)tmpreg;
}
# 705 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_bus.h"
static __inline uint32_t LL_APB1_GRP2_IsEnabledClock(uint32_t Periphs)
{
  return (((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->APB2ENR) & (Periphs)) == Periphs);
}
# 741 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_bus.h"
static __inline void LL_APB1_GRP2_DisableClock(uint32_t Periphs)
{
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->APB2ENR) &= ~(Periphs));
}
# 778 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_bus.h"
static __inline void LL_APB1_GRP2_ForceReset(uint32_t Periphs)
{
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->APB2RSTR) |= (Periphs));
}
# 815 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_bus.h"
static __inline void LL_APB1_GRP2_ReleaseReset(uint32_t Periphs)
{
  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->APB2RSTR) &= ~(Periphs));
}
# 34 "../Core/Inc/main.h" 2
# 1 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_system.h" 1
# 349 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_system.h"
static __inline void LL_SYSCFG_SetRemapMemory(uint32_t Memory)
{
  (((((SYSCFG_TypeDef *) (0x40000000UL + 0x00010000UL))->CFGR1)) = ((((((((SYSCFG_TypeDef *) (0x40000000UL + 0x00010000UL))->CFGR1))) & (~((0x3UL << (0U))))) | (Memory))));
}
# 362 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_system.h"
static __inline uint32_t LL_SYSCFG_GetRemapMemory(void)
{
  return (uint32_t)(((((SYSCFG_TypeDef *) (0x40000000UL + 0x00010000UL))->CFGR1) & ((0x3UL << (0U)))));
}
# 416 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_system.h"
static __inline void LL_SYSCFG_SetRemapDMA_USART(uint32_t Remap)
{
  (((((SYSCFG_TypeDef *) (0x40000000UL + 0x00010000UL))->CFGR1)) = ((((((((SYSCFG_TypeDef *) (0x40000000UL + 0x00010000UL))->CFGR1))) & (~((Remap & 0x00FF00FFU) << 8U))) | ((Remap & 0xFF00FF00U)))));
}
# 461 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_system.h"
static __inline void LL_SYSCFG_SetRemapDMA_ADC(uint32_t Remap)
{
  (((((SYSCFG_TypeDef *) (0x40000000UL + 0x00010000UL))->CFGR1)) = ((((((((SYSCFG_TypeDef *) (0x40000000UL + 0x00010000UL))->CFGR1))) & (~((0x1UL << (8U))))) | (Remap))));
}
# 494 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_system.h"
static __inline void LL_SYSCFG_SetRemapDMA_TIM(uint32_t Remap)
{
  (((((SYSCFG_TypeDef *) (0x40000000UL + 0x00010000UL))->CFGR1)) = ((((((((SYSCFG_TypeDef *) (0x40000000UL + 0x00010000UL))->CFGR1))) & (~((Remap & 0x00FF00FFU) << 8U))) | ((Remap & 0xFF00FF00U)))));
}
# 547 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_system.h"
static __inline void LL_SYSCFG_EnableFastModePlus(uint32_t ConfigFastModePlus)
{
  ((((SYSCFG_TypeDef *) (0x40000000UL + 0x00010000UL))->CFGR1) |= (ConfigFastModePlus));
}
# 575 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_system.h"
static __inline void LL_SYSCFG_DisableFastModePlus(uint32_t ConfigFastModePlus)
{
  ((((SYSCFG_TypeDef *) (0x40000000UL + 0x00010000UL))->CFGR1) &= ~(ConfigFastModePlus));
}
# 626 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_system.h"
static __inline void LL_SYSCFG_SetEXTISource(uint32_t Port, uint32_t Line)
{
  (((((SYSCFG_TypeDef *) (0x40000000UL + 0x00010000UL))->EXTICR[Line & 0xFF])) = ((((((((SYSCFG_TypeDef *) (0x40000000UL + 0x00010000UL))->EXTICR[Line & 0xFF]))) & (~((0xFUL << (0U)) << (Line >> 16)))) | (Port << (Line >> 16)))));
}
# 676 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_system.h"
static __inline uint32_t LL_SYSCFG_GetEXTISource(uint32_t Line)
{
  return (uint32_t)(((((SYSCFG_TypeDef *) (0x40000000UL + 0x00010000UL))->EXTICR[Line & 0xFF]) & (((0xFUL << (0U)) << (Line >> 16)))) >> (Line >> 16));
}
# 1534 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_system.h"
static __inline void LL_SYSCFG_SetTIMBreakInputs(uint32_t Break)
{



  (((((SYSCFG_TypeDef *) (0x40000000UL + 0x00010000UL))->CFGR2)) = ((((((((SYSCFG_TypeDef *) (0x40000000UL + 0x00010000UL))->CFGR2))) & (~((0x1UL << (0U)) | (0x1UL << (1U))))) | (Break))));

}
# 1555 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_system.h"
static __inline uint32_t LL_SYSCFG_GetTIMBreakInputs(void)
{




  return (uint32_t)(((((SYSCFG_TypeDef *) (0x40000000UL + 0x00010000UL))->CFGR2) & ((0x1UL << (0U)) | (0x1UL << (1U)))));

}






static __inline uint32_t LL_SYSCFG_IsActiveFlag_SP(void)
{
  return (((((SYSCFG_TypeDef *) (0x40000000UL + 0x00010000UL))->CFGR2) & ((0x1UL << (8U)))) == ((0x1UL << (8U))));
}






static __inline void LL_SYSCFG_ClearFlag_SP(void)
{
  ((((SYSCFG_TypeDef *) (0x40000000UL + 0x00010000UL))->CFGR2) |= ((0x1UL << (8U))));
}
# 1603 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_system.h"
static __inline uint32_t LL_DBGMCU_GetDeviceID(void)
{
  return (uint32_t)(((((DBGMCU_TypeDef *) (0x40000000UL + 0x00015800UL))->IDCODE) & ((0xFFFUL << (0U)))));
}
# 1615 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_system.h"
static __inline uint32_t LL_DBGMCU_GetRevisionID(void)
{
  return (uint32_t)(((((DBGMCU_TypeDef *) (0x40000000UL + 0x00015800UL))->IDCODE) & ((0xFFFFUL << (16U)))) >> (16U));
}






static __inline void LL_DBGMCU_EnableDBGStopMode(void)
{
  ((((DBGMCU_TypeDef *) (0x40000000UL + 0x00015800UL))->CR) |= ((0x1UL << (1U))));
}






static __inline void LL_DBGMCU_DisableDBGStopMode(void)
{
  ((((DBGMCU_TypeDef *) (0x40000000UL + 0x00015800UL))->CR) &= ~((0x1UL << (1U))));
}






static __inline void LL_DBGMCU_EnableDBGStandbyMode(void)
{
  ((((DBGMCU_TypeDef *) (0x40000000UL + 0x00015800UL))->CR) |= ((0x1UL << (2U))));
}






static __inline void LL_DBGMCU_DisableDBGStandbyMode(void)
{
  ((((DBGMCU_TypeDef *) (0x40000000UL + 0x00015800UL))->CR) &= ~((0x1UL << (2U))));
}
# 1687 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_system.h"
static __inline void LL_DBGMCU_APB1_GRP1_FreezePeriph(uint32_t Periphs)
{
  ((((DBGMCU_TypeDef *) (0x40000000UL + 0x00015800UL))->APB1FZ) |= (Periphs));
}
# 1719 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_system.h"
static __inline void LL_DBGMCU_APB1_GRP1_UnFreezePeriph(uint32_t Periphs)
{
  ((((DBGMCU_TypeDef *) (0x40000000UL + 0x00015800UL))->APB1FZ) &= ~(Periphs));
}
# 1739 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_system.h"
static __inline void LL_DBGMCU_APB1_GRP2_FreezePeriph(uint32_t Periphs)
{
  ((((DBGMCU_TypeDef *) (0x40000000UL + 0x00015800UL))->APB2FZ) |= (Periphs));
}
# 1759 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_system.h"
static __inline void LL_DBGMCU_APB1_GRP2_UnFreezePeriph(uint32_t Periphs)
{
  ((((DBGMCU_TypeDef *) (0x40000000UL + 0x00015800UL))->APB2FZ) &= ~(Periphs));
}
# 1779 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_system.h"
static __inline void LL_FLASH_SetLatency(uint32_t Latency)
{
  (((((FLASH_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00002000UL))->ACR)) = ((((((((FLASH_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00002000UL))->ACR))) & (~((0x1UL << (0U))))) | (Latency))));
}
# 1791 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_system.h"
static __inline uint32_t LL_FLASH_GetLatency(void)
{
  return (uint32_t)(((((FLASH_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00002000UL))->ACR) & ((0x1UL << (0U)))));
}






static __inline void LL_FLASH_EnablePrefetch(void)
{
  ((((FLASH_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00002000UL))->ACR) |= ((0x1UL << (4U))));
}






static __inline void LL_FLASH_DisablePrefetch(void)
{
  ((((FLASH_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00002000UL))->ACR) &= ~((0x1UL << (4U))));
}






static __inline uint32_t LL_FLASH_IsPrefetchEnabled(void)
{
  return (((((FLASH_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00002000UL))->ACR) & ((0x1UL << (5U)))) == ((0x1UL << (5U))));
}
# 35 "../Core/Inc/main.h" 2
# 1 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_exti.h" 1
# 58 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_exti.h"
typedef struct
{

  uint32_t Line_0_31;


  FunctionalState LineCommand;


  uint8_t Mode;


  uint8_t Trigger;

} LL_EXTI_InitTypeDef;
# 274 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_exti.h"
static __inline void LL_EXTI_EnableIT_0_31(uint32_t ExtiLine)
{
  ((((EXTI_TypeDef *) (0x40000000UL + 0x00010400UL))->IMR) |= (ExtiLine));
}
# 322 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_exti.h"
static __inline void LL_EXTI_DisableIT_0_31(uint32_t ExtiLine)
{
  ((((EXTI_TypeDef *) (0x40000000UL + 0x00010400UL))->IMR) &= ~(ExtiLine));
}
# 371 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_exti.h"
static __inline uint32_t LL_EXTI_IsEnabledIT_0_31(uint32_t ExtiLine)
{
  return (((((EXTI_TypeDef *) (0x40000000UL + 0x00010400UL))->IMR) & (ExtiLine)) == (ExtiLine));
}
# 425 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_exti.h"
static __inline void LL_EXTI_EnableEvent_0_31(uint32_t ExtiLine)
{
  ((((EXTI_TypeDef *) (0x40000000UL + 0x00010400UL))->EMR) |= (ExtiLine));

}
# 472 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_exti.h"
static __inline void LL_EXTI_DisableEvent_0_31(uint32_t ExtiLine)
{
  ((((EXTI_TypeDef *) (0x40000000UL + 0x00010400UL))->EMR) &= ~(ExtiLine));
}
# 518 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_exti.h"
static __inline uint32_t LL_EXTI_IsEnabledEvent_0_31(uint32_t ExtiLine)
{
  return (((((EXTI_TypeDef *) (0x40000000UL + 0x00010400UL))->EMR) & (ExtiLine)) == (ExtiLine));

}
# 572 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_exti.h"
static __inline void LL_EXTI_EnableRisingTrig_0_31(uint32_t ExtiLine)
{
  ((((EXTI_TypeDef *) (0x40000000UL + 0x00010400UL))->RTSR) |= (ExtiLine));

}
# 618 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_exti.h"
static __inline void LL_EXTI_DisableRisingTrig_0_31(uint32_t ExtiLine)
{
  ((((EXTI_TypeDef *) (0x40000000UL + 0x00010400UL))->RTSR) &= ~(ExtiLine));

}
# 657 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_exti.h"
static __inline uint32_t LL_EXTI_IsEnabledRisingTrig_0_31(uint32_t ExtiLine)
{
  return (((((EXTI_TypeDef *) (0x40000000UL + 0x00010400UL))->RTSR) & (ExtiLine)) == (ExtiLine));
}
# 710 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_exti.h"
static __inline void LL_EXTI_EnableFallingTrig_0_31(uint32_t ExtiLine)
{
  ((((EXTI_TypeDef *) (0x40000000UL + 0x00010400UL))->FTSR) |= (ExtiLine));
}
# 754 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_exti.h"
static __inline void LL_EXTI_DisableFallingTrig_0_31(uint32_t ExtiLine)
{
  ((((EXTI_TypeDef *) (0x40000000UL + 0x00010400UL))->FTSR) &= ~(ExtiLine));
}
# 792 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_exti.h"
static __inline uint32_t LL_EXTI_IsEnabledFallingTrig_0_31(uint32_t ExtiLine)
{
  return (((((EXTI_TypeDef *) (0x40000000UL + 0x00010400UL))->FTSR) & (ExtiLine)) == (ExtiLine));
}
# 843 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_exti.h"
static __inline void LL_EXTI_GenerateSWI_0_31(uint32_t ExtiLine)
{
  ((((EXTI_TypeDef *) (0x40000000UL + 0x00010400UL))->SWIER) |= (ExtiLine));
}
# 891 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_exti.h"
static __inline uint32_t LL_EXTI_IsActiveFlag_0_31(uint32_t ExtiLine)
{
  return (((((EXTI_TypeDef *) (0x40000000UL + 0x00010400UL))->PR) & (ExtiLine)) == (ExtiLine));
}
# 931 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_exti.h"
static __inline uint32_t LL_EXTI_ReadFlag_0_31(uint32_t ExtiLine)
{
  return (uint32_t)(((((EXTI_TypeDef *) (0x40000000UL + 0x00010400UL))->PR) & (ExtiLine)));
}
# 971 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_exti.h"
static __inline void LL_EXTI_ClearFlag_0_31(uint32_t ExtiLine)
{
  ((((EXTI_TypeDef *) (0x40000000UL + 0x00010400UL))->PR) = (ExtiLine));
}
# 986 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_exti.h"
uint32_t LL_EXTI_Init(LL_EXTI_InitTypeDef *EXTI_InitStruct);
uint32_t LL_EXTI_DeInit(void);
void LL_EXTI_StructInit(LL_EXTI_InitTypeDef *EXTI_InitStruct);
# 36 "../Core/Inc/main.h" 2
# 1 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_cortex.h" 1
# 95 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_cortex.h"
static __inline uint32_t LL_SYSTICK_IsActiveCounterFlag(void)
{
  return ((((SysTick_Type *) ((0xE000E000UL) + 0x0010UL) )->CTRL & (1UL << 16U)) == ((1UL << 16U)));
}
# 108 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_cortex.h"
static __inline void LL_SYSTICK_SetClkSource(uint32_t Source)
{
  if (Source == (1UL << 2U))
  {
    ((((SysTick_Type *) ((0xE000E000UL) + 0x0010UL) )->CTRL) |= ((1UL << 2U)));
  }
  else
  {
    ((((SysTick_Type *) ((0xE000E000UL) + 0x0010UL) )->CTRL) &= ~((1UL << 2U)));
  }
}
# 127 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_cortex.h"
static __inline uint32_t LL_SYSTICK_GetClkSource(void)
{
  return ((((SysTick_Type *) ((0xE000E000UL) + 0x0010UL) )->CTRL) & ((1UL << 2U)));
}






static __inline void LL_SYSTICK_EnableIT(void)
{
  ((((SysTick_Type *) ((0xE000E000UL) + 0x0010UL) )->CTRL) |= ((1UL << 1U)));
}






static __inline void LL_SYSTICK_DisableIT(void)
{
  ((((SysTick_Type *) ((0xE000E000UL) + 0x0010UL) )->CTRL) &= ~((1UL << 1U)));
}






static __inline uint32_t LL_SYSTICK_IsEnabledIT(void)
{
  return (((((SysTick_Type *) ((0xE000E000UL) + 0x0010UL) )->CTRL) & ((1UL << 1U))) == ((1UL << 1U)));
}
# 175 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_cortex.h"
static __inline void LL_LPM_EnableSleep(void)
{

  ((((SCB_Type *) ((0xE000E000UL) + 0x0D00UL) )->SCR) &= ~(((uint32_t)(1UL << 2U))));
}






static __inline void LL_LPM_EnableDeepSleep(void)
{

  ((((SCB_Type *) ((0xE000E000UL) + 0x0D00UL) )->SCR) |= (((uint32_t)(1UL << 2U))));
}
# 199 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_cortex.h"
static __inline void LL_LPM_EnableSleepOnExit(void)
{

  ((((SCB_Type *) ((0xE000E000UL) + 0x0D00UL) )->SCR) |= (((uint32_t)(1UL << 1U))));
}






static __inline void LL_LPM_DisableSleepOnExit(void)
{

  ((((SCB_Type *) ((0xE000E000UL) + 0x0D00UL) )->SCR) &= ~(((uint32_t)(1UL << 1U))));
}







static __inline void LL_LPM_EnableEventOnPend(void)
{

  ((((SCB_Type *) ((0xE000E000UL) + 0x0D00UL) )->SCR) |= (((uint32_t)(1UL << 4U))));
}







static __inline void LL_LPM_DisableEventOnPend(void)
{

  ((((SCB_Type *) ((0xE000E000UL) + 0x0D00UL) )->SCR) &= ~(((uint32_t)(1UL << 4U))));
}
# 253 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_cortex.h"
static __inline uint32_t LL_CPUID_GetImplementer(void)
{
  return (uint32_t)(((((SCB_Type *) ((0xE000E000UL) + 0x0D00UL) )->CPUID) & ((0xFFUL << 24U))) >> 24U);
}






static __inline uint32_t LL_CPUID_GetVariant(void)
{
  return (uint32_t)(((((SCB_Type *) ((0xE000E000UL) + 0x0D00UL) )->CPUID) & ((0xFUL << 20U))) >> 20U);
}






static __inline uint32_t LL_CPUID_GetArchitecture(void)
{
  return (uint32_t)(((((SCB_Type *) ((0xE000E000UL) + 0x0D00UL) )->CPUID) & ((0xFUL << 16U))) >> 16U);
}






static __inline uint32_t LL_CPUID_GetParNo(void)
{
  return (uint32_t)(((((SCB_Type *) ((0xE000E000UL) + 0x0D00UL) )->CPUID) & ((0xFFFUL << 4U))) >> 4U);
}






static __inline uint32_t LL_CPUID_GetRevision(void)
{
  return (uint32_t)(((((SCB_Type *) ((0xE000E000UL) + 0x0D00UL) )->CPUID) & ((0xFUL ))) >> 0U);
}
# 37 "../Core/Inc/main.h" 2
# 1 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_utils.h" 1
# 90 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_utils.h"
typedef struct
{
  uint32_t PLLMul;
# 105 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_utils.h"
  uint32_t Prediv;





} LL_UTILS_PLLInitTypeDef;




typedef struct
{
  uint32_t AHBCLKDivider;





  uint32_t APB1CLKDivider;




} LL_UTILS_ClkInitTypeDef;
# 168 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_utils.h"
static __inline uint32_t LL_GetUID_Word0(void)
{
  return (uint32_t)(((*((uint32_t *)0x1FFFF7ACUL))));
}





static __inline uint32_t LL_GetUID_Word1(void)
{
  return (uint32_t)(((*((uint32_t *)(0x1FFFF7ACUL + 4U)))));
}





static __inline uint32_t LL_GetUID_Word2(void)
{
  return (uint32_t)(((*((uint32_t *)(0x1FFFF7ACUL + 8U)))));
}







static __inline uint32_t LL_GetFlashSize(void)
{
  return (uint16_t)(((*((uint32_t *)0x1FFFF7CCUL))));
}
# 219 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_utils.h"
static __inline void LL_InitTick(uint32_t HCLKFrequency, uint32_t Ticks)
{

  ((SysTick_Type *) ((0xE000E000UL) + 0x0010UL) )->LOAD = (uint32_t)((HCLKFrequency / Ticks) - 1UL);
  ((SysTick_Type *) ((0xE000E000UL) + 0x0010UL) )->VAL = 0UL;
  ((SysTick_Type *) ((0xE000E000UL) + 0x0010UL) )->CTRL = (1UL << 2U) |
                   (1UL );
}

void LL_Init1msTick(uint32_t HCLKFrequency);
void LL_mDelay(uint32_t Delay);
# 239 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_utils.h"
void LL_SetSystemCoreClock(uint32_t HCLKFrequency);

ErrorStatus LL_SetFlashLatency(uint32_t Frequency);

ErrorStatus LL_PLL_ConfigSystemClock_HSI(LL_UTILS_PLLInitTypeDef *UTILS_PLLInitStruct,
                                         LL_UTILS_ClkInitTypeDef *UTILS_ClkInitStruct);




ErrorStatus LL_PLL_ConfigSystemClock_HSE(uint32_t HSEFrequency, uint32_t HSEBypass,
                                         LL_UTILS_PLLInitTypeDef *UTILS_PLLInitStruct, LL_UTILS_ClkInitTypeDef *UTILS_ClkInitStruct);
# 38 "../Core/Inc/main.h" 2
# 1 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_pwr.h" 1
# 212 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_pwr.h"
static __inline void LL_PWR_EnableBkUpAccess(void)
{
  ((((PWR_TypeDef *) (0x40000000UL + 0x00007000UL))->CR) |= ((0x1UL << (8U))));
}






static __inline void LL_PWR_DisableBkUpAccess(void)
{
  ((((PWR_TypeDef *) (0x40000000UL + 0x00007000UL))->CR) &= ~((0x1UL << (8U))));
}






static __inline uint32_t LL_PWR_IsEnabledBkUpAccess(void)
{
  return (((((PWR_TypeDef *) (0x40000000UL + 0x00007000UL))->CR) & ((0x1UL << (8U)))) == ((0x1UL << (8U))));
}
# 246 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_pwr.h"
static __inline void LL_PWR_SetRegulModeDS(uint32_t RegulMode)
{
  (((((PWR_TypeDef *) (0x40000000UL + 0x00007000UL))->CR)) = ((((((((PWR_TypeDef *) (0x40000000UL + 0x00007000UL))->CR))) & (~((0x1UL << (0U))))) | (RegulMode))));
}
# 258 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_pwr.h"
static __inline uint32_t LL_PWR_GetRegulModeDS(void)
{
  return (uint32_t)(((((PWR_TypeDef *) (0x40000000UL + 0x00007000UL))->CR) & ((0x1UL << (0U)))));
}
# 274 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_pwr.h"
static __inline void LL_PWR_SetPowerMode(uint32_t PDMode)
{
  (((((PWR_TypeDef *) (0x40000000UL + 0x00007000UL))->CR)) = ((((((((PWR_TypeDef *) (0x40000000UL + 0x00007000UL))->CR))) & (~(((0x1UL << (1U))| (0x1UL << (0U)))))) | (PDMode))));
}
# 288 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_pwr.h"
static __inline uint32_t LL_PWR_GetPowerMode(void)
{
  return (uint32_t)(((((PWR_TypeDef *) (0x40000000UL + 0x00007000UL))->CR) & (((0x1UL << (1U))| (0x1UL << (0U))))));
}
# 385 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_pwr.h"
static __inline void LL_PWR_EnableWakeUpPin(uint32_t WakeUpPin)
{
  ((((PWR_TypeDef *) (0x40000000UL + 0x00007000UL))->CSR) |= (WakeUpPin));
}
# 413 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_pwr.h"
static __inline void LL_PWR_DisableWakeUpPin(uint32_t WakeUpPin)
{
  ((((PWR_TypeDef *) (0x40000000UL + 0x00007000UL))->CSR) &= ~(WakeUpPin));
}
# 441 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_pwr.h"
static __inline uint32_t LL_PWR_IsEnabledWakeUpPin(uint32_t WakeUpPin)
{
  return (((((PWR_TypeDef *) (0x40000000UL + 0x00007000UL))->CSR) & (WakeUpPin)) == (WakeUpPin));
}
# 460 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_pwr.h"
static __inline uint32_t LL_PWR_IsActiveFlag_WU(void)
{
  return (((((PWR_TypeDef *) (0x40000000UL + 0x00007000UL))->CSR) & ((0x1UL << (0U)))) == ((0x1UL << (0U))));
}






static __inline uint32_t LL_PWR_IsActiveFlag_SB(void)
{
  return (((((PWR_TypeDef *) (0x40000000UL + 0x00007000UL))->CSR) & ((0x1UL << (1U)))) == ((0x1UL << (1U))));
}
# 503 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_pwr.h"
static __inline void LL_PWR_ClearFlag_SB(void)
{
  ((((PWR_TypeDef *) (0x40000000UL + 0x00007000UL))->CR) |= ((0x1UL << (3U))));
}






static __inline void LL_PWR_ClearFlag_WU(void)
{
  ((((PWR_TypeDef *) (0x40000000UL + 0x00007000UL))->CR) |= ((0x1UL << (2U))));
}
# 526 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_pwr.h"
ErrorStatus LL_PWR_DeInit(void);
# 39 "../Core/Inc/main.h" 2
# 1 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h" 1
# 47 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
static const uint8_t CHANNEL_OFFSET_TAB[] =
{
  (uint8_t)((((0x40000000UL + 0x00020000UL) + 0x00000000UL) + 0x00000008UL) - ((0x40000000UL + 0x00020000UL) + 0x00000000UL)),
  (uint8_t)((((0x40000000UL + 0x00020000UL) + 0x00000000UL) + 0x0000001CUL) - ((0x40000000UL + 0x00020000UL) + 0x00000000UL)),
  (uint8_t)((((0x40000000UL + 0x00020000UL) + 0x00000000UL) + 0x00000030UL) - ((0x40000000UL + 0x00020000UL) + 0x00000000UL)),
  (uint8_t)((((0x40000000UL + 0x00020000UL) + 0x00000000UL) + 0x00000044UL) - ((0x40000000UL + 0x00020000UL) + 0x00000000UL)),
  (uint8_t)((((0x40000000UL + 0x00020000UL) + 0x00000000UL) + 0x00000058UL) - ((0x40000000UL + 0x00020000UL) + 0x00000000UL)),






};
# 93 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
typedef struct
{
  uint32_t PeriphOrM2MSrcAddress;




  uint32_t MemoryOrM2MDstAddress;




  uint32_t Direction;





  uint32_t Mode;






  uint32_t PeriphOrM2MSrcIncMode;





  uint32_t MemoryOrM2MDstIncMode;





  uint32_t PeriphOrM2MSrcDataSize;





  uint32_t MemoryOrM2MDstDataSize;





  uint32_t NbData;
# 156 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
  uint32_t Priority;




} LL_DMA_InitTypeDef;
# 583 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
static __inline void LL_DMA_EnableChannel(DMA_TypeDef *DMAx, uint32_t Channel)
{
  ((((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMAx + CHANNEL_OFFSET_TAB[Channel - 1U])))->CCR) |= ((0x1UL << (0U))));
}
# 602 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
static __inline void LL_DMA_DisableChannel(DMA_TypeDef *DMAx, uint32_t Channel)
{
  ((((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMAx + CHANNEL_OFFSET_TAB[Channel - 1U])))->CCR) &= ~((0x1UL << (0U))));
}
# 621 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
static __inline uint32_t LL_DMA_IsEnabledChannel(DMA_TypeDef *DMAx, uint32_t Channel)
{
  return (((((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMAx + CHANNEL_OFFSET_TAB[Channel - 1U])))->CCR) & ((0x1UL << (0U)))) == ((0x1UL << (0U))));

}
# 656 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
static __inline void LL_DMA_ConfigTransfer(DMA_TypeDef *DMAx, uint32_t Channel, uint32_t Configuration)
{
  (((((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMAx + CHANNEL_OFFSET_TAB[Channel - 1U])))->CCR)) = ((((((((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMAx + CHANNEL_OFFSET_TAB[Channel - 1U])))->CCR))) & (~((0x1UL << (4U)) | (0x1UL << (14U)) | (0x1UL << (5U)) | (0x1UL << (6U)) | (0x1UL << (7U)) | (0x3UL << (8U)) | (0x3UL << (10U)) | (0x3UL << (12U))))) | (Configuration))));


}
# 682 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
static __inline void LL_DMA_SetDataTransferDirection(DMA_TypeDef *DMAx, uint32_t Channel, uint32_t Direction)
{
  (((((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMAx + CHANNEL_OFFSET_TAB[Channel - 1U])))->CCR)) = ((((((((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMAx + CHANNEL_OFFSET_TAB[Channel - 1U])))->CCR))) & (~((0x1UL << (4U)) | (0x1UL << (14U))))) | (Direction))));

}
# 706 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
static __inline uint32_t LL_DMA_GetDataTransferDirection(DMA_TypeDef *DMAx, uint32_t Channel)
{
  return (((((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMAx + CHANNEL_OFFSET_TAB[Channel - 1U])))->CCR) & ((0x1UL << (4U)) | (0x1UL << (14U)))));

}
# 731 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
static __inline void LL_DMA_SetMode(DMA_TypeDef *DMAx, uint32_t Channel, uint32_t Mode)
{
  (((((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMAx + CHANNEL_OFFSET_TAB[Channel - 1U])))->CCR)) = ((((((((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMAx + CHANNEL_OFFSET_TAB[Channel - 1U])))->CCR))) & (~((0x1UL << (5U))))) | (Mode))));

}
# 753 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
static __inline uint32_t LL_DMA_GetMode(DMA_TypeDef *DMAx, uint32_t Channel)
{
  return (((((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMAx + CHANNEL_OFFSET_TAB[Channel - 1U])))->CCR) & ((0x1UL << (5U)))));

}
# 776 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
static __inline void LL_DMA_SetPeriphIncMode(DMA_TypeDef *DMAx, uint32_t Channel, uint32_t PeriphOrM2MSrcIncMode)
{
  (((((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMAx + CHANNEL_OFFSET_TAB[Channel - 1U])))->CCR)) = ((((((((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMAx + CHANNEL_OFFSET_TAB[Channel - 1U])))->CCR))) & (~((0x1UL << (6U))))) | (PeriphOrM2MSrcIncMode))));

}
# 798 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
static __inline uint32_t LL_DMA_GetPeriphIncMode(DMA_TypeDef *DMAx, uint32_t Channel)
{
  return (((((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMAx + CHANNEL_OFFSET_TAB[Channel - 1U])))->CCR) & ((0x1UL << (6U)))));

}
# 821 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
static __inline void LL_DMA_SetMemoryIncMode(DMA_TypeDef *DMAx, uint32_t Channel, uint32_t MemoryOrM2MDstIncMode)
{
  (((((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMAx + CHANNEL_OFFSET_TAB[Channel - 1U])))->CCR)) = ((((((((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMAx + CHANNEL_OFFSET_TAB[Channel - 1U])))->CCR))) & (~((0x1UL << (7U))))) | (MemoryOrM2MDstIncMode))));

}
# 843 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
static __inline uint32_t LL_DMA_GetMemoryIncMode(DMA_TypeDef *DMAx, uint32_t Channel)
{
  return (((((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMAx + CHANNEL_OFFSET_TAB[Channel - 1U])))->CCR) & ((0x1UL << (7U)))));

}
# 867 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
static __inline void LL_DMA_SetPeriphSize(DMA_TypeDef *DMAx, uint32_t Channel, uint32_t PeriphOrM2MSrcDataSize)
{
  (((((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMAx + CHANNEL_OFFSET_TAB[Channel - 1U])))->CCR)) = ((((((((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMAx + CHANNEL_OFFSET_TAB[Channel - 1U])))->CCR))) & (~((0x3UL << (8U))))) | (PeriphOrM2MSrcDataSize))));

}
# 890 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
static __inline uint32_t LL_DMA_GetPeriphSize(DMA_TypeDef *DMAx, uint32_t Channel)
{
  return (((((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMAx + CHANNEL_OFFSET_TAB[Channel - 1U])))->CCR) & ((0x3UL << (8U)))));

}
# 914 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
static __inline void LL_DMA_SetMemorySize(DMA_TypeDef *DMAx, uint32_t Channel, uint32_t MemoryOrM2MDstDataSize)
{
  (((((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMAx + CHANNEL_OFFSET_TAB[Channel - 1U])))->CCR)) = ((((((((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMAx + CHANNEL_OFFSET_TAB[Channel - 1U])))->CCR))) & (~((0x3UL << (10U))))) | (MemoryOrM2MDstDataSize))));

}
# 937 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
static __inline uint32_t LL_DMA_GetMemorySize(DMA_TypeDef *DMAx, uint32_t Channel)
{
  return (((((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMAx + CHANNEL_OFFSET_TAB[Channel - 1U])))->CCR) & ((0x3UL << (10U)))));

}
# 962 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
static __inline void LL_DMA_SetChannelPriorityLevel(DMA_TypeDef *DMAx, uint32_t Channel, uint32_t Priority)
{
  (((((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMAx + CHANNEL_OFFSET_TAB[Channel - 1U])))->CCR)) = ((((((((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMAx + CHANNEL_OFFSET_TAB[Channel - 1U])))->CCR))) & (~((0x3UL << (12U))))) | (Priority))));

}
# 986 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
static __inline uint32_t LL_DMA_GetChannelPriorityLevel(DMA_TypeDef *DMAx, uint32_t Channel)
{
  return (((((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMAx + CHANNEL_OFFSET_TAB[Channel - 1U])))->CCR) & ((0x3UL << (12U)))));

}
# 1009 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
static __inline void LL_DMA_SetDataLength(DMA_TypeDef *DMAx, uint32_t Channel, uint32_t NbData)
{
  (((((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMAx + CHANNEL_OFFSET_TAB[Channel - 1U])))->CNDTR)) = ((((((((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMAx + CHANNEL_OFFSET_TAB[Channel - 1U])))->CNDTR))) & (~((0xFFFFUL << (0U))))) | (NbData))));

}
# 1031 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
static __inline uint32_t LL_DMA_GetDataLength(DMA_TypeDef *DMAx, uint32_t Channel)
{
  return (((((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMAx + CHANNEL_OFFSET_TAB[Channel - 1U])))->CNDTR) & ((0xFFFFUL << (0U)))));

}
# 1060 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
static __inline void LL_DMA_ConfigAddresses(DMA_TypeDef *DMAx, uint32_t Channel, uint32_t SrcAddress,
                                            uint32_t DstAddress, uint32_t Direction)
{

  if (Direction == (0x1UL << (4U)))
  {
    ((((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMAx + CHANNEL_OFFSET_TAB[Channel - 1U])))->CMAR) = (SrcAddress));
    ((((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMAx + CHANNEL_OFFSET_TAB[Channel - 1U])))->CPAR) = (DstAddress));
  }

  else
  {
    ((((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMAx + CHANNEL_OFFSET_TAB[Channel - 1U])))->CPAR) = (SrcAddress));
    ((((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMAx + CHANNEL_OFFSET_TAB[Channel - 1U])))->CMAR) = (DstAddress));
  }
}
# 1094 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
static __inline void LL_DMA_SetMemoryAddress(DMA_TypeDef *DMAx, uint32_t Channel, uint32_t MemoryAddress)
{
  ((((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMAx + CHANNEL_OFFSET_TAB[Channel - 1U])))->CMAR) = (MemoryAddress));
}
# 1116 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
static __inline void LL_DMA_SetPeriphAddress(DMA_TypeDef *DMAx, uint32_t Channel, uint32_t PeriphAddress)
{
  ((((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMAx + CHANNEL_OFFSET_TAB[Channel - 1U])))->CPAR) = (PeriphAddress));
}
# 1136 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
static __inline uint32_t LL_DMA_GetMemoryAddress(DMA_TypeDef *DMAx, uint32_t Channel)
{
  return (((((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMAx + CHANNEL_OFFSET_TAB[Channel - 1U])))->CMAR)));
}
# 1156 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
static __inline uint32_t LL_DMA_GetPeriphAddress(DMA_TypeDef *DMAx, uint32_t Channel)
{
  return (((((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMAx + CHANNEL_OFFSET_TAB[Channel - 1U])))->CPAR)));
}
# 1178 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
static __inline void LL_DMA_SetM2MSrcAddress(DMA_TypeDef *DMAx, uint32_t Channel, uint32_t MemoryAddress)
{
  ((((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMAx + CHANNEL_OFFSET_TAB[Channel - 1U])))->CPAR) = (MemoryAddress));
}
# 1200 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
static __inline void LL_DMA_SetM2MDstAddress(DMA_TypeDef *DMAx, uint32_t Channel, uint32_t MemoryAddress)
{
  ((((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMAx + CHANNEL_OFFSET_TAB[Channel - 1U])))->CMAR) = (MemoryAddress));
}
# 1220 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
static __inline uint32_t LL_DMA_GetM2MSrcAddress(DMA_TypeDef *DMAx, uint32_t Channel)
{
  return (((((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMAx + CHANNEL_OFFSET_TAB[Channel - 1U])))->CPAR)));
}
# 1240 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
static __inline uint32_t LL_DMA_GetM2MDstAddress(DMA_TypeDef *DMAx, uint32_t Channel)
{
  return (((((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMAx + CHANNEL_OFFSET_TAB[Channel - 1U])))->CMAR)));
}
# 1347 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
static __inline uint32_t LL_DMA_IsActiveFlag_GI1(DMA_TypeDef *DMAx)
{
  return (((DMAx->ISR) & ((0x1UL << (0U)))) == ((0x1UL << (0U))));
}







static __inline uint32_t LL_DMA_IsActiveFlag_GI2(DMA_TypeDef *DMAx)
{
  return (((DMAx->ISR) & ((0x1UL << (4U)))) == ((0x1UL << (4U))));
}







static __inline uint32_t LL_DMA_IsActiveFlag_GI3(DMA_TypeDef *DMAx)
{
  return (((DMAx->ISR) & ((0x1UL << (8U)))) == ((0x1UL << (8U))));
}







static __inline uint32_t LL_DMA_IsActiveFlag_GI4(DMA_TypeDef *DMAx)
{
  return (((DMAx->ISR) & ((0x1UL << (12U)))) == ((0x1UL << (12U))));
}







static __inline uint32_t LL_DMA_IsActiveFlag_GI5(DMA_TypeDef *DMAx)
{
  return (((DMAx->ISR) & ((0x1UL << (16U)))) == ((0x1UL << (16U))));
}
# 1428 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
static __inline uint32_t LL_DMA_IsActiveFlag_TC1(DMA_TypeDef *DMAx)
{
  return (((DMAx->ISR) & ((0x1UL << (1U)))) == ((0x1UL << (1U))));
}







static __inline uint32_t LL_DMA_IsActiveFlag_TC2(DMA_TypeDef *DMAx)
{
  return (((DMAx->ISR) & ((0x1UL << (5U)))) == ((0x1UL << (5U))));
}







static __inline uint32_t LL_DMA_IsActiveFlag_TC3(DMA_TypeDef *DMAx)
{
  return (((DMAx->ISR) & ((0x1UL << (9U)))) == ((0x1UL << (9U))));
}







static __inline uint32_t LL_DMA_IsActiveFlag_TC4(DMA_TypeDef *DMAx)
{
  return (((DMAx->ISR) & ((0x1UL << (13U)))) == ((0x1UL << (13U))));
}







static __inline uint32_t LL_DMA_IsActiveFlag_TC5(DMA_TypeDef *DMAx)
{
  return (((DMAx->ISR) & ((0x1UL << (17U)))) == ((0x1UL << (17U))));
}
# 1509 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
static __inline uint32_t LL_DMA_IsActiveFlag_HT1(DMA_TypeDef *DMAx)
{
  return (((DMAx->ISR) & ((0x1UL << (2U)))) == ((0x1UL << (2U))));
}







static __inline uint32_t LL_DMA_IsActiveFlag_HT2(DMA_TypeDef *DMAx)
{
  return (((DMAx->ISR) & ((0x1UL << (6U)))) == ((0x1UL << (6U))));
}







static __inline uint32_t LL_DMA_IsActiveFlag_HT3(DMA_TypeDef *DMAx)
{
  return (((DMAx->ISR) & ((0x1UL << (10U)))) == ((0x1UL << (10U))));
}







static __inline uint32_t LL_DMA_IsActiveFlag_HT4(DMA_TypeDef *DMAx)
{
  return (((DMAx->ISR) & ((0x1UL << (14U)))) == ((0x1UL << (14U))));
}







static __inline uint32_t LL_DMA_IsActiveFlag_HT5(DMA_TypeDef *DMAx)
{
  return (((DMAx->ISR) & ((0x1UL << (18U)))) == ((0x1UL << (18U))));
}
# 1590 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
static __inline uint32_t LL_DMA_IsActiveFlag_TE1(DMA_TypeDef *DMAx)
{
  return (((DMAx->ISR) & ((0x1UL << (3U)))) == ((0x1UL << (3U))));
}







static __inline uint32_t LL_DMA_IsActiveFlag_TE2(DMA_TypeDef *DMAx)
{
  return (((DMAx->ISR) & ((0x1UL << (7U)))) == ((0x1UL << (7U))));
}







static __inline uint32_t LL_DMA_IsActiveFlag_TE3(DMA_TypeDef *DMAx)
{
  return (((DMAx->ISR) & ((0x1UL << (11U)))) == ((0x1UL << (11U))));
}







static __inline uint32_t LL_DMA_IsActiveFlag_TE4(DMA_TypeDef *DMAx)
{
  return (((DMAx->ISR) & ((0x1UL << (15U)))) == ((0x1UL << (15U))));
}







static __inline uint32_t LL_DMA_IsActiveFlag_TE5(DMA_TypeDef *DMAx)
{
  return (((DMAx->ISR) & ((0x1UL << (19U)))) == ((0x1UL << (19U))));
}
# 1675 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
static __inline void LL_DMA_ClearFlag_GI1(DMA_TypeDef *DMAx)
{
  ((DMAx->IFCR) = ((0x1UL << (0U))));
}
# 1690 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
static __inline void LL_DMA_ClearFlag_GI2(DMA_TypeDef *DMAx)
{
  ((DMAx->IFCR) = ((0x1UL << (4U))));
}
# 1705 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
static __inline void LL_DMA_ClearFlag_GI3(DMA_TypeDef *DMAx)
{
  ((DMAx->IFCR) = ((0x1UL << (8U))));
}
# 1720 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
static __inline void LL_DMA_ClearFlag_GI4(DMA_TypeDef *DMAx)
{
  ((DMAx->IFCR) = ((0x1UL << (12U))));
}
# 1735 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
static __inline void LL_DMA_ClearFlag_GI5(DMA_TypeDef *DMAx)
{
  ((DMAx->IFCR) = ((0x1UL << (16U))));
}
# 1780 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
static __inline void LL_DMA_ClearFlag_TC1(DMA_TypeDef *DMAx)
{
  ((DMAx->IFCR) = ((0x1UL << (1U))));
}







static __inline void LL_DMA_ClearFlag_TC2(DMA_TypeDef *DMAx)
{
  ((DMAx->IFCR) = ((0x1UL << (5U))));
}







static __inline void LL_DMA_ClearFlag_TC3(DMA_TypeDef *DMAx)
{
  ((DMAx->IFCR) = ((0x1UL << (9U))));
}







static __inline void LL_DMA_ClearFlag_TC4(DMA_TypeDef *DMAx)
{
  ((DMAx->IFCR) = ((0x1UL << (13U))));
}







static __inline void LL_DMA_ClearFlag_TC5(DMA_TypeDef *DMAx)
{
  ((DMAx->IFCR) = ((0x1UL << (17U))));
}
# 1861 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
static __inline void LL_DMA_ClearFlag_HT1(DMA_TypeDef *DMAx)
{
  ((DMAx->IFCR) = ((0x1UL << (2U))));
}







static __inline void LL_DMA_ClearFlag_HT2(DMA_TypeDef *DMAx)
{
  ((DMAx->IFCR) = ((0x1UL << (6U))));
}







static __inline void LL_DMA_ClearFlag_HT3(DMA_TypeDef *DMAx)
{
  ((DMAx->IFCR) = ((0x1UL << (10U))));
}







static __inline void LL_DMA_ClearFlag_HT4(DMA_TypeDef *DMAx)
{
  ((DMAx->IFCR) = ((0x1UL << (14U))));
}







static __inline void LL_DMA_ClearFlag_HT5(DMA_TypeDef *DMAx)
{
  ((DMAx->IFCR) = ((0x1UL << (18U))));
}
# 1942 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
static __inline void LL_DMA_ClearFlag_TE1(DMA_TypeDef *DMAx)
{
  ((DMAx->IFCR) = ((0x1UL << (3U))));
}







static __inline void LL_DMA_ClearFlag_TE2(DMA_TypeDef *DMAx)
{
  ((DMAx->IFCR) = ((0x1UL << (7U))));
}







static __inline void LL_DMA_ClearFlag_TE3(DMA_TypeDef *DMAx)
{
  ((DMAx->IFCR) = ((0x1UL << (11U))));
}







static __inline void LL_DMA_ClearFlag_TE4(DMA_TypeDef *DMAx)
{
  ((DMAx->IFCR) = ((0x1UL << (15U))));
}







static __inline void LL_DMA_ClearFlag_TE5(DMA_TypeDef *DMAx)
{
  ((DMAx->IFCR) = ((0x1UL << (19U))));
}
# 2038 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
static __inline void LL_DMA_EnableIT_TC(DMA_TypeDef *DMAx, uint32_t Channel)
{
  ((((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMAx + CHANNEL_OFFSET_TAB[Channel - 1U])))->CCR) |= ((0x1UL << (1U))));
}
# 2057 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
static __inline void LL_DMA_EnableIT_HT(DMA_TypeDef *DMAx, uint32_t Channel)
{
  ((((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMAx + CHANNEL_OFFSET_TAB[Channel - 1U])))->CCR) |= ((0x1UL << (2U))));
}
# 2076 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
static __inline void LL_DMA_EnableIT_TE(DMA_TypeDef *DMAx, uint32_t Channel)
{
  ((((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMAx + CHANNEL_OFFSET_TAB[Channel - 1U])))->CCR) |= ((0x1UL << (3U))));
}
# 2095 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
static __inline void LL_DMA_DisableIT_TC(DMA_TypeDef *DMAx, uint32_t Channel)
{
  ((((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMAx + CHANNEL_OFFSET_TAB[Channel - 1U])))->CCR) &= ~((0x1UL << (1U))));
}
# 2114 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
static __inline void LL_DMA_DisableIT_HT(DMA_TypeDef *DMAx, uint32_t Channel)
{
  ((((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMAx + CHANNEL_OFFSET_TAB[Channel - 1U])))->CCR) &= ~((0x1UL << (2U))));
}
# 2133 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
static __inline void LL_DMA_DisableIT_TE(DMA_TypeDef *DMAx, uint32_t Channel)
{
  ((((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMAx + CHANNEL_OFFSET_TAB[Channel - 1U])))->CCR) &= ~((0x1UL << (3U))));
}
# 2152 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
static __inline uint32_t LL_DMA_IsEnabledIT_TC(DMA_TypeDef *DMAx, uint32_t Channel)
{
  return (((((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMAx + CHANNEL_OFFSET_TAB[Channel - 1U])))->CCR) & ((0x1UL << (1U)))) == ((0x1UL << (1U))));

}
# 2172 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
static __inline uint32_t LL_DMA_IsEnabledIT_HT(DMA_TypeDef *DMAx, uint32_t Channel)
{
  return (((((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMAx + CHANNEL_OFFSET_TAB[Channel - 1U])))->CCR) & ((0x1UL << (2U)))) == ((0x1UL << (2U))));

}
# 2192 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
static __inline uint32_t LL_DMA_IsEnabledIT_TE(DMA_TypeDef *DMAx, uint32_t Channel)
{
  return (((((DMA_Channel_TypeDef *)((uint32_t)((uint32_t)DMAx + CHANNEL_OFFSET_TAB[Channel - 1U])))->CCR) & ((0x1UL << (3U)))) == ((0x1UL << (3U))));

}
# 2207 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
uint32_t LL_DMA_Init(DMA_TypeDef *DMAx, uint32_t Channel, LL_DMA_InitTypeDef *DMA_InitStruct);
uint32_t LL_DMA_DeInit(DMA_TypeDef *DMAx, uint32_t Channel);
void LL_DMA_StructInit(LL_DMA_InitTypeDef *DMA_InitStruct);
# 40 "../Core/Inc/main.h" 2
# 1 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h" 1
# 64 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
typedef struct
{

  uint32_t BaudRate;




  uint32_t DataWidth;





  uint32_t StopBits;





  uint32_t Parity;





  uint32_t TransferDirection;





  uint32_t HardwareFlowControl;





  uint32_t OverSampling;





} LL_USART_InitTypeDef;




typedef struct
{
  uint32_t ClockOutput;






  uint32_t ClockPolarity;






  uint32_t ClockPhase;






  uint32_t LastBitClockPulse;







} LL_USART_ClockInitTypeDef;
# 562 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_Enable(USART_TypeDef *USARTx)
{
  ((USARTx->CR1) |= ((0x1UL << (0U))));
}
# 576 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_Disable(USART_TypeDef *USARTx)
{
  ((USARTx->CR1) &= ~((0x1UL << (0U))));
}







static __inline uint32_t LL_USART_IsEnabled(USART_TypeDef *USARTx)
{
  return ((((USARTx->CR1) & ((0x1UL << (0U)))) == ((0x1UL << (0U)))) ? 1UL : 0UL);
}
# 642 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_EnableDirectionRx(USART_TypeDef *USARTx)
{
  ((USARTx->CR1) |= ((0x1UL << (2U))));
}







static __inline void LL_USART_DisableDirectionRx(USART_TypeDef *USARTx)
{
  ((USARTx->CR1) &= ~((0x1UL << (2U))));
}







static __inline void LL_USART_EnableDirectionTx(USART_TypeDef *USARTx)
{
  ((USARTx->CR1) |= ((0x1UL << (3U))));
}







static __inline void LL_USART_DisableDirectionTx(USART_TypeDef *USARTx)
{
  ((USARTx->CR1) &= ~((0x1UL << (3U))));
}
# 693 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_SetTransferDirection(USART_TypeDef *USARTx, uint32_t TransferDirection)
{
  (((USARTx->CR1)) = ((((((USARTx->CR1))) & (~((0x1UL << (2U)) | (0x1UL << (3U))))) | (TransferDirection))));
}
# 709 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline uint32_t LL_USART_GetTransferDirection(USART_TypeDef *USARTx)
{
  return (uint32_t)(((USARTx->CR1) & ((0x1UL << (2U)) | (0x1UL << (3U)))));
}
# 728 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_SetParity(USART_TypeDef *USARTx, uint32_t Parity)
{
  (((USARTx->CR1)) = ((((((USARTx->CR1))) & (~((0x1UL << (9U)) | (0x1UL << (10U))))) | (Parity))));
}
# 743 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline uint32_t LL_USART_GetParity(USART_TypeDef *USARTx)
{
  return (uint32_t)(((USARTx->CR1) & ((0x1UL << (9U)) | (0x1UL << (10U)))));
}
# 757 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_SetWakeUpMethod(USART_TypeDef *USARTx, uint32_t Method)
{
  (((USARTx->CR1)) = ((((((USARTx->CR1))) & (~((0x1UL << (11U))))) | (Method))));
}
# 770 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline uint32_t LL_USART_GetWakeUpMethod(USART_TypeDef *USARTx)
{
  return (uint32_t)(((USARTx->CR1) & ((0x1UL << (11U)))));
}
# 788 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_SetDataWidth(USART_TypeDef *USARTx, uint32_t DataWidth)
{
  (((USARTx->CR1)) = ((((((USARTx->CR1))) & (~((0x1UL << (12U))))) | (DataWidth))));
}
# 805 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline uint32_t LL_USART_GetDataWidth(USART_TypeDef *USARTx)
{
  return (uint32_t)(((USARTx->CR1) & ((0x1UL << (12U)))));
}







static __inline void LL_USART_EnableMuteMode(USART_TypeDef *USARTx)
{
  ((USARTx->CR1) |= ((0x1UL << (13U))));
}







static __inline void LL_USART_DisableMuteMode(USART_TypeDef *USARTx)
{
  ((USARTx->CR1) &= ~((0x1UL << (13U))));
}







static __inline uint32_t LL_USART_IsEnabledMuteMode(USART_TypeDef *USARTx)
{
  return ((((USARTx->CR1) & ((0x1UL << (13U)))) == ((0x1UL << (13U)))) ? 1UL : 0UL);
}
# 852 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_SetOverSampling(USART_TypeDef *USARTx, uint32_t OverSampling)
{
  (((USARTx->CR1)) = ((((((USARTx->CR1))) & (~((0x1UL << (15U))))) | (OverSampling))));
}
# 865 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline uint32_t LL_USART_GetOverSampling(USART_TypeDef *USARTx)
{
  return (uint32_t)(((USARTx->CR1) & ((0x1UL << (15U)))));
}
# 881 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_SetLastClkPulseOutput(USART_TypeDef *USARTx, uint32_t LastBitClockPulse)
{
  (((USARTx->CR2)) = ((((((USARTx->CR2))) & (~((0x1UL << (8U))))) | (LastBitClockPulse))));
}
# 897 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline uint32_t LL_USART_GetLastClkPulseOutput(USART_TypeDef *USARTx)
{
  return (uint32_t)(((USARTx->CR2) & ((0x1UL << (8U)))));
}
# 913 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_SetClockPhase(USART_TypeDef *USARTx, uint32_t ClockPhase)
{
  (((USARTx->CR2)) = ((((((USARTx->CR2))) & (~((0x1UL << (9U))))) | (ClockPhase))));
}
# 928 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline uint32_t LL_USART_GetClockPhase(USART_TypeDef *USARTx)
{
  return (uint32_t)(((USARTx->CR2) & ((0x1UL << (9U)))));
}
# 944 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_SetClockPolarity(USART_TypeDef *USARTx, uint32_t ClockPolarity)
{
  (((USARTx->CR2)) = ((((((USARTx->CR2))) & (~((0x1UL << (10U))))) | (ClockPolarity))));
}
# 959 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline uint32_t LL_USART_GetClockPolarity(USART_TypeDef *USARTx)
{
  return (uint32_t)(((USARTx->CR2) & ((0x1UL << (10U)))));
}
# 987 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_ConfigClock(USART_TypeDef *USARTx, uint32_t Phase, uint32_t Polarity, uint32_t LBCPOutput)
{
  (((USARTx->CR2)) = ((((((USARTx->CR2))) & (~((0x1UL << (9U)) | (0x1UL << (10U)) | (0x1UL << (8U))))) | (Phase | Polarity | LBCPOutput))));
}
# 1000 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_EnableSCLKOutput(USART_TypeDef *USARTx)
{
  ((USARTx->CR2) |= ((0x1UL << (11U))));
}
# 1013 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_DisableSCLKOutput(USART_TypeDef *USARTx)
{
  ((USARTx->CR2) &= ~((0x1UL << (11U))));
}
# 1026 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline uint32_t LL_USART_IsEnabledSCLKOutput(USART_TypeDef *USARTx)
{
  return ((((USARTx->CR2) & ((0x1UL << (11U)))) == ((0x1UL << (11U)))) ? 1UL : 0UL);
}
# 1044 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_SetStopBitsLength(USART_TypeDef *USARTx, uint32_t StopBits)
{
  (((USARTx->CR2)) = ((((((USARTx->CR2))) & (~((0x3UL << (12U))))) | (StopBits))));
}
# 1061 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline uint32_t LL_USART_GetStopBitsLength(USART_TypeDef *USARTx)
{
  return (uint32_t)(((USARTx->CR2) & ((0x3UL << (12U)))));
}
# 1095 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_ConfigCharacter(USART_TypeDef *USARTx, uint32_t DataWidth, uint32_t Parity,
                                              uint32_t StopBits)
{
  (((USARTx->CR1)) = ((((((USARTx->CR1))) & (~((0x1UL << (9U)) | (0x1UL << (10U)) | (0x1UL << (12U))))) | (Parity | DataWidth))));
  (((USARTx->CR2)) = ((((((USARTx->CR2))) & (~((0x3UL << (12U))))) | (StopBits))));
}
# 1111 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_SetTXRXSwap(USART_TypeDef *USARTx, uint32_t SwapConfig)
{
  (((USARTx->CR2)) = ((((((USARTx->CR2))) & (~((0x1UL << (15U))))) | (SwapConfig))));
}
# 1124 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline uint32_t LL_USART_GetTXRXSwap(USART_TypeDef *USARTx)
{
  return (uint32_t)(((USARTx->CR2) & ((0x1UL << (15U)))));
}
# 1138 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_SetRXPinLevel(USART_TypeDef *USARTx, uint32_t PinInvMethod)
{
  (((USARTx->CR2)) = ((((((USARTx->CR2))) & (~((0x1UL << (16U))))) | (PinInvMethod))));
}
# 1151 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline uint32_t LL_USART_GetRXPinLevel(USART_TypeDef *USARTx)
{
  return (uint32_t)(((USARTx->CR2) & ((0x1UL << (16U)))));
}
# 1165 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_SetTXPinLevel(USART_TypeDef *USARTx, uint32_t PinInvMethod)
{
  (((USARTx->CR2)) = ((((((USARTx->CR2))) & (~((0x1UL << (17U))))) | (PinInvMethod))));
}
# 1178 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline uint32_t LL_USART_GetTXPinLevel(USART_TypeDef *USARTx)
{
  return (uint32_t)(((USARTx->CR2) & ((0x1UL << (17U)))));
}
# 1194 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_SetBinaryDataLogic(USART_TypeDef *USARTx, uint32_t DataLogic)
{
  (((USARTx->CR2)) = ((((((USARTx->CR2))) & (~((0x1UL << (18U))))) | (DataLogic))));
}
# 1207 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline uint32_t LL_USART_GetBinaryDataLogic(USART_TypeDef *USARTx)
{
  return (uint32_t)(((USARTx->CR2) & ((0x1UL << (18U)))));
}
# 1223 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_SetTransferBitOrder(USART_TypeDef *USARTx, uint32_t BitOrder)
{
  (((USARTx->CR2)) = ((((((USARTx->CR2))) & (~((0x1UL << (19U))))) | (BitOrder))));
}
# 1238 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline uint32_t LL_USART_GetTransferBitOrder(USART_TypeDef *USARTx)
{
  return (uint32_t)(((USARTx->CR2) & ((0x1UL << (19U)))));
}
# 1251 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_EnableAutoBaudRate(USART_TypeDef *USARTx)
{
  ((USARTx->CR2) |= ((0x1UL << (20U))));
}
# 1264 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_DisableAutoBaudRate(USART_TypeDef *USARTx)
{
  ((USARTx->CR2) &= ~((0x1UL << (20U))));
}
# 1277 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline uint32_t LL_USART_IsEnabledAutoBaud(USART_TypeDef *USARTx)
{
  return ((((USARTx->CR2) & ((0x1UL << (20U)))) == ((0x1UL << (20U)))) ? 1UL : 0UL);
}
# 1297 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_SetAutoBaudRateMode(USART_TypeDef *USARTx, uint32_t AutoBaudRateMode)
{
  (((USARTx->CR2)) = ((((((USARTx->CR2))) & (~((0x3UL << (21U))))) | (AutoBaudRateMode))));
}
# 1316 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline uint32_t LL_USART_GetAutoBaudRateMode(USART_TypeDef *USARTx)
{
  return (uint32_t)(((USARTx->CR2) & ((0x3UL << (21U)))));
}







static __inline void LL_USART_EnableRxTimeout(USART_TypeDef *USARTx)
{
  ((USARTx->CR2) |= ((0x1UL << (23U))));
}







static __inline void LL_USART_DisableRxTimeout(USART_TypeDef *USARTx)
{
  ((USARTx->CR2) &= ~((0x1UL << (23U))));
}







static __inline uint32_t LL_USART_IsEnabledRxTimeout(USART_TypeDef *USARTx)
{
  return ((((USARTx->CR2) & ((0x1UL << (23U)))) == ((0x1UL << (23U)))) ? 1UL : 0UL);
}
# 1377 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_ConfigNodeAddress(USART_TypeDef *USARTx, uint32_t AddressLen, uint32_t NodeAddress)
{
  (((USARTx->CR2)) = ((((((USARTx->CR2))) & (~((0xFFUL << (24U)) | (0x1UL << (4U))))) | ((uint32_t)(AddressLen | (NodeAddress << (24U)))))));

}
# 1393 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline uint32_t LL_USART_GetNodeAddress(USART_TypeDef *USARTx)
{
  return (uint32_t)(((USARTx->CR2) & ((0xFFUL << (24U)))) >> (24U));
}
# 1406 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline uint32_t LL_USART_GetNodeAddressLen(USART_TypeDef *USARTx)
{
  return (uint32_t)(((USARTx->CR2) & ((0x1UL << (4U)))));
}
# 1419 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_EnableRTSHWFlowCtrl(USART_TypeDef *USARTx)
{
  ((USARTx->CR3) |= ((0x1UL << (8U))));
}
# 1432 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_DisableRTSHWFlowCtrl(USART_TypeDef *USARTx)
{
  ((USARTx->CR3) &= ~((0x1UL << (8U))));
}
# 1445 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_EnableCTSHWFlowCtrl(USART_TypeDef *USARTx)
{
  ((USARTx->CR3) |= ((0x1UL << (9U))));
}
# 1458 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_DisableCTSHWFlowCtrl(USART_TypeDef *USARTx)
{
  ((USARTx->CR3) &= ~((0x1UL << (9U))));
}
# 1477 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_SetHWFlowCtrl(USART_TypeDef *USARTx, uint32_t HardwareFlowControl)
{
  (((USARTx->CR3)) = ((((((USARTx->CR3))) & (~((0x1UL << (8U)) | (0x1UL << (9U))))) | (HardwareFlowControl))));
}
# 1495 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline uint32_t LL_USART_GetHWFlowCtrl(USART_TypeDef *USARTx)
{
  return (uint32_t)(((USARTx->CR3) & ((0x1UL << (8U)) | (0x1UL << (9U)))));
}







static __inline void LL_USART_EnableOneBitSamp(USART_TypeDef *USARTx)
{
  ((USARTx->CR3) |= ((0x1UL << (11U))));
}







static __inline void LL_USART_DisableOneBitSamp(USART_TypeDef *USARTx)
{
  ((USARTx->CR3) &= ~((0x1UL << (11U))));
}







static __inline uint32_t LL_USART_IsEnabledOneBitSamp(USART_TypeDef *USARTx)
{
  return ((((USARTx->CR3) & ((0x1UL << (11U)))) == ((0x1UL << (11U)))) ? 1UL : 0UL);
}







static __inline void LL_USART_EnableOverrunDetect(USART_TypeDef *USARTx)
{
  ((USARTx->CR3) &= ~((0x1UL << (12U))));
}







static __inline void LL_USART_DisableOverrunDetect(USART_TypeDef *USARTx)
{
  ((USARTx->CR3) |= ((0x1UL << (12U))));
}







static __inline uint32_t LL_USART_IsEnabledOverrunDetect(USART_TypeDef *USARTx)
{
  return ((((USARTx->CR3) & ((0x1UL << (12U)))) != (0x1UL << (12U))) ? 1UL : 0UL);
}
# 1617 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_SetBaudRate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t OverSampling,
                                          uint32_t BaudRate)
{
  uint32_t usartdiv;
  uint32_t brrtemp;

  if (OverSampling == (0x1UL << (15U)))
  {
    usartdiv = (uint16_t)(((((PeriphClk)*2U) + ((BaudRate)/2U))/(BaudRate)));
    brrtemp = usartdiv & 0xFFF0U;
    brrtemp |= (uint16_t)((usartdiv & (uint16_t)0x000FU) >> 1U);
    USARTx->BRR = brrtemp;
  }
  else
  {
    USARTx->BRR = (uint16_t)((((PeriphClk) + ((BaudRate)/2U))/(BaudRate)));
  }
}
# 1649 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline uint32_t LL_USART_GetBaudRate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t OverSampling)
{
  uint32_t usartdiv;
  uint32_t brrresult = 0x0U;

  usartdiv = USARTx->BRR;

  if (usartdiv == 0U)
  {

  }
  else if (OverSampling == (0x1UL << (15U)))
  {
    usartdiv = (uint16_t)((usartdiv & 0xFFF0U) | ((usartdiv & 0x0007U) << 1U)) ;
    if (usartdiv != 0U)
    {
      brrresult = (PeriphClk * 2U) / usartdiv;
    }
  }
  else
  {
    if ((usartdiv & 0xFFFFU) != 0U)
    {
      brrresult = PeriphClk / usartdiv;
    }
  }
  return (brrresult);
}
# 1685 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_SetRxTimeout(USART_TypeDef *USARTx, uint32_t Timeout)
{
  (((USARTx->RTOR)) = ((((((USARTx->RTOR))) & (~((0xFFFFFFUL << (0U))))) | (Timeout))));
}







static __inline uint32_t LL_USART_GetRxTimeout(USART_TypeDef *USARTx)
{
  return (uint32_t)(((USARTx->RTOR) & ((0xFFFFFFUL << (0U)))));
}
# 2029 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_EnableHalfDuplex(USART_TypeDef *USARTx)
{
  ((USARTx->CR3) |= ((0x1UL << (3U))));
}
# 2042 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_DisableHalfDuplex(USART_TypeDef *USARTx)
{
  ((USARTx->CR3) &= ~((0x1UL << (3U))));
}
# 2055 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline uint32_t LL_USART_IsEnabledHalfDuplex(USART_TypeDef *USARTx)
{
  return ((((USARTx->CR3) & ((0x1UL << (3U)))) == ((0x1UL << (3U)))) ? 1UL : 0UL);
}
# 2157 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_SetDEDeassertionTime(USART_TypeDef *USARTx, uint32_t Time)
{
  (((USARTx->CR1)) = ((((((USARTx->CR1))) & (~((0x1FUL << (16U))))) | (Time << (16U)))));
}
# 2170 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline uint32_t LL_USART_GetDEDeassertionTime(USART_TypeDef *USARTx)
{
  return (uint32_t)(((USARTx->CR1) & ((0x1FUL << (16U)))) >> (16U));
}
# 2184 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_SetDEAssertionTime(USART_TypeDef *USARTx, uint32_t Time)
{
  (((USARTx->CR1)) = ((((((USARTx->CR1))) & (~((0x1FUL << (21U))))) | (Time << (21U)))));
}
# 2197 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline uint32_t LL_USART_GetDEAssertionTime(USART_TypeDef *USARTx)
{
  return (uint32_t)(((USARTx->CR1) & ((0x1FUL << (21U)))) >> (21U));
}
# 2210 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_EnableDEMode(USART_TypeDef *USARTx)
{
  ((USARTx->CR3) |= ((0x1UL << (14U))));
}
# 2223 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_DisableDEMode(USART_TypeDef *USARTx)
{
  ((USARTx->CR3) &= ~((0x1UL << (14U))));
}
# 2236 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline uint32_t LL_USART_IsEnabledDEMode(USART_TypeDef *USARTx)
{
  return ((((USARTx->CR3) & ((0x1UL << (14U)))) == ((0x1UL << (14U)))) ? 1UL : 0UL);
}
# 2252 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_SetDESignalPolarity(USART_TypeDef *USARTx, uint32_t Polarity)
{
  (((USARTx->CR3)) = ((((((USARTx->CR3))) & (~((0x1UL << (15U))))) | (Polarity))));
}
# 2267 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline uint32_t LL_USART_GetDESignalPolarity(USART_TypeDef *USARTx)
{
  return (uint32_t)(((USARTx->CR3) & ((0x1UL << (15U)))));
}
# 2305 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_ConfigAsyncMode(USART_TypeDef *USARTx)
{







  ((USARTx->CR2) &= ~((0x1UL << (11U))));
# 2326 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
  ((USARTx->CR3) &= ~((0x1UL << (3U))));


}
# 2358 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_ConfigSyncMode(USART_TypeDef *USARTx)
{
# 2377 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
  ((USARTx->CR3) &= ~((0x1UL << (3U))));



  ((USARTx->CR2) |= ((0x1UL << (11U))));
}
# 2466 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_ConfigHalfDuplexMode(USART_TypeDef *USARTx)
{







  ((USARTx->CR2) &= ~((0x1UL << (11U))));
# 2489 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
  ((USARTx->CR3) |= ((0x1UL << (3U))));
}
# 2622 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_ConfigMultiProcessMode(USART_TypeDef *USARTx)
{







  ((USARTx->CR2) &= ~((0x1UL << (11U))));
# 2643 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
  ((USARTx->CR3) &= ~(((0x1UL << (3U)))));


}
# 2662 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline uint32_t LL_USART_IsActiveFlag_PE(USART_TypeDef *USARTx)
{
  return ((((USARTx->ISR) & ((0x1UL << (0U)))) == ((0x1UL << (0U)))) ? 1UL : 0UL);
}







static __inline uint32_t LL_USART_IsActiveFlag_FE(USART_TypeDef *USARTx)
{
  return ((((USARTx->ISR) & ((0x1UL << (1U)))) == ((0x1UL << (1U)))) ? 1UL : 0UL);
}







static __inline uint32_t LL_USART_IsActiveFlag_NE(USART_TypeDef *USARTx)
{
  return ((((USARTx->ISR) & ((0x1UL << (2U)))) == ((0x1UL << (2U)))) ? 1UL : 0UL);
}







static __inline uint32_t LL_USART_IsActiveFlag_ORE(USART_TypeDef *USARTx)
{
  return ((((USARTx->ISR) & ((0x1UL << (3U)))) == ((0x1UL << (3U)))) ? 1UL : 0UL);
}







static __inline uint32_t LL_USART_IsActiveFlag_IDLE(USART_TypeDef *USARTx)
{
  return ((((USARTx->ISR) & ((0x1UL << (4U)))) == ((0x1UL << (4U)))) ? 1UL : 0UL);
}







static __inline uint32_t LL_USART_IsActiveFlag_RXNE(USART_TypeDef *USARTx)
{
  return ((((USARTx->ISR) & ((0x1UL << (5U)))) == ((0x1UL << (5U)))) ? 1UL : 0UL);
}







static __inline uint32_t LL_USART_IsActiveFlag_TC(USART_TypeDef *USARTx)
{
  return ((((USARTx->ISR) & ((0x1UL << (6U)))) == ((0x1UL << (6U)))) ? 1UL : 0UL);
}







static __inline uint32_t LL_USART_IsActiveFlag_TXE(USART_TypeDef *USARTx)
{
  return ((((USARTx->ISR) & ((0x1UL << (7U)))) == ((0x1UL << (7U)))) ? 1UL : 0UL);
}
# 2767 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline uint32_t LL_USART_IsActiveFlag_nCTS(USART_TypeDef *USARTx)
{
  return ((((USARTx->ISR) & ((0x1UL << (9U)))) == ((0x1UL << (9U)))) ? 1UL : 0UL);
}
# 2780 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline uint32_t LL_USART_IsActiveFlag_CTS(USART_TypeDef *USARTx)
{
  return ((((USARTx->ISR) & ((0x1UL << (10U)))) == ((0x1UL << (10U)))) ? 1UL : 0UL);
}







static __inline uint32_t LL_USART_IsActiveFlag_RTO(USART_TypeDef *USARTx)
{
  return ((((USARTx->ISR) & ((0x1UL << (11U)))) == ((0x1UL << (11U)))) ? 1UL : 0UL);
}
# 2819 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline uint32_t LL_USART_IsActiveFlag_ABRE(USART_TypeDef *USARTx)
{
  return ((((USARTx->ISR) & ((0x1UL << (14U)))) == ((0x1UL << (14U)))) ? 1UL : 0UL);
}
# 2832 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline uint32_t LL_USART_IsActiveFlag_ABR(USART_TypeDef *USARTx)
{
  return ((((USARTx->ISR) & ((0x1UL << (15U)))) == ((0x1UL << (15U)))) ? 1UL : 0UL);
}







static __inline uint32_t LL_USART_IsActiveFlag_BUSY(USART_TypeDef *USARTx)
{
  return ((((USARTx->ISR) & ((0x1UL << (16U)))) == ((0x1UL << (16U)))) ? 1UL : 0UL);
}







static __inline uint32_t LL_USART_IsActiveFlag_CM(USART_TypeDef *USARTx)
{
  return ((((USARTx->ISR) & ((0x1UL << (17U)))) == ((0x1UL << (17U)))) ? 1UL : 0UL);
}







static __inline uint32_t LL_USART_IsActiveFlag_SBK(USART_TypeDef *USARTx)
{
  return ((((USARTx->ISR) & ((0x1UL << (18U)))) == ((0x1UL << (18U)))) ? 1UL : 0UL);
}







static __inline uint32_t LL_USART_IsActiveFlag_RWU(USART_TypeDef *USARTx)
{
  return ((((USARTx->ISR) & ((0x1UL << (19U)))) == ((0x1UL << (19U)))) ? 1UL : 0UL);
}
# 2902 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline uint32_t LL_USART_IsActiveFlag_TEACK(USART_TypeDef *USARTx)
{
  return ((((USARTx->ISR) & ((0x1UL << (21U)))) == ((0x1UL << (21U)))) ? 1UL : 0UL);
}







static __inline uint32_t LL_USART_IsActiveFlag_REACK(USART_TypeDef *USARTx)
{
  return ((((USARTx->ISR) & ((0x1UL << (22U)))) == ((0x1UL << (22U)))) ? 1UL : 0UL);
}







static __inline void LL_USART_ClearFlag_PE(USART_TypeDef *USARTx)
{
  ((USARTx->ICR) = ((0x1UL << (0U))));
}







static __inline void LL_USART_ClearFlag_FE(USART_TypeDef *USARTx)
{
  ((USARTx->ICR) = ((0x1UL << (1U))));
}







static __inline void LL_USART_ClearFlag_NE(USART_TypeDef *USARTx)
{
  ((USARTx->ICR) = ((0x1UL << (2U))));
}







static __inline void LL_USART_ClearFlag_ORE(USART_TypeDef *USARTx)
{
  ((USARTx->ICR) = ((0x1UL << (3U))));
}







static __inline void LL_USART_ClearFlag_IDLE(USART_TypeDef *USARTx)
{
  ((USARTx->ICR) = ((0x1UL << (4U))));
}







static __inline void LL_USART_ClearFlag_TC(USART_TypeDef *USARTx)
{
  ((USARTx->ICR) = ((0x1UL << (6U))));
}
# 3008 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_ClearFlag_nCTS(USART_TypeDef *USARTx)
{
  ((USARTx->ICR) = ((0x1UL << (9U))));
}







static __inline void LL_USART_ClearFlag_RTO(USART_TypeDef *USARTx)
{
  ((USARTx->ICR) = ((0x1UL << (11U))));
}
# 3045 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_ClearFlag_CM(USART_TypeDef *USARTx)
{
  ((USARTx->ICR) = ((0x1UL << (17U))));
}
# 3079 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_EnableIT_IDLE(USART_TypeDef *USARTx)
{
  ((USARTx->CR1) |= ((0x1UL << (4U))));
}







static __inline void LL_USART_EnableIT_RXNE(USART_TypeDef *USARTx)
{
  ((USARTx->CR1) |= ((0x1UL << (5U))));
}







static __inline void LL_USART_EnableIT_TC(USART_TypeDef *USARTx)
{
  ((USARTx->CR1) |= ((0x1UL << (6U))));
}







static __inline void LL_USART_EnableIT_TXE(USART_TypeDef *USARTx)
{
  ((USARTx->CR1) |= ((0x1UL << (7U))));
}







static __inline void LL_USART_EnableIT_PE(USART_TypeDef *USARTx)
{
  ((USARTx->CR1) |= ((0x1UL << (8U))));
}







static __inline void LL_USART_EnableIT_CM(USART_TypeDef *USARTx)
{
  ((USARTx->CR1) |= ((0x1UL << (14U))));
}







static __inline void LL_USART_EnableIT_RTO(USART_TypeDef *USARTx)
{
  ((USARTx->CR1) |= ((0x1UL << (26U))));
}
# 3190 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_EnableIT_ERROR(USART_TypeDef *USARTx)
{
  ((USARTx->CR3) |= ((0x1UL << (0U))));
}
# 3203 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_EnableIT_CTS(USART_TypeDef *USARTx)
{
  ((USARTx->CR3) |= ((0x1UL << (10U))));
}
# 3230 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_DisableIT_IDLE(USART_TypeDef *USARTx)
{
  ((USARTx->CR1) &= ~((0x1UL << (4U))));
}







static __inline void LL_USART_DisableIT_RXNE(USART_TypeDef *USARTx)
{
  ((USARTx->CR1) &= ~((0x1UL << (5U))));
}







static __inline void LL_USART_DisableIT_TC(USART_TypeDef *USARTx)
{
  ((USARTx->CR1) &= ~((0x1UL << (6U))));
}







static __inline void LL_USART_DisableIT_TXE(USART_TypeDef *USARTx)
{
  ((USARTx->CR1) &= ~((0x1UL << (7U))));
}







static __inline void LL_USART_DisableIT_PE(USART_TypeDef *USARTx)
{
  ((USARTx->CR1) &= ~((0x1UL << (8U))));
}







static __inline void LL_USART_DisableIT_CM(USART_TypeDef *USARTx)
{
  ((USARTx->CR1) &= ~((0x1UL << (14U))));
}







static __inline void LL_USART_DisableIT_RTO(USART_TypeDef *USARTx)
{
  ((USARTx->CR1) &= ~((0x1UL << (26U))));
}
# 3341 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_DisableIT_ERROR(USART_TypeDef *USARTx)
{
  ((USARTx->CR3) &= ~((0x1UL << (0U))));
}
# 3354 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_DisableIT_CTS(USART_TypeDef *USARTx)
{
  ((USARTx->CR3) &= ~((0x1UL << (10U))));
}
# 3381 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline uint32_t LL_USART_IsEnabledIT_IDLE(USART_TypeDef *USARTx)
{
  return ((((USARTx->CR1) & ((0x1UL << (4U)))) == ((0x1UL << (4U)))) ? 1UL : 0UL);
}







static __inline uint32_t LL_USART_IsEnabledIT_RXNE(USART_TypeDef *USARTx)
{
  return ((((USARTx->CR1) & ((0x1UL << (5U)))) == ((0x1UL << (5U)))) ? 1U : 0U);
}







static __inline uint32_t LL_USART_IsEnabledIT_TC(USART_TypeDef *USARTx)
{
  return ((((USARTx->CR1) & ((0x1UL << (6U)))) == ((0x1UL << (6U)))) ? 1UL : 0UL);
}







static __inline uint32_t LL_USART_IsEnabledIT_TXE(USART_TypeDef *USARTx)
{
  return ((((USARTx->CR1) & ((0x1UL << (7U)))) == ((0x1UL << (7U)))) ? 1U : 0U);
}







static __inline uint32_t LL_USART_IsEnabledIT_PE(USART_TypeDef *USARTx)
{
  return ((((USARTx->CR1) & ((0x1UL << (8U)))) == ((0x1UL << (8U)))) ? 1UL : 0UL);
}







static __inline uint32_t LL_USART_IsEnabledIT_CM(USART_TypeDef *USARTx)
{
  return ((((USARTx->CR1) & ((0x1UL << (14U)))) == ((0x1UL << (14U)))) ? 1UL : 0UL);
}







static __inline uint32_t LL_USART_IsEnabledIT_RTO(USART_TypeDef *USARTx)
{
  return ((((USARTx->CR1) & ((0x1UL << (26U)))) == ((0x1UL << (26U)))) ? 1UL : 0UL);
}
# 3488 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline uint32_t LL_USART_IsEnabledIT_ERROR(USART_TypeDef *USARTx)
{
  return ((((USARTx->CR3) & ((0x1UL << (0U)))) == ((0x1UL << (0U)))) ? 1UL : 0UL);
}
# 3501 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline uint32_t LL_USART_IsEnabledIT_CTS(USART_TypeDef *USARTx)
{
  return ((((USARTx->CR3) & ((0x1UL << (10U)))) == ((0x1UL << (10U)))) ? 1UL : 0UL);
}
# 3536 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_EnableDMAReq_RX(USART_TypeDef *USARTx)
{
  ((USARTx->CR3) |= ((0x1UL << (6U))));
}







static __inline void LL_USART_DisableDMAReq_RX(USART_TypeDef *USARTx)
{
  ((USARTx->CR3) &= ~((0x1UL << (6U))));
}







static __inline uint32_t LL_USART_IsEnabledDMAReq_RX(USART_TypeDef *USARTx)
{
  return ((((USARTx->CR3) & ((0x1UL << (6U)))) == ((0x1UL << (6U)))) ? 1UL : 0UL);
}







static __inline void LL_USART_EnableDMAReq_TX(USART_TypeDef *USARTx)
{
  ((USARTx->CR3) |= ((0x1UL << (7U))));
}







static __inline void LL_USART_DisableDMAReq_TX(USART_TypeDef *USARTx)
{
  ((USARTx->CR3) &= ~((0x1UL << (7U))));
}







static __inline uint32_t LL_USART_IsEnabledDMAReq_TX(USART_TypeDef *USARTx)
{
  return ((((USARTx->CR3) & ((0x1UL << (7U)))) == ((0x1UL << (7U)))) ? 1UL : 0UL);
}







static __inline void LL_USART_EnableDMADeactOnRxErr(USART_TypeDef *USARTx)
{
  ((USARTx->CR3) |= ((0x1UL << (13U))));
}







static __inline void LL_USART_DisableDMADeactOnRxErr(USART_TypeDef *USARTx)
{
  ((USARTx->CR3) &= ~((0x1UL << (13U))));
}







static __inline uint32_t LL_USART_IsEnabledDMADeactOnRxErr(USART_TypeDef *USARTx)
{
  return ((((USARTx->CR3) & ((0x1UL << (13U)))) == ((0x1UL << (13U)))) ? 1UL : 0UL);
}
# 3639 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline uint32_t LL_USART_DMA_GetRegAddr(USART_TypeDef *USARTx, uint32_t Direction)
{
  uint32_t data_reg_addr;

  if (Direction == 0x00000000U)
  {

    data_reg_addr = (uint32_t) &(USARTx->TDR);
  }
  else
  {

    data_reg_addr = (uint32_t) &(USARTx->RDR);
  }

  return data_reg_addr;
}
# 3671 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline uint8_t LL_USART_ReceiveData8(USART_TypeDef *USARTx)
{
  return (uint8_t)(((USARTx->RDR) & (((uint16_t)0x01FFU))) & 0xFFU);
}







static __inline uint16_t LL_USART_ReceiveData9(USART_TypeDef *USARTx)
{
  return (uint16_t)(((USARTx->RDR) & (((uint16_t)0x01FFU))));
}
# 3694 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_TransmitData8(USART_TypeDef *USARTx, uint8_t Value)
{
  USARTx->TDR = Value;
}
# 3706 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_TransmitData9(USART_TypeDef *USARTx, uint16_t Value)
{
  USARTx->TDR = (uint16_t)(Value & 0x1FFUL);
}
# 3727 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_RequestAutoBaudRate(USART_TypeDef *USARTx)
{
  ((USARTx->RQR) |= ((uint16_t)(0x1UL << (0U))));
}







static __inline void LL_USART_RequestBreakSending(USART_TypeDef *USARTx)
{
  ((USARTx->RQR) |= ((uint16_t)(0x1UL << (1U))));
}







static __inline void LL_USART_RequestEnterMuteMode(USART_TypeDef *USARTx)
{
  ((USARTx->RQR) |= ((uint16_t)(0x1UL << (2U))));
}
# 3762 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
static __inline void LL_USART_RequestRxDataFlush(USART_TypeDef *USARTx)
{
  ((USARTx->RQR) |= ((uint16_t)(0x1UL << (3U))));
}
# 3790 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
ErrorStatus LL_USART_DeInit(USART_TypeDef *USARTx);
ErrorStatus LL_USART_Init(USART_TypeDef *USARTx, LL_USART_InitTypeDef *USART_InitStruct);
void LL_USART_StructInit(LL_USART_InitTypeDef *USART_InitStruct);
ErrorStatus LL_USART_ClockInit(USART_TypeDef *USARTx, LL_USART_ClockInitTypeDef *USART_ClockInitStruct);
void LL_USART_ClockStructInit(LL_USART_ClockInitTypeDef *USART_ClockInitStruct);
# 41 "../Core/Inc/main.h" 2
# 1 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_gpio.h" 1
# 64 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_gpio.h"
typedef struct
{
  uint32_t Pin;


  uint32_t Mode;




  uint32_t Speed;




  uint32_t OutputType;




  uint32_t Pull;




  uint32_t Alternate;



} LL_GPIO_InitTypeDef;
# 268 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_gpio.h"
static __inline void LL_GPIO_SetPinMode(GPIO_TypeDef *GPIOx, uint32_t Pin, uint32_t Mode)
{
  (((GPIOx->MODER)) = ((((((GPIOx->MODER))) & (~(((Pin * Pin) * (0x3UL << (0U)))))) | (((Pin * Pin) * Mode)))));
}
# 302 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_gpio.h"
static __inline uint32_t LL_GPIO_GetPinMode(GPIO_TypeDef *GPIOx, uint32_t Pin)
{
  return (uint32_t)(((GPIOx->MODER) & (((Pin * Pin) * (0x3UL << (0U))))) / (Pin * Pin));
}
# 336 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_gpio.h"
static __inline void LL_GPIO_SetPinOutputType(GPIO_TypeDef *GPIOx, uint32_t PinMask, uint32_t OutputType)
{
  (((GPIOx->OTYPER)) = ((((((GPIOx->OTYPER))) & (~(PinMask))) | ((PinMask * OutputType)))));
}
# 370 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_gpio.h"
static __inline uint32_t LL_GPIO_GetPinOutputType(GPIO_TypeDef *GPIOx, uint32_t Pin)
{
  return (uint32_t)(((GPIOx->OTYPER) & (Pin)) / Pin);
}
# 406 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_gpio.h"
static __inline void LL_GPIO_SetPinSpeed(GPIO_TypeDef *GPIOx, uint32_t Pin, uint32_t Speed)
{
  (((GPIOx->OSPEEDR)) = ((((((GPIOx->OSPEEDR))) & (~(((Pin * Pin) * (0x3UL << (0U)))))) | (((Pin * Pin) * Speed)))));
}
# 441 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_gpio.h"
static __inline uint32_t LL_GPIO_GetPinSpeed(GPIO_TypeDef *GPIOx, uint32_t Pin)
{
  return (uint32_t)(((GPIOx->OSPEEDR) & (((Pin * Pin) * (0x3UL << (0U))))) / (Pin * Pin));
}
# 474 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_gpio.h"
static __inline void LL_GPIO_SetPinPull(GPIO_TypeDef *GPIOx, uint32_t Pin, uint32_t Pull)
{
  (((GPIOx->PUPDR)) = ((((((GPIOx->PUPDR))) & (~(((Pin * Pin) * (0x3UL << (0U)))))) | (((Pin * Pin) * Pull)))));
}
# 506 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_gpio.h"
static __inline uint32_t LL_GPIO_GetPinPull(GPIO_TypeDef *GPIOx, uint32_t Pin)
{
  return (uint32_t)(((GPIOx->PUPDR) & (((Pin * Pin) * (0x3UL << (0U))))) / (Pin * Pin));
}
# 537 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_gpio.h"
static __inline void LL_GPIO_SetAFPin_0_7(GPIO_TypeDef *GPIOx, uint32_t Pin, uint32_t Alternate)
{
  (((GPIOx->AFR[0])) = ((((((GPIOx->AFR[0]))) & (~(((((Pin * Pin) * Pin) * Pin) * (0xFUL << (0U)))))) | (((((Pin * Pin) * Pin) * Pin) * Alternate)))));

}
# 566 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_gpio.h"
static __inline uint32_t LL_GPIO_GetAFPin_0_7(GPIO_TypeDef *GPIOx, uint32_t Pin)
{
  return (uint32_t)(((GPIOx->AFR[0]) & (((((Pin * Pin) * Pin) * Pin) * (0xFUL << (0U))))) / (((Pin * Pin) * Pin) * Pin));

}
# 598 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_gpio.h"
static __inline void LL_GPIO_SetAFPin_8_15(GPIO_TypeDef *GPIOx, uint32_t Pin, uint32_t Alternate)
{
  (((GPIOx->AFR[1])) = ((((((GPIOx->AFR[1]))) & (~((((((Pin >> 8U) * (Pin >> 8U)) * (Pin >> 8U)) * (Pin >> 8U)) * (0xFUL << (0U)))))) | ((((((Pin >> 8U) * (Pin >> 8U)) * (Pin >> 8U)) * (Pin >> 8U)) * Alternate)))));

}
# 628 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_gpio.h"
static __inline uint32_t LL_GPIO_GetAFPin_8_15(GPIO_TypeDef *GPIOx, uint32_t Pin)
{
  return (uint32_t)(((GPIOx->AFR[1]) & ((((((Pin >> 8U) * (Pin >> 8U)) * (Pin >> 8U)) * (Pin >> 8U)) * (0xFUL << (0U))))) / ((((Pin >> 8U) *

                                 (Pin >> 8U)) * (Pin >> 8U)) * (Pin >> 8U)));
}
# 665 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_gpio.h"
static __inline void LL_GPIO_LockPin(GPIO_TypeDef *GPIOx, uint32_t PinMask)
{
  volatile uint32_t temp;
  ((GPIOx->LCKR) = ((0x1UL << (16U)) | PinMask));
  ((GPIOx->LCKR) = (PinMask));
  ((GPIOx->LCKR) = ((0x1UL << (16U)) | PinMask));
  temp = ((GPIOx->LCKR));
  (void) temp;
}
# 699 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_gpio.h"
static __inline uint32_t LL_GPIO_IsPinLocked(GPIO_TypeDef *GPIOx, uint32_t PinMask)
{
  return (((GPIOx->LCKR) & (PinMask)) == (PinMask));
}







static __inline uint32_t LL_GPIO_IsAnyPinLocked(GPIO_TypeDef *GPIOx)
{
  return (((GPIOx->LCKR) & ((0x1UL << (16U)))) == ((0x1UL << (16U))));
}
# 729 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_gpio.h"
static __inline uint32_t LL_GPIO_ReadInputPort(GPIO_TypeDef *GPIOx)
{
  return (uint32_t)(((GPIOx->IDR)));
}
# 758 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_gpio.h"
static __inline uint32_t LL_GPIO_IsInputPinSet(GPIO_TypeDef *GPIOx, uint32_t PinMask)
{
  return (((GPIOx->IDR) & (PinMask)) == (PinMask));
}
# 770 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_gpio.h"
static __inline void LL_GPIO_WriteOutputPort(GPIO_TypeDef *GPIOx, uint32_t PortValue)
{
  ((GPIOx->ODR) = (PortValue));
}







static __inline uint32_t LL_GPIO_ReadOutputPort(GPIO_TypeDef *GPIOx)
{
  return (uint32_t)(((GPIOx->ODR)));
}
# 810 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_gpio.h"
static __inline uint32_t LL_GPIO_IsOutputPinSet(GPIO_TypeDef *GPIOx, uint32_t PinMask)
{
  return (((GPIOx->ODR) & (PinMask)) == (PinMask));
}
# 839 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_gpio.h"
static __inline void LL_GPIO_SetOutputPin(GPIO_TypeDef *GPIOx, uint32_t PinMask)
{
  ((GPIOx->BSRR) = (PinMask));
}
# 868 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_gpio.h"
static __inline void LL_GPIO_ResetOutputPin(GPIO_TypeDef *GPIOx, uint32_t PinMask)
{
  ((GPIOx->BRR) = (PinMask));
}
# 897 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_gpio.h"
static __inline void LL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint32_t PinMask)
{
  uint32_t odr = ((GPIOx->ODR));
  ((GPIOx->BSRR) = (((odr & PinMask) << 16u) | (~odr & PinMask)));
}
# 912 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_gpio.h"
ErrorStatus LL_GPIO_DeInit(GPIO_TypeDef *GPIOx);
ErrorStatus LL_GPIO_Init(GPIO_TypeDef *GPIOx, LL_GPIO_InitTypeDef *GPIO_InitStruct);
void LL_GPIO_StructInit(LL_GPIO_InitTypeDef *GPIO_InitStruct);
# 42 "../Core/Inc/main.h" 2
# 68 "../Core/Inc/main.h"
void Error_Handler(void);
# 203 "../Core/Inc\\triceConfig.h" 2
static inline void ToggleOpticalFeedbackLED( void ){
    LL_GPIO_TogglePin(((GPIO_TypeDef *) ((0x40000000UL + 0x08000000UL) + 0x00000000UL)), (0x00000020U));
}
# 222 "../Core/Inc\\triceConfig.h"
static inline uint32_t triceTxDataRegisterEmptyUartA(void) {
    return LL_USART_IsActiveFlag_TXE(((USART_TypeDef *) (0x40000000UL + 0x00004400UL)));
}




static inline void triceTransmitData8UartA(uint8_t v) {
    LL_USART_TransmitData8(((USART_TypeDef *) (0x40000000UL + 0x00004400UL)), v);
    ToggleOpticalFeedbackLED();
}



static inline void triceEnableTxEmptyInterruptUartA(void) {
    LL_USART_EnableIT_TXE(((USART_TypeDef *) (0x40000000UL + 0x00004400UL)));
}



static inline void triceDisableTxEmptyInterruptUartA(void) {
    LL_USART_DisableIT_TXE(((USART_TypeDef *) (0x40000000UL + 0x00004400UL)));
}
# 96 "../../../src/trice.h" 2

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
# 98 "../../../src/trice.h" 2
# 131 "../../../src/trice.h"
void TriceWriteDevice( TriceWriteDevice_t device, uint8_t *buf, size_t len );



uint16_t TriceStamp16( void );



uint32_t TriceStamp32( void );
# 202 "../../../src/trice.h"
extern char* const triceCommandBuffer;
extern int triceCommandFlag;
extern int triceCommandLength;

size_t TriceDepth( void );
size_t TriceDepthMax( void );

extern uint32_t* TriceBufferWritePosition;


size_t triceNonBlockingWriteUartA( void const * buf, size_t nByte );
size_t triceNonBlockingWriteUartB( void const * buf, size_t nByte );






extern unsigned triceDepthMax;
extern size_t triceFifoDepthMax;
extern size_t triceStreamBufferDepthMax;

void TriceOut( uint32_t* tb, size_t tLen );
void TriceLogBufferInfo( void );

void TriceTransfer( void );
void TriceCheckSet( int index );
# 252 "../../../src/trice.h"
extern uint8_t TriceCycle;
# 304 "../../../src/trice.h"
void TriceBlockingWriteUartA( uint8_t const * buf, unsigned len );
uint8_t TriceNextUint8UartA( void );
void triceServeTransmitUartA(void);
void triceTriggerTransmitUartA(void);
unsigned TriceOutDepthUartA( void );
# 319 "../../../src/trice.h"
unsigned TriceOutDepth( void );
# 389 "../../../src/trice.h"
static inline uint32_t aFloat( float f ){
    union {
        float from;
        uint32_t to;
    } pun = { .from = f };
    return pun.to;
}


static inline float asFloat( uint32_t x ){
    union {
        uint32_t from;
        float to;
    } pun = { .from = x };
    return pun.to;
}


static inline uint64_t aDouble( double x ){
    union {
        double d;
        uint64_t u;
    } t;
    t.d = x;
    return t.u;
}
# 503 "../../../src/trice.h"
extern const int TriceTypeS0;
extern const int TriceTypeS2;
extern const int TriceTypeS4;
extern const int TriceTypeX0;
# 8 "../../../src/triceCheck.c" 2




static int32_t FloatToInt32( float f );
static int64_t DoubleToInt64( double f );
static void exampleOfManualSerialization( void );
static void exampleOfManualJSONencoding(void);
static void exampleOfBuffersAndFunctions(void);

static void showTriceDepth( void ){

    {
        uint16_t triceDepth = TriceDepth();
        uint16_t triceDepthMax = TriceDepthMax();
        { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint32_t ts = TriceStamp32(); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0xC000|(3037))); TriceBufferWritePosition = (uint32_t*)p; }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (((ts) >> 16)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; }; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(triceDepth) | ((uint32_t)(100)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(triceDepthMax-100) | ((uint32_t)((((0x1000/2) + 3) & ~3))<<16)); }while(0); } __set_PRIMASK(primaskstate); };
    }




}




void TriceCheckSet(int index) {
    char* A = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
    char* s = "AAAAAAAAAAAA";
    float x = (float)1089.6082763671875;
    double y = 518.0547492508867;

    switch (index) {
           case 41: {
                           float a = (float)5.934;
                           float b = a + ((a > 0) ? 0.0005f : -0.0005f);
                           int c = (int)b;
                           int d = (int)(b * 1000) % 1000;
                           int e = (int)(1000 * (float)(a - c));
                           { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint32_t ts = TriceStamp32(); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0xC000|(4716))); TriceBufferWritePosition = (uint32_t*)p; }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (((ts) >> 16)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; }; do{ uint16_t v = ((20)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(a))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(c)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(d)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(c)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(e)); }while(0); } __set_PRIMASK(primaskstate); };
                           { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint32_t ts = TriceStamp32(); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0xC000|(1570))); TriceBufferWritePosition = (uint32_t*)p; }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (((ts) >> 16)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; }; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat( 1.0/11 ))); }while(0); } __set_PRIMASK(primaskstate); };
                       }
    break; case 50: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7420)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 51: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2244)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 52: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1977)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 53: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7846)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 54: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2831)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 55: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3150)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 56: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1327)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 57: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3111)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 58: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3159)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 59: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2039)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 60: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5589)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 61: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7322)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 62: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7707)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 63: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6334)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 64: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1556)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 65: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6822)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 66: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6078)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 67: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7224)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 68: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1931)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 69: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7227)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 70: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4609)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 71: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1296)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 72: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5292)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 73: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7304)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 74: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5651)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 75: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1800)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 76: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1322)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 77: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6227)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 78: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6257)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 79: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7793)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 80: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1052)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 81: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2635)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 82: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6972)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 83: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3148)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 84: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6639)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 85: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2521)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 86: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5572)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 87: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4573)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 88: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1606)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 89: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2807)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 90: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5762)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 91: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1929)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 92: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2022)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 93: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6837)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 94: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4171)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 95: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4554)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 96: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5163)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 97: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3309)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 98: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3897)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 99: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5118)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 100: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4798)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 101: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3641)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 102: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6183)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 103: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2818)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 104: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6245)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 105: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7445)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 106: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5722)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 107: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7147)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 108: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3715)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 109: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2096)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 110: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6987)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 111: {
                              unsigned len = strlen(s);
                              { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7354)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len)); }while(0); } __set_PRIMASK(primaskstate); };
                              do { uint32_t ssiz = strlen( s ); do { uint32_t limit = 120 -8; uint32_t len_ = ssiz; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5129)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, s, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0); } while(0);
                              { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1439)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len)); }while(0); } __set_PRIMASK(primaskstate); };
                              do { uint32_t limit = 120 -8; uint32_t len_ = len; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5182)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, s, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0);

                              { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7260)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(-555555555.5555555555))); }while(0); } __set_PRIMASK(primaskstate); };
                              { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3291)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(-555555555.5555555555))); }while(0); } __set_PRIMASK(primaskstate); };
                              { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4343)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(-555555555.5555555555))); }while(0); } __set_PRIMASK(primaskstate); };
                          }
    break; case 122: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7679)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 123: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5550)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 124: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7867)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 125: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7875)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 126: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4680)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 127: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5219)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 128: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1169)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 129: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1058)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-1)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 130: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6341)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-1)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 131: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2590)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-1)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 132: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7612)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-1)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 133: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4929)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-1)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 134: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2778)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-1)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 135: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7508)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-1)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 136: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7618)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(-555555555.5555555555))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(-555555555.5555555555))>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 137: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5474)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(-555555555.5555555555))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(-555555555.5555555555))>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 138: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3164)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(-555555555.5555555555))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(-555555555.5555555555))>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 139: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5683)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 140: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2113)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((*(volatile uint32_t*)0xE000E018UL))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 141: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1076)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((*(volatile uint32_t*)0xE000E018UL))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 142: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2052)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((*(volatile uint32_t*)0xE000E018UL))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 143: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7509)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((*(volatile uint32_t*)0xE000E018UL))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 144: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1524)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((16)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(1)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(2)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 145: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7306)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 146: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2595)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 147: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4965)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 148: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7388)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 149: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4826)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 150: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7624)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((16)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(1)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(2)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 151: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1898)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 152: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6283)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 153: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5556)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 154: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2746)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((16)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(1)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(2)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 155: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2869)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 156: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3506)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 157: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4889)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((16)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(1)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(2)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 158: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3885)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(158)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 159: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4144)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(159)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(159)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 160: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1603)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(160)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(2)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 161: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7163)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((16)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(161)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(161)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(2)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 162: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6182)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 163: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4045)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 164: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5585)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 165: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5114)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((12)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 166: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6099)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((16)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 167: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2493)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((20)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 168: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3618)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((24)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-6)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 169: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7990)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((28)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-7)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 170: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3339)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((32)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-8)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 171: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2232)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((36)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-8)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-9)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 172: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6850)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((40)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-8)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-9)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-10)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 173: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6090)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((44)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-8)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-9)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-10)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-11)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 174: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5495)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((48)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-8)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-9)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-10)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-11)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-12)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 175: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4369)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 176: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6881)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(1)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 177: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6054)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(2)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 178: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7515)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((12)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 179: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4409)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((16)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(4)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 180: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6543)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((20)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(5)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 181: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5706)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((24)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(6)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 182: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3749)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((28)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(7)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 183: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1187)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((32)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(8)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 184: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5157)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((36)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(8)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(9)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 185: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4191)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((40)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(8)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(9)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(10)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 186: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5030)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((44)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(8)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(9)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(10)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(11)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 187: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1708)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((48)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(8)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(9)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(10)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(11)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(12)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 188: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2228)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 189: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4777)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((1)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint8_t)(-1))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 190: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6821)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((2)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((0x0000FF00 & ((uint32_t)(-2)<< 8)) |((uint8_t)(-1))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 191: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2030)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((3)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((0x00FF0000 & ((uint32_t)(-3)<<16)) |(0x0000FF00 & ((uint32_t)(-2)<< 8)) |((uint8_t)(-1))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 192: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2955)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-4)<<24) |(0x00FF0000 & ((uint32_t)(-3)<<16)) |(0x0000FF00 & ((uint32_t)(-2)<< 8)) |((uint8_t)(-1))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 193: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6626)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((5)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-4)<<24) |(0x00FF0000 & ((uint32_t)(-3)<<16)) |(0x0000FF00 & ((uint32_t)(-2)<< 8)) |((uint8_t)(-1))); }while(0); do{ *TriceBufferWritePosition++ = (((uint8_t)(-5))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 194: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4356)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((6)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-4)<<24) |(0x00FF0000 & ((uint32_t)(-3)<<16)) |(0x0000FF00 & ((uint32_t)(-2)<< 8)) |((uint8_t)(-1))); }while(0); do{ *TriceBufferWritePosition++ = ((0x0000FF00 & ((uint32_t)(-6)<< 8)) |((uint8_t)(-5))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 195: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4819)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((7)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-4)<<24) |(0x00FF0000 & ((uint32_t)(-3)<<16)) |(0x0000FF00 & ((uint32_t)(-2)<< 8)) |((uint8_t)(-1))); }while(0); do{ *TriceBufferWritePosition++ = ((0x00FF0000 & ((uint32_t)(-7)<<16)) |(0x0000FF00 & ((uint32_t)(-6)<< 8)) |((uint8_t)(-5))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 196: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3598)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-4)<<24) |(0x00FF0000 & ((uint32_t)(-3)<<16)) |(0x0000FF00 & ((uint32_t)(-2)<< 8)) |((uint8_t)(-1))); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-8)<<24) |(0x00FF0000 & ((uint32_t)(-7)<<16)) |(0x0000FF00 & ((uint32_t)(-6)<< 8)) |((uint8_t)(-5))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 197: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1273)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((9)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-4)<<24) |(0x00FF0000 & ((uint32_t)(-3)<<16)) |(0x0000FF00 & ((uint32_t)(-2)<< 8)) |((uint8_t)(-1))); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-8)<<24) |(0x00FF0000 & ((uint32_t)(-7)<<16)) |(0x0000FF00 & ((uint32_t)(-6)<< 8)) |((uint8_t)(-5))); }while(0); do{ *TriceBufferWritePosition++ = (((uint8_t)(-9))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 198: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5514)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((10)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-4)<<24) |(0x00FF0000 & ((uint32_t)(-3)<<16)) |(0x0000FF00 & ((uint32_t)(-2)<< 8)) |((uint8_t)(-1))); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-8)<<24) |(0x00FF0000 & ((uint32_t)(-7)<<16)) |(0x0000FF00 & ((uint32_t)(-6)<< 8)) |((uint8_t)(-5))); }while(0); do{ *TriceBufferWritePosition++ = ((0x0000FF00 & ((uint32_t)(-10)<< 8)) |((uint8_t)(-9))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 199: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6823)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((11)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-4)<<24) |(0x00FF0000 & ((uint32_t)(-3)<<16)) |(0x0000FF00 & ((uint32_t)(-2)<< 8)) |((uint8_t)(-1))); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-8)<<24) |(0x00FF0000 & ((uint32_t)(-7)<<16)) |(0x0000FF00 & ((uint32_t)(-6)<< 8)) |((uint8_t)(-5))); }while(0); do{ *TriceBufferWritePosition++ = ((0x00FF0000 & ((uint32_t)(-11)<<16))|(0x0000FF00 & ((uint32_t)(-10)<< 8)) |((uint8_t)(-9))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 200: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1093)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((12)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-4)<<24) |(0x00FF0000 & ((uint32_t)(-3)<<16)) |(0x0000FF00 & ((uint32_t)(-2)<< 8)) |((uint8_t)(-1))); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-8)<<24) |(0x00FF0000 & ((uint32_t)(-7)<<16)) |(0x0000FF00 & ((uint32_t)(-6)<< 8)) |((uint8_t)(-5))); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-12)<<24)|(0x00FF0000 & ((uint32_t)(-11)<<16))|(0x0000FF00 & ((uint32_t)(-10)<< 8)) |((uint8_t)(-9))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 201: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6889)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 202: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4793)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((1)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint8_t)(1))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 203: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1120)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((2)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((0x0000FF00 & ((uint32_t)(2)<< 8)) |((uint8_t)(1))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 204: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2618)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((3)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((0x00FF0000 & ((uint32_t)(3)<<16)) |(0x0000FF00 & ((uint32_t)(2)<< 8)) |((uint8_t)(1))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 205: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3465)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(4)<<24) |(0x00FF0000 & ((uint32_t)(3)<<16)) |(0x0000FF00 & ((uint32_t)(2)<< 8)) |((uint8_t)(1))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 206: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5134)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((5)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(4)<<24) |(0x00FF0000 & ((uint32_t)(3)<<16)) |(0x0000FF00 & ((uint32_t)(2)<< 8)) |((uint8_t)(1))); }while(0); do{ *TriceBufferWritePosition++ = (((uint8_t)(5))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 207: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5101)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((6)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(4)<<24) |(0x00FF0000 & ((uint32_t)(3)<<16)) |(0x0000FF00 & ((uint32_t)(2)<< 8)) |((uint8_t)(1))); }while(0); do{ *TriceBufferWritePosition++ = ((0x0000FF00 & ((uint32_t)(6)<< 8)) |((uint8_t)(5))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 208: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3444)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((7)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(4)<<24) |(0x00FF0000 & ((uint32_t)(3)<<16)) |(0x0000FF00 & ((uint32_t)(2)<< 8)) |((uint8_t)(1))); }while(0); do{ *TriceBufferWritePosition++ = ((0x00FF0000 & ((uint32_t)(7)<<16)) |(0x0000FF00 & ((uint32_t)(6)<< 8)) |((uint8_t)(5))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 209: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6625)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(4)<<24) |(0x00FF0000 & ((uint32_t)(3)<<16)) |(0x0000FF00 & ((uint32_t)(2)<< 8)) |((uint8_t)(1))); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(8)<<24) |(0x00FF0000 & ((uint32_t)(7)<<16)) |(0x0000FF00 & ((uint32_t)(6)<< 8)) |((uint8_t)(5))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 210: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4028)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((9)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(4)<<24) |(0x00FF0000 & ((uint32_t)(3)<<16)) |(0x0000FF00 & ((uint32_t)(2)<< 8)) |((uint8_t)(1))); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(8)<<24) |(0x00FF0000 & ((uint32_t)(7)<<16)) |(0x0000FF00 & ((uint32_t)(6)<< 8)) |((uint8_t)(5))); }while(0); do{ *TriceBufferWritePosition++ = (((uint8_t)(9))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 211: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5214)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((10)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(4)<<24) |(0x00FF0000 & ((uint32_t)(3)<<16)) |(0x0000FF00 & ((uint32_t)(2)<< 8)) |((uint8_t)(1))); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(8)<<24) |(0x00FF0000 & ((uint32_t)(7)<<16)) |(0x0000FF00 & ((uint32_t)(6)<< 8)) |((uint8_t)(5))); }while(0); do{ *TriceBufferWritePosition++ = ((0x0000FF00 & ((uint32_t)(10)<< 8)) |((uint8_t)(9))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 212: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3273)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((11)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(4)<<24) |(0x00FF0000 & ((uint32_t)(3)<<16)) |(0x0000FF00 & ((uint32_t)(2)<< 8)) |((uint8_t)(1))); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(8)<<24) |(0x00FF0000 & ((uint32_t)(7)<<16)) |(0x0000FF00 & ((uint32_t)(6)<< 8)) |((uint8_t)(5))); }while(0); do{ *TriceBufferWritePosition++ = ((0x00FF0000 & ((uint32_t)(11)<<16))|(0x0000FF00 & ((uint32_t)(10)<< 8)) |((uint8_t)(9))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 213: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4859)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((12)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(4)<<24) |(0x00FF0000 & ((uint32_t)(3)<<16)) |(0x0000FF00 & ((uint32_t)(2)<< 8)) |((uint8_t)(1))); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(8)<<24) |(0x00FF0000 & ((uint32_t)(7)<<16)) |(0x0000FF00 & ((uint32_t)(6)<< 8)) |((uint8_t)(5))); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(12)<<24)|(0x00FF0000 & ((uint32_t)(11)<<16))|(0x0000FF00 & ((uint32_t)(10)<< 8)) |((uint8_t)(9))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 214: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7429)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 215: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3404)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((2)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 216: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6706)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 217: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3774)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((6)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-3)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 218: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4243)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-3) | ((uint32_t)(-4)<<16)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 219: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6266)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((10)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-3) | ((uint32_t)(-4)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-5)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 220: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6750)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((12)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-3) | ((uint32_t)(-4)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-5) | ((uint32_t)(-6)<<16)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 221: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6634)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((14)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-3) | ((uint32_t)(-4)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-5) | ((uint32_t)(-6)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-7)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 222: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1036)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((16)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-3) | ((uint32_t)(-4)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-5) | ((uint32_t)(-6)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-7) | ((uint32_t)(-8)<<16)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 223: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2626)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((18)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-3) | ((uint32_t)(-4)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-5) | ((uint32_t)(-6)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-7) | ((uint32_t)(-8)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-9)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 224: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7537)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((20)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-3) | ((uint32_t)(-4)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-5) | ((uint32_t)(-6)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-7) | ((uint32_t)(-8)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-9) | ((uint32_t)(-10)<<16)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 225: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6097)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((22)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-3) | ((uint32_t)(-4)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-5) | ((uint32_t)(-6)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-7) | ((uint32_t)(-8)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-9) | ((uint32_t)(-10)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-11)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 226: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6891)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((24)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-3) | ((uint32_t)(-4)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-5) | ((uint32_t)(-6)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-7) | ((uint32_t)(-8)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-9) | ((uint32_t)(-10)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-11)| ((uint32_t)(-12)<<16)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 227: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6800)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 228: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2697)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((2)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(1)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 229: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4552)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(1) | ((uint32_t)(2)<<16)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 230: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5086)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((6)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(1) | ((uint32_t)(2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(3)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 231: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7226)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(1) | ((uint32_t)(2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(3) | ((uint32_t)(4)<<16)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 232: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4651)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((10)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(1) | ((uint32_t)(2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(3) | ((uint32_t)(4)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(5)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 233: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2009)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((12)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(1) | ((uint32_t)(2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(3) | ((uint32_t)(4)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(5) | ((uint32_t)(6)<<16)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 234: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1805)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((14)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(1) | ((uint32_t)(2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(3) | ((uint32_t)(4)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(5) | ((uint32_t)(6)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(7)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 235: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4458)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((16)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(1) | ((uint32_t)(2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(3) | ((uint32_t)(4)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(5) | ((uint32_t)(6)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(7) | ((uint32_t)(8)<<16)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 236: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4396)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((18)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(1) | ((uint32_t)(2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(3) | ((uint32_t)(4)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(5) | ((uint32_t)(6)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(7) | ((uint32_t)(8)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(9)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 237: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3901)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((20)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(1) | ((uint32_t)(2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(3) | ((uint32_t)(4)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(5) | ((uint32_t)(6)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(7) | ((uint32_t)(8)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(9) | ((uint32_t)(10)<<16)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 238: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1196)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((22)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(1) | ((uint32_t)(2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(3) | ((uint32_t)(4)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(5) | ((uint32_t)(6)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(7) | ((uint32_t)(8)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(9) | ((uint32_t)(10)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(11)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 239: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5622)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((24)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(1) | ((uint32_t)(2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(3) | ((uint32_t)(4)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(5) | ((uint32_t)(6)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(7) | ((uint32_t)(8)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(9) | ((uint32_t)(10)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(11)| ((uint32_t)(12)<<16)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 240: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3923)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 241: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2676)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((2)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 242: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5677)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 243: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1218)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((6)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-3)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 244: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1822)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-3) | ((uint32_t)(-4)<<16)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 245: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7576)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((10)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-3) | ((uint32_t)(-4)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-5)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 246: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3413)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((12)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-3) | ((uint32_t)(-4)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-5) | ((uint32_t)(-6)<<16)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 247: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1460)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((14)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-3) | ((uint32_t)(-4)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-5) | ((uint32_t)(-6)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-7)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 248: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3125)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((16)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-3) | ((uint32_t)(-4)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-5) | ((uint32_t)(-6)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-7) | ((uint32_t)(-8)<<16)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 249: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5870)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((18)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-3) | ((uint32_t)(-4)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-5) | ((uint32_t)(-6)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-7) | ((uint32_t)(-8)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-9)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 250: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7559)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((20)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-3) | ((uint32_t)(-4)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-5) | ((uint32_t)(-6)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-7) | ((uint32_t)(-8)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-9) | ((uint32_t)(-10)<<16)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 251: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2528)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((22)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-3) | ((uint32_t)(-4)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-5) | ((uint32_t)(-6)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-7) | ((uint32_t)(-8)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-9) | ((uint32_t)(-10)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-11)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 252: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3313)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((24)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-3) | ((uint32_t)(-4)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-5) | ((uint32_t)(-6)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-7) | ((uint32_t)(-8)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-9) | ((uint32_t)(-10)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-11)| ((uint32_t)(-12)<<16)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 253: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3303)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 254: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2057)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((2)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 255: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2216)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 256: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3850)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((6)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-3)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 257: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2529)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-3) | ((uint32_t)(-4)<<16)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 258: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7966)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((10)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-3) | ((uint32_t)(-4)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-5)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 259: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2313)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((12)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-3) | ((uint32_t)(-4)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-5) | ((uint32_t)(-6)<<16)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 260: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2265)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((14)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-3) | ((uint32_t)(-4)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-5) | ((uint32_t)(-6)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-7)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 261: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7807)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((16)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-3) | ((uint32_t)(-4)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-5) | ((uint32_t)(-6)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-7) | ((uint32_t)(-8)<<16)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 262: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6977)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((18)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-3) | ((uint32_t)(-4)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-5) | ((uint32_t)(-6)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-7) | ((uint32_t)(-8)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-9)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 263: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6108)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((20)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-3) | ((uint32_t)(-4)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-5) | ((uint32_t)(-6)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-7) | ((uint32_t)(-8)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-9) | ((uint32_t)(-10)<<16)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 264: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1700)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((22)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-3) | ((uint32_t)(-4)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-5) | ((uint32_t)(-6)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-7) | ((uint32_t)(-8)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-9) | ((uint32_t)(-10)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-11)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 265: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2700)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((24)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-3) | ((uint32_t)(-4)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-5) | ((uint32_t)(-6)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-7) | ((uint32_t)(-8)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-9) | ((uint32_t)(-10)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-11)| ((uint32_t)(-12)<<16)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 266: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3067)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 267: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5489)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 268: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5560)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 269: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3397)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((12)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 270: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7650)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((16)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 271: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7629)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((20)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 272: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5320)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((24)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-6)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 273: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7046)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((28)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-7)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 274: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6510)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((32)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-8)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 275: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1247)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((36)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-8)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-9)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 276: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6218)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((40)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-8)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-9)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-10)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 277: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6864)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((44)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-8)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-9)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-10)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-11)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 278: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3247)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((48)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-8)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-9)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-10)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-11)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-12)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 279: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2394)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 280: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6922)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(1)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 281: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6409)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(2)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 282: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4958)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((12)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 283: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3782)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((16)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(4)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 284: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6340)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((20)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(5)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 285: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5869)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((24)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(6)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 286: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3362)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((28)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(7)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 287: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7378)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((32)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(8)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 288: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3526)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((36)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(8)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(9)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 289: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5859)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((40)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(8)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(9)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(10)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 290: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4194)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((44)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(8)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(9)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(10)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(11)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 291: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7438)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((48)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(8)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(9)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(10)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(11)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(12)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 292: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4843)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 293: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7873)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-1)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 294: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3768)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((16)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-1)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-2)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 295: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1632)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((24)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-1)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-2)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-3)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 296: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7047)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((32)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-1)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-2)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-4)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 297: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6310)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((40)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-1)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-2)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-4)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-5)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 298: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1777)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((48)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-1)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-2)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-4)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-5)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-6)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 299: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2434)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((56)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-1)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-2)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-4)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-5)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-6)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-7)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 300: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1761)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((64)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-1)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-2)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-4)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-5)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-6)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-7)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-8)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-8)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 301: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2565)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((72)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-1)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-2)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-4)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-5)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-6)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-7)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-8)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-8)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-9)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-9)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 302: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3696)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((80)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-1)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-2)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-4)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-5)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-6)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-7)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-8)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-8)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-9)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-9)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-10)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-10)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 303: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4096)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((88)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-1)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-2)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-4)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-5)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-6)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-7)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-8)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-8)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-9)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-9)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-10)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-10)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-11)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-11)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 304: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1306)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((96)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-1)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-2)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-4)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-5)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-6)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-7)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-8)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-8)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-9)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-9)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-10)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-10)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-11)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-11)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-12)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-12)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 305: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4493)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 306: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2845)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(1)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 307: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7544)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((16)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(1)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(2)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 308: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4551)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((16)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(0x1122334455667788)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(0x1122334455667788)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(0x0102030405060708)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(0x0102030405060708)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 309: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6955)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((24)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(1)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(2)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(3)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 310: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2205)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((32)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(1)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(2)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(4)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 311: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7969)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((40)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(1)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(2)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(4)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(5)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 312: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6618)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((48)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(1)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(2)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(4)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(5)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(6)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 313: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6851)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((56)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(1)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(2)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(4)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(5)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(6)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(7)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 314: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5725)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((64)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(1)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(2)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(4)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(5)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(6)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(7)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(8)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(8)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 315: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5735)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((72)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(1)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(2)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(4)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(5)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(6)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(7)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(8)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(8)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(9)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(9)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 316: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1417)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((80)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(1)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(2)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(4)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(5)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(6)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(7)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(8)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(8)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(9)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(9)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(10)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(10)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 317: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3679)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((88)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(1)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(2)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(4)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(5)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(6)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(7)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(8)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(8)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(9)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(9)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(10)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(10)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(11)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(11)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 318: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4982)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((96)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(1)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(2)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(4)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(5)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(6)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(7)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(8)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(8)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(9)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(9)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(10)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(10)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(11)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(11)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(12)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(12)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 319: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3564)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 320: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6565)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 321: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7666)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 322: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2906)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(12345)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 323: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2121)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 324: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5658)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 325: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3703)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 326: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1516)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((*(volatile uint32_t*)0xE000E018UL))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 327: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4591)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((*(volatile uint32_t*)0xE000E018UL))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 328: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3908)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((*(volatile uint32_t*)0xE000E018UL))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 329: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7346)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((*(volatile uint32_t*)0xE000E018UL))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 330: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1316)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((*(volatile uint32_t*)0xE000E018UL))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 331: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5543)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((*(volatile uint32_t*)0xE000E018UL))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 332: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4586)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((*(volatile uint32_t*)0xE000E018UL))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 333: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3455)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((*(volatile uint32_t*)0xE000E018UL))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 334: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7021)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((*(volatile uint32_t*)0xE000E018UL))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 335: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5773)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((*(volatile uint32_t*)0xE000E018UL))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 336: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3047)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((*(volatile uint32_t*)0xE000E018UL))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 337: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2459)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((*(volatile uint32_t*)0xE000E018UL))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 338: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4444)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((*(volatile uint32_t*)0xE000E018UL))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 339: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3487)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 340: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5316)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(0xff)<<24) |(0x00FF0000 & ((uint32_t)(0x80)<<16)) |(0x0000FF00 & ((uint32_t)(0x7f)<< 8)) |((uint8_t)(1))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 341: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6631)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(0xff)<<24) |(0x00FF0000 & ((uint32_t)(0x80)<<16)) |(0x0000FF00 & ((uint32_t)(0x7f)<< 8)) |((uint8_t)(1))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 342: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4454)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(0xff)<<24) |(0x00FF0000 & ((uint32_t)(0x80)<<16)) |(0x0000FF00 & ((uint32_t)(0x7f)<< 8)) |((uint8_t)(1))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 343: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6179)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(0xff)<<24) |(0x00FF0000 & ((uint32_t)(0x80)<<16)) |(0x0000FF00 & ((uint32_t)(0x7f)<< 8)) |((uint8_t)(1))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 344: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7208)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(0xff)<<24) |(0x00FF0000 & ((uint32_t)(0x80)<<16)) |(0x0000FF00 & ((uint32_t)(0x7f)<< 8)) |((uint8_t)(1))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 345: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4288)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(0xff)<<24) |(0x00FF0000 & ((uint32_t)(0x80)<<16)) |(0x0000FF00 & ((uint32_t)(0x7f)<< 8)) |((uint8_t)(1))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 346: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1486)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(0xff)<<24) |(0x00FF0000 & ((uint32_t)(0x80)<<16)) |(0x0000FF00 & ((uint32_t)(0x7f)<< 8)) |((uint8_t)(1))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 347: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3425)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(0xff)<<24) |(0x00FF0000 & ((uint32_t)(0x80)<<16)) |(0x0000FF00 & ((uint32_t)(0x7f)<< 8)) |((uint8_t)(1))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 348: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4601)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(0xff)<<24) |(0x00FF0000 & ((uint32_t)(0x80)<<16)) |(0x0000FF00 & ((uint32_t)(0x7f)<< 8)) |((uint8_t)(1))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 349: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((5900)<<16)|(5900))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(1) | ((uint32_t)(0x7fff)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(0x8000) | ((uint32_t)(0xffff)<<16)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 350: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((5248)<<16)|(5248))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(1) | ((uint32_t)(0x7fff)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(0x8000) | ((uint32_t)(0xffff)<<16)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 351: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((3281)<<16)|(3281))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(1) | ((uint32_t)(0x7fff)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(0x8000) | ((uint32_t)(0xffff)<<16)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 352: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((2836)<<16)|(2836))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((16)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(0x7fffffff)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(0x80000000)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(0xffffffff)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 353: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((2810)<<16)|(2810))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((16)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(0x7fffffff)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(0x80000000)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(0xffffffff)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 354: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((4886)<<16)|(4886))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(0xAAAAAAAA)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 355: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((6724)<<16)|(6724))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((32)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(1)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(0x7fffffffffffffff)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(0x7fffffffffffffff)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(0x8000000000000000)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(0x8000000000000000)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(0xffffffffffffffff)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(0xffffffffffffffff)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 356: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((3611)<<16)|(3611))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((32)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(1)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(0x7fffffffffffffff)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(0x7fffffffffffffff)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(0x8000000000000000)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(0x8000000000000000)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(0xffffffffffffffff)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(0xffffffffffffffff)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 357: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((1221)<<16)|(1221))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(0xAAAAAAAAAAAAAAAA)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(0xAAAAAAAAAAAAAAAA)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 358: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1655)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 359: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((7808)<<16)|(7808))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((1)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint8_t)(0xA1))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 360: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((2805)<<16)|(2805))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((2)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((0x0000FF00 & ((uint32_t)(0xA2)<< 8)) |((uint8_t)(0xA1))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 361: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((4623)<<16)|(4623))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((3)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((0x00FF0000 & ((uint32_t)(0xA3)<<16)) |(0x0000FF00 & ((uint32_t)(0xA2)<< 8)) |((uint8_t)(0xA1))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 362: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((3418)<<16)|(3418))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(0xA4)<<24) |(0x00FF0000 & ((uint32_t)(0xA3)<<16)) |(0x0000FF00 & ((uint32_t)(0xA2)<< 8)) |((uint8_t)(0xA1))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 363: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((2358)<<16)|(2358))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((5)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(0xA4)<<24) |(0x00FF0000 & ((uint32_t)(0xA3)<<16)) |(0x0000FF00 & ((uint32_t)(0xA2)<< 8)) |((uint8_t)(0xA1))); }while(0); do{ *TriceBufferWritePosition++ = (((uint8_t)(0xA5))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 364: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((7880)<<16)|(7880))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((6)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(0xA4)<<24) |(0x00FF0000 & ((uint32_t)(0xA3)<<16)) |(0x0000FF00 & ((uint32_t)(0xA2)<< 8)) |((uint8_t)(0xA1))); }while(0); do{ *TriceBufferWritePosition++ = ((0x0000FF00 & ((uint32_t)(0xA6)<< 8)) |((uint8_t)(0xA5))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 365: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((3839)<<16)|(3839))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((7)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(0xA4)<<24) |(0x00FF0000 & ((uint32_t)(0xA3)<<16)) |(0x0000FF00 & ((uint32_t)(0xA2)<< 8)) |((uint8_t)(0xA1))); }while(0); do{ *TriceBufferWritePosition++ = ((0x00FF0000 & ((uint32_t)(0xA7)<<16)) |(0x0000FF00 & ((uint32_t)(0xA6)<< 8)) |((uint8_t)(0xA5))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 366: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((2938)<<16)|(2938))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(0xA4)<<24) |(0x00FF0000 & ((uint32_t)(0xA3)<<16)) |(0x0000FF00 & ((uint32_t)(0xA2)<< 8)) |((uint8_t)(0xA1))); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(0xA8)<<24) |(0x00FF0000 & ((uint32_t)(0xA7)<<16)) |(0x0000FF00 & ((uint32_t)(0xA6)<< 8)) |((uint8_t)(0xA5))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 367: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((4327)<<16)|(4327))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((1)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint8_t)(-1))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 368: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((2787)<<16)|(2787))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((2)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((0x0000FF00 & ((uint32_t)(-2)<< 8)) |((uint8_t)(-1))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 369: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((3021)<<16)|(3021))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((3)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((0x00FF0000 & ((uint32_t)(-3)<<16)) |(0x0000FF00 & ((uint32_t)(-2)<< 8)) |((uint8_t)(-1))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 370: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((3142)<<16)|(3142))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-4)<<24) |(0x00FF0000 & ((uint32_t)(-3)<<16)) |(0x0000FF00 & ((uint32_t)(-2)<< 8)) |((uint8_t)(-1))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 371: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((1509)<<16)|(1509))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((5)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-4)<<24) |(0x00FF0000 & ((uint32_t)(-3)<<16)) |(0x0000FF00 & ((uint32_t)(-2)<< 8)) |((uint8_t)(-1))); }while(0); do{ *TriceBufferWritePosition++ = (((uint8_t)(-5))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 372: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((4666)<<16)|(4666))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((6)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-4)<<24) |(0x00FF0000 & ((uint32_t)(-3)<<16)) |(0x0000FF00 & ((uint32_t)(-2)<< 8)) |((uint8_t)(-1))); }while(0); do{ *TriceBufferWritePosition++ = ((0x0000FF00 & ((uint32_t)(-6)<< 8)) |((uint8_t)(-5))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 373: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((2524)<<16)|(2524))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((7)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-4)<<24) |(0x00FF0000 & ((uint32_t)(-3)<<16)) |(0x0000FF00 & ((uint32_t)(-2)<< 8)) |((uint8_t)(-1))); }while(0); do{ *TriceBufferWritePosition++ = ((0x00FF0000 & ((uint32_t)(-7)<<16)) |(0x0000FF00 & ((uint32_t)(-6)<< 8)) |((uint8_t)(-5))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 374: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((3933)<<16)|(3933))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-4)<<24) |(0x00FF0000 & ((uint32_t)(-3)<<16)) |(0x0000FF00 & ((uint32_t)(-2)<< 8)) |((uint8_t)(-1))); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-8)<<24) |(0x00FF0000 & ((uint32_t)(-7)<<16)) |(0x0000FF00 & ((uint32_t)(-6)<< 8)) |((uint8_t)(-5))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 375: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((2803)<<16)|(2803))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((1)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint8_t)(1))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 376: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((1765)<<16)|(1765))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((2)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((0x0000FF00 & ((uint32_t)(2)<< 8)) |((uint8_t)(1))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 377: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((4080)<<16)|(4080))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((3)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((0x00FF0000 & ((uint32_t)(3)<<16)) |(0x0000FF00 & ((uint32_t)(2)<< 8)) |((uint8_t)(1))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 378: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((3319)<<16)|(3319))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(4)<<24) |(0x00FF0000 & ((uint32_t)(3)<<16)) |(0x0000FF00 & ((uint32_t)(2)<< 8)) |((uint8_t)(1))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 379: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((2498)<<16)|(2498))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((5)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(4)<<24) |(0x00FF0000 & ((uint32_t)(3)<<16)) |(0x0000FF00 & ((uint32_t)(2)<< 8)) |((uint8_t)(1))); }while(0); do{ *TriceBufferWritePosition++ = (((uint8_t)(5))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 380: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((1792)<<16)|(1792))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((6)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(4)<<24) |(0x00FF0000 & ((uint32_t)(3)<<16)) |(0x0000FF00 & ((uint32_t)(2)<< 8)) |((uint8_t)(1))); }while(0); do{ *TriceBufferWritePosition++ = ((0x0000FF00 & ((uint32_t)(6)<< 8)) |((uint8_t)(5))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 381: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((4497)<<16)|(4497))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((7)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(4)<<24) |(0x00FF0000 & ((uint32_t)(3)<<16)) |(0x0000FF00 & ((uint32_t)(2)<< 8)) |((uint8_t)(1))); }while(0); do{ *TriceBufferWritePosition++ = ((0x00FF0000 & ((uint32_t)(7)<<16)) |(0x0000FF00 & ((uint32_t)(6)<< 8)) |((uint8_t)(5))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 382: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((1030)<<16)|(1030))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(4)<<24) |(0x00FF0000 & ((uint32_t)(3)<<16)) |(0x0000FF00 & ((uint32_t)(2)<< 8)) |((uint8_t)(1))); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(8)<<24) |(0x00FF0000 & ((uint32_t)(7)<<16)) |(0x0000FF00 & ((uint32_t)(6)<< 8)) |((uint8_t)(5))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 383: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((7491)<<16)|(7491))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((1)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint8_t)(201))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 384: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((2033)<<16)|(2033))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((2)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((0x0000FF00 & ((uint32_t)(202)<< 8)) |((uint8_t)(201))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 385: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((4167)<<16)|(4167))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((3)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((0x00FF0000 & ((uint32_t)(203)<<16)) |(0x0000FF00 & ((uint32_t)(202)<< 8)) |((uint8_t)(201))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 386: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((3027)<<16)|(3027))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(204)<<24) |(0x00FF0000 & ((uint32_t)(203)<<16)) |(0x0000FF00 & ((uint32_t)(202)<< 8)) |((uint8_t)(201))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 387: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((3556)<<16)|(3556))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((5)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(204)<<24) |(0x00FF0000 & ((uint32_t)(203)<<16)) |(0x0000FF00 & ((uint32_t)(202)<< 8)) |((uint8_t)(201))); }while(0); do{ *TriceBufferWritePosition++ = (((uint8_t)(205))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 388: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((1602)<<16)|(1602))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((6)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(204)<<24) |(0x00FF0000 & ((uint32_t)(203)<<16)) |(0x0000FF00 & ((uint32_t)(202)<< 8)) |((uint8_t)(201))); }while(0); do{ *TriceBufferWritePosition++ = ((0x0000FF00 & ((uint32_t)(206)<< 8)) |((uint8_t)(205))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 389: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((7037)<<16)|(7037))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((7)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(204)<<24) |(0x00FF0000 & ((uint32_t)(203)<<16)) |(0x0000FF00 & ((uint32_t)(202)<< 8)) |((uint8_t)(201))); }while(0); do{ *TriceBufferWritePosition++ = ((0x00FF0000 & ((uint32_t)(207)<<16)) |(0x0000FF00 & ((uint32_t)(206)<< 8)) |((uint8_t)(205))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 390: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((5745)<<16)|(5745))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(204)<<24) |(0x00FF0000 & ((uint32_t)(203)<<16)) |(0x0000FF00 & ((uint32_t)(202)<< 8)) |((uint8_t)(201))); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(208)<<24) |(0x00FF0000 & ((uint32_t)(207)<<16)) |(0x0000FF00 & ((uint32_t)(206)<< 8)) |((uint8_t)(205))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 391: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((5500)<<16)|(5500))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(0x81)<<24) |(0x00FF0000 & ((uint32_t)(0x81)<<16)) |(0x0000FF00 & ((uint32_t)(0x81)<< 8)) |((uint8_t)(0x81))); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(0x81)<<24) |(0x00FF0000 & ((uint32_t)(0x81)<<16)) |(0x0000FF00 & ((uint32_t)(0x81)<< 8)) |((uint8_t)(0x81))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 392: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7394)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 393: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((2902)<<16)|(2902))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((2)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-111)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 394: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((3091)<<16)|(3091))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-111) | ((uint32_t)(-222)<<16)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 395: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((7510)<<16)|(7510))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((6)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-111) | ((uint32_t)(-222)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-333)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 396: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((4728)<<16)|(4728))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-111) | ((uint32_t)(-222)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-333) | ((uint32_t)(-444)<<16)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 397: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((1335)<<16)|(1335))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(0x0123cafe)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 398: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((1727)<<16)|(1727))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-111)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 399: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((4596)<<16)|(4596))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-111)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-222)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 400: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((4577)<<16)|(4577))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-111)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-222)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 401: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((3456)<<16)|(3456))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((12)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-111)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-222)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-333)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 402: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((5312)<<16)|(5312))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((12)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-111)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-222)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-333)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 403: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((6859)<<16)|(6859))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((16)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-111)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-222)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-333)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-444)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 404: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((5439)<<16)|(5439))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((16)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-111)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-222)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-333)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-444)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 405: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((1165)<<16)|(1165))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-111)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-111)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 406: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((5308)<<16)|(5308))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((16)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-111)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-111)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-222)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-222)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 407: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((2982)<<16)|(2982))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((2)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(60001)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 408: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((4308)<<16)|(4308))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(60001) | ((uint32_t)(60002)<<16)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 409: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((7606)<<16)|(7606))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((6)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(60001) | ((uint32_t)(60002)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(60003)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 410: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((1786)<<16)|(1786))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(60001) | ((uint32_t)(60002)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(60003) | ((uint32_t)(60004)<<16)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 411: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((2495)<<16)|(2495))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(4000000001)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 412: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((5082)<<16)|(5082))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(4000000001)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(4000000002)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 413: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((3841)<<16)|(3841))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((12)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(4000000001)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(4000000002)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(4000000003)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 414: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((7625)<<16)|(7625))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((16)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(4000000001)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(4000000002)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(4000000003)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(4000000004)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 415: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((3903)<<16)|(3903))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-1)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 416: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((4882)<<16)|(4882))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((16)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-1)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-2)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 417: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((3959)<<16)|(3959))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-1)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 418: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((4034)<<16)|(4034))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((16)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-1)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-2)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 419: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6005)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 420: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((4159)<<16)|(4159))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 421: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((5090)<<16)|(5090))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 422: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((4048)<<16)|(4048))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 423: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((1790)<<16)|(1790))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 424: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((7275)<<16)|(7275))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 425: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((1264)<<16)|(1264))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 426: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((7829)<<16)|(7829))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 427: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((1744)<<16)|(1744))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 428: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((4116)<<16)|(4116))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 429: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((1555)<<16)|(1555))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 430: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((6014)<<16)|(6014))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 431: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((6472)<<16)|(6472))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 432: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((5605)<<16)|(5605))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 433: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((5268)<<16)|(5268))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 434: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((4445)<<16)|(4445))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 435: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((4132)<<16)|(4132))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 436: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((3029)<<16)|(3029))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 437: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((1893)<<16)|(1893))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 438: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((1375)<<16)|(1375))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };{ uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((2917)<<16)|(2917))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };{ uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((5877)<<16)|(5877))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 439: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((4632)<<16)|(4632))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };{ uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((7539)<<16)|(7539))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 440: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5420)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 441: {
                              s = "AAAAAAAAAAAA";
                              { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7741)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(strlen(s))); }while(0); } __set_PRIMASK(primaskstate); };
                              do { uint32_t ssiz = strlen( s ); do { uint32_t limit = 120 -8; uint32_t len_ = ssiz; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5176)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, s, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0); } while(0);
                          }
    break; case 446: {
            s = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBCCCCCCCC";


                          }
# 502 "../../../src/triceCheck.c"
    break; case 502: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3619)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(strlen(s))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 503: do { uint32_t ssiz = strlen( s ); do { uint32_t limit = 120 -8; uint32_t len_ = ssiz; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1235)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, s, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0); } while(0);
    break; case 504: {
                              int i = 15;
                              { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2639)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
                              { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3430)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(i)); }while(0); } __set_PRIMASK(primaskstate); };
                              { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7801)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(i)); }while(0); } __set_PRIMASK(primaskstate); };
                              { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5696)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(i)); }while(0); } __set_PRIMASK(primaskstate); };
                              { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3775)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(i)); }while(0); } __set_PRIMASK(primaskstate); };
                              { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3043)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(i)); }while(0); } __set_PRIMASK(primaskstate); };
                              { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6229)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(i)); }while(0); } __set_PRIMASK(primaskstate); };
                              { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3192)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(i)); }while(0); } __set_PRIMASK(primaskstate); };
                              { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2570)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(i)); }while(0); } __set_PRIMASK(primaskstate); };
                              { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1470)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(i)); }while(0); } __set_PRIMASK(primaskstate); };
                              { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1873)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(i)); }while(0); } __set_PRIMASK(primaskstate); };
                          }
    break; case 518: {
                              char a = 'A';
                              { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6720)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
                              { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7669)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(a)); }while(0); } __set_PRIMASK(primaskstate); };
                              { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2970)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(a)); }while(0); } __set_PRIMASK(primaskstate); };
                              { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6044)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(a)); }while(0); } __set_PRIMASK(primaskstate); };
                              { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1964)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(a)); }while(0); } __set_PRIMASK(primaskstate); };
                              { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2567)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
                              { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7736)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(0)); }while(0); } __set_PRIMASK(primaskstate); };
                              { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4795)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
                          }
    break; case 529: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7074)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((1)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint8_t)(0x84))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 530: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4532)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((2)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(0x1234)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 531: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1536)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(0xaabbccdd)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 532: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1507)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(0x1122334455667788)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(0x1122334455667788)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 533: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1717)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(0xaabbccdd)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 534: {
                                float f = (float)123.456;
                                { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4115)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
                                { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3554)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(f))); }while(0); } __set_PRIMASK(primaskstate); };
                                { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2554)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(f))); }while(0); } __set_PRIMASK(primaskstate); };
                                { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7773)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(f))); }while(0); } __set_PRIMASK(primaskstate); };
                                { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3790)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(f))); }while(0); } __set_PRIMASK(primaskstate); };
                                { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3340)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(f))); }while(0); } __set_PRIMASK(primaskstate); };
                          }
    break; case 543: {
                                double d = 123.456;
                                { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7188)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
                                { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7792)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(d))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(d))>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
                                { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6452)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(d))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(d))>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
                                { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3920)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(d))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(d))>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
                                { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4504)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(d))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(d))>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
                                { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2968)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(d))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(d))>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
                          }
    break; case 552: {
                              s = "café";
                              { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1972)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
                              do { uint32_t ssiz = strlen( s ); do { uint32_t limit = 120 -8; uint32_t len_ = ssiz; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4139)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, s, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0); } while(0);
                              do { uint32_t ssiz = strlen( s ); do { uint32_t limit = 120 -8; uint32_t len_ = ssiz; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6473)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, s, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0); } while(0);
                              do { uint32_t ssiz = strlen( s ); do { uint32_t limit = 120 -8; uint32_t len_ = ssiz; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7906)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, s, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0); } while(0);
                              do { uint32_t ssiz = strlen( s ); do { uint32_t limit = 120 -8; uint32_t len_ = ssiz; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3777)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, s, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0); } while(0);
                              do { uint32_t ssiz = strlen( s ); do { uint32_t limit = 120 -8; uint32_t len_ = ssiz; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1033)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, s, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0); } while(0);
                              do { uint32_t ssiz = strlen( s ); do { uint32_t limit = 120 -8; uint32_t len_ = ssiz; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4405)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, s, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0); } while(0);
                          }
    break; case 562: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1630)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 563: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((4024)<<16)|(4024))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((1)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint8_t)(-1))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 564: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((6003)<<16)|(6003))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((2)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((0x0000FF00 & ((uint32_t)(-2)<< 8)) |((uint8_t)(-1))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 565: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((1360)<<16)|(1360))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((3)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((0x00FF0000 & ((uint32_t)(-3)<<16)) |(0x0000FF00 & ((uint32_t)(-2)<< 8)) |((uint8_t)(-1))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 566: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((7038)<<16)|(7038))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-4)<<24) |(0x00FF0000 & ((uint32_t)(-3)<<16)) |(0x0000FF00 & ((uint32_t)(-2)<< 8)) |((uint8_t)(-1))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 567: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((7053)<<16)|(7053))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((5)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-4)<<24) |(0x00FF0000 & ((uint32_t)(-3)<<16)) |(0x0000FF00 & ((uint32_t)(-2)<< 8)) |((uint8_t)(-1))); }while(0); do{ *TriceBufferWritePosition++ = (((uint8_t)(-5))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 568: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((3060)<<16)|(3060))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((6)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-4)<<24) |(0x00FF0000 & ((uint32_t)(-3)<<16)) |(0x0000FF00 & ((uint32_t)(-2)<< 8)) |((uint8_t)(-1))); }while(0); do{ *TriceBufferWritePosition++ = ((0x0000FF00 & ((uint32_t)(-6)<< 8)) |((uint8_t)(-5))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 569: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((1308)<<16)|(1308))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((7)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-4)<<24) |(0x00FF0000 & ((uint32_t)(-3)<<16)) |(0x0000FF00 & ((uint32_t)(-2)<< 8)) |((uint8_t)(-1))); }while(0); do{ *TriceBufferWritePosition++ = ((0x00FF0000 & ((uint32_t)(-7)<<16)) |(0x0000FF00 & ((uint32_t)(-6)<< 8)) |((uint8_t)(-5))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 570: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((1363)<<16)|(1363))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-4)<<24) |(0x00FF0000 & ((uint32_t)(-3)<<16)) |(0x0000FF00 & ((uint32_t)(-2)<< 8)) |((uint8_t)(-1))); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-8)<<24) |(0x00FF0000 & ((uint32_t)(-7)<<16)) |(0x0000FF00 & ((uint32_t)(-6)<< 8)) |((uint8_t)(-5))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 571: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((5413)<<16)|(5413))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((9)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-4)<<24) |(0x00FF0000 & ((uint32_t)(-3)<<16)) |(0x0000FF00 & ((uint32_t)(-2)<< 8)) |((uint8_t)(-1))); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-8)<<24) |(0x00FF0000 & ((uint32_t)(-7)<<16)) |(0x0000FF00 & ((uint32_t)(-6)<< 8)) |((uint8_t)(-5))); }while(0); do{ *TriceBufferWritePosition++ = (((uint8_t)(-9))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 572: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((2100)<<16)|(2100))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((10)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-4)<<24) |(0x00FF0000 & ((uint32_t)(-3)<<16)) |(0x0000FF00 & ((uint32_t)(-2)<< 8)) |((uint8_t)(-1))); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-8)<<24) |(0x00FF0000 & ((uint32_t)(-7)<<16)) |(0x0000FF00 & ((uint32_t)(-6)<< 8)) |((uint8_t)(-5))); }while(0); do{ *TriceBufferWritePosition++ = ((0x0000FF00 & ((uint32_t)(-10)<< 8)) |((uint8_t)(-9))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 573: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((1424)<<16)|(1424))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((11)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-4)<<24) |(0x00FF0000 & ((uint32_t)(-3)<<16)) |(0x0000FF00 & ((uint32_t)(-2)<< 8)) |((uint8_t)(-1))); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-8)<<24) |(0x00FF0000 & ((uint32_t)(-7)<<16)) |(0x0000FF00 & ((uint32_t)(-6)<< 8)) |((uint8_t)(-5))); }while(0); do{ *TriceBufferWritePosition++ = ((0x00FF0000 & ((uint32_t)(-11)<<16))|(0x0000FF00 & ((uint32_t)(-10)<< 8)) |((uint8_t)(-9))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 574: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((6831)<<16)|(6831))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((12)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-4)<<24) |(0x00FF0000 & ((uint32_t)(-3)<<16)) |(0x0000FF00 & ((uint32_t)(-2)<< 8)) |((uint8_t)(-1))); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-8)<<24) |(0x00FF0000 & ((uint32_t)(-7)<<16)) |(0x0000FF00 & ((uint32_t)(-6)<< 8)) |((uint8_t)(-5))); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-12)<<24)|(0x00FF0000 & ((uint32_t)(-11)<<16))|(0x0000FF00 & ((uint32_t)(-10)<< 8)) |((uint8_t)(-9))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 575: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((6042)<<16)|(6042))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((9)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-3)<<24) |(0x00FF0000 & ((uint32_t)(-3)<<16)) |(0x0000FF00 & ((uint32_t)(-3)<< 8)) |((uint8_t)(-3))); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-3)<<24) |(0x00FF0000 & ((uint32_t)(-3)<<16)) |(0x0000FF00 & ((uint32_t)(-3)<< 8)) |((uint8_t)(-3))); }while(0); do{ *TriceBufferWritePosition++ = (((uint8_t)(-3))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 576: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((1287)<<16)|(1287))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((1)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint8_t)(-1))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 577: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((5600)<<16)|(5600))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((2)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((0x0000FF00 & ((uint32_t)(-2)<< 8)) |((uint8_t)(-1))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 578: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((3422)<<16)|(3422))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((3)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((0x00FF0000 & ((uint32_t)(-3)<<16)) |(0x0000FF00 & ((uint32_t)(-2)<< 8)) |((uint8_t)(-1))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 579: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((3603)<<16)|(3603))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-4)<<24) |(0x00FF0000 & ((uint32_t)(-3)<<16)) |(0x0000FF00 & ((uint32_t)(-2)<< 8)) |((uint8_t)(-1))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 580: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((3630)<<16)|(3630))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((5)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-4)<<24) |(0x00FF0000 & ((uint32_t)(-3)<<16)) |(0x0000FF00 & ((uint32_t)(-2)<< 8)) |((uint8_t)(-1))); }while(0); do{ *TriceBufferWritePosition++ = (((uint8_t)(-5))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 581: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((4221)<<16)|(4221))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((6)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-4)<<24) |(0x00FF0000 & ((uint32_t)(-3)<<16)) |(0x0000FF00 & ((uint32_t)(-2)<< 8)) |((uint8_t)(-1))); }while(0); do{ *TriceBufferWritePosition++ = ((0x0000FF00 & ((uint32_t)(-6)<< 8)) |((uint8_t)(-5))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 582: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((1293)<<16)|(1293))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((7)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-4)<<24) |(0x00FF0000 & ((uint32_t)(-3)<<16)) |(0x0000FF00 & ((uint32_t)(-2)<< 8)) |((uint8_t)(-1))); }while(0); do{ *TriceBufferWritePosition++ = ((0x00FF0000 & ((uint32_t)(-7)<<16)) |(0x0000FF00 & ((uint32_t)(-6)<< 8)) |((uint8_t)(-5))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 583: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((7949)<<16)|(7949))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-4)<<24) |(0x00FF0000 & ((uint32_t)(-3)<<16)) |(0x0000FF00 & ((uint32_t)(-2)<< 8)) |((uint8_t)(-1))); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-8)<<24) |(0x00FF0000 & ((uint32_t)(-7)<<16)) |(0x0000FF00 & ((uint32_t)(-6)<< 8)) |((uint8_t)(-5))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 584: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((5405)<<16)|(5405))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((9)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-4)<<24) |(0x00FF0000 & ((uint32_t)(-3)<<16)) |(0x0000FF00 & ((uint32_t)(-2)<< 8)) |((uint8_t)(-1))); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-8)<<24) |(0x00FF0000 & ((uint32_t)(-7)<<16)) |(0x0000FF00 & ((uint32_t)(-6)<< 8)) |((uint8_t)(-5))); }while(0); do{ *TriceBufferWritePosition++ = (((uint8_t)(-9))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 585: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((3609)<<16)|(3609))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((10)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-4)<<24) |(0x00FF0000 & ((uint32_t)(-3)<<16)) |(0x0000FF00 & ((uint32_t)(-2)<< 8)) |((uint8_t)(-1))); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-8)<<24) |(0x00FF0000 & ((uint32_t)(-7)<<16)) |(0x0000FF00 & ((uint32_t)(-6)<< 8)) |((uint8_t)(-5))); }while(0); do{ *TriceBufferWritePosition++ = ((0x0000FF00 & ((uint32_t)(-10)<< 8)) |((uint8_t)(-9))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 586: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((4328)<<16)|(4328))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((11)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-4)<<24) |(0x00FF0000 & ((uint32_t)(-3)<<16)) |(0x0000FF00 & ((uint32_t)(-2)<< 8)) |((uint8_t)(-1))); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-8)<<24) |(0x00FF0000 & ((uint32_t)(-7)<<16)) |(0x0000FF00 & ((uint32_t)(-6)<< 8)) |((uint8_t)(-5))); }while(0); do{ *TriceBufferWritePosition++ = ((0x00FF0000 & ((uint32_t)(-11)<<16))|(0x0000FF00 & ((uint32_t)(-10)<< 8)) |((uint8_t)(-9))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 587: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((6506)<<16)|(6506))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((12)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-4)<<24) |(0x00FF0000 & ((uint32_t)(-3)<<16)) |(0x0000FF00 & ((uint32_t)(-2)<< 8)) |((uint8_t)(-1))); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-8)<<24) |(0x00FF0000 & ((uint32_t)(-7)<<16)) |(0x0000FF00 & ((uint32_t)(-6)<< 8)) |((uint8_t)(-5))); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-12)<<24)|(0x00FF0000 & ((uint32_t)(-11)<<16))|(0x0000FF00 & ((uint32_t)(-10)<< 8)) |((uint8_t)(-9))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 588: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((4744)<<16)|(4744))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((9)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-3)<<24) |(0x00FF0000 & ((uint32_t)(-3)<<16)) |(0x0000FF00 & ((uint32_t)(-3)<< 8)) |((uint8_t)(-3))); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-3)<<24) |(0x00FF0000 & ((uint32_t)(-3)<<16)) |(0x0000FF00 & ((uint32_t)(-3)<< 8)) |((uint8_t)(-3))); }while(0); do{ *TriceBufferWritePosition++ = (((uint8_t)(-3))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 589: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((3353)<<16)|(3353))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((9)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-3)<<24) |(0x00FF0000 & ((uint32_t)(-3)<<16)) |(0x0000FF00 & ((uint32_t)(-3)<< 8)) |((uint8_t)(-3))); }while(0); do{ *TriceBufferWritePosition++ = (((uint32_t)(-3)<<24) |(0x00FF0000 & ((uint32_t)(-3)<<16)) |(0x0000FF00 & ((uint32_t)(-3)<< 8)) |((uint8_t)(-3))); }while(0); do{ *TriceBufferWritePosition++ = (((uint8_t)(-3))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 590: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6236)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 591: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((3808)<<16)|(3808))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((2)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 592: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((5739)<<16)|(5739))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 593: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((7710)<<16)|(7710))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((6)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-3)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 594: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((3864)<<16)|(3864))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-3) | ((uint32_t)(-4)<<16)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 595: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((6936)<<16)|(6936))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((10)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-3) | ((uint32_t)(-4)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-5)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 596: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((4792)<<16)|(4792))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((12)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-3) | ((uint32_t)(-4)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-5) | ((uint32_t)(-6)<<16)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 597: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((2019)<<16)|(2019))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((14)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-3) | ((uint32_t)(-4)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-5) | ((uint32_t)(-6)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-7)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 598: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((5896)<<16)|(5896))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((16)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-3) | ((uint32_t)(-4)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-5) | ((uint32_t)(-6)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-7) | ((uint32_t)(-8)<<16)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 599: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((6585)<<16)|(6585))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((18)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-3) | ((uint32_t)(-4)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-5) | ((uint32_t)(-6)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-7) | ((uint32_t)(-8)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-9)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 600: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((5295)<<16)|(5295))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((20)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-3) | ((uint32_t)(-4)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-5) | ((uint32_t)(-6)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-7) | ((uint32_t)(-8)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-9) | ((uint32_t)(-10)<<16)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 601: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((2157)<<16)|(2157))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((22)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-3) | ((uint32_t)(-4)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-5) | ((uint32_t)(-6)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-7) | ((uint32_t)(-8)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-9) | ((uint32_t)(-10)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-11)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 602: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((6057)<<16)|(6057))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((24)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-3) | ((uint32_t)(-4)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-5) | ((uint32_t)(-6)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-7) | ((uint32_t)(-8)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-9) | ((uint32_t)(-10)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-11)| ((uint32_t)(-12)<<16)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 603: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((3417)<<16)|(3417))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((18)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(3) | ((uint32_t)(3)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(3) | ((uint32_t)(3)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(3) | ((uint32_t)(3)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(3) | ((uint32_t)(3)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(3)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 604: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((7036)<<16)|(7036))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((2)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 605: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((4296)<<16)|(4296))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 606: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((6391)<<16)|(6391))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((6)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-3)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 607: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((1661)<<16)|(1661))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-3) | ((uint32_t)(-4)<<16)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 608: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((3800)<<16)|(3800))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((10)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-3) | ((uint32_t)(-4)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-5)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 609: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((1517)<<16)|(1517))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((12)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-3) | ((uint32_t)(-4)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-5) | ((uint32_t)(-6)<<16)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 610: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((1963)<<16)|(1963))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((14)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-3) | ((uint32_t)(-4)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-5) | ((uint32_t)(-6)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-7)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 611: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((7621)<<16)|(7621))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((16)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-3) | ((uint32_t)(-4)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-5) | ((uint32_t)(-6)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-7) | ((uint32_t)(-8)<<16)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 612: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((7688)<<16)|(7688))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((18)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-3) | ((uint32_t)(-4)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-5) | ((uint32_t)(-6)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-7) | ((uint32_t)(-8)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-9)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 613: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((3950)<<16)|(3950))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((20)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-3) | ((uint32_t)(-4)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-5) | ((uint32_t)(-6)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-7) | ((uint32_t)(-8)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-9) | ((uint32_t)(-10)<<16)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 614: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((5743)<<16)|(5743))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((22)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-3) | ((uint32_t)(-4)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-5) | ((uint32_t)(-6)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-7) | ((uint32_t)(-8)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-9) | ((uint32_t)(-10)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-11)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 615: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((1814)<<16)|(1814))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((24)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1) | ((uint32_t)(-2)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-3) | ((uint32_t)(-4)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-5) | ((uint32_t)(-6)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-7) | ((uint32_t)(-8)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-9) | ((uint32_t)(-10)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-11)| ((uint32_t)(-12)<<16)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 616: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((1374)<<16)|(1374))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((18)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(3) | ((uint32_t)(3)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(3) | ((uint32_t)(3)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(3) | ((uint32_t)(3)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(3) | ((uint32_t)(3)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(3)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 617: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7422)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 618: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((2468)<<16)|(2468))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 619: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((3673)<<16)|(3673))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 620: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((6899)<<16)|(6899))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((12)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 621: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((7262)<<16)|(7262))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((16)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 622: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((3178)<<16)|(3178))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((20)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 623: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((6903)<<16)|(6903))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((24)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-6)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 624: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((5657)<<16)|(5657))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((28)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-7)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 625: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((3812)<<16)|(3812))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((32)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-8)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 626: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((4378)<<16)|(4378))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((36)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-8)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-9)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 627: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((2074)<<16)|(2074))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((40)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-8)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-9)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-10)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 628: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((1116)<<16)|(1116))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((44)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-8)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-9)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-10)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-11)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 629: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((7701)<<16)|(7701))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((48)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-8)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-9)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-10)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-11)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-12)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 630: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((2108)<<16)|(2108))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((48)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(3.14159))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(3.14159))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(3.14159))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 631: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((7894)<<16)|(7894))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 632: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((5612)<<16)|(5612))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 633: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((2072)<<16)|(2072))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((12)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 634: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((1499)<<16)|(1499))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((16)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 635: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((4587)<<16)|(4587))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((20)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 636: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((5830)<<16)|(5830))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((24)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-6)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 637: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((7732)<<16)|(7732))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((28)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-7)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 638: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((4772)<<16)|(4772))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((32)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-8)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 639: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((7577)<<16)|(7577))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((36)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-8)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-9)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 640: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((2414)<<16)|(2414))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((40)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-8)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-9)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-10)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 641: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((4996)<<16)|(4996))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((44)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-8)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-9)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-10)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-11)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 642: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((7143)<<16)|(7143))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((48)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-8)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-9)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-10)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-11)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-12)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 643: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((5141)<<16)|(5141))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((48)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(3.14159))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(3.14159))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(3.14159))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 644: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((4629)<<16)|(4629))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((48)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(3.14159))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(3.14159))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(3.14159))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 645: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6289)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 646: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((6544)<<16)|(6544))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-1)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 647: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((1969)<<16)|(1969))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((16)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-1)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-2)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 648: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((7907)<<16)|(7907))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((24)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-1)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-2)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-3)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 649: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((7692)<<16)|(7692))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((32)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-1)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-2)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-4)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 650: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((4822)<<16)|(4822))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((40)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-1)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-2)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-4)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-5)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 651: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((6175)<<16)|(6175))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((48)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-1)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-2)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-4)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-5)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-6)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 652: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((7713)<<16)|(7713))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((56)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-1)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-2)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-4)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-5)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-6)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-7)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 653: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((7466)<<16)|(7466))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((64)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-1)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-2)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-4)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-5)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-6)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-7)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-8)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-8)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 654: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((2004)<<16)|(2004))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((72)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-1)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-2)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-4)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-5)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-6)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-7)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-8)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-8)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-9)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-9)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 655: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((7210)<<16)|(7210))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((80)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-1)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-2)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-4)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-5)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-6)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-7)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-8)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-8)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-9)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-9)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-10)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-10)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 656: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((1853)<<16)|(1853))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((88)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-1)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-2)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-4)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-5)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-6)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-7)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-8)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-8)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-9)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-9)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-10)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-10)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-11)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-11)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 657: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((7598)<<16)|(7598))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((96)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-1)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-2)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-4)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-5)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-6)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-7)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-8)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-8)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-9)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-9)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-10)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-10)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-11)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-11)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-12)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-12)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 658: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((3301)<<16)|(3301))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((96)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(3.14159))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(3.14159))>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(3.14159))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(3.14159))>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(3.14159))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(3.14159))>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 659: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((1177)<<16)|(1177))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-1)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 660: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((1711)<<16)|(1711))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((16)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-1)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-2)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 661: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((5366)<<16)|(5366))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((24)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-1)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-2)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-3)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 662: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((5684)<<16)|(5684))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((32)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-1)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-2)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-4)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 663: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((6946)<<16)|(6946))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((40)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-1)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-2)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-4)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-5)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 664: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((3996)<<16)|(3996))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((48)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-1)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-2)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-4)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-5)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-6)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 665: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((3656)<<16)|(3656))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((56)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-1)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-2)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-4)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-5)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-6)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-7)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 666: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((4247)<<16)|(4247))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((64)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-1)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-2)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-4)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-5)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-6)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-7)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-8)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-8)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 667: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((6178)<<16)|(6178))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((72)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-1)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-2)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-4)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-5)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-6)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-7)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-8)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-8)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-9)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-9)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 668: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((6036)<<16)|(6036))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((80)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-1)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-2)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-4)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-5)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-6)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-7)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-8)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-8)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-9)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-9)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-10)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-10)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 669: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((7443)<<16)|(7443))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((88)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-1)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-2)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-4)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-5)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-6)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-7)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-8)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-8)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-9)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-9)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-10)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-10)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-11)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-11)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 670: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((2992)<<16)|(2992))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((96)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-1)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-2)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-4)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-5)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-6)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-6)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-7)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-7)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-8)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-8)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-9)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-9)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-10)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-10)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-11)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-11)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(-12)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-12)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 671: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((2967)<<16)|(2967))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; do{ uint16_t v = ((96)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(3)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(3)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(3.14159))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(3.14159))>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(3.14159))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(3.14159))>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(3.14159))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(3.14159))>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 672: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7520)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 673: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4605)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 674: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3606)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 675: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7782)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 676: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6833)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 677: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6213)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 678: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4432)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(-x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 679: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2812)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(-x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 680: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4013)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(-x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 681: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1538)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(-x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 682: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1983)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(-x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 683: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7886)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 684: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3683)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 685: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4633)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 686: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4261)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 687: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6915)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 688: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3701)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(-x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 689: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2996)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(-x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 690: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2370)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(-x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 691: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5078)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(-x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 692: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6084)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(-x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 693: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7483)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 694: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4069)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 695: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3925)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 696: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5119)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 697: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5261)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 698: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6172)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(-x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 699: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5851)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(-x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 700: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1586)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(-x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 701: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6313)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(-x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 702: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7112)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(-x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 703: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3392)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 704: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1311)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 705: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2784)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 706: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5212)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 707: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4686)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 708: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7390)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 709: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2156)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 710: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2398)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 711: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5737)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 712: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7337)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 713: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7454)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 714: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6132)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 715: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3584)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 716: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7902)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 717: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6447)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 718: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4186)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 719: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1784)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 720: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1151)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 721: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7325)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 722: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7423)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(y))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(y))>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 723: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3911)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(y))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(y))>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 724: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2683)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(y))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(y))>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 725: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3795)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(y))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(y))>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 726: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2309)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(y))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(y))>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 727: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3402)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(y))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(y))>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 728: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2246)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(y))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(y))>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 729: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3502)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(y))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(y))>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 730: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3522)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(y))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(y))>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 731: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5373)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(y))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(y))>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 732: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6033)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(y))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(y))>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 733: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3952)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(y))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(y))>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 734: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6569)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 735: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3801)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(-x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 736: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6756)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(-x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 737: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4536)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(-x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 738: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1770)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(-x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 739: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5290)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(-x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 740: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1890)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(-x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 741: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5723)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(-y))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(-y))>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 742: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7252)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(-y))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(-y))>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 743: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4535)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(-y))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(-y))>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 744: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5874)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(-y))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(-y))>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 745: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1907)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(-y))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(-y))>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 746: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5738)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(-y))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(-y))>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 747: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4366)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 748: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2658)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 749: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4865)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 750: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7560)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 751: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5902)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 752: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1164)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 753: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4932)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 754: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6113)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(y))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(y))>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 755: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5603)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(y))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(y))>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 756: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6598)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(y))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(y))>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 757: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3097)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(y))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(y))>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 758: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2154)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(y))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(y))>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 759: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4949)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(y))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(y))>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 760: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7350)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 761: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4362)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((16)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(FloatToInt32(x))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(x))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(x))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 762: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7353)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((16)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((int32_t)(x))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(x))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(x))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 763: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2105)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((16)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(x)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(x))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(x))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 764: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1841)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((16)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(FloatToInt32(-x))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(-x))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(-x))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(-x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 765: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7146)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((16)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((int32_t)(-x))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(-x))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(-x))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(-x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 766: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7026)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((16)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-x)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(-x))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(-x))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(-x))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 767: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5593)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((32)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(DoubleToInt64(y))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(DoubleToInt64(y))>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(y))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(y))>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(y))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(y))>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(y))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(y))>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 768: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4835)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((32)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((int64_t)(y))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)((int64_t)(y))>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(y))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(y))>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(y))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(y))>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(y))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(y))>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 769: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3987)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((32)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(y)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(y)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(y))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(y))>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(y))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(y))>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(y))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(y))>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 770: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5698)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((32)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(DoubleToInt64(-y))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(DoubleToInt64(-y))>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(-y))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(-y))>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(-y))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(-y))>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(-y))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(-y))>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 771: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4801)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((32)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((int64_t)(-y))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)((int64_t)(-y))>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(-y))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(-y))>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(-y))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(-y))>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(-y))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(-y))>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 772: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3306)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((32)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-y)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(-y)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(-y))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(-y))>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(-y))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(-y))>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(aDouble(-y))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(aDouble(-y))>>32)); }while(0);; } __set_PRIMASK(primaskstate); };
    break; case 773: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6124)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 774: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1857)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((1)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint8_t)(0))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 775: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1495)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((1)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint8_t)(2))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 776: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4893)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((1)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint8_t)(-1))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 777: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4235)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((1)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint8_t)(-1))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 778: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7395)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((1)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint8_t)(-1))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 779: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7189)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((1)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint8_t)(-1))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 780: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6875)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((1)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint8_t)(-1))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 781: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7752)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((1)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint8_t)(-1))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 782: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1294)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((1)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint8_t)(-1))); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 783: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1220)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((2)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(0)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 784: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5802)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((2)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(2)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 785: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6024)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((2)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 786: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4509)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((2)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 787: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4277)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((2)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 788: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2415)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((2)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 789: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5288)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((2)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(-1)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 790: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1057)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(0)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 791: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5876)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(2)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 792: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6159)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-1)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 793: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4896)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-2)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 794: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2606)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-3)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 795: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7421)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-4)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 796: { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3963)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(-5)); }while(0); } __set_PRIMASK(primaskstate); };
    break; case 797: do { uint32_t limit = 120 -8; uint32_t len_ = 0; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4770)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, A, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0);
    break; case 798: do { uint32_t limit = 120 -8; uint32_t len_ = 1; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4770)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, A, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0);
    break; case 799: do { uint32_t limit = 120 -8; uint32_t len_ = 2; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4770)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, A, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0);
    break; case 800: do { uint32_t limit = 120 -8; uint32_t len_ = 3; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4770)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, A, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0);
    break; case 801: do { uint32_t limit = 120 -8; uint32_t len_ = 4; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4770)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, A, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0);
    break; case 802: do { uint32_t limit = 120 -8; uint32_t len_ = 5; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4770)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, A, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0);
    break; case 803: do { uint32_t limit = 120 -8; uint32_t len_ = 6; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4770)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, A, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0);
    break; case 804: do { uint32_t limit = 120 -8; uint32_t len_ = 7; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4770)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, A, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0);
    break; case 805: do { uint32_t limit = 120 -8; uint32_t len_ = 8; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4770)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, A, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0);
    break; case 806: do { uint32_t limit = 120 -8; uint32_t len_ = 9; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4770)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, A, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0);
    break; case 807: do { uint32_t limit = 120 -8; uint32_t len_ = 10; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4770)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, A, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0);
    break; case 808: do { uint32_t limit = 120 -8; uint32_t len_ = 11; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4770)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, A, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0);
    break; case 809: do { uint32_t limit = 120 -8; uint32_t len_ = 12; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4770)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, A, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0);
    break; case 810: do { uint32_t limit = 120 -8; uint32_t len_ = 13; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4770)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, A, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0);
    break; case 811: do { uint32_t limit = 120 -8; uint32_t len_ = 14; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4770)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, A, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0);
    break; case 812: do { uint32_t limit = 120 -8; uint32_t len_ = 15; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4770)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, A, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0);
    break; case 813: do { uint32_t limit = 120 -8; uint32_t len_ = 16; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4770)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, A, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0);
    break; case 814: do { uint32_t limit = 120 -8; uint32_t len_ = 17; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4770)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, A, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0);
    break; case 815: do { uint32_t limit = 120 -8; uint32_t len_ = 18; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4770)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, A, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0);
    break; case 816: do { uint32_t limit = 120 -8; uint32_t len_ = 19; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4770)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, A, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0);
    break; case 817: do { uint32_t limit = 120 -8; uint32_t len_ = 120; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4770)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, A, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0);
    break; case 818: do { uint32_t limit = 120 -8; uint32_t len_ = 121; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4770)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, A, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0);
    break; case 819: do { uint32_t limit = 120 -8; uint32_t len_ = 122; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4770)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, A, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0);
    break; case 820: do { uint32_t limit = 120 -8; uint32_t len_ = 123; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4770)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, A, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0);
    break; case 821: do { uint32_t limit = 120 -8; uint32_t len_ = 124; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4770)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, A, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0);
    break; case 822: do { uint32_t limit = 120 -8; uint32_t len_ = 125; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4770)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, A, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0);
    break; case 823: do { uint32_t limit = 120 -8; uint32_t len_ = 126; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4770)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, A, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0);
    break; case 824: do { uint32_t limit = 120 -8; uint32_t len_ = 127; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4770)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, A, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0);
    break; case 825: do { uint32_t limit = 120 -8; uint32_t len_ = 128; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4770)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, A, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0);
    break; case 826: do { uint32_t limit = 120 -8; uint32_t len_ = 129; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4770)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, A, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0);
    break; case 827: exampleOfManualJSONencoding();
    break; case 828: exampleOfBuffersAndFunctions();
    break; case 829: exampleOfManualSerialization();
    break; case 830:
# 850 "../../../src/triceCheck.c"
    break; case 850: showTriceDepth();
        break;
        default:
        break;
    }
}
# 894 "../../../src/triceCheck.c"
typedef struct{
    float z;
    uint16_t u;
      int8_t s;
    char* addr;
    uint32_t x;
     int32_t y;
    char names[3][5];

    uint64_t bitmask;
} Tryout_t;



static int serializeTryout( char* dst, Tryout_t const * src ){
    char * p = dst;

    do{ char* n = "z"; int size = sizeof( src->z ); memcpy( p, &(src->z), size ); p += size; do { uint32_t ssiz = strlen( n ); do { uint32_t limit = 120 -8; uint32_t len_ = ssiz; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5135)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, n, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0); } while(0); { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6144)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(size)); }while(0); } __set_PRIMASK(primaskstate); };}while(0);
    do{ char* n = "u"; int size = sizeof( src->u ); memcpy( p, &(src->u), size ); p += size; do { uint32_t ssiz = strlen( n ); do { uint32_t limit = 120 -8; uint32_t len_ = ssiz; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5135)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, n, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0); } while(0); { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6144)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(size)); }while(0); } __set_PRIMASK(primaskstate); };}while(0);
    do{ char* n = "s"; int size = sizeof( src->s ); memcpy( p, &(src->s), size ); p += size; do { uint32_t ssiz = strlen( n ); do { uint32_t limit = 120 -8; uint32_t len_ = ssiz; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5135)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, n, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0); } while(0); { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6144)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(size)); }while(0); } __set_PRIMASK(primaskstate); };}while(0);
    do{ char* n = "addr"; int size = sizeof( src->addr ); memcpy( p, &(src->addr), size ); p += size; do { uint32_t ssiz = strlen( n ); do { uint32_t limit = 120 -8; uint32_t len_ = ssiz; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5135)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, n, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0); } while(0); { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6144)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(size)); }while(0); } __set_PRIMASK(primaskstate); };}while(0);
    do{ char* n = "x"; int size = sizeof( src->x ); memcpy( p, &(src->x), size ); p += size; do { uint32_t ssiz = strlen( n ); do { uint32_t limit = 120 -8; uint32_t len_ = ssiz; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5135)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, n, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0); } while(0); { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6144)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(size)); }while(0); } __set_PRIMASK(primaskstate); };}while(0);
    do{ char* n = "y"; int size = sizeof( src->y ); memcpy( p, &(src->y), size ); p += size; do { uint32_t ssiz = strlen( n ); do { uint32_t limit = 120 -8; uint32_t len_ = ssiz; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5135)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, n, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0); } while(0); { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6144)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(size)); }while(0); } __set_PRIMASK(primaskstate); };}while(0);
    do{ char* n = "names"; int size = sizeof( src->names ); memcpy( p, &(src->names), size ); p += size; do { uint32_t ssiz = strlen( n ); do { uint32_t limit = 120 -8; uint32_t len_ = ssiz; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5135)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, n, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0); } while(0); { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6144)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(size)); }while(0); } __set_PRIMASK(primaskstate); };}while(0);

    do{ char* n = "bitmask"; int size = sizeof( src->bitmask ); memcpy( p, &(src->bitmask), size ); p += size; do { uint32_t ssiz = strlen( n ); do { uint32_t limit = 120 -8; uint32_t len_ = ssiz; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5135)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, n, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0); } while(0); { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6144)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(size)); }while(0); } __set_PRIMASK(primaskstate); };}while(0);

    return p - dst;
}

static int deserializeTryout( Tryout_t * const dst, char const * src ){
    char const * p = src;

    do{ char* n = "z"; int size = sizeof( dst->z ); memcpy( &(dst->z), p, size ); p += size; do { uint32_t ssiz = strlen( n ); do { uint32_t limit = 120 -8; uint32_t len_ = ssiz; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5152)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, n, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0); } while(0); { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3355)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(size)); }while(0); } __set_PRIMASK(primaskstate); };}while(0);
    do{ char* n = "u"; int size = sizeof( dst->u ); memcpy( &(dst->u), p, size ); p += size; do { uint32_t ssiz = strlen( n ); do { uint32_t limit = 120 -8; uint32_t len_ = ssiz; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5152)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, n, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0); } while(0); { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3355)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(size)); }while(0); } __set_PRIMASK(primaskstate); };}while(0);
    do{ char* n = "s"; int size = sizeof( dst->s ); memcpy( &(dst->s), p, size ); p += size; do { uint32_t ssiz = strlen( n ); do { uint32_t limit = 120 -8; uint32_t len_ = ssiz; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5152)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, n, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0); } while(0); { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3355)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(size)); }while(0); } __set_PRIMASK(primaskstate); };}while(0);
    do{ char* n = "addr"; int size = sizeof( dst->addr ); memcpy( &(dst->addr), p, size ); p += size; do { uint32_t ssiz = strlen( n ); do { uint32_t limit = 120 -8; uint32_t len_ = ssiz; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5152)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, n, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0); } while(0); { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3355)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(size)); }while(0); } __set_PRIMASK(primaskstate); };}while(0);
    do{ char* n = "x"; int size = sizeof( dst->x ); memcpy( &(dst->x), p, size ); p += size; do { uint32_t ssiz = strlen( n ); do { uint32_t limit = 120 -8; uint32_t len_ = ssiz; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5152)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, n, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0); } while(0); { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3355)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(size)); }while(0); } __set_PRIMASK(primaskstate); };}while(0);
    do{ char* n = "y"; int size = sizeof( dst->y ); memcpy( &(dst->y), p, size ); p += size; do { uint32_t ssiz = strlen( n ); do { uint32_t limit = 120 -8; uint32_t len_ = ssiz; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5152)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, n, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0); } while(0); { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3355)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(size)); }while(0); } __set_PRIMASK(primaskstate); };}while(0);
    do{ char* n = "names"; int size = sizeof( dst->names ); memcpy( &(dst->names), p, size ); p += size; do { uint32_t ssiz = strlen( n ); do { uint32_t limit = 120 -8; uint32_t len_ = ssiz; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5152)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, n, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0); } while(0); { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3355)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(size)); }while(0); } __set_PRIMASK(primaskstate); };}while(0);

    do{ char* n = "bitmask"; int size = sizeof( dst->bitmask ); memcpy( &(dst->bitmask), p, size ); p += size; do { uint32_t ssiz = strlen( n ); do { uint32_t limit = 120 -8; uint32_t len_ = ssiz; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5152)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, n, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0); } while(0); { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3355)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(size)); }while(0); } __set_PRIMASK(primaskstate); };}while(0);

    return p - src;
}

static void exampleOfManualSerialization( void ){
    Tryout_t tx;
    Tryout_t rx;
    static char dst[100];
    char* src;
    int len;



    tx.z = (float)123.456;
    tx.u = 44444;
    tx.addr="Haus";
    tx.s = -2;
    tx.x = 0xaa55bb77;
    tx.y = -1000000;

    memcpy( tx.names[0], "aaa", strlen( "aaa" ) );
    memcpy( tx.names[1], "bbbb", strlen( "bbbb" ) );
    memcpy( tx.names[2], "ccccc", strlen( "ccccc" ) );
# 972 "../../../src/triceCheck.c"
    tx.bitmask = 0xAAAA55550000FFFF;



    len = serializeTryout( dst, &tx );
    { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5410)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    do { do { uint32_t limit = 120 -8; uint32_t len_ = sizeof(tx); if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6468)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, &tx, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0); } while(0);
    { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2822)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };

    { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2706)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    do { do { uint32_t limit = 120 -8; uint32_t len_ = len; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3396)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, dst, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0); } while(0);
    { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4774)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };

    src = dst;

    len = deserializeTryout( &rx, src );
    { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7187)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    do { do { uint32_t limit = 120 -8; uint32_t len_ = sizeof(rx); if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6577)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, &rx, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0); } while(0);
    { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5383)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };

    { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5237)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(sizeof(tx))); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len)); }while(0); } __set_PRIMASK(primaskstate); };
    do { do { uint32_t limit = 120 -8; uint32_t len_ = sizeof(tx); if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6773)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, &tx, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0); } while(0);
    do { do { uint32_t limit = 120 -8; uint32_t len_ = len; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3543)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, dst, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0); } while(0);
}

static void exampleOfManualJSONencoding(void){
    typedef struct {
    int Apple, Birn;
    float Fish;
    } Ex_t;
    Ex_t Ex = { -1, 2, (float)2.781 };
    { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7582)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((12)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(Ex.Apple)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(Ex.Birn)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(aFloat(Ex.Fish))); }while(0); } __set_PRIMASK(primaskstate); };
}

static void exampleOfBuffersAndFunctions(void){
    static int8_t b8[24] = { 0, -1, -2, 0x33, 4, 5, 6, 7, 8, 9, 10, 11, 0, -1, -2, 0x33, 4, 5, 6, 7, 8, 9, 10, 11 };
    static int16_t b16[] = { 0, -1, -2, 0x3344 };
    static int32_t b32[] = { 0, -1, -2, 0x33445555};
    static int64_t b64[4] = { 0, -1, -2, 0x3344555566666666 };
    char* s = "abcde 12345";
    uint32_t len;

    do { uint32_t ssiz = strlen( s ); do { uint32_t limit = 120 -8; uint32_t len_ = ssiz; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1436)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, s, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0); } while(0);
    len = strlen(s);
    do { uint32_t limit = 120 -8; uint32_t len_ = len; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3769)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, s, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0);
    { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6888)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len)); }while(0); } __set_PRIMASK(primaskstate); };
    do { do { uint32_t limit = 120 -8; uint32_t len_ = len; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1587)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, s, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0); } while(0);
    { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3130)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    do { do { uint32_t limit = 120 -8; uint32_t len_ = len; if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6145)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, s, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0); } while(0);
    { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7051)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };

    { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7214)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    do { do { uint32_t limit = 120 -8; uint32_t len_ = sizeof(b8)/sizeof(int8_t); if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1238)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, b8, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0); } while(0);
    { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6623)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4383)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    do { do { uint32_t limit = 120 -8; uint32_t len_ = sizeof(b8)/sizeof(int8_t); if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5469)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, b8, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0); } while(0);
    { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2454)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4315)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    do { do { uint32_t limit = 120 -8; uint32_t len_ = 2*sizeof(b16)/sizeof(int16_t); if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7019)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, b16, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0); } while(0);
    { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6717)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5766)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    do { do { uint32_t limit = 120 -8; uint32_t len_ = 4*sizeof(b32)/sizeof(int32_t); if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7870)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, b32, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0); } while(0);
    { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5095)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(5782)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    do { do { uint32_t limit = 120 -8; uint32_t len_ = 8*sizeof(b64)/sizeof(int64_t); if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1951)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, b64, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0); } while(0);
    { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7721)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };

    { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6381)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    do { do { uint32_t limit = 120 -8; uint32_t len_ = sizeof(b8) /sizeof(int8_t); if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2844)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, b8, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0); } while(0);


    { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3434)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    do { do { uint32_t limit = 120 -8; uint32_t len_ = sizeof(b8) /sizeof(int8_t); if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint16_t ts = TriceStamp16(); do{ *TriceBufferWritePosition++ = ((0x80008000|((6950)<<16)|(6950))); }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); }; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, b8, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0); } while(0);


    { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(4491)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    do { do { uint32_t limit = 120 -8; uint32_t len_ = 2*sizeof(b16)/sizeof(int16_t); if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(3620)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, b16, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0); } while(0);


    { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(6999)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    do { do { uint32_t limit = 120 -8; uint32_t len_ = 4*sizeof(b32)/sizeof(int32_t); if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2711)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, b32, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0); } while(0);


    { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(7286)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };
    do { do { uint32_t limit = 120 -8; uint32_t len_ = 8*sizeof(b64)/sizeof(int64_t); if( len_ > limit ){ { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(2240)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(len_)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(limit)); }while(0); } __set_PRIMASK(primaskstate); }; len_ = limit; } { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (0x4000|(1484)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; if( len_ <= 127 ){ do{ uint16_t v = ((len_)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); }else{ do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0x8000 | (len_))); TriceBufferWritePosition = (uint32_t*)p; }while(0); TriceCycle++; } do{ memcpy( TriceBufferWritePosition, b64, len_ ); TriceBufferWritePosition += (len_+3)>>2; }while(0); } __set_PRIMASK(primaskstate); } } while(0); } while(0);

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
