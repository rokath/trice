/*! \file triceConfig.h
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
// internal values
//

//! used as TRICE_CODE macro option for more flash occupation, but decreases execution time
#define MORE_FLASH_AND_SPEED 30 //!< value is only to distinguish from LESS_FLASH and NO_CODE

//! used as TRICE_CODE macro option for less flash occupation, but increases execution time
#define LESS_FLASH_AND_SPEED 20 //!< value is only to distinguish from MORE_FLASH and NO_CODE

//! used as TRICE_CODE macro option for no trice code generation
#define NO_CODE 10 //!< value is only to distinguish from MORE_FLASH and LESS_FLASH

//! used as TRICE_STRINGS macro option for no runtime string support at all (smallest code size)
#define NONE_RUNTIME  100 //!< value is only to distinguish from RARE_RUNTIME and FULL_RUNTIME

//! used as TRICE_STRINGS macro option for rare strings runtime support (small code size)
//! This is fine if sometimes even long strings occur at runtime
#define RARE_RUNTIME 200 //!< value is only to distinguish from NONE_RUNTIME and FULL_RUNTIME

//! used as TRICE_STRINGS macro option for often and long strings runtime support (more code)
//! support for effective runtime string transfer, increments code size, so enable only if you need the effective string transfer 
#define FULL_RUNTIME  300 //!< value is only to distinguish from RARE_RUNTIME and NONE_RUNTIME

///////////////////////////////////////////////////////////////////////////////
// user adaption
//

//!< a byte count for buffering traces, must be power of 2, one basic trace needs 4 bytes
#define TRICE_FIFO_SIZE 4096 //!< must be a power of 2, one trice needs 4 to 32 bytes, must hold trice bursts until they are transmitted, fifo is transmitted with lower priority
#define TRICE_START_BYTE (0xeb) //!< trice header start (chose any unusual byte)
#define TRICE_LOCAL_ADDR (0x60) //!< trice addess of this device (choose free)
#define TRICE_DISPL_ADDR (0x60) //!< trice terminal address for this device (choose free)

#ifndef TRICE_CODE
//! set kind of TRICE code generation globally
//! set compiler switch "optimize for time" accordingly!
//! to switch of TRICE code generation file specific add `#define TRICE_CODE NO_CODE`
//! at start of file before including trice.h
#define TRICE_CODE MORE_FLASH_AND_SPEED // options: NO_CODE, LESS_FLASH_AND_SPEED, MORE_FLASH_AND_SPEED 
#endif

//! setting for string support code
#define TRICE_STRINGS FULL_RUNTIME //! options: NONE_RUNTIME, RARE_RUNTIME, FULL_RUNTIME

//! enable encryption here, adds about 150 bytes code
//! call trice tool with log switch "-key your_password -show" and put passphrase here
//#define ENCRYPT XTEA_KEY( a9, 4a, 8f, e5, cc, b1, 9b, a6, 1c, 4c, 08, 73, d3, 91, e9, 87 ); //!< -key test

#if TRICE_STRINGS == FULL_RUNTIME

#ifdef ENCRYPT
#error todo: encryption does not yet work with FULL_RUNTIME runtime strings, set TRICE_STRINGS to RARE_RUNTIME
#endif

//! Must be able to hold all in a burst arriving strings including 10 bytes for each string.
//! Buffer is transmitted to port with highest priority.
#define RUNTIME_STRING_FIFO_SIZE 1000
#endif

//! Enable this for legacy projects with printf( "...%s...", ... ); statements
//! This is only for easy porting and has no advantage in time and space compared to printf|sprintf
//! But you can simply exchange 'printf(...)' with 'TRICE_P(...)' without concerning about static or dynamic strings
//! Needs internally a vsnprintf() implementation what adds a few KB code size
//! If the output strings in the legacy project are static, consider splitting the
//! legacy printf() into a TRICE* sequence to avoid enabling this switch.
//! Enabling TRICE_PRINTF_ADAPTER adds a few KB code for the library.
#define TRICE_PRINTF_ADAPTER 1 // options: 0, 1

#if 1 == TRICE_PRINTF_ADAPTER
#if TRICE_STRINGS == NONE_RUNTIME
#error needs TRICE_STRINGS set to RARE_RUNTIME or FULL_RUNTIME
#endif
#define TRICE_PRINTF_ADAPTER_BUFFERSIZE 100 //!< longest legacy printf should fit here
#endif // #if 1 == TRICE_PRINTF_ADAPTER

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

//#define TRICE_QUICK_AND_DIRTY_ONLY_PUTCHAR // for a quick start you can enable this line and rely only putchar

#ifdef TRICE_QUICK_AND_DIRTY_ONLY_PUTCHAR
TRICE_INLINE void tricePutchar( char c ){
    LL_USART_TransmitData8( TRICE_UART, c); // put your putchar call here
}

#define TRICE_ENTER_CRITICAL_SECTION {
#define TRICE_LEAVE_CRITICAL_SECTION }
TRICE_INLINE uint32_t triceTxDataRegisterEmpty( void ){ return 1; } // would be good to implement it for better performance, otherwise you need to use a time interval for the Tx calls
TRICE_INLINE void triceTransmitData8( uint8_t d ){ tricePutchar( d ); }
TRICE_INLINE void triceEableTxEmptyInterrupt( void ){}
TRICE_INLINE void triceDisableTxEmptyInterrupt( void ){}

#else // #ifdef TRICE_QUICK_AND_DIRTY_ONLY_PUTCHAR

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

//! Check if a new byte can be written into trice transmit register.
//! \retval 0 == not empty
//! \retval !0 == empty
//! User must provide this function.
TRICE_INLINE uint32_t triceTxDataRegisterEmpty( void ){
    return LL_USART_IsActiveFlag_TXE( TRICE_UART );
}

//! Write value d into trice transmit register.
//! \param d byte to transmit
//! User must provide this function.
TRICE_INLINE void triceTransmitData8( uint8_t d ){
    LL_USART_TransmitData8( TRICE_UART, d);
}

//! Allow interrupt for empty trice data transmit register.
//! User must provide this function.
TRICE_INLINE void triceEableTxEmptyInterrupt( void ){
    LL_USART_EnableIT_TXE( TRICE_UART );
}

//! Disallow interrupt for empty trice data transmit register.
//! User must provide this function.
TRICE_INLINE void triceDisableTxEmptyInterrupt( void ){
    LL_USART_DisableIT_TXE( TRICE_UART );
}

#endif // #else // #ifdef TRICE_QUICK_AND_DIRTY_ONLY_PUTCHAR

#define SYSTICKVAL16 SysTick->VAL //!< STM32 specific, set to 0 as starting point with nonSTM MCE

#ifdef __cplusplus
}
#endif

#endif /* TRICE_CONFIG_H_ */
