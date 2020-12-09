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

/**
 * @file
 * Timing and timer services.
 *
 * These system utilities provide standard timing ( freq, delay and elapse )
 * services that can be used for timing purposes and a simple way to install
 * periodic software timers in the system.
 *
 */

#ifndef _H_TIMING
#define _H_TIMING

#ifdef  __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include "timing_cfg.h"

extern uint32_t     freq_hz( void );
extern void         delay_ns ( uint64_t ns );
extern void         delay_us ( uint64_t us );
extern void         delay_ms ( uint64_t ms );
extern uint64_t		clock_ns (void);
extern uint64_t		clock_us (void);
extern uint64_t		clock_ms (void);
extern uint64_t     elapsed_time_ns ( uint64_t since_ns );
extern uint64_t     elapsed_time_us ( uint64_t since_us );
extern uint64_t     elapsed_time_ms ( uint64_t since_ms );

#ifdef  __cplusplus
}
#endif

#endif
