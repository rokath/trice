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
**  Timing services.
**
** ------------------------------------------------------------
*/

#include <stddef.h>
#include <pal.h>
#include <timing.h>

/**
 * @brief
 *      returns the clock frequency (in Hz)
 *
 * @return
 *      clock frequency
 *
 */
extern uint32_t freq_hz( void )
{
    return pal_freq_hz();
}

/**
 * @brief
 *      delays (looping) for @em ns nanoseconds
 *
 * @param ns
 *      delay time in nanoseconds
 *
 * @return
 *      nothing
 *
 */
extern void   delay_ns ( uint64_t ns )
{
    pal_delay_ns(ns);
    return;
}

/**
 * @brief
 *      delays (looping) for @em us microseconds
 *
 * @param us
 *      delay time in microseconds
 *
 * @return
 *      nothing
 *
 */
extern void   delay_us ( uint64_t us )
{
    pal_delay_us(us);
    return;
}

/**
 * @brief
 *      delays (looping) for @em ms miliseconds
 *
 * @param ms
 *      delay time in miliseconds
 *
 * @return
 *      nothing
 *
 */
extern void   delay_ms ( uint64_t ms )
{
    pal_delay_ms(ms);
    return;
}

/**
 * @brief
 *      clock value in nanoseconds
 *
 * @return
 *      clock value in nanoseconds
 *
 */
extern uint64_t     clock_ns (void)
{
    return pal_clock_ns();
}

/**
 * @brief
 *      clock value in microseconds
 *
 * @return
 *      clock value in microseconds
 *
 */
extern uint64_t     clock_us (void)
{
    return pal_clock_us();
}

/**
 * @brief
 *      clock value in miliseconds
 *
 * @return
 *      clock value in miliseconds
 *
 */
extern uint64_t     clock_ms (void)
{
    return pal_clock_ms();
}


/**
 * @brief
 *      returns elapsed time in ns since last @em since_ns mark
 *
 * @param since_ns
 *      ns mark
 *
 * @return
 *      The elapsed time since the reference mark
 *
 */
extern uint64_t  elapsed_time_ns ( uint64_t since_ns )
{
    return clock_ns() - since_ns;
}

/**
 * @brief
 *      returns elapsed time in us since last @em since_us mark
 *
 * @param since_us
 *      us mark
 *
 * @return
 *      The elapsed time since the reference mark
 *
 */
extern uint64_t  elapsed_time_us ( uint64_t since_us )
{
    return clock_us() - since_us;
}

/**
 * @brief
 *      returns elapsed time in ms since last @em since_ms mark
 *
 * @param since_ms
 *      ms mark
 *
 * @return
 *      The elapsed time since the reference mark
 *
 */
extern uint64_t  elapsed_time_ms ( uint64_t since_ms )
{
    return clock_ms() - since_ms;
}



