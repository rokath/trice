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
**  Interrupt services.
**
** ------------------------------------------------------------
*/

#include <stddef.h>
#include <assert.h>

#include <pal.h>
#include <interrupts.h>

#if  ( __POSIX_KERNEL__ != 0 )
#include <errno.h>
#include "posix_pthread.h"
#include "posix_time.h"
#include "kernel.h"
#include "k_schedule.h"
#endif

#include "interrupts_cfg.h"

/*
 * hash table and entries
 */
interrupt_entry_t           *interrupt_hash_table[INTERRUPTS_HASH_SIZE];
static interrupt_entry_t    interrupt_entries[INTERRUPTS_MAX_HANDLERS];
static int                  interrupt_last_entry;

#if  ( __POSIX_KERNEL__ != 0 )
#if (PAL_ARCH_MAXINTRS > 32)
uint32_t    _posix_interrupts_bitmask_array[(PAL_ARCH_MAXINTRS + 31) / 32];
#else
uint32_t    _posix_interrupts_bitmask = 0;
#endif
#endif


/* Private API */

static interrupt_entry_t *interrupt_entry_alloc(void)
{
    int         ena, i;
    uint32_t    index;

    index = interrupt_last_entry;
    for (i = 0; i < INTERRUPTS_MAX_HANDLERS; i++)
    {
        if (interrupt_entries[index].in_use == false)
        {
            ena = interrupts_disable_if_enabled();
            if (interrupt_entries[index].in_use == false)
            {
                interrupt_entries[index].in_use = true;
                interrupts_enable_if(ena);
                return &interrupt_entries[index];
            }
            interrupts_enable_if(ena);
        }
        index ++;
        if (index == INTERRUPTS_MAX_HANDLERS)
        {
            index = 0;
        }
    }

    return NULL;
}

/*
 * From P1003.1d:
 * "The system shall call intr_handlers in the reverse order that the ISRs were
 * registered until one of the intr_handlers returns a code signifying that the
 * interrupt has been handled. The most recently registered ISR is thus called
 * first."
 */
static void interrupt_entry_add_to_hash(interrupt_entry_t *entry, uint32_t number)
{
    bool ena;

    ena = interrupts_disable_if_enabled();

    entry->next = interrupt_hash_table[number % INTERRUPTS_HASH_SIZE];
        interrupt_hash_table[number % INTERRUPTS_HASH_SIZE] = entry;

    interrupts_enable_if(ena);
    return;
}

static interrupt_entry_t *interrupt_entry_remove_from_hash(uint32_t number,
                            interrupt_handler_t handler, void *context)
{
    interrupt_entry_t **pp, *ret;
    bool ena;

    ena = interrupts_disable_if_enabled();
    pp = &interrupt_hash_table[number % INTERRUPTS_HASH_SIZE];
    while (*pp != NULL && !((*pp)->handler == handler && (*pp)->context == context))
    {
        pp = (interrupt_entry_t **)&((*pp)->next);
    }

    if (*pp != NULL)
    {
        ret = *pp;
        *pp = ret->next;
    }
    else
    {
        ret = NULL;
    }
    interrupts_enable_if(ena);

    return ret;
}

#if  ( __POSIX_KERNEL__ != 0 )
static interrupt_entry_t *interrupt_entry_find_in_hash(uint32_t number,
                            interrupt_handler_t handler, void *context)
{
    interrupt_entry_t *entry;
    bool ena;

    ena = interrupts_disable_if_enabled();

    entry = interrupt_hash_table[number % INTERRUPTS_HASH_SIZE];
    while (entry != NULL)
    {
        if (entry->handler == handler && entry->context == context && entry->number == number)
        {
            break;
        }
        entry = entry->next;
    }

    interrupts_enable_if(ena);

    return entry;
}
#endif /* (__POSIX_KERNEL__ != 0 ) */

