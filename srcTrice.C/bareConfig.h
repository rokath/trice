/*! \file triceConfig.h
\brief This file is for trice specific project settings
\details adapt needed fifo size, compiler settings and 4 hardware access functions
- triceTxDataRegisterEmpty()
- triceTransmitData8()
- triceEnableTxEmptyInterrupt()
- triceDisableTxEmptyInterrupt()
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef BARE_CONFIG_H_
#define BARE_CONFIG_H_

#include <stdint.h>
#include "main.h" // hardware specific stuff

#ifdef __cplusplus
extern "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// main settings
//

//! TRICE_FIFO_BYTE_SIZE must be a power of 2, one trice needs 4 to 32 or one day more bytes.
//! The fifo has to hold trice bursts until they are transmitted.
//! It is transmitted with lower priority in the background for example with the UART tx interrupt.
#define TRICE_FIFO_BYTE_SIZE 512 

//! Set according to hardware
#define TRICE_UART USART2 

//! Set to MORE_FLASH_AND_SPEED, LESS_FLASH_AND_SPEED or NO_CODE
#define TRICE_CODE MORE_FLASH_AND_SPEED

//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// compiler adaption
//

#if defined( __GNUC__ ) /* gnu compiler ###################################### */ \
 || defined(__IAR_SYSTEMS_ICC__) /* IAR compiler ############################# */ \
 || defined(__TASKING__) /* TASKING compiler (same bugs as GNU!)############## */

#define TRICE_INLINE static inline //! used for trice code

#define ALIGN4                                  //!< align to 4 byte boundary preamble
#define ALIGN4_END __attribute__ ((aligned(4))) //!< align to 4 byte boundary post declaration
#define PACKED                                  //!< pack data preamble
#define PACKED_END __attribute__ ((packed))     //!< pack data post declaration

//! TRICE_ENTER_CRITICAL_SECTION saves interrupt state and disables Interrupts.
//! \details Workaround for ARM Cortex M0 and M0+:
//! \li __get_PRIMASK() is 0 when interrupts are enabled globally.
//! \li __get_PRIMASK() is 1 when interrupts are disabled globally.
//! If trices are used only outside critical sections or interrupts,
//! you can leave this macro empty for more speed. Use only '{' in that case.
#define TRICE_ENTER_CRITICAL_SECTION { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); {

//! TRICE_LEAVE_CRITICAL_SECTION restores interrupt state.
//! \details Workaround for ARM Cortex M0 and M0+:
//! \li __get_PRIMASK() is 0 when interrupts are enabled globally.
//! \li __get_PRIMASK() is 1 when interrupts are disabled globally.
//! If trices are used only outside critical sections or interrupts,
//! you can leave this macro pair empty for more speed. Use only '}' in that case.
#define TRICE_LEAVE_CRITICAL_SECTION } __set_PRIMASK(primaskstate); }

#elif defined(__arm__) // ARMkeil IDE #########################################

#define TRICE_INLINE static inline //! used for trice code

#define ALIGN4 __align(4) //!< align to 4 byte boundary preamble
#define ALIGN4_END        //!< align to 4 byte boundary post declaration
#define PACKED __packed   //!< pack data preamble
#define PACKED_END        //!< pack data post declaration

//! TRICE_ENTER_CRITICAL_SECTION saves interrupt state and disables Interrupts.
//! \details Workaround for ARM Cortex M0 and M0+:
//! \li __get_PRIMASK() is 0 when interrupts are enabled globally.
//! \li __get_PRIMASK() is 1 when interrupts are disabled globally.
//! If trices are used only outside critical sections or interrupts,
//! you can leave this macro empty for more speed. Use only '{' in that case.
#define TRICE_ENTER_CRITICAL_SECTION { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); {

//! TRICE_LEAVE_CRITICAL_SECTION restores interrupt state.
//! \details Workaround for ARM Cortex M0 and M0+:
//! \li __get_PRIMASK() is 0 when interrupts are enabled globally.
//! \li __get_PRIMASK() is 1 when interrupts are disabled globally.
//! If trices are used only outside critical sections or interrupts,
//! you can leave this macro pair empty for more speed. Use only '}' in that case.
#define TRICE_LEAVE_CRITICAL_SECTION } __set_PRIMASK(primaskstate); }

#elif 1 // ####################################################################
#error "add new compiler here"
#else // ######################################################################
#error unknown compliler
#endif // compiler adaptions ##################################################

//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// hardware specific interface functions
//

//! Check if a new byte can be written into trice transmit register.
//! \retval 0 == not empty
//! \retval !0 == empty
//! User must provide this function.
TRICE_INLINE uint32_t triceTxDataRegisterEmpty( void ){
    return LL_USART_IsActiveFlag_TXE( TRICE_UART );
}

//! Write value v into trice transmit register.
//! \param v byte to transmit
//! User must provide this function.
TRICE_INLINE void triceTransmitData8( uint8_t v ){
    LL_USART_TransmitData8( TRICE_UART, v);
}

//! Allow interrupt for empty trice data transmit register.
//! User must provide this function.
TRICE_INLINE void triceEnableTxEmptyInterrupt( void ){
    LL_USART_EnableIT_TXE( TRICE_UART );
}

//! Disallow interrupt for empty trice data transmit register.
//! User must provide this function.
TRICE_INLINE void triceDisableTxEmptyInterrupt( void ){
    LL_USART_DisableIT_TXE( TRICE_UART );
}

//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// trice time measurement
//

#if defined( __arm__ )       /* Defined by GNU C and RealView */ \
    || defined( __thumb__ )  /* Defined by GNU C and RealView in Thumb mode */ \
    || defined( _ARM )       /* Defined by ImageCraft C */ \
    || defined( _M_ARM )     /* Defined by Visual Studio */ \
    || defined( _M_ARMT )    /* Defined by Visual Studio in Thumb mode */ \
    || defined( __arm )      /* Defined by Diab */ \
    || defined( __ICCARM__ ) /* IAR */ \
	|| defined( __CC_ARM )   /* ARM's (RealView) compiler */ \
    || defined( __ARM__ )    /* TASKING VX ARM toolset C compiler */ \
    || defined( __CARM__ )   /* TASKING VX ARM toolset C compiler */ \
    || defined( __CPARM__ )  /* TASKING VX ARM toolset C++ compiler */
#define SYSTICKVAL32 (*(volatile uint32_t*)0xE000E018UL)
#else
#error "unknown architecture"
#endif

#define SYSTICKVAL16 ((uint16_t)SYSTICKVAL32)

//
///////////////////////////////////////////////////////////////////////////////

//!< tests
void triceCheckSet( int index ); 

#ifdef __cplusplus
}
#endif

#endif /* BARE_CONFIG_H_ */
