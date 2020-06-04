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
**  Timer services.
**
** ------------------------------------------------------------
*/

#include <stddef.h>
#include <timers.h>
#include <pal.h>
#include "timing_cfg.h"

#if ( TIMING_USETIMERS != 0 )


/**
 * @brief
 *      registers a periodic software handler
 *
 *      Installs a periodic software timer handler that will execute every
 *      @em freq microseconds. The application is supposed to continue
 *      execution to do something else after the registration.
 *
 *      This software timer handler runs asynchronously to the application.
 *      In single-threaded applications, the software handler runs at interrupt
 *      level (timer interrupt on the chip as clock source).
 *      In multi-threaded applications, this software handler runs in the OS clock
 *      interrupt handler.
 *
 *      In single-threaded applications you can configure the timers base periodicity.
 *      In multi-threaded applications, the timers base periodicity is the
 *      OS system clock.  In either case, the period for all your software timers
 *      should be best a multiple of the timer's base periodicity.
 *
 * @param context
 *      timer handler context
 * @param freq
 *      periodicity of your software timer (in microscs )
 * @param handler
 *      timer handler
 * @return
 *      Upon success the function returns a not-NULL timer id, NULL otherwise
 *
 */
extern void*    timer_register_handler   (void* context, int64_t freq, timer_handler_t handler)
{
    int64_t ofreq;

#if __POSIX_KERNEL__ == 0
    #include "timing_cfg.h"
    ofreq = (freq) /(TIMING_BASE);
#else
    #include "posix_threads_cfg.h"
    ofreq = (freq) /(POSIX_THREADS_OSTICKMICROSCS) ;
#endif
    if ( ofreq == 0 ) return NULL;
    return pal_timer_register_handler( context, ofreq, handler );
}

/**
 * @brief
 *      de-registers a periodic software handler
 *
 * @param id
 *      timer id
 * @return
 *      true if success, false otherwise
 *
 */
extern bool     timer_deregister_handler (void* id)
{
    return pal_timer_deregister_handler( id );
}

#endif