static void interrupt_free_entry(interrupt_entry_t *entry)
{
    entry->number   = 0;
    entry->context  = NULL;
    entry->handler  = NULL;
#if ( __POSIX_KERNEL__ != 0 )
    entry->thread   = NULL;
#endif
    entry->is_native    = false;
    entry->next     = NULL;
    entry->in_use       = 0;

    return;
}

/*
 * Removes and frees all entries for interrupt 'number' from the hash.
 * Used when installing a native interrupt handler.
 */
static void interrupt_remove_entries_from_hash(uint32_t number)
{
    interrupt_entry_t **pp, *entry;
    bool ena;

    ena = interrupts_disable_if_enabled();

    pp = &interrupt_hash_table[number % INTERRUPTS_HASH_SIZE];
    while (*pp != NULL)
    {
        if ((*pp)->number == number)
        {
            entry = *pp;
            *pp = entry->next;
            interrupt_free_entry(entry);
        }
        else
        {
            pp = (interrupt_entry_t **)&((*pp)->next);
        }
    }

    interrupts_enable_if(ena);

    return;
}


/*
 * Removes and frees, if any, native interrupt handler for interrupt 'number' from the hash.
 * Used when installing a non native interrupt handler.
 */
static void interrupt_remove_native_entry_from_hash(uint32_t number)
{
    interrupt_entry_t **pp, *entry = NULL;
    bool ena;

    ena = interrupts_disable_if_enabled();

    pp = &interrupt_hash_table[number % INTERRUPTS_HASH_SIZE];
    while (*pp != NULL && !((*pp)->number == number && (*pp)->is_native == true))
    {
        pp = (interrupt_entry_t **)&((*pp)->next);
    }

    if (*pp != NULL)
    {
        entry = *pp;
        *pp = entry->next;
    }

    interrupts_enable_if(ena);

    if (entry != NULL)
    {
        interrupt_free_entry(entry);
    }

    return;
}


#if  ( __POSIX_KERNEL__ != 0 )
static inline void interrupts_notify_thread(posix_thread_t *th)
{
    if (th != NULL)
    {
        th->intr_notify = 1;
        if ( th->intr_wait )
        {
             posix_thread_ready(th, POSIX_WAKE_TIMEDINTR);
        }
    }
}
#endif

/* Public API */

/**
 * @brief
 *      Register a native interrupt handler
 *
 * This function declares an interrupt line as native and installs its native
 * handler.  Native interrupt lines can only hold one handler which executes
 * immediately upon interrupt arrival, bypassing all framework activities.
 *
 * Native lines do not support sharing.  In multi-threaded applications, threads
 * cannot block on native handlers.
 *
 * The @em handler function should be declared with the __INTERRUPT_NATIVE
 * macro.
 *
 * @param number
 *      Interrupt number
 * @param context
 *      Native handler context (normally private driver data)
 * @param handler
 *      Native handler
 *
 * @return  true if native handler was installed, false if handler could not be
 *          installed (incorrect value for number).
 */
extern bool interrupt_register_native(uint32_t number, void *context, interrupt_native_handler_t handler)
{
    interrupt_entry_t *p;

    if (number >= PAL_ARCH_MAXINTRS) return false;

    p = interrupt_entry_alloc();
    if (p != NULL)
    {
        if (pal_architecture_set_native_handler(number, handler))
        {
            p->context  = context;
            p->number   = (uint16_t)number;
            p->handler  = (interrupt_handler_t)handler;
            p->is_native    = true;
            interrupt_remove_entries_from_hash(number);
            interrupt_entry_add_to_hash(p, number);
            return true;
        }
        else
        {
            interrupt_free_entry(p);
        }
    }

    return false;
}

/**
 * @brief
 *      Register a normal interrupt handler
 *
 * This function installs a normal interrupt handler. Normal interrupt support
 * more than one interrupt handler per interrupt number. Interrupt handlers
 * are called in the reverse order that they were registered until one of the
 * handlers returns a code signifying that the interrupt has been handled.
 *
 * Normal handlers are not as fast as native handlers since they must execute
 * after some framework housekeeping activities.
 * In multi-threaded applications, threads can block on normal handlers but
 * only when they are declared as posix interrupts (see interrupt_set_posix()).
 *
 * @param number
 *      Interrupt number
 * @param context
 *      Handler context (normally private driver data)
 * @param handler
 *      Interrupt handler
 *
 * @return true to indicate success, or false otherwise.
 */
