/*! \file triceConfig.h
\brief This file is for trice specific project settings
\details adapt needed fifo size, compiler settings and 4 hardware access functions
- triceTxDataRegisterEmpty()
- triceTransmitData8()
- triceEnableTxEmptyInterrupt()
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
#define TRICE_FIFO_SIZE 1024 //!< must be a power of 2, one trice needs 4 to 32 bytes, must hold trice bursts until they are transmitted, fifo is transmitted with lower priority
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
//#define RUNTIME_STRING_FIFO_SIZE 1000
#endif

//! Enable this for legacy projects with printf( "...%s...", ... ); statements
//! This is only for easy porting and has no advantage in time and space compared to printf|sprintf
//! But you can simply exchange 'printf(...)' with 'TRICE_P(...)' without concerning about static or dynamic strings
//! Needs internally a vsnprintf() implementation what adds a few KB code size
//! If the output strings in the legacy project are static, consider splitting the
//! legacy printf() into a TRICE* sequence to avoid enabling this switch.
//! Enabling TRICE_PRINTF_ADAPTER adds a few KB code for the library.
#define TRICE_PRINTF_ADAPTER 

#ifdef TRICE_PRINTF_ADAPTER
#if TRICE_STRINGS == NONE_RUNTIME
#error needs TRICE_STRINGS set to RARE_RUNTIME or FULL_RUNTIME
#endif
#define TRICE_PRINTF_ADAPTER_BUFFERSIZE 100 //!< longest legacy printf should fit here
#endif // #ifdef TRICE_PRINTF_ADAPTER

///////////////////////////////////////////////////////////////////////////////
// hardware specific stuff (enable only one of these header files)
//

#define STM32 132

#define ARCH STM32

#if ARCH == STM32
#include "main.h" // hardware specific stuff

#define SYSTICKVAL16 SysTick->VAL //!< STM32 specific, set to 0 as starting point with nonSTM MCE

#endif

#define WITH_INTERRUPTS
//#define NO_INTERRUPTS

#define USE_OWN_TRICE_WRITE_FUNCTION

#ifdef USE_OWN_TRICE_WRITE_FUNCTION
extern uint16_t writeCount;
extern uint16_t writeCountMax;
#define TRICE_WRITE_BUFFER_SIZE 400 //!< 
#define TRICE_WRITE_COUNT_LIMIT 100 //!< allowed filling for trice transfer
//#define TRICE_PAUSE triceWriteServer(); // do{ Pause(); } while(0) // put your own Pause here, if needed

//! replace with your ReadTickCount() function for more accurate timing
//#define CURRENT_TICK (tick + 1) 

//! Your tick unit multiplied with this count results in a trice message count transfer rate, 
//! for example 1 trice message per 10 ms if tick is 1ms and this value 10.
//| That means 8byte/10ms = 800 bytes per second. Your write channel must be able to transfer this in the average.
//! A 115200 baud serial port speed results in theroretical about 10000 bytes per second but in praxis
//! this value is lower because of processor interaction, for example 5000 bytes per second.
//! Than take in regard if your write channel is used by other functionality, for example 
//! when TRICE_STRINGS == FULL_RUNTIME is configured. You have to subtract the average of this too
//! to get the real value. The point is, that with TRICE bursts the write buffer gets not overloaded.
//#define TRICE_OUT_INTERVAL_TICKS 1

#endif //  #ifdef USE_OWN_TRICE_WRITE_FUNCTION

#include "triceCompilerConfig.h"
#include "triceStm32_LLConfig.h"

//#include "triceStm32PutCharConfig.h" // does not work now
//#include "triceStm32WriteConfig.h" // does not compile yet
//#include "triceArduinoConfig.h" // does not work yet

#ifndef TRICE_ENTER_CRITICAL_SECTION
#define TRICE_ENTER_CRITICAL_SECTION {
#define TRICE_LEAVE_CRITICAL_SECTION }
#endif



#ifdef __cplusplus
}
#endif

#endif /* TRICE_CONFIG_H_ */
