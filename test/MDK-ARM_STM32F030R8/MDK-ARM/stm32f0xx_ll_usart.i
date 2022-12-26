# 1 "../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_usart.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 365 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_usart.c" 2
# 22 "../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_usart.c"
# 1 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h" 1
# 29 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h"
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
# 30 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_usart.h" 2
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
# 23 "../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_usart.c" 2
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
# 24 "../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_usart.c" 2
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
# 25 "../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_usart.c" 2
# 126 "../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_usart.c"
ErrorStatus LL_USART_DeInit(USART_TypeDef *USARTx)
{
  ErrorStatus status = SUCCESS;


  ((void)0U);

  if (USARTx == ((USART_TypeDef *) (0x40000000UL + 0x00013800UL)))
  {

    LL_APB1_GRP2_ForceReset((0x1UL << (14U)));


    LL_APB1_GRP2_ReleaseReset((0x1UL << (14U)));
  }

  else if (USARTx == ((USART_TypeDef *) (0x40000000UL + 0x00004400UL)))
  {

    LL_APB1_GRP1_ForceReset((0x1UL << (17U)));


    LL_APB1_GRP1_ReleaseReset((0x1UL << (17U)));
  }
# 211 "../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_usart.c"
  else
  {
    status = ERROR;
  }

  return (status);
}
# 233 "../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_usart.c"
ErrorStatus LL_USART_Init(USART_TypeDef *USARTx, LL_USART_InitTypeDef *USART_InitStruct)
{
  ErrorStatus status = ERROR;
  uint32_t periphclk = 0x00000000U;

  LL_RCC_ClocksTypeDef RCC_Clocks;



  ((void)0U);
  ((void)0U);
  ((void)0U);
  ((void)0U);
  ((void)0U);
  ((void)0U);
  ((void)0U);
  ((void)0U);



  if (LL_USART_IsEnabled(USARTx) == 0U)
  {







    (((USARTx->CR1)) = ((((((USARTx->CR1))) & (~(((0x1UL << (12U)) | (0x1UL << (10U)) | (0x1UL << (9U)) | (0x1UL << (3U)) | (0x1UL << (2U)) | (0x1UL << (15U)))))) | ((USART_InitStruct->DataWidth | USART_InitStruct->Parity | USART_InitStruct->TransferDirection | USART_InitStruct->OverSampling)))));
# 273 "../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_usart.c"
    LL_USART_SetStopBitsLength(USARTx, USART_InitStruct->StopBits);






    LL_USART_SetHWFlowCtrl(USARTx, USART_InitStruct->HardwareFlowControl);




    if (USARTx == ((USART_TypeDef *) (0x40000000UL + 0x00013800UL)))
    {
      periphclk = LL_RCC_GetUSARTClockFreq((uint32_t)0U);
    }

    else if (USARTx == ((USART_TypeDef *) (0x40000000UL + 0x00004400UL)))
    {




      LL_RCC_GetSystemClocksFreq(&RCC_Clocks);
      periphclk = RCC_Clocks.PCLK1_Frequency;

    }
# 353 "../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_usart.c"
    else
    {

    }





    if ((periphclk != 0x00000000U)
        && (USART_InitStruct->BaudRate != 0U))
    {
      status = SUCCESS;
      LL_USART_SetBaudRate(USARTx,
                           periphclk,
                           USART_InitStruct->OverSampling,
                           USART_InitStruct->BaudRate);


      ((void)0U);
    }
  }


  return (status);
}
# 387 "../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_usart.c"
void LL_USART_StructInit(LL_USART_InitTypeDef *USART_InitStruct)
{

  USART_InitStruct->BaudRate = 9600U;
  USART_InitStruct->DataWidth = 0x00000000U;
  USART_InitStruct->StopBits = 0x00000000U;
  USART_InitStruct->Parity = 0x00000000U ;
  USART_InitStruct->TransferDirection = ((0x1UL << (3U)) |(0x1UL << (2U)));
  USART_InitStruct->HardwareFlowControl = 0x00000000U;
  USART_InitStruct->OverSampling = 0x00000000U;
}
# 413 "../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_usart.c"
ErrorStatus LL_USART_ClockInit(USART_TypeDef *USARTx, LL_USART_ClockInitTypeDef *USART_ClockInitStruct)
{
  ErrorStatus status = SUCCESS;


  ((void)0U);
  ((void)0U);



  if (LL_USART_IsEnabled(USARTx) == 0U)
  {

    if (USART_ClockInitStruct->ClockOutput == 0x00000000U)
    {



      LL_USART_DisableSCLKOutput(USARTx);
    }
    else
    {

      ((void)0U);


      ((void)0U);
      ((void)0U);
      ((void)0U);
# 450 "../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_usart.c"
      (((USARTx->CR2)) = ((((((USARTx->CR2))) & (~((0x1UL << (11U)) | (0x1UL << (9U)) | (0x1UL << (10U)) | (0x1UL << (8U))))) | ((0x1UL << (11U)) | USART_ClockInitStruct->ClockPolarity | USART_ClockInitStruct->ClockPhase | USART_ClockInitStruct->LastBitClockPulse))));



    }
  }

  else
  {
    status = ERROR;
  }

  return (status);
}







void LL_USART_ClockStructInit(LL_USART_ClockInitTypeDef *USART_ClockInitStruct)
{

  USART_ClockInitStruct->ClockOutput = 0x00000000U;
  USART_ClockInitStruct->ClockPolarity = 0x00000000U;

  USART_ClockInitStruct->ClockPhase = 0x00000000U;

  USART_ClockInitStruct->LastBitClockPulse = 0x00000000U;

}
