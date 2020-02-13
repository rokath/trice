/*! \file traceConfig.h
\brief This file is for traceLog specific project settings
\details adapt needed fifo size, compiler settings and 4 hardware access functions
- traceLogTxDataRegisterEmpty()
- traceLogTransmitData8()
- traceLogEableTxEmptyInterrupt()
- traceLogDisableTxEmptyInterrupt()
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRACECONFIG_H_
#define TRACECONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h" // hardware specific stuff

///////////////////////////////////////////////////////////////////////////////
// user adaption
//

//!< a byte count for buffering traces, must be power of 2, one basic trace needs 4 bytes
#define TL_FIFO_SIZE 512 
#define TL_START_BYTE (0xeb) //!< traceLog header start (chose any unusual byte)
#define TL_LOCAL_ADDR (0x60) //!< traceLog addess of this device (choose free)
#define TL_DISPL_ADDR (0x61) //!< traceLog terminal address for this device (choose free)
//#define TRACELOG_OFF //!< enable this line to disable traceLog code generation

#define SYSTICKVAL16 SysTick->VAL //!< STM32 specific

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
If traceLogs are used only outside critical sections or interrupts
you can leave this macro pair empty for more speed.
*/
#define TL_ENTER_CRITICAL_SECTION { uint32_t primaskstate = __get_PRIMASK(); __disable_irq(); {

/*! Restore interrupt state
\details Workaround for ARM Cortex M0 and M0+
\li __get_PRIMASK() is 0 when interrupts are enabled globally
\li __get_PRIMASK() is 1 when interrupts are disabled globally
If traceLogs are used only outside critical sections or interrupts
you can leave this macro pair empty for more speed.
*/
#define TL_LEAVE_CRITICAL_SECTION } __set_PRIMASK(primaskstate); }

#else // ######################################################################

// some other compliler

#endif // compiler adaptions ##################################################

///////////////////////////////////////////////////////////////////////////////
// hardware specific interface functions tested on NUCLEO-STM32F030
//

/*! Check if a new byte can be written into traceLog transmit register.
\retval false == not empty
\retval true == epmty
User must provide this function.
*/
TL_INLINE uint32_t traceLogTxDataRegisterEmpty( void ){
    return LL_USART_IsActiveFlag_TXE( USART2 );
}

/*! Write value d into traceLog transmit register.
\param d byte to transmit
User must provide this function.
*/
TL_INLINE void traceLogTransmitData8( uint8_t d ){
    LL_USART_TransmitData8( USART2, d);
}

/*! Allow interrupt for empty traceLog data transmit register.
User must provide this function.
*/
TL_INLINE void traceLogEableTxEmptyInterrupt( void ){
    //LL_USART_EnableIT_TXE( USART2 ); // In STM32 done imlizit by writing data
}

/*! Disallow interrupt for empty traceLog data transmit register.
User must provide this function.
*/
TL_INLINE void traceLogDisableTxEmptyInterrupt( void ){
    LL_USART_DisableIT_TXE( USART2 );
}

#ifdef __cplusplus
}
#endif

#endif /* CONFIG_H_ */
