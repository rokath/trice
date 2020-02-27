/*! \file config.h
\brief This file is for trice specific project settings
\details adapt needed fifo size, compiler settings and 4 hardware access functions
- triceTxDataRegisterEmpty()
- triceTransmitData8()
- triceEableTxEmptyInterrupt()
- triceDisableTxEmptyInterrupt()
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h" // hardware specific stuff

///////////////////////////////////////////////////////////////////////////////
// user adaption
//

//!< a byte count for buffering traces, must be power of 2, one basic trace needs 4 bytes
#define TRICE_FIFO_SIZE 2048 
#define TRICE_START_BYTE (0xeb) //!< trice header start (chose any unusual byte)
#define TRICE_LOCAL_ADDR (0x60) //!< trice addess of this device (choose free)
#define TRICE_DISPL_ADDR (0x60) //!< trice terminal address for this device (choose free)
#define TRICE_SHORT_MEMORY 0 //!< 1 means less Flash needed but slower, set compiler switch "optimize for time" accordingly!
#ifndef TRICE_LEVEL
#define TRICE_LEVEL 100 //!< enable or disable trice code generation
#endif
//#define TREYFER_KEY { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88 } //!< enable to encrypt trice transmit data (must be done in trice-tool too)

//! Enable this for legacy projects with printf( "...%s...", ... ); statements
//! This is only for easy porting and has no advantage in time and space compared to printf
//! But you can simply exchange 'printf(...)' with 'trice(...)' without concerning about static or dynamic strings
//! Needs internally a vsnprintf() implementation what adds a few KB code size
//! If the output strings in the legacy project are static, consider splitting the
//! legacy printf() into a TRICE* sequence to avoid enabling this switch.
#define TRICE_PRINTF_ADAPTER
#define TRICE_PRINTF_ADAPTER_BUFFERSIZE 100 //!< longest legacy printf should fit here, only neede if TRICE_PRINTF_ADAPTER is defined

#define SYSTICKVAL16 SysTick->VAL //!< STM32 specific

///////////////////////////////////////////////////////////////////////////////
// compiler adaptions
//

#ifdef __GNUC__ // gnu compiler ###############################################

#define TRICE_INLINE static inline //! used for trice code

#define ALIGN4                                  //!< align to 4 byte boundary preamble
#define ALIGN4_END __attribute__ ((aligned(4))) //!< align to 4 byte boundary post declaration
#define PACKED                                  //!< pack data preamble
#define PACKED_END __attribute__ ((packed))      //!< pack data post declaration

#elif defined(__arm__) // ARMkeil IDE #########################################

#define TRICE_INLINE static inline //! used for trice code

#define ALIGN4 __align(4) //!< align to 4 byte boundary preamble
#define ALIGN4_END        //!< align to 4 byte boundary post declaration
#define PACKED __packed   //!< pack data preamble
#define PACKED_END        //!< pack data post declaration

#else // ######################################################################

// some other compliler

#endif // compiler adaptions ##################################################

///////////////////////////////////////////////////////////////////////////////
// hardware specific interface functions tested on NUCLEO-STM32F030
//

/*! Save interrupt state and disable Interrupts
\details Workaround for ARM Cortex M0 and M0+
\li __get_PRIMASK() is 0 when interrupts are enabled globally
\li __get_PRIMASK() is 1 when interrupts are disabled globally
If trices are used only outside critical sections or interrupts
you can leave this macro pair empty for more speed.
*/
#define TRICE_ENTER_CRITICAL_SECTION { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); {

/*! Restore interrupt state
\details Workaround for ARM Cortex M0 and M0+
\li __get_PRIMASK() is 0 when interrupts are enabled globally
\li __get_PRIMASK() is 1 when interrupts are disabled globally
If trices are used only outside critical sections or interrupts
you can leave this macro pair empty for more speed.
*/
#define TRICE_LEAVE_CRITICAL_SECTION } __set_PRIMASK(primaskstate); }

/*! Check if a new byte can be written into trice transmit register.
\retval 0 == not empty
\retval !0 == epmty
User must provide this function.
*/
TRICE_INLINE uint32_t triceTxDataRegisterEmpty( void ){
    return LL_USART_IsActiveFlag_TXE( USART2 );
}

/*! Write value d into trice transmit register.
\param d byte to transmit
User must provide this function.
*/
TRICE_INLINE void triceTransmitData8( uint8_t d ){
    LL_USART_TransmitData8( USART2, d);
}

/*! Allow interrupt for empty trice data transmit register.
User must provide this function.
*/
TRICE_INLINE void triceEableTxEmptyInterrupt( void ){
    //LL_USART_EnableIT_TXE( USART2 ); // In STM32 done imlizit by writing data
}

/*! Disallow interrupt for empty trice data transmit register.
User must provide this function.
*/
TRICE_INLINE void triceDisableTxEmptyInterrupt( void ){
    LL_USART_DisableIT_TXE( USART2 );
}

#ifdef __cplusplus
}
#endif

#endif /* TRICE_CONFIG_H_ */
