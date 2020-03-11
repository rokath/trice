/*! \file msTicks.c
\brief  hardware specific implementation of tick functionality
\details usable for timing tasks
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/


#include "config.h"

#if 1 // def TRACELOG
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
//#include <CriticalSection.h>
//#include "Tr16.h"
//#include "TraceUtilities.h"
#include "Fifo.h"
#endif // #ifdef TRACELOG

//#include "DefaultMacros.h"
#include "msTicks.h"


#ifdef WINDOWS_OS
#include "Windows.h" // sysinfoapi.h for https://docs.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-gettickcount

#define CHECK_AND_ENABLE_INTERRUPTS // for STM32 needed
#endif


volatile uint64_t MillisecondsSincePowerOn = 0u; //!< use for timings
volatile uint8_t MsFlag = 0u; //!< use to detect millicecond event (systick timer)

#ifdef HAVE_NO_MILLISECONDS_INTERRUPT
#include "usTicks.h"


/*! workaround if no 1ms interrupt is configured */
void Serve1msTicks( void )
{
    static uint32_t Timer = 0;
    if( 0xffffffffu == ElapsedUsThenRestartTimer( 1000u, &Timer ) )
    {
        MillisecondsSincePowerOn++;
    }
}


#endif // #ifdef HAVE_NO_MILLISECONDS_INTERRUPT


/*! \return current millisecond count since power on 
- wraps after 49,71 days
*/
uint32_t ReadMsTickCount( void )
{
#ifdef WINDOWS_OS
    MillisecondsSincePowerOn = GetTickCount();
#endif
    return (uint32_t)MillisecondsSincePowerOn;
}


/*! Compute the MsTick difference and consider wraps and counting direction
\details wraps after 49,7 days
\param StartMs first tick 
\param StopMs second tick
\return time delta
*/
uint32_t MsDelta( uint32_t StartMs, uint32_t StopMs )
{
    uint32_t Delta = (uint32_t)( StopMs - StartMs ); // Ms counts forwards here and is 32 bits long
    return Delta;
}


/*! store current ms tick in a timer variable
\param pTimer timer address 
*/
void StartMsTimer( uint32_t* pTimer )
{
    *pTimer = ReadMsTickCount();
}


/*! compute ms count since a timer started
\param pTimer timer address
\return elapsed ticks
*/
uint32_t ElapsedMs( uint32_t* pTimer )
{
    uint32_t Ms = ReadMsTickCount();
    uint32_t Elapsed = MsDelta( *pTimer, Ms ); 
    //CHECK_AND_ENABLE_INTERRUPTS
    return Elapsed;
}


/*! \brief used for measurement of periodical events in millisecond ticks
\details for example place inside an interrupt routine which is triggered by external events
The timer is restarted with each call of this function.
\param pTimer timer address
\return time elapsed
*/
uint32_t ElapsedMsAndRestartTimer( uint32_t* pTimer )
{
    uint32_t Ms = ReadMsTickCount(); // counts full 32 bit
    uint32_t Elapsed = MsDelta( *pTimer, Ms ); 
    *pTimer = Ms; // restart timer
    return Elapsed;
}


/*! \brief handling of timed events 
\details As example place fn inside an endless loop to execute code in time intervals.
The timer is restarted only with call of this function when the tick limit was reached.
\param MsLimit tick to elapse before timer is restarted
\param pTimer timer address
\retval != 0xffffffffu ticks elapsed since last timer start
\retval == 0xffffffffu timer tick limit reached and timer was restarted
*/
uint32_t ElapsedMsThenRestartTimer( uint32_t MsLimit, uint32_t* pTimer )
{
    uint32_t Ms = ReadMsTickCount();
    uint32_t Elapsed = MsDelta( *pTimer, Ms ); 
    if( Elapsed >= MsLimit ) // at ms level euality is possible
    {
        *pTimer = Ms; // restart timer
        return 0xffffffffu;
    }
    return Elapsed; // one more to avoid return of 0 here
}


/*! empty function, no operation
*/
void NoopMsTicks( void )
{
    // do nothing
}


/*! Return only after specified number of milliseconds.
During delay call a function
\param Ms wait time
\param pFnCallback function to be called during delay. Could be NoopMsTicks():
*/
void DelayMs( uint32_t Ms, void pFnCallback(void) )
{
    static uint32_t Timer;
    StartMsTimer( &Timer );
    do
    {
        if( pFnCallback )
        {
            pFnCallback();
        }
    }
    while( ElapsedMs( &Timer ) < Ms );
}
