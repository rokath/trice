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

#define STM32_LL 132
#define STM32_HAL 232
#define STM32_HAL_G4 234
#define SEGGER_RTT 333
#define SEGGER_RTTB 334
#define SEGGER_RTTD 335
#define SEGGER_RTTD_ASM 336

//
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
// user adaption (not hardware specific)
//
//!< a byte count for buffering traces, must be power of 2, one basic trace needs 4 bytes
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


#ifndef TRICE_VARIANT
#error specify TRICE_VARIANT in project settings or in a copy of this file
#endif

#ifndef TRICE_INSIDE_INTERRUPTS
#define TRICE_INSIDE_INTERRUPTS 0
#endif

#if TRICE_VARIANT == STM32_LL
#define TRICE_PUSH(v) triceFifoPush(v)
#define TRICE_FIFO_SIZE 256 //!< must be a power of 2, one trice needs 4 to 32 bytes, must hold trice bursts until they are transmitted, fifo is transmitted with lower priority
#define TRICE_SERVER_TICK ms
//#define LL_INTERFACE_NO_INTERRUPTS // a trice needs about 4 clocks less for no handling CRITICAL-SECTION
#define LL_INTERFACE_WITH_INTERRUPTS // a trice needs about 4 clocks longer for handling CRITICAL-SECTION
#include "triceConfigTx.h"

#elif TRICE_VARIANT == STM32_HAL
#define TRICE_PUSH(v) triceFifoPush(v) // tricePush( v )
#define TRICE_FIFO_SIZE 256 //!< must be a power of 2, one trice needs 4 to 32 bytes, must hold trice bursts until they are transmitted, fifo is transmitted with lower priority
#include "main.h" // hardware specific stuff
extern UART_HandleTypeDef huart2;
#define TRICE_UART_HANDLE_PTR (&huart2)
#define TRICE_SERVER_TICK HAL_GetTick()
//#define HAL_INTERFACE_BLOCKING_MODE
//#define HAL_INTERFACE_INTERRUPT_MODE
#define HAL_INTERFACE_DMA_MODE

#elif TRICE_VARIANT == STM32_HAL_G4
#define TRICE_PUSH(v) triceFifoPush(v) // tricePush( v )
#define TRICE_FIFO_SIZE 256 //!< must be a power of 2, one trice needs 4 to 32 bytes, must hold trice bursts until they are transmitted, fifo is transmitted with lower priority
#include "main.h" // hardware specific stuff
extern UART_HandleTypeDef hlpuart1;
#define TRICE_UART_HANDLE_PTR (&hlpuart1)
#define TRICE_SERVER_TICK HAL_GetTick()
//#define HAL_INTERFACE_BLOCKING_MODE
//#define HAL_INTERFACE_INTERRUPT_MODE
#define HAL_INTERFACE_DMA_MODE

#elif TRICE_VARIANT == SEGGER_RTTB
#include "SEGGER_RTT.h"
#define TRICE_PUSH(v) triceFifoPush(v)
#define TRICE_FIFO_SIZE 1024 //!< must be a power of 2, one trice needs 4 to 32 bytes, must hold trice bursts until they are transmitted, fifo is transmitted with lower priority
#define LL_INTERFACE_NO_INTERRUPTS

#elif TRICE_VARIANT == SEGGER_RTT
#include "SEGGER_RTT.h"
#define TRICE_PUSH(v) tricePush( v )
#define TRICE_FIFO_SIZE 0 //!< must be a power of 2, one trice needs 4 to 32 bytes, must hold trice bursts until they are transmitted, fifo is transmitted with lower priority
#define LL_INTERFACE_NO_INTERRUPTS

#elif TRICE_VARIANT == SEGGER_RTTD
#include "SEGGER_RTT.h"
#define TRICE_FIFO_SIZE 0 //!< must be a power of 2, one trice needs 4 to 32 bytes, must hold trice bursts until they are transmitted, fifo is transmitted with lower priority
#define TRICE_PUSH(v) do{ uint32_t y = v; SEGGER_RTT_Write( 0, &y, sizeof(uint32_t) ); }while(0)

#elif TRICE_VARIANT == SEGGER_RTTD_ASM
#include "SEGGER_RTT.h"
#define TRICE_FIFO_SIZE 0 //!< must be a power of 2, one trice needs 4 to 32 bytes, must hold trice bursts until they are transmitted, fifo is transmitted with lower priority
//#define RTT_USE_ASM (0)
#define TRICE_PUSH(v) do{ uint32_t y = v; SEGGER_RTT_Write( 0, &y, sizeof(uint32_t) ); }while(0)
//#define TRICE_PUSH(v) do{ uint32_t y = v; SEGGER_RTT_ASM_WriteSkipNoLock( 0, &y, sizeof(uint32_t) ); }while(0)
#endif

#include "triceConfigTxInterrupt.h"
#include "triceConfigCriticalSection.h"

//extern uint16_t writeCount;
extern uint16_t writeCountMax;
#ifndef TRICE_WRITE_BUFFER_SIZE
#define TRICE_WRITE_BUFFER_SIZE 600 //!< only used inside triceWrite.c
#endif
#define TRICE_WRITE_COUNT_LIMIT (TRICE_WRITE_BUFFER_SIZE * 2 / 3 //!< allowed filling for trice transfer
#define TRICE_WRITE_SPACE_MIN 8 //!<
//#define TRICE_PAUSE triceWriteServer(); // do{ Pause(); } while(0) // put your own Pause here, if needed

//#include "triceStm32PutCharConfig.h" // does not work now
//#include "triceStm32WriteConfig.h" // does not compile yet
//#include "triceArduinoConfig.h" // does not work yet

#ifdef __cplusplus
}
#endif

#endif /* TRICE_CONFIG_H_ */
