/*! \file triceConfig.h
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#ifndef TRICE_CONFIG_H_
#define TRICE_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
// Enabling next line leads to XTEA encryption  with the key. Only wrapped barel over UART us encrypted right now.
//#define ENCRYPT XTEA_KEY( ea, bb, ec, 6f, 31, 80, 4e, b9, 68, e2, fa, ea, ae, f1, 50, 54 ); //!< -password MySecret

#define TRICE_HEADLINE \
TRICE0( Id(65422), "s:                                                   \ns:                     myProject                     \ns:                                                   \n\n")

///////////////////////////////////////////////////////////////////////////////
//
//#define TRICE_ENTER_CRITICAL_SECTION { // Uncomment for more speed but only if TRICE macros & fifo access cannot
//#define TRICE_LEAVE_CRITICAL_SECTION } // get interrupted by other TRICE macros or fifo access (e.g. interrupts).
//
///////////////////////////////////////////////////////////////////////////////

#define TRICE_FIFO_BYTE_SIZE 2048 //!< must be a power of 2

///////////////////////////////////////////////////////////////////////////////
//

//! Select target trice transfer encoding. 
#define TRICE_ENCODING TRICE_FLEX_ENCODING

//! Set endianess according to target hardware. Options: TRICE_BIG_ENDIANNESS, TRICE_LITTLE_ENDIANNESS.
//! Some compiler offer an automatic detection for this.
#define TRICE_HARDWARE_ENDIANNNESS TRICE_LITTLE_ENDIANNESS 

//! Set byte order according desired transfer format. Options: TRICE_BIG_ENDIANNESS, TRICE_LITTLE_ENDIANNESS. 
//! TRICE_BIG_ENDIANNESS is network order.
//! If TRICE_TRANSFER_ENDIANNESS is equal to TRICE_HARDWARE_ENDIANNNESS the trice code is smaller and more efficient.
//! When set to TRICE_LITTLE_ENDIANNESS the trice tool -encoding format specifier is extended by a letter 'L'.
//! Example -encoding "flexL".
#define TRICE_TRANSFER_ENDIANNESS TRICE_LITTLE_ENDIANNESS 

//#define TRICE_U8PUSH(v) do{ triceU8PushSeggerRTT(v); triceU8Push(v); } while(0) //!< Set trice out channel(s) 
#define TRICE_U32PUSH(v) do{ /* triceU32PushSeggerRTT(v);*/ triceU32Push(v); } while(0) //!< Set trice out channel(s) 

//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// uncomment for trice wrap transfer format
// #define TRICE_WRAP_START_BYTE 0xEB // 235, to do: also as trice tool parameter
// #define TRICE_WRAP_LOCAL_ADDR 0x80 // to do: also as trice tool parameter
// #define TRICE_WRAP_DEST_ADDR  0x81 // to do: also as trice tool parameter
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// uncomment for trice UART transfer format
#define TRICE_UART USART2 // set UART number
///////////////////////////////////////////////////////////////////////////////

void triceCheckSetTime( int index ); //!< tests
void triceCheckSetSpace( int index ); //!< tests

// These macros can help to reduce code size if the project contains many TRICE macros.
// If a macro is commented out here, inside trice.c an appropriate function is enabled containing the TRICE macro.
// Using trice instead of TRICE results in only one function call instead of inlining the TRICE macro.
//  #define trice0i( i, pFmt )   
//  #define trice8_1i( i, pFmt, d0 )                             //! comment out to generate code for it
//  #define trice8_2i( i, pFmt, d0, d1 )                         //! comment out to generate code for it
//  #define trice8_3i( i, pFmt, d0, d1, d2 )                     //! comment out to generate code for it
//  #define trice8_4i( i, pFmt, d0, d1, d2, d3 )                 //! comment out to generate code for it
//  #define trice8_5i( i, pFmt, d0, d1, d2, d3, d4 )             //! comment out to generate code for it
//  #define trice8_6i( i, pFmt, d0, d1, d2, d3, d4, d5 )         //! comment out to generate code for it
//  #define trice8_7i( i, pFmt, d0, d1, d2, d3, d4, d5, d6 )     //! comment out to generate code for it
//  #define trice8_8i( i, pFmt, d0, d1, d2, d3, d4, d5, d6, d7 ) //! comment out to generate code for it
//  #define trice8_1( i, pFmt, d0 )                              //! comment out to generate code for it
//  #define trice8_2( i, pFmt, d0, d1 )                          //! comment out to generate code for it
//  #define trice8_3( i, pFmt, d0, d1, d2 )                      //! comment out to generate code for it
//  #define trice8_4( i, pFmt, d0, d1, d2, d3 )                  //! comment out to generate code for it
//  #define trice8_5( i, pFmt, d0, d1, d2, d3, d4 )              //! comment out to generate code for it
//  #define trice8_6( i, pFmt, d0, d1, d2, d3, d4, d5 )          //! comment out to generate code for it
//  #define trice8_7( i, pFmt, d0, d1, d2, d3, d4, d5, d6 )      //! comment out to generate code for it
//  #define trice8_8( i, pFmt, d0, d1, d2, d3, d4, d5, d6, d7 )  //! comment out to generate code for it
//  #define trice16_1i( i, pFmt, d0 )                            //! comment out to generate code for it
//  #define trice16_2i( i, pFmt, d0, d1 )                        //! comment out to generate code for it
//  #define trice16_3i( i, pFmt, d0, d1, d2 )                    //! comment out to generate code for it
//  #define trice16_4i( i, pFmt, d0, d1, d2, d3 )                //! comment out to generate code for it
//  #define trice32_1i( i, pFmt, d0 )                            //! comment out to generate code for it
//  #define trice32_2i( i, pFmt, d0, d1 )                        //! comment out to generate code for it
//  #define trice32_3i( i, pFmt, d0, d1, d2 )                    //! comment out to generate code for it
//  #define trice32_4i( i, pFmt, d0, d1, d2, d3 )                //! comment out to generate code for it
//  #define trice16_1( i, pFmt, d0 )                             //! comment out to generate code for it
//  #define trice16_2( i, pFmt, d0, d1 )                         //! comment out to generate code for it
//  #define trice16_3( i, pFmt, d0, d1, d2 )                     //! comment out to generate code for it
//  #define trice16_4( i, pFmt, d0, d1, d2, d3 )                 //! comment out to generate code for it
//  #define trice32_1( i, pFmt, d0 )                             //! comment out to generate code for it
//  #define trice32_2( i, pFmt, d0, d1 )                         //! comment out to generate code for it
//  #define trice32_3( i, pFmt, d0, d1, d2 )                     //! comment out to generate code for it
//  #define trice32_4( i, pFmt, d0, d1, d2, d3 )                 //! comment out to generate code for it
//  #define trice64_1i( i, pFmt, d0 )                            //! comment out to generate code for it
//  #define trice64_2i( i, pFmt, d0, d1 )                        //! comment out to generate code for it
//  #define trice64_1( i, pFmt, d0 )                             //! comment out to generate code for it
//  #define trice64_2( i, pFmt, d0, d1 )                         //! comment out to generate code for it

#ifdef __cplusplus
}
#endif

#endif /* TRICE_CONFIG_H_ */