extern bool interrupt_register (uint32_t number, void* context, interrupt_handler_t handler)
{
    interrupt_entry_t *p;

    if (number >= PAL_ARCH_MAXINTRS) return false;

    p = interrupt_entry_alloc();
    if (p != NULL)
    {
        pal_architecture_restore_default_handler(number);
        interrupt_remove_native_entry_from_hash(number);
        p->context  = context;
        p->number   = (uint16_t)number;
        p->handler  = handler;
        interrupt_entry_add_to_hash(p, number);
        return true;
    }

    return false;
}

/**
 * @brief
 *      De-register a normal interrupt handler
 *
 * This function de-registers a normal interrupt handler for the specified
 * interrupt line.
 *
 * @param number
 *      Interrupt number
 * @param context
 *      Interrupt handler context (normally private driver data)
 * @param handler
 *      Interrupt handler
 *
 * @return true to indicate success, or false otherwise.
 */
extern bool interrupt_deregister     (uint32_t number, void* context, interrupt_handler_t handler)
{
    interrupt_entry_t *entry;

    if (number >= PAL_ARCH_MAXINTRS) return false;

    entry = interrupt_entry_remove_from_hash(number, handler, context);
    if (entry != NULL)
    {
        interrupt_free_entry(entry);
    }

    return entry != NULL ? true : false;
}

/**
 * @brief
 *      Configure interrupt type
 *
 * This function sets the interrupt type for a given interrupt. It should be
 * used by drivers during initialization to configure the interrupt in the
 * controller.
 *
 * @param number
 *      Interrupt number
 * @param cfg
 *      Interrupt type
 *
 * @return true to indicate success, or false otherwise.
 */
extern bool interrupt_configure( uint32_t number, interrupt_cfg_t cfg )
{
    switch ( cfg )
    {
        case EDGE_RISING : return pal_architecture_interrupt_configure(number,true,true);
        case EDGE_FALLING: return pal_architecture_interrupt_configure(number,true,false);
        case LEVEL_HIGH  : return pal_architecture_interrupt_configure(number,false,true);
        case LEVEL_LOW   : return pal_architecture_interrupt_configure(number,false,false);
        default          : break;
    }
    return false;
}


extern bool interrupt_priority(uint32_t number, uint32_t preemptionpriority, uint32_t subpriority)
{
    return pal_architecture_interrupt_priority(number, preemptionpriority, subpriority);
}

#if  ( __POSIX_KERNEL__ != 0 )
/**
 * @brief
 *      Configure a posix interrupt
 *
 * This function declares a normal interrupt line as a posix interrupt.  With
 * posix interrupts, threads can be associated with registered handlers using
 * pthread_interrupt_associate().  Threads can block waiting for interrupt
 * synchronization using pthread_interrupt_timedwait().
 *
 * @param number
 *      Interrupt number
 * @param posix
 *      True to set a posix interrupt
 *
 * @return true to indicate success, or false otherwise.
 */
extern bool interrupt_set_posix(uint32_t number, bool posix)
{
    if (posix)
    {
#if (PAL_ARCH_MAXINTRS > 32)
        _posix_interrupts_bitmask_array[number / 32] |= (1 << (number % 32));
#else
        _posix_interrupts_bitmask |= ( 1 << number );
#endif
    }
    else
    {
#if (PAL_ARCH_MAXINTRS > 32)
        _posix_interrupts_bitmask_array[number / 32] &= ~(1 << (number % 32));
#else
        _posix_interrupts_bitmask &= ~( 1 << number );
#endif
    }
    return true;
}

