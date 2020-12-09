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

/**
 * @file
 *      Interrupt services.
 *
 * These system utilities provide services to disable, enable, mask,
 * configure, acknowledge etc.. interrupts. Plus extra mechanisms to
 * register/associate native and normal interrupt handlers.
 *
 * These utilities are mostly used by interrupt-driven driver software in the
 * Software Platform.
 */

#ifndef _INTERRUPTS_H
#define _INTERRUPTS_H

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#if ( __POSIX_KERNEL__ != 0 )
# include <time.h>
#endif

#include <pal_arch.h>           // processor specific implementations
#include "interrupts_cfg.h"     // SwPlatform interrupt service configuration


/* Macro definitions */

/** Define native interrupt handler. See interrupt_register_native().
 *  For example: <code>__INTERRUPT_NATIVE void my_handler(void)</code>
 */
#define __INTERRUPT_NATIVE              PAL_ARCH_INTERRUPT_NATIVE

// interrupt handler return values
#define INTERRUPT_NOT_HANDLED           1   /**< Interrupt has not been handled  */
#define INTERRUPT_HANDLED               2   /**< Interrupt has been handled  */
#define INTERRUPT_NOTIFY                4   /**< Notify to awake associated waiting thread  */


/* Type definitions */

/**
 * @brief
 *  Interrupt configurations
 *
 *  Valid values for the configuration of interrupts.
 */
typedef enum
{
    EDGE_RISING,    /**< Interrupt on rising edge */
    EDGE_FALLING,   /**< Interrupt on falling edge */
    LEVEL_HIGH,     /**< Interrupt on high level */
    LEVEL_LOW       /**< Interrupt on low level */
} interrupt_cfg_t;

/** Native interrupt handler prototype */
typedef void (*interrupt_native_handler_t)( void );

/** Normal interrupt handler prototype */
typedef uint32_t (*interrupt_handler_t)( uint32_t number, void * context );

/*
 * interrupt handler context
 */
typedef struct interrupt_entry_s interrupt_entry_t;
struct interrupt_entry_s
{
    uint16_t                number;
    volatile bool           in_use;
    bool                    is_native;
    void                    *context;
    interrupt_handler_t     handler;
#if ( __POSIX_KERNEL__ != 0 )
    void*                   thread;
#endif
    interrupt_entry_t * volatile next;
};

/* Prototypes */

static inline void     interrupts_enable(void);
static inline void     interrupts_disable(void);
static inline uint32_t  interrupts_disable_if_enabled(void);
static inline void      interrupts_enable_if(bool prev);

static inline void     interrupt_enable(uint32_t number);
static inline void     interrupt_disable(uint32_t number);

static inline void     interrupts_set_enable_mask(uint32_t mask);
static inline uint32_t interrupts_get_enable_mask(void);

extern bool     interrupt_register_native(uint32_t number, void* context, interrupt_native_handler_t handler);
extern bool     interrupt_register(uint32_t number, void* context, interrupt_handler_t handler);
extern bool     interrupt_deregister(uint32_t number, void* context, interrupt_handler_t handler);
extern bool     interrupt_configure(uint32_t number, interrupt_cfg_t cfg);
extern bool     interrupt_priority(uint32_t number, uint32_t preemptionpriority, uint32_t subpriority);

static inline void     interrupt_acknowledge(uint32_t number);
static inline void*    interrupt_native_context(uint32_t number);
static inline uint32_t interrupt_get_current(void);

#if ( __POSIX_KERNEL__ != 0 )
static inline void     posix_interrupts_disable(void);
static inline void     posix_interrupts_enable(void);

extern bool     interrupt_set_posix(uint32_t number, bool posix);

extern int      pthread_interrupt_associate(uint32_t number, interrupt_handler_t handler, void* context);
extern int      pthread_interrupt_disassociate(uint32_t number, interrupt_handler_t handler, void* context);
extern int      pthread_interrupt_timedwait(const struct timespec* timeout);
#endif /* ( __POSIX_KERNEL__ != 0 ) */


/* Global data */

/** global interrupt context table */
extern interrupt_entry_t *interrupt_hash_table[INTERRUPTS_HASH_SIZE];

#if ( __POSIX_KERNEL__ != 0 )
#if (PAL_ARCH_MAXINTRS > 32)
extern uint32_t     _posix_interrupts_bitmask_array[(PAL_ARCH_MAXINTRS + 31) / 32];
#else
extern uint32_t     _posix_interrupts_bitmask;
#endif
extern volatile int _posix_irq_level; /* defined in services/posix/kernel/threads/src/k_schedule.c */
extern volatile int _posix_posix_interrupts_disabled; /* defined in services/posix/kernel/threads/src/k_schedule.c */
#endif


/* Inline function definitions */

/* prevent code compaction in these inline functions */
#pragma optimize R

/**
 * @brief
 *      Enable interrupt handling
 *
 * This function globally enables all maskable interrupts.
 *
 * @return Nothing
 *
 * @see
 *      interrupts_disable()
 */
static inline void interrupts_enable(void)
{
    pal_architecture_interrupts_enable();
}

/**
 * @brief
 *      Conditionally enable interrupt handling
 *
 * Conditionally enable interrupt handling. To be paired up with interrupts_disable_if_enabled().
 *
 * @param
 * 	when true enable interrupt when false do nothing
 *
 * @return Nothing
 *
 * @see
 *      interrupts_disable_if_enabled()
 */
static inline void interrupts_enable_if(bool prev)
{
    if (prev)
    {
        pal_architecture_interrupts_enable();
    }
}

