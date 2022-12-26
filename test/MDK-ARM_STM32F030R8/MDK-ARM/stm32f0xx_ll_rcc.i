# 1 "../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_rcc.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 365 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_rcc.c" 2
# 23 "../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_rcc.c"
# 1 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_rcc.h" 1
# 29 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_rcc.h"
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
# 30 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_rcc.h" 2
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
# 24 "../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_rcc.c" 2
# 79 "../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_rcc.c"
uint32_t RCC_GetSystemClockFreq(void);
uint32_t RCC_GetHCLKClockFreq(uint32_t SYSCLK_Frequency);
uint32_t RCC_GetPCLK1ClockFreq(uint32_t HCLK_Frequency);
uint32_t RCC_PLL_GetFreqDomain_SYS(void);
# 112 "../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_rcc.c"
ErrorStatus LL_RCC_DeInit(void)
{
  volatile uint32_t vl_mask;


  LL_RCC_HSI_Enable();


  while(LL_RCC_HSI_IsReady() != 1U)
  {}


  LL_RCC_HSI_SetCalibTrimming(0x10U);


  vl_mask = 0xFFFFFFFFU;
  ((vl_mask) &= ~(((0x3UL << (0U)) | (0xFUL << (4U)) | (0x7UL << (8U)) | (0xFUL << (24U)))));


  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CFGR) = ((vl_mask)));


  while(LL_RCC_GetSysClkSource() != (0x00000000U))
  {}


  vl_mask = ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CR));


  ((vl_mask) &= ~(((0x1UL << (24U)) | (0x1UL << (19U)) | (0x1UL << (16U)))));


  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CR) = ((vl_mask)));


  while(LL_RCC_PLL_IsReady() != 0U)
  {}


  LL_RCC_HSE_DisableBypass();


  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CFGR) = ((0x00000000U)));
# 165 "../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_rcc.c"
  LL_RCC_HSI14_SetCalibTrimming(0x10U);
  LL_RCC_HSI14_Disable();
  LL_RCC_HSI14_EnableADCControl();


  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CFGR2) = ((0x00000000U)));


  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CFGR3) = ((0x00000000U)));






  vl_mask = ((0x1UL << (16U)) | (0x1UL << (17U)) | (0x1UL << (18U)) | (0x1UL << (19U)) | (0x1UL << (20U)) | (0x1UL << (21U)) | (0x1UL << (23U)));




  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CIR) = ((vl_mask)));


  ((((RCC_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00001000UL))->CIR) = ((0x00000000U)));


  LL_RCC_ClearResetFlags();

  return SUCCESS;
}
# 227 "../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_rcc.c"
void LL_RCC_GetSystemClocksFreq(LL_RCC_ClocksTypeDef *RCC_Clocks)
{

  RCC_Clocks->SYSCLK_Frequency = RCC_GetSystemClockFreq();


  RCC_Clocks->HCLK_Frequency = RCC_GetHCLKClockFreq(RCC_Clocks->SYSCLK_Frequency);


  RCC_Clocks->PCLK1_Frequency = RCC_GetPCLK1ClockFreq(RCC_Clocks->HCLK_Frequency);
}
# 250 "../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_rcc.c"
uint32_t LL_RCC_GetUSARTClockFreq(uint32_t USARTxSource)
{
  uint32_t usart_frequency = 0x00000000U;


  ((void)0U);

  if (USARTxSource == (uint32_t)0U)
  {

    switch (LL_RCC_GetUSARTClockSource(USARTxSource))
    {
      case (uint32_t)(((uint32_t)0U << 24) | (0x00000001U)):
        usart_frequency = RCC_GetSystemClockFreq();
        break;

      case (uint32_t)(((uint32_t)0U << 24) | (0x00000003U)):
        if (LL_RCC_HSI_IsReady())
        {
          usart_frequency = 8000000;
        }
        break;

      case (uint32_t)(((uint32_t)0U << 24) | (0x00000002U)):
        if (LL_RCC_LSE_IsReady())
        {
          usart_frequency = 32768;
        }
        break;

      case (uint32_t)(((uint32_t)0U << 24) | (0x00000000U)):
      default:
        usart_frequency = RCC_GetPCLK1ClockFreq(RCC_GetHCLKClockFreq(RCC_GetSystemClockFreq()));
        break;
    }
  }
# 352 "../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_rcc.c"
  return usart_frequency;
}
# 362 "../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_rcc.c"
uint32_t LL_RCC_GetI2CClockFreq(uint32_t I2CxSource)
{
  uint32_t i2c_frequency = 0x00000000U;


  ((void)0U);


  if (I2CxSource == (0x1UL << (4U)))
  {
    switch (LL_RCC_GetI2CClockSource(I2CxSource))
    {
      case (0x1UL << (4U)):
        i2c_frequency = RCC_GetSystemClockFreq();
        break;

      case (0x00000000U):
      default:
        if (LL_RCC_HSI_IsReady())
        {
          i2c_frequency = 8000000;
        }
        break;
    }
  }

  return i2c_frequency;
}
# 491 "../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_rcc.c"
uint32_t RCC_GetSystemClockFreq(void)
{
  uint32_t frequency = 0U;


  switch (LL_RCC_GetSysClkSource())
  {
    case (0x00000000U):
      frequency = 8000000;
      break;

    case (0x00000004U):
      frequency = 8000000;
      break;

    case (0x00000008U):
      frequency = RCC_PLL_GetFreqDomain_SYS();
      break;







    default:
      frequency = 8000000;
      break;
  }

  return frequency;
}






uint32_t RCC_GetHCLKClockFreq(uint32_t SYSCLK_Frequency)
{

  return ((SYSCLK_Frequency) >> AHBPrescTable[((LL_RCC_GetAHBPrescaler()) & (0xFUL << (4U))) >> (4U)]);
}






uint32_t RCC_GetPCLK1ClockFreq(uint32_t HCLK_Frequency)
{

  return ((HCLK_Frequency) >> APBPrescTable[(LL_RCC_GetAPB1Prescaler()) >> (8U)]);
}




uint32_t RCC_PLL_GetFreqDomain_SYS(void)
{
  uint32_t pllinputfreq = 0U, pllsource = 0U;




  pllsource = LL_RCC_PLL_GetMainSource();

  switch (pllsource)
  {




    case (0x00000000U):
      pllinputfreq = 8000000 / 2U;

      break;







    case (0x00010000U):
      pllinputfreq = 8000000;
      break;

    default:



      pllinputfreq = 8000000 / 2U;

      break;
  }



  return (((pllinputfreq / (LL_RCC_PLL_GetPrediv() + 1U))) * ((((LL_RCC_PLL_GetMultiplicator()) & (0xFUL << (18U))) >> (uint32_t)18U) + 2U));

}
