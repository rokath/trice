# 1 "../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_dma.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 365 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_dma.c" 2
# 23 "../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_dma.c"
# 1 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h" 1
# 29 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h"
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
# 30 "C:/Users/ms/STM32Cube/Repository/STM32Cube_FW_F0_V1.11.2/Drivers/STM32F0xx_HAL_Driver/Inc\\stm32f0xx_ll_dma.h" 2
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
# 24 "../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_dma.c" 2
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
# 25 "../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_dma.c" 2
# 188 "../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_dma.c"
uint32_t LL_DMA_DeInit(DMA_TypeDef *DMAx, uint32_t Channel)
{
  DMA_Channel_TypeDef *tmp = (DMA_Channel_TypeDef *)((DMA_Channel_TypeDef *) (((0x40000000UL + 0x00020000UL) + 0x00000000UL) + 0x00000008UL));
  ErrorStatus status = SUCCESS;


  ((void)0U);

    tmp = (DMA_Channel_TypeDef *)(((((uint32_t)(DMAx) == ((uint32_t)((DMA_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00000000UL)))) && ((uint32_t)(Channel) == ((uint32_t)0x00000001U))) ? ((DMA_Channel_TypeDef *) (((0x40000000UL + 0x00020000UL) + 0x00000000UL) + 0x00000008UL)) : (((uint32_t)(DMAx) == ((uint32_t)((DMA_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00000000UL)))) && ((uint32_t)(Channel) == ((uint32_t)0x00000002U))) ? ((DMA_Channel_TypeDef *) (((0x40000000UL + 0x00020000UL) + 0x00000000UL) + 0x0000001CUL)) : (((uint32_t)(DMAx) == ((uint32_t)((DMA_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00000000UL)))) && ((uint32_t)(Channel) == ((uint32_t)0x00000003U))) ? ((DMA_Channel_TypeDef *) (((0x40000000UL + 0x00020000UL) + 0x00000000UL) + 0x00000030UL)) : (((uint32_t)(DMAx) == ((uint32_t)((DMA_TypeDef *) ((0x40000000UL + 0x00020000UL) + 0x00000000UL)))) && ((uint32_t)(Channel) == ((uint32_t)0x00000004U))) ? ((DMA_Channel_TypeDef *) (((0x40000000UL + 0x00020000UL) + 0x00000000UL) + 0x00000044UL)) : ((DMA_Channel_TypeDef *) (((0x40000000UL + 0x00020000UL) + 0x00000000UL) + 0x00000058UL))));


    ((tmp->CCR) &= ~((0x1UL << (0U))));


    ((tmp->CCR) = ((0U)));


    ((tmp->CNDTR) = ((0U)));


    ((tmp->CPAR) = ((0U)));


    ((tmp->CMAR) = ((0U)));






    if (Channel == 0x00000001U)
    {

      LL_DMA_ClearFlag_GI1(DMAx);
    }
    else if (Channel == 0x00000002U)
    {

      LL_DMA_ClearFlag_GI2(DMAx);
    }
    else if (Channel == 0x00000003U)
    {

      LL_DMA_ClearFlag_GI3(DMAx);
    }
    else if (Channel == 0x00000004U)
    {

      LL_DMA_ClearFlag_GI4(DMAx);
    }
    else if (Channel == 0x00000005U)
    {

      LL_DMA_ClearFlag_GI5(DMAx);
    }
# 258 "../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_dma.c"
    else
    {
      status = ERROR;
    }

  return status;
}
# 287 "../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_dma.c"
uint32_t LL_DMA_Init(DMA_TypeDef *DMAx, uint32_t Channel, LL_DMA_InitTypeDef *DMA_InitStruct)
{

  ((void)0U);


  ((void)0U);
  ((void)0U);
  ((void)0U);
  ((void)0U);
  ((void)0U);
  ((void)0U);
  ((void)0U);



  ((void)0U);
# 317 "../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_dma.c"
  LL_DMA_ConfigTransfer(DMAx, Channel, DMA_InitStruct->Direction | DMA_InitStruct->Mode | DMA_InitStruct->PeriphOrM2MSrcIncMode | DMA_InitStruct->MemoryOrM2MDstIncMode | DMA_InitStruct->PeriphOrM2MSrcDataSize | DMA_InitStruct->MemoryOrM2MDstDataSize | DMA_InitStruct->Priority);
# 329 "../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_dma.c"
  LL_DMA_SetMemoryAddress(DMAx, Channel, DMA_InitStruct->MemoryOrM2MDstAddress);





  LL_DMA_SetPeriphAddress(DMAx, Channel, DMA_InitStruct->PeriphOrM2MSrcAddress);





  LL_DMA_SetDataLength(DMAx, Channel, DMA_InitStruct->NbData);
# 351 "../Drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_ll_dma.c"
  return SUCCESS;
}






void LL_DMA_StructInit(LL_DMA_InitTypeDef *DMA_InitStruct)
{

  DMA_InitStruct->PeriphOrM2MSrcAddress = 0x00000000U;
  DMA_InitStruct->MemoryOrM2MDstAddress = 0x00000000U;
  DMA_InitStruct->Direction = 0x00000000U;
  DMA_InitStruct->Mode = 0x00000000U;
  DMA_InitStruct->PeriphOrM2MSrcIncMode = 0x00000000U;
  DMA_InitStruct->MemoryOrM2MDstIncMode = 0x00000000U;
  DMA_InitStruct->PeriphOrM2MSrcDataSize = 0x00000000U;
  DMA_InitStruct->MemoryOrM2MDstDataSize = 0x00000000U;
  DMA_InitStruct->NbData = 0x00000000U;



  DMA_InitStruct->Priority = 0x00000000U;
}
