/*! \file triceConfigStm32_HAL.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_CONFIG_CRITICAL_SECTION_H_
#define TRICE_CONFIG_CRITICAL_SECTION_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "main.h" // hardware specific stuff

#ifdef TRICE_INSIDE_INTERRUPTS

//! Save interrupt state and disable Interrupts
//! \details Workaround for ARM Cortex M0 and M0+
//! \li __get_PRIMASK() is 0 when interrupts are enabled globally
//! \li __get_PRIMASK() is 1 when interrupts are disabled globally
//! If trices are used only outside critical sections or interrupts
//! you can leave this macro pair empty for more speed.
#define TRICE_ENTER_CRITICAL_SECTION { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); {

//! Restore interrupt state
//! \details Workaround for ARM Cortex M0 and M0+
//! \li __get_PRIMASK() is 0 when interrupts are enabled globally
//! \li __get_PRIMASK() is 1 when interrupts are disabled globally
//! If trices are used only outside critical sections or interrupts
//! you can leave this macro pair empty for more speed.
#define TRICE_LEAVE_CRITICAL_SECTION } __set_PRIMASK(primaskstate); }

#else // #ifdef TRICE_INSIDE_INTERRUPTS

#define TRICE_ENTER_CRITICAL_SECTION {
#define TRICE_LEAVE_CRITICAL_SECTION }

#endif // #else // #ifdef TRICE_INSIDE_INTERRUPTS


#ifdef __cplusplus
}
#endif

#endif /* TRICE_CONFIG_CRITICAL_SECTION_H_ */
