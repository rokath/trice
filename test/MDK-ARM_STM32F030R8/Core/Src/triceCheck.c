/*! \file triceCheck.c
\brief trices for tool evaluation
\author thomas.hoehenleitner [at] seerose.net
*******************************************************************************/
#include <float.h>
//#define TRICE_OFF // enable this line to disable trice code generation in this file object
#include "trice.h"
//#define TRICE_FILE Id(11004)

//lint -e666 -e826 -e831 -e665

static int32_t FloatToInt32( float f );
static int64_t DoubleToInt64( double f );
static void exampleOfManualSerialization( void );
static void exampleOfManualJSONencoding(void);
static void exampleOfBuffersAndFunctions(void);

static void showTriceDepth( void ){
    #if TRICE_MODE == TRICE_DOUBLE_BUFFER
    {
        uint16_t triceDepth = TriceDepth();
        uint16_t triceDepthMax = TriceDepthMax();
        TRICE16( Id(0),"MSG: ✅… STOP  TriceDepth = %d, TriceDepthMax = %d + %4u of %d\n", triceDepth, TRICE_DATA_OFFSET, triceDepthMax-TRICE_DATA_OFFSET, TRICE_HALF_BUFFER_SIZE );
    } 
    #endif
    #if TRICE_MODE == TRICE_STREAM_BUFFER
        TRICE( Id(0), "MSG:triceFifoDepthMax = %d of max %d, triceStreamBufferDepthMax = %d of max %d\n", triceFifoDepthMax, TRICE_FIFO_ELEMENTS, triceStreamBufferDepthMax, TRICE_BUFFER_SIZE );
    #endif
}

