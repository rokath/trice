/*! \file internalTrice16.h
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/

#include <stdint.h>


#define TRICE_PUT16_1( v0 )                                                 TRICE_PUT( TRICE_SHORT0(v0) ); 

#define TRICE_PUT16_2( v0, v1                                           )   TRICE_PUT( TRICE_SHORT0(v0) | TRICE_SHORT1(v1) ); 
                                                                           
#define TRICE_PUT16_3( v0, v1, v2                                       )   TRICE_PUT(TRICE_SHORT0(v0) | TRICE_SHORT1(v1) ); \
                                                                            TRICE_PUT(TRICE_SHORT0(v2) ); 
                                                      
#define TRICE_PUT16_4( v0, v1, v2, v3                                   )   TRICE_PUT(TRICE_SHORT0(v0) | TRICE_SHORT1(v1) ); \
                                                                            TRICE_PUT(TRICE_SHORT0(v2) | TRICE_SHORT1(v3) );
                                                                    
#define TRICE_PUT16_5( v0, v1, v2, v3, v4                               )   TRICE_PUT(TRICE_SHORT0(v0) | TRICE_SHORT1(v1) ); \
                                                                            TRICE_PUT(TRICE_SHORT0(v2) | TRICE_SHORT1(v3) ); \
                                                                            TRICE_PUT(TRICE_SHORT0(v4) ); \
                                                                 
#define TRICE_PUT16_6( v0, v1, v2, v3, v4, v5                           )   TRICE_PUT(TRICE_SHORT0(v0) | TRICE_SHORT1(v1) ); \
                                                                            TRICE_PUT(TRICE_SHORT0(v2) | TRICE_SHORT1(v3) ); \
                                                                            TRICE_PUT(TRICE_SHORT0(v4) | TRICE_SHORT1(v5) ); 
                                                                           
#define TRICE_PUT16_7( v0, v1, v2, v3, v4, v5, v6                       )   TRICE_PUT(TRICE_SHORT0(v0) | TRICE_SHORT1(v1) ); \
                                                                            TRICE_PUT(TRICE_SHORT0(v2) | TRICE_SHORT1(v3) ); \
                                                                            TRICE_PUT(TRICE_SHORT0(v4) | TRICE_SHORT1(v5) ); \
                                                                            TRICE_PUT(TRICE_SHORT0(v6) );
                                                                           
#define TRICE_PUT16_8( v0, v1, v2, v3, v4, v5, v6, v7                   )   TRICE_PUT(TRICE_SHORT0(v0) | TRICE_SHORT1(v1) ); \
                                                                            TRICE_PUT(TRICE_SHORT0(v2) | TRICE_SHORT1(v3) ); \
                                                                            TRICE_PUT(TRICE_SHORT0(v4) | TRICE_SHORT1(v5) ); \
                                                                            TRICE_PUT(TRICE_SHORT0(v6) | TRICE_SHORT1(v7) );
                                                                           
#define TRICE_PUT16_9( v0, v1, v2, v3, v4, v5, v6, v7, v8               )   TRICE_PUT(TRICE_SHORT0(v0) | TRICE_SHORT1(v1) ); \
                                                                            TRICE_PUT(TRICE_SHORT0(v2) | TRICE_SHORT1(v3) ); \
                                                                            TRICE_PUT(TRICE_SHORT0(v4) | TRICE_SHORT1(v5) ); \
                                                                            TRICE_PUT(TRICE_SHORT0(v6) | TRICE_SHORT1(v7) ); \
                                                                            TRICE_PUT(TRICE_SHORT0(v8) );
                                                                            
#define TRICE_PUT16_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9           )   TRICE_PUT(TRICE_SHORT0(v0) | TRICE_SHORT1(v1) ); \
                                                                            TRICE_PUT(TRICE_SHORT0(v2) | TRICE_SHORT1(v3) ); \
                                                                            TRICE_PUT(TRICE_SHORT0(v4) | TRICE_SHORT1(v5) ); \
                                                                            TRICE_PUT(TRICE_SHORT0(v6) | TRICE_SHORT1(v7) ); \
                                                                            TRICE_PUT(TRICE_SHORT0(v8) | TRICE_SHORT1(v9) );
                                                                            
#define TRICE_PUT16_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10      )   TRICE_PUT(TRICE_SHORT0(v0) | TRICE_SHORT1(v1) ); \
                                                                            TRICE_PUT(TRICE_SHORT0(v2) | TRICE_SHORT1(v3) ); \
                                                                            TRICE_PUT(TRICE_SHORT0(v4) | TRICE_SHORT1(v5) ); \
                                                                            TRICE_PUT(TRICE_SHORT0(v6) | TRICE_SHORT1(v7) ); \
                                                                            TRICE_PUT(TRICE_SHORT0(v8) | TRICE_SHORT1(v9) ); \
                                                                            TRICE_PUT(TRICE_SHORT0(v10)); \
                                                                            
#define TRICE_PUT16_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 )   TRICE_PUT(TRICE_SHORT0(v0) | TRICE_SHORT1(v1) ); \
                                                                            TRICE_PUT(TRICE_SHORT0(v2) | TRICE_SHORT1(v3) ); \
                                                                            TRICE_PUT(TRICE_SHORT0(v4) | TRICE_SHORT1(v5) ); \
                                                                            TRICE_PUT(TRICE_SHORT0(v6) | TRICE_SHORT1(v7) ); \
                                                                            TRICE_PUT(TRICE_SHORT0(v8) | TRICE_SHORT1(v9) ); \
                                                                            TRICE_PUT(TRICE_SHORT0(v10)| TRICE_SHORT1(v11)); \

//! TRICE16_1 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 a 8 bit bit value
#define TRICE16_1( tid, pFmt, v0 ) \
    TRICE_ENTER tid; CNTC(2); \
    TRICE_PUT16_1( v0 )  \
    TRICE_LEAVE

//! TRICE16_2 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v1 are 8 bit bit values
#define TRICE16_2( id, pFmt, v0, v1 ) \
    TRICE_ENTER id; CNTC(4); \
    TRICE_PUT16_2 ( v0, v1 ); \
    TRICE_LEAVE

//! TRICE16_3 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v2 are 8 bit bit values
#define TRICE16_3( id, pFmt, v0, v1, v2 ) \
    TRICE_ENTER id; CNTC(6); \
    TRICE_PUT16_3( v0, v1, v2 ); \
    TRICE_LEAVE

//! TRICE16_4 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v3 are 8 bit bit values
#define TRICE16_4( id, pFmt, v0, v1, v2, v3 ) \
    TRICE_ENTER id; CNTC(8); \
    TRICE_PUT16_4( v0, v1, v2, v3  ); \
    TRICE_LEAVE

//! TRICE16_5 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v4 are 8 bit bit values
#define TRICE16_5( id, pFmt, v0, v1, v2, v3, v4 ) \
    TRICE_ENTER id; CNTC(10); \
    TRICE_PUT16_5( v0, v1, v2, v3, v4  ); \
    TRICE_LEAVE

//! TRICE16_6 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v5 are 8 bit bit values
#define TRICE16_6( id, pFmt, v0, v1, v2, v3, v4, v5 ) \
    TRICE_ENTER id; CNTC(12); \
    TRICE_PUT16_6( v0, v1, v2, v3, v4, v5 ); \
    TRICE_LEAVE

//! TRICE16_8 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v6 are 8 bit bit values
#define TRICE16_7( id, pFmt, v0, v1, v2, v3, v4, v5, v6 ) \
    TRICE_ENTER id; CNTC(14); \
    TRICE_PUT16_7( v0, v1, v2, v3, v4, v5, v6 ); \
    TRICE_LEAVE

//! TRICE16_8 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v7 are 8 bit bit values
#define TRICE16_8( id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7 ) \
    TRICE_ENTER id; CNTC(16); \
    TRICE_PUT16_8( v0, v1, v2, v3, v4, v5, v6, v7 ); \
    TRICE_LEAVE

//! TRICE16_8 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v7 are 8 bit bit values
#define TRICE16_9( id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8 ) \
    TRICE_ENTER id; CNTC(18); \
    TRICE_PUT16_9( v0, v1, v2, v3, v4, v5, v6, v7, v8 ); \
    TRICE_LEAVE

//! TRICE16_8 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v7 are 8 bit bit values
#define TRICE16_10( id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9 ) \
    TRICE_ENTER id; CNTC(20); \
    TRICE_PUT16_10( v0, v1, v2, v3, v4, v5, v6, v7, v8, v9 ); \
    TRICE_LEAVE

//! TRICE16_8 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v7 are 8 bit bit values
#define TRICE16_11( id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 ) \
    TRICE_ENTER id; CNTC(22); \
    TRICE_PUT16_11( v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 ); \
    TRICE_LEAVE

//! TRICE16_12 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v11 are 8 bit bit values
#define TRICE16_12( id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 ) \
    TRICE_ENTER id; CNTC(24); \
    TRICE_PUT16_12( v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 ) \
    TRICE_LEAVE



#define trice16_1( fmt, v0 ) //!< trice16_1 is an empty macro
#define trice16_2( fmt, v0, v1 ) //!< trice16_2 is an empty macro
#define trice16_3( fmt, v0, v1, v2 ) //!< trice16_3 is an empty macro
#define trice16_4( fmt, v0, v1, v2, v3 ) //!< trice16_4 is an empty macro
#define trice16_5( fmt, v0, v1, v2, v3, v4 ) //!< trice16_5 is an empty macro
#define trice16_6( fmt, v0, v1, v2, v3, v4, v5 ) //!< trice16_6 is an empty macro
#define trice16_7( fmt, v0, v1, v2, v3, v4, v5, v6 ) //!< trice16_7 is an empty macro
#define trice16_8( fmt, v0, v1, v2, v3, v4, v5, v6, v7 ) //!< trice16_8 is an empty macro
#define trice16_9( fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8 ) //!< trice16_9 is an empty macro
#define trice16_10( fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9 ) //!< trice16_10 is an empty macro
#define trice16_11( fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 ) //!< trice16_11 is an empty macro
#define trice16_12( fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 ) //!< trice16_12 is an empty macro

#define trice16_COUNT(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12, NAME,...) NAME
#define trice16(fmt, ...) trice16_COUNT(__VA_ARGS__,trice16_12,trice16_11,trice16_10,trice16_9,trice16_8,trice16_7,trice16_6,trice16_5,trice16_4,trice16_3,trice16_2,trice16_1)(fmt, __VA_ARGS__)

#define trice16_COUNT(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12, NAME,...) NAME
#define trice16_M(tid,fmt, ...) trice16_COUNT(__VA_ARGS__,trice16_12_M,trice16_11_M,trice16_10_M,trice16_9_M,trice16_8_M,trice16_7_M,trice16_6_M,trice16_5_M,trice16_4_M,trice16_3_M,trice16_2_M,trice16_1_M)(tid,fmt, __VA_ARGS__)

//! trice16_1_m writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 a 8 bit bit value
#define trice16_1_m( tid, v0 ) \
    TRICE_ENTER \
    TRICE_PUT( (2<<24) | ((TRICE_CYCLE)<<16) | (0x4000|(tid)) ); \
    TRICE_PUT16_1( v0 ) \
    TRICE_LEAVE

#define trice16_2_m( tid, v0, v1 ) \
    TRICE_ENTER \
    TRICE_PUT( (4<<24) | ((TRICE_CYCLE)<<16) | (0x4000|(tid)) ); \
    TRICE_PUT16_2( v0, v1); \
    TRICE_LEAVE

#define trice16_3_m( tid, v0, v1, v2 ) \
    TRICE_ENTER \
    TRICE_PUT( (6<<24) | ((TRICE_CYCLE)<<16) | (0x4000|(tid)) ); \
    TRICE_PUT16_3 ( v0, v1, v2 ); \
    TRICE_LEAVE

#define trice16_4_m( tid, v0, v1, v2, v3 ) \
    TRICE_ENTER \
    TRICE_PUT( (8<<24) | ((TRICE_CYCLE)<<16) | (0x4000|(tid)) ); \
    TRICE_PUT16_4( v0, v1, v2, v3 ); \
    TRICE_LEAVE

#define trice16_5_m( tid, v0, v1, v2, v3, v4 ) \
    TRICE_ENTER \
    TRICE_PUT( (10<<24) | ((TRICE_CYCLE)<<16) | (0x4000|(tid)) ); \
    TRICE_PUT16_5( v0, v1, v2, v3, v4 ); \
    TRICE_LEAVE

#define trice16_6_m( tid, v0, v1, v2, v3, v4, v5 ) \
    TRICE_ENTER \
    TRICE_PUT( (12<<24) | ((TRICE_CYCLE)<<16) | (0x4000|(tid)) ); \
    TRICE_PUT16_6( v0, v1, v2, v3, v4, v5 ); \
    TRICE_LEAVE

#define trice16_7_m( tid, v0, v1, v2, v3, v4, v5, v6 ) \
    TRICE_ENTER \
    TRICE_PUT( (14<<24) | ((TRICE_CYCLE)<<16) | (0x4000|(tid)) ); \
    TRICE_PUT16_7( v0, v1, v2, v3, v4, v5, v6 ); \
    TRICE_LEAVE

#define trice16_8_m( tid, v0, v1, v2, v3, v4, v5, v6, v7 ) \
    TRICE_ENTER \
    TRICE_PUT( (16<<24) | ((TRICE_CYCLE)<<16) | (0x4000|(tid)) ); \
    TRICE_PUT16_8( v0, v1, v2, v3, v4, v5, v6, v7 ); \
    TRICE_LEAVE

#define trice16_9_m( tid, v0, v1, v2, v3, v4, v5, v6, v7, v8 ) \
    TRICE_ENTER \
    TRICE_PUT( (18<<24) | ((TRICE_CYCLE)<<16) | (0x4000|(tid)) ); \
    TRICE_PUT16_9( v0, v1, v2, v3, v4, v5, v6, v7, v8 ); \
    TRICE_LEAVE

#define trice16_10_m( tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9 ) \
    TRICE_ENTER \
    TRICE_PUT( (20<<24) | ((TRICE_CYCLE)<<16) | (0x4000|(tid)) ); \
    TRICE_PUT16_10( v0, v1, v2, v3, v4, v5, v6, v7, v8, v9 ); \
    TRICE_LEAVE

#define trice16_11_m( tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 ) \
    TRICE_ENTER \
    TRICE_PUT( (22<<24) | ((TRICE_CYCLE)<<16) | (0x4000|(tid)) ); \
    TRICE_PUT16_11( v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 ); \
    TRICE_LEAVE

#define trice16_12_m( tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 ) \
    TRICE_ENTER \
    TRICE_PUT( (24<<24) | ((TRICE_CYCLE)<<16) | (0x4000|(tid)) ); \
    TRICE_PUT16_12( v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 ) \
    TRICE_LEAVE

#define trice16_1_M( tid,  fmt, v0 ) trice16_1_fn( tid,  (uint16_t)(v0) ) //!< trice16_1_M is a macro calling a function to reduce code size.
#define trice16_2_M( tid,  fmt, v0, v1 ) trice16_2_fn( tid,  (uint16_t)(v0), (uint16_t)(v1) ) //!< trice16_2_M is a macro calling a function to reduce code size.
#define trice16_3_M( tid,  fmt, v0, v1, v2 ) trice16_3_fn( tid,  (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2) ) //!< trice16_3_M is a macro calling a function to reduce code size.
#define trice16_4_M( tid,  fmt, v0, v1, v2, v3 ) trice16_4_fn( tid,  (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3) ) //!< trice16_4_M is a macro calling a function to reduce code size.
#define trice16_5_M( tid,  fmt, v0, v1, v2, v3, v4 ) trice16_5_fn( tid,  (uint16_t)v0, (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4) ) //!< trice16_5_M is a macro calling a function to reduce code size.
#define trice16_6_M( tid,  fmt, v0, v1, v2, v3, v4, v5 ) trice16_6_fn( tid,  (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5) ) //!< trice16_6_M is a macro calling a function to reduce code size.
#define trice16_7_M( tid,  fmt, v0, v1, v2, v3, v4, v5, v6 ) trice16_7_fn( tid,  (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6) ) //!< trice16_7_M is a macro calling a function to reduce code size.
#define trice16_8_M( tid,  fmt, v0, v1, v2, v3, v4, v5, v6, v7 ) trice16_8_fn( tid,  (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6), (uint16_t)(v7) ) //!< trice16_8_M is a macro calling a function to reduce code size.
#define trice16_9_M( tid,  fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8 ) trice16_9_fn( tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6), (uint16_t)(v7), (uint16_t)(v8) ) //!< trice16_9_M is a macro calling a function to reduce code size.
#define trice16_10_M( tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9 ) trice16_10_fn( tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6), (uint16_t)(v7), (uint16_t)(v8), (uint16_t)(v9) ) //!< trice16_10_M is a macro calling a function to reduce code size.
#define trice16_11_M( tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 ) trice16_11_fn( tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6), (uint16_t)(v7), (uint16_t)(v8), (uint16_t)(v9), (uint16_t)(v10) ) //!< trice16_11_M is a macro calling a function to reduce code size.
#define trice16_12_M( tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 ) trice16_12_fn( tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6), (uint16_t)(v7), (uint16_t)(v8), (uint16_t)(v9), (uint16_t)(v10), (uint16_t)(v11) ) //!< trice16_12_M is a macro calling a function to reduce code size.

void trice16_1_fn( uint16_t tid,  uint16_t v0 );
void trice16_2_fn( uint16_t tid,  uint16_t v0, uint16_t v1 );
void trice16_3_fn( uint16_t tid,  uint16_t v0, uint16_t v1, uint16_t v2 );
void trice16_4_fn( uint16_t tid,  uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3 );
void trice16_5_fn( uint16_t tid,  uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4 );
void trice16_6_fn( uint16_t tid,  uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5 );
void trice16_7_fn( uint16_t tid,  uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6 );
void trice16_8_fn( uint16_t tid,  uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7 );
void trice16_9_fn( uint16_t tid,  uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7, uint16_t v8 );
void trice16_10_fn( uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7, uint16_t v8, uint16_t v9 );
void trice16_11_fn( uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7, uint16_t v8, uint16_t v9, uint16_t v10 );
void trice16_12_fn( uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7, uint16_t v8, uint16_t v9, uint16_t v10, uint16_t v11 );

#define Trice16_1( fmt, v0 ) //!< Trice16_1 is an empty macro
#define Trice16_2( fmt, v0, v1 ) //!< Trice16_2 is an empty macro
#define Trice16_3( fmt, v0, v1, v2 ) //!< Trice16_3 is an empty macro
#define Trice16_4( fmt, v0, v1, v2, v3 ) //!< Trice16_4 is an empty macro
#define Trice16_5( fmt, v0, v1, v2, v3, v4 ) //!< Trice16_5 is an empty macro
#define Trice16_6( fmt, v0, v1, v2, v3, v4, v5 ) //!< Trice16_6 is an empty macro
#define Trice16_7( fmt, v0, v1, v2, v3, v4, v5, v6 ) //!< Trice16_7 is an empty macro
#define Trice16_8( fmt, v0, v1, v2, v3, v4, v5, v6, v7 ) //!< Trice16_8 is an empty macro
#define Trice16_9( fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8 ) //!< Trice16_9 is an empty macro
#define Trice16_10( fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9 ) //!< Trice16_10 is an empty macro
#define Trice16_11( fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 ) //!< Trice16_11 is an empty macro
#define Trice16_12( fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 ) //!< Trice16_12 is an empty macro

#define Trice16_COUNT(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12, NAME,...) NAME
#define Trice16(fmt, ...) Trice16_COUNT(__VA_ARGS__,Trice16_12,Trice16_11,Trice16_10,Trice16_9,Trice16_8,Trice16_7,Trice16_6,Trice16_5,Trice16_4,Trice16_3,Trice16_2,Trice16_1)(fmt, __VA_ARGS__)

#define Trice16_COUNT(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12, NAME,...) NAME
#define Trice16_M(tid,fmt, ...) Trice16_COUNT(__VA_ARGS__,Trice16_12_M,Trice16_11_M,Trice16_10_M,Trice16_9_M,Trice16_8_M,Trice16_7_M,Trice16_6_M,Trice16_5_M,Trice16_4_M,Trice16_3_M,Trice16_2_M,Trice16_1_M)(tid,fmt, __VA_ARGS__)

//! Trice16_1_m writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 a 8 bit bit value
#define Trice16_1_m( tid, v0 ) \
    TRICE_ENTER \
    uint16_t ts = TriceStamp16(); \
    TRICE_PUT(0x80008000|(tid<<16)|tid); \
    TRICE_PUT( 2<<24 | (TRICE_CYCLE<<16) | ts ); \
    TRICE_PUT16_1( v0 ) \
    TRICE_LEAVE

#define Trice16_2_m( tid, v0, v1 ) \
    TRICE_ENTER \
    uint16_t ts = TriceStamp16(); \
    TRICE_PUT(0x80008000|(tid<<16)|tid); \
    TRICE_PUT( 4<<24 | (TRICE_CYCLE<<16) | ts ); \
    TRICE_PUT16_2( v0, v1); \
    TRICE_LEAVE

#define Trice16_3_m( tid, v0, v1, v2 ) \
    TRICE_ENTER \
    uint16_t ts = TriceStamp16(); \
    TRICE_PUT(0x80008000|(tid<<16)|tid); \
    TRICE_PUT( 6<<24 | (TRICE_CYCLE<<16) | ts ); \
    TRICE_PUT16_3 ( v0, v1, v2 ); \
    TRICE_LEAVE

#define Trice16_4_m( tid, v0, v1, v2, v3 ) \
    TRICE_ENTER \
    uint16_t ts = TriceStamp16(); \
    TRICE_PUT(0x80008000|(tid<<16)|tid); \
    TRICE_PUT( 8<<24 | (TRICE_CYCLE<<16) | ts ); \
    TRICE_PUT16_4( v0, v1, v2, v3 ); \
    TRICE_LEAVE

#define Trice16_5_m( tid, v0, v1, v2, v3, v4 ) \
    TRICE_ENTER \
    uint16_t ts = TriceStamp16(); \
    TRICE_PUT(0x80008000|(tid<<16)|tid); \
    TRICE_PUT( 10<<24 | (TRICE_CYCLE<<16) | ts ); \
    TRICE_PUT16_5( v0, v1, v2, v3, v4 ); \
    TRICE_LEAVE

#define Trice16_6_m( tid, v0, v1, v2, v3, v4, v5 ) \
    TRICE_ENTER \
    uint16_t ts = TriceStamp16(); \
    TRICE_PUT(0x80008000|(tid<<16)|tid); \
    TRICE_PUT( 12<<24 | (TRICE_CYCLE<<16) | ts ); \
    TRICE_PUT16_6( v0, v1, v2, v3, v4, v5 ); \
    TRICE_LEAVE

#define Trice16_7_m( tid, v0, v1, v2, v3, v4, v5, v6 ) \
    TRICE_ENTER \
    uint16_t ts = TriceStamp16(); \
    TRICE_PUT(0x80008000|(tid<<16)|tid); \
    TRICE_PUT( 14<<24 | (TRICE_CYCLE<<16) | ts ); \
    TRICE_PUT16_7( v0, v1, v2, v3, v4, v5, v6 ); \
    TRICE_LEAVE

#define Trice16_8_m( tid, v0, v1, v2, v3, v4, v5, v6, v7 ) \
    TRICE_ENTER \
    uint16_t ts = TriceStamp16(); \
    TRICE_PUT(0x80008000|(tid<<16)|tid); \
    TRICE_PUT( 16<<24 | (TRICE_CYCLE<<16) | ts ); \
    TRICE_PUT16_8( v0, v1, v2, v3, v4, v5, v6, v7 ); \
    TRICE_LEAVE

#define Trice16_9_m( tid, v0, v1, v2, v3, v4, v5, v6, v7, v8 ) \
    TRICE_ENTER \
    uint16_t ts = TriceStamp16(); \
    TRICE_PUT(0x80008000|(tid<<16)|tid); \
    TRICE_PUT( 18<<24 | (TRICE_CYCLE<<16) | ts ); \
    TRICE_PUT16_9( v0, v1, v2, v3, v4, v5, v6, v7, v8 ); \
    TRICE_LEAVE

#define Trice16_10_m( tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9 ) \
    TRICE_ENTER \
    uint16_t ts = TriceStamp16(); \
    TRICE_PUT(0x80008000|(tid<<16)|tid); \
    TRICE_PUT( 20<<24 | (TRICE_CYCLE<<16) | ts ); \
    TRICE_PUT16_10( v0, v1, v2, v3, v4, v5, v6, v7, v8, v9 ); \
    TRICE_LEAVE

#define Trice16_11_m( tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 ) \
    TRICE_ENTER \
    uint16_t ts = TriceStamp16(); \
    TRICE_PUT(0x80008000|(tid<<16)|tid); \
    TRICE_PUT( 22<<24 | (TRICE_CYCLE<<16) | ts ); \
    TRICE_PUT16_11( v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 ); \
    TRICE_LEAVE

#define Trice16_12_m( tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 ) \
    TRICE_ENTER \
    uint16_t ts = TriceStamp16(); \
    TRICE_PUT(0x80008000|(tid<<16)|tid); \
    TRICE_PUT( 24<<24 | (TRICE_CYCLE<<16) | ts ); \
    TRICE_PUT16_12( v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 ) \
    TRICE_LEAVE

#define Trice16_1_M( tid,  fmt, v0 ) Trice16_1_fn( tid,  (uint16_t)(v0) ) //!< Trice16_1_M is a macro calling a function to reduce code size.
#define Trice16_2_M( tid,  fmt, v0, v1 ) Trice16_2_fn( tid,  (uint16_t)(v0), (uint16_t)(v1) ) //!< Trice16_2_M is a macro calling a function to reduce code size.
#define Trice16_3_M( tid,  fmt, v0, v1, v2 ) Trice16_3_fn( tid,  (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2) ) //!< Trice16_3_M is a macro calling a function to reduce code size.
#define Trice16_4_M( tid,  fmt, v0, v1, v2, v3 ) Trice16_4_fn( tid,  (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3) ) //!< Trice16_4_M is a macro calling a function to reduce code size.
#define Trice16_5_M( tid,  fmt, v0, v1, v2, v3, v4 ) Trice16_5_fn( tid,  (uint16_t)v0, (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4) ) //!< Trice16_5_M is a macro calling a function to reduce code size.
#define Trice16_6_M( tid,  fmt, v0, v1, v2, v3, v4, v5 ) Trice16_6_fn( tid,  (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5) ) //!< Trice16_6_M is a macro calling a function to reduce code size.
#define Trice16_7_M( tid,  fmt, v0, v1, v2, v3, v4, v5, v6 ) Trice16_7_fn( tid,  (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6) ) //!< Trice16_7_M is a macro calling a function to reduce code size.
#define Trice16_8_M( tid,  fmt, v0, v1, v2, v3, v4, v5, v6, v7 ) Trice16_8_fn( tid,  (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6), (uint16_t)(v7) ) //!< Trice16_8_M is a macro calling a function to reduce code size.
#define Trice16_9_M( tid,  fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8 ) Trice16_9_fn( tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6), (uint16_t)(v7), (uint16_t)(v8) ) //!< Trice16_9_M is a macro calling a function to reduce code size.
#define Trice16_10_M( tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9 ) Trice16_10_fn( tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6), (uint16_t)(v7), (uint16_t)(v8), (uint16_t)(v9) ) //!< Trice16_10_M is a macro calling a function to reduce code size.
#define Trice16_11_M( tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 ) Trice16_11_fn( tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6), (uint16_t)(v7), (uint16_t)(v8), (uint16_t)(v9), (uint16_t)(v10) ) //!< Trice16_11_M is a macro calling a function to reduce code size.
#define Trice16_12_M( tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 ) Trice16_12_fn( tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6), (uint16_t)(v7), (uint16_t)(v8), (uint16_t)(v9), (uint16_t)(v10), (uint16_t)(v11) ) //!< Trice16_12_M is a macro calling a function to reduce code size.

void Trice16_1_fn( uint16_t tid,  uint16_t v0 );
void Trice16_2_fn( uint16_t tid,  uint16_t v0, uint16_t v1 );
void Trice16_3_fn( uint16_t tid,  uint16_t v0, uint16_t v1, uint16_t v2 );
void Trice16_4_fn( uint16_t tid,  uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3 );
void Trice16_5_fn( uint16_t tid,  uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4 );
void Trice16_6_fn( uint16_t tid,  uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5 );
void Trice16_7_fn( uint16_t tid,  uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6 );
void Trice16_8_fn( uint16_t tid,  uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7 );
void Trice16_9_fn( uint16_t tid,  uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7, uint16_t v8 );
void Trice16_10_fn( uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7, uint16_t v8, uint16_t v9 );
void Trice16_11_fn( uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7, uint16_t v8, uint16_t v9, uint16_t v10 );
void Trice16_12_fn( uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7, uint16_t v8, uint16_t v9, uint16_t v10, uint16_t v11 );





#define TRice16_1( fmt, v0 ) //!< TRice16_1 is an empty macro
#define TRice16_2( fmt, v0, v1 ) //!< TRice16_2 is an empty macro
#define TRice16_3( fmt, v0, v1, v2 ) //!< TRice16_3 is an empty macro
#define TRice16_4( fmt, v0, v1, v2, v3 ) //!< TRice16_4 is an empty macro
#define TRice16_5( fmt, v0, v1, v2, v3, v4 ) //!< TRice16_5 is an empty macro
#define TRice16_6( fmt, v0, v1, v2, v3, v4, v5 ) //!< TRice16_6 is an empty macro
#define TRice16_7( fmt, v0, v1, v2, v3, v4, v5, v6 ) //!< TRice16_7 is an empty macro
#define TRice16_8( fmt, v0, v1, v2, v3, v4, v5, v6, v7 ) //!< TRice16_8 is an empty macro
#define TRice16_9( fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8 ) //!< TRice16_9 is an empty macro
#define TRice16_10( fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9 ) //!< TRice16_10 is an empty macro
#define TRice16_11( fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 ) //!< TRice16_11 is an empty macro
#define TRice16_12( fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 ) //!< TRice16_12 is an empty macro

#define TRice16_COUNT(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12, NAME,...) NAME
#define TRice16(fmt, ...) TRice16_COUNT(__VA_ARGS__,TRice16_12,TRice16_11,TRice16_10,TRice16_9,TRice16_8,TRice16_7,TRice16_6,TRice16_5,TRice16_4,TRice16_3,TRice16_2,TRice16_1)(fmt, __VA_ARGS__)

#define TRice16_COUNT(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12, NAME,...) NAME
#define TRice16_M(tid,fmt, ...) TRice16_COUNT(__VA_ARGS__,TRice16_12_M,TRice16_11_M,TRice16_10_M,TRice16_9_M,TRice16_8_M,TRice16_7_M,TRice16_6_M,TRice16_5_M,TRice16_4_M,TRice16_3_M,TRice16_2_M,TRice16_1_M)(tid,fmt, __VA_ARGS__)

//! TRice16_1_m writes trice data as fast as possible in a buffer.
//! \param id is a 14 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 a 8 bit bit value
#define TRice16_1_m( tid, v0 ) \
    TRICE_ENTER \
    uint32_t ts = TRICE_HTOTL(TriceStamp32()); \
    TRICE_PUT((ts<<16) | 0xc000 | tid); \
    TRICE_PUT( 2<<24 | (TRICE_CYCLE<<16) | (ts>>16) ); \
    TRICE_PUT16_1( v0 ) \
    TRICE_LEAVE

#define TRice16_2_m( tid, v0, v1 ) \
    TRICE_ENTER \
    uint32_t ts = TRICE_HTOTL(TriceStamp32()); \
    TRICE_PUT((ts<<16) | 0xc000 | tid); \
    TRICE_PUT( 4<<24 | (TRICE_CYCLE<<16) | (ts>>16) ); \
    TRICE_PUT16_2( v0, v1); \
    TRICE_LEAVE

#define TRice16_3_m( tid, v0, v1, v2 ) \
    TRICE_ENTER \
    uint32_t ts = TRICE_HTOTL(TriceStamp32()); \
    TRICE_PUT((ts<<16) | 0xc000 | tid); \
    TRICE_PUT( 6<<24 | (TRICE_CYCLE<<16) | (ts>>16) ); \
    TRICE_PUT16_3 ( v0, v1, v2 ); \
    TRICE_LEAVE

#define TRice16_4_m( tid, v0, v1, v2, v3 ) \
    TRICE_ENTER \
    uint32_t ts = TRICE_HTOTL(TriceStamp32()); \
    TRICE_PUT((ts<<16) | 0xc000 | tid); \
    TRICE_PUT( 8<<24 | (TRICE_CYCLE<<16) | (ts>>16) ); \
    TRICE_PUT16_4( v0, v1, v2, v3 ); \
    TRICE_LEAVE

#define TRice16_5_m( tid, v0, v1, v2, v3, v4 ) \
    TRICE_ENTER \
    uint32_t ts = TRICE_HTOTL(TriceStamp32()); \
    TRICE_PUT((ts<<16) | 0xc000 | tid); \
    TRICE_PUT( 10<<24 | (TRICE_CYCLE<<16) | (ts>>16) ); \
    TRICE_PUT16_5( v0, v1, v2, v3, v4 ); \
    TRICE_LEAVE

#define TRice16_6_m( tid, v0, v1, v2, v3, v4, v5 ) \
    TRICE_ENTER \
    uint32_t ts = TRICE_HTOTL(TriceStamp32()); \
    TRICE_PUT((ts<<16) | 0xc000 | tid); \
    TRICE_PUT( 12<<24 | (TRICE_CYCLE<<16) | (ts>>16) ); \
    TRICE_PUT16_6( v0, v1, v2, v3, v4, v5 ); \
    TRICE_LEAVE

#define TRice16_7_m( tid, v0, v1, v2, v3, v4, v5, v6 ) \
    TRICE_ENTER \
    uint32_t ts = TRICE_HTOTL(TriceStamp32()); \
    TRICE_PUT((ts<<16) | 0xc000 | tid); \
    TRICE_PUT( 14<<24 | (TRICE_CYCLE<<16) | (ts>>16) ); \
    TRICE_PUT16_7( v0, v1, v2, v3, v4, v5, v6 ); \
    TRICE_LEAVE

#define TRice16_8_m( tid, v0, v1, v2, v3, v4, v5, v6, v7 ) \
    TRICE_ENTER \
    uint32_t ts = TRICE_HTOTL(TriceStamp32()); \
    TRICE_PUT((ts<<16) | 0xc000 | tid); \
    TRICE_PUT( 16<<24 | (TRICE_CYCLE<<16) | (ts>>16) ); \
    TRICE_PUT16_8( v0, v1, v2, v3, v4, v5, v6, v7 ); \
    TRICE_LEAVE

#define TRice16_9_m( tid, v0, v1, v2, v3, v4, v5, v6, v7, v8 ) \
    TRICE_ENTER \
    uint32_t ts = TRICE_HTOTL(TriceStamp32()); \
    TRICE_PUT((ts<<16) | 0xc000 | tid); \
    TRICE_PUT( 18<<24 | (TRICE_CYCLE<<16) | (ts>>16) ); \
    TRICE_PUT16_9( v0, v1, v2, v3, v4, v5, v6, v7, v8 ); \
    TRICE_LEAVE

#define TRice16_10_m( tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9 ) \
    TRICE_ENTER \
    uint32_t ts = TRICE_HTOTL(TriceStamp32()); \
    TRICE_PUT((ts<<16) | 0xc000 | tid); \
    TRICE_PUT( 20<<24 | (TRICE_CYCLE<<16) | (ts>>16) ); \
    TRICE_PUT16_10( v0, v1, v2, v3, v4, v5, v6, v7, v8, v9 ); \
    TRICE_LEAVE

#define TRice16_11_m( tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 ) \
    TRICE_ENTER \
    uint32_t ts = TRICE_HTOTL(TriceStamp32()); \
    TRICE_PUT((ts<<16) | 0xc000 | tid); \
    TRICE_PUT( 22<<24 | (TRICE_CYCLE<<16) | (ts>>16) ); \
    TRICE_PUT16_11( v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 ); \
    TRICE_LEAVE

#define TRice16_12_m( tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 ) \
    TRICE_ENTER \
    uint32_t ts = TRICE_HTOTL(TriceStamp32()); \
    TRICE_PUT((ts<<16) | 0xc000 | tid); \
    TRICE_PUT( 24<<24 | (TRICE_CYCLE<<16) | (ts>>16) ); \
    TRICE_PUT16_12( v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 ) \
    TRICE_LEAVE

#define TRice16_1_M( tid,  fmt, v0 ) TRice16_1_fn( tid,  (uint16_t)(v0) ) //!< TRice16_1_M is a macro calling a function to reduce code size.
#define TRice16_2_M( tid,  fmt, v0, v1 ) TRice16_2_fn( tid,  (uint16_t)(v0), (uint16_t)(v1) ) //!< TRice16_2_M is a macro calling a function to reduce code size.
#define TRice16_3_M( tid,  fmt, v0, v1, v2 ) TRice16_3_fn( tid,  (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2) ) //!< TRice16_3_M is a macro calling a function to reduce code size.
#define TRice16_4_M( tid,  fmt, v0, v1, v2, v3 ) TRice16_4_fn( tid,  (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3) ) //!< TRice16_4_M is a macro calling a function to reduce code size.
#define TRice16_5_M( tid,  fmt, v0, v1, v2, v3, v4 ) TRice16_5_fn( tid,  (uint16_t)v0, (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4) ) //!< TRice16_5_M is a macro calling a function to reduce code size.
#define TRice16_6_M( tid,  fmt, v0, v1, v2, v3, v4, v5 ) TRice16_6_fn( tid,  (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5) ) //!< TRice16_6_M is a macro calling a function to reduce code size.
#define TRice16_7_M( tid,  fmt, v0, v1, v2, v3, v4, v5, v6 ) TRice16_7_fn( tid,  (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6) ) //!< TRice16_7_M is a macro calling a function to reduce code size.
#define TRice16_8_M( tid,  fmt, v0, v1, v2, v3, v4, v5, v6, v7 ) TRice16_8_fn( tid,  (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6), (uint16_t)(v7) ) //!< TRice16_16_M is a macro calling a function to reduce code size.
#define TRice16_9_M( tid,  fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8 ) TRice16_9_fn( tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6), (uint16_t)(v7), (uint16_t)(v8) ) //!< TRice16_9_M is a macro calling a function to reduce code size.
#define TRice16_10_M( tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9 ) TRice16_10_fn( tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6), (uint16_t)(v7), (uint16_t)(v8), (uint16_t)(v9) ) //!< TRice16_10_M is a macro calling a function to reduce code size.
#define TRice16_11_M( tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 ) TRice16_11_fn( tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6), (uint16_t)(v7), (uint16_t)(v8), (uint16_t)(v9), (uint16_t)(v10) ) //!< TRice16_11_M is a macro calling a function to reduce code size.
#define TRice16_12_M( tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 ) TRice16_12_fn( tid, (uint16_t)(v0), (uint16_t)(v1), (uint16_t)(v2), (uint16_t)(v3), (uint16_t)(v4), (uint16_t)(v5), (uint16_t)(v6), (uint16_t)(v7), (uint16_t)(v8), (uint16_t)(v9), (uint16_t)(v10), (uint16_t)(v11) ) //!< TRice16_12_M is a macro calling a function to reduce code size.

void TRice16_1_fn( uint16_t tid,  uint16_t v0 );
void TRice16_2_fn( uint16_t tid,  uint16_t v0, uint16_t v1 );
void TRice16_3_fn( uint16_t tid,  uint16_t v0, uint16_t v1, uint16_t v2 );
void TRice16_4_fn( uint16_t tid,  uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3 );
void TRice16_5_fn( uint16_t tid,  uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4 );
void TRice16_6_fn( uint16_t tid,  uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5 );
void TRice16_7_fn( uint16_t tid,  uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6 );
void TRice16_8_fn( uint16_t tid,  uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7 );
void TRice16_9_fn( uint16_t tid,  uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7, uint16_t v8 );
void TRice16_10_fn( uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7, uint16_t v8, uint16_t v9 );
void TRice16_11_fn( uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7, uint16_t v8, uint16_t v9, uint16_t v10 );
void TRice16_12_fn( uint16_t tid, uint16_t v0, uint16_t v1, uint16_t v2, uint16_t v3, uint16_t v4, uint16_t v5, uint16_t v6, uint16_t v7, uint16_t v8, uint16_t v9, uint16_t v10, uint16_t v11 );




