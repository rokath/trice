/*! \file internalTrice32.h
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/

#define TRICE32(  tid,fmt, ...) TRICE_COUNT(__VA_ARGS__,TRICE32_12,  TRICE32_11,  TRICE32_10,  TRICE32_9,  TRICE32_8,  TRICE32_7,  TRICE32_6,  TRICE32_5,  TRICE32_4,  TRICE32_3,  TRICE32_2,  TRICE32_1)(  tid,fmt, __VA_ARGS__)
#define trice32(      fmt, ...) TRICE_COUNT(__VA_ARGS__,trice32_12,  trice32_11,  trice32_10,  trice32_9,  trice32_8,  trice32_7,  trice32_6,  trice32_5,  trice32_4,  trice32_3,  trice32_2,  trice32_1)(      fmt, __VA_ARGS__)
#define trice32_M(tid,fmt, ...) TRICE_COUNT(__VA_ARGS__,trice32_12_M,trice32_11_M,trice32_10_M,trice32_9_M,trice32_8_M,trice32_7_M,trice32_6_M,trice32_5_M,trice32_4_M,trice32_3_M,trice32_2_M,trice32_1_M)(tid,fmt, __VA_ARGS__)
#define Trice32(      fmt, ...) TRICE_COUNT(__VA_ARGS__,Trice32_12,  Trice32_11,  Trice32_10,  Trice32_9,  Trice32_8,  Trice32_7,  Trice32_6,  Trice32_5,  Trice32_4,  Trice32_3,  Trice32_2,  Trice32_1)(      fmt, __VA_ARGS__)
#define Trice32_M(tid,fmt, ...) TRICE_COUNT(__VA_ARGS__,Trice32_12_M,Trice32_11_M,Trice32_10_M,Trice32_9_M,Trice32_8_M,Trice32_7_M,Trice32_6_M,Trice32_5_M,Trice32_4_M,Trice32_3_M,Trice32_2_M,Trice32_1_M)(tid,fmt, __VA_ARGS__)
#define TRice32(      fmt, ...) TRICE_COUNT(__VA_ARGS__,TRice32_12,  TRice32_11,  TRice32_10,  TRice32_9,  TRice32_8,  TRice32_7,  TRice32_6,  TRice32_5,  TRice32_4,  TRice32_3,  TRice32_2,  TRice32_1)(      fmt, __VA_ARGS__)
#define TRice32_M(tid,fmt, ...) TRICE_COUNT(__VA_ARGS__,TRice32_12_M,TRice32_11_M,TRice32_10_M,TRice32_9_M,TRice32_8_M,TRice32_7_M,TRice32_6_M,TRice32_5_M,TRice32_4_M,TRice32_3_M,TRice32_2_M,TRice32_1_M)(tid,fmt, __VA_ARGS__)

//!TRICE32_B expects inside pFmt only one format specifier, which is used n times by using pFmt n times.
//! It is usable for showing n 32-bit values.
#define TRICE32_B( id, pFmt, buf, n) do { TRICE_N( id, pFmt, buf, 4*n); } while(0)

//!TRICE32_F expects inside pFmt just a string which is assumed to be a remote function name.
//! The trice tool displays the pFmt string followed by n times (32-bit value i).
//! The idea behind is to generate an id - function pointer referece list from the generated til.json file to
//! compile it into a remote device and execute the inside pFmt named function remotely.
//! Look for "TRICE32_F example" inside triceCheck.c.
#define TRICE32_F  TRICE32_B

#define TRICE_PUT32_1( v0 )                                                 TRICE_PUT( (uint32_t)(v0) ); 

#define TRICE_PUT32_2( v0, v1                                           )   TRICE_PUT( (uint32_t)(v0) ); \
                                                                            TRICE_PUT( (uint32_t)(v1) );
                                                                           
#define TRICE_PUT32_3( v0, v1, v2                                       )   TRICE_PUT( (uint32_t)(v0) ); \
                                                                            TRICE_PUT( (uint32_t)(v1) ); \
                                                                            TRICE_PUT( (uint32_t)(v2) ); 
                                                      
#define TRICE_PUT32_4( v0, v1, v2, v3                                   )   TRICE_PUT( (uint32_t)(v0) ); \
                                                                            TRICE_PUT( (uint32_t)(v1) ); \
                                                                            TRICE_PUT( (uint32_t)(v2) ); \
                                                                            TRICE_PUT( (uint32_t)(v3) );
                                                                    
#define TRICE_PUT32_5( v0, v1, v2, v3, v4                               )   TRICE_PUT( (uint32_t)(v0) ); \
                                                                            TRICE_PUT( (uint32_t)(v1) ); \
                                                                            TRICE_PUT( (uint32_t)(v2) ); \
                                                                            TRICE_PUT( (uint32_t)(v3) ); \
                                                                            TRICE_PUT( (uint32_t)(v4) );
                                                                 
#define TRICE_PUT32_6( v0, v1, v2, v3, v4, v5                           )   TRICE_PUT( (uint32_t)(v0) ); \
                                                                            TRICE_PUT( (uint32_t)(v1) ); \
                                                                            TRICE_PUT( (uint32_t)(v2) ); \
                                                                            TRICE_PUT( (uint32_t)(v3) ); \
                                                                            TRICE_PUT( (uint32_t)(v4) ); \
                                                                            TRICE_PUT( (uint32_t)(v5) );
                                                                           
#define TRICE_PUT32_7( v0, v1, v2, v3, v4, v5, v6                       )   TRICE_PUT( (uint32_t)(v0) ); \
                                                                            TRICE_PUT( (uint32_t)(v1) ); \
                                                                            TRICE_PUT( (uint32_t)(v2) ); \
                                                                            TRICE_PUT( (uint32_t)(v3) ); \
                                                                            TRICE_PUT( (uint32_t)(v4) ); \
                                                                            TRICE_PUT( (uint32_t)(v5) ); \
                                                                            TRICE_PUT( (uint32_t)(v6) );
                                                                           
#define TRICE_PUT32_8( v0, v1, v2, v3, v4, v5, v6, v7                   )   TRICE_PUT( (uint32_t)(v0) ); \
                                                                            TRICE_PUT( (uint32_t)(v1) ); \
                                                                            TRICE_PUT( (uint32_t)(v2) ); \
                                                                            TRICE_PUT( (uint32_t)(v3) ); \
                                                                            TRICE_PUT( (uint32_t)(v4) ); \
                                                                            TRICE_PUT( (uint32_t)(v5) ); \
                                                                            TRICE_PUT( (uint32_t)(v6) ); \
                                                                            TRICE_PUT( (uint32_t)(v7) ); 
                                                                           
#define TRICE_PUT32_9( v0, v1, v2, v3, v4, v5, v6, v7, v8               )   TRICE_PUT( (uint32_t)(v0) ); \
                                                                            TRICE_PUT( (uint32_t)(v1) ); \
                                                                            TRICE_PUT( (uint32_t)(v2) ); \
                                                                            TRICE_PUT( (uint32_t)(v3) ); \
                                                                            TRICE_PUT( (uint32_t)(v4) ); \
                                                                            TRICE_PUT( (uint32_t)(v5) ); \
                                                                            TRICE_PUT( (uint32_t)(v6) ); \
                                                                            TRICE_PUT( (uint32_t)(v7) ); \
                                                                            TRICE_PUT( (uint32_t)(v8) );
                                                                            
#define TRICE_PUT32_10(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9           )   TRICE_PUT( (uint32_t)(v0) ); \
                                                                            TRICE_PUT( (uint32_t)(v1) ); \
                                                                            TRICE_PUT( (uint32_t)(v2) ); \
                                                                            TRICE_PUT( (uint32_t)(v3) ); \
                                                                            TRICE_PUT( (uint32_t)(v4) ); \
                                                                            TRICE_PUT( (uint32_t)(v5) ); \
                                                                            TRICE_PUT( (uint32_t)(v6) ); \
                                                                            TRICE_PUT( (uint32_t)(v7) ); \
                                                                            TRICE_PUT( (uint32_t)(v8) ); \
                                                                            TRICE_PUT( (uint32_t)(v9) );
                                                                            
#define TRICE_PUT32_11(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10      )   TRICE_PUT( (uint32_t)(v0) ); \
                                                                            TRICE_PUT( (uint32_t)(v1) ); \
                                                                            TRICE_PUT( (uint32_t)(v2) ); \
                                                                            TRICE_PUT( (uint32_t)(v3) ); \
                                                                            TRICE_PUT( (uint32_t)(v4) ); \
                                                                            TRICE_PUT( (uint32_t)(v5) ); \
                                                                            TRICE_PUT( (uint32_t)(v6) ); \
                                                                            TRICE_PUT( (uint32_t)(v7) ); \
                                                                            TRICE_PUT( (uint32_t)(v8) ); \
                                                                            TRICE_PUT( (uint32_t)(v9) ); \
                                                                            TRICE_PUT( (uint32_t)(v10) );
                                                                            
#define TRICE_PUT32_12(v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 )   TRICE_PUT( (uint32_t)(v0) ); \
                                                                            TRICE_PUT( (uint32_t)(v1) ); \
                                                                            TRICE_PUT( (uint32_t)(v2) ); \
                                                                            TRICE_PUT( (uint32_t)(v3) ); \
                                                                            TRICE_PUT( (uint32_t)(v4) ); \
                                                                            TRICE_PUT( (uint32_t)(v5) ); \
                                                                            TRICE_PUT( (uint32_t)(v6) ); \
                                                                            TRICE_PUT( (uint32_t)(v7) ); \
                                                                            TRICE_PUT( (uint32_t)(v8) ); \
                                                                            TRICE_PUT( (uint32_t)(v9) ); \
                                                                            TRICE_PUT( (uint32_t)(v10) ); \
                                                                            TRICE_PUT( (uint32_t)(v11) );

//! TRICE32_1 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 a 8 bit bit value
#define TRICE32_1( tid, pFmt, v0 ) \
    TRICE_ENTER tid; CNTC(4); \
    TRICE_PUT32_1( v0 )  \
    TRICE_LEAVE

//! TRICE32_2 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v1 are 8 bit bit values
#define TRICE32_2( id, pFmt, v0, v1 ) \
    TRICE_ENTER id; CNTC(8); \
    TRICE_PUT32_2 ( v0, v1 ); \
    TRICE_LEAVE

//! TRICE32_3 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v2 are 8 bit bit values
#define TRICE32_3( id, pFmt, v0, v1, v2 ) \
    TRICE_ENTER id; CNTC(12); \
    TRICE_PUT32_3( v0, v1, v2 ); \
    TRICE_LEAVE

//! TRICE32_4 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v3 are 8 bit bit values
#define TRICE32_4( id, pFmt, v0, v1, v2, v3 ) \
    TRICE_ENTER id; CNTC(16); \
    TRICE_PUT32_4( v0, v1, v2, v3 ); \
    TRICE_LEAVE

//! TRICE32_5 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v4 are 8 bit bit values
#define TRICE32_5( id, pFmt, v0, v1, v2, v3, v4 ) \
    TRICE_ENTER id; CNTC(20); \
    TRICE_PUT32_5( v0, v1, v2, v3, v4  ); \
    TRICE_LEAVE

//! TRICE32_6 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v5 are 8 bit bit values
#define TRICE32_6( id, pFmt, v0, v1, v2, v3, v4, v5 ) \
    TRICE_ENTER id; CNTC(24); \
    TRICE_PUT32_6( v0, v1, v2, v3, v4, v5 ); \
    TRICE_LEAVE

//! TRICE32_8 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v6 are 8 bit bit values
#define TRICE32_7( id, pFmt, v0, v1, v2, v3, v4, v5, v6 ) \
    TRICE_ENTER id; CNTC(28); \
    TRICE_PUT32_7( v0, v1, v2, v3, v4, v5, v6 ); \
    TRICE_LEAVE

//! TRICE32_8 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v7 are 8 bit bit values
#define TRICE32_8( id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7 ) \
    TRICE_ENTER id; CNTC(32); \
    TRICE_PUT32_8( v0, v1, v2, v3, v4, v5, v6, v7 ); \
    TRICE_LEAVE

//! TRICE32_8 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v7 are 8 bit bit values
#define TRICE32_9( id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8 ) \
    TRICE_ENTER id; CNTC(36); \
    TRICE_PUT32_9( v0, v1, v2, v3, v4, v5, v6, v7, v8 ); \
    TRICE_LEAVE

//! TRICE32_8 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v7 are 8 bit bit values
#define TRICE32_10( id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9 ) \
    TRICE_ENTER id; CNTC(40); \
    TRICE_PUT32_10( v0, v1, v2, v3, v4, v5, v6, v7, v8, v9 ); \
    TRICE_LEAVE

//! TRICE32_8 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v7 are 8 bit bit values
#define TRICE32_11( id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 ) \
    TRICE_ENTER id; CNTC(44); \
    TRICE_PUT32_11( v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 ); \
    TRICE_LEAVE

//! TRICE32_12 writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 - v11 are 8 bit bit values
#define TRICE32_12( id, pFmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 ) \
    TRICE_ENTER id; CNTC(48); \
    TRICE_PUT32_12( v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 ) \
    TRICE_LEAVE

#define trice32_1( fmt, v0 ) //!< trice32_1 is an empty macro
#define trice32_2( fmt, v0, v1 ) //!< trice32_2 is an empty macro
#define trice32_3( fmt, v0, v1, v2 ) //!< trice32_3 is an empty macro
#define trice32_4( fmt, v0, v1, v2, v3 ) //!< trice32_4 is an empty macro
#define trice32_5( fmt, v0, v1, v2, v3, v4 ) //!< trice32_5 is an empty macro
#define trice32_6( fmt, v0, v1, v2, v3, v4, v5 ) //!< trice32_6 is an empty macro
#define trice32_7( fmt, v0, v1, v2, v3, v4, v5, v6 ) //!< trice32_7 is an empty macro
#define trice32_8( fmt, v0, v1, v2, v3, v4, v5, v6, v7 ) //!< trice32_8 is an empty macro
#define trice32_9( fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8 ) //!< trice32_9 is an empty macro
#define trice32_10( fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9 ) //!< trice32_10 is an empty macro
#define trice32_11( fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 ) //!< trice32_11 is an empty macro
#define trice32_12( fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 ) //!< trice32_12 is an empty macro

//! trice32_1_m writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 a 8 bit bit value
#define trice32_1_m( tid, v0 ) \
    TRICE_ENTER \
    TRICE_PUT( (4<<24) | ((TRICE_CYCLE)<<16) | (0x4000|(tid)) ); \
    TRICE_PUT32_1( v0 ) \
    TRICE_LEAVE

#define trice32_2_m( tid, v0, v1 ) \
    TRICE_ENTER \
    TRICE_PUT( (8<<24) | ((TRICE_CYCLE)<<16) | (0x4000|(tid)) ); \
    TRICE_PUT32_2( v0, v1); \
    TRICE_LEAVE

#define trice32_3_m( tid, v0, v1, v2 ) \
    TRICE_ENTER \
    TRICE_PUT( (12<<24) | ((TRICE_CYCLE)<<16) | (0x4000|(tid)) ); \
    TRICE_PUT32_3 ( v0, v1, v2 ); \
    TRICE_LEAVE

#define trice32_4_m( tid, v0, v1, v2, v3 ) \
    TRICE_ENTER \
    TRICE_PUT( (16<<24) | ((TRICE_CYCLE)<<16) | (0x4000|(tid)) ); \
    TRICE_PUT32_4( v0, v1, v2, v3 ); \
    TRICE_LEAVE

#define trice32_5_m( tid, v0, v1, v2, v3, v4 ) \
    TRICE_ENTER \
    TRICE_PUT( (20<<24) | ((TRICE_CYCLE)<<16) | (0x4000|(tid)) ); \
    TRICE_PUT32_5( v0, v1, v2, v3, v4 ); \
    TRICE_LEAVE

#define trice32_6_m( tid, v0, v1, v2, v3, v4, v5 ) \
    TRICE_ENTER \
    TRICE_PUT( (24<<24) | ((TRICE_CYCLE)<<16) | (0x4000|(tid)) ); \
    TRICE_PUT32_6( v0, v1, v2, v3, v4, v5 ); \
    TRICE_LEAVE

#define trice32_7_m( tid, v0, v1, v2, v3, v4, v5, v6 ) \
    TRICE_ENTER \
    TRICE_PUT( (28<<24) | ((TRICE_CYCLE)<<16) | (0x4000|(tid)) ); \
    TRICE_PUT32_7( v0, v1, v2, v3, v4, v5, v6 ); \
    TRICE_LEAVE

#define trice32_8_m( tid, v0, v1, v2, v3, v4, v5, v6, v7 ) \
    TRICE_ENTER \
    TRICE_PUT( (32<<24) | ((TRICE_CYCLE)<<16) | (0x4000|(tid)) ); \
    TRICE_PUT32_8( v0, v1, v2, v3, v4, v5, v6, v7 ); \
    TRICE_LEAVE

#define trice32_9_m( tid, v0, v1, v2, v3, v4, v5, v6, v7, v8 ) \
    TRICE_ENTER \
    TRICE_PUT( (36<<24) | ((TRICE_CYCLE)<<16) | (0x4000|(tid)) ); \
    TRICE_PUT32_9( v0, v1, v2, v3, v4, v5, v6, v7, v8 ); \
    TRICE_LEAVE

#define trice32_10_m( tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9 ) \
    TRICE_ENTER \
    TRICE_PUT( (40<<24) | ((TRICE_CYCLE)<<16) | (0x4000|(tid)) ); \
    TRICE_PUT32_10( v0, v1, v2, v3, v4, v5, v6, v7, v8, v9 ); \
    TRICE_LEAVE

#define trice32_11_m( tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 ) \
    TRICE_ENTER \
    TRICE_PUT( (44<<24) | ((TRICE_CYCLE)<<16) | (0x4000|(tid)) ); \
    TRICE_PUT32_11( v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 ); \
    TRICE_LEAVE

#define trice32_12_m( tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 ) \
    TRICE_ENTER \
    TRICE_PUT( (48<<24) | ((TRICE_CYCLE)<<16) | (0x4000|(tid)) ); \
    TRICE_PUT32_12( v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 ) \
    TRICE_LEAVE

#define trice32_1_M( tid,  fmt, v0 ) trice32_1_fn( tid,  (uint32_t)(v0) ) //!< trice32_1_M is a macro calling a function to reduce code size.
#define trice32_2_M( tid,  fmt, v0, v1 ) trice32_2_fn( tid,  (uint32_t)(v0), (uint32_t)(v1) ) //!< trice32_2_M is a macro calling a function to reduce code size.
#define trice32_3_M( tid,  fmt, v0, v1, v2 ) trice32_3_fn( tid,  (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2) ) //!< trice32_3_M is a macro calling a function to reduce code size.
#define trice32_4_M( tid,  fmt, v0, v1, v2, v3 ) trice32_4_fn( tid,  (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3) ) //!< trice32_4_M is a macro calling a function to reduce code size.
#define trice32_5_M( tid,  fmt, v0, v1, v2, v3, v4 ) trice32_5_fn( tid,  (uint32_t)v0, (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4) ) //!< trice32_5_M is a macro calling a function to reduce code size.
#define trice32_6_M( tid,  fmt, v0, v1, v2, v3, v4, v5 ) trice32_6_fn( tid,  (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5) ) //!< trice32_6_M is a macro calling a function to reduce code size.
#define trice32_7_M( tid,  fmt, v0, v1, v2, v3, v4, v5, v6 ) trice32_7_fn( tid,  (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6) ) //!< trice32_7_M is a macro calling a function to reduce code size.
#define trice32_8_M( tid,  fmt, v0, v1, v2, v3, v4, v5, v6, v7 ) trice32_8_fn( tid,  (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6), (uint32_t)(v7) ) //!< trice32_8_M is a macro calling a function to reduce code size.
#define trice32_9_M( tid,  fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8 ) trice32_9_fn( tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6), (uint32_t)(v7), (uint32_t)(v8) ) //!< trice32_9_M is a macro calling a function to reduce code size.
#define trice32_10_M( tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9 ) trice32_10_fn( tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6), (uint32_t)(v7), (uint32_t)(v8), (uint32_t)(v9) ) //!< trice32_10_M is a macro calling a function to reduce code size.
#define trice32_11_M( tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 ) trice32_11_fn( tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6), (uint32_t)(v7), (uint32_t)(v8), (uint32_t)(v9), (uint32_t)(v10) ) //!< trice32_11_M is a macro calling a function to reduce code size.
#define trice32_12_M( tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 ) trice32_12_fn( tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6), (uint32_t)(v7), (uint32_t)(v8), (uint32_t)(v9), (uint32_t)(v10), (uint32_t)(v11) ) //!< trice32_12_M is a macro calling a function to reduce code size.

void trice32_1_fn( uint32_t tid,  uint32_t v0 );
void trice32_2_fn( uint32_t tid,  uint32_t v0, uint32_t v1 );
void trice32_3_fn( uint32_t tid,  uint32_t v0, uint32_t v1, uint32_t v2 );
void trice32_4_fn( uint32_t tid,  uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3 );
void trice32_5_fn( uint32_t tid,  uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4 );
void trice32_6_fn( uint32_t tid,  uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5 );
void trice32_7_fn( uint32_t tid,  uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6 );
void trice32_8_fn( uint32_t tid,  uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7 );
void trice32_9_fn( uint32_t tid,  uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7, uint32_t v8 );
void trice32_10_fn( uint32_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7, uint32_t v8, uint32_t v9 );
void trice32_11_fn( uint32_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7, uint32_t v8, uint32_t v9, uint32_t v10 );
void trice32_12_fn( uint32_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7, uint32_t v8, uint32_t v9, uint32_t v10, uint32_t v11 );

#define Trice32_1( fmt, v0 ) //!< Trice32_1 is an empty macro
#define Trice32_2( fmt, v0, v1 ) //!< Trice32_2 is an empty macro
#define Trice32_3( fmt, v0, v1, v2 ) //!< Trice32_3 is an empty macro
#define Trice32_4( fmt, v0, v1, v2, v3 ) //!< Trice32_4 is an empty macro
#define Trice32_5( fmt, v0, v1, v2, v3, v4 ) //!< Trice32_5 is an empty macro
#define Trice32_6( fmt, v0, v1, v2, v3, v4, v5 ) //!< Trice32_6 is an empty macro
#define Trice32_7( fmt, v0, v1, v2, v3, v4, v5, v6 ) //!< Trice32_7 is an empty macro
#define Trice32_8( fmt, v0, v1, v2, v3, v4, v5, v6, v7 ) //!< Trice32_8 is an empty macro
#define Trice32_9( fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8 ) //!< Trice32_9 is an empty macro
#define Trice32_10( fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9 ) //!< Trice32_10 is an empty macro
#define Trice32_11( fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 ) //!< Trice32_11 is an empty macro
#define Trice32_12( fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 ) //!< Trice32_12 is an empty macro

//! Trice32_1_m writes trice data as fast as possible in a buffer.
//! \param id is a 16 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 a 8 bit bit value
#define Trice32_1_m( tid, v0 ) \
    TRICE_ENTER \
    uint32_t ts = TriceStamp16(); \
    TRICE_PUT(0x80008000|(tid<<16)|tid); \
    TRICE_PUT( 4<<24 | (TRICE_CYCLE<<16) | ts ); \
    TRICE_PUT32_1( v0 ) \
    TRICE_LEAVE

#define Trice32_2_m( tid, v0, v1 ) \
    TRICE_ENTER \
    uint32_t ts = TriceStamp16(); \
    TRICE_PUT(0x80008000|(tid<<16)|tid); \
    TRICE_PUT( 8<<24 | (TRICE_CYCLE<<16) | ts ); \
    TRICE_PUT32_2( v0, v1); \
    TRICE_LEAVE

#define Trice32_3_m( tid, v0, v1, v2 ) \
    TRICE_ENTER \
    uint32_t ts = TriceStamp16(); \
    TRICE_PUT(0x80008000|(tid<<16)|tid); \
    TRICE_PUT( 12<<24 | (TRICE_CYCLE<<16) | ts ); \
    TRICE_PUT32_3 ( v0, v1, v2 ); \
    TRICE_LEAVE

#define Trice32_4_m( tid, v0, v1, v2, v3 ) \
    TRICE_ENTER \
    uint32_t ts = TriceStamp16(); \
    TRICE_PUT(0x80008000|(tid<<16)|tid); \
    TRICE_PUT( 16<<24 | (TRICE_CYCLE<<16) | ts ); \
    TRICE_PUT32_4( v0, v1, v2, v3 ); \
    TRICE_LEAVE

#define Trice32_5_m( tid, v0, v1, v2, v3, v4 ) \
    TRICE_ENTER \
    uint32_t ts = TriceStamp16(); \
    TRICE_PUT(0x80008000|(tid<<16)|tid); \
    TRICE_PUT( 20<<24 | (TRICE_CYCLE<<16) | ts ); \
    TRICE_PUT32_5( v0, v1, v2, v3, v4 ); \
    TRICE_LEAVE

#define Trice32_6_m( tid, v0, v1, v2, v3, v4, v5 ) \
    TRICE_ENTER \
    uint32_t ts = TriceStamp16(); \
    TRICE_PUT(0x80008000|(tid<<16)|tid); \
    TRICE_PUT( 24<<24 | (TRICE_CYCLE<<16) | ts ); \
    TRICE_PUT32_6( v0, v1, v2, v3, v4, v5 ); \
    TRICE_LEAVE

#define Trice32_7_m( tid, v0, v1, v2, v3, v4, v5, v6 ) \
    TRICE_ENTER \
    uint32_t ts = TriceStamp16(); \
    TRICE_PUT(0x80008000|(tid<<16)|tid); \
    TRICE_PUT( 28<<24 | (TRICE_CYCLE<<16) | ts ); \
    TRICE_PUT32_7( v0, v1, v2, v3, v4, v5, v6 ); \
    TRICE_LEAVE

#define Trice32_8_m( tid, v0, v1, v2, v3, v4, v5, v6, v7 ) \
    TRICE_ENTER \
    uint32_t ts = TriceStamp16(); \
    TRICE_PUT(0x80008000|(tid<<16)|tid); \
    TRICE_PUT( 32<<24 | (TRICE_CYCLE<<16) | ts ); \
    TRICE_PUT32_8( v0, v1, v2, v3, v4, v5, v6, v7 ); \
    TRICE_LEAVE

#define Trice32_9_m( tid, v0, v1, v2, v3, v4, v5, v6, v7, v8 ) \
    TRICE_ENTER \
    uint32_t ts = TriceStamp16(); \
    TRICE_PUT(0x80008000|(tid<<16)|tid); \
    TRICE_PUT( 36<<24 | (TRICE_CYCLE<<16) | ts ); \
    TRICE_PUT32_9( v0, v1, v2, v3, v4, v5, v6, v7, v8 ); \
    TRICE_LEAVE

#define Trice32_10_m( tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9 ) \
    TRICE_ENTER \
    uint32_t ts = TriceStamp16(); \
    TRICE_PUT(0x80008000|(tid<<16)|tid); \
    TRICE_PUT( 40<<24 | (TRICE_CYCLE<<16) | ts ); \
    TRICE_PUT32_10( v0, v1, v2, v3, v4, v5, v6, v7, v8, v9 ); \
    TRICE_LEAVE

#define Trice32_11_m( tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 ) \
    TRICE_ENTER \
    uint32_t ts = TriceStamp16(); \
    TRICE_PUT(0x80008000|(tid<<16)|tid); \
    TRICE_PUT( 44<<24 | (TRICE_CYCLE<<16) | ts ); \
    TRICE_PUT32_11( v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 ); \
    TRICE_LEAVE

#define Trice32_12_m( tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 ) \
    TRICE_ENTER \
    uint32_t ts = TriceStamp16(); \
    TRICE_PUT(0x80008000|(tid<<16)|tid); \
    TRICE_PUT( 48<<24 | (TRICE_CYCLE<<16) | ts ); \
    TRICE_PUT32_12( v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 ) \
    TRICE_LEAVE

#define Trice32_1_M( tid,  fmt, v0 ) Trice32_1_fn( tid,  (uint32_t)(v0) ) //!< Trice32_1_M is a macro calling a function to reduce code size.
#define Trice32_2_M( tid,  fmt, v0, v1 ) Trice32_2_fn( tid,  (uint32_t)(v0), (uint32_t)(v1) ) //!< Trice32_2_M is a macro calling a function to reduce code size.
#define Trice32_3_M( tid,  fmt, v0, v1, v2 ) Trice32_3_fn( tid,  (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2) ) //!< Trice32_3_M is a macro calling a function to reduce code size.
#define Trice32_4_M( tid,  fmt, v0, v1, v2, v3 ) Trice32_4_fn( tid,  (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3) ) //!< Trice32_4_M is a macro calling a function to reduce code size.
#define Trice32_5_M( tid,  fmt, v0, v1, v2, v3, v4 ) Trice32_5_fn( tid,  (uint32_t)v0, (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4) ) //!< Trice32_5_M is a macro calling a function to reduce code size.
#define Trice32_6_M( tid,  fmt, v0, v1, v2, v3, v4, v5 ) Trice32_6_fn( tid,  (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5) ) //!< Trice32_6_M is a macro calling a function to reduce code size.
#define Trice32_7_M( tid,  fmt, v0, v1, v2, v3, v4, v5, v6 ) Trice32_7_fn( tid,  (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6) ) //!< Trice32_7_M is a macro calling a function to reduce code size.
#define Trice32_8_M( tid,  fmt, v0, v1, v2, v3, v4, v5, v6, v7 ) Trice32_8_fn( tid,  (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6), (uint32_t)(v7) ) //!< Trice32_8_M is a macro calling a function to reduce code size.
#define Trice32_9_M( tid,  fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8 ) Trice32_9_fn( tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6), (uint32_t)(v7), (uint32_t)(v8) ) //!< Trice32_9_M is a macro calling a function to reduce code size.
#define Trice32_10_M( tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9 ) Trice32_10_fn( tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6), (uint32_t)(v7), (uint32_t)(v8), (uint32_t)(v9) ) //!< Trice32_10_M is a macro calling a function to reduce code size.
#define Trice32_11_M( tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 ) Trice32_11_fn( tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6), (uint32_t)(v7), (uint32_t)(v8), (uint32_t)(v9), (uint32_t)(v10) ) //!< Trice32_11_M is a macro calling a function to reduce code size.
#define Trice32_12_M( tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 ) Trice32_12_fn( tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6), (uint32_t)(v7), (uint32_t)(v8), (uint32_t)(v9), (uint32_t)(v10), (uint32_t)(v11) ) //!< Trice32_12_M is a macro calling a function to reduce code size.

void Trice32_1_fn( uint32_t tid,  uint32_t v0 );
void Trice32_2_fn( uint32_t tid,  uint32_t v0, uint32_t v1 );
void Trice32_3_fn( uint32_t tid,  uint32_t v0, uint32_t v1, uint32_t v2 );
void Trice32_4_fn( uint32_t tid,  uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3 );
void Trice32_5_fn( uint32_t tid,  uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4 );
void Trice32_6_fn( uint32_t tid,  uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5 );
void Trice32_7_fn( uint32_t tid,  uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6 );
void Trice32_8_fn( uint32_t tid,  uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7 );
void Trice32_9_fn( uint32_t tid,  uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7, uint32_t v8 );
void Trice32_10_fn( uint32_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7, uint32_t v8, uint32_t v9 );
void Trice32_11_fn( uint32_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7, uint32_t v8, uint32_t v9, uint32_t v10 );
void Trice32_12_fn( uint32_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7, uint32_t v8, uint32_t v9, uint32_t v10, uint32_t v11 );

#define TRice32_1( fmt, v0 ) //!< TRice32_1 is an empty macro
#define TRice32_2( fmt, v0, v1 ) //!< TRice32_2 is an empty macro
#define TRice32_3( fmt, v0, v1, v2 ) //!< TRice32_3 is an empty macro
#define TRice32_4( fmt, v0, v1, v2, v3 ) //!< TRice32_4 is an empty macro
#define TRice32_5( fmt, v0, v1, v2, v3, v4 ) //!< TRice32_5 is an empty macro
#define TRice32_6( fmt, v0, v1, v2, v3, v4, v5 ) //!< TRice32_6 is an empty macro
#define TRice32_7( fmt, v0, v1, v2, v3, v4, v5, v6 ) //!< TRice32_7 is an empty macro
#define TRice32_8( fmt, v0, v1, v2, v3, v4, v5, v6, v7 ) //!< TRice32_8 is an empty macro
#define TRice32_9( fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8 ) //!< TRice32_9 is an empty macro
#define TRice32_10( fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9 ) //!< TRice32_10 is an empty macro
#define TRice32_11( fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 ) //!< TRice32_11 is an empty macro
#define TRice32_12( fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 ) //!< TRice32_12 is an empty macro

//! TRice32_1_m writes trice data as fast as possible in a buffer.
//! \param id is a 14 bit Trice id in upper 2 bytes of a 32 bit value
//! \param v0 a 8 bit bit value
#define TRice32_1_m( tid, v0 ) \
    TRICE_ENTER \
    uint32_t ts = TRICE_HTOTL(TriceStamp32()); \
    TRICE_PUT((ts<<16) | 0xc000 | tid); \
    TRICE_PUT( 4<<24 | (TRICE_CYCLE<<16) | (ts>>16) ); \
    TRICE_PUT32_1( v0 ) \
    TRICE_LEAVE

#define TRice32_2_m( tid, v0, v1 ) \
    TRICE_ENTER \
    uint32_t ts = TRICE_HTOTL(TriceStamp32()); \
    TRICE_PUT((ts<<16) | 0xc000 | tid); \
    TRICE_PUT( 8<<24 | (TRICE_CYCLE<<16) | (ts>>16) ); \
    TRICE_PUT32_2( v0, v1); \
    TRICE_LEAVE

#define TRice32_3_m( tid, v0, v1, v2 ) \
    TRICE_ENTER \
    uint32_t ts = TRICE_HTOTL(TriceStamp32()); \
    TRICE_PUT((ts<<16) | 0xc000 | tid); \
    TRICE_PUT( 12<<24 | (TRICE_CYCLE<<16) | (ts>>16) ); \
    TRICE_PUT32_3 ( v0, v1, v2 ); \
    TRICE_LEAVE

#define TRice32_4_m( tid, v0, v1, v2, v3 ) \
    TRICE_ENTER \
    uint32_t ts = TRICE_HTOTL(TriceStamp32()); \
    TRICE_PUT((ts<<16) | 0xc000 | tid); \
    TRICE_PUT( 16<<24 | (TRICE_CYCLE<<16) | (ts>>16) ); \
    TRICE_PUT32_4( v0, v1, v2, v3 ); \
    TRICE_LEAVE

#define TRice32_5_m( tid, v0, v1, v2, v3, v4 ) \
    TRICE_ENTER \
    uint32_t ts = TRICE_HTOTL(TriceStamp32()); \
    TRICE_PUT((ts<<16) | 0xc000 | tid); \
    TRICE_PUT( 20<<24 | (TRICE_CYCLE<<16) | (ts>>16) ); \
    TRICE_PUT32_5( v0, v1, v2, v3, v4 ); \
    TRICE_LEAVE

#define TRice32_6_m( tid, v0, v1, v2, v3, v4, v5 ) \
    TRICE_ENTER \
    uint32_t ts = TRICE_HTOTL(TriceStamp32()); \
    TRICE_PUT((ts<<16) | 0xc000 | tid); \
    TRICE_PUT( 24<<24 | (TRICE_CYCLE<<16) | (ts>>16) ); \
    TRICE_PUT32_6( v0, v1, v2, v3, v4, v5 ); \
    TRICE_LEAVE

#define TRice32_7_m( tid, v0, v1, v2, v3, v4, v5, v6 ) \
    TRICE_ENTER \
    uint32_t ts = TRICE_HTOTL(TriceStamp32()); \
    TRICE_PUT((ts<<16) | 0xc000 | tid); \
    TRICE_PUT( 28<<24 | (TRICE_CYCLE<<16) | (ts>>16) ); \
    TRICE_PUT32_7( v0, v1, v2, v3, v4, v5, v6 ); \
    TRICE_LEAVE

#define TRice32_8_m( tid, v0, v1, v2, v3, v4, v5, v6, v7 ) \
    TRICE_ENTER \
    uint32_t ts = TRICE_HTOTL(TriceStamp32()); \
    TRICE_PUT((ts<<16) | 0xc000 | tid); \
    TRICE_PUT( 32<<24 | (TRICE_CYCLE<<16) | (ts>>16) ); \
    TRICE_PUT32_8( v0, v1, v2, v3, v4, v5, v6, v7 ); \
    TRICE_LEAVE

#define TRice32_9_m( tid, v0, v1, v2, v3, v4, v5, v6, v7, v8 ) \
    TRICE_ENTER \
    uint32_t ts = TRICE_HTOTL(TriceStamp32()); \
    TRICE_PUT((ts<<16) | 0xc000 | tid); \
    TRICE_PUT( 36<<24 | (TRICE_CYCLE<<16) | (ts>>16) ); \
    TRICE_PUT32_9( v0, v1, v2, v3, v4, v5, v6, v7, v8 ); \
    TRICE_LEAVE

#define TRice32_10_m( tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9 ) \
    TRICE_ENTER \
    uint32_t ts = TRICE_HTOTL(TriceStamp32()); \
    TRICE_PUT((ts<<16) | 0xc000 | tid); \
    TRICE_PUT( 40<<24 | (TRICE_CYCLE<<16) | (ts>>16) ); \
    TRICE_PUT32_10( v0, v1, v2, v3, v4, v5, v6, v7, v8, v9 ); \
    TRICE_LEAVE

#define TRice32_11_m( tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 ) \
    TRICE_ENTER \
    uint32_t ts = TRICE_HTOTL(TriceStamp32()); \
    TRICE_PUT((ts<<16) | 0xc000 | tid); \
    TRICE_PUT( 44<<24 | (TRICE_CYCLE<<16) | (ts>>16) ); \
    TRICE_PUT32_11( v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 ); \
    TRICE_LEAVE

#define TRice32_12_m( tid, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 ) \
    TRICE_ENTER \
    uint32_t ts = TRICE_HTOTL(TriceStamp32()); \
    TRICE_PUT((ts<<16) | 0xc000 | tid); \
    TRICE_PUT( 48<<24 | (TRICE_CYCLE<<16) | (ts>>16) ); \
    TRICE_PUT32_12( v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 ) \
    TRICE_LEAVE

#define TRice32_1_M( tid,  fmt, v0 ) TRice32_1_fn( tid,  (uint32_t)(v0) ) //!< TRice32_1_M is a macro calling a function to reduce code size.
#define TRice32_2_M( tid,  fmt, v0, v1 ) TRice32_2_fn( tid,  (uint32_t)(v0), (uint32_t)(v1) ) //!< TRice32_2_M is a macro calling a function to reduce code size.
#define TRice32_3_M( tid,  fmt, v0, v1, v2 ) TRice32_3_fn( tid,  (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2) ) //!< TRice32_3_M is a macro calling a function to reduce code size.
#define TRice32_4_M( tid,  fmt, v0, v1, v2, v3 ) TRice32_4_fn( tid,  (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3) ) //!< TRice32_4_M is a macro calling a function to reduce code size.
#define TRice32_5_M( tid,  fmt, v0, v1, v2, v3, v4 ) TRice32_5_fn( tid,  (uint32_t)v0, (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4) ) //!< TRice32_5_M is a macro calling a function to reduce code size.
#define TRice32_6_M( tid,  fmt, v0, v1, v2, v3, v4, v5 ) TRice32_6_fn( tid,  (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5) ) //!< TRice32_6_M is a macro calling a function to reduce code size.
#define TRice32_7_M( tid,  fmt, v0, v1, v2, v3, v4, v5, v6 ) TRice32_7_fn( tid,  (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6) ) //!< TRice32_7_M is a macro calling a function to reduce code size.
#define TRice32_8_M( tid,  fmt, v0, v1, v2, v3, v4, v5, v6, v7 ) TRice32_8_fn( tid,  (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6), (uint32_t)(v7) ) //!< TRice32_32_M is a macro calling a function to reduce code size.
#define TRice32_9_M( tid,  fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8 ) TRice32_9_fn( tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6), (uint32_t)(v7), (uint32_t)(v8) ) //!< TRice32_9_M is a macro calling a function to reduce code size.
#define TRice32_10_M( tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9 ) TRice32_10_fn( tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6), (uint32_t)(v7), (uint32_t)(v8), (uint32_t)(v9) ) //!< TRice32_10_M is a macro calling a function to reduce code size.
#define TRice32_11_M( tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10 ) TRice32_11_fn( tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6), (uint32_t)(v7), (uint32_t)(v8), (uint32_t)(v9), (uint32_t)(v10) ) //!< TRice32_11_M is a macro calling a function to reduce code size.
#define TRice32_12_M( tid, fmt, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11 ) TRice32_12_fn( tid, (uint32_t)(v0), (uint32_t)(v1), (uint32_t)(v2), (uint32_t)(v3), (uint32_t)(v4), (uint32_t)(v5), (uint32_t)(v6), (uint32_t)(v7), (uint32_t)(v8), (uint32_t)(v9), (uint32_t)(v10), (uint32_t)(v11) ) //!< TRice32_12_M is a macro calling a function to reduce code size.

void TRice32_1_fn( uint32_t tid,  uint32_t v0 );
void TRice32_2_fn( uint32_t tid,  uint32_t v0, uint32_t v1 );
void TRice32_3_fn( uint32_t tid,  uint32_t v0, uint32_t v1, uint32_t v2 );
void TRice32_4_fn( uint32_t tid,  uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3 );
void TRice32_5_fn( uint32_t tid,  uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4 );
void TRice32_6_fn( uint32_t tid,  uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5 );
void TRice32_7_fn( uint32_t tid,  uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6 );
void TRice32_8_fn( uint32_t tid,  uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7 );
void TRice32_9_fn( uint32_t tid,  uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7, uint32_t v8 );
void TRice32_10_fn( uint32_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7, uint32_t v8, uint32_t v9 );
void TRice32_11_fn( uint32_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7, uint32_t v8, uint32_t v9, uint32_t v10 );
void TRice32_12_fn( uint32_t tid, uint32_t v0, uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7, uint32_t v8, uint32_t v9, uint32_t v10, uint32_t v11 );