/**
 * @brief
 *      Associate current thread with an interrupt handler
 *
 * This function associates the current thread with the specified interrupt
 * handler for the specified interrupt.  The thread can block, waiting for
 * notification, by calling pthread_interrupt_timedwait().  The handler can
 * cause the thread to be notified by including the INTERRUPT_NOTIFY flag in
 * its return value.  Only one thread can be associated with a handler at the
 * same time.
 *
 * The @em handler and @em context parameters should match the ones given to the
 * corresponding interrupt_register() call of the handler.
 *
 * @param number
 *      Interrupt number
 * @param handler
 *      Interrupt handler
 * @param context
 *      Interrupt handler context
 *
 * @return 0 if successful, or an error code otherwise.
 *
 * @see
 *      pthread_interrupt_disassociate()
 */
extern int  pthread_interrupt_associate ( uint32_t number, interrupt_handler_t handler, void* context )
{
    interrupt_entry_t   *entry;

    if ( (number >= PAL_ARCH_MAXINTRS) ||  (handler==NULL)  )
    {
        return  EINVAL;
    }

#if (PAL_ARCH_MAXINTRS > 32)
    if ((_posix_interrupts_bitmask_array[number / 32] & (1 << (number % 32))) == 0)
#else
    if ( ( _posix_interrupts_bitmask & ( 1 << number ) ) == 0 )
#endif
    {
        return  EINVAL;
    }

    entry = interrupt_entry_find_in_hash(number, handler, context);
    if (entry != NULL)
    {
        entry->thread = posix_self_thread();
        return 0;
    }

    return EINVAL;
}

/**
 * @brief
 *      Disassociate thread from interrupt handler
 *
 * This function cancels any existing association between the interrupt line and
 * the specified interrupt handler.
 *
 * The @em handler and @em context parameters should match the ones given to the
 * corresponding interrupt_register() call.
 *
 * @param number
 *      Interrupt number
 * @param handler
 *      Interrupt handler
 * @param context
 *      Interrupt handler context
 *
 * @return 0 if successful, or an error code otherwise.
 *
 * @see
 *      pthread_interrupt_associate()
 */
extern int  pthread_interrupt_disassociate   ( uint32_t number, interrupt_handler_t handler,  void* context )
{
    interrupt_entry_t *entry;

    if ( (number >= PAL_ARCH_MAXINTRS) || (handler==NULL) )
    {
        return  EINVAL;
    }

#if (PAL_ARCH_MAXINTRS > 32)
    if ( ( _posix_interrupts_bitmask_array[number / 32] & ( 1 << (number % 32) ) ) == 0 )
#else
    if ( ( _posix_interrupts_bitmask & ( 1 << number ) ) == 0 )
#endif
    {
        return  EINVAL;
    }

    entry = interrupt_entry_find_in_hash(number, handler, context);
    if (entry != NULL)
    {
        entry->thread = NULL;
        return 0;
    }

    return EINVAL;
}

/**
 * @brief
 *      Block current thread waiting for interrupt notification
 *
 * This function causes the calling thread to block until notified by an
 * interrupt handler (see pthread_interrupt_associate()) or when an optional
 * timeout was reached.
 *
 * If an interrupt notification was delivered to the calling thread prior to the
 * call to the pthread_interrupt_timedwait() function, and this notification has
 * not previously caused a call to the pthread_interrupt_timedwait() function to
 * be unblocked, then the calling thread is not blocked and instead the
 * pthread_interrupt_timedwait() function returns immediately.
 *
 * Under no circumstance will the function fail with a timeout if the interrupt
 * notification occurred prior to the pthread_interrupt_timedwait() call.
 *
 * @param timeout
 *      pointer to read-only timespec variable or NULL
 *
 * @return 0 if successful, or an error code otherwis.
 */
