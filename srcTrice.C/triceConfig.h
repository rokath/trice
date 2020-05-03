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
//! used as TRICE_CODE macro option for more flash occupation, but decreases execution time and needs smaller buffers
#define MORE_FLASH_AND_SPEED 30 //!< value is only to distinguish from LESS_FLASH and NO_CODE

//! used as TRICE_CODE macro option for less flash occupation, but increases execution time and needs bigger buffers
#define LESS_FLASH_AND_SPEED 20 //!< value is only to distinguish from MORE_FLASH and NO_CODE

//! used as TRICE_CODE macro option for no trice code generation
#define NO_CODE 0 //!< value is only to distinguish from MORE_FLASH or LESS_FLASH ans must be 0
//
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// user adaption (not hardware specific)
//
//!< a byte count for buffering traces, must be power of 2, one basic trace needs 4 bytes
#define TRICE_FIFO_SIZE 2048 //!< must be a power of 2, one trice needs 4 to 32 bytes, must hold trice bursts until they are transmitted, fifo is transmitted with lower priority
#define TRICE_START_BYTE (0xeb) //!< trice header start (chose any unusual byte)
#define TRICE_LOCAL_ADDR (0x60) //!< trice addess of this device (choose free)
#define TRICE_DISPL_ADDR (0x60) //!< trice terminal address for this device (choose free)

//! set kind of TRICE code generation globally
//! set compiler switch "optimize for time" accordingly!
//! to switch of TRICE code generation file specific add `#define TRICE_CODE NO_CODE`
//! at start of file before including trice.h
#define TRICE_CODE MORE_FLASH_AND_SPEED // options: NO_CODE, LESS_FLASH_AND_SPEED, MORE_FLASH_AND_SPEED 

//! setting for runtime generated string support code
//! when enabling this include triceRuntimeGeneratedStingsRare.c or triceRuntimeGeneratedStingsMany.c in project
#define TRICE_RUNTIME_GENERATED_STRINGS_SUPPORT 
//#define TRICE_RUNTIME_GENERATED_STRINGS_SUPPORT_EXPERIMENTAL 

//! enable encryption here, adds about 150 bytes code
//! call trice tool with log switch "-key your_password -show" and put passphrase here
//#define ENCRYPT XTEA_KEY( a9, 4a, 8f, e5, cc, b1, 9b, a6, 1c, 4c, 08, 73, d3, 91, e9, 87 ); //!< -key test

//! Enable this for legacy projects with printf( "...%s...", ... ); statements
//! This is only for easy porting and has no advantage in time and space compared to printf|sprintf
//! But you can simply exchange 'printf(...)' with 'TRICE_P(...)' without concerning about static or or runtime generated strings
//! Needs internally a vsnprintf() implementation what adds a few KB code size
//! If the output strings in the legacy project are static, consider splitting the
//! legacy printf() into a TRICE* sequence to avoid enabling this switch.
//! Enabling TRICE_PRINTF_ADAPTER adds a few KB code for the library.
//! needs tricePrintfAdapter.c in project
#define TRICE_PRINTF_ADAPTER 
#ifdef TRICE_PRINTF_ADAPTER
#define TRICE_PRINTF_ADAPTER_BUFFERSIZE 100 //!< longest legacy printf line should fit here
#endif // #ifdef TRICE_PRINTF_ADAPTER
//
///////////////////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////////////////////
// hardware specific stuff (enable only one of these header files)
//

#include "triceConfigCompiler.h"

#define STM32_LL 132
#define STM32_HAL 232

#define ARCH STM32_LL

#if ARCH == STM32_LL
#include "main.h" // hardware specific stuff
#define SYSTICKVAL16 SysTick->VAL //!< STM32 specific, set to 0 as starting point with nonSTM MCE
#define TRICE_SERVER_TICK ms
//#define LL_INTERFACE_NO_INTERRUPTS
#define LL_INTERFACE_WITH_INTERRUPTS
#include "triceConfigStm32_LL.h"
#endif

#if ARCH == STM32_HAL
#include "main.h" // hardware specific stuff
#define SYSTICKVAL16 SysTick->VAL //!< STM32 specific, set to 0 as starting point with nonSTM MCE
extern UART_HandleTypeDef huart2;
#define TRICE_UART_HANDLE_PTR (&huart2)
#define TRICE_SERVER_TICK HAL_GetTick()
//#define HAL_INTERFACE_BLOCKING_MODE
//#define HAL_INTERFACE_INTERRUPT_MODE
#define HAL_INTERFACE_DMA_MODE
#endif



extern uint16_t writeCount;
extern uint16_t writeCountMax;
#define TRICE_WRITE_BUFFER_SIZE 600 //!< 
#define TRICE_WRITE_COUNT_LIMIT 300 //!< allowed filling for trice transfer
//#define TRICE_PAUSE triceWriteServer(); // do{ Pause(); } while(0) // put your own Pause here, if needed

//#include "triceStm32PutCharConfig.h" // does not work now
//#include "triceStm32WriteConfig.h" // does not compile yet
//#include "triceArduinoConfig.h" // does not work yet

#ifdef __cplusplus
}
#endif

#endif /* TRICE_CONFIG_H_ */
