/*! \file triceConfig.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
// Enabling next line results in XTEA encryption  with the key.
//#define ENCRYPT XTEA_KEY( ea, bb, ec, 6f, 31, 80, 4e, b9, 68, e2, fa, ea, ae, f1, 50, 54 ); //!< -password MySecret
#ifdef ENCRYPT
// #define DECRYPT //!< usually not needed
void triceServeFifoEncryptedToBytesBuffer(void);
#endif

#define TRICE_STM32

//! Set endianess according to target hardware. Options: TRICE_BIG_ENDIANNESS, TRICE_LITTLE_ENDIANNESS.
//! Some compiler offer an automatic detection for this.
#define TRICE_HARDWARE_ENDIANNESS TRICE_LITTLE_ENDIANNESS 

//! Set byte order according desired transfer format. Options: TRICE_BIG_ENDIANNESS, TRICE_LITTLE_ENDIANNESS. 
//! TRICE_BIG_ENDIANNESS is network order.
//! If TRICE_TRANSFER_ENDIANNESS is equal to TRICE_HARDWARE_ENDIANNESS the trice code is smaller and more efficient.
//! When set to TRICE_LITTLE_ENDIANNESS the trice tool -encoding format specifier is extended by a letter 'L'.
//! Example -encoding "flexL".
#define TRICE_TRANSFER_ENDIANNESS TRICE_LITTLE_ENDIANNESS 

/*
#define TRICE_HEADLINE ...
- It is the very first TRICE message after reset. Customize as you like.

#define TRICE_CYCLE_COUNTER 0
- The TRICE macros are a bit faster.
- The TRICE transfer message is a byte shorter. 
- Lost TRICEs are not detected and the additional trice log switch "-cc=false" is needed.

#define TRICE_CYCLE_COUNTER 1
- The TRICE macros are a bit slower.
- The TRICE transfer message is a byte longer.
- Lost TRICEs are detected and no additional trice log switch is needed.

#define TRICE_ENTER
#define TRICE_LEAVE
- The TRICE macros are a bit faster.
- Inside interrupts TRICE macros forbidden.

#define TRICE_ENTER TRICE_ENTER_CRITICAL_SECTION
#define TRICE_LEAVE TRICE_LEAVE_CRITICAL_SECTION
- The TRICE macros are a bit slower.
- Inside interrupts TRICE macros allowed.

#define PUT(x) do{ *wTb++ = x; }while(0)
#define PUT_BUFFER( dynString, len ) do{ memcpy( wTb, dynString, len ); wTb += (len+3)>>2; }while(0)
- Double buffering is used for fastest TRICE macro execution.
- The read buffer gets the write buffer, when read buffer is empty.
- The read buffer must be read out completely before the write buffer can overflow.

#define TRICE_TRANSFER_MESSAGE TRICE_SINGLE_MESSAGE
- The TRICE data stream is COBS or COBS/R encoded and each packages contains exactly one TRICE message.
- This ok for most cases, but could add too much padding bytes when encryption is used.

#define TRICE_TRANSFER_MESSAGE TRICE_MULTI_MESSAGE
- The TRICE data stream is COBS or COBS/R encoded and each packages can contain more than one TRICE message.
- This ok for most cases, but could need less padding bytes when encryption is used.
- This allows the complete read out of one double buffer half within one TRICE_READ_AND_TRANSLATE_INTERVAL_MS.
- In turn the TRICE_FIFO_BYTE_SIZE must be big enough to hold a complete TRICE burst.
- But on the other hand the TRICE_BUFFER_SIZE can get smaller twice that size.
- That means de-facto triple buffering.

#define TRICE_READ_AND_TRANSLATE_INTERVAL_MS 10
- This is the milliseconds interval for TRICE buffer read out.
- This time should be shorter than visible delays.
- When TRICE_TRANSFER_MESSAGE is TRICE_SINGLE_MESSAGE, max one TRICE message is read in this interval.
  - When in the average more than one trice messages occur in this interval, the write buffer will overflow.

#define TRICE_BUFFER_SIZE 1200
- This is the size of both buffers together.
- One buffer must be able to hold the max TRICE burst count.
- Start with a big value and use the diagnostics value TriceDepthMax to minimize the RAM needs.

#define TRICE_FIFO_BYTE_SIZE 64 
- Must be a power of 2.
- 32 could be ok in dependence of the maximum used param count when TRICE_TRANSFER_MESSAGE is TRICE_SINGLE_MESSAGE.
- Must hold only one TRICE message when TRICE_TRANSFER_MESSAGE is TRICE_SINGLE_MESSAGE.
*/


