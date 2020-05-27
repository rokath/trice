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

#ifndef TIMESPEC_UTIL_H_
#define TIMESPEC_UTIL_H_

#if __POSIX_KERNEL__
    #include <sys/types.h>
    #include <stdint.h>
	#include <time.h>


    #define MSEC_PER_SEC   1000
    #define NSEC_PER_MSEC (1000 * 1000)
    #define NSEC_PER_SEC  (1000 * 1000 * 1000)

    static inline void
    timespec_add(struct timespec * ts_a, struct timespec * ts_b)
    {
        ts_a->tv_sec  += ts_b->tv_sec;
        ts_a->tv_nsec += ts_b->tv_nsec;
        while (ts_a->tv_nsec > NSEC_PER_SEC)
        {
            ts_a->tv_sec++;
            ts_a->tv_nsec -= NSEC_PER_SEC;
        }
    }

    static inline void
    timespec_from_ms(struct timespec * ts, int32_t ms)
    {
        ts->tv_sec  =  ms / MSEC_PER_SEC;
        ts->tv_nsec = (ms % MSEC_PER_SEC) * NSEC_PER_MSEC;
    }

    static inline void
    timespec_from_clock(struct timespec * ts, clock_t clk)
    {
        ts->tv_sec  = (time_t)(clk / CLOCKS_PER_SEC);
        ts->tv_nsec = (long)((clk % CLOCKS_PER_SEC) * 1000000000 / CLOCKS_PER_SEC);
    }

    static inline int32_t
    timespec_delta_ms(struct timespec * ts_a, struct timespec * ts_b)
    {
        int32_t delta_ms;

        delta_ms  = (ts_a->tv_sec  - ts_b->tv_sec)  * MSEC_PER_SEC;
        delta_ms += (ts_a->tv_nsec - ts_b->tv_nsec) / NSEC_PER_MSEC;
        return delta_ms;
    }
#endif /* __POSIX_KERNEL__ */

#endif /* TIMESPEC_UTIL_H_ */
