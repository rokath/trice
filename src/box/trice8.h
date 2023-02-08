/*! \file internalTrice8.h
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/

#define TRICE8(  tid, fmt, ...) CONCAT2(TRICE8_,  COUNT_ARGUMENTS(__VA_ARGS__))(tid, fmt, ##__VA_ARGS__)
#define trice8(  tid, fmt, ...) CONCAT2(trice8_,  COUNT_ARGUMENTS(__VA_ARGS__))(tid, fmt, ##__VA_ARGS__)
#define Trice8(  tid, fmt, ...) CONCAT2(Trice8_,  COUNT_ARGUMENTS(__VA_ARGS__))(tid, fmt, ##__VA_ARGS__)
#define TRice8(  tid, fmt, ...) CONCAT2(TRice8_,  COUNT_ARGUMENTS(__VA_ARGS__))(tid, fmt, ##__VA_ARGS__)
//#define trice8M( tid, fmt, ...) CONCAT2(trice8M_, COUNT_ARGUMENTS(__VA_ARGS__))(tid, fmt, ##__VA_ARGS__)
//#define Trice8M( tid, fmt, ...) CONCAT2(trice8M_, COUNT_ARGUMENTS(__VA_ARGS__))(tid, fmt, ##__VA_ARGS__)
//#define TRice8M( tid, fmt, ...) CONCAT2(trice8M_, COUNT_ARGUMENTS(__VA_ARGS__))(tid, fmt, ##__VA_ARGS__)

//!TRICE8_B expects inside pFmt only one format specifier, which is used n times by using pFmt n times.
//! It is usable for showing n 8-bit values.
#define TRICE8_B( id, pFmt, buf, n) do { TRICE_N( id, pFmt, buf, n); } while(0)

//!TRICE8_F expects inside pFmt just a string which is assumed to be a remote function name.
//! The trice tool displays the pFmt string followed by n times (value i).
//! The idea behind is to generate an id - function pointer referece list from the generated til.json file to
//! compile it into a remote device and execute the inside pFmt named function remotely.
//! Look for "TRICE8_F example" inside triceCheck.c.
#define TRICE8_F  TRICE8_B

#define TRICE_PUT8_1(  v0 )                                                 TRICE_PUT(                                                   TRICE_BYTE0(v0));

#define TRICE_PUT8_2(  v0, v1                                           )   TRICE_PUT(                                   TRICE_BYTE1(v1) |TRICE_BYTE0(v0));
                                                                            
#define TRICE_PUT8_3(  v0, v1, v2                                       )   TRICE_PUT(                  TRICE_BYTE2(v2) |TRICE_BYTE1(v1) |TRICE_BYTE0(v0));
                                                                            
#define TRICE_PUT8_4(  v0, v1, v2, v3                                   )   TRICE_PUT( TRICE_BYTE3(v3) |TRICE_BYTE2(v2) |TRICE_BYTE1(v1) |TRICE_BYTE0(v0));
                                                                            
#define TRICE_PUT8_5(  v0, v1, v2, v3, v4                               )   TRICE_PUT( TRICE_BYTE3(v3) |TRICE_BYTE2(v2) |TRICE_BYTE1(v1) |TRICE_BYTE0(v0)); \
                                                                            TRICE_PUT(                                                    TRICE_BYTE0(v4)); 
                                                                            
#define TRICE_PUT8_6(  v0, v1, v2, v3, v4, v5                           )   TRICE_PUT( TRICE_BYTE3(v3) |TRICE_BYTE2(v2) |TRICE_BYTE1(v1) |TRICE_BYTE0(v0)); \
                                                                            TRICE_PUT(                                   TRICE_BYTE1(v5) |TRICE_BYTE0(v4)); 
                                                                            
#define TRICE_PUT8_7(  v0, v1, v2, v3, v4, v5, v6                       )   TRICE_PUT( TRICE_BYTE3(v3) |TRICE_BYTE2(v2) |TRICE_BYTE1(v1) |TRICE_BYTE0(v0)); \
                                                                            TRICE_PUT(                  TRICE_BYTE2(v6) |TRICE_BYTE1(v5) |TRICE_BYTE0(v4));
                                                                            
#define TRICE_PUT8_8(  v0, v1, v2, v3, v4, v5, v6, v7                   )   TRICE_PUT( TRICE_BYTE3(v3) |TRICE_BYTE2(v2) |TRICE_BYTE1(v1) |TRICE_BYTE0(v0)); \
                                                                            TRICE_PUT( TRICE_BYTE3(v7) |TRICE_BYTE2(v6) |TRICE_BYTE1(v5) |TRICE_BYTE0(v4));
                                                                            
#define TRICE_PUT8_9(  v0, v1, v2, v3, v4, v5, v6, v7, v8               )   TRICE_PUT( TRICE_BYTE3(v3) |TRICE_BYTE2(v2) |TRICE_BYTE1(v1) |TRICE_BYTE0(v0)); \
                                                                            TRICE_PUT( TRICE_BYTE3(v7) |TRICE_BYTE2(v6) |TRICE_BYTE1(v5) |TRICE_BYTE0(v4)); \
                                                                            TRICE_PUT(                                                    TRICE_BYTE0(v8));
                                                                            
#define TRICE_PUT8_10( v0, v1, v2, v3, v4, v5, v6, v7, v8, v9           )   TRICE_PUT( TRICE_BYTE3(v3) |TRICE_BYTE2(v2) |TRICE_BYTE1(v1) |TRICE_BYTE0(v0)); \
                                                                            TRICE_PUT( TRICE_BYTE3(v7) |TRICE_BYTE2(v6) |TRICE_BYTE1(v5) |TRICE_BYTE0(v4)); \
                                                                            TRICE_PUT(                                   TRICE_BYTE1(v9) |TRICE_BYTE0(v8));
                                                                            
#define TRICE_PUT8_11( v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10      )   TRICE_PUT( TRICE_BYTE3(v3) |TRICE_BYTE2(v2) |TRICE_BYTE1(v1) |TRICE_BYTE0(v0)); \
                                                                            TRICE_PUT( TRICE_BYTE3(v7) |TRICE_BYTE2(v6) |TRICE_BYTE1(v5) |TRICE_BYTE0(v4)); \
                                                                            TRICE_PUT(                  TRICE_BYTE2(v10)|TRICE_BYTE1(v9) |TRICE_BYTE0(v8));
                                                                            
#define TRICE_PUT8_12( v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 )   TRICE_PUT( TRICE_BYTE3(v3) |TRICE_BYTE2(v2) |TRICE_BYTE1(v1) |TRICE_BYTE0(v0)); \
                                                                            TRICE_PUT( TRICE_BYTE3(v7) |TRICE_BYTE2(v6) |TRICE_BYTE1(v5) |TRICE_BYTE0(v4)); \
                                                                            TRICE_PUT( TRICE_BYTE3(v11)|TRICE_BYTE2(v10)|TRICE_BYTE1(v9) |TRICE_BYTE0(v8));

//! TRICE8_1 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 a 8 bit bit value
#define TRICE8_1( tid, pFmt, v0 ) \
    TRICE_ENTER tid; CNTC(1); \
    TRICE_PUT8_1( v0 )  \
    TRICE_LEAVE

//! TRICE8_2 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v1 are 8 bit bit values
#define TRICE8_2( id, pFmt, v0, v1 ) \
    TRICE_ENTER id; CNTC(2); \
    TRICE_PUT8_2 ( v0, v1 ); \
    TRICE_LEAVE

//! TRICE8_3 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v2 are 8 bit bit values
#define TRICE8_3( id, pFmt, v0, v1, v2 ) \
    TRICE_ENTER id; CNTC(3); \
    TRICE_PUT8_3( v0, v1, v2 ); \
    TRICE_LEAVE

//! TRICE8_4 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v3 are 8 bit bit values
#define TRICE8_4( id, pFmt, v0, v1, v2, v3 ) \
    TRICE_ENTER id; CNTC(4); \
    TRICE_PUT8_4( v0, v1, v2, v3  ); \
    TRICE_LEAVE

//! TRICE8_5 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v4 are 8 bit bit values
#define TRICE8_5( id, pFmt, v0, v1, v2, v3, v4 ) \
    TRICE_ENTER id; CNTC(5); \
    TRICE_PUT8_5( v0, v1, v2, v3, v4  ); \
    TRICE_LEAVE

//! TRICE8_6 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v5 are 8 bit bit values
#define TRICE8_6( id, pFmt, v0, v1, v2, v3, v4, v5 ) \
    TRICE_ENTER id; CNTC(6); \
    TRICE_PUT8_6( v0, v1, v2, v3, v4, v5 ); \
    TRICE_LEAVE

//! TRICE8_8 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v6 are 8 bit bit values
#define TRICE8_7( id, pFmt, v0, v1, v2, v3, v4, v5, v6 ) \
    TRICE_ENTER id; CNTC(7); \
    TRICE_PUT8_7( v0, v1, v2, v3, v4, v5, v6 ); \
    TRICE_LEAVE

//! TRICE8_8 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v7 are 8 bit bit values
#define TRICE8_8( id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7 ) \
    TRICE_ENTER id; CNTC(8); \
    TRICE_PUT8_8( v0, v1, v2, v3, v4, v5, v6, v7 ); \
    TRICE_LEAVE

//! TRICE8_8 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v7 are 8 bit bit values
#define TRICE8_9( id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8 ) \
    TRICE_ENTER id; CNTC(9); \
    TRICE_PUT8_9( v0, v1, v2, v3, v4, v5, v6, v7, v8 ); \
    TRICE_LEAVE

//! TRICE8_8 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v7 are 8 bit bit values
#define TRICE8_10( id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9 ) \
    TRICE_ENTER id; CNTC(10); \
    TRICE_PUT8_10( v0, v1, v2, v3, v4, v5, v6, v7, v8, v9 ); \
    TRICE_LEAVE

//! TRICE8_8 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v7 are 8 bit bit values
#define TRICE8_11( id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 ) \
    TRICE_ENTER id; CNTC(11); \
    TRICE_PUT8_11( v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 ); \
    TRICE_LEAVE

//! TRICE8_12 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v11 are 8 bit bit values
#define TRICE8_12( id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 ) \
    TRICE_ENTER id; CNTC(12); \
    TRICE_PUT8_12( v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 ) \
    TRICE_LEAVE

//  //! trice8_1 is an empty macro. It is used as a place holder, which is replaced for compilation in this way:
//  //! - user written code: `trice8_1( iD( 5732),         "msg:value=%d\n", -1 );`
//  //! - modified code:     `trice8M_1( 7009, "msg:value=%d\n", -1 );`
//  #define trice8_1( fmt, v0 )
//  #define trice8_2( fmt, v0, v1 ) //!< trice8_2 is an empty macro
//  #define trice8_3( fmt, v0, v1, v2 ) //!< trice8_3 is an empty macro
//  #define trice8_4( fmt, v0, v1, v2, v3 ) //!< trice8_4 is an empty macro
//  #define trice8_5( fmt, v0, v1, v2, v3, v4 ) //!< trice8_5 is an empty macro
//  #define trice8_6( fmt, v0, v1, v2, v3, v4, v5 ) //!< trice8_6 is an empty macro
//  #define trice8_7( fmt, v0, v1, v2, v3, v4, v5, v6 ) //!< trice8_7 is an empty macro
//  #define trice8_8( fmt, v0, v1, v2, v3, v4, v5, v6, v7 ) //!< trice8_8 is an empty macro
//  #define trice8_9( fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8 ) //!< trice8_9 is an empty macro
//  #define trice8_10( fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9 ) //!< trice8_10 is an empty macro
//  #define trice8_11( fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 ) //!< trice8_11 is an empty macro
//  #define trice8_12( fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 ) //!< trice8_12 is an empty macro

//! trice8m_1 writes trice data as fast as possible in a buffer.
//! This macro is used internally and not intended for user applications.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 a 8 bit bit value
#define trice8m_1( tid, v0 ) \
    TRICE_ENTER \
    TRICE_PUT( (1<<24) | ((TRICE_CYCLE)<<16) | (0x4000|(tid)) ); \
    TRICE_PUT8_1( v0 ) \
    TRICE_LEAVE

#define trice8m_2( tid, v0, v1 ) \
    TRICE_ENTER \
    TRICE_PUT( (2<<24) | ((TRICE_CYCLE)<<16) | (0x4000|(tid)) ); \
    TRICE_PUT8_2( v0, v1); \
    TRICE_LEAVE

#define trice8m_3( tid, v0, v1, v2 ) \
    TRICE_ENTER \
    TRICE_PUT( (3<<24) | ((TRICE_CYCLE)<<16) | (0x4000|(tid)) ); \
    TRICE_PUT8_3 ( v0, v1, v2 ); \
    TRICE_LEAVE

#define trice8m_4( tid, v0, v1, v2, v3 ) \
    TRICE_ENTER \
    TRICE_PUT( (4<<24) | ((TRICE_CYCLE)<<16) | (0x4000|(tid)) ); \
    TRICE_PUT8_4( v0, v1, v2, v3 ); \
    TRICE_LEAVE

#define trice8m_5( tid, v0, v1, v2, v3, v4 ) \
    TRICE_ENTER \
    TRICE_PUT( (5<<24) | ((TRICE_CYCLE)<<16) | (0x4000|(tid)) ); \
    TRICE_PUT8_5( v0, v1, v2, v3, v4 ); \
    TRICE_LEAVE

#define trice8m_6( tid, v0, v1, v2, v3, v4, v5 ) \
    TRICE_ENTER \
    TRICE_PUT( (6<<24) | ((TRICE_CYCLE)<<16) | (0x4000|(tid)) ); \
    TRICE_PUT8_6( v0, v1, v2, v3, v4, v5 ); \
    TRICE_LEAVE

#define trice8m_7( tid, v0, v1, v2, v3, v4, v5, v6 ) \
    TRICE_ENTER \
    TRICE_PUT( (7<<24) | ((TRICE_CYCLE)<<16) | (0x4000|(tid)) ); \
    TRICE_PUT8_7( v0, v1, v2, v3, v4, v5, v6 ); \
    TRICE_LEAVE

#define trice8m_8( tid, v0, v1, v2, v3, v4, v5, v6, v7 ) \
    TRICE_ENTER \
    TRICE_PUT( (8<<24) | ((TRICE_CYCLE)<<16) | (0x4000|(tid)) ); \
    TRICE_PUT8_8( v0, v1, v2, v3, v4, v5, v6, v7 ); \
    TRICE_LEAVE

#define trice8m_9( tid, v0, v1, v2, v3, v4, v5, v6, v7, v8 ) \
    TRICE_ENTER \
    TRICE_PUT( (9<<24) | ((TRICE_CYCLE)<<16) | (0x4000|(tid)) ); \
    TRICE_PUT8_9( v0, v1, v2, v3, v4, v5, v6, v7, v8 ); \
    TRICE_LEAVE

#define trice8m_10( tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9 ) \
    TRICE_ENTER \
    TRICE_PUT( (10<<24) | ((TRICE_CYCLE)<<16) | (0x4000|(tid)) ); \
    TRICE_PUT8_10( v0, v1, v2, v3, v4, v5, v6, v7, v8, v9 ); \
    TRICE_LEAVE

#define trice8m_11( tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 ) \
    TRICE_ENTER \
    TRICE_PUT( (11<<24) | ((TRICE_CYCLE)<<16) | (0x4000|(tid)) ); \
    TRICE_PUT8_11( v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 ); \
    TRICE_LEAVE

#define trice8m_12( tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 ) \
    TRICE_ENTER \
    TRICE_PUT( (12<<24) | ((TRICE_CYCLE)<<16) | (0x4000|(tid)) ); \
    TRICE_PUT8_12( v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 ) \
    TRICE_LEAVE

#define trice8_1( tid,  fmt, v0 ) trice8fn_1( tid,  (uint8_t)(v0) ) //!< trice8M_1 is a macro calling a function to reduce code size, this way avoiding code inlining.
#define trice8_2( tid,  fmt, v0, v1 ) trice8fn_2( tid,  (uint8_t)(v0), (uint8_t)(v1) ) //!< trice8M_2 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define trice8_3( tid,  fmt, v0, v1, v2 ) trice8fn_3( tid,  (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2) ) //!< trice8M_3 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define trice8_4( tid,  fmt, v0, v1, v2, v3 ) trice8fn_4( tid,  (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3) ) //!< trice8M_4 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define trice8_5( tid,  fmt, v0, v1, v2, v3, v4 ) trice8fn_5( tid,  (uint8_t)v0, (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3), (uint8_t)(v4) ) //!< trice8M_5 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define trice8_6( tid,  fmt, v0, v1, v2, v3, v4, v5 ) trice8fn_6( tid,  (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3), (uint8_t)(v4), (uint8_t)(v5) ) //!< trice8M_6 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define trice8_7( tid,  fmt, v0, v1, v2, v3, v4, v5, v6 ) trice8fn_7( tid,  (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3), (uint8_t)(v4), (uint8_t)(v5), (uint8_t)(v6) ) //!< trice8M_7 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define trice8_8( tid,  fmt, v0, v1, v2, v3, v4, v5, v6, v7 ) trice8fn_8( tid,  (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3), (uint8_t)(v4), (uint8_t)(v5), (uint8_t)(v6), (uint8_t)(v7) ) //!< trice8M_8 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define trice8_9( tid,  fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8 ) trice8fn_9( tid, (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3), (uint8_t)(v4), (uint8_t)(v5), (uint8_t)(v6), (uint8_t)(v7), (uint8_t)(v8) ) //!< trice8M_9 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define trice8_10( tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9 ) trice8fn_10( tid, (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3), (uint8_t)(v4), (uint8_t)(v5), (uint8_t)(v6), (uint8_t)(v7), (uint8_t)(v8), (uint8_t)(v9) ) //!< trice8M_10 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define trice8_11( tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 ) trice8fn_11( tid, (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3), (uint8_t)(v4), (uint8_t)(v5), (uint8_t)(v6), (uint8_t)(v7), (uint8_t)(v8), (uint8_t)(v9), (uint8_t)(v10) ) //!< trice8M_1M ia macro calling a function to reduce code size, this way avoiding code inlining.
#define trice8_12( tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 ) trice8fn_12( tid, (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3), (uint8_t)(v4), (uint8_t)(v5), (uint8_t)(v6), (uint8_t)(v7), (uint8_t)(v8), (uint8_t)(v9), (uint8_t)(v10), (uint8_t)(v11) ) //!< trice8M_12 ia macro calling a function to reduce code size, this way avoiding code inlining.

void trice8fn_1( uint16_t tid,  uint8_t v0 );
void trice8fn_2( uint16_t tid,  uint8_t v0, uint8_t v1 );
void trice8fn_3( uint16_t tid,  uint8_t v0, uint8_t v1, uint8_t v2 );
void trice8fn_4( uint16_t tid,  uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3 );
void trice8fn_5( uint16_t tid,  uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4 );
void trice8fn_6( uint16_t tid,  uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5 );
void trice8fn_7( uint16_t tid,  uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6 );
void trice8fn_8( uint16_t tid,  uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6, uint8_t v7 );
void trice8fn_9( uint16_t tid,  uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6, uint8_t v7, uint8_t v8 );
void trice8fn_10( uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6, uint8_t v7, uint8_t v8, uint8_t v9 );
void trice8fn_11( uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6, uint8_t v7, uint8_t v8, uint8_t v9, uint8_t v10 );
void trice8fn_12( uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6, uint8_t v7, uint8_t v8, uint8_t v9, uint8_t v10, uint8_t v11 );

//  #define Trice8_1( fmt, v0 ) //!< Trice8_1 is an empty macro
//  #define Trice8_2( fmt, v0, v1 ) //!< Trice8_2 is an empty macro
//  #define Trice8_3( fmt, v0, v1, v2 ) //!< Trice8_3 is an empty macro
//  #define Trice8_4( fmt, v0, v1, v2, v3 ) //!< Trice8_4 is an empty macro
//  #define Trice8_5( fmt, v0, v1, v2, v3, v4 ) //!< Trice8_5 is an empty macro
//  #define Trice8_6( fmt, v0, v1, v2, v3, v4, v5 ) //!< Trice8_6 is an empty macro
//  #define Trice8_7( fmt, v0, v1, v2, v3, v4, v5, v6 ) //!< Trice8_7 is an empty macro
//  #define Trice8_8( fmt, v0, v1, v2, v3, v4, v5, v6, v7 ) //!< Trice8_8 is an empty macro
//  #define Trice8_9( fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8 ) //!< Trice8_9 is an empty macro
//  #define Trice8_10( fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9 ) //!< Trice8_10 is an empty macro
//  #define Trice8_11( fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 ) //!< Trice8_11 is an empty macro
//  #define Trice8_12( fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 ) //!< Trice8_12 is an empty macro

//! Trice8m_1 writes trice data as fast as possible in a buffer.
//! This macro is used internally and not intended for user applications.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 a 8 bit bit value
#define Trice8m_1( tid, v0 ) \
    TRICE_ENTER \
    uint16_t ts = TriceStamp16(); \
    TRICE_PUT(0x80008000|(tid<<16)|tid); \
    TRICE_PUT( 1<<24 | (TRICE_CYCLE<<16) | ts ); \
    TRICE_PUT8_1( v0 ) \
    TRICE_LEAVE

#define Trice8m_2( tid, v0, v1 ) \
    TRICE_ENTER \
    uint16_t ts = TriceStamp16(); \
    TRICE_PUT(0x80008000|(tid<<16)|tid); \
    TRICE_PUT( 2<<24 | (TRICE_CYCLE<<16) | ts ); \
    TRICE_PUT8_2( v0, v1); \
    TRICE_LEAVE

#define Trice8m_3( tid, v0, v1, v2 ) \
    TRICE_ENTER \
    uint16_t ts = TriceStamp16(); \
    TRICE_PUT(0x80008000|(tid<<16)|tid); \
    TRICE_PUT( 3<<24 | (TRICE_CYCLE<<16) | ts ); \
    TRICE_PUT8_3 ( v0, v1, v2 ); \
    TRICE_LEAVE

#define Trice8m_4( tid, v0, v1, v2, v3 ) \
    TRICE_ENTER \
    uint16_t ts = TriceStamp16(); \
    TRICE_PUT(0x80008000|(tid<<16)|tid); \
    TRICE_PUT( 4<<24 | (TRICE_CYCLE<<16) | ts ); \
    TRICE_PUT8_4( v0, v1, v2, v3 ); \
    TRICE_LEAVE

#define Trice8m_5( tid, v0, v1, v2, v3, v4 ) \
    TRICE_ENTER \
    uint16_t ts = TriceStamp16(); \
    TRICE_PUT(0x80008000|(tid<<16)|tid); \
    TRICE_PUT( 5<<24 | (TRICE_CYCLE<<16) | ts ); \
    TRICE_PUT8_5( v0, v1, v2, v3, v4 ); \
    TRICE_LEAVE

#define Trice8m_6( tid, v0, v1, v2, v3, v4, v5 ) \
    TRICE_ENTER \
    uint16_t ts = TriceStamp16(); \
    TRICE_PUT(0x80008000|(tid<<16)|tid); \
    TRICE_PUT( 6<<24 | (TRICE_CYCLE<<16) | ts ); \
    TRICE_PUT8_6( v0, v1, v2, v3, v4, v5 ); \
    TRICE_LEAVE

#define Trice8m_7( tid, v0, v1, v2, v3, v4, v5, v6 ) \
    TRICE_ENTER \
    uint16_t ts = TriceStamp16(); \
    TRICE_PUT(0x80008000|(tid<<16)|tid); \
    TRICE_PUT( 7<<24 | (TRICE_CYCLE<<16) | ts ); \
    TRICE_PUT8_7( v0, v1, v2, v3, v4, v5, v6 ); \
    TRICE_LEAVE

#define Trice8m_8( tid, v0, v1, v2, v3, v4, v5, v6, v7 ) \
    TRICE_ENTER \
    uint16_t ts = TriceStamp16(); \
    TRICE_PUT(0x80008000|(tid<<16)|tid); \
    TRICE_PUT( 8<<24 | (TRICE_CYCLE<<16) | ts ); \
    TRICE_PUT8_8( v0, v1, v2, v3, v4, v5, v6, v7 ); \
    TRICE_LEAVE

#define Trice8m_9( tid, v0, v1, v2, v3, v4, v5, v6, v7, v8 ) \
    TRICE_ENTER \
    uint16_t ts = TriceStamp16(); \
    TRICE_PUT(0x80008000|(tid<<16)|tid); \
    TRICE_PUT( 9<<24 | (TRICE_CYCLE<<16) | ts ); \
    TRICE_PUT8_9( v0, v1, v2, v3, v4, v5, v6, v7, v8 ); \
    TRICE_LEAVE

#define Trice8m_10( tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9 ) \
    TRICE_ENTER \
    uint16_t ts = TriceStamp16(); \
    TRICE_PUT(0x80008000|(tid<<16)|tid); \
    TRICE_PUT( 10<<24 | (TRICE_CYCLE<<16) | ts ); \
    TRICE_PUT8_10( v0, v1, v2, v3, v4, v5, v6, v7, v8, v9 ); \
    TRICE_LEAVE

#define Trice8m_11( tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 ) \
    TRICE_ENTER \
    uint16_t ts = TriceStamp16(); \
    TRICE_PUT(0x80008000|(tid<<16)|tid); \
    TRICE_PUT( 11<<24 | (TRICE_CYCLE<<16) | ts ); \
    TRICE_PUT8_11( v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 ); \
    TRICE_LEAVE

#define Trice8m_12( tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 ) \
    TRICE_ENTER \
    uint16_t ts = TriceStamp16(); \
    TRICE_PUT(0x80008000|(tid<<16)|tid); \
    TRICE_PUT( 12<<24 | (TRICE_CYCLE<<16) | ts ); \
    TRICE_PUT8_12( v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 ) \
    TRICE_LEAVE

#define Trice8_1( tid,  fmt, v0 ) Trice8fn_1( tid,  (uint8_t)(v0) ) //!< Trice8M_1 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define Trice8_2( tid,  fmt, v0, v1 ) Trice8fn_2( tid,  (uint8_t)(v0), (uint8_t)(v1) ) //!< Trice8M_2 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define Trice8_3( tid,  fmt, v0, v1, v2 ) Trice8fn_3( tid,  (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2) ) //!< Trice8M_3 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define Trice8_4( tid,  fmt, v0, v1, v2, v3 ) Trice8fn_4( tid,  (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3) ) //!< Trice8M_4 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define Trice8_5( tid,  fmt, v0, v1, v2, v3, v4 ) Trice8fn_5( tid,  (uint8_t)v0, (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3), (uint8_t)(v4) ) //!< Trice8M_5 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define Trice8_6( tid,  fmt, v0, v1, v2, v3, v4, v5 ) Trice8fn_6( tid,  (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3), (uint8_t)(v4), (uint8_t)(v5) ) //!< Trice8M_6 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define Trice8_7( tid,  fmt, v0, v1, v2, v3, v4, v5, v6 ) Trice8fn_7( tid,  (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3), (uint8_t)(v4), (uint8_t)(v5), (uint8_t)(v6) ) //!< Trice8M_7 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define Trice8_8( tid,  fmt, v0, v1, v2, v3, v4, v5, v6, v7 ) Trice8fn_8( tid,  (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3), (uint8_t)(v4), (uint8_t)(v5), (uint8_t)(v6), (uint8_t)(v7) ) //!< Trice8M_8 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define Trice8_9( tid,  fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8 ) Trice8fn_9( tid, (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3), (uint8_t)(v4), (uint8_t)(v5), (uint8_t)(v6), (uint8_t)(v7), (uint8_t)(v8) ) //!< Trice8M_9 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define Trice8_10( tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9 ) Trice8fn_10( tid, (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3), (uint8_t)(v4), (uint8_t)(v5), (uint8_t)(v6), (uint8_t)(v7), (uint8_t)(v8), (uint8_t)(v9) ) //!< Trice8M_10 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define Trice8_11( tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 ) Trice8fn_11( tid, (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3), (uint8_t)(v4), (uint8_t)(v5), (uint8_t)(v6), (uint8_t)(v7), (uint8_t)(v8), (uint8_t)(v9), (uint8_t)(v10) ) //!< Trice8M_11 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define Trice8_12( tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 ) Trice8fn_12( tid, (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3), (uint8_t)(v4), (uint8_t)(v5), (uint8_t)(v6), (uint8_t)(v7), (uint8_t)(v8), (uint8_t)(v9), (uint8_t)(v10), (uint8_t)(v11) ) //!< Trice8M_12 ia macro calling a function to reduce code size, this way avoiding code inlining.

void Trice8fn_1( uint16_t tid,  uint8_t v0 );
void Trice8fn_2( uint16_t tid,  uint8_t v0, uint8_t v1 );
void Trice8fn_3( uint16_t tid,  uint8_t v0, uint8_t v1, uint8_t v2 );
void Trice8fn_4( uint16_t tid,  uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3 );
void Trice8fn_5( uint16_t tid,  uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4 );
void Trice8fn_6( uint16_t tid,  uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5 );
void Trice8fn_7( uint16_t tid,  uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6 );
void Trice8fn_8( uint16_t tid,  uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6, uint8_t v7 );
void Trice8fn_9( uint16_t tid,  uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6, uint8_t v7, uint8_t v8 );
void Trice8fn_10( uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6, uint8_t v7, uint8_t v8, uint8_t v9 );
void Trice8fn_11( uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6, uint8_t v7, uint8_t v8, uint8_t v9, uint8_t v10 );
void Trice8fn_12( uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6, uint8_t v7, uint8_t v8, uint8_t v9, uint8_t v10, uint8_t v11 );

//  #define TRice8_1( fmt, v0 ) //!< TRice8_1 is an empty macro
//  #define TRice8_2( fmt, v0, v1 ) //!< TRice8_2 is an empty macro
//  #define TRice8_3( fmt, v0, v1, v2 ) //!< TRice8_3 is an empty macro
//  #define TRice8_4( fmt, v0, v1, v2, v3 ) //!< TRice8_4 is an empty macro
//  #define TRice8_5( fmt, v0, v1, v2, v3, v4 ) //!< TRice8_5 is an empty macro
//  #define TRice8_6( fmt, v0, v1, v2, v3, v4, v5 ) //!< TRice8_6 is an empty macro
//  #define TRice8_7( fmt, v0, v1, v2, v3, v4, v5, v6 ) //!< TRice8_7 is an empty macro
//  #define TRice8_8( fmt, v0, v1, v2, v3, v4, v5, v6, v7 ) //!< TRice8_8 is an empty macro
//  #define TRice8_9( fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8 ) //!< TRice8_9 is an empty macro
//  #define TRice8_10( fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9 ) //!< TRice8_10 is an empty macro
//  #define TRice8_11( fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 ) //!< TRice8_11 is an empty macro
//  #define TRice8_12( fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 ) //!< TRice8_12 is an empty macro

//! TRice8m_1 writes trice data as fast as possible in a buffer.
//! \param id is a 14 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 a 8 bit bit value
#define TRice8m_1( tid, v0 ) \
    TRICE_ENTER \
    uint32_t ts = TRICE_HTOTL(TriceStamp32()); \
    TRICE_PUT((ts<<16) | 0xc000 | tid); \
    TRICE_PUT( 1<<24 | (TRICE_CYCLE<<16) | (ts>>16) ); \
    TRICE_PUT8_1( v0 ) \
    TRICE_LEAVE

#define TRice8m_2( tid, v0, v1 ) \
    TRICE_ENTER \
    uint32_t ts = TRICE_HTOTL(TriceStamp32()); \
    TRICE_PUT((ts<<16) | 0xc000 | tid); \
    TRICE_PUT( 2<<24 | (TRICE_CYCLE<<16) | (ts>>16) ); \
    TRICE_PUT8_2( v0, v1); \
    TRICE_LEAVE

#define TRice8m_3( tid, v0, v1, v2 ) \
    TRICE_ENTER \
    uint32_t ts = TRICE_HTOTL(TriceStamp32()); \
    TRICE_PUT((ts<<16) | 0xc000 | tid); \
    TRICE_PUT( 3<<24 | (TRICE_CYCLE<<16) | (ts>>16) ); \
    TRICE_PUT8_3 ( v0, v1, v2 ); \
    TRICE_LEAVE

#define TRice8m_4( tid, v0, v1, v2, v3 ) \
    TRICE_ENTER \
    uint32_t ts = TRICE_HTOTL(TriceStamp32()); \
    TRICE_PUT((ts<<16) | 0xc000 | tid); \
    TRICE_PUT( 4<<24 | (TRICE_CYCLE<<16) | (ts>>16) ); \
    TRICE_PUT8_4( v0, v1, v2, v3 ); \
    TRICE_LEAVE

#define TRice8m_5( tid, v0, v1, v2, v3, v4 ) \
    TRICE_ENTER \
    uint32_t ts = TRICE_HTOTL(TriceStamp32()); \
    TRICE_PUT((ts<<16) | 0xc000 | tid); \
    TRICE_PUT( 5<<24 | (TRICE_CYCLE<<16) | (ts>>16) ); \
    TRICE_PUT8_5( v0, v1, v2, v3, v4 ); \
    TRICE_LEAVE

#define TRice8m_6( tid, v0, v1, v2, v3, v4, v5 ) \
    TRICE_ENTER \
    uint32_t ts = TRICE_HTOTL(TriceStamp32()); \
    TRICE_PUT((ts<<16) | 0xc000 | tid); \
    TRICE_PUT( 6<<24 | (TRICE_CYCLE<<16) | (ts>>16) ); \
    TRICE_PUT8_6( v0, v1, v2, v3, v4, v5 ); \
    TRICE_LEAVE

#define TRice8m_7( tid, v0, v1, v2, v3, v4, v5, v6 ) \
    TRICE_ENTER \
    uint32_t ts = TRICE_HTOTL(TriceStamp32()); \
    TRICE_PUT((ts<<16) | 0xc000 | tid); \
    TRICE_PUT( 7<<24 | (TRICE_CYCLE<<16) | (ts>>16) ); \
    TRICE_PUT8_7( v0, v1, v2, v3, v4, v5, v6 ); \
    TRICE_LEAVE

#define TRice8m_8( tid, v0, v1, v2, v3, v4, v5, v6, v7 ) \
    TRICE_ENTER \
    uint32_t ts = TRICE_HTOTL(TriceStamp32()); \
    TRICE_PUT((ts<<16) | 0xc000 | tid); \
    TRICE_PUT( 8<<24 | (TRICE_CYCLE<<16) | (ts>>16) ); \
    TRICE_PUT8_8( v0, v1, v2, v3, v4, v5, v6, v7 ); \
    TRICE_LEAVE

#define TRice8m_9( tid, v0, v1, v2, v3, v4, v5, v6, v7, v8 ) \
    TRICE_ENTER \
    uint32_t ts = TRICE_HTOTL(TriceStamp32()); \
    TRICE_PUT((ts<<16) | 0xc000 | tid); \
    TRICE_PUT( 9<<24 | (TRICE_CYCLE<<16) | (ts>>16) ); \
    TRICE_PUT8_9( v0, v1, v2, v3, v4, v5, v6, v7, v8 ); \
    TRICE_LEAVE

#define TRice8m_10( tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9 ) \
    TRICE_ENTER \
    uint32_t ts = TRICE_HTOTL(TriceStamp32()); \
    TRICE_PUT((ts<<16) | 0xc000 | tid); \
    TRICE_PUT( 10<<24 | (TRICE_CYCLE<<16) | (ts>>16) ); \
    TRICE_PUT8_10( v0, v1, v2, v3, v4, v5, v6, v7, v8, v9 ); \
    TRICE_LEAVE

#define TRice8m_11( tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 ) \
    TRICE_ENTER \
    uint32_t ts = TRICE_HTOTL(TriceStamp32()); \
    TRICE_PUT((ts<<16) | 0xc000 | tid); \
    TRICE_PUT( 11<<24 | (TRICE_CYCLE<<16) | (ts>>16) ); \
    TRICE_PUT8_11( v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 ); \
    TRICE_LEAVE

#define TRice8m_12( tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 ) \
    TRICE_ENTER \
    uint32_t ts = TRICE_HTOTL(TriceStamp32()); \
    TRICE_PUT((ts<<16) | 0xc000 | tid); \
    TRICE_PUT( 12<<24 | (TRICE_CYCLE<<16) | (ts>>16) ); \
    TRICE_PUT8_12( v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 ) \
    TRICE_LEAVE

#define TRice8_1( tid,  fmt, v0 ) TRice8fn_1( tid,  (uint8_t)(v0) ) //!< TRice8M_1 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define TRice8_2( tid,  fmt, v0, v1 ) TRice8fn_2( tid,  (uint8_t)(v0), (uint8_t)(v1) ) //!< TRice8M_2 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define TRice8_3( tid,  fmt, v0, v1, v2 ) TRice8fn_3( tid,  (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2) ) //!< TRice8M_3 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define TRice8_4( tid,  fmt, v0, v1, v2, v3 ) TRice8fn_4( tid,  (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3) ) //!< TRice8M_4 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define TRice8_5( tid,  fmt, v0, v1, v2, v3, v4 ) TRice8fn_5( tid,  (uint8_t)v0, (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3), (uint8_t)(v4) ) //!< TRice8M_5 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define TRice8_6( tid,  fmt, v0, v1, v2, v3, v4, v5 ) TRice8fn_6( tid,  (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3), (uint8_t)(v4), (uint8_t)(v5) ) //!< TRice8M_6 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define TRice8_7( tid,  fmt, v0, v1, v2, v3, v4, v5, v6 ) TRice8fn_7( tid,  (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3), (uint8_t)(v4), (uint8_t)(v5), (uint8_t)(v6) ) //!< TRice8M_7 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define TRice8_8( tid,  fmt, v0, v1, v2, v3, v4, v5, v6, v7 ) TRice8fn_8( tid,  (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3), (uint8_t)(v4), (uint8_t)(v5), (uint8_t)(v6), (uint8_t)(v7) ) //!< TRice8M_8 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define TRice8_9( tid,  fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8 ) TRice8fn_9( tid, (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3), (uint8_t)(v4), (uint8_t)(v5), (uint8_t)(v6), (uint8_t)(v7), (uint8_t)(v8) ) //!< TRice8M_9 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define TRice8_10( tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9 ) TRice8fn_10( tid, (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3), (uint8_t)(v4), (uint8_t)(v5), (uint8_t)(v6), (uint8_t)(v7), (uint8_t)(v8), (uint8_t)(v9) ) //!< TRice8M_10 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define TRice8_11( tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 ) TRice8fn_11( tid, (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3), (uint8_t)(v4), (uint8_t)(v5), (uint8_t)(v6), (uint8_t)(v7), (uint8_t)(v8), (uint8_t)(v9), (uint8_t)(v10) ) //!< TRice8M_11 ia macro calling a function to reduce code size, this way avoiding code inlining.
#define TRice8_12( tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 ) TRice8fn_12( tid, (uint8_t)(v0), (uint8_t)(v1), (uint8_t)(v2), (uint8_t)(v3), (uint8_t)(v4), (uint8_t)(v5), (uint8_t)(v6), (uint8_t)(v7), (uint8_t)(v8), (uint8_t)(v9), (uint8_t)(v10), (uint8_t)(v11) ) //!< TRice8M_12 ia macro calling a function to reduce code size, this way avoiding code inlining.

void TRice8fn_1( uint16_t tid,  uint8_t v0 );
void TRice8fn_2( uint16_t tid,  uint8_t v0, uint8_t v1 );
void TRice8fn_3( uint16_t tid,  uint8_t v0, uint8_t v1, uint8_t v2 );
void TRice8fn_4( uint16_t tid,  uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3 );
void TRice8fn_5( uint16_t tid,  uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4 );
void TRice8fn_6( uint16_t tid,  uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5 );
void TRice8fn_7( uint16_t tid,  uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6 );
void TRice8fn_8( uint16_t tid,  uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6, uint8_t v7 );
void TRice8fn_9( uint16_t tid,  uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6, uint8_t v7, uint8_t v8 );
void TRice8fn_10( uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6, uint8_t v7, uint8_t v8, uint8_t v9 );
void TRice8fn_11( uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6, uint8_t v7, uint8_t v8, uint8_t v9, uint8_t v10 );
void TRice8fn_12( uint16_t tid, uint8_t v0, uint8_t v1, uint8_t v2, uint8_t v3, uint8_t v4, uint8_t v5, uint8_t v6, uint8_t v7, uint8_t v8, uint8_t v9, uint8_t v10, uint8_t v11 );

///////////////////////////////////////////////////////////////////////////////
//

#if TRICE_DEFAULT_PARAMETER_BIT_WIDTH == 8

#define TRICE_1  TRICE8_1  //!< Default parameter bit width for 1  parameter count TRICE is 8.
#define TRICE_2  TRICE8_2  //!< Default parameter bit width for 2  parameter count TRICE is 8.
#define TRICE_3  TRICE8_3  //!< Default parameter bit width for 3  parameter count TRICE is 8.
#define TRICE_4  TRICE8_4  //!< Default parameter bit width for 4  parameter count TRICE is 8.
#define TRICE_5  TRICE8_5  //!< Default parameter bit width for 5  parameter count TRICE is 8.
#define TRICE_6  TRICE8_6  //!< Default parameter bit width for 6  parameter count TRICE is 8.
#define TRICE_7  TRICE8_7  //!< Default parameter bit width for 7  parameter count TRICE is 8.
#define TRICE_8  TRICE8_8  //!< Default parameter bit width for 8  parameter count TRICE is 8.
#define TRICE_9  TRICE8_9  //!< Default parameter bit width for 9  parameter count TRICE is 8.
#define TRICE_10 TRICE8_10 //!< Default parameter bit width for 10 parameter count TRICE is 8.
#define TRICE_11 TRICE8_11 //!< Default parameter bit width for 11 parameter count TRICE is 8.
#define TRICE_12 TRICE8_12 //!< Default parameter bit width for 12 parameter count TRICE is 8.

#define trice_1  trice8_1  //!< Default parameter bit width for 1  parameter count trice is 8.
#define trice_2  trice8_2  //!< Default parameter bit width for 2  parameter count trice is 8.
#define trice_3  trice8_3  //!< Default parameter bit width for 3  parameter count trice is 8.
#define trice_4  trice8_4  //!< Default parameter bit width for 4  parameter count trice is 8.
#define trice_5  trice8_5  //!< Default parameter bit width for 5  parameter count trice is 8.
#define trice_6  trice8_6  //!< Default parameter bit width for 6  parameter count trice is 8.
#define trice_7  trice8_7  //!< Default parameter bit width for 7  parameter count trice is 8.
#define trice_8  trice8_8  //!< Default parameter bit width for 8  parameter count trice is 8.
#define trice_9  trice8_9  //!< Default parameter bit width for 9  parameter count trice is 8.
#define trice_10 trice8_10 //!< Default parameter bit width for 10 parameter count trice is 8.
#define trice_11 trice8_11 //!< Default parameter bit width for 11 parameter count trice is 8.
#define trice_12 trice8_12 //!< Default parameter bit width for 12 parameter count trice is 8.

#define Trice_1  Trice8_1  //!< Default parameter bit width for 1  parameter count Trice is 8.
#define Trice_2  Trice8_2  //!< Default parameter bit width for 2  parameter count Trice is 8.
#define Trice_3  Trice8_3  //!< Default parameter bit width for 3  parameter count Trice is 8.
#define Trice_4  Trice8_4  //!< Default parameter bit width for 4  parameter count Trice is 8.
#define Trice_5  Trice8_5  //!< Default parameter bit width for 5  parameter count Trice is 8.
#define Trice_6  Trice8_6  //!< Default parameter bit width for 6  parameter count Trice is 8.
#define Trice_7  Trice8_7  //!< Default parameter bit width for 7  parameter count Trice is 8.
#define Trice_8  Trice8_8  //!< Default parameter bit width for 8  parameter count Trice is 8.
#define Trice_9  Trice8_9  //!< Default parameter bit width for 9  parameter count Trice is 8.
#define Trice_10 Trice8_10 //!< Default parameter bit width for 10 parameter count Trice is 8.
#define Trice_11 Trice8_11 //!< Default parameter bit width for 11 parameter count Trice is 8.
#define Trice_12 Trice8_12 //!< Default parameter bit width for 12 parameter count Trice is 8.

#define TRice_1  TRice8_1  //!< Default parameter bit width for 1  parameter count TRice is 8.
#define TRice_2  TRice8_2  //!< Default parameter bit width for 2  parameter count TRice is 8.
#define TRice_3  TRice8_3  //!< Default parameter bit width for 3  parameter count TRice is 8.
#define TRice_4  TRice8_4  //!< Default parameter bit width for 4  parameter count TRice is 8.
#define TRice_5  TRice8_5  //!< Default parameter bit width for 5  parameter count TRice is 8.
#define TRice_6  TRice8_6  //!< Default parameter bit width for 6  parameter count TRice is 8.
#define TRice_7  TRice8_7  //!< Default parameter bit width for 7  parameter count TRice is 8.
#define TRice_8  TRice8_8  //!< Default parameter bit width for 8  parameter count TRice is 8.
#define TRice_9  TRice8_9  //!< Default parameter bit width for 9  parameter count TRice is 8.
#define TRice_10 TRice8_10 //!< Default parameter bit width for 10 parameter count TRice is 8.
#define TRice_11 TRice8_11 //!< Default parameter bit width for 11 parameter count TRice is 8.
#define TRice_12 TRice8_12 //!< Default parameter bit width for 12 parameter count TRice is 8.

#define triceM_1  trice8M_1  //!< Default parameter bit width for 1  parameter count triceM is 8.
#define triceM_2  trice8M_2  //!< Default parameter bit width for 2  parameter count triceM is 8.
#define triceM_3  trice8M_3  //!< Default parameter bit width for 3  parameter count triceM is 8.
#define triceM_4  trice8M_4  //!< Default parameter bit width for 4  parameter count triceM is 8.
#define triceM_5  trice8M_5  //!< Default parameter bit width for 5  parameter count triceM is 8.
#define triceM_6  trice8M_6  //!< Default parameter bit width for 6  parameter count triceM is 8.
#define triceM_7  trice8M_7  //!< Default parameter bit width for 7  parameter count triceM is 8.
#define triceM_8  trice8M_8  //!< Default parameter bit width for 8  parameter count triceM is 8.
#define triceM_9  trice8M_9  //!< Default parameter bit width for 9  parameter count triceM is 8.
#define triceM_10 trice8M_10 //!< Default parameter bit width for 10 parameter count triceM is 8.
#define triceM_11 trice8M_11 //!< Default parameter bit width for 11 parameter count triceM is 8.
#define triceM_12 trice8M_12 //!< Default parameter bit width for 12 parameter count triceM is 8.

#define TriceM_1  Trice8M_1  //!< Default parameter bit width for 1  parameter count TriceM is 8.
#define TriceM_2  Trice8M_2  //!< Default parameter bit width for 2  parameter count TriceM is 8.
#define TriceM_3  Trice8M_3  //!< Default parameter bit width for 3  parameter count TriceM is 8.
#define TriceM_4  Trice8M_4  //!< Default parameter bit width for 4  parameter count TriceM is 8.
#define TriceM_5  Trice8M_5  //!< Default parameter bit width for 5  parameter count TriceM is 8.
#define TriceM_6  Trice8M_6  //!< Default parameter bit width for 6  parameter count TriceM is 8.
#define TriceM_7  Trice8M_7  //!< Default parameter bit width for 7  parameter count TriceM is 8.
#define TriceM_8  Trice8M_8  //!< Default parameter bit width for 8  parameter count TriceM is 8.
#define TriceM_9  Trice8M_9  //!< Default parameter bit width for 9  parameter count TriceM is 8.
#define TriceM_10 Trice8M_10 //!< Default parameter bit width for 10 parameter count TriceM is 8.
#define TriceM_11 Trice8M_11 //!< Default parameter bit width for 11 parameter count TriceM is 8.
#define TriceM_12 Trice8M_12 //!< Default parameter bit width for 12 parameter count TriceM is 8.

#define TRiceM_1  TRice8M_1  //!< Default parameter bit width for 1  parameter count TRiceM is 8.
#define TRiceM_2  TRice8M_2  //!< Default parameter bit width for 2  parameter count TRiceM is 8.
#define TRiceM_3  TRice8M_3  //!< Default parameter bit width for 3  parameter count TRiceM is 8.
#define TRiceM_4  TRice8M_4  //!< Default parameter bit width for 4  parameter count TRiceM is 8.
#define TRiceM_5  TRice8M_5  //!< Default parameter bit width for 5  parameter count TRiceM is 8.
#define TRiceM_6  TRice8M_6  //!< Default parameter bit width for 6  parameter count TRiceM is 8.
#define TRiceM_7  TRice8M_7  //!< Default parameter bit width for 7  parameter count TRiceM is 8.
#define TRiceM_8  TRice8M_8  //!< Default parameter bit width for 8  parameter count TRiceM is 8.
#define TRiceM_9  TRice8M_9  //!< Default parameter bit width for 9  parameter count TRiceM is 8.
#define TRiceM_10 TRice8M_10 //!< Default parameter bit width for 10 parameter count TRiceM is 8.
#define TRiceM_11 TRice8M_11 //!< Default parameter bit width for 11 parameter count TRiceM is 8.
#define TRiceM_12 TRice8M_12 //!< Default parameter bit width for 12 parameter count TRiceM is 8.

#endif // #if TRICE_DEFAULT_PARAMETER_BIT_WIDTH == 8

//
///////////////////////////////////////////////////////////////////////////////