/**
 * @brief
 *      Disable interrupt handling
 *
 * This function globally disables all maskable interrupts.  Executing with
 * interrupts disabled might have an effect on the overall response of the
 * system (real-time latency); they should be disabled for extremely short
 * periods of time only.
 *
 * @return Nothing
 *
 * @see
 *      interrupts_enable()
 */
static inline void interrupts_disable(void)
{
    pal_architecture_interrupts_disable();
}

/**
 * @brief
 *      Disable interrupt handling
 *
 * Disable interrupts on the processor. This function returns a value not equal
 * to 0 when interrupts were enabled at the time of this function call.
 *
 * @return
 *		0 when interrupts were already disabled at the time of this function call,
 *		non-zero when interrupts were enabled at the time of this function call
 *
 * @see
 *      interrupts_enable()
 */

static inline uint32_t interrupts_disable_if_enabled(void)
{
    return pal_architecture_interrupts_disable_if_enabled();
}

/**
 * @brief
 *      Enables one interrupt
 *
 * This function enables the maskable interrupt specified by @em number.
 *
 * @param number
 *      interrupt number
 *
 * @return Nothing
 *
 * @see
 *      interrupt_disable()
 */
static inline void interrupt_enable(uint32_t number)
{
    pal_architecture_interrupt_enable(number);
}

/**
 * @brief
 *      Disable one interrupt
 *
 * This function disables the maskable interrupt specified by @em number.  It
 * is used by system drivers to avoid race conditions between interrupts and
 * application code.
 *
 * @param number
 *      interrupt number
 *
 * @return Nothing
 *
 * @see
 *      interrupts_enable()
 */
static inline void interrupt_disable(uint32_t number)
{
    pal_architecture_interrupt_disable(number);
}

/**
 * @brief
 *      Enable set of interrupts
 *
 * This function enables a set of maskable interrupts given by @em mask.
 *
 * @param mask
 *      Set of interrupts to enable
 *
 * @return Nothing
 */
static inline void interrupts_set_enable_mask(uint32_t mask)
{
    pal_architecture_interrupts_set_mask (mask);
}

/**
 * @brief
 *      Get current enabled interrupts
 *
 * This function returns a mask with the currently enabled interrupts.
 *
 * @return Set of enabled interrupts.
 */
static inline uint32_t interrupts_get_enable_mask(void)
{
    return pal_architecture_interrupts_get_mask();
}

/**
 * @brief
 *      Acknowledge an interrupt
 *
 * This function should be used in interrupt handlers to acknowledge reception
 * of the specified interrupt.  It will clear the interrupt pending flag in the
 * interrupt controller and allow generation of a new interrupt.
 *
 * @param number
 *      Interrupt number
 *
 * @return Nothing
 */
static inline void interrupt_acknowledge(uint32_t number)
{
    pal_architecture_interrupt_acknowledge(number);
}

/**
 * @brief
 *      Get native interrupt context
 *
 * This function returns the native interrupt context that was provided with
 * the interrupt_register_native() call.
 *
 * @param number
 *      Native interrupt number
 *
 * @return Native interrupt context.
 */
static inline void* interrupt_native_context(uint32_t number)
{
    interrupt_entry_t *p;

    p = interrupt_hash_table[number % INTERRUPTS_HASH_SIZE];
    while (p != NULL && p->number != number)
    {
        p = p->next;
    }

    return p != NULL ? p->context : NULL;
}

/**
 * @brief
 *      Get current interrupt number
 *
 * This function retrieves the number of the currently executing interrupt.
 * It can be used inside an interrupt handler to find out which device
 * triggered the interrupt.  It is normally followed by a call to
 * interrupt_native_context() to get the associated interrupt context.
 *
 * @return Currently executing interrupt.
 */
static inline uint32_t interrupt_get_current(void)
{
   return pal_architecture_current_interrupt();
}

#if ( __POSIX_KERNEL__ != 0 )

/**
 * @brief
 *      Disable all posix interrupts
 *
 * This function disables all posix interrupts in the system.  With posix
 * interrupts disabled, the system is protected against unwanted context
 * switches; the scheduler is effectively locked.  Non-posix interrupts are
 * still served.
 *
 * The functions posix_interrupts_disable()/posix_interrupts_enable() do not
 * support nesting.  They should always be called in combination, preferably
 * at the same function level.
 *
 * Application code that runs while posix interrupts are disabled should be
 * fast and should never use posix system calls.
 *
 * @return Nothing
 */
static inline void     posix_interrupts_disable(void)
{
    if ( _posix_irq_level==0 )
    {
        pal_architecture_interrupts_disable();
        pal_architecture_posix_interrupts_disable();
        _posix_posix_interrupts_disabled ++;
        pal_architecture_interrupts_enable();
    }
}

/**
 * @brief
 *      Enable all posix interrupts
 *
 * This function enables all posix interrupts in the system. It should
 * be called exactly once for each call to posix_interrupts_disable().
 * Nesting is not supported.
 *
 * @return Nothing
 */
static inline void     posix_interrupts_enable(void)
{
    if ( _posix_irq_level==0 )
    {
        pal_architecture_interrupts_disable();
        pal_architecture_posix_interrupts_enable();
        _posix_posix_interrupts_disabled --;
        pal_architecture_interrupts_enable();
    }
}

#endif /* ( __POSIX_KERNEL__ != 0 ) */

/* restore code compaction optimization setting */
#pragma optimize restore

#ifdef  __cplusplus
}
#endif

#endif /* _INTERRUPTS_H */
