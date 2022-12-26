# 1 "../Core/Src/main.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 365 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "../Core/Src/main.c" 2
# 21 "../Core/Src/main.c"
# 1 "../Core/Inc\\main.h" 1
# 31 "../Core/Inc\\main.h"
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
# 35 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include\\core_cm0.h" 2 3
# 63 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include\\core_cm0.h" 3
# 1 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include/cmsis_version.h" 1 3
# 29 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include/cmsis_version.h" 3
# 64 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include\\core_cm0.h" 2 3
# 115 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include\\core_cm0.h" 3
# 1 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include/cmsis_compiler.h" 1 3
# 41 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include/cmsis_compiler.h" 3
# 1 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include/cmsis_armclang.h" 1 3
# 31 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include/cmsis_armclang.h" 3


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
# 34 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include/cmsis_armclang.h" 2 3
# 68 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include/cmsis_armclang.h" 3
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
# 142 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include/cmsis_armclang.h" 3
__attribute__((always_inline)) static __inline uint32_t __get_CONTROL(void)
{
  uint32_t result;

  __asm volatile ("MRS %0, control" : "=r" (result) );
  return(result);
}
# 172 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include/cmsis_armclang.h" 3
__attribute__((always_inline)) static __inline void __set_CONTROL(uint32_t control)
{
  __asm volatile ("MSR control, %0" : : "r" (control) : "memory");
}
# 196 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include/cmsis_armclang.h" 3
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
# 268 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include/cmsis_armclang.h" 3
__attribute__((always_inline)) static __inline void __set_PSP(uint32_t topOfProcStack)
{
  __asm volatile ("MSR psp, %0" : : "r" (topOfProcStack) : );
}
# 292 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include/cmsis_armclang.h" 3
__attribute__((always_inline)) static __inline uint32_t __get_MSP(void)
{
  uint32_t result;

  __asm volatile ("MRS %0, msp" : "=r" (result) );
  return(result);
}
# 322 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include/cmsis_armclang.h" 3
__attribute__((always_inline)) static __inline void __set_MSP(uint32_t topOfMainStack)
{
  __asm volatile ("MSR msp, %0" : : "r" (topOfMainStack) : );
}
# 373 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include/cmsis_armclang.h" 3
__attribute__((always_inline)) static __inline uint32_t __get_PRIMASK(void)
{
  uint32_t result;

  __asm volatile ("MRS %0, primask" : "=r" (result) );
  return(result);
}
# 403 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include/cmsis_armclang.h" 3
__attribute__((always_inline)) static __inline void __set_PRIMASK(uint32_t priMask)
{
  __asm volatile ("MSR primask, %0" : : "r" (priMask) : "memory");
}
# 876 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include/cmsis_armclang.h" 3
__attribute__((always_inline)) static __inline uint32_t __ROR(uint32_t op1, uint32_t op2)
{
  op2 %= 32U;
  if (op2 == 0U)
  {
    return op1;
  }
  return (op1 >> op2) | (op1 << (32U - op2));
}
# 1121 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include/cmsis_armclang.h" 3
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
# 1146 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include/cmsis_armclang.h" 3
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
# 42 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/CMSIS/Include/cmsis_compiler.h" 2 3
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
# 32 "../Core/Inc\\main.h" 2
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
# 33 "../Core/Inc\\main.h" 2
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
# 34 "../Core/Inc\\main.h" 2
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
# 35 "../Core/Inc\\main.h" 2
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
# 36 "../Core/Inc\\main.h" 2
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
# 37 "../Core/Inc\\main.h" 2
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
# 38 "../Core/Inc\\main.h" 2
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
# 39 "../Core/Inc\\main.h" 2
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
# 40 "../Core/Inc\\main.h" 2
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
# 41 "../Core/Inc\\main.h" 2
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
# 42 "../Core/Inc\\main.h" 2
# 68 "../Core/Inc\\main.h"
void Error_Handler(void);
# 22 "../Core/Src/main.c" 2



# 1 "../../../src\\trice.h" 1
# 88 "../../../src\\trice.h"
typedef enum{
    UartA,
    UartB,
    Rtt0,
    Cgo
} TriceWriteDevice_t;

# 1 "../Core/Inc\\triceConfig.h" 1
# 203 "../Core/Inc\\triceConfig.h"
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
# 96 "../../../src\\trice.h" 2

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
# 98 "../../../src\\trice.h" 2
# 131 "../../../src\\trice.h"
void TriceWriteDevice( TriceWriteDevice_t device, uint8_t *buf, size_t len );



