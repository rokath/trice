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

///////////////////////////////////////////////////////////////////////////////
// internal values (do not change)
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
// user adaption (not hardware specific)
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
// hardware specific stuff (enable only one of these header files)
//
#include "triceStm32InterruptConfig.h" // ok
//#include "triceStm32PutCharConfig.h" // does not work now
//#include "triceStm32WriteConfig.h" // does not compile yet
//#include "triceArduinoConfig.h" // does not work yet


#ifdef __cplusplus
}
#endif

#endif /* TRICE_CONFIG_H_ */