extern int  pthread_interrupt_timedwait      ( const struct timespec* timeout )
{
    posix_thread_t* self = posix_self_thread();
#if ( __POSIX_TIMEOUTS != 0 )
    clock_t ticks = 0;
    posix_timer_t timer;
    int oldcanceltype = 0;
#endif
    int err = 0;
    int i = 0;

    posix_scheduler_lock();
    posix_interrupts_disable();

    if (self->intr_notify)
    {
        // already notified
        posix_interrupts_enable();
    }
    else
    {
        self->intr_wait = 1;
        posix_thread_reset_rc();

        posix_interrupts_enable();

#if ( __POSIX_TIMEOUTS != 0 )
        if (timeout != NULL)
        {
            if( !posix_valid_timespec( timeout ) )
            {
                posix_scheduler_unlock();
                return EINVAL;
            }

            posix_timer_init(&timer, CLOCK_REALTIME, POSIX_TIMER_TIMEOUT);

            if ((ticks = posix_timespec_to_ticks( timer.clock,timeout )))
            {
                if (posix_set_timer(ticks,&timer,false) != 0)
                {
                    /* time has elapsed */
                    posix_scheduler_unlock();
                    return 0;
                }
                oldcanceltype = self->canceltype;
                self->canceltype = PTHREAD_CANCEL_DEFERRED;
            }
        }
#endif

        // block thread
        posix_thread_wait();

        /* scheduler locked */
#if ( __POSIX_TIMEOUTS != 0 )
        if (ticks)
        {
            if (self->wake_reason == POSIX_WAKE_TIMEOUT)
            {
                err = ETIMEDOUT;
            }
            else
            {
                /* thread has been awaken but not due to
                * a delay reason but due to a signal
                * or cancellation or _WAKE_TIMEDINTR */
                posix_timer_rm(&timer);
            }

            self->canceltype = oldcanceltype;
        }
#endif

        if (self->wake_reason == POSIX_WAKE_SIGNAL)
        {
            err = EINTR;
        }
        /* else : _WAKE_TIMEDINTR */
    }

    // we're done
    self->intr_notify = 0;

    posix_scheduler_unlock();
    pthread_testcancel();

    return err;
}

#endif /* ( __POSIX_KERNEL__ != 0 ) */


/*
 * Common interrupt handlers for the software platform framework
 *
 * NOTE: These handlers are used from architecture specific 'pal'
 *       implementations.
 */

// non-posix handler
void interrupts_common_handler(uint32_t number)
{
    interrupt_entry_t   *entry, *next;
    uint32_t            result;

    entry = interrupt_hash_table[number % INTERRUPTS_HASH_SIZE];
    while (entry != NULL)
    {
        if (entry->number == number)
        {
            next = entry->next;
            if (entry->is_native)
            {
                ((interrupt_native_handler_t)(entry->handler))();
                break;
            }
            else
            {
                result = entry->handler(number, entry->context);
#if ( __POSIX_KERNEL__ != 0 )
                if ( result & INTERRUPT_NOTIFY )
                {
                    posix_thread_t *th = entry->thread;

                    interrupts_notify_thread(th);
                }
#endif
                if (result & INTERRUPT_HANDLED)
                {
                    break;
                }
            }
            entry = next;
        }
        else
        {
            entry = entry->next;
        }
    }

    return;
}

#if ( __POSIX_KERNEL__ != 0 )

// posix handler
void posix_interrupts_handler(uint32_t number)
{
    // run with the scheduler locked
    _posix_sched_lock++;

#if POSIX_THREADS_DEBUG
    if( _posix_current_thread != NULL )
    {
        _posix_current_thread->ra     = _posix_resume_address;
        _posix_current_thread->spuser = _posix_user_stack;
    }
#endif

    // handle the interrupt
    interrupts_common_handler( number );

#if __POSIX_THREAD_CPUTIME
    posix_cpuclock_update();
#endif

    if( _posix_current_thread != NULL )
    {
        // this will decrease _posix_sched_lock
        posix_interrupt_schedule_point();
    }
    else
    {
        _posix_sched_lock--;
    }

    return;
}

#endif /* ( __POSIX_KERNEL__ != 0 ) */