uint16_t TriceStamp16( void );



uint32_t TriceStamp32( void );
# 202 "../../../src\\trice.h"
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
# 252 "../../../src\\trice.h"
extern uint8_t TriceCycle;
# 304 "../../../src\\trice.h"
void TriceBlockingWriteUartA( uint8_t const * buf, unsigned len );
uint8_t TriceNextUint8UartA( void );
void triceServeTransmitUartA(void);
void triceTriggerTransmitUartA(void);
unsigned TriceOutDepthUartA( void );
# 319 "../../../src\\trice.h"
unsigned TriceOutDepth( void );
# 389 "../../../src\\trice.h"
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
# 503 "../../../src\\trice.h"
extern const int TriceTypeS0;
extern const int TriceTypeS2;
extern const int TriceTypeS4;
extern const int TriceTypeX0;
# 26 "../Core/Src/main.c" 2
# 45 "../Core/Src/main.c"
uint32_t milliSecond = 0;
uint64_t microSecond = 0;



void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
# 68 "../Core/Src/main.c"
static inline uint64_t ReadUs64( void ){
    static uint64_t us_1 = 0;
    uint64_t us = microSecond + (((((SysTick_Type *) ((0xE000E000UL) + 0x0010UL) )->LOAD - ((SysTick_Type *) ((0xE000E000UL) + 0x0010UL) )->VAL) * 87381LL) >> 22);
    if( us < us_1){
        us += 1000;
    }
    us_1 = us;
    return us;
}
# 85 "../Core/Src/main.c"
static inline uint32_t ReadUs32( void ){
    return (uint32_t)ReadUs64();
}

uint16_t TriceStamp16( void ){
    return (uint16_t)(ReadUs32()%10000);
}

uint32_t TriceStamp32( void ){
    return ReadUs32();
}





static unsigned timingError64Count = 0;
static unsigned timingError32Count = 0;


static void serveUs( void ){
    static uint64_t st64_1 = 0;
    static uint32_t st32_1 = 0;
    uint64_t st64 = ReadUs64();
    uint32_t st32 = ReadUs32();
    static int virgin64 = 1;
    static int virgin32 = 1;
    if( st64 < st64_1 ){
        timingError64Count++;
        if( virgin64 ){
            virgin64 = 0;
            { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint32_t ts = TriceStamp32(); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0xC000|(7408))); TriceBufferWritePosition = (uint32_t*)p; }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (((ts) >> 16)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; }; do{ uint16_t v = ((16)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(st64)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(st64)>>32)); }while(0);; do{ *TriceBufferWritePosition++ = ((uint32_t)(st64_1)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)((uint64_t)(st64_1)>>32)); }while(0);; } __set_PRIMASK(primaskstate); };

        }
    }
    if( st32 < st32_1 ){
        timingError32Count++;
        if( virgin32 ){
            virgin32 = 0;
            { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint32_t ts = TriceStamp32(); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0xC000|(2140))); TriceBufferWritePosition = (uint32_t*)p; }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (((ts) >> 16)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; }; do{ uint16_t v = ((8)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(st32)); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(st32_1)); }while(0); } __set_PRIMASK(primaskstate); };
        }
    }
    st64_1 = st64;
    st32_1 = st32;
}







