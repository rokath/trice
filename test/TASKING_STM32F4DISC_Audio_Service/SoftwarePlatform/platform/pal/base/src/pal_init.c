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
** ------------------------------------------------------------
*/

#include <time.h>
#include <stdbool.h>

#include <pal.h>

// Software Platform init handler
void pal_init (void)
{
    pal_architecture_processor_init();

    /* c-library implementation */
    __clocks_per_sec = PAL_CLOCKHZ;

    pal_architecture_interrupts_init();

#if (__TIMERS__ > 0) && ( __POSIX_KERNEL__ == 0 )
    pal_architecture_timer_interrupt_init();
    pal_architecture_timer_interrupt_enable();
#endif

    pal_architecture_clock_init();

#if ( __POSIX_KERNEL__ == 0 )
    pal_architecture_interrupts_enable();
#endif
}