//! Double Buffering, ...
#if 0
//! Use and adapt this as you like
#define TRICE_HEADLINE \
    TRICE0( Id( 46700), "s:                                          \ns:     " ); \
    TRICE0( Id( 56296), "att: COBS, double buf, ~cycle, ~int " ); \
    TRICE0( Id( 46377), "s:     \ns:                                          \n");
#define TRICE_CYCLE_COUNTER 0 //! add cycle counter
#define TRICE_ENTER 
#define TRICE_LEAVE
#define PUT(x) do{ *wTb++ = x; }while(0)
#define PUT_BUFFER( dynString, len ) do{ memcpy( wTb, dynString, len ); wTb += (len+3)>>2; }while(0)
#define TRICE_TRANSFER_MESSAGE TRICE_SINGLE_MESSAGE
#define TRICE_READ_AND_TRANSLATE_INTERVAL_MS 10
#define TRICE_BUFFER_SIZE 1200
#define TRICE_FIFO_BYTE_SIZE 64 //!< must be a power of 2, 32 could be ok in dependence of the maximum param count
#endif

//! Double Buffering, ...
#if 1
//! Use and adapt this as you like
#define TRICE_HEADLINE \
    TRICE0( Id( 46700), "s:                                          \ns:     " ); \
    TRICE0( Id( 36046), "att: COBS, double buf, +cycle, +int " ); \
    TRICE0( Id( 46377), "s:     \ns:                                          \n");
#define TRICE_CYCLE_COUNTER 1 //! add cycle counter
#define TRICE_ENTER TRICE_ENTER_CRITICAL_SECTION
#define TRICE_LEAVE TRICE_LEAVE_CRITICAL_SECTION
#define PUT(x) do{ *wTb++ = x; }while(0)
#define PUT_BUFFER( dynString, len ) do{ memcpy( wTb, dynString, len ); wTb += (len+3)>>2; }while(0)
#define TRICE_TRANSFER_MESSAGE TRICE_SINGLE_MESSAGE
#define TRICE_READ_AND_TRANSLATE_INTERVAL_MS 10
#define TRICE_BUFFER_SIZE 5000 //!< This is the size of both buffers together
#define TRICE_FIFO_BYTE_SIZE 256 //!< must be a power of 2, 32 could be ok in dependence of the maximum param count
#endif



//! Double Buffering, ...
#if 0
//! Use and adapt this as you like
#define TRICE_HEADLINE \
    TRICE0( Id( 46700), "s:                                          \ns:     " ); \
    TRICE0( Id( 36046), "att: COBS, double buf, +cycle, +int " ); \
    TRICE0( Id( 46377), "s:     \ns:                                          \n");
#define TRICE_CYCLE_COUNTER 1 //! add cycle counter
#define TRICE_ENTER TRICE_ENTER_CRITICAL_SECTION
#define TRICE_LEAVE TRICE_LEAVE_CRITICAL_SECTION
#define PUT(x) do{ *wTb++ = x; }while(0)
#define PUT_BUFFER( dynString, len ) do{ memcpy( wTb, dynString, len ); wTb += (len+3)>>2; }while(0)
#define TRICE_TRANSFER_MESSAGE TRICE_MULTI_MESSAGE
#define TRICE_READ_AND_TRANSLATE_INTERVAL_MS 10
#define TRICE_BUFFER_SIZE 2000 //!< This is the size of both buffers together
#define TRICE_FIFO_BYTE_SIZE 1024 //!< must be a power of 2, 32 could be ok in dependence of the maximum param count
#endif



// #define TRICE_RTT_CHANNEL 0 //!< Uncomment and set channel number for SeggerRTT usage


//  #ifdef TRICE_NO_CODE_GENERATION
//  #define TRICE_ENCODING TRICE_NOCODE_ENCODING //!< Select target trice transfer encoding.
//  #else
//  #define TRICE_ENCODING TRICE_COBSR_ENCODING //!< Select target trice transfer encoding.
//  #endif


//  #define TRICE_U8PUSH(v)  do{ /*triceU8PushSeggerRTT(v);  */ triceU8Push(v); } while(0) //!< Set trice out channel(s) 
//  #define TRICE_U32PUSH(v) do{ /*triceU32PushSeggerRTT(v); */ triceU32Push(v); } while(0) //!< Set trice out channel(s) 

//#define UART_LL_STM32 //!< set UART hardware dependency
//#define TRICE_UART USART2 //!< set UART number if UART is used

void triceCheckSet( int index ); //!< tests





#ifdef __cplusplus
}
#endif

#endif /* TRICE_CONFIG_H_ */