int main(void)
{







  LL_APB1_GRP2_EnableClock((0x1UL << (0U)));
  LL_APB1_GRP1_EnableClock((0x1UL << (28U)));






  SystemClock_Config();


    ((SysTick_Type *) ((0xE000E000UL) + 0x0010UL) )->CTRL |= (1UL << 1U);



  MX_GPIO_Init();
  MX_USART2_UART_Init();





    LL_USART_EnableIT_RXNE(((USART_TypeDef *) (0x40000000UL + 0x00004400UL)));
# 215 "../Core/Src/main.c"
   { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint32_t ts = TriceStamp32(); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0xC000|(1089))); TriceBufferWritePosition = (uint32_t*)p; }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (((ts) >> 16)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; }; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); }; { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint32_t ts = TriceStamp32(); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0xC000|(2473))); TriceBufferWritePosition = (uint32_t*)p; }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (((ts) >> 16)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; }; do{ uint16_t v = ((1)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = (((uint8_t)(222))); }while(0); } __set_PRIMASK(primaskstate); }; { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint32_t ts = TriceStamp32(); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0xC000|(5096))); TriceBufferWritePosition = (uint32_t*)p; }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (((ts) >> 16)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; }; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); }; { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint32_t ts = TriceStamp32(); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0xC000|(2966))); TriceBufferWritePosition = (uint32_t*)p; }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (((ts) >> 16)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; }; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); }; TriceLogBufferInfo(); { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint32_t ts = TriceStamp32(); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0xC000|(1631))); TriceBufferWritePosition = (uint32_t*)p; }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (((ts) >> 16)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; }; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); }; { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint32_t ts = TriceStamp32(); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0xC000|(1493))); TriceBufferWritePosition = (uint32_t*)p; }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (((ts) >> 16)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; }; do{ uint16_t v = ((0)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); } __set_PRIMASK(primaskstate); };;





    for(;;){
# 231 "../Core/Src/main.c"
        static unsigned lastMs = 0;
        if( milliSecond >= lastMs + 10 ){
            lastMs = milliSecond;
            TriceTransfer();
        }




        {
            static unsigned lastTricesTime = 0;

            if( milliSecond >= lastTricesTime + 21 ){
                const int begin = 0;
                const int end = 1000;
                static int index = begin;


                TriceCheckSet(index);

                {
                    static uint16_t triceDepthMax_1 = 0;
                    uint16_t triceDepthMax = TriceDepthMax();
                    if( triceDepthMax_1 != triceDepthMax ){
                        triceDepthMax_1 = triceDepthMax;
                        { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint32_t ts = TriceStamp32(); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0xC000|(1192))); TriceBufferWritePosition = (uint32_t*)p; }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (((ts) >> 16)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; }; do{ uint16_t v = ((6)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)(index) | ((uint32_t)(triceDepthMax)<<16)); }while(0); do{ *TriceBufferWritePosition++ = ((uint16_t)((((0x1000/2) + 3) & ~3))); }while(0); } __set_PRIMASK(primaskstate); };
                    }
                }
# 271 "../Core/Src/main.c"
                if( timingError64Count ){
                    { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint32_t ts = TriceStamp32(); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0xC000|(1352))); TriceBufferWritePosition = (uint32_t*)p; }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (((ts) >> 16)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; }; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(timingError64Count)); }while(0); } __set_PRIMASK(primaskstate); };
                }
                if( timingError32Count ){
                    { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); { { uint32_t ts = TriceStamp32(); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = ((0xC000|(6479))); TriceBufferWritePosition = (uint32_t*)p; }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (ts); TriceBufferWritePosition = (uint32_t*)p; }while(0); do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (((ts) >> 16)); TriceBufferWritePosition = (uint32_t*)p; }while(0);; }; do{ uint16_t v = ((4)<<8) | TriceCycle++; do{ uint16_t* p = (uint16_t*)TriceBufferWritePosition; *p++ = (v); TriceBufferWritePosition = (uint32_t*)p; }while(0); }while(0); do{ *TriceBufferWritePosition++ = ((uint32_t)(timingError32Count)); }while(0); } __set_PRIMASK(primaskstate); };
                }
                index++;
                index = index > end ? begin : index;






                lastTricesTime = milliSecond;
            }
        }
        serveUs();
        __builtin_arm_wfi();
        serveUs();
    }


}





