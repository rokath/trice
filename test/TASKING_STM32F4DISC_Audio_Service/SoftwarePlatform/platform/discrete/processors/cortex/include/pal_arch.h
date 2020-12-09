/* ------------------------------------------------------------
**
**  Copyright (c) 2013-2015 Altium Limited
**
**  This software is the proprietary, copyrighted property of
**  Altium Ltd. All Right Reserved.
**
**  SVN revision information:
**  $Rev: 14999 $:
**  $Date: 2015-04-16 16:03:59 +0200 (Thu, 16 Apr 2015) $:
**
** ------------------------------------------------------------
*/

#ifndef _H_PAL_ARCH
#define _H_PAL_ARCH

#include <assert.h>
#include <pal_arch_cortex_variant_cmsis.h>

#include "pal_cfg.h"
#include "pal_arch_cfg.h"

#define PAL_ARCH_INTERRUPT_NATIVE
#define PAL_ARCH_NESTING_INTERRUPTS_SUPPORT     0
#ifdef __CPU_ARMV6M__
#define PAL_ARCH_MAXINTRS                       32
#else
#define PAL_ARCH_MAXINTRS                       240
#endif

extern void pal_architecture_timer_interrupt_start(void);
extern void pal_architecture_timer_interrupt_stop(void);

#if ( __POSIX_KERNEL__ != 0 )
#include "pal_arch_posix.h"
#endif

#ifndef __CPU_ARMV6M__
extern uint32_t     _pal_cortex_nvic_intlinesnum;
#endif
#if ( __POSIX_KERNEL__ != 0 )
#ifdef __CPU_ARMV6M__
extern uint32_t     _posix_interrupts_bitmask;
extern uint32_t     _pal_enabled_posix_interrupts_bitmask;
#else
extern uint32_t     _posix_interrupts_bitmask_array[(PAL_ARCH_MAXINTRS + 31) / 32];
extern uint32_t     _pal_enabled_posix_interrupts_bitmask_array[(PAL_ARCH_MAXINTRS + 31) / 32];
#endif
#endif

static inline void pal_architecture_interrupts_enable(void)
{
    __asm("CPSIE i");
}

static inline void pal_architecture_interrupts_disable(void)
{
    __asm("CPSID i");
}

static inline uint32_t pal_architecture_interrupts_disable_if_enabled(void)
{
    uint32_t ret;
    ret = !(__get_PRIMASK() & 1);
    if (ret)
    {
        __asm("CPSID i");
    }
    return ret;
}

static inline void pal_architecture_interrupt_enable(uint32_t number)
{
    assert(number < PAL_ARCH_MAXINTRS);

#ifdef __CPU_ARMV6M__
# if ( __POSIX_KERNEL__ != 0 )
    if (_posix_interrupts_bitmask & (1 << number))
    {
        _pal_enabled_posix_interrupts_bitmask |= (1 << number);
    }
# endif
    NVIC->ISER[0] = 1 << number;
#else
#if ( __POSIX_KERNEL__ != 0 )
    if (_posix_interrupts_bitmask_array[number / 32] & (1 << (number % 32)))
    {
        _pal_enabled_posix_interrupts_bitmask_array[number / 32] |= (1 << (number % 32));
    }
#endif
    NVIC->ISER[number / 32] = 1 << (number % 32);
#endif
}

static inline void pal_architecture_interrupt_disable(uint32_t number)
{
    assert(number < PAL_ARCH_MAXINTRS);

#ifdef __CPU_ARMV6M__
# if ( __POSIX_KERNEL__ != 0 )
    if (_posix_interrupts_bitmask & (1 << number))
    {
        _pal_enabled_posix_interrupts_bitmask &= ~(1 << number);
    }
# endif
    NVIC->ICER[0] = 1 << number;
#else
#if ( __POSIX_KERNEL__ != 0 )
    if (_posix_interrupts_bitmask_array[number/32] & (1 << (number%32)))
    {
        _pal_enabled_posix_interrupts_bitmask_array[number/32] &= ~(1 << (number%32));
    }
#endif
    NVIC->ICER[number / 32] = 1 << (number % 32);
#endif
}

/*
 * Note: mask is only 32 bits, NVIC on ARMv7M can have 240 interrupts
 */
static inline void pal_architecture_interrupts_set_mask(uint32_t mask)
{
    NVIC->ISER[0] = mask;
    NVIC->ICER[0] = ~mask;
}

/*
 * Note: mask is only 32 bits, NVIC on ARMv7M can have 240 interrupts
 */
static inline uint32_t pal_architecture_interrupts_get_mask(void)
{
    return NVIC->ISER[0];
}

static inline void pal_architecture_interrupt_acknowledge(uint32_t number)
{
    assert(number < PAL_ARCH_MAXINTRS);

#ifdef __CPU_ARMV6M__
    NVIC->ICPR[0] = 1 << number;
#else
    NVIC->ICPR[number / 32] = 1 << (number % 32);
#endif
}

static inline uint32_t pal_architecture_current_interrupt(void)
{
    return ((SCB->ICSR & SCB_ICSR_VECTACTIVE_Msk) >> SCB_ICSR_VECTACTIVE_Pos) - 16;
}

static inline uint32_t pal_architecture_get_current_sp(void)
{
    return __get_MSP();
}

#if ( (__TIMERS__ > 0) || (__POSIX_TIMERS > 0 ) )
extern bool pal_architecture_timer_interrupt_init(void);
#endif

extern bool pal_architecture_interrupts_init(void);
extern bool pal_architecture_processor_init(void);
extern bool pal_architecture_interrupt_configure(uint32_t number, bool edge, bool high);
extern bool pal_architecture_interrupt_priority(uint32_t number, uint32_t prepriority, uint32_t subpriority);
extern bool pal_architecture_clock_init(void);
extern bool pal_architecture_set_native_handler(uint32_t number, void *handler);
extern void pal_architecture_restore_default_handler(uint32_t number);
extern void cortex_install_interrupt_handlers(void);
extern void cortex_copy_and_switch_vector_table(void);

extern void pal_architecture_timer_interrupt_enable(void);
extern void pal_architecture_timer_interrupt_disable(void);
extern bool pal_architecture_timer_interrupt_init(void);
extern void pal_architecture_timer_interrupt_acknowledge(void);

#endif

