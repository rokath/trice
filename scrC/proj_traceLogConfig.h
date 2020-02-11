/*! \file traceConfig.h
\brief This file is for tracelog specific project settings
\details adapt needed fifo size and add your compiler settings
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRACECONFIG_H_
#define TRACECONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h" // hardware specific stuff

#define TL_FIFO_SIZE 512 //!< a byte count for buffering traces, must be power of 2, one basic trace needs 4 bytes
#define TL_START_BYTE (0xeb) //!< tracelog header start (chose any unusual byte)
#define TL_LOCAL_ADDR (0x60) //!< tracelog addess of this device (choose free)
#define TL_DISPL_ADDR (0x61) //!< tracelog terminal address for this device (choose free)
//#define TRACELOG_OFF

///////////////////////////////////////////////////////////////////////////////
// compiler adaptions
//

#ifdef __GNUC__ // gnu compiler ###############################################

#define TL_INLINE static inline // todo

#define ALIGN4
#define ALIGN4_END __attribute__ ((aligned(4)))
#define PACKED
#define PACKED_END __attribute__ ((packed))

//! Save interrupt state and disable Interrupts
#define TL_ENTER_CRITICAL_SECTION { // todo

//! Restore interrupt state
#define TL_LEAVE_CRITICAL_SECTIO } // todo

#elif defined(__arm__) // ARMkeil IDE

#define TL_INLINE static inline

#define ALIGN4 __align(4) //!< ARM Keil syntax ################################
#define ALIGN4_END
#define PACKED __packed
#define PACKED_END

/*! Save interrupt state and disable Interrupts
\details Workaround for ARM Cortex M0 and M0+
\li __get_PRIMASK() is 0 when interrupts are enabled globally
\li __get_PRIMASK() is 1 when interrupts are disabled globally
If tracelogs are used only outside critical sections or interrupts
you can leave this macro pair empty for more speed.
*/
#define TL_ENTER_CRITICAL_SECTION { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); {

/*! Restore interrupt state
\details Workaround for ARM Cortex M0 and M0+
\li __get_PRIMASK() is 0 when interrupts are enabled globally
\li __get_PRIMASK() is 1 when interrupts are disabled globally
If tracelogs are used only outside critical sections or interrupts
you can leave this macro pair empty for more speed.
*/
#define TL_LEAVE_CRITICAL_SECTION } __set_PRIMASK(primaskstate); }

#else // ######################################################################

// some other compliler

#endif // compiler adaptions

#ifdef __cplusplus
}
#endif

#endif /* CONFIG_H_ */