//! TriceCheck writes out all types of trices with fixed values for testing
//! \details One trice has one subtrace, if param size max 2 bytes. 
//! Traces with more bytes as parameter consist of several subtraces.
void TriceCheck(int index) {
    char* A = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
    char* s = "AAAAAAAAAAAA";
    float  x = (float)1089.6082763671875; // 0x44883377
    double y = 518.0547492508867; // 0x4080307020601050

    switch (index) {
           case __LINE__: 

    // mixed 16-bit and 32-bit timestamps
   TRice8M_1 ( 7009, "msg:value=%d\n", -1 );// trice tool modified code for compilation
   Trice8M_2 ( 5664, "msg:value=%d, %d\n", -1, -2 );// trice tool modified code for compilation
   TRice8M_3 ( 2103, "msg:value=%d, %d, %d\n", -1, -2, -3 );// trice tool modified code for compilation
   Trice8M_4 ( 7087, "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );// trice tool modified code for compilation
   TRice8M_5 ( 4931, "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );// trice tool modified code for compilation
   Trice8M_6 ( 2621, "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );// trice tool modified code for compilation
   TRice8M_7 ( 3488, "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );// trice tool modified code for compilation
   Trice8M_8 ( 4329, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );// trice tool modified code for compilation
   break; case __LINE__: TRice8M_9 ( 6469, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );// trice tool modified code for compilation
   break; case __LINE__: Trice8M_10( 6335, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );// trice tool modified code for compilation
   break; case __LINE__: TRice8M_11( 2410, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );// trice tool modified code for compilation
   break; case __LINE__: Trice8M_12( 2070, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );// trice tool modified code for compilation

   break; case __LINE__: trice8     ( iD(7009), "msg:value=%d\n", -__LINE__  ); // user written code, empty macro for patching, function call, no stamp

#if 1

    // normal use cases (3 macros)
    break; case __LINE__: trice      (           "msg:value=%d\n", -__LINE__  ); // user written code, empty macro for patching, function call, no stamp
    break; case __LINE__: Trice      (           "msg:value=%d\n", -__LINE__  ); // user written code, empty macro for patching, function call, 16-bit stamp
    break; case __LINE__: TRice      (           "msg:value=%d\n", -__LINE__  ); // user written code, empty macro for patching, function call, 32-bit stamp
    break; case __LINE__: triceM     ( 7009,     "msg:value=%d\n", -__LINE__  ); // trice tool modified code for compilation, no stamp
    break; case __LINE__: TriceM     ( 7009,     "msg:value=%d\n", -__LINE__  ); // trice tool modified code for compilation, 16-bit stamp
    break; case __LINE__: TRiceM     ( 7009,     "msg:value=%d\n", -__LINE__  ); // trice tool modified code for compilation, 32-bit stamp

    break; case __LINE__: trice_1    (           "msg:value=%d\n", -__LINE__  ); // user written code, empty macro for patching, function call, no stamp
    break; case __LINE__: Trice_1    (           "msg:value=%d\n", -__LINE__  ); // user written code, empty macro for patching, function call, 16-bit stamp
    break; case __LINE__: TRice_1    (           "msg:value=%d\n", -__LINE__  ); // user written code, empty macro for patching, function call, 32-bit stamp
    break; case __LINE__: triceM_1   ( 7009,     "msg:value=%d\n", -__LINE__  ); // trice tool modified code for compilation, no stamp
    break; case __LINE__: TriceM_1   ( 7009,     "msg:value=%d\n", -__LINE__  ); // trice tool modified code for compilation, 16-bit stamp
    break; case __LINE__: TRiceM_1   ( 7009,     "msg:value=%d\n", -__LINE__  ); // trice tool modified code for compilation, 32-bit stamp

    // use cases for 8-bit data
    break; case __LINE__: trice8     (           "msg:value=%d\n", -__LINE__  ); // user written code, empty macro for patching, function call, no stamp
    break; case __LINE__: Trice8     (           "msg:value=%d\n", -__LINE__  ); // user written code, empty macro for patching, function call, 16-bit stamp
    break; case __LINE__: TRice8     (           "msg:value=%d\n", -__LINE__  ); // user written code, empty macro for patching, function call, 32-bit stamp
    break; case __LINE__: trice8M    ( 7009,     "msg:value=%d\n", -__LINE__  ); // trice tool modified code for compilation, no stamp
    break; case __LINE__: Trice8M    ( 7009,     "msg:value=%d\n", -__LINE__  ); // trice tool modified code for compilation, 16-bit stamp
    break; case __LINE__: TRice8M    ( 7009,     "msg:value=%d\n", -__LINE__  ); // trice tool modified code for compilation, 32-bit stamp

    break; case __LINE__: trice8_1   (           "msg:value=%d\n", -__LINE__  ); // user written code, empty macro for patching, function call, no stamp
    break; case __LINE__: Trice8_1   (           "msg:value=%d\n", -__LINE__  ); // user written code, empty macro for patching, function call, 16-bit stamp
    break; case __LINE__: TRice8_1   (           "msg:value=%d\n", -__LINE__  ); // user written code, empty macro for patching, function call, 32-bit stamp
    break; case __LINE__: trice8M_1  ( 7009,     "msg:value=%d\n", -__LINE__  ); // trice tool modified code for compilation, no stamp
    break; case __LINE__: Trice8M_1  ( 7009,     "msg:value=%d\n", -__LINE__  ); // trice tool modified code for compilation, 16-bit stamp
    break; case __LINE__: TRice8M_1  ( 7009,     "msg:value=%d\n", -__LINE__  ); // trice tool modified code for compilation, 32-bit stamp

    // use cases for 16-bit data
    break; case __LINE__: trice16    (           "msg:value=%d\n", -__LINE__  ); // user written code, empty macro for patching, function call, no stamp
    break; case __LINE__: Trice16    (           "msg:value=%d\n", -__LINE__  ); // user written code, empty macro for patching, function call, 16-bit stamp
    break; case __LINE__: TRice16    (           "msg:value=%d\n", -__LINE__  ); // user written code, empty macro for patching, function call, 32-bit stamp
    break; case __LINE__: trice16M   ( 7009,     "msg:value=%d\n", -__LINE__  ); // trice tool modified code for compilation, no stamp
    break; case __LINE__: Trice16M   ( 7009,     "msg:value=%d\n", -__LINE__  ); // trice tool modified code for compilation, 16-bit stamp
    break; case __LINE__: TRice16M   ( 7009,     "msg:value=%d\n", -__LINE__  ); // trice tool modified code for compilation, 32-bit stamp

    break; case __LINE__: trice16_1  (           "msg:value=%d\n", -__LINE__  ); // user written code, empty macro for patching, function call, no stamp
    break; case __LINE__: Trice16_1  (           "msg:value=%d\n", -__LINE__  ); // user written code, empty macro for patching, function call, 16-bit stamp
    break; case __LINE__: TRice16_1  (           "msg:value=%d\n", -__LINE__  ); // user written code, empty macro for patching, function call, 32-bit stamp
    break; case __LINE__: trice16M_1 ( 7009,     "msg:value=%d\n", -__LINE__  ); // trice tool modified code for compilation, no stamp
    break; case __LINE__: Trice16M_1 ( 7009,     "msg:value=%d\n", -__LINE__  ); // trice tool modified code for compilation, 16-bit stamp
    break; case __LINE__: TRice16M_1 ( 7009,     "msg:value=%d\n", -__LINE__  ); // trice tool modified code for compilation, 32-bit stamp

    // use cases for 32-bit data
    break; case __LINE__: trice32    (           "msg:value=%d\n", -__LINE__  ); // user written code, empty macro for patching, function call, no stamp
    break; case __LINE__: Trice32    (           "msg:value=%d\n", -__LINE__  ); // user written code, empty macro for patching, function call, 16-bit stamp
    break; case __LINE__: TRice32    (           "msg:value=%d\n", -__LINE__  ); // user written code, empty macro for patching, function call, 32-bit stamp
    break; case __LINE__: trice32M   ( 7009,     "msg:value=%d\n", -__LINE__  ); // trice tool modified code for compilation, no stamp
    break; case __LINE__: Trice32M   ( 7009,     "msg:value=%d\n", -__LINE__  ); // trice tool modified code for compilation, 16-bit stamp
    break; case __LINE__: TRice32M   ( 7009,     "msg:value=%d\n", -__LINE__  ); // trice tool modified code for compilation, 32-bit stamp

    break; case __LINE__: trice32_1  (           "msg:value=%d\n", -__LINE__  ); // user written code, empty macro for patching, function call, no stamp
    break; case __LINE__: Trice32_1  (           "msg:value=%d\n", -__LINE__  ); // user written code, empty macro for patching, function call, 16-bit stamp
    break; case __LINE__: TRice32_1  (           "msg:value=%d\n", -__LINE__  ); // user written code, empty macro for patching, function call, 32-bit stamp
    break; case __LINE__: trice32M_1 ( 7009,     "msg:value=%d\n", -__LINE__  ); // trice tool modified code for compilation, no stamp
    break; case __LINE__: Trice32M_1 ( 7009,     "msg:value=%d\n", -__LINE__  ); // trice tool modified code for compilation, 16-bit stamp
    break; case __LINE__: TRice32M_1 ( 7009,     "msg:value=%d\n", -__LINE__  ); // trice tool modified code for compilation, 32-bit stamp

    // use cases for 64-bit data
    break; case __LINE__: trice64    (           "msg:value=%d\n", -__LINE__  ); // user written code, empty macro for patching, function call, no stamp
    break; case __LINE__: Trice64    (           "msg:value=%d\n", -__LINE__  ); // user written code, empty macro for patching, function call, 16-bit stamp
    break; case __LINE__: TRice64    (           "msg:value=%d\n", -__LINE__  ); // user written code, empty macro for patching, function call, 32-bit stamp
    break; case __LINE__: trice64M   ( 7009,     "msg:value=%d\n", -__LINE__  ); // trice tool modified code for compilation, no stamp
    break; case __LINE__: Trice64M   ( 7009,     "msg:value=%d\n", -__LINE__  ); // trice tool modified code for compilation, 16-bit stamp
    break; case __LINE__: TRice64M   ( 7009,     "msg:value=%d\n", -__LINE__  ); // trice tool modified code for compilation, 32-bit stamp

    break; case __LINE__: trice64_1  (           "msg:value=%d\n", -__LINE__  ); // user written code, empty macro for patching, function call, no stamp
    break; case __LINE__: Trice64_1  (           "msg:value=%d\n", -__LINE__  ); // user written code, empty macro for patching, function call, 16-bit stamp
    break; case __LINE__: TRice64_1  (           "msg:value=%d\n", -__LINE__  ); // user written code, empty macro for patching, function call, 32-bit stamp
    break; case __LINE__: trice64M_1 ( 7009,     "msg:value=%d\n", -__LINE__  ); // trice tool modified code for compilation, no stamp
    break; case __LINE__: Trice64M_1 ( 7009,     "msg:value=%d\n", -__LINE__  ); // trice tool modified code for compilation, 16-bit stamp
    break; case __LINE__: TRice64M_1 ( 7009,     "msg:value=%d\n", -__LINE__  ); // trice tool modified code for compilation, 32-bit stamp

    // legacy use cases
  //break; case __LINE__: TRICE      (           "msg:value=%d\n", -__LINE__  ); // user written code, empty macro for patching, inlined code behind, default stamp
  //break; case __LINE__: TRICE      ( Id(0),    "msg:value=%d\n", -__LINE__  ); // user written code, empty macro for patching, inlined code behind, no stamp
  //break; case __LINE__: TRICE      ( Id(0),    "msg:value=%d\n", -__LINE__  ); // user written code, empty macro for patching, inlined code behind, 16-bit stamp
  //break; case __LINE__: TRICE      ( Id(0),    "msg:value=%d\n", -__LINE__  ); // user written code, empty macro for patching, inlined code behind, 32-bit stamp
    break; case __LINE__: TRICE      ( Id(0), "msg:value=%d\n", -__LINE__  ); // trice tool modified code for compilation, empty macro for patching, inlined code behind, no stamp
    break; case __LINE__: TRICE      ( Id(0), "msg:value=%d\n", -__LINE__  ); // trice tool modified code for compilation, empty macro for patching, inlined code behind, 16-bit stamp
    break; case __LINE__: TRICE      ( Id(0), "msg:value=%d\n", -__LINE__  ); // trice tool modified code for compilation, empty macro for patching, inlined code behind, 32-bit stamp

    // no stamp:
    break; case __LINE__: trice8     (           "msg:value=%d\n", -__LINE__ ); // user written code, empty macro for patching, no stamp
    break; case __LINE__: trice8M    ( 7009,     "msg:value=%d\n", -__LINE__ ); // trice tool modified code for compilation, calls internally fuction trice8fn_1, no stamp
    break; case __LINE__: trice8_1   (           "msg:value=%d\n", -__LINE__ ); // user written code, empty macro for patching
    break; case __LINE__: trice8M_1  ( 7009,     "msg:value=%d\n", -__LINE__ ); // trice tool modified code for compilation, calls internally fuction trice8fn_1
    break; case __LINE__: trice8fn_1 ( 7009,              (uint8_t)-__LINE__ ); // This is a direct function call, which works, but is not inteded for user code.

  //break; case __LINE__: TRICE8     (           "msg:value=%d\n", -__LINE__ ); // legacy with default stamp
  //break; case __LINE__: TRICE8_1   (           "msg:value=%d\n", -__LINE__ ); // legacy with default stamp
  //break; case __LINE__: TRICE8     ( Id(0),    "msg:value=%d\n", -__LINE__ ); // user written code without stamp
  //break; case __LINE__: TRICE8_1   ( Id(0),    "msg:value=%d\n", -__LINE__ ); // user written code without stamp
    break; case __LINE__: TRICE8     ( Id(0), "msg:value=%d\n", -__LINE__ ); // trice tool modified code for compilation, inlines code, no stamp
    break; case __LINE__: TRICE8_1   ( Id(0), "msg:value=%d\n", -__LINE__ ); // trice tool modified code for compilation, inlines code, no stamp
     
    // 16-bit stamp:
    break; case __LINE__: Trice8     (           "msg:value=%d\n", -__LINE__ ); // user written code, empty macro for patching, 16-bit stamp
    break; case __LINE__: Trice8M    ( 7009,     "msg:value=%d\n", -__LINE__ ); // trice tool modified code for compilation, calls internally fuction Trice8fn_1, 16-bit stamp
    break; case __LINE__: Trice8_1   (           "msg:value=%d\n", -__LINE__ ); // user written code, empty macro for patching
    break; case __LINE__: Trice8M_1  ( 7009,     "msg:value=%d\n", -__LINE__ ); // trice tool modified code for compilation, calls internally fuction Trice8fn_1
    break; case __LINE__: Trice8fn_1 ( 7009,              (uint8_t)-__LINE__ ); // This is a direct function call, which works, but is not inteded for user code.

  //break; case __LINE__: TRICE8     (           "msg:value=%d\n", -__LINE__ ); // legacy with default stamp
  //break; case __LINE__: TRICE8_1   (           "msg:value=%d\n", -__LINE__ ); // legacy with default stamp
  //break; case __LINE__: TRICE8     ( Id(0),    "msg:value=%d\n", -__LINE__ ); // user written code with 16-bit stamp
  //break; case __LINE__: TRICE8_1   ( Id(0),    "msg:value=%d\n", -__LINE__ ); // user written code with 16-bit stamp
    break; case __LINE__: TRICE8     ( Id(0), "msg:value=%d\n", -__LINE__ ); // trice tool modified code for compilation, inlines code, with 16-bit stamp
    break; case __LINE__: TRICE8_1   ( Id(0), "msg:value=%d\n", -__LINE__ ); // trice tool modified code for compilation, inlines code, with 16-bit stamp
     
    // 32-bit stamp:
    break; case __LINE__: TRice8     (           "msg:value=%d\n", -__LINE__ ); // user written code, empty macro for patching, 32-bit stamp
    break; case __LINE__: TRice8M    ( 7009,     "msg:value=%d\n", -__LINE__ ); // trice tool modified code for compilation, calls internally fuction TRice8fn_1, 32-bit stamp
    break; case __LINE__: TRice8_1   (           "msg:value=%d\n", -__LINE__ ); // user written code, empty macro for patching
    break; case __LINE__: TRice8M_1  ( 7009,     "msg:value=%d\n", -__LINE__ ); // trice tool modified code for compilation, calls internally fuction TRice8fn_1
    break; case __LINE__: TRice8fn_1 ( 7009,              (uint8_t)-__LINE__ ); // This is a direct function call, which works, but not inteded for user code.

  //break; case __LINE__: TRICE8     (           "msg:value=%d\n", -__LINE__ ); // legacy default with default stamp
  //break; case __LINE__: TRICE8_1   (           "msg:value=%d\n", -__LINE__ ); // legacy default with default stamp
  //break; case __LINE__: TRICE8     ( Id(0),    "msg:value=%d\n", -__LINE__ ); // user written code with 32-bit stamp
  //break; case __LINE__: TRICE8_1   ( Id(0),    "msg:value=%d\n", -__LINE__ ); // user written code with 32-bit stamp
    break; case __LINE__: TRICE8     ( Id(0), "msg:value=%d\n", -__LINE__ ); // trice tool modified code for compilation, inlines code, with 32-bit stamp
    break; case __LINE__: TRICE8_1   ( Id(0), "msg:value=%d\n", -__LINE__ ); // trice tool modified code for compilation, inlines code, with 32-bit stamp

    break; case __LINE__: trice8_1 ( "msg:value=%d\n", -1 );// user written code
    break; case __LINE__: trice8_2 ( "msg:value=%d, %d\n", -1, -2 );// user written code
    break; case __LINE__: trice8_3 ( "msg:value=%d, %d, %d\n", -1, -2, -3 );// user written code
    break; case __LINE__: trice8_4 ( "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );// user written code
    break; case __LINE__: trice8_5 ( "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );// user written code
    break; case __LINE__: trice8_6 ( "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );// user written code
    break; case __LINE__: trice8_7 ( "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );// user written code
    break; case __LINE__: trice8_8 ( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );// user written code
    break; case __LINE__: trice8_9 ( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );// user written code
    break; case __LINE__: trice8_10( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );// user written code
    break; case __LINE__: trice8_11( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );// user written code
    break; case __LINE__: trice8_12( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );// user written code
    
    break; case __LINE__: trice8   ( "msg:value=%d\n", -1 );// user written code
    break; case __LINE__: trice8   ( "msg:value=%d, %d\n", -1, -2 );// user written code
    break; case __LINE__: trice8   ( "msg:value=%d, %d, %d\n", -1, -2, -3 );// user written code
    break; case __LINE__: trice8   ( "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );// user written code
    break; case __LINE__: trice8   ( "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );// user written code
    break; case __LINE__: trice8   ( "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );// user written code
    break; case __LINE__: trice8   ( "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );// user written code
    break; case __LINE__: trice8   ( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );// user written code
    break; case __LINE__: trice8   ( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );// user written code
    break; case __LINE__: trice8   ( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );// user written code
    break; case __LINE__: trice8   ( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );// user written code
    break; case __LINE__: trice8   ( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );// user written code
    
    break; case __LINE__: trice8M_1 ( 7009, "msg:value=%d\n", -1 );// trice tool modified code for compilation
    break; case __LINE__: trice8M_2 ( 5664, "msg:value=%d, %d\n", -1, -2 );// trice tool modified code for compilation
    break; case __LINE__: trice8M_3 ( 2103, "msg:value=%d, %d, %d\n", -1, -2, -3 );// trice tool modified code for compilation
    break; case __LINE__: trice8M_4 ( 7087, "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );// trice tool modified code for compilation
    break; case __LINE__: trice8M_5 ( 4931, "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );// trice tool modified code for compilation
    break; case __LINE__: trice8M_6 ( 2621, "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );// trice tool modified code for compilation
    break; case __LINE__: trice8M_7 ( 3488, "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );// trice tool modified code for compilation
    break; case __LINE__: trice8M_8 ( 4329, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );// trice tool modified code for compilation
    break; case __LINE__: trice8M_9 ( 6469, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );// trice tool modified code for compilation
    break; case __LINE__: trice8M_10( 6335, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );// trice tool modified code for compilation
    break; case __LINE__: trice8M_11( 2410, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );// trice tool modified code for compilation
    break; case __LINE__: trice8M_12( 2070, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );// trice tool modified code for compilation
                                       
    break; case __LINE__: trice8M   ( 2625, "msg:value=%d\n", -1 );// trice tool modified code for compilation
    break; case __LINE__: trice8M   ( 1762, "msg:value=%d, %d\n", -1, -2 );// trice tool modified code for compilation
    break; case __LINE__: trice8M   ( 7960, "msg:value=%d, %d, %d\n", -1, -2, -3 );// trice tool modified code for compilation
    break; case __LINE__: trice8M   ( 1184, "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );// trice tool modified code for compilation
    break; case __LINE__: trice8M   ( 2873, "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );// trice tool modified code for compilation
    break; case __LINE__: trice8M   ( 5861, "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );// trice tool modified code for compilation
    break; case __LINE__: trice8M   ( 5563, "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );// trice tool modified code for compilation
    break; case __LINE__: trice8M   ( 5122, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );// trice tool modified code for compilation
    break; case __LINE__: trice8M   ( 6832, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );// trice tool modified code for compilation
    break; case __LINE__: trice8M   ( 2527, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );// trice tool modified code for compilation
    break; case __LINE__: trice8M   ( 3931, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );// trice tool modified code for compilation
    break; case __LINE__: trice8M   ( 2412, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );// trice tool modified code for compilation
    
    break; case __LINE__: Trice8_1 ( "msg:value=%d\n", -1 );// user written code
    break; case __LINE__: Trice8_2 ( "msg:value=%d, %d\n", -1, -2 );// user written code
    break; case __LINE__: Trice8_3 ( "msg:value=%d, %d, %d\n", -1, -2, -3 );// user written code
    break; case __LINE__: Trice8_4 ( "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );// user written code
    break; case __LINE__: Trice8_5 ( "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );// user written code
    break; case __LINE__: Trice8_6 ( "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );// user written code
    break; case __LINE__: Trice8_7 ( "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );// user written code
    break; case __LINE__: Trice8_8 ( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );// user written code
    break; case __LINE__: Trice8_9 ( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );// user written code
    break; case __LINE__: Trice8_10( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );// user written code
    break; case __LINE__: Trice8_11( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );// user written code
    break; case __LINE__: Trice8_12( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );// user written code
    
    break; case __LINE__: Trice8   ( "msg:value=%d\n", -1 );// user written code
    break; case __LINE__: Trice8   ( "msg:value=%d, %d\n", -1, -2 );// user written code
    break; case __LINE__: Trice8   ( "msg:value=%d, %d, %d\n", -1, -2, -3 );// user written code
    break; case __LINE__: Trice8   ( "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );// user written code
    break; case __LINE__: Trice8   ( "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );// user written code
    break; case __LINE__: Trice8   ( "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );// user written code
    break; case __LINE__: Trice8   ( "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );// user written code
    break; case __LINE__: Trice8   ( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );// user written code
    break; case __LINE__: Trice8   ( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );// user written code
    break; case __LINE__: Trice8   ( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );// user written code
    break; case __LINE__: Trice8   ( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );// user written code
    break; case __LINE__: Trice8   ( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );// user written code
    
    break; case __LINE__: Trice8M_1 ( 7009, "msg:value=%d\n", -1 );// trice tool modified code for compilation
    break; case __LINE__: Trice8M_2 ( 5664, "msg:value=%d, %d\n", -1, -2 );// trice tool modified code for compilation
    break; case __LINE__: Trice8M_3 ( 2103, "msg:value=%d, %d, %d\n", -1, -2, -3 );// trice tool modified code for compilation
    break; case __LINE__: Trice8M_4 ( 7087, "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );// trice tool modified code for compilation
    break; case __LINE__: Trice8M_5 ( 4931, "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );// trice tool modified code for compilation
    break; case __LINE__: Trice8M_6 ( 2621, "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );// trice tool modified code for compilation
    break; case __LINE__: Trice8M_7 ( 3488, "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );// trice tool modified code for compilation
    break; case __LINE__: Trice8M_8 ( 4329, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );// trice tool modified code for compilation
    break; case __LINE__: Trice8M_9 ( 6469, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );// trice tool modified code for compilation
    break; case __LINE__: Trice8M_10( 6335, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );// trice tool modified code for compilation
    break; case __LINE__: Trice8M_11( 2410, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );// trice tool modified code for compilation
    break; case __LINE__: Trice8M_12( 2070, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );// trice tool modified code for compilation
    
    break; case __LINE__: Trice8M   ( 2625, "msg:value=%d\n", -1 );// trice tool modified code for compilation
    break; case __LINE__: Trice8M   ( 1762, "msg:value=%d, %d\n", -1, -2 );// trice tool modified code for compilation
    break; case __LINE__: Trice8M   ( 7960, "msg:value=%d, %d, %d\n", -1, -2, -3 );// trice tool modified code for compilation
    break; case __LINE__: Trice8M   ( 1184, "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );// trice tool modified code for compilation
    break; case __LINE__: Trice8M   ( 2873, "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );// trice tool modified code for compilation
    break; case __LINE__: Trice8M   ( 5861, "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );// trice tool modified code for compilation
    break; case __LINE__: Trice8M   ( 5563, "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );// trice tool modified code for compilation
    break; case __LINE__: Trice8M   ( 5122, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );// trice tool modified code for compilation
    break; case __LINE__: Trice8M   ( 6832, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );// trice tool modified code for compilation
    break; case __LINE__: Trice8M   ( 2527, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );// trice tool modified code for compilation
    break; case __LINE__: Trice8M   ( 3931, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );// trice tool modified code for compilation
    break; case __LINE__: Trice8M   ( 2412, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );// trice tool modified code for compilation
    
    break; case __LINE__: TRice8_1 ( "msg:value=%d\n", -1 );// user written code
    break; case __LINE__: TRice8_2 ( "msg:value=%d, %d\n", -1, -2 );// user written code
    break; case __LINE__: TRice8_3 ( "msg:value=%d, %d, %d\n", -1, -2, -3 );// user written code
    break; case __LINE__: TRice8_4 ( "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );// user written code
    break; case __LINE__: TRice8_5 ( "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );// user written code
    break; case __LINE__: TRice8_6 ( "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );// user written code
    break; case __LINE__: TRice8_7 ( "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );// user written code
    break; case __LINE__: TRice8_8 ( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );// user written code
    break; case __LINE__: TRice8_9 ( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );// user written code
    break; case __LINE__: TRice8_10( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );// user written code
    break; case __LINE__: TRice8_11( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );// user written code
    break; case __LINE__: TRice8_12( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );// user written code
    
    break; case __LINE__: TRice8   ( "msg:value=%d\n", -1 );// user written code
    break; case __LINE__: TRice8   ( "msg:value=%d, %d\n", -1, -2 );// user written code
    break; case __LINE__: TRice8   ( "msg:value=%d, %d, %d\n", -1, -2, -3 );// user written code
    break; case __LINE__: TRice8   ( "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );// user written code
    break; case __LINE__: TRice8   ( "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );// user written code
    break; case __LINE__: TRice8   ( "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );// user written code
    break; case __LINE__: TRice8   ( "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );// user written code
    break; case __LINE__: TRice8   ( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );// user written code
    break; case __LINE__: TRice8   ( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );// user written code
    break; case __LINE__: TRice8   ( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );// user written code
    break; case __LINE__: TRice8   ( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );// user written code
    break; case __LINE__: TRice8   ( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );// user written code
    
    break; case __LINE__: TRice8M_1 ( 7009, "msg:value=%d\n", -1 );// trice tool modified code for compilation
    break; case __LINE__: TRice8M_2 ( 5664, "msg:value=%d, %d\n", -1, -2 );// trice tool modified code for compilation
    break; case __LINE__: TRice8M_3 ( 2103, "msg:value=%d, %d, %d\n", -1, -2, -3 );// trice tool modified code for compilation
    break; case __LINE__: TRice8M_4 ( 7087, "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );// trice tool modified code for compilation
    break; case __LINE__: TRice8M_5 ( 4931, "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );// trice tool modified code for compilation
    break; case __LINE__: TRice8M_6 ( 2621, "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );// trice tool modified code for compilation
    break; case __LINE__: TRice8M_7 ( 3488, "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );// trice tool modified code for compilation
    break; case __LINE__: TRice8M_8 ( 4329, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );// trice tool modified code for compilation
    break; case __LINE__: TRice8M_9 ( 6469, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );// trice tool modified code for compilation
    break; case __LINE__: TRice8M_10( 6335, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );// trice tool modified code for compilation
    break; case __LINE__: TRice8M_11( 2410, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );// trice tool modified code for compilation
    break; case __LINE__: TRice8M_12( 2070, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );// trice tool modified code for compilation
    
    break; case __LINE__: TRice8M   ( 2625, "msg:value=%d\n", -1 );// trice tool modified code for compilation
    break; case __LINE__: TRice8M   ( 1762, "msg:value=%d, %d\n", -1, -2 );// trice tool modified code for compilation
    break; case __LINE__: TRice8M   ( 7960, "msg:value=%d, %d, %d\n", -1, -2, -3 );// trice tool modified code for compilation
    break; case __LINE__: TRice8M   ( 1184, "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );// trice tool modified code for compilation
    break; case __LINE__: TRice8M   ( 2873, "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );// trice tool modified code for compilation
    break; case __LINE__: TRice8M   ( 5861, "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );// trice tool modified code for compilation
    break; case __LINE__: TRice8M   ( 5563, "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );// trice tool modified code for compilation
    break; case __LINE__: TRice8M   ( 5122, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );// trice tool modified code for compilation
    break; case __LINE__: TRice8M   ( 6832, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );// trice tool modified code for compilation
    break; case __LINE__: TRice8M   ( 2527, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );// trice tool modified code for compilation
    break; case __LINE__: TRice8M   ( 3931, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );// trice tool modified code for compilation
    break; case __LINE__: TRice8M   ( 2412, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );// trice tool modified code for compilation

    break; case __LINE__: trice16_1 ( "msg:value=%d\n", -1 );// user written code
    break; case __LINE__: trice16_2 ( "msg:value=%d, %d\n", -1, -2 );// user written code
    break; case __LINE__: trice16_3 ( "msg:value=%d, %d, %d\n", -1, -2, -3 );// user written code
    break; case __LINE__: trice16_4 ( "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );// user written code
    break; case __LINE__: trice16_5 ( "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );// user written code
    break; case __LINE__: trice16_6 ( "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );// user written code
    break; case __LINE__: trice16_7 ( "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );// user written code
    break; case __LINE__: trice16_8 ( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );// user written code
    break; case __LINE__: trice16_9 ( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );// user written code
    break; case __LINE__: trice16_10( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );// user written code
    break; case __LINE__: trice16_11( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );// user written code
    break; case __LINE__: trice16_12( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );// user written code

    break; case __LINE__: trice16( "msg:value=%d\n", -1 );// user written code
    break; case __LINE__: trice16( "msg:value=%d, %d\n", -1, -2 );// user written code
    break; case __LINE__: trice16( "msg:value=%d, %d, %d\n", -1, -2, -3 );// user written code
    break; case __LINE__: trice16( "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );// user written code
    break; case __LINE__: trice16( "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );// user written code
    break; case __LINE__: trice16( "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );// user written code
    break; case __LINE__: trice16( "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );// user written code
    break; case __LINE__: trice16( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );// user written code
    break; case __LINE__: trice16( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );// user written code
    break; case __LINE__: trice16( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );// user written code
    break; case __LINE__: trice16( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );// user written code
    break; case __LINE__: trice16( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );// user written code

    break; case __LINE__: trice16M_1 ( 3421, "msg:value=%d\n", -1 );// trice tool modified code for compilation
    break; case __LINE__: trice16M_2 ( 7538, "msg:value=%d, %d\n", -1, -2 );// trice tool modified code for compilation
    break; case __LINE__: trice16M_3 ( 6976, "msg:value=%d, %d, %d\n", -1, -2, -3 );// trice tool modified code for compilation
    break; case __LINE__: trice16M_4 ( 1249, "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );// trice tool modified code for compilation
    break; case __LINE__: trice16M_5 ( 6240, "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );// trice tool modified code for compilation
    break; case __LINE__: trice16M_6 ( 3738, "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );// trice tool modified code for compilation
    break; case __LINE__: trice16M_7 ( 3372, "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );// trice tool modified code for compilation
    break; case __LINE__: trice16M_8 ( 6060, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );// trice tool modified code for compilation
    break; case __LINE__: trice16M_9 ( 4052, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );// trice tool modified code for compilation
    break; case __LINE__: trice16M_10( 5619, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );// trice tool modified code for compilation
    break; case __LINE__: trice16M_11( 5053, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );// trice tool modified code for compilation
    break; case __LINE__: trice16M_12( 3971, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );// trice tool modified code for compilation

    break; case __LINE__: trice16M   ( 3421, "msg:value=%d\n", -1 );// trice tool modified code for compilation
    break; case __LINE__: trice16M   ( 7538, "msg:value=%d, %d\n", -1, -2 );// trice tool modified code for compilation
    break; case __LINE__: trice16M   ( 6976, "msg:value=%d, %d, %d\n", -1, -2, -3 );// trice tool modified code for compilation
    break; case __LINE__: trice16M   ( 1249, "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );// trice tool modified code for compilation
    break; case __LINE__: trice16M   ( 6240, "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );// trice tool modified code for compilation
    break; case __LINE__: trice16M   ( 3738, "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );// trice tool modified code for compilation
    break; case __LINE__: trice16M   ( 3372, "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );// trice tool modified code for compilation
    break; case __LINE__: trice16M   ( 6060, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );// trice tool modified code for compilation
    break; case __LINE__: trice16M   ( 4052, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );// trice tool modified code for compilation
    break; case __LINE__: trice16M   ( 5619, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );// trice tool modified code for compilation
    break; case __LINE__: trice16M   ( 5053, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );// trice tool modified code for compilation
    break; case __LINE__: trice16M   ( 3971, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );// trice tool modified code for compilation

    break; case __LINE__: Trice16_1 ( "msg:value=%d\n", -1 );// user written code
    break; case __LINE__: Trice16_2 ( "msg:value=%d, %d\n", -1, -2 );// user written code
    break; case __LINE__: Trice16_3 ( "msg:value=%d, %d, %d\n", -1, -2, -3 );// user written code
    break; case __LINE__: Trice16_4 ( "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );// user written code
    break; case __LINE__: Trice16_5 ( "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );// user written code
    break; case __LINE__: Trice16_6 ( "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );// user written code
    break; case __LINE__: Trice16_7 ( "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );// user written code
    break; case __LINE__: Trice16_8 ( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );// user written code
    break; case __LINE__: Trice16_9 ( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );// user written code
    break; case __LINE__: Trice16_10( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );// user written code
    break; case __LINE__: Trice16_11( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );// user written code
    break; case __LINE__: Trice16_12( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );// user written code

    break; case __LINE__: Trice16( "msg:value=%d\n", -1 );// user written code
    break; case __LINE__: Trice16( "msg:value=%d, %d\n", -1, -2 );// user written code
    break; case __LINE__: Trice16( "msg:value=%d, %d, %d\n", -1, -2, -3 );// user written code
    break; case __LINE__: Trice16( "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );// user written code
    break; case __LINE__: Trice16( "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );// user written code
    break; case __LINE__: Trice16( "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );// user written code
    break; case __LINE__: Trice16( "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );// user written code
    break; case __LINE__: Trice16( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );// user written code
    break; case __LINE__: Trice16( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );// user written code
    break; case __LINE__: Trice16( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );// user written code
    break; case __LINE__: Trice16( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );// user written code
    break; case __LINE__: Trice16( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );// user written code
    
    break; case __LINE__: Trice16M_1 ( 3421, "msg:value=%d\n", -1 );// trice tool modified code for compilation
    break; case __LINE__: Trice16M_2 ( 7538, "msg:value=%d, %d\n", -1, -2 );// trice tool modified code for compilation
    break; case __LINE__: Trice16M_3 ( 6976, "msg:value=%d, %d, %d\n", -1, -2, -3 );// trice tool modified code for compilation
    break; case __LINE__: Trice16M_4 ( 1249, "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );// trice tool modified code for compilation
    break; case __LINE__: Trice16M_5 ( 6240, "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );// trice tool modified code for compilation
    break; case __LINE__: Trice16M_6 ( 3738, "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );// trice tool modified code for compilation
    break; case __LINE__: Trice16M_7 ( 3372, "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );// trice tool modified code for compilation
    break; case __LINE__: Trice16M_8 ( 6060, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );// trice tool modified code for compilation
    break; case __LINE__: Trice16M_9 ( 4052, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );// trice tool modified code for compilation
    break; case __LINE__: Trice16M_10( 5619, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );// trice tool modified code for compilation
    break; case __LINE__: Trice16M_11( 5053, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );// trice tool modified code for compilation
    break; case __LINE__: Trice16M_12( 3971, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );// trice tool modified code for compilation

    break; case __LINE__: Trice16M   ( 3421, "msg:value=%d\n", -1 );// trice tool modified code for compilation
    break; case __LINE__: Trice16M   ( 7538, "msg:value=%d, %d\n", -1, -2 );// trice tool modified code for compilation
    break; case __LINE__: Trice16M   ( 6976, "msg:value=%d, %d, %d\n", -1, -2, -3 );// trice tool modified code for compilation
    break; case __LINE__: Trice16M   ( 1249, "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );// trice tool modified code for compilation
    break; case __LINE__: Trice16M   ( 6240, "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );// trice tool modified code for compilation
    break; case __LINE__: Trice16M   ( 3738, "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );// trice tool modified code for compilation
    break; case __LINE__: Trice16M   ( 3372, "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );// trice tool modified code for compilation
    break; case __LINE__: Trice16M   ( 6060, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );// trice tool modified code for compilation
    break; case __LINE__: Trice16M   ( 4052, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );// trice tool modified code for compilation
    break; case __LINE__: Trice16M   ( 5619, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );// trice tool modified code for compilation
    break; case __LINE__: Trice16M   ( 5053, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );// trice tool modified code for compilation
    break; case __LINE__: Trice16M   ( 3971, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );// trice tool modified code for compilation

    break; case __LINE__: TRice16_1 ( "msg:value=%d\n", -1 );// user written code
    break; case __LINE__: TRice16_2 ( "msg:value=%d, %d\n", -1, -2 );// user written code
    break; case __LINE__: TRice16_3 ( "msg:value=%d, %d, %d\n", -1, -2, -3 );// user written code
    break; case __LINE__: TRice16_4 ( "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );// user written code
    break; case __LINE__: TRice16_5 ( "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );// user written code
    break; case __LINE__: TRice16_6 ( "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );// user written code
    break; case __LINE__: TRice16_7 ( "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );// user written code
    break; case __LINE__: TRice16_8 ( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );// user written code
    break; case __LINE__: TRice16_9 ( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );// user written code
    break; case __LINE__: TRice16_10( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );// user written code
    break; case __LINE__: TRice16_11( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );// user written code
    break; case __LINE__: TRice16_12( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );// user written code

    break; case __LINE__: TRice16( "msg:value=%d\n", -1 );// user written code
    break; case __LINE__: TRice16( "msg:value=%d, %d\n", -1, -2 );// user written code
    break; case __LINE__: TRice16( "msg:value=%d, %d, %d\n", -1, -2, -3 );// user written code
    break; case __LINE__: TRice16( "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );// user written code
    break; case __LINE__: TRice16( "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );// user written code
    break; case __LINE__: TRice16( "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );// user written code
    break; case __LINE__: TRice16( "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );// user written code
    break; case __LINE__: TRice16( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );// user written code
    break; case __LINE__: TRice16( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );// user written code
    break; case __LINE__: TRice16( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );// user written code
    break; case __LINE__: TRice16( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );// user written code
    break; case __LINE__: TRice16( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );// user written code

    break; case __LINE__: TRice16M_1 ( 3421, "msg:value=%d\n", -1 );// trice tool modified code for compilation
    break; case __LINE__: TRice16M_2 ( 7538, "msg:value=%d, %d\n", -1, -2 );// trice tool modified code for compilation
    break; case __LINE__: TRice16M_3 ( 6976, "msg:value=%d, %d, %d\n", -1, -2, -3 );// trice tool modified code for compilation
    break; case __LINE__: TRice16M_4 ( 1249, "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );// trice tool modified code for compilation
    break; case __LINE__: TRice16M_5 ( 6240, "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );// trice tool modified code for compilation
    break; case __LINE__: TRice16M_6 ( 3738, "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );// trice tool modified code for compilation
    break; case __LINE__: TRice16M_7 ( 3372, "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );// trice tool modified code for compilation
    break; case __LINE__: TRice16M_8 ( 6060, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );// trice tool modified code for compilation
    break; case __LINE__: TRice16M_9 ( 4052, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );// trice tool modified code for compilation
    break; case __LINE__: TRice16M_10( 5619, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );// trice tool modified code for compilation
    break; case __LINE__: TRice16M_11( 5053, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );// trice tool modified code for compilation
    break; case __LINE__: TRice16M_12( 3971, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );// trice tool modified code for compilation

    break; case __LINE__: TRice16M   ( 3421, "msg:value=%d\n", -1 );// trice tool modified code for compilation
    break; case __LINE__: TRice16M   ( 7538, "msg:value=%d, %d\n", -1, -2 );// trice tool modified code for compilation
    break; case __LINE__: TRice16M   ( 6976, "msg:value=%d, %d, %d\n", -1, -2, -3 );// trice tool modified code for compilation
    break; case __LINE__: TRice16M   ( 1249, "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );// trice tool modified code for compilation
    break; case __LINE__: TRice16M   ( 6240, "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );// trice tool modified code for compilation
    break; case __LINE__: TRice16M   ( 3738, "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );// trice tool modified code for compilation
    break; case __LINE__: TRice16M   ( 3372, "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );// trice tool modified code for compilation
    break; case __LINE__: TRice16M   ( 6060, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );// trice tool modified code for compilation
    break; case __LINE__: TRice16M   ( 4052, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );// trice tool modified code for compilation
    break; case __LINE__: TRice16M   ( 5619, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );// trice tool modified code for compilation
    break; case __LINE__: TRice16M   ( 5053, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );// trice tool modified code for compilation
    break; case __LINE__: TRice16M   ( 3971, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );// trice tool modified code for compilation

    break; case __LINE__: trice32_1 ( "msg:value=%d\n", -1 );// user written code
    break; case __LINE__: trice32_2 ( "msg:value=%d, %d\n", -1, -2 );// user written code
    break; case __LINE__: trice32_3 ( "msg:value=%d, %d, %d\n", -1, -2, -3 );// user written code
    break; case __LINE__: trice32_4 ( "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );// user written code
    break; case __LINE__: trice32_5 ( "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );// user written code
    break; case __LINE__: trice32_6 ( "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );// user written code
    break; case __LINE__: trice32_7 ( "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );// user written code
    break; case __LINE__: trice32_8 ( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );// user written code
    break; case __LINE__: trice32_9 ( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );// user written code
    break; case __LINE__: trice32_10( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );// user written code
    break; case __LINE__: trice32_11( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );// user written code
    break; case __LINE__: trice32_12( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );// user written code
    
    break; case __LINE__: trice32( "msg:value=%d\n", -1 );// user written code
    break; case __LINE__: trice32( "msg:value=%d, %d\n", -1, -2 );// user written code
    break; case __LINE__: trice32( "msg:value=%d, %d, %d\n", -1, -2, -3 );// user written code
    break; case __LINE__: trice32( "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );// user written code
    break; case __LINE__: trice32( "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );// user written code
    break; case __LINE__: trice32( "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );// user written code
    break; case __LINE__: trice32( "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );// user written code
    break; case __LINE__: trice32( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );// user written code
    break; case __LINE__: trice32( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );// user written code
    break; case __LINE__: trice32( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );// user written code
    break; case __LINE__: trice32( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );// user written code
    break; case __LINE__: trice32( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );// user written code

    break; case __LINE__: trice32M_1 ( 3712, "msg:value=%d\n", -1 );// trice tool modified code for compilation
    break; case __LINE__: trice32M_2 ( 4275, "msg:value=%d, %d\n", -1, -2 );// trice tool modified code for compilation
    break; case __LINE__: trice32M_3 ( 1846, "msg:value=%d, %d, %d\n", -1, -2, -3 );// trice tool modified code for compilation
    break; case __LINE__: trice32M_4 ( 7155, "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );// trice tool modified code for compilation
    break; case __LINE__: trice32M_5 ( 2705, "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );// trice tool modified code for compilation
    break; case __LINE__: trice32M_6 ( 3020, "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );// trice tool modified code for compilation
    break; case __LINE__: trice32M_7 ( 3760, "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );// trice tool modified code for compilation
    break; case __LINE__: trice32M_8 ( 1960, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );// trice tool modified code for compilation
    break; case __LINE__: trice32M_9 ( 2025, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );// trice tool modified code for compilation
    break; case __LINE__: trice32M_10( 1559, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );// trice tool modified code for compilation
    break; case __LINE__: trice32M_11( 3457, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );// trice tool modified code for compilation
    break; case __LINE__: trice32M_12( 5347, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );// trice tool modified code for compilation

    break; case __LINE__: trice32M   ( 3712, "msg:value=%d\n", -1 );// trice tool modified code for compilation
    break; case __LINE__: trice32M   ( 4275, "msg:value=%d, %d\n", -1, -2 );// trice tool modified code for compilation
    break; case __LINE__: trice32M   ( 1846, "msg:value=%d, %d, %d\n", -1, -2, -3 );// trice tool modified code for compilation
    break; case __LINE__: trice32M   ( 7155, "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );// trice tool modified code for compilation
    break; case __LINE__: trice32M   ( 2705, "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );// trice tool modified code for compilation
    break; case __LINE__: trice32M   ( 3020, "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );// trice tool modified code for compilation
    break; case __LINE__: trice32M   ( 3760, "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );// trice tool modified code for compilation
    break; case __LINE__: trice32M   ( 1960, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );// trice tool modified code for compilation
    break; case __LINE__: trice32M   ( 2025, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );// trice tool modified code for compilation
    break; case __LINE__: trice32M   ( 1559, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );// trice tool modified code for compilation
    break; case __LINE__: trice32M   ( 3457, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );// trice tool modified code for compilation
    break; case __LINE__: trice32M   ( 5347, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );// trice tool modified code for compilation

    break; case __LINE__: Trice32_1 ( "msg:value=%d\n", -1 );// user written code
    break; case __LINE__: Trice32_2 ( "msg:value=%d, %d\n", -1, -2 );// user written code
    break; case __LINE__: Trice32_3 ( "msg:value=%d, %d, %d\n", -1, -2, -3 );// user written code
    break; case __LINE__: Trice32_4 ( "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );// user written code
    break; case __LINE__: Trice32_5 ( "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );// user written code
    break; case __LINE__: Trice32_6 ( "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );// user written code
    break; case __LINE__: Trice32_7 ( "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );// user written code
    break; case __LINE__: Trice32_8 ( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );// user written code
    break; case __LINE__: Trice32_9 ( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );// user written code
    break; case __LINE__: Trice32_10( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );// user written code
    break; case __LINE__: Trice32_11( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );// user written code
    break; case __LINE__: Trice32_12( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );// user written code

    break; case __LINE__: Trice32( "msg:value=%d\n", -1 );// user written code
    break; case __LINE__: Trice32( "msg:value=%d, %d\n", -1, -2 );// user written code
    break; case __LINE__: Trice32( "msg:value=%d, %d, %d\n", -1, -2, -3 );// user written code
    break; case __LINE__: Trice32( "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );// user written code
    break; case __LINE__: Trice32( "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );// user written code
    break; case __LINE__: Trice32( "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );// user written code
    break; case __LINE__: Trice32( "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );// user written code
    break; case __LINE__: Trice32( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );// user written code
    break; case __LINE__: Trice32( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );// user written code
    break; case __LINE__: Trice32( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );// user written code
    break; case __LINE__: Trice32( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );// user written code
    break; case __LINE__: Trice32( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );// user written code

    break; case __LINE__: Trice32M_1 ( 3712, "msg:value=%d\n", -1 );// trice tool modified code for compilation
    break; case __LINE__: Trice32M_2 ( 4275, "msg:value=%d, %d\n", -1, -2 );// trice tool modified code for compilation
    break; case __LINE__: Trice32M_3 ( 1846, "msg:value=%d, %d, %d\n", -1, -2, -3 );// trice tool modified code for compilation
    break; case __LINE__: Trice32M_4 ( 7155, "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );// trice tool modified code for compilation
    break; case __LINE__: Trice32M_5 ( 2705, "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );// trice tool modified code for compilation
    break; case __LINE__: Trice32M_6 ( 3020, "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );// trice tool modified code for compilation
    break; case __LINE__: Trice32M_7 ( 3760, "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );// trice tool modified code for compilation
    break; case __LINE__: Trice32M_8 ( 1960, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );// trice tool modified code for compilation
    break; case __LINE__: Trice32M_9 ( 2025, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );// trice tool modified code for compilation
    break; case __LINE__: Trice32M_10( 1559, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );// trice tool modified code for compilation
    break; case __LINE__: Trice32M_11( 3457, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );// trice tool modified code for compilation
    break; case __LINE__: Trice32M_12( 5347, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );// trice tool modified code for compilation

    break; case __LINE__: Trice32M   ( 3712, "msg:value=%d\n", -1 );// trice tool modified code for compilation
    break; case __LINE__: Trice32M   ( 4275, "msg:value=%d, %d\n", -1, -2 );// trice tool modified code for compilation
    break; case __LINE__: Trice32M   ( 1846, "msg:value=%d, %d, %d\n", -1, -2, -3 );// trice tool modified code for compilation
    break; case __LINE__: Trice32M   ( 7155, "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );// trice tool modified code for compilation
    break; case __LINE__: Trice32M   ( 2705, "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );// trice tool modified code for compilation
    break; case __LINE__: Trice32M   ( 3020, "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );// trice tool modified code for compilation
    break; case __LINE__: Trice32M   ( 3760, "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );// trice tool modified code for compilation
    break; case __LINE__: Trice32M   ( 1960, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );// trice tool modified code for compilation
    break; case __LINE__: Trice32M   ( 2025, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );// trice tool modified code for compilation
    break; case __LINE__: Trice32M   ( 1559, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );// trice tool modified code for compilation
    break; case __LINE__: Trice32M   ( 3457, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );// trice tool modified code for compilation
    break; case __LINE__: Trice32M   ( 5347, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );// trice tool modified code for compilation

    break; case __LINE__: TRice32_1 ( "msg:value=%d\n", -1 );// user written code
    break; case __LINE__: TRice32_2 ( "msg:value=%d, %d\n", -1, -2 );// user written code
    break; case __LINE__: TRice32_3 ( "msg:value=%d, %d, %d\n", -1, -2, -3 );// user written code
    break; case __LINE__: TRice32_4 ( "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );// user written code
    break; case __LINE__: TRice32_5 ( "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );// user written code
    break; case __LINE__: TRice32_6 ( "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );// user written code
    break; case __LINE__: TRice32_7 ( "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );// user written code
    break; case __LINE__: TRice32_8 ( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );// user written code
    break; case __LINE__: TRice32_9 ( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );// user written code
    break; case __LINE__: TRice32_10( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );// user written code
    break; case __LINE__: TRice32_11( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );// user written code
    break; case __LINE__: TRice32_12( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );// user written code
    
    break; case __LINE__: TRice32( "msg:value=%d\n", -1 );// user written code
    break; case __LINE__: TRice32( "msg:value=%d, %d\n", -1, -2 );// user written code
    break; case __LINE__: TRice32( "msg:value=%d, %d, %d\n", -1, -2, -3 );// user written code
    break; case __LINE__: TRice32( "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );// user written code
    break; case __LINE__: TRice32( "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );// user written code
    break; case __LINE__: TRice32( "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );// user written code
    break; case __LINE__: TRice32( "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );// user written code
    break; case __LINE__: TRice32( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );// user written code
    break; case __LINE__: TRice32( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );// user written code
    break; case __LINE__: TRice32( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );// user written code
    break; case __LINE__: TRice32( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );// user written code
    break; case __LINE__: TRice32( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );// user written code

    break; case __LINE__: TRice32M_1 ( 3712, "msg:value=%d\n", -1 );// trice tool modified code for compilation
    break; case __LINE__: TRice32M_2 ( 4275, "msg:value=%d, %d\n", -1, -2 );// trice tool modified code for compilation
    break; case __LINE__: TRice32M_3 ( 1846, "msg:value=%d, %d, %d\n", -1, -2, -3 );// trice tool modified code for compilation
    break; case __LINE__: TRice32M_4 ( 7155, "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );// trice tool modified code for compilation
    break; case __LINE__: TRice32M_5 ( 2705, "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );// trice tool modified code for compilation
    break; case __LINE__: TRice32M_6 ( 3020, "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );// trice tool modified code for compilation
    break; case __LINE__: TRice32M_7 ( 3760, "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );// trice tool modified code for compilation
    break; case __LINE__: TRice32M_8 ( 1960, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );// trice tool modified code for compilation
    break; case __LINE__: TRice32M_9 ( 2025, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );// trice tool modified code for compilation
    break; case __LINE__: TRice32M_10( 1559, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );// trice tool modified code for compilation
    break; case __LINE__: TRice32M_11( 3457, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );// trice tool modified code for compilation
    break; case __LINE__: TRice32M_12( 5347, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );// trice tool modified code for compilation

    break; case __LINE__: TRice32M   ( 3712, "msg:value=%d\n", -1 );// trice tool modified code for compilation
    break; case __LINE__: TRice32M   ( 4275, "msg:value=%d, %d\n", -1, -2 );// trice tool modified code for compilation
    break; case __LINE__: TRice32M   ( 1846, "msg:value=%d, %d, %d\n", -1, -2, -3 );// trice tool modified code for compilation
    break; case __LINE__: TRice32M   ( 7155, "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );// trice tool modified code for compilation
    break; case __LINE__: TRice32M   ( 2705, "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );// trice tool modified code for compilation
    break; case __LINE__: TRice32M   ( 3020, "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );// trice tool modified code for compilation
    break; case __LINE__: TRice32M   ( 3760, "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );// trice tool modified code for compilation
    break; case __LINE__: TRice32M   ( 1960, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );// trice tool modified code for compilation
    break; case __LINE__: TRice32M   ( 2025, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );// trice tool modified code for compilation
    break; case __LINE__: TRice32M   ( 1559, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );// trice tool modified code for compilation
    break; case __LINE__: TRice32M   ( 3457, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );// trice tool modified code for compilation
    break; case __LINE__: TRice32M   ( 5347, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );// trice tool modified code for compilation

    break; case __LINE__: trice64_1 ( "msg:value=%d\n", -1 );// user written code
    break; case __LINE__: trice64_2 ( "msg:value=%d, %d\n", -1, -2 );// user written code
    break; case __LINE__: trice64_3 ( "msg:value=%d, %d, %d\n", -1, -2, -3 );// user written code
    break; case __LINE__: trice64_4 ( "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );// user written code
    break; case __LINE__: trice64_5 ( "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );// user written code
    break; case __LINE__: trice64_6 ( "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );// user written code
    break; case __LINE__: trice64_7 ( "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );// user written code
    break; case __LINE__: trice64_8 ( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );// user written code
    break; case __LINE__: trice64_9 ( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );// user written code
    break; case __LINE__: trice64_10( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );// user written code
    break; case __LINE__: trice64_11( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );// user written code
    break; case __LINE__: trice64_12( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );// user written code

    break; case __LINE__: trice64( "msg:value=%d\n", -1 );// user written code
    break; case __LINE__: trice64( "msg:value=%d, %d\n", -1, -2 );// user written code
    break; case __LINE__: trice64( "msg:value=%d, %d, %d\n", -1, -2, -3 );// user written code
    break; case __LINE__: trice64( "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );// user written code
    break; case __LINE__: trice64( "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );// user written code
    break; case __LINE__: trice64( "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );// user written code
    break; case __LINE__: trice64( "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );// user written code
    break; case __LINE__: trice64( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );// user written code
    break; case __LINE__: trice64( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );// user written code
    break; case __LINE__: trice64( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );// user written code
    break; case __LINE__: trice64( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );// user written code
    break; case __LINE__: trice64( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );// user written code

    break; case __LINE__: trice64M_1 ( 2563, "msg:value=%d\n", -1 );// trice tool modified code for compilation
    break; case __LINE__: trice64M_2 ( 4590, "msg:value=%d, %d\n", -1, -2 );// trice tool modified code for compilation
    break; case __LINE__: trice64M_3 ( 1279, "msg:value=%d, %d, %d\n", -1, -2, -3 );// trice tool modified code for compilation
    break; case __LINE__: trice64M_4 ( 7030, "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );// trice tool modified code for compilation
    break; case __LINE__: trice64M_5 ( 6420, "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );// trice tool modified code for compilation
    break; case __LINE__: trice64M_6 ( 5834, "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );// trice tool modified code for compilation
    break; case __LINE__: trice64M_7 ( 1307, "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );// trice tool modified code for compilation
    break; case __LINE__: trice64M_8 ( 1025, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );// trice tool modified code for compilation
    break; case __LINE__: trice64M_9 ( 4086, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );// trice tool modified code for compilation
    break; case __LINE__: trice64M_10( 4476, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );// trice tool modified code for compilation
    break; case __LINE__: trice64M_11( 1609, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );// trice tool modified code for compilation
    break; case __LINE__: trice64M_12( 3071, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );// trice tool modified code for compilation

    break; case __LINE__: trice64M   ( 2563, "msg:value=%d\n", -1 );// trice tool modified code for compilation
    break; case __LINE__: trice64M   ( 4590, "msg:value=%d, %d\n", -1, -2 );// trice tool modified code for compilation
    break; case __LINE__: trice64M   ( 1279, "msg:value=%d, %d, %d\n", -1, -2, -3 );// trice tool modified code for compilation
    break; case __LINE__: trice64M   ( 7030, "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );// trice tool modified code for compilation
    break; case __LINE__: trice64M   ( 6420, "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );// trice tool modified code for compilation
    break; case __LINE__: trice64M   ( 5834, "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );// trice tool modified code for compilation
    break; case __LINE__: trice64M   ( 1307, "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );// trice tool modified code for compilation
    break; case __LINE__: trice64M   ( 1025, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );// trice tool modified code for compilation
    break; case __LINE__: trice64M   ( 4086, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );// trice tool modified code for compilation
    break; case __LINE__: trice64M   ( 4476, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );// trice tool modified code for compilation
    break; case __LINE__: trice64M   ( 1609, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );// trice tool modified code for compilation
    break; case __LINE__: trice64M   ( 3071, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );// trice tool modified code for compilation

    break; case __LINE__: Trice64_1 ( "msg:value=%d\n", -1 );// user written code
    break; case __LINE__: Trice64_2 ( "msg:value=%d, %d\n", -1, -2 );// user written code
    break; case __LINE__: Trice64_3 ( "msg:value=%d, %d, %d\n", -1, -2, -3 );// user written code
    break; case __LINE__: Trice64_4 ( "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );// user written code
    break; case __LINE__: Trice64_5 ( "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );// user written code
    break; case __LINE__: Trice64_6 ( "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );// user written code
    break; case __LINE__: Trice64_7 ( "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );// user written code
    break; case __LINE__: Trice64_8 ( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );// user written code
    break; case __LINE__: Trice64_9 ( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );// user written code
    break; case __LINE__: Trice64_10( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );// user written code
    break; case __LINE__: Trice64_11( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );// user written code
    break; case __LINE__: Trice64_12( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );// user written code
    
    break; case __LINE__: Trice64( "msg:value=%d\n", -1 );// user written code
    break; case __LINE__: Trice64( "msg:value=%d, %d\n", -1, -2 );// user written code
    break; case __LINE__: Trice64( "msg:value=%d, %d, %d\n", -1, -2, -3 );// user written code
    break; case __LINE__: Trice64( "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );// user written code
    break; case __LINE__: Trice64( "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );// user written code
    break; case __LINE__: Trice64( "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );// user written code
    break; case __LINE__: Trice64( "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );// user written code
    break; case __LINE__: Trice64( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );// user written code
    break; case __LINE__: Trice64( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );// user written code
    break; case __LINE__: Trice64( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );// user written code
    break; case __LINE__: Trice64( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );// user written code
    break; case __LINE__: Trice64( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );// user written code

    break; case __LINE__: Trice64M_1 ( 2563, "msg:value=%d\n", -1 );// trice tool modified code for compilation
    break; case __LINE__: Trice64M_2 ( 4590, "msg:value=%d, %d\n", -1, -2 );// trice tool modified code for compilation
    break; case __LINE__: Trice64M_3 ( 1279, "msg:value=%d, %d, %d\n", -1, -2, -3 );// trice tool modified code for compilation
    break; case __LINE__: Trice64M_4 ( 7030, "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );// trice tool modified code for compilation
    break; case __LINE__: Trice64M_5 ( 6420, "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );// trice tool modified code for compilation
    break; case __LINE__: Trice64M_6 ( 5834, "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );// trice tool modified code for compilation
    break; case __LINE__: Trice64M_7 ( 1307, "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );// trice tool modified code for compilation
    break; case __LINE__: Trice64M_8 ( 1025, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );// trice tool modified code for compilation
    break; case __LINE__: Trice64M_9 ( 4086, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );// trice tool modified code for compilation
    break; case __LINE__: Trice64M_10( 4476, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );// trice tool modified code for compilation
    break; case __LINE__: Trice64M_11( 1609, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );// trice tool modified code for compilation
    break; case __LINE__: Trice64M_12( 3071, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );// trice tool modified code for compilation

    break; case __LINE__: Trice64M   ( 2563, "msg:value=%d\n", -1 );// trice tool modified code for compilation
    break; case __LINE__: Trice64M   ( 4590, "msg:value=%d, %d\n", -1, -2 );// trice tool modified code for compilation
    break; case __LINE__: Trice64M   ( 1279, "msg:value=%d, %d, %d\n", -1, -2, -3 );// trice tool modified code for compilation
    break; case __LINE__: Trice64M   ( 7030, "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );// trice tool modified code for compilation
    break; case __LINE__: Trice64M   ( 6420, "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );// trice tool modified code for compilation
    break; case __LINE__: Trice64M   ( 5834, "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );// trice tool modified code for compilation
    break; case __LINE__: Trice64M   ( 1307, "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );// trice tool modified code for compilation
    break; case __LINE__: Trice64M   ( 1025, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );// trice tool modified code for compilation
    break; case __LINE__: Trice64M   ( 4086, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );// trice tool modified code for compilation
    break; case __LINE__: Trice64M   ( 4476, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );// trice tool modified code for compilation
    break; case __LINE__: Trice64M   ( 1609, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );// trice tool modified code for compilation
    break; case __LINE__: Trice64M   ( 3071, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );// trice tool modified code for compilation

    break; case __LINE__: TRice64_1 ( "msg:value=%d\n", -1 );// user written code
    break; case __LINE__: TRice64_2 ( "msg:value=%d, %d\n", -1, -2 );// user written code
    break; case __LINE__: TRice64_3 ( "msg:value=%d, %d, %d\n", -1, -2, -3 );// user written code
    break; case __LINE__: TRice64_4 ( "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );// user written code
    break; case __LINE__: TRice64_5 ( "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );// user written code
    break; case __LINE__: TRice64_6 ( "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );// user written code
    break; case __LINE__: TRice64_7 ( "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );// user written code
    break; case __LINE__: TRice64_8 ( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );// user written code
    break; case __LINE__: TRice64_9 ( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );// user written code
    break; case __LINE__: TRice64_10( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );// user written code
    break; case __LINE__: TRice64_11( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );// user written code
    break; case __LINE__: TRice64_12( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );// user written code
    
    break; case __LINE__: TRice64( "msg:value=%d\n", -1 );// user written code
    break; case __LINE__: TRice64( "msg:value=%d, %d\n", -1, -2 );// user written code
    break; case __LINE__: TRice64( "msg:value=%d, %d, %d\n", -1, -2, -3 );// user written code
    break; case __LINE__: TRice64( "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );// user written code
    break; case __LINE__: TRice64( "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );// user written code
    break; case __LINE__: TRice64( "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );// user written code
    break; case __LINE__: TRice64( "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );// user written code
    break; case __LINE__: TRice64( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );// user written code
    break; case __LINE__: TRice64( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );// user written code
    break; case __LINE__: TRice64( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );// user written code
    break; case __LINE__: TRice64( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );// user written code
    break; case __LINE__: TRice64( "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );// user written code

    break; case __LINE__: TRice64M_1 ( 2563, "msg:value=%d\n", -1 );// trice tool modified code for compilation
    break; case __LINE__: TRice64M_2 ( 4590, "msg:value=%d, %d\n", -1, -2 );// trice tool modified code for compilation
    break; case __LINE__: TRice64M_3 ( 1279, "msg:value=%d, %d, %d\n", -1, -2, -3 );// trice tool modified code for compilation
    break; case __LINE__: TRice64M_4 ( 7030, "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );// trice tool modified code for compilation
    break; case __LINE__: TRice64M_5 ( 6420, "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );// trice tool modified code for compilation
    break; case __LINE__: TRice64M_6 ( 5834, "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );// trice tool modified code for compilation
    break; case __LINE__: TRice64M_7 ( 1307, "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );// trice tool modified code for compilation
    break; case __LINE__: TRice64M_8 ( 1025, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );// trice tool modified code for compilation
    break; case __LINE__: TRice64M_9 ( 4086, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );// trice tool modified code for compilation
    break; case __LINE__: TRice64M_10( 4476, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );// trice tool modified code for compilation
    break; case __LINE__: TRice64M_11( 1609, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );// trice tool modified code for compilation
    break; case __LINE__: TRice64M_12( 3071, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );// trice tool modified code for compilation

    break; case __LINE__: TRice64M   ( 2563, "msg:value=%d\n", -1 );// trice tool modified code for compilation
    break; case __LINE__: TRice64M   ( 4590, "msg:value=%d, %d\n", -1, -2 );// trice tool modified code for compilation
    break; case __LINE__: TRice64M   ( 1279, "msg:value=%d, %d, %d\n", -1, -2, -3 );// trice tool modified code for compilation
    break; case __LINE__: TRice64M   ( 7030, "msg:value=%d, %d, %d, %d\n", -1, -2, -3, -4 );// trice tool modified code for compilation
    break; case __LINE__: TRice64M   ( 6420, "msg:value=%d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );// trice tool modified code for compilation
    break; case __LINE__: TRice64M   ( 5834, "msg:value=%d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );// trice tool modified code for compilation
    break; case __LINE__: TRice64M   ( 1307, "msg:value=%d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7 );// trice tool modified code for compilation
    break; case __LINE__: TRice64M   ( 1025, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );// trice tool modified code for compilation
    break; case __LINE__: TRice64M   ( 4086, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );// trice tool modified code for compilation
    break; case __LINE__: TRice64M   ( 4476, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );// trice tool modified code for compilation
    break; case __LINE__: TRice64M   ( 1609, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );// trice tool modified code for compilation
    break; case __LINE__: TRice64M   ( 3071, "msg:value=%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );// trice tool modified code for compilation

    break; case __LINE__:
    break; case __LINE__: { 
                           float a = (float)5.934;
                           float b = a + ((a > 0) ? 0.0005f : -0.0005f);
                           int c = (int)b;
                           int d = (int)(b * 1000) % 1000;
                           int e = (int)(1000 * (float)(a - c)); 
                           TRICE( Id(0), "msg:x = %g = %d.%03d, %d.%03d\n", aFloat(a), c, d, c, e ); //lint !e666
                           TRICE( Id(0), "1/11 = %g\n", aFloat( 1.0/11 ) ); //lint !e666
                       }
    break; case __LINE__: TRICE( Id(0), "FATAL:magenta+b:red\n" );
    break; case __LINE__: TRICE( Id(0), "CRITICAL:red+i:default+h\n" );
    break; case __LINE__: TRICE( Id(0), "EMERGENCY:red+i:blue\n" );
    break; case __LINE__: TRICE( Id(0), "ERROR:11:red\n" );
    break; case __LINE__: TRICE( Id(0), "WARNING:11+i:red\n" );
    break; case __LINE__: TRICE( Id(0), "ATTENTION:11:green\n" );
    break; case __LINE__: TRICE( Id(0), "INFO:cyan+b:default+h\n" );
    break; case __LINE__: TRICE( Id(0), "DEBUG:130+i\n" );
    break; case __LINE__: TRICE( Id(0), "TRACE:default+i:default+h\n" );
    break; case __LINE__: TRICE( Id(0), "TIME:blue+i:blue+h\n" );
    break; case __LINE__: TRICE( Id(0), "MESSAGE:green+h:black\n" );
    break; case __LINE__: TRICE( Id(0), "READ:black+i:yellow+h\n" );
    break; case __LINE__: TRICE( Id(0), "WRITE:black+u:yellow+h\n" );
    break; case __LINE__: TRICE( Id(0), "RECEIVE:black+h:black\n" );
    break; case __LINE__: TRICE( Id(0), "TRANSMIT:black:black+h\n" );
    break; case __LINE__: TRICE( Id(0), "DIAG:yellow+i:default+h\n" );
    break; case __LINE__: TRICE( Id(0), "INTERRUPT:magenta+i:default+h\n" );
    break; case __LINE__: TRICE( Id(0), "SIGNAL:118+i\n" );
    break; case __LINE__: TRICE( Id(0), "TEST:yellow+h:black\n" );
    break; case __LINE__: TRICE( Id(0), "DEFAULT:off\n" );
    break; case __LINE__: TRICE( Id(0), "NOTICE:blue:white+h\n" );
    break; case __LINE__: TRICE( Id(0), "ALERT:magenta:magenta+h\n" );
    break; case __LINE__: TRICE( Id(0), "ASSERT:yellow+i:blue\n" );
    break; case __LINE__: TRICE( Id(0), "ALARM:red+i:white+h\n" );
    break; case __LINE__: TRICE( Id(0), "CYCLE:blue+i:default+h\n" );
    break; case __LINE__: TRICE( Id(0), "VERBOSE:blue:default\n" );
    break; case __LINE__: TRICE( Id(0), "fatal:magenta+b:red\n" );
    break; case __LINE__: TRICE( Id(0), "critical:red+i:default+h\n" );
    break; case __LINE__: TRICE( Id(0), "emergency:red+i:blue\n" );
    break; case __LINE__: TRICE( Id(0), "error:11:red\n" );
    break; case __LINE__: TRICE( Id(0), "warning:11+i:red\n" );
    break; case __LINE__: TRICE( Id(0), "attention:11:green\n" );
    break; case __LINE__: TRICE( Id(0), "info:cyan+b:default+h\n" );
    break; case __LINE__: TRICE( Id(0), "debug:130+i\n" );
    break; case __LINE__: TRICE( Id(0), "trace:default+i:default+h\n" );
    break; case __LINE__: TRICE( Id(0), "tme:blue+i:blue+h\n" );
    break; case __LINE__: TRICE( Id(0), "message:green+h:black\n" );
    break; case __LINE__: TRICE( Id(0), "read:black+i:yellow+h\n" );
    break; case __LINE__: TRICE( Id(0), "write:black+u:yellow+h\n" );
    break; case __LINE__: TRICE( Id(0), "receive:black+h:black\n" );
    break; case __LINE__: TRICE( Id(0), "transmit:black:black+h\n" );
    break; case __LINE__: TRICE( Id(0), "diag:yellow+i:default+h\n" );
    break; case __LINE__: TRICE( Id(0), "interrupt:magenta+i:default+h\n" );
    break; case __LINE__: TRICE( Id(0), "signal:118+i\n" );
    break; case __LINE__: TRICE( Id(0), "test:yellow+h:black\n" );
    break; case __LINE__: TRICE( Id(0), "default:off\n" );
    break; case __LINE__: TRICE( Id(0), "notice:blue:white+h\n" );
    break; case __LINE__: TRICE( Id(0), "allert:magenta:magenta+h\n" );
    break; case __LINE__: TRICE( Id(0), "assert:yellow+i:blue\n" );
    break; case __LINE__: TRICE( Id(0), "alarm:red+i:white+h\n" );
    break; case __LINE__: TRICE( Id(0), "cycle:blue+i:default+h\n" );
    break; case __LINE__: TRICE( Id(0), "verbose:blue:default\n" );
    break; case __LINE__: TRICE( Id(0), "dbg:Hi!\n" );
    break; case __LINE__: TRICE( Id(0), "dbg:\tHi!\n" );
    break; case __LINE__: TRICE( Id(0), "dbg:\t\tHi!\n" );
    break; case __LINE__: TRICE( Id(0), "dbg:\t\t\tHi!\n" );
    break; case __LINE__: TRICE( Id(0), "dbg:\\tHi!\n" );
    //break; case __LINE__: TRICE( Id(0), "dbg:\aHi!\n" ); // sound!
    //break; case __LINE__: TRICE( Id(0), "dbg:\a\aHi!\n" ); // sound!
    //break; case __LINE__: TRICE( Id(0), "dbg:\a\a\aHi!\n" ); // sound!
    break; case __LINE__: TRICE( Id(0), "dbg:\\aHi!\n" );
    break; case __LINE__: {
                              unsigned len = strlen(s);
                              TRICE32( Id(0), "dbg:len=%u:", len );
                              TRICE_S( Id(0), "sig:TRICE_S=%s\n", s );
                              TRICE32( Id(0), "dbg:len=%u:", len);
                              TRICE_N( Id(0), "sig:TRICE_N=%s\n", s, len );

                              TRICE( Id(0), "rd:TRICE line %E (%%E)\n", aFloat(-555555555.5555555555) );
                              TRICE( Id(0), "rd:TRICE line %F (%%F)\n", aFloat(-555555555.5555555555) );
                              TRICE( Id(0), "rd:TRICE line %G (%%G)\n", aFloat(-555555555.5555555555) );
                          }
    break; case __LINE__: TRICE32_1( Id(0), "rd:TRICE32_1 line %u (%%u)\n", -1 );
    break; case __LINE__: TRICE32_1( Id(0), "rd:TRICE32_1 line %b (%%b)\n", -1 );
    break; case __LINE__: TRICE32_1( Id(0), "rd:TRICE32_1 line %o (%%o)\n", -1 );
    break; case __LINE__: TRICE32_1( Id(0), "rd:TRICE32_1 line %O (%%O)\n", -1 );
    break; case __LINE__: TRICE32_1( Id(0), "rd:TRICE32_1 line %X (%%X)\n", -1 );
    break; case __LINE__: TRICE32_1( Id(0), "rd:TRICE32_1 line %x (%%x)\n", -1 );
    break; case __LINE__: TRICE32_1( Id(0), "rd:TRICE32_1 line %d (%%d)\n", -1 );
    break; case __LINE__: TRICE64_1( Id(0), "rd:TRICE64_1 line %u (%%u)\n", -1 );
    break; case __LINE__: TRICE64_1( Id(0), "rd:TRICE64_1 line %b (%%b)\n", -1 );
    break; case __LINE__: TRICE64_1( Id(0), "rd:TRICE64_1 line %o (%%o)\n", -1 );
    break; case __LINE__: TRICE64_1( Id(0), "rd:TRICE64_1 line %O (%%O)\n", -1 );
    break; case __LINE__: TRICE64_1( Id(0), "rd:TRICE64_1 line %X (%%C)\n", -1 );
    break; case __LINE__: TRICE64_1( Id(0), "rd:TRICE64_1 line %x (%%x)\n", -1 );
    break; case __LINE__: TRICE64_1( Id(0), "rd:TRICE64_1 line %d (%%d)\n", -1 );
    break; case __LINE__: TRICE64( Id(0), "rd:TRICE64 line %E (%%E)\n", aDouble(-555555555.5555555555) );
    break; case __LINE__: TRICE64( Id(0), "rd:TRICE64 line %F (%%F)\n", aDouble(-555555555.5555555555) );
    break; case __LINE__: TRICE64( Id(0), "rd:TRICE64 line %G (%%G)\n", aDouble(-555555555.5555555555) );
    break; case __LINE__: TRICE( Id(0), "sig:Some time measurements\n" );
    break; case __LINE__: TRICE( Id(0), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
    break; case __LINE__: TRICE( Id(0), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
    break; case __LINE__: TRICE( Id(0), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
    break; case __LINE__: TRICE( Id(0), "isr:TRICE isr message, SysTick is %6d\n", SYSTICKVAL);
    break; case __LINE__: TRICE64( Id(0), "rd:TRICE64 %d, %d\n", 1, 2 );
    break; case __LINE__: TRICE( Id(0), "tim:TRICE START time message\n");
    break; case __LINE__: TRICE( Id(0), "tim:TRICE STOP time message\n");
    break; case __LINE__: TRICE( Id(0), "tim:TRICE START time message\n");
    break; case __LINE__: TRICE( Id(0), "tim:TRICE STOP time message\n");
    break; case __LINE__: TRICE( Id(0), "tim:TRICE START time message\n");
    break; case __LINE__: TRICE64( Id(0), "rd:TRICE64 %d, %d\n", 1, 2 );
    break; case __LINE__: TRICE( Id(0), "tim:TRICE STOP time message\n");
    break; case __LINE__: TRICE( Id(0), "tim:TRICE --------------------------------------------------\n");
    break; case __LINE__: TRICE( Id(0), "tim:TRICE --------------------------------------------------\n");
    break; case __LINE__: TRICE64( Id(0), "rd:TRICE64 %d, %d\n", 1, 2 );
    break; case __LINE__: TRICE( Id(0), "tim:TRICE --------------------------------------------------\n");
    break; case __LINE__: TRICE( Id(0), "tim:TRICE --------------------------------------------------\n");
    break; case __LINE__: TRICE64( Id(0), "rd:TRICE64 %d, %d\n", 1, 2 );
    break; case __LINE__: TRICE( Id(0),  "rd:TRICE32_1 line %d\n", __LINE__ );
    break; case __LINE__: TRICE64_1( Id(0),  "rd:TRICE64_1 %d\n", __LINE__ );
    break; case __LINE__: TRICE32_2( Id(0),  "rd:TRICE32_2 line %d,%d\n", __LINE__, 2 );
    break; case __LINE__: TRICE64_2( Id(0),  "rd:TRICE64_2 line %d,%d\n", __LINE__, 2 );

    break; case __LINE__: TRICE( Id(0), "sig:TRICE with 1 to 12 values\n" );
    break; case __LINE__: TRICE( Id(0),"rd:TRICE %d\n", -1 );
    break; case __LINE__: TRICE( Id(0),"rd:TRICE %d, %d\n", -1, -2 );
    break; case __LINE__: TRICE( Id(0),"rd:TRICE %d, %d, %d\n", -1, -2, -3 );
    break; case __LINE__: TRICE( Id(0),"rd:TRICE %d, %d, %d, %d\n", -1, -2, -3, -4 );
    break; case __LINE__: TRICE( Id(0),"rd:TRICE %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
    break; case __LINE__: TRICE( Id(0),"rd:TRICE %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
    break; case __LINE__: TRICE( Id(0),"rd:TRICE %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
    break; case __LINE__: TRICE( Id(0),"rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
    break; case __LINE__: TRICE( Id(0),"rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
    break; case __LINE__: TRICE( Id(0),"rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
    break; case __LINE__: TRICE( Id(0),"rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
    break; case __LINE__: TRICE( Id(0),"rd:TRICE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
    break; case __LINE__: TRICE( Id(0), "sig:TRICE_n with 1 to 12 values\n" );
    break; case __LINE__: TRICE_1( Id(0),   "rd:TRICE_1 %d\n", 1 );
    break; case __LINE__: TRICE_2( Id(0),   "rd:TRICE_2 %d, %d\n", 1, 2 );
    break; case __LINE__: TRICE_3( Id(0),   "rd:TRICE_3 %d, %d, %d\n", 1, 2, 3 );
    break; case __LINE__: TRICE_4( Id(0),   "rd:TRICE_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
    break; case __LINE__: TRICE_5( Id(0),   "rd:TRICE_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
    break; case __LINE__: TRICE_6( Id(0),   "rd:TRICE_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
    break; case __LINE__: TRICE_7( Id(0),   "rd:TRICE_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
    break; case __LINE__: TRICE_8( Id(0),   "rd:TRICE_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
    break; case __LINE__: TRICE_9( Id(0),   "rd:TRICE_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
    break; case __LINE__: TRICE_10( Id(0),  "rd:TRICE_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
    break; case __LINE__: TRICE_11( Id(0),  "rd:TRICE_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
    break; case __LINE__: TRICE_12( Id(0),  "rd:TRICE_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );				
  //break; case __LINE__: TRICE( Id(0), "sig:TRICE8 with 1 to 12 values\n" );
  //break; case __LINE__: TRICE8( Id(0),"rd:TRICE8 %d\n", -1 );
  //break; case __LINE__: TRICE8( Id(0),"rd:TRICE8 %d, %d\n", -1, -2 );
  //break; case __LINE__: TRICE8( Id(0),"rd:TRICE8 %d, %d, %d\n", -1, -2, -3 );
  //break; case __LINE__: TRICE8( Id(0),"rd:TRICE8 %d, %d, %d, %d\n", -1, -2, -3, -4 );
  //break; case __LINE__: TRICE8( Id(0),"rd:TRICE8 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
  //break; case __LINE__: TRICE8( Id(0),"rd:TRICE8 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
  //break; case __LINE__: TRICE8( Id(0),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
  //break; case __LINE__: TRICE8( Id(0),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
  //break; case __LINE__: TRICE8( Id(0),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
  //break; case __LINE__: TRICE8( Id(0),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
  //break; case __LINE__: TRICE8( Id(0),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
  //break; case __LINE__: TRICE8( Id(0),"rd:TRICE8 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
  //break; case __LINE__: TRICE( Id(0), "sig:TRICE8_n with 1 to 12 values\n" );
  //break; case __LINE__: TRICE8_1( Id(0),   "rd:TRICE8_1 %d\n", 1 );
  //break; case __LINE__: TRICE8_2( Id(0),   "rd:TRICE8_2 %d, %d\n", 1, 2 );
  //break; case __LINE__: TRICE8_3( Id(0),   "rd:TRICE8_3 %d, %d, %d\n", 1, 2, 3 );
  //break; case __LINE__: TRICE8_4( Id(0),   "rd:TRICE8_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
  //break; case __LINE__: TRICE8_5( Id(0),   "rd:TRICE8_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
  //break; case __LINE__: TRICE8_6( Id(0),   "rd:TRICE8_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
  //break; case __LINE__: TRICE8_7( Id(0),   "rd:TRICE8_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
  //break; case __LINE__: TRICE8_8( Id(0),   "rd:TRICE8_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
  //break; case __LINE__: TRICE8_9( Id(0),   "rd:TRICE8_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
  //break; case __LINE__: TRICE8_10( Id(0),  "rd:TRICE8_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
  //break; case __LINE__: TRICE8_11( Id(0),  "rd:TRICE8_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
  //break; case __LINE__: TRICE8_12( Id(0),  "rd:TRICE8_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );
  //break; case __LINE__: TRICE( Id(0), "sig:TRICE16 with 1 to 12 values\n" );
  //break; case __LINE__: TRICE16( Id(0),"rd:TRICE16 %d\n", -1 );
  //break; case __LINE__: TRICE16( Id(0),"rd:TRICE16 %d, %d\n", -1, -2 );
  //break; case __LINE__: TRICE16( Id(0),"rd:TRICE16 %d, %d, %d\n", -1, -2, -3 );
  //break; case __LINE__: TRICE16( Id(0),"rd:TRICE16 %d, %d, %d, %d\n", -1, -2, -3, -4 );
  //break; case __LINE__: TRICE16( Id(0),"rd:TRICE16 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
  //break; case __LINE__: TRICE16( Id(0),"rd:TRICE16 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
  //break; case __LINE__: TRICE16( Id(0),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
  //break; case __LINE__: TRICE16( Id(0),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
  //break; case __LINE__: TRICE16( Id(0),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
  //break; case __LINE__: TRICE16( Id(0),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
  //break; case __LINE__: TRICE16( Id(0),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
  //break; case __LINE__: TRICE16( Id(0),"rd:TRICE16 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
  //break; case __LINE__: TRICE( Id(0), "sig:TRICE16_n with 1 to 12 values\n" );
  //break; case __LINE__: TRICE16_1( Id(0),  "rd:TRICE16_1 %d\n", 1 );
  //break; case __LINE__: TRICE16_2( Id(0),  "rd:TRICE16_2 %d, %d\n", 1, 2 );
  //break; case __LINE__: TRICE16_3( Id(0),  "rd:TRICE16_3 %d, %d, %d\n", 1, 2, 3 );
  //break; case __LINE__: TRICE16_4( Id(0),  "rd:TRICE16_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
  //break; case __LINE__: TRICE16_5( Id(0),  "rd:TRICE16_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
  //break; case __LINE__: TRICE16_6( Id(0),  "rd:TRICE16_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
  //break; case __LINE__: TRICE16_7( Id(0),  "rd:TRICE16_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
  //break; case __LINE__: TRICE16_8( Id(0),  "rd:TRICE16_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
  //break; case __LINE__: TRICE16_9( Id(0),  "rd:TRICE16_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
  //break; case __LINE__: TRICE16_10( Id(0), "rd:TRICE16_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
  //break; case __LINE__: TRICE16_11( Id(0), "rd:TRICE16_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
  //break; case __LINE__: TRICE16_12( Id(0), "rd:TRICE16_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );
  //break; case __LINE__: TRICE( Id(0), "sig:TRICE16 with 1 to 12 pointer\n" );
  //break; case __LINE__: TRICE16( Id(0),"rd:TRICE16 %p\n", -1 );
  //break; case __LINE__: TRICE16( Id(0),"rd:TRICE16 %p, %p\n", -1, -2 );
  //break; case __LINE__: TRICE16( Id(0),"rd:TRICE16 %p, %p, %p\n", -1, -2, -3 );
  //break; case __LINE__: TRICE16( Id(0),"rd:TRICE16 %p, %p, %p, %p\n", -1, -2, -3, -4 );
  //break; case __LINE__: TRICE16( Id(0),"rd:TRICE16 %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5 );
  //break; case __LINE__: TRICE16( Id(0),"rd:TRICE16 %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6 );
  //break; case __LINE__: TRICE16( Id(0),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7);
  //break; case __LINE__: TRICE16( Id(0),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8 );
  //break; case __LINE__: TRICE16( Id(0),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
  //break; case __LINE__: TRICE16( Id(0),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
  //break; case __LINE__: TRICE16( Id(0),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
  //break; case __LINE__: TRICE16( Id(0),"rd:TRICE16 %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p, %p\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
  //break; case __LINE__: TRICE( Id(0), "sig:TRICE16 with 1 to 12 hex\n" );
  //break; case __LINE__: TRICE16( Id(0),"rd:TRICE16 %X\n", -1 );
  //break; case __LINE__: TRICE16( Id(0),"rd:TRICE16 %X, %X\n", -1, -2 );
  //break; case __LINE__: TRICE16( Id(0),"rd:TRICE16 %X, %X, %X\n", -1, -2, -3 );
  //break; case __LINE__: TRICE16( Id(0),"rd:TRICE16 %X, %X, %X, %X\n", -1, -2, -3, -4 );
  //break; case __LINE__: TRICE16( Id(0),"rd:TRICE16 %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5 );
  //break; case __LINE__: TRICE16( Id(0),"rd:TRICE16 %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6 );
  //break; case __LINE__: TRICE16( Id(0),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7);
  //break; case __LINE__: TRICE16( Id(0),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8 );
  //break; case __LINE__: TRICE16( Id(0),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
  //break; case __LINE__: TRICE16( Id(0),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
  //break; case __LINE__: TRICE16( Id(0),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
  //break; case __LINE__: TRICE16( Id(0),"rd:TRICE16 %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X, %X\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
    break; case __LINE__: TRICE( Id(0), "sig:TRICE32 with 1 to 12 values\n" );
    break; case __LINE__: TRICE32( Id(0),"rd:TRICE32 %d\n", -1 );
    break; case __LINE__: TRICE32( Id(0),"rd:TRICE32 %d, %d\n", -1, -2 );
    break; case __LINE__: TRICE32( Id(0),"rd:TRICE32 %d, %d, %d\n", -1, -2, -3 );
    break; case __LINE__: TRICE32( Id(0),"rd:TRICE32 %d, %d, %d, %d\n", -1, -2, -3, -4 );
    break; case __LINE__: TRICE32( Id(0),"rd:TRICE32 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
    break; case __LINE__: TRICE32( Id(0),"rd:TRICE32 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
    break; case __LINE__: TRICE32( Id(0),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
    break; case __LINE__: TRICE32( Id(0),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
    break; case __LINE__: TRICE32( Id(0),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
    break; case __LINE__: TRICE32( Id(0),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
    break; case __LINE__: TRICE32( Id(0),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
    break; case __LINE__: TRICE32( Id(0),"rd:TRICE32 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
    break; case __LINE__: TRICE( Id(0), "signal:TRICE32_n with 1 to 12 values\n" );
    break; case __LINE__: TRICE32_1( Id(0),  "rd:TRICE32_1 %d\n", 1 );
    break; case __LINE__: TRICE32_2( Id(0),  "rd:TRICE32_2 %d, %d\n", 1, 2 );
    break; case __LINE__: TRICE32_3( Id(0),  "rd:TRICE32_3 %d, %d, %d\n", 1, 2, 3 );
    break; case __LINE__: TRICE32_4( Id(0),  "rd:TRICE32_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
    break; case __LINE__: TRICE32_5( Id(0),  "rd:TRICE32_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
    break; case __LINE__: TRICE32_6( Id(0),  "rd:TRICE32_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
    break; case __LINE__: TRICE32_7( Id(0),  "rd:TRICE32_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
    break; case __LINE__: TRICE32_8( Id(0),  "rd:TRICE32_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
    break; case __LINE__: TRICE32_9( Id(0),  "rd:TRICE32_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
    break; case __LINE__: TRICE32_10( Id(0), "rd:TRICE32_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
    break; case __LINE__: TRICE32_11( Id(0), "rd:TRICE32_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
    break; case __LINE__: TRICE32_12( Id(0), "rd:TRICE32_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );
    break; case __LINE__: TRICE( Id(0), "sig:TRICE64 with 1 to 12 values\n" );
  //break; case __LINE__: TRICE64( Id(0),"rd:TRICE64 %d\n", -1 );
  //break; case __LINE__: TRICE64( Id(0),"rd:TRICE64 %d, %d\n", -1, -2 );
  //break; case __LINE__: TRICE64( Id(0),"rd:TRICE64 %d, %d, %d\n", -1, -2, -3 );
  //break; case __LINE__: TRICE64( Id(0),"rd:TRICE64 %d, %d, %d, %d\n", -1, -2, -3, -4 );
  //break; case __LINE__: TRICE64( Id(0),"rd:TRICE64 %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5 );
  //break; case __LINE__: TRICE64( Id(0),"rd:TRICE64 %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6 );
  //break; case __LINE__: TRICE64( Id(0),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7);
  //break; case __LINE__: TRICE64( Id(0),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
  //break; case __LINE__: TRICE64( Id(0),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
  //break; case __LINE__: TRICE64( Id(0),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10);
  //break; case __LINE__: TRICE64( Id(0),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
  //break; case __LINE__: TRICE64( Id(0),"rd:TRICE64 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
  //break; case __LINE__: TRICE( Id(0), "sig:TRICE64_n with 1 to 12 values\n" );
  //break; case __LINE__: TRICE64_1( Id(0),  "rd:TRICE64_1 %d\n", 1 );
  //break; case __LINE__: TRICE64_2( Id(0),  "rd:TRICE64_2 %d, %d\n", 1, 2 );
  //break; case __LINE__: TRICE64_2( Id(0),  "rd:TRICE64_2 %x, %x\n", 0x1122334455667788, 0x0102030405060708 );
  //break; case __LINE__: TRICE64_3( Id(0),  "rd:TRICE64_3 %d, %d, %d\n", 1, 2, 3 );
  //break; case __LINE__: TRICE64_4( Id(0),  "rd:TRICE64_4 %d, %d, %d, %d\n", 1, 2, 3, 4 );
  //break; case __LINE__: TRICE64_5( Id(0),  "rd:TRICE64_5 %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5 );
  //break; case __LINE__: TRICE64_6( Id(0),  "rd:TRICE64_6 %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6 );
  //break; case __LINE__: TRICE64_7( Id(0),  "rd:TRICE64_7 %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7);
  //break; case __LINE__: TRICE64_8( Id(0),  "rd:TRICE64_8 %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8 );
  //break; case __LINE__: TRICE64_9( Id(0),  "rd:TRICE64_9 %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9 );
  //break; case __LINE__: TRICE64_10( Id(0), "rd:TRICE64_10 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
  //break; case __LINE__: TRICE64_11( Id(0), "rd:TRICE64_11 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 );
  //break; case __LINE__: TRICE64_12( Id(0), "rd:TRICE64_12 %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n", 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 );
  //break; case __LINE__: TRICE( Id(0), "sig:Colors and ticks\n" );
  //break; case __LINE__: TRICE( Id(0), "--------------------------------------------------\n");
  //break; case __LINE__: TRICE( Id(0), "--------------------------------------------------\n");
  //break; case __LINE__: TRICE( Id(0), "dbg:12345 as 16bit is %016b\n", 12345);
  //break; case __LINE__: TRICE( Id(0), "--------------------------------------------------\n");
  //break; case __LINE__: TRICE( Id(0), "sig:This ASSERT error is just a demo and no real error:\n");
  //break; case __LINE__: TRICE( Id(0), "--------------------------------------------------\n");
  //break; case __LINE__: TRICE( Id(0), "ERR:error       message, SysTick is %6u\n", SYSTICKVAL);
  //break; case __LINE__: TRICE( Id(0), "WRN:warning     message, SysTick is %6u\n", SYSTICKVAL);
  //break; case __LINE__: TRICE( Id(0), "ATT:attention   message, SysTick is %6u\n", SYSTICKVAL);
  //break; case __LINE__: TRICE( Id(0), "DIA:diagnostics message, SysTick is %6u\n", SYSTICKVAL);
  //break; case __LINE__: TRICE( Id(0), "TIM:timing      message, SysTick is %6u\n", SYSTICKVAL);
  //break; case __LINE__: TRICE( Id(0), "DBG:debug       message, SysTick is %6u\n", SYSTICKVAL);
  //break; case __LINE__: TRICE( Id(0), "SIG:signal      message, SysTick is %6u\n", SYSTICKVAL);
  //break; case __LINE__: TRICE( Id(0), "RD:read         message, SysTick is %6u\n", SYSTICKVAL);
  //break; case __LINE__: TRICE( Id(0), "WR:write        message, SysTick is %6u\n", SYSTICKVAL);
  //break; case __LINE__: TRICE( Id(0), "ISR:interrupt   message, SysTick is %6u\n", SYSTICKVAL);
  //break; case __LINE__: TRICE( Id(0), "MSG:normal      message, SysTick is %6u\n", SYSTICKVAL);
  //break; case __LINE__: TRICE( Id(0), "INFO:informal   message, SysTick is %6u\n", SYSTICKVAL);
  //break; case __LINE__: TRICE( Id(0), "time:TRICE32_1  message, SysTick is %6u\n", SYSTICKVAL);
  //break; case __LINE__: TRICE( Id(0), "sig:Various ranges and formats\n" );
  //break; case __LINE__: TRICE8( Id(0), "tst:TRICE8  %%03x -> %03x  %03x  %03x  %03x\n", 1, 0x7f, 0x80, 0xff);
  //break; case __LINE__: TRICE8( Id(0), "tst:TRICE8   %%4d -> %4d %4d %4d %4d\n", 1, 0x7f, 0x80, 0xff);
  //break; case __LINE__: TRICE8( Id(0), "tst:TRICE8   %%4u -> %4u %4u %4u %4u\n", 1, 0x7f, 0x80, 0xff);
  //break; case __LINE__: TRICE8( Id(0), "tst:TRICE8   %%4o -> %4o %4o %4o %4o\n", 1, 0x7f, 0x80, 0xff);
  //break; case __LINE__: TRICE8( Id(0), "tst:TRICE8   %%4O -> %4O %4O %4O %4O\n", 1, 0x7f, 0x80, 0xff);
  //break; case __LINE__: TRICE8( Id(0), "tst:TRICE8   %%4X -> %4X %4X %4X %4X\n", 1, 0x7f, 0x80, 0xff);
  //break; case __LINE__: TRICE8( Id(0), "tst:TRICE8   %%4x -> %4x %4x %4x %4x\n", 1, 0x7f, 0x80, 0xff);
  //break; case __LINE__: TRICE8( Id(0), "tst:TRICE8   %%8b -> %8b %8b %8b %8b\n", 1, 0x7f, 0x80, 0xff);
  //break; case __LINE__: TRICE8( Id(0), "tst:TRICE8  %%08b -> %08b %08b %08b %08b\n", 1, 0x7f, 0x80, 0xff);
  //break; case __LINE__: TRICE16(Id(0), "tst:TRICE16  %%05x ->   %05x   %05x   %05x   %05x\n", 1, 0x7fff, 0x8000, 0xffff);
  //break; case __LINE__: TRICE16(Id(0), "tst:TRICE16   %%6d ->  %6d  %6d  %6d  %6d\n",         1, 0x7fff, 0x8000, 0xffff);
  //break; case __LINE__: TRICE16(Id(0), "tst:TRICE16   %%7o -> %7o %7o %7o %7o\n",             1, 0x7fff, 0x8000, 0xffff);
  //break; case __LINE__: TRICE32(Id(0), "tst:TRICE32 %%09x ->      %09x      %09x       %09x     %09x\n", 1, 0x7fffffff, 0x80000000, 0xffffffff);
  //break; case __LINE__: TRICE32(Id(0), "tst:TRICE32 %%10d ->     %10d     %10d     %10d    %10x\n",      1, 0x7fffffff, 0x80000000, 0xffffffff);
  //break; case __LINE__: TRICE32(Id(0), "att:TRICE32 %40b\n", 0xAAAAAAAA);
  //break; case __LINE__: TRICE64(Id(0), "tst:TRICE64 %%09x ->      %09x      %09x       %09x     %09x\n", 1, 0x7fffffffffffffff, 0x8000000000000000, 0xffffffffffffffff);
  //break; case __LINE__: TRICE64(Id(0), "tst:TRICE64 %%10d ->     %10d     %10d     %10d    %10x\n",      1, 0x7fffffffffffffff, 0x8000000000000000, 0xffffffffffffffff);
  //break; case __LINE__: TRICE64(Id(0), "att:TRICE64 %70b\n", 0xAAAAAAAAAAAAAAAA);
  //break; case __LINE__: TRICE( Id(0), "sig:Legacy TRICE8\n" );
  //break; case __LINE__: TRICE8_1(Id(0), "tst:TRICE8_1 %02x\n", 0xA1);
  //break; case __LINE__: TRICE8_2(Id(0), "tst:TRICE8_2 %02x %02x\n", 0xA1, 0xA2);
  //break; case __LINE__: TRICE8_3(Id(0), "tst:TRICE8_3 %02x %02x %02x\n", 0xA1, 0xA2, 0xA3);
  //break; case __LINE__: TRICE8_4(Id(0), "tst:TRICE8_4 %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4);
  //break; case __LINE__: TRICE8_5(Id(0), "tst:TRICE8_5 %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5 );
  //break; case __LINE__: TRICE8_6(Id(0), "tst:TRICE8_6 %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6);
  //break; case __LINE__: TRICE8_7(Id(0), "tst:TRICE8_7 %02x %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7);
  //break; case __LINE__: TRICE8_8(Id(0), "tst:TRICE8_8 %02x %02x %02x %02x %02x %02x %02x %02x\n", 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8);
  //break; case __LINE__: TRICE8_1(Id(0), "tst:TRICE8_1 %d\n",                      -1);
  //break; case __LINE__: TRICE8_2(Id(0), "tst:TRICE8_2 %d %d\n",                   -1, -2);
  //break; case __LINE__: TRICE8_3(Id(0), "tst:TRICE8_3 %d %d %d\n",                -1, -2, -3);
  //break; case __LINE__: TRICE8_4(Id(0), "tst:TRICE8_4 %d %d %d %d\n",             -1, -2, -3, -4);
  //break; case __LINE__: TRICE8_5(Id(0), "tst:TRICE8_5 %d %d %d %d %d\n",          -1, -2, -3, -4, -5);
  //break; case __LINE__: TRICE8_6(Id(0), "tst:TRICE8_6 %d %d %d %d %d %d\n",       -1, -2, -3, -4, -5, -6);
  //break; case __LINE__: TRICE8_7(Id(0), "tst:TRICE8_7 %d %d %d %d %d %d %d\n",    -1, -2, -3, -4, -5, -6, -7);
  //break; case __LINE__: TRICE8_8(Id(0), "tst:TRICE8_8 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8);
  //break; case __LINE__: TRICE8_1(Id(0), "tst:TRICE8_1 %d\n", 1);
  //break; case __LINE__: TRICE8_2(Id(0), "tst:TRICE8_2 %d %d\n", 1, 2);
  //break; case __LINE__: TRICE8_3(Id(0), "tst:TRICE8_3 %d %d %d\n", 1, 2, 3);
  //break; case __LINE__: TRICE8_4(Id(0), "tst:TRICE8_4 %d %d %d %d\n", 1, 2, 3, 4);
  //break; case __LINE__: TRICE8_5(Id(0), "tst:TRICE8_5 %d %d %d %d %d\n", 1, 2, 3, 4, 5);
  //break; case __LINE__: TRICE8_6(Id(0), "tst:TRICE8_6 %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6);
  //break; case __LINE__: TRICE8_7(Id(0), "tst:TRICE8_7 %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7);
  //break; case __LINE__: TRICE8_8(Id(0), "tst:TRICE8_8 %d %d %d %d %d %d %d %d\n", 1, 2, 3, 4, 5, 6, 7, 8);
  //break; case __LINE__: TRICE8_1(Id(0), "tst:TRICE8_1 %u\n", 201);
  //break; case __LINE__: TRICE8_2(Id(0), "tst:TRICE8_2 %u %u\n", 201, 202);
  //break; case __LINE__: TRICE8_3(Id(0), "tst:TRICE8_3 %u %u %u\n", 201, 202, 203);
  //break; case __LINE__: TRICE8_4(Id(0), "tst:TRICE8_4 %u %u %u %u\n", 201, 202, 203, 204);
  //break; case __LINE__: TRICE8_5(Id(0), "tst:TRICE8_5 %u %u %u %u %u\n", 201, 202, 203, 204, 205);
  //break; case __LINE__: TRICE8_6(Id(0), "tst:TRICE8_6 %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206);
  //break; case __LINE__: TRICE8_7(Id(0), "tst:TRICE8_7 %u %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206, 207);
  //break; case __LINE__: TRICE8_8(Id(0), "tst:TRICE8_8 %u %u %u %u %u %u %u %u\n", 201, 202, 203, 204, 205, 206, 207, 208);
  //break; case __LINE__: TRICE8_8(Id(0), "tst:TRICE8_1 %%d=%d, %%u=%u, 0x%%x=0x%x, 0x%%2x=0x%2x, 0x%%02x=0x%02x, 0x%%3x=0x%3x, 0x%%03x=0x%03x, %%b=%b\n", 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81);
    break; case __LINE__: TRICE( Id(0), "sig:Legacy TRICE16-64\n" );
    break; case __LINE__: TRICE16_1(Id(0), "tst:TRICE16_1 %d\n", -111);
    break; case __LINE__: TRICE16_2(Id(0), "tst:TRICE16_2 %d %d\n", -111, -222);
    break; case __LINE__: TRICE16_3(Id(0), "tst:TRICE16_3 %d %d %d\n", -111, -222, -333);
    break; case __LINE__: TRICE16_4(Id(0), "tst:TRICE16_4 %d %d %d %d\n", -111, -222, -333, -444);
    break; case __LINE__: TRICE32_1(Id(0), "tst:TRICE32_1 %08x\n", 0x0123cafe);
    break; case __LINE__: TRICE32_1(Id(0), "tst:TRICE32_1 %d\n", -111);
    break; case __LINE__: TRICE32_2(Id(0), "tst:TRICE32_2 %x %x\n", -111, -222);
    break; case __LINE__: TRICE32_2(Id(0), "tst:TRICE32_2 %d %d\n", -111, -222);
    break; case __LINE__: TRICE32_3(Id(0), "tst:TRICE32_3 %x %x %x\n", -111, -222, -333);
    break; case __LINE__: TRICE32_3(Id(0), "tst:TRICE32_3 %d %d %d\n", -111, -222, -333);
    break; case __LINE__: TRICE32_4(Id(0), "tst:TRICE32_4 %x %x %x %x\n", -111, -222, -333, -444);
    break; case __LINE__: TRICE32_4(Id(0), "tst:TRICE32_4 %d %d %d %d\n", -111, -222, -333, -444);
    break; case __LINE__: TRICE64_1(Id(0), "tst:TRICE64_1 %d\n", -111);
    break; case __LINE__: TRICE64_2(Id(0), "tst:TRICE64_2 %d %d\n", -111, -222);
    break; case __LINE__: TRICE16_1(Id(0), "tst:TRICE16_1 %u\n", 60001);
    break; case __LINE__: TRICE16_2(Id(0), "tst:TRICE16_2 %u %u\n", 60001, 60002);
    break; case __LINE__: TRICE16_3(Id(0), "tst:TRICE16_3 %u %u %u\n", 60001, 60002, 60003);
    break; case __LINE__: TRICE16_4(Id(0), "tst:TRICE16_4 %u %u %u %u\n", 60001, 60002, 60003, 60004);
    break; case __LINE__: TRICE32_1(Id(0), "tst:TRICE32_1 %u\n", 4000000001);
    break; case __LINE__: TRICE32_2(Id(0), "tst:TRICE32_2 %u %u\n", 4000000001, 4000000002);
    break; case __LINE__: TRICE32_3(Id(0), "tst:TRICE32_3 %u %u %u\n", 4000000001, 4000000002, 4000000003);
    break; case __LINE__: TRICE32_4(Id(0), "tst:TRICE32_4 %u %u %u %u\n", 4000000001, 4000000002, 4000000003, 4000000004);
    break; case __LINE__: TRICE64_1(Id(0), "tst:TRICE64_1 %x\n", -1);
    break; case __LINE__: TRICE64_2(Id(0), "tst:TRICE64_2 %x %x\n", -1, -2);
    break; case __LINE__: TRICE64_1(Id(0), "tst:TRICE64_1 %u\n", -1);
    break; case __LINE__: TRICE64_2(Id(0), "tst:TRICE64_2 %u %u\n", -1, -2);
  //break; case __LINE__: TRICE( Id(0), "sig:colored single letters and several TRICE macros in one line\n" );
  //break; case __LINE__: TRICE(Id(0), "e:A");
  //break; case __LINE__: TRICE(Id(0), "w:B");
  //break; case __LINE__: TRICE(Id(0), "a:c");
  //break; case __LINE__: TRICE(Id(0), "wr:d");
  //break; case __LINE__: TRICE(Id(0), "rd:e\n");
  //break; case __LINE__: TRICE(Id(0), "diag:f");
  //break; case __LINE__: TRICE(Id(0), "d:G");
  //break; case __LINE__: TRICE(Id(0), "t:H");
  //break; case __LINE__: TRICE(Id(0), "time:i");
  //break; case __LINE__: TRICE(Id(0), "message:J");
  //break; case __LINE__: TRICE(Id(0), "dbg:k\n");
  //break; case __LINE__: TRICE(Id(0), "1");
  //break; case __LINE__: TRICE(Id(0), "2");
  //break; case __LINE__: TRICE(Id(0), "3");
  //break; case __LINE__: TRICE(Id(0), "4");
  //break; case __LINE__: TRICE(Id(0), "e:7");
  //break; case __LINE__: TRICE(Id(0), "m:12");
  //break; case __LINE__: TRICE(Id(0), "m:123\n");
  //break; case __LINE__: TRICE(Id(0), "e:A");TRICE(Id(0), "w:B");TRICE(Id(0), "a:c");
  //break; case __LINE__: TRICE(Id(0), "wr:d");TRICE(Id(0), "rd:e\n");//TRICE(Id(0), "diag:f");
/*
    break; case __LINE__: TRICE( Id(0), "sig:Runtime generated strings\n" );
    break; case __LINE__: {
                              s = "AAAAAAAAAAAA";
                              TRICE32( Id(0), "dbg:len=%u:", strlen(s) );
                              TRICE_S( Id(0), "sig:%s\n", s );
                          }
    break; case __LINE__: {
            s = "\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBCCCCCCCC\
";
                          }
    break; case __LINE__: TRICE32( Id(0), "dbg:len=%u:", strlen(s) );
    break; case __LINE__: TRICE_S( Id(0), "sig:%s\n", s );
    break; case __LINE__: TRICE( Id(0), "sig:Runtime generated strings\n" );
    break; case __LINE__: {
            s = "\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\
DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\
EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\
";
                          }
    break; case __LINE__: TRICE32( Id(0), "dbg:len=%u: ", strlen(s) );
    break; case __LINE__: TRICE_S( Id(0), "sig:%s\n", s );
    break; case __LINE__: TRICE( Id(0), "sig:Runtime generated strings\n" );
    break; case __LINE__: {
            s = "\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\
DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\
EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\
";
                          }
    break; case __LINE__: TRICE32( Id(0), "dbg:len=%u: ", strlen(s) );
    break; case __LINE__: TRICE_S( Id(0), "sig:%s\n", s );
    break; case __LINE__: TRICE( Id(0), "sig:Runtime generated strings\n" );
    break; case __LINE__: {
            s = "\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\
DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\
EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\
DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\
EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\
BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB\
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC\
DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\
EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\
";
                          }
    break; case __LINE__: TRICE32( Id(0), "dbg:len=%u: ", strlen(s) );
    break; case __LINE__: TRICE_S( Id(0), "sig:%s\n", s );
    break; case __LINE__: { // https://yourbasic.org/golang/fmt-printf-reference-cheat-sheet/
                              int i = 15;
                              TRICE( Id(0), "sig:Integer (indent, base, sign)\n" );
                              TRICE( Id(0), "rd: 15 	%d 	Base 10\n", i );
                              TRICE( Id(0), "rd: +15 	%+d 	Always show sign\n", i );
                              TRICE( Id(0), "rd:   15 	%4d 	Pad with spaces (width 4, right justified)\n", i );
                              TRICE( Id(0), "rd: 15   	%-4d 	Pad with spaces (width 4, left justified)\n", i );
                              TRICE( Id(0), "rd: 0015 	%04d 	Pad with zeroes (width 4)\n", i );
                              TRICE( Id(0), "rd: 1111 	%b 	Base 2\n", i );
                              TRICE( Id(0), "rd: 17 	%o 	Base 8\n", i );
                              TRICE( Id(0), "rd: f 	%x 	Base 16, lowercase\n", i );
                              TRICE( Id(0), "rd: F 	%X 	Base 16, uppercase\n", i );
                              TRICE( Id(0), "rd: 0xf 	%#x 	Base 16, with leading 0x\n", i );
                          }

    break; case __LINE__: {
                              char a = 'A';
                              TRICE( Id(0), "sig:Character (quoted, Unicode)\n" );
                              TRICE( Id(0), "rd: A 	%c 	Character\n", a );
                              TRICE( Id(0), "rd: 'A' 	%q 	Quoted character\n", a );
                              TRICE( Id(0), "rd: U+0041 	%U 	Unicode\n", a );// todo
                              TRICE( Id(0), "rd: U+0041 'A' 	%#U 	Unicode with character\n", a );// todo
                              TRICE( Id(0), "sig:Boolean (true/false)\n" );
                              TRICE( Id(0), "rd:Use %%t to format a boolean as true (%t) or false (%t).\n", 1, 0 );
                              TRICE( Id(0), "sig:Pointer (hex)\n" );
                          }

    break; case __LINE__: TRICE8( Id(0), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x84 );
    break; case __LINE__: TRICE16( Id(0), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x1234 );
    break; case __LINE__: TRICE32( Id(0), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0xaabbccdd );
    break; case __LINE__: TRICE64( Id(0), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0x1122334455667788 );
    break; case __LINE__: TRICE( Id(0), "rd:Use %%p to format a pointer in base 16 notation with leading 0x. (%p)\n", 0xaabbccdd);
    break; case __LINE__: {
                                float f = (float)123.456; 
                                TRICE( Id(0), "sig:Float (indent, precision, scientific notation)\n" );
                                TRICE( Id(0), "rd: 1.234560e+02		%e 	%%e Scientific notation\n", aFloat(f) );
                                TRICE( Id(0), "rd: 123.456000		%f 	%%f Decimal point, no exponent\n", aFloat(f) );
                                TRICE( Id(0), "rd: 123.46	 		%.2f		%%.2f Default width, precision 2\n", aFloat(f) );
                                TRICE( Id(0), "rd: ␣␣123.46		%8.2f	%%8.2f Width 8, precision 2\n", aFloat(f) );
                                TRICE( Id(0), "rd: 123.456		%g		%%g Exponent as needed, necessary digits only\n", aFloat(f) );
                          }
    break; case __LINE__: {
                                double d = 123.456;
                                TRICE( Id(0), "sig:Double (indent, precision, scientific notation)\n" );
                                TRICE64( Id(0), "rd: 1.234560e+02		%e 	%%e Scientific notation\n", aDouble(d) );
                                TRICE64( Id(0), "rd: 123.456000		%f 	%%f Decimal point, no exponent\n", aDouble(d) );
                                TRICE64( Id(0), "rd: 123.46			%.2f    	%%.2f Default width, precision 2\n", aDouble(d) );
                                TRICE64( Id(0), "rd: ␣␣123.46		%8.2f 	%%8.2f Width 8, precision 2\n", aDouble(d) );
                                TRICE64( Id(0), "rd: 123.456		%g		%%g Exponent as needed, necessary digits only\n", aDouble(d) );
                          }
    break; case __LINE__: {
                              s = "café";
                              TRICE( Id(0), "sig:String or byte slice (quote, indent, hex)\n" );
                              TRICE_S( Id(0), "rd: café 			%s		Plain string\n", s );
                              TRICE_S( Id(0), "rd: ␣␣café 		%6s 		Width 6, right justify\n", s );
                              TRICE_S( Id(0), "rd: café␣␣ 		%-6s 		Width 6, left justify\n", s );
                              TRICE_S( Id(0), "rd: \"café\" 		%q	 	Quoted string\n", s );
                              TRICE_S( Id(0), "rd: 636166c3a9 		%x	Hex dump of byte values\n", s );
                              TRICE_S( Id(0), "rd: 63 61 66 c3 a9 	% x	Hex dump with spaces\n", s );
                          }
*/
  //break; case __LINE__: TRICE( Id(0), "sig:TRICE8 with variable param count 1 to 12\n" );
  //break; case __LINE__: TRICE8(Id(0), "tst:TRICE8 %d\n", -1 );
  //break; case __LINE__: TRICE8(Id(0), "tst:TRICE8 %d %d\n", -1, -2 );
  //break; case __LINE__: TRICE8(Id(0), "tst:TRICE8 %d %d %d\n", -1, -2, -3 );
  //break; case __LINE__: TRICE8(Id(0), "tst:TRICE8 %d %d %d %d\n", -1, -2, -3, -4 );
  //break; case __LINE__: TRICE8(Id(0), "tst:TRICE8 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
  //break; case __LINE__: TRICE8(Id(0), "tst:TRICE8 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
  //break; case __LINE__: TRICE8(Id(0), "tst:TRICE8 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
  //break; case __LINE__: TRICE8(Id(0), "tst:TRICE8 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
  //break; case __LINE__: TRICE8(Id(0), "tst:TRICE8 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
  //break; case __LINE__: TRICE8(Id(0), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
  //break; case __LINE__: TRICE8(Id(0), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
  //break; case __LINE__: TRICE8(Id(0), "tst:TRICE8 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
  //break; case __LINE__: TRICE8(Id(0), "tst:TRICE8 %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );
  //break; case __LINE__: TRICE8_1 (Id(0), "tst:TRICE8_1  %d\n", -1 );
  //break; case __LINE__: TRICE8_2 (Id(0), "tst:TRICE8_2  %d %d\n", -1, -2 );
  //break; case __LINE__: TRICE8_3 (Id(0), "tst:TRICE8_3  %d %d %d\n", -1, -2, -3 );
  //break; case __LINE__: TRICE8_4 (Id(0), "tst:TRICE8_4  %d %d %d %d\n", -1, -2, -3, -4 );
  //break; case __LINE__: TRICE8_5 (Id(0), "tst:TRICE8_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
  //break; case __LINE__: TRICE8_6 (Id(0), "tst:TRICE8_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
  //break; case __LINE__: TRICE8_7 (Id(0), "tst:TRICE8_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
  //break; case __LINE__: TRICE8_8 (Id(0), "tst:TRICE8_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
  //break; case __LINE__: TRICE8_9 (Id(0), "tst:TRICE8_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
  //break; case __LINE__: TRICE8_10(Id(0), "tst:TRICE8_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
  //break; case __LINE__: TRICE8_11(Id(0), "tst:TRICE8_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
  //break; case __LINE__: TRICE8_12(Id(0), "tst:TRICE8_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
  //break; case __LINE__: TRICE8  (Id(0), "tst:TRICE8   %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );
  //break; case __LINE__: TRICE8_9(Id(0), "tst:TRICE8_9 %t %b %x %X %d %u %o %O %p\n", -3, -3, -3, -3, -3, -3, -3, -3, -3 );
  //break; case __LINE__: TRICE( Id(0), "sig:TRICE16 with variable param count 1 to 12\n" );
  //break; case __LINE__: TRICE16(Id(0), "tst:TRICE16 %d\n", -1 );
  //break; case __LINE__: TRICE16(Id(0), "tst:TRICE16 %d %d\n", -1, -2 );
  //break; case __LINE__: TRICE16(Id(0), "tst:TRICE16 %d %d %d\n", -1, -2, -3 );
  //break; case __LINE__: TRICE16(Id(0), "tst:TRICE16 %d %d %d %d\n", -1, -2, -3, -4 );
  //break; case __LINE__: TRICE16(Id(0), "tst:TRICE16 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
  //break; case __LINE__: TRICE16(Id(0), "tst:TRICE16 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
  //break; case __LINE__: TRICE16(Id(0), "tst:TRICE16 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
  //break; case __LINE__: TRICE16(Id(0), "tst:TRICE16 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
  //break; case __LINE__: TRICE16(Id(0), "tst:TRICE16 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
  //break; case __LINE__: TRICE16(Id(0), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
  //break; case __LINE__: TRICE16(Id(0), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
  //break; case __LINE__: TRICE16(Id(0), "tst:TRICE16 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
  //break; case __LINE__: TRICE16(Id(0), "tst:TRICE16 %t %b %x %X %d %u %o %O %p\n", 3, 3, 3, 3, 3, 3, 3, 3, 3 );
  //break; case __LINE__: TRICE16_1 (Id(0), "tst:TRICE16_1  %d\n", -1 );
  //break; case __LINE__: TRICE16_2 (Id(0), "tst:TRICE16_2  %d %d\n", -1, -2 );
  //break; case __LINE__: TRICE16_3 (Id(0), "tst:TRICE16_3  %d %d %d\n", -1, -2, -3 );
  //break; case __LINE__: TRICE16_4 (Id(0), "tst:TRICE16_4  %d %d %d %d\n", -1, -2, -3, -4 );
  //break; case __LINE__: TRICE16_5 (Id(0), "tst:TRICE16_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
  //break; case __LINE__: TRICE16_6 (Id(0), "tst:TRICE16_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
  //break; case __LINE__: TRICE16_7 (Id(0), "tst:TRICE16_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
  //break; case __LINE__: TRICE16_8 (Id(0), "tst:TRICE16_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
  //break; case __LINE__: TRICE16_9 (Id(0), "tst:TRICE16_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
  //break; case __LINE__: TRICE16_10(Id(0), "tst:TRICE16_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
  //break; case __LINE__: TRICE16_11(Id(0), "tst:TRICE16_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
  //break; case __LINE__: TRICE16_12(Id(0), "tst:TRICE16_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
  //break; case __LINE__: TRICE16_9(Id(0), "tst:TRICE16 %t %b %x %X %d %u %o %O %p\n", 3, 3, 3, 3, 3, 3, 3, 3, 3 );
    break; case __LINE__: TRICE( Id(0), "sig:TRICE32 with variable param count 1 to 12\n" );
    break; case __LINE__: TRICE32(Id(0), "tst:TRICE32 %d\n", -1 );
    break; case __LINE__: TRICE32(Id(0), "tst:TRICE32 %d %d\n", -1, -2 );
    break; case __LINE__: TRICE32(Id(0), "tst:TRICE32 %d %d %d\n", -1, -2, -3 );
    break; case __LINE__: TRICE32(Id(0), "tst:TRICE32 %d %d %d %d\n", -1, -2, -3, -4 );
    break; case __LINE__: TRICE32(Id(0), "tst:TRICE32 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
    break; case __LINE__: TRICE32(Id(0), "tst:TRICE32 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
    break; case __LINE__: TRICE32(Id(0), "tst:TRICE32 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
    break; case __LINE__: TRICE32(Id(0), "tst:TRICE32 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
    break; case __LINE__: TRICE32(Id(0), "tst:TRICE32 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
    break; case __LINE__: TRICE32(Id(0), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
    break; case __LINE__: TRICE32(Id(0), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
    break; case __LINE__: TRICE32(Id(0), "tst:TRICE32 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
    break; case __LINE__: TRICE32(Id(0), "tst:TRICE32 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));
    break; case __LINE__: TRICE32_1 (Id(0), "tst:TRICE32_1  %d\n", -1 );
    break; case __LINE__: TRICE32_2 (Id(0), "tst:TRICE32_2  %d %d\n", -1, -2 );
    break; case __LINE__: TRICE32_3 (Id(0), "tst:TRICE32_3  %d %d %d\n", -1, -2, -3 );
    break; case __LINE__: TRICE32_4 (Id(0), "tst:TRICE32_4  %d %d %d %d\n", -1, -2, -3, -4 );
    break; case __LINE__: TRICE32_5 (Id(0), "tst:TRICE32_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
    break; case __LINE__: TRICE32_6 (Id(0), "tst:TRICE32_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
    break; case __LINE__: TRICE32_7 (Id(0), "tst:TRICE32_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
    break; case __LINE__: TRICE32_8 (Id(0), "tst:TRICE32_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
    break; case __LINE__: TRICE32_9 (Id(0), "tst:TRICE32_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
    break; case __LINE__: TRICE32_10(Id(0), "tst:TRICE32_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
    break; case __LINE__: TRICE32_11(Id(0), "tst:TRICE32_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
    break; case __LINE__: TRICE32_12(Id(0), "tst:TRICE32_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
    break; case __LINE__: TRICE32   (Id(0), "tst:TRICE32    %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));
    break; case __LINE__: TRICE32_12(Id(0), "tst:TRICE32_12 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aFloat(3.14159), aFloat(3.14159), aFloat(3.14159));
    break; case __LINE__: TRICE( Id(0), "sig:TRICE64 with variable param count 1 to 12\n" );
    break; case __LINE__: TRICE64(Id(0), "tst:TRICE64 %d\n", -1 );
    break; case __LINE__: TRICE64(Id(0), "tst:TRICE64 %d %d\n", -1, -2 );
    break; case __LINE__: TRICE64(Id(0), "tst:TRICE64 %d %d %d\n", -1, -2, -3 );
    break; case __LINE__: TRICE64(Id(0), "tst:TRICE64 %d %d %d %d\n", -1, -2, -3, -4 );
    break; case __LINE__: TRICE64(Id(0), "tst:TRICE64 %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
    break; case __LINE__: TRICE64(Id(0), "tst:TRICE64 %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
    break; case __LINE__: TRICE64(Id(0), "tst:TRICE64 %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
    break; case __LINE__: TRICE64(Id(0), "tst:TRICE64 %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
    break; case __LINE__: TRICE64(Id(0), "tst:TRICE64 %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
    break; case __LINE__: TRICE64(Id(0), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
    break; case __LINE__: TRICE64(Id(0), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
    break; case __LINE__: TRICE64(Id(0), "tst:TRICE64 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
    break; case __LINE__: TRICE64(Id(0), "tst:TRICE64 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aDouble(3.14159), aDouble(3.14159), aDouble(3.14159));
    break; case __LINE__: TRICE64_1 (Id(0), "tst:TRICE64_1  %d\n", -1 );
    break; case __LINE__: TRICE64_2 (Id(0), "tst:TRICE64_2  %d %d\n", -1, -2 );
    break; case __LINE__: TRICE64_3 (Id(0), "tst:TRICE64_3  %d %d %d\n", -1, -2, -3 );
    break; case __LINE__: TRICE64_4 (Id(0), "tst:TRICE64_4  %d %d %d %d\n", -1, -2, -3, -4 );
    break; case __LINE__: TRICE64_5 (Id(0), "tst:TRICE64_5  %d %d %d %d %d\n", -1, -2, -3, -4, -5 );
    break; case __LINE__: TRICE64_6 (Id(0), "tst:TRICE64_6  %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6 );
    break; case __LINE__: TRICE64_7 (Id(0), "tst:TRICE64_7  %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7 );
    break; case __LINE__: TRICE64_8 (Id(0), "tst:TRICE64_8  %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8 );
    break; case __LINE__: TRICE64_9 (Id(0), "tst:TRICE64_9  %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9 );
    break; case __LINE__: TRICE64_10(Id(0), "tst:TRICE64_10 %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10 );
    break; case __LINE__: TRICE64_11(Id(0), "tst:TRICE64_11 %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11 );
    break; case __LINE__: TRICE64_12(Id(0), "tst:TRICE64_12 %d %d %d %d %d %d %d %d %d %d %d %d\n", -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12 );
    break; case __LINE__: TRICE64_12(Id(0), "tst:TRICE64_12 %t %b %x %X %d %u %o %O %p %e %f %g\n", 3, 3, 3, 3, 3, 3, 3, 3, 3, aDouble(3.14159), aDouble(3.14159), aDouble(3.14159));
    break; case __LINE__: TRICE( Id(0), "att:positive and negative float in format variants\n");
    break; case __LINE__: TRICE( Id(0), "rd:TRICE float %f (%%f)\n", aFloat(x) );
    break; case __LINE__: TRICE( Id(0), "rd:TRICE float %9f (%%9f)\n", aFloat(x) );
    break; case __LINE__: TRICE( Id(0), "rd:TRICE float %.9f (%%.9f)\n", aFloat(x) );
    break; case __LINE__: TRICE( Id(0), "rd:TRICE float %9.f (%%9.f)\n", aFloat(x) );
    break; case __LINE__: TRICE( Id(0), "rd:TRICE float %9.6f (%%9.6f)\n", aFloat(x) );
    break; case __LINE__: TRICE( Id(0), "rd:TRICE float %f (%%f)\n", aFloat(-x) );
    break; case __LINE__: TRICE( Id(0), "rd:TRICE float %9f (%%9f)\n", aFloat(-x) );
    break; case __LINE__: TRICE( Id(0), "rd:TRICE float %.9f (%%.9f)\n", aFloat(-x) );
    break; case __LINE__: TRICE( Id(0), "rd:TRICE float %9.f (%%9.f)\n", aFloat(-x) );
    break; case __LINE__: TRICE( Id(0), "rd:TRICE float %9.6f (%%9.6f)\n", aFloat(-x) );
    break; case __LINE__: TRICE( Id(0), "rd:TRICE float %+f (%%f)\n", aFloat(x) );
    break; case __LINE__: TRICE( Id(0), "rd:TRICE float %+9f (%%9f)\n", aFloat(x) );
    break; case __LINE__: TRICE( Id(0), "rd:TRICE float %+.9f (%%.9f)\n", aFloat(x) );
    break; case __LINE__: TRICE( Id(0), "rd:TRICE float %+9.f (%%9.f)\n", aFloat(x) );
    break; case __LINE__: TRICE( Id(0), "rd:TRICE float %+9.6f (%%9.6f)\n", aFloat(x) );
    break; case __LINE__: TRICE( Id(0), "rd:TRICE float %+f (%%f)\n", aFloat(-x) );
    break; case __LINE__: TRICE( Id(0), "rd:TRICE float %+9f (%%9f)\n", aFloat(-x) );
    break; case __LINE__: TRICE( Id(0), "rd:TRICE float %+.9f (%%.9f)\n", aFloat(-x) );
    break; case __LINE__: TRICE( Id(0), "rd:TRICE float %+9.f (%%9.f)\n", aFloat(-x) );
    break; case __LINE__: TRICE( Id(0), "rd:TRICE float %+9.6f (%%9.6f)\n", aFloat(-x) );
    break; case __LINE__: TRICE( Id(0), "rd:TRICE float %-f (%%f)\n", aFloat(x) );
    break; case __LINE__: TRICE( Id(0), "rd:TRICE float %-9f (%%9f)\n", aFloat(x) );
    break; case __LINE__: TRICE( Id(0), "rd:TRICE float %-.9f (%%.9f)\n", aFloat(x) );
    break; case __LINE__: TRICE( Id(0), "rd:TRICE float %-9.f (%%9.f)\n", aFloat(x) );
    break; case __LINE__: TRICE( Id(0), "rd:TRICE float %-9.6f (%%9.6f)\n", aFloat(x) );
    break; case __LINE__: TRICE( Id(0), "rd:TRICE float %-f (%%f)\n", aFloat(-x) );
    break; case __LINE__: TRICE( Id(0), "rd:TRICE float %-9f (%%9f)\n", aFloat(-x) );
    break; case __LINE__: TRICE( Id(0), "rd:TRICE float %-.9f (%%.9f)\n", aFloat(-x) );
    break; case __LINE__: TRICE( Id(0), "rd:TRICE float %-9.f (%%9.f)\n", aFloat(-x) );
    break; case __LINE__: TRICE( Id(0), "rd:TRICE float %-9.6f (%%9.6f)\n", aFloat(-x) );
    break; case __LINE__: TRICE( Id(0), "att:positive float & double in variants\n");
    break; case __LINE__: TRICE32_1( Id(0), "rd:TRICE32_1 float %e (%%e)\n", aFloat(x) );
    break; case __LINE__: TRICE32_1( Id(0), "rd:TRICE32_1 float %f (%%f)\n", aFloat(x) );
    break; case __LINE__: TRICE32_1( Id(0), "rd:TRICE32_1 float %g (%%g)\n", aFloat(x) );
    break; case __LINE__: TRICE32_1( Id(0), "rd:TRICE32_1 float %E (%%E)\n", aFloat(x) );
    break; case __LINE__: TRICE32_1( Id(0), "rd:TRICE32_1 float %F (%%F)\n", aFloat(x) );
    break; case __LINE__: TRICE32_1( Id(0), "rd:TRICE32_1 float %G (%%G)\n", aFloat(x) );
    break; case __LINE__: TRICE32( Id(0), "rd:TRICE32 float %e (%%e)\n", aFloat(x) );
    break; case __LINE__: TRICE32( Id(0), "rd:TRICE32 float %f (%%f)\n", aFloat(x) );
    break; case __LINE__: TRICE32( Id(0), "rd:TRICE32 float %g (%%g)\n", aFloat(x) );
    break; case __LINE__: TRICE32( Id(0), "rd:TRICE32 float %E (%%E)\n", aFloat(x) );
    break; case __LINE__: TRICE32( Id(0), "rd:TRICE32 float %F (%%F)\n", aFloat(x) );
    break; case __LINE__: TRICE32( Id(0), "rd:TRICE32 float %G (%%G)\n", aFloat(x) );
    break; case __LINE__: TRICE( Id(0), "rd:TRICE float %e (%%e)\n", aFloat(x) );
    break; case __LINE__: TRICE( Id(0), "rd:TRICE float %f (%%f)\n", aFloat(x) );
    break; case __LINE__: TRICE( Id(0), "rd:TRICE float %g (%%g)\n", aFloat(x) );
    break; case __LINE__: TRICE( Id(0), "rd:TRICE float %E (%%E)\n", aFloat(x) );
    break; case __LINE__: TRICE( Id(0), "rd:TRICE float %F (%%F)\n", aFloat(x) );
    break; case __LINE__: TRICE( Id(0), "rd:TRICE float %G (%%G)\n", aFloat(x) );
    break; case __LINE__: TRICE64( Id(0), "rd:TRICE64 double %e (%%e), aDouble(y)\n", aDouble(y) );
    break; case __LINE__: TRICE64( Id(0), "rd:TRICE64 double %f (%%f), aDouble(y)\n", aDouble(y) );
    break; case __LINE__: TRICE64( Id(0), "rd:TRICE64 double %g (%%g), aDouble(y)\n", aDouble(y) );
    break; case __LINE__: TRICE64( Id(0), "rd:TRICE64 double %E (%%E), aDouble(y)\n", aDouble(y) );
    break; case __LINE__: TRICE64( Id(0), "rd:TRICE64 double %F (%%F), aDouble(y)\n", aDouble(y) );
    break; case __LINE__: TRICE64( Id(0), "rd:TRICE64 double %G (%%G), aDouble(y)\n", aDouble(y) );
    break; case __LINE__: TRICE64_1( Id(0), "rd:TRICE64_1 double %e (%%e), aDouble(y)\n", aDouble(y) );
    break; case __LINE__: TRICE64_1( Id(0), "rd:TRICE64_1 double %f (%%f), aDouble(y)\n", aDouble(y) );
    break; case __LINE__: TRICE64_1( Id(0), "rd:TRICE64_1 double %g (%%g), aDouble(y)\n", aDouble(y) );
    break; case __LINE__: TRICE64_1( Id(0), "rd:TRICE64_1 double %E (%%E), aDouble(y)\n", aDouble(y) );
    break; case __LINE__: TRICE64_1( Id(0), "rd:TRICE64_1 double %F (%%F), aDouble(y)\n", aDouble(y) );
    break; case __LINE__: TRICE64_1( Id(0), "rd:TRICE64_1 double %G (%%G), aDouble(y)\n", aDouble(y) );
    break; case __LINE__: TRICE( Id(0), "att:negative float & double\n" );
    break; case __LINE__: TRICE( Id(0), "rd:TRICE float %e (%%e)\n", aFloat(-x) );
    break; case __LINE__: TRICE( Id(0), "rd:TRICE float %f (%%f)\n", aFloat(-x) );
    break; case __LINE__: TRICE( Id(0), "rd:TRICE float %g (%%g)\n", aFloat(-x) );
    break; case __LINE__: TRICE( Id(0), "rd:TRICE float %E (%%E)\n", aFloat(-x) );
    break; case __LINE__: TRICE( Id(0), "rd:TRICE float %F (%%F)\n", aFloat(-x) );
    break; case __LINE__: TRICE( Id(0), "rd:TRICE float %G (%%G)\n", aFloat(-x) );
    break; case __LINE__: TRICE64( Id(0), "rd:TRICE64 double %e (%%e), aDouble(y)\n", aDouble(-y) );
    break; case __LINE__: TRICE64( Id(0), "rd:TRICE64 double %f (%%f), aDouble(y)\n", aDouble(-y) );
    break; case __LINE__: TRICE64( Id(0), "rd:TRICE64 double %g (%%g), aDouble(y)\n", aDouble(-y) );
    break; case __LINE__: TRICE64( Id(0), "rd:TRICE64 double %E (%%E), aDouble(y)\n", aDouble(-y) );
    break; case __LINE__: TRICE64( Id(0), "rd:TRICE64 double %F (%%F), aDouble(y)\n", aDouble(-y) );
    break; case __LINE__: TRICE64( Id(0), "rd:TRICE64 double %G (%%G), aDouble(y)\n", aDouble(-y) );
    break; case __LINE__: TRICE( Id(0), "att:formatted float & double\n" );
    break; case __LINE__: TRICE32( Id(0), "rd:TRICE32 float %12.6e (%%12.6e)\n", aFloat(x) );
    break; case __LINE__: TRICE32( Id(0), "rd:TRICE32 float %12.6f (%%12.6f)\n", aFloat(x) );
    break; case __LINE__: TRICE32( Id(0), "rd:TRICE32 float %12.6g (%%12.6g)\n", aFloat(x) );
    break; case __LINE__: TRICE32( Id(0), "rd:TRICE32 float %12.6E (%%12.6E)\n", aFloat(x) );
    break; case __LINE__: TRICE32( Id(0), "rd:TRICE32 float %12.6F (%%12.6F)\n", aFloat(x) );
    break; case __LINE__: TRICE32( Id(0), "rd:TRICE32 float %12.6G (%%12.6G)\n", aFloat(x) );
    break; case __LINE__: TRICE64( Id(0), "rd:TRICE64 double %12.6e (%%12.6e), aDouble(y)\n", aDouble(y) );
    break; case __LINE__: TRICE64( Id(0), "rd:TRICE64 double %12.6f (%%12.6f), aDouble(y)\n", aDouble(y) );
    break; case __LINE__: TRICE64( Id(0), "rd:TRICE64 double %12.6g (%%12.6g), aDouble(y)\n", aDouble(y) );
    break; case __LINE__: TRICE64( Id(0), "rd:TRICE64 double %12.6E (%%12.6E), aDouble(y)\n", aDouble(y) );
    break; case __LINE__: TRICE64( Id(0), "rd:TRICE64 double %12.6F (%%12.6F), aDouble(y)\n", aDouble(y) );
    break; case __LINE__: TRICE64( Id(0), "rd:TRICE64 double %12.6G (%%12.6G), aDouble(y)\n", aDouble(y) );
    break; case __LINE__: TRICE( Id(0), "att:mixed int & float & double & bits\n" );
    break; case __LINE__: TRICE32( Id(0), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n", FloatToInt32(x), aFloat(x), aFloat(x), aFloat(x) );
    break; case __LINE__: TRICE32( Id(0), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",    (int32_t)(x), aFloat(x), aFloat(x), aFloat(x) );
    break; case __LINE__: TRICE32( Id(0), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",              x,  aFloat(x), aFloat(x), aFloat(x) );
    break; case __LINE__: TRICE32( Id(0), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n", FloatToInt32(-x), aFloat(-x), aFloat(-x), aFloat(-x) );
    break; case __LINE__: TRICE32( Id(0), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",    (int32_t)(-x), aFloat(-x), aFloat(-x), aFloat(-x) );
    break; case __LINE__: TRICE32( Id(0), "rd:TRICE32 int %d, float %f (%%f), %08x, %032b\n",              -x,  aFloat(-x), aFloat(-x), aFloat(-x) );
    break; case __LINE__: TRICE64( Id(0), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n", DoubleToInt64(y), aDouble(y), aDouble(y), aDouble(y) );
    break; case __LINE__: TRICE64( Id(0), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",     (int64_t)(y), aDouble(y), aDouble(y), aDouble(y) );
    break; case __LINE__: TRICE64( Id(0), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",               y,  aDouble(y), aDouble(y), aDouble(y) );
    break; case __LINE__: TRICE64( Id(0), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n", DoubleToInt64(-y), aDouble(-y), aDouble(-y), aDouble(-y) );
    break; case __LINE__: TRICE64( Id(0), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",     (int64_t)(-y), aDouble(-y), aDouble(-y), aDouble(-y) );
    break; case __LINE__: TRICE64( Id(0), "rd:TRICE64 int %d, double %f (%%f), %016x, %064b\n",               -y,  aDouble(-y), aDouble(-y), aDouble(-y) );
  //break; case __LINE__: TRICE( Id(0), "att:Various single arguments\n" );
  //break; case __LINE__: TRICE8( Id(0), "rd:TRICE8 line %t (%%t ,0)\n", 0 );
  //break; case __LINE__: TRICE8( Id(0), "rd:TRICE8 line %t (%%t ,2)\n", 2 );
  //break; case __LINE__: TRICE8( Id(0), "rd:TRICE8 line %u (%%u ,-1)\n", -1 );
  //break; case __LINE__: TRICE8( Id(0), "rd:TRICE8 line %b (%%b ,-1)\n", -1 );
  //break; case __LINE__: TRICE8( Id(0), "rd:TRICE8 line %o (%%o ,-1)\n", -1 );
  //break; case __LINE__: TRICE8( Id(0), "rd:TRICE8 line %O (%%O ,-1)\n", -1 );
  //break; case __LINE__: TRICE8( Id(0), "rd:TRICE8 line %X (%%X ,-1)\n", -1 );
  //break; case __LINE__: TRICE8( Id(0), "rd:TRICE8 line %x (%%x ,-1)\n", -1 );
  //break; case __LINE__: TRICE8( Id(0), "rd:TRICE8 line %d (%%d ,-1)\n", -1 );
  //break; case __LINE__: TRICE16_1( Id(0), "rd:TRICE16_1 line %t (%%t ,0)\n", 0 );
  //break; case __LINE__: TRICE16_1( Id(0), "rd:TRICE16_1 line %t (%%t ,2)\n", 2 );
  //break; case __LINE__: TRICE16_1( Id(0), "rd:TRICE16_1 line %u (%%u -1)\n", -1 );
  //break; case __LINE__: TRICE16_1( Id(0), "rd:TRICE16_1 line %b (%%b -1)\n", -1 );
  //break; case __LINE__: TRICE16_1( Id(0), "rd:TRICE16_1 line %o (%%o -1)\n", -1 );
  //break; case __LINE__: TRICE16_1( Id(0), "rd:TRICE16_1 line %x (%%x -1)\n", -1 );
  //break; case __LINE__: TRICE16_1( Id(0), "rd:TRICE16_1 line %d (%%d -1)\n", -1 );
    break; case __LINE__: TRICE( Id(0), "rd:TRICE line %t (%%t -1)\n", 0 );
    break; case __LINE__: TRICE( Id(0), "rd:TRICE line %t (%%t -1)\n", 2 );
    break; case __LINE__: TRICE( Id(0), "rd:TRICE line %u (%%u)\n", -1 );
    break; case __LINE__: TRICE( Id(0), "rd:TRICE line %b (%%b)\n", -2 );
    break; case __LINE__: TRICE( Id(0), "rd:TRICE line %o (%%o)\n", -3 );
    break; case __LINE__: TRICE( Id(0), "rd:TRICE line %x (%%x)\n", -4 );
    break; case __LINE__: TRICE( Id(0), "rd:TRICE line %d (%%d)\n", -5 );
    break; case __LINE__: TRICE_N( Id(0), "msg:%s\n", A, 0 );
    break; case __LINE__: TRICE_N( Id(0), "msg:%s\n", A, 1 );
    break; case __LINE__: TRICE_N( Id(0), "msg:%s\n", A, 2 );
    break; case __LINE__: TRICE_N( Id(0), "msg:%s\n", A, 3 );
    break; case __LINE__: TRICE_N( Id(0), "msg:%s\n", A, 4 );
    break; case __LINE__: TRICE_N( Id(0), "msg:%s\n", A, 5 );
    break; case __LINE__: TRICE_N( Id(0), "msg:%s\n", A, 6 );
    break; case __LINE__: TRICE_N( Id(0), "msg:%s\n", A, 7 );
    break; case __LINE__: TRICE_N( Id(0), "msg:%s\n", A, 8 );
    break; case __LINE__: TRICE_N( Id(0), "msg:%s\n", A, 9 );
    break; case __LINE__: TRICE_N( Id(0), "msg:%s\n", A, 10 );
    break; case __LINE__: TRICE_N( Id(0), "msg:%s\n", A, 11 );
    break; case __LINE__: TRICE_N( Id(0), "msg:%s\n", A, 12 );
    break; case __LINE__: TRICE_N( Id(0), "msg:%s\n", A, 13 );
    break; case __LINE__: TRICE_N( Id(0), "msg:%s\n", A, 14 );
    break; case __LINE__: TRICE_N( Id(0), "msg:%s\n", A, 15 );
    break; case __LINE__: TRICE_N( Id(0), "msg:%s\n", A, 16 );
    break; case __LINE__: TRICE_N( Id(0), "msg:%s\n", A, 17 );
    break; case __LINE__: TRICE_N( Id(0), "msg:%s\n", A, 18 );
    break; case __LINE__: TRICE_N( Id(0), "msg:%s\n", A, 19 );
    break; case __LINE__: TRICE_N( Id(0), "msg:%s\n", A, 120 );
    break; case __LINE__: TRICE_N( Id(0), "msg:%s\n", A, 121 );
    break; case __LINE__: TRICE_N( Id(0), "msg:%s\n", A, 122 );
    break; case __LINE__: TRICE_N( Id(0), "msg:%s\n", A, 123 );
    break; case __LINE__: TRICE_N( Id(0), "msg:%s\n", A, 124 );
    break; case __LINE__: TRICE_N( Id(0), "msg:%s\n", A, 125 );
    break; case __LINE__: TRICE_N( Id(0), "msg:%s\n", A, 126 );
    break; case __LINE__: TRICE_N( Id(0), "msg:%s\n", A, 127 );
    break; case __LINE__: TRICE_N( Id(0), "msg:%s\n", A, 128 );
    break; case __LINE__: TRICE_N( Id(0), "msg:%s\n", A, 129 );
    break; case __LINE__: exampleOfManualJSONencoding();
    break; case __LINE__: exampleOfBuffersAndFunctions();
    break; case __LINE__: exampleOfManualSerialization();
#endif
    break; case __LINE__: 
#ifdef XTEA_ENCRYPT_KEY
        TRICE0 (Id(0), "--------------------------------------------------\n" );
        {
            uint32_t by[8] = {1,2,3,4,5,6};
            int count = (sizeof(by) + 7) & ~7; // only multiple of 8 encryptable
            TRICE32(  Id(0), "msg: message = %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
            TRICE32( Id(0), "tim: pre TriceEncryption SysTick=%d\n", SYSTICKVAL );
            XTEAEncrypt(by, count>>2);
            TRICE32( Id(0), "tim: post TriceEncryption SysTick=%d\n", SYSTICKVAL );
            TRICE32(  Id(0), "att: TriceEncrypted =  %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
            #ifdef XTEA_DECRYPT
            TRICE16_1( Id(0), "tim: pre TriceDecryption SysTick=%d\n", SYSTICKVAL );
            XTEADecrypt(by, count>>2);
            TRICE16_1( Id(0), "tim: post TriceDecryption SysTick=%d\n", SYSTICKVAL );
            #endif
            TRICE32(  Id(0), "msg: messge = %08x %08x %08x %08x %08x %08x\n", by[0], by[1], by[2], by[3], by[4], by[5] );
        }
        TRICE0 (Id(0), "--------------------------------------------------\n" );
#endif

    break; case __LINE__: showTriceDepth();
        break;
        default:
        break;
    }
}




//! SCOPY is a helper macro for struct serialization.
#define SCOPY( element ) do{ char* n = #element; int size = sizeof( src->element ); memcpy( p, &(src->element), size ); p += size; TRICE_S( Id(0), "rd:sizeof(%8s)", n ); TRICE( Id(0), " = %d\n", size);}while(0);

    //! DCOPY is a helper macro for struct deserialization.
#define DCOPY( element ) do{ char* n = #element; int size = sizeof( dst->element ); memcpy( &(dst->element), p, size ); p += size; TRICE_S( Id(0), "rd:sizeof(%8s)", n ); TRICE( Id(0), " = %d\n", size);}while(0);


//  typedef struct{
//      float x;
//      float y;
//      uint8_t rgb[3];
//      //float z; // it seems, that the compiler does not align this with -o3 & time !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//  } Point_t; //!< Point_t is small struct type.
//  
//  static int serializePoint( char* dst, Point_t const * src ){
//      char * p = dst;
//  
//      SCOPY( x )
//      SCOPY( y )
//      SCOPY( rgb )
//  
//      return p - dst;
//  }
//  
//  static int deserializePoint( Point_t * const dst, char const * src ){
//      char const * p = src;
//  
//      DCOPY( x )
//      DCOPY( y )
//      DCOPY( rgb )
//      
//      return p - src;
//  }

typedef struct{
    float z;
    uint16_t u;
      int8_t s;
    char* addr;
    uint32_t x;
     int32_t y;
    char names[3][5];
    //Point_t point[2];
    uint64_t bitmask;
} Tryout_t; //!<  Tryout_t is a struct example embedding an other struct.



static int serializeTryout( char* dst, Tryout_t const * src ){
    char * p = dst;

    SCOPY( z )
    SCOPY( u )
    SCOPY( s )
    SCOPY( addr )
    SCOPY( x )
    SCOPY( y )
    SCOPY( names )
    //p += serializePoint( p, src->point );
    SCOPY( bitmask )  

    return p - dst;
}

static int deserializeTryout( Tryout_t * const dst, char const * src ){
    char const * p = src;

    DCOPY( z )
    DCOPY( u )
    DCOPY( s )
    DCOPY( addr )
    DCOPY( x )
    DCOPY( y )
    DCOPY( names )
    //p += deserializePoint( dst->point, p );
    DCOPY( bitmask )  

    return p - src;
}

static void exampleOfManualSerialization( void ){
    Tryout_t tx; // struct to transfer 
    Tryout_t rx; // "received" struct
    static char dst[100]; // serialized data
    char* src; // "copy" - assume, data transferred now
    int len; // serialized byte count
    
    /////////////////////////////////////////////////////////
    // fill tx with data
    tx.z = (float)123.456;
    tx.u = 44444;
    tx.addr="Haus";
    tx.s = -2;
    tx.x = 0xaa55bb77;
    tx.y = -1000000;

    memcpy( tx.names[0], "aaa", strlen( "aaa" ) ); 
    memcpy( tx.names[1], "bbbb", strlen( "bbbb" ) ); 
    memcpy( tx.names[2], "ccccc", strlen( "ccccc" ) ); 

    //  tx.point[0].x = 2.22;
    //  tx.point[0].y = -3.33;
    //  tx.point[0].rgb[0] = 0x44;
    //  tx.point[0].rgb[0] = 0x66;
    //  tx.point[0].rgb[0] = 0x88;
    //  
    //  tx.point[1].x = -66.66;
    //  tx.point[1].y = +5.5555;
    //  tx.point[1].rgb[0] = 0xee;
    //  tx.point[1].rgb[0] = 0xaa;
    //  tx.point[1].rgb[0] = 0xbb;
    
    tx.bitmask = 0xAAAA55550000FFFF;
    //
    ////////////////////////////////////////////////////////
    
    len = serializeTryout( dst, &tx );
    TRICE ( Id(0), "inf: Tryout tx struct:" );
    TRICE_B( Id(0), " %02x ", &tx, sizeof(tx) );
    TRICE( Id(0), "\n" );
    
    TRICE ( Id(0), "inf: Tryout buffer:" );
    TRICE_B( Id(0), " %02x ", dst, len ); //lint !e670
    TRICE( Id(0), "\n" );

    src = dst; // "data transfer"
    
    len = deserializeTryout( &rx, src );
    TRICE ( Id(0), "inf: Tryout rx struct:" );
    TRICE_B( Id(0), " %02x ", &rx, sizeof(rx) );
    TRICE( Id(0), "\n" );

    TRICE( Id(0), "inf:sizeOf(Tryout) = %d, buffer length = %d\n", sizeof(tx), len );
    TRICE8_F( Id(0), "info:TryoutStructFunction", &tx, sizeof(tx) );
    TRICE8_F( Id(0), "info:TryoutBufferFunction", dst, len ); //lint !e670
}

static void exampleOfManualJSONencoding(void){
    typedef  struct {
    int Apple, Birn;
    float Fish;
    } Ex_t;
    Ex_t Ex = { -1, 2, (float)2.781 };
    TRICE( Id(0), "att:MyStructEvaluationFunction(json:ExA{Apple:%d, Birn:%u, Fisch:%f}\n", Ex.Apple, Ex.Birn, aFloat(Ex.Fish) );
}

static void exampleOfBuffersAndFunctions(void){
    static int8_t   b8[24] = { 0, -1, -2, 0x33, 4, 5, 6, 7, 8, 9, 10, 11, 0, -1, -2, 0x33, 4, 5, 6, 7, 8, 9, 10, 11 };
    static int16_t b16[] = { 0, -1, -2, 0x3344 };
    static int32_t b32[] = { 0, -1, -2, 0x33445555};
    static int64_t b64[4] = { 0, -1, -2, 0x3344555566666666 };
    char* s = "abcde 12345";
    uint32_t len;

    TRICE_S( Id(0), "msg:With TRICE_S:%s\n", s );
    len = strlen(s);
    TRICE_N( Id(0), "sig:With TRICE_N:%s\n", s, len );
    TRICE32( Id(0), "att:len=%u:With TRICE_B:\n", len);
    TRICE_B( Id(0), "  %02x", s, len );
    TRICE( Id(0), "\n" );
    TRICE_B( Id(0), "%4d", s, len );
    TRICE( Id(0), "\n" );

    TRICE( Id(0), "notice:TRICE_B example: " );
    TRICE_B( Id(0), "  %02x", b8, sizeof(b8)/sizeof(int8_t) );
    TRICE( Id(0), "\n" );
    TRICE( Id(0), "notice:TRICE8_B example:" );
    TRICE8_B( Id(0), "  %02x", b8, sizeof(b8)/sizeof(int8_t) );
    TRICE( Id(0), "\n" );
    TRICE( Id(0), "notice:TRICE16_B example:" );
    TRICE16_B( Id(0), "  %04x", b16, sizeof(b16)/sizeof(int16_t) );
    TRICE( Id(0), "\n" );
    TRICE( Id(0), "notice:TRICE32_B example:" );
    TRICE32_B( Id(0), "  %08x", b32, sizeof(b32)/sizeof(int32_t) );
    TRICE( Id(0), "\n" );
    TRICE( Id(0), "notice:TRICE64_B example:" );
    TRICE64_B( Id(0), "  %016x", b64, sizeof(b64)/sizeof(int64_t) );
    TRICE( Id(0), "\n" );    

    TRICE( Id(0), "notice:TRICE_F example: " );
    TRICE_F( Id(0), "info:FunctionNameW",   b8,  sizeof(b8) /sizeof(int8_t) );
    //shown as: TRICE_F example: FunctionNameW(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)
    
    TRICE( Id(0), "notice:TRICE8_F example:" );
    TRICE8_F(  Id(0), "info:FunctionNameW",   b8,  sizeof(b8) /sizeof(int8_t) );
    //shown as: TRICE8_F example:FunctionNameW(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)(00)(ff)(fe)(33)(04)(05)(06)(07)(08)(09)(0a)(0b)
    
    TRICE( Id(0), "notice:TRICE16_F example:" );
    TRICE16_F( Id(0), "sig:FunctionNameX",    b16, sizeof(b16)/sizeof(int16_t) );
    //shown as: TRICE16_F example:FunctionNameX(0000)(ffff)(fffe)(3344) 

    TRICE( Id(0), "notice:TRICE32_F example:" );
    TRICE32_F( Id(0), "diag:FunctionNameY",   b32, sizeof(b32)/sizeof(int32_t) );
    //shown as: TRICE32_F example:FunctionNameY(00000000)(ffffffff)(fffffffe)(33445555)

    TRICE( Id(0), "notice:TRICE64_F example:" );
    TRICE64_F( Id(0), "fatal:FunctionNameZ", b64, sizeof(b64)/sizeof(int64_t) );
    //shown as: TRICE64_F example:FunctionNameZ(0000000000000000)(ffffffffffffffff)(fffffffffffffffe)(3344555566666666)
}

static int32_t FloatToInt32( float f ){
    if( f >= 0 ){
        return (int32_t)f;
    }
    return -(int32_t)-f;
}

static int64_t DoubleToInt64( double f ){
    if( f >= 0 ){
        return (int64_t)f;
    }
    return -(int64_t)-f;
}


#ifdef TRICE_LOG_OVER_MODBUS_FUNC24_ALSO

//! TriceLogModbusBufferInfo todo: into -> triceCheck.c
void TriceLogModbusBufferInfo( void ){
    TRICE32( Id(0), "att: Trice modbus buffer size:%5u ", TRICE_MODBUS_BUFFER_SIZE );
}

#endif

void TriceLogDepthMax( void ){
    size_t tdm = TriceDepthMax();
    if( tdm <= TRICE_HALF_BUFFER_SIZE ){
        TRICE16( Id(0),"diag:TriceDepthMax =%4u of %d\n", tdm, TRICE_HALF_BUFFER_SIZE );
    }else{
        TRICE16( Id(0),"err:TriceDepthMax =%4u of %d (overflow!)\n", tdm, TRICE_HALF_BUFFER_SIZE );
    }
}
