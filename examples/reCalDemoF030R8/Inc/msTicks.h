/*! \file msTicks.h
\brief  hardware specific implementation of milliseconds tick functionality
\details usable for timing tasks on milliseconds basis
It is your responsibility to increment variable MillisecondsSincePowerOn 
every ms. You should configure a timer interrupt for that. As workaround
define HAVE_NO_MILLISECONDS_INTERRUPT and call Serve1msTicks() at least
1 time per millisecond.
\todo lint code
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/
#ifndef MSTICKS_H_
#define MSTICKS_H_

#ifdef __cplusplus
extern "C" {
#endif


#if 1 //def AUTOMATIC_HEADER_INLUSION_
// use this for doxygen, define AUTOMATIC_HEADER_INLUSION_ in
// .doxygen file section PREDEFINED
#include "config.h" // should be first include file, at least befor other project specific files
#include <stdint.h>
#endif // #ifdef AUTOMATIC_HEADER_INLUSION_

#define MILLI_SECONDS(n) (n) //!< for better code readability

#ifndef WINDOWS_OS
extern volatile uint64_t MillisecondsSincePowerOn; //extern volatile uint32_t MillisecondsSincePowerOn; //!< use for ms timings
#endif
extern volatile uint8_t MsFlag; //!< use to detect millicecond event (systick timer)


#ifdef HAVE_NO_MILLISECONDS_INTERRUPT
/*! workaround if no 1ms interrupt is configured to increment MillisecondsSincePowerOn */
void Serve1msTicks( void );
#endif // #ifdef HAVE_NO_MILLISECONDS_INTERRUPT


uint32_t ReadMsTickCount( void );
uint32_t MsDelta( uint32_t StartMs, uint32_t StopMs );
void StartMsTimer( uint32_t* pTimer );
uint32_t ElapsedMs( uint32_t* pTimer );
uint32_t ElapsedMsAndRestartTimer( uint32_t* pTimer );
uint32_t ElapsedMsThenRestartTimer( uint32_t MsLimit, uint32_t* pTimer );
void NoopMsTicks( void );
void DelayMs( uint32_t Ms, void pFnCallback(void) );

#if 0 //ndef RETURN_ON_MS_TIMEOUT
/** us should be safely be unter 1000, otherwise use RETURN_ON_MS_TIMEOUT
*/
#define RETURN_VALUE_ON_MS_TIMEOUT( ms, value ) do{ \
    uint32_t ToTimer = 0; \
    StartMsTimer( &ToTimer ); \
    if( (us) > ElapsedMs( &ToTimer ) ) { \
        return value; \
    } \
} while(0)
#endif // #ifndef RETURN_ON_US_TIMEOUT


#ifdef __cplusplus
}
#endif

#endif /* MSTICKS_H_ */

