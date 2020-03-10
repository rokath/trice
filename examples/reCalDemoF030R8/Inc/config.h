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
#define TRICE_FIFO_SIZE 1024
#define TRICE_START_BYTE (0xeb) //!< trice header start (chose any unusual byte)
#define TRICE_LOCAL_ADDR (0x60) //!< trice addess of this device (choose free)
#define TRICE_DISPL_ADDR (0x60) //!< trice terminal address for this device (choose free)
#define TRICE_SHORT_MEMORY 0 //!< 1 means less Flash needed but slower, set compiler switch "optimize for time" accordingly!
#ifndef TRICE_LEVEL
#define TRICE_LEVEL 100 //!< enable or disable trice code generation
#endif

//! enable encryption here
//! call trice tool with log switch "-key your_password -show" and put passphrase here
#//define ENCRYPT XTEA_KEY( a9, 4a, 8f, e5, cc, b1, 9b, a6, 1c, 4c, 08, 73, d3, 91, e9, 87 ); //!< -key test

//! Enable this for legacy projects with printf( "...%s...", ... ); statements
//! This is only for easy porting and has no advantage in time and space compared to printf
//! But you can simply exchange 'printf(...)' with 'trice(...)' without concerning about static or dynamic strings
//! Needs internally a vsnprintf() implementation what adds a few KB code size
//! If the output strings in the legacy project are static, consider splitting the
//! legacy printf() into a TRICE* sequence to avoid enabling this switch.
#define TRICE_PRINTF_ADAPTER
#define TRICE_PRINTF_ADAPTER_BUFFERSIZE 100 //!< longest legacy printf should fit here, only neede if TRICE_PRINTF_ADAPTER is defined

#define SYSTICKVAL16 SysTick->VAL //!< STM32 specific

#if 0


#include <stdlib.h>

///////////////////////////////////////////////////////////////////////////////
// REMOTE CALL
//#define RC_ADDR_OFFSET 0x60 //!< offest for easier debugging
//#define RC_ADDR(n) (RC_ADDR_OFFSET+(n)) //!< remote call address transformation
#define RC_LOCAL_ADDRESS (TRICE_LOCAL_ADDR) //!< remote call addess of this device
#define RCTX_FIFO_SIZE 0x600 //!< also tracelog messages are buffered here
#define RCRX_FIFO_SIZE 0x200 //!< mainly for responses
//
///////////////////////////////////////////////////////////////////////////////

#define LEFT 0

// ### DefaultMacros.h
//#define RESULT_NEXT_HEADER_IN_FIFO 0xC0
//#define RESULT_ANSWER   1 //!< macro for a received remote call answer
#define RESULT_OK       0 //!< macro for successful end of function
#define RESULT_ERROR   -1 //!< macro for unsuccessful end of function
//#define RESULT_NACK    -2 //!< macro for RBL ack answer NACK
//#define RESULT_EMPTY   -3 //!< macro for empty channel
#define RESULT_TIMEOUT -4 //!< see CONTROL_GEAR_ANSWER_TIMEOUT_ERROR
#define RESULT_LOCKED  -5 //!< no reentrance allowed
//
#define UNUSED_PARAMETER( x ) { x = x; }


#define DEFAULT_MS_TIMEOUT 1000

// ### ReadAndWrite.h

//extern uint32_t WriteTimeoutCount; //!< write error counter
//extern uint32_t ReadTimeoutCount; //!< read error counter
//
//
//
//#define IO_NEXT_DEFAULT_MS_TIMEOUT 30
//
//
int Read(int fd, uint8_t* pBuf, size_t count);
//int Read1(int fd, uint8_t* pBuf, size_t count); // tryout code!!!!
int Write(int fd, uint8_t const * pBuf, size_t count);
//



#define IO_NEXT_DEFAULT_MS_TIMEOUT 30

#include "Fifo.h"



static inline void Pause( void ){}


//#define REPORT_LINE(Value) do{ REPORT_FILE(); TRICE16_2( Id( 8986), " in line %d (0x%02x)\n", __LINE__, Value ); }while(0)
#define REPORT_FAILURE(Value) do{ REPORT_FILE(); TRICE32_2( Id(46005), "ERR: in line %d (0x%08x)\n", __LINE__, Value ); }while(0)
//#define REPORT_FAILURE16(a,b,c) do{ REPORT_FILE(); TRICE16_4( Id(17192), "ERR: in line %d (0x%04x,0x%04x,0x%04x)\n", __LINE__, a,b,c ); }while(0)
//#define REPORT_VALUE(Value)   do{ REPORT_FILE(); TRICE32_2( Id(11917),  "att: line %d, value = 0x%08x\r\n", __LINE__, Value ); }while(0)
//#define REPORT_ONLY_VALUE(Value)   do{  TRICE32_2( Id(33840),  "att: line %d, value = 0x%08x\r\n", __LINE__, Value ); }while(0)
#define ASSERTION do{ REPORT_FILE(); TRICE16_2( Id(16598), "err:local address 0x%02x:!ASSERT in line %d\n", RC_LOCAL_ADDRESS, __LINE__ ); }while(0)
//#define ASSERT( flag ) if(!(flag)) { ASSERTION; } //!< report if flag is not true
//
//
//
//
//#define ASSERT_OR_RETURN( flag )                if(!(flag)) { ASSERTION; return;              } //!< if flag is not true return result
#define ASSERT_OR_RETURN_RESULT( flag, result ) if(!(flag)) { ASSERTION; return result;       } //!< if flag is not true return result
#define ASSERT_OR_RETURN_RESULT_ERROR( flag )   if(!(flag)) { ASSERTION; return RESULT_ERROR; } //!< if flag is not true return result

#endif

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

#define ENTER_CRITICAL_SECTION TRICE_ENTER_CRITICAL_SECTION
#define LEAVE_CRITICAL_SECTION TRICE_LEAVE_CRITICAL_SECTION

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

//void Dbg_triceTransmitData8( uint8_t d );

/*! Allow interrupt for empty trice data transmit register.
User must provide this function.
*/
TRICE_INLINE void triceEableTxEmptyInterrupt( void ){
    LL_USART_EnableIT_TXE( USART2 );
}

/*! Disallow interrupt for empty trice data transmit register.
User must provide this function.
*/
TRICE_INLINE void triceDisableTxEmptyInterrupt( void ){
    LL_USART_DisableIT_TXE( USART2 );
}


enum{
    noTx, // no transmission
    triceTx, // trice packet in transmission
    reCalTx // remote call packet in transmission
};

#ifdef __cplusplus
}
#endif

#endif /* TRICE_CONFIG_H_ */
