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

#include <pal.h>

static inline void pal_delay( uint64_t wait, uint64_t time_factor )
{
    uint64_t    delay              = wait * pal_freq_hz() / time_factor;
    uint64_t    end                = pal_clock() + delay;

    while ( pal_clock() < end) /* wait */;
}

void pal_delay_ns ( uint64_t ns )
{
    pal_delay( ns, 1000L*1000L*1000L );
}

void pal_delay_us ( uint64_t us )
{
    pal_delay( us, 1000L * 1000L );
}

void pal_delay_ms ( uint64_t ms ) {
    pal_delay( ms, 1000L );
}

