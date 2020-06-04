/* ------------------------------------------------------------
**
**  Copyright (c) 2013-2015 Altium Limited
**
**  This software is the proprietary, copyrighted property of
**  Altium Ltd. All Right Reserved.
**
**  SVN revision information:
**  $Rev: 14907 $:
**  $Date: 2015-01-19 13:30:51 +0100 (Mon, 19 Jan 2015) $:
**
**
**  Processor Abstraction Layer. Part of the Software Platform.
**  These interfaces are not meant to be used directly from
**  application code.
**
** ------------------------------------------------------------
*/

#ifndef _PAL_H
#define _PAL_H

#include <stdint.h>
#include <stdbool.h>
#include "pal_arch.h"
#include "pal_cfg.h"

#define __TIMERS__                  PAL_USETIMERS
/* PAL_USETIMERS is set by the timing service, so we know it is available */
#if  ( __TIMERS__ > 0 )
#include "timing_cfg.h"
#endif

#ifdef PAL_ARCH_CLOCKHZ  /* allow target-specific override of clock settings */
#undef PAL_CLOCKHZ
#define PAL_CLOCKHZ                 PAL_ARCH_CLOCKHZ
#endif

extern void             interrupts_common_handler (uint32_t);

#if ( (__TIMERS__ != 0) || (__POSIX_KERNEL__ !=0 ) )
#define pal_timer_interrupt_acknowledge             pal_architecture_timer_interrupt_acknowledge
#define pal_timer_interrupt_enable                  pal_architecture_timer_interrupt_enable
#define pal_timer_interrupt_disable                 pal_architecture_timer_interrupt_disable
#define pal_timer_interrupt_start                   pal_architecture_timer_interrupt_start
#define pal_timer_interrupt_stop                    pal_architecture_timer_interrupt_stop
#define pal_timer_interrupt_init                    pal_architecture_timer_interrupt_init
#endif

#if ( __POSIX_KERNEL__ != 0 )
extern int              pal_lsbit32_index(uint32_t val32);
extern int              pal_lsbit64_index(uint64_t val64);
extern int              pal_msbit32_index(uint32_t val32);
extern int              pal_msbit64_index(uint64_t val64);
#endif

#if  ( __TIMERS__ > 0 )
#define PAL_TIMERS_BASE              TIMING_BASE
#define PAL_MAXTIMERS                TIMING_MAX
typedef void (*pal_timer_handler_t) (void *);
extern bool  pal_timers_init (void );
extern void* pal_timer_register_handler   (void*, int64_t, pal_timer_handler_t);
extern bool  pal_timer_deregister_handler (void*);
extern void  pal_timers_common_handler    (void);
#endif

#if ( __POSIX_KERNEL__ != 0 )
extern volatile int      _posix_irq_level;
extern volatile int      _posix_sched_lock;
extern void     posix_clock_interrupt_handler( void );
extern void     posix_interrupts_handler (uint32_t);
#endif // ( __POSIX_KERNEL__ == 0 )

static inline uint32_t pal_freq_hz( void ) { return (uint32_t) PAL_CLOCKHZ; }

extern uint64_t clock(void);
#define pal_clock   clock

static inline uint64_t pal_clock_ns (void)
{
    return ( pal_clock() * (1000L*1000L*1000L) / PAL_CLOCKHZ );
}

static inline uint64_t pal_clock_us (void)
{
    return ( pal_clock() * (1000L*1000L) / PAL_CLOCKHZ );
}

static inline uint64_t pal_clock_ms (void)
{
    return ( pal_clock() * (1000L) / PAL_CLOCKHZ );
}

extern void     pal_delay_ns ( uint64_t ns );
extern void     pal_delay_us ( uint64_t us );
extern void     pal_delay_ms ( uint64_t ms );

#endif //_PAL_H