void SystemClock_Config(void)
{
  LL_FLASH_SetLatency((0x1UL << (0U)));
  while(LL_FLASH_GetLatency() != (0x1UL << (0U)))
  {
  }
  LL_RCC_HSI_Enable();


  while(LL_RCC_HSI_IsReady() != 1)
  {

  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_PLL_ConfigDomain_SYS((0x00000000U), (0x00280000U));
  LL_RCC_PLL_Enable();


  while(LL_RCC_PLL_IsReady() != 1)
  {

  }
  LL_RCC_SetAHBPrescaler((0x00000000U));
  LL_RCC_SetAPB1Prescaler((0x00000000U));
  LL_RCC_SetSysClkSource((0x00000002U));


  while(LL_RCC_GetSysClkSource() != (0x00000008U))
  {

  }
  LL_Init1msTick(48000000);
  LL_SetSystemCoreClock(48000000);
}






static void MX_USART2_UART_Init(void)
{





  LL_USART_InitTypeDef USART_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};


  LL_APB1_GRP1_EnableClock((0x1UL << (17U)));

  LL_AHB1_GRP1_EnableClock((0x1UL << (17U)));




  GPIO_InitStruct.Pin = (0x00000004U);
  GPIO_InitStruct.Mode = (0x2UL << (0U));
  GPIO_InitStruct.Speed = (0x00000000U);
  GPIO_InitStruct.OutputType = (0x00000000U);
  GPIO_InitStruct.Pull = (0x00000000U);
  GPIO_InitStruct.Alternate = (0x0000001U);
  LL_GPIO_Init(((GPIO_TypeDef *) ((0x40000000UL + 0x08000000UL) + 0x00000000UL)), &GPIO_InitStruct);

  GPIO_InitStruct.Pin = (0x00000008U);
  GPIO_InitStruct.Mode = (0x2UL << (0U));
  GPIO_InitStruct.Speed = (0x00000000U);
  GPIO_InitStruct.OutputType = (0x00000000U);
  GPIO_InitStruct.Pull = (0x00000000U);
  GPIO_InitStruct.Alternate = (0x0000001U);
  LL_GPIO_Init(((GPIO_TypeDef *) ((0x40000000UL + 0x08000000UL) + 0x00000000UL)), &GPIO_InitStruct);


  __NVIC_SetPriority(USART2_IRQn, 0);
  __NVIC_EnableIRQ(USART2_IRQn);




  USART_InitStruct.BaudRate = 921600;
  USART_InitStruct.DataWidth = 0x00000000U;
  USART_InitStruct.StopBits = 0x00000000U;
  USART_InitStruct.Parity = 0x00000000U;
  USART_InitStruct.TransferDirection = ((0x1UL << (3U)) |(0x1UL << (2U)));
  USART_InitStruct.HardwareFlowControl = 0x00000000U;
  USART_InitStruct.OverSampling = 0x00000000U;
  LL_USART_Init(((USART_TypeDef *) (0x40000000UL + 0x00004400UL)), &USART_InitStruct);
  LL_USART_DisableIT_CTS(((USART_TypeDef *) (0x40000000UL + 0x00004400UL)));
  LL_USART_ConfigAsyncMode(((USART_TypeDef *) (0x40000000UL + 0x00004400UL)));
  LL_USART_Enable(((USART_TypeDef *) (0x40000000UL + 0x00004400UL)));




}






static void MX_GPIO_Init(void)
{
  LL_EXTI_InitTypeDef EXTI_InitStruct = {0};
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};


  LL_AHB1_GRP1_EnableClock((0x1UL << (19U)));
  LL_AHB1_GRP1_EnableClock((0x1UL << (22U)));
  LL_AHB1_GRP1_EnableClock((0x1UL << (17U)));


  LL_GPIO_ResetOutputPin(((GPIO_TypeDef *) ((0x40000000UL + 0x08000000UL) + 0x00000000UL)), (0x00000020U));


  LL_SYSCFG_SetEXTISource((uint32_t)2U, (uint32_t)(4U << 16U | 3U));


  LL_GPIO_SetPinPull(((GPIO_TypeDef *) ((0x40000000UL + 0x08000000UL) + 0x00000800UL)), (0x00002000U), (0x00000000U));


  LL_GPIO_SetPinMode(((GPIO_TypeDef *) ((0x40000000UL + 0x08000000UL) + 0x00000800UL)), (0x00002000U), (0x00000000U));


  EXTI_InitStruct.Line_0_31 = (0x1UL << (13U));
  EXTI_InitStruct.LineCommand = ENABLE;
  EXTI_InitStruct.Mode = ((uint8_t)0x00U);
  EXTI_InitStruct.Trigger = ((uint8_t)0x02U);
  LL_EXTI_Init(&EXTI_InitStruct);


  GPIO_InitStruct.Pin = (0x00000020U);
  GPIO_InitStruct.Mode = (0x1UL << (0U));
  GPIO_InitStruct.Speed = (0x00000000U);
  GPIO_InitStruct.OutputType = (0x00000000U);
  GPIO_InitStruct.Pull = (0x00000000U);
  LL_GPIO_Init(((GPIO_TypeDef *) ((0x40000000UL + 0x08000000UL) + 0x00000000UL)), &GPIO_InitStruct);

}
# 451 "../Core/Src/main.c"
void Error_Handler(void)
{


  __disable_irq();
  for(;;)
  {
  }

}
